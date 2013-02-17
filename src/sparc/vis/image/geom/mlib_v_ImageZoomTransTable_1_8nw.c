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

#pragma ident	"@(#)mlib_v_ImageZoomTransTable_1_8nw.c	9.2	07/11/05 SMI"

/*
 * FUNCTION
 *      Internal functions for mlib_ImageZoomTranslateTable.
 */

#include <vis_proto.h>
#include <mlib_image.h>
#include <mlib_ImageInterpTable.h>
#include <mlib_ImageZoomTranslateTable.h>
#include <mlib_v_ImageZoomTransTable_8nw.h>

/* *********************************************************** */

#define	FILTER_TYPE	mlib_d64
#define	FP_TYPE	mlib_d64

/* *********************************************************** */

#define	VPARAMX                                                 \
	mlib_s32 x, mlib_zoom_workspace *ws, FP_TYPE *xb,       \
	FP_TYPE *pbuff, mlib_u8 *sp0

/* *********************************************************** */

#define	VPARAMXY1                                                 \
	mlib_s32 x, mlib_zoom_workspace *ws, FILTER_TYPE *fptr,   \
	FP_TYPE *xb, FP_TYPE *ybuff, mlib_u8 *sp0

/* *********************************************************** */

#define	VPARAMXY2	VPARAMXY1, mlib_u8 *spp0
#define	VPARAMXY3	VPARAMXY2, mlib_u8 *sp10

#define	VPARAMXY4	VPARAMXY3, mlib_u8 *sp20

/* *********************************************************** */

#define	DECL_LINE                                               \
	mlib_s32 i;                                             \
	mlib_s32 dx = ws->dx;                                   \
	mlib_s32 size = ws->size;                               \
	mlib_s32 x_shift = ws->x_shift;                         \
	mlib_u8 *sp1, *sp2

/* *********************************************************** */

#define	DECL_LINE_HI                                            \
	FP_TYPE d0, d1, r0;                                     \
	FP_TYPE xfd0;                                           \
	FP_TYPE acc1;                                           \
	mlib_f32 *fpbuff = (mlib_f32 *)pbuff

/* *********************************************************** */

#define	DECL_LINE_LOW                                           \
	mlib_u8 *sp3, *sp4;                                     \
	mlib_s32 delta = ws->dy;                                \
	FP_TYPE bb, d0;                                         \
	FP_TYPE xfd0;                                           \
	FP_TYPE yfd0;                                           \
	FP_TYPE acc1

/* *********************************************************** */

#define	DECL_COEF                                               \
	mlib_s32 i;                                             \
	mlib_s32 xoff = ws->off;                                \
	mlib_s32 dx = ws->dx;                                   \
	mlib_s32 delta = ws->dy;                                \
	mlib_s32 size = ws->size;                               \
	mlib_s32 xf_shift = ws->xf_shift;                       \
	mlib_s32 xf_mask = ws->xf_mask;                         \
	mlib_s16 *xfp1, *xfp2, *xfp3, *xfp4;                    \
	mlib_u8 *xx

/* *********************************************************** */

#define	CALC_SRC_PTRX_4(ss)                                     \
	sp1 = ss + (x >> x_shift);                              \
	sp2 = ss + ((x + dx) >> x_shift);                       \
	sp3 = ss + ((x + 2 * dx) >> x_shift);                   \
	sp4 = ss + ((x + 3 * dx) >> x_shift)

/* *********************************************************** */

#define	CALC_SRC_PTRX_3(ss)                                     \
	sp1 = ss + (x >> x_shift);                              \
	sp2 = ss + ((x + dx) >> x_shift);                       \
	sp3 = ss + ((x + 2 * dx) >> x_shift)

/* *********************************************************** */

#define	CALC_X_4()                                              \
	x1 = x >> x_shift;                                      \
	x2 = (x + dx) >> x_shift;                               \
	x3 = (x + 2 * dx) >> x_shift;                           \
	x4 = (x + 3 * dx) >> x_shift

/* *********************************************************** */

#define	CALC_X_3()                                              \
	x1 = x >> x_shift;                                      \
	x2 = (x + dx) >> x_shift;                               \
	x3 = (x + 2 * dx) >> x_shift

/* *********************************************************** */

#define	CALC_SRCX_4(ss)                                         \
	sp1 = ss + x1;                                          \
	sp2 = ss + x2;                                          \
	sp3 = ss + x3;                                          \
	sp4 = ss + x4

/* *********************************************************** */

#define	MLIB_FILTER_X4(x, dx, filter, l)                                   \
	xx = (mlib_u8 *)filter + l;                                        \
	xfp1 = (mlib_s16 *)(xx + ((x >> xf_shift) & xf_mask));             \
	xfp2 = (mlib_s16 *)(xx + (((x + dx) >> xf_shift) & xf_mask));      \
	xfp3 =                                                             \
	    (mlib_s16 *)(xx + (((x + 2 * dx) >> xf_shift) & xf_mask));     \
	xfp4 = (mlib_s16 *)(xx + (((x + 3 * dx) >> xf_shift) & xf_mask))

/* *********************************************************** */

#define	FMUL_16x16(x, y)                                          \
	vis_fpadd16(vis_fmul8sux16(x, y), vis_fmul8ulx16(x, y))

/* *********************************************************** */

#define	LD_4x32_U8(acc1, acc2, sp1, sp2, sp3, sp4)              \
	acc1 = vis_faligndata(vis_ld_u8(sp4 + 1), acc1);        \
	acc1 = vis_faligndata(vis_ld_u8(sp3 + 1), acc1);        \
	acc1 = vis_faligndata(vis_ld_u8(sp2 + 1), acc1);        \
	acc1 = vis_faligndata(vis_ld_u8(sp1 + 1), acc1);        \
	acc1 = vis_faligndata(vis_ld_u8(sp4), acc1);            \
	acc1 = vis_faligndata(vis_ld_u8(sp3), acc1);            \
	acc1 = vis_faligndata(vis_ld_u8(sp2), acc1);            \
	acc1 = vis_faligndata(vis_ld_u8(sp1), acc1);            \
	acc2 = vis_faligndata(vis_ld_u8(sp4 + 3), acc2);        \
	acc2 = vis_faligndata(vis_ld_u8(sp3 + 3), acc2);        \
	acc2 = vis_faligndata(vis_ld_u8(sp2 + 3), acc2);        \
	acc2 = vis_faligndata(vis_ld_u8(sp1 + 3), acc2);        \
	acc2 = vis_faligndata(vis_ld_u8(sp4 + 2), acc2);        \
	acc2 = vis_faligndata(vis_ld_u8(sp3 + 2), acc2);        \
	acc2 = vis_faligndata(vis_ld_u8(sp2 + 2), acc2);        \
	acc2 = vis_faligndata(vis_ld_u8(sp1 + 2), acc2)

/* *********************************************************** */

#define	LD_3x32_U8(acc1, acc2, sp1, sp2, sp3, sp4)              \
	acc1 = vis_faligndata(vis_ld_u8(sp4 + 1), acc1);        \
	acc1 = vis_faligndata(vis_ld_u8(sp3 + 1), acc1);        \
	acc1 = vis_faligndata(vis_ld_u8(sp2 + 1), acc1);        \
	acc1 = vis_faligndata(vis_ld_u8(sp1 + 1), acc1);        \
	acc1 = vis_faligndata(vis_ld_u8(sp4), acc1);            \
	acc1 = vis_faligndata(vis_ld_u8(sp3), acc1);            \
	acc1 = vis_faligndata(vis_ld_u8(sp2), acc1);            \
	acc1 = vis_faligndata(vis_ld_u8(sp1), acc1);            \
	acc2 = vis_faligndata(vis_ld_u8(sp4 + 2), acc2);        \
	acc2 = vis_faligndata(vis_ld_u8(sp3 + 2), acc2);        \
	acc2 = vis_faligndata(vis_ld_u8(sp2 + 2), acc2);        \
	acc2 = vis_faligndata(vis_ld_u8(sp1 + 2), acc2)

/* *********************************************************** */

#define	LD_2x32_U8(acc1, sp1, sp2, sp3, sp4)                    \
	acc1 = vis_faligndata(vis_ld_u8(sp4 + 1), acc1);        \
	acc1 = vis_faligndata(vis_ld_u8(sp3 + 1), acc1);        \
	acc1 = vis_faligndata(vis_ld_u8(sp2 + 1), acc1);        \
	acc1 = vis_faligndata(vis_ld_u8(sp1 + 1), acc1);        \
	acc1 = vis_faligndata(vis_ld_u8(sp4), acc1);            \
	acc1 = vis_faligndata(vis_ld_u8(sp3), acc1);            \
	acc1 = vis_faligndata(vis_ld_u8(sp2), acc1);            \
	acc1 = vis_faligndata(vis_ld_u8(sp1), acc1)

/* *********************************************************** */

#define	LD_1x32_U8(acc1, sp1, sp2, sp3, sp4)                    \
	acc1 = vis_faligndata(vis_ld_u8(sp4), acc1);            \
	acc1 = vis_faligndata(vis_ld_u8(sp3), acc1);            \
	acc1 = vis_faligndata(vis_ld_u8(sp2), acc1);            \
	acc1 = vis_faligndata(vis_ld_u8(sp1), acc1)

/* *********************************************************** */

#define	LD_32_S16(acc1, xfp1, xfp2, xfp3, xfp4, N)              \
	acc1 = vis_faligndata(vis_ld_u16(xfp4 + N), acc1);      \
	acc1 = vis_faligndata(vis_ld_u16(xfp3 + N), acc1);      \
	acc1 = vis_faligndata(vis_ld_u16(xfp2 + N), acc1);      \
	acc1 = vis_faligndata(vis_ld_u16(xfp1 + N), acc1)

/* *********************************************************** */

