/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bfs.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: melalj <melalj@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/30 09:00:42 by archid-           #+#    #+#             */
/*   Updated: 2020/11/24 03:37:20 by archid-          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "netflow.h"

/*
  2 or above are edges that we have found throught bfs
  1 for those who belong into a path
*/

static int g_turn = 2;

void node_dump(t_qnode *node) {
	t_edge *edge = node->blob;
	ft_printf("%s-%s ", edge->src->name, edge->dst->name);
}

static t_queue *bfs_helper(t_graph *g, t_hash *parent) {
    t_queue *path;
	t_edge *tmp;
    char *prev;

    if (!g || !hash_get(parent, g->sink->name, NULL))
        return NULL;
    path = queue_init();
    prev = g->sink->name;
    while (true) {
		tmp = hash_get(parent, prev, NULL);
        tmp->seen = true;
        queue_enq(path, queue_node(tmp, sizeof(t_edge *), false));
        if (tmp->src == g->source)
            break;
        prev = tmp->src->name;
    }
    return path;
}

static void handle_edge(t_edge *edge, t_queue *residuals,
						t_queue *open, t_hash *parent) {
	t_hnode hnode;

    if (edge->residual->seen == g_turn)
        queue_enq(residuals, queue_node(edge->residual, sizeof(t_edge *), false));
    else if (edge->dst->seen != g_turn) {
        queue_enq(open,
                  queue_node(edge->dst->edges, sizeof(t_edge *), false));
		hnode = (t_hnode){edge->dst->name, edge};
		hash_add(parent, &hnode);
        edge->dst->seen = g_turn;
        edge->seen = g_turn;
    }
}

t_edge *extract_edge(t_queue *q) {
	t_edge *e;
    t_qnode *tmp;

	tmp = queue_deq(q);
	e = tmp->blob;
	queue_node_del(&tmp, queue_blob_keep);
	return e;
}

void enqueue_edges(t_queue *q, t_queue *other) {
	t_qnode *walk;

	walk = QFIRST(other);
	while (walk != QTAIL(other)) {
		queue_enq(q, queue_node(walk->blob, sizeof(t_edge *), false));
		walk = walk->next;
	}
}

static void bfs_loop(t_graph *g, t_queue *residuals, t_hash *parent) {
    t_queue *open;
	t_edge *edge;
    bool arrived;

    arrived = false;
    open = queue_init();
	enqueue_edges(open, g->source->edges);
    while (!arrived && queue_size(open)) {
		edge = extract_edge(open);
		if (edge->seen != g_turn && edge->seen != 1)
			handle_edge(edge, residuals, open, parent);
		if (edge->dst == g->sink && edge->seen != 1)
			arrived = true;
    }
    queue_del(&open, queue_blob_keep);
}

t_queue *bfs_find(t_graph *g) {
    t_hash	*parent;
    t_queue *residuals;
    t_queue *path;

    parent = hash_init(g->n_vertices, hash_default_del);
    residuals = queue_init();
    g->source->seen = g_turn;
    bfs_loop(g, residuals, parent);
    path = bfs_helper(g, parent);
    g_turn++;

	ft_printf("\n");
	ft_printf("%d>> ", g_turn);
	queue_iter(path, false, node_dump);

    free(parent);
    return path;
}
