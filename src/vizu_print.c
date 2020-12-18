/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vizu_print.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: archid- <archid-@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/18 14:07:03 by archid-           #+#    #+#             */
/*   Updated: 2020/12/18 18:31:15 by archid-          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "visu.h"

void		flow_print(void *pflow)
{
	size_t i;
	t_flow flow;

	if (!(flow = pflow))
		return ;
	i = 0;
	ft_putstr(" ");
	while (i < flow->size)
	{
		if (flow->stage[i].unit != (size_t)NIL_ANT)
			ft_printf(" | %{magenta_fg}%5s: %{italic}%{yellow_fg}%4zu%{reset}",
						flow->stage[i].vertex, flow->stage[i].unit);
		else
			ft_printf(" | %{bold} *no ant*	%{reset}");
		i++;
	}
	ft_putendl(" |");
}

void		print_top_bar(size_t size)
{
	size_t i;

	i = 0;
	ft_putstr("  ");
	while (i < size)
	{
		ft_printf(BORDER_COLOR "+-------");
		i++;
	}
	ft_printf("+" DEFAULT_COLOR "\n ");
}

void		flow_ascii(void *pflow)
{
	size_t i;
	t_flow flow;

	if (!(flow = pflow))
		return ;
	print_top_bar(flow->size);
	i = 0;
	while (i < flow->size)
	{
		if (flow->stage[i].unit != (size_t)NIL_ANT)
			ft_printf(BORDER_COLOR " | " DEFAULT_COLOR UNIT_CELL_COLOR "%-4zu ",
						flow->stage[i].unit);
		else
			ft_printf(BORDER_COLOR " | " EMPTY_CELL_COLOR " /// ");
		i++;
	}
	ft_printf(BORDER_COLOR " | " DEFAULT_COLOR "\n");
	g_last_flow = flow->size;
}

void		print_flows_ascii(t_lst flows)
{
	ft_putendl("");
	lst_iter(flows, true, flow_ascii);
	print_top_bar(g_last_flow);
}

void		flow_out(t_flow flow)
{
	size_t i;

	i = flow->size;
	while (i--)
		if (flow->stage[i].unit != (size_t)NIL_ANT)
			ft_printf("L%zu-%s ", flow->stage[i].unit, flow->stage[i].vertex);
}
