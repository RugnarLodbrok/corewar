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

# include "libft.h"
# include "op.h"

# define VM_ENDIAN BIG_ENDIAN

typedef struct
{
	char *name;
	char *comment;
	uint live;
} t_champ;

typedef struct
{
	uint champ_id;
	uint pc;
	byte reg[REG_NUMBER][4];
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
	int host_endian;
} t_vm;

void load_bytecode(const char *f_name, char *ptr, t_champ *champ);
void t_proc_init(t_proc *proc, t_vm *vm, int n);
int t_op_exec(t_op *op, t_proc *proc, t_vm *vm);
t_op *read_op(const byte *ptr);
void t_vm_init(t_vm *vm, int n_champs);
void t_vm_add_champ(t_vm *vm, const char *f_name);
void t_vm_up(t_vm *vm);
void t_vm_step(t_vm *vm);
void t_vm_destruct(t_vm *vm);

uint read_uint(int host_endian, byte *mem, byte len);
void write_uint(int host_endian, uint v, byte *mem, byte len);

#endif
