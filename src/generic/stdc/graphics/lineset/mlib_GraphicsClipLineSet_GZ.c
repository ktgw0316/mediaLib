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

#pragma ident	"@(#)mlib_GraphicsClipLineSet_GZ.c	9.2	07/10/09 SMI"

/*
 *  FUNCTIONS
 *    mlib_GraphicsClipLineSet_GZ_8  - clipping function for Gouraud shading
 *                                       Z-buffering modes
 *    mlib_GraphicsClipLineSet_GZ_32 - clipping function for Gouraud shading
 *                                       Z-buffering modes
 *  SYNOPSIS
 *    void  mlib_GraphicsClipLineSet_GZ_[8|32]
 *                                    (mlib_u8  **rtable,
 *                                     mlib_u8  **zrtable,
 *                                     mlib_s32 width,
 *                                     mlib_s32 height,
 *                                     mlib_s32 stride,
 *                                     mlib_s32 zstride,
 *                                     mlib_s16 *x,
 *                                     mlib_s16 *y,
 *                                     mlib_s16 *z,
 *                                     mlib_s32 *c,
 *                                     mlib_s32 nlines,
 *                                     mlib_s32 dp0,
 *                                     mlib_s32 dp1,
 *                                     void     (*SpamLineSet)());
 *
 *  ARGUMENTS
 *    rtable      Pointer to array of starts of row, rtable[-1][-1] is buffer
 *                  for unwanted stores
 *    zrtable     Pointer to array of starts of row of zbuffer, zrtable[-1][-1]
 *                  is buffer for unwanted stores
 *    width       Image.Width - 1, is using to discover whether clipping needed
 *    height      Image.Height - 1, is using to discover whether clipping needed
 *    x           Pointer to array of X coordinate of the points
 *    y           Pointer to array of Y coordinate of the points
 *    z           Pointer to array of Z coordinate of the points
 *    c           Pointer to array of colors of the points
 *    nlines      Number of lines
 *    dp0         Step from (x0, y0) to the (x0, y0) of the next line
 *    dp1         Step from (x1, y1) to the (x1, y1) of the next line,
 *                for example
 *                  DrawLineSet       dp0 = 2, dp1 = 2
 *                  DrawLineFanSet    dp0 = 0, dp1 = 1
 *                  DrawLineStripSet  dp0 = 1, dp1 = 1
 *    SpamLineSet Function is called to draw
 *    stride      Stride of the image
 *    zstride     Stride of the zbuffer image
 *    coords      Array of values sets using in *Spam* functions:
 *                  coords[MLIB_GDS_STARTX + i * MLIB_GDS_NEXT_GZ]      holds X
 *                  coords[MLIB_GDS_STARTY + i * MLIB_GDS_NEXT_GZ]      holds Y
 *                  coords[MLIB_GDS_LENGTHLINES + i * MLIB_GDS_NEXT_GZ] holds LL
 *                  coords[MLIB_GDS_STEPLINES + i * MLIB_GDS_NEXT_GZ]   holds SL
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
 *   coordsend    for(; coords < coordsend; coords += MLIB_GDS_NEXT_GZ)
 */

#include <mlib_graphics.h>
#include <mlib_GraphicsDrawSet.h>

/* *********************************************************** */

#define	NB	256
#define	IND	MLIB_GDS_LENGTHLINES

/* *********************************************************** */

