/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   math_stradd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: melalj <melalj@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/20 08:04:34 by melalj            #+#    #+#             */
/*   Updated: 2019/11/05 17:07:15 by melalj           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../ft_printf.h"

static int	*math_stradd_init(char **result, char *s1, char *s2)
{
	int *len;

	len = (int *)malloc(sizeof(int) * 4);
	len[0] = ft_strlen(s1);
	len[1] = ft_strlen(s2);
	len[2] = (len[0] > len[1] ? len[0] : len[1]) + 1;
	*result = ft_strnew(len[2]);
	my_bzero(*result, len[2]);
	len[0]--;
	len[1]--;
	len[3] = 0;
	return (len);
}

char		*math_stradd(char *s1, char *s2)
{
	char	*result;
	int		*len;
	char	*dump;

	len = math_stradd_init(&result, s1, s2);
	while (len[0] >= 0 && len[1] >= 0 && --len[2] >= 0)
	{
		result[len[2]] = ((s1[len[0]] + s2[len[1]] + len[3] - 96) % 10) + 48;
		len[3] = (s1[len[0]] + s2[len[1]] - 96 + len[3]) / 10;
		len[0]--;
		len[1]--;
	}
	while (len[0] >= 0 || len[1] >= 0)
	{
		result[--len[2]] = len[0] > len[1] ? ((s1[len[0]] +
			len[3] - 48) % 10) + 48 : ((s2[len[1]] + len[3] - 48) % 10) + 48;
		len[3] = len[0] > len[1] ? (s1[len[0]] - 48 + len[3]) / 10 :
									(s2[len[1]] - 48 + len[3]) / 10;
		len[0]--;
		len[1]--;
	}
	result[len[2] - 1] = (len[3]) ? len[3] + 48 : result[len[2] - 1];
	dump = free_sl(len) ? cln_zero(result) : 0;
	free(result);
	return (dump);
}
