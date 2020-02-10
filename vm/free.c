/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpoetess <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/10 15:31:24 by rpoetess          #+#    #+#             */
/*   Updated: 2020/02/10 15:31:26 by rpoetess         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "get_next_line.h"
#include "vm.h"

void		t_vm_destruct(t_vm *vm)
{
	int	i;

	i = -1;
	while (++i < vm->n_champs)
	{
		free(vm->champs[i].name);
		free(vm->champs[i].comment);
	}
	i = -1;
	while (++i < (int)vm->procs.count)
		free(vm->procs.data[i]);
	t_arrayp_del(&vm->procs);
	free(vm->mem);
}
