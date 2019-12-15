/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   visu.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: melalj <melalj@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/11 10:38:01 by melalj            #+#    #+#             */
/*   Updated: 2019/12/15 05:13:17 by melalj           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "visu.h"

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

int	visu_init(t_graph *g)
{
	t_dvisu		data;
	SDL_Texture	*tex;
	SDL_Rect srcr;
	SDL_Rect dstr;

	(void)g;
	data.window = NULL;
	init(&data);
	tex = get_imagetex(data, "resources/GIMP.png");
	SDL_RenderClear(data.rend);
	srcr.x = 0;
	srcr.y = 0;
	SDL_QueryTexture(tex, NULL, NULL, &srcr.w, &srcr.h);
	// srcr.w = 2;
	// srcr.h = 2;
	ft_printf("srcr.w : %d\n", srcr.w);
	dstr.w = 200;
	dstr.h = 200;
	dstr.x = data.w_width / 2 - dstr.w / 2; // to make something centered x = w.w / 2 - rect / 2
	dstr.y = 0;
	SDL_RenderCopy(data.rend, tex, &srcr, &dstr);
	SDL_RenderPresent(data.rend);
	SDL_PumpEvents();
	SDL_Delay(2000);
	visu_quit(data);
	return (0);
}
