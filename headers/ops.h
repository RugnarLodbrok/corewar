/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ops.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpoetess <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/05 19:46:38 by rpoetess          #+#    #+#             */
/*   Updated: 2020/02/05 19:46:40 by rpoetess         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef OPS_H
# define OPS_H

# include "vm.h"

int		op_aff(t_op_context *c, void *arg1, void *arg2, void *arg3);
int		op_ld(t_op_context *c, void *arg1, void *arg2, void *arg3);
int		op_st(t_op_context *c, void *arg1, void *arg2, void *arg3);
int		op_live(t_op_context *c, void *arg1, void *arg2, uint *arg3);
int		op_add(t_op_context *c, void *arg1, void *arg2, void *arg3);
int		op_sub(t_op_context *c, void *arg1, void *arg2, void *arg3);
int		op_and(t_op_context *c, void *arg1, void *arg2, void *arg3);
int		op_or(t_op_context *c, void *arg1, void *arg2, void *arg3);
int		op_xor(t_op_context *c, void *arg1, void *arg2, void *arg3);
int		op_zjmp(t_op_context *c, void *arg1, void *arg2, void *arg3);
int		op_ldi(t_op_context *c, void *arg1, void *arg2, void *arg3);
int		op_sti(t_op_context *c, void *arg1, void *arg2, void *arg3);
int		op_lld(t_op_context *c, void *arg1, void *arg2, void *arg3);
int		op_lldi(t_op_context *c, void *arg1, void *arg2, void *arg3);
int		op_fork(t_op_context *c, void *arg1, void *arg2, void *arg3);
int		op_lfork(t_op_context *c, void *arg1, void *arg2, void *arg3);

#endif
