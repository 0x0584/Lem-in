/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interpreter.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: melalj <melalj@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/17 14:23:51 by melalj            #+#    #+#             */
/*   Updated: 2019/11/05 17:06:36 by melalj           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../ft_printf.h"

static int		format_len(const char *s)
{
	int i;

	i = 0;
	while (s[++i])
	{
		if (!ft_strchr("-+0 .*hlztL#", s[i]) && !ft_isdigit(s[i]))
			break ;
	}
	return (i);
}

static t_format	init_format(t_format format)
{
	format.flags[0] = 0;
	format.flags[1] = 0;
	format.flags[2] = 0;
	format.flags[3] = 0;
	format.flags[4] = 0;
	format.width = 0;
	format.precision = -1;
	format.length = 0;
	format.specifier = 0;
	format.c = 0;
	return (format);
}

static t_format	format_extract(t_format format, const char *s, int *count)
{
	int i;

	i = format_len(s);
	format = init_format(format);
	format = flags_extract(format, s, i);
	format = width_extract(format, s, i);
	format = precision_extract(format, s, i);
	format = length_extract(format, s, i);
	format = specifier_extract(format, s, i);
	*count += i;
	return (format);
}

char			*interprete(t_format *format, const char *s, int *count)
{
	*format = format_extract(*format, s, count);
	if (ft_strchr("uo", format->specifier))
		return (ft_strset('0', print_uo(format)));
	if (ft_strchr("xX", format->specifier))
		return (ft_strset('0', print_x(format)));
	return (conversion_check(format));
}
