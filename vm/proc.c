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

void t_proc_init(t_proc *proc, int id, int pc)
{
	ft_bzero(proc, sizeof(t_proc));
	proc->id = id;
	proc->champ_id = id;
	proc->pc = pc % MEM_SIZE;
}
