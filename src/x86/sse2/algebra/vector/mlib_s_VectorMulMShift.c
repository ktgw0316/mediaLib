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

#pragma ident	"@(#)mlib_s_VectorMulMShift.c	9.5	07/11/05 SMI"

/*
 * FUNCTIONS
 *     mlib_VectorMulMShift_[S16_S16|S16C_S16C]_[Sat|Mod] -
 *          Multiplication of a signed 16-bit vector by a matrix with shifting
 *
 * SYNOPSIS
 *     mlib_status mlib_VectorMulMShift_S16_S16_Sat(mlib_s16       *z,
 *                                                  const mlib_s16 *x,
 *                                                  const mlib_s16 *y,
 *                                                  mlib_s32       m,
 *                                                  mlib_s32       n,
 *                                                  mlib_s32       shift)
 *     mlib_status mlib_VectorMulMShift_S16_S16_Mod(mlib_s16       *z,
 *                                                  const mlib_s16 *x,
 *                                                  const mlib_s16 *y,
 *                                                  mlib_s32       m,
 *                                                  mlib_s32       n,
 *                                                  mlib_s32       shift)
 *     mlib_status mlib_VectorMulMShift_S16C_S16C_Sat(mlib_s16       *z,
 *                                                    const mlib_s16 *x,
 *                                                    const mlib_s16 *y,
 *                                                    mlib_s32       m,
 *                                                    mlib_s32       n,
 *                                                    mlib_s32       shift)
 *     mlib_status mlib_VectorMulMShift_S16C_S16C_Mod(mlib_s16       *z,
 *                                                    const mlib_s16 *x,
 *                                                    const mlib_s16 *y,
 *                                                    mlib_s32       m,
 *                                                    mlib_s32       n,
 *                                                    mlib_s32       shift)
 *
 * ARGUMENTS
 *     z          the pointer to the first element of the result vector
 *     x          the pointer to the first element of the input vector
 *     y          the pointer to the first element of the input matrix in
 *                row - major order
 *     m          the number of rows in the matrix, and number of elements
 *                in the input vector
 *     n          the number of columns in the matrix, and number of elements
 *                in the result vector
 *     shift      the right shifting factor
 *
 * DESCRIPTION
 *                  m
 *          z[i] = SUM (x[k] * y[k][i]) * 2^(-shift)
 *                 k = 1
 */

#include <mlib_algebra.h>
#include <mlib_AlgebraUtil.h>
#include <mlib_VectorMulM_proto.h>

#ifdef __SUNPRO_C
#include <sunmedia_intrin.h>
#else
#include <emmintrin.h>
#endif

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_VectorMulMShift_S16_S16_Mod = \
	__mlib_VectorMulMShift_S16_S16_Mod
#pragma weak mlib_VectorMulMShift_S16_S16_Sat = \
	__mlib_VectorMulMShift_S16_S16_Sat
#pragma weak mlib_VectorMulMShift_S16C_S16C_Mod = \
	__mlib_VectorMulMShift_S16C_S16C_Mod
#pragma weak mlib_VectorMulMShift_S16C_S16C_Sat = \
	__mlib_VectorMulMShift_S16C_S16C_Sat

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */

__typeof__(__mlib_VectorMulMShift_S16_S16_Mod) mlib_VectorMulMShift_S16_S16_Mod
	__attribute__((weak, alias("__mlib_VectorMulMShift_S16_S16_Mod")));
__typeof__(__mlib_VectorMulMShift_S16_S16_Sat) mlib_VectorMulMShift_S16_S16_Sat
	__attribute__((weak, alias("__mlib_VectorMulMShift_S16_S16_Sat")));
__typeof__(__mlib_VectorMulMShift_S16C_S16C_Mod)
	mlib_VectorMulMShift_S16C_S16C_Mod
	__attribute__((weak, alias("__mlib_VectorMulMShift_S16C_S16C_Mod")));
