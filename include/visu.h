/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   visu.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: archid- <archid-@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/18 13:44:45 by archid-           #+#    #+#             */
/*   Updated: 2020/12/18 14:16:40 by archid-          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef VISU_H
# define VISU_H

# include <sys/ioctl.h>
# include <sys/stat.h>

# include "network.h"

# define DEFAULT_COLOR			"%{reset}"
# define BORDER_COLOR			"%{bold}%{magenta_fg}"
# define EMPTY_CELL_COLOR		"%{blue_fg}"
# define UNIT_CELL_COLOR		"%{yellow_fg}"

void			show(t_network net, size_t maxflow);

void			flow_print(void *pflow);
void			print_top_bar(size_t size);
void			flow_ascii(void *pflow);
void			print_flows_ascii(t_lst flows);
void			flow_out(t_flow flow);

extern size_t	g_last_flow;

extern bool		g_visualize;
extern bool		g_output_maxflow;
extern bool		g_output_flow_info;
extern bool		g_output_ant_lines;

#endif
