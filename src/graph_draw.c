/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   graph_draw.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: melalj <melalj@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/23 19:30:44 by melalj            #+#    #+#             */
/*   Updated: 2020/01/09 04:09:12 by archid-          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../lem_in.h"
#ifdef USE_VISU
SDL_Texture	*get_imagetex(t_dvisu *data, char *image_path)
{
	SDL_Surface *sur;
	SDL_Texture *tex;

	if (!(sur = IMG_Load(image_path)))
	{
		ft_printf("error creating surface\n");
		return (NULL);
	}
	tex = SDL_CreateTextureFromSurface(data->rend, sur);
	SDL_FreeSurface(sur);
	if (!tex)
	{
		ft_printf("error creating texture: %s\n", SDL_GetError());
		return (NULL);
	}
	return (tex);
}

int	graph_draw(t_graph *g)
{
	SDL_Rect	nodes_size;
	SDL_Texture	*tex;
	t_node		*curr;
	int			*ranges_x;
	int			*ranges_y;
	t_dvisu		*data;

	nodes_size.w = 30;
	nodes_size.h = 30;

	data = get_visu_data();
	curr = g->nodes_lst;
	while (curr && !edges_draw(curr))
		curr = curr->next;
	ft_putendl(" edges are drawn");
	curr = g->nodes_lst;
	ranges_x = range_comp(0, data->max_c.x, 0, data->w_width - 50);
	ranges_y = range_comp(0, data->max_c.y, 0, data->w_height - 100 - 250);
	while (curr)
	{
		if (ft_strequ(curr->name, g->source->name))
			tex = get_imagetex(data, "resources/start.png");
		else if (ft_strequ(curr->name, g->sink->name))
			tex = get_imagetex(data, "resources/end.png");
		else
			tex = get_imagetex(data, "resources/circle.png");
		nodes_size.x = map(curr->cords.x, ranges_x);
		nodes_size.y = map(curr->cords.y, ranges_y);
		/* edges_draw(curr); // edges drawn on top of nodes */
		SDL_RenderCopy(data->rend, tex, NULL, &nodes_size);
		curr = curr->next;
		SDL_DestroyTexture(tex);
	}
	free(ranges_x);
	free(ranges_y);
	return (1);
}
#endif
