/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   conversion_f.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: melalj <melalj@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/19 03:21:20 by melalj            #+#    #+#             */
/*   Updated: 2019/10/27 20:37:48 by melalj           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../ft_printf.h"

static char	*unusual_f(t_double a, char *result)
{
	char *dump;
	char *dumpp;

	if (a.t_bf.mantissa == 0)
		return (ft_strdup("0"));
	result = ft_unsigned(a.t_bf.mantissa);
	dump = ft_itoa(math_abs(1 - 16383 - 63));
	dumpp = math_strpower("5", dump);
	free(dump);
	dump = math_strhit(result, dumpp);
	free(result);
	free(dumpp);
	return (dump);
}

static char	*usual_f(t_double a, char *result)
{
	char *dump;
	char *dumpp;

	dump = math_strpower("2", "63");
	dumpp = ft_unsigned(a.t_bf.mantissa);
	result = math_stradd(dump, dumpp);
	free(dump);
	free(dumpp);
	dump = ft_itoa(math_abs(a.t_bf.expo - 16383 - 63));
	if ((a.t_bf.expo - 16383 - 63) >= 0)
		dumpp = math_strpower("2", dump);
	else
		dumpp = math_strpower("5", dump);
	free(dump);
	dump = math_strhit(result, dumpp);
	free(dumpp);
	free(result);
	return (dump);
}

static char	*extreme_val(t_format format, t_double a)
{
	char *s;
	char *dump;

	dump = NULL;
	if (a.t_bf.mantissa > 0)
		dump = ft_strdup("nan");
	else if (a.t_bf.mantissa == 0)
	{
		if (a.t_bf.sign)
			dump = ft_strdup("-inf");
		else
			dump = (format.flags[PLUS])
				? ft_strjoin("+", "inf") : ft_strdup("inf");
		s = (format.flags[SPACE] && !format.flags[PLUS] && !a.t_bf.sign)
				? ft_strjoin(" ", dump) : ft_strdup(dump);
		free(dump);
		dump = s;
	}
	format.flags[ZERO] = 0;
	s = formating(format, dump, 0);
	free(dump);
	return (s);
}

static char	*flag_plus(t_format format, t_double a, char *result)
{
	char *s;
	char *dump;

	if (format.width && format.flags[ZERO])
	{
		format.width--;
		result = formating(format, result, 1);
	}
	if (format.flags[PLUS])
		s = (a.t_bf.sign) ? ft_strjoin("-", result) : ft_strjoin("+", result);
	else
		s = (a.t_bf.sign) ? ft_strjoin("-", result) : ft_strdup(result);
	dump = (format.flags[SPACE] && !format.flags[PLUS]
		&& !a.t_bf.sign) ? ft_strjoin(" ", s) : ft_strdup(s);
	free(s);
	return (dump);
}

char		*conversion_f(t_format *format)
{
	t_double	a;
	char		*result;
	char		*dump;
	char		*dumpp;

	format->precision = (format->precision == -1) ? 6 : format->precision;
	if (!(dumpp = NULL) && format->length == 'L')
		a.ld = va_arg(format->ap, long double);
	else
		a.ld = va_arg(format->ap, double);
	if (!(result = NULL) && a.t_bf.expo == 0)
		dump = unusual_f(a, result);
	else if (a.t_bf.expo == 32767)
		return (extreme_val(*format, a));
	else
		dump = usual_f(a, result);
	rounding(*format, &dump, a.t_bf.expo - 16383 - 63);
	dumpp = cln_zero(dump);
	result = free_sl(dump) ? point_p(*format, dumpp, a.t_bf.expo - 16446) : 0;
	dump = free_sl(dumpp) ? flag_plus(*format, a, result) : 0;
	free(result);
	result = formating(*format, dump, 1);
	free(dump);
	return (result);
}
