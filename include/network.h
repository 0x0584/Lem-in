/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   network.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: archid- <archid-@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/26 23:45:35 by archid-           #+#    #+#             */
/*   Updated: 2020/12/18 13:14:18 by archid-          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef NETWORK_H
# define NETWORK_H

# include "graph.h"

# define NIL_ANT				-1

typedef struct s_flow_pair		t_flow_pair;
typedef struct s_flow			*t_flow;
typedef struct s_network		*t_network;

struct			s_flow_pair
{
	char			*vertex;
	size_t			unit;
};

struct			s_flow
{
	t_flow_pair		*stage;
	size_t			size;
	size_t			total_units;
};

struct			s_network
{
	t_lst			flows;
	size_t			n_units;
};

t_network		network_setup(t_graph graph, size_t units);
void			network_pushflow(t_network net);
void			network_del(t_network *anet);

int				shortest_path(void *path_a, void *path_b);

#endif
