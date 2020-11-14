/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bfs.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: melalj <melalj@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/30 09:00:42 by archid-           #+#    #+#             */
/*   Updated: 2020/11/14 17:15:51 by archid-          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../lem_in.h"
#include "queue.h"

/*
  2 or above are edges that we have found throught bfs
  1 for those who belong into a path
*/

static int g_turn = 2;

struct s_rewire_handy {
    t_queue **apath;     /* keeping track of each path */
    t_qnode **walk_edge; /* moving through the paths one edge at a time */
    size_t n_paths;      /* number of paths */
    size_t curr;         /* walk index */

    /* edge pair used while walking */
    t_qnode *e1;
    t_qnode *e2;
};

static t_queue *bfs_helper(t_graph *g, t_edge **parent) {
    t_queue *path;
    size_t prev;

    if (!g || !parent[g->sink->index])
        return NULL;
    path = queue_init();
    prev = g->sink->index;
    while (true) {
        parent[prev]->seen = true;
        queue_enq(path, queue_dry_node(parent[prev], sizeof(t_edge *)));
        if (parent[prev]->node_src == g->start)
            break;
        prev = parent[prev]->node_src->index;
    }
    return path;
}

void handle_edge(t_edge *edge, t_queue *residuals, t_queue *open,
                 t_edge **parent) {
    if (edge->residual->seen == g_turn)
        queue_enq(residuals, queue_dry_node(edge->residual, sizeof(t_edge *)));
    else if (edge->node_dst->seen != g_turn) {
        queue_enq(open,
                  queue_dry_node(edge->node_dst->edges, sizeof(t_edge *)));
        parent[edge->node_dst->index] = edge;
        edge->node_dst->seen = g_turn;
        edge->seen = g_turn;
    }
}

void bfs_loop(t_graph *g, t_queue *residuals, t_edge **parent) {
    t_edge *walk;
    t_qnode *tmp;
    t_queue *helper;
    bool arrived;

    arrived = false;
    helper = queue_init();
    queue_enq(helper, queue_dry_node(g->start->edges, sizeof(t_edge *)));
    while (!arrived && queue_size(helper)) {
        tmp = queue_deq(helper);
        walk = tmp->blob;
        while (!arrived && walk) {
            if (walk->seen != g_turn && walk->seen != 1)
                handle_edge(walk, residuals, helper, parent);
            if (walk->node_dst == g->sink && walk->seen != 1)
                arrived = true;
            walk = walk->next;
        }
        queue_node_del(&tmp, queue_node_del_dry);
    }
    queue_del(&helper, queue_node_del_dry);
}

t_queue *bfs_find(t_graph *g) {
    t_edge **parent;
    t_queue *residuals;
    t_queue *path;

    parent = ft_memalloc(g->n_nodes * sizeof(t_edge *));
    residuals = queue_init();
    g->start->seen = g_turn;
    bfs_loop(g, residuals, parent);
    path = bfs_helper(g, parent);
    free(parent);
    g_turn++;
    return path;
}

void sort_by_node_src_name(struct s_rewire_handy *info) {

    size_t i;
    size_t j;
    t_queue *q;
    t_qnode *e;

    if (!info->apath || !info->walk_edge || !info->n_paths)
        return;
    i = 0;
    while (i < info->n_paths - 1) {
        j = 0;
        while (j < info->n_paths - 1) {
            if (ft_strcmp(AS_EDGE(info->walk_edge[j])->node_src->name,
                          AS_EDGE(info->walk_edge[j + 1])->node_src->name) >
                0) {
                q = info->apath[j];
                info->apath[j] = info->apath[j + 1];
                info->apath[j + 1] = q;
                e = info->walk_edge[j];
                info->walk_edge[j] = info->walk_edge[j + 1];
                info->walk_edge[j + 1] = e;
            }
            j++;
        }
        i++;
    }
}

static bool has_arrived(t_graph *g, t_edge *e) {
    if (!e)
        return false;
    return (g->start == e->node_src);
}

