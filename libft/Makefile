# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: melalj <melalj@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2019/10/16 11:00:35 by melalj            #+#    #+#              #
#    Updated: 2019/12/31 16:02:15 by archid-          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SRC_PATH = src
SRC_NAME = ft_printf.c\
interpreter.c\
format_extract.c\
conversions.c\
ft_printd.c\
conversion_s.c\
itoa_long.c\
itoa_unsi.c\
formating.c\
width_prec.c\
octa_unsi.c\
hexadeci.c\
cln_zero.c\
conversion_c.c\
conversion_f.c\
conversion_p.c\
conversion_pr.c\
math_abs.c\
math_stradd.c\
math_strpower.c\
math_strhit.c\
point_p.c\
precision_f.c\
rounding.c\
print_uo.c\
print_x.c\
print_x_tools.c\
repeat_char.c\
ft_atoi.c\
ft_bzero.c\
ft_free.c\
ft_isalnum.c\
ft_isalpha.c\
ft_isascii.c\
ft_isblank.c\
ft_isdigit.c\
ft_islower.c\
ft_isprint.c\
ft_isupper.c\
ft_isxdigit.c\
ft_itoa.c\
ft_itoa_base_u.c\
ft_lstadd.c\
ft_lstdel.c\
ft_lstdelone.c\
ft_lstiter.c\
ft_lstmap.c\
ft_lstnew.c\
ft_memalloc.c\
ft_memccpy.c\
ft_memchr.c\
ft_memcmp.c\
ft_memcpy.c\
ft_memdel.c\
ft_memmove.c\
ft_memset.c\
ft_putchar.c\
ft_putchar_fd.c\
ft_putendl.c\
ft_putendl_fd.c\
ft_putnbr.c\
ft_putnbr_fd.c\
ft_putstr.c\
ft_putstr_fd.c\
ft_strcat.c\
ft_strchr.c\
ft_strclr.c\
ft_strcmp.c\
ft_strcpy.c\
ft_strdel.c\
ft_strdup.c\
ft_strequ.c\
ft_striter.c\
ft_striteri.c\
ft_strjoin.c\
ft_strlcat.c\
ft_strlen.c\
ft_uitoa_base.c\
ft_strlowcase.c\
ft_strmap.c\
ft_strmapi.c\
ft_strncat.c\
ft_strncmp.c\
ft_strncpy.c\
ft_strnequ.c\
ft_strnew.c\
ft_strnstr.c\
ft_strrchr.c\
ft_strset.c\
ft_strsplit.c\
ft_strstr.c\
ft_strsub.c\
ft_strtrim.c\
ft_tolower.c\
ft_toupper.c\
my_bzero.c\
mystrcat.c\
ft_strndup.c\
ft_isnumber.c

OBJ_PATH = obj
OBJ_NAME = $(SRC_NAME:.c=.o)
OBJ = $(addprefix $(OBJ_PATH)/, $(OBJ_NAME))

CC = gcc
CFLAGS = -Wall -Werror -Wextra
NAME = libft.a
.PHONY: all, clean, fclean, re
all: $(NAME)
$(NAME): $(OBJ)
	ar rc $@ $^
$(OBJ_PATH)/%.o: $(SRC_PATH)/%.c
	@mkdir $(OBJ_PATH) 2> /dev/null || true
	$(CC) $(CFLAGS) -o $@ -c $<

clean:
	@rm -f $(OBJ)
	@rmdir $(OBJ_PATH) 2> /dev/null || true
fclean: clean
	@rm -f $(NAME)
re: fclean all
