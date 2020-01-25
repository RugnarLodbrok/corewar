CC = gcc
NAME = corewar
FLAGS = -Wall -Wextra -Werror

SRC_VM = \
	vm/parse_args.c \
	vm/vm_main.c \
	vm/vm.c \
	vm/proc.c \
	vm/operation.c \
	vm/commands.c \
	vm/loader.c \
	vm/endian_conversion.c \
	vm/state_writer.c \
	vm/op.c

OPTION = -Iheaders -Ilibft
OBJ_VM = $(SRC_VM:.c=.o)

all : $(NAME)
	make -C asm/ all

vm : $(NAME)

$(NAME) : libft/libft.a $(OBJ_VM)
	$(CC) -o $(NAME) $(OBJ_VM) $(OPTION) -L libft/ -lft

%.o: %.c headers/vm.h headers/op.h headers/ops.h
	@echo compile $(<) "->" $(<:.c=.o)
	$(CC) $(FLAGS) -c $(<) -o $(<:.c=.o) $(OPTION)

libft/libft.a :
	make -C libft/

clean :
	rm -f $(OBJ_VM)
	make -C libft/ clean
	make -C asm/ clean

fclean : clean
	rm -f $(NAME)
	make -C libft/ fclean
	make -C asm/ fclean

re : fclean all
