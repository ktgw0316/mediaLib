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

#pragma ident	"@(#)mlib_ImageReplaceColor_Fp.c	9.2	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_ImageReplaceColor_Fp - replace a color in the image with
 *                                  another color.
 *      mlib_ImageReplaceColor_Fp_Inp - replace a color in the image with
 *                                      another color.
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_ImageReplaceColor_Fp(mlib_image       *dst,
 *                                            const mlib_image *src,
 *                                            const mlib_d64   *color1,
 *                                            const mlib_d64   *color2);
 *      mlib_status mlib_ImageReplaceColor_Fp_Inp(mlib_image     *srcdst,
 *                                                const mlib_d64 *color1,
 *                                                const mlib_d64 *color2);
 *
 * ARGUMENT
 *      dst     pointer to output image
 *      src     pointer to input image
 *      srcdst  pointer to input and output image
 *      color1  array of color to find in the source image
 *      color2  array of color to replace color1 with
 *
 * RESTRICTION
 *      src and dst must be of the same type, the same size and
 *      the same number of channels. They can have one through four
 *      channels. They can be in MLIB_FLOAT or MLIB_DOUBLE data type.
 *      The length of color array must be not less than the number of
 *      channels in the images.
 *
 *      srcdst can has one through four channels. It can be in
 *      MLIB_FLOAT or MLIB_DOUBLE data type. The length of color array
 *      must be not less than the number of channels in the image.
 *
 * DESCRIPTION
 *      Function scans the source image for all pixels with color value
 *      equal to color1 and replaces these pixels with color2.
 */

#include <mlib_image.h>
#include <mlib_ImageCheck.h>
#include <mlib_ImageReplaceColor.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageReplaceColor_Fp = __mlib_ImageReplaceColor_Fp
#pragma weak mlib_ImageReplaceColor_Fp_Inp = __mlib_ImageReplaceColor_Fp_Inp

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_ImageReplaceColor_Fp) mlib_ImageReplaceColor_Fp
    __attribute__((weak, alias("__mlib_ImageReplaceColor_Fp")));
