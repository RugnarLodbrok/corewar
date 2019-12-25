/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vm.h                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksticks <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/25 16:12:17 by ksticks           #+#    #+#             */
/*   Updated: 2019/12/25 16:12:18 by ksticks          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef VM_H
# define VM_H

#include "libft_compat.h"
#include "op.h"

typedef struct {
	uint pc;
	uint reg[REG_NUMBER];
	uint carry;
	uint live;
	uint delay;
} t_proc;

typedef struct
{
	uint i;
	void *mem;
	t_proc *proc;
	int shutdown;
} t_vm;

void t_proc_init(t_proc *proc, int n);

#endif
