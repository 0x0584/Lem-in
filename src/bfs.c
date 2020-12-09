/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bfs.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: archid- <archid-@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/24 22:52:55 by archid-           #+#    #+#             */
/*   Updated: 2020/12/09 01:50:47 by archid-          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "graph.h"

typedef struct edge_pair {
    t_edge edge;
    t_edge from;
} t_edge_pair;

static t_mark g_mark = INITIAL;

static void hash_add_edge(t_hash parent, t_edge edge, t_edge from) {
    t_lst found;
    t_lstnode pair;
    t_edge_pair current;

    current = (t_edge_pair){edge, from};
    pair = lst_node(&current, sizeof(t_edge_pair), true);
    if ((found = hash_get(parent, edge->dst->name, NULL)))
        lst_push_back(found, pair);
    else
        hash_add(parent, edge->dst->name,
                 lst_push_back(lst_alloc(blob_free), pair));
}

static t_edge edge_mark(t_edge e, t_mark mark) {
    e->seen = mark;
    if (e->src->seen != BELONG_TO_PATH)
        e->src->seen = mark;
    if (e->dst->seen != BELONG_TO_PATH)
        e->dst->seen = mark;
    return e;
}

static bool edge_unseen(t_edge e) { return e->seen != g_mark; }

static bool edge_fresh(t_edge e) {
    return e->seen != BELONG_TO_PATH && e->dst->seen != BELONG_TO_PATH &&
           e->seen != g_mark && e->dst->seen != g_mark;
}

static bool edge_crossing_path(t_edge e) {
    return e->dst->seen == BELONG_TO_PATH;
}

static bool edge_path_residual(t_edge e) {
    return e->residual->seen == BELONG_TO_PATH;
}

static void enqueue_edges(t_edge e, t_lst open, t_hash parent,
                          bool (*check_edge)(t_edge e)) {
    t_lstnode walk;

    walk = lst_front(e->dst->edges);
    while (walk) {
        if (check_edge(walk->blob)) {
            edge_mark(walk->blob, g_mark);
            hash_add_edge(parent, walk->blob, e);
            lst_push_back_blob(open, walk->blob, sizeof(t_edge), false);
        }
        lst_node_forward(&walk);
    }
}

static void handle_edge(t_edge e, t_lst open, t_hash parent) {
    edge_mark(e, g_mark);
    if (edge_fresh(e))
        enqueue_edges(e, open, parent, edge_fresh);
    else if (edge_crossing_path(e))
        enqueue_edges(e, open, parent,
                      edge_path_residual(e) ? edge_unseen : edge_path_residual);
}

static t_lst bfs_setup(t_graph g, t_hash parent) {
    t_lst open;
    t_lstnode walk;

    open = lst_alloc(blob_keep);
    walk = lst_front(g->source->edges);
    while (walk) {
        lst_push_back_blob(open, walk->blob, sizeof(t_edge), false);
        hash_add_edge(parent, walk->blob, NULL);
        lst_node_forward(&walk);
    }
    return open;
}

static void bfs_loop(t_graph g, t_hash parent) {
    t_lst open;
    t_edge e;

    open = bfs_setup(g, parent);
    while (!lst_empty(open))
        if (handle_edge(e = lst_pop_front_blob(open), open, parent),
            e->dst != g->sink)
            break;
    lst_del(&open);
}

static bool valid_pair(t_edge_pair *current, t_edge_pair *prev) {
    return !prev || !prev->from || current->edge == prev->from;
}

static t_lst backtrack_source(t_graph g, t_hash parent, t_edge_pair *prev) {
    t_lstnode walk;
    t_edge_pair *current;
    t_lst path;

    walk = lst_front(hash_get(parent, g->sink->name, NULL));
    while (walk) {
        if (valid_pair(current = walk->blob, prev) &&
            (path = prev && !prev->from
                        ? lst_alloc(blob_keep)
                        : backtrack_source(g, parent, current))) {
            edge_mark(current->edge, BELONG_TO_PATH);
            return lst_push_back_blob(path, current->edge, sizeof(t_edge),
                                      false);
        }
        lst_node_forward(&walk);
    }
    return NULL;
}

static t_lst construct_path(t_graph g, t_hash parent) {
    return backtrack_source(g, parent, NULL);
}

static void print_path(t_lst path) {
    t_lstnode walk;

    if (!path)
        return;
    ft_printf("length: %zu >> ", lst_size(path));
    walk = lst_front(path);
    while (walk) {
        print_edge(walk->blob);
        lst_node_forward(&walk);
    }
    ft_putendl("");
}

t_lst bfs(t_graph g) {
    t_hash parent;
    t_lst path;

    if (!g || !(parent = hash_alloc(g->n_vertices, lst_free)))
        return NULL;
    bfs_loop(g, parent);
    path = construct_path(g, parent);
    print_path(path);
    g_mark++;
    hash_del(&parent);
    return path;
}
