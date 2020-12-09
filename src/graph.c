/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   graph.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: archid- <archid-@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/23 01:02:42 by archid-           #+#    #+#             */
/*   Updated: 2020/12/09 00:20:09 by archid-          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "graph.h"

void print_edge(t_edge e) {
    if (!e)
        return;
    ft_printf("[src: %s(%d) | dst: %s(%d)](%d) ", e->src->name, e->src->seen,
              e->dst->name, e->dst->seen, e->seen);
}

void assert_path_connected(t_lst path) {
	t_lstnode walk;
	t_edge edge;
	t_edge prev;

	prev = NULL;
	walk = lst_front(path);
	while (walk) {
		edge = walk->blob;
		if (prev && prev->dst != edge->src)
            exit(ft_dprintf(2, "%s\n", "PATH IS NOT CONNECTED"));
		prev = edge;
		lst_node_forward(&walk);
	}
}

void assert_path_has_correct_edges(t_lst path) {
    t_lstnode walk;
    t_edge e;

    walk = lst_front(path);
    while (walk) {
        e = walk->blob;
        if (e->residual->seen == e->seen)
            exit(ft_dprintf(2, "%s\n", "PATH HAS RESIDUAL EDGE"));
        if (e->seen != BELONG_TO_PATH || e->src->seen != BELONG_TO_PATH ||
            e->dst->seen != BELONG_TO_PATH)
            exit(ft_dprintf(2, "%s\n", "PATH HAS AN INCORRECT EDGE"));
        lst_node_forward(&walk);
    }
}

void assert_paths_correct(t_lst paths) {
    t_lstnode walk;

    walk = lst_front(paths);
    while (walk) {
        assert_path_connected(walk->blob);
        /* assert_path_has_correct_edges(walk->blob); */
        lst_node_forward(&walk);
    }
}

void vertex_del(void *vert)
{
	t_vertex v;

	if (!(v = vert))
		return ;
	free(v->name);
	lst_del(&v->edges);
	free(v);
}

void edge_del(void *edge)
{
	free(edge);
}

void vertex_init(t_vertex vert, char *name, int x, int y) {
	vert->name = name;
	vert->edges = lst_alloc(blob_free);
	vert->x = x;
	vert->y = y;
	vert->seen = 0;
}

t_hashnode vertex_alloc(char *line) {
	t_vertex vert;
	char **split;
	char **ptr;

	vert = malloc(sizeof(struct s_vertex));
	split = ft_strsplit(line, ' ');
	vertex_init(vert, ft_strdup(split[0]),
				ft_atoi(split[1]), ft_atoi(split[2]));
	ptr = split;
	while (*ptr)
		ft_strdel(ptr++);
	free(split);
	return (t_hashnode){vert->name, vert};
}

void edge_init(t_hashnode **e, t_hashnode **re, t_vertex src, t_vertex dst)
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

t_vertex get_vertex(t_graph g, char *name) {
	if (!g || !name)
		return NULL;
	return hash_get(g->vertices, name, NULL);
}

bool edge_alloc(t_graph g, char *line, t_hashnode *e, t_hashnode *re)
{
	t_vertex u;
	t_vertex v;
	char **split;
	char **ptr;
	bool valid;

	split = ft_strsplit(line, '-');
	if ((valid = (u = get_vertex(g, split[0])) &&
		 (v = get_vertex(g, split[1]))))
		edge_init(&e, &re, u, v);
	else
		e->blob = NULL, re->blob = NULL;
	ptr = split;
	while (*ptr)
		ft_strdel(ptr++);
	free(split);
	return valid;
}

t_graph graph_init(t_hash V, t_hash E)
{
	t_graph g;

	g = malloc(sizeof(struct s_graph));
	g->n_vertices = 0;
	g->vertices = V;
	g->edges = E;
	g->source = NULL;
	g->sink = NULL;
	return (g);
}

void	graph_del(t_graph *g)
{
	if (!g || !*g)
		return ;
	hash_del(&(*g)->vertices);
	hash_del(&(*g)->edges);
	free(*g);
	*g = NULL;
}
