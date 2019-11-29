/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   itoa_unsi.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: melalj <melalj@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/23 18:03:53 by melalj            #+#    #+#             */
/*   Updated: 2019/11/05 17:07:15 by melalj           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../ft_printf.h"

char		*ft_unsigned(unsigned long long int n)
{
	unsigned long long int		temp;
	int							len;
	char						*s;

	temp = n;
	len = 0;
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
	return (s);
}
