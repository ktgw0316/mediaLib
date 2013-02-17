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

#pragma ident "@(#)mlib_s_ImageBlend_Fp.c	9.3	07/11/05 SMI"

/*
 * FUNCTION
 *	mlib_ImageBlend_Fp		- blend two images
 *	mlib_ImageBlend1_Fp_Inp		- blend two images
 *	mlib_ImageBlend2_Fp_Inp		- blend two images
 *
 * SYNOPSIS
 *	mlib_status mlib_ImageBlend_Fp(mlib_image       *dst,
 *	                               const mlib_image *src1,
 *                                     const mlib_image *src2,
 *                                     const mlib_image *alpha);
 *	mlib_status mlib_ImageBlend1_Fp_Inp(mlib_image *src1dst,
 *	                                    const mlib_image *src2,
 *	                                    const mlib_image *alpha);
 *	mlib_status mlib_ImageBlend2_Fp_Inp(mlib_image *src2dst,
 *	                                    const mlib_image *src1,
 *	                                    const mlib_image *alpha);
 *
 * ARGUMENT
 *	dst     pointer to output image
 *	src1    pointer to first input image
 *	src2    pointer to second input image
 *	src1dst	pointer to the first input and the output image
 *	src2dst	pointer to the second input and the output image
 *	alpha	pointer to the alpha image
 *
 * RESTRICTION
 *	src1, src2, src1dst, src2dst, and dst must be the same type,
 *	the same size and the same number of channels.
 *	They can have 1, 2, 3, or 4 channels.
 *	They can be in MLIB_FLOAT or MLIB_DOUBLE data type.
 *	alpha must be the same type and the same size as src1, src2,
 *	src1dst, src2dst, and dst; it must either be the same number
 *	of channels as src1, src2, src1dst, src2dst, and dst, or be
 *	the single channel which the same alpha factor is used for
 *	all channels.
 *
 * DESCRIPTION
 *	dst[x][y][c] = alpha[x][y][c] * src1[x][y][c] +
 *	               (1 - alpha[x][y][c]) * src2[x][y][c]
 *	src1dst[x][y][c] = alpha[x][y][c] * src1dst[x][y][c] +
 *	                   (1 - alpha[x][y][c]) * src2[x][y][c]
 *	src2dst[x][y][c] = alpha[x][y][c] * src1[x][y][c] +
 *	                   (1 - alpha[x][y][c]) * src2dst[x][y][c]
 *
 *	If alpha is single channel:
 *
 *	dst[x][y][c] = alpha[x][y] * src1[x][y][c] +
 *	               (1 - alpha[x][y]) * src2[x][y][c]
 *	src1dst[x][y][c] = alpha[x][y] * src1dst[x][y][c] +
 *	                   (1 - alpha[x][y]) * src2[x][y][c]
 *	src2dst[x][y][c] = alpha[x][y] * src1[x][y][c] +
 *	                   (1 - alpha[x][y]) * src2dst[x][y][c]
 */

#include <mlib_image.h>
#include <mlib_ImageCheck.h>
#include "mlib_s_ImageBlend_Fp.h"

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageBlend_Fp = __mlib_ImageBlend_Fp
#pragma weak mlib_ImageBlend1_Fp_Inp = __mlib_ImageBlend1_Fp_Inp
#pragma weak mlib_ImageBlend2_Fp_Inp = __mlib_ImageBlend2_Fp_Inp

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */

__typeof__(__mlib_ImageBlend_Fp) mlib_ImageBlend_Fp
	__attribute__((weak, alias("__mlib_ImageBlend_Fp")));
__typeof__(__mlib_ImageBlend1_Fp_Inp) mlib_ImageBlend1_Fp_Inp
	__attribute__((weak, alias("__mlib_ImageBlend1_Fp_Inp")));
__typeof__(__mlib_ImageBlend2_Fp_Inp) mlib_ImageBlend2_Fp_Inp
	__attribute__((weak, alias("__mlib_ImageBlend2_Fp_Inp")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

mlib_status
__mlib_ImageBlend_Fp(
	mlib_image *dst,
	const mlib_image *src1,
	const mlib_image *src2,
	const mlib_image *alpha)
{
	mlib_type dtype;
	mlib_s32 slb1, slb2, dlb, alb, xsize, ysize, nchan, achan;
	void *sa1, *sa2, *da, *aa;

	MLIB_IMAGE_CHECK(dst);
	MLIB_IMAGE_CHECK(src1);
	MLIB_IMAGE_CHECK(src2);
	MLIB_IMAGE_FULL_EQUAL(dst, src1);
	MLIB_IMAGE_FULL_EQUAL(dst, src2);
	MLIB_IMAGE_SIZE_EQUAL(dst, alpha);
	MLIB_IMAGE_TYPE_EQUAL(dst, alpha);

	dtype = mlib_ImageGetType(dst);
	nchan = mlib_ImageGetChannels(dst);
	achan = mlib_ImageGetChannels(alpha);
	xsize = mlib_ImageGetWidth(dst);
	ysize = mlib_ImageGetHeight(dst);
	slb1 = mlib_ImageGetStride(src1);
	slb2 = mlib_ImageGetStride(src2);
	dlb = mlib_ImageGetStride(dst);
	alb = mlib_ImageGetStride(alpha);
	sa1 = mlib_ImageGetData(src1);
	sa2 = mlib_ImageGetData(src2);
	da = mlib_ImageGetData(dst);
	aa = mlib_ImageGetData(alpha);

	if (achan == nchan) {
		xsize *= nchan;
		if (dtype == MLIB_FLOAT) {
			mlib_s_ImageBlend_Fp_f32(da, dlb,
						sa1, slb1,
						sa2, slb2,
						aa, alb,
						xsize, ysize);
			return (MLIB_SUCCESS);
		} else
		if (dtype == MLIB_DOUBLE) {
			mlib_s_ImageBlend_Fp_d64(da, dlb,
						sa1, slb1,
						sa2, slb2,
						aa, alb,
						xsize, ysize);
			return (MLIB_SUCCESS);
		}
	} else
	if (achan == 1) {
		if (dtype == MLIB_FLOAT) {
			mlib_s_ImageBlendSA_Fp_f32(da, dlb,
						sa1, slb1,
						sa2, slb2,
						aa, alb,
						xsize, ysize, nchan);
			return (MLIB_SUCCESS);
		} else
		if (dtype == MLIB_DOUBLE) {
			mlib_s_ImageBlendSA_Fp_d64(da, dlb,
						sa1, slb1,
						sa2, slb2,
						aa, alb,
						xsize, ysize, nchan);
			return (MLIB_SUCCESS);
		}
	}

	return (MLIB_FAILURE);
}

/* *********************************************************** */

mlib_status
__mlib_ImageBlend1_Fp_Inp(
	mlib_image *src1dst,
	const mlib_image *src2,
	const mlib_image *alpha)
{
	return (__mlib_ImageBlend_Fp(src1dst, src1dst, src2, alpha));
}

/* *********************************************************** */

mlib_status
__mlib_ImageBlend2_Fp_Inp(
	mlib_image *src2dst,
	const mlib_image *src1,
	const mlib_image *alpha)
{
	return (__mlib_ImageBlend_Fp(src2dst, src1, src2dst, alpha));
}

/* *********************************************************** */
