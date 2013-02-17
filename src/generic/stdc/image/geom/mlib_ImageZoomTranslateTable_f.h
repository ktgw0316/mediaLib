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

#ifndef _MLIB_IMAGEZOOMTRANSLATETABLE_F_H
#define	_MLIB_IMAGEZOOMTRANSLATETABLE_F_H

#pragma ident	"@(#)mlib_ImageZoomTranslateTable_f.h	9.2	07/10/09 SMI"

/*
 * FUNCTION
 *      Internal functions for mlib_ImageZoomTranslateTable.
 */

#ifdef __cplusplus
extern "C" {
#endif

#include <mlib_types.h>
#include <mlib_image_types.h>
#include <mlib_status.h>
#include <mlib_ImageDivTables.h>

#if IMG_TYPE == 4 || IMG_TYPE == 5
#ifdef _MSC_VER
#pragma optimize("", off)
#endif		   /* _MSC_VER */
#endif		   /* IMG_TYPE == 4 || IMG_TYPE == 5 */

/* *********************************************************** */

    typedef union
{
	mlib_d64 d64;
	struct
	{
		mlib_f32 f0;
		mlib_f32 f1;
	} f32s;
} d64_2_f32;

/* *********************************************************** */

#if FLT_BITS == 2

#define	FILTER_TYPE	mlib_f32
#define	FP_TYPE	mlib_f32

#elif FLT_BITS == 3	/* FLT_BITS == 2 */

#define	FILTER_TYPE	mlib_d64
#define	FP_TYPE	mlib_d64

#endif /* FLT_BITS == 2 */

/* *********************************************************** */

#if IMG_TYPE == 1

#define	DTYPE	mlib_u8
#define	SAT(dst, ival, val)	SAT8(dst, ival, val)
#ifdef MLIB_USE_FTOI_CLAMPING
#define	SAT_OFF	-127.5
#else /* MLIB_USE_FTOI_CLAMPING */
#define	SAT_OFF	0.5
#endif /* MLIB_USE_FTOI_CLAMPING */
#define	SRC2FP(x)	U82FP(x)

#elif IMG_TYPE == 2	/* IMG_TYPE == 1 */

#define	DTYPE	mlib_s16
#define	SAT(dst, ival, val)	SAT16(dst, ival, val)
#ifdef MLIB_USE_FTOI_CLAMPING
#define	SAT_OFF	0.5
#else /* MLIB_USE_FTOI_CLAMPING */
#define	SAT_OFF	0.0
#endif /* MLIB_USE_FTOI_CLAMPING */
#define	SRC2FP(x)	(x)

#elif IMG_TYPE == 3	/* IMG_TYPE == 1 */

#define	DTYPE	mlib_s32
#define	SAT(dst, ival, val)	SAT32(dst, ival, val)
#define	SAT_OFF	0.0
#define	SRC2FP(x)	(x)

#elif IMG_TYPE == 4	/* IMG_TYPE == 1 */

#define	DTYPE	mlib_f32
#define	SAT(dst, ival, val)	dst = val
#define	SAT_OFF	0.0
#define	SRC2FP(x)	(x)

#elif IMG_TYPE == 5	/* IMG_TYPE == 1 */

#define	DTYPE	mlib_d64
#define	SAT(dst, ival, val)	dst = val
#define	SAT_OFF	0.0
#define	SRC2FP(x)	(x)

#elif IMG_TYPE == 0	/* IMG_TYPE == 1 */

#define	DTYPE	mlib_u16
#define	SAT(dst, ival, val)	SAT_U16(dst, ival, val)
#ifdef MLIB_USE_FTOI_CLAMPING
#define	SAT_OFF	-32767.5
#else /* MLIB_USE_FTOI_CLAMPING */
#define	SAT_OFF	0.5
#endif /* MLIB_USE_FTOI_CLAMPING */
#define	SRC2FP(x)	(x)

#endif /* IMG_TYPE == 1 */

/* *********************************************************** */

#ifdef _MSC_VER

#define	U82FP(x)	(x)

#else /* _MSC_VER */

#if FLT_BITS == 2
#define	U82FP(x)	mlib_U82F32[x]
#elif FLT_BITS == 3	/* FLT_BITS == 2 */
#define	U82FP(x)	mlib_U82D64[x]
#endif /* FLT_BITS == 2 */
#endif /* _MSC_VER */

/* *********************************************************** */

#define	DECLAREVAR                                              \
	mlib_s32 xLeft, x0, y0, ynew;                           \
	mlib_s32 ySrc;                                          \
	mlib_s32 yStart = ws->yStart;                           \
	mlib_s32 yFinish = ws->yFinish;                         \
	mlib_s32 max_xsize = ws->max_xsize;                     \
	mlib_s32 dstYStride = ws->dstStride;                    \
	mlib_s32 nchan = ws->nchan;                             \
	mlib_s32 x_shift = ws->x_shift;                         \
	mlib_s32 y_shift = ws->y_shift;                         \
	mlib_s32 filterposy, sbits = 0, x_mask;                 \
	mlib_s32 size;                                          \
	DTYPE *srcPixelPtr, *dstPixelPtr;                       \
	mlib_s32 xf_shift, xf_mask;                             \
	mlib_s32 m = table->width;                              \
	mlib_s32 n = table->height;                             \
	mlib_s32 xf_bits = table->width_bits;                   \
	mlib_s32 yf_bits = table->height_bits;                  \
	mlib_s32 subsampleBitsH = table->subsampleBitsH;        \
	mlib_s32 subsampleBitsV = table->subsampleBitsV;        \
	FILTER_TYPE *filterX, *filterY, *fpy;                   \
	mlib_s32 i, j, k, l, kw, kh, off

/* *********************************************************** */

#ifdef SRC_EXTEND

#define	PARAMS	PARAMS_EXT

#define	CALL_PARAMS                                                 \
	dstData, lineAddr, x_ind, leftEdges, rightEdges, xStarts,   \
	yStarts, ws, table, buff

#define	DECL_LINE_ADD	mlib_s32 off = ws -> off

#define	XPARAMS                                                      \
	mlib_s32 x, FILTER_TYPE *filterX, mlib_zoom_workspace *ws,   \
	FP_TYPE *pbuff, DTYPE *srcPixelPtr, mlib_s32 *x_ind,         \
	mlib_s32 *x_tab

#define	CALL_XPARAMS	x, filterX, ws, pbuff, srcPixelPtr, x_ind, x_tab

#define	X1PARAMS                                                      \
	mlib_s32 x, FILTER_TYPE * filterX, mlib_zoom_workspace *ws,   \
	FP_TYPE *ybuff, mlib_s32 *x_ind, mlib_s32 *x_tab,             \
	FILTER_TYPE *fpyy, DTYPE *srcPixelPtr

#define	CALL_X1PARAMS	x, filterX, ws, ybuff, x_ind, x_tab, fpyy, srcPixelPtr

#else /* SRC_EXTEND */

#define	PARAMS	PARAMS_NW

#define	CALL_PARAMS                                                       \
	dstData, lineAddr, leftEdges, rightEdges, xStarts, yStarts, ws,   \
	table, buff

#define	DECL_LINE_ADD                                           \
	mlib_s32 x_mask = ws->x_mask;                           \
	mlib_s32 x_mask0 = ws->x_mask0;                         \
	DTYPE *srcPixelPtr0 = srcPixelPtr

#define	XPARAMS                                                      \
	mlib_s32 x, FILTER_TYPE *filterX, mlib_zoom_workspace *ws,   \
	FP_TYPE *pbuff, DTYPE *srcPixelPtr

#define	CALL_XPARAMS	x, filterX, ws, pbuff, srcPixelPtr

#define	X1PARAMS                                                     \
	mlib_s32 x, FILTER_TYPE *filterX, mlib_zoom_workspace *ws,   \
	FP_TYPE *ybuff, FILTER_TYPE *fpyy, DTYPE *srcPixelPtr

#define	CALL_X1PARAMS	x, filterX, ws, ybuff, fpyy, srcPixelPtr

#endif /* SRC_EXTEND */

/* *********************************************************** */

#define	X2PARAMS	X1PARAMS, DTYPE *sP1
#define	X3PARAMS	X2PARAMS, DTYPE *sP2
#define	X4PARAMS	X3PARAMS, DTYPE *sP3

/* *********************************************************** */

#define	DECL_LINE                                               \
	mlib_s32 i;                                             \
	mlib_s32 xSrc;                                          \
	mlib_s32 size = ws->size;                               \
	mlib_s32 x_shift = ws->x_shift;                         \
	mlib_s32 xf_shift = ws->xf_shift;                       \
	mlib_s32 xf_mask = ws->xf_mask;                         \
	mlib_s32 filterposx;                                    \
	FILTER_TYPE *fptr;                                      \
	                                                        \
	DECL_LINE_ADD

/* *********************************************************** */

#define	CLIP                                                             \
	dstPixelPtr = (DTYPE *) ((mlib_u8 *)dstPixelPtr + dstYStride);   \
	y0 = yStarts[j];                                                 \
	ynew = yStarts[j + 1];

/* *********************************************************** */

#define	DIST_BITS()                                                        \
	xf_shift = x_shift - subsampleBitsH - xf_bits - FLT_BITS;          \
	xf_mask = (((1 << subsampleBitsH) - 1) << (xf_bits + FLT_BITS));   \
	yf_shift = y_shift - subsampleBitsV - yf_bits - FLT_BITS;          \
	yf_mask = (((1 << subsampleBitsV) - 1) << (yf_bits + FLT_BITS))

/* *********************************************************** */

#ifdef SRC_EXTEND

#define	CALC_SRC_PTRY1()	srcPixelPtr = (DTYPE*)lineAddr[ySrc + l] + k

#define	CALC_SRC_PTRY2()                                        \
	CALC_SRC_PTRY1();                                       \
	sP1 = (DTYPE *) lineAddr[ySrc + l + 1] + k

#define	CALC_SRC_PTRY3()                                        \
	CALC_SRC_PTRY2();                                       \
	sP2 = (DTYPE *) lineAddr[ySrc + l + 2] + k

#define	CALC_SRC_PTRY4()                                        \
	CALC_SRC_PTRY3();                                       \
	sP3 = (DTYPE *) lineAddr[ySrc + l + 3] + k

#define	CALC_SRC_PTRX()	xSrc = (x >> x_shift) + off
#define	CALC_SRC_PTRX1()	CALC_SRC_PTRX()
#define	CALC_SRC_PTRX2()	CALC_SRC_PTRX()
#define	CALC_SRC_PTRX3()	CALC_SRC_PTRX()

#define	GET_SRC(N)	SRC2FP(srcPixelPtr[x_ind[xSrc + N]])
#define	GET_SRC1(N)	SRC2FP(sP1[x_ind[xSrc + N]])
#define	GET_SRC2(N)	SRC2FP(sP2[x_ind[xSrc + N]])
#define	GET_SRC3(N)	SRC2FP(sP3[x_ind[xSrc + N]])

#define	UPDATE_X	x = x_tab[i+1]

#else /* SRC_EXTEND */

#define	CALC_SRC_PTRY1()                                        \
	srcPixelPtr = (DTYPE *) lineAddr[ySrc + l] + coff

#define	CALC_SRC_PTRY2()                                        \
	CALC_SRC_PTRY1();                                       \
	sP1 = (DTYPE *) lineAddr[ySrc + l + 1] + coff

#define	CALC_SRC_PTRY3()                                        \
	CALC_SRC_PTRY2();                                       \
	sP2 = (DTYPE *) lineAddr[ySrc + l + 2] + coff

#define	CALC_SRC_PTRY4()                                        \
	CALC_SRC_PTRY3();                                       \
	sP3 = (DTYPE *) lineAddr[ySrc + l + 3] + coff

#define	CALC_SRC_PTRX()                                         \
	xSrc = (x >> x_shift) & x_mask;                         \
	xSrc += ((xSrc >> 1) & x_mask0);                        \
	srcPixelPtr = srcPixelPtr0 + xSrc

#define	CALC_SRC_PTRX1()	CALC_SRC_PTRX(); sP1 = sP1_0 + xSrc

#define	CALC_SRC_PTRX2()	CALC_SRC_PTRX1(); sP2 = sP2_0 + xSrc

#define	CALC_SRC_PTRX3()	CALC_SRC_PTRX2(); sP3 = sP3_0 + xSrc

#define	GET_SRC(N)	SRC2FP(srcPixelPtr[CHAN##N])
#define	GET_SRC1(N)	SRC2FP(sP1[CHAN##N])
#define	GET_SRC2(N)	SRC2FP(sP2[CHAN##N])
#define	GET_SRC3(N)	SRC2FP(sP3[CHAN##N])

#define	UPDATE_X	x += dx

#endif /* SRC_EXTEND */

/* *********************************************************** */

#define	MLIB_FILTER_X(x, filter, l)                                   \
	filterposx = (x >> xf_shift) & xf_mask;                       \
	fptr = (FILTER_TYPE *) ((mlib_u8 *)filter + filterposx) + l

/* *********************************************************** */

#define	GET_FILTER_Y()                                            \
	filterposy = (y >> yf_shift) & yf_mask;                   \
	fpy = (FILTER_TYPE *) ((mlib_u8 *)filterY + filterposy)

/* *********************************************************** */

#ifdef MLIB_USE_FTOI_CLAMPING

#define	SAT8(DST, ival, val)                                    \
	ival = (mlib_s32)(val * (1 << 24));                     \
	DST = (ival >> 24) ^ 0x80

#define	SAT16(DST, ival, val)                                   \
	ival = (mlib_s32)(val * (1 << 16));                     \
	DST = (ival >> 16)

#define	SAT_U16(DST, ival, val)                                 \
	ival = (mlib_s32)(val * (1 << 16));                     \
	DST = (ival >> 16) ^ 0x8000

#define	SAT32(DST, ival, val)	DST = (mlib_s32)(val)

#else /* MLIB_USE_FTOI_CLAMPING */

#define	CLAMP(DST, val, max, min)                               \
	if (val >= max)                                         \
	    val = max;                                          \
	if (val <= min)                                         \
	    val = min;                                          \
	DST = (DTYPE) val

#define	SAT8(DST, ival, val)                                    \
	CLAMP(DST, val, MLIB_U8_MAX, MLIB_U8_MIN)

#define	SAT16(DST, ival, val)                                   \
	CLAMP(DST, val, MLIB_S16_MAX, MLIB_S16_MIN)

#define	SAT_U16(DST, ival, val)                                 \
	CLAMP(DST, val, MLIB_U16_MAX, MLIB_U16_MIN)

#define	SAT32(DST, ival, val)                                   \
	CLAMP(DST, val, MLIB_S32_MAX, MLIB_S32_MIN)

#endif /* MLIB_USE_FTOI_CLAMPING */

/* *********************************************************** */

#define	MAX_KER	4

/* *********************************************************** */

#define	CHAN0	0
#define	CHAN1	nchan
#define	CHAN2	chan2
#define	CHAN3	chan3

/* *********************************************************** */

static void
mlib_calc4(
    FILTER_TYPE *fp,
    FP_TYPE **buff_arr,
    mlib_s32 bnd,
    FP_TYPE *ybuff,
    mlib_zoom_workspace *ws)
{
	mlib_s32 i;
	FP_TYPE yf0, yf1, yf2, yf3;
	FP_TYPE *buff0, *buff1, *buff2, *buff3;

#if FLT_BITS == 2
	mlib_s32 size2 = ws->size2;
#else /* FLT_BITS == 2 */
	mlib_s32 size = ws->size;
#endif /* FLT_BITS == 2 */

	buff0 = buff_arr[bnd];
	buff1 = buff_arr[bnd + 1];
	buff2 = buff_arr[bnd + 2];
	buff3 = buff_arr[bnd + 3];
	yf0 = fp[0];
	yf1 = fp[1];
	yf2 = fp[2];
	yf3 = fp[3];

#if FLT_BITS == 2
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < size2; i++) {
		d64_2_f32 yd0, yd1, yd2, yd3, yy;

		yd0.d64 = ((mlib_d64 *)buff0)[i];
		yd1.d64 = ((mlib_d64 *)buff1)[i];
		yd2.d64 = ((mlib_d64 *)buff2)[i];
		yd3.d64 = ((mlib_d64 *)buff3)[i];
		yy.d64 = ((mlib_d64 *)ybuff)[i];

		yy.f32s.f0 += yd0.f32s.f0 * yf0 + yd1.f32s.f0 * yf1 +
		    yd2.f32s.f0 * yf2 + yd3.f32s.f0 * yf3;
		yy.f32s.f1 += yd0.f32s.f1 * yf0 + yd1.f32s.f1 * yf1 +
		    yd2.f32s.f1 * yf2 + yd3.f32s.f1 * yf3;

		((mlib_d64 *)ybuff)[i] = yy.d64;
	}

#else /* FLT_BITS == 2 */

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < size; i++) {
		FP_TYPE c0;

		c0 = buff0[i] * yf0 + buff1[i] * yf1 + buff2[i] * yf2 +
		    buff3[i] * yf3;
		ybuff[i] += c0;
	}

