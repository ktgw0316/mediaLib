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

#pragma ident	"@(#)mlib_s_VectorDotProd16.c	9.5	07/11/05 SMI"

/*
 * FUNCTIONS
 *      mlib_VectorDotProd_S16_Sat - dot product of signed 16-bit format
 *                                   vectors
 *
 * SYNOPSIS
 *     mlib_status mlib_VectorDotProd_S16_Sat(mlib_d64       *z,
 *                                            const mlib_s16 *x,
 *                                            const mlib_s16 *y,
 *                                            mlib_s32       n)
 *
 * ARGUMENTS
 *      z    pointer to the dot product of the vectors
 *      x    pointer to the first element of the first vector
 *      y    pointer to the first element of the second vector
 *      n    number of elements in the vectors
 *
 * DESCRIPTION
 *           n
 *      z = SUM (x[i] * y[i])
 *          i = 1
 */

/*
 * FUNCTIONS
 *      mlib_VectorDotProd_S16C_Sat - dot product of signed 16-bit format
 *                                    complex vectors
 *
 * SYNOPSIS
 *     mlib_status mlib_VectorDotProd_S16C_Sat(mlib_d64 *z,
 *                                             mlib_s16 *x,
 *                                             mlib_s16 *y,
 *                                             mlib_s32 n)
 *
 * ARGUMENTS
 *      z    pointer to the dot product of the vectors. z[0] hold the
 *           real parts, and z[1] hold the imaginary parts
 *      x    pointer to the first complex element of the first vector.
 *           x[2*i] hold the real parts, and x[2*i + 1] hold the imaginary
 *           parts
 *      y    pointer to the first complex element of the second vector.
 *           y[2*i] hold the real parts, and y[2*i + 1] hold the imaginary
 *           parts
 *      n    number of complex elements in the vectors
 *
 * DESCRIPTION
 *           n              *
 *      z = SUM (x[i] * y[i] )
 *          i = 1
 */

#include <mlib_algebra.h>

#ifdef __SUNPRO_C
#include <sunmedia_intrin.h>
#else
#include <emmintrin.h>
#endif

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_VectorDotProd_S16_Sat = __mlib_VectorDotProd_S16_Sat
#pragma weak mlib_VectorDotProd_S16C_Sat = __mlib_VectorDotProd_S16C_Sat

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */

__typeof__(__mlib_VectorDotProd_S16_Sat) mlib_VectorDotProd_S16_Sat
	__attribute__((weak, alias("__mlib_VectorDotProd_S16_Sat")));
