# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: melalj <melalj@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2019/10/16 11:00:35 by melalj            #+#    #+#              #
#    Updated: 2019/12/04 04:00:21 by archid-          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SRC_PATH = src
SRC_NAME = lem_in_main.c\
			parser.c\
			read_line.c\
			hash_t.c\
			get_lines.c\
			queue.c\
			bfs.c

OBJ_PATH = obj

OBJ_NAME = $(SRC_NAME:.c=.o)
OBJ = $(addprefix $(OBJ_PATH)/, $(OBJ_NAME))
LDFLIBS = -L libft -lft


CC = gcc
CFLAGS = -Wall -Wextra -g
NAME = lem-in
.PHONY: all, clean, fclean, re
all: $(NAME)
$(NAME): $(OBJ)
	@cd libft ; make
	$(CC) $(LDFLIBS) $^ -o $@
$(OBJ_PATH)/%.o: $(SRC_PATH)/%.c
	@mkdir $(@D)
	$(CC) $(CFLAGS) -o $@ -c $< -Ilibft -Llibft -lft
clean:
	@make -C libft clean
	@rm -vf $(OBJ)
	@rmdir $(OBJ_PATH) 2> /dev/null || true
fclean: clean
	@make -C libft fclean
	@rm -vf $(NAME)
re: fclean all
