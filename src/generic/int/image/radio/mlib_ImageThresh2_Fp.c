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

#pragma ident	"@(#)mlib_ImageThresh2_Fp.c	9.2	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_ImageThresh2_Fp - thresholding
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageThresh2_Fp(mlib_image       *dst,
 *                                       const mlib_image *src,
 *                                       const mlib_d64   *thresh,
 *                                       const mlib_d64   *glow);
 *
 * ARGUMENT
 *      dst     pointer to output image
 *      src     pointer to input image
 *      thresh  array of thresholds
 *      glow    array of values below thresholds
 *
 * RESTRICTION
 *      The images must have the same type, the same size, and the same number
 *      of channels.
 *      The images can have 1, 2, 3, or 4 channels.
 *      The images can be in MLIB_FLOAT or MLIB_DOUBLE data type.
 *
 * DESCRIPTION
 *      If the pixel value is below the threshold for that channel,
 *      set the destination to the glow value for that channel.
 *      Otherwise, set the destination to the source.
 *
 *                      +- glow[c]       src[x][y][c] <= thresh[c]
 *      dst[x][y][c]  = |
 *                      +- src[x][y][c]  src[x][y][c] >  thresh[c]
 */

#include <mlib_image.h>
#include <mlib_ImageCheck.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageThresh2_Fp = __mlib_ImageThresh2_Fp

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_ImageThresh2_Fp) mlib_ImageThresh2_Fp
    __attribute__((weak, alias("__mlib_ImageThresh2_Fp")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

static void mlib_ImageThresh2_F321(
    const mlib_f32 *psrc,
    mlib_s32 src_stride,
    mlib_f32 *pdst,
    mlib_s32 dst_stride,
    mlib_s32 width,
    mlib_s32 height,
    const mlib_d64 *thresh,
    const mlib_d64 *glow);

static void mlib_ImageThresh2_F322(
    const mlib_f32 *psrc,
    mlib_s32 src_stride,
    mlib_f32 *pdst,
    mlib_s32 dst_stride,
    mlib_s32 width,
    mlib_s32 height,
    const mlib_d64 *thresh,
    const mlib_d64 *glow);

static void mlib_ImageThresh2_F323(
    const mlib_f32 *psrc,
    mlib_s32 src_stride,
    mlib_f32 *pdst,
    mlib_s32 dst_stride,
    mlib_s32 width,
    mlib_s32 height,
    const mlib_d64 *thresh,
    const mlib_d64 *glow);

static void mlib_ImageThresh2_F324(
    const mlib_f32 *psrc,
    mlib_s32 src_stride,
    mlib_f32 *pdst,
    mlib_s32 dst_stride,
    mlib_s32 width,
    mlib_s32 height,
    const mlib_d64 *thresh,
    const mlib_d64 *glow);

static void mlib_ImageThresh2_D641(
    const mlib_d64 *psrc,
    mlib_s32 src_stride,
    mlib_d64 *pdst,
    mlib_s32 dst_stride,
    mlib_s32 width,
    mlib_s32 height,
    const mlib_d64 *thresh,
    const mlib_d64 *glow);

static void mlib_ImageThresh2_D642(
    const mlib_d64 *psrc,
    mlib_s32 src_stride,
    mlib_d64 *pdst,
    mlib_s32 dst_stride,
    mlib_s32 width,
    mlib_s32 height,
    const mlib_d64 *thresh,
    const mlib_d64 *glow);

static void mlib_ImageThresh2_D643(
    const mlib_d64 *psrc,
    mlib_s32 src_stride,
    mlib_d64 *pdst,
    mlib_s32 dst_stride,
    mlib_s32 width,
    mlib_s32 height,
    const mlib_d64 *thresh,
    const mlib_d64 *glow);

static void mlib_ImageThresh2_D644(
    const mlib_d64 *psrc,
    mlib_s32 src_stride,
    mlib_d64 *pdst,
    mlib_s32 dst_stride,
    mlib_s32 width,
    mlib_s32 height,
    const mlib_d64 *thresh,
    const mlib_d64 *glow);

/* *********************************************************** */

mlib_status
__mlib_ImageThresh2_Fp(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_d64 *thresh,
    const mlib_d64 *glow)
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
			mlib_ImageThresh2_F321(psrc, sstride, pdst, dstride,
			    width, height, thresh, glow);
			break;
		case 2:
			mlib_ImageThresh2_F322(psrc, sstride, pdst, dstride,
			    width, height, thresh, glow);
			break;
		case 3:
			mlib_ImageThresh2_F323(psrc, sstride, pdst, dstride,
			    width, height, thresh, glow);
			break;
		case 4:
			mlib_ImageThresh2_F324(psrc, sstride, pdst, dstride,
			    width, height, thresh, glow);
			break;
		}

		return (MLIB_SUCCESS);
	} else if (dtype == MLIB_DOUBLE) {

		switch (nchan) {
		case 1:
			mlib_ImageThresh2_D641(psrc, sstride, pdst, dstride,
			    width, height, thresh, glow);
			break;
		case 2:
			mlib_ImageThresh2_D642(psrc, sstride, pdst, dstride,
			    width, height, thresh, glow);
			break;
		case 3:
			mlib_ImageThresh2_D643(psrc, sstride, pdst, dstride,
			    width, height, thresh, glow);
			break;
		case 4:
			mlib_ImageThresh2_D644(psrc, sstride, pdst, dstride,
			    width, height, thresh, glow);
			break;
		}

		return (MLIB_SUCCESS);
	}

	return (MLIB_FAILURE);
}

