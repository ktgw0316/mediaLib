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

#pragma ident	"@(#)mlib_s_VectorAve.c	9.5	07/11/05 SMI"

/*
 * FUNCTIONS
 *      mlib_VectorAve_[U8_U8|S8_S8],
 *      mlib_VectorAve_S16_[U8|S8],
 *      mlib_VectorAve_[U8|S8] - average of signed/unsigned
 *                               8-bit format vectors
 *
 *      mlib_VectorAve_[S16|S32]_S16,
 *      mlib_VectorAve_S16 - average of signed 16-bit format vectors
 *
 *      mlib_VectorAve_S32_S32,
 *      mlib_VectorAve_S32 - average of signed 32-bit format vectors
 *
 * SYNOPSIS
 *      mlib_status mlib_VectorAve_U8_U8(mlib_u8       *z,
 *                                       const mlib_u8 *x,
 *                                       const mlib_u8 *y,
 *                                       mlib_s32      n)
 *      mlib_status mlib_VectorAve_S8_S8(mlib_s8       *z,
 *                                       const mlib_s8 *x,
 *                                       const mlib_s8 *y,
 *                                       mlib_s32      n)
 *      mlib_status mlib_VectorAve_S16_U8(mlib_s16      *z,
 *                                        const mlib_u8 *x,
 *                                        const mlib_u8 *y,
 *                                        mlib_s32      n)
 *      mlib_status mlib_VectorAve_S16_S8(mlib_s16      *z,
 *                                        const mlib_s8 *x,
 *                                        const mlib_s8 *y,
 *                                        mlib_s32      n)
 *      mlib_status mlib_VectorAve_S16_S16(mlib_s16       *z,
 *                                         const mlib_s16 *x,
 *                                         const mlib_s16 *y,
 *                                         mlib_s32       n)
 *      mlib_status mlib_VectorAve_S32_S16(mlib_s32       *z,
 *                                         const mlib_s16 *x,
 *                                         const mlib_s16 *y,
 *                                         mlib_s32       n)
 *      mlib_status mlib_VectorAve_S32_S32(mlib_s32       *z,
 *                                         const mlib_s32 *x,
 *                                         const mlib_s32 *y,
 *                                         mlib_s32       n)
 *
 *      mlib_status mlib_VectorAve_U8(mlib_u8       *xz,
 *                                    const mlib_u8 *y,
 *                                    mlib_s32      n)
 *      mlib_status mlib_VectorAve_S8(mlib_s8       *xz,
 *                                    const mlib_s8 *y,
 *                                    mlib_s32      n)
 *      mlib_status mlib_VectorAve_S16(mlib_s16       *xz,
 *                                     const mlib_s16 *y,
 *                                     mlib_s32       n)
 *      mlib_status mlib_VectorAve_S32(mlib_s32       *xz,
 *                                     const mlib_s32 *y,
 *                                     mlib_s32       n)
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
 *      z = (x + y + 1) / 2
 */

