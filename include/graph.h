/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   graph.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: archid- <archid-@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/26 23:45:17 by archid-           #+#    #+#             */
/*   Updated: 2020/12/16 20:00:51 by archid-          ###   ########.fr       */
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

void			print_edge(t_edge e);
void			print_graph(t_graph g);
void			print_edge2(void *blob);

void			level_graph(t_graph g);

#endif
