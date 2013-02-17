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

#pragma ident	"@(#)mlib_s_VectorMerge.c	9.5	07/11/05 SMI"

/*
 * FUNCTIONS
 *      mlib_VectorMerge_U8C_U8    - merge two  8-bit format vectors
 *      mlib_VectorMerge_S8C_S8    - merge two  8-bit format vectors
 *      mlib_VectorMerge_S16C_S16  - merge two 16-bit format vectors
 *      mlib_VectorMerge_S32C_S32  - merge two 32-bit format vectors
 *
 * SYNOPSIS
 *      mlib_status mlib_VectorMerge_U8C_U8(mlib_u8       *z,
 *                                          const mlib_u8 *r,
 *                                          const mlib_u8 *i,
 *                                          mlib_s32      n);
 *      mlib_status mlib_VectorMerge_S8C_S8(mlib_s8       *z,
 *                                          const mlib_s8 *r,
 *                                          const mlib_s8 *i,
 *                                          mlib_s32      n);
 *      mlib_status mlib_VectorMerge_S16C_S16(mlib_s16       *z,
 *                                            const mlib_s16 *r,
 *                                            const mlib_s16 *i,
 *                                            mlib_s32       n);
 *      mlib_status mlib_VectorMerge_S32C_S32(mlib_s32       *z,
 *                                            const mlib_s32 *r,
 *                                            const mlib_s32 *i,
 *                                            mlib_s32       n);
 *
 * ARGUMENTS
 *      z    pointer to the first element of the resultant vector
 *      r    pointer to the first element of the 1 input vector (real parts)
 *      i    pointer to the first element of the 2 input vector (imag. parts)
 *      n    number of elements in the vectors
 *
 * DESCRIPTION
 *
 *      Z = R + J*I
 *
 *      R = (r1 r2 ... rn)
 *      I = (i1 i2 ... in)
 *      Z = (r1 + j*i1 r2 + j*i2 ... rn + j*in)
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

#pragma weak mlib_VectorMerge_U8C_U8 = __mlib_VectorMerge_U8C_U8
#pragma weak mlib_VectorMerge_S16C_S16 = __mlib_VectorMerge_S16C_S16
#pragma weak mlib_VectorMerge_S8C_S8 = __mlib_VectorMerge_U8C_U8
#pragma weak mlib_VectorMerge_S32C_S32 = __mlib_VectorMerge_S32C_S32
#pragma weak __mlib_VectorMerge_S8C_S8 = __mlib_VectorMerge_U8C_U8

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */

__typeof__(__mlib_VectorMerge_U8C_U8) mlib_VectorMerge_U8C_U8
	__attribute__((weak, alias("__mlib_VectorMerge_U8C_U8")));
__typeof__(__mlib_VectorMerge_S16C_S16) mlib_VectorMerge_S16C_S16
	__attribute__((weak, alias("__mlib_VectorMerge_S16C_S16")));
__typeof__(__mlib_VectorMerge_U8C_U8) mlib_VectorMerge_S8C_S8
	__attribute__((weak, alias("__mlib_VectorMerge_U8C_U8")));
