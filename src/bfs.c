/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bfs.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: archid- <archid-@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/30 09:00:42 by archid-           #+#    #+#             */
/*   Updated: 2019/12/23 22:17:05 by archid-          ###   ########.fr       */
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
		/* // getchar(); */

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
				// getchar();
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
		/* // getchar(); */
	}

	t_queue *path;

	queue_del(&helper, queue_node_del_dry);
	path = bfs_helper(g, parent);
	free(parent);

	turn++;

	return path;
	/* return NULL; */
}


/* this seems to work but kinda hard to impelement, since paths could cross */
/* paths: such that head -> sink ... -> source -> tail */

typedef struct		s_pair
{
	t_qnode		*e1;
	t_qnode		*e2;
}					t_pair;

void		memswap(void *foo, void *bar, size_t size)
{
	void *tmp;

	tmp = malloc(size);
	ft_memcpy(tmp, bar, size);
	ft_memcpy(bar, foo, size);
	ft_memcpy(foo, tmp, size);
	free(tmp);
}

void		sort_by_node_src_name(t_queue **base, t_qnode **node, size_t size)
{

	size_t i;
	size_t j;

	/* FIXME: use a better sorting algorithm just for the sake of complexity */

	if (!base || !node || !size)
		return ;
	i = 0;
	while (i < size - 1)
	{
		j = 0;
		while (j < size - 1)
		{
			if (!ft_strcmp(AS_EDGE(node[j])->node_src->name,
						   AS_EDGE(node[j + 1])->node_src->name))
			{
				memswap(base[j], base[j + 1], sizeof(t_queue *));
				memswap(node[j], node[j + 1], sizeof(t_qnode *));
			}
			j++;
		}
		i++;
	}
}

