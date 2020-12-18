/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: archid- <archid-@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/18 11:57:54 by archid-           #+#    #+#             */
/*   Updated: 2020/12/18 19:40:04 by archid-          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lem_in.h"
#include "visu.h"

float	measure(clock_t t1, clock_t t2)
{
	return (1000 * (t2 - t1) / CLOCKS_PER_SEC);
}

void	measure_timing(const char *str, clock_t t1, clock_t t2)
{
	if (g_verbose)
		ft_dprintf(2, str, measure(t1, t2));
}

int		main(int argc, char *argv[])
{
	t_network	net;
	t_graph		g;
	clock_t		t1;
	clock_t		t2;

	parse_args(argc, argv);
	t1 = clock();
	if (!(g = read_graph()))
		return (-1);
	t2 = clock();
	measure_timing("graph is read: %{bold}%.2f%{reset} ms\n", t1, t2);
	t1 = clock();
	net = network_setup(g, g_ants);
	t2 = clock();
	measure_timing("network is read: %{bold}%.2f%{reset} ms\n", t1, t2);
	graph_del(&g);
	t1 = clock();
	network_pushflow(net);
	t2 = clock();
	measure_timing("flow is pushed: %{bold}%.2f%{reset} ms\n", t1, t2);
	network_del(&net);
	return (0);
}
