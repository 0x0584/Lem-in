/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_charappend.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: melalj <melalj@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/19 02:32:41 by melalj            #+#    #+#             */
/*   Updated: 2019/11/05 16:59:39 by melalj           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

char	*ft_chrappend(char **s, char c, int ovr)
{
	char	*tmp;
	int		i;

	i = ft_strlen(*s);
	tmp = (char *)malloc(i + 2);
	i = 0;
	while (*s && (*s)[i])
	{
		tmp[i] = (*s)[i];
		i++;
	}
	tmp[i] = c;
	tmp[i + 1] = '\0';
	if (ovr)
	{
		free(*s);
		*s = tmp;
	}
	return (tmp);
}