t_queue		*re_wire_paths(t_graph *g, t_queue *paths)
{
	/* TODO:
	 *
	 * move through all the paths one edge per time
	 * if two edges have same destination, follow them
	 * see which hits on a residual edge,
	 * set  e1->next = e2->next, e2->next = f1->next
	 * repeat
	 */

	t_qnode *walk;

	t_queue **apath; /* keeping tack of each path */
	t_qnode **walk_edge;	 /* moving through the paths one edge at a time */
	size_t	n_paths;
	size_t	curr;

	if (!paths || !(n_paths = queue_size(paths)))
	{
		ft_putendl("no paths are found!");
		return NULL;
	}

	curr = 0;
	walk = paths->head->next;
	apath = malloc(n_paths * sizeof(t_queue *));
	walk_edge = malloc(n_paths * sizeof(t_qnode *));

	/* setup all found paths */
	ft_putendl("current edges:\n");
	while (walk != paths->tail)
	{
		apath[curr] = walk->blob;
		walk_edge[curr] = apath[curr]->head->next;
		edge_dump(walk_edge[curr]);
		curr++;
		walk = walk->next;
	}
	ft_putendl(" /// \n");

	bool done;

	while (true)
	{
		curr = 0;
		while (curr < n_paths)
		{
			queue_iter(apath[curr++], true, edge_dump);
			ft_putendl("");
		}

		/* if all paths have arrived, that means edge is all NULLs */
		curr = 0;
		done = true;
		while (curr < n_paths && done)
			if (AS_EDGE(walk_edge[curr++])->node_src != g->start)
				done = false;
		if (done)
			break ;

		/* detect whether two edges have the same node_src
		 * a handy way is to edge based on node_src->name */

		/* moving one edge on each path */

		sort_by_node_src_name(apath, walk_edge, n_paths);

		ft_putendl(" checking the edge ");
		// getchar();

		curr = 0;
		while (curr < n_paths - 1)
		{
			if (!ft_strcmp(AS_EDGE(walk_edge[curr])->node_src->name,
							AS_EDGE(walk_edge[curr + 1])->node_src->name)
					&& AS_EDGE(walk_edge[curr])->node_src != g->start)
			{
				/* we have found a collision, two nodes have the same start */
				ft_putendl("edges having the same src node");
				edge_dump(walk_edge[curr]);	 /* path 1 */
				edge_dump(walk_edge[curr + 1]); /* path 2 */
				ft_putendl("\n -- \n");
				// getchar();

				t_qnode *e1;
				t_qnode *e2;

				int residual;

				e1 = walk_edge[curr]->next;
				e2 = walk_edge[curr + 1]->next;

				ft_putendl(" next of  ");
				edge_dump(walk_edge[curr]);
				ft_putendl(" is  ");
				edge_dump(e1);
				ft_putendl(" having residual  ");
				ft_printf("%s %s\n", AS_EDGE(e1)->residual->node_src->name,
							AS_EDGE(e1)->residual->node_dst->name);
				ft_putendl(" // // ");
				ft_putendl(" next of  ");
				edge_dump(walk_edge[curr + 1]);
				ft_putendl(" is  ");
				edge_dump(e2);
				ft_putendl(" having residual  ");
				ft_printf("%s %s\n", AS_EDGE(e2)->residual->node_src->name,
							AS_EDGE(e2)->residual->node_dst->name);

				/* if (AS_EDGE(e1)->seen && AS_EDGE(e1)->residual->seen) */
				if (AS_EDGE(walk_edge[curr])->residual == AS_EDGE(e2))
				{
					residual = 1; /* residual is in path 2 */

					ft_putendl("curr is the node and e2 is residual walk_edge[curr + 1]");
					edge_dump(walk_edge[curr]);
					edge_dump(walk_edge[curr + 1]->next);
					/* edge[curr] is an edge of path 1 that
					 * have e2 (edge[curr + 1]) as its residual */
				}
				else
				{
					residual = 0;  /* residual is in path 1 */
					ft_putendl("curr is the node and e1 is residual walk_edge[curr]");
					edge_dump(walk_edge[curr]);
				}

				/* ft_putendl(">>> residual is: "); */
				/* ft_printf("%s %s\n", AS_EDGE(residual)->node_src->name, */
				/* 			AS_EDGE(residual)->node_dst->name); */

				ft_putendl(">>> collision between ");
				edge_dump(walk_edge[curr + residual]);
				ft_putendl(" and ");
				edge_dump(walk_edge[curr + !residual]);

				ft_putendl("\n -- \n");

				/* remove residual edge from path 2 */
				queue_node_del_next(apath[curr + residual],
									walk_edge[curr + residual],
									queue_node_del_dry);
				walk_edge[curr + residual] = walk_edge[curr + residual]->next;

				/* removing the edge itself from path 1 */
				walk_edge[curr + !residual] = walk_edge[curr + !residual]->prev;
				queue_node_del_next(apath[curr + !residual],
									walk_edge[curr + !residual],
									queue_node_del_dry);

				ft_putendl(">>> now we have ");
				edge_dump(walk_edge[curr + residual]);
				ft_putendl(" and ");
				edge_dump(walk_edge[curr + !residual]);

				t_qnode *old_next = walk_edge[curr + !residual]->next;

				/* merge the other half of each path a-b 0-9 => a-9 0-b */
				ft_putendl(" ---- paths ---- \n");
				queue_iter(apath[curr + residual], true, edge_dump);
				ft_putendl(" ---- ---- ---- \n");
				queue_iter(apath[curr + !residual], true, edge_dump);
				ft_putendl(" ---- paths ---- \n");

				queue_swap_halfs(apath[curr + !residual],
								 apath[curr + residual],
								 walk_edge[curr + !residual],
								 walk_edge[curr + residual]);

				walk_edge[curr + !residual] = old_next;

				ft_putendl(" ---- after merge ---- \n");
				queue_iter(apath[curr + residual], true, edge_dump);
				ft_putendl(" ---- ---- ---- \n");
				queue_iter(apath[curr + !residual], true, edge_dump);
				ft_putendl(" ---- paths ---- \n");

				ft_putendl("  collision edges after ");
				edge_dump(walk_edge[curr + residual]);
				edge_dump(walk_edge[curr + !residual]);
				// getchar();
			}
			curr++;
		}

		curr = 0;
		ft_putendl(" all edges after collision testing  ");
		while (curr < n_paths)
		{
			edge_dump(walk_edge[curr]);
			curr++;
		}

		curr = 0;
		ft_putendl(" moving edges \n");

		while (curr < n_paths)
		{
			ft_putendl(" old ");
			edge_dump(walk_edge[curr]);
			/* ignoring paths that have reached the source */
			if (AS_EDGE(walk_edge[curr])->node_src != g->start)
				walk_edge[curr] = walk_edge[curr]->next;
			else
				ft_printf("already arrived! -> ");
			edge_dump(walk_edge[curr]);

			ft_putendl(" /// ");
			curr++;
		}

		// getchar();
	}

	free(apath);
	free(walk_edge);
	return (paths);
}
