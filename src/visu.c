/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   visu.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: melalj <melalj@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/11 10:38:01 by melalj            #+#    #+#             */
/*   Updated: 2020/01/09 20:15:39 by archid-          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../lem_in.h"
#ifdef USE_VISU

int	edge_draw(t_edge *edge, int type)
{
	t_dvisu		*data;

	data = get_visu_data();
	data->f = type;
	if (type == -1 && (edge->path_n == -1 && edge->residual->path_n == -1))
	{
		edge->residual->color.hex = 0x031cfc;
		edge->color.hex = 0x031cfc;
		edge->v_c = type;
		edge->residual->v_c = type;
	}
	if (type == 1)
	{
		edge->residual->path_n = data->path_n;
		edge->path_n = data->path_n;
		edge->color.hex = 0x032050 + 50000 * edge->path_n;
		edge->residual->color.hex = 0x032050 + 50000 * edge->path_n;
		edge->v_c = type;
		edge->residual->v_c = type;
	}
	if (type == 2)
	{
		edge->residual->path_n = data->path_n;
		edge->path_n = data->path_n;
		edge->color.hex = 0x032050 + 20000 * edge->path_n;
		edge->residual->color.hex = 0x032050 + 20000 * edge->path_n;
		edge->v_c = type;
		edge->residual->v_c = type;
	}
	if (type == 3)
	{
		edge->residual->path_n = data->path_n;
		edge->path_n = data->path_n;
		edge->color.hex = 0x00ff00;
		edge->residual->color.hex = 0x00ff00;
		edge->v_c = type;
		edge->residual->v_c = type;
	}
	SDL_RenderPresent(data->rend);
	return (1);
}

static t_dvisu		*__visu_data(t_dvisu *v)
{
	static t_dvisu *visu_data = NULL;

	if (v != NULL)
		visu_data = v;
	return (visu_data);
}

void		set_visu_data(t_dvisu *v)
{
	(void)__visu_data(v);
}

t_dvisu		*get_visu_data(void)
{
	return (__visu_data(NULL));
}

static void	data_init(t_dvisu *data)
{
	data->w_width = VISU_WIN_WIDTH;
	data->w_height = VISU_WIN_HEIGHT;
	data->rend = NULL;
	data->window = NULL;
	data->s_surface = NULL;
	data->tex = NULL;
	data->f = 0;
	data->path_n = 0;
}

int	init(void)
{
	t_dvisu *data;

	ft_printf("initing visu\n");
	data_init(data = get_visu_data());
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

void	visu_quit(void)
{
	t_dvisu *data;

	data = get_visu_data();
	SDL_DestroyRenderer(data->rend);
	SDL_DestroyWindow(data->window);
	SDL_Quit();
}

int	visu_init(t_graph *g)
{
	SDL_Event	event;
	int			close_requested;
	t_dvisu *data;

	init();
	close_requested = 0;
	data = get_visu_data();
	while (!close_requested)
	{
		SDL_RenderClear(data->rend);
		graph_draw(g);
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT
				|| (event.type == SDL_KEYDOWN && event.key.keysym.sym == 27))
				close_requested = 1;
		}
		SDL_RenderPresent(data->rend);
		SDL_PumpEvents();
	}
	return (0);
}
#endif
