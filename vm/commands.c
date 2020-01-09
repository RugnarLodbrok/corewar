#include "libft.h"
#include "vm.h"

int		op_live(t_vm *vm, t_proc *proc, void *arg1, void *arg2, uint *arg3)
{
	if (read_uint(vm->host_endian, arg1, 4) == proc->champ_id)
		vm->champs->live = 0;
	return (1);
}

int		op_ld(t_vm *vm, t_proc *proc, void *arg1, void *arg2, void *arg3)
{
	uint v;

	ft_memcpy(arg2, arg1, sizeof(char) * REG_SIZE);
	v = read_uint(vm->host_endian, arg2, 4);
	proc->carry = (v != 0);
	return (1);
}

int		op_st(t_vm *vm, t_proc *proc, void *arg1, void *arg2, void *arg3)
{
	ft_memcpy(arg2, arg1, 4 * sizeof(char));
	return (1);
}

int 	op_add(t_vm *vm, t_proc *proc, void *arg1, void *arg2, void *arg3)
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

int		op_sub(t_vm *vm, t_proc *proc, void *arg1, void *arg2, void *arg3)
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

int 	op_and(t_vm *vm, t_proc *proc, void *arg1, void *arg2, void *arg3)
{
	uint a;
	uint b;
	uint c;

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

int 	op_or(t_vm *vm, t_proc *proc, void *arg1, void *arg2, void *arg3)
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

int 	op_xor(t_vm *vm, t_proc *proc, void *arg1, void *arg2, void *arg3)
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

int 	op_zjmp(t_vm *vm, t_proc *proc, void *arg1, void *arg2, void *arg3)
{
	if (proc->carry == 1)
		proc->pc += read_uint(vm->host_endian, arg1, 4);
	return (1);
}

int 	op_ldi(t_vm *vm, t_proc *proc, void *arg1, void *arg2, void *arg3)
{
	int n1;
	int n2;

	n1 = read_uint(vm->host_endian, arg1, 4);
	n2 = read_uint(vm->host_endian, arg2, 4);
	write_uint(vm->host_endian, n1 + n2, arg3, 4);
	return (1);
}

int 	op_sti(t_vm *vm, t_proc *proc, void *arg1, void *arg2, void *arg3)
{
	int n2;
	int n3;

	n2 = read_uint(vm->host_endian, arg2, 4);
	n3 = read_uint(vm->host_endian, arg3, 4);
	write_uint(vm->host_endian, n3 + n2, arg1, 4);
	return (1);
}

int		op_fork(t_vm *vm, t_proc *proc, void *arg1, void *arg2, void *arg3)
{
	t_proc	*new;
	int		i;
	int 	j;

	new = (t_proc*)malloc(sizeof(t_proc));
	i = -1;
	j = -1;
	new->champ_id = proc->champ_id;
	new->pc = proc->pc;
	//new.reg = proc->reg;
	new->carry = proc->carry;
	new->live = proc->carry;
	new->delay = proc->delay;
	while (++i < REG_NUMBER)
		while (++j < 4)
			new->reg[i][j] = proc->reg[i][j];
	ft_memcpy(new,  arg1, 40);
	return (1);
}

int 	op_lld(t_vm *vm, t_proc *proc, void *arg1, void *arg2, void *arg3)
{
	ft_memcpy(arg2, arg1, DIR_SIZE * sizeof(char));
	return (1);
}

int		op_lldi(t_vm *vm, t_proc *proc, void *arg1, void *arg2, void *arg3)
{
	int	n1;
	int	n2;

	n1 = read_uint(vm->host_endian, arg1, 4);
	n2 = read_uint(vm->host_endian, arg2, 4);
	write_uint(vm->host_endian, n1 + n2, arg3, 4);
	return (1);
}

int		op_lfork(t_vm *vm, t_proc *proc, void *arg1, void *arg2, void *arg3)
{
	op_fork(vm, proc, arg1, arg2, arg3);
}

int		op_aff(t_vm *vm, t_proc *proc, void *arg1, void *arg2, void *arg3)
{
	byte c;

	c = (byte)read_uint(vm->host_endian, arg1, REG_SIZE);
	write(1, &c, 1);
	return (1);
}
