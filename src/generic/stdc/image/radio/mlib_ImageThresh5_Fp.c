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

#pragma ident	"@(#)mlib_ImageThresh5_Fp.c	9.2	07/10/09 SMI"

/*
 * FUNCTION
 *      mlib_ImageThresh5_Fp - thresholding
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageThresh5_Fp(mlib_image       *dst,
 *                                       const mlib_image *src,
 *                                       const mlib_d64   *thigh,
 *                                       const mlib_d64   *tlow,
 *                                       const mlib_d64   *gmid);
 *
 * ARGUMENT
 *      dst     pointer to output image
 *      src     pointer to input image
 *      thigh   array of high thresholds
 *      tlow    array of low thresholds
 *      gmid    array of values between thresholds
 *
 * RESTRICTION
 *      The images must have the same type, the same size, and the same number
 *      of channels.
 *      The images can have 1, 2, 3, or 4 channels.
 *      The images can be in MLIB_FLOAT or MLIB_DOUBLE data type.
 *
 * DESCRIPTION
 *      If the pixel channel value is below the thigh for that channel
 *      and the pixel channel value is above the tlow for that channel,
 *      set the destination to the gmid value for that channel.
 *
 *                      +- src[x][y][c] src[x][y][c] < tlow[c]
 *                      |
 *      dst[x][y][c]  = +- gmid[c]      tlow[c] <= src[x][y][c] <= thigh[c]
 *                      |
 *                      +- src[x][y][c] src[x][y][c] > thigh[c]
 */

