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

#pragma ident	"@(#)mlib_GraphicsClipLineSet_B.c	9.2	07/10/09 SMI"

/*
 *  FUNCTIONS
 *    mlib_GraphicsClipLineSet_B    - clipping function for alpha-blending mode
 *
 *  SYNOPSIS
 *    void  mlib_GraphicsClipLineSet_B
 *                                    (mlib_u8  **rtable,
 *                                     mlib_s32 width,
 *                                     mlib_s32 height,
 *                                     mlib_s32 stride,
 *                                     mlib_s16 *x,
 *                                     mlib_s16 *y,
 *                                     mlib_s32 nlines,
 *                                     mlib_s32 dp0,
 *                                     mlib_s32 dp1,
 *                                     mlib_s32 c,
 *                                     mlib_s32 alp);
 *  ARGUMENTS
 *    rtable      Pointer to array of starts of row, rtable[-1][-1] is buffer
 *                  for unwanted stores.
 *    width       Image.Width - 1, used to check whether clipping is needed.
 *    height      Image.Height - 1, used to check whether clipping is needed.
 *    stride      Stride of the image.
 *    x           Pointer to array of X coordinate of the points.
 *    y           Pointer to array of Y coordinate of the points.
 *    nlines      Number of lines.
 *    dp0         Step from (x0, y0) to the (x0, y0) of the next line
 *    dp1         Step from (x1, y1) to the (x1, y1) of the next line,
 *                for example
 *                  DrawLineSet       dp0 = 2, dp1 = 2
 *                  DrawLineFanSet    dp0 = 0, dp1 = 1
 *                  DrawLineStripSet  dp0 = 1, dp1 = 1
 *    c           Color used in the drawing.
 *    alp         Alpha value for the blending.
 *
 *  DESCRIPTION
 *    The drawable buffer is either MLIB_BYTE (8-bit) or MLIB_INT (32-bit)
 *    type single channel mediaLib image.
 *
 *    Pixels falling outside the drawable buffer will be clipped and discarded.
 *
 *    All colors are specified in 32-bit format. For 8-bit buffer, only
 *    the bits 0-7 are used and the bits 8-23 should be zero.
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
#include <mlib_GraphicsBlend.h>

/* *********************************************************** */

#define	NB	512

/* *********************************************************** */

