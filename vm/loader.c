/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loader.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksticks <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/25 18:30:53 by ksticks           #+#    #+#             */
/*   Updated: 2019/12/25 18:30:55 by ksticks          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include "libft.h"
#include "vm.h"

static void	fd_read_uint(int fd, uint *buff, const char *f_name)
{
	ft_assert(read(fd, buff, 4) == 4, "can't read '%s'", f_name);
	if (endian() == LITTLE_ENDIAN)
		ft_memrev((void *)buff, 4);
}

size_t		load_bytecode(const char *f_name, void *ptr, t_champ *champ)
{
	int		fd;
	uint	buff;
	uint	code_size;

	if ((fd = open(f_name, O_RDONLY)) < 0)
		ft_error_exit("can't open file: %s", f_name);
	champ->name = ft_calloc(PROG_NAME_LENGTH, sizeof(char));
	champ->comment = ft_calloc(COMMENT_LENGTH, sizeof(char));
	fd_read_uint(fd, &buff, f_name);
	ft_assert(buff == COREWAR_EXEC_MAGIC, "magic mismatch");
	ft_assert(read(fd, champ->name, PROG_NAME_LENGTH) == PROG_NAME_LENGTH,
			"can't read '%s'", f_name);
	fd_read_uint(fd, &buff, f_name);
	ft_assert(buff == 0, "corrupted file");
	fd_read_uint(fd, &code_size, f_name);
	ft_assert(read(fd, champ->comment, COMMENT_LENGTH) == COMMENT_LENGTH,
			"can't read '%s'", f_name);
	fd_read_uint(fd, &buff, f_name);
	ft_assert(buff == 0, "corrupted file");
	ft_assert(read(fd, ptr, code_size) == code_size,
			"can't read '%s'", f_name);
	return (code_size);
}
