/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_line.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: melalj <melalj@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/15 04:36:41 by melalj            #+#    #+#             */
/*   Updated: 2019/12/03 23:52:49 by archid-          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../lem_in.h"

void	read_line(int fd, char **line)
{
	char *dump;
	char tmp[2];

	*line = ft_strdup("");
	while (read(fd, tmp, 1))
	{
		if (tmp[0] == '\n')
			break ;
		tmp[1] = 0;
		dump = ft_strjoin(*line, tmp);
		free(*line);
		*line = dump;
	}
}
