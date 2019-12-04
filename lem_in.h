/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lem_in.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: melalj <melalj@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/07 09:09:55 by melalj            #+#    #+#             */
/*   Updated: 2019/12/04 06:30:09 by archid-          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEMIN_H
# define LEMIN_H

# include "libft/libft.h"
# include "src/queue.h"

#include <stdbool.h>

/*
** types
*/

typedef enum e_node_type	t_node_type;
typedef enum e_node_state	t_node_state;
typedef struct s_node		t_node;

typedef struct s_edges		t_edges;
typedef struct s_parse		t_parse;
typedef struct s_graph		t_graph;

enum e_node_type
{
	NODE_DEFAULT = 1,
	NODE_START, NODE_END
};

enum e_node_state
{
	NODE_FRESH = 0,
	NODE_SEEN,
	NODE_TAKEN
};

struct s_node
{
	struct s_edges	*edges;
	struct s_node	*next;

	size_t			index;
	char			*name;
	t_node_type 	type;
	t_node_state	seen;
};

struct s_edges
{
	struct s_node	*node_dst;
	struct s_node	*node_src;
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
	struct s_node	*sink;
	t_node			*adjlst;
	t_node			**refs;		/* array where each node is indexed based on interance.
								 * however if the # of nodes of big, this would consume
								 * large space. used to find parent of each node while
								 * traversing the graph using BFS() */
	size_t			n_nodes;
};

t_graph						*graph_init(t_node **refs, t_node **nodes, int nodes_c);
t_queue						*bfs(t_graph *graph);
void						node_info(t_node *node);
void						node_dump(t_qnode *e);
t_queue						*list_shortest_paths(t_graph *graph);

void						read_line(int fd, char **line);
t_parse						*get_lines(int *nodes_c);
unsigned long				hash(unsigned char *str);

/* FIXME: review add_node */
int							add_node(t_node **lst_node, t_parse *lines, int nodes_c, int prop,
										t_node **refs);
t_node						**h_table(t_node **refs, t_parse *lines, int nodes_c);
int							edges_fill(t_node **lst_node, t_parse *lines, int nodes_c);

#endif
