/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: melalj <melalj@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/16 15:19:56 by melalj            #+#    #+#             */
/*   Updated: 2019/11/18 03:56:46 by melalj           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H

# include <stdarg.h>
# include "libft.h"
# define MINUS 0
# define PLUS 1
# define SPACE 2
# define HASH 3
# define ZERO 4
# define LEN 0
# define S1 1
# define S2 2
# define REST 3

# define DEFAULT    -1

# define L_H        104
# define L_HH       72
# define L_L        108
# define L_LL       40

typedef struct				s_format
{
	char					flags[5];
	int						width;
	int						precision;
	char					length;
	char					specifier;
	va_list					ap;
	int						c;
}							t_format;

typedef union				u_udouble {
	long double				ld;
	double					d;
	unsigned long			u;
	struct					s_bf
	{
		unsigned long long	mantissa:63;
		int					int_p:1;
		unsigned int		expo:15;
		unsigned int		sign:1;
	}						t_bf;
}							t_double;

int							ft_printf(const char *format, ...);
char						*interprete(t_format *format,
								const char *s, int *count);
t_format					flags_extract(t_format format,
								const char *s, int len);
t_format					width_extract(t_format format,
								const char *s, int len);
t_format					precision_extract(t_format format,
								const char *s, int len);
t_format					length_extract(t_format format,
								const char *s, int len);
t_format					specifier_extract(t_format format,
								const char *s, int len);
char						*ft_long(long long int n);
char						*ft_unsigned(unsigned long long int n);
char						*ft_printd(t_format *all);
char						*conversion_check(t_format *format);
char						*conversion_s(t_format *format);
char						*formating(t_format format, char *sa, int a);
char						*octa(unsigned long long int o);
void						ft_precision(char **res, t_format all);
void						ft_width(char **res, t_format all, int x);
char						*hexa(unsigned long long int i, int a);
void						width_formating(t_format format, char **str);
void						precision_formating(t_format format,
								char **str, int a);
char						*conversion_c(t_format *format);
char						*conversion_p(t_format *format);
char						*conversion_pr(t_format *format);
char						*conversion_f(t_format *format);
void						my_bzero(void *s, size_t n);
char						*math_stradd(char *s1, char *s2);
int							math_abs(int nbr);
char						*cln_zero(char *str);
char						*math_strhit(char *s1, char *times);
char						*math_strpower(char *base, char *power);
char						*point_p(t_format format, char *str, int power);
void						rounding(t_format format, char **str, int p_pos);
char						*ft_trimmer(char	*str);
int							print_uo(t_format *format);
int							repeate_char(char c, int i);
int							put_x(int *params, t_format *fs,
								int add_zeros, char *s);
int							put_trailing_spaces(t_format *fs,
								int condition, int comp_target);
int							set_x_right_prefix(int val, t_format *fs,
								int comp_target, int dp);
int							print_x(t_format *fs);
#endif
