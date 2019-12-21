/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bfs.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: archid- <archid-@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/30 09:00:42 by archid-           #+#    #+#             */
/*   Updated: 2019/12/21 08:50:14 by archid-          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "queue.h"
#include "../lem_in.h"

/*
  NOTE: parents should hold each node's parents in order to reconstruct
  the part from start to sink
*/



# define AS_EDGE(o)			((t_edge *)o->blob)

static void		bfs_reset_graph(t_graph *g)
{
	t_node *walk;

	walk = g->nodes_lst;
	while (walk)
	{
		walk->seen = false;
		walk = walk->next;
	}
}


void	list_edges_dump(t_qnode *e)
{
	t_edge *walk;

	if (!e)
		return ;
	walk = e->blob;
	ft_printf("edges of source: %s\n\n", walk->node_src->name);
	while (walk)
	{
		ft_printf("L%s-%s\n", walk->node_src->name, walk->node_dst->name);
		walk = walk->next;
	}
	ft_putendl("---\n");
}

void	edge_dump(t_qnode *e)
{
	t_edge *walk;

	if (!e)
		return ;
	walk = e->blob;
	/* ft_printf("edge of source: %s\n\n", walk->node_src->name); */
	ft_printf("%s %s\n", walk->node_src->name, walk->node_dst->name);
	/* ft_putendl("---\n"); */
}

static t_queue	*bfs_helper(t_graph *g, t_edge **parent)
{
	t_queue *path;
	size_t	prev;

	if (!g || !parent[g->sink->index])
	{
		ft_putendl_fd("sink not found", 2);
		return NULL;
	}

	path = queue_init();
	prev = g->sink->index;
	while (true)
	{
		parent[prev]->seen = true;
		ft_printf("%s-%s ", parent[prev]->node_dst->name,
				  parent[prev]->node_src->name);
		queue_enq(path, queue_dry_node(parent[prev], sizeof(t_edge *)));
		if (parent[prev]->node_src == g->start)
			break ;
		prev = parent[prev]->node_src->index;
	}
	ft_putendl("\n");
	queue_iter(path, false, edge_dump);
	/* bfs_reset_graph(g); */
	return path;
}

t_queue			*bfs_find(t_graph *g)
{
	/*
	   2 or above are edges that we have found throught bfs
	   1 for those who belong into a path
	*/

	static int turn = 2;

	t_edge	**parent;
	t_qnode	*tmp;
	t_edge	*e;
	t_edge	*residual;
	t_queue *helper;

	bool	use_residual;
	t_queue *residuals;

	bool arrived;

	parent = ft_memalloc(g->n_nodes * sizeof(t_edge *));
	helper = queue_init();
	residuals = queue_init();

	e = g->start->edges;

	arrived = false;
	g->start->seen = turn;
	queue_enq(helper, queue_dry_node(g->start->edges, sizeof(t_edge *)));
	e = queue_last(helper)->blob;

	while (!arrived && queue_size(helper))
	{

		ft_putendl("current queue: ");
		queue_iter(helper, true, list_edges_dump);
		getchar();

		tmp = queue_deq(helper);

		/* e = tmp->blob; */

		/* while (e) */
		/* { */
		/* 	ft_printf("%s - %s\n", e->node_src->name, e->node_dst->name); */
		/* 	e = e->next; */
		/* } */

		e = tmp->blob;

		/* queue_node_del(&tmp, queue_node_del_dry); */

		use_residual = true;

		while (e)
		{

			ft_printf("(%s # %s) ", e->node_src->name, e->node_dst->name);
			if (e->seen == 1) /* belongs to a path */
				ft_putendl("this is a is used");

			if (e->seen != turn && e->seen != 1)
			{

				if (e->residual->seen == turn)
					queue_enq(residuals, queue_dry_node(e->residual,
														sizeof(t_edge *)));
				else
				{
					queue_enq(helper, queue_dry_node(e->node_dst->edges,
													 sizeof(t_edge *)));
					parent[e->node_dst->index] = e;
					ft_printf(" enq > (%s # %s) \n", e->node_src->name,
							  e->node_dst->name);
					/* we're at that node */
					e->node_dst->seen = turn;
					e->seen = turn;
				}
			}

			/* if both of them are not seen */
			/* how to get back using residual?? */



			if (e->node_dst == g->sink && e->seen != 1)
			{
				ft_putendl_fd("sink found!!", 2);
				getchar();
				arrived = true;
				break;
			}


			/* queue_iter(helper, false, list_edges_dump); */
			e = e->next;

		}

		ft_putstr("\n\n(residuals)\n");
		if (!queue_size(residuals))
			ft_putstr(" // no residuals\n");
		queue_iter(residuals, true, edge_dump);

		/* if (use_residual) */
		/* { */
		/* 	e = AS_EDGE(tmp); */
		/* 	residual = e->residual; */




		/* } */
		ft_putendl("\n ---\n move \n");
		getchar();
	}

	t_queue *path;

	queue_del(&helper, queue_node_del_dry);
	path = bfs_helper(g, parent);
	free(parent);

	turn++;

	return path;
	/* return NULL; */
}

t_queue		*list_shortest_paths(t_graph *graph)
{
	t_queue *paths;
	t_queue *tmp;

	paths = queue_init();
	while ((tmp = bfs_find(graph)))
	{
		/* queue_iter(tmp, true, edge_dump); */
		/* ft_putendl(" path:  \n "); */
		queue_enq(paths, queue_node(tmp, sizeof(t_queue)));
		/* FIXME: free things */
	}
	return (paths);
}

void		re_wire_paths(t_queue *paths)
{
	/* TODO:
	 *
	 * move through all the paths one edge per time
	 * if two edges have same destination, follow them
	 * see which hits on a residual edge,
	 * set  e1->next = e2->next, e2->next = f1->next
	 * repeat
	 */
}
