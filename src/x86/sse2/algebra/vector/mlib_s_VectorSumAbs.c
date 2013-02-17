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

#pragma ident	"@(#)mlib_s_VectorSumAbs.c	9.6	07/11/05 SMI"

/*
 * FUNCTIONS
 *      mlib_VectorSumAbs_U8 -  sum of the absolute values
 *                              of unsigned 8-bit format vector
 *      mlib_VectorSumAbs_S8 -  sum of the absolute values
 *                              of signed 8-bit format vector
 *      mlib_VectorSumAbs_S16 - sum of the absolute values
 *                              of signed 16-bit format vector
 *      mlib_VectorSumAbs_S32 - sum of the absolute values
 *                              of signed 32-bit format vector
 *
 * SYNOPSIS
 *      mlib_status mlib_VectorSumAbs_S16_Sat(mlib_d64       * z,
 *                                            const mlib_s16 * x,
 *                                            mlib_s32       n);
 *      mlib_status mlib_VectorSumAbs_S32_Sat(mlib_d64       * z,
 *                                            const mlib_s32 * x,
 *                                            mlib_s32       n);
 *      mlib_status mlib_VectorSumAbs_S8_Sat(mlib_d64      * z,
 *                                           const mlib_s8 * x,
 *                                           mlib_s32      n);
 *      mlib_status mlib_VectorSumAbs_U8_Sat(mlib_d64      * z,
 *                                           const mlib_u8 * x,
 *                                           mlib_s32      n);
 *
 * ARGUMENTS
 *      z    pointer to the sum of the absolute values of the vector
 *      x    pointer to the first element of the input vector
 *      n    number of elements in the vector
 *
 * DESCRIPTION
 *          n -1
 *      z = SUM |x[i]|
 *          i = 0
 */

#include <mlib_algebra.h>
#include <mlib_AlgebraUtil.h>

#ifdef __SUNPRO_C
#include <sunmedia_intrin.h>
#else
#include <emmintrin.h>
#endif

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_VectorSumAbs_S16_Sat = __mlib_VectorSumAbs_S16_Sat
#pragma weak mlib_VectorSumAbs_S8_Sat = __mlib_VectorSumAbs_S8_Sat
#pragma weak mlib_VectorSumAbs_U8_Sat = __mlib_VectorSumAbs_U8_Sat
#pragma weak mlib_VectorSumAbs_S32_Sat = __mlib_VectorSumAbs_S32_Sat

#elif defined(__GNUC__)

__typeof__(__mlib_VectorSumAbs_S16_Sat) mlib_VectorSumAbs_S16_Sat
	__attribute__((weak, alias("__mlib_VectorSumAbs_S16_Sat")));
__typeof__(__mlib_VectorSumAbs_S8_Sat) mlib_VectorSumAbs_S8_Sat
	__attribute__((weak, alias("__mlib_VectorSumAbs_S8_Sat")));
__typeof__(__mlib_VectorSumAbs_U8_Sat) mlib_VectorSumAbs_U8_Sat
	__attribute__((weak, alias("__mlib_VectorSumAbs_U8_Sat")));
__typeof__(__mlib_VectorSumAbs_S32_Sat) mlib_VectorSumAbs_S32_Sat
	__attribute__((weak, alias("__mlib_VectorSumAbs_S32_Sat")));

#else

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	ABS_VALUE(x)	(((x) ^ ((x) >> 31)) - ((x) >> 31))

/* ****************************************************************** */