#ifndef __INTEL_COMPILER

#define	INIT_THRESH(n)                                 \
	gtemp = thresh[n];                                 \
	mlib_s32 thresh##n = *((mlib_s32 *) &gtemp);       \
	gtemp = glow[n];                                   \
	mlib_s32 low##n = *((mlib_s32 *) &gtemp)

#else

#define	INIT_THRESH(n)                                 \
	volatile mlib_f32 tmp##n = thresh[n];              \
	mlib_s32 thresh##n = *((mlib_s32 *) &tmp##n);      \
	tmp##n = glow[n];                                  \
	mlib_s32 low##n = *((mlib_s32 *) &tmp##n)

#endif

#define	DO_THRESH_F32(src, thresh, low)                \
	((src & 0x80000000) && (thresh & 0x80000000)) ?    \
		((src < thresh) ? src : low) :                 \
		((src <= thresh) ? low : src)

#define	DO_THRESH_D64(src, thresh, low)                \
	((src & 0x8000000000000000) &&                     \
		(thresh & 0x8000000000000000)) ?               \
		((src < thresh) ? src : low) :                 \
		((src <= thresh) ? low : src)

#define	THRESH0(s0, t)	DO_THRESH_##t(s0, thresh0, low0)
#define	THRESH1(s0, t)	DO_THRESH_##t(s0, thresh1, low1)
#define	THRESH2(s0, t)	DO_THRESH_##t(s0, thresh2, low2)
#define	THRESH3(s0, t)	DO_THRESH_##t(s0, thresh3, low3)

/* *********************************************************** */

void
mlib_ImageThresh2_F321(
    const mlib_f32 *psrc,
    mlib_s32 src_stride,
    mlib_f32 *pdst,
    mlib_s32 dst_stride,
    mlib_s32 width,
    mlib_s32 height,
    const mlib_d64 *thresh,
    const mlib_d64 *glow)
{
	mlib_s32 i, j;
	mlib_s32 src0, src1, src2, src3, src4, src5, src6, src7;
	mlib_f32 gtemp;
	INIT_THRESH(0);

	src_stride >>= 2;
	dst_stride >>= 2;

	for (i = 0; i < height; i++) {
		const mlib_s32 *psrc_row = (mlib_s32 *)psrc + i * src_stride;
		mlib_s32 *pdst_row = (mlib_s32 *)pdst + i * dst_stride;

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

			pdst_row[j    ] = THRESH0(src0, F32);
			pdst_row[j + 1] = THRESH0(src1, F32);
			pdst_row[j + 2] = THRESH0(src2, F32);
			pdst_row[j + 3] = THRESH0(src3, F32);
			pdst_row[j + 4] = THRESH0(src4, F32);
			pdst_row[j + 5] = THRESH0(src5, F32);
			pdst_row[j + 6] = THRESH0(src6, F32);
			pdst_row[j + 7] = THRESH0(src7, F32);

		}

		for (; j < width; j++) {
			src0 = psrc_row[j];
			pdst_row[j    ] = THRESH0(src0, F32);
		}
	}
}

