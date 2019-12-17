/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   visu.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: melalj <melalj@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/11 10:38:01 by melalj            #+#    #+#             */
/*   Updated: 2019/12/17 17:22:09 by melalj           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "visu.h"

int	map(int val, int *in, int *out)
{
	return ((val - in[0]) * (out[1] - out[0]) / (in[1] - in[0]) + out[0]);
}

int	init(t_dvisu *data)
{
	data->w_width = 1280;
	data->w_height = 720;

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

static void	visu_quit(t_dvisu data)
{
	SDL_DestroyRenderer(data.rend);
	SDL_DestroyWindow(data.window);
	SDL_Quit();
}

SDL_Texture	*get_imagetex(t_dvisu data, char *image_path)
{
	SDL_Surface *sur;
	SDL_Texture *tex;

	if (!(sur = IMG_Load(image_path)))
	{
		ft_printf("error creating surface\n");
		return (NULL);
	}
	tex = SDL_CreateTextureFromSurface(data.rend, sur);
	SDL_FreeSurface(sur);
	if (!tex)
	{
		ft_printf("error creating texture\n");
		return (NULL);
	}
	return (tex);
}

int	nodes_draw(t_dvisu data, t_graph *g, SDL_Rect dstr)
{
	SDL_Texture	*tex;
	t_node		*curr;
	int			in[2];
	int			out[2];

	ft_printf("screen width : %d --- screen height : %d\n", data.w_width, data.w_height);
	ft_printf("nbr of nodes : %zu --- max_cords x : %d - y : %d\n", g->n_nodes, g->max_c.x, g->max_c.y);
	tex = get_imagetex(data, "resources/circle.png");
	// dstr.w = 20;
	// dstr.h = 20;
	curr = g->nodes_lst;
	while (curr)
	{
		in[0] = 0;
		in[1] = g->max_c.x;
		out[0] = 0;
		out[1] = data.w_width - 100;
		dstr.x = map(curr->cords.x,in, out); // to make something centered x = w.w / 2 - rect / 2
		ft_printf("maping x : %d\n", dstr.x);
		in[0] = 0;
		in[1] = g->max_c.y;
		out[0] = 0;
		out[1] = data.w_height - 100;
		dstr.y = map(curr->cords.y,in, out); // to make something centered x = w.w / 2 - rect / 2
		ft_printf("maping y : %d\n", dstr.y);
		SDL_RenderCopy(data.rend, tex, NULL, &dstr);
		curr = curr->next;
	}
	SDL_DestroyTexture(tex);
	return (0);
}

int	visu_init(t_graph *g)
{
	t_dvisu		data;
	SDL_Rect	dstr;
	SDL_Event	event;
	int close_requested = 0;

	data.window = NULL;
	init(&data);
	// // SDL_QueryTexture(tex, NULL, NULL, &srcr.w, &srcr.h);
	dstr.w = 20;
	dstr.h = 20;
	while (!close_requested)
	{
		SDL_RenderClear(data.rend);
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT
				|| (event.type == SDL_KEYDOWN && event.key.keysym.sym == 27))
				close_requested = 1;
			else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == 105)
			{
				dstr.w += 5;
				dstr.h += 5;
			}
			else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == 111)
			{
				dstr.w -= 5;
				dstr.h -= 5;
			}
		}
		nodes_draw(data, g, dstr);
		SDL_RenderPresent(data.rend);
		SDL_PumpEvents();
	}
	visu_quit(data);
	return (0);
}
