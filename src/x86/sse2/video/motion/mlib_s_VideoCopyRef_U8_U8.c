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

#pragma ident	"@(#)mlib_s_VideoCopyRef_U8_U8.c	9.2	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_VideoCopyRef_U8_U8_wxh,
 *      mlib_VideoCopyRef_U8_U8      - Copies a wxh block from reference block
 *                                     to current block
 *
 * SYNOPSIS
 *    mlib_status mlib_VideoCopyRef_U8_U8_wxh(mlib_u8       *curr_block,
 *                                            const mlib_u8 *ref_block,
 *                                            mlib_s32      stride)
 *    mlib_status mlib_VideoCopyRef_U8_U8(mlib_u8       *curr_block,
 *                                        const mlib_u8 *ref_block,
 *                                        mlib_s32      width,
 *                                        mlib_s32      height,
 *                                        mlib_s32      stride)
 *
 * ARGUMENT
 *      curr_block      Pointer to current block, must be 8-byte aligned
 *      ref_block       Pointer to reference block
 *      width           Width of the blocks, must be a multiply of 8
 *      height          Height of the blocks, must be a multiply of 4
 *      stride          Stride in bytes between adjacent rows in both current
 *                      and reference block, must be a multiply of 8
 */

#include <mlib_video.h>
#include <mlib_Utils.h>

#ifdef __SUNPRO_C
#include <sunmedia_intrin.h>
#else
#include <emmintrin.h>
#endif

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_VideoCopyRef_U8_U8 = __mlib_VideoCopyRef_U8_U8
#pragma weak mlib_VideoCopyRef_U8_U8_16x16 = __mlib_VideoCopyRef_U8_U8_16x16
#pragma weak mlib_VideoCopyRef_U8_U8_16x8 = __mlib_VideoCopyRef_U8_U8_16x8
#pragma weak mlib_VideoCopyRef_U8_U8_8x16 = __mlib_VideoCopyRef_U8_U8_8x16
#pragma weak mlib_VideoCopyRef_U8_U8_8x4 = __mlib_VideoCopyRef_U8_U8_8x4
#pragma weak mlib_VideoCopyRef_U8_U8_8x8 = __mlib_VideoCopyRef_U8_U8_8x8

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_VideoCopyRef_U8_U8) mlib_VideoCopyRef_U8_U8
	__attribute__((weak, alias("__mlib_VideoCopyRef_U8_U8")));
__typeof__(__mlib_VideoCopyRef_U8_U8_16x16) mlib_VideoCopyRef_U8_U8_16x16
	__attribute__((weak, alias("__mlib_VideoCopyRef_U8_U8_16x16")));
__typeof__(__mlib_VideoCopyRef_U8_U8_16x8) mlib_VideoCopyRef_U8_U8_16x8
	__attribute__((weak, alias("__mlib_VideoCopyRef_U8_U8_16x8")));
__typeof__(__mlib_VideoCopyRef_U8_U8_8x16) mlib_VideoCopyRef_U8_U8_8x16
	__attribute__((weak, alias("__mlib_VideoCopyRef_U8_U8_8x16")));
__typeof__(__mlib_VideoCopyRef_U8_U8_8x4) mlib_VideoCopyRef_U8_U8_8x4
	__attribute__((weak, alias("__mlib_VideoCopyRef_U8_U8_8x4")));
__typeof__(__mlib_VideoCopyRef_U8_U8_8x8) mlib_VideoCopyRef_U8_U8_8x8
	__attribute__((weak, alias("__mlib_VideoCopyRef_U8_U8_8x8")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	MLIB_S_VIDEOCOPY8(height)	                        \
	mlib_u8 *sl1, *dd, *sl, *sd;				\
	sl = sd = (void *)ref_block;                            \
	sl1 = dd = curr_block;					\
	__m128i txmm0, txmm1, txmm2, txmm3;			\
	for (y = 0; y < height; y++) {                          \
		txmm0 = _mm_loadu_si128((__m128i *)sd);		\
		_mm_storel_epi64((__m128i *)dd, txmm0);		\
	    sl = sd = sl + stride;                              \
	    sl1 = dd = sl1 + stride;                            \
	}

#define	MLIB_S_VIDEOCOPY16(height)	                        \
	mlib_u8 *sl1, *dd, *sl, *sd;				\
	sl = sd = (void *)ref_block;                            \
	sl1 = dd = curr_block;					\
	__m128i txmm0, txmm1, txmm2, txmm3;			\
	for (y = 0; y < height; y++) {                          \
		txmm0 = _mm_loadu_si128((__m128i *)sd);		\
		_mm_storeu_si128((__m128i *)dd, txmm0);		\
	    sl = sd = sl + stride;                              \
	    sl1 = dd = sl1 + stride;                            \
	}

/* ************************************************************ */
mlib_status
__mlib_VideoCopyRef_U8_U8_16x16(
	mlib_u8 *curr_block,
	const mlib_u8 *ref_block,
	mlib_s32 stride)
{
	mlib_s32 y;

	MLIB_S_VIDEOCOPY16(16);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VideoCopyRef_U8_U8_8x16(
	mlib_u8 *curr_block,
	const mlib_u8 *ref_block,
	mlib_s32 stride)
{
	mlib_s32 y;

	MLIB_S_VIDEOCOPY8(16);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VideoCopyRef_U8_U8_16x8(
	mlib_u8 *curr_block,
	const mlib_u8 *ref_block,
	mlib_s32 stride)
{
	mlib_s32 y;

	MLIB_S_VIDEOCOPY16(8);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VideoCopyRef_U8_U8_8x8(
	mlib_u8 *curr_block,
	const mlib_u8 *ref_block,
	mlib_s32 stride)
{
	mlib_s32 y;

	MLIB_S_VIDEOCOPY8(8);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VideoCopyRef_U8_U8_8x4(
	mlib_u8 *curr_block,
	const mlib_u8 *ref_block,
	mlib_s32 stride)
{
	mlib_s32 y;

	MLIB_S_VIDEOCOPY8(4);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VideoCopyRef_U8_U8(
	mlib_u8 *curr_block,
	const mlib_u8 *ref_block,
	mlib_s32 width,
	mlib_s32 height,
	mlib_s32 stride)
{
	mlib_s32 y;

	if (width & 0xF) {
		MLIB_S_VIDEOCOPY8(height);
	} else {
		MLIB_S_VIDEOCOPY16(height);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
