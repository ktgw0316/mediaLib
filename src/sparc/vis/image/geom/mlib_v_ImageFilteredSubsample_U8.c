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

#pragma ident	"@(#)mlib_v_ImageFilteredSubsample_U8.c	9.2	07/11/05 SMI"

/*
 * FUNCTION
 *
 *      mlib_ImageFilteredSubsample - Antialias filter and subsample an image
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_ImageFilteredSubsample(mlib_image       *dst,
 *                                              const mlib_image *src,
 *                                              mlib_s32         scaleX,
 *                                              mlib_s32         scaleY,
 *                                              mlib_s32         transX,
 *                                              mlib_s32         transY,
 *                                              const mlib_d64   *hKernel,
 *                                              const mlib_d64   *vKernel,
 *                                              mlib_s32         hSize,
 *                                              mlib_s32         vSize,
 *                                              mlib_s32         hParity,
 *                                              mlib_s32         vParity,
 *                                              mlib_edge        edge)
 * ARGUMENTS
 *
 *      dst       Pointer to the destination image.
 *      src       Pointer to the source image.
 *      scaleX    The x scale factor of subsampling.
 *      scaleY    The y scale factor of subsampling.
 *      transX    The x translation.
 *      transY    The y translation.
 *      hKernel   Pointer to the compact form of the horizontal kernel.
 *      vKernel   Pointer to the compact form of the vertical kernel.
 *      hSize     Size of the array hKernel.
 *      vSize     Size of the array vKernel.
 *      hParity   Parity of the horizontal kernel (0 => even, 1 => odd).
 *      vParity   Parity of the vertical kernel (0 => even, 1 => odd).
 *      edge      Type of edge condition.
 *
 * DESCRIPTION
 *
 *      An operator to antialias filter and subsample images.  The effect of
 *      this operation on an image is equivalent to performing convolution
 *      (filter) followed by subsample (zoom out) operations.
 *
 *      This operator is similar to the image ZoomTranslate operator.  But
 *      they have different definitions on scale factors and translations,
 *      hence use different coordinate mapping equations.  The scaleX and
 *      scaleY used by FilteredSubsample are the reciprocals of the zoomx
 *      and zoomy, respectively, used by ZoomTranslate.
 *
 *      The FilteredSubsample operator mapping equations are given by
 *
 *          xS = xD*scaleX + transX
 *          yS = yD*scaleY + transY
 *
 *      where, a point (xD, yD) in the destination image is backward mapped
 *      to a point (xS, yS) in the source image.  The arguments transX and
 *      transY are provided to support tiling.
 *
 *      The subsample terms, i.e., the scale factors scaleX and scaleY,
 *      are restricted to positive integral values.  Geometrically, one
 *      destination pixel maps to scaleX by scaleY source pixels.
 *      With odd scale factors, destination pixel centers map directly onto
 *      source pixel centers.  With even scale factors, destination pixels
 *      map squarely between source pixel centers.  Below are examples of
 *      even, odd, and combination cases.
 *
 *          s = source pixel centers
 *          d = destination pixel centers mapped to source
 *
 *          s   s   s   s   s   s           s   s   s   s   s   s
 *            d       d       d
 *          s   s   s   s   s   s           s   d   s   s   d   s
 *
 *          s   s   s   s   s   s           s   s   s   s   s   s
 *            d       d       d
 *          s   s   s   s   s   s           s   s   s   s   s   s
 *
 *          s   s   s   s   s   s           s   d   s   s   d   s
 *            d       d       d
 *          s   s   s   s   s   s           s   s   s   s   s   s
 *
 *          Even scaleX/Y factors            Odd scaleX/Y factors
 *
 *          s   s   s   s   s   s           s   s   s   s   s   s
 *              d           d
 *          s   s   s   s   s   s           s d s   s d s   s d s
 *
 *          s   s   s   s   s   s           s   s   s   s   s   s
 *              d           d
 *          s   s   s   s   s   s           s   s   s   s   s   s
 *
 *          s   s   s   s   s   s           s d s   s d s   s d s
 *              d           d
 *          s   s   s   s   s   s           s   s   s   s   s   s
 *
 *         Odd/even scaleX/Y factors      Even/odd scaleX/Y factors
 *
 *      The applied filter is quadrant symmetric (typically antialias +
 *      resample).  The filter is product-separable, quadrant symmetric,
 *      and is defined by half of its span.  Parity is used to signify
 *      whether the symmetric kernel has a double center (even parity) or
 *      a single center value (odd parity).  For example, if hParity == 0
 *      (even), the horizontal kernel is defined as:
 *
 *      hKernel[hSize-1], ..., hKernel[0], hKernel[0], ..., hKernel[hSize-1]
 *
 *      Otherwise, if hParity == 1 (odd), the horizontal kernel is defined
 *      as:
 *
 *          hKernel[hSize-1], ..., hKernel[0], ..., hKernel[hSize-1]
 *
 *      Horizontal and vertical kernels representing convolved resample
 *      (i.e., the combined separable kernels) can be computed from a
 *      convolution filter (with odd parity), a resample filter, and because
 *      the subsample factors affect resample weights, the subsample scale
 *      factors.  It is the user's responsibility to provide meaningful
 *      combined kernels.
 *
 *      To compute the value of a pixel centered at point (xD, yD) in the
 *      destination image, apply the combined kernel to the source image by
 *      aligning the kernel's geometric center to the backward mapped point
 *      (xS, yS) in the source image.  In the cases that it can not be
 *      exactly on top of point (xS, yS), the kernel's center should be
 *      half-pixel right and/or below that point.  When this is done in a
 *      separable manner, the centers of horizontal and vertical kernels
 *      should align with xS and yS, respectively.
 *
 *      The combination of subsampling and filtering has performance
 *      benefits over sequential operator usage in part due to the symmetry
 *      constraints imposed by only allowing integer parameters for scaling
 *      and only allowing separable symmetric filters.
 *
 * RESTRICTION
 *
 *      The src and the dst must be the same type and have same number
 *      of channels (1, 2, 3, or 4).
 *      The images can be of type MLIB_BYTE, MLIB_SHORT, MLIB_USHORT or
 *      MLIB_INT.
 *
 *      The image pixels are assumed to be centered at .5 coordinate points,
 *      and the upper-left corner pixel of an image is located at (0.5, 0.5).
 *
 *      scaleX > 0
 *      scaleY > 0
 *
 *      The edge condition can be one of the following:
 *              MLIB_EDGE_DST_NO_WRITE  (default)
 */

