#ifndef OPS_H
# define OPS_H

# include "vm.h"

int		op_aff(t_vm *vm, t_proc *proc, void *arg1, void *arg2, void *arg3);
int		op_ld(t_vm *vm, t_proc *proc, void *arg1, void *arg2, void *arg3);
int		op_st(t_vm *vm, t_proc *proc, void *arg1, void *arg2, void *arg3);
int		op_live(t_vm *vm, t_proc *proc, void *arg1, void *arg2, uint *arg3);
int 	op_add(t_vm *vm, t_proc *proc, void *arg1, void *arg2, void *arg3);
int		op_sub(t_vm *vm, t_proc *proc, void *arg1, void *arg2, void *arg3);
int 	op_and(t_vm *vm, t_proc *proc, void *arg1, void *arg2, void *arg3);
int 	op_or(t_vm *vm, t_proc *proc, void *arg1, void *arg2, void *arg3);
int 	op_xor(t_vm *vm, t_proc *proc, void *arg1, void *arg2, void *arg3);
int 	op_zjmp(t_vm *vm, t_proc *proc, void *arg1, void *arg2, void *arg3);
int 	op_ldi(t_vm *vm, t_proc *proc, void *arg1, void *arg2, void *arg3);
int 	op_sti(t_vm *vm, t_proc *proc, void *arg1, void *arg2, void *arg3);
int 	op_lld(t_vm *vm, t_proc *proc, void *arg1, void *arg2, void *arg3);
int		op_lldi(t_vm *vm, t_proc *proc, void *arg1, void *arg2, void *arg3);
int		op_lfork(t_vm *vm, t_proc *proc, void *arg1, void *arg2, void *arg3);

#endif
