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

#pragma ident	"@(#)mlib_s_VectorMulM.c	9.4	07/11/05 SMI"

/*
 * FUNCTIONS
 *      mlib_VectorMulM_U8_U8[Sat, Mod]   - multiplication of unsigned 8-bit
 *                                          format vector by matrix
 *      mlib_VectorMulM_S8_S8[Sat, Mod]   - multiplication of signed 8-bit
 *                                          format vector by matrix
 *      mlib_VectorMulM_S16_S16[Sat, Mod] - multiplication of signed 16-bit
 *                                          format vector by matrix
 *      mlib_VectorMulM_S32_S32[Sat, Mod] - multiplication of signed 32-bit
 *                                          format vector by matrix
 *
 * SYNOPSIS
 *      mlib_status mlib_VectorMulM_U8_U8[Sat, Mod](mlib_u8       *z,
 *                                                 const mlib_u8 *x,
 *                                                 const mlib_u8 *y,
 *                                                 mlib_s32      m,
 *                                                 mlib_s32      n);
 *      mlib_status mlib_VectorMulM_S8_S8[Sat, Mod](mlib_s8       *z,
 *                                                 const mlib_s8 *x,
 *                                                 const mlib_s8 *y,
 *                                                 mlib_s32      m,
 *                                                 mlib_s32      n);
 *      mlib_status mlib_VectorMulM_S16_S16[Sat, Mod](mlib_s16       *z,
 *                                                   const mlib_s16 *x,
 *                                                   const mlib_s16 *y,
 *                                                   mlib_s32       m,
 *                                                   mlib_s32       n);
 *      mlib_status mlib_VectorMulM_S32_S32[Sat, Mod](mlib_s32       *z,
 *                                                   const mlib_s32 *x,
 *                                                   const mlib_s32 *y,
 *                                                   mlib_s32       m,
 *                                                   mlib_s32       n);
 *
 * ARGUMENTS
 *      z    pointer to the first element of the result vector
 *      x    pointer to the first element of the input vector
 *      y    pointer to the first element of the input matrix
 *      m    number of rows in the matrix, and number of elements
 *           in the input vector
 *      n    number of columns in the matrix, and number of elements
 *           in the result vector
 *
 * DESCRIPTION
 *              m
 *      z[i] = SUM (x[k] * y[k][i])
 *             k = 1
 */

/*
 * FUNCTIONS
 *      mlib_VectorMulM_U8C_U8C_[Sat|Mod]    - multiplication of unsigned 8-bit
 *                                             format
 *                                             complex vector by complex matrix
 *      mlib_VectorMulM_S8C_S8C_[Sat|Mod]    - multiplication of signed 8-bit
 *                                             format
 *                                             complex vector by complex matrix
 *      mlib_VectorMulM_S16C_S16C_[Sat|Mod]  - multiplication of signed 16-bit
 *                                             format
 *                                             complex vector by complex matrix
 *       mlib_VectorMulM_S32C_S32C_[Sat|Mod] - multiplication of signed 32-bit
 *                                             format
 *                                             complex vector by complex matrix
 *
 * SYNOPSIS
 *      mlib_status mlib_VectorMulM_U8C_U8C[Sat, Mod](mlib_u8       *z,
 *                                                   const mlib_u8 *x,
 *                                                   const mlib_u8 *y,
 *                                                   mlib_s32      m,
 *                                                   mlib_s32      n);
 *      mlib_status mlib_VectorMulM_S8C_S8C[Sat, Mod](mlib_s8       *z,
 *                                                   const mlib_s8 *x,
 *                                                   const mlib_s8 *y,
 *                                                   mlib_s32      m,
 *                                                   mlib_s32      n);
 *      mlib_status mlib_VectorMulM_S16C_S16C[Sat, Mod](mlib_s16       *z,
 *                                                     const mlib_s16 *x,
 *                                                     const mlib_s16 *y,
 *                                                     mlib_s32       m,
 *                                                     mlib_s32       n);
 *      mlib_status mlib_VectorMulM_S32C_S32C[Sat, Mod](mlib_s32       *z,
 *                                                     const mlib_s32 *x,
 *                                                     const mlib_s32 *y,
 *                                                     mlib_s32       m,
 *                                                     mlib_s32       n);
 *
 * ARGUMENTS
 *      z    pointer to the first element of the result vector
 *           z[2*i] hold the real parts, and z[2*i + 1] hold the imaginary
 *           parts
 *      x    pointer to the first element of the input vector
 *           x[2*i] hold the real parts, and x[2*i + 1] hold the imaginary
 *           parts
 *      y    pointer to the first element of the input matrix
 *           y[2*i] hold the real parts, and y[2*i + 1] hold the imaginary
 *           parts
 *      m    number of rows in the matrix, and number of elements
 *           in the input vector
 *      n    number of columns in the matrix, and number of elements
 *           in the result vector
 *
 * DESCRIPTION
 *              m
 *      z[i] = SUM (x[k] * y[k][i])
 *             k = 1
 */

#include <mlib_algebra.h>
#include <mlib_VectorMulM.h>

#ifdef __SUNPRO_C
#include <sunmedia_intrin.h>
#else
#include <emmintrin.h>
#endif

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_VectorMulM_S32_S16_Mod = __mlib_VectorMulM_S32_S16_Mod
#pragma weak mlib_VectorMulM_S16C_S8C_Sat = __mlib_VectorMulM_S16C_S8C_Sat
#pragma weak mlib_VectorMulM_S16_S16_Mod = __mlib_VectorMulM_S16_S16_Mod
#pragma weak mlib_VectorMulM_S16C_U8C_Sat = __mlib_VectorMulM_S16C_U8C_Sat
#pragma weak mlib_VectorMulM_S8_S8_Mod = __mlib_VectorMulM_S8_S8_Mod
#pragma weak mlib_VectorMulM_S32C_S16C_Mod = __mlib_VectorMulM_S32C_S16C_Mod
#pragma weak mlib_VectorMulM_S32_S16_Sat = __mlib_VectorMulM_S32_S16_Sat
#pragma weak mlib_VectorMulM_S16C_S16C_Mod = __mlib_VectorMulM_S16C_S16C_Mod
#pragma weak mlib_VectorMulM_U8_U8_Mod = __mlib_VectorMulM_U8_U8_Mod
#pragma weak mlib_VectorMulM_S16_S16_Sat = __mlib_VectorMulM_S16_S16_Sat
#pragma weak mlib_VectorMulM_S8C_S8C_Mod = __mlib_VectorMulM_S8C_S8C_Mod
#pragma weak mlib_VectorMulM_S8_S8_Sat = __mlib_VectorMulM_S8_S8_Sat
#pragma weak mlib_VectorMulM_S32C_S16C_Sat = __mlib_VectorMulM_S32C_S16C_Sat
#pragma weak mlib_VectorMulM_U8C_U8C_Mod = __mlib_VectorMulM_U8C_U8C_Mod
#pragma weak mlib_VectorMulM_S16C_S16C_Sat = __mlib_VectorMulM_S16C_S16C_Sat
#pragma weak mlib_VectorMulM_U8_U8_Sat = __mlib_VectorMulM_U8_U8_Sat
#pragma weak mlib_VectorMulM_S8C_S8C_Sat = __mlib_VectorMulM_S8C_S8C_Sat
#pragma weak mlib_VectorMulM_S16_S8_Mod = __mlib_VectorMulM_S16_S8_Mod
#pragma weak mlib_VectorMulM_S16_U8_Mod = __mlib_VectorMulM_S16_U8_Mod
#pragma weak mlib_VectorMulM_U8C_U8C_Sat = __mlib_VectorMulM_U8C_U8C_Sat
#pragma weak mlib_VectorMulM_S16C_S8C_Mod = __mlib_VectorMulM_S16C_S8C_Mod
#pragma weak mlib_VectorMulM_S16C_U8C_Mod = __mlib_VectorMulM_S16C_U8C_Mod
#pragma weak mlib_VectorMulM_S16_S8_Sat = __mlib_VectorMulM_S16_S8_Sat
#pragma weak mlib_VectorMulM_S16_U8_Sat = __mlib_VectorMulM_S16_U8_Sat

#elif defined(__GNUC__)

__typeof__(__mlib_VectorMulM_S32_S16_Mod) mlib_VectorMulM_S32_S16_Mod
	__attribute__((weak, alias("__mlib_VectorMulM_S32_S16_Mod")));
__typeof__(__mlib_VectorMulM_S16C_S8C_Sat) mlib_VectorMulM_S16C_S8C_Sat
	__attribute__((weak, alias("__mlib_VectorMulM_S16C_S8C_Sat")));
__typeof__(__mlib_VectorMulM_S16_S16_Mod) mlib_VectorMulM_S16_S16_Mod
	__attribute__((weak, alias("__mlib_VectorMulM_S16_S16_Mod")));
__typeof__(__mlib_VectorMulM_S16C_U8C_Sat) mlib_VectorMulM_S16C_U8C_Sat
	__attribute__((weak, alias("__mlib_VectorMulM_S16C_U8C_Sat")));
__typeof__(__mlib_VectorMulM_S8_S8_Mod) mlib_VectorMulM_S8_S8_Mod
	__attribute__((weak, alias("__mlib_VectorMulM_S8_S8_Mod")));
__typeof__(__mlib_VectorMulM_S32C_S16C_Mod) mlib_VectorMulM_S32C_S16C_Mod
	__attribute__((weak, alias("__mlib_VectorMulM_S32C_S16C_Mod")));
__typeof__(__mlib_VectorMulM_S32_S16_Sat) mlib_VectorMulM_S32_S16_Sat
	__attribute__((weak, alias("__mlib_VectorMulM_S32_S16_Sat")));
__typeof__(__mlib_VectorMulM_S16C_S16C_Mod) mlib_VectorMulM_S16C_S16C_Mod
	__attribute__((weak, alias("__mlib_VectorMulM_S16C_S16C_Mod")));
