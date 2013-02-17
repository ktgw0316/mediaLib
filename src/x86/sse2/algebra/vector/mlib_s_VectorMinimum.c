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

#pragma ident	"@(#)mlib_s_VectorMinimum.c	9.5	07/11/05 SMI"

/*
 * FUNCTIONS
 *      mlib_VectorMinimum - find the minimum
 *
 * SYNOPSIS
 *      mlib_status mlib_VectorMinimum_U8(mlib_u8       *min,
 *                                        const mlib_u8 *x,
 *                                        mlib_s32      n);
 *      mlib_status mlib_VectorMinimum_S8(mlib_s8       *min,
 *                                        const mlib_s8 *x,
 *                                        mlib_s32      n);
 *      mlib_status mlib_VectorMinimum_S16(mlib_s16       *min,
 *                                         const mlib_s16 *x,
 *                                         mlib_s32       n);
 *      mlib_status mlib_VectorMinimum_S32(mlib_s32       *min,
 *                                         const mlib_s32 *x,
 *                                         mlib_s32       n);
 *
 * ARGUMENTS
 *      min    the pointer to minimum value
 *      x      the pointer to the first element of the source vector
 *      n      the number of elements in the source vector
 *
 * DESCRIPTION
 *            n - 1
 *      min = Min x[i]
 *            i = 0
 */

/*
 * FUNCTIONS
 *      mlib_VectorMinimumMag - find the minimum
 *
 * SYNOPSIS
 *      mlib_status mlib_VectorMinimumMag_U8C(mlib_u8       *min,
 *                                            const mlib_u8 *x,
 *                                            mlib_s32      n);
 *      mlib_status mlib_VectorMinimumMag_S8C(mlib_s8       *min,
 *                                            const mlib_s8 *x,
 *                                            mlib_s32      n);
 *      mlib_status mlib_VectorMinimumMag_S16C(mlib_s16       *min,
 *                                             const mlib_s16 *x,
 *                                             mlib_s32       n);
 *      mlib_status mlib_VectorMinimumMag_S32C(mlib_s32       *min,
 *                                             const mlib_s32 *x,
 *                                             mlib_s32       n);
 *
 * ARGUMENTS
 *      min    the pointer to minimum value
 *      x      the pointer to the first element of the source vector
 *      n      the number of elements in the source vector
 *
 * DESCRIPTION
 *      This function finds the first element with the minimum
 *      magnitude, then puts the real and imaginary parts of it into
 *      min[0] and min[1], respectively.
 *
 *            n - 1
 *      min = Min x[i]
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

#pragma weak mlib_VectorMinimum_S8 = __mlib_VectorMinimum_S8
#pragma weak mlib_VectorMinimum_U8 = __mlib_VectorMinimum_U8
#pragma weak mlib_VectorMinimum_S16 = __mlib_VectorMinimum_S16
#pragma weak mlib_VectorMinimum_S32 = __mlib_VectorMinimum_S32
#pragma weak mlib_VectorMinimumMag_S8C = __mlib_VectorMinimumMag_S8C
#pragma weak mlib_VectorMinimumMag_U8C = __mlib_VectorMinimumMag_U8C
#pragma weak mlib_VectorMinimumMag_S16C = __mlib_VectorMinimumMag_S16C
#pragma weak mlib_VectorMinimumMag_S32C = __mlib_VectorMinimumMag_S32C

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */

__typeof__(__mlib_VectorMinimum_S8) mlib_VectorMinimum_S8
	__attribute__((weak, alias("__mlib_VectorMinimum_S8")));
__typeof__(__mlib_VectorMinimum_U8) mlib_VectorMinimum_U8
	__attribute__((weak, alias("__mlib_VectorMinimum_U8")));
__typeof__(__mlib_VectorMinimum_S16) mlib_VectorMinimum_S16
	__attribute__((weak, alias("__mlib_VectorMinimum_S16")));
__typeof__(__mlib_VectorMinimum_S32) mlib_VectorMinimum_S32
	__attribute__((weak, alias("__mlib_VectorMinimum_S32")));
__typeof__(__mlib_VectorMinimumMag_S8C) mlib_VectorMinimumMag_S8C
	__attribute__((weak, alias("__mlib_VectorMinimumMag_S8C")));
