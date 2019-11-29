/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   itoa_long.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: melalj <melalj@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/23 18:03:48 by melalj            #+#    #+#             */
/*   Updated: 2019/11/05 17:07:15 by melalj           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../ft_printf.h"

static void	ft_isnega(long long int *n, int *sign)
{
	if (*n < 0)
	{
		*n *= -1;
		*sign = 1;
	}
}

char		*ft_long(long long int n)
{
	long long int		temp;
	int					len;
	int					sign;
	char				*s;

	if ((unsigned long)n == -9223372036854775808U)
		return (ft_strdup("-9223372036854775808"));
	temp = n;
	sign = 0;
	ft_isnega(&n, &sign);
	len = sign;
	while (temp /= 10)
		len++;
	s = ft_strnew(len + 1);
	if (!s)
		return (NULL);
	while (len >= 0)
	{
		s[len] = n % 10 + '0';
		n /= 10;
		len--;
	}
	if (sign == 1)
		s[0] = '-';
	return (s);
}
