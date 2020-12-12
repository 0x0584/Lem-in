/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bfs.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: archid- <archid-@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/24 22:52:55 by archid-           #+#    #+#             */
/*   Updated: 2020/12/12 14:26:15 by archid-          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "graph.h"

typedef struct edge_pair {
    t_edge edge;
    t_edge from;
} t_edge_pair;

static t_mark g_mark = M_INITIAL;
static t_graph g_graph = NULL;

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

static void edge_mark(t_edge e, t_mark mark) {
    if (e->seen != M_BELONG_TO_PATH)
        e->seen = mark;
    if (e->src->seen != M_BELONG_TO_PATH)
        e->src->seen = mark;
    if (e->dst->seen != M_BELONG_TO_PATH)
        e->dst->seen = mark;
}

static bool edge_unseen_or_crossing_path(t_edge e) {
    return (e->seen != g_mark && e->dst->seen != g_mark) ||
           (e->seen != M_BELONG_TO_PATH && e->dst->seen == M_BELONG_TO_PATH);
}

static bool edge_fresh(t_edge e) {
    return ((e->seen != M_BELONG_TO_PATH && e->dst->seen != M_BELONG_TO_PATH) &&
            (e->seen != g_mark && e->dst->seen != g_mark));
}

static bool edge_crossing_path(t_edge e) {
    return (e->seen != M_BELONG_TO_PATH && e->seen != g_mark &&
            e->dst->seen == M_BELONG_TO_PATH);
}

static bool edge_path_residual(t_edge e) {
    return (e->residual->seen == M_BELONG_TO_PATH);
}

static bool enqueue_edges(t_edge e, t_lst open, t_hash parent,
                          bool (*check_edge)(t_edge e)) {
    t_lstnode walk;
    t_edge next;

    /* { ft_printf("\n%{white_bg}%{black_fg}edge enqueue:%{reset}\n"); } */
    walk = lst_front(e->dst->edges);
    while (walk) {
        next = walk->blob;
        if (next->dst != e->src && check_edge(next)) {
            /* { print_edge2(next); } */
            hash_add_edge(parent, next, e);
            if (next->dst == g_graph->sink)
                return true;
            lst_push_back_blob(open, next, sizeof(t_edge), false);
        }
        lst_node_forward(&walk);
    }
    /* { ft_putendl("========================================"); } */
    return false;
}

static bool handle_edge(t_edge e, t_lst open, t_hash parent) {
    if (edge_fresh(e)) {
        /* { ft_printf("%{green_fg}fresh%{reset}\n"); } */
        edge_mark(e, g_mark);
        return enqueue_edges(e, open, parent, edge_unseen_or_crossing_path);
    } else if (edge_crossing_path(e)) {
        /* { ft_printf("%{red_fg}crossing a path%{reset}\n"); } */
        edge_mark(e, g_mark);
        return enqueue_edges(e, open, parent,
                             edge_path_residual(e)
                                 ? edge_unseen_or_crossing_path
                                 : edge_path_residual);
    } else {
        /* { ft_printf("%{yellow_fg}ignored%{reset}\n"); } */
        return false;
    }
}

static bool bfs_setup(t_graph g, t_lst *open, t_hash *parent) {
    t_lstnode walk;

    if (!(g_graph = g))
        return false;
    {
        /* ft_putendl("\nbfs setup "); */
        /* ft_putendl("\n"); */
        /* print_graph(g); */
        /* ft_putendl("\n"); */
    }
    g_graph->source->seen = M_FRESH;
    g_graph->sink->seen = M_FRESH;
    *parent = hash_alloc(g->n_vertices, lst_free);
    *open = lst_alloc(blob_keep);
    walk = lst_front(g->source->edges);
    while (walk) {
        if (edge_fresh(walk->blob)) {
            /* { print_edge2(walk->blob); } */
            lst_push_back_blob(*open, walk->blob, sizeof(t_edge), false);
            hash_add_edge(*parent, walk->blob, NULL);
        }
        lst_node_forward(&walk);
    }
    return true;
}

static bool valid_pair(t_edge_pair *current, t_edge_pair *prev) {
    return !prev || !prev->from || current->edge == prev->from;
}

static t_lst backtrack_source(t_hash parent, const char *key,
                              t_edge_pair *prev) {
    t_lstnode walk;
    t_edge_pair *current;
    t_lst path;

    walk = lst_front(hash_get(parent, key, NULL));
    while (walk) {
        if (valid_pair(current = walk->blob, prev) &&
            (path = prev && !current->from
                        ? lst_alloc(blob_keep)
                        : backtrack_source(parent, current->from->dst->name,
                                           current))) {
            edge_mark(current->edge, M_BELONG_TO_PATH);
            return lst_push_back_blob(path, current->edge, sizeof(t_edge),
                                      false);
        }
        lst_node_forward(&walk);
    }
    return NULL;
}

static t_lst construct_path(t_hash parent) {
    return backtrack_source(parent, g_graph->sink->name, NULL);
}

t_lst bfs(t_graph g) {
    t_hash parent;
    t_lst path;
    t_lst open;
    t_edge e;
    bool sink_reached;

    if (!bfs_setup(g, &open, &parent))
        return NULL;
    sink_reached = false;
    while (!sink_reached && !lst_empty(open)) {
        e = lst_pop_front_blob(open);
        /* { */
        /*     ft_printf("\n%{bold}edge dequeue:%{reset} "); */
        /*     print_edge(e); */
        /* } */
        sink_reached = handle_edge(e, open, parent);
    }
    lst_del(&open);
    path = sink_reached ? construct_path(parent) : NULL;
    hash_del(&parent);
    g_mark++;
    return path;
}