/* *********************************************************** */

void
mlib_ImageThresh2_F322(
    const mlib_f32 *psrc,
    mlib_s32 src_stride,
    mlib_f32 *pdst,
    mlib_s32 dst_stride,
    mlib_s32 width,
    mlib_s32 height,
    const mlib_d64 *thresh,
    const mlib_d64 *glow)
{
	mlib_s32 i, j;
	mlib_s32 src0, src1, src2, src3, src4, src5, src6, src7;
	mlib_f32 gtemp;
	INIT_THRESH(0);
	INIT_THRESH(1);

	src_stride >>= 2;
	dst_stride >>= 2;
	width <<= 1;

	for (i = 0; i < height; i++) {
		const mlib_s32 *psrc_row = (mlib_s32 *)psrc + i * src_stride;
		mlib_s32 *pdst_row = (mlib_s32 *)pdst + i * dst_stride;

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

			pdst_row[j    ] = THRESH0(src0, F32);
			pdst_row[j + 1] = THRESH1(src1, F32);
			pdst_row[j + 2] = THRESH0(src2, F32);
			pdst_row[j + 3] = THRESH1(src3, F32);
			pdst_row[j + 4] = THRESH0(src4, F32);
			pdst_row[j + 5] = THRESH1(src5, F32);
			pdst_row[j + 6] = THRESH0(src6, F32);
			pdst_row[j + 7] = THRESH1(src7, F32);


		}
		for (; j < width; j += 2) {
			src0 = psrc_row[j];
			src1 = psrc_row[j + 1];
			pdst_row[j    ] = THRESH0(src0, F32);
			pdst_row[j + 1] = THRESH1(src1, F32);
		}
	}
}

/* *********************************************************** */

void
mlib_ImageThresh2_F323(
    const mlib_f32 *psrc,
    mlib_s32 src_stride,
    mlib_f32 *pdst,
    mlib_s32 dst_stride,
    mlib_s32 width,
    mlib_s32 height,
    const mlib_d64 *thresh,
    const mlib_d64 *glow)
{
	mlib_s32 i, j;
	mlib_s32 src0, src1, src2, src3, src4, src5, src6, src7, src8, src9,
	    src10, src11;
	mlib_f32 gtemp;
	INIT_THRESH(0);
	INIT_THRESH(1);
	INIT_THRESH(2);

	src_stride >>= 2;
	dst_stride >>= 2;
	width = width * 3;

	for (i = 0; i < height; i++) {
		const mlib_s32 *psrc_row = (mlib_s32 *)psrc + i * src_stride;
		mlib_s32 *pdst_row = (mlib_s32 *)pdst + i * dst_stride;

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


			pdst_row[j    ] = THRESH0(src0, F32);
			pdst_row[j + 1] = THRESH1(src1, F32);
			pdst_row[j + 2] = THRESH2(src2, F32);
			pdst_row[j + 3] = THRESH0(src3, F32);
			pdst_row[j + 4] = THRESH1(src4, F32);
			pdst_row[j + 5] = THRESH2(src5, F32);
			pdst_row[j + 6] = THRESH0(src6, F32);
			pdst_row[j + 7] = THRESH1(src7, F32);
			pdst_row[j + 8] = THRESH2(src8, F32);
			pdst_row[j + 9] = THRESH0(src9, F32);
			pdst_row[j + 10] = THRESH1(src10, F32);
			pdst_row[j + 11] = THRESH2(src11, F32);

		}

		for (; j < width; j += 3) {
			src0 = psrc_row[j];
			src1 = psrc_row[j + 1];
			src2 = psrc_row[j + 2];

			pdst_row[j    ] = THRESH0(src0, F32);
			pdst_row[j + 1] = THRESH1(src1, F32);
			pdst_row[j + 2] = THRESH2(src2, F32);

		}
	}
}

/* *********************************************************** */

