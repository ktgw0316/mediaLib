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

#pragma ident	"@(#)mlib_c_VideoDeQuantizeIDCT.c	9.4	07/10/30 SMI"

/*
 * FUNCTIONS
 *      mlib_VideoDeQuantizeIDCT8x8_U8_S16[_NA] - performs Dequantization and
 *				computes the inverse DCT in the intra mode.
 *      mlib_VideoDeQuantizeIDCT8x8_S16_S16_B12[_NA]- performs Dequantization
 *				and computes the inverse DCT in the
 *				inter mode.
 *
 * SYNOPSIS
 *      mlib_status  mlib_VideoDeQuantizeIDCT8x8_U8_S16[_NA](
 *      			mlib_u8	*block,
 *      			const mlib_s16 coeffs[64],
 *      			const mlib_d64 dqtable[64],
 *      			mlib_s32       stride)
 *      mlib_status  mlib_VideoDeQuantizeIDCT8x8_S16_S16_B12[_NA](
 *      			mlib_s16       *block,
 *      			const mlib_s16 coeffs[64],
 *      			const mlib_d64 dqtable[64])
 *
 * ARGUMENTS
 *
 *      block        Pointer to 8x8 block in the current frame or
 *                   motion-compensated reference block.
 *      coeffs       Pointer to input quantized DCT coefficients.
 *      dqtable	     Pointer to Dequantization table.
 *      stride       Stride in bytes between adjacent rows in a block,
 *			and the stride applies to the block which is the
 *			part of the current frame being reconstructed, in
 *			case of an intra mode.
 *
 * RESTRICTIONS
 *	The block and the coeffs and the dqtable must be 8-byte aligned
 *	mlib_VideoDeQuantizeIDCT8x8_U8_S16
 *	and mlib_VideoDeQuantizeIDCT8x8_S16_S16_B12.
 *	the stride must be a multiple of 8 for
 *	mlib_VideoDeQuantizeIDCT8x8_U8_S16.
 *
 *	The output of the mlib_VideoDeQuantizeIDCT8x8_S16_S16_B12[_NA]
 *	should be within range [-2048, 2047] if coeffs is obtained from
 *	correspondent direct DCT8x8Quantize. This function can be used
 *	in JPEG with the 12 bits sample precision. For MPEG, the output
 *	which is really the difference between the current block and
 *	the reference block can occupy 9 bits, and is also represented
 *	in 16-bit data.
 *	The output needs to be added to the motion-compensated
 *	reference block to reconstruct the current block.
 *
 *	dqtable should be the output of mlib_VideDeQuantizeInit_S16.
 *
 * DESCRIPTION
 *
 *     F = F(u, v)  u = 0, 1, ... ,7; v = 0, 1, ... ,7
 *     f = f(x, y)  x = 0, 1, ... ,7; y = 0, 1, ... ,7
 *
 *   Dequantization algorithm:
 *     F(u,v) = F(u,v) * dqtable(u,v);
 *
 *   IDCT algorithm:
 *     f(x, y) =
 *      1   7   7                       pi*(2*x+1)*u      pi*(2*y+1)*v
 *     ---*SUM SUM C(u)*C(v)*F(u,v)*cos(------------)*cos(------------)
 *      4  u=0 v=0                           16                16
 *
 *     C(u) = 1/sqrt(2) if u == 0, otherwise C(u) = 1
 */

#include <mlib_video.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_VideoDeQuantizeIDCT8x8_S16_S16_B12 =\
			__mlib_VideoDeQuantizeIDCT8x8_S16_S16_B12
#pragma weak mlib_VideoDeQuantizeIDCT8x8_S16_S16_B12_NA =\
			__mlib_VideoDeQuantizeIDCT8x8_S16_S16_B12_NA
#pragma weak mlib_VideoDeQuantizeIDCT8x8_U8_S16 =\
			__mlib_VideoDeQuantizeIDCT8x8_U8_S16
#pragma weak mlib_VideoDeQuantizeIDCT8x8_U8_S16_NA =\
			__mlib_VideoDeQuantizeIDCT8x8_U8_S16_NA

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_VideoDeQuantizeIDCT8x8_S16_S16_B12)
	mlib_VideoDeQuantizeIDCT8x8_S16_S16_B12
__attribute__((weak, alias("__mlib_VideoDeQuantizeIDCT8x8_S16_S16_B12")));
__typeof__(__mlib_VideoDeQuantizeIDCT8x8_S16_S16_B12_NA)
	mlib_VideoDeQuantizeIDCT8x8_S16_S16_B12_NA
__attribute__((weak, alias("__mlib_VideoDeQuantizeIDCT8x8_S16_S16_B12_NA")));
__typeof__(__mlib_VideoDeQuantizeIDCT8x8_U8_S16)
	mlib_VideoDeQuantizeIDCT8x8_U8_S16
__attribute__((weak, alias("__mlib_VideoDeQuantizeIDCT8x8_U8_S16")));
__typeof__(__mlib_VideoDeQuantizeIDCT8x8_U8_S16_NA)
	mlib_VideoDeQuantizeIDCT8x8_U8_S16_NA
__attribute__((weak, alias("__mlib_VideoDeQuantizeIDCT8x8_U8_S16_NA")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */


/* *********************************************************** */

mlib_status
__mlib_VideoDeQuantizeIDCT8x8_U8_S16(
	mlib_u8 *block,
	const mlib_s16 coeffs[64],
	const mlib_d64 dqtable[64],
	mlib_s32 stride)
{

	if (MLIB_FAILURE == __mlib_VideoDeQuantize_S16(
				(mlib_s16 *)coeffs, dqtable))
			return (MLIB_FAILURE);

	if (MLIB_FAILURE == __mlib_VideoIDCT8x8_U8_S16(block,
				coeffs, stride))
		return (MLIB_FAILURE);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VideoDeQuantizeIDCT8x8_U8_S16_NA(
	mlib_u8 *block,
	const mlib_s16 coeffs[64],
	const mlib_d64 dqtable[64],
	mlib_s32 stride)
{
	if (coeffs == NULL || block == NULL)
		return (MLIB_NULLPOINTER);

	if (stride <= 0)
		return (MLIB_OUTOFRANGE);

	return (__mlib_VideoDeQuantizeIDCT8x8_U8_S16(block,
				coeffs, dqtable, stride));
}

/* *********************************************************** */

mlib_status
__mlib_VideoDeQuantizeIDCT8x8_S16_S16_B12(
	mlib_s16 *block,
	const mlib_s16 coeffs[64],
	const mlib_d64 dqtable[64])
{
	if (MLIB_FAILURE == __mlib_VideoDeQuantize_S16(
				(mlib_s16 *)coeffs, dqtable))
		return (MLIB_FAILURE);

	if (MLIB_FAILURE == __mlib_VideoIDCT8x8_S16_S16_B12(block,
				coeffs))
		return (MLIB_FAILURE);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VideoDeQuantizeIDCT8x8_S16_S16_B12_NA(
	mlib_s16 *block,
	const mlib_s16 coeffs[64],
	const mlib_d64 dqtable[64])
{
	if (coeffs == NULL || block == NULL)
		return (MLIB_NULLPOINTER);

	return (__mlib_VideoDeQuantizeIDCT8x8_S16_S16_B12(block,
				coeffs, dqtable));
}

/* *********************************************************** */
