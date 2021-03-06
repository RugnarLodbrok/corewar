/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vm.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksticks <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/25 17:47:28 by ksticks           #+#    #+#             */
/*   Updated: 2019/12/25 17:47:29 by ksticks          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "libft_compat.h"
#include "vm.h"
#include "stdio.h"

void		t_vm_add_champ(t_vm *vm, const char *f_name)
{
	uint	pc;
	t_proc	*proc;
	int		n;
	size_t	len;

	n = (int)vm->procs.count;
	pc = (MEM_SIZE / vm->n_champs) * n;
	len = load_bytecode(f_name, (byte *)vm->mem + pc, &vm->champs[n]);
	ft_assert(len <= CHAMP_MAX_SIZE, "champ `%s` size %d > %d\n",
			vm->champs[n].name, len, CHAMP_MAX_SIZE);
	proc = malloc(sizeof(t_proc));
	t_proc_init(proc, n, mem_mod(pc));
	write_uint(vm, UINT_MAX - n, &proc->reg[0][0], 4);
	t_arrayp_push(&vm->procs, proc);
	if (vm->mode == MODE_VIS)
	{
		write_proc_update(proc, vm->champs[n].name);
		write_mem(vm->mem, proc->pc % MEM_SIZE, len, n);
	}
	else if (vm->mode & (MODE_DEFAULT | MODE_DUMP))
		ft_printf("* Player %d, weighing %lu bytes, \"%s\" (\"%s\") !\n",
				n + 1, len, vm->champs[n].name, vm->champs[n].comment);
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
	}
	if (proc->delay)
		proc->delay--;
	if (proc->delay)
		return ;
	t_opc_exec(proc->op, proc, vm);
	proc->op = 0;
}

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
		if ((int)(vm->i - proc->last_live) > vm->cycles_to_die ||
			!proc->last_live)
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
