/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: melalj <melalj@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/24 13:54:11 by melalj            #+#    #+#             */
/*   Updated: 2019/11/29 17:01:42 by melalj           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../lem_in.h"

t_node	*new_node(t_parse *line, int prop)
{
	t_node	*new_n;
	char	**s_line;

	s_line = ft_strsplit(line->line, ' ');
	new_n = (t_node *)malloc(sizeof(t_node));
	new_n->name = ft_strdup(s_line[0]);
	free_tab(s_line);
	new_n->edges = NULL;
	new_n->next = NULL;
	new_n->type = NODE_DEFAULT;
	ft_printf("prop : %d\n", prop);
	if (prop > 1)
		new_n->type = (prop == 2 ? NODE_START : NODE_END);
	return (new_n);
}

int		add_node(t_node **lst_node, t_parse *lines, int nodes_c, int prop)
{
	char	**s_lines;
	t_node	*curr;
	int		hash_h;

	s_lines = ft_strsplit(lines->line, ' ');
	// ft_printf("added node %s in the hash %d\n", s_lines[0], hash((unsigned char*)s_lines[0]) % nodes_c);
	hash_h = hash((unsigned char*)s_lines[0]) % nodes_c;
	if (!lst_node[hash_h])
	{
		lst_node[hash_h] = new_node(lines, prop);
		free_tab(s_lines);
		return (1);
	}
	curr = lst_node[hash_h];
	if (ft_strequ((curr)->name, s_lines[0]) && free_tab(s_lines))
		return (0);
	while (curr->next)
	{
		ft_printf("%s | %s\n", s_lines[0], curr->name);
		if (ft_strequ((curr)->name, s_lines[0]) && free_tab(s_lines))
			return (0);
		(curr) = (curr)->next;
	}
	(curr)->next = new_node(lines, prop);
	free_tab(s_lines);
	return (1);
}

t_node	*get_node(t_node **lst_node, char *name, int nodes_c)
{
	int		hash_h;
	t_node	*curr;

	hash_h = hash((unsigned char*)name) % nodes_c;
	curr = lst_node[hash_h];
	while (curr)
	{
		if (ft_strequ((curr)->name, name))
			return (curr);
		curr = curr->next;
	}
	return (NULL);
}

int	add_edge(t_node *src, t_node *dst)
{
	t_edges *curr;

	if (src->edges == NULL)
	{
		src->edges = (t_edges *)malloc(sizeof(t_edges));
		src->edges->node_src = src;
		src->edges->node_dst = dst;
		src->edges->next = NULL;
	}
	else
	{
		curr = src->edges;
		while (curr->next)
			curr = curr->next;
		curr->next = (t_edges *)malloc(sizeof(t_edges));
		curr->next->node_src = src;
		curr->next->node_dst = dst;
		curr->next->next = NULL;
	}
	return (1);
}

int	edges_fill(t_node **lst_node, t_parse *lines, int nodes_c)
{
	char	**s_lines;
	t_node	*node[2];

	while (lines->type < 2)
		lines = lines->next;
	while (lines)
	{
		if (lines->line[0] == '#')
		{
			lines = lines->next;
			continue;
		}
		s_lines = ft_strsplit(lines->line, '-');
		node[0] = get_node(lst_node, s_lines[0], nodes_c);
		node[1] = get_node(lst_node, s_lines[1], nodes_c);
		add_edge(node[0], node[1]);
		ft_printf("node %s, %d --- edge %s\n", node[0]->name, node[0]->type	,node[0]->edges->node_dst->name);
		add_edge(node[1], node[0]);
		// ft_printf("node %s --- from %s to %s\n", node[1]->name, node[1]->edges->node_src->name, node[1]->edges->node_dst->name);
		// ft_printf("node %s --- edge %s\n", node[1]->name, node[1]->edges->node_dst->name);
		lines = lines->next;
		free_tab(s_lines);
	}
	return (1);
}


// int	nodes_exract(void)
// {
// 	char	*line;
// 	char	**sline;
// 	int		i;

// 	line = NULL;
// 	read_line(0, &line);
// 	i = -1;
// 	if (line && (sline = ft_strsplit(line, ' ')))
// 		while (sline[++i])
// 		{
// 			if (!i)
// 				add_node();
// 			if (i && !ft_isnumber(sline[i]))
// 				break ;
// 		}
// 	if (i != 3)
// 		return (0);
// 	return (1);
// }
 