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

#pragma ident	"@(#)mlib_v_ImageZoomTransTable_3_8nw.c	9.2	07/11/05 SMI"

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

#define	VPARAM1LL                                                     \
	FILTER_TYPE *fp, mlib_s32 x, FILTER_TYPE *filterX,            \
	mlib_zoom_workspace *ws, FP_TYPE *ybuff, mlib_u8 *sup0

#define	VPARAM1LS	VPARAM1LL, FP_TYPE dsat, mlib_f32 *dl

/* *********************************************************** */

#define	DECL_LINE                                               \
	mlib_s32 i;                                             \
	mlib_s32 dx = ws->dx;                                   \
	mlib_s32 size = ws->size;                               \
	mlib_s32 x_shift = ws->x_shift;                         \
	mlib_s32 xf_shift = ws->xf_shift;                       \
	mlib_s32 xf_mask = ws->xf_mask;                         \
	mlib_s32 filterpos;                                     \
	mlib_s32 xoff = ws->off;                                \
	FILTER_TYPE *fptr

/* *********************************************************** */

#define	CALC_SRC_PTRX() xSrc = (x >> x_shift); sp = sp0 + xSrc

/* *********************************************************** */

#define	MLIB_FILTER(X, filter, l)                                    \
	filterpos = ((X) >> X##f_shift) & X##f_mask;                 \
	fptr = (FILTER_TYPE *) ((mlib_u8 *)filter + filterpos + l)

/* *********************************************************** */

#define	FMUL_16x16(x, y)                                          \
	vis_fpadd16(vis_fmul8sux16(x, y), vis_fmul8ulx16(x, y))

/* *********************************************************** */

#define	LD_4x32_U8(acc1, acc2, acc3, acc4, acc5, acc6, sp0, sp1)   \
	{                                                          \
	    mlib_d64 *dpSrc;                                       \
	    mlib_d64 data0, data1, data2, s1, s2;                  \
	                                                           \
	    vis_alignaddr(sp0, 0);                                 \
	    dpSrc = (mlib_d64 *)((mlib_addr)sp0 & (~7));           \
	    data0 = dpSrc[0];                                      \
	    data1 = dpSrc[1];                                      \
	    data2 = dpSrc[2];                                      \
	    acc1 = vis_faligndata(data0, data1);                   \
	    s1 = vis_faligndata(data1, data2);                     \
	    acc3 = vis_bshuffle(acc1, s1);                         \
	    vis_alignaddr(sp1, 0);                                 \
	    dpSrc = (mlib_d64 *)((mlib_addr)sp1 & (~7));           \
	    data0 = dpSrc[0];                                      \
	    data1 = dpSrc[1];                                      \
	    data2 = dpSrc[2];                                      \
	    acc4 = vis_faligndata(data0, data1);                   \
	    s2 = vis_faligndata(data1, data2);                     \
	    acc6 = vis_bshuffle(acc4, s2);                         \
	    vis_alignaddr(0, 3);                                   \
	    acc2 = vis_faligndata(acc1, s1);                       \
	    acc5 = vis_faligndata(acc4, s2);                       \
	}

/* *********************************************************** */

#define	LD_3x32_U8(acc1, acc2, acc3, acc4, sp0, sp1)            \
	{                                                       \
	    mlib_d64 *dpSrc;                                    \
	    mlib_d64 data0, data1, data2, s1, s2;               \
	                                                        \
	    vis_alignaddr(sp0, 0);                              \
	    dpSrc = (mlib_d64 *)((mlib_addr)sp0 & (~7));        \
	    data0 = dpSrc[0];                                   \
	    data1 = dpSrc[1];                                   \
	    data2 = dpSrc[2];                                   \
	    acc1 = vis_faligndata(data0, data1);                \
	    s1 = vis_faligndata(data1, data2);                  \
	    acc2 = vis_bshuffle(acc1, s1);                      \
	    vis_alignaddr(sp1, 0);                              \
	    dpSrc = (mlib_d64 *)((mlib_addr)sp1 & (~7));        \
	    data0 = dpSrc[0];                                   \
	    data1 = dpSrc[1];                                   \
	    data2 = dpSrc[2];                                   \
	    acc3 = vis_faligndata(data0, data1);                \
	    s2 = vis_faligndata(data1, data2);                  \
	    acc4 = vis_bshuffle(acc3, s2);                      \
	}

/* *********************************************************** */

#define	LD_2x32_U8(acc1, acc2, acc3, acc4, sp0, sp1)            \
	{                                                       \
	    mlib_d64 *dpSrc;                                    \
	    mlib_d64 data0, data1;                              \
	                                                        \
	    vis_alignaddr(sp0, 0);                              \
	    dpSrc = (mlib_d64 *)((mlib_addr)sp0 & (~7));        \
	    data0 = dpSrc[0];                                   \
	    data1 = dpSrc[1];                                   \
	    acc1 = vis_faligndata(data0, data1);                \
	    acc2 = vis_bshuffle(acc1, acc1);                    \
	    vis_alignaddr(sp1, 0);                              \
	    dpSrc = (mlib_d64 *)((mlib_addr)sp1 & (~7));        \
	    data0 = dpSrc[0];                                   \
	    data1 = dpSrc[1];                                   \
	    acc3 = vis_faligndata(data0, data1);                \
	    acc4 = vis_bshuffle(acc3, acc3);                    \
	}

/* *********************************************************** */

#define	LD_1x32_U8(acc1, sp)                                    \
	acc1 = vis_faligndata(vis_ld_u8(sp + 2), acc1);         \
	acc1 = vis_faligndata(vis_ld_u8(sp + 1), acc1);         \
	acc1 = vis_faligndata(vis_ld_u8(sp), acc1)

/* *********************************************************** */

void
mlib_calc_1_4_3ch(
    VPARAM1LL)
{
	DECL_LINE;
	FP_TYPE bb, d0, d1, d2, d3, xfd0, xfd1;
	FP_TYPE yfd0, acc1, acc2, acc3, acc4, acc5, acc6;
	mlib_u8 *sp0, *sp1;

	vis_write_bmask(0x67889ABB, 0);
	yfd0 = fp[0];

#pragma pipeloop(0)
	for (i = 0; i < size - 1; i += 2) {
		sp0 = sup0 + 3 * (x >> x_shift);
		sp1 = sup0 + 3 * ((x + dx) >> x_shift);
		MLIB_FILTER(x, filterX, xoff);
		xfd0 = *fptr;

		filterpos = ((x + dx) >> xf_shift) & xf_mask;
		fptr = (FILTER_TYPE *) ((mlib_u8 *)filterX + filterpos + xoff);
		xfd1 = *fptr;

		LD_4x32_U8(acc1, acc2, acc3, acc4, acc5, acc6, sp0, sp1);

		bb = ybuff[i];

		d0 = vis_fmul8x16au(vis_read_hi(acc1), vis_read_hi(xfd0));
		d1 = vis_fmul8x16al(vis_read_hi(acc2), vis_read_hi(xfd0));
		d2 = vis_fmul8x16au(vis_read_hi(acc3), vis_read_lo(xfd0));
		d3 = vis_fmul8x16al(vis_read_lo(acc3), vis_read_lo(xfd0));
		d0 = vis_fpadd16(d0, d1);
		d2 = vis_fpadd16(d2, d3);
		d0 = vis_fpadd16(d0, d2);
		d0 = FMUL_16x16(d0, yfd0);

		ybuff[i] = vis_fpadd16(bb, d0);

		bb = ybuff[i + 1];

		d0 = vis_fmul8x16au(vis_read_hi(acc4), vis_read_hi(xfd1));
		d1 = vis_fmul8x16al(vis_read_hi(acc5), vis_read_hi(xfd1));
		d2 = vis_fmul8x16au(vis_read_hi(acc6), vis_read_lo(xfd1));
		d3 = vis_fmul8x16al(vis_read_lo(acc6), vis_read_lo(xfd1));
		d0 = vis_fpadd16(d0, d1);
		d2 = vis_fpadd16(d2, d3);
		d0 = vis_fpadd16(d0, d2);
		d0 = FMUL_16x16(d0, yfd0);

		ybuff[i + 1] = vis_fpadd16(bb, d0);

		x += 2 * dx;
	}

	if ((size & 1) != 0) {
		mlib_d64 *dpSrc;
		mlib_d64 data0, data1, data2, s1;

		sp0 = sup0 + 3 * (x >> x_shift);
		MLIB_FILTER(x, filterX, xoff);
		xfd0 = *fptr;

		vis_alignaddr(sp0, 0);
		dpSrc = (mlib_d64 *)((mlib_addr)sp0 & (~7));
		data0 = dpSrc[0];
		data1 = dpSrc[1];
		data2 = dpSrc[2];
		acc1 = vis_faligndata(data0, data1);
		s1 = vis_faligndata(data1, data2);
		acc3 = vis_bshuffle(acc1, s1);
		vis_alignaddr(0, 3);
		acc2 = vis_faligndata(acc1, s1);

		bb = ybuff[i];

		d0 = vis_fmul8x16au(vis_read_hi(acc1), vis_read_hi(xfd0));
		d1 = vis_fmul8x16al(vis_read_hi(acc2), vis_read_hi(xfd0));
		d2 = vis_fmul8x16au(vis_read_hi(acc3), vis_read_lo(xfd0));
		d3 = vis_fmul8x16al(vis_read_lo(acc3), vis_read_lo(xfd0));
		d0 = vis_fpadd16(d0, d1);
		d2 = vis_fpadd16(d2, d3);
		d0 = vis_fpadd16(d0, d2);
		d0 = FMUL_16x16(d0, yfd0);

		ybuff[i] = vis_fpadd16(bb, d0);
	}
}

/* *********************************************************** */

void
mlib_calc_1_3_3ch(
    VPARAM1LL)
{
	DECL_LINE;
	FP_TYPE bb, d0, d1, d2, xfd0, xfd1;
	FP_TYPE yfd0, acc1, acc2, acc3, acc4;
	mlib_u8 *sp0, *sp1;

	vis_write_bmask(0x34556788, 0);
	yfd0 = fp[0];

#pragma pipeloop(0)
	for (i = 0; i < size - 1; i += 2) {
		sp0 = sup0 + 3 * (x >> x_shift);
		sp1 = sup0 + 3 * ((x + dx) >> x_shift);
		MLIB_FILTER(x, filterX, xoff);
		xfd0 = *fptr;

		filterpos = ((x + dx) >> xf_shift) & xf_mask;
		fptr = (FILTER_TYPE *) ((mlib_u8 *)filterX + filterpos + xoff);
		xfd1 = *fptr;

		LD_3x32_U8(acc1, acc2, acc3, acc4, sp0, sp1);

		bb = ybuff[i];

		d0 = vis_fmul8x16au(vis_read_hi(acc1), vis_read_hi(xfd0));
		d1 = vis_fmul8x16al(vis_read_hi(acc2), vis_read_hi(xfd0));
		d2 = vis_fmul8x16au(vis_read_lo(acc2), vis_read_lo(xfd0));
		d0 = vis_fpadd16(d0, d1);
		d0 = vis_fpadd16(d0, d2);
		d0 = FMUL_16x16(d0, yfd0);

		ybuff[i] = vis_fpadd16(bb, d0);

		bb = ybuff[i + 1];

		d0 = vis_fmul8x16au(vis_read_hi(acc3), vis_read_hi(xfd1));
		d1 = vis_fmul8x16al(vis_read_hi(acc4), vis_read_hi(xfd1));
		d2 = vis_fmul8x16au(vis_read_lo(acc4), vis_read_lo(xfd1));
		d0 = vis_fpadd16(d0, d1);
		d0 = vis_fpadd16(d0, d2);
		d0 = FMUL_16x16(d0, yfd0);

		ybuff[i + 1] = vis_fpadd16(bb, d0);

		x += 2 * dx;
	}

	if ((size & 1) != 0) {
		mlib_d64 *dpSrc;
		mlib_d64 data0, data1, data2, s1;

		sp0 = sup0 + 3 * (x >> x_shift);
		MLIB_FILTER(x, filterX, xoff);
		xfd0 = *fptr;

		vis_alignaddr(sp0, 0);
		dpSrc = (mlib_d64 *)((mlib_addr)sp0 & (~7));
		data0 = dpSrc[0];
		data1 = dpSrc[1];
		data2 = dpSrc[2];
		acc1 = vis_faligndata(data0, data1);
		s1 = vis_faligndata(data1, data2);
		acc2 = vis_bshuffle(acc1, s1);

		bb = ybuff[i];

		d0 = vis_fmul8x16au(vis_read_hi(acc1), vis_read_hi(xfd0));
		d1 = vis_fmul8x16al(vis_read_hi(acc2), vis_read_hi(xfd0));
		d2 = vis_fmul8x16au(vis_read_lo(acc2), vis_read_lo(xfd0));
		d0 = vis_fpadd16(d0, d1);
		d0 = vis_fpadd16(d0, d2);
		d0 = FMUL_16x16(d0, yfd0);

		ybuff[i] = vis_fpadd16(bb, d0);
	}
}

/* *********************************************************** */

void
mlib_calc_1_2_3ch(
    VPARAM1LL)
{
	DECL_LINE;
	FP_TYPE bb, d0, d1;
	FP_TYPE yfd0, acc1, acc2, acc3, acc4;
	mlib_f32 xff0, xff1;
	mlib_u8 *sp0, *sp1;

	vis_write_bmask(0x34556788, 0);

	yfd0 = fp[0];

#pragma pipeloop(0)
	for (i = 0; i < size - 1; i += 2) {
		sp0 = sup0 + 3 * (x >> x_shift);
		MLIB_FILTER(x, filterX, xoff);
		xff0 = *(mlib_f32 *)fptr;

		sp1 = sup0 + 3 * ((x + dx) >> x_shift);
		filterpos = ((x + dx) >> xf_shift) & xf_mask;
		fptr = (FILTER_TYPE *) ((mlib_u8 *)filterX + filterpos + xoff);
		xff1 = *(mlib_f32 *)fptr;

		LD_2x32_U8(acc1, acc2, acc3, acc4, sp0, sp1);

		bb = ybuff[i];

		d0 = vis_fmul8x16au(vis_read_hi(acc1), xff0);
		d1 = vis_fmul8x16al(vis_read_hi(acc2), xff0);
		d0 = vis_fpadd16(d0, d1);
		d0 = FMUL_16x16(d0, yfd0);

		ybuff[i] = vis_fpadd16(bb, d0);

		bb = ybuff[i + 1];

		d0 = vis_fmul8x16au(vis_read_hi(acc3), xff1);
		d1 = vis_fmul8x16al(vis_read_hi(acc4), xff1);
		d0 = vis_fpadd16(d0, d1);
		d0 = FMUL_16x16(d0, yfd0);

		ybuff[i + 1] = vis_fpadd16(bb, d0);

		x += 2 * dx;
	}

	if ((size & 1) != 0) {
		mlib_d64 *dpSrc;
		mlib_d64 data0, data1;

		sp0 = sup0 + 3 * (x >> x_shift);
		MLIB_FILTER(x, filterX, xoff);
		xff0 = *(mlib_f32 *)fptr;

		vis_alignaddr(sp0, 0);
		dpSrc = (mlib_d64 *)((mlib_addr)sp0 & (~7));
		data0 = dpSrc[0];
		data1 = dpSrc[1];
		acc1 = vis_faligndata(data0, data1);
		acc2 = vis_bshuffle(acc1, acc1);

		bb = ybuff[i];

		d0 = vis_fmul8x16au(vis_read_hi(acc1), xff0);
		d1 = vis_fmul8x16al(vis_read_hi(acc2), xff0);
		d0 = vis_fpadd16(d0, d1);
		d0 = FMUL_16x16(d0, yfd0);

		ybuff[i] = vis_fpadd16(bb, d0);
	}
}

/* *********************************************************** */

void
mlib_calc_1_1_3ch(
    VPARAM1LL)
{
	DECL_LINE;
	FP_TYPE bb, d0;
	FP_TYPE yfd0, acc1;
	mlib_f32 xff;
	mlib_u8 *supp;

	vis_alignaddr((void *)0, 7);

	yfd0 = fp[0];

#pragma pipeloop(0)
	for (i = 0; i < size; i++) {
		supp = sup0 + 3 * (x >> x_shift);
		LD_1x32_U8(acc1, supp);

		MLIB_FILTER(x, filterX, xoff);
		xff = *(mlib_f32 *)fptr;
		bb = ybuff[i];

		d0 = vis_fmul8x16au(vis_read_hi(acc1), xff);
		d0 = FMUL_16x16(d0, yfd0);

		ybuff[i] = vis_fpadd16(bb, d0);

		x += dx;
	}
}

/* *********************************************************** */

void
mlib_calc_1_4s_3ch(
    VPARAM1LS)
{
	DECL_LINE;
	FP_TYPE bb, d0, d1, d2, d3, xfd0, xfd1;
	FP_TYPE yfd0, acc1, acc2, acc3, acc4, acc5, acc6;
	mlib_u8 *sp0, *sp1;

	vis_write_bmask(0x67889ABB, 0);
	yfd0 = fp[0];

#pragma pipeloop(0)
	for (i = 0; i < size - 1; i += 2) {
		sp0 = sup0 + 3 * (x >> x_shift);
		sp1 = sup0 + 3 * ((x + dx) >> x_shift);
		MLIB_FILTER(x, filterX, xoff);
		xfd0 = *fptr;

		filterpos = ((x + dx) >> xf_shift) & xf_mask;
		fptr = (FILTER_TYPE *) ((mlib_u8 *)filterX + filterpos + xoff);
		xfd1 = *fptr;

		LD_4x32_U8(acc1, acc2, acc3, acc4, acc5, acc6, sp0, sp1);

		bb = ybuff[i];

		d0 = vis_fmul8x16au(vis_read_hi(acc1), vis_read_hi(xfd0));
		d1 = vis_fmul8x16al(vis_read_hi(acc2), vis_read_hi(xfd0));
		d2 = vis_fmul8x16au(vis_read_hi(acc3), vis_read_lo(xfd0));
		d3 = vis_fmul8x16al(vis_read_lo(acc3), vis_read_lo(xfd0));
		d0 = vis_fpadd16(d0, d1);
		d2 = vis_fpadd16(d2, d3);
		d0 = vis_fpadd16(d0, d2);
		d0 = FMUL_16x16(d0, yfd0);

		d0 = vis_fpadd16(bb, d0);
		dl[i] = vis_fpack16(d0);
		ybuff[i] = dsat;

		bb = ybuff[i + 1];

		d0 = vis_fmul8x16au(vis_read_hi(acc4), vis_read_hi(xfd1));
		d1 = vis_fmul8x16al(vis_read_hi(acc5), vis_read_hi(xfd1));
		d2 = vis_fmul8x16au(vis_read_hi(acc6), vis_read_lo(xfd1));
		d3 = vis_fmul8x16al(vis_read_lo(acc6), vis_read_lo(xfd1));
		d0 = vis_fpadd16(d0, d1);
		d2 = vis_fpadd16(d2, d3);
		d0 = vis_fpadd16(d0, d2);
		d0 = FMUL_16x16(d0, yfd0);

		d0 = vis_fpadd16(bb, d0);
		dl[i + 1] = vis_fpack16(d0);
		ybuff[i + 1] = dsat;

		x += 2 * dx;
	}

	if ((size & 1) != 0) {
		mlib_d64 *dpSrc;
		mlib_d64 data0, data1, data2, s1;

		sp0 = sup0 + 3 * (x >> x_shift);
		MLIB_FILTER(x, filterX, xoff);
		xfd0 = *fptr;

		vis_alignaddr(sp0, 0);
		dpSrc = (mlib_d64 *)((mlib_addr)sp0 & (~7));
		data0 = dpSrc[0];
		data1 = dpSrc[1];
		data2 = dpSrc[2];
		acc1 = vis_faligndata(data0, data1);
		s1 = vis_faligndata(data1, data2);
		acc3 = vis_bshuffle(acc1, s1);
		vis_alignaddr(0, 3);
		acc2 = vis_faligndata(acc1, s1);

		bb = ybuff[i];

		d0 = vis_fmul8x16au(vis_read_hi(acc1), vis_read_hi(xfd0));
		d1 = vis_fmul8x16al(vis_read_hi(acc2), vis_read_hi(xfd0));
		d2 = vis_fmul8x16au(vis_read_hi(acc3), vis_read_lo(xfd0));
		d3 = vis_fmul8x16al(vis_read_lo(acc3), vis_read_lo(xfd0));
		d0 = vis_fpadd16(d0, d1);
		d2 = vis_fpadd16(d2, d3);
		d0 = vis_fpadd16(d0, d2);
		d0 = FMUL_16x16(d0, yfd0);

		d0 = vis_fpadd16(bb, d0);
		dl[i] = vis_fpack16(d0);
		ybuff[i] = dsat;
	}
}

/* *********************************************************** */

void
mlib_calc_1_3s_3ch(
    VPARAM1LS)
{
	DECL_LINE;
	FP_TYPE bb, d0, d1, d2, xfd0, xfd1;
	FP_TYPE yfd0, acc1, acc2, acc3, acc4;
	mlib_u8 *sp0, *sp1;

	vis_write_bmask(0x34556788, 0);
	yfd0 = fp[0];

#pragma pipeloop(0)
	for (i = 0; i < size - 1; i += 2) {
		sp0 = sup0 + 3 * (x >> x_shift);
		sp1 = sup0 + 3 * ((x + dx) >> x_shift);
		MLIB_FILTER(x, filterX, xoff);
		xfd0 = *fptr;

		filterpos = ((x + dx) >> xf_shift) & xf_mask;
		fptr = (FILTER_TYPE *) ((mlib_u8 *)filterX + filterpos + xoff);
		xfd1 = *fptr;

		LD_3x32_U8(acc1, acc2, acc3, acc4, sp0, sp1);

		bb = ybuff[i];

		d0 = vis_fmul8x16au(vis_read_hi(acc1), vis_read_hi(xfd0));
		d1 = vis_fmul8x16al(vis_read_hi(acc2), vis_read_hi(xfd0));
		d2 = vis_fmul8x16au(vis_read_lo(acc2), vis_read_lo(xfd0));
		d0 = vis_fpadd16(d0, d1);
		d0 = vis_fpadd16(d0, d2);
		d0 = FMUL_16x16(d0, yfd0);

		d0 = vis_fpadd16(bb, d0);
		dl[i] = vis_fpack16(d0);
		ybuff[i] = dsat;

		bb = ybuff[i + 1];

		d0 = vis_fmul8x16au(vis_read_hi(acc3), vis_read_hi(xfd1));
		d1 = vis_fmul8x16al(vis_read_hi(acc4), vis_read_hi(xfd1));
		d2 = vis_fmul8x16au(vis_read_lo(acc4), vis_read_lo(xfd1));
		d0 = vis_fpadd16(d0, d1);
		d0 = vis_fpadd16(d0, d2);
		d0 = FMUL_16x16(d0, yfd0);

		d0 = vis_fpadd16(bb, d0);
		dl[i + 1] = vis_fpack16(d0);
		ybuff[i + 1] = dsat;

		x += 2 * dx;
	}

	if ((size & 1) != 0) {
		mlib_d64 *dpSrc;
		mlib_d64 data0, data1, data2, s1;

		sp0 = sup0 + 3 * (x >> x_shift);
		MLIB_FILTER(x, filterX, xoff);
		xfd0 = *fptr;

		vis_alignaddr(sp0, 0);
		dpSrc = (mlib_d64 *)((mlib_addr)sp0 & (~7));
		data0 = dpSrc[0];
		data1 = dpSrc[1];
		data2 = dpSrc[2];
		acc1 = vis_faligndata(data0, data1);
		s1 = vis_faligndata(data1, data2);
		acc2 = vis_bshuffle(acc1, s1);

		bb = ybuff[i];

		d0 = vis_fmul8x16au(vis_read_hi(acc1), vis_read_hi(xfd0));
		d1 = vis_fmul8x16al(vis_read_hi(acc2), vis_read_hi(xfd0));
		d2 = vis_fmul8x16au(vis_read_lo(acc2), vis_read_lo(xfd0));
		d0 = vis_fpadd16(d0, d1);
		d0 = vis_fpadd16(d0, d2);
		d0 = FMUL_16x16(d0, yfd0);

		d0 = vis_fpadd16(bb, d0);
		dl[i] = vis_fpack16(d0);
		ybuff[i] = dsat;
	}
}

/* *********************************************************** */

void
mlib_calc_1_2s_3ch(
    VPARAM1LS)
{
	DECL_LINE;
	FP_TYPE bb, d0, d1;
	FP_TYPE yfd0, acc1, acc2, acc3, acc4;
	mlib_f32 xff0, xff1;
	mlib_u8 *sp0, *sp1;

	vis_write_bmask(0x34556788, 0);

	yfd0 = fp[0];

#pragma pipeloop(0)
	for (i = 0; i < size - 1; i += 2) {
		sp0 = sup0 + 3 * (x >> x_shift);
		MLIB_FILTER(x, filterX, xoff);
		xff0 = *(mlib_f32 *)fptr;

		sp1 = sup0 + 3 * ((x + dx) >> x_shift);
		filterpos = ((x + dx) >> xf_shift) & xf_mask;
		fptr = (FILTER_TYPE *) ((mlib_u8 *)filterX + filterpos + xoff);
		xff1 = *(mlib_f32 *)fptr;

		LD_2x32_U8(acc1, acc2, acc3, acc4, sp0, sp1);

		bb = ybuff[i];

		d0 = vis_fmul8x16au(vis_read_hi(acc1), xff0);
		d1 = vis_fmul8x16al(vis_read_hi(acc2), xff0);
		d0 = vis_fpadd16(d0, d1);
		d0 = FMUL_16x16(d0, yfd0);

		d0 = vis_fpadd16(bb, d0);
		dl[i] = vis_fpack16(d0);
		ybuff[i] = dsat;

		bb = ybuff[i + 1];

		d0 = vis_fmul8x16au(vis_read_hi(acc3), xff1);
		d1 = vis_fmul8x16al(vis_read_hi(acc4), xff1);
		d0 = vis_fpadd16(d0, d1);
		d0 = FMUL_16x16(d0, yfd0);

		d0 = vis_fpadd16(bb, d0);
		dl[i + 1] = vis_fpack16(d0);
		ybuff[i + 1] = dsat;

		x += 2 * dx;
	}

	if ((size & 1) != 0) {
		mlib_d64 *dpSrc;
		mlib_d64 data0, data1;

		sp0 = sup0 + 3 * (x >> x_shift);
		MLIB_FILTER(x, filterX, xoff);
		xff0 = *(mlib_f32 *)fptr;

		vis_alignaddr(sp0, 0);
		dpSrc = (mlib_d64 *)((mlib_addr)sp0 & (~7));
		data0 = dpSrc[0];
		data1 = dpSrc[1];
		acc1 = vis_faligndata(data0, data1);
		acc2 = vis_bshuffle(acc1, acc1);

		bb = ybuff[i];

		d0 = vis_fmul8x16au(vis_read_hi(acc1), xff0);
		d1 = vis_fmul8x16al(vis_read_hi(acc2), xff0);
		d0 = vis_fpadd16(d0, d1);
		d0 = FMUL_16x16(d0, yfd0);

		d0 = vis_fpadd16(bb, d0);
		dl[i] = vis_fpack16(d0);
		ybuff[i] = dsat;
	}
}

/* *********************************************************** */

void
mlib_calc_1_1s_3ch(
    VPARAM1LS)
{
	DECL_LINE;
	FP_TYPE bb, d0;
	FP_TYPE yfd0, acc1;
	mlib_f32 xff;
	mlib_u8 *supp;

	vis_alignaddr((void *)0, 7);

	yfd0 = fp[0];

#pragma pipeloop(0)
	for (i = 0; i < size; i++) {
		supp = sup0 + 3 * (x >> x_shift);
		LD_1x32_U8(acc1, supp);

		MLIB_FILTER(x, filterX, xoff);
		xff = *(mlib_f32 *)fptr;
		bb = ybuff[i];

		d0 = vis_fmul8x16au(vis_read_hi(acc1), xff);
		d0 = FMUL_16x16(d0, yfd0);

		d0 = vis_fpadd16(bb, d0);
		dl[i] = vis_fpack16(d0);
		ybuff[i] = dsat;

		x += dx;
	}
}

/* *********************************************************** */

void
mlib_calc_h_4_3ch(
    VPARAMH)
{
	DECL_LINE;
	FP_TYPE xfd, d0, d1, d2, d3;
	mlib_f32 s0, s1, s2, s3;
	mlib_s32 xSrc;
	mlib_f32 *sp;

#pragma pipeloop(0)
	for (i = 0; i < size; i++) {
		CALC_SRC_PTRX();
		MLIB_FILTER(x, filterX, xoff);
		xfd = *fptr;
		s0 = sp[0];
		s1 = sp[1];
		s2 = sp[2];
		s3 = sp[3];

		d0 = vis_fmul8x16au(s0, vis_read_hi(xfd));
		d1 = vis_fmul8x16al(s1, vis_read_hi(xfd));
		d2 = vis_fmul8x16au(s2, vis_read_lo(xfd));
		d3 = vis_fmul8x16al(s3, vis_read_lo(xfd));

		d0 = vis_fpadd16(d0, d1);
		d2 = vis_fpadd16(d2, d3);

		pbuff[i] = vis_fpadd16(d0, d2);

		x += dx;
	}
}

/* *********************************************************** */

void
mlib_calc_h_4_off_3ch(
    VPARAMH)
{
	DECL_LINE;
	FP_TYPE xfd, bb, d0, d1, d2, d3;
	mlib_f32 s0, s1, s2, s3;
	mlib_s32 xSrc;
	mlib_f32 *sp;

#pragma pipeloop(0)
	for (i = 0; i < size; i++) {
		CALC_SRC_PTRX();
		MLIB_FILTER(x, filterX, xoff);
		xfd = *fptr;
		s0 = sp[0];
		s1 = sp[1];
		s2 = sp[2];
		s3 = sp[3];
		bb = pbuff[i];

		d0 = vis_fmul8x16au(s0, vis_read_hi(xfd));
		d1 = vis_fmul8x16al(s1, vis_read_hi(xfd));
		d2 = vis_fmul8x16au(s2, vis_read_lo(xfd));
		d3 = vis_fmul8x16al(s3, vis_read_lo(xfd));

		d0 = vis_fpadd16(d0, d1);
		d2 = vis_fpadd16(d2, d3);
		d0 = vis_fpadd16(d0, d2);

		pbuff[i] = vis_fpadd16(bb, d0);

		x += dx;
	}
}

/* *********************************************************** */

void
mlib_calc_h_3_3ch(
    VPARAMH)
{
	DECL_LINE;
	FP_TYPE xfd, d0, d1, d2;
	mlib_f32 s0, s1, s2;
	mlib_s32 xSrc;
	mlib_f32 *sp;

#pragma pipeloop(0)
	for (i = 0; i < size; i++) {
		CALC_SRC_PTRX();
		MLIB_FILTER(x, filterX, xoff);
		xfd = *fptr;
		s0 = sp[0];
		s1 = sp[1];
		s2 = sp[2];

		d0 = vis_fmul8x16au(s0, vis_read_hi(xfd));
		d1 = vis_fmul8x16al(s1, vis_read_hi(xfd));
		d2 = vis_fmul8x16au(s2, vis_read_lo(xfd));

		d0 = vis_fpadd16(d0, d1);
		pbuff[i] = vis_fpadd16(d0, d2);

		x += dx;
	}
}

/* *********************************************************** */

void
mlib_calc_h_3_off_3ch(
    VPARAMH)
{
	DECL_LINE;
	FP_TYPE xfd, bb, d0, d1, d2;
	mlib_f32 s0, s1, s2;
	mlib_s32 xSrc;
	mlib_f32 *sp;

#pragma pipeloop(0)
	for (i = 0; i < size; i++) {
		CALC_SRC_PTRX();
		MLIB_FILTER(x, filterX, xoff);
		xfd = *fptr;
		s0 = sp[0];
		s1 = sp[1];
		s2 = sp[2];
		bb = pbuff[i];

		d0 = vis_fmul8x16au(s0, vis_read_hi(xfd));
		d1 = vis_fmul8x16al(s1, vis_read_hi(xfd));
		d2 = vis_fmul8x16au(s2, vis_read_lo(xfd));

		d0 = vis_fpadd16(d0, d1);
		d0 = vis_fpadd16(d0, d2);

		pbuff[i] = vis_fpadd16(bb, d0);

		x += dx;
	}
}

/* *********************************************************** */

void
mlib_calc_h_2_3ch(
    VPARAMH)
{
	DECL_LINE;
	FP_TYPE d0, d1;
	mlib_f32 xff, s0, s1;
	mlib_s32 xSrc;
	mlib_f32 *sp;

#pragma pipeloop(0)
	for (i = 0; i < size; i++) {
		CALC_SRC_PTRX();
		MLIB_FILTER(x, filterX, xoff);
		xff = *(mlib_f32 *)fptr;
		s0 = sp[0];
		s1 = sp[1];

		d0 = vis_fmul8x16au(s0, xff);
		d1 = vis_fmul8x16al(s1, xff);

		pbuff[i] = vis_fpadd16(d0, d1);

		x += dx;
	}
}

/* *********************************************************** */

void
mlib_calc_h_2_off_3ch(
    VPARAMH)
{
	DECL_LINE;
	FP_TYPE bb, d0, d1;
	mlib_f32 xff, s0, s1;
	mlib_s32 xSrc;
	mlib_f32 *sp;

#pragma pipeloop(0)
	for (i = 0; i < size; i++) {
		CALC_SRC_PTRX();
		MLIB_FILTER(x, filterX, xoff);
		xff = *(mlib_f32 *)fptr;
		s0 = sp[0];
		s1 = sp[1];
		bb = pbuff[i];

		d0 = vis_fmul8x16au(s0, xff);
		d1 = vis_fmul8x16al(s1, xff);

		d0 = vis_fpadd16(d0, d1);

		pbuff[i] = vis_fpadd16(bb, d0);

		x += dx;
	}
}

/* *********************************************************** */

void
mlib_calc_h_1_3ch(
    VPARAMH)
{
	DECL_LINE;
	mlib_f32 xff, s0;
	mlib_s32 xSrc;
	mlib_f32 *sp;

#pragma pipeloop(0)
	for (i = 0; i < size; i++) {
		CALC_SRC_PTRX();
		MLIB_FILTER(x, filterX, xoff);
		xff = *(mlib_f32 *)fptr;
		s0 = sp[0];

		pbuff[i] = vis_fmul8x16au(s0, xff);

		x += dx;
	}
}

/* *********************************************************** */

void
mlib_calc_h_1_off_3ch(
    VPARAMH)
{
	DECL_LINE;
	FP_TYPE bb, d0;
	mlib_f32 xff, s0;
	mlib_s32 xSrc;
	mlib_f32 *sp;

#pragma pipeloop(0)
	for (i = 0; i < size; i++) {
		CALC_SRC_PTRX();
		MLIB_FILTER(x, filterX, xoff);
		xff = *(mlib_f32 *)fptr;
		s0 = sp[0];
		bb = pbuff[i];

		d0 = vis_fmul8x16au(s0, xff);

		pbuff[i] = vis_fpadd16(bb, d0);

		x += dx;
	}
}

/* *********************************************************** */