__typeof__(__mlib_VectorMulM_U8_U8_Mod) mlib_VectorMulM_U8_U8_Mod
	__attribute__((weak, alias("__mlib_VectorMulM_U8_U8_Mod")));
__typeof__(__mlib_VectorMulM_S16_S16_Sat) mlib_VectorMulM_S16_S16_Sat
	__attribute__((weak, alias("__mlib_VectorMulM_S16_S16_Sat")));
__typeof__(__mlib_VectorMulM_S8C_S8C_Mod) mlib_VectorMulM_S8C_S8C_Mod
	__attribute__((weak, alias("__mlib_VectorMulM_S8C_S8C_Mod")));
__typeof__(__mlib_VectorMulM_S8_S8_Sat) mlib_VectorMulM_S8_S8_Sat
	__attribute__((weak, alias("__mlib_VectorMulM_S8_S8_Sat")));
__typeof__(__mlib_VectorMulM_S32C_S16C_Sat) mlib_VectorMulM_S32C_S16C_Sat
	__attribute__((weak, alias("__mlib_VectorMulM_S32C_S16C_Sat")));
__typeof__(__mlib_VectorMulM_U8C_U8C_Mod) mlib_VectorMulM_U8C_U8C_Mod
	__attribute__((weak, alias("__mlib_VectorMulM_U8C_U8C_Mod")));
__typeof__(__mlib_VectorMulM_S16C_S16C_Sat) mlib_VectorMulM_S16C_S16C_Sat
	__attribute__((weak, alias("__mlib_VectorMulM_S16C_S16C_Sat")));
__typeof__(__mlib_VectorMulM_U8_U8_Sat) mlib_VectorMulM_U8_U8_Sat
	__attribute__((weak, alias("__mlib_VectorMulM_U8_U8_Sat")));
__typeof__(__mlib_VectorMulM_S8C_S8C_Sat) mlib_VectorMulM_S8C_S8C_Sat
	__attribute__((weak, alias("__mlib_VectorMulM_S8C_S8C_Sat")));
__typeof__(__mlib_VectorMulM_S16_S8_Mod) mlib_VectorMulM_S16_S8_Mod
	__attribute__((weak, alias("__mlib_VectorMulM_S16_S8_Mod")));
__typeof__(__mlib_VectorMulM_S16_U8_Mod) mlib_VectorMulM_S16_U8_Mod
	__attribute__((weak, alias("__mlib_VectorMulM_S16_U8_Mod")));
__typeof__(__mlib_VectorMulM_U8C_U8C_Sat) mlib_VectorMulM_U8C_U8C_Sat
	__attribute__((weak, alias("__mlib_VectorMulM_U8C_U8C_Sat")));
__typeof__(__mlib_VectorMulM_S16C_S8C_Mod) mlib_VectorMulM_S16C_S8C_Mod
	__attribute__((weak, alias("__mlib_VectorMulM_S16C_S8C_Mod")));
__typeof__(__mlib_VectorMulM_S16C_U8C_Mod) mlib_VectorMulM_S16C_U8C_Mod
	__attribute__((weak, alias("__mlib_VectorMulM_S16C_U8C_Mod")));
__typeof__(__mlib_VectorMulM_S16_S8_Sat) mlib_VectorMulM_S16_S8_Sat
	__attribute__((weak, alias("__mlib_VectorMulM_S16_S8_Sat")));
__typeof__(__mlib_VectorMulM_S16_U8_Sat) mlib_VectorMulM_S16_U8_Sat
	__attribute__((weak, alias("__mlib_VectorMulM_S16_U8_Sat")));

#else

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* ****************************************************************** */

mlib_status
mlib_VectorMulM_S_U8_U8_Mod(
	mlib_u8 *z,
	const mlib_u8 *x,
	const mlib_u8 *y,
	mlib_s32 m,
	mlib_s32 n,
	mlib_s32 nstride)
{
	if (m < 1 || n < 1)
		return (MLIB_FAILURE);

	mlib_s32 i, k, sum, nstep, n1, n2, n3, xk;
	mlib_u8 *px, *py, *pz;
	__m128i xbuf, ybuf, zbuf, xhi, xlo, yhi, ylo, zero, mask;

	nstep = 16 / sizeof (mlib_u8);
	n1 = ((16 - ((mlib_addr)z & 15)) & 15) / sizeof (mlib_u8);
	n2 = (n - n1) / nstep;
	n3 = n - n1 - n2 * nstep;

	if (n == 1 && nstride == 1) {
		px = (mlib_u8 *)x;
		py = (mlib_u8 *)y;
		sum = 0;
		zero = _mm_setzero_si128();
		zbuf = zero;

		for (k = 0; k < m / nstep; k++) {
			xbuf = _mm_loadu_si128((__m128i *)px);
			ybuf = _mm_loadu_si128((__m128i *)py);

			xlo = _mm_unpacklo_epi8(zero, xbuf);
			xhi = _mm_unpackhi_epi8(zero, xbuf);
			ylo = _mm_unpacklo_epi8(zero, ybuf);
			yhi = _mm_unpackhi_epi8(zero, ybuf);

			xlo = _mm_mulhi_epu16(xlo, ylo);
			xhi = _mm_mulhi_epu16(xhi, yhi);
			zbuf = _mm_add_epi16(zbuf, xlo);
			zbuf = _mm_add_epi16(zbuf, xhi);

			px += nstep;
			py += nstep;
		}
		if (m >= nstep) {
			mlib_s16 tmp[8];
			_mm_storeu_si128((__m128i *)tmp, zbuf);
			for (k = 0; k < 8; k++)
				sum += tmp[k];
		}
		for (k = 0; k < m % nstep; k++) {
			sum += (mlib_s32)(*px) * (*py);
			px++;
			py++;
		}
		*z = sum;

	} else if (n2 < 1) {
		for (i = 0; i < n; i++) {
			sum = 0;
			px = (mlib_u8 *)x;
			py = (mlib_u8 *)y + i;
			for (k = 0; k < m; k++) {
				sum += (mlib_s32)(*px++) * (*py);
				py += nstride;
			}
			z[i] = sum;
		}
	} else {
		mlib_s32 delta = nstride - n;
		px = (mlib_u8 *)x;
		py = (mlib_u8 *)y;
		zero = _mm_setzero_si128();
		mask = _mm_set1_epi16(0xff);

		xk = px[0];
		pz = (mlib_u8 *)z;
		xbuf = _mm_set1_epi16(xk << 8);

		for (i = 0; i < n1; i++) {
			(*pz) = xk * (*py);
			py++;
			pz++;
		}

		for (i = 0; i < n2; i++) {
			ybuf = _mm_loadu_si128((__m128i *)py);
			ylo = _mm_unpacklo_epi8(zero, ybuf);
			yhi = _mm_unpackhi_epi8(zero, ybuf);

			ylo = _mm_mulhi_epu16(ylo, xbuf);
			yhi = _mm_mulhi_epu16(yhi, xbuf);
			ylo = _mm_and_si128(ylo, mask);
			yhi = _mm_and_si128(yhi, mask);
			zbuf = _mm_packus_epi16(ylo, yhi);

			_mm_store_si128((__m128i *)pz, zbuf);

			py += nstep;
			pz += nstep;
		}
		for (i = 0; i < n3; i++) {
			(*pz) = xk * (*py);
			py++;
			pz++;
		}
		py += delta;

		for (k = 1; k < m; k++) {
			xk = px[k];
			pz = (mlib_u8 *)z;
			xbuf = _mm_set1_epi16(xk << 8);

			for (i = 0; i < n1; i++) {
				(*pz) += xk * (*py);
				py++;
				pz++;
			}
			for (i = 0; i < n2; i++) {
				ybuf = _mm_loadu_si128((__m128i *)py);
				zbuf = _mm_load_si128((__m128i *)pz);
				ylo = _mm_unpacklo_epi8(zero, ybuf);
				yhi = _mm_unpackhi_epi8(zero, ybuf);

				ylo = _mm_mulhi_epu16(ylo, xbuf);
				yhi = _mm_mulhi_epu16(yhi, xbuf);
				ylo = _mm_and_si128(ylo, mask);
				yhi = _mm_and_si128(yhi, mask);
				ybuf = _mm_packus_epi16(ylo, yhi);

				zbuf = _mm_add_epi8(zbuf, ybuf);
				_mm_store_si128((__m128i *)pz, zbuf);

				py += nstep;
				pz += nstep;
			}

			for (i = 0; i < n3; i++) {
				(*pz) += xk * (*py);
				py++;
				pz++;
			}
			py += delta;
		}
	}

	return (MLIB_SUCCESS);
}

/* ****************************************************************** */

