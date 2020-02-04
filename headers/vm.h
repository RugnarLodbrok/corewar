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

# define VM_USAGE "USGE: ./corewar [-i | -p] [-v V] [-d N] [[-n N] CHAMP] ..."
# define OCTETS_PER_LINE 64

# define VM_ENDIAN BIG_ENDIAN
# define MODE_VERBOSE 16
# define MODE_DUMP 8
# define MODE_PRINT 4
# define MODE_VIS 2
# define MODE_DEFAULT 1

# define VERBOSE_LIVES 1
# define VERBOSE_CYCLES 2
# define VERBOSE_OPS 4
# define VERBOSE_DEATHS 8
# define VERBOSE_PC 16

typedef struct
{
	const char *champs[5];
	int dump;
	int v_flag;
	uint mode;
} t_args;

typedef struct
{
	char *name;
	char *comment;
	uint live;
} t_champ;

typedef struct
{
    uint mark;
	uint id;
	uint champ_id;
	uint pc;
	uint last_pos;
	byte args[3];
	byte reg[REG_NUMBER][REG_SIZE];
	uint carry;
	uint last_live;
	uint delay;
	int dead;
	t_op *op;
} t_proc;

typedef struct
{
	uint mode;
	int dump;
	int v_flag;
	int n_champs;
	t_champ champs[4];
	int winner;
	uint i;
	byte *mem;
	t_arrayp procs;
	int cycles_to_die;
	uint i_before_check;
	uint live_ops_since_check;
	uint checks_without_delta;
	int shutdown;
	int host_endian;
} t_vm;

typedef struct
{
	t_op *op;
	t_vm *vm;
	t_proc *proc;
	uint cursor;
	int changed_memory;
	int invalid_args;
} t_op_context;

void parse_args(t_args *args, int ac, char **av);
size_t load_bytecode(const char *f_name, void *ptr, t_champ *champ);
void t_proc_init(t_proc *proc, int id, uint pc);
int t_op_exec(t_op *op, t_proc *proc, t_vm *vm);
t_op *read_op(const byte *ptr);
void t_vm_init(t_vm *vm, int n_champs, t_args args);
void t_vm_add_champ(t_vm *vm, const char *f_name);
void t_vm_step(t_vm *vm);
void t_vm_destruct(t_vm *vm);
void t_vm_print(t_vm *vm);

void put_hex(uint v, int digits);
void write_memory(t_vm *vm);
void write_proc_update(t_proc *proc, const char *name);
void write_proc_stdout(t_vm *vm, int proc_num, char c);
void write_mem(byte *mem, uint pc, size_t len, int proc_id);
void write_cycle(uint i);

short int read_short_int(t_vm *vm, byte *mem);
uint read_uint(t_vm *vm, byte *mem, byte len);
void write_uint(t_vm *vm, uint v, byte *mem, byte len);
byte *apply_idx_mod(t_op_context *c, byte *ptr);
void t_vm_memcpy(t_vm *vm, void *dst, const void *src, long int n);
uint mem_mod(long int pc);

#endif
