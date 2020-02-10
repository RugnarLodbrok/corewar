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
		write_init();
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

void		put_hex(uint v, int digits)
{
	unsigned int	a;
	unsigned int	b;

	if (!digits)
		return ;
	a = v / 16;
	b = v - a * 16;
	put_hex(a, digits - 1);
	if (b < 10)
		ft_putchar('0' + b);
	else
		ft_putchar('a' - 10 + b);
}
