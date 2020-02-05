/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands_math.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpoetess <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/05 16:45:10 by rpoetess          #+#    #+#             */
/*   Updated: 2020/02/05 16:45:12 by rpoetess         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "vm.h"

int	op_add(t_op_context *c, void *arg1, void *arg2, void *arg3)
{
	int a;
	int b;

	a = read_uint(c->vm, arg1, 4);
	b = read_uint(c->vm, arg2, 4);
	c->proc->carry = (a + b == 0);
	write_uint(c->vm, a + b, arg3, 4);
	return (1);
}

int	op_sub(t_op_context *c, void *arg1, void *arg2, void *arg3)
{
	int a;
	int b;

	a = read_uint(c->vm, arg1, 4);
	b = read_uint(c->vm, arg2, 4);
	c->proc->carry = (a == b);
	write_uint(c->vm, a - b, arg3, 4);
	return (1);
}

int	op_and(t_op_context *c, void *arg1, void *arg2, void *arg3)
{
	uint a;
	uint b;
	uint r;

	a = read_uint(c->vm, apply_idx_mod(c, arg1), 4);
	b = read_uint(c->vm, apply_idx_mod(c, arg2), 4);
	r = a & b;
	c->proc->carry = !r;
	write_uint(c->vm, r, arg3, 4);
	return (1);
}

int	op_or(t_op_context *c, void *arg1, void *arg2, void *arg3)
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

int	op_xor(t_op_context *c, void *arg1, void *arg2, void *arg3)
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
