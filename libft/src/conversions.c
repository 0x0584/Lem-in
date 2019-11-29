/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   conversions.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: melalj <melalj@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/17 19:28:45 by melalj            #+#    #+#             */
/*   Updated: 2019/10/27 20:15:50 by melalj           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../ft_printf.h"

char	*conversion_check(t_format *format)
{
	if (format->specifier == 's')
		return (conversion_s(format));
	else if (format->specifier == 'd' || format->specifier == 'i')
		return (ft_printd(format));
	else if (format->specifier == 'c')
		return (conversion_c(format));
	else if (format->specifier == 'p')
		return (conversion_p(format));
	else if (format->specifier == 'f')
		return (conversion_f(format));
	else
		return (conversion_pr(format));
	return (0);
}
