#ifndef HASH_H
#define HASH_H

#include <stdlib.h>

#include "queue.h"

typedef 	void (*hash_del)(void *, size_t);

#define AS_HNODE(foo) ((t_hnode *)(foo->blob))

typedef struct hash_node {
	char *key;
	void *blob;
} t_hnode;

typedef struct s_hash {
	t_queue		**array;			/* queue will hold s_vertex and s_edge
									 * as pairs of key/value struct hnode */
	size_t		size;
	hash_del	del;
} t_hash;

t_hash *hash_init(size_t size, hash_del del);

bool hash_add(t_hash *h,  t_hnode *node);
void hash_free(t_hash *h);
bool hash_default_cmp(void *u, void *v);
void *hash_get(t_hash *h, char *key, void *val_default);
void hash_default_del(void *ptr, size_t size);

#endif /* HASH_H */
