#include "libft.h"
#include "vm.h"

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
		t_vm_add_champ(&vm, (const char *)av[i]);
		++i;
	}
	t_vm_up(&vm);
	while (!vm.shutdown)
		t_vm_step(&vm);
	t_vm_destruct(&vm);
	return (0);
}
