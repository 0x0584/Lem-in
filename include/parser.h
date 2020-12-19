/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: archid- <archid-@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/26 23:45:40 by archid-           #+#    #+#             */
/*   Updated: 2020/12/19 13:10:50 by archid-          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

# include "graph.h"

# define JSON_OUTPUT "out.json"

enum			e_tag
{
	tag_error = 0,
	tag_other,
	tag_start,
	tag_end
};

void			parse_args(int argc, char *argv[]);

bool			ft_isnumber(char *s);
bool			error_message(char *line);
bool			valid_vertex_char(char c);
bool			valid_edge_line(char *line);
bool			valid_vertex_line(char *line);
int				valid_comment(char *line);
bool			valid_line(char *line, t_lst verts, t_lst edges);
bool			parse_edges(t_graph g, t_lst ledges);
enum e_tag		check_tag(t_graph g, t_lstnode *walk);
bool			parse_vertices(t_graph g, t_lst lverts);
t_graph			parse_graph(t_lst lverts, t_lst ledges);
t_graph			read_graph(void);

void			print_map(t_lst lverts, t_lst ledges);

extern int		g_ants;
extern char		*g_error_line;
extern int		g_state;

extern bool		g_verbose;
extern bool		g_visualize;
extern bool		g_output_maxflow;
extern bool		g_output_info;
extern bool		g_output_ant_lines;
extern bool		g_json_output;

#endif
