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

#pragma ident	"@(#)mlib_ImageAffineTable_F32nw_2.c	9.2	07/10/09 SMI"

/*
 * FUNCTION
 *      Internal functions for mlib_ImageAffineTable.
 */

#include <mlib_image.h>
#include <mlib_ImageInterpTable.h>
#include <mlib_ImageAffineTable.h>

/* *********************************************************** */

typedef union
{
	mlib_d64 d64;
	struct
	{
		float f0;
		float f1;
	} f32s;
} d64_2_f32;

/* *********************************************************** */

#define	FILTER_TYPE	mlib_f32
#define	FP_TYPE	mlib_f32

#define	DTYPE	mlib_f32

/* *********************************************************** */

#define	DEF_VARS_LINE                                           \
	d64_2_f32 xd0, yd0, sd0, sd1, se0, se1;                 \
	FP_TYPE c0, c1, d0, d1;                                 \
	DTYPE *sp0, *sp1;                                       \
	mlib_d64 *fptrx, *fptry;                                \
	mlib_s32 srcStride = ws->srcStride / sizeof (DTYPE);    \
	mlib_s32 i, filterpos, xSrc, ySrc;                      \
	mlib_s32 dx = ws->dx;                                   \
	mlib_s32 dy = ws->dy;                                   \
	mlib_s32 x_shift = ws->x_shift;                         \
	mlib_s32 y_shift = ws->y_shift;                         \
	mlib_s32 xf_shift = ws->xf_shift;                       \
	mlib_s32 xf_mask = ws->xf_mask;                         \
	mlib_s32 yf_shift = ws->yf_shift;                       \
	mlib_s32 yf_mask = ws->yf_mask;                         \
	mlib_s32 size = ws->size;                               \
	mlib_s32 x = ws->x0;                                    \
	mlib_s32 y = ws->y0;                                    \
	mlib_s32 b_step = ws->b_step

/* *********************************************************** */

#define	MLIB_FILTER(X, filter)                                  \
	filterpos = ((X) >> X##f_shift) & X##f_mask;            \
	fptr##X = (mlib_d64 *)((mlib_u8 *)filter + filterpos)

/* *********************************************************** */

#define	CALC_SRC_PTR()                                          \
	xSrc = ((x >> x_shift) & x_mask) + k;                   \
	ySrc = (y >> y_shift);                                  \
	sp0 = (DTYPE *) lineAddr[ySrc] + xSrc;                  \
	sp1 = sp0 + srcStride

/* *********************************************************** */

#define	LINE_FUNC(KERN)                                                  \
	void                                                             \
	mlib_ImageAffineTableLine_F32nw_##KERN(FP_TYPE * buff,           \
		const FILTER_TYPE * filterX,                             \
		const FILTER_TYPE * filterY, const mlib_u8 **lineAddr,   \
		mlib_affine_workspace * ws)

/* *********************************************************** */

LINE_FUNC(c2_2_4)
{
	DEF_VARS_LINE;
	d64_2_f32 sd2, sd3, se2, se3, xd1;
	mlib_s32 x_mask = ws->x_mask;
	mlib_s32 k = ws->k;

	CALC_SRC_PTR();

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < size; i++) {
		MLIB_FILTER(x, filterX);
		xd0.d64 = fptrx[0];
		xd1.d64 = fptrx[1];

		MLIB_FILTER(y, filterY);
		yd0.d64 = fptry[0];

		sd0.d64 = ((mlib_d64 *)sp0)[0];
		sd1.d64 = ((mlib_d64 *)sp0)[1];
		sd2.d64 = ((mlib_d64 *)sp0)[2];
		sd3.d64 = ((mlib_d64 *)sp0)[3];
		c0 = (xd0.f32s.f0 * sd0.f32s.f0 + xd0.f32s.f1 * sd1.f32s.f0 +
			xd1.f32s.f0 * sd2.f32s.f0 + xd1.f32s.f1 * sd3.f32s.f0);
		d0 = (xd0.f32s.f0 * sd0.f32s.f1 + xd0.f32s.f1 * sd1.f32s.f1 +
			xd1.f32s.f0 * sd2.f32s.f1 + xd1.f32s.f1 * sd3.f32s.f1);

		se0.d64 = ((mlib_d64 *)sp1)[0];
		se1.d64 = ((mlib_d64 *)sp1)[1];
		se2.d64 = ((mlib_d64 *)sp1)[2];
		se3.d64 = ((mlib_d64 *)sp1)[3];
		c1 = (xd0.f32s.f0 * se0.f32s.f0 + xd0.f32s.f1 * se1.f32s.f0 +
			xd1.f32s.f0 * se2.f32s.f0 + xd1.f32s.f1 * se3.f32s.f0);
		d1 = (xd0.f32s.f0 * se0.f32s.f1 + xd0.f32s.f1 * se1.f32s.f1 +
			xd1.f32s.f0 * se2.f32s.f1 + xd1.f32s.f1 * se3.f32s.f1);

		buff[i * b_step] += yd0.f32s.f0 * c0 + yd0.f32s.f1 * c1;
		buff[i * b_step + 1] += yd0.f32s.f0 * d0 + yd0.f32s.f1 * d1;

		x += dx;
		y += dy;
		CALC_SRC_PTR();
	}
}

