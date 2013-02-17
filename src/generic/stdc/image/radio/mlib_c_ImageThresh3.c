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

#pragma ident	"@(#)mlib_c_ImageThresh3.c	9.2	07/10/09 SMI"

/*
 * FUNCTION
 *      mlib_ImageThresh3 - thresholding
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageThresh3(mlib_image       *dst,
 *                                    const mlib_image *src,
 *                                    const mlib_s32   *thresh,
 *                                    const mlib_s32   *ghigh);
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
 *      The images can be in MLIB_BYTE, MLIB_SHORT,
 *      MLIB_USHORT or MLIB_INT data type.
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

#pragma weak mlib_ImageThresh3 = __mlib_ImageThresh3

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_ImageThresh3) mlib_ImageThresh3
    __attribute__((weak, alias("__mlib_ImageThresh3")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	STRIP(pd, ps, w, h, ch, th, gh, data_type)              \
	{                                                       \
	    data_type s0;                                       \
	                                                        \
	    for (i = 0; i < h; i++)                             \
		for (j = 0; j < w; j++)                         \
		    for (k = 0; k < ch; k++) {                  \
			s0 = ps[i * src_stride + j * ch + k];   \
			pd[i * dst_stride + j * ch + k] =       \
			    (s0 <= th[k]) ? s0 : gh[k];         \
		    }                                           \
	}

/* *********************************************************** */

#define	INIT_THRESH0(n)	thresh0 = thresh[(n)]; ghigh0 = ghigh[(n)]

/* *********************************************************** */

#define	INIT_THRESH1(n)	thresh1 = thresh[(n)]; ghigh1 = ghigh[(n)]

/* *********************************************************** */

#define	INIT_THRESH2(n)	thresh2 = thresh[(n)]; ghigh2 = ghigh[(n)]

/* *********************************************************** */

#define	INIT_THRESH3(n)	thresh3 = thresh[(n)]; ghigh3 = ghigh[(n)]

/* *********************************************************** */

#define	DO_THRESH(s0, th, gh)                                   \
	(((gh) & (((th) - (s0)) >> 31)) | ((s0) & ~(((th) -     \
	    (s0)) >> 31)))

/* *********************************************************** */

#define	THRESH0(s0)	DO_THRESH(s0, thresh0, ghigh0)
#define	THRESH1(s0)	DO_THRESH(s0, thresh1, ghigh1)

#define	THRESH2(s0)	DO_THRESH(s0, thresh2, ghigh2)

#define	THRESH3(s0)	DO_THRESH(s0, thresh3, ghigh3)

/* *********************************************************** */

#define	THRESH_INDEX(s0, index)                                 \
	(DO_THRESH(s0, thresh[index], ghigh[index]))

/* *********************************************************** */

static void mlib_c_ImageThresh3_U81(
    const mlib_u8 *psrc,
    mlib_s32 src_stride,
    mlib_u8 *pdst,
    mlib_s32 dst_stride,
    mlib_s32 width,
    mlib_s32 height,
    const mlib_s32 *thresh,
    const mlib_s32 *ghigh);

static void mlib_c_ImageThresh3_U82(
    const mlib_u8 *psrc,
    mlib_s32 src_stride,
    mlib_u8 *pdst,
    mlib_s32 dst_stride,
    mlib_s32 width,
    mlib_s32 height,
    const mlib_s32 *thresh,
    const mlib_s32 *ghigh);

static void mlib_c_ImageThresh3_U83(
    const mlib_u8 *psrc,
    mlib_s32 src_stride,
    mlib_u8 *pdst,
    mlib_s32 dst_stride,
    mlib_s32 width,
    mlib_s32 height,
    const mlib_s32 *thresh,
    const mlib_s32 *ghigh);

static void mlib_c_ImageThresh3_U84(
    const mlib_u8 *psrc,
    mlib_s32 src_stride,
    mlib_u8 *pdst,
    mlib_s32 dst_stride,
    mlib_s32 width,
    mlib_s32 height,
    const mlib_s32 *thresh,
    const mlib_s32 *ghigh);

