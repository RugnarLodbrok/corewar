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

	for (i = 0; i < MEM_SIZE; ++i)
	{
		if (!(i % OCTETS_PER_LINE))
			ft_printf("0x%04x : ", i);
		put_hex(vm->mem[i], 2);
		ft_printf(" ");
		if (!((i + 1) % OCTETS_PER_LINE))
			ft_printf("\n");
	}
}
