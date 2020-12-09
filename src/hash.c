/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hash.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: archid- <archid-@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/26 23:44:57 by archid-           #+#    #+#             */
/*   Updated: 2020/12/09 01:56:42 by archid-          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "hash.h"

size_t compute_hash(char *s, size_t size, size_t sum, size_t mod) {
    size_t i;
    size_t j;
    size_t tmp;
    size_t mul;

    i = 0;
    while (i < size) {
        tmp = 0;
        mul = 1;
        j = i;
        while (j < i + 4 && s[j]) {
            tmp += s[i] * mul;
            j++, mul *= 256;
        }
        sum = (sum + tmp) % mod, i++;
    }
    return sum;
}

size_t sfold(char *s, size_t mod) {
    size_t size = ft_strlen(s);
    return compute_hash(s, size / 4, compute_hash(s, size, 0, mod), mod) % mod;
}

t_hash hash_alloc(size_t size, void (*del)(void *)) {
	t_hash hash;
	size_t i;

	hash = malloc(sizeof(t_hash));
	size = ft_sqrt(size) + 1;
	hash->size = size;
	hash->array = malloc(size * sizeof(t_lst));
	hash->del = del;
	i = 0;
	while (i < size)
		hash->array[i++] = NULL;
	return hash;
}

bool hash_add(t_hash h, char *key, void *blob)
{
	size_t hash;
	t_lstnode walk;
	t_hashnode hnode;
	t_hashnode *tmp;

	if (!h || !blob || !key)
		return false;
	if (!h->array[hash = sfold(key, h->size) ])
	{
		hnode = (t_hashnode){ft_strdup(key), blob};
		h->array[hash] = lst_alloc(h->del);
	    lst_push_back_blob(h->array[hash], &hnode, sizeof(t_hashnode), true);
		return true;
	}
	walk = lst_front(h->array[hash]);
	while (walk)
	{
		tmp = walk->blob;
		if (!ft_strcmp(tmp->key, key))
			return false;
		lst_node_forward(&walk);
	}
	hnode = (t_hashnode){ft_strdup(key), blob};
    lst_push_front_blob(h->array[hash], &hnode, sizeof(t_hashnode), true);
	return true;
}

void *hash_get(t_hash h, char *key, void *default_val)
{
	size_t hash;
	t_lstnode walk;
	t_hashnode *tmp;

	if (!h->array[hash = sfold(key, h->size)])
		return default_val;
	walk = lst_front(h->array[hash]);
	while (walk)
	{
		tmp = walk->blob;
		if (!ft_strcmp(tmp->key, key))
			return tmp->blob;
	    lst_node_forward(&walk);
	}
	return default_val;
}

void hash_del(t_hash *h) {
    size_t i;
    t_hash hash;
    t_hashnode *tmp;

    if (!h || !(hash = *h))
        return;
    i = 0;
    while (i < hash->size) {
        while (!lst_empty(hash->array[i])) {
            tmp = lst_pop_back_blob(hash->array[i]);
            free(tmp->key);
            hash->del(tmp->blob);
        }
        lst_del(&hash->array[i++]);
    }
    free(hash->array);
    free(hash);
    *h = NULL;
}