void
mlib_ImageThresh2_F324(
    const mlib_f32 *psrc,
    mlib_s32 src_stride,
    mlib_f32 *pdst,
    mlib_s32 dst_stride,
    mlib_s32 width,
    mlib_s32 height,
    const mlib_d64 *thresh,
    const mlib_d64 *glow)
{
	mlib_s32 i, j;
	mlib_s32 src0, src1, src2, src3, src4, src5, src6, src7;
	mlib_f32 gtemp;
	INIT_THRESH(0);
	INIT_THRESH(1);
	INIT_THRESH(2);
	INIT_THRESH(3);

	src_stride >>= 2;
	dst_stride >>= 2;
	width <<= 2;

	for (i = 0; i < height; i++) {
		const mlib_s32 *psrc_row = (mlib_s32 *)psrc + i * src_stride;
		mlib_s32 *pdst_row = (mlib_s32 *)pdst + i * dst_stride;

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

			pdst_row[j    ] = THRESH0(src0, F32);
			pdst_row[j + 1] = THRESH1(src1, F32);
			pdst_row[j + 2] = THRESH2(src2, F32);
			pdst_row[j + 3] = THRESH3(src3, F32);
			pdst_row[j + 4] = THRESH0(src4, F32);
			pdst_row[j + 5] = THRESH1(src5, F32);
			pdst_row[j + 6] = THRESH2(src6, F32);
			pdst_row[j + 7] = THRESH3(src7, F32);

		}

		for (; j < width; j += 4) {
			src0 = psrc_row[j];
			src1 = psrc_row[j + 1];
			src2 = psrc_row[j + 2];
			src3 = psrc_row[j + 3];

			pdst_row[j    ] = THRESH0(src0, F32);
			pdst_row[j + 1] = THRESH1(src1, F32);
			pdst_row[j + 2] = THRESH2(src2, F32);
			pdst_row[j + 3] = THRESH3(src3, F32);

		}
	}
}

/* *********************************************************** */


#if defined(_NO_LONGLONG)
/* *********************************************************** */
void
mlib_ImageThresh2_D641(
    const mlib_d64 *psrc,
    mlib_s32 src_stride,
    mlib_d64 *pdst,
    mlib_s32 dst_stride,
    mlib_s32 width,
    mlib_s32 height,
    const mlib_d64 *thresh,
    const mlib_d64 *glow)
{
	mlib_s32 i, j;
	mlib_d64 thresh0 = thresh[0];
	mlib_d64 low = glow[0];
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
			pdst_row[j] = (src0 <= thresh0) ? low : src0;
			pdst_row[j + 1] = (src1 <= thresh0) ? low : src1;
			pdst_row[j + 2] = (src2 <= thresh0) ? low : src2;
			pdst_row[j + 3] = (src3 <= thresh0) ? low : src3;
			pdst_row[j + 4] = (src4 <= thresh0) ? low : src4;
			pdst_row[j + 5] = (src5 <= thresh0) ? low : src5;
			pdst_row[j + 6] = (src6 <= thresh0) ? low : src6;
			pdst_row[j + 7] = (src7 <= thresh0) ? low : src7;
		}

		for (; j < width; j++) {
			src0 = psrc_row[j];
			pdst_row[j] = (src0 <= thresh0) ? low : src0;
		}
	}
}

/* *********************************************************** */

void
mlib_ImageThresh2_D642(
    const mlib_d64 *psrc,
    mlib_s32 src_stride,
    mlib_d64 *pdst,
    mlib_s32 dst_stride,
    mlib_s32 width,
    mlib_s32 height,
    const mlib_d64 *thresh,
    const mlib_d64 *glow)
{
	mlib_s32 i, j;
	mlib_d64 thresh0 = thresh[0], thresh1 = thresh[1];
	mlib_d64 low0 = glow[0], low1 = glow[1];
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
			pdst_row[j] = (src0 <= thresh0) ? low0 : src0;
			pdst_row[j + 1] = (src1 <= thresh1) ? low1 : src1;
			pdst_row[j + 2] = (src2 <= thresh0) ? low0 : src2;
			pdst_row[j + 3] = (src3 <= thresh1) ? low1 : src3;
			pdst_row[j + 4] = (src4 <= thresh0) ? low0 : src4;
			pdst_row[j + 5] = (src5 <= thresh1) ? low1 : src5;
			pdst_row[j + 6] = (src6 <= thresh0) ? low0 : src6;
			pdst_row[j + 7] = (src7 <= thresh1) ? low1 : src7;
		}

		for (; j < width; j += 2) {
			src0 = psrc_row[j];
			src1 = psrc_row[j + 1];
			pdst_row[j] = (src0 <= thresh0) ? low0 : src0;
			pdst_row[j + 1] = (src1 <= thresh1) ? low1 : src1;
		}
	}
}