mlib_status
mlib_VectorMulM_S_S8_S8_Mod(
	mlib_s8 *z,
	const mlib_s8 *x,
	const mlib_s8 *y,
	mlib_s32 m,
	mlib_s32 n,
	mlib_s32 nstride)
{
	if (m < 1 || n < 1)
		return (MLIB_FAILURE);

	mlib_s32 i, k, sum, nstep, n1, n2, n3, xk;
	mlib_s8 *px, *py, *pz;
	__m128i xbuf, ybuf, zbuf, xhi, xlo, yhi, ylo, zero, mask;

	nstep = 16 / sizeof (mlib_s8);
	n1 = ((16 - ((mlib_addr)z & 15)) & 15) / sizeof (mlib_s8);
	n2 = (n - n1) / nstep;
	n3 = n - n1 - n2 * nstep;

	if (n == 1 && nstride == 1) {
		px = (mlib_s8 *)x;
		py = (mlib_s8 *)y;
		sum = 0;
		zero = _mm_setzero_si128();
		zbuf = zero;

		for (k = 0; k < m / nstep; k++) {
			xbuf = _mm_loadu_si128((__m128i *)px);
			ybuf = _mm_loadu_si128((__m128i *)py);

			xlo = _mm_unpacklo_epi8(zero, xbuf);
			xhi = _mm_unpackhi_epi8(zero, xbuf);
			ylo = _mm_unpacklo_epi8(zero, ybuf);
			yhi = _mm_unpackhi_epi8(zero, ybuf);

			xlo = _mm_mulhi_epi16(xlo, ylo);
			xhi = _mm_mulhi_epi16(xhi, yhi);
			zbuf = _mm_add_epi16(zbuf, xlo);
			zbuf = _mm_add_epi16(zbuf, xhi);

			px += nstep;
			py += nstep;
		}
		if (m >= nstep) {
			mlib_s16 tmp[8];
			_mm_storeu_si128((__m128i *)tmp, zbuf);
			for (k = 0; k < 8; k++)
				sum += tmp[k];
		}
		for (k = 0; k < m % nstep; k++) {
			sum += (mlib_s32)(*px) * (*py);
			px++;
			py++;
		}
		*z = sum;

	} else if (n2 < 1) {
		for (i = 0; i < n; i++) {
			sum = 0;
			px = (mlib_s8 *)x;
			py = (mlib_s8 *)y + i;
			for (k = 0; k < m; k++) {
				sum += (mlib_s32)(*px++) * (*py);
				py += nstride;
			}
			z[i] = sum;
		}
	} else {
		mlib_s32 delta = nstride - n;
		px = (mlib_s8 *)x;
		py = (mlib_s8 *)y;
		zero = _mm_setzero_si128();
		mask = _mm_set1_epi16(0xff);

		xk = px[0];
		pz = (mlib_s8 *)z;
		xbuf = _mm_set1_epi16(xk << 8);

		for (i = 0; i < n1; i++) {
			(*pz) = xk * (*py);
			py++;
			pz++;
		}
		for (i = 0; i < n2; i++) {
			ybuf = _mm_loadu_si128((__m128i *)py);
			ylo = _mm_unpacklo_epi8(zero, ybuf);
			yhi = _mm_unpackhi_epi8(zero, ybuf);

			ylo = _mm_mulhi_epu16(ylo, xbuf);
			yhi = _mm_mulhi_epu16(yhi, xbuf);
			ylo = _mm_and_si128(ylo, mask);
			yhi = _mm_and_si128(yhi, mask);
			zbuf = _mm_packus_epi16(ylo, yhi);

			_mm_store_si128((__m128i *)pz, zbuf);

			py += nstep;
			pz += nstep;
		}
		for (i = 0; i < n3; i++) {
			(*pz) = xk * (*py);
			py++;
			pz++;
		}
		py += delta;

		for (k = 1; k < m; k++) {
			xk = px[k];
			pz = (mlib_s8 *)z;
			xbuf = _mm_set1_epi16(xk << 8);

			for (i = 0; i < n1; i++) {
				(*pz) += xk * (*py);
				py++;
				pz++;
			}
			for (i = 0; i < n2; i++) {
				ybuf = _mm_loadu_si128((__m128i *)py);
				zbuf = _mm_load_si128((__m128i *)pz);
				ylo = _mm_unpacklo_epi8(zero, ybuf);
				yhi = _mm_unpackhi_epi8(zero, ybuf);

				ylo = _mm_mulhi_epi16(ylo, xbuf);
				yhi = _mm_mulhi_epi16(yhi, xbuf);
				ylo = _mm_and_si128(ylo, mask);
				yhi = _mm_and_si128(yhi, mask);
				ybuf = _mm_packus_epi16(ylo, yhi);

				zbuf = _mm_add_epi8(zbuf, ybuf);
				_mm_store_si128((__m128i *)pz, zbuf);

				py += nstep;
				pz += nstep;
			}
			for (i = 0; i < n3; i++) {
				(*pz) += xk * (*py);
				py++;
				pz++;
			}
			py += delta;
		}
	}

	return (MLIB_SUCCESS);
}

/* ****************************************************************** */

mlib_status
mlib_VectorMulM_S_S16_U8_Mod(
	mlib_s16 *z,
	const mlib_u8 *x,
	const mlib_u8 *y,
	mlib_s32 m,
	mlib_s32 n,
	mlib_s32 nstride)
{
	if (m < 1 || n < 1)
		return (MLIB_FAILURE);

	mlib_s32 i, k, sum, nstep, n1, n2, n3, xk;
	mlib_u8 *px, *py;
	mlib_s16 *pz;
	__m128i xbuf, ybuf, zbuf, xhi, xlo, yhi, ylo, zero, mask;

	nstep = 16 / sizeof (mlib_u8);
	n1 = ((16 - ((mlib_addr)z & 15)) & 15) / sizeof (mlib_s16);
	n2 = (n - n1) / nstep;
	n3 = n - n1 - n2 * nstep;

	if (n == 1 && nstride == 1) {
		px = (mlib_u8 *)x;
		py = (mlib_u8 *)y;
		sum = 0;
		zero = _mm_setzero_si128();
		zbuf = zero;

		for (k = 0; k < m / nstep; k++) {
			xbuf = _mm_loadu_si128((__m128i *)px);
			ybuf = _mm_loadu_si128((__m128i *)py);

			xlo = _mm_unpacklo_epi8(zero, xbuf);
			xhi = _mm_unpackhi_epi8(zero, xbuf);
			ylo = _mm_unpacklo_epi8(zero, ybuf);
			yhi = _mm_unpackhi_epi8(zero, ybuf);

			xlo = _mm_mulhi_epu16(xlo, ylo);
			xhi = _mm_mulhi_epu16(xhi, yhi);
			zbuf = _mm_add_epi16(zbuf, xlo);
			zbuf = _mm_add_epi16(zbuf, xhi);

			px += nstep;
			py += nstep;
		}
		if (m >= nstep) {
			mlib_s16 tmp[8];
			_mm_storeu_si128((__m128i *)tmp, zbuf);
			for (k = 0; k < 8; k++)
				sum += tmp[k];
		}
		for (k = 0; k < m % nstep; k++) {
			sum += (mlib_s32)(*px) * (*py);
			px++;
			py++;
		}
		*z = sum;

	} else if (n2 < 1) {
		for (i = 0; i < n; i++) {
			sum = 0;
			px = (mlib_u8 *)x;
			py = (mlib_u8 *)y + i;
			for (k = 0; k < m; k++) {
				sum += (mlib_s32)(*px++) * (*py);
				py += nstride;
			}
			z[i] = sum;
		}
	} else {
		mlib_s32 delta = nstride - n;
		px = (mlib_u8 *)x;
		py = (mlib_u8 *)y;
		zero = _mm_setzero_si128();

		xk = px[0];
		pz = (mlib_s16 *)z;
		xbuf = _mm_set1_epi16(xk << 8);

		for (i = 0; i < n1; i++) {
			(*pz) = xk * (*py);
			py++;
			pz++;
		}
		for (i = 0; i < n2; i++) {
			ybuf = _mm_loadu_si128((__m128i *)py);
			ylo = _mm_unpacklo_epi8(zero, ybuf);
			yhi = _mm_unpackhi_epi8(zero, ybuf);

			ylo = _mm_mulhi_epu16(ylo, xbuf);
			yhi = _mm_mulhi_epu16(yhi, xbuf);

			_mm_store_si128((__m128i *)pz, ylo);
			_mm_store_si128((__m128i *)pz + 1, yhi);

			py += nstep;
			pz += nstep;
		}
		for (i = 0; i < n3; i++) {
			(*pz) = xk * (*py);
			py++;
			pz++;
		}
		py += delta;

		for (k = 1; k < m; k++) {
			xk = px[k];
			pz = (mlib_s16 *)z;
			xbuf = _mm_set1_epi16(xk << 8);

			for (i = 0; i < n1; i++) {
				(*pz) += xk * (*py);
				py++;
				pz++;
			}
			for (i = 0; i < n2; i++) {
				ybuf = _mm_loadu_si128((__m128i *)py);
				xlo = _mm_load_si128((__m128i *)pz);
				xhi = _mm_load_si128((__m128i *)pz + 1);

				ylo = _mm_unpacklo_epi8(zero, ybuf);
				yhi = _mm_unpackhi_epi8(zero, ybuf);

				ylo = _mm_mulhi_epu16(ylo, xbuf);
				yhi = _mm_mulhi_epu16(yhi, xbuf);

				ylo = _mm_add_epi16(ylo, xlo);
				yhi = _mm_add_epi16(yhi, xhi);

				_mm_store_si128((__m128i *)pz, ylo);
				_mm_store_si128((__m128i *)pz + 1, yhi);

				py += nstep;
				pz += nstep;
			}

			for (i = 0; i < n3; i++) {
				(*pz) += xk * (*py);
				py++;
				pz++;
			}
			py += delta;
		}
	}

	return (MLIB_SUCCESS);
}

/* ****************************************************************** */

