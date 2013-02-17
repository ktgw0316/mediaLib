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

#pragma ident	"@(#)mlib_ImageConstDiv_Fp.c	9.3	07/10/09 SMI"

/*
 * FUNCTION
 *      mlib_ImageConstDiv_Fp           - divide an image into constants
 *      mlib_ImageConstDiv_Fp_Inp       - divide an image into constants
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageConstDiv_Fp(mlib_image       *dst,
 *                                        const mlib_image *src,
 *                                        const mlib_d64   *consts)
 *      mlib_status mlib_ImageConstDiv_Fp_Inp(mlib_image     *srcdst,
 *                                            const mlib_d64 *consts)
 *
 * ARGUMENT
 *      src     pointer to the input image
 *      dst     pointer to the output image
 *      srcdst  pointer to the input and the output image
 *      consts  array of constants to be divided by each channel of the pixels
 *
 * RESTRICTION
 *      The images must have the same type, the same size, and the same number
 *      of channels.
 *      The images can have 1, 2, 3, or 4 channels.
 *      The images can be in MLIB_FLOAT or MLIB_DOUBLE data type.
 *
 *      For MLIB_DOUBLE type, the exponent must be in mlib_f32 range.
 *
 * DESCRIPTION
 *      For the general version:
 *              dst[x][y][c] = consts[c] / src[x][y][c]
 *
 *      For in-place processing version (*_Inp):
 *              srcdst[x][y][c] = consts[c] / srcdst[x][y][c]
 *
 *      If the result of the operation underflows/overflows the
 *      minimum/maximum value supported by the destination image, then it
 *      will be clamped to the minimum/maximum value respectively.
 */

#include <mlib_image.h>
#include <mlib_ImageCheck.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageConstDiv_Fp = __mlib_ImageConstDiv_Fp
#pragma weak mlib_ImageConstDiv_Fp_Inp = __mlib_ImageConstDiv_Fp_Inp

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_ImageConstDiv_Fp) mlib_ImageConstDiv_Fp
    __attribute__((weak, alias("__mlib_ImageConstDiv_Fp")));
__typeof__(__mlib_ImageConstDiv_Fp_Inp) mlib_ImageConstDiv_Fp_Inp
    __attribute__((weak, alias("__mlib_ImageConstDiv_Fp_Inp")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	ARITH_FUN(x, c)	((c) / (x))

/* *********************************************************** */

static mlib_status mlib_ImageConstDiv_F32(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_d64 *consts);

static mlib_status mlib_ImageConstDiv_D64(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_d64 *consts);

/* *********************************************************** */

mlib_status
__mlib_ImageConstDiv_Fp(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_d64 *consts)
{
	mlib_type type = mlib_ImageGetType(dst);

	MLIB_IMAGE_CHECK(dst);
	MLIB_IMAGE_CHECK(src);
	MLIB_IMAGE_FULL_EQUAL(dst, src);

	switch (type) {
	case MLIB_FLOAT:
		mlib_ImageConstDiv_F32(dst, src, consts);
		return (MLIB_SUCCESS);

	case MLIB_DOUBLE:
		mlib_ImageConstDiv_D64(dst, src, consts);
		return (MLIB_SUCCESS);

	default:
		return (MLIB_FAILURE);
	}
}

/* *********************************************************** */

mlib_status
mlib_ImageConstDiv_F32(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_d64 *consts)
{
	mlib_f32 *psrc, *pdst;
	mlib_s32 slb, dlb, xsize, ysize, nchan;
	mlib_f32 dalpha;
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
			dalpha = (mlib_f32)consts[k];
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < xsize; i++) {
				pdst[i * nchan + k] =
				    ARITH_FUN(psrc[i * nchan + k], dalpha);
			}
		}

		psrc = (void *)((mlib_u8 *)psrc + slb);
		pdst = (void *)((mlib_u8 *)pdst + dlb);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_ImageConstDiv_D64(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_d64 *consts)
{
	mlib_d64 *psrc, *pdst;
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
				pdst[i * nchan + k] =
				    ARITH_FUN(psrc[i * nchan + k], dconst);
			}
		}

		psrc = (void *)((mlib_u8 *)psrc + slb);
		pdst = (void *)((mlib_u8 *)pdst + dlb);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_ImageConstDiv_Fp_Inp(
    mlib_image *srcdst,
    const mlib_d64 *consts)
{
	return (__mlib_ImageConstDiv_Fp(srcdst, srcdst, consts));
}

/* *********************************************************** */
