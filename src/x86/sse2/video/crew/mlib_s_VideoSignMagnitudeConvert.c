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

#pragma ident	"@(#)mlib_s_VideoSignMagnitudeConvert.c	9.4	07/11/05 SMI"

/*
 * FUNCTIONS
 *      mlib_VideoSignMagnitudeConvert -
 *              Conversion from standard 2's Complement
 *              mlib_s32 integer representation
 *              to Sign Magnitude representation and back.
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_VideoSignMagnitudeConvert_S16(mlib_s16 *srcdst,
 *                                                     mlib_s32 n);
 *      mlib_status mlib_VideoSignMagnitudeConvert_S16_S16(mlib_s16       *dst,
 *                                                         const mlib_s16 *src,
 *                                                         mlib_s32       n);
 *      mlib_status mlib_VideoSignMagnitudeConvert_S32(mlib_s32 *srcdst,
 *                                                     mlib_s32 n);
 *      mlib_status mlib_VideoSignMagnitudeConvert_S32_S32(mlib_s32       *dst,
 *                                                         const mlib_s32 *src,
 *                                                         mlib_s32       n);
 * ARGUMENT
 *      dst     pointer to output data array
 *      src     pointer to input data array
 *      srcdst  pointer to input and output data array
 *      n       array size
 *
 * DESCRIPTION
 *
 * Conversion from standard 2's Complement signed integer representation
 * to Sign Magnitude representation and back.
 *
 *  Example for 4-bit signed integer representation :
 *
 *  Decimal    2's       Sign
 *         Complement  Magnitude
 *   -8       1000      N/A
 *   -7       1001      1111
 *   -6       1010      1110
 *   -5       1011      1101
 *   -4       1100      1100
 *   -3       1101      1011
 *   -2       1110      1010
 *   -1       1111      1001
 *    0       0000      0000
 *    1       0001      0001
 *    2       0010      0010
 *    3       0011      0011
 *    4       0100      0100
 *    5       0101      0101
 *    6       0110      0110
 *    7       0111      0111
 *
 */

#include <mlib_video.h>
#include <mlib_video.h>
#ifdef __SUNPRO_C
#include <sunmedia_intrin.h>
#else
#include <emmintrin.h>
#endif

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_VideoSignMagnitudeConvert_S16 = \
	__mlib_VideoSignMagnitudeConvert_S16
#pragma weak mlib_VideoSignMagnitudeConvert_S16_S16 = \
	__mlib_VideoSignMagnitudeConvert_S16_S16
#pragma weak mlib_VideoSignMagnitudeConvert_S32 = \
	__mlib_VideoSignMagnitudeConvert_S32
#pragma weak mlib_VideoSignMagnitudeConvert_S32_S32 = \
	__mlib_VideoSignMagnitudeConvert_S32_S32

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_VideoSignMagnitudeConvert_S16)
	mlib_VideoSignMagnitudeConvert_S16
	__attribute__((weak, alias("__mlib_VideoSignMagnitudeConvert_S16")));
__typeof__(__mlib_VideoSignMagnitudeConvert_S16_S16)
	mlib_VideoSignMagnitudeConvert_S16_S16
	__attribute__((weak,
	alias("__mlib_VideoSignMagnitudeConvert_S16_S16")));
__typeof__(__mlib_VideoSignMagnitudeConvert_S32)
	mlib_VideoSignMagnitudeConvert_S32
	__attribute__((weak, alias("__mlib_VideoSignMagnitudeConvert_S32")));
