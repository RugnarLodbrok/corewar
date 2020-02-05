/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vm_main.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpoetess <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/03 16:03:48 by rpoetess          #+#    #+#             */
/*   Updated: 2020/02/03 16:03:50 by rpoetess         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "get_next_line.h"
#include "vm.h"

static void	loop_print(t_vm *vm)
{
	int		status;
	int		steps;
	char	*line;

	t_vm_print(vm);
	while ((status = get_next_line(STDIN_FILENO, &line)))
	{
		steps = ft_atoi(line);
		free(line);
		if (steps == 0)
			steps = 1;
		while (steps-- > 0)
		{
			if (vm->shutdown)
				return ;
			t_vm_step(vm);
			t_vm_print(vm);
		}
	}
	ft_assert(!status, "can't read stdin");
}

static void	loop_vis(t_vm *vm)
{
	int		status;
	int		i;
	char	*line;

	while ((status = get_next_line(STDIN_FILENO, &line)) > 0)
	{
		i = ft_atoi(line);
		free(line);
		while (i-- > 0)
		{
			if (vm->shutdown)
				return ;
			t_vm_step(vm);
		}
		i = -1;
		while (++i < (int)vm->procs.count)
			write_proc_update(vm->procs.data[i], 0);
		write_cycle(vm->i);
	}
	ft_assert(!status, "can't read stdin");
}

void		main_loop(t_vm *vm, int dump)
{
	if (vm->mode == MODE_DEFAULT)
		while (!vm->shutdown)
			t_vm_step(vm);
	else if (vm->mode == MODE_VIS)
		loop_vis(vm);
	else if (vm->mode == MODE_PRINT)
		loop_print(vm);
	else if (vm->mode == MODE_DUMP)
	{
		while (dump-- > 0 && !vm->shutdown)
			t_vm_step(vm);
		t_vm_print(vm);
	}
}

static void	print_winner(t_vm *vm)
{
	if (vm->mode & (MODE_DEFAULT | MODE_DUMP | MODE_PRINT))
	{
		ft_assert(vm->winner < vm->n_champs, "invalid winner #%d", vm->winner);
		ft_printf("Contestant %d, \"%s\", has won !\n",
				vm->winner + 1, vm->champs[vm->winner].name);
	}
	else if (vm->mode == MODE_VIS)
	{
		ft_printf("type: end\n\n");
	}
}

void		t_args_destruct(t_args args)
{
	int i;

	i = -1;
	while (args.champs[++i])
		free(args.champs);
}

int			main(int ac, char **av)
{
	int		i;
	t_vm	vm;
	t_args	args;

	parse_args(&args, ac, av);
	t_vm_init(&vm, ft_len((void **)&args.champs[0]), args);
	i = -1;
	if (vm.mode & (MODE_DEFAULT | MODE_DUMP))
		ft_printf("Introducing contestants...\n");
	while (args.champs[++i])
		t_vm_add_champ(&vm, args.champs[i]);
	main_loop(&vm, args.dump);
	if (vm.shutdown)
		print_winner(&vm);
	t_vm_destruct(&vm);
	//t_args_destruct(args);
	return (0);
}
