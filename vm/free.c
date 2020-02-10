#include "libft.h"
#include "get_next_line.h"
#include "vm.h"


void		t_vm_destruct(t_vm *vm)
{
	int	i;

	i = -1;
	while (++i < vm->n_champs)
	{
		free(vm->champs[i].name);
		free(vm->champs[i].comment);
	}
	i = -1;
	while (++i < vm->procs.count)
		free(vm->procs.data[i]);
	t_arrayp_del(&vm->procs);
	free(vm->mem);
}
