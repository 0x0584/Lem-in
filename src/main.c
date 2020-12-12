/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: archid- <archid-@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/18 11:57:54 by archid-           #+#    #+#             */
/*   Updated: 2020/12/10 22:54:16 by archid-          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "lem_in.h"

float measure(clock_t t1, clock_t t2) {
    return 1000 * (t2 - t1) / CLOCKS_PER_SEC;
}

int main(void) {
    t_network net;
    t_graph g;
    clock_t t1, t2;

    t1 = clock();
    if (!(g = read_graph()))
        return -1;
    t2 = clock();
    ft_dprintf(2, "graph is read in %{bold}%.2f%{reset} ms\n", measure(t1, t2));
    t1 = clock();
    net = netflow_setup(g, g_ants);
    t2 = clock();
    ft_dprintf(2, "network is ready in %{bold}%.2f%{reset} ms\n", measure(t1, t2));
    graph_del(&g);
    t1 = clock();
    netflow_pushflow(net);
    t2 = clock();
    ft_dprintf(2, "flow is pushed in %{bold}%.2f%{reset} ms\n", measure(t1, t2));
    netflow_del(&net);
    return (0);
}