/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands_reg2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpoetess <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/05 16:41:28 by rpoetess          #+#    #+#             */
/*   Updated: 2020/02/05 16:41:31 by rpoetess         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "vm.h"

int	op_st(t_op_context *c, void *arg1, void *arg2, void *arg3)
{
	(void)arg3;
	arg2 = apply_idx_mod(c, arg2);
	t_vm_memcpy(c->vm, arg2, arg1, sizeof(char) * REG_SIZE);
	c->changed_memory = (int)((byte *)arg2 - c->vm->mem);
	return (1);
}

int	op_sti(t_op_context *c, void *arg1, void *arg2, void *arg3)
{
	int n2;
	int n3;
	int target;

	n2 = read_short_int(c->vm, arg2);
	n3 = read_short_int(c->vm, arg3);
	target = (int)mem_mod(c->proc->pc) + (n2 + n3) % IDX_MOD;
	if (c->vm->v_flag & VERBOSE_OPS)
		ft_printf("       | -> store to %d + %d = %d (with pc and mod %d)\n",
				n2, n3, n2 + n3, target);
	target = mem_mod(target);
	t_vm_memcpy(c->vm, &c->vm->mem[target], arg1, REG_SIZE);
	c->changed_memory = target;
	return (1);
}
