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

void write_proc_update(t_vm *vm, int proc_num, const char *name)
{
	int i;
	t_proc *proc;
	if (vm->mode == MODE_VIS)
	{
		proc = vm->procs.data[proc_num];
		ft_printf("type: proc_update\n");
		ft_printf("id: %d\n", proc_num);
		if (name)
			ft_printf("name: %s\n", name);
		if (proc->op)
			ft_printf("op: %s\n", proc->op->name);
		else
			ft_printf("op: %s\n", "null");
		ft_printf("delay: %d\n", proc->delay);
		ft_printf("pc: %d\n", proc->pc);
		ft_printf("registers:\n");
		for (i = 0; i < REG_NUMBER; ++i)
			ft_printf("  - \"%02hhx%02hhx%02hhx%02hhx\"\n",
					proc->reg[i][0], proc->reg[i][1],
					proc->reg[i][2], proc->reg[i][3]);
		ft_printf("\n");
	}
}

void write_proc_stdout(t_vm *vm, int proc_num, char c)
{
	(void)vm;
	ft_printf("type: arr\n");
	ft_printf("id: %d\n", proc_num);
	ft_printf("char: \"%c\"\n\n", c);
}

void write_mem(byte *mem, int pc, size_t len, int proc_id)
{
	size_t i;

	ft_printf("type: write_mem\n");
	ft_printf("pc: %d\n", pc);
	if (proc_id >= 0)
		ft_printf("proc_id: %d\n", proc_id);
	ft_printf("data: \"");
	for (i = 0; i < len; ++i)
		put_hex(mem[i], 2);
	ft_printf("\"\n\n");
}

void write_end(void)
{
	ft_printf("type: end\n\n");
}
