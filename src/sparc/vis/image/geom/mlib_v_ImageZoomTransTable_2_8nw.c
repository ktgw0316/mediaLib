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

#pragma ident	"@(#)mlib_v_ImageZoomTransTable_2_8nw.c	9.2	07/11/05 SMI"

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

#define	VPARAMHH2                                              \
	mlib_s32 x, mlib_zoom_workspace *ws, FP_TYPE *pbuff,   \
	FP_TYPE *xb, mlib_u8 *sp11

/* *********************************************************** */

#define	VPARAMHH1                                                 \
	mlib_s32 x, mlib_zoom_workspace *ws, FILTER_TYPE *fptr,   \
	FP_TYPE *ybuff, FP_TYPE *xb, mlib_u8 *sp11

/* *********************************************************** */

#define	DECL_COEF                                               \
	mlib_s32 i;                                             \
	mlib_s32 xoff = ws->off;                                \
	mlib_s32 dx = ws->dx;                                   \
	mlib_s32 delta = ws->dy;                                \
	mlib_s32 div_size2 = ws->size;                          \
	mlib_s32 xf_shift = ws->xf_shift;                       \
	mlib_s32 xf_mask = ws->xf_mask;                         \
	mlib_s16 *fp0, *fp1;                                    \
	mlib_u8 *fp

/* *********************************************************** */

#define	DECL_LINE                                               \
	mlib_s32 i;                                             \
	mlib_s32 dx = ws->dx;                                   \
	mlib_s32 size = ws->size;                               \
	mlib_s32 x_shift = ws->x_shift;                         \
	mlib_s16 *sp0 = (mlib_s16 *)sp11;                       \
	mlib_s16 *sp

/* *********************************************************** */

#define	CALC_SRC_PTRX()                                         \
	sp = sp0 + (x >> x_shift);                              \
	spp = sp0 + ((x + dx) >> x_shift)

/* *********************************************************** */

#define	MLIB_FILTER_X()                                               \
	fp = (mlib_u8 *)filterX + xoff;                               \
	fp0 = (mlib_s16 *)(fp + ((x >> xf_shift) & xf_mask));         \
	fp1 = (mlib_s16 *)(fp + (((x + dx) >> xf_shift) & xf_mask))

/* *********************************************************** */

#define	FMUL_16x16(x, y)                                          \
	vis_fpadd16(vis_fmul8sux16(x, y), vis_fmul8ulx16(x, y))

/* *********************************************************** */

#define	LD_S16_1(ac0, sp, spp)                                  \
	ac0 = vis_faligndata(vis_ld_u16(spp + 1), ac0);         \
	ac0 = vis_faligndata(vis_ld_u16(sp + 1), ac0);          \
	ac0 = vis_faligndata(vis_ld_u16(spp), ac0);             \
	ac0 = vis_faligndata(vis_ld_u16(sp), ac0)

/* *********************************************************** */

#define	LD_S16_2(ac1, sp, spp)                                  \
	ac1 = vis_faligndata(vis_ld_u16(spp + 3), ac1);         \
	ac1 = vis_faligndata(vis_ld_u16(sp + 3), ac1);          \
	ac1 = vis_faligndata(vis_ld_u16(spp + 2), ac1);         \
	ac1 = vis_faligndata(vis_ld_u16(sp + 2), ac1)

/* *********************************************************** */

#define	LD_S16_3(ac1, sp, spp)                                  \
	ac1 = vis_faligndata(vis_ld_u16(spp + 2), ac1);         \
	ac1 = vis_faligndata(vis_ld_u16(sp + 2), ac1)

/* *********************************************************** */

#define	LD_C_S16(ac0, pfp0, pfp1, N)                            \
	ac0 = vis_faligndata(vis_ld_u16(pfp1 + N), ac0);        \
	ac0 = vis_faligndata(vis_ld_u16(pfp1 + N), ac0);        \
	ac0 = vis_faligndata(vis_ld_u16(pfp0 + N), ac0);        \
	ac0 = vis_faligndata(vis_ld_u16(pfp0 + N), ac0)

/* *********************************************************** */

#define	LD_C_H_S16(ac0, pfp0, N)                                \
	ac0 = vis_faligndata(vis_ld_u16(pfp0 + N + 1), ac0);    \
	ac0 = vis_faligndata(vis_ld_u16(pfp0 + N + 1), ac0);    \
	ac0 = vis_faligndata(vis_ld_u16(pfp0 + N), ac0);        \
	ac0 = vis_faligndata(vis_ld_u16(pfp0 + N), ac0)

