/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   graph_json.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: archid- <archid-@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/18 13:09:26 by archid-           #+#    #+#             */
/*   Updated: 2020/12/18 13:28:37 by archid-          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "graph.h"

int				g_fd;

void		vertex_as_json(const char *str, t_vertex v)
{
	ft_dprintf(g_fd, "\"%s\": {\"name\":\"%s\", \"x\": %d, \"y\": %d}",
				str, v->name, v->x, v->y);
}

void		vertex_with_edges_as_json(const char *key, void *blob)
{
	t_vertex	v;
	t_lstnode	walk;
	t_edge		e;

	v = blob;
	ft_dprintf(g_fd, ",\n");
	ft_dprintf(g_fd, "\"%s\": {", key);
	ft_dprintf(g_fd, "\"x\": %d, \"y\": %d, \"level\": %d,",
				v->x, v->y, v->level);
	ft_dprintf(g_fd, "\"edges\": [");
	walk = lst_front(v->edges);
	while (walk)
	{
		e = walk->blob;
		ft_dprintf(g_fd, "\"%s\"", e->dst->name);
		if (lst_node_forward(&walk))
			ft_dprintf(g_fd, ",");
	}
	ft_dprintf(g_fd, "]}");
}

void		vertices_as_json(t_hash verts)
{
	ft_dprintf(g_fd, "\"vertices\": {");
	ft_dprintf(g_fd, "\"count\": \"%zu\"", hash_count(verts));
	hash_iter(verts, vertex_with_edges_as_json);
	ft_dprintf(g_fd, "}");
}

void		path_as_json(void *path, size_t index)
{
	t_lstnode	walk;
	t_edge		e;

	walk = lst_front(path);
	e = walk->blob;
	ft_dprintf(g_fd, ",\n \"%zu\": [\"%s\"", index, e->src->name);
	while (walk)
	{
		e = walk->blob;
		ft_dprintf(g_fd, ", \"%s\"", e->dst->name);
		lst_node_forward(&walk);
	}
	ft_dprintf(g_fd, "]");
}

void		paths_as_json(t_lst paths)
{
	ft_dprintf(g_fd, "\"paths\": {");
	ft_dprintf(g_fd, "\"size\": %zu", lst_size(paths));
	lst_iteri(paths, true, path_as_json);
	ft_dprintf(g_fd, "}");
}
