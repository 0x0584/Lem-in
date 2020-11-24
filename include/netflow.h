#ifndef NETFLOW_H
#define NETFLOW_H

#include "graph.h"


struct s_rewire_handy {
    t_queue		**apath;     /* keeping track of each path */
    t_qnode		**walk_edge; /* moving through the paths one edge at a time */
    int			n_paths;      /* number of paths */
    int			curr;         /* walk index */

    /* edge pair used while walking */
    t_qnode		*e1;
    t_qnode		*e2;
};

#define AS_EDGE(e) ((t_edge *)e->blob)

typedef struct path
{
	char		**nodes;
	int			*ants;
	int			n_ants;
	int			size;
} t_path;

typedef struct s_flow_network {
    struct path *paths;
	int			n_paths;
    int			n_units;
    int			maxflow;
} t_netflow;


t_queue *bfs_find(t_graph *g);
t_queue *re_wire_paths(t_graph *g, t_queue *paths);
void netflow_pushflow(t_netflow *net);
t_netflow *netflow_setup(t_graph *graph, size_t units);
void netflow_del(t_netflow **anet);

/********************************/

bool rewire_done(t_graph *g, struct s_rewire_handy *info);
void walk_edges(t_graph *g, struct s_rewire_handy *info);
void pick_pair(t_graph *g, struct s_rewire_handy *info);
bool prepare_info(struct s_rewire_handy *info, t_queue *paths);
void sort_by_src_name(struct s_rewire_handy *info);
bool has_arrived(t_graph *g, t_edge *e);
t_qnode *next_edge(t_graph *g, t_qnode *edge);
void set_walk_edges(t_queue *paths, struct s_rewire_handy *info);


#endif /* NETFLOW_H */
