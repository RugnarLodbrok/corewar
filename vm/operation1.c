/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   operation1.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksticks <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/10 13:55:59 by ksticks           #+#    #+#             */
/*   Updated: 2020/02/10 13:56:00 by ksticks          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vm.h"

t_op		*read_op(const byte *ptr)
{
	int	i;

	i = -1;
	while (g_op_tab[++i].name)
		if (g_op_tab[i].code == *ptr)
			return (&g_op_tab[i]);
	return (0);
}

void		t_opc_init(t_op_context *c, t_vm *vm, t_proc *proc, t_op *op)
{
	ft_bzero(c, sizeof(t_op_context));
	c->op = op;
	c->proc = proc;
	c->vm = vm;
	c->cursor = 1;
	c->changed_memory = -1;
}