#define	LD_32_H_S16(acc1, xfp1, xfp2, N)                         \
	acc1 = vis_faligndata(vis_ld_u16(xfp2 + N + 1), acc1);   \
	acc1 = vis_faligndata(vis_ld_u16(xfp1 + N + 1), acc1);   \
	acc1 = vis_faligndata(vis_ld_u16(xfp2 + N), acc1);       \
	acc1 = vis_faligndata(vis_ld_u16(xfp1 + N), acc1)

/* *********************************************************** */

#define	MULX_4()                                                \
	d0 = vis_fmul8x16(vis_read_hi(acc1), xfd0);             \
	d1 = vis_fmul8x16(vis_read_lo(acc1), xfd1);             \
	d2 = vis_fmul8x16(vis_read_hi(acc2), xfd2);             \
	d3 = vis_fmul8x16(vis_read_lo(acc2), xfd3);             \
	d0 = vis_fpadd16(d0, d1);                               \
	d2 = vis_fpadd16(d2, d3);                               \
	d0 = vis_fpadd16(d0, d2)

/* *********************************************************** */

#define	MULX_3()                                                \
	d0 = vis_fmul8x16(vis_read_hi(acc1), xfd0);             \
	d1 = vis_fmul8x16(vis_read_lo(acc1), xfd1);             \
	d2 = vis_fmul8x16(vis_read_hi(acc2), xfd2);             \
	d0 = vis_fpadd16(d0, d1);                               \
	d0 = vis_fpadd16(d0, d2)

/* *********************************************************** */

#define	MULX_2()                                                \
	d0 = vis_fmul8x16(vis_read_hi(acc1), xfd0);             \
	d1 = vis_fmul8x16(vis_read_lo(acc1), xfd1);             \
	d0 = vis_fpadd16(d0, d1)

/* *********************************************************** */

void
mlib_calc_4x_off_1ch(
    VPARAMX)
{
	DECL_LINE;
	DECL_LINE_HI;
	FP_TYPE d2, d3, r1, r2;
	FP_TYPE xfd1;
	FP_TYPE acc2;

	i = 0;

	if (i <= size - 2) {

		sp1 = sp0 + (x >> x_shift);
		x += dx;
		sp2 = sp0 + (x >> x_shift);
		x += dx;

		d0 = vis_fpmerge(vis_read_lo(vis_ld_u8_i(sp1, 0)),
		    vis_read_lo(vis_ld_u8_i(sp1, 1)));
		d1 = vis_fpmerge(vis_read_lo(vis_ld_u8_i(sp2, 0)),
		    vis_read_lo(vis_ld_u8_i(sp2, 1)));
		d2 = vis_fpmerge(vis_read_lo(vis_ld_u8_i(sp1, 2)),
		    vis_read_lo(vis_ld_u8_i(sp1, 3)));
		d3 = vis_fpmerge(vis_read_lo(vis_ld_u8_i(sp2, 2)),
		    vis_read_lo(vis_ld_u8_i(sp2, 3)));
		acc1 = vis_fpmerge(vis_read_lo(d0), vis_read_lo(d1));
		acc2 = vis_fpmerge(vis_read_lo(d2), vis_read_lo(d3));

#pragma pipeloop(0)
		for (i = 0; i <= size - 4; i += 2) {
			mlib_s32 cx0, cx1;

			xfd0 = xb[i];
			xfd1 = xb[i + 1];

			cx0 = x >> x_shift;
			sp1 = sp0 + cx0;
			x += dx;
			cx1 = x >> x_shift;
			sp2 = sp0 + cx1;
			x += dx;

			d0 = vis_fpmerge(vis_read_lo(vis_ld_u8_i(sp1, 0)),
			    vis_read_lo(vis_ld_u8_i(sp1, 1)));
			d1 = vis_fpmerge(vis_read_lo(vis_ld_u8_i(sp2, 0)),
			    vis_read_lo(vis_ld_u8_i(sp2, 1)));
			d2 = vis_fpmerge(vis_read_lo(vis_ld_u8_i(sp1, 2)),
			    vis_read_lo(vis_ld_u8_i(sp1, 3)));
			d3 = vis_fpmerge(vis_read_lo(vis_ld_u8_i(sp2, 2)),
			    vis_read_lo(vis_ld_u8_i(sp2, 3)));

			r0 = vis_fmul8x16(vis_read_lo(acc1), xfd0);
			r1 = vis_fmul8x16(vis_read_lo(acc2), xfd1);

			acc1 = vis_fpmerge(vis_read_lo(d0), vis_read_lo(d1));
			acc2 = vis_fpmerge(vis_read_lo(d2), vis_read_lo(d3));

			r2 = vis_fpadd16(r0, r1);
			fpbuff[(i >> 1)] =
			    vis_fpadd16s(vis_read_lo(r2), vis_read_hi(r2));
		}

		xfd0 = xb[i];
		xfd1 = xb[i + 1];

		r0 = vis_fmul8x16(vis_read_lo(acc1), xfd0);
		r1 = vis_fmul8x16(vis_read_lo(acc2), xfd1);

		r2 = vis_fpadd16(r0, r1);
		fpbuff[(i >> 1)] =
		    vis_fpadd16s(vis_read_lo(r2), vis_read_hi(r2));
		i += 2;
	}

	if ((size & 1) != 0) {
		sp1 = sp0 + (x >> x_shift);

		d0 = vis_fpmerge(vis_read_lo(vis_ld_u8_i(sp1, 0)),
		    vis_read_lo(vis_ld_u8_i(sp1, 1)));
		d2 = vis_fpmerge(vis_read_lo(vis_ld_u8_i(sp1, 2)),
		    vis_read_lo(vis_ld_u8_i(sp1, 3)));
		acc1 = vis_fpmerge(vis_read_lo(d0), vis_read_lo(d0));
		acc2 = vis_fpmerge(vis_read_lo(d2), vis_read_lo(d2));

		xfd0 = xb[i];
		xfd1 = xb[i + 1];

		r0 = vis_fmul8x16(vis_read_lo(acc1), xfd0);
		r1 = vis_fmul8x16(vis_read_lo(acc2), xfd1);

		r2 = vis_fpadd16(r0, r1);
		fpbuff[(i >> 1)] =
		    vis_fpadd16s(vis_read_lo(r2), vis_read_hi(r2));
	}
}

/* *********************************************************** */

void
mlib_calc_4x_1ch(
    VPARAMX)
{
	DECL_LINE;
	DECL_LINE_HI;
	FP_TYPE d2, d3, r1, r2;
	FP_TYPE xfd1;
	FP_TYPE acc2;
	mlib_f32 bb;

	i = 0;

	if (i <= size - 2) {

		sp1 = sp0 + (x >> x_shift);
		x += dx;
		sp2 = sp0 + (x >> x_shift);
		x += dx;

		d0 = vis_fpmerge(vis_read_lo(vis_ld_u8_i(sp1, 0)),
		    vis_read_lo(vis_ld_u8_i(sp1, 1)));
		d1 = vis_fpmerge(vis_read_lo(vis_ld_u8_i(sp2, 0)),
		    vis_read_lo(vis_ld_u8_i(sp2, 1)));
		d2 = vis_fpmerge(vis_read_lo(vis_ld_u8_i(sp1, 2)),
		    vis_read_lo(vis_ld_u8_i(sp1, 3)));
		d3 = vis_fpmerge(vis_read_lo(vis_ld_u8_i(sp2, 2)),
		    vis_read_lo(vis_ld_u8_i(sp2, 3)));
		acc1 = vis_fpmerge(vis_read_lo(d0), vis_read_lo(d1));
		acc2 = vis_fpmerge(vis_read_lo(d2), vis_read_lo(d3));

#pragma pipeloop(0)
		for (i = 0; i <= size - 4; i += 2) {
			mlib_s32 cx0, cx1;

			xfd0 = xb[i];
			xfd1 = xb[i + 1];
			bb = fpbuff[(i >> 1)];

			cx0 = x >> x_shift;
			sp1 = sp0 + cx0;
			x += dx;
			cx1 = x >> x_shift;
			sp2 = sp0 + cx1;
			x += dx;

			d0 = vis_fpmerge(vis_read_lo(vis_ld_u8_i(sp1, 0)),
			    vis_read_lo(vis_ld_u8_i(sp1, 1)));
			d1 = vis_fpmerge(vis_read_lo(vis_ld_u8_i(sp2, 0)),
			    vis_read_lo(vis_ld_u8_i(sp2, 1)));
			d2 = vis_fpmerge(vis_read_lo(vis_ld_u8_i(sp1, 2)),
			    vis_read_lo(vis_ld_u8_i(sp1, 3)));
			d3 = vis_fpmerge(vis_read_lo(vis_ld_u8_i(sp2, 2)),
			    vis_read_lo(vis_ld_u8_i(sp2, 3)));

			r0 = vis_fmul8x16(vis_read_lo(acc1), xfd0);
			r1 = vis_fmul8x16(vis_read_lo(acc2), xfd1);

			acc1 = vis_fpmerge(vis_read_lo(d0), vis_read_lo(d1));
			acc2 = vis_fpmerge(vis_read_lo(d2), vis_read_lo(d3));

			r2 = vis_fpadd16(r0, r1);
			fpbuff[(i >> 1)] =
			    vis_fpadd16s(bb, vis_fpadd16s(vis_read_lo(r2),
			    vis_read_hi(r2)));
		}

		xfd0 = xb[i];
		xfd1 = xb[i + 1];
		bb = fpbuff[(i >> 1)];

		r0 = vis_fmul8x16(vis_read_lo(acc1), xfd0);
		r1 = vis_fmul8x16(vis_read_lo(acc2), xfd1);

		r2 = vis_fpadd16(r0, r1);
		fpbuff[(i >> 1)] =
		    vis_fpadd16s(bb, vis_fpadd16s(vis_read_lo(r2),
		    vis_read_hi(r2)));
		i += 2;
	}

	if ((size & 1) != 0) {
		sp1 = sp0 + (x >> x_shift);

		d0 = vis_fpmerge(vis_read_lo(vis_ld_u8_i(sp1, 0)),
		    vis_read_lo(vis_ld_u8_i(sp1, 1)));
		d2 = vis_fpmerge(vis_read_lo(vis_ld_u8_i(sp1, 2)),
		    vis_read_lo(vis_ld_u8_i(sp1, 3)));
		acc1 = vis_fpmerge(vis_read_lo(d0), vis_read_lo(d0));
		acc2 = vis_fpmerge(vis_read_lo(d2), vis_read_lo(d2));

		xfd0 = xb[i];
		xfd1 = xb[i + 1];
		bb = fpbuff[(i >> 1)];

		r0 = vis_fmul8x16(vis_read_lo(acc1), xfd0);
		r1 = vis_fmul8x16(vis_read_lo(acc2), xfd1);

		r2 = vis_fpadd16(r0, r1);
		fpbuff[(i >> 1)] =
		    vis_fpadd16s(bb, vis_fpadd16s(vis_read_lo(r2),
		    vis_read_hi(r2)));
	}
}

