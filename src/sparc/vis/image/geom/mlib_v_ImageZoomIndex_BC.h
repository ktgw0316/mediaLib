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

#ifndef _MLIB_V_IMAGEZOOMINDEX_BC_H
#define	_MLIB_V_IMAGEZOOMINDEX_BC_H

#pragma ident	"@(#)mlib_v_ImageZoomIndex_BC.h	9.2	07/11/05 SMI"

#endif /* _MLIB_V_IMAGEZOOMINDEX_BC_H */

#ifdef __cplusplus
extern "C" {
#endif

/* *********************************************************** */

#if defined(IDX_U8_U8) || defined(IDX_U8_S16)

#define	IDX_TYPE	mlib_u8

#else		   /* defined(IDX_U8_U8) || defined(IDX_U8_S16) */

#define	IDX_TYPE	mlib_s16

#endif		   /* defined(IDX_U8_U8) || defined(IDX_U8_S16) */

#define	FILTER	filter_tbl

#if defined(IDX_U8_U8) || defined(IDX_S16_U8)

#define	TYPE_A	mlib_f32
#define	CALC_BC	BICUBIC_U8_ONE_LINE
#define	RES	tdp[i] = vis_fpack16(sum);
#define	VAR

#else		   /* defined(IDX_U8_U8) || defined(IDX_S16_U8) */

#define	TYPE_A	mlib_d64
#define	CALC_BC	BICUBIC_S16_ONE_LINE
#define	RES                                                     \
	res_hi = vis_fmuld8sux16(fone, vis_read_hi(sum));       \
	res_lo = vis_fmuld8sux16(fone, vis_read_lo(sum));       \
	tdp[i] = vis_fpackfix_pair(res_hi, res_lo)
#define	VAR                                                     \
	mlib_f32 fone = vis_to_float(0x1000100);                \
	mlib_d64 res_lo, res_hi

#endif		   /* defined(IDX_U8_U8) || defined(IDX_S16_U8) */

#if defined(IDX_U8_U8)

#define	FUNC_NAME_BC(X)	mlib_v_ImageZoomIndex_U8_U8_Bicubic_##X##_Line

#elif defined(IDX_S16_U8)	/* defined(IDX_U8_U8) */

#define	FUNC_NAME_BC(X)	mlib_v_ImageZoomIndex_S16_U8_Bicubic_##X##_Line

#elif defined(IDX_S16_S16)	/* defined(IDX_U8_U8) */

#define	FUNC_NAME_BC(X)	mlib_v_ImageZoomIndex_S16_S16_Bicubic_##X##_Line

#elif defined(IDX_U8_S16)	/* defined(IDX_U8_U8) */

#define	FUNC_NAME_BC(X)	mlib_v_ImageZoomIndex_U8_S16_Bicubic_##X##_Line

#endif		   /* defined(IDX_U8_U8) */

/* *********************************************************** */

static void FUNC_NAME_BC(4)  (
    mlib_s32 width,
    mlib_s32 x,
    mlib_s32 dx,
    mlib_s32 y,
    mlib_s32 src_stride,
    IDX_TYPE * tsp,
    TYPE_A * tdp,
    TYPE_A * colortable,
    mlib_d64 *buf0,
    mlib_d64 *buf1,
    mlib_d64 *buf2,
    mlib_d64 *buf3,
    mlib_u8 *filter_tbl)
{
	mlib_s32 i, cx, pos;
	mlib_d64 *flt_pos, sum, dxf0, dxf1, dxf2, dxf3, dyf0, dyf1, dyf2, dyf3,
	    r6;
	TYPE_A a0, a1, a2, a3;
	mlib_d64 r0, r1, r2, r3, r4, r5;

	VAR;

	pos = (y >> MLIB_XY_SHIFT) & MLIB_XY_MASK;
	flt_pos = (mlib_d64 *)((mlib_u8 *)FILTER + pos);
	dyf0 = flt_pos[0];
	dyf1 = flt_pos[1];
	dyf2 = flt_pos[2];
	dyf3 = flt_pos[3];

	pos = (x >> MLIB_XY_SHIFT) & MLIB_XY_MASK;
	flt_pos = (mlib_d64 *)((mlib_u8 *)FILTER + pos);
	dxf0 = flt_pos[0];
	dxf1 = flt_pos[1];
	dxf2 = flt_pos[2];
	dxf3 = flt_pos[3];

#pragma nomemorydepend
#pragma pipeloop(0)
	for (i = 0; i < width; i++) {

		cx = x >> MLIB_SHIFT;
		x += dx;

/* 1st line */
		CALC_BC;
		buf0[i] = r6;
		sum = FPMUL16(dyf0, r6);

/* 2nd line */
		cx += src_stride;
		CALC_BC;
		buf1[i] = r6;
		sum = vis_fpadd16(FPMUL16(dyf1, r6), sum);

/* 3th line */
		cx += src_stride;
		CALC_BC;
		buf2[i] = r6;
		sum = vis_fpadd16(FPMUL16(dyf2, r6), sum);

/* 4hr line */
		cx += src_stride;
		CALC_BC;
		buf3[i] = r6;
		sum = vis_fpadd16(FPMUL16(dyf3, r6), sum);

		RES;

		pos = (x >> MLIB_XY_SHIFT) & MLIB_XY_MASK;
		flt_pos = (mlib_d64 *)((mlib_u8 *)FILTER + pos);
		dxf0 = flt_pos[0];
		dxf1 = flt_pos[1];
		dxf2 = flt_pos[2];
		dxf3 = flt_pos[3];
	}
}

/* *********************************************************** */

static void FUNC_NAME_BC(3)  (
    mlib_s32 width,
    mlib_s32 x,
    mlib_s32 dx,
    mlib_s32 y,
    mlib_s32 src_stride,
    IDX_TYPE * tsp,
    TYPE_A * tdp,
    TYPE_A * colortable,
    mlib_d64 *buf0,
    mlib_d64 *buf1,
    mlib_d64 *buf2,
    mlib_d64 *buf3,
    mlib_u8 *filter_tbl)
{
	mlib_s32 i, cx, pos;
	mlib_d64 *flt_pos, sum, dxf0, dxf1, dxf2, dxf3, dyf0, dyf1, dyf2, dyf3,
	    r6;
	TYPE_A a0, a1, a2, a3;
	mlib_d64 r0, r1, r2, r3, r4, r5;

	VAR;

	tsp += src_stride;

	pos = (y >> MLIB_XY_SHIFT) & MLIB_XY_MASK;
	flt_pos = (mlib_d64 *)((mlib_u8 *)FILTER + pos);
	dyf0 = flt_pos[0];
	dyf1 = flt_pos[1];
	dyf2 = flt_pos[2];
	dyf3 = flt_pos[3];

	pos = (x >> MLIB_XY_SHIFT) & MLIB_XY_MASK;
	flt_pos = (mlib_d64 *)((mlib_u8 *)FILTER + pos);
	dxf0 = flt_pos[0];
	dxf1 = flt_pos[1];
	dxf2 = flt_pos[2];
	dxf3 = flt_pos[3];

#pragma nomemorydepend
#pragma pipeloop(0)
	for (i = 0; i < width; i++) {

		cx = x >> MLIB_SHIFT;
		x += dx;

/* 1st line */
		r6 = buf0[i];
		sum = FPMUL16(dyf0, r6);

/* 2nd line */
		CALC_BC;
		buf1[i] = r6;
		sum = vis_fpadd16(FPMUL16(dyf1, r6), sum);

/* 3th line */
		cx += src_stride;
		CALC_BC;
		buf2[i] = r6;
		sum = vis_fpadd16(FPMUL16(dyf2, r6), sum);

/* 4hr line */
		cx += src_stride;
		CALC_BC;
		buf3[i] = r6;
		sum = vis_fpadd16(FPMUL16(dyf3, r6), sum);

		RES;

		pos = (x >> MLIB_XY_SHIFT) & MLIB_XY_MASK;
		flt_pos = (mlib_d64 *)((mlib_u8 *)FILTER + pos);
		dxf0 = flt_pos[0];
		dxf1 = flt_pos[1];
		dxf2 = flt_pos[2];
		dxf3 = flt_pos[3];
	}
}

/* *********************************************************** */

static void FUNC_NAME_BC(2)  (
    mlib_s32 width,
    mlib_s32 x,
    mlib_s32 dx,
    mlib_s32 y,
    mlib_s32 src_stride,
    IDX_TYPE * tsp,
    TYPE_A * tdp,
    TYPE_A * colortable,
    mlib_d64 *buf0,
    mlib_d64 *buf1,
    mlib_d64 *buf2,
    mlib_d64 *buf3,
    mlib_u8 *filter_tbl)
{
	mlib_s32 i, cx, pos;
	mlib_d64 *flt_pos, sum, dxf0, dxf1, dxf2, dxf3, dyf0, dyf1, dyf2, dyf3,
	    r6;
	TYPE_A a0, a1, a2, a3;
	mlib_d64 r0, r1, r2, r3, r4, r5;

	VAR;

	tsp += 2 * src_stride;

	pos = (y >> MLIB_XY_SHIFT) & MLIB_XY_MASK;
	flt_pos = (mlib_d64 *)((mlib_u8 *)FILTER + pos);
	dyf0 = flt_pos[0];
	dyf1 = flt_pos[1];
	dyf2 = flt_pos[2];
	dyf3 = flt_pos[3];

	pos = (x >> MLIB_XY_SHIFT) & MLIB_XY_MASK;
	flt_pos = (mlib_d64 *)((mlib_u8 *)FILTER + pos);
	dxf0 = flt_pos[0];
	dxf1 = flt_pos[1];
	dxf2 = flt_pos[2];
	dxf3 = flt_pos[3];

#pragma nomemorydepend
#pragma pipeloop(0)
	for (i = 0; i < width; i++) {

		cx = x >> MLIB_SHIFT;
		x += dx;

/* 1st line */
		r6 = buf0[i];
		sum = FPMUL16(dyf0, r6);

/* 2nd line */
		r6 = buf1[i];
		sum = vis_fpadd16(FPMUL16(dyf1, r6), sum);

/* 3th line */
		CALC_BC;
		buf2[i] = r6;
		sum = vis_fpadd16(FPMUL16(dyf2, r6), sum);

/* 4hr line */
		cx += src_stride;
		CALC_BC;
		buf3[i] = r6;
		sum = vis_fpadd16(FPMUL16(dyf3, r6), sum);

		RES;

		pos = (x >> MLIB_XY_SHIFT) & MLIB_XY_MASK;
		flt_pos = (mlib_d64 *)((mlib_u8 *)FILTER + pos);
		dxf0 = flt_pos[0];
		dxf1 = flt_pos[1];
		dxf2 = flt_pos[2];
		dxf3 = flt_pos[3];
	}
}

/* *********************************************************** */

static void FUNC_NAME_BC(1)  (
    mlib_s32 width,
    mlib_s32 x,
    mlib_s32 dx,
    mlib_s32 y,
    mlib_s32 src_stride,
    IDX_TYPE * tsp,
    TYPE_A * tdp,
    TYPE_A * colortable,
    mlib_d64 *buf0,
    mlib_d64 *buf1,
    mlib_d64 *buf2,
    mlib_d64 *buf3,
    mlib_u8 *filter_tbl)
{
	mlib_s32 i, cx, pos;
	mlib_d64 *flt_pos, sum, dxf0, dxf1, dxf2, dxf3, dyf0, dyf1, dyf2, dyf3,
	    r6;
	TYPE_A a0, a1, a2, a3;
	mlib_d64 r0, r1, r2, r3, r4, r5;

	VAR;

	tsp += 3 * src_stride;

	pos = (y >> MLIB_XY_SHIFT) & MLIB_XY_MASK;
	flt_pos = (mlib_d64 *)((mlib_u8 *)FILTER + pos);
	dyf0 = flt_pos[0];
	dyf1 = flt_pos[1];
	dyf2 = flt_pos[2];
	dyf3 = flt_pos[3];

	pos = (x >> MLIB_XY_SHIFT) & MLIB_XY_MASK;
	flt_pos = (mlib_d64 *)((mlib_u8 *)FILTER + pos);
	dxf0 = flt_pos[0];
	dxf1 = flt_pos[1];
	dxf2 = flt_pos[2];
	dxf3 = flt_pos[3];

#pragma nomemorydepend
#pragma pipeloop(0)
	for (i = 0; i < width; i++) {

		cx = x >> MLIB_SHIFT;
		x += dx;

/* 1st line */
		r6 = buf0[i];
		sum = FPMUL16(dyf0, r6);

/* 2nd line */
		r6 = buf1[i];
		sum = vis_fpadd16(FPMUL16(dyf1, r6), sum);

/* 3th line */
		r6 = buf2[i];
		sum = vis_fpadd16(FPMUL16(dyf2, r6), sum);

/* 4hr line */
		CALC_BC;
		buf3[i] = r6;
		sum = vis_fpadd16(FPMUL16(dyf3, r6), sum);

		RES;

		pos = (x >> MLIB_XY_SHIFT) & MLIB_XY_MASK;
		flt_pos = (mlib_d64 *)((mlib_u8 *)FILTER + pos);
		dxf0 = flt_pos[0];
		dxf1 = flt_pos[1];
		dxf2 = flt_pos[2];
		dxf3 = flt_pos[3];
	}
}

/* *********************************************************** */

static void FUNC_NAME_BC(0)  (
    mlib_s32 width,
    mlib_s32 y,
    TYPE_A * tdp,
    mlib_d64 *buf0,
    mlib_d64 *buf1,
    mlib_d64 *buf2,
    mlib_d64 *buf3,
    mlib_u8 *filter_tbl)
{
	mlib_s32 i, pos;
	mlib_d64 *flt_pos, sum, dyf0, dyf1, dyf2, dyf3, r6;

	VAR;

	pos = (y >> MLIB_XY_SHIFT) & MLIB_XY_MASK;
	flt_pos = (mlib_d64 *)((mlib_u8 *)FILTER + pos);
	dyf0 = flt_pos[0];
	dyf1 = flt_pos[1];
	dyf2 = flt_pos[2];
	dyf3 = flt_pos[3];

#pragma nomemorydepend
#pragma pipeloop(0)
	for (i = 0; i < width; i++) {

/* 1st line */
		r6 = buf0[i];
		sum = FPMUL16(dyf0, r6);

/* 2nd line */
		r6 = buf1[i];
		sum = vis_fpadd16(FPMUL16(dyf1, r6), sum);

/* 3th line */
		r6 = buf2[i];
		sum = vis_fpadd16(FPMUL16(dyf2, r6), sum);

/* 4hr line */
		r6 = buf3[i];
		sum = vis_fpadd16(FPMUL16(dyf3, r6), sum);

		RES;
	}
}

/* *********************************************************** */

#undef TYPE_A
#undef FILTER
#undef RES
#undef VAR
#undef CALC_BC
#undef IDX_TYPE
#undef FUNC_NAME_BC

/* *********************************************************** */

#ifdef __cplusplus
}
#endif
