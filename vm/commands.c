#include "libft.h"
#include "vm.h"

int		op_live(t_op_context *c, void *arg1, void *arg2, void *arg3)
{
	uint reg_value;
	(void)arg2;
	(void)arg3;
	reg_value = read_uint(c->vm->host_endian, arg1, REG_SIZE);
	if (UINT_MAX - reg_value == c->proc->champ_id + 1)
		c->proc->last_live = c->vm->i;
	return (1);
}

int		op_ld(t_op_context *c, void *arg1, void *arg2, void *arg3)
{
	uint v;

	(void)arg3;
	ft_memcpy(arg2, arg1, sizeof(char) * REG_SIZE);
	v = read_uint(c->vm->host_endian, arg2, 4);
	c->proc->carry = (v != 0);
	return (1);
}

int		op_st(t_op_context *c, void *arg1, void *arg2, void *arg3)
{
	(void)arg3;
	ft_memcpy(arg2, arg1, sizeof(char) * REG_SIZE);
	c->changed_memory = (int)((byte*)arg2 - c->vm->mem);
	return (1);
}

int 	op_add(t_op_context *c, void *arg1, void *arg2, void *arg3)
{
	int a;
	int b;

	a = read_uint(c->vm->host_endian, arg1, 4);
	b = read_uint(c->vm->host_endian, arg2, 4);
	if (a + b == 0)
		c->proc->carry = 1;
	else
		c->proc->carry = 0;
	write_uint(c->vm->host_endian, a + b, arg3, 4);
	return (1);
}

int		op_sub(t_op_context *c, void *arg1, void *arg2, void *arg3)
{
	int a;
	int b;

	a = read_uint(c->vm->host_endian, arg1, 4);
	b = read_uint(c->vm->host_endian, arg2, 4);
	if (a == b)
		c->proc->carry = 1;
	else
		c->proc->carry = 0;
	write_uint(c->vm->host_endian, a - b, arg3, 4);
	return (1);
}

int 	op_and(t_op_context *c, void *arg1, void *arg2, void *arg3)
{
	uint a;
	uint b;
	uint d;

	a = read_uint(c->vm->host_endian, arg1, 4);
	b = read_uint(c->vm->host_endian, arg2, 4);
	d = a & b;
	if (d == 0)
		c->proc->carry = 1;
	else
		c->proc->carry = 0;
	write_uint(c->vm->host_endian, d, arg3, 4);
	return (1);
}

int 	op_or(t_op_context *c, void *arg1, void *arg2, void *arg3)
{
	uint a;
	uint b;
	uint d;

	a = read_uint(c->vm->host_endian, arg1, 4);
	b = read_uint(c->vm->host_endian, arg2, 4);
	d = a | b;
	if (d == 0)
		c->proc->carry = 1;
	else
		c->proc->carry = 0;
	write_uint(c->vm->host_endian, d, arg3, 4);
	return (1);
}

int 	op_xor(t_op_context *c, void *arg1, void *arg2, void *arg3)
{
	uint a;
	uint b;
	uint d;

	a = read_uint(c->vm->host_endian, arg1, 4);
	b = read_uint(c->vm->host_endian, arg2, 4);
	d = a ^ b;
	if (d == 0)
		c->proc->carry = 1;
	else
		c->proc->carry = 0;
	write_uint(c->vm->host_endian, d, arg3, 4);
	return (1);
}

int 	op_zjmp(t_op_context *c, void *arg1, void *arg2, void *arg3)
{
	(void)arg2;
	(void)arg3;
	if (c->proc->carry)
		c->proc->pc += read_short_int(c->vm, arg1);
	return (1);
}

int 	op_ldi(t_op_context *c, void *arg1, void *arg2, void *arg3)
{
	int n1;
	int n2;
	int target;

	n1 = read_short_int(c->vm, arg1);
	n2 = read_short_int(c->vm, arg2);
	target = (int)c->proc->pc + n1 + n2;
	ft_memcpy(arg3, c->vm->mem + target, REG_SIZE);
	return (1);
}

int 	op_sti(t_op_context *c, void *arg1, void *arg2, void *arg3)
{
	int n2;
	int n3;
	int target;

	n2 = read_short_int(c->vm, arg2);
	n3 = read_short_int(c->vm, arg3);
	target = (int)c->proc->pc + n2 + n3;
	ft_memcpy(c->vm->mem + target, arg1, REG_SIZE);
	c->changed_memory = target;
	return (1);
}

int		op_fork(t_op_context *c, void *arg1, void *arg2, void *arg3)
{
	t_proc	*new;
	int		i;
	int 	j;

	(void)arg2;
	(void)arg3;
	new = (t_proc*)malloc(sizeof(t_proc));
	i = -1;
	j = -1;
	new->champ_id = c->proc->champ_id;
	new->pc = c->proc->pc;
	//new.reg = c->proc->reg;
	new->carry = c->proc->carry;
	new->last_live = c->proc->last_live;
	new->delay = c->proc->delay;
	while (++i < REG_NUMBER)
		while (++j < 4)
			new->reg[i][j] = c->proc->reg[i][j];
	ft_memcpy(new,  arg1, 40);
	return (1);
}

int 	op_lld(t_op_context *c, void *arg1, void *arg2, void *arg3)
{
	(void)c;
	(void)arg3;
	ft_memcpy(arg2, arg1, DIR_SIZE * sizeof(char));
	return (1);
}

int		op_lldi(t_op_context *c, void *arg1, void *arg2, void *arg3)
{
	int	n1;
	int	n2;

	n1 = read_uint(c->vm->host_endian, arg1, 4);
	n2 = read_uint(c->vm->host_endian, arg2, 4);
	write_uint(c->vm->host_endian, n1 + n2, arg3, 4);
	return (1);
}

int		op_lfork(t_op_context *c, void *arg1, void *arg2, void *arg3)
{
	op_fork(c, arg1, arg2, arg3);
	return (1);
}

int		op_aff(t_op_context *c, void *arg1, void *arg2, void *arg3)
{
	byte d;

	(void)arg2;
	(void)arg3;
	d = (byte)read_uint(c->vm->host_endian, arg1, REG_SIZE);
	if (c->vm->mode == MODE_VIS)
		write_proc_stdout(c->vm, c->proc->id, d);
	if (c->vm->mode == MODE_DEFAULT)
		write(1, &c, 1);
	return (1);
}
