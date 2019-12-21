
#include "../lem_in.h"



static t_queue	*gen_bfs_path(t_graph *graph, int *nodes_ref)
{
	t_queue *q;
	size_t	prev;
	size_t	depth;
	size_t	i;

	if (nodes_ref[graph->sink->index] == -1)
		return (NULL);
	i = 0;
	depth = 0;
	prev = graph->sink->index;	/* starting at the sink */
	q = queue_init();
	while (true)
	{
		if (graph->nodes_ref[prev]->type == NODE_DEFAULT)
			graph->nodes_ref[prev]->seen = NODE_TAKEN;
		queue_enq(q, queue_node(graph->nodes_ref[prev], sizeof(t_node)));
		if (nodes_ref[prev] == -1)
			break ;
		depth++;
		prev = nodes_ref[prev];	/* number of nodes in the path */
	}
	return (q);
}

void		print_node(t_qnode *node)
{
	t_node *e;

	if (!node)
		return ;
	e = (t_node *)node->blob;
}

void	reset_graph_discovery(t_graph *g)
{
	size_t i;

	i = 0;
	while (i < g->n_nodes)
	{
		if (g->nodes_ref[i]->seen != NODE_TAKEN)
			g->nodes_ref[i]->seen = NODE_FRESH;
		i++;
	}
}


/* FIXME: save edges instead of nodes
 * NOTE: this should save edges instead of nodes!! */

t_queue		*bfs(t_graph *graph)
{
	t_queue		*q;
	t_node		*node;
	t_edge		*edge_walk;
	int			*nodes_ref;
	t_queue		*path;

	if (!graph || !graph->start || !graph->sink)
		return (false);

	nodes_ref = ft_memset(malloc(graph->n_nodes * sizeof(int)), -1,
							graph->n_nodes * sizeof(int));

	q = queue_init();
	graph->start->seen = NODE_SEEN;
	queue_enq(q, queue_node(graph->start, sizeof(t_node)));
	while (queue_size(q))
	{

		queue_iter(q, false, node_oneline_dump);

		if ((node = queue_deq(q)->blob) == graph->sink)
			break ;		/* reached */
		ft_printf("\n>>> current: %s", node->name);

		edge_walk = node->edges;
		/* getchar(); */
		while (edge_walk)
		{
			/* queue_enq(q, queue_node(edge_walk->residual->node_dst, */
			/* 							sizeof(t_node))); */
			if (edge_walk->node_dst->seen == NODE_FRESH)
			{
				edge_walk->node_dst->seen = NODE_SEEN;
				queue_enq(q, queue_node(edge_walk->node_dst, sizeof(t_node)));
				nodes_ref[edge_walk->node_dst->index] = node->index;
			}
			edge_walk = edge_walk->next;
		}
	}
	path = gen_bfs_path(graph, nodes_ref);
	reset_graph_discovery(graph);
	free(nodes_ref);
	return (path);
}
