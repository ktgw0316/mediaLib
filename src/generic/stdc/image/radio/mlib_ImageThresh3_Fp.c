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

#pragma ident	"@(#)mlib_ImageThresh3_Fp.c	9.2	07/10/09 SMI"

/*
 * FUNCTION
 *      mlib_ImageThresh3_Fp - thresholding
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageThresh3_Fp(mlib_image       *dst,
 *                                       const mlib_image *src,
 *                                       const mlib_d64   *thresh,
 *                                       const mlib_d64   *ghigh);
 *
 * ARGUMENT
 *      dst     pointer to output image
 *      src     pointer to input image
 *      thresh  array of thresholds
 *      ghigh   array of values above thresholds
 *
 * RESTRICTION
 *      The images must have the same type, the same size, and the same number
 *      of channels.
 *      The images can have 1, 2, 3, or 4 channels.
 *      The images can be in MLIB_FLOAT or MLIB_DOUBLE data type.
 *
 * DESCRIPTION
 *      If the pixel value is above the threshold for that channel,
 *      set the destination to the ghigh value for that channel.
 *      Otherwise, set the destination to the source.
 *
 *                      +- src[x][y][c]  src[x][y][c] <= thresh[c]
 *      dst[x][y][c]  = |
 *                      +- ghigh[c]      src[x][y][c] >  thresh[c]
 */

