/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   graph.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: archid- <archid-@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/30 19:03:24 by archid-           #+#    #+#             */
/*   Updated: 2019/12/08 21:19:48 by archid-          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../lem_in.h"

/*
  TODO: nodes should be inserted at place using insertion sort
  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

  using an array to hold each node,
*/

void	helper_merge_halfs(t_node **clone, t_node **refs, size_t low, size_t high)
{
	size_t mid;
	size_t left;
	size_t right;
	size_t sort_index;

	left = low;
	right = MID(low, high);
	sort_index = low;
	mid = MID(low, high);
	while (sort_index < high)
		if (left < mid && (right >= high || clone[left]->index <= clone[right]->index))
			refs[sort_index++] = clone[left++];
		else
			refs[sort_index++] = clone[right++];
}

void	helper_merge_sort(t_node **clone, t_node **refs, size_t low, size_t high)
{
	size_t mid;

	if (high - low < 2)
		return ;
	mid = MID(low, high);
	helper_merge_sort(refs, clone, low, mid);
	helper_merge_sort(refs, clone, mid, high);
	helper_merge_halfs(clone, refs, low, high);
}

t_node	**graph_sort_refs(t_graph *g)
{
	t_node **clone;

	clone = CLONE(g->nodes_ref, sizeof(t_node *) * g->n_nodes);
	helper_merge_sort(clone, g->nodes_ref, 0, g->n_nodes);
	free(clone);
	return (clone);
}

t_node	*adjacency_list(t_graph *g)
{
	t_node *head;


}
