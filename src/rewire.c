/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rewire.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: archid- <archid-@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/26 23:46:38 by archid-           #+#    #+#             */
/*   Updated: 2020/12/03 12:37:53 by archid-          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "netflow.h"

static bool detect_collision(t_graph *g, struct s_rewire_handy *info) {
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
        || (info->e1 && !has_arrived(g, AS_EDGE(info->e2)) &&
            !ft_strcmp(AS_EDGE(info->e1)->src->name,
                       AS_EDGE(info->walk_edge[info->curr + 1])->src->name));
}

static void discover_collision(t_graph *g, struct s_rewire_handy *info,
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
    /* bool move_edge; */

    /* XXX: fill residual */
    /* move_edge = false; */
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
        /* move_edge = true; */
    } else if (after1 && AS_EDGE(after1)->residual ==
                             AS_EDGE(info->walk_edge[info->curr + 1])) {
        *which_residual = 1;
        info->walk_edge[info->curr] = after1;
        /* move_edge = true; */
    } else {
        *which_residual = 0;
    }
    /* return move_edge; */
}

static void remove_residuals(struct s_rewire_handy *info, int index, t_vertex *src) {
	ft_printf("remove residuals of %d\n", index);
	ft_printf(" => vertex %s\n", src->name);

	while (AS_EDGE(info->walk_edge[index]->next)->dst != src)
	{
		node_dump(info->walk_edge[index]->next);
		ft_putendl("");

		/* release edge */
		AS_EDGE(info->walk_edge[index]->next)->seen = FRESH;

		/* remove residual edge from its path */
		queue_node_del_next(info->apath[index], info->walk_edge[index], queue_blob_keep);
	}

	ft_putstr(" ==> ");
	node_dump(info->walk_edge[index]->next);
	ft_putendl(" <==\n");

	ft_putendl("\n ********* \n");
}

static void fix_collision(t_graph *g, struct s_rewire_handy *info) {
    int residual;
    bool move_edge;

	discover_collision(g, info, &residual);

	ft_printf("residual is in %d\n", residual);

	while (AS_EDGE(info->walk_edge[info->curr + residual])->residual->seen == BELONG_TO_PATH)
		info->walk_edge[info->curr + residual] = info->walk_edge[info->curr + residual]->prev;

	while (AS_EDGE(info->walk_edge[info->curr + !residual])->residual->seen == BELONG_TO_PATH)
		info->walk_edge[info->curr + !residual] = info->walk_edge[info->curr + !residual]->prev;

	t_qnode *walk;

	t_vertex *residual_src = AS_EDGE(info->walk_edge[info->curr + residual])->src;
	t_vertex *not_residual_src = AS_EDGE(info->walk_edge[info->curr + !residual])->src;

	{
		ft_putendl(" ================ BEFORE ================ ");

		walk = info->walk_edge[info->curr + residual];
		ft_putendl("residual to tail ");
		while (walk != QTAIL(info->apath[info->curr + residual])) {
			node_dump(walk); ft_putendl("");
			walk = walk->next;
		}
		ft_putendl("\n -------------- \n");

		walk = info->walk_edge[info->curr + !residual];
		ft_putendl("not residual to tail ");
		while (walk != QTAIL(info->apath[info->curr + !residual])) {
			node_dump(walk); ft_putendl("");
			walk = walk->next;
		}
		ft_putendl("\n -------------- \n");
	}

	remove_residuals(info, info->curr + residual, not_residual_src);
	info->walk_edge[info->curr + residual] = info->walk_edge[info->curr + residual]->next;
	remove_residuals(info, info->curr + !residual, residual_src);

	{
		ft_putendl(" ================ AFTER ================ ");

		walk = info->walk_edge[info->curr + residual];
		ft_putendl("residual to tail ");
		while (walk != QTAIL(info->apath[info->curr + residual])) {
			node_dump(walk); ft_putendl("");
			walk = walk->next;
		}
		ft_putendl("\n -------------- \n");

		walk = info->walk_edge[info->curr + !residual];
		ft_putendl("not residual to tail ");
		while (walk != QTAIL(info->apath[info->curr + !residual])) {
			node_dump(walk); ft_putendl("");
			walk = walk->next;
		}
		ft_putendl("\n -------------- \n");
	}

    /* merge the other half of each path a-b 0-9 => a-9 0-b */
    queue_swap_halfs(info->apath[info->curr + !residual],
                     info->apath[info->curr + residual],
                     info->walk_edge[info->curr + !residual],
                     info->walk_edge[info->curr + residual]);
}

static bool handle_collision(t_graph *g, struct s_rewire_handy *info) {
    bool collision;

    info->curr = 0;
    collision = false;
    while (info->curr < info->n_paths - 1) {
        pick_pair(g, info);
        if (detect_collision(g, info)) {
            fix_collision(g, info);
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
