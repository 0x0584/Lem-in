/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   netflow.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: archid- <archid-@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/23 19:06:16 by archid-           #+#    #+#             */
/*   Updated: 2020/12/15 20:29:53 by archid-          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "visu.h"

static int fd;

static bool json_output = true;

static void flow_free(void *blob) {
    t_flow flow;
    size_t i;

    if (!(flow = blob))
        return;
    i = 0;
    while (i < flow->size)
        free(flow->stage[i++].vertex);
    free(flow->stage);
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
        flow->stage[i].vertex = ft_strdup(e->dst->name);
        flow->stage[i].unit = 0;
        i++, lst_node_forward(&walk);
    }
    return flow;
}

static bool flow_push(t_flow flow, size_t unit) {
    if (!flow->total_units)
        return false;
    flow->stage->unit = unit;
    flow->total_units--;
    return true;
}

static bool flow_sync(t_flow flow) {
    size_t i;
    bool flag;

    flag = false;
    i = flow->size - 1;
    while (i) {
        flag = (flow->stage[i - 1].unit != NIL_ANT) || flag;
        flow->stage[i].unit = flow->stage[i - 1].unit;
        i--;
    }
    flow->stage->unit = NIL_ANT;
    return flag;
}

static void path_to_flow(void *path, void *flows) {
    lst_push_back_blob(flows, flow_alloc(path), sizeof(t_flow), false);
}

static void netflow_regulate(t_network net) {
    t_lstnode walk;
    t_flow flow_a;
    t_flow flow_b;
    size_t sum;
    size_t div;
    size_t mod;

    sum = 0;
    walk = lst_front(net->flows);
    flow_a = lst_rear_blob(net->flows);
    while (walk) {
        flow_b = walk->blob;
        flow_b->total_units = flow_a->size - flow_b->size;
        sum += flow_b->total_units;
        lst_node_forward(&walk);
    }
    div = (net->n_units - sum) / lst_size(net->flows);
    mod = (net->n_units - sum) % lst_size(net->flows);
    walk = lst_front(net->flows);
    while (walk) {
        flow_a = walk->blob;
        flow_a->total_units += div;
        if (mod)
            flow_a->total_units++, mod--;
        lst_node_forward(&walk);
    }

    {
        /* lst_iter(net->flows, true, print_flow_info); */
        /* ft_putendl("\n"); */
    }
}

static size_t netflow_push(t_network net, size_t *unit) {
    t_lstnode walk;
    size_t maxflow;

    walk = lst_front(net->flows);
    maxflow = 0;
    while (walk && *unit <= net->n_units) {
        if (flow_push(walk->blob, *unit))
            *unit += 1, maxflow++;
        lst_node_forward(&walk);
    }
    return maxflow;
}

static bool netflow_sync(t_network net) {
    t_lstnode walk;
    bool flag;

    flag = false;
    walk = lst_front(net->flows);
    while (walk) {
        flag = flow_sync(walk->blob) || flag;
        lst_node_forward(&walk);
    }
    return flag;
}

static size_t netflow_simulate(t_network net, bool visualize) {
    size_t instructions;
    size_t unit;
    bool flag;
    size_t maxflow;

    netflow_regulate(net);
    unit = 1;
    flag = true;
    instructions = 0;
    if (visualize)
        show(net, 0);
    while (flag) {
        maxflow = netflow_push(net, &unit);
        if (visualize)
            show(net, maxflow);
        if ((flag = netflow_sync(net)))
            instructions++;
    }
    if (visualize)
        show(net, maxflow);
    return instructions;
}

static void vertex_as_JSON(const char *str, t_vertex v) {
    ft_dprintf(fd, "\"%s\": {\"name\":\"%s\", \"x\": %d, \"y\": %d}", str,
               v->name, v->x, v->y);
}

