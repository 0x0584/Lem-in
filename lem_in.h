/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lem_in.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: melalj <melalj@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/07 09:09:55 by melalj            #+#    #+#             */
/*   Updated: 2020/11/14 18:22:20 by archid-          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEMIN_H
#define LEMIN_H

#include "libft/libft.h"
#include "src/queue.h"

#define CLONE(foo, size) ft_memcpy(malloc(size), foo, size)
#define MID(l, h) (((l) + (h)) / 2)

/* ***** data types **********************************************************/

typedef struct s_cords {
    int x;
    int y;
} t_cords;

typedef enum e_node_type t_node_type;
enum e_node_type { NODE_DEFAULT = 1, NODE_START, NODE_END };

typedef struct s_node t_node;
struct s_node /* vertex list */
{
    char *name;
    size_t index;
    t_cords cords;
    enum e_node_type type;

    int seen;

    struct s_edge *edges;
    struct s_node *next;
};

typedef union u_rgb {
    int hex;
    char rbg[4];
} t_rgb;

typedef struct s_edge t_edge;
struct s_edge {
    int seen;
    struct s_edge *residual;
    struct s_node *node_dst;
    struct s_node *node_src;
    struct s_edge *next;
};

/* FIXME: move start and sink to solver */
typedef struct s_dvisu t_dvisu;
typedef struct s_graph t_graph;
struct s_graph {
    t_node *nodes_lst;
    t_node **nodes_ref; /* FIXME: remove this, no longer needed! */
    size_t n_nodes;
    t_cords max_c;
    struct s_node *start; /* FIXME: rename to source */
    struct s_node *sink;
    t_dvisu *data; /*i need it to pass it to bfs as argument*/
};

struct s_rewire_handy {
    t_queue **apath;     /* keeping track of each path */
    t_qnode **walk_edge; /* moving through the paths one edge at a time */
    size_t n_paths;      /* number of paths */
    size_t curr;         /* walk index */

    /* edge pair used while walking */
    t_qnode *e1;
    t_qnode *e2;
};

typedef struct s_parse t_parse;
struct s_parse {
    char *line;
    t_node_type prop;
    int type;
    struct s_parse *next;
};

typedef struct s_solver t_solver;
struct s_solver {
    t_graph *g;
    int n_ants;
    t_queue *paths;
};

/* FIXME: implement array of Bytes, and keep track loading level of
 * each flow, so that the sync-in happens at the rear Byte(s),
 * while the sync-out would happen starting at the front Byte. */

#define PATH_MAX_LENGTH (sizeof(unsigned) * 8)
#define AS_EDGE(e) ((t_edge *)e->blob)

/*
 * The way the pomper works
 *
 * at first the path_flow is 0
 */

typedef struct s_flow {
    t_edge **path; /* from source to sink */
    unsigned latency;
    bool cut;
    size_t n_synced;
    /*
       this will hold int indicating the ant going in
       the index of the ant in the ueue is it's destication
    */
    t_queue *ants;
} t_flow;

#define MIN(a, b) ((a) > (b) ? (b) : (a))

typedef struct s_flow_network {
    t_queue *flows;
    t_queue *sync;
    size_t n_units;
    size_t maxflow;
} t_netflow;

/* ***** function prototypes *************************************************/

/* FIXME: re-write functions so that they are generalized */
t_graph *graph_init(t_node **refs, t_node **nodes, int nodes_c);
t_queue *bfs(t_graph *graph);
t_queue *list_shortest_paths(t_graph *graph);

void read_line(int fd, char **line);
t_parse *get_lines(int *nodes_c);
unsigned long hash(unsigned char *str);

/* FIXME: review add_node */
int add_node(t_node **lst_node, t_parse *lines, int nodes_c, int prop,
             t_node **refs);
t_node **h_table(t_node **refs, t_parse *lines, int nodes_c);
int edges_fill(t_node **lst_node, t_parse *lines, int nodes_c);

t_queue *bfs_find(t_graph *g);
t_queue *re_wire_paths(t_graph *g, t_queue *paths);
void netflow_pushflow(t_netflow *net);
t_netflow *netflow_setup(t_graph *graph, size_t units);
void netflow_del(t_netflow **anet);

void lstdel_node(void *c, size_t size);
void helper_lst_alloc(t_node **head, t_node *walk, t_node *node);
t_graph *graph_init(t_node **refs, t_node **nodes, int nodes_c);
void graph_free(t_graph *g);


/********************************/

bool rewire_done(t_graph *g, struct s_rewire_handy *info);
void walk_edges(t_graph *g, struct s_rewire_handy *info);
void pick_pair(t_graph *g, struct s_rewire_handy *info);
bool prepare_info(struct s_rewire_handy *info, t_queue *paths);
void sort_by_node_src_name(struct s_rewire_handy *info);
bool has_arrived(t_graph *g, t_edge *e);
t_qnode *next_edge(t_graph *g, t_qnode *edge);
void set_walk_edges(t_queue *paths, struct s_rewire_handy *info);


#endif
