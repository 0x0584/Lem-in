/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lem_in_main.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: melalj <melalj@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/12 22:07:41 by melalj            #+#    #+#             */
/*   Updated: 2019/12/17 03:17:14 by archid-          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../lem_in.h"

void node_info(t_node *node)
{
	char *type;
	char *seen;

	ft_putendl("========================================");
	ft_printf("index: %d -- name: '%s'\n", node->index, node->name);
	if (node->type == NODE_DEFAULT)
		type = "NODE";
	else
		type = (node->type == NODE_START ? "source" : "sink");
	ft_printf("type: %s -- seen: '%s'\n", type, node->seen == NODE_TAKEN ? "taken" : "fresh-or-seen");
	ft_putendl("========================================");
}

void node_dump(t_qnode *e)
{
	t_node *node;
	t_edge *e_walk;

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
	g->n_nodes = nodes_c;
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

			curr = curr->next;
		}
		i++;
	}
	return g;
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
		ft_printf("\n == ==  ==\n\n");
		walk = walk->next;
	}

}

void	graph_free(t_graph *g)
{
	/* ft_lstdel(&g->nodes_lst, lstdel_node); */
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

int		main(void)
{
	t_parse		*pp;
	t_node		**nodes;
	int			nodes_c;
	int			i;
	t_graph		*g;
	t_node		**refs;

	/* (void)getchar(); */

	ft_printf(" // ");
	i = 0;
	pp = get_lines(&nodes_c);

	/* `node' array of nodes */
	refs = (t_node **)malloc(sizeof(t_node *) * nodes_c);
	nodes = h_table(refs, pp, nodes_c);
	ft_putendl(" === filling edges === ");
	edges_fill(nodes, pp, nodes_c);

	g = graph_init(refs, nodes, nodes_c);
	graph_dump(g);

	t_queue *paths;
	t_queue *tmp_path;
	t_qnode *tmp;

	ft_printf("source: %s | sink: %s\n", g->start->name, g->sink->name);
	paths = list_shortest_paths(g);
	while (queue_size(paths))
	{
		ft_putendl("-----------");
		tmp = queue_deq(paths);
		tmp_path = tmp->blob;
		queue_iter(tmp_path, false, node_oneline_dump);
		ft_putendl("\n");
		queue_iter(tmp_path, false, node_full_dump);
		ft_putendl("\n");
		queue_del(&tmp_path, queue_del_helper);
		ft_putendl("-----------");
		/* sleep(3); */
	}
	queue_del(&paths, queue_del_helper);

	/* sp1 = bfs(g); */
	/* sp2 = bfs(g); */
	/* ft_printf(" ==== heree ====\n "); */
	/* queue_iter(sp1, node_dump); */
	/* queue_del(&sp1, queue_del_helper); */
	/* queue_iter(sp2, node_dump); */

	/* queue_del(&sp2, queue_del_helper); */

	/* graph_dump(g); */
	graph_free(g);

	return (0);
}
