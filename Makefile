# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: melalj <melalj@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2019/10/16 11:00:35 by melalj            #+#    #+#              #
#    Updated: 2020/01/07 04:55:31 by archid-          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

include init.mk

NAME		= lem-in

SRC_PATH	= src
SRC_NAME	= parser.c read_line.c hash_t.c get_lines.c	\
			  queue.c graph.c bfs.c netflow.c lem_in_main.c

ifeq ($(VISU),1)
	SRC_NAME += tools.c visu.c graph_draw.c edges_draw.c
endif

OBJ_PATH	= .obj
OBJ_NAME	= $(SRC_NAME:.c=.o)
OBJ			= $(addprefix $(OBJ_PATH)/, $(OBJ_NAME))

all: init $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

$(OBJ_PATH)/%.o: $(SRC_PATH)/%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -o $@ -c $< $(DEPS)

clean:
	@make -C libft clean
	@rm -f $(OBJ)

fclean:
	@make -C libft fclean
	@rm -f $(NAME) $(OBJ)

re: fclean all

.PHONY: all, clean, fclean, re
