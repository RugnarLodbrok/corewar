/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dissasembler.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cormund <cormund@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/15 17:48:28 by cormund           #+#    #+#             */
/*   Updated: 2020/02/25 14:47:38 by cormund          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "asm.h"

static int			get_number(int size)
{
	int				num;
	int				shift;

	num = 0;
	shift = 8 * size;
	while (shift > 0)
	{
		shift -= 8;
		num |= (((int)*(g_data.data++) & 0xff) << shift);
	}
	return (num);
}

static void			print_args(int op_code, int fd)
{
	unsigned char	code_types;
	int				shift;
	int				n_arg;

	shift = 6;
	code_types = (unsigned char)get_number(1);
	n_arg = g_op_tab[op_code - 1].args_num;
	while (n_arg--)
	{
		if (((code_types >> shift) & 0x3) == REG_CODE)
			ft_fprintf(fd, "r%hhd%s", get_number(1), n_arg ? ", " : "\n");
		else if (((code_types >> shift) & 0x3) == IND_CODE)
			ft_fprintf(fd, "%hd%s", get_number(2), n_arg ? ", " : "\n");
		else if (g_op_tab[op_code - 1].dir_size)
			ft_fprintf(fd, "%%%hd%s", get_number(2), n_arg ? ", " : "\n");
		else
			ft_fprintf(fd, "%%%d%s", get_number(4), n_arg ? ", " : "\n");
		shift -= 2;
	}
}

static void			print_asm_opers(char *end, int fd)
{
	int				op_code;

	while (g_data.data < end)
	{
		op_code = get_number(1);
		ft_fprintf(fd, "%s		", g_op_tab[op_code - 1].name);
		if (g_op_tab[op_code - 1].need_types)
			print_args(op_code, fd);
		else if (g_op_tab[op_code - 1].dir_size)
			ft_fprintf(fd, "%%%hd\n", get_number(2));
		else
			ft_fprintf(fd, "%%%d\n", get_number(4));
	}
}

void				dissasembler(t_champ *champ)
{
	int				fd;
	int				code_size;

	if (g_data.data_size == 0)
		ERROR("Empty file");
	if (g_data.data_size < PROG_NAME_LENGTH + ASM_NULL_SIZE * 2 +\
		ASM_MAGIC_SIZE + COMMENT_LENGTH)
		ERROR("Incorrect file");
	fd = open(champ->file_name, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR |\
											S_IWUSR | S_IRGRP | S_IROTH);
	if (fd == ASM_ERROR)
		ERROR(strerror(errno));
	ft_fprintf(fd, ".name		\"%s\"\n", g_data.data + ASM_MAGIC_SIZE);
	g_data.data += PROG_NAME_LENGTH + ASM_NULL_SIZE + ASM_MAGIC_SIZE;
	code_size = get_number(4);
	if (code_size <= 0 || code_size > CHAMP_MAX_SIZE)
		ERROR("Incorrect file");
	ft_fprintf(fd, ".comment	\"%s\"\n\n", g_data.data);
	g_data.data += COMMENT_LENGTH + ASM_NULL_SIZE;
	print_asm_opers(g_data.data + code_size, fd);
	close(fd);
}
