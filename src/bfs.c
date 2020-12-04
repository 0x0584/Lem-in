/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bfs.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: archid- <archid-@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/24 22:52:55 by archid-           #+#    #+#             */
/*   Updated: 2020/12/04 18:31:07 by archid-          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "netflow.h"

/*
  2 or above are edges that we have found throught bfs
  1 for those who belong into a path
*/


enum e_state g_turn = INITIAL;

void edge_print(t_edge *edge) {
	if (edge)
		ft_printf("[%s(%d)-%s(%d)|%d] ", edge->src->name, edge->src->seen,
				  edge->dst->name, edge->dst->seen, edge->seen);
}

void node_dump(t_qnode *node) { edge_print(node->blob); }

void dump_hash_map(t_hash *hash) {
	t_qnode *walk;
	size_t i = 0;
	t_hnode *tmp;

	while (i < hash->size) {
		if (hash->array[i]) {
			walk = QFIRST(hash->array[i]);
			while (walk != QTAIL(hash->array[i])) {
				tmp = walk->blob;
				ft_printf("Key: `%s` \n", tmp->key);
				queue_iter(tmp->blob, true, node_dump);
				ft_putendl("\n-----------\n");
				walk = walk->next;
			}
		}
		i++;
	}
}

bool edge_state(t_edge *edge, t_edge *backwards) {
    ft_printf("Edge: ");
    edge_print(edge);
    ft_printf(" ");

    if (!backwards) {
        ft_printf("from sink\n");
        return true;
    }

    ft_printf("Previous: ");
    edge_print(backwards);
    ft_putstr(" >> ");

    if (edge->src->seen == backwards->src->seen &&
			backwards->src->seen != BELONG_TO_PATH) {
        ft_printf("fresh edge\n");
        return true;
    } else if (edge->src->seen == BELONG_TO_PATH) {
        if (edge->residual->seen == BELONG_TO_PATH) {
            ft_printf("source belong to residual\n");
        } else {
            ft_printf("source coming from fresh\n");
        }
        return true;
    } else if (edge->dst->seen == BELONG_TO_PATH &&
               backwards->residual->seen == BELONG_TO_PATH) {
        ft_printf("destination belong to path\n");
        return true;
    } else {
        ft_printf("wrong edge\n");
        return false;
    }
}

static t_queue *traverse_hash(t_graph *g, char *key, t_hash *parent,
                              t_edge *backwards) {
    t_queue *path;
    t_queue *prev;
    t_qnode *walk;
    t_edge *edge;

    if (!(prev = hash_get(parent, key, NULL)))
        return NULL;
    walk = QFIRST(prev);
    while (walk != QTAIL(prev)) {
        edge = walk->blob;

        bool state = edge_state(edge, backwards);

        /* ft_printf("state: %d\n", state); */

        if (state &&
            (path = (edge->src == g->source
                         ? queue_init()
                         : traverse_hash(g, edge->src->name, parent, edge)))) {
            edge->src->seen = BELONG_TO_PATH;
            edge->seen = BELONG_TO_PATH;
            return queue_push_front(path,
                                    queue_node(edge, sizeof(t_edge *), false));
        }
        walk = walk->next;
    }
    return NULL;
}

static t_queue *construct_path(t_graph *g, t_hash *parent) {
    return traverse_hash(g, g->sink->name, parent, NULL);
}

static t_edge *extract_edge(t_queue *q) {
    t_edge *e;
    t_qnode *tmp;

    tmp = queue_deq(q);
    e = tmp->blob;
    queue_node_del(&tmp, queue_blob_keep);
    return e;
}

static void enqueue_edges(t_queue *q, t_queue *other) {
    t_qnode *walk;
	t_edge *edge;

    walk = QFIRST(other);
    while (walk != QTAIL(other)) {
		edge = walk->blob;
		ft_printf(" enqueuing: %s-%s\n", edge->src->name, edge->dst->name);
        queue_enq(q, queue_node(walk->blob, sizeof(t_edge *), false));
        walk = walk->next;
    }
}

