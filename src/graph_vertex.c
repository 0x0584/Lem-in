/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   graph_vertex.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: archid- <archid-@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/18 13:01:09 by archid-           #+#    #+#             */
/*   Updated: 2020/12/18 13:01:30 by archid-          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "graph.h"

void		vertex_del(void *vert)
{
	t_vertex v;

	if (!(v = vert))
		return ;
	free(v->name);
	lst_del(&v->edges);
	free(v);
}

void		vertex_init(t_vertex vert, char *name, int x, int y)
{
	vert->name = name;
	vert->edges = lst_alloc(blob_keep);
	vert->x = x;
	vert->y = y;
	vert->seen = 0;
	vert->level = -1;
}

t_hashnode	vertex_alloc(char *line)
{
	t_vertex	vert;
	char		**split;
	char		**ptr;

	vert = malloc(sizeof(struct s_vertex));
	split = ft_strsplit(line, ' ');
	vertex_init(vert, ft_strdup(split[0]),
				ft_atoi(split[1]), ft_atoi(split[2]));
	ptr = split;
	while (*ptr)
		ft_strdel(ptr++);
	free(split);
	return ((t_hashnode){vert->name, vert});
}
