/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   correction_extra.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: archid- <archid-@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/18 12:46:32 by archid-           #+#    #+#             */
/*   Updated: 2020/12/18 12:47:22 by archid-          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "graph.h"

bool		residual(t_edge e)
{
	return (e->residual->seen == M_BELONG_TO_PATH);
}

bool		residual_of(t_edge e, t_edge re)
{
	return (re == e->residual);
}

void		set_edge_fresh(t_edge e)
{
	e->seen = M_FRESH;
	e->src->seen = M_FRESH;
	e->dst->seen = M_FRESH;
}

void		set_edge_to_path(t_edge e)
{
	e->seen = M_BELONG_TO_PATH;
	e->src->seen = M_BELONG_TO_PATH;
	e->dst->seen = M_BELONG_TO_PATH;
}
