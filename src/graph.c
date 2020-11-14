/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   graph.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: melalj <melalj@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/30 19:03:24 by archid-           #+#    #+#             */
/*   Updated: 2020/11/14 19:38:27 by archid-          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../lem_in.h"


void vertex_info(t_vertex *vertex)
{
	char *type;
	// char *seen;

	ft_putendl("========================================");
	ft_printf("index: %d -- name: '%s'\n", vertex->index, vertex->name);
	if (vertex->type == V_DEFAULT)
		type = "VERTEX";
	else
		type = (vertex->type == V_SOURCE ? "source" : "sink");
	ft_printf("type: %s -- seen: '%d'\n", type, vertex->seen);
	ft_putendl("========================================");
}

void vertex_dump(t_qnode *e)
{
	t_vertex *vertex;
	// t_edge *e_walk;

	vertex_info(vertex = e->blob);
	/*
	ft_putendl("\n=========== edges ==========");
	e_walk = vertex->edges;
	while (e_walk)
	{
		vertex_info(e_walk->dst);
		if (e_walk->next)
			ft_putendl("~~~~~~~~~~~~~~~~~~~~~~~~~");
		e_walk = e_walk->next;
	}
	ft_putendl("===========       ==========\n");
	*/
	/* sleep(1); */
}

void lstdel_vertex(void *c, size_t size)
{
	t_edge *edges;
	t_edge *tmp;

	if (!size || !c)
		return ;
	edges = ((t_vertex *)c)->edges;
	while (edges)
	{
		tmp = edges;
		edges = edges->next;
		free(tmp);
	}
	free(((t_vertex *)c)->name);
}

void	helper_lst_alloc(t_vertex **head, t_vertex *walk, t_vertex *vertex)
{
	if (!head || !vertex)
		return ;

	if (!*head)
		*head = CLONE(vertex, sizeof(t_vertex));
	else
	{
		walk->next = CLONE(vertex, sizeof(vertex));
	}
}

t_graph *graph_init(t_vertex **refs, t_vertex **vertices, int vertices_c)
{
	t_graph *g;
	t_vertex	*curr;
	t_vertex	*walk;

	size_t	i;

	i = 0;
	g = (t_graph *)ft_memalloc(sizeof(t_graph));
	g->vertices_ref = refs;
	g->max_c.x = 0;
	g->max_c.y = 0;
	g->n_vertices = vertices_c;
	g->source = NULL;
	g->sink = NULL;
	while ((int)i < vertices_c)
	{
		curr = vertices[i];
		while (curr)
		{
			if (curr->type == V_SOURCE)
				g->source = curr;
			else if (curr->type == V_SINK)
				g->sink = curr;
			if (g->vertices_lst == NULL)
			{
				g->vertices_lst = ft_memcpy(malloc(sizeof(t_vertex)),
											curr, sizeof(t_vertex));
				walk = g->vertices_lst;
			}
			else
			{
				walk->next = ft_memcpy(malloc(sizeof(t_vertex)),
										curr, sizeof(t_vertex));
				walk = walk->next;
			}
			g->max_c.x = (g->max_c.x > curr->cords.x ?
							g->max_c.x : curr->cords.x);
			g->max_c.y = (g->max_c.y > curr->cords.y ?
							g->max_c.y : curr->cords.y);
			curr = curr->next;
		}
		i++;
	}
	if (!(g->source) || !(g->sink))
	{
		ft_printf("error no start or end\n");
		exit(1);
	}
	return (g);
}

void	graph_dump(t_graph *g)
{
	t_vertex *walk;
	t_edge *e;

	walk = g->vertices_lst;
	while (walk)
	{
		ft_printf("<> name: %s type: %d <>\n", walk->name, walk->type);
		e = walk->edges;
		ft_printf(" == edges ==\n");
		while (e)
		{
			ft_printf("<> src %s type: %d <> ", e->src->name,
					  e->src->type);
			ft_printf("<> dst %s type: %d <>\n", e->dst->name,
					  e->dst->type);
			e = e->next;
		}
		ft_printf(" == ===== ==\n\n");
		walk = walk->next;
	}

}

void	graph_free(t_graph *g)
{
	t_vertex *vertex;
	t_vertex *nwalk;
	t_edge *edge;
	t_edge *tmp;
	t_edge *ewalk;

	nwalk = g->vertices_lst;
	while (nwalk)
	{
		vertex = nwalk;
		nwalk = nwalk->next;
		ewalk = vertex->edges;
		while (ewalk)
		{
			/* FIXME:  */
			edge = ewalk;
			ewalk = ewalk->next;
			if (edge->src->edges)
			{
				tmp = edge->dst->edges;
				edge->dst->edges = NULL;
				free(tmp);
			}
			free(edge->src->edges);
			edge->src->edges = NULL;
		}
		free(vertex->name);
		free(vertex);
	}
	free(g);
}


void	vertex_full_dump(t_qnode *e)
{
	t_vertex *vertex;
	t_edge *edge;			/* NOTE: if we save the edge, wont't need this,
								 * but let's get it working first, the fix
								 * design issues. */

	if (!e)
		return ;
	vertex = e->blob;
	edge = vertex->edges;
	ft_printf("vertex: %s\n", vertex->name);
	while (edge && edge->src != vertex)
	{
		ft_printf("<%s,%s>\n", edge->src->name, edge->dst->name);
		edge = edge->next;
	}
	ft_putendl("");
}

void	vertex_oneline_dump(t_qnode *e)
{
	t_vertex *vertex;
	t_edge *edge;			/* NOTE: if we save the edge, wont't need this,
								 * but let's get it working first, the fix
								 * design issues. */

	if (!e)
		return ;
	vertex = e->blob;
	edge = vertex->edges;
	ft_printf(" %s", vertex->name);
}

void	edge_oneline_dump(t_qnode *e)
{
	// t_vertex *vertex;
	t_edge *edge;			/* NOTE: if we save the edge, wont't need this,
								 * but let's get it working first, the fix
								 * design issues. */

	if (!e)
		return ;
	edge = e->blob;
	ft_printf(" <%s-%s> ",
			  edge->src->name,
			  edge->dst->name);
}
