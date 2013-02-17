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

#pragma ident	"@(#)mlib_s_VectorSub.c	9.9	07/11/05 SMI"

/*
 * FUNCTIONS
 *      mlib_VectorSub8  - subtraction of 8-bit format vectors
 *      mlib_VectorSub16 - subtraction of 16-bit format vectors
 *      mlib_VectorSub32 - subtraction of 32-bit format vectors
 *
 * SYNOPSIS
 *      mlib_status mlib_VectorSub_U8_U8_[Sat|Mod](mlib_u8      *z,
 *                                                const mlib_u8 *x,
 *                                                const mlib_u8 *y,
 *                                                mlib_s32      n);
 *      mlib_status mlib_VectorSub_S8_S8_[Sat|Mod](mlib_s8      *z,
 *                                                const mlib_s8 *x,
 *                                                const mlib_s8 *y,
 *                                                mlib_s32      n);
 *      mlib_status mlib_VectorSub_S16_U8_[Sat|Mod](mlib_s16      *z,
 *                                                  const mlib_u8 *x,
 *                                                  const mlib_u8 *y,
 *                                                  mlib_s32      n);
 *      mlib_status mlib_VectorSub_S16_S8_[Sat|Mod](mlib_s16      *z,
 *                                                  const mlib_s8 *x,
 *                                                  const mlib_s8 *y,
 *                                                  mlib_s32      n);
 *      mlib_status mlib_VectorSub_S16_S16_[Sat|Mod](mlib_s16      *z,
 *                                                  const mlib_s16 *x,
 *                                                  const mlib_s16 *y,
 *                                                  mlib_s32       n);
 *      mlib_status mlib_VectorSub_S32_S16_[Sat|Mod](mlib_s32      *z,
 *                                                  const mlib_s16 *x,
 *                                                  const mlib_s16 *y,
 *                                                  mlib_s32       n);
 *      mlib_status mlib_VectorSub_S32_S32_[Sat|Mod](mlib_s32      *z,
 *                                                  const mlib_s32 *x,
 *                                                  const mlib_s32 *y,
 *                                                  mlib_s32       n);
 *
 *      mlib_status mlib_VectorSub_U8_[Sat|Mod](mlib_u8       *xz,
 *                                              const mlib_u8 *y,
 *                                              mlib_s32      n);
 *      mlib_status mlib_VectorSub_S8_[Sat|Mod](mlib_s8       *xz,
 *                                              const mlib_s8 *y,
 *                                              mlib_s32      n);
 *      mlib_status mlib_VectorSub_S16_[Sat|Mod](mlib_s16       *xz,
 *                                               const mlib_s16 *y,
 *                                               mlib_s32       n);
 *      mlib_status mlib_VectorSub_S32_[Sat|Mod](mlib_s32       *xz,
 *                                               const mlib_s32 *y,
 *                                               mlib_s32       n);
 *
 * ARGUMENTS
 *      z    pointer to the first element of the result vector
 *      xz   pointer to the first element of the first input and
 *           the output vector
 *      x    pointer to the first element of the first vector
 *      y    pointer to the first element of the second vector
 *      n    number of elements in the vectors
 *
 * DESCRIPTION
 *      z = x - y
 */

/*
 * FUNCTIONS
 *      mlib_VectorSub8C  - subtraction of 8-bit format complex vectors
 *      mlib_VectorSub16C - subtraction of 16-bit format complex vectors
 *      mlib_VectorSub32C - subtraction of 32-bit format complex vectors
 *
 * SYNOPSIS
 *      mlib_status mlib_VectorSub_U8C_U8C_[Sat|Mod](mlib_u8       *z,
 *                                                   const mlib_u8 *x,
 *                                                   const mlib_u8 *y,
 *                                                   mlib_s32      n);
 *      mlib_status mlib_VectorSub_S8C_S8C_[Sat|Mod](mlib_s8       *z,
 *                                                   const mlib_s8 *x,
 *                                                   const mlib_s8 *y,
 *                                                   mlib_s32      n);
 *      mlib_status mlib_VectorSub_S16C_U8C_[Sat|Mod](mlib_s16      *z,
 *                                                    const mlib_u8 *x,
 *                                                    const mlib_u8 *y,
 *                                                    mlib_s32      n);
 *      mlib_status mlib_VectorSub_S16C_S8C_[Sat|Mod](mlib_s16      *z,
 *                                                    const mlib_s8 *x,
 *                                                    const mlib_s8 *y,
 *                                                    mlib_s32      n);
 *      mlib_status mlib_VectorSub_S16C_S16C_[Sat|Mod](mlib_s16       *z,
 *                                                     const mlib_s16 *x,
 *                                                     const mlib_s16 *y,
 *                                                     mlib_s32       n);
 *      mlib_status mlib_VectorSub_S32C_S16C_[Sat|Mod](mlib_s32       *z,
 *                                                     const mlib_s16 *x,
 *                                                     const mlib_s16 *y,
 *                                                     mlib_s32       n);
 *      mlib_status mlib_VectorSub_S32C_S32C_[Sat|Mod](mlib_s32       *z,
 *                                                     const mlib_s32 *x,
 *                                                     const mlib_s32 *y,
 *                                                     mlib_s32       n);
 *
 *      mlib_status mlib_VectorSub_U8C_[Sat|Mod](mlib_u8       *xz,
 *                                               const mlib_u8 *y,
 *                                               mlib_s32      n);
 *      mlib_status mlib_VectorSub_S8C_[Sat|Mod](mlib_s8       *xz,
 *                                               const mlib_s8 *y,
 *                                               mlib_s32      n);
 *      mlib_status mlib_VectorSub_S16C_[Sat|Mod](mlib_s16       *xz,
 *                                                const mlib_s16 *y,
 *                                                mlib_s32       n);
 *      mlib_status mlib_VectorSub_S32C_[Sat|Mod](mlib_s32       *xz,
 *                                                const mlib_s32 *y,
 *                                                mlib_s32       n);
 *
 * ARGUMENTS
 *      z    pointer to the first complex element of the result vector.
 *           z[2*i] hold the real parts, and z[2*i + 1] hold the imaginary
 *           parts
 *      xz   pointer to the first element of the first input and
 *           the output vector
 *           zx[2*i] hold the real parts, and xz[2*i + 1] hold the imaginary
 *           parts
 *      x    pointer to the first complex element of the first vector.
 *           x[2*i] hold the real parts, and x[2*i + 1] hold the imaginary
 *           parts
 *      y    pointer to the first complex element of the second vector.
 *           y[2*i] hold the real parts, and y[2*i + 1] hold the imaginary
 *           parts
 *      n    number of complex elements in the vectors
 *
 * DESCRIPTION
 *      z = x - y
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

#pragma weak mlib_VectorSub_S16_Sat = __mlib_VectorSub_S16_Sat
#pragma weak mlib_VectorSub_S32C_Sat = __mlib_VectorSub_S32C_Sat
#pragma weak mlib_VectorSub_S8C_S8C_Sat = __mlib_VectorSub_S8C_S8C_Sat
#pragma weak mlib_VectorSub_S16_S8_Mod = __mlib_VectorSub_S16_S8_Mod
#pragma weak mlib_VectorSub_S16C_Sat = __mlib_VectorSub_S16C_Sat
#pragma weak mlib_VectorSub_S16_U8_Mod = __mlib_VectorSub_S16_U8_Mod
#pragma weak mlib_VectorSub_U8C_U8C_Sat = __mlib_VectorSub_U8C_U8C_Sat
#pragma weak mlib_VectorSub_S16C_S8C_Mod = __mlib_VectorSub_S16C_S8C_Mod
#pragma weak mlib_VectorSub_S8_Mod = __mlib_VectorSub_S8_Mod
#pragma weak mlib_VectorSub_S16C_U8C_Mod = __mlib_VectorSub_S16C_U8C_Mod
#pragma weak mlib_VectorSub_U8_Mod = __mlib_VectorSub_U8_Mod
#pragma weak mlib_VectorSub_S8C_Mod = __mlib_VectorSub_S8C_Mod
#pragma weak mlib_VectorSub_S32_S32_Mod = __mlib_VectorSub_S32_S32_Mod
#pragma weak mlib_VectorSub_U8C_Mod = __mlib_VectorSub_U8C_Mod
#pragma weak mlib_VectorSub_S32_S16_Mod = __mlib_VectorSub_S32_S16_Mod
#pragma weak mlib_VectorSub_S8_Sat = __mlib_VectorSub_S8_Sat
#pragma weak mlib_VectorSub_S16_S16_Mod = __mlib_VectorSub_S16_S16_Mod
#pragma weak mlib_VectorSub_S32C_S32C_Mod = __mlib_VectorSub_S32C_S32C_Mod
#pragma weak mlib_VectorSub_U8_Sat = __mlib_VectorSub_U8_Sat
#pragma weak mlib_VectorSub_S8_S8_Mod = __mlib_VectorSub_S8_S8_Mod
#pragma weak mlib_VectorSub_S8C_Sat = __mlib_VectorSub_S8C_Sat
#pragma weak mlib_VectorSub_S32C_S16C_Mod = __mlib_VectorSub_S32C_S16C_Mod
#pragma weak mlib_VectorSub_S32_S32_Sat = __mlib_VectorSub_S32_S32_Sat
#pragma weak mlib_VectorSub_U8C_Sat = __mlib_VectorSub_U8C_Sat
#pragma weak mlib_VectorSub_S32_Mod = __mlib_VectorSub_S32_Mod
#pragma weak mlib_VectorSub_S16C_S16C_Mod = __mlib_VectorSub_S16C_S16C_Mod
#pragma weak mlib_VectorSub_U8_U8_Mod = __mlib_VectorSub_U8_U8_Mod
#pragma weak mlib_VectorSub_S16_Mod = __mlib_VectorSub_S16_Mod
#pragma weak mlib_VectorSub_S32C_Mod = __mlib_VectorSub_S32C_Mod
#pragma weak mlib_VectorSub_S8C_S8C_Mod = __mlib_VectorSub_S8C_S8C_Mod
#pragma weak mlib_VectorSub_S16_S16_Sat = __mlib_VectorSub_S16_S16_Sat
#pragma weak mlib_VectorSub_S32C_S32C_Sat = __mlib_VectorSub_S32C_S32C_Sat
#pragma weak mlib_VectorSub_S16C_Mod = __mlib_VectorSub_S16C_Mod
#pragma weak mlib_VectorSub_S8_S8_Sat = __mlib_VectorSub_S8_S8_Sat
#pragma weak mlib_VectorSub_U8C_U8C_Mod = __mlib_VectorSub_U8C_U8C_Mod
#pragma weak mlib_VectorSub_S32_Sat = __mlib_VectorSub_S32_Sat
#pragma weak mlib_VectorSub_S16C_S16C_Sat = __mlib_VectorSub_S16C_S16C_Sat
#pragma weak mlib_VectorSub_U8_U8_Sat = __mlib_VectorSub_U8_U8_Sat

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */

