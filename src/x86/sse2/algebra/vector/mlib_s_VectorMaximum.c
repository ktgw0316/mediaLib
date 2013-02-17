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

#pragma ident	"@(#)mlib_s_VectorMaximum.c	9.5	07/11/05 SMI"

/*
 * FUNCTIONS
 *      mlib_VectorMaximum - find the maximum
 *
 * SYNOPSIS
 *      mlib_status mlib_VectorMaximum_U8(mlib_u8       *max,
 *                                        const mlib_u8 *x,
 *                                        mlib_s32      n);
 *      mlib_status mlib_VectorMaximum_S8(mlib_s8       *max,
 *                                        const mlib_s8 *x,
 *                                        mlib_s32      n);
 *      mlib_status mlib_VectorMaximum_S16(mlib_s16       *max,
 *                                         const mlib_s16 *x,
 *                                         mlib_s32       n);
 *      mlib_status mlib_VectorMaximum_S32(mlib_s32       *max,
 *                                         const mlib_s32 *x,
 *                                         mlib_s32       n);
 *
 * ARGUMENTS
 *      max    the pointer to maximum value
 *      x      the pointer to the first element of the source vector
 *      n      the number of elements in the source vector
 *
 * DESCRIPTION
 *            n - 1
 *      max = Max x[i]
 *            i = 0
 */

/*
 * FUNCTIONS
 *      mlib_VectorMaximumMag - find the maximum
 *
 * SYNOPSIS
 *      mlib_status mlib_VectorMaximumMag_U8C(mlib_u8       *max,
 *                                            const mlib_u8 *x,
 *                                            mlib_s32      n);
 *      mlib_status mlib_VectorMaximumMag_S8C(mlib_s8       *max,
 *                                            const mlib_s8 *x,
 *                                            mlib_s32      n);
 *      mlib_status mlib_VectorMaximumMag_S16C(mlib_s16       *max,
 *                                             const mlib_s16 *x,
 *                                             mlib_s32       n);
 *      mlib_status mlib_VectorMaximumMag_S32C(mlib_s32       *max,
 *                                             const mlib_s32 *x,
 *                                             mlib_s32       n);
 *
 * ARGUMENTS
 *      max    the pointer to maximum value
 *      x      the pointer to the first element of the source vector
 *      n      the number of elements in the source vector
 *
 * DESCRIPTION
 *      This function finds the first element with the maximum
 *      magnitude, then puts the real and imaginary parts of it into
 *      max[0] and max[1], respectively.
 *
 *            n - 1
 *      max = Max x[i]
 *            i = 0
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

#pragma weak mlib_VectorMaximum_S8 = __mlib_VectorMaximum_S8
#pragma weak mlib_VectorMaximum_U8 = __mlib_VectorMaximum_U8
#pragma weak mlib_VectorMaximum_S16 = __mlib_VectorMaximum_S16
#pragma weak mlib_VectorMaximum_S32 = __mlib_VectorMaximum_S32
#pragma weak mlib_VectorMaximumMag_S8C = __mlib_VectorMaximumMag_S8C
#pragma weak mlib_VectorMaximumMag_U8C = __mlib_VectorMaximumMag_U8C
#pragma weak mlib_VectorMaximumMag_S16C = __mlib_VectorMaximumMag_S16C
#pragma weak mlib_VectorMaximumMag_S32C = __mlib_VectorMaximumMag_S32C

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */

__typeof__(__mlib_VectorMaximum_S8) mlib_VectorMaximum_S8
	__attribute__((weak, alias("__mlib_VectorMaximum_S8")));
__typeof__(__mlib_VectorMaximum_U8) mlib_VectorMaximum_U8
	__attribute__((weak, alias("__mlib_VectorMaximum_U8")));
__typeof__(__mlib_VectorMaximum_S16) mlib_VectorMaximum_S16
	__attribute__((weak, alias("__mlib_VectorMaximum_S16")));
__typeof__(__mlib_VectorMaximum_S32) mlib_VectorMaximum_S32
	__attribute__((weak, alias("__mlib_VectorMaximum_S32")));
