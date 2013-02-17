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

#pragma ident	"@(#)mlib_VideoIDCT8x8_DC.c	9.2	07/10/09 SMI"

/*
 * FUNCTION
 *
 *  mlib_VideoIDCT8x8_U8_S16_DC  - Computes the inverse IDCT in the intra mode
 *  mlib_VideoIDCT8x8_S16_S16_DC - Computes the inverse IDCT in the inter mode
 *
 * SYNOPSIS
 *
 *  mlib_status mlib_VideoIDCT8x8_U8_S16_DC(mlib_u8        *block,
 *                                          const mlib_s16 *coeffs,
 *                                          mlib_s32       stride)
 *
 *  mlib_status mlib_VideoIDCT8x8_S16_S16_DC(mlib_s16       block[64],
 *                                           const mlib_s16 *coeffs)
 *
 * DESCRIPTION
 *  function should be used when coeffs[u][v] (u, v = 0...7) are non zero
 *  only for u = 0 and v = 0, the other word when only DC coeff is present,
 *  so block[x][y] = coeffs[0][0] / 8;
 *
 * ARGUMENT
 *  block       Pointer to an 8x8 motion-compensated block which is the
 *              difference between the reference block and current block
 *              or a block in the current frame
 *  coeffs      Pointer to the input DCT coefficients
 *  stride      Stride in bytes between adjacent rows in a block
 *
 *  It is assumed that block and coeffs are 8-byte aligned and stride is
 *  a multiple of 8.
 */

#include <mlib_video.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_VideoIDCT8x8_S16_S16_DC = __mlib_VideoIDCT8x8_S16_S16_DC
#pragma weak mlib_VideoIDCT8x8_U8_S16_DC = __mlib_VideoIDCT8x8_U8_S16_DC

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_VideoIDCT8x8_S16_S16_DC) mlib_VideoIDCT8x8_S16_S16_DC
	__attribute__((weak, alias("__mlib_VideoIDCT8x8_S16_S16_DC")));
__typeof__(__mlib_VideoIDCT8x8_U8_S16_DC) mlib_VideoIDCT8x8_U8_S16_DC
	__attribute__((weak, alias("__mlib_VideoIDCT8x8_U8_S16_DC")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

/* do not perform the copying by mlib_d64 data type for x86 */
#ifdef i386

typedef struct
{
	mlib_s32 int0, int1;
} two_int;

#define	TYPE_64BIT	two_int

#else /* i386 */

#define	TYPE_64BIT	mlib_d64

#endif /* i386 */

/* *********************************************************** */

mlib_status
__mlib_VideoIDCT8x8_U8_S16_DC(
	mlib_u8 *block,
	const mlib_s16 *coeffs,
	mlib_s32 stride)
{
	mlib_s32 val, res;
	TYPE_64BIT dres;

	val = coeffs[0];
	res = (val + 4) >> 3;

	res = (res | ~((val - 2044) >> 31)) & ~(val >> 31);

	res |= res << 8;
	res |= res << 16;

	((mlib_s32 *)block)[0] = res;
	((mlib_s32 *)block)[1] = res;
	dres = ((TYPE_64BIT *) (block))[0];

	*((TYPE_64BIT *) (block + stride)) = dres;
	*((TYPE_64BIT *) (block + 2 * stride)) = dres;
	*((TYPE_64BIT *) (block + 3 * stride)) = dres;
	*((TYPE_64BIT *) (block + 4 * stride)) = dres;
	*((TYPE_64BIT *) (block + 5 * stride)) = dres;
	*((TYPE_64BIT *) (block + 6 * stride)) = dres;
	*((TYPE_64BIT *) (block + 7 * stride)) = dres;

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VideoIDCT8x8_S16_S16_DC(
	mlib_s16 *block,
	const mlib_s16 *coeffs)
{
	mlib_s32 val, res;
	TYPE_64BIT dres;

	val = coeffs[0];
	res = (val + 4) >> 3;

	res = (res & 0xffff) | (res << 16);

	((mlib_s32 *)block)[0] = res;
	((mlib_s32 *)block)[1] = res;
	dres = ((TYPE_64BIT *) (block))[0];

	((TYPE_64BIT *) (block))[1] = dres;
	((TYPE_64BIT *) (block))[2] = dres;
	((TYPE_64BIT *) (block))[3] = dres;
	((TYPE_64BIT *) (block))[4] = dres;
	((TYPE_64BIT *) (block))[5] = dres;
	((TYPE_64BIT *) (block))[6] = dres;
	((TYPE_64BIT *) (block))[7] = dres;
	((TYPE_64BIT *) (block))[8] = dres;
	((TYPE_64BIT *) (block))[9] = dres;
	((TYPE_64BIT *) (block))[10] = dres;
	((TYPE_64BIT *) (block))[11] = dres;
	((TYPE_64BIT *) (block))[12] = dres;
	((TYPE_64BIT *) (block))[13] = dres;
	((TYPE_64BIT *) (block))[14] = dres;
	((TYPE_64BIT *) (block))[15] = dres;

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
