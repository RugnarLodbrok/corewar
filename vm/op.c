/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   op.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zaz <marvin@42.fr>                         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2013/10/04 11:43:01 by zaz               #+#    #+#             */
/*   Updated: 2013/11/06 14:44:35 by zaz              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "op.h"
#include "ops.h"

t_op op_tab[17] = {
	{
		"live",
		1,
		{T_DIR},
		1,
		10,
		"alive",
		op_live,
		0,
		4,
		0,
		0
	},
	{
		"ld",
		2,
		{T_DIR | T_IND, T_REG},
		2,
		5,
		"load",
		op_ld,
		1,
		4,
		1,
		0
	},
	{
		"st",
		2,
		{T_REG, T_IND | T_REG},
		3,
		5,
		"store",
		op_st,
		1,
		4,
		1,
		0
	},
	{
		"add",
		3,
		{T_REG, T_REG, T_REG},
		4,
		10,
		"addition",
		op_add,
		1,
		4,
		1,
		0
	},
	{
		"sub",
		3,
		{T_REG, T_REG, T_REG},
		5,
		10,
		"subtraction",
		op_sub,
		1,
		4,
		1,
		0
	},
	{
		"and",
		3,
		{T_REG | T_DIR | T_IND, T_REG | T_IND | T_DIR, T_REG},
		6,
		6,
		"et (and  r1, r2, r3   r1&r2 -> r3",
		op_and,
		1,
		4,
		1,
		0
	},
	{
		"or",
		3,
		{T_REG | T_IND | T_DIR, T_REG | T_IND | T_DIR, T_REG},
		7,
		6,
		"ou  (or   r1, r2, r3   r1 | r2 -> r3",
		op_or,
		1,
		4,
		1,
		0
	},
	{
		"xor",
		3,
		{T_REG | T_IND | T_DIR, T_REG | T_IND | T_DIR, T_REG},
		8,
		6,
		"ou (xor  r1, r2, r3   r1^r2 -> r3",
		op_xor,
		1,
		4,
		1,
		0
	},
	{
		"zjmp",
		1,
		{T_DIR},
		9,
		20,
		"jump if zero",
		op_zjmp,
		0,
		2,
		0,
		1
	},
	{
		"ldi",
		3,
		{T_REG | T_DIR | T_IND, T_DIR | T_REG, T_REG},
		0x0a,
		25,
		"load index",
		op_ldi,
		1,
		2,
		1,
		1
	},
	{
		"sti",
		3,
		{T_REG, T_REG | T_DIR | T_IND, T_DIR | T_REG},
		0x0b,
		25,
		"store index",
		op_sti,
		1,
		2,
		1,
		1
	},
	{
		"fork",
		1,
		{T_DIR},
		0x0c,
		800,
		"fork",
		op_fork,
		0,
		2,
		0,
		1
	},
	{
		"lld",
		2,
		{T_DIR | T_IND, T_REG},
		0x0d,
		10,
		"long load",
		op_lld,
		1,
		4,
		1,
		0
	},
	{
		"lldi",
		3,
		{T_REG | T_DIR | T_IND, T_DIR | T_REG, T_REG},
		0x0e,
		50,
		"long load index",
		op_lldi,
		1,
		2,
		1,
		1
	},
	{
		"lfork",
		1,
		{T_DIR},
		0x0f,
		1000,
		"long fork",
		op_lfork,
		0,
		2,
		0,
		1
	},
	{
		"aff",
		1,
		{T_REG},
		0x10,
		2,
		"put char",
		op_aff,
		1,
		4,
		1,
		0
	},
	{0, 0, {0}, 0, 0, 0, 0, 0, 0, 0, 0}
};

byte arg_type_to_code[5] = {
		0,
		REG_CODE,
		DIR_CODE,
		0,
		IND_CODE,
};
byte code_to_arg_type[5] = {
		0,
		T_REG,
		T_DIR,
		T_IND,
		0,
};