mlib_status
mlib_VectorMulM_S_S16_S8_Mod(
	mlib_s16 *z,
	const mlib_s8 *x,
	const mlib_s8 *y,
	mlib_s32 m,
	mlib_s32 n,
	mlib_s32 nstride)
{
	if (m < 1 || n < 1)
		return (MLIB_FAILURE);

	mlib_s32 i, k, sum, nstep, n1, n2, n3, xk;
	mlib_s8 *px, *py;
	mlib_s16 *pz;
	__m128i xbuf, ybuf, zbuf, xhi, xlo, yhi, ylo, zero, mask;

	nstep = 16 / sizeof (mlib_s8);
	n1 = ((16 - ((mlib_addr)z & 15)) & 15) / sizeof (mlib_s16);
	n2 = (n - n1) / nstep;
	n3 = n - n1 - n2 * nstep;

	if (n == 1 && nstride == 1) {
		px = (mlib_s8 *)x;
		py = (mlib_s8 *)y;
		sum = 0;
		zero = _mm_setzero_si128();
		zbuf = zero;

		for (k = 0; k < m / nstep; k++) {
			xbuf = _mm_loadu_si128((__m128i *)px);
			ybuf = _mm_loadu_si128((__m128i *)py);

			xlo = _mm_unpacklo_epi8(zero, xbuf);
			xhi = _mm_unpackhi_epi8(zero, xbuf);
			ylo = _mm_unpacklo_epi8(zero, ybuf);
			yhi = _mm_unpackhi_epi8(zero, ybuf);

			xlo = _mm_mulhi_epi16(xlo, ylo);
			xhi = _mm_mulhi_epi16(xhi, yhi);
			zbuf = _mm_add_epi16(zbuf, xlo);
			zbuf = _mm_add_epi16(zbuf, xhi);

			px += nstep;
			py += nstep;
		}
		if (m >= nstep) {
			mlib_s16 tmp[8];
			_mm_storeu_si128((__m128i *)tmp, zbuf);
			for (k = 0; k < 8; k++)
				sum += tmp[k];
		}
		for (k = 0; k < m % nstep; k++) {
			sum += (mlib_s32)(*px) * (*py);
			px++;
			py++;
		}
		*z = sum;

	} else if (n2 < 1) {
		for (i = 0; i < n; i++) {
			sum = 0;
			px = (mlib_s8 *)x;
			py = (mlib_s8 *)y + i;
			for (k = 0; k < m; k++) {
				sum += (mlib_s32)(*px++) * (*py);
				py += nstride;
			}
			z[i] = sum;
		}
	} else {
		mlib_s32 delta = nstride - n;
		px = (mlib_s8 *)x;
		py = (mlib_s8 *)y;
		zero = _mm_setzero_si128();

		xk = px[0];
		pz = (mlib_s16 *)z;
		xbuf = _mm_set1_epi16(xk << 8);

		for (i = 0; i < n1; i++) {
			(*pz) = xk * (*py);
			py++;
			pz++;
		}
		for (i = 0; i < n2; i++) {
			ybuf = _mm_loadu_si128((__m128i *)py);
			ylo = _mm_unpacklo_epi8(zero, ybuf);
			yhi = _mm_unpackhi_epi8(zero, ybuf);

			ylo = _mm_mulhi_epi16(ylo, xbuf);
			yhi = _mm_mulhi_epi16(yhi, xbuf);

			_mm_store_si128((__m128i *)pz, ylo);
			_mm_store_si128((__m128i *)pz + 1, yhi);

			py += nstep;
			pz += nstep;
		}
		for (i = 0; i < n3; i++) {
			(*pz) = xk * (*py);
			py++;
			pz++;
		}
		py += delta;

		for (k = 1; k < m; k++) {
			xk = px[k];
			pz = (mlib_s16 *)z;
			xbuf = _mm_set1_epi16(xk << 8);

			for (i = 0; i < n1; i++) {
				(*pz) += xk * (*py);
				py++;
				pz++;
			}
			for (i = 0; i < n2; i++) {
				ybuf = _mm_loadu_si128((__m128i *)py);
				xlo = _mm_load_si128((__m128i *)pz);
				xhi = _mm_load_si128((__m128i *)pz + 1);

				ylo = _mm_unpacklo_epi8(zero, ybuf);
				yhi = _mm_unpackhi_epi8(zero, ybuf);

				ylo = _mm_mulhi_epi16(ylo, xbuf);
				yhi = _mm_mulhi_epi16(yhi, xbuf);

				ylo = _mm_add_epi16(ylo, xlo);
				yhi = _mm_add_epi16(yhi, xhi);

				_mm_store_si128((__m128i *)pz, ylo);
				_mm_store_si128((__m128i *)pz + 1, yhi);

				py += nstep;
				pz += nstep;
			}

			for (i = 0; i < n3; i++) {
				(*pz) += xk * (*py);
				py++;
				pz++;
			}
			py += delta;
		}
	}

	return (MLIB_SUCCESS);
}

/* ****************************************************************** */

mlib_status
mlib_VectorMulM_S_S16_S16_Mod(
	mlib_s16 *z,
	const mlib_s16 *x,
	const mlib_s16 *y,
	mlib_s32 m,
	mlib_s32 n,
	mlib_s32 nstride)
{
	if (m < 1 || n < 1)
		return (MLIB_FAILURE);

	mlib_s32 i, k, sum, nstep, n1, n2, n3, xk;
	mlib_s16 *px, *py;
	mlib_s16 *pz;
	__m128i xbuf, ybuf, zbuf, xhi, xlo, yhi, ylo, zero, mask;

	nstep = 16 / sizeof (mlib_s16);
	n1 = ((16 - ((mlib_addr)z & 15)) & 15) / sizeof (mlib_s16);
	n2 = (n - n1) / nstep;
	n3 = n - n1 - n2 * nstep;

	if (n == 1 && nstride == 1) {
		px = (mlib_s16 *)x;
		py = (mlib_s16 *)y;
		sum = 0;
		zbuf = _mm_setzero_si128();

		for (k = 0; k < m / nstep; k++) {
			xbuf = _mm_loadu_si128((__m128i *)px);
			ybuf = _mm_loadu_si128((__m128i *)py);

			xlo = _mm_mullo_epi16(xbuf, ybuf);
			zbuf = _mm_add_epi16(zbuf, xlo);

			px += nstep;
			py += nstep;
		}
		if (m >= nstep) {
			mlib_s16 tmp[8];
			_mm_storeu_si128((__m128i *)tmp, zbuf);
			for (k = 0; k < 8; k++)
				sum += tmp[k];
		}
		for (k = 0; k < m % nstep; k++) {
			sum += (mlib_s32)(*px) * (*py);
			px++;
			py++;
		}
		*z = sum;

	} else if (n2 < 1) {
		for (i = 0; i < n; i++) {
			sum = 0;
			px = (mlib_s16 *)x;
			py = (mlib_s16 *)y + i;
			for (k = 0; k < m; k++) {
				sum += (mlib_s32)(*px++) * (*py);
				py += nstride;
			}
			z[i] = sum;
		}
	} else {
		mlib_s32 delta = nstride - n;
		px = (mlib_s16 *)x;
		py = (mlib_s16 *)y;

		xk = px[0];
		pz = (mlib_s16 *)z;
		xbuf = _mm_set1_epi16(xk);

		for (i = 0; i < n1; i++) {
			(*pz) = xk * (*py);
			py++;
			pz++;
		}
		for (i = 0; i < n2; i++) {
			ybuf = _mm_loadu_si128((__m128i *)py);
			zbuf = _mm_mullo_epi16(xbuf, ybuf);

			_mm_store_si128((__m128i *)pz, zbuf);

			py += nstep;
			pz += nstep;
		}
		for (i = 0; i < n3; i++) {
			(*pz) = xk * (*py);
			py++;
			pz++;
		}
		py += delta;

		for (k = 1; k < m; k++) {
			xk = px[k];
			pz = (mlib_s16 *)z;
			xbuf = _mm_set1_epi16(xk);

			for (i = 0; i < n1; i++) {
				(*pz) += xk * (*py);
				py++;
				pz++;
			}
			for (i = 0; i < n2; i++) {
				ybuf = _mm_loadu_si128((__m128i *)py);
				zbuf = _mm_load_si128((__m128i *)pz);

				ybuf = _mm_mullo_epi16(ybuf, xbuf);
				zbuf = _mm_add_epi16(zbuf, ybuf);

				_mm_store_si128((__m128i *)pz, zbuf);

				py += nstep;
				pz += nstep;
			}

			for (i = 0; i < n3; i++) {
				(*pz) += xk * (*py);
				py++;
				pz++;
			}
			py += delta;
		}
	}

	return (MLIB_SUCCESS);
}

/* ****************************************************************** */

