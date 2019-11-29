/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strsub.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: melalj <melalj@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/15 19:18:24 by melalj            #+#    #+#             */
/*   Updated: 2019/11/05 16:59:39 by melalj           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

char	*ft_strsub(char const *s, unsigned int start, size_t len)
{
	char	*c_s;
	int		i;

	i = 0;
	if (!s)
		return (0);
	c_s = (char *)malloc(len + 1);
	if (!c_s)
		return (0);
	while (len)
	{
		c_s[i] = s[start];
		start++;
		len--;
		i++;
	}
	c_s[i] = '\0';
	return (c_s);
}
