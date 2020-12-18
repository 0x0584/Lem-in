/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   network_extra.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: archid- <archid-@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/18 20:28:00 by archid-           #+#    #+#             */
/*   Updated: 2020/12/18 20:30:37 by archid-          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "network.h"

size_t		nil_unit(void)
{
	return ((size_t)-1);
}

void		compute_maxflow(void *flow, void *maxflow)
{
	size_t		i;

	i = 0;
	while (i < ((t_flow)flow)->size)
		*((size_t *)maxflow) += ((t_flow)flow)->stage[i++].unit != nil_unit();
}

void		compute_maxflow_(void *flow, void *maxflow)
{
	*((size_t *)maxflow) += ((t_flow)flow)->size;
}
