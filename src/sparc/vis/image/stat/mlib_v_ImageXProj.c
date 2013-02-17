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

#pragma ident	"@(#)mlib_v_ImageXProj.c	9.2	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_ImageXProj - calculates X Projections for input image
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageXProj(mlib_d64         *xproj,
 *                                  const mlib_image *src)
 *
 * ARGUMENTS
 *      xproj       Pointer to X - projection vector
 *      src         Pointer to an image
 *
 * RESTRICTION
 *      The image must be a single channel image.
 *      It can be of MLIB_BYTE, MLIB_SHORT, MLIB_USHORT or MLIB_INT data type.
 *
 * DESCRIPTION
 *                 h-1
 *      xproj[i] = SUM (src[i][j]);
 *                 j = 0
 */

#include <stdlib.h>
#include <mlib_image.h>
#include <vis_proto.h>
#include <mlib_ImageCheck.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageXProj = __mlib_ImageXProj

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_ImageXProj) mlib_ImageXProj
    __attribute__((weak, alias("__mlib_ImageXProj")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

static void mlib_v_ImageXProj_U8(
    const mlib_image *src,
    mlib_d64 *xproj);

static void mlib_v_ImageXProj_U16(
    const mlib_image *src,
    mlib_d64 *xproj);

static void mlib_v_ImageXProj_S16(
    const mlib_image *src,
    mlib_d64 *xproj);

static void mlib_v_ImageXProj_S32(
    const mlib_image *src,
    mlib_d64 *xproj);

/* *********************************************************** */

mlib_status
__mlib_ImageXProj(
    mlib_d64 *xproj,
    const mlib_image *src)
{
/* check for obvious errors */
	MLIB_IMAGE_CHECK(src);
	MLIB_IMAGE_HAVE_CHAN(src, 1);

	if (xproj == NULL)
		return (MLIB_NULLPOINTER);

	switch (mlib_ImageGetType(src)) {
/* handle MLIB_BYTE data type of image */
	case MLIB_BYTE:
		mlib_v_ImageXProj_U8(src, xproj);
		break;

/* handle MLIB_USHORT data type of image */
	case MLIB_USHORT:
		mlib_v_ImageXProj_U16(src, xproj);
		break;

/* handle MLIB_SHORT data type of image */
	case MLIB_SHORT:
		mlib_v_ImageXProj_S16(src, xproj);
		break;

/* handle MLIB_INT data type of image */
	case MLIB_INT:
		mlib_v_ImageXProj_S32(src, xproj);
		break;

/* discard any other data types */
	default:
		return (MLIB_FAILURE);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#define	X_MAIN_CYCLE_U8                                         \
	{                                                       \
	    mlib_d64 dxh, dxl;                                  \
	                                                        \
	    dxh = vis_fmul8x16al(vis_read_hi(Load), scale);     \
	    accm0 = vis_fpadd16(accm0, dxh);                    \
	    dxl = vis_fmul8x16al(vis_read_lo(Load), scale);     \
	    accm1 = vis_fpadd16(accm1, dxl);                    \
	}

/* *********************************************************** */

#define	X_EXPAND_ACC_U8                                         \
	res1 = vis_fmuld8ulx16(expand, vis_read_hi(accm0));     \
	res2 = vis_fmuld8ulx16(expand, vis_read_lo(accm0));     \
	dsum1 = vis_fpadd32(dsum1, res1);                       \
	dsum2 = vis_fpadd32(dsum2, res2);                       \
	res1 = vis_fmuld8ulx16(expand, vis_read_hi(accm1));     \
	res2 = vis_fmuld8ulx16(expand, vis_read_lo(accm1));     \
	dsum3 = vis_fpadd32(dsum3, res1);                       \
	dsum4 = vis_fpadd32(dsum4, res2)

/* *********************************************************** */

void
mlib_v_ImageXProj_U8(
    const mlib_image *src,
    mlib_d64 *xproj)
{
	mlib_u8 *sp = mlib_ImageGetData(src);
	mlib_u8 *spl;

	mlib_s32 width = mlib_ImageGetWidth(src);
	mlib_s32 height = mlib_ImageGetHeight(src);
	mlib_s32 stride = mlib_ImageGetStride(src);
	mlib_s32 offset, accm;
	mlib_s32 i = 0, j, k;
	mlib_s32 proj[8];

	mlib_f32 scale = vis_to_float((mlib_u32)256);
	mlib_f32 expand = vis_to_float(0x10001);

	mlib_d64 *dsp;
	mlib_d64 accm0, accm1, res1, res2;
	mlib_d64 dsum1, dsum2, dsum3, dsum4;
	mlib_d64 Load, dx0, dx1;

	if (!(stride & 7)) {
		offset = (8 - (mlib_addr)sp) & 7;

		for (; i < offset && i < width; i++) {
			spl = sp + i;
			accm = 0;
			for (j = 0; j < height; j++, spl += stride) {
				accm += *spl;
			}

			xproj[i] = (mlib_d64)accm;
		}

		for (; i < width - 8; i += 8) {

			spl = sp + i;
			accm0 = vis_fzero();
			accm1 = vis_fzero();
			dsum1 = 0;
			dsum2 = 0;
			dsum3 = 0;
			dsum4 = 0;

			for (j = 0; j < height >> 7; j++) {
				for (k = 0; k < 128; k++, spl += stride) {
					Load = *(mlib_d64 *)spl;
					X_MAIN_CYCLE_U8;
				}

				X_EXPAND_ACC_U8;

				accm0 = vis_fzero();
				accm1 = vis_fzero();
			}

			for (j <<= 7; j < height; j++, spl += stride) {
				Load = *(mlib_d64 *)spl;
				X_MAIN_CYCLE_U8;
			}

			X_EXPAND_ACC_U8;

			((mlib_f32 *)proj)[0] = vis_read_hi(dsum1);
			xproj[i] = proj[0];
			((mlib_f32 *)proj)[1] = vis_read_lo(dsum1);
			xproj[i + 1] = proj[1];
			((mlib_f32 *)proj)[2] = vis_read_hi(dsum2);
			xproj[i + 2] = proj[2];
			((mlib_f32 *)proj)[3] = vis_read_lo(dsum2);
			xproj[i + 3] = proj[3];
			((mlib_f32 *)proj)[4] = vis_read_hi(dsum3);
			xproj[i + 4] = proj[4];
			((mlib_f32 *)proj)[5] = vis_read_lo(dsum3);
			xproj[i + 5] = proj[5];
			((mlib_f32 *)proj)[6] = vis_read_hi(dsum4);
			xproj[i + 6] = proj[6];
			((mlib_f32 *)proj)[7] = vis_read_lo(dsum4);
			xproj[i + 7] = proj[7];
		}

		for (; i < width; i++) {
			spl = sp + i;
			accm = 0;
			for (j = 0; j < height; j++, spl += stride) {
				accm += *spl;
			}

			xproj[i] = (mlib_d64)accm;
		}
	} else {
		for (; i < width - 8; i += 8) {

			spl = sp + i;
			accm0 = vis_fzero();
			accm1 = vis_fzero();
			dsum1 = 0.;
			dsum2 = 0.;
			dsum3 = 0.;
			dsum4 = 0.;

			for (j = 0; j < height >> 7; j++) {
				for (k = 0; k < 128; k++, spl += stride) {
					dsp = (mlib_d64 *)vis_alignaddr(spl, 0);
					dx0 = dsp[0];
					dx1 = dsp[1];
					Load = vis_faligndata(dx0, dx1);
					X_MAIN_CYCLE_U8;
				}

				X_EXPAND_ACC_U8;

				accm0 = vis_fzero();
				accm1 = vis_fzero();
			}

			for (j <<= 7; j < height; j++, spl += stride) {
				dsp = (mlib_d64 *)vis_alignaddr(spl, 0);
				dx0 = dsp[0];
				dx1 = dsp[1];
				Load = vis_faligndata(dx0, dx1);
				X_MAIN_CYCLE_U8;
			}

			X_EXPAND_ACC_U8;

			((mlib_f32 *)proj)[0] = vis_read_hi(dsum1);
			xproj[i] = proj[0];
			((mlib_f32 *)proj)[1] = vis_read_lo(dsum1);
			xproj[i + 1] = proj[1];
			((mlib_f32 *)proj)[2] = vis_read_hi(dsum2);
			xproj[i + 2] = proj[2];
			((mlib_f32 *)proj)[3] = vis_read_lo(dsum2);
			xproj[i + 3] = proj[3];
			((mlib_f32 *)proj)[4] = vis_read_hi(dsum3);
			xproj[i + 4] = proj[4];
			((mlib_f32 *)proj)[5] = vis_read_lo(dsum3);
			xproj[i + 5] = proj[5];
			((mlib_f32 *)proj)[6] = vis_read_hi(dsum4);
			xproj[i + 6] = proj[6];
			((mlib_f32 *)proj)[7] = vis_read_lo(dsum4);
			xproj[i + 7] = proj[7];
		}

		for (; i < width; i++) {
			spl = sp + i;
			accm = 0;
			for (j = 0; j < height; j++, spl += stride) {
				accm += *spl;
			}

			xproj[i] = (mlib_d64)accm;
		}
	}
}

/* *********************************************************** */

#define	X_MAIN_CYCLE_16                                                 \
	acc1 =                                                          \
	vis_fpadd32(acc1, vis_fmuld8ulx16(scale, vis_read_hi(Load)));   \
	acc2 =                                                          \
	vis_fpadd32(acc2, vis_fmuld8ulx16(scale, vis_read_lo(Load)))

/* *********************************************************** */

void
mlib_v_ImageXProj_U16(
    const mlib_image *src,
    mlib_d64 *xproj)
{
/* width of source image */
	mlib_s32 Width = mlib_ImageGetWidth(src);

/* height of source image */
	mlib_s32 Height = mlib_ImageGetHeight(src);

/* linestride = bytes to next row */
	mlib_s32 YStride = (mlib_ImageGetStride(src)) / 2;

/* pointers to the data of image */
	mlib_u16 *LinePntr = (mlib_u16 *)mlib_ImageGetData(src);
	mlib_u16 *PixelPntr = (mlib_u16 *)mlib_ImageGetData(src);

	mlib_u16 *Pntrend;

	mlib_s32 proj[8];
	mlib_d64 *dpx;

	mlib_d64 dx0;
	mlib_d64 dx1;

	mlib_d64 Load;

	mlib_d64 xorMask = vis_to_double_dup(0x80008000);
	mlib_f32 scale = vis_to_float(0x10001);

	mlib_s32 patch_s16_2_u16 = Height << 15;
	mlib_s32 alignSteps = (8 - (mlib_addr)PixelPntr) & 7;

/* number of pixel to process */
	mlib_s32 PixelNumber = Height;

	mlib_s32 NewWidth, size = Width;
	mlib_s32 i, j, c;

	mlib_d64 acc1, acc2;

	if ((YStride & 7) == 0) {

		for (c = 0; c < (alignSteps / 2); c++) {
			mlib_d64 Acc = 0;

			if (c == Width)
				break;
			PixelPntr = LinePntr + c;
			for (j = 0; j < Height; j++) {
				Acc += *PixelPntr;
				PixelPntr += YStride;
			}

			xproj[c] = (mlib_d64)Acc;
		}

		NewWidth = Width - c - ((Width - c) & 7);

		for (i = c; i < NewWidth; i += 4) {
			PixelPntr = LinePntr + i;

			acc1 = 0;
			acc2 = 0;

			for (j = 0; j < PixelNumber; j++) {
				Load = ((mlib_d64 *)PixelPntr)[0];
				Load = vis_fxor(Load, xorMask);
				PixelPntr += YStride;

				X_MAIN_CYCLE_16;
			}

			((mlib_f32 *)proj)[0] = vis_read_hi(acc1);
			xproj[i] = proj[0] + patch_s16_2_u16;
			((mlib_f32 *)proj)[1] = vis_read_lo(acc1);
			xproj[i + 1] = proj[1] + patch_s16_2_u16;
			((mlib_f32 *)proj)[2] = vis_read_hi(acc2);
			xproj[i + 2] = proj[2] + patch_s16_2_u16;
			((mlib_f32 *)proj)[3] = vis_read_lo(acc2);
			xproj[i + 3] = proj[3] + patch_s16_2_u16;
		}

		for (i = NewWidth + c; i < Width; i++) {
			mlib_d64 Acc = 0;

			PixelPntr = LinePntr + i;
			for (j = 0; j < Height; j++) {
				Acc += *PixelPntr;
				PixelPntr += YStride;
			}

			xproj[i] = (mlib_d64)Acc;
		}
	} else {
		c = 0;
		Pntrend = PixelPntr + size - 1;
		while ((Pntrend - PixelPntr + 1) >= 4) {
			acc1 = 0;
			acc2 = 0;

			for (i = 0; i < PixelNumber - 1; i++) {
				dpx = (mlib_d64 *)vis_alignaddr(PixelPntr, 0);
				dx0 = dpx[0];
				dx1 = dpx[1];
				Load = vis_faligndata(dx0, dx1);
				Load = vis_fxor(Load, xorMask);
				PixelPntr += YStride;

				X_MAIN_CYCLE_16;
			}

			if (i < PixelNumber) {
				dpx = (mlib_d64 *)vis_alignaddr(PixelPntr, 0);
				dx0 = vis_ld_d64_nf(dpx);
				dx1 = vis_ld_d64_nf(dpx + 1);
				Load = vis_faligndata(dx0, dx1);
				Load = vis_fxor(Load, xorMask);

				X_MAIN_CYCLE_16;
			}

			((mlib_f32 *)proj)[0] = vis_read_hi(acc1);
			xproj[c] = proj[0] + patch_s16_2_u16;
			((mlib_f32 *)proj)[1] = vis_read_lo(acc1);
			xproj[c + 1] = proj[1] + patch_s16_2_u16;
			((mlib_f32 *)proj)[2] = vis_read_hi(acc2);
			xproj[c + 2] = proj[2] + patch_s16_2_u16;
			((mlib_f32 *)proj)[3] = vis_read_lo(acc2);
			xproj[c + 3] = proj[3] + patch_s16_2_u16;

			LinePntr += 4;
			c += 4;
			PixelPntr = LinePntr;
		}

		PixelNumber = (Pntrend - PixelPntr + 1);
		for (i = 0; i < PixelNumber; i++) {
			mlib_d64 Acc = 0;

			PixelPntr = LinePntr + i;
			for (j = 0; j < Height; j++) {
				Acc += *PixelPntr;
				PixelPntr += YStride;
			}

			xproj[c] = (mlib_d64)Acc;
			c++;
		}
	}
}

/* *********************************************************** */

void
mlib_v_ImageXProj_S16(
    const mlib_image *src,
    mlib_d64 *xproj)
{
/* width of source image */
	mlib_s32 Width = mlib_ImageGetWidth(src);

/* height of source image */
	mlib_s32 Height = mlib_ImageGetHeight(src);

/* linestride = bytes to next row */
	mlib_s32 YStride = (mlib_ImageGetStride(src)) / 2;

/* pointers to the data of image */
	mlib_s16 *LinePntr = (mlib_s16 *)mlib_ImageGetData(src);
	mlib_s16 *PixelPntr = (mlib_s16 *)mlib_ImageGetData(src);

	mlib_s16 *Pntrend;

	mlib_s32 proj[8];
	mlib_d64 *dpx;

	mlib_d64 dx0;
	mlib_d64 dx1;

	mlib_d64 Load;

	mlib_f32 scale = vis_to_float(0x10001);

	mlib_s32 alignSteps = (8 - (mlib_addr)PixelPntr) & 7;

/* number of pixel to process */
	mlib_s32 PixelNumber = Height;

	mlib_s32 NewWidth, size = Width;
	mlib_s32 i, j, c;

	mlib_d64 acc1, acc2;

	if ((YStride & 7) == 0) {

		for (c = 0; c < (alignSteps / 2); c++) {
			mlib_d64 Acc = 0;

			if (c == Width)
				break;
			PixelPntr = LinePntr + c;
			for (j = 0; j < Height; j++) {
				Acc += *PixelPntr;
				PixelPntr += YStride;
			}

			xproj[c] = (mlib_d64)Acc;
		}

		NewWidth = Width - c - ((Width - c) & 7);

		for (i = c; i < NewWidth; i += 4) {
			PixelPntr = LinePntr + i;

			acc1 = 0;
			acc2 = 0;

			for (j = 0; j < PixelNumber; j++) {
				Load = ((mlib_d64 *)PixelPntr)[0];
				PixelPntr += YStride;

				X_MAIN_CYCLE_16;
			}

			((mlib_f32 *)proj)[0] = vis_read_hi(acc1);
			xproj[i] = proj[0];
			((mlib_f32 *)proj)[1] = vis_read_lo(acc1);
			xproj[i + 1] = proj[1];
			((mlib_f32 *)proj)[2] = vis_read_hi(acc2);
			xproj[i + 2] = proj[2];
			((mlib_f32 *)proj)[3] = vis_read_lo(acc2);
			xproj[i + 3] = proj[3];
		}

		for (i = NewWidth + c; i < Width; i++) {
			mlib_d64 Acc = 0;

			PixelPntr = LinePntr + i;
			for (j = 0; j < Height; j++) {
				Acc += *PixelPntr;
				PixelPntr += YStride;
			}

			xproj[i] = (mlib_d64)Acc;
		}
	} else {
		c = 0;
		Pntrend = PixelPntr + size - 1;
		while ((Pntrend - PixelPntr + 1) >= 4) {
			acc1 = 0;
			acc2 = 0;

			for (i = 0; i < PixelNumber - 1; i++) {
				dpx = (mlib_d64 *)vis_alignaddr(PixelPntr, 0);
				dx0 = dpx[0];
				dx1 = dpx[1];
				Load = vis_faligndata(dx0, dx1);
				PixelPntr += YStride;

				X_MAIN_CYCLE_16;
			}

			if (i < PixelNumber) {
				dpx = (mlib_d64 *)vis_alignaddr(PixelPntr, 0);
				dx0 = vis_ld_d64_nf(dpx);
				dx1 = vis_ld_d64_nf(dpx + 1);
				Load = vis_faligndata(dx0, dx1);
				X_MAIN_CYCLE_16;
			}

			((mlib_f32 *)proj)[0] = vis_read_hi(acc1);
			xproj[c] = proj[0];
			((mlib_f32 *)proj)[1] = vis_read_lo(acc1);
			xproj[c + 1] = proj[1];
			((mlib_f32 *)proj)[2] = vis_read_hi(acc2);
			xproj[c + 2] = proj[2];
			((mlib_f32 *)proj)[3] = vis_read_lo(acc2);
			xproj[c + 3] = proj[3];

			LinePntr += 4;
			c += 4;
			PixelPntr = LinePntr;
		}

		PixelNumber = (Pntrend - PixelPntr + 1);
		for (i = 0; i < PixelNumber; i++) {
			mlib_d64 Acc = 0;

			PixelPntr = LinePntr + i;
			for (j = 0; j < Height; j++) {
				Acc += *PixelPntr;
				PixelPntr += YStride;
			}

			xproj[c] = (mlib_d64)Acc;
			c++;
		}
	}
}

/* *********************************************************** */

void
mlib_v_ImageXProj_S32(
    const mlib_image *src,
    mlib_d64 *xproj)
{
/* width of source image */
	mlib_s32 Width = mlib_ImageGetWidth(src);

/* height of source image */
	mlib_s32 Height = mlib_ImageGetHeight(src);

/* linestride = bytes to next row */
	mlib_s32 YStride = mlib_ImageGetStride(src) >> 2;

/* pointers to the data of image */
	mlib_s32 *LinePntr = (mlib_s32 *)mlib_ImageGetData(src), *PixelPntr;

	mlib_s32 i, j;
	mlib_d64 Accum0, Accum1, Accum2, Accum3;

	for (i = 0; i < Width - 4; i += 4) {
		Accum0 = 0;
		Accum1 = 0;
		Accum2 = 0;
		Accum3 = 0;
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
