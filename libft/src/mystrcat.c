/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mystrcat.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: melalj <melalj@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/23 18:04:08 by melalj            #+#    #+#             */
/*   Updated: 2019/11/05 16:59:39 by melalj           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

char	*my_strcat(char **s1, char **s2)
{
	char	*res;
	int		i;
	int		j;

	j = ft_strlen(*s1);
	i = j + ft_strlen(*s2);
	res = ft_strnew(i);
	i = 0;
	while (i < j)
	{
		res[i] = (*s1)[i];
		i++;
	}
	j = 0;
	while ((*s2)[j])
	{
		res[i] = (*s2)[j];
		i++;
		j++;
	}
	res[i] = '\0';
	free(*s1);
	free(*s2);
	return (res);
}