#endif /* FLT_BITS == 2 */
}

/* *********************************************************** */

static void
mlib_calc4s(
    FILTER_TYPE *fp,
    FP_TYPE **buff_arr,
    mlib_s32 bnd,
    FP_TYPE *ybuff,
    DTYPE *dPtr,
    mlib_zoom_workspace *ws)
{
	mlib_s32 i;
	FP_TYPE yf0, yf1, yf2, yf3;
	FP_TYPE *buff0, *buff1, *buff2, *buff3;
	mlib_s32 size, chan;

#if IMG_TYPE < 3
#ifdef MLIB_USE_FTOI_CLAMPING
	mlib_s32 ival;
#endif /* MLIB_USE_FTOI_CLAMPING */
#endif /* IMG_TYPE < 3 */

	chan = ws->nchan;
	size = ws->size;
	buff0 = buff_arr[bnd];
	buff1 = buff_arr[bnd + 1];
	buff2 = buff_arr[bnd + 2];
	buff3 = buff_arr[bnd + 3];
	yf0 = fp[0];
	yf1 = fp[1];
	yf2 = fp[2];
	yf3 = fp[3];

#if FLT_BITS == 2
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < (size >> 1); i++) {
		d64_2_f32 yd0, yd1, yd2, yd3, yy;

		yd0.d64 = ((mlib_d64 *)buff0)[i];
		yd1.d64 = ((mlib_d64 *)buff1)[i];
		yd2.d64 = ((mlib_d64 *)buff2)[i];
		yd3.d64 = ((mlib_d64 *)buff3)[i];
		yy.d64 = ((mlib_d64 *)ybuff)[i];

		yy.f32s.f0 += yd0.f32s.f0 * yf0 + yd1.f32s.f0 * yf1 +
		    yd2.f32s.f0 * yf2 + yd3.f32s.f0 * yf3;
		yy.f32s.f1 += yd0.f32s.f1 * yf0 + yd1.f32s.f1 * yf1 +
		    yd2.f32s.f1 * yf2 + yd3.f32s.f1 * yf3;

		SAT(dPtr[0], ival, yy.f32s.f0);
		dPtr += chan;
		yy.f32s.f0 = SAT_OFF;

		SAT(dPtr[0], ival, yy.f32s.f1);
		dPtr += chan;
		yy.f32s.f1 = SAT_OFF;

		((mlib_d64 *)ybuff)[i] = yy.d64;
	}

	if (size & 1) {
		d64_2_f32 yd0, yd1, yd2, yd3, yy;

		yd0.d64 = ((mlib_d64 *)buff0)[i];
		yd1.d64 = ((mlib_d64 *)buff1)[i];
		yd2.d64 = ((mlib_d64 *)buff2)[i];
		yd3.d64 = ((mlib_d64 *)buff3)[i];
		yy.d64 = ((mlib_d64 *)ybuff)[i];

		yy.f32s.f0 += yd0.f32s.f0 * yf0 + yd1.f32s.f0 * yf1 +
		    yd2.f32s.f0 * yf2 + yd3.f32s.f0 * yf3;

		SAT(dPtr[0], ival, yy.f32s.f0);
		yy.f32s.f0 = SAT_OFF;

		((mlib_d64 *)ybuff)[i] = yy.d64;
	}
