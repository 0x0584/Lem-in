/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lem_in_main.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: melalj <melalj@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/12 22:07:41 by melalj            #+#    #+#             */
/*   Updated: 2020/11/15 18:46:46 by archid-          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../lem_in.h"
# include "visu.h"

void	parser_free(t_parse *p)
{
	t_parse *walk;
	t_parse *hold;

	walk = p;
	while (walk)
	{
		hold = walk;
		walk = walk->next;
		free(hold->line);
		free(hold);
	}
}

int		main(void)
{
	t_parse		*pp;
	t_vertex		**vertices;
	int			vertices_c;
	int			i;
	t_graph		*g;
	t_vertex		**refs;
	size_t		n_ants;


	i = 0;
	pp = get_lines(&vertices_c);

	n_ants = (size_t)ft_atoi(pp->line);
	refs = (t_vertex **)malloc(sizeof(t_vertex *) * vertices_c);
	vertices = h_table(refs, pp, vertices_c);
	edges_fill(vertices, pp, vertices_c);

	parser_free(pp);

	g = graph_init(refs, vertices, vertices_c);

	t_netflow *farm = netflow_setup(g, n_ants);
	netflow_pushflow(farm);
	netflow_del(&farm);


	/* FIXME: fix double free  */
	// graph_free(g);

	return (0);
}
