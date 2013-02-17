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

#pragma ident	"@(#)mlib_GraphicsDrawLine_Z.c	9.2	07/10/09 SMI"

/*
 *  FUNCTION
 *    mlib_GraphicsDrawLine_Z_[8|32] - draw line with Z buffering
 *
 *  SYNOPSIS
 *    mlib_status mlib_GraphicsDrawLine_Z_[8|32]
 *                                      (mlib_image *buffer,
 *                                       mlib_image *zbuffer,
 *                                       mlib_s16   x1,
 *                                       mlib_s16   y1,
 *                                       mlib_s16   z1,
 *                                       mlib_s16   x2,
 *                                       mlib_s16   y2,
 *                                       mlib_s16   z2,
 *                                       mlib_s32   c);
 *  ARGUMENTS
 *    buffer  Pointer to the image which the function is drawing into.
 *    zbuffer Pointer to the image which holds the Z buffer.
 *    x1      X coordinate of the first point.
 *    y1      Y coordinate of the first point.
 *    z1      Z coordinate of the first point.
 *    x2      X coordinate of the second point.
 *    y2      Y coordinate of the second point.
 *    z2      Z coordinate of the second point.
 *    c       Color used in drawing.
 *
 *  DESCRIPTION
 *    The drawable buffer is either MLIB_BYTE(8-bit) or MLIB_INT(32-bit)
 *    type single channel mediaLib image. Pixels fall outside the
 *    drawable buffer will be clipped and discarded.
 *    All colors are specified in 32-bit format. For 8-bit buffer, only
 *    the bits 0-7 are used and the bits 8-23 should be zero. For 32-bit
 *    frame buffer, the bits 24-31 are reserved for alpha channel and
 *    should be 0xff.
 *    In Z buffering, the Z value of any point inside a polygon is
 *    determined by linear interpolation of vertex Z value along each
 *    edge and then between edges along each scanline.
 */

/*
 *  FUNCTION
 *    mlib_GraphicsDrawLine_BZ_[8|32] - draw line with Z buffering
 *					and blending
 *
 *  SYNOPSIS
 *    mlib_status mlib_GraphicsDrawLine_BZ_[8|32]
 *                                      (mlib_image *buffer,
 *                                       mlib_image *zbuffer,
 *                                       mlib_s16   x1,
 *                                       mlib_s16   y1,
 *                                       mlib_s16   z1,
 *                                       mlib_s16   x2,
 *                                       mlib_s16   y2,
 *                                       mlib_s16   z2,
 *                                       mlib_s32   c,
 *                                       mlib_s32   a);
 *  ARGUMENTS
 *    buffer  Pointer to the image which the function is drawing into.
 *    zbuffer Pointer to the image which holds the Z buffer.
 *    x1      X coordinate of the first point.
 *    y1      Y coordinate of the first point.
 *    z1      Z coordinate of the first point.
 *    x2      X coordinate of the second point.
 *    y2      Y coordinate of the second point.
 *    z2      Z coordinate of the second point.
 *    c       Color used in the drawing.
 *    a       Alpha value for the blending.
 *
 *  DESCRIPTION
 *    The drawable buffer is either MLIB_BYTE (8-bit) or MLIB_INT (32-bit)
 *    type single channel mediaLib image. The Z buffer is a MLIB_SHORT type
 *    single channel mediaLib image and must have the same width and height
 *    as the drawable buffer.
 *
 *    Pixels fall outside the drawable buffer will be clipped and discarded.
 *
 *    All colors are specified in 32-bit format. For 8-bit buffer, only
 *    the bits 0-7 are used and the bits 8-23 should be zero.
 *
 *    The Z value of any point inside a polygon is determined by linear
 *    interpolation of vertex Z value along each edge and then between
 *    edges along each scanline.
 *
 *    In alpha value, only the bits 0-7 are used and the bits 8-23 should
 *    be zero.  For each drawable pixel, the original pixel value is
 *    blended with the drawing color to produce the final pixel value
 *
 *        d = (s * (255 - a) + c * a) / 255
 *
 *    where c is the color used in the drawing, a is the alpha value, s
 *    is the original value of the pixel, and d is the final value of the
 *    pixel.
 */

