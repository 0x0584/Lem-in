/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lst.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: archid- <archid-@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/29 16:34:27 by archid-           #+#    #+#             */
/*   Updated: 2020/12/08 23:03:58 by archid-          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LST_H
#define LST_H

#include "libft.h"

typedef struct s_lst_node {
    void *blob;
    size_t size;
    struct s_lst_node *next;
    struct s_lst_node *prev;
} * t_lstnode;

typedef struct s_lst {
    t_lstnode head;
    t_lstnode tail;
    size_t size;
    void (*del)(void *blob);
} * t_lst;

void blob_free(void *blob);
void blob_keep(void *blob);

t_lstnode lst_node(void *blob, size_t size, bool alloc);
void lst_node_del(t_lst lst, t_lstnode *anode);
void lst_node_del_with(t_lstnode *anode, void (*del)(void *blob));

t_lst lst_alloc(void (*del)(void *blob));
void lst_free(void *lst);
void lst_del(t_lst *alst);
void lst_del_with(t_lst *alst, void (*del)(void *blob));

size_t lst_size(t_lst lst);
bool lst_empty(t_lst lst);
t_lstnode lst_head(t_lst lst);
t_lstnode lst_tail(t_lst lst);
t_lstnode lst_front(t_lst lst);
t_lstnode lst_rear(t_lst lst);

t_lst lst_push_front(t_lst lst, t_lstnode node);
t_lst lst_push_back(t_lst lst, t_lstnode node);
t_lstnode lst_pop_back(t_lst lst);
t_lstnode lst_pop_front(t_lst lst);

t_lst lst_push_front_blob(t_lst lst, void *blob, size_t size, bool alloc);
t_lst lst_push_back_blob(t_lst lst, void *blob, size_t size, bool alloc);
void *lst_pop_back_blob(t_lst lst);
void *lst_pop_front_blob(t_lst lst);

void lst_iter(t_lst lst, bool front, void (*apply)(void *blob));
void lst_iteri(t_lst lst, bool front,
               void (*index_apply)(void *blob, size_t index));

void lst_node_forward(t_lstnode *anode);
void lst_node_backward(t_lstnode *anode);

#endif
