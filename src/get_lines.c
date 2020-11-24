/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_lines.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: melalj <melalj@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/12 22:08:00 by melalj            #+#    #+#             */
/*   Updated: 2020/11/22 20:02:48 by archid-          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../lem_in.h"
#include <fcntl.h>

int debug_fd;

int check_vertex(char *line) // func that check if the node is a valid node
{
	char **sline;
	int i;

	if (line && line[0] == '#' && line[1] != '#') // jumping the comments
		return (-1);
	else if (line && line[0] == '#' && line[1] == '#') // test the command is a start or end command if not its considerade as a command
	{
		if (ft_strequ(line + 2, "start"))
			return (V_SOURCE);
		if (ft_strequ(line + 2, "end"))
			return (V_SINK);
	}
	sline = ft_strsplit(line, ' ');
	i = -1;
	while (sline[++i])
	{
		if (!i && sline[0][0] == 'L')
			break ;
		if (i && !ft_isnumber(sline[i]))
			break ;
	}
	free_tab(sline);
	if (i != 3)
		return (0);
	return (1);
}

int check_edge(char *line)
{
	char **sline;
	int i;

	if (line && line[0] == '#')
		return (-1);
	sline = ft_strsplit(line, '-');
	i = -1;
	while (sline[++i])
	{
		// if (!ft_isnumber(sline[i]))
			//compare node names using set table (this condition is shit)
		//	break ;
		if (i == 1 && ft_strequ(sline[1], sline[0]))
			break ;
	}
	free_tab(sline);
	if (i != 2)
		return (0);
	return (1);
}

static t_parse	*add_pline(char *line, int type, int prop)
{
	t_parse	*p_lines;

	if (!(p_lines = (t_parse*)malloc(sizeof(t_parse))))
		return (NULL);
	p_lines->line = ft_strdup(line);
	p_lines->prop = prop;
	p_lines->type = type;
	p_lines->next = NULL;
	return (p_lines);
}

int		parse_line(t_parse **p_lines, int *type, int *prop)
{
	char *line;

	/* read_line(debug_fd, &line); */
	read_line(0, &line);
	if (ft_strequ(line, ""))
		*prop = -1;
	else if (!(*type) && ft_isnumber(line))
	{
		if (ft_atoi(line) <= 0)
		{
			ft_printf("number of ants is not valid\n");
			exit(1);
		}
		*p_lines = add_pline(line, *type, *prop);
		(*type)++;
	}
	else if ((*type) == 1 && ((*prop) = check_vertex(line)))
	{
		*p_lines = add_pline(line, *type, *prop);
	}
	else if (check_edge(line))
	{
		(*type) = ((*type) == 1) ? (*type) + 1 : (*type);
		*p_lines = add_pline(line, *type, *prop);
	}
	else // need to free the list
	{
		ft_printf("error in parsing the input\n");
		exit(1);
	}
	free(line);
	return ((*type == 1 && *prop == 1) ? 1 : 0);
}

t_parse	*get_lines(int *vertices_c)
{
	t_parse	*p_lines;
	t_parse	*current;
	int		type;
	int		prop;

	p_lines = NULL;
	type = 0;
	*vertices_c = 0;
	debug_fd = open("test_farm", O_RDONLY);
	while (!type)
		parse_line(&(current), &type, &prop);
	p_lines = current;
	while (p_lines && type >= 0)
	{
		*vertices_c += parse_line(&(current->next), &type, &prop);
		if (!current->next && prop != 0)
			break ;
		if (!current->next && prop == 0)
		{
			current = current->next;
			continue;
		}
		// ft_printf("line |%s| --- type : %d --- prop : %d\n",
		// current->next->line, type, prop);
		current = current->next;
	}
	close(debug_fd);
	return (p_lines);
}

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
