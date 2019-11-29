/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_addline.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: melalj <melalj@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/08 10:56:28 by melalj            #+#    #+#             */
/*   Updated: 2019/11/05 16:59:44 by melalj           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

void	add_line(char **line, char *to_add)
{
	char *tmp;

	if (!to_add)
		return ;
	if (!line || !*line)
		*line = ft_strdup("");
	tmp = ft_strjoin(*line, to_add);
	free(*line);
	*line = NULL;
	*line = tmp;
}
