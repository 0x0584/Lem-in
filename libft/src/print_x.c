/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_x.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: melalj <melalj@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/10 20:41:54 by aihya             #+#    #+#             */
/*   Updated: 2019/11/05 17:07:15 by melalj           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../ft_printf.h"

static int	print_x_on_right(t_format *fs, char *s, int size, int val)
{
	int		ret;

	ret = 0;
	if (fs->precision != DEFAULT)
	{
		if (fs->precision >= fs->width)
		{
			ret += set_x_right_prefix(val, fs, fs->precision, 0);
			ret += repeate_char(48, fs->precision - size);
		}
		else if (fs->precision >= size)
		{
			ret += set_x_right_prefix(val, fs, fs->precision, 0);
			ret += repeate_char(48, fs->precision - size);
		}
		else
			ret += set_x_right_prefix(val, fs, size, 0);
	}
	else
		ret += set_x_right_prefix(val, fs, size, DEFAULT);
	ft_putstr(s);
	return (ret + size);
}

static void	init_args(int args[2], int htag_condition, int size)
{
	args[0] = htag_condition;
	args[1] = size;
}

static int	print_x_on_left(t_format *fs, char *s, int size, int val)
{
	int		ret;
	int		args[2];

	ret = 0;
	init_args(args, val != 0 && (fs->flags[HASH]), fs->precision - size);
	if (fs->precision != DEFAULT)
	{
		if (fs->precision >= fs->width)
			ret += put_x(args, fs, 1, s);
		else if (fs->precision >= size)
		{
			ret += put_x(args, fs, 1, s);
			ret += put_trailing_spaces(fs, args[0], fs->precision);
		}
		else
		{
			ret += put_x(args, fs, 0, s);
			ret += put_trailing_spaces(fs, fs->flags[HASH], size);
		}
		return (ret + size);
	}
	ret += put_x(args, fs, 0, s);
	ret += put_trailing_spaces(fs, args[0], size);
	return (ret + size);
}

static char	*get_s(unsigned long long int val, t_format *fs)
{
	if (val == 0 && fs->precision == 0)
		return (ft_strdup(""));
	return (ft_itoa_base_u(val, 16, fs->specifier == 'x' ? 0 : 1));
}

int			print_x(t_format *fs)
{
	unsigned long long int	val;
	char					*s;
	int						size;
	int						ret;

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
	ret = 0;
	if (fs->flags[MINUS])
		ret = print_x_on_left(fs, s, size, val);
	else
		ret = print_x_on_right(fs, s, size, val);
	free(s);
	return (ret);
}
