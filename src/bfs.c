/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bfs.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: archid- <archid-@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/30 09:00:42 by archid-           #+#    #+#             */
/*   Updated: 2019/12/04 06:41:17 by archid-          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "queue.h"
#include "../lem_in.h"

/*
   NOTE: parents should hold each node's parents in order to reconstruct
   the part from start to sink
*/

static t_queue	*gen_bfs_path(t_graph *graph, int *refs)
{
	t_queue *q;
	size_t	prev;
	size_t	depth;
	size_t	i;

	prev = graph->sink->index;	/* starting at the sink */
	depth = 0;
	i = 0;
	if (refs[prev] == -1)
		return (NULL);
	ft_printf("sink:");
	q = queue_init();
	while (true)
	{
		/* sleep(1); */
		/* case of a dest_node in one edge of the sink's edges */
		if (graph->refs[prev]->type == NODE_DEFAULT)
			graph->refs[prev]->seen = NODE_TAKEN;
		queue_enq(q, queue_node(graph->refs[prev], sizeof(t_node)));
		ft_printf(" -> '%s'", graph->refs[prev]->name);
		if (refs[prev] == -1)
			break ;
		depth++;
		prev = refs[prev];
		/* queue_iter(q, node_dump); */
		/* sleep(2); */
	}
	ft_printf(" -> :start\n");
	return (q);
}

void		print_node(t_qnode *node)
{
	t_node *e;

	if (!node)
		return ;
	e = (t_node *)node->blob;
	ft_printf("%s %d\n", e->name, e->type);
}

void	reset_graph_discovery(t_graph *g)
{
	size_t i;

	i = 0;
	while (i < g->n_nodes)
	{
		if (g->refs[i]->seen != NODE_TAKEN)
			g->refs[i]->seen = NODE_FRESH;
		i++;
	}
}
/*
void		queue_del_helper(void *blob, size_t size)
{
	if (size)
		free(blob);
}
*/

 /* use array where each node is enumerated, first!  */

t_queue		*bfs(t_graph *graph)
{
	/* t_queue		*parents; */
	t_queue		*q;
	t_node		*node;
	t_edges		*edge_walk;
	int			*refs;
	t_queue		*path;

	if (!graph || !graph->start || !graph->sink)
		return (false);
	refs = ft_memset(malloc(graph->n_nodes * sizeof(int)), -1,
						graph->n_nodes * sizeof(int));
	q = queue_init();
	ft_printf(" // BFS START // \n");
	/* parents = queue_init(); */
	graph->start->seen = NODE_SEEN;
	queue_enq(q, queue_node(graph->start, sizeof(t_node)));
	/* system("sleep 1"); */
	while (queue_size(q))
	{
		if ((node = queue_deq(q)->blob) == graph->sink)
			break ;		/* reached */
		edge_walk = node->edges;
		ft_printf(" // EDGES START // \n");
		/* system("sleep 1"); */
		while (edge_walk)
		{
			ft_printf("%s -> [%s]\n", edge_walk->node_src->name,
						edge_walk->node_dst->name);
			if (edge_walk->node_dst->seen == NODE_FRESH)
			{
				edge_walk->node_dst->seen = NODE_SEEN;
				queue_enq(q, queue_node(edge_walk->node_dst, sizeof(t_node)));

				ft_printf("[%zu]", queue_size(q));
				ft_printf("+\n");
				/* system("sleep 1"); */
				/* keep the parent of each node, so that we can trace the back from the sink */
				refs[edge_walk->node_dst->index] = node->index;
			}
			edge_walk = edge_walk->next;
		}
		/* free(node); */
// it might need a queue_del if we actually duplicate memory.
		ft_printf("\n");
	}
	size_t i = 0;
	path = gen_bfs_path(graph, refs);
	reset_graph_discovery(graph);
	/* queue_del(&q, queue_del_helper); */
	free(refs);
	return (path);
	/* return (false); */				/* unreached */
}

t_queue		*list_shortest_paths(t_graph *graph)
{
	t_queue *paths;
	t_queue *tmp;

	paths = queue_init();

	while ((tmp = bfs(graph)))
	{
		/* queue_iter(tmp, node_dump); */
		/* sleep(1); */
		queue_enq(paths, queue_node(tmp, sizeof(t_queue)));
		/* queue_del(&tmp, queue_del_helper); */ /* queue_node() allocates a copy, but we're
											* not duplicating the actual queue, we're
											* just duplicating tmp. so nothing to free, again!
											* since it's all would be free'd at the very end
											* with graph_free() */
	}
	return (paths);
}
