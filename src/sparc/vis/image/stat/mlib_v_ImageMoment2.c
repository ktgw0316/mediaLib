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

#pragma ident	"@(#)mlib_v_ImageMoment2.c	9.3	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_ImageMoment2 - calculates moment of power two
 *                          for the input image
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageMoment2(mlib_d64         *moment,
 *                                    const mlib_image *img);
 *
 * ARGUMENTS
 *      moment   pointer to moment array
 *      img      pointer to an image
 *
 * DESCRIPTION
 *
 *           1    w-1 h-1         2
 *      c = --- * SUM SUM(x[i][j])
 *          w*h   i = 0 j = 0
 *
 * RESTRICTION
 *      The image can have 1, 2, 3 or 4 channels.
 *      It can be of MLIB_BYTE, MLIB_SHORT, MLIB_USHORT or MLIB_INT data type.
 */

#include <vis_proto.h>
#include <mlib_image.h>
#include <mlib_SysMath.h>
#include <mlib_ImageCheck.h>
#include <mlib_v_ImageMoment2.h>
#include <mlib_v_ImageStat.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageMoment2 = __mlib_ImageMoment2

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_ImageMoment2) mlib_ImageMoment2
    __attribute__((weak, alias("__mlib_ImageMoment2")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

mlib_status
__mlib_ImageMoment2(
    mlib_d64 *moment,
    const mlib_image *img)
{
/* total number of pixels */
	mlib_s32 num_pixel;
	mlib_d64 res[4];
	mlib_d64 divider;

/* check for obvious errors */
	MLIB_IMAGE_CHECK(img);

	if (moment == NULL)
		return (MLIB_NULLPOINTER);

/* total number of pixels */
	num_pixel = mlib_ImageGetWidth(img) * mlib_ImageGetHeight(img);
	divider = 1.0 / (mlib_d64)num_pixel;

	switch (mlib_ImageGetType(img)) {
/* handle MLIB_BYTE data type of image */
	case MLIB_BYTE:

		if (mlib_ImageGetChannels(img) == 3)
			mlib_v_ImageMoment2_U8_3(img, res);
		else
			mlib_v_ImageMoment2_U8_124(img, res);
		break;

/* handle MLIB_SHORT data type of image */
	case MLIB_USHORT:
		mlib_v_ImageMoment2_U16(img, moment);
		return (MLIB_SUCCESS);

/* handle MLIB_SHORT data type of image */
	case MLIB_SHORT:

		if (mlib_ImageGetChannels(img) == 3)
			mlib_v_ImageMoment2_S16_3(img, res);
		else
			mlib_v_ImageMoment2_S16_124(img, res);
		break;

/* handle MLIB_INT data type of image */
	case MLIB_INT:

		if (mlib_ImageGetChannels(img) == 3)
			mlib_v_ImageMoment2_S32_3(img, res);
		else
			mlib_v_ImageMoment2_S32_124(img, res);
		break;

/* discard any other data types */
	default:
		return (MLIB_FAILURE);
	}

	switch (mlib_ImageGetChannels(img)) {
	case 1:
		moment[0] = (res[0] + res[1] + res[2] + res[3]) * divider;
		break;

	case 2:
		moment[0] = (res[0] + res[2]) * divider;
		moment[1] = (res[1] + res[3]) * divider;
		break;

	case 4:
		moment[3] = res[3] * divider;
	case 3:
		moment[0] = res[0] * divider;
		moment[1] = res[1] * divider;
		moment[2] = res[2] * divider;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

void
mlib_v_ImageMoment2_U8_124(
    const mlib_image *img,
    mlib_d64 *res)
{
/* pointer for pixel of source */
	mlib_u8 *srcPixelPtr = (mlib_u8 *)mlib_ImageGetData(img);

/* pointer for row of source */
	mlib_u8 *srcRowPtr = (mlib_u8 *)srcPixelPtr;

/* width of source image */
	mlib_s32 srcWidth = mlib_ImageGetWidth(img);

/* height of source image */
	mlib_s32 srcHeight = mlib_ImageGetHeight(img);

/* total number of pixels */
	mlib_s32 srcTotalPixNum = srcHeight * srcWidth;

/* linestride = bytes to next row */
	mlib_s32 srcYStride = mlib_ImageGetStride(img);

/* number of channels */
	mlib_s32 numOfChannels = mlib_ImageGetChannels(img);

/* indices */
	mlib_s32 i, j;
	mlib_s32 alignBytes;
	mlib_s32 tailBytes;
	mlib_s32 loadsNumber;
	mlib_s32 cycleNumber;

	mlib_s32 lEdgeMask, rEdgeMask;
	mlib_d64 tmpArr[1];
	mlib_d64 *srcLoadPtr;
	mlib_d64 dzero = vis_fzero();
	mlib_d64 srcLoad, loadHi, loadLo, mul01, mul23, mul45, mul67, acc1 =
	    dzero, acc2 = dzero;
	mlib_d64 mtwo0, mtwo1, mtwo2, mtwo3;
	mlib_f32 scale = vis_to_float(0x100);
	mlib_s32 byteWidth = srcWidth << (numOfChannels >> 1);
	type_union_mlib_d64 utmp0, utmp1;

	mtwo0 = mtwo1 = mtwo2 = mtwo3 = dzero;

	for (i = 0; i < 4; i++)
		res[i] = dzero;

	if (byteWidth == srcYStride) {
		srcWidth = srcTotalPixNum;
		byteWidth = srcTotalPixNum << (numOfChannels >> 1);
		srcHeight = 1;
	}

	for (j = 0; j < srcHeight; j++) {
		tmpArr[0] = dzero;
		alignBytes = (mlib_addr)srcPixelPtr & 7;
		tailBytes = byteWidth + alignBytes;
		loadsNumber = tailBytes >> 3;
		tailBytes &= 7;

		lEdgeMask = vis_edge8(srcPixelPtr, srcPixelPtr + byteWidth - 1);
		rEdgeMask = ~(0xff >> tailBytes);

		srcLoadPtr = (mlib_d64 *)(srcPixelPtr - alignBytes);
		alignBytes &= 3;

		srcLoad = (*srcLoadPtr++);
		vis_pst_8(srcLoad, tmpArr, lEdgeMask);
		srcLoad = tmpArr[0];

		acc1 = acc2 = dzero;
		while (loadsNumber > 0) {
			cycleNumber =
			    (loadsNumber >= 0x4000) ? 0x4000 : loadsNumber;
			loadsNumber -= cycleNumber;
			for (i = 0; i < cycleNumber; i++) {
				loadHi =
				    vis_fpmerge(vis_read_hi(dzero),
				    vis_read_hi(srcLoad));
				loadLo =
				    vis_fmul8x16al(vis_read_lo(srcLoad), scale);

				srcLoad = vis_ld_d64_nf(srcLoadPtr);
				srcLoadPtr++;

				mul01 =
				    vis_fmuld8ulx16(vis_read_hi(loadHi),
				    vis_read_hi(loadHi));
				mul23 =
				    vis_fmuld8ulx16(vis_read_lo(loadHi),
				    vis_read_lo(loadHi));
				mul45 =
				    vis_fmuld8ulx16(vis_read_hi(loadLo),
				    vis_read_hi(loadLo));
				mul67 =
				    vis_fmuld8ulx16(vis_read_lo(loadLo),
				    vis_read_lo(loadLo));
				acc1 = vis_fpadd32(acc1, mul01);
				acc2 = vis_fpadd32(acc2, mul23);
				acc1 = vis_fpadd32(acc1, mul45);
				acc2 = vis_fpadd32(acc2, mul67);
			}

			utmp0.db = acc1;
			utmp1.db = acc2;
			mtwo0 += utmp0.two_int.int0;
			mtwo1 += utmp0.two_int.int1;
			mtwo2 += utmp1.two_int.int0;
			mtwo3 += utmp1.two_int.int1;
			acc1 = acc2 = dzero;
		}

		tmpArr[0] = dzero;

		if (tailBytes) {
			vis_pst_8(srcLoad, tmpArr, rEdgeMask);
			srcLoad = tmpArr[0];
			loadHi =
			    vis_fpmerge(vis_read_hi(dzero),
			    vis_read_hi(srcLoad));
			loadLo = vis_fmul8x16al(vis_read_lo(srcLoad), scale);

			mul01 =
			    vis_fmuld8ulx16(vis_read_hi(loadHi),
			    vis_read_hi(loadHi));
			mul23 =
			    vis_fmuld8ulx16(vis_read_lo(loadHi),
			    vis_read_lo(loadHi));
			mul45 =
			    vis_fmuld8ulx16(vis_read_hi(loadLo),
			    vis_read_hi(loadLo));
			mul67 =
			    vis_fmuld8ulx16(vis_read_lo(loadLo),
			    vis_read_lo(loadLo));
			acc1 = vis_fpadd32(acc1, mul01);
			acc2 = vis_fpadd32(acc2, mul23);
			acc1 = vis_fpadd32(acc1, mul45);
			acc2 = vis_fpadd32(acc2, mul67);

			utmp0.db = acc1;
			utmp1.db = acc2;
			mtwo0 += utmp0.two_int.int0;
			mtwo1 += utmp0.two_int.int1;
			mtwo2 += utmp1.two_int.int0;
			mtwo3 += utmp1.two_int.int1;
			acc1 = acc2 = dzero;
		}

		res[3 & (0 - alignBytes)] += mtwo0;
		res[3 & (1 - alignBytes)] += mtwo1;
		res[3 & (2 - alignBytes)] += mtwo2;
		res[(3 - alignBytes)] += mtwo3;

		mtwo0 = mtwo1 = mtwo2 = mtwo3 = dzero;

		srcRowPtr += srcYStride;
		srcPixelPtr = srcRowPtr;
	}
}

/* *********************************************************** */

void
mlib_v_ImageMoment2_U8_3(
    const mlib_image *img,
    mlib_d64 *res)
{
/* pointer for pixel of source */
	mlib_u8 *srcPixelPtr = (mlib_u8 *)mlib_ImageGetData(img);

/* pointer for row of source */
	mlib_u8 *srcRowPtr = (mlib_u8 *)srcPixelPtr;

/* width of source image */
	mlib_s32 srcWidth = mlib_ImageGetWidth(img);

/* height of source image */
	mlib_s32 srcHeight = mlib_ImageGetHeight(img);

/* linestride = bytes to next row */
	mlib_s32 srcYStride = mlib_ImageGetStride(img);

/* indices */
	mlib_s32 i, j;
	mlib_s32 alignBytes;
	mlib_s32 tailBytes;
	mlib_s32 loadsNumber;
	mlib_s32 cycleNumber;

	mlib_s32 lEdgeMask, rEdgeMask;
	mlib_d64 tmpArr[3];
	mlib_d64 *srcLoadPtr;
	mlib_d64 dzero = vis_fzero();
	mlib_d64 srcLoad, loadHi, loadLo, mul01, mul23, mul45, mul67;
	mlib_d64 acc1 = dzero, acc2 = dzero, acc3 = dzero;
	mlib_d64 mtwo0, mtwo1, mtwo2;
	mlib_f32 scale = vis_to_float(0x100);
	mlib_s32 byteWidth = srcWidth * 3;
	type_union_mlib_d64 utmp0, utmp1, utmp2;

/* compute mean3_off(align, sumn) */
	const mlib_u8 al2off[12] = { 0, 2, 1, 0, 2, 1, 0, 2, 1, 0, 2, 1 };

	mtwo0 = mtwo1 = mtwo2 = dzero;
	res[0] = res[1] = res[2] = dzero;

	if (byteWidth == srcYStride) {
		srcWidth *= srcHeight;
		byteWidth = srcWidth * 3;
		srcHeight = 1;
	}

	for (j = 0; j < srcHeight; j++) {
		tmpArr[0] = dzero;
		alignBytes = (mlib_addr)srcPixelPtr & 7;
		loadsNumber = byteWidth + alignBytes;

		lEdgeMask = vis_edge8(srcPixelPtr, srcPixelPtr + byteWidth - 1);

		srcLoadPtr = (mlib_d64 *)(srcPixelPtr - alignBytes);
		srcLoad = (*srcLoadPtr++);
		vis_pst_8(srcLoad, tmpArr, lEdgeMask);
		srcLoad = tmpArr[0];
		cycleNumber = loadsNumber / 24;
		tailBytes = loadsNumber - 24 * cycleNumber;

		while (cycleNumber > 0) {
			loadsNumber =
			    (cycleNumber >= 0x1000) ? 0x1000 : cycleNumber;
			cycleNumber -= loadsNumber;
			loadHi =
			    vis_fpmerge(vis_read_hi(dzero),
			    vis_read_hi(srcLoad));
			loadLo = vis_fmul8x16al(vis_read_lo(srcLoad), scale);
			srcLoad = (*srcLoadPtr++);

			mul01 =
			    vis_fmuld8ulx16(vis_read_hi(loadHi),
			    vis_read_hi(loadHi));
			mul23 =
			    vis_fmuld8ulx16(vis_read_lo(loadHi),
			    vis_read_lo(loadHi));
#pragma pipeloop(0)
			for (i = 1; i < loadsNumber; i++) {
				mul45 =
				    vis_fmuld8ulx16(vis_read_hi(loadLo),
				    vis_read_hi(loadLo));
				mul67 =
				    vis_fmuld8ulx16(vis_read_lo(loadLo),
				    vis_read_lo(loadLo));
				acc1 = vis_fpadd32(acc1, mul01);
				acc2 = vis_fpadd32(acc2, mul23);
				acc3 = vis_fpadd32(acc3, mul45);
				acc1 = vis_fpadd32(acc1, mul67);

				loadHi =
				    vis_fpmerge(vis_read_hi(dzero),
				    vis_read_hi(srcLoad));
				loadLo =
				    vis_fmul8x16al(vis_read_lo(srcLoad), scale);
				srcLoad = (*srcLoadPtr++);

				mul01 =
				    vis_fmuld8ulx16(vis_read_hi(loadHi),
				    vis_read_hi(loadHi));
				mul23 =
				    vis_fmuld8ulx16(vis_read_lo(loadHi),
				    vis_read_lo(loadHi));
				mul45 =
				    vis_fmuld8ulx16(vis_read_hi(loadLo),
				    vis_read_hi(loadLo));
				mul67 =
				    vis_fmuld8ulx16(vis_read_lo(loadLo),
				    vis_read_lo(loadLo));
				acc2 = vis_fpadd32(acc2, mul01);
				acc3 = vis_fpadd32(acc3, mul23);
				acc1 = vis_fpadd32(acc1, mul45);
				acc2 = vis_fpadd32(acc2, mul67);

				loadHi =
				    vis_fpmerge(vis_read_hi(dzero),
				    vis_read_hi(srcLoad));
				loadLo =
				    vis_fmul8x16al(vis_read_lo(srcLoad), scale);
				srcLoad = (*srcLoadPtr++);

				mul01 =
				    vis_fmuld8ulx16(vis_read_hi(loadHi),
				    vis_read_hi(loadHi));
				mul23 =
				    vis_fmuld8ulx16(vis_read_lo(loadHi),
				    vis_read_lo(loadHi));
				mul45 =
				    vis_fmuld8ulx16(vis_read_hi(loadLo),
				    vis_read_hi(loadLo));
				mul67 =
				    vis_fmuld8ulx16(vis_read_lo(loadLo),
				    vis_read_lo(loadLo));
				acc3 = vis_fpadd32(acc3, mul01);
				acc1 = vis_fpadd32(acc1, mul23);
				acc2 = vis_fpadd32(acc2, mul45);
				acc3 = vis_fpadd32(acc3, mul67);

				loadHi =
				    vis_fpmerge(vis_read_hi(dzero),
				    vis_read_hi(srcLoad));
				loadLo =
				    vis_fmul8x16al(vis_read_lo(srcLoad), scale);
				srcLoad = (*srcLoadPtr++);

				mul01 =
				    vis_fmuld8ulx16(vis_read_hi(loadHi),
				    vis_read_hi(loadHi));
				mul23 =
				    vis_fmuld8ulx16(vis_read_lo(loadHi),
				    vis_read_lo(loadHi));
			}

			mul45 =
			    vis_fmuld8ulx16(vis_read_hi(loadLo),
			    vis_read_hi(loadLo));
			mul67 =
			    vis_fmuld8ulx16(vis_read_lo(loadLo),
			    vis_read_lo(loadLo));
			acc1 = vis_fpadd32(acc1, mul01);
			acc2 = vis_fpadd32(acc2, mul23);
			acc3 = vis_fpadd32(acc3, mul45);
			acc1 = vis_fpadd32(acc1, mul67);

			loadHi =
			    vis_fpmerge(vis_read_hi(dzero),
			    vis_read_hi(srcLoad));
			loadLo = vis_fmul8x16al(vis_read_lo(srcLoad), scale);
			srcLoad = (*srcLoadPtr++);

			mul01 =
			    vis_fmuld8ulx16(vis_read_hi(loadHi),
			    vis_read_hi(loadHi));
			mul23 =
			    vis_fmuld8ulx16(vis_read_lo(loadHi),
			    vis_read_lo(loadHi));
			mul45 =
			    vis_fmuld8ulx16(vis_read_hi(loadLo),
			    vis_read_hi(loadLo));
			mul67 =
			    vis_fmuld8ulx16(vis_read_lo(loadLo),
			    vis_read_lo(loadLo));
			acc2 = vis_fpadd32(acc2, mul01);
			acc3 = vis_fpadd32(acc3, mul23);
			acc1 = vis_fpadd32(acc1, mul45);
			acc2 = vis_fpadd32(acc2, mul67);

			loadHi =
			    vis_fpmerge(vis_read_hi(dzero),
			    vis_read_hi(srcLoad));
			loadLo = vis_fmul8x16al(vis_read_lo(srcLoad), scale);
			srcLoad = vis_ld_d64_nf(srcLoadPtr);
			srcLoadPtr++;

			mul01 =
			    vis_fmuld8ulx16(vis_read_hi(loadHi),
			    vis_read_hi(loadHi));
			mul23 =
			    vis_fmuld8ulx16(vis_read_lo(loadHi),
			    vis_read_lo(loadHi));
			mul45 =
			    vis_fmuld8ulx16(vis_read_hi(loadLo),
			    vis_read_hi(loadLo));
			mul67 =
			    vis_fmuld8ulx16(vis_read_lo(loadLo),
			    vis_read_lo(loadLo));
			acc3 = vis_fpadd32(acc3, mul01);
			acc1 = vis_fpadd32(acc1, mul23);
			acc2 = vis_fpadd32(acc2, mul45);
			acc3 = vis_fpadd32(acc3, mul67);

			utmp0.db = acc1;
			utmp1.db = acc2;
			utmp2.db = acc3;
			utmp0.two_float.fl0 =
			    vis_fpadd32s(utmp0.two_float.fl0,
			    utmp1.two_float.fl1);
			utmp0.two_float.fl1 =
			    vis_fpadd32s(utmp0.two_float.fl1,
			    utmp2.two_float.fl0);
			utmp1.two_float.fl0 =
			    vis_fpadd32s(utmp1.two_float.fl0,
			    utmp2.two_float.fl1);
			mtwo0 += utmp0.two_int.int0;
			mtwo1 += utmp0.two_int.int1;
			mtwo2 += utmp1.two_int.int0;
			acc1 = acc2 = acc3 = dzero;
		}

		tmpArr[0] = tmpArr[1] = tmpArr[2] = dzero;

		if (tailBytes) {
			rEdgeMask = ~(0xffffff >> tailBytes);
			vis_pst_8(srcLoad, tmpArr, rEdgeMask >> 16);
			srcLoad = vis_ld_d64_nf(srcLoadPtr);
			srcLoadPtr++;
			vis_pst_8(srcLoad, tmpArr + 1, rEdgeMask >> 8);
			srcLoad = vis_ld_d64_nf(srcLoadPtr);
			srcLoadPtr++;
			vis_pst_8(srcLoad, tmpArr + 2, rEdgeMask);

			srcLoad = tmpArr[0];

			loadHi =
			    vis_fpmerge(vis_read_hi(dzero),
			    vis_read_hi(srcLoad));
			loadLo = vis_fmul8x16al(vis_read_lo(srcLoad), scale);

			mul01 =
			    vis_fmuld8ulx16(vis_read_hi(loadHi),
			    vis_read_hi(loadHi));
			mul23 =
			    vis_fmuld8ulx16(vis_read_lo(loadHi),
			    vis_read_lo(loadHi));
			mul45 =
			    vis_fmuld8ulx16(vis_read_hi(loadLo),
			    vis_read_hi(loadLo));
			mul67 =
			    vis_fmuld8ulx16(vis_read_lo(loadLo),
			    vis_read_lo(loadLo));
			acc1 = vis_fpadd32(acc1, mul01);
			acc2 = vis_fpadd32(acc2, mul23);
			acc3 = vis_fpadd32(acc3, mul45);
			acc1 = vis_fpadd32(acc1, mul67);

			srcLoad = tmpArr[1];
			loadHi =
			    vis_fpmerge(vis_read_hi(dzero),
			    vis_read_hi(srcLoad));
			loadLo = vis_fmul8x16al(vis_read_lo(srcLoad), scale);

			mul01 =
			    vis_fmuld8ulx16(vis_read_hi(loadHi),
			    vis_read_hi(loadHi));
			mul23 =
			    vis_fmuld8ulx16(vis_read_lo(loadHi),
			    vis_read_lo(loadHi));
			mul45 =
			    vis_fmuld8ulx16(vis_read_hi(loadLo),
			    vis_read_hi(loadLo));
			mul67 =
			    vis_fmuld8ulx16(vis_read_lo(loadLo),
			    vis_read_lo(loadLo));
			acc2 = vis_fpadd32(acc2, mul01);
			acc3 = vis_fpadd32(acc3, mul23);
			acc1 = vis_fpadd32(acc1, mul45);
			acc2 = vis_fpadd32(acc2, mul67);

			srcLoad = tmpArr[2];
			loadHi =
			    vis_fpmerge(vis_read_hi(dzero),
			    vis_read_hi(srcLoad));
			loadLo = vis_fmul8x16al(vis_read_lo(srcLoad), scale);

			mul01 =
			    vis_fmuld8ulx16(vis_read_hi(loadHi),
			    vis_read_hi(loadHi));
			mul23 =
			    vis_fmuld8ulx16(vis_read_lo(loadHi),
			    vis_read_lo(loadHi));
			mul45 =
			    vis_fmuld8ulx16(vis_read_hi(loadLo),
			    vis_read_hi(loadLo));
			mul67 =
			    vis_fmuld8ulx16(vis_read_lo(loadLo),
			    vis_read_lo(loadLo));
			acc3 = vis_fpadd32(acc3, mul01);
			acc1 = vis_fpadd32(acc1, mul23);
			acc2 = vis_fpadd32(acc2, mul45);
			acc3 = vis_fpadd32(acc3, mul67);

			utmp0.db = acc1;
			utmp1.db = acc2;
			utmp2.db = acc3;
			utmp0.two_float.fl0 =
			    vis_fpadd32s(utmp0.two_float.fl0,
			    utmp1.two_float.fl1);
			utmp0.two_float.fl1 =
			    vis_fpadd32s(utmp0.two_float.fl1,
			    utmp2.two_float.fl0);
			utmp1.two_float.fl0 =
			    vis_fpadd32s(utmp1.two_float.fl0,
			    utmp2.two_float.fl1);
			mtwo0 += utmp0.two_int.int0;
			mtwo1 += utmp0.two_int.int1;
			mtwo2 += utmp1.two_int.int0;
			acc1 = acc2 = acc3 = dzero;
		}

		res[al2off[alignBytes + 0]] += mtwo0;
		res[al2off[alignBytes + 2]] += mtwo1;
		res[al2off[alignBytes + 1]] += mtwo2;

		mtwo0 = mtwo1 = mtwo2 = dzero;

		srcRowPtr += srcYStride;
		srcPixelPtr = srcRowPtr;
	}
}

/* *********************************************************** */

#define	MUL3CH                                                             \
	dr1 =                                                              \
	vis_fmuld8sux16(vis_read_hi(srcLoad), vis_read_hi(srcLoad));       \
	dr2 =                                                              \
	vis_fmuld8ulx16(vis_read_hi(srcLoad), vis_read_hi(srcLoad));       \
	dr3 =                                                              \
	vis_fmuld8sux16(vis_read_lo(secondLoad),                           \
	    vis_read_lo(secondLoad));                                      \
	dr4 =                                                              \
	vis_fmuld8ulx16(vis_read_lo(secondLoad),                           \
	    vis_read_lo(secondLoad));                                      \
	dr1 = vis_fpadd32(dr1, mone);                                      \
/*                                                                         \
 * sub one                                                                 \
 */                                                                        \
	dr2 = vis_fpadd32(dr1, dr2);                                       \
	dr3 = vis_fpadd32(dr2, dr3);                                       \
	dc1.db = vis_fpadd32(dr3, dr4);                                    \
	mtwo0 += (mlib_d64)dc1.two_int.int0;                               \
	mtwo1 += (mlib_d64)dc1.two_int.int1;                               \
	dr1 =                                                              \
	vis_fmuld8sux16(vis_read_hi(secondLoad),                           \
	    vis_read_hi(secondLoad));                                      \
	dr2 =                                                              \
	vis_fmuld8ulx16(vis_read_hi(secondLoad),                           \
	    vis_read_hi(secondLoad));                                      \
	dr3 =                                                              \
	vis_fmuld8sux16(vis_read_lo(thirdLoad), vis_read_lo(thirdLoad));   \
	dr4 =                                                              \
	vis_fmuld8ulx16(vis_read_lo(thirdLoad), vis_read_lo(thirdLoad));   \
	dr1 = vis_fpadd32(dr1, mone);                                      \
/*                                                                         \
 * sub one                                                                 \
 */                                                                        \
	dr2 = vis_fpadd32(dr1, dr2);                                       \
	dr3 = vis_fpadd32(dr2, dr3);                                       \
	dc1.db = vis_fpadd32(dr3, dr4);                                    \
	mtwo1 += (mlib_d64)dc1.two_int.int0;                               \
	mtwo2 += (mlib_d64)dc1.two_int.int1;                               \
	dr1 =                                                              \
	vis_fmuld8sux16(vis_read_hi(thirdLoad), vis_read_hi(thirdLoad));   \
	dr2 =                                                              \
	vis_fmuld8ulx16(vis_read_hi(thirdLoad), vis_read_hi(thirdLoad));   \
	dr3 =                                                              \
	vis_fmuld8sux16(vis_read_lo(srcLoad), vis_read_lo(srcLoad));       \
	dr4 =                                                              \
	vis_fmuld8ulx16(vis_read_lo(srcLoad), vis_read_lo(srcLoad));       \
	dr1 = vis_fpadd32(dr1, mone);                                      \
/*                                                                         \
 * sub one                                                                 \
 */                                                                        \
	dr2 = vis_fpadd32(dr1, dr2);                                       \
	dr3 = vis_fpadd32(dr2, dr3);                                       \
	dc1.db = vis_fpadd32(dr3, dr4);                                    \
	mtwo2 += (mlib_d64)dc1.two_int.int0;                               \
	mtwo0 += (mlib_d64)dc1.two_int.int1

/* *********************************************************** */

#define	ACC3CH                                                             \
	acc1 =                                                             \
	vis_fpadd32(acc1, vis_fmuld8ulx16(scale, vis_read_hi(srcLoad)));   \
	acc1 =                                                             \
	vis_fpadd32(acc1, vis_fmuld8ulx16(scale,                           \
	    vis_read_lo(secondLoad)));                                     \
	acc2 =                                                             \
	vis_fpadd32(acc2, vis_fmuld8ulx16(scale, vis_read_lo(srcLoad)));   \
	acc2 =                                                             \
	vis_fpadd32(acc2, vis_fmuld8ulx16(scale,                           \
	    vis_read_hi(thirdLoad)));                                      \
	acc3 =                                                             \
	vis_fpadd32(acc3, vis_fmuld8ulx16(scale,                           \
	    vis_read_hi(secondLoad)));                                     \
	acc3 =                                                             \
	vis_fpadd32(acc3, vis_fmuld8ulx16(scale,                           \
	    vis_read_lo(thirdLoad)))

/* *********************************************************** */

#define	SUM3CH                                                  \
	firstAccumulator.db = acc1;                             \
	sum0 += firstAccumulator.two_int.int0;                  \
	sum1 += firstAccumulator.two_int.int1;                  \
	firstAccumulator.db = acc2;                             \
	sum2 += firstAccumulator.two_int.int0;                  \
	sum0 += firstAccumulator.two_int.int1;                  \
	firstAccumulator.db = acc3;                             \
	sum1 += firstAccumulator.two_int.int0;                  \
	sum2 += firstAccumulator.two_int.int1

/* *********************************************************** */

void
mlib_v_ImageMoment2_U16(
    const mlib_image *src,
    mlib_d64 *res)
{
/* pointer for pixel of source */
	mlib_u16 *srcPixelPtr = (mlib_u16 *)mlib_ImageGetData(src);

/* pointer for row of source */
	mlib_u8 *srcRowPtr = (mlib_u8 *)srcPixelPtr;

/* width of source image */
	mlib_s32 srcWidth = mlib_ImageGetWidth(src);

/* height of source image */
	mlib_s32 srcHeight = mlib_ImageGetHeight(src);

/* total number of pixels */
	mlib_s32 srcTotalPixNum = srcHeight * srcWidth;

/* linestride = bytes to next row */
	mlib_s32 srcYStride = mlib_ImageGetStride(src);

/* number of channels */
	mlib_s32 numOfChannels = mlib_ImageGetChannels(src);

/* indices */
	mlib_s32 i, j, k;

	mlib_d64 *srcLoadPtr;
	mlib_d64 srcLoad;
	mlib_d64 tmpArr[3];
	mlib_s32 lEdgeMask, rEdgeMask;
	mlib_s32 alignBytes;

	mlib_f32 scale = vis_to_float(0x10001);

	mlib_d64 sum0 = 0.0, sum1 = 0.0, sum2 = 0.0, sum3 = 0.0;
	mlib_d64 mtwo0 = 0.0, mtwo1 = 0.0, mtwo2 = 0.0, mtwo3 = 0.0;
	mlib_s32 loadsNumber, cycleNumber;

	mlib_d64 dr1, dr2, dr3, dr4, a1, a2;
	mlib_d64 acc1 = vis_fzero(), acc2 = vis_fzero();
	mlib_d64 rowMean[4], rowMTwo[4];
	mlib_s32 byteWidth, tailBytes;
	mlib_d64 divider = (mlib_d64)srcTotalPixNum;

/* compute mean3_off(align, sumn) */
	const mlib_u8 al2off[6] = { 0, 2, 1, 0, 2, 1 };
	type_union_mlib_d64 dc1, dc2;
	type_union_mlib_d64 firstAccumulator;
	type_union_mlib_d64 secondAccumulator;
	mlib_d64 mone = vis_fone();
	mlib_s32 compens;
	mlib_d64 offset4 = vis_to_double_dup(0x80008000);

	firstAccumulator.db = secondAccumulator.db = vis_fzero();
	tmpArr[0] = vis_fzero();

	for (i = 0; i < 4; i++)
		rowMean[i] = rowMTwo[i] = vis_fzero();

	if (numOfChannels != 3) {

		byteWidth = srcWidth << (numOfChannels >> 1);

		if (2 * byteWidth == srcYStride) {
			srcWidth = srcTotalPixNum;
			byteWidth = srcTotalPixNum << (numOfChannels >> 1);
			srcHeight = 1;
		}

		for (j = 0; j < srcHeight; j++) {

			tmpArr[0] = vis_fzero();
			alignBytes = (mlib_addr)srcPixelPtr & 7;
			tailBytes = byteWidth + (alignBytes >> 1);
			loadsNumber = tailBytes >> 2;
			tailBytes &= 3;

			lEdgeMask =
			    vis_edge16(srcPixelPtr,
			    srcPixelPtr + byteWidth - 1);
			rEdgeMask = ~(0xf >> tailBytes);

			srcLoadPtr =
			    (mlib_d64 *)((mlib_u8 *)srcPixelPtr - alignBytes);
			alignBytes >>= 1;

			srcLoad = (*srcLoadPtr++);
			srcLoad = vis_fxor(srcLoad, offset4);
			vis_pst_16(srcLoad, tmpArr, lEdgeMask);
			srcLoad = tmpArr[0];

			cycleNumber = 0;
			compens = (loadsNumber + 1) >> 1;

			while (loadsNumber > 0) {
				cycleNumber =
				    loadsNumber > 65536 ? 65536 : loadsNumber;
				loadsNumber -= cycleNumber;

				for (k = 0; k < (cycleNumber >> 1); k++) {

					dr1 =
					    vis_fmuld8sux16(vis_read_hi
					    (srcLoad), vis_read_hi(srcLoad));
					dr2 =
					    vis_fmuld8ulx16(vis_read_hi
					    (srcLoad), vis_read_hi(srcLoad));
					dr3 =
					    vis_fmuld8sux16(vis_read_lo
					    (srcLoad), vis_read_lo(srcLoad));
					dr4 =
					    vis_fmuld8ulx16(vis_read_lo
					    (srcLoad), vis_read_lo(srcLoad));
/* sub one */
					dr1 = vis_fpadd32(dr1, mone);
/* sub one */
					dr3 = vis_fpadd32(dr3, mone);
					dc1.db = vis_fpadd32(dr1, dr2);
					dc2.db = vis_fpadd32(dr3, dr4);

					a1 = vis_fmuld8ulx16(scale,
					    vis_read_hi(srcLoad));
					a2 = vis_fmuld8ulx16(scale,
					    vis_read_lo(srcLoad));
					srcLoad = (*srcLoadPtr++);
					srcLoad = vis_fxor(srcLoad, offset4);

					acc1 = vis_fpadd32(acc1, a1);
					acc2 = vis_fpadd32(acc2, a2);

					dr1 =
					    vis_fmuld8sux16(vis_read_hi
					    (srcLoad), vis_read_hi(srcLoad));
					dr2 =
					    vis_fmuld8ulx16(vis_read_hi
					    (srcLoad), vis_read_hi(srcLoad));
					dr3 =
					    vis_fmuld8sux16(vis_read_lo
					    (srcLoad), vis_read_lo(srcLoad));
					dr4 =
					    vis_fmuld8ulx16(vis_read_lo
					    (srcLoad), vis_read_lo(srcLoad));
					dc1.db = vis_fpadd32(dc1.db, dr1);
					dc2.db = vis_fpadd32(dc2.db, dr3);
					dc1.db = vis_fpadd32(dc1.db, dr2);
					dc2.db = vis_fpadd32(dc2.db, dr4);

					mtwo0 += (mlib_d64)dc1.two_int.int0;
					mtwo1 += (mlib_d64)dc1.two_int.int1;
					mtwo2 += (mlib_d64)dc2.two_int.int0;
					mtwo3 += (mlib_d64)dc2.two_int.int1;

					a1 = vis_fmuld8ulx16(scale,
					    vis_read_hi(srcLoad));
					a2 = vis_fmuld8ulx16(scale,
					    vis_read_lo(srcLoad));
					srcLoad = vis_ld_d64_nf(srcLoadPtr);
					srcLoadPtr++;
					srcLoad = vis_fxor(srcLoad, offset4);

					acc1 = vis_fpadd32(acc1, a1);
					acc2 = vis_fpadd32(acc2, a2);
				}

				firstAccumulator.db = acc1;
				secondAccumulator.db = acc2;

				sum0 += firstAccumulator.two_int.int0;
				sum1 += firstAccumulator.two_int.int1;
				sum2 += secondAccumulator.two_int.int0;
				sum3 += secondAccumulator.two_int.int1;

				firstAccumulator.db = secondAccumulator.db =
				    acc1 = acc2 = vis_fzero();
			}

			tmpArr[0] = dc1.db = dc2.db = vis_fzero();

			if (cycleNumber & 1) {

				dr1 =
				    vis_fmuld8sux16(vis_read_hi(srcLoad),
				    vis_read_hi(srcLoad));
				dr2 =
				    vis_fmuld8ulx16(vis_read_hi(srcLoad),
				    vis_read_hi(srcLoad));
				dr3 =
				    vis_fmuld8sux16(vis_read_lo(srcLoad),
				    vis_read_lo(srcLoad));
				dr4 =
				    vis_fmuld8ulx16(vis_read_lo(srcLoad),
				    vis_read_lo(srcLoad));
/* sub one */
				dr1 = vis_fpadd32(dr1, mone);
/* sub one */
				dr3 = vis_fpadd32(dr3, mone);
				dc1.db = vis_fpadd32(dr1, dr2);
				dc2.db = vis_fpadd32(dr3, dr4);

				a1 = vis_fmuld8ulx16(scale,
				    vis_read_hi(srcLoad));
				a2 = vis_fmuld8ulx16(scale,
				    vis_read_lo(srcLoad));
				srcLoad = vis_ld_d64_nf(srcLoadPtr);
				srcLoadPtr++;
				srcLoad = vis_fxor(srcLoad, offset4);

				acc1 = vis_fpadd32(acc1, a1);
				acc2 = vis_fpadd32(acc2, a2);
			}

			if (tailBytes) {
				vis_pst_16(srcLoad, tmpArr, rEdgeMask);
				srcLoad = tmpArr[0];

				dr1 =
				    vis_fmuld8sux16(vis_read_hi(srcLoad),
				    vis_read_hi(srcLoad));
				dr2 =
				    vis_fmuld8ulx16(vis_read_hi(srcLoad),
				    vis_read_hi(srcLoad));
				dr3 =
				    vis_fmuld8sux16(vis_read_lo(srcLoad),
				    vis_read_lo(srcLoad));
				dr4 =
				    vis_fmuld8ulx16(vis_read_lo(srcLoad),
				    vis_read_lo(srcLoad));
				dc1.db = vis_fpadd32(dc1.db, dr1);
				dc2.db = vis_fpadd32(dc2.db, dr3);
				dc1.db = vis_fpadd32(dc1.db, dr2);
				dc2.db = vis_fpadd32(dc2.db, dr4);

				a1 = vis_fmuld8ulx16(scale,
				    vis_read_hi(srcLoad));
				a2 = vis_fmuld8ulx16(scale,
				    vis_read_lo(srcLoad));
				acc1 = vis_fpadd32(acc1, a1);
				acc2 = vis_fpadd32(acc2, a2);
			}

			mtwo0 += (mlib_d64)dc1.two_int.int0;
			mtwo1 += (mlib_d64)dc1.two_int.int1;
			mtwo2 += (mlib_d64)dc2.two_int.int0;
			mtwo3 += (mlib_d64)dc2.two_int.int1;

			firstAccumulator.db = acc1;
			secondAccumulator.db = acc2;

			sum0 += firstAccumulator.two_int.int0;
			sum1 += firstAccumulator.two_int.int1;
			sum2 += secondAccumulator.two_int.int0;
			sum3 += secondAccumulator.two_int.int1;

			firstAccumulator.db = secondAccumulator.db = acc1 =
			    acc2 = vis_fzero();
			tmpArr[0] = vis_fzero();

			rowMTwo[3 & (0 - alignBytes)] +=
			    mtwo0 + (mlib_d64)compens;
			rowMTwo[3 & (1 - alignBytes)] +=
			    mtwo1 + (mlib_d64)compens;
			rowMTwo[3 & (2 - alignBytes)] +=
			    mtwo2 + (mlib_d64)compens;
			rowMTwo[(3 - alignBytes)] += mtwo3 + (mlib_d64)compens;
			rowMean[3 & (0 - alignBytes)] += sum0;
			rowMean[3 & (1 - alignBytes)] += sum1;
			rowMean[3 & (2 - alignBytes)] += sum2;
			rowMean[(3 - alignBytes)] += sum3;

			mtwo0 = mtwo1 = mtwo2 = mtwo3 = sum0 = sum1 = sum2 =
			    sum3 = vis_fzero();

			srcRowPtr += srcYStride;
			srcPixelPtr = (mlib_u16 *)srcRowPtr;
		}
	} else {
/* case 3: */

		mlib_d64 secondLoad, thirdLoad;
		mlib_d64 acc3;

		byteWidth = srcWidth * 3;

		if (2 * byteWidth == srcYStride) {
			srcWidth = srcTotalPixNum;
			byteWidth = srcTotalPixNum * 3;
			srcHeight = 1;
		}

		acc3 = vis_fzero();

		for (j = 0; j < srcHeight; j++) {
			alignBytes = (mlib_addr)srcPixelPtr & 7;
			tailBytes = byteWidth + (alignBytes >> 1);

			lEdgeMask =
			    vis_edge16(srcPixelPtr,
			    srcPixelPtr + byteWidth - 1);

			srcLoadPtr =
			    (mlib_d64 *)((mlib_u8 *)srcPixelPtr - alignBytes);
			srcLoad = (*srcLoadPtr++);
			secondLoad = vis_ld_d64_nf(srcLoadPtr);
			srcLoadPtr++;
			srcLoad = vis_fxor(srcLoad, offset4);
			secondLoad = vis_fxor(secondLoad, offset4);
			vis_pst_16(srcLoad, tmpArr, lEdgeMask);
			srcLoad = tmpArr[0];
			alignBytes >>= 1;
			loadsNumber = tailBytes / 12;
			tailBytes -= loadsNumber * 12;
			compens = loadsNumber << 1;

			for (; loadsNumber > 0; ) {
				cycleNumber =
				    loadsNumber > 0x8000 ? 0x8000 : loadsNumber;
				loadsNumber -= cycleNumber;

#pragma pipeloop(0)
				for (k = 0; k < cycleNumber; k++) {
					thirdLoad = (*srcLoadPtr++);
					thirdLoad =
					    vis_fxor(thirdLoad, offset4);

					MUL3CH;
					ACC3CH;
					srcLoad = vis_ld_d64_nf(srcLoadPtr);
					srcLoadPtr++;

					secondLoad = vis_ld_d64_nf(srcLoadPtr);
					srcLoadPtr++;
					srcLoad = vis_fxor(srcLoad, offset4);
					secondLoad =
					    vis_fxor(secondLoad, offset4);
				}

				SUM3CH;
				acc1 = acc2 = acc3 = vis_fzero();
			}

			tmpArr[0] = tmpArr[1] = tmpArr[2] = vis_fzero();

			if (tailBytes) {
				rEdgeMask = ~(0xfff >> tailBytes);
				thirdLoad = vis_ld_d64_nf(srcLoadPtr);
				srcLoadPtr++;
				thirdLoad = vis_fxor(thirdLoad, offset4);
				vis_pst_16(srcLoad, tmpArr, rEdgeMask >> 8);
				vis_pst_16(secondLoad, tmpArr + 1,
				    rEdgeMask >> 4);
				vis_pst_16(thirdLoad, tmpArr + 2, rEdgeMask);
				srcLoad = tmpArr[0];
				secondLoad = tmpArr[1];
				thirdLoad = tmpArr[2];

				MUL3CH;
				ACC3CH;
				SUM3CH;
				acc1 = acc2 = acc3 = vis_fzero();

				tmpArr[0] = vis_fzero();
				compens += 2;
			}

			rowMTwo[al2off[alignBytes + 0]] +=
			    mtwo0 + (mlib_d64)compens;
			rowMTwo[al2off[alignBytes + 2]] +=
			    mtwo1 + (mlib_d64)compens;
			rowMTwo[al2off[alignBytes + 1]] +=
			    mtwo2 + (mlib_d64)compens;
			rowMean[al2off[alignBytes + 0]] += sum0;
			rowMean[al2off[alignBytes + 2]] += sum1;
			rowMean[al2off[alignBytes + 1]] += sum2;
			mtwo0 = mtwo1 = mtwo2 = sum0 = sum1 = sum2 =
			    vis_fzero();

			srcRowPtr += srcYStride;
			srcPixelPtr = (mlib_u16 *)srcRowPtr;
		}
	}

	switch (numOfChannels) {
	case 1:
		sum0 =
		    (rowMean[0] + rowMean[1] + rowMean[2] +
		    rowMean[3]) / divider;
		mtwo0 =
		    (rowMTwo[0] + rowMTwo[1] + rowMTwo[2] +
		    rowMTwo[3]) / divider;
		res[0] =
		    mlib_fabs(mtwo0 + (sum0 + 0x8000) * 0x10000 -
		    (mlib_d64)0x40000000);
		break;

	case 2:
		sum0 = (rowMean[0] + rowMean[2]) / divider;
		mtwo0 = (rowMTwo[0] + rowMTwo[2]) / divider;
		res[0] =
		    mlib_fabs(mtwo0 + (sum0 + 0x8000) * 0x10000 -
		    (mlib_d64)0x40000000);
		sum1 = (rowMean[1] + rowMean[3]) / divider;
		mtwo1 = (rowMTwo[1] + rowMTwo[3]) / divider;
		res[1] =
		    mlib_fabs(mtwo1 + (sum1 + 0x8000) * 0x10000 -
		    (mlib_d64)0x40000000);
		break;

	case 3:
		sum0 = rowMean[0] / divider;
		sum1 = rowMean[1] / divider;
		sum2 = rowMean[2] / divider;
		mtwo0 = rowMTwo[0] / divider;
		mtwo1 = rowMTwo[1] / divider;
		mtwo2 = rowMTwo[2] / divider;
		res[0] =
		    mlib_fabs(mtwo0 + (sum0 + 0x8000) * 0x10000 -
		    (mlib_d64)0x40000000);
		res[1] =
		    mlib_fabs(mtwo1 + (sum1 + 0x8000) * 0x10000 -
		    (mlib_d64)0x40000000);
		res[2] =
		    mlib_fabs(mtwo2 + (sum2 + 0x8000) * 0x10000 -
		    (mlib_d64)0x40000000);
		break;

	default:	/* only case 4: */
		sum0 = rowMean[0] / divider;
		sum1 = rowMean[1] / divider;
		sum2 = rowMean[2] / divider;
		sum3 = rowMean[3] / divider;
		mtwo0 = rowMTwo[0] / divider;
		mtwo1 = rowMTwo[1] / divider;
		mtwo2 = rowMTwo[2] / divider;
		mtwo3 = rowMTwo[3] / divider;
		res[0] =
		    mlib_fabs(mtwo0 + (sum0 + 0x8000) * 0x10000 -
		    (mlib_d64)0x40000000);
		res[1] =
		    mlib_fabs(mtwo1 + (sum1 + 0x8000) * 0x10000 -
		    (mlib_d64)0x40000000);
		res[2] =
		    mlib_fabs(mtwo2 + (sum2 + 0x8000) * 0x10000 -
		    (mlib_d64)0x40000000);
		res[3] =
		    mlib_fabs(mtwo3 + (sum3 + 0x8000) * 0x10000 -
		    (mlib_d64)0x40000000);
		break;
	}
}

#if 0
{
/* pointer to the data of image */
	mlib_u16 *psrc = (mlib_u16 *)mlib_ImageGetData(img);

/* accumulators by channels */
	mlib_d64 c1, c2, c3, c4;

/* height of image */
	mlib_s32 height = mlib_ImageGetHeight(img);

/* elements to next row */
	mlib_s32 src_stride = mlib_ImageGetStride(img) >> 1;
	mlib_s32 src_chan = mlib_ImageGetChannels(img);

/* number of elements in the row */
	mlib_s32 size_row = mlib_ImageGetWidth(img) << (src_chan >> 1);

/* indices */
	mlib_s32 i, j;
	mlib_s32 ch1, ch2, ch3, ch4;
	mlib_u32 a1, a2;
	mlib_s32 align;

	res[0] = res[1] = res[2] = res[3] = c1 = c2 = c3 = c4 = 0.0;

	if (src_stride == size_row) {
/* This images is not a sub-images and can be treated as a 1-D vectors */
		size_row *= height;
		height = 1;
	}

	for (i = 0; i < height; i++) {
		j = 0;

		if (2 & (mlib_addr)psrc) {
			ch4 = ((mlib_u16 *)psrc)[0];
			c4 += ch4 * (mlib_d64)ch4;
			j = 1;
		}

		align = j;
		a1 = ((mlib_s32 *)(psrc + j))[0];
#pragma pipeloop(0)
		for (; j <= (size_row - 4); j += 4) {
			a2 = ((mlib_s32 *)(psrc + j))[1];
#ifdef _LITTLE_ENDIAN
			ch1 = a1 & 0xffff;
			ch2 = a1 >> 16;
			ch3 = a2 & 0xffff;
			ch4 = a2 >> 16;
#else /* _LITTLE_ENDIAN */
			ch1 = a1 >> 16;
			ch2 = a1 & 0xffff;
			ch3 = a2 >> 16;
			ch4 = a2 & 0xffff;
#endif /* _LITTLE_ENDIAN */
			a1 = ((mlib_s32 *)(psrc + j))[2];
			c1 += ch1 * (mlib_d64)ch1;
			c2 += ch2 * (mlib_d64)ch2;
			c3 += ch3 * (mlib_d64)ch3;
			c4 += ch4 * (mlib_d64)ch4;
		}

		if (size_row > j) {
#ifdef _LITTLE_ENDIAN
			ch1 = a1 & 0xffff;
			ch2 = a1 >> 16;
#else /* _LITTLE_ENDIAN */
			ch1 = a1 >> 16;
			ch2 = a1 & 0xffff;
#endif /* _LITTLE_ENDIAN */
			c1 += ch1 * (mlib_d64)ch1;
			j++;

			if (size_row > j) {
				c2 += ch2 * (mlib_d64)ch2;
				j++;

				if (size_row > j) {
					ch3 = ((mlib_u16 *)psrc)[j];
					c3 += ch3 * (mlib_d64)ch3;
				}
			}
		}

		res[0 + align] += c1;
		res[1 + align] += c2;
		res[2 + align] += c3;
		res[3 & (3 + align)] += c4;
		c1 = c2 = c3 = c4 = 0.0;
		psrc += src_stride;
	}
}

#endif /* 0 */

/* *********************************************************** */

#if 0
void
mlib_v_ImageMoment2_U16_3(
    const mlib_image *img,
    mlib_d64 *res)
{
/* pointer to the data of image */
	mlib_u16 *psrc = (mlib_u16 *)mlib_ImageGetData(img);

/* accumulators by channels */
	mlib_d64 c1, c2, c3;

/* height of image */
	mlib_s32 height = mlib_ImageGetHeight(img);

/* elements to next row */
	mlib_s32 src_stride = mlib_ImageGetStride(img) >> 1;

/* number of elements in the row */
	mlib_s32 size_row = mlib_ImageGetWidth(img) * 3;

/* indices */
	mlib_s32 i, j;
	mlib_u32 a1, a2, a3;
	mlib_s32 ch0, ch1, ch2;

	if (src_stride == size_row) {
/* This images is not a sub-images and can be treated as a 1-D vectors */
		size_row *= height;
		height = 1;
	}

	c1 = c2 = c3 = 0.0;
	for (i = 0; i < height; i++) {
		j = 0;

		if (2 & (mlib_addr)psrc) {
			ch0 = ((mlib_u16 *)psrc)[0];
			a3 = ((mlib_s32 *)(psrc + 1))[0];
#ifdef _LITTLE_ENDIAN
			ch1 = a3 & 0xffff;
			ch2 = a3 >> 16;
#else /* _LITTLE_ENDIAN */
			ch1 = a3 >> 16;
			ch2 = a3 & 0xffff;
#endif /* _LITTLE_ENDIAN */
			c1 += ch0 * (mlib_d64)ch0;
			c2 += ch1 * (mlib_d64)ch1;
			c3 += ch2 * (mlib_d64)ch2;
			j = 3;
		}

		a1 = ((mlib_s32 *)(psrc + j))[0];
#pragma pipeloop(0)
		for (; j <= (size_row - 6); j += 6) {
			a2 = ((mlib_s32 *)(psrc + j))[1];
			a3 = ((mlib_s32 *)(psrc + j))[2];
#ifdef _LITTLE_ENDIAN
			ch0 = a1 & 0xffff;
			ch1 = a1 >> 16;
			ch2 = a2 & 0xffff;
#else /* _LITTLE_ENDIAN */
			ch0 = a1 >> 16;
			ch1 = a1 & 0xffff;
			ch2 = a2 >> 16;
#endif /* _LITTLE_ENDIAN */
			c1 += ch0 * (mlib_d64)ch0;
			c2 += ch1 * (mlib_d64)ch1;
			c3 += ch2 * (mlib_d64)ch2;
			a1 = ((mlib_s32 *)(psrc + j))[3];

#ifdef _LITTLE_ENDIAN
			ch0 = a2 >> 16;
			ch1 = a3 & 0xffff;
			ch2 = a3 >> 16;
#else /* _LITTLE_ENDIAN */
			ch0 = a2 & 0xffff;
			ch1 = a3 >> 16;
			ch2 = a3 & 0xffff;
#endif /* _LITTLE_ENDIAN */
			c1 += ch0 * (mlib_d64)ch0;
			c2 += ch1 * (mlib_d64)ch1;
			c3 += ch2 * (mlib_d64)ch2;
		}

		if (j < size_row) {
			ch2 = ((mlib_u16 *)psrc)[j + 2];
#ifdef _LITTLE_ENDIAN
			ch0 = a1 & 0xffff;
			ch1 = a1 >> 16;
#else /* _LITTLE_ENDIAN */
			ch0 = a1 >> 16;
			ch1 = a1 & 0xffff;
#endif /* _LITTLE_ENDIAN */
			c1 += ch0 * (mlib_d64)ch0;
			c2 += ch1 * (mlib_d64)ch1;
			c3 += ch2 * (mlib_d64)ch2;
		}

		psrc += src_stride;
	}

	res[0] = c1;
	res[1] = c2;
	res[2] = c3;
}

#endif /* 0 */

/* *********************************************************** */

void
mlib_v_ImageMoment2_S16_124(
    const mlib_image *img,
    mlib_d64 *res)
{
/* pointer for pixel of source */
	mlib_s16 *srcPixelPtr = (mlib_s16 *)mlib_ImageGetData(img);

/* pointer for row of source */
	mlib_u8 *srcRowPtr = (mlib_u8 *)srcPixelPtr;

/* width of source image */
	mlib_s32 srcWidth = mlib_ImageGetWidth(img);

/* height of source image */
	mlib_s32 srcHeight = mlib_ImageGetHeight(img);

/* linestride = bytes to next row */
	mlib_s32 srcYStride = mlib_ImageGetStride(img);

/* number of channels */
	mlib_s32 numOfChannels = mlib_ImageGetChannels(img);

/* indices */
	mlib_s32 i, j, k;

	mlib_d64 *srcLoadPtr;
	mlib_d64 srcLoad;
	mlib_d64 tmpArr[1];
	mlib_s32 lEdgeMask, rEdgeMask;
	mlib_s32 alignBytes;
	mlib_d64 mtwo0 = 0.0, mtwo1 = 0.0, mtwo2 = 0.0, mtwo3 = 0.0;
	mlib_s32 cycleNumber;

	mlib_d64 dr1, dr2, dr3, dr4;
	mlib_s32 byteWidth, tailBytes;
	type_union_mlib_d64 dc1, dc2;
	mlib_d64 mone = vis_fone();

	for (i = 0; i < 4; i++)
		res[i] = vis_fzero();

	byteWidth = srcWidth << (numOfChannels >> 1);

	if (2 * byteWidth == srcYStride) {
		srcWidth *= srcHeight;
		byteWidth = srcWidth << (numOfChannels >> 1);
		srcHeight = 1;
	}

	for (j = 0; j < srcHeight; j++) {

		tmpArr[0] = vis_fzero();
		alignBytes = (mlib_addr)srcPixelPtr & 7;
		tailBytes = byteWidth + (alignBytes >> 1);
		cycleNumber = tailBytes >> 2;
		tailBytes &= 3;

		lEdgeMask =
		    vis_edge16(srcPixelPtr, srcPixelPtr + byteWidth - 1);
		rEdgeMask = ~(0xf >> tailBytes);

		srcLoadPtr = (mlib_d64 *)((mlib_u8 *)srcPixelPtr - alignBytes);
		alignBytes >>= 1;

		srcLoad = (*srcLoadPtr++);
		vis_pst_16(srcLoad, tmpArr, lEdgeMask);
		srcLoad = tmpArr[0];

		for (k = 0; k < (cycleNumber >> 1); k++) {

			dr1 =
			    vis_fmuld8sux16(vis_read_hi(srcLoad),
			    vis_read_hi(srcLoad));
			dr2 =
			    vis_fmuld8ulx16(vis_read_hi(srcLoad),
			    vis_read_hi(srcLoad));
			dr3 =
			    vis_fmuld8sux16(vis_read_lo(srcLoad),
			    vis_read_lo(srcLoad));
			dr4 =
			    vis_fmuld8ulx16(vis_read_lo(srcLoad),
			    vis_read_lo(srcLoad));
			srcLoad = (*srcLoadPtr++);
/* sub one */
			dr1 = vis_fpadd32(dr1, mone);
/* sub one */
			dr3 = vis_fpadd32(dr3, mone);
			dc1.db = vis_fpadd32(dr1, dr2);
			dc2.db = vis_fpadd32(dr3, dr4);

			dr1 =
			    vis_fmuld8sux16(vis_read_hi(srcLoad),
			    vis_read_hi(srcLoad));
			dr2 =
			    vis_fmuld8ulx16(vis_read_hi(srcLoad),
			    vis_read_hi(srcLoad));
			dr3 =
			    vis_fmuld8sux16(vis_read_lo(srcLoad),
			    vis_read_lo(srcLoad));
			dr4 =
			    vis_fmuld8ulx16(vis_read_lo(srcLoad),
			    vis_read_lo(srcLoad));
			srcLoad = vis_ld_d64_nf(srcLoadPtr);
			srcLoadPtr++;
			dc1.db = vis_fpadd32(dc1.db, dr1);
			dc2.db = vis_fpadd32(dc2.db, dr3);
			dc1.db = vis_fpadd32(dc1.db, dr2);
			dc2.db = vis_fpadd32(dc2.db, dr4);

			mtwo0 += (mlib_d64)dc1.two_int.int0;
			mtwo1 += (mlib_d64)dc1.two_int.int1;
			mtwo2 += (mlib_d64)dc2.two_int.int0;
			mtwo3 += (mlib_d64)dc2.two_int.int1;
		}

		tmpArr[0] = dc1.db = dc2.db = vis_fzero();

		if (cycleNumber & 1) {

			dr1 =
			    vis_fmuld8sux16(vis_read_hi(srcLoad),
			    vis_read_hi(srcLoad));
			dr2 =
			    vis_fmuld8ulx16(vis_read_hi(srcLoad),
			    vis_read_hi(srcLoad));
			dr3 =
			    vis_fmuld8sux16(vis_read_lo(srcLoad),
			    vis_read_lo(srcLoad));
			dr4 =
			    vis_fmuld8ulx16(vis_read_lo(srcLoad),
			    vis_read_lo(srcLoad));
			srcLoad = vis_ld_d64_nf(srcLoadPtr);
			srcLoadPtr++;
/* sub one */
			dr1 = vis_fpadd32(dr1, mone);
/* sub one */
			dr3 = vis_fpadd32(dr3, mone);
			dc1.db = vis_fpadd32(dr1, dr2);
			dc2.db = vis_fpadd32(dr3, dr4);
		}

		if (tailBytes) {
			vis_pst_16(srcLoad, tmpArr, rEdgeMask);
			srcLoad = tmpArr[0];

			dr1 =
			    vis_fmuld8sux16(vis_read_hi(srcLoad),
			    vis_read_hi(srcLoad));
			dr2 =
			    vis_fmuld8ulx16(vis_read_hi(srcLoad),
			    vis_read_hi(srcLoad));
			dr3 =
			    vis_fmuld8sux16(vis_read_lo(srcLoad),
			    vis_read_lo(srcLoad));
			dr4 =
			    vis_fmuld8ulx16(vis_read_lo(srcLoad),
			    vis_read_lo(srcLoad));
			dc1.db = vis_fpadd32(dc1.db, dr1);
			dc2.db = vis_fpadd32(dc2.db, dr3);
			dc1.db = vis_fpadd32(dc1.db, dr2);
			dc2.db = vis_fpadd32(dc2.db, dr4);
		}

		mtwo0 += (mlib_d64)dc1.two_int.int0;
		mtwo1 += (mlib_d64)dc1.two_int.int1;
		mtwo2 += (mlib_d64)dc2.two_int.int0;
		mtwo3 += (mlib_d64)dc2.two_int.int1;

		tmpArr[0] = vis_fzero();

		res[3 & (0 - alignBytes)] +=
		    mtwo0 + (mlib_d64)((cycleNumber + 1) >> 1);
		res[3 & (1 - alignBytes)] +=
		    mtwo1 + (mlib_d64)((cycleNumber + 1) >> 1);
		res[3 & (2 - alignBytes)] +=
		    mtwo2 + (mlib_d64)((cycleNumber + 1) >> 1);
		res[(3 - alignBytes)] +=
		    mtwo3 + (mlib_d64)((cycleNumber + 1) >> 1);

		mtwo0 = mtwo1 = mtwo2 = mtwo3 = vis_fzero();

		srcRowPtr += srcYStride;
		srcPixelPtr = (mlib_s16 *)srcRowPtr;
	}
}

/* *********************************************************** */

#define	MUL3CH                                                             \
	dr1 =                                                              \
	vis_fmuld8sux16(vis_read_hi(srcLoad), vis_read_hi(srcLoad));       \
	dr2 =                                                              \
	vis_fmuld8ulx16(vis_read_hi(srcLoad), vis_read_hi(srcLoad));       \
	dr3 =                                                              \
	vis_fmuld8sux16(vis_read_lo(secondLoad),                           \
	    vis_read_lo(secondLoad));                                      \
	dr4 =                                                              \
	vis_fmuld8ulx16(vis_read_lo(secondLoad),                           \
	    vis_read_lo(secondLoad));                                      \
	dr1 = vis_fpadd32(dr1, mone);                                      \
/*                                                                         \
 * sub one                                                                 \
 */                                                                        \
	dr2 = vis_fpadd32(dr1, dr2);                                       \
	dr3 = vis_fpadd32(dr2, dr3);                                       \
	dc1.db = vis_fpadd32(dr3, dr4);                                    \
	mtwo0 += (mlib_d64)dc1.two_int.int0;                               \
	mtwo1 += (mlib_d64)dc1.two_int.int1;                               \
	dr1 =                                                              \
	vis_fmuld8sux16(vis_read_hi(secondLoad),                           \
	    vis_read_hi(secondLoad));                                      \
	dr2 =                                                              \
	vis_fmuld8ulx16(vis_read_hi(secondLoad),                           \
	    vis_read_hi(secondLoad));                                      \
	dr3 =                                                              \
	vis_fmuld8sux16(vis_read_lo(thirdLoad), vis_read_lo(thirdLoad));   \
	dr4 =                                                              \
	vis_fmuld8ulx16(vis_read_lo(thirdLoad), vis_read_lo(thirdLoad));   \
	dr1 = vis_fpadd32(dr1, mone);                                      \
/*                                                                         \
 * sub one                                                                 \
 */                                                                        \
	dr2 = vis_fpadd32(dr1, dr2);                                       \
	dr3 = vis_fpadd32(dr2, dr3);                                       \
	dc1.db = vis_fpadd32(dr3, dr4);                                    \
	mtwo1 += (mlib_d64)dc1.two_int.int0;                               \
	mtwo2 += (mlib_d64)dc1.two_int.int1;                               \
	dr1 =                                                              \
	vis_fmuld8sux16(vis_read_hi(thirdLoad), vis_read_hi(thirdLoad));   \
	dr2 =                                                              \
	vis_fmuld8ulx16(vis_read_hi(thirdLoad), vis_read_hi(thirdLoad));   \
	dr3 =                                                              \
	vis_fmuld8sux16(vis_read_lo(srcLoad), vis_read_lo(srcLoad));       \
	dr4 =                                                              \
	vis_fmuld8ulx16(vis_read_lo(srcLoad), vis_read_lo(srcLoad));       \
	dr1 = vis_fpadd32(dr1, mone);                                      \
/*                                                                         \
 * sub one                                                                 \
 */                                                                        \
	dr2 = vis_fpadd32(dr1, dr2);                                       \
	dr3 = vis_fpadd32(dr2, dr3);                                       \
	dc1.db = vis_fpadd32(dr3, dr4);                                    \
	mtwo2 += (mlib_d64)dc1.two_int.int0;                               \
	mtwo0 += (mlib_d64)dc1.two_int.int1

/* *********************************************************** */

void
mlib_v_ImageMoment2_S16_3(
    const mlib_image *img,
    mlib_d64 *res)
{
/* pointer for pixel of source */
	mlib_s16 *srcPixelPtr = (mlib_s16 *)mlib_ImageGetData(img);

/* pointer for row of source */
	mlib_u8 *srcRowPtr = (mlib_u8 *)srcPixelPtr;

/* width of source image */
	mlib_s32 srcWidth = mlib_ImageGetWidth(img);

/* height of source image */
	mlib_s32 srcHeight = mlib_ImageGetHeight(img);

/* linestride = bytes to next row */
	mlib_s32 srcYStride = mlib_ImageGetStride(img);

/* indices */
	mlib_s32 i, j;

	mlib_d64 *srcLoadPtr;
	mlib_d64 srcLoad, secondLoad, thirdLoad;
	mlib_d64 tmpArr[3];
	mlib_s32 lEdgeMask, rEdgeMask;
	mlib_s32 alignBytes;
	mlib_d64 mtwo0 = 0.0, mtwo1 = 0.0, mtwo2 = 0.0;
	mlib_d64 dr1, dr2, dr3, dr4;
	mlib_s32 loadsNumber;
	mlib_s32 byteWidth;

/* compute mean3_off(align, sumn) */
	const mlib_u8 al2off[6] = { 0, 2, 1, 0, 2, 1 };
	type_union_mlib_d64 dc1;
	mlib_d64 mone = vis_fone();
	mlib_s32 compens;

	for (i = 0; i < 3; i++)
		res[i] = vis_fzero();

	byteWidth = srcWidth * 3;

	if (2 * byteWidth == srcYStride) {
		srcWidth *= srcHeight;
		byteWidth = srcWidth * 3;
		srcHeight = 1;
	}

	for (j = 0; j < srcHeight; j++) {
		tmpArr[0] = vis_fzero();
		alignBytes = (mlib_addr)srcPixelPtr & 7;
		loadsNumber = byteWidth + (alignBytes >> 1);

		lEdgeMask =
		    vis_edge16(srcPixelPtr, srcPixelPtr + byteWidth - 1);

		srcLoadPtr = (mlib_d64 *)((mlib_u8 *)srcPixelPtr - alignBytes);
		srcLoad = (*srcLoadPtr++);
		secondLoad = vis_ld_d64_nf(srcLoadPtr);
		srcLoadPtr++;
		vis_pst_16(srcLoad, tmpArr, lEdgeMask);
		srcLoad = tmpArr[0];
		alignBytes >>= 1;
		compens = 0;

#pragma pipeloop(0)
		for (; loadsNumber >= 24; loadsNumber -= 12) {
			thirdLoad = vis_ld_d64_nf(srcLoadPtr);
			srcLoadPtr++;

			MUL3CH;
			srcLoad = (*srcLoadPtr++);

			secondLoad = (*srcLoadPtr++);
			compens += 2;
		}

		if (loadsNumber >= 12) {
			thirdLoad = vis_ld_d64_nf(srcLoadPtr);
			srcLoadPtr++;

			MUL3CH;
			srcLoad = vis_ld_d64_nf(srcLoadPtr);
			srcLoadPtr++;

			secondLoad = vis_ld_d64_nf(srcLoadPtr);
			srcLoadPtr++;
			compens += 2;
			loadsNumber -= 12;
		}

		tmpArr[0] = tmpArr[1] = tmpArr[2] = vis_fzero();

		if (loadsNumber) {
			rEdgeMask = ~(0xfff >> loadsNumber);
			thirdLoad = vis_ld_d64_nf(srcLoadPtr);
			vis_pst_16(srcLoad, tmpArr, rEdgeMask >> 8);
			vis_pst_16(secondLoad, tmpArr + 1, rEdgeMask >> 4);
			vis_pst_16(thirdLoad, tmpArr + 2, rEdgeMask);
			srcLoad = tmpArr[0];
			secondLoad = tmpArr[1];
			thirdLoad = tmpArr[2];

			MUL3CH;
			tmpArr[0] = vis_fzero();

			compens += 2;
		}

		res[al2off[alignBytes + 0]] += mtwo0 + (mlib_d64)compens;
		res[al2off[alignBytes + 2]] += mtwo1 + (mlib_d64)compens;
		res[al2off[alignBytes + 1]] += mtwo2 + (mlib_d64)compens;
		mtwo0 = mtwo1 = mtwo2 = vis_fzero();

		srcRowPtr += srcYStride;
		srcPixelPtr = (mlib_s16 *)srcRowPtr;
	}
}

/* *********************************************************** */

#define	MUL_S32(x)	((x) * (mlib_d64) (x))

/* *********************************************************** */

void
mlib_v_ImageMoment2_S32_124(
    const mlib_image *img,
    mlib_d64 *res)
{
/* pointer to the data of image */
	mlib_s32 *psrc = (mlib_s32 *)mlib_ImageGetData(img);

/* accumulators by channels */
	mlib_d64 c1, c2, c3, c4;

/* height of image */
	mlib_s32 height = mlib_ImageGetHeight(img);

/* elements to next row */
	mlib_s32 src_stride = mlib_ImageGetStride(img) >> 2;
	mlib_s32 src_chan = mlib_ImageGetChannels(img);

/* number of elements in the row */
	mlib_s32 size_row = mlib_ImageGetWidth(img) << (src_chan >> 1);

/* indices */
	mlib_s32 i, j, k;

	res[0] = res[1] = res[2] = c1 = c2 = c3 = c4 = 0.0;

	if (src_stride == size_row) {
/* This images is not a sub-images and can be treated as a 1-D vectors */
		size_row *= height;
		height = 1;
	}

	for (i = 0; i < height; i++) {
		for (j = 0; j <= (size_row - 4); j += 4) {
			c1 += MUL_S32(psrc[j]);
			c2 += MUL_S32(psrc[j + 1]);
			c3 += MUL_S32(psrc[j + 2]);
			c4 += MUL_S32(psrc[j + 3]);
		}

		for (k = 0; j < size_row; j++, k++)
			res[k] += MUL_S32(psrc[j]);
		psrc += src_stride;
	}

	res[0] += c1;
	res[1] += c2;
	res[2] += c3;
	res[3] = c4;
}

/* *********************************************************** */

void
mlib_v_ImageMoment2_S32_3(
    const mlib_image *img,
    mlib_d64 *res)
{
/* pointer to the data of image */
	mlib_s32 *psrc = (mlib_s32 *)mlib_ImageGetData(img);

/* accumulators by channels */
	mlib_d64 c1, c2, c3;

/* height of image */
	mlib_s32 height = mlib_ImageGetHeight(img);

/* elements to next row */
	mlib_s32 src_stride = mlib_ImageGetStride(img) >> 2;

/* number of elements in the row */
	mlib_s32 size_row = mlib_ImageGetWidth(img) * 3;

/* indices */
	mlib_s32 i, j;

	c1 = c2 = c3 = 0.0;

	if (src_stride == size_row) {
/* This images is not a sub-images and can be treated as a 1-D vectors */
		size_row *= height;
		height = 1;
	}

	for (i = 0; i < height; i++) {
		for (j = 0; j <= (size_row - 3); j += 3) {
			c1 += MUL_S32(psrc[j]);
			c2 += MUL_S32(psrc[j + 1]);
			c3 += MUL_S32(psrc[j + 2]);
		}

		psrc += src_stride;
	}

	res[0] = c1;
	res[1] = c2;
	res[2] = c3;
}

/* *********************************************************** */
