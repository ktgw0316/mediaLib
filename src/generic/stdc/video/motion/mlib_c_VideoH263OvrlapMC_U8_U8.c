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

#pragma ident	"@(#)mlib_c_VideoH263OvrlapMC_U8_U8.c	9.3	07/10/09 SMI"

/*
 * FUNCTION
 *   mlib_VideoH263OverlappedMC_U8_U8 - Generates the 8x8 luminance prediction
 *                                      block (motion-compensated reference
 *                                      block) in the Advanced Prediction Mode
 *                                      for a H.263 decoder
 *
 * SYNOPSIS
 *   mlib_status mlib_VideoH263OverlappedMC_U8_U8(mlib_u8       *curr_block,
 *                                                const mlib_u8 *ref_block,
 *                                                mlib_s32      mch,
 *                                                mlib_s32      mcv,
 *                                                mlib_s32      mah,
 *                                                mlib_s32      mav,
 *                                                mlib_s32      mbh,
 *                                                mlib_s32      mbv,
 *                                                mlib_s32      mlh,
 *                                                mlib_s32      mlv,
 *                                                mlib_s32      mrh,
 *                                                mlib_s32      mrv,
 *                                                mlib_s32      curr_stride,
 *                                                mlib_s32      ref_stride)
 *
 * ARGUMENTS
 *      curr_block  Pointer to current block, must be 8-byte aligned
 *      ref_block   Pointer to reference block in the reference frame
 *                  at the same location as the current block
 *                  in the current frame
 *      mch         Horizontal coordinate of the motion vector
 *                  for the current block
 *      mcv         Vertical coordinate of the motion vector
 *                  for the current block
 *      mah         Horizontal coordinate of the motion vector
 *                  for the block above the current block
 *      mav         Vertical coordinate of the motion vector
 *                  for the block above the current block
 *      mbh         Horizontal coordinate of the motion vector
 *                  for the block below the current block
 *      mbv         Vertical coordinate of the motion vector
 *                  for the block below the current block
 *      mlh         Horizontal coordinate of the motion vector
 *                  for the block to the left of the current block
 *      mlv         Vertical coordinate of the motion vector
 *                  for the block to the left of the current block
 *      mrh         Horizontal coordinate of the motion vector
 *                  for the block to the right of the current block
 *      mrv         Vertical coordinate of the motion vector
 *                  for the block to the right of the current block
 *      curr_stride Stride in bytes between adjacent rows
 *                  in the current block, must be a multiply of 8
 *      ref_stride  Stride in bytes between adjacent rows
 *                  in the reference block, must be a multiply of 8
 */

