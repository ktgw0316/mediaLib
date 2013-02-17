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

#pragma ident	"@(#)mlib_s_VectorDotProd8.c	9.3	07/11/05 SMI"

/*
 * FUNCTIONS
 *      mlib_VectorDotProd_U8_Sat - dot product of unsigned 8-bit format vectors
 *      mlib_VectorDotProd_S8_Sat - dot product of signed 8-bit format vectors
 *
 *
 * SYNOPSIS
 *     mlib_status mlib_VectorDotProd_U8_Sat(mlib_d64      *z,
 *                                           const mlib_u8 *x,
 *                                           const mlib_u8 *y,
 *                                           mlib_s32      n);
 *     mlib_status mlib_VectorDotProd_S8_Sat(mlib_d64      *z,
 *                                           const mlib_s8 *x,
 *                                           const mlib_s8 *y,
 *                                           mlib_s32      n);
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
 *      mlib_VectorDotProd_U8C_Sat - dot product of unsigned 8-bit format
 *                                   complex vectors
 *      mlib_VectorDotProd_S8C_Sat - dot product of signed 8-bit format
 *                                   complex vectors
 *
 * SYNOPSIS
 *     mlib_status mlib_VectorDotProd_U8C_Sat(mlib_d64      *z,
 *                                            const mlib_u8 *x,
 *                                            const mlib_u8 *y,
 *                                            mlib_s32      n);
 *     mlib_status mlib_VectorDotProd_S8C_Sat(mlib_d64      *z,
 *                                            const mlib_s8 *x,
 *                                            const mlib_s8 *y,
 *                                            mlib_s32      n);
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

#pragma weak mlib_VectorDotProd_U8C_Sat = __mlib_VectorDotProd_U8C_Sat
#pragma weak mlib_VectorDotProd_S8_Sat = __mlib_VectorDotProd_S8_Sat
#pragma weak mlib_VectorDotProd_U8_Sat = __mlib_VectorDotProd_U8_Sat
#pragma weak mlib_VectorDotProd_S8C_Sat = __mlib_VectorDotProd_S8C_Sat

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */

__typeof__(__mlib_VectorDotProd_U8C_Sat) mlib_VectorDotProd_U8C_Sat
	__attribute__((weak, alias("__mlib_VectorDotProd_U8C_Sat")));
__typeof__(__mlib_VectorDotProd_S8_Sat) mlib_VectorDotProd_S8_Sat
	__attribute__((weak, alias("__mlib_VectorDotProd_S8_Sat")));
__typeof__(__mlib_VectorDotProd_U8_Sat) mlib_VectorDotProd_U8_Sat
	__attribute__((weak, alias("__mlib_VectorDotProd_U8_Sat")));
