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

#pragma ident	"@(#)mlib_s_VectorSumAbsDiff.c	9.2	07/11/05 SMI"

/*
 * FUNCTIONS
 *     mlib_VectorSumAbsDiff_[U8|S8|S16|S32]_Sat - sum of absolute difference
 *
 * SYNOPSIS
 *      mlib_status mlib_VectorSumAbsDiff_S16_Sat(mlib_d64       * z,
 *                                                const mlib_s16 * x,
 *                                                const mlib_s16 * y,
 *                                                mlib_s32       n);
 *      mlib_status mlib_VectorSumAbsDiff_S32_Sat(mlib_d64       * z,
 *                                                const mlib_s32 * x,
 *                                                const mlib_s32 * y,
 *                                                mlib_s32       n);
 *      mlib_status mlib_VectorSumAbsDiff_S8_Sat(mlib_d64      * z,
 *                                               const mlib_s8 * x,
 *                                               const mlib_s8 * y,
 *                                               mlib_s32      n);
 *      mlib_status mlib_VectorSumAbsDiff_U8_Sat(mlib_d64      * z,
 *                                               const mlib_u8 * x,
 *                                               const mlib_u8 * y,
 *                                               mlib_s32      n);
 *
 * ARGUMENTS
 *      z    pointer to the sum of the absolute difference
 *      x    pointer to the first input vector
 *      y    pointer to the second input vector
 *      n    number of elements in the vector
 *
 * DESCRIPTION
 *          n - 1
 *      z = SUM |x[i] - y[i]|
 *          i = 0
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

#pragma weak mlib_VectorSumAbsDiff_S32_Sat = __mlib_VectorSumAbsDiff_S32_Sat
#pragma weak mlib_VectorSumAbsDiff_S16_Sat = __mlib_VectorSumAbsDiff_S16_Sat
#pragma weak mlib_VectorSumAbsDiff_S8_Sat = __mlib_VectorSumAbsDiff_S8_Sat
#pragma weak mlib_VectorSumAbsDiff_U8_Sat = __mlib_VectorSumAbsDiff_U8_Sat

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */

__typeof__(__mlib_VectorSumAbsDiff_S32_Sat) mlib_VectorSumAbsDiff_S32_Sat
	__attribute__((weak, alias("__mlib_VectorSumAbsDiff_S32_Sat")));
__typeof__(__mlib_VectorSumAbsDiff_S16_Sat) mlib_VectorSumAbsDiff_S16_Sat
	__attribute__((weak, alias("__mlib_VectorSumAbsDiff_S16_Sat")));
__typeof__(__mlib_VectorSumAbsDiff_S8_Sat) mlib_VectorSumAbsDiff_S8_Sat
	__attribute__((weak, alias("__mlib_VectorSumAbsDiff_S8_Sat")));
