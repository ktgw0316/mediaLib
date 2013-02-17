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

#pragma ident	"@(#)mlib_s_VideoIDCT8x8_DC.c	9.3	07/11/05 SMI"

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

#ifdef __SUNPRO_C
#include <sunmedia_intrin.h>
#else
#include <emmintrin.h>
#endif
/* *********************************************************** */

mlib_status
__mlib_VideoIDCT8x8_U8_S16_DC(
	mlib_u8 *block,
	const mlib_s16 *coeffs,
	mlib_s32 stride)
{
	mlib_s32 val;
	mlib_s16 res;
	__m128i txmm0;

	val = coeffs[0];
	res = (val + 4) >> 3;

	res = (res | ~((val - 2044) >> 31)) & ~(val >> 31);

	txmm0 = _mm_set1_epi8(res);

	_mm_storel_epi64((__m128i *)block, txmm0);
	_mm_storel_epi64((__m128i *)(block + stride), txmm0);
	_mm_storel_epi64((__m128i *)(block + 2 * stride), txmm0);
	_mm_storel_epi64((__m128i *)(block + 3 * stride), txmm0);
	_mm_storel_epi64((__m128i *)(block + 4 * stride), txmm0);
	_mm_storel_epi64((__m128i *)(block + 5 * stride), txmm0);
	_mm_storel_epi64((__m128i *)(block + 6 * stride), txmm0);
	_mm_storel_epi64((__m128i *)(block + 7 * stride), txmm0);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VideoIDCT8x8_S16_S16_DC(
	mlib_s16 *block,
	const mlib_s16 *coeffs)
{
	mlib_s32 val, res;
	__m128i txmm0;

	val = coeffs[0];
	res = (val + 4) >> 3;

	res = (res & 0xffff) | (res << 16);

	txmm0 = _mm_set1_epi32(res);

	_mm_storeu_si128((__m128i *)(block), txmm0);
	_mm_storeu_si128((__m128i *)(block + 8), txmm0);
	_mm_storeu_si128((__m128i *)(block + 16), txmm0);
	_mm_storeu_si128((__m128i *)(block + 24), txmm0);
	_mm_storeu_si128((__m128i *)(block + 32), txmm0);
	_mm_storeu_si128((__m128i *)(block + 40), txmm0);
	_mm_storeu_si128((__m128i *)(block + 48), txmm0);
	_mm_storeu_si128((__m128i *)(block + 56), txmm0);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
