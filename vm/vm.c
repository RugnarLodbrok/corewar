#include "libft.h"
#include "vm.h"

void t_vm_init(t_vm *vm)
{
	ft_bzero(vm, sizeof(t_vm));
	vm->mem = ft_calloc(MEM_SIZE, sizeof(char));
	vm->proc = ft_calloc(1, sizeof(t_proc));
	t_proc_init(vm->proc, 1);
}

void t_vm_step(t_vm *vm)
{
	ft_printf("cycle %u\n", vm->i);
	vm->i++;
	if (vm->i > 1024)
		vm->shutdown = 1;
}

void t_vm_destruct(t_vm *vm)
{
	free(vm->mem);
	free(vm->proc);
}

int main(void)
{
	t_vm vm;

	t_vm_init(&vm);
	while(!vm.shutdown)
		t_vm_step(&vm);
	t_vm_destruct(&vm);
	return (0);
}
