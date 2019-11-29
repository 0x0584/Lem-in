/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: melalj <melalj@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/20 19:00:04 by melalj            #+#    #+#             */
/*   Updated: 2019/11/05 17:07:49 by melalj           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../ft_printf.h"

static char	*ft_printd_va_arg(t_format *all)
{
	if (all->length == 'H')
		return (ft_long((long long int)(signed char)va_arg(all->ap, int)));
	else if (all->length == 'h')
		return (ft_long((long long int)(short int)va_arg(all->ap, int)));
	else if (all->length == 'l' || all->length == '(')
		return (ft_long(va_arg(all->ap, long long int)));
	else
		return (ft_long((long long int)va_arg(all->ap, int)));
}

char		*ft_printd(t_format *all)
{
	char	*res;
	char	*pls;

	res = ft_printd_va_arg(all);
	ft_precision(&res, *all);
	if (all->precision == 0 && res[0] == '0' && !(all->flags[ZERO] = 0))
		res[0] = '\0';
	if (all->flags[PLUS] == 1 && res[0] != '-' && res[0] != '+'
		&& (pls = ft_strdup("+")))
		res = my_strcat(&pls, &res);
	all->flags[SPACE] =
		(all->flags[PLUS] || res[0] == '-') ? 0 : all->flags[SPACE];
	all->flags[ZERO] = (all->precision != -1) ? 0 : all->flags[ZERO];
	all->width = (all->flags[SPACE]) ? all->width - 1 : all->width;
	ft_width(&res, *all, 0);
	if (all->flags[SPACE] == 1)
	{
		pls = ft_strdup(" ");
		res = my_strcat(&pls, &res);
	}
	return (res);
}
