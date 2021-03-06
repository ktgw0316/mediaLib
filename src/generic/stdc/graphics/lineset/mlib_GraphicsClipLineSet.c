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

#pragma ident	"@(#)mlib_GraphicsClipLineSet.c	9.2	07/10/09 SMI"

/*
 *  FUNCTIONS
 *    mlib_GraphicsClipLineSet    - clipping function for simple modes
 *
 *  SYNOPSIS
 *    void  mlib_GraphicsClipLineSet
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
 *                                     void     (*SpamLineSet)());
 *
 *  ARGUMENTS
 *    rtable      Pointer to array of starts of row, rtable[-1][-1] is buffer
 *                  for unwanted stores
 *    width       Image.Width - 1, is using to discover whether clipping needed
 *    height      Image.Height - 1, is using to discover whether clipping needed
 *    x           Pointer to array of X coordinate of the points
 *    y           Pointer to array of Y coordinate of the points
 *    nlines      Number of lines
 *    dp0         Step from (x0, y0) to the (x0, y0) of the next line
 *    dp1         Step from (x1, y1) to the (x1, y1) of the next line,
 *                for example
 *                  DrawLineSet       dp0 = 2, dp1 = 2
 *                  DrawLineFanSet    dp0 = 0, dp1 = 1
 *                  DrawLineStripSet  dp0 = 1, dp1 = 1
 *    c           Color to draw, for MLIB_BYTE type of image color must be
 *                  the same in all bytes :
 *                    c = (color & 0xFF),
 *                    c |= (c << 8),
 *                    c |= (c << 16)
 *    SpamLineSet Function is called to draw
 *    stride      Stride of the image
 *    coords      Array of values sets using in *Spam* functions:
 *                  coords[MLIB_GDS_STARTX + i * MLIB_GDS_NEXT]      holds X
 *                  coords[MLIB_GDS_STARTY + i * MLIB_GDS_NEXT]      holds Y
 *                  coords[MLIB_GDS_LENGTHLINES + i * MLIB_GDS_NEXT] holds LL
 *                  coords[MLIB_GDS_STEPLINES + i * MLIB_GDS_NEXT]   holds SL
 *                  if |dx| > |dy| then
 *                    X  = x0, Y = ~(MLIB_HALF + (y0 << MLIB_SHIFT))
 *                    LL = |dx|, SL = (mlib_s32) (32768. * dy / LL)
 *                  else
 *                    X = ~(MLIB_HALF + (x0 << MLIB_SHIFT)), Y = y0
 *                    LL = |dy|, SL = (mlib_s32) (32768. * dx / LL)
 *                  endif
 *                , so we could understand in *Spam* functions what is
 *                bigger |dx| or |dy| by negative value of opposite starting
 *                value. Note we always step by +1 by main axis, never -1.
 *   coordsend    for(; coords < coordsend; coords += MLIB_GDS_NEXT)
 */

#include <mlib_graphics.h>
#include <mlib_GraphicsDrawSet.h>

/* *********************************************************** */

#define	NB	512

/* *********************************************************** */

void
mlib_GraphicsClipLineSet(
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
	void (*SpamLineSet) ())
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
	mlib_d64 d_t, d_c;
	mlib_f32 f_width, f_height, f_WIDTH, f_HEIGHT;
	mlib_f32 f_zero = 0.f, f_one = 1.f;

	f_width = width;
	f_height = height;
	f_WIDTH = (f_width + f_one) * MLIB_ONE_FL - f_one;
	f_HEIGHT = (f_height + f_one) * MLIB_ONE_FL - f_one;

	((mlib_s32 *)&d_t)[0] = c;
	((mlib_s32 *)&d_t)[1] = c;
	d_c = d_t;
	dxy = (mlib_addr)y - (mlib_addr)x;
	dp0 <<= 1;
	dp1 <<= 1;
	alllines = nlines;

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

		for (; pa < pe; pa += MLIB_GDS_NEXT) {
			clipexist |= clip;
			x0 = *px0;
			y0 = *(mlib_s16 *)((mlib_u8 *)px0 + dxy);
			x1 = *px1;
			y1 = *(mlib_s16 *)((mlib_u8 *)px1 + dxy);
			px0 = (mlib_s16 *)((mlib_u8 *)px0 + dp0);
			px1 = (mlib_s16 *)((mlib_u8 *)px1 + dp1);
			clip = (((width - x0) | (height - y0) | (width -
				x1) | (height -
				y1)) | (x0 | y0 | x1 | y1)) >> 31;
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
					pa[MLIB_GDS_STARTY] =
						half - ((y0 +
						y1) << MLIB_SHIFT);
					sl = -y1;
				} else
					pa[MLIB_GDS_STARTY] =
						half - (y0 << MLIB_SHIFT);
			} else {
				sl = x1;
				clip |= ady;

				if (y1 < 0) {
					pa[MLIB_GDS_STARTX] =
						half - ((x0 +
						x1) << MLIB_SHIFT);
					pa[MLIB_GDS_STARTY] = y0 + y1;
					sl = -x1;
				} else
					pa[MLIB_GDS_STARTX] =
						half - (x0 << MLIB_SHIFT);
			}

			pa[MLIB_GDS_LENGTHLINES] = clip;
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

		SpamLineSet(rtable, stride, ps, pe, c, d_c, width - 1,
			height - 1);

		px0_ = (mlib_s16 *)((mlib_u8 *)px0_ + dp0 * NB);
		px1_ = (mlib_s16 *)((mlib_u8 *)px1_ + dp1 * NB);
	}
}

/* *********************************************************** */
