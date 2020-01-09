#******************************************************************************#
#                                                                              #
#                                                         :::      ::::::::    #
#    init.mk                                            :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: archid- <archid-@student.1337.ma>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2020/01/07 04:27:17 by archid-           #+#    #+#              #
#    Updated: 2020/01/09 20:06:20 by archid-          ###   ########.fr        #
#                                                                              #
#******************************************************************************#

NAME		= lem-in
FARM		= barfar

VISU		?= 0
DEBUG		?= 0

CC			= gcc
CFLAGS		= -Wall -Wextra -g -Ilibft
LDFLAGS		= -Llibft -lft

ifeq ($(DEBUG),1)
	CFLAGS  += -DDEBUG
endif

ifeq ($(shell uname),Linux)
	SDL_LIB  = `sdl2-config --static-libs` -lSDL2_image
	SDL_DEPS = `sdl2-config --cflags`
else
	SDL_LIB  = -L ~/.brew/Cellar/sdl2/2.0.10/lib -lSDL2-2.0.0 -lSDL2	\
			   -L ~/.brew/Cellar/sdl2_image/2.0.5/lib -lSDL2_image		\
			   -lSDL2_image-2.0.0										\
			   -L ~/.brew/Cellar/sdl2_ttf/2.0.15/lib -lSDL2_ttf			\
			   -lSDL2_ttf-2.0.0
	SDL_DEPS =  -I ~/.brew/Cellar/sdl2_image/2.0.5/include/SDL2			\
			    -I ~/.brew/Cellar/sdl2/2.0.10/include/SDL2				\
			    -I ~/.brew/Cellar/sdl2_ttf/2.0.15/include/SDL2
endif

ifeq ($(VISU),1)
	CFLAGS	  += -DUSE_VISU $(SDL_DEPS)
	LDFLAGS   += $(SDL_LIB)
endif

info:
	@echo "make [all|clean|fclean|re|ft|check|distcheck]"

ft:
	@make -C libft

distcheck:
	uname -a
	$(CC) --version

check:
#	@make fclean
	ls -lR
	@make all VISU=$(VISU) DEBUG=$(DEBUG)
	./$(NAME) < $(FARM)

# echo "Exit status: $(?)"

.PHONY: ft init check distcheck info
