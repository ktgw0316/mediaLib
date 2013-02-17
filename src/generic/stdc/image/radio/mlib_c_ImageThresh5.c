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

#pragma ident	"@(#)mlib_c_ImageThresh5.c	9.2	07/10/09 SMI"

/*
 * FUNCTION
 *      mlib_ImageThresh5 - thresholding
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageThresh5(mlib_image       *dst,
 *                                    const mlib_image *src,
 *                                    const mlib_s32   *thigh,
 *                                    const mlib_s32   *tlow,
 *                                    const mlib_s32   *gmid);
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
 *      The images can be in MLIB_BYTE,
 *      MLIB_SHORT, MLIB_USHORT or MLIB_INT data type.
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

#pragma weak mlib_ImageThresh5 = __mlib_ImageThresh5

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_ImageThresh5) mlib_ImageThresh5
    __attribute__((weak, alias("__mlib_ImageThresh5")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

static void mlib_c_ImageThresh5_U81(
    const mlib_u8 *psrc,
    mlib_s32 src_stride,
    mlib_u8 *pdst,
    mlib_s32 dst_stride,
    mlib_s32 width,
    mlib_s32 height,
    const mlib_s32 *thigh,
    const mlib_s32 *tlow,
    const mlib_s32 *gmid);

static void mlib_c_ImageThresh5_U82(
    const mlib_u8 *psrc,
    mlib_s32 src_stride,
    mlib_u8 *pdst,
    mlib_s32 dst_stride,
    mlib_s32 width,
    mlib_s32 height,
    const mlib_s32 *thigh,
    const mlib_s32 *tlow,
    const mlib_s32 *gmid);

static void mlib_c_ImageThresh5_U83(
    const mlib_u8 *psrc,
    mlib_s32 src_stride,
    mlib_u8 *pdst,
    mlib_s32 dst_stride,
    mlib_s32 width,
    mlib_s32 height,
    const mlib_s32 *thigh,
    const mlib_s32 *tlow,
    const mlib_s32 *gmid);

static void mlib_c_ImageThresh5_U84(
    const mlib_u8 *psrc,
    mlib_s32 src_stride,
    mlib_u8 *pdst,
    mlib_s32 dst_stride,
    mlib_s32 width,
    mlib_s32 height,
    const mlib_s32 *thigh,
    const mlib_s32 *tlow,
    const mlib_s32 *gmid);

static void mlib_c_ImageThresh5_S161(
    const mlib_s16 *psrc,
    mlib_s32 src_stride,
    mlib_s16 *pdst,
    mlib_s32 dst_stride,
    mlib_s32 width,
    mlib_s32 height,
    const mlib_s32 *thigh,
    const mlib_s32 *tlow,
    const mlib_s32 *gmid);

static void mlib_c_ImageThresh5_S162(
    const mlib_s16 *psrc,
    mlib_s32 src_stride,
    mlib_s16 *pdst,
    mlib_s32 dst_stride,
    mlib_s32 width,
    mlib_s32 height,
    const mlib_s32 *thigh,
    const mlib_s32 *tlow,
    const mlib_s32 *gmid);

static void mlib_c_ImageThresh5_S163(
    const mlib_s16 *psrc,
    mlib_s32 src_stride,
    mlib_s16 *pdst,
    mlib_s32 dst_stride,
    mlib_s32 width,
    mlib_s32 height,
    const mlib_s32 *thigh,
    const mlib_s32 *tlow,
    const mlib_s32 *gmid);

static void mlib_c_ImageThresh5_S164(
    const mlib_s16 *psrc,
    mlib_s32 src_stride,
    mlib_s16 *pdst,
    mlib_s32 dst_stride,
    mlib_s32 width,
    mlib_s32 height,
    const mlib_s32 *thigh,
    const mlib_s32 *tlow,
    const mlib_s32 *gmid);

static void mlib_c_ImageThresh5_U161(
    const mlib_u16 *psrc,
    mlib_s32 src_stride,
    mlib_u16 *pdst,
    mlib_s32 dst_stride,
    mlib_s32 width,
    mlib_s32 height,
    const mlib_s32 *thigh,
    const mlib_s32 *tlow,
    const mlib_s32 *gmid);

static void mlib_c_ImageThresh5_U162(
    const mlib_u16 *psrc,
    mlib_s32 src_stride,
    mlib_u16 *pdst,
    mlib_s32 dst_stride,
    mlib_s32 width,
    mlib_s32 height,
    const mlib_s32 *thigh,
    const mlib_s32 *tlow,
    const mlib_s32 *gmid);

static void mlib_c_ImageThresh5_U163(
    const mlib_u16 *psrc,
    mlib_s32 src_stride,
    mlib_u16 *pdst,
    mlib_s32 dst_stride,
    mlib_s32 width,
    mlib_s32 height,
    const mlib_s32 *thigh,
    const mlib_s32 *tlow,
    const mlib_s32 *gmid);

static void mlib_c_ImageThresh5_U164(
    const mlib_u16 *psrc,
    mlib_s32 src_stride,
    mlib_u16 *pdst,
    mlib_s32 dst_stride,
    mlib_s32 width,
    mlib_s32 height,
    const mlib_s32 *thigh,
    const mlib_s32 *tlow,
    const mlib_s32 *gmid);

static void mlib_c_ImageThresh5_S321(
    const mlib_s32 *psrc,
    mlib_s32 src_stride,
    mlib_s32 *pdst,
    mlib_s32 dst_stride,
    mlib_s32 width,
    mlib_s32 height,
    const mlib_s32 *thigh,
    const mlib_s32 *tlow,
    const mlib_s32 *gmid);

static void mlib_c_ImageThresh5_S322(
    const mlib_s32 *psrc,
    mlib_s32 src_stride,
    mlib_s32 *pdst,
    mlib_s32 dst_stride,
    mlib_s32 width,
    mlib_s32 height,
    const mlib_s32 *thigh,
    const mlib_s32 *tlow,
    const mlib_s32 *gmid);

static void mlib_c_ImageThresh5_S323(
    const mlib_s32 *psrc,
    mlib_s32 src_stride,
    mlib_s32 *pdst,
    mlib_s32 dst_stride,
    mlib_s32 width,
    mlib_s32 height,
    const mlib_s32 *thigh,
    const mlib_s32 *tlow,
    const mlib_s32 *gmid);

static void mlib_c_ImageThresh5_S324(
    const mlib_s32 *psrc,
    mlib_s32 src_stride,
    mlib_s32 *pdst,
    mlib_s32 dst_stride,
    mlib_s32 width,
    mlib_s32 height,
    const mlib_s32 *thigh,
    const mlib_s32 *tlow,
    const mlib_s32 *gmid);

/* *********************************************************** */