#else /* FLT_BITS == 2 */

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < size; i++) {
		FP_TYPE c0, val;

		c0 = buff0[i] * yf0 + buff1[i] * yf1 + buff2[i] * yf2 +
		    buff3[i] * yf3;
		ybuff[i] += c0;
		val = ybuff[i];
		SAT(dPtr[0], ival, val);
		dPtr += chan;
		ybuff[i] = SAT_OFF;
	}

#endif /* FLT_BITS == 2 */
}

/* *********************************************************** */

static void
mlib_calc3(
    FILTER_TYPE *fp,
    FP_TYPE **buff_arr,
    mlib_s32 bnd,
    FP_TYPE *ybuff,
    mlib_zoom_workspace *ws)
{
	mlib_s32 i;
	FP_TYPE yf0, yf1, yf2;
	FP_TYPE *buff0, *buff1, *buff2;

#if FLT_BITS == 2
	mlib_s32 size2 = ws->size2;
#else /* FLT_BITS == 2 */
	mlib_s32 size = ws->size;
#endif /* FLT_BITS == 2 */

	buff0 = buff_arr[bnd];
	buff1 = buff_arr[bnd + 1];
	buff2 = buff_arr[bnd + 2];
	yf0 = fp[0];
	yf1 = fp[1];
	yf2 = fp[2];

#if FLT_BITS == 2
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < size2; i++) {
		d64_2_f32 yd0, yd1, yd2, yy;

		yd0.d64 = ((mlib_d64 *)buff0)[i];
		yd1.d64 = ((mlib_d64 *)buff1)[i];
		yd2.d64 = ((mlib_d64 *)buff2)[i];
		yy.d64 = ((mlib_d64 *)ybuff)[i];

		yy.f32s.f0 += yd0.f32s.f0 * yf0 + yd1.f32s.f0 * yf1 +
		    yd2.f32s.f0 * yf2;
		yy.f32s.f1 += yd0.f32s.f1 * yf0 + yd1.f32s.f1 * yf1 +
		    yd2.f32s.f1 * yf2;

		((mlib_d64 *)ybuff)[i] = yy.d64;
	}

#else /* FLT_BITS == 2 */

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < size; i++) {
		FP_TYPE c0;

		c0 = buff0[i] * yf0 + buff1[i] * yf1 + buff2[i] * yf2;
		ybuff[i] += c0;
	}

#endif /* FLT_BITS == 2 */
}

/* *********************************************************** */

static void
mlib_calc3s(
    FILTER_TYPE *fp,
    FP_TYPE **buff_arr,
    mlib_s32 bnd,
    FP_TYPE *ybuff,
    DTYPE *dPtr,
    mlib_zoom_workspace *ws)
{
	mlib_s32 i;
	FP_TYPE yf0, yf1, yf2;
	FP_TYPE *buff0, *buff1, *buff2;
	mlib_s32 size, chan;

#if IMG_TYPE < 3
#ifdef MLIB_USE_FTOI_CLAMPING
	mlib_s32 ival;
#endif /* MLIB_USE_FTOI_CLAMPING */
#endif /* IMG_TYPE < 3 */

	chan = ws->nchan;
	size = ws->size;
	buff0 = buff_arr[bnd];
	buff1 = buff_arr[bnd + 1];
	buff2 = buff_arr[bnd + 2];
	yf0 = fp[0];
	yf1 = fp[1];
	yf2 = fp[2];

#if FLT_BITS == 2
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < (size >> 1); i++) {
		d64_2_f32 yd0, yd1, yd2, yy;

		yd0.d64 = ((mlib_d64 *)buff0)[i];
		yd1.d64 = ((mlib_d64 *)buff1)[i];
		yd2.d64 = ((mlib_d64 *)buff2)[i];
		yy.d64 = ((mlib_d64 *)ybuff)[i];

		yy.f32s.f0 += yd0.f32s.f0 * yf0 + yd1.f32s.f0 * yf1 +
		    yd2.f32s.f0 * yf2;
		yy.f32s.f1 += yd0.f32s.f1 * yf0 + yd1.f32s.f1 * yf1 +
		    yd2.f32s.f1 * yf2;

		SAT(dPtr[0], ival, yy.f32s.f0);
		dPtr += chan;
		yy.f32s.f0 = SAT_OFF;

		SAT(dPtr[0], ival, yy.f32s.f1);
		dPtr += chan;
		yy.f32s.f1 = SAT_OFF;

		((mlib_d64 *)ybuff)[i] = yy.d64;
	}

	if (size & 1) {
		d64_2_f32 yd0, yd1, yd2, yy;

		yd0.d64 = ((mlib_d64 *)buff0)[i];
		yd1.d64 = ((mlib_d64 *)buff1)[i];
		yd2.d64 = ((mlib_d64 *)buff2)[i];
		yy.d64 = ((mlib_d64 *)ybuff)[i];

		yy.f32s.f0 += yd0.f32s.f0 * yf0 + yd1.f32s.f0 * yf1 +
		    yd2.f32s.f0 * yf2;

		SAT(dPtr[0], ival, yy.f32s.f0);
		yy.f32s.f0 = SAT_OFF;

		((mlib_d64 *)ybuff)[i] = yy.d64;
	}
#else /* FLT_BITS == 2 */

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < size; i++) {
		FP_TYPE c0, val;

		c0 = buff0[i] * yf0 + buff1[i] * yf1 + buff2[i] * yf2;
		ybuff[i] += c0;
		val = ybuff[i];
		SAT(dPtr[0], ival, val);
		dPtr += chan;
		ybuff[i] = SAT_OFF;
	}

#endif /* FLT_BITS == 2 */
}

/* *********************************************************** */

static void
mlib_calc2(
    FILTER_TYPE *fp,
    FP_TYPE **buff_arr,
    mlib_s32 bnd,
    FP_TYPE *ybuff,
    mlib_zoom_workspace *ws)
{
	mlib_s32 i;
	FP_TYPE yf0, yf1;
	FP_TYPE *buff0, *buff1;

#if FLT_BITS == 2
	mlib_s32 size2 = ws->size2;
#else /* FLT_BITS == 2 */
	mlib_s32 size = ws->size;
#endif /* FLT_BITS == 2 */

	buff0 = buff_arr[bnd];
	buff1 = buff_arr[bnd + 1];
	yf0 = fp[0];
	yf1 = fp[1];

#if FLT_BITS == 2
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < size2; i++) {
		d64_2_f32 yd0, yd1, yy;

		yd0.d64 = ((mlib_d64 *)buff0)[i];
		yd1.d64 = ((mlib_d64 *)buff1)[i];
		yy.d64 = ((mlib_d64 *)ybuff)[i];

		yy.f32s.f0 += yd0.f32s.f0 * yf0 + yd1.f32s.f0 * yf1;
		yy.f32s.f1 += yd0.f32s.f1 * yf0 + yd1.f32s.f1 * yf1;

		((mlib_d64 *)ybuff)[i] = yy.d64;
	}

#else /* FLT_BITS == 2 */

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < size; i++) {
		FP_TYPE c0;

		c0 = buff0[i] * yf0 + buff1[i] * yf1;
		ybuff[i] += c0;
	}

#endif /* FLT_BITS == 2 */
}

/* *********************************************************** */

static void
mlib_calc2s(
    FILTER_TYPE *fp,
    FP_TYPE **buff_arr,
    mlib_s32 bnd,
    FP_TYPE *ybuff,
    DTYPE *dPtr,
    mlib_zoom_workspace *ws)
{
	mlib_s32 i;
	FP_TYPE yf0, yf1;
	FP_TYPE *buff0, *buff1;
	mlib_s32 size, chan;

#if IMG_TYPE < 3
#ifdef MLIB_USE_FTOI_CLAMPING
	mlib_s32 ival;
#endif /* MLIB_USE_FTOI_CLAMPING */
#endif /* IMG_TYPE < 3 */

	chan = ws->nchan;
	size = ws->size;
	buff0 = buff_arr[bnd];
	buff1 = buff_arr[bnd + 1];
	yf0 = fp[0];
	yf1 = fp[1];

#if FLT_BITS == 2
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < (size >> 1); i++) {
		d64_2_f32 yd0, yd1, yy;

		yd0.d64 = ((mlib_d64 *)buff0)[i];
		yd1.d64 = ((mlib_d64 *)buff1)[i];
		yy.d64 = ((mlib_d64 *)ybuff)[i];

		yy.f32s.f0 += yd0.f32s.f0 * yf0 + yd1.f32s.f0 * yf1;
		yy.f32s.f1 += yd0.f32s.f1 * yf0 + yd1.f32s.f1 * yf1;

		SAT(dPtr[0], ival, yy.f32s.f0);
		dPtr += chan;
		yy.f32s.f0 = SAT_OFF;

		SAT(dPtr[0], ival, yy.f32s.f1);
		dPtr += chan;
		yy.f32s.f1 = SAT_OFF;

		((mlib_d64 *)ybuff)[i] = yy.d64;
	}

	if (size & 1) {
		d64_2_f32 yd0, yd1, yy;

		yd0.d64 = ((mlib_d64 *)buff0)[i];
		yd1.d64 = ((mlib_d64 *)buff1)[i];
		yy.d64 = ((mlib_d64 *)ybuff)[i];

		yy.f32s.f0 += yd0.f32s.f0 * yf0 + yd1.f32s.f0 * yf1;

		SAT(dPtr[0], ival, yy.f32s.f0);
		yy.f32s.f0 = SAT_OFF;

		((mlib_d64 *)ybuff)[i] = yy.d64;
	}
