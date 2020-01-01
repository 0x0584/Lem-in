# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: melalj <melalj@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2019/10/16 11:00:35 by melalj            #+#    #+#              #
#    Updated: 2020/01/01 23:14:39 by archid-          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

VISU	 ?= 0
DEBUG	 ?= 0

SRC_PATH = src
SRC_NAME = lem_in_main.c	\
			parser.c		\
			read_line.c		\
			hash_t.c		\
			get_lines.c		\
			queue.c			\
			bfs.c			\
			netflow.c

OBJ_PATH = obj
LDFLAGS  = -L libft -lft
DEPS	 = -I include -I libft/include
CC		 = gcc
CFLAGS   = -Wall -Wextra -g

ifeq ($(VISU),1)
	CFLAGS	+= -DUSE_VISU
	LDFLAGS += -L ~/.brew/Cellar/sdl2/2.0.10/lib -lSDL2-2.0.0 -lSDL2	\
			   -L ~/.brew/Cellar/sdl2_image/2.0.5/lib -lSDL2_image		\
			   -lSDL2_image-2.0.0										\
			   -L ~/.brew/Cellar/sdl2_ttf/2.0.15/lib -lSDL2_ttf			\
			   -lSDL2_ttf-2.0.0
	DEPS	+= -I ~/.brew/Cellar/sdl2_image/2.0.5/include/SDL2			\
			   -I ~/.brew/Cellar/sdl2/2.0.10/include/SDL2				\
			   -I ~/.brew/Cellar/sdl2_ttf/2.0.15/include/SDL2
	SRC_NAME += tools.c			\
				visu.c			\
				graph_draw.c	\
				edges_draw.c
endif

ifeq ($(DEBUG),1)
	CFLAGS += -DDEBUG
endif

OBJ_NAME = $(SRC_NAME:.c=.o)
OBJ		 = $(addprefix $(OBJ_PATH)/, $(OBJ_NAME))


NAME     = lem-in

.PHONY: all, clean, fclean, re

all: $(NAME)

$(NAME): $(OBJ)
	@cd libft ; make
	$(CC) $(CFLAGS) $^ -o $@ -Llibft -lft -Ilibft $(LDFLAGS)

$(OBJ_PATH)/%.o: $(SRC_PATH)/%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -o $@ -c $< -Ilibft $(DEPS)

clean:
	@make -C libft clean
	@rm -f $(OBJ)

#	@rmdir $(OBJ_PATH) 2> /dev/null || true

fclean: clean
	@make -C libft fclean
	@rm -f $(NAME)

re: fclean all

distcheck:
	uname -a
	$(CC) --version

check:
	@make fclean
	ls -lR
	@make
	./lem-in < barfarm && echo $?
