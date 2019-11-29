/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: melalj <melalj@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/20 15:35:41 by melalj            #+#    #+#             */
/*   Updated: 2019/11/05 16:59:39 by melalj           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

char	*ft_strset(char c, int i)
{
	char	*str;
	int		j;

	if (!(str = (char *)malloc(sizeof(char) * (i + 1))))
	{
		ft_putendl_fd("could not allocat space in ft_strset", 2);
		return (NULL);
	}
	j = -1;
	while (++j < i)
		str[j] = c;
	str[j] = '\0';
	return (str);
}
