/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   conversion_c.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: melalj <melalj@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/18 18:37:56 by melalj            #+#    #+#             */
/*   Updated: 2019/10/03 20:30:36 by melalj           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../ft_printf.h"

char	*conversion_c(t_format *format)
{
	char *s;

	s = ft_strnew(1);
	s[0] = (char)va_arg(format->ap, int);
	if (format->specifier == 'c' && !s[0])
		format->c = 1;
	if (format->width > 0)
		format->width -= (format->specifier == 'c' && !s[0]) ? 1 : 0;
	if (format->c && format->flags[MINUS])
		write(1, "", 1);
	return (formating(*format, s, 0));
}