#include <vis_proto.h>
#include <mlib_image.h>
#include <mlib_ImageCheck.h>
#include <mlib_v_ImageFilteredSubsample.h>

/* *********************************************************** */

/* for the time being the performance is better in case of this define */
#define	KER2x2_ONLY

#define	IMG_TYPE	1

/* *********************************************************** */

#define	DSCALE	(1u << (31 - iscale))

#define	FTYPE	mlib_d64

#define	STYPE	mlib_u8

#define	FS_FUNC(SUFF)	mlib_v_ImageFilteredSubsample_8nw_##SUFF

/* *********************************************************** */

#define	PARAMS                                                            \
	FTYPE *buff, STYPE * sp0, STYPE * sp1, FTYPE * pkk,               \
	mlib_s32 xsize, mlib_s32 off, mlib_s32 step, mlib_s32 nchan

/* *********************************************************** */

#define	PARAMS1	PARAMS, STYPE *dp

/* *********************************************************** */

#define	LD_1x4_U8(s0, sp0)                                      \
	s0 = vis_faligndata(vis_ld_u8_i(sp0, step3), s0);       \
	s0 = vis_faligndata(vis_ld_u8_i(sp0, step2), s0);       \
	s0 = vis_faligndata(vis_ld_u8_i(sp0, step), s0);        \
	s0 = vis_faligndata(vis_ld_u8_i(sp0, 0), s0)

/* *********************************************************** */

