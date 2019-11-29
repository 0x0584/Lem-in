/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: melalj <melalj@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/19 22:46:02 by melalj            #+#    #+#             */
/*   Updated: 2019/11/05 16:59:39 by melalj           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

static int	nbr_count(int n)
{
	int		tmp;
	int		len;

	len = 1;
	tmp = n;
	if (n < 0)
		len++;
	while (tmp != 0)
	{
		tmp /= 10;
		len++;
	}
	return (len);
}

char		*ft_itoa(int n)
{
	char	*nbr;
	int		i;

	if (n == 0 || n == -0)
		return (ft_strdup("0"));
	i = nbr_count(n);
	if (!(nbr = (char *)malloc((i) * sizeof(char))))
		return (0);
	nbr[--i] = '\0';
	if (n < 0)
	{
		nbr[0] = '-';
		while (i > 1)
		{
			nbr[--i] = '0' + (n % 10 * -1);
			n /= 10;
		}
	}
	else
		while (i > 0)
		{
			nbr[i-- - 1] = '0' + n % 10;
			n /= 10;
		}
	return (nbr);
}
