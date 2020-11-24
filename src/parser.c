#include "parser.h"

int g_state = 0;
int g_ants = 0;
bool g_verbose = 0;
char *g_error_line;

bool ft_isnumber(char *s) {
	while (*s)
		if (!ft_isdigit(*s++))
			return false;
	return true;
}

bool error_message(char *line) {
	if(g_verbose)
		ft_asprintf(&g_error_line, ": `%s` is not valid\n", line);
	ft_printf("ERROR%s\n", g_error_line);
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
	if (*line && g_state == 0 && ft_isnumber(line))
	{
		g_ants = ft_atoi(line);
		g_state++;
		return true;
	}
	if (*line && g_state == 1 && valid_vertex_line(line))
	{
		queue_enq(verts, queue_node(line, sizeof(char *), false));
		return true;
	}
	if (*line && valid_edge_line(line))
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
			return edge_del(enode.blob, sizeof(t_edge)),
				edge_del(renode.blob, sizeof(t_edge)), false;
		hash_add(g->edges, &enode);
		hash_add(g->edges, &renode);
		queue_enq(((t_edge *)enode.blob)->src->edges,
				  queue_node(enode.blob, sizeof(t_edge *), false));
		queue_enq(((t_edge *)enode.blob)->src->edges,
				  queue_node(renode.blob, sizeof(t_edge *), false));
		queue_enq(((t_edge *)renode.blob)->src->edges,
				  queue_node(renode.blob, sizeof(t_edge *), false));
		queue_enq(((t_edge *)renode.blob)->src->edges,
				  queue_node(enode.blob, sizeof(t_edge *), false));
	}
	return true;
}

int check_tag(t_graph *g, t_qnode **walk) {
	if (!ft_strncmp((*walk)->blob, "##", 2))
	{
		if (!ft_strcmp((*walk)->blob, "##start"))
		{
			if (g->source)
				return 0;
			else
				return 1;
		}
		else if (!ft_strcmp((*walk)->blob, "##end"))
		{
			if (g->sink)
				return 0;
			else
				return 2;
		}
		*walk = (*walk)->next;
	}
	return 1;
}

bool parse_vertices(t_graph *g, t_queue *lverts) {
	t_qnode *walk;
	t_hnode hnode;
	int status;

	walk = QFIRST(lverts);
	while (walk != QTAIL(lverts))
	{
		status = check_tag(g, &walk);
		if (!status)
			return false;
		hnode = vertex_alloc(walk->blob);
		if (!hash_add(g->vertices, &hnode))
			return vertex_del(hnode.blob, sizeof(t_vertex *)), false;
		if (status == 1)
			g->source = hnode.blob;
		else if (status == 2)
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
	if (!parse_vertices(g, lverts) || !parse_edges(g, ledges))
		return graph_free(g), NULL;
	return g;
}

t_graph *read_graph(void) {
	t_queue *verts;
	t_queue *edges;
	char *line;
	bool valid;

	valid = true;
	verts = queue_init(), edges = queue_init();
	while (valid && gnl(1, &line))
		if (valid_comment(line))
			free(line);
		else if (!valid_line(line, verts, edges))
		    valid = false;
	free(line);
	queue_del(&verts, queue_blob_free);
	queue_del(&edges, queue_blob_free);
	return (valid ? parse_graph(verts, edges) : NULL);
}
