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

#pragma ident	"@(#)mlib_GraphicsClipTriangleHor.c	9.2	07/10/09 SMI"

/*
 *  FUNCTIONS
 *    mlib_GraphicsClipTriangleHor    - clipping function for simple modes
 *    mlib_GraphicsClipTriangleHor_B  - clipping function for simple modes
 *						with alpha blending
 *
 *  SYNOPSIS
 *    void  mlib_GraphicsClipTriangleHor
 *                                        (mlib_u8  **rtable,
 *                                         mlib_s32 width,
 *                                         mlib_s32 height,
 *                                         mlib_s16 *x,
 *                                         mlib_s16 *y,
 *                                         mlib_s32 nlines,
 *                                         mlib_s32 dp0,
 *                                         mlib_s32 dp1,
 *                                         mlib_s32 c,
 *                                         void     (*SpamLineHor)());
 *    void  mlib_GraphicsClipTriangleHor
 *                                        (mlib_u8  **rtable,
 *                                         mlib_s32 width,
 *                                         mlib_s32 height,
 *                                         mlib_s16 *x,
 *                                         mlib_s16 *y,
 *                                         mlib_s32 nlines,
 *                                         mlib_s32 dp0,
 *                                         mlib_s32 dp1,
 *                                         mlib_s32 c,
 *                                         mlib_s32 alpha,
 *                                         void     (*SpamLineHor)());
 *
 *  ARGUMENTS
 *    rtable      Pointer to array of starts of row, rtable[-1][-1] is buffer
 *                for unwanted stores
 *    width       Image.Width - 1, is using to discover whether clipping needed
 *    height      Image.Height - 1, is using to discover whether clipping needed
 *    x           Pointer to array of X coordinate of the points
 *    y           Pointer to array of Y coordinate of the points
 *    nlines      Number of lines
 *    dp0         Step from (x0, y0) to the (x0, y0) of the next line
 *    dp1         Step from (x1, y1) to the (x1, y1) of the next line,
 *                for example
 *                  DrawTriangleSet       dp0 = 3, dp1 = 3
 *                  DrawTriangleFanSet    dp0 = 0, dp1 = 1
 *                  DrawTriangleStripSet  dp0 = 1, dp1 = 1
 *    color       Color to draw, for MLIB_BYTE type of image color must be
 *                  the same in all bytes :
 *                    c  = (color & 0xFF),
 *                    c |= (c << 8),
 *                    c |= (c << 16)
 *    alpha       Alpha value for the blending.
 *    SpamLineHor Function is calling to fill
 */

#include <mlib_graphics.h>
#include <mlib_GraphicsDrawSet.h>

/* *********************************************************** */

#define	NB	512

/* *********************************************************** */

static void
mlib_GraphicsClipTriangleHor_Fast(
	mlib_s32 *coords,
	mlib_s32 ys,
	mlib_s32 n,
	mlib_s32 isleft,
	mlib_f32 f_lx,
	mlib_f32 f_lstp,
	mlib_f32 f_rx,
	mlib_f32 f_rstp)
{
	mlib_s32 j;

	if (isleft)
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (j = 0; j < n; j++) {
			coords[MLIB_GDS_STARTY + j * MLIB_GDS_NEXT] = ys++;
			coords[MLIB_GDS_STARTX + j * MLIB_GDS_NEXT] = f_lx;
			coords[MLIB_GDS_STOPX + j * MLIB_GDS_NEXT] = f_rx;
			f_lx += f_lstp;
			f_rx += f_rstp;
	} else
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (j = 0; j < n; j++) {
			coords[MLIB_GDS_STARTY + j * MLIB_GDS_NEXT] = ys++;
			coords[MLIB_GDS_STOPX + j * MLIB_GDS_NEXT] = f_lx;
			coords[MLIB_GDS_STARTX + j * MLIB_GDS_NEXT] = f_rx;
			f_lx += f_lstp;
			f_rx += f_rstp;
		}
}

/* *********************************************************** */

