/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rewire.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: archid- <archid-@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/26 23:46:38 by archid-           #+#    #+#             */
/*   Updated: 2020/11/28 03:53:49 by archid-          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "netflow.h"

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
							   int *which_residual) {
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

	/* FIXME: fill residual */
    move_edge = false;
    if (info->e2 &&
        (AS_EDGE(info->walk_edge[info->curr])->residual) == AS_EDGE(info->e2))
        /* residual by default in this case would be path2
         * have the residual right after one */
        *which_residual = 1;
    else if (after2 && AS_EDGE(after2)->residual ==
                           AS_EDGE(info->walk_edge[info->curr])) {
        /* however, in this case, the residual in the path1 itself, since after
         * two nodes, we have found the residual */
        *which_residual = 0;
        info->walk_edge[info->curr + 1] = after2;
        move_edge = true;
    } else if (after1 && AS_EDGE(after1)->residual ==
                             AS_EDGE(info->walk_edge[info->curr + 1])) {
        *which_residual = 1;
        info->walk_edge[info->curr] = after1;
        move_edge = true;
    } else
        *which_residual = 0;
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

	AS_EDGE(info->walk_edge[info->curr + residual]->next)->seen = FRESH;

    /* remove residual edge from its path */
    queue_node_del_next(info->apath[info->curr + residual],
                        info->walk_edge[info->curr + residual],
                        queue_blob_keep);
    info->walk_edge[info->curr + residual] =
        info->walk_edge[info->curr + residual]->next;

	AS_EDGE(info->walk_edge[info->curr + !residual]->next)->seen = FRESH;

    /* removing the edge itself from its none residual path */
    queue_node_del_next(info->apath[info->curr + !residual],
                        info->walk_edge[info->curr + !residual],
                        queue_blob_keep);

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

void update_path_lengths(t_qnode *node) {
	t_queue *q;

	q = node->blob;
	q->size = queue_count(q);
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
    free(info.apath);
    free(info.walk_edge);
	queue_iter(paths, true, update_path_lengths);
    return (paths);
}
