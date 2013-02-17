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
 * Copyright 2005 Sun Microsystems, Inc.  All rights reserved.
 * Use is subject to license terms.
 */

#pragma ident	"@(#)mlib_c_VectorMinimum.c	9.2	07/10/09 SMI"

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
#include <mlib_c_VectorMaximum.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_VectorMinimum_S8 = __mlib_VectorMinimum_S8
#pragma weak mlib_VectorMinimum_U8 = __mlib_VectorMinimum_U8
#pragma weak mlib_VectorMinimum_S16 = __mlib_VectorMinimum_S16
#pragma weak mlib_VectorMinimumMag_S8C = __mlib_VectorMinimumMag_S8C
#pragma weak mlib_VectorMinimumMag_U8C = __mlib_VectorMinimumMag_U8C
#pragma weak mlib_VectorMinimumMag_S16C = __mlib_VectorMinimumMag_S16C

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */

__typeof__(__mlib_VectorMinimum_S8) mlib_VectorMinimum_S8
	__attribute__((weak, alias("__mlib_VectorMinimum_S8")));
__typeof__(__mlib_VectorMinimum_U8) mlib_VectorMinimum_U8
	__attribute__((weak, alias("__mlib_VectorMinimum_U8")));
__typeof__(__mlib_VectorMinimum_S16) mlib_VectorMinimum_S16
	__attribute__((weak, alias("__mlib_VectorMinimum_S16")));
__typeof__(__mlib_VectorMinimumMag_S8C) mlib_VectorMinimumMag_S8C
	__attribute__((weak, alias("__mlib_VectorMinimumMag_S8C")));
__typeof__(__mlib_VectorMinimumMag_U8C) mlib_VectorMinimumMag_U8C
	__attribute__((weak, alias("__mlib_VectorMinimumMag_U8C")));
