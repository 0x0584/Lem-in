/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memccpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: melalj <melalj@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/12 04:21:44 by melalj            #+#    #+#             */
/*   Updated: 2019/11/05 16:59:39 by melalj           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

void	*ft_memccpy(void *dst, const void *src, int c, size_t n)
{
	unsigned char	*copy_dst;
	unsigned char	*copy_src;
	size_t			i;

	copy_src = (unsigned char *)src;
	copy_dst = (unsigned char *)dst;
	i = 0;
	while (i < n)
	{
		copy_dst[i] = copy_src[i];
		if (copy_src[i] == (unsigned char)c)
			return (&copy_dst[i + 1]);
		i++;
	}
	return (NULL);
}
