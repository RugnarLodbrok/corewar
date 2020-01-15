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

short int read_short_int(int host_endian, byte *mem)
{
	short int ret;

	ret = 0;
	ft_memcpy(&ret, mem, sizeof(short int));
	if (VM_ENDIAN != host_endian)
		ft_memrev(&ret, sizeof(short int));
	return (ret);
}

uint read_uint(int host_endian, byte *mem, byte len)
{
	uint ret;

	ret = 0;
	if (host_endian == LITTLE_ENDIAN)
		ft_memcpy(((byte*)&ret) + (4 - len), mem, len);
	else
		ft_memcpy(&ret, mem, len);
	if (VM_ENDIAN != host_endian)
		ft_memrev(&ret, 4);
	return (ret);
}

void write_uint(int host_endian, uint v, byte *mem, byte len)
{
	if (VM_ENDIAN != host_endian)
		ft_memrev(&v, 4);
	if (host_endian == LITTLE_ENDIAN)
		ft_memcpy(mem, ((byte*)&v) + (4 - len), len);
	else
		ft_memcpy(mem, &v, len);
}
