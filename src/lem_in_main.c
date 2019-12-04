/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lem_in_main.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: melalj <melalj@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/12 22:07:41 by melalj            #+#    #+#             */
/*   Updated: 2019/12/04 06:41:29 by archid-          ###   ########.fr       */
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
	t_edges *e_walk;

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
	t_edges *edges;
	t_edges *tmp;

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

t_graph *graph_init(t_node **refs, t_node **nodes, int nodes_c)
{
	t_graph *g;
	t_node	*curr;
	t_node	*walk;

	size_t	i;

	i = 0;
	g = (t_graph *)ft_memalloc(sizeof(t_graph));
	g->refs = refs;
	g->n_nodes = nodes_c;
	ft_printf(" // ");
	i = 0;
	while ((int)i < nodes_c)
	{

		/* have adj list */
		ft_printf("i : %d \n", i);
		curr = nodes[i];

		node_info(curr);

		/* i = 3 */
		/* pass over list of hash[i] */
		while (curr)
		{

			if (curr->type == NODE_START)
				g->start = curr;
			else if (curr->type == NODE_END)
				g->sink = curr;

			/*  */
			if (g->adjlst == NULL)
			{
				g->adjlst = ft_memcpy(malloc(sizeof(t_node)), curr, sizeof(t_node));
				walk = g->adjlst;
			}
			else
			{
				ft_putstr(" // ");
				ft_printf("b: %p %p | ", walk, walk->next);
				walk->next = ft_memcpy(malloc(sizeof(t_node)), curr, sizeof(t_node));
				walk = walk->next;
				ft_printf("a: %p %p \n", walk, walk->next);
			}
			/*  */
			ft_printf("%s\n", curr->name);
			curr = curr->next;
		}
		i++;
	}
	return g;
}

void	graph_dump(t_graph *g)
{
	t_node *walk;
	t_edges *e;

	walk = g->adjlst;
	while (walk)
	{
		ft_printf("<> name: %s type: %d <>\n", walk->name, walk->type);
		e = walk->edges;
		ft_printf(" == edges ==\n");
		while (e)
		{
			ft_printf("<> src %s %d <>\n", e->node_src->name, e->node_src->type);
			ft_printf("<> dst %s %d <>\n", e->node_dst->name, e->node_dst->type);
			e = e->next;
		}
		ft_printf("\n == ==  ==\n\n");
		walk = walk->next;
	}

}
void	graph_free(t_graph *g)
{
	/* ft_lstdel(&g->adjlst, lstdel_node); */
	free(g);
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
	edges_fill(nodes, pp, nodes_c);


	g = graph_init(refs, nodes, nodes_c);

	t_queue *paths;
	t_queue *tmp_path;
	t_qnode *tmp;

	paths = list_shortest_paths(g);
	while (queue_size(paths))
	{
		ft_putendl("-----------");
		tmp = queue_deq(paths);
		tmp_path = tmp->blob;
		queue_iter(tmp_path, node_dump);
		queue_del(&tmp_path, queue_del_helper);
		ft_putendl("-----------");
		sleep(3);
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
