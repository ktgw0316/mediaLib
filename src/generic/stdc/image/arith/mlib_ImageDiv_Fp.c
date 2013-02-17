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

#pragma ident	"@(#)mlib_ImageDiv_Fp.c	9.3	07/10/09 SMI"

/*
 * FUNCTION
 *      mlib_ImageDiv_Fp        - divide one image by another
 *      mlib_ImageDiv1_Fp_Inp   - divide one image by another
 *      mlib_ImageDiv2_Fp_Inp   - divide one image by another
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageDiv_Fp(mlib_image *dst,
 *                                   const mlib_image *src1,
 *                                   const mlib_image *src2)
 *      mlib_status mlib_ImageDiv1_Fp_Inp(mlib_image *src1dst,
 *                                        const mlib_image *src2)
 *      mlib_status mlib_ImageDiv2_Fp_Inp(mlib_image *src2dst,
 *                                        const mlib_image *src1)
 *
 * ARGUMENT
 *      src1     pointer to the first input image
 *      src2     pointer to the second input image
 *      dst      pointer to the output image
 *      src1dst  pointer to the first input and the output image
 *      src2dst  pointer to the second input and the output image
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
 *              dst[x][y][c] = src1[x][y][c] / src2[x][y][c]
 *
 *      For in-place processing versions:
 *          mlib_ImageDiv1_Fp_Inp:
 *              src1dst[x][y][c] = src1dst[x][y][c] / src2[x][y][c]
 *          mlib_ImageDiv2_Fp_Inp:
 *              src2dst[x][y][c] = src1[x][y][c] / src2dst[x][y][c]
 *
 *      The operation follows the IEEE-754 standard.
 */

#include <mlib_image.h>
#include <mlib_ImageCheck.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageDiv1_Fp_Inp = __mlib_ImageDiv1_Fp_Inp
#pragma weak mlib_ImageDiv2_Fp_Inp = __mlib_ImageDiv2_Fp_Inp
#pragma weak mlib_ImageDiv_Fp = __mlib_ImageDiv_Fp

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_ImageDiv1_Fp_Inp) mlib_ImageDiv1_Fp_Inp
    __attribute__((weak, alias("__mlib_ImageDiv1_Fp_Inp")));
__typeof__(__mlib_ImageDiv2_Fp_Inp) mlib_ImageDiv2_Fp_Inp
    __attribute__((weak, alias("__mlib_ImageDiv2_Fp_Inp")));
__typeof__(__mlib_ImageDiv_Fp) mlib_ImageDiv_Fp
    __attribute__((weak, alias("__mlib_ImageDiv_Fp")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

static mlib_status mlib_ImageDiv_Fp_F32(
    mlib_image *dst,
    const mlib_image *src1,
    const mlib_image *src2);

static mlib_status mlib_ImageDiv_Fp_D64(
    mlib_image *dst,
    const mlib_image *src1,
    const mlib_image *src2);

/* *********************************************************** */

mlib_status
__mlib_ImageDiv_Fp(
    mlib_image *dst,
    const mlib_image *src1,
    const mlib_image *src2)
{
	mlib_type type = mlib_ImageGetType(dst);

	MLIB_IMAGE_CHECK(dst);
	MLIB_IMAGE_CHECK(src1);
	MLIB_IMAGE_CHECK(src2);
	MLIB_IMAGE_FULL_EQUAL(dst, src1);
	MLIB_IMAGE_FULL_EQUAL(dst, src2);

	switch (type) {
	case MLIB_FLOAT:
		mlib_ImageDiv_Fp_F32(dst, src1, src2);
		return (MLIB_SUCCESS);

	case MLIB_DOUBLE:
		mlib_ImageDiv_Fp_D64(dst, src1, src2);
		return (MLIB_SUCCESS);

	default:
		return (MLIB_FAILURE);
	}
}

/* *********************************************************** */

mlib_status
mlib_ImageDiv_Fp_F32(
    mlib_image *dst,
    const mlib_image *src1,
    const mlib_image *src2)
{
	mlib_f32 *dl, *sl1, *sl2;
	mlib_s32 slb1, slb2, dlb, xsize, ysize, nchan;
	mlib_s32 i, j;

	nchan = mlib_ImageGetChannels(dst);
	xsize = mlib_ImageGetWidth(dst);
	ysize = mlib_ImageGetHeight(dst);
	dlb = mlib_ImageGetStride(dst);
	dl = (void *)mlib_ImageGetData(dst);

	dlb /= 4;
	slb1 = mlib_ImageGetStride(src1) / 4;
	sl1 = mlib_ImageGetData(src1);
	slb2 = mlib_ImageGetStride(src2) / 4;
	sl2 = mlib_ImageGetData(src2);
	xsize *= nchan;

	for (j = 0; j < ysize; j++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < xsize; i++) {
			dl[i] = sl1[i] / sl2[i];
		}

		dl += dlb;
		sl1 += slb1;
		sl2 += slb2;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_ImageDiv_Fp_D64(
    mlib_image *dst,
    const mlib_image *src1,
    const mlib_image *src2)
{
	mlib_d64 *dl, *sl1, *sl2;
	mlib_s32 slb1, slb2, dlb, xsize, ysize, nchan;
	mlib_s32 i, j;

	nchan = mlib_ImageGetChannels(dst);
	xsize = mlib_ImageGetWidth(dst);
	ysize = mlib_ImageGetHeight(dst);
	dlb = mlib_ImageGetStride(dst);
	dl = (void *)mlib_ImageGetData(dst);

	dlb /= 8;
	slb1 = mlib_ImageGetStride(src1) / 8;
	sl1 = mlib_ImageGetData(src1);
	slb2 = mlib_ImageGetStride(src2) / 8;
	sl2 = mlib_ImageGetData(src2);
	xsize *= nchan;

	for (j = 0; j < ysize; j++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < xsize; i++) {
			dl[i] = sl1[i] / sl2[i];
		}

		dl += dlb;
		sl1 += slb1;
		sl2 += slb2;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_ImageDiv1_Fp_Inp(
    mlib_image *src1dst,
    const mlib_image *src2)
{
	return (__mlib_ImageDiv_Fp(src1dst, src1dst, src2));
}

/* *********************************************************** */

mlib_status
__mlib_ImageDiv2_Fp_Inp(
    mlib_image *src2dst,
    const mlib_image *src1)
{
	return (__mlib_ImageDiv_Fp(src2dst, src1, src2dst));
}

/* *********************************************************** */
