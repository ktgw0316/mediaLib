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

#pragma ident	"@(#)mlib_s_VectorConj.c	9.3	07/11/05 SMI"

/*
 * FUNCTIONS
 *      mlib_VectorConj_S8C_S8C_Sat,
 *      mlib_VectorConj_S8C_Sat      - computation of 8-bit format
 *                                     complex conjugated vector
 *      mlib_VectorConj_S16C_S16C_Sat,
 *      mlib_VectorConj_S16C_Sat      - computation of 16-bit format
 *                                      complex conjugated vector
 *      mlib_VectorConj_S32C_S32C_Sat,
 *      mlib_VectorConj_S32C_Sat      - computation of 32-bit format
 *                                      complex conjugated vector
 *
 * SYNOPSIS
 *      mlib_status mlib_VectorConj_S8C_S8C_Sat(mlib_s8       *z,
 *                                              const mlib_s8 *x,
 *                                              mlib_s32      n);
 *      mlib_status mlib_VectorConj_S8C_Sat(mlib_s8  *xz,
 *                                          mlib_s32 n);
 *
 *      mlib_status mlib_VectorConj_S16C_S16C_Sat(mlib_s16      *z,
 *                                                constmlib_s16 *x,
 *                                                mlib_s32      n);
 *      mlib_status mlib_VectorConj_S16C_Sat(mlib_s16 *xz,
 *                                           mlib_s32 n);
 *      mlib_status mlib_VectorConj_S32C_S32C_Sat(mlib_s32 *x,
 *                                                mlib_s32 n);
 *      mlib_status mlib_VectorConj_S32C_Sat(mlib_s32 *xz,
 *                                           mlib_s32 n);
 * ARGUMENTS
 *      z    pointer to the first complex element of the result vector.
 *           z[2*i] hold the real parts, and z[2*i + 1] hold the imaginary
 *           parts
 *      x    pointer to the first complex element of the input vector.
 *           x[2*i] hold the real parts, and x[2*i + 1] hold the imaginary
 *           parts
 *      n    number of complex elements in the vectors
 *
 * DESCRIPTION
 *           *
 *      z = x
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

#pragma weak mlib_VectorConj_S32C_Sat = __mlib_VectorConj_S32C_Sat
#pragma weak mlib_VectorConj_S8C_S8C_Sat = __mlib_VectorConj_S8C_S8C_Sat
#pragma weak mlib_VectorConj_S32C_S32C_Sat = __mlib_VectorConj_S32C_S32C_Sat
#pragma weak mlib_VectorConj_S16C_Sat = __mlib_VectorConj_S16C_Sat
#pragma weak mlib_VectorConj_S8C_Sat = __mlib_VectorConj_S8C_Sat
#pragma weak mlib_VectorConj_S16C_S16C_Sat = __mlib_VectorConj_S16C_S16C_Sat

#elif defined(__GNUC__)

__typeof__(__mlib_VectorConj_S32C_Sat) mlib_VectorConj_S32C_Sat
	__attribute__((weak, alias("__mlib_VectorConj_S32C_Sat")));
__typeof__(__mlib_VectorConj_S8C_S8C_Sat) mlib_VectorConj_S8C_S8C_Sat
	__attribute__((weak, alias("__mlib_VectorConj_S8C_S8C_Sat")));
__typeof__(__mlib_VectorConj_S32C_S32C_Sat) mlib_VectorConj_S32C_S32C_Sat
	__attribute__((weak, alias("__mlib_VectorConj_S32C_S32C_Sat")));
__typeof__(__mlib_VectorConj_S16C_Sat) mlib_VectorConj_S16C_Sat
	__attribute__((weak, alias("__mlib_VectorConj_S16C_Sat")));
__typeof__(__mlib_VectorConj_S8C_Sat) mlib_VectorConj_S8C_Sat
	__attribute__((weak, alias("__mlib_VectorConj_S8C_Sat")));
__typeof__(__mlib_VectorConj_S16C_S16C_Sat) mlib_VectorConj_S16C_S16C_Sat
	__attribute__((weak, alias("__mlib_VectorConj_S16C_S16C_Sat")));

#else

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	CHECK                                                   \
	if ((xx == NULL) || (zz == NULL) || (n <= 0))           \
	    return MLIB_FAILURE

#define	CONTROL(MAX, MIN, obj)	(((c = obj) == MIN) ? MAX : - c)

/* *********************************************************** */

