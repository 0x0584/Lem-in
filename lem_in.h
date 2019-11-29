/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lem_in.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: melalj <melalj@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/07 09:09:55 by melalj            #+#    #+#             */
/*   Updated: 2019/11/28 16:44:15 by melalj           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEMIN_H
# define LEMIN_H
# include "libft/libft.h"

typedef struct		s_node
{
	char			*name;
	struct s_edges	*edges;
	struct s_node	*next;
}					t_node;

typedef struct		s_edges
{
	struct s_node	*node_dst;
	struct s_node	*node_src;
	struct s_edges	*next;
}					t_edges;

typedef struct		s_parse
{
	char			*line;
	int				type;
	struct s_parse	*next;
}					t_parse;

void	read_line(int fd, char **line);
t_parse	*get_lines(int *nodes_c);
unsigned long	hash(unsigned char *str);
int		add_node(t_node **lst_node, t_parse *lines, int nodes_c);
t_node		**h_table(t_parse *lines, int nodes_c);
int	edges_fill(t_node **lst_node, t_parse *lines, int nodes_c);
#endif