__typeof__(__mlib_ImageReplaceColor_Fp_Inp) mlib_ImageReplaceColor_Fp_Inp
    __attribute__((weak, alias("__mlib_ImageReplaceColor_Fp_Inp")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

#if defined(_NO_LONGLONG)
/* *********************************************************** */

#define	COLOR_TYPE	mlib_d64

/* *********************************************************** */

#ifndef __INTEL_COMPILER

#define	GET_CONST(N)                            \
	DTYPE a##N = (DTYPE) color1[N];         \
	DTYPE b##N = (DTYPE) color2[N]

#else

#define	GET_CONST(N)                            \
	volatile DTYPE tmp##N = color1[N];      \
	DTYPE a##N = tmp##N;                    \
	DTYPE b##N = (DTYPE) color2[N]

#endif

/* *********************************************************** */

#define	PROCESS_CH(N)	(s##N == a##N) ? b##N : s##N

/* *********************************************************** */

#define	FUNC_NAME	mlib_ImageReplaceColor_F32
#define	DTYPE	mlib_f32

#define	TTYPE	DTYPE

#include <mlib_ImageReplaceColor.h>

/* *********************************************************** */

#define	FUNC_NAME	mlib_ImageReplaceColor_D64
#define	DTYPE	mlib_d64

#include <mlib_ImageReplaceColor.h>

/* *********************************************************** */

mlib_status
__mlib_ImageReplaceColor_Fp(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_d64 *color1,
    const mlib_d64 *color2)
{
	mlib_s32 nchan, width, height, sstride, dstride;
	void *sdata, *ddata;
	mlib_type type;
	mlib_s32 t_sh;

	MLIB_IMAGE_CHECK(dst);
	MLIB_IMAGE_CHECK(src);
	MLIB_IMAGE_FULL_EQUAL(dst, src);

	if (color1 == NULL)
		return (MLIB_NULLPOINTER);
	if (color2 == NULL)
		return (MLIB_NULLPOINTER);

	MLIB_IMAGE_GET_ALL_PARAMS(dst, type, nchan, width, height, dstride,
	    ddata);
	sstride = mlib_ImageGetStride(src);
	sdata = mlib_ImageGetData(src);

	if (type == MLIB_FLOAT) {
		t_sh = 2;
	} else {
		t_sh = 3;
	}

	sstride >>= t_sh;
	dstride >>= t_sh;

	if (sstride == dstride && sstride == nchan * width) {
		width *= height;
		height = 1;
	}

	switch (type) {
	case MLIB_FLOAT:
		return mlib_ImageReplaceColor_F32(sdata, ddata, sstride,
		    dstride, width, height, nchan, color1, color2);

	case MLIB_DOUBLE:
		return mlib_ImageReplaceColor_D64(sdata, ddata, sstride,
		    dstride, width, height, nchan, color1, color2);

	default:
		return (MLIB_FAILURE);
	}
}

/* *********************************************************** */

mlib_status
__mlib_ImageReplaceColor_Fp_Inp(
    mlib_image *srcdst,
    const mlib_d64 *color1,
    const mlib_d64 *color2)
{
	return (__mlib_ImageReplaceColor_Fp(srcdst, srcdst, color1, color2));
}

/* *********************************************************** */
#else
/* *********************************************************** */

#ifndef __INTEL_COMPILER

#define	GET_CONST(N, STYPE, DTYPE)                    \
	gtemp = (STYPE) color1[N];                        \
	DTYPE a##N = *((DTYPE *) &gtemp);                 \
	gtemp = (STYPE) color2[N];                        \
	DTYPE b##N = *((DTYPE *) &gtemp)

#else

#define	GET_CONST(N, STYPE, DTYPE)                   \
	volatile STYPE tmp##N = color1[N];               \
	DTYPE a##N = *((DTYPE *) &tmp##N);               \
	tmp##N = color2[N];                              \
	DTYPE b##N = *((DTYPE *) &tmp##N)

#endif

/* *********************************************************** */

#define	PROCESS_CH(N)	(s##N == a##N) ? b##N : s##N

/* *********************************************************** */

mlib_status
__mlib_ImageReplaceColor_Fp(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_d64 *color1,
    const mlib_d64 *color2)
{
	mlib_s32 nchan, width, height, sstride, dstride;
	void *sdata, *ddata;
	mlib_type type;
	mlib_s32 t_sh;

	MLIB_IMAGE_CHECK(dst);
	MLIB_IMAGE_CHECK(src);
	MLIB_IMAGE_FULL_EQUAL(dst, src);

	if (color1 == NULL)
		return (MLIB_NULLPOINTER);
	if (color2 == NULL)
		return (MLIB_NULLPOINTER);

	MLIB_IMAGE_GET_ALL_PARAMS(dst, type, nchan, width, height, dstride,
	    ddata);
	sstride = mlib_ImageGetStride(src);
	sdata = mlib_ImageGetData(src);

	if (type == MLIB_FLOAT) {
		t_sh = 2;
	} else {
		t_sh = 3;
	}

	sstride >>= t_sh;
	dstride >>= t_sh;

	if (sstride == dstride && sstride == nchan * width) {
		width *= height;
		height = 1;
	}

	switch (type) {
	case MLIB_FLOAT:
		return mlib_ImageReplaceColor_F32(sdata, ddata, sstride,
		    dstride, width, height, nchan, color1, color2);

	case MLIB_DOUBLE:
		return mlib_ImageReplaceColor_D64(sdata, ddata, sstride,
		    dstride, width, height, nchan, color1, color2);

	default:
		return (MLIB_FAILURE);
	}
}

/* *********************************************************** */

mlib_status
mlib_ImageReplaceColor_F32(
	const mlib_f32 * sl, mlib_f32 * dl, mlib_s32 sstride,
    mlib_s32 dstride, mlib_s32 width, mlib_s32 height,
    mlib_s32 nchan, const mlib_d64 * color1,
    const mlib_d64 * color2)
{
	mlib_s32 i, j;
	mlib_s32 s0, s1, s2, s3;
	mlib_f32 gtemp;
	mlib_s32 *sp = (mlib_s32 *)sl;
	mlib_s32 *dp = (mlib_s32 *)dl;

	if (nchan == 1) {
		GET_CONST(0, mlib_f32, mlib_s32);
		for (j = 0; j < height; j++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i <= width - 4; i += 4) {
				s0 = sp[i];
				dp[i] = PROCESS_CH(0);
				s0 = sp[i + 1];
				dp[i + 1] = PROCESS_CH(0);
				s0 = sp[i + 2];
				dp[i + 2] = PROCESS_CH(0);
				s0 = sp[i + 3];
				dp[i + 3] = PROCESS_CH(0);
			}

			for (; i < width; i++) {
				s0 = sp[i];
				dp[i] = PROCESS_CH(0);
			}

			sp += sstride;
			dp += dstride;
		}

	} else if (nchan == 2) {
		GET_CONST(0, mlib_f32, mlib_s32);
		GET_CONST(1, mlib_f32, mlib_s32);
		for (j = 0; j < height; j++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < width; i++) {
				s0 = sp[2 * i];
				s1 = sp[2 * i + 1];
				dp[2 * i] = PROCESS_CH(0);
				dp[2 * i + 1] = PROCESS_CH(1);
			}

			sp += sstride;
			dp += dstride;
		}

	} else if (nchan == 3) {
		GET_CONST(0, mlib_f32, mlib_s32);
		GET_CONST(1, mlib_f32, mlib_s32);
		GET_CONST(2, mlib_f32, mlib_s32);
		for (j = 0; j < height; j++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < width; i++) {
				s0 = sp[3 * i];
				s1 = sp[3 * i + 1];
				s2 = sp[3 * i + 2];
				dp[3 * i] = PROCESS_CH(0);
				dp[3 * i + 1] = PROCESS_CH(1);
				dp[3 * i + 2] = PROCESS_CH(2);
			}

			sp += sstride;
			dp += dstride;
		}

	} else {	/* if (nchan == 4) */

		GET_CONST(0, mlib_f32, mlib_s32);
		GET_CONST(1, mlib_f32, mlib_s32);
		GET_CONST(2, mlib_f32, mlib_s32);
		GET_CONST(3, mlib_f32, mlib_s32);
		for (j = 0; j < height; j++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < width; i++) {
				s0 = sp[4 * i];
				s1 = sp[4 * i + 1];
				s2 = sp[4 * i + 2];
				s3 = sp[4 * i + 3];
				dp[4 * i] = PROCESS_CH(0);
				dp[4 * i + 1] = PROCESS_CH(1);
				dp[4 * i + 2] = PROCESS_CH(2);
				dp[4 * i + 3] = PROCESS_CH(3);
			}

			sp += sstride;
			dp += dstride;
		}
	}

	return (MLIB_SUCCESS);

}

