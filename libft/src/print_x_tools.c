/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_x_tools.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: melalj <melalj@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/22 18:27:40 by aihya             #+#    #+#             */
/*   Updated: 2019/11/05 17:07:15 by melalj           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../ft_printf.h"

int		put_x(int *params, t_format *fs, int add_zeros, char *s)
{
	int		ret;
	int		htag_condition;
	int		size;

	ret = 0;
	htag_condition = params[0];
	size = params[1];
	if (htag_condition)
	{
		ret += 2;
		ft_putstr(fs->specifier == 'x' ? "0x" : "0X");
	}
	if (add_zeros)
		ret += repeate_char(48, size);
	ft_putstr(s);
	return (ret);
}

int		put_trailing_spaces(t_format *fs, int condition, int comp_target)
{
	int		ret;

	ret = 0;
	if (condition)
		ret += repeate_char(32, fs->width - comp_target - 2);
	else
		ret += repeate_char(32, fs->width - comp_target);
	return (ret);
}

int		set_x_right_prefix(int val, t_format *fs, int comp_target, int dp)
{
	int		ret;

	ret = 0;
	if (val != 0 && (fs->flags[HASH]))
	{
		if (dp == DEFAULT && (fs->flags[ZERO]))
		{
			ft_putstr(fs->specifier == 'x' ? "0x" : "0X");
			ret += repeate_char(48, fs->width - comp_target - 2) + 2;
		}
		else
		{
			ret += repeate_char(32, fs->width - comp_target - 2) + 2;
			ft_putstr(fs->specifier == 'x' ? "0x" : "0X");
		}
	}
	else
	{
		if (dp == DEFAULT && (fs->flags[ZERO]))
			ret += repeate_char(48, fs->width - comp_target);
		else
			ret += repeate_char(32, fs->width - comp_target);
	}
	return (ret);
}
