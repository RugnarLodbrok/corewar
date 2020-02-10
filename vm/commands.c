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

int	op_live(t_op_context *c, void *arg1, void *arg2, void *arg3)
{
	uint reg_value;
	uint champ_id;

	(void)arg2;
	(void)arg3;
	c->proc->last_live = c->vm->i;
	c->vm->live_ops_since_check++;
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

int	op_zjmp(t_op_context *c, void *arg1, void *arg2, void *arg3)
{
	(void)arg2;
	(void)arg3;
	if (c->proc->carry)
		c->proc->pc = mem_mod(
				c->proc->pc + read_int16(c->vm, arg1) % IDX_MOD);
	return (1);
}

int	op_lfork(t_op_context *c, void *arg1, void *arg2, void *arg3)
{
	t_proc *p;

	(void)arg2;
	(void)arg3;
	p = (t_proc *)malloc(sizeof(t_proc));
	t_vm_memcpy(c->vm, p, c->proc, sizeof(t_proc));
	p->id = c->vm->procs.count;
	p->pc = mem_mod(p->pc + read_int16(c->vm, arg1));
	p->op = 0;
	if (c->vm->mode == MODE_VIS)
		write_proc_update(p, c->vm->champs[p->champ_id].name);
	t_arrayp_push(&c->vm->procs, p);
	return (1);
}

int	op_fork(t_op_context *c, void *arg1, void *arg2, void *arg3)
{
	return (op_lfork(c, apply_idx_mod(c, arg1), arg2, arg3));
}

int	op_aff(t_op_context *c, void *arg1, void *arg2, void *arg3)
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
