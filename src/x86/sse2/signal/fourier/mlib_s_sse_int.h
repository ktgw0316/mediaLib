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
#ifndef	_MLIB_S_SSE_INT_H
#define	_MLIB_S_SSE_INT_H

#pragma ident	"@(#)mlib_s_sse_int.h	9.5	07/11/05 SMI"

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>

#ifdef	__SUNPRO_C
#include <sunmedia_intrin.h>
#else
#include <emmintrin.h>
#endif

#define	_mm_mulhi_epi16_vis(a, b)	_mm_mulhi_epi16(a, b)

/* rounding with _mm_adds_epi16 is introduced here to improve the precision */
#define	DEF_ADDER(scale)                                               \
	__m128i adder_##scale = _mm_set1_epi16(1 << (scale - 1));
#define	REDEF_ADDER(scale)                                             \
	adder_##scale = _mm_set1_epi16(1 << (scale - 1));
#define	_mm_fmul8x16_vis(a, scale)                                     \
	_mm_srai_epi16(_mm_adds_epi16(a, adder_##scale), scale)

#if 0
static __m128i _mm_mulhi_epi16_vis(__m128i a, __m128i b)
{
	__m128i ret;
	int i, ta, tb, tc, td;

#if 0
	ret = _mm_mulhi_epi16(a, b);
#else
	/*
	 * to get same precision with following VIS code
	 * #define MULT16(d0, d1)                             \
	 * vis_fpadd16(vis_fmul8sux16(d0, d1), vis_fmul8ulx16(d0, d1))
	 */
	__m128i tmpb128;
	mlib_s16 *tmpa = (mlib_s16 *)&ret;
	mlib_s16 *tmpb = (mlib_s16 *)&tmpb128;
	_mm_storeu_si128((__m128i *)tmpa, a);
	_mm_storeu_si128((__m128i *)tmpb, b);
	for (i = 0; i < 8; i++) {
		ta = tmpa[i];
		tb = tmpb[i];
		tc = (((ta >> 8) * tb) + (1<<7)) >> 8;
		td = (((ta & 0xff) * tb) + (1<<15)) >> 16;
		tmpa[i] = tc + td;
	}
#endif

	return (ret);
}
#endif

#if 0
static __m128i _mm_fmul8x16_vis(__m128i a, int scale)
{
	__m128i ret;
	int i, ta, tb, tc, td;

#if 0
	ret = _mm_srai_epi16(a, scale);
#else
	/* to be more accurate (the precision that VIS can get) */
	__m128i tmpa128;
	mlib_s16 *tmpa = (mlib_s16 *)&tmpa128;
	mlib_s16 *tmpb = (mlib_s16 *)&ret;
	_mm_storeu_si128((__m128i *)tmpa, a);
	for (i = 0; i < 8; i++) {
		ta = tmpa[i];
		tmpb[i] = (ta + (1 << (scale - 1))) >> scale;
	}
#endif

	return (ret);
}
#endif

#ifdef __cplusplus
}
#endif

#endif /* _MLIB_S_SSE_INT_H */
