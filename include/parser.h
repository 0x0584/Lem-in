#ifndef PARSER_H
#define PARSER_H

#include "graph.h"

enum e_tag { tag_error = 0, tag_other, tag_start, tag_end };

bool ft_isnumber(char *s);
bool error_message(char *line);
bool valid_vertex_char(char c);
bool valid_edge_line(char *line);
bool valid_vertex_line(char *line);
int valid_comment(char *line);
bool valid_line(char *line, t_queue *verts, t_queue *edges);
bool parse_edges(t_graph *g, t_queue *ledges);
enum e_tag check_tag(t_graph *g, t_qnode **walk);
bool parse_vertices(t_graph *g, t_queue *lverts);
t_graph *parse_graph(t_queue *lverts, t_queue *ledges);
t_graph *read_graph(void);

extern int g_state;
extern int g_ants;
extern bool g_verbose;
extern char *g_error_line;

#endif /* PARSER_H */