/* *********************************************************** */

void
mlib_ImageThresh2_D643(
    const mlib_d64 *psrc,
    mlib_s32 src_stride,
    mlib_d64 *pdst,
    mlib_s32 dst_stride,
    mlib_s32 width,
    mlib_s32 height,
    const mlib_d64 *thresh,
    const mlib_d64 *glow)
{
	mlib_s32 i, j;
	mlib_d64 thresh0 = thresh[0], thresh1 = thresh[1];
	mlib_d64 thresh2 = thresh[2];
	mlib_d64 low0 = glow[0], low1 = glow[1], low2 = glow[2];
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
			pdst_row[j] = (src0 <= thresh0) ? low0 : src0;
			pdst_row[j + 1] = (src1 <= thresh1) ? low1 : src1;
			pdst_row[j + 2] = (src2 <= thresh2) ? low2 : src2;
			pdst_row[j + 3] = (src3 <= thresh0) ? low0 : src3;
			pdst_row[j + 4] = (src4 <= thresh1) ? low1 : src4;
			pdst_row[j + 5] = (src5 <= thresh2) ? low2 : src5;
			pdst_row[j + 6] = (src6 <= thresh0) ? low0 : src6;
			pdst_row[j + 7] = (src7 <= thresh1) ? low1 : src7;
			pdst_row[j + 8] = (src8 <= thresh2) ? low2 : src8;
			pdst_row[j + 9] = (src9 <= thresh0) ? low0 : src9;
			pdst_row[j + 10] = (src10 <= thresh1) ? low1 : src10;
			pdst_row[j + 11] = (src11 <= thresh2) ? low2 : src11;
		}

		for (; j < width; j += 3) {
			src0 = psrc_row[j];
			src1 = psrc_row[j + 1];
			src2 = psrc_row[j + 2];
			pdst_row[j] = (src0 <= thresh0) ? low0 : src0;
			pdst_row[j + 1] = (src1 <= thresh1) ? low1 : src1;
			pdst_row[j + 2] = (src2 <= thresh2) ? low2 : src2;
		}
	}
}

/* *********************************************************** */

void
mlib_ImageThresh2_D644(
    const mlib_d64 *psrc,
    mlib_s32 src_stride,
    mlib_d64 *pdst,
    mlib_s32 dst_stride,
    mlib_s32 width,
    mlib_s32 height,
    const mlib_d64 *thresh,
    const mlib_d64 *glow)
{
	mlib_s32 i, j;
	mlib_d64 thresh0 = thresh[0], thresh1 = thresh[1];
	mlib_d64 thresh2 = thresh[2], thresh3 = thresh[3];
	mlib_d64 low0 = glow[0], low1 = glow[1];
	mlib_d64 low2 = glow[2], low3 = glow[3];
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
			pdst_row[j] = (src0 <= thresh0) ? low0 : src0;
			pdst_row[j + 1] = (src1 <= thresh1) ? low1 : src1;
			pdst_row[j + 2] = (src2 <= thresh2) ? low2 : src2;
			pdst_row[j + 3] = (src3 <= thresh3) ? low3 : src3;
			pdst_row[j + 4] = (src4 <= thresh0) ? low0 : src4;
			pdst_row[j + 5] = (src5 <= thresh1) ? low1 : src5;
			pdst_row[j + 6] = (src6 <= thresh2) ? low2 : src6;
			pdst_row[j + 7] = (src7 <= thresh3) ? low3 : src7;
		}

		for (; j < width; j += 4) {
			src0 = psrc_row[j];
			src1 = psrc_row[j + 1];
			src2 = psrc_row[j + 2];
			src3 = psrc_row[j + 3];
			pdst_row[j] = (src0 <= thresh0) ? low0 : src0;
			pdst_row[j + 1] = (src1 <= thresh1) ? low1 : src1;
			pdst_row[j + 2] = (src2 <= thresh2) ? low2 : src2;
			pdst_row[j + 3] = (src3 <= thresh3) ? low3 : src3;
		}
	}
}

