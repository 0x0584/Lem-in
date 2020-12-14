/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   netflow.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: archid- <archid-@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/23 19:06:16 by archid-           #+#    #+#             */
/*   Updated: 2020/12/14 17:49:44 by archid-          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lem_in.h"

static size_t last_flow;
static bool visualize = false;

/* static void print_flow_info(void *pflow) { */
/*     t_flow flow; */
/*     size_t i; */

/*     if (!(flow = pflow)) */
/*         return; */
/*     ft_printf("n edges: %zu ", flow->size); */
/*     ft_printf("total units: %zu ", flow->total_units); */
/*     i = 0; */
/*     while (i < flow->size) { */
/*         ft_printf(" | %{magenta_fg}[%d]%{italic}%5s%{reset} ", i, */
/*                   flow->stage[i].vertex); */
/*         i++; */
/*     } */
/*     ft_putendl(" |"); */
/* } */

/* static void flow_print(void *pflow) { */
/*     size_t i; */
/*     t_flow flow; */

/*     if (!(flow = pflow)) */
/*         return; */
/*     i = 0; */
/*     while (i < flow->size) { */
/*         if (flow->stage[i].unit != NIL_ANT) */
/*             ft_printf(" | %{magenta_fg}[%d]%{italic}%zu-%s%{reset} ", i, */
/*                       flow->stage[i].unit, flow->stage[i].vertex); */
/*         else */
/*             ft_printf(" | %{bold}*[%d]no ant*%{reset} ", i); */
/*         i++; */
/*     } */
/*     ft_putendl(" |"); */
/* } */

#define DEFAULT_COLOR "%{reset}"
#define BORDER_COLOR "%{bold}%{magenta_fg}"
#define EMPTY_CELL_COLOR "%{blue_fg}"
#define UNIT_CELL_COLOR "%{yellow_fg}"

#define TOP_BAR BORDER_COLOR "-------" DEFAULT_COLOR
#define CORNER BORDER_COLOR "+" DEFAULT_COLOR
#define SIDE_BAR BORDER_COLOR " | " DEFAULT_COLOR
#define EMPTY_CELL EMPTY_CELL_COLOR " /// " DEFAULT_COLOR
#define UNIT_CELL SIDE_BAR UNIT_CELL_COLOR "%-4zu " DEFAULT_COLOR

static void print_top_bar(size_t size) {
    size_t i;

    i = 0;
    ft_putstr("  ");
    while (i < size)
        ft_printf(CORNER TOP_BAR), i++;
    ft_printf(CORNER "\n ");
}

static void flow_ascii(void *pflow) {
    size_t i;
    t_flow flow;

    if (!(flow = pflow))
        return;
    print_top_bar(flow->size);
    i = 0;
    while (i < flow->size) {
        if (flow->stage[i].unit != NIL_ANT)
            ft_printf(UNIT_CELL, flow->stage[i].unit);
        else
            ft_printf(SIDE_BAR EMPTY_CELL);
        i++;
    }
    ft_printf(SIDE_BAR "\n");
    last_flow = flow->size;
}

static void print_flows_ascii(t_lst flows) {
    ft_putendl("");
    lst_iter(flows, true, flow_ascii);
    print_top_bar(last_flow);
}

static void network_print(t_network net, size_t maxflow) {
    system("clear");

    /* ft_putendl(""); */
    /* ft_printf("n_flows: %zu\n", lst_size(net->flows)); */
    /* ft_printf("n_units: %zu\n", net->n_units); */
    /* lst_iter(net->flows, true, print_flow_info); */

    /* ft_putendl("\n"); */
    /* lst_iter(net->flows, true, flow_print); */

    ft_printf("\n  %{underline}%{italic}maxflow is%{reset}%{underline}");
    if (maxflow == lst_size(net->flows))
        ft_printf(" %{green_fg}%zu%{reset}", maxflow);
    else if (maxflow > lst_size(net->flows) / 2)
        ft_printf(" %{yellow_fg}%zu%{reset}", maxflow);
    else
        ft_printf(" %{red_fg}%zu%{reset}", maxflow);
    print_flows_ascii(net->flows);
    system("sleep 1");
}

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

static size_t netflow_simulate(t_network net) {
    t_lstnode walk;
    size_t instructions;
    size_t unit;
    bool flag;

    netflow_regulate(net);
    unit = 0;
    flag = true;
    instructions = 0;
    while (flag) {
        flag = false;
        walk = lst_front(net->flows);
        while (walk && unit < net->n_units) {
            if (flow_push(walk->blob, unit))
                unit++;
            lst_node_forward(&walk);
        }
        walk = lst_front(net->flows);
        while (walk) {
            flag = flow_sync(walk->blob) || flag;
            lst_node_forward(&walk);
        }
        instructions += flag;
    }
    return instructions;
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
        if ((result = netflow_simulate(net)) >= prev) {
            lst_node_free(flows, lst_extract(flows, lst_rear(flows)));
            break;
        }
        prev = result;
    }
    {
        /* ft_printf("\n%{green_fg}final paths%{reset}\n"); */
        /* lst_iter(paths, true, print_path); */
        assert_paths_correct(graph, paths);
    }
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

static void flow_out(t_flow flow) {
    size_t i;

    i = flow->size;
    while (i--)
        if (flow->stage[i].unit != NIL_ANT)
            ft_printf("L%zu-%s ", flow->stage[i].unit, flow->stage[i].vertex);
}

void netflow_pushflow(t_network net) {
    t_lstnode walk;
    size_t maxflow;
    size_t unit;
    bool flag;

    netflow_regulate(net);
    unit = 1;
    flag = true;
    if (visualize) {
        network_print(net, 0);
        ft_putendl("");
    }
    while (flag) {
        maxflow = 0;
        flag = false;
        walk = lst_front(net->flows);
        while (walk && unit <= net->n_units) {
            if (flow_push(walk->blob, unit))
                unit++, maxflow++;
            lst_node_forward(&walk);
        }
        walk = lst_front(net->flows);
        while (walk) {
            flow_out(walk->blob);
            lst_node_forward(&walk);
        }
        if (visualize)
            network_print(net, maxflow);
        walk = lst_front(net->flows);
        while (walk) {
            flag = flow_sync(walk->blob) || flag;
            lst_node_forward(&walk);
        }
        ft_putstr(flag ? "\n" : "");
    }
    if (visualize) {
        network_print(net, maxflow);
        system("tail -f /dev/null");
    }
    ft_putendl("");
}
