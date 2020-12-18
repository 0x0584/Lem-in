/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   graph_edge.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: archid- <archid-@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/18 13:00:19 by archid-           #+#    #+#             */
/*   Updated: 2020/12/18 13:04:34 by archid-          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "graph.h"

t_vertex	get_vertex(t_graph g, char *name)
{
	if (!g || !name)
		return (NULL);
	return (hash_get(g->vertices, name, NULL));
}

void		edge_init(t_hashnode **e, t_hashnode **re,
							t_vertex src, t_vertex dst)
{
	(*e)->blob = malloc(sizeof(struct s_edge));
	((t_edge)((*e)->blob))->seen = 0;
	((t_edge)((*e)->blob))->src = src;
	((t_edge)((*e)->blob))->dst = dst;
	ft_asprintf(&(*e)->key, "%s-%s", src->name, dst->name);
	(*re)->blob = malloc(sizeof(struct s_edge));
	((t_edge)((*re)->blob))->seen = 0;
	((t_edge)((*re)->blob))->src = dst;
	((t_edge)((*re)->blob))->dst = src;
	ft_asprintf(&(*re)->key, "%s-%s", dst->name, src->name);
	((t_edge)((*e)->blob))->residual = (*re)->blob;
	((t_edge)((*re)->blob))->residual = (*e)->blob;
}

bool		edge_alloc(t_graph g, char *line, t_hashnode *e, t_hashnode *re)
{
	t_vertex	u;
	t_vertex	v;
	char		**split;
	char		**ptr;
	bool		valid;

	split = ft_strsplit(line, '-');
	valid = false;
	if ((u = get_vertex(g, split[0])) &&
			(v = get_vertex(g, split[1])))
	{
		valid = true;
		edge_init(&e, &re, u, v);
	}
	else
	{
		e->blob = NULL;
		re->blob = NULL;
	}
	ptr = split;
	while (*ptr)
		ft_strdel(ptr++);
	free(split);
	return (valid);
}

void		edge_del(void *edge)
{
	if (edge)
		free(edge);
}
