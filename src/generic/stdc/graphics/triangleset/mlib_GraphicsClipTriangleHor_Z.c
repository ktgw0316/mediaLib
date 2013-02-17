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

#pragma ident	"@(#)mlib_GraphicsClipTriangleHor_Z.c	9.2	07/10/09 SMI"

/*
 *  FUNCTIONS
 *    mlib_GraphicsClipTriangleHor_Z   - clipping function for Z-buffer mode
 *
 *  SYNOPSIS
 *    void  mlib_GraphicsClipTriangleHor_Z
 *                                        (mlib_u8  **rtable,
 *                                         mlib_u8  **zrtable,
 *                                         mlib_s32 width,
 *                                         mlib_s32 height,
 *                                         mlib_s16 *x,
 *                                         mlib_s16 *y,
 *                                         mlib_s16 *z,
 *                                         mlib_s32 nlines,
 *                                         mlib_s32 dp0,
 *                                         mlib_s32 dp1,
 *                                         mlib_s32 color,
 *                                         void     (*SpamLineHor)());
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
 *    SpamLineHor Function is calling to fill
 */

/*
 *  FUNCTIONS
 *    mlib_GraphicsClipTriangleHor_BZ   - clipping function for Z-buffer mode
 *					  and alpha blending
 *
 *  SYNOPSIS
 *    void  mlib_GraphicsClipTriangleHor_BZ
 *                                        (mlib_u8  **rtable,
 *                                         mlib_u8  **zrtable,
 *                                         mlib_s32 width,
 *                                         mlib_s32 height,
 *                                         mlib_s16 *x,
 *                                         mlib_s16 *y,
 *                                         mlib_s16 *z,
 *                                         mlib_s32 nlines,
 *                                         mlib_s32 dp0,
 *                                         mlib_s32 dp1,
 *                                         mlib_s32 color,
 *                                         mlib_s32 alpha,
 *                                         void     (*SpamLineHor)());
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
 *    alpha	  Alpha blending value.
 *    SpamLineHor Function is calling to fill
 */

#include <mlib_graphics.h>
#include <mlib_GraphicsDrawSet.h>

/* *********************************************************** */

#define	NB	256

/* *********************************************************** */

#define	MLIB_GET_DATA_Z                                         \
	mlib_f32 *coords_f = (mlib_f32 *)coords;                \
	mlib_f32 f_ys, f_ly, f_ry;                              \
	mlib_f32 f_lx, f_rx, f_lstp, f_rstp;                    \
	mlib_f32 f_lz, f_rz, f_zlstp, f_zrstp;                  \
	mlib_s32 j;                                             \
	                                                        \
	f_ys = ys;                                              \
	f_ly = f_lp[0];                                         \
	f_lx = f_lp[1];                                         \
	f_lstp = f_lp[2];                                       \
	f_lz = f_lp[3];                                         \
	f_zlstp = f_lp[4];                                      \
	f_ry = f_rp[0];                                         \
	f_rx = f_rp[1];                                         \
	f_rstp = f_rp[2];                                       \
	f_rz = f_rp[3];                                         \
	f_zrstp = f_rp[4];                                      \
	f_lx += f_lstp * (f_ys - f_ly);                         \
	f_lz += f_zlstp * (f_ys - f_ly);                        \
	f_rx += f_rstp * (f_ys - f_ry);                         \
	f_rz += f_zrstp * (f_ys - f_ry)

/* *********************************************************** */

static mlib_s32
mlib_GraphicsClipTriangleHor_Fast_Z(
    mlib_s32 *coords,
    mlib_s32 ys,
    mlib_s32 n,
    mlib_f32 *f_lp,
    mlib_f32 *f_rp)
{
	MLIB_GET_DATA_Z;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (j = 0; j < n; j++) {
		coords[MLIB_GDS_STARTY + j * MLIB_GDS_NEXT_GZ] = ys++;
		coords[MLIB_GDS_STARTX + j * MLIB_GDS_NEXT_GZ] = f_lx;
		coords[MLIB_GDS_STOPX + j * MLIB_GDS_NEXT_GZ] = f_rx;
		coords[MLIB_GDS_STARTZ + j * MLIB_GDS_NEXT_GZ] = f_lz;
		coords_f[MLIB_GDS_STEPZ + j * MLIB_GDS_NEXT_GZ] = (f_rz - f_lz);
		f_lx += f_lstp;
		f_rx += f_rstp;
		f_lz += f_zlstp;
		f_rz += f_zrstp;
	}

	return (n);
}