#define	INIT_THRESH0(n)                                         \
	thigh0 = thigh[(n)];                                    \
	tlow0 = tlow[(n)];                                      \
	gmid0 = gmid[(n)]

/* *********************************************************** */

#define	INIT_THRESH1(n)                                         \
	thigh1 = thigh[(n)];                                    \
	tlow1 = tlow[(n)];                                      \
	gmid1 = gmid[(n)]

/* *********************************************************** */

#define	INIT_THRESH2(n)                                         \
	thigh2 = thigh[(n)];                                    \
	tlow2 = tlow[(n)];                                      \
	gmid2 = gmid[(n)]

/* *********************************************************** */

#define	INIT_THRESH3(n)                                         \
	thigh3 = thigh[(n)];                                    \
	tlow3 = tlow[(n)];                                      \
	gmid3 = gmid[(n)]

/* *********************************************************** */

#define	DO_THRESH(s0, th, tl, gm)                                  \
	((s0 & (((s0 - tl) | (th - s0)) >> 31)) | (gm & ~(((s0 -   \
	    tl) | (th - s0)) >> 31)))

/* *********************************************************** */

#define	THRESH0(s0)	DO_THRESH(s0, thigh0, tlow0, gmid0)
#define	THRESH1(s0)	DO_THRESH(s0, thigh1, tlow1, gmid1)

#define	THRESH2(s0)	DO_THRESH(s0, thigh2, tlow2, gmid2)

#define	THRESH3(s0)	DO_THRESH(s0, thigh3, tlow3, gmid3)

/* *********************************************************** */

#define	THRESH_INDEX(s0, index)                                   \
	(DO_THRESH(s0, thigh[index], tlow[index], gmid[index]))

/* *********************************************************** */

#define	STRIP(pd, ps, w, h, ch, th, tl, gm, data_type)          \
	{                                                       \
	    data_type s0;                                       \
	                                                        \
	    for (i = 0; i < h; i++)                             \
		for (j = 0; j < w; j++)                         \
		    for (k = 0; k < ch; k++) {                  \
			s0 = ps[i * src_stride + j * ch + k];   \
			pd[i * dst_stride + j * ch + k] =       \
			    (s0 < tl[k]) ? s0 : ((s0 <=         \
			    th[k]) ? gm[k] : s0);               \
		    }                                           \
	}

/* *********************************************************** */

