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

#pragma ident	"@(#)mlib_s_VideoInterpY_S16_U8.c	9.2	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_VideoInterpY_S16_U8_wxh,
 *      mlib_VideoInterpY_S16_U8     - Performs half-pixel interpolation in
 *                                     the Y-direction for non-replenishment
 *                                     mode
 *
 * SYNOPSIS
 *    mlib_status mlib_VideoInterpY_S16_U8_wxh(mlib_s16      *mc_block,
 *                                             const mlib_u8 *ref_block,
 *                                             mlib_s32      frame_stride,
 *                                             mlib_s32      field_stride)
 *    mlib_status mlib_VideoInterpY_S16_U8(mlib_s16      *mc_block,
 *                                         const mlib_u8 *ref_block,
 *                                         mlib_s32      width,
 *                                         mlib_s32      height,
 *                                         mlib_s32      frame_stride,
 *                                         mlib_s32      field_stride)
 *
 * ARGUMENT
 *      mc_block        Pointer to a wxh motion-compensated reference block,
 *                      must be 8-byte aligned
 *      ref_block       Pointer to reference block
 *      width           Width of the blocks, must be a multiply of 8
 *      height          Height of the blocks, must be a multiply of 4
 *      frame_stride    Stride in bytes between adjacent rows in a frame in
 *                      reference block, must be a multiply of 8
 *      field_stride    Stride in bytes between adjacent rows in a field in
 *                      reference block, must be a multiply of 8
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

#pragma weak mlib_VideoInterpY_S16_U8 = __mlib_VideoInterpY_S16_U8
#pragma weak mlib_VideoInterpY_S16_U8_16x16 = \
	__mlib_VideoInterpY_S16_U8_16x16
#pragma weak mlib_VideoInterpY_S16_U8_16x8 = __mlib_VideoInterpY_S16_U8_16x8
#pragma weak mlib_VideoInterpY_S16_U8_8x16 = __mlib_VideoInterpY_S16_U8_8x16
#pragma weak mlib_VideoInterpY_S16_U8_8x4 = __mlib_VideoInterpY_S16_U8_8x4
#pragma weak mlib_VideoInterpY_S16_U8_8x8 = __mlib_VideoInterpY_S16_U8_8x8

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_VideoInterpY_S16_U8) mlib_VideoInterpY_S16_U8
	__attribute__((weak, alias("__mlib_VideoInterpY_S16_U8")));
__typeof__(__mlib_VideoInterpY_S16_U8_16x16) mlib_VideoInterpY_S16_U8_16x16
	__attribute__((weak, alias("__mlib_VideoInterpY_S16_U8_16x16")));
__typeof__(__mlib_VideoInterpY_S16_U8_16x8) mlib_VideoInterpY_S16_U8_16x8
	__attribute__((weak, alias("__mlib_VideoInterpY_S16_U8_16x8")));
__typeof__(__mlib_VideoInterpY_S16_U8_8x16) mlib_VideoInterpY_S16_U8_8x16
	__attribute__((weak, alias("__mlib_VideoInterpY_S16_U8_8x16")));
__typeof__(__mlib_VideoInterpY_S16_U8_8x4) mlib_VideoInterpY_S16_U8_8x4
	__attribute__((weak, alias("__mlib_VideoInterpY_S16_U8_8x4")));
