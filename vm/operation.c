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

extern t_op	op_tab[17];
extern byte	code_to_arg_type[5];
extern byte	arg_type_to_code[5];

t_op		*read_op(const byte *ptr)
{
	int	i;

	i = -1;
	while (op_tab[++i].name)
		if (op_tab[i].code == *ptr)
			return (&op_tab[i]);
	return (0);
}

void		t_op_parse_arg_types(t_op_context *c, byte *arg_codes)
{
	uint	i;

	i = 0;
	ft_bzero(arg_codes, sizeof(byte) * 3);
	if (!c->op->need_types)
		while (i < c->op->args_num)
		{
			arg_codes[i] = arg_type_to_code[c->op->args_types[i]];
			i++;
		}
	else
	{
		i = 0;
		while (i < c->op->args_num)
		{
			arg_codes[i] = (c->vm->mem[mem_mod(c->proc->pc + c->cursor)]
					>> (2 * (3 - i))) & (byte)0x3; //!achtung
			if (!(code_to_arg_type[arg_codes[i]] & c->op->args_types[i]))
				c->invalid_args = 1;
			i++;
		}
		c->cursor++;
	}
}

void		t_op_parse_args(t_op_context *c, const byte *arg_types, byte **args)
{
	int		i;
	uint	reg_number;

	i = -1;
	while (++i < c->op->args_num)
		if (arg_types[i] == DIR_CODE)
		{
			args[i] = &c->vm->mem[mem_mod(c->proc->pc + c->cursor)];
			c->cursor += c->op->dir_size;
		}
		else if (arg_types[i] == IND_CODE)
		{
			args[i] = &c->vm->mem[
					mem_mod(c->proc->pc +
							read_short_int(c->vm, &c->vm->mem[mem_mod(
									c->proc->pc + c->cursor)]))];
			c->cursor += IND_SIZE;
		}
		else if (arg_types[i] == REG_CODE)
		{
			reg_number = read_uint(c->vm,
								&c->vm->mem[mem_mod(
										c->proc->pc + c->cursor)],
								REG_ARG_SIZE) - 1;
			if (reg_number >= REG_NUMBER)
				c->invalid_args = 1;
			else
				args[i] = &c->proc->reg[reg_number][0];
			c->cursor += REG_ARG_SIZE;
		}
}

void		t_op_context_init(t_op_context *c, t_vm *vm, t_proc *proc, t_op *op)
{
	ft_bzero(c, sizeof(t_op_context));
	c->op = op;
	c->proc = proc;
	c->vm = vm;
	c->cursor = 1;
	c->changed_memory = -1;
}

static void	t_op_adv(t_op_context *c)
{
	const t_op	*op = c->op;
	const t_vm	*vm = c->vm;
	t_proc		*proc;

	proc = c->proc;
	if (op->code != 9 || !proc->carry)
	{
		if (vm->v_flag & VERBOSE_PC)
		{
			ft_printf("ADV %u (0x%04x -> 0x%04x) ",
					c->cursor, proc->pc, proc->pc + c->cursor);
			while (c->cursor--)
			{
				put_hex(vm->mem[proc->pc], 2);
				proc->pc = mem_mod(proc->pc + 1);
				ft_printf(" ");
			}
			ft_printf("\n");
		}
		else
			proc->pc = mem_mod(proc->pc + c.cursor);
	}
}

void		t_op_exec(t_op *op, t_proc *proc, t_vm *vm)
{
	t_op_context	c;
	byte			*args[3];
	byte			arg_types[3];

	t_op_context_init(&c, vm, proc, op);
	ft_bzero(&args[0], 3);
	t_op_parse_arg_types(&c, &arg_types[0]);
	t_op_parse_args(&c, &arg_types[0], &args[0]);
	if (!c.invalid_args)
	{
		if (vm->v_flag & VERBOSE_OPS)
			ft_printf("P %4d | %s\n", proc->id + 1, proc->op->name);
		op->f(&c, args[0], args[1], args[2]);
		if (c.changed_memory >= 0 && c.changed_memory < MEM_SIZE &&
			vm->mode == MODE_VIS)
			write_mem(vm->mem, c.changed_memory, REG_SIZE, proc->id);
	}
	t_op_adv(&c);
}