mlib_status
__mlib_VectorSumAbs_U8_Sat(
	mlib_d64 *z,
	const mlib_u8 *x,
	mlib_s32 n)
{
	mlib_s32 i, nstep, n1, n2, n3, sum = 0;
	mlib_u8 *px = (mlib_u8 *)x;
	__m128i zero, xbuf, zbuf, mbuf;

	if (n <= 0)
		return (MLIB_FAILURE);

	zero = _mm_setzero_si128();
	zbuf = zero;

	nstep = 16 / sizeof (mlib_u8);
	n1 = ((16 - ((mlib_addr)x & 15)) & 15) / sizeof (mlib_u8);
	n2 = (n - n1) / nstep;
	n3 = n - n1 - n2 * nstep;

	if (n2 < 1) {
		for (i = 0; i < n; i++)
			sum += *px++;
		*z = sum;
	} else {
		mlib_d64 dsum;
		long long pz[2];

		for (i = 0; i < n1; i++)
			sum += *px++;
		for (i = 0; i < n2; i++) {
			xbuf = _mm_load_si128((__m128i *)px);
			mbuf = _mm_sad_epu8(xbuf, zero);
			zbuf = _mm_add_epi64(zbuf, mbuf);
			px += nstep;
		}
		for (i = 0; i < n3; i++)
			sum += *px++;

		dsum = sum;
		_mm_storeu_si128((__m128i *)pz, zbuf);
		dsum += pz[0];
		dsum += pz[1];
		*z = dsum;
	}
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VectorSumAbs_S8_Sat(
	mlib_d64 *z,
	const mlib_s8 *x,
	mlib_s32 n)
{
	mlib_s32 i, nstep, n1, n2, n3, xval, sum = 0;
	mlib_s8 *px = (mlib_s8 *)x;
	__m128i zero, xbuf, zbuf, mbuf, mext;

	if (n <= 0)
		return (MLIB_FAILURE);

	zero = _mm_setzero_si128();
	zbuf = zero;

	nstep = 16 / sizeof (mlib_s8);
	n1 = ((16 - ((mlib_addr)x & 15)) & 15) / sizeof (mlib_s8);
	n2 = (n - n1) / nstep;
	n3 = n - n1 - n2 * nstep;

	if (n2 < 1) {
		for (i = 0; i < n; i++) {
			xval = *px++;
			sum += ABS_VALUE(xval);
		}
		*z = sum;
	} else {
		mlib_d64 dsum;
		long long pz[2];

		for (i = 0; i < n1; i++) {
			xval = *px++;
			sum += ABS_VALUE(xval);
		}

		for (i = 0; i < n2; i++) {
			xbuf = _mm_load_si128((__m128i *)px);
			mext = _mm_cmpgt_epi8(zero, xbuf);
			xbuf = _mm_xor_si128(xbuf, mext);
			xbuf = _mm_sub_epi8(xbuf, mext);
			mbuf = _mm_sad_epu8(xbuf, zero);
			zbuf = _mm_add_epi64(zbuf, mbuf);
			px += nstep;
		}
		for (i = 0; i < n3; i++) {
			xval = *px++;
			sum += ABS_VALUE(xval);
		}

		dsum = sum;
		_mm_storeu_si128((__m128i *)pz, zbuf);
		dsum += pz[0];
		dsum += pz[1];
		*z = dsum;
	}
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VectorSumAbs_S16_Sat(
	mlib_d64 *z,
	const mlib_s16 *x,
	mlib_s32 n)
{
	mlib_s32 i, nstep, n1, n2, n3, xval, sum = 0;
	mlib_s16 *px = (mlib_s16 *)x;
	__m128i zero, xbuf, zbuf32, zbuf64, xlo, xhi, mext;

	if (n <= 0)
		return (MLIB_FAILURE);

	zero = _mm_setzero_si128();
	zbuf64 = zero;

	nstep = 16 / sizeof (mlib_s16);
	n1 = ((16 - ((mlib_addr)x & 15)) & 15) / sizeof (mlib_s16);
	n2 = (n - n1) / nstep;
	n3 = n - n1 - n2 * nstep;

	if (n2 < 1) {
		for (i = 0; i < n; i++) {
			xval = *px++;
			sum += ABS_VALUE(xval);
		}
		*z = sum;
	} else {
		mlib_s32 nblock = n2 >> 12;
		mlib_s32 tail = n2 & 4095;
		mlib_s32 k;
		mlib_d64 dsum;
		long long pz[2];

		for (i = 0; i < n1; i++) {
			xval = *px++;
			sum += ABS_VALUE(xval);
		}
		for (k = 0; k < nblock; k++) {
			zbuf32 = zero;
			for (i = 0; i < 4096; i++) {
				xbuf = _mm_load_si128((__m128i *)px);
				mext = _mm_srai_epi16(xbuf, 15);
				xbuf = _mm_xor_si128(xbuf, mext);
				xbuf = _mm_sub_epi16(xbuf, mext);
				xlo = _mm_unpacklo_epi16(xbuf, zero);
				xhi = _mm_unpackhi_epi16(xbuf, zero);
				zbuf32 = _mm_add_epi32(zbuf32, xlo);
				zbuf32 = _mm_add_epi32(zbuf32, xhi);
				px += nstep;
			}
			xlo = _mm_unpacklo_epi32(zbuf32, zero);
			xhi = _mm_unpackhi_epi32(zbuf32, zero);
			zbuf64 = _mm_add_epi64(zbuf64, xlo);
			zbuf64 = _mm_add_epi64(zbuf64, xhi);
		}
		zbuf32 = zero;
		for (i = 0; i < tail; i++) {
			xbuf = _mm_load_si128((__m128i *)px);
			mext = _mm_srai_epi16(xbuf, 15);
			xbuf = _mm_xor_si128(xbuf, mext);
			xbuf = _mm_sub_epi16(xbuf, mext);
			xlo = _mm_unpacklo_epi16(xbuf, zero);
			xhi = _mm_unpackhi_epi16(xbuf, zero);
			zbuf32 = _mm_add_epi32(zbuf32, xlo);
			zbuf32 = _mm_add_epi32(zbuf32, xhi);
			px += nstep;
		}
		xlo = _mm_unpacklo_epi32(zbuf32, zero);
		xhi = _mm_unpackhi_epi32(zbuf32, zero);
		zbuf64 = _mm_add_epi64(zbuf64, xlo);
		zbuf64 = _mm_add_epi64(zbuf64, xhi);
		for (i = 0; i < n3; i++) {
			xval = *px++;
			sum += ABS_VALUE(xval);
		}

		dsum = sum;
		_mm_storeu_si128((__m128i *)pz, zbuf64);
		dsum += pz[0];
		dsum += pz[1];
		*z = dsum;
	}
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VectorSumAbs_S32_Sat(
	mlib_d64 *z,
	const mlib_s32 *x,
	mlib_s32 n)
{
	mlib_s32 i, nstep, n1, n2, n3, xval;
	mlib_s32 *px = (mlib_s32 *)x;
	long long sum = 0;
	__m128i zero, xbuf, zbuf, xlo, xhi, mext;

	if (n <= 0)
		return (MLIB_FAILURE);

	zero = _mm_setzero_si128();
	zbuf = zero;

	nstep = 16 / sizeof (mlib_s32);
	n1 = ((16 - ((mlib_addr)x & 15)) & 15) / sizeof (mlib_s32);
	n2 = (n - n1) / nstep;
	n3 = n - n1 - n2 * nstep;

	if (n2 < 1) {
		for (i = 0; i < n; i++) {
			xval = *px++;
			sum += (mlib_u32)ABS_VALUE(xval);
		}
		*z = sum;
	} else {
		mlib_d64 dsum;
		long long pz[2];

		for (i = 0; i < n1; i++) {
			xval = *px++;
			sum += (mlib_u32)ABS_VALUE(xval);
		}
		for (i = 0; i < n2; i++) {
			xbuf = _mm_load_si128((__m128i *)px);
			mext = _mm_srai_epi32(xbuf, 31);
			xbuf = _mm_xor_si128(xbuf, mext);
			xbuf = _mm_sub_epi32(xbuf, mext);
			xlo = _mm_unpacklo_epi32(xbuf, zero);
			xhi = _mm_unpackhi_epi32(xbuf, zero);
			zbuf = _mm_add_epi64(zbuf, xlo);
			zbuf = _mm_add_epi64(zbuf, xhi);
			px += nstep;
		}
		for (i = 0; i < n3; i++) {
			xval = *px++;
			sum += (mlib_u32)ABS_VALUE(xval);
		}

		dsum = sum;
		_mm_storeu_si128((__m128i *)pz, zbuf);
		dsum += pz[0];
		dsum += pz[1];
		*z = dsum;
	}
	return (MLIB_SUCCESS);
}

/* *********************************************************** */