static void
mlib_GraphicsClipTriangleHor_Slow(
	mlib_s32 *coords,
	mlib_s32 ys,
	mlib_s32 n,
	mlib_s32 isleft,
	mlib_f32 f_lx,
	mlib_f32 f_lstp,
	mlib_f32 f_rx,
	mlib_f32 f_rstp,
	mlib_f32 f_w)
{
	mlib_f32 f_z = 0.f;
	mlib_s32 j;

	if (isleft)
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (j = 0; j < n; j++) {
			coords[MLIB_GDS_STARTY + j * MLIB_GDS_NEXT] = ys++;
			coords[MLIB_GDS_STARTX + j * MLIB_GDS_NEXT] =
				(f_lx < f_z) ? f_z : f_lx;
			coords[MLIB_GDS_STOPX + j * MLIB_GDS_NEXT] =
				(f_rx > f_w) ? f_w : f_rx;
			f_lx += f_lstp;
			f_rx += f_rstp;
	} else
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (j = 0; j < n; j++) {
			coords[MLIB_GDS_STARTY + j * MLIB_GDS_NEXT] = ys++;
			coords[MLIB_GDS_STARTX + j * MLIB_GDS_NEXT] =
				(f_rx < f_z) ? f_z : f_rx;
			coords[MLIB_GDS_STOPX + j * MLIB_GDS_NEXT] =
				(f_lx > f_w) ? f_w : f_lx;
			f_rx += f_rstp;
			f_lx += f_lstp;
		}
}

/* *********************************************************** */

#define	MLIB_Y_SORT                                             \
	mlib_s32 y0 = *py0, y1 = *py1, y2 = py1[1];             \
	mlib_s32 t0, t1, t2;                                    \
	                                                        \
	py0 += dp0;                                             \
	py1 += dp1;                                             \
	if (y0 <= y1) {                                         \
	    if (y1 <= y2) {                                     \
		t0 = 0;                                         \
		t1 = 1;                                         \
		t2 = 2;                                         \
	    } else {                                            \
		t1 = 2;                                         \
		if (y0 <= y2) {                                 \
		    t0 = 0;                                     \
		    t2 = 1;                                     \
		} else {                                        \
		    t0 = 1;                                     \
		    t2 = 0;                                     \
		}                                               \
	    }                                                   \
	} else {                                                \
	    if (y0 <= y2) {                                     \
		t0 = 1;                                         \
		t1 = 0;                                         \
		t2 = 2;                                         \
	    } else {                                            \
		t0 = 2;                                         \
		if (y1 <= y2) {                                 \
		    t1 = 0;                                     \
		    t2 = 1;                                     \
		} else {                                        \
		    t1 = 1;                                     \
		    t2 = 0;                                     \
		}                                               \
	    }                                                   \
	}                                                       \
	                                                        \
	yy[t0] = y0;                                            \
	yy[t1] = y1;                                            \
	yy[t2] = y2

/* *********************************************************** */

