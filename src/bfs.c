/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bfs.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: melalj <melalj@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/30 09:00:42 by archid-           #+#    #+#             */
/*   Updated: 2020/01/09 20:18:12 by archid-          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "queue.h"
#include "../lem_in.h"



void	list_edges_dump(t_qnode *e)
{
	t_edge *walk;

	if (!e)
		return ;
	walk = e->blob;
	ft_printf("edges of source: %s\n\n", walk->node_src->name);
	while (walk)
	{
		ft_printf("<%s, %s>\n", walk->node_src->name, walk->node_dst->name);
		walk = walk->next;
	}
	ft_putendl("---\n");
}

void	edge_dump(t_qnode *e)
{
	t_edge *walk;

	if (!e || !e->blob)
		return ;
	walk = e->blob;
	/* ft_printf("edge of source: %s\n\n", walk->node_src->name); */
	// ft_printf("%s %s\n", walk->node_src->name, walk->node_dst->name);
	/* ft_putendl("---\n"); */
}

static t_queue	*bfs_helper(t_graph *g, t_edge **parent)
{
	t_queue *path;
	size_t	prev;
#ifdef USE_VISU
	t_dvisu *data;
	data = get_visu_data();
#endif
	if (!g || !parent[g->sink->index])
	{
#ifdef DEBUG
		ft_putendl_fd("sink not found", 2);
#endif
		return NULL;
	}

	path = queue_init();
	prev = g->sink->index;
	while (true)
	{
		parent[prev]->seen = true;
		// ft_printf("%s-%s ", parent[prev]->node_dst->name,
		// 		  parent[prev]->node_src->name);
#ifdef USE_VISU
		edge_draw(parent[prev], 1);
		graph_draw(g);
#endif
		queue_enq(path, queue_dry_node(parent[prev], sizeof(t_edge *)));
		if (parent[prev]->node_src == g->source)
		{
#ifdef USE_VISU
		edge_draw(parent[prev], 1);
		graph_draw(g);
#endif

			break ;
		}
		prev = parent[prev]->node_src->index;

	}

#ifdef USE_VISU
		data->path_n++;
#endif
#ifdef	USE_VISU
			graph_draw(g);
#endif

#ifdef DEBUG
	ft_putendl("\n");
#endif
	// queue_iter(path, false, edge_dump);
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
	t_queue *helper;
	t_queue *residuals;
	bool	arrived;

	parent = ft_memalloc(g->n_nodes * sizeof(t_edge *));
	helper = queue_init();
	residuals = queue_init();

	e = g->source->edges;

	arrived = false;
	g->source->seen = turn;
	queue_enq(helper, queue_dry_node(g->source->edges, sizeof(t_edge *)));
	e = queue_last(helper)->blob;

	while (!arrived && queue_size(helper))
	{
#ifdef DEBUG
		ft_putendl("current queue: ");
		queue_iter(helper, true, list_edges_dump);
		ft_putendl(" ---- ");
		/* getchar(); */
#endif
		tmp = queue_deq(helper);
		e = tmp->blob;
		while (e)
		{
#ifdef DEBUG
			ft_printf("(%s # %s) ", e->node_src->name, e->node_dst->name);
			if (e->seen == 1) /* belongs to a path */
				ft_putendl("this is a is used");
#endif
			if (e->seen != turn && e->seen != 1)
			{
				if (e->residual->seen == turn)
					queue_enq(residuals, queue_dry_node(e->residual,
														sizeof(t_edge *)));
				else if (e->node_dst->seen != turn)
				{
#ifdef USE_VISU
					edge_draw(e, -1);
					graph_draw(g);
#endif

					queue_enq(helper, queue_dry_node(e->node_dst->edges,
													 sizeof(t_edge *)));
					parent[e->node_dst->index] = e;
					// ft_printf(" enq > (%s # %s) \n", e->node_src->name,
							//   e->node_dst->name);
					/* we're at that node */
					e->node_dst->seen = turn;
					e->seen = turn;
				}
			}
			if (e->node_dst == g->sink && e->seen != 1)
			{
#ifdef DEBUG
				ft_putendl_fd("sink found!!", 2);
				getchar();
#endif
				arrived = true;
				break;
			}
			e = e->next;
		}

#ifdef DEBUG
		ft_putstr("\n\n(residuals)\n");
		if (!queue_size(residuals))
			ft_putstr(" // no residuals\n");
		queue_iter(residuals, true, edge_dump);
		ft_putendl("\n ---\n move \n");
#endif
	}

	t_queue *path;

	queue_del(&helper, queue_node_del_dry);
	path = bfs_helper(g, parent);

	free(parent);
	turn++;
	return path;
}

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
	t_queue *q;
	t_qnode *e;

	/* XXX: use a better sorting algorithm just for the sake of complexity */
	/* it won't be a big number of so bubble would be just good here, or maybe
	 * insertion sort would just be better */
	if (!base || !node || !size)
		return ;
	i = 0;
	while (i < size - 1)
	{
		j = 0;
		while (j < size - 1)
		{
			if (ft_strcmp(QNODE_AS(t_edge, node[j])->node_src->name,
						  QNODE_AS(t_edge, node[j + 1])->node_src->name) > 0)
			{

				q = base[j];
				base[j] = base[j + 1];
				base[j + 1] = q;
				e = node[j];
				node[j] = node[j + 1];
				node[j + 1] = e;

				/* memswap(base[j], base[j + 1], sizeof(t_queue *)); */
				/* memswap(node[j], node[j + 1], sizeof(t_qnode *)); */
			}
			j++;
		}
		i++;
	}
}


