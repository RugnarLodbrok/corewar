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

#include "libft.h"
#include "op.h"

typedef struct
{
	char *name;
	char *comment;
} t_champ;
typedef struct
{
	uint champ_id;
	uint pc;
	uint reg[REG_NUMBER];
	uint carry;
	uint live;
	uint delay;
	t_op *op;
} t_proc;

typedef struct
{
	int n_champs;
	t_champ champs[4];
	uint i;
	void *mem;
	t_arrayp procs;
	int shutdown;
} t_vm;

void load_bytecode(const char *f_name, char *ptr, char **name, char **comment);
void t_proc_init(t_proc *proc, int n);
void t_op_exec(t_op *op, t_proc *proc, t_vm *vm);
t_op *read_op(const byte *ptr);
void t_vm_init(t_vm *vm, int n_champs);
void t_vm_add_champ(t_vm *vm, const char *f_name);
void t_vm_up(t_vm *vm);
void t_vm_step(t_vm *vm);
void t_vm_destruct(t_vm *vm);

#endif
