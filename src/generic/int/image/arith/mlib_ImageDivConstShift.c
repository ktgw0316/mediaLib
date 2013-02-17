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

#pragma ident   "@(#)mlib_ImageDivConstShift.c	9.2	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_ImageDivConstShift         - divide an image by constants
 *      mlib_ImageDivConstShift_Inp     - divide an image by constants
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageDivConstShift(mlib_image       *dst,
 *                                          const mlib_image *src,
 *                                          const mlib_s32   *consts,
 *                                          mlib_s32         shift)
 *      mlib_status mlib_ImageDivConstShift_Inp(mlib_image     *srcdst,
 *                                              const mlib_s32 *consts,
 *                                              mlib_s32       shift)
 *
 * ARGUMENT
 *      src     pointer to the input image
 *      dst     pointer to the output image
 *      srcdst  pointer to the input and the output image
 *      consts  array of constants to divide into each channel of the pixels
 *      shift   left shifting factor
 *
 * RESTRICTION
 *      The images must have the same type, the same size, and the same number
 *      of channels.
 *      The images can have 1, 2, 3, or 4 channels.
 *      The images can be in MLIB_BYTE, MLIB_SHORT,
 *      MLIB_USHORT or MLIB_INT data type.
 *
 *      0 <= shift <= 31
 *
 * DESCRIPTION
 *      For the general version:
 *              dst[x][y][c] = src[x][y][c] / consts[c] * 2^shift
 *
 *      For in-place processing version (*_Inp):
 *              srcdst[x][y][c] = srcdst[x][y][c] / consts[c] * 2^shift
 *
 *      If the result of the operation underflows/overflows the
 *      minimum/maximum value supported by the destination image, then it
 *      will be clamped to the minimum/maximum value respectively.
 */
#ifndef _NO_LONGLONG

#include <mlib_image.h>
#include <mlib_ImageCheck.h>
#include <mlib_i_ImageDivConstShift.h>
/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageDivConstShift = __mlib_ImageDivConstShift
#pragma weak mlib_ImageDivConstShift_Inp = __mlib_ImageDivConstShift_Inp

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_ImageDivConstShift) mlib_ImageDivConstShift
    __attribute__((weak, alias("__mlib_ImageDivConstShift")));