void
mlib_GraphicsClipTriangleHor(
	mlib_u8 **rtable,
	mlib_s32 width,
	mlib_s32 height,
	mlib_s16 *x,
	mlib_s16 *y,
	mlib_s32 nlines,
	mlib_s32 dp0,
	mlib_s32 dp1,
	mlib_s32 c,
	void (*SpamLineHor) ())
{
	mlib_f32 f_width = width + 1;
	mlib_f32 f_zero = 0.f;
	mlib_d64 d_c, d_t;
	mlib_s32 a[MLIB_GDS_NEXT * NB], *ps = a, nleft = NB;
	mlib_s16 *px0 = x, *px1 = x + 1, *py0 = y, *py1 = y + 1;
	mlib_s32 i;

	((mlib_s32 *)&d_t)[0] = c;
	((mlib_s32 *)&d_t)[1] = c;
	d_c = d_t;

	if (dp0 != 3)
		nlines = (3 * nlines) >> 1;

	for (i = 0; i < nlines; i += 3) {
		mlib_f32 f_x0, f_x1, f_x2, f_y0, f_y1, f_y2;
		mlib_f32 f_stp0, f_stp1, f_stp2;
		mlib_s32 ystart, ymedium, yfinish;
		mlib_s32 xx[3], yy[3], clipx, clipy;

		{
			MLIB_Y_SORT;
			{
				mlib_s32 x0 = *px0, x1 = *px1, x2 = px1[1];

				px0 += dp0;
				px1 += dp1;
				clipx = (x0 | x1 | x2 | (width - x0) | (width -
					x1) | (width - x2)) >> 31;

				xx[t0] = x0;
				xx[t1] = x1;
				xx[t2] = x2;
			}
		}

		f_x0 = xx[0];
		f_x1 = xx[1];
		f_x2 = xx[2];
		f_y0 = yy[0];
		f_y1 = yy[1];
		f_y2 = yy[2];
		ystart = yy[0];
		yfinish = yy[2];
		ymedium = yy[1];

		clipy = (ystart | (height - yfinish)) >> 31;

		if (yfinish - ystart < MLIB_STEP_TABLE_LENGTH) {
			f_stp2 = (f_x2 -
				f_x0) * mlib_GraphicsDivTable_f32[yfinish -
				ystart];
			f_stp0 = (f_x1 -
				f_x0) * mlib_GraphicsDivTable_f32[ymedium -
				ystart];
			f_stp1 = (f_x2 -
				f_x1) * mlib_GraphicsDivTable_f32[yfinish -
				ymedium];
		} else {
			f_stp0 = (f_x1 - f_x0) / (f_y1 - f_y0);
			f_stp1 = (f_x2 - f_x1) / (f_y2 - f_y1);
			f_stp2 = (f_x2 - f_x0) / (f_y2 - f_y0);
			f_stp0 = (f_y1 == f_y0) ? f_zero : f_stp0;
			f_stp1 = (f_y2 == f_y1) ? f_zero : f_stp1;
			f_stp2 = (f_y2 == f_y0) ? f_zero : f_stp2;
		}

		f_x0 += 0.5f;
		f_x1 += 0.5f;
		f_x2 += 0.5f;

		if ((yfinish - ystart < nleft) && !(clipx | clipy)) {
			mlib_s32 isleft;

			if (ystart != ymedium) {
				isleft = (f_stp0 <= f_stp2);
				ystart++;
				mlib_GraphicsClipTriangleHor_Fast(ps, ystart,
					ymedium - ystart, isleft, f_x0 + f_stp0,
					f_stp0, f_x0 + f_stp2, f_stp2);
				f_x0 = f_x0 + f_stp2 * (ymedium - ystart + 1);
				ps += (ymedium - ystart) * MLIB_GDS_NEXT;
			} else
				isleft = (f_x0 > f_x1);

			nleft -= (yfinish - ystart);

			yfinish = yfinish - ymedium;
			mlib_GraphicsClipTriangleHor_Fast(ps, ymedium, yfinish,
				isleft, f_x1, f_stp1, f_x0, f_stp2);
			ps += yfinish * MLIB_GDS_NEXT;
		} else {
			mlib_s32 isleft, n;

			isleft = (ystart != ymedium) ? (f_stp2 <=
				f_stp0) : (f_x0 <= f_x1);
			ystart = (ystart >= 0) ? ystart : 0;
			yfinish = (yfinish <= height) ? yfinish : (height + 1);

			while (ystart < yfinish) {
				mlib_f32 f_lx, f_rx, f_rstp, f_ys;

				f_ys = ystart;

				if (ystart < ymedium) {
					f_lx = f_x0 + f_stp2 * (f_ys - f_y0);
					f_rx = f_x0 + f_stp0 * (f_ys - f_y0);
					f_rstp = f_stp0;
					n = ymedium;
					n = (n <= height) ? n : (height + 1);
				} else {
					f_lx = f_x0 + f_stp2 * (f_ys - f_y0);
					f_rx = f_x1 + f_stp1 * (f_ys - f_y1);
					f_rstp = f_stp1;
					n = yfinish;
				}

				n = n - ystart;
				n = (n <= nleft) ? n : nleft;

				if (clipx)
					mlib_GraphicsClipTriangleHor_Slow(ps,
						ystart, n, isleft, f_lx, f_stp2,
						f_rx, f_rstp, f_width);
				else
					mlib_GraphicsClipTriangleHor_Fast(ps,
						ystart, n, isleft, f_lx, f_stp2,
						f_rx, f_rstp);

				ps += n * MLIB_GDS_NEXT;
				ystart += n;
				nleft -= n;

				if (!nleft) {
					SpamLineHor(rtable, a, ps, c, d_c);
					ps = a;
					nleft = NB;
				}
			}
		}
	}

	if (ps > a) {
		SpamLineHor(rtable, a, ps, c, d_c);
	}
}

/* *********************************************************** */