/*
 * FUNCTIONS
 *      mlib_VectorAve_[U8C_U8C|S8C_S8C],
 *      mlib_VectorAve_S16C_[U8C|S8C],
 *      mlib_VectorAve_[U8C|S8C] - average of signed/unsigned
 *                                 8-bit format complex vectors
 *
 *      mlib_VectorAve_[S16C|S32C]_S16C,
 *      mlib_VectorAve_S16C - average of signed 16-bit format complex vectors
 *
 *      mlib_VectorAve_S32C_S32C,
 *      mlib_VectorAve_S32C - average of signed 32-bit format complex vectors
 *
 * SYNOPSIS
 *      mlib_status mlib_VectorAve_U8C_U8C(mlib_u8       *z,
 *                                         const mlib_u8 *x,
 *                                         const mlib_u8 *y,
 *                                         mlib_s32      n)
 *      mlib_status mlib_VectorAve_S8C_S8C(mlib_s8       *z,
 *                                         const mlib_s8 *x,
 *                                         const mlib_s8 *y,
 *                                         mlib_s32      n)
 *      mlib_status mlib_VectorAve_S16C_U8C(mlib_s16      *z,
 *                                          const mlib_u8 *x,
 *                                          const mlib_u8 *y,
 *                                          mlib_s32      n)
 *      mlib_status mlib_VectorAve_S16C_S8C(mlib_s16      *z,
 *                                          const mlib_s8 *x,
 *                                          const mlib_s8 *y,
 *                                          mlib_s32      n)
 *      mlib_status mlib_VectorAve_S16C_S16C(mlib_s16       *z,
 *                                           const mlib_s16 *x,
 *                                           const mlib_s16 *y,
 *                                           mlib_s32       n)
 *      mlib_status mlib_VectorAve_S32C_S16C(mlib_s32       *z,
 *                                           const mlib_s16 *x,
 *                                           const mlib_s16 *y,
 *                                           mlib_s32       n)
 *      mlib_status mlib_VectorAve_S32C_S32C(mlib_s32       *z,
 *                                           const mlib_s32 *x,
 *                                           const mlib_s32 *y,
 *                                           mlib_s32       n)
 *
 *      mlib_status mlib_VectorAve_U8C(mlib_u8       *xz,
 *                                     const mlib_u8 *y,
 *                                     mlib_s32      n)
 *      mlib_status mlib_VectorAve_S8C(mlib_s8       *xz,
 *                                     const mlib_s8 *y,
 *                                     mlib_s32      n)
 *      mlib_status mlib_VectorAve_S16C(mlib_s16       *xz,
 *                                      const mlib_s16 *y,
 *                                      mlib_s32       n)
 *      mlib_status mlib_VectorAve_S32C(mlib_s32       *xz,
 *                                      const mlib_s32 *y,
 *                                      mlib_s32       n)
 *
 * ARGUMENTS
 *      z    pointer to the first complex element of the result vector.
 *           z[2*i] hold the real parts, and z[2*i + 1] hold the imaginary
 *           parts
 *      xz   pointer to the first element of the first input and
 *           the output vector.
 *           xz[2*i] hold the real parts, and xz[2*i + 1] hold the imaginary
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
 *      z = (x + y + 1) / 2
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

#pragma weak mlib_VectorAve_S8 = __mlib_VectorAve_S8
#pragma weak mlib_VectorAve_U8 = __mlib_VectorAve_U8
#pragma weak mlib_VectorAve_S16	= __mlib_VectorAve_S16
#pragma weak mlib_VectorAve_S32	= __mlib_VectorAve_S32
#pragma weak mlib_VectorAve_S8C	= __mlib_VectorAve_S8C
#pragma weak mlib_VectorAve_U8C	= __mlib_VectorAve_U8C
#pragma weak mlib_VectorAve_S16C = __mlib_VectorAve_S16C
#pragma weak mlib_VectorAve_S32C = __mlib_VectorAve_S32C
#pragma weak mlib_VectorAve_S8_S8 = __mlib_VectorAve_S8_S8
#pragma weak mlib_VectorAve_U8_U8 = __mlib_VectorAve_U8_U8
#pragma weak mlib_VectorAve_S16_U8 = __mlib_VectorAve_S16_U8
#pragma weak mlib_VectorAve_S16_S8 = __mlib_VectorAve_S16_S8
#pragma weak mlib_VectorAve_S16_S16 = __mlib_VectorAve_S16_S16
#pragma weak mlib_VectorAve_S32_S16 = __mlib_VectorAve_S32_S16
#pragma weak mlib_VectorAve_S32_S32 = __mlib_VectorAve_S32_S32
#pragma weak mlib_VectorAve_S8C_S8C = __mlib_VectorAve_S8C_S8C
#pragma weak mlib_VectorAve_U8C_U8C = __mlib_VectorAve_U8C_U8C
#pragma weak mlib_VectorAve_S16C_U8C = __mlib_VectorAve_S16C_U8C
#pragma weak mlib_VectorAve_S16C_S8C = __mlib_VectorAve_S16C_S8C
#pragma weak mlib_VectorAve_S16C_S16C = __mlib_VectorAve_S16C_S16C
#pragma weak mlib_VectorAve_S32C_S16C = __mlib_VectorAve_S32C_S16C
#pragma weak mlib_VectorAve_S32C_S32C = __mlib_VectorAve_S32C_S32C

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */

__typeof__(__mlib_VectorAve_S8) mlib_VectorAve_S8
	__attribute__((weak, alias("__mlib_VectorAve_S8")));
__typeof__(__mlib_VectorAve_U8) mlib_VectorAve_U8
	__attribute__((weak, alias("__mlib_VectorAve_U8")));