#include <mlib_image.h>
#include <mlib_ImageCheck.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageThresh5_Fp = __mlib_ImageThresh5_Fp

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_ImageThresh5_Fp) mlib_ImageThresh5_Fp
    __attribute__((weak, alias("__mlib_ImageThresh5_Fp")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

static void mlib_ImageThresh5_F321(
    const mlib_f32 *psrc,
    mlib_s32 src_stride,
    mlib_f32 *pdst,
    mlib_s32 dst_stride,
    mlib_s32 width,
    mlib_s32 height,
    const mlib_d64 *thigh,
    const mlib_d64 *tlow,
    const mlib_d64 *gmid);

static void mlib_ImageThresh5_F322(
    const mlib_f32 *psrc,
    mlib_s32 src_stride,
    mlib_f32 *pdst,
    mlib_s32 dst_stride,
    mlib_s32 width,
    mlib_s32 height,
    const mlib_d64 *thigh,
    const mlib_d64 *tlow,
    const mlib_d64 *gmid);

static void mlib_ImageThresh5_F323(
    const mlib_f32 *psrc,
    mlib_s32 src_stride,
    mlib_f32 *pdst,
    mlib_s32 dst_stride,
    mlib_s32 width,
    mlib_s32 height,
    const mlib_d64 *thigh,
    const mlib_d64 *tlow,
    const mlib_d64 *gmid);

static void mlib_ImageThresh5_F324(
    const mlib_f32 *psrc,
    mlib_s32 src_stride,
    mlib_f32 *pdst,
    mlib_s32 dst_stride,
    mlib_s32 width,
    mlib_s32 height,
    const mlib_d64 *thigh,
    const mlib_d64 *tlow,
    const mlib_d64 *gmid);

static void mlib_ImageThresh5_D641(
    const mlib_d64 *psrc,
    mlib_s32 src_stride,
    mlib_d64 *pdst,
    mlib_s32 dst_stride,
    mlib_s32 width,
    mlib_s32 height,
    const mlib_d64 *thigh,
    const mlib_d64 *tlow,
    const mlib_d64 *gmid);

static void mlib_ImageThresh5_D642(
    const mlib_d64 *psrc,
    mlib_s32 src_stride,
    mlib_d64 *pdst,
    mlib_s32 dst_stride,
    mlib_s32 width,
    mlib_s32 height,
    const mlib_d64 *thigh,
    const mlib_d64 *tlow,
    const mlib_d64 *gmid);

static void mlib_ImageThresh5_D643(
    const mlib_d64 *psrc,
    mlib_s32 src_stride,
    mlib_d64 *pdst,
    mlib_s32 dst_stride,
    mlib_s32 width,
    mlib_s32 height,
    const mlib_d64 *thigh,
    const mlib_d64 *tlow,
    const mlib_d64 *gmid);

static void mlib_ImageThresh5_D644(
    const mlib_d64 *psrc,
    mlib_s32 src_stride,
    mlib_d64 *pdst,
    mlib_s32 dst_stride,
    mlib_s32 width,
    mlib_s32 height,
    const mlib_d64 *thigh,
    const mlib_d64 *tlow,
    const mlib_d64 *gmid);

/* *********************************************************** */

#ifndef __INTEL_COMPILER

#define	INIT_THRESH(TTYPE, n)                \
	TTYPE thigh##n = thigh[n];           \
	TTYPE tlow##n = tlow[n];             \
	TTYPE gmid##n = gmid[n]

#else

#define	INIT_THRESH(TTYPE, n)                \
	volatile TTYPE htmp##n = thigh[n];   \
	TTYPE thigh##n = htmp##n;            \
	volatile TTYPE ltmp##n = tlow[n];    \
	TTYPE tlow##n = ltmp##n;             \
	TTYPE gmid##n = gmid[n]

#endif

/* *********************************************************** */

#define	DO_THRESH(s0, th, tl, gm)                               \
	((((s0) < (tl)) || ((s0) > (th))) ? (s0) : (gm))

/* *********************************************************** */

#define	THRESH0(s0)	DO_THRESH(s0, thigh0, tlow0, gmid0)
#define	THRESH1(s0)	DO_THRESH(s0, thigh1, tlow1, gmid1)
#define	THRESH2(s0)	DO_THRESH(s0, thigh2, tlow2, gmid2)
#define	THRESH3(s0)	DO_THRESH(s0, thigh3, tlow3, gmid3)

/* *********************************************************** */

mlib_status
__mlib_ImageThresh5_Fp(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_d64 *thigh,
    const mlib_d64 *tlow,
    const mlib_d64 *gmid)
{
	mlib_s32 sstride, dstride, width, height, nchan;
	mlib_type dtype;
	void *psrc, *pdst;
	mlib_s32 i;

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

	for (i = 0; i < nchan; i++) {
		if (thigh[i] < tlow[i])
			return (MLIB_FAILURE);
	}

	if (dtype == MLIB_FLOAT) {

		switch (nchan) {
		case 1:
			mlib_ImageThresh5_F321(psrc, sstride,
			    pdst, dstride, width, height, thigh, tlow, gmid);
			break;
		case 2:
			mlib_ImageThresh5_F322(psrc, sstride,
			    pdst, dstride, width, height, thigh, tlow, gmid);
			break;
		case 3:
			mlib_ImageThresh5_F323(psrc, sstride,
			    pdst, dstride, width, height, thigh, tlow, gmid);
			break;
		case 4:
			mlib_ImageThresh5_F324(psrc, sstride,
			    pdst, dstride, width, height, thigh, tlow, gmid);
			break;
		}

		return (MLIB_SUCCESS);
	} else if (dtype == MLIB_DOUBLE) {

		switch (nchan) {
		case 1:
			mlib_ImageThresh5_D641(psrc, sstride,
			    pdst, dstride, width, height, thigh, tlow, gmid);
			break;
		case 2:
			mlib_ImageThresh5_D642(psrc, sstride,
			    pdst, dstride, width, height, thigh, tlow, gmid);
			break;
		case 3:
			mlib_ImageThresh5_D643(psrc, sstride,
			    pdst, dstride, width, height, thigh, tlow, gmid);
			break;
		case 4:
			mlib_ImageThresh5_D644(psrc, sstride,
			    pdst, dstride, width, height, thigh, tlow, gmid);
			break;
		}

		return (MLIB_SUCCESS);
	}

	return (MLIB_FAILURE);
}

/* *********************************************************** */

void
mlib_ImageThresh5_F321(
    const mlib_f32 *psrc,
    mlib_s32 src_stride,
    mlib_f32 *pdst,
    mlib_s32 dst_stride,
    mlib_s32 width,
    mlib_s32 height,
    const mlib_d64 *thigh,
    const mlib_d64 *tlow,
    const mlib_d64 *gmid)
{
	mlib_s32 i, j;
	mlib_f32 src0, src1, src2, src3, src4, src5, src6, src7;
	INIT_THRESH(mlib_f32, 0);

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
			pdst_row[j] = THRESH0(src0);
			pdst_row[j + 1] = THRESH0(src1);
			pdst_row[j + 2] = THRESH0(src2);
			pdst_row[j + 3] = THRESH0(src3);
			pdst_row[j + 4] = THRESH0(src4);
			pdst_row[j + 5] = THRESH0(src5);
			pdst_row[j + 6] = THRESH0(src6);
			pdst_row[j + 7] = THRESH0(src7);
		}

		for (; j < width; j++) {
			src0 = psrc_row[j];
			pdst_row[j] = THRESH0(src0);
		}
	}
}