#define	LD_2x4_U8(s0, s1, sp0, sp1)                             \
	s0 = vis_faligndata(vis_ld_u8_i(sp0, step3), s0);       \
	s1 = vis_faligndata(vis_ld_u8_i(sp1, step3), s1);       \
	s0 = vis_faligndata(vis_ld_u8_i(sp0, step2), s0);       \
	s1 = vis_faligndata(vis_ld_u8_i(sp1, step2), s1);       \
	s0 = vis_faligndata(vis_ld_u8_i(sp0, step), s0);        \
	s1 = vis_faligndata(vis_ld_u8_i(sp1, step), s1);        \
	s0 = vis_faligndata(vis_ld_u8_i(sp0, 0), s0);           \
	s1 = vis_faligndata(vis_ld_u8_i(sp1, 0), s1)

/* *********************************************************** */

static void FS_FUNC(2x2i) (
    PARAMS)
{
	FTYPE s0, s1, s2, s3, dd;
	FTYPE kk;
	mlib_s32 iker0 = pkk[0];
	mlib_s32 i, step2, step3;

	kk = vis_to_double_dup((iker0 & 0xFFFF0000) | ((iker0 >> 16) & 0xFFFF));

	step2 = step + step;
	step3 = step + step2;

#pragma pipeloop(0)
	for (i = 0; i < (xsize + 3) / 4; i++) {
		mlib_u8 *sp0a = sp0 + off;
		mlib_u8 *sp1a = sp1 + off;

		LD_2x4_U8(s0, s1, sp0, sp0a);
		LD_2x4_U8(s2, s3, sp1, sp1a);

		dd = buff[i];
		dd = vis_fpadd16(dd, vis_fmul8x16(vis_read_hi(s0), kk));
		dd = vis_fpadd16(dd, vis_fmul8x16(vis_read_hi(s1), kk));
		dd = vis_fpadd16(dd, vis_fmul8x16(vis_read_hi(s2), kk));
		dd = vis_fpadd16(dd, vis_fmul8x16(vis_read_hi(s3), kk));
		buff[i] = dd;

		sp0 += 4 * step;
		sp1 += 4 * step;
	}
}

/* *********************************************************** */

static void FS_FUNC(2x2l) (
    PARAMS1)
{
	FTYPE s0, s1, s2, s3, dd;
	FTYPE val0 = pkk[4], kk;
	mlib_s32 iker0 = pkk[0];
	mlib_s32 i, step2, step3;

	kk = vis_to_double_dup((iker0 & 0xFFFF0000) | ((iker0 >> 16) & 0xFFFF));

	step2 = step + step;
	step3 = step + step2;

#pragma pipeloop(0)
	for (i = 0; i < xsize / 4; i++) {
		mlib_u8 *sp0a = sp0 + off;
		mlib_u8 *sp1a = sp1 + off;

		LD_2x4_U8(s0, s1, sp0, sp0a);
		LD_2x4_U8(s2, s3, sp1, sp1a);

		dd = buff[i];
		dd = vis_fpadd16(dd, vis_fmul8x16(vis_read_hi(s0), kk));
		dd = vis_fpadd16(dd, vis_fmul8x16(vis_read_hi(s1), kk));
		dd = vis_fpadd16(dd, vis_fmul8x16(vis_read_hi(s2), kk));
		dd = vis_fpadd16(dd, vis_fmul8x16(vis_read_hi(s3), kk));
		buff[i] = val0;

		dd = vis_freg_pair(vis_fzeros(), vis_fpack16(dd));
		vis_st_u8(dd, dp + 3 * nchan);
		dd = vis_faligndata(dd, dd);
		vis_st_u8(dd, dp + 2 * nchan);
		dd = vis_faligndata(dd, dd);
		vis_st_u8(dd, dp + nchan);
		dd = vis_faligndata(dd, dd);
		vis_st_u8(dd, dp);

		sp0 += 4 * step;
		sp1 += 4 * step;
		dp += 4 * nchan;
	}

	if (xsize & 3) {
		mlib_u8 *sp0a = sp0 + off;
		mlib_u8 *sp1a = sp1 + off;

		LD_2x4_U8(s0, s1, sp0, sp0a);
		LD_2x4_U8(s2, s3, sp1, sp1a);

		dd = buff[i];
		dd = vis_fpadd16(dd, vis_fmul8x16(vis_read_hi(s0), kk));
		dd = vis_fpadd16(dd, vis_fmul8x16(vis_read_hi(s1), kk));
		dd = vis_fpadd16(dd, vis_fmul8x16(vis_read_hi(s2), kk));
		dd = vis_fpadd16(dd, vis_fmul8x16(vis_read_hi(s3), kk));
		buff[i] = val0;

		dd = vis_freg_pair(vis_fzeros(), vis_fpack16(dd));
		dd = vis_faligndata(dd, dd);

		if ((xsize & 3) >= 3)
			vis_st_u8(dd, dp + 2 * nchan);
		dd = vis_faligndata(dd, dd);

		if ((xsize & 3) >= 2)
			vis_st_u8(dd, dp + nchan);
		dd = vis_faligndata(dd, dd);
		vis_st_u8(dd, dp);
	}
}