mlib_status
__mlib_ImageThresh5(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_s32 *thigh,
    const mlib_s32 *tlow,
    const mlib_s32 *gmid)
{
	mlib_s32 sstride, dstride, width, height, nchan;
	mlib_type dtype;
	void *psrc, *pdst;
	mlib_s32 th[4], tl[4], gm[4];
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

	if (dtype == MLIB_BYTE) {

		for (i = 0; i < nchan; i++) {
			gm[i] = gmid[i] & 255;
			th[i] = thigh[i];
			tl[i] = tlow[i];

			if ((th[i] < 0) || (tl[i] > MLIB_U8_MAX))
				gm[i] = th[i] = tl[i] = 0;
			else {

				if (th[i] > MLIB_U8_MAX)
					th[i] = MLIB_U8_MAX;

				if (tl[i] < 0)
					tl[i] = 0;
			}
		}

/* x * 4.0 < x * 2.2 + 256 */
		if ((height * width) < 160) {
			switch (nchan) {
			case 1:
				mlib_c_ImageThresh5_U81(psrc, sstride,
				    pdst, dstride, width, height, th, tl, gm);
				break;
			case 2:
				mlib_c_ImageThresh5_U82(psrc, sstride,
				    pdst, dstride, width, height, th, tl, gm);
				break;
			case 3:
				mlib_c_ImageThresh5_U83(psrc, sstride,
				    pdst, dstride, width, height, th, tl, gm);
				break;
			case 4:
				mlib_c_ImageThresh5_U84(psrc, sstride,
				    pdst, dstride, width, height, th, tl, gm);
				break;
			}
		} else {
			mlib_s32 c, t;
			mlib_d64 lookup[128];
			mlib_u8 *arr[4], *p;
			mlib_u8 gu;

			arr[0] = (mlib_u8 *)(&(lookup[0]));
			arr[1] = (mlib_u8 *)(&(lookup[32]));
			arr[2] = (mlib_u8 *)(&(lookup[64]));
			arr[3] = (mlib_u8 *)(&(lookup[96]));
			for (c = 0; c < nchan; c++) {
				p = arr[c];
				t = tl[c];
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i < t; i++) {
					p[i] = i;
				}

				t = th[c];
				gu = gm[c];
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (; i <= t; i++) {
					p[i] = gu;
				}

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (; i < 256; i++) {
					p[i] = i;
				}
			}

			return (__mlib_ImageLookUp
					(dst, src, (const void **)arr));
		}

		return (MLIB_SUCCESS);
	} else if (dtype == MLIB_SHORT) {

		for (i = 0; i < nchan; i++) {
			gm[i] = (gmid[i] << 16) >> 16;
			th[i] = thigh[i];
			tl[i] = tlow[i];

			if ((tl[i] > MLIB_S16_MAX) || (th[i] < MLIB_S16_MIN))
				gm[i] = th[i] = tl[i] = 0;
			else {

				if (th[i] > MLIB_S16_MAX)
					th[i] = MLIB_S16_MAX;

				if (tl[i] < MLIB_S16_MIN)
					tl[i] = MLIB_S16_MIN;
			}
		}

		switch (nchan) {
		case 1:
			mlib_c_ImageThresh5_S161(psrc, sstride, pdst, dstride,
			    width, height, th, tl, gm);
			break;
		case 2:
			mlib_c_ImageThresh5_S162(psrc, sstride, pdst, dstride,
			    width, height, th, tl, gm);
			break;
		case 3:
			mlib_c_ImageThresh5_S163(psrc, sstride, pdst, dstride,
			    width, height, th, tl, gm);
			break;
		case 4:
			mlib_c_ImageThresh5_S164(psrc, sstride, pdst, dstride,
			    width, height, th, tl, gm);
			break;
		}

		return (MLIB_SUCCESS);
	} else if (dtype == MLIB_USHORT) {

		for (i = 0; i < nchan; i++) {
			gm[i] = (gmid[i] << 16) >> 16;
			th[i] = thigh[i];
			tl[i] = tlow[i];

			if ((tl[i] > MLIB_U16_MAX) || (th[i] < MLIB_U16_MIN))
				gm[i] = th[i] = tl[i] = 0;
			else {

				if (th[i] > MLIB_U16_MAX)
					th[i] = MLIB_U16_MAX;

				if (tl[i] < MLIB_U16_MIN)
					tl[i] = MLIB_U16_MIN;
			}
		}

		switch (nchan) {
		case 1:
			mlib_c_ImageThresh5_U161(psrc, sstride, pdst, dstride,
			    width, height, th, tl, gm);
			break;
		case 2:
			mlib_c_ImageThresh5_U162(psrc, sstride, pdst, dstride,
			    width, height, th, tl, gm);
			break;
		case 3:
			mlib_c_ImageThresh5_U163(psrc, sstride, pdst, dstride,
			    width, height, th, tl, gm);
			break;
		case 4:
			mlib_c_ImageThresh5_U164(psrc, sstride, pdst, dstride,
			    width, height, th, tl, gm);
			break;
		}

		return (MLIB_SUCCESS);
	} else if (dtype == MLIB_INT) {

		switch (nchan) {
		case 1:
			mlib_c_ImageThresh5_S321(psrc, sstride,
			    pdst, dstride, width, height, thigh, tlow, gmid);
			break;
		case 2:
			mlib_c_ImageThresh5_S322(psrc, sstride,
			    pdst, dstride, width, height, thigh, tlow, gmid);
			break;
		case 3:
			mlib_c_ImageThresh5_S323(psrc, sstride,
			    pdst, dstride, width, height, thigh, tlow, gmid);
			break;
		case 4:
			mlib_c_ImageThresh5_S324(psrc, sstride,
			    pdst, dstride, width, height, thigh, tlow, gmid);
			break;
		}

		return (MLIB_SUCCESS);
	}

	return (MLIB_FAILURE);
}

/* *********************************************************** */

void
mlib_c_ImageThresh5_U81(
    const mlib_u8 *psrc,
    mlib_s32 src_stride,
    mlib_u8 *pdst,
    mlib_s32 dst_stride,
    mlib_s32 width,
    mlib_s32 height,
    const mlib_s32 *thigh,
    const mlib_s32 *tlow,
    const mlib_s32 *gmid)
{
	mlib_s32 i, j, k;
	mlib_s32 thigh0, tlow0;
	mlib_s32 gmid0;

	if (width < 16) {
		STRIP(pdst, psrc, width, height, 1, thigh, tlow, gmid, mlib_u8);

		return;
	}

	INIT_THRESH0(0);
	for (i = 0; i < height; i++) {
		const mlib_u8 *psrc_row = psrc + i * src_stride;
		mlib_u8 *pdst_row = pdst + i * dst_stride;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (j = 0; j <= (width - 8); j += 8) {
			pdst_row[j] = THRESH0(psrc_row[j]);
			pdst_row[j + 1] = THRESH0(psrc_row[j + 1]);
			pdst_row[j + 2] = THRESH0(psrc_row[j + 2]);
			pdst_row[j + 3] = THRESH0(psrc_row[j + 3]);
			pdst_row[j + 4] = THRESH0(psrc_row[j + 4]);
			pdst_row[j + 5] = THRESH0(psrc_row[j + 5]);
			pdst_row[j + 6] = THRESH0(psrc_row[j + 6]);
			pdst_row[j + 7] = THRESH0(psrc_row[j + 7]);
		}

		for (; j < width; j++) {
			pdst_row[j] = THRESH0(psrc_row[j]);
		}
	}
}

/* *********************************************************** */

void
mlib_c_ImageThresh5_U82(
    const mlib_u8 *psrc,
    mlib_s32 src_stride,
    mlib_u8 *pdst,
    mlib_s32 dst_stride,
    mlib_s32 width,
    mlib_s32 height,
    const mlib_s32 *thigh,
    const mlib_s32 *tlow,
    const mlib_s32 *gmid)
{
	mlib_s32 i, j, k;
	mlib_s32 thigh0, thigh1;
	mlib_s32 tlow0, tlow1;
	mlib_s32 gmid0, gmid1;

	if (width < 16) {
		STRIP(pdst, psrc, width, height, 2, thigh, tlow, gmid, mlib_u8);

		return;
	}

	INIT_THRESH0(0);
	INIT_THRESH1(1);
	width <<= 1;
	for (i = 0; i < height; i++) {
		const mlib_u8 *psrc_row = psrc + i * src_stride;
		mlib_u8 *pdst_row = pdst + i * dst_stride;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (j = 0; j <= (width - 8); j += 8) {
			pdst_row[j] = THRESH0(psrc_row[j]);
			pdst_row[j + 1] = THRESH1(psrc_row[j + 1]);
			pdst_row[j + 2] = THRESH0(psrc_row[j + 2]);
			pdst_row[j + 3] = THRESH1(psrc_row[j + 3]);
			pdst_row[j + 4] = THRESH0(psrc_row[j + 4]);
			pdst_row[j + 5] = THRESH1(psrc_row[j + 5]);
			pdst_row[j + 6] = THRESH0(psrc_row[j + 6]);
			pdst_row[j + 7] = THRESH1(psrc_row[j + 7]);
		}

		for (; j < width; j += 2) {
			pdst_row[j] = THRESH0(psrc_row[j]);
			pdst_row[j + 1] = THRESH1(psrc_row[j + 1]);
		}
	}
}

