#include "libft.h"
#include "get_next_line.h"
#include "vm.h"

void		t_args_destruct(t_args args)
{
	int i;

	i = -1;
	while (args.champs[++i])
	{
		free(args.champs);
	}
}

void		t_vm_destruct(t_vm *vm)
{
	int	i;

	i = -1;
	while (++i < vm->n_champs)
	{
		free(vm->champs[i].name);
		free(vm->champs[i].comment);
	}
	free(vm->mem);
	t_arrayp_del(&vm->procs);
}
