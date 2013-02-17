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

#pragma ident   "@(#)mlib_i_ImageConstSub.c	9.2    07/11/05"

/*
 * FUNCTION
 *      mlib_ImageConstSub - subtraction image from constant
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageConstSub(mlib_image *dst,
 *                                     mlib_image *src,
 *                                     mlib_s32   *c);
 *
 * ARGUMENT
 *      src     pointer to input image
 *      dst     pointer to output image
 *      c       array of constants each pixel to be subtracted from
 *
 * RESTRICTION
 *      src, and dst must be the same type, the same size and the same
 *      number of channels.
 *      They can have 1, 2, 3 or 4 channels. They can be in MLIB_BYTE,
 *      MLIB_SHORT, MLIB_USHORT or MLIB_INT data type.
 *
 * DESCRIPTION
 *      dst(i, j) = c - src(i, j).
 */

#include <mlib_image.h>
#include <mlib_ImageCheck.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageConstSub = __mlib_ImageConstSub

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_ImageConstSub) mlib_ImageConstSub
    __attribute__((weak, alias("__mlib_ImageConstSub")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

static mlib_status mlib_i_ImageConstSub_U8(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_s32 *c);

static mlib_status mlib_i_ImageConstSub_S16(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_s32 *c);

static mlib_status mlib_i_ImageConstSub_U16(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_s32 *c);

static mlib_status mlib_i_ImageConstSub_S32(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_s32 *c);

static mlib_status mlib_c_ImageConstSub_U8(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_s32 *c);

static mlib_status mlib_c_ImageConstSub_S16(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_s32 *c);

static mlib_status mlib_c_ImageConstSub_U16(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_s32 *c);

static mlib_status mlib_c_ImageConstSub_S32(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_s32 *c);

/* *********************************************************** */
#ifndef _NO_LONGLONG

mlib_status
__mlib_ImageConstSub(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_s32 *c)
{
	mlib_type dtype;

	MLIB_IMAGE_CHECK(dst);
	MLIB_IMAGE_CHECK(src);
	MLIB_IMAGE_FULL_EQUAL(dst, src);

	if (c == NULL)
		return (MLIB_FAILURE);

	dtype = mlib_ImageGetType(dst);

	if (dtype == MLIB_BYTE) {

		return (mlib_i_ImageConstSub_U8(dst, src, c));

	} else if (dtype == MLIB_SHORT) {

		return (mlib_i_ImageConstSub_S16(dst, src, c));

	} else if (dtype == MLIB_USHORT) {

		return (mlib_i_ImageConstSub_U16(dst, src, c));

	} else if (dtype == MLIB_INT) {

		return (mlib_i_ImageConstSub_S32(dst, src, c));
	}

	return (MLIB_FAILURE);
}

/* *********************************************************** */

mlib_status
mlib_i_ImageConstSub_U8(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_s32 *c)
{
	mlib_s64 ilut[96], dff;
	mlib_u8 *lut[4];
	mlib_s32 i, nchan = mlib_ImageGetChannels(src);

#ifdef _LITTLE_ENDIAN
	mlib_u32 sum = 0xfcfdfeff;
#else /* _LITTLE_ENDIAN */
	mlib_u32 sum = 0xfffefdfc;
#endif /* _LITTLE_ENDIAN */

/* lut generation */
	((mlib_s32 *)ilut)[0] = ((mlib_s32 *)ilut)[1] = 0xffffffff;
	dff = ilut[0];
/* lut[  0 - 255] = 255 */
	for (i = 1; i < 32; i++)
		ilut[i] = dff;
/* lut[512 - 767] = 0 */
	for (i = 64; i < 96; i++)
		ilut[i] = 0;
/* lut[256 - 511] = 255 - i */
	for (i = 64; i < 128; i++) {
		((mlib_s32 *)ilut)[i] = sum;
		sum -= 0x4040404;
	}

	for (i = 0; i < nchan; i++) {
		if (c[i] >= 510)
			lut[i] = (mlib_u8 *)ilut;
		else if (c[i] <= 0)
			lut[i] = (mlib_u8 *)(ilut + 64);
		else
			lut[i] = 511 - c[i] + (mlib_u8 *)ilut;
	}

	return (__mlib_ImageLookUp(dst, src, (const void **)lut));
}

/* *********************************************************** */

mlib_status
mlib_i_ImageConstSub_S16(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_s32 *c)
{
	mlib_s16 *psrc, *pdst, con0 = c[0], con1, con2, con3;
	mlib_s32 slb, dlb, xsize, ysize, nchan;
	mlib_s32 i, j;

	nchan = mlib_ImageGetChannels(dst);
	xsize = mlib_ImageGetWidth(dst);
	ysize = mlib_ImageGetHeight(dst);
	dlb = mlib_ImageGetStride(dst);
	pdst = (void *)mlib_ImageGetData(dst);

	slb = mlib_ImageGetStride(src);
	psrc = mlib_ImageGetData(src);

	slb >>= 1;
	dlb >>= 1;

/* if src and dst - 1-D vector */
	if (xsize * nchan == slb && slb == dlb) {
		xsize *= ysize;
		ysize = 1;
	}

	if (nchan == 1) {
		for (j = 0; j < ysize; j++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < xsize; i++) {
				pdst[i] = con0 - psrc[i];
			}

			psrc += slb;
			pdst += dlb;
		}
	} else {
		con1 = c[1];

		if (nchan == 2) {
			for (j = 0; j < ysize; j++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i < xsize; i++) {
					pdst[2 * i] = con0 - psrc[2 * i];
					pdst[2 * i + 1] =
					    con1 - psrc[2 * i + 1];
				}

				psrc += slb;
				pdst += dlb;
			}
		} else {
			con2 = c[2];

			if (nchan == 3) {
				for (j = 0; j < ysize; j++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
					for (i = 0; i < xsize; i++) {
						pdst[3 * i] =
						    con0 - psrc[3 * i];
						pdst[3 * i + 1] =
						    con1 - psrc[3 * i + 1];
						pdst[3 * i + 2] =
						    con2 - psrc[3 * i + 2];
					}

					psrc += slb;
					pdst += dlb;
				}
			} else {
				con3 = c[3];
				for (j = 0; j < ysize; j++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
					for (i = 0; i < xsize; i++) {
						pdst[4 * i] =
						    con0 - psrc[4 * i];
						pdst[4 * i + 1] =
						    con1 - psrc[4 * i + 1];
						pdst[4 * i + 2] =
						    con2 - psrc[4 * i + 2];
						pdst[4 * i + 3] =
						    con3 - psrc[4 * i + 3];
					}

					psrc += slb;
					pdst += dlb;
				}
			}
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#define	SUB_SAT_U16(dst, a, b)                                  \
	sum = (mlib_s32)(a) - (mlib_s32)(b);                    \
	dst = ((sum | ((sum << 15) >> 31)) & ~(sum >> 17))

/* *********************************************************** */

mlib_status
mlib_i_ImageConstSub_U16(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_s32 *c)
{
	mlib_u16 *psrc, *pdst;
	mlib_s32 con0, con1, con2, con3, sum;
	mlib_s32 slb, dlb, xsize, ysize, nchan;
	mlib_s32 i, j;

	nchan = mlib_ImageGetChannels(dst);
	xsize = mlib_ImageGetWidth(dst);
	ysize = mlib_ImageGetHeight(dst);
	dlb = mlib_ImageGetStride(dst);
	pdst = (void *)mlib_ImageGetData(dst);

	slb = mlib_ImageGetStride(src);
	psrc = mlib_ImageGetData(src);

	slb >>= 1;
	dlb >>= 1;

	con0 = c[0];

	if (con0 > 2 * MLIB_U16_MAX)
		con0 = 2 * MLIB_U16_MAX;
	if (con0 < MLIB_U16_MIN)
		con0 = MLIB_U16_MIN;

/* if src and dst - 1-D vector */
	if (xsize * nchan == slb && slb == dlb) {
		xsize *= ysize;
		ysize = 1;
	}

	if (nchan == 1) {
		for (j = 0; j < ysize; j++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < xsize; i++) {
				SUB_SAT_U16(pdst[i], con0, psrc[i]);
			}

			psrc += slb;
			pdst += dlb;
		}
	} else {
		con1 = c[1];

		if (con1 > 2 * MLIB_U16_MAX)
			con1 = 2 * MLIB_U16_MAX;
		if (con1 < MLIB_U16_MIN)
			con1 = MLIB_U16_MIN;
		if (nchan == 2) {
			for (j = 0; j < ysize; j++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i < xsize; i++) {
					SUB_SAT_U16(pdst[2 * i], con0,
					    psrc[2 * i]);
					SUB_SAT_U16(pdst[2 * i + 1], con1,
					    psrc[2 * i + 1]);
				}

				psrc += slb;
				pdst += dlb;
			}
		} else {
			con2 = c[2];

			if (con2 > 2 * MLIB_U16_MAX)
				con2 = 2 * MLIB_U16_MAX;
			if (con2 < MLIB_U16_MIN)
				con2 = MLIB_U16_MIN;
			if (nchan == 3) {
				for (j = 0; j < ysize; j++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
					for (i = 0; i < xsize; i++) {
						SUB_SAT_U16(pdst[3 * i], con0,
						    psrc[3 * i]);
						SUB_SAT_U16(pdst[3 * i + 1],
						    con1, psrc[3 * i + 1]);
						SUB_SAT_U16(pdst[3 * i + 2],
						    con2, psrc[3 * i + 2]);
					}

					psrc += slb;
					pdst += dlb;
				}
			} else {
				con3 = c[3];

				if (con3 > 2 * MLIB_U16_MAX)
					con3 = 2 * MLIB_U16_MAX;
				if (con3 < MLIB_U16_MIN)
					con3 = MLIB_U16_MIN;
				for (j = 0; j < ysize; j++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
					for (i = 0; i < xsize; i++) {
						SUB_SAT_U16(pdst[4 * i], con0,
						    psrc[4 * i]);
						SUB_SAT_U16(pdst[4 * i + 1],
						    con1, psrc[4 * i + 1]);
						SUB_SAT_U16(pdst[4 * i + 2],
						    con2, psrc[4 * i + 2]);
						SUB_SAT_U16(pdst[4 * i + 3],
						    con3, psrc[4 * i + 3]);
					}

					psrc += slb;
					pdst += dlb;
				}
			}
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_i_ImageConstSub_S32(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_s32 *c)
{
	mlib_s32 *psrc, *pdst, con0 = c[0], con1, con2, con3;
	mlib_s32 slb, dlb, xsize, ysize, nchan;
	mlib_s32 i, j;

	nchan = mlib_ImageGetChannels(dst);
	xsize = mlib_ImageGetWidth(dst);
	ysize = mlib_ImageGetHeight(dst);
	dlb = mlib_ImageGetStride(dst);
	pdst = (void *)mlib_ImageGetData(dst);

	slb = mlib_ImageGetStride(src);
	psrc = mlib_ImageGetData(src);

	slb >>= 2;
	dlb >>= 2;

/* if src and dst - 1-D vector */
	if (xsize * nchan == slb && slb == dlb) {
		xsize *= ysize;
		ysize = 1;
	}

	if (nchan == 1) {
		for (j = 0; j < ysize; j++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < xsize; i++) {
				pdst[i] = con0 - psrc[i];
			}

			psrc += slb;
			pdst += dlb;
		}
	} else {
		con1 = c[1];

		if (nchan == 2) {
			for (j = 0; j < ysize; j++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i < xsize; i++) {
					pdst[2 * i] = con0 - psrc[2 * i];
					pdst[2 * i + 1] =
					    con1 - psrc[2 * i + 1];
				}

				psrc += slb;
				pdst += dlb;
			}
		} else {
			con2 = c[2];

			if (nchan == 3) {
				for (j = 0; j < ysize; j++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
					for (i = 0; i < xsize; i++) {
						pdst[3 * i] =
						    con0 - psrc[3 * i];
						pdst[3 * i + 1] =
						    con1 - psrc[3 * i + 1];
						pdst[3 * i + 2] =
						    con2 - psrc[3 * i + 2];
					}

					psrc += slb;
					pdst += dlb;
				}
			} else {
				con3 = c[3];
				for (j = 0; j < ysize; j++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
					for (i = 0; i < xsize; i++) {
						pdst[4 * i] =
						    con0 - psrc[4 * i];
						pdst[4 * i + 1] =
						    con1 - psrc[4 * i + 1];
						pdst[4 * i + 2] =
						    con2 - psrc[4 * i + 2];
						pdst[4 * i + 3] =
						    con3 - psrc[4 * i + 3];
					}

					psrc += slb;
					pdst += dlb;
				}
			}
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
#else

mlib_status
__mlib_ImageConstSub(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_s32 *c)
{
	mlib_type dtype;

	MLIB_IMAGE_CHECK(dst);
	MLIB_IMAGE_CHECK(src);
	MLIB_IMAGE_FULL_EQUAL(dst, src);

	if (c == NULL)
		return (MLIB_FAILURE);

	dtype = mlib_ImageGetType(dst);

	if (dtype == MLIB_BYTE) {

		return (mlib_c_ImageConstSub_U8(dst, src, c));

	} else if (dtype == MLIB_SHORT) {

		return (mlib_c_ImageConstSub_S16(dst, src, c));

	} else if (dtype == MLIB_USHORT) {

		return (mlib_c_ImageConstSub_U16(dst, src, c));

	} else if (dtype == MLIB_INT) {

		return (mlib_c_ImageConstSub_S32(dst, src, c));
	}

	return (MLIB_FAILURE);
}

/* *********************************************************** */

mlib_status
mlib_c_ImageConstSub_U8(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_s32 *c)
{
	mlib_d64 dlut[96], dff;
	mlib_u8 *lut[4];
	mlib_s32 i, nchan = mlib_ImageGetChannels(src);

#ifdef _LITTLE_ENDIAN
	mlib_u32 sum = 0xfcfdfeff;
#else /* _LITTLE_ENDIAN */
	mlib_u32 sum = 0xfffefdfc;
#endif /* _LITTLE_ENDIAN */

/* lut generation */
	((mlib_s32 *)dlut)[0] = ((mlib_s32 *)dlut)[1] = 0xffffffff;
	dff = dlut[0];
/* lut[  0 - 255] = 255 */
	for (i = 1; i < 32; i++)
		dlut[i] = dff;
/* lut[512 - 767] = 0 */
	for (i = 64; i < 96; i++)
		dlut[i] = 0;
/* lut[256 - 511] = 255 - i */
	for (i = 64; i < 128; i++) {
		((mlib_s32 *)dlut)[i] = sum;
		sum -= 0x4040404;
	}

	for (i = 0; i < nchan; i++) {
		if (c[i] >= 510)
			lut[i] = (mlib_u8 *)dlut;
		else if (c[i] <= 0)
			lut[i] = (mlib_u8 *)(dlut + 64);
		else
			lut[i] = 511 - c[i] + (mlib_u8 *)dlut;
	}

	return (__mlib_ImageLookUp(dst, src, (const void **)lut));
}

/* *********************************************************** */

mlib_status
mlib_c_ImageConstSub_S16(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_s32 *c)
{
	mlib_s16 *psrc, *pdst, con0 = c[0], con1, con2, con3;
	mlib_s32 slb, dlb, xsize, ysize, nchan;
	mlib_s32 i, j;

	nchan = mlib_ImageGetChannels(dst);
	xsize = mlib_ImageGetWidth(dst);
	ysize = mlib_ImageGetHeight(dst);
	dlb = mlib_ImageGetStride(dst);
	pdst = (void *)mlib_ImageGetData(dst);

	slb = mlib_ImageGetStride(src);
	psrc = mlib_ImageGetData(src);

	slb >>= 1;
	dlb >>= 1;

/* if src and dst - 1-D vector */
	if (xsize * nchan == slb && slb == dlb) {
		xsize *= ysize;
		ysize = 1;
	}

	if (nchan == 1) {
		for (j = 0; j < ysize; j++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < xsize; i++) {
				pdst[i] = con0 - psrc[i];
			}

			psrc += slb;
			pdst += dlb;
		}
	} else {
		con1 = c[1];

		if (nchan == 2) {
			for (j = 0; j < ysize; j++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i < xsize; i++) {
					pdst[2 * i] = con0 - psrc[2 * i];
					pdst[2 * i + 1] =
					    con1 - psrc[2 * i + 1];
				}

				psrc += slb;
				pdst += dlb;
			}
		} else {
			con2 = c[2];

			if (nchan == 3) {
				for (j = 0; j < ysize; j++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
					for (i = 0; i < xsize; i++) {
						pdst[3 * i] =
						    con0 - psrc[3 * i];
						pdst[3 * i + 1] =
						    con1 - psrc[3 * i + 1];
						pdst[3 * i + 2] =
						    con2 - psrc[3 * i + 2];
					}

					psrc += slb;
					pdst += dlb;
				}
			} else {
				con3 = c[3];
				for (j = 0; j < ysize; j++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
					for (i = 0; i < xsize; i++) {
						pdst[4 * i] =
						    con0 - psrc[4 * i];
						pdst[4 * i + 1] =
						    con1 - psrc[4 * i + 1];
						pdst[4 * i + 2] =
						    con2 - psrc[4 * i + 2];
						pdst[4 * i + 3] =
						    con3 - psrc[4 * i + 3];
					}

					psrc += slb;
					pdst += dlb;
				}
			}
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#define	SUB_SAT_U16(dst, a, b)                                  \
	sum = (mlib_s32)(a) - (mlib_s32)(b);                    \
	dst = ((sum | ((sum << 15) >> 31)) & ~(sum >> 17))

/* *********************************************************** */

mlib_status
mlib_c_ImageConstSub_U16(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_s32 *c)
{
	mlib_u16 *psrc, *pdst;
	mlib_s32 con0, con1, con2, con3, sum;
	mlib_s32 slb, dlb, xsize, ysize, nchan;
	mlib_s32 i, j;

	nchan = mlib_ImageGetChannels(dst);
	xsize = mlib_ImageGetWidth(dst);
	ysize = mlib_ImageGetHeight(dst);
	dlb = mlib_ImageGetStride(dst);
	pdst = (void *)mlib_ImageGetData(dst);

	slb = mlib_ImageGetStride(src);
	psrc = mlib_ImageGetData(src);

	slb >>= 1;
	dlb >>= 1;

	con0 = c[0];

	if (con0 > 2 * MLIB_U16_MAX)
		con0 = 2 * MLIB_U16_MAX;
	if (con0 < MLIB_U16_MIN)
		con0 = MLIB_U16_MIN;

/* if src and dst - 1-D vector */
	if (xsize * nchan == slb && slb == dlb) {
		xsize *= ysize;
		ysize = 1;
	}

	if (nchan == 1) {
		for (j = 0; j < ysize; j++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < xsize; i++) {
				SUB_SAT_U16(pdst[i], con0, psrc[i]);
			}

			psrc += slb;
			pdst += dlb;
		}
	} else {
		con1 = c[1];

		if (con1 > 2 * MLIB_U16_MAX)
			con1 = 2 * MLIB_U16_MAX;
		if (con1 < MLIB_U16_MIN)
			con1 = MLIB_U16_MIN;
		if (nchan == 2) {
			for (j = 0; j < ysize; j++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i < xsize; i++) {
					SUB_SAT_U16(pdst[2 * i], con0,
					    psrc[2 * i]);
					SUB_SAT_U16(pdst[2 * i + 1], con1,
					    psrc[2 * i + 1]);
				}

				psrc += slb;
				pdst += dlb;
			}
		} else {
			con2 = c[2];

			if (con2 > 2 * MLIB_U16_MAX)
				con2 = 2 * MLIB_U16_MAX;
			if (con2 < MLIB_U16_MIN)
				con2 = MLIB_U16_MIN;
			if (nchan == 3) {
				for (j = 0; j < ysize; j++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
					for (i = 0; i < xsize; i++) {
						SUB_SAT_U16(pdst[3 * i], con0,
						    psrc[3 * i]);
						SUB_SAT_U16(pdst[3 * i + 1],
						    con1, psrc[3 * i + 1]);
						SUB_SAT_U16(pdst[3 * i + 2],
						    con2, psrc[3 * i + 2]);
					}

					psrc += slb;
					pdst += dlb;
				}
			} else {
				con3 = c[3];

				if (con3 > 2 * MLIB_U16_MAX)
					con3 = 2 * MLIB_U16_MAX;
				if (con3 < MLIB_U16_MIN)
					con3 = MLIB_U16_MIN;
				for (j = 0; j < ysize; j++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
					for (i = 0; i < xsize; i++) {
						SUB_SAT_U16(pdst[4 * i], con0,
						    psrc[4 * i]);
						SUB_SAT_U16(pdst[4 * i + 1],
						    con1, psrc[4 * i + 1]);
						SUB_SAT_U16(pdst[4 * i + 2],
						    con2, psrc[4 * i + 2]);
						SUB_SAT_U16(pdst[4 * i + 3],
						    con3, psrc[4 * i + 3]);
					}

					psrc += slb;
					pdst += dlb;
				}
			}
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_c_ImageConstSub_S32(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_s32 *c)
{
	mlib_s32 *psrc, *pdst, con0 = c[0], con1, con2, con3;
	mlib_s32 slb, dlb, xsize, ysize, nchan;
	mlib_s32 i, j;

	nchan = mlib_ImageGetChannels(dst);
	xsize = mlib_ImageGetWidth(dst);
	ysize = mlib_ImageGetHeight(dst);
	dlb = mlib_ImageGetStride(dst);
	pdst = (void *)mlib_ImageGetData(dst);

	slb = mlib_ImageGetStride(src);
	psrc = mlib_ImageGetData(src);

	slb >>= 2;
	dlb >>= 2;

/* if src and dst - 1-D vector */
	if (xsize * nchan == slb && slb == dlb) {
		xsize *= ysize;
		ysize = 1;
	}

	if (nchan == 1) {
		for (j = 0; j < ysize; j++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < xsize; i++) {
				pdst[i] = con0 - psrc[i];
			}

			psrc += slb;
			pdst += dlb;
		}
	} else {
		con1 = c[1];

		if (nchan == 2) {
			for (j = 0; j < ysize; j++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i < xsize; i++) {
					pdst[2 * i] = con0 - psrc[2 * i];
					pdst[2 * i + 1] =
					    con1 - psrc[2 * i + 1];
				}

				psrc += slb;
				pdst += dlb;
			}
		} else {
			con2 = c[2];

			if (nchan == 3) {
				for (j = 0; j < ysize; j++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
					for (i = 0; i < xsize; i++) {
						pdst[3 * i] =
						    con0 - psrc[3 * i];
						pdst[3 * i + 1] =
						    con1 - psrc[3 * i + 1];
						pdst[3 * i + 2] =
						    con2 - psrc[3 * i + 2];
					}

					psrc += slb;
					pdst += dlb;
				}
			} else {
				con3 = c[3];
				for (j = 0; j < ysize; j++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
					for (i = 0; i < xsize; i++) {
						pdst[4 * i] =
						    con0 - psrc[4 * i];
						pdst[4 * i + 1] =
						    con1 - psrc[4 * i + 1];
						pdst[4 * i + 2] =
						    con2 - psrc[4 * i + 2];
						pdst[4 * i + 3] =
						    con3 - psrc[4 * i + 3];
					}

					psrc += slb;
					pdst += dlb;
				}
			}
		}
	}

	return (MLIB_SUCCESS);
}

#endif
/* *********************************************************** */