/* *********************************************************** */

void
mlib_c_ImageThresh5_U83(
    const mlib_u8 *psrc,
    mlib_s32 src_stride,
    mlib_u8 *pdst,
    mlib_s32 dst_stride,
    mlib_s32 width,
    mlib_s32 height,
    const mlib_s32 *thigh,
    const mlib_s32 *tlow,
    const mlib_s32 *gmid)
{
	mlib_s32 i, j, k;
	mlib_s32 thigh0, thigh1, thigh2;
	mlib_s32 tlow0, tlow1, tlow2;
	mlib_s32 gmid0, gmid1, gmid2;

	if (width < 16) {
		STRIP(pdst, psrc, width, height, 3, thigh, tlow, gmid, mlib_u8);

		return;
	}

	width = width * 3;
	INIT_THRESH0(0);
	INIT_THRESH1(1);
	INIT_THRESH2(2);
	for (i = 0; i < height; i++) {
		const mlib_u8 *psrc_row = psrc + i * src_stride;
		mlib_u8 *pdst_row = pdst + i * dst_stride;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (j = 0; j <= (width - 12); j += 12) {
			pdst_row[j] = THRESH0(psrc_row[j]);
			pdst_row[j + 1] = THRESH1(psrc_row[j + 1]);
			pdst_row[j + 2] = THRESH2(psrc_row[j + 2]);
			pdst_row[j + 3] = THRESH0(psrc_row[j + 3]);
			pdst_row[j + 4] = THRESH1(psrc_row[j + 4]);
			pdst_row[j + 5] = THRESH2(psrc_row[j + 5]);
			pdst_row[j + 6] = THRESH0(psrc_row[j + 6]);
			pdst_row[j + 7] = THRESH1(psrc_row[j + 7]);
			pdst_row[j + 8] = THRESH2(psrc_row[j + 8]);
			pdst_row[j + 9] = THRESH0(psrc_row[j + 9]);
			pdst_row[j + 10] = THRESH1(psrc_row[j + 10]);
			pdst_row[j + 11] = THRESH2(psrc_row[j + 11]);
		}

		for (; j < width; j += 3) {
			pdst_row[j] = THRESH0(psrc_row[j]);
			pdst_row[j + 1] = THRESH1(psrc_row[j + 1]);
			pdst_row[j + 2] = THRESH2(psrc_row[j + 2]);
		}
	}
}

/* *********************************************************** */

void
mlib_c_ImageThresh5_U84(
    const mlib_u8 *psrc,
    mlib_s32 src_stride,
    mlib_u8 *pdst,
    mlib_s32 dst_stride,
    mlib_s32 width,
    mlib_s32 height,
    const mlib_s32 *thigh,
    const mlib_s32 *tlow,
    const mlib_s32 *gmid)
{
	mlib_s32 i, j, k;
	mlib_s32 thigh0, thigh1, thigh2, thigh3;
	mlib_s32 tlow0, tlow1, tlow2, tlow3;
	mlib_s32 gmid0, gmid1, gmid2, gmid3;

	if (width < 16) {
		STRIP(pdst, psrc, width, height, 4, thigh, tlow, gmid, mlib_u8);

		return;
	}

	INIT_THRESH0(0);
	INIT_THRESH1(1);
	INIT_THRESH2(2);
	INIT_THRESH3(3);

	width <<= 2;
	for (i = 0; i < height; i++) {
		const mlib_u8 *psrc_row = psrc + i * src_stride;
		mlib_u8 *pdst_row = pdst + i * dst_stride;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (j = 0; j <= (width - 8); j += 8) {
			pdst_row[j] = THRESH0(psrc_row[j]);
			pdst_row[j + 1] = THRESH1(psrc_row[j + 1]);
			pdst_row[j + 2] = THRESH2(psrc_row[j + 2]);
			pdst_row[j + 3] = THRESH3(psrc_row[j + 3]);
			pdst_row[j + 4] = THRESH0(psrc_row[j + 4]);
			pdst_row[j + 5] = THRESH1(psrc_row[j + 5]);
			pdst_row[j + 6] = THRESH2(psrc_row[j + 6]);
			pdst_row[j + 7] = THRESH3(psrc_row[j + 7]);
		}

		if (j < width) {
			pdst_row[j] = THRESH0(psrc_row[j]);
			pdst_row[j + 1] = THRESH1(psrc_row[j + 1]);
			pdst_row[j + 2] = THRESH2(psrc_row[j + 2]);
			pdst_row[j + 3] = THRESH3(psrc_row[j + 3]);
		}
	}
}

/* *********************************************************** */

void
mlib_c_ImageThresh5_S161(
    const mlib_s16 *psrc,
    mlib_s32 src_stride,
    mlib_s16 *pdst,
    mlib_s32 dst_stride,
    mlib_s32 width,
    mlib_s32 height,
    const mlib_s32 *thigh,
    const mlib_s32 *tlow,
    const mlib_s32 *gmid)
{
	mlib_s32 i, j;
	mlib_s32 thigh0, tlow0;
	mlib_s32 gmid0;

	INIT_THRESH0(0);
	src_stride >>= 1;
	dst_stride >>= 1;

	for (i = 0; i < height; i++) {
		const mlib_s16 *psrc_row = psrc + i * src_stride;
		mlib_s16 *pdst_row = pdst + i * dst_stride;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (j = 0; j <= (width - 8); j += 8) {
			pdst_row[j] = THRESH0(psrc_row[j]);
			pdst_row[j + 1] = THRESH0(psrc_row[j + 1]);
			pdst_row[j + 2] = THRESH0(psrc_row[j + 2]);
			pdst_row[j + 3] = THRESH0(psrc_row[j + 3]);
			pdst_row[j + 4] = THRESH0(psrc_row[j + 4]);
			pdst_row[j + 5] = THRESH0(psrc_row[j + 5]);
			pdst_row[j + 6] = THRESH0(psrc_row[j + 6]);
			pdst_row[j + 7] = THRESH0(psrc_row[j + 7]);
		}

		for (; j < width; j++) {
			pdst_row[j] = THRESH0(psrc_row[j]);
		}
	}
}

