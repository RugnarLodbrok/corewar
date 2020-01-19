/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   proc.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksticks <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/25 16:28:39 by ksticks           #+#    #+#             */
/*   Updated: 2019/12/25 16:28:41 by ksticks          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vm.h"
#include "libft.h"

void t_proc_init(t_proc *proc, t_vm *vm, int id)
{
	proc->id = id;
	proc->champ_id = id;
	ft_bzero(proc, sizeof(t_proc));
	write_uint(vm->host_endian, UINT_MAX - id - 1, &proc->reg[0][0], 4);
}
