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

#pragma ident	"@(#)mlib_s_VectorDistance.c	9.2	07/11/05 SMI"

/*
 * FUNCTIONS
 *     mlib_VectorDistance_[U8|S8|S16|S32]_Sat - Euclidean distance
 *
 * SYNOPSIS
 *     mlib_status mlib_VectorDistance_U8_Sat(mlib_d64      *z,
 *                                            const mlib_u8 *x,
 *                                            const mlib_u8 *y,
 *                                            mlib_s32      n);
 *     mlib_status mlib_VectorDistance_S8_Sat(mlib_d64      *z,
 *                                            const mlib_s8 *x,
 *                                            const mlib_s8 *y,
 *                                            mlib_s32      n);
 *     mlib_status mlib_VectorDistance_S16_Sat(mlib_d64       *z,
 *                                             const mlib_s16 *x,
 *                                             const mlib_s16 *y,
 *                                             mlib_s32       n);
 *     mlib_status mlib_VectorDistance_S32_Sat(mlib_d64       *z,
 *                                             const mlib_s32 *x,
 *                                             const mlib_s32 *y,
 *                                             mlib_s32       n);
 *
 * ARGUMENTS
 *      z    pointer to the Euclidean distance
 *      x    pointer to the first input vector
 *      y    pointer to the second input vector
 *      n    number of elements in the vector
 *
 * DESCRIPTION
 *                n - 1
 *      z = sqrt( SUM (x[i] - y[i])^2 )
 *                i = 0
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

#pragma weak mlib_VectorDistance_S16_Sat = __mlib_VectorDistance_S16_Sat
#pragma weak mlib_VectorDistance_S8_Sat = __mlib_VectorDistance_S8_Sat
#pragma weak mlib_VectorDistance_U8_Sat = __mlib_VectorDistance_U8_Sat
#pragma weak mlib_VectorDistance_S32_Sat = __mlib_VectorDistance_S32_Sat

#elif defined(__GNUC__)

__typeof__(__mlib_VectorDistance_S16_Sat) mlib_VectorDistance_S16_Sat
	__attribute__((weak, alias("__mlib_VectorDistance_S16_Sat")));
__typeof__(__mlib_VectorDistance_S8_Sat) mlib_VectorDistance_S8_Sat
	__attribute__((weak, alias("__mlib_VectorDistance_S8_Sat")));
__typeof__(__mlib_VectorDistance_U8_Sat) mlib_VectorDistance_U8_Sat
	__attribute__((weak, alias("__mlib_VectorDistance_U8_Sat")));
__typeof__(__mlib_VectorDistance_S32_Sat) mlib_VectorDistance_S32_Sat
	__attribute__((weak, alias("__mlib_VectorDistance_S32_Sat")));

#else

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	MLIB_C_VECTORDISTANCE(DTYPE, Z, X, Y, N)                \
	DTYPE *px, *py;                                         \
	mlib_d64 acc, item;                                     \
	mlib_s32 i;                                             \
	                                                        \
	if (N <= 0)                                             \
	    return (MLIB_FAILURE);                              \
	px = (DTYPE *) X;                                       \
	py = (DTYPE *) Y;                                       \
	acc = 0.0;                                              \
	for (i = 0; i < N; i++) {                               \
	    item = (mlib_d64)(px[i] - py[i]);                   \
	    acc += item * item;                                 \
	}                                                       \
	                                                        \
	Z[0] = mlib_sqrt(acc);                                  \
	return MLIB_SUCCESS

/* *********************************************************** */

#define	VECTOR_DIS_U8(ld)				\
	xbuf = _mm_load_si128((__m128i *)px);		\
	ybuf = _mm_##ld##_si128((__m128i *)py);		\
	buf1 = _mm_unpacklo_epi8(xbuf, zero);		\
	buf2 = _mm_unpackhi_epi8(xbuf, zero);		\
	buf3 = _mm_unpacklo_epi8(ybuf, zero);		\
	buf4 = _mm_unpackhi_epi8(ybuf, zero);		\
	buf1 = _mm_sub_epi16(buf1, buf3);		\
	buf2 = _mm_sub_epi16(buf2, buf4);		\
	buf1 = _mm_madd_epi16(buf1, buf1);		\
	buf2 = _mm_madd_epi16(buf2, buf2);		\
	zbuf32 = _mm_add_epi32(zbuf32, buf1);		\
	zbuf32 = _mm_add_epi32(zbuf32, buf2);		\
	px += nstep;					\
	py += nstep

