/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_free.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: melalj <melalj@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/14 00:54:30 by melalj            #+#    #+#             */
/*   Updated: 2019/11/05 18:30:22 by melalj           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

int		free_sl(void *to_free)
{
	free(to_free);
	return (1);
}

int		free_tab(char **to_free)
{
	int	i;

	i = -1;
	if (to_free)
	{
		while (to_free[++i])
			free_sl(to_free[i]);
		free_sl(to_free);
	}
	return (1);
}
