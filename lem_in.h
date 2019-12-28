/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lem_in.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: melalj <melalj@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/07 09:09:55 by melalj            #+#    #+#             */
/*   Updated: 2019/12/23 23:39:38 by melalj           ###   ########.fr       */
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

typedef struct	s_cords
{
	int x;
	int y;
}				t_cords;


typedef struct s_node		t_node;
struct s_node					/* vertex list */
{
	char				*name;
	size_t				index;
	t_cords				cords;
	enum e_node_type	type;
	enum e_node_state	seen;
	struct s_edge		*edges;
	struct s_node		*next;
};

typedef struct s_edge		t_edge;
struct s_edge
{
	int				seen;
	int				v_c;
	int				path_n;
	int				drawn;

	struct s_edge	*residual;
	struct s_node	*node_dst;
	struct s_node	*node_src;
	struct s_edge	*next;
};

/* FIXME: move start and sink to solver */
typedef struct s_dvisu		t_dvisu;
typedef struct s_graph		t_graph;
struct s_graph
{
	t_node			*nodes_lst;	 /* FIXME: really implement adjlst, that was
									just a list of all nodes

									NOTE: probably won't need it. */
	t_node			**nodes_ref;
	size_t			n_nodes;
	t_cords			max_c;
	struct s_node	*start;		/* FIXME: rename to source */
	struct s_node	*sink;
	t_dvisu			*data; /*i need it to pass it to bfs as argument*/
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

/* *************************** visu ****************************************/
# include "./visu_lib/SDL_LIB/2.0.10/include/SDL2/SDL.h"
# include "./visu_lib/SDL_IMG_LIB/include/SDL2/SDL_image.h"

struct s_dvisu
{
	SDL_Window		*window;
	SDL_Surface		*s_surface;
	SDL_Texture		*tex;
	int				w_width;
	int				w_height;
	SDL_Renderer	*rend;
	int				path_n;
};

int	visu_init(t_graph *g);
void	visu_quit();
int		graph_draw(t_graph *g);
int		edge_draw(t_graph *g, t_edge *edge, int type);
int		edges_draw(t_graph *g, t_node *node);
int		nodes_draw(t_graph *g, SDL_Rect dstr);
/* *************************** visu end ************************************/

/* *************************** tools ***************************************/
int	map(int val, int *ranges);
int	*range_comp(int in_s, int in_e, int out_s, int out_e);
/* *************************** tools end ***********************************/

/* ***** function prototypes *************************************************/

/* FIXME: re-write functions so that they are generalized */
t_graph						*graph_init(t_node **refs, t_node **nodes, int nodes_c);
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
t_node						**h_table(t_node **refs, t_parse *lines, int nodes_c);
int							edges_fill(t_node **lst_node, t_parse *lines, int nodes_c);

void	node_oneline_dump(t_qnode *e);
#endif
