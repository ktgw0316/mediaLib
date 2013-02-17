/*
 * CDDL HEADER START
 *
 * The contents of this file are subject to the terms of the
 * Common Development and Distribution License (the "License").
 * You may not use this file except in compliance with the License.
 *
 * You can obtain a copy of the license at usr/src/OPENSOLARIS.LICENSE
 * or http://www.opensolaris.org/os/licensing.
 * See the License for the specific language governing permissions
 * and limitations under the License.
 *
 * When distributing Covered Code, include this CDDL HEADER in each
 * file and include the License file at usr/src/OPENSOLARIS.LICENSE.
 * If applicable, add the following below this CDDL HEADER, with the
 * fields enclosed by brackets "[]" replaced with your own identifying
 * information: Portions Copyright [yyyy] [name of copyright owner]
 *
 * CDDL HEADER END
 */

/*
 * Copyright 2007 Sun Microsystems, Inc.  All rights reserved.
 * Use is subject to license terms.
 */

#pragma ident	"@(#)mlib_s_ImageConstNotOr_Bit.c	9.3	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_ImageConstNotOr_Bit - image logical operation with constant
 *                                 (type of image - BIT)
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageConstNotOr_Bit(mlib_image       *dst,
 *                                           const mlib_image *src,
 *                                           const mlib_s32   *c);
 *
 * ARGUMENT
 *      dst     pointer to output image
 *      src     pointer to input image
 *      c       array of constants for each channel
 *
 * DESCRIPTION
 *      C version file for one of the following operations:
 *
 *      And  dst(i, j) = c & src(i, j)
 *      Or  dst(i, j) = c | src(i, j)
 *      Xor  dst(i, j) = c ^ src(i, j)
 *      NotAnd  dst(i, j) = ~(c & src(i, j))
 *      NotOr  dst(i, j) = ~(c | src(i, j))
 *      NotXor  dst(i, j) = ~(c ^ src(i, j))
 *      AndNot  dst(i, j) = c & (~src(i, j))
 *      OrNot  dst(i, j) = c | (~src(i, j))
 *
 * NOTE
 *      Because of the similarity among the 8 functions, one header
 *      file mlib_s_ImageConstLogic_Bit.h is used in the 8 files
 *      (mlib_ImageConst*_Bit.c) for the 8 similar functions.
 */

/* *********************************************************** */

#if 0
#define	CONSTLOGIC_bit(c, a)	~((c) | (a))
#define	CONSTLOGIC(c, a)	~((c) | (a))
#define	CONSTLOGIC_SSE2(c, a)	_mm_andnot_si128(_mm_or_si128(c, a), ones)
#endif

/* C = ~c */

#define	REV_CONST
#define	CONSTLOGIC_bit(c, a)	~((c) | (a))
#define	CONSTLOGIC(C, a)	(C) & (~(a))
#define	CONSTLOGIC_SSE2(C, a)	_mm_andnot_si128(a, C)


/* *********************************************************** */

#define	CONST_LOGIC_BITS                                                   \
	mlib_status                                                        \
	mlib_ImageConstNotOr_Bit(mlib_image *dst, const mlib_image *src,   \
	    const mlib_s32 *c)

#include <mlib_s_ImageConstLogic_Bit.h>

/* *********************************************************** */
