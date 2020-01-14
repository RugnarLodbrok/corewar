#include "libft.h"
#include "vm.h"

uint read_uint(int host_endian, byte *mem, byte len);

int main(int ac, char **av)
{
	int i;
	t_vm vm;
	int n_champs;

	n_champs = ac - 1;
	t_vm_init(&vm, n_champs);
	i = 1;
	while(i < ac)
	{
		if (!ft_strcmp("-v", av[i]))
			vm.mode = MODE_VIS;
		++i;
	}
	write_memory(&vm);
	i = 1;
	while(i < ac)
	{
		if (!ft_startswith(av[i], "-"))
			t_vm_add_champ(&vm, (const char *)av[i]);
		++i;
	}
	while (!vm.shutdown)
		t_vm_step(&vm);
	t_vm_destruct(&vm);
	return (0);
}
