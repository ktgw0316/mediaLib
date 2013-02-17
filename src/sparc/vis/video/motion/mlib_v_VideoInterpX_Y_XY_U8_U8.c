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

#pragma ident	"@(#)mlib_v_VideoInterpX_Y_XY_U8_U8.c	9.2	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_VideoInterpX_Y_XY_U8_U8 - Performs half-pixel interpolation in
 *                                     both X and Y directions
 *                                     for replenishment mode
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_VideoInterpX_Y_XY_U8_U8(mlib_u8       *outputX,
 *                                               mlib_u8       *outputY,
 *                                               mlib_u8       *outputXY,
 *                                               const mlib_u8 *image,
 *                                               mlib_s32      stride,
 *                                               mlib_s32      width,
 *                                               mlib_s32      height);
 *      mlib_status mlib_v_VideoInterpX_Y_XY_U8_U8_comm(mlib_u8       *outputX,
 *                                                      mlib_u8       *outputY,
 *                                                      mlib_u8       *outputXY,
 *                                                      const mlib_u8 *image,
 *                                                      mlib_s32      stride,
 *                                                      mlib_s32      width,
 *                                                      mlib_s32      height);
 *
 * ARGUMENT
 *      outputX    Pointer to output of X-interpolation,
 *                 must be 8-byte aligned
 *      outputY    Pointer to output of Y-interpolation,
 *                 must be 8-byte aligned
 *      outputXY   Pointer to output of XY-interpolation,
 *                 must be 8-byte aligned
 *      image      Pointer to image data, must be 8-byte aligned
 *      stride     Stride in bytes between rows in image,
 *                 must be a multiple of 8
 *      width      Width of image, must be a multiple of 8
 *      height     Height of image, must be a multiple of 2
 */

