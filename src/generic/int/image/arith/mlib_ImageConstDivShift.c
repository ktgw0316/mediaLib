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

#pragma ident   "@(#)mlib_ImageConstDivShift.c	9.2	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_ImageConstDivShift         - divide an image into constants
 *      mlib_ImageConstDivShift_Inp     - divide an image into constants
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageConstDivShift(mlib_image       *dst,
 *                                          const mlib_image *src,
 *                                          const mlib_s32   *consts,
 *                                          mlib_s32         shift)
 *      mlib_status mlib_ImageConstDivShift_Inp(mlib_image     *srcdst,
 *                                              const mlib_s32 *consts,
 *                                              mlib_s32       shift)
 *
 * ARGUMENT
 *      dst     pointer to the output image
 *      src     pointer to the input image
 *      srcdst  pointer to the input and the output image
 *      consts  array of constants to be divided by each channel of the pixels
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
 *              dst[x][y][c] = consts[c] / src[x][y][c] * 2^shift
 *
 *      For in-place processing version (*_Inp):
 *              srcdst[x][y][c] = consts[c] / srcdst[x][y][c] * 2^shift
 *
 *      If the result of the operation underflows/overflows the
 *      minimum/maximum value supported by the destination image, then it
 *      will be clamped to the minimum/maximum value respectively.
 */

#ifndef _NO_LONGLONG

#include <mlib_image.h>
#include <mlib_ImageCheck.h>
#include <mlib_ImageConstDiv.h>
#include <mlib_i_ImageConstDiv.h>
#include <mlib_i_ImageArith_Div_Tab_U8.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageConstDivShift = __mlib_ImageConstDivShift
#pragma weak mlib_ImageConstDivShift_Inp = __mlib_ImageConstDivShift_Inp

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_ImageConstDivShift) mlib_ImageConstDivShift
    __attribute__((weak, alias("__mlib_ImageConstDivShift")));
