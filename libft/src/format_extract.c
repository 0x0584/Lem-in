/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   format_extract.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: melalj <melalj@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/17 17:10:58 by melalj            #+#    #+#             */
/*   Updated: 2019/10/27 21:41:43 by melalj           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../ft_printf.h"
#include <stdio.h>

t_format	specifier_extract(t_format format, const char *s, int len)
{
	format.specifier = s[len];
	return (format);
}

t_format	width_extract(t_format format, const char *s, int len)
{
	int		i;
	char	*dump;

	i = 0;
	while (++i <= len)
	{
		if ((s[i] != '0') && (s[i - 1] != '.')
			&& (s[i - 1] == '0' || !ft_isdigit(s[i - 1])) && ft_isdigit(s[i]))
		{
			format.width = ft_atoi(&(s[i]));
			if (format.width < 0)
				format.width = 0;
			dump = ft_itoa(format.width);
			i += ft_strlen(dump) - 1;
			free(dump);
		}
		else if (s[i] == '*' && s[i - 1] != '.')
		{
			format.width = va_arg(format.ap, int);
			if (format.width < 0 && (format.flags[MINUS] = 1))
				format.width = (format.width) * -1;
		}
	}
	return (format);
}

t_format	precision_extract(t_format format, const char *s, int len)
{
	int		i;
	char	*dump;

	i = 0;
	while (++i <= len)
	{
		if (s[i] == '.')
			format.precision = 0;
		else if (s[i - 1] == '.' && ft_isdigit(s[i]))
		{
			format.precision = ft_atoi(&(s[i]));
			if (format.precision < 0)
				format.precision = DEFAULT;
			dump = ft_itoa(format.precision);
			i += ft_strlen(dump) - 1;
			free(dump);
		}
		else if (s[i - 1] == '.' && s[i] == '*')
		{
			format.precision = va_arg(format.ap, int);
			if (format.precision < 0)
				format.precision = DEFAULT;
		}
	}
	return (format);
}

t_format	length_extract(t_format format, const char *s, int len)
{
	int i;

	i = 0;
	while (++i <= len)
	{
		if (s[i] == 'h' && s[i - 1] == 'h')
			format.length = 'H';
		else if (s[i] == 'l' && s[i - 1] == 'l')
			format.length = '(';
		else if (s[i] == 'h')
			format.length = 'h';
		else if (s[i] == 'l')
			format.length = 'l';
		else if (s[i] == 'L')
			format.length = 'L';
	}
	return (format);
}

t_format	flags_extract(t_format format, const char *s, int len)
{
	int i;

	i = 0;
	while (++i <= len)
	{
		if (s[i] == '-')
			format.flags[MINUS] = 1;
		else if (s[i] == '+')
			format.flags[PLUS] = 1;
		else if (s[i] == ' ')
			format.flags[SPACE] = 1;
		else if (s[i] == '#')
			format.flags[HASH] = 1;
		else if (s[i - 1] != '.' && s[i] == '0' && !ft_isdigit(s[i - 1]))
			format.flags[ZERO] = 1;
	}
	return (format);
}