/* *********************************************************** */

void
mlib_calc_h_4_2ch(
    VPARAMHH2)
{
	DECL_LINE;
	FP_TYPE xfd0, xfd1;
	FP_TYPE ac0, ac1, d0, d1, d2;
	mlib_f32 *fpbuff = (mlib_f32 *)pbuff;

	sp = sp0 + (x >> x_shift);
	x += dx;

	ac0 = vis_faligndata(vis_ld_u16_i(sp, 2), ac0);
	ac0 = vis_faligndata(vis_ld_u16_i(sp, 0), ac0);
	ac1 = vis_faligndata(vis_ld_u16_i(sp, 6), ac1);
	ac1 = vis_faligndata(vis_ld_u16_i(sp, 4), ac1);

#pragma pipeloop(0)
	for (i = 0; i <= size - 2; i++) {
		sp = sp0 + (x >> x_shift);
		x += dx;

		xfd0 = xb[2 * i];
		xfd1 = xb[2 * i + 1];

		d0 = vis_fmul8x16(vis_read_hi(ac0), xfd0);
		ac0 = vis_faligndata(vis_ld_u16_i(sp, 2), ac0);
		ac0 = vis_faligndata(vis_ld_u16_i(sp, 0), ac0);

		d1 = vis_fmul8x16(vis_read_hi(ac1), xfd1);
		ac1 = vis_faligndata(vis_ld_u16_i(sp, 6), ac1);
		ac1 = vis_faligndata(vis_ld_u16_i(sp, 4), ac1);

		d2 = vis_fpadd16(d0, d1);
		fpbuff[i] = vis_fpadd16s(vis_read_lo(d2), vis_read_hi(d2));
	}

	xfd0 = xb[2 * i];
	xfd1 = xb[2 * i + 1];

	d0 = vis_fmul8x16(vis_read_hi(ac0), xfd0);
	d1 = vis_fmul8x16(vis_read_hi(ac1), xfd1);
	d2 = vis_fpadd16(d0, d1);
	fpbuff[i] = vis_fpadd16s(vis_read_lo(d2), vis_read_hi(d2));
}

/* *********************************************************** */

void
mlib_calc_h_4_off_2ch(
    VPARAMHH2)
{
	DECL_LINE;
	FP_TYPE xfd0, xfd1;
	FP_TYPE ac0, ac1, d0, d1, d2;
	mlib_f32 *fpbuff = (mlib_f32 *)pbuff, bb;

	sp = sp0 + (x >> x_shift);
	x += dx;

	ac0 = vis_faligndata(vis_ld_u16_i(sp, 2), ac0);
	ac0 = vis_faligndata(vis_ld_u16_i(sp, 0), ac0);
	ac1 = vis_faligndata(vis_ld_u16_i(sp, 6), ac1);
	ac1 = vis_faligndata(vis_ld_u16_i(sp, 4), ac1);

#pragma pipeloop(0)
	for (i = 0; i <= size - 2; i++) {
		sp = sp0 + (x >> x_shift);
		x += dx;

		xfd0 = xb[2 * i];
		xfd1 = xb[2 * i + 1];
		bb = fpbuff[i];

		d0 = vis_fmul8x16(vis_read_hi(ac0), xfd0);
		ac0 = vis_faligndata(vis_ld_u16_i(sp, 2), ac0);
		ac0 = vis_faligndata(vis_ld_u16_i(sp, 0), ac0);

		d1 = vis_fmul8x16(vis_read_hi(ac1), xfd1);
		ac1 = vis_faligndata(vis_ld_u16_i(sp, 6), ac1);
		ac1 = vis_faligndata(vis_ld_u16_i(sp, 4), ac1);

		d2 = vis_fpadd16(d0, d1);
		fpbuff[i] =
		    vis_fpadd16s(bb, vis_fpadd16s(vis_read_lo(d2),
		    vis_read_hi(d2)));
	}

	xfd0 = xb[2 * i];
	xfd1 = xb[2 * i + 1];
	bb = fpbuff[i];

	d0 = vis_fmul8x16(vis_read_hi(ac0), xfd0);
	d1 = vis_fmul8x16(vis_read_hi(ac1), xfd1);
	d2 = vis_fpadd16(d0, d1);
	fpbuff[i] =
	    vis_fpadd16s(bb, vis_fpadd16s(vis_read_lo(d2), vis_read_hi(d2)));
}

