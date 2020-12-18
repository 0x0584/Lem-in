/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   graph.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: archid- <archid-@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/26 23:45:17 by archid-           #+#    #+#             */
/*   Updated: 2020/12/18 13:28:10 by archid-          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GRAPH_H
# define GRAPH_H

# include "hash.h"

typedef struct s_vertex		*t_vertex;
typedef struct s_edge		*t_edge;
typedef struct s_graph		*t_graph;
typedef struct s_edge_pair	t_edge_pair;

typedef enum	e_mark
{
	M_FRESH,
	M_BELONG_TO_PATH,
	M_INITIAL
}				t_mark;

struct			s_vertex
{
	t_mark			seen;
	char			*name;
	t_lst			edges;
	int				x;
	int				y;
	int				level;
};

struct			s_edge
{
	t_mark			seen;
	t_edge			residual;
	t_vertex		dst;
	t_vertex		src;
};

struct			s_graph
{
	t_hash			vertices;
	t_hash			edges;

	size_t			n_vertices;

	t_vertex		source;
	t_vertex		sink;
};

struct			s_edge_pair
{
	t_edge			edge;
	t_edge			from;
};

t_hashnode		vertex_alloc(char *line);
bool			edge_alloc(t_graph g, char *line,
							t_hashnode *e, t_hashnode *re);

t_graph			graph_init(t_hash v, t_hash e);
void			graph_del(t_graph *g);

void			vertex_del(void *vertex);
void			edge_del(void *edge);

t_lst			bfs(t_graph g);
void			correct_paths(t_lst paths);

void			assert_path_connected(t_lst path);
void			assert_path_has_correct_edges(t_graph g, t_lst path);
void			assert_paths_correct(t_graph g, t_lst paths);

void			level_graph(t_graph g);

bool			residual(t_edge e);
bool			residual_of(t_edge e, t_edge re);
void			set_edge_fresh(t_edge e);
void			set_edge_to_path(t_edge e);
void			edge_mark(t_edge e, t_mark mark);
bool			edge_unseen_or_crossing_path(t_edge e);
bool			edge_fresh(t_edge e);
bool			edge_crossing_path(t_edge e);
bool			edge_path_residual(t_edge e);

t_lst			construct_path(t_graph g, t_hash parent);

void			paths_as_json(t_lst paths);
void			path_as_json(void *path, size_t index);
void			vertices_as_json(t_hash verts);
void			vertex_with_edges_as_json(const char *key, void *blob);
void			vertex_as_json(const char *str, t_vertex v);

extern t_mark	g_mark;
extern int		g_fd;

#endif
