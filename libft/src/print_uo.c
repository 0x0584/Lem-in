/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_uo.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: melalj <melalj@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/08 16:59:11 by aihya             #+#    #+#             */
/*   Updated: 2019/11/05 17:07:15 by melalj           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../ft_printf.h"

void			prefix(char *str, char *prefix)
{
	ft_putstr(prefix);
	ft_putstr(str);
}

static int		print_o_on_right(t_format *fs, char *s, int size)
{
	int		ret;

	ret = 0;
	if (fs->precision != DEFAULT)
	{
		if (fs->precision >= fs->width)
			ret += repeate_char(48, fs->precision - size);
		else if (fs->precision >= size)
		{
			ret += repeate_char(32, fs->width - fs->precision);
			ret += repeate_char(48, fs->precision - size);
		}
		else
			ret += repeate_char(32, fs->width - size);
	}
	else
		ret += repeate_char(fs->flags[ZERO] ? 48 : 32, fs->width - size);
	ft_putstr(s);
	return (ret + size);
}

static int		print_o_on_left(t_format *fs, char *s, int size)
{
	int		ret;

	ret = repeate_char(48, fs->precision - size);
	if (fs->precision >= fs->width)
		ft_putstr(s);
	else if (fs->precision > size)
	{
		ft_putstr(s);
		ret += repeate_char(32, fs->width - fs->precision);
	}
	else
	{
		ft_putstr(s);
		ret += repeate_char(32, fs->width - size);
	}
	return (ret + size);
}

static char		*get_s(unsigned long long int val, t_format *fs)
{
	char	*s;
	char	*tmp;
	int		base;

	if (val == 0)
	{
		if (fs->precision == 0 || (fs->flags[HASH]))
			s = ft_strdup("");
		else
			s = ft_strdup("0");
	}
	else
	{
		base = fs->specifier == 'o' ? 8 : 10;
		s = ft_itoa_base_u(val, base, 0);
	}
	if (fs->flags[HASH])
	{
		tmp = ft_strjoin("0", s);
		free(s);
		s = ft_strdup(tmp);
		free(tmp);
	}
	return (s);
}

int				print_uo(t_format *fs)
{
	unsigned long long int	val;
	char					*s;
	int						size;
	int						ret;

	ret = 0;
	if (fs->length == L_HH)
		val = (unsigned char)va_arg(fs->ap, int);
	else if (fs->length == L_H)
		val = (unsigned short int)va_arg(fs->ap, int);
	else if (fs->length == L_L)
		val = (unsigned long int)va_arg(fs->ap, long int);
	else if (fs->length == L_LL)
		val = (unsigned long long int)va_arg(fs->ap, long long int);
	else
		val = (unsigned int)va_arg(fs->ap, int);
	s = get_s(val, fs);
	size = (int)ft_strlen(s);
	if (fs->flags[MINUS])
		ret = print_o_on_left(fs, s, size);
	else
		ret = print_o_on_right(fs, s, size);
	free(s);
	return (ret);
}
