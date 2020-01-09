/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   edges_draw.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: melalj <melalj@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/23 20:06:04 by melalj            #+#    #+#             */
/*   Updated: 2020/01/09 01:33:21 by archid-          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../lem_in.h"
#ifdef USE_VISU

int		edge_bfs_iter(t_edge *edge)
{
	t_dvisu *data;

	data = get_visu_data();
	if (edge->v_c < 0 || edge->residual->v_c < 0)
	{
		SDL_SetRenderDrawColor(data->rend, 255, 150, 255, 255);
		if (edge->v_c == -1 || edge->residual->v_c == -1)
			SDL_Delay(300);
		edge->v_c--;
		edge->residual->v_c--;
		return (1);
	}
	else
		return (0);
}

int		edge_bfs_path(t_edge *edge)
{
	int change;
	t_dvisu *data;

	data = get_visu_data();
	change = (data->path_n != 0) ?  data->path_n * 100 : 1;
	change = (edge->path_n == -1) ? change : edge->path_n;
	if (edge->v_c > 0 || edge->residual->v_c > 0)
	{
		SDL_SetRenderDrawColor(data->rend, 255 * change ,
							   150 * change, 1 * change, 255);
		if (edge->v_c == 1 || edge->residual->v_c == 1)
			SDL_Delay(300);
		edge->path_n = (edge->path_n == -1) ? data->path_n : edge->path_n;
		edge->residual->path_n = (edge->path_n == -1)
			? data->path_n : edge->path_n;
		edge->v_c++;
		edge->residual->v_c++;
		return (1);
	}
	else
		return (0);
}

int	edges_draw(t_node *node)
{
	t_edge	*curr;
	int		*ranges_x;
	int		*ranges_y;
	t_cords	src;
	t_cords	dst;
	t_dvisu *data;

	data = get_visu_data();
	curr = node->edges;
	// make the line
	ranges_x = range_comp(0, data->max_c.x, 0, data->w_width - 50);
	src.x = map(node->cords.x, ranges_x) + 10;
	ranges_y = range_comp(0, data->max_c.y, 0, data->w_height - 100 - 250);
	src.y = map(node->cords.y, ranges_y) + 10;
	while (curr)
	{
		curr->color.hex = (data->f == 1 && curr->path_n == -1)
			? 0 : curr->color.hex;
		if (curr->color.hex)
		{
			SDL_SetRenderDrawColor(data->rend, curr->color.rbg[2],
								   curr->color.rbg[1], curr->color.rbg[0], 255);
			if (curr->v_c == 1 || curr->v_c == -1
				|| curr->residual->v_c == 1 || curr->residual->v_c == -1)
				SDL_Delay(200);
			curr->v_c +=5;
			curr->residual->v_c +=5;
		}
		else
			SDL_SetRenderDrawColor(data->rend, 150, 0, 255, 255);
		dst.x = map(curr->node_dst->cords.x, ranges_x) + 10;
		dst.y = map(curr->node_dst->cords.y, ranges_y) + 10;
		SDL_RenderDrawLine(data->rend, src.x, src.y, dst.x, dst.y);
		curr = curr->next;
		ft_putchar('c');
	}
	ft_putendl("");
	free(ranges_x);
	free(ranges_y);
	SDL_SetRenderDrawColor(data->rend, 0, 0, 0, 255);
	return (0);
}
#endif
