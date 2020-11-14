/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hash_t.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: melalj <melalj@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/24 13:07:14 by melalj            #+#    #+#             */
/*   Updated: 2020/11/14 19:36:23 by archid-          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../lem_in.h"

unsigned long	hash(unsigned char *str)
{
	unsigned long	hash;
	int				c;

	hash = 5381;
	while ((c = *str++))
		hash = ((hash << 5) + hash) + c;
	/*
	** hash * 33 + c
	*/
	return (hash);
}

t_vertex		**h_table(t_vertex **refs, t_parse *lines, int vertices_c)
{
	t_vertex	**vertices;
	int		i;
	int		prop;

	vertices = (t_vertex **)malloc(sizeof(t_vertex *) * vertices_c);
	i = -1;
	while (++i < vertices_c)
		vertices[i] = NULL;
	lines = lines->next;
	while (lines && lines->type == 1)
	{
		if (lines->line[0] != '#' && !add_vertex(vertices, lines, vertices_c,
						prop, refs) && ft_printf("duplicate name error\n"))
			exit(1);
		if (lines->next)
			prop = lines->prop > 1 ? lines->prop : 0;
		lines = lines->next;
	}
	return (vertices);
}