#else /* FLT_BITS == 2 */

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < size; i++) {
		FP_TYPE c0, val;

		c0 = buff0[i] * yf0 + buff1[i] * yf1;
		ybuff[i] += c0;
		val = ybuff[i];
		SAT(dPtr[0], ival, val);
		dPtr += chan;
		ybuff[i] = SAT_OFF;
	}

#endif /* FLT_BITS == 2 */
}

/* *********************************************************** */

static void
mlib_calc1(
    FILTER_TYPE *fp,
    FP_TYPE **buff_arr,
    mlib_s32 bnd,
    FP_TYPE *ybuff,
    mlib_zoom_workspace *ws)
{
	mlib_s32 i;
	FP_TYPE yf0;
	FP_TYPE *buff0;

#if FLT_BITS == 2
	mlib_s32 size2 = ws->size2;
#else /* FLT_BITS == 2 */
	mlib_s32 size = ws->size;
#endif /* FLT_BITS == 2 */

	buff0 = buff_arr[bnd];
	yf0 = fp[0];

#if FLT_BITS == 2
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < size2; i++) {
		d64_2_f32 yd0, yy;

		yd0.d64 = ((mlib_d64 *)buff0)[i];
		yy.d64 = ((mlib_d64 *)ybuff)[i];

		yy.f32s.f0 += yd0.f32s.f0 * yf0;
		yy.f32s.f1 += yd0.f32s.f1 * yf0;

		((mlib_d64 *)ybuff)[i] = yy.d64;
	}

#else /* FLT_BITS == 2 */

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < size; i++) {
		FP_TYPE c0;

		c0 = buff0[i] * yf0;
		ybuff[i] += c0;
	}

#endif /* FLT_BITS == 2 */
}

/* *********************************************************** */

static void
mlib_calc1s(
    FILTER_TYPE *fp,
    FP_TYPE **buff_arr,
    mlib_s32 bnd,
    FP_TYPE *ybuff,
    DTYPE *dPtr,
    mlib_zoom_workspace *ws)
{
	mlib_s32 i;
	FP_TYPE yf0;
	FP_TYPE *buff0;
	mlib_s32 size, chan;

#if IMG_TYPE < 3
#ifdef MLIB_USE_FTOI_CLAMPING
	mlib_s32 ival;
#endif /* MLIB_USE_FTOI_CLAMPING */
#endif /* IMG_TYPE < 3 */

	chan = ws->nchan;
	size = ws->size;
	buff0 = buff_arr[bnd];
	yf0 = fp[0];

#if FLT_BITS == 2
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < (size >> 1); i++) {
		d64_2_f32 yd0, yy;

		yd0.d64 = ((mlib_d64 *)buff0)[i];
		yy.d64 = ((mlib_d64 *)ybuff)[i];

		yy.f32s.f0 += yd0.f32s.f0 * yf0;
		yy.f32s.f1 += yd0.f32s.f1 * yf0;

		SAT(dPtr[0], ival, yy.f32s.f0);
		dPtr += chan;
		yy.f32s.f0 = SAT_OFF;

		SAT(dPtr[0], ival, yy.f32s.f1);
		dPtr += chan;
		yy.f32s.f1 = SAT_OFF;

		((mlib_d64 *)ybuff)[i] = yy.d64;
	}

	if (size & 1) {
		d64_2_f32 yd0, yy;

		yd0.d64 = ((mlib_d64 *)buff0)[i];
		yy.d64 = ((mlib_d64 *)ybuff)[i];

		yy.f32s.f0 += yd0.f32s.f0 * yf0;

		SAT(dPtr[0], ival, yy.f32s.f0);
		yy.f32s.f0 = SAT_OFF;

		((mlib_d64 *)ybuff)[i] = yy.d64;
	}
#else /* FLT_BITS == 2 */

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < size; i++) {
		FP_TYPE c0, val;

		c0 = buff0[i] * yf0;
		ybuff[i] += c0;
		val = ybuff[i];
		SAT(dPtr[0], ival, val);
		dPtr += chan;
		ybuff[i] = SAT_OFF;
	}

#endif /* FLT_BITS == 2 */
}

/* *********************************************************** */

#if FLT_BITS == 2

static void
calc_line_4_0(
    XPARAMS)
{
	DECL_LINE;
	d64_2_f32 xd0, xd1;

#ifndef SRC_EXTEND
	mlib_s32 dx = ws->dx;
	mlib_s32 nchan = ws->nchan,
	    chan2 = nchan + nchan, chan3 = chan2 + nchan;
#endif /* SRC_EXTEND */

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < size; i++) {
		MLIB_FILTER_X(x, filterX, 0);
		xd0.d64 = ((mlib_d64 *)fptr)[0];
		xd1.d64 = ((mlib_d64 *)fptr)[1];
		CALC_SRC_PTRX();

		pbuff[i] =
		    (GET_SRC(0) * xd0.f32s.f0 + GET_SRC(1) * xd0.f32s.f1 +
		    GET_SRC(2) * xd1.f32s.f0 + GET_SRC(3) * xd1.f32s.f1);

		UPDATE_X;
	}
}

static void
calc_line_3_0(
    XPARAMS)
{
	DECL_LINE;
	d64_2_f32 xd0, xd1;

#ifndef SRC_EXTEND
	mlib_s32 dx = ws->dx;
	mlib_s32 nchan = ws->nchan, chan2 = nchan + nchan;
#endif /* SRC_EXTEND */

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < size; i++) {
		MLIB_FILTER_X(x, filterX, 0);
		xd0.d64 = ((mlib_d64 *)fptr)[0];
		xd1.d64 = ((mlib_d64 *)fptr)[1];
		CALC_SRC_PTRX();

		pbuff[i] =
		    (GET_SRC(0) * xd0.f32s.f0 + GET_SRC(1) * xd0.f32s.f1 +
		    GET_SRC(2) * xd1.f32s.f0);

		UPDATE_X;
	}
}

static void
calc_line_2_0(
    XPARAMS)
{
	DECL_LINE;
	d64_2_f32 xd0;

#ifndef SRC_EXTEND
	mlib_s32 dx = ws->dx;
	mlib_s32 nchan = ws->nchan;
#endif /* SRC_EXTEND */

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < size; i++) {
		MLIB_FILTER_X(x, filterX, 0);
		xd0.d64 = ((mlib_d64 *)fptr)[0];
		CALC_SRC_PTRX();

		pbuff[i] =
		    (GET_SRC(0) * xd0.f32s.f0 + GET_SRC(1) * xd0.f32s.f1);

		UPDATE_X;
	}
}

#else /* FLT_BITS == 2 */

static void
calc_line_4_0(
    XPARAMS)
{
	DECL_LINE;
	FP_TYPE xf0, xf1, xf2, xf3;

#ifndef SRC_EXTEND
	mlib_s32 dx = ws->dx;
	mlib_s32 nchan = ws->nchan,
	    chan2 = nchan + nchan, chan3 = chan2 + nchan;
#endif /* SRC_EXTEND */

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < size; i++) {
		MLIB_FILTER_X(x, filterX, 0);
		xf0 = fptr[0];
		xf1 = fptr[1];
		xf2 = fptr[2];
		xf3 = fptr[3];
		CALC_SRC_PTRX();

		pbuff[i] =
		    GET_SRC(0) * xf0 + GET_SRC(1) * xf1 + GET_SRC(2) * xf2 +
		    GET_SRC(3) * xf3;

		UPDATE_X;
	}
}

static void
calc_line_3_0(
    XPARAMS)
{
	DECL_LINE;
	FP_TYPE xf0, xf1, xf2;

#ifndef SRC_EXTEND
	mlib_s32 dx = ws->dx;
	mlib_s32 nchan = ws->nchan, chan2 = nchan + nchan;
#endif /* SRC_EXTEND */

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < size; i++) {
		MLIB_FILTER_X(x, filterX, 0);
		xf0 = fptr[0];
		xf1 = fptr[1];
		xf2 = fptr[2];
		CALC_SRC_PTRX();

		pbuff[i] =
		    GET_SRC(0) * xf0 + GET_SRC(1) * xf1 + GET_SRC(2) * xf2;

		UPDATE_X;
	}
}

static void
calc_line_2_0(
    XPARAMS)
{
	DECL_LINE;
	FP_TYPE xf0, xf1;

#ifndef SRC_EXTEND
	mlib_s32 dx = ws->dx;
	mlib_s32 nchan = ws->nchan;
#endif /* SRC_EXTEND */

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < size; i++) {
		MLIB_FILTER_X(x, filterX, 0);
		xf0 = fptr[0];
		xf1 = fptr[1];
		CALC_SRC_PTRX();

		pbuff[i] = GET_SRC(0) * xf0 + GET_SRC(1) * xf1;

		UPDATE_X;
	}
}

#endif /* FLT_BITS == 2 */

/* *********************************************************** */

static void
calc_line_1_0(
    XPARAMS)
{
	DECL_LINE;
	FP_TYPE xf0;

#ifndef SRC_EXTEND
	mlib_s32 dx = ws->dx;
#endif /* SRC_EXTEND */

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < size; i++) {
		MLIB_FILTER_X(x, filterX, 0);
		xf0 = fptr[0];
		CALC_SRC_PTRX();

		pbuff[i] = GET_SRC(0) * xf0;

		UPDATE_X;
	}
}