/* *********************************************************** */

void
mlib_calc_h_3_2ch(
    VPARAMHH2)
{
	DECL_LINE;
	FP_TYPE xfd0, xfd1;
	FP_TYPE ac0, d0, d1, d2;
	mlib_f32 *fpbuff = (mlib_f32 *)pbuff;

	sp = sp0 + (x >> x_shift);
	x += dx;

	ac0 = vis_faligndata(vis_ld_u16_i(sp, 4), ac0);
	ac0 = vis_faligndata(vis_ld_u16_i(sp, 2), ac0);
	ac0 = vis_faligndata(vis_ld_u16_i(sp, 0), ac0);

#pragma pipeloop(0)
	for (i = 0; i <= size - 2; i++) {
		sp = sp0 + (x >> x_shift);
		x += dx;

		xfd0 = xb[2 * i];
		xfd1 = xb[2 * i + 1];

		d0 = vis_fmul8x16(vis_read_hi(ac0), xfd0);
		d1 = vis_fmul8x16(vis_read_lo(ac0), xfd1);
		ac0 = vis_faligndata(vis_ld_u16_i(sp, 4), ac0);
		ac0 = vis_faligndata(vis_ld_u16_i(sp, 2), ac0);
		ac0 = vis_faligndata(vis_ld_u16_i(sp, 0), ac0);

		d2 = vis_fpadd16(d0, d1);
		fpbuff[i] = vis_fpadd16s(vis_read_lo(d2), vis_read_hi(d2));
	}

	xfd0 = xb[2 * i];
	xfd1 = xb[2 * i + 1];

	d0 = vis_fmul8x16(vis_read_hi(ac0), xfd0);
	d1 = vis_fmul8x16(vis_read_lo(ac0), xfd1);
	d2 = vis_fpadd16(d0, d1);
	fpbuff[i] = vis_fpadd16s(vis_read_lo(d2), vis_read_hi(d2));
}

/* *********************************************************** */

void
mlib_calc_h_3_off_2ch(
    VPARAMHH2)
{
	DECL_LINE;
	FP_TYPE xfd0, xfd1;
	FP_TYPE ac0, d0, d1, d2;
	mlib_f32 *fpbuff = (mlib_f32 *)pbuff, bb;

	sp = sp0 + (x >> x_shift);
	x += dx;

	ac0 = vis_faligndata(vis_ld_u16_i(sp, 4), ac0);
	ac0 = vis_faligndata(vis_ld_u16_i(sp, 2), ac0);
	ac0 = vis_faligndata(vis_ld_u16_i(sp, 0), ac0);

#pragma pipeloop(0)
	for (i = 0; i <= size - 2; i++) {
		sp = sp0 + (x >> x_shift);
		x += dx;

		xfd0 = xb[2 * i];
		xfd1 = xb[2 * i + 1];
		bb = fpbuff[i];

		d0 = vis_fmul8x16(vis_read_hi(ac0), xfd0);
		d1 = vis_fmul8x16(vis_read_lo(ac0), xfd1);
		ac0 = vis_faligndata(vis_ld_u16_i(sp, 4), ac0);
		ac0 = vis_faligndata(vis_ld_u16_i(sp, 2), ac0);
		ac0 = vis_faligndata(vis_ld_u16_i(sp, 0), ac0);

		d2 = vis_fpadd16(d0, d1);
		fpbuff[i] =
		    vis_fpadd16s(bb, vis_fpadd16s(vis_read_lo(d2),
		    vis_read_hi(d2)));
	}

	xfd0 = xb[2 * i];
	xfd1 = xb[2 * i + 1];
	bb = fpbuff[i];

	d0 = vis_fmul8x16(vis_read_hi(ac0), xfd0);
	d1 = vis_fmul8x16(vis_read_lo(ac0), xfd1);
	d2 = vis_fpadd16(d0, d1);
	fpbuff[i] =
	    vis_fpadd16s(bb, vis_fpadd16s(vis_read_lo(d2), vis_read_hi(d2)));
}

/* *********************************************************** */

