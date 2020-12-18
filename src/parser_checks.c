/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_checks.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: archid- <archid-@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/18 13:05:47 by archid-           #+#    #+#             */
/*   Updated: 2020/12/18 13:06:45 by archid-          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

bool		valid_vertex_char(char c)
{
	return (c && c != ' ' && c != '-');
}

bool		valid_edge_line(char *line)
{
	int i;

	i = 0;
	if (*line == 'L' || (*line == '#' && *(line + 1) == '#'))
		return (false);
	while (valid_vertex_char(line[i]))
		i++;
	if (!line[i] || line[i] == ' ')
		return (false);
	i++;
	while (valid_vertex_char(line[i]))
		i++;
	return (line[i] == '\0');
}

bool		valid_vertex_line(char *line)
{
	int i;

	if (*line == '#' && *(line + 1) == '#')
		return (true);
	if (*line == 'L')
		return (false);
	i = 0;
	while (valid_vertex_char(line[i]))
		i++;
	if (!line[i] || line[i] == '-')
		return (false);
	i++;
	while (line[i] && ft_isdigit(line[i]))
		i++;
	if (line[i] != ' ')
		return (false);
	i++;
	while (line[i] && ft_isdigit(line[i]))
		i++;
	return (line[i] == '\0');
}

int			valid_comment(char *line)
{
	return (*line == '#' && *(line + 1) != '#');
}

bool		valid_line(char *line, t_lst verts, t_lst edges)
{
	if (g_state == 0 && ft_isnumber(line))
	{
		g_ants = ft_atoi(line);
		free(line);
		if (g_ants <= 0)
			return (error_message("number of ants"));
		g_state++;
		return (true);
	}
	if (g_state == 1 && valid_vertex_line(line))
	{
		lst_push_back_blob(verts, line, sizeof(char *), false);
		return (true);
	}
	if (valid_edge_line(line))
	{
		g_state++;
		lst_push_back_blob(edges, line, sizeof(char *), false);
		return (true);
	}
	return (error_message(line));
}
