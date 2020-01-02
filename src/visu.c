/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   visu.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: melalj <melalj@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/11 10:38:01 by melalj            #+#    #+#             */
/*   Updated: 2020/01/02 11:51:03 by melalj           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../lem_in.h"
#ifdef USE_VISU
void	data_init(t_dvisu *data)
{
	data->w_width = 1280;
	data->w_height = 950;
	data->rend = NULL;
	data->window = NULL;
	data->s_surface = NULL;
	data->tex = NULL;
	data->path_n = 0;
}

int	init(t_dvisu *data)
{
	data_init(data);
	ft_printf("initing visu\n");
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
		return (0);
	}
	else
	{
		if (!(data->window = SDL_CreateWindow("SDL Tutorial",
			SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, data->w_width,
											data->w_height, SDL_WINDOW_SHOWN)))
		{
			ft_printf("Window could not be created! SDL_Error: %s\n",
															SDL_GetError());
			return (0);
		}
		if (!(data->rend = SDL_CreateRenderer(data->window, -1,
					SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED)))
		{
			ft_printf("error creating renderer! : %s\n", SDL_GetError());
			return (0);
		}
	}
	return (1);
}

void	visu_quit(t_dvisu data)
{
	SDL_DestroyRenderer(data.rend);
	SDL_DestroyWindow(data.window);
	SDL_Quit();
}

int	edge_draw(t_graph *g, t_edge *edge, int type)
{
	// int		*ranges_x;
	// int		*ranges_y;
	// t_cords	src;
	// t_cords	dst;
	static int path = 0;
	SDL_Rect	dstr;

	edge->v_c = (!(edge->v_c) || edge->v_c * type < 0)  ? type : edge->v_c;
	if ((edge->v_c) && edge->v_c * type < 0)
	{
		edge->v_c = 0;
		edge->residual->v_c = 0;
	}
	if (type > 0 && ft_strequ(edge->node_dst->name, g->sink->name))
		path++;
	// ft_printf("path %d --- %s -- %s\n", path, edge->node_dst->name, g->sink->name);
	g->data->path_n = path;
	dstr.w = 30;
	dstr.h = 30;
	// ft_printf("edge %s - %s to color\n", edge->node_src->name, edge->node_dst->name);
	nodes_draw(g, dstr);
	// SDL_SetRenderDrawColor(g->data->rend, 255, 150, 0, 255);
	// ranges_x = range_comp(0, g->max_c.x, 0, g->data->w_width - 100);
	// src.x = map(edge->node_src->cords.x, ranges_x) + 10;
	// ranges_y = range_comp(0, g->max_c.y, 0, g->data->w_height - 100);
	// src.y = map(edge->node_src->cords.y, ranges_y) + 10;
	// dst.x = map(edge->node_dst->cords.x, ranges_x) + 10;
	// dst.y = map(edge->node_dst->cords.y, ranges_y) + 10;
	// SDL_RenderDrawLine(g->data->rend, src.x, src.y, dst.x, dst.y);
	// free(ranges_x);
	// free(ranges_y);
	SDL_RenderPresent(g->data->rend);
	// SDL_SetRenderDrawColor(g->data->rend, 0, 0, 0, 255);
	return (1);
}

int	visu_init(t_graph *g)
{
	SDL_Rect	dstr;
	SDL_Event	event;
	int			close_requested;

	close_requested = 0;
	init(g->data);
	dstr.w = 20;
	dstr.h = 20;
	while (!close_requested)
	{
		SDL_RenderClear(g->data->rend);
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT
				|| (event.type == SDL_KEYDOWN && event.key.keysym.sym == 27))
				close_requested = 1;
		}
		graph_draw(g);
		SDL_RenderPresent(g->data->rend);
		SDL_PumpEvents();
	}
	return (0);
}
#endif
