/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   extra.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: archid- <archid-@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/16 13:08:01 by archid-           #+#    #+#             */
/*   Updated: 2020/12/16 13:35:55 by archid-          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_u64 ft_abs(t_s64 x)
{
	return ((x) < 0 ? (x) * -1 : (x));
}

t_s64		max(t_s64 a, t_s64 b)
{
	return	((a) > (b) ? (a) : (b));
}

t_s64			min(t_s64 a, t_s64 b)
{
	return	((a) < (b) ? (a) : (b));
}

t_u64			umax(t_u64 a, t_u64 b)
{
	return	((a) > (b) ? (a) : (b));
}

t_u64			umin(t_u64 a, t_u64 b)
{
	return	((a) < (b) ? (a) : (b));
}

void		*ft_calloc(size_t n, size_t sz)
{
	return (ft_memalloc((n) * (sz)));
}

int			get_digit(char c)
{
	return (c - '0');
}

int			to_digit(char c)
{
	return (c + '0');
}
