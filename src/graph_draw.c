/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   graph_draw.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: melalj <melalj@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/23 19:30:44 by melalj            #+#    #+#             */
/*   Updated: 2020/01/02 12:06:48 by melalj           ###   ########.fr       */
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

int	nodes_draw(t_graph *g, SDL_Rect dstr)
{
	SDL_Texture	*tex;
	t_node		*curr;
	int			*ranges_x;
	int			*ranges_y;

	// ft_printf("screen width : %d --- screen height : %d\n", data.w_width, data.w_height);
	// ft_printf("nbr of nodes : %zu --- max_cords x : %d - y : %d\n", g->n_nodes, g->max_c.x, g->max_c.y);
	// dstr.w = 20; //temp zoom effects (not really a zoom)
	// dstr.h = 20;
	curr = g->nodes_lst;
	while (curr && !edges_draw(g, curr))
		curr = curr->next;
	curr = g->nodes_lst;
	ranges_x = range_comp(0, g->max_c.x, 0, g->data->w_width - 50);
	ranges_y = range_comp(0, g->max_c.y, 0, g->data->w_height - 100 - 250);
	while (curr)
	{
		if (ft_strequ(curr->name, g->start->name))
			tex = get_imagetex(g->data, "resources/start.png");
		else if (ft_strequ(curr->name, g->sink->name))
			tex = get_imagetex(g->data, "resources/end.png");
		else
			tex = get_imagetex(g->data, "resources/circle.png");
		dstr.x = map(curr->cords.x, ranges_x);
		dstr.y = map(curr->cords.y, ranges_y);
		// edges_draw(g->data, g, curr); // edges drawn on top of nodes
		SDL_RenderCopy(g->data->rend, tex, NULL, &dstr);
		curr = curr->next;
		SDL_DestroyTexture(tex);
	}
	free(ranges_x);
	free(ranges_y);
	return (0);
}

int	graph_draw(t_graph *g)
{
	SDL_Rect	nodes_size;

	nodes_size.w = 30;
	nodes_size.h = 30;
	nodes_draw(g, nodes_size);
	return (1);
}
#endif