/* *********************************************************** */

static void FS_FUNC(1x1l) (
    PARAMS1)
{
	FTYPE s0, dd;
	FTYPE val0 = pkk[4], kk;
	mlib_s32 iker0 = pkk[0] * 4.0;
	mlib_s32 i, step2, step3;

	kk = vis_to_double_dup((iker0 & 0xFFFF0000) | ((iker0 >> 16) & 0xFFFF));

	step2 = step + step;
	step3 = step + step2;

#pragma pipeloop(0)
	for (i = 0; i < xsize / 4; i++) {
		LD_1x4_U8(s0, sp0);

		dd = buff[i];
		dd = vis_fpadd16(dd, vis_fmul8x16(vis_read_hi(s0), kk));
		buff[i] = val0;

		dd = vis_freg_pair(vis_fzeros(), vis_fpack16(dd));
		vis_st_u8(dd, dp + 3 * nchan);
		dd = vis_faligndata(dd, dd);
		vis_st_u8(dd, dp + 2 * nchan);
		dd = vis_faligndata(dd, dd);
		vis_st_u8(dd, dp + nchan);
		dd = vis_faligndata(dd, dd);
		vis_st_u8(dd, dp);

		sp0 += 4 * step;
		dp += 4 * nchan;
	}

	if (xsize & 3) {
		LD_1x4_U8(s0, sp0);

		dd = buff[i];
		dd = vis_fpadd16(dd, vis_fmul8x16(vis_read_hi(s0), kk));
		buff[i] = val0;

		dd = vis_freg_pair(vis_fzeros(), vis_fpack16(dd));
		dd = vis_faligndata(dd, dd);

		if ((xsize & 3) >= 3)
			vis_st_u8(dd, dp + 2 * nchan);
		dd = vis_faligndata(dd, dd);

		if ((xsize & 3) >= 2)
			vis_st_u8(dd, dp + nchan);
		dd = vis_faligndata(dd, dd);
		vis_st_u8(dd, dp);
	}
}

/* *********************************************************** */

static void FS_FUNC(2x2i_4) (
    PARAMS)
{
	FTYPE dd;
	FTYPE kk;
	mlib_s32 iker0 = pkk[0];
	mlib_s32 i;

	kk = vis_to_double_dup((iker0 & 0xFFFF0000) | ((iker0 >> 16) & 0xFFFF));

#pragma pipeloop(0)
	for (i = 0; i < xsize; i++) {
		mlib_u8 *sp0a = sp0 + off;
		mlib_u8 *sp1a = sp1 + off;

		dd = buff[i];
		dd = vis_fpadd16(dd, vis_fmul8x16(*(mlib_f32 *)sp0, kk));
		dd = vis_fpadd16(dd, vis_fmul8x16(*(mlib_f32 *)sp0a, kk));
		dd = vis_fpadd16(dd, vis_fmul8x16(*(mlib_f32 *)sp1, kk));
		dd = vis_fpadd16(dd, vis_fmul8x16(*(mlib_f32 *)sp1a, kk));
		buff[i] = dd;

		sp0 += step;
		sp1 += step;
	}
}

/* *********************************************************** */

