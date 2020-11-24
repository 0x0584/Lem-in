#******************************************************************************#
#                                                                              #
#                                                         :::      ::::::::    #
#    init.mk                                            :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: archid- <archid-@student.1337.ma>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2020/01/07 04:27:17 by archid-           #+#    #+#              #
#    Updated: 2020/11/18 11:07:51 by archid-          ###   ########.fr        #
#                                                                              #
#******************************************************************************#

NAME		= lem-in
FARM		= barfarm

DEBUG		?= 0

CC			= gcc
CFLAGS		= -Wall -Wextra -g -Ilibft -Iinclude
LDFLAGS		= -Llibft -lft

ifeq ($(DEBUG),1)
	CFLAGS  += -DDEBUG
endif

info:
	@echo "make [all|clean|fclean|re|ft|check|distcheck|info]"

ft:
	@make -C libft

distcheck:
	uname -a
	$(CC) --version

check:
	@make fclean
	ls -lR
	@make all
	./$(NAME) < $(FARM)

# echo "Exit status: $(?)"

.PHONY: ft init check distcheck info