mlib_status
mlib_VectorMulM_S_S32_S16_Mod(
	mlib_s32 *z,
	const mlib_s16 *x,
	const mlib_s16 *y,
	mlib_s32 m,
	mlib_s32 n,
	mlib_s32 nstride)
{
	if (m < 1 || n < 1)
		return (MLIB_FAILURE);

	mlib_s32 i, k, sum, nstep, n1, n2, n3, xk;
	mlib_s16 *px, *py;
	mlib_s32 *pz;
	__m128i xbuf, ybuf, zbuf, xhi, xlo, yhi, ylo, zero, mask;

	nstep = 16 / sizeof (mlib_s16);
	n1 = ((16 - ((mlib_addr)z & 15)) & 15) / sizeof (mlib_s32);
	n2 = (n - n1) / nstep;
	n3 = n - n1 - n2 * nstep;

	if (n == 1 && nstride == 1) {
		px = (mlib_s16 *)x;
		py = (mlib_s16 *)y;
		sum = 0;
		ylo = _mm_setzero_si128();
		yhi = ylo;

		for (k = 0; k < m / nstep; k++) {
			xbuf = _mm_loadu_si128((__m128i *)px);
			ybuf = _mm_loadu_si128((__m128i *)py);

			xlo = _mm_mullo_epi16(xbuf, ybuf);
			xhi = _mm_mulhi_epi16(xbuf, ybuf);
			xbuf = _mm_unpacklo_epi16(xlo, xhi);
			ybuf = _mm_unpackhi_epi16(xlo, xhi);
			ylo = _mm_add_epi32(ylo, xbuf);
			yhi = _mm_add_epi32(yhi, ybuf);

			px += nstep;
			py += nstep;
		}
		if (m >= nstep) {
			mlib_s32 tmp[4];
			ylo = _mm_add_epi32(ylo, yhi);
			_mm_storeu_si128((__m128i *)tmp, ylo);
			for (k = 0; k < 4; k++)
				sum += tmp[k];
		}
		for (k = 0; k < m % nstep; k++) {
			sum += (mlib_s32)(*px) * (*py);
			px++;
			py++;
		}
		*z = sum;

	} else if (n2 < 1) {
		for (i = 0; i < n; i++) {
			sum = 0;
			px = (mlib_s16 *)x;
			py = (mlib_s16 *)y + i;
			for (k = 0; k < m; k++) {
				sum += (mlib_s32)(*px++) * (*py);
				py += nstride;
			}
			z[i] = sum;
		}
	} else {
		mlib_s32 delta = nstride - n;
		px = (mlib_s16 *)x;
		py = (mlib_s16 *)y;
		zero = _mm_setzero_si128();

		xk = px[0];
		pz = (mlib_s32 *)z;
		xbuf = _mm_set1_epi16(xk);

		for (i = 0; i < n1; i++) {
			(*pz) = xk * (*py);
			py++;
			pz++;
		}
		for (i = 0; i < n2; i++) {
			ybuf = _mm_loadu_si128((__m128i *)py);
			ylo = _mm_mullo_epi16(xbuf, ybuf);
			yhi = _mm_mulhi_epi16(xbuf, ybuf);

			ybuf = _mm_unpacklo_epi16(ylo, yhi);
			zbuf = _mm_unpackhi_epi16(ylo, yhi);
			_mm_store_si128((__m128i *)pz, ybuf);
			_mm_store_si128((__m128i *)pz + 1, zbuf);

			py += nstep;
			pz += nstep;
		}
		for (i = 0; i < n3; i++) {
			(*pz) = xk * (*py);
			py++;
			pz++;
		}
		py += delta;

		for (k = 1; k < m; k++) {
			xk = px[k];
			pz = (mlib_s32 *)z;
			xbuf = _mm_set1_epi16(xk);

			for (i = 0; i < n1; i++) {
				(*pz) += xk * (*py);
				py++;
				pz++;
			}
			for (i = 0; i < n2; i++) {
				ybuf = _mm_loadu_si128((__m128i *)py);
				xlo = _mm_load_si128((__m128i *)pz);
				xhi = _mm_load_si128((__m128i *)pz + 1);

				ylo = _mm_mullo_epi16(ybuf, xbuf);
				yhi = _mm_mulhi_epi16(ybuf, xbuf);
				ybuf = _mm_unpacklo_epi16(ylo, yhi);
				zbuf = _mm_unpackhi_epi16(ylo, yhi);
				ybuf = _mm_add_epi32(xlo, ybuf);
				zbuf = _mm_add_epi32(xhi, zbuf);

				_mm_store_si128((__m128i *)pz, ybuf);
				_mm_store_si128((__m128i *)pz + 1, zbuf);

				py += nstep;
				pz += nstep;
			}

			for (i = 0; i < n3; i++) {
				(*pz) += xk * (*py);
				py++;
				pz++;
			}
			py += delta;
		}
	}

	return (MLIB_SUCCESS);
}

/* ****************************************************************** */

mlib_status
__mlib_VectorMulM_U8_U8_Mod(
	mlib_u8 *z,
	const mlib_u8 *x,
	const mlib_u8 *y,
	mlib_s32 m,
	mlib_s32 n)
{
	return (mlib_VectorMulM_S_U8_U8_Mod(z, x, y, m, n, n));
}

mlib_status
__mlib_VectorMulM_S8_S8_Mod(
	mlib_s8 *z,
	const mlib_s8 *x,
	const mlib_s8 *y,
	mlib_s32 m,
	mlib_s32 n)
{
	return (mlib_VectorMulM_S_S8_S8_Mod(z, x, y, m, n, n));
}

mlib_status
__mlib_VectorMulM_S16_U8_Mod(
	mlib_s16 *z,
	const mlib_u8 *x,
	const mlib_u8 *y,
	mlib_s32 m,
	mlib_s32 n)
{
	return (mlib_VectorMulM_S_S16_U8_Mod(z, x, y, m, n, n));
}

mlib_status
__mlib_VectorMulM_S16_S8_Mod(
	mlib_s16 *z,
	const mlib_s8 *x,
	const mlib_s8 *y,
	mlib_s32 m,
	mlib_s32 n)
{
	return (mlib_VectorMulM_S_S16_S8_Mod(z, x, y, m, n, n));
}

mlib_status
__mlib_VectorMulM_S16_S16_Mod(
	mlib_s16 *z,
	const mlib_s16 *x,
	const mlib_s16 *y,
	mlib_s32 m,
	mlib_s32 n)
{
	return (mlib_VectorMulM_S_S16_S16_Mod(z, x, y, m, n, n));
}

mlib_status
__mlib_VectorMulM_S32_S16_Mod(
	mlib_s32 *z,
	const mlib_s16 *x,
	const mlib_s16 *y,
	mlib_s32 m,
	mlib_s32 n)
{
	return (mlib_VectorMulM_S_S32_S16_Mod(z, x, y, m, n, n));
}

/* ****************************************************************** */

	VECTOR_MULM_FP(U8, U8, Sat)
	VECTOR_MULM_FP(S8, S8, Sat)
	VECTOR_MULM_FP(S16, U8, Sat)
	VECTOR_MULM_FP(S16, S8, Sat)
	VECTOR_MULM_FP(S16, S16, Sat)
	VECTOR_MULM_FP(S32, S16, Sat)

/* ****************************************************************** */

mlib_status
mlib_VectorMulM_S_U8C_U8C_Mod(
	mlib_u8 *z,
	const mlib_u8 *x,
	const mlib_u8 *y,
	mlib_s32 m,
	mlib_s32 n,
	mlib_s32 nstride)
{
	if (m < 1 || n < 1)
		return (MLIB_FAILURE);

	mlib_s32 i, k, sum0, sum1, nstep, n1, n2, n3, x0, x1;
	mlib_u8 *px, *py, *pz;
	__m128i xbuf0, xbuf1, ybuf, zbuf, ylo, yhi, zero, mask1, mask2;
	__m128i buf1, buf2, buf3, buf4;

	nstep = 16 / sizeof (mlib_u8);
	n2 = (n + n) / nstep;
	n3 = (n + n - n2 * nstep) / 2;

	if (n2 < 1) {
		pz = (mlib_u8 *)z;
		for (i = 0; i < n; i++) {
			sum0 = 0;
			sum1 = 0;
			px = (mlib_u8 *)x;
			py = (mlib_u8 *)y + i * 2;
			for (k = 0; k < m; k++) {
				sum0 += (mlib_s32)px[0] * py[0]
					- (mlib_s32)px[1] * py[1];
				sum1 += (mlib_s32)px[0] * py[1]
					+ (mlib_s32)px[1] * py[0];
				px += 2;
				py += nstride * 2;
			}
			pz[0] = sum0;
			pz[1] = sum1;
			pz += 2;
		}
	} else {
		mlib_s32 delta = (nstride - n) * 2;
		px = (mlib_u8 *)x;
		py = (mlib_u8 *)y;
		zero = _mm_setzero_si128();
		mask1 = _mm_set1_epi16(0x00ff);
		mask2 = _mm_set1_epi32(0xffff);

		x0 = px[0];
		x1 = px[1];
		xbuf0 = _mm_set1_epi32((x0 & 0xffff) | ((-x1) << 16));
		xbuf1 = _mm_set1_epi32((x0 << 16) | (x1 & 0xffff));
		pz = (mlib_u8 *)z;

		for (i = 0; i < n2; i++) {
			ybuf = _mm_loadu_si128((__m128i *)py);
			ylo = _mm_unpacklo_epi8(ybuf, zero);
			yhi = _mm_unpackhi_epi8(ybuf, zero);

			buf1 = _mm_madd_epi16(xbuf0, ylo);
			buf2 = _mm_madd_epi16(xbuf0, yhi);
			buf3 = _mm_madd_epi16(xbuf1, ylo);
			buf4 = _mm_madd_epi16(xbuf1, yhi);

			buf1 = _mm_and_si128(buf1, mask1);
			buf2 = _mm_and_si128(buf2, mask1);
			buf3 = _mm_slli_epi16(buf3, 8);
			buf4 = _mm_slli_epi16(buf4, 8);
			buf1 = _mm_or_si128(buf1, buf3);
			buf2 = _mm_or_si128(buf2, buf4);

			buf3 = _mm_unpacklo_epi32(buf1, buf2);
			buf4 = _mm_unpackhi_epi32(buf1, buf2);
			buf1 = _mm_unpacklo_epi32(buf3, buf4);
			buf2 = _mm_unpackhi_epi32(buf3, buf4);
			buf1 = _mm_and_si128(buf1, mask2);
			buf2 = _mm_slli_epi32(buf2, 16);
			buf1 = _mm_or_si128(buf1, buf2);

			_mm_storeu_si128((__m128i *)pz, buf1);

			py += nstep;
			pz += nstep;
		}
		for (i = 0; i < n3; i++) {
			pz[0] = x0 * py[0] - x1 * py[1];
			pz[1] = x0 * py[1] + x1 * py[0];
			py += 2;
			pz += 2;
		}
		py += delta;

		for (k = 1; k < m; k++) {
			x0 = px[k * 2 + 0];
			x1 = px[k * 2 + 1];
			xbuf0 = _mm_set1_epi32((x0 & 0xffff) | ((-x1) << 16));
			xbuf1 = _mm_set1_epi32((x0 << 16) | (x1 & 0xffff));
			pz = (mlib_u8 *)z;

			for (i = 0; i < n2; i++) {
				ybuf = _mm_loadu_si128((__m128i *)py);
				zbuf = _mm_loadu_si128((__m128i *)pz);
				ylo = _mm_unpacklo_epi8(ybuf, zero);
				yhi = _mm_unpackhi_epi8(ybuf, zero);

				buf1 = _mm_madd_epi16(xbuf0, ylo);
				buf2 = _mm_madd_epi16(xbuf0, yhi);
				buf3 = _mm_madd_epi16(xbuf1, ylo);
				buf4 = _mm_madd_epi16(xbuf1, yhi);

				buf1 = _mm_and_si128(buf1, mask1);
				buf2 = _mm_and_si128(buf2, mask1);
				buf3 = _mm_slli_epi16(buf3, 8);
				buf4 = _mm_slli_epi16(buf4, 8);
				buf1 = _mm_or_si128(buf1, buf3);
				buf2 = _mm_or_si128(buf2, buf4);

				buf3 = _mm_unpacklo_epi32(buf1, buf2);
				buf4 = _mm_unpackhi_epi32(buf1, buf2);
				buf1 = _mm_unpacklo_epi32(buf3, buf4);
				buf2 = _mm_unpackhi_epi32(buf3, buf4);
				buf1 = _mm_and_si128(buf1, mask2);
				buf2 = _mm_slli_epi32(buf2, 16);
				buf1 = _mm_or_si128(buf1, buf2);

				zbuf = _mm_add_epi8(zbuf, buf1);
				_mm_storeu_si128((__m128i *)pz, zbuf);

				py += nstep;
				pz += nstep;
			}
			for (i = 0; i < n3; i++) {
				pz[0] += x0 * py[0] - x1 * py[1];
				pz[1] += x0 * py[1] + x1 * py[0];
				py += 2;
				pz += 2;
			}
			py += delta;
		}
	}

	return (MLIB_SUCCESS);
}

