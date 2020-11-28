/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   graph.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: archid- <archid-@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/26 23:45:17 by archid-           #+#    #+#             */
/*   Updated: 2020/11/28 01:24:03 by archid-          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GRAPH_H
#define GRAPH_H

#include "hash.h"

typedef struct s_vertex t_vertex;
typedef struct s_edge t_edge;
typedef struct s_graph t_graph;

enum e_bfs_turn {
    FRESH,
    BELONG_TO_PATH,
    INITIAL,
};

extern enum e_bfs_turn g_turn;

struct s_vertex {
    enum e_bfs_turn seen;
    char *name;
    t_queue *edges;
    int x;
    int y;
};

struct s_edge {
    enum e_bfs_turn seen;
    t_edge *residual;
    t_vertex *dst;
    t_vertex *src;
};

struct s_graph {
    size_t n_vertices;
    t_hash *vertices;
    t_hash *edges;
    t_vertex *source;
    t_vertex *sink;
};

t_hnode vertex_alloc(char *line);
bool edge_alloc(t_graph *g, char *line, t_hnode *e, t_hnode *re);

t_graph *graph_init(t_hash *V, t_hash *E);
void graph_free(t_graph *g);

bool vertex_cmp(void *u, void *v, size_t size);
void vertex_del(void *v);

bool edge_cmp(void *e1, void *e2, size_t size);
void edge_del(void *edge);

void node_dump(t_qnode *node);
void print_edge(t_qnode *node);
void print_path(t_qnode *node);
#endif /* GRAPH_H */
