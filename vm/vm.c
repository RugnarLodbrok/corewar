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

void t_vm_init(t_vm *vm, int n_champs)
{
	ft_bzero(vm, sizeof(t_vm));
	vm->mode = MODE_DEFAULT;
	vm->n_champs = n_champs;
	vm->mem = ft_calloc(MEM_SIZE, sizeof(char));
	vm->host_endian = endian();
	vm->cycles_to_die = CYCLE_TO_DIE;
	vm->i_before_check = vm->cycles_to_die;
	t_arrayp_init(&vm->procs);
}

void t_vm_print(t_vm *vm)
{
	int i;

	for (i = 0; i < MEM_SIZE; ++i)
	{
		put_hex(vm->mem[i], 2);
		if ((i + 1) % 64)
			ft_printf(" ");
		else
			ft_printf("\n");
	}
}

void t_vm_add_champ(t_vm *vm, const char *f_name)
{
	uint champ_offset;
	t_proc *proc;
	int n;
	size_t len;

	n = (int)vm->procs.count;
	champ_offset = (MEM_SIZE / vm->n_champs) * n;
	len = load_bytecode(f_name, (byte *)vm->mem + champ_offset, &vm->champs[n]);
	proc = malloc(sizeof(t_proc));
	t_proc_init(proc, vm, n);
	t_arrayp_push(&vm->procs, proc);
	if (vm->mode == MODE_VIS)
	{
		write_proc_update(proc, vm->champs[n].name);
		write_mem(vm->mem, proc->pc, len, n);
	}
	else if (vm->mode == MODE_DEFAULT)
	{
		ft_printf("champ: %s\n", vm->champs[n].name);
		ft_printf("champ comment: %s\n", vm->champs[n].comment);
	}
}

static void t_vm_proc_step(t_vm *vm, t_proc *proc)
{
	if (!proc->op)
	{
		if (!(proc->op = read_op(&vm->mem[proc->pc])))
		{
			proc->pc++;
			return;
		}
		proc->delay = proc->op->delay;
	}
	if (proc->delay)
	{
		proc->delay--;
		return;
	}
	t_op_exec(proc->op, proc, vm);
	proc->op = 0;
}

static void t_vm_death_check(t_vm *vm)
{
	int i;
	t_proc *proc;

	for (i = 0; i < (int)vm->procs.count; ++i)
	{
		proc = vm->procs.data[i];
		if (vm->i - proc->last_live > vm->cycles_to_die || !proc->last_live)
		{
			proc->dead = 1;
			if (vm->mode == MODE_VIS)
				write_proc_update(proc, 0);
		}
	}
	if (vm->live_ops_since_check >= NBR_LIVE ||
		vm->checks_without_delta > MAX_CHECKS)
	{
		vm->checks_without_delta = 0;
		vm->cycles_to_die -= CYCLE_DELTA;
	}
	else
		vm->checks_without_delta++;
	if (vm->cycles_to_die < 1)
		vm->cycles_to_die = 1;
	vm->i_before_check = vm->cycles_to_die;
}

void t_vm_step(t_vm *vm)
{
	int i;
	t_proc *proc;

	i = -1;
	while (++i < (int)vm->procs.count)
	{
		if ((proc = vm->procs.data[i])->dead)
			continue;
		t_vm_proc_step(vm, proc);
		if (vm->mode == MODE_VIS)
			write_proc_update(proc, 0);
	}
	vm->i++;
	if (!--vm->i_before_check)
		t_vm_death_check(vm);
}

void t_vm_destruct(t_vm *vm)
{
	free(vm->mem);
	t_arrayp_del(&vm->procs);
}
