/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   netflow.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: archid- <archid-@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/23 19:06:16 by archid-           #+#    #+#             */
/*   Updated: 2020/12/09 00:03:31 by archid-          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lem_in.h"

static void flow_free(void *blob) {
    t_flow flow;
    size_t i;

    if (!(flow = blob))
        return;
    i = 0;
    while (i < flow->size)
        free(flow->stage[i++].vertex);
    free(flow);
}

static t_flow flow_alloc(t_lst path) {
    t_flow flow;
    t_lstnode walk;
    t_edge e;
    size_t i;

    flow = malloc(sizeof(struct s_flow));
    flow->total_units = 0;
    flow->size = lst_size(path);
    flow->stage = malloc(flow->size * sizeof(t_flow_pair));
    i = 0;
    walk = lst_front(path);
    while (walk) {
        e = walk->blob;
        flow->stage[i++].vertex = ft_strdup(e->dst->name);
        lst_node_forward(&walk);
    }
    return flow;
}

static t_network netflow_alloc(t_lst paths, size_t units) {
    t_network net;
    t_lstnode walk;

    net = malloc(sizeof(struct s_network));
    net->flows = lst_alloc(flow_free);
    walk = lst_front(paths);
    while (walk) {
        lst_push_back_blob(net->flows, flow_alloc(walk->blob), sizeof(t_flow),
                           false);
        lst_node_forward(&walk);
    }
    net->n_units = units;
    net->maxflow = 0;
    return net;
}

t_network netflow_setup(t_graph graph, size_t units) {
    t_network net;
    t_lst path;
    t_lst paths;

    paths = lst_alloc(lst_free);
    while ((path = bfs(graph)))
        correct_paths(lst_push_back_blob(paths, path, sizeof(t_lst), false));
    net = netflow_alloc(paths, units);
    lst_del(&paths);
    return net;
}

void netflow_pushflow(t_network net) {
    ;
    ;
}

void netflow_del(t_network *anet) {
    t_network net;

    if (!anet || !(net = *anet))
        return;
    lst_del(&net->flows);
    free(net);
    *anet = NULL;
}
