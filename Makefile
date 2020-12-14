#******************************************************************************#
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: archid- <archid-@student.1337.ma>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2020/11/18 19:51:22 by archid-           #+#    #+#              #
#    Updated: 2020/12/14 22:42:32 by archid-          ###   ########.fr        #
#                                                                              #
#******************************************************************************#

include config.mk

INC_PATH	= include
INCLUDES	= $(shell find $(INC_PATH) -name '*.h' -type f)

SRC_PATH	= src
SRC_NAME	= lst.c graph.c bfs.c netflow.c main.c \
			  correction.c parser.c hash.c vizu.c

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

$(OBJ_PATH)/%.o: $(SRC_PATH)/%.c $(INCLUDES)
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
