/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   correction.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: archid- <archid-@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/26 23:46:38 by archid-           #+#    #+#             */
/*   Updated: 2020/12/18 12:48:42 by archid-          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "graph.h"

static void		swap_paths(t_lst path_a, t_lstnode walk_path_a,
							t_lst path_b, t_lstnode walk_path_b)
{
	t_lstnode	tmp;
	t_lst		front;

	set_edge_to_path(walk_path_a->blob);
	set_edge_to_path(walk_path_a->next->blob);
	set_edge_to_path(walk_path_b->blob);
	set_edge_to_path(walk_path_b->prev->blob);
	front = lst_alloc(blob_keep);
	while (lst_front(path_b) != walk_path_b)
		lst_push_back(front, lst_pop_front(path_b));
	tmp = lst_rear(front);
	while (lst_rear(path_a) != walk_path_a)
		lst_insert_after(front, tmp, lst_pop_back(path_a));
	while (!lst_empty(path_b))
		lst_push_back(path_a, lst_pop_front(path_b));
	while (!lst_empty(front))
		lst_push_back(path_b, lst_pop_front(front));
	lst_del(&front);
}

static void		remove_residual(t_lst path_a, t_lstnode *walk_a,
								t_lst path_b, t_lstnode *walk_b)
{
	lst_node_backward(walk_a);
	lst_node_forward(walk_b);
	while (residual_of((*walk_a)->next->blob, (*walk_b)->prev->blob))
	{
		set_edge_fresh((*walk_a)->next->blob);
		set_edge_fresh((*walk_b)->prev->blob);
		lst_remove_next(path_a, *walk_a);
		lst_remove_previous(path_b, *walk_b);
	}
	swap_paths(path_a, *walk_a, path_b, *walk_b);
}

static void		correct_path(t_lst path_a, t_lst paths)
{
	t_lstnode	walk_a;
	t_lstnode	walk_b;
	t_lstnode	path_b;

	walk_a = lst_front(path_a);
	while (walk_a)
	{
		while (walk_a && !residual(walk_a->blob))
			if (!lst_node_forward(&walk_a))
				return ;
		path_b = lst_front(paths);
		while (path_b)
		{
			walk_b = lst_front(path_b->blob);
			while (walk_b && !residual_of(walk_a->blob, walk_b->blob))
				lst_node_forward(&walk_b);
			if (walk_b)
			{
				remove_residual(path_a, &walk_a, path_b->blob, &walk_b);
				break ;
			}
			lst_node_forward(&path_b);
		}
	}
}

void			correct_paths(t_lst paths)
{
	t_lstnode	walk;
	t_lstnode	tmp;
	t_lst		paths2;

	paths2 = lst_copy_shallow(paths);
	walk = lst_front(paths2);
	while (walk)
	{
		correct_path(walk->blob, paths2);
		tmp = walk;
		lst_node_forward(&walk);
		lst_remove(paths2, &tmp);
	}
	lst_del(&paths2);
}

int				shortest_path(void *path_a, void *path_b)
{
	size_t size_a;
	size_t size_b;

	size_a = lst_size(path_a);
	size_b = lst_size(path_b);
	if (size_b == size_a)
		return (0);
	else
		return ((size_a < size_b) ? 1 : -1);
}