/* *********************************************************** */
#else
/* *********************************************************** */
void
mlib_ImageThresh2_D641(
    const mlib_d64 *psrc,
    mlib_s32 src_stride,
    mlib_d64 *pdst,
    mlib_s32 dst_stride,
    mlib_s32 width,
    mlib_s32 height,
    const mlib_d64 *thresh,
    const mlib_d64 *glow)
{
	mlib_s32 i, j;
	mlib_s64 thresh0 = *((mlib_s64 *) &thresh[0]);
	mlib_s64 low0 = *((mlib_s64 *) &glow[0]);
	mlib_s64 src0, src1, src2, src3, src4, src5, src6, src7;

	src_stride >>= 3;
	dst_stride >>= 3;

	for (i = 0; i < height; i++) {
		const mlib_s64 *psrc_row = (mlib_s64 *)psrc + i * src_stride;
		mlib_s64 *pdst_row = (mlib_s64 *)pdst + i * dst_stride;

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

			pdst_row[j    ] = THRESH0(src0, D64);
			pdst_row[j + 1] = THRESH0(src1, D64);
			pdst_row[j + 2] = THRESH0(src2, D64);
			pdst_row[j + 3] = THRESH0(src3, D64);
			pdst_row[j + 4] = THRESH0(src4, D64);
			pdst_row[j + 5] = THRESH0(src5, D64);
			pdst_row[j + 6] = THRESH0(src6, D64);
			pdst_row[j + 7] = THRESH0(src7, D64);

		}

		for (; j < width; j++) {
			src0 = psrc_row[j];
			pdst_row[j    ] = THRESH0(src0, D64);

		}
	}
}

/* *********************************************************** */

void
mlib_ImageThresh2_D642(
    const mlib_d64 *psrc,
    mlib_s32 src_stride,
    mlib_d64 *pdst,
    mlib_s32 dst_stride,
    mlib_s32 width,
    mlib_s32 height,
    const mlib_d64 *thresh,
    const mlib_d64 *glow)
{
	mlib_s32 i, j;
	mlib_s64 thresh0 = *((mlib_s64 *) &thresh[0]);
	mlib_s64 thresh1 = *((mlib_s64 *) &thresh[1]);
	mlib_s64 low0 = *((mlib_s64 *) &glow[0]);
	mlib_s64 low1 = *((mlib_s64 *) &glow[1]);
	mlib_s64 src0, src1, src2, src3, src4, src5, src6, src7;

	src_stride >>= 3;
	dst_stride >>= 3;
	width <<= 1;

	for (i = 0; i < height; i++) {
		const mlib_s64 *psrc_row = (mlib_s64 *)psrc + i * src_stride;
		mlib_s64 *pdst_row = (mlib_s64 *)pdst + i * dst_stride;

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

			pdst_row[j    ] = THRESH0(src0, D64);
			pdst_row[j + 1] = THRESH1(src1, D64);
			pdst_row[j + 2] = THRESH0(src2, D64);
			pdst_row[j + 3] = THRESH1(src3, D64);
			pdst_row[j + 4] = THRESH0(src4, D64);
			pdst_row[j + 5] = THRESH1(src5, D64);
			pdst_row[j + 6] = THRESH0(src6, D64);
			pdst_row[j + 7] = THRESH1(src7, D64);
		}

		for (; j < width; j += 2) {
			src0 = psrc_row[j];
			src1 = psrc_row[j + 1];
			pdst_row[j    ] = THRESH0(src0, D64);
			pdst_row[j + 1] = THRESH1(src1, D64);
		}
	}
}

/* *********************************************************** */

