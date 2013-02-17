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

#ifndef	_MLIB_S_IMAGEREPLACECOLOR_H
#define	_MLIB_S_IMAGEREPLACECOLOR_H

#pragma ident	"@(#)mlib_s_ImageReplaceColor.h	9.3	07/11/05 SMI"

/*
 * FUNCTION
 *      Internal function for any image operation
 *      with array of __m128i parameters.
 *
 *   File contains macro function implementation for any type:
 *     FUNC_NAME     - name of internal function.
 *     DTYPE         - data type of image.
 *     LOAD_CONST    - load from cnst array
 *     PROCESS_CH(N) - macro to process source s##N of __m128i type and
 *                     store result by dp pointer
 */

#ifdef __cplusplus
extern "C" {
#endif

#include <mlib_types.h>
#include <mlib_status.h>
#ifdef  __SUNPRO_C
#include <sunmedia_intrin.h>
#else
#include <emmintrin.h>
#endif

/* *********************************************************** */

#define	REPLACE_STORE(dp, cmask, aa, bb)                       \
	dd =                                                   \
		_mm_or_si128(                                  \
		_mm_andnot_si128(cmask, aa),                   \
		_mm_and_si128(cmask, bb));                     \
	_mm_storeu_si128(dp, dd);                              \
	dp++

/* *********************************************************** */
#define	REPLACE_STORE_EDGE(dp, cmask, emask, aa, bb)           \
	dd =                                                   \
		_mm_or_si128(                                  \
		_mm_andnot_si128(cmask, aa),                   \
		_mm_and_si128(cmask, bb));                     \
	_mm_maskmoveu_si128(dd, emask, (char *)dp)

/* *********************************************************** */


#define	CLAMP_U8(x)                                            \
	(((x) > MLIB_U8_MAX) ? MLIB_U8_MAX : (((x) <           \
	    MLIB_U8_MIN) ? MLIB_U8_MIN : (x)))

/* *********************************************************** */

#define	CLAMP_S16(x)                                           \
	(((x) > MLIB_S16_MAX) ? MLIB_S16_MAX : (((x) <         \
	    MLIB_S16_MIN) ? MLIB_S16_MIN : (x)))

/* *********************************************************** */

#define	CLAMP_U16(x)                                           \
	(((x) > MLIB_U16_MAX) ? MLIB_U16_MAX : (((x) <         \
	    MLIB_U16_MIN) ? MLIB_U16_MIN : (x)))

#ifdef __cplusplus
}
#endif

#endif /* _MLIB_S_IMAGEREPLACECOLOR_H */

/* *********************************************************** */

#ifdef FUNC_NAME

/* *********************************************************** */

static mlib_status
FUNC_NAME(_124) (
    const DTYPE *sl,
    DTYPE *dl,
    mlib_s32 sstride,
    mlib_s32 dstride,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s32 nchan,
    const __m128i *cnst)
{
	__m128i s0, dd;
	__m128i cmask, emask;
	__m128i r0, r1, r2, c0, c1, c2;
	mlib_s32 i, j, off;

	LOAD_CONST(0, 0);

	width = nchan * width * sizeof (DTYPE);

	emask = _mm_load_si128(
			((__m128i *)mlib_mask128i_arr) + OFF_SET);
#ifdef  __SUNPRO_C
#pragma pipeloop(0)
#endif
	for (i = 0; i < height; i++) {
		DTYPE *dend = (DTYPE *) ((mlib_u8 *)dl + width) - 1;
		__m128i *sp = (__m128i *)sl;
		__m128i *dp = (__m128i *)dl;

#ifdef  __SUNPRO_C
#pragma pipeloop(0)
#endif
		for (j = 0; j <= (width - 16); j += 16) {
			s0 = _mm_loadu_si128(sp);
			PROCESS_CH(0);
			sp++;
		}

		if ((DTYPE *) dp <= dend) {
			s0 = _mm_loadu_si128(sp);
			COMPARE_CH(0);
			REPLACE_STORE_EDGE(dp, cmask, emask, s0, r0);
		}

		sl += sstride;
		dl += dstride;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

static mlib_status
FUNC_NAME(_3) (
    const DTYPE *sl,
    DTYPE *dl,
    mlib_s32 sstride,
    mlib_s32 dstride,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s32 nchan,
    const __m128i *cnst)
{
	__m128i s0, s1, s2, sx, dd;
	__m128i cmask, emask;
	__m128i r0, r1, r2, c0, c1, c2;
	mlib_s32 i, j, off;

	width = nchan * width * sizeof (DTYPE);
	emask = _mm_load_si128(
			((__m128i *)mlib_mask128i_arr) + OFF_SET);
#ifdef  __SUNPRO_C
#pragma pipeloop(0)
#endif
	for (i = 0; i < height; i++) {
		__m128i *sp;
		__m128i *dp;

		LOAD_CONST(0, 0);
		LOAD_CONST(1, 1);
		LOAD_CONST(2, 2);

		sp = (__m128i *)sl;
		dp = (__m128i *)dl;
#ifdef  __SUNPRO_C
#pragma pipeloop(0)
#endif
		for (j = width; j > 47; j -= 48) {
			s0 = _mm_loadu_si128(sp);
			sp++;
			s1 = _mm_loadu_si128(sp);
			sp++;
			s2 = _mm_loadu_si128(sp);
			sp++;
			PROCESS_CH(0);
			PROCESS_CH(1);
			PROCESS_CH(2);
		}

		if (j > 0) {
			s0 = _mm_loadu_si128(sp);
			sp++;
			COMPARE_CH(0);
			if (j > 15) {
				REPLACE_STORE(dp, cmask, s0, r0);
				s1 = _mm_loadu_si128(sp);
				sp++;
				COMPARE_CH(1);
				s0 = s1; r0 = r1;
				if (j > 31) {
					REPLACE_STORE(dp, cmask, s0, r0);
					s2 = _mm_loadu_si128(sp);
					COMPARE_CH(2);
					s0 = s2; r0 = r2;
				}
			}

			REPLACE_STORE_EDGE(dp, cmask, emask, s0, r0);
		}

		sl += sstride;
		dl += dstride;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#endif

/* *********************************************************** */


#undef  FUNC_NAME
#undef  DTYPE
#undef  PROCESS_CH
