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

void	ld(t_vm *vm, t_proc *proc, void *arg1, void *arg2, void *arg3)
{
	int status;
	int tmp1;
	int tmp2;
	int tmp3;

	tmp1 = (int)arg1;
	tmp2 = (int)arg2;
	status = check_tdir_tind(arg1);
	if (status == 1)
	{
		tmp2 = tmp1;
		if (tmp1 == 0)
			proc->carry = 1;
		else
			proc->carry = 0;
	}
	else{
		tmp1 = (int)(rev_byte(arg1));
	}
}

void	st(t_vm *vm, t_proc *proc, void *arg1, void *arg2, void *arg3)
{
	int status;

	status = check_treg_tind(arg2);
	if (status)
		ft_memcpy(arg2, arg1, 4 * sizeof(char) * 2);
	else
		ft_memcpy(vm->mem + proc->pc + *(int*)rev_byte(arg2), arg1, 4 * sizeof(char) * 2);
}
