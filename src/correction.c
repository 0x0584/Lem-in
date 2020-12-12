/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   correction.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: archid- <archid-@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/26 23:46:38 by archid-           #+#    #+#             */
/*   Updated: 2020/12/12 14:25:32 by archid-          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "netflow.h"

int shortest_path(void *path_a, void *path_b) {
    size_t size_a;
    size_t size_b;

    size_a = lst_size(path_a), size_b = lst_size(path_b);
    if (size_b == size_a)
        return 0;
    else
        return (size_a < size_b) ? 1 : -1;
}

void print_edge_show_residual(void *blob) {
    t_edge e = blob;

    ft_printf("%s-%s%{red_fg}%s%{reset} ", e->src->name, e->dst->name,
              e->residual->seen == M_BELONG_TO_PATH ? "*" : "");
}

void print_path(void *path) {
    t_lstnode walk;
    t_edge e;

    if (!path)
        return;
    ft_printf("------------- \npath length: %zu >> ", lst_size(path));
    walk = lst_front(path);
    while (walk) {
        e = walk->blob;
        ft_printf("%s-%s%{red_fg}%s%{reset} ", e->src->name, e->dst->name,
                  e->residual->seen == M_BELONG_TO_PATH ? "*" : "");
        lst_node_forward(&walk);
    }
    ft_putendl("\n-------------");
}

static bool residual(t_edge e) { return e->residual->seen == M_BELONG_TO_PATH; }

static bool residual_of(t_edge e, t_edge re) { return re == e->residual; }

static void set_edge_fresh(t_edge e) {
    e->seen = M_FRESH;
    e->src->seen = M_FRESH;
    e->dst->seen = M_FRESH;
}

static void set_edge_to_path(t_edge e) {
    e->seen = M_BELONG_TO_PATH;
    e->src->seen = M_BELONG_TO_PATH;
    e->dst->seen = M_BELONG_TO_PATH;
}

static void swap_paths(t_lst path_a, t_lstnode walk_path_a, t_lst path_b,
                       t_lstnode walk_path_b) {
    t_lstnode tmp;
    t_lst front;

    set_edge_to_path(walk_path_a->blob);
    set_edge_to_path(walk_path_a->next->blob);
    set_edge_to_path(walk_path_b->blob);
    set_edge_to_path(walk_path_b->prev->blob);
    front = lst_alloc(blob_keep);
    while (lst_front(path_b) != walk_path_b)
        lst_push_back(front, lst_pop_front(path_b));
    tmp = lst_rear(front);
    while (lst_rear(path_a) != walk_path_a)
        lst_insert_after(front, tmp, lst_pop_back(path_a));
    while (!lst_empty(path_b))
        lst_push_back(path_a, lst_pop_front(path_b));
    while (!lst_empty(front))
        lst_push_back(path_b, lst_pop_front(front));
    lst_del(&front);
}

static void correct_path(t_lst path_a, t_lst paths) {
    t_lstnode walk_a;
    t_lstnode walk_b;
    t_lstnode path_b;
    t_lstnode tmp;
    bool has_residual;

    walk_a = lst_front(path_a);
    while (walk_a) {
        while (walk_a && !residual(walk_a->blob))
            if (!lst_node_forward(&walk_a))
                return;
        path_b = lst_front(paths);
        while (path_b) {
            if (path_b->blob == path_a) {
                lst_node_forward(&path_b);
                continue;
            }
            has_residual = false;
            walk_b = lst_front(path_b->blob);
            while (walk_b && !residual_of(walk_a->blob, walk_b->blob)) {
                has_residual = residual(walk_b->blob) || has_residual;
                lst_node_forward(&walk_b);
            }
            if (walk_b) {
                lst_node_backward(&walk_a), lst_node_forward(&walk_b);
                while (residual_of(walk_a->next->blob, walk_b->prev->blob)) {
                    set_edge_fresh(walk_a->next->blob);
                    set_edge_fresh(walk_b->prev->blob);
                    lst_remove_next(path_a, walk_a);
                    lst_remove_previous(path_b->blob, walk_b);
                }
                swap_paths(path_a, walk_a, path_b->blob, walk_b);
                break;
            }
            tmp = has_residual ? NULL : path_b;
            lst_node_forward(&path_b);
            lst_remove(paths, &tmp);
        }
    }
}

void mark_path(void *lst) {
    t_lstnode walk;

    walk = lst_front(lst);
    while (walk) {
        set_edge_to_path(walk->blob);
        lst_node_forward(&walk);
    }
}

void correct_paths(t_lst paths) {
    t_lstnode walk;
    t_lstnode tmp;
    t_lst paths2;

    /* lst_iter(paths, true, print_path); */
    paths2 = lst_copy_shallow(paths);
    walk = lst_front(paths2);
    while (walk) {
        correct_path(walk->blob, paths2);
        tmp = walk;
        lst_node_forward(&walk);
        lst_remove(paths2, &tmp);
    }
    lst_del(&paths2);
    /* lst_iter(paths, true, print_path); */
}