__typeof__(__mlib_VideoInterpY_S16_U8_8x8) mlib_VideoInterpY_S16_U8_8x8
	__attribute__((weak, alias("__mlib_VideoInterpY_S16_U8_8x8")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	MLIB_S_VIDEOINTERP8(height)   			          \
	for (y = 0; y < height; y++) {                            \
		mlib_s16 *dp = mc_block + y * 8;		  \
		txmm0 = _mm_loadu_si128((__m128i *)sp);		  \
		txmm1 = _mm_loadu_si128((__m128i *)(sp + frame_stride));\
		txmm2 = _mm_avg_epu8(txmm0, txmm1);		  \
		txmm3 = _mm_unpacklo_epi8(txmm2, txmm7);	  \
		_mm_storeu_si128((__m128i *)dp, txmm3);		  \
		sl = sp = sl + field_stride;                      \
	}

#define	MLIB_S_VIDEOINTERP16(height)                              \
	for (y = 0; y < height; y++) {                            \
		mlib_s16 *dp = mc_block + y * 16;		  \
		txmm0 = _mm_loadu_si128((__m128i *)sp);		  \
		txmm1 = _mm_loadu_si128((__m128i *)(sp + frame_stride));\
		txmm2 = _mm_avg_epu8(txmm0, txmm1);		  \
		txmm3 = _mm_unpacklo_epi8(txmm2, txmm7);	  \
		_mm_storeu_si128((__m128i *)dp, txmm3);	  	  \
		txmm4 = _mm_unpackhi_epi8(txmm2, txmm7);	  \
		_mm_storeu_si128((__m128i *)(dp + 8), txmm4);	  \
		sl = sp = sl + field_stride;                      \
	}


/* *********************************************************** */

mlib_status
__mlib_VideoInterpY_S16_U8_8x8(
	mlib_s16 *mc_block,
	const mlib_u8 *ref_block,
	mlib_s32 frame_stride,
	mlib_s32 field_stride)
{
	__m128i txmm0, txmm1, txmm2, txmm3, txmm4, txmm5, txmm7;
	txmm7 = _mm_setzero_si128();
	mlib_s32 x, y;
	const mlib_u8 *sl, *sp;
	sl = sp = ref_block;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif
	MLIB_S_VIDEOINTERP8(8);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VideoInterpY_S16_U8_16x8(
	mlib_s16 *mc_block,
	const mlib_u8 *ref_block,
	mlib_s32 frame_stride,
	mlib_s32 field_stride)
{
	__m128i txmm0, txmm1, txmm2, txmm3, txmm4, txmm5, txmm7;
	txmm7 = _mm_setzero_si128();
	mlib_s32 x, y;
	const mlib_u8 *sl, *sp;
	sl = sp = ref_block;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif
	MLIB_S_VIDEOINTERP16(8);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VideoInterpY_S16_U8_16x16(
	mlib_s16 *mc_block,
	const mlib_u8 *ref_block,
	mlib_s32 frame_stride,
	mlib_s32 field_stride)
{
	__m128i txmm0, txmm1, txmm2, txmm3, txmm4, txmm5, txmm7;
	txmm7 = _mm_setzero_si128();
	mlib_s32 x, y;
	const mlib_u8 *sl, *sp;
	sl = sp = ref_block;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif
	MLIB_S_VIDEOINTERP16(16);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VideoInterpY_S16_U8_8x16(
	mlib_s16 *mc_block,
	const mlib_u8 *ref_block,
	mlib_s32 frame_stride,
	mlib_s32 field_stride)
{
	__m128i txmm0, txmm1, txmm2, txmm3, txmm4, txmm5, txmm7;
	txmm7 = _mm_setzero_si128();
	mlib_s32 x, y;
	const mlib_u8 *sl, *sp;
	sl = sp = ref_block;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif
	MLIB_S_VIDEOINTERP8(16);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VideoInterpY_S16_U8_8x4(
	mlib_s16 *mc_block,
	const mlib_u8 *ref_block,
	mlib_s32 frame_stride,
	mlib_s32 field_stride)
{
	__m128i txmm0, txmm1, txmm2, txmm3, txmm4, txmm5, txmm7;
	txmm7 = _mm_setzero_si128();
	mlib_s32 x, y;
	const mlib_u8 *sl, *sp;
	sl = sp = ref_block;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif
	MLIB_S_VIDEOINTERP8(4);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VideoInterpY_S16_U8(
	mlib_s16 *mc_block,
	const mlib_u8 *ref_block,
	mlib_s32 width,
	mlib_s32 height,
	mlib_s32 frame_stride,
	mlib_s32 field_stride)
{
	__m128i txmm0, txmm1, txmm2, txmm3, txmm4, txmm5, txmm7;
	txmm7 = _mm_setzero_si128();
	mlib_s32 x, y;
	const mlib_u8 *sl, *sp;
	sl = sp = ref_block;

	if (width & 0xF) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif
		MLIB_S_VIDEOINTERP8(height);
	} else {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif
		MLIB_S_VIDEOINTERP16(height);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
