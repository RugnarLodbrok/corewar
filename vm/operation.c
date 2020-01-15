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

uint t_op_parse_arg_types(t_op *op, const byte *args_ptr, byte *arg_types)
{
	uint i;
	uint offset;

	offset = 0;
	ft_bzero(arg_types, sizeof(byte) * 3);
	if (!op->need_types)
		ft_memcpy(arg_types, &op->args_types[0], sizeof(byte) * op->args_num);
	else
	{
		for (i = 0; i < op->args_num; ++i)
			arg_types[i] = (*(args_ptr + offset) >> (2 * (3 - i))) & (byte)0x3;
		offset++;
	}
	return (offset);
}

uint t_op_parse_args(t_op *op, t_vm *vm, t_proc *proc, byte **args)
{
	int i;
	byte arg_types[3];
	uint offset;
	uint reg_number;
	byte *p;

	p = vm->mem + proc->pc;
	offset = 1;
	offset += t_op_parse_arg_types(op, p + offset, &arg_types[0]);
	for (i = 0; i < op->args_num; ++i)
	{
		if (arg_types[i] == DIR_CODE)
		{
			args[i] = p + offset;
			offset += op->dir_size;
		}
		else if (arg_types[i] == IND_CODE)
		{
			args[i] = p + read_uint(vm->host_endian, p + offset, IND_ARG_SIZE);
			offset += IND_ARG_SIZE;
		}
		else if (arg_types[i] == REG_CODE)
		{
			reg_number = read_uint(vm->host_endian, p + offset, REG_ARG_SIZE);
			args[i] = &proc->reg[reg_number][0];
			offset += REG_ARG_SIZE;
		}
	}
	return offset;
}

int t_op_exec(t_op *op, t_proc *proc, t_vm *vm)
{
	uint old_pc;
	uint new_pc;
	byte *args[3]; //pointers that will be passed to op function

	old_pc = proc->pc;
	new_pc = proc->pc + t_op_parse_args(op, vm, proc, &args[0]);
	op->f(vm, proc, args[0], args[1], args[2]);
	if (proc->pc == old_pc)
		proc->pc = new_pc;
	return (0);
}
