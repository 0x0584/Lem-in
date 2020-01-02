/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hash_t.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: melalj <melalj@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/24 13:07:14 by melalj            #+#    #+#             */
/*   Updated: 2020/01/02 11:16:52 by melalj           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../lem_in.h"

unsigned long	hash(unsigned char *str)
{
	unsigned long	hash;
	int				c;

	hash = 5381;
	while ((c = *str++))
		hash = ((hash << 5) + hash) + c;
	/*
	** hash * 33 + c
	*/
	return (hash);
}

t_node		**h_table(t_node **refs, t_parse *lines, int nodes_c)
{
	t_node	**nodes;
	int		i;
	int		prop;

	nodes = (t_node **)malloc(sizeof(t_node *) * nodes_c);
	i = -1;
	while (++i < nodes_c)
		nodes[i] = NULL;
	lines = lines->next;
	while (lines && lines->type == 1)
	{
		if (lines->line[0] != '#' && !add_node(nodes, lines, nodes_c,
						prop, refs) && ft_printf("duplicate name error\n"))
			exit(1);
		if (lines->next)
			prop = lines->prop > 1 ? lines->prop : 0;
		lines = lines->next;
	}
	return (nodes);
}