/* *********************************************************** */

void
mlib_calc_3x_off_1ch(
    VPARAMX)
{
	DECL_LINE;
	DECL_LINE_HI;
	FP_TYPE d2, d3, r1, r2;
	FP_TYPE xfd1;
	FP_TYPE acc2;
	mlib_f32 null = 0;

	i = 0;

	if (i <= size - 2) {

		sp1 = sp0 + (x >> x_shift);
		x += dx;
		sp2 = sp0 + (x >> x_shift);
		x += dx;

		d0 = vis_fpmerge(vis_read_lo(vis_ld_u8_i(sp1, 0)),
		    vis_read_lo(vis_ld_u8_i(sp1, 1)));
		d1 = vis_fpmerge(vis_read_lo(vis_ld_u8_i(sp2, 0)),
		    vis_read_lo(vis_ld_u8_i(sp2, 1)));
		d2 = vis_fpmerge(vis_read_lo(vis_ld_u8_i(sp1, 2)), null);
		d3 = vis_fpmerge(vis_read_lo(vis_ld_u8_i(sp2, 2)), null);
		acc1 = vis_fpmerge(vis_read_lo(d0), vis_read_lo(d1));
		acc2 = vis_fpmerge(vis_read_lo(d2), vis_read_lo(d3));

#pragma pipeloop(0)
		for (i = 0; i <= size - 4; i += 2) {
			mlib_s32 cx0, cx1;

			xfd0 = xb[i];
			xfd1 = xb[i + 1];

			cx0 = x >> x_shift;
			sp1 = sp0 + cx0;
			x += dx;
			cx1 = x >> x_shift;
			sp2 = sp0 + cx1;
			x += dx;

			d0 = vis_fpmerge(vis_read_lo(vis_ld_u8_i(sp1, 0)),
			    vis_read_lo(vis_ld_u8_i(sp1, 1)));
			d1 = vis_fpmerge(vis_read_lo(vis_ld_u8_i(sp2, 0)),
			    vis_read_lo(vis_ld_u8_i(sp2, 1)));
			d2 = vis_fpmerge(vis_read_lo(vis_ld_u8_i(sp1, 2)),
			    null);
			d3 = vis_fpmerge(vis_read_lo(vis_ld_u8_i(sp2, 2)),
			    null);

			r0 = vis_fmul8x16(vis_read_lo(acc1), xfd0);
			r1 = vis_fmul8x16(vis_read_lo(acc2), xfd1);

			acc1 = vis_fpmerge(vis_read_lo(d0), vis_read_lo(d1));
			acc2 = vis_fpmerge(vis_read_lo(d2), vis_read_lo(d3));

			r2 = vis_fpadd16(r0, r1);
			fpbuff[(i >> 1)] =
			    vis_fpadd16s(vis_read_lo(r2), vis_read_hi(r2));
		}

		xfd0 = xb[i];
		xfd1 = xb[i + 1];

		r0 = vis_fmul8x16(vis_read_lo(acc1), xfd0);
		r1 = vis_fmul8x16(vis_read_lo(acc2), xfd1);

		r2 = vis_fpadd16(r0, r1);
		fpbuff[(i >> 1)] =
		    vis_fpadd16s(vis_read_lo(r2), vis_read_hi(r2));
		i += 2;
	}

	if ((size & 1) != 0) {
		sp1 = sp0 + (x >> x_shift);

		d0 = vis_fpmerge(vis_read_lo(vis_ld_u8_i(sp1, 0)),
		    vis_read_lo(vis_ld_u8_i(sp1, 1)));
		d2 = vis_fpmerge(vis_read_lo(vis_ld_u8_i(sp1, 2)), null);
		acc1 = vis_fpmerge(vis_read_lo(d0), vis_read_lo(d0));
		acc2 = vis_fpmerge(vis_read_lo(d2), vis_read_lo(d2));

		xfd0 = xb[i];
		xfd1 = xb[i + 1];

		r0 = vis_fmul8x16(vis_read_lo(acc1), xfd0);
		r1 = vis_fmul8x16(vis_read_lo(acc2), xfd1);

		r2 = vis_fpadd16(r0, r1);
		fpbuff[(i >> 1)] =
		    vis_fpadd16s(vis_read_lo(r2), vis_read_hi(r2));
	}
}

/* *********************************************************** */

void
mlib_calc_3x_1ch(
    VPARAMX)
{
	DECL_LINE;
	DECL_LINE_HI;
	FP_TYPE d2, d3, r1, r2;
	FP_TYPE xfd1;
	FP_TYPE acc2;
	mlib_f32 bb, null = 0;

	i = 0;

	if (i <= size - 2) {

		sp1 = sp0 + (x >> x_shift);
		x += dx;
		sp2 = sp0 + (x >> x_shift);
		x += dx;

		d0 = vis_fpmerge(vis_read_lo(vis_ld_u8_i(sp1, 0)),
		    vis_read_lo(vis_ld_u8_i(sp1, 1)));
		d1 = vis_fpmerge(vis_read_lo(vis_ld_u8_i(sp2, 0)),
		    vis_read_lo(vis_ld_u8_i(sp2, 1)));
		d2 = vis_fpmerge(vis_read_lo(vis_ld_u8_i(sp1, 2)), null);
		d3 = vis_fpmerge(vis_read_lo(vis_ld_u8_i(sp2, 2)), null);
		acc1 = vis_fpmerge(vis_read_lo(d0), vis_read_lo(d1));
		acc2 = vis_fpmerge(vis_read_lo(d2), vis_read_lo(d3));

#pragma pipeloop(0)
		for (i = 0; i <= size - 4; i += 2) {
			mlib_s32 cx0, cx1;

			xfd0 = xb[i];
			xfd1 = xb[i + 1];
			bb = fpbuff[(i >> 1)];

			cx0 = x >> x_shift;
			sp1 = sp0 + cx0;
			x += dx;
			cx1 = x >> x_shift;
			sp2 = sp0 + cx1;
			x += dx;

			d0 = vis_fpmerge(vis_read_lo(vis_ld_u8_i(sp1, 0)),
			    vis_read_lo(vis_ld_u8_i(sp1, 1)));
			d1 = vis_fpmerge(vis_read_lo(vis_ld_u8_i(sp2, 0)),
			    vis_read_lo(vis_ld_u8_i(sp2, 1)));
			d2 = vis_fpmerge(vis_read_lo(vis_ld_u8_i(sp1, 2)),
			    null);
			d3 = vis_fpmerge(vis_read_lo(vis_ld_u8_i(sp2, 2)),
			    null);

			r0 = vis_fmul8x16(vis_read_lo(acc1), xfd0);
			r1 = vis_fmul8x16(vis_read_lo(acc2), xfd1);

			acc1 = vis_fpmerge(vis_read_lo(d0), vis_read_lo(d1));
			acc2 = vis_fpmerge(vis_read_lo(d2), vis_read_lo(d3));

			r2 = vis_fpadd16(r0, r1);
			fpbuff[(i >> 1)] =
			    vis_fpadd16s(bb, vis_fpadd16s(vis_read_lo(r2),
			    vis_read_hi(r2)));
		}

		xfd0 = xb[i];
		xfd1 = xb[i + 1];
		bb = fpbuff[(i >> 1)];

		r0 = vis_fmul8x16(vis_read_lo(acc1), xfd0);
		r1 = vis_fmul8x16(vis_read_lo(acc2), xfd1);

		r2 = vis_fpadd16(r0, r1);
		fpbuff[(i >> 1)] =
		    vis_fpadd16s(bb, vis_fpadd16s(vis_read_lo(r2),
		    vis_read_hi(r2)));
		i += 2;
	}

	if ((size & 1) != 0) {
		sp1 = sp0 + (x >> x_shift);

		d0 = vis_fpmerge(vis_read_lo(vis_ld_u8_i(sp1, 0)),
		    vis_read_lo(vis_ld_u8_i(sp1, 1)));
		d2 = vis_fpmerge(vis_read_lo(vis_ld_u8_i(sp1, 2)), null);
		acc1 = vis_fpmerge(vis_read_lo(d0), vis_read_lo(d0));
		acc2 = vis_fpmerge(vis_read_lo(d2), vis_read_lo(d2));

		xfd0 = xb[i];
		xfd1 = xb[i + 1];
		bb = fpbuff[(i >> 1)];

		r0 = vis_fmul8x16(vis_read_lo(acc1), xfd0);
		r1 = vis_fmul8x16(vis_read_lo(acc2), xfd1);

		r2 = vis_fpadd16(r0, r1);
		fpbuff[(i >> 1)] =
		    vis_fpadd16s(bb, vis_fpadd16s(vis_read_lo(r2),
		    vis_read_hi(r2)));
	}
}

/* *********************************************************** */

