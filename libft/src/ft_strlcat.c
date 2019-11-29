/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: melalj <melalj@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/11 04:36:44 by melalj            #+#    #+#             */
/*   Updated: 2019/11/05 16:59:39 by melalj           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t size)
{
	char	*end_dst;
	size_t	max;
	size_t	r;
	size_t	p;

	p = ft_strlen(src) + size;
	r = ft_strlen(dst) + ft_strlen(src);
	max = size - 1 - ft_strlen(dst);
	end_dst = dst + ft_strlen(dst);
	while (*src && (int)max > 0)
	{
		*end_dst = *src;
		src++;
		end_dst++;
		max--;
	}
	*end_dst = '\0';
	if (r > p)
		return (p);
	return (r);
}
