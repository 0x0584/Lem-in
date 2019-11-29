/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hexadeci.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: melalj <melalj@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/23 18:03:38 by melalj            #+#    #+#             */
/*   Updated: 2019/11/05 17:07:15 by melalj           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../ft_printf.h"

char	*reverse(char *line, int b)
{
	int		i;
	char	*temp;
	int		j;

	j = 0;
	i = ft_strlen(line);
	temp = malloc(sizeof(char) * i + 1);
	while (i > 0)
	{
		i--;
		if (b == 0)
			temp[j] = ft_tolower(line[i]);
		if (b == 1)
			temp[j] = line[i];
		j++;
	}
	temp[j] = '\0';
	free(line);
	line = ft_strdup(temp);
	free(temp);
	return (line);
}

char	*hexa(unsigned long long int a, int x)
{
	unsigned long long int	b;
	int						i;
	unsigned long long int	c;
	char					*ret;

	ret = malloc(sizeof(char) * 100);
	b = a;
	i = 0;
	if (!b)
	{
		free(ret);
		return (ft_strdup("0"));
	}
	while (b != 0)
	{
		c = b % 16;
		if (c < 10)
			ret[i++] = 48 + c;
		else
			ret[i++] = 55 + c;
		b = b / 16;
	}
	ret[i] = '\0';
	ret = reverse(ret, x);
	return (ret);
}