/* *********************************************************** */

void
mlib_c_ImageThresh5_S162(
    const mlib_s16 *psrc,
    mlib_s32 src_stride,
    mlib_s16 *pdst,
    mlib_s32 dst_stride,
    mlib_s32 width,
    mlib_s32 height,
    const mlib_s32 *thigh,
    const mlib_s32 *tlow,
    const mlib_s32 *gmid)
{
	mlib_s32 i, j;
	mlib_s32 thigh0, thigh1;
	mlib_s32 tlow0, tlow1;
	mlib_s32 gmid0, gmid1;

	INIT_THRESH0(0);
	INIT_THRESH1(1);
	src_stride >>= 1;
	dst_stride >>= 1;
	width <<= 1;

	for (i = 0; i < height; i++) {
		const mlib_s16 *psrc_row = psrc + i * src_stride;
		mlib_s16 *pdst_row = pdst + i * dst_stride;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (j = 0; j <= (width - 8); j += 8) {
			pdst_row[j] = THRESH0(psrc_row[j]);
			pdst_row[j + 1] = THRESH1(psrc_row[j + 1]);
			pdst_row[j + 2] = THRESH0(psrc_row[j + 2]);
			pdst_row[j + 3] = THRESH1(psrc_row[j + 3]);
			pdst_row[j + 4] = THRESH0(psrc_row[j + 4]);
			pdst_row[j + 5] = THRESH1(psrc_row[j + 5]);
			pdst_row[j + 6] = THRESH0(psrc_row[j + 6]);
			pdst_row[j + 7] = THRESH1(psrc_row[j + 7]);
		}

		for (; j < width; j += 2) {
			pdst_row[j] = THRESH0(psrc_row[j]);
			pdst_row[j + 1] = THRESH1(psrc_row[j + 1]);
		}
	}
}

/* *********************************************************** */

void
mlib_c_ImageThresh5_S163(
    const mlib_s16 *psrc,
    mlib_s32 src_stride,
    mlib_s16 *pdst,
    mlib_s32 dst_stride,
    mlib_s32 width,
    mlib_s32 height,
    const mlib_s32 *thigh,
    const mlib_s32 *tlow,
    const mlib_s32 *gmid)
{
	mlib_s32 i, j;
	mlib_s32 thigh0, thigh1, thigh2;
	mlib_s32 tlow0, tlow1, tlow2;
	mlib_s32 gmid0, gmid1, gmid2;

	INIT_THRESH0(0);
	INIT_THRESH1(1);
	INIT_THRESH2(2);
	width = width * 3;
	src_stride >>= 1;
	dst_stride >>= 1;
	for (i = 0; i < height; i++) {
		const mlib_s16 *psrc_row = psrc + i * src_stride;
		mlib_s16 *pdst_row = pdst + i * dst_stride;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (j = 0; j <= (width - 12); j += 12) {
			pdst_row[j] = THRESH0(psrc_row[j]);
			pdst_row[j + 1] = THRESH1(psrc_row[j + 1]);
			pdst_row[j + 2] = THRESH2(psrc_row[j + 2]);
			pdst_row[j + 3] = THRESH0(psrc_row[j + 3]);
			pdst_row[j + 4] = THRESH1(psrc_row[j + 4]);
			pdst_row[j + 5] = THRESH2(psrc_row[j + 5]);
			pdst_row[j + 6] = THRESH0(psrc_row[j + 6]);
			pdst_row[j + 7] = THRESH1(psrc_row[j + 7]);
			pdst_row[j + 8] = THRESH2(psrc_row[j + 8]);
			pdst_row[j + 9] = THRESH0(psrc_row[j + 9]);
			pdst_row[j + 10] = THRESH1(psrc_row[j + 10]);
			pdst_row[j + 11] = THRESH2(psrc_row[j + 11]);
		}

		for (; j < width; j += 3) {
			pdst_row[j] = THRESH0(psrc_row[j]);
			pdst_row[j + 1] = THRESH1(psrc_row[j + 1]);
			pdst_row[j + 2] = THRESH2(psrc_row[j + 2]);
		}
	}
}

/* *********************************************************** */

void
mlib_c_ImageThresh5_S164(
    const mlib_s16 *psrc,
    mlib_s32 src_stride,
    mlib_s16 *pdst,
    mlib_s32 dst_stride,
    mlib_s32 width,
    mlib_s32 height,
    const mlib_s32 *thigh,
    const mlib_s32 *tlow,
    const mlib_s32 *gmid)
{
	mlib_s32 i, j;
	mlib_s32 thigh0, thigh1, thigh2, thigh3;
	mlib_s32 tlow0, tlow1, tlow2, tlow3;
	mlib_s32 gmid0, gmid1, gmid2, gmid3;

	INIT_THRESH0(0);
	INIT_THRESH1(1);
	INIT_THRESH2(2);
	INIT_THRESH3(3);
	width <<= 2;
	src_stride >>= 1;
	dst_stride >>= 1;
	for (i = 0; i < height; i++) {
		const mlib_s16 *psrc_row = psrc + i * src_stride;
		mlib_s16 *pdst_row = pdst + i * dst_stride;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (j = 0; j <= (width - 8); j += 8) {
			pdst_row[j] = THRESH0(psrc_row[j]);
			pdst_row[j + 1] = THRESH1(psrc_row[j + 1]);
			pdst_row[j + 2] = THRESH2(psrc_row[j + 2]);
			pdst_row[j + 3] = THRESH3(psrc_row[j + 3]);
			pdst_row[j + 4] = THRESH0(psrc_row[j + 4]);
			pdst_row[j + 5] = THRESH1(psrc_row[j + 5]);
			pdst_row[j + 6] = THRESH2(psrc_row[j + 6]);
			pdst_row[j + 7] = THRESH3(psrc_row[j + 7]);
		}

		if (j < width) {
			pdst_row[j] = THRESH0(psrc_row[j]);
			pdst_row[j + 1] = THRESH1(psrc_row[j + 1]);
			pdst_row[j + 2] = THRESH2(psrc_row[j + 2]);
			pdst_row[j + 3] = THRESH3(psrc_row[j + 3]);
		}
	}
}

