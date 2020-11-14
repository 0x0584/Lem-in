/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   netflow.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: archid- <archid-@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/23 19:06:16 by archid-           #+#    #+#             */
/*   Updated: 2020/11/14 15:35:10 by archid-          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../lem_in.h"

#define NIL_ANT									(size_t)-1

t_qnode		*nil_ant(void)
{
	size_t ant;

	ant = NIL_ANT;
	return queue_node(&ant, sizeof(size_t));
}

bool		is_nilant(t_qnode *e)
{
	if (!e || *QNODE_AS(size_t, e) != NIL_ANT)
		return false;
	return true;
}

t_flow		flow_nil()
{
	t_flow nil;

	nil.path = NULL;
	nil.latency = (unsigned)-1;
	nil.cut = false;
	return (nil);
}

t_flow		flow_init(t_queue *path)
{
	t_flow		flow;
	size_t		latency;
	t_qnode		*node;

	if (!path || !(latency = queue_size(path)))
		return flow_nil();
	flow.latency = latency;
	flow.path = (t_edge **)malloc(latency * sizeof(t_edge *));
	flow.ants = queue_init();
	while (latency--)
	{
		node = queue_deq(path);
		flow.path[latency] = node->blob;
		queue_node_del(&node, queue_node_del_dry);
		queue_enq(flow.ants, nil_ant());
	}
	flow.cut = false;
	flow.n_synced = 0;
	return (flow);
}

bool	flow_has_ants(t_flow *flow)
{
	t_qnode *walk;

	walk = QFIRST(flow->ants);
	while (walk != QTAIL(flow->ants))
	{
		if (!is_nilant(walk))
			return (true);
		walk = walk->next;
	}
	return (false);
}

/* this alters the priority of each node */
bool	lower_latency(t_qnode *in, t_qnode *with)
{
	t_flow *f1;
	t_flow *f2;

	if (!in || !with || !(f1 = in->blob) || !(f2 = with->blob))
		return (false);
	if (f1->latency > f2->latency)
		return true;
	return false;
}

/* mask is at max SIGNED_LONG_MIN to avoid overfow */
static void		ft_putbits(unsigned long chunk, unsigned long mask)
{
	unsigned long walk;

	walk = (mask + 1);
	while (walk)
	{
		walk >>= 1;
		if(walk > 0)
			ft_putchar(walk & chunk ? '1' : '0');
	}
}

	/*
	   should avoid possible overflow, shound be a bigint array, where we
	   know the size of bits there

	   given array A[] of N integers of 32 bit, then we have at total N * 32 bits,
	   if a number n has m bits, then at least (m / 32 bits + m % 32 bits),
	   or array of (m / 32 + (m % 32)),

	   this representation is suitable for th kind of support the flow have,
	   since we only need to maintain a current going, so not much to do with
	   bits that are within.

	   check is always done starting from [n-1], since the current is going from
	   [0] towards [N-1]
	*/

void			flow_log_ants(t_flow *f)
{
	t_qnode *walk;
	size_t	size;
	size_t i;

	walk = QHEAD(f->ants)->next;
	size = queue_size(f->ants);
	i = 0;
	while (walk != QTAIL(f->ants))
	{
		if (!is_nilant(walk))
			ft_printf("L%zu-%s ", *QNODE_AS(size_t, walk),
					  f->path[f->cut ? f->latency - i - 1
							  : size - i - 1]->node_dst->name);
		i++;
		walk = walk->next;
	}
}

void			flow_dump(t_qnode *e)
{
	t_flow	*flow;
	char	*cut;

	if (!e)
		return ;
	flow = e->blob;
	flow_log_ants(flow);
}

void			netflow_log(t_netflow *net)
{
	if (!net)
		return ;
	queue_iter(net->flows, false, flow_dump);
	queue_iter(net->sync, false, flow_dump);
}

static t_netflow		*netflow_init(t_queue *paths)
{
	t_netflow		*net;
	size_t			n_paths;
	t_qnode			*path;
	t_flow			flow;

	if (!paths || !(n_paths = queue_size(paths)) ||
			!(net = (t_netflow *)malloc(sizeof(t_netflow))))
		return (NULL);
	net->flows = queue_init();
	net->sync = queue_init();
	/* setting them back from the source to the sink */
	while (queue_size(paths))
	{
		path = queue_deq(paths);
		flow = flow_init(path->blob);
		queue_penq(net->flows, queue_node(&flow, sizeof(t_flow)),
					lower_latency);
		queue_node_del(&path, queue_node_del_dry);
	}
	return (net);
}

t_netflow	*netflow_setup(t_graph *graph, size_t units)
{
	t_queue		*paths;
	t_queue		*tmp;
	t_netflow	*net;

	paths = queue_init();
	while ((tmp = bfs_find(graph)))
		queue_enq(paths, queue_dry_node(tmp, sizeof(t_queue *)));
	if (!queue_size(paths))
		return NULL;
	net = netflow_init(re_wire_paths(graph, paths));
	net->n_units = units;
	net->maxflow = units;
	queue_del(&paths, queue_node_del_dry);
	netflow_log(net);
	return (net);
}

