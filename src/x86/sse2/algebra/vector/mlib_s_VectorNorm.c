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

#pragma ident	"@(#)mlib_s_VectorNorm.c	9.3	07/11/05 SMI"

/*
 * FUNCTIONS
 *      mlib_VectorNorm_U8_Sat  - unsigned 8-bit format vector norm
 *      mlib_VectorNorm_S8_Sat  - signed   8-bit format vector norm
 *      mlib_VectorNorm_S16_Sat - 16-bit format vector norm
 *      mlib_VectorNorm_S32_Sat - 32-bit format vector norm
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_VectorNorm_U8_Sat(mlib_d64      *z,
 *                                         const mlib_u8 *x,
 *                                         mlib_s32      n);
 *      mlib_status mlib_VectorNorm_S8_Sat(mlib_d64      *z,
 *                                         const mlib_s8 *x,
 *                                         mlib_s32      n);
 *      mlib_status mlib_VectorNorm_S16_Sat(mlib_d64       *z,
 *                                          const mlib_s16 *x,
 *                                          mlib_s32       n);
 *      mlib_status mlib_VectorNorm_S32_Sat(mlib_d64       *z,
 *                                          const mlib_s32 *x,
 *                                          mlib_s32       n);
 *
 * ARGUMENTS
 *      z    pointer to the norm of the vector
 *      x    pointer to the first element of the input vector
 *      n    number of elements in the vectors
 *
 * DESCRIPTION
 *                 n
 *      z = sqrt( SUM(x[i] * x[i]) )
 *                i = 1
 */

#include <mlib_algebra.h>
#include <mlib_SysMath.h>

#ifdef __SUNPRO_C
#include <sunmedia_intrin.h>
#else
#include <emmintrin.h>
#endif

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_VectorNorm_S32_Sat = __mlib_VectorNorm_S32_Sat
#pragma weak mlib_VectorNorm_S16_Sat = __mlib_VectorNorm_S16_Sat
#pragma weak mlib_VectorNorm_S8_Sat = __mlib_VectorNorm_S8_Sat
#pragma weak mlib_VectorNorm_U8_Sat = __mlib_VectorNorm_U8_Sat

#elif defined(__GNUC__)

__typeof__(__mlib_VectorNorm_S32_Sat) mlib_VectorNorm_S32_Sat
	__attribute__((weak, alias("__mlib_VectorNorm_S32_Sat")));
__typeof__(__mlib_VectorNorm_S16_Sat) mlib_VectorNorm_S16_Sat
	__attribute__((weak, alias("__mlib_VectorNorm_S16_Sat")));
__typeof__(__mlib_VectorNorm_S8_Sat) mlib_VectorNorm_S8_Sat
	__attribute__((weak, alias("__mlib_VectorNorm_S8_Sat")));
__typeof__(__mlib_VectorNorm_U8_Sat) mlib_VectorNorm_U8_Sat
	__attribute__((weak, alias("__mlib_VectorNorm_U8_Sat")));

#else

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

