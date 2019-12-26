#ifndef OPS_H
# define OPS_H

# include "vm.h"

int		op_aff(t_vm *vm, t_proc *proc, void *arg1, void *arg2, void *arg3);
int		op_ld(t_vm *vm, t_proc *proc, void *arg1, void *arg2, void *arg3);

#endif
