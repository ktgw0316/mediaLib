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

#pragma ident	"@(#)mlib_s_VectorSplit.c	9.3	07/11/05 SMI"

/*
 * FUNCTIONS
 *      mlib_VectorSplit_U8_U8C    - split an 8-bit format vector
 *      mlib_VectorSplit_S8_S8C    - split an 8-bit format vector
 *      mlib_VectorSplit_S16_S16C  - split a 16-bit format vector
 *      mlib_VectorSplit_S32_S32C  - split a 32-bit format vector
 *
 * SYNOPSIS
 *      mlib_status mlib_VectorSplit_S8_S8C(mlib_s8       * r,
 *                                          mlib_s8       * i,
 *                                          const mlib_s8 * x,
 *                                          mlib_s32      n);
 *      mlib_status mlib_VectorSplit_U8_U8C(mlib_u8       * r,
 *                                          mlib_u8       * i,
 *                                          const mlib_u8 * x,
 *                                          mlib_s32      n);
 *      mlib_status mlib_VectorSplit_S16_S16C(mlib_s16       * r,
 *                                            mlib_s16       * i,
 *                                            const mlib_s16 * x,
 *                                            mlib_s32       n);
 *      mlib_status mlib_VectorSplit_S32_S32C(mlib_s32       * r,
 *                                            mlib_s32       * i,
 *                                            const mlib_s32 * x,
 *                                            mlib_s32       n);
 *
 * ARGUMENTS
 *      r    pointer to the first element of the real part.
 *      i    pointer to the first element of the imaginary part.
 *      x    pointer to the first element of the Complex input vector.
 *           x[2*i] holds the real part, and x[2*i + 1] holds
 *           the imaginary parts.
 *      n    number of elements in the vectors.
 *
 * DESCRIPTION
 *
 *      X = R + J*I
 *      X = (r1 + j*i1 r2 + j*i2 ... rn + j*in)
 *
 *      R = (r1 r2 ... rn)
 *      I = (i1 i2 ... in)
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

#pragma weak mlib_VectorSplit_S8_S8C = __mlib_VectorSplit_S8_S8C
#pragma weak mlib_VectorSplit_S32_S32C = __mlib_VectorSplit_S32_S32C
#pragma weak mlib_VectorSplit_U8_U8C = __mlib_VectorSplit_U8_U8C
#pragma weak mlib_VectorSplit_S16_S16C = __mlib_VectorSplit_S16_S16C

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */

__typeof__(__mlib_VectorSplit_S8_S8C) mlib_VectorSplit_S8_S8C
	__attribute__((weak, alias("__mlib_VectorSplit_S8_S8C")));
__typeof__(__mlib_VectorSplit_S32_S32C) mlib_VectorSplit_S32_S32C
	__attribute__((weak, alias("__mlib_VectorSplit_S32_S32C")));
__typeof__(__mlib_VectorSplit_U8_U8C) mlib_VectorSplit_U8_U8C
	__attribute__((weak, alias("__mlib_VectorSplit_U8_U8C")));