/* ****************************************************************** */

mlib_status
mlib_VectorMulM_S_S8C_S8C_Mod(
	mlib_s8 *z,
	const mlib_s8 *x,
	const mlib_s8 *y,
	mlib_s32 m,
	mlib_s32 n,
	mlib_s32 nstride)
{
	if (m < 1 || n < 1)
		return (MLIB_FAILURE);

	mlib_s32 i, k, sum0, sum1, nstep, n1, n2, n3, x0, x1;
	mlib_s8 *px, *py, *pz;
	__m128i xbuf0, xbuf1, ybuf, zbuf, ylo, yhi, zero, mask1, mask2;
	__m128i buf1, buf2, buf3, buf4;

	nstep = 16 / sizeof (mlib_s8);
	n2 = (n + n) / nstep;
	n3 = (n + n - n2 * nstep) / 2;
	if (n2 < 1) {
		pz = (mlib_s8 *)z;
		for (i = 0; i < n; i++) {
			sum0 = 0;
			sum1 = 0;
			px = (mlib_s8 *)x;
			py = (mlib_s8 *)y + i * 2;
			for (k = 0; k < m; k++) {
				sum0 += (mlib_s32)px[0] * py[0]
					- (mlib_s32)px[1] * py[1];
				sum1 += (mlib_s32)px[0] * py[1]
					+ (mlib_s32)px[1] * py[0];
				px += 2;
				py += nstride * 2;
			}
			pz[0] = sum0;
			pz[1] = sum1;
			pz += 2;
		}
	} else {
		mlib_s32 delta = (nstride - n) * 2;
		px = (mlib_s8 *)x;
		py = (mlib_s8 *)y;
		zero = _mm_setzero_si128();
		mask1 = _mm_set1_epi16(0x00ff);
		mask2 = _mm_set1_epi32(0xffff);

		x0 = px[0];
		x1 = px[1];
		xbuf0 = _mm_set1_epi32((x0 & 0xffff) | ((-x1) << 16));
		xbuf1 = _mm_set1_epi32((x0 << 16) | (x1 & 0xffff));
		pz = (mlib_s8 *)z;

		for (i = 0; i < n2; i++) {
			ybuf = _mm_loadu_si128((__m128i *)py);
			ylo = _mm_unpacklo_epi8(zero, ybuf);
			yhi = _mm_unpackhi_epi8(zero, ybuf);
			ylo = _mm_srai_epi16(ylo, 8);
			yhi = _mm_srai_epi16(yhi, 8);

			buf1 = _mm_madd_epi16(xbuf0, ylo);
			buf2 = _mm_madd_epi16(xbuf0, yhi);
			buf3 = _mm_madd_epi16(xbuf1, ylo);
			buf4 = _mm_madd_epi16(xbuf1, yhi);

			buf1 = _mm_and_si128(buf1, mask1);
			buf2 = _mm_and_si128(buf2, mask1);
			buf3 = _mm_slli_epi16(buf3, 8);
			buf4 = _mm_slli_epi16(buf4, 8);
			buf1 = _mm_or_si128(buf1, buf3);
			buf2 = _mm_or_si128(buf2, buf4);

			buf3 = _mm_unpacklo_epi32(buf1, buf2);
			buf4 = _mm_unpackhi_epi32(buf1, buf2);
			buf1 = _mm_unpacklo_epi32(buf3, buf4);
			buf2 = _mm_unpackhi_epi32(buf3, buf4);
			buf1 = _mm_and_si128(buf1, mask2);
			buf2 = _mm_slli_epi32(buf2, 16);
			buf1 = _mm_or_si128(buf1, buf2);

			_mm_storeu_si128((__m128i *)pz, buf1);

			py += nstep;
			pz += nstep;
		}
		for (i = 0; i < n3; i++) {
			pz[0] = x0 * py[0] - x1 * py[1];
			pz[1] = x0 * py[1] + x1 * py[0];
			py += 2;
			pz += 2;
		}
		py += delta;

		for (k = 1; k < m; k++) {
			x0 = px[k * 2 + 0];
			x1 = px[k * 2 + 1];
			xbuf0 = _mm_set1_epi32((x0 & 0xffff) | ((-x1) << 16));
			xbuf1 = _mm_set1_epi32((x0 << 16) | (x1 & 0xffff));
			pz = (mlib_s8 *)z;

			for (i = 0; i < n2; i++) {
				ybuf = _mm_loadu_si128((__m128i *)py);
				zbuf = _mm_loadu_si128((__m128i *)pz);
				ylo = _mm_unpacklo_epi8(zero, ybuf);
				yhi = _mm_unpackhi_epi8(zero, ybuf);
				ylo = _mm_srai_epi16(ylo, 8);
				yhi = _mm_srai_epi16(yhi, 8);

				buf1 = _mm_madd_epi16(xbuf0, ylo);
				buf2 = _mm_madd_epi16(xbuf0, yhi);
				buf3 = _mm_madd_epi16(xbuf1, ylo);
				buf4 = _mm_madd_epi16(xbuf1, yhi);

				buf1 = _mm_and_si128(buf1, mask1);
				buf2 = _mm_and_si128(buf2, mask1);
				buf3 = _mm_slli_epi16(buf3, 8);
				buf4 = _mm_slli_epi16(buf4, 8);
				buf1 = _mm_or_si128(buf1, buf3);
				buf2 = _mm_or_si128(buf2, buf4);

				buf3 = _mm_unpacklo_epi32(buf1, buf2);
				buf4 = _mm_unpackhi_epi32(buf1, buf2);
				buf1 = _mm_unpacklo_epi32(buf3, buf4);
				buf2 = _mm_unpackhi_epi32(buf3, buf4);
				buf1 = _mm_and_si128(buf1, mask2);
				buf2 = _mm_slli_epi32(buf2, 16);
				buf1 = _mm_or_si128(buf1, buf2);

				zbuf = _mm_add_epi8(zbuf, buf1);
				_mm_storeu_si128((__m128i *)pz, zbuf);

				py += nstep;
				pz += nstep;
			}
			for (i = 0; i < n3; i++) {
				pz[0] += x0 * py[0] - x1 * py[1];
				pz[1] += x0 * py[1] + x1 * py[0];
				py += 2;
				pz += 2;
			}
			py += delta;
		}
	}

	return (MLIB_SUCCESS);
}

/* ****************************************************************** */

