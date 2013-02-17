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

#pragma ident	"@(#)mlib_v_VideoIDCT8x8_DC.c	9.3	07/11/05 SMI"

/*
 * FUNCTION
 *
 *      mlib_VideoIDCT8x8_U8_S16_DC  - Computes the inverse IDCT
 *                                     in the intra mode
 *      mlib_VideoIDCT8x8_S16_S16_DC - Computes the inverse IDCT
 *                                     in the inter mode
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_VideoIDCT8x8_S16_S16_DC(mlib_s16       *block,
 *                                               const mlib_s16 *coeffs);
 *      mlib_status mlib_VideoIDCT8x8_U8_S16_DC(mlib_u8        *block,
 *                                              const mlib_s16 *coeffs,
 *                                              mlib_s32       stride);
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
#include <vis_proto.h>

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

mlib_status
__mlib_VideoIDCT8x8_U8_S16_DC(
	mlib_u8 *block,
	const mlib_s16 *coeffs,
	mlib_s32 stride)
{
	mlib_f32 p00, mul;
	mlib_d64 res;

	vis_write_gsr(7 << 3);
	mul = vis_to_float(0x20202020);
	p00 = *(mlib_f32 *)coeffs;
	res = vis_fmul8x16au(mul, p00);
	p00 = vis_fpack16(res);
	res = vis_fpmerge(p00, p00);

	*((mlib_d64 *)(block)) = res;
	*((mlib_d64 *)(block + stride)) = res;
	*((mlib_d64 *)(block + 2 * stride)) = res;
	*((mlib_d64 *)(block + 3 * stride)) = res;
	*((mlib_d64 *)(block + 4 * stride)) = res;
	*((mlib_d64 *)(block + 5 * stride)) = res;
	*((mlib_d64 *)(block + 6 * stride)) = res;
	*((mlib_d64 *)(block + 7 * stride)) = res;

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VideoIDCT8x8_S16_S16_DC(
	mlib_s16 *block,
	const mlib_s16 *coeffs)
{
	mlib_f32 p00, mul;
	mlib_d64 res;

	mul = vis_to_float(0x20202020);
	p00 = *(mlib_f32 *)coeffs;
	res = vis_fmul8x16au(mul, p00);

	((mlib_d64 *)(block))[0] = res;
	((mlib_d64 *)(block))[1] = res;
	((mlib_d64 *)(block))[2] = res;
	((mlib_d64 *)(block))[3] = res;
	((mlib_d64 *)(block))[4] = res;
	((mlib_d64 *)(block))[5] = res;
	((mlib_d64 *)(block))[6] = res;
	((mlib_d64 *)(block))[7] = res;
	((mlib_d64 *)(block))[8] = res;
	((mlib_d64 *)(block))[9] = res;
	((mlib_d64 *)(block))[10] = res;
	((mlib_d64 *)(block))[11] = res;
	((mlib_d64 *)(block))[12] = res;
	((mlib_d64 *)(block))[13] = res;
	((mlib_d64 *)(block))[14] = res;
	((mlib_d64 *)(block))[15] = res;

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
