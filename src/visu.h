/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   visu.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: melalj <melalj@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/09 00:20:55 by melalj            #+#    #+#             */
/*   Updated: 2019/12/15 05:12:22 by melalj           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef VISU_H
# define VISU_H
# include "../visu_lib/SDL_LIB/2.0.10/include/SDL2/SDL.h"
# include "../visu_lib/SDL_IMG_LIB/include/SDL2/SDL_image.h"
# include "../lem_in.h"

typedef struct s_dvisu
{
	SDL_Window		*window;
	SDL_Surface		*s_surface;
	int				w_width;
	int				w_height;
	SDL_Renderer	*rend;
}				t_dvisu;

int visu_init(t_graph *g);
#endif