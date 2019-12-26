#include "libft.h"
#include "vm.h"

int		check_tdir_tind(void *arg)
{

}

int		check_treg_tind(void *arg)
{

}

void	*rev_byte(void *arg)
{

}

void	live(t_vm *vm, t_proc *proc, uint *arg1, uint *arg2, uint *arg3)
{

}

//	vm->mem + proc->pc

int		op_ld(t_vm *vm, t_proc *proc, void *arg1, void *arg2, void *arg3)
{
	uint v;

	ft_memcpy(arg2, arg1, DIR_SIZE * sizeof(char));
	v = read_uint(vm->host_endian, arg2, 4);
	proc->carry = (v != 0);
	return (1);
}
/*
void	st(t_vm *vm, t_proc *proc, void *arg1, void *arg2, void *arg3)
{
	int status;

	status = check_treg_tind(arg2);
	if (status)
		ft_memcpy(arg2, arg1, 4 * sizeof(char) * 2);
	else
		ft_memcpy(vm->mem + proc->pc + *(int*)rev_byte(arg2), arg1, 4 * sizeof(char) * 2);
}
*/
int		op_aff(t_vm *vm, t_proc *proc, void *arg1, void *arg2, void *arg3)
{
	write(1, &arg1, 1);
	return (1);
}
