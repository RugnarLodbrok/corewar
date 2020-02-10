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

# define VM_USAGE "USGE: ./corewar [-i | -p] [-v V] [-d N] [[-n N] CHAMP] ..." \
"\n\t-i yaml-output; used for vis\n\t-d dump after N cycles" \
"\n\t-v verbosity; show: \t1 lives\n\t\t\t\t2 cycles" \
"\n\t\t\t\t4 ops\n\t\t\t\t8 deaths\n\t\t\t\t16 pc"
//# define VM_USAGE_1 "\n\t-i yaml-output; used for vis\n\t-d dump after N cycles"
//# define VM_USAGE_2 "\n\t-v verbosity; show: \t1 lives\n\t\t\t\t2 cycles"
//# define VM_USAGE_3 "\n\t\t\t\t4 ops\n\t\t\t\t8 deaths\n\t\t\t\t16 pc"
//# define VM_USAGE VM_USAGE_0 VM_USAGE_1 VM_USAGE_2 VM_USAGE_3

# define OCTETS_PER_LINE 64

# define VM_ENDIAN BIG_ENDIAN
# define MODE_DUMP 8
# define MODE_PRINT 4
# define MODE_VIS 2
# define MODE_DEFAULT 1

# define VERBOSE_LIVES 1
# define VERBOSE_CYCLES 2
# define VERBOSE_OPS 4
# define VERBOSE_DEATHS 8
# define VERBOSE_PC 16

typedef struct	s_args
{
	const char	*champs[5];
	int			dump;
	int			v_flag;
	uint		mode;
}				t_args;

typedef struct	s_champs
{
	char		*name;
	char		*comment;
	uint		live;
}				t_champ;

typedef struct	s_proc
{
	uint		mark;
	uint		id;
	uint		champ_id;
	uint		pc;
	uint		last_pos;
	byte		args[3];
	byte		reg[REG_NUMBER][REG_SIZE];
	uint		carry;
	uint		last_live;
	uint		delay;
	int			dead;
	t_op		*op;
}				t_proc;

typedef struct	s_vm
{
	uint		mode;
	int			dump;
	int			v_flag;
	int			n_champs;
	t_champ		champs[4];
	int			winner;
	uint		i;
	byte		*mem;
	t_arrayp	procs;
	int			cycles_to_die;
	uint		i_before_check;
	uint		live_ops_since_check;
	uint		checks_without_delta;
	int			shutdown;
	int			host_endian;
}				t_vm;

typedef struct	s_op_context
{
	t_op		*op;
	t_vm		*vm;
	t_proc		*proc;
	uint		cursor;
	int			changed_memory;
	int			invalid_args;
}				t_op_context;

t_op			g_op_tab[17];
byte			g_code_to_arg_type[5];
byte			g_arg_type_to_code[5];

void			parse_args(t_args *args, int ac, char **av);
size_t			load_bytecode(const char *f_name, void *ptr, t_champ *champ);
void			t_proc_init(t_proc *proc, int id, uint pc);
void			t_opc_init(t_op_context *c, t_vm *vm, t_proc *proc, t_op *op);
void			t_opc_exec(t_op *op, t_proc *proc, t_vm *vm);
t_op			*read_op(const byte *ptr);
void			t_vm_init(t_vm *vm, int n_champs, t_args args);
void			t_vm_add_champ(t_vm *vm, const char *f_name);
void			t_vm_step(t_vm *vm);
void			t_vm_destruct(t_vm *vm);
void			t_vm_print(t_vm *vm);

void			put_hex(uint v, int digits);
void			write_init(void);
void			write_proc_update(t_proc *proc, const char *name);
void			write_proc_stdout(t_vm *vm, int proc_num, char c);
void			write_mem(byte *mem, uint pc, size_t len, int proc_id);
void			write_cycle(uint i);

short int		read_int16(t_vm *vm, byte *mem);
uint			read_uint(t_vm *vm, byte *mem, byte len);
void			write_uint(t_vm *vm, uint v, byte *mem, byte len);
byte			*apply_idx_mod(t_op_context *c, byte *ptr);
void			t_vm_memcpy(t_vm *vm, void *dst, const void *src, long int n);
uint			mem_mod(long int pc);

#endif