static void hash_add_parent(t_edge *edge, t_hash *parent) {
    t_queue *tmp;

    if (!(tmp = hash_get(parent, edge->dst->name, NULL)))
        hash_add(
            parent, edge->dst->name,
            queue_enq(queue_init(), queue_node(edge, sizeof(t_edge *), false)));
    else
        queue_enq(tmp, queue_node(edge, sizeof(t_edge *), false));
}

static void handle_edge(t_edge *edge, t_queue *open, t_queue *resids,
                        t_hash *parent) {
	ft_printf(" IN %s-%s ", edge->src->name, edge->dst->name);
    if (edge->residual->seen == g_turn || edge->dst->seen == g_turn){
		ft_printf(" IGNORE %s-%s\n", edge->src->name, edge->dst->name);
        return;
	}
    else if (edge->dst->seen == BELONG_TO_PATH) {
        ft_printf(" Residual %s-%s \n", edge->src->name, edge->dst->name);
		if (edge->residual->seen != BELONG_TO_PATH)
			queue_enq(resids, queue_node(edge, sizeof(t_edge *), false));
		else {
			enqueue_edges(open, edge->dst->edges);
			hash_add_parent(edge, parent);
			edge->seen = g_turn;
		}
        return;
    }
	ft_printf(" FRESH %s-%s\n", edge->src->name, edge->dst->name);
    enqueue_edges(open, edge->dst->edges);
    hash_add_parent(edge, parent);
	edge->dst->seen = g_turn;
    edge->seen = g_turn;
}

static void enqueue_residuals(t_queue *open, t_queue *resids, t_hash *parent) {
    t_edge *current;
    t_edge *edge;
    t_qnode *tmp;
	t_queue *tmp_queue;

	tmp_queue = queue_init();
    ft_putendl(">>> enqueue_residuals");
    while (queue_size(resids)) {
        tmp = queue_deq(resids), current = AS_EDGE(tmp);
        queue_node_del(&tmp, queue_blob_keep);
        current->seen = g_turn;
        hash_add_parent(current, parent);
        tmp = QFIRST(current->dst->edges);
        ft_printf(" || from >> ");
        edge_print(current);
        ft_printf(" /// ");
        edge_print(current->residual);
        ft_printf("\n");

        while (tmp != QTAIL(current->dst->edges)) {
            edge = tmp->blob;
            if (!ft_strcmp(edge->src->name, current->dst->name) &&
                edge->residual->seen == BELONG_TO_PATH) {
                ft_printf(" || to >> ");
                edge_print(edge);
                ft_printf(" /// ");
                edge_print(edge->residual);
                ft_printf("\n");

                edge->seen = g_turn;
                hash_add_parent(edge, parent);
                enqueue_edges(open, edge->dst->edges);
            }
            tmp = tmp->next;
        }
    }

	while (queue_size(tmp_queue))
		queue_enq(resids, queue_deq(tmp_queue));
}

static bool bfs_loop(t_graph *g, t_hash *parent) {
    t_queue *open;
    t_queue *resids;
    t_edge *edge;
    bool arrived;

    arrived = false;
    open = queue_init();
    resids = queue_init();
    enqueue_edges(open, g->source->edges);
    while (!arrived && queue_size(open)) {
        edge = extract_edge(open);
        if (edge->seen != g_turn && edge->seen != BELONG_TO_PATH)
            handle_edge(edge, open, resids, parent);
        if (edge->dst == g->sink && edge->seen != BELONG_TO_PATH)
            arrived = true;
        else if (!queue_size(open))
            enqueue_residuals(open, resids, parent);
    }
    queue_del(&resids, queue_blob_keep);
    queue_del(&open, queue_blob_keep);

	dump_hash_map(parent);
    return arrived;
}

static void queue_del_parent(void *node) {
    t_queue *q;

    q = node;
    queue_del(&q, queue_blob_keep);
}

t_queue *bfs_find(t_graph *g) {
    t_hash *parent;
    t_queue *path;

    parent = hash_init(g->n_vertices, queue_del_parent);
    g->source->seen = g_turn;
    path = bfs_loop(g, parent) ? construct_path(g, parent) : NULL;

    /* ft_printf("%d>> ", g_turn); */
    /* queue_iter(path, false, print_edge); */
    /* ft_putendl(""); */

    hash_free(parent);
    g_turn++;
    return path;
}