/* *********************************************************** */

#if FLT_BITS == 2

static void
calc_line_4_off(
    XPARAMS)
{
	DECL_LINE;
	d64_2_f32 xd0, xd1;

#ifndef SRC_EXTEND
	mlib_s32 dx = ws->dx;
	mlib_s32 nchan = ws->nchan,
	    chan2 = nchan + nchan, chan3 = chan2 + nchan;
	mlib_s32 off = ws->off;
#endif /* SRC_EXTEND */

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < size; i++) {
		MLIB_FILTER_X(x, filterX, off);
		xd0.d64 = ((mlib_d64 *)fptr)[0];
		xd1.d64 = ((mlib_d64 *)fptr)[1];
		CALC_SRC_PTRX();

		pbuff[i] +=
		    (GET_SRC(0) * xd0.f32s.f0 + GET_SRC(1) * xd0.f32s.f1 +
		    GET_SRC(2) * xd1.f32s.f0 + GET_SRC(3) * xd1.f32s.f1);

		UPDATE_X;
	}
}

static void
calc_line_3_off(
    XPARAMS)
{
	DECL_LINE;
	d64_2_f32 xd0, xd1;

#ifndef SRC_EXTEND
	mlib_s32 dx = ws->dx;
	mlib_s32 nchan = ws->nchan, chan2 = nchan + nchan;
	mlib_s32 off = ws->off;
#endif /* SRC_EXTEND */

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < size; i++) {
		MLIB_FILTER_X(x, filterX, off);
		xd0.d64 = ((mlib_d64 *)fptr)[0];
		xd1.d64 = ((mlib_d64 *)fptr)[1];
		CALC_SRC_PTRX();

		pbuff[i] +=
		    (GET_SRC(0) * xd0.f32s.f0 + GET_SRC(1) * xd0.f32s.f1 +
		    GET_SRC(2) * xd1.f32s.f0);

		UPDATE_X;
	}
}

static void
calc_line_2_off(
    XPARAMS)
{
	DECL_LINE;
	d64_2_f32 xd0;

#ifndef SRC_EXTEND
	mlib_s32 dx = ws->dx;
	mlib_s32 nchan = ws->nchan;
	mlib_s32 off = ws->off;
#endif /* SRC_EXTEND */

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < size; i++) {
		MLIB_FILTER_X(x, filterX, off);
		xd0.d64 = ((mlib_d64 *)fptr)[0];
		CALC_SRC_PTRX();

		pbuff[i] +=
		    (GET_SRC(0) * xd0.f32s.f0 + GET_SRC(1) * xd0.f32s.f1);

		UPDATE_X;
	}
}

#else /* FLT_BITS == 2 */

static void
calc_line_4_off(
    XPARAMS)
{
	DECL_LINE;
	FP_TYPE xf0, xf1, xf2, xf3;

#ifndef SRC_EXTEND
	mlib_s32 dx = ws->dx;
	mlib_s32 nchan = ws->nchan,
	    chan2 = nchan + nchan, chan3 = chan2 + nchan;
	mlib_s32 off = ws->off;
#endif /* SRC_EXTEND */

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < size; i++) {
		MLIB_FILTER_X(x, filterX, off);
		xf0 = fptr[0];
		xf1 = fptr[1];
		xf2 = fptr[2];
		xf3 = fptr[3];
		CALC_SRC_PTRX();

		pbuff[i] +=
		    GET_SRC(0) * xf0 + GET_SRC(1) * xf1 + GET_SRC(2) * xf2 +
		    GET_SRC(3) * xf3;

		UPDATE_X;
	}
}

static void
calc_line_3_off(
    XPARAMS)
{
	DECL_LINE;
	FP_TYPE xf0, xf1, xf2;

#ifndef SRC_EXTEND
	mlib_s32 dx = ws->dx;
	mlib_s32 nchan = ws->nchan, chan2 = nchan + nchan;
	mlib_s32 off = ws->off;
#endif /* SRC_EXTEND */

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < size; i++) {
		MLIB_FILTER_X(x, filterX, off);
		xf0 = fptr[0];
		xf1 = fptr[1];
		xf2 = fptr[2];
		CALC_SRC_PTRX();

		pbuff[i] +=
		    GET_SRC(0) * xf0 + GET_SRC(1) * xf1 + GET_SRC(2) * xf2;

		UPDATE_X;
	}
}

static void
calc_line_2_off(
    XPARAMS)
{
	DECL_LINE;
	FP_TYPE xf0, xf1;

#ifndef SRC_EXTEND
	mlib_s32 dx = ws->dx;
	mlib_s32 nchan = ws->nchan;
	mlib_s32 off = ws->off;
#endif /* SRC_EXTEND */

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < size; i++) {
		MLIB_FILTER_X(x, filterX, off);
		xf0 = fptr[0];
		xf1 = fptr[1];
		CALC_SRC_PTRX();

		pbuff[i] += GET_SRC(0) * xf0 + GET_SRC(1) * xf1;

		UPDATE_X;
	}
}

#endif /* FLT_BITS == 2 */

/* *********************************************************** */

static void
calc_line_1_off(
    XPARAMS)
{
	DECL_LINE;
	FP_TYPE xf0;

#ifndef SRC_EXTEND
	mlib_s32 dx = ws->dx;
	mlib_s32 off = ws->off;
#endif /* SRC_EXTEND */

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < size; i++) {
		MLIB_FILTER_X(x, filterX, off);
		xf0 = fptr[0];
		CALC_SRC_PTRX();

		pbuff[i] += GET_SRC(0) * xf0;

		UPDATE_X;
	}
}

/* *********************************************************** */

#ifdef LOW_SPECIAL

static void
calc_4_1(
    X1PARAMS)
{
	DECL_LINE;
	FP_TYPE c0, xf0, xf1, xf2, xf3;
	FP_TYPE yf0;

#ifndef SRC_EXTEND
	mlib_s32 dx = ws->dx;
	mlib_s32 nchan = ws->nchan,
	    chan2 = nchan + nchan, chan3 = chan2 + nchan;
	mlib_s32 off = ws->off;
#endif /* SRC_EXTEND */

	yf0 = fpyy[0];

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < size; i++) {
		MLIB_FILTER_X(x, filterX, off);
		xf0 = fptr[0];
		xf1 = fptr[1];
		xf2 = fptr[2];
		xf3 = fptr[3];
		CALC_SRC_PTRX();

		c0 = GET_SRC(0) * xf0 + GET_SRC(1) * xf1 + GET_SRC(2) * xf2 +
		    GET_SRC(3) * xf3;
		ybuff[i] += c0 * yf0;

		UPDATE_X;
	}
}

static void
calc_3_1(
    X1PARAMS)
{
	DECL_LINE;
	FP_TYPE c0, xf0, xf1, xf2;
	FP_TYPE yf0;

#ifndef SRC_EXTEND
	mlib_s32 dx = ws->dx;
	mlib_s32 nchan = ws->nchan, chan2 = nchan + nchan;
	mlib_s32 off = ws->off;
#endif /* SRC_EXTEND */

	yf0 = fpyy[0];

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < size; i++) {
		MLIB_FILTER_X(x, filterX, off);
		xf0 = fptr[0];
		xf1 = fptr[1];
		xf2 = fptr[2];
		CALC_SRC_PTRX();

		c0 = GET_SRC(0) * xf0 + GET_SRC(1) * xf1 + GET_SRC(2) * xf2;
		ybuff[i] += c0 * yf0;

		UPDATE_X;
	}
}

static void
calc_2_1(
    X1PARAMS)
{
	DECL_LINE;
	FP_TYPE c0, xf0, xf1;
	FP_TYPE yf0;

#ifndef SRC_EXTEND
	mlib_s32 dx = ws->dx;
	mlib_s32 nchan = ws->nchan;
	mlib_s32 off = ws->off;
#endif /* SRC_EXTEND */

	yf0 = fpyy[0];

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < size; i++) {
		MLIB_FILTER_X(x, filterX, off);
		xf0 = fptr[0];
		xf1 = fptr[1];
		CALC_SRC_PTRX();

		c0 = GET_SRC(0) * xf0 + GET_SRC(1) * xf1;
		ybuff[i] += c0 * yf0;

		UPDATE_X;
	}
}

static void
calc_1_1(
    X1PARAMS)
{
	DECL_LINE;
	FP_TYPE c0, xf0;
	FP_TYPE yf0;

#ifndef SRC_EXTEND
	mlib_s32 dx = ws->dx;
	mlib_s32 off = ws->off;
#endif /* SRC_EXTEND */

	yf0 = fpyy[0];

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < size; i++) {
		MLIB_FILTER_X(x, filterX, off);
		xf0 = fptr[0];
		CALC_SRC_PTRX();

		c0 = GET_SRC(0) * xf0;
		ybuff[i] += c0 * yf0;

		UPDATE_X;
	}
}

static void
calc_4_2(
    X2PARAMS)
{
	DECL_LINE;
	FP_TYPE c0, c1, xf0, xf1, xf2, xf3;
	FP_TYPE yf0, yf1;

#ifndef SRC_EXTEND
	mlib_s32 dx = ws->dx;
	mlib_s32 nchan = ws->nchan,
	    chan2 = nchan + nchan, chan3 = chan2 + nchan;
	DTYPE *sP1_0 = sP1;
	mlib_s32 off = ws->off;
#endif /* SRC_EXTEND */

	yf0 = fpyy[0];
	yf1 = fpyy[1];

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < size; i++) {
		MLIB_FILTER_X(x, filterX, off);
		xf0 = fptr[0];
		xf1 = fptr[1];
		xf2 = fptr[2];
		xf3 = fptr[3];
		CALC_SRC_PTRX1();

		c0 = GET_SRC(0) * xf0 + GET_SRC(1) * xf1 + GET_SRC(2) * xf2 +
		    GET_SRC(3) * xf3;
		c1 = GET_SRC1(0) * xf0 + GET_SRC1(1) * xf1 + GET_SRC1(2) * xf2 +
		    GET_SRC1(3) * xf3;
		ybuff[i] += c0 * yf0 + c1 * yf1;

		UPDATE_X;
	}
}

