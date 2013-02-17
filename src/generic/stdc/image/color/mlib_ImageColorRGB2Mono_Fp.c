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

#pragma ident	"@(#)mlib_ImageColorRGB2Mono_Fp.c	9.2	07/10/09 SMI"

/*
 * FUNCTION
 *      mlib_ImageColorRGB2Mono_Fp - color to mono conversion
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageColorRGB2Mono_Fp(mlib_image       *dst,
 *                                             const mlib_image *src,
 *                                             const mlib_d64   *weight);
 *
 *      mlib_status mlib_ImageColorRGB2CIEMono_Fp(mlib_image       *dst,
 *                                                const mlib_image *src);
 *
 * ARGUMENT
 *      dst     pointer to output image
 *      src     pointer to input image
 *      weight  array of 3 blending coefficients
 *
 * RESTRICTION
 *      src and dst must be the same type, and the same size.
 *      The input must be a 3-channel image,
 *      and the output image must be a single channel image.
 *      They can be in MLIB_FLOAT or MLIB_DOUBLE data type.
 *
 * DESCRIPTION
 *      mlib_ImageColorRGB2Mono_Fp
 *        Z =  weight[0] * R +  weight[1] * G +  weight[2] * B
 *      mlib_ImageColorRGB2CIEMono_Fp
 *        Z =  0.2125 * R +  0.7154 * G +  0.0721 * B
 *
 *     It is required that all blending coefficients(weight) be no more
 *     than 1.0
 */

#include <mlib_image.h>
#include <mlib_ImageCheck.h>
#include <mlib_SysMath.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageColorRGB2CIEMono_Fp = __mlib_ImageColorRGB2CIEMono_Fp
#pragma weak mlib_ImageColorRGB2Mono_Fp = __mlib_ImageColorRGB2Mono_Fp

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_ImageColorRGB2CIEMono_Fp) mlib_ImageColorRGB2CIEMono_Fp
    __attribute__((weak, alias("__mlib_ImageColorRGB2CIEMono_Fp")));