/* *********************************************************** */

void
mlib_c_ImageThresh5_U161(
    const mlib_u16 *psrc,
    mlib_s32 src_stride,
    mlib_u16 *pdst,
    mlib_s32 dst_stride,
    mlib_s32 width,
    mlib_s32 height,
    const mlib_s32 *thigh,
    const mlib_s32 *tlow,
    const mlib_s32 *gmid)
{
	mlib_s32 i, j;
	mlib_s32 thigh0, tlow0;
	mlib_s32 gmid0;

	INIT_THRESH0(0);
	src_stride >>= 1;
	dst_stride >>= 1;

	for (i = 0; i < height; i++) {
		const mlib_u16 *psrc_row = psrc + i * src_stride;
		mlib_u16 *pdst_row = pdst + i * dst_stride;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (j = 0; j <= (width - 8); j += 8) {
			pdst_row[j] = THRESH0(psrc_row[j]);
			pdst_row[j + 1] = THRESH0(psrc_row[j + 1]);
			pdst_row[j + 2] = THRESH0(psrc_row[j + 2]);
			pdst_row[j + 3] = THRESH0(psrc_row[j + 3]);
			pdst_row[j + 4] = THRESH0(psrc_row[j + 4]);
			pdst_row[j + 5] = THRESH0(psrc_row[j + 5]);
			pdst_row[j + 6] = THRESH0(psrc_row[j + 6]);
			pdst_row[j + 7] = THRESH0(psrc_row[j + 7]);
		}

		for (; j < width; j++) {
			pdst_row[j] = THRESH0(psrc_row[j]);
		}
	}
}

/* *********************************************************** */

void
mlib_c_ImageThresh5_U162(
    const mlib_u16 *psrc,
    mlib_s32 src_stride,
    mlib_u16 *pdst,
    mlib_s32 dst_stride,
    mlib_s32 width,
    mlib_s32 height,
    const mlib_s32 *thigh,
    const mlib_s32 *tlow,
    const mlib_s32 *gmid)
{
	mlib_s32 i, j;
	mlib_s32 thigh0, thigh1;
	mlib_s32 tlow0, tlow1;
	mlib_s32 gmid0, gmid1;

	INIT_THRESH0(0);
	INIT_THRESH1(1);
	src_stride >>= 1;
	dst_stride >>= 1;
	width <<= 1;

	for (i = 0; i < height; i++) {
		const mlib_u16 *psrc_row = psrc + i * src_stride;
		mlib_u16 *pdst_row = pdst + i * dst_stride;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (j = 0; j <= (width - 8); j += 8) {
			pdst_row[j] = THRESH0(psrc_row[j]);
			pdst_row[j + 1] = THRESH1(psrc_row[j + 1]);
			pdst_row[j + 2] = THRESH0(psrc_row[j + 2]);
			pdst_row[j + 3] = THRESH1(psrc_row[j + 3]);
			pdst_row[j + 4] = THRESH0(psrc_row[j + 4]);
			pdst_row[j + 5] = THRESH1(psrc_row[j + 5]);
			pdst_row[j + 6] = THRESH0(psrc_row[j + 6]);
			pdst_row[j + 7] = THRESH1(psrc_row[j + 7]);
		}

		for (; j < width; j += 2) {
			pdst_row[j] = THRESH0(psrc_row[j]);
			pdst_row[j + 1] = THRESH1(psrc_row[j + 1]);
		}
	}
}

/* *********************************************************** */

void
mlib_c_ImageThresh5_U163(
    const mlib_u16 *psrc,
    mlib_s32 src_stride,
    mlib_u16 *pdst,
    mlib_s32 dst_stride,
    mlib_s32 width,
    mlib_s32 height,
    const mlib_s32 *thigh,
    const mlib_s32 *tlow,
    const mlib_s32 *gmid)
{
	mlib_s32 i, j;
	mlib_s32 thigh0, thigh1, thigh2;
	mlib_s32 tlow0, tlow1, tlow2;
	mlib_s32 gmid0, gmid1, gmid2;

	INIT_THRESH0(0);
	INIT_THRESH1(1);
	INIT_THRESH2(2);
	width = width * 3;
	src_stride >>= 1;
	dst_stride >>= 1;
	for (i = 0; i < height; i++) {
		const mlib_u16 *psrc_row = psrc + i * src_stride;
		mlib_u16 *pdst_row = pdst + i * dst_stride;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (j = 0; j <= (width - 12); j += 12) {
			pdst_row[j] = THRESH0(psrc_row[j]);
			pdst_row[j + 1] = THRESH1(psrc_row[j + 1]);
			pdst_row[j + 2] = THRESH2(psrc_row[j + 2]);
			pdst_row[j + 3] = THRESH0(psrc_row[j + 3]);
			pdst_row[j + 4] = THRESH1(psrc_row[j + 4]);
			pdst_row[j + 5] = THRESH2(psrc_row[j + 5]);
			pdst_row[j + 6] = THRESH0(psrc_row[j + 6]);
			pdst_row[j + 7] = THRESH1(psrc_row[j + 7]);
			pdst_row[j + 8] = THRESH2(psrc_row[j + 8]);
			pdst_row[j + 9] = THRESH0(psrc_row[j + 9]);
			pdst_row[j + 10] = THRESH1(psrc_row[j + 10]);
			pdst_row[j + 11] = THRESH2(psrc_row[j + 11]);
		}

		for (; j < width; j += 3) {
			pdst_row[j] = THRESH0(psrc_row[j]);
			pdst_row[j + 1] = THRESH1(psrc_row[j + 1]);
			pdst_row[j + 2] = THRESH2(psrc_row[j + 2]);
		}
	}
}