__typeof__(__mlib_VectorSub_S16_Sat) mlib_VectorSub_S16_Sat
	__attribute__((weak, alias("__mlib_VectorSub_S16_Sat")));
__typeof__(__mlib_VectorSub_S32C_Sat) mlib_VectorSub_S32C_Sat
	__attribute__((weak, alias("__mlib_VectorSub_S32C_Sat")));
__typeof__(__mlib_VectorSub_S8C_S8C_Sat) mlib_VectorSub_S8C_S8C_Sat
	__attribute__((weak, alias("__mlib_VectorSub_S8C_S8C_Sat")));
__typeof__(__mlib_VectorSub_S16_S8_Mod) mlib_VectorSub_S16_S8_Mod
	__attribute__((weak, alias("__mlib_VectorSub_S16_S8_Mod")));
__typeof__(__mlib_VectorSub_S16C_Sat) mlib_VectorSub_S16C_Sat
	__attribute__((weak, alias("__mlib_VectorSub_S16C_Sat")));
__typeof__(__mlib_VectorSub_S16_U8_Mod) mlib_VectorSub_S16_U8_Mod
	__attribute__((weak, alias("__mlib_VectorSub_S16_U8_Mod")));
__typeof__(__mlib_VectorSub_U8C_U8C_Sat) mlib_VectorSub_U8C_U8C_Sat
	__attribute__((weak, alias("__mlib_VectorSub_U8C_U8C_Sat")));
__typeof__(__mlib_VectorSub_S16C_S8C_Mod) mlib_VectorSub_S16C_S8C_Mod
	__attribute__((weak, alias("__mlib_VectorSub_S16C_S8C_Mod")));
__typeof__(__mlib_VectorSub_S8_Mod) mlib_VectorSub_S8_Mod
	__attribute__((weak, alias("__mlib_VectorSub_S8_Mod")));
__typeof__(__mlib_VectorSub_S16C_U8C_Mod) mlib_VectorSub_S16C_U8C_Mod
	__attribute__((weak, alias("__mlib_VectorSub_S16C_U8C_Mod")));
__typeof__(__mlib_VectorSub_U8_Mod) mlib_VectorSub_U8_Mod
	__attribute__((weak, alias("__mlib_VectorSub_U8_Mod")));
__typeof__(__mlib_VectorSub_S8C_Mod) mlib_VectorSub_S8C_Mod
	__attribute__((weak, alias("__mlib_VectorSub_S8C_Mod")));
__typeof__(__mlib_VectorSub_S32_S32_Mod) mlib_VectorSub_S32_S32_Mod
	__attribute__((weak, alias("__mlib_VectorSub_S32_S32_Mod")));
__typeof__(__mlib_VectorSub_U8C_Mod) mlib_VectorSub_U8C_Mod
	__attribute__((weak, alias("__mlib_VectorSub_U8C_Mod")));
__typeof__(__mlib_VectorSub_S32_S16_Mod) mlib_VectorSub_S32_S16_Mod
	__attribute__((weak, alias("__mlib_VectorSub_S32_S16_Mod")));
__typeof__(__mlib_VectorSub_S8_Sat) mlib_VectorSub_S8_Sat
	__attribute__((weak, alias("__mlib_VectorSub_S8_Sat")));
__typeof__(__mlib_VectorSub_S16_S16_Mod) mlib_VectorSub_S16_S16_Mod
	__attribute__((weak, alias("__mlib_VectorSub_S16_S16_Mod")));
__typeof__(__mlib_VectorSub_S32C_S32C_Mod) mlib_VectorSub_S32C_S32C_Mod
	__attribute__((weak, alias("__mlib_VectorSub_S32C_S32C_Mod")));
