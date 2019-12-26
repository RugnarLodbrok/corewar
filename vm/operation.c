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

int t_op_exec(t_op *op, t_proc *proc, t_vm *vm)
{
	int i;
	byte args_types_byte;
	byte *args_ptr;
	byte arg_types[3];
	byte *args[3];

	args_ptr = vm->mem + proc->pc + 2;
	args_types_byte = *(args_ptr - 1);
	for (i = 0; i < op->args_num; ++i)
	{
		arg_types[i] = (args_types_byte >> (2 * i)) & (byte)0x3;
		if (arg_types[i] == DIR_CODE)
		{
			args[i] = args_ptr;
			args_ptr += DIR_SIZE;
		}
		else if (arg_types[i] == IND_CODE)
		{
			args[i] = vm->mem + proc->pc +
					  read_uint(vm->host_endian, args_ptr, IND_SIZE);
			args_ptr += IND_SIZE;
		}
		else if (arg_types[i] == REG_CODE)
		{
			args[i] = &proc->reg[read_uint(vm->host_endian, args_ptr,
										   REG_SIZE)][0];
			args_ptr += REG_SIZE;
		}
		else
			return 0;
	}
	return op->f(vm, proc, &args[0], &args[1], &args[2]);
}
