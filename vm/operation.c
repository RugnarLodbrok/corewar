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

t_arg_types t_op_parse_arg_types(t_op *op, byte **args_ptr)
{
	t_arg_types ret;
	int i;

	ft_bzero(&ret, sizeof(t_arg_types));
	if (!op->need_types)
		ft_memcpy(&ret.args, &op->args_types, sizeof(byte)*op->args_num);
	else
	{
		for (i = 0; i < op->args_num; ++i)
			ret.args[i] = (**args_ptr >> (2 * (3 - i))) & (byte)0x3;
		(*args_ptr)++;
	}
	return (ret);
}

int t_op_exec(t_op *op, t_proc *proc, t_vm *vm)
{
	int i;
	byte *args_ptr;
	t_arg_types arg_types;
	byte *args[3]; //pointers that will be passed to op function
	uint reg_number;

	args_ptr = vm->mem + proc->pc + 1;
	arg_types = t_op_parse_arg_types(op, &args_ptr);
	for (i = 0; i < op->args_num; ++i)
	{
		if (arg_types.args[i] == DIR_CODE)
		{
			args[i] = args_ptr;
			args_ptr += op->dir_size;
		}
		else if (arg_types.args[i] == IND_CODE)
		{
			args[i] = vm->mem + proc->pc +
					  read_uint(vm->host_endian, args_ptr, IND_ARG_SIZE);
			args_ptr += IND_ARG_SIZE;
		}
		else if (arg_types.args[i] == REG_CODE)
		{
			reg_number = read_uint(vm->host_endian, args_ptr, REG_ARG_SIZE);
			args[i] = &proc->reg[reg_number][0];
			args_ptr += REG_ARG_SIZE;
		}
		else
			return 0;
	}
	if (op->f(vm, proc, args[0], args[1], args[2]))
	{
		proc->pc = args_ptr - vm->mem;
		return (1);
	}
	return (0);
}