void
mlib_calc_h_2_2ch(
    VPARAMHH2)
{
	DECL_LINE;
	FP_TYPE xfd0;
	FP_TYPE ac0, d0;
	mlib_f32 *fpbuff = (mlib_f32 *)pbuff;

	sp = sp0 + (x >> x_shift);
	x += dx;

	ac0 = vis_faligndata(vis_ld_u16_i(sp, 2), ac0);
	ac0 = vis_faligndata(vis_ld_u16_i(sp, 0), ac0);

#pragma pipeloop(0)
	for (i = 0; i <= size - 2; i++) {
		sp = sp0 + (x >> x_shift);
		x += dx;

		xfd0 = xb[2 * i];

		d0 = vis_fmul8x16(vis_read_hi(ac0), xfd0);
		ac0 = vis_faligndata(vis_ld_u16_i(sp, 2), ac0);
		ac0 = vis_faligndata(vis_ld_u16_i(sp, 0), ac0);

		fpbuff[i] = vis_fpadd16s(vis_read_lo(d0), vis_read_hi(d0));
	}

	xfd0 = xb[2 * i];

	d0 = vis_fmul8x16(vis_read_hi(ac0), xfd0);
	fpbuff[i] = vis_fpadd16s(vis_read_lo(d0), vis_read_hi(d0));
}

/* *********************************************************** */

void
mlib_calc_h_2_off_2ch(
    VPARAMHH2)
{
	DECL_LINE;
	FP_TYPE xfd0;
	FP_TYPE ac0, d0;
	mlib_f32 *fpbuff = (mlib_f32 *)pbuff, bb;

	sp = sp0 + (x >> x_shift);
	x += dx;

	ac0 = vis_faligndata(vis_ld_u16_i(sp, 2), ac0);
	ac0 = vis_faligndata(vis_ld_u16_i(sp, 0), ac0);

#pragma pipeloop(0)
	for (i = 0; i <= size - 2; i++) {
		sp = sp0 + (x >> x_shift);
		x += dx;

		xfd0 = xb[2 * i];

		bb = fpbuff[i];

		d0 = vis_fmul8x16(vis_read_hi(ac0), xfd0);
		ac0 = vis_faligndata(vis_ld_u16_i(sp, 2), ac0);
		ac0 = vis_faligndata(vis_ld_u16_i(sp, 0), ac0);

		fpbuff[i] =
		    vis_fpadd16s(bb, vis_fpadd16s(vis_read_lo(d0),
		    vis_read_hi(d0)));
	}

	xfd0 = xb[2 * i];

	bb = fpbuff[i];

	d0 = vis_fmul8x16(vis_read_hi(ac0), xfd0);
	fpbuff[i] =
	    vis_fpadd16s(bb, vis_fpadd16s(vis_read_lo(d0), vis_read_hi(d0)));
}

/* *********************************************************** */

void
mlib_calc_coef_4x_high_2ch(
    VPARAMXY)
{
	DECL_COEF;
	FP_TYPE ac0;

#pragma pipeloop(0)
	for (i = 0; i < div_size2; i++) {
		MLIB_FILTER_X();
		LD_C_H_S16(ac0, fp0, 0);
		xb[4 * i] = ac0;
		LD_C_H_S16(ac0, fp0, 2);
		xb[4 * i + 1] = ac0;
		LD_C_H_S16(ac0, fp1, 0);
		xb[4 * i + 2] = ac0;
		LD_C_H_S16(ac0, fp1, 2);
		xb[4 * i + 3] = ac0;

		x += delta;
	}
}

/* *********************************************************** */

void
mlib_calc_coef_3x_high_2ch(
    VPARAMXY)
{
	DECL_COEF;
	FP_TYPE ac0, null = 0;

#pragma pipeloop(0)
	for (i = 0; i < div_size2; i++) {
		MLIB_FILTER_X();
		LD_C_H_S16(ac0, fp0, 0);
		xb[4 * i] = ac0;
		ac0 = null;
		ac0 = vis_faligndata(vis_ld_u16(fp0 + 2), ac0);
		ac0 = vis_faligndata(vis_ld_u16(fp0 + 2), ac0);
		xb[4 * i + 1] = ac0;
		LD_C_H_S16(ac0, fp1, 0);
		xb[4 * i + 2] = ac0;
		ac0 = null;
		ac0 = vis_faligndata(vis_ld_u16(fp1 + 2), ac0);
		ac0 = vis_faligndata(vis_ld_u16(fp1 + 2), ac0);
		xb[4 * i + 3] = ac0;

		x += delta;
	}
}

/* *********************************************************** */

