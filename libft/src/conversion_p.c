/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   conversion_p.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: melalj <melalj@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/18 19:45:37 by melalj            #+#    #+#             */
/*   Updated: 2019/10/03 20:31:23 by melalj           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../ft_printf.h"

static void	conversion_pprecision(t_format *format, int *i)
{
	if (format->precision != -1)
	{
		*i = format->width;
		format->width = format->precision;
		format->flags[ZERO] = -1;
		format->precision = -1;
	}
}

static void	conversion_pwidth(t_format *format, int *i, char **dump)
{
	format->width = *i;
	if ((format->width > 0))
	{
		format->flags[ZERO] = 0;
		width_formating(*format, dump);
	}
}

char		*conversion_p(t_format *format)
{
	long long int	p;
	char			*s;
	char			*dump;
	int				i;

	p = (long long int)va_arg(format->ap, void *);
	dump = hexa(p, 0);
	dump[0] = (!p && !format->precision) ? '\0' : dump[0];
	conversion_pprecision(format, &i);
	if (format->flags[ZERO])
	{
		format->width -= (format->flags[ZERO] < 0) ? 0 : 2;
		s = formating(*format, dump, 1);
		free(dump);
		dump = ft_strjoin("0x", s);
	}
	else
	{
		s = ft_strjoin("0x", dump);
		free(dump);
		dump = formating(*format, s, 1);
		free(s);
	}
	conversion_pwidth(format, &i, &dump);
	return (dump);
}