static void
calc_3_2(
    X2PARAMS)
{
	DECL_LINE;
	FP_TYPE c0, c1, xf0, xf1, xf2;
	FP_TYPE yf0, yf1;

#ifndef SRC_EXTEND
	mlib_s32 dx = ws->dx;
	mlib_s32 nchan = ws->nchan, chan2 = nchan + nchan;
	DTYPE *sP1_0 = sP1;
	mlib_s32 off = ws->off;
#endif /* SRC_EXTEND */

	yf0 = fpyy[0];
	yf1 = fpyy[1];

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < size; i++) {
		MLIB_FILTER_X(x, filterX, off);
		xf0 = fptr[0];
		xf1 = fptr[1];
		xf2 = fptr[2];
		CALC_SRC_PTRX1();

		c0 = GET_SRC(0) * xf0 + GET_SRC(1) * xf1 + GET_SRC(2) * xf2;
		c1 = GET_SRC1(0) * xf0 + GET_SRC1(1) * xf1 + GET_SRC1(2) * xf2;
		ybuff[i] += c0 * yf0 + c1 * yf1;

		UPDATE_X;
	}
}

static void
calc_2_2(
    X2PARAMS)
{
	DECL_LINE;
	FP_TYPE c0, c1, xf0, xf1;
	FP_TYPE yf0, yf1;

#ifndef SRC_EXTEND
	mlib_s32 dx = ws->dx;
	mlib_s32 nchan = ws->nchan;
	DTYPE *sP1_0 = sP1;
	mlib_s32 off = ws->off;
#endif /* SRC_EXTEND */

	yf0 = fpyy[0];
	yf1 = fpyy[1];

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < size; i++) {
		MLIB_FILTER_X(x, filterX, off);
		xf0 = fptr[0];
		xf1 = fptr[1];
		CALC_SRC_PTRX1();

		c0 = GET_SRC(0) * xf0 + GET_SRC(1) * xf1;
		c1 = GET_SRC1(0) * xf0 + GET_SRC1(1) * xf1;
		ybuff[i] += c0 * yf0 + c1 * yf1;

		UPDATE_X;
	}
}

static void
calc_1_2(
    X2PARAMS)
{
	DECL_LINE;
	FP_TYPE c0, c1, xf0;
	FP_TYPE yf0, yf1;

#ifndef SRC_EXTEND
	mlib_s32 dx = ws->dx;
	DTYPE *sP1_0 = sP1;
	mlib_s32 off = ws->off;
#endif /* SRC_EXTEND */

	yf0 = fpyy[0];
	yf1 = fpyy[1];

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < size; i++) {
		MLIB_FILTER_X(x, filterX, off);
		xf0 = fptr[0];
		CALC_SRC_PTRX1();

		c0 = GET_SRC(0) * xf0;
		c1 = GET_SRC1(0) * xf0;
		ybuff[i] += c0 * yf0 + c1 * yf1;

		UPDATE_X;
	}
}

static void
calc_4_3(
    X3PARAMS)
{
	DECL_LINE;
	FP_TYPE c0, c1, c2;
	FP_TYPE xf0, xf1, xf2, xf3;
	FP_TYPE yf0, yf1, yf2;

#ifndef SRC_EXTEND
	mlib_s32 dx = ws->dx;
	mlib_s32 nchan = ws->nchan,
	    chan2 = nchan + nchan, chan3 = chan2 + nchan;
	DTYPE *sP1_0 = sP1;
	DTYPE *sP2_0 = sP2;
	mlib_s32 off = ws->off;
#endif /* SRC_EXTEND */

	yf0 = fpyy[0];
	yf1 = fpyy[1];
	yf2 = fpyy[2];

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < size; i++) {
		MLIB_FILTER_X(x, filterX, off);
		xf0 = fptr[0];
		xf1 = fptr[1];
		xf2 = fptr[2];
		xf3 = fptr[3];
		CALC_SRC_PTRX2();

		c0 = GET_SRC(0) * xf0 + GET_SRC(1) * xf1 + GET_SRC(2) * xf2 +
		    GET_SRC(3) * xf3;
		c1 = GET_SRC1(0) * xf0 + GET_SRC1(1) * xf1 + GET_SRC1(2) * xf2 +
		    GET_SRC1(3) * xf3;
		c2 = GET_SRC2(0) * xf0 + GET_SRC2(1) * xf1 + GET_SRC2(2) * xf2 +
		    GET_SRC2(3) * xf3;
		ybuff[i] += c0 * yf0 + c1 * yf1 + c2 * yf2;

		UPDATE_X;
	}
}

static void
calc_3_3(
    X3PARAMS)
{
	DECL_LINE;
	FP_TYPE c0, c1, c2;
	FP_TYPE xf0, xf1, xf2;
	FP_TYPE yf0, yf1, yf2;

#ifndef SRC_EXTEND
	mlib_s32 dx = ws->dx;
	mlib_s32 nchan = ws->nchan, chan2 = nchan + nchan;
	DTYPE *sP1_0 = sP1;
	DTYPE *sP2_0 = sP2;
	mlib_s32 off = ws->off;
#endif /* SRC_EXTEND */

	yf0 = fpyy[0];
	yf1 = fpyy[1];
	yf2 = fpyy[2];

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < size; i++) {
		MLIB_FILTER_X(x, filterX, off);
		xf0 = fptr[0];
		xf1 = fptr[1];
		xf2 = fptr[2];
		CALC_SRC_PTRX2();

		c0 = GET_SRC(0) * xf0 + GET_SRC(1) * xf1 + GET_SRC(2) * xf2;
		c1 = GET_SRC1(0) * xf0 + GET_SRC1(1) * xf1 + GET_SRC1(2) * xf2;
		c2 = GET_SRC2(0) * xf0 + GET_SRC2(1) * xf1 + GET_SRC2(2) * xf2;
		ybuff[i] += c0 * yf0 + c1 * yf1 + c2 * yf2;

		UPDATE_X;
	}
}

static void
calc_2_3(
    X3PARAMS)
{
	DECL_LINE;
	FP_TYPE c0, c1, c2;
	FP_TYPE xf0, xf1;
	FP_TYPE yf0, yf1, yf2;

#ifndef SRC_EXTEND
	mlib_s32 dx = ws->dx;
	mlib_s32 nchan = ws->nchan;
	DTYPE *sP1_0 = sP1;
	DTYPE *sP2_0 = sP2;
	mlib_s32 off = ws->off;
#endif /* SRC_EXTEND */

	yf0 = fpyy[0];
	yf1 = fpyy[1];
	yf2 = fpyy[2];

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < size; i++) {
		MLIB_FILTER_X(x, filterX, off);
		xf0 = fptr[0];
		xf1 = fptr[1];
		CALC_SRC_PTRX2();

		c0 = GET_SRC(0) * xf0 + GET_SRC(1) * xf1;
		c1 = GET_SRC1(0) * xf0 + GET_SRC1(1) * xf1;
		c2 = GET_SRC2(0) * xf0 + GET_SRC2(1) * xf1;
		ybuff[i] += c0 * yf0 + c1 * yf1 + c2 * yf2;

		UPDATE_X;
	}
}

static void
calc_1_3(
    X3PARAMS)
{
	DECL_LINE;
	FP_TYPE c0, c1, c2;
	FP_TYPE xf0;
	FP_TYPE yf0, yf1, yf2;

#ifndef SRC_EXTEND
	mlib_s32 dx = ws->dx;
	DTYPE *sP1_0 = sP1;
	DTYPE *sP2_0 = sP2;
	mlib_s32 off = ws->off;
#endif /* SRC_EXTEND */

	yf0 = fpyy[0];
	yf1 = fpyy[1];
	yf2 = fpyy[2];

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < size; i++) {
		MLIB_FILTER_X(x, filterX, off);
		xf0 = fptr[0];
		CALC_SRC_PTRX2();

		c0 = GET_SRC(0) * xf0;
		c1 = GET_SRC1(0) * xf0;
		c2 = GET_SRC2(0) * xf0;
		ybuff[i] += c0 * yf0 + c1 * yf1 + c2 * yf2;

		UPDATE_X;
	}
}

static void
calc_4_4(
    X4PARAMS)
{
	DECL_LINE;
	FP_TYPE c0, c1, c2, c3;
	FP_TYPE xf0, xf1, xf2, xf3;
	FP_TYPE yf0, yf1, yf2, yf3;

#ifndef SRC_EXTEND
	mlib_s32 dx = ws->dx;
	mlib_s32 nchan = ws->nchan,
	    chan2 = nchan + nchan, chan3 = chan2 + nchan;
	DTYPE *sP1_0 = sP1;
	DTYPE *sP2_0 = sP2;
	DTYPE *sP3_0 = sP3;
	mlib_s32 off = ws->off;
#endif /* SRC_EXTEND */

	yf0 = fpyy[0];
	yf1 = fpyy[1];
	yf2 = fpyy[2];
	yf3 = fpyy[3];

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < size; i++) {
		MLIB_FILTER_X(x, filterX, off);
		xf0 = fptr[0];
		xf1 = fptr[1];
		xf2 = fptr[2];
		xf3 = fptr[3];
		CALC_SRC_PTRX3();

		c0 = GET_SRC(0) * xf0 + GET_SRC(1) * xf1 + GET_SRC(2) * xf2 +
		    GET_SRC(3) * xf3;
		c1 = GET_SRC1(0) * xf0 + GET_SRC1(1) * xf1 + GET_SRC1(2) * xf2 +
		    GET_SRC1(3) * xf3;
		c2 = GET_SRC2(0) * xf0 + GET_SRC2(1) * xf1 + GET_SRC2(2) * xf2 +
		    GET_SRC2(3) * xf3;
		c3 = GET_SRC3(0) * xf0 + GET_SRC3(1) * xf1 + GET_SRC3(2) * xf2 +
		    GET_SRC3(3) * xf3;
		ybuff[i] += c0 * yf0 + c1 * yf1 + c2 * yf2 + c3 * yf3;

		UPDATE_X;
	}
}

