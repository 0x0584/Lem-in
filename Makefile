#******************************************************************************#
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: archid- <archid-@student.1337.ma>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2020/11/18 19:51:22 by archid-           #+#    #+#              #
#    Updated: 2020/11/25 05:09:05 by archid-          ###   ########.fr        #
#                                                                              #
#******************************************************************************#

include config.mk

SRC_PATH	= src
SRC_NAME	= queue.c graph.c bfs.c netflow.c lem_in_main.c \
			  rewire_utils.c rewire_info.c rewire.c parser.c hash.c

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