mlib_status
mlib_VectorMulM_S_S16C_U8C_Mod(
	mlib_s16 *z,
	const mlib_u8 *x,
	const mlib_u8 *y,
	mlib_s32 m,
	mlib_s32 n,
	mlib_s32 nstride)
{
	if (m < 1 || n < 1)
		return (MLIB_FAILURE);

	mlib_s32 i, k, sum0, sum1, nstep, n1, n2, n3, x0, x1;
	mlib_u8 *px, *py;
	mlib_s16 *pz;
	__m128i xbuf0, xbuf1, ybuf, ylo, yhi, zlo, zhi, zero, mask;
	__m128i buf1, buf2, buf3, buf4;

	nstep = 16 / sizeof (mlib_u8);
	n2 = (n + n) / nstep;
	n3 = (n + n - n2 * nstep) / 2;

	if (n2 < 1) {
		pz = (mlib_s16 *)z;
		for (i = 0; i < n; i++) {
			sum0 = 0;
			sum1 = 0;
			px = (mlib_u8 *)x;
			py = (mlib_u8 *)y + i * 2;
			for (k = 0; k < m; k++) {
				sum0 += (mlib_s32)px[0] * py[0]
					- (mlib_s32)px[1] * py[1];
				sum1 += (mlib_s32)px[0] * py[1]
					+ (mlib_s32)px[1] * py[0];
				px += 2;
				py += nstride * 2;
			}
			pz[0] = sum0;
			pz[1] = sum1;
			pz += 2;
		}
	} else {
		mlib_s32 delta = (nstride - n) * 2;
		px = (mlib_u8 *)x;
		py = (mlib_u8 *)y;
		zero = _mm_setzero_si128();
		mask = _mm_set1_epi32(0xffff);

		x0 = px[0];
		x1 = px[1];
		xbuf0 = _mm_set1_epi32((x0 & 0xffff) | ((-x1) << 16));
		xbuf1 = _mm_set1_epi32((x0 << 16) | (x1 & 0xffff));
		pz = (mlib_s16 *)z;

		for (i = 0; i < n2; i++) {
			ybuf = _mm_loadu_si128((__m128i *)py);
			ylo = _mm_unpacklo_epi8(ybuf, zero);
			yhi = _mm_unpackhi_epi8(ybuf, zero);

			buf1 = _mm_madd_epi16(xbuf0, ylo);
			buf2 = _mm_madd_epi16(xbuf0, yhi);
			buf3 = _mm_madd_epi16(xbuf1, ylo);
			buf4 = _mm_madd_epi16(xbuf1, yhi);

			buf1 = _mm_and_si128(buf1, mask);
			buf2 = _mm_and_si128(buf2, mask);
			buf3 = _mm_slli_epi32(buf3, 16);
			buf4 = _mm_slli_epi32(buf4, 16);
			buf1 = _mm_or_si128(buf1, buf3);
			buf2 = _mm_or_si128(buf2, buf4);

			_mm_storeu_si128((__m128i *)pz, buf1);
			_mm_storeu_si128((__m128i *)pz + 1, buf2);

			py += nstep;
			pz += nstep;
		}
		for (i = 0; i < n3; i++) {
			pz[0] = x0 * py[0] - x1 * py[1];
			pz[1] = x0 * py[1] + x1 * py[0];
			py += 2;
			pz += 2;
		}
		py += delta;

		for (k = 1; k < m; k++) {
			x0 = px[k * 2 + 0];
			x1 = px[k * 2 + 1];
			xbuf0 = _mm_set1_epi32((x0 & 0xffff) | ((-x1) << 16));
			xbuf1 = _mm_set1_epi32((x0 << 16) | (x1 & 0xffff));
			pz = (mlib_s16 *)z;

			for (i = 0; i < n2; i++) {
				ybuf = _mm_loadu_si128((__m128i *)py);
				zlo = _mm_loadu_si128((__m128i *)pz);
				zhi = _mm_loadu_si128((__m128i *)pz + 1);
				ylo = _mm_unpacklo_epi8(ybuf, zero);
				yhi = _mm_unpackhi_epi8(ybuf, zero);

				buf1 = _mm_madd_epi16(xbuf0, ylo);
				buf2 = _mm_madd_epi16(xbuf0, yhi);
				buf3 = _mm_madd_epi16(xbuf1, ylo);
				buf4 = _mm_madd_epi16(xbuf1, yhi);

				buf1 = _mm_and_si128(buf1, mask);
				buf2 = _mm_and_si128(buf2, mask);
				buf3 = _mm_slli_epi32(buf3, 16);
				buf4 = _mm_slli_epi32(buf4, 16);
				buf1 = _mm_or_si128(buf1, buf3);
				buf2 = _mm_or_si128(buf2, buf4);

				zlo = _mm_add_epi16(zlo, buf1);
				zhi = _mm_add_epi16(zhi, buf2);
				_mm_storeu_si128((__m128i *)pz, zlo);
				_mm_storeu_si128((__m128i *)pz + 1, zhi);

				py += nstep;
				pz += nstep;
			}
			for (i = 0; i < n3; i++) {
				pz[0] += x0 * py[0] - x1 * py[1];
				pz[1] += x0 * py[1] + x1 * py[0];
				py += 2;
				pz += 2;
			}
			py += delta;
		}
	}

	return (MLIB_SUCCESS);
}

/* ****************************************************************** */

mlib_status
mlib_VectorMulM_S_S16C_S8C_Mod(
	mlib_s16 *z,
	const mlib_s8 *x,
	const mlib_s8 *y,
	mlib_s32 m,
	mlib_s32 n,
	mlib_s32 nstride)
{
	if (m < 1 || n < 1)
		return (MLIB_FAILURE);

	mlib_s32 i, k, sum0, sum1, nstep, n1, n2, n3, x0, x1;
	mlib_s8 *px, *py;
	mlib_s16 *pz;
	__m128i xbuf0, xbuf1, ybuf, ylo, yhi, zlo, zhi, zero, mask;
	__m128i buf1, buf2, buf3, buf4;

	nstep = 16 / sizeof (mlib_s8);
	n2 = (n + n) / nstep;
	n3 = (n + n - n2 * nstep) / 2;

	if (n2 < 1) {
		pz = (mlib_s16 *)z;
		for (i = 0; i < n; i++) {
			sum0 = 0;
			sum1 = 0;
			px = (mlib_s8 *)x;
			py = (mlib_s8 *)y + i * 2;
			for (k = 0; k < m; k++) {
				sum0 += (mlib_s32)px[0] * py[0]
					- (mlib_s32)px[1] * py[1];
				sum1 += (mlib_s32)px[0] * py[1]
					+ (mlib_s32)px[1] * py[0];
				px += 2;
				py += nstride * 2;
			}
			pz[0] = sum0;
			pz[1] = sum1;
			pz += 2;
		}
	} else {
		mlib_s32 delta = (nstride - n) * 2;
		px = (mlib_s8 *)x;
		py = (mlib_s8 *)y;
		zero = _mm_setzero_si128();
		mask = _mm_set1_epi32(0xffff);

		x0 = px[0];
		x1 = px[1];
		xbuf0 = _mm_set1_epi32((x0 & 0xffff) | ((-x1) << 16));
		xbuf1 = _mm_set1_epi32((x0 << 16) | (x1 & 0xffff));
		pz = (mlib_s16 *)z;

		for (i = 0; i < n2; i++) {
			ybuf = _mm_loadu_si128((__m128i *)py);
			ylo = _mm_unpacklo_epi8(zero, ybuf);
			yhi = _mm_unpackhi_epi8(zero, ybuf);
			ylo = _mm_srai_epi16(ylo, 8);
			yhi = _mm_srai_epi16(yhi, 8);

			buf1 = _mm_madd_epi16(xbuf0, ylo);
			buf2 = _mm_madd_epi16(xbuf0, yhi);
			buf3 = _mm_madd_epi16(xbuf1, ylo);
			buf4 = _mm_madd_epi16(xbuf1, yhi);

			buf1 = _mm_and_si128(buf1, mask);
			buf2 = _mm_and_si128(buf2, mask);
			buf3 = _mm_slli_epi32(buf3, 16);
			buf4 = _mm_slli_epi32(buf4, 16);
			buf1 = _mm_or_si128(buf1, buf3);
			buf2 = _mm_or_si128(buf2, buf4);

			_mm_storeu_si128((__m128i *)pz, buf1);
			_mm_storeu_si128((__m128i *)pz + 1, buf2);

			py += nstep;
			pz += nstep;
		}
		for (i = 0; i < n3; i++) {
			pz[0] = x0 * py[0] - x1 * py[1];
			pz[1] = x0 * py[1] + x1 * py[0];
			py += 2;
			pz += 2;
		}
		py += delta;

		for (k = 1; k < m; k++) {
			x0 = px[k * 2 + 0];
			x1 = px[k * 2 + 1];
			xbuf0 = _mm_set1_epi32((x0 & 0xffff) | ((-x1) << 16));
			xbuf1 = _mm_set1_epi32((x0 << 16) | (x1 & 0xffff));
			pz = (mlib_s16 *)z;

			for (i = 0; i < n2; i++) {
				ybuf = _mm_loadu_si128((__m128i *)py);
				zlo = _mm_loadu_si128((__m128i *)pz);
				zhi = _mm_loadu_si128((__m128i *)pz + 1);
				ylo = _mm_unpacklo_epi8(zero, ybuf);
				yhi = _mm_unpackhi_epi8(zero, ybuf);
				ylo = _mm_srai_epi16(ylo, 8);
				yhi = _mm_srai_epi16(yhi, 8);

				buf1 = _mm_madd_epi16(xbuf0, ylo);
				buf2 = _mm_madd_epi16(xbuf0, yhi);
				buf3 = _mm_madd_epi16(xbuf1, ylo);
				buf4 = _mm_madd_epi16(xbuf1, yhi);

				buf1 = _mm_and_si128(buf1, mask);
				buf2 = _mm_and_si128(buf2, mask);
				buf3 = _mm_slli_epi32(buf3, 16);
				buf4 = _mm_slli_epi32(buf4, 16);
				buf1 = _mm_or_si128(buf1, buf3);
				buf2 = _mm_or_si128(buf2, buf4);

				zlo = _mm_add_epi16(zlo, buf1);
				zhi = _mm_add_epi16(zhi, buf2);
				_mm_storeu_si128((__m128i *)pz, zlo);
				_mm_storeu_si128((__m128i *)pz + 1, zhi);

				py += nstep;
				pz += nstep;
			}
			for (i = 0; i < n3; i++) {
				pz[0] += x0 * py[0] - x1 * py[1];
				pz[1] += x0 * py[1] + x1 * py[0];
				py += 2;
				pz += 2;
			}
			py += delta;
		}
	}

	return (MLIB_SUCCESS);
}

/* ****************************************************************** */