/* *********************************************************** */

void
mlib_ImageThresh5_F322(
    const mlib_f32 *psrc,
    mlib_s32 src_stride,
    mlib_f32 *pdst,
    mlib_s32 dst_stride,
    mlib_s32 width,
    mlib_s32 height,
    const mlib_d64 *thigh,
    const mlib_d64 *tlow,
    const mlib_d64 *gmid)
{
	mlib_s32 i, j;
	mlib_f32 src0, src1, src2, src3, src4, src5, src6, src7;
	INIT_THRESH(mlib_f32, 0);
	INIT_THRESH(mlib_f32, 1);

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
			pdst_row[j] = THRESH0(src0);
			pdst_row[j + 1] = THRESH1(src1);
			pdst_row[j + 2] = THRESH0(src2);
			pdst_row[j + 3] = THRESH1(src3);
			pdst_row[j + 4] = THRESH0(src4);
			pdst_row[j + 5] = THRESH1(src5);
			pdst_row[j + 6] = THRESH0(src6);
			pdst_row[j + 7] = THRESH1(src7);
		}

		for (; j < width; j += 2) {
			src0 = psrc_row[j];
			src1 = psrc_row[j + 1];
			pdst_row[j] = THRESH0(src0);
			pdst_row[j + 1] = THRESH1(src1);
		}
	}
}

/* *********************************************************** */

void
mlib_ImageThresh5_F323(
    const mlib_f32 *psrc,
    mlib_s32 src_stride,
    mlib_f32 *pdst,
    mlib_s32 dst_stride,
    mlib_s32 width,
    mlib_s32 height,
    const mlib_d64 *thigh,
    const mlib_d64 *tlow,
    const mlib_d64 *gmid)
{
	mlib_s32 i, j;
	mlib_f32 src0, src1, src2, src3, src4, src5, src6, src7, src8, src9,
	    src10, src11;
	INIT_THRESH(mlib_f32, 0);
	INIT_THRESH(mlib_f32, 1);
	INIT_THRESH(mlib_f32, 2);

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
			pdst_row[j] = THRESH0(src0);
			pdst_row[j + 1] = THRESH1(src1);
			pdst_row[j + 2] = THRESH2(src2);
			pdst_row[j + 3] = THRESH0(src3);
			pdst_row[j + 4] = THRESH1(src4);
			pdst_row[j + 5] = THRESH2(src5);
			pdst_row[j + 6] = THRESH0(src6);
			pdst_row[j + 7] = THRESH1(src7);
			pdst_row[j + 8] = THRESH2(src8);
			pdst_row[j + 9] = THRESH0(src9);
			pdst_row[j + 10] = THRESH1(src10);
			pdst_row[j + 11] = THRESH2(src11);
		}

		for (; j < width; j += 3) {
			src0 = psrc_row[j];
			src1 = psrc_row[j + 1];
			src2 = psrc_row[j + 2];
			pdst_row[j] = THRESH0(src0);
			pdst_row[j + 1] = THRESH1(src1);
			pdst_row[j + 2] = THRESH2(src2);
		}
	}
}

