/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lem_in_main.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: melalj <melalj@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/12 22:07:41 by melalj            #+#    #+#             */
/*   Updated: 2019/11/28 15:33:21 by melalj           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../lem_in.h"

int main()
{
	t_parse *pp;
	t_node	**nodes;
	// t_parse *pe;
	t_node	*curr;
	int nodes_c;
	int i;

	pp = get_lines(&nodes_c);
	nodes = h_table(pp, nodes_c);
	i = -1;
	while (++i < nodes_c)
	{
		ft_printf("i : %d\n", i);
		curr = nodes[i];
		while (curr)
		{
			ft_printf("%s\n", curr->name);
			curr = curr->next;
		}
	}
	edges_fill(nodes, pp, nodes_c);
	return (0);
}
