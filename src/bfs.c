/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bfs.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: melalj <melalj@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/30 09:00:42 by archid-           #+#    #+#             */
/*   Updated: 2020/11/14 18:11:29 by archid-          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../lem_in.h"
#include "queue.h"

/*
  2 or above are edges that we have found throught bfs
  1 for those who belong into a path
*/

static int g_turn = 2;

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

static void handle_edge(t_edge *edge, t_queue *residuals, t_queue *open,
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

static void bfs_loop(t_graph *g, t_queue *residuals, t_edge **parent) {
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
