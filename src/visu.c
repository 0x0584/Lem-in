/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   visu.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: melalj <melalj@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/11 10:38:01 by melalj            #+#    #+#             */
/*   Updated: 2019/12/22 14:59:58 by melalj           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../lem_in.h"

int	map(int val, int *ranges)
{
	int d;
	int result;

	d = (ranges[1] - ranges[0]) + ranges[2];
	d = (!d) ? 1 : d;
	result = (val - ranges[0]) * (ranges[3] - ranges[2]) / d;
	// ft_printf("result %d --- %d %d d %d\n", result, ranges[0], ranges[1], d);
	return (result);
}

void	data_init(t_dvisu *data)
{
	data->w_width = 1280;
	data->w_height = 720;
	data->rend = NULL;
	data->window = NULL;
	data->s_surface = NULL;
	data->tex = NULL;
}

int	init(t_dvisu *data)
{
	data_init(data);
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

int	*range_comp(int in_s, int in_e, int out_s, int out_e)
{
	int *comp_range;

	if (!(comp_range = (int *)malloc(sizeof(int) * 4)))
		return (NULL);
	comp_range[0] = in_s;
	comp_range[1] = in_e;
	comp_range[2] = out_s;
	comp_range[3] = out_e;
	return (comp_range);
}

int	edge_draw(t_graph *g, t_edge *edge, int type)
{
	// int		*ranges_x;
	// int		*ranges_y;
	// t_cords	src;
	// t_cords	dst;
	SDL_Rect	dstr;

	edge->v_c = type;
	dstr.w = 20;
	dstr.h = 20;
	// ft_printf("edge %s - %s to color\n", edge->node_src->name, edge->node_dst->name);
	nodes_draw(*(g->data), g, dstr);
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

int	edges_draw(t_dvisu data, t_graph *g, t_node *node)
{
	t_edge	*curr;
	int		*ranges_x;
	int		*ranges_y;
	t_cords	src;
	t_cords	dst;

	curr = node->edges;
	ranges_x = range_comp(0, g->max_c.x, 0, data.w_width - 100);
	src.x = map(node->cords.x, ranges_x) + 10;
	ranges_y = range_comp(0, g->max_c.y, 0, data.w_height - 100);
	src.y = map(node->cords.y, ranges_y) + 10;
	while (curr)
	{
	// ft_printf("drawing edge %s - %s\n", curr->node_src->name, curr->node_dst->name);
	if (curr->v_c > 0 || curr->residual->v_c > 0)
	{
		SDL_SetRenderDrawColor(data.rend, 255, 150, 0, 255);
		curr->v_c++;
		curr->residual->v_c++;
		if(curr->v_c == 1 || curr->residual->v_c == 1)
			SDL_Delay(300);
		// ft_printf ("     colored\n");
	}
	else if (curr->v_c < 0 || curr->residual->v_c < 0)
	{
		SDL_SetRenderDrawColor(data.rend, 255, 150, 255, 255);
		curr->v_c--;
		curr->residual->v_c--;
		if(curr->v_c == -1 || curr->residual->v_c == -1)
			SDL_Delay(300);
		// ft_printf ("     colored\n");
	}
	else
		SDL_SetRenderDrawColor(data.rend, 255, 0, 0, 255);
		dst.x = map(curr->node_dst->cords.x, ranges_x) + 10;
		dst.y = map(curr->node_dst->cords.y, ranges_y) + 10;
		SDL_RenderDrawLine(data.rend, src.x, src.y, dst.x, dst.y);
		curr = curr->next;
	}
	free(ranges_x);
	free(ranges_y);
	SDL_SetRenderDrawColor(data.rend, 0, 0, 0, 255);
	return (0);
}

int	nodes_draw(t_dvisu data, t_graph *g, SDL_Rect dstr)
{
	SDL_Texture	*tex;
	t_node		*curr;
	int			*ranges;

	// ft_printf("screen width : %d --- screen height : %d\n", data.w_width, data.w_height);
	// ft_printf("nbr of nodes : %zu --- max_cords x : %d - y : %d\n", g->n_nodes, g->max_c.x, g->max_c.y);
	// dstr.w = 20; //temp zoom effects (not really a zoom)
	// dstr.h = 20;
	curr = g->nodes_lst;
	while (curr && !edges_draw(data, g, curr))
		curr = curr->next;
	curr = g->nodes_lst;
	while (curr)
	{
		if (ft_strequ(curr->name, g->start->name))
			tex = get_imagetex(data, "resources/start.png");
		else if (ft_strequ(curr->name, g->sink->name))
			tex = get_imagetex(data, "resources/end.png");
		else
			tex = get_imagetex(data, "resources/circle.png");
		ranges = range_comp(0, g->max_c.x, 0, data.w_width - 100);
		dstr.x = map(curr->cords.x, ranges);
		ranges = range_comp(0, g->max_c.y, 0, data.w_height - 100);
		dstr.y = map(curr->cords.y, ranges);
		// edges_draw(data, g, curr);
		SDL_RenderCopy(data.rend, tex, NULL, &dstr);
		curr = curr->next;
		SDL_DestroyTexture(tex);
	}
	return (0);
}

t_dvisu	visu_init(t_graph *g)
{
	t_dvisu		data;
	SDL_Rect	dstr;
	SDL_Event	event;
	int			close_requested;

	close_requested = 0;
	init(&data);
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
	return (data);
}
