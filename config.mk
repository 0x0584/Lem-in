#******************************************************************************#
#                                                                              #
#                                                         :::      ::::::::    #
#    config.mk                                          :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: archid- <archid-@student.1337.ma>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2020/01/07 04:27:17 by archid-           #+#    #+#              #
#    Updated: 2020/12/18 12:30:10 by archid-          ###   ########.fr        #
#                                                                              #
#******************************************************************************#

FARM		= crossmap
DEBUG		?= 1

CC			= gcc
LDFLAGS		= -Llibft -lft

ifeq ($(DEBUG),1)
	CFLAGS  = -g
else
	CFLAGS  = -Werror
endif

CFLAGS		+= -Wall -Wextra -Ilibft -Iinclude

default: all

info:
	@echo "make [all|clean|fclean|re|ft|check|distcheck|info]"

ft:
	@make -C libft DEBUG=$(DEBUG)

distcheck:
	@uname -a
	@echo CC: $(CC)
	@echo CFLAGS: $(CFLAGS)
	@echo LDFLAGS: $(LDFLAGS)
	@echo
	@$(CC) --version

check:
	@make all
	@./test2.sh

.PHONY: ft init check distcheck info default
