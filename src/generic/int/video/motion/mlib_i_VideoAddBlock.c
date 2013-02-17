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

#pragma ident	"@(#)mlib_i_VideoAddBlock.c	9.2	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_VideoAddBlock_U8_S16 - Performs additions of prediction and
 *                                  coefficient data
 *
 * SYNOPSIS
 *    mlib_status mlib_VideoAddBlock_U8_S16(mlib_u8        *curr_block,
 *                                          const mlib_s16 *mc_block,
 *                                          mlib_s32       stride)
 *
 * ARGUMENT
 *      curr_block  Pointer to  current block, must be 8-byte aligned
 *      mc_block    Pointer to 8x8 motion-compensated block(prediction data),
 *                  must be 8-byte aligned
 *      stride      Stride in bytes between adjacent rows in current block,
 *                  must be a multiple of 8
 */

#include <mlib_video.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_VideoAddBlock_U8_S16 = __mlib_VideoAddBlock_U8_S16

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_VideoAddBlock_U8_S16) mlib_VideoAddBlock_U8_S16
	__attribute__((weak, alias("__mlib_VideoAddBlock_U8_S16")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	SATURATE(src, dst)                                        \
	dst = ((src) | ~(((src) - 255) >> 31)) & ~((src) >> 31)

/* *********************************************************** */

mlib_status
__mlib_VideoAddBlock_U8_S16(
	mlib_u8 *curr_block,
	const mlib_s16 *mc_block,
	mlib_s32 stride)
{
	mlib_s32 x, y;
	mlib_u8 *dl, *dp;
	mlib_u8 a0, a1;
	const mlib_s16 *sp;

	dl = dp = curr_block;
	sp = mc_block;

	for (y = 0; y < 8; y++) {
		for (x = 0; x < 4; x++) {

			SATURATE((*dp) + (*sp), a0);
			sp++;
			dp++;
			SATURATE((*dp) + (*sp), a1);
			sp++;
			dp++;

#ifdef _LITTLE_ENDIAN
			((mlib_u16 *)dp)[-1] = (((mlib_u16)a1) << 8)
				| (((mlib_u16)a0));
#else
			((mlib_u16 *)dp)[-1] = (((mlib_u16)a0) << 8)
				| (((mlib_u16)a1));
#endif
		}

		dl = dp = dl + stride;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