__typeof__(__mlib_VectorMerge_S32C_S32) mlib_VectorMerge_S32C_S32
	__attribute__((weak, alias("__mlib_VectorMerge_S32C_S32")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

#if defined(_MSC_VER) || defined(__GNUC__)

mlib_status __mlib_VectorMerge_S8C_S8(
    mlib_s8 *z,
    const mlib_s8 *r,
    const mlib_s8 *i,
    mlib_s32 n)
{
	return (__mlib_VectorMerge_U8C_U8(z, r, i, n));
}

#endif /* _MSC_VER || __GNUC__ */

/* *********************************************************** */

#define	MERGE_U8(ld1, ld2, st)				\
	rbuf = _mm_##ld1##_si128((__m128i *)pr);	\
	ibuf = _mm_##ld2##_si128((__m128i *)pi);	\
	buf1 = _mm_unpacklo_epi8(rbuf, ibuf);		\
	buf2 = _mm_unpackhi_epi8(rbuf, ibuf);		\
	_mm_##st##_si128((__m128i *)pz, buf1);		\
	pz += nstep;					\
	_mm_##st##_si128((__m128i *)pz, buf2);		\
	pz += nstep;					\
	pr += nstep;					\
	pi += nstep

mlib_status
__mlib_VectorMerge_U8C_U8(
	mlib_u8 *z,
	const mlib_u8 *r,
	const mlib_u8 *i,
	mlib_s32 n)
{
	if (n < 1)
		return (MLIB_FAILURE);

	mlib_s32 k, ar, ai, az, nstep, n1, n2, n3;
	mlib_u8 *pr = (mlib_u8 *)r, *pi = (mlib_u8 *)i, *pz = (mlib_u8 *)z;
	__m128i rbuf, ibuf, buf1, buf2;

	ar = (mlib_addr)r & 15;
	ai = (mlib_addr)i & 15;
	az = (mlib_addr)z & 15;

	nstep = 16 / sizeof (mlib_u8);
	n1 = ((16 - ar) & 15) / sizeof (mlib_u8);
	n2 = (n - n1) / nstep;
	n3 = n - n1 - n2 * nstep;

	if (n2 < 1) {
		for (k = 0; k < n; k++) {
			pz[0] = *pr++;
			pz[1] = *pi++;
			pz += 2;
		}
	} else {
		for (k = 0; k < n1; k++) {
			pz[0] = *pr++;
			pz[1] = *pi++;
			pz += 2;
		}

		if (ar == ai && (ar * 2 & 15) == az) {
			for (k = 0; k < n2; k++) {
				MERGE_U8(load, load, store);
			}
		} else if (ar == ai) {
			for (k = 0; k < n2; k++) {
				MERGE_U8(load, load, storeu);
			}
		} else {
			for (k = 0; k < n2; k++) {
				MERGE_U8(load, loadu, storeu);
			}
		}

		for (k = 0; k < n3; k++) {
			pz[0] = *pr++;
			pz[1] = *pi++;
			pz += 2;
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#define	MERGE_S16(ld1, ld2, st)				\
	rbuf = _mm_##ld1##_si128((__m128i *)pr);	\
	ibuf = _mm_##ld2##_si128((__m128i *)pi);	\
	buf1 = _mm_unpacklo_epi16(rbuf, ibuf);		\
	buf2 = _mm_unpackhi_epi16(rbuf, ibuf);		\
	_mm_##st##_si128((__m128i *)pz, buf1);		\
	pz += nstep;					\
	_mm_##st##_si128((__m128i *)pz, buf2);		\
	pz += nstep;					\
	pr += nstep;					\
	pi += nstep

mlib_status
__mlib_VectorMerge_S16C_S16(
	mlib_s16 *z,
	const mlib_s16 *r,
	const mlib_s16 *i,
	mlib_s32 n)
{
	if (n < 1)
		return (MLIB_FAILURE);

	mlib_s32 k, ar, ai, az, nstep, n1, n2, n3;
	mlib_s16 *pr = (mlib_s16 *)r, *pi = (mlib_s16 *)i, *pz = (mlib_s16 *)z;
	__m128i rbuf, ibuf, buf1, buf2;

	ar = (mlib_addr)r & 15;
	ai = (mlib_addr)i & 15;
	az = (mlib_addr)z & 15;

	nstep = 16 / sizeof (mlib_s16);
	n1 = ((16 - ar) & 15) / sizeof (mlib_s16);
	n2 = (n - n1) / nstep;
	n3 = n - n1 - n2 * nstep;

	if (n2 < 1) {
		for (k = 0; k < n; k++) {
			pz[0] = *pr++;
			pz[1] = *pi++;
			pz += 2;
		}
	} else {
		for (k = 0; k < n1; k++) {
			pz[0] = *pr++;
			pz[1] = *pi++;
			pz += 2;
		}

		if (ar == ai && (ar * 2 & 15) == az) {
			for (k = 0; k < n2; k++) {
				MERGE_S16(load, load, store);
			}
		} else if (ar == ai) {
			for (k = 0; k < n2; k++) {
				MERGE_S16(load, load, storeu);
			}
		} else {
			for (k = 0; k < n2; k++) {
				MERGE_S16(load, loadu, storeu);
			}
		}

		for (k = 0; k < n3; k++) {
			pz[0] = *pr++;
			pz[1] = *pi++;
			pz += 2;
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#define	MERGE_S32(ld1, ld2, st)				\
	rbuf = _mm_##ld1##_si128((__m128i *)pr);	\
	ibuf = _mm_##ld2##_si128((__m128i *)pi);	\
	buf1 = _mm_unpacklo_epi32(rbuf, ibuf);		\
	buf2 = _mm_unpackhi_epi32(rbuf, ibuf);		\
	_mm_##st##_si128((__m128i *)pz, buf1);		\
	pz += nstep;					\
	_mm_##st##_si128((__m128i *)pz, buf2);		\
	pz += nstep;					\
	pr += nstep;					\
	pi += nstep

mlib_status
__mlib_VectorMerge_S32C_S32(
	mlib_s32 *z,
	const mlib_s32 *r,
	const mlib_s32 *i,
	mlib_s32 n)
{
	if (n < 1)
		return (MLIB_FAILURE);

	mlib_s32 k, ar, ai, az, nstep, n1, n2, n3;
	mlib_s32 *pr = (mlib_s32 *)r, *pi = (mlib_s32 *)i, *pz = (mlib_s32 *)z;
	__m128i rbuf, ibuf, buf1, buf2;

	ar = (mlib_addr)r & 15;
	ai = (mlib_addr)i & 15;
	az = (mlib_addr)z & 15;

	nstep = 16 / sizeof (mlib_s32);
	n1 = ((16 - ar) & 15) / sizeof (mlib_s32);
	n2 = (n - n1) / nstep;
	n3 = n - n1 - n2 * nstep;

	if (n2 < 1) {
		for (k = 0; k < n; k++) {
			pz[0] = *pr++;
			pz[1] = *pi++;
			pz += 2;
		}
	} else {
		for (k = 0; k < n1; k++) {
			pz[0] = *pr++;
			pz[1] = *pi++;
			pz += 2;
		}

		if (ar == ai && (ar * 2 & 15) == az) {
			for (k = 0; k < n2; k++) {
				MERGE_S32(load, load, store);
			}
		} else if (ar == ai) {
			for (k = 0; k < n2; k++) {
				MERGE_S32(load, load, storeu);
			}
		} else {
			for (k = 0; k < n2; k++) {
				MERGE_S32(load, loadu, storeu);
			}
		}

		for (k = 0; k < n3; k++) {
			pz[0] = *pr++;
			pz[1] = *pi++;
			pz += 2;
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
