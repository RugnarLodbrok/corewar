/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vm2.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpoetess <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/05 19:51:16 by rpoetess          #+#    #+#             */
/*   Updated: 2020/02/05 19:51:18 by rpoetess         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "vm.h"

static void	t_vm_kill_proc(t_vm *vm, t_proc *proc)
{
	int	i;
	int	found_alive;

	proc->dead = 1;
	if (vm->v_flag & VERBOSE_DEATHS)
		ft_printf("Process %u hasn't lived for %u cycles (CTD %d)\n",
				proc->id + 1, vm->i - proc->last_live - 1, vm->cycles_to_die);
	found_alive = 0;
	i = -1;
	while (++i < (int)vm->procs.count)
	{
		proc = vm->procs.data[i];
		found_alive += !proc->dead;
	}
	vm->shutdown = !found_alive;
}

static void	t_vm_death_check(t_vm *vm)
{
	int		i;
	t_proc	*proc;

	i = vm->procs.count;
	while (--i >= 0)
	{
		if ((proc = vm->procs.data[i])->dead)
			continue;
		if ((int)(vm->i - proc->last_live) > vm->cycles_to_die
			|| !proc->last_live)
			t_vm_kill_proc(vm, proc);
	}
	vm->checks_without_delta++;
	if (vm->live_ops_since_check >= NBR_LIVE ||
		vm->checks_without_delta >= MAX_CHECKS)
	{
		vm->checks_without_delta = 0;
		vm->cycles_to_die -= CYCLE_DELTA;
		if (vm->v_flag & VERBOSE_CYCLES)
			ft_printf("Cycle to die is now %d\n", vm->cycles_to_die);
	}
	vm->i_before_check = vm->cycles_to_die > 0 ? vm->cycles_to_die : 1;
	vm->live_ops_since_check = 0;
}

static void	t_vm_proc_step(t_vm *vm, t_proc *proc)
{
	if (!proc->op)
	{
		if (!(proc->op = read_op(&vm->mem[mem_mod(proc->pc)])))
		{
			proc->pc = mem_mod(proc->pc + 1);
			return ;
		}
		proc->delay = proc->op->delay;
		proc->last_pos = proc->pc;
	}
	if (proc->delay)
		proc->delay--;
	if (proc->delay)
		return ;
	t_op_exec(proc->op, proc, vm);
	proc->op = 0;
}

void		t_vm_step(t_vm *vm)
{
	int		i;
	t_proc	*proc;

	if (vm->v_flag & VERBOSE_CYCLES)
		ft_printf("It is now cycle %u\n", vm->i + 1);
	i = (int)vm->procs.count;
	while (--i >= 0)
	{
		if ((proc = vm->procs.data[i])->dead)
			continue;
		t_vm_proc_step(vm, proc);
	}
	vm->i++;
	if (!--vm->i_before_check)
		t_vm_death_check(vm);
}
