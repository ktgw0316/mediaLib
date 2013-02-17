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

#pragma ident	"@(#)mlib_s_VideoColorMerge3_S16.c	9.5	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_VideoColorMerge3_S16 - merges three channels colors
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_VideoColorMerge3_S16(mlib_s16       *colors,
 *                                            const mlib_s16 *color1,
 *                                            const mlib_s16 *color2,
 *                                            const mlib_s16 *color3,
 *                                            mlib_s32       n)
 *
 * ARGUMENTS
 *      colors    Pointer to colors multicomponent row, 8-byte aligned
 *      color1    Pointer to first color component row, 8-byte aligned
 *      color2    Pointer to second color component row, 8-byte aligned
 *      color3    Pointer to third color component row, 8-byte aligned
 *      n         Length of the color1, color2, color3 arrays
 *
 * DESCRIPTION
 *
 *      colors[3*i]   = color1[i]
 *      colors[3*i+1] = color2[i]
 *      colors[3*i+2] = color3[i]
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

#pragma weak mlib_VideoColorMerge3_S16 = __mlib_VideoColorMerge3_S16

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_VideoColorMerge3_S16) mlib_VideoColorMerge3_S16
	__attribute__((weak, alias("__mlib_VideoColorMerge3_S16")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */
#define	PACK_1	\
{							\
	_mm_storel_epi64((__m128i *)pd, x_d);			\
	pd += 3;				\
	_mm_storel_epi64((__m128i *)pd, _mm_srli_si128(x_d, 8));	\
	pd += 3;				\
}

#define	PACK_2	\
{							\
	pout = (mlib_s16 *)(&x_d);			\
	_mm_storel_epi64((__m128i *)pd, x_d);			\
	pd += 3;				\
	*((mlib_s32 *)pd) = *((mlib_s32 *)(pout+4));		\
	pd += 2;				\
	*pd = pout[6];			\
	pd += 1;				\
}

/* *********************************************************** */
mlib_status
mlib_VideoColorMerge3_S16_aligned(
	mlib_s16 *colors,
	const mlib_s16 *color1,
	const mlib_s16 *color2,
	const mlib_s16 *color3,
	mlib_s32 n);

mlib_status
mlib_VideoColorMerge3_S16_naligned(
	mlib_s16 *colors,
	const mlib_s16 *color1,
	const mlib_s16 *color2,
	const mlib_s16 *color3,
	mlib_s32 n);

mlib_status
__mlib_VideoColorMerge3_S16(
	mlib_s16 *colors,
	const mlib_s16 *color1,
	const mlib_s16 *color2,
	const mlib_s16 *color3,
	mlib_s32 n)
{
	if (0 == (0xf & ((mlib_addr)color1 |
		(mlib_addr)color2 | (mlib_addr)color3))) {
		return mlib_VideoColorMerge3_S16_aligned(
				colors, color1, color2, color3, n);
	} else {
		return mlib_VideoColorMerge3_S16_naligned(
				colors, color1, color2, color3, n);
	}
}

mlib_status
mlib_VideoColorMerge3_S16_aligned(
	mlib_s16 *colors,
	const mlib_s16 *color1,
	const mlib_s16 *color2,
	const mlib_s16 *color3,
	mlib_s32 n)
{
	mlib_s16 *ps1, *ps2, *ps3, *pd, *pout;
	mlib_s32 i;

	ps1 = (mlib_s16 *)color1;
	ps2 = (mlib_s16 *)color2;
	ps3 = (mlib_s16 *)color3;
	pd = colors;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i <= (n - 16); i += 8)	{
		__m128i x_s1, x_s2, x_s3, x_d1, x_d2, x_d3, x_d4, x_d;
		x_s1 = _mm_load_si128((__m128i *)ps1);
		ps1 += 8;
		x_s2 = _mm_load_si128((__m128i *)ps2);
		ps2 += 8;
		x_s3 = _mm_load_si128((__m128i *)ps3);
		ps3 += 8;

		x_d1 = _mm_unpacklo_epi16(x_s1, x_s2);
		x_d2 = _mm_unpackhi_epi16(x_s1, x_s2);
		x_d3 = _mm_unpacklo_epi16(x_s3, x_s3);
		x_d4 = _mm_unpackhi_epi16(x_s3, x_s3);

		x_d = _mm_unpacklo_epi32(x_d1, x_d3);
		PACK_1;

		x_d = _mm_unpackhi_epi32(x_d1, x_d3);
		PACK_1;

		x_d = _mm_unpacklo_epi32(x_d2, x_d4);
		PACK_1;

		x_d = _mm_unpackhi_epi32(x_d2, x_d4);
		PACK_1;
	}

	if (i <= (n - 8)) {
		__m128i x_s1, x_s2, x_s3, x_d1, x_d2, x_d3, x_d4, x_d;
		x_s1 = _mm_load_si128((__m128i *)ps1);
		ps1 += 8;
		x_s2 = _mm_load_si128((__m128i *)ps2);
		ps2 += 8;
		x_s3 = _mm_load_si128((__m128i *)ps3);
		ps3 += 8;

		x_d1 = _mm_unpacklo_epi16(x_s1, x_s2);
		x_d2 = _mm_unpackhi_epi16(x_s1, x_s2);
		x_d3 = _mm_unpacklo_epi16(x_s3, x_s3);
		x_d4 = _mm_unpackhi_epi16(x_s3, x_s3);

		x_d = _mm_unpacklo_epi32(x_d1, x_d3);
		PACK_1;

		x_d = _mm_unpackhi_epi32(x_d1, x_d3);
		PACK_1;

		x_d = _mm_unpacklo_epi32(x_d2, x_d4);
		PACK_1;

		x_d = _mm_unpackhi_epi32(x_d2, x_d4);
		PACK_2;

		i += 8;
	}

	if (i <= (n - 4)) {
		__m128i x_s1, x_s2, x_s3, x_d1, x_d2, x_d3, x_d4, x_d;
		x_s1 = _mm_loadl_epi64((__m128i *)ps1);
		ps1 += 4;
		x_s2 = _mm_loadl_epi64((__m128i *)ps2);
		ps2 += 4;
		x_s3 = _mm_loadl_epi64((__m128i *)ps3);
		ps3 += 4;

		x_d1 = _mm_unpacklo_epi16(x_s1, x_s2);
		x_d3 = _mm_unpacklo_epi16(x_s3, x_s3);

		x_d = _mm_unpacklo_epi32(x_d1, x_d3);
		PACK_1;

		x_d = _mm_unpackhi_epi32(x_d1, x_d3);
		PACK_2;

		i += 4;
	}

	/* pure C implementation */
	{
		mlib_u32 *dp = (mlib_u32 *)pd;
		mlib_u32 *sp0 = (mlib_u32 *)ps1;
		mlib_u32 *sp1 = (mlib_u32 *)ps2;
		mlib_u32 *sp2 = (mlib_u32 *)ps3;
		mlib_u32 s0, s1, s2;

		for (; i <= (n - 2); i += 2) {
			s0 = (*sp0++);
			s1 = (*sp1++);
			s2 = (*sp2++);
			dp[2] = (s2 & ~0xFFFF) + (s1 >> 16);
			dp[1] = (s0 & ~0xFFFF) + (s2 & 0xFFFF);
			dp[0] = (s1 << 16) + (s0 & 0xFFFF);
			dp += 3;
		}

		if (i < n) {
			s0 = *((mlib_u16 *)sp0);
			s1 = *((mlib_u16 *)sp1);
			s2 = *((mlib_u16 *)sp2);
			dp[0] = (s1 << 16) + s0;
			((mlib_u16 *)dp)[2] = s2;
		}
	}

	return (MLIB_SUCCESS);
}

mlib_status
mlib_VideoColorMerge3_S16_naligned(
	mlib_s16 *colors,
	const mlib_s16 *color1,
	const mlib_s16 *color2,
	const mlib_s16 *color3,
	mlib_s32 n)
{
	mlib_s16 *ps1, *ps2, *ps3, *pd, *pout;
	mlib_s32 i;

	ps1 = (mlib_s16 *)color1;
	ps2 = (mlib_s16 *)color2;
	ps3 = (mlib_s16 *)color3;
	pd = colors;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i <= (n - 16); i += 8)	{
		__m128i x_s1, x_s2, x_s3, x_d1, x_d2, x_d3, x_d4, x_d;
		x_s1 = _mm_loadu_si128((__m128i *)ps1);
		ps1 += 8;
		x_s2 = _mm_loadu_si128((__m128i *)ps2);
		ps2 += 8;
		x_s3 = _mm_loadu_si128((__m128i *)ps3);
		ps3 += 8;

		x_d1 = _mm_unpacklo_epi16(x_s1, x_s2);
		x_d2 = _mm_unpackhi_epi16(x_s1, x_s2);
		x_d3 = _mm_unpacklo_epi16(x_s3, x_s3);
		x_d4 = _mm_unpackhi_epi16(x_s3, x_s3);

		x_d = _mm_unpacklo_epi32(x_d1, x_d3);
		PACK_1;

		x_d = _mm_unpackhi_epi32(x_d1, x_d3);
		PACK_1;

		x_d = _mm_unpacklo_epi32(x_d2, x_d4);
		PACK_1;

		x_d = _mm_unpackhi_epi32(x_d2, x_d4);
		PACK_1;
	}

	if (i <= (n - 8)) {
		__m128i x_s1, x_s2, x_s3, x_d1, x_d2, x_d3, x_d4, x_d;
		x_s1 = _mm_loadu_si128((__m128i *)ps1);
		ps1 += 8;
		x_s2 = _mm_loadu_si128((__m128i *)ps2);
		ps2 += 8;
		x_s3 = _mm_loadu_si128((__m128i *)ps3);
		ps3 += 8;

		x_d1 = _mm_unpacklo_epi16(x_s1, x_s2);
		x_d2 = _mm_unpackhi_epi16(x_s1, x_s2);
		x_d3 = _mm_unpacklo_epi16(x_s3, x_s3);
		x_d4 = _mm_unpackhi_epi16(x_s3, x_s3);

		x_d = _mm_unpacklo_epi32(x_d1, x_d3);
		PACK_1;

		x_d = _mm_unpackhi_epi32(x_d1, x_d3);
		PACK_1;

		x_d = _mm_unpacklo_epi32(x_d2, x_d4);
		PACK_1;

		x_d = _mm_unpackhi_epi32(x_d2, x_d4);
		PACK_2;

		i += 8;
	}

	if (i <= (n - 4)) {
		__m128i x_s1, x_s2, x_s3, x_d1, x_d2, x_d3, x_d4, x_d;
		x_s1 = _mm_loadl_epi64((__m128i *)ps1);
		ps1 += 4;
		x_s2 = _mm_loadl_epi64((__m128i *)ps2);
		ps2 += 4;
		x_s3 = _mm_loadl_epi64((__m128i *)ps3);
		ps3 += 4;

		x_d1 = _mm_unpacklo_epi16(x_s1, x_s2);
		x_d3 = _mm_unpacklo_epi16(x_s3, x_s3);

		x_d = _mm_unpacklo_epi32(x_d1, x_d3);
		PACK_1;

		x_d = _mm_unpackhi_epi32(x_d1, x_d3);
		PACK_2;

		i += 4;
	}

	/* pure C implementation */
	{
		mlib_u32 *dp = (mlib_u32 *)pd;
		mlib_u32 *sp0 = (mlib_u32 *)ps1;
		mlib_u32 *sp1 = (mlib_u32 *)ps2;
		mlib_u32 *sp2 = (mlib_u32 *)ps3;
		mlib_u32 s0, s1, s2;

		for (; i <= (n - 2); i += 2) {
			s0 = (*sp0++);
			s1 = (*sp1++);
			s2 = (*sp2++);
			dp[2] = (s2 & ~0xFFFF) + (s1 >> 16);
			dp[1] = (s0 & ~0xFFFF) + (s2 & 0xFFFF);
			dp[0] = (s1 << 16) + (s0 & 0xFFFF);
			dp += 3;
		}

		if (i < n) {
			s0 = *((mlib_u16 *)sp0);
			s1 = *((mlib_u16 *)sp1);
			s2 = *((mlib_u16 *)sp2);
			dp[0] = (s1 << 16) + s0;
			((mlib_u16 *)dp)[2] = s2;
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
