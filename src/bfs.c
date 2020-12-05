/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bfs.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: archid- <archid-@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/24 22:52:55 by archid-           #+#    #+#             */
/*   Updated: 2020/12/05 01:58:25 by archid-          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "netflow.h"

/*
  2 or above are edges that we have found throught bfs
  1 for those who belong into a path
*/

enum e_state g_turn = INITIAL;

typedef struct edge_pair {
	t_edge *edge;
	t_edge *parent;
} t_edge_pair;

void edge_print(t_edge *edge) {
	if (edge)
		ft_printf("[%s(%d)-%s(%d)|%d] ", edge->src->name, edge->src->seen,
				  edge->dst->name, edge->dst->seen, edge->seen);
}

void edge_pair_print(t_edge_pair *pair) {
	if (pair) {
		ft_putstr(" parent: ");
		edge_print(pair->parent);
		ft_putstr(" edge: ");
		edge_print(pair->edge);
		ft_putstr("\n");
	}
}

void node_dump(t_qnode *node) { edge_print(node->blob); }

void node_pair_dump(t_qnode *node) { edge_pair_print(node->blob); }

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
				queue_iter(tmp->blob, true, node_pair_dump);
				ft_putendl("\n-----------\n");
				walk = walk->next;
			}
		}
		i++;
	}
}

bool edge_state(t_edge_pair *edge_pair, t_edge_pair *backwards_pair) {
    ft_printf("Edge: ");
    edge_pair_print(edge_pair);
    ft_printf(" ");

    if (!backwards_pair) {
        ft_printf("from sink\n");
        return true;
    }

    ft_printf("Previous: ");
    edge_pair_print(backwards_pair);
    ft_putstr(" =>>> \n ");

	return edge_pair->edge == backwards_pair->parent;
}

static t_queue *traverse_hash(t_graph *g, char *key, t_hash *parent,
                              t_edge_pair *backwards_pair) {
    t_queue *path;
    t_queue *prev;
    t_qnode *walk;
    t_edge_pair *edge_pair;

    if (!(prev = hash_get(parent, key, NULL)))
        return NULL;
    walk = QFIRST(prev);
    while (walk != QTAIL(prev)) {
        edge_pair = walk->blob;

        bool state = edge_state(edge_pair, backwards_pair);

        /* ft_printf("state: %d\n", state); */

        if (state && (path = (edge_pair->edge->src == g->source
                                  ? queue_init()
                                  : traverse_hash(g, edge_pair->edge->src->name,
                                                  parent, edge_pair)))) {
            edge_pair->edge->src->seen = BELONG_TO_PATH;
            edge_pair->edge->seen = BELONG_TO_PATH;
            return queue_push_front(
                path, queue_node(edge_pair->edge, sizeof(t_edge *), false));
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


static void hash_add_parent(t_edge *edge, t_edge *parent_edge, t_hash *parent) {
    t_queue *tmp;
	t_edge_pair pair;

	pair = (t_edge_pair){edge, parent_edge};
    if (!(tmp = hash_get(parent, edge->dst->name, NULL)))
        hash_add(
            parent, edge->dst->name,
            queue_enq(queue_init(), queue_node(&pair, sizeof(t_edge_pair), true)));
	 else
        queue_enq(tmp, queue_node(&pair, sizeof(t_edge_pair), true));
}

static void enqueue_edges(t_queue *q, t_edge *in_edge, t_hash *parent) {
    t_qnode *walk;
	t_edge *edge;
	t_queue *other = in_edge->dst->edges;

    walk = QFIRST(other);
    while (walk != QTAIL(other)) {
		edge = walk->blob;
		ft_printf(" enqueuing: %s-%s\n", edge->src->name, edge->dst->name);
        queue_enq(q, queue_node(walk->blob, sizeof(t_edge *), false));
		hash_add_parent(edge, in_edge, parent);
        walk = walk->next;
    }
}

static void enqueue_source_edges(t_queue *q, t_queue *other) {
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

static void handle_edge(t_edge *edge, t_queue *open, t_queue *resids,
                        t_hash *parent) {
	ft_printf(" IN %s-%s ", edge->src->name, edge->dst->name);
    if (edge->residual->seen == g_turn || edge->dst->seen == g_turn){
		ft_printf(" IGNORE %s-%s\n", edge->src->name, edge->dst->name);
        return;
	}
    else if (edge->dst->seen == BELONG_TO_PATH) {
        ft_printf(" Residual %s-%s \n", edge->src->name, edge->dst->name);
		/* if (edge->residual->seen != BELONG_TO_PATH) */
			queue_enq(resids, queue_node(edge, sizeof(t_edge *), false));
		/* else { */
		/* 	enqueue_edges(open, edge, parent); */
		/* 	/\* hash_add_parent(edge, parent); *\/ */
		/* 	edge->seen = g_turn; */
		/* } */
        return;
    }
	ft_printf(" FRESH %s-%s\n", edge->src->name, edge->dst->name);
    enqueue_edges(open, edge, parent);
    /* hash_add_parent(edge, parent); */
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
        tmp = QFIRST(current->dst->edges);
        ft_printf(" || from >> ");
        edge_print(current);
        ft_printf(" /// ");
        edge_print(current->residual);
        ft_printf("\n");

        /* enqueue_edges(open, current, parent); */
        while (tmp != QTAIL(current->dst->edges)) {
            edge = tmp->blob;
            /* handle_edge(edge, open, resids, parent); */
            if (edge->src == current->dst) {
                if (edge->residual->seen == BELONG_TO_PATH) {
                    ft_printf(" || to >> ");
                    edge_print(edge);
                    ft_printf(" /// ");
                    edge_print(edge->residual);
                    ft_printf("\n");

                    edge->seen = g_turn;
                    /* hash_add_parent(edge, parent); */
                    hash_add_parent(edge, current, parent);
                    enqueue_edges(open, edge, parent);
                } else if (current->residual->seen == BELONG_TO_PATH) {
                    queue_enq(tmp_queue,
                              queue_node(edge, sizeof(t_edge *), false));
                }
            }
            tmp = tmp->next;
        }
    }

    while (queue_size(tmp_queue))
        queue_enq(resids, queue_deq(tmp_queue));
    queue_del(&tmp_queue, queue_blob_keep);
}

static bool bfs_loop(t_graph *g, t_hash *parent) {
    t_queue *open;
    t_queue *resids;
    t_edge *edge;
    bool arrived;

    arrived = false;
    open = queue_init();
    resids = queue_init();
    enqueue_source_edges(open, g->source->edges);
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
    queue_del(&q, queue_blob_free);
}

t_queue *bfs_find(t_graph *g) {
    t_hash *parent;
    t_queue *path;

    parent = hash_init(g->n_vertices, queue_del_parent);
    g->source->seen = g_turn;
    path = bfs_loop(g, parent) ? construct_path(g, parent) : NULL;

    ft_printf("%d>> ", g_turn);
    queue_iter(path, false, print_edge);
    ft_putendl("");

    hash_free(parent);
    g_turn++;
    return path;
}
