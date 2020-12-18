/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bfs_extra.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: archid- <archid-@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/18 12:49:17 by archid-           #+#    #+#             */
/*   Updated: 2020/12/18 12:50:39 by archid-          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "graph.h"

void		edge_mark(t_edge e, t_mark mark)
{
	if (e->seen != M_BELONG_TO_PATH)
		e->seen = mark;
	if (e->src->seen != M_BELONG_TO_PATH)
		e->src->seen = mark;
	if (e->dst->seen != M_BELONG_TO_PATH)
		e->dst->seen = mark;
}

bool		edge_unseen_or_crossing_path(t_edge e)
{
	return ((e->seen != g_mark && e->dst->seen != g_mark) ||
			(e->seen != M_BELONG_TO_PATH && e->dst->seen == M_BELONG_TO_PATH));
}

bool		edge_fresh(t_edge e)
{
	return ((e->seen != M_BELONG_TO_PATH && e->dst->seen != M_BELONG_TO_PATH) &&
			(e->seen != g_mark && e->dst->seen != g_mark));
}

bool		edge_crossing_path(t_edge e)
{
	return (e->seen != M_BELONG_TO_PATH && e->seen != g_mark &&
			e->dst->seen == M_BELONG_TO_PATH);
}

bool		edge_path_residual(t_edge e)
{
	return (e->residual->seen == M_BELONG_TO_PATH);
}