mlib_status
__mlib_VectorDistance_U8_Sat(
	mlib_d64 *z,
	const mlib_u8 *x,
	const mlib_u8 *y,
	mlib_s32 n)
{
	if (n <= 0)
		return (MLIB_FAILURE);

	mlib_s32 i, ax, ay, nstep, n1, n2, n3, sub, sum = 0;
	const mlib_u8 *px = x, *py = y;
	__m128i zero, xbuf, ybuf, zbuf32, zbuf64, buf1, buf2, buf3, buf4;
	zero = _mm_setzero_si128();
	zbuf64 = zero;

	ax = (mlib_addr)x & 15;
	ay = (mlib_addr)y & 15;
	nstep = 16 / sizeof (mlib_u8);
	n1 = ((16 - ax) & 15) / sizeof (mlib_u8);
	n2 = (n - n1) / nstep;
	n3 = n - n1 - n2 * nstep;

	if (n2 > 0) {
		for (i = 0; i < n1; i++) {
			sub = (mlib_s32)(*px++) - (*py++);
			sum += sub * sub;
		}

		mlib_s32 nblock = n2 >> 12;
		mlib_s32 tail = n2 & 4095;
		mlib_s32 k;

		if (ax == ay) {
			for (k = 0; k < nblock; k++) {
				zbuf32 = zero;
				for (i = 0; i < 4096; i++) {
					VECTOR_DIS_U8(load);
				}
				buf1 = _mm_unpacklo_epi32(zbuf32, zero);
				buf2 = _mm_unpackhi_epi32(zbuf32, zero);
				zbuf64 = _mm_add_epi64(zbuf64, buf1);
				zbuf64 = _mm_add_epi64(zbuf64, buf2);
			}
			zbuf32 = zero;
			for (i = 0; i < tail; i++) {
				VECTOR_DIS_U8(load);
			}
		} else {
			for (k = 0; k < nblock; k++) {
				zbuf32 = zero;
				for (i = 0; i < 4096; i++) {
					VECTOR_DIS_U8(loadu);
				}
				buf1 = _mm_unpacklo_epi32(zbuf32, zero);
				buf2 = _mm_unpackhi_epi32(zbuf32, zero);
				zbuf64 = _mm_add_epi64(zbuf64, buf1);
				zbuf64 = _mm_add_epi64(zbuf64, buf2);
			}
			zbuf32 = zero;
			for (i = 0; i < tail; i++) {
				VECTOR_DIS_U8(loadu);
			}
		}
		buf1 = _mm_unpacklo_epi32(zbuf32, zero);
		buf2 = _mm_unpackhi_epi32(zbuf32, zero);
		zbuf64 = _mm_add_epi64(zbuf64, buf1);
		zbuf64 = _mm_add_epi64(zbuf64, buf2);

		for (i = 0; i < n3; i++) {
			sub = (mlib_s32)(*px++) - (*py++);
			sum += sub * sub;
		}

		mlib_d64 dsum = sum;
		long long pz[2];
		_mm_storeu_si128((__m128i *)pz, zbuf64);
		dsum += pz[0];
		dsum += pz[1];
		*z = mlib_sqrt(dsum);
	} else {
		for (i = 0; i < n; i++) {
			sub = (mlib_s32)(*px++) - (*py++);
			sum += sub * sub;
		}
		*z = mlib_sqrt((mlib_d64)sum);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#define	VECTOR_DIS_S8(ld)				\
	xbuf = _mm_load_si128((__m128i *)px);		\
	ybuf = _mm_##ld##_si128((__m128i *)py);		\
	buf1 = _mm_unpacklo_epi8(zero, xbuf);		\
	buf2 = _mm_unpackhi_epi8(zero, xbuf);		\
	buf3 = _mm_unpacklo_epi8(zero, ybuf);		\
	buf4 = _mm_unpackhi_epi8(zero, ybuf);		\
	buf1 = _mm_srai_epi16(buf1, 8);			\
	buf2 = _mm_srai_epi16(buf2, 8);			\
	buf3 = _mm_srai_epi16(buf3, 8);			\
	buf4 = _mm_srai_epi16(buf4, 8);			\
	buf1 = _mm_sub_epi16(buf1, buf3);		\
	buf2 = _mm_sub_epi16(buf2, buf4);		\
	buf1 = _mm_madd_epi16(buf1, buf1);		\
	buf2 = _mm_madd_epi16(buf2, buf2);		\
	zbuf32 = _mm_add_epi32(zbuf32, buf1);		\
	zbuf32 = _mm_add_epi32(zbuf32, buf2);		\
	px += nstep;					\
	py += nstep

mlib_status
__mlib_VectorDistance_S8_Sat(
	mlib_d64 *z,
	const mlib_s8 *x,
	const mlib_s8 *y,
	mlib_s32 n)
{
	if (n <= 0)
		return (MLIB_FAILURE);

	mlib_s32 i, ax, ay, nstep, n1, n2, n3, sub, sum = 0;
	const mlib_s8 *px = x, *py = y;
	__m128i zero, xbuf, ybuf, zbuf32, zbuf64, buf1, buf2, buf3, buf4;
	zero = _mm_setzero_si128();
	zbuf64 = zero;

	ax = (mlib_addr)x & 15;
	ay = (mlib_addr)y & 15;
	nstep = 16 / sizeof (mlib_s8);
	n1 = ((16 - ax) & 15) / sizeof (mlib_s8);
	n2 = (n - n1) / nstep;
	n3 = n - n1 - n2 * nstep;

	if (n2 > 0) {
		for (i = 0; i < n1; i++) {
			sub = (mlib_s32)(*px++) - (*py++);
			sum += sub * sub;
		}

		mlib_s32 nblock = n2 >> 12;
		mlib_s32 tail = n2 & 4095;
		mlib_s32 k;

		if (ax == ay) {
			for (k = 0; k < nblock; k++) {
				zbuf32 = zero;
				for (i = 0; i < 4096; i++) {
					VECTOR_DIS_S8(load);
				}
				buf1 = _mm_unpacklo_epi32(zbuf32, zero);
				buf2 = _mm_unpackhi_epi32(zbuf32, zero);
				zbuf64 = _mm_add_epi64(zbuf64, buf1);
				zbuf64 = _mm_add_epi64(zbuf64, buf2);
			}
			zbuf32 = zero;
			for (i = 0; i < tail; i++) {
				VECTOR_DIS_S8(load);
			}
		} else {
			for (k = 0; k < nblock; k++) {
				zbuf32 = zero;
				for (i = 0; i < 4096; i++) {
					VECTOR_DIS_S8(loadu);
				}
				buf1 = _mm_unpacklo_epi32(zbuf32, zero);
				buf2 = _mm_unpackhi_epi32(zbuf32, zero);
				zbuf64 = _mm_add_epi64(zbuf64, buf1);
				zbuf64 = _mm_add_epi64(zbuf64, buf2);
			}
			zbuf32 = zero;
			for (i = 0; i < tail; i++) {
				VECTOR_DIS_S8(loadu);
			}
		}
		buf1 = _mm_unpacklo_epi32(zbuf32, zero);
		buf2 = _mm_unpackhi_epi32(zbuf32, zero);
		zbuf64 = _mm_add_epi64(zbuf64, buf1);
		zbuf64 = _mm_add_epi64(zbuf64, buf2);

		for (i = 0; i < n3; i++) {
			sub = (mlib_s32)(*px++) - (*py++);
			sum += sub * sub;
		}

		mlib_d64 dsum = sum;
		long long pz[2];
		_mm_storeu_si128((__m128i *)pz, zbuf64);
		dsum += pz[0];
		dsum += pz[1];
		*z = mlib_sqrt(dsum);
	} else {
		for (i = 0; i < n; i++) {
			sub = (mlib_s32)(*px++) - (*py++);
			sum += sub * sub;
		}
		*z = mlib_sqrt((mlib_d64)sum);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#define	VECTOR_DIS_S16(ld)				\
	xbuf = _mm_load_si128((__m128i *)px);		\
	ybuf = _mm_##ld##_si128((__m128i *)py);		\
	buf1 = _mm_sub_epi16(xbuf, ybuf);		\
	mask = _mm_cmpgt_epi16(ybuf, xbuf);		\
	buf1 = _mm_xor_si128(buf1, mask);		\
	buf1 = _mm_sub_epi16(buf1, mask);		\
	buf2 = _mm_mullo_epi16(buf1, buf1);		\
	buf4 = _mm_mulhi_epu16(buf1, buf1);		\
	buf1 = _mm_unpacklo_epi16(buf2, buf4);		\
	buf3 = _mm_unpackhi_epi16(buf2, buf4);		\
	buf2 = _mm_unpackhi_epi32(buf1, zero);		\
	buf1 = _mm_unpacklo_epi32(buf1, zero);		\
	buf4 = _mm_unpackhi_epi32(buf3, zero);		\
	buf3 = _mm_unpacklo_epi32(buf3, zero);		\
	zbuf64 = _mm_add_epi64(zbuf64, buf1);		\
	zbuf64 = _mm_add_epi64(zbuf64, buf2);		\
	zbuf64 = _mm_add_epi64(zbuf64, buf3);		\
	zbuf64 = _mm_add_epi64(zbuf64, buf4);		\
	px += nstep;					\
	py += nstep

mlib_status
__mlib_VectorDistance_S16_Sat(
	mlib_d64 *z,
	const mlib_s16 *x,
	const mlib_s16 *y,
	mlib_s32 n)
{
	if (n <= 0)
		return (MLIB_FAILURE);

	mlib_s32 i, ax, ay, nstep, n1, n2, n3;
	mlib_d64 dsub, dsum = 0;
	const mlib_s16 *px = x, *py = y;
	__m128i zero, xbuf, ybuf, zbuf32, zbuf64, buf1, buf2, buf3, buf4, mask;
	zero = _mm_setzero_si128();
	zbuf64 = zero;

	ax = (mlib_addr)x & 15;
	ay = (mlib_addr)y & 15;
	nstep = 16 / sizeof (mlib_s16);
	n1 = ((16 - ax) & 15) / sizeof (mlib_s16);
	n2 = (n - n1) / nstep;
	n3 = n - n1 - n2 * nstep;

	if (n2 > 0) {
		for (i = 0; i < n1; i++) {
			dsub = (mlib_d64)(*px++) - (*py++);
			dsum += dsub * dsub;
		}

		if (ax == ay) {
			for (i = 0; i < n2; i++) {
				VECTOR_DIS_S16(load);
			}
		} else {
			for (i = 0; i < n2; i++) {
				VECTOR_DIS_S16(loadu);
			}
		}

		for (i = 0; i < n3; i++) {
			dsub = (mlib_d64)(*px++) - (*py++);
			dsum += dsub * dsub;
		}

		long long pz[2];
		_mm_storeu_si128((__m128i *)pz, zbuf64);
		dsum += pz[0];
		dsum += pz[1];
		*z = mlib_sqrt(dsum);
	} else {
		for (i = 0; i < n; i++) {
			dsub = (mlib_d64)(*px++) - (*py++);
			dsum += dsub * dsub;
		}
		*z = mlib_sqrt(dsum);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VectorDistance_S32_Sat(
	mlib_d64 *z,
	const mlib_s32 *x,
	const mlib_s32 *y,
	mlib_s32 n)
{
	mlib_d64 acc, item;
	mlib_s32 i;

	if (n <= 0)
		return (MLIB_FAILURE);

	acc = 0.0;
	for (i = 0; i < n; i++) {
		item = x[i] - (mlib_d64)y[i];
		acc += item * item;
	}

	z[0] = mlib_sqrt(acc);
	return (MLIB_SUCCESS);
}

/* *********************************************************** */