/* *********************************************************** */

void
mlib_ImageThresh5_F324(
    const mlib_f32 *psrc,
    mlib_s32 src_stride,
    mlib_f32 *pdst,
    mlib_s32 dst_stride,
    mlib_s32 width,
    mlib_s32 height,
    const mlib_d64 *thigh,
    const mlib_d64 *tlow,
    const mlib_d64 *gmid)
{
	mlib_s32 i, j;
	mlib_f32 src0, src1, src2, src3, src4, src5, src6, src7;
	INIT_THRESH(mlib_f32, 0);
	INIT_THRESH(mlib_f32, 1);
	INIT_THRESH(mlib_f32, 2);
	INIT_THRESH(mlib_f32, 3);

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
			pdst_row[j] = THRESH0(src0);
			pdst_row[j + 1] = THRESH1(src1);
			pdst_row[j + 2] = THRESH2(src2);
			pdst_row[j + 3] = THRESH3(src3);
			pdst_row[j + 4] = THRESH0(src4);
			pdst_row[j + 5] = THRESH1(src5);
			pdst_row[j + 6] = THRESH2(src6);
			pdst_row[j + 7] = THRESH3(src7);
		}

		for (; j < width; j += 4) {
			src0 = psrc_row[j];
			src1 = psrc_row[j + 1];
			src2 = psrc_row[j + 2];
			src3 = psrc_row[j + 3];
			pdst_row[j] = THRESH0(src0);
			pdst_row[j + 1] = THRESH1(src1);
			pdst_row[j + 2] = THRESH2(src2);
			pdst_row[j + 3] = THRESH3(src3);
		}
	}
}

/* *********************************************************** */

void
mlib_ImageThresh5_D641(
    const mlib_d64 *psrc,
    mlib_s32 src_stride,
    mlib_d64 *pdst,
    mlib_s32 dst_stride,
    mlib_s32 width,
    mlib_s32 height,
    const mlib_d64 *thigh,
    const mlib_d64 *tlow,
    const mlib_d64 *gmid)
{
	mlib_s32 i, j;
	mlib_d64 src0, src1, src2, src3, src4, src5, src6, src7;
	INIT_THRESH(mlib_d64, 0);

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
			pdst_row[j] = THRESH0(src0);
			pdst_row[j + 1] = THRESH0(src1);
			pdst_row[j + 2] = THRESH0(src2);
			pdst_row[j + 3] = THRESH0(src3);
			pdst_row[j + 4] = THRESH0(src4);
			pdst_row[j + 5] = THRESH0(src5);
			pdst_row[j + 6] = THRESH0(src6);
			pdst_row[j + 7] = THRESH0(src7);
		}

		for (; j < width; j++) {
			src0 = psrc_row[j];
			pdst_row[j] = THRESH0(src0);
		}
	}
}

/* *********************************************************** */

void
mlib_ImageThresh5_D642(
    const mlib_d64 *psrc,
    mlib_s32 src_stride,
    mlib_d64 *pdst,
    mlib_s32 dst_stride,
    mlib_s32 width,
    mlib_s32 height,
    const mlib_d64 *thigh,
    const mlib_d64 *tlow,
    const mlib_d64 *gmid)
{
	mlib_s32 i, j;
	mlib_d64 src0, src1, src2, src3, src4, src5, src6, src7;
	INIT_THRESH(mlib_d64, 0);
	INIT_THRESH(mlib_d64, 1);

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
			pdst_row[j] = THRESH0(src0);
			pdst_row[j + 1] = THRESH1(src1);
			pdst_row[j + 2] = THRESH0(src2);
			pdst_row[j + 3] = THRESH1(src3);
			pdst_row[j + 4] = THRESH0(src4);
			pdst_row[j + 5] = THRESH1(src5);
			pdst_row[j + 6] = THRESH0(src6);
			pdst_row[j + 7] = THRESH1(src7);
		}

		for (; j < width; j += 2) {
			src0 = psrc_row[j];
			src1 = psrc_row[j + 1];
			pdst_row[j] = THRESH0(src0);
			pdst_row[j + 1] = THRESH1(src1);
		}
	}
}

