#include "hash.h"

size_t compute_hash(char *s, size_t size, size_t sum) {
    for (size_t i = 0; i < size; ++i)
	{
		size_t tmp = 0;
        for (size_t mul = 1, j = i, k = i + 4; s[j] && j < k; ++j, mul *= 256)
            tmp += s[i] * mul;
		sum += tmp;
		printf("%zu\n", tmp);
	}
    return sum;
}

size_t sfold(char *s, size_t mod) {
    size_t size = ft_strlen(s);
    return compute_hash(s, size / 4, compute_hash(s, size, 0)) % mod;
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

bool hash_add(t_hash *h, t_hnode *node)
{
	size_t hash;
	t_qnode *walk;

	if (!h || !node)
		return false;
	hash = sfold(node->key, h->size);
	if (!h->array[hash])
	{
		h->array[hash] = queue_init();
		queue_enq(h->array[hash], queue_node(node, sizeof(t_hnode), true));
		return true;
	}
	walk = QFIRST(h->array[hash]);
	while (walk != QTAIL(h->array[hash]))
	{
		if (!ft_strcmp(AS_HNODE(walk)->key, node->key))
			return false;
		walk = walk->next;
	}
	queue_enq(h->array[hash], queue_node(node, sizeof(t_hnode), true));
	return true;
}

void *hash_get(t_hash *h, char *key, void *val_default)
{
	size_t hash = sfold(key, h->size);

	t_qnode *walk;

	walk = QFIRST(h->array[hash]);
	while (walk != QTAIL(h->array[hash]))
	{
		if (!ft_strcmp(AS_HNODE(walk)->key, key))
			return AS_HNODE(walk)->blob;
		walk = walk->next;
	}
	return val_default;
}

void hash_default_del(void *ptr, size_t size) {
	(void)size;
	free(ptr);
}

void hash_free(t_hash *h) {
	size_t i;

	if (!h)
		return ;
	i = 0;
	while (i < h->size) {
		if (!h->array[i])
			continue;
		queue_del(&h->array[i], h->del);
	}
}