__typeof__(__mlib_ImageDivConstShift_Inp) mlib_ImageDivConstShift_Inp
    __attribute__((weak, alias("__mlib_ImageDivConstShift_Inp")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */
#define	BUFF_SIZE	256

/* *********************************************************** */
mlib_status
__mlib_ImageDivConstShift(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_s32 *consts,
    mlib_s32 shift)
{
	mlib_type stype, dtype;
	mlib_s32 nchan, k;

	MLIB_IMAGE_CHECK(dst);
	MLIB_IMAGE_CHECK(src);
	MLIB_IMAGE_FULL_EQUAL(dst, src);

	if (shift < 0 || shift > 31)
	    return (MLIB_OUTOFRANGE);

	stype = mlib_ImageGetType(src);
	dtype = mlib_ImageGetType(dst);
	nchan = mlib_ImageGetChannels(dst);

	if (stype == MLIB_BYTE)
		return (mlib_i_ImageDivConstShift_U8(dst, src, consts, shift));
	if (stype == MLIB_SHORT)
		return (mlib_i_ImageDivConstShift_S16(dst, src, consts, shift));
	if (stype == MLIB_USHORT)
		return (mlib_i_ImageDivConstShift_U16(dst, src, consts, shift));
	if (stype == MLIB_INT)
		return (mlib_i_ImageDivConstShift_S32(dst, src, consts, shift));

	return (MLIB_FAILURE);
}

/* *********************************************************** */
mlib_status
mlib_i_ImageDivConstShift_U8(
		mlib_image *dst,
		const mlib_image *src,
		const mlib_s32 *consts,
		mlib_s32 shift)
{
	mlib_s64 ilut[128];
	mlib_u8 *lut[4], *plut = (mlib_u8 *)ilut;

	mlib_s32 nchan = mlib_ImageGetChannels(dst);
	mlib_s32 i, k;

	for (k = 0; k < nchan; k ++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */

		for (i = 0; i < 256; i ++) {
			mlib_s64 lval, temp;

			temp = ((mlib_s64)i << shift + 24)
				/ (((mlib_s64)consts[k] << 16) + 1);

			if (temp >= 0) {
				lval = temp + 128 >> 8;
			} else {
				lval = -((-temp) + 128 >> 8);
			}

			if (lval > 255) {
				plut[i] = 255;
			} else if (lval < 0) {
				plut[i] = 0;
			} else {
				plut[i] = lval;
			}
		}

		lut[k] = plut;
		plut += 256;

	}
	return (__mlib_ImageLookUp(dst, src, (const void **)lut));

}


/* *********************************************************** */

mlib_status
mlib_i_ImageDivConstShift_S16(
		mlib_image *dst,
		const mlib_image *src,
		const mlib_s32 *consts,
		mlib_s32 shift)
{
	mlib_s32 buf[BUFF_SIZE + 2];
	mlib_s16 *sl, *dl, *sp, *dp;
	mlib_s32 slb, dlb, xsize, ysize, nchan;
	mlib_s32 i, j, k, num, size;

	nchan = mlib_ImageGetChannels(dst);
	xsize = mlib_ImageGetWidth(dst);
	ysize = mlib_ImageGetHeight(dst);
	dlb = mlib_ImageGetStride(dst);
	dl = (void *)mlib_ImageGetData(dst);

	slb = mlib_ImageGetStride(src);
	sl = mlib_ImageGetData(src);

	slb /= 2;
	dlb /= 2;

	for (j = 0; j < ysize; j++) {
		for (k = 0; k < nchan; k++) {
			for (size = 0; size < xsize; size += num) {
				num = xsize - size;

				if (num > BUFF_SIZE)
					num = BUFF_SIZE;

				sp = sl + size * nchan + k;
				dp = dl + size * nchan + k;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i < num; i++) {
					mlib_s64 lval, temp;

					buf[i] = sp[i * nchan];

					if (consts[k] != 0) {
						temp = ((mlib_s64)buf[i]
							<< shift + 16)
							/ consts[k];

						if (temp >= 0) {
							lval = temp >> 16;
						} else {
							lval = -((-temp) >> 16);
						}

						if (lval > MLIB_S16_MAX) {
							dp[i * nchan] =
								MLIB_S16_MAX;
						} else if
							(lval < MLIB_S16_MIN) {
							dp[i * nchan] =
								MLIB_S16_MIN;
						} else {
							dp[i * nchan] = lval;
						}
					} else if (buf[i] == 0) {
						dp[i * nchan] = 0;
					} else if (buf[i] > 0) {
						dp[i * nchan] = MLIB_S16_MAX;
					} else {
						dp[i * nchan] = MLIB_S16_MIN;
					}
				}
			}
		}

		sl += slb;
		dl += dlb;
	}
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_i_ImageDivConstShift_U16(
		mlib_image *dst,
		const mlib_image *src,
		const mlib_s32 *consts,
		mlib_s32 shift)
{
	mlib_s32 buf[BUFF_SIZE + 2];
	mlib_u16 *sl, *dl, *sp, *dp;
	mlib_s32 slb, dlb, xsize, ysize, nchan;
	mlib_s32 i, j, k, num, size;

	nchan = mlib_ImageGetChannels(dst);
	xsize = mlib_ImageGetWidth(dst);
	ysize = mlib_ImageGetHeight(dst);
	dlb = mlib_ImageGetStride(dst);
	dl = (void *)mlib_ImageGetData(dst);

	slb = mlib_ImageGetStride(src);
	sl = mlib_ImageGetData(src);

	slb /= 2;
	dlb /= 2;

	for (j = 0; j < ysize; j++) {
		for (k = 0; k < nchan; k++) {
			for (size = 0; size < xsize; size += num) {
				num = xsize - size;

				if (num > BUFF_SIZE)
					num = BUFF_SIZE;

				sp = sl + size * nchan + k;
				dp = dl + size * nchan + k;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i < num; i++) {
					mlib_s64 lval, temp;

					buf[i] = sp[i * nchan];

					if (consts[k] != 0) {
						temp = ((mlib_s64)buf[i]
								<< shift + 16)
								/ consts[k];

						if (temp >= 0) {
							lval = temp >> 16;
						} else {
							lval = -((-temp) >> 16);
						}

						if (lval > MLIB_U16_MAX) {
							dp[i * nchan] =
								MLIB_U16_MAX;
						} else if
							(lval < MLIB_U16_MIN) {
							dp[i * nchan] =
								MLIB_U16_MIN;
						} else {
							dp[i * nchan] = lval;
						}
					} else if (buf[i] == 0) {
						dp[i * nchan] = 0;
					} else {
						dp[i * nchan] = MLIB_U16_MAX;
					}
				}
			}
		}

		sl += slb;
		dl += dlb;
	}

	return (MLIB_SUCCESS);
}


/* *********************************************************** */


mlib_status
mlib_i_ImageDivConstShift_S32(
		mlib_image *dst,
		const mlib_image *src,
		const mlib_s32 *consts,
		mlib_s32 shift)
{
	mlib_s32 *psrc, *pdst;
	mlib_s32 slb, dlb, xsize, ysize, nchan;
	mlib_s32 i, j, k;

	nchan = mlib_ImageGetChannels(dst);
	xsize = mlib_ImageGetWidth(dst);
	ysize = mlib_ImageGetHeight(dst);
	dlb = mlib_ImageGetStride(dst);
	pdst = (void *)mlib_ImageGetData(dst);

	slb = mlib_ImageGetStride(src);
	psrc = mlib_ImageGetData(src);

	for (j = 0; j < ysize; j++) {
		for (k = 0; k < nchan; k++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < xsize; i++) {
				mlib_s64 sd, lval, temp;

				sd = psrc[i * nchan + k];

				if (consts[k] != 0) {
					lval = ((mlib_s64)sd << shift)
						/ consts[k];

					if (lval > MLIB_S32_MAX) {
						pdst[i * nchan + k] =
							MLIB_S32_MAX;
					} else if (lval < MLIB_S32_MIN) {
						pdst[i * nchan + k] =
							MLIB_S32_MIN;
					} else {
						pdst[i * nchan + k] = lval;
					}
				} else if (sd == 0) {
					pdst[i * nchan + k] = 0;
				} else if (sd > 0) {
					pdst[i * nchan + k] = MLIB_S32_MAX;
				} else {
					pdst[i * nchan + k] = MLIB_S32_MIN;
				}
			}
		}

		psrc = (void *)((mlib_u8 *)psrc + slb);
		pdst = (void *)((mlib_u8 *)pdst + dlb);
	}

	return (MLIB_SUCCESS);

}


