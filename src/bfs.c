/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bfs.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: archid- <archid-@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/24 22:52:55 by archid-           #+#    #+#             */
/*   Updated: 2020/12/18 14:26:53 by archid-          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "graph.h"

static t_graph	g_graph = NULL;
t_mark	g_mark = M_INITIAL;

static void		hash_add_edge(t_hash parent, t_edge edge, t_edge from)
{
	t_lst		found;
	t_lstnode	pair;
	t_edge_pair current;

	current = (t_edge_pair){edge, from};
	pair = lst_node(&current, sizeof(t_edge_pair), true);
	if ((found = hash_get(parent, edge->dst->name, NULL)))
		lst_push_back(found, pair);
	else
		hash_add(parent, edge->dst->name,
					lst_push_back(lst_alloc(blob_free), pair));
}

static bool		enqueue_edges(t_edge e, t_lst open, t_hash parent,
								bool (*check_edge)(t_edge e))
{
	t_lstnode	walk;
	t_edge		next;

	walk = lst_front(e->dst->edges);
	while (walk)
	{
		next = walk->blob;
		if (next->dst != e->src && check_edge(next))
		{
			hash_add_edge(parent, next, e);
			if (next->dst == g_graph->sink)
				return (true);
			lst_push_back_blob(open, next, sizeof(t_edge), false);
		}
		lst_node_forward(&walk);
	}
	return (false);
}

static bool		handle_edge(t_edge e, t_lst open, t_hash parent)
{
	if (edge_fresh(e))
	{
		edge_mark(e, g_mark);
		return (enqueue_edges(e, open, parent, edge_unseen_or_crossing_path));
	}
	else if (edge_crossing_path(e))
	{
		edge_mark(e, g_mark);
		return (enqueue_edges(e, open, parent, edge_path_residual(e)
								? edge_unseen_or_crossing_path
									: edge_path_residual));
	}
	else
		return (false);
}

static t_lst	bfs_setup(t_lst *open, t_hash *parent)
{
	t_lstnode walk;
	t_edge e;

	g_graph->source->seen = M_FRESH;
	g_graph->sink->seen = M_FRESH;
	*parent = hash_alloc(g_graph->n_vertices, lst_free);
	*open = lst_alloc(blob_keep);
	walk = lst_front(g_graph->source->edges);
	while (walk)
	{
		e = walk->blob;
		if (edge_fresh(e))
		{
			lst_push_back_blob(*open, e, sizeof(t_edge), false);
			hash_add_edge(*parent, e, NULL);
			if (e->dst == g_graph->sink)
				return lst_push_back_blob(lst_alloc(blob_keep),
											e, sizeof(t_edge), false);
		}
		lst_node_forward(&walk);
	}
	return NULL;
}

t_lst			bfs(t_graph g)
{
	t_hash	parent;
	t_lst	path;
	t_lst	open;
	t_edge	e;
	bool	sink_reached;

	if (!(g_graph = g))
		return NULL;
	if ((path = bfs_setup(&open, &parent)))
		return path;
	sink_reached = false;
	while (!sink_reached && !lst_empty(open))
	{
		e = lst_pop_front_blob(open);
		sink_reached = handle_edge(e, open, parent);
	}
	lst_del(&open);
	path = sink_reached ? construct_path(g, parent) : NULL;
	hash_del(&parent);
	g_mark++;
	return (path);
}