mlib_status
__mlib_VectorNorm_U8_Sat(
	mlib_d64 *z,
	const mlib_u8 *x,
	mlib_s32 n)
{
	if (n <= 0)
		return (MLIB_FAILURE);

	mlib_s32 i, nstep, n1, n2, n3, sum = 0;
	const mlib_u8 *px = x;
	__m128i zero, xbuf, zbuf32, zbuf64, buf1, buf2;
	zero = _mm_setzero_si128();
	zbuf64 = zero;

	nstep = 16 / sizeof (mlib_u8);
	n1 = ((16 - ((mlib_addr)x & 15)) & 15) / sizeof (mlib_u8);
	n2 = (n - n1) / nstep;
	n3 = n - n1 - n2 * nstep;

	if (n2 > 0) {
		for (i = 0; i < n1; i++) {
			sum += (mlib_s32)(*px) * (*px);
			px++;
		}

		mlib_s32 nblock = n2 >> 12;
		mlib_s32 tail = n2 & 4095;
		mlib_s32 k;
		for (k = 0; k < nblock; k++) {
			zbuf32 = zero;
			for (i = 0; i < 4096; i++) {
				xbuf = _mm_load_si128((__m128i *)px);
				buf1 = _mm_unpacklo_epi8(xbuf, zero);
				buf2 = _mm_unpackhi_epi8(xbuf, zero);
				buf1 = _mm_madd_epi16(buf1, buf1);
				buf2 = _mm_madd_epi16(buf2, buf2);
				zbuf32 = _mm_add_epi32(zbuf32, buf1);
				zbuf32 = _mm_add_epi32(zbuf32, buf2);
				px += nstep;
			}
			buf1 = _mm_unpacklo_epi32(zbuf32, zero);
			buf2 = _mm_unpackhi_epi32(zbuf32, zero);
			zbuf64 = _mm_add_epi64(zbuf64, buf1);
			zbuf64 = _mm_add_epi64(zbuf64, buf2);
		}
		zbuf32 = zero;
		for (i = 0; i < tail; i++) {
			xbuf = _mm_load_si128((__m128i *)px);
			buf1 = _mm_unpacklo_epi8(xbuf, zero);
			buf2 = _mm_unpackhi_epi8(xbuf, zero);
			buf1 = _mm_madd_epi16(buf1, buf1);
			buf2 = _mm_madd_epi16(buf2, buf2);
			zbuf32 = _mm_add_epi32(zbuf32, buf1);
			zbuf32 = _mm_add_epi32(zbuf32, buf2);
			px += nstep;
		}
		buf1 = _mm_unpacklo_epi32(zbuf32, zero);
		buf2 = _mm_unpackhi_epi32(zbuf32, zero);
		zbuf64 = _mm_add_epi64(zbuf64, buf1);
		zbuf64 = _mm_add_epi64(zbuf64, buf2);

		for (i = 0; i < n3; i++) {
			sum += (mlib_s32)(*px) * (*px);
			px++;
		}

		mlib_d64 dsum = sum;
		long long pz[2];
		_mm_storeu_si128((__m128i *)pz, zbuf64);
		dsum += pz[0];
		dsum += pz[1];
		*z = mlib_sqrt(dsum);
	} else {
		for (i = 0; i < n; i++) {
			sum += (mlib_s32)(*px) * (*px);
			px++;
		}
		*z = mlib_sqrt((mlib_d64)sum);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VectorNorm_S8_Sat(
	mlib_d64 *z,
	const mlib_s8 *x,
	mlib_s32 n)
{
	if (n <= 0)
		return (MLIB_FAILURE);

	mlib_s32 i, nstep, n1, n2, n3, sum = 0;
	const mlib_s8 *px = x;
	__m128i zero, xbuf, zbuf32, zbuf64, buf1, buf2;
	zero = _mm_setzero_si128();
	zbuf64 = zero;

	nstep = 16 / sizeof (mlib_s8);
	n1 = ((16 - ((mlib_addr)x & 15)) & 15) / sizeof (mlib_s8);
	n2 = (n - n1) / nstep;
	n3 = n - n1 - n2 * nstep;

	if (n2 > 0) {
		for (i = 0; i < n1; i++) {
			sum += (mlib_s32)(*px) * (*px);
			px++;
		}

		mlib_s32 nblock = n2 >> 12;
		mlib_s32 tail = n2 & 4095;
		mlib_s32 k;
		for (k = 0; k < nblock; k++) {
			zbuf32 = zero;
			for (i = 0; i < 4096; i++) {
				xbuf = _mm_load_si128((__m128i *)px);
				buf1 = _mm_unpacklo_epi8(zero, xbuf);
				buf2 = _mm_unpackhi_epi8(zero, xbuf);
				buf1 = _mm_srai_epi16(buf1, 8);
				buf2 = _mm_srai_epi16(buf2, 8);
				buf1 = _mm_madd_epi16(buf1, buf1);
				buf2 = _mm_madd_epi16(buf2, buf2);
				zbuf32 = _mm_add_epi32(zbuf32, buf1);
				zbuf32 = _mm_add_epi32(zbuf32, buf2);
				px += nstep;
			}
			buf1 = _mm_unpacklo_epi32(zbuf32, zero);
			buf2 = _mm_unpackhi_epi32(zbuf32, zero);
			zbuf64 = _mm_add_epi64(zbuf64, buf1);
			zbuf64 = _mm_add_epi64(zbuf64, buf2);
		}
		zbuf32 = zero;
		for (i = 0; i < tail; i++) {
			xbuf = _mm_load_si128((__m128i *)px);
			buf1 = _mm_unpacklo_epi8(zero, xbuf);
			buf2 = _mm_unpackhi_epi8(zero, xbuf);
			buf1 = _mm_srai_epi16(buf1, 8);
			buf2 = _mm_srai_epi16(buf2, 8);
			buf1 = _mm_madd_epi16(buf1, buf1);
			buf2 = _mm_madd_epi16(buf2, buf2);
			zbuf32 = _mm_add_epi32(zbuf32, buf1);
			zbuf32 = _mm_add_epi32(zbuf32, buf2);
			px += nstep;
		}
		buf1 = _mm_unpacklo_epi32(zbuf32, zero);
		buf2 = _mm_unpackhi_epi32(zbuf32, zero);
		zbuf64 = _mm_add_epi64(zbuf64, buf1);
		zbuf64 = _mm_add_epi64(zbuf64, buf2);

		for (i = 0; i < n3; i++) {
			sum += (mlib_s32)(*px) * (*px);
			px++;
		}

		mlib_d64 dsum = sum;
		long long pz[2];
		_mm_storeu_si128((__m128i *)pz, zbuf64);
		dsum += pz[0];
		dsum += pz[1];
		*z = mlib_sqrt(dsum);
	} else {
		for (i = 0; i < n; i++) {
			sum += (mlib_s32)(*px) * (*px);
			px++;
		}
		*z = mlib_sqrt((mlib_d64)sum);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VectorNorm_S16_Sat(
	mlib_d64 *z,
	const mlib_s16 *x,
	mlib_s32 n)
{
	if (n <= 0)
		return (MLIB_FAILURE);

	mlib_s32 i, nstep, n1, n2, n3;
	mlib_d64 dsum = 0.0;
	const mlib_s16 *px = x;
	__m128i zero, xbuf, zbuf32, zbuf64, buf1, buf2;
	zero = _mm_setzero_si128();
	zbuf64 = zero;

	nstep = 16 / sizeof (mlib_s16);
	n1 = ((16 - ((mlib_addr)x & 15)) & 15) / sizeof (mlib_s16);
	n2 = (n - n1) / nstep;
	n3 = n - n1 - n2 * nstep;

	if (n2 > 0) {
		for (i = 0; i < n1; i++) {
			dsum += (mlib_d64)(*px) * (*px);
			px++;
		}

		for (i = 0; i < n2; i++) {
			xbuf = _mm_load_si128((__m128i *)px);
			zbuf32 = _mm_madd_epi16(xbuf, xbuf);
			buf1 = _mm_unpacklo_epi32(zbuf32, zero);
			buf2 = _mm_unpackhi_epi32(zbuf32, zero);
			zbuf64 = _mm_add_epi64(zbuf64, buf1);
			zbuf64 = _mm_add_epi64(zbuf64, buf2);
			px += nstep;
		}

		for (i = 0; i < n3; i++) {
			dsum += (mlib_d64)(*px) * (*px);
			px++;
		}

		long long pz[2];
		_mm_storeu_si128((__m128i *)pz, zbuf64);
		dsum += pz[0];
		dsum += pz[1];
		*z = mlib_sqrt(dsum);
	} else {
		for (i = 0; i < n; i++) {
			dsum += (mlib_d64)(*px) * (*px);
			px++;
		}
		*z = mlib_sqrt(dsum);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VectorNorm_S32_Sat(
	mlib_d64 *z,
	const mlib_s32 *x,
	mlib_s32 n)
{
	mlib_s32 *px = (mlib_s32 *)x;
	mlib_s32 i;
	mlib_d64 sum = 0;

	if (n <= 0)
		return (MLIB_FAILURE);
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < n; i++)
		sum += px[i] * (mlib_d64)px[i];
	z[0] = mlib_sqrt((mlib_d64)sum);
	return (MLIB_SUCCESS);
}

/* *********************************************************** */