__typeof__(__mlib_ImageConstDivShift_Inp) mlib_ImageConstDivShift_Inp
    __attribute__((weak, alias("__mlib_ImageConstDivShift_Inp")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	BUFF_SIZE	256
#define	ZERO_DELTA  1e-16

/* *********************************************************** */

#ifdef MLIB_USE_FTOI_CLAMPING

#define	FLOAT2INT_CLAMP(DST, X)	DST = ((mlib_s32)(X))

#else /* MLIB_USE_FTOI_CLAMPING */

/* *********************************************************** */

#define	FLOAT2INT_CLAMP(DST, X)                                 \
	{                                                       \
	    mlib_d64 sat_s32_tmp = (X);                         \
	                                                        \
	    if (sat_s32_tmp >= MLIB_S32_MAX)                    \
		sat_s32_tmp = MLIB_S32_MAX;                     \
	    if (sat_s32_tmp <= MLIB_S32_MIN)                    \
		sat_s32_tmp = MLIB_S32_MIN;                     \
	    DST = (mlib_s32)sat_s32_tmp;                        \
	}

#endif /* MLIB_USE_FTOI_CLAMPING */

/* *********************************************************** */
#define	SAT_U8(y, x)	\
	if (x > MLIB_U8_MAX) {	\
		y = MLIB_U8_MAX;	\
	} else if (x < 0) {	\
		y = 0;	\
	} else {	\
		y = x;	\
	}

/* *********************************************************** */
#define	SAT_S16(y, x)	\
	if (x > MLIB_S16_MAX) {	\
		y = MLIB_S16_MAX;	\
	} else if (x < MLIB_S16_MIN) {	\
		y = MLIB_S16_MIN;	\
	} else {	\
		y = x;	\
	}

/* *********************************************************** */
#define	SAT_U16(y, x)	\
	if (x > MLIB_U16_MAX) {	\
		y = MLIB_U16_MAX;	\
	} else if (x < MLIB_U16_MIN) {	\
		y = MLIB_U16_MIN;	\
	} else {	\
		y = x;	\
	}

/* *********************************************************** */
#define	SAT_S32(y, x)	\
	if (x > MLIB_S32_MAX) {	\
		y = MLIB_S32_MAX;	\
	} else if (x < MLIB_S32_MIN) {	\
		y = MLIB_S32_MIN;	\
	} else {	\
		y = x;	\
	}

/* ********************************************************** */

mlib_status
__mlib_ImageConstDivShift(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_s32 *consts,
    mlib_s32 shift)
{
	mlib_type type = mlib_ImageGetType(dst);
	mlib_s32 nchan = mlib_ImageGetChannels(dst);
	mlib_s64 ishift, dc[4];
	mlib_s32 k;

	MLIB_IMAGE_CHECK(dst);
	MLIB_IMAGE_CHECK(src);
	MLIB_IMAGE_FULL_EQUAL(dst, src);

	if (shift < 0 || shift > 31)
		return (MLIB_OUTOFRANGE);

	ishift = 1u << shift;

	for (k = 0; k < nchan; k++) {
		dc[k] = consts[k] * ishift;
	}

	switch (type) {
	case MLIB_BYTE:
		mlib_i_ImageConstDiv_U8(dst, src, dc);
		return (MLIB_SUCCESS);

	case MLIB_SHORT:
		mlib_i_ImageConstDiv_S16(dst, src, dc);
		return (MLIB_SUCCESS);

	case MLIB_USHORT:
		mlib_i_ImageConstDiv_U16(dst, src, dc);
		return (MLIB_SUCCESS);

	case MLIB_INT:
		mlib_i_ImageConstDiv_S32(dst, src, dc);
		return (MLIB_SUCCESS);

	default:
		return (MLIB_FAILURE);
	}
}

/* *********************************************************** */

mlib_status
mlib_i_ImageConstDiv_U8(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_s64 *consts)
{
	mlib_s64 ilut[128];
	mlib_u8 *lut[4], *plut = (mlib_u8 *)ilut;
	mlib_s32 nchan;
	mlib_s32 i, j, k;

	nchan = mlib_ImageGetChannels(dst);

	for (j = 0; j < nchan; j++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < 256; i++) {
			mlib_s64 lval, temp;

			if (i != 0) {
				temp = (consts[j] << 1) / i + 1;

				if (temp >= 0) {
					lval = temp >> 1;
				} else {
					lval = -((-temp) >> 1);
				}
				SAT_U8(plut[i], lval);
			} else if (consts[j] == 0) {
				plut[i] = 0;
			} else if (consts[j] > 0) {
				plut[i] = MLIB_U8_MAX;
			} else {
				plut[i] = MLIB_U8_MIN;
			}
		}

		lut[j] = plut;
		plut += 256;
	}

	return (__mlib_ImageLookUp(dst, src, (const void **)lut));
}

/* *********************************************************** */

mlib_status
mlib_i_ImageConstDiv_S16(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_s64 *consts)
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
					mlib_s64 lval;

					buf[i] = sp[i * nchan];

					if (buf[i] != 0) {
						lval = consts[k] / buf[i];
						SAT_S16(dp[i * nchan], lval);
					} else if (consts[k] == 0) {
						dp[i * nchan] = 0;
					} else if (consts[k] > 0) {
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
mlib_i_ImageConstDiv_U16(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_s64 *consts)
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
					mlib_s64 lval;

					buf[i] = sp[i * nchan];

					if (buf[i] != 0) {
						lval = consts[k] / buf[i];
						SAT_U16(dp[i * nchan], lval);
					} else if (consts[k] > 0) {
						dp[i * nchan] = MLIB_U16_MAX;
					} else {
						dp[i * nchan] = MLIB_U16_MIN;
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
mlib_i_ImageConstDiv_S32(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_s64 *consts)
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
				mlib_s64 sd, lval;

				sd = psrc[i * nchan + k];

				if (sd != 0) {
					lval = consts[k] / sd;
					SAT_S32(pdst[i * nchan + k], lval);
				} else if (consts[k] == 0) {
					pdst[i * nchan + k] = 0;
				} else if (consts[k] > 0) {
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
__mlib_ImageConstDivShift_Inp(
    mlib_image *srcdst,
    const mlib_s32 *consts,
    mlib_s32 shift)
{
	return (__mlib_ImageConstDivShift(srcdst, srcdst, consts, shift));
}

/* *********************************************************** */

mlib_status
mlib_ImageConstDiv_U8(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_d64 *consts)
{

	mlib_s64 dlut[128];
	mlib_u8 *lut[4], *plut = (mlib_u8 *)dlut;
	mlib_s32 nchan;
	mlib_f32 da[4], fconst;
	mlib_s64 daa[4], iconst;

	mlib_s32 i, j, k;
	nchan = mlib_ImageGetChannels(dst);

	for (k = 0; k < nchan; k++) {
		if ((consts[k] > MLIB_S32_MAX) ||
				(consts[k] < MLIB_S32_MIN)) {
#ifdef MLIB_USE_FTOI_CLAMPING
			mlib_f32 cc = (mlib_f32)(consts[k] * (1 << 23));
#else /* MLIB_USE_FTOI_CLAMPING */
			mlib_f32 cc = (mlib_f32)(consts[k]);
#endif /* MLIB_USE_FTOI_CLAMPING */

			if (cc <= 0)
				da[k] = 0;
			else
				da[k] = cc;

		} else {
			if (consts[k] >= 0) {
				mlib_s64 cc = (mlib_s64)
					(consts[k] * (1u << 15));
				daa[k] = cc;
			} else {
				daa[k] = 0;
			}
		}
	}


/* lut generation */
	for (j = 0; j < nchan; j++) {
		if ((consts[j] > MLIB_S32_MAX) || (consts[j] < MLIB_S32_MIN)) {
			fconst = da[j];

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < 256; i++) {
#ifdef MLIB_USE_FTOI_CLAMPING
				mlib_f32 fval =
					fconst * mlib_1DIVU8_F32[i]
					+ (mlib_f32)(1 << 22);
				mlib_s32 ival = (mlib_s32)(fval);

				plut[i] = ival >> 23;
#else /* MLIB_USE_FTOI_CLAMPING */
				mlib_f32 fval = fconst * mlib_1DIVU8_F32[i]
					+ 0.5f;

				if (fval >= MLIB_U8_MAX)
					fval = MLIB_U8_MAX;

				plut[i] = (mlib_s32)fval;
#endif /* MLIB_USE_FTOI_CLAMPING */
			}

			lut[j] = plut;
			plut += 256;
		} else {

			for (i = 0; i < 256; i++) {

				if (i != 0) {
					mlib_s64 ival =
						daa[j]
						* mlib_ImageArith_ConstDiv_U8[i]
						+ ((mlib_s64)1u << 30)
						>> 31;
					if (ival >= MLIB_U8_MAX) {
						ival = MLIB_U8_MAX;
					}
					plut[i] = (mlib_s32)ival;
				} else if (consts[j] <= 0) {
					plut[i] = 0;
				} else {
					plut[i] = MLIB_U8_MAX;
				}
			}

			lut[j] = plut;
			plut += 256;
		}
	}
	return (__mlib_ImageLookUp(dst, src, (const void **)lut));
}

/* *********************************************************** */

mlib_status
mlib_ImageConstDiv_S16(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_d64 *consts)
{
	mlib_s32 buf1[BUFF_SIZE + 2];
	mlib_s32 buf2[BUFF_SIZE + 2];
	mlib_s16 *sl, *dl, *sp, *dp;
	mlib_s32 slb, dlb, xsize, ysize, nchan;


#ifdef MLIB_USE_FTOI_CLAMPING
	mlib_f32 da[4];
#endif /* MLIB_USE_FTOI_CLAMPING */
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

#ifdef MLIB_USE_FTOI_CLAMPING
	for (k = 0; k < nchan; k++) {
		da[k] = (mlib_f32)(consts[k] * 65536.0);
	}

#endif /* MLIB_USE_FTOI_CLAMPING */

	for (j = 0; j < ysize; j++) {
		for (k = 0; k < nchan; k++) {
			if ((consts[k] > MLIB_S32_MAX) ||
					(consts[k] < MLIB_S32_MIN)) {
#ifdef MLIB_USE_FTOI_CLAMPING
				mlib_f32 fconst = da[k];
#else /* MLIB_USE_FTOI_CLAMPING */
				mlib_f32 fconst = (mlib_f32)consts[k];
#endif /* MLIB_USE_FTOI_CLAMPING */

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
						mlib_f32 fval;
						mlib_s32 ival;

						buf1[i] = sp[i * nchan];
						fval = fconst
							/ ((mlib_f32)buf1[i]
							+ (mlib_f32)ZERO_DELTA);
#ifdef MLIB_USE_FTOI_CLAMPING
/* 0.5 * (1 << 16) */
						fval = fval + 32768.f;
						ival = (mlib_s32)(fval);
						buf2[i] = ival;
						dp[i * nchan] = buf2[i] >> 16;
#else /* MLIB_USE_FTOI_CLAMPING */

						if (fval >= MLIB_S16_MAX)
							fval = MLIB_S16_MAX;

						if (fval <= MLIB_S16_MIN)
							fval = MLIB_S16_MIN;
						ival = (mlib_s32)(fval);
						buf2[i] = ival;
						dp[i * nchan] = buf2[i];
#endif /* MLIB_USE_FTOI_CLAMPING */
					}
				}
			} else {

				mlib_s32 iconst = (mlib_s32)(consts[k] + 0.5);

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
						mlib_s64 lval;

						buf1[i] = sp[i * nchan];

						if (buf1[i] != 0) {
							lval = iconst / buf1[i];

							if
							(lval > MLIB_S16_MAX) {
								dp[i * nchan] =
								MLIB_S16_MAX;
							} else if
							(lval < MLIB_S16_MIN) {
								dp[i * nchan] =
								MLIB_S16_MIN;
							} else {
								dp[i * nchan] =
									lval;
							}
						} else if (consts[k] == 0) {
							dp[i * nchan] = 0;
						} else if (iconst >= 0) {
							dp[i * nchan] =
								MLIB_S16_MAX;
						} else {
							dp[i * nchan] =
								MLIB_S16_MIN;
						}
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
mlib_ImageConstDiv_U16(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_d64 *consts)
{
	mlib_s32 buf1[BUFF_SIZE + 2];
	mlib_s32 buf2[BUFF_SIZE + 2];
	mlib_u16 *sl, *dl, *sp, *dp;
	mlib_s32 slb, dlb, xsize, ysize, nchan;

#ifdef MLIB_USE_FTOI_CLAMPING
	mlib_f32 da[4];
#endif /* MLIB_USE_FTOI_CLAMPING */
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

#ifdef MLIB_USE_FTOI_CLAMPING
	for (k = 0; k < nchan; k++) {
		da[k] = (mlib_f32)(consts[k] * 65536.0);
	}

#endif /* MLIB_USE_FTOI_CLAMPING */

	for (j = 0; j < ysize; j++) {
		for (k = 0; k < nchan; k++) {

			if ((consts[k] > MLIB_S32_MAX) ||
					(consts[k] < MLIB_S32_MIN)) {
#ifdef MLIB_USE_FTOI_CLAMPING
				mlib_f32 fconst = da[k];
#else /* MLIB_USE_FTOI_CLAMPING */
				mlib_f32 fconst = (mlib_f32)consts[k];
#endif /* MLIB_USE_FTOI_CLAMPING */

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
						mlib_f32 fval;
						mlib_s32 ival;

						buf1[i] = sp[i * nchan];
						fval = fconst
							/ ((mlib_f32)buf1[i]
							+ (mlib_f32)ZERO_DELTA);
#ifdef MLIB_USE_FTOI_CLAMPING
						fval = fval
						- (32767.5 * 65536.0);
						ival = (mlib_s32)(fval);
						buf2[i] = ival;
						dp[i * nchan] =
						(buf2[i] >> 16) ^ 0x8000;
#else /* MLIB_USE_FTOI_CLAMPING */
						fval = fval + 0.5f;

						if (fval >= MLIB_U16_MAX)
							fval = MLIB_U16_MAX;

						if (fval <= MLIB_U16_MIN)
							fval = MLIB_U16_MIN;
						ival = (mlib_s32)(fval);
						buf2[i] = ival;
						dp[i * nchan] = buf2[i];
#endif /* MLIB_USE_FTOI_CLAMPING */
					}
				}
			} else {
				mlib_s32 iconst = (mlib_s32)(consts[k] + 0.5);

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
						mlib_s64 lval;

						buf1[i] = sp[i * nchan];

						if (buf1[i] != 0) {
							lval = iconst / buf1[i];

							if
							(lval > MLIB_U16_MAX) {
								dp[i * nchan] =
								MLIB_U16_MAX;
							} else if
							(lval < MLIB_U16_MIN) {
								dp[i * nchan] =
								MLIB_U16_MIN;
							} else {
								dp[i * nchan] =
									lval;
							}
						} else if (consts[k] == 0) {
							dp[i * nchan] = 0;
						} else if (iconst >= 0) {
							dp[i * nchan] =
								MLIB_U16_MAX;
						} else {
							dp[i * nchan] =
								MLIB_U16_MIN;
						}
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
mlib_ImageConstDiv_S32(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_d64 *consts)
{
	mlib_s32 *psrc, *pdst;
	mlib_s32 slb, dlb, xsize, ysize, nchan;
	mlib_d64 dconst;
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

			if ((consts[k] > MLIB_S32_MAX) ||
					(consts[k] < MLIB_S32_MIN)) {

				dconst = consts[k];
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i < xsize; i++) {
					mlib_d64 sd, r_hi, r_lo;

					sd = psrc[i * nchan + k] + ZERO_DELTA;
					r_hi = (mlib_d64)(1.f / (mlib_f32)sd);
					r_lo = (1.0 - sd * r_hi) * r_hi;
					FLOAT2INT_CLAMP(pdst[i * nchan + k],
							dconst * (r_hi + r_lo));
				}
			} else {

				mlib_s32 iconst = (mlib_s32)(consts[k] + 0.5);

				for (i = 0; i < xsize; i++) {
					mlib_s64 lval;

					if (psrc[i * nchan + k] != 0) {
						lval = iconst
							/ psrc[i * nchan + k];

						if (lval > MLIB_S32_MAX) {
							pdst[i * nchan + k] =
								MLIB_S32_MAX;
						} else if
							(lval < MLIB_S32_MIN) {
							pdst[i * nchan + k] =
								MLIB_S32_MIN;
						} else {
							pdst[i * nchan + k] =
								lval;
						}
					} else if (consts[k] == 0) {
						pdst[i * nchan + k] = 0;
					} else if (iconst >= 0) {
						pdst[i * nchan + k] =
							MLIB_S32_MAX;
					} else {
						pdst[i * nchan + k] =
							MLIB_S32_MIN;
					}
				}
			}
		}
		psrc = (void *)((mlib_u8 *)psrc + slb);
		pdst = (void *)((mlib_u8 *)pdst + dlb);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
#else

#include <mlib_image.h>
#include <mlib_ImageCheck.h>
#include <mlib_ImageConstDiv.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageConstDivShift = __mlib_ImageConstDivShift
#pragma weak mlib_ImageConstDivShift_Inp = __mlib_ImageConstDivShift_Inp

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_ImageConstDivShift) mlib_ImageConstDivShift
    __attribute__((weak, alias("__mlib_ImageConstDivShift")));
__typeof__(__mlib_ImageConstDivShift_Inp) mlib_ImageConstDivShift_Inp
    __attribute__((weak, alias("__mlib_ImageConstDivShift_Inp")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	BUFF_SIZE	256
#define	ZERO_DELTA	1e-16

/* *********************************************************** */

#ifdef MLIB_USE_FTOI_CLAMPING

#define	FLOAT2INT_CLAMP(DST, X)	DST = ((mlib_s32)(X))

#else /* MLIB_USE_FTOI_CLAMPING */

/* *********************************************************** */

#define	FLOAT2INT_CLAMP(DST, X)                                 \
	{                                                       \
	    mlib_d64 sat_s32_tmp = (X);                         \
	                                                        \
	    if (sat_s32_tmp >= MLIB_S32_MAX)                    \
		sat_s32_tmp = MLIB_S32_MAX;                     \
	    if (sat_s32_tmp <= MLIB_S32_MIN)                    \
		sat_s32_tmp = MLIB_S32_MIN;                     \
	    DST = (mlib_s32)sat_s32_tmp;                        \
	}

#endif /* MLIB_USE_FTOI_CLAMPING */

/* *********************************************************** */

mlib_status
__mlib_ImageConstDivShift(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_s32 *consts,
    mlib_s32 shift)
{
	mlib_type type = mlib_ImageGetType(dst);
	mlib_s32 nchan = mlib_ImageGetChannels(dst);
	mlib_d64 dshift, dc[4];
	mlib_s32 k;

	MLIB_IMAGE_CHECK(dst);
	MLIB_IMAGE_CHECK(src);
	MLIB_IMAGE_FULL_EQUAL(dst, src);

	if (shift < 0 || shift > 31)
		return (MLIB_OUTOFRANGE);

	dshift = (mlib_d64)(1u << shift);

	for (k = 0; k < nchan; k++) {
		dc[k] = consts[k] * dshift;
	}

	switch (type) {
	case MLIB_BYTE:
		mlib_ImageConstDiv_U8(dst, src, dc);
		return (MLIB_SUCCESS);

	case MLIB_SHORT:
		mlib_ImageConstDiv_S16(dst, src, dc);
		return (MLIB_SUCCESS);

	case MLIB_USHORT:
		mlib_ImageConstDiv_U16(dst, src, dc);
		return (MLIB_SUCCESS);

	case MLIB_INT:
		mlib_ImageConstDiv_S32(dst, src, dc);
		return (MLIB_SUCCESS);

	default:
		return (MLIB_FAILURE);
	}
}

/* *********************************************************** */

mlib_status
mlib_ImageConstDiv_U8(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_d64 *consts)
{
	mlib_d64 dlut[128];
	mlib_u8 *lut[4], *plut = (mlib_u8 *)dlut;
	mlib_s32 nchan;
	mlib_f32 da[4], fconst;
	mlib_s32 i, j, k;

	nchan = mlib_ImageGetChannels(dst);

	for (k = 0; k < nchan; k++) {
#ifdef MLIB_USE_FTOI_CLAMPING
		mlib_f32 cc = (mlib_f32)(consts[k] * (1 << 23));
#else /* MLIB_USE_FTOI_CLAMPING */
		mlib_f32 cc = (mlib_f32)(consts[k]);
#endif /* MLIB_USE_FTOI_CLAMPING */

		if (cc <= 0)
			da[k] = 0;
		else
			da[k] = cc;
	}

/* lut generation */
	for (j = 0; j < nchan; j++) {
		fconst = da[j];
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < 256; i++) {
#ifdef MLIB_USE_FTOI_CLAMPING
			mlib_f32 fval =
			    fconst * mlib_1DIVU8_F32[i] + (mlib_f32)(1 << 22);
			mlib_s32 ival = (mlib_s32)(fval);

			plut[i] = ival >> 23;
#else /* MLIB_USE_FTOI_CLAMPING */
			mlib_f32 fval = fconst * mlib_1DIVU8_F32[i] + 0.5f;

			if (fval >= MLIB_U8_MAX)
				fval = MLIB_U8_MAX;
			plut[i] = (mlib_s32)fval;
#endif /* MLIB_USE_FTOI_CLAMPING */
		}

		lut[j] = plut;
		plut += 256;
	}

	return (__mlib_ImageLookUp(dst, src, (const void **)lut));
}

/* *********************************************************** */

mlib_status
mlib_ImageConstDiv_S16(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_d64 *consts)
{
	mlib_s32 buf1[BUFF_SIZE + 2];
	mlib_s32 buf2[BUFF_SIZE + 2];
	mlib_s16 *sl, *dl, *sp, *dp;
	mlib_s32 slb, dlb, xsize, ysize, nchan;

#ifdef MLIB_USE_FTOI_CLAMPING
	mlib_f32 da[4];
#endif /* MLIB_USE_FTOI_CLAMPING */
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

#ifdef MLIB_USE_FTOI_CLAMPING
	for (k = 0; k < nchan; k++) {
		da[k] = (mlib_f32)(consts[k] * 65536.0);
	}

#endif /* MLIB_USE_FTOI_CLAMPING */

	for (j = 0; j < ysize; j++) {
		for (k = 0; k < nchan; k++) {
#ifdef MLIB_USE_FTOI_CLAMPING
			mlib_f32 fconst = da[k];
#else /* MLIB_USE_FTOI_CLAMPING */
			mlib_f32 fconst = (mlib_f32)consts[k];
#endif /* MLIB_USE_FTOI_CLAMPING */

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
					mlib_f32 fval;
					mlib_s32 ival;

					buf1[i] = sp[i * nchan];
					fval =
					    fconst / ((mlib_f32)buf1[i] +
					    (mlib_f32)ZERO_DELTA);
#ifdef MLIB_USE_FTOI_CLAMPING
/* 0.5 * (1 << 16) */
					fval = fval + 32768.f;
					ival = (mlib_s32)(fval);
					buf2[i] = ival;
					dp[i * nchan] = buf2[i] >> 16;
#else /* MLIB_USE_FTOI_CLAMPING */

					if (fval >= MLIB_S16_MAX)
						fval = MLIB_S16_MAX;

					if (fval <= MLIB_S16_MIN)
						fval = MLIB_S16_MIN;
					ival = (mlib_s32)(fval);
					buf2[i] = ival;
					dp[i * nchan] = buf2[i];
#endif /* MLIB_USE_FTOI_CLAMPING */
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
mlib_ImageConstDiv_U16(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_d64 *consts)
{
	mlib_s32 buf1[BUFF_SIZE + 2];
	mlib_s32 buf2[BUFF_SIZE + 2];
	mlib_u16 *sl, *dl, *sp, *dp;
	mlib_s32 slb, dlb, xsize, ysize, nchan;

#ifdef MLIB_USE_FTOI_CLAMPING
	mlib_f32 da[4];
#endif /* MLIB_USE_FTOI_CLAMPING */
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

#ifdef MLIB_USE_FTOI_CLAMPING
	for (k = 0; k < nchan; k++) {
		da[k] = (mlib_f32)(consts[k] * 65536.0);
	}

#endif /* MLIB_USE_FTOI_CLAMPING */

	for (j = 0; j < ysize; j++) {
		for (k = 0; k < nchan; k++) {
#ifdef MLIB_USE_FTOI_CLAMPING
			mlib_f32 fconst = da[k];
#else /* MLIB_USE_FTOI_CLAMPING */
			mlib_f32 fconst = (mlib_f32)consts[k];
#endif /* MLIB_USE_FTOI_CLAMPING */

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
					mlib_f32 fval;
					mlib_s32 ival;

					buf1[i] = sp[i * nchan];
					fval =
					    fconst / ((mlib_f32)buf1[i] +
					    (mlib_f32)ZERO_DELTA);
#ifdef MLIB_USE_FTOI_CLAMPING
					fval = fval - (32767.5 * 65536.0);
					ival = (mlib_s32)(fval);
					buf2[i] = ival;
					dp[i * nchan] =
					    (buf2[i] >> 16) ^ 0x8000;
#else /* MLIB_USE_FTOI_CLAMPING */
					fval = fval + 0.5f;

					if (fval >= MLIB_U16_MAX)
						fval = MLIB_U16_MAX;

					if (fval <= MLIB_U16_MIN)
						fval = MLIB_U16_MIN;
					ival = (mlib_s32)(fval);
					buf2[i] = ival;
					dp[i * nchan] = buf2[i];
#endif /* MLIB_USE_FTOI_CLAMPING */
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
mlib_ImageConstDiv_S32(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_d64 *consts)
{
	mlib_s32 *psrc, *pdst;
	mlib_s32 slb, dlb, xsize, ysize, nchan;
	mlib_d64 dconst;
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
			dconst = consts[k];
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < xsize; i++) {
				mlib_d64 sd, r_hi, r_lo;

				sd = psrc[i * nchan + k] + ZERO_DELTA;
				r_hi = (mlib_d64)(1.f / (mlib_f32)sd);
				r_lo = (1.0 - sd * r_hi) * r_hi;
				FLOAT2INT_CLAMP(pdst[i * nchan + k],
				    dconst * (r_hi + r_lo));
			}
		}

		psrc = (void *)((mlib_u8 *)psrc + slb);
		pdst = (void *)((mlib_u8 *)pdst + dlb);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_ImageConstDivShift_Inp(
    mlib_image *srcdst,
    const mlib_s32 *consts,
    mlib_s32 shift)
{
	return (__mlib_ImageConstDivShift(srcdst, srcdst, consts, shift));
}

#endif
/* *********************************************************** */