/* *********************************************************** */

mlib_status
mlib_ImageReplaceColor_D64(
	const mlib_d64 * sl, mlib_d64 * dl, mlib_s32 sstride,
    mlib_s32 dstride, mlib_s32 width, mlib_s32 height,
    mlib_s32 nchan, const mlib_d64 * color1,
    const mlib_d64 * color2)
{
	mlib_s32 i, j;
	mlib_s64 s0, s1, s2, s3;
	mlib_d64 gtemp;
	mlib_s64 *sp = (mlib_s64 *)sl;
	mlib_s64 *dp = (mlib_s64 *)dl;

	if (nchan == 1) {
		GET_CONST(0, mlib_d64, mlib_s64);
		for (j = 0; j < height; j++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i <= width - 4; i += 4) {
				s0 = sp[i];
				dp[i] = PROCESS_CH(0);
				s0 = sp[i + 1];
				dp[i + 1] = PROCESS_CH(0);
				s0 = sp[i + 2];
				dp[i + 2] = PROCESS_CH(0);
				s0 = sp[i + 3];
				dp[i + 3] = PROCESS_CH(0);
			}

			for (; i < width; i++) {
				s0 = sp[i];
				dp[i] = PROCESS_CH(0);
			}

			sp += sstride;
			dp += dstride;
		}

	} else if (nchan == 2) {
		GET_CONST(0, mlib_d64, mlib_s64);
		GET_CONST(1, mlib_d64, mlib_s64);
		for (j = 0; j < height; j++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < width; i++) {
				s0 = sp[2 * i];
				s1 = sp[2 * i + 1];
				dp[2 * i] = PROCESS_CH(0);
				dp[2 * i + 1] = PROCESS_CH(1);
			}

			sp += sstride;
			dp += dstride;
		}

	} else if (nchan == 3) {
		GET_CONST(0, mlib_d64, mlib_s64);
		GET_CONST(1, mlib_d64, mlib_s64);
		GET_CONST(2, mlib_d64, mlib_s64);
		for (j = 0; j < height; j++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < width; i++) {
				s0 = sp[3 * i];
				s1 = sp[3 * i + 1];
				s2 = sp[3 * i + 2];
				dp[3 * i] = PROCESS_CH(0);
				dp[3 * i + 1] = PROCESS_CH(1);
				dp[3 * i + 2] = PROCESS_CH(2);
			}

			sp += sstride;
			dp += dstride;
		}

	} else {	/* if (nchan == 4) */

		GET_CONST(0, mlib_d64, mlib_s64);
		GET_CONST(1, mlib_d64, mlib_s64);
		GET_CONST(2, mlib_d64, mlib_s64);
		GET_CONST(3, mlib_d64, mlib_s64);
		for (j = 0; j < height; j++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < width; i++) {
				s0 = sp[4 * i];
				s1 = sp[4 * i + 1];
				s2 = sp[4 * i + 2];
				s3 = sp[4 * i + 3];
				dp[4 * i] = PROCESS_CH(0);
				dp[4 * i + 1] = PROCESS_CH(1);
				dp[4 * i + 2] = PROCESS_CH(2);
				dp[4 * i + 3] = PROCESS_CH(3);
			}

			sp += sstride;
			dp += dstride;
		}
	}

	return (MLIB_SUCCESS);

}
/* *********************************************************** */

mlib_status
__mlib_ImageReplaceColor_Fp_Inp(
    mlib_image *srcdst,
    const mlib_d64 *color1,
    const mlib_d64 *color2)
{
	return (__mlib_ImageReplaceColor_Fp(srcdst, srcdst, color1, color2));
}

/* *********************************************************** */
#endif
