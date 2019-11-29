/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pathjoin.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: melalj <melalj@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/14 12:09:04 by melalj            #+#    #+#             */
/*   Updated: 2019/11/05 16:59:39 by melalj           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

char	*ft_pathjoin(char *dir, char *file)
{
	char *str;
	char *result;

	str = NULL;
	result = NULL;
	if (ft_strequ(dir, "/"))
	{
		result = ft_strjoin(dir, file);
		return (result);
	}
	str = ft_strjoin(dir, "/");
	result = ft_strjoin(str, file);
	free(str);
	return (result);
}
