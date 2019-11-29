/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   math_strpower.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: melalj <melalj@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/20 10:20:24 by melalj            #+#    #+#             */
/*   Updated: 2019/11/05 17:07:15 by melalj           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../ft_printf.h"

char	*math_strpower(char *base, char *power)
{
	int		power_i;
	char	*dump;
	char	*result;

	power_i = ft_atoi(power);
	result = math_strhit(base, "1");
	while (--power_i)
	{
		dump = math_strhit(result, base);
		free(result);
		result = dump;
	}
	return (result);
}