#include <mlib_graphics.h>
#include <mlib_GraphicsDrawSet.h>
#include <mlib_GraphicsMacro.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_GraphicsDrawLine_Z_8 = __mlib_GraphicsDrawLine_Z_8
#pragma weak mlib_GraphicsDrawLine_Z_32 = __mlib_GraphicsDrawLine_Z_32
#pragma weak mlib_GraphicsDrawLine_BZ_8 = __mlib_GraphicsDrawLine_BZ_8
#pragma weak mlib_GraphicsDrawLine_BZ_32 = __mlib_GraphicsDrawLine_BZ_32

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_GraphicsDrawLine_Z_8) mlib_GraphicsDrawLine_Z_8
	__attribute__((weak, alias("__mlib_GraphicsDrawLine_Z_8")));
__typeof__(__mlib_GraphicsDrawLine_Z_32) mlib_GraphicsDrawLine_Z_32
	__attribute__((weak, alias("__mlib_GraphicsDrawLine_Z_32")));
__typeof__(__mlib_GraphicsDrawLine_BZ_8) mlib_GraphicsDrawLine_BZ_8
	__attribute__((weak, alias("__mlib_GraphicsDrawLine_BZ_8")));
__typeof__(__mlib_GraphicsDrawLine_BZ_32) mlib_GraphicsDrawLine_BZ_32
	__attribute__((weak, alias("__mlib_GraphicsDrawLine_BZ_32")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

void
mlib_GraphicsClipLine_Z(
	mlib_s32 *coords,
	mlib_s32 width,
	mlib_s32 height,
	mlib_s16 *x,
	mlib_s16 *y,
	mlib_s16 *z)
{
	const mlib_s32 half = -MLIB_HALF - 1;
	mlib_s32 *pa = coords;
	mlib_s32 x0 = x[0], x1 = x[1], y0 = y[0], y1 = y[1];
	mlib_s32 z0 = z[0], z1 = z[1];
	mlib_s32 clip, zz, adx, ady, sl, ll;

	y1 = y1 - y0;
	x1 = x1 - x0;
	z1 = z1 - z0;
	zz = y1 >> 31;
	ady = (zz & -y1) | (y1 & ~zz);
	zz = x1 >> 31;
	adx = (zz & -x1) | (x1 & ~zz);
	pa[MLIB_GDS_STARTX] = x0;
	pa[MLIB_GDS_STARTY] = y0;
	pa[MLIB_GDS_STEPZ] = z1;

	if (adx > ady) {
		sl = y1;
		ll = adx;

		if (x1 < 0) {
			pa[MLIB_GDS_STARTX] = x0 + x1;
			pa[MLIB_GDS_STARTY] = half - ((y0 + y1) << MLIB_SHIFT);
			pa[MLIB_GDS_STARTZ] = half - ((z0 + z1) << MLIB_SHIFT);
			pa[MLIB_GDS_STEPZ] = -z1;
			sl = -y1;
		} else {
			pa[MLIB_GDS_STARTY] = half - (y0 << MLIB_SHIFT);
			pa[MLIB_GDS_STARTZ] = half - (z0 << MLIB_SHIFT);
		}
	} else {
		sl = x1;
		ll = ady;

		if (y1 < 0) {
			pa[MLIB_GDS_STARTX] = half - ((x0 + x1) << MLIB_SHIFT);
			pa[MLIB_GDS_STARTY] = y0 + y1;
			pa[MLIB_GDS_STARTZ] = half - ((z0 + z1) << MLIB_SHIFT);
			pa[MLIB_GDS_STEPZ] = -z1;
			sl = -x1;
		} else {
			pa[MLIB_GDS_STARTX] = half - (x0 << MLIB_SHIFT);
			pa[MLIB_GDS_STARTZ] = half - (z0 << MLIB_SHIFT);
		}
	}

	pa[MLIB_GDS_LENGTHLINES] = ll;
	pa[MLIB_GDS_STEPLINES] = sl;

	{
		mlib_f32 f_mul0, f_div0, zf_mul0;

		f_mul0 = pa[MLIB_GDS_STEPLINES];
		ll = pa[MLIB_GDS_LENGTHLINES];
		zf_mul0 = pa[MLIB_GDS_STEPZ];
		f_div0 = (ll <
			MLIB_STEP_TABLE_LENGTH) ? mlib_GraphicsStepTable_f32[ll]
			: MLIB_ONE_FL / ll;
		pa[MLIB_GDS_STEPLINES] = f_mul0 * f_div0;
		pa[MLIB_GDS_STEPZ] = zf_mul0 * f_div0;
	}

	x1 = x[1];
	y1 = y[1];

	clip = (((width - x0) | (height - y0) | (width - x1) | (height -
		y1)) | (x0 | y0 | x1 | y1)) >> 31;

	if (clip) {
		const mlib_f32 f_one = 1.f, f_zero = 0.f;
		mlib_f32 f_x0, f_y0, f_x1, f_y1, f_tB, f_tE, f_ll, f_sl;
		mlib_f32 f_z0;
		mlib_f32 f_w, f_h, f_div0, zf_mul0;
		mlib_s32 startx, starty;

		x0 = pa[MLIB_GDS_STARTX] + 1;

		if ((x0 & (MLIB_ONE - 1)) != (MLIB_HALF)) {
			f_w = width;
			f_h = (height + 1) * MLIB_ONE_FL - f_one;
			startx = MLIB_GDS_STARTX;
			starty = MLIB_GDS_STARTY;
		} else {
			f_w = height;
			f_h = (width + 1) * MLIB_ONE_FL - f_one;
			startx = MLIB_GDS_STARTY;
			starty = MLIB_GDS_STARTX;
		}

		f_x0 = pa[startx];
		f_y0 = pa[starty];
		f_z0 = -f_one - (mlib_f32)(pa[MLIB_GDS_STARTZ]);
		f_ll = pa[MLIB_GDS_LENGTHLINES];
		f_div0 = pa[MLIB_GDS_STEPLINES];
		zf_mul0 = pa[MLIB_GDS_STEPZ];
		pa[MLIB_GDS_LENGTHLINES] = -1;

		f_sl = f_div0;
		f_y0 = -f_one - f_y0;
		f_x1 = f_x0 + f_ll;
		f_y1 = (f_sl == f_zero) ? f_y0 : f_y0 + f_div0 * f_ll;

		f_tB = f_y0;
		f_tE = f_y1;
		f_tB = (f_tB < f_zero) ? f_zero : f_tB;
		f_tE = (f_tE < f_zero) ? f_zero : f_tE;
		f_tB = (f_tB > f_h) ? f_h : f_tB;
		f_tE = (f_tE > f_h) ? f_h : f_tE;

		if ((f_sl != f_zero) || (f_tB == f_y0)) {
			if ((f_sl != f_zero) && (f_ll != f_zero)) {
				f_tB = f_x0 + (f_one / f_div0) * (f_tB - f_y0);
				f_tE = f_x1 + (f_one / f_div0) * (f_tE - f_y1);
				f_tB = (f_tB > f_x0) ? f_tB : f_x0;
				f_tE = (f_tE < f_x1) ? f_tE : f_x1;
			} else {
				f_y0 = f_tB;
				f_y1 = f_tE;
				f_tB = f_x0;
				f_tE = f_x1;
			}

			f_tB = (f_tB < f_zero) ? f_zero : f_tB;
			f_tE = (f_tE > f_w) ? f_w : f_tE;

			if (f_tB <= f_tE) {
				pa[startx] = f_tB;
				pa[MLIB_GDS_LENGTHLINES] = f_tE;
				pa[startx] += ((pa[startx] == f_tB) ? 0 : 1);
				pa[MLIB_GDS_LENGTHLINES] =
					pa[MLIB_GDS_LENGTHLINES] - pa[startx];

				if ((f_sl != f_zero) && (f_ll != f_zero)) {
					f_tB = pa[startx];
					pa[starty] =
						-(f_y0 + f_div0 * (f_tB -
						f_x0)) - f_one;
					pa[MLIB_GDS_STARTZ] =
						-(f_z0 + zf_mul0 * (f_tB -
						f_x0)) - f_one;
				} else {
					pa[starty] = -f_y0 - f_one;
					pa[MLIB_GDS_STARTZ] =
						-(f_z0 + zf_mul0 * (f_tB -
						f_x0)) - f_one;
				}
			}
		}
	}
}

/* *********************************************************** */

#define	MLIB_GRAPHICS_LINE_INNER_Z                              \
	mlib_s32 coords[MLIB_GDS_NEXT_GZ];                      \
	mlib_s16 x[2], y[2], z[2];                              \
	mlib_d64 d_c;                                           \
	                                                        \
	x[0] = xx1;                                             \
	x[1] = xx2;                                             \
	y[0] = yy1;                                             \
	y[1] = yy2;                                             \
	z[0] = zz1;                                             \
	z[1] = zz2

/* *********************************************************** */

mlib_status
__mlib_GraphicsDrawLine_Z_8(
	mlib_image *buffer,
	mlib_image *zbuffer,
	mlib_s16 xx1,
	mlib_s16 yy1,
	mlib_s16 zz1,
	mlib_s16 xx2,
	mlib_s16 yy2,
	mlib_s16 zz2,
	mlib_s32 c)
{
	MLIB_GRAPHICS_DECL_VARZ;

	MLIB_GRAPHICS_LINE_INNER_Z;

	MLIB_GRAPHICS_COLOR_8(c);
	MLIB_GRAPHICS_TO_DOUBLE(d_c, c);
	MLIB_GRAPHICS_CHECKZ(MLIB_NOTHING, MLIB_NOTHING);

	mlib_GraphicsClipLine_Z(coords, width, height, x, y, z);
	mlib_GraphicsSpamLineSet_Z_8(rtable, zrtable, stride, zstride, coords,
		coords + MLIB_GDS_NEXT_GZ, c, d_c);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_GraphicsDrawLine_Z_32(
	mlib_image *buffer,
	mlib_image *zbuffer,
	mlib_s16 xx1,
	mlib_s16 yy1,
	mlib_s16 zz1,
	mlib_s16 xx2,
	mlib_s16 yy2,
	mlib_s16 zz2,
	mlib_s32 c)
{
	MLIB_GRAPHICS_DECL_VARZ;

	MLIB_GRAPHICS_LINE_INNER_Z;

	MLIB_GRAPHICS_COLOR_32(c);
	MLIB_GRAPHICS_TO_DOUBLE(d_c, c);
	MLIB_GRAPHICS_CHECKZ(MLIB_NOTHING, MLIB_NOTHING);

	mlib_GraphicsClipLine_Z(coords, width, height, x, y, z);
	mlib_GraphicsSpamLineSet_Z_32(rtable, zrtable, stride, zstride, coords,
		coords + MLIB_GDS_NEXT_GZ, c, d_c);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_GraphicsDrawLine_BZ_8(
	mlib_image *buffer,
	mlib_image *zbuffer,
	mlib_s16 xx1,
	mlib_s16 yy1,
	mlib_s16 zz1,
	mlib_s16 xx2,
	mlib_s16 yy2,
	mlib_s16 zz2,
	mlib_s32 c,
	mlib_s32 a)
{
	MLIB_GRAPHICS_DECL_VARZ;

	MLIB_GRAPHICS_LINE_INNER_Z;

	MLIB_GRAPHICS_COLOR_8(c);
	MLIB_GRAPHICS_TO_DOUBLE(d_c, c);
	MLIB_GRAPHICS_CHECKZ(MLIB_NOTHING, MLIB_NOTHING);

	mlib_GraphicsClipLine_Z(coords, width, height, x, y, z);
	mlib_GraphicsSpamLineSet_BZ_8(rtable, zrtable, stride, zstride, coords,
		coords + MLIB_GDS_NEXT_GZ, c, a, d_c);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_GraphicsDrawLine_BZ_32(
	mlib_image *buffer,
	mlib_image *zbuffer,
	mlib_s16 xx1,
	mlib_s16 yy1,
	mlib_s16 zz1,
	mlib_s16 xx2,
	mlib_s16 yy2,
	mlib_s16 zz2,
	mlib_s32 c,
	mlib_s32 a)
{
	MLIB_GRAPHICS_DECL_VARZ;

	MLIB_GRAPHICS_LINE_INNER_Z;

	MLIB_GRAPHICS_COLOR_32(c);
	MLIB_GRAPHICS_TO_DOUBLE(d_c, c);
	MLIB_GRAPHICS_CHECKZ(MLIB_NOTHING, MLIB_NOTHING);

	mlib_GraphicsClipLine_Z(coords, width, height, x, y, z);
	mlib_GraphicsSpamLineSet_BZ_32(rtable, zrtable, stride, zstride, coords,
		coords + MLIB_GDS_NEXT_GZ, c, a, d_c);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
