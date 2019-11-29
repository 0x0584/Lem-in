/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   precision_f.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: melalj <melalj@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/03 20:23:39 by melalj            #+#    #+#             */
/*   Updated: 2019/11/05 17:07:15 by melalj           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../ft_printf.h"

static int	point_pos(char *str)
{
	int i;

	i = -1;
	while (str[++i] && str[i] != '.')
		;
	return (i);
}

static void	precision_addz(t_format format, char **str, int j)
{
	char *dump;
	char *s;

	dump = ft_strnew(format.precision - j);
	my_bzero(dump, format.precision - j);
	s = *str;
	*str = ft_strjoin(s, dump);
	free(s);
	free(dump);
}

void		precision_formating(t_format format, char **str, int a)
{
	char	*s;
	int		i;
	int		j;

	j = -1;
	s = *str;
	i = point_pos(*str);
	*str = ft_strnew((format.specifier == 'f')
		? format.precision + i + 2 : format.precision);
	i = -1;
	if (format.specifier == 'f')
		while (s[++i] && s[i] != '.')
			(*str)[i] = s[i];
	i = (i == -1) ? i : i - 1;
	format.precision -= (format.specifier != 'f') ? 1 : 0;
	format.precision -= (format.specifier == 'f'
		&& !format.precision && !format.flags[HASH]) ? 1 : 0;
	while (s[++i] && ++j <= format.precision)
		(*str)[i] = s[i];
	(*str)[i] = '\0';
	if (a && format.specifier == 'f' && j < format.precision)
		precision_addz(format, str, j);
	free(s);
}