__typeof__(__mlib_VectorMaximumMag_S8C) mlib_VectorMaximumMag_S8C
	__attribute__((weak, alias("__mlib_VectorMaximumMag_S8C")));
__typeof__(__mlib_VectorMaximumMag_U8C) mlib_VectorMaximumMag_U8C
	__attribute__((weak, alias("__mlib_VectorMaximumMag_U8C")));
__typeof__(__mlib_VectorMaximumMag_S16C) mlib_VectorMaximumMag_S16C
	__attribute__((weak, alias("__mlib_VectorMaximumMag_S16C")));
__typeof__(__mlib_VectorMaximumMag_S32C) mlib_VectorMaximumMag_S32C
	__attribute__((weak, alias("__mlib_VectorMaximumMag_S32C")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	MAX_CMP(max, arg)	max = (arg > max) ? arg : max

/* *********************************************************** */

mlib_status
__mlib_VectorMaximum_U8(
	mlib_u8 *max,
	const mlib_u8 *x,
	mlib_s32 n)
{
	if (n < 1)
		return (MLIB_FAILURE);

	const mlib_u8 *px = x;
	mlib_u8 pm[8], smax = MLIB_U8_MIN;
	mlib_s32 ax, nstep, n1, n2, n3, i;
	__m128i mmax, xbuf;

	ax = (mlib_addr)x & 15;
	nstep = 16 / sizeof (mlib_u8);
	n1 = ((16 - ax) & 15) / sizeof (mlib_u8);
	n2 = (n - n1) / nstep;
	n3 = n - n1 - n2 * nstep;

	if (n2 > 1) {
		for (i = 0; i < n1; i++) {
			MAX_CMP(smax, *px);
			px++;
		}

		mmax = _mm_load_si128((__m128i *)px);
		px += nstep;
		for (i = 1; i < n2; i++) {
			xbuf = _mm_load_si128((__m128i *)px);
			mmax = _mm_max_epu8(mmax, xbuf);
			px += nstep;
		}
		xbuf = _mm_unpacklo_epi64(mmax, mmax);
		mmax = _mm_unpackhi_epi64(mmax, mmax);
		mmax = _mm_max_epu8(mmax, xbuf);
		_mm_storel_epi64((__m128i *)pm, mmax);
		for (i = 0; i < 8; i++) {
			MAX_CMP(smax, pm[i]);
		}

		for (i = 0; i < n3; i++) {
			MAX_CMP(smax, *px);
			px++;
		}
	} else {
		for (i = 0; i < n; i++) {
			MAX_CMP(smax, *px);
			px++;
		}
	}

	*max = smax;
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VectorMaximum_S8(
	mlib_s8 *max,
	const mlib_s8 *x,
	mlib_s32 n)
{
	if (n < 1)
		return (MLIB_FAILURE);

	const mlib_s8 *px = x;
	mlib_s8 pm[8], smax = MLIB_S8_MIN;
	mlib_s32 ax, nstep, n1, n2, n3, i;
	__m128i mmax, xbuf, incs;
	incs = _mm_set1_epi8(MLIB_S8_MIN);

	ax = (mlib_addr)x & 15;
	nstep = 16 / sizeof (mlib_s8);
	n1 = ((16 - ax) & 15) / sizeof (mlib_s8);
	n2 = (n - n1) / nstep;
	n3 = n - n1 - n2 * nstep;

	if (n2 > 1) {
		for (i = 0; i < n1; i++) {
			MAX_CMP(smax, *px);
			px++;
		}

		mmax = _mm_load_si128((__m128i *)px);
		mmax = _mm_sub_epi8(mmax, incs);
		px += nstep;
		for (i = 1; i < n2; i++) {
			xbuf = _mm_load_si128((__m128i *)px);
			xbuf = _mm_sub_epi8(xbuf, incs);
			mmax = _mm_max_epu8(mmax, xbuf);
			px += nstep;
		}
		xbuf = _mm_unpacklo_epi64(mmax, mmax);
		mmax = _mm_unpackhi_epi64(mmax, mmax);
		mmax = _mm_max_epu8(mmax, xbuf);
		mmax = _mm_add_epi8(mmax, incs);
		_mm_storel_epi64((__m128i *)pm, mmax);
		for (i = 0; i < 8; i++) {
			MAX_CMP(smax, pm[i]);
		}

		for (i = 0; i < n3; i++) {
			MAX_CMP(smax, *px);
			px++;
		}
	} else {
		for (i = 0; i < n; i++) {
			MAX_CMP(smax, *px);
			px++;
		}
	}

	*max = smax;
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VectorMaximum_S16(
	mlib_s16 *max,
	const mlib_s16 *x,
	mlib_s32 n)
{
	if (n < 1)
		return (MLIB_FAILURE);

	const mlib_s16 *px = x;
	mlib_s16 pm[4], smax = MLIB_S16_MIN;
	mlib_s32 ax, nstep, n1, n2, n3, i;
	__m128i mmax, xbuf;

	ax = (mlib_addr)x & 15;
	nstep = 16 / sizeof (mlib_s16);
	n1 = ((16 - ax) & 15) / sizeof (mlib_s16);
	n2 = (n - n1) / nstep;
	n3 = n - n1 - n2 * nstep;

	if (n2 > 1) {
		for (i = 0; i < n1; i++) {
			MAX_CMP(smax, *px);
			px++;
		}

		mmax = _mm_load_si128((__m128i *)px);
		px += nstep;
		for (i = 1; i < n2; i++) {
			xbuf = _mm_load_si128((__m128i *)px);
			mmax = _mm_max_epi16(mmax, xbuf);
			px += nstep;
		}
		xbuf = _mm_unpacklo_epi64(mmax, mmax);
		mmax = _mm_unpackhi_epi64(mmax, mmax);
		mmax = _mm_max_epi16(mmax, xbuf);
		_mm_storel_epi64((__m128i *)pm, mmax);
		for (i = 0; i < 4; i++) {
			MAX_CMP(smax, pm[i]);
		}

		for (i = 0; i < n3; i++) {
			MAX_CMP(smax, *px);
			px++;
		}
	} else {
		for (i = 0; i < n; i++) {
			MAX_CMP(smax, *px);
			px++;
		}
	}

	*max = smax;
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VectorMaximum_S32(
	mlib_s32 *max,
	const mlib_s32 *x,
	mlib_s32 n)
{
	if (n < 1)
		return (MLIB_FAILURE);

	const mlib_s32 *px = x;
	mlib_s32 pm[4], smax = MLIB_S32_MIN;
	mlib_s32 ax, nstep, n1, n2, n3, i;
	__m128i mmax, xbuf, mask;

	ax = (mlib_addr)x & 15;
	nstep = 16 / sizeof (mlib_s32);
	n1 = ((16 - ax) & 15) / sizeof (mlib_s32);
	n2 = (n - n1) / nstep;
	n3 = n - n1 - n2 * nstep;

	if (n2 > 1) {
		for (i = 0; i < n1; i++) {
			MAX_CMP(smax, *px);
			px++;
		}

		mmax = _mm_load_si128((__m128i *)px);
		px += nstep;
		for (i = 1; i < n2; i++) {
			xbuf = _mm_load_si128((__m128i *)px);
			mask = _mm_cmpgt_epi32(xbuf, mmax);
			xbuf = _mm_and_si128(mask, xbuf);
			mmax = _mm_andnot_si128(mask, mmax);
			mmax = _mm_or_si128(mmax, xbuf);
			px += nstep;
		}
		_mm_storeu_si128((__m128i *)pm, mmax);
		for (i = 0; i < 4; i++) {
			MAX_CMP(smax, pm[i]);
		}

		for (i = 0; i < n3; i++) {
			MAX_CMP(smax, *px);
			px++;
		}
	} else {
		for (i = 0; i < n; i++) {
			MAX_CMP(smax, *px);
			px++;
		}
	}

	*max = smax;
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VectorMaximumMag_U8C(
	mlib_u8 *max,
	const mlib_u8 *x,
	mlib_s32 n)
{
	if (n <= 0)
		return (MLIB_FAILURE);

	const mlib_u8 *px = x;
	mlib_s32 i, nstep, n2, n3;
	mlib_u16 val_max;
	mlib_u32 mag, mag_max, ind, ind_max;
	__m128i xbuf, xlo, xhi;
	__m128i mmaxmag, mmaxval, buf1, buf2, buf3, buf4, zero;
	__m128i mmaxind, mind, incs;

	nstep = 16 / sizeof (mlib_u8);
	n2 = n / (nstep / 2);
	n3 = n - n2 * nstep / 2;

	if (n2 > 0) {
		zero = _mm_setzero_si128();
		mind = _mm_setr_epi32(0, 1, 2, 3);
		incs = _mm_set1_epi32(4);

		xbuf = _mm_loadu_si128((__m128i *)px);
		xlo  = _mm_unpacklo_epi8(xbuf, zero);
		xhi  = _mm_unpackhi_epi8(xbuf, zero);
		mmaxval = xlo;
		mmaxmag = _mm_madd_epi16(xlo, xlo);
		mmaxind = mind;

		buf1 = _mm_madd_epi16(xhi, xhi);
		buf2 = _mm_cmpgt_epi32(buf1, mmaxmag);
		buf3 = _mm_and_si128(buf2, xhi);
		buf4 = _mm_and_si128(buf2, buf1);
		mmaxval = _mm_andnot_si128(buf2, mmaxval);
		mmaxmag = _mm_andnot_si128(buf2, mmaxmag);
		mmaxval = _mm_or_si128(mmaxval, buf3);
		mmaxmag = _mm_or_si128(mmaxmag, buf4);

		mind = _mm_add_epi32(mind, incs);
		mmaxind = _mm_or_si128(
			_mm_and_si128(buf2, mind),
			_mm_andnot_si128(buf2, mmaxind));

		px += nstep;

		if (((mlib_addr)px & 15) == 0) {
			for (i = 1; i < n2; i++) {
				xbuf = _mm_load_si128((__m128i *)px);
				xlo  = _mm_unpacklo_epi8(xbuf, zero);
				xhi  = _mm_unpackhi_epi8(xbuf, zero);

				buf1 = _mm_madd_epi16(xlo, xlo);
				buf2 = _mm_cmpgt_epi32(buf1, mmaxmag);
				buf3 = _mm_and_si128(buf2, xlo);
				buf4 = _mm_and_si128(buf2, buf1);
				mmaxval = _mm_andnot_si128(buf2, mmaxval);
				mmaxmag = _mm_andnot_si128(buf2, mmaxmag);
				mmaxval = _mm_or_si128(mmaxval, buf3);
				mmaxmag = _mm_or_si128(mmaxmag, buf4);

				/* save the index */
				mind = _mm_add_epi32(mind, incs);
				mmaxind = _mm_or_si128(
					_mm_and_si128(buf2, mind),
					_mm_andnot_si128(buf2, mmaxind));

				buf1 = _mm_madd_epi16(xhi, xhi);
				buf2 = _mm_cmpgt_epi32(buf1, mmaxmag);
				buf3 = _mm_and_si128(buf2, xhi);
				buf4 = _mm_and_si128(buf2, buf1);
				mmaxval = _mm_andnot_si128(buf2, mmaxval);
				mmaxmag = _mm_andnot_si128(buf2, mmaxmag);
				mmaxval = _mm_or_si128(mmaxval, buf3);
				mmaxmag = _mm_or_si128(mmaxmag, buf4);

				/* save the index */
				mind = _mm_add_epi32(mind, incs);
				mmaxind = _mm_or_si128(
					_mm_and_si128(buf2, mind),
					_mm_andnot_si128(buf2, mmaxind));

				px += nstep;
			}
		} else {
			for (i = 1; i < n2; i++) {
				xbuf = _mm_loadu_si128((__m128i *)px);
				xlo  = _mm_unpacklo_epi8(xbuf, zero);
				xhi  = _mm_unpackhi_epi8(xbuf, zero);

				buf1 = _mm_madd_epi16(xlo, xlo);
				buf2 = _mm_cmpgt_epi32(buf1, mmaxmag);
				buf3 = _mm_and_si128(buf2, xlo);
				buf4 = _mm_and_si128(buf2, buf1);
				mmaxval = _mm_andnot_si128(buf2, mmaxval);
				mmaxmag = _mm_andnot_si128(buf2, mmaxmag);
				mmaxval = _mm_or_si128(mmaxval, buf3);
				mmaxmag = _mm_or_si128(mmaxmag, buf4);

				/* save the index */
				mind = _mm_add_epi32(mind, incs);
				mmaxind = _mm_or_si128(
					_mm_and_si128(buf2, mind),
					_mm_andnot_si128(buf2, mmaxind));

				buf1 = _mm_madd_epi16(xhi, xhi);
				buf2 = _mm_cmpgt_epi32(buf1, mmaxmag);
				buf3 = _mm_and_si128(buf2, xhi);
				buf4 = _mm_and_si128(buf2, buf1);
				mmaxval = _mm_andnot_si128(buf2, mmaxval);
				mmaxmag = _mm_andnot_si128(buf2, mmaxmag);
				mmaxval = _mm_or_si128(mmaxval, buf3);
				mmaxmag = _mm_or_si128(mmaxmag, buf4);

				/* save the index */
				mind = _mm_add_epi32(mind, incs);
				mmaxind = _mm_or_si128(
					_mm_and_si128(buf2, mind),
					_mm_andnot_si128(buf2, mmaxind));

				px += nstep;
			}
		}

		mlib_u32 mags[4], vals[4], inds[4], val_max_32 = 0;
		_mm_storeu_si128((__m128i *)mags, mmaxmag);
		_mm_storeu_si128((__m128i *)vals, mmaxval);
		_mm_storeu_si128((__m128i *)inds, mmaxind);
		val_max_32 = vals[0];
		mag_max = mags[0];
		ind_max = inds[0];

		for (i = 1; i < 4; i++) {
			mag = mags[i];
			ind = inds[i];
			if ((mag > mag_max) ||
				(mag == mag_max && ind < ind_max)) {
				val_max_32 = vals[i];
				ind_max = ind;
				mag_max = mag;
			}
		}
		val_max = (val_max_32 & 0xff) | ((val_max_32 >> 8) & 0xff00);

		for (i = 0; i < n3; i++) {
			mag = (mlib_s32)px[0] * px[0] + (mlib_s32)px[1] * px[1];

			if (mag > mag_max) {
				val_max = *(mlib_u16 *)px;
				mag_max = mag;
			}
			px += 2;
		}
	} else {
		val_max = *(mlib_u16 *)px;
		mag_max = (mlib_s32)px[0] * px[0] + (mlib_s32)px[1] * px[1];
		px += 2;

		for (i = 1; i < n; i++) {
			mag = (mlib_s32)px[0] * px[0] + (mlib_s32)px[1] * px[1];
			if (mag > mag_max) {
				val_max = *(mlib_u16 *)px;
				mag_max = mag;
			}
			px += 2;
		}
	}
	*(mlib_u16 *)max = val_max;

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_s32
mlib_VectorMaximumMag_S8C_part(
	mlib_u16 *max,
	const mlib_s8 *x,
	mlib_s32 n)
{
	const mlib_s8 *px = x;
	mlib_s32 i, nstep, n2, n3;
	mlib_u16 val_max, ind_max, ind;
	mlib_u32 mag, mag_max;
	__m128i xbuf, mmaxmag, mmaxval, mmaxind, mind;
	__m128i buf1, buf2, zero, mask, base, incs;

	val_max = 0;
	mag_max = 0;

	nstep = 16 / sizeof (mlib_s8);
	n2 = n / (nstep / 2);
	n3 = n - n2 * nstep / 2;

	if (n2 > 1) {
		base = _mm_set1_epi16(-1);
		incs = _mm_set1_epi16(8);
		mask = _mm_set1_epi16(0xff00);
		zero = _mm_setzero_si128();
		mind = _mm_setr_epi16(0, 1, 2, 3, 4, 5, 6, 7);

		xbuf = _mm_loadu_si128((__m128i *)px);
		buf1 = _mm_slli_epi16(xbuf, 8);
		buf2 = _mm_and_si128(xbuf, mask);
		buf1 = _mm_mulhi_epi16(buf1, buf1);
		buf2 = _mm_mulhi_epi16(buf2, buf2);
		buf1 = _mm_add_epi16(buf1, buf2); /*  0~32768 */
		mmaxmag = _mm_add_epi16(buf1, base); /* -1~32767 */
		mmaxval = xbuf;
		mmaxind = mind;
		px += nstep;

		if (((mlib_addr)px & 15) == 0) {
			for (i = 1; i < n2; i++) {
				xbuf = _mm_load_si128((__m128i *)px);
				buf1 = _mm_slli_epi16(xbuf, 8);
				buf2 = _mm_and_si128(xbuf, mask);
				buf1 = _mm_mulhi_epi16(buf1, buf1);
				buf2 = _mm_mulhi_epi16(buf2, buf2);
				buf1 = _mm_add_epi16(buf1, buf2); /*  0~32768 */
				buf1 = _mm_add_epi16(buf1, base); /* -1~32767 */
				buf2 = _mm_cmpgt_epi16(buf1, mmaxmag);
				xbuf = _mm_and_si128(buf2, xbuf);
				mmaxmag = _mm_max_epi16(mmaxmag, buf1);
				mmaxval = _mm_andnot_si128(buf2, mmaxval);
				mmaxval = _mm_or_si128(mmaxval, xbuf);

				/* save the index */
				mind = _mm_add_epi16(mind, incs);
				mmaxind = _mm_or_si128(
					_mm_and_si128(buf2, mind),
					_mm_andnot_si128(buf2, mmaxind));

				px += nstep;
			}
		} else {
			for (i = 1; i < n2; i++) {
				xbuf = _mm_loadu_si128((__m128i *)px);
				buf1 = _mm_slli_epi16(xbuf, 8);
				buf2 = _mm_and_si128(xbuf, mask);
				buf1 = _mm_mulhi_epi16(buf1, buf1);
				buf2 = _mm_mulhi_epi16(buf2, buf2);
				buf1 = _mm_add_epi16(buf1, buf2); /*  0~32768 */
				buf1 = _mm_add_epi16(buf1, base); /* -1~32767 */
				buf2 = _mm_cmpgt_epi16(buf1, mmaxmag);
				xbuf = _mm_and_si128(buf2, xbuf);
				mmaxmag = _mm_max_epi16(mmaxmag, buf1);
				mmaxval = _mm_andnot_si128(buf2, mmaxval);
				mmaxval = _mm_or_si128(mmaxval, xbuf);

				/* save the index */
				mind = _mm_add_epi16(mind, incs);
				mmaxind = _mm_or_si128(
					_mm_and_si128(buf2, mind),
					_mm_andnot_si128(buf2, mmaxind));

				px += nstep;
			}
		}
		mmaxmag = _mm_sub_epi16(mmaxmag, base);
		mlib_u16 mags[8], vals[8], inds[8];
		_mm_storeu_si128((__m128i *)mags, mmaxmag);
		_mm_storeu_si128((__m128i *)vals, mmaxval);
		_mm_storeu_si128((__m128i *)inds, mmaxind);
		mag_max = mags[0];
		val_max = vals[0];
		ind_max = inds[0];
		for (i = 1; i < 8; i++) {
			mag = mags[i];
			ind = inds[i];
			if ((mag > mag_max) ||
				(mag == mag_max && ind < ind_max)) {
				val_max = vals[i];
				mag_max = mag;
				ind_max = ind;
			}
		}
		for (i = 0; i < n3; i++) {
			mag = (mlib_s32)px[0] * px[0] + (mlib_s32)px[1] * px[1];

			if (mag > mag_max) {
				val_max = *(mlib_u16 *)px;
				mag_max = mag;
			}
			px += 2;
		}
	} else {
		val_max = *(mlib_u16 *)px;
		mag_max = (mlib_s32)px[0] * px[0] + (mlib_s32)px[1] * px[1];
		px += 2;

		for (i = 1; i < n; i++) {
			mag = (mlib_s32)px[0] * px[0] + (mlib_s32)px[1] * px[1];

			if (mag > mag_max) {
				val_max = *(mlib_u16 *)px;
				mag_max = mag;
			}
			px += 2;
		}
	}
	*(mlib_u16 *)max = val_max;

	return (mag_max);
}

mlib_status
__mlib_VectorMaximumMag_S8C(
	mlib_s8 *max,
	const mlib_s8 *x,
	mlib_s32 n)
{
	if (n <= 0)
		return (MLIB_FAILURE);
	else if (n <= 65536) {
		mlib_VectorMaximumMag_S8C_part((mlib_u16 *)max, x, n);
		return (MLIB_SUCCESS);
	} else {
		mlib_s32 nblock = n >> 16;
		mlib_s32 ntail = n & 65535;
		mlib_s32 i, mag, mag_max;
		mlib_u16 val, val_max;
		const mlib_s8 *px = x;

		mag_max = mlib_VectorMaximumMag_S8C_part(&val, px, n);
		for (i = 1; i < nblock; i++) {
			mag = mlib_VectorMaximumMag_S8C_part(&val_max,
				px, 65536);
			if (mag > mag_max) {
				val_max = val;
				mag_max = mag;
			}
			px += 65536 * 2;
		}
		mag = mlib_VectorMaximumMag_S8C_part(&val_max, px, n);
		if (mag > mag_max) {
			val_max = val;
			mag_max = mag;
		}

		*(mlib_u16 *)max = val_max;
		return (MLIB_SUCCESS);
	}
}

/* *********************************************************** */

mlib_status
__mlib_VectorMaximumMag_S16C(
	mlib_s16 *max,
	const mlib_s16 *x,
	mlib_s32 n)
{
	if (n <= 0)
		return (MLIB_FAILURE);

	const mlib_s16 *px = x;
	mlib_s32 i, nstep, n2, n3;
	mlib_u32 mag, mag_max, val_max, ind, ind_max;
	__m128i xbuf, mmaxmag, mmaxval, buf1, buf2, buf3, buf4, base;
	__m128i mmaxind, mind, incs;

	nstep = 16 / sizeof (mlib_s16);
	n2 = n / (nstep / 2);
	n3 = n - n2 * nstep / 2;

	if (n2 > 0) {
		base = _mm_set1_epi32(-1);
		mind = _mm_setr_epi32(0, 1, 2, 3);
		incs = _mm_set1_epi32(4);

		xbuf = _mm_loadu_si128((__m128i *)px);
		buf1 = _mm_madd_epi16(xbuf, xbuf);
		mmaxmag = _mm_add_epi32(buf1, base);
		mmaxval = xbuf;
		mmaxind = mind;
		px += nstep;

		if (((mlib_addr)px & 15) == 0) {
			for (i = 1; i < n2; i++) {
				xbuf = _mm_load_si128((__m128i *)px);
				buf1 = _mm_madd_epi16(xbuf, xbuf);
				buf1 = _mm_add_epi32(buf1, base);
				buf2 = _mm_cmpgt_epi32(buf1, mmaxmag);
				buf3 = _mm_and_si128(buf2, xbuf);
				buf4 = _mm_and_si128(buf2, buf1);
				mmaxval = _mm_andnot_si128(buf2, mmaxval);
				mmaxmag = _mm_andnot_si128(buf2, mmaxmag);
				mmaxval = _mm_or_si128(mmaxval, buf3);
				mmaxmag = _mm_or_si128(mmaxmag, buf4);

				/* save the index */
				mind = _mm_add_epi16(mind, incs);
				mmaxind = _mm_or_si128(
					_mm_and_si128(buf2, mind),
					_mm_andnot_si128(buf2, mmaxind));

				px += nstep;
			}
		} else {
			for (i = 1; i < n2; i++) {
				xbuf = _mm_loadu_si128((__m128i *)px);
				buf1 = _mm_madd_epi16(xbuf, xbuf);
				buf1 = _mm_add_epi32(buf1, base);
				buf2 = _mm_cmpgt_epi32(buf1, mmaxmag);
				buf3 = _mm_and_si128(buf2, xbuf);
				buf4 = _mm_and_si128(buf2, buf1);
				mmaxval = _mm_andnot_si128(buf2, mmaxval);
				mmaxmag = _mm_andnot_si128(buf2, mmaxmag);
				mmaxval = _mm_or_si128(mmaxval, buf3);
				mmaxmag = _mm_or_si128(mmaxmag, buf4);

				/* save the index */
				mind = _mm_add_epi16(mind, incs);
				mmaxind = _mm_or_si128(
					_mm_and_si128(buf2, mind),
					_mm_andnot_si128(buf2, mmaxind));

				px += nstep;
			}
		}
		mmaxmag = _mm_sub_epi32(mmaxmag, base);
		mlib_u32 mags[4], vals[4], inds[4];
		_mm_storeu_si128((__m128i *)mags, mmaxmag);
		_mm_storeu_si128((__m128i *)vals, mmaxval);
		_mm_storeu_si128((__m128i *)inds, mmaxind);
		val_max = vals[0];
		mag_max = mags[0];
		ind_max = inds[0];
		for (i = 1; i < 4; i++) {
			mag = mags[i];
			ind = inds[i];
			if ((mag > mag_max) ||
				(mag == mag_max && ind < ind_max)) {
				val_max = vals[i];
				mag_max = mag;
				ind_max = ind;
			}
		}

		for (i = 0; i < n3; i++) {
			mag = (mlib_s32)px[0] * px[0] + (mlib_s32)px[1] * px[1];

			if (mag > mag_max) {
				val_max = *(mlib_u32 *)px;
				mag_max = mag;
			}
			px += 2;
		}
	} else {
		val_max = *(mlib_u32 *)px;
		mag_max = (mlib_s32)px[0] * px[0] + (mlib_s32)px[1] * px[1];
		px += 2;
		for (i = 1; i < n; i++) {
			mag = (mlib_s32)px[0] * px[0] + (mlib_s32)px[1] * px[1];
			if (mag > mag_max) {
				val_max = *(mlib_u32 *)px;
				mag_max = mag;
			}
			px += 2;
		}
	}
	*(mlib_u32 *)max = val_max;

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#define	COMPUTE_C(TYPE, TYPE_S)                                  \
	TYPE *psrc = (TYPE *) x;                                 \
	mlib_d64 __max_re, __max_im;                             \
	mlib_s32 i;                                              \
	mlib_d64 __val_re;                                       \
	mlib_d64 __val_im;                                       \
	mlib_d64 __val;                                          \
	mlib_d64 __val_max;                                      \
	                                                         \
	if (n <= 0)                                              \
	    return (MLIB_FAILURE);                               \
	__max_re = __max_im = __val_max = 0;                     \
	for (i = 0; i < n; i++) {                                \
	    __val_re = psrc[2 * i];                              \
	    __val_im = psrc[2 * i + 1];                          \
	    __val = __val_im * __val_im + __val_re * __val_re;   \
	    if (__val > __val_max) {                             \
		__max_re = __val_re;                             \
		__max_im = __val_im;                             \
		__val_max = __val;                               \
	    }                                                    \
	}                                                        \
	                                                         \
	max[0] = __max_re;                                       \
	max[1] = __max_im;                                       \
	return MLIB_SUCCESS

mlib_status
__mlib_VectorMaximumMag_S32C(
	mlib_s32 *max,
	const mlib_s32 *x,
	mlib_s32 n)
{
	COMPUTE_C(mlib_s32,
		S32);
}

/* *********************************************************** */
