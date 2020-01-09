/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lem_in.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: melalj <melalj@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/07 09:09:55 by melalj            #+#    #+#             */
/*   Updated: 2020/01/09 05:18:23 by archid-          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEMIN_H
# define LEMIN_H

#define USE_VISU

# include "libft/libft.h"
# include "src/queue.h"

# ifdef USE_VISU
#  include "SDL2/SDL.h"
#  include "SDL2/SDL_image.h"
#  define VISU_WIN_WIDTH					1280
#  define VISU_WIN_HEIGHT					950
# endif

# define CLONE(foo, size)					ft_memcpy(malloc(size), foo, size)
# define MID(l, h)							(((l) + (h)) / 2)
# define MIN(a, b)							((a) > (b) ? (b) : (a))

// #define DEBUG

typedef union				u_rgb_color
{
	int				hex;
	char			rbg[4];
}							t_rgb;

typedef struct				s_2d_coordinates
{
	int				x;
	int				y;
}							t_cords;

typedef enum				e_graph_node_type
{
	NODE_DEFAULT = 1,
	NODE_START, NODE_END
}							t_node_type;

typedef struct				s_parse
{
	char			*line;
	t_node_type 	prop;
	int				type;
	struct s_parse	*next;
}							t_parse;

typedef struct				s_node
{
	int				seen;
	char			*name;
	size_t			index;
	t_cords			cords;
	t_node_type		type;
	struct s_edge	*edges;
	struct s_node	*next;
}							t_node;

typedef struct				s_edge
{
	int				seen;
	struct s_edge	*residual;
	struct s_node	*node_dst;
	struct s_node	*node_src;
	struct s_edge	*next;
# ifdef USE_VISU
	int				v_c;
	int				path_n;
	int				drawn;
	t_rgb			color;
# endif
}							t_edge;

typedef struct				s_graph
{
	t_node			*nodes_lst;
	size_t			n_nodes;
	struct s_node	*source;
	struct s_node	*sink;
}							t_graph;

typedef struct				s_flow
{
	t_edge			**path;
	unsigned		latency;
	bool			cut;
	size_t			n_synced;
	t_queue			*ants;
}							t_flow;

typedef struct				s_flow_network
{
	t_queue			*flows;
	t_queue			*sync;
	size_t			n_units;
	size_t			maxflow;
}							t_netflow;

# ifdef USE_VISU
typedef struct				s_data_visualizer
{
	SDL_Window		*window;
	SDL_Surface		*s_surface;
	SDL_Texture		*tex;
	SDL_Renderer	*rend;
	int				f;			/* ?? */
	int				path_n;
	int				w_width;
	int				w_height;
	t_cords			max_c;
}							t_dvisu;

void						set_visu_data(t_dvisu *v);
t_dvisu						*get_visu_data(void);
int							visu_init(t_graph *g);
void						visu_quit(void);
SDL_Texture					*get_imagetex(t_dvisu *data, char *image_path);

int							map(int val, int *ranges); /* ?? */
int							*range_comp(int in_s, int in_e, int out_s,
											int out_e); /* ?? */

int							graph_draw(t_graph *g);
int							edge_draw(t_edge *edge, int type);
int							edges_draw(t_node *node);
# endif

void						read_line(int fd, char **line);
t_parse						*get_lines(int *nodes_c);
unsigned long				hash(unsigned char *str);
int							add_node(t_node **lst_node, t_parse *lines,
									 int nodes_c, int prop);
t_node						**h_table(t_parse *lines, int nodes_c);
int							edges_fill(t_node **lst_node, t_parse *lines,
									   int nodes_c);

t_graph						*graph_init(t_node **nodes, int n_nodes);
void						graph_dump(t_graph *g);
void						graph_free(t_graph *g);
t_queue						*bfs(t_graph *graph);
t_queue						*bfs_find(t_graph *g);
t_queue						*re_wire_paths(t_graph *g, t_queue *paths);

void						netflow_pushflow(t_netflow *net);
t_netflow					*netflow_setup(t_graph *graph, size_t units);
void						netflow_del(t_netflow **anet);

void						lstdel_node(void *c, size_t size);
void						helper_lst_alloc(t_node **head, t_node *walk,
											 t_node *node);
void						node_full_dump(t_qnode *e);
void						node_oneline_dump(t_qnode *e);
void						edge_oneline_dump(t_qnode *e);
void						node_info(t_node *node);
void						node_dump(t_qnode *e);
void						edge_dump(t_qnode *e);

#endif