mlib_status
mlib_VectorMulM_S_S16C_S16C_Mod(
	mlib_s16 *z,
	const mlib_s16 *x,
	const mlib_s16 *y,
	mlib_s32 m,
	mlib_s32 n,
	mlib_s32 nstride)
{
	if (m < 1 || n < 1)
		return (MLIB_FAILURE);

	mlib_s32 i, k, sum0, sum1, nstep, n1, n2, n3, x0, x1;
	mlib_s16 *px, *py;
	mlib_s16 *pz;
	__m128i xbuf00, xbuf01, xbuf1, ybuf, zbuf, zero, mask;
	__m128i buf1, buf2, buf3, buf4;

	nstep = 16 / sizeof (mlib_s16);
	n2 = (n + n) / nstep;
	n3 = (n + n - n2 * nstep) / 2;

	if (n2 < 1) {
		pz = (mlib_s16 *)z;
		for (i = 0; i < n; i++) {
			sum0 = 0;
			sum1 = 0;
			px = (mlib_s16 *)x;
			py = (mlib_s16 *)y + i * 2;
			for (k = 0; k < m; k++) {
				sum0 += (mlib_s32)px[0] * py[0]
					- (mlib_s32)px[1] * py[1];
				sum1 += (mlib_s32)px[0] * py[1]
					+ (mlib_s32)px[1] * py[0];
				px += 2;
				py += nstride * 2;
			}
			pz[0] = sum0;
			pz[1] = sum1;
			pz += 2;
		}
	} else {
		mlib_s32 delta = (nstride - n) * 2;
		px = (mlib_s16 *)x;
		py = (mlib_s16 *)y;
		zero = _mm_setzero_si128();
		mask = _mm_set1_epi32(0xffff);

		x0 = px[0];
		x1 = px[1];
		xbuf00 = _mm_set1_epi32(x0 & 0xffff);
		xbuf01 = _mm_set1_epi32(x1 << 16);
		xbuf1 = _mm_set1_epi32((x0 << 16) | (x1 & 0xffff));
		pz = (mlib_s16 *)z;

		for (i = 0; i < n2; i++) {
			ybuf = _mm_loadu_si128((__m128i *)py);

			buf1 = _mm_madd_epi16(xbuf00, ybuf);
			buf2 = _mm_madd_epi16(xbuf01, ybuf);
			buf3 = _mm_madd_epi16(xbuf1, ybuf);
			buf1 = _mm_sub_epi32(buf1, buf2);

			buf1 = _mm_and_si128(buf1, mask);
			buf3 = _mm_slli_epi32(buf3, 16);
			buf1 = _mm_or_si128(buf1, buf3);

			_mm_storeu_si128((__m128i *)pz, buf1);

			py += nstep;
			pz += nstep;
		}
		for (i = 0; i < n3; i++) {
			pz[0] = x0 * py[0] - x1 * py[1];
			pz[1] = x0 * py[1] + x1 * py[0];
			py += 2;
			pz += 2;
		}
		py += delta;

		for (k = 1; k < m; k++) {
			x0 = px[k * 2 + 0];
			x1 = px[k * 2 + 1];
			xbuf00 = _mm_set1_epi32(x0 & 0xffff);
			xbuf01 = _mm_set1_epi32(x1 << 16);
			xbuf1 = _mm_set1_epi32((x0 << 16) | (x1 & 0xffff));
			pz = (mlib_s16 *)z;

			for (i = 0; i < n2; i++) {
				ybuf = _mm_loadu_si128((__m128i *)py);
				zbuf = _mm_loadu_si128((__m128i *)pz);

				buf1 = _mm_madd_epi16(xbuf00, ybuf);
				buf2 = _mm_madd_epi16(xbuf01, ybuf);
				buf3 = _mm_madd_epi16(xbuf1, ybuf);
				buf1 = _mm_sub_epi32(buf1, buf2);

				buf1 = _mm_and_si128(buf1, mask);
				buf3 = _mm_slli_epi32(buf3, 16);
				buf1 = _mm_or_si128(buf1, buf3);

				zbuf = _mm_add_epi16(zbuf, buf1);
				_mm_storeu_si128((__m128i *)pz, zbuf);

				py += nstep;
				pz += nstep;
			}
			for (i = 0; i < n3; i++) {
				pz[0] += x0 * py[0] - x1 * py[1];
				pz[1] += x0 * py[1] + x1 * py[0];
				py += 2;
				pz += 2;
			}
			py += delta;
		}
	}

	return (MLIB_SUCCESS);
}

/* ****************************************************************** */

mlib_status
mlib_VectorMulM_S_S32C_S16C_Mod(
	mlib_s32 *z,
	const mlib_s16 *x,
	const mlib_s16 *y,
	mlib_s32 m,
	mlib_s32 n,
	mlib_s32 nstride)
{
	if (m < 1 || n < 1)
		return (MLIB_FAILURE);

	mlib_s32 i, k, sum0, sum1, nstep, n1, n2, n3, x0, x1;
	mlib_s16 *px, *py;
	mlib_s32 *pz;
	__m128i xbuf00, xbuf01, xbuf1, ybuf, zlo, zhi, zero, mask;
	__m128i buf1, buf2, buf3, buf4;

	nstep = 16 / sizeof (mlib_s16);
	n2 = (n + n) / nstep;
	n3 = (n + n - n2 * nstep) / 2;

	if (n2 < 1) {
		pz = (mlib_s32 *)z;
		for (i = 0; i < n; i++) {
			sum0 = 0;
			sum1 = 0;
			px = (mlib_s16 *)x;
			py = (mlib_s16 *)y + i * 2;
			for (k = 0; k < m; k++) {
				sum0 += (mlib_s32)px[0] * py[0]
					- (mlib_s32)px[1] * py[1];
				sum1 += (mlib_s32)px[0] * py[1]
					+ (mlib_s32)px[1] * py[0];
				px += 2;
				py += nstride * 2;
			}
			pz[0] = sum0;
			pz[1] = sum1;
			pz += 2;
		}
	} else {
		mlib_s32 delta = (nstride - n) * 2;
		px = (mlib_s16 *)x;
		py = (mlib_s16 *)y;
		zero = _mm_setzero_si128();
		mask = _mm_set1_epi32(0xffff);

		x0 = px[0];
		x1 = px[1];
		xbuf00 = _mm_set1_epi32(x0 & 0xffff);
		xbuf01 = _mm_set1_epi32(x1 << 16);
		xbuf1 = _mm_set1_epi32((x0 << 16) | (x1 & 0xffff));
		pz = (mlib_s32 *)z;

		for (i = 0; i < n2; i++) {
			ybuf = _mm_loadu_si128((__m128i *)py);

			buf1 = _mm_madd_epi16(xbuf00, ybuf);
			buf2 = _mm_madd_epi16(xbuf01, ybuf);
			buf3 = _mm_madd_epi16(xbuf1, ybuf);
			buf1 = _mm_sub_epi32(buf1, buf2);

			buf2 = _mm_unpacklo_epi32(buf1, buf3);
			buf4 = _mm_unpackhi_epi32(buf1, buf3);

			_mm_storeu_si128((__m128i *)pz, buf2);
			_mm_storeu_si128((__m128i *)pz + 1, buf4);

			py += nstep;
			pz += nstep;
		}
		for (i = 0; i < n3; i++) {
			pz[0] = x0 * py[0] - x1 * py[1];
			pz[1] = x0 * py[1] + x1 * py[0];
			py += 2;
			pz += 2;
		}
		py += delta;

		for (k = 1; k < m; k++) {
			x0 = px[k * 2 + 0];
			x1 = px[k * 2 + 1];
			xbuf00 = _mm_set1_epi32(x0 & 0xffff);
			xbuf01 = _mm_set1_epi32(x1 << 16);
			xbuf1 = _mm_set1_epi32((x0 << 16) | (x1 & 0xffff));
			pz = (mlib_s32 *)z;

			for (i = 0; i < n2; i++) {
				ybuf = _mm_loadu_si128((__m128i *)py);
				zlo = _mm_loadu_si128((__m128i *)pz);
				zhi = _mm_loadu_si128((__m128i *)pz + 1);

				buf1 = _mm_madd_epi16(xbuf00, ybuf);
				buf2 = _mm_madd_epi16(xbuf01, ybuf);
				buf3 = _mm_madd_epi16(xbuf1, ybuf);
				buf1 = _mm_sub_epi32(buf1, buf2);

				buf2 = _mm_unpacklo_epi32(buf1, buf3);
				buf4 = _mm_unpackhi_epi32(buf1, buf3);

				zlo = _mm_add_epi32(zlo, buf2);
				zhi = _mm_add_epi32(zhi, buf4);

				_mm_storeu_si128((__m128i *)pz, zlo);
				_mm_storeu_si128((__m128i *)pz + 1, zhi);

				py += nstep;
				pz += nstep;
			}
			for (i = 0; i < n3; i++) {
				pz[0] += x0 * py[0] - x1 * py[1];
				pz[1] += x0 * py[1] + x1 * py[0];
				py += 2;
				pz += 2;
			}
			py += delta;
		}
	}

	return (MLIB_SUCCESS);
}

/* ****************************************************************** */

mlib_status
__mlib_VectorMulM_U8C_U8C_Mod(
	mlib_u8 *z,
	const mlib_u8 *x,
	const mlib_u8 *y,
	mlib_s32 m,
	mlib_s32 n)
{
	return (mlib_VectorMulM_S_U8C_U8C_Mod(z, x, y, m, n, n));
}

mlib_status
__mlib_VectorMulM_S8C_S8C_Mod(
	mlib_s8 *z,
	const mlib_s8 *x,
	const mlib_s8 *y,
	mlib_s32 m,
	mlib_s32 n)
{
	return (mlib_VectorMulM_S_S8C_S8C_Mod(z, x, y, m, n, n));
}

mlib_status
__mlib_VectorMulM_S16C_U8C_Mod(
	mlib_s16 *z,
	const mlib_u8 *x,
	const mlib_u8 *y,
	mlib_s32 m,
	mlib_s32 n)
{
	return (mlib_VectorMulM_S_S16C_U8C_Mod(z, x, y, m, n, n));
}

mlib_status
__mlib_VectorMulM_S16C_S8C_Mod(
	mlib_s16 *z,
	const mlib_s8 *x,
	const mlib_s8 *y,
	mlib_s32 m,
	mlib_s32 n)
{
	return (mlib_VectorMulM_S_S16C_S8C_Mod(z, x, y, m, n, n));
}

mlib_status
__mlib_VectorMulM_S16C_S16C_Mod(
	mlib_s16 *z,
	const mlib_s16 *x,
	const mlib_s16 *y,
	mlib_s32 m,
	mlib_s32 n)
{
	return (mlib_VectorMulM_S_S16C_S16C_Mod(z, x, y, m, n, n));
}

mlib_status
__mlib_VectorMulM_S32C_S16C_Mod(
	mlib_s32 *z,
	const mlib_s16 *x,
	const mlib_s16 *y,
	mlib_s32 m,
	mlib_s32 n)
{
	return (mlib_VectorMulM_S_S32C_S16C_Mod(z, x, y, m, n, n));
}

/* ****************************************************************** */

	VECTORC_MULM_FP(U8, U8, Sat)
	VECTORC_MULM_FP(S8, S8, Sat)
	VECTORC_MULM_FP(S16, U8, Sat)
	VECTORC_MULM_FP(S16, S8, Sat)
	VECTORC_MULM_FP(S16, S16, Sat)
	VECTORC_MULM_FP(S32, S16, Sat)

/* ****************************************************************** */
