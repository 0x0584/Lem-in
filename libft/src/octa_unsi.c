/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   octa_unsi.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: melalj <melalj@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/23 18:04:15 by melalj            #+#    #+#             */
/*   Updated: 2019/11/05 17:07:15 by melalj           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../ft_printf.h"

char	*octa(unsigned long long int o)
{
	char	*oct;
	int		i;

	i = 0;
	oct = (char*)malloc(sizeof(char) * 23);
	while (i < 22)
	{
		oct[21 - i] = (o % 8) + '0';
		i++;
		o /= 8;
	}
	oct[22] = '\0';
	return (ft_trimmer(oct));
}

char	*ft_trimmer(char *str)
{
	int		i;
	char	*s;

	i = 0;
	while (str[i] == '0')
	{
		if (str[i + 1] != '\0')
			i++;
		else
			break ;
	}
	str[22] = '\0';
	s = ft_strdup(str + i);
	free(str);
	return (s);
}