__typeof__(__mlib_VectorMinimumMag_S16C) mlib_VectorMinimumMag_S16C
	__attribute__((weak, alias("__mlib_VectorMinimumMag_S16C")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	MIN_LOG(min, arg)	min += ((arg - min) & ((arg - min) >> 31))
#define	MIN_CMP(min, arg)	min = (arg < min) ? arg : min

/* *********************************************************** */

mlib_status
__mlib_VectorMinimum_U8(
	mlib_u8 *min,
	const mlib_u8 *x,
	mlib_s32 n)
{
	mlib_u8 *psrc = (mlib_u8 *)x;
	mlib_u8 min1, min2, min3;
	mlib_s32 i;

	if (n <= 0)
		return (MLIB_FAILURE);

	min1 = min2 = min3 = MLIB_U8_MAX;

	for (i = 0; i <= (n - 3); i += 3) {
		MIN_LOG(min1, psrc[i]);
		MIN_LOG(min2, psrc[i + 1]);
		MIN_LOG(min3, psrc[i + 2]);
	}

	if (i < n)
		MIN_LOG(min1, psrc[i]);
	i++;

	if (i < n)
		MIN_LOG(min2, psrc[i]);

	MIN_LOG(min1, min2);
	MIN_LOG(min1, min3);

	min[0] = min1;

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VectorMinimum_S8(
	mlib_s8 *min,
	const mlib_s8 *x,
	mlib_s32 n)
{
	mlib_s8 *psrc = (mlib_s8 *)x;
	mlib_d64 min1, min2, min3, min4;
	mlib_s32 i;

	if (n <= 0)
		return (MLIB_FAILURE);

	min1 = min2 = min3 = min4 = MLIB_S8_MAX;

	for (i = 0; i <= (n - 4); i += 4) {
		MIN_CMP(min1, psrc[i]);
		MIN_CMP(min2, psrc[i + 1]);
		MIN_CMP(min3, psrc[i + 2]);
		MIN_CMP(min4, psrc[i + 3]);
	}

	if (i < n)
		MIN_CMP(min1, psrc[i]);
	i++;

	if (i < n)
		MIN_CMP(min2, psrc[i]);
	i++;

	if (i < n)
		MIN_CMP(min3, psrc[i]);

	MIN_CMP(min1, min2);
	MIN_CMP(min3, min4);
	MIN_CMP(min1, min3);

	min[0] = (mlib_s8)min1;

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VectorMinimum_S16(
	mlib_s16 *min,
	const mlib_s16 *x,
	mlib_s32 n)
{
	mlib_s16 *psrc = (mlib_s16 *)x;
	mlib_d64 min1, min2, min3, min4;
	mlib_s32 i;

	if (n <= 0)
		return (MLIB_FAILURE);

	min1 = min2 = min3 = min4 = MLIB_S16_MAX;

	for (i = 0; i <= (n - 4); i += 4) {
		MIN_CMP(min1, psrc[i]);
		MIN_CMP(min2, psrc[i + 1]);
		MIN_CMP(min3, psrc[i + 2]);
		MIN_CMP(min4, psrc[i + 3]);
	}

	if (i < n)
		MIN_CMP(min1, psrc[i]);
	i++;

	if (i < n)
		MIN_CMP(min2, psrc[i]);
	i++;

	if (i < n)
		MIN_CMP(min3, psrc[i]);

	MIN_CMP(min1, min2);
	MIN_CMP(min3, min4);
	MIN_CMP(min1, min3);

	min[0] = (mlib_s16)min1;

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VectorMinimumMag_U8C(
	mlib_u8 *min,
	const mlib_u8 *x,
	mlib_s32 n)
{
	mlib_u8 *psrc = (mlib_u8 *)x;
	const mlib_d64 *psqw = mlib_sqw_S8 + 128;
	mlib_u8 min_re, min_im;
	mlib_s32 i;
	mlib_d64 val;
	mlib_d64 val_min;

	if (n <= 0)
		return (MLIB_FAILURE);

	min_re = psrc[0];
	min_im = psrc[1];
	val_min = psqw[min_re] + psqw[min_im];

	for (i = 1; i < n; i++) {
		val = psqw[psrc[2 * i]] + psqw[psrc[2 * i + 1]];

		if (val < val_min) {
			min_re = psrc[2 * i];
			min_im = psrc[2 * i + 1];
			val_min = val;
		}
	}

	min[0] = min_re;
	min[1] = min_im;

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VectorMinimumMag_S8C(
	mlib_s8 *min,
	const mlib_s8 *x,
	mlib_s32 n)
{
	mlib_s8 *psrc = (mlib_s8 *)x;
	const mlib_d64 *psqw = mlib_sqw_S8 + 128;
	mlib_s8 min_re, min_im;
	mlib_s32 i;
	mlib_d64 val;
	mlib_d64 val_min;

	if (n <= 0)
		return (MLIB_FAILURE);

	min_re = psrc[0];
	min_im = psrc[1];
	val_min = psqw[min_re] + psqw[min_im];

	for (i = 1; i < n; i++) {
		val = psqw[psrc[2 * i]] + psqw[psrc[2 * i + 1]];

		if (val < val_min) {
			min_re = psrc[2 * i];
			min_im = psrc[2 * i + 1];
			val_min = val;
		}
	}

	min[0] = min_re;
	min[1] = min_im;

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VectorMinimumMag_S16C(
	mlib_s16 *min,
	const mlib_s16 *x,
	mlib_s32 n)
{
	mlib_s16 *psrc = (mlib_s16 *)x;
	mlib_d64 min_re, min_im;
	mlib_s32 i;
	const mlib_d64 *psqw = mlib_S16uh + 128;
	mlib_d64 val;
	mlib_d64 val_min;

	if (n <= 0)
		return (MLIB_FAILURE);

	min_re = psrc[0];
	min_im = psrc[1];
	val_min = min_re * min_re + min_im * min_im;

#pragma pipeloop(0)
	for (i = 1; i < n; i++) {
		mlib_d64 ld00 = psqw[psrc[2 * i] >> 8];
		mlib_d64 ld01 = mlib_S16lh[psrc[2 * i] & 0xFF];
		mlib_d64 ld10 = psqw[psrc[2 * i + 1] >> 8];
		mlib_d64 ld11 = mlib_S16lh[psrc[2 * i + 1] & 0xFF];

		val = (ld00 + ld01) * (ld00 + ld01) +
			(ld10 + ld11) * (ld10 + ld11);

		if (val < val_min) {
			min_re = (ld00 + ld01);
			min_im = (ld10 + ld11);
			val_min = val;
		}
	}

	min[0] = (mlib_s16)min_re;
	min[1] = (mlib_s16)min_im;

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
