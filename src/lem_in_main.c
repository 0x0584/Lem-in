/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lem_in_main.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: archid- <archid-@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/18 11:57:54 by archid-           #+#    #+#             */
/*   Updated: 2020/11/25 03:59:47 by archid-          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "lem_in.h"

void vertex_dump(t_vertex *v) {
	ft_printf("name %s\n", v->name);
}

int		main(void)
{
	t_netflow *net;
	t_graph *g;

	g_error_line = NULL;
	if (!(g = read_graph()))
		return 0;
	net = netflow_setup(g, g_ants);
	graph_free(g);
	netflow_pushflow(net);
	netflow_del(&net);
	return (0);
}
