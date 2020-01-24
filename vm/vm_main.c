#include "libft.h"
#include "get_next_line.h"
#include "vm.h"

void main_loop(t_vm *vm, int dump)
{
	int status;
	char *line;
	int steps;

	if (vm->mode == MODE_DEFAULT)
	{
		while (!vm->shutdown)
			t_vm_step(vm);
	}
	if (vm->mode == MODE_VIS)
	{
		while ((status = get_next_line(STDIN_FILENO, &line)) > 0)
		{
			steps = ft_atoi(line);
			free(line);
			while (steps-- > 0)
			{
				if (vm->shutdown)
					return;
				t_vm_step(vm);
			}
			write_cycle(vm->i);
		}
		if (status < 0)
			ft_error_exit("can't read stdin");
	}
	if (vm->mode == MODE_PRINT)
	{
		t_vm_print(vm);
		while ((status = get_next_line(STDIN_FILENO, &line)))
		{
			steps = ft_atoi(line);
			free(line);
			if (steps == 0)
				steps = 1;
			while (steps-- > 0)
			{
				if (vm->shutdown)
					return;
				t_vm_step(vm);
				t_vm_print(vm);
			}
		}
		if (status < 0)
			ft_error_exit("can't read stdin");
	}
	if (vm->mode == MODE_DUMP)
	{
		while (dump-- > 0)
			t_vm_step(vm);
		t_vm_print(vm);
	}
}

int main(int ac, char **av)
{
	int i;
	t_vm vm;
	t_args args;

	parse_args(&args, ac, av);
	t_vm_init(&vm, ft_len((void **)&args.champs[0]), args.mode);
	i = -1;
	while (args.champs[++i])
		t_vm_add_champ(&vm, args.champs[i]);
	main_loop(&vm, args.dump);
	t_vm_destruct(&vm);
	if (vm.mode == MODE_VIS)
		write_end();
	return (0);
}
