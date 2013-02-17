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

#pragma ident	"@(#)mlib_ImageColorConvert_Fp.c	9.2	07/10/09 SMI"

/*
 * FUNCTION
 *      mlib_ImageColorConvert1_Fp - color conversion without offset
 *      mlib_ImageColorConvert2_Fp - color conversion with offsef
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageColorConvert1_Fp(mlib_image       *dst,
 *                                             const mlib_image *src,
 *                                             const mlib_d64   *fmat);
 *
 *      mlib_status mlib_ImageColorConvert2_Fp(mlib_image       *dst,
 *                                             const mlib_image *src,
 *                                             const mlib_d64   *fmat,
 *                                             const mlib_d64   *offset);
 *
 *      mlib_status mlib_ImageColorRGB2XYZ_Fp(mlib_image       *dst,
 *                                            const mlib_image *src);
 *
 *      mlib_status mlib_ImageColorXYZ2RGB_Fp(mlib_image       *dst,
 *                                            const mlib_image *src);
 *
 *      mlib_status mlib_ImageColorRGB2YCC_Fp(mlib_image       *dst,
 *                                            const mlib_image *src);
 *
 *      mlib_status mlib_ImageColorYCC2RGB_Fp(mlib_image       *dst,
 *                                            const mlib_image *src);
 *
 * ARGUMENT
 *      dst     pointer to output image
 *      src     pointer to input image
 *      fmat    9 parameters of color matrix in row major order
 *      offset  3 offset values
 *
 * RESTRICTION
 *      src and dst must be the same type, the same size and the same
 *      number of channels.
 *      They must be 3-channel images.
 *      They can be in MLIB_FLOAT or MLIB_DOUBLE data type.
 *
 * DESCRIPTION
 *      mlib_ImageColorConvert1()
 *        |X|   |fmat[0] fmat[1] fmat[2]|   |R|
 *        |Y| = |fmat[3] fmat[4] fmat[5]| * |G|
 *        |Z|   |fmat[6] fmat[7] fmat[8]|   |B|
 *      mlib_ImageColorConvert2()
 *        |X|   |fmat[0] fmat[1] fmat[2]|   |R|   |offset[0]|
 *        |Y| = |fmat[3] fmat[4] fmat[5]| * |G| + |offset[1]|
 *        |Z|   |fmat[6] fmat[7] fmat[8]|   |B|   |offset[2]|
 *      mlib_ImageColorRGB2XYZ()
 *        convert ITU-R Rec.708 RGB with D64 white point into CIE XYZ
 *      mlib_ImageColorXYZ2RGB()
 *        convert CIE XYZ into ITU-R Rec.708 RGB with D64 white point
 *      mlib_ImageColorRGB2YCC()
 *        convert computer R'G'B' into ITU-R Rec.601 Y'CbCr
 *      mlib_ImageColorYCC2EGB()
 *        convert ITU-R Rec.601 Y'CbCr into computer R'G'B'
 */

#include <mlib_image.h>
#include <mlib_ImageCheck.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageColorConvert1_Fp = __mlib_ImageColorConvert1_Fp
#pragma weak mlib_ImageColorConvert2_Fp = __mlib_ImageColorConvert2_Fp
#pragma weak mlib_ImageColorRGB2XYZ_Fp = __mlib_ImageColorRGB2XYZ_Fp
#pragma weak mlib_ImageColorRGB2YCC_Fp = __mlib_ImageColorRGB2YCC_Fp
#pragma weak mlib_ImageColorXYZ2RGB_Fp = __mlib_ImageColorXYZ2RGB_Fp
#pragma weak mlib_ImageColorYCC2RGB_Fp = __mlib_ImageColorYCC2RGB_Fp

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_ImageColorConvert1_Fp) mlib_ImageColorConvert1_Fp
    __attribute__((weak, alias("__mlib_ImageColorConvert1_Fp")));
__typeof__(__mlib_ImageColorConvert2_Fp) mlib_ImageColorConvert2_Fp
    __attribute__((weak, alias("__mlib_ImageColorConvert2_Fp")));
__typeof__(__mlib_ImageColorRGB2XYZ_Fp) mlib_ImageColorRGB2XYZ_Fp
    __attribute__((weak, alias("__mlib_ImageColorRGB2XYZ_Fp")));
__typeof__(__mlib_ImageColorRGB2YCC_Fp) mlib_ImageColorRGB2YCC_Fp
    __attribute__((weak, alias("__mlib_ImageColorRGB2YCC_Fp")));
