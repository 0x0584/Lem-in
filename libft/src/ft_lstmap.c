/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: melalj <melalj@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/24 00:53:52 by melalj            #+#    #+#             */
/*   Updated: 2019/11/05 17:07:52 by melalj           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"
#include "../libft.h"

t_list	*ft_lstmap(t_list *lst, t_list *(*f)(t_list *elem))
{
	t_list	*current;
	t_list	*mod_lst_head;
	t_list	*mod_lst;

	current = lst;
	mod_lst = f(current);
	mod_lst_head = mod_lst;
	while (current->next)
	{
		mod_lst->next = f(current->next);
		mod_lst = mod_lst->next;
		current = current->next;
	}
	return (mod_lst_head);
}
