#******************************************************************************#
#                                                                              #
#                                                         :::      ::::::::    #
#    config.mk                                          :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: archid- <archid-@student.1337.ma>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2020/01/07 04:27:17 by archid-           #+#    #+#              #
#    Updated: 2020/12/03 20:01:41 by archid-          ###   ########.fr        #
#                                                                              #
#******************************************************************************#

FARM		= big_s

DEBUG		?= 1

CC			= gcc
LDFLAGS		= -Llibft -lft

ifeq ($(DEBUG),1)
	CFLAGS  += -g
else
	CFLAGS  += -Werror
endif

CFLAGS		+= -Wall -Wextra -Ilibft -Iinclude

info:
	@echo "make [all|clean|fclean|re|ft|check|distcheck|info]"

ft:
	@make -C libft DEBUG=$(DEBUG)

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
