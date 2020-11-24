/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lem_in_main.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: archid- <archid-@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/18 11:57:54 by archid-           #+#    #+#             */
/*   Updated: 2020/11/23 19:25:12 by archid-          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "lem_in.h"

t_netflow *netflow_prepare(void) {
	t_netflow *net;
	t_graph *g;

	if (!(g = read_graph())) {
		graph_free(g);
		return NULL;
	}
	net = netflow_setup(g, g_ants);
	graph_free(g);
	return net;
}

int		main(void)
{
	t_netflow *farm;

	g_error_line = ft_strdup("");
	farm = netflow_prepare();
	netflow_pushflow(farm);
	netflow_del(&farm);
	free(g_error_line);
	return (0);
}
