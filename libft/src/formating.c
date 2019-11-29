/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   formating.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: melalj <melalj@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/17 20:46:39 by melalj            #+#    #+#             */
/*   Updated: 2019/10/03 20:31:23 by melalj           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../ft_printf.h"

char		*formating(t_format format, char *str, int a)
{
	char *s;

	s = ft_strdup(str);
	if (format.precision != -1 && format.specifier != '%')
		precision_formating(format, &s, a);
	if (format.width)
		width_formating(format, &s);
	return (s);
}

static void	int_init(int *i, int *j, char *str, int *len)
{
	*i = -1;
	*j = -1;
	*len = ft_strlen(str);
}

void		width_formating(t_format format, char **str)
{
	char	*tmp;
	int		len;
	int		i;
	int		j;

	int_init(&i, &j, *str, &len);
	if (format.width > len && (tmp = *str))
	{
		if ((*str = ft_strnew(format.width)) && !(format.flags[MINUS]))
		{
			while (++j < format.width - len)
				(*str)[j] = format.flags[ZERO] ? '0' : ' ';
			while (tmp[++i])
				(*str)[j++] = tmp[i];
			(*str)[j] = '\0';
		}
		else
		{
			while (tmp[++i])
				(*str)[++j] = tmp[i];
			while (++j < format.width)
				(*str)[j] = ' ';
		}
		free(tmp);
	}
}