__typeof__(__mlib_VectorAve_S16) mlib_VectorAve_S16
	__attribute__((weak, alias("__mlib_VectorAve_S16")));
__typeof__(__mlib_VectorAve_S32) mlib_VectorAve_S32
	__attribute__((weak, alias("__mlib_VectorAve_S32")));
__typeof__(__mlib_VectorAve_S8C) mlib_VectorAve_S8C
	__attribute__((weak, alias("__mlib_VectorAve_S8C")));
__typeof__(__mlib_VectorAve_U8C) mlib_VectorAve_U8C
	__attribute__((weak, alias("__mlib_VectorAve_U8C")));
__typeof__(__mlib_VectorAve_S16C) mlib_VectorAve_S16C
	__attribute__((weak, alias("__mlib_VectorAve_S16C")));
__typeof__(__mlib_VectorAve_S32C) mlib_VectorAve_S32C
	__attribute__((weak, alias("__mlib_VectorAve_S32C")));
__typeof__(__mlib_VectorAve_S8_S8) mlib_VectorAve_S8_S8
	__attribute__((weak, alias("__mlib_VectorAve_S8_S8")));
__typeof__(__mlib_VectorAve_U8_U8) mlib_VectorAve_U8_U8
	__attribute__((weak, alias("__mlib_VectorAve_U8_U8")));
__typeof__(__mlib_VectorAve_S16_U8) mlib_VectorAve_S16_U8
	__attribute__((weak, alias("__mlib_VectorAve_S16_U8")));
__typeof__(__mlib_VectorAve_S16_S8) mlib_VectorAve_S16_S8
	__attribute__((weak, alias("__mlib_VectorAve_S16_S8")));
__typeof__(__mlib_VectorAve_S16_S16) mlib_VectorAve_S16_S16
	__attribute__((weak, alias("__mlib_VectorAve_S16_S16")));
__typeof__(__mlib_VectorAve_S32_S16) mlib_VectorAve_S32_S16
	__attribute__((weak, alias("__mlib_VectorAve_S32_S16")));
__typeof__(__mlib_VectorAve_S32_S32) mlib_VectorAve_S32_S32
	__attribute__((weak, alias("__mlib_VectorAve_S32_S32")));
__typeof__(__mlib_VectorAve_S8C_S8C) mlib_VectorAve_S8C_S8C
	__attribute__((weak, alias("__mlib_VectorAve_S8C_S8C")));
__typeof__(__mlib_VectorAve_U8C_U8C) mlib_VectorAve_U8C_U8C
	__attribute__((weak, alias("__mlib_VectorAve_U8C_U8C")));
__typeof__(__mlib_VectorAve_S16C_U8C) mlib_VectorAve_S16C_U8C
	__attribute__((weak, alias("__mlib_VectorAve_S16C_U8C")));
__typeof__(__mlib_VectorAve_S16C_S8C) mlib_VectorAve_S16C_S8C
	__attribute__((weak, alias("__mlib_VectorAve_S16C_S8C")));
__typeof__(__mlib_VectorAve_S16C_S16C) mlib_VectorAve_S16C_S16C
	__attribute__((weak, alias("__mlib_VectorAve_S16C_S16C")));
__typeof__(__mlib_VectorAve_S32C_S16C) mlib_VectorAve_S32C_S16C
	__attribute__((weak, alias("__mlib_VectorAve_S32C_S16C")));
__typeof__(__mlib_VectorAve_S32C_S32C) mlib_VectorAve_S32C_S32C
	__attribute__((weak, alias("__mlib_VectorAve_S32C_S32C")));
#else /* defined(__SUNPRO_C) */

#error	"unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	AVE_U8_U8(ld1, ld2, st)				\
	xbuf = _mm_##ld1##_si128((__m128i *)px);	\
	ybuf = _mm_##ld2##_si128((__m128i *)py);	\
	zbuf = _mm_avg_epu8(xbuf, ybuf);		\
	_mm_##st##_si128((__m128i *)pz, zbuf);		\
	px += nstep;					\
	py += nstep;					\
	pz += nstep

