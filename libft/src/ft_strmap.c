/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: melalj <melalj@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/15 15:43:43 by melalj            #+#    #+#             */
/*   Updated: 2019/11/05 16:59:39 by melalj           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

char	*ft_strmap(const char *s, char (*f)(char))
{
	char	*str;
	char	*res;
	int		len;

	if (!s)
		return (0);
	len = ft_strlen((char*)s);
	str = (char *)malloc(len + 1);
	if (!str)
		return (0);
	res = str;
	while (*s)
		*str++ = f(*s++);
	*str = 0;
	return (res);
}