static void mlib_c_ImageThresh3_S161(
    const mlib_s16 *psrc,
    mlib_s32 src_stride,
    mlib_s16 *pdst,
    mlib_s32 dst_stride,
    mlib_s32 width,
    mlib_s32 height,
    const mlib_s32 *thresh,
    const mlib_s32 *ghigh);

static void mlib_c_ImageThresh3_S162(
    const mlib_s16 *psrc,
    mlib_s32 src_stride,
    mlib_s16 *pdst,
    mlib_s32 dst_stride,
    mlib_s32 width,
    mlib_s32 height,
    const mlib_s32 *thresh,
    const mlib_s32 *ghigh);

static void mlib_c_ImageThresh3_S163(
    const mlib_s16 *psrc,
    mlib_s32 src_stride,
    mlib_s16 *pdst,
    mlib_s32 dst_stride,
    mlib_s32 width,
    mlib_s32 height,
    const mlib_s32 *thresh,
    const mlib_s32 *ghigh);

static void mlib_c_ImageThresh3_S164(
    const mlib_s16 *psrc,
    mlib_s32 src_stride,
    mlib_s16 *pdst,
    mlib_s32 dst_stride,
    mlib_s32 width,
    mlib_s32 height,
    const mlib_s32 *thresh,
    const mlib_s32 *ghigh);

static void mlib_c_ImageThresh3_U161(
    const mlib_u16 *psrc,
    mlib_s32 src_stride,
    mlib_u16 *pdst,
    mlib_s32 dst_stride,
    mlib_s32 width,
    mlib_s32 height,
    const mlib_s32 *thresh,
    const mlib_s32 *ghigh);

static void mlib_c_ImageThresh3_U162(
    const mlib_u16 *psrc,
    mlib_s32 src_stride,
    mlib_u16 *pdst,
    mlib_s32 dst_stride,
    mlib_s32 width,
    mlib_s32 height,
    const mlib_s32 *thresh,
    const mlib_s32 *ghigh);

static void mlib_c_ImageThresh3_U163(
    const mlib_u16 *psrc,
    mlib_s32 src_stride,
    mlib_u16 *pdst,
    mlib_s32 dst_stride,
    mlib_s32 width,
    mlib_s32 height,
    const mlib_s32 *thresh,
    const mlib_s32 *ghigh);

static void mlib_c_ImageThresh3_U164(
    const mlib_u16 *psrc,
    mlib_s32 src_stride,
    mlib_u16 *pdst,
    mlib_s32 dst_stride,
    mlib_s32 width,
    mlib_s32 height,
    const mlib_s32 *thresh,
    const mlib_s32 *ghigh);

static void mlib_c_ImageThresh3_S321(
    const mlib_s32 *psrc,
    mlib_s32 src_stride,
    mlib_s32 *pdst,
    mlib_s32 dst_stride,
    mlib_s32 width,
    mlib_s32 height,
    const mlib_s32 *thresh,
    const mlib_s32 *ghigh);

static void mlib_c_ImageThresh3_S322(
    const mlib_s32 *psrc,
    mlib_s32 src_stride,
    mlib_s32 *pdst,
    mlib_s32 dst_stride,
    mlib_s32 width,
    mlib_s32 height,
    const mlib_s32 *thresh,
    const mlib_s32 *ghigh);

static void mlib_c_ImageThresh3_S323(
    const mlib_s32 *psrc,
    mlib_s32 src_stride,
    mlib_s32 *pdst,
    mlib_s32 dst_stride,
    mlib_s32 width,
    mlib_s32 height,
    const mlib_s32 *thresh,
    const mlib_s32 *ghigh);

static void mlib_c_ImageThresh3_S324(
    const mlib_s32 *psrc,
    mlib_s32 src_stride,
    mlib_s32 *pdst,
    mlib_s32 dst_stride,
    mlib_s32 width,
    mlib_s32 height,
    const mlib_s32 *thresh,
    const mlib_s32 *ghigh);

/* *********************************************************** */

