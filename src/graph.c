/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   graph.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: archid- <archid-@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/23 01:02:42 by archid-           #+#    #+#             */
/*   Updated: 2020/12/18 13:02:52 by archid-          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "graph.h"

t_graph			graph_init(t_hash v, t_hash e)
{
	t_graph g;

	g = malloc(sizeof(struct s_graph));
	g->n_vertices = 0;
	g->vertices = v;
	g->edges = e;
	g->source = NULL;
	g->sink = NULL;
	return (g);
}

void			graph_del(t_graph *g)
{
	if (!g || !*g)
		return ;
	hash_del(&(*g)->vertices);
	hash_del(&(*g)->edges);
	free(*g);
	*g = NULL;
}
