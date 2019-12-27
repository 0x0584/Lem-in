/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lem_in.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: melalj <melalj@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/07 09:09:55 by melalj            #+#    #+#             */
/*   Updated: 2019/12/27 17:28:58 by archid-          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEMIN_H
# define LEMIN_H

# include "libft/libft.h"
# include "src/queue.h"

# define CLONE(foo, size)					ft_memcpy(malloc(size), foo, size)
# define MID(l, h)							(((l) + (h)) / 2)

/* ***** data types **********************************************************/

typedef enum e_node_type	t_node_type;
enum e_node_type
{
	NODE_DEFAULT = 1,
	NODE_START, NODE_END
};

typedef enum e_node_state	t_node_state;
enum e_node_state
{
	NODE_FRESH = 0,
	NODE_SEEN, NODE_TAKEN
}				seen;

typedef struct s_node		t_node;
struct s_node					/* vertex list */
{
	char				*name;
	size_t				index;
	enum e_node_type	type;
	enum e_node_state	seen;
	struct s_edge		*edges;
	struct s_node		*next;
};

typedef struct s_edge		t_edge;
struct s_edge
{
	int				seen;

	struct s_edge	*residual;
	struct s_node	*node_dst;
	struct s_node	*node_src;
	struct s_edge	*next;
};

/* FIXME: move start and sink to solver */
typedef struct s_graph		t_graph;
struct s_graph
{
	t_node			*nodes_lst;	 /* FIXME: really implement adjlst, that was
									just a list of all nodes

									NOTE: probably won't need it. */
	t_node			**nodes_ref;
	size_t			n_nodes;
	struct s_node	*start;		/* FIXME: rename to source */
	struct s_node	*sink;
};

typedef struct s_parse		t_parse;
struct s_parse
{
	char			*line;
	t_node_type 	prop;
	int				type;
	struct s_parse	*next;
};

typedef struct s_solver		t_solver;
struct s_solver
{
	t_graph			*g;
	int				n_ants;
	t_queue			*paths;
};

# define PATH_MAX_LENGTH			(sizeof(unsigned) * 8)
# define AS_EDGE(e)					((t_edge *)e->blob)

/*
 * The way the pomper works
 *
 * at first the path_flow is 0
 */

typedef struct				s_flow
{
	t_edge			**path;
	unsigned		latency;
	/* This represents the ants as bits and each next
	 *
	 * each ant going is a (1 & !blocked)
	 */

	/* FIXME: set a bigger flow current: (unsigned char *) */
	unsigned		current;
	unsigned		cmask;
	bool			cut;
	size_t			n_arrived;
}							t_flow;

# define MIN(a, b)								((a) > (b) ? (b) : (a))

typedef struct				s_flow_network
{
	t_queue			*flows;
	t_queue			*sync;
	size_t			n_units;
	size_t			maxflow;
}							t_netflow;

/* ***** function prototypes *************************************************/



/* FIXME: re-write functions so that they are generalized */
t_graph						*graph_init(t_node **refs, t_node **nodes,
											int nodes_c);
t_queue						*bfs(t_graph *graph);
void						node_info(t_node *node);
void						node_dump(t_qnode *e);
t_queue						*list_shortest_paths(t_graph *graph);

void						read_line(int fd, char **line);
t_parse						*get_lines(int *nodes_c);
unsigned long				hash(unsigned char *str);

/* FIXME: review add_node */
int							add_node(t_node **lst_node, t_parse *lines,
										int nodes_c, int prop,
										t_node **refs);
t_node						**h_table(t_node **refs, t_parse *lines,
										int nodes_c);
int							edges_fill(t_node **lst_node, t_parse *lines,
										int nodes_c);

void	node_oneline_dump(t_qnode *e);

void	edge_dump(t_qnode *e);

t_queue						*bfs_find(t_graph *g);
t_queue						*re_wire_paths(t_graph *g, t_queue *paths);
void						netflow_pushflow(t_netflow *net);
t_netflow					*netflow_setup(t_graph *graph, size_t units);
void						netflow_del(t_netflow **anet);

#endif
