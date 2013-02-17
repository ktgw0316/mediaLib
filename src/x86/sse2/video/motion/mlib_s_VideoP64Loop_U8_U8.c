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

#pragma ident	"@(#)mlib_s_VideoP64Loop_U8_U8.c	9.2	07/11/05 SMI"

/*
 * FUNCTION
 *    mlib_VideoP64Loop_U8_U8 - Applies a 2-dimensional 3x3 spatial
 *                              filter on the reference block
 *
 * SYNOPSIS
 *    mlib_status mlib_VideoP64Loop_U8_U8(mlib_u8       *curr_block,
 *                                        const mlib_u8 *ref_block,
 *                                        mlib_s32      stride)
 *
 * ARGUMENT
 *      curr_block Pointer to current block, must be 8-byte aligned
 *      ref_block  Pointer to reference block
 *      stride     Stride in bytes between adjacent rows in both
 *                 current and reference block, must be a multiply of 8
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

#pragma weak mlib_VideoP64Loop_U8_U8 = __mlib_VideoP64Loop_U8_U8

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_VideoP64Loop_U8_U8) mlib_VideoP64Loop_U8_U8
	__attribute__((weak, alias("__mlib_VideoP64Loop_U8_U8")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	FILTERX(x)					\
	t0 = _mm_srli_si128(txmm##x, 2);		\
	t1 = _mm_slli_si128(txmm##x, 2);		\
	t7 = _mm_and_si128(txmm##x, CF);		\
	t2 = _mm_andnot_si128(CF, t0);			\
	t3 = _mm_andnot_si128(CF, t1);			\
	t4 = _mm_or_si128(t2, t7);			\
	t5 = _mm_or_si128(t3, t7);			\
	t6 = _mm_add_epi16(t4, txmm##x);		\
	t7 = _mm_add_epi16(t5, txmm##x);		\
	txmm##x = _mm_add_epi16(t6, t7);

#define	LOADL(x)                                                \
		txmm##x = _mm_loadu_si128((__m128i *)sl);	\
		txmm##x = _mm_unpacklo_epi8(txmm##x, Czero);

#define	ADDL(x, y)                                              \
	txmm##x = _mm_add_epi16(txmm##x, txmm##y);

/* *********************************************************** */

#define	ADDLRND(x, y)                                           \
	txmm##x = _mm_add_epi16(txmm##x, txmm##y);		\
	txmm##x = _mm_add_epi16(txmm##x, C8);

/* *********************************************************** */

#define	STORB(x)                                                      \
	t0 = _mm_add_epi16(txmm##x, C2);			      \
	t0 = _mm_srli_epi16(t0, 2);				      \
	t0 = _mm_packus_epi16(t0, Czero);			      \
	_mm_storel_epi64((__m128i *)sd, t0);

/* *********************************************************** */

#define	STORSUM(x, y)                                              \
	t0 = _mm_add_epi16(txmm##x, txmm##y);			   \
	t0 = _mm_srli_epi16(t0, 4);		      	   	   \
	t0 = _mm_packus_epi16(t0, Czero);			   \
	_mm_storel_epi64((__m128i *)sd, t0);

/* *********************************************************** */

mlib_status
__mlib_VideoP64Loop_U8_U8(
	mlib_u8 *curr_block,
	const mlib_u8 *ref_block,
	mlib_s32 stride)
{
	const mlib_u8 *sl;
	mlib_u8 *sd;
	mlib_u32 in0, in1, in2, in3, in4, in5, in6, in7;
	mlib_u32 a0, a1, a2, a3;
	mlib_u32 b0, b1, b2, b3;
	mlib_u32 c0, c1, c2, c3;
	mlib_u32 d0, d1;
	__m128i txmm0, txmm1, txmm2, txmm3, txmm4, txmm5, txmm6, txmm7;
	__m128i t0, t1, t2, t3, t4, t5, t6, t7;
	__m128i Czero, CF, C2, C4, C8;

	Czero = _mm_setzero_si128();
	C2 = _mm_set1_epi16(2);
	C4 = _mm_set1_epi16(4);
	C8 = _mm_set1_epi16(8);
	CF = _mm_set_epi32(0xff0000, 0, 0, 0xff);

	sd = curr_block;
	sl = ref_block;

	LOADL(0);
	sl += stride;
	FILTERX(0);

	LOADL(1);
	sl += stride;
	FILTERX(1);

	STORB(0);
	sd += stride;
	ADDL(0, 1);

	LOADL(2);
	sl += stride;
	FILTERX(2);

	ADDLRND(1, 2);
	STORSUM(0, 1);
	sd += stride;

	LOADL(3);
	sl += stride;
	FILTERX(3);

	ADDL(2, 3);
	STORSUM(1, 2);
	sd += stride;

	LOADL(4);
	sl += stride;
	FILTERX(4);

	ADDLRND(3, 4);
	STORSUM(2, 3);
	sd += stride;

	LOADL(5);
	sl += stride;
	FILTERX(5);

	ADDL(4, 5);
	STORSUM(3, 4);
	sd += stride;

	LOADL(6);
	sl += stride;
	FILTERX(6);

	ADDLRND(5, 6);
	STORSUM(4, 5);
	sd += stride;

	LOADL(7);
	FILTERX(7);

	ADDL(6, 7);
	STORSUM(5, 6);
	sd += stride;

	STORB(7);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
