/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   conversion_pr.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: melalj <melalj@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/17 17:32:11 by melalj            #+#    #+#             */
/*   Updated: 2019/10/03 20:16:45 by melalj           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../ft_printf.h"

char	*conversion_pr(t_format *format)
{
	char *s;

	s = ft_strnew(1);
	s[0] = format->specifier;
	if (!s[0])
		return (ft_strdup(""));
	return (formating(*format, s, 1));
}
