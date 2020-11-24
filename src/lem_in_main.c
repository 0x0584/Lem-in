/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lem_in_main.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: archid- <archid-@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/18 11:57:54 by archid-           #+#    #+#             */
/*   Updated: 2020/11/24 23:39:47 by archid-          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "lem_in.h"

void vertex_dump(t_vertex *v) {
	ft_printf("name %s\n", v->name);
}

void graph_dump(t_graph *g) {
	ft_printf("n vertices: %zu\n", g->n_vertices);
	vertex_dump(g->source);
	vertex_dump(g->sink);
}

t_netflow *netflow_prepare(void) {
	t_netflow *net;
	t_graph *g;

	if (!(g = read_graph()))
		return NULL;
	graph_dump(g);
	net = netflow_setup(g, g_ants);
	graph_free(g);
	return net;
}

int		main(void)
{
	t_netflow *farm;

	g_error_line = ft_strdup("");
	if ((farm = netflow_prepare()))
	{
		netflow_pushflow(farm);
		netflow_del(&farm);
	}
	free(g_error_line);
	return (0);
}
