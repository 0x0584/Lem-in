/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lem_in_main.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: melalj <melalj@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/12 22:07:41 by melalj            #+#    #+#             */
/*   Updated: 2019/11/30 22:45:42 by archid-          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../lem_in.h"


t_graph *graph_init(t_node **nodes, int nodes_c)
{
	t_graph *g;
	t_node	*curr;
	t_node	*walk;

	int		i;

	i = 0;
	g = (t_graph *)ft_memalloc(sizeof(t_graph));

	ft_printf(" // ");
	getchar();
	while (i < nodes_c)
	{

		/* have adj list */
		ft_printf("i : %d\n", i);
		curr = nodes[i];
		/* i = 3 */
		/* pass over list of hash[i] */
		getchar();
		while (curr)
		{

			if (curr->type == NODE_START)
				g->start = curr;
			else if (curr->type == NODE_END)
				g->end = curr;

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

	/* (void)getchar(); */

	ft_printf(" // ");

	i = 0;

	pp = get_lines(&nodes_c);

	/* `node' array of nodes */
	nodes = h_table(pp, nodes_c);

	t_graph *g;
	edges_fill(nodes, pp, nodes_c);

	g = graph_init(nodes, nodes_c);

	ft_printf(" ==== heree ====\n ");
	bfs(g, g->start, g->end);

	/* graph_dump(g); */
	/* graph_free(g); */

	return (0);
}
