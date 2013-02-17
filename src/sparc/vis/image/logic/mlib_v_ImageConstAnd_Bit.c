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
 * Copyright 2005 Sun Microsystems, Inc.  All rights reserved.
 * Use is subject to license terms.
 */

#pragma ident	"@(#)mlib_v_ImageConstAnd_Bit.c	9.2	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_ImageConstAnd_Bit - image logical operation with constant
 *                               (type of image - BIT)
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageConstAnd_Bit(mlib_image *dst,
 *                                         const mlib_image *src,
 *                                         const mlib_s32   *c);
 *
 * ARGUMENT
 *      dst     pointer to output image
 *      src     pointer to input image
 *      c       array of constants for each channel
 *
 * DESCRIPTION
 *      VIS version file for one of the following operations:
 *
 *      And     dst(i, j) = c & src(i, j)
 *      Or      dst(i, j) = c | src(i, j)
 *      Xor     dst(i, j) = c ^ src(i, j)
 *      NotAnd  dst(i, j) = ~(c & src(i, j))
 *      NotOr   dst(i, j) = ~(c | src(i, j))
 *      NotXor  dst(i, j) = ~(c ^ src(i, j))
 *      AndNot  dst(i, j) = c & (~src(i, j))
 *      OrNot   dst(i, j) = c & (~src(i, j))
 *
 */

/* *********************************************************** */

#define	VIS_CONSTLOGIC(c, a)	vis_fand(a, c)
#define	C_CONSTLOGIC(c, a)	(c) & (a)

/* *********************************************************** */

#define	CONST_LOGIC_BITS                                                   \
	mlib_status                                                        \
	mlib_v_ImageConstAnd_Bit(mlib_image *dst, const mlib_image *src,   \
	    const mlib_s32 *c)

#include <mlib_v_ImageConstLogic_Bit.h>

/* *********************************************************** */
