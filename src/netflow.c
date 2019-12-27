/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   netflow.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: archid- <archid-@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/23 19:06:16 by archid-           #+#    #+#             */
/*   Updated: 2019/12/27 18:08:33 by archid-          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../lem_in.h"

t_flow		flow_nil()
{
	t_flow nil;

	nil.path = NULL;
	nil.latency = (unsigned)-1;
	nil.current = 0;
	nil.cut = false;
	return (nil);
}

t_flow		flow_init(t_queue *path)
{
	t_flow		p;
	size_t		latency;
	t_qnode		*node;

	if (!path || !(latency = queue_size(path)))
		return flow_nil();
	p.latency = latency;
	p.path = (t_edge **)malloc(latency * sizeof(t_edge *));
	p.current = 0U;
	p.cmask = 0U;
	while (latency--)
	{
		if (p.cmask)
			p.cmask <<= 1;
		p.cmask |= 1;
		node = queue_deq(path);
		p.path[latency] = node->blob;
		queue_node_del(&node, queue_node_del_dry);
	}
	p.cut = false;
	p.n_arrived = 0;
	return (p);
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

# define FLOW_SATURATED(f)				 ((f->current & f->cmask) == f->cmask)

void			flow_dump(t_qnode *e)
{
	t_flow *flow;
	unsigned i;
	unsigned index;
	char *cut;

	if (!e)
		return ;
	flow = e->blob;
	cut = flow->cut ? "C": " ";
	if (flow->cut && (flow->current & flow->cmask) == 0)
		cut = "X";
	ft_printf(" >> [%s] flow of %u / %zu ",
			  cut, flow->latency, flow->n_arrived);
	ft_putstr("mask: ");
	ft_putbits(flow->cmask, flow->cmask);
	ft_putstr(" current: ");
	ft_putbits(flow->current, flow->cmask);
	ft_putchar('\n');
	i = 0;
	while (i < flow->latency)
	{
		/* sync-in */
		if (!FLOW_SATURATED(flow) && (flow->current & (1 << i)))
		{
			ft_printf(" v L%d-%s using <%s, %s>\n",
					  i, flow->path[i]->node_dst->name,
					  flow->path[i]->node_src->name,
					  flow->path[i]->node_dst->name);
		}
		/* sync */
		else if (FLOW_SATURATED(flow))
			ft_printf(" ^ L%d-%s using <%s, %s>\n",
					  /* add latency */
					  (flow->n_arrived) + (flow->latency - i - 1),
					  flow->path[flow->latency - i - 1]->node_dst->name,
					  flow->path[flow->latency - i - 1]->node_src->name,
					  flow->path[flow->latency - i - 1]->node_dst->name);

		/* sync-out */
		else
		{
			/* i++; */
		    /* if (((flow->current >> i) & ((flow->cmask >> i) + 1))) */
			/* { */
			/* 	ft_printf(" v L%d-%s using <%s, %s>\n", */
			/* 			  i, flow->path[flow->latency - i - 1]->node_dst->name, */
			/* 			  flow->path[flow->latency - i - 1]->node_src->name, */
			/* 			  flow->path[flow->latency - i - 1]->node_dst->name); */
			/* } */
			/* continue ; */
		}
		i++;
	}

	ft_putendl("\n");
}

void			netflow_log(t_netflow *net)
{
	if (!net)
		return ;
	ft_printf(" ====== network has %zu over %zu flow(s) =====\n"
				" // flows based on lower latency\n",
			  net->n_units, queue_size(net->flows));

	queue_iter(net->flows, false, flow_dump);
	queue_iter(net->sync, false, flow_dump);
	ft_putendl(" ============ //// =================================== ");
	getchar();
}

t_netflow		*netflow_init(t_queue *paths)
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
	{
		ft_putendl(" path:  \n ");
		queue_iter(tmp, true, edge_dump);
		queue_iter(tmp, false, edge_dump); /* from tail: source -> sink */
		queue_enq(paths, queue_dry_node(tmp, sizeof(t_queue *)));
	}
	if (!queue_size(paths))
	{
		ft_putendl("no paths were found!");
		return NULL;
	}
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

   if n_arrived > latency, we have `latency' of ants as
    (index + n_arrived at once)
   else n_arrived - index

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
	t_qnode *tmp;
	size_t  qsize;

	qsize = queue_size(net->flows);
	net->maxflow = MIN(qsize, net->n_units);
	walk = net->flows->head->next;
	/* looking for sigma set */
	while (net->n_units && walk != net->flows->tail)
	{
		tmp = walk;

		if ((walk = walk->next) == net->flows->tail)
			break ;

		/*
		   which one is better? two ants over two paths of 4 or one of 4
		   the answer is two over rwo.
		*/

		if (qsize - net->maxflow ||
				QNODE_AS(struct s_flow, tmp)->latency == (unsigned)-1)
		{
			tmp->prev->next = tmp->next;
			tmp->next->prev = tmp->prev;
			QNODE_AS(struct s_flow, tmp)->cut = true;
			queue_penq(net->sync, tmp, lower_latency);
			qsize--;
		}
	}
	if (!net->n_units)
	{
		queue_iter(net->flows, true, set_flow_cut);
		while (queue_size(net->flows))
			queue_penq(net->sync, queue_deq(net->flows), lower_latency);
	}
	ft_printf("maxflow is: %zu\n", net->maxflow);
	/* netflow_log(net); */
	return net->maxflow;
}

static bool cut_flow(t_flow *f)
{
	if (!f || !f->cut)
		return false;
	if (!(f->current & f->cmask))
		return (true);
	f->current <<= 1;
	f->n_arrived++;
	return false;
}

static bool sync_flow(t_flow *f)
{
	if (FLOW_SATURATED(f) && !f->cut)
		f->n_arrived++;
	else if (f->cut)
		return !cut_flow(f);
	else						/* still loading */
	{
		if (!f->current)
			f->current = 1;
		else if (!FLOW_SATURATED(f))
		{
			f->current <<= 1;
			f->current |= 1;
		}
	}
	return true;
}

bool		netflow_sync(t_netflow *net)
{
	t_qnode *walk;
	int		turn;
	bool	sync_in;
	bool	sync_out;

	turn = 0;
	walk = net->flows->tail->prev;
	sync_in = false;
	while (net->n_units && walk != net->flows->head)
	{
		net->n_units--;
		sync_flow(QNODE_AS(struct s_flow, walk));
		ft_printf("current of flow (%d) is %d\n",
				  turn++,
				  QNODE_AS(struct s_flow, walk)->current);
		sync_in = true;
		walk = walk->prev;
	}
	if (net->n_units < net->maxflow)
		netflow_shrink(net); /* optimal netflow / n_unites */
	if (sync_in)
	{
		ft_putendl("\n ====== after pushing ===== ");
		netflow_log(net);
	}
	sync_out = false;
	walk = net->sync->head->next;
	while (walk != net->sync->tail)
	{
		if (sync_flow(QNODE_AS(struct s_flow, walk)))
		{
			ft_putendl("sync!");
			sync_out = true;
		}
		walk = walk->next;
	}
	if (sync_out)
	{
		ft_putendl("\n ====== after syncing ===== ");
		netflow_log(net);
	}
	return (sync_out || sync_in);
}

void		netflow_pushflow(t_netflow *net)
{
	size_t allflows;
	bool state;

	ft_printf("number of ants: %lu\n", net->n_units);
	allflows = queue_size(net->flows);
	state = true;
	while (state)
	{
		if (net->maxflow > queue_size(net->flows))
			netflow_shrink(net); /* optimal netflow / n_unites */
		state = netflow_sync(net);

	}
}
