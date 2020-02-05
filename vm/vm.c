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

void		t_vm_init(t_vm *vm, int n_champs, t_args args)
{
	ft_bzero(vm, sizeof(t_vm));
	vm->mode = args.mode;
	vm->v_flag = args.v_flag;
	vm->n_champs = n_champs;
	vm->mem = ft_calloc(MEM_SIZE, sizeof(char));
	vm->host_endian = endian();
	vm->cycles_to_die = CYCLE_TO_DIE;
	vm->i_before_check = vm->cycles_to_die;
	t_arrayp_init(&vm->procs);
	if (vm->mode == MODE_VIS)
		write_memory(vm);
}

void		t_vm_print(t_vm *vm)
{
	int	i;

	i = -1;
	while (++i < MEM_SIZE)
	{
		if (!(i % OCTETS_PER_LINE))
			ft_printf("0x%04x : ", i);
		put_hex(vm->mem[i], 2);
		ft_printf(" ");
		if (!((i + 1) % OCTETS_PER_LINE))
			ft_printf("\n");
	}
}

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

void		t_vm_destruct(t_vm *vm)
{
	free(vm->mem);
	t_arrayp_del(&vm->procs);
}

uint		mem_mod(long int pc)
{
	while (pc < 0)
		pc += MEM_SIZE;
	return (pc % MEM_SIZE);
}
