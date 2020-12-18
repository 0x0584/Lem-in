/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   graph_assert.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: archid- <archid-@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/18 13:03:00 by archid-           #+#    #+#             */
/*   Updated: 2020/12/18 13:03:11 by archid-          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "graph.h"

void		assert_path_connected(t_lst path)
{
	t_lstnode	walk;
	t_edge		edge;
	t_edge		prev;

	prev = NULL;
	walk = lst_front(path);
	while (walk)
	{
		edge = walk->blob;
		if (prev && prev->dst != edge->src)
			exit(ft_dprintf(2, "%s\n", "PATH IS NOT CONNECTED"));
		prev = edge;
		lst_node_forward(&walk);
	}
}

void		assert_path_has_correct_edges(t_graph g, t_lst path)
{
	t_lstnode	walk;
	t_edge		e;

	walk = lst_front(path);
	while (walk)
	{
		e = walk->blob;
		if (e->residual->seen == e->seen)
			exit(ft_dprintf(2, "%s\n", "PATH HAS RESIDUAL EDGE"));
		if (e->seen != M_BELONG_TO_PATH ||
				(e->src != g->source && e->src->seen != M_BELONG_TO_PATH) ||
				(e->dst != g->sink && e->dst->seen != M_BELONG_TO_PATH))
			exit(ft_dprintf(2, "%s\n", "PATH HAS AN INCORRECT EDGE"));
		lst_node_forward(&walk);
	}
}

bool		edges_share_vertex(t_graph g, t_edge e, t_edge o)
{
	return ((e->src == o->src && e->src != g->source) ||
				(e->dst == o->dst && e->dst != g->sink));
}

bool		path_is_correct(t_graph g, t_lst path_a, t_lst path_b)
{
	t_lstnode walk_a;
	t_lstnode walk_b;

	walk_a = lst_front(path_a);
	while (walk_a)
	{
		walk_b = lst_front(path_b);
		while (walk_b)
		{
			if (edges_share_vertex(g, walk_a->blob, walk_b->blob))
				return (false);
			lst_node_forward(&walk_b);
		}
		lst_node_forward(&walk_a);
	}
	return (true);
}

void		assert_paths_have_distinct_vertices(t_graph g, t_lst paths)
{
	t_lstnode walk_a;
	t_lstnode walk_b;

	walk_a = lst_front(paths);
	while (walk_a)
	{
		walk_b = lst_front(paths);
		while (walk_b)
		{
			if (walk_b != walk_a &&
				!path_is_correct(g, walk_b->blob, walk_a->blob))
				exit(ft_dprintf(2, "%s\n", "PATHS SHARE VERTEX"));
			lst_node_forward(&walk_b);
		}
		lst_node_forward(&walk_a);
	}
}

void		assert_paths_correct(t_graph g, t_lst paths)
{
	t_lstnode walk;

	walk = lst_front(paths);
	while (walk)
	{
		assert_path_connected(walk->blob);
		assert_path_has_correct_edges(g, walk->blob);
		lst_node_forward(&walk);
	}
	assert_paths_have_distinct_vertices(g, paths);
}