__typeof__(__mlib_VectorSub_U8_Sat) mlib_VectorSub_U8_Sat
	__attribute__((weak, alias("__mlib_VectorSub_U8_Sat")));
__typeof__(__mlib_VectorSub_S8_S8_Mod) mlib_VectorSub_S8_S8_Mod
	__attribute__((weak, alias("__mlib_VectorSub_S8_S8_Mod")));
__typeof__(__mlib_VectorSub_S8C_Sat) mlib_VectorSub_S8C_Sat
	__attribute__((weak, alias("__mlib_VectorSub_S8C_Sat")));
__typeof__(__mlib_VectorSub_S32C_S16C_Mod) mlib_VectorSub_S32C_S16C_Mod
	__attribute__((weak, alias("__mlib_VectorSub_S32C_S16C_Mod")));
__typeof__(__mlib_VectorSub_S32_S32_Sat) mlib_VectorSub_S32_S32_Sat
	__attribute__((weak, alias("__mlib_VectorSub_S32_S32_Sat")));
__typeof__(__mlib_VectorSub_U8C_Sat) mlib_VectorSub_U8C_Sat
	__attribute__((weak, alias("__mlib_VectorSub_U8C_Sat")));
__typeof__(__mlib_VectorSub_S32_Mod) mlib_VectorSub_S32_Mod
	__attribute__((weak, alias("__mlib_VectorSub_S32_Mod")));
__typeof__(__mlib_VectorSub_S16C_S16C_Mod) mlib_VectorSub_S16C_S16C_Mod
	__attribute__((weak, alias("__mlib_VectorSub_S16C_S16C_Mod")));
__typeof__(__mlib_VectorSub_U8_U8_Mod) mlib_VectorSub_U8_U8_Mod
	__attribute__((weak, alias("__mlib_VectorSub_U8_U8_Mod")));
__typeof__(__mlib_VectorSub_S16_Mod) mlib_VectorSub_S16_Mod
	__attribute__((weak, alias("__mlib_VectorSub_S16_Mod")));
__typeof__(__mlib_VectorSub_S32C_Mod) mlib_VectorSub_S32C_Mod
	__attribute__((weak, alias("__mlib_VectorSub_S32C_Mod")));
__typeof__(__mlib_VectorSub_S8C_S8C_Mod) mlib_VectorSub_S8C_S8C_Mod
	__attribute__((weak, alias("__mlib_VectorSub_S8C_S8C_Mod")));
__typeof__(__mlib_VectorSub_S16_S16_Sat) mlib_VectorSub_S16_S16_Sat
	__attribute__((weak, alias("__mlib_VectorSub_S16_S16_Sat")));
__typeof__(__mlib_VectorSub_S32C_S32C_Sat) mlib_VectorSub_S32C_S32C_Sat
	__attribute__((weak, alias("__mlib_VectorSub_S32C_S32C_Sat")));
__typeof__(__mlib_VectorSub_S16C_Mod) mlib_VectorSub_S16C_Mod
	__attribute__((weak, alias("__mlib_VectorSub_S16C_Mod")));
__typeof__(__mlib_VectorSub_S8_S8_Sat) mlib_VectorSub_S8_S8_Sat
	__attribute__((weak, alias("__mlib_VectorSub_S8_S8_Sat")));
__typeof__(__mlib_VectorSub_U8C_U8C_Mod) mlib_VectorSub_U8C_U8C_Mod
	__attribute__((weak, alias("__mlib_VectorSub_U8C_U8C_Mod")));
__typeof__(__mlib_VectorSub_S32_Sat) mlib_VectorSub_S32_Sat
	__attribute__((weak, alias("__mlib_VectorSub_S32_Sat")));
__typeof__(__mlib_VectorSub_S16C_S16C_Sat) mlib_VectorSub_S16C_S16C_Sat
	__attribute__((weak, alias("__mlib_VectorSub_S16C_S16C_Sat")));
