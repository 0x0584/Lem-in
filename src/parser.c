/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: archid- <archid-@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/20 01:02:06 by archid-           #+#    #+#             */
/*   Updated: 2020/11/25 01:12:22 by archid-          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

int g_state = 0;
int g_ants = 0;
bool g_verbose = 1;
char *g_error_line;

bool ft_isnumber(char *s) {
	while (*s)
		if (!ft_isdigit(*s++))
			return false;
	return true;
}

bool error_message(char *line) {
	if(g_verbose)
		ft_asprintf(&g_error_line, ": `%s` is not valid", line);
	ft_dprintf(2, "ERROR%s\n", g_error_line);
	ft_strchange(&g_error_line, ft_strdup(""));
	return false;
}

bool valid_vertex_char(char c) {
	return c && c != ' ' && c != '-';
}

bool valid_edge_line(char *line)
{
	int i;

	i = 0;
	if (*line == 'L' || (*line == '#' && *(line + 1) == '#'))
		return false;
	while (valid_vertex_char(line[i]))
		i++;
	if (!line[i] || line[i] == ' ')
		return false;
	i++;
	while (valid_vertex_char(line[i]))
		i++;
	return line[i] == '\0';
}

bool valid_vertex_line(char *line)
{
	int i;
	if (*line == '#' && *(line + 1) == '#')
		return true;
	if (*line == 'L')
		return false;
	i = 0;
	while (valid_vertex_char(line[i]))
		i++;
	if (!line[i] || line[i] == '-')
		return false;
	i++;
	while (line[i] && ft_isdigit(line[i]))
		i++;
	if (line[i] != ' ')
		return false;
	i++;
	while (line[i] && ft_isdigit(line[i]))
		i++;
	return line[i] == '\0';
}

int valid_comment(char *line) {
	return *line == '#' && *(line + 1) != '#';
}

bool valid_line(char *line, t_queue *verts, t_queue *edges)
{
	if (g_state == 0 && ft_isnumber(line))
	{
		if ((g_ants = ft_atoi(line)) <= 0 )
			return error_message("number of ants"), false;
		g_state++;
		return true;
	}
	if (g_state == 1 && valid_vertex_line(line))
	{
		queue_enq(verts, queue_node(line, sizeof(char *), false));
		return true;
	}
	if (valid_edge_line(line))
	{
		g_state++;
		queue_enq(edges, queue_node(line, sizeof(char *), false));
		return true;
	}
	return error_message(line);
}

bool parse_edges(t_graph *g, t_queue *ledges)
{
	t_qnode *walk;
	t_hnode enode;
	t_hnode renode;

	walk = QFIRST(ledges);
	while (walk != QTAIL(ledges))
	{
		if (!edge_alloc(g, walk->blob, &enode, &renode))
			return error_message(walk->blob), false;
		if (!enode.blob)
			ft_printf("edge is NULL");
		if (!renode.blob)
			ft_printf("residual edge is NULL");
		queue_enq(((t_edge *)enode.blob)->src->edges,
				  queue_node(enode.blob, sizeof(t_edge *), false));
		queue_enq(((t_edge *)enode.blob)->src->edges,
				  queue_node(renode.blob, sizeof(t_edge *), false));
		queue_enq(((t_edge *)renode.blob)->src->edges,
				  queue_node(renode.blob, sizeof(t_edge *), false));
		queue_enq(((t_edge *)renode.blob)->src->edges,
				  queue_node(enode.blob, sizeof(t_edge *), false));
		hash_add(g->edges, &enode), hash_add(g->edges, &renode);
		walk = walk->next;
	}
	return true;
}

enum e_tag check_tag(t_graph *g, t_qnode **walk) {
	char *tmp;

	tmp = (*walk)->blob;
	if (!ft_strncmp(tmp, "##", 2))
	{
		*walk = (*walk)->next;
		if (!ft_strcmp(tmp, "##start"))
			return g->source ? tag_error : tag_start;
		else if (!ft_strcmp(tmp, "##end"))
			return  g->sink ? tag_error : tag_end;
	}
	return tag_other;
}

bool parse_vertices(t_graph *g, t_queue *lverts) {
	t_qnode *walk;
	t_hnode hnode;
	enum e_tag status;

	walk = QFIRST(lverts);
	while (walk != QTAIL(lverts))
	{
		status = check_tag(g, &walk);
		if (!status || walk == QTAIL(lverts) ||
			check_tag(g, &walk) != tag_other)
			return error_message("Wrong tag"), false;
		hnode = vertex_alloc(walk->blob);
		if (!hash_add(g->vertices, &hnode))
			return error_message(walk->blob),
				vertex_del(hnode.blob, sizeof(t_vertex *)), false;
		if (status == tag_start)
			g->source = hnode.blob;
		else if (status == tag_end)
			g->sink = hnode.blob;
		g->n_vertices++;
		walk = walk->next;
	}
	return true;
}

t_graph *parse_graph(t_queue *lverts, t_queue *ledges) {
	t_graph *g;

	g = graph_init(hash_init(queue_size(lverts), vertex_del),
				   hash_init(queue_size(ledges), edge_del));
	if (!parse_vertices(g, lverts))
		return graph_free(g), NULL;
	else if (!g->n_vertices)
		return error_message("Empty map"), graph_free(g), NULL;
	else if (!parse_edges(g, ledges))
		return graph_free(g), NULL;
	else if (!g->source || !g->sink)
		return error_message("Source/Sink are not defined"),
			graph_free(g), NULL;
	return g;
}

t_graph *read_graph(void) {
	t_queue *verts;
	t_queue *edges;
	t_graph *g;
	char *line;
	bool valid;

	valid = true;
	verts = queue_init(), edges = queue_init();
	while (valid && gnl(0, &line) && *line)
		if (valid_comment(line))
			free(line);
		else if (!valid_line(line, verts, edges))
		    valid = false;
	free(line);
	g =	(valid ? parse_graph(verts, edges) : NULL);
	queue_del(&verts, queue_blob_free);
	queue_del(&edges, queue_blob_free);
	return g;
}
