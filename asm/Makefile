# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: cormund <cormund@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2019/12/23 17:26:57 by cormund           #+#    #+#              #
#    Updated: 2020/02/25 14:30:51 by cormund          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME := asm
DIR_LIBFT := ../libft/
LIBFT = $(DIR_LIBFT)libft.a
HEADERS := asm.h op.h
CC := gcc
CFLAGS := -Wall -Wextra -Werror -O3
REMOVE := /bin/rm -rf
DIR_INCLUDE := -Iinclude -I../libft
DIR_SRC := src/

vpath %.c $(DIR_SRC)
vpath %.o $(DIR_SRC)
vpath %.h $(DIR_INCLUDE)

SRC_NAME := src/asm.c \
			src/read_input.c \
			src/parser.c \
			src/error_manager.c \
			src/op.c \
			src/parsing_header.c \
			src/parsing_args.c \
			src/parsing_oper.c \
			src/parsing_labels.c \
			src/replace_args2numbers.c \
			src/translator.c \
			src/options.c \
			src/cleanup.c \
			src/dissasembler.c

OBJ_NAME = $(SRC_NAME:.c=.o)

all : $(NAME)

$(NAME) : $(LIBFT) $(OBJ_NAME)
	@$(CC) $(CFLAGS) $(OBJ_NAME) -lft -L $(DIR_LIBFT) -o $@
	@printf "\r\e[J\e[32m$@\e[0m done!\n\e[?25h"

%.o : %.c
	@$(CC) $(CFLAGS) -c $(<) $(DIR_INCLUDE) -o $(@)
	@printf "\r\e[?25l\e[Jcompiling \e[32m$(notdir $<)\e[0m"

$(LIBFT):
	@$(MAKE) -C $(DIR_LIBFT)
	@$(MAKE) -C $(DIR_LIBFT) clean

clean:
	@rm -f $(OBJ_NAME)
	@$(MAKE) -C $(DIR_LIBFT) fclean

fclean: clean
	@$(REMOVE) $(NAME)

re: fclean all
