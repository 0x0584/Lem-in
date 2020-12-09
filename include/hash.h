/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hash.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: archid- <archid-@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/26 23:45:23 by archid-           #+#    #+#             */
/*   Updated: 2020/12/08 17:28:17 by archid-          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HASH_H
#define HASH_H

#include <stdlib.h>

#include "lst.h"

typedef struct hash_node {
    char *key;
    void *blob;
} t_hashnode;

typedef struct s_hash {
    t_lst *array;
    size_t size;
	void (*del)(void *);
} * t_hash;

t_hash hash_alloc(size_t size, void (*del)(void *));

bool hash_add(t_hash h, char *key, void *blob);
void hash_del(t_hash *h);
void *hash_get(t_hash h, char *key, void *val_default);
void hash_default_del(void *ptr);

#endif /* HASH_H */
