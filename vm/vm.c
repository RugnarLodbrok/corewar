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
#include "stdarg.h"
#include "stdio.h"

void t_vm_init(t_vm* vm, int n_champs)
{
	ft_bzero(vm, sizeof(t_vm));
	vm->mode = MODE_DEFAULT;
	vm->n_champs = n_champs;
	vm->mem = ft_calloc(MEM_SIZE, sizeof(char));
	vm->host_endian = endian();
	t_arrayp_init(&vm->procs);
}

void t_vm_print(t_vm* vm, const char* format, ...)
{
	va_list ap;

	if (vm->mode == MODE_DEFAULT)
	{
				va_start(ap, format);
		ft_printf_ap(STDOUT_FILENO, format, ap);
				va_end(ap);
	}
}

void t_vm_add_champ(t_vm* vm, const char* f_name)
{
	uint champ_offset;
	t_proc* proc;
	int n;
	size_t len;

	n = (int)vm->procs.count;
	champ_offset = (MEM_SIZE / vm->n_champs) * n;
	len = load_bytecode(f_name, (byte*)vm->mem + champ_offset, &vm->champs[n]);
	proc = malloc(sizeof(t_proc));
	t_proc_init(proc, vm, n);
	t_arrayp_push(&vm->procs, proc);
	if (vm->mode == MODE_VIS)
	{
		write_proc_update(vm, n, vm->champs[n].name);
		write_mem(vm->mem, proc->pc, len);
	}
	else if (vm->mode == MODE_DEFAULT)
	{
		ft_printf("champ: %s\n", vm->champs[n].name);
		ft_printf("champ comment: %s\n", vm->champs[n].comment);
	}
}

void t_vm_step(t_vm* vm)
{
	int i;
	t_proc* proc;

	i = -1;
	while (++i < vm->procs.count)
	{
		proc = vm->procs.data[i];
		if (!proc->op)
		{
			if (!(proc->op = read_op(&vm->mem[proc->pc])))
			{
				vm->shutdown = 1;
				break;
			}
			proc->delay = proc->op->delay;
			write_proc_update(vm, i, 0);
		}
		if (proc->delay)
		{
			proc->delay--;
			continue;
		}
		if (!t_op_exec(proc->op, proc, vm))
		{
			ft_printf("invalid op\n");
			vm->shutdown = 1;
		}
		proc->op = 0;
		write_proc_update(vm, i, 0);
	}
	vm->i++;
	if (vm->i > 1024)
		vm->shutdown = 1;
}

void t_vm_destruct(t_vm* vm)
{
	free(vm->mem);
	t_arrayp_del(&vm->procs);
}