void
mlib_calc_coef_2x_high_2ch(
    VPARAMXY)
{
	DECL_COEF;
	FP_TYPE ac0;

#pragma pipeloop(0)
	for (i = 0; i < div_size2; i++) {
		MLIB_FILTER_X();
		LD_C_H_S16(ac0, fp0, 0);
		xb[4 * i] = ac0;
		LD_C_H_S16(ac0, fp1, 0);
		xb[4 * i + 2] = ac0;
		x += delta;
	}
}

/* *********************************************************** */

void
mlib_calc_coef_4x_2ch(
    VPARAMXY)
{
	DECL_COEF;
	FP_TYPE ac0;

#pragma pipeloop(0)
	for (i = 0; i < div_size2; i++) {
		MLIB_FILTER_X();
		LD_C_S16(ac0, fp0, fp1, 0);
		xb[4 * i] = ac0;
		LD_C_S16(ac0, fp0, fp1, 1);
		xb[4 * i + 1] = ac0;
		LD_C_S16(ac0, fp0, fp1, 2);
		xb[4 * i + 2] = ac0;
		LD_C_S16(ac0, fp0, fp1, 3);
		xb[4 * i + 3] = ac0;

		x += delta;
	}
}

/* *********************************************************** */

void
mlib_calc_coef_3x_2ch(
    VPARAMXY)
{
	DECL_COEF;
	FP_TYPE ac0;

#pragma pipeloop(0)
	for (i = 0; i < div_size2; i++) {
		MLIB_FILTER_X();
		LD_C_S16(ac0, fp0, fp1, 0);
		xb[4 * i] = ac0;
		LD_C_S16(ac0, fp0, fp1, 1);
		xb[4 * i + 1] = ac0;
		LD_C_S16(ac0, fp0, fp1, 2);
		xb[4 * i + 2] = ac0;
		x += delta;
	}
}

/* *********************************************************** */

void
mlib_calc_coef_2x_2ch(
    VPARAMXY)
{
	DECL_COEF;
	FP_TYPE ac0;

#pragma pipeloop(0)
	for (i = 0; i < div_size2; i++) {
		MLIB_FILTER_X();
		LD_C_S16(ac0, fp0, fp1, 0);
		xb[4 * i] = ac0;
		LD_C_S16(ac0, fp0, fp1, 1);
		xb[4 * i + 1] = ac0;
		x += delta;
	}
}

/* *********************************************************** */

void
mlib_calc_h1_w4_2ch(
    VPARAMHH1)
{
	DECL_LINE;
	mlib_s16 *spp;
	mlib_s32 delta = ws->dy;
	FP_TYPE xfd0, xfd1, yfd0;
	FP_TYPE ac0, d0, d1, d2, bb;

	yfd0 = fptr[0];

#pragma pipeloop(0)
	for (i = 0; i < size; i += 2) {
		CALC_SRC_PTRX();
		xfd0 = xb[2 * i];
		xfd1 = xb[2 * i + 1];
		LD_S16_1(ac0, sp, spp);
		d0 = vis_fmul8x16(vis_read_hi(ac0), xfd0);
		d1 = vis_fmul8x16(vis_read_lo(ac0), xfd1);
		d2 = vis_fpadd16(d0, d1);
		xfd0 = xb[2 * i + 2];
		xfd1 = xb[2 * i + 3];
		LD_S16_2(ac0, sp, spp);
		d0 = vis_fmul8x16(vis_read_hi(ac0), xfd0);
		d1 = vis_fmul8x16(vis_read_lo(ac0), xfd1);
		d0 = vis_fpadd16(d0, d1);
		bb = ybuff[(i >> 1)];
		d0 = vis_fpadd16(d0, d2);
		d0 = FMUL_16x16(d0, yfd0);
		ybuff[(i >> 1)] = vis_fpadd16(bb, d0);
		x += delta;
	}

	if ((size & 1) != 0) {
		spp = sp = sp0 + (x >> x_shift);
		xfd0 = xb[2 * i];
		xfd1 = xb[2 * i + 1];
		LD_S16_1(ac0, sp, spp);
		d0 = vis_fmul8x16(vis_read_hi(ac0), xfd0);
		d1 = vis_fmul8x16(vis_read_lo(ac0), xfd1);
		d2 = vis_fpadd16(d0, d1);
		xfd0 = xb[2 * i + 2];
		xfd1 = xb[2 * i + 3];
		LD_S16_2(ac0, sp, spp);
		d0 = vis_fmul8x16(vis_read_hi(ac0), xfd0);
		d1 = vis_fmul8x16(vis_read_lo(ac0), xfd1);
		d0 = vis_fpadd16(d0, d1);
		bb = ybuff[(i >> 1)];
		d0 = vis_fpadd16(d0, d2);
		d0 = FMUL_16x16(d0, yfd0);
		ybuff[(i >> 1)] = vis_fpadd16(bb, d0);
	}
}