void
mlib_ImageThresh2_D643(
    const mlib_d64 *psrc,
    mlib_s32 src_stride,
    mlib_d64 *pdst,
    mlib_s32 dst_stride,
    mlib_s32 width,
    mlib_s32 height,
    const mlib_d64 *thresh,
    const mlib_d64 *glow)
{
	mlib_s32 i, j;
	mlib_s64 thresh0 = *((mlib_s64 *) &thresh[0]);
	mlib_s64 thresh1 = *((mlib_s64 *) &thresh[1]);
	mlib_s64 thresh2 = *((mlib_s64 *) &thresh[2]);
	mlib_s64 low0 = *((mlib_s64 *) &glow[0]);
	mlib_s64 low1 = *((mlib_s64 *) &glow[1]);
	mlib_s64 low2 = *((mlib_s64 *) &glow[2]);
	mlib_s64 src0, src1, src2, src3, src4, src5, src6, src7, src8, src9,
	    src10, src11;

	src_stride >>= 3;
	dst_stride >>= 3;
	width = width * 3;

	for (i = 0; i < height; i++) {
		const mlib_s64 *psrc_row = (mlib_s64 *)psrc + i * src_stride;
		mlib_s64 *pdst_row = (mlib_s64 *)pdst + i * dst_stride;

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

			pdst_row[j    ] = THRESH0(src0, D64);
			pdst_row[j + 1] = THRESH1(src1, D64);
			pdst_row[j + 2] = THRESH2(src2, D64);
			pdst_row[j + 3] = THRESH0(src3, D64);
			pdst_row[j + 4] = THRESH1(src4, D64);
			pdst_row[j + 5] = THRESH2(src5, D64);
			pdst_row[j + 6] = THRESH0(src6, D64);
			pdst_row[j + 7] = THRESH1(src7, D64);
			pdst_row[j + 8] = THRESH2(src8, D64);
			pdst_row[j + 9] = THRESH0(src9, D64);
			pdst_row[j + 10] = THRESH1(src10, D64);
			pdst_row[j + 11] = THRESH2(src11, D64);

		}

		for (; j < width; j += 3) {
			src0 = psrc_row[j];
			src1 = psrc_row[j + 1];
			src2 = psrc_row[j + 2];
			pdst_row[j    ] = THRESH0(src0, D64);
			pdst_row[j + 1] = THRESH1(src1, D64);
			pdst_row[j + 2] = THRESH2(src2, D64);

		}
	}
}

/* *********************************************************** */

void
mlib_ImageThresh2_D644(
    const mlib_d64 *psrc,
    mlib_s32 src_stride,
    mlib_d64 *pdst,
    mlib_s32 dst_stride,
    mlib_s32 width,
    mlib_s32 height,
    const mlib_d64 *thresh,
    const mlib_d64 *glow)
{
	mlib_s32 i, j;
	mlib_s64 thresh0 = *((mlib_s64 *) &thresh[0]);
	mlib_s64 thresh1 = *((mlib_s64 *) &thresh[1]);
	mlib_s64 thresh2 = *((mlib_s64 *) &thresh[2]);
	mlib_s64 thresh3 = *((mlib_s64 *) &thresh[3]);
	mlib_s64 low0 = *((mlib_s64 *) &glow[0]);
	mlib_s64 low1 = *((mlib_s64 *) &glow[1]);
	mlib_s64 low2 = *((mlib_s64 *) &glow[2]);
	mlib_s64 low3 = *((mlib_s64 *) &glow[3]);
	mlib_s64 src0, src1, src2, src3, src4, src5, src6, src7;

	src_stride >>= 3;
	dst_stride >>= 3;
	width <<= 2;

	for (i = 0; i < height; i++) {
		const mlib_s64 *psrc_row = (mlib_s64 *)psrc + i * src_stride;
		mlib_s64 *pdst_row = (mlib_s64 *)pdst + i * dst_stride;

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

			pdst_row[j    ] = THRESH0(src0, D64);
			pdst_row[j + 1] = THRESH1(src1, D64);
			pdst_row[j + 2] = THRESH2(src2, D64);
			pdst_row[j + 3] = THRESH3(src3, D64);
			pdst_row[j + 4] = THRESH0(src4, D64);
			pdst_row[j + 5] = THRESH1(src5, D64);
			pdst_row[j + 6] = THRESH2(src6, D64);
			pdst_row[j + 7] = THRESH3(src7, D64);

		}

		for (; j < width; j += 4) {
			src0 = psrc_row[j];
			src1 = psrc_row[j + 1];
			src2 = psrc_row[j + 2];
			src3 = psrc_row[j + 3];

			pdst_row[j    ] = THRESH0(src0, D64);
			pdst_row[j + 1] = THRESH1(src1, D64);
			pdst_row[j + 2] = THRESH2(src2, D64);
			pdst_row[j + 3] = THRESH3(src3, D64);

		}
	}
}

/* *********************************************************** */
#endif