static bool	has_arrived(t_graph *g, t_edge *e)
{
	if (!e)
		return false;
	return (g->source == e->node_src);
}

static t_qnode		*next_edge(t_graph *g, t_queue *q, t_qnode *edge)
{
	if (!g || !edge)
		return NULL;
	return has_arrived(g, QNODE_AS(struct s_edge, edge)) ? edge: edge->next;
}

static void	set_walk_edges(t_queue *paths, t_queue **apath, t_qnode **walk_edge)
{
	t_qnode *walk;
	size_t curr;

	walk = paths->head->next;
#ifdef DEBUG
	ft_putendl("current edges:\n");
#endif
	curr = 0;
	while (walk != paths->tail)
	{
		apath[curr] = walk->blob;
		walk_edge[curr] = apath[curr]->head->next;
#ifdef USE_VISU
		edge_draw(QNODE_AS(t_edge, walk_edge[curr]), 1);
#endif
		/* edge_dump(walk_edge[curr]); */
		curr++;
		walk = walk->next;
	}

#ifdef DEBUG
	ft_putendl(" /// \n");
#endif
}

t_queue		*re_wire_paths(t_graph *g, t_queue *paths)
{
	/* BUG: re_wiring is working */

	t_queue **apath;	 /* keeping track of each path */
	t_qnode **walk_edge; /* moving through the paths one edge at a time */
	size_t	n_paths;
	size_t	curr;
	bool	collision;

	if (!paths || !(n_paths = queue_size(paths)))
	{
#ifdef DEBUG
		ft_putendl("no paths are found!");
#endif
		return NULL;
	}

	curr = 0;
	apath = malloc(n_paths * sizeof(t_queue *));
	walk_edge = malloc(n_paths * sizeof(t_qnode *));
	/* setup all found paths */
	set_walk_edges(paths, apath, walk_edge);

	bool done;
	while (true)
	{
#ifdef DEBUG
		curr = 0;
		while (curr < n_paths)
		{
			queue_iter(apath[curr++], true, edge_dump);
			ft_putendl("");
		}
#endif
		/* if all paths have arrived, that means edge is all NULLs */
		curr = 0;
		done = true;
#ifdef USE_VISU
	graph_draw(g);
	ft_putendl("moving..");
#endif

		while (curr < n_paths && done)
		{
			if (QNODE_AS(t_edge, walk_edge[curr++])->node_src != g->source)
				done = false;
		}
		if (done)
			break ;

		/* detect whether two edges have the same node_src
		 * a handy way is to edge based on node_src->name */

		/* moving one edge on each path */

		sort_by_node_src_name(apath, walk_edge, n_paths);
#ifdef DEBUG
		curr = 0;
		ft_putendl(" all edges before collision testing  ");
		while (curr < n_paths)
		{
			edge_dump(walk_edge[curr]);
			curr++;
		}

		ft_putendl(" checking the edge ");
		getchar();
#endif
		curr = 0;
		collision = false;
		while (curr < n_paths - 1)
		{
			t_qnode *e1;
			t_qnode *e2;

			e1 = next_edge(g, apath[curr], walk_edge[curr]);
			e2 = next_edge(g, apath[curr + 1], walk_edge[curr + 1]);
#ifdef DEBUG
			ft_putendl("// edges");
			edge_dump(walk_edge[curr]); ft_putstr(" >> ");		/* path 1 */
			edge_dump(e1);
			edge_dump(walk_edge[curr + 1]); ft_putstr(" >> "); /* path 2 */
			edge_dump(e2);
			ft_putendl("// ------ ");
			getchar();
#endif
			/* looking for edges that cae from the same node */
			if (/* #1 check current edges first, curr and curr + 1 */
				(!has_arrived(g, QNODE_AS(t_edge, walk_edge[curr]))
				 && !ft_strcmp(QNODE_AS(t_edge, walk_edge[curr])->node_src->name,
							   QNODE_AS(t_edge, walk_edge[curr + 1])->node_src->name))
				/* 2# or either path 2 has an edge that does */
				|| (e2 && !has_arrived(g, QNODE_AS(t_edge, e2))
					&& !ft_strcmp(QNODE_AS(t_edge, walk_edge[curr])->node_src->name,
								  QNODE_AS(t_edge, e2)->node_src->name))
				/* 3# path 1 is the one who does  */
				|| (e1 && !has_arrived(g, QNODE_AS(t_edge, e2))
					&& !ft_strcmp(QNODE_AS(t_edge, e1)->node_src->name,
								  QNODE_AS(t_edge, walk_edge[curr + 1])->node_src->name)))
			{
#ifdef DEBUG

				/* we have found a collision, two nodes have the same source */
				ft_putendl("edges having the same src node");
				edge_dump(walk_edge[curr]);		/* path 1 */
				edge_dump(e1);
				edge_dump(walk_edge[curr + 1]); /* path 2 */
				edge_dump(e2);
				ft_putendl("\n -- \n");
				getchar();
#endif
				int		residual;
				t_qnode *after1;
				t_qnode *after2;

				/*
				   here if one has not moved, hasn't moved then set
				   it to NULL and test afterwards!
				*/
				if (e1 == (after1 = next_edge(g, apath[curr], e1)))
					after1 = NULL;
				if (e2 == (after2 = next_edge(g, apath[curr + 1], e2)))
					after2 = NULL;
#ifdef DEBUG
				ft_putendl(" next of  ");
				edge_dump(walk_edge[curr]);
				ft_putendl(" is  ");
				edge_dump(e1);
				ft_putendl(" // // ");
				ft_putendl(" next of  ");
				edge_dump(walk_edge[curr + 1]);
				ft_putendl(" is  ");
				edge_dump(e2);
				ft_putendl("after 1");
				edge_dump(after1);
				ft_putendl("after 2");
				edge_dump(after2);

				getchar();
#endif
				/* preparing target edges */
				bool move_edge;

				move_edge = false;
				if (e2 && QNODE_AS(t_edge, walk_edge[curr])->residual == QNODE_AS(t_edge, e2))
					/* residual by default in this case would be path2
					 * have the residual right after one */
					residual = 1;
				/* however, in this case, the residual in the path1 itself, since after
				 * two nodes, we have found the residual */
				else if (after2 && QNODE_AS(t_edge, after2)->residual == QNODE_AS(t_edge, walk_edge[curr]))
				{
					residual = 0;
					walk_edge[curr + !residual] = after2;
					move_edge = true;
				}
				else if (after1 && QNODE_AS(t_edge, after1)->residual == QNODE_AS(t_edge, walk_edge[curr + 1]))
				{
					residual = 1;
					walk_edge[curr + !residual] = after1;
					move_edge = true;
				}
				else
					/* residual is in path 1 or in path 2 */
					residual = 0;
#ifdef DEBUG
				ft_putendl(">>> collision between \nresidual: ");
				edge_dump(walk_edge[curr + residual]->next);
				ft_putendl(" and !residual: ");
				edge_dump(walk_edge[curr + !residual]);
				ft_putendl(" // \n path of residual ");
				queue_iter(apath[curr + residual], false, edge_dump);
				ft_putendl(" path of not residual ");
				queue_iter(apath[curr + !residual], false, edge_dump);
				ft_putendl("\n /////////////////// \n");

				getchar();
#endif
				if (move_edge)
				{
#ifdef DEBUG
					ft_putendl(">>> backward in residual since the collision happens\n"
							   " ahead by an edge");
					edge_dump(walk_edge[curr + residual]);
					ft_putendl(" and now on ");
					edge_dump(walk_edge[curr + residual]->prev);
#endif
					walk_edge[curr + residual] = walk_edge[curr + residual]->prev;
				}

				walk_edge[curr + !residual] = walk_edge[curr + !residual]->prev;

#ifdef USE_VISU
		edge_draw(QNODE_AS(t_edge, walk_edge[curr + residual]), 3);
		edge_draw(QNODE_AS(t_edge, walk_edge[curr + !residual]), 3);
		graph_draw(g);
		SDL_Delay(500);
#endif
				t_qnode *old_prev = walk_edge[curr + !residual]->prev;

				/* remove residual edge from its path */
				queue_node_del_next(apath[curr + residual],
									walk_edge[curr + residual],
									queue_node_del_dry);
				walk_edge[curr + residual] = walk_edge[curr + residual]->next;

				/* removing the edge itself from its none residual path */
				queue_node_del_next(apath[curr + !residual],
									walk_edge[curr + !residual],
									queue_node_del_dry);
#ifdef DEBUG
				ft_putendl(" // \n path of residual ");
				queue_iter(apath[curr + residual], false, edge_dump);

				ft_putendl(">>> now we have ");
				edge_dump(walk_edge[curr + residual]);
				ft_putendl(" and ");
				edge_dump(walk_edge[curr + !residual]);

				ft_putendl(" ---- paths ---- \n");
				queue_iter(apath[curr + residual], true, edge_dump);
				ft_putendl(" ---- ---- ---- \n");
				queue_iter(apath[curr + !residual], true, edge_dump);
				ft_putendl(" ---- paths ---- \n");
#endif
				t_qnode *old_next = walk_edge[curr + !residual]->next;

				/* merge the other half of each path a-b 0-9 => a-9 0-b */
				queue_swap_halfs(apath[curr + !residual], apath[curr + residual],
									walk_edge[curr + !residual],
									walk_edge[curr + residual]);
				walk_edge[curr + !residual] = old_next;
#ifdef DEBUG
				ft_putendl(" ---- after merge ---- \n");
				queue_iter(apath[curr + residual], true, edge_dump);
				ft_putendl(" ---- ---- ---- \n");
				queue_iter(apath[curr + !residual], true, edge_dump);
				ft_putendl(" ---- paths ---- \n");
#endif
				if (move_edge)
					walk_edge[curr + residual] = old_prev;
#ifdef USE_VISU
				edge_draw(QNODE_AS(t_edge, walk_edge[curr + residual]), 2);
				edge_draw(QNODE_AS(t_edge, walk_edge[curr + !residual]), 2);
				graph_draw(g);
				SDL_Delay(500);
#endif

#ifdef DEBUG
				ft_putendl("  collision edges after ");
				edge_dump(walk_edge[curr + residual]);
				edge_dump(walk_edge[curr + !residual]);
				ft_putendl(" ////// current //////  ");
				edge_dump(walk_edge[curr]);
				edge_dump(walk_edge[curr + 1]);
				ft_putendl(" ////////////  ");
				getchar();
#endif
				collision = true;
				break;
			}
			curr++;
		}

#ifdef DEBUG
		curr = 0;
		ft_putendl(" all edges after collision testing  ");
		while (curr < n_paths)
		{
			edge_dump(walk_edge[curr]);
			curr++;
		}
#endif
		if (collision)
		{
#ifdef DEBUG
			ft_putendl(" /// collision ");
			getchar();
#endif
			set_walk_edges(paths, apath, walk_edge); /* source from the sink again */
			continue ;
		}
		curr = 0;
#ifdef DEBUG
		ft_putendl(" moving edges \n");
#endif
		while (curr < n_paths)
		{
#ifdef DEBUG
			ft_putendl(" old ");
			edge_dump(walk_edge[curr]);
#endif
			/* ignoring paths that have reached the source */
			if (QNODE_AS(t_edge, walk_edge[curr])->node_src != g->source)
				walk_edge[curr] = walk_edge[curr]->next;
#ifdef DEBUG
			else
				ft_printf("already arrived! -> ");
			edge_dump(walk_edge[curr]);
#endif
#ifdef USE_VISU
			edge_draw(QNODE_AS(t_edge, walk_edge[curr]), 1);
			graph_draw(g);
			ft_putendl("coloring moving edge");
#endif
			curr++;
		}
	}
	free(apath);
	free(walk_edge);
	return (paths);
}
