/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   netflow.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: archid- <archid-@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/23 19:06:16 by archid-           #+#    #+#             */
/*   Updated: 2020/12/13 03:03:06 by archid-          ###   ########.fr       */
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
    free(flow->stage);
    free(flow);
}

static t_flow flow_alloc(t_lst path) {
    t_flow flow;
    t_lstnode walk;
    t_edge e;
    size_t i;

    flow = malloc(sizeof(struct s_flow));
    flow->total_units = NIL_ANT;
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
    return net;
}

t_network netflow_setup(t_graph graph, size_t units) {
    t_network net;
    t_lst path;
    t_lst paths;

    paths = lst_alloc(lst_free);
    while ((path = bfs(graph)))
        correct_paths(lst_push_back_blob(paths, path, sizeof(t_lst), false));
    ft_printf("\n%{green_fg}final paths%{reset}\n");
    lst_iter(lst_insertion_sort(paths, shortest_path), true, print_path);
    assert_paths_correct(graph, paths);
    net = netflow_alloc(paths, units);
    lst_del(&paths);
    return net;
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
    flow_push(flow, NIL_ANT);
    return flag;
}

static void flow_out(t_flow flow) {
    size_t i;

    i = flow->size;
    while (i--)
        if (flow->stage[i].unit != NIL_ANT) {
            ft_printf("L%zu-%s ", flow->stage[i].unit, flow->stage[i].vertex);
        }
}

static size_t netflow_simulate(t_network net, size_t n_paths) {
    t_lst paths;
    t_lstnode walk;
    size_t instructions;
    size_t unit;
    bool flag;

    unit = 0;
    instructions = 0;
    paths = lst_shrink(lst_copy_shallow(net->flows), false, n_paths);
    flag = true;
    while (flag) {
        flag = false;
        walk = lst_front(paths);
        while (walk && unit < net->n_units) {
            flow_push(walk->blob, ++unit);
            lst_node_forward(&walk);
        }
        walk = lst_front(paths);
        while (walk) {
            flag = flow_sync(walk->blob) || flag;
            lst_node_forward(&walk);
        }
        instructions++;
    }
    lst_del(&paths);
    return instructions;
}

static void flow_print(void *pflow) {
    size_t i;
    t_flow flow;

    if (!(flow = pflow))
        return;
    ft_printf("\nsize: %zu\n", flow->total_units);
    ft_printf("n edges: %zu\n", flow->size);
    i = 0;
    while (i < flow->size) {
        if (flow->stage[i].unit != NIL_ANT)
            ft_printf(" | %{magenta_fg}[%d]%{italic}%zu-%s%{reset} ", i,
                      flow->stage[i].unit, flow->stage[i].vertex);
        else
            ft_printf(" | %{bold}*[%d]no ant*%{reset} ", i);
        i++;
    }
    ft_putendl(" |");
}

static void netflow_regulate(t_network net, size_t n_paths) {
    t_lstnode walk;
    t_flow flow_a;
    t_flow flow_b;
    size_t sum;
    size_t div;
    size_t mod;

    sum = 0;
    lst_shrink(net->flows, false, n_paths);
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
        lst_node_forward(&walk);
    }
    walk = lst_front(net->flows);
    while (mod-- && walk) {
        flow_a = walk->blob;
        flow_a->total_units++;
        lst_node_forward(&walk);
    }

    {
        lst_iter(net->flows, true, flow_print);
        ft_putendl("\n");
    }
}

static void netflow_prepare(t_network net) {
    size_t result;
    size_t n_paths;
    size_t prev;
    t_lstnode walk;

    n_paths = 1;
    prev = NIL_ANT;
    walk = lst_front(net->flows);
    while (walk) {
        if ((result = netflow_simulate(net, n_paths)) >= prev)
            break;
        prev = result;
        if (lst_node_forward(&walk))
            n_paths++;
    }
    netflow_regulate(net, n_paths);
}

static size_t last_flow;

static void flow_ascii(void *pflow) {
    size_t i;
    size_t j;
    t_flow flow;

    if (!(flow = pflow))
        return;
    i = 0;
    ft_putstr("  ");
    while (i < flow->size) {
        ft_printf("%{bold}+-------%{reset}"), i++;
    }
    ft_printf("%{bold}+%{reset}\n ");

    i = 0;
    while (i < flow->size)
        if (flow->stage[i++].unit != NIL_ANT)
            ft_printf(" %{bold}|%{reset}   %{yellow_fg}*%{reset}  ");
        else
            ft_printf(" %{bold}|%{reset}   %{bold}.%{reset}  ");
    ft_printf(" %{bold}|%{reset}\n");
    last_flow = flow->size;
}

static void network_print(t_network net, size_t maxflow) {
    t_lstnode walk;
    size_t i;

    ft_printf("n_flows: %zu ", lst_size(net->flows));
    ft_printf("%n_units: %zu\n", net->n_units);

    ft_putendl("\n");
    lst_iter(net->flows, true, flow_print);

    ft_printf("\n  %{underline}%{italic}maxflow is%{reset}%{underline}");
    if (maxflow == lst_size(net->flows))
        ft_printf(" %{green_fg}%zu%{reset}", maxflow);
    else if (maxflow > lst_size(net->flows) / 2)
        ft_printf(" %{yellow_fg}%zu%{reset}", maxflow);
    else
        ft_printf(" %{red_fg}%zu%{reset}", maxflow);
    ft_putendl("");
    lst_iter(net->flows, true, flow_ascii);

    i = 0;
    ft_putstr("  ");
    while (i < last_flow) {
        ft_printf("%{bold}+-------%{reset}"), i++;
    }
    ft_printf("%{bold}+%{reset}\n");
}

void netflow_pushflow(t_network net) {
    t_lstnode walk;
    size_t maxflow;
    size_t unit;
    bool flag;

    netflow_prepare(net);
    walk = lst_front(net->flows);
    unit = 0, flag = true;
    while (flag) {
        maxflow = 0;
        flag = false;
        walk = lst_front(net->flows);
        while (walk && unit < net->n_units) {
            if (flow_push(walk->blob, unit))
                unit++, maxflow++;
            lst_node_forward(&walk);
        }
        walk = lst_front(net->flows);
        while (walk) {
            /* flow_out(walk->blob); */
            lst_node_forward(&walk);
        }
        network_print(net, maxflow);
        walk = lst_front(net->flows);
        while (walk) {
            flag = flow_sync(walk->blob) || flag;
            lst_node_forward(&walk);
        }
        ft_putstr(flag ? "\n" : "");
    }
    network_print(net, maxflow);
}

void netflow_del(t_network *anet) {
    t_network net;

    if (!anet || !(net = *anet))
        return;
    lst_del(&net->flows);
    free(net);
    *anet = NULL;
}