void		queue_flow_del(void *blob, size_t size)
{
	t_flow *f1;

	if (!blob || !size)
		return ;
	f1 = blob;
	free(f1->path);
	free(f1);
}

void		netflow_del(t_netflow **anet)
{

	if (!anet || !*anet)
		return ;
	queue_del(&(*anet)->flows, queue_flow_del);
	queue_del(&(*anet)->sync, queue_flow_del);
	free(*anet);
	*anet = NULL;
}



/*
   to name the bits raveling the network, since all flows are sorted based on
   latency, then the ants would be like

   if n_synced > latency, we have `latency' of ants as
    (index + n_synced at once)
   else n_synced - index

 */

# define MAX(a, b)					(MIN(b, a) ? (a) : (b))

void		set_flow_cut(t_qnode *e)
{
	if (!e)
		return ;
	QNODE_AS(struct s_flow, e)->cut = true;
}

size_t		netflow_shrink(t_netflow *net)
{
	t_qnode *walk;
	t_qnode *prev;


	if (QHEAD(net->flows)->next == QTAIL(net->flows))
		return (0);
	walk = QLAST(net->flows);
	net->maxflow = QNODE_AS(struct s_flow, walk)->latency;
	/* looking for sigma set */
	while (net->n_units && walk != QHEAD(net->flows))
	{
		prev = walk;
		if ((walk = walk->prev) == QHEAD(net->flows))
			break ;
		/* FIXME: find a better condition */
		if (net->n_units >= QNODE_AS(struct s_flow, walk)->latency
				|| (QNODE_AS(struct s_flow, walk)->latency
						== QNODE_AS(struct s_flow, prev)->latency
					&& net->n_units - 1))
		{
			net->maxflow += QNODE_AS(struct s_flow, walk)->latency;
			continue ;
		}
		while ((prev = queue_deq(net->flows)))
		{
			QNODE_AS(struct s_flow, prev)->cut = true;
			queue_penq(net->sync, prev, lower_latency);
			if (prev == walk)
				break;
		}
		break;
	}
	if (!net->n_units)
	{
		queue_iter(net->flows, true, set_flow_cut);
		while (queue_size(net->flows))
			queue_penq(net->sync, queue_deq(net->flows), lower_latency);
	}
	/* netflow_log(net); */
	return net->maxflow;
}

static bool cut_flow(t_flow *flow)
{
	t_qnode *tmp;

	if (!flow || !flow->cut)
		return false;
	/* no more ants are going in */
	if (!flow_has_ants(flow))
		return (true);
	/* sync the ant only if !NIL_ANT */
	if (!is_nilant(tmp = queue_deq(flow->ants)))
		flow->n_synced++;
	queue_node_del(&tmp, queue_del_helper);
	return false;
}

bool		is_saturated(t_flow *flow)
{
	size_t count;
	t_qnode *walk;

	count = 0;
	walk = QFIRST(flow->ants);
	while (walk != QTAIL(flow->ants))
	{
		count += !is_nilant(walk);
		walk = walk->next;
	}
	return (count == flow->latency);
}

static bool sync_flow(size_t ant, t_flow *flow)
{
	t_qnode *tmp;

	if (flow->cut)
		return !cut_flow(flow);
	if (is_saturated(flow))
		flow->n_synced++;
	/* dequeue arriving ant, enqueue the new ant */
	tmp = queue_deq(flow->ants);
	queue_node_del(&tmp, queue_del_helper);
	queue_enq(flow->ants, queue_node(&ant, sizeof(size_t)));
	return (true);
}

bool		netflow_sync(t_netflow *net)
{
	static size_t n_sent = 0;
	t_qnode *walk;
	int		turn;
	bool	sync_in;
	bool	sync_out;

	turn = 0;
	walk = net->flows->tail->prev;
	sync_in = false;
	while (net->n_units && walk != net->flows->head)
	{
		sync_in = true;
		net->n_units--;
		sync_flow(n_sent++, QNODE_AS(struct s_flow, walk));
		walk = walk->prev;
	}
	netflow_shrink(net);
	sync_out = false;
	walk = net->sync->head->next;
	while (walk != net->sync->tail)
	{
		if (sync_flow(NIL_ANT, QNODE_AS(struct s_flow, walk)))
			sync_out = true;
		walk = walk->next;
	}
	if (!sync_in && !sync_out)
		n_sent = 0;				/* all bits have arrived */
	return (sync_out || sync_in);
}

void		netflow_pushflow(t_netflow *net)
{
	bool state;

	state = true;
	while (state)
	{
		netflow_shrink(net);
		state = netflow_sync(net);
		netflow_log(net);
		ft_putendl("");
	}
}
