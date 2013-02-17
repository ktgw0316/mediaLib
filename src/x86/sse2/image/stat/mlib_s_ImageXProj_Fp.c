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

#pragma ident	"@(#)mlib_s_ImageXProj_Fp.c	9.4	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_ImageXProj_Fp - calculates X Projections for input image
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageXProj_Fp(mlib_d64         *xproj,
 *                                     const mlib_image *src)
 *
 * ARGUMENTS
 *      xproj       Pointer to X-projection vector
 */

#include <mlib_image.h>
#include <mlib_ImageCheck.h>
#ifdef __SUNPRO_C
#include <sunmedia_intrin.h>
#else
#include <emmintrin.h>
#endif

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageXProj_Fp = __mlib_ImageXProj_Fp

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_ImageXProj_Fp) mlib_ImageXProj_Fp
    __attribute__((weak, alias("__mlib_ImageXProj_Fp")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

static void mlib_ImageXProj_F32(
    const mlib_image *src,
    mlib_d64 *xproj);
static void mlib_ImageXProj_D64(
    const mlib_image *src,
    mlib_d64 *xproj);

/* *********************************************************** */

mlib_status
__mlib_ImageXProj_Fp(
    mlib_d64 *xproj,
    const mlib_image *src)
{
/* check for obvious errors */
	MLIB_IMAGE_CHECK(src);
	MLIB_IMAGE_HAVE_CHAN(src, 1);

	if (xproj == NULL)
		return (MLIB_NULLPOINTER);

	switch (mlib_ImageGetType(src)) {
	case MLIB_FLOAT:
		mlib_ImageXProj_F32(src, xproj);
		break;

	case MLIB_DOUBLE:
		mlib_ImageXProj_D64(src, xproj);
		break;

	default:
		return (MLIB_FAILURE);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

void
mlib_ImageXProj_F32(
    const mlib_image *src,
    mlib_d64 *xproj)
{
/* width of source image */
	mlib_s32 width = mlib_ImageGetWidth(src);
/* height of source image */
	mlib_s32 height = mlib_ImageGetHeight(src);
/* stride of source image */
	mlib_s32 stride = mlib_ImageGetStride(src);

	mlib_f32 *sp = mlib_ImageGetData(src);
	mlib_f32 *spl;

	mlib_s32 i, j, k;

	for (i = 0; i <= width - 4; i += 4) {
		spl = sp + i;
		__m128d x_sum0_d64, x_sum1_d64;
		x_sum0_d64 = x_sum1_d64 = _mm_setzero_pd();

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#pragma unroll(4)
#endif /* __SUNPRO_C */
		for (j = 0; j < height; j++) {
			__m128d x_s0, x_s1;
			x_s0 = _mm_cvtps_pd(*((__m128 *)spl));
			x_s1 = _mm_cvtps_pd(*((__m128 *)(spl + 2)));
			x_sum0_d64 = _mm_add_pd(x_sum0_d64, x_s0);

			x_sum1_d64 = _mm_add_pd(x_sum1_d64, x_s1);
			spl = (mlib_f32 *)((mlib_u8 *)spl + stride);
		}

		_mm_storeu_pd((xproj + i), x_sum0_d64);
		_mm_storeu_pd((xproj + i + 2), x_sum1_d64);
	}

	if (i <= width - 2) {
		spl = sp + i;
		__m128d x_sum0_d64 = _mm_setzero_pd();

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#pragma unroll(4)
#endif /* __SUNPRO_C */
		for (j = 0; j < height; j++) {
			__m128d x_s0;
			x_s0 = _mm_cvtps_pd(*((__m128 *)spl));
			x_sum0_d64 = _mm_add_pd(x_sum0_d64, x_s0);
			spl = (mlib_f32 *)((mlib_u8 *)spl + stride);
		}

		_mm_storeu_pd((xproj + i), x_sum0_d64);
		i += 2;
	}

	if (i < width) {
		spl = sp + i;
		mlib_d64 accm = 0;
		for (j = 0; j < height; j++) {
			accm += *spl;
			spl = (mlib_f32 *)((mlib_u8 *)spl + stride);
		}

		xproj[i] = (mlib_d64)accm;
	}
}

/* *********************************************************** */

/* Pure C Implementation */

void
mlib_ImageXProj_D64(
    const mlib_image *src,
    mlib_d64 *xproj)
{
/* width of source image */
	mlib_s32 Width = mlib_ImageGetWidth(src),
/* height of source image */
	    Height = mlib_ImageGetHeight(src),
/* bytes to next row */
	    YStride = mlib_ImageGetStride(src) >> 3;
	mlib_d64 *LinePntr = (mlib_d64 *)mlib_ImageGetData(src), *PixelPntr;
	mlib_s32 i, j;
	mlib_d64 Accum0, Accum1, Accum2, Accum3;

	for (i = 0; i <= Width - 4; i += 4) {
		Accum0 = Accum1 = Accum2 = Accum3 = 0;
		PixelPntr = LinePntr + i;

		for (j = 0; j < Height; j++) {
			Accum0 += *PixelPntr;
			Accum1 += *(PixelPntr + 1);
			Accum2 += *(PixelPntr + 2);
			Accum3 += *(PixelPntr + 3);
			PixelPntr += YStride;
		}

		xproj[i] = Accum0;
		xproj[i + 1] = Accum1;
		xproj[i + 2] = Accum2;
		xproj[i + 3] = Accum3;
	}

	for (; i < Width; i++) {
		Accum0 = 0;
		PixelPntr = LinePntr + i;

		for (j = 0; j < Height; j++) {
			Accum0 += *(PixelPntr);
			PixelPntr += YStride;
		}

		xproj[i] = Accum0;
	}
}

/* *********************************************************** */