mlib_status
__mlib_ImageThresh3(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_s32 *thresh,
    const mlib_s32 *ghigh)
{
	mlib_s32 sstride, dstride, width, height, nchan;
	mlib_type dtype;
	void *psrc, *pdst;
	mlib_s32 th[4], tl[4], gh[4];
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

	if (dtype == MLIB_BYTE) {
		for (i = 0; i < nchan; i++) {

			if (thresh[i] < MLIB_U8_MIN) {
				for (i = 0; i < nchan; i++) {
					th[i] = MLIB_U8_MAX;
					gh[i] = ghigh[i] & 255;

					if (thresh[i] < MLIB_U8_MIN)
						tl[i] = MLIB_U8_MIN;
					else if (thresh[i] >= MLIB_U8_MAX)
						gh[i] = tl[i] = MLIB_U8_MAX;
					else
						tl[i] = thresh[i] + 1;
				}

				return __mlib_ImageThresh5(dst, src, th, tl,
				    gh);
			}
		}

		for (i = 0; i < nchan; i++) {
			gh[i] = ghigh[i] & 255;

			if (thresh[i] > MLIB_U8_MAX)
				th[i] = MLIB_U8_MAX;
			else
				th[i] = thresh[i];
		}

/* x * 3.5 < x * 2.2 + 256 */
		if ((width * height) < 200) {
			switch (nchan) {
			case 1:
				mlib_c_ImageThresh3_U81(psrc, sstride, pdst,
				    dstride, width, height, th, gh);
				break;
			case 2:
				mlib_c_ImageThresh3_U82(psrc, sstride, pdst,
				    dstride, width, height, th, gh);
				break;
			case 3:
				mlib_c_ImageThresh3_U83(psrc, sstride, pdst,
				    dstride, width, height, th, gh);
				break;
			case 4:
				mlib_c_ImageThresh3_U84(psrc, sstride, pdst,
				    dstride, width, height, th, gh);
				break;
			}
		} else {
			mlib_s32 i, c, t;
			mlib_d64 lookup[128];
			mlib_u8 *arr[4], *p;
			mlib_u8 ghu;

			arr[0] = (mlib_u8 *)(&(lookup[0]));
			arr[1] = (mlib_u8 *)(&(lookup[32]));
			arr[2] = (mlib_u8 *)(&(lookup[64]));
			arr[3] = (mlib_u8 *)(&(lookup[96]));
			for (c = 0; c < nchan; c++) {
				p = arr[c];
				t = th[c];
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i <= t; i++) {
					p[i] = i;
				}

				ghu = gh[c];
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (; i < 256; i++) {
					p[i] = ghu;
				}
			}

			return (__mlib_ImageLookUp
				(dst, src, (const void **)arr));
		}

		return (MLIB_SUCCESS);
	} else if (dtype == MLIB_SHORT) {
		for (i = 0; i < nchan; i++) {

			if (thresh[i] < MLIB_S16_MIN) {
				for (i = 0; i < nchan; i++) {
					th[i] = MLIB_S16_MAX;
					gh[i] = (ghigh[i] << 16) >> 16;

					if (thresh[i] < MLIB_S16_MIN)
						tl[i] = MLIB_S16_MIN;
					else if (thresh[i] >= MLIB_S16_MAX)
						gh[i] = tl[i] = MLIB_S16_MAX;
					else
						tl[i] = thresh[i] + 1;
				}

				return __mlib_ImageThresh5(dst, src, th, tl,
				    gh);
			}
		}

		for (i = 0; i < nchan; i++) {
			gh[i] = (ghigh[i] << 16) >> 16;

			if (thresh[i] > MLIB_S16_MAX)
				th[i] = MLIB_S16_MAX;
			else
				th[i] = thresh[i];
		}

		switch (nchan) {
		case 1:
			mlib_c_ImageThresh3_S161(psrc, sstride, pdst, dstride,
			    width, height, th, gh);
			break;
		case 2:
			mlib_c_ImageThresh3_S162(psrc, sstride, pdst, dstride,
			    width, height, th, gh);
			break;
		case 3:
			mlib_c_ImageThresh3_S163(psrc, sstride, pdst, dstride,
			    width, height, th, gh);
			break;
		case 4:
			mlib_c_ImageThresh3_S164(psrc, sstride, pdst, dstride,
			    width, height, th, gh);
			break;
		}

		return (MLIB_SUCCESS);
	} else if (dtype == MLIB_USHORT) {
		for (i = 0; i < nchan; i++) {

			if (thresh[i] < MLIB_U16_MIN) {
				for (i = 0; i < nchan; i++) {
					th[i] = MLIB_U16_MAX;
					gh[i] = (ghigh[i] << 16) >> 16;

					if (thresh[i] < MLIB_U16_MIN)
						tl[i] = MLIB_U16_MIN;
					else if (thresh[i] >= MLIB_U16_MAX)
						gh[i] = tl[i] = MLIB_U16_MAX;
					else
						tl[i] = thresh[i] + 1;
				}

				return __mlib_ImageThresh5(dst, src, th, tl,
				    gh);
			}
		}

		for (i = 0; i < nchan; i++) {
			gh[i] = (ghigh[i] << 16) >> 16;

			if (thresh[i] > MLIB_U16_MAX)
				th[i] = MLIB_U16_MAX;
			else
				th[i] = thresh[i];
		}

		switch (nchan) {
		case 1:
			mlib_c_ImageThresh3_U161(psrc, sstride, pdst, dstride,
			    width, height, th, gh);
			break;
		case 2:
			mlib_c_ImageThresh3_U162(psrc, sstride, pdst, dstride,
			    width, height, th, gh);
			break;
		case 3:
			mlib_c_ImageThresh3_U163(psrc, sstride, pdst, dstride,
			    width, height, th, gh);
			break;
		case 4:
			mlib_c_ImageThresh3_U164(psrc, sstride, pdst, dstride,
			    width, height, th, gh);
			break;
		}

		return (MLIB_SUCCESS);
	} else if (dtype == MLIB_INT) {
		switch (nchan) {
		case 1:
			mlib_c_ImageThresh3_S321(psrc, sstride,
			    pdst, dstride, width, height, thresh, ghigh);
			break;
		case 2:
			mlib_c_ImageThresh3_S322(psrc, sstride,
			    pdst, dstride, width, height, thresh, ghigh);
			break;
		case 3:
			mlib_c_ImageThresh3_S323(psrc, sstride,
			    pdst, dstride, width, height, thresh, ghigh);
			break;
		case 4:
			mlib_c_ImageThresh3_S324(psrc, sstride,
			    pdst, dstride, width, height, thresh, ghigh);
			break;
		}

		return (MLIB_SUCCESS);
	}

	return (MLIB_FAILURE);
}

