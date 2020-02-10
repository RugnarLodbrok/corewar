/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   endian_conversion.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksticks <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/26 18:06:16 by ksticks           #+#    #+#             */
/*   Updated: 2019/12/26 18:06:18 by ksticks          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vm.h"

short int	read_int16(t_vm *vm, byte *mem)
{
	short int ret;

	ret = 0;
	if (vm->host_endian == LITTLE_ENDIAN)
		if (mem < vm->mem || mem >= vm->mem + MEM_SIZE)
			mem += 2;
	t_vm_memcpy(vm, &ret, mem, sizeof(short int));
	if (VM_ENDIAN != vm->host_endian)
		ft_memrev(&ret, sizeof(short int));
	return (ret);
}

uint		read_uint(t_vm *vm, byte *mem, byte len)
{
	uint ret;

	ret = 0;
	if (vm->host_endian == LITTLE_ENDIAN)
		t_vm_memcpy(vm, ((byte *)&ret) + (4 - len), mem, len);
	else
		t_vm_memcpy(vm, &ret, mem, len);
	if (VM_ENDIAN != vm->host_endian)
		ft_memrev(&ret, 4);
	return (ret);
}

void		write_uint(t_vm *vm, uint v, byte *mem, byte len)
{
	if (VM_ENDIAN != vm->host_endian)
		ft_memrev(&v, 4);
	if (vm->host_endian == LITTLE_ENDIAN)
		t_vm_memcpy(vm, mem, ((byte *)&v) + (4 - len), len);
	else
		t_vm_memcpy(vm, mem, &v, len);
}

byte		*apply_idx_mod(t_op_context *c, byte *ptr)
{
	long int	pc;
	int			proc_pc;

	pc = ptr - c->vm->mem;
	if (pc < 0 || pc >= MEM_SIZE)
		return (ptr);
	proc_pc = c->proc->pc;
	return (&c->vm->mem[mem_mod((pc - proc_pc) % IDX_MOD + proc_pc)]);
}

void		t_vm_memcpy(t_vm *vm, void *dst, const void *src, long int n)
{
	long int	i;
	byte		*ds;
	const byte	*sr = src;
	long int	s;
	long int	d;

	s = ((byte *)src - vm->mem);
	d = ((byte *)dst - vm->mem);
	ds = dst;
	i = -1;
	if (d >= 0 && d <= MEM_SIZE)
	{
		if (s >= 0 && s <= MEM_SIZE)
			while (++i < n)
				vm->mem[mem_mod(d + i)] = vm->mem[mem_mod(s + i)];
		else
			while (++i < n)
				vm->mem[mem_mod(d + i)] = sr[i];
	}
	else if (s >= 0 && s <= MEM_SIZE)
		while (++i < n)
			ds[i] = vm->mem[mem_mod(s + i)];
	else
		while (++i < n)
			ds[i] = sr[i];
}
