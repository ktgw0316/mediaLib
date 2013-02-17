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

#pragma ident	"@(#)mlib_v_ImageMulAlphaFunc.c	9.2	07/11/05 SMI"

/*
 *  Internal function for mlib_ImageMulAlpha
 */

#include <vis_proto.h>
#include <mlib_image.h>
#include <mlib_v_ImageMulAlpha.h>

/* *********************************************************** */

static void mlib_ImageMulAlpha_S32_2(
    mlib_s32 *src,
    mlib_s32 s_stride,
    mlib_s32 *dst,
    mlib_s32 d_stride,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s32 alpha);

static void mlib_ImageMulAlpha_S32_3(
    mlib_s32 *src,
    mlib_s32 s_stride,
    mlib_s32 *dst,
    mlib_s32 d_stride,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s32 alpha);

static void mlib_ImageMulAlpha_S32_4(
    mlib_s32 *src,
    mlib_s32 s_stride,
    mlib_s32 *dst,
    mlib_s32 d_stride,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s32 alpha);

/* *********************************************************** */

void
mlib_ImageMulAlpha_S32_2(
    mlib_s32 *src,
    mlib_s32 s_stride,
    mlib_s32 *dst,
    mlib_s32 d_stride,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s32 alpha)
{
	mlib_s32 i, j, d_0, d_1;
	mlib_d64 shift_const = 1. / 2147483648.;

	s_stride -= width << 1;
	d_stride -= width << 1;

	if (alpha == 0) {
		for (i = 0; i < height; i++) {
#pragma pipeloop(0)
			for (j = 0; j < width; j++) {
				d_0 = src[0];
				d_1 =
				    (mlib_s32)((mlib_d64)src[1] *
				    (mlib_d64)d_0 * shift_const);
				dst[0] = d_0;
				dst[1] = d_1;
				src += 2;
				dst += 2;
			}

			src += s_stride;
			dst += d_stride;
		}

		return;
	}

	for (i = 0; i < height; i++) {
#pragma pipeloop(0)
		for (j = 0; j < width; j++) {
			d_0 = src[1];
			d_1 =
			    (mlib_s32)((mlib_d64)src[0] * (mlib_d64)d_0 *
			    shift_const);
			dst[0] = d_1;
			dst[1] = d_0;
			src += 2;
			dst += 2;
		}

		src += s_stride;
		dst += d_stride;
	}
}

/* *********************************************************** */

void
mlib_ImageMulAlpha_S32_3(
    mlib_s32 *src,
    mlib_s32 s_stride,
    mlib_s32 *dst,
    mlib_s32 d_stride,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s32 alpha)
{
	mlib_s32 i, j, d_0, d_1, d_2;
	mlib_d64 a;
	mlib_d64 shift_const = 1. / 2147483648.;

	s_stride -= width + (width << 1);
	d_stride -= width + (width << 1);

	if (alpha == 0) {
		for (i = 0; i < height; i++) {
#pragma pipeloop(0)
			for (j = 0; j < width; j++) {
				d_2 = src[0];
				a = (mlib_d64)d_2;
				d_0 =
				    (mlib_s32)((mlib_d64)src[1] * a *
				    shift_const);
				d_1 =
				    (mlib_s32)((mlib_d64)src[2] * a *
				    shift_const);
				dst[0] = d_2;
				dst[1] = d_0;
				dst[2] = d_1;
				src += 3;
				dst += 3;
			}

			src += s_stride;
			dst += d_stride;
		}

		return;
	}

	if (alpha == 1) {
		for (i = 0; i < height; i++) {
#pragma pipeloop(0)
			for (j = 0; j < width; j++) {
				d_2 = src[1];
				a = (mlib_d64)d_2;
				d_0 =
				    (mlib_s32)((mlib_d64)src[0] * a *
				    shift_const);
				d_1 =
				    (mlib_s32)((mlib_d64)src[2] * a *
				    shift_const);
				dst[0] = d_0;
				dst[1] = d_2;
				dst[2] = d_1;
				src += 3;
				dst += 3;
			}

			src += s_stride;
			dst += d_stride;
		}

		return;
	}

	for (i = 0; i < height; i++) {
#pragma pipeloop(0)
		for (j = 0; j < width; j++) {
			d_2 = src[2];
			a = (mlib_d64)d_2;
			d_0 = (mlib_s32)((mlib_d64)src[0] * a * shift_const);
			d_1 = (mlib_s32)((mlib_d64)src[1] * a * shift_const);
			dst[0] = d_0;
			dst[1] = d_1;
			dst[2] = d_2;
			src += 3;
			dst += 3;
		}

		src += s_stride;
		dst += d_stride;
	}
}

