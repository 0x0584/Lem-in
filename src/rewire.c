#include "../lem_in.h"

static bool detect_collition(t_graph *g, struct s_rewire_handy *info) {
    /* looking for edges that cae from the same node */
    return /* #1 check current edges first, curr and curr + 1 */
        (!has_arrived(g, AS_EDGE(info->walk_edge[info->curr])) &&
         !ft_strcmp(AS_EDGE(info->walk_edge[info->curr])->src->name,
                    AS_EDGE(info->walk_edge[info->curr + 1])->src->name))
        /* 2# or either path 2 has an edge that does */
        || (info->e2 && !has_arrived(g, AS_EDGE(info->e2)) &&
            !ft_strcmp(AS_EDGE(info->walk_edge[info->curr])->src->name,
                       AS_EDGE(info->e2)->src->name))
        /* 3# path 1 is the one who does  */
        ||
        (info->e1 && !has_arrived(g, AS_EDGE(info->e2)) &&
         !ft_strcmp(AS_EDGE(info->e1)->src->name,
                    AS_EDGE(info->walk_edge[info->curr + 1])->src->name));
}

static bool discover_collition(t_graph *g, struct s_rewire_handy *info,
							   int *residual) {
    t_qnode *after1;
    t_qnode *after2;

    /*
      here if one has not moved, hasn't moved then set
      it to NULL and test afterwards!
    */

    if (info->e1 == (after1 = next_edge(g, info->e1)))
        after1 = NULL;
    if (info->e2 == (after2 = next_edge(g, info->e2)))
        after2 = NULL;

    /* preparing target edges */
    bool move_edge;

    move_edge = false;
    if (info->e2 &&
        AS_EDGE(info->walk_edge[info->curr])->residual == AS_EDGE(info->e2))
        /* residual by default in this case would be path2
         * have the residual right after one */
        *residual = 1;
    else if (after2 && AS_EDGE(after2)->residual ==
                           AS_EDGE(info->walk_edge[info->curr])) {
        /* however, in this case, the residual in the path1 itself, since after
         * two nodes, we have found the residual */
        *residual = 0;
        info->walk_edge[info->curr + 1] = after2;
        move_edge = true;
    } else if (after1 && AS_EDGE(after1)->residual ==
                             AS_EDGE(info->walk_edge[info->curr + 1])) {
        *residual = 1;
        info->walk_edge[info->curr] = after1;
        move_edge = true;
    } else
        *residual = 0;
    return move_edge;
}

static void fix_collition(t_graph *g, struct s_rewire_handy *info) {
    int residual;
    bool move_edge = discover_collition(g, info, &residual);

    if (move_edge)
        info->walk_edge[info->curr + residual] =
            info->walk_edge[info->curr + residual]->prev;

    info->walk_edge[info->curr + !residual] =
        info->walk_edge[info->curr + !residual]->prev;

    t_qnode *old_prev = info->walk_edge[info->curr + !residual]->prev;

    /* remove residual edge from its path */
    queue_node_del_next(info->apath[info->curr + residual],
                        info->walk_edge[info->curr + residual],
                        queue_node_del_dry);
    info->walk_edge[info->curr + residual] =
        info->walk_edge[info->curr + residual]->next;

    /* removing the edge itself from its none residual path */
    queue_node_del_next(info->apath[info->curr + !residual],
                        info->walk_edge[info->curr + !residual],
                        queue_node_del_dry);

    t_qnode *old_next = info->walk_edge[info->curr + !residual]->next;

    /* merge the other half of each path a-b 0-9 => a-9 0-b */
    queue_swap_halfs(info->apath[info->curr + !residual],
                     info->apath[info->curr + residual],
                     info->walk_edge[info->curr + !residual],
                     info->walk_edge[info->curr + residual]);
    info->walk_edge[info->curr + !residual] = old_next;

    if (move_edge)
        info->walk_edge[info->curr + residual] = old_prev;
}


static bool handle_collision(t_graph *g, struct s_rewire_handy *info) {
	bool collision;

	info->curr = 0;
	collision = false;
	while (info->curr < info->n_paths - 1) {
		pick_pair(g, info);
		if (detect_collition(g, info)) {
			fix_collition(g, info);
			collision = true;
			break;
		}
		info->curr++;
	}
	return collision;
}

t_queue *re_wire_paths(t_graph *g, t_queue *paths) {
    struct s_rewire_handy info;

    if (!prepare_info(&info, paths))
        return NULL;
    while (!rewire_done(g, &info)) {
        sort_by_src_name(&info);
        if (handle_collision(g, &info)) {
            set_walk_edges(paths, &info); /* start from the sink again */
            continue;
        }
        walk_edges(g, &info);
    }

	ft_printf("\n\n");
	for (int i = 0; i < info.n_paths; ++i) {
		ft_printf("R %d>> ", i);
		queue_iter(info.apath[i], false, node_dump);
		ft_printf("\n");
	}
	ft_printf("\n\n");

    free(info.apath);
    free(info.walk_edge);
    return (paths);
}
