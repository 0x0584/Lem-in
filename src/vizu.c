/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vizu.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: archid- <archid-@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/18 14:02:24 by archid-           #+#    #+#             */
/*   Updated: 2020/12/18 18:30:29 by archid-          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "visu.h"
#include "parser.h"

size_t			g_last_flow;

static void		show_maxflow(t_network net, size_t maxflow)
{
	if (g_output_maxflow)
	{
		ft_printf("\n  %{underline}%{italic}maxflow is%{reset}%{underline}");
		if (maxflow == lst_size(net->flows))
			ft_printf(" %{green_fg}%zu%{reset}\n", maxflow);
		else if (maxflow >= lst_size(net->flows) / 2)
			ft_printf(" %{yellow_fg}%zu%{reset}\n", maxflow);
		else
			ft_printf(" %{red_fg}%zu%{reset}\n", maxflow);
	}
}

static void		print_flow_lines(t_network net)
{
	t_lstnode walk;

	walk = lst_front(net->flows);
	while (walk)
	{
		flow_out(walk->blob);
		lst_node_forward(&walk);
	}
	if (!g_visualize)
		ft_putendl("");
}

void			show(t_network net, size_t maxflow)
{
	if (g_visualize)
		system("clear");
	show_maxflow(net, maxflow);
	if (g_output_info)
	{
		ft_putendl("");
		lst_iter(net->flows, true, flow_print);
		ft_putendl("");
	}
	if (g_output_ant_lines || !g_visualize)
	{
		if (g_visualize)
			ft_putstr("  ");
		print_flow_lines(net);
	}
	if (g_visualize)
	{
		ft_putendl("");
		print_flows_ascii(net->flows);
		system("sleep 1");
	}
}