#define	CONJ(TYPE)							\
	pz[0] = px[0];							\
	pz[1] = CONTROL(MLIB_##TYPE##_MAX, MLIB_##TYPE##_MIN, px[1]);	\
	px += 2;							\
	pz += 2

/* *********************************************************** */

#define	CONJ_S8C(ld, st)				\
	xbuf = _mm_##ld##_si128((__m128i *)px);		\
	zbuf = _mm_xor_si128(xbuf, mask1);		\
	mask3 = _mm_cmpeq_epi8(xbuf, mask2);		\
	mask3 = _mm_andnot_si128(mask3, mask1);		\
	zbuf = _mm_sub_epi8(zbuf, mask3);		\
	_mm_##st##_si128((__m128i *)pz, zbuf);		\
	px += nstep;					\
	pz += nstep

mlib_status
__mlib_VectorConj_S8C_S8C_Sat(
	mlib_s8 *zz,
	const mlib_s8 *xx,
	mlib_s32 n)
{
	CHECK;

	const mlib_s8 *px = xx;
	mlib_s8 *pz = zz;

	mlib_s32 i, ax, az, n1, n2, n3, nstep, c;
	__m128i xbuf, zbuf, mask1, mask2, mask3;
	mask1 = _mm_set1_epi16(0xff00);
	mask2 = _mm_set1_epi16(0x8000);

	ax = (mlib_addr)xx & 15;
	az = (mlib_addr)zz & 15;
	nstep = 16 / sizeof (mlib_s8);

	if (ax & 1) {
		for (i = 0; i < n / 8; i++) {
			CONJ_S8C(loadu, storeu);
		}
		for (i = 0; i < n % 8; i++) {
			CONJ(S8);
		}
	} else {
		n1 = ((16 - ax) & 15) / sizeof (mlib_s8);
		n2 = (n + n - n1) / nstep;
		n3 = n + n - n1 - n2 * nstep;

		if (n2 < 1) {
			for (i = 0; i < n; i++) {
				CONJ(S8);
			}
		} else {
			for (i = 0; i < n1 / 2; i++) {
				CONJ(S8);
			}

			if (ax == az) {
				for (i = 0; i < n2; i++) {
					CONJ_S8C(loadu, storeu);
				}
			} else {
				for (i = 0; i < n2; i++) {
					CONJ_S8C(loadu, storeu);
				}
			}
			for (i = 0; i < n3 / 2; i++) {
				CONJ(S8);
			}
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VectorConj_S8C_Sat(
	mlib_s8 *xz,
	mlib_s32 n)
{
	return (__mlib_VectorConj_S8C_S8C_Sat(xz, xz, n));
}

/* *********************************************************** */

#define	CONJ_S16C(ld, st)				\
	xbuf = _mm_##ld##_si128((__m128i *)px);		\
	zbuf = _mm_xor_si128(xbuf, mask1);		\
	mask3 = _mm_cmpeq_epi16(xbuf, mask2);		\
	mask3 = _mm_andnot_si128(mask3, mask1);		\
	zbuf = _mm_sub_epi16(zbuf, mask3);		\
	_mm_##st##_si128((__m128i *)pz, zbuf);		\
	px += nstep;					\
	pz += nstep

mlib_status
__mlib_VectorConj_S16C_S16C_Sat(
	mlib_s16 *zz,
	const mlib_s16 *xx,
	mlib_s32 n)
{
	CHECK;

	const mlib_s16 *px = xx;
	mlib_s16 *pz = zz;

	mlib_s32 i, ax, az, n1, n2, n3, nstep, c;
	__m128i xbuf, zbuf, mask1, mask2, mask3;
	mask1 = _mm_set1_epi32(0xffff0000);
	mask2 = _mm_set1_epi32(0x80000000);

	ax = (mlib_addr)xx & 15;
	az = (mlib_addr)zz & 15;
	nstep = 16 / sizeof (mlib_s16);

	if (ax & 3) {
		for (i = 0; i < n / 4; i++) {
			CONJ_S16C(loadu, storeu);
		}
		for (i = 0; i < n % 4; i++) {
			CONJ(S16);
		}
	} else {
		n1 = ((16 - ax) & 15) / sizeof (mlib_s16);
		n2 = (n + n - n1) / nstep;
		n3 = n + n - n1 - n2 * nstep;

		if (n2 < 1) {
			for (i = 0; i < n; i++) {
				CONJ(S16);
			}
		} else {
			for (i = 0; i < n1 / 2; i++) {
				CONJ(S16);
			}

			if (ax == az) {
				for (i = 0; i < n2; i++) {
					CONJ_S16C(loadu, storeu);
				}
			} else {
				for (i = 0; i < n2; i++) {
					CONJ_S16C(loadu, storeu);
				}
			}
			for (i = 0; i < n3 / 2; i++) {
				CONJ(S16);
			}
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VectorConj_S16C_Sat(
	mlib_s16 *xz,
	mlib_s32 n)
{
	return (__mlib_VectorConj_S16C_S16C_Sat(xz, xz, n));
}

/* *********************************************************** */

#define	CONJ_S32C(ld, st)				\
	xbuf = _mm_##ld##_si128((__m128i *)px);		\
	zbuf = _mm_xor_si128(xbuf, mask1);		\
	mask3 = _mm_cmpeq_epi32(xbuf, mask2);		\
	mask3 = _mm_andnot_si128(mask3, mask1);		\
	zbuf = _mm_sub_epi32(zbuf, mask3);		\
	_mm_##st##_si128((__m128i *)pz, zbuf);		\
	px += nstep;					\
	pz += nstep

mlib_status
__mlib_VectorConj_S32C_S32C_Sat(
	mlib_s32 *zz,
	const mlib_s32 *xx,
	mlib_s32 n)
{
	CHECK;

	const mlib_s32 *px = xx;
	mlib_s32 *pz = zz;

	mlib_s32 i, ax, az, n1, n2, n3, nstep, c;
	__m128i xbuf, zbuf, mask1, mask2, mask3;
	mask1 = _mm_setr_epi32(0, 0xffffffff, 0, 0xffffffff);
	mask2 = _mm_setr_epi32(0, 0x80000000, 0, 0x80000000);

	ax = (mlib_addr)xx & 15;
	az = (mlib_addr)zz & 15;
	nstep = 16 / sizeof (mlib_s32);

	if (ax & 7) {
		for (i = 0; i < n / 2; i++) {
			CONJ_S32C(loadu, storeu);
		}
		for (i = 0; i < n % 2; i++) {
			CONJ(S32);
		}
	} else {
		n1 = ((16 - ax) & 15) / sizeof (mlib_s32);
		n2 = (n + n - n1) / nstep;
		n3 = n + n - n1 - n2 * nstep;

		if (n2 < 1) {
			for (i = 0; i < n; i++) {
				CONJ(S32);
			}
		} else {
			for (i = 0; i < n1 / 2; i++) {
				CONJ(S32);
			}

			if (ax == az) {
				for (i = 0; i < n2; i++) {
					CONJ_S32C(loadu, storeu);
				}
			} else {
				for (i = 0; i < n2; i++) {
					CONJ_S32C(loadu, storeu);
				}
			}
			for (i = 0; i < n3 / 2; i++) {
				CONJ(S32);
			}
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VectorConj_S32C_Sat(
	mlib_s32 *xz,
	mlib_s32 n)
{
	return (__mlib_VectorConj_S32C_S32C_Sat(xz, xz, n));
}

/* *********************************************************** */
