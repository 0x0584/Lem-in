/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   conversion_s.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: melalj <melalj@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/17 17:32:11 by melalj            #+#    #+#             */
/*   Updated: 2019/10/03 18:25:23 by melalj           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../ft_printf.h"

char	*conversion_s(t_format *format)
{
	char *s;

	s = va_arg(format->ap, char *);
	if (!s)
		s = ft_strdup("(null)");
	return (formating(*format, s, 1));
}