/* *********************************************************** */

void
mlib_calc_h1_w3_2ch(
    VPARAMHH1)
{
	DECL_LINE;
	mlib_s16 *spp;
	mlib_s32 delta = ws->dy;
	FP_TYPE xfd0, xfd1, yfd0;
	FP_TYPE ac0, d0, d1, d2, bb;

	yfd0 = fptr[0];

#pragma pipeloop(0)
	for (i = 0; i < size; i += 2) {
		CALC_SRC_PTRX();
		xfd0 = xb[2 * i];
		xfd1 = xb[2 * i + 1];
		LD_S16_1(ac0, sp, spp);
		d0 = vis_fmul8x16(vis_read_hi(ac0), xfd0);
		d1 = vis_fmul8x16(vis_read_lo(ac0), xfd1);
		d0 = vis_fpadd16(d0, d1);
		xfd0 = xb[2 * i + 2];
		LD_S16_3(ac0, sp, spp);
		d2 = vis_fmul8x16(vis_read_hi(ac0), xfd0);
		bb = ybuff[(i >> 1)];
		d0 = vis_fpadd16(d0, d2);
		d0 = FMUL_16x16(d0, yfd0);
		ybuff[(i >> 1)] = vis_fpadd16(bb, d0);
		x += delta;
	}

	if ((size & 1) != 0) {
		spp = sp = sp0 + (x >> x_shift);
		xfd0 = xb[2 * i];
		xfd1 = xb[2 * i + 1];
		LD_S16_1(ac0, sp, spp);
		d0 = vis_fmul8x16(vis_read_hi(ac0), xfd0);
		d1 = vis_fmul8x16(vis_read_lo(ac0), xfd1);
		d0 = vis_fpadd16(d0, d1);
		xfd0 = xb[2 * i + 2];
		LD_S16_3(ac0, sp, spp);
		d2 = vis_fmul8x16(vis_read_hi(ac0), xfd0);
		bb = ybuff[(i >> 1)];
		d0 = vis_fpadd16(d0, d2);
		d0 = FMUL_16x16(d0, yfd0);
		ybuff[(i >> 1)] = vis_fpadd16(bb, d0);
	}
}

/* *********************************************************** */

void
mlib_calc_h1_w2_2ch(
    VPARAMHH1)
{
	DECL_LINE;
	mlib_s16 *spp;
	mlib_s32 delta = ws->dy;
	FP_TYPE xfd0, xfd1, yfd0;
	FP_TYPE ac0, d0, d1, bb;

	yfd0 = fptr[0];

#pragma pipeloop(0)
	for (i = 0; i < size; i += 2) {
		CALC_SRC_PTRX();
		xfd0 = xb[2 * i];
		xfd1 = xb[2 * i + 1];
		LD_S16_1(ac0, sp, spp);
		d0 = vis_fmul8x16(vis_read_hi(ac0), xfd0);
		d1 = vis_fmul8x16(vis_read_lo(ac0), xfd1);
		bb = ybuff[(i >> 1)];
		d0 = vis_fpadd16(d0, d1);
		d0 = FMUL_16x16(d0, yfd0);
		ybuff[(i >> 1)] = vis_fpadd16(bb, d0);
		x += delta;
	}

	if ((size & 1) != 0) {
		spp = sp = sp0 + (x >> x_shift);
		xfd0 = xb[2 * i];
		xfd1 = xb[2 * i + 1];
		LD_S16_1(ac0, sp, spp);
		d0 = vis_fmul8x16(vis_read_hi(ac0), xfd0);
		d1 = vis_fmul8x16(vis_read_lo(ac0), xfd1);
		bb = ybuff[(i >> 1)];
		d0 = vis_fpadd16(d0, d1);
		d0 = FMUL_16x16(d0, yfd0);
		ybuff[(i >> 1)] = vis_fpadd16(bb, d0);
	}
}

/* *********************************************************** */