__typeof__(__mlib_VectorSumAbsDiff_U8_Sat) mlib_VectorSumAbsDiff_U8_Sat
	__attribute__((weak, alias("__mlib_VectorSumAbsDiff_U8_Sat")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	ABS_VALUE(x)	(((x) ^ ((x) >> 31)) - ((x) >> 31))

/* *********************************************************** */

#define	MLIB_C_VECTORSUMABSDIFF(DTYPE, Z, X, Y, N)              \
	DTYPE *px, *py;                                         \
	mlib_u32 acc;                                           \
	mlib_s32 i;                                             \
	                                                        \
	if (N <= 0)                                             \
	    return (MLIB_FAILURE);                              \
	px = (DTYPE *) X;                                       \
	py = (DTYPE *) Y;                                       \
	acc = 0;                                                \
	for (i = 0; i < N; i++) {                               \
	    acc += (mlib_u32)ABS_VALUE(px[i] - py[i]);          \
	}                                                       \
	                                                        \
	Z[0] = (mlib_d64)acc;                                   \
	return MLIB_SUCCESS

/* *********************************************************** */

mlib_status
__mlib_VectorSumAbsDiff_U8_Sat(
	mlib_d64 *z,
	const mlib_u8 *x,
	const mlib_u8 *y,
	mlib_s32 n)
{
	if (n <= 0)
		return (MLIB_FAILURE);

	mlib_s32 i, nstep, ax, ay, n1, n2, n3, diff, sum = 0;
	mlib_u8 *px = (mlib_u8 *)x, *py = (mlib_u8 *)y;
	__m128i zero, xbuf, ybuf, zbuf, mbuf;
	zbuf = _mm_setzero_si128();

	nstep = 16 / sizeof (mlib_u8);
	ax = (mlib_addr)x & 15;
	ay = (mlib_addr)y & 15;
	n1 = ((16 - ax) & 15) / sizeof (mlib_u8);
	n2 = (n - n1) / nstep;
	n3 = n - n1 - n2 * nstep;

	if (n2 < 1) {
		for (i = 0; i < n; i++) {
			diff = (mlib_s32)(*px++) - (*py++);
			sum += ABS_VALUE(diff);
		}
		*z = sum;
	} else {
		for (i = 0; i < n1; i++) {
			diff = (mlib_s32)(*px++) - (*py++);
			sum += ABS_VALUE(diff);
		}
		if (ax == ay) {
			for (i = 0; i < n2; i++) {
				xbuf = _mm_load_si128((__m128i *)px);
				ybuf = _mm_load_si128((__m128i *)py);
				mbuf = _mm_sad_epu8(xbuf, ybuf);
				zbuf = _mm_add_epi64(zbuf, mbuf);
				px += nstep;
				py += nstep;
			}
		} else {
			for (i = 0; i < n2; i++) {
				xbuf = _mm_load_si128((__m128i *)px);
				ybuf = _mm_loadu_si128((__m128i *)py);
				mbuf = _mm_sad_epu8(xbuf, ybuf);
				zbuf = _mm_add_epi64(zbuf, mbuf);
				px += nstep;
				py += nstep;
			}
		}
		for (i = 0; i < n3; i++) {
			diff = (mlib_s32)(*px++) - (*py++);
			sum += ABS_VALUE(diff);
		}

		mlib_d64 dsum = sum;
		long long pz[2];
		_mm_storeu_si128((__m128i *)pz, zbuf);
		dsum += pz[0];
		dsum += pz[1];
		*z = dsum;
	}
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VectorSumAbsDiff_S8_Sat(
	mlib_d64 *z,
	const mlib_s8 *x,
	const mlib_s8 *y,
	mlib_s32 n)
{
	if (n <= 0)
		return (MLIB_FAILURE);

	mlib_s32 i, nstep, ax, ay, n1, n2, n3, diff, sum = 0;
	mlib_s8 *px = (mlib_s8 *)x, *py = (mlib_s8 *)y;
	__m128i zero, xbuf, ybuf, zbuf, mext, mbuf;
	zero = _mm_setzero_si128();
	zbuf = zero;

	nstep = 16 / sizeof (mlib_s8);
	ax = (mlib_addr)x & 15;
	ay = (mlib_addr)y & 15;
	n1 = ((16 - ax) & 15) / sizeof (mlib_s8);
	n2 = (n - n1) / nstep;
	n3 = n - n1 - n2 * nstep;

	if (n2 < 1) {
		for (i = 0; i < n; i++) {
			diff = (mlib_s32)(*px++) - (*py++);
			sum += ABS_VALUE(diff);
		}
		*z = sum;
	} else {
		for (i = 0; i < n1; i++) {
			diff = (mlib_s32)(*px++) - (*py++);
			sum += ABS_VALUE(diff);
		}
		if (ax == ay) {
			for (i = 0; i < n2; i++) {
				xbuf = _mm_load_si128((__m128i *)px);
				ybuf = _mm_load_si128((__m128i *)py);
				mext = _mm_cmpgt_epi8(ybuf, xbuf);
				mbuf = _mm_sub_epi8(xbuf, ybuf);
				mbuf = _mm_xor_si128(mbuf, mext);
				mbuf = _mm_sub_epi8(mbuf, mext);
				mbuf = _mm_sad_epu8(mbuf, zero);
				zbuf = _mm_add_epi64(zbuf, mbuf);
				px += nstep;
				py += nstep;
			}
		} else {
			for (i = 0; i < n2; i++) {
				xbuf = _mm_load_si128((__m128i *)px);
				ybuf = _mm_loadu_si128((__m128i *)py);
				mext = _mm_cmpgt_epi8(ybuf, xbuf);
				mbuf = _mm_sub_epi8(xbuf, ybuf);
				mbuf = _mm_xor_si128(mbuf, mext);
				mbuf = _mm_sub_epi8(mbuf, mext);
				mbuf = _mm_sad_epu8(mbuf, zero);
				zbuf = _mm_add_epi64(zbuf, mbuf);
				px += nstep;
				py += nstep;
			}
		}
		for (i = 0; i < n3; i++) {
			diff = (mlib_s32)(*px++) - (*py++);
			sum += ABS_VALUE(diff);
		}

		mlib_d64 dsum = sum;
		long long pz[2];
		_mm_storeu_si128((__m128i *)pz, zbuf);
		dsum += pz[0];
		dsum += pz[1];
		*z = dsum;
	}
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VectorSumAbsDiff_S16_Sat(
	mlib_d64 *z,
	const mlib_s16 *x,
	const mlib_s16 *y,
	mlib_s32 n)
{
	if (n <= 0)
		return (MLIB_FAILURE);

	mlib_s32 i, nstep, ax, ay, n1, n2, n3, xval, sum = 0;
	mlib_s16 *px = (mlib_s16 *)x, *py = (mlib_s16 *)y;
	__m128i zero, xbuf, ybuf, zbuf32, zbuf64, xlo, xhi, mext;
	zero = _mm_setzero_si128();
	zbuf64 = zero;

	nstep = 16 / sizeof (mlib_s16);
	ax = (mlib_addr)x & 15;
	ay = (mlib_addr)y & 15;
	n1 = ((16 - ax) & 15) / sizeof (mlib_s16);
	n2 = (n - n1) / nstep;
	n3 = n - n1 - n2 * nstep;

	if (n2 < 1) {
		for (i = 0; i < n; i++) {
			xval = (mlib_s32)(*px++) - (*py++);
			sum += ABS_VALUE(xval);
		}
		*z = sum;
	} else {
		for (i = 0; i < n1; i++) {
			xval = (mlib_s32)(*px++) - (*py++);
			sum += ABS_VALUE(xval);
		}
		mlib_s32 nblock = n2 >> 12;
		mlib_s32 tail = n2 & 4095;
		mlib_s32 k;
		if (ax == ay) {
			for (k = 0; k < nblock; k++) {
				zbuf32 = zero;
				for (i = 0; i < 4096; i++) {
					xbuf = _mm_load_si128((__m128i *)px);
					ybuf = _mm_load_si128((__m128i *)py);
					mext = _mm_cmpgt_epi16(ybuf, xbuf);
					xbuf = _mm_sub_epi16(xbuf, ybuf);
					xbuf = _mm_xor_si128(xbuf, mext);
					xbuf = _mm_sub_epi16(xbuf, mext);
					xlo = _mm_unpacklo_epi16(xbuf, zero);
					xhi = _mm_unpackhi_epi16(xbuf, zero);
					zbuf32 = _mm_add_epi32(zbuf32, xlo);
					zbuf32 = _mm_add_epi32(zbuf32, xhi);
					px += nstep;
					py += nstep;
				}
				xlo = _mm_unpacklo_epi32(zbuf32, zero);
				xhi = _mm_unpackhi_epi32(zbuf32, zero);
				zbuf64 = _mm_add_epi64(zbuf64, xlo);
				zbuf64 = _mm_add_epi64(zbuf64, xhi);
			}
			zbuf32 = zero;
			for (i = 0; i < tail; i++) {
				xbuf = _mm_load_si128((__m128i *)px);
				ybuf = _mm_load_si128((__m128i *)py);
				mext = _mm_cmpgt_epi16(ybuf, xbuf);
				xbuf = _mm_sub_epi16(xbuf, ybuf);
				xbuf = _mm_xor_si128(xbuf, mext);
				xbuf = _mm_sub_epi16(xbuf, mext);
				xlo = _mm_unpacklo_epi16(xbuf, zero);
				xhi = _mm_unpackhi_epi16(xbuf, zero);
				zbuf32 = _mm_add_epi32(zbuf32, xlo);
				zbuf32 = _mm_add_epi32(zbuf32, xhi);
				px += nstep;
				py += nstep;
			}
			xlo = _mm_unpacklo_epi32(zbuf32, zero);
			xhi = _mm_unpackhi_epi32(zbuf32, zero);
			zbuf64 = _mm_add_epi64(zbuf64, xlo);
			zbuf64 = _mm_add_epi64(zbuf64, xhi);
		} else { /* not aligned */
			for (k = 0; k < nblock; k++) {
				zbuf32 = zero;
				for (i = 0; i < 4096; i++) {
					xbuf = _mm_load_si128((__m128i *)px);
					ybuf = _mm_loadu_si128((__m128i *)py);
					mext = _mm_cmpgt_epi16(ybuf, xbuf);
					xbuf = _mm_sub_epi16(xbuf, ybuf);
					xbuf = _mm_xor_si128(xbuf, mext);
					xbuf = _mm_sub_epi16(xbuf, mext);
					xlo = _mm_unpacklo_epi16(xbuf, zero);
					xhi = _mm_unpackhi_epi16(xbuf, zero);
					zbuf32 = _mm_add_epi32(zbuf32, xlo);
					zbuf32 = _mm_add_epi32(zbuf32, xhi);
					px += nstep;
					py += nstep;
				}
				xlo = _mm_unpacklo_epi32(zbuf32, zero);
				xhi = _mm_unpackhi_epi32(zbuf32, zero);
				zbuf64 = _mm_add_epi64(zbuf64, xlo);
				zbuf64 = _mm_add_epi64(zbuf64, xhi);
			}
			zbuf32 = zero;
			for (i = 0; i < tail; i++) {
				xbuf = _mm_load_si128((__m128i *)px);
				ybuf = _mm_loadu_si128((__m128i *)py);
				mext = _mm_cmpgt_epi16(ybuf, xbuf);
				xbuf = _mm_sub_epi16(xbuf, ybuf);
				xbuf = _mm_xor_si128(xbuf, mext);
				xbuf = _mm_sub_epi16(xbuf, mext);
				xlo = _mm_unpacklo_epi16(xbuf, zero);
				xhi = _mm_unpackhi_epi16(xbuf, zero);
				zbuf32 = _mm_add_epi32(zbuf32, xlo);
				zbuf32 = _mm_add_epi32(zbuf32, xhi);
				px += nstep;
				py += nstep;
			}
			xlo = _mm_unpacklo_epi32(zbuf32, zero);
			xhi = _mm_unpackhi_epi32(zbuf32, zero);
			zbuf64 = _mm_add_epi64(zbuf64, xlo);
			zbuf64 = _mm_add_epi64(zbuf64, xhi);
		}
		for (i = 0; i < n3; i++) {
			xval = (mlib_s32)(*px++) - (*py++);
			sum += ABS_VALUE(xval);
		}

		mlib_d64 dsum = sum;
		long long pz[2];
		_mm_storeu_si128((__m128i *)pz, zbuf64);
		dsum += pz[0];
		dsum += pz[1];
		*z = dsum;
	}
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VectorSumAbsDiff_S32_Sat(
	mlib_d64 *z,
	const mlib_s32 *x,
	const mlib_s32 *y,
	mlib_s32 n)
{
	if (n <= 0)
		return (MLIB_FAILURE);

	mlib_s32 i, nstep, ax, ay, n1, n2, n3;
	mlib_s32 *px = (mlib_s32 *)x, *py = (mlib_s32 *)y;
	__m128i zero, xbuf, ybuf, zbuf, xlo, xhi, mext;
	mlib_d64 dsum = 0.0;
	zero = _mm_setzero_si128();
	zbuf = zero;

	nstep = 16 / sizeof (mlib_s32);
	ax = (mlib_addr)x & 15;
	ay = (mlib_addr)y & 15;
	n1 = ((16 - ax) & 15) / sizeof (mlib_s32);
	n2 = (n - n1) / nstep;
	n3 = n - n1 - n2 * nstep;

	if (n2 < 1) {
		for (i = 0; i < n; i++) {
			dsum += mlib_fabs((mlib_d64)(*px++) - (*py++));
		}
		*z = dsum;
	} else {
		for (i = 0; i < n1; i++) {
			dsum += mlib_fabs((mlib_d64)(*px++) - (*py++));
		}
		if (ax == ay) {
			for (i = 0; i < n2; i++) {
				xbuf = _mm_load_si128((__m128i *)px);
				ybuf = _mm_load_si128((__m128i *)py);
				mext = _mm_cmpgt_epi32(ybuf, xbuf);
				xbuf = _mm_sub_epi32(xbuf, ybuf);
				xbuf = _mm_xor_si128(xbuf, mext);
				xbuf = _mm_sub_epi32(xbuf, mext);
				xlo = _mm_unpacklo_epi32(xbuf, zero);
				xhi = _mm_unpackhi_epi32(xbuf, zero);
				zbuf = _mm_add_epi64(zbuf, xlo);
				zbuf = _mm_add_epi64(zbuf, xhi);
				px += nstep;
				py += nstep;
			}
		} else {
			for (i = 0; i < n2; i++) {
				xbuf = _mm_load_si128((__m128i *)px);
				ybuf = _mm_loadu_si128((__m128i *)py);
				mext = _mm_cmpgt_epi32(ybuf, xbuf);
				xbuf = _mm_sub_epi32(xbuf, ybuf);
				xbuf = _mm_xor_si128(xbuf, mext);
				xbuf = _mm_sub_epi32(xbuf, mext);
				xlo = _mm_unpacklo_epi32(xbuf, zero);
				xhi = _mm_unpackhi_epi32(xbuf, zero);
				zbuf = _mm_add_epi64(zbuf, xlo);
				zbuf = _mm_add_epi64(zbuf, xhi);
				px += nstep;
				py += nstep;
			}
		}
		for (i = 0; i < n3; i++) {
			dsum += mlib_fabs((mlib_d64)(*px++) - (*py++));
		}

		long long pz[2];
		_mm_storeu_si128((__m128i *)pz, zbuf);
		dsum += pz[0];
		dsum += pz[1];
		*z = dsum;
	}
	return (MLIB_SUCCESS);
}

/* *********************************************************** */