__typeof__(__mlib_ImageColorXYZ2RGB_Fp) mlib_ImageColorXYZ2RGB_Fp
    __attribute__((weak, alias("__mlib_ImageColorXYZ2RGB_Fp")));
__typeof__(__mlib_ImageColorYCC2RGB_Fp) mlib_ImageColorYCC2RGB_Fp
    __attribute__((weak, alias("__mlib_ImageColorYCC2RGB_Fp")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

static mlib_status mlib_ImageColorConvert1_F32(
    const mlib_f32 *src,
    mlib_s32 slb,
    mlib_f32 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_d64 *fmat);

static mlib_status mlib_ImageColorConvert1_D64(
    const mlib_d64 *src,
    mlib_s32 slb,
    mlib_d64 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_d64 *fmat);

static mlib_status mlib_ImageColorConvert2_F32(
    const mlib_f32 *src,
    mlib_s32 slb,
    mlib_f32 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_d64 *fmat,
    const mlib_d64 *offset);

static mlib_status mlib_ImageColorConvert2_D64(
    const mlib_d64 *src,
    mlib_s32 slb,
    mlib_d64 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_d64 *fmat,
    const mlib_d64 *offset);

/* *********************************************************** */

mlib_status
mlib_ImageColorConvert1_F32(
    const mlib_f32 *src,
    mlib_s32 slb,
    mlib_f32 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_d64 *fmat)
{
/* pointers for pixel and line of source */
	mlib_f32 *sa, *sl;

/* pointers for pixel and line of destination */
	mlib_f32 *da, *dl;

/* indices */
	mlib_s32 i, j;

/* accumulators */
	mlib_d64 sum1, sum2, sum3;

/* coeff's */
	mlib_d64 k0, k1, k2, k3, k4, k5, k6, k7, k8;
	mlib_d64 p0, p1, p2;

/* keep kernel in regs */
	k0 = fmat[0];
	k1 = fmat[1];
	k2 = fmat[2];
	k3 = fmat[3];
	k4 = fmat[4];
	k5 = fmat[5];
	k6 = fmat[6];
	k7 = fmat[7];
	k8 = fmat[8];

	sa = sl = (void *)src;
	da = dl = dst;

	for (j = 0; j < ysize; j++) {

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < 3 * xsize; i += 3) {
			p0 = (mlib_d64)sa[i];
			p1 = (mlib_d64)sa[i + 1];
			p2 = (mlib_d64)sa[i + 2];
			sum1 = p0 * k0 + p1 * k1 + p2 * k2;
			sum2 = p0 * k3 + p1 * k4 + p2 * k5;
			sum3 = p0 * k6 + p1 * k7 + p2 * k8;

			(*da++) = (mlib_f32)sum1;
			(*da++) = (mlib_f32)sum2;
			(*da++) = (mlib_f32)sum3;
		}

/* set src ptrs to next row */
		sl = sa = sl + slb;
/* set dst ptrs to next row */
		dl = da = dl + dlb;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_ImageColorConvert1_D64(
    const mlib_d64 *src,
    mlib_s32 slb,
    mlib_d64 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_d64 *fmat)
{
/* pointers for pixel and line of source */
	mlib_d64 *sa, *sl;

/* pointers for pixel and line of destination */
	mlib_d64 *da, *dl;

/* indices */
	mlib_s32 i, j;

/* accumulators */
	mlib_d64 sum1, sum2, sum3;

/* coeff's */
	mlib_d64 k0, k1, k2, k3, k4, k5, k6, k7, k8;
	mlib_d64 p0, p1, p2;

/* keep kernel in regs */
	k0 = fmat[0];
	k1 = fmat[1];
	k2 = fmat[2];
	k3 = fmat[3];
	k4 = fmat[4];
	k5 = fmat[5];
	k6 = fmat[6];
	k7 = fmat[7];
	k8 = fmat[8];

	sa = sl = (void *)src;
	da = dl = dst;

	for (j = 0; j < ysize; j++) {

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < 3 * xsize; i += 3) {
			p0 = sa[i];
			p1 = sa[i + 1];
			p2 = sa[i + 2];
			sum1 = p0 * k0 + p1 * k1 + p2 * k2;
			sum2 = p0 * k3 + p1 * k4 + p2 * k5;
			sum3 = p0 * k6 + p1 * k7 + p2 * k8;

			(*da++) = sum1;
			(*da++) = sum2;
			(*da++) = sum3;
		}

/* set src ptrs to next row */
		sl = sa = sl + slb;
/* set dst ptrs to next row */
		dl = da = dl + dlb;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_ImageColorConvert2_F32(
    const mlib_f32 *src,
    mlib_s32 slb,
    mlib_f32 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_d64 *fmat,
    const mlib_d64 *offset)
{
/* pointers for pixel and line of source */
	mlib_f32 *sa, *sl;

/* pointers for pixel and line of destination */
	mlib_f32 *da, *dl;

/* indices */
	mlib_s32 i, j;

/* accumulators */
	mlib_d64 sum1, sum2, sum3;

/* coeff's */
	mlib_d64 k0, k1, k2, k3, k4, k5, k6, k7, k8;
	mlib_d64 p0, p1, p2;
	mlib_d64 off0, off1, off2;

/* keep kernel in regs */
	k0 = fmat[0];
	k1 = fmat[1];
	k2 = fmat[2];
	k3 = fmat[3];
	k4 = fmat[4];
	k5 = fmat[5];
	k6 = fmat[6];
	k7 = fmat[7];
	k8 = fmat[8];

	off0 = offset[0];
	off1 = offset[1];
	off2 = offset[2];

	sa = sl = (void *)src;
	da = dl = dst;

	for (j = 0; j < ysize; j++) {

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < 3 * xsize; i += 3) {
			p0 = (mlib_d64)sa[i];
			p1 = (mlib_d64)sa[i + 1];
			p2 = (mlib_d64)sa[i + 2];
			sum1 = p0 * k0 + p1 * k1 + p2 * k2 + off0;
			sum2 = p0 * k3 + p1 * k4 + p2 * k5 + off1;
			sum3 = p0 * k6 + p1 * k7 + p2 * k8 + off2;

			(*da++) = (mlib_f32)sum1;
			(*da++) = (mlib_f32)sum2;
			(*da++) = (mlib_f32)sum3;
		}

/* set src ptrs to next row */
		sl = sa = sl + slb;
/* set dst ptrs to next row */
		dl = da = dl + dlb;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_ImageColorConvert2_D64(
    const mlib_d64 *src,
    mlib_s32 slb,
    mlib_d64 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_d64 *fmat,
    const mlib_d64 *offset)
{
/* pointers for pixel and line of source */
	mlib_d64 *sa, *sl;

/* pointers for pixel and line of destination */
	mlib_d64 *da, *dl;

/* indices */
	mlib_s32 i, j;

/* accumulators */
	mlib_d64 sum1, sum2, sum3;

/* coeff's */
	mlib_d64 k0, k1, k2, k3, k4, k5, k6, k7, k8;
	mlib_d64 p0, p1, p2;
	mlib_d64 off0, off1, off2;

/* keep kernel in regs */
	k0 = fmat[0];
	k1 = fmat[1];
	k2 = fmat[2];
	k3 = fmat[3];
	k4 = fmat[4];
	k5 = fmat[5];
	k6 = fmat[6];
	k7 = fmat[7];
	k8 = fmat[8];

	off0 = offset[0];
	off1 = offset[1];
	off2 = offset[2];

	sa = sl = (void *)src;
	da = dl = dst;

	for (j = 0; j < ysize; j++) {

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < 3 * xsize; i += 3) {
			p0 = sa[i];
			p1 = sa[i + 1];
			p2 = sa[i + 2];
			sum1 = p0 * k0 + p1 * k1 + p2 * k2 + off0;
			sum2 = p0 * k3 + p1 * k4 + p2 * k5 + off1;
			sum3 = p0 * k6 + p1 * k7 + p2 * k8 + off2;

			(*da++) = sum1;
			(*da++) = sum2;
			(*da++) = sum3;
		}

/* set src ptrs to next row */
		sl = sa = sl + slb;
/* set dst ptrs to next row */
		dl = da = dl + dlb;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_ImageColorConvert1_Fp(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_d64 *fmat)
{
	mlib_s32 slb, dlb, xsize, ysize;
	mlib_type dtype;
	void *sa, *da;

	MLIB_IMAGE_CHECK(src);
	MLIB_IMAGE_CHECK(dst);
	MLIB_IMAGE_FULL_EQUAL(dst, src);
	MLIB_IMAGE_HAVE_CHAN(dst, 3);

	if (fmat == NULL)
		return (MLIB_NULLPOINTER);

	dtype = mlib_ImageGetType(dst);
	xsize = mlib_ImageGetWidth(dst);
	ysize = mlib_ImageGetHeight(dst);
	slb = mlib_ImageGetStride(src);
	dlb = mlib_ImageGetStride(dst);
	sa = mlib_ImageGetData(src);
	da = mlib_ImageGetData(dst);

	if (dtype == MLIB_FLOAT) {
		return mlib_ImageColorConvert1_F32(sa, slb / 4,
		    da, dlb / 4, xsize, ysize, fmat);
	} else if (dtype == MLIB_DOUBLE) {
		return mlib_ImageColorConvert1_D64(sa, slb / 8,
		    da, dlb / 8, xsize, ysize, fmat);
	} else
		return (MLIB_FAILURE);
}

/* *********************************************************** */

mlib_status
__mlib_ImageColorConvert2_Fp(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_d64 *fmat,
    const mlib_d64 *offset)
{
	mlib_s32 slb, dlb, xsize, ysize;
	mlib_type dtype;
	void *sa, *da;

	MLIB_IMAGE_CHECK(src);
	MLIB_IMAGE_CHECK(dst);
	MLIB_IMAGE_FULL_EQUAL(dst, src);
	MLIB_IMAGE_HAVE_CHAN(dst, 3);

	if (fmat == NULL || offset == NULL)
		return (MLIB_NULLPOINTER);

	dtype = mlib_ImageGetType(dst);
	xsize = mlib_ImageGetWidth(dst);
	ysize = mlib_ImageGetHeight(dst);
	slb = mlib_ImageGetStride(src);
	dlb = mlib_ImageGetStride(dst);
	sa = mlib_ImageGetData(src);
	da = mlib_ImageGetData(dst);

	if (dtype == MLIB_FLOAT) {
		return mlib_ImageColorConvert2_F32(sa, slb / 4,
		    da, dlb / 4, xsize, ysize, fmat, offset);
	} else if (dtype == MLIB_DOUBLE) {
		return mlib_ImageColorConvert2_D64(sa, slb / 8,
		    da, dlb / 8, xsize, ysize, fmat, offset);
	} else
		return (MLIB_FAILURE);
}

/* *********************************************************** */

mlib_status
__mlib_ImageColorRGB2XYZ_Fp(
    mlib_image *dst,
    const mlib_image *src)
{
/* Rec709 RGB with D64 White Point to CIE XYZ */
	mlib_d64 fmat[9] = { 0.412453, 0.357580, 0.180423,
		0.212671, 0.715160, 0.072169,
		0.019334, 0.119193, 0.950227
	};

	return (__mlib_ImageColorConvert1_Fp(dst, src, fmat));
}

/* *********************************************************** */

mlib_status
__mlib_ImageColorXYZ2RGB_Fp(
    mlib_image *dst,
    const mlib_image *src)
{
/* CIE XYZ to Rec709 RGB with D64 White Point */
	mlib_d64 fmat[9] = { 3.240479, -1.537150, -0.498535,
		-0.969256, 1.875992, 0.041566,
		0.055648, -0.204043, 1.057311
	};

	return (__mlib_ImageColorConvert1_Fp(dst, src, fmat));
}

/* *********************************************************** */

mlib_status
__mlib_ImageColorRGB2YCC_Fp(
    mlib_image *dst,
    const mlib_image *src)
{
/* Computer R'G'B' to Rec601 Y'CbCr */
	mlib_d64 fmat[9] = { 65.738 / 256, 129.057 / 256, 25.064 / 256,
		-37.945 / 256, -74.494 / 256, 112.439 / 256,
		112.439 / 256, -94.154 / 256, -18.285 / 256
	};
	mlib_d64 offset[3] = { 16, 128, 128 };

	return (__mlib_ImageColorConvert2_Fp(dst, src, fmat, offset));
}

/* *********************************************************** */

mlib_status
__mlib_ImageColorYCC2RGB_Fp(
    mlib_image *dst,
    const mlib_image *src)
{
/* Rec601 Y'CbCr to Computer R'G'B' */
	mlib_d64 fmat[9] = { 298.082 / 256, 0, 408.583 / 256,
		298.082 / 256, -100.291 / 256, -208.120 / 256,
		298.082 / 256, 516.411 / 256, 0
	};
	mlib_d64 offset[3] = { -222.953, 135.575, -276.836 };

	return (__mlib_ImageColorConvert2_Fp(dst, src, fmat, offset));
}

/* *********************************************************** */
