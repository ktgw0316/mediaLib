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

#pragma ident	"@(#)mlib_v_ImageZoomTransTable_4_8nw.c	9.2	07/11/05 SMI"

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

#define	VPARAM1L                                                      \
	FILTER_TYPE *fp, mlib_s32 x, FILTER_TYPE *filterX,            \
	mlib_zoom_workspace *ws, FP_TYPE *ybuff, mlib_f32 *sp0

#define	VPARAM2L	VPARAM1L, mlib_f32 *sp1

/* *********************************************************** */

#define	DECL_LINE                                               \
	mlib_s32 i;                                             \
	mlib_s32 xSrc;                                          \
	mlib_s32 dx = ws->dx;                                   \
	mlib_s32 size = ws->size;                               \
	mlib_s32 x_shift = ws->x_shift;                         \
	mlib_s32 xf_shift = ws->xf_shift;                       \
	mlib_s32 xf_mask = ws->xf_mask;                         \
	mlib_s32 filterpos;                                     \
	mlib_s32 xoff = ws->off;                                \
	mlib_f32 *sp;                                           \
	FILTER_TYPE *fptr

/* *********************************************************** */

#define	CALC_SRC_PTRX() xSrc = (x >> x_shift); sp = sp0 + xSrc

/* *********************************************************** */

#define	CALC_SRC_PTRX2() CALC_SRC_PTRX(); spp = sp1 + xSrc

/* *********************************************************** */

