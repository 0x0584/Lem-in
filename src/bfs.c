/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bfs.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: archid- <archid-@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/30 09:00:42 by archid-           #+#    #+#             */
/*   Updated: 2019/11/30 18:24:12 by archid-          ###   ########.fr       */
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

bool		bfs(t_node *graph, t_node *start, t_node *sink)
{
	/* t_queue		*parents; */
	t_queue		*q;
	t_node		*node;
	t_edges		*edge_walk;

	if (!graph || !start || !sink)
		return (false);
	q = queue_init();
	/* parents = queue_init(); */
	start->seen= true;
	queue_enq(q, queue_node(start, sizeof(t_node)));
	while (queue_size(q))
	{
		if ((node = queue_deq(q)->blob) == sink)
			return (true);		/* reached */
		edge_walk = node->edges;
		while (edge_walk)
		{
			if (!edge_walk->node_dst->seen)
			{
				edge_walk->node_dst->seen = true;
				queue_enq(q, queue_node(edge_walk->node_dst, sizeof(t_node)));
			}
			edge_walk = edge_walk->next;
		}
	}
	/* return (gen_path(start, sink, parents)); */
	return (false);				/* unreached */
}
