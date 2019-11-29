/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: melalj <melalj@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/16 16:01:13 by melalj            #+#    #+#             */
/*   Updated: 2019/11/05 17:06:23 by melalj           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../ft_printf.h"

int		ft_printf(const char *s, ...)
{
	int			i;
	int			a;
	char		*str;
	t_format	format;
	int			len;

	i = -1;
	a = 0;
	len = ft_strlen(s);
	va_start(format.ap, s);
	while (++i < len)
	{
		if (s[i] == '%' && s[i + 1] && (str = interprete(&format, &(s[i]), &i)))
		{
			a += format.c;
			if ((a += ft_strlen(str)) && !ft_strchr("uoxX", format.specifier))
				ft_putstr(str);
			if (free_sl(str) && format.c && !format.flags[MINUS])
				write(1, "", 1);
		}
		else if (s[i] != '%' && ++a)
			ft_putchar(s[i]);
	}
	va_end(format.ap);
	return (a);
}
