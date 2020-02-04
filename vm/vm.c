/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vm.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksticks <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/25 17:47:28 by ksticks           #+#    #+#             */
/*   Updated: 2019/12/25 17:47:29 by ksticks          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "libft_compat.h"
#include "vm.h"
#include "stdio.h"

void		t_vm_init(t_vm *vm, int n_champs, t_args args)
{
	ft_bzero(vm, sizeof(t_vm));
	vm->mode = args.mode;
	vm->v_flag = args.v_flag;
	vm->n_champs = n_champs;
	vm->mem = ft_calloc(MEM_SIZE, sizeof(char));
	vm->host_endian = endian();
	vm->cycles_to_die = CYCLE_TO_DIE;
	vm->i_before_check = vm->cycles_to_die;
	t_arrayp_init(&vm->procs);
	if (vm->mode == MODE_VIS)
		write_memory(vm);
}

void		t_vm_print(t_vm *vm)
{
	int	i;

	for (i = 0; i < MEM_SIZE; ++i)
	{
		if (!(i % OCTETS_PER_LINE))
			ft_printf("0x%04x : ", i);
		put_hex(vm->mem[i], 2);
		ft_printf(" ");
		if (!((i + 1) % OCTETS_PER_LINE))
			ft_printf("\n");
	}
}

void		t_vm_add_champ(t_vm *vm, const char *f_name)
{
	uint	pc;
	t_proc	*proc;
	int		n;
	size_t	len;

	n = (int)vm->procs.count;
	pc = (MEM_SIZE / vm->n_champs) * n;
	len = load_bytecode(f_name, (byte *)vm->mem + pc, &vm->champs[n]);
	ft_assert(len <= CHAMP_MAX_SIZE, "champ `%s` size %d > %d\n",
			vm->champs[n].name, len, CHAMP_MAX_SIZE);
	proc = malloc(sizeof(t_proc));
	t_proc_init(proc, n, mem_mod(pc));
	write_uint(vm, UINT_MAX - n, &proc->reg[0][0], 4);
	t_arrayp_push(&vm->procs, proc);
	if (vm->mode == MODE_VIS)
	{
		write_proc_update(proc, vm->champs[n].name);
		write_mem(vm->mem, proc->pc % MEM_SIZE, len, n);
	}
	else if (vm->mode & (MODE_DEFAULT | MODE_DUMP))
		ft_printf("* Player %d, weighing %lu bytes, \"%s\" (\"%s\") !\n",
				n + 1, len, vm->champs[n].name, vm->champs[n].comment);
}

static void	t_vm_proc_step(t_vm *vm, t_proc *proc)
{
	if (!proc->op)
	{
		if (!(proc->op = read_op(&vm->mem[mem_mod(proc->pc)])))
		{
			proc->pc = mem_mod(proc->pc + 1);
			return ;
		}
		proc->delay = proc->op->delay;
		proc->last_pos = proc->pc;
	}
	if (proc->delay)
		proc->delay--;
	if (proc->delay)
		return ;
	t_op_exec(proc->op, proc, vm);
	proc->op = 0;
}

static void	t_vm_kill_proc(t_vm *vm, t_proc *proc)
{
	int	i;
	int	found_alive;

	proc->dead = 1;
	if (vm->v_flag & VERBOSE_DEATHS)
		ft_printf("Process %u hasn't lived for %u cycles (CTD %u)\n",
				proc->id, vm->i, vm->cycles_to_die);
	found_alive = 0;
	i = -1;
	while (++i < (int)vm->procs.count)
	{
		proc = vm->procs.data[i];
		found_alive += !proc->dead;
	}
	vm->shutdown = !found_alive;
}

static void	t_vm_death_check(t_vm *vm)
{
	int		i;
	t_proc	*proc;

	for (i = 0; i < (int)vm->procs.count; ++i)
	{
		proc = vm->procs.data[i];
		if (vm->i - proc->last_live > vm->cycles_to_die || !proc->last_live)
			t_vm_kill_proc(vm, proc);
	}
	if (vm->live_ops_since_check >= NBR_LIVE ||
		vm->checks_without_delta > MAX_CHECKS)
	{
		vm->checks_without_delta = 0;
		vm->cycles_to_die -= CYCLE_DELTA;
		if (vm->v_flag & VERBOSE_CYCLES)
			ft_printf("Cycle to die is now %u\n", vm->cycles_to_die);
	}
	else
		vm->checks_without_delta++;
	if (vm->cycles_to_die < 1)
		vm->cycles_to_die = 1;
	vm->i_before_check = vm->cycles_to_die;
}

void		t_vm_step(t_vm *vm)
{
	int		i;
	t_proc	*proc;
	int		proc_cnt;

	if (vm->v_flag & VERBOSE_CYCLES)
		ft_printf("It is now cycle %u\n", vm->i + 1);
	i = -1;
	proc_cnt = (int)vm->procs.count;
	while (++i < proc_cnt)
	{
		if ((proc = vm->procs.data[i])->dead)
			continue;
		t_vm_proc_step(vm, proc);
	}
	vm->i++;
	if (vm->mode == MODE_VERBOSE && vm->v_flag & VERBOSE_CYCLES)
		ft_printf("It is now cycle %d\n", vm->i);
	if (!--vm->i_before_check)
		t_vm_death_check(vm);
}

void		t_vm_destruct(t_vm *vm)
{
	free(vm->mem);
	t_arrayp_del(&vm->procs);
}

uint		mem_mod(long int pc)
{
	while (pc < 0)
		pc += MEM_SIZE;
	return (pc % MEM_SIZE);
}
