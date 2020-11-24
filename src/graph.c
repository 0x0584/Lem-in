/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   graph.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: melalj <melalj@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/30 19:03:24 by archid-           #+#    #+#             */
/*   Updated: 2020/11/23 20:41:57 by archid-          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "graph.h"

void vertex_init(t_vertex *vert, char *name, int x, int y) {
	vert->name = name;
	vert->edges = queue_init();
	vert->x = x;
	vert->y = y;
	vert->seen = 0;
}

t_hnode vertex_alloc(char *line) {
	t_vertex *vert;
	char **split;
	char **ptr;

	vert = malloc(sizeof(t_vertex));
	split = ft_strsplit(line, ' ');
	vertex_init(vert, ft_strdup(split[0]),
				ft_atoi(split[1]), ft_atoi(split[2]));
	ptr = split;
	while (*ptr)
		ft_strdel(ptr++);
	free(split);
	return (t_hnode){vert->name, vert};
}

void edge_init(t_hnode **e, t_hnode **re, t_vertex *src, t_vertex *dst)
{
	(*e)->blob = malloc(sizeof(t_edge));
    ((t_edge *)((*e)->blob))->seen = 0;
    ((t_edge *)((*e)->blob))->src = src;
	((t_edge *)((*e)->blob))->dst = dst;
	(*e)->key = ft_strnew(0);
	ft_strchange(&(*e)->key, ft_strjoin(src->name, "-"));
	ft_strchange(&(*e)->key, ft_strjoin((*e)->key, dst->name));
	(*re)->blob = malloc(sizeof(t_edge));
    ((t_edge *)((*re)->blob))->seen = 0;
    ((t_edge *)((*re)->blob))->src = dst;
	((t_edge *)((*re)->blob))->dst = src;
	(*re)->key = ft_strnew(0);
	ft_strchange(&(*re)->key, ft_strjoin(dst->name, "-"));
	ft_strchange(&(*re)->key, ft_strjoin((*re)->key, src->name));
	((t_edge *)((*e)->blob))->residual = (*re)->blob;
	((t_edge *)((*re)->blob))->residual = (*e)->blob;
}

t_vertex *get_vertex(t_graph *g, char *name) {
	if (!g || !name)
		return NULL;
	return hash_get(g->vertices, name, NULL);
}

bool edge_alloc(t_graph *g, char *line, t_hnode *e, t_hnode *re)
{
	t_vertex *u;
	t_vertex *v;
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

bool vertex_cmp(void *u, void *v, size_t size)
{
	t_vertex *uer;
	t_vertex *ver;

	(void) size;

	uer = u;
	ver = v;

	return !ft_strcmp(uer->name, ver->name);
}

bool edge_cmp(void *e1, void *e2, size_t size)
{
	t_edge *e;
	t_edge *f;

	(void) size;

	e = e1;
	f = e2;

	return !ft_strcmp(e->src->name, f->src->name) &&
		!ft_strcmp(e->dst->name, f->dst->name);
}

void vertex_del(void *vert, size_t size)
{
	t_vertex *v;

	(void)size;
	v = vert;
	free(v->name);
	queue_del(&v->edges, edge_del);
	free(v);
}


void edge_del(void *edge, size_t size)
{
	t_edge *e;

	(void)size;
	e = edge;
	free(e->residual);
	free(e);
}

t_graph *graph_init(t_hash *V, t_hash *E)
{
	t_graph *g;

	g = malloc(sizeof(t_graph));
	g->n_vertices = 0;
	g->vertices = V;
	g->edges = E;
	g->source = NULL;
	g->sink = NULL;
	return (g);
}

void	graph_free(t_graph *g)
{
	hash_free(g->vertices);
	hash_free(g->edges);
	free(g);
}
