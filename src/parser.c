/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: melalj <melalj@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/24 13:54:11 by melalj            #+#    #+#             */
/*   Updated: 2020/11/14 19:36:23 by archid-          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../lem_in.h"

t_vertex	*new_vertex(int index, t_parse *line, int prop)
{
	t_vertex	*new_n;
	char	**s_line;

	s_line = ft_strsplit(line->line, ' ');
	new_n = (t_vertex *)malloc(sizeof(t_vertex));
	new_n->name = ft_strdup(s_line[0]);
	new_n->cords.x = ft_atoi(s_line[1]);
	new_n->cords.y = ft_atoi(s_line[2]);
	free_tab(s_line);
	new_n->edges = NULL;
	new_n->next = NULL;
	new_n->type = V_DEFAULT;
	new_n->seen = false;
	new_n->index = index;
	// ft_printf("prop : %d\n", prop);
	if (prop > 1)
		new_n->type = (prop == 2 ? V_SOURCE : V_SINK);
	return (new_n);
}

int		add_vertex(t_vertex **lst_vertex, t_parse *lines, int vertices_c, int prop, t_vertex **refs)
{
	static size_t	index = 0;
	char			**s_lines;
	t_vertex			*curr;
	int				hash_h;

	s_lines = ft_strsplit(lines->line, ' ');
	hash_h = hash((unsigned char*)s_lines[0]) % vertices_c;
	refs[index] = new_vertex(index, lines, prop);
	// ft_printf("added vertex %s in the hash %d\n", s_lines[0],
	//						hash((unsigned char*)s_lines[0]) % vertices_c);
	if (!lst_vertex[hash_h])
	{
		lst_vertex[hash_h] = refs[index++];
		free_tab(s_lines);
		return (1);
	}
	curr = lst_vertex[hash_h];
	if (ft_strequ((curr)->name, s_lines[0]) && free_tab(s_lines))
		return (0);
	while (curr->next)
	{
		// ft_printf("%s | %s\n", s_lines[0], curr->name);
		if (ft_strequ((curr)->name, s_lines[0]) && free_tab(s_lines))
			return (0);
		(curr) = (curr)->next;
	}
	(curr)->next = refs[index++];
	free_tab(s_lines);
	return (1);
}

// get vertex based on a hash
t_vertex	*get_vertex(t_vertex **lst_vertex, char *name, int vertices_c)
{
	int		hash_h;
	t_vertex	*curr;

	hash_h = hash((unsigned char*)name) % vertices_c;
	curr = lst_vertex[hash_h];
	while (curr)
	{
		if (ft_strequ((curr)->name, name))
			return (curr);
		curr = curr->next;
	}
	return (NULL);
}

/*
   add the edge and it's residual
*/

int	add_edge(t_vertex *src, t_vertex *dst, bool is_residual, t_edge *e)
{
	t_edge *curr;
	t_edge *tmp;

	is_residual ^= true;
	if (src->edges == NULL)
	{
		src->edges = (t_edge *)malloc(sizeof(t_edge));
		tmp = src->edges;
		src->edges->src = src;
		src->edges->dst = dst;
		src->edges->next = NULL;
	}
	else
	{
		curr = src->edges;
			if ((ft_strequ(curr->src->name, src->name)
					&& ft_strequ(curr->dst->name, dst->name))
					|| (ft_strequ(curr->src->name, dst->name)
							&& ft_strequ(curr->dst->name, src->name)))
			{
				exit(1);
			}
		while (curr->next)
		{
			curr = curr->next;
		}
		curr->next = (t_edge *)malloc(sizeof(t_edge));
		curr->next->src = src;
		curr->next->dst = dst;
		curr->next->next = NULL;
		tmp = curr->next;
	}

	if (is_residual == false)
	{
		// this need to stay tmp not curr
		tmp->residual = e;
		e->residual = tmp;

		e->seen = 0;
		tmp->seen = 0;
		return 0;
	}
	add_edge(dst, src, true, tmp);
	return (1);
}

int	edges_fill(t_vertex **lst_vertex, t_parse *lines, int vertices_c)
{
	char	**s_lines;
	t_vertex	*vertex[2];

	while (lines && lines->type < 2) /* this just to walk till the edges
										in the stored lines ant number has
										type 0 and vertices has type 1 */
	{
		ft_printf("%s\n", lines->line);
		lines = lines->next;
	}
	while (lines)
	{
		if (lines->line[0] == '#')
		{
			lines = lines->next;
			continue;
		}
		s_lines = ft_strsplit(lines->line, '-');
		vertex[0] = get_vertex(lst_vertex, s_lines[0], vertices_c);
		vertex[1] = get_vertex(lst_vertex, s_lines[1], vertices_c);
		if (!vertex[0] || !vertex[1])
		{
			ft_printf("edge not found\n");
			exit(1);
		}
		add_edge(vertex[0], vertex[1], false, NULL);
		lines = lines->next;
		free_tab(s_lines);
	}
	return (1);
}
