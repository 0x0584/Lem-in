/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa_base_u.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: melalj <melalj@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/08 17:44:26 by aihya             #+#    #+#             */
/*   Updated: 2019/11/05 17:08:56 by melalj           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"
#include "../libft.h"
#include "../libft.h"
#include "../libft.h"

#define UPPER_CASE	"0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ"
#define LOWER_CASE	"0123456789abcdefghijklmnopqrstuvwxyz"

static char	*convert_to_base(unsigned long long value, unsigned base, int cap)
{
	int					i;
	char				buffer[66];
	unsigned long long	n;
	char				*base_conv_chars;

	base_conv_chars = ft_strdup(cap == 1 ? UPPER_CASE : LOWER_CASE);
	n = value;
	i = 0;
	while (n >= base)
	{
		buffer[i] = base_conv_chars[n % base];
		n = n / base;
		i++;
	}
	buffer[i++] = base_conv_chars[n];
	buffer[i] = '\0';
	free(base_conv_chars);
	return (ft_strdup(buffer));
}

char		*ft_itoa_base_u(unsigned long long value, int base, int cap)
{
	char	*res;
	char	*buffer;
	int		len;
	int		j;

	buffer = convert_to_base(value, (unsigned)base, cap);
	len = (int)ft_strlen(buffer);
	res = (char *)malloc(sizeof(char) * (len + 1));
	len--;
	j = 0;
	while (len >= 0)
	{
		res[j] = buffer[len];
		len--;
		j++;
	}
	free(buffer);
	res[j] = '\0';
	return (res);
}
