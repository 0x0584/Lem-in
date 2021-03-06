/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: archid- <archid-@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/20 01:02:06 by archid-           #+#    #+#             */
/*   Updated: 2020/12/19 13:19:44 by archid-          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

int		g_ants = 0;
int		g_state = 0;
char	*g_error_line = NULL;

bool		ft_isnumber(char *s)
{
	char	*ptr;

	ptr = s;
	while (*ptr)
		if (ptr - s > 10 || !ft_isdigit(*ptr++))
			return (false);
	return (ft_atoi(s) == ft_atol(s));
}

bool		error_message(char *line)
{
	if (g_verbose)
		ft_asprintf(&g_error_line, ": `%s` is not valid", line);
	ft_dprintf(2, "ERROR%s\n", g_error_line ? g_error_line : "");
	ft_strchange(&g_error_line, NULL);
	return (false);
}

void		print_line(void *line)
{
	ft_printf("%s\n", line);
}

void		print_map(t_lst lverts, t_lst ledges)
{
	ft_printf("%d\n", g_ants);
	lst_iter(lverts, true, print_line);
	lst_iter(ledges, true, print_line);
}

t_graph		read_graph(void)
{
	t_lst	verts;
	t_lst	edges;
	t_graph g;
	char	*line;
	bool	valid;

	valid = true;
	verts = lst_alloc(blob_free);
	edges = lst_alloc(blob_free);
	while (valid && gnl(0, &line) > 0 && *line)
		if (valid_comment(line))
			free(line);
		else if (!valid_line(line, verts, edges))
			valid = false;
	free(line);
	gnl_cleanup();
	g = (valid ? parse_graph(verts, edges) : NULL);
	lst_del(&verts);
	lst_del(&edges);
	return (g);
}
