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

#pragma ident	"@(#)mlib_s_VideoSumAbsDiff.c	9.2	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_VideoSumAbsDiff - Computes the sum of absolute differences
 *                             between the pixels in the current block and
 *                             the corresponding pixels in the reference
 *                             block
 *
 * SYNOPSIS
 *    mlib_s32 mlib_VideoSumAbsDiff(mlib_u8       *curr_block,
 *                                  const mlib_u8 *ref_block,
 *                                  mlib_s32      width,
 *                                  mlib_s32      height,
 *                                  mlib_s32      stride)
 *
 * ARGUMENT
 *      curr_block  Pointer to current block, must be 8-byte aligned
 *      ref_block   Pointer to reference block
 *      width       Width of the blocks, must be a multiply of 8
 *      height      Height of the blocks, must be a multiply of 4
 *      stride      Stride in bytes between adjacent rows in a block,
 *                  must be a multiply of 8
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

#pragma weak mlib_VideoSumAbsDiff = __mlib_VideoSumAbsDiff

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_VideoSumAbsDiff) mlib_VideoSumAbsDiff
	__attribute__((weak, alias("__mlib_VideoSumAbsDiff")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */



/* *********************************************************** */

mlib_s32
__mlib_VideoSumAbsDiff(
	mlib_u8 *curr_block,
	const mlib_u8 *ref_block,
	mlib_s32 width,
	mlib_s32 height,
	mlib_s32 stride)
{
	__m128i txmm0, txmm1, txmm2, txmm3, txmm4, txmm5, txmm6, txmm7;

	mlib_s32 accum;
	mlib_u8 *psa1 = curr_block;
	mlib_u8 *psa2 = (void *)ref_block;
	__m128i *sa1 = (__m128i *)curr_block;
	__m128i *sa2 = (__m128i *)ref_block;
	mlib_s32 i, j;
	mlib_s32 dw = width & 0xF;

	txmm7 = _mm_setzero_si128();
	txmm6 = txmm7;

	for (i = 0; i < height; i++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif
		for (j = 0; j < width - dw; j += 16) {
			txmm0 = _mm_loadu_si128((__m128i *)sa1++);
			txmm1 = _mm_loadu_si128((__m128i *)sa2++);
			txmm2 = _mm_sad_epu8(txmm0, txmm1);
			txmm7 = _mm_add_epi32(txmm7, txmm2);
		}
		for (j = 0; j < dw; j += 8) {
			txmm0 = _mm_loadu_si128((__m128i *)sa1++);
			txmm0 = _mm_unpacklo_epi64(txmm0, txmm6);
			txmm1 = _mm_loadu_si128((__m128i *)sa2++);
			txmm1 = _mm_unpacklo_epi64(txmm1, txmm6);
			txmm2 = _mm_sad_epu8(txmm0, txmm1);
			txmm7 = _mm_add_epi32(txmm7, txmm2);
		}

		psa1 += stride;
		psa2 += stride;
		sa1 = (__m128i *)psa1;
		sa2 = (__m128i *)psa2;
	}
	accum = _mm_cvtsi128_si32(txmm7);
	txmm7 = _mm_srli_si128(txmm7, 8);
	accum += _mm_cvtsi128_si32(txmm7);

	return (accum);
}

/* *********************************************************** */