void
mlib_GraphicsClipLineSet_B_8(
	mlib_u8 **rtable,
	mlib_s32 width,
	mlib_s32 height,
	mlib_s32 stride,
	mlib_s16 *x,
	mlib_s16 *y,
	mlib_s32 nlines,
	mlib_s32 dp0,
	mlib_s32 dp1,
	mlib_s32 c,
	mlib_s32 alp)
{
	mlib_addr dxy;
	mlib_s32 a[MLIB_GDS_NEXT * (NB + 1)];
	mlib_s32 *pa, *ps, *pe;
	mlib_s16 *px0_ = x, *px1_ = x + 1, *px0, *px1;
	mlib_s32 i, count, x0, y0, x1, y1, alllines;
	mlib_s32 zz, adx, ady, clip, clipexist, half = -MLIB_HALF - 1;
	mlib_s32 ll0, ll1, ll2, ll3, sl = 0;
	mlib_f32 f_div0, f_mul0;
	mlib_f32 f_div1, f_mul1;
	mlib_f32 f_div2, f_mul2;
	mlib_f32 f_div3, f_mul3;
	mlib_f32 f_width, f_height, f_WIDTH, f_HEIGHT;
	mlib_f32 f_zero = 0.f, f_one = 1.f;
	mlib_s32 points[NB + 1], start = 1;
	mlib_s32 tx0, ty0, z0;
	mlib_u8 *data0;

	MLIB_GRAPHICS_DECL_VAR_B_8

	MLIB_GRAPHICS_SET_VAR_B_8(c, alp)

	f_width = width;
	f_height = height;
	f_WIDTH = (f_width + f_one) * MLIB_ONE_FL - f_one;
	f_HEIGHT = (f_height + f_one) * MLIB_ONE_FL - f_one;

	dxy = (mlib_addr)y - (mlib_addr)x;
	dp0 <<= 1;
	dp1 <<= 1;
	alllines = nlines;

	if (dp0 == 0) {  /* FanSet */
		tx0 = x[0];
		ty0 = y[0];
		z0 = ty0 | tx0 | (height - ty0) | (width - tx0);

		if (z0 >= 0) {
			data0 = (mlib_u8 *)rtable[ty0] + tx0;
			MLIB_GRAPHICS_BLEND2_8_PP(points, data0)
		}
	}

	while (alllines > 0) {

		nlines = (alllines > NB) ? NB : alllines;
		alllines -= nlines;
		pa = a + MLIB_GDS_NEXT;
		ps = a + MLIB_GDS_NEXT;
		px0 = px0_;
		px1 = px1_;

		clip = 0;
		clipexist = 0;
		pe = ps + MLIB_GDS_NEXT * nlines;

		if (dp0 == 2) {  /* StripSet */

			for (i = 1; pa < pe; pa += MLIB_GDS_NEXT, i++) {
				clipexist |= clip;
				x0 = *px0;
				y0 = *(mlib_s16 *)((mlib_u8 *)px0 + dxy);
				x1 = *px1;
				y1 = *(mlib_s16 *)((mlib_u8 *)px1 + dxy);

				z0 = (height - y1) | (width - x1);
				z0 = (z0 | y1 | x1) >> 31;

				px0 = (mlib_s16 *)((mlib_u8 *)px0 + dp0);
				px1 = (mlib_s16 *)((mlib_u8 *)px1 + dp1);
				clip = (width - x0) | (height - y0) | x0 | y0;
				clip = (clip | z0) >> 31;

				tx0 = x1 | z0;
				ty0 = y1 | z0;
				data0 = (mlib_u8 *)rtable[ty0] + tx0;
				MLIB_GRAPHICS_BLEND2_8_PP(points + i, data0)

				y1 = y1 - y0;
				x1 = x1 - x0;
				zz = y1 >> 31;
				ady = (zz & -y1) | (y1 & ~zz);
				zz = x1 >> 31;
				adx = (zz & -x1) | (x1 & ~zz);
				pa[MLIB_GDS_STEPLINES - MLIB_GDS_NEXT] = sl;
				pa[MLIB_GDS_STARTX] = x0;
				pa[MLIB_GDS_STARTY] = y0;

				if (adx > ady) {
					sl = y1;
					clip |= adx;

					if (x1 < 0) {
						pa[MLIB_GDS_STARTX] = x0 + x1;
						pa[MLIB_GDS_STARTY] = half -
						(y0 + y1 << MLIB_SHIFT);
						sl = -y1;
					} else
						pa[MLIB_GDS_STARTY] = half
							- (y0 << MLIB_SHIFT);
				} else {
					sl = x1;
					clip |= ady;

					if (y1 < 0) {
						pa[MLIB_GDS_STARTX] = half -
							(x0 + x1 << MLIB_SHIFT);
						pa[MLIB_GDS_STARTY] = y0 + y1;
						sl = -x1;
					} else
						pa[MLIB_GDS_STARTX] = half
							- (x0 << MLIB_SHIFT);
				}

				pa[MLIB_GDS_LENGTHLINES] = clip;
			}
		} else {
			for (; pa < pe; pa += MLIB_GDS_NEXT) {
				clipexist |= clip;
				x0 = *px0;
				y0 = *(mlib_s16 *)((mlib_u8 *)px0 + dxy);
				x1 = *px1;
				y1 = *(mlib_s16 *)((mlib_u8 *)px1 + dxy);
				px0 = (mlib_s16 *)((mlib_u8 *)px0 + dp0);
				px1 = (mlib_s16 *)((mlib_u8 *)px1 + dp1);
				clip = (((width - x0) | (height - y0) |
					(width - x1) | (height - y1)) |
					(x0 | y0 | x1 | y1)) >> 31;
				y1 = y1 - y0;
				x1 = x1 - x0;
				zz = y1 >> 31;
				ady = (zz & -y1) | (y1 & ~zz);
				zz = x1 >> 31;
				adx = (zz & -x1) | (x1 & ~zz);
				pa[MLIB_GDS_STEPLINES - MLIB_GDS_NEXT] = sl;
				pa[MLIB_GDS_STARTX] = x0;
				pa[MLIB_GDS_STARTY] = y0;

				if (adx > ady) {
					sl = y1;
					clip |= adx;

					if (x1 < 0) {
						pa[MLIB_GDS_STARTX] = x0 + x1;
						pa[MLIB_GDS_STARTY] = half -
							(y0 + y1 << MLIB_SHIFT);
						sl = -y1;
					} else
						pa[MLIB_GDS_STARTY] = half
							- (y0 << MLIB_SHIFT);
				} else {
					sl = x1;
					clip |= ady;

					if (y1 < 0) {
						pa[MLIB_GDS_STARTX] = half -
							(x0 + x1 << MLIB_SHIFT);
						pa[MLIB_GDS_STARTY] = y0 + y1;
						sl = -x1;
					} else
						pa[MLIB_GDS_STARTX] = half -
							(x0 << MLIB_SHIFT);
				}

				pa[MLIB_GDS_LENGTHLINES] = clip;
			}
		}
		clipexist = (clip | clipexist) >> 31;
		pa[MLIB_GDS_STEPLINES - MLIB_GDS_NEXT] = sl;
		count = ((mlib_addr)pa - (mlib_addr)ps) >> 4;
		pe = pa;
		pa = ps;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < count - 3; i += 4) {
			f_mul0 = pa[MLIB_GDS_STEPLINES + i * MLIB_GDS_NEXT];
			ll0 = pa[MLIB_GDS_LENGTHLINES + i * MLIB_GDS_NEXT];
			f_mul1 = pa[MLIB_GDS_STEPLINES + (i +
				1) * MLIB_GDS_NEXT];
			ll1 = pa[MLIB_GDS_LENGTHLINES + (i +
				1) * MLIB_GDS_NEXT];
			f_mul2 = pa[MLIB_GDS_STEPLINES + (i +
				2) * MLIB_GDS_NEXT];
			ll2 = pa[MLIB_GDS_LENGTHLINES + (i +
				2) * MLIB_GDS_NEXT];
			f_mul3 = pa[MLIB_GDS_STEPLINES + (i +
				3) * MLIB_GDS_NEXT];
			ll3 = pa[MLIB_GDS_LENGTHLINES + (i +
				3) * MLIB_GDS_NEXT];
			f_div0 = (ll0 <
				MLIB_STEP_TABLE_LENGTH) ?
				mlib_GraphicsStepTable_f32[ll0] : MLIB_ONE_FL /
				ll0;
			f_div1 = (ll1 <
				MLIB_STEP_TABLE_LENGTH) ?
				mlib_GraphicsStepTable_f32[ll1] : MLIB_ONE_FL /
				ll1;
			f_div2 = (ll2 <
				MLIB_STEP_TABLE_LENGTH) ?
				mlib_GraphicsStepTable_f32[ll2] : MLIB_ONE_FL /
				ll2;
			f_div3 = (ll3 <
				MLIB_STEP_TABLE_LENGTH) ?
				mlib_GraphicsStepTable_f32[ll3] : MLIB_ONE_FL /
				ll3;
			pa[MLIB_GDS_STEPLINES + i * MLIB_GDS_NEXT] =
				f_mul0 * f_div0;
			pa[MLIB_GDS_STEPLINES + (i + 1) * MLIB_GDS_NEXT] =
				f_mul1 * f_div1;
			pa[MLIB_GDS_STEPLINES + (i + 2) * MLIB_GDS_NEXT] =
				f_mul2 * f_div2;
			pa[MLIB_GDS_STEPLINES + (i + 3) * MLIB_GDS_NEXT] =
				f_mul3 * f_div3;
		}

		if (count & 2) {
			f_mul0 = pa[MLIB_GDS_STEPLINES + i * MLIB_GDS_NEXT];
			ll0 = pa[MLIB_GDS_LENGTHLINES + i * MLIB_GDS_NEXT];
			f_mul1 = pa[MLIB_GDS_STEPLINES + (i +
				1) * MLIB_GDS_NEXT];
			ll1 = pa[MLIB_GDS_LENGTHLINES + (i +
				1) * MLIB_GDS_NEXT];
			f_div0 = (ll0 <
				MLIB_STEP_TABLE_LENGTH) ?
				mlib_GraphicsStepTable_f32[ll0] : MLIB_ONE_FL /
				ll0;
			f_div1 = (ll1 <
				MLIB_STEP_TABLE_LENGTH) ?
				mlib_GraphicsStepTable_f32[ll1] : MLIB_ONE_FL /
				ll1;
			pa[MLIB_GDS_STEPLINES + i * MLIB_GDS_NEXT] =
				f_mul0 * f_div0;
			pa[MLIB_GDS_STEPLINES + (i + 1) * MLIB_GDS_NEXT] =
				f_mul1 * f_div1;
			i += 2;
		}

		if (count & 1) {
			f_mul0 = pa[MLIB_GDS_STEPLINES + i * MLIB_GDS_NEXT];
			ll0 = pa[MLIB_GDS_LENGTHLINES + i * MLIB_GDS_NEXT];
			f_div0 = (ll0 <
				MLIB_STEP_TABLE_LENGTH) ?
				mlib_GraphicsStepTable_f32[ll0] : MLIB_ONE_FL /
				ll0;
			pa[MLIB_GDS_STEPLINES + i * MLIB_GDS_NEXT] =
				f_mul0 * f_div0;
		}

		if (clipexist) {
			clip = ps[MLIB_GDS_LENGTHLINES];
			pa = ps;
			for (i = 0; i < count; i++) {
				if (pa[MLIB_GDS_LENGTHLINES] < 0) {
					mlib_f32 f_x0, f_y0, f_x1, f_y1, f_tB,
						f_tE, f_ll, f_sl;
					mlib_f32 f_w, f_h;
					mlib_s32 startx, starty;
					mlib_s16 *xx0, *xx1;

					x0 = pa[MLIB_GDS_STARTX] + 1;
					f_x0 = pa[MLIB_GDS_STARTX];
					f_y0 = pa[MLIB_GDS_STARTY];

					if ((x0 & (MLIB_ONE - 1)) !=
						(MLIB_HALF)) {
						f_w = f_width;
						f_h = f_HEIGHT;
						startx = MLIB_GDS_STARTX;
						starty = MLIB_GDS_STARTY;
						xx0 = px0_;
						xx1 = px1_;
					} else {
						f_w = f_height;
						f_h = f_WIDTH;
						startx = MLIB_GDS_STARTY;
						starty = MLIB_GDS_STARTX;
						xx0 = (mlib_s16 *)((mlib_u8 *)
							px0_ + dxy);
						xx1 = (mlib_s16 *)((mlib_u8 *)
							px1_ + dxy);
						f_x0 = f_y0;
						f_y0 = pa[MLIB_GDS_STARTX];
					}

					x0 = *(mlib_s16 *)((mlib_u8 *)xx0 +
						dp0 * i);
					x1 = *(mlib_s16 *)((mlib_u8 *)xx1 +
						dp1 * i);
					ll0 = x1 - x0;
					zz = ll0 >> 31;
					ll0 = (zz & -ll0) | (ll0 & ~zz);
					f_ll = ll0;

					f_mul0 = pa[MLIB_GDS_STEPLINES];
					f_div0 = (ll0 <
						MLIB_STEP_TABLE_LENGTH) ?
						mlib_GraphicsStepTable_f32[ll0]
						: MLIB_ONE_FL / ll0;
					pa[MLIB_GDS_STEPLINES] =
						f_mul0 * f_div0;
					f_div0 = pa[MLIB_GDS_STEPLINES];

					f_sl = f_mul0 * MLIB_ONE_FL;
					f_y0 = -f_one - f_y0;
					f_x1 = f_x0 + f_ll;
					f_y1 = (f_sl ==
						f_zero) ? f_y0 : f_y0 +
						f_div0 * f_ll;

					f_tB = f_y0;
					f_tE = f_y1;
					f_tB = (f_tB < f_zero) ? f_zero : f_tB;
					f_tE = (f_tE < f_zero) ? f_zero : f_tE;
					f_tB = (f_tB > f_h) ? f_h : f_tB;
					f_tE = (f_tE > f_h) ? f_h : f_tE;

					if ((f_sl != f_zero) ||
					    (f_tB == f_y0)) {
						if ((f_sl != f_zero) &&
							(f_ll != f_zero)) {
							f_tB = f_x0 +
								(f_one /
								f_div0) *
								(f_tB - f_y0);
							f_tE = f_x1 +
								(f_one /
								f_div0) *
								(f_tE - f_y1);
							f_tB = (f_tB >
								f_x0) ? f_tB :
								f_x0;
							f_tE = (f_tE <
								f_x1) ? f_tE :
								f_x1;
						} else {
							f_y0 = f_tB;
							f_y1 = f_tE;
							f_tB = f_x0;
							f_tE = f_x1;
						}

						f_tB = (f_tB <
							f_zero) ? f_zero : f_tB;
						f_tE = (f_tE >
							f_w) ? f_w : f_tE;

						if (f_tB <= f_tE) {
						    pa[startx] = f_tB;
						    pa[MLIB_GDS_LENGTHLINES]
							= f_tE;
						    pa[startx] +=
							((pa[startx] ==
							f_tB) ? 0 : 1);
						    pa[MLIB_GDS_LENGTHLINES] =
							pa[MLIB_GDS_LENGTHLINES]
							- pa[startx];

						    if ((f_sl != f_zero) &&
							(f_ll !=
							f_zero)) {
							    f_tB = pa[startx];
							    pa[starty] =
								-(f_y0 +
								f_div0 *
								(f_tB -
								f_x0)) -
								f_one;
						    } else
							    pa[starty] =
								-f_y0 -
								f_one;
						}
					}
				}

				pa += MLIB_GDS_NEXT;
			}
		}

#ifdef	MLIB_VIS
		mlib_GraphicsSpamLineSet_B_8(rtable, stride, ps, pe,
					ca, a_1, vca, va_1);
#else
		mlib_GraphicsSpamLineSet_B_8(rtable, stride, ps, pe,
					ca, a_1, 0, 0);
#endif

		if (dp0 == 2) {  /* StripSet */
			mlib_s16 *px = px0_ + start;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = start; i < nlines; i++, px++) {
				tx0 = *px;
				ty0 = *(mlib_s16 *)((mlib_u8 *)px + dxy);
				z0 = (height - ty0) | (width - tx0);
				z0 = (z0 | ty0 | tx0) >> 31;
				ty0 |= z0;
				tx0 |= z0;
				data0 = (mlib_u8 *)rtable[ty0];
				data0[tx0] = (mlib_u8)points[i];
			}
			start = 0;
			points[0] = points[NB];
		}

		px0_ = (mlib_s16 *)((mlib_u8 *)px0_ + dp0 * NB);
		px1_ = (mlib_s16 *)((mlib_u8 *)px1_ + dp1 * NB);
	}

	if (dp0 == 0) {  /* FanSet */
		if (z0 >= 0) {
			data0 = (mlib_u8 *)rtable[ty0];
			data0[tx0] = (mlib_u8)points[0];
		}

	}
}

