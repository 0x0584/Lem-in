/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: melalj <melalj@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/16 19:04:24 by melalj            #+#    #+#             */
/*   Updated: 2019/11/05 16:59:39 by melalj           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

char	*ft_strtrim(char const *s)
{
	int		start;
	int		end;
	char	*c_s;
	char	*rc_s;

	if (!s)
		return (0);
	start = 0;
	end = ft_strlen(s);
	while ((s[start] > 7 && s[start] < 14) || s[start] == ' ')
		start++;
	while ((s[end] > 7 && s[end] < 14) || s[end] == ' ' || s[end] == '\0')
		end--;
	if (end < 0)
		return (ft_strdup(""));
	c_s = (char*)malloc(sizeof(char) * (end - start + 2));
	if (!c_s)
		return (0);
	rc_s = c_s;
	while (start < end + 1)
		*c_s++ = (char)s[start++];
	*c_s = '\0';
	return (rc_s);
}
