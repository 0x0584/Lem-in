/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   width_prec.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: melalj <melalj@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/23 18:04:30 by melalj            #+#    #+#             */
/*   Updated: 2019/11/05 17:07:15 by melalj           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../ft_printf.h"

static void	temper(char **temp, t_format all, int i, char **res)
{
	int		j;

	j = 0;
	if (all.flags[ZERO] == 1 && all.flags[MINUS] == 0 && (all.precision
	<= 0 || *res[0] == '-'))
	{
		if (*res[0] == '-' && (*res[0] = '0'))
		{
			my_bzero(*temp, (all.width - i));
			*temp[0] = '-';
		}
		else if (all.flags[PLUS] == 1 && *res[0] != '-' && (*res[0] = '0'))
		{
			my_bzero(*temp, (all.width - i));
			*temp[0] = '+';
		}
		else
			my_bzero(*temp, (all.width - i));
	}
	else
		while (j < (all.width - i))
		{
			(*temp)[j] = ' ';
			j++;
		}
}

void		ft_precision(char **res, t_format all)
{
	int		i;
	char	*temp;

	i = ft_strlen(*res);
	if (i <= all.precision)
	{
		temp = ft_strnew((all.precision - i) + 1);
		if (*res[0] == '-')
		{
			*res[0] = '0';
			my_bzero(temp, ((all.precision - i)) + 1);
			temp[0] = '-';
		}
		else
			my_bzero(temp, (all.precision - i));
		*res = my_strcat(&temp, res);
	}
}

void		ft_width(char **res, t_format all, int x)
{
	int		i;
	char	*temp;

	i = ft_strlen(*res);
	if (all.precision != -1 && all.flags[ZERO])
		return ;
	if (all.flags[HASH] == 1 && (all.specifier == 'x'
	|| all.specifier == 'X') && x != 0)
		i += 2;
	if (i < all.width)
	{
		temp = ft_strnew(all.width - i);
		temper(&temp, all, i, res);
		if (all.flags[MINUS] == 1)
			*res = my_strcat(res, &temp);
		else
			*res = my_strcat(&temp, res);
	}
}
