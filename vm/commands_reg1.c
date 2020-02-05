/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands_reg.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpoetess <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/05 16:41:14 by rpoetess          #+#    #+#             */
/*   Updated: 2020/02/05 16:41:15 by rpoetess         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "vm.h"

int	op_lld(t_op_context *c, void *arg1, void *arg2, void *arg3)
{
	(void)arg3;
	t_vm_memcpy(c->vm, arg2, arg1, sizeof(char) * REG_SIZE);
	c->proc->carry = (0 == read_uint(c->vm, arg2, 4));
	return (1);
}

int	op_ld(t_op_context *c, void *arg1, void *arg2, void *arg3)
{
	return (op_lld(c, apply_idx_mod(c, arg1), arg2, arg3));
}

int	op_ldi(t_op_context *c, void *arg1, void *arg2, void *arg3)
{
	int n1;
	int n2;
	int target;

	n1 = read_short_int(c->vm, arg1);
	n2 = read_short_int(c->vm, arg2);
	target = (int)c->proc->pc + (n1 + n2) % IDX_MOD;
	if (c->vm->v_flag & VERBOSE_OPS)
		ft_printf("       | -> load from %d + %d = %d (with pc and mod %d)\n",
				n1, n2, n1 + n2, target);
	target = mem_mod(target);
	t_vm_memcpy(c->vm, arg3, &c->vm->mem[target], REG_SIZE);
	return (1);
}

int	op_lldi(t_op_context *c, void *arg1, void *arg2, void *arg3)
{
	int n1;
	int n2;
	int target;

	n1 = read_short_int(c->vm, arg1);
	n2 = read_short_int(c->vm, arg2);
	target = (int)c->proc->pc + n1 + n2;
	if (c->vm->v_flag & VERBOSE_OPS)
		ft_printf("       | -> load from %d + %d = %d (with pc and mod %d)",
				n1, n2, n1 + n2, target);
	target = mem_mod(target);
	t_vm_memcpy(c->vm, arg3, &c->vm->mem[target], REG_SIZE);
	return (1);
}