void
mlib_GraphicsClipLineSet_GZ_8(
	mlib_u8 **rtable,
	mlib_u8 **zrtable,
	mlib_s32 width,
	mlib_s32 height,
	mlib_s32 stride,
	mlib_s32 zstride,
	mlib_s16 *x,
	mlib_s16 *y,
	mlib_s16 *z,
	mlib_s32 *c,
	mlib_s32 nlines,
	mlib_s32 dp0,
	mlib_s32 dp1,
	void (*SpamLineSet) ())
{
	mlib_addr dxy;
	mlib_s32 a[MLIB_GDS_NEXT_GZ * (NB + 1)];
	mlib_s32 *pa, *ps, *pe;
	mlib_s16 *px0_ = x, *px1_ = x + 1, *px0, *px1;
	mlib_s32 *pc0 = c, *pc1 = c + 1;
	mlib_s32 i, dxz, x0, y0, z0, x1, y1, z1, c0, c1, count, alllines;
	mlib_s32 zz, adx, ady, clip, clipexist, half = -MLIB_HALF - 1;
	mlib_s32 ll0, ll1, sl = 0;
	mlib_f32 f_div0, f_mul0, zf_mul0, cf_mul0;
	mlib_f32 f_div1, f_mul1, zf_mul1, cf_mul1;
	mlib_f32 f_width, f_height, f_WIDTH, f_HEIGHT;
	mlib_f32 f_zero = 0.f, f_one = 1.f;

	f_width = width;
	f_height = height;
	f_WIDTH = (f_width + f_one) * MLIB_ONE_FL - f_one;
	f_HEIGHT = (f_height + f_one) * MLIB_ONE_FL - f_one;

	dxy = (mlib_u8 *)y - (mlib_u8 *)x;
	dxz = (mlib_u8 *)z - (mlib_u8 *)x;
	dp0 <<= 1;
	dp1 <<= 1;
	alllines = nlines;

	while (alllines > 0) {

		nlines = (alllines > NB) ? NB : alllines;
		alllines -= nlines;
		pa = a + MLIB_GDS_NEXT_GZ;
		ps = a + MLIB_GDS_NEXT_GZ;
		px0 = px0_;
		px1 = px1_;

		clip = 0;
		clipexist = 0;
		pe = ps + MLIB_GDS_NEXT_GZ * nlines;

		for (; pa < pe; pa += MLIB_GDS_NEXT_GZ) {
			clipexist |= clip;
			x0 = *px0;
			y0 = *(mlib_s16 *)((mlib_u8 *)px0 + dxy);
			z0 = *(mlib_s16 *)((mlib_u8 *)px0 + dxz);
			x1 = *px1;
			y1 = *(mlib_s16 *)((mlib_u8 *)px1 + dxy);
			z1 = *(mlib_s16 *)((mlib_u8 *)px1 + dxz);
			c0 = (*pc0) & 0xFF;
			c1 = (*pc1) & 0xFF;
			px0 = (mlib_s16 *)((mlib_u8 *)px0 + dp0);
			px1 = (mlib_s16 *)((mlib_u8 *)px1 + dp1);
			pc0 = (mlib_s32 *)((mlib_u8 *)pc0 + 2 * dp0);
			pc1 = (mlib_s32 *)((mlib_u8 *)pc1 + 2 * dp1);
			clip = (((width - x0) | (height - y0) | (width -
				x1) | (height -
				y1)) | (x0 | y0 | x1 | y1)) >> 31;
			y1 = y1 - y0;
			x1 = x1 - x0;
			z1 = z1 - z0;
			c1 = c1 - c0;
			zz = y1 >> 31;
			ady = (zz & -y1) | (y1 & ~zz);
			zz = x1 >> 31;
			adx = (zz & -x1) | (x1 & ~zz);
			pa[MLIB_GDS_STEPLINES - MLIB_GDS_NEXT_GZ] = sl;
			pa[MLIB_GDS_STARTX] = x0;
			pa[MLIB_GDS_STARTY] = y0;
			pa[MLIB_GDS_STEPZ] = z1;
			pa[MLIB_GDS_STEPC] = c1;

			if (adx > ady) {
				sl = y1;
				clip |= adx;

				if (x1 < 0) {
					pa[MLIB_GDS_STARTX] = x0 + x1;
					pa[MLIB_GDS_STARTY] =
						half - ((y0 +
						y1) << MLIB_SHIFT);
					pa[MLIB_GDS_STARTZ] =
						half - ((z0 +
						z1) << MLIB_SHIFT);
					pa[MLIB_GDS_STEPZ] = -z1;
					pa[MLIB_GDS_STARTC] =
						half - ((c0 +
						c1) << MLIB_SHIFT);
					pa[MLIB_GDS_STEPC] = -c1;
					sl = -y1;
				} else {
					pa[MLIB_GDS_STARTY] =
						half - (y0 << MLIB_SHIFT);
					pa[MLIB_GDS_STARTZ] =
						half - (z0 << MLIB_SHIFT);
					pa[MLIB_GDS_STARTC] =
						half - (c0 << MLIB_SHIFT);
				}
			} else {
				sl = x1;
				clip |= ady;

				if (y1 < 0) {
					pa[MLIB_GDS_STARTX] =
						half - ((x0 +
						x1) << MLIB_SHIFT);
					pa[MLIB_GDS_STARTY] = y0 + y1;
					pa[MLIB_GDS_STARTZ] =
						half - ((z0 +
						z1) << MLIB_SHIFT);
					pa[MLIB_GDS_STEPZ] = -z1;
					pa[MLIB_GDS_STARTC] =
						half - ((c0 +
						c1) << MLIB_SHIFT);
					pa[MLIB_GDS_STEPC] = -c1;
					sl = -x1;
				} else {
					pa[MLIB_GDS_STARTX] =
						half - (x0 << MLIB_SHIFT);
					pa[MLIB_GDS_STARTZ] =
						half - (z0 << MLIB_SHIFT);
					pa[MLIB_GDS_STARTC] =
						half - (c0 << MLIB_SHIFT);
				}
			}

			pa[MLIB_GDS_LENGTHLINES] = clip;
		}

		clipexist = (clip | clipexist) >> 31;
		pa[MLIB_GDS_STEPLINES - MLIB_GDS_NEXT_GZ] = sl;
		count = ((mlib_u8 *)pa - (mlib_u8 *)ps) >> 5;
		pe = pa;
		pa = ps;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < count - 1; i += 2) {
			f_mul0 = pa[MLIB_GDS_STEPLINES + i * MLIB_GDS_NEXT_GZ];
			ll0 = pa[MLIB_GDS_LENGTHLINES + i * MLIB_GDS_NEXT_GZ];
			zf_mul0 = pa[MLIB_GDS_STEPZ + i * MLIB_GDS_NEXT_GZ];
			cf_mul0 = pa[MLIB_GDS_STEPC + i * MLIB_GDS_NEXT_GZ];
			f_mul1 = pa[MLIB_GDS_STEPLINES + (i +
				1) * MLIB_GDS_NEXT_GZ];
			ll1 = pa[MLIB_GDS_LENGTHLINES + (i +
				1) * MLIB_GDS_NEXT_GZ];
			zf_mul1 =
				pa[MLIB_GDS_STEPZ + (i + 1) * MLIB_GDS_NEXT_GZ];
			cf_mul1 =
				pa[MLIB_GDS_STEPC + (i + 1) * MLIB_GDS_NEXT_GZ];
			f_div0 = (ll0 <
				MLIB_STEP_TABLE_LENGTH) ?
				mlib_GraphicsStepTable_f32[ll0] : MLIB_ONE_FL /
				ll0;
			f_div1 = (ll1 <
				MLIB_STEP_TABLE_LENGTH) ?
				mlib_GraphicsStepTable_f32[ll1] : MLIB_ONE_FL /
				ll1;
			pa[MLIB_GDS_STEPLINES + i * MLIB_GDS_NEXT_GZ] =
				f_mul0 * f_div0;
			pa[MLIB_GDS_STEPZ + i * MLIB_GDS_NEXT_GZ] =
				zf_mul0 * f_div0;
			pa[MLIB_GDS_STEPC + i * MLIB_GDS_NEXT_GZ] =
				cf_mul0 * f_div0;
			pa[MLIB_GDS_STEPLINES + (i + 1) * MLIB_GDS_NEXT_GZ] =
				f_mul1 * f_div1;
			pa[MLIB_GDS_STEPZ + (i + 1) * MLIB_GDS_NEXT_GZ] =
				zf_mul1 * f_div1;
			pa[MLIB_GDS_STEPC + (i + 1) * MLIB_GDS_NEXT_GZ] =
				cf_mul1 * f_div1;
		}

		if (count & 1) {
			f_mul0 = pa[MLIB_GDS_STEPLINES + i * MLIB_GDS_NEXT_GZ];
			ll0 = pa[MLIB_GDS_LENGTHLINES + i * MLIB_GDS_NEXT_GZ];
			zf_mul0 = pa[MLIB_GDS_STEPZ + i * MLIB_GDS_NEXT_GZ];
			cf_mul0 = pa[MLIB_GDS_STEPC + i * MLIB_GDS_NEXT_GZ];
			f_div0 = (ll0 <
				MLIB_STEP_TABLE_LENGTH) ?
				mlib_GraphicsStepTable_f32[ll0] : MLIB_ONE_FL /
				ll0;
			pa[MLIB_GDS_STEPLINES + i * MLIB_GDS_NEXT_GZ] =
				f_mul0 * f_div0;
			pa[MLIB_GDS_STEPZ + i * MLIB_GDS_NEXT_GZ] =
				zf_mul0 * f_div0;
			pa[MLIB_GDS_STEPC + i * MLIB_GDS_NEXT_GZ] =
				cf_mul0 * f_div0;
		}

		if (clipexist) {
			clip = ps[MLIB_GDS_LENGTHLINES];
			pa = ps;
			for (i = 0; i < count; i++) {
				if (pa[MLIB_GDS_LENGTHLINES] < 0) {
					mlib_f32 f_x0, f_y0, f_z0, f_x1, f_y1,
						fc0, f_tB, f_tE, f_ll, f_sl;
					mlib_f32 f_w, f_h;
					mlib_s32 startx, starty;
					mlib_s16 *xx0, *xx1;

					x0 = pa[MLIB_GDS_STARTX] + 1;
					f_x0 = pa[MLIB_GDS_STARTX];
					f_y0 = pa[MLIB_GDS_STARTY];
					f_z0 = -f_one -
						(mlib_f32)(pa[MLIB_GDS_STARTZ]);
					fc0 = -f_one -
						(mlib_f32)(pa[MLIB_GDS_STARTC]);

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
					zf_mul0 = pa[MLIB_GDS_STEPZ];
					cf_mul0 = pa[MLIB_GDS_STEPC];
					f_div0 = (ll0 <
						MLIB_STEP_TABLE_LENGTH) ?
						mlib_GraphicsStepTable_f32[ll0]
						: MLIB_ONE_FL / ll0;
					pa[MLIB_GDS_STEPLINES] =
						f_mul0 * f_div0;
					pa[MLIB_GDS_STEPZ] = zf_mul0 * f_div0;
					pa[MLIB_GDS_STEPC] = cf_mul0 * f_div0;
					f_div0 = pa[MLIB_GDS_STEPLINES];
					zf_mul0 = pa[MLIB_GDS_STEPZ];
					cf_mul0 = pa[MLIB_GDS_STEPC];

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
							pa[MLIB_GDS_LENGTHLINES]
								=
								pa
								[IND]
								- pa[startx];
							f_tB = pa[startx];

							if ((f_sl != f_zero) &&
								(f_ll !=
								f_zero)) {
								pa[starty] =
									-(f_y0 +
									f_div0 *
									(f_tB -
									f_x0)) -
									f_one;
							} else {
								pa[starty] =
									-f_y0 -
									f_one;
							}

							pa[MLIB_GDS_STARTZ] =
								-(f_z0 +
								zf_mul0 *
								(f_tB - f_x0)) -
								f_one;
							pa[MLIB_GDS_STARTC] =
								-(fc0 +
								cf_mul0 *
								(f_tB - f_x0)) -
								f_one;
						}
					}
				}

				pa += MLIB_GDS_NEXT_GZ;
			}
		}

		SpamLineSet(rtable, zrtable, stride, zstride, ps, pe, width - 1,
			height - 1);

		px0_ = (mlib_s16 *)((mlib_u8 *)px0_ + dp0 * NB);
		px1_ = (mlib_s16 *)((mlib_u8 *)px1_ + dp1 * NB);
	}
}

