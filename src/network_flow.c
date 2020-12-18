/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   network_flow.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: archid- <archid-@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/18 13:15:42 by archid-           #+#    #+#             */
/*   Updated: 2020/12/18 13:18:31 by archid-          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "network.h"

void		flow_free(void *blob)
{
	t_flow flow;
	size_t i;

	if (!(flow = blob))
		return ;
	i = 0;
	while (i < flow->size)
		free(flow->stage[i++].vertex);
	free(flow->stage);
	free(flow);
}

t_flow		flow_alloc(t_lst path)
{
	t_flow		flow;
	t_lstnode	walk;
	t_edge		e;
	size_t		i;

	flow = malloc(sizeof(struct s_flow));
	flow->total_units = 0;
	flow->size = lst_size(path);
	flow->stage = malloc(flow->size * sizeof(t_flow_pair));
	i = 0;
	walk = lst_front(path);
	while (walk)
	{
		e = walk->blob;
		flow->stage[i].vertex = ft_strdup(e->dst->name);
		flow->stage[i].unit = 0;
		i++;
		lst_node_forward(&walk);
	}
	return (flow);
}

bool		flow_push(t_flow flow, size_t unit)
{
	if (!flow->total_units)
		return (false);
	flow->stage->unit = unit;
	flow->total_units--;
	return (true);
}

bool		flow_sync(t_flow flow)
{
	size_t	i;
	bool	flag;

	flag = false;
	i = flow->size - 1;
	while (i)
	{
		flag = (flow->stage[i - 1].unit != (size_t)NIL_ANT) || flag;
		flow->stage[i].unit = flow->stage[i - 1].unit;
		i--;
	}
	flow->stage->unit = NIL_ANT;
	return (flag);
}

void		path_to_flow(void *path, void *flows)
{
	lst_push_back_blob(flows, flow_alloc(path), sizeof(t_flow), false);
}
