/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bfs.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: archid- <archid-@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/30 09:00:42 by archid-           #+#    #+#             */
/*   Updated: 2019/11/30 22:53:46 by archid-          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "queue.h"
#include "../lem_in.h"

/*
   NOTE: parents should hold each node's parents in order to reconstruct
   the part from start to sink
*/

t_queue		*gen_path(t_node *start, t_node *sink, t_queue *parents)
{
	(void)start;
	(void)sink;
	(void)parents;
	return (NULL);
}

void		print_node(t_qnode *node)
{
	t_node *e;

	if (!node)
		return ;
	e = (t_node *)node->blob;
	ft_printf("%s %d\n", e->name, e->type);
}

void		queue_del_helper(void *blob, size_t size)
{
	if (size)
		free(blob);
}

bool		bfs(t_graph *graph, t_node *start, t_node *sink)
{
	/* t_queue		*parents; */
	t_queue		*q;
	t_node		*node;
	t_edges		*edge_walk;

	if (!graph || !start || !sink)
		return (false);
	q = queue_init();
	ft_printf(" // BFS START // ");
	/* parents = queue_init(); */
	start->seen= true;
	queue_enq(q, queue_node(start, sizeof(t_node)));
	system("sleep 1");
	while (queue_size(q))
	{
		if ((node = queue_deq(q)->blob) == sink)
			return (true);		/* reached */
		edge_walk = node->edges;
		ft_printf(" // EDGES START // ");
		system("sleep 1");
		while (edge_walk)
		{
			ft_printf("src : %s %s\n", edge_walk->node_src->name, edge_walk->node_dst->name);
			if (!edge_walk->node_dst->seen)
			{
				edge_walk->node_dst->seen = true;
				queue_enq(q, queue_node(edge_walk->node_dst, sizeof(t_node)));

				ft_printf("[%zu]", queue_size(q));
				ft_printf("+");
				system("sleep 1");
			}
			edge_walk = edge_walk->next;
		}
		ft_printf("\n");
	}
	/* return (gen_path(start, sink, parents)); */
	return (false);				/* unreached */
}
