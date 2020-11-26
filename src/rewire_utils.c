/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rewire_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: archid- <archid-@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/25 15:26:30 by archid-           #+#    #+#             */
/*   Updated: 2020/11/25 15:26:39 by archid-          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "netflow.h"

void sort_by_src_name(struct s_rewire_handy *info) {
	int i;
    int j;
    t_queue *q;
    t_qnode *e;

    if (!info->apath || !info->walk_edge || !info->n_paths)
        return;
    i = 0;
    while (i < info->n_paths - 1) {
        j = 0;
        while (j < info->n_paths - 1) {
            if (ft_strcmp(AS_EDGE(info->walk_edge[j])->src->name,
                          AS_EDGE(info->walk_edge[j + 1])->src->name) >
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

bool has_arrived(t_graph *g, t_edge *e) {
    if (!e)
        return false;
    return (g->source == e->src);
}

t_qnode *next_edge(t_graph *g, t_qnode *edge) {
    if (!g || !edge)
        return NULL;
    return has_arrived(g, QNODE_AS(struct s_edge, edge)) ? edge : edge->next;
}

void set_walk_edges(t_queue *paths, struct s_rewire_handy *info) {
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