void
mlib_calc_2x_off_1ch(
    VPARAMX)
{
	DECL_LINE;
	DECL_LINE_HI;

	i = 0;

	if (i <= size - 2) {

		sp1 = sp0 + (x >> x_shift);
		x += dx;
		sp2 = sp0 + (x >> x_shift);
		x += dx;

		d0 = vis_fpmerge(vis_read_lo(vis_ld_u8_i(sp1, 0)),
		    vis_read_lo(vis_ld_u8_i(sp1, 1)));
		d1 = vis_fpmerge(vis_read_lo(vis_ld_u8_i(sp2, 0)),
		    vis_read_lo(vis_ld_u8_i(sp2, 1)));
		acc1 = vis_fpmerge(vis_read_lo(d0), vis_read_lo(d1));

#pragma pipeloop(0)
		for (i = 0; i <= size - 4; i += 2) {
			mlib_s32 cx0, cx1;

			xfd0 = xb[i];

			cx0 = x >> x_shift;
			sp1 = sp0 + cx0;
			x += dx;
			cx1 = x >> x_shift;
			sp2 = sp0 + cx1;
			x += dx;

			d0 = vis_fpmerge(vis_read_lo(vis_ld_u8_i(sp1, 0)),
			    vis_read_lo(vis_ld_u8_i(sp1, 1)));
			d1 = vis_fpmerge(vis_read_lo(vis_ld_u8_i(sp2, 0)),
			    vis_read_lo(vis_ld_u8_i(sp2, 1)));

			r0 = vis_fmul8x16(vis_read_lo(acc1), xfd0);

			acc1 = vis_fpmerge(vis_read_lo(d0), vis_read_lo(d1));

			fpbuff[(i >> 1)] =
			    vis_fpadd16s(vis_read_lo(r0), vis_read_hi(r0));
		}

		xfd0 = xb[i];

		r0 = vis_fmul8x16(vis_read_lo(acc1), xfd0);

		fpbuff[(i >> 1)] =
		    vis_fpadd16s(vis_read_lo(r0), vis_read_hi(r0));
		i += 2;
	}

	if ((size & 1) != 0) {
		sp1 = sp0 + (x >> x_shift);

		d0 = vis_fpmerge(vis_read_lo(vis_ld_u8_i(sp1, 0)),
		    vis_read_lo(vis_ld_u8_i(sp1, 1)));
		acc1 = vis_fpmerge(vis_read_lo(d0), vis_read_lo(d0));

		xfd0 = xb[i];

		r0 = vis_fmul8x16(vis_read_lo(acc1), xfd0);
		fpbuff[(i >> 1)] =
		    vis_fpadd16s(vis_read_lo(r0), vis_read_hi(r0));
	}
}

/* *********************************************************** */

void
mlib_calc_2x_1ch(
    VPARAMX)
{
	DECL_LINE;
	DECL_LINE_HI;
	mlib_f32 bb;

	i = 0;

	if (i <= size - 2) {

		sp1 = sp0 + (x >> x_shift);
		x += dx;
		sp2 = sp0 + (x >> x_shift);
		x += dx;

		d0 = vis_fpmerge(vis_read_lo(vis_ld_u8_i(sp1, 0)),
		    vis_read_lo(vis_ld_u8_i(sp1, 1)));
		d1 = vis_fpmerge(vis_read_lo(vis_ld_u8_i(sp2, 0)),
		    vis_read_lo(vis_ld_u8_i(sp2, 1)));
		acc1 = vis_fpmerge(vis_read_lo(d0), vis_read_lo(d1));

#pragma pipeloop(0)
		for (i = 0; i <= size - 4; i += 2) {
			mlib_s32 cx0, cx1;

			xfd0 = xb[i];
			bb = fpbuff[(i >> 1)];

			cx0 = x >> x_shift;
			sp1 = sp0 + cx0;
			x += dx;
			cx1 = x >> x_shift;
			sp2 = sp0 + cx1;
			x += dx;

			d0 = vis_fpmerge(vis_read_lo(vis_ld_u8_i(sp1, 0)),
			    vis_read_lo(vis_ld_u8_i(sp1, 1)));
			d1 = vis_fpmerge(vis_read_lo(vis_ld_u8_i(sp2, 0)),
			    vis_read_lo(vis_ld_u8_i(sp2, 1)));

			r0 = vis_fmul8x16(vis_read_lo(acc1), xfd0);

			acc1 = vis_fpmerge(vis_read_lo(d0), vis_read_lo(d1));

			fpbuff[(i >> 1)] =
			    vis_fpadd16s(bb, vis_fpadd16s(vis_read_lo(r0),
			    vis_read_hi(r0)));
		}

		xfd0 = xb[i];
		bb = fpbuff[(i >> 1)];

		r0 = vis_fmul8x16(vis_read_lo(acc1), xfd0);

		fpbuff[(i >> 1)] =
		    vis_fpadd16s(bb, vis_fpadd16s(vis_read_lo(r0),
		    vis_read_hi(r0)));
		i += 2;
	}

	if ((size & 1) != 0) {
		sp1 = sp0 + (x >> x_shift);

		d0 = vis_fpmerge(vis_read_lo(vis_ld_u8_i(sp1, 0)),
		    vis_read_lo(vis_ld_u8_i(sp1, 1)));
		acc1 = vis_fpmerge(vis_read_lo(d0), vis_read_lo(d0));

		xfd0 = xb[i];
		bb = fpbuff[(i >> 1)];

		r0 = vis_fmul8x16(vis_read_lo(acc1), xfd0);
		fpbuff[(i >> 1)] =
		    vis_fpadd16s(bb, vis_fpadd16s(vis_read_lo(r0),
		    vis_read_hi(r0)));
	}
}

/* *********************************************************** */

void
mlib_calc_4y_4x_1ch(
    VPARAMXY4)
{
	DECL_LINE;
	DECL_LINE_LOW;
	FP_TYPE d1, d2, d3;
	FP_TYPE xfd1, xfd2, xfd3;
	FP_TYPE yfd1, yfd2, yfd3;
	FP_TYPE acc2;
	mlib_s32 x1, x2, x3, x4;

	yfd0 = fptr[0];
	yfd1 = fptr[1];
	yfd2 = fptr[2];
	yfd3 = fptr[3];

#pragma pipeloop(0)
	for (i = 0; i <= size - 4; i += 4) {
		CALC_X_4();
		xfd0 = xb[i];
		xfd1 = xb[i + 1];
		xfd2 = xb[i + 2];
		xfd3 = xb[i + 3];
		bb = ybuff[(i >> 2)];

		CALC_SRCX_4(sp0);
		LD_4x32_U8(acc1, acc2, sp1, sp2, sp3, sp4);
		MULX_4();
		d0 = FMUL_16x16(d0, yfd0);
		bb = vis_fpadd16(bb, d0);

		CALC_SRCX_4(spp0);
		LD_4x32_U8(acc1, acc2, sp1, sp2, sp3, sp4);
		MULX_4();
		d0 = FMUL_16x16(d0, yfd1);
		bb = vis_fpadd16(bb, d0);

		CALC_SRCX_4(sp10);
		LD_4x32_U8(acc1, acc2, sp1, sp2, sp3, sp4);
		MULX_4();
		d0 = FMUL_16x16(d0, yfd2);
		bb = vis_fpadd16(bb, d0);

		CALC_SRCX_4(sp20);
		LD_4x32_U8(acc1, acc2, sp1, sp2, sp3, sp4);
		MULX_4();
		d0 = FMUL_16x16(d0, yfd3);
		ybuff[(i >> 2)] = vis_fpadd16(bb, d0);
		x += delta;
	}

	if ((size & 3) != 0) {
		CALC_X_3();
		xfd0 = xb[i];
		xfd1 = xb[i + 1];
		xfd2 = xb[i + 2];
		xfd3 = xb[i + 3];
		bb = ybuff[(i >> 2)];

		if ((size & 3) == 1) {
			x4 = x3 = x2 = x1;
		} else if ((size & 3) == 2) {
			x4 = x3 = x2;
		} else {
			x4 = x3;
		}

		CALC_SRCX_4(sp0);
		LD_4x32_U8(acc1, acc2, sp1, sp2, sp3, sp4);
		MULX_4();
		d0 = FMUL_16x16(d0, yfd0);
		bb = vis_fpadd16(bb, d0);

		CALC_SRCX_4(spp0);
		LD_4x32_U8(acc1, acc2, sp1, sp2, sp3, sp4);
		MULX_4();
		d0 = FMUL_16x16(d0, yfd1);
		bb = vis_fpadd16(bb, d0);

		CALC_SRCX_4(sp10);
		LD_4x32_U8(acc1, acc2, sp1, sp2, sp3, sp4);
		MULX_4();
		d0 = FMUL_16x16(d0, yfd2);
		bb = vis_fpadd16(bb, d0);

		CALC_SRCX_4(sp20);
		LD_4x32_U8(acc1, acc2, sp1, sp2, sp3, sp4);
		MULX_4();
		d0 = FMUL_16x16(d0, yfd3);
		ybuff[(i >> 2)] = vis_fpadd16(bb, d0);
	}
}

/* *********************************************************** */