#include <mlib_image.h>
#include <mlib_ImageCheck.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageThresh3_Fp = __mlib_ImageThresh3_Fp

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_ImageThresh3_Fp) mlib_ImageThresh3_Fp
    __attribute__((weak, alias("__mlib_ImageThresh3_Fp")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

static void mlib_ImageThresh3_F321(
    const mlib_f32 *psrc,
    mlib_s32 src_stride,
    mlib_f32 *pdst,
    mlib_s32 dst_stride,
    mlib_s32 width,
    mlib_s32 height,
    const mlib_d64 *thresh,
    const mlib_d64 *ghigh);

static void mlib_ImageThresh3_F322(
    const mlib_f32 *psrc,
    mlib_s32 src_stride,
    mlib_f32 *pdst,
    mlib_s32 dst_stride,
    mlib_s32 width,
    mlib_s32 height,
    const mlib_d64 *thresh,
    const mlib_d64 *ghigh);

static void mlib_ImageThresh3_F323(
    const mlib_f32 *psrc,
    mlib_s32 src_stride,
    mlib_f32 *pdst,
    mlib_s32 dst_stride,
    mlib_s32 width,
    mlib_s32 height,
    const mlib_d64 *thresh,
    const mlib_d64 *ghigh);

static void mlib_ImageThresh3_F324(
    const mlib_f32 *psrc,
    mlib_s32 src_stride,
    mlib_f32 *pdst,
    mlib_s32 dst_stride,
    mlib_s32 width,
    mlib_s32 height,
    const mlib_d64 *thresh,
    const mlib_d64 *ghigh);

static void mlib_ImageThresh3_D641(
    const mlib_d64 *psrc,
    mlib_s32 src_stride,
    mlib_d64 *pdst,
    mlib_s32 dst_stride,
    mlib_s32 width,
    mlib_s32 height,
    const mlib_d64 *thresh,
    const mlib_d64 *ghigh);

static void mlib_ImageThresh3_D642(
    const mlib_d64 *psrc,
    mlib_s32 src_stride,
    mlib_d64 *pdst,
    mlib_s32 dst_stride,
    mlib_s32 width,
    mlib_s32 height,
    const mlib_d64 *thresh,
    const mlib_d64 *ghigh);

static void mlib_ImageThresh3_D643(
    const mlib_d64 *psrc,
    mlib_s32 src_stride,
    mlib_d64 *pdst,
    mlib_s32 dst_stride,
    mlib_s32 width,
    mlib_s32 height,
    const mlib_d64 *thresh,
    const mlib_d64 *ghigh);

static void mlib_ImageThresh3_D644(
    const mlib_d64 *psrc,
    mlib_s32 src_stride,
    mlib_d64 *pdst,
    mlib_s32 dst_stride,
    mlib_s32 width,
    mlib_s32 height,
    const mlib_d64 *thresh,
    const mlib_d64 *ghigh);

/* *********************************************************** */

mlib_status
__mlib_ImageThresh3_Fp(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_d64 *thresh,
    const mlib_d64 *ghigh)
{
	mlib_s32 sstride, dstride, width, height, nchan;
	mlib_type dtype;
	void *psrc, *pdst;

	MLIB_IMAGE_CHECK(src);
	MLIB_IMAGE_CHECK(dst);
	MLIB_IMAGE_FULL_EQUAL(src, dst);

	dtype = mlib_ImageGetType(src);
	nchan = mlib_ImageGetChannels(src);
	width = mlib_ImageGetWidth(src);
	height = mlib_ImageGetHeight(src);
	sstride = mlib_ImageGetStride(src);
	dstride = mlib_ImageGetStride(dst);
	psrc = mlib_ImageGetData(src);
	pdst = mlib_ImageGetData(dst);

	if (dtype == MLIB_FLOAT) {

		switch (nchan) {
		case 1:
			mlib_ImageThresh3_F321(psrc, sstride,
			    pdst, dstride, width, height, thresh, ghigh);
			break;
		case 2:
			mlib_ImageThresh3_F322(psrc, sstride,
			    pdst, dstride, width, height, thresh, ghigh);
			break;
		case 3:
			mlib_ImageThresh3_F323(psrc, sstride,
			    pdst, dstride, width, height, thresh, ghigh);
			break;
		case 4:
			mlib_ImageThresh3_F324(psrc, sstride,
			    pdst, dstride, width, height, thresh, ghigh);
			break;
		}

		return (MLIB_SUCCESS);
	} else if (dtype == MLIB_DOUBLE) {
		switch (nchan) {
		case 1:
			mlib_ImageThresh3_D641(psrc, sstride,
			    pdst, dstride, width, height, thresh, ghigh);
			break;
		case 2:
			mlib_ImageThresh3_D642(psrc, sstride,
			    pdst, dstride, width, height, thresh, ghigh);
			break;
		case 3:
			mlib_ImageThresh3_D643(psrc, sstride,
			    pdst, dstride, width, height, thresh, ghigh);
			break;
		case 4:
			mlib_ImageThresh3_D644(psrc, sstride,
			    pdst, dstride, width, height, thresh, ghigh);
			break;
		}

		return (MLIB_SUCCESS);
	}

	return (MLIB_FAILURE);
}

/* *********************************************************** */

#ifndef __INTEL_COMPILER

#define	INIT_THRESH(n)                          \
	mlib_f32 thresh##n = thresh[n];         \
	mlib_f32 high##n = ghigh[n]

#else

#define	INIT_THRESH(n)                          \
	volatile mlib_f32 tmp##n = thresh[n];   \
	mlib_f32 thresh##n = tmp##n;            \
	mlib_f32 high##n = ghigh[n]

#endif

/* *********************************************************** */

void
mlib_ImageThresh3_F321(
    const mlib_f32 *psrc,
    mlib_s32 src_stride,
    mlib_f32 *pdst,
    mlib_s32 dst_stride,
    mlib_s32 width,
    mlib_s32 height,
    const mlib_d64 *thresh,
    const mlib_d64 *ghigh)
{
	mlib_s32 i, j;
	mlib_f32 src0, src1, src2, src3, src4, src5, src6, src7;
	INIT_THRESH(0);

	src_stride >>= 2;
	dst_stride >>= 2;

	for (i = 0; i < height; i++) {
		const mlib_f32 *psrc_row = psrc + i * src_stride;
		mlib_f32 *pdst_row = pdst + i * dst_stride;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (j = 0; j <= (width - 8); j += 8) {
			src0 = psrc_row[j];
			src1 = psrc_row[j + 1];
			src2 = psrc_row[j + 2];
			src3 = psrc_row[j + 3];
			src4 = psrc_row[j + 4];
			src5 = psrc_row[j + 5];
			src6 = psrc_row[j + 6];
			src7 = psrc_row[j + 7];
			pdst_row[j    ] = (src0 <= thresh0) ? src0 : high0;
			pdst_row[j + 1] = (src1 <= thresh0) ? src1 : high0;
			pdst_row[j + 2] = (src2 <= thresh0) ? src2 : high0;
			pdst_row[j + 3] = (src3 <= thresh0) ? src3 : high0;
			pdst_row[j + 4] = (src4 <= thresh0) ? src4 : high0;
			pdst_row[j + 5] = (src5 <= thresh0) ? src5 : high0;
			pdst_row[j + 6] = (src6 <= thresh0) ? src6 : high0;
			pdst_row[j + 7] = (src7 <= thresh0) ? src7 : high0;
		}

		for (; j < width; j++) {
			src0 = psrc_row[j];
			pdst_row[j] = (src0 <= thresh0) ? src0 : high0;
		}
	}
}

/* *********************************************************** */

void
mlib_ImageThresh3_F322(
    const mlib_f32 *psrc,
    mlib_s32 src_stride,
    mlib_f32 *pdst,
    mlib_s32 dst_stride,
    mlib_s32 width,
    mlib_s32 height,
    const mlib_d64 *thresh,
    const mlib_d64 *ghigh)
{
	mlib_s32 i, j;
	mlib_f32 src0, src1, src2, src3, src4, src5, src6, src7;
	INIT_THRESH(0);
	INIT_THRESH(1);

	src_stride >>= 2;
	dst_stride >>= 2;
	width <<= 1;

	for (i = 0; i < height; i++) {
		const mlib_f32 *psrc_row = psrc + i * src_stride;
		mlib_f32 *pdst_row = pdst + i * dst_stride;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (j = 0; j <= (width - 8); j += 8) {
			src0 = psrc_row[j];
			src1 = psrc_row[j + 1];
			src2 = psrc_row[j + 2];
			src3 = psrc_row[j + 3];
			src4 = psrc_row[j + 4];
			src5 = psrc_row[j + 5];
			src6 = psrc_row[j + 6];
			src7 = psrc_row[j + 7];
			pdst_row[j] = (src0 <= thresh0) ? src0 : high0;
			pdst_row[j + 1] = (src1 <= thresh1) ? src1 : high1;
			pdst_row[j + 2] = (src2 <= thresh0) ? src2 : high0;
			pdst_row[j + 3] = (src3 <= thresh1) ? src3 : high1;
			pdst_row[j + 4] = (src4 <= thresh0) ? src4 : high0;
			pdst_row[j + 5] = (src5 <= thresh1) ? src5 : high1;
			pdst_row[j + 6] = (src6 <= thresh0) ? src6 : high0;
			pdst_row[j + 7] = (src7 <= thresh1) ? src7 : high1;
		}

		for (; j < width; j += 2) {
			src0 = psrc_row[j];
			src1 = psrc_row[j + 1];
			pdst_row[j] = (src0 <= thresh0) ? src0 : high0;
			pdst_row[j + 1] = (src1 <= thresh1) ? src1 : high1;
		}
	}
}

/* *********************************************************** */

void
mlib_ImageThresh3_F323(
    const mlib_f32 *psrc,
    mlib_s32 src_stride,
    mlib_f32 *pdst,
    mlib_s32 dst_stride,
    mlib_s32 width,
    mlib_s32 height,
    const mlib_d64 *thresh,
    const mlib_d64 *ghigh)
{
	mlib_s32 i, j;
	mlib_f32 src0, src1, src2, src3, src4, src5, src6, src7, src8, src9,
	    src10, src11;
	INIT_THRESH(0);
	INIT_THRESH(1);
	INIT_THRESH(2);

	src_stride >>= 2;
	dst_stride >>= 2;
	width = width * 3;

	for (i = 0; i < height; i++) {
		const mlib_f32 *psrc_row = psrc + i * src_stride;
		mlib_f32 *pdst_row = pdst + i * dst_stride;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (j = 0; j <= (width - 12); j += 12) {
			src0 = psrc_row[j];
			src1 = psrc_row[j + 1];
			src2 = psrc_row[j + 2];
			src3 = psrc_row[j + 3];
			src4 = psrc_row[j + 4];
			src5 = psrc_row[j + 5];
			src6 = psrc_row[j + 6];
			src7 = psrc_row[j + 7];
			src8 = psrc_row[j + 8];
			src9 = psrc_row[j + 9];
			src10 = psrc_row[j + 10];
			src11 = psrc_row[j + 11];
			pdst_row[j] = (src0 <= thresh0) ? src0 : high0;
			pdst_row[j + 1] = (src1 <= thresh1) ? src1 : high1;
			pdst_row[j + 2] = (src2 <= thresh2) ? src2 : high2;
			pdst_row[j + 3] = (src3 <= thresh0) ? src3 : high0;
			pdst_row[j + 4] = (src4 <= thresh1) ? src4 : high1;
			pdst_row[j + 5] = (src5 <= thresh2) ? src5 : high2;
			pdst_row[j + 6] = (src6 <= thresh0) ? src6 : high0;
			pdst_row[j + 7] = (src7 <= thresh1) ? src7 : high1;
			pdst_row[j + 8] = (src8 <= thresh2) ? src8 : high2;
			pdst_row[j + 9] = (src9 <= thresh0) ? src9 : high0;
			pdst_row[j + 10] = (src10 <= thresh1) ? src10 : high1;
			pdst_row[j + 11] = (src11 <= thresh2) ? src11 : high2;
		}

		for (; j < width; j += 3) {
			src0 = psrc_row[j];
			src1 = psrc_row[j + 1];
			src2 = psrc_row[j + 2];
			pdst_row[j] = (src0 <= thresh0) ? src0 : high0;
			pdst_row[j + 1] = (src1 <= thresh1) ? src1 : high1;
			pdst_row[j + 2] = (src2 <= thresh2) ? src2 : high2;
		}
	}
}

/* *********************************************************** */

void
mlib_ImageThresh3_F324(
    const mlib_f32 *psrc,
    mlib_s32 src_stride,
    mlib_f32 *pdst,
    mlib_s32 dst_stride,
    mlib_s32 width,
    mlib_s32 height,
    const mlib_d64 *thresh,
    const mlib_d64 *ghigh)
{
	mlib_s32 i, j;
	mlib_f32 src0, src1, src2, src3, src4, src5, src6, src7;
	INIT_THRESH(0);
	INIT_THRESH(1);
	INIT_THRESH(2);
	INIT_THRESH(3);

	src_stride >>= 2;
	dst_stride >>= 2;
	width <<= 2;

	for (i = 0; i < height; i++) {
		const mlib_f32 *psrc_row = psrc + i * src_stride;
		mlib_f32 *pdst_row = pdst + i * dst_stride;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (j = 0; j <= (width - 8); j += 8) {
			src0 = psrc_row[j];
			src1 = psrc_row[j + 1];
			src2 = psrc_row[j + 2];
			src3 = psrc_row[j + 3];
			src4 = psrc_row[j + 4];
			src5 = psrc_row[j + 5];
			src6 = psrc_row[j + 6];
			src7 = psrc_row[j + 7];
			pdst_row[j] = (src0 <= thresh0) ? src0 : high0;
			pdst_row[j + 1] = (src1 <= thresh1) ? src1 : high1;
			pdst_row[j + 2] = (src2 <= thresh2) ? src2 : high2;
			pdst_row[j + 3] = (src3 <= thresh3) ? src3 : high3;
			pdst_row[j + 4] = (src4 <= thresh0) ? src4 : high0;
			pdst_row[j + 5] = (src5 <= thresh1) ? src5 : high1;
			pdst_row[j + 6] = (src6 <= thresh2) ? src6 : high2;
			pdst_row[j + 7] = (src7 <= thresh3) ? src7 : high3;
		}

		for (; j < width; j += 4) {
			src0 = psrc_row[j];
			src1 = psrc_row[j + 1];
			src2 = psrc_row[j + 2];
			src3 = psrc_row[j + 3];
			pdst_row[j] = (src0 <= thresh0) ? src0 : high0;
			pdst_row[j + 1] = (src1 <= thresh1) ? src1 : high1;
			pdst_row[j + 2] = (src2 <= thresh2) ? src2 : high2;
			pdst_row[j + 3] = (src3 <= thresh3) ? src3 : high3;
		}
	}
}

/* *********************************************************** */

void
mlib_ImageThresh3_D641(
    const mlib_d64 *psrc,
    mlib_s32 src_stride,
    mlib_d64 *pdst,
    mlib_s32 dst_stride,
    mlib_s32 width,
    mlib_s32 height,
    const mlib_d64 *thresh,
    const mlib_d64 *ghigh)
{
	mlib_s32 i, j;
	mlib_d64 thresh0 = thresh[0];
	mlib_d64 high = ghigh[0];
	mlib_d64 src0, src1, src2, src3, src4, src5, src6, src7;

	src_stride >>= 3;
	dst_stride >>= 3;

	for (i = 0; i < height; i++) {
		const mlib_d64 *psrc_row = psrc + i * src_stride;
		mlib_d64 *pdst_row = pdst + i * dst_stride;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (j = 0; j <= (width - 8); j += 8) {
			src0 = psrc_row[j];
			src1 = psrc_row[j + 1];
			src2 = psrc_row[j + 2];
			src3 = psrc_row[j + 3];
			src4 = psrc_row[j + 4];
			src5 = psrc_row[j + 5];
			src6 = psrc_row[j + 6];
			src7 = psrc_row[j + 7];
			pdst_row[j] = (src0 <= thresh0) ? src0 : high;
			pdst_row[j + 1] = (src1 <= thresh0) ? src1 : high;
			pdst_row[j + 2] = (src2 <= thresh0) ? src2 : high;
			pdst_row[j + 3] = (src3 <= thresh0) ? src3 : high;
			pdst_row[j + 4] = (src4 <= thresh0) ? src4 : high;
			pdst_row[j + 5] = (src5 <= thresh0) ? src5 : high;
			pdst_row[j + 6] = (src6 <= thresh0) ? src6 : high;
			pdst_row[j + 7] = (src7 <= thresh0) ? src7 : high;
		}

		for (; j < width; j++) {
			src0 = psrc_row[j];
			pdst_row[j] = (src0 <= thresh0) ? src0 : high;
		}
	}
}

/* *********************************************************** */

void
mlib_ImageThresh3_D642(
    const mlib_d64 *psrc,
    mlib_s32 src_stride,
    mlib_d64 *pdst,
    mlib_s32 dst_stride,
    mlib_s32 width,
    mlib_s32 height,
    const mlib_d64 *thresh,
    const mlib_d64 *ghigh)
{
	mlib_s32 i, j;
	mlib_d64 thresh0 = thresh[0], thresh1 = thresh[1];
	mlib_d64 high0 = ghigh[0], high1 = ghigh[1];
	mlib_d64 src0, src1, src2, src3, src4, src5, src6, src7;

	src_stride >>= 3;
	dst_stride >>= 3;
	width <<= 1;

	for (i = 0; i < height; i++) {
		const mlib_d64 *psrc_row = psrc + i * src_stride;
		mlib_d64 *pdst_row = pdst + i * dst_stride;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (j = 0; j <= (width - 8); j += 8) {
			src0 = psrc_row[j];
			src1 = psrc_row[j + 1];
			src2 = psrc_row[j + 2];
			src3 = psrc_row[j + 3];
			src4 = psrc_row[j + 4];
			src5 = psrc_row[j + 5];
			src6 = psrc_row[j + 6];
			src7 = psrc_row[j + 7];
			pdst_row[j] = (src0 <= thresh0) ? src0 : high0;
			pdst_row[j + 1] = (src1 <= thresh1) ? src1 : high1;
			pdst_row[j + 2] = (src2 <= thresh0) ? src2 : high0;
			pdst_row[j + 3] = (src3 <= thresh1) ? src3 : high1;
			pdst_row[j + 4] = (src4 <= thresh0) ? src4 : high0;
			pdst_row[j + 5] = (src5 <= thresh1) ? src5 : high1;
			pdst_row[j + 6] = (src6 <= thresh0) ? src6 : high0;
			pdst_row[j + 7] = (src7 <= thresh1) ? src7 : high1;
		}

		for (; j < width; j += 2) {
			src0 = psrc_row[j];
			src1 = psrc_row[j + 1];
			pdst_row[j] = (src0 <= thresh0) ? src0 : high0;
			pdst_row[j + 1] = (src1 <= thresh1) ? src1 : high1;
		}
	}
}

/* *********************************************************** */

void
mlib_ImageThresh3_D643(
    const mlib_d64 *psrc,
    mlib_s32 src_stride,
    mlib_d64 *pdst,
    mlib_s32 dst_stride,
    mlib_s32 width,
    mlib_s32 height,
    const mlib_d64 *thresh,
    const mlib_d64 *ghigh)
{
	mlib_s32 i, j;
	mlib_d64 thresh0 = thresh[0], thresh1 = thresh[1];
	mlib_d64 thresh2 = thresh[2];
	mlib_d64 high0 = ghigh[0];
	mlib_d64 high1 = ghigh[1];
	mlib_d64 high2 = ghigh[2];
	mlib_d64 src0, src1, src2, src3, src4, src5, src6, src7, src8, src9,
	    src10, src11;

	src_stride >>= 3;
	dst_stride >>= 3;
	width = width * 3;

	for (i = 0; i < height; i++) {
		const mlib_d64 *psrc_row = psrc + i * src_stride;
		mlib_d64 *pdst_row = pdst + i * dst_stride;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (j = 0; j <= (width - 12); j += 12) {
			src0 = psrc_row[j];
			src1 = psrc_row[j + 1];
			src2 = psrc_row[j + 2];
			src3 = psrc_row[j + 3];
			src4 = psrc_row[j + 4];
			src5 = psrc_row[j + 5];
			src6 = psrc_row[j + 6];
			src7 = psrc_row[j + 7];
			src8 = psrc_row[j + 8];
			src9 = psrc_row[j + 9];
			src10 = psrc_row[j + 10];
			src11 = psrc_row[j + 11];
			pdst_row[j] = (src0 <= thresh0) ? src0 : high0;
			pdst_row[j + 1] = (src1 <= thresh1) ? src1 : high1;
			pdst_row[j + 2] = (src2 <= thresh2) ? src2 : high2;
			pdst_row[j + 3] = (src3 <= thresh0) ? src3 : high0;
			pdst_row[j + 4] = (src4 <= thresh1) ? src4 : high1;
			pdst_row[j + 5] = (src5 <= thresh2) ? src5 : high2;
			pdst_row[j + 6] = (src6 <= thresh0) ? src6 : high0;
			pdst_row[j + 7] = (src7 <= thresh1) ? src7 : high1;
			pdst_row[j + 8] = (src8 <= thresh2) ? src8 : high2;
			pdst_row[j + 9] = (src9 <= thresh0) ? src9 : high0;
			pdst_row[j + 10] = (src10 <= thresh1) ? src10 : high1;
			pdst_row[j + 11] = (src11 <= thresh2) ? src11 : high2;
		}

		for (; j < width; j += 3) {
			src0 = psrc_row[j];
			src1 = psrc_row[j + 1];
			src2 = psrc_row[j + 2];
			pdst_row[j] = (src0 <= thresh0) ? src0 : high0;
			pdst_row[j + 1] = (src1 <= thresh1) ? src1 : high1;
			pdst_row[j + 2] = (src2 <= thresh2) ? src2 : high2;
		}
	}
}

/* *********************************************************** */

void
mlib_ImageThresh3_D644(
    const mlib_d64 *psrc,
    mlib_s32 src_stride,
    mlib_d64 *pdst,
    mlib_s32 dst_stride,
    mlib_s32 width,
    mlib_s32 height,
    const mlib_d64 *thresh,
    const mlib_d64 *ghigh)
{
	mlib_s32 i, j;
	mlib_d64 thresh0 = thresh[0], thresh1 = thresh[1];
	mlib_d64 thresh2 = thresh[2], thresh3 = thresh[3];
	mlib_d64 high0 = ghigh[0], high1 = ghigh[1];
	mlib_d64 high2 = ghigh[2], high3 = ghigh[3];
	mlib_d64 src0, src1, src2, src3, src4, src5, src6, src7;

	src_stride >>= 3;
	dst_stride >>= 3;
	width <<= 2;

	for (i = 0; i < height; i++) {
		const mlib_d64 *psrc_row = psrc + i * src_stride;
		mlib_d64 *pdst_row = pdst + i * dst_stride;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (j = 0; j <= (width - 8); j += 8) {
			src0 = psrc_row[j];
			src1 = psrc_row[j + 1];
			src2 = psrc_row[j + 2];
			src3 = psrc_row[j + 3];
			src4 = psrc_row[j + 4];
			src5 = psrc_row[j + 5];
			src6 = psrc_row[j + 6];
			src7 = psrc_row[j + 7];
			pdst_row[j] = (src0 <= thresh0) ? src0 : high0;
			pdst_row[j + 1] = (src1 <= thresh1) ? src1 : high1;
			pdst_row[j + 2] = (src2 <= thresh2) ? src2 : high2;
			pdst_row[j + 3] = (src3 <= thresh3) ? src3 : high3;
			pdst_row[j + 4] = (src4 <= thresh0) ? src4 : high0;
			pdst_row[j + 5] = (src5 <= thresh1) ? src5 : high1;
			pdst_row[j + 6] = (src6 <= thresh2) ? src6 : high2;
			pdst_row[j + 7] = (src7 <= thresh3) ? src7 : high3;
		}

		for (; j < width; j += 4) {
			src0 = psrc_row[j];
			src1 = psrc_row[j + 1];
			src2 = psrc_row[j + 2];
			src3 = psrc_row[j + 3];
			pdst_row[j] = (src0 <= thresh0) ? src0 : high0;
			pdst_row[j + 1] = (src1 <= thresh1) ? src1 : high1;
			pdst_row[j + 2] = (src2 <= thresh2) ? src2 : high2;
			pdst_row[j + 3] = (src3 <= thresh3) ? src3 : high3;
		}
	}
}

/* *********************************************************** */
