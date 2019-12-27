# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: melalj <melalj@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2019/10/16 11:00:35 by melalj            #+#    #+#              #
#    Updated: 2019/12/27 19:17:27 by archid-          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SRC_PATH = src
SRC_FILES = $(shell find $(SRC_PATH) -name '*.c' -type f) 

OBJ_PATH = .obj

OBJS := $(patsubst $(SRC_PATH)/%.c, $(OBJ_PATH)/%.o, $(SRC_FILES))
#OBJ = $(addprefix $(OBJ_PATH)/, $(OBJ_NAME))
LDFLIBS = -L libft -lft

CC = gcc
CFLAGS = -Wall -Wextra -g
NAME = lem-in

.PHONY: all, clean, fclean, re

all: $(NAME)

$(NAME): $(OBJS)
	@cd libft ; make
	$(CC) $(LDFLIBS) $^ -o $@ -Llibft -lft -Ilibft

$(OBJ_PATH)/%.o: $(SRC_PATH)/%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -o $@ -c $< -Ilibft

clean:
	@make -C libft clean
	@rm -vf $(OBJ)

#	@rmdir $(OBJ_PATH) 2> /dev/null || true
fclean: clean
	@make -C libft fclean
	@rm -vf $(NAME)
re: fclean all

distcheck:
	uname -a
	$(CC) --version
check:
	@make fclean
	ls -lR
	@make
	./lem-in && echo $?
	