#define	MLIB_FILTER(X, filter, l)                                    \
	filterpos = ((X) >> X##f_shift) & X##f_mask;                 \
	fptr = (FILTER_TYPE *) ((mlib_u8 *)filter + filterpos + l)

/* *********************************************************** */

#define	FMUL_16x16(x, y)                                          \
	vis_fpadd16(vis_fmul8sux16(x, y), vis_fmul8ulx16(x, y))

/* *********************************************************** */

void
mlib_calc_1_4_4ch(
    VPARAM1L)
{
	DECL_LINE;
	FP_TYPE xfd, bb, d0, d1, d2, d3;
	FP_TYPE yfd0;
	mlib_f32 s0, s1, s2, s3;

	yfd0 = fp[0];

#pragma pipeloop(0)
	for (i = 0; i < size; i++) {
		CALC_SRC_PTRX();
		MLIB_FILTER(x, filterX, xoff);
		xfd = *fptr;
		s0 = sp[0];
		s1 = sp[1];
		s2 = sp[2];
		s3 = sp[3];
		bb = ybuff[i];

		d0 = vis_fmul8x16au(s0, vis_read_hi(xfd));
		d1 = vis_fmul8x16al(s1, vis_read_hi(xfd));
		d2 = vis_fmul8x16au(s2, vis_read_lo(xfd));
		d3 = vis_fmul8x16al(s3, vis_read_lo(xfd));
		d0 = vis_fpadd16(d0, d1);
		d2 = vis_fpadd16(d2, d3);
		d0 = vis_fpadd16(d0, d2);
		d0 = FMUL_16x16(d0, yfd0);

		ybuff[i] = vis_fpadd16(bb, d0);

		x += dx;
	}
}

/* *********************************************************** */

void
mlib_calc_1_3_4ch(
    VPARAM1L)
{
	DECL_LINE;
	FP_TYPE xfd, bb, d0, d1, d2;
	FP_TYPE yfd0;
	mlib_f32 s0, s1, s2;

	yfd0 = fp[0];

#pragma pipeloop(0)
	for (i = 0; i < size; i++) {
		CALC_SRC_PTRX();
		MLIB_FILTER(x, filterX, xoff);
		xfd = *fptr;
		s0 = sp[0];
		s1 = sp[1];
		s2 = sp[2];
		bb = ybuff[i];

		d0 = vis_fmul8x16au(s0, vis_read_hi(xfd));
		d1 = vis_fmul8x16al(s1, vis_read_hi(xfd));
		d2 = vis_fmul8x16au(s2, vis_read_lo(xfd));
		d0 = vis_fpadd16(d0, d1);
		d0 = vis_fpadd16(d0, d2);
		d0 = FMUL_16x16(d0, yfd0);

		ybuff[i] = vis_fpadd16(bb, d0);

		x += dx;
	}
}

/* *********************************************************** */

void
mlib_calc_1_2_4ch(
    VPARAM1L)
{
	DECL_LINE;
	FP_TYPE bb, d0, d1;
	FP_TYPE yfd0;
	mlib_f32 xff, s0, s1;

	yfd0 = fp[0];

#pragma pipeloop(0)
	for (i = 0; i < size; i++) {
		CALC_SRC_PTRX();
		MLIB_FILTER(x, filterX, xoff);
		xff = *(mlib_f32 *)fptr;
		s0 = sp[0];
		s1 = sp[1];
		bb = ybuff[i];

		d0 = vis_fmul8x16au(s0, xff);
		d1 = vis_fmul8x16al(s1, xff);
		d0 = vis_fpadd16(d0, d1);
		d0 = FMUL_16x16(d0, yfd0);

		ybuff[i] = vis_fpadd16(bb, d0);

		x += dx;
	}
}

/* *********************************************************** */

void
mlib_calc_1_1_4ch(
    VPARAM1L)
{
	DECL_LINE;
	FP_TYPE bb, d0;
	FP_TYPE yfd0;
	mlib_f32 xff, s0;

	yfd0 = fp[0];

#pragma pipeloop(0)
	for (i = 0; i < size; i++) {
		CALC_SRC_PTRX();
		MLIB_FILTER(x, filterX, xoff);
		xff = *(mlib_f32 *)fptr;
		s0 = sp[0];
		bb = ybuff[i];

		d0 = vis_fmul8x16au(s0, xff);
		d0 = FMUL_16x16(d0, yfd0);

		ybuff[i] = vis_fpadd16(bb, d0);

		x += dx;
	}
}

/* *********************************************************** */

void
mlib_calc_2_4_4ch(
    VPARAM2L)
{
	DECL_LINE;
	FP_TYPE xfd, bb, d0, d1, d2, d3, d4, d5;
	FP_TYPE yfd0, yfd1;
	mlib_f32 s0, s1, s2, s3;
	mlib_f32 *spp;

	yfd0 = fp[0];
	yfd1 = fp[1];

#pragma pipeloop(0)
	for (i = 0; i < size; i++) {
		CALC_SRC_PTRX2();
		MLIB_FILTER(x, filterX, xoff);
		xfd = *fptr;
		s0 = sp[0];
		s1 = sp[1];
		s2 = sp[2];
		s3 = sp[3];
		bb = ybuff[i];

		d0 = vis_fmul8x16au(s0, vis_read_hi(xfd));
		d1 = vis_fmul8x16al(s1, vis_read_hi(xfd));
		d2 = vis_fmul8x16au(s2, vis_read_lo(xfd));
		d3 = vis_fmul8x16al(s3, vis_read_lo(xfd));
		d0 = vis_fpadd16(d0, d1);
		d2 = vis_fpadd16(d2, d3);
		d0 = vis_fpadd16(d0, d2);

		d4 = FMUL_16x16(d0, yfd0);

		s0 = spp[0];
		s1 = spp[1];
		s2 = spp[2];
		s3 = spp[3];

		d0 = vis_fmul8x16au(s0, vis_read_hi(xfd));
		d1 = vis_fmul8x16al(s1, vis_read_hi(xfd));
		d2 = vis_fmul8x16au(s2, vis_read_lo(xfd));
		d3 = vis_fmul8x16al(s3, vis_read_lo(xfd));
		d0 = vis_fpadd16(d0, d1);
		d2 = vis_fpadd16(d2, d3);
		d0 = vis_fpadd16(d0, d2);

		d5 = FMUL_16x16(d0, yfd1);

		d0 = vis_fpadd16(d4, d5);

		ybuff[i] = vis_fpadd16(bb, d0);

		x += dx;
	}
}

/* *********************************************************** */

void
mlib_calc_2_3_4ch(
    VPARAM2L)
{
	DECL_LINE;
	FP_TYPE xfd, bb, d0, d1, d2, d4, d5;
	FP_TYPE yfd0, yfd1;
	mlib_f32 s0, s1, s2;
	mlib_f32 *spp;

	yfd0 = fp[0];
	yfd1 = fp[1];

#pragma pipeloop(0)
	for (i = 0; i < size; i++) {
		CALC_SRC_PTRX2();
		MLIB_FILTER(x, filterX, xoff);
		xfd = *fptr;
		s0 = sp[0];
		s1 = sp[1];
		s2 = sp[2];
		bb = ybuff[i];

		d0 = vis_fmul8x16au(s0, vis_read_hi(xfd));
		d1 = vis_fmul8x16al(s1, vis_read_hi(xfd));
		d2 = vis_fmul8x16au(s2, vis_read_lo(xfd));
		d0 = vis_fpadd16(d0, d1);
		d0 = vis_fpadd16(d0, d2);

		d4 = FMUL_16x16(d0, yfd0);

		s0 = spp[0];
		s1 = spp[1];
		s2 = spp[2];

		d0 = vis_fmul8x16au(s0, vis_read_hi(xfd));
		d1 = vis_fmul8x16al(s1, vis_read_hi(xfd));
		d2 = vis_fmul8x16au(s2, vis_read_lo(xfd));
		d0 = vis_fpadd16(d0, d1);
		d0 = vis_fpadd16(d0, d2);

		d5 = FMUL_16x16(d0, yfd1);

		d0 = vis_fpadd16(d4, d5);

		ybuff[i] = vis_fpadd16(bb, d0);

		x += dx;
	}
}

/* *********************************************************** */

void
mlib_calc_2_2_4ch(
    VPARAM2L)
{
	DECL_LINE;
	FP_TYPE bb, d0, d1, d4, d5;
	FP_TYPE yfd0, yfd1;
	mlib_f32 xff, s0, s1;
	mlib_f32 *spp;

	yfd0 = fp[0];
	yfd1 = fp[1];

#pragma pipeloop(0)
	for (i = 0; i < size; i++) {
		CALC_SRC_PTRX2();
		MLIB_FILTER(x, filterX, xoff);
		xff = *(mlib_f32 *)fptr;
		s0 = sp[0];
		s1 = sp[1];
		bb = ybuff[i];

		d0 = vis_fmul8x16au(s0, xff);
		d1 = vis_fmul8x16al(s1, xff);
		d0 = vis_fpadd16(d0, d1);

		d4 = FMUL_16x16(d0, yfd0);

		s0 = spp[0];
		s1 = spp[1];

		d0 = vis_fmul8x16au(s0, xff);
		d1 = vis_fmul8x16al(s1, xff);
		d0 = vis_fpadd16(d0, d1);

		d5 = FMUL_16x16(d0, yfd1);

		d0 = vis_fpadd16(d4, d5);

		ybuff[i] = vis_fpadd16(bb, d0);

		x += dx;
	}
}

/* *********************************************************** */

void
mlib_calc_2_1_4ch(
    VPARAM2L)
{
	DECL_LINE;
	FP_TYPE bb, d0, d4, d5;
	FP_TYPE yfd0, yfd1;
	mlib_f32 xff, s0;
	mlib_f32 *spp;

	yfd0 = fp[0];
	yfd1 = fp[1];

#pragma pipeloop(0)
	for (i = 0; i < size; i++) {
		CALC_SRC_PTRX2();
		MLIB_FILTER(x, filterX, xoff);
		xff = *(mlib_f32 *)fptr;
		s0 = sp[0];
		bb = ybuff[i];

		d0 = vis_fmul8x16au(s0, xff);
		d4 = FMUL_16x16(d0, yfd0);

		s0 = spp[0];

		d0 = vis_fmul8x16au(s0, xff);
		d5 = FMUL_16x16(d0, yfd1);

		d0 = vis_fpadd16(d4, d5);

		ybuff[i] = vis_fpadd16(bb, d0);

		x += dx;
	}
}

/* *********************************************************** */

void
mlib_calc_h_4_4ch(
    VPARAMH)
{
	DECL_LINE;
	FP_TYPE xfd, d0, d1, d2, d3;
	mlib_f32 s0, s1, s2, s3;

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
mlib_calc_h_4_off_4ch(
    VPARAMH)
{
	DECL_LINE;
	FP_TYPE xfd, bb, d0, d1, d2, d3;
	mlib_f32 s0, s1, s2, s3;

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
mlib_calc_h_3_4ch(
    VPARAMH)
{
	DECL_LINE;
	FP_TYPE xfd, d0, d1, d2;
	mlib_f32 s0, s1, s2;

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
mlib_calc_h_3_off_4ch(
    VPARAMH)
{
	DECL_LINE;
	FP_TYPE xfd, bb, d0, d1, d2;
	mlib_f32 s0, s1, s2;

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
mlib_calc_h_2_4ch(
    VPARAMH)
{
	DECL_LINE;
	FP_TYPE d0, d1;
	mlib_f32 xff, s0, s1;

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
mlib_calc_h_2_off_4ch(
    VPARAMH)
{
	DECL_LINE;
	FP_TYPE bb, d0, d1;
	mlib_f32 xff, s0, s1;

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
mlib_calc_h_1_4ch(
    VPARAMH)
{
	DECL_LINE;
	mlib_f32 xff, s0;

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
mlib_calc_h_1_off_4ch(
    VPARAMH)
{
	DECL_LINE;
	FP_TYPE bb, d0;
	mlib_f32 xff, s0;

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
