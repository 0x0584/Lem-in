/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   visu.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: melalj <melalj@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/11 10:38:01 by melalj            #+#    #+#             */
/*   Updated: 2020/01/05 09:51:22 by melalj           ###   ########.fr       */
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
	data->f = 0;
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
	SDL_Rect	dstr;

	// edge->v_c = (!(edge->v_c) || edge->v_c * type < 0)  ? type : edge->v_c;
	g->data->f = type;
	if (type == -1 && (edge->path_n == -1 && edge->residual->path_n == -1))
	{
		edge->residual->color.hex = 0x031cfc;
		edge->color.hex = 0x031cfc;
		edge->v_c = type;
		edge->residual->v_c = type;
	}
	if (type == 1)
	{
		edge->residual->path_n = g->data->path_n;
		edge->path_n = g->data->path_n;
		edge->color.hex = 0x032050 + 50000 * edge->path_n;
		edge->residual->color.hex = 0x032050 + 50000 * edge->path_n;
		edge->v_c = type;
		edge->residual->v_c = type;
	}
	// if ((edge->v_c) && edge->v_c * type < 0)
	// {
	// 	edge->v_c = 0;
	// 	edge->residual->v_c = 0;
	// }
	// if (type > 0 && ft_strequ(edge->node_dst->name, g->sink->name))
	// 	path++;
	// g->data->path_n = path;
	dstr.w = 30;
	dstr.h = 30;
	nodes_draw(g, dstr);
	SDL_RenderPresent(g->data->rend);
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