/* *********************************************************** */

static mlib_s32
mlib_GraphicsClipTriangleHor_Slow_Z(
    mlib_s32 *coords,
    mlib_s32 ys,
    mlib_s32 n,
    mlib_f32 *f_lp,
    mlib_f32 *f_rp)
{
	MLIB_GET_DATA_Z;

	for (j = 0; j < n; j++) {
		coords[MLIB_GDS_STARTY + j * MLIB_GDS_NEXT_GZ] = ys++;
		coords[MLIB_GDS_STARTX + j * MLIB_GDS_NEXT_GZ] =
		    f_lx + ((f_lx >= 0.f) ? 0.f : -1.f);
		coords[MLIB_GDS_STOPX + j * MLIB_GDS_NEXT_GZ] = f_rx;
		coords[MLIB_GDS_STARTZ + j * MLIB_GDS_NEXT_GZ] = f_lz;
		coords_f[MLIB_GDS_STEPZ + j * MLIB_GDS_NEXT_GZ] = f_rz - f_lz;
		f_lx += f_lstp;
		f_rx += f_rstp;
		f_lz += f_zlstp;
		f_rz += f_zrstp;
	}

	return (n);
}

/* *********************************************************** */

static void
mlib_GraphicsClipTriangleHor_Step_Z(
    mlib_s32 *coords,
    mlib_s32 *coordsend,
    mlib_s32 allclip,
    mlib_f32 f_w)
{
	mlib_f32 f_lx, f_rx, f_lz, f_rz;
	mlib_s32 j, n = (coordsend - coords) / MLIB_GDS_NEXT_GZ;

	if (allclip)
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (j = 0; j < n; j++) {
			mlib_f32 f_lx_, f_rx_;

			f_lx = coords[MLIB_GDS_STARTX + j * MLIB_GDS_NEXT_GZ];
			f_rx = coords[MLIB_GDS_STOPX + j * MLIB_GDS_NEXT_GZ];
			f_rz = ((mlib_f32 *)coords)[MLIB_GDS_STEPZ +
			    j * MLIB_GDS_NEXT_GZ];
			f_lz = coords[MLIB_GDS_STARTZ + j * MLIB_GDS_NEXT_GZ];
			f_lx_ = (f_lx < 0.f) ? 0.f : f_lx;
			f_rx_ = (f_rx > f_w) ? f_w : f_rx;
			coords[MLIB_GDS_STARTX + j * MLIB_GDS_NEXT_GZ] = f_lx_;
			coords[MLIB_GDS_STOPX + j * MLIB_GDS_NEXT_GZ] = f_rx_;
			coords[MLIB_GDS_STEPZ + j * MLIB_GDS_NEXT_GZ] =
			    f_rz / (f_rx - f_lx);
			coords[MLIB_GDS_STARTZ + j * MLIB_GDS_NEXT_GZ] =
			    f_lz + (f_lx_ - f_lx) * coords[MLIB_GDS_STEPZ +
			    j * MLIB_GDS_NEXT_GZ];
	} else
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (j = 0; j < n; j++) {
			f_lx = coords[MLIB_GDS_STARTX + j * MLIB_GDS_NEXT_GZ];
			f_rx = coords[MLIB_GDS_STOPX + j * MLIB_GDS_NEXT_GZ];
			f_rz = ((mlib_f32 *)coords)[MLIB_GDS_STEPZ +
			    j * MLIB_GDS_NEXT_GZ];
			coords[MLIB_GDS_STEPZ + j * MLIB_GDS_NEXT_GZ] =
			    f_rz / (f_rx - f_lx);
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

#define	MLIB_Y_DIVIDE                                                \
	mlib_f32 f_div0, f_div1, f_div2;                             \
	                                                             \
	f_y0 = yy[0];                                                \
	f_y1 = yy[1];                                                \
	f_y2 = yy[2];                                                \
	ystart = yy[0];                                              \
	yfinish = yy[2];                                             \
	ymedium = yy[1];                                             \
	clipy = (ystart | (height - yfinish)) >> 31;                 \
	if (yfinish - ystart < MLIB_STEP_TABLE_LENGTH) {             \
	    f_div0 = mlib_GraphicsDivTable_f32[ymedium - ystart];    \
	    f_div1 = mlib_GraphicsDivTable_f32[yfinish - ymedium];   \
	    f_div2 = mlib_GraphicsDivTable_f32[yfinish - ystart];    \
	} else {                                                     \
	    f_div0 = 1.f / (f_y1 - f_y0);                            \
	    f_div1 = 1.f / (f_y2 - f_y1);                            \
	    f_div2 = 1.f / (f_y2 - f_y0);                            \
	}                                                            \
	                                                             \
	f_x0 = xx[0];                                                \
	f_x1 = xx[1];                                                \
	f_x2 = xx[2];                                                \
	f_x0 += 0.5f;                                                \
	f_x1 += 0.5f;                                                \
	f_x2 += 0.5f;                                                \
	f_p0[0] = f_y0;                                              \
	f_p0[1] = f_x0;                                              \
	f_p0[2] = (f_x1 - f_x0) * f_div0;                            \
	f_p1[0] = f_y1;                                              \
	f_p1[1] = f_x1;                                              \
	f_p1[2] = (f_x2 - f_x1) * f_div1;                            \
	f_p2[0] = f_y0;                                              \
	f_p2[1] = f_x0;                                              \
	f_p2[2] = (f_x2 - f_x0) * f_div2

/* *********************************************************** */

#define	MLIB_CALC_COLOR(N, M)                                   \
	f_p0[N] = f_c0 + MLIB_HALF;                             \
	f_p0[M] = (f_c1 - f_c0) * f_div0;                       \
	f_p1[N] = f_c1 + MLIB_HALF;                             \
	f_p1[M] = (f_c2 - f_c1) * f_div1;                       \
	f_p2[N] = f_c0 + MLIB_HALF;                             \
	f_p2[M] = (f_c2 - f_c0) * f_div2

/* *********************************************************** */

#define	FUN_NAME(F)	mlib_GraphicsClipTriangleHor_##F

/* *********************************************************** */

void
mlib_GraphicsClipTriangleHor_Z(
    mlib_u8 **rtable,
    mlib_u8 **zrtable,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s16 *x,
    mlib_s16 *y,
    mlib_s16 *z,
    mlib_s32 nlines,
    mlib_s32 dp0,
    mlib_s32 dp1,
    mlib_s32 c,
    void (*SpamLineHor) ())
{
	const mlib_f32 f_width = width + 1;
	mlib_d64 d_c, d_t;
	mlib_s32 a[MLIB_GDS_NEXT_GZ * NB], *ps = a, nleft = NB;
	mlib_s16 *px0 = x, *px1 = x + 1, *py0 = y, *py1 = y + 1;
	mlib_s16 *pz0 = z, *pz1 = z + 1;
	mlib_s32 i, allclip = 0;

	((mlib_s32 *)&d_t)[0] = c;
	((mlib_s32 *)&d_t)[1] = c;
	d_c = d_t;

	if (dp0 != 3)
		nlines = (3 * nlines) >> 1;

	for (i = 0; i < nlines; i += 3) {
		mlib_f32 f_p0[5], f_p1[5], f_p2[5];
		mlib_s32 ystart, ymedium, yfinish, isleft;
		mlib_s32 xx[3], yy[3], zz[3], clipx, clipy;

		{
			MLIB_Y_SORT;

			{
				mlib_s32 x0 = *px0, x1 = *px1, x2 = px1[1];
				mlib_s32 z0 = *pz0, z1 = *pz1, z2 = pz1[1];

				px0 += dp0;
				px1 += dp1;
				pz0 += dp0;
				pz1 += dp1;

				clipx = (x0 | x1 | x2 | (width - x0) | (width -
				    x1) | (width - x2)) >> 31;
				allclip |= clipx;

				xx[t0] = x0;
				xx[t1] = x1;
				xx[t2] = x2;
				zz[t0] = z0 << MLIB_SHIFT;
				zz[t1] = z1 << MLIB_SHIFT;
				zz[t2] = z2 << MLIB_SHIFT;
			}
		}

		{
			mlib_f32 f_x0, f_x1, f_x2, f_y0, f_y1, f_y2, f_c0, f_c1,
			    f_c2;

			MLIB_Y_DIVIDE;

			f_c0 = zz[0];
			f_c1 = zz[1];
			f_c2 = zz[2];

			MLIB_CALC_COLOR(3, 4);

			if (ystart != ymedium) {
				isleft = (f_p2[2] <= f_p0[2]);
				ystart++;
			} else
				isleft = (f_x0 <= f_x1);
		}

		{
			mlib_f32 *f_lp, *f_rp;

			if (isleft) {
				f_lp = f_p2;
				f_rp = f_p0;
			} else {
				f_rp = f_p2;
				f_lp = f_p0;
			}

			if ((yfinish - ystart < nleft) && !(clipx | clipy)) {
				if (ystart != ymedium)
					ps += FUN_NAME(Fast_Z)
					    (ps, ystart, ymedium -
					    ystart, f_lp, f_rp) *
					    MLIB_GDS_NEXT_GZ;

				if (yfinish != ymedium) {
					if (isleft)
						f_rp = f_p1;
					else
						f_lp = f_p1;
					ps += FUN_NAME(Fast_Z)
					    (ps, ymedium, yfinish -
					    ymedium, f_lp, f_rp) *
					    MLIB_GDS_NEXT_GZ;
				}

				nleft -= (yfinish - ystart);
			} else {
				mlib_s32 ym = height + 1;

				ystart = (ystart >= 0) ? ystart : 0;
				yfinish = (yfinish <= height) ? yfinish : ym;
				ym = (ymedium <= height) ? ymedium : ym;

				while (ystart < yfinish) {
					mlib_s32 n = ym - ystart;

					if (ystart >= ymedium) {
						if (isleft)
							f_rp = f_p1;
						else
							f_lp = f_p1;
						n = yfinish - ystart;
					}

					n = (n <= nleft) ? n : nleft;

					if (clipx)
						n = FUN_NAME(Slow_Z)
						    (ps, ystart, n, f_lp, f_rp);
					else
						n = FUN_NAME(Fast_Z)
						    (ps, ystart, n, f_lp, f_rp);

					ps += n * MLIB_GDS_NEXT_GZ;
					ystart += n;
					nleft -= n;

					if (!nleft) {
						FUN_NAME(Step_Z)
						    (a, ps, allclip, f_width);
						allclip = clipx;
						SpamLineHor(rtable, zrtable, a,
						    ps, c, d_c);
						ps = a;
						nleft = NB;
					}
				}
			}
		}
	}

	if (ps > a) {
		FUN_NAME(Step_Z) (a, ps, allclip, f_width);
		SpamLineHor(rtable, zrtable, a, ps, c, d_c);
	}
}

/* *********************************************************** */

void
mlib_GraphicsClipTriangleHor_BZ(
    mlib_u8 **rtable,
    mlib_u8 **zrtable,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s16 *x,
    mlib_s16 *y,
    mlib_s16 *z,
    mlib_s32 nlines,
    mlib_s32 dp0,
    mlib_s32 dp1,
    mlib_s32 c,
    mlib_s32 alpha,
    void (*SpamLineHor) ())
{
	const mlib_f32 f_width = width + 1;
	mlib_d64 d_c, d_t;
	mlib_s32 a[MLIB_GDS_NEXT_GZ * NB], *ps = a, nleft = NB;
	mlib_s16 *px0 = x, *px1 = x + 1, *py0 = y, *py1 = y + 1;
	mlib_s16 *pz0 = z, *pz1 = z + 1;
	mlib_s32 i, allclip = 0;

	((mlib_s32 *)&d_t)[0] = c;
	((mlib_s32 *)&d_t)[1] = c;
	d_c = d_t;

	if (dp0 != 3)
		nlines = (3 * nlines) >> 1;

	for (i = 0; i < nlines; i += 3) {
		mlib_f32 f_p0[5], f_p1[5], f_p2[5];
		mlib_s32 ystart, ymedium, yfinish, isleft;
		mlib_s32 xx[3], yy[3], zz[3], clipx, clipy;

		{
			MLIB_Y_SORT;

			{
				mlib_s32 x0 = *px0, x1 = *px1, x2 = px1[1];
				mlib_s32 z0 = *pz0, z1 = *pz1, z2 = pz1[1];

				px0 += dp0;
				px1 += dp1;
				pz0 += dp0;
				pz1 += dp1;

				clipx = (x0 | x1 | x2 | (width - x0) | (width -
				    x1) | (width - x2)) >> 31;
				allclip |= clipx;

				xx[t0] = x0;
				xx[t1] = x1;
				xx[t2] = x2;
				zz[t0] = z0 << MLIB_SHIFT;
				zz[t1] = z1 << MLIB_SHIFT;
				zz[t2] = z2 << MLIB_SHIFT;
			}
		}

		{
			mlib_f32 f_x0, f_x1, f_x2, f_y0, f_y1, f_y2, f_c0, f_c1,
			    f_c2;

			MLIB_Y_DIVIDE;

			f_c0 = zz[0];
			f_c1 = zz[1];
			f_c2 = zz[2];

			MLIB_CALC_COLOR(3, 4);

			if (ystart != ymedium) {
				isleft = (f_p2[2] <= f_p0[2]);
				ystart++;
			} else
				isleft = (f_x0 <= f_x1);
		}

		{
			mlib_f32 *f_lp, *f_rp;

			if (isleft) {
				f_lp = f_p2;
				f_rp = f_p0;
			} else {
				f_rp = f_p2;
				f_lp = f_p0;
			}

			if ((yfinish - ystart < nleft) && !(clipx | clipy)) {
				if (ystart != ymedium)
					ps += FUN_NAME(Fast_Z)
					    (ps, ystart, ymedium -
					    ystart, f_lp, f_rp) *
					    MLIB_GDS_NEXT_GZ;

				if (yfinish != ymedium) {
					if (isleft)
						f_rp = f_p1;
					else
						f_lp = f_p1;
					ps += FUN_NAME(Fast_Z)
					    (ps, ymedium, yfinish -
					    ymedium, f_lp, f_rp) *
					    MLIB_GDS_NEXT_GZ;
				}

				nleft -= (yfinish - ystart);
			} else {
				mlib_s32 ym = height + 1;

				ystart = (ystart >= 0) ? ystart : 0;
				yfinish = (yfinish <= height) ? yfinish : ym;
				ym = (ymedium <= height) ? ymedium : ym;

				while (ystart < yfinish) {
					mlib_s32 n = ym - ystart;

					if (ystart >= ymedium) {
						if (isleft)
							f_rp = f_p1;
						else
							f_lp = f_p1;
						n = yfinish - ystart;
					}

					n = (n <= nleft) ? n : nleft;

					if (clipx)
						n = FUN_NAME(Slow_Z)
						    (ps, ystart, n, f_lp, f_rp);
					else
						n = FUN_NAME(Fast_Z)
						    (ps, ystart, n, f_lp, f_rp);

					ps += n * MLIB_GDS_NEXT_GZ;
					ystart += n;
					nleft -= n;

					if (!nleft) {
						FUN_NAME(Step_Z)
						    (a, ps, allclip, f_width);
						allclip = clipx;
						SpamLineHor(rtable, zrtable, a,
						    ps, c, alpha, d_c);
						ps = a;
						nleft = NB;
					}
				}
			}
		}
	}

	if (ps > a) {
		FUN_NAME(Step_Z) (a, ps, allclip, f_width);
		SpamLineHor(rtable, zrtable, a, ps, c, alpha, d_c);
	}
}

/* *********************************************************** */
