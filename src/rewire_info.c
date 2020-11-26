/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rewire_info.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: archid- <archid-@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/26 23:46:44 by archid-           #+#    #+#             */
/*   Updated: 2020/11/26 23:46:46 by archid-          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "netflow.h"

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
        if (AS_EDGE(info->walk_edge[info->curr])->src != g->source)
            info->walk_edge[info->curr] = info->walk_edge[info->curr]->next;
        info->curr++;
    }
}


bool rewire_done(t_graph *g, struct s_rewire_handy *info) {
	bool done;

	info->curr = 0;
	done = true;
	while (info->curr < info->n_paths && done)
		if (AS_EDGE(info->walk_edge[info->curr++])->src != g->source)
			done = false;
	return done;
}