/* *********************************************************** */

void
mlib_c_ImageThresh3_U81(
    const mlib_u8 *psrc,
    mlib_s32 src_stride,
    mlib_u8 *pdst,
    mlib_s32 dst_stride,
    mlib_s32 width,
    mlib_s32 height,
    const mlib_s32 *thresh,
    const mlib_s32 *ghigh)
{
	mlib_s32 i, j, k;
	mlib_s32 thresh0;
	mlib_s32 ghigh0;

	if (width < 16) {
		STRIP(pdst, psrc, width, height, 1, thresh, ghigh, mlib_u8);

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
mlib_c_ImageThresh3_U82(
    const mlib_u8 *psrc,
    mlib_s32 src_stride,
    mlib_u8 *pdst,
    mlib_s32 dst_stride,
    mlib_s32 width,
    mlib_s32 height,
    const mlib_s32 *thresh,
    const mlib_s32 *ghigh)
{
	mlib_s32 i, j, k;
	mlib_s32 thresh0, thresh1;
	mlib_s32 ghigh0, ghigh1;

	if (width < 16) {
		STRIP(pdst, psrc, width, height, 2, thresh, ghigh, mlib_u8);

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
mlib_c_ImageThresh3_U83(
    const mlib_u8 *psrc,
    mlib_s32 src_stride,
    mlib_u8 *pdst,
    mlib_s32 dst_stride,
    mlib_s32 width,
    mlib_s32 height,
    const mlib_s32 *thresh,
    const mlib_s32 *ghigh)
{
	mlib_s32 i, j, k;
	mlib_s32 thresh0, thresh1, thresh2;
	mlib_s32 ghigh0, ghigh1, ghigh2;

	if (width < 16) {
		STRIP(pdst, psrc, width, height, 3, thresh, ghigh, mlib_u8);

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
mlib_c_ImageThresh3_U84(
    const mlib_u8 *psrc,
    mlib_s32 src_stride,
    mlib_u8 *pdst,
    mlib_s32 dst_stride,
    mlib_s32 width,
    mlib_s32 height,
    const mlib_s32 *thresh,
    const mlib_s32 *ghigh)
{
	mlib_s32 i, j, k;
	mlib_s32 thresh0, thresh1, thresh2, thresh3;
	mlib_s32 ghigh0, ghigh1, ghigh2, ghigh3;

	if (width < 16) {
		STRIP(pdst, psrc, width, height, 4, thresh, ghigh, mlib_u8);

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
mlib_c_ImageThresh3_S161(
    const mlib_s16 *psrc,
    mlib_s32 src_stride,
    mlib_s16 *pdst,
    mlib_s32 dst_stride,
    mlib_s32 width,
    mlib_s32 height,
    const mlib_s32 *thresh,
    const mlib_s32 *ghigh)
{
	mlib_s32 i, j;
	mlib_s32 thresh0;
	mlib_s32 ghigh0;

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
mlib_c_ImageThresh3_S162(
    const mlib_s16 *psrc,
    mlib_s32 src_stride,
    mlib_s16 *pdst,
    mlib_s32 dst_stride,
    mlib_s32 width,
    mlib_s32 height,
    const mlib_s32 *thresh,
    const mlib_s32 *ghigh)
{
	mlib_s32 i, j;
	mlib_s32 thresh0, thresh1;
	mlib_s32 ghigh0, ghigh1;

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
mlib_c_ImageThresh3_S163(
    const mlib_s16 *psrc,
    mlib_s32 src_stride,
    mlib_s16 *pdst,
    mlib_s32 dst_stride,
    mlib_s32 width,
    mlib_s32 height,
    const mlib_s32 *thresh,
    const mlib_s32 *ghigh)
{
	mlib_s32 i, j;
	mlib_s32 thresh0, thresh1, thresh2;
	mlib_s32 ghigh0, ghigh1, ghigh2;

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
mlib_c_ImageThresh3_S164(
    const mlib_s16 *psrc,
    mlib_s32 src_stride,
    mlib_s16 *pdst,
    mlib_s32 dst_stride,
    mlib_s32 width,
    mlib_s32 height,
    const mlib_s32 *thresh,
    const mlib_s32 *ghigh)
{
	mlib_s32 i, j;
	mlib_s32 thresh0, thresh1, thresh2, thresh3;
	mlib_s32 ghigh0, ghigh1, ghigh2, ghigh3;

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
mlib_c_ImageThresh3_U161(
    const mlib_u16 *psrc,
    mlib_s32 src_stride,
    mlib_u16 *pdst,
    mlib_s32 dst_stride,
    mlib_s32 width,
    mlib_s32 height,
    const mlib_s32 *thresh,
    const mlib_s32 *ghigh)
{
	mlib_s32 i, j;
	mlib_s32 thresh0;
	mlib_s32 ghigh0;

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
mlib_c_ImageThresh3_U162(
    const mlib_u16 *psrc,
    mlib_s32 src_stride,
    mlib_u16 *pdst,
    mlib_s32 dst_stride,
    mlib_s32 width,
    mlib_s32 height,
    const mlib_s32 *thresh,
    const mlib_s32 *ghigh)
{
	mlib_s32 i, j;
	mlib_s32 thresh0, thresh1;
	mlib_s32 ghigh0, ghigh1;

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
mlib_c_ImageThresh3_U163(
    const mlib_u16 *psrc,
    mlib_s32 src_stride,
    mlib_u16 *pdst,
    mlib_s32 dst_stride,
    mlib_s32 width,
    mlib_s32 height,
    const mlib_s32 *thresh,
    const mlib_s32 *ghigh)
{
	mlib_s32 i, j;
	mlib_s32 thresh0, thresh1, thresh2;
	mlib_s32 ghigh0, ghigh1, ghigh2;

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
mlib_c_ImageThresh3_U164(
    const mlib_u16 *psrc,
    mlib_s32 src_stride,
    mlib_u16 *pdst,
    mlib_s32 dst_stride,
    mlib_s32 width,
    mlib_s32 height,
    const mlib_s32 *thresh,
    const mlib_s32 *ghigh)
{
	mlib_s32 i, j;
	mlib_s32 thresh0, thresh1, thresh2, thresh3;
	mlib_s32 ghigh0, ghigh1, ghigh2, ghigh3;

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

#define	DO_THRESH_S32(s0, th, gh)	((s0 > th) ? (gh) : (s0))

/* *********************************************************** */

#define	THRESH0_S32(s0)	DO_THRESH_S32(s0, thresh0, ghigh0)
#define	THRESH1_S32(s0)	DO_THRESH_S32(s0, thresh1, ghigh1)

#define	THRESH2_S32(s0)	DO_THRESH_S32(s0, thresh2, ghigh2)

#define	THRESH3_S32(s0)	DO_THRESH_S32(s0, thresh3, ghigh3)

/* *********************************************************** */

#define	THRESH_S32_LT0(s0)                                      \
	(((ghigh0) & (((thresh0 -                               \
	    (s0)) | ~(s0)) >> 31)) | ((s0) & ~(((thresh0 -      \
	    (s0)) | ~(s0)) >> 31)))

/* *********************************************************** */

#define	THRESH_S32_GE0(s0)                                      \
	(((ghigh0) & (((thresh0 -                               \
	    (s0)) & ~(s0)) >> 31)) | ((s0) & ~(((thresh0 -      \
	    (s0)) & ~(s0)) >> 31)))

/* *********************************************************** */

void
mlib_c_ImageThresh3_S321(
    const mlib_s32 *psrc,
    mlib_s32 src_stride,
    mlib_s32 *pdst,
    mlib_s32 dst_stride,
    mlib_s32 width,
    mlib_s32 height,
    const mlib_s32 *thresh,
    const mlib_s32 *ghigh)
{
	mlib_s32 i, j, k;
	mlib_s32 thresh0;
	mlib_s32 ghigh0;

	src_stride >>= 2;
	dst_stride >>= 2;

	if (width < 16) {
		STRIP(pdst, psrc, width, height, 1, thresh, ghigh, mlib_s32);

		return;
	}

	INIT_THRESH0(0);

	if (thresh0 < 0) {
		for (i = 0; i < height; i++) {
			const mlib_s32 *psrc_row = psrc + i * src_stride;
			mlib_s32 *pdst_row = pdst + i * dst_stride;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j = 0; j <= (width - 8); j += 8) {
				pdst_row[j] = THRESH_S32_LT0(psrc_row[j]);
				pdst_row[j + 1] =
				    THRESH_S32_LT0(psrc_row[j + 1]);
				pdst_row[j + 2] =
				    THRESH_S32_LT0(psrc_row[j + 2]);
				pdst_row[j + 3] =
				    THRESH_S32_LT0(psrc_row[j + 3]);
				pdst_row[j + 4] =
				    THRESH_S32_LT0(psrc_row[j + 4]);
				pdst_row[j + 5] =
				    THRESH_S32_LT0(psrc_row[j + 5]);
				pdst_row[j + 6] =
				    THRESH_S32_LT0(psrc_row[j + 6]);
				pdst_row[j + 7] =
				    THRESH_S32_LT0(psrc_row[j + 7]);
			}

			for (; j < width; j++) {
				pdst_row[j] = THRESH_S32_LT0(psrc_row[j]);
			}
		}
	} else {
		for (i = 0; i < height; i++) {
			const mlib_s32 *psrc_row = psrc + i * src_stride;
			mlib_s32 *pdst_row = pdst + i * dst_stride;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j = 0; j <= (width - 8); j += 8) {
				pdst_row[j] = THRESH_S32_GE0(psrc_row[j]);
				pdst_row[j + 1] =
				    THRESH_S32_GE0(psrc_row[j + 1]);
				pdst_row[j + 2] =
				    THRESH_S32_GE0(psrc_row[j + 2]);
				pdst_row[j + 3] =
				    THRESH_S32_GE0(psrc_row[j + 3]);
				pdst_row[j + 4] =
				    THRESH_S32_GE0(psrc_row[j + 4]);
				pdst_row[j + 5] =
				    THRESH_S32_GE0(psrc_row[j + 5]);
				pdst_row[j + 6] =
				    THRESH_S32_GE0(psrc_row[j + 6]);
				pdst_row[j + 7] =
				    THRESH_S32_GE0(psrc_row[j + 7]);
			}

			for (; j < width; j++) {
				pdst_row[j] = THRESH_S32_GE0(psrc_row[j]);
			}
		}
	}
}

/* *********************************************************** */

void
mlib_c_ImageThresh3_S322(
    const mlib_s32 *psrc,
    mlib_s32 src_stride,
    mlib_s32 *pdst,
    mlib_s32 dst_stride,
    mlib_s32 width,
    mlib_s32 height,
    const mlib_s32 *thresh,
    const mlib_s32 *ghigh)
{
	mlib_s32 i, j, k;
	mlib_s32 thresh0 = thresh[0], thresh1 = thresh[1];
	mlib_s32 high0 = ghigh[0], high1 = ghigh[1];
	mlib_s32 src0, src1, src2, src3, src4, src5, src6, src7;

	src_stride >>= 2;
	dst_stride >>= 2;

	if (width < 16) {
		STRIP(pdst, psrc, width, height, 2, thresh, ghigh, mlib_s32);

		return;
	}

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
mlib_c_ImageThresh3_S323(
    const mlib_s32 *psrc,
    mlib_s32 src_stride,
    mlib_s32 *pdst,
    mlib_s32 dst_stride,
    mlib_s32 width,
    mlib_s32 height,
    const mlib_s32 *thresh,
    const mlib_s32 *ghigh)
{
	mlib_s32 i, j, k;
	mlib_s32 thresh0 = thresh[0], thresh1 = thresh[1];
	mlib_s32 thresh2 = thresh[2];
	mlib_s32 high0 = ghigh[0];
	mlib_s32 high1 = ghigh[1];
	mlib_s32 high2 = ghigh[2];
	mlib_s32 src0, src1, src2, src3, src4, src5, src6, src7, src8, src9,
	    src10, src11;

	src_stride >>= 2;
	dst_stride >>= 2;

	if (width < 16) {
		STRIP(pdst, psrc, width, height, 3, thresh, ghigh, mlib_s32);

		return;
	}

	width = width * 3;
	for (i = 0; i < height; i++) {
		const mlib_s32 *psrc_row = psrc + i * src_stride;
		mlib_s32 *pdst_row = pdst + i * dst_stride;

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
mlib_c_ImageThresh3_S324(
    const mlib_s32 *psrc,
    mlib_s32 src_stride,
    mlib_s32 *pdst,
    mlib_s32 dst_stride,
    mlib_s32 width,
    mlib_s32 height,
    const mlib_s32 *thresh,
    const mlib_s32 *ghigh)
{
	mlib_s32 i, j, k;
	mlib_s32 thresh0 = thresh[0], thresh1 = thresh[1];
	mlib_s32 thresh2 = thresh[2], thresh3 = thresh[3];
	mlib_s32 high0 = ghigh[0], high1 = ghigh[1];
	mlib_s32 high2 = ghigh[2], high3 = ghigh[3];
	mlib_s32 src0, src1, src2, src3, src4, src5, src6, src7;

	src_stride >>= 2;
	dst_stride >>= 2;

	if (width < 16) {
		STRIP(pdst, psrc, width, height, 4, thresh, ghigh, mlib_s32);

		return;
	}

	width <<= 2;
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