__typeof__(__mlib_VectorSub_U8_U8_Sat) mlib_VectorSub_U8_U8_Sat
	__attribute__((weak, alias("__mlib_VectorSub_U8_U8_Sat")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	SAT_U8(x)		\
	if (x < 0)		\
	    x = 0;		\
	else if (x > 255)	\
	    x = 255

#define	SAT_S8(x)		\
	if (x < -128)		\
	    x = -128;		\
	else if (x > 127)	\
	    x = 127

#define	SAT_S16(x)			\
	if (x < MLIB_S16_MIN)		\
	    x = MLIB_S16_MIN;		\
	else if (x > MLIB_S16_MAX)	\
	    x = MLIB_S16_MAX

#define	SAT_S32(x)			\
	if (x < MLIB_S32_MIN)		\
	    x = MLIB_S32_MIN;		\
	else if (x > MLIB_S32_MAX)	\
	    x = MLIB_S32_MAX

/* ****************************************************** */

#define	VECTOR_SUB(mmsub)					\
	if (ax == ay && ax == az) {				\
		for (i = 0; i < n2; i++) {			\
			xbuf = _mm_load_si128((__m128i *)px);	\
			ybuf = _mm_load_si128((__m128i *)py);	\
			zbuf = mmsub(xbuf, ybuf);		\
			_mm_store_si128((__m128i *)pz, zbuf);	\
			px += nstep;				\
			py += nstep;				\
			pz += nstep;				\
		}						\
	} else if (ax == ay) {					\
		for (i = 0; i < n2; i++) {			\
			xbuf = _mm_load_si128((__m128i *)px);	\
			ybuf = _mm_load_si128((__m128i *)py);	\
			zbuf = mmsub(xbuf, ybuf);		\
			_mm_storeu_si128((__m128i *)pz, zbuf);	\
			px += nstep;				\
			py += nstep;				\
			pz += nstep;				\
		}						\
	} else if (ax == az) {					\
		for (i = 0; i < n2; i++) {			\
			xbuf = _mm_load_si128((__m128i *)px);	\
			ybuf = _mm_loadu_si128((__m128i *)py);	\
			zbuf = mmsub(xbuf, ybuf);		\
			_mm_store_si128((__m128i *)pz, zbuf);	\
			px += nstep;				\
			py += nstep;				\
			pz += nstep;				\
		}						\
	} else {						\
		for (i = 0; i < n2; i++) {			\
			xbuf = _mm_load_si128((__m128i *)px);	\
			ybuf = _mm_loadu_si128((__m128i *)py);	\
			zbuf = mmsub(xbuf, ybuf);		\
			_mm_storeu_si128((__m128i *)pz, zbuf);	\
			px += nstep;				\
			py += nstep;				\
			pz += nstep;				\
		}						\
	}

/* *********************************************************** */

mlib_status
__mlib_VectorSub_U8_U8_Sat(
	mlib_u8 *z,
	const mlib_u8 *x,
	const mlib_u8 *y,
	mlib_s32 n)
{
	mlib_s32 i, sum, ax, ay, az, nstep, n1, n2, n3;
	mlib_u8 *px = (mlib_u8 *)x, *py = (mlib_u8 *)y, *pz = (mlib_u8 *)z;
	__m128i xbuf, ybuf, zbuf;

	if (n < 1)
		return (MLIB_FAILURE);

	ax = (mlib_addr)x & 15;
	ay = (mlib_addr)y & 15;
	az = (mlib_addr)z & 15;

	nstep = 16 / sizeof (mlib_u8);
	n1 = ((16 - ((mlib_addr)x & 15)) & 15) / sizeof (mlib_u8);
	n2 = (n - n1) / nstep;
	n3 = n - n1 - n2 * nstep;

	if (n2 < 1) {
		for (i = 0; i < n; i++) {
			sum = (*px++) - (*py++);
			SAT_U8(sum);
			*pz++ = sum;
		}
	} else {
		for (i = 0; i < n1; i++) {
			sum = (*px++) - (*py++);
			SAT_U8(sum);
			*pz++ = sum;
		}

		VECTOR_SUB(_mm_subs_epu8);

		for (i = 0; i < n3; i++) {
			sum = (*px++) - (*py++);
			SAT_U8(sum);
			*pz++ = sum;
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */


mlib_status
__mlib_VectorSub_U8_Sat(
	mlib_u8 *xz,
	const mlib_u8 *y,
	mlib_s32 n)
{
	return (__mlib_VectorSub_U8_U8_Sat(xz, xz, y, n));
}

/* *********************************************************** */

mlib_status
__mlib_VectorSub_U8_U8_Mod(
	mlib_u8 *z,
	const mlib_u8 *x,
	const mlib_u8 *y,
	mlib_s32 n)
{
	mlib_s32 i, sum, ax, ay, az, nstep, n1, n2, n3;
	mlib_u8 *px = (mlib_u8 *)x, *py = (mlib_u8 *)y, *pz = (mlib_u8 *)z;
	__m128i xbuf, ybuf, zbuf;

	if (n < 1)
		return (MLIB_FAILURE);

	ax = (mlib_addr)x & 15;
	ay = (mlib_addr)y & 15;
	az = (mlib_addr)z & 15;

	nstep = 16 / sizeof (mlib_u8);
	n1 = ((16 - ((mlib_addr)x & 15)) & 15) / sizeof (mlib_u8);
	n2 = (n - n1) / nstep;
	n3 = n - n1 - n2 * nstep;

	if (n2 < 1) {
		for (i = 0; i < n; i++) {
			sum = (*px++) - (*py++);
			*pz++ = sum;
		}
	} else {
		for (i = 0; i < n1; i++) {
			sum = (*px++) - (*py++);
			*pz++ = sum;
		}

		VECTOR_SUB(_mm_sub_epi8);

		for (i = 0; i < n3; i++) {
			sum = (*px++) - (*py++);
			*pz++ = sum;
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VectorSub_U8_Mod(
	mlib_u8 *xz,
	const mlib_u8 *y,
	mlib_s32 n)
{
	return (__mlib_VectorSub_U8_U8_Mod(xz, xz, y, n));
}

/* *********************************************************** */

mlib_status
__mlib_VectorSub_S16_U8_Mod(
	mlib_s16 *z,
	const mlib_u8 *x,
	const mlib_u8 *y,
	mlib_s32 n)
{
	mlib_s32 i, sum, ax, ay, az, nstep, n1, n2, n3;
	mlib_u8 *px = (mlib_u8 *)x, *py = (mlib_u8 *)y;
	mlib_s16 *pz = (mlib_s16 *)z;
	__m128i xbuf, ybuf, zbuf, xbuf1, ybuf1, zbuf1, zero;

	if (n < 1)
		return (MLIB_FAILURE);

	zero = _mm_setzero_si128();

	ax = (mlib_addr)x & 15;
	ay = (mlib_addr)y & 15;
	az = (mlib_addr)z & 15;

	nstep = 16 / sizeof (mlib_u8);
	n1 = ((16 - ((mlib_addr)x & 15)) & 15) / sizeof (mlib_u8);
	n2 = (n - n1) / nstep;
	n3 = n - n1 - n2 * nstep;

	if (n2 < 1) {
		for (i = 0; i < n; i++) {
			*pz++ = (mlib_s32)(*px++) - (*py++);
		}
	} else {
		for (i = 0; i < n1; i++) {
			*pz++ = (mlib_s32)(*px++) - (*py++);
		}

		if (ax == ay && (ax * 2 & 15) == az) {
			for (i = 0; i < n2; i++) {
				xbuf  = _mm_load_si128((__m128i *)px);
				ybuf  = _mm_load_si128((__m128i *)py);
				xbuf1 = _mm_unpacklo_epi8(xbuf, zero);
				ybuf1 = _mm_unpacklo_epi8(ybuf, zero);
				xbuf  = _mm_unpackhi_epi8(xbuf, zero);
				ybuf  = _mm_unpackhi_epi8(ybuf, zero);
				zbuf1 = _mm_sub_epi16(xbuf1, ybuf1);
				zbuf  = _mm_sub_epi16(xbuf, ybuf);
				_mm_store_si128((__m128i *)pz, zbuf1);
				_mm_store_si128((__m128i *)pz + 1, zbuf);
				px += nstep;
				py += nstep;
				pz += nstep;
			}
		} else if (ax == ay) {
			for (i = 0; i < n2; i++) {
				xbuf  = _mm_load_si128((__m128i *)px);
				ybuf  = _mm_load_si128((__m128i *)py);
				xbuf1 = _mm_unpacklo_epi8(xbuf, zero);
				ybuf1 = _mm_unpacklo_epi8(ybuf, zero);
				xbuf  = _mm_unpackhi_epi8(xbuf, zero);
				ybuf  = _mm_unpackhi_epi8(ybuf, zero);
				zbuf1 = _mm_sub_epi16(xbuf1, ybuf1);
				zbuf  = _mm_sub_epi16(xbuf, ybuf);
				_mm_storeu_si128((__m128i *)pz, zbuf1);
				_mm_storeu_si128((__m128i *)pz + 1, zbuf);
				px += nstep;
				py += nstep;
				pz += nstep;
			}
		} else if ((ax * 2 & 15) == az) {
			for (i = 0; i < n2; i++) {
				xbuf  = _mm_load_si128((__m128i *)px);
				ybuf  = _mm_loadu_si128((__m128i *)py);
				xbuf1 = _mm_unpacklo_epi8(xbuf, zero);
				ybuf1 = _mm_unpacklo_epi8(ybuf, zero);
				xbuf  = _mm_unpackhi_epi8(xbuf, zero);
				ybuf  = _mm_unpackhi_epi8(ybuf, zero);
				zbuf1 = _mm_sub_epi16(xbuf1, ybuf1);
				zbuf  = _mm_sub_epi16(xbuf, ybuf);
				_mm_store_si128((__m128i *)pz, zbuf1);
				_mm_store_si128((__m128i *)pz + 1, zbuf);
				px += nstep;
				py += nstep;
				pz += nstep;
			}
		} else {
			for (i = 0; i < n2; i++) {
				xbuf  = _mm_load_si128((__m128i *)px);
				ybuf  = _mm_loadu_si128((__m128i *)py);
				xbuf1 = _mm_unpacklo_epi8(xbuf, zero);
				ybuf1 = _mm_unpacklo_epi8(ybuf, zero);
				xbuf  = _mm_unpackhi_epi8(xbuf, zero);
				ybuf  = _mm_unpackhi_epi8(ybuf, zero);
				zbuf1 = _mm_sub_epi16(xbuf1, ybuf1);
				zbuf  = _mm_sub_epi16(xbuf, ybuf);
				_mm_storeu_si128((__m128i *)pz, zbuf1);
				_mm_storeu_si128((__m128i *)pz + 1, zbuf);
				px += nstep;
				py += nstep;
				pz += nstep;
			}
		}

		for (i = 0; i < n3; i++) {
			*pz++ = (mlib_s32)(*px++) - (*py++);
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VectorSub_S16_S8_Mod(
	mlib_s16 *z,
	const mlib_s8 *x,
	const mlib_s8 *y,
	mlib_s32 n)
{
	mlib_s32 i, sum, ax, ay, az, nstep, n1, n2, n3;
	mlib_s8 *px = (mlib_s8 *)x, *py = (mlib_s8 *)y;
	mlib_s16 *pz = (mlib_s16 *)z;
	__m128i xbuf, ybuf, zbuf, xbuf1, ybuf1, zbuf1, zero, xext, yext;

	if (n < 1)
		return (MLIB_FAILURE);

	zero = _mm_setzero_si128();

	ax = (mlib_addr)x & 15;
	ay = (mlib_addr)y & 15;
	az = (mlib_addr)z & 15;

	nstep = 16 / sizeof (mlib_s8);
	n1 = ((16 - ((mlib_addr)x & 15)) & 15) / sizeof (mlib_s8);
	n2 = (n - n1) / nstep;
	n3 = n - n1 - n2 * nstep;

	if (n2 < 1) {
		for (i = 0; i < n; i++) {
			*pz++ = (mlib_s32)(*px++) - (*py++);
		}
	} else {
		for (i = 0; i < n1; i++) {
			*pz++ = (mlib_s32)(*px++) - (*py++);
		}

		if (ax == ay && (ax * 2 & 15) == az) {
			for (i = 0; i < n2; i++) {
				xbuf  = _mm_load_si128((__m128i *)px);
				ybuf  = _mm_load_si128((__m128i *)py);
				xext  = _mm_cmpgt_epi8(zero, xbuf);
				yext  = _mm_cmpgt_epi8(zero, ybuf);
				xbuf1 = _mm_unpacklo_epi8(xbuf, xext);
				ybuf1 = _mm_unpacklo_epi8(ybuf, yext);
				xbuf  = _mm_unpackhi_epi8(xbuf, xext);
				ybuf  = _mm_unpackhi_epi8(ybuf, yext);
				zbuf1 = _mm_sub_epi16(xbuf1, ybuf1);
				zbuf  = _mm_sub_epi16(xbuf, ybuf);
				_mm_store_si128((__m128i *)pz, zbuf1);
				_mm_store_si128((__m128i *)pz + 1, zbuf);
				px += nstep;
				py += nstep;
				pz += nstep;
			}
		} else if (ax == ay) {
			for (i = 0; i < n2; i++) {
				xbuf  = _mm_load_si128((__m128i *)px);
				ybuf  = _mm_load_si128((__m128i *)py);
				xext  = _mm_cmpgt_epi8(zero, xbuf);
				yext  = _mm_cmpgt_epi8(zero, ybuf);
				xbuf1 = _mm_unpacklo_epi8(xbuf, xext);
				ybuf1 = _mm_unpacklo_epi8(ybuf, yext);
				xbuf  = _mm_unpackhi_epi8(xbuf, xext);
				ybuf  = _mm_unpackhi_epi8(ybuf, yext);
				zbuf1 = _mm_sub_epi16(xbuf1, ybuf1);
				zbuf  = _mm_sub_epi16(xbuf, ybuf);
				_mm_storeu_si128((__m128i *)pz, zbuf1);
				_mm_storeu_si128((__m128i *)pz + 1, zbuf);
				px += nstep;
				py += nstep;
				pz += nstep;
			}
		} else if ((ax * 2 & 15) == az) {
			for (i = 0; i < n2; i++) {
				xbuf  = _mm_load_si128((__m128i *)px);
				ybuf  = _mm_loadu_si128((__m128i *)py);
				xext  = _mm_cmpgt_epi8(zero, xbuf);
				yext  = _mm_cmpgt_epi8(zero, ybuf);
				xbuf1 = _mm_unpacklo_epi8(xbuf, xext);
				ybuf1 = _mm_unpacklo_epi8(ybuf, yext);
				xbuf  = _mm_unpackhi_epi8(xbuf, xext);
				ybuf  = _mm_unpackhi_epi8(ybuf, yext);
				zbuf1 = _mm_sub_epi16(xbuf1, ybuf1);
				zbuf  = _mm_sub_epi16(xbuf, ybuf);
				_mm_store_si128((__m128i *)pz, zbuf1);
				_mm_store_si128((__m128i *)pz + 1, zbuf);
				px += nstep;
				py += nstep;
				pz += nstep;
			}
		} else {
			for (i = 0; i < n2; i++) {
				xbuf  = _mm_load_si128((__m128i *)px);
				ybuf  = _mm_loadu_si128((__m128i *)py);
				xext  = _mm_cmpgt_epi8(zero, xbuf);
				yext  = _mm_cmpgt_epi8(zero, ybuf);
				xbuf1 = _mm_unpacklo_epi8(xbuf, xext);
				ybuf1 = _mm_unpacklo_epi8(ybuf, yext);
				xbuf  = _mm_unpackhi_epi8(xbuf, xext);
				ybuf  = _mm_unpackhi_epi8(ybuf, yext);
				zbuf1 = _mm_sub_epi16(xbuf1, ybuf1);
				zbuf  = _mm_sub_epi16(xbuf, ybuf);
				_mm_storeu_si128((__m128i *)pz, zbuf1);
				_mm_storeu_si128((__m128i *)pz + 1, zbuf);
				px += nstep;
				py += nstep;
				pz += nstep;
			}
		}

		for (i = 0; i < n3; i++) {
			*pz++ = (mlib_s32)(*px++) - (*py++);
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VectorSub_S8_S8_Sat(
	mlib_s8 *z,
	const mlib_s8 *x,
	const mlib_s8 *y,
	mlib_s32 n)
{
	mlib_s32 i, sum, ax, ay, az, nstep, n1, n2, n3;
	mlib_s8 *px = (mlib_s8 *)x, *py = (mlib_s8 *)y, *pz = (mlib_s8 *)z;
	__m128i xbuf, ybuf, zbuf;

	if (n < 1)
		return (MLIB_FAILURE);

	ax = (mlib_addr)x & 15;
	ay = (mlib_addr)y & 15;
	az = (mlib_addr)z & 15;

	nstep = 16 / sizeof (mlib_s8);
	n1 = ((16 - ((mlib_addr)x & 15)) & 15) / sizeof (mlib_s8);
	n2 = (n - n1) / nstep;
	n3 = n - n1 - n2 * nstep;

	if (n2 < 1) {
		for (i = 0; i < n; i++) {
			sum = (*px++) - (*py++);
			SAT_S8(sum);
			*pz++ = sum;
		}
	} else {
		for (i = 0; i < n1; i++) {
			sum = (*px++) - (*py++);
			SAT_S8(sum);
			*pz++ = sum;
		}

		VECTOR_SUB(_mm_subs_epi8);

		for (i = 0; i < n3; i++) {
			sum = (*px++) - (*py++);
			SAT_S8(sum);
			*pz++ = sum;
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VectorSub_S8_Sat(
	mlib_s8 *xz,
	const mlib_s8 *y,
	mlib_s32 n)
{
	return (__mlib_VectorSub_S8_S8_Sat(xz, xz, y, n));
}

/* *********************************************************** */

mlib_status
__mlib_VectorSub_S8_S8_Mod(
	mlib_s8 *z,
	const mlib_s8 *x,
	const mlib_s8 *y,
	mlib_s32 n)
{
	mlib_s32 i, sum, ax, ay, az, nstep, n1, n2, n3;
	mlib_s8 *px = (mlib_s8 *)x, *py = (mlib_s8 *)y, *pz = (mlib_s8 *)z;
	__m128i xbuf, ybuf, zbuf;

	if (n < 1)
		return (MLIB_FAILURE);

	ax = (mlib_addr)x & 15;
	ay = (mlib_addr)y & 15;
	az = (mlib_addr)z & 15;

	nstep = 16 / sizeof (mlib_s8);
	n1 = ((16 - ((mlib_addr)x & 15)) & 15) / sizeof (mlib_s8);
	n2 = (n - n1) / nstep;
	n3 = n - n1 - n2 * nstep;

	if (n2 < 1) {
		for (i = 0; i < n; i++) {
			sum = (*px++) - (*py++);
			*pz++ = sum;
		}
	} else {
		for (i = 0; i < n1; i++) {
			sum = (*px++) - (*py++);
			*pz++ = sum;
		}

		VECTOR_SUB(_mm_sub_epi8);

		for (i = 0; i < n3; i++) {
			sum = (*px++) - (*py++);
			*pz++ = sum;
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VectorSub_S8_Mod(
	mlib_s8 *xz,
	const mlib_s8 *y,
	mlib_s32 n)
{
	return (__mlib_VectorSub_S8_S8_Mod(xz, xz, y, n));
}

/* *********************************************************** */

mlib_status
__mlib_VectorSub_S16_S16_Sat(
	mlib_s16 *z,
	const mlib_s16 *x,
	const mlib_s16 *y,
	mlib_s32 n)
{
	mlib_s32 i, sum, ax, ay, az, nstep, n1, n2, n3;
	mlib_s16 *px = (mlib_s16 *)x, *py = (mlib_s16 *)y, *pz = (mlib_s16 *)z;
	__m128i xbuf, ybuf, zbuf;

	if (n < 1)
		return (MLIB_FAILURE);

	ax = (mlib_addr)x & 15;
	ay = (mlib_addr)y & 15;
	az = (mlib_addr)z & 15;

	nstep = 16 / sizeof (mlib_s16);
	n1 = ((16 - ((mlib_addr)x & 15)) & 15) / sizeof (mlib_s16);
	n2 = (n - n1) / nstep;
	n3 = n - n1 - n2 * nstep;

	if (n2 < 1) {
		for (i = 0; i < n; i++) {
			sum = (*px++) - (*py++);
			SAT_S16(sum);
			*pz++ = sum;
		}
	} else {
		for (i = 0; i < n1; i++) {
			sum = (*px++) - (*py++);
			SAT_S16(sum);
			*pz++ = sum;
		}

		VECTOR_SUB(_mm_subs_epi16);

		for (i = 0; i < n3; i++) {
			sum = (*px++) - (*py++);
			SAT_S16(sum);
			*pz++ = sum;
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VectorSub_S16_Sat(
	mlib_s16 *xz,
	const mlib_s16 *y,
	mlib_s32 n)
{
	return (__mlib_VectorSub_S16_S16_Sat(xz, xz, y, n));
}

/* *********************************************************** */

mlib_status
__mlib_VectorSub_S16_S16_Mod(
	mlib_s16 *z,
	const mlib_s16 *x,
	const mlib_s16 *y,
	mlib_s32 n)
{
	mlib_s32 i, sum, ax, ay, az, nstep, n1, n2, n3;
	mlib_s16 *px = (mlib_s16 *)x, *py = (mlib_s16 *)y, *pz = (mlib_s16 *)z;
	__m128i xbuf, ybuf, zbuf;

	if (n < 1)
		return (MLIB_FAILURE);

	ax = (mlib_addr)x & 15;
	ay = (mlib_addr)y & 15;
	az = (mlib_addr)z & 15;

	nstep = 16 / sizeof (mlib_s16);
	n1 = ((16 - ((mlib_addr)x & 15)) & 15) / sizeof (mlib_s16);
	n2 = (n - n1) / nstep;
	n3 = n - n1 - n2 * nstep;

	if (n2 < 1) {
		for (i = 0; i < n; i++) {
			sum = (*px++) - (*py++);
			*pz++ = sum;
		}
	} else {
		for (i = 0; i < n1; i++) {
			sum = (*px++) - (*py++);
			*pz++ = sum;
		}

		VECTOR_SUB(_mm_sub_epi16);

		for (i = 0; i < n3; i++) {
			sum = (*px++) - (*py++);
			*pz++ = sum;
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VectorSub_S16_Mod(
	mlib_s16 *xz,
	const mlib_s16 *y,
	mlib_s32 n)
{
	return (__mlib_VectorSub_S16_S16_Mod(xz, xz, y, n));
}

/* *********************************************************** */

mlib_status
__mlib_VectorSub_S32_S16_Mod(
	mlib_s32 *z,
	const mlib_s16 *x,
	const mlib_s16 *y,
	mlib_s32 n)
{
	mlib_s32 i, sum, ax, ay, az, nstep, n1, n2, n3;
	mlib_s16 *px = (mlib_s16 *)x, *py = (mlib_s16 *)y;
	mlib_s32 *pz = (mlib_s32 *)z;
	__m128i xbuf, ybuf, zbuf, xbuf1, ybuf1, zbuf1, zero, xext, yext;

	if (n < 1)
		return (MLIB_FAILURE);

	zero = _mm_setzero_si128();

	ax = (mlib_addr)x & 15;
	ay = (mlib_addr)y & 15;
	az = (mlib_addr)z & 15;

	nstep = 16 / sizeof (mlib_s16);
	n1 = ((16 - ((mlib_addr)x & 15)) & 15) / sizeof (mlib_s16);
	n2 = (n - n1) / nstep;
	n3 = n - n1 - n2 * nstep;

	if (n2 < 1) {
		for (i = 0; i < n; i++) {
			*pz++ = (mlib_s32)(*px++) - (*py++);
		}
	} else {
		for (i = 0; i < n1; i++) {
			*pz++ = (mlib_s32)(*px++) - (*py++);
		}

		if (ax == ay && (ax * 2 & 15) == az) {
			for (i = 0; i < n2; i++) {
				xbuf  = _mm_load_si128((__m128i *)px);
				ybuf  = _mm_load_si128((__m128i *)py);
				xext  = _mm_cmpgt_epi16(zero, xbuf);
				yext  = _mm_cmpgt_epi16(zero, ybuf);
				xbuf1 = _mm_unpacklo_epi16(xbuf, xext);
				ybuf1 = _mm_unpacklo_epi16(ybuf, yext);
				xbuf  = _mm_unpackhi_epi16(xbuf, xext);
				ybuf  = _mm_unpackhi_epi16(ybuf, yext);
				zbuf1 = _mm_sub_epi32(xbuf1, ybuf1);
				zbuf  = _mm_sub_epi32(xbuf, ybuf);
				_mm_store_si128((__m128i *)pz, zbuf1);
				_mm_store_si128((__m128i *)pz + 1, zbuf);
				px += nstep;
				py += nstep;
				pz += nstep;
			}
		} else if (ax == ay) {
			for (i = 0; i < n2; i++) {
				xbuf  = _mm_load_si128((__m128i *)px);
				ybuf  = _mm_load_si128((__m128i *)py);
				xext  = _mm_cmpgt_epi16(zero, xbuf);
				yext  = _mm_cmpgt_epi16(zero, ybuf);
				xbuf1 = _mm_unpacklo_epi16(xbuf, xext);
				ybuf1 = _mm_unpacklo_epi16(ybuf, yext);
				xbuf  = _mm_unpackhi_epi16(xbuf, xext);
				ybuf  = _mm_unpackhi_epi16(ybuf, yext);
				zbuf1 = _mm_sub_epi32(xbuf1, ybuf1);
				zbuf  = _mm_sub_epi32(xbuf, ybuf);
				_mm_storeu_si128((__m128i *)pz, zbuf1);
				_mm_storeu_si128((__m128i *)pz + 1, zbuf);
				px += nstep;
				py += nstep;
				pz += nstep;
			}
		} else if ((ax * 2 & 15) == az) {
			for (i = 0; i < n2; i++) {
				xbuf  = _mm_load_si128((__m128i *)px);
				ybuf  = _mm_loadu_si128((__m128i *)py);
				xext  = _mm_cmpgt_epi16(zero, xbuf);
				yext  = _mm_cmpgt_epi16(zero, ybuf);
				xbuf1 = _mm_unpacklo_epi16(xbuf, xext);
				ybuf1 = _mm_unpacklo_epi16(ybuf, yext);
				xbuf  = _mm_unpackhi_epi16(xbuf, xext);
				ybuf  = _mm_unpackhi_epi16(ybuf, yext);
				zbuf1 = _mm_sub_epi32(xbuf1, ybuf1);
				zbuf  = _mm_sub_epi32(xbuf, ybuf);
				_mm_store_si128((__m128i *)pz, zbuf1);
				_mm_store_si128((__m128i *)pz + 1, zbuf);
				px += nstep;
				py += nstep;
				pz += nstep;
			}
		} else {
			for (i = 0; i < n2; i++) {
				xbuf  = _mm_load_si128((__m128i *)px);
				ybuf  = _mm_loadu_si128((__m128i *)py);
				xext  = _mm_cmpgt_epi16(zero, xbuf);
				yext  = _mm_cmpgt_epi16(zero, ybuf);
				xbuf1 = _mm_unpacklo_epi16(xbuf, xext);
				ybuf1 = _mm_unpacklo_epi16(ybuf, yext);
				xbuf  = _mm_unpackhi_epi16(xbuf, xext);
				ybuf  = _mm_unpackhi_epi16(ybuf, yext);
				zbuf1 = _mm_sub_epi32(xbuf1, ybuf1);
				zbuf  = _mm_sub_epi32(xbuf, ybuf);
				_mm_storeu_si128((__m128i *)pz, zbuf1);
				_mm_storeu_si128((__m128i *)pz + 1, zbuf);
				px += nstep;
				py += nstep;
				pz += nstep;
			}
		}

		for (i = 0; i < n3; i++) {
			*pz++ = (mlib_s32)(*px++) - (*py++);
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VectorSub_S32_S32_Sat(
	mlib_s32 *z,
	const mlib_s32 *x,
	const mlib_s32 *y,
	mlib_s32 n)
{
	const mlib_s32 *px = x, *py = y;
	mlib_s32 i, *pz = z;
	mlib_d64 sum;
	__m128i xbuf, ybuf, zbuf;
	__m128d buf1, buf2, buf3, dmin, dmax;

	if (n < 1)
		return (MLIB_FAILURE);

	dmin = _mm_set1_pd(MLIB_S32_MIN);
	dmax = _mm_set1_pd(MLIB_S32_MAX);

	for (i = 0; i < n / 2; i++) {
		xbuf = _mm_loadl_epi64((__m128i *)px);
		ybuf = _mm_loadl_epi64((__m128i *)py);

		buf1 = _mm_cvtepi32_pd(xbuf);
		buf2 = _mm_cvtepi32_pd(ybuf);
		buf3 = _mm_sub_pd(buf1, buf2);
		buf3 = _mm_max_pd(buf3, dmin);
		buf3 = _mm_min_pd(buf3, dmax);
		zbuf = _mm_cvtpd_epi32(buf3);

		_mm_storel_epi64((__m128i *)pz, zbuf);
		px += 2;
		py += 2;
		pz += 2;
	}

	if (n & 1) {
		sum = (mlib_d64)(*px++) - (*py++);
		if (sum > MLIB_S32_MAX)
			*pz++ = MLIB_S32_MAX;
		else if (sum < MLIB_S32_MIN)
			*pz++ = MLIB_S32_MIN;
		else
			*pz++ = sum;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VectorSub_S32_Sat(
	mlib_s32 *xz,
	const mlib_s32 *y,
	mlib_s32 n)
{
	return (__mlib_VectorSub_S32_S32_Sat(xz, xz, y, n));
}

/* *********************************************************** */

/* workaround for CR 6562805, should be removed if it is fixed */
#define	CR_6562805

#ifdef	CR_6562805
static void mmsub0(int *px, int *py, int *pz, int n2, int nstep)
{
	int i;
	__m128i xbuf, ybuf, zbuf;

	for (i = 0; i < n2; i++) {
		xbuf = _mm_load_si128((__m128i *)px);
		ybuf = _mm_load_si128((__m128i *)py);
		zbuf = _mm_sub_epi32(xbuf, ybuf);
		_mm_store_si128((__m128i *)pz, zbuf);
		px += nstep;
		py += nstep;
		pz += nstep;
	}
}

static void mmsub1(int *px, int *py, int *pz, int n2, int nstep)
{
	int i;
	__m128i xbuf, ybuf, zbuf;

	for (i = 0; i < n2; i++) {
		xbuf = _mm_load_si128((__m128i *)px);
		ybuf = _mm_load_si128((__m128i *)py);
		zbuf = _mm_sub_epi32(xbuf, ybuf);
		_mm_storeu_si128((__m128i *)pz, zbuf);
		px += nstep;
		py += nstep;
		pz += nstep;
	}
}

static void mmsub2(int *px, int *py, int *pz, int n2, int nstep)
{
	int i;
	__m128i xbuf, ybuf, zbuf;

	for (i = 0; i < n2; i++) {
		xbuf = _mm_load_si128((__m128i *)px);
		ybuf = _mm_loadu_si128((__m128i *)py);
		zbuf = _mm_sub_epi32(xbuf, ybuf);
		_mm_store_si128((__m128i *)pz, zbuf);
		px += nstep;
		py += nstep;
		pz += nstep;
	}
}

static void mmsub3(int *px, int *py, int *pz, int n2, int nstep)
{
	int i;
	__m128i xbuf, ybuf, zbuf;

	for (i = 0; i < n2; i++) {
		xbuf = _mm_load_si128((__m128i *)px);
		ybuf = _mm_loadu_si128((__m128i *)py);
		zbuf = _mm_sub_epi32(xbuf, ybuf);
		_mm_storeu_si128((__m128i *)pz, zbuf);
		px += nstep;
		py += nstep;
		pz += nstep;
	}
}
#endif

mlib_status
__mlib_VectorSub_S32_S32_Mod(
	mlib_s32 *z,
	const mlib_s32 *x,
	const mlib_s32 *y,
	mlib_s32 n)
{
	mlib_s32 i, ax, ay, az, nstep, n1, n2, n3;
	mlib_s32 *px = (mlib_s32 *)x, *py = (mlib_s32 *)y, *pz = (mlib_s32 *)z;
	__m128i xbuf, ybuf, zbuf;

	if (n < 1)
		return (MLIB_FAILURE);

	ax = (mlib_addr)x & 15;
	ay = (mlib_addr)y & 15;
	az = (mlib_addr)z & 15;

	nstep = 16 / sizeof (mlib_s32);
	n1 = ((16 - ((mlib_addr)x & 15)) & 15) / sizeof (mlib_s32);
	n2 = (n - n1) / nstep;
	n3 = n - n1 - n2 * nstep;

	if (n2 < 1) {
		for (i = 0; i < n; i++) {
			*pz++ = (*px++) - (*py++);
		}
	} else {
		for (i = 0; i < n1; i++) {
			*pz++ = (*px++) - (*py++);
		}

#ifdef	CR_6562805
	if (ax == ay && ax == az) {
		mmsub0(px, py, pz, n2, nstep);
	} else if (ax == ay) {
		mmsub1(px, py, pz, n2, nstep);
	} else if (ax == az) {
		mmsub2(px, py, pz, n2, nstep);
	} else {
		mmsub3(px, py, pz, n2, nstep);
	}
	px += n2 * nstep;
	py += n2 * nstep;
	pz += n2 * nstep;
#else
		VECTOR_SUB(_mm_sub_epi32);
#endif

		for (i = 0; i < n3; i++) {
			*pz++ = (*px++) - (*py++);
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VectorSub_S32_Mod(
	mlib_s32 *xz,
	const mlib_s32 *y,
	mlib_s32 n)
{
	return (__mlib_VectorSub_S32_S32_Mod(xz, xz, y, n));
}

/* *********************************************************** */

mlib_status
__mlib_VectorSub_U8C_U8C_Sat(
	mlib_u8 *z,
	const mlib_u8 *x,
	const mlib_u8 *y,
	mlib_s32 n)
{
	return (__mlib_VectorSub_U8_U8_Sat(z, x, y, n + n));
}

/* *********************************************************** */

mlib_status
__mlib_VectorSub_S16C_U8C_Mod(
	mlib_s16 *z,
	const mlib_u8 *x,
	const mlib_u8 *y,
	mlib_s32 n)
{
	return (__mlib_VectorSub_S16_U8_Mod(z, x, y, n + n));
}

/* *********************************************************** */

mlib_status
__mlib_VectorSub_U8C_Sat(
	mlib_u8 *xz,
	const mlib_u8 *y,
	mlib_s32 n)
{
	return (__mlib_VectorSub_U8_U8_Sat(xz, xz, y, n + n));
}

/* *********************************************************** */

mlib_status
__mlib_VectorSub_U8C_U8C_Mod(
	mlib_u8 *z,
	const mlib_u8 *x,
	const mlib_u8 *y,
	mlib_s32 n)
{
	return (__mlib_VectorSub_U8_U8_Mod(z, x, y, n + n));
}

/* *********************************************************** */

mlib_status
__mlib_VectorSub_S16C_S8C_Mod(
	mlib_s16 *z,
	const mlib_s8 *x,
	const mlib_s8 *y,
	mlib_s32 n)
{
	return (__mlib_VectorSub_S16_S8_Mod(z, x, y, n + n));
}

/* *********************************************************** */

mlib_status
__mlib_VectorSub_U8C_Mod(
	mlib_u8 *xz,
	const mlib_u8 *y,
	mlib_s32 n)
{
	return (__mlib_VectorSub_U8_U8_Mod(xz, xz, y, n + n));
}

/* *********************************************************** */

mlib_status
__mlib_VectorSub_S8C_S8C_Sat(
	mlib_s8 *z,
	const mlib_s8 *x,
	const mlib_s8 *y,
	mlib_s32 n)
{
	return (__mlib_VectorSub_S8_S8_Sat(z, x, y, n + n));
}

/* *********************************************************** */

mlib_status
__mlib_VectorSub_S8C_Sat(
	mlib_s8 *xz,
	const mlib_s8 *y,
	mlib_s32 n)
{
	return (__mlib_VectorSub_S8_S8_Sat(xz, xz, y, n + n));
}

/* *********************************************************** */

mlib_status
__mlib_VectorSub_S8C_S8C_Mod(
	mlib_s8 *z,
	const mlib_s8 *x,
	const mlib_s8 *y,
	mlib_s32 n)
{
	return (__mlib_VectorSub_S8_S8_Mod(z, x, y, n + n));
}

/* *********************************************************** */

mlib_status
__mlib_VectorSub_S8C_Mod(
	mlib_s8 *xz,
	const mlib_s8 *y,
	mlib_s32 n)
{
	return (__mlib_VectorSub_S8_S8_Mod(xz, xz, y, n + n));
}

/* *********************************************************** */

mlib_status
__mlib_VectorSub_S16C_S16C_Sat(
	mlib_s16 *z,
	const mlib_s16 *x,
	const mlib_s16 *y,
	mlib_s32 n)
{
	return (__mlib_VectorSub_S16_S16_Sat(z, x, y, n + n));
}

/* *********************************************************** */

mlib_status
__mlib_VectorSub_S32C_S16C_Mod(
	mlib_s32 *z,
	const mlib_s16 *x,
	const mlib_s16 *y,
	mlib_s32 n)
{
	return (__mlib_VectorSub_S32_S16_Mod(z, x, y, n + n));
}

/* *********************************************************** */

mlib_status
__mlib_VectorSub_S16C_Sat(
	mlib_s16 *xz,
	const mlib_s16 *y,
	mlib_s32 n)
{
	return (__mlib_VectorSub_S16_S16_Sat(xz, xz, y, n + n));
}

/* *********************************************************** */

mlib_status
__mlib_VectorSub_S16C_S16C_Mod(
	mlib_s16 *z,
	const mlib_s16 *x,
	const mlib_s16 *y,
	mlib_s32 n)
{
	return (__mlib_VectorSub_S16_S16_Mod(z, x, y, n + n));
}

/* *********************************************************** */

mlib_status
__mlib_VectorSub_S16C_Mod(
	mlib_s16 *xz,
	const mlib_s16 *y,
	mlib_s32 n)
{
	return (__mlib_VectorSub_S16_S16_Mod(xz, xz, y, n + n));
}

/* *********************************************************** */

mlib_status
__mlib_VectorSub_S32C_S32C_Sat(
	mlib_s32 *z,
	const mlib_s32 *x,
	const mlib_s32 *y,
	mlib_s32 n)
{
	return (__mlib_VectorSub_S32_S32_Sat(z, x, y, n + n));
}

/* *********************************************************** */

mlib_status
__mlib_VectorSub_S32C_Sat(
	mlib_s32 *xz,
	const mlib_s32 *y,
	mlib_s32 n)
{
	return (__mlib_VectorSub_S32_S32_Sat(xz, xz, y, n + n));
}

/* *********************************************************** */

mlib_status
__mlib_VectorSub_S32C_S32C_Mod(
	mlib_s32 *z,
	const mlib_s32 *x,
	const mlib_s32 *y,
	mlib_s32 n)
{
	return (__mlib_VectorSub_S32_S32_Mod(z, x, y, n + n));
}

/* *********************************************************** */

mlib_status
__mlib_VectorSub_S32C_Mod(
	mlib_s32 *xz,
	const mlib_s32 *y,
	mlib_s32 n)
{
	return (__mlib_VectorSub_S32_S32_Mod(xz, xz, y, n + n));
}

/* *********************************************************** */