static void vertex_with_edges_as_JSON(const char *key, void *blob) {
    t_vertex v;
    t_lstnode walk;
    t_edge e;

    v = blob;
    ft_dprintf(fd, ",\n");
    ft_dprintf(fd, "\"%s\": {", key);
    ft_dprintf(fd, "\"x\": %d, \"y\": %d, \"level\": %d,", v->x, v->y,
               v->level);
    ft_dprintf(fd, "\"edges\": [");
    walk = lst_front(v->edges);
    while (walk) {
        e = walk->blob;
        ft_dprintf(fd, "\"%s\"", e->dst->name);
        if (lst_node_forward(&walk))
            ft_dprintf(fd, ",");
    }
    ft_dprintf(fd, "]}");
}

static void vertices_as_JSON(t_hash verts) {

    ft_dprintf(fd, "\"vertices\": {");
    ft_dprintf(fd, "\"count\": \"%zu\"", hash_count(verts));
    hash_iter(verts, vertex_with_edges_as_JSON);
    ft_dprintf(fd, "}");
}

static void path_as_JSON(void *path, size_t index) {
    t_lstnode walk;
    t_edge e;

    walk = lst_front(path);
    e = walk->blob;
    ft_dprintf(fd, ",\n \"%zu\": [\"%s\"", index, e->src->name);
    while (walk) {
        e = walk->blob;
        ft_dprintf(fd, ", \"%s\"", e->dst->name);
        lst_node_forward(&walk);
    }
    ft_dprintf(fd, "]");
}

static void paths_as_JSON(t_lst paths) {
    ft_dprintf(fd, "\"paths\": {");
    ft_dprintf(fd, "\"size\": %zu", lst_size(paths));
    lst_iteri(paths, true, path_as_JSON);
    ft_dprintf(fd, "}");
}

static void save_JSON(t_graph g, t_lst paths) {
    fd = open("out.json", O_CREAT | O_APPEND | O_RDWR, 0666);
    ftruncate(fd, 0);

    level_graph(g);

    ft_dprintf(fd, "{");

    vertex_as_JSON("start", g->source);
    ft_dprintf(fd, ",\n");

    vertex_as_JSON("end", g->sink);
    ft_dprintf(fd, ",\n");

    vertices_as_JSON(g->vertices);
    ft_dprintf(fd, ",\n");

    paths_as_JSON(paths);

    ft_dprintf(fd, "}\n");
    close(fd);
}

static void netflow_prepare(t_graph graph, t_network net) {
    t_lst path;
    t_lst paths;
    t_lst flows;
    size_t result;
    size_t prev;

    prev = NIL_ANT;
    flows = net->flows;
    paths = lst_alloc(lst_free);
    while ((path = bfs(graph))) {
        correct_paths(lst_push_back_blob(paths, path, sizeof(t_lst), false));
        lst_iter_arg(lst_insertion_sort(paths, shortest_path), true,
                     lst_clear(flows), path_to_flow);
        if ((result = netflow_simulate(net, false)) >= prev) {
            lst_node_free(flows, lst_extract(flows, lst_rear(flows)));
            lst_node_free(paths, lst_extract(paths, lst_rear(paths)));
            break;
        }
        prev = result;
    }
    {
        /* ft_printf("\n%{green_fg}final paths%{reset}\n"); */
        /* lst_iter(paths, true, print_path); */
        assert_paths_correct(graph, paths);
    }
    if (json_output)
        save_JSON(graph, paths);
    lst_del(&paths);
}

t_network netflow_setup(t_graph graph, size_t units) {
    t_network net;

    net = malloc(sizeof(struct s_network));
    net->flows = lst_alloc(flow_free);
    net->n_units = units;
    netflow_prepare(graph, net);
    return net;
}

void netflow_del(t_network *anet) {
    t_network net;

    if (!anet || !(net = *anet))
        return;
    lst_del(&net->flows);
    free(net);
    *anet = NULL;
}

void netflow_pushflow(t_network net) { netflow_simulate(net, true); }
