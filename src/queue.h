/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   queue.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: archid- <archid-@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/29 16:34:27 by archid-           #+#    #+#             */
/*   Updated: 2019/12/21 05:58:49 by archid-          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef QUEUE_H
# define QUEUE_H

# include "../libft/libft.h"

typedef struct s_queue_node	t_qnode;
struct							s_queue_node
{
	void				*blob;
	size_t				size;
	struct s_queue_node	*next;
	struct s_queue_node	*prev;
};

/*
** always head -> tail
** inserting nodes between head and tail
*/

typedef struct s_queue			t_queue;
struct							s_queue
{
	struct s_queue_node *head;
	struct s_queue_node *tail;
	void				(*del)(void *, size_t);
};

void							queue_del_helper(void *blob, size_t size);
t_qnode							*queue_node(void *blob, size_t size);
void							queue_node_del(t_qnode **a_node,
												void (*del)(void *, size_t));
t_queue							*queue_init(void);
void							queue_iter(t_queue *q, bool from_head,
											void (*f)(t_qnode *));
size_t							queue_size(t_queue *q);
void							queue_del(t_queue **a_queue,
											void (*del)(void *, size_t));
void							queue_enq(t_queue *queue, t_qnode *node);
t_qnode 						*queue_deq(t_queue *queue);

t_qnode							*queue_last(t_queue *q);
void							queue_node_del_dry(void *blob, size_t size);
t_qnode							*queue_dry_node(void *data, size_t size);

#endif