__typeof__(__mlib_ImageColorRGB2Mono_Fp) mlib_ImageColorRGB2Mono_Fp
    __attribute__((weak, alias("__mlib_ImageColorRGB2Mono_Fp")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

static mlib_status mlib_ImageColorRGB2Mono_Fp_F32(
    const mlib_f32 *src,
    mlib_s32 slb,
    mlib_f32 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_d64 *weight);

static mlib_status mlib_ImageColorRGB2Mono_Fp_D64(
    const mlib_d64 *src,
    mlib_s32 slb,
    mlib_d64 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_d64 *weight);

/* *********************************************************** */

mlib_status
mlib_ImageColorRGB2Mono_Fp_F32(
    const mlib_f32 *src,
    mlib_s32 slb,
    mlib_f32 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_d64 *weight)
{
	mlib_f32 *sa, *sl;
	mlib_f32 *da, *dl;
	mlib_f32 sf0, sf1, sf2, df1, w[3];
	mlib_f32 sf3, sf4, sf5, df2;
	mlib_f32 sf6, sf7, sf8, df3;
	mlib_s32 i, j;
	mlib_s32 num_blocks, leftover_cols;

	w[0] = (mlib_f32)weight[0];
	w[1] = (mlib_f32)weight[1];
	w[2] = (mlib_f32)weight[2];

	sl = sa = (void *)src;
	dl = da = dst;

	num_blocks = xsize / 3;
	leftover_cols = xsize - num_blocks * 3;

	for (j = 0; j < ysize; j++) {
		for (i = 0; i < num_blocks; i++) {
			sf0 = (mlib_f32)(*sa++);
			sf1 = (mlib_f32)(*sa++);
			sf2 = (mlib_f32)(*sa++);
			sf3 = (mlib_f32)(*sa++);
			sf4 = (mlib_f32)(*sa++);
			sf5 = (mlib_f32)(*sa++);
			sf6 = (mlib_f32)(*sa++);
			sf7 = (mlib_f32)(*sa++);
			sf8 = (mlib_f32)(*sa++);

			df1 = w[0] * sf0 + w[1] * sf1 + w[2] * sf2;
			df2 = w[0] * sf3 + w[1] * sf4 + w[2] * sf5;
			df3 = w[0] * sf6 + w[1] * sf7 + w[2] * sf8;

			(*da++) = df1;
			(*da++) = df2;
			(*da++) = df3;
		}

		if (leftover_cols) {
			sf0 = (*sa++);
			sf1 = (*sa++);
			sf2 = (*sa++);

			df1 = w[0] * sf0 + w[1] * sf1 + w[2] * sf2;
			(*da++) = df1;

			if (leftover_cols > 1) {
				sf0 = (*sa++);
				sf1 = (*sa++);
				sf2 = (*sa++);

				df1 = w[0] * sf0 + w[1] * sf1 + w[2] * sf2;
				(*da++) = df1;
			}
		}

		sl = sa = sl + slb;
		dl = da = dl + dlb;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_ImageColorRGB2Mono_Fp_D64(
    const mlib_d64 *src,
    mlib_s32 slb,
    mlib_d64 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_d64 *weight)
{
	mlib_d64 *sa, *sl;
	mlib_d64 *da, *dl;
	mlib_d64 sf0, sf1, sf2, df1, w[3];
	mlib_d64 sf3, sf4, sf5, df2;
	mlib_d64 sf6, sf7, sf8, df3;
	mlib_s32 i, j;
	mlib_s32 num_blocks, leftover_cols;

	w[0] = weight[0];
	w[1] = weight[1];
	w[2] = weight[2];

	sl = sa = (void *)src;
	dl = da = dst;

	num_blocks = xsize / 3;
	leftover_cols = xsize - num_blocks * 3;

	for (j = 0; j < ysize; j++) {
		for (i = 0; i < num_blocks; i++) {
			sf0 = (*sa++);
			sf1 = (*sa++);
			sf2 = (*sa++);
			sf3 = (*sa++);
			sf4 = (*sa++);
			sf5 = (*sa++);
			sf6 = (*sa++);
			sf7 = (*sa++);
			sf8 = (*sa++);

			df1 = w[0] * sf0 + w[1] * sf1 + w[2] * sf2;
			df2 = w[0] * sf3 + w[1] * sf4 + w[2] * sf5;
			df3 = w[0] * sf6 + w[1] * sf7 + w[2] * sf8;

			(*da++) = df1;
			(*da++) = df2;
			(*da++) = df3;
		}

		if (leftover_cols) {
			sf0 = (*sa++);
			sf1 = (*sa++);
			sf2 = (*sa++);

			df1 = w[0] * sf0 + w[1] * sf1 + w[2] * sf2;
			(*da++) = df1;

			if (leftover_cols > 1) {
				sf0 = (*sa++);
				sf1 = (*sa++);
				sf2 = (*sa++);

				df1 = w[0] * sf0 + w[1] * sf1 + w[2] * sf2;
				(*da++) = df1;
			}
		}

		sl = sa = sl + slb;
		dl = da = dl + dlb;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_ImageColorRGB2Mono_Fp(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_d64 *weight)
{
	mlib_s32 slb, dlb, xsize, ysize;
	mlib_type dtype;
	void *sa, *da;
	mlib_s32 i;

	MLIB_IMAGE_CHECK(src);
	MLIB_IMAGE_CHECK(dst);
	MLIB_IMAGE_TYPE_EQUAL(dst, src);
	MLIB_IMAGE_SIZE_EQUAL(dst, src);
	MLIB_IMAGE_HAVE_CHAN(src, 3);
	MLIB_IMAGE_HAVE_CHAN(dst, 1);

	for (i = 0; i < 3; i++)
		if (mlib_fabs(weight[i]) > 1.0)
			return (MLIB_OUTOFRANGE);

	dtype = mlib_ImageGetType(dst);
	xsize = mlib_ImageGetWidth(dst);
	ysize = mlib_ImageGetHeight(dst);
	slb = mlib_ImageGetStride(src);
	dlb = mlib_ImageGetStride(dst);
	sa = mlib_ImageGetData(src);
	da = mlib_ImageGetData(dst);

	if (dtype == MLIB_FLOAT) {

		slb >>= 2;
		dlb >>= 2;
		return mlib_ImageColorRGB2Mono_Fp_F32(sa, slb, da, dlb, xsize,
		    ysize, weight);
	} else if (dtype == MLIB_DOUBLE) {

		slb >>= 3;
		dlb >>= 3;
		return mlib_ImageColorRGB2Mono_Fp_D64(sa, slb, da, dlb, xsize,
		    ysize, weight);
	} else
		return (MLIB_FAILURE);
}

/* *********************************************************** */

mlib_status
__mlib_ImageColorRGB2CIEMono_Fp(
    mlib_image *dst,
    const mlib_image *src)
{
/* blending coefficients for CIE */
	mlib_d64 weight[3] = { 0.2125, 0.7154, 0.0721 };

	return (__mlib_ImageColorRGB2Mono_Fp(dst, src, weight));
}

/* *********************************************************** */
