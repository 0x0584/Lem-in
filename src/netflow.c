/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   netflow.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: archid- <archid-@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/23 19:06:16 by archid-           #+#    #+#             */
/*   Updated: 2020/11/15 20:29:31 by archid-          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../lem_in.h"

int paths_cmp(t_qnode *path1, t_qnode *path2) {
	t_queue *p1;
	t_queue *p2;

	p1 = path1->blob;
	p2 = path2->blob;
	return queue_size(p2) - queue_size(p1);
}

void init_path(t_path *path, t_queue *edges)
{
	t_qnode	*tmp;
	size_t	i;

	i = 0;
	path->size = queue_size(edges);
	path->nodes = malloc(path->size * sizeof(char *));
	while (i < path->size)
	{
		tmp = queue_pop(edges);
		path->nodes[i] = AS_EDGE(tmp)->dst->name;
		queue_node_del(&tmp, queue_node_del_dry);
		i++;
	}
}

static t_netflow		*netflow_init(t_queue *paths)
{
	t_netflow	*net;
	t_qnode		*tmp;
	size_t		size;

	size = queue_size(paths);
	net = malloc(sizeof(t_netflow));
	net->paths = malloc(size * sizeof(t_path));
	net->n_paths = size;
	while (size--)
	{
		tmp = queue_deq(paths);
		init_path(&net->paths[size], tmp->blob);
		queue_node_del(&tmp, queue_node_del_dry);
	}
	return (net);
}

t_netflow				*netflow_setup(t_graph *graph, size_t units)
{
	t_queue		*paths;
	t_queue		*tmp;
	t_netflow	*net;

	paths = queue_init();
	while ((tmp = bfs_find(graph)))
		queue_enq(paths, queue_dry_node(tmp, sizeof(t_queue *)));
	if (!queue_size(paths))
		return NULL;
	queue_mergesort(&paths, paths_cmp);
	net = netflow_init(re_wire_paths(graph, paths));
	net->n_units = units;
	net->maxflow = 0;
	queue_del(&paths, queue_node_del_dry);

	for (int i = 0; i < net->n_paths; ++i) {
		ft_printf("path %d has size %zu: ", i, net->paths[i].size);

		for (int j = 0; j < net->paths[i].size; ++j) {
			ft_printf("%s ", net->paths[i].nodes[j]);
		}
		ft_printf("\n");
	}

	return (net);
}

void		netflow_del(t_netflow **anet)
{
}

void		netflow_pushflow(t_netflow *net)
{
}
