/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_lines.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: melalj <melalj@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/12 22:08:00 by melalj            #+#    #+#             */
/*   Updated: 2019/11/29 15:56:23 by melalj           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../lem_in.h"

int check_node(char *line)
{
	char **sline;
	int i;

	if (line && line[0] == '#' && line[1] != '#')
		return (-1);
	else if (line && line[0] == '#' && line[1] == '#')
	{
		if (ft_strequ(line + 2, "start"))
			return (NODE_START);
		if (ft_strequ(line + 2, "end"))
			return (NODE_END);
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
		if (!ft_isnumber(sline[i])) //compare node names using hash table (this condition is shit)
			break ;
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

	read_line(0, &line);
	if (ft_strequ(line, ""))
		*prop = -1;
	else if (!(*type) && ft_isnumber(line))
	{
		*p_lines = add_pline(line, *type, *prop);
		(*type)++;
	}
	else if ((*type) == 1 && ((*prop) = check_node(line)))
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

t_parse	*get_lines(int *nodes_c)
{
	t_parse	*p_lines;
	t_parse	*current;
	int		type;
	int		prop;

	p_lines = NULL;
	type = 0;
	*nodes_c = 0;
	parse_line(&(current), &type, &prop);
	p_lines = current;
	while (p_lines && type >= 0)
	{
		*nodes_c += parse_line(&(current->next), &type, &prop);
		if (!current->next && prop != 0)
			break ;
		if (!current->next && prop == 0)
			continue;
		// ft_printf("line |%s| --- type : %d --- prop : %d\n", current->next->line, type, prop);
		current = current->next;
	}
	return (p_lines);
}