/* *********************************************************** */
mlib_status
__mlib_ImageDivConstShift_Inp(
    mlib_image *srcdst,
    const mlib_s32 *consts,
    mlib_s32 shift)
{
	return (__mlib_ImageDivConstShift(srcdst, srcdst, consts, shift));
}

/* *********************************************************** */

#else

#include <mlib_image.h>
#include <mlib_ImageCheck.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageDivConstShift = __mlib_ImageDivConstShift
#pragma weak mlib_ImageDivConstShift_Inp = __mlib_ImageDivConstShift_Inp

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_ImageDivConstShift) mlib_ImageDivConstShift
    __attribute__((weak, alias("__mlib_ImageDivConstShift")));
__typeof__(__mlib_ImageDivConstShift_Inp) mlib_ImageDivConstShift_Inp
    __attribute__((weak, alias("__mlib_ImageDivConstShift_Inp")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

mlib_status
__mlib_ImageDivConstShift(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_s32 *consts,
    mlib_s32 shift)
{
	mlib_d64 dalpha[4], dbeta[4], dshift;
	mlib_s32 nchan, k;

	MLIB_IMAGE_CHECK(dst);
	MLIB_IMAGE_CHECK(src);
	MLIB_IMAGE_FULL_EQUAL(dst, src);
	nchan = mlib_ImageGetChannels(dst);

	if (shift < 0 || shift > 31)
		return (MLIB_OUTOFRANGE);

	dshift = (1u << shift);
	for (k = 0; k < nchan; k++) {
		dalpha[k] =
		    (consts[k] == 0) ? 2.0 * MLIB_S32_MAX : dshift / consts[k];
		dbeta[k] = 0;
	}

	return (__mlib_ImageScale2(dst, src, dalpha, dbeta));
}

/* *********************************************************** */

mlib_status
__mlib_ImageDivConstShift_Inp(
    mlib_image *srcdst,
    const mlib_s32 *consts,
    mlib_s32 shift)
{
	return (__mlib_ImageDivConstShift(srcdst, srcdst, consts, shift));
}

#endif
/* *********************************************************** */
