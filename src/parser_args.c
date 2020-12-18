/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_args.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: archid- <archid-@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/18 18:03:05 by archid-           #+#    #+#             */
/*   Updated: 2020/12/18 18:26:50 by archid-          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

bool	g_verbose = false;
bool	g_visualize = false;
bool	g_output_maxflow = false;
bool	g_output_info = false;
bool	g_output_ant_lines = false;
bool	g_json_output = false;

void	parse_args(int argc, char *argv[])
{
	int i;

	i = 1;
	while (i < argc)
	{
		if (!ft_strcmp(argv[i], "-V"))
			g_verbose = true;
		else if (!ft_strcmp(argv[i], "-v"))
			g_visualize = true;
		else if (!ft_strcmp(argv[i], "-m"))
			g_output_maxflow = true;
		else if (!ft_strcmp(argv[i], "-i"))
			g_output_info = true;
		else if (!ft_strcmp(argv[i], "-a"))
			g_output_ant_lines = true;
		else if (!ft_strcmp(argv[i], "-j"))
			g_json_output = true;
		i++;
	}
}