void
mlib_calc_4y_3x_1ch(
    VPARAMXY4)
{
	DECL_LINE;
	DECL_LINE_LOW;
	FP_TYPE d1, d2;
	FP_TYPE xfd1, xfd2;
	FP_TYPE yfd1, yfd2, yfd3;
	FP_TYPE acc2;
	mlib_s32 x1, x2, x3, x4;

	yfd0 = fptr[0];
	yfd1 = fptr[1];
	yfd2 = fptr[2];
	yfd3 = fptr[3];

#pragma pipeloop(0)
	for (i = 0; i <= size - 4; i += 4) {
		CALC_X_4();
		xfd0 = xb[i];
		xfd1 = xb[i + 1];
		xfd2 = xb[i + 2];
		bb = ybuff[(i >> 2)];

		CALC_SRCX_4(sp0);
		LD_3x32_U8(acc1, acc2, sp1, sp2, sp3, sp4);
		MULX_3();
		d0 = FMUL_16x16(d0, yfd0);
		bb = vis_fpadd16(bb, d0);

		CALC_SRCX_4(spp0);
		LD_3x32_U8(acc1, acc2, sp1, sp2, sp3, sp4);
		MULX_3();
		d0 = FMUL_16x16(d0, yfd1);
		bb = vis_fpadd16(bb, d0);

		CALC_SRCX_4(sp10);
		LD_3x32_U8(acc1, acc2, sp1, sp2, sp3, sp4);
		MULX_3();
		d0 = FMUL_16x16(d0, yfd2);
		bb = vis_fpadd16(bb, d0);

		CALC_SRCX_4(sp20);
		LD_3x32_U8(acc1, acc2, sp1, sp2, sp3, sp4);
		MULX_3();
		d0 = FMUL_16x16(d0, yfd3);
		ybuff[(i >> 2)] = vis_fpadd16(bb, d0);
		x += delta;
	}

	if ((size & 3) != 0) {
		CALC_X_3();
		xfd0 = xb[i];
		xfd1 = xb[i + 1];
		xfd2 = xb[i + 2];
		bb = ybuff[(i >> 2)];

		if ((size & 3) == 1) {
			x4 = x3 = x2 = x1;
		} else if ((size & 3) == 2) {
			x4 = x3 = x2;
		} else {
			x4 = x3;
		}

		CALC_SRCX_4(sp0);
		LD_3x32_U8(acc1, acc2, sp1, sp2, sp3, sp4);
		MULX_3();
		d0 = FMUL_16x16(d0, yfd0);
		bb = vis_fpadd16(bb, d0);

		CALC_SRCX_4(spp0);
		LD_3x32_U8(acc1, acc2, sp1, sp2, sp3, sp4);
		MULX_3();
		d0 = FMUL_16x16(d0, yfd1);
		bb = vis_fpadd16(bb, d0);

		CALC_SRCX_4(sp10);
		LD_3x32_U8(acc1, acc2, sp1, sp2, sp3, sp4);
		MULX_3();
		d0 = FMUL_16x16(d0, yfd2);
		bb = vis_fpadd16(bb, d0);

		CALC_SRCX_4(sp20);
		LD_3x32_U8(acc1, acc2, sp1, sp2, sp3, sp4);
		MULX_3();
		d0 = FMUL_16x16(d0, yfd3);
		ybuff[(i >> 2)] = vis_fpadd16(bb, d0);
	}
}

/* *********************************************************** */

void
mlib_calc_4y_2x_1ch(
    VPARAMXY4)
{
	DECL_LINE;
	DECL_LINE_LOW;
	FP_TYPE d1;
	FP_TYPE xfd1;
	FP_TYPE yfd1, yfd2, yfd3;
	mlib_s32 x1, x2, x3, x4;

	yfd0 = fptr[0];
	yfd1 = fptr[1];
	yfd2 = fptr[2];
	yfd3 = fptr[3];

#pragma pipeloop(0)
	for (i = 0; i <= size - 4; i += 4) {
		CALC_X_4();
		xfd0 = xb[i];
		xfd1 = xb[i + 1];
		bb = ybuff[(i >> 2)];

		CALC_SRCX_4(sp0);
		LD_2x32_U8(acc1, sp1, sp2, sp3, sp4);
		MULX_2();
		d0 = FMUL_16x16(d0, yfd0);
		bb = vis_fpadd16(bb, d0);

		CALC_SRCX_4(spp0);
		LD_2x32_U8(acc1, sp1, sp2, sp3, sp4);
		MULX_2();
		d0 = FMUL_16x16(d0, yfd1);
		bb = vis_fpadd16(bb, d0);

		CALC_SRCX_4(sp10);
		LD_2x32_U8(acc1, sp1, sp2, sp3, sp4);
		MULX_2();
		d0 = FMUL_16x16(d0, yfd2);
		bb = vis_fpadd16(bb, d0);

		CALC_SRCX_4(sp20);
		LD_2x32_U8(acc1, sp1, sp2, sp3, sp4);
		MULX_2();
		d0 = FMUL_16x16(d0, yfd3);
		ybuff[(i >> 2)] = vis_fpadd16(bb, d0);
		x += delta;
	}

	if ((size & 3) != 0) {
		CALC_X_3();
		xfd0 = xb[i];
		xfd1 = xb[i + 1];
		bb = ybuff[(i >> 2)];

		if ((size & 3) == 1) {
			x4 = x3 = x2 = x1;
		} else if ((size & 3) == 2) {
			x4 = x3 = x2;
		} else {
			x4 = x3;
		}

		CALC_SRCX_4(sp0);
		LD_2x32_U8(acc1, sp1, sp2, sp3, sp4);
		MULX_2();
		d0 = FMUL_16x16(d0, yfd0);
		bb = vis_fpadd16(bb, d0);

		CALC_SRCX_4(spp0);
		LD_2x32_U8(acc1, sp1, sp2, sp3, sp4);
		MULX_2();
		d0 = FMUL_16x16(d0, yfd1);
		bb = vis_fpadd16(bb, d0);

		CALC_SRCX_4(sp10);
		LD_2x32_U8(acc1, sp1, sp2, sp3, sp4);
		MULX_2();
		d0 = FMUL_16x16(d0, yfd2);
		bb = vis_fpadd16(bb, d0);

		CALC_SRCX_4(sp20);
		LD_2x32_U8(acc1, sp1, sp2, sp3, sp4);
		MULX_2();
		d0 = FMUL_16x16(d0, yfd3);
		ybuff[(i >> 2)] = vis_fpadd16(bb, d0);
	}
}

/* *********************************************************** */

void
mlib_calc_4y_1x_1ch(
    VPARAMXY4)
{
	DECL_LINE;
	DECL_LINE_LOW;
	FP_TYPE yfd1, yfd2, yfd3;
	mlib_s32 x1, x2, x3, x4;

	yfd0 = fptr[0];
	yfd1 = fptr[1];
	yfd2 = fptr[2];
	yfd3 = fptr[3];

#pragma pipeloop(0)
	for (i = 0; i <= size - 4; i += 4) {
		CALC_X_4();
		xfd0 = xb[i];
		bb = ybuff[(i >> 2)];

		CALC_SRCX_4(sp0);
		LD_1x32_U8(acc1, sp1, sp2, sp3, sp4);
		d0 = vis_fmul8x16(vis_read_hi(acc1), xfd0);
		d0 = FMUL_16x16(d0, yfd0);
		bb = vis_fpadd16(bb, d0);

		CALC_SRCX_4(spp0);
		LD_1x32_U8(acc1, sp1, sp2, sp3, sp4);
		d0 = vis_fmul8x16(vis_read_hi(acc1), xfd0);
		d0 = FMUL_16x16(d0, yfd1);
		bb = vis_fpadd16(bb, d0);

		CALC_SRCX_4(sp10);
		LD_1x32_U8(acc1, sp1, sp2, sp3, sp4);
		d0 = vis_fmul8x16(vis_read_hi(acc1), xfd0);
		d0 = FMUL_16x16(d0, yfd2);
		bb = vis_fpadd16(bb, d0);

		CALC_SRCX_4(sp20);
		LD_1x32_U8(acc1, sp1, sp2, sp3, sp4);
		d0 = vis_fmul8x16(vis_read_hi(acc1), xfd0);
		d0 = FMUL_16x16(d0, yfd3);
		ybuff[(i >> 2)] = vis_fpadd16(bb, d0);
		x += delta;
	}

	if ((size & 3) != 0) {
		CALC_X_3();
		xfd0 = xb[i];
		bb = ybuff[(i >> 2)];

		if ((size & 3) == 1) {
			x4 = x3 = x2 = x1;
		} else if ((size & 3) == 2) {
			x4 = x3 = x2;
		} else {
			x4 = x3;
		}

		CALC_SRCX_4(sp0);
		LD_1x32_U8(acc1, sp1, sp2, sp3, sp4);
		d0 = vis_fmul8x16(vis_read_hi(acc1), xfd0);
		d0 = FMUL_16x16(d0, yfd0);
		bb = vis_fpadd16(bb, d0);

		CALC_SRCX_4(spp0);
		LD_1x32_U8(acc1, sp1, sp2, sp3, sp4);
		d0 = vis_fmul8x16(vis_read_hi(acc1), xfd0);
		d0 = FMUL_16x16(d0, yfd1);
		bb = vis_fpadd16(bb, d0);

		CALC_SRCX_4(sp10);
		LD_1x32_U8(acc1, sp1, sp2, sp3, sp4);
		d0 = vis_fmul8x16(vis_read_hi(acc1), xfd0);
		d0 = FMUL_16x16(d0, yfd2);
		bb = vis_fpadd16(bb, d0);

		CALC_SRCX_4(sp20);
		LD_1x32_U8(acc1, sp1, sp2, sp3, sp4);
		d0 = vis_fmul8x16(vis_read_hi(acc1), xfd0);
		d0 = FMUL_16x16(d0, yfd3);
		ybuff[(i >> 2)] = vis_fpadd16(bb, d0);
	}
}

/* *********************************************************** */

