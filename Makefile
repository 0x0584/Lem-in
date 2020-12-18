#******************************************************************************#
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: archid- <archid-@student.1337.ma>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2020/11/18 19:51:22 by archid-           #+#    #+#              #
#    Updated: 2020/12/18 12:11:52 by archid-          ###   ########.fr        #
#                                                                              #
#******************************************************************************#

include config.mk

INC_PATH	= include
INCLUDES	= $(shell find $(INC_PATH) -name '*.h' -type f)

SRC_PATH	= src
SRCS		= $(shell find $(SRC_PATH) -name '*.c' -type f)

OBJ_PATH	= .objs
OBJS		= $(patsubst $(SRC_PATH)/%.c, $(OBJ_PATH)/%.o, $(SRCS))

ifeq ($(DEBUG),1)
	NAME	= $(OBJ_PATH)/lem-in.$(shell uname)
else
	NAME    = lem-in
endif

all: ft $(NAME)
ifeq ($(DEBUG),1)
	@ln -sf $(NAME) lem-in
endif

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

$(OBJ_PATH)/%.o: $(SRC_PATH)/%.c $(INCLUDES)
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -o $@ -c $<

clean:
	@make -C libft clean
	@rm -f $(OBJS)

fclean:
	@make -C libft fclean
	@rm -f $(NAME) $(OBJS) lem-in

re: fclean all

.PHONY: all $(NAME) clean fclean re
