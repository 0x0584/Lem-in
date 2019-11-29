/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   point_p.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: melalj <melalj@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/22 21:47:20 by melalj            #+#    #+#             */
/*   Updated: 2019/11/05 17:07:15 by melalj           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../ft_printf.h"

static void	add_pre(t_format format, char **str)
{
	char *s;
	char *dump;

	dump = ft_strnew(format.precision + 2);
	my_bzero(dump, format.precision + 1);
	dump[0] = '.';
	s = ft_strjoin(*str, dump);
	free(dump);
	free(*str);
	*str = s;
}

static char	*no_int_part(char *str, int len, int power)
{
	char *result;
	char *dump;

	result = ft_strnew(len + 2 + math_abs(len + power));
	result[0] = '0';
	result[1] = '.';
	my_bzero(&(result[2]), math_abs(len + power));
	result[2 + math_abs(len + power)] = '\0';
	dump = result;
	result = ft_strjoin(dump, str);
	free(dump);
	return (result);
}

char		*point_p(t_format format, char *str, int power)
{
	char	*result;
	int		len;
	int		i;
	int		j;

	len = ft_strlen(str);
	if (len + power <= 0)
		result = no_int_part(str, len, power);
	else
	{
		result = ft_strnew(len + 1);
		i = 0;
		j = 0;
		while (str[i])
		{
			if (j == len + power)
				result[j++] = '.';
			else
				result[j++] = str[i++];
		}
		if (j == len)
			add_pre(format, &result);
	}
	return (result);
}