void
mlib_calc_3y_4x_1ch(
    VPARAMXY3)
{
	DECL_LINE;
	DECL_LINE_LOW;
	FP_TYPE d1, d2, d3;
	FP_TYPE xfd1, xfd2, xfd3;
	FP_TYPE yfd1, yfd2;
	FP_TYPE acc2;
	mlib_s32 x1, x2, x3, x4;

	yfd0 = fptr[0];
	yfd1 = fptr[1];
	yfd2 = fptr[2];

#pragma pipeloop(0)
	for (i = 0; i <= size - 4; i += 4) {
		CALC_X_4();
		xfd0 = xb[i];
		xfd1 = xb[i + 1];
		xfd2 = xb[i + 2];
		xfd3 = xb[i + 3];
		bb = ybuff[(i >> 2)];

		CALC_SRCX_4(sp0);
		LD_4x32_U8(acc1, acc2, sp1, sp2, sp3, sp4);
		MULX_4();
		d0 = FMUL_16x16(d0, yfd0);
		bb = vis_fpadd16(bb, d0);

		CALC_SRCX_4(spp0);
		LD_4x32_U8(acc1, acc2, sp1, sp2, sp3, sp4);
		MULX_4();
		d0 = FMUL_16x16(d0, yfd1);
		bb = vis_fpadd16(bb, d0);

		CALC_SRCX_4(sp10);
		LD_4x32_U8(acc1, acc2, sp1, sp2, sp3, sp4);
		MULX_4();
		d0 = FMUL_16x16(d0, yfd2);
		ybuff[(i >> 2)] = vis_fpadd16(bb, d0);
		x += delta;
	}

	if ((size & 3) != 0) {
		CALC_X_3();
		xfd0 = xb[i];
		xfd1 = xb[i + 1];
		xfd2 = xb[i + 2];
		xfd3 = xb[i + 3];
		bb = ybuff[(i >> 2)];

		if ((size & 3) == 1) {
			x4 = x3 = x2 = x1;
		} else if ((size & 3) == 2) {
			x4 = x3 = x2;
		} else {
			x4 = x3;
		}

		CALC_SRCX_4(sp0);
		LD_4x32_U8(acc1, acc2, sp1, sp2, sp3, sp4);
		MULX_4();
		d0 = FMUL_16x16(d0, yfd0);
		bb = vis_fpadd16(bb, d0);

		CALC_SRCX_4(spp0);
		LD_4x32_U8(acc1, acc2, sp1, sp2, sp3, sp4);
		MULX_4();
		d0 = FMUL_16x16(d0, yfd1);
		bb = vis_fpadd16(bb, d0);

		CALC_SRCX_4(sp10);
		LD_4x32_U8(acc1, acc2, sp1, sp2, sp3, sp4);
		MULX_4();
		d0 = FMUL_16x16(d0, yfd2);
		ybuff[(i >> 2)] = vis_fpadd16(bb, d0);
	}
}

/* *********************************************************** */

void
mlib_calc_3y_3x_1ch(
    VPARAMXY3)
{
	DECL_LINE;
	DECL_LINE_LOW;
	FP_TYPE d1, d2;
	FP_TYPE xfd1, xfd2;
	FP_TYPE yfd1, yfd2;
	FP_TYPE acc2;
	mlib_s32 x1, x2, x3, x4;

	yfd0 = fptr[0];
	yfd1 = fptr[1];
	yfd2 = fptr[2];

#pragma pipeloop(0)
	for (i = 0; i <= size - 4; i += 4) {
		CALC_X_4();
		xfd0 = xb[i];
		xfd1 = xb[i + 1];
		xfd2 = xb[i + 2];
		bb = ybuff[(i >> 2)];

		CALC_SRCX_4(sp0);
		LD_3x32_U8(acc1, acc2, sp1, sp2, sp3, sp4);
		MULX_3();
		d0 = FMUL_16x16(d0, yfd0);
		bb = vis_fpadd16(bb, d0);

		CALC_SRCX_4(spp0);
		LD_3x32_U8(acc1, acc2, sp1, sp2, sp3, sp4);
		MULX_3();
		d0 = FMUL_16x16(d0, yfd1);
		bb = vis_fpadd16(bb, d0);

		CALC_SRCX_4(sp10);
		LD_3x32_U8(acc1, acc2, sp1, sp2, sp3, sp4);
		MULX_3();
		d0 = FMUL_16x16(d0, yfd2);
		ybuff[(i >> 2)] = vis_fpadd16(bb, d0);
		x += delta;
	}

	if ((size & 3) != 0) {
		CALC_X_3();
		xfd0 = xb[i];
		xfd1 = xb[i + 1];
		xfd2 = xb[i + 2];
		bb = ybuff[(i >> 2)];

		if ((size & 3) == 1) {
			x4 = x3 = x2 = x1;
		} else if ((size & 3) == 2) {
			x4 = x3 = x2;
		} else {
			x4 = x3;
		}

		CALC_SRCX_4(sp0);
		LD_3x32_U8(acc1, acc2, sp1, sp2, sp3, sp4);
		MULX_3();
		d0 = FMUL_16x16(d0, yfd0);
		bb = vis_fpadd16(bb, d0);

		CALC_SRCX_4(spp0);
		LD_3x32_U8(acc1, acc2, sp1, sp2, sp3, sp4);
		MULX_3();
		d0 = FMUL_16x16(d0, yfd1);
		bb = vis_fpadd16(bb, d0);

		CALC_SRCX_4(sp10);
		LD_3x32_U8(acc1, acc2, sp1, sp2, sp3, sp4);
		MULX_3();
		d0 = FMUL_16x16(d0, yfd2);
		ybuff[(i >> 2)] = vis_fpadd16(bb, d0);
	}
}

/* *********************************************************** */

void
mlib_calc_3y_2x_1ch(
    VPARAMXY3)
{
	DECL_LINE;
	DECL_LINE_LOW;
	FP_TYPE d1;
	FP_TYPE xfd1;
	FP_TYPE yfd1, yfd2;
	mlib_s32 x1, x2, x3, x4;

	yfd0 = fptr[0];
	yfd1 = fptr[1];
	yfd2 = fptr[2];

#pragma pipeloop(0)
	for (i = 0; i <= size - 4; i += 4) {
		CALC_X_4();
		xfd0 = xb[i];
		xfd1 = xb[i + 1];
		bb = ybuff[(i >> 2)];

		CALC_SRCX_4(sp0);
		LD_2x32_U8(acc1, sp1, sp2, sp3, sp4);
		MULX_2();
		d0 = FMUL_16x16(d0, yfd0);
		bb = vis_fpadd16(bb, d0);

		CALC_SRCX_4(spp0);
		LD_2x32_U8(acc1, sp1, sp2, sp3, sp4);
		MULX_2();
		d0 = FMUL_16x16(d0, yfd1);
		bb = vis_fpadd16(bb, d0);

		CALC_SRCX_4(sp10);
		LD_2x32_U8(acc1, sp1, sp2, sp3, sp4);
		MULX_2();
		d0 = FMUL_16x16(d0, yfd2);
		ybuff[(i >> 2)] = vis_fpadd16(bb, d0);
		x += delta;
	}

	if ((size & 3) != 0) {
		CALC_X_3();
		xfd0 = xb[i];
		xfd1 = xb[i + 1];
		bb = ybuff[(i >> 2)];

		if ((size & 3) == 1) {
			x4 = x3 = x2 = x1;
		} else if ((size & 3) == 2) {
			x4 = x3 = x2;
		} else {
			x4 = x3;
		}

		CALC_SRCX_4(sp0);
		LD_2x32_U8(acc1, sp1, sp2, sp3, sp4);
		MULX_2();
		d0 = FMUL_16x16(d0, yfd0);
		bb = vis_fpadd16(bb, d0);

		CALC_SRCX_4(spp0);
		LD_2x32_U8(acc1, sp1, sp2, sp3, sp4);
		MULX_2();
		d0 = FMUL_16x16(d0, yfd1);
		bb = vis_fpadd16(bb, d0);

		CALC_SRCX_4(sp10);
		LD_2x32_U8(acc1, sp1, sp2, sp3, sp4);
		MULX_2();
		d0 = FMUL_16x16(d0, yfd2);
		ybuff[(i >> 2)] = vis_fpadd16(bb, d0);
	}
}

/* *********************************************************** */

void
mlib_calc_3y_1x_1ch(
    VPARAMXY3)
{
	DECL_LINE;
	DECL_LINE_LOW;
	FP_TYPE yfd1, yfd2;
	mlib_s32 x1, x2, x3, x4;

	yfd0 = fptr[0];
	yfd1 = fptr[1];
	yfd2 = fptr[2];

#pragma pipeloop(0)
	for (i = 0; i <= size - 4; i += 4) {
		CALC_X_4();
		xfd0 = xb[i];
		bb = ybuff[(i >> 2)];

		CALC_SRCX_4(sp0);
		LD_1x32_U8(acc1, sp1, sp2, sp3, sp4);
		d0 = vis_fmul8x16(vis_read_hi(acc1), xfd0);
		d0 = FMUL_16x16(d0, yfd0);
		bb = vis_fpadd16(bb, d0);

		CALC_SRCX_4(spp0);
		LD_1x32_U8(acc1, sp1, sp2, sp3, sp4);
		d0 = vis_fmul8x16(vis_read_hi(acc1), xfd0);
		d0 = FMUL_16x16(d0, yfd1);
		bb = vis_fpadd16(bb, d0);

		CALC_SRCX_4(sp10);
		LD_1x32_U8(acc1, sp1, sp2, sp3, sp4);
		d0 = vis_fmul8x16(vis_read_hi(acc1), xfd0);
		d0 = FMUL_16x16(d0, yfd2);
		ybuff[(i >> 2)] = vis_fpadd16(bb, d0);
		x += delta;
	}

	if ((size & 3) != 0) {
		CALC_X_3();
		xfd0 = xb[i];
		bb = ybuff[(i >> 2)];

		if ((size & 3) == 1) {
			x4 = x3 = x2 = x1;
		} else if ((size & 3) == 2) {
			x4 = x3 = x2;
		} else {
			x4 = x3;
		}

		CALC_SRCX_4(sp0);
		LD_1x32_U8(acc1, sp1, sp2, sp3, sp4);
		d0 = vis_fmul8x16(vis_read_hi(acc1), xfd0);
		d0 = FMUL_16x16(d0, yfd0);
		bb = vis_fpadd16(bb, d0);

		CALC_SRCX_4(spp0);
		LD_1x32_U8(acc1, sp1, sp2, sp3, sp4);
		d0 = vis_fmul8x16(vis_read_hi(acc1), xfd0);
		d0 = FMUL_16x16(d0, yfd1);
		bb = vis_fpadd16(bb, d0);

		CALC_SRCX_4(sp10);
		LD_1x32_U8(acc1, sp1, sp2, sp3, sp4);
		d0 = vis_fmul8x16(vis_read_hi(acc1), xfd0);
		d0 = FMUL_16x16(d0, yfd2);
		ybuff[(i >> 2)] = vis_fpadd16(bb, d0);
	}
}

/* *********************************************************** */

