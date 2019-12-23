/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   netflow.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: archid- <archid-@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/23 19:06:16 by archid-           #+#    #+#             */
/*   Updated: 2019/12/23 23:24:56 by archid-          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../lem_in.h"

t_flow		flow_nil()
{
	t_flow nil;

	nil.path = NULL;
	nil.n_edges = 0;
	nil.current = 0;
	nil.blocked = false;
	return (nil);
}

t_flow		flow_init(t_queue *path)
{
	t_flow		p;
	size_t		n_edges;
	t_qnode		*node;

	if (!path || !(n_edges = queue_size(path)))
		return flow_nil();
	p.n_edges = queue_size(path);
	p.path = (t_edge **)malloc(n_edges * sizeof(t_edge *));
	p.mask = 0;
	while (n_edges--)
	{
		if (p.mask)
			p.mask <<= 1;
		p.mask |= 1;
		node = queue_deq(path);
		p.path[n_edges] = node->blob;
		queue_node_del(&node, queue_node_del_dry);
	}
	p.blocked = false;
	p.current = 0;
	return (p);
}

/* capacity is the length, n_edges */
void		sort_based_on_capacity(t_flow *flows)
{

}

t_netflow		*flow_network_init(t_queue *paths)
{
	t_netflow		*net;
	size_t			n_paths;
	t_qnode			*path;

	if (!paths || !(n_paths = queue_size(paths)) ||
			!(net = (t_netflow *)malloc(sizeof(t_netflow))))
		return (NULL);
	net->n_flows = n_paths;
	net->flows = (t_flow *)malloc(sizeof(t_netflow) * n_paths);
	/* setting them back from the source to the sink */
	while (n_paths--)
	{
		path = queue_deq(paths);
		net->flows[n_paths] = flow_init(path->blob);
		queue_node_del(&path, queue_node_del_dry);
	}
	sort_based_on_capacity(&net->flows[n_paths]);
	net->optimal = net->flows[0];
	return (net);
}


t_netflow	*flow_network_setup(t_graph *graph, size_t units)
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
	re_wire_paths(graph, paths);
	net = flow_network_init(paths);
	net->n_units = units;
	queue_del(&paths, queue_node_del_dry);
	return (net);
}

void		flow_network_del(t_netflow **anet)
{

	size_t i;

	if (!anet || !*anet)
		return ;
	i = 0;
	while (i < (*anet)->n_flows)
		free ((*anet)->flows[i++].path);
	free((*anet)->flows);
	free(*anet);
	*anet = NULL;
}

void		flow_log(t_flow f)
{
	/* TODO: this simulate the action of
	 * pomp.current = (pomp.current << 1) & !pomp.blocked;
	 *
	 * probably gonna need to turn path into an array in order
	 * to print efficiently
	*/

}

void		push_flow_through(t_netflow *net)
{
	t_flow		*pomp;
	unsigned	turn;
	unsigned	n_blocked;

	n_blocked = 0;
	ft_printf("number of ants: %lu\n", net->n_units);
	while (net->n_units)
	{
		/* if only one path is remaining, use the shortest to send remaining units */
		// ft_printf("push everything left from the shortest path(%u)\n", turn);
		while ((n_blocked == net->n_flows) && net->n_units)
		{
			net->optimal.current = (((net->optimal.current << 1) | 1)
										& net->optimal.mask);
			net->n_units += (!net->n_units ? 0 : -1);
			flow_log(net->optimal);

			char *tmp;
			tmp = ft_utoa_base(net->optimal.current, "01");
			ft_printf(" (%s)\n", tmp);
			ft_printf("remaining ants: %lu\n", net->n_units);
			free(tmp);

			getchar();
			if (!net->n_units)
				break ;

		}
		/* pushing flow through teh network */
		turn = 0;
		while (turn < net->n_flows)
		{
			pomp = &net->flows[turn];
			if (net->n_units < net->flows[turn].n_edges)
			{
				ft_printf("path(%u) is blocked: still got %lu ants\n",
							turn, net->n_units);
				net->flows[turn].blocked = true;
				n_blocked++;
				turn++;
				continue ;
			}
			char *tmp;


			pomp->current = (((pomp->current << 1) | !pomp->blocked) & pomp->mask);
			net->n_units += (!net->n_units ? 0 : -1);

			char	buff[sizeof(unsigned) * 8 + 1];
			short rem_bits;

			rem_bits = MIN(sizeof(unsigned) * 8 - ft_strlen(tmp), pomp->n_edges);

			buff[rem_bits] = '\0';
			while (rem_bits--)
				buff[rem_bits] = '0';

			tmp = ft_utoa_base(pomp->current, "01");
			ft_printf("path(%u) flow (%s)\n", turn, tmp);
			ft_printf("remaining ants: %lu\n", net->n_units);
			free(tmp);

			turn++;
			getchar();
		}
	}
}
