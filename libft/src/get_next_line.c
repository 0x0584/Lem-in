/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: melalj <melalj@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/30 00:25:50 by melalj            #+#    #+#             */
/*   Updated: 2019/11/05 17:07:52 by melalj           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"
#include "../libft.h"

static int		ft_read(char **str, int fd)
{
	int		ret;
	char	*s;
	char	buf[BUFF_SIZE + 1];

	if ((ret = read(fd, buf, BUFF_SIZE)) == -1)
		return (-1);
	buf[ret] = '\0';
	s = ft_strdup(*str);
	free(*str);
	*str = ft_joinwith(s, buf, "");
	free(s);
	return (ret);
}

static int		ft_get_line(char **str, char **line, char *s)
{
	int	i;

	i = 0;
	if (*s == '\n')
		i = 1;
	*s = 0;
	*line = NULL;
	free(*line);
	*line = ft_strdup(*str);
	if (i == 0 && ft_strlen(*str) != 0)
	{
		free(*str);
		*str = ft_strdup("");
		return (1);
	}
	else if (i == 0 && !(ft_strlen(*str)))
		return (0);
	free(*str);
	*str = NULL;
	return (i);
}

int				get_next_line(int const fd, char **line)
{
	int			ret;
	char		*s;
	static char	*str;

	if (str == 0)
		str = ft_strnew(0);
	if (!line || BUFF_SIZE < 1)
		return (-1);
	ret = BUFF_SIZE;
	while (line)
	{
		s = str;
		while (*s || ret < BUFF_SIZE)
		{
			if (*s == '\n' || *s == 0)
				return (ft_get_line(&str, line, s));
			s++;
		}
		ret = ft_read(&str, fd);
		if (ret == -1)
			return (-1);
	}
	return (0);
}
