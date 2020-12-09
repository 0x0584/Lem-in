/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lst.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: archid- <archid-@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/29 17:12:11 by archid-           #+#    #+#             */
/*   Updated: 2020/12/09 01:49:12 by archid-          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lst.h"

t_lstnode lst_node(void *blob, size_t size, bool alloc) {
    t_lstnode node;

    node = malloc(sizeof(struct s_lst_node));
    node->size = blob ? size : 0;
    if (alloc)
        node->blob = blob ? ft_memcpy(malloc(size), blob, size) : NULL;
    else
        node->blob = blob;
    node->next = NULL;
    node->prev = NULL;
    return (node);
}

void blob_free(void *blob) { free(blob); }

void blob_keep(void *blob) { (void)blob; }

void lst_node_del_with(t_lstnode *anode, void (*del)(void *blob)) {
    if (!anode || !*anode)
        return;
    (*del)((*anode)->blob);
    free(*anode);
    *anode = NULL;
}

void lst_node_del(t_lst q, t_lstnode *anode) {
    lst_node_del_with(anode, q->del);
}

t_lst lst_alloc(void (*del)(void *blob)) {
    t_lst lst;

    lst = malloc(sizeof(struct s_lst));
    lst->head = lst_node(NULL, 0, false);
    lst->tail = lst_node(NULL, 0, false);
	lst->head->prev = NULL;
    lst->head->next = lst->tail;
    lst->tail->prev = lst->head;
	lst->tail->next = NULL;
    lst->size = 0;
    lst->del = del;
    return (lst);
}

void lst_del_with(t_lst *alst, void (*del)(void *blob)) {
    t_lstnode walk;
    t_lstnode tmp;
	t_lst lst;

    if (!del || !alst || !(lst = *alst))
        return;
    walk = lst->head->next;
    while (walk != lst->tail) {
        tmp = walk;
        walk = walk->next;
        lst_node_del_with(&tmp, del);
    }
    lst_node_del_with(&lst->head, del);
    lst_node_del_with(&lst->tail, del);
    free(lst);
    *alst = NULL;
}

void lst_del(t_lst *alst) {
    if (alst && *alst)
		lst_del_with(alst, (*alst)->del);
}


void lst_free(void *lst) {
	t_lst tmp;

    if ((tmp = lst))
		lst_del(&tmp);
}

size_t lst_size(t_lst lst) { return lst ? lst->size : 0; }

bool lst_empty(t_lst lst) { return !lst || lst->head->next == lst->tail; }

t_lstnode lst_head(t_lst lst) { return lst ? lst->head : NULL; }
t_lstnode lst_tail(t_lst lst) { return lst ? lst->tail : NULL; }

t_lstnode lst_front(t_lst lst) {
    return lst_empty(lst) ? NULL : lst_head(lst)->next;
}

t_lstnode lst_rear(t_lst lst) {
    return lst_empty(lst) ? NULL : lst_tail(lst)->prev;
}

t_lst lst_push_front(t_lst lst, t_lstnode node) {
    if (!lst || !node)
        return lst;
    node->next = lst->head->next;
    lst->head->next->prev = node;
    node->prev = lst->head;
    lst->head->next = node;
    lst->size++;
    return lst;
}

t_lst lst_push_front_blob(t_lst lst, void *blob, size_t size, bool alloc) {
    if (!lst)
        return NULL;
    else
        return lst_push_front(lst, lst_node(blob, size, alloc));
}

t_lst lst_push_back(t_lst lst, t_lstnode node) {
    if (!lst || !node)
        return NULL;
    lst->tail->prev->next = node;
    node->prev = lst->tail->prev;
    node->next = lst->tail;
    lst->tail->prev = node;
    lst->size++;
    return lst;
}

t_lst lst_push_back_blob(t_lst lst, void *blob, size_t size, bool alloc) {
    if (!lst)
        return NULL;
    else
        return lst_push_back(lst, lst_node(blob, size, alloc));
}

t_lstnode lst_pop_back(t_lst lst) {
    t_lstnode node;

    if (lst_empty(lst))
        return (NULL);
    node = lst->tail->prev;
    lst->tail->prev = node->prev;
    node->prev->next = lst->tail;
    lst->size--;
    return (node);
}

void *lst_pop_back_blob(t_lst lst) {
    t_lstnode tmp;
    void *blob;

    if (lst_empty(lst))
        return NULL;
    tmp = lst_pop_back(lst);
    blob = tmp->blob;
    lst_node_del_with(&tmp, blob_keep);
    return blob;
}

t_lstnode lst_pop_front(t_lst lst) {
    t_lstnode node;

    if (lst_empty(lst))
        return (NULL);
    node = lst->head->next;
    node->next->prev = lst->head;
    lst->head->next = node->next;
    lst->size--;
    return (node);
}

void *lst_pop_front_blob(t_lst lst) {
    t_lstnode tmp;
    void *blob;

    if (lst_empty(lst))
        return NULL;
    tmp = lst_pop_front(lst);
    blob = tmp->blob;
    lst_node_del_with(&tmp, blob_keep);
    return blob;
}

void lst_iter(t_lst lst, bool front, void (*apply)(void *blob)) {
    t_lstnode walk;

    if (lst_empty(lst))
        return;
    walk = front ? lst->head->next : lst->tail->prev;
    while (walk != (front ? lst->tail : lst->head)) {
        apply(walk->blob);
        walk = front ? walk->next : walk->prev;
    }
}

void lst_iteri(t_lst lst, bool front,
               void (*index_apply)(void *blob, size_t size)) {
    t_lstnode walk;
    size_t i;

    if (lst_empty(lst))
        return;
    i = 0;
    walk = front ? lst->head->next : lst->tail->prev;
    while (walk != (front ? lst->tail : lst->head)) {
        index_apply(walk->blob, i++);
        walk = front ? walk->next : walk->prev;
    }
}

void lst_node_forward(t_lstnode *anode) {
    t_lstnode next;

    if (!anode || !*anode)
        return;
    next = (*anode)->next;
    *anode = next->next ? next : NULL;
}

void lst_node_backward(t_lstnode *anode) {
    t_lstnode prev;

    if (!anode || !*anode)
        return;
    prev = (*anode)->prev;
    *anode = prev->prev ? prev : NULL;
}
