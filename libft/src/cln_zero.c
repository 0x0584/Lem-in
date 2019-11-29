/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cln_zero.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: melalj <melalj@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/20 09:25:05 by melalj            #+#    #+#             */
/*   Updated: 2019/10/03 20:33:02 by melalj           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../ft_printf.h"

char	*cln_zero(char *str)
{
	char	*cln;
	int		i;

	i = -1;
	while (str[++i] == '0')
		;
	cln = ft_strsub(str, i, ft_strlen(str) - i);
	return (cln);
}
