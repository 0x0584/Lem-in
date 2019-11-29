/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   math_strhit.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: melalj <melalj@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/20 09:15:57 by melalj            #+#    #+#             */
/*   Updated: 2019/11/05 17:07:15 by melalj           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../ft_printf.h"

char	*math_strhit(char *s1, char *s2)
{
	char	*result;
	int		len[3];
	int		i;
	int		carry;

	len[1] = ft_strlen(s1);
	len[2] = ft_strlen(s2);
	len[0] = len[1] + len[2];
	result = (char *)malloc(sizeof(char) * (len[0] + 1));
	my_bzero(result, len[0]);
	i = 0;
	while (len[2] > 0 && !(carry = 0))
	{
		i = len[1];
		while (i > 0)
		{
			result[len[2] + i - 1] +=
				carry + ((s2[len[2] - 1] - 48) * (s1[i - 1] - 48)) - 48;
			carry = result[len[2] + i - 1] / 10;
			result[len[2] + i - 1] = (result[len[2] + i - 1] % 10) + 48;
			i--;
		}
		result[len[2]-- + i - 1] += carry;
	}
	return (result);
}
