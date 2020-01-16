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
# include "libft_compat.h"
# include "op.h"

# define VM_ENDIAN BIG_ENDIAN
# define MODE_VIS 1
# define MODE_DEFAULT 0

typedef struct
{
	char *name;
	char *comment;
	uint live;
} t_champ;

typedef struct
{
	uint id;
	uint champ_id;
	uint pc;
	byte reg[REG_NUMBER][REG_SIZE];
	uint carry;
	uint live;
	uint delay;
	t_op *op;
} t_proc;

typedef struct
{
	uint mode;
	int n_champs;
	t_champ champs[4];
	uint i;
	byte *mem;
	t_arrayp procs;
	int shutdown;
	int host_endian;
} t_vm;

typedef struct
{
	t_op *op;
	t_vm *vm;
	t_proc *proc;
	uint cursor;
	int ind_arg;
	byte ind_val[REG_SIZE];
} t_op_context;

size_t load_bytecode(const char *f_name, void *ptr, t_champ *champ);
void t_proc_init(t_proc *proc, t_vm *vm, int n);
int t_op_exec(t_op *op, t_proc *proc, t_vm *vm);
t_op *read_op(const byte *ptr);
void t_vm_init(t_vm *vm, int n_champs);
void t_vm_add_champ(t_vm *vm, const char *f_name);
void t_vm_step(t_vm *vm);
void t_vm_destruct(t_vm *vm);
void t_vm_print(t_vm *vm, const char *format, ...);

void write_memory(t_vm *vm);
void write_proc_update(t_vm *vm, int proc_num, const char *name);
void write_proc_stdout(t_vm *vm, int proc_num, char c);
void write_mem(byte *mem, int pc, size_t len, int proc_id);
void write_end(void);

short int read_short_int(int host_endian, byte *mem);
uint read_uint(int host_endian, byte *mem, byte len);
void write_uint(int host_endian, uint v, byte *mem, byte len);

#endif