void
mlib_calc_2y_4x_1ch(
    VPARAMXY2)
{
	DECL_LINE;
	DECL_LINE_LOW;
	FP_TYPE d1, d2, d3;
	FP_TYPE xfd1, xfd2, xfd3;
	FP_TYPE yfd1;
	FP_TYPE acc2;
	mlib_s32 x1, x2, x3, x4;

	yfd0 = fptr[0];
	yfd1 = fptr[1];

#pragma pipeloop(0)
	for (i = 0; i <= size - 4; i += 4) {
		CALC_X_4();
		xfd0 = xb[i];
		xfd1 = xb[i + 1];
		xfd2 = xb[i + 2];
		xfd3 = xb[i + 3];
		bb = ybuff[(i >> 2)];

		CALC_SRCX_4(sp0);
		LD_4x32_U8(acc1, acc2, sp1, sp2, sp3, sp4);
		MULX_4();
		d0 = FMUL_16x16(d0, yfd0);
		bb = vis_fpadd16(bb, d0);

		CALC_SRCX_4(spp0);
		LD_4x32_U8(acc1, acc2, sp1, sp2, sp3, sp4);
		MULX_4();
		d0 = FMUL_16x16(d0, yfd1);
		ybuff[(i >> 2)] = vis_fpadd16(bb, d0);
		x += delta;
	}

	if ((size & 3) != 0) {
		CALC_X_3();
		xfd0 = xb[i];
		xfd1 = xb[i + 1];
		xfd2 = xb[i + 2];
		xfd3 = xb[i + 3];
		bb = ybuff[(i >> 2)];

		if ((size & 3) == 1) {
			x4 = x3 = x2 = x1;
		} else if ((size & 3) == 2) {
			x4 = x3 = x2;
		} else {
			x4 = x3;
		}

		CALC_SRCX_4(sp0);
		LD_4x32_U8(acc1, acc2, sp1, sp2, sp3, sp4);
		MULX_4();
		d0 = FMUL_16x16(d0, yfd0);
		bb = vis_fpadd16(bb, d0);

		CALC_SRCX_4(spp0);
		LD_4x32_U8(acc1, acc2, sp1, sp2, sp3, sp4);
		MULX_4();
		d0 = FMUL_16x16(d0, yfd1);
		ybuff[(i >> 2)] = vis_fpadd16(bb, d0);
	}
}

/* *********************************************************** */

void
mlib_calc_2y_3x_1ch(
    VPARAMXY2)
{
	DECL_LINE;
	DECL_LINE_LOW;
	FP_TYPE d1, d2;
	FP_TYPE xfd1, xfd2;
	FP_TYPE yfd1;
	FP_TYPE acc2;
	mlib_s32 x1, x2, x3, x4;

	yfd0 = fptr[0];
	yfd1 = fptr[1];

#pragma pipeloop(0)
	for (i = 0; i <= size - 4; i += 4) {
		CALC_X_4();
		xfd0 = xb[i];
		xfd1 = xb[i + 1];
		xfd2 = xb[i + 2];
		bb = ybuff[(i >> 2)];

		CALC_SRCX_4(sp0);
		LD_3x32_U8(acc1, acc2, sp1, sp2, sp3, sp4);
		MULX_3();
		d0 = FMUL_16x16(d0, yfd0);
		bb = vis_fpadd16(bb, d0);

		CALC_SRCX_4(spp0);
		LD_3x32_U8(acc1, acc2, sp1, sp2, sp3, sp4);
		MULX_3();
		d0 = FMUL_16x16(d0, yfd1);
		ybuff[(i >> 2)] = vis_fpadd16(bb, d0);
		x += delta;
	}

	if ((size & 3) != 0) {
		CALC_X_3();
		xfd0 = xb[i];
		xfd1 = xb[i + 1];
		xfd2 = xb[i + 2];
		bb = ybuff[(i >> 2)];

		if ((size & 3) == 1) {
			x4 = x3 = x2 = x1;
		} else if ((size & 3) == 2) {
			x4 = x3 = x2;
		} else {
			x4 = x3;
		}

		CALC_SRCX_4(sp0);
		LD_3x32_U8(acc1, acc2, sp1, sp2, sp3, sp4);
		MULX_3();
		d0 = FMUL_16x16(d0, yfd0);
		bb = vis_fpadd16(bb, d0);

		CALC_SRCX_4(spp0);
		LD_3x32_U8(acc1, acc2, sp1, sp2, sp3, sp4);
		MULX_3();
		d0 = FMUL_16x16(d0, yfd1);
		ybuff[(i >> 2)] = vis_fpadd16(bb, d0);
	}
}

/* *********************************************************** */

void
mlib_calc_2y_2x_1ch(
    VPARAMXY2)
{
	DECL_LINE;
	DECL_LINE_LOW;
	FP_TYPE d1;
	FP_TYPE xfd1;
	FP_TYPE yfd1;
	mlib_s32 x1, x2, x3, x4;

	yfd0 = fptr[0];
	yfd1 = fptr[1];

#pragma pipeloop(0)
	for (i = 0; i <= size - 4; i += 4) {
		CALC_X_4();
		xfd0 = xb[i];
		xfd1 = xb[i + 1];
		bb = ybuff[(i >> 2)];

		CALC_SRCX_4(sp0);
		LD_2x32_U8(acc1, sp1, sp2, sp3, sp4);
		MULX_2();
		d0 = FMUL_16x16(d0, yfd0);
		bb = vis_fpadd16(bb, d0);

		CALC_SRCX_4(spp0);
		LD_2x32_U8(acc1, sp1, sp2, sp3, sp4);
		MULX_2();
		d0 = FMUL_16x16(d0, yfd1);
		ybuff[(i >> 2)] = vis_fpadd16(bb, d0);
		x += delta;
	}

	if ((size & 3) != 0) {
		CALC_X_3();
		xfd0 = xb[i];
		xfd1 = xb[i + 1];
		bb = ybuff[(i >> 2)];

		if ((size & 3) == 1) {
			x4 = x3 = x2 = x1;
		} else if ((size & 3) == 2) {
			x4 = x3 = x2;
		} else {
			x4 = x3;
		}

		CALC_SRCX_4(sp0);
		LD_2x32_U8(acc1, sp1, sp2, sp3, sp4);
		MULX_2();
		d0 = FMUL_16x16(d0, yfd0);
		bb = vis_fpadd16(bb, d0);

		CALC_SRCX_4(spp0);
		LD_2x32_U8(acc1, sp1, sp2, sp3, sp4);
		MULX_2();
		d0 = FMUL_16x16(d0, yfd1);
		ybuff[(i >> 2)] = vis_fpadd16(bb, d0);
	}
}

/* *********************************************************** */

void
mlib_calc_2y_1x_1ch(
    VPARAMXY2)
{
	DECL_LINE;
	DECL_LINE_LOW;
	FP_TYPE yfd1;
	mlib_s32 x1, x2, x3, x4;

	yfd0 = fptr[0];
	yfd1 = fptr[1];

#pragma pipeloop(0)
	for (i = 0; i <= size - 4; i += 4) {
		CALC_X_4();
		xfd0 = xb[i];
		bb = ybuff[(i >> 2)];

		CALC_SRCX_4(sp0);
		LD_1x32_U8(acc1, sp1, sp2, sp3, sp4);
		d0 = vis_fmul8x16(vis_read_hi(acc1), xfd0);
		d0 = FMUL_16x16(d0, yfd0);
		bb = vis_fpadd16(bb, d0);

		CALC_SRCX_4(spp0);
		LD_1x32_U8(acc1, sp1, sp2, sp3, sp4);
		d0 = vis_fmul8x16(vis_read_hi(acc1), xfd0);
		d0 = FMUL_16x16(d0, yfd1);
		ybuff[(i >> 2)] = vis_fpadd16(bb, d0);
		x += delta;
	}

	if ((size & 3) != 0) {
		CALC_X_3();
		xfd0 = xb[i];
		bb = ybuff[(i >> 2)];

		if ((size & 3) == 1) {
			x4 = x3 = x2 = x1;
		} else if ((size & 3) == 2) {
			x4 = x3 = x2;
		} else {
			x4 = x3;
		}

		CALC_SRCX_4(sp0);
		LD_1x32_U8(acc1, sp1, sp2, sp3, sp4);
		d0 = vis_fmul8x16(vis_read_hi(acc1), xfd0);
		d0 = FMUL_16x16(d0, yfd0);
		bb = vis_fpadd16(bb, d0);

		CALC_SRCX_4(spp0);
		LD_1x32_U8(acc1, sp1, sp2, sp3, sp4);
		d0 = vis_fmul8x16(vis_read_hi(acc1), xfd0);
		d0 = FMUL_16x16(d0, yfd1);
		ybuff[(i >> 2)] = vis_fpadd16(bb, d0);
	}
}

/* *********************************************************** */

void
mlib_calc_1y_4x_1ch(
    VPARAMXY1)
{
	DECL_LINE;
	DECL_LINE_LOW;
	FP_TYPE d1, d2, d3;
	FP_TYPE xfd1, xfd2, xfd3;
	FP_TYPE acc2;

	yfd0 = fptr[0];

#pragma pipeloop(0)
	for (i = 0; i <= size - 4; i += 4) {
		CALC_SRC_PTRX_4(sp0);
		LD_4x32_U8(acc1, acc2, sp1, sp2, sp3, sp4);
		xfd0 = xb[i];
		xfd1 = xb[i + 1];
		xfd2 = xb[i + 2];
		xfd3 = xb[i + 3];
		bb = ybuff[(i >> 2)];
		MULX_4();
		d0 = FMUL_16x16(d0, yfd0);
		ybuff[(i >> 2)] = vis_fpadd16(bb, d0);
		x += delta;
	}

	if ((size & 3) != 0) {
		CALC_SRC_PTRX_3(sp0);
		xfd0 = xb[i];
		xfd1 = xb[i + 1];
		xfd2 = xb[i + 2];
		xfd3 = xb[i + 3];
		bb = ybuff[(i >> 2)];

		if ((size & 3) == 1) {
			sp4 = sp3 = sp2 = sp1;
		} else if ((size & 3) == 2) {
			sp4 = sp3 = sp2;
		} else {
			sp4 = sp3;
		}

		LD_4x32_U8(acc1, acc2, sp1, sp2, sp3, sp4);
		MULX_4();
		d0 = FMUL_16x16(d0, yfd0);
		ybuff[(i >> 2)] = vis_fpadd16(bb, d0);
	}
}