static void
calc_3_4(
    X4PARAMS)
{
	DECL_LINE;
	FP_TYPE c0, c1, c2, c3;
	FP_TYPE xf0, xf1, xf2;
	FP_TYPE yf0, yf1, yf2, yf3;

#ifndef SRC_EXTEND
	mlib_s32 dx = ws->dx;
	mlib_s32 nchan = ws->nchan, chan2 = nchan + nchan;
	DTYPE *sP1_0 = sP1;
	DTYPE *sP2_0 = sP2;
	DTYPE *sP3_0 = sP3;
	mlib_s32 off = ws->off;
#endif /* SRC_EXTEND */

	yf0 = fpyy[0];
	yf1 = fpyy[1];
	yf2 = fpyy[2];
	yf3 = fpyy[3];

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < size; i++) {
		MLIB_FILTER_X(x, filterX, off);
		xf0 = fptr[0];
		xf1 = fptr[1];
		xf2 = fptr[2];
		CALC_SRC_PTRX3();

		c0 = GET_SRC(0) * xf0 + GET_SRC(1) * xf1 + GET_SRC(2) * xf2;
		c1 = GET_SRC1(0) * xf0 + GET_SRC1(1) * xf1 + GET_SRC1(2) * xf2;
		c2 = GET_SRC2(0) * xf0 + GET_SRC2(1) * xf1 + GET_SRC2(2) * xf2;
		c3 = GET_SRC3(0) * xf0 + GET_SRC3(1) * xf1 + GET_SRC3(2) * xf2;
		ybuff[i] += c0 * yf0 + c1 * yf1 + c2 * yf2 + c3 * yf3;

		UPDATE_X;
	}
}

static void
calc_2_4(
    X4PARAMS)
{
	DECL_LINE;
	FP_TYPE c0, c1, c2, c3;
	FP_TYPE xf0, xf1;
	FP_TYPE yf0, yf1, yf2, yf3;

#ifndef SRC_EXTEND
	mlib_s32 dx = ws->dx;
	mlib_s32 nchan = ws->nchan;
	DTYPE *sP1_0 = sP1;
	DTYPE *sP2_0 = sP2;
	DTYPE *sP3_0 = sP3;
	mlib_s32 off = ws->off;
#endif /* SRC_EXTEND */

	yf0 = fpyy[0];
	yf1 = fpyy[1];
	yf2 = fpyy[2];
	yf3 = fpyy[3];

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < size; i++) {
		MLIB_FILTER_X(x, filterX, off);
		xf0 = fptr[0];
		xf1 = fptr[1];
		CALC_SRC_PTRX3();

		c0 = GET_SRC(0) * xf0 + GET_SRC(1) * xf1;
		c1 = GET_SRC1(0) * xf0 + GET_SRC1(1) * xf1;
		c2 = GET_SRC2(0) * xf0 + GET_SRC2(1) * xf1;
		c3 = GET_SRC3(0) * xf0 + GET_SRC3(1) * xf1;
		ybuff[i] += c0 * yf0 + c1 * yf1 + c2 * yf2 + c3 * yf3;

		UPDATE_X;
	}
}

static void
calc_1_4(
    X4PARAMS)
{
	DECL_LINE;
	FP_TYPE c0, c1, c2, c3;
	FP_TYPE xf0;
	FP_TYPE yf0, yf1, yf2, yf3;

#ifndef SRC_EXTEND
	mlib_s32 dx = ws->dx;
	DTYPE *sP1_0 = sP1;
	DTYPE *sP2_0 = sP2;
	DTYPE *sP3_0 = sP3;
	mlib_s32 off = ws->off;
#endif /* SRC_EXTEND */

	yf0 = fpyy[0];
	yf1 = fpyy[1];
	yf2 = fpyy[2];
	yf3 = fpyy[3];

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < size; i++) {
		MLIB_FILTER_X(x, filterX, off);
		xf0 = fptr[0];
		CALC_SRC_PTRX3();

		c0 = GET_SRC(0) * xf0;
		c1 = GET_SRC1(0) * xf0;
		c2 = GET_SRC2(0) * xf0;
		c3 = GET_SRC3(0) * xf0;
		ybuff[i] += c0 * yf0 + c1 * yf1 + c2 * yf2 + c3 * yf3;

		UPDATE_X;
	}
}

/* *********************************************************** */

static void
mlib_ImageZoomTranslateTable_low(
    PARAMS,
    FP_TYPE *buff)
{
	DECLAREVAR;
#ifndef SRC_EXTEND
	mlib_s32 xRight;
#endif /* SRC_EXTEND */
	DTYPE *sP1, *sP2, *sP3;
	FILTER_TYPE *fpyy;
	mlib_s32 yf_shift, yf_mask;
	FP_TYPE *ybuff;
	FP_TYPE val;
	DTYPE *dPtr;
	DTYPE *dstDataIni;
	mlib_s32 y, x;

#ifndef SRC_EXTEND
	mlib_s32 coff;
#else /* SRC_EXTEND */
	mlib_s32 *x_tab;
#endif /* SRC_EXTEND */

	ybuff = buff + n * max_xsize;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < max_xsize; i++) {
		ybuff[i] = SAT_OFF;
	}

#if FLT_BITS == 2
	filterX = table->dataH_f32;
	filterY = table->dataV_f32;
#else /* FLT_BITS == 2 */
	filterX = table->dataH_d64;
	filterY = table->dataV_d64;
#endif /* FLT_BITS == 2 */

	DIST_BITS();

#ifndef SRC_EXTEND
	switch (nchan) {
	case 1:
		sbits = 0;
		break;
	case 2:
		sbits = 1;
		break;
	case 3:
		sbits = 1;
		break;
	case 4:
		sbits = 2;
		break;
	}

	ws->x_mask0 = 0;

	if (nchan == 3)
		ws->x_mask0 = ~0;

#else /* SRC_EXTEND */
	sbits = 0;
#endif /* SRC_EXTEND */
	x_mask = ~((1 << sbits) - 1);
	x_shift -= sbits;

	ws->x_shift = x_shift;
	ws->x_mask = x_mask;
	ws->xf_shift = xf_shift;
	ws->xf_mask = xf_mask;
	ws->yf_shift = yf_shift;
	ws->yf_mask = yf_mask;

#ifndef SRC_EXTEND
	xLeft = leftEdges[yStart];
	xRight = rightEdges[yStart];
	if (xLeft > xRight) {
		return;
	}
	size = xRight - xLeft + 1;
	x0 = xStarts[yStart];

/* move to kernel center */
	x0 -= ws->x_move;
#else /* SRC_EXTEND */
	size = leftEdges[0];
	xLeft = leftEdges[1];
	if (size <= 0) {
		return;
	}
	x_tab = rightEdges;
	x0 = x_tab[0];
#endif /* SRC_EXTEND */

	ws->x0 = x0;
	ws->size = size;
	dstDataIni = (DTYPE *) dstData + CHAN1 * xLeft;

	for (k = 0; k < CHAN1; k++) {
		dstPixelPtr = dstDataIni;

		for (j = yStart; j <= yFinish; j++) {
			CLIP;

/* move to kernel center */
			y = y0 - ws->y_move;
			GET_FILTER_Y();

			ynew = (ynew - ws->y_move) >> y_shift;
			ySrc = y >> y_shift;

			ws->y0 = y0;

			for (l = 0; l < n; l += kh) {

				kh = n - l;

#if IMG_TYPE <= 2

				if (kh > 2)
					kh = 2;
#else /* IMG_TYPE <= 2 */

				if (kh > MAX_KER)
					kh = MAX_KER;
#endif /* IMG_TYPE <= 2 */
				fpyy = fpy + l;

				if (kh == 4) {

/* offset in current kernel line */
					for (off = 0; off < m; off += kw) {
						x = x0;
						ws->off = off;
#ifndef SRC_EXTEND
						coff = k + CHAN1 * off;
#endif /* SRC_EXTEND */
						CALC_SRC_PTRY4();

						kw = m - off;

						if (kw > MAX_KER)
							kw = MAX_KER;

						if (kw == 4) {
							calc_4_4(CALL_X1PARAMS,
							    sP1, sP2, sP3);

						} else if (kw == 3) {
							calc_3_4(CALL_X1PARAMS,
							    sP1, sP2, sP3);

						} else if (kw == 2) {
							calc_2_4(CALL_X1PARAMS,
							    sP1, sP2, sP3);

						} else {	/* if (kw==1 */

							calc_1_4(CALL_X1PARAMS,
							    sP1, sP2, sP3);
						}
					}

				} else if (kh == 3) {

/* offset in current kernel line */
					for (off = 0; off < m; off += kw) {
						x = x0;
						ws->off = off;
#ifndef SRC_EXTEND
						coff = k + CHAN1 * off;
#endif /* SRC_EXTEND */
						CALC_SRC_PTRY3();

						kw = m - off;

						if (kw > MAX_KER)
							kw = MAX_KER;

						if (kw == 4) {
							calc_4_3(CALL_X1PARAMS,
							    sP1, sP2);

						} else if (kw == 3) {
							calc_3_3(CALL_X1PARAMS,
							    sP1, sP2);

						} else if (kw == 2) {
							calc_2_3(CALL_X1PARAMS,
							    sP1, sP2);

						} else {	/* if (kw==1) */

							calc_1_3(CALL_X1PARAMS,
							    sP1, sP2);
						}
					}

				} else if (kh == 2) {

/* offset in current kernel line */
					for (off = 0; off < m; off += kw) {
						x = x0;
						ws->off = off;
#ifndef SRC_EXTEND
						coff = k + CHAN1 * off;
#endif /* SRC_EXTEND */
						CALC_SRC_PTRY2();

						kw = m - off;

						if (kw > MAX_KER)
							kw = MAX_KER;

						if (kw == 4) {
							calc_4_2(CALL_X1PARAMS,
							    sP1);

						} else if (kw == 3) {
							calc_3_2(CALL_X1PARAMS,
							    sP1);

						} else if (kw == 2) {
							calc_2_2(CALL_X1PARAMS,
							    sP1);

						} else {	/* if (kw==1) */

							calc_1_2(CALL_X1PARAMS,
							    sP1);
						}
					}

				} else {
/* kh == 1 */

/* offset in current kernel line */
					for (off = 0; off < m; off += kw) {
						x = x0;
						ws->off = off;
#ifndef SRC_EXTEND
						coff = k + CHAN1 * off;
#endif /* SRC_EXTEND */
						CALC_SRC_PTRY1();

						kw = m - off;

						if (kw > MAX_KER)
							kw = MAX_KER;

						if (kw == 4) {
							calc_4_1(CALL_X1PARAMS);

						} else if (kw == 3) {
							calc_3_1(CALL_X1PARAMS);

						} else if (kw == 2) {
							calc_2_1(CALL_X1PARAMS);

						} else {	/* if (kw==1) */

							calc_1_1(CALL_X1PARAMS);
						}
					}
				}
			}

			dPtr = dstPixelPtr + k;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < size; i++) {
#if IMG_TYPE < 3
#ifdef MLIB_USE_FTOI_CLAMPING
				mlib_s32 ival;
#endif /* MLIB_USE_FTOI_CLAMPING */
#endif /* IMG_TYPE < 3 */
				val = ybuff[i];
				SAT(dPtr[0], ival, val);
				dPtr += CHAN1;
				ybuff[i] = SAT_OFF;
			}
		}
	}
}

