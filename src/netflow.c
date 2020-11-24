/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   netflow.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: archid- <archid-@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/23 19:06:16 by archid-           #+#    #+#             */
/*   Updated: 2020/11/24 03:43:39 by archid-          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lem_in.h"

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
	path->ants = malloc(path->size * sizeof(int));
	while (i < path->size)
	{
		tmp = queue_pop(edges);
		path->nodes[i] = ft_strdup(AS_EDGE(tmp)->dst->name);
		path->ants[i]  = -1;
		queue_node_del(&tmp, queue_blob_keep);
		i++;
	}
}

t_netflow		*netflow_init(t_queue *paths)
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
		queue_node_del(&tmp, queue_blob_keep);
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
		queue_enq(paths, queue_node(tmp, sizeof(t_queue *), false));
	if (!queue_size(paths))
		return NULL;
	queue_mergesort(&paths, paths_cmp);
	net = netflow_init(re_wire_paths(graph, paths));
	net->n_units = units;
	net->maxflow = 0;
	queue_del(&paths, queue_blob_keep);

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
	size_t i;
	size_t j;

	i = 0;
	while (i < (*anet)->n_paths) {
		j = 0;
		while (j < (*anet)->paths[i].size) {
			free((*anet)->paths[i].nodes[j]);
			j++;
		}
		free((*anet)->paths[i].nodes);
		i++;
	}
	free((*anet)->paths);
	*anet = NULL;
}

int netflow_send(t_netflow *net, int n_paths) {
	int ant;
	int i;
	int j;
	int count;
	bool flag = true;

	count = 0;
	ant = 0;
	while (flag) {
		flag = false;
		i = 0;
		while (ant < net->n_units && i < n_paths)
			net->paths[i++].ants[0] = ant++ + 1;
		i = 0;
		while (i < n_paths) {
			j = net->paths[i].size - 1;
			while (j) {
				if (net->paths[i].ants[j - 1] != -1)
					flag = true;
				net->paths[i].ants[j] = net->paths[i].ants[j - 1];
				j--;
			}
			net->paths[i].ants[0] = -1;
			i++;
		}
		count++;
	}

	return count;
}

int netflow_simulate(t_netflow *net) {
	int i = 0;
	int n_used = 0;
	int prev = INT_MAX;
	int tmp;

	while (n_used < net->n_paths) {
		tmp = netflow_send(net, n_used + 1);
		printf("prev: %d current: %d\n", prev, tmp);
		if (tmp > prev)
			break;
		prev = tmp;
		n_used++;
	}
	printf("\nused: %d\n", n_used);

	int sum;
	sum = 0;
	while (i < n_used)
	{
		net->paths[i].n_ants = (net->paths[n_used - 1].size - net->paths[i].size);
		sum += net->paths[i].n_ants;
		i++;
	}

	printf("sum: %d\n", sum);

	if (net->n_units - sum > 0)
	{
		int mod = (net->n_units - sum) % n_used;
		int div = (net->n_units - sum) / n_used;

		i = 0;
		while (i < n_used)
		{
			net->paths[i].n_ants += div;
			if (mod)
				net->paths[i].n_ants += mod--;
			i++;
		}
	}

	i = 0;
	while (i < n_used)
	{
		printf("%d = %d\n", i, net->paths[i].n_ants);
		i++;
	}
	return n_used;
}

void		netflow_pushflow(t_netflow *net)
{
	int i;
	int j;
	int n_paths;
	bool flag;
	int ant;

	ant = 0;
	n_paths = netflow_simulate(net);
	flag = true;
	while (flag)
	{
		i = 0;
		flag = false;
		while (i < n_paths && ant < net->n_units) {
			if (net->paths[i].n_ants)
				net->paths[i].ants[0] = ++ant;
			i++;
		}
		i = 0;
		while (i < n_paths)
		{
			j = net->paths[i].size - 1;
			while (j >= 0) {
				if (net->paths[i].ants[j] != -1)
					printf("L%d-%s ", net->paths[i].ants[j],
						   net->paths[i].nodes[j]);
				j--;
			}
			i++;
		}
		i = 0;
		while (i < n_paths)
		{
			j = net->paths[i].size - 1;
			while (j >= 0) {
				if (net->paths[i].ants[j] != -1)
					flag = true;
				net->paths[i].ants[j] = net->paths[i].ants[j - 1];
				j--;
			}
			net->paths[i].ants[0] = -1;
			i++;
		}
		if (flag)
			printf("\n");
	}
}