/* *********************************************************** */

void
mlib_ImageMulAlpha_S32_4(
    mlib_s32 *src,
    mlib_s32 s_stride,
    mlib_s32 *dst,
    mlib_s32 d_stride,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s32 alpha)
{
	mlib_s32 i, j;
	mlib_s32 d_0, d_1, d_2, d_3;
	mlib_d64 a;
	mlib_d64 shift_const = 1. / 2147483648.;

	s_stride -= width << 2;
	d_stride -= width << 2;

	if (alpha == 0) {
		for (i = 0; i < height; i++) {
#pragma pipeloop(0)
			for (j = 0; j < width; j++) {
				d_3 = src[0];
				a = (mlib_d64)d_3;
				d_0 =
				    (mlib_s32)((mlib_d64)src[1] * a *
				    shift_const);
				d_1 =
				    (mlib_s32)((mlib_d64)src[2] * a *
				    shift_const);
				d_2 =
				    (mlib_s32)((mlib_d64)src[3] * a *
				    shift_const);
				dst[0] = d_3;
				dst[1] = d_0;
				dst[2] = d_1;
				dst[3] = d_2;
				src += 4;
				dst += 4;
			}

			src += s_stride;
			dst += d_stride;
		}

		return;
	}

	if (alpha == 1) {
		for (i = 0; i < height; i++) {
#pragma pipeloop(0)
			for (j = 0; j < width; j++) {
				d_3 = src[1];
				a = (mlib_d64)d_3;
				d_0 =
				    (mlib_s32)((mlib_d64)src[0] * a *
				    shift_const);
				d_1 =
				    (mlib_s32)((mlib_d64)src[2] * a *
				    shift_const);
				d_2 =
				    (mlib_s32)((mlib_d64)src[3] * a *
				    shift_const);
				dst[0] = d_0;
				dst[1] = d_3;
				dst[2] = d_1;
				dst[3] = d_2;
				src += 4;
				dst += 4;
			}

			src += s_stride;
			dst += d_stride;
		}

		return;
	}

	if (alpha == 2) {
		for (i = 0; i < height; i++) {
#pragma pipeloop(0)
			for (j = 0; j < width; j++) {
				d_3 = src[2];
				a = (mlib_d64)d_3;
				d_0 =
				    (mlib_s32)((mlib_d64)src[0] * a *
				    shift_const);
				d_1 =
				    (mlib_s32)((mlib_d64)src[1] * a *
				    shift_const);
				d_2 =
				    (mlib_s32)((mlib_d64)src[3] * a *
				    shift_const);
				dst[0] = d_0;
				dst[1] = d_1;
				dst[2] = d_3;
				dst[3] = d_2;
				src += 4;
				dst += 4;
			}

			src += s_stride;
			dst += d_stride;
		}

		return;
	}

	for (i = 0; i < height; i++) {
#pragma pipeloop(0)
		for (j = 0; j < width; j++) {
			d_3 = src[3];
			a = (mlib_d64)d_3;
			d_0 = (mlib_s32)((mlib_d64)src[0] * a * shift_const);
			d_1 = (mlib_s32)((mlib_d64)src[1] * a * shift_const);
			d_2 = (mlib_s32)((mlib_d64)src[2] * a * shift_const);
			dst[0] = d_0;
			dst[1] = d_1;
			dst[2] = d_2;
			dst[3] = d_3;
			src += 4;
			dst += 4;
		}

		src += s_stride;
		dst += d_stride;
	}
}

/* *********************************************************** */

mlib_status
mlib_ImageMulAlpha_S32(
    mlib_s32 *sl,
    mlib_s32 *dl,
    mlib_s32 sstride,
    mlib_s32 dstride,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s32 channel,
    mlib_s32 alpha)
{
	switch (channel) {
	case 2:
		mlib_ImageMulAlpha_S32_2(sl, sstride, dl, dstride, width,
		    height, alpha);
		return (MLIB_SUCCESS);
	case 3:
		mlib_ImageMulAlpha_S32_3(sl, sstride, dl, dstride, width,
		    height, alpha);
		return (MLIB_SUCCESS);
	case 4:
		mlib_ImageMulAlpha_S32_4(sl, sstride, dl, dstride, width,
		    height, alpha);
		return (MLIB_SUCCESS);
	}

	return (MLIB_FAILURE);
}

/* *********************************************************** */