#endif /* LOW_SPECIAL */

/* *********************************************************** */

static void
mlib_ImageZoomTranslateTable_high(
    PARAMS,
    FP_TYPE *buff)
{
	DECLAREVAR;
#ifndef SRC_EXTEND
	mlib_s32 xRight;
#endif /* SRC_EXTEND */
	mlib_s32 yf_shift, yf_mask;
	FP_TYPE **buff_arr, *pbuff, *ybuff;
	DTYPE *dPtr;
	DTYPE *dstDataIni;
	mlib_s32 bind, y_step, y, x;

#ifndef SRC_EXTEND
	mlib_s32 coff;
#else /* SRC_EXTEND */
	mlib_s32 *x_tab;
#endif /* SRC_EXTEND */

	ybuff = buff + n * max_xsize;
	buff_arr = (FP_TYPE **) (ybuff + max_xsize);

	for (k = 0; k < n; k++)
		buff_arr[k] = buff + k * max_xsize;
	for (k = 0; k < n; k++)
		buff_arr[n + k] = buff_arr[k];

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < max_xsize; i++) {
		ybuff[i] = SAT_OFF;
	}

#if FLT_BITS == 2
	filterX = table->dataH_f32;
	filterY = table->dataV_f32;
#else /* FLT_BITS == 2 */
	filterX = table->dataH_d64;
	filterY = table->dataV_d64;
#endif /* FLT_BITS == 2 */

	DIST_BITS();

#ifndef SRC_EXTEND
	switch (nchan) {
	case 1:
		sbits = 0;
		break;
	case 2:
		sbits = 1;
		break;
	case 3:
		sbits = 1;
		break;
	case 4:
		sbits = 2;
		break;
	}

	ws->x_mask0 = 0;

	if (nchan == 3)
		ws->x_mask0 = ~0;

#else /* SRC_EXTEND */
	sbits = 0;
#endif /* SRC_EXTEND */
	x_mask = ~((1 << sbits) - 1);
	x_shift -= sbits;

	ws->x_shift = x_shift;
	ws->x_mask = x_mask;
	ws->xf_shift = xf_shift;
	ws->xf_mask = xf_mask;
	ws->yf_shift = yf_shift;
	ws->yf_mask = yf_mask;

#ifndef SRC_EXTEND
	xLeft = leftEdges[yStart];
	xRight = rightEdges[yStart];
	if (xLeft > xRight) {
		return;
	}
	size = xRight - xLeft + 1;
	x0 = xStarts[yStart];

/* move to kernel center */
	x0 -= ws->x_move;
#else /* SRC_EXTEND */
	size = leftEdges[0];
	xLeft = leftEdges[1];
	if (size <= 0) {
		return;
	}
	x_tab = rightEdges;
	x0 = x_tab[0];
#endif /* SRC_EXTEND */

	ws->x0 = x0;
	ws->size = size;

#if FLT_BITS == 2
	ws->size2 = (size + 1) >> 1;
#endif /* FLT_BITS == 2 */

	dstDataIni = (DTYPE *) dstData + CHAN1 * xLeft;

	for (k = 0; k < CHAN1; k++) {
		dstPixelPtr = dstDataIni;
		y_step = n;
		bind = 0;

		for (j = yStart; j <= yFinish; j++) {
			CLIP if (y_step >= n) {
				y_step = n;
			} else {
				bind += y_step;

				if (bind >= n)
					bind -= n;
			}

			ws->y_step = y_step;

/* move to kernel center */
			y = y0 - ws->y_move;
			GET_FILTER_Y();

			ynew = (ynew - ws->y_move) >> y_shift;
			ySrc = y >> y_shift;

			ws->y0 = y0;
			dPtr = dstPixelPtr + k;

			for (l = n - y_step; l < n; l++) {

				pbuff = buff_arr[bind + l];

/* offset in current kernel line */
				for (off = 0; off < m; off += kw) {
					x = x0;

					ws->off = off;
#ifndef SRC_EXTEND
					coff = k + CHAN1 * off;
#endif /* SRC_EXTEND */
					CALC_SRC_PTRY1();

					kw = m - off;

					if (kw >= MAX_KER)
						kw = MAX_KER;

					if (off == 0) {
						if (kw == 4) {
							calc_line_4_0
							    (CALL_XPARAMS);

						} else if (kw == 3) {
							calc_line_3_0
							    (CALL_XPARAMS);

						} else if (kw == 2) {
							calc_line_2_0
							    (CALL_XPARAMS);

						} else {	/* if (kw==1) */

							calc_line_1_0
							    (CALL_XPARAMS);
						}

					} else {

						if (kw == 4) {
							calc_line_4_off
							    (CALL_XPARAMS);

						} else if (kw == 3) {
							calc_line_3_off
							    (CALL_XPARAMS);

						} else if (kw == 2) {
							calc_line_2_off
							    (CALL_XPARAMS);

						} else {	/* if (kw==1) */

							calc_line_1_off
							    (CALL_XPARAMS);
						}
					}
				}
			}

			for (l = 0; l < n; l += kh) {
				kh = n - l;

				if (kh >= MAX_KER)
					kh = MAX_KER;

				if (kh == 4) {
					if ((l + kh) == n) {
						mlib_calc4s(fpy + l, buff_arr,
						    bind + l, ybuff, dPtr, ws);
					} else {
						mlib_calc4(fpy + l, buff_arr,
						    bind + l, ybuff, ws);
					}

				} else if (kh == 3) {

					if ((l + kh) == n) {
						mlib_calc3s(fpy + l, buff_arr,
						    bind + l, ybuff, dPtr, ws);
					} else {
						mlib_calc3(fpy + l, buff_arr,
						    bind + l, ybuff, ws);
					}

				} else if (kh == 2) {

					if ((l + kh) == n) {
						mlib_calc2s(fpy + l, buff_arr,
						    bind + l, ybuff, dPtr, ws);
					} else {
						mlib_calc2(fpy + l, buff_arr,
						    bind + l, ybuff, ws);
					}

				} else {
/* kh == 1 */

					if ((l + kh) == n) {
						mlib_calc1s(fpy + l, buff_arr,
						    bind + l, ybuff, dPtr, ws);
					} else {
						mlib_calc1(fpy + l, buff_arr,
						    bind + l, ybuff, ws);
					}
				}
			}

			y_step = ynew - ySrc;
		}
	}
}

/* *********************************************************** */

mlib_status
FUNC_NAME(
    PARAMS)
{
	FP_TYPE *buff;
	mlib_d64 zoomy;
	mlib_s32 n, max_xsize;

	max_xsize = ws->max_xsize;
	zoomy = ws->zoomy;
	n = table->height;

	max_xsize = (max_xsize + 1) & ~1;
	ws->max_xsize = max_xsize;
	buff =
	    __mlib_malloc((n + 1) * max_xsize * sizeof (FP_TYPE) +
	    2 * n * sizeof (FP_TYPE *));

	if (buff == NULL)
		return (MLIB_FAILURE);

#ifdef LOW_SPECIAL

/* 1/zoomy >= n */
	if (n * zoomy <= 1) {
		mlib_ImageZoomTranslateTable_low(CALL_PARAMS);
	} else
#endif /* LOW_SPECIAL */
	{
		mlib_ImageZoomTranslateTable_high(CALL_PARAMS);
	}

	__mlib_free(buff);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#if IMG_TYPE == 4 || IMG_TYPE == 5
#ifdef _MSC_VER
#pragma optimize("", on)
#endif /* _MSC_VER */
#endif /* IMG_TYPE == 4 || IMG_TYPE == 5 */

#ifdef __cplusplus
}
#endif

#endif /* _MLIB_IMAGEZOOMTRANSLATETABLE_F_H */