static void FS_FUNC(2x2l_4) (
    PARAMS1)
{
	FTYPE dd;
	FTYPE val0 = pkk[4], kk;
	mlib_s32 iker0 = pkk[0];
	mlib_s32 i;

	kk = vis_to_double_dup((iker0 & 0xFFFF0000) | ((iker0 >> 16) & 0xFFFF));

#pragma pipeloop(0)
	for (i = 0; i < xsize; i++) {
		mlib_u8 *sp0a = sp0 + off;
		mlib_u8 *sp1a = sp1 + off;

		dd = buff[i];
		dd = vis_fpadd16(dd, vis_fmul8x16(*(mlib_f32 *)sp0, kk));
		dd = vis_fpadd16(dd, vis_fmul8x16(*(mlib_f32 *)sp0a, kk));
		dd = vis_fpadd16(dd, vis_fmul8x16(*(mlib_f32 *)sp1, kk));
		dd = vis_fpadd16(dd, vis_fmul8x16(*(mlib_f32 *)sp1a, kk));
		buff[i] = val0;

		*(mlib_f32 *)dp = vis_fpack16(dd);

		sp0 += step;
		sp1 += step;
		dp += 4;
	}
}

/* *********************************************************** */

static void FS_FUNC(1x1l_4) (
    PARAMS1)
{
	FTYPE dd;
	FTYPE val0 = pkk[4], kk;
	mlib_s32 iker0 = pkk[0] * 4.0;
	mlib_s32 i;

	kk = vis_to_double_dup((iker0 & 0xFFFF0000) | ((iker0 >> 16) & 0xFFFF));

#pragma pipeloop(0)
	for (i = 0; i < xsize; i++) {
		dd = buff[i];
		dd = vis_fpadd16(dd, vis_fmul8x16(*(mlib_f32 *)sp0, kk));
		buff[i] = val0;

		*(mlib_f32 *)dp = vis_fpack16(dd);

		sp0 += step;
		dp += 4;
	}
}

/* *********************************************************** */

