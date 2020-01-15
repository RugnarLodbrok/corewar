#include "libft.h"
#include "get_next_line.h"
#include "vm.h"

void main_loop(t_vm* vm)
{
	int status;
	char* line;
	int steps;

	if (vm->mode == MODE_DEFAULT)
	{
		while (!vm->shutdown)
			t_vm_step(vm);
	}
	if (vm->mode == MODE_VIS)
	{
		while ((status = get_next_line(STDIN_FILENO, &line)))
		{
			steps = ft_atoi(line);
			free(line);
			while (steps-- > 0)
			{
				if (vm->shutdown)
					return;
				t_vm_step(vm);
			}
		}
		if (status < 0)
			ft_error_exit("can't read stdin");
	}
}

int main(int ac, char** av)
{
	int i;
	t_vm vm;
	int n_champs;

	n_champs = ac - 1;
	t_vm_init(&vm, n_champs);
	i = 1;
	while (i < ac)
	{
		if (!ft_strcmp("-v", av[i]))
			vm.mode = MODE_VIS;
		++i;
	}
	write_memory(&vm);
	i = 1;
	while (i < ac)
	{
		if (!ft_startswith(av[i], "-"))
			t_vm_add_champ(&vm, (const char*)av[i]);
		++i;
	}
	main_loop(&vm);
	t_vm_destruct(&vm);
	if (vm.mode == MODE_VIS)
		write_end();
	return (0);
}
