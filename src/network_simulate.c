/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   network_simulate.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: archid- <archid-@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/18 13:22:00 by archid-           #+#    #+#             */
/*   Updated: 2020/12/18 19:24:52 by archid-          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "visu.h"

static size_t	compute_difference(t_lst flows)
{
	t_lstnode	walk;
	size_t		sum;
	t_flow		flow_a;
	t_flow		flow_b;

	sum = 0;
	walk = lst_front(flows);
	flow_a = lst_rear_blob(flows);
	while (walk)
	{
		flow_b = walk->blob;
		flow_b->total_units = flow_a->size - flow_b->size;
		sum += flow_b->total_units;
		lst_node_forward(&walk);
	}
	return (sum);
}

static void		network_regulate(t_network net)
{
	t_lstnode	walk;
	t_flow		flow_a;
	size_t		sum;
	size_t		div;
	size_t		mod;

	sum = compute_difference(net->flows);
	div = (net->n_units - sum) / lst_size(net->flows);
	mod = (net->n_units - sum) % lst_size(net->flows);
	walk = lst_front(net->flows);
	while (walk)
	{
		flow_a = walk->blob;
		flow_a->total_units += div;
		if (mod)
		{
			flow_a->total_units++;
			mod--;
		}
		lst_node_forward(&walk);
	}
}

static size_t	network_push(t_network net, size_t *unit)
{
	t_lstnode	walk;
	size_t		maxflow;

	walk = lst_front(net->flows);
	maxflow = 0;
	while (walk && *unit <= net->n_units)
	{
		if (flow_push(walk->blob, *unit))
		{
			*unit += 1;
			maxflow++;
		}
		lst_node_forward(&walk);
	}
	return (maxflow);
}

static bool		network_sync(t_network net)
{
	t_lstnode	walk;
	bool		flag;

	flag = false;
	walk = lst_front(net->flows);
	while (walk)
	{
		flag = flow_sync(walk->blob) || flag;
		lst_node_forward(&walk);
	}
	return (flag);
}

size_t			network_simulate(t_network net, bool visualize)
{
	size_t	instructions;
	size_t	unit;
	bool	flag;
	size_t	maxflow;

	network_regulate(net);
	unit = 1;
	flag = true;
	instructions = 0;
	if (visualize)
		show(net, 0);
	while (flag)
	{
		maxflow = network_push(net, &unit);
		if (visualize)
			show(net, maxflow);
		if ((flag = network_sync(net)))
			instructions++;
	}
	if (visualize)
		show(net, maxflow);
	return (instructions);
}
