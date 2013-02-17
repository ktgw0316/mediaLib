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

#pragma ident	"@(#)mlib_s_VideoColorSplit4_S16.c	9.4	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_VideoColorSplit4_S16 - split four channels colors
 *
 * SYNOPSIS
 *      mlib_status mlib_VideoColorSplit4_S16(mlib_s16       *color1,
 *                                            mlib_s16       *color2,
 *                                            mlib_s16       *color3,
 *                                            mlib_s16       *color4,
 *                                            const mlib_s16 *colors,
 *                                            mlib_s32       n)
 *
 * ARGUMENTS
 *      color1    Pointer to first  color component row, 8-byte aligned
 *      color2    Pointer to second color component row, 8-byte aligned
 *      color3    Pointer to third  color component row, 8-byte aligned
 *      color4    Pointer to forth  color component row, 8-byte aligned
 *      colors    Pointer to colors multicomponent row, 8-byte aligned
 *      n         Length of color1, color2, color3, color4 arrays
 *                Note: length of colors array must be 4*n
 *
 * DESCRIPTION
 *
 *      color1[i] = colors[4*i]
 *      color2[i] = colors[4*i+1]
 *      color3[i] = colors[4*i+2]
 *      color4[i] = colors[4*i+3]
 *
 *      0 <= i < n
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