__typeof__(__mlib_VectorSplit_S16_S16C) mlib_VectorSplit_S16_S16C
	__attribute__((weak, alias("__mlib_VectorSplit_S16_S16C")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	SPLIT(format, z, n, out_r, out_i)                       \
	{                                                       \
	    format *dst_r = (format *) out_r;                   \
	    format *dst_i = (format *) out_i;                   \
	    format *src = (format *) z;                         \
	    mlib_s32 j;                                         \
	                                                        \
	    for (j = 0; j < n; j++) {                           \
		(*dst_r++) = (*src++);                          \
		(*dst_i++) = (*src++);                          \
	    }                                                   \
	    return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);     \
	}

/* *********************************************************** */

#ifndef _LITTLE_ENDIAN

#define	MERGE_U8_S32(dst32, b1, b2, b3, b4)                         \
	(dst32) = ((b1) << 24) + ((b2) << 16) + ((b3) << 8) + b4;

#else /* _LITTLE_ENDIAN */

#define	MERGE_U8_S32(dst32, b1, b2, b3, b4)                         \
	(dst32) = ((b4) << 24) + ((b3) << 16) + ((b2) << 8) + b1;

#endif /* _LITTLE_ENDIAN */

/* *********************************************************** */

#define	SPLIT_U8(ld, st1, st2)				\
	buf1 = _mm_##ld##_si128((__m128i *)px);		\
	px += nstep;					\
	buf2 = _mm_##ld##_si128((__m128i *)px);		\
	px += nstep;					\
							\
	rbuf = _mm_unpacklo_epi8(buf1, buf2);		\
	ibuf = _mm_unpackhi_epi8(buf1, buf2);		\
	buf1 = _mm_unpacklo_epi8(rbuf, ibuf);		\
	buf2 = _mm_unpackhi_epi8(rbuf, ibuf);		\
							\
	rbuf = _mm_unpacklo_epi8(buf1, buf2);		\
	ibuf = _mm_unpackhi_epi8(buf1, buf2);		\
	buf1 = _mm_unpacklo_epi8(rbuf, ibuf);		\
	buf2 = _mm_unpackhi_epi8(rbuf, ibuf);		\
							\
	_mm_##st1##_si128((__m128i *)pr, buf1);		\
	_mm_##st2##_si128((__m128i *)pi, buf2);		\
	pr += nstep;					\
	pi += nstep

mlib_status
__mlib_VectorSplit_U8_U8C(
	mlib_u8 *r,
	mlib_u8 *i,
	const mlib_u8 *x,
	mlib_s32 n)
{
	if (n < 1)
		return (MLIB_FAILURE);

	mlib_s32 k, ar, ai, ax, nstep, n1, n2, n3;
	mlib_u8 *pr, *pi, *px;
	__m128i rbuf, ibuf, buf1, buf2;

	ax = (mlib_addr)x & 15;

	if (ax & 1) {
		r[0] = x[0];
		i[n - 1] = x[2 * n -1];
		pr = (mlib_u8 *)i;
		pi = (mlib_u8 *)r + 1;
		px = (mlib_u8 *)x + 1;
		ax = (ax + 1) & 15;
		n--;
	} else {
		pr = (mlib_u8 *)r;
		pi = (mlib_u8 *)i;
		px = (mlib_u8 *)x;
	}

	ar = (mlib_addr)pr & 15;
	ai = (mlib_addr)pi & 15;

	nstep = 16 / sizeof (mlib_u8);
	n1 = ((16 - ax) & 15) / sizeof (mlib_u8);
	n2 = (n + n - n1) / nstep / 2;
	n3 = n + n - n1 - n2 * nstep * 2;

	if (n2 < 1) {
		for (k = 0; k < n; k++) {
			*pr++ = px[0];
			*pi++ = px[1];
			px += 2;
		}
	} else {
		for (k = 0; k < n1 / 2; k++) {
			*pr++ = px[0];
			*pi++ = px[1];
			px += 2;
		}
		for (k = 0; k < n2; k++) {
			SPLIT_U8(load, storeu, storeu);
		}
		for (k = 0; k < n3 / 2; k++) {
			*pr++ = px[0];
			*pi++ = px[1];
			px += 2;
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VectorSplit_S8_S8C(
	mlib_s8 *r,
	mlib_s8 *i,
	const mlib_s8 *x,
	mlib_s32 n)
{
	return __mlib_VectorSplit_U8_U8C((mlib_u8 *)r, (mlib_u8 *)i,
		(mlib_u8 *)x, n);
}

/* *********************************************************** */

#define	SPLIT_S16(ld, st1, st2)				\
	buf1 = _mm_##ld##_si128((__m128i *)px);		\
	px += nstep;					\
	buf2 = _mm_##ld##_si128((__m128i *)px);		\
	px += nstep;					\
							\
	rbuf = _mm_unpacklo_epi16(buf1, buf2);		\
	ibuf = _mm_unpackhi_epi16(buf1, buf2);		\
	buf1 = _mm_unpacklo_epi16(rbuf, ibuf);		\
	buf2 = _mm_unpackhi_epi16(rbuf, ibuf);		\
							\
	rbuf = _mm_unpacklo_epi16(buf1, buf2);		\
	ibuf = _mm_unpackhi_epi16(buf1, buf2);		\
							\
	_mm_##st1##_si128((__m128i *)pr, rbuf);		\
	_mm_##st2##_si128((__m128i *)pi, ibuf);		\
	pr += nstep;					\
	pi += nstep

mlib_status
__mlib_VectorSplit_S16_S16C(
	mlib_s16 *r,
	mlib_s16 *i,
	const mlib_s16 *x,
	mlib_s32 n)
{
	if (n < 1)
		return (MLIB_FAILURE);

	mlib_s32 k, ar, ai, ax, nstep, n1, n2, n3;
	mlib_s16 *pr, *pi, *px;
	__m128i rbuf, ibuf, buf1, buf2;

	ax = (mlib_addr)x & 15;

	if (ax & 3) {
		r[0] = x[0];
		i[n - 1] = x[2 * n -1];
		pr = (mlib_s16 *)i;
		pi = (mlib_s16 *)r + 1;
		px = (mlib_s16 *)x + 1;
		ax = (ax + 1) & 15;
		n--;
	} else {
		pr = (mlib_s16 *)r;
		pi = (mlib_s16 *)i;
		px = (mlib_s16 *)x;
	}

	ar = (mlib_addr)pr & 15;
	ai = (mlib_addr)pi & 15;

	nstep = 16 / sizeof (mlib_s16);
	n1 = ((16 - ax) & 15) / sizeof (mlib_s16);
	n2 = (n + n - n1) / nstep / 2;
	n3 = n + n - n1 - n2 * nstep * 2;

	if (n2 < 1) {
		for (k = 0; k < n; k++) {
			*pr++ = px[0];
			*pi++ = px[1];
			px += 2;
		}
	} else {
		for (k = 0; k < n1 / 2; k++) {
			*pr++ = px[0];
			*pi++ = px[1];
			px += 2;
		}
		for (k = 0; k < n2; k++) {
			SPLIT_S16(load, storeu, storeu);
		}
		for (k = 0; k < n3 / 2; k++) {
			*pr++ = px[0];
			*pi++ = px[1];
			px += 2;
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#define	SPLIT_S32(ld, st1, st2)				\
	buf1 = _mm_##ld##_si128((__m128i *)px);		\
	px += nstep;					\
	buf2 = _mm_##ld##_si128((__m128i *)px);		\
	px += nstep;					\
							\
	rbuf = _mm_unpacklo_epi32(buf1, buf2);		\
	ibuf = _mm_unpackhi_epi32(buf1, buf2);		\
	buf1 = _mm_unpacklo_epi32(rbuf, ibuf);		\
	buf2 = _mm_unpackhi_epi32(rbuf, ibuf);		\
							\
	_mm_##st1##_si128((__m128i *)pr, buf1);		\
	_mm_##st2##_si128((__m128i *)pi, buf2);		\
	pr += nstep;					\
	pi += nstep

mlib_status
__mlib_VectorSplit_S32_S32C(
	mlib_s32 *r,
	mlib_s32 *i,
	const mlib_s32 *x,
	mlib_s32 n)
{
	if (n < 1)
		return (MLIB_FAILURE);

	mlib_s32 k, ar, ai, ax, nstep, n1, n2, n3;
	mlib_s32 *pr, *pi, *px;
	__m128i rbuf, ibuf, buf1, buf2;

	ax = (mlib_addr)x & 15;

	if (ax & 7) {
		r[0] = x[0];
		i[n - 1] = x[2 * n -1];
		pr = (mlib_s32 *)i;
		pi = (mlib_s32 *)r + 1;
		px = (mlib_s32 *)x + 1;
		ax = (ax + 1) & 15;
		n--;
	} else {
		pr = (mlib_s32 *)r;
		pi = (mlib_s32 *)i;
		px = (mlib_s32 *)x;
	}

	ar = (mlib_addr)pr & 15;
	ai = (mlib_addr)pi & 15;

	nstep = 16 / sizeof (mlib_s32);
	n1 = ((16 - ax) & 15) / sizeof (mlib_s32);
	n2 = (n + n - n1) / nstep / 2;
	n3 = n + n - n1 - n2 * nstep * 2;

	if (n2 < 1) {
		for (k = 0; k < n; k++) {
			*pr++ = px[0];
			*pi++ = px[1];
			px += 2;
		}
	} else {
		for (k = 0; k < n1 / 2; k++) {
			*pr++ = px[0];
			*pi++ = px[1];
			px += 2;
		}
		for (k = 0; k < n2; k++) {
			SPLIT_S32(load, storeu, storeu);
		}
		for (k = 0; k < n3 / 2; k++) {
			*pr++ = px[0];
			*pi++ = px[1];
			px += 2;
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
