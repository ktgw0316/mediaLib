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

#pragma ident	"@(#)mlib_s_VideoColorSplit3.c	9.5	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_VideoColorSplit3 - split three channels colors
 *
 * SYNOPSIS
 *      mlib_status mlib_VideoColorSplit3(mlib_u8       *color1,
 *                                        mlib_u8       *color2,
 *                                        mlib_u8       *color3,
 *                                        const mlib_u8 *colors,
 *                                        mlib_s32      n)
 *
 * ARGUMENTS
 *      color1    Pointer to first  color component row, 8-byte aligned
 *      color2    Pointer to second color component row, 8-byte aligned
 *      color3    Pointer to third  color component row, 8-byte aligned
 *      colors    Pointer to colors multicomponent row, 8-byte aligned
 *      n         Length of color1, color2, color3 arrays
 *                Note: length of colors array must be 3*n
 *
 * DESCRIPTION
 *
 *        color1[i] = colors[3*i]
 *        color2[i] = colors[3*i+1]
 *        color3[i] = colors[3*i+2]
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

#pragma weak mlib_VideoColorSplit3 = __mlib_VideoColorSplit3

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_VideoColorSplit3) mlib_VideoColorSplit3
	__attribute__((weak, alias("__mlib_VideoColorSplit3")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */
#define	Separate48							\
{									\
	x_t1 = _mm_unpacklo_epi8(x_s0, _mm_srli_si128(x_s1, 8));	\
	x_t2 = _mm_unpacklo_epi8(_mm_srli_si128(x_s0, 8), x_s2);	\
	x_t3 = _mm_unpacklo_epi8(x_s1, _mm_srli_si128(x_s2, 8));	\
	x_t4 = _mm_unpacklo_epi8(x_t1, _mm_srli_si128(x_t2, 8));	\
	x_t5 = _mm_unpacklo_epi8(_mm_srli_si128(x_t1, 8), x_t3);	\
									\
	x_t6 = _mm_unpacklo_epi8(x_t4, _mm_srli_si128(x_t5, 8));	\
	x_t7 = _mm_unpacklo_epi8(x_t2, _mm_srli_si128(x_t3, 8));	\
	x_t8 = _mm_unpacklo_epi8(_mm_srli_si128(x_t4, 8), x_t7);	\
	x_t9 = _mm_unpacklo_epi8(x_t5, _mm_srli_si128(x_t7, 8));	\
									\
	x_d0 = _mm_unpacklo_epi8(x_t6, _mm_srli_si128(x_t8, 8));	\
	x_d1 = _mm_unpacklo_epi8(_mm_srli_si128(x_t6, 8), x_t9);	\
	x_d2 = _mm_unpacklo_epi8(x_t8, _mm_srli_si128(x_t9, 8));	\
}

#define	Separate24							\
{									\
	x_t1 = _mm_unpacklo_epi8(x_s0, _mm_srli_si128(x_s1, 8));	\
	x_t2 = _mm_unpacklo_epi8(_mm_srli_si128(x_s0, 8), x_zero);	\
	x_t3 = _mm_unpacklo_epi8(x_s1, x_zero);			\
	x_t4 = _mm_unpacklo_epi8(x_t1, _mm_srli_si128(x_t2, 8));	\
	x_t5 = _mm_unpacklo_epi8(_mm_srli_si128(x_t1, 8), x_t3);	\
									\
	x_t6 = _mm_unpacklo_epi8(x_t4, _mm_srli_si128(x_t5, 8));	\
	x_t7 = _mm_unpacklo_epi8(x_t2, _mm_srli_si128(x_t3, 8));	\
	x_t8 = _mm_unpacklo_epi8(_mm_srli_si128(x_t4, 8), x_t7);	\
	x_t9 = _mm_unpacklo_epi8(x_t5, _mm_srli_si128(x_t7, 8));	\
									\
	x_d0 = _mm_unpacklo_epi8(x_t6, _mm_srli_si128(x_t8, 8));	\
	x_d1 = _mm_unpacklo_epi8(_mm_srli_si128(x_t6, 8), x_t9);	\
	x_d2 = _mm_unpacklo_epi8(x_t8, _mm_srli_si128(x_t9, 8));	\
}

/* *********************************************************** */
mlib_status
mlib_VideoColorSplit3_aligned(
	mlib_u8 *color1,
	mlib_u8 *color2,
	mlib_u8 *color3,
	const mlib_u8 *colors,
	mlib_s32 n);

mlib_status
mlib_VideoColorSplit3_naligned(
	mlib_u8 *color1,
	mlib_u8 *color2,
	mlib_u8 *color3,
	const mlib_u8 *colors,
	mlib_s32 n);

mlib_status
__mlib_VideoColorSplit3(
	mlib_u8 *color1,
	mlib_u8 *color2,
	mlib_u8 *color3,
	const mlib_u8 *colors,
	mlib_s32 n)
{
	if (0 == (0xf & ((mlib_addr)color1 | (mlib_addr)color2 |
		(mlib_addr)color3 |	(mlib_addr)colors))) {
		return mlib_VideoColorSplit3_aligned(
				color1, color2, color3, colors, n);
	} else {
		return mlib_VideoColorSplit3_naligned(
				color1, color2, color3, colors, n);
	}
}

mlib_status
mlib_VideoColorSplit3_aligned(
	mlib_u8 *color1,
	mlib_u8 *color2,
	mlib_u8 *color3,
	const mlib_u8 *colors,
	mlib_s32 n)
{
	mlib_u8 *pd0, *pd1, *pd2, *ps;
	__m128i x_d0, x_d1, x_d2, x_s0, x_s1, x_s2;
	__m128i x_t1, x_t2, x_t3, x_t4, x_t5, x_t6, x_t7, x_t8, x_t9;
	mlib_s32 i;
	const __m128i x_zero = _mm_setzero_si128();

	pd0 = color1;
	pd1 = color2;
	pd2 = color3;
	ps = (mlib_u8 *)colors;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i <= (n - 16); i += 16) {
		x_s0 = _mm_load_si128((__m128i *)ps);
		ps += 16;
		x_s1 = _mm_load_si128((__m128i *)ps);
		ps += 16;
		x_s2 = _mm_load_si128((__m128i *)ps);
		ps += 16;

		Separate48;

		_mm_store_si128((__m128i *)pd0, x_d0);
		pd0 += 16;
		_mm_store_si128((__m128i *)pd1, x_d1);
		pd1 += 16;
		_mm_store_si128((__m128i *)pd2, x_d2);
		pd2 += 16;
	}

	if (i <= (n - 8)) {
		x_s0 = _mm_load_si128((__m128i *)ps);
		ps += 16;
		x_s1 = _mm_loadl_epi64((__m128i *)ps);
		ps += 8;

		Separate24;

		_mm_storel_epi64((__m128i *)pd0, x_d0);
		pd0 += 8;
		_mm_storel_epi64((__m128i *)pd1, x_d1);
		pd1 += 8;
		_mm_storel_epi64((__m128i *)pd2, x_d2);
		pd2 += 8;

		i += 8;
	}

	/* C implementation */
	{
		mlib_u16 *sp = (mlib_u16 *)ps;
		mlib_u16 *dp0 = (mlib_u16 *)pd0;
		mlib_u16 *dp1 = (mlib_u16 *)pd1;
		mlib_u16 *dp2 = (mlib_u16 *)pd2;
		mlib_s32 s0, s1, s2;

		for (; i <= (n - 2); i += 2) {
			s0 = sp[0];
			s1 = sp[1];
			s2 = sp[2];
			(*dp0++) = (s1 & ~0xFF) + (s0 & 0xFF);
			(*dp1++) = (s2 << 8) + (s0 >> 8);
			(*dp2++) = (s2 & ~0xFF) + (s1 & 0xFF);
			sp += 3;
		}

		if (i < n) {
			s0 = sp[0];
			*((mlib_u8 *)dp0) = s0;
			*((mlib_u8 *)dp1) = s0 >> 8;
			*((mlib_u8 *)dp2) = ((mlib_u8 *)sp)[2];
		}
	}

	return (MLIB_SUCCESS);

}

mlib_status
mlib_VideoColorSplit3_naligned(
	mlib_u8 *color1,
	mlib_u8 *color2,
	mlib_u8 *color3,
	const mlib_u8 *colors,
	mlib_s32 n)
{
	mlib_u8 *pd0, *pd1, *pd2, *ps;
	__m128i x_d0, x_d1, x_d2, x_s0, x_s1, x_s2;
	__m128i x_t1, x_t2, x_t3, x_t4, x_t5, x_t6, x_t7, x_t8, x_t9;
	mlib_s32 i;
	const __m128i x_zero = _mm_setzero_si128();

	pd0 = color1;
	pd1 = color2;
	pd2 = color3;
	ps = (mlib_u8 *)colors;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i <= (n - 16); i += 16) {
		x_s0 = _mm_loadu_si128((__m128i *)ps);
		ps += 16;
		x_s1 = _mm_loadu_si128((__m128i *)ps);
		ps += 16;
		x_s2 = _mm_loadu_si128((__m128i *)ps);
		ps += 16;

		Separate48;

		_mm_storeu_si128((__m128i *)pd0, x_d0);
		pd0 += 16;
		_mm_storeu_si128((__m128i *)pd1, x_d1);
		pd1 += 16;
		_mm_storeu_si128((__m128i *)pd2, x_d2);
		pd2 += 16;
	}

	if (i <= (n - 8)) {
		x_s0 = _mm_loadu_si128((__m128i *)ps);
		ps += 16;
		x_s1 = _mm_loadl_epi64((__m128i *)ps);
		ps += 8;

		Separate24;

		_mm_storel_epi64((__m128i *)pd0, x_d0);
		pd0 += 8;
		_mm_storel_epi64((__m128i *)pd1, x_d1);
		pd1 += 8;
		_mm_storel_epi64((__m128i *)pd2, x_d2);
		pd2 += 8;

		i += 8;
	}

	/* C implementation */
	{
		mlib_u16 *sp = (mlib_u16 *)ps;
		mlib_u16 *dp0 = (mlib_u16 *)pd0;
		mlib_u16 *dp1 = (mlib_u16 *)pd1;
		mlib_u16 *dp2 = (mlib_u16 *)pd2;
		mlib_s32 s0, s1, s2;

		for (; i <= (n - 2); i += 2) {
			s0 = sp[0];
			s1 = sp[1];
			s2 = sp[2];
			(*dp0++) = (s1 & ~0xFF) + (s0 & 0xFF);
			(*dp1++) = (s2 << 8) + (s0 >> 8);
			(*dp2++) = (s2 & ~0xFF) + (s1 & 0xFF);
			sp += 3;
		}

		if (i < n) {
			s0 = sp[0];
			*((mlib_u8 *)dp0) = s0;
			*((mlib_u8 *)dp1) = s0 >> 8;
			*((mlib_u8 *)dp2) = ((mlib_u8 *)sp)[2];
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