/* *********************************************************** */

void
mlib_c_ImageThresh5_U164(
    const mlib_u16 *psrc,
    mlib_s32 src_stride,
    mlib_u16 *pdst,
    mlib_s32 dst_stride,
    mlib_s32 width,
    mlib_s32 height,
    const mlib_s32 *thigh,
    const mlib_s32 *tlow,
    const mlib_s32 *gmid)
{
	mlib_s32 i, j;
	mlib_s32 thigh0, thigh1, thigh2, thigh3;
	mlib_s32 tlow0, tlow1, tlow2, tlow3;
	mlib_s32 gmid0, gmid1, gmid2, gmid3;

	INIT_THRESH0(0);
	INIT_THRESH1(1);
	INIT_THRESH2(2);
	INIT_THRESH3(3);
	width <<= 2;
	src_stride >>= 1;
	dst_stride >>= 1;
	for (i = 0; i < height; i++) {
		const mlib_u16 *psrc_row = psrc + i * src_stride;
		mlib_u16 *pdst_row = pdst + i * dst_stride;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (j = 0; j <= (width - 8); j += 8) {
			pdst_row[j] = THRESH0(psrc_row[j]);
			pdst_row[j + 1] = THRESH1(psrc_row[j + 1]);
			pdst_row[j + 2] = THRESH2(psrc_row[j + 2]);
			pdst_row[j + 3] = THRESH3(psrc_row[j + 3]);
			pdst_row[j + 4] = THRESH0(psrc_row[j + 4]);
			pdst_row[j + 5] = THRESH1(psrc_row[j + 5]);
			pdst_row[j + 6] = THRESH2(psrc_row[j + 6]);
			pdst_row[j + 7] = THRESH3(psrc_row[j + 7]);
		}

		if (j < width) {
			pdst_row[j] = THRESH0(psrc_row[j]);
			pdst_row[j + 1] = THRESH1(psrc_row[j + 1]);
			pdst_row[j + 2] = THRESH2(psrc_row[j + 2]);
			pdst_row[j + 3] = THRESH3(psrc_row[j + 3]);
		}
	}
}

/* *********************************************************** */

#define	DO_THRESH_S32(s0, th, tl, gm)                                 \
	((((mlib_u32)(s0 - tl)) <= ((mlib_u32)(th))) ? (gm) : (s0))

/* *********************************************************** */

#define	THRESH0_S32(s0)	DO_THRESH_S32(s0, thigh0, tlow0, gmid0)
#define	THRESH1_S32(s0)	DO_THRESH_S32(s0, thigh1, tlow1, gmid1)

#define	THRESH2_S32(s0)	DO_THRESH_S32(s0, thigh2, tlow2, gmid2)

#define	THRESH3_S32(s0)	DO_THRESH_S32(s0, thigh3, tlow3, gmid3)

/* *********************************************************** */

void
mlib_c_ImageThresh5_S321(
    const mlib_s32 *psrc,
    mlib_s32 src_stride,
    mlib_s32 *pdst,
    mlib_s32 dst_stride,
    mlib_s32 width,
    mlib_s32 height,
    const mlib_s32 *thigh,
    const mlib_s32 *tlow,
    const mlib_s32 *gmid)
{
	mlib_s32 i, j, k;
	mlib_s32 thigh0, tlow0;
	mlib_s32 gmid0;
	mlib_s32 src0, src1, src2, src3, src4, src5, src6, src7;

	src_stride >>= 2;
	dst_stride >>= 2;

	if (width < 16) {
		STRIP(pdst, psrc, width, height, 1, thigh, tlow, gmid,
		    mlib_s32);
		return;
	}

	INIT_THRESH0(0);
	thigh0 -= tlow0;

	for (i = 0; i < height; i++) {
		const mlib_s32 *psrc_row = psrc + i * src_stride;
		mlib_s32 *pdst_row = pdst + i * dst_stride;

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
			pdst_row[j] = THRESH0_S32(src0);
			pdst_row[j + 1] = THRESH0_S32(src1);
			pdst_row[j + 2] = THRESH0_S32(src2);
			pdst_row[j + 3] = THRESH0_S32(src3);
			pdst_row[j + 4] = THRESH0_S32(src4);
			pdst_row[j + 5] = THRESH0_S32(src5);
			pdst_row[j + 6] = THRESH0_S32(src6);
			pdst_row[j + 7] = THRESH0_S32(src7);
		}

		for (; j < width; j++) {
			pdst_row[j] = THRESH0_S32(psrc_row[j]);
		}
	}
}

/* *********************************************************** */

void
mlib_c_ImageThresh5_S322(
    const mlib_s32 *psrc,
    mlib_s32 src_stride,
    mlib_s32 *pdst,
    mlib_s32 dst_stride,
    mlib_s32 width,
    mlib_s32 height,
    const mlib_s32 *thigh,
    const mlib_s32 *tlow,
    const mlib_s32 *gmid)
{
	mlib_s32 i, j, k;
	mlib_s32 thigh0, thigh1;
	mlib_s32 tlow0, tlow1;
	mlib_s32 gmid0, gmid1;
	mlib_s32 src0, src1, src2, src3, src4, src5, src6, src7;

	src_stride >>= 2;
	dst_stride >>= 2;

	if (width < 16) {
		STRIP(pdst, psrc, width, height, 2, thigh, tlow, gmid,
		    mlib_s32);
		return;
	}

	INIT_THRESH0(0);
	INIT_THRESH1(1);
	thigh0 -= tlow0;
	thigh1 -= tlow1;

	width <<= 1;
	for (i = 0; i < height; i++) {
		const mlib_s32 *psrc_row = psrc + i * src_stride;
		mlib_s32 *pdst_row = pdst + i * dst_stride;

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
			pdst_row[j] = THRESH0_S32(src0);
			pdst_row[j + 1] = THRESH1_S32(src1);
			pdst_row[j + 2] = THRESH0_S32(src2);
			pdst_row[j + 3] = THRESH1_S32(src3);
			pdst_row[j + 4] = THRESH0_S32(src4);
			pdst_row[j + 5] = THRESH1_S32(src5);
			pdst_row[j + 6] = THRESH0_S32(src6);
			pdst_row[j + 7] = THRESH1_S32(src7);
		}

		for (; j < width; j += 2) {
			pdst_row[j] = THRESH0_S32(psrc_row[j]);
			pdst_row[j + 1] = THRESH1_S32(psrc_row[j + 1]);
		}
	}
}