#pragma weak mlib_VideoColorSplit4_S16 = __mlib_VideoColorSplit4_S16

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_VideoColorSplit4_S16) mlib_VideoColorSplit4_S16
	__attribute__((weak, alias("__mlib_VideoColorSplit4_S16")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */
#define	Separate64_S16					\
{							\
	x_t1 = _mm_unpacklo_epi16(x_s0, x_s1);		\
	x_t2 = _mm_unpackhi_epi16(x_s0, x_s1);		\
	x_t3 = _mm_unpacklo_epi16(x_s2, x_s3);		\
	x_t4 = _mm_unpackhi_epi16(x_s2, x_s3);		\
							\
	x_t5 = _mm_unpacklo_epi16(x_t1, x_t2);		\
	x_t6 = _mm_unpackhi_epi16(x_t1, x_t2);		\
	x_t7 = _mm_unpacklo_epi16(x_t3, x_t4);		\
	x_t8 = _mm_unpackhi_epi16(x_t3, x_t4);		\
							\
	x_d0 = _mm_unpacklo_epi64(x_t5, x_t7);		\
	x_d1 = _mm_unpackhi_epi64(x_t5, x_t7);		\
	x_d2 = _mm_unpacklo_epi64(x_t6, x_t8);		\
	x_d3 = _mm_unpackhi_epi64(x_t6, x_t8);		\
}

#define	Separate32_S16					\
{							\
	x_t1 = _mm_unpacklo_epi16(x_s0, x_s1);		\
	x_t2 = _mm_unpackhi_epi16(x_s0, x_s1);		\
							\
	x_t5 = _mm_unpacklo_epi16(x_t1, x_t2);		\
	x_t6 = _mm_unpackhi_epi16(x_t1, x_t2);		\
							\
	x_d0 = _mm_unpacklo_epi64(x_t5, x_zero);	\
	x_d1 = _mm_unpackhi_epi64(x_t5, x_zero);	\
	x_d2 = _mm_unpacklo_epi64(x_t6, x_zero);	\
	x_d3 = _mm_unpackhi_epi64(x_t6, x_zero);	\
}

/* *********************************************************** */
mlib_status
mlib_VideoColorSplit4_S16_aligned(
	mlib_s16 *color1,
	mlib_s16 *color2,
	mlib_s16 *color3,
	mlib_s16 *color4,
	const mlib_s16 *colors,
	mlib_s32 n);

mlib_status
mlib_VideoColorSplit4_S16_naligned(
	mlib_s16 *color1,
	mlib_s16 *color2,
	mlib_s16 *color3,
	mlib_s16 *color4,
	const mlib_s16 *colors,
	mlib_s32 n);

mlib_status
__mlib_VideoColorSplit4_S16(
	mlib_s16 *color1,
	mlib_s16 *color2,
	mlib_s16 *color3,
	mlib_s16 *color4,
	const mlib_s16 *colors,
	mlib_s32 n)
{
	if (0 == (0xf & ((mlib_addr)color1 |
		(mlib_addr)color2 |	(mlib_addr)color3 |
		(mlib_addr)color4 |	(mlib_addr)colors))) {
		return mlib_VideoColorSplit4_S16_aligned(
				color1, color2, color3, color4, colors, n);
	} else {
		return mlib_VideoColorSplit4_S16_naligned(
				color1, color2, color3, color4, colors, n);
	}
}

mlib_status
mlib_VideoColorSplit4_S16_aligned(
	mlib_s16 *color1,
	mlib_s16 *color2,
	mlib_s16 *color3,
	mlib_s16 *color4,
	const mlib_s16 *colors,
	mlib_s32 n)
{
	__m128i x_s0, x_s1, x_s2, x_s3, x_d0, x_d1, x_d2, x_d3;
	__m128i x_t1, x_t2, x_t3, x_t4, x_t5, x_t6, x_t7, x_t8;
	const __m128i x_zero = _mm_setzero_si128();
	mlib_s16 *ps, *pd0, *pd1, *pd2, *pd3;
	mlib_s32 i;

	ps = (mlib_s16 *)colors;
	pd0 = color1;
	pd1 = color2;
	pd2 = color3;
	pd3 = color4;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i <= (n - 8); i += 8) {
		x_s0 = _mm_load_si128((__m128i *)ps);
		ps += 8;
		x_s1 = _mm_load_si128((__m128i *)ps);
		ps += 8;
		x_s2 = _mm_load_si128((__m128i *)ps);
		ps += 8;
		x_s3 = _mm_load_si128((__m128i *)ps);
		ps += 8;

		Separate64_S16;

		_mm_store_si128((__m128i *)pd0, x_d0);
		pd0 += 8;
		_mm_store_si128((__m128i *)pd1, x_d1);
		pd1 += 8;
		_mm_store_si128((__m128i *)pd2, x_d2);
		pd2 += 8;
		_mm_store_si128((__m128i *)pd3, x_d3);
		pd3 += 8;
	}

	if (i <= (n - 4)) {
		x_s0 = _mm_load_si128((__m128i *)ps);
		ps += 8;
		x_s1 = _mm_load_si128((__m128i *)ps);
		ps += 8;
		Separate32_S16;

		_mm_storel_epi64((__m128i *)pd0, x_d0);
		pd0 += 4;
		_mm_storel_epi64((__m128i *)pd1, x_d1);
		pd1 += 4;
		_mm_storel_epi64((__m128i *)pd2, x_d2);
		pd2 += 4;
		_mm_storel_epi64((__m128i *)pd3, x_d3);
		pd3 += 4;

		i += 4;
	}

	/* pure C implementation */
	{
		mlib_u32 *sp = (mlib_u32 *)ps;
		mlib_u32 *dp0 = (mlib_u32 *)pd0;
		mlib_u32 *dp1 = (mlib_u32 *)pd1;
		mlib_u32 *dp2 = (mlib_u32 *)pd2;
		mlib_u32 *dp3 = (mlib_u32 *)pd3;
		mlib_u32 s0, s1, s2, s3;

		for (; i <= (n - 2); i += 2) {
			s0 = sp[0];
			s1 = sp[1];
			s2 = sp[2];
			s3 = sp[3];
			(*dp1++) = (s2 & ~0xFFFF) + (s0 >> 16);
			(*dp0++) = (s2 << 16) + (s0 & 0xFFFF);
			(*dp3++) = (s3 & ~0xFFFF) + (s1 >> 16);
			(*dp2++) = (s3 << 16) + (s1 & 0xFFFF);
			sp += 4;
		}

		if (i < n) {
			s0 = sp[0];
			s1 = sp[1];
			((mlib_u16 *)dp1)[0] = s0 >> 16;
			((mlib_u16 *)dp0)[0] = s0;
			((mlib_u16 *)dp3)[0] = s1 >> 16;
			((mlib_u16 *)dp2)[0] = s1;
		}
	}

	return (MLIB_SUCCESS);
}

mlib_status
mlib_VideoColorSplit4_S16_naligned(
	mlib_s16 *color1,
	mlib_s16 *color2,
	mlib_s16 *color3,
	mlib_s16 *color4,
	const mlib_s16 *colors,
	mlib_s32 n)
{
	__m128i x_s0, x_s1, x_s2, x_s3, x_d0, x_d1, x_d2, x_d3;
	__m128i x_t1, x_t2, x_t3, x_t4, x_t5, x_t6, x_t7, x_t8;
	const __m128i x_zero = _mm_setzero_si128();
	mlib_s16 *ps, *pd0, *pd1, *pd2, *pd3;
	mlib_s32 i;

	ps = (mlib_s16 *)colors;
	pd0 = color1;
	pd1 = color2;
	pd2 = color3;
	pd3 = color4;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i <= (n - 8); i += 8) {
		x_s0 = _mm_loadu_si128((__m128i *)ps);
		ps += 8;
		x_s1 = _mm_loadu_si128((__m128i *)ps);
		ps += 8;
		x_s2 = _mm_loadu_si128((__m128i *)ps);
		ps += 8;
		x_s3 = _mm_loadu_si128((__m128i *)ps);
		ps += 8;

		Separate64_S16;

		_mm_storeu_si128((__m128i *)pd0, x_d0);
		pd0 += 8;
		_mm_storeu_si128((__m128i *)pd1, x_d1);
		pd1 += 8;
		_mm_storeu_si128((__m128i *)pd2, x_d2);
		pd2 += 8;
		_mm_storeu_si128((__m128i *)pd3, x_d3);
		pd3 += 8;
	}

	if (i <= (n - 4)) {
		x_s0 = _mm_loadu_si128((__m128i *)ps);
		ps += 8;
		x_s1 = _mm_loadu_si128((__m128i *)ps);
		ps += 8;
		Separate32_S16;

		_mm_storel_epi64((__m128i *)pd0, x_d0);
		pd0 += 4;
		_mm_storel_epi64((__m128i *)pd1, x_d1);
		pd1 += 4;
		_mm_storel_epi64((__m128i *)pd2, x_d2);
		pd2 += 4;
		_mm_storel_epi64((__m128i *)pd3, x_d3);
		pd3 += 4;

		i += 4;
	}

	/* pure C implementation */
	{
		mlib_u32 *sp = (mlib_u32 *)ps;
		mlib_u32 *dp0 = (mlib_u32 *)pd0;
		mlib_u32 *dp1 = (mlib_u32 *)pd1;
		mlib_u32 *dp2 = (mlib_u32 *)pd2;
		mlib_u32 *dp3 = (mlib_u32 *)pd3;
		mlib_u32 s0, s1, s2, s3;

		for (; i <= (n - 2); i += 2) {
			s0 = sp[0];
			s1 = sp[1];
			s2 = sp[2];
			s3 = sp[3];
			(*dp1++) = (s2 & ~0xFFFF) + (s0 >> 16);
			(*dp0++) = (s2 << 16) + (s0 & 0xFFFF);
			(*dp3++) = (s3 & ~0xFFFF) + (s1 >> 16);
			(*dp2++) = (s3 << 16) + (s1 & 0xFFFF);
			sp += 4;
		}

		if (i < n) {
			s0 = sp[0];
			s1 = sp[1];
			((mlib_u16 *)dp1)[0] = s0 >> 16;
			((mlib_u16 *)dp0)[0] = s0;
			((mlib_u16 *)dp3)[0] = s1 >> 16;
			((mlib_u16 *)dp2)[0] = s1;
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
