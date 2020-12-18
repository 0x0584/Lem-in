/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   network.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: archid- <archid-@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/23 19:06:16 by archid-           #+#    #+#             */
/*   Updated: 2020/12/18 13:28:12 by archid-          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "visu.h"

static bool		g_json_output = false;

static void		save_json(t_graph g, t_lst paths)
{
	g_fd = open("out.json", O_CREAT | O_APPEND | O_RDWR, 0666);
	ftruncate(g_fd, 0);
	level_graph(g);
	ft_dprintf(g_fd, "{");
	vertex_as_json("start", g->source);
	ft_dprintf(g_fd, ",\n");
	vertex_as_json("end", g->sink);
	ft_dprintf(g_fd, ",\n");
	vertices_as_json(g->vertices);
	ft_dprintf(g_fd, ",\n");
	paths_as_json(paths);
	ft_dprintf(g_fd, "}\n");
	close(g_fd);
}

static void		network_prepare(t_graph graph, t_network net)
{
	t_lst	path;
	t_lst	paths;
	t_lst	flows;
	size_t	result;
	size_t	prev;

	prev = NIL_ANT;
	flows = net->flows;
	paths = lst_alloc(lst_free);
	while ((path = bfs(graph)))
	{
		correct_paths(lst_push_back_blob(paths, path, sizeof(t_lst), false));
		lst_iter_arg(lst_insertion_sort(paths, shortest_path), true,
						lst_clear(flows), path_to_flow);
		if ((result = network_simulate(net, false)) >= prev)
		{
			lst_node_free(flows, lst_extract(flows, lst_rear(flows)));
			lst_node_free(paths, lst_extract(paths, lst_rear(paths)));
			break ;
		}
		prev = result;
	}
	{
		assert_paths_correct(graph, paths);
	}
	if (g_json_output)
		save_json(graph, paths);
	lst_del(&paths);
}

t_network		network_setup(t_graph graph, size_t units)
{
	t_network net;

	net = malloc(sizeof(struct s_network));
	net->flows = lst_alloc(flow_free);
	net->n_units = units;
	network_prepare(graph, net);
	return (net);
}

void			network_del(t_network *anet)
{
	t_network net;

	if (!anet || !(net = *anet))
		return ;
	lst_del(&net->flows);
	free(net);
	*anet = NULL;
}

void			network_pushflow(t_network net)
{
	network_simulate(net, true);
}