__typeof__(__mlib_VectorDotProd_S16C_Sat) mlib_VectorDotProd_S16C_Sat
	__attribute__((weak, alias("__mlib_VectorDotProd_S16C_Sat")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	VECTOR_DOTPROD_S16(ld)				\
	xbuf = _mm_load_si128((__m128i *)px);		\
	ybuf = _mm_##ld##_si128((__m128i *)py);		\
	zbuf32 = _mm_madd_epi16(xbuf, ybuf);		\
	msk1 = _mm_cmpgt_epi32(zero, zbuf32);		\
	msk2 = _mm_cmpeq_epi32(min32, zbuf32);		\
	mask = _mm_andnot_si128(msk2, msk1);		\
	buf1 = _mm_unpacklo_epi32(zbuf32, mask);	\
	buf2 = _mm_unpackhi_epi32(zbuf32, mask);	\
	zbuf64 = _mm_add_epi64(zbuf64, buf1);		\
	zbuf64 = _mm_add_epi64(zbuf64, buf2);		\
	px += nstep;					\
	py += nstep

/* *********************************************************** */

mlib_status
__mlib_VectorDotProd_S16_Sat(
	mlib_d64 *z,
	const mlib_s16 *x,
	const mlib_s16 *y,
	mlib_s32 n)
{
	if (n <= 0)
		return (MLIB_FAILURE);

	mlib_s32 i, ax, ay, nstep, n1, n2, n3;
	mlib_d64 dsum = 0.0;
	const mlib_s16 *px = x, *py = y;
	__m128i zero, xbuf, ybuf, zbuf32, zbuf64, buf1, buf2;
	__m128i msk1, msk2, mask, min32;
	zero = _mm_setzero_si128();
	min32 = _mm_set1_epi32(MLIB_S32_MIN);
	zbuf64 = zero;

	ax = (mlib_addr)x & 15;
	ay = (mlib_addr)y & 15;
	nstep = 16 / sizeof (mlib_s16);
	n1 = ((16 - ax) & 15) / sizeof (mlib_s16);
	n2 = (n - n1) / nstep;
	n3 = n - n1 - n2 * nstep;

	if (n2 > 0) {
		for (i = 0; i < n1; i++) {
			dsum += (mlib_d64)(*px++) * (*py++);
		}

		for (i = 0; i < n2; i++) {
			VECTOR_DOTPROD_S16(loadu);
		}

		for (i = 0; i < n3; i++) {
			dsum += (mlib_d64)(*px++) * (*py++);
		}

		long long pz[2];
		_mm_storeu_si128((__m128i *)pz, zbuf64);
		dsum += pz[0];
		dsum += pz[1];
		*z = dsum;
	} else {
		for (i = 0; i < n; i++) {
			dsum += (mlib_d64)(*px++) * (*py++);
		}
		*z = dsum;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#define	VECTOR_DOTPROD_S16C(ld1, ld2)			\
	xbuf = _mm_##ld1##_si128((__m128i *)px);	\
	ybuf = _mm_##ld2##_si128((__m128i *)py);	\
							\
	rbuf = _mm_madd_epi16(xbuf, ybuf);		\
	msk1 = _mm_cmpgt_epi32(zero, rbuf);		\
	msk2 = _mm_cmpeq_epi32(min32, rbuf);		\
	mask = _mm_andnot_si128(msk2, msk1);		\
	buf1 = _mm_unpacklo_epi32(rbuf, mask);		\
	buf2 = _mm_unpackhi_epi32(rbuf, mask);		\
	rbuf64 = _mm_add_epi64(rbuf64, buf1);		\
	rbuf64 = _mm_add_epi64(rbuf64, buf2);		\
							\
	buf1 = _mm_srli_epi32(xbuf, 16);		\
	buf2 = _mm_slli_epi32(xbuf, 16);		\
	buf3 = _mm_madd_epi16(buf1, ybuf);		\
	buf4 = _mm_madd_epi16(buf2, ybuf);		\
	ibuf = _mm_sub_epi32(buf3, buf4);		\
	mask = _mm_cmpgt_epi32(zero, ibuf);		\
	buf1 = _mm_unpacklo_epi32(ibuf, mask);		\
	buf2 = _mm_unpackhi_epi32(ibuf, mask);		\
	ibuf64 = _mm_add_epi64(ibuf64, buf1);		\
	ibuf64 = _mm_add_epi64(ibuf64, buf2);		\
							\
	px += nstep;					\
	py += nstep

/* *********************************************************** */

mlib_status
__mlib_VectorDotProd_S16C_Sat(
	mlib_d64 *z,
	const mlib_s16 *x,
	const mlib_s16 *y,
	mlib_s32 n)
{
	if (n <= 0)
		return (MLIB_FAILURE);

	mlib_s32 i, ax, ay, nstep, n1, n2, n3;
	mlib_d64 sum0 = 0, sum1 = 0;
	const mlib_s16 *px = x, *py = y;
	__m128i zero, xbuf, ybuf, rbuf, ibuf, rbuf64, ibuf64;
	__m128i buf1, buf2, buf3, buf4, msk1, msk2, mask, min32;
	zero = _mm_setzero_si128();
	min32 = _mm_set1_epi32(MLIB_S32_MIN);
	rbuf64 = zero;
	ibuf64 = zero;

	ax = (mlib_addr)x & 15;
	nstep = 16 / sizeof (mlib_s16);
	n1 = ((16 - ax) & 15) / sizeof (mlib_s16);
	n2 = (n + n - n1) / nstep;
	n3 = n + n - n1 - n2 * nstep;

	if ((ax & 3) == 0) {
		if (n2 > 0) {
			for (i = 0; i < n1 / 2; i++) {
				sum0 += (mlib_d64)px[0] * py[0] + px[1] * py[1];
				sum1 += (mlib_d64)px[1] * py[0] - px[0] * py[1];
				px += 2;
				py += 2;
			}

			for (i = 0; i < n2; i++) {
				VECTOR_DOTPROD_S16C(load, loadu);
			}

			for (i = 0; i < n3 / 2; i++) {
				sum0 += (mlib_d64)px[0] * py[0] + px[1] * py[1];
				sum1 += (mlib_d64)px[1] * py[0] - px[0] * py[1];
				px += 2;
				py += 2;
			}

			long long pz[2];
			_mm_storeu_si128((__m128i *)pz, rbuf64);
			z[0] = (mlib_d64)sum0 + pz[0] + pz[1];
			_mm_storeu_si128((__m128i *)pz, ibuf64);
			z[1] = (mlib_d64)sum1 + pz[0] + pz[1];
		} else {
			for (i = 0; i < n; i++) {
				sum0 += (mlib_d64)px[0] * py[0] + px[1] * py[1];
				sum1 += (mlib_d64)px[1] * py[0] - px[0] * py[1];
				px += 2;
				py += 2;
			}
			z[0] = (mlib_d64)sum0;
			z[1] = (mlib_d64)sum1;
		}
	} else {
		n2 = (n + n) / nstep;
		n3 = n - n2 * (nstep / 2);

		for (i = 0; i < n2; i++) {
			VECTOR_DOTPROD_S16C(loadu, loadu);
		}

		for (i = 0; i < n3; i++) {
			sum0 += (mlib_s32)px[0] * py[0] + px[1] * py[1];
			sum1 += (mlib_s32)px[1] * py[0] - px[0] * py[1];
			px += 2;
			py += 2;
		}

		long long pz[2];
		_mm_storeu_si128((__m128i *)pz, rbuf64);
		z[0] = (mlib_d64)sum0 + pz[0] + pz[1];
		_mm_storeu_si128((__m128i *)pz, ibuf64);
		z[1] = (mlib_d64)sum1 + pz[0] + pz[1];
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
