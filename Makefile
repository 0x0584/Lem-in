#******************************************************************************#
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: archid- <archid-@student.1337.ma>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2020/11/18 19:51:22 by archid-           #+#    #+#              #
#    Updated: 2020/12/03 20:21:00 by archid-          ###   ########.fr        #
#                                                                              #
#******************************************************************************#

include config.mk

SRC_PATH	= src
SRC_NAME	= queue.c graph.c bfs.c netflow.c lem_in_main.c \
			  rewire_utils.c rewire_info.c rewire.c parser.c hash.c
OBJ_PATH	= .obj
OBJ_NAME	:= $(SRC_NAME:.c=.o)
OBJ			:= $(addprefix $(OBJ_PATH)/, $(OBJ_NAME))

ifeq ($(DEBUG),1)
	NAME	= $(OBJ_PATH)/lem-in.$(shell uname)
else
	NAME    = lem-in
endif

all: ft $(NAME)
ifeq ($(DEBUG),1)
	@ln -sf $(NAME) lem-in
endif

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
	@rm -f $(NAME) $(OBJ) lem-in

re: fclean all

.PHONY: all $(NAME) clean fclean re