__typeof__(__mlib_VectorMinimumMag_U8C) mlib_VectorMinimumMag_U8C
	__attribute__((weak, alias("__mlib_VectorMinimumMag_U8C")));
__typeof__(__mlib_VectorMinimumMag_S16C) mlib_VectorMinimumMag_S16C
	__attribute__((weak, alias("__mlib_VectorMinimumMag_S16C")));
__typeof__(__mlib_VectorMinimumMag_S32C) mlib_VectorMinimumMag_S32C
	__attribute__((weak, alias("__mlib_VectorMinimumMag_S32C")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	MIN_CMP(min, arg)	min = (arg < min) ? arg : min

/* *********************************************************** */

mlib_status
__mlib_VectorMinimum_U8(
	mlib_u8 *min,
	const mlib_u8 *x,
	mlib_s32 n)
{
	if (n < 1)
		return (MLIB_FAILURE);

	const mlib_u8 *px = x;
	mlib_u8 pm[8], smin = MLIB_U8_MAX;
	mlib_s32 ax, nstep, n1, n2, n3, i;
	__m128i mmin, xbuf;

	ax = (mlib_addr)x & 15;
	nstep = 16 / sizeof (mlib_u8);
	n1 = ((16 - ax) & 15) / sizeof (mlib_u8);
	n2 = (n - n1) / nstep;
	n3 = n - n1 - n2 * nstep;

	if (n2 > 1) {
		for (i = 0; i < n1; i++) {
			MIN_CMP(smin, *px);
			px++;
		}

		mmin = _mm_load_si128((__m128i *)px);
		px += nstep;
		for (i = 1; i < n2; i++) {
			xbuf = _mm_load_si128((__m128i *)px);
			mmin = _mm_min_epu8(mmin, xbuf);
			px += nstep;
		}
		xbuf = _mm_unpacklo_epi64(mmin, mmin);
		mmin = _mm_unpackhi_epi64(mmin, mmin);
		mmin = _mm_min_epu8(mmin, xbuf);
		_mm_storel_epi64((__m128i *)pm, mmin);
		for (i = 0; i < 8; i++) {
			MIN_CMP(smin, pm[i]);
		}

		for (i = 0; i < n3; i++) {
			MIN_CMP(smin, *px);
			px++;
		}
	} else {
		for (i = 0; i < n; i++) {
			MIN_CMP(smin, *px);
			px++;
		}
	}

	*min = smin;
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VectorMinimum_S8(
	mlib_s8 *min,
	const mlib_s8 *x,
	mlib_s32 n)
{
	if (n < 1)
		return (MLIB_FAILURE);

	const mlib_s8 *px = x;
	mlib_s8 pm[8], smin = MLIB_S8_MAX;
	mlib_s32 ax, nstep, n1, n2, n3, i;
	__m128i mmin, xbuf, incs;
	incs = _mm_set1_epi8(MLIB_S8_MIN);

	ax = (mlib_addr)x & 15;
	nstep = 16 / sizeof (mlib_s8);
	n1 = ((16 - ax) & 15) / sizeof (mlib_s8);
	n2 = (n - n1) / nstep;
	n3 = n - n1 - n2 * nstep;

	if (n2 > 1) {
		for (i = 0; i < n1; i++) {
			MIN_CMP(smin, *px);
			px++;
		}

		mmin = _mm_load_si128((__m128i *)px);
		mmin = _mm_sub_epi8(mmin, incs);
		px += nstep;
		for (i = 1; i < n2; i++) {
			xbuf = _mm_load_si128((__m128i *)px);
			xbuf = _mm_sub_epi8(xbuf, incs);
			mmin = _mm_min_epu8(mmin, xbuf);
			px += nstep;
		}
		xbuf = _mm_unpacklo_epi64(mmin, mmin);
		mmin = _mm_unpackhi_epi64(mmin, mmin);
		mmin = _mm_min_epu8(mmin, xbuf);
		mmin = _mm_add_epi8(mmin, incs);
		_mm_storel_epi64((__m128i *)pm, mmin);
		for (i = 0; i < 8; i++) {
			MIN_CMP(smin, pm[i]);
		}

		for (i = 0; i < n3; i++) {
			MIN_CMP(smin, *px);
			px++;
		}
	} else {
		for (i = 0; i < n; i++) {
			MIN_CMP(smin, *px);
			px++;
		}
	}

	*min = smin;
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VectorMinimum_S16(
	mlib_s16 *min,
	const mlib_s16 *x,
	mlib_s32 n)
{
	if (n < 1)
		return (MLIB_FAILURE);

	const mlib_s16 *px = x;
	mlib_s16 pm[4], smin = MLIB_S16_MAX;
	mlib_s32 ax, nstep, n1, n2, n3, i;
	__m128i mmin, xbuf;

	ax = (mlib_addr)x & 15;
	nstep = 16 / sizeof (mlib_s16);
	n1 = ((16 - ax) & 15) / sizeof (mlib_s16);
	n2 = (n - n1) / nstep;
	n3 = n - n1 - n2 * nstep;

	if (n2 > 1) {
		for (i = 0; i < n1; i++) {
			MIN_CMP(smin, *px);
			px++;
		}

		mmin = _mm_load_si128((__m128i *)px);
		px += nstep;
		for (i = 1; i < n2; i++) {
			xbuf = _mm_load_si128((__m128i *)px);
			mmin = _mm_min_epi16(mmin, xbuf);
			px += nstep;
		}
		xbuf = _mm_unpacklo_epi64(mmin, mmin);
		mmin = _mm_unpackhi_epi64(mmin, mmin);
		mmin = _mm_min_epi16(mmin, xbuf);
		_mm_storel_epi64((__m128i *)pm, mmin);
		for (i = 0; i < 4; i++) {
			MIN_CMP(smin, pm[i]);
		}

		for (i = 0; i < n3; i++) {
			MIN_CMP(smin, *px);
			px++;
		}
	} else {
		for (i = 0; i < n; i++) {
			MIN_CMP(smin, *px);
			px++;
		}
	}

	*min = smin;
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VectorMinimum_S32(
	mlib_s32 *min,
	const mlib_s32 *x,
	mlib_s32 n)
{
	if (n < 1)
		return (MLIB_FAILURE);

	const mlib_s32 *px = x;
	mlib_s32 pm[4], smin = MLIB_S32_MAX;
	mlib_s32 ax, nstep, n1, n2, n3, i;
	__m128i mmin, xbuf, mask;

	ax = (mlib_addr)x & 15;
	nstep = 16 / sizeof (mlib_s32);
	n1 = ((16 - ax) & 15) / sizeof (mlib_s32);
	n2 = (n - n1) / nstep;
	n3 = n - n1 - n2 * nstep;

	if (n2 > 1) {
		for (i = 0; i < n1; i++) {
			MIN_CMP(smin, *px);
			px++;
		}

		mmin = _mm_load_si128((__m128i *)px);
		px += nstep;
		for (i = 1; i < n2; i++) {
			xbuf = _mm_load_si128((__m128i *)px);
			mask = _mm_cmpgt_epi32(mmin, xbuf);
			xbuf = _mm_and_si128(mask, xbuf);
			mmin = _mm_andnot_si128(mask, mmin);
			mmin = _mm_or_si128(mmin, xbuf);
			px += nstep;
		}
		_mm_storeu_si128((__m128i *)pm, mmin);
		for (i = 0; i < 4; i++) {
			MIN_CMP(smin, pm[i]);
		}

		for (i = 0; i < n3; i++) {
			MIN_CMP(smin, *px);
			px++;
		}
	} else {
		for (i = 0; i < n; i++) {
			MIN_CMP(smin, *px);
			px++;
		}
	}

	*min = smin;
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VectorMinimumMag_U8C(
	mlib_u8 *min,
	const mlib_u8 *x,
	mlib_s32 n)
{
	if (n <= 0)
		return (MLIB_FAILURE);

	const mlib_u8 *px = x;
	mlib_s32 i, nstep, n2, n3;
	mlib_u16 val_min;
	mlib_u32 mag, mag_min, ind, ind_min;
	__m128i xbuf, xlo, xhi;
	__m128i mminmag, mminval, buf1, buf2, buf3, buf4, zero;
	__m128i mminind, mind, incs;

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
		mminval = xlo;
		mminmag = _mm_madd_epi16(xlo, xlo);
		mminind = mind;

		buf1 = _mm_madd_epi16(xhi, xhi);
		buf2 = _mm_cmpgt_epi32(mminmag, buf1);
		buf3 = _mm_and_si128(buf2, xhi);
		buf4 = _mm_and_si128(buf2, buf1);
		mminval = _mm_andnot_si128(buf2, mminval);
		mminmag = _mm_andnot_si128(buf2, mminmag);
		mminval = _mm_or_si128(mminval, buf3);
		mminmag = _mm_or_si128(mminmag, buf4);

		mind = _mm_add_epi32(mind, incs);
		mminind = _mm_or_si128(
			_mm_and_si128(buf2, mind),
			_mm_andnot_si128(buf2, mminind));

		px += nstep;

		if (((mlib_addr)px & 15) == 0) {
			for (i = 1; i < n2; i++) {
				xbuf = _mm_load_si128((__m128i *)px);
				xlo  = _mm_unpacklo_epi8(xbuf, zero);
				xhi  = _mm_unpackhi_epi8(xbuf, zero);

				buf1 = _mm_madd_epi16(xlo, xlo);
				buf2 = _mm_cmpgt_epi32(mminmag, buf1);
				buf3 = _mm_and_si128(buf2, xlo);
				buf4 = _mm_and_si128(buf2, buf1);
				mminval = _mm_andnot_si128(buf2, mminval);
				mminmag = _mm_andnot_si128(buf2, mminmag);
				mminval = _mm_or_si128(mminval, buf3);
				mminmag = _mm_or_si128(mminmag, buf4);

				/* save the index */
				mind = _mm_add_epi32(mind, incs);
				mminind = _mm_or_si128(
					_mm_and_si128(buf2, mind),
					_mm_andnot_si128(buf2, mminind));

				buf1 = _mm_madd_epi16(xhi, xhi);
				buf2 = _mm_cmpgt_epi32(mminmag, buf1);
				buf3 = _mm_and_si128(buf2, xhi);
				buf4 = _mm_and_si128(buf2, buf1);
				mminval = _mm_andnot_si128(buf2, mminval);
				mminmag = _mm_andnot_si128(buf2, mminmag);
				mminval = _mm_or_si128(mminval, buf3);
				mminmag = _mm_or_si128(mminmag, buf4);

				/* save the index */
				mind = _mm_add_epi32(mind, incs);
				mminind = _mm_or_si128(
					_mm_and_si128(buf2, mind),
					_mm_andnot_si128(buf2, mminind));

				px += nstep;
			}
		} else {
			for (i = 1; i < n2; i++) {
				xbuf = _mm_loadu_si128((__m128i *)px);
				xlo  = _mm_unpacklo_epi8(xbuf, zero);
				xhi  = _mm_unpackhi_epi8(xbuf, zero);

				buf1 = _mm_madd_epi16(xlo, xlo);
				buf2 = _mm_cmpgt_epi32(mminmag, buf1);
				buf3 = _mm_and_si128(buf2, xlo);
				buf4 = _mm_and_si128(buf2, buf1);
				mminval = _mm_andnot_si128(buf2, mminval);
				mminmag = _mm_andnot_si128(buf2, mminmag);
				mminval = _mm_or_si128(mminval, buf3);
				mminmag = _mm_or_si128(mminmag, buf4);

				/* save the index */
				mind = _mm_add_epi32(mind, incs);
				mminind = _mm_or_si128(
					_mm_and_si128(buf2, mind),
					_mm_andnot_si128(buf2, mminind));

				buf1 = _mm_madd_epi16(xhi, xhi);
				buf2 = _mm_cmpgt_epi32(mminmag, buf1);
				buf3 = _mm_and_si128(buf2, xhi);
				buf4 = _mm_and_si128(buf2, buf1);
				mminval = _mm_andnot_si128(buf2, mminval);
				mminmag = _mm_andnot_si128(buf2, mminmag);
				mminval = _mm_or_si128(mminval, buf3);
				mminmag = _mm_or_si128(mminmag, buf4);

				/* save the index */
				mind = _mm_add_epi32(mind, incs);
				mminind = _mm_or_si128(
					_mm_and_si128(buf2, mind),
					_mm_andnot_si128(buf2, mminind));

				px += nstep;
			}
		}

		mlib_u32 mags[4], vals[4], inds[4], val_min_32 = 0;
		_mm_storeu_si128((__m128i *)mags, mminmag);
		_mm_storeu_si128((__m128i *)vals, mminval);
		_mm_storeu_si128((__m128i *)inds, mminind);
		val_min_32 = vals[0];
		mag_min = mags[0];
		ind_min = inds[0];

		for (i = 1; i < 4; i++) {
			mag = mags[i];
			ind = inds[i];
			if ((mag < mag_min) ||
				(mag == mag_min && ind < ind_min)) {
				val_min_32 = vals[i];
				ind_min = ind;
				mag_min = mag;
			}
		}
		val_min = (val_min_32 & 0xff) | ((val_min_32 >> 8) & 0xff00);

		for (i = 0; i < n3; i++) {
			mag = (mlib_s32)px[0] * px[0] + (mlib_s32)px[1] * px[1];

			if (mag < mag_min) {
				val_min = *(mlib_u16 *)px;
				mag_min = mag;
			}
			px += 2;
		}
	} else {
		val_min = *(mlib_u16 *)px;
		mag_min = (mlib_s32)px[0] * px[0] + (mlib_s32)px[1] * px[1];
		px += 2;

		for (i = 1; i < n; i++) {
			mag = (mlib_s32)px[0] * px[0] + (mlib_s32)px[1] * px[1];
			if (mag < mag_min) {
				val_min = *(mlib_u16 *)px;
				mag_min = mag;
			}
			px += 2;
		}
	}
	*(mlib_u16 *)min = val_min;

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_s32
mlib_VectorMinimumMag_S8C_part(
	mlib_u16 *min,
	const mlib_s8 *x,
	mlib_s32 n)
{
	const mlib_s8 *px = x;
	mlib_s32 i, nstep, n2, n3;
	mlib_u16 val_min, ind_min, ind;
	mlib_u32 mag, mag_min;
	__m128i xbuf, mminmag, mminval, mminind, mind;
	__m128i buf1, buf2, zero, mask, base, incs;

	val_min = 0;
	mag_min = 0;

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
		mminmag = _mm_add_epi16(buf1, base); /* -1~32767 */
		mminval = xbuf;
		mminind = mind;
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
				buf2 = _mm_cmpgt_epi16(mminmag, buf1);
				xbuf = _mm_and_si128(buf2, xbuf);
				mminmag = _mm_min_epi16(mminmag, buf1);
				mminval = _mm_andnot_si128(buf2, mminval);
				mminval = _mm_or_si128(mminval, xbuf);

				/* save the index */
				mind = _mm_add_epi16(mind, incs);
				mminind = _mm_or_si128(
					_mm_and_si128(buf2, mind),
					_mm_andnot_si128(buf2, mminind));

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
				buf2 = _mm_cmpgt_epi16(mminmag, buf1);
				xbuf = _mm_and_si128(buf2, xbuf);
				mminmag = _mm_min_epi16(mminmag, buf1);
				mminval = _mm_andnot_si128(buf2, mminval);
				mminval = _mm_or_si128(mminval, xbuf);

				/* save the index */
				mind = _mm_add_epi16(mind, incs);
				mminind = _mm_or_si128(
					_mm_and_si128(buf2, mind),
					_mm_andnot_si128(buf2, mminind));

				px += nstep;
			}
		}
		mminmag = _mm_sub_epi16(mminmag, base);
		mlib_u16 mags[8], vals[8], inds[8];
		_mm_storeu_si128((__m128i *)mags, mminmag);
		_mm_storeu_si128((__m128i *)vals, mminval);
		_mm_storeu_si128((__m128i *)inds, mminind);
		mag_min = mags[0];
		val_min = vals[0];
		ind_min = inds[0];
		for (i = 1; i < 8; i++) {
			mag = mags[i];
			ind = inds[i];
			if ((mag < mag_min) ||
				(mag == mag_min && ind < ind_min)) {
				val_min = vals[i];
				mag_min = mag;
				ind_min = ind;
			}
		}
		for (i = 0; i < n3; i++) {
			mag = (mlib_s32)px[0] * px[0] + (mlib_s32)px[1] * px[1];

			if (mag < mag_min) {
				val_min = *(mlib_u16 *)px;
				mag_min = mag;
			}
			px += 2;
		}
	} else {
		val_min = *(mlib_u16 *)px;
		mag_min = (mlib_s32)px[0] * px[0] + (mlib_s32)px[1] * px[1];
		px += 2;

		for (i = 1; i < n; i++) {
			mag = (mlib_s32)px[0] * px[0] + (mlib_s32)px[1] * px[1];

			if (mag < mag_min) {
				val_min = *(mlib_u16 *)px;
				mag_min = mag;
			}
			px += 2;
		}
	}
	*(mlib_u16 *)min = val_min;

	return (mag_min);
}

mlib_status
__mlib_VectorMinimumMag_S8C(
	mlib_s8 *min,
	const mlib_s8 *x,
	mlib_s32 n)
{
	if (n <= 0)
		return (MLIB_FAILURE);
	else if (n <= 65536) {
		mlib_VectorMinimumMag_S8C_part((mlib_u16 *)min, x, n);
		return (MLIB_SUCCESS);
	} else {
		mlib_s32 nblock = n >> 16;
		mlib_s32 ntail = n & 65535;
		mlib_s32 i, mag, mag_min;
		mlib_u16 val, val_min;
		const mlib_s8 *px = x;

		mag_min = mlib_VectorMinimumMag_S8C_part(&val, px, n);
		for (i = 1; i < nblock; i++) {
			mag = mlib_VectorMinimumMag_S8C_part(&val_min,
				px, 65536);
			if (mag < mag_min) {
				val_min = val;
				mag_min = mag;
			}
			px += 65536 * 2;
		}
		mag = mlib_VectorMinimumMag_S8C_part(&val_min, px, n);
		if (mag < mag_min) {
			val_min = val;
			mag_min = mag;
		}

		*(mlib_u16 *)min = val_min;
		return (MLIB_SUCCESS);
	}

}
/* *********************************************************** */

mlib_status
__mlib_VectorMinimumMag_S16C(
	mlib_s16 *min,
	const mlib_s16 *x,
	mlib_s32 n)
{
	if (n <= 0)
		return (MLIB_FAILURE);

	const mlib_s16 *px = x;
	mlib_s32 i, nstep, n2, n3;
	mlib_u32 mag, mag_min, val_min, ind, ind_min;
	__m128i xbuf, mminmag, mminval, buf1, buf2, buf3, buf4, base;
	__m128i mminind, mind, incs;

	nstep = 16 / sizeof (mlib_s16);
	n2 = n / (nstep / 2);
	n3 = n - n2 * nstep / 2;

	if (n2 > 0) {
		base = _mm_set1_epi32(-1);
		mind = _mm_setr_epi32(0, 1, 2, 3);
		incs = _mm_set1_epi32(4);

		xbuf = _mm_loadu_si128((__m128i *)px);
		buf1 = _mm_madd_epi16(xbuf, xbuf);
		mminmag = _mm_add_epi32(buf1, base);
		mminval = xbuf;
		mminind = mind;
		px += nstep;

		if (((mlib_addr)px & 15) == 0) {
			for (i = 1; i < n2; i++) {
				xbuf = _mm_load_si128((__m128i *)px);
				buf1 = _mm_madd_epi16(xbuf, xbuf);
				buf1 = _mm_add_epi32(buf1, base);
				buf2 = _mm_cmpgt_epi32(mminmag, buf1);
				buf3 = _mm_and_si128(buf2, xbuf);
				buf4 = _mm_and_si128(buf2, buf1);
				mminval = _mm_andnot_si128(buf2, mminval);
				mminmag = _mm_andnot_si128(buf2, mminmag);
				mminval = _mm_or_si128(mminval, buf3);
				mminmag = _mm_or_si128(mminmag, buf4);

				/* save the index */
				mind = _mm_add_epi16(mind, incs);
				mminind = _mm_or_si128(
					_mm_and_si128(buf2, mind),
					_mm_andnot_si128(buf2, mminind));

				px += nstep;
			}
		} else {
			for (i = 1; i < n2; i++) {
				xbuf = _mm_loadu_si128((__m128i *)px);
				buf1 = _mm_madd_epi16(xbuf, xbuf);
				buf1 = _mm_add_epi32(buf1, base);
				buf2 = _mm_cmpgt_epi32(mminmag, buf1);
				buf3 = _mm_and_si128(buf2, xbuf);
				buf4 = _mm_and_si128(buf2, buf1);
				mminval = _mm_andnot_si128(buf2, mminval);
				mminmag = _mm_andnot_si128(buf2, mminmag);
				mminval = _mm_or_si128(mminval, buf3);
				mminmag = _mm_or_si128(mminmag, buf4);

				/* save the index */
				mind = _mm_add_epi16(mind, incs);
				mminind = _mm_or_si128(
					_mm_and_si128(buf2, mind),
					_mm_andnot_si128(buf2, mminind));

				px += nstep;
			}
		}
		mminmag = _mm_sub_epi32(mminmag, base);
		mlib_u32 mags[4], vals[4], inds[4];
		_mm_storeu_si128((__m128i *)mags, mminmag);
		_mm_storeu_si128((__m128i *)vals, mminval);
		_mm_storeu_si128((__m128i *)inds, mminind);
		val_min = vals[0];
		mag_min = mags[0];
		ind_min = inds[0];
		for (i = 1; i < 4; i++) {
			mag = mags[i];
			ind = inds[i];
			if ((mag < mag_min) ||
				(mag == mag_min && ind < ind_min)) {
				val_min = vals[i];
				mag_min = mag;
				ind_min = ind;
			}
		}

		for (i = 0; i < n3; i++) {
			mag = (mlib_s32)px[0] * px[0] + (mlib_s32)px[1] * px[1];

			if (mag < mag_min) {
				val_min = *(mlib_u32 *)px;
				mag_min = mag;
			}
			px += 2;
		}
	} else {
		val_min = *(mlib_u32 *)px;
		mag_min = (mlib_s32)px[0] * px[0] + (mlib_s32)px[1] * px[1];
		px += 2;
		for (i = 1; i < n; i++) {
			mag = (mlib_s32)px[0] * px[0] + (mlib_s32)px[1] * px[1];
			if (mag < mag_min) {
				val_min = *(mlib_u32 *)px;
				mag_min = mag;
			}
			px += 2;
		}
	}
	*(mlib_u32 *)min = val_min;

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#define	COMPUTE_C(TYPE, TYPE_S)                                  \
	TYPE *psrc = (TYPE *) x;                                 \
	mlib_d64 __min_re, __min_im;                             \
	mlib_s32 i;                                              \
	mlib_d64 __val_re;                                       \
	mlib_d64 __val_im;                                       \
	mlib_d64 __val;                                          \
	mlib_d64 __val_min;                                      \
	                                                         \
	if (n <= 0)                                              \
	    return (MLIB_FAILURE);                               \
	__min_re = psrc[0];                                      \
	__min_im = psrc[1];                                      \
	__val_min = __min_im * __min_im + __min_re * __min_re;   \
	for (i = 1; i < n; i++) {                                \
	    __val_re = psrc[2 * i];                              \
	    __val_im = psrc[2 * i + 1];                          \
	    __val = __val_im * __val_im + __val_re * __val_re;   \
	    if (__val < __val_min) {                             \
		__min_re = __val_re;                             \
		__min_im = __val_im;                             \
		__val_min = __val;                               \
	    }                                                    \
	}                                                        \
	                                                         \
	min[0] = __min_re;                                       \
	min[1] = __min_im;                                       \
	return MLIB_SUCCESS

mlib_status
__mlib_VectorMinimumMag_S32C(
	mlib_s32 *min,
	const mlib_s32 *x,
	mlib_s32 n)
{
	COMPUTE_C(mlib_s32,
		S32);
}

/* *********************************************************** */
