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

/*
void    *ft_cut_memcpy(t_op_context *pc, void *dst, const void *src, int n)
{
	int     i;
	int     j_start;
	char    *mem_start;
	char    *s;
	char    *d;
	void    *mod_mem;

	j_start = 0;
	i = 0;
	if (!src && !dst)
		return (0);
	s = (char*)src;
	d = (char*)dst;
	while (i < n)
	{
		if (j_start == 0 && pc + i < pc->vm->mem + MEM_SIZE)
			*d++ = *s++;
		else
		{
			while (i < n)
			{
				mem_start[j_start++] = *s++;
				i++;
			}
		}
		i++;
	}
	return (dst);
}*/

/*
void ft_memcpy2(byte*mem, char dst, char src, size_t n)
{
    int i;

    i = -1;
//    printf("%d\n", dst);
//    printf("%d\n", src);
    while(++i < n)
        mem[(dst + i) % MEM_SIZE] = mem[(src + i) % MEM_SIZE];
}
*/
/*
void *ft_memcpy2(byte *mem, void *dst, const void *src, size_t n)
{
    int i;
    char *s;
    char *d;
	char *test;

    i = -1;
    s = (char *)src;
    d = (char *)dst;
    if (!src && !dst)
        return (0);
    if ((t_byte *)dst >= mem && dst < mem + MEM_SIZE)
		while (++i < n)
			mem[((char*)dst - (char*)mem + i) % MEM_SIZE] = mem[((char*)src - (char*)mem + i) % MEM_SIZE];
	else
		while (++i < n)
			d[i] = mem[((char *)src - (char*)mem + i) % MEM_SIZE];
	return (dst);
}
*/
short int read_short_int(t_vm *vm, byte *mem)
{
	short int	ret;

	ret = 0;
	if (vm->host_endian == LITTLE_ENDIAN)
		if (mem < vm->mem || mem >= vm->mem + MEM_SIZE)
			mem += 2;
	t_vm_memcpy(vm, &ret, mem, sizeof(short int));
	if (VM_ENDIAN != vm->host_endian)
		ft_memrev(&ret, sizeof(short int));
	return (ret);
}

uint read_uint(t_vm *vm, byte *mem, byte len)
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

void write_uint(t_vm *vm, uint v, byte *mem, byte len)
{
    /*
<<<<<<< HEAD
	uint	ret;

	ret = 0;
	if (host_endian == LITTLE_ENDIAN)
		//ft_memcpy(((byte*)&ret) + (4 - len), mem, len);
	    ft_memcpy2(mem, ((byte*)&ret) + (4 - len), mem, len);
	else
		//ft_memcpy(&ret, mem, len);
	    ft_memcpy2(mem, &ret, mem, len);
	if (VM_ENDIAN != host_endian)
		ft_memrev(&ret, 4);
	return (ret);
     */
	if (VM_ENDIAN != vm->host_endian)
		ft_memrev(&v, 4);
	if (vm->host_endian == LITTLE_ENDIAN)
		t_vm_memcpy(vm, mem, ((byte *)&v) + (4 - len), len);
	else
		t_vm_memcpy(vm, mem, &v, len);
}

byte *apply_idx_mod(t_op_context *c, byte *ptr)
{
	long int pc;
	int proc_pc;

	pc = ptr - c->vm->mem;
	if (pc < 0 || pc >= MEM_SIZE)
		return (ptr);
	proc_pc = c->proc->pc;
	return &c->vm->mem[((pc - proc_pc) % IDX_MOD + proc_pc) % MEM_SIZE];
}

void t_vm_memcpy(t_vm *vm, void *dst, const void *src, long int n)
{
	long int i;
	byte *ds;
	const byte *sr = src;
	const long int s = (byte*)src - vm->mem;
	const long int d = (byte*)dst - vm->mem;

	ds = dst;
	i = -1;
	if (d >= 0 && d <= MEM_SIZE)
	{
		if (s >= 0 && s <= MEM_SIZE)
			while (++i < n)
				vm->mem[(d + i) % MEM_SIZE] = vm->mem[(s + i) % MEM_SIZE];
		else
			while (++i < n)
				vm->mem[(d + i) % MEM_SIZE] = sr[i];

	}
	else if (s >= 0 && s <= MEM_SIZE)
		while (++i < n)
			ds[i] = vm->mem[(s + i) % MEM_SIZE];
	else
		while (++i < n)
			ds[i] = sr[i];
}