#include <mlib_video.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_VideoH263OverlappedMC_U8_U8 = \
	__mlib_VideoH263OverlappedMC_U8_U8

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_VideoH263OverlappedMC_U8_U8) mlib_VideoH263OverlappedMC_U8_U8
	__attribute__((weak, alias("__mlib_VideoH263OverlappedMC_U8_U8")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	CALC(x1, x2, x3, c1, c2, c3)                            \
	((c1 * (x1) + c2 * (x2) + c3 * (x3) + 4) >> 3)

/* *********************************************************** */

#define	CALC611(x1, x2, x3)	((6*(x1) + x2 + x3 + 4) >> 3)

/* *********************************************************** */

mlib_status
__mlib_VideoH263OverlappedMC_U8_U8(
	mlib_u8 *curr_block,
	const mlib_u8 *ref_block,
	mlib_s32 mch,
	mlib_s32 mcv,
	mlib_s32 mah,
	mlib_s32 mav,
	mlib_s32 mbh,
	mlib_s32 mbv,
	mlib_s32 mlh,
	mlib_s32 mlv,
	mlib_s32 mrh,
	mlib_s32 mrv,
	mlib_s32 curr_stride,
	mlib_s32 ref_stride)
{
	const mlib_u8 *sp1, *sp2, *sp3, *sp4;
	mlib_s32 ref_stride2 = ref_stride << 1;

	sp1 = (ref_block + mch + mcv * ref_stride);
	sp2 = (ref_block + mah + mav * ref_stride);
	sp3 = (ref_block + mlh + mlv * ref_stride);
	sp4 = (ref_block + mrh + mrv * ref_stride);

	curr_block[0] = (2 * sp1[0] + sp2[0] + sp3[0] + 2) >> 2;
	curr_block[1] = CALC(sp1[2], sp2[2], sp3[2], 5, 2, 1);
	curr_block[2] = CALC(sp1[4], sp2[4], sp3[4], 5, 2, 1);
	curr_block[3] = CALC(sp1[6], sp2[6], sp3[6], 5, 2, 1);
	curr_block[4] = CALC(sp1[8], sp2[8], sp4[8], 5, 2, 1);
	curr_block[5] = CALC(sp1[10], sp2[10], sp4[10], 5, 2, 1);
	curr_block[6] = CALC(sp1[12], sp2[12], sp4[12], 5, 2, 1);
	curr_block[7] = (2 * sp1[14] + sp2[14] + sp4[14] + 2) >> 2;
	sp1 += ref_stride2;
	sp2 += ref_stride2;
	sp3 += ref_stride2;
	sp4 += ref_stride2;
	curr_block += curr_stride;

	curr_block[0] = CALC(sp1[0], sp2[0], sp3[0], 5, 1, 2);
	curr_block[1] = CALC(sp1[2], sp2[2], sp3[2], 5, 1, 2);
	curr_block[2] = CALC(sp1[4], sp2[4], sp3[4], 5, 2, 1);
	curr_block[3] = CALC(sp1[6], sp2[6], sp3[6], 5, 2, 1);
	curr_block[4] = CALC(sp1[8], sp2[8], sp4[8], 5, 2, 1);
	curr_block[5] = CALC(sp1[10], sp2[10], sp4[10], 5, 2, 1);
	curr_block[6] = CALC(sp1[12], sp2[12], sp4[12], 5, 1, 2);
	curr_block[7] = CALC(sp1[14], sp2[14], sp4[14], 5, 1, 2);
	sp1 += ref_stride2;
	sp2 += ref_stride2;
	sp3 += ref_stride2;
	sp4 += ref_stride2;
	curr_block += curr_stride;

	curr_block[0] = CALC(sp1[0], sp2[0], sp3[0], 5, 1, 2);
	curr_block[1] = CALC(sp1[2], sp2[2], sp3[2], 5, 1, 2);
	curr_block[2] = CALC611(sp1[4], sp2[4], sp3[4]);
	curr_block[3] = CALC611(sp1[6], sp2[6], sp3[6]);
	curr_block[4] = CALC611(sp1[8], sp2[8], sp4[8]);
	curr_block[5] = CALC611(sp1[10], sp2[10], sp4[10]);
	curr_block[6] = CALC(sp1[12], sp2[12], sp4[12], 5, 1, 2);
	curr_block[7] = CALC(sp1[14], sp2[14], sp4[14], 5, 1, 2);
	sp1 += ref_stride2;
	sp2 += ref_stride2;
	sp3 += ref_stride2;
	sp4 += ref_stride2;
	curr_block += curr_stride;

	curr_block[0] = CALC(sp1[0], sp2[0], sp3[0], 5, 1, 2);
	curr_block[1] = CALC(sp1[2], sp2[2], sp3[2], 5, 1, 2);
	curr_block[2] = CALC611(sp1[4], sp2[4], sp3[4]);
	curr_block[3] = CALC611(sp1[6], sp2[6], sp3[6]);
	curr_block[4] = CALC611(sp1[8], sp2[8], sp4[8]);
	curr_block[5] = CALC611(sp1[10], sp2[10], sp4[10]);
	curr_block[6] = CALC(sp1[12], sp2[12], sp4[12], 5, 1, 2);
	curr_block[7] = CALC(sp1[14], sp2[14], sp4[14], 5, 1, 2);
	sp1 += ref_stride2;
	sp2 = (ref_block + mbh + (mbv + 8) * ref_stride);
	sp3 += ref_stride2;
	sp4 += ref_stride2;
	curr_block += curr_stride;

	curr_block[0] = CALC(sp1[0], sp2[0], sp3[0], 5, 1, 2);
	curr_block[1] = CALC(sp1[2], sp2[2], sp3[2], 5, 1, 2);
	curr_block[2] = CALC611(sp1[4], sp2[4], sp3[4]);
	curr_block[3] = CALC611(sp1[6], sp2[6], sp3[6]);
	curr_block[4] = CALC611(sp1[8], sp2[8], sp4[8]);
	curr_block[5] = CALC611(sp1[10], sp2[10], sp4[10]);
	curr_block[6] = CALC(sp1[12], sp2[12], sp4[12], 5, 1, 2);
	curr_block[7] = CALC(sp1[14], sp2[14], sp4[14], 5, 1, 2);
	sp1 += ref_stride2;
	sp2 += ref_stride2;
	sp3 += ref_stride2;
	sp4 += ref_stride2;
	curr_block += curr_stride;

	curr_block[0] = CALC(sp1[0], sp2[0], sp3[0], 5, 1, 2);
	curr_block[1] = CALC(sp1[2], sp2[2], sp3[2], 5, 1, 2);
	curr_block[2] = CALC611(sp1[4], sp2[4], sp3[4]);
	curr_block[3] = CALC611(sp1[6], sp2[6], sp3[6]);
	curr_block[4] = CALC611(sp1[8], sp2[8], sp4[8]);
	curr_block[5] = CALC611(sp1[10], sp2[10], sp4[10]);
	curr_block[6] = CALC(sp1[12], sp2[12], sp4[12], 5, 1, 2);
	curr_block[7] = CALC(sp1[14], sp2[14], sp4[14], 5, 1, 2);
	sp1 += ref_stride2;
	sp2 += ref_stride2;
	sp3 += ref_stride2;
	sp4 += ref_stride2;
	curr_block += curr_stride;

	curr_block[0] = CALC(sp1[0], sp2[0], sp3[0], 5, 1, 2);
	curr_block[1] = CALC(sp1[2], sp2[2], sp3[2], 5, 1, 2);
	curr_block[2] = CALC(sp1[4], sp2[4], sp3[4], 5, 2, 1);
	curr_block[3] = CALC(sp1[6], sp2[6], sp3[6], 5, 2, 1);
	curr_block[4] = CALC(sp1[8], sp2[8], sp4[8], 5, 2, 1);
	curr_block[5] = CALC(sp1[10], sp2[10], sp4[10], 5, 2, 1);
	curr_block[6] = CALC(sp1[12], sp2[12], sp4[12], 5, 1, 2);
	curr_block[7] = CALC(sp1[14], sp2[14], sp4[14], 5, 1, 2);
	sp1 += ref_stride2;
	sp2 += ref_stride2;
	sp3 += ref_stride2;
	sp4 += ref_stride2;
	curr_block += curr_stride;

	curr_block[0] = (2 * sp1[0] + sp2[0] + sp3[0] + 2) >> 2;
	curr_block[1] = CALC(sp1[2], sp2[2], sp3[2], 5, 2, 1);
	curr_block[2] = CALC(sp1[4], sp2[4], sp3[4], 5, 2, 1);
	curr_block[3] = CALC(sp1[6], sp2[6], sp3[6], 5, 2, 1);
	curr_block[4] = CALC(sp1[8], sp2[8], sp4[8], 5, 2, 1);
	curr_block[5] = CALC(sp1[10], sp2[10], sp4[10], 5, 2, 1);
	curr_block[6] = CALC(sp1[12], sp2[12], sp4[12], 5, 2, 1);
	curr_block[7] = (2 * sp1[14] + sp2[14] + sp4[14] + 2) >> 2;

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