mlib_status
__mlib_VectorAve_U8_U8(
	mlib_u8	*z,
	const mlib_u8 *x,
	const mlib_u8 *y,
	mlib_s32 n)
{
	if (n < 1)
		return (MLIB_FAILURE);

	mlib_s32 i, sum, ax, ay, az, nstep, n1, n2, n3;
	const mlib_u8 *px = x, *py = y;
	mlib_u8 *pz = z;
	__m128i xbuf, ybuf, zbuf, buf1, buf2;

	ax = (mlib_addr)x & 15;
	ay = (mlib_addr)y & 15;
	az = (mlib_addr)z & 15;

	nstep = 16 / sizeof (mlib_u8);
	n1 = ((16 - ax) & 15) / sizeof (mlib_u8);
	n2 = (n - n1) / nstep;
	n3 = n - n1 - n2 * nstep;

	if (n2 < 1) {
		for (i = 0; i < n; i++) {
			sum = (*px++) + (*py++);
			*pz++ = (sum + 1) >> 1;
		}
	} else {
		for (i = 0; i < n1; i++) {
			sum = (*px++) + (*py++);
			*pz++ = (sum + 1) >> 1;
		}

		if (ax == ay && ax == az) {
			for (i = 0; i < n2; i++) {
				AVE_U8_U8(load, load, store);
			}
		} else if (ax == ay) {
			for (i = 0; i < n2; i++) {
				AVE_U8_U8(load, load, storeu);
			}
		} else {
			for (i = 0; i < n2; i++) {
				AVE_U8_U8(load, loadu, storeu);
			}
		}

		for (i = 0; i < n3; i++) {
			sum = (*px++) + (*py++);
			*pz++ = (sum + 1) >> 1;
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#define	AVE_S8_S8(ld1, ld2, st)				\
	xbuf = _mm_##ld1##_si128((__m128i *)px);	\
	ybuf = _mm_##ld2##_si128((__m128i *)py);	\
	xbuf = _mm_sub_epi8(xbuf, incs);		\
	ybuf = _mm_sub_epi8(ybuf, incs);		\
	zbuf = _mm_avg_epu8(xbuf, ybuf);		\
	zbuf = _mm_add_epi8(zbuf, incs);		\
	_mm_##st##_si128((__m128i *)pz, zbuf);		\
	px += nstep;					\
	py += nstep;					\
	pz += nstep

mlib_status
__mlib_VectorAve_S8_S8(
	mlib_s8	*z,
	const mlib_s8 *x,
	const mlib_s8 *y,
	mlib_s32 n)
{
	if (n < 1)
		return (MLIB_FAILURE);

	mlib_s32 i, sum, ax, ay, az, nstep, n1, n2, n3;
	const mlib_s8 *px = x, *py = y;
	mlib_s8 *pz = z;
	__m128i xbuf, ybuf, zbuf, buf1, buf2, incs;
	incs = _mm_set1_epi8(MLIB_S8_MIN);

	ax = (mlib_addr)x & 15;
	ay = (mlib_addr)y & 15;
	az = (mlib_addr)z & 15;

	nstep = 16 / sizeof (mlib_s8);
	n1 = ((16 - ax) & 15) / sizeof (mlib_s8);
	n2 = (n - n1) / nstep;
	n3 = n - n1 - n2 * nstep;

	if (n2 < 1) {
		for (i = 0; i < n; i++) {
			sum = (*px++) + (*py++);
			*pz++ = (sum + 1) >> 1;
		}
	} else {
		for (i = 0; i < n1; i++) {
			sum = (*px++) + (*py++);
			*pz++ = (sum + 1) >> 1;
		}

		if (ax == ay && ax == az) {
			for (i = 0; i < n2; i++) {
				AVE_S8_S8(load, load, store);
			}
		} else if (ax == ay) {
			for (i = 0; i < n2; i++) {
				AVE_S8_S8(load, load, storeu);
			}
		} else {
			for (i = 0; i < n2; i++) {
				AVE_S8_S8(load, loadu, storeu);
			}
		}

		for (i = 0; i < n3; i++) {
			sum = (*px++) + (*py++);
			*pz++ = (sum + 1) >> 1;
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#define	AVE_S16_U8(ld1, ld2, st)			\
	xbuf = _mm_##ld1##_si128((__m128i *)px);	\
	ybuf = _mm_##ld2##_si128((__m128i *)py);	\
	zbuf = _mm_avg_epu8(xbuf, ybuf);		\
	buf1 = _mm_unpacklo_epi8(zbuf, zero);		\
	buf2 = _mm_unpackhi_epi8(zbuf, zero);		\
	_mm_##st##_si128((__m128i *)pz, buf1);		\
	_mm_##st##_si128((__m128i *)pz + 1, buf2);	\
	px += nstep;					\
	py += nstep;					\
	pz += nstep

mlib_status
__mlib_VectorAve_S16_U8(
	mlib_s16 *z,
	const mlib_u8 *x,
	const mlib_u8 *y,
	mlib_s32 n)
{
	if (n < 1)
		return (MLIB_FAILURE);

	mlib_s32 i, sum, ax, ay, az, nstep, n1, n2, n3;
	const mlib_u8 *px = x, *py = y;
	mlib_s16 *pz = z;
	__m128i xbuf, ybuf, zbuf, buf1, buf2, zero;
	zero = _mm_setzero_si128();

	ax = (mlib_addr)x & 15;
	ay = (mlib_addr)y & 15;
	az = (mlib_addr)z & 15;

	nstep = 16 / sizeof (mlib_u8);
	n1 = ((16 - ax) & 15) / sizeof (mlib_u8);
	n2 = (n - n1) / nstep;
	n3 = n - n1 - n2 * nstep;

	if (n2 < 1) {
		for (i = 0; i < n; i++) {
			sum = (*px++) + (*py++);
			*pz++ = (sum + 1) >> 1;
		}
	} else {
		for (i = 0; i < n1; i++) {
			sum = (*px++) + (*py++);
			*pz++ = (sum + 1) >> 1;
		}

		if (ax == ay && (ax * 2 & 15) == az) {
			for (i = 0; i < n2; i++) {
				AVE_S16_U8(load, load, store);
			}
		} else if (ax == ay) {
			for (i = 0; i < n2; i++) {
				AVE_S16_U8(load, load, storeu);
			}
		} else {
			for (i = 0; i < n2; i++) {
				AVE_S16_U8(load, loadu, storeu);
			}
		}

		for (i = 0; i < n3; i++) {
			sum = (*px++) + (*py++);
			*pz++ = (sum + 1) >> 1;
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */


#define	AVE_S16_S8(ld1, ld2, st)			\
	xbuf = _mm_##ld1##_si128((__m128i *)px);	\
	ybuf = _mm_##ld2##_si128((__m128i *)py);	\
	xbuf = _mm_sub_epi8(xbuf, incs);		\
	ybuf = _mm_sub_epi8(ybuf, incs);		\
	zbuf = _mm_avg_epu8(xbuf, ybuf);		\
	zbuf = _mm_add_epi8(zbuf, incs);		\
	buf1 = _mm_unpacklo_epi8(zero, zbuf);		\
	buf2 = _mm_unpackhi_epi8(zero, zbuf);		\
	buf1 = _mm_srai_epi16(buf1, 8);			\
	buf2 = _mm_srai_epi16(buf2, 8);			\
	_mm_##st##_si128((__m128i *)pz, buf1);		\
	_mm_##st##_si128((__m128i *)pz + 1, buf2);	\
	px += nstep;					\
	py += nstep;					\
	pz += nstep

mlib_status
__mlib_VectorAve_S16_S8(
	mlib_s16 *z,
	const mlib_s8 *x,
	const mlib_s8 *y,
	mlib_s32 n)
{
	if (n < 1)
		return (MLIB_FAILURE);

	mlib_s32 i, sum, ax, ay, az, nstep, n1, n2, n3;
	const mlib_s8 *px = x, *py = y;
	mlib_s16 *pz = z;
	__m128i xbuf, ybuf, zbuf, buf1, buf2, zero, incs;
	zero = _mm_setzero_si128();
	incs = _mm_set1_epi8(MLIB_S8_MIN);

	ax = (mlib_addr)x & 15;
	ay = (mlib_addr)y & 15;
	az = (mlib_addr)z & 15;

	nstep = 16 / sizeof (mlib_s8);
	n1 = ((16 - ax) & 15) / sizeof (mlib_s8);
	n2 = (n - n1) / nstep;
	n3 = n - n1 - n2 * nstep;

	if (n2 < 1) {
		for (i = 0; i < n; i++) {
			sum = (*px++) + (*py++);
			*pz++ = (sum + 1) >> 1;
		}
	} else {
		for (i = 0; i < n1; i++) {
			sum = (*px++) + (*py++);
			*pz++ = (sum + 1) >> 1;
		}

		if (ax == ay && (ax * 2 & 15) == az) {
			for (i = 0; i < n2; i++) {
				AVE_S16_S8(load, load, store);
			}
		} else if (ax == ay) {
			for (i = 0; i < n2; i++) {
				AVE_S16_S8(load, load, storeu);
			}
		} else {
			for (i = 0; i < n2; i++) {
				AVE_S16_S8(load, loadu, storeu);
			}
		}

		for (i = 0; i < n3; i++) {
			sum = (*px++) + (*py++);
			*pz++ = (sum + 1) >> 1;
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#define	AVE_S16_S16(ld1, ld2, st)			\
	xbuf = _mm_##ld1##_si128((__m128i *)px);	\
	ybuf = _mm_##ld2##_si128((__m128i *)py);	\
	xbuf = _mm_sub_epi16(xbuf, incs);		\
	ybuf = _mm_sub_epi16(ybuf, incs);		\
	zbuf = _mm_avg_epu16(xbuf, ybuf);		\
	zbuf = _mm_add_epi16(zbuf, incs);		\
	_mm_##st##_si128((__m128i *)pz, zbuf);		\
	px += nstep;					\
	py += nstep;					\
	pz += nstep

mlib_status
__mlib_VectorAve_S16_S16(
	mlib_s16 *z,
	const mlib_s16 *x,
	const mlib_s16 *y,
	mlib_s32 n)
{
	if (n < 1)
		return (MLIB_FAILURE);

	mlib_s32 i, sum, ax, ay, az, nstep, n1, n2, n3;
	const mlib_s16 *px = x, *py = y;
	mlib_s16 *pz = z;
	__m128i xbuf, ybuf, zbuf, buf1, buf2, incs;
	incs = _mm_set1_epi16(MLIB_S16_MIN);

	ax = (mlib_addr)x & 15;
	ay = (mlib_addr)y & 15;
	az = (mlib_addr)z & 15;

	nstep = 16 / sizeof (mlib_s16);
	n1 = ((16 - ax) & 15) / sizeof (mlib_s16);
	n2 = (n - n1) / nstep;
	n3 = n - n1 - n2 * nstep;

	if (n2 < 1) {
		for (i = 0; i < n; i++) {
			sum = (*px++) + (*py++);
			*pz++ = (sum + 1) >> 1;
		}
	} else {
		for (i = 0; i < n1; i++) {
			sum = (*px++) + (*py++);
			*pz++ = (sum + 1) >> 1;
		}

		if (ax == ay && ax == az) {
			for (i = 0; i < n2; i++) {
				AVE_S16_S16(load, load, store);
			}
		} else if (ax == ay) {
			for (i = 0; i < n2; i++) {
				AVE_S16_S16(load, load, storeu);
			}
		} else {
			for (i = 0; i < n2; i++) {
				AVE_S16_S16(load, loadu, storeu);
			}
		}

		for (i = 0; i < n3; i++) {
			sum = (*px++) + (*py++);
			*pz++ = (sum + 1) >> 1;
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#define	AVE_S32_S16(ld1, ld2, st)			\
	xbuf = _mm_##ld1##_si128((__m128i *)px);	\
	ybuf = _mm_##ld2##_si128((__m128i *)py);	\
	xbuf = _mm_sub_epi16(xbuf, incs);		\
	ybuf = _mm_sub_epi16(ybuf, incs);		\
	zbuf = _mm_avg_epu16(xbuf, ybuf);		\
	zbuf = _mm_add_epi16(zbuf, incs);		\
	buf1 = _mm_unpacklo_epi16(zero, zbuf);		\
	buf2 = _mm_unpackhi_epi16(zero, zbuf);		\
	buf1 = _mm_srai_epi32(buf1, 16);		\
	buf2 = _mm_srai_epi32(buf2, 16);		\
	_mm_##st##_si128((__m128i *)pz, buf1);		\
	_mm_##st##_si128((__m128i *)pz + 1, buf2);	\
	px += nstep;					\
	py += nstep;					\
	pz += nstep

mlib_status
__mlib_VectorAve_S32_S16(
	mlib_s32 *z,
	const mlib_s16 *x,
	const mlib_s16 *y,
	mlib_s32 n)
{
	if (n < 1)
		return (MLIB_FAILURE);

	mlib_s32 i, sum, ax, ay, az, nstep, n1, n2, n3;
	const mlib_s16 *px = x, *py = y;
	mlib_s32 *pz = z;
	__m128i xbuf, ybuf, zbuf, buf1, buf2, incs, zero;
	incs = _mm_set1_epi16(MLIB_S16_MIN);
	zero = _mm_setzero_si128();

	ax = (mlib_addr)x & 15;
	ay = (mlib_addr)y & 15;
	az = (mlib_addr)z & 15;

	nstep = 16 / sizeof (mlib_s16);
	n1 = ((16 - ax) & 15) / sizeof (mlib_s16);
	n2 = (n - n1) / nstep;
	n3 = n - n1 - n2 * nstep;

	if (n2 < 1) {
		for (i = 0; i < n; i++) {
			sum = (*px++) + (*py++);
			*pz++ = (sum + 1) >> 1;
		}
	} else {
		for (i = 0; i < n1; i++) {
			sum = (*px++) + (*py++);
			*pz++ = (sum + 1) >> 1;
		}

		if (ax == ay && (ax * 2 & 15) == az) {
			for (i = 0; i < n2; i++) {
				AVE_S32_S16(load, load, store);
			}
		} else if (ax == ay) {
			for (i = 0; i < n2; i++) {
				AVE_S32_S16(load, load, storeu);
			}
		} else {
			for (i = 0; i < n2; i++) {
				AVE_S32_S16(load, loadu, storeu);
			}
		}

		for (i = 0; i < n3; i++) {
			sum = (*px++) + (*py++);
			*pz++ = (sum + 1) >> 1;
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#define	AVE_S32_S32(ld1, ld2, st)			\
	xbuf = _mm_##ld1##_si128((__m128i *)px);	\
	ybuf = _mm_##ld2##_si128((__m128i *)py);	\
	buf1 = _mm_srai_epi32(xbuf, 1);			\
	buf2 = _mm_srai_epi32(ybuf, 1);			\
	zbuf = _mm_or_si128(xbuf, ybuf);		\
	buf1 = _mm_add_epi32(buf1, buf2);		\
	zbuf = _mm_and_si128(zbuf, mask);		\
	zbuf = _mm_add_epi32(zbuf, buf1);		\
	_mm_##st##_si128((__m128i *)pz, zbuf);		\
	px += nstep;					\
	py += nstep;					\
	pz += nstep

mlib_status
__mlib_VectorAve_S32_S32(
	mlib_s32 *z,
	const mlib_s32 *x,
	const mlib_s32 *y,
	mlib_s32 n)
{
	if (n < 1)
		return (MLIB_FAILURE);

	mlib_s32 i, ax, ay, az, nstep, n1, n2, n3;
	mlib_s32 ave, xval, yval;
	const mlib_s32 *px = x, *py = y;
	mlib_s32 *pz = z;
	__m128i xbuf, ybuf, zbuf, buf1, buf2, mask;
	mask = _mm_set1_epi32(1);

	ax = (mlib_addr)x & 15;
	ay = (mlib_addr)y & 15;
	az = (mlib_addr)z & 15;

	nstep = 16 / sizeof (mlib_s32);
	n1 = ((16 - ax) & 15) / sizeof (mlib_s32);
	n2 = (n - n1) / nstep;
	n3 = n - n1 - n2 * nstep;

	if (n2 < 1) {
		for (i = 0; i < n; i++) {
			xval = *px++;
			yval = *py++;
			ave = (xval >> 1) + (yval >> 1) + ((xval | yval) & 1);
			*pz++ = ave;
		}
	} else {
		for (i = 0; i < n1; i++) {
			xval = *px++;
			yval = *py++;
			ave = (xval >> 1) + (yval >> 1) + ((xval | yval) & 1);
			*pz++ = ave;
		}

		if (ax == ay && ax == az) {
			for (i = 0; i < n2; i++) {
				AVE_S32_S32(load, load, store);
			}
		} else if (ax == ay) {
			for (i = 0; i < n2; i++) {
				AVE_S32_S32(load, load, storeu);
			}
		} else {
			for (i = 0; i < n2; i++) {
				AVE_S32_S32(load, loadu, storeu);
			}
		}

		for (i = 0; i < n3; i++) {
			xval = *px++;
			yval = *py++;
			ave = (xval >> 1) + (yval >> 1) + ((xval | yval) & 1);
			*pz++ = ave;
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VectorAve_U8(
	mlib_u8 *xz,
	const mlib_u8 *y,
	mlib_s32 n)
{
	return (__mlib_VectorAve_U8_U8(xz, xz, y, n));
}

/* *********************************************************** */

mlib_status
__mlib_VectorAve_S8(
	mlib_s8 *xz,
	const mlib_s8 *y,
	mlib_s32 n)
{
	return (__mlib_VectorAve_S8_S8(xz, xz, y, n));
}

/* *********************************************************** */

mlib_status
__mlib_VectorAve_S16(
	mlib_s16 *xz,
	const mlib_s16 *y,
	mlib_s32 n)
{
	return (__mlib_VectorAve_S16_S16(xz, xz, y, n));
}

/* *********************************************************** */

mlib_status
__mlib_VectorAve_S32(
	mlib_s32 *xz,
	const mlib_s32 *y,
	mlib_s32 n)
{
	return (__mlib_VectorAve_S32_S32(xz, xz, y, n));
}

/* *********************************************************** */

mlib_status
__mlib_VectorAve_U8C_U8C(
	mlib_u8 *z,
	const mlib_u8 *x,
	const mlib_u8 *y,
	mlib_s32 n)
{
	return (__mlib_VectorAve_U8_U8(z, x, y, n + n));
}

/* *********************************************************** */

mlib_status
__mlib_VectorAve_S8C_S8C(
	mlib_s8	*z,
	const mlib_s8 *x,
	const mlib_s8 *y,
	mlib_s32 n)
{
	return (__mlib_VectorAve_S8_S8(z, x, y, n + n));
}

/* *********************************************************** */

mlib_status
__mlib_VectorAve_S16C_U8C(
	mlib_s16 *z,
	const mlib_u8 *x,
	const mlib_u8 *y,
	mlib_s32 n)
{
	return (__mlib_VectorAve_S16_U8(z, x, y, n + n));
}

/* *********************************************************** */

mlib_status
__mlib_VectorAve_S16C_S8C(
	mlib_s16 *z,
	const mlib_s8 *x,
	const mlib_s8 *y,
	mlib_s32 n)
{
	return (__mlib_VectorAve_S16_S8(z, x, y, n + n));
}

/* *********************************************************** */

mlib_status
__mlib_VectorAve_S16C_S16C(
	mlib_s16 *z,
	const mlib_s16 *x,
	const mlib_s16 *y,
	mlib_s32 n)
{
	return (__mlib_VectorAve_S16_S16(z, x, y, n + n));
}

/* *********************************************************** */

mlib_status
__mlib_VectorAve_S32C_S16C(
	mlib_s32 *z,
	const mlib_s16 *x,
	const mlib_s16 *y,
	mlib_s32 n)
{
	return (__mlib_VectorAve_S32_S16(z, x, y, n + n));
}

/* *********************************************************** */

mlib_status
__mlib_VectorAve_S32C_S32C(
	mlib_s32 *z,
	const mlib_s32 *x,
	const mlib_s32 *y,
	mlib_s32 n)
{
	return (__mlib_VectorAve_S32_S32(z, x, y, n + n));
}

/* *********************************************************** */

mlib_status
__mlib_VectorAve_U8C(
	mlib_u8 *xz,
	const mlib_u8 *y,
	mlib_s32 n)
{
	return (__mlib_VectorAve_U8(xz, y, n + n));
}

/* *********************************************************** */

mlib_status
__mlib_VectorAve_S8C(
	mlib_s8 *xz,
	const mlib_s8 *y,
	mlib_s32 n)
{
	return (__mlib_VectorAve_S8(xz, y, n + n));
}

/* *********************************************************** */

mlib_status
__mlib_VectorAve_S16C(
	mlib_s16 *xz,
	const mlib_s16 *y,
	mlib_s32 n)
{
	return (__mlib_VectorAve_S16(xz, y, n + n));
}

/* *********************************************************** */

mlib_status
__mlib_VectorAve_S32C(
	mlib_s32 *xz,
	const mlib_s32 *y,
	mlib_s32 n)
{
	return (__mlib_VectorAve_S32(xz, y, n + n));
}

/* *********************************************************** */
