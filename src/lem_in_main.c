/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lem_in_main.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: melalj <melalj@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/12 22:07:41 by melalj            #+#    #+#             */
/*   Updated: 2020/01/09 05:24:49 by archid-          ###   ########.fr       */
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
	t_node		**nodes;
	int			nodes_c;
	t_graph		*g;
	size_t		n_ants;

	pp = get_lines(&nodes_c);

	n_ants = (size_t)ft_atoi(pp->line);
#ifdef DEBUG
	ft_printf("%lu\n", n_ants);
#endif
	nodes = h_table(pp, nodes_c);
	edges_fill(nodes, pp, nodes_c);

	parser_free(pp);

#ifdef USE_VISU
	set_visu_data((t_dvisu *)ft_memalloc(sizeof(t_dvisu)));
#endif
	g = graph_init(nodes, nodes_c);

#ifdef USE_VISU
	visu_init(g);
#endif

#ifdef DEBUG
	graph_dump(g);
	ft_printf("source: %s | sink: %s\n", g->start->name, g->sink->name);
#endif

	t_netflow *farm = netflow_setup(g, n_ants);

#ifdef USE_VISU
	SDL_Event event;
	int close_requested = 0;
	while (!close_requested)
	{
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT
					|| (event.type == SDL_KEYDOWN
						&& event.key.keysym.sym == 27))
				close_requested = 1;
		}
	}
#endif


	netflow_pushflow(farm);
	netflow_del(&farm);

	/* FIXME: fix double free  */
	// graph_free(g);

	return (0);
}
