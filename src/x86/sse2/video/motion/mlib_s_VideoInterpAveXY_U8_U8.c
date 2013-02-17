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
 * Copyright 2006 Sun Microsystems, Inc.  All rights reserved.
 * Use is subject to license terms.
 */

#pragma ident	"@(#)mlib_s_VideoInterpAveXY_U8_U8.c	9.2	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_VideoInterpAveXY_U8_U8_wxh,
 *      mlib_VideoInterpAveXY_U8_U8     - Performs half-pixel interpolation
 *                                        and averaging in both X and Y
 *                                        directions for replenishment mode.
 *
 * SYNOPSIS
 *    mlib_status mlib_VideoInterpAveXY_U8_U8_wxh(mlib_u8       *curr_block,
 *                                                const mlib_u8 *ref_block,
 *                                                mlib_s32      frame_stride,
 *                                                mlib_s32      field_stride)
 *    mlib_status mlib_VideoInterpAveXY_U8_U8(mlib_u8       *curr_block,
 *                                            const mlib_u8 *ref_block,
 *                                            mlib_s32      width,
 *                                            mlib_s32      height,
 *                                            mlib_s32      frame_stride,
 *                                            mlib_s32      field_stride)
 *
 * ARGUMENT
 *      curr_block      Pointer to current block, must be 8-byte aligned
 *      ref_block       Pointer to reference block
 *      width           Width of the blocks, must be a multiply of 8
 *      height          Height of the blocks, must be a multiply of 4
 *      frame_stride    Stride in bytes between adjacent rows in a frame in
 *                      both current and reference block,
 *                      must be a multiply of 8
 *      field_stride    Stride in bytes between adjacent rows in a field in
 *                      both current and reference block,
 *                      must be a multiply of 8
 */

#include <mlib_video.h>

#ifdef __SUNPRO_C
#include <sunmedia_intrin.h>
#else
#include <emmintrin.h>
#endif

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_VideoInterpAveXY_U8_U8 = __mlib_VideoInterpAveXY_U8_U8
#pragma weak mlib_VideoInterpAveXY_U8_U8_16x16 = \
	__mlib_VideoInterpAveXY_U8_U8_16x16
#pragma weak mlib_VideoInterpAveXY_U8_U8_16x8 = \
	__mlib_VideoInterpAveXY_U8_U8_16x8
#pragma weak mlib_VideoInterpAveXY_U8_U8_8x16 = \
	__mlib_VideoInterpAveXY_U8_U8_8x16
#pragma weak mlib_VideoInterpAveXY_U8_U8_8x4 = \
	__mlib_VideoInterpAveXY_U8_U8_8x4
#pragma weak mlib_VideoInterpAveXY_U8_U8_8x8 = \
	__mlib_VideoInterpAveXY_U8_U8_8x8

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_VideoInterpAveXY_U8_U8) mlib_VideoInterpAveXY_U8_U8
	__attribute__((weak, alias("__mlib_VideoInterpAveXY_U8_U8")));
__typeof__(__mlib_VideoInterpAveXY_U8_U8_16x16)
	mlib_VideoInterpAveXY_U8_U8_16x16
	__attribute__((weak, alias("__mlib_VideoInterpAveXY_U8_U8_16x16")));
__typeof__(__mlib_VideoInterpAveXY_U8_U8_16x8) mlib_VideoInterpAveXY_U8_U8_16x8
	__attribute__((weak, alias("__mlib_VideoInterpAveXY_U8_U8_16x8")));
__typeof__(__mlib_VideoInterpAveXY_U8_U8_8x16) mlib_VideoInterpAveXY_U8_U8_8x16
	__attribute__((weak, alias("__mlib_VideoInterpAveXY_U8_U8_8x16")));
__typeof__(__mlib_VideoInterpAveXY_U8_U8_8x4) mlib_VideoInterpAveXY_U8_U8_8x4
	__attribute__((weak, alias("__mlib_VideoInterpAveXY_U8_U8_8x4")));
