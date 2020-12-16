/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: archid- <archid-@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/20 01:02:06 by archid-           #+#    #+#             */
/*   Updated: 2020/12/16 18:13:34 by archid-          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

int		g_state = 0;
int		g_ants = 0;
bool	g_verbose = 1;
char	*g_error_line = NULL;

bool		ft_isnumber(char *s)
{
	while (*s)
		if (!ft_isdigit(*s++))
			return (false);
	return (true);
}

bool		error_message(char *line)
{
	if (g_verbose)
		ft_asprintf(&g_error_line, ": `%s` is not valid", line);
	ft_dprintf(2, "ERROR%s\n", g_error_line);
	ft_strchange(&g_error_line, NULL);
	return (false);
}

bool		valid_vertex_char(char c)
{
	return (c && c != ' ' && c != '-');
}

bool		valid_edge_line(char *line)
{
	int i;

	i = 0;
	if (*line == 'L' || (*line == '#' && *(line + 1) == '#'))
		return (false);
	while (valid_vertex_char(line[i]))
		i++;
	if (!line[i] || line[i] == ' ')
		return (false);
	i++;
	while (valid_vertex_char(line[i]))
		i++;
	return (line[i] == '\0');
}

bool		valid_vertex_line(char *line)
{
	int i;

	if (*line == '#' && *(line + 1) == '#')
		return (true);
	if (*line == 'L')
		return (false);
	i = 0;
	while (valid_vertex_char(line[i]))
		i++;
	if (!line[i] || line[i] == '-')
		return (false);
	i++;
	while (line[i] && ft_isdigit(line[i]))
		i++;
	if (line[i] != ' ')
		return (false);
	i++;
	while (line[i] && ft_isdigit(line[i]))
		i++;
	return (line[i] == '\0');
}

int			valid_comment(char *line)
{
	return (*line == '#' && *(line + 1) != '#');
}

bool		valid_line(char *line, t_lst verts, t_lst edges)
{
	if (g_state == 0 && ft_isnumber(line))
	{
		g_ants = ft_atoi(line);
		free(line);
		if (g_ants <= 0)
			return (error_message("number of ants"));
		g_state++;
		return (true);
	}
	if (g_state == 1 && valid_vertex_line(line))
	{
		lst_push_back_blob(verts, line, sizeof(char *), false);
		return (true);
	}
	if (valid_edge_line(line))
	{
		g_state++;
		lst_push_back_blob(edges, line, sizeof(char *), false);
		return (true);
	}
	return (error_message(line));
}

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

void		print_line(void *line)
{
	ft_printf("%s\n", line);
}

void		print_map(t_lst lverts, t_lst ledges)
{
	ft_printf("%d\n", g_ants);
	lst_iter(lverts, true, print_line);
	lst_iter(ledges, true, print_line);
	ft_putendl("");
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

t_graph		read_graph(void)
{
	t_lst	verts;
	t_lst	edges;
	t_graph g;
	char	*line;
	bool	valid;

	valid = true;
	verts = lst_alloc(blob_free);
	edges = lst_alloc(blob_free);
	while (valid && gnl(0, &line) > 0 && *line)
		if (valid_comment(line))
			free(line);
		else if (!valid_line(line, verts, edges))
			valid = false;
	free(line);
	gnl_cleanup();
	g = (valid ? parse_graph(verts, edges) : NULL);
	lst_del(&verts);
	lst_del(&edges);
	return (g);
}