#include <mlib_video.h>
#include <vis_proto.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_VideoInterpX_Y_XY_U8_U8 = __mlib_VideoInterpX_Y_XY_U8_U8

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_VideoInterpX_Y_XY_U8_U8) mlib_VideoInterpX_Y_XY_U8_U8
	__attribute__((weak, alias("__mlib_VideoInterpX_Y_XY_U8_U8")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	MLIB_V_VIDEOINTERP(dst, src, srcf, N)                          \
	sum##N##hi =                                                   \
	vis_fpsub16(vis_fmul8x16al(vis_read_hi(src), fm1), rounder);   \
	sum##N##lo =                                                   \
	vis_fpsub16(vis_fmul8x16al(vis_read_lo(src), fm1), rounder);   \
	sum##N##hi =                                                   \
	vis_fpadd16(sum##N##hi,                                        \
		vis_fmul8x16al(vis_read_hi(srcf), fm1));               \
	sum##N##lo =                                                   \
	vis_fpadd16(sum##N##lo, vis_fpmerge(fzero,                     \
		vis_read_lo(srcf)));                                   \
	dst = vis_fpack16_pair(sum##N##hi, sum##N##lo)

/* *********************************************************** */

#define	MLIB_V_VIDEOINTERP1(dst, src, srcf, N)                  \
	sum##N##hi =                                            \
	vis_fpadd16(vis_fmul8x16al(vis_read_hi(src), fm1),      \
		vis_fmul8x16al(vis_read_hi(srcf), fm1));        \
	sum##N##lo =                                            \
	vis_fpadd16(vis_fpmerge(fzero, vis_read_lo(srcf)),      \
		vis_fmul8x16al(vis_read_lo(src), fm1));         \
	dst =                                                   \
	vis_fpack16_pair(vis_fpsub16(sum##N##hi, rounder),      \
		vis_fpsub16(sum##N##lo, rounder))

/* *********************************************************** */

#define	VIS_PST_8(a, b, c)	vis_pst_8(a, b, c)

/* *********************************************************** */

#define	fzero	vis_fzeros()

/* *********************************************************** */

#define	BUFF_SIZE	32

static mlib_status mlib_v_VideoInterpX_Y_XY_U8_U8_w16(
	mlib_u8 *outputX,
	mlib_u8 *outputY,
	mlib_u8 *outputXY,
	const mlib_u8 *image,
	mlib_s32 stride,
	mlib_s32 height);

/* *********************************************************** */

mlib_status
mlib_v_VideoInterpX_Y_XY_U8_U8_comm(
	mlib_u8 *outputX,
	mlib_u8 *outputY,
	mlib_u8 *outputXY,
	const mlib_u8 *image,
	mlib_s32 stride,
	mlib_s32 width,
	mlib_s32 height)
{
	mlib_d64 buff_local[BUFF_SIZE], *buff = buff_local;
	mlib_f32 fm1 = vis_to_float(0x100);
	mlib_f32 scale = vis_to_float(0x80808080);
	mlib_d64 rounder = vis_fone();
	mlib_d64 *sl0, *sl1, *sl2, *dlX, *dlY, *dlZ;
	mlib_d64 a0, a1, ax, b0, b1, bx, c0, c1, cx;
	mlib_d64 sum0hi, sum0lo, sum1hi, sum1lo, sum2hi, sum2lo, xy_hi, xy_lo;
	mlib_s32 x, y;

	vis_write_gsr((6 << 3) + 1);
	stride /= 8;
	width /= 8;

	if (2 * width > BUFF_SIZE) {
		buff = __mlib_malloc((2 * width) * sizeof (mlib_d64));

		if (buff == NULL)
			return (MLIB_FAILURE);
	}

	dlX = (mlib_d64 *)outputX;
	dlY = (mlib_d64 *)outputY;
	dlZ = (mlib_d64 *)outputXY;
	sl0 = (mlib_d64 *)image;

/*
 * Calc X-Interpolation for first line and save sums to buffer
 */
	a1 = sl0[0];
	for (x = 0; x < width; x++) {
		a0 = a1;
		a1 = sl0[x + 1];
		ax = vis_faligndata(a0, a1);

/*
 * X-Interpolation
 */
		MLIB_V_VIDEOINTERP1(dlX[x], a0, ax, 0);

		buff[2 * x] = sum0hi;
		buff[2 * x + 1] = sum0lo;
	}

	dlX += stride;

	for (y = 0; y <= (height - 2 - 1); y += 2) {
		sl1 = sl0 + stride;
		sl2 = sl1 + stride;

		b1 = sl1[0];
		c1 = sl2[0];
		for (x = 0; x < width; x++) {
			a0 = sl0[x];
			b0 = b1;
			b1 = sl1[x + 1];
			bx = vis_faligndata(b0, b1);
			c0 = c1;
			c1 = sl2[x + 1];
			cx = vis_faligndata(c0, c1);

			sum0hi = buff[2 * x];
			sum0lo = buff[2 * x + 1];

/*
 * Y-Interpolation
 */
			MLIB_V_VIDEOINTERP(dlY[x], a0, b0, 1);
			MLIB_V_VIDEOINTERP(dlY[x + stride], b0, c0, 1);

/*
 * X-Interpolation
 */
			MLIB_V_VIDEOINTERP(dlX[x], b0, bx, 1);
			MLIB_V_VIDEOINTERP1(dlX[x + stride], c0, cx, 2);

/*
 * XY-Interpolation
 */
			xy_hi = vis_fpadd16(sum0hi, sum1hi);
			xy_lo = vis_fpadd16(sum0lo, sum1lo);
			xy_hi = vis_fmul8x16(scale, xy_hi);
			xy_lo = vis_fmul8x16(scale, xy_lo);
			dlZ[x] = vis_fpack16_pair(xy_hi, xy_lo);

			xy_hi = vis_fpadd16(sum1hi, sum2hi);
			xy_lo = vis_fpadd16(sum1lo, sum2lo);
			xy_hi = vis_fmul8x16(scale, xy_hi);
			xy_lo = vis_fmul8x16(scale, xy_lo);
			dlZ[x + stride] = vis_fpack16_pair(xy_hi, xy_lo);

			buff[2 * x] = sum2hi;
			buff[2 * x + 1] = sum2lo;
		}

		sl0 += 2 * stride;
		dlX += 2 * stride;
		dlY += 2 * stride;
		dlZ += 2 * stride;
	}

	sl1 = sl0 + stride;
	b1 = sl1[0];

	for (x = 0; x < width; x++) {
		a0 = sl0[x];
		b0 = b1;
		b1 = vis_ld_d64_nf(sl1 + x + 1);
		bx = vis_faligndata(b0, b1);

/*
 * Y-Interpolation
 */
		MLIB_V_VIDEOINTERP(dlY[x], a0, b0, 1);

/*
 * X-Interpolation
 */
		MLIB_V_VIDEOINTERP(dlX[x], b0, bx, 1);

/*
 * XY-Interpolation
 */
		sum0hi = buff[2 * x];
		sum0lo = buff[2 * x + 1];

		xy_hi = vis_fpadd16(sum0hi, sum1hi);
		xy_lo = vis_fpadd16(sum0lo, sum1lo);
		xy_hi = vis_fmul8x16(scale, xy_hi);
		xy_lo = vis_fmul8x16(scale, xy_lo);
		dlZ[x] = vis_fpack16_pair(xy_hi, xy_lo);

		dlY[x + stride] = b0;
		dlZ[x + stride] = dlX[x];
	}

/*
 * Taking care of the last pel of each line
 */
	for (y = 0; y < height; y++) {
		outputX[(y * 8 * stride) + 8 * width - 1] =
			image[(y * 8 * stride) + 8 * width - 1];
		outputXY[(y * 8 * stride) + 8 * width - 1] =
			outputY[(y * 8 * stride) + 8 * width - 1];
	}

	if (buff != buff_local)
		__mlib_free(buff);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VideoInterpX_Y_XY_U8_U8(
	mlib_u8 *outputX,
	mlib_u8 *outputY,
	mlib_u8 *outputXY,
	const mlib_u8 *image,
	mlib_s32 stride,
	mlib_s32 width,
	mlib_s32 height)
{
	mlib_f32 fm1 = vis_to_float(0x100);
	mlib_f32 scale = vis_to_float(0x80808080);
	mlib_d64 rounder = vis_fone();
	mlib_d64 *sl0, *dlX, *dlY, *dlZ;
	mlib_d64 a0, ax, b0, bx, c0, cx;
	mlib_d64 sum0hi, sum0lo, sum1hi, sum1lo, sum2hi, sum2lo, xy_hi, xy_lo;
	mlib_s32 y;

	if (width != 8) {
		if (width == 16) {
			return mlib_v_VideoInterpX_Y_XY_U8_U8_w16(outputX,
				outputY, outputXY, image, stride, height);
		}

		return mlib_v_VideoInterpX_Y_XY_U8_U8_comm(outputX, outputY,
			outputXY, image, stride, width, height);
	}

/*
 * Special code for width == 8
 */

	vis_write_gsr((6 << 3) + 1);
	stride /= 8;

	dlX = (mlib_d64 *)outputX;
	dlY = (mlib_d64 *)outputY;
	dlZ = (mlib_d64 *)outputXY;
	sl0 = (mlib_d64 *)image;

/*
 * Calc X-Interpolation for first line and save sums
 */
	a0 = sl0[0];
	ax = vis_faligndata(a0, a0);
	MLIB_V_VIDEOINTERP1(dlX[0], a0, ax, 2);
	VIS_PST_8(a0, dlX, 1);
	dlX += stride;

	for (y = 0; y <= (height - 2 - 1); y += 2) {
		a0 = sl0[0];
		b0 = sl0[stride];
		c0 = sl0[2 * stride];
		bx = vis_faligndata(b0, b0);
		cx = vis_faligndata(c0, c0);

		sum0hi = sum2hi;
		sum0lo = sum2lo;

/*
 * Y-Interpolation
 */
		MLIB_V_VIDEOINTERP(dlY[0], a0, b0, 1);
		MLIB_V_VIDEOINTERP(dlY[stride], b0, c0, 1);

/*
 * X-Interpolation
 */
		MLIB_V_VIDEOINTERP(dlX[0], b0, bx, 1);
		MLIB_V_VIDEOINTERP1(dlX[stride], c0, cx, 2);

/*
 * XY-Interpolation
 */
		xy_hi = vis_fpadd16(sum0hi, sum1hi);
		xy_lo = vis_fpadd16(sum0lo, sum1lo);
		xy_hi = vis_fmul8x16(scale, xy_hi);
		xy_lo = vis_fmul8x16(scale, xy_lo);
		dlZ[0] = vis_fpack16_pair(xy_hi, xy_lo);

		xy_hi = vis_fpadd16(sum1hi, sum2hi);
		xy_lo = vis_fpadd16(sum1lo, sum2lo);
		xy_hi = vis_fmul8x16(scale, xy_hi);
		xy_lo = vis_fmul8x16(scale, xy_lo);
		dlZ[stride] = vis_fpack16_pair(xy_hi, xy_lo);

		VIS_PST_8(b0, dlX, 1);
		VIS_PST_8(c0, dlX + stride, 1);
		VIS_PST_8(dlY[0], dlZ, 1);
		VIS_PST_8(dlY[stride], dlZ + stride, 1);

		sl0 += 2 * stride;
		dlX += 2 * stride;
		dlY += 2 * stride;
		dlZ += 2 * stride;
	}

	a0 = sl0[0];
	b0 = sl0[stride];
	bx = vis_faligndata(b0, b0);

/*
 * Y-Interpolation
 */
	MLIB_V_VIDEOINTERP(dlY[0], a0, b0, 1);

/*
 * X-Interpolation
 */
	MLIB_V_VIDEOINTERP(dlX[0], b0, bx, 1);

/*
 * XY-Interpolation
 */
	sum0hi = sum2hi;
	sum0lo = sum2lo;

	xy_hi = vis_fpadd16(sum0hi, sum1hi);
	xy_lo = vis_fpadd16(sum0lo, sum1lo);
	xy_hi = vis_fmul8x16(scale, xy_hi);
	xy_lo = vis_fmul8x16(scale, xy_lo);
	dlZ[0] = vis_fpack16_pair(xy_hi, xy_lo);

	dlY[stride] = b0;
	dlZ[stride] = dlX[0];

	VIS_PST_8(b0, dlX, 1);
	VIS_PST_8(dlY[0], dlZ, 1);

	VIS_PST_8(b0, dlZ + stride, 1);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_v_VideoInterpX_Y_XY_U8_U8_w16(
	mlib_u8 *outputX,
	mlib_u8 *outputY,
	mlib_u8 *outputXY,
	const mlib_u8 *image,
	mlib_s32 stride,
	mlib_s32 height)
{
	mlib_f32 fm1 = vis_to_float(0x100);
	mlib_f32 scale = vis_to_float(0x80808080);
	mlib_d64 rounder = vis_fone();
	mlib_d64 *sl0, *dlX, *dlY, *dlZ;
	mlib_d64 a0, a1, ax, ay, b0, b1, bx, by, c0, c1, cx, cy;
	mlib_d64 sum0hi, sum0lo, sum1hi, sum1lo, sum2hi, sum2lo, xy_hi, xy_lo;
	mlib_d64 sum3hi, sum3lo, sum4hi, sum4lo, sum5hi, sum5lo;
	mlib_s32 y;

	vis_write_gsr((6 << 3) + 1);
	stride /= 8;

	dlX = (mlib_d64 *)outputX;
	dlY = (mlib_d64 *)outputY;
	dlZ = (mlib_d64 *)outputXY;
	sl0 = (mlib_d64 *)image;

/*
 * Calc X-Interpolation for first line and save sums
 */
	a0 = sl0[0];
	a1 = sl0[1];
	ax = vis_faligndata(a0, a1);
	ay = vis_faligndata(a1, a1);
	MLIB_V_VIDEOINTERP1(dlX[0], a0, ax, 4);
	MLIB_V_VIDEOINTERP1(dlX[1], a1, ay, 5);

	VIS_PST_8(a1, dlX + 1, 1);
	dlX += stride;

	for (y = 0; y <= (height - 2 - 1); y += 2) {

		a0 = sl0[0];
		a1 = sl0[1];
		b0 = sl0[stride];
		b1 = sl0[stride + 1];
		c0 = sl0[2 * stride];
		c1 = sl0[2 * stride + 1];
		bx = vis_faligndata(b0, b1);
		cx = vis_faligndata(c0, c1);
		by = vis_faligndata(b1, b1);
		cy = vis_faligndata(c1, c1);

		sum0hi = sum4hi;
		sum0lo = sum4lo;
		sum1hi = sum5hi;
		sum1lo = sum5lo;

/*
 * Y-Interpolation
 */
		MLIB_V_VIDEOINTERP(dlY[0], a0, b0, 2);
		MLIB_V_VIDEOINTERP(dlY[1], a1, b1, 2);
		MLIB_V_VIDEOINTERP(dlY[stride], b0, c0, 2);
		MLIB_V_VIDEOINTERP(dlY[stride + 1], b1, c1, 2);

/*
 * X-Interpolation
 */
		MLIB_V_VIDEOINTERP(dlX[0], b0, bx, 2);
		MLIB_V_VIDEOINTERP(dlX[1], b1, by, 3);
		MLIB_V_VIDEOINTERP1(dlX[stride], c0, cx, 4);
		MLIB_V_VIDEOINTERP1(dlX[stride + 1], c1, cy, 5);

/*
 * XY-Interpolation
 */
		xy_hi = vis_fpadd16(sum0hi, sum2hi);
		xy_lo = vis_fpadd16(sum0lo, sum2lo);
		xy_hi = vis_fmul8x16(scale, xy_hi);
		xy_lo = vis_fmul8x16(scale, xy_lo);
		dlZ[0] = vis_fpack16_pair(xy_hi, xy_lo);

		xy_hi = vis_fpadd16(sum1hi, sum3hi);
		xy_lo = vis_fpadd16(sum1lo, sum3lo);
		xy_hi = vis_fmul8x16(scale, xy_hi);
		xy_lo = vis_fmul8x16(scale, xy_lo);
		dlZ[1] = vis_fpack16_pair(xy_hi, xy_lo);

		xy_hi = vis_fpadd16(sum2hi, sum4hi);
		xy_lo = vis_fpadd16(sum2lo, sum4lo);
		xy_hi = vis_fmul8x16(scale, xy_hi);
		xy_lo = vis_fmul8x16(scale, xy_lo);
		dlZ[stride] = vis_fpack16_pair(xy_hi, xy_lo);

		xy_hi = vis_fpadd16(sum3hi, sum5hi);
		xy_lo = vis_fpadd16(sum3lo, sum5lo);
		xy_hi = vis_fmul8x16(scale, xy_hi);
		xy_lo = vis_fmul8x16(scale, xy_lo);
		dlZ[stride + 1] = vis_fpack16_pair(xy_hi, xy_lo);

		VIS_PST_8(b1, dlX + 1, 1);
		VIS_PST_8(c1, dlX + stride + 1, 1);
		VIS_PST_8(dlY[1], dlZ + 1, 1);
		VIS_PST_8(dlY[stride + 1], dlZ + stride + 1, 1);

		sl0 += 2 * stride;
		dlX += 2 * stride;
		dlY += 2 * stride;
		dlZ += 2 * stride;
	}

	a0 = sl0[0];
	a1 = sl0[1];
	b0 = sl0[stride];
	b1 = sl0[stride + 1];
	bx = vis_faligndata(b0, b1);
	by = vis_faligndata(b1, b1);

/*
 * Y-Interpolation
 */
	MLIB_V_VIDEOINTERP(dlY[0], a0, b0, 2);
	MLIB_V_VIDEOINTERP(dlY[1], a1, b1, 2);

/*
 * X-Interpolation
 */
	MLIB_V_VIDEOINTERP(dlX[0], b0, bx, 2);
	MLIB_V_VIDEOINTERP(dlX[1], b1, by, 3);

/*
 * XY-Interpolation
 */
	sum0hi = sum4hi;
	sum0lo = sum4lo;
	sum1hi = sum5hi;
	sum1lo = sum5lo;

	xy_hi = vis_fpadd16(sum0hi, sum2hi);
	xy_lo = vis_fpadd16(sum0lo, sum2lo);
	xy_hi = vis_fmul8x16(scale, xy_hi);
	xy_lo = vis_fmul8x16(scale, xy_lo);
	dlZ[0] = vis_fpack16_pair(xy_hi, xy_lo);

	xy_hi = vis_fpadd16(sum1hi, sum3hi);
	xy_lo = vis_fpadd16(sum1lo, sum3lo);
	xy_hi = vis_fmul8x16(scale, xy_hi);
	xy_lo = vis_fmul8x16(scale, xy_lo);
	dlZ[1] = vis_fpack16_pair(xy_hi, xy_lo);

	dlY[stride] = b0;
	dlY[stride + 1] = b1;
	dlZ[stride] = dlX[0];
	dlZ[stride + 1] = dlX[1];

	VIS_PST_8(b1, dlX + 1, 1);
	VIS_PST_8(dlY[1], dlZ + 1, 1);

	VIS_PST_8(b1, dlZ + stride + 1, 1);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
