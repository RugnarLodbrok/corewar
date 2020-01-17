CC = gcc
NAME = corewar
VM = corewar_vm
FLAGS = -Wall -Wextra -Werror

SRC_VM = \
	vm/vm_main.c \
	vm/vm.c \
	vm/proc.c \
	vm/operation.c \
	vm/commands.c \
	vm/loader.c \
	vm/endian_conversion.c \
	vm/state_writer.c \
	resources/op.c

OPTION = -Iheaders -Ilibft
OBJ_VM = $(SRC_VM:.c=.o)

all : $(VM)

$(VM) : libft/libft.a $(OBJ_VM)
	$(CC) -o $(VM) $(OBJ_VM) $(OPTION) -L libft/ -lft

%.o: %.c
	@echo compile $(<) "->" $(<:.c=.o)
	$(CC) $(FLAGS) -c $(<) -o $(<:.c=.o) $(OPTION)

libft/libft.a :
	make -C libft/

clean :
	rm -f $(OBJ_VM)
	make -C libft/ clean

fclean : clean
	rm -f $(VM)
	make -C libft/ fclean

re : fclean all
