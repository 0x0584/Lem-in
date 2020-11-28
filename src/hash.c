/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hash.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: archid- <archid-@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/26 23:44:57 by archid-           #+#    #+#             */
/*   Updated: 2020/11/27 04:50:37 by archid-          ###   ########.fr       */
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

t_hash *hash_init(size_t size, hash_del del) {
	t_hash *hash;
	size_t i;

	hash = malloc(sizeof(t_hash));
	hash->size = size;
	hash->array = malloc(size * sizeof(t_queue *));
	hash->del = del;
	i = 0;
	while (i < size)
		hash->array[i++] = NULL;
	return hash;
}

bool hash_add(t_hash *h, char *key, void *blob)
{
	size_t hash;
	t_qnode *walk;
	t_hnode hnode;

	if (!h || !blob || !key)
		return false;
	hash = sfold(key, h->size);
	if (!h->array[hash])
	{
		hnode = (t_hnode){ft_strdup(key), blob};
		h->array[hash] = queue_init();
		queue_enq(h->array[hash], queue_node(&hnode, sizeof(t_hnode), true));
		return true;
	}
	walk = QFIRST(h->array[hash]);
	while (walk != QTAIL(h->array[hash]))
	{
		if (!ft_strcmp(AS_HNODE(walk)->key, key))
			return false;
		walk = walk->next;
	}
	hnode = (t_hnode){ft_strdup(key), blob};
	queue_enq(h->array[hash], queue_node(&hnode, sizeof(t_hnode), true));
	return true;
}

void *hash_get(t_hash *h, char *key, void *val_default)
{
	size_t hash = sfold(key, h->size);
	t_qnode *walk;

	if (!h->array[hash])
		return val_default;
	walk = QFIRST(h->array[hash]);
	while (walk != QTAIL(h->array[hash]))
	{
		if (!ft_strcmp(AS_HNODE(walk)->key, key))
			return AS_HNODE(walk)->blob;
		walk = walk->next;
	}
	return val_default;
}

void hash_default_del(void *ptr) {
	(void)ptr;
	return ;
}

void hash_free(t_hash *h) {
	size_t i;
	t_hnode *tmp_hnode;
	t_qnode *tmp_qnode;

	if (!h)
		return ;
	i = 0;
	while (i < h->size) {
		while (queue_size(h->array[i])) {
			tmp_qnode = queue_deq(h->array[i]);
			tmp_hnode = tmp_qnode->blob;
			free(tmp_hnode->key);
			h->del(tmp_hnode->blob);
			free(tmp_hnode);
			free(tmp_qnode);
		}
		queue_del(&h->array[i], queue_blob_free);
		i++;
	}
	free(h->array);
	free(h);
}
