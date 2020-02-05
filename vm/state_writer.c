/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   state_writer.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpoetess <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/03 16:06:45 by rpoetess          #+#    #+#             */
/*   Updated: 2020/02/03 16:06:49 by rpoetess         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "vm.h"

void	put_hex(uint v, int digits)
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

void	write_memory(t_vm *vm)
{
	int	i;

	i = 0;
	ft_printf("type: mem_init\n");
	ft_printf("data: \"");
	while (++i < MEM_SIZE)
		put_hex(*&vm->mem[i], 2);
	ft_printf("\"\n\n");
}

void	write_proc_update(t_proc *proc, const char *name)
{
	int	i;

	i = -1;
	ft_printf("type: proc_update\n");
	ft_printf("id: %d\n", proc->id);
	if (proc->dead)
	{
		ft_printf("dead : 1\n\n");
		return ;
	}
	if (name)
		ft_printf("name: %s\n", name);
	ft_printf("champ_id: %d\n", proc->champ_id);
	if (proc->op)
		ft_printf("op: %s\n", proc->op->name);
	else
		ft_printf("op: %s\n", "null");
	ft_printf("delay: %d\n", proc->delay);
	ft_printf("pc: %d\n", mem_mod(proc->pc));
	ft_printf("registers:\n");
	while (++i < REG_NUMBER)
		ft_printf("  - \"%02hhx%02hhx%02hhx%02hhx\"\n",
				proc->reg[i][0], proc->reg[i][1],
				proc->reg[i][2], proc->reg[i][3]);
	ft_printf("\n");
}

void	write_proc_stdout(t_vm *vm, int proc_num, char c)
{
	(void)vm;
	ft_printf("type: arr\n");
	ft_printf("id: %d\n", proc_num);
	ft_printf("char: \"%c\"\n\n", c);
}

void	write_mem(byte *mem, uint pc, size_t len, int proc_id)
{
	size_t	i;

	i = -1;
	ft_printf("type: write_mem\n");
	ft_printf("pc: %d\n", mem_mod(pc));
	if (proc_id >= 0)
		ft_printf("proc_id: %d\n", proc_id);
	ft_printf("data: \"");
	while (++i < len)
		put_hex(mem[mem_mod((long)(pc + i))], 2);
	ft_printf("\"\n\n");
}

void	write_cycle(uint i)
{
	ft_printf("type: cycle\n");
	ft_printf("value: %u\n\n", i);
}
