/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bfs_level.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: archid- <archid-@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/18 12:51:51 by archid-           #+#    #+#             */
/*   Updated: 2020/12/18 12:52:19 by archid-          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "graph.h"

static void		enqueue_edge(void *edge, void *open)
{
	t_edge e;

	e = edge;
	if (e->seen != g_mark && e->dst->seen != g_mark)
		lst_push_back_blob(open, edge, sizeof(t_edge), false);
}

void			level_graph(t_graph g)
{
	t_lst	open;
	t_edge	e;

	open = lst_alloc(blob_keep);
	g->source->level = 1;
	lst_iter_arg(g->source->edges, true, open, enqueue_edge);
	while (!lst_empty(open))
	{
		e = lst_pop_front_blob(open);
		if (e->dst->level == -1)
			e->dst->level = e->src->level + 1;
		lst_iter_arg(e->dst->edges, true, open, enqueue_edge);
		e->seen = g_mark;
		e->dst->seen = g_mark;
	}
	lst_del(&open);
}