__typeof__(__mlib_VideoInterpAveXY_U8_U8_8x8) mlib_VideoInterpAveXY_U8_U8_8x8
	__attribute__((weak, alias("__mlib_VideoInterpAveXY_U8_U8_8x8")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	MLIB_S_VIDEOINTERPAVG8(height)                         \
	for (y = 0; y < height; y++) {                               \
	    const mlib_u8 *sd2 = sd + frame_stride;                  \
		txmm0 = _mm_loadu_si128((__m128i *)dd);		     \
		txmm1 = _mm_loadu_si128((__m128i *)sd);		     \
		txmm2 = _mm_loadu_si128((__m128i *)(sd + 1));	     \
		txmm3 = _mm_loadu_si128((__m128i *)sd2);	     \
		txmm4 = _mm_loadu_si128((__m128i *)(sd2 + 1));	     \
		txmm1 = _mm_avg_epu8(txmm1, txmm3);		     \
		txmm2 = _mm_avg_epu8(txmm2, txmm4);		     \
		txmm3 = _mm_avg_epu8(txmm1, txmm2);		     \
		txmm6 = _mm_avg_epu8(txmm0, txmm3);		     \
		_mm_storel_epi64((__m128i *)dd, txmm6);		     \
	                                                             \
	    sl = sd = sl + field_stride;                             \
	    dl = dd = dl + field_stride;                             \
	}

#define	MLIB_S_VIDEOINTERPAVG16(height)                         \
	sl = sd = ref_block;                                         \
	dl = dd = curr_block;                                        \
	for (y = 0; y < height; y++) {                               \
	    const mlib_u8 *sd2 = sd + frame_stride;                  \
		txmm0 = _mm_loadu_si128((__m128i *)dd);		     \
		txmm1 = _mm_loadu_si128((__m128i *)sd);		     \
		txmm2 = _mm_loadu_si128((__m128i *)(sd + 1));	     \
		txmm3 = _mm_loadu_si128((__m128i *)sd2);	     \
		txmm4 = _mm_loadu_si128((__m128i *)(sd2 + 1));	     \
		txmm1 = _mm_avg_epu8(txmm1, txmm3);		     \
		txmm2 = _mm_avg_epu8(txmm2, txmm4);		     \
		txmm3 = _mm_avg_epu8(txmm1, txmm2);		     \
		txmm6 = _mm_avg_epu8(txmm0, txmm3);		     \
		_mm_storeu_si128((__m128i *)dd, txmm6);		     \
	                                                             \
	    sl = sd = sl + field_stride;                             \
	    dl = dd = dl + field_stride;                             \
	}


/* *********************************************************** */

mlib_status
__mlib_VideoInterpAveXY_U8_U8_8x8(
	mlib_u8 *curr_block,
	const mlib_u8 *ref_block,
	mlib_s32 frame_stride,
	mlib_s32 field_stride)
{
	__m128i txmm0, txmm1, txmm2, txmm3, txmm4, txmm5, txmm6, txmm7;
	mlib_s32 x, y;
	const mlib_u8 *sl, *sd;
	mlib_u8 *dl, *dd;
	sl = sd = ref_block;
	dl = dd = curr_block;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif
	MLIB_S_VIDEOINTERPAVG8(8);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VideoInterpAveXY_U8_U8_16x8(
	mlib_u8 *curr_block,
	const mlib_u8 *ref_block,
	mlib_s32 frame_stride,
	mlib_s32 field_stride)
{
	__m128i txmm0, txmm1, txmm2, txmm3, txmm4, txmm5, txmm6, txmm7;
	mlib_s32 x, y;
	const mlib_u8 *sl, *sd;
	mlib_u8 *dl, *dd;
	sl = sd = ref_block;
	dl = dd = curr_block;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif
	MLIB_S_VIDEOINTERPAVG16(8);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VideoInterpAveXY_U8_U8_16x16(
	mlib_u8 *curr_block,
	const mlib_u8 *ref_block,
	mlib_s32 frame_stride,
	mlib_s32 field_stride)
{
	__m128i txmm0, txmm1, txmm2, txmm3, txmm4, txmm5, txmm6, txmm7;
	mlib_s32 x, y;
	const mlib_u8 *sl, *sd;
	mlib_u8 *dl, *dd;
	sl = sd = ref_block;
	dl = dd = curr_block;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif
	MLIB_S_VIDEOINTERPAVG16(16);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VideoInterpAveXY_U8_U8_8x16(
	mlib_u8 *curr_block,
	const mlib_u8 *ref_block,
	mlib_s32 frame_stride,
	mlib_s32 field_stride)
{
	__m128i txmm0, txmm1, txmm2, txmm3, txmm4, txmm5, txmm6, txmm7;
	mlib_s32 x, y;
	const mlib_u8 *sl, *sd;
	mlib_u8 *dl, *dd;
	sl = sd = ref_block;
	dl = dd = curr_block;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif
	MLIB_S_VIDEOINTERPAVG8(16);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VideoInterpAveXY_U8_U8_8x4(
	mlib_u8 *curr_block,
	const mlib_u8 *ref_block,
	mlib_s32 frame_stride,
	mlib_s32 field_stride)
{
	__m128i txmm0, txmm1, txmm2, txmm3, txmm4, txmm5, txmm6, txmm7;
	mlib_s32 x, y;
	const mlib_u8 *sl, *sd;
	mlib_u8 *dl, *dd;
	sl = sd = ref_block;
	dl = dd = curr_block;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif
	MLIB_S_VIDEOINTERPAVG8(4);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VideoInterpAveXY_U8_U8(
	mlib_u8 *curr_block,
	const mlib_u8 *ref_block,
	mlib_s32 width,
	mlib_s32 height,
	mlib_s32 frame_stride,
	mlib_s32 field_stride)
{
	__m128i txmm0, txmm1, txmm2, txmm3, txmm4, txmm5, txmm6, txmm7;
	mlib_s32 x, y;
	const mlib_u8 *sl, *sd;
	mlib_u8 *dl, *dd;
	sl = sd = ref_block;
	dl = dd = curr_block;
	if (width & 0xF) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif
		MLIB_S_VIDEOINTERPAVG8(height);
	} else {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif
		MLIB_S_VIDEOINTERPAVG16(height);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
