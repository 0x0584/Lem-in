/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   netflow.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: archid- <archid-@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/26 23:45:35 by archid-           #+#    #+#             */
/*   Updated: 2020/12/08 23:35:21 by archid-          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef NETFLOW_H
#define NETFLOW_H

#include "graph.h"

typedef struct s_flow_pair {
	char *vertex;
	size_t unit;
} t_flow_pair;

typedef struct s_flow {
	t_flow_pair *stage;
    size_t size;
    size_t total_units;
} * t_flow;

typedef struct s_network {
    t_lst flows;
    size_t n_units;
    size_t maxflow;
} *t_network;

t_network netflow_setup(t_graph graph, size_t units);
void netflow_pushflow(t_network net);
void netflow_del(t_network *anet);

#endif /* NETFLOW_H */
