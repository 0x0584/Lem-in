/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_graph.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: archid- <archid-@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/18 13:07:20 by archid-           #+#    #+#             */
/*   Updated: 2020/12/18 13:08:13 by archid-          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

void		bind_edges(t_graph g, t_hashnode enode, t_hashnode renode)
{
	t_edge	e;
	t_edge	re;

	e = enode.blob;
	re = renode.blob;
	if (hash_add(g->edges, enode.key, enode.blob) &&
			hash_add(g->edges, renode.key, renode.blob))
	{
		lst_push_back_blob(e->src->edges, e, sizeof(t_edge *), false);
		lst_push_back_blob(re->src->edges, re, sizeof(t_edge *), false);
	}
	else
	{
		edge_del(enode.blob);
		edge_del(renode.blob);
	}
	free(enode.key);
	free(renode.key);
}

bool		parse_edges(t_graph g, t_lst ledges)
{
	t_lstnode	walk;
	t_hashnode	enode;
	t_hashnode	renode;

	walk = lst_front(ledges);
	while (walk)
	{
		if (!edge_alloc(g, walk->blob, &enode, &renode))
			return (error_message(walk->blob));
		bind_edges(g, enode, renode);
		lst_node_forward(&walk);
	}
	return (true);
}

enum e_tag	check_tag(t_graph g, t_lstnode *walk)
{
	char *tmp;

	tmp = (*walk)->blob;
	if (!ft_strncmp(tmp, "##", 2))
	{
		lst_node_forward(walk);
		if (!ft_strcmp(tmp, "##start"))
			return (g->source ? tag_error : tag_start);
		else if (!ft_strcmp(tmp, "##end"))
			return (g->sink ? tag_error : tag_end);
	}
	return (tag_other);
}

bool		parse_vertices(t_graph g, t_lst lverts)
{
	t_lstnode	walk;
	t_hashnode	hnode;
	enum e_tag	status;

	walk = lst_front(lverts);
	while (walk)
	{
		status = check_tag(g, &walk);
		if (!status || !walk || check_tag(g, &walk) != tag_other)
			return (error_message("Wrong tag"));
		hnode = vertex_alloc(walk->blob);
		if (!hash_add(g->vertices, hnode.key, hnode.blob))
		{
			vertex_del(hnode.blob);
			return (error_message(walk->blob));
		}
		if (status == tag_start)
			g->source = hnode.blob;
		else if (status == tag_end)
			g->sink = hnode.blob;
		g->n_vertices++;
		lst_node_forward(&walk);
	}
	return (true);
}

t_graph		parse_graph(t_lst lverts, t_lst ledges)
{
	t_graph g;

	g = graph_init(hash_alloc(lst_size(lverts), vertex_del),
					hash_alloc(lst_size(ledges), edge_del));
	if (!parse_vertices(g, lverts))
		graph_del(&g);
	else if (!g->n_vertices)
	{
		error_message("Empty map");
		graph_del(&g);
	}
	else if (!parse_edges(g, ledges))
		graph_del(&g);
	else if (!g->source || !g->sink)
	{
		error_message("Source/Sink are not defined");
		graph_del(&g);
	}
	if (g)
		print_map(lverts, ledges);
	return (g);
}