/* *********************************************************** */

LINE_FUNC(c4_2_4)
{
	DEF_VARS_LINE;
	d64_2_f32 sd2, sd3, se2, se3, xd1;
	mlib_s32 x_mask = ws->x_mask;
	mlib_s32 k = ws->k;

	CALC_SRC_PTR();

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < size; i++) {
		MLIB_FILTER(x, filterX);
		xd0.d64 = fptrx[0];
		xd1.d64 = fptrx[1];

		MLIB_FILTER(y, filterY);
		yd0.d64 = fptry[0];

		sd0.d64 = ((mlib_d64 *)sp0)[0];
		sd1.d64 = ((mlib_d64 *)sp0)[2];
		sd2.d64 = ((mlib_d64 *)sp0)[4];
		sd3.d64 = ((mlib_d64 *)sp0)[6];
		c0 = (xd0.f32s.f0 * sd0.f32s.f0 + xd0.f32s.f1 * sd1.f32s.f0 +
			xd1.f32s.f0 * sd2.f32s.f0 + xd1.f32s.f1 * sd3.f32s.f0);
		d0 = (xd0.f32s.f0 * sd0.f32s.f1 + xd0.f32s.f1 * sd1.f32s.f1 +
			xd1.f32s.f0 * sd2.f32s.f1 + xd1.f32s.f1 * sd3.f32s.f1);

		se0.d64 = ((mlib_d64 *)sp1)[0];
		se1.d64 = ((mlib_d64 *)sp1)[2];
		se2.d64 = ((mlib_d64 *)sp1)[4];
		se3.d64 = ((mlib_d64 *)sp1)[6];
		c1 = (xd0.f32s.f0 * se0.f32s.f0 + xd0.f32s.f1 * se1.f32s.f0 +
			xd1.f32s.f0 * se2.f32s.f0 + xd1.f32s.f1 * se3.f32s.f0);
		d1 = (xd0.f32s.f0 * se0.f32s.f1 + xd0.f32s.f1 * se1.f32s.f1 +
			xd1.f32s.f0 * se2.f32s.f1 + xd1.f32s.f1 * se3.f32s.f1);

		buff[i * b_step] += yd0.f32s.f0 * c0 + yd0.f32s.f1 * c1;
		buff[i * b_step + 1] += yd0.f32s.f0 * d0 + yd0.f32s.f1 * d1;

		x += dx;
		y += dy;
		CALC_SRC_PTR();
	}
}

/* *********************************************************** */

#define	CALC_SRC_PTR3()                                         \
	xSrc = (x >> x_shift);                                  \
	ySrc = (y >> y_shift);                                  \
	sp0 = (DTYPE *) lineAddr[ySrc] + 2 * xSrc;              \
	sp1 = sp0 + srcStride;                                  \
	sp_0 = (DTYPE *) lineAddr[ySrc] + 2 * srcWid + xSrc;    \
	sp_1 = sp_0 + srcStride

/* *********************************************************** */

LINE_FUNC(c3_2_2)
{
	DEF_VARS_LINE;
	FP_TYPE *sp_0, *sp_1, sf0, sf1;
	FP_TYPE e0, e1;
	mlib_s32 srcWid = ws->srcWidth;

	CALC_SRC_PTR3();

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < size; i++) {
		MLIB_FILTER(x, filterX);
		xd0.d64 = fptrx[0];

		MLIB_FILTER(y, filterY);
		yd0.d64 = fptry[0];

		sd0.d64 = ((mlib_d64 *)sp0)[0];
		sd1.d64 = ((mlib_d64 *)sp0)[1];
		sf0 = sp_0[0];
		sf1 = sp_0[1];
		c0 = xd0.f32s.f0 * sd0.f32s.f0 + xd0.f32s.f1 * sd1.f32s.f0;
		d0 = xd0.f32s.f0 * sd0.f32s.f1 + xd0.f32s.f1 * sd1.f32s.f1;
		e0 = xd0.f32s.f0 * sf0 + xd0.f32s.f1 * sf1;

		se0.d64 = ((mlib_d64 *)sp1)[0];
		se1.d64 = ((mlib_d64 *)sp1)[1];
		sf0 = sp_1[0];
		sf1 = sp_1[1];
		c1 = xd0.f32s.f0 * se0.f32s.f0 + xd0.f32s.f1 * se1.f32s.f0;
		d1 = xd0.f32s.f0 * se0.f32s.f1 + xd0.f32s.f1 * se1.f32s.f1;
		e1 = xd0.f32s.f0 * sf0 + xd0.f32s.f1 * sf1;

		buff[i * b_step] += yd0.f32s.f0 * c0 + yd0.f32s.f1 * c1;
		buff[i * b_step + 1] += yd0.f32s.f0 * d0 + yd0.f32s.f1 * d1;
		buff[i * b_step + 2] += yd0.f32s.f0 * e0 + yd0.f32s.f1 * e1;

		x += dx;
		y += dy;
		CALC_SRC_PTR3();
	}
}

/* *********************************************************** */
