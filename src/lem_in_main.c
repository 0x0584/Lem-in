/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lem_in_main.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: melalj <melalj@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/12 22:07:41 by melalj            #+#    #+#             */
/*   Updated: 2020/01/02 00:38:45 by archid-          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../lem_in.h"
# include "visu.h"

void node_info(t_node *node)
{
	char *type;
	// char *seen;

	ft_putendl("========================================");
	ft_printf("index: %d -- name: '%s'\n", node->index, node->name);
	if (node->type == NODE_DEFAULT)
		type = "NODE";
	else
		type = (node->type == NODE_START ? "source" : "sink");
	ft_printf("type: %s -- seen: '%d'\n", type, node->seen);
	ft_putendl("========================================");
}

void node_dump(t_qnode *e)
{
	t_node *node;
	// t_edge *e_walk;

	node_info(node = e->blob);
	/*
	ft_putendl("\n=========== edges ==========");
	e_walk = node->edges;
	while (e_walk)
	{
		node_info(e_walk->node_dst);
		if (e_walk->next)
			ft_putendl("~~~~~~~~~~~~~~~~~~~~~~~~~");
		e_walk = e_walk->next;
	}
	ft_putendl("===========       ==========\n");
	*/
	/* sleep(1); */
}

void lstdel_node(void *c, size_t size)
{
	t_edge *edges;
	t_edge *tmp;

	if (!size || !c)
		return ;
	edges = ((t_node *)c)->edges;
	while (edges)
	{
		tmp = edges;
		edges = edges->next;
		free(tmp);
	}
	free(((t_node *)c)->name);
}

void	helper_lst_alloc(t_node **head, t_node *walk, t_node *node)
{
	if (!head || !node)
		return ;

	if (!*head)
		*head = CLONE(node, sizeof(t_node));
	else
	{
		walk->next = CLONE(node, sizeof(node));
	}
}

t_graph *graph_init(t_node **refs, t_node **nodes, int nodes_c)
{
	t_graph *g;
	t_node	*curr;
	t_node	*walk;

	size_t	i;

	i = 0;
	g = (t_graph *)ft_memalloc(sizeof(t_graph));
	g->nodes_ref = refs;
	g->max_c.x = 0;
	g->max_c.y = 0;
	g->n_nodes = nodes_c;
#ifdef USE_VISU
	g->data = (t_dvisu *)malloc(sizeof(t_dvisu));
#endif
	while ((int)i < nodes_c)
	{
		curr = nodes[i];
		while (curr)
		{
			if (curr->type == NODE_START)
				g->start = curr;
			else if (curr->type == NODE_END)
				g->sink = curr;
			if (g->nodes_lst == NULL)
			{
				g->nodes_lst = ft_memcpy(malloc(sizeof(t_node)),
											curr, sizeof(t_node));
				walk = g->nodes_lst;
			}
			else
			{
				walk->next = ft_memcpy(malloc(sizeof(t_node)),
										curr, sizeof(t_node));
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
	return (g);
}

void	graph_dump(t_graph *g)
{
	t_node *walk;
	t_edge *e;

	walk = g->nodes_lst;
	while (walk)
	{
		ft_printf("<> name: %s type: %d <>\n", walk->name, walk->type);
		e = walk->edges;
		ft_printf(" == edges ==\n");
		while (e)
		{
			ft_printf("<> src %s type: %d <> ", e->node_src->name,
					  e->node_src->type);
			ft_printf("<> dst %s type: %d <>\n", e->node_dst->name,
					  e->node_dst->type);
			e = e->next;
		}
		ft_printf(" == ===== ==\n\n");
		walk = walk->next;
	}

}

void	graph_free(t_graph *g)
{
	t_node *node;
	t_node *nwalk;
	t_edge *edge;
	t_edge *tmp;
	t_edge *ewalk;

	nwalk = g->nodes_lst;
	while (nwalk)
	{
		node = nwalk;
		nwalk = nwalk->next;
		ewalk = node->edges;
		while (ewalk)
		{
			/* FIXME:  */
			edge = ewalk;
			ewalk = ewalk->next;
			if (edge->node_src->edges)
			{
				tmp = edge->node_dst->edges;
				edge->node_dst->edges = NULL;
				free(tmp);
			}
			free(edge->node_src->edges);
			edge->node_src->edges = NULL;
		}
		free(node->name);
		free(node);
	}
	free(g);
}


void	node_full_dump(t_qnode *e)
{
	t_node *node;
	t_edge *edge;			/* NOTE: if we save the edge, wont't need this,
								 * but let's get it working first, the fix
								 * design issues. */

	if (!e)
		return ;
	node = e->blob;
	edge = node->edges;
	ft_printf("node: %s\n", node->name);
	while (edge && edge->node_src != node)
	{
		ft_printf("<%s,%s>\n", edge->node_src->name, edge->node_dst->name);
		edge = edge->next;
	}
	ft_putendl("");
}

void	node_oneline_dump(t_qnode *e)
{
	t_node *node;
	t_edge *edge;			/* NOTE: if we save the edge, wont't need this,
								 * but let's get it working first, the fix
								 * design issues. */

	if (!e)
		return ;
	node = e->blob;
	edge = node->edges;
	ft_printf(" %s", node->name);
}

void	edge_oneline_dump(t_qnode *e)
{
	// t_node *node;
	t_edge *edge;			/* NOTE: if we save the edge, wont't need this,
								 * but let's get it working first, the fix
								 * design issues. */

	if (!e)
		return ;
	edge = e->blob;
	ft_printf(" <%s-%s> ",
			  edge->node_src->name,
			  edge->node_dst->name);
}

void	parser_free(t_parse *p)
{
	t_parse *walk;
	t_parse *hold;

	walk = p;
	while (walk)
	{
		hold = walk;
		walk = walk->next;
		free(hold->line);
		free(hold);
	}
}

int		main(void)
{
	t_parse		*pp;
	t_node		**nodes;
	int			nodes_c;
	int			i;
	t_graph		*g;
	t_node		**refs;
	size_t		n_ants;


	i = 0;
	pp = get_lines(&nodes_c);

	n_ants = (size_t)ft_atoi(pp->line);
#ifdef DEBUG
	ft_printf("%lu\n", n_ants);
#endif
	refs = (t_node **)malloc(sizeof(t_node *) * nodes_c);
	nodes = h_table(refs, pp, nodes_c);

	edges_fill(nodes, pp, nodes_c);

	parser_free(pp);

	g = graph_init(refs, nodes, nodes_c);

#ifdef DEBUG
	graph_dump(g);
	ft_printf("source: %s | sink: %s\n", g->start->name, g->sink->name);
#endif

	t_netflow *farm = netflow_setup(g, n_ants);

#ifdef USE_VISU
	visu_init(g);
	SDL_Event event;
	int close_requested = 0;
	while (!close_requested)
	{
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT
					|| (event.type == SDL_KEYDOWN
						&& event.key.keysym.sym == 27))
				close_requested = 1;
		}
	}
#endif

	netflow_pushflow(farm);
	netflow_del(&farm);

	/* FIXME: fix double free  */
	/* graph_free(g); */


	return (0);
}
