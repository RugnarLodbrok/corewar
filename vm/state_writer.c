#include "libft.h"
#include "vm.h"

static void put_hex(uint v, int digits)
{
	unsigned int a;
	unsigned int b;

	if (!digits)
		return;
	a = v / 16;
	b = v - a * 16;
	put_hex(a, digits - 1);
	if (b < 10)
		ft_putchar('0' + b);
	else
		ft_putchar('a' - 10 + b);
}

void write_memory(t_vm *vm)
{
	int i;

	if (vm->mode == MODE_DEFAULT)
		ft_print_memory(vm->mem, 64);
	if (vm->mode == MODE_VIS)
	{
		ft_printf("type: mem_init\n");
		ft_printf("data: \"");
		for (i = 0; i < MEM_SIZE; ++i)
		{
			put_hex(*&vm->mem[i], 2);
		}
		ft_printf("\"\n\n");
	}
}

void write_proc_pos_update(t_vm *vm, int proc_num)
{
	t_proc *proc;
	if (vm->mode == MODE_VIS)
	{
		proc = vm->procs.data[proc_num];
		ft_printf("type: proc_move\n");
		ft_printf("id: %d\n", proc_num);
		ft_printf("delay: %d\n", proc->delay);
		ft_printf("pc: %d\n\n", proc->pc);
	}
}

void write_proc_stdout(t_vm *vm, int proc_num, char c)
{
	ft_printf("type: arr\n");
	ft_printf("id: %d\n", proc_num);
	ft_printf("char: %c\n\n", c);
}

void write_new_proc(int id, char *name, int pc)
{
	ft_printf("type: new_proc\n");
	ft_printf("id: %d\n", id);
	ft_printf("name: %s\n", name);
	ft_printf("pc: %d\n\n", pc);
}

void write_mem(byte *mem, int pc, size_t len)
{
	size_t i;

	ft_printf("type: write_mem\n");
	ft_printf("pc: %d\n", pc);
	ft_printf("data: \"");
	for (i = 0; i < len; ++i)
		put_hex(mem[pc + i], 2);
	ft_printf("\"\n\n");
}

void write_end(void)
{
	ft_printf("type: end\n\n");
}