/* *********************************************************** */

void
mlib_c_ImageThresh5_S323(
    const mlib_s32 *psrc,
    mlib_s32 src_stride,
    mlib_s32 *pdst,
    mlib_s32 dst_stride,
    mlib_s32 width,
    mlib_s32 height,
    const mlib_s32 *thigh,
    const mlib_s32 *tlow,
    const mlib_s32 *gmid)
{
	mlib_s32 i, j, k;
	mlib_s32 thigh0, thigh1, thigh2;
	mlib_s32 tlow0, tlow1, tlow2;
	mlib_s32 gmid0, gmid1, gmid2;
	mlib_s32 src0, src1, src2, src3, src4, src5;

	src_stride >>= 2;
	dst_stride >>= 2;

	if (width < 16) {
		STRIP(pdst, psrc, width, height, 3, thigh, tlow, gmid,
		    mlib_s32);
		return;
	}

	INIT_THRESH0(0);
	INIT_THRESH1(1);
	INIT_THRESH2(2);
	thigh0 -= tlow0;
	thigh1 -= tlow1;
	thigh2 -= tlow2;

	width = width * 3;
	for (i = 0; i < height; i++) {
		const mlib_s32 *psrc_row = psrc + i * src_stride;
		mlib_s32 *pdst_row = pdst + i * dst_stride;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (j = 0; j <= (width - 6); j += 6) {
			src0 = psrc_row[j];
			src1 = psrc_row[j + 1];
			src2 = psrc_row[j + 2];
			src3 = psrc_row[j + 3];
			src4 = psrc_row[j + 4];
			src5 = psrc_row[j + 5];
			pdst_row[j] = THRESH0_S32(src0);
			pdst_row[j + 1] = THRESH1_S32(src1);
			pdst_row[j + 2] = THRESH2_S32(src2);
			pdst_row[j + 3] = THRESH0_S32(src3);
			pdst_row[j + 4] = THRESH1_S32(src4);
			pdst_row[j + 5] = THRESH2_S32(src5);
		}

		for (; j < width; j += 3) {
			pdst_row[j] = THRESH0_S32(psrc_row[j]);
			pdst_row[j + 1] = THRESH1_S32(psrc_row[j + 1]);
			pdst_row[j + 2] = THRESH2_S32(psrc_row[j + 2]);
		}
	}
}

/* *********************************************************** */

void
mlib_c_ImageThresh5_S324(
    const mlib_s32 *psrc,
    mlib_s32 src_stride,
    mlib_s32 *pdst,
    mlib_s32 dst_stride,
    mlib_s32 width,
    mlib_s32 height,
    const mlib_s32 *thigh,
    const mlib_s32 *tlow,
    const mlib_s32 *gmid)
{
	mlib_s32 i, j, k;
	mlib_s32 thigh0, thigh1;
	mlib_s32 tlow0, tlow1;
	mlib_s32 src0, src1, src2, src3;
	mlib_s32 gmid0, gmid1;

	src_stride >>= 2;
	dst_stride >>= 2;

	if (width < 16) {
		STRIP(pdst, psrc, width, height, 4, thigh, tlow, gmid,
		    mlib_s32);
		return;
	}

	width <<= 2;
	for (i = 0; i < height; i++) {
		const mlib_s32 *psrc_row = psrc + i * src_stride;
		mlib_s32 *pdst_row = pdst + i * dst_stride;

		INIT_THRESH0(0);
		INIT_THRESH1(1);
		thigh0 -= tlow0;
		thigh1 -= tlow1;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (j = 0; j <= (width - 8); j += 8) {
			src0 = psrc_row[j];
			src1 = psrc_row[j + 1];
			src2 = psrc_row[j + 4];
			src3 = psrc_row[j + 5];
			pdst_row[j] = THRESH0_S32(src0);
			pdst_row[j + 1] = THRESH1_S32(src1);
			pdst_row[j + 4] = THRESH0_S32(src2);
			pdst_row[j + 5] = THRESH1_S32(src3);
		}

		if (j < width) {
			pdst_row[j] = THRESH0_S32(psrc_row[j]);
			pdst_row[j + 1] = THRESH1_S32(psrc_row[j + 1]);
		}

		INIT_THRESH0(2);
		INIT_THRESH1(3);
		thigh0 -= tlow0;
		thigh1 -= tlow1;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (j = 0; j <= (width - 8); j += 8) {
			src0 = psrc_row[j + 2];
			src1 = psrc_row[j + 3];
			src2 = psrc_row[j + 6];
			src3 = psrc_row[j + 7];
			pdst_row[j + 2] = THRESH0_S32(src0);
			pdst_row[j + 3] = THRESH1_S32(src1);
			pdst_row[j + 6] = THRESH0_S32(src2);
			pdst_row[j + 7] = THRESH1_S32(src3);
		}

		if (j < width) {
			pdst_row[j + 2] = THRESH0_S32(psrc_row[j + 2]);
			pdst_row[j + 3] = THRESH1_S32(psrc_row[j + 3]);
		}
	}
}

/* *********************************************************** */
