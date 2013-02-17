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

#pragma ident	"@(#)mlib_s_VideoColorSplit2.c	9.5	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_VideoColorSplit2 - split two channels colors
 *
 * SYNOPSIS
 *      mlib_status mlib_VideoColorSplit2(mlib_u8       *color1,
 *                                        mlib_u8       *color2,
 *                                        const mlib_u8 *colors,
 *                                        mlib_s32      n)
 *
 * ARGUMENTS
 *      color1    Pointer to first  color component row, 8-byte aligned
 *      color2    Pointer to second color component row, 8-byte aligned
 *      colors    Pointer to colors multicomponent row, 8-byte aligned
 *      n         Length of color1, color2 arrays
 *                Note: length of colors array must be 2*n
 *
 * DESCRIPTION
 *
 *        color1[i] = colors[2*i]
 *        color2[i] = colors[2*i+1]
 *
 *        0 <= i < n
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

#pragma weak mlib_VideoColorSplit2 = __mlib_VideoColorSplit2

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_VideoColorSplit2) mlib_VideoColorSplit2
	__attribute__((weak, alias("__mlib_VideoColorSplit2")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

mlib_status
mlib_VideoColorSplit2_aligned(
	mlib_u8 *color1,
	mlib_u8 *color2,
	const mlib_u8 *colors,
	mlib_s32 n);

mlib_status
mlib_VideoColorSplit2_naligned(
	mlib_u8 *color1,
	mlib_u8 *color2,
	const mlib_u8 *colors,
	mlib_s32 n);

mlib_status
__mlib_VideoColorSplit2(
	mlib_u8 *color1,
	mlib_u8 *color2,
	const mlib_u8 *colors,
	mlib_s32 n)
{
	if (0 == (0xf & ((mlib_addr)color1 |
		(mlib_addr)color2 | (mlib_addr)colors))) {
		return mlib_VideoColorSplit2_aligned(
				color1, color2, colors, n);
	} else {
		return mlib_VideoColorSplit2_naligned(
				color1, color2, colors, n);
	}
}

mlib_status
mlib_VideoColorSplit2_aligned(
	mlib_u8 *color1,
	mlib_u8 *color2,
	const mlib_u8 *colors,
	mlib_s32 n)
{
	mlib_u8 *pd1, *pd2, *ps;
	mlib_s32 i;
	__m128i x_s1, x_s2, x_d1, x_d2, x_d;
	const __m128i x_mask1 = _mm_set1_epi16(0xff);
	const __m128i x_zero = _mm_setzero_si128();

	ps = (mlib_u8 *)colors;
	pd1 = color1;
	pd2 = color2;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i <= (n - 16); i += 16) {
#if 0
/* alternative implementation: not fast enough */
		x_s1 = _mm_load_si128((__m128i *)ps);
		ps += 16;
		x_s2 = _mm_load_si128((__m128i *)ps);
		ps += 16;

		x_d1 = _mm_unpacklo_epi8(x_s1, x_s2);
		x_d2 = _mm_unpackhi_epi8(x_s1, x_s2);
		x_s1 = _mm_unpacklo_epi8(x_d1, x_d2);
		x_s2 = _mm_unpackhi_epi8(x_d1, x_d2);
		x_d1 = _mm_unpacklo_epi8(x_s1, x_s2);
		x_d2 = _mm_unpackhi_epi8(x_s1, x_s2);
		x_s1 = _mm_unpacklo_epi8(x_d1, x_d2);
		x_s2 = _mm_unpackhi_epi8(x_d1, x_d2);

		_mm_store_si128((__m128i *)pd1, x_s1);
		pd1 += 16;
		_mm_store_si128((__m128i *)pd2, x_s2);
		pd2 += 16;
#else
		x_s1 = _mm_load_si128((__m128i *)ps);
		ps += 16;
		x_s2 = _mm_load_si128((__m128i *)ps);
		ps += 16;

		x_d1 = _mm_and_si128(x_mask1, x_s1);
		x_d2 = _mm_and_si128(x_mask1, x_s2);
		x_d = _mm_packus_epi16(x_d1, x_d2);
		_mm_store_si128((__m128i *)pd1, x_d);
		pd1 += 16;

		x_d1 = _mm_srli_epi16(x_s1, 8);
		x_d2 = _mm_srli_epi16(x_s2, 8);
		x_d = _mm_packus_epi16(x_d1, x_d2);
		_mm_store_si128((__m128i *)pd2, x_d);
		pd2 += 16;
#endif
	}

	if (i <= (n - 8)) {
		x_s1 = _mm_load_si128((__m128i *)ps);
		ps += 16;

		x_d1 = _mm_and_si128(x_mask1, x_s1);
		x_d = _mm_packus_epi16(x_d1, x_zero);
		_mm_storel_epi64((__m128i *)pd1, x_d);
		pd1 += 8;

		x_d1 = _mm_srli_epi16(x_s1, 8);
		x_d = _mm_packus_epi16(x_d1, x_zero);
		_mm_storel_epi64((__m128i *)pd2, x_d);
		pd2 += 8;

		i += 8;
	}

	/* pure C implementation */
	{
		mlib_u16 *sp = (mlib_u16 *)ps;
		mlib_u16 *dp0 = (mlib_u16 *)pd1;
		mlib_u16 *dp1 = (mlib_u16 *)pd2;
		mlib_s32 s0, s1;

		for (; i <= (n - 2); i += 2) {
			s0 = sp[0];
			s1 = sp[1];
			(*dp1++) = (s1 & ~0xff) | (s0 >> 8);
			(*dp0++) = (s1 << 8) | (s0 & 0xff);
			sp += 2;
		}

		if (i < n) {
			s0 = sp[0];
			*((mlib_u8 *)dp1) = s0 >> 8;
			*((mlib_u8 *)dp0) = s0;
		}
	}

	return (MLIB_SUCCESS);
}

mlib_status
mlib_VideoColorSplit2_naligned(
	mlib_u8 *color1,
	mlib_u8 *color2,
	const mlib_u8 *colors,
	mlib_s32 n)
{
	mlib_u8 *pd1, *pd2, *ps;
	mlib_s32 i;
	__m128i x_s1, x_s2, x_d1, x_d2, x_d;
	const __m128i x_mask1 = _mm_set1_epi16(0xff);
	const __m128i x_zero = _mm_setzero_si128();

	ps = (mlib_u8 *)colors;
	pd1 = color1;
	pd2 = color2;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i <= (n - 16); i += 16) {
		x_s1 = _mm_loadu_si128((__m128i *)ps);
		ps += 16;
		x_s2 = _mm_loadu_si128((__m128i *)ps);
		ps += 16;

		x_d1 = _mm_and_si128(x_mask1, x_s1);
		x_d2 = _mm_and_si128(x_mask1, x_s2);
		x_d = _mm_packus_epi16(x_d1, x_d2);
		_mm_storeu_si128((__m128i *)pd1, x_d);
		pd1 += 16;

		x_d1 = _mm_srli_epi16(x_s1, 8);
		x_d2 = _mm_srli_epi16(x_s2, 8);
		x_d = _mm_packus_epi16(x_d1, x_d2);
		_mm_storeu_si128((__m128i *)pd2, x_d);
		pd2 += 16;
	}

	if (i <= (n - 8)) {
		x_s1 = _mm_loadu_si128((__m128i *)ps);
		ps += 16;

		x_d1 = _mm_and_si128(x_mask1, x_s1);
		x_d = _mm_packus_epi16(x_d1, x_zero);
		_mm_storel_epi64((__m128i *)pd1, x_d);
		pd1 += 8;

		x_d1 = _mm_srli_epi16(x_s1, 8);
		x_d = _mm_packus_epi16(x_d1, x_zero);
		_mm_storel_epi64((__m128i *)pd2, x_d);
		pd2 += 8;

		i += 8;
	}

	/* pure C implementation */
	{
		mlib_u16 *sp = (mlib_u16 *)ps;
		mlib_u16 *dp0 = (mlib_u16 *)pd1;
		mlib_u16 *dp1 = (mlib_u16 *)pd2;
		mlib_s32 s0, s1;

		for (; i <= (n - 2); i += 2) {
			s0 = sp[0];
			s1 = sp[1];
			(*dp1++) = (s1 & ~0xff) | (s0 >> 8);
			(*dp0++) = (s1 << 8) | (s0 & 0xff);
			sp += 2;
		}

		if (i < n) {
			s0 = sp[0];
			*((mlib_u8 *)dp1) = s0 >> 8;
			*((mlib_u8 *)dp0) = s0;
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
