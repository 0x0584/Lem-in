/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: melalj <melalj@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/24 13:54:11 by melalj            #+#    #+#             */
/*   Updated: 2019/12/31 20:10:13 by archid-          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../lem_in.h"

t_node	*new_node(int index, t_parse *line, int prop)
{
	t_node	*new_n;
	char	**s_line;

	s_line = ft_strsplit(line->line, ' ');
	new_n = (t_node *)malloc(sizeof(t_node));
	new_n->name = ft_strdup(s_line[0]);
	new_n->cords.x = ft_atoi(s_line[1]);
	new_n->cords.y = ft_atoi(s_line[2]);
	free_tab(s_line);
	new_n->edges = NULL;
	new_n->next = NULL;
	new_n->type = NODE_DEFAULT;
	new_n->seen = false;
	new_n->index = index;
	// ft_printf("prop : %d\n", prop);
	if (prop > 1)
		new_n->type = (prop == 2 ? NODE_START : NODE_END);
	return (new_n);
}

int		add_node(t_node **lst_node, t_parse *lines, int nodes_c, int prop, t_node **refs)
{
	static size_t	index = 0;
	char			**s_lines;
	t_node			*curr;
	int				hash_h;

	s_lines = ft_strsplit(lines->line, ' ');
	hash_h = hash((unsigned char*)s_lines[0]) % nodes_c;
	refs[index] = new_node(index, lines, prop);
	// ft_printf("added node %s in the hash %d\n", s_lines[0],
	//						hash((unsigned char*)s_lines[0]) % nodes_c);
	if (!lst_node[hash_h])
	{
		lst_node[hash_h] = refs[index++];
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
	(curr)->next = refs[index++];
	free_tab(s_lines);
	return (1);
}

// get node based on a hash
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

/*
   add the edge and it's residual
*/

int	add_edge(t_node *src, t_node *dst, bool is_residual, t_edge *e)
{
	t_edge *curr;
	t_edge *tmp;

	is_residual ^= true;
	if (src->edges == NULL)
	{
		src->edges = (t_edge *)malloc(sizeof(t_edge));
		tmp = src->edges;
		src->edges->node_src = src;
		src->edges->node_dst = dst;
		src->edges->next = NULL;
	}
	else
	{
		curr = src->edges;
			ft_printf("%s - %s | %s - %s\n", curr->node_src->name, src->name, curr->node_dst->name, dst->name);
			if ((ft_strequ(curr->node_src->name, src->name) && ft_strequ(curr->node_dst->name, dst->name))
				|| (ft_strequ(curr->node_src->name, dst->name) && ft_strequ(curr->node_dst->name, src->name)))
			{
				ft_printf("error : duplicated edge\n");
				exit(1);
			}
		while (curr->next)
		{
			curr = curr->next;
		}
		curr->next = (t_edge *)malloc(sizeof(t_edge));
		curr->next->node_src = src;
		curr->next->node_dst = dst;
		curr->next->next = NULL;
		tmp = curr->next;
	}

#ifdef USE_VISU
	tmp->v_c = 0;
	tmp->path_n = -1;
	tmp->drawn = 0;
#endif

	if (is_residual == false)
	{
		// this need to stay tmp not curr
		tmp->residual = e;
		e->residual = tmp;

		e->seen = 0;
		tmp->seen = 0;

		ft_printf("edge: <%s, %s> | residual <%s, %s>\n",
				  e->node_src->name, e->node_dst->name,
				  e->residual->node_src->name, e->residual->node_dst->name);
		return 0;
	}
	add_edge(dst, src, true, tmp);
	return (1);
}

int	edges_fill(t_node **lst_node, t_parse *lines, int nodes_c)
{
	char	**s_lines;
	t_node	*node[2];

	while (lines && lines->type < 2) /* this just to walk till the edges
										in the stored lines ant number has
										type 0 and nodes has type 1 */
	{
		ft_printf(">>> %s\n", lines->line);
		lines = lines->next;
	}
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
		ft_printf("allo\n");
		add_edge(node[0], node[1], false, NULL);
		ft_printf("node %s, %d --- edge %s\n", node[0]->name,
				  node[0]->type	,node[0]->edges->node_dst->name);
  		/* add_edge(node[1], node[0]); */
		ft_printf("node %s --- from %s to %s\n", node[1]->name,
				  node[1]->edges->node_src->name,
				  node[1]->edges->node_dst->name);
		ft_printf("node %s --- edge %s\n", node[1]->name,
				  node[1]->edges->node_dst->name);
		lines = lines->next;
		free_tab(s_lines);
	}
	return (1);
}


// !?
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
