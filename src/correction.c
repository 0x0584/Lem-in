/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   correction.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: archid- <archid-@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/26 23:46:38 by archid-           #+#    #+#             */
/*   Updated: 2020/12/12 13:41:24 by archid-          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "netflow.h"

int shortest_path(void *path_a, void *path_b) {
    size_t size_a;
    size_t size_b;

    size_a = lst_size(path_a), size_b = lst_size(path_b);
    if (size_b == size_a)
        return 0;
    else if (size_b < size_a)
        return -1;
    else
        return 1;
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

static void set_edge_fresh(t_edge e) { e->seen = M_FRESH; }

static void swap_paths(t_lst path_a, t_lstnode walk_path_a, t_lst path_b,
                       t_lstnode walk_path_b) {
    t_lstnode tmp;
    t_lst front;

    ft_putendl("/////////////////////////////////////////");
    front = lst_alloc(blob_keep);

    while (lst_front(path_b) != walk_path_b)
        lst_push_back(front, lst_pop_front(path_b));

    {
        ft_printf(" %{red_fg}>>> ----- front path --------%{reset}\n");
        print_path(front);
    }

    tmp = lst_rear(front);
    while (lst_rear(path_a) != walk_path_a)
        lst_insert_after(front, tmp, lst_pop_back(path_a));

    {
        ft_printf(" %{red_fg}>>> ----- front path 2 --------%{reset}\n");
        print_path(front);
    }

    {
        ft_printf(" %{red_fg}>>> -----  path a --------%{reset}\n");
        print_path(path_a);
    }

    {
        ft_printf(" %{red_fg}>>> -----  path b --------%{reset}\n");
        print_path(path_b);
    }
    ft_putendl("/////////////////////////////////////////");

    while (!lst_empty(path_b))
        lst_push_back(path_a, lst_pop_front(path_b));

    while (!lst_empty(front))
        lst_push_back(path_b, lst_pop_front(front));

    lst_del(&front);

    {
        ft_printf(" %{red_fg}>>> -----  PATH A --------%{reset}\n");
        print_path(path_a);
    }

    {
        ft_printf(" %{red_fg}>>> -----  PATH B --------%{reset}\n");
        print_path(path_b);
    }

    /* path_a->next->prev = path_b->prev; */
    /* path_b->prev->next = path_a->next; */
    /* path_a->next = path_b; */
    /* path_b->prev = path_a; */
}

static void correct_path(t_lst path_a, t_lst paths) {
    t_lstnode walk_a;
    t_lstnode walk_b;
    t_lstnode path_b;
    t_lstnode tmp;
    bool has_residual;

    {
        ft_printf(" %{red_fg}>>> ----- in path --------%{reset}\n");
        print_path(path_a);
    }
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
            {
                ft_printf(
                    " %{yellow_fg}>>> ----- current path --------%{reset}\n");
                print_path(path_b->blob);
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
                    {
                        ft_printf(" %{red_fg}>>> ----- path_a "
                                  "--------%{reset}\n");
                        print_path(path_a);
                        ft_printf(" %{yellow_fg}>>> ----- path_b "
                                  "--------%{reset}\n");
                        print_path(path_b->blob);
                        ft_putendl(" ######### ");
                    }
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

void correct_paths(t_lst paths) {
    t_lstnode walk;
    t_lstnode tmp;
    t_lst paths2;

    {
        ft_printf("\n%{green_fg}paths before%{reset}\n");
        lst_iter(paths, true, print_path);
    }

    paths2 = lst_insertion_sort(lst_copy_shallow(paths), shortest_path);
    walk = lst_front(paths2);
    while (walk) {
        correct_path(walk->blob, paths2);
        tmp = walk;
        lst_node_forward(&walk);
        lst_remove(paths2, &tmp);
    }

    lst_del(&paths2);
    {
        ft_printf(" %{cyan_fg}>>> ----- original paths --------%{reset}\n");
        lst_iter(paths, true, print_path);
        ft_printf(" %{magenta_fg}>>> ----- paths copy --------%{reset}\n");
        lst_iter(paths2, true, print_path);
        ft_putendl(" >>> -------- -----");
        assert_paths_correct(paths);
    }
}
