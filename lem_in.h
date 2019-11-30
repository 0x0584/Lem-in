/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lem_in.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: melalj <melalj@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/07 09:09:55 by melalj            #+#    #+#             */
/*   Updated: 2019/11/30 22:33:41 by archid-          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEMIN_H
# define LEMIN_H

# include "libft/libft.h"
#include <stdbool.h>

/*
** types
*/

typedef enum e_node_type	t_node_type;
typedef struct s_node		t_node;
typedef struct s_edges		t_edges;
typedef struct s_parse		t_parse;
typedef struct s_graph		t_graph;

enum e_node_type
{
	NODE_DEFAULT = 1,
	NODE_START, NODE_END
};


struct s_node
{
	char			*name;
	t_node_type 	type;
	bool			seen;
	struct s_edges	*edges;
	struct s_node	*next;
};

struct s_edges
{
	struct s_node	*node_dst;
	struct s_node	*node_src;
	int i;
	struct s_edges	*next;
};

struct s_parse
{
	char			*line;
	t_node_type 	prop;
	int				type;
	struct s_parse	*next;
};

struct s_graph
{
	struct s_node	*start;
	struct s_node	*end;
	t_node			*adjlst;
	size_t			n_nodes;
};

t_graph						*graph_init(t_node **nodes, int nodes_c);
bool						bfs(t_graph *graph, t_node *start, t_node *sink);

void						read_line(int fd, char **line);
t_parse						*get_lines(int *nodes_c);
unsigned long				hash(unsigned char *str);
int							add_node(t_node **lst_node, t_parse *lines, int nodes_c, int prop);
t_node						**h_table(t_parse *lines, int nodes_c);
int							edges_fill(t_node **lst_node, t_parse *lines, int nodes_c);

#endif