/* *********************************************************** */

#undef  NB
#define	NB	128

#undef  IND
#define	IND	MLIB_GDS_LENGTHLINES

void
mlib_GraphicsClipLineSet_GZ_32(
	mlib_u8 **rtable,
	mlib_u8 **zrtable,
	mlib_s32 width,
	mlib_s32 height,
	mlib_s32 stride,
	mlib_s32 zstride,
	mlib_s16 *x,
	mlib_s16 *y,
	mlib_s16 *z,
	mlib_s32 *c,
	mlib_s32 nlines,
	mlib_s32 dp0,
	mlib_s32 dp1,
	void (*SpamLineSet) ())
{
	mlib_addr dxy;
	mlib_s32 a[MLIB_GDS_NEXT_GZ32 * (NB + 1)];
	mlib_s32 *pa, *ps, *pe;
	mlib_s16 *px0_ = x, *px1_ = x + 1, *px0, *px1;
	mlib_s32 *pc0 = c, *pc1 = c + 1;
	mlib_s32 i, dxz, x0, y0, z0, x1, y1, z1;
	mlib_s32 c0, c1, c0_, c1_, count, alllines;
	mlib_s32 zz, adx, ady, clip, clipexist, half = -MLIB_HALF - 1;
	mlib_s32 ll0, sl = 0;
	mlib_f32 f_div0, f_mul0, zf_mul0, cf_mul0, cf_mul1, cf_mul2;
	mlib_f32 f_width, f_height, f_WIDTH, f_HEIGHT;
	mlib_f32 f_zero = 0.f, f_one = 1.f;

	f_width = width;
	f_height = height;
	f_WIDTH = (f_width + f_one) * MLIB_ONE_FL - f_one;
	f_HEIGHT = (f_height + f_one) * MLIB_ONE_FL - f_one;

	dxy = (mlib_u8 *)y - (mlib_u8 *)x;
	dxz = (mlib_u8 *)z - (mlib_u8 *)x;
	dp0 <<= 1;
	dp1 <<= 1;
	alllines = nlines;

	while (alllines > 0) {

		nlines = (alllines > NB) ? NB : alllines;
		alllines -= nlines;
		pa = a + MLIB_GDS_NEXT_GZ32;
		ps = a + MLIB_GDS_NEXT_GZ32;
		px0 = px0_;
		px1 = px1_;

		clip = 0;
		clipexist = 0;
		pe = ps + MLIB_GDS_NEXT_GZ32 * nlines;

		for (; pa < pe; pa += MLIB_GDS_NEXT_GZ32) {
			clipexist |= clip;
			x0 = *px0;
			y0 = *(mlib_s16 *)((mlib_u8 *)px0 + dxy);
			z0 = *(mlib_s16 *)((mlib_u8 *)px0 + dxz);
			x1 = *px1;
			y1 = *(mlib_s16 *)((mlib_u8 *)px1 + dxy);
			z1 = *(mlib_s16 *)((mlib_u8 *)px1 + dxz);
			c1 = *pc0;
			c0 = *pc1;
			px0 = (mlib_s16 *)((mlib_u8 *)px0 + dp0);
			px1 = (mlib_s16 *)((mlib_u8 *)px1 + dp1);
			clip = (((width - x0) | (height - y0) | (width -
				x1) | (height -
				y1)) | (x0 | y0 | x1 | y1)) >> 31;
			y1 = y1 - y0;
			x1 = x1 - x0;
			z1 = z1 - z0;
			zz = y1 >> 31;
			ady = (zz & -y1) | (y1 & ~zz);
			zz = x1 >> 31;
			adx = (zz & -x1) | (x1 & ~zz);
			pa[MLIB_GDS_STEPLINES - MLIB_GDS_NEXT_GZ32] = sl;
			pa[MLIB_GDS_STARTX] = x0;
			pa[MLIB_GDS_STARTY] = y0;
			pa[MLIB_GDS_STEPZ] = z1;

			if (adx > ady) {
				sl = y1;
				clip |= adx;

				if (x1 < 0) {
					pa[MLIB_GDS_STARTX] = x0 + x1;
					pa[MLIB_GDS_STARTY] =
						half - ((y0 +
						y1) << MLIB_SHIFT);
					pa[MLIB_GDS_STARTZ] =
						half - ((z0 +
						z1) << MLIB_SHIFT);
					pa[MLIB_GDS_STEPZ] = -z1;
					sl = -y1;
				} else {
					mlib_s32 t;

					pa[MLIB_GDS_STARTY] =
						half - (y0 << MLIB_SHIFT);
					pa[MLIB_GDS_STARTZ] =
						half - (z0 << MLIB_SHIFT);
					t = c0;
					c0 = c1;
					c1 = t;
				}
			} else {
				sl = x1;
				clip |= ady;

				if (y1 < 0) {
					pa[MLIB_GDS_STARTX] =
						half - ((x0 +
						x1) << MLIB_SHIFT);
					pa[MLIB_GDS_STARTY] = y0 + y1;
					pa[MLIB_GDS_STARTZ] =
						half - ((z0 +
						z1) << MLIB_SHIFT);
					pa[MLIB_GDS_STEPZ] = -z1;
					sl = -x1;
				} else {
					mlib_s32 t;

					pa[MLIB_GDS_STARTX] =
						half - (x0 << MLIB_SHIFT);
					pa[MLIB_GDS_STARTZ] =
						half - (z0 << MLIB_SHIFT);
					t = c0;
					c0 = c1;
					c1 = t;
				}
			}

			c0_ = c0 & 0xFF;
			c1_ = c1 & 0xFF;
			pa[MLIB_GDS_STEPC] = (c1_ - c0_);
			pa[MLIB_GDS_STARTC] = half - (c0_ << MLIB_SHIFT);
			c0_ = (c0 >> 8) & 0xFF;
			c1_ = (c1 >> 8) & 0xFF;
			pa[MLIB_GDS_STEPC1] = (c1_ - c0_);
			pa[MLIB_GDS_STARTC1] = half - (c0_ << MLIB_SHIFT);
			c0_ = (c0 >> 16) & 0xFF;
			c1_ = (c1 >> 16) & 0xFF;
			pa[MLIB_GDS_STEPC2] = (c1_ - c0_);
			pa[MLIB_GDS_STARTC2] = half - (c0_ << MLIB_SHIFT);
			pa[MLIB_GDS_LENGTHLINES] = clip;
			pc0 = (mlib_s32 *)((mlib_u8 *)pc0 + 2 * dp0);
			pc1 = (mlib_s32 *)((mlib_u8 *)pc1 + 2 * dp1);
		}

		clipexist = (clip | clipexist) >> 31;
		pa[MLIB_GDS_STEPLINES - MLIB_GDS_NEXT_GZ32] = sl;
		count = ((mlib_u8 *)pa - (mlib_u8 *)ps) >> 6;
		pe = pa;
		pa = ps;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < count; i++) {
			f_mul0 = pa[MLIB_GDS_STEPLINES +
				i * MLIB_GDS_NEXT_GZ32];
			ll0 = pa[MLIB_GDS_LENGTHLINES + i * MLIB_GDS_NEXT_GZ32];
			zf_mul0 = pa[MLIB_GDS_STEPZ + i * MLIB_GDS_NEXT_GZ32];
			cf_mul0 = pa[MLIB_GDS_STEPC + i * MLIB_GDS_NEXT_GZ32];
			cf_mul1 = pa[MLIB_GDS_STEPC1 + i * MLIB_GDS_NEXT_GZ32];
			cf_mul2 = pa[MLIB_GDS_STEPC2 + i * MLIB_GDS_NEXT_GZ32];
			f_div0 = (ll0 <
				MLIB_STEP_TABLE_LENGTH) ?
				mlib_GraphicsStepTable_f32[ll0] : MLIB_ONE_FL /
				ll0;
			pa[MLIB_GDS_STEPLINES + i * MLIB_GDS_NEXT_GZ32] =
				f_mul0 * f_div0;
			pa[MLIB_GDS_STEPZ + i * MLIB_GDS_NEXT_GZ32] =
				zf_mul0 * f_div0;
			pa[MLIB_GDS_STEPC + i * MLIB_GDS_NEXT_GZ32] =
				cf_mul0 * f_div0;
			pa[MLIB_GDS_STEPC1 + i * MLIB_GDS_NEXT_GZ32] =
				cf_mul1 * f_div0;
			pa[MLIB_GDS_STEPC2 + i * MLIB_GDS_NEXT_GZ32] =
				cf_mul2 * f_div0;
		}

		if (clipexist) {
			clip = ps[MLIB_GDS_LENGTHLINES];
			pa = ps;
			for (i = 0; i < count; i++) {
				if (pa[MLIB_GDS_LENGTHLINES] < 0) {
					mlib_f32 f_x0, f_y0, f_z0, f_x1, f_y1,
						fc0, fc1, fc2, f_tB, f_tE, f_ll,
						f_sl;
					mlib_f32 f_w, f_h;
					mlib_s32 startx, starty;
					mlib_s16 *xx0, *xx1;

					x0 = pa[MLIB_GDS_STARTX] + 1;
					f_x0 = pa[MLIB_GDS_STARTX];
					f_y0 = pa[MLIB_GDS_STARTY];
					f_z0 = -f_one -
						(mlib_f32)(pa[MLIB_GDS_STARTZ]);
					fc0 = -f_one -
						(mlib_f32)(pa[MLIB_GDS_STARTC]);
					fc1 = -f_one -
						(mlib_f32)(pa
						[MLIB_GDS_STARTC1]);
					fc2 = -f_one -
						(mlib_f32)(pa
						[MLIB_GDS_STARTC2]);

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
					zf_mul0 = pa[MLIB_GDS_STEPZ];
					cf_mul0 = pa[MLIB_GDS_STEPC];
					cf_mul1 = pa[MLIB_GDS_STEPC1];
					cf_mul2 = pa[MLIB_GDS_STEPC2];
					f_div0 = (ll0 <
						MLIB_STEP_TABLE_LENGTH) ?
						mlib_GraphicsStepTable_f32[ll0]
						: MLIB_ONE_FL / ll0;
					pa[MLIB_GDS_STEPLINES] =
						f_mul0 * f_div0;
					pa[MLIB_GDS_STEPZ] = zf_mul0 * f_div0;
					pa[MLIB_GDS_STEPC] = cf_mul0 * f_div0;
					pa[MLIB_GDS_STEPC1] = cf_mul1 * f_div0;
					pa[MLIB_GDS_STEPC2] = cf_mul2 * f_div0;
					f_div0 = pa[MLIB_GDS_STEPLINES];
					zf_mul0 = pa[MLIB_GDS_STEPZ];
					cf_mul0 = pa[MLIB_GDS_STEPC];
					cf_mul1 = pa[MLIB_GDS_STEPC1];
					cf_mul2 = pa[MLIB_GDS_STEPC2];

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
							pa[MLIB_GDS_LENGTHLINES]
								=
								pa
								[IND]
								- pa[startx];
							f_tB = pa[startx];

							if ((f_sl != f_zero) &&
								(f_ll !=
								f_zero)) {
								pa[starty] =
									-(f_y0 +
									f_div0 *
									(f_tB -
									f_x0)) -
									f_one;
							} else {
								pa[starty] =
									-f_y0 -
									f_one;
							}

							pa[MLIB_GDS_STARTZ] =
								-(f_z0 +
								zf_mul0 *
								(f_tB - f_x0)) -
								f_one;
							pa[MLIB_GDS_STARTC] =
								-(fc0 +
								cf_mul0 *
								(f_tB - f_x0)) -
								f_one;
							pa[MLIB_GDS_STARTC1] =
								-(fc1 +
								cf_mul1 *
								(f_tB - f_x0)) -
								f_one;
							pa[MLIB_GDS_STARTC2] =
								-(fc2 +
								cf_mul2 *
								(f_tB - f_x0)) -
								f_one;
						}
					}
				}

				pa += MLIB_GDS_NEXT_GZ32;
			}
		}

		SpamLineSet(rtable, zrtable, stride, zstride, ps, pe, width - 1,
			height - 1);

		px0_ = (mlib_s16 *)((mlib_u8 *)px0_ + dp0 * NB);
		px1_ = (mlib_s16 *)((mlib_u8 *)px1_ + dp1 * NB);
	}
}

/* *********************************************************** */