/* *********************************************************** */

void
mlib_ImageThresh5_D643(
    const mlib_d64 *psrc,
    mlib_s32 src_stride,
    mlib_d64 *pdst,
    mlib_s32 dst_stride,
    mlib_s32 width,
    mlib_s32 height,
    const mlib_d64 *thigh,
    const mlib_d64 *tlow,
    const mlib_d64 *gmid)
{
	mlib_s32 i, j;
	mlib_d64 src0, src1, src2, src3, src4, src5, src6, src7, src8, src9,
	    src10, src11;
	INIT_THRESH(mlib_d64, 0);
	INIT_THRESH(mlib_d64, 1);
	INIT_THRESH(mlib_d64, 2);

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
			pdst_row[j] = THRESH0(src0);
			pdst_row[j + 1] = THRESH1(src1);
			pdst_row[j + 2] = THRESH2(src2);
			pdst_row[j + 3] = THRESH0(src3);
			pdst_row[j + 4] = THRESH1(src4);
			pdst_row[j + 5] = THRESH2(src5);
			pdst_row[j + 6] = THRESH0(src6);
			pdst_row[j + 7] = THRESH1(src7);
			pdst_row[j + 8] = THRESH2(src8);
			pdst_row[j + 9] = THRESH0(src9);
			pdst_row[j + 10] = THRESH1(src10);
			pdst_row[j + 11] = THRESH2(src11);
		}

		for (; j < width; j += 3) {
			src0 = psrc_row[j];
			src1 = psrc_row[j + 1];
			src2 = psrc_row[j + 2];
			pdst_row[j] = THRESH0(src0);
			pdst_row[j + 1] = THRESH1(src1);
			pdst_row[j + 2] = THRESH2(src2);
		}
	}
}

/* *********************************************************** */

void
mlib_ImageThresh5_D644(
    const mlib_d64 *psrc,
    mlib_s32 src_stride,
    mlib_d64 *pdst,
    mlib_s32 dst_stride,
    mlib_s32 width,
    mlib_s32 height,
    const mlib_d64 *thigh,
    const mlib_d64 *tlow,
    const mlib_d64 *gmid)
{
	mlib_s32 i, j;
	mlib_d64 src0, src1, src2, src3, src4, src5, src6, src7;
	INIT_THRESH(mlib_d64, 0);
	INIT_THRESH(mlib_d64, 1);
	INIT_THRESH(mlib_d64, 2);
	INIT_THRESH(mlib_d64, 3);

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
			pdst_row[j] = THRESH0(src0);
			pdst_row[j + 1] = THRESH1(src1);
			pdst_row[j + 2] = THRESH2(src2);
			pdst_row[j + 3] = THRESH3(src3);
			pdst_row[j + 4] = THRESH0(src4);
			pdst_row[j + 5] = THRESH1(src5);
			pdst_row[j + 6] = THRESH2(src6);
			pdst_row[j + 7] = THRESH3(src7);
		}

		for (; j < width; j += 4) {
			src0 = psrc_row[j];
			src1 = psrc_row[j + 1];
			src2 = psrc_row[j + 2];
			src3 = psrc_row[j + 3];
			pdst_row[j] = THRESH0(src0);
			pdst_row[j + 1] = THRESH1(src1);
			pdst_row[j + 2] = THRESH2(src2);
			pdst_row[j + 3] = THRESH3(src3);
		}
	}
}

/* *********************************************************** */
