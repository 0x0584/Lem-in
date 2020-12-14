/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lst.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: archid- <archid-@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/29 17:12:11 by archid-           #+#    #+#             */
/*   Updated: 2020/12/14 17:09:37 by archid-          ###   ########.fr       */
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

void lst_node_del(t_lst lst, t_lstnode *anode) {
    lst_node_del_with(anode, lst->del);
}

void lst_node_free(t_lst lst, t_lstnode anode) { lst_node_del(lst, &anode); }

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

void *lst_front_blob(t_lst lst) {
    t_lstnode front;

    if (!(front = lst_front(lst)))
        return NULL;
    return front->blob;
}

void *lst_rear_blob(t_lst lst) {
    t_lstnode rear;

    if (!(rear = lst_rear(lst)))
        return NULL;
    return rear->blob;
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

void lst_iter_arg(t_lst lst, bool front, void *arg,
                  void (*apply_arg)(void *blob, void *arg)) {
    t_lstnode walk;

    if (lst_empty(lst))
        return;
    walk = front ? lst->head->next : lst->tail->prev;
    while (walk != (front ? lst->tail : lst->head)) {
        apply_arg(walk->blob, arg);
        walk = front ? walk->next : walk->prev;
    }
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

void lst_iteri_arg(t_lst lst, bool front, void *params,
                   void (*index_apply_arg)(void *blob, size_t index,
                                           void *arg)) {
    t_lstnode walk;
    size_t i;

    if (lst_empty(lst))
        return;
    i = 0;
    walk = front ? lst->head->next : lst->tail->prev;
    while (walk != (front ? lst->tail : lst->head)) {
        index_apply_arg(walk->blob, i++, params);
        walk = front ? walk->next : walk->prev;
    }
}

void lst_iteri(t_lst lst, bool front,
               void (*index_apply)(void *blob, size_t index)) {
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

bool lst_node_forward(t_lstnode *anode) {
    t_lstnode next;

    if (!anode || !*anode)
        return false;
    next = (*anode)->next;
    return (*anode = next->next ? next : NULL);
}

bool lst_node_backward(t_lstnode *anode) {
    t_lstnode prev;

    if (!anode || !*anode)
        return false;
    prev = (*anode)->prev;
    return (*anode = prev->prev ? prev : NULL);
}

t_lst lst_insert_at(t_lst lst, t_lstnode at, t_lstnode node) {
    if (!lst || !at || !node || at == node)
        return lst;
    else if (at == lst->head)
        return lst_push_front(lst, node);
    else if (at == lst->tail)
        return lst_push_back(lst, node);
    node->prev = at->prev;
    at->prev->next = node;
    at->prev = node;
    node->next = at;
    lst->size++;
    return lst;
}

t_lst lst_insert_after(t_lst lst, t_lstnode before, t_lstnode node) {
    if (!lst || !before || !node)
        return lst;
    else if (before == lst->tail)
        return lst_push_back(lst, node);
    else
        return lst_insert_at(lst, before->next, node);
}

t_lst lst_insert_before(t_lst lst, t_lstnode after, t_lstnode node) {
    if (!lst || !after || !node)
        return lst;
    else if (after == lst->head)
        return lst_push_front(lst, node);
    else
        return lst_insert_at(lst, after->prev, node);
}

void lst_remove(t_lst lst, t_lstnode *node) {
    t_lstnode tmp;

    if ((tmp = lst_extract(lst, *node))) {
        lst_node_del(lst, &tmp);
        *node = NULL;
    }
}

void lst_remove_next(t_lst lst, t_lstnode node) {
    t_lstnode tmp;

    if (node) {
        tmp = node->next;
        lst_remove(lst, &tmp);
    }
}

void lst_remove_previous(t_lst lst, t_lstnode node) {
    t_lstnode tmp;

    if (node) {
        tmp = node->prev;
        lst_remove(lst, &tmp);
    }
}

t_lstnode lst_extract(t_lst lst, t_lstnode node) {
    if (!lst || !node || lst->head == node || lst->tail == node)
        return NULL;
    node->prev->next = node->next;
    node->next->prev = node->prev;
    node->next = NULL;
    node->prev = NULL;
    lst->size--;
    return node;
}

void *lst_extract_blob(t_lst lst, t_lstnode *node) {
    t_lstnode tmp;
    void *blob;

    if (!node || !*node || !(tmp = lst_extract(lst, *node)))
        return NULL;
    blob = tmp->blob;
    lst_node_del_with(&tmp, blob_keep);
    *node = NULL;
    return blob;
}

t_lst lst_insertion_sort(t_lst lst, int (*cmp)(void *, void *)) {
    t_lstnode walk;
    t_lstnode front;
    t_lstnode pivot;

    if (lst_size(lst) < 2)
        return lst;
    walk = lst_front(lst);
    while (walk) {
        front = lst_front(lst);
        pivot = walk, lst_node_forward(&walk);
        while (front != pivot) {
            if (cmp(front->blob, pivot->blob) > 0)
                lst_node_forward(&front);
            else {
                lst_insert_at(lst, front, lst_extract(lst, pivot));
                break;
            }
        }
    }
    return lst;
}

t_lst lst_clear(t_lst lst) {
    t_lstnode tmp;

    while (!lst_empty(lst)) {
        tmp = lst_pop_back(lst);
        lst_node_del(lst, &tmp);
    }
    return lst;
}

t_lst lst_copy(t_lst lst, void *(*func_copy)(void *), void (*del)(void *)) {
    t_lst copy;
    t_lstnode walk;

    if (!(walk = lst_front(lst)) || !func_copy)
        return NULL;
    copy = lst_alloc(del);
    while (walk) {
        lst_push_back_blob(copy, func_copy(walk->blob), walk->size, false);
        lst_node_forward(&walk);
    }
    return copy;
}

static void *blob_identity(void *blob) { return blob; }

t_lst lst_copy_shallow(t_lst lst) {
    return lst_copy(lst, blob_identity, blob_keep);
}

t_lst lst_copy_deep(t_lst lst, void *(copy)(void *)) {
    return lst_copy(lst, copy, blob_free);
}

t_lst lst_shrink(t_lst lst, bool from_head, size_t size) {
	t_lstnode (*func)(t_lst);
	t_lstnode tmp;

	if (lst_size(lst) <= size)
		return lst;
	func = from_head ? lst_pop_front : lst_pop_back;
	while (lst_size(lst) > size)
		tmp = func(lst), lst_node_del(lst, &tmp);
	return lst;
}