/* *********************************************************** */

void
mlib_GraphicsClipLineSet_B_32(
	mlib_u8 **rtable,
	mlib_s32 width,
	mlib_s32 height,
	mlib_s32 stride,
	mlib_s16 *x,
	mlib_s16 *y,
	mlib_s32 nlines,
	mlib_s32 dp0,
	mlib_s32 dp1,
	mlib_s32 c,
	mlib_s32 alp)
{
	mlib_addr dxy;
	mlib_s32 a[MLIB_GDS_NEXT * (NB + 1)];
	mlib_s32 *pa, *ps, *pe;
	mlib_s16 *px0_ = x, *px1_ = x + 1, *px0, *px1;
	mlib_s32 i, count, x0, y0, x1, y1, alllines;
	mlib_s32 zz, adx, ady, clip, clipexist, half = -MLIB_HALF - 1;
	mlib_s32 ll0, ll1, ll2, ll3, sl = 0;
	mlib_f32 f_div0, f_mul0;
	mlib_f32 f_div1, f_mul1;
	mlib_f32 f_div2, f_mul2;
	mlib_f32 f_div3, f_mul3;
	mlib_f32 f_width, f_height, f_WIDTH, f_HEIGHT;
	mlib_f32 f_zero = 0.f, f_one = 1.f;
	mlib_s32 points[NB + 1], start = 1;
	mlib_s32 tx0, ty0, z0;
	mlib_s32 *data0;

	MLIB_GRAPHICS_DECL_VAR_B_32

	MLIB_GRAPHICS_SET_VAR_B_32(c, alp)

	f_width = width;
	f_height = height;
	f_WIDTH = (f_width + f_one) * MLIB_ONE_FL - f_one;
	f_HEIGHT = (f_height + f_one) * MLIB_ONE_FL - f_one;

	dxy = (mlib_addr)y - (mlib_addr)x;
	dp0 <<= 1;
	dp1 <<= 1;
	alllines = nlines;

	if (dp0 == 0) {  /* FanSet */
		tx0 = x[0];
		ty0 = y[0];
		z0 = ty0 | tx0 | (height - ty0) | (width - tx0);
		if (z0 >= 0) {
			data0 = (mlib_s32 *)rtable[ty0] + tx0;
			MLIB_GRAPHICS_BLEND2_32_PP(points, data0)
		}
	}

	while (alllines > 0) {

		nlines = (alllines > NB) ? NB : alllines;
		alllines -= nlines;
		pa = a + MLIB_GDS_NEXT;
		ps = a + MLIB_GDS_NEXT;
		px0 = px0_;
		px1 = px1_;

		clip = 0;
		clipexist = 0;
		pe = ps + MLIB_GDS_NEXT * nlines;

		if (dp0 == 2) {  /* StripSet */

			for (i = 1; pa < pe; pa += MLIB_GDS_NEXT, i++) {
				clipexist |= clip;
				x0 = *px0;
				y0 = *(mlib_s16 *)((mlib_u8 *)px0 + dxy);
				x1 = *px1;
				y1 = *(mlib_s16 *)((mlib_u8 *)px1 + dxy);

				z0 = (height - y1) | (width - x1);
				z0 = (z0 | y1 | x1) >> 31;

				px0 = (mlib_s16 *)((mlib_u8 *)px0 + dp0);
				px1 = (mlib_s16 *)((mlib_u8 *)px1 + dp1);
				clip = (width - x0) | (height - y0) | x0 | y0;
				clip = (clip | z0) >> 31;

				tx0 = x1 | z0;
				ty0 = y1 | z0;
				data0 = (mlib_s32 *)rtable[ty0] + tx0;
				MLIB_GRAPHICS_BLEND2_32_PP(points + i, data0)

				y1 = y1 - y0;
				x1 = x1 - x0;
				zz = y1 >> 31;
				ady = (zz & -y1) | (y1 & ~zz);
				zz = x1 >> 31;
				adx = (zz & -x1) | (x1 & ~zz);
				pa[MLIB_GDS_STEPLINES - MLIB_GDS_NEXT] = sl;
				pa[MLIB_GDS_STARTX] = x0;
				pa[MLIB_GDS_STARTY] = y0;

				if (adx > ady) {
					sl = y1;
					clip |= adx;

					if (x1 < 0) {
						pa[MLIB_GDS_STARTX] = x0 + x1;
						pa[MLIB_GDS_STARTY] = half -
							(y0 + y1 << MLIB_SHIFT);
						sl = -y1;
					} else
						pa[MLIB_GDS_STARTY] = half -
							(y0 << MLIB_SHIFT);
				} else {
					sl = x1;
					clip |= ady;

					if (y1 < 0) {
						pa[MLIB_GDS_STARTX] = half -
							(x0 + x1 << MLIB_SHIFT);
						pa[MLIB_GDS_STARTY] = y0 + y1;
						sl = -x1;
					} else
						pa[MLIB_GDS_STARTX] = half -
							(x0 << MLIB_SHIFT);
				}

				pa[MLIB_GDS_LENGTHLINES] = clip;
			}

		} else {
			for (; pa < pe; pa += MLIB_GDS_NEXT) {
				clipexist |= clip;
				x0 = *px0;
				y0 = *(mlib_s16 *)((mlib_u8 *)px0 + dxy);
				x1 = *px1;
				y1 = *(mlib_s16 *)((mlib_u8 *)px1 + dxy);
				px0 = (mlib_s16 *)((mlib_u8 *)px0 + dp0);
				px1 = (mlib_s16 *)((mlib_u8 *)px1 + dp1);
				clip = (((width - x0) | (height - y0) |
					(width - x1) | (height - y1)) |
					(x0 | y0 | x1 | y1)) >> 31;
				y1 = y1 - y0;
				x1 = x1 - x0;
				zz = y1 >> 31;
				ady = (zz & -y1) | (y1 & ~zz);
				zz = x1 >> 31;
				adx = (zz & -x1) | (x1 & ~zz);
				pa[MLIB_GDS_STEPLINES - MLIB_GDS_NEXT] = sl;
				pa[MLIB_GDS_STARTX] = x0;
				pa[MLIB_GDS_STARTY] = y0;

				if (adx > ady) {
					sl = y1;
					clip |= adx;

					if (x1 < 0) {
						pa[MLIB_GDS_STARTX] = x0 + x1;
						pa[MLIB_GDS_STARTY] = half -
							(y0 + y1 << MLIB_SHIFT);
						sl = -y1;
					} else
						pa[MLIB_GDS_STARTY] = half -
							(y0 << MLIB_SHIFT);
				} else {
					sl = x1;
					clip |= ady;

					if (y1 < 0) {
						pa[MLIB_GDS_STARTX] = half -
							(x0 + x1 << MLIB_SHIFT);
						pa[MLIB_GDS_STARTY] = y0 + y1;
						sl = -x1;
					} else
						pa[MLIB_GDS_STARTX] = half -
							(x0 << MLIB_SHIFT);
				}

				pa[MLIB_GDS_LENGTHLINES] = clip;
			}
		}
		clipexist = (clip | clipexist) >> 31;
		pa[MLIB_GDS_STEPLINES - MLIB_GDS_NEXT] = sl;
		count = ((mlib_addr)pa - (mlib_addr)ps) >> 4;
		pe = pa;
		pa = ps;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < count - 3; i += 4) {
			f_mul0 = pa[MLIB_GDS_STEPLINES + i * MLIB_GDS_NEXT];
			ll0 = pa[MLIB_GDS_LENGTHLINES + i * MLIB_GDS_NEXT];
			f_mul1 = pa[MLIB_GDS_STEPLINES + (i +
				1) * MLIB_GDS_NEXT];
			ll1 = pa[MLIB_GDS_LENGTHLINES + (i +
				1) * MLIB_GDS_NEXT];
			f_mul2 = pa[MLIB_GDS_STEPLINES + (i +
				2) * MLIB_GDS_NEXT];
			ll2 = pa[MLIB_GDS_LENGTHLINES + (i +
				2) * MLIB_GDS_NEXT];
			f_mul3 = pa[MLIB_GDS_STEPLINES + (i +
				3) * MLIB_GDS_NEXT];
			ll3 = pa[MLIB_GDS_LENGTHLINES + (i +
				3) * MLIB_GDS_NEXT];
			f_div0 = (ll0 <
				MLIB_STEP_TABLE_LENGTH) ?
				mlib_GraphicsStepTable_f32[ll0] : MLIB_ONE_FL /
				ll0;
			f_div1 = (ll1 <
				MLIB_STEP_TABLE_LENGTH) ?
				mlib_GraphicsStepTable_f32[ll1] : MLIB_ONE_FL /
				ll1;
			f_div2 = (ll2 <
				MLIB_STEP_TABLE_LENGTH) ?
				mlib_GraphicsStepTable_f32[ll2] : MLIB_ONE_FL /
				ll2;
			f_div3 = (ll3 <
				MLIB_STEP_TABLE_LENGTH) ?
				mlib_GraphicsStepTable_f32[ll3] : MLIB_ONE_FL /
				ll3;
			pa[MLIB_GDS_STEPLINES + i * MLIB_GDS_NEXT] =
				f_mul0 * f_div0;
			pa[MLIB_GDS_STEPLINES + (i + 1) * MLIB_GDS_NEXT] =
				f_mul1 * f_div1;
			pa[MLIB_GDS_STEPLINES + (i + 2) * MLIB_GDS_NEXT] =
				f_mul2 * f_div2;
			pa[MLIB_GDS_STEPLINES + (i + 3) * MLIB_GDS_NEXT] =
				f_mul3 * f_div3;
		}

		if (count & 2) {
			f_mul0 = pa[MLIB_GDS_STEPLINES + i * MLIB_GDS_NEXT];
			ll0 = pa[MLIB_GDS_LENGTHLINES + i * MLIB_GDS_NEXT];
			f_mul1 = pa[MLIB_GDS_STEPLINES + (i +
				1) * MLIB_GDS_NEXT];
			ll1 = pa[MLIB_GDS_LENGTHLINES + (i +
				1) * MLIB_GDS_NEXT];
			f_div0 = (ll0 <
				MLIB_STEP_TABLE_LENGTH) ?
				mlib_GraphicsStepTable_f32[ll0] : MLIB_ONE_FL /
				ll0;
			f_div1 = (ll1 <
				MLIB_STEP_TABLE_LENGTH) ?
				mlib_GraphicsStepTable_f32[ll1] : MLIB_ONE_FL /
				ll1;
			pa[MLIB_GDS_STEPLINES + i * MLIB_GDS_NEXT] =
				f_mul0 * f_div0;
			pa[MLIB_GDS_STEPLINES + (i + 1) * MLIB_GDS_NEXT] =
				f_mul1 * f_div1;
			i += 2;
		}

		if (count & 1) {
			f_mul0 = pa[MLIB_GDS_STEPLINES + i * MLIB_GDS_NEXT];
			ll0 = pa[MLIB_GDS_LENGTHLINES + i * MLIB_GDS_NEXT];
			f_div0 = (ll0 <
				MLIB_STEP_TABLE_LENGTH) ?
				mlib_GraphicsStepTable_f32[ll0] : MLIB_ONE_FL /
				ll0;
			pa[MLIB_GDS_STEPLINES + i * MLIB_GDS_NEXT] =
				f_mul0 * f_div0;
		}

		if (clipexist) {
			clip = ps[MLIB_GDS_LENGTHLINES];
			pa = ps;
			for (i = 0; i < count; i++) {
				if (pa[MLIB_GDS_LENGTHLINES] < 0) {
					mlib_f32 f_x0, f_y0, f_x1, f_y1, f_tB,
						f_tE, f_ll, f_sl;
					mlib_f32 f_w, f_h;
					mlib_s32 startx, starty;
					mlib_s16 *xx0, *xx1;

					x0 = pa[MLIB_GDS_STARTX] + 1;
					f_x0 = pa[MLIB_GDS_STARTX];
					f_y0 = pa[MLIB_GDS_STARTY];

					if ((x0 & (MLIB_ONE - 1)) !=
						(MLIB_HALF)) {
						f_w = f_width;
						f_h = f_HEIGHT;
						startx = MLIB_GDS_STARTX;
						starty = MLIB_GDS_STARTY;
						xx0 = px0_;
						xx1 = px1_;
					} else {
						f_w = f_height;
						f_h = f_WIDTH;
						startx = MLIB_GDS_STARTY;
						starty = MLIB_GDS_STARTX;
						xx0 = (mlib_s16 *)((mlib_u8 *)
							px0_ + dxy);
						xx1 = (mlib_s16 *)((mlib_u8 *)
							px1_ + dxy);
						f_x0 = f_y0;
						f_y0 = pa[MLIB_GDS_STARTX];
					}

					x0 = *(mlib_s16 *)((mlib_u8 *)xx0 +
						dp0 * i);
					x1 = *(mlib_s16 *)((mlib_u8 *)xx1 +
						dp1 * i);
					ll0 = x1 - x0;
					zz = ll0 >> 31;
					ll0 = (zz & -ll0) | (ll0 & ~zz);
					f_ll = ll0;

					f_mul0 = pa[MLIB_GDS_STEPLINES];
					f_div0 = (ll0 <
						MLIB_STEP_TABLE_LENGTH) ?
						mlib_GraphicsStepTable_f32[ll0]
						: MLIB_ONE_FL / ll0;
					pa[MLIB_GDS_STEPLINES] =
						f_mul0 * f_div0;
					f_div0 = pa[MLIB_GDS_STEPLINES];

					f_sl = f_mul0 * MLIB_ONE_FL;
					f_y0 = -f_one - f_y0;
					f_x1 = f_x0 + f_ll;
					f_y1 = (f_sl ==
						f_zero) ? f_y0 : f_y0 +
						f_div0 * f_ll;

					f_tB = f_y0;
					f_tE = f_y1;
					f_tB = (f_tB < f_zero) ? f_zero : f_tB;
					f_tE = (f_tE < f_zero) ? f_zero : f_tE;
					f_tB = (f_tB > f_h) ? f_h : f_tB;
					f_tE = (f_tE > f_h) ? f_h : f_tE;

					if ((f_sl != f_zero) ||
					    (f_tB == f_y0)) {
						if ((f_sl != f_zero) &&
							(f_ll != f_zero)) {
							f_tB = f_x0 +
								(f_one /
								f_div0) *
								(f_tB - f_y0);
							f_tE = f_x1 +
								(f_one /
								f_div0) *
								(f_tE - f_y1);
							f_tB = (f_tB >
								f_x0) ? f_tB :
								f_x0;
							f_tE = (f_tE <
								f_x1) ? f_tE :
								f_x1;
						} else {
							f_y0 = f_tB;
							f_y1 = f_tE;
							f_tB = f_x0;
							f_tE = f_x1;
						}

						f_tB = (f_tB <
							f_zero) ? f_zero : f_tB;
						f_tE = (f_tE >
							f_w) ? f_w : f_tE;

						if (f_tB <= f_tE) {
						    pa[startx] = f_tB;
						    pa[MLIB_GDS_LENGTHLINES]
							= f_tE;
						    pa[startx] +=
							((pa[startx] ==
							f_tB) ? 0 : 1);
						    pa[MLIB_GDS_LENGTHLINES] =
							pa[MLIB_GDS_LENGTHLINES]
							- pa[startx];

						    if ((f_sl != f_zero) &&
							(f_ll !=
							f_zero)) {
							    f_tB = pa[startx];
							    pa[starty] =
								-(f_y0 +
								f_div0 *
								(f_tB -
								f_x0)) -
								f_one;
						    } else
							    pa[starty] =
								-f_y0 -
								f_one;
						}
					}
				}

				pa += MLIB_GDS_NEXT;
			}
		}

#ifdef	MLIB_VIS
		mlib_GraphicsSpamLineSet_B_32(rtable, stride, ps, pe,
					ca1, ca2, ca3, a_1, vca, va_1);
#else
		mlib_GraphicsSpamLineSet_B_32(rtable, stride, ps, pe,
					ca1, ca2, ca3, a_1, 0, 0);
#endif

		if (dp0 == 2) {  /* StripSet */
			mlib_s16 *px = px0_ + start;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = start; i < nlines; i++, px++) {
				tx0 = *px;
				ty0 = *(mlib_s16 *)((mlib_u8 *)px + dxy);
				z0 = (height - ty0) | (width - tx0);
				z0 = (z0 | ty0 | tx0) >> 31;
				ty0 |= z0;
				tx0 |= z0;
				data0 = (mlib_s32 *)rtable[ty0];
				data0[tx0] = points[i];
			}
			start = 0;
			points[0] = points[NB];
		}

		px0_ = (mlib_s16 *)((mlib_u8 *)px0_ + dp0 * NB);
		px1_ = (mlib_s16 *)((mlib_u8 *)px1_ + dp1 * NB);
	}

	if (dp0 == 0) {  /* FanSet */
		if (z0 >= 0) {
			data0 = (mlib_s32 *)rtable[ty0];
			data0[tx0] = points[0];
		}

	}

}

/* *********************************************************** */
