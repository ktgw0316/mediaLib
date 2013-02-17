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

#pragma ident	"@(#)mlib_v_ImageYProj.c	9.3	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_ImageYProj - calculates Y Projections for input image
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageYProj(mlib_d64         *yproj,
 *                                  const mlib_image *src)
 *
 * ARGUMENTS
 *      yproj       Pointer to Y - projection vector
 *      src         Pointer to an image
 *
 * RESTRICTION
 *      The image must be a single channel image.
 *      It can be of MLIB_BYTE, MLIB_SHORT, MLIB_USHORT or MLIB_INT data type.
 *
 * DESCRIPTION
 *                 w-1
 *      yproj[j] = SUM (src[i][j]);
 *                 i = 0
 */

#include <vis_proto.h>
#include <mlib_image.h>
#include <mlib_ImageCheck.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageYProj = __mlib_ImageYProj

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_ImageYProj) mlib_ImageYProj
    __attribute__((weak, alias("__mlib_ImageYProj")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

static void mlib_v_ImageYProj_U8(
    const mlib_image *src,
    mlib_d64 *yproj);
static void mlib_v_ImageYProj_U16(
    const mlib_image *src,
    mlib_d64 *yproj);
static void mlib_v_ImageYProj_S16(
    const mlib_image *src,
    mlib_d64 *yproj);
static void mlib_v_ImageYProj_S32(
    const mlib_image *src,
    mlib_d64 *yproj);

/* *********************************************************** */

mlib_status
__mlib_ImageYProj(
    mlib_d64 *yproj,
    const mlib_image *src)
{
/* check for obvious errors */
	MLIB_IMAGE_CHECK(src);
	MLIB_IMAGE_HAVE_CHAN(src, 1);

	if (yproj == NULL)
		return (MLIB_NULLPOINTER);

	switch (mlib_ImageGetType(src)) {
/* handle MLIB_BYTE data type of image */
	case MLIB_BYTE:
		mlib_v_ImageYProj_U8(src, yproj);
		break;

/* handle MLIB_USHORT data type of image */
	case MLIB_USHORT:
		mlib_v_ImageYProj_U16(src, yproj);
		break;

/* handle MLIB_SHORT data type of image */
	case MLIB_SHORT:
		mlib_v_ImageYProj_S16(src, yproj);
		break;

/* handle MLIB_INT data type of image */
	case MLIB_INT:
		mlib_v_ImageYProj_S32(src, yproj);
		break;

/* discard any other data types */
	default:
		return (MLIB_FAILURE);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#define	Y_MAIN_CYCLE_U8                                         \
	{                                                       \
	    mlib_d64 dxh, dxl;                                  \
	                                                        \
	    dxh = vis_fmul8x16al(vis_read_hi(dx), scale);       \
	    acc1 = vis_fpadd16(acc1, dxh);                      \
	    dxl = vis_fmul8x16al(vis_read_lo(dx), scale);       \
	    acc2 = vis_fpadd16(acc2, dxl);                      \
	}

/* *********************************************************** */

#define	Y_MUL_U8                                                \
	acc1 = vis_fmul8x16al(vis_read_hi(dx), scale);          \
	acc2 = vis_fmul8x16al(vis_read_lo(dx), scale)

/* *********************************************************** */

#define	Y_TAIL_U8                                               \
	res2 = vis_fand(acc1, mask);                            \
	acc1 = vis_faligndata(acc1, acc1);                      \
	res1 = vis_fand(acc1, mask);                            \
	dsum = vis_fpadd32(dsum, res1);                         \
	dsum = vis_fpadd32(dsum, res2);                         \
	res2 = vis_fand(acc2, mask);                            \
	acc2 = vis_faligndata(acc2, acc2);                      \
	res1 = vis_fand(acc2, mask);                            \
	dsum = vis_fpadd32(dsum, res1);                         \
	dsum = vis_fpadd32(dsum, res2)

/* *********************************************************** */

void
mlib_v_ImageYProj_U8(
    const mlib_image *src,
    mlib_d64 *yproj)
{
/* width of source image */
	mlib_s32 Width = mlib_ImageGetWidth(src);

/* height of source image */
	mlib_s32 Height = mlib_ImageGetHeight(src);

/* linestride = bytes to next row */
	mlib_s32 YStride = mlib_ImageGetStride(src);

/* pointers to the data of image */
	mlib_u8 *px = (mlib_u8 *)mlib_ImageGetData(src);
	mlib_u8 *pxend;

	mlib_d64 *dpx;
	mlib_d64 *dpxend;

	mlib_s32 proj[1];
	mlib_f32 scale = vis_to_float((mlib_u32)256);

	mlib_d64 dx;
	mlib_d64 midd[2];
	mlib_d64 mask = vis_to_double_dup(0xffff);

	mlib_d64 dsum;

	mlib_d64 acc1, acc2;
	mlib_d64 res1, res2;

/* number of pixel to process */
	mlib_s32 size = Width;

/* number of 32-bit loads */
	mlib_s32 loadsNumber;

/* number of cycles 256 * 32-bit loads */
	mlib_s32 cycleNumber;

	mlib_s32 emask;
	mlib_s32 j;

	(void *)vis_alignaddr(0, 6);
	midd[0] = midd[1] = vis_fzero();
	for (j = 0; j < Height; j++) {

		dsum = vis_fzero();

		pxend = px + size - 1;
		dpx = (mlib_d64 *)((mlib_addr)px & (~7));
		dpxend = (mlib_d64 *)((mlib_addr)pxend & (~7));
		dx = dpx[0];
		emask = vis_edge8(px, pxend);
		vis_pst_8(dx, midd, emask);
		dx = midd[0];

		loadsNumber = dpxend - dpx;

		if (loadsNumber <= 0) {
			Y_MUL_U8;
			Y_TAIL_U8;
			dpx++;
		}

		while (loadsNumber > 0) {
			cycleNumber = loadsNumber;

			if (cycleNumber > 256)
				cycleNumber = 256;
			acc1 = acc2 = vis_fzero();
			loadsNumber -= cycleNumber;
			for (; cycleNumber > 0; cycleNumber--) {
				Y_MAIN_CYCLE_U8;
				dpx++;
				dx = dpx[0];
			}

			Y_TAIL_U8;
		}

		if ((mlib_addr)dpx <= (mlib_addr)pxend) {
			dx = dpx[0];
			emask = vis_edge8(dpx, pxend);
			vis_pst_8(dx, midd + 1, emask);
			dx = midd[1];
			Y_MUL_U8;
			Y_TAIL_U8;
		}

		((mlib_f32 *)proj)[0] =
		    vis_fpadd32s(vis_read_hi(dsum), vis_read_lo(dsum));
		yproj[j] = (mlib_d64)proj[0];

		midd[0] = midd[1] = vis_fzero();
		px += YStride;
	}
}

/* *********************************************************** */

#define	Y_MAIN_CYCLE_16                                         \
	{                                                       \
	    mlib_d64 dxh, dxl;                                  \
	                                                        \
	    dxh = vis_fmuld8ulx16(scale, vis_read_hi(dx));      \
	    acc1 = vis_fpadd32(acc1, dxh);                      \
	    dxl = vis_fmuld8ulx16(scale, vis_read_lo(dx));      \
	    acc2 = vis_fpadd32(acc2, dxl);                      \
	}

/* *********************************************************** */

void
mlib_v_ImageYProj_U16(
    const mlib_image *src,
    mlib_d64 *yproj)
{
/* width of source image */
	mlib_s32 Width = mlib_ImageGetWidth(src);

/* height of source image */
	mlib_s32 Height = mlib_ImageGetHeight(src);

/* linestride = bytes to next row */
	mlib_s32 YStride = (mlib_ImageGetStride(src)) / 2;

/* pointers to the data of image */
	mlib_u16 *px = (mlib_u16 *)mlib_ImageGetData(src);
	mlib_u16 *pxend;

	mlib_d64 *dpx;
	mlib_d64 *dpxend;

	mlib_s32 proj[1];
	mlib_s32 patch_s16_2_u16 = Width << 15;
	mlib_d64 xorMask = vis_to_double_dup(0x80008000);
	mlib_f32 scale = vis_to_float((mlib_u32)0x10001);

	mlib_d64 dx;
	mlib_d64 midd[2];

	mlib_d64 acc1, acc2;

	mlib_s32 emask;
	mlib_s32 j, i;
	mlib_s32 size = Width;

	midd[0] = midd[1] = vis_fzero();
	for (j = 0; j < Height; j++) {

		acc1 = vis_fzero();
		acc2 = vis_fzero();

		pxend = px + size - 1;
		dpx = (mlib_d64 *)((mlib_addr)px & (~7));
		dpxend = (mlib_d64 *)((mlib_addr)pxend & (~7));
		dx = dpx[0];
		dx = vis_fxor(dx, xorMask);
		emask = vis_edge16(px, pxend);
		vis_pst_16(dx, midd, emask);
		dx = midd[0];
		Y_MAIN_CYCLE_16;
		dpx++;

		i = dpxend - dpx;
		for (; i > 0; i--) {
			dx = dpx[0];
			dx = vis_fxor(dx, xorMask);
			Y_MAIN_CYCLE_16;
			dpx++;
		}

		if ((mlib_addr)dpx <= (mlib_addr)pxend) {
			dx = dpx[0];
			dx = vis_fxor(dx, xorMask);
			emask = vis_edge16(dpx, pxend);
			vis_pst_16(dx, midd + 1, emask);
			dx = midd[1];
			Y_MAIN_CYCLE_16;
		}

		acc1 = vis_fpadd32(acc1, acc2);
		((mlib_f32 *)proj)[0] =
		    vis_fpadd32s(vis_read_hi(acc1), vis_read_lo(acc1));
		yproj[j] = (mlib_d64)proj[0] + patch_s16_2_u16;
		px += YStride;
		midd[0] = midd[1] = vis_fzero();
	}
}

/* *********************************************************** */

void
mlib_v_ImageYProj_S16(
    const mlib_image *src,
    mlib_d64 *yproj)
{
/* width of source image */
	mlib_s32 Width = mlib_ImageGetWidth(src);

/* height of source image */
	mlib_s32 Height = mlib_ImageGetHeight(src);

/* linestride = bytes to next row */
	mlib_s32 YStride = (mlib_ImageGetStride(src)) / 2;

/* pointers to the data of image */
	mlib_s16 *px = (mlib_s16 *)mlib_ImageGetData(src);
	mlib_s16 *pxend;

	mlib_d64 *dpx;
	mlib_d64 *dpxend;

	mlib_s32 proj[1];
	mlib_f32 scale = vis_to_float((mlib_u32)0x10001);

	mlib_d64 dx;
	mlib_d64 midd[2];

	mlib_d64 acc1, acc2;

	mlib_s32 emask;
	mlib_s32 j, i;
	mlib_s32 size = Width;

	midd[0] = midd[1] = vis_fzero();
	for (j = 0; j < Height; j++) {

		acc1 = vis_fzero();
		acc2 = vis_fzero();

		pxend = px + size - 1;
		dpx = (mlib_d64 *)((mlib_addr)px & (~7));
		dpxend = (mlib_d64 *)((mlib_addr)pxend & (~7));
		dx = dpx[0];
		emask = vis_edge16(px, pxend);
		vis_pst_16(dx, midd, emask);
		dx = midd[0];
		Y_MAIN_CYCLE_16;
		dpx++;

		i = dpxend - dpx;
		for (; i > 0; i--) {
			dx = dpx[0];
			Y_MAIN_CYCLE_16;
			dpx++;
		}

		if ((mlib_addr)dpx <= (mlib_addr)pxend) {
			dx = dpx[0];
			emask = vis_edge16(dpx, pxend);
			vis_pst_16(dx, midd + 1, emask);
			dx = midd[1];
			Y_MAIN_CYCLE_16;
		}

		acc1 = vis_fpadd32(acc1, acc2);
		((mlib_f32 *)proj)[0] =
		    vis_fpadd32s(vis_read_hi(acc1), vis_read_lo(acc1));
		yproj[j] = (mlib_d64)proj[0];
		px += YStride;
		midd[0] = midd[1] = vis_fzero();
	}
}

/* *********************************************************** */

void
mlib_v_ImageYProj_S32(
    const mlib_image *src,
    mlib_d64 *yproj)
{
/* width of source image */
	mlib_s32 Width = mlib_ImageGetWidth(src);

/* height of source image */
	mlib_s32 Height = mlib_ImageGetHeight(src);

/* linestride = bytes to next row */
	mlib_s32 YStride = (mlib_ImageGetStride(src)) / 4;

/* pointers to the data of image */
	mlib_s32 *LinePntr = (mlib_s32 *)mlib_ImageGetData(src);
	mlib_s32 *PixelPntr = (mlib_s32 *)mlib_ImageGetData(src);

	mlib_d64 Accum1;
	mlib_s32 i, j;

	for (j = 0; j < Height; j++) {
		Accum1 = 0;

		for (i = 0; i < Width; i++)
			Accum1 += (*PixelPntr++);
		yproj[j] = (mlib_d64)Accum1;

		LinePntr += YStride;
		PixelPntr = LinePntr;
	}
}

/* *********************************************************** */
