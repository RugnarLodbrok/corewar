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
#include "vm.h"

void t_vm_init(t_vm *vm, int n_champs)
{
	ft_bzero(vm, sizeof(t_vm));
	vm->n_champs = n_champs;
	vm->mem = ft_calloc(MEM_SIZE, sizeof(char));
	t_arrayp_init(&vm->procs);
}

void t_vm_add_champ(t_vm *vm, const char *f_name)
{
	uint champ_offset;
	t_proc proc;
	int n;

	n = vm->procs.count;
	champ_offset = (MEM_SIZE / vm->n_champs) * n;
	load_bytecode(f_name, vm->mem + champ_offset,
			&vm->champs[n].name, &vm->champs[n].comment);
	t_proc_init(&proc, n);
	t_arrayp_push(&vm->procs, &proc);
}

void t_vm_up(t_vm *vm)
{
//	vm->proc = ft_calloc(1, sizeof(t_proc));
//	t_proc_init(vm->proc, 1);
}

void t_vm_step(t_vm *vm)
{
	int i;
	t_proc *proc;

	ft_printf("cycle %u\n", vm->i);
	i = -1;
	while(++i < vm->procs.count)
	{
		proc = vm->procs.data[i];
		if (!proc->op)
		{
			proc->op = read_op(&vm->mem[proc->pc]);
			proc->delay = proc->op->delay;
		}
		if (proc->delay)
		{
			proc->delay--;
			continue;
		}
		t_op_exec(proc->op, proc, vm);
		proc->op = 0;
	}
	vm->i++;
	if (vm->i > 1024)
		vm->shutdown = 1;
}

void t_vm_destruct(t_vm *vm)
{
	free(vm->mem);
	t_arrayp_del(&vm->procs);
}