void
mlib_GraphicsClipTriangleHor_B(
	mlib_u8 **rtable,
	mlib_s32 width,
	mlib_s32 height,
	mlib_s16 *x,
	mlib_s16 *y,
	mlib_s32 nlines,
	mlib_s32 dp0,
	mlib_s32 dp1,
	mlib_s32 c,
	mlib_s32 alpha,
	void (*SpamLineHor) ())
{
	mlib_f32 f_width = width + 1;
	mlib_f32 f_zero = 0.f;
	mlib_d64 d_c, d_t;
	mlib_s32 a[MLIB_GDS_NEXT * NB], *ps = a, nleft = NB;
	mlib_s16 *px0 = x, *px1 = x + 1, *py0 = y, *py1 = y + 1;
	mlib_s32 i;

	((mlib_s32 *)&d_t)[0] = c;
	((mlib_s32 *)&d_t)[1] = c;
	d_c = d_t;

	if (dp0 != 3)
		nlines = (3 * nlines) >> 1;

	for (i = 0; i < nlines; i += 3) {
		mlib_f32 f_x0, f_x1, f_x2, f_y0, f_y1, f_y2;
		mlib_f32 f_stp0, f_stp1, f_stp2;
		mlib_s32 ystart, ymedium, yfinish;
		mlib_s32 xx[3], yy[3], clipx, clipy;

		{
			MLIB_Y_SORT;
			{
				mlib_s32 x0 = *px0, x1 = *px1, x2 = px1[1];

				px0 += dp0;
				px1 += dp1;
				clipx = (x0 | x1 | x2 | (width - x0) | (width -
					x1) | (width - x2)) >> 31;

				xx[t0] = x0;
				xx[t1] = x1;
				xx[t2] = x2;
			}
		}

		f_x0 = xx[0];
		f_x1 = xx[1];
		f_x2 = xx[2];
		f_y0 = yy[0];
		f_y1 = yy[1];
		f_y2 = yy[2];
		ystart = yy[0];
		yfinish = yy[2];
		ymedium = yy[1];

		clipy = (ystart | (height - yfinish)) >> 31;

		if (yfinish - ystart < MLIB_STEP_TABLE_LENGTH) {
			f_stp2 = (f_x2 -
				f_x0) * mlib_GraphicsDivTable_f32[yfinish -
				ystart];
			f_stp0 = (f_x1 -
				f_x0) * mlib_GraphicsDivTable_f32[ymedium -
				ystart];
			f_stp1 = (f_x2 -
				f_x1) * mlib_GraphicsDivTable_f32[yfinish -
				ymedium];
		} else {
			f_stp0 = (f_x1 - f_x0) / (f_y1 - f_y0);
			f_stp1 = (f_x2 - f_x1) / (f_y2 - f_y1);
			f_stp2 = (f_x2 - f_x0) / (f_y2 - f_y0);
			f_stp0 = (f_y1 == f_y0) ? f_zero : f_stp0;
			f_stp1 = (f_y2 == f_y1) ? f_zero : f_stp1;
			f_stp2 = (f_y2 == f_y0) ? f_zero : f_stp2;
		}

		f_x0 += 0.5f;
		f_x1 += 0.5f;
		f_x2 += 0.5f;

		if ((yfinish - ystart < nleft) && !(clipx | clipy)) {
			mlib_s32 isleft;

			if (ystart != ymedium) {
				isleft = (f_stp0 <= f_stp2);
				ystart++;
				mlib_GraphicsClipTriangleHor_Fast(ps, ystart,
					ymedium - ystart, isleft, f_x0 + f_stp0,
					f_stp0, f_x0 + f_stp2, f_stp2);
				f_x0 = f_x0 + f_stp2 * (ymedium - ystart + 1);
				ps += (ymedium - ystart) * MLIB_GDS_NEXT;
			} else
				isleft = (f_x0 > f_x1);

			nleft -= (yfinish - ystart);

			yfinish = yfinish - ymedium;
			mlib_GraphicsClipTriangleHor_Fast(ps, ymedium, yfinish,
				isleft, f_x1, f_stp1, f_x0, f_stp2);
			ps += yfinish * MLIB_GDS_NEXT;
		} else {
			mlib_s32 isleft, n;

			isleft = (ystart != ymedium) ? (f_stp2 <=
				f_stp0) : (f_x0 <= f_x1);
			ystart = (ystart >= 0) ? ystart : 0;
			yfinish = (yfinish <= height) ? yfinish : (height + 1);

			while (ystart < yfinish) {
				mlib_f32 f_lx, f_rx, f_rstp, f_ys;

				f_ys = ystart;

				if (ystart < ymedium) {
					f_lx = f_x0 + f_stp2 * (f_ys - f_y0);
					f_rx = f_x0 + f_stp0 * (f_ys - f_y0);
					f_rstp = f_stp0;
					n = ymedium;
					n = (n <= height) ? n : (height + 1);
				} else {
					f_lx = f_x0 + f_stp2 * (f_ys - f_y0);
					f_rx = f_x1 + f_stp1 * (f_ys - f_y1);
					f_rstp = f_stp1;
					n = yfinish;
				}

				n = n - ystart;
				n = (n <= nleft) ? n : nleft;

				if (clipx)
					mlib_GraphicsClipTriangleHor_Slow(ps,
						ystart, n, isleft, f_lx, f_stp2,
						f_rx, f_rstp, f_width);
				else
					mlib_GraphicsClipTriangleHor_Fast(ps,
						ystart, n, isleft, f_lx, f_stp2,
						f_rx, f_rstp);

				ps += n * MLIB_GDS_NEXT;
				ystart += n;
				nleft -= n;

				if (!nleft) {
					SpamLineHor(rtable, a, ps,
						c, alpha, d_c);
					ps = a;
					nleft = NB;
				}
			}
		}
	}

	if (ps > a) {
		SpamLineHor(rtable, a, ps, c, alpha, d_c);
	}
}

/* *********************************************************** */
