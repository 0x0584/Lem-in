/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rounding.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: melalj <melalj@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/03 18:22:02 by melalj            #+#    #+#             */
/*   Updated: 2019/11/05 17:07:15 by melalj           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../ft_printf.h"

static void	mm_bzero(char **s, char c, int len)
{
	while (--len >= 0)
		(*s)[len] = c;
}

void		rounding(t_format fs, char **s, int p_pos)
{
	int		i;
	char	*rd;
	int		len;
	char	*dump;

	len = ft_strlen(*s);
	if (p_pos > 0 || (len - (i = len + p_pos + fs.precision - 1) < 0 || i < 0))
		return ;
	rd = ft_strnew(len - i);
	mm_bzero(&rd, '0', len - i);
	rd[0] = '1';
	if ((*s)[i + 1] > '5' && (dump = math_stradd((*s), rd)) && free_sl(*s))
		*s = dump;
	else if ((*s)[i + 1] == '5')
	{
		if (((*s)[i] - 48) % 2)
			dump = math_stradd((*s), rd);
		else if (ft_atoi(&((*s)[i + 2])))
			dump = math_stradd((*s), rd);
		else
			dump = ft_strdup(*s);
		free(*s);
		*s = dump;
	}
	free(rd);
}
