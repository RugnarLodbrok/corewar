/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpoetess <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/03 16:08:57 by rpoetess          #+#    #+#             */
/*   Updated: 2020/02/03 16:08:58 by rpoetess         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "vm.h"

int op_live(t_op_context *c, void *arg1, void *arg2, void *arg3)
{
	uint reg_value;
	uint champ_id;

	(void)arg2;
	(void)arg3;
	c->proc->last_live = c->vm->i;
	reg_value = read_uint(c->vm, arg1, REG_SIZE);
	champ_id = UINT_MAX - reg_value;
	if (champ_id == c->proc->champ_id && champ_id < (uint)c->vm->n_champs)
	{
		c->vm->winner = champ_id;
		if (c->vm->v_flag & VERBOSE_LIVES)
			ft_printf("Player %d (%s) is said to be alive\n",
					  champ_id + 1, c->vm->champs[champ_id].name);
	}
	return (1);
}

int op_lld(t_op_context *c, void *arg1, void *arg2, void *arg3)
{
	(void)arg3;
	t_vm_memcpy(c->vm, arg2, arg1, sizeof(char) * REG_SIZE);
	c->proc->carry = (0 == read_uint(c->vm, arg2, 4));
	return (1);
}

int op_ld(t_op_context *c, void *arg1, void *arg2, void *arg3)
{
	return (op_lld(c, apply_idx_mod(c, arg1), arg2, arg3));
}

int op_st(t_op_context *c, void *arg1, void *arg2, void *arg3)
{
	(void)arg3;
	arg2 = apply_idx_mod(c, arg2);
	t_vm_memcpy(c->vm, arg2, arg1, sizeof(char) * REG_SIZE);
	c->changed_memory = (int)((byte *)arg2 - c->vm->mem);
	return (1);
}

int op_add(t_op_context *c, void *arg1, void *arg2, void *arg3)
{
	int a;
	int b;

	a = read_uint(c->vm, arg1, 4);
	b = read_uint(c->vm, arg2, 4);
	c->proc->carry = (a + b == 0);
	write_uint(c->vm, a + b, arg3, 4);
	return (1);
}

int op_sub(t_op_context *c, void *arg1, void *arg2, void *arg3)
{
	int a;
	int b;

	a = read_uint(c->vm, arg1, 4);
	b = read_uint(c->vm, arg2, 4);
	c->proc->carry = (a == b);
	write_uint(c->vm, a - b, arg3, 4);
	return (1);
}

int op_and(t_op_context *c, void *arg1, void *arg2, void *arg3)
{
	uint a;
	uint b;
	uint r;

	a = read_uint(c->vm, apply_idx_mod(c, arg1), 4);
	b = read_uint(c->vm, apply_idx_mod(c, arg2), 4);
	c->proc->carry = !(r = a & b);
	write_uint(c->vm, r, arg3, 4);
	return (1);
}

int op_or(t_op_context *c, void *arg1, void *arg2, void *arg3)
{
	uint a;
	uint b;
	uint d;

	a = read_uint(c->vm, arg1, 4);
	b = read_uint(c->vm, arg2, 4);
	d = a | b;
	if (d == 0)
		c->proc->carry = 1;
	else
		c->proc->carry = 0;
	write_uint(c->vm, d, arg3, 4);
	return (1);
}

int op_xor(t_op_context *c, void *arg1, void *arg2, void *arg3)
{
	uint a;
	uint b;
	uint d;

	a = read_uint(c->vm, arg1, 4);
	b = read_uint(c->vm, arg2, 4);
	d = a ^ b;
	if (d == 0)
		c->proc->carry = 1;
	else
		c->proc->carry = 0;
	write_uint(c->vm, d, arg3, 4);
	return (1);
}

int op_zjmp(t_op_context *c, void *arg1, void *arg2, void *arg3)
{
	(void)arg2;
	(void)arg3;
	if (c->proc->carry)
		c->proc->pc = mem_mod(
				c->proc->pc + read_short_int(c->vm, arg1) % IDX_MOD);
	return (1);
}

int op_ldi(t_op_context *c, void *arg1, void *arg2, void *arg3)
{
	int n1;
	int n2;
	int target;

	n1 = read_short_int(c->vm, arg1);
	n2 = read_short_int(c->vm, arg2);
	target = (int)c->proc->pc + (n1 + n2) % IDX_MOD;
	t_vm_memcpy(c->vm, arg3, &c->vm->mem[mem_mod(target)], REG_SIZE);
//  t_vm_memcpy(c->vm, arg3, &c->vm->mem[target], REG_SIZE);
	return (1);
}

int op_lldi(t_op_context *c, void *arg1, void *arg2, void *arg3)
{
	int n1;
	int n2;
	int target;

	n1 = read_short_int(c->vm, arg1);
	n2 = read_short_int(c->vm, arg2);
	target = (int)c->proc->pc + n1 + n2;
	t_vm_memcpy(c->vm, arg3, &c->vm->mem[mem_mod(target)], REG_SIZE);
	return (1);
}

int op_sti(t_op_context *c, void *arg1, void *arg2, void *arg3)
{
	int n2;
	int n3;
	int target;

	n2 = read_short_int(c->vm, arg2);
	n3 = read_short_int(c->vm, arg3);
	target = (int)mem_mod(c->proc->pc) + (n2 + n3) % IDX_MOD;
	t_vm_memcpy(c->vm, &c->vm->mem[mem_mod(target)], arg1, REG_SIZE);
	c->changed_memory = mem_mod(target);
	return (1);
}

int op_lfork(t_op_context *c, void *arg1, void *arg2, void *arg3)
{
	t_proc *p;

	(void)arg2;
	(void)arg3;
	p = (t_proc *)malloc(sizeof(t_proc));
	t_vm_memcpy(c->vm, p, c->proc, sizeof(t_proc));
	p->id = c->vm->procs.count;
	p->pc = mem_mod(p->pc + (read_short_int(c->vm, arg1) % IDX_MOD));
	p->op = 0;
	if (c->vm->mode == MODE_VIS)
		write_proc_update(p, c->vm->champs[p->champ_id].name);
	t_arrayp_push(&c->vm->procs, p);
	return (1);
}

int op_fork(t_op_context *c, void *arg1, void *arg2, void *arg3)
{
	return (op_lfork(c, apply_idx_mod(c, arg1), arg2, arg3));
}

int op_aff(t_op_context *c, void *arg1, void *arg2, void *arg3)
{
	byte d;

	(void)arg2;
	(void)arg3;
	d = (byte)read_uint(c->vm, arg1, REG_SIZE);
	if (c->vm->mode == MODE_VIS)
		write_proc_stdout(c->vm, c->proc->id, d);
	if (c->vm->mode == MODE_DEFAULT)
		write(1, &d, 1);
	return (1);
}
