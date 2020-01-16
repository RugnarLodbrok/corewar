/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   operation.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksticks <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/25 17:20:05 by ksticks           #+#    #+#             */
/*   Updated: 2019/12/25 17:20:06 by ksticks          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft_compat.h"
#include "vm.h"

extern t_op op_tab[17];

t_op *read_op(const byte *ptr)
{
	int i;

	i = -1;
	while (op_tab[++i].name)
		if (op_tab[i].code == *ptr)
			return (&op_tab[i]);
	return (0);
}

void t_op_parse_arg_types(t_op_context *c, byte *arg_types)
{
	uint i;
	byte *pc;

	ft_bzero(arg_types, sizeof(byte) * 3);
	if (!c->op->need_types)
		ft_memcpy(arg_types, &c->op->args_types[0],
				  sizeof(byte) * c->op->args_num);
	else
	{
		pc = c->vm->mem + c->proc->pc;
		for (i = 0; i < c->op->args_num; ++i)
			arg_types[i] = (*(pc + c->cursor) >> (2 * (3 - i))) & (byte)0x3;
		c->cursor++;
	}
}

void t_op_parse_args(t_op_context *c, const byte *arg_types, byte **args)
{
	int i;
	uint reg_number;
	byte *p;

	p = c->vm->mem + c->proc->pc;
	for (i = 0; i < c->op->args_num; ++i)
	{
		if (arg_types[i] == DIR_CODE)
		{
			args[i] = p + c->cursor;
			c->cursor += c->op->dir_size;
		}
		else if (arg_types[i] == IND_CODE)
		{
			c->ind_arg = read_short_int(c->vm->host_endian, p + c->cursor);
			args[i] = p + c->ind_arg;
			ft_memcpy(&c->ind_val[0], p + c->ind_arg, REG_SIZE);
			c->cursor += IND_SIZE;
		}
		else if (arg_types[i] == REG_CODE)
		{
			reg_number = read_uint(c->vm->host_endian, p + c->cursor,
								   REG_ARG_SIZE);
			args[i] = &c->proc->reg[reg_number - 1][0];
			c->cursor += REG_ARG_SIZE;
		}
	}
}

int t_op_exec(t_op *op, t_proc *proc, t_vm *vm)
{
	t_op_context c;
	uint old_pc;
	byte *args[3];
	byte arg_types[3];

	c.op = op;
	c.proc = proc;
	c.vm = vm;
	c.cursor = 1;
	c.ind_arg = INT_MAX;
	ft_bzero(&args[0], 3);
	old_pc = proc->pc;
	t_op_parse_arg_types(&c, &arg_types[0]);
	t_op_parse_args(&c, &arg_types[0], &args[0]);
	op->f(vm, proc, args[0], args[1], args[2]);
	if (c.ind_arg != INT_MAX)
		if (ft_memcmp(&c.ind_val[0], vm->mem + proc->pc + c.ind_arg, REG_SIZE))
			if (vm->mode == MODE_VIS)
				write_mem(vm->mem + proc->pc + c.ind_arg, proc->pc + c.ind_arg, REG_SIZE, proc->id);
	if (proc->pc == old_pc)
		proc->pc += c.cursor;
	return (1);
}
