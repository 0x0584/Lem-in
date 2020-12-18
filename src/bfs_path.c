/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bfs_path.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: archid- <archid-@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/18 12:52:52 by archid-           #+#    #+#             */
/*   Updated: 2020/12/18 12:54:00 by archid-          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "graph.h"

static bool		valid_pair(t_edge_pair *current, t_edge_pair *prev)
{
	return (!prev || !prev->from || current->edge == prev->from);
}

static t_lst	backtrack_source(t_hash parent, const char *key,
									t_edge_pair *prev)
{
	t_lstnode	walk;
	t_edge_pair *current;
	t_lst		path;

	walk = lst_front(hash_get(parent, key, NULL));
	while (walk)
	{
		if (valid_pair(current = walk->blob, prev) &&
			(path = prev && !current->from
				? lst_alloc(blob_keep)
					: backtrack_source(parent, current->from->dst->name,
										current)))
		{
			edge_mark(current->edge, M_BELONG_TO_PATH);
			return (lst_push_back_blob(path, current->edge, sizeof(t_edge),
										false));
		}
		lst_node_forward(&walk);
	}
	return (NULL);
}

t_lst			construct_path(t_graph g, t_hash parent)
{
	return (backtrack_source(parent, g->sink->name, NULL));
}
