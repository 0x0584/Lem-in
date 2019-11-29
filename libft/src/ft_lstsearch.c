/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstsearch.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: melalj <melalj@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/24 19:01:53 by melalj            #+#    #+#             */
/*   Updated: 2019/11/05 17:07:52 by melalj           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"
#include "../libft.h"

t_list	*ft_lstsearch(t_list **lst, void *to_search)
{
	t_list	*current;
	t_list	*error;

	error = ft_lstnew("not found", 10);
	current = *lst;
	while (current != NULL)
	{
		if (!ft_memcmp(current->content, to_search, current->content_size))
			return (current);
		current = current->next;
	}
	return (error);
}