static t_qnode *next_edge(t_graph *g, t_qnode *edge) {
    if (!g || !edge)
        return NULL;
    return has_arrived(g, QNODE_AS(struct s_edge, edge)) ? edge : edge->next;
}

static void set_walk_edges(t_queue *paths, struct s_rewire_handy *info) {
    t_qnode *walk;
    size_t curr;

    walk = paths->head->next;
    curr = 0;
    while (walk != paths->tail) {
        info->apath[curr] = walk->blob;
        info->walk_edge[curr] = info->apath[curr]->head->next;
        curr++;
        walk = walk->next;
    }
}

bool detect_collition(t_graph *g, struct s_rewire_handy *info) {
    /* looking for edges that cae from the same node */
    return /* #1 check current edges first, curr and curr + 1 */
        (!has_arrived(g, AS_EDGE(info->walk_edge[info->curr])) &&
         !ft_strcmp(AS_EDGE(info->walk_edge[info->curr])->node_src->name,
                    AS_EDGE(info->walk_edge[info->curr + 1])->node_src->name))
        /* 2# or either path 2 has an edge that does */
        || (info->e2 && !has_arrived(g, AS_EDGE(info->e2)) &&
            !ft_strcmp(AS_EDGE(info->walk_edge[info->curr])->node_src->name,
                       AS_EDGE(info->e2)->node_src->name))
        /* 3# path 1 is the one who does  */
        ||
        (info->e1 && !has_arrived(g, AS_EDGE(info->e2)) &&
         !ft_strcmp(AS_EDGE(info->e1)->node_src->name,
                    AS_EDGE(info->walk_edge[info->curr + 1])->node_src->name));
}

bool discover_collition(t_graph *g, struct s_rewire_handy *info,
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

void fix_collition(t_graph *g, struct s_rewire_handy *info) {
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

bool prepare_info(struct s_rewire_handy *info, t_queue *paths) {
    if (!paths || !(info->n_paths = queue_size(paths)))
        return NULL;

    info->apath = malloc(info->n_paths * sizeof(t_queue *));
    info->walk_edge = malloc(info->n_paths * sizeof(t_qnode *));
    /* setup all found paths */
    set_walk_edges(paths, info);
    return true;
}

void pick_pair(t_graph *g, struct s_rewire_handy *info) {
    info->e1 = next_edge(g, info->walk_edge[info->curr]);
    info->e2 = next_edge(g, info->walk_edge[info->curr + 1]);
}

void walk_edges(t_graph *g, struct s_rewire_handy *info) {
    info->curr = 0;
    while (info->curr < info->n_paths) {
        /* ignoring paths that have reached the source */
        if (AS_EDGE(info->walk_edge[info->curr])->node_src != g->start)
            info->walk_edge[info->curr] = info->walk_edge[info->curr]->next;
        info->curr++;
    }
}

t_queue *re_wire_paths(t_graph *g, t_queue *paths) {
    struct s_rewire_handy info;
    bool collision;
    bool done;

    if (!prepare_info(&info, paths))
        return NULL;

    while (true) {
        /* if all paths have arrived, that means edge is all NULLs */
        info.curr = 0;
        done = true;
        while (info.curr < info.n_paths && done)
            if (AS_EDGE(info.walk_edge[info.curr++])->node_src != g->start)
                done = false;
        if (done)
            break;

        /* detect whether two edges have the same node_src
         * a handy way is to edge based on node_src->name */

        /* moving one edge on each path */

        sort_by_node_src_name(&info);
        info.curr = 0;
        collision = false;
        while (info.curr < info.n_paths - 1) {
            pick_pair(g, &info);
            if (detect_collition(g, &info)) {
                fix_collition(g, &info);
                collision = true;
                break;
            }
            info.curr++;
        }

        if (collision) {
            set_walk_edges(paths, &info); /* start from the sink again */
            continue;
        }
        walk_edges(g, &info);
    }

    free(info.apath);
    free(info.walk_edge);
    return (paths);
}
