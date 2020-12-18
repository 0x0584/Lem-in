/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   graph_json.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: archid- <archid-@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/18 13:09:26 by archid-           #+#    #+#             */
/*   Updated: 2020/12/18 13:10:58 by archid-          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "graph.h"

static int		g_fd;

static void		vertex_as_json(const char *str, t_vertex v)
{
	ft_dprintf(g_fd, "\"%s\": {\"name\":\"%s\", \"x\": %d, \"y\": %d}",
				str, v->name, v->x, v->y);
}

static void		vertex_with_edges_as_json(const char *key, void *blob)
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

static void		vertices_as_json(t_hash verts)
{
	ft_dprintf(g_fd, "\"vertices\": {");
	ft_dprintf(g_fd, "\"count\": \"%zu\"", hash_count(verts));
	hash_iter(verts, vertex_with_edges_as_json);
	ft_dprintf(g_fd, "}");
}

static void		path_as_json(void *path, size_t index)
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

static void		paths_as_json(t_lst paths)
{
	ft_dprintf(g_fd, "\"paths\": {");
	ft_dprintf(g_fd, "\"size\": %zu", lst_size(paths));
	lst_iteri(paths, true, path_as_json);
	ft_dprintf(g_fd, "}");
}

void			save_json(t_graph g, t_lst paths)
{
	g_fd = open("out.json", O_CREAT | O_APPEND | O_RDWR, 0666);
	ftruncate(g_fd, 0);
	level_graph(g);
	ft_dprintf(g_fd, "{");
	vertex_as_json("start", g->source);
	ft_dprintf(g_fd, ",\n");
	vertex_as_json("end", g->sink);
	ft_dprintf(g_fd, ",\n");
	vertices_as_json(g->vertices);
	ft_dprintf(g_fd, ",\n");
	paths_as_json(paths);
	ft_dprintf(g_fd, "}\n");
	close(g_fd);
}