/* *********************************************************** */

void
mlib_calc_1y_3x_1ch(
    VPARAMXY1)
{
	DECL_LINE;
	DECL_LINE_LOW;
	FP_TYPE d1, d2;
	FP_TYPE xfd1, xfd2;
	FP_TYPE acc2;

	yfd0 = fptr[0];

#pragma pipeloop(0)
	for (i = 0; i <= size - 4; i += 4) {
		CALC_SRC_PTRX_4(sp0);
		LD_3x32_U8(acc1, acc2, sp1, sp2, sp3, sp4);
		xfd0 = xb[i];
		xfd1 = xb[i + 1];
		xfd2 = xb[i + 2];
		bb = ybuff[(i >> 2)];
		MULX_3();
		d0 = FMUL_16x16(d0, yfd0);
		ybuff[(i >> 2)] = vis_fpadd16(bb, d0);
		x += delta;
	}

	if ((size & 3) != 0) {
		CALC_SRC_PTRX_3(sp0);
		xfd0 = xb[i];
		xfd1 = xb[i + 1];
		xfd2 = xb[i + 2];
		bb = ybuff[(i >> 2)];

		if ((size & 3) == 1) {
			sp4 = sp3 = sp2 = sp1;
		} else if ((size & 3) == 2) {
			sp4 = sp3 = sp2;
		} else {
			sp4 = sp3;
		}

		LD_3x32_U8(acc1, acc2, sp1, sp2, sp3, sp4);
		MULX_3();
		d0 = FMUL_16x16(d0, yfd0);
		ybuff[(i >> 2)] = vis_fpadd16(bb, d0);
	}
}

/* *********************************************************** */

void
mlib_calc_1y_2x_1ch(
    VPARAMXY1)
{
	DECL_LINE;
	DECL_LINE_LOW;
	FP_TYPE d1;
	FP_TYPE xfd1;

	yfd0 = fptr[0];

#pragma pipeloop(0)
	for (i = 0; i <= size - 4; i += 4) {
		CALC_SRC_PTRX_4(sp0);
		LD_2x32_U8(acc1, sp1, sp2, sp3, sp4);
		xfd0 = xb[i];
		xfd1 = xb[i + 1];
		bb = ybuff[(i >> 2)];
		MULX_2();
		d0 = FMUL_16x16(d0, yfd0);
		ybuff[(i >> 2)] = vis_fpadd16(bb, d0);
		x += delta;
	}

	if ((size & 3) != 0) {
		CALC_SRC_PTRX_3(sp0);
		xfd0 = xb[i];
		xfd1 = xb[i + 1];
		bb = ybuff[(i >> 2)];

		if ((size & 3) == 1) {
			sp4 = sp3 = sp2 = sp1;
		} else if ((size & 3) == 2) {
			sp4 = sp3 = sp2;
		} else {
			sp4 = sp3;
		}

		LD_2x32_U8(acc1, sp1, sp2, sp3, sp4);
		MULX_2();
		d0 = FMUL_16x16(d0, yfd0);
		ybuff[(i >> 2)] = vis_fpadd16(bb, d0);
	}
}

/* *********************************************************** */

void
mlib_calc_1y_1x_1ch(
    VPARAMXY1)
{
	DECL_LINE;
	DECL_LINE_LOW;

	yfd0 = fptr[0];

#pragma pipeloop(0)
	for (i = 0; i <= size - 4; i += 4) {
		CALC_SRC_PTRX_4(sp0);
		LD_1x32_U8(acc1, sp1, sp2, sp3, sp4);
		xfd0 = xb[i];
		bb = ybuff[(i >> 2)];
		d0 = vis_fmul8x16(vis_read_hi(acc1), xfd0);
		d0 = FMUL_16x16(d0, yfd0);
		ybuff[(i >> 2)] = vis_fpadd16(bb, d0);
		x += delta;
	}

	if ((size & 3) != 0) {
		CALC_SRC_PTRX_3(sp0);
		xfd0 = xb[i];
		bb = ybuff[(i >> 2)];

		if ((size & 3) == 1) {
			sp4 = sp3 = sp2 = sp1;
		} else if ((size & 3) == 2) {
			sp4 = sp3 = sp2;
		} else {
			sp4 = sp3;
		}

		LD_1x32_U8(acc1, sp1, sp2, sp3, sp4);
		d0 = vis_fmul8x16(vis_read_hi(acc1), xfd0);
		d0 = FMUL_16x16(d0, yfd0);
		ybuff[(i >> 2)] = vis_fpadd16(bb, d0);
	}
}

/* *********************************************************** */

void
mlib_calc_coef_4x_high_1ch(
    VPARAMXY)
{
	DECL_COEF;
	FP_TYPE acc1, acc2, acc3, acc4;

#pragma pipeloop(0)
	for (i = 0; i < size; i += 4) {
		MLIB_FILTER_X4(x, dx, filterX, xoff);
		LD_32_H_S16(acc1, xfp1, xfp2, 0);
		LD_32_H_S16(acc2, xfp1, xfp2, 2);
		LD_32_H_S16(acc3, xfp3, xfp4, 0);
		LD_32_H_S16(acc4, xfp3, xfp4, 2);
		xb[i] = acc1;
		xb[i + 1] = acc2;
		xb[i + 3] = acc4;
		xb[i + 2] = acc3;
		x += delta;
	}
}

/* *********************************************************** */

void
mlib_calc_coef_3x_high_1ch(
    VPARAMXY)
{
	DECL_COEF;
	FP_TYPE acc1, acc2, acc3, acc4, null = 0;

#pragma pipeloop(0)
	for (i = 0; i < size; i += 4) {
		MLIB_FILTER_X4(x, dx, filterX, xoff);
		LD_32_H_S16(acc1, xfp1, xfp2, 0);
		acc2 = null;
		acc2 = vis_faligndata(vis_ld_u16(xfp2 + 2), acc2);
		acc2 = vis_faligndata(vis_ld_u16(xfp1 + 2), acc2);
		LD_32_H_S16(acc3, xfp3, xfp4, 0);
		acc4 = null;
		acc4 = vis_faligndata(vis_ld_u16(xfp4 + 2), acc4);
		acc4 = vis_faligndata(vis_ld_u16(xfp3 + 2), acc4);
		xb[i] = acc1;
		xb[i + 1] = acc2;
		xb[i + 2] = acc3;
		xb[i + 3] = acc4;
		x += delta;
	}
}

/* *********************************************************** */

void
mlib_calc_coef_2x_high_1ch(
    VPARAMXY)
{
	DECL_COEF;
	FP_TYPE acc1, acc2;

#pragma pipeloop(0)
	for (i = 0; i < size; i += 4) {
		MLIB_FILTER_X4(x, dx, filterX, xoff);
		LD_32_H_S16(acc1, xfp1, xfp2, 0);
		LD_32_H_S16(acc2, xfp3, xfp4, 0);
		xb[i] = acc1;
		xb[i + 2] = acc2;
		x += delta;
	}
}

/* *********************************************************** */

void
mlib_calc_coef_4x_1ch(
    VPARAMXY)
{
	DECL_COEF;
	FP_TYPE acc1, acc2, acc3, acc4;

#pragma pipeloop(0)
	for (i = 0; i < size; i += 4) {
		MLIB_FILTER_X4(x, dx, filterX, xoff);
		LD_32_S16(acc1, xfp1, xfp2, xfp3, xfp4, 0);
		LD_32_S16(acc2, xfp1, xfp2, xfp3, xfp4, 1);
		LD_32_S16(acc3, xfp1, xfp2, xfp3, xfp4, 2);
		LD_32_S16(acc4, xfp1, xfp2, xfp3, xfp4, 3);
		xb[i] = acc1;
		xb[i + 1] = acc2;
		xb[i + 3] = acc4;
		xb[i + 2] = acc3;
		x += delta;
	}
}

/* *********************************************************** */

void
mlib_calc_coef_3x_1ch(
    VPARAMXY)
{
	DECL_COEF;
	FP_TYPE acc1, acc2, acc3;

#pragma pipeloop(0)
	for (i = 0; i < size; i += 4) {
		MLIB_FILTER_X4(x, dx, filterX, xoff);
		LD_32_S16(acc1, xfp1, xfp2, xfp3, xfp4, 0);
		LD_32_S16(acc2, xfp1, xfp2, xfp3, xfp4, 1);
		LD_32_S16(acc3, xfp1, xfp2, xfp3, xfp4, 2);
		xb[i] = acc1;
		xb[i + 1] = acc2;
		xb[i + 2] = acc3;
		x += delta;
	}
}

/* *********************************************************** */

void
mlib_calc_coef_2x_1ch(
    VPARAMXY)
{
	DECL_COEF;
	FP_TYPE acc1, acc2;

#pragma pipeloop(0)
	for (i = 0; i < size; i += 4) {
		MLIB_FILTER_X4(x, dx, filterX, xoff);
		LD_32_S16(acc1, xfp1, xfp2, xfp3, xfp4, 0);
		LD_32_S16(acc2, xfp1, xfp2, xfp3, xfp4, 1);
		xb[i] = acc1;
		xb[i + 1] = acc2;
		x += delta;
	}
}

/* *********************************************************** */

void
mlib_calc_coef_1x_1ch(
    VPARAMXY)
{
	DECL_COEF;
	FP_TYPE acc1;

#pragma pipeloop(0)
	for (i = 0; i < size; i += 4) {
		MLIB_FILTER_X4(x, dx, filterX, xoff);
		LD_32_S16(acc1, xfp1, xfp2, xfp3, xfp4, 0);
		xb[i] = acc1;
		x += delta;
	}
}

/* *********************************************************** */
