# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: melalj <melalj@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2019/10/16 11:00:35 by melalj            #+#    #+#              #
#    Updated: 2020/11/14 18:25:46 by archid-          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

include init.mk

SRC_PATH	= src
SRC_NAME	= parser.c read_line.c hash_t.c get_lines.c	\
			  queue.c graph.c bfs.c netflow.c lem_in_main.c \
			  rewire_utils.c rewire_info.c rewire.c

ifeq ($(VISU),1)
	SRC_NAME += tools.c visu.c graph_draw.c edges_draw.c
endif

OBJ_PATH	= .obj
OBJ_NAME	:= $(SRC_NAME:.c=.o)
OBJ			:= $(addprefix $(OBJ_PATH)/, $(OBJ_NAME))

all: ft $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

$(OBJ_PATH)/%.o: $(SRC_PATH)/%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -o $@ -c $<

clean:
	@make -C libft clean
	@rm -f $(OBJ)

fclean:
	@make -C libft fclean
	@rm -f $(NAME) $(OBJ)

re: fclean all

.PHONY: all $(NAME) clean fclean re
