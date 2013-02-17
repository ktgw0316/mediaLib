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

#pragma ident	"@(#)mlib_s_VectorConvert1.c	9.2	07/11/05 SMI"

/*
 * FUNCTIONS
 *      mlib_VectorConvert_S16_U8_Mod  - copy with expand U8->S16
 *      mlib_VectorConvert_S32_U8_Mod  - copy with expand U8->S32
 *      mlib_VectorConvert_S16_S8_Mod  - copy with expand S8->S16
 *      mlib_VectorConvert_S32_S8_Mod  - copy with expand S8->S32
 *      mlib_VectorConvert_S32_S16_Mod - copy with expand S16->S32
 *
 *      mlib_VectorConvert_S16C_U8C_Mod  - copy with expand U8->S16
 *      mlib_VectorConvert_S32C_U8C_Mod  - copy with expand U8->S32
 *      mlib_VectorConvert_S16C_S8C_Mod  - copy with expand S8->S16
 *      mlib_VectorConvert_S32C_S8C_Mod  - copy with expand S8->S32
 *      mlib_VectorConvert_S32C_S16C_Mod - copy with expand S16->S32
 *
 * SYNOPSIS
 *      mlib_status mlib_VectorConvert_S16_U8_Mod(mlib_s16      *z,
 *                                                const mlib_u8 *x,
 *                                                mlib_s32      n);
 *      mlib_status mlib_VectorConvert_S32_U8_Mod(mlib_s32      *z,
 *                                                const mlib_u8 *x,
 *                                                mlib_s32      n);
 *      mlib_status mlib_VectorConvert_S16_S8_Mod(mlib_s16      *z,
 *                                                const mlib_s8 *x,
 *                                                mlib_s32      n);
 *      mlib_status mlib_VectorConvert_S32_S8_Mod(mlib_s32      *z,
 *                                                const mlib_s8 *x,
 *                                                mlib_s32      n);
 *      mlib_status mlib_VectorConvert_S32_S16_Mod(mlib_s32       *z,
 *                                                 const mlib_s16 *x,
 *                                                 mlib_s32       n);
 *
 *      mlib_status mlib_VectorConvert_S16C_U8C_Mod(mlib_s16      *z,
 *                                                  const mlib_u8 *x,
 *                                                  mlib_s32      n);
 *      mlib_status mlib_VectorConvert_S32C_U8C_Mod(mlib_s32      *z,
 *                                                  const mlib_u8 *x,
 *                                                  mlib_s32      n);
 *      mlib_status mlib_VectorConvert_S16C_S8C_Mod(mlib_s16      *z,
 *                                                  const mlib_s8 *x,
 *                                                  mlib_s32      n);
 *      mlib_status mlib_VectorConvert_S32C_S8C_Mod(mlib_s32      *z,
 *                                                  const mlib_s8 *x,
 *                                                  mlib_s32      n);
 *      mlib_status mlib_VectorConvert_S32C_S16C_Mod(mlib_s32       *z,
 *                                                   const mlib_s16 *x,
 *                                                   mlib_s32       n);
 *
 * ARGUMENTS
 *      z    pointer to the first element of the result vector
 *      x    pointer to the first element of the input vector
 *      n    number of elements in the vectors
 *
 *           for complex - xx[2*i] hold the real parts, and
 *                         xx[2*i + 1] hold the imaginary parts
 *
 * DESCRIPTION
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

#pragma weak mlib_VectorConvert_S16C_U8C_Mod = \
	__mlib_VectorConvert_S16C_U8C_Mod
#pragma weak mlib_VectorConvert_S32_S16_Mod = \
	__mlib_VectorConvert_S32_S16_Mod
#pragma weak mlib_VectorConvert_S32_S8_Mod = __mlib_VectorConvert_S32_S8_Mod
#pragma weak mlib_VectorConvert_S32_U8_Mod = __mlib_VectorConvert_S32_U8_Mod
#pragma weak mlib_VectorConvert_S16_S8_Mod = __mlib_VectorConvert_S16_S8_Mod
#pragma weak mlib_VectorConvert_S16_U8_Mod = __mlib_VectorConvert_S16_U8_Mod
#pragma weak mlib_VectorConvert_S32C_S16C_Mod = \
	__mlib_VectorConvert_S32C_S16C_Mod
#pragma weak mlib_VectorConvert_S32C_S8C_Mod = \
	__mlib_VectorConvert_S32C_S8C_Mod
#pragma weak mlib_VectorConvert_S32C_U8C_Mod = \
	__mlib_VectorConvert_S32C_U8C_Mod
#pragma weak mlib_VectorConvert_S16C_S8C_Mod = \
	__mlib_VectorConvert_S16C_S8C_Mod

#elif defined(__GNUC__)

__typeof__(__mlib_VectorConvert_S16C_U8C_Mod) mlib_VectorConvert_S16C_U8C_Mod
	__attribute__((weak, alias("__mlib_VectorConvert_S16C_U8C_Mod")));
__typeof__(__mlib_VectorConvert_S32_S16_Mod) mlib_VectorConvert_S32_S16_Mod
	__attribute__((weak, alias("__mlib_VectorConvert_S32_S16_Mod")));
__typeof__(__mlib_VectorConvert_S32_S8_Mod) mlib_VectorConvert_S32_S8_Mod
	__attribute__((weak, alias("__mlib_VectorConvert_S32_S8_Mod")));
__typeof__(__mlib_VectorConvert_S32_U8_Mod) mlib_VectorConvert_S32_U8_Mod
	__attribute__((weak, alias("__mlib_VectorConvert_S32_U8_Mod")));
__typeof__(__mlib_VectorConvert_S16_S8_Mod) mlib_VectorConvert_S16_S8_Mod
	__attribute__((weak, alias("__mlib_VectorConvert_S16_S8_Mod")));
__typeof__(__mlib_VectorConvert_S16_U8_Mod) mlib_VectorConvert_S16_U8_Mod
	__attribute__((weak, alias("__mlib_VectorConvert_S16_U8_Mod")));
__typeof__(__mlib_VectorConvert_S32C_S16C_Mod) mlib_VectorConvert_S32C_S16C_Mod
	__attribute__((weak, alias("__mlib_VectorConvert_S32C_S16C_Mod")));
__typeof__(__mlib_VectorConvert_S32C_S8C_Mod) mlib_VectorConvert_S32C_S8C_Mod
	__attribute__((weak, alias("__mlib_VectorConvert_S32C_S8C_Mod")));
__typeof__(__mlib_VectorConvert_S32C_U8C_Mod) mlib_VectorConvert_S32C_U8C_Mod
	__attribute__((weak, alias("__mlib_VectorConvert_S32C_U8C_Mod")));
__typeof__(__mlib_VectorConvert_S16C_S8C_Mod) mlib_VectorConvert_S16C_S8C_Mod
	__attribute__((weak, alias("__mlib_VectorConvert_S16C_S8C_Mod")));

#else

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

mlib_status
__mlib_VectorConvert_S16_U8_Mod(
	mlib_s16 *z,
	const mlib_u8 *x,
	mlib_s32 n)
{
	if (n < 1)
		return (MLIB_FAILURE);

	mlib_s32 i, ax, az, nstep, n1, n2, n3;
	mlib_u8 *px = (mlib_u8 *)x;
	mlib_s16 *pz = (mlib_s16 *)z;
	__m128i xbuf, zlo, zhi, zero;
	zero = _mm_setzero_si128();

	ax = (mlib_addr)x & 15;
	az = (mlib_addr)z & 15;

	nstep = 16 / sizeof (mlib_u8);
	n1 = ((16 - ax) & 15) / sizeof (mlib_u8);
	n2 = (n - n1) / nstep;
	n3 = n - n1 - n2 * nstep;

	if (n2 < 1) {
		for (i = 0; i < n; i++) {
			*pz++ = *px++;
		}
	} else {
		for (i = 0; i < n1; i++) {
			*pz++ = *px++;
		}

		if ((ax * 2 & 15) == az) {
			for (i = 0; i < n2; i++) {
				xbuf = _mm_load_si128((__m128i *)px);
				zlo = _mm_unpacklo_epi8(xbuf, zero);
				zhi = _mm_unpackhi_epi8(xbuf, zero);
				_mm_store_si128((__m128i *)pz, zlo);
				_mm_store_si128((__m128i *)pz + 1, zhi);
				px += nstep;
				pz += nstep;
			}

		} else {
			for (i = 0; i < n2; i++) {
				xbuf = _mm_load_si128((__m128i *)px);
				zlo = _mm_unpacklo_epi8(xbuf, zero);
				zhi = _mm_unpackhi_epi8(xbuf, zero);
				_mm_storeu_si128((__m128i *)pz, zlo);
				_mm_storeu_si128((__m128i *)pz + 1, zhi);
				px += nstep;
				pz += nstep;
			}
		}

		for (i = 0; i < n3; i++) {
			*pz++ = *px++;
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VectorConvert_S32_U8_Mod(
	mlib_s32 *z,
	const mlib_u8 *x,
	mlib_s32 n)
{
	if (n < 1)
		return (MLIB_FAILURE);

	mlib_s32 i, ax, az, nstep, n1, n2, n3;
	mlib_u8 *px = (mlib_u8 *)x;
	mlib_s32 *pz = (mlib_s32 *)z;
	__m128i xbuf, zlo, zhi, buf1, buf2, buf3, buf4, zero;
	zero = _mm_setzero_si128();

	ax = (mlib_addr)x & 15;
	az = (mlib_addr)z & 15;

	nstep = 16 / sizeof (mlib_u8);
	n1 = ((16 - ax) & 15) / sizeof (mlib_u8);
	n2 = (n - n1) / nstep;
	n3 = n - n1 - n2 * nstep;

	if (n2 < 1) {
		for (i = 0; i < n; i++) {
			*pz++ = *px++;
		}
	} else {
		for (i = 0; i < n1; i++) {
			*pz++ = *px++;
		}

		if ((ax * 4 & 15) == az) {
			for (i = 0; i < n2; i++) {
				xbuf = _mm_load_si128((__m128i *)px);
				zlo = _mm_unpacklo_epi8(xbuf, zero);
				zhi = _mm_unpackhi_epi8(xbuf, zero);
				buf1 = _mm_unpacklo_epi16(zlo, zero);
				buf2 = _mm_unpackhi_epi16(zlo, zero);
				buf3 = _mm_unpacklo_epi16(zhi, zero);
				buf4 = _mm_unpackhi_epi16(zhi, zero);
				_mm_store_si128((__m128i *)pz, buf1);
				_mm_store_si128((__m128i *)pz + 1, buf2);
				_mm_store_si128((__m128i *)pz + 2, buf3);
				_mm_store_si128((__m128i *)pz + 3, buf4);
				px += nstep;
				pz += nstep;
			}
		} else {
			for (i = 0; i < n2; i++) {
				xbuf = _mm_load_si128((__m128i *)px);
				zlo = _mm_unpacklo_epi8(xbuf, zero);
				zhi = _mm_unpackhi_epi8(xbuf, zero);
				buf1 = _mm_unpacklo_epi16(zlo, zero);
				buf2 = _mm_unpackhi_epi16(zlo, zero);
				buf3 = _mm_unpacklo_epi16(zhi, zero);
				buf4 = _mm_unpackhi_epi16(zhi, zero);
				_mm_storeu_si128((__m128i *)pz, buf1);
				_mm_storeu_si128((__m128i *)pz + 1, buf2);
				_mm_storeu_si128((__m128i *)pz + 2, buf3);
				_mm_storeu_si128((__m128i *)pz + 3, buf4);
				px += nstep;
				pz += nstep;
			}
		}

		for (i = 0; i < n3; i++) {
			*pz++ = *px++;
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VectorConvert_S16_S8_Mod(
	mlib_s16 *z,
	const mlib_s8 *x,
	mlib_s32 n)
{
	if (n < 1)
		return (MLIB_FAILURE);

	mlib_s32 i, ax, az, nstep, n1, n2, n3;
	mlib_s8 *px = (mlib_s8 *)x;
	mlib_s16 *pz = (mlib_s16 *)z;
	__m128i xbuf, zlo, zhi, zero;
	zero = _mm_setzero_si128();

	ax = (mlib_addr)x & 15;
	az = (mlib_addr)z & 15;

	nstep = 16 / sizeof (mlib_s8);
	n1 = ((16 - ax) & 15) / sizeof (mlib_s8);
	n2 = (n - n1) / nstep;
	n3 = n - n1 - n2 * nstep;

	if (n2 < 1) {
		for (i = 0; i < n; i++) {
			*pz++ = *px++;
		}
	} else {
		for (i = 0; i < n1; i++) {
			*pz++ = *px++;
		}

		if ((ax * 2 & 15) == az) {
			for (i = 0; i < n2; i++) {
				xbuf = _mm_load_si128((__m128i *)px);
				zlo = _mm_unpacklo_epi8(zero, xbuf);
				zhi = _mm_unpackhi_epi8(zero, xbuf);
				zlo = _mm_srai_epi16(zlo, 8);
				zhi = _mm_srai_epi16(zhi, 8);
				_mm_store_si128((__m128i *)pz, zlo);
				_mm_store_si128((__m128i *)pz + 1, zhi);
				px += nstep;
				pz += nstep;
			}

		} else {
			for (i = 0; i < n2; i++) {
				xbuf = _mm_load_si128((__m128i *)px);
				zlo = _mm_unpacklo_epi8(zero, xbuf);
				zhi = _mm_unpackhi_epi8(zero, xbuf);
				zlo = _mm_srai_epi16(zlo, 8);
				zhi = _mm_srai_epi16(zhi, 8);
				_mm_storeu_si128((__m128i *)pz, zlo);
				_mm_storeu_si128((__m128i *)pz + 1, zhi);
				px += nstep;
				pz += nstep;
			}
		}

		for (i = 0; i < n3; i++) {
			*pz++ = *px++;
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VectorConvert_S32_S8_Mod(
	mlib_s32 *z,
	const mlib_s8 *x,
	mlib_s32 n)
{
	if (n < 1)
		return (MLIB_FAILURE);

	mlib_s32 i, ax, az, nstep, n1, n2, n3;
	mlib_s8 *px = (mlib_s8 *)x;
	mlib_s32 *pz = (mlib_s32 *)z;
	__m128i xbuf, zlo, zhi, buf1, buf2, buf3, buf4, zero;
	zero = _mm_setzero_si128();

	ax = (mlib_addr)x & 15;
	az = (mlib_addr)z & 15;

	nstep = 16 / sizeof (mlib_s8);
	n1 = ((16 - ax) & 15) / sizeof (mlib_s8);
	n2 = (n - n1) / nstep;
	n3 = n - n1 - n2 * nstep;

	if (n2 < 1) {
		for (i = 0; i < n; i++) {
			*pz++ = *px++;
		}
	} else {
		for (i = 0; i < n1; i++) {
			*pz++ = *px++;
		}

		if ((ax * 4 & 15) == az) {
			for (i = 0; i < n2; i++) {
				xbuf = _mm_load_si128((__m128i *)px);
				zlo = _mm_unpacklo_epi8(zero, xbuf);
				zhi = _mm_unpackhi_epi8(zero, xbuf);
				buf1 = _mm_unpacklo_epi16(zero, zlo);
				buf2 = _mm_unpackhi_epi16(zero, zlo);
				buf3 = _mm_unpacklo_epi16(zero, zhi);
				buf4 = _mm_unpackhi_epi16(zero, zhi);
				buf1 = _mm_srai_epi32(buf1, 24);
				buf2 = _mm_srai_epi32(buf2, 24);
				buf3 = _mm_srai_epi32(buf3, 24);
				buf4 = _mm_srai_epi32(buf4, 24);
				_mm_store_si128((__m128i *)pz, buf1);
				_mm_store_si128((__m128i *)pz + 1, buf2);
				_mm_store_si128((__m128i *)pz + 2, buf3);
				_mm_store_si128((__m128i *)pz + 3, buf4);
				px += nstep;
				pz += nstep;
			}
		} else {
			for (i = 0; i < n2; i++) {
				xbuf = _mm_load_si128((__m128i *)px);
				zlo = _mm_unpacklo_epi8(zero, xbuf);
				zhi = _mm_unpackhi_epi8(zero, xbuf);
				buf1 = _mm_unpacklo_epi16(zero, zlo);
				buf2 = _mm_unpackhi_epi16(zero, zlo);
				buf3 = _mm_unpacklo_epi16(zero, zhi);
				buf4 = _mm_unpackhi_epi16(zero, zhi);
				buf1 = _mm_srai_epi32(buf1, 24);
				buf2 = _mm_srai_epi32(buf2, 24);
				buf3 = _mm_srai_epi32(buf3, 24);
				buf4 = _mm_srai_epi32(buf4, 24);
				_mm_storeu_si128((__m128i *)pz, buf1);
				_mm_storeu_si128((__m128i *)pz + 1, buf2);
				_mm_storeu_si128((__m128i *)pz + 2, buf3);
				_mm_storeu_si128((__m128i *)pz + 3, buf4);
				px += nstep;
				pz += nstep;
			}
		}

		for (i = 0; i < n3; i++) {
			*pz++ = *px++;
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VectorConvert_S32_S16_Mod(
	mlib_s32 *z,
	const mlib_s16 *x,
	mlib_s32 n)
{
	if (n < 1)
		return (MLIB_FAILURE);

	mlib_s32 i, ax, az, nstep, n1, n2, n3;
	mlib_s16 *px = (mlib_s16 *)x;
	mlib_s32 *pz = (mlib_s32 *)z;
	__m128i xbuf, zlo, zhi, zero;
	zero = _mm_setzero_si128();

	ax = (mlib_addr)x & 15;
	az = (mlib_addr)z & 15;

	nstep = 16 / sizeof (mlib_s16);
	n1 = ((16 - ax) & 15) / sizeof (mlib_s16);
	n2 = (n - n1) / nstep;
	n3 = n - n1 - n2 * nstep;

	if (n2 < 1) {
		for (i = 0; i < n; i++) {
			*pz++ = *px++;
		}
	} else {
		for (i = 0; i < n1; i++) {
			*pz++ = *px++;
		}

		if ((ax * 2 & 15) == az) {
			for (i = 0; i < n2; i++) {
				xbuf = _mm_load_si128((__m128i *)px);
				zlo = _mm_unpacklo_epi16(zero, xbuf);
				zhi = _mm_unpackhi_epi16(zero, xbuf);
				zlo = _mm_srai_epi32(zlo, 16);
				zhi = _mm_srai_epi32(zhi, 16);
				_mm_store_si128((__m128i *)pz, zlo);
				_mm_store_si128((__m128i *)pz + 1, zhi);
				px += nstep;
				pz += nstep;
			}
		} else {
			for (i = 0; i < n2; i++) {
				xbuf = _mm_load_si128((__m128i *)px);
				zlo = _mm_unpacklo_epi16(zero, xbuf);
				zhi = _mm_unpackhi_epi16(zero, xbuf);
				zlo = _mm_srai_epi32(zlo, 16);
				zhi = _mm_srai_epi32(zhi, 16);
				_mm_storeu_si128((__m128i *)pz, zlo);
				_mm_storeu_si128((__m128i *)pz + 1, zhi);
				px += nstep;
				pz += nstep;
			}
		}

		for (i = 0; i < n3; i++) {
			*pz++ = *px++;
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VectorConvert_S16C_U8C_Mod(
	mlib_s16 *z,
	const mlib_u8 *x,
	mlib_s32 n)
{
	return (__mlib_VectorConvert_S16_U8_Mod(z, x, n + n));
}

/* *********************************************************** */

mlib_status
__mlib_VectorConvert_S32C_U8C_Mod(
	mlib_s32 *z,
	const mlib_u8 *x,
	mlib_s32 n)
{
	return (__mlib_VectorConvert_S32_U8_Mod(z, x, n + n));
}

/* *********************************************************** */

mlib_status
__mlib_VectorConvert_S16C_S8C_Mod(
	mlib_s16 *z,
	const mlib_s8 *x,
	mlib_s32 n)
{
	return (__mlib_VectorConvert_S16_S8_Mod(z, x, n + n));
}

/* *********************************************************** */

mlib_status
__mlib_VectorConvert_S32C_S8C_Mod(
	mlib_s32 *z,
	const mlib_s8 *x,
	mlib_s32 n)
{
	return (__mlib_VectorConvert_S32_S8_Mod(z, x, n + n));
}

/* *********************************************************** */

mlib_status
__mlib_VectorConvert_S32C_S16C_Mod(
	mlib_s32 *z,
	const mlib_s16 *x,
	mlib_s32 n)
{
	return (__mlib_VectorConvert_S32_S16_Mod(z, x, n + n));
}

/* *********************************************************** */