__typeof__(__mlib_VectorDotProd_S8C_Sat) mlib_VectorDotProd_S8C_Sat
	__attribute__((weak, alias("__mlib_VectorDotProd_S8C_Sat")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	VECTOR_DOTPROD_U8(ld)				\
	xbuf = _mm_load_si128((__m128i *)px);		\
	ybuf = _mm_##ld##_si128((__m128i *)py);		\
	buf1 = _mm_unpacklo_epi8(xbuf, zero);		\
	buf2 = _mm_unpackhi_epi8(xbuf, zero);		\
	buf3 = _mm_unpacklo_epi8(ybuf, zero);		\
	buf4 = _mm_unpackhi_epi8(ybuf, zero);		\
	buf1 = _mm_madd_epi16(buf1, buf3);		\
	buf2 = _mm_madd_epi16(buf2, buf4);		\
	zbuf32 = _mm_add_epi32(zbuf32, buf1);		\
	zbuf32 = _mm_add_epi32(zbuf32, buf2);		\
	px += nstep;					\
	py += nstep

mlib_status
__mlib_VectorDotProd_U8_Sat(
	mlib_d64 *z,
	const mlib_u8 *x,
	const mlib_u8 *y,
	mlib_s32 n)
{
	if (n <= 0)
		return (MLIB_FAILURE);

	mlib_s32 i, ax, ay, nstep, n1, n2, n3, sum = 0;
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
			sum += (mlib_s32)(*px++) * (*py++);
		}

		mlib_s32 nblock = n2 >> 12;
		mlib_s32 tail = n2 & 4095;
		mlib_s32 k;

		if (ax == ay) {
			for (k = 0; k < nblock; k++) {
				zbuf32 = zero;
				for (i = 0; i < 4096; i++) {
					VECTOR_DOTPROD_U8(load);
				}
				buf1 = _mm_unpacklo_epi32(zbuf32, zero);
				buf2 = _mm_unpackhi_epi32(zbuf32, zero);
				zbuf64 = _mm_add_epi64(zbuf64, buf1);
				zbuf64 = _mm_add_epi64(zbuf64, buf2);
			}
			zbuf32 = zero;
			for (i = 0; i < tail; i++) {
				VECTOR_DOTPROD_U8(load);
			}
		} else {
			for (k = 0; k < nblock; k++) {
				zbuf32 = zero;
				for (i = 0; i < 4096; i++) {
					VECTOR_DOTPROD_U8(loadu);
				}
				buf1 = _mm_unpacklo_epi32(zbuf32, zero);
				buf2 = _mm_unpackhi_epi32(zbuf32, zero);
				zbuf64 = _mm_add_epi64(zbuf64, buf1);
				zbuf64 = _mm_add_epi64(zbuf64, buf2);
			}
			zbuf32 = zero;
			for (i = 0; i < tail; i++) {
				VECTOR_DOTPROD_U8(loadu);
			}
		}
		buf1 = _mm_unpacklo_epi32(zbuf32, zero);
		buf2 = _mm_unpackhi_epi32(zbuf32, zero);
		zbuf64 = _mm_add_epi64(zbuf64, buf1);
		zbuf64 = _mm_add_epi64(zbuf64, buf2);

		for (i = 0; i < n3; i++) {
			sum += (mlib_s32)(*px++) * (*py++);
		}

		mlib_d64 dsum = sum;
		long long pz[2];
		_mm_storeu_si128((__m128i *)pz, zbuf64);
		dsum += pz[0];
		dsum += pz[1];
		*z = dsum;
	} else {
		for (i = 0; i < n; i++) {
			sum += (mlib_s32)(*px++) * (*py++);
		}
		*z = (mlib_d64)sum;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#define	VECTOR_DOTPROD_S8(ld)				\
	xbuf = _mm_load_si128((__m128i *)px);		\
	ybuf = _mm_##ld##_si128((__m128i *)py);		\
	msk1 = _mm_cmpgt_epi8(zero, xbuf);		\
	msk2 = _mm_cmpgt_epi8(zero, ybuf);		\
	buf1 = _mm_unpacklo_epi8(xbuf, msk1);		\
	buf2 = _mm_unpackhi_epi8(xbuf, msk1);		\
	buf3 = _mm_unpacklo_epi8(ybuf, msk2);		\
	buf4 = _mm_unpackhi_epi8(ybuf, msk2);		\
	buf1 = _mm_madd_epi16(buf1, buf3);		\
	buf2 = _mm_madd_epi16(buf2, buf4);		\
	zbuf32 = _mm_add_epi32(zbuf32, buf1);		\
	zbuf32 = _mm_add_epi32(zbuf32, buf2);		\
	px += nstep;					\
	py += nstep

mlib_status
__mlib_VectorDotProd_S8_Sat(
	mlib_d64 *z,
	const mlib_s8 *x,
	const mlib_s8 *y,
	mlib_s32 n)
{
	if (n <= 0)
		return (MLIB_FAILURE);

	mlib_s32 i, ax, ay, nstep, n1, n2, n3, sum = 0;
	const mlib_s8 *px = x, *py = y;
	__m128i zero, xbuf, ybuf, zbuf32, zbuf64, buf1, buf2, buf3, buf4;
	__m128i msk1, msk2, mask;
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
			sum += (mlib_s32)(*px++) * (*py++);
		}

		mlib_s32 nblock = n2 >> 12;
		mlib_s32 tail = n2 & 4095;
		mlib_s32 k;

		if (ax == ay) {
			for (k = 0; k < nblock; k++) {
				zbuf32 = zero;
				for (i = 0; i < 4096; i++) {
					VECTOR_DOTPROD_S8(load);
				}
				mask = _mm_cmpgt_epi32(zero, zbuf32);
				buf1 = _mm_unpacklo_epi32(zbuf32, mask);
				buf2 = _mm_unpackhi_epi32(zbuf32, mask);
				zbuf64 = _mm_add_epi64(zbuf64, buf1);
				zbuf64 = _mm_add_epi64(zbuf64, buf2);
			}
			zbuf32 = zero;
			for (i = 0; i < tail; i++) {
				VECTOR_DOTPROD_S8(load);
			}
		} else {
			for (k = 0; k < nblock; k++) {
				zbuf32 = zero;
				for (i = 0; i < 4096; i++) {
					VECTOR_DOTPROD_S8(loadu);
				}
				mask = _mm_cmpgt_epi32(zero, zbuf32);
				buf1 = _mm_unpacklo_epi32(zbuf32, mask);
				buf2 = _mm_unpackhi_epi32(zbuf32, mask);
				zbuf64 = _mm_add_epi64(zbuf64, buf1);
				zbuf64 = _mm_add_epi64(zbuf64, buf2);
			}
			zbuf32 = zero;
			for (i = 0; i < tail; i++) {
				VECTOR_DOTPROD_S8(loadu);
			}
		}
		mask = _mm_cmpgt_epi32(zero, zbuf32);
		buf1 = _mm_unpacklo_epi32(zbuf32, mask);
		buf2 = _mm_unpackhi_epi32(zbuf32, mask);
		zbuf64 = _mm_add_epi64(zbuf64, buf1);
		zbuf64 = _mm_add_epi64(zbuf64, buf2);

		for (i = 0; i < n3; i++) {
			sum += (mlib_s32)(*px++) * (*py++);
		}

		mlib_d64 dsum = sum;
		long long pz[2];
		_mm_storeu_si128((__m128i *)pz, zbuf64);
		dsum += pz[0];
		dsum += pz[1];
		*z = dsum;
	} else {
		for (i = 0; i < n; i++) {
			sum += (mlib_s32)(*px++) * (*py++);
		}
		*z = (mlib_d64)sum;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#define	VECTOR_DOTPROD_U8C(ld1, ld2)			\
	xbuf = _mm_##ld1##_si128((__m128i *)px);	\
	ybuf = _mm_##ld2##_si128((__m128i *)py);	\
							\
	rbuf = _mm_and_si128(xbuf, mask1);		\
	ibuf = _mm_srli_epi16(xbuf, 8);			\
	buf1 = _mm_unpacklo_epi8(ybuf, zero);		\
	buf2 = _mm_unpackhi_epi8(ybuf, zero);		\
							\
	buf3 = _mm_unpacklo_epi16(rbuf, ibuf);		\
	buf4 = _mm_unpackhi_epi16(rbuf, ibuf);		\
	buf5 = _mm_madd_epi16(buf1, buf3);		\
	buf6 = _mm_madd_epi16(buf2, buf4);		\
	xbuf = _mm_add_epi32(buf5, buf6);		\
							\
	rbuf = _mm_sub_epi16(zero, rbuf);		\
	buf3 = _mm_unpacklo_epi16(ibuf, rbuf);		\
	buf4 = _mm_unpackhi_epi16(ibuf, rbuf);		\
	buf5 = _mm_madd_epi16(buf1, buf3);		\
	buf6 = _mm_madd_epi16(buf2, buf4);		\
	ybuf = _mm_add_epi32(buf5, buf6);		\
							\
	buf1 = _mm_unpacklo_epi32(xbuf, ybuf);		\
	buf2 = _mm_unpackhi_epi32(xbuf, ybuf);		\
	zbuf32 = _mm_add_epi32(zbuf32, buf1);		\
	zbuf32 = _mm_add_epi32(zbuf32, buf2);		\
	px += nstep;					\
	py += nstep

mlib_status
__mlib_VectorDotProd_U8C_Sat(
	mlib_d64 *z,
	const mlib_u8 *x,
	const mlib_u8 *y,
	mlib_s32 n)
{
	if (n <= 0)
		return (MLIB_FAILURE);

	mlib_s32 i, ax, ay, nstep, n1, n2, n3, sum0 = 0, sum1 = 0;
	const mlib_u8 *px = x, *py = y;
	__m128i zero, xbuf, ybuf, rbuf, ibuf, zbuf32, zbuf64;
	__m128i buf1, buf2, buf3, buf4, buf5, buf6, mask1, mask;
	zero = _mm_setzero_si128();
	mask1 = _mm_set1_epi16(0x00ff);
	zbuf64 = zero;

	ax = (mlib_addr)x & 15;
	nstep = 16 / sizeof (mlib_u8);
	n1 = ((16 - ax) & 15) / sizeof (mlib_u8);
	n2 = (n + n - n1) / nstep;
	n3 = n + n - n1 - n2 * nstep;

	if ((ax & 1) == 0) {
		if (n2 > 0) {
			for (i = 0; i < n1 / 2; i++) {
				sum0 += (mlib_s32)px[0] * py[0] + px[1] * py[1];
				sum1 += (mlib_s32)px[1] * py[0] - px[0] * py[1];
				px += 2;
				py += 2;
			}

			mlib_s32 nblock = n2 >> 12;
			mlib_s32 tail = n2 & 4095;
			mlib_s32 k;

			for (k = 0; k < nblock; k++) {
				zbuf32 = zero;
				for (i = 0; i < 4096; i++) {
					VECTOR_DOTPROD_U8C(load, loadu);
				}
				mask = _mm_cmpgt_epi32(zero, zbuf32);
				buf1 = _mm_unpacklo_epi32(zbuf32, mask);
				buf2 = _mm_unpackhi_epi32(zbuf32, mask);
				zbuf64 = _mm_add_epi64(zbuf64, buf1);
				zbuf64 = _mm_add_epi64(zbuf64, buf2);
			}
			zbuf32 = zero;
			for (i = 0; i < tail; i++) {
				VECTOR_DOTPROD_U8C(load, loadu);
			}
			mask = _mm_cmpgt_epi32(zero, zbuf32);
			buf1 = _mm_unpacklo_epi32(zbuf32, mask);
			buf2 = _mm_unpackhi_epi32(zbuf32, mask);
			zbuf64 = _mm_add_epi64(zbuf64, buf1);
			zbuf64 = _mm_add_epi64(zbuf64, buf2);

			for (i = 0; i < n3 / 2; i++) {
				sum0 += (mlib_s32)px[0] * py[0] + px[1] * py[1];
				sum1 += (mlib_s32)px[1] * py[0] - px[0] * py[1];
				px += 2;
				py += 2;
			}

			long long pz[2];
			_mm_storeu_si128((__m128i *)pz, zbuf64);
			z[0] = (mlib_d64)sum0 + pz[0];
			z[1] = (mlib_d64)sum1 + pz[1];
		} else {
			for (i = 0; i < n; i++) {
				sum0 += (mlib_s32)px[0] * py[0] + px[1] * py[1];
				sum1 += (mlib_s32)px[1] * py[0] - px[0] * py[1];
				px += 2;
				py += 2;
			}
			z[0] = (mlib_d64)sum0;
			z[1] = (mlib_d64)sum1;
		}
	} else {

		n2 = (n + n) / nstep;
		n3 = n + n - n2 * nstep;

		mlib_s32 nblock = n2 >> 12;
		mlib_s32 tail = n2 & 4095;
		mlib_s32 k;

		for (k = 0; k < nblock; k++) {
			zbuf32 = zero;
			for (i = 0; i < 4096; i++) {
				VECTOR_DOTPROD_U8C(loadu, loadu);
			}
			mask = _mm_cmpgt_epi32(zero, zbuf32);
			buf1 = _mm_unpacklo_epi32(zbuf32, mask);
			buf2 = _mm_unpackhi_epi32(zbuf32, mask);
			zbuf64 = _mm_add_epi64(zbuf64, buf1);
			zbuf64 = _mm_add_epi64(zbuf64, buf2);
		}
		zbuf32 = zero;
		for (i = 0; i < tail; i++) {
			VECTOR_DOTPROD_U8C(loadu, loadu);
		}
		mask = _mm_cmpgt_epi32(zero, zbuf32);
		buf1 = _mm_unpacklo_epi32(zbuf32, mask);
		buf2 = _mm_unpackhi_epi32(zbuf32, mask);
		zbuf64 = _mm_add_epi64(zbuf64, buf1);
		zbuf64 = _mm_add_epi64(zbuf64, buf2);

		for (i = 0; i < n3 / 2; i++) {
			sum0 += (mlib_s32)px[0] * py[0] + px[1] * py[1];
			sum1 += (mlib_s32)px[1] * py[0] - px[0] * py[1];
			px += 2;
			py += 2;
		}

		long long pz[2];
		_mm_storeu_si128((__m128i *)pz, zbuf64);
		z[0] = (mlib_d64)sum0 + pz[0];
		z[1] = (mlib_d64)sum1 + pz[1];
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#define	VECTOR_DOTPROD_S8C(ld1, ld2)			\
	xbuf = _mm_##ld1##_si128((__m128i *)px);	\
	ybuf = _mm_##ld2##_si128((__m128i *)py);	\
							\
	rbuf = _mm_slli_epi16(xbuf, 8);			\
	mask = _mm_cmpgt_epi8(zero, ybuf);		\
	rbuf = _mm_srai_epi16(rbuf, 8);			\
	ibuf = _mm_srai_epi16(xbuf, 8);			\
	buf1 = _mm_unpacklo_epi8(ybuf, mask);		\
	buf2 = _mm_unpackhi_epi8(ybuf, mask);		\
							\
	buf3 = _mm_unpacklo_epi16(rbuf, ibuf);		\
	buf4 = _mm_unpackhi_epi16(rbuf, ibuf);		\
	buf5 = _mm_madd_epi16(buf1, buf3);		\
	buf6 = _mm_madd_epi16(buf2, buf4);		\
	xbuf = _mm_add_epi32(buf5, buf6);		\
							\
	rbuf = _mm_sub_epi16(zero, rbuf);		\
	buf3 = _mm_unpacklo_epi16(ibuf, rbuf);		\
	buf4 = _mm_unpackhi_epi16(ibuf, rbuf);		\
	buf5 = _mm_madd_epi16(buf1, buf3);		\
	buf6 = _mm_madd_epi16(buf2, buf4);		\
	ybuf = _mm_add_epi32(buf5, buf6);		\
							\
	buf1 = _mm_unpacklo_epi32(xbuf, ybuf);		\
	buf2 = _mm_unpackhi_epi32(xbuf, ybuf);		\
	zbuf32 = _mm_add_epi32(zbuf32, buf1);		\
	zbuf32 = _mm_add_epi32(zbuf32, buf2);		\
	px += nstep;					\
	py += nstep

mlib_status
__mlib_VectorDotProd_S8C_Sat(
	mlib_d64 *z,
	const mlib_s8 *x,
	const mlib_s8 *y,
	mlib_s32 n)
{
	if (n <= 0)
		return (MLIB_FAILURE);

	mlib_s32 i, ax, ay, nstep, n1, n2, n3, sum0 = 0, sum1 = 0;
	const mlib_s8 *px = x, *py = y;
	__m128i zero, xbuf, ybuf, rbuf, ibuf, zbuf32, zbuf64;
	__m128i buf1, buf2, buf3, buf4, buf5, buf6, mask;
	zero = _mm_setzero_si128();
	zbuf64 = zero;

	ax = (mlib_addr)x & 15;
	nstep = 16 / sizeof (mlib_s8);
	n1 = ((16 - ax) & 15) / sizeof (mlib_s8);
	n2 = (n + n - n1) / nstep;
	n3 = n + n - n1 - n2 * nstep;

	if ((ax & 1) == 0) {
		if (n2 > 0) {
			for (i = 0; i < n1 / 2; i++) {
				sum0 += (mlib_s32)px[0] * py[0] + px[1] * py[1];
				sum1 += (mlib_s32)px[1] * py[0] - px[0] * py[1];
				px += 2;
				py += 2;
			}

			mlib_s32 nblock = n2 >> 12;
			mlib_s32 tail = n2 & 4095;
			mlib_s32 k;

			for (k = 0; k < nblock; k++) {
				zbuf32 = zero;
				for (i = 0; i < 4096; i++) {
					VECTOR_DOTPROD_S8C(load, loadu);
				}
				mask = _mm_cmpgt_epi32(zero, zbuf32);
				buf1 = _mm_unpacklo_epi32(zbuf32, mask);
				buf2 = _mm_unpackhi_epi32(zbuf32, mask);
				zbuf64 = _mm_add_epi64(zbuf64, buf1);
				zbuf64 = _mm_add_epi64(zbuf64, buf2);
			}
			zbuf32 = zero;
			for (i = 0; i < tail; i++) {
				VECTOR_DOTPROD_S8C(load, loadu);
			}
			mask = _mm_cmpgt_epi32(zero, zbuf32);
			buf1 = _mm_unpacklo_epi32(zbuf32, mask);
			buf2 = _mm_unpackhi_epi32(zbuf32, mask);
			zbuf64 = _mm_add_epi64(zbuf64, buf1);
			zbuf64 = _mm_add_epi64(zbuf64, buf2);

			for (i = 0; i < n3 / 2; i++) {
				sum0 += (mlib_s32)px[0] * py[0] + px[1] * py[1];
				sum1 += (mlib_s32)px[1] * py[0] - px[0] * py[1];
				px += 2;
				py += 2;
			}

			long long pz[2];
			_mm_storeu_si128((__m128i *)pz, zbuf64);
			z[0] = (mlib_d64)sum0 + pz[0];
			z[1] = (mlib_d64)sum1 + pz[1];
		} else {
			for (i = 0; i < n; i++) {
				sum0 += (mlib_s32)px[0] * py[0] + px[1] * py[1];
				sum1 += (mlib_s32)px[1] * py[0] - px[0] * py[1];
				px += 2;
				py += 2;
			}
			z[0] = (mlib_d64)sum0;
			z[1] = (mlib_d64)sum1;
		}
	} else {

		n2 = (n + n) / nstep;
		n3 = n + n - n2 * nstep;

		mlib_s32 nblock = n2 >> 12;
		mlib_s32 tail = n2 & 4095;
		mlib_s32 k;

		for (k = 0; k < nblock; k++) {
			zbuf32 = zero;
			for (i = 0; i < 4096; i++) {
				VECTOR_DOTPROD_S8C(loadu, loadu);
			}
			mask = _mm_cmpgt_epi32(zero, zbuf32);
			buf1 = _mm_unpacklo_epi32(zbuf32, mask);
			buf2 = _mm_unpackhi_epi32(zbuf32, mask);
			zbuf64 = _mm_add_epi64(zbuf64, buf1);
			zbuf64 = _mm_add_epi64(zbuf64, buf2);
		}
		zbuf32 = zero;
		for (i = 0; i < tail; i++) {
			VECTOR_DOTPROD_S8C(loadu, loadu);
		}
		mask = _mm_cmpgt_epi32(zero, zbuf32);
		buf1 = _mm_unpacklo_epi32(zbuf32, mask);
		buf2 = _mm_unpackhi_epi32(zbuf32, mask);
		zbuf64 = _mm_add_epi64(zbuf64, buf1);
		zbuf64 = _mm_add_epi64(zbuf64, buf2);

		for (i = 0; i < n3 / 2; i++) {
			sum0 += (mlib_s32)px[0] * py[0] + px[1] * py[1];
			sum1 += (mlib_s32)px[1] * py[0] - px[0] * py[1];
			px += 2;
			py += 2;
		}

		long long pz[2];
		_mm_storeu_si128((__m128i *)pz, zbuf64);
		z[0] = (mlib_d64)sum0 + pz[0];
		z[1] = (mlib_d64)sum1 + pz[1];
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
