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
 * Copyright 2007 Sun Microsystems, Inc.  All rights reserved.
 * Use is subject to license terms.
 */

#pragma ident	"@(#)mlib_s_VideoColorMerge2.c	9.5	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_VideoColorMerge2 - merges two channels colors
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_VideoColorMerge2(mlib_u8       *colors,
 *                                        const mlib_u8 *color1,
 *                                        const mlib_u8 *color2,
 *                                        mlib_s32      n)
 *
 * ARGUMENTS
 *      colors    Pointer to colors multicomponent row, 8-byte aligned
 *      color1    Pointer to first color component row, 8-byte aligned
 *      color2    Pointer to second color component row, 8-byte aligned
 *      n         Length of the color1 array
 *
 * DESCRIPTION
 *
 *      colors[2*i]   = color1[i]
 *      colors[2*i+1] = color2[i]
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

#pragma weak mlib_VideoColorMerge2 = __mlib_VideoColorMerge2

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_VideoColorMerge2) mlib_VideoColorMerge2
	__attribute__((weak, alias("__mlib_VideoColorMerge2")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */
mlib_status
mlib_VideoColorMerge2_aligned(
	mlib_u8 *colors,
	const mlib_u8 *color1,
	const mlib_u8 *color2,
	mlib_s32 n);

mlib_status
mlib_VideoColorMerge2_naligned(
	mlib_u8 *colors,
	const mlib_u8 *color1,
	const mlib_u8 *color2,
	mlib_s32 n);

mlib_status
__mlib_VideoColorMerge2(
	mlib_u8 *colors,
	const mlib_u8 *color1,
	const mlib_u8 *color2,
	mlib_s32 n)
{
	if (0 == (0xf & ((mlib_addr)color1 |
		(mlib_addr)color2 | (mlib_addr)colors))) {
		return mlib_VideoColorMerge2_aligned(
				colors, color1, color2, n);
	} else {
		return mlib_VideoColorMerge2_naligned(
				colors, color1, color2, n);
	}
}

mlib_status
mlib_VideoColorMerge2_aligned(
	mlib_u8 *colors,
	const mlib_u8 *color1,
	const mlib_u8 *color2,
	mlib_s32 n)
{
	__m128i x_s1, x_s2, x_d;
	mlib_u8 *ps1, *ps2, *pd;
	mlib_s32 i;

	ps1 = (mlib_u8 *)color1;
	ps2 = (mlib_u8 *)color2;
	pd = colors;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i <= (n - 16); i += 16)	{
		x_s1 = _mm_load_si128((__m128i *)ps1);
		ps1 += 16;
		x_s2 = _mm_load_si128((__m128i *)ps2);
		ps2 += 16;
		x_d = _mm_unpacklo_epi8(x_s1, x_s2);
		_mm_store_si128((__m128i *)pd, x_d);
		pd += 16;
		x_d = _mm_unpackhi_epi8(x_s1, x_s2);
		_mm_store_si128((__m128i *)pd, x_d);
		pd += 16;
	}

	if (i <= (n - 8)) {
		x_s1 = _mm_loadl_epi64((__m128i *)ps1);
		ps1 += 8;
		x_s2 = _mm_loadl_epi64((__m128i *)ps2);
		ps2 += 8;
		x_d = _mm_unpacklo_epi8(x_s1, x_s2);
		_mm_store_si128((__m128i *)pd, x_d);
		pd += 16;
		i += 8;
	}

	/* pure C implementation */
	{
		mlib_u16 *dp = (mlib_u16 *)pd;
		mlib_u16 *sp0 = (mlib_u16 *)ps1;
		mlib_u16 *sp1 = (mlib_u16 *)ps2;
		mlib_s32 s0, s1;

		for (; i <= (n - 2); i += 2) {
			s0 = (*sp0++);
			s1 = (*sp1++);
			dp[1] = (s1 & ~0xff) | (s0 >> 8);
			dp[0] = (s1 << 8) | (s0 & 0xff);
			dp += 2;
		}

		if (i <= (n-1)) {
			dp[0] = (((mlib_u8 *)sp1)[0] << 8) |
				((mlib_u8 *)sp0)[0];
		}
	}

	return (MLIB_SUCCESS);
}

mlib_status
mlib_VideoColorMerge2_naligned(
	mlib_u8 *colors,
	const mlib_u8 *color1,
	const mlib_u8 *color2,
	mlib_s32 n)
{
	__m128i x_s1, x_s2, x_d;
	mlib_u8 *ps1, *ps2, *pd;
	mlib_s32 i;

	ps1 = (mlib_u8 *)color1;
	ps2 = (mlib_u8 *)color2;
	pd = colors;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i <= (n - 16); i += 16)	{
		x_s1 = _mm_loadu_si128((__m128i *)ps1);
		ps1 += 16;
		x_s2 = _mm_loadu_si128((__m128i *)ps2);
		ps2 += 16;
		x_d = _mm_unpacklo_epi8(x_s1, x_s2);
		_mm_storeu_si128((__m128i *)pd, x_d);
		pd += 16;
		x_d = _mm_unpackhi_epi8(x_s1, x_s2);
		_mm_storeu_si128((__m128i *)pd, x_d);
		pd += 16;
	}

	if (i <= (n - 8)) {
		x_s1 = _mm_loadl_epi64((__m128i *)ps1);
		ps1 += 8;
		x_s2 = _mm_loadl_epi64((__m128i *)ps2);
		ps2 += 8;
		x_d = _mm_unpacklo_epi8(x_s1, x_s2);
		_mm_storeu_si128((__m128i *)pd, x_d);
		pd += 16;
		i += 8;
	}

	/* pure C implementation */
	{
		mlib_u16 *dp = (mlib_u16 *)pd;
		mlib_u16 *sp0 = (mlib_u16 *)ps1;
		mlib_u16 *sp1 = (mlib_u16 *)ps2;
		mlib_s32 s0, s1;

		for (; i <= (n - 2); i += 2) {
			s0 = (*sp0++);
			s1 = (*sp1++);
			dp[1] = (s1 & ~0xff) | (s0 >> 8);
			dp[0] = (s1 << 8) | (s0 & 0xff);
			dp += 2;
		}

		if (i <= (n-1)) {
			dp[0] = (((mlib_u8 *)sp1)[0] << 8) |
				((mlib_u8 *)sp0)[0];
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
