/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_joinwith.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: melalj <melalj@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/22 19:32:10 by melalj            #+#    #+#             */
/*   Updated: 2019/11/05 16:59:39 by melalj           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

char	*ft_joinwith(char *s1, char *s2, char *joint)
{
	char	*str;
	char	*result;
	int		ba;
	int		bb;

	ba = 0;
	bb = 0;
	if (!s1 && (ba = 1))
		s1 = ft_strdup("");
	if (!s2 && (bb = 1))
		s2 = ft_strdup("");
	str = NULL;
	result = NULL;
	str = ft_strjoin(s1, joint);
	result = ft_strjoin(str, s2);
	free(str);
	if (ba == 1)
		free_sl(s1);
	if (ba == 1)
		free_sl(s2);
	return (result);
}