__typeof__(__mlib_VideoSignMagnitudeConvert_S32_S32)
	mlib_VideoSignMagnitudeConvert_S32_S32
	__attribute__((weak,
	alias("__mlib_VideoSignMagnitudeConvert_S32_S32")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

mlib_status
__mlib_VideoSignMagnitudeConvert_S16(
	mlib_s16 *srcdst,
	mlib_s32 n)
{
	return (__mlib_VideoSignMagnitudeConvert_S16_S16(srcdst, srcdst, n));
}

/* *********************************************************** */

mlib_status
__mlib_VideoSignMagnitudeConvert_S32(
	mlib_s32 *srcdst,
	mlib_s32 n)
{
	return (__mlib_VideoSignMagnitudeConvert_S32_S32(srcdst, srcdst, n));
}

/* *********************************************************** */

mlib_status
mlib_VideoSignMagnitudeConvert_S16_S16_aligned(
	mlib_s16 *dst,
	const mlib_s16 *src,
	mlib_s32 n)
{
	mlib_s32 i, nval1, nval2, nmask;
	__m128i *psrc, *pdst, out, val1, val2, mask, temp;
	const __m128i const8 = _mm_set1_epi32(0x80008000);

	psrc = (__m128i*)src;
	pdst = (__m128i*)dst;
	for (i = 0; i < (n >>3); i++) {
		val1 = _mm_load_si128(&psrc[i]);
		val2 = _mm_sub_epi16(const8, val1);
		mask = _mm_srai_epi16(val1, 15);
		temp = _mm_and_si128(mask, val2);
		out = _mm_andnot_si128(mask, val1);
		out = _mm_add_epi16(temp, out);
		_mm_store_si128(&pdst[i], out);
	}

	for (i <<= 3; i < n; i++) {
		nval1 = src[i];
		nval2 = 0x8000 - nval1;
		nmask = (nval1 >> 15);
		dst[i] = (mlib_s16)((nval1 & ~nmask) + (nmask & nval2));
	}

	return (MLIB_SUCCESS);
}

mlib_status
mlib_VideoSignMagnitudeConvert_S16_S16_naligned(
	mlib_s16 *dst,
	const mlib_s16 *src,
	mlib_s32 n)
{
	mlib_s32 i, nval1, nval2, nmask;
	__m128i *psrc, *pdst, out, val1, val2, mask, temp;
	const __m128i const8 = _mm_set1_epi32(0x80008000);

	psrc = (__m128i*)src;
	pdst = (__m128i*)dst;
	for (i = 0; i < (n >>3); i++) {
		val1 = _mm_loadu_si128(&psrc[i]);
		val2 = _mm_sub_epi16(const8, val1);
		mask = _mm_srai_epi16(val1, 15);
		temp = _mm_and_si128(mask, val2);
		out = _mm_andnot_si128(mask, val1);
		out = _mm_add_epi16(temp, out);
		_mm_storeu_si128(&pdst[i], out);
	}

	for (i <<= 3; i < n; i++) {
		nval1 = src[i];
		nval2 = 0x8000 - nval1;
		nmask = (nval1 >> 15);
		dst[i] = (mlib_s16)((nval1 & ~nmask) + (nmask & nval2));
	}

	return (MLIB_SUCCESS);
}

mlib_status
__mlib_VideoSignMagnitudeConvert_S16_S16(
	mlib_s16 *dst,
	const mlib_s16 *src,
	mlib_s32 n)
{
	if (n <= 0)
		return (MLIB_FAILURE);

	if (0 == (0xf & ((mlib_addr)src | (mlib_addr)dst))) {
		return mlib_VideoSignMagnitudeConvert_S16_S16_aligned(
				dst, src, n);
	} else {
		return mlib_VideoSignMagnitudeConvert_S16_S16_naligned(
				dst, src, n);
	}
}

/* *********************************************************** */
mlib_status
mlib_VideoSignMagnitudeConvert_S32_S32_aligned(
	mlib_s32 *dst,
	const mlib_s32 *src,
	mlib_s32 n)
{
	mlib_s32 i, nval1, nval2, nmask;
	__m128i *psrc, *pdst, out, val1, val2, mask, temp;
	const __m128i const8 = _mm_set1_epi32(0x80000000);

	psrc = (__m128i*)src;
	pdst = (__m128i*)dst;
	for (i = 0; i < (n >>2); i++) {
		val1 = _mm_load_si128(&psrc[i]);
		val2 = _mm_sub_epi32(const8, val1);
		mask = _mm_srai_epi32(val1, 31);
		temp = _mm_and_si128(mask, val2);
		out = _mm_andnot_si128(mask, val1);
		out = _mm_add_epi32(temp, out);
		_mm_store_si128(&pdst[i], out);
	}

	for (i <<= 2; i < n; i++) {
		nval1 = src[i];
		nval2 = 0x80000000 - nval1;
		nmask = (nval1 >> 31);
		dst[i] = (nval1 & ~nmask) + (nmask & nval2);
	}

	return (MLIB_SUCCESS);
}

mlib_status
mlib_VideoSignMagnitudeConvert_S32_S32_naligned(
	mlib_s32 *dst,
	const mlib_s32 *src,
	mlib_s32 n)
{
	mlib_s32 i, nval1, nval2, nmask;
	__m128i *psrc, *pdst, out, val1, val2, mask, temp;
	const __m128i const8 = _mm_set1_epi32(0x80000000);

	psrc = (__m128i*)src;
	pdst = (__m128i*)dst;
	for (i = 0; i < (n >>2); i++) {
		val1 = _mm_loadu_si128(&psrc[i]);
		val2 = _mm_sub_epi32(const8, val1);
		mask = _mm_srai_epi32(val1, 31);
		temp = _mm_and_si128(mask, val2);
		out = _mm_andnot_si128(mask, val1);
		out = _mm_add_epi32(temp, out);
		_mm_storeu_si128(&pdst[i], out);
	}

	for (i <<= 2; i < n; i++) {
		nval1 = src[i];
		nval2 = 0x80000000 - nval1;
		nmask = (nval1 >> 31);
		dst[i] = (nval1 & ~nmask) + (nmask & nval2);
	}

	return (MLIB_SUCCESS);
}


mlib_status
__mlib_VideoSignMagnitudeConvert_S32_S32(
	mlib_s32 *dst,
	const mlib_s32 *src,
	mlib_s32 n)
{
	if (n <= 0)
		return (MLIB_FAILURE);

	if (0 == (0xf & ((mlib_addr)src | (mlib_addr)dst))) {
		return mlib_VideoSignMagnitudeConvert_S32_S32_aligned(
				dst, src, n);
	} else {
		return mlib_VideoSignMagnitudeConvert_S32_S32_naligned(
				dst, src, n);
	}
}

/* *********************************************************** */