mlib_status FS_FUNC(1)  (
    mlib_image *dst,
    const mlib_image *src,
    mlib_s32 scaleX,
    mlib_s32 scaleY,
    mlib_s32 transX,
    mlib_s32 transY,
    const mlib_d64 *hKernel,
    const mlib_d64 *vKernel,
    mlib_s32 iscale,
    mlib_s32 hSize,
    mlib_s32 vSize,
    mlib_s32 hParity,
    mlib_s32 vParity,
    mlib_edge edge)
{
	STYPE *psrc, *pdst, *sl, *slk, *dl;
	mlib_s32 slb, dlb, swidth, sheight, dwidth, dheight, nchan;
	mlib_s32 kw = 2 * hSize - hParity;
	mlib_s32 kh = 2 * vSize - vParity;
	mlib_s32 step, stepRight, stepDown;
	mlib_s32 rnd_val = 1 << (6 - iscale);
	FTYPE *buff, kk[5], val0 = vis_to_double_dup(rnd_val | (rnd_val << 16));
	mlib_s32 x_min, x_max, y_min, y_max, xsize, bsize;
	mlib_s32 i, j, k, ix, iy, flag4c = 0;

	vis_write_gsr((iscale << 3) | 7);
	kk[4] = val0;

	dwidth = mlib_ImageGetWidth(dst);
	dheight = mlib_ImageGetHeight(dst);
	dlb = mlib_ImageGetStride(dst);
	pdst = (void *)mlib_ImageGetData(dst);

	nchan = mlib_ImageGetChannels(src);
	swidth = mlib_ImageGetWidth(src);
	sheight = mlib_ImageGetHeight(src);
	slb = mlib_ImageGetStride(src);
	psrc = (void *)mlib_ImageGetData(src);

	slb /= sizeof (STYPE);
	dlb /= sizeof (STYPE);

	step = scaleX * nchan;
	stepRight = (kw - 1) * nchan;
	stepDown = (kh - 1) * slb;

/* 0.5*scaleX - 0.5*(kw-1) */
	transX += (scaleX - (kw - 1)) >> 1;
/* 0.5*scaleY - 0.5*(kh-1) */
	transY += (scaleY - (kh - 1)) >> 1;

	x_min = scaleX - 1 - transX;
	y_min = scaleY - 1 - transY;
	x_max = swidth - kw - transX;
	y_max = sheight - kh - transY;

	if (x_min >= 0)
		x_min /= scaleX;
	else
		x_min = 0;
	if (y_min >= 0)
		y_min /= scaleY;
	else
		y_min = 0;
	if (x_max >= 0)
		x_max /= scaleX;
	else
		x_max = -1;
	if (y_max >= 0)
		y_max /= scaleY;
	else
		y_max = -1;
	if (x_max >= dwidth)
		x_max = dwidth - 1;
	if (y_max >= dheight)
		y_max = dheight - 1;

	xsize = x_max - x_min + 1;

	if (xsize <= 0)
		return (MLIB_SUCCESS);

/* if (nchan == 3 && (((mlib_addr)pdst | dlb) & 3) == 0) flag4c = 1; */

	if (nchan == 4 &&
	    (((mlib_addr)psrc | (mlib_addr)pdst | slb | dlb) & 3) == 0)
		flag4c = 1;
	bsize = (flag4c) ? xsize : ((xsize + 3) / 4);
	buff = __mlib_malloc(sizeof (FTYPE) * bsize);

	if (buff == NULL)
		return (MLIB_FAILURE);

#pragma pipeloop(0)
	for (i = 0; i < bsize; i++) {
		buff[i] = val0;
	}

	sl = psrc + (y_min * scaleY + transY) * slb + (x_min * scaleX +
	    transX) * nchan;
	dl = pdst + y_min * dlb + x_min * nchan;

	for (j = y_min; j <= y_max; j++) {
		slk = sl;

		for (k = 0; k < nchan; k++) {
			STYPE *sl0 = slk;
			STYPE *sl1 = slk + stepDown;

			if (flag4c && k)
				continue;

			for (iy = vSize - 1; iy >= 0; iy--) {
				mlib_s32 off = stepRight;
				STYPE *sp0 = sl0;
				STYPE *sp1 = sl1;

				for (ix = hSize - 1; ix >= 0; ix--) {
					mlib_s32 flagl = 0;
					mlib_d64 dscale = DSCALE;

					if (off == 0)
						dscale *= 0.5;
					if (sp0 == sp1)
						dscale *= 0.5;

					if (iy == 0 && ix == 0)
						flagl = 1;

					kk[0] =
					    hKernel[ix] * vKernel[iy] * dscale;
					kk[0] += (kk[0] > 0) ? 0.5 : -0.5;

					if (!flag4c) {
						if (!flagl) {
							FS_FUNC(2x2i) (buff,
							    sp0, sp1, kk, xsize,
							    off, step, nchan);
						} else {

							if (off == 0 &&
							    sp0 == sp1) {
								FS_FUNC(1x1l)
								    (buff,
								    sp0, sp1,
								    kk, xsize,
								    off, step,
								    nchan,
								    dl + k);
							} else {
								FS_FUNC(2x2l)
								    (buff,
								    sp0, sp1,
								    kk, xsize,
								    off, step,
								    nchan,
								    dl + k);
							}
						}

					} else {

						if (!flagl) {
							FS_FUNC(2x2i_4) (buff,
							    sp0, sp1, kk, xsize,
							    off, step, nchan);
						} else {

							if (off == 0 &&
							    sp0 == sp1) {
								FS_FUNC(1x1l_4)
								    (buff, sp0,
								    sp1, kk,
								    xsize, off,
								    step, nchan,
								    dl + k);
							} else {
								FS_FUNC(2x2l_4)
								    (buff, sp0,
								    sp1, kk,
								    xsize, off,
								    step, nchan,
								    dl + k);
							}
						}
					}

					sp0 += nchan;
					sp1 += nchan;
					off -= 2 * nchan;
				}

				sl0 += slb;
				sl1 -= slb;
			}

			slk++;
		}

		sl += scaleY * slb;
		dl += dlb;
	}

	__mlib_free(buff);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