__typeof__(__mlib_VectorMulMShift_S16C_S16C_Sat)
	mlib_VectorMulMShift_S16C_S16C_Sat
	__attribute__((weak, alias("__mlib_VectorMulMShift_S16C_S16C_Sat")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#ifndef MLIB_USE_FTOI_CLAMPING

#define	shift_round_Sat(out, in, divisor)                       \
	{                                                       \
	    mlib_d64 val = in * divisor;                        \
	                                                        \
	    if (val > MLIB_S32_MAX)                             \
		out = MLIB_S16_MAX;                             \
	    else if (val < MLIB_S32_MIN)                        \
		out = MLIB_S16_MIN;                             \
	    else                                                \
		out = ((mlib_s32)val) >> 16;                    \
	}

#else /* MLIB_USE_FTOI_CLAMPING */

#define	shift_round_Sat(out, in, divisor)                       \
	out = ((mlib_s32)(in * divisor)) >> 16

#endif /* MLIB_USE_FTOI_CLAMPING */

#define	shift_round_Mod(out, in, shift)                         \
	out = mlib_D64_to_S32_Mod(in) >> shift

/* *********************************************************** */

mlib_status
mlib_VectorMulMShift_S_S16_S16_Sat(
	mlib_s16 *z,
	const mlib_s16 *x,
	const mlib_s16 *y,
	mlib_s32 m,
	mlib_s32 n,
	mlib_s32 ns,
	mlib_s32 shift)
{
	mlib_s32 i, j;
	mlib_d64 divisor = 1 << (16 - shift);

	if (m <= 0 || n <= 0)
		return (MLIB_FAILURE);

	if (shift < 1 || shift > 16)
		return (MLIB_OUTOFRANGE);

	for (i = 0; i <= (n - 4); i += 4) {
		mlib_d64 dz0 = 0, dz1 = 0, dz2 = 0, dz3 = 0;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (j = 0; j < m; j++) {
			mlib_d64 dxj = (mlib_d64)x[j];

			dz0 += dxj * (mlib_d64)y[j * ns + i];
			dz1 += dxj * (mlib_d64)y[j * ns + i + 1];
			dz2 += dxj * (mlib_d64)y[j * ns + i + 2];
			dz3 += dxj * (mlib_d64)y[j * ns + i + 3];
		}

		shift_round_Sat(z[i], dz0, divisor);
		shift_round_Sat(z[i + 1], dz1, divisor);
		shift_round_Sat(z[i + 2], dz2, divisor);
		shift_round_Sat(z[i + 3], dz3, divisor);
	}

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (; i < n; i++) {
		mlib_d64 dz = 0;

		for (j = 0; j <= (m - 4); j += 4) {
			dz += ((mlib_d64)x[j]) * ((mlib_d64)y[j * ns + i]);
			dz += ((mlib_d64)x[j + 1]) * ((mlib_d64)y[(j + 1) * ns +
				i]);
			dz += ((mlib_d64)x[j + 2]) * ((mlib_d64)y[(j + 2) * ns +
				i]);
			dz += ((mlib_d64)x[j + 3]) * ((mlib_d64)y[(j + 3) * ns +
				i]);
		}

		for (; j < m; j++)
			dz += (mlib_d64)x[j] * (mlib_d64)y[j * ns + i];

		shift_round_Sat(z[i], dz, divisor);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_VectorMulMShift_S_S16C_S16C_Sat(
	mlib_s16 *z,
	const mlib_s16 *x,
	const mlib_s16 *y,
	mlib_s32 m,
	mlib_s32 n,
	mlib_s32 ns,
	mlib_s32 shift)
{
	mlib_s32 i, j;
	mlib_d64 divisor = 1 << (16 - shift);
	mlib_s32 ns2;

	if (m <= 0 || n <= 0)
		return (MLIB_FAILURE);

	if (shift < 1 || shift > 16)
		return (MLIB_OUTOFRANGE);

	ns2 = ns << 1;
	for (i = 0; i <= (n - 4); i += 4) {
		mlib_d64 dz0_r = 0, dz0_i = 0, dz1_r = 0, dz1_i = 0;
		mlib_d64 dz2_r = 0, dz2_i = 0, dz3_r = 0, dz3_i = 0;
		const mlib_s16 *addr = y + (i << 1);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (j = 0; j < m; j++) {
			mlib_d64 dxj_r = (mlib_d64)x[j * 2];
			mlib_d64 dxj_i = (mlib_d64)x[j * 2 + 1];

			mlib_d64 dy0_r = (mlib_d64)addr[0];
			mlib_d64 dy0_i = (mlib_d64)addr[1];
			mlib_d64 dy1_r = (mlib_d64)addr[2];
			mlib_d64 dy1_i = (mlib_d64)addr[3];

			mlib_d64 dy2_r = (mlib_d64)addr[4];
			mlib_d64 dy2_i = (mlib_d64)addr[5];
			mlib_d64 dy3_r = (mlib_d64)addr[6];
			mlib_d64 dy3_i = (mlib_d64)addr[7];

			addr += ns2;

			dz0_r += dxj_r * dy0_r - dxj_i * dy0_i;
			dz0_i += dxj_r * dy0_i + dxj_i * dy0_r;
			dz1_r += dxj_r * dy1_r - dxj_i * dy1_i;
			dz1_i += dxj_r * dy1_i + dxj_i * dy1_r;
			dz2_r += dxj_r * dy2_r - dxj_i * dy2_i;
			dz2_i += dxj_r * dy2_i + dxj_i * dy2_r;
			dz3_r += dxj_r * dy3_r - dxj_i * dy3_i;
			dz3_i += dxj_r * dy3_i + dxj_i * dy3_r;
		}

		shift_round_Sat(z[i * 2 + 0], dz0_r, divisor);
		shift_round_Sat(z[i * 2 + 1], dz0_i, divisor);
		shift_round_Sat(z[i * 2 + 2], dz1_r, divisor);
		shift_round_Sat(z[i * 2 + 3], dz1_i, divisor);
		shift_round_Sat(z[i * 2 + 4], dz2_r, divisor);
		shift_round_Sat(z[i * 2 + 5], dz2_i, divisor);
		shift_round_Sat(z[i * 2 + 6], dz3_r, divisor);
		shift_round_Sat(z[i * 2 + 7], dz3_i, divisor);
	}

	for (; i < n; i++) {
		mlib_d64 dz_r = 0, dz_i = 0;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (j = 0; j < m; j++) {
			mlib_s32 a = (j * ns + i) * 2;

			dz_r += ((mlib_d64)x[j * 2]) * ((mlib_d64)y[a]) -
				((mlib_d64)x[j * 2 + 1]) * ((mlib_d64)y[a + 1]);
			dz_i += ((mlib_d64)x[j * 2]) * ((mlib_d64)y[a + 1]) +
				((mlib_d64)x[j * 2 + 1]) * ((mlib_d64)y[a]);
		}

		shift_round_Sat(z[i * 2], dz_r, divisor);
		shift_round_Sat(z[i * 2 + 1], dz_i, divisor);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VectorMulMShift_S16_S16_Sat(
	mlib_s16 *z,
	const mlib_s16 *x,
	const mlib_s16 *y,
	mlib_s32 m,
	mlib_s32 n,
	mlib_s32 shift)
{
	return mlib_VectorMulMShift_S_S16_S16_Sat(z,
		x, y, m, n, n, shift);
}

mlib_status
__mlib_VectorMulMShift_S16C_S16C_Sat(
	mlib_s16 *z,
	const mlib_s16 *x,
	const mlib_s16 *y,
	mlib_s32 m,
	mlib_s32 n,
	mlib_s32 shift)
{
	return mlib_VectorMulMShift_S_S16C_S16C_Sat(z,
		x, y, m, n, n, shift);
}

/* *********************************************************** */

mlib_status
mlib_VectorMulMShift_S_S16_S16_Mod(
	mlib_s16 *z,
	const mlib_s16 *x,
	const mlib_s16 *y,
	mlib_s32 m,
	mlib_s32 n,
	mlib_s32 ns,
	mlib_s32 shift)
{
	if (m < 1 || n < 1)
		return (MLIB_FAILURE);
	if (shift < 1 || shift > 16)
		return (MLIB_OUTOFRANGE);

	mlib_s32 i, k, sum, nstep, n1, n2, n3, xk;
	mlib_s16 *px, *py, *pz;
	__m128i xbuf, ybuf, zbuf, xhi, xlo, yhi, ylo, zlo, zhi, zero, mask;
	mask = _mm_set1_epi32(0xffff);

	nstep = 16 / sizeof (mlib_s16);

	if (n == 1 && ns == 1) {
		px = (mlib_s16 *)x;
		py = (mlib_s16 *)y;
		sum = 0;
		zlo = _mm_setzero_si128();
		zhi = zlo;

		for (k = 0; k < m / nstep; k++) {
			xbuf = _mm_loadu_si128((__m128i *)px);
			ybuf = _mm_loadu_si128((__m128i *)py);

			xlo = _mm_mullo_epi16(xbuf, ybuf);
			xhi = _mm_mulhi_epi16(xbuf, ybuf);
			ylo = _mm_unpacklo_epi16(xlo, xhi);
			yhi = _mm_unpackhi_epi16(xlo, xhi);

			zlo = _mm_add_epi32(zlo, ylo);
			zhi = _mm_add_epi32(zhi, yhi);

			px += nstep;
			py += nstep;
		}
		if (m >= nstep) {
			mlib_s32 tmp[4];
			zlo = _mm_add_epi32(zlo, zhi);
			_mm_storeu_si128((__m128i *)tmp, zlo);
			for (k = 0; k < 4; k++)
				sum += tmp[k];
		}
		for (k = 0; k < m % nstep; k++) {
			sum += (mlib_s32)(*px) * (*py);
			px++;
			py++;
		}
		*z = sum >> shift;

	} else if (n < nstep) {
		for (i = 0; i < n; i++) {
			sum = 0;
			px = (mlib_s16 *)x;
			py = (mlib_s16 *)y + i;
			for (k = 0; k < m; k++) {
				sum += (mlib_s32)(*px++) * (*py);
				py += ns;
			}
			z[i] = sum >> shift;
		}
	} else {
		mlib_s32 delta = ns - n;
		mlib_s32 *tmpz, *ptz;
		tmpz = (mlib_s32 *)__mlib_malloc(n * 4);

		n2 = n / nstep;
		n3 = n % nstep;
		px = (mlib_s16 *)x;
		py = (mlib_s16 *)y;

		xk = px[0];
		ptz = (mlib_s32 *)tmpz;
		xbuf = _mm_set1_epi16(xk);

		for (i = 0; i < n2; i++) {
			ybuf = _mm_loadu_si128((__m128i *)py);

			xlo = _mm_mullo_epi16(xbuf, ybuf);
			xhi = _mm_mulhi_epi16(xbuf, ybuf);
			ylo = _mm_unpacklo_epi16(xlo, xhi);
			yhi = _mm_unpackhi_epi16(xlo, xhi);

			_mm_store_si128((__m128i *)ptz, ylo);
			_mm_store_si128((__m128i *)ptz + 1, yhi);

			py += nstep;
			ptz += nstep;
		}
		for (i = 0; i < n3; i++) {
			(*ptz) = xk * (*py);
			py++;
			ptz++;
		}
		py += delta;

		for (k = 1; k < m; k++) {
			xk = px[k];
			ptz = (mlib_s32 *)tmpz;
			xbuf = _mm_set1_epi16(xk);

			for (i = 0; i < n2; i++) {
				ybuf = _mm_loadu_si128((__m128i *)py);
				zlo = _mm_load_si128((__m128i *)ptz);
				zhi = _mm_load_si128((__m128i *)ptz + 1);

				xlo = _mm_mullo_epi16(xbuf, ybuf);
				xhi = _mm_mulhi_epi16(xbuf, ybuf);
				ylo = _mm_unpacklo_epi16(xlo, xhi);
				yhi = _mm_unpackhi_epi16(xlo, xhi);
				zlo = _mm_add_epi32(zlo, ylo);
				zhi = _mm_add_epi32(zhi, yhi);

				_mm_store_si128((__m128i *)ptz, zlo);
				_mm_store_si128((__m128i *)ptz + 1, zhi);

				py += nstep;
				ptz += nstep;
			}

			for (i = 0; i < n3; i++) {
				(*ptz) += xk * (*py);
				py++;
				ptz++;
			}
			py += delta;
		}

		pz = (mlib_s16 *)z;
		ptz = (mlib_s32 *)tmpz;

		for (i = 0; i < n2; i++) {
			zlo = _mm_load_si128((__m128i *)ptz);
			zhi = _mm_load_si128((__m128i *)ptz + 1);
			zlo = _mm_srli_epi32(zlo, shift);
			zhi = _mm_srli_epi32(zhi, shift);

			xlo = _mm_unpacklo_epi32(zlo, zhi);
			xhi = _mm_unpackhi_epi32(zlo, zhi);
			zlo = _mm_unpacklo_epi32(xlo, xhi);
			zhi = _mm_unpackhi_epi32(xlo, xhi);

			zlo = _mm_and_si128(zlo, mask);
			zhi = _mm_slli_epi32(zhi, 16);
			zlo = _mm_or_si128(zlo, zhi);

			_mm_storeu_si128((__m128i *)pz, zlo);
			pz += nstep;
			ptz += nstep;
		}
		for (i = 0; i < n3; i++) {
			(*pz++) = (*ptz++) >> shift;
		}
		__mlib_free(tmpz);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_VectorMulMShift_S_S16C_S16C_Mod(
	mlib_s16 *z,
	const mlib_s16 *x,
	const mlib_s16 *y,
	mlib_s32 m,
	mlib_s32 n,
	mlib_s32 ns,
	mlib_s32 shift)
{
	if (m < 1 || n < 1)
		return (MLIB_FAILURE);
	if (shift < 1 || shift > 16)
		return (MLIB_OUTOFRANGE);

	mlib_s32 i, k, sum0, sum1, nstep, n1, n2, n3, x0, x1;
	mlib_s16 *px, *py;
	mlib_s16 *pz;
	__m128i xbuf00, xbuf01, xbuf1, ybuf, zbuf, zero, mask;
	__m128i buf1, buf2, buf3, buf4, xlo, xhi, ylo, yhi, zlo, zhi;

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
				py += ns * 2;
			}
			pz[0] = sum0 >> shift;
			pz[1] = sum1 >> shift;
			pz += 2;
		}
	} else {
		mlib_s32 delta = (ns - n) * 2;
		mlib_s32 *tmpz, *ptz;
		tmpz = (mlib_s32 *)__mlib_malloc(n * 8);

		px = (mlib_s16 *)x;
		py = (mlib_s16 *)y;
		zero = _mm_setzero_si128();
		mask = _mm_set1_epi32(0xffff);

		x0 = px[0];
		x1 = px[1];
		xbuf00 = _mm_set1_epi32(x0 & 0xffff);
		xbuf01 = _mm_set1_epi32(x1 << 16);
		xbuf1 = _mm_set1_epi32((x0 << 16) | (x1 & 0xffff));
		ptz = (mlib_s32 *)tmpz;

		for (i = 0; i < n2; i++) {
			ybuf = _mm_loadu_si128((__m128i *)py);

			buf1 = _mm_madd_epi16(xbuf00, ybuf);
			buf2 = _mm_madd_epi16(xbuf01, ybuf);
			buf1 = _mm_sub_epi32(buf1, buf2);
			buf3 = _mm_madd_epi16(xbuf1, ybuf);

			_mm_store_si128((__m128i *)ptz, buf1);
			_mm_store_si128((__m128i *)ptz + 1, buf3);

			py += nstep;
			ptz += nstep;
		}
		for (i = 0; i < n3; i++) {
			ptz[0] = x0 * py[0] - x1 * py[1];
			ptz[1] = x0 * py[1] + x1 * py[0];
			py += 2;
			ptz += 2;
		}
		py += delta;

		for (k = 1; k < m; k++) {
			x0 = px[k * 2 + 0];
			x1 = px[k * 2 + 1];
			xbuf00 = _mm_set1_epi32(x0 & 0xffff);
			xbuf01 = _mm_set1_epi32(x1 << 16);
			xbuf1 = _mm_set1_epi32((x0 << 16) | (x1 & 0xffff));
			ptz = (mlib_s32 *)tmpz;

			for (i = 0; i < n2; i++) {
				ybuf = _mm_loadu_si128((__m128i *)py);
				zlo = _mm_loadu_si128((__m128i *)ptz);
				zhi = _mm_loadu_si128((__m128i *)ptz + 1);

				buf1 = _mm_madd_epi16(xbuf00, ybuf);
				buf2 = _mm_madd_epi16(xbuf01, ybuf);
				buf1 = _mm_sub_epi32(buf1, buf2);
				buf3 = _mm_madd_epi16(xbuf1, ybuf);

				zlo = _mm_add_epi32(zlo, buf1);
				zhi = _mm_add_epi32(zhi, buf3);
				_mm_store_si128((__m128i *)ptz, zlo);
				_mm_store_si128((__m128i *)ptz + 1, zhi);

				py += nstep;
				ptz += nstep;
			}
			for (i = 0; i < n3; i++) {
				ptz[0] += x0 * py[0] - x1 * py[1];
				ptz[1] += x0 * py[1] + x1 * py[0];
				py += 2;
				ptz += 2;
			}
			py += delta;
		}

		pz = (mlib_s16 *)z;
		ptz = (mlib_s32 *)tmpz;

		for (i = 0; i < n2; i++) {
			zlo = _mm_load_si128((__m128i *)ptz);
			zhi = _mm_load_si128((__m128i *)ptz + 1);
			zlo = _mm_srli_epi32(zlo, shift);
			zhi = _mm_srli_epi32(zhi, shift);

			zlo = _mm_and_si128(zlo, mask);
			zhi = _mm_slli_epi32(zhi, 16);
			zlo = _mm_or_si128(zlo, zhi);

			_mm_storeu_si128((__m128i *)pz, zlo);
			pz += nstep;
			ptz += nstep;
		}
		for (i = 0; i < n3; i++) {
			pz[0] = ptz[0] >> shift;
			pz[1] = ptz[1] >> shift;
			pz += 2;
			ptz += 2;
		}
		__mlib_free(tmpz);
	}

	return (MLIB_SUCCESS);
}

/* ****************************************************************** */

mlib_status
__mlib_VectorMulMShift_S16_S16_Mod(
	mlib_s16 *z,
	const mlib_s16 *x,
	const mlib_s16 *y,
	mlib_s32 m,
	mlib_s32 n,
	mlib_s32 shift)
{
	return mlib_VectorMulMShift_S_S16_S16_Mod(z,
		x, y, m, n, n, shift);
}

mlib_status
__mlib_VectorMulMShift_S16C_S16C_Mod(
	mlib_s16 *z,
	const mlib_s16 *x,
	const mlib_s16 *y,
	mlib_s32 m,
	mlib_s32 n,
	mlib_s32 shift)
{
	return mlib_VectorMulMShift_S_S16C_S16C_Mod(z,
		x, y, m, n, n, shift);
}

/* *********************************************************** */
