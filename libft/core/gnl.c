/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: archid- <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/11 18:01:11 by archid-           #+#    #+#             */
/*   Updated: 2020/11/25 03:59:18 by archid-          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	*g_cache[0xFF] = {NULL};

static t_state	extract_nl_line(char **cache, char **line)
{
	char *nl;
	char *old_cache;

	if (!cache || !*cache || !line)
		return (failure);
	old_cache = *cache;
	if (!(nl = ft_strchr(*cache, '\n')))
		return (eof);
	*nl = '\0';
	if (!(*line = ft_strdup(*cache)))
		return (failure);
	if (!(*cache = ft_strdup(nl + 1)))
		return (failure);
	free(old_cache);
	return (success);
}

static ssize_t	cached_read(const int fd, char **cache)
{
	char	*buff;
	char	*old_cache;
	ssize_t	nbytes;

	if (!(buff = ALLOC(char *, 1, BUFF_SIZE + 1)))
		return (failure);
	while ((nbytes = read(fd, buff, BUFF_SIZE)) > 0)
	{
		buff[nbytes] = '\0';
		if (*cache == NULL)
			*cache = ft_strdup(buff);
		else
		{
			old_cache = *cache;
			*cache = ft_strjoin(old_cache, buff);
			free(old_cache);
		}
		if (ft_strchr(*cache, '\n'))
			break ;
	}
	ft_strdel(&buff);
	return (nbytes);
}

int				gnl(const int fd, char **line)
{
	ssize_t		nbytes;

	if (!line || fd < 0 || BUFF_SIZE <= 0 || read(fd, NULL, 0) < 0)
		return (failure);
	if (g_cache[fd] && extract_nl_line(&g_cache[fd], line) > 0)
		return (success);
	*line = NULL;
	if ((nbytes = cached_read(fd, &g_cache[fd])) < 0)
		return (failure);
	if (nbytes == 0 && g_cache[fd])
	{
		*line = ft_strdup(g_cache[fd]);
		ft_strdel(&g_cache[fd]);
		return ((*line && ft_strlen(*line)) ? success : eof);
	}
	return (extract_nl_line(&g_cache[fd], line) > 0);
}

void gnl_free_cache(void)
{
	int i = 0;

	while (i < 0xFF)
		free(g_cache[i++]);
}
