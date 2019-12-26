#include "libft.h"
#include "vm.h"

void	live(t_vm *vm, t_proc *proc, uint *arg1, uint *arg2, uint *arg3)
{

}

int		ld(t_vm *vm, t_proc *proc, void *arg1, void *arg2, void *arg3)
{
	uint v;

	ft_memcpy(arg2, arg1, DIR_SIZE * sizeof(char));
	v = read_uint(vm->host_endian, arg2, 4);
	proc->carry = (v != 0);
	return (1);
}

int		st(t_vm *vm, t_proc *proc, void *arg1, void *arg2, void *arg3)
{
	ft_memcpy(arg2, arg1, 4 * sizeof(char));
	return (1);
}

int 	add(t_vm *vm, t_proc *proc, void *arg1, void *arg2, void *arg3)
{
	int a;
	int b;

	a = read_uint(vm->host_endian, arg1, 4);
	b = read_uint(vm->host_endian, arg2, 4);
	if (a + b == 0)
		proc->carry = 1;
	else
		proc->carry = 0;
	write_uint(vm->host_endian, a + b, arg3, 4);
	return (1);
}

int		sub(t_vm *vm, t_proc *proc, void *arg1, void *arg2, void *arg3)
{
	int a;
	int b;

	a = read_uint(vm->host_endian, arg1, 4);
	b = read_uint(vm->host_endian, arg2, 4);
	if (a + b == 0)
		proc->carry = 1;
	else
		proc->carry = 0;
	write_uint(vm->host_endian, a - b, arg3, 4);
	return (1);
}

int 	and(t_vm *vm, t_proc *proc, void *arg1, void *arg2, void *arg3)
{
	uint a;
	uint b;
	uint c;

<<<<<<< HEAD
	a = read_uint(vm->host_endian, arg1, 4);
	b = read_uint(vm->host_endian, arg2, 4);
	c = a & b;
	if (c == 0)
		proc->carry = 1;
	else
		proc->carry = 0;
	write_uint(vm->host_endian, c, arg3, 4);
	return (1);
}

int 	or(t_vm *vm, t_proc *proc, void *arg1, void *arg2, void *arg3)
=======
int		op_ld(t_vm *vm, t_proc *proc, void *arg1, void *arg2, void *arg3)
>>>>>>> a384b508d62428738f8b6e07bbf8f263dce67424
{
	uint a;
	uint b;
	uint c;

	a = read_uint(vm->host_endian, arg1, 4);
	b = read_uint(vm->host_endian, arg2, 4);
	c = a | b;
	if (c == 0)
		proc->carry = 1;
	else
		proc->carry = 0;
	write_uint(vm->host_endian, c, arg3, 4);
	return (1);
}

int 	xor(t_vm *vm, t_proc *proc, void *arg1, void *arg2, void *arg3)
{
	uint a;
	uint b;
	uint c;

	a = read_uint(vm->host_endian, arg1, 4);
	b = read_uint(vm->host_endian, arg2, 4);
	c = a ^ b;
	if (c == 0)
		proc->carry = 1;
	else
		proc->carry = 0;
	write_uint(vm->host_endian, c, arg3, 4);
	return (1);
}

int 	zjmp(t_vm *vm, t_proc *proc, void *arg1, void *arg2, void *arg3)
{
	if (proc->carry == 1)
		proc->pc += read_uint(vm->host_endian, arg1, 4);
	return (1);
}

int 	ldi(t_vm *vm, t_proc *proc, void *arg1, void *arg2, void *arg3)
{
	int n1;
	int n2;

	n1 = read_uint(vm->host_endian, arg1, 4);
	n2 = read_uint(vm->host_endian, arg2, 4);
	ft_memcpy(arg3, proc->pc + n1 + n2, 4);
	return (1);
}
<<<<<<< HEAD

int 	sti(t_vm *vm, t_proc *proc, void *arg1, void *arg2, void *arg3)
{
	int n2;
	int n3;

	n2 = read_uint(vm->host_endian, arg2, 4);
	n3 = read_uint(vm->host_endian, arg3, 4);
	ft_memcpy(proc->pc + n3 + n2, arg1, 4);
	return (1);
}

int		f_fork(t_vm *vm, t_proc *proc, void *arg1, void *arg2, void *arg3)
{
	t_proc	new;
	int		i;
	int 	j;

	i = -1;
	j = -1;
	new.champ_id = proc->champ_id;
	new.pc = proc->pc;
	//new.reg = proc->reg;
	new.carry = proc->carry;
	new.live = proc->carry;
	new.delay = proc->delay;
	while (++i < REG_NUMBER)
		while (++j < 4)
			new.reg[i][j] = proc->reg[i][j];
	return (1);
}

int 	lld(t_vm *vm, t_proc *proc, void *arg1, void *arg2, void *arg3)
{

}

int		aff(t_vm *vm, t_proc *proc, void *arg1, void *arg2, void *arg3)
=======
*/
int		op_aff(t_vm *vm, t_proc *proc, void *arg1, void *arg2, void *arg3)
>>>>>>> a384b508d62428738f8b6e07bbf8f263dce67424
{
	write(1, &arg1, 1);
	return (1);
}
