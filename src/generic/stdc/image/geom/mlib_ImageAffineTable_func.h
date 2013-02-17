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

#ifndef _MLIB_IMAGEAFFINETABLE_FUNC_H
#define	_MLIB_IMAGEAFFINETABLE_FUNC_H

#pragma ident	"@(#)mlib_ImageAffineTable_func.h	9.2	07/10/09 SMI"

/*
 * FUNCTION
 *      Internal functions for mlib_ImageAffineTable.
 */

#ifdef __cplusplus
extern "C" {
#endif

#include <mlib_ImageDivTables.h>

/* *********************************************************** */

#if FLT_BITS == 2

#define	FILTER_TYPE	mlib_f32
#define	FP_TYPE	mlib_f32

#elif FLT_BITS == 3

#define	FILTER_TYPE	mlib_d64
#define	FP_TYPE	mlib_d64

#endif		   /* FLT_BITS == 2 */

/* *********************************************************** */

#if IMG_TYPE == 1

#define	DTYPE	mlib_u8
#define	SAT(dst, ival, val)	SAT8(dst, ival, val)
#ifdef MLIB_USE_FTOI_CLAMPING
#define	SAT_OFF	-127.5
#else
#define	SAT_OFF	0.5
#endif		   /* MLIB_USE_FTOI_CLAMPING */
#define	SRC2FP(x)	U82FP(x)

#elif IMG_TYPE == 2

#define	DTYPE	mlib_s16
#define	SAT(dst, ival, val)	SAT16(dst, ival, val)
#ifdef MLIB_USE_FTOI_CLAMPING
#define	SAT_OFF	0.5
#else
#define	SAT_OFF	0.0
#endif		   /* MLIB_USE_FTOI_CLAMPING */
#define	SRC2FP(x)	(x)

#elif IMG_TYPE == 3

#define	DTYPE	mlib_s32
#define	SAT(dst, ival, val)	SAT32(dst, ival, val)
#define	SAT_OFF	0.0
#define	SRC2FP(x)	(x)

#elif IMG_TYPE == 4

#define	DTYPE	mlib_f32
#define	SAT(dst, ival, val)	dst = val
#define	SAT_OFF	0.0
#define	SRC2FP(x)	(x)

#elif IMG_TYPE == 5

#define	DTYPE	mlib_d64
#define	SAT(dst, ival, val)	dst = val
#define	SAT_OFF	0.0
#define	SRC2FP(x)	(x)

#elif IMG_TYPE == 0

#define	DTYPE	mlib_u16
#define	SAT(dst, ival, val)	SAT_U16(dst, ival, val)
#ifdef MLIB_USE_FTOI_CLAMPING
#define	SAT_OFF	-32767.5
#else
#define	SAT_OFF	0.5
#endif		   /* MLIB_USE_FTOI_CLAMPING */
#define	SRC2FP(x)	(x)
#endif		   /* IMG_TYPE == 1 */

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

#else

#define	CLAMP(DST, val, max, min, type)                         \
	if (val >= max)                                         \
	    val = max;                                          \
	if (val <= min)                                         \
	    val = min;                                          \
	DST = (type) val
#define	SAT8(DST, ival, val)                                    \
	CLAMP(DST, val, MLIB_U8_MAX, MLIB_U8_MIN, mlib_u8)
#define	SAT16(DST, ival, val)                                   \
	CLAMP(DST, val, MLIB_S16_MAX, MLIB_S16_MIN, mlib_s16)
#define	SAT_U16(DST, ival, val)                                 \
	CLAMP(DST, val, MLIB_U16_MAX, MLIB_U16_MIN, mlib_u16)
#define	SAT32(DST, ival, val)                                   \
	CLAMP(DST, val, MLIB_S32_MAX, MLIB_S32_MIN, mlib_s32)

#endif		   /* MLIB_USE_FTOI_CLAMPING */

/* *********************************************************** */

#ifdef _MSC_VER
#define	U82FP(x)	(x)
#else
#if FLT_BITS == 2
#define	U82FP(x)	mlib_U82F32[(x)]
#elif FLT_BITS == 3
#define	U82FP(x)	mlib_U82D64[(x)]
#endif		   /* FLT_BITS == 2 */
#endif		   /* _MSC_VER */

/* *********************************************************** */

#define	DECLAREVAR                                              \
	mlib_s32 *sides = ws->sides;                            \
	mlib_s32 affine_mask = ws->affine_mask;                 \
	mlib_s32 type = ws->type;                               \
	mlib_s32 xLeft, xRight, x0, y0;                         \
	mlib_s32 yStart = ws->yStart;                           \
	mlib_s32 yFinish = ws->yFinish;                         \
	mlib_s32 max_xsize = ws->max_xsize;                     \
	mlib_s32 dstStride = ws->dstStride;                     \
	mlib_s32 nchan = ws->nchan;                             \
	mlib_s32 x_shift = ws->x_shift0;                        \
	mlib_s32 y_shift = ws->y_shift;                         \
	mlib_s32 xf_shift, xf_mask, yf_shift, yf_mask;          \
	mlib_s32 b_step, old_size, size = 0;                    \
	DTYPE *dstPixelPtr;                                     \
	mlib_s32 m = table->width;                              \
	mlib_s32 n = table->height;                             \
	mlib_s32 xf_bits = table->width_bits;                   \
	mlib_s32 yf_bits = table->height_bits;                  \
	mlib_s32 subsampleBitsH = table->subsampleBitsH;        \
	mlib_s32 subsampleBitsV = table->subsampleBitsV;        \
	FILTER_TYPE *filterX, *filterY;                         \
	mlib_s32 i, j, k, l, kw, kh, off

/* *********************************************************** */

#define	DEF_VARS_LINE                                           \
	FP_TYPE xf0, yf0, c0;                                   \
	DTYPE *srcPixelPtr;                                     \
	FILTER_TYPE *fptr;                                      \
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
	mlib_s32 k = ws->k;                                     \
	mlib_s32 b_step = ws->b_step

/* *********************************************************** */

#define	CLIP(NCHAN)                                             \
	dstData += dstStride;                                   \
	xLeft = leftEdges[j];                                   \
	xRight = rightEdges[j];                                 \
	if (xLeft > xRight)                                     \
	    continue;                                           \
	x0 = xStarts[j];                                        \
	y0 = yStarts[j];                                        \
	if (affine_mask != 0) {                                 \
	    ws->dx = sides[j * 2 + 2];                          \
	    ws->dy = sides[j * 2 + 3];                          \
	}                                                       \
	dstPixelPtr = (DTYPE *) dstData + NCHAN * xLeft;        \
	size = xRight - xLeft + 1

/* *********************************************************** */

#define	DIST_BITS()                                                        \
	xf_shift = x_shift - subsampleBitsH - xf_bits - FLT_BITS;          \
	xf_mask = (((1 << subsampleBitsH) - 1) << (xf_bits + FLT_BITS));   \
	yf_shift = y_shift - subsampleBitsV - yf_bits - FLT_BITS;          \
	yf_mask = (((1 << subsampleBitsV) - 1) << (yf_bits + FLT_BITS))

/* *********************************************************** */

#ifdef SRC_EXTEND
#define	CALC_SRC_PTR()                                          \
	xSrc = (x >> x_shift);                                  \
	ySrc = (y >> y_shift);                                  \
	srcPixelPtr = (DTYPE *) lineAddr[ySrc] + k
#define	NEXT_LINE(N)	srcPixelPtr = (DTYPE*)lineAddr[ySrc + N] + k
#define	GET_SRC(N)	SRC2FP(srcPixelPtr[x_ind[xSrc + N]])
#define	CALL_FUNC(N)                                                    \
	if (kh == 4) {                                                  \
	    mlib_ImageAffineTableLine_##N##ext_4_4(buff,                \
		filterX + off, filterY + l, lineAddr + l, x_ind, ws);   \
	} else if (kh == 2) {                                           \
	    if (kw == 4)                                                \
		mlib_ImageAffineTableLine_##N##ext_2_4(buff,            \
		    filterX + off, filterY + l, lineAddr + l, x_ind,    \
		    ws);                                                \
	    else if (kw == 3)                                           \
		mlib_ImageAffineTableLine_##N##ext_2_3(buff,            \
		    filterX + off, filterY + l, lineAddr + l, x_ind,    \
		    ws);                                                \
	    else if (kw == 2)                                           \
		mlib_ImageAffineTableLine_##N##ext_2_2(buff,            \
		    filterX + off, filterY + l, lineAddr + l, x_ind,    \
		    ws);                                                \
	    else if (kw == 1)                                           \
		mlib_ImageAffineTableLine_##N##ext_2_1(buff,            \
		    filterX + off, filterY + l, lineAddr + l, x_ind,    \
		    ws);                                                \
	} else                                                          \
/*                                                                      \
 * if (kh == 1)                                                         \
 */                                                                     \
	{                                                               \
	    if (kw == 4)                                                \
		mlib_ImageAffineTableLine_##N##ext_1_4(buff,            \
		    filterX + off, filterY + l, lineAddr + l, x_ind,    \
		    ws);                                                \
	    else if (kw == 3)                                           \
		mlib_ImageAffineTableLine_##N##ext_1_3(buff,            \
		    filterX + off, filterY + l, lineAddr + l, x_ind,    \
		    ws);                                                \
	    else if (kw == 2)                                           \
		mlib_ImageAffineTableLine_##N##ext_1_2(buff,            \
		    filterX + off, filterY + l, lineAddr + l, x_ind,    \
		    ws);                                                \
	    else if (kw == 1)                                           \
		mlib_ImageAffineTableLine_##N##ext_1_1(buff,            \
		    filterX + off, filterY + l, lineAddr + l, x_ind,    \
		    ws);                                                \
	}
#else
#ifdef THREE_CHAN

#define	CALC_SRC_PTR()                                          \
	xSrc = (x >> x_shift);                                  \
	ySrc = (y >> y_shift);                                  \
	srcPixelPtr = (DTYPE *) lineAddr[ySrc] + 3 * xSrc + k

#define	CALL_FUNC(N)                                                 \
	if (kh == 4) {                                               \
	    mlib_ImageAffineTableLine_##N##_3nw_4_4(buff,            \
		filterX + off, filterY + l, lineAddr + l, ws);       \
	} else if (kh == 2) {                                        \
	    if (kw == 4)                                             \
		mlib_ImageAffineTableLine_##N##_3nw_2_4(buff,        \
		    filterX + off, filterY + l, lineAddr + l, ws);   \
	    else if (kw == 3)                                        \
		mlib_ImageAffineTableLine_##N##_3nw_2_3(buff,        \
		    filterX + off, filterY + l, lineAddr + l, ws);   \
	    else if (kw == 2)                                        \
		mlib_ImageAffineTableLine_##N##_3nw_2_2(buff,        \
		    filterX + off, filterY + l, lineAddr + l, ws);   \
	    else if (kw == 1)                                        \
		mlib_ImageAffineTableLine_##N##_3nw_2_1(buff,        \
		    filterX + off, filterY + l, lineAddr + l, ws);   \
	} else                                                       \
/*                                                                   \
 * if (kh == 1)                                                      \
 */                                                                  \
	{                                                            \
	    if (kw == 4)                                             \
		mlib_ImageAffineTableLine_##N##_3nw_1_4(buff,        \
		    filterX + off, filterY + l, lineAddr + l, ws);   \
	    else if (kw == 3)                                        \
		mlib_ImageAffineTableLine_##N##_3nw_1_3(buff,        \
		    filterX + off, filterY + l, lineAddr + l, ws);   \
	    else if (kw == 2)                                        \
		mlib_ImageAffineTableLine_##N##_3nw_1_2(buff,        \
		    filterX + off, filterY + l, lineAddr + l, ws);   \
	    else if (kw == 1)                                        \
		mlib_ImageAffineTableLine_##N##_3nw_1_1(buff,        \
		    filterX + off, filterY + l, lineAddr + l, ws);   \
	}
#else

#define	CALC_SRC_PTR()                                          \
	xSrc = ((x >> x_shift) & x_mask) + k;                   \
	ySrc = (y >> y_shift);                                  \
	srcPixelPtr = (DTYPE *) lineAddr[ySrc] + xSrc

#define	CALL_FUNC(N)                                                 \
	if (kh == 4) {                                               \
	    mlib_ImageAffineTableLine_##N##nw_4_4(buff,              \
		filterX + off, filterY + l, lineAddr + l, ws);       \
	} else if (kh == 2) {                                        \
	    if (kw == 4)                                             \
		mlib_ImageAffineTableLine_##N##nw_2_4(buff,          \
		    filterX + off, filterY + l, lineAddr + l, ws);   \
	    else if (kw == 3)                                        \
		mlib_ImageAffineTableLine_##N##nw_2_3(buff,          \
		    filterX + off, filterY + l, lineAddr + l, ws);   \
	    else if (kw == 2)                                        \
		mlib_ImageAffineTableLine_##N##nw_2_2(buff,          \
		    filterX + off, filterY + l, lineAddr + l, ws);   \
	    else if (kw == 1)                                        \
		mlib_ImageAffineTableLine_##N##nw_2_1(buff,          \
		    filterX + off, filterY + l, lineAddr + l, ws);   \
	} else                                                       \
/*                                                                   \
 * if (kh == 1)                                                      \
 */                                                                  \
	{                                                            \
	    if (kw == 4)                                             \
		mlib_ImageAffineTableLine_##N##nw_1_4(buff,          \
		    filterX + off, filterY + l, lineAddr + l, ws);   \
	    else if (kw == 3)                                        \
		mlib_ImageAffineTableLine_##N##nw_1_3(buff,          \
		    filterX + off, filterY + l, lineAddr + l, ws);   \
	    else if (kw == 2)                                        \
		mlib_ImageAffineTableLine_##N##nw_1_2(buff,          \
		    filterX + off, filterY + l, lineAddr + l, ws);   \
	    else if (kw == 1)                                        \
		mlib_ImageAffineTableLine_##N##nw_1_1(buff,          \
		    filterX + off, filterY + l, lineAddr + l, ws);   \
	}
#endif		   /* THREE_CHAN */
#define	NEXT_LINE(N)	srcPixelPtr += srcStride
#define	GET_SRC(N)	SRC2FP(srcPixelPtr[CHAN##N])
#endif		   /* SRC_EXTEND */

/* *********************************************************** */

#define	MLIB_FILTER(X, filter)                                   \
	filterpos = ((X) >> X##f_shift) & X##f_mask;             \
	fptr = (FILTER_TYPE *) ((mlib_u8 *)filter + filterpos)

/* *********************************************************** */

#define	MAX_KER	4
#define	BUFF_SIZE	20

/* *********************************************************** */

#define	CHAN0	0
#define	CHAN1	nchan
#define	CHAN2	chan2
#define	CHAN3	chan3

/* *********************************************************** */
static
LINE_FUNC(4_4)
{
	DEF_VARS_LINE;
	FP_TYPE xf1, xf2, xf3;
	FP_TYPE yf1, yf2, yf3;
	FP_TYPE c1, c2, c3;

#ifndef SRC_EXTEND
	mlib_s32 srcStride = ws->srcStride / sizeof (DTYPE);
	mlib_s32 nchan = ws->nchan;
	mlib_s32 chan2 = nchan + nchan;
	mlib_s32 chan3 = nchan + chan2;
#endif /* SRC_EXTEND */

#ifndef THREE_CHAN
#ifndef SRC_EXTEND
	mlib_s32 x_mask = ws->x_mask;
#endif /* SRC_EXTEND */
#endif /* THREE_CHAN */

	MLIB_FILTER(y, filterY);
	yf0 = fptr[0];
	yf1 = fptr[1];
	yf2 = fptr[2];
	yf3 = fptr[3];
	MLIB_FILTER(x, filterX);
	xf0 = fptr[0];
	xf1 = fptr[1];
	xf2 = fptr[2];
	xf3 = fptr[3];
	CALC_SRC_PTR();

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < size - 1; i++) {
		x += dx;
		y += dy;

		c0 = GET_SRC(0) * xf0 + GET_SRC(1) * xf1 + GET_SRC(2) * xf2 +
		    GET_SRC(3) * xf3;
		NEXT_LINE(1);
		c1 = GET_SRC(0) * xf0 + GET_SRC(1) * xf1 + GET_SRC(2) * xf2 +
		    GET_SRC(3) * xf3;
		NEXT_LINE(2);
		c2 = GET_SRC(0) * xf0 + GET_SRC(1) * xf1 + GET_SRC(2) * xf2 +
		    GET_SRC(3) * xf3;
		NEXT_LINE(3);
		c3 = GET_SRC(0) * xf0 + GET_SRC(1) * xf1 + GET_SRC(2) * xf2 +
		    GET_SRC(3) * xf3;
		buff[i * b_step] += yf0 * c0 + yf1 * c1 + yf2 * c2 + yf3 * c3;

		MLIB_FILTER(y, filterY);
		yf0 = fptr[0];
		yf1 = fptr[1];
		yf2 = fptr[2];
		yf3 = fptr[3];
		MLIB_FILTER(x, filterX);
		xf0 = fptr[0];
		xf1 = fptr[1];
		xf2 = fptr[2];
		xf3 = fptr[3];
		CALC_SRC_PTR();
	}

	c0 = GET_SRC(0) * xf0 + GET_SRC(1) * xf1 + GET_SRC(2) * xf2 +
	    GET_SRC(3) * xf3;
	NEXT_LINE(1);
	c1 = GET_SRC(0) * xf0 + GET_SRC(1) * xf1 + GET_SRC(2) * xf2 +
	    GET_SRC(3) * xf3;
	NEXT_LINE(2);
	c2 = GET_SRC(0) * xf0 + GET_SRC(1) * xf1 + GET_SRC(2) * xf2 +
	    GET_SRC(3) * xf3;
	NEXT_LINE(3);
	c3 = GET_SRC(0) * xf0 + GET_SRC(1) * xf1 + GET_SRC(2) * xf2 +
	    GET_SRC(3) * xf3;
	buff[i * b_step] += yf0 * c0 + yf1 * c1 + yf2 * c2 + yf3 * c3;
}

/* *********************************************************** */

static
LINE_FUNC(2_4)
{
	DEF_VARS_LINE;
	FP_TYPE xf1, xf2, xf3;
	FP_TYPE yf1;
	FP_TYPE c1;

#ifndef SRC_EXTEND
	mlib_s32 srcStride = ws->srcStride / sizeof (DTYPE);
	mlib_s32 nchan = ws->nchan;
	mlib_s32 chan2 = nchan + nchan;
	mlib_s32 chan3 = nchan + chan2;
#endif /* SRC_EXTEND */

#ifndef THREE_CHAN
#ifndef SRC_EXTEND
	mlib_s32 x_mask = ws->x_mask;
#endif /* SRC_EXTEND */
#endif /* THREE_CHAN */

	MLIB_FILTER(y, filterY);
	yf0 = fptr[0];
	yf1 = fptr[1];
	MLIB_FILTER(x, filterX);
	xf0 = fptr[0];
	xf1 = fptr[1];
	xf2 = fptr[2];
	xf3 = fptr[3];
	CALC_SRC_PTR();

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < size - 1; i++) {
		x += dx;
		y += dy;

		c0 = GET_SRC(0) * xf0 + GET_SRC(1) * xf1 + GET_SRC(2) * xf2 +
		    GET_SRC(3) * xf3;
		NEXT_LINE(1);
		c1 = GET_SRC(0) * xf0 + GET_SRC(1) * xf1 + GET_SRC(2) * xf2 +
		    GET_SRC(3) * xf3;
		buff[i * b_step] += yf0 * c0 + yf1 * c1;

		MLIB_FILTER(y, filterY);
		yf0 = fptr[0];
		yf1 = fptr[1];
		MLIB_FILTER(x, filterX);
		xf0 = fptr[0];
		xf1 = fptr[1];
		xf2 = fptr[2];
		xf3 = fptr[3];
		CALC_SRC_PTR();
	}

	c0 = GET_SRC(0) * xf0 + GET_SRC(1) * xf1 + GET_SRC(2) * xf2 +
	    GET_SRC(3) * xf3;
	NEXT_LINE(1);
	c1 = GET_SRC(0) * xf0 + GET_SRC(1) * xf1 + GET_SRC(2) * xf2 +
	    GET_SRC(3) * xf3;
	buff[i * b_step] += yf0 * c0 + yf1 * c1;
}

/* *********************************************************** */

LINE_FUNC(2_3)
{
	DEF_VARS_LINE;
	FP_TYPE xf1, xf2;
	FP_TYPE yf1;
	FP_TYPE c1;

#ifndef SRC_EXTEND
	mlib_s32 srcStride = ws->srcStride / sizeof (DTYPE);
	mlib_s32 nchan = ws->nchan;
	mlib_s32 chan2 = nchan + nchan;
#endif /* SRC_EXTEND */

#ifndef THREE_CHAN
#ifndef SRC_EXTEND
	mlib_s32 x_mask = ws->x_mask;
#endif /* SRC_EXTEND */
#endif /* THREE_CHAN */

	MLIB_FILTER(y, filterY);
	yf0 = fptr[0];
	yf1 = fptr[1];
	MLIB_FILTER(x, filterX);
	xf0 = fptr[0];
	xf1 = fptr[1];
	xf2 = fptr[2];
	CALC_SRC_PTR();

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < size - 1; i++) {
		x += dx;
		y += dy;

		c0 = GET_SRC(0) * xf0 + GET_SRC(1) * xf1 + GET_SRC(2) * xf2;
		NEXT_LINE(1);
		c1 = GET_SRC(0) * xf0 + GET_SRC(1) * xf1 + GET_SRC(2) * xf2;
		buff[i * b_step] += yf0 * c0 + yf1 * c1;

		MLIB_FILTER(y, filterY);
		yf0 = fptr[0];
		yf1 = fptr[1];
		MLIB_FILTER(x, filterX);
		xf0 = fptr[0];
		xf1 = fptr[1];
		xf2 = fptr[2];
		CALC_SRC_PTR();
	}

	c0 = GET_SRC(0) * xf0 + GET_SRC(1) * xf1 + GET_SRC(2) * xf2;
	NEXT_LINE(1);
	c1 = GET_SRC(0) * xf0 + GET_SRC(1) * xf1 + GET_SRC(2) * xf2;
	buff[i * b_step] += yf0 * c0 + yf1 * c1;
}

/* *********************************************************** */

static
LINE_FUNC(2_2)
{
	DEF_VARS_LINE;
	FP_TYPE xf1;
	FP_TYPE yf1;
	FP_TYPE c1;

#ifndef SRC_EXTEND
	mlib_s32 srcStride = ws->srcStride / sizeof (DTYPE);
	mlib_s32 nchan = ws->nchan;
#endif /* SRC_EXTEND */

#ifndef THREE_CHAN
#ifndef SRC_EXTEND
	mlib_s32 x_mask = ws->x_mask;
#endif /* SRC_EXTEND */
#endif /* THREE_CHAN */

	MLIB_FILTER(y, filterY);
	yf0 = fptr[0];
	yf1 = fptr[1];
	MLIB_FILTER(x, filterX);
	xf0 = fptr[0];
	xf1 = fptr[1];
	CALC_SRC_PTR();

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < size - 1; i++) {
		x += dx;
		y += dy;

		c0 = GET_SRC(0) * xf0 + GET_SRC(1) * xf1;
		NEXT_LINE(1);
		c1 = GET_SRC(0) * xf0 + GET_SRC(1) * xf1;
		buff[i * b_step] += yf0 * c0 + yf1 * c1;

		MLIB_FILTER(y, filterY);
		yf0 = fptr[0];
		yf1 = fptr[1];
		MLIB_FILTER(x, filterX);
		xf0 = fptr[0];
		xf1 = fptr[1];
		CALC_SRC_PTR();
	}

	c0 = GET_SRC(0) * xf0 + GET_SRC(1) * xf1;
	NEXT_LINE(1);
	c1 = GET_SRC(0) * xf0 + GET_SRC(1) * xf1;
	buff[i * b_step] += yf0 * c0 + yf1 * c1;
}

/* *********************************************************** */

static
LINE_FUNC(2_1)
{
	DEF_VARS_LINE;
	FP_TYPE yf1;
	FP_TYPE c1;

#ifndef SRC_EXTEND
	mlib_s32 srcStride = ws->srcStride / sizeof (DTYPE);
#endif /* SRC_EXTEND */

#ifndef THREE_CHAN
#ifndef SRC_EXTEND
	mlib_s32 x_mask = ws->x_mask;
#endif /* SRC_EXTEND */
#endif /* THREE_CHAN */

	MLIB_FILTER(y, filterY);
	yf0 = fptr[0];
	yf1 = fptr[1];
	MLIB_FILTER(x, filterX);
	xf0 = fptr[0];
	CALC_SRC_PTR();

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < size - 1; i++) {
		x += dx;
		y += dy;

		c0 = GET_SRC(0) * xf0;
		NEXT_LINE(1);
		c1 = GET_SRC(0) * xf0;
		buff[i * b_step] += yf0 * c0 + yf1 * c1;

		MLIB_FILTER(y, filterY);
		yf0 = fptr[0];
		yf1 = fptr[1];
		MLIB_FILTER(x, filterX);
		xf0 = fptr[0];
		CALC_SRC_PTR();
	}

	c0 = GET_SRC(0) * xf0;
	NEXT_LINE(1);
	c1 = GET_SRC(0) * xf0;
	buff[i * b_step] += yf0 * c0 + yf1 * c1;
}

/* *********************************************************** */
static
LINE_FUNC(1_4)
{
	DEF_VARS_LINE;
	FP_TYPE xf1, xf2, xf3;

#ifndef SRC_EXTEND
	mlib_s32 nchan = ws->nchan;
	mlib_s32 chan2 = nchan + nchan;
	mlib_s32 chan3 = nchan + chan2;
#endif /* SRC_EXTEND */

#ifndef THREE_CHAN
#ifndef SRC_EXTEND
	mlib_s32 x_mask = ws->x_mask;
#endif /* SRC_EXTEND */
#endif /* THREE_CHAN */

	MLIB_FILTER(y, filterY);
	yf0 = fptr[0];
	MLIB_FILTER(x, filterX);
	xf0 = fptr[0];
	xf1 = fptr[1];
	xf2 = fptr[2];
	xf3 = fptr[3];
	CALC_SRC_PTR();

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < size - 1; i++) {
		x += dx;
		y += dy;

		c0 = GET_SRC(0) * xf0 + GET_SRC(1) * xf1 + GET_SRC(2) * xf2 +
		    GET_SRC(3) * xf3;
		buff[i * b_step] += yf0 * c0;

		MLIB_FILTER(y, filterY);
		yf0 = fptr[0];
		MLIB_FILTER(x, filterX);
		xf0 = fptr[0];
		xf1 = fptr[1];
		xf2 = fptr[2];
		xf3 = fptr[3];
		CALC_SRC_PTR();
	}

	c0 = GET_SRC(0) * xf0 + GET_SRC(1) * xf1 + GET_SRC(2) * xf2 +
	    GET_SRC(3) * xf3;
	buff[i * b_step] += yf0 * c0;
}

/* *********************************************************** */

static
LINE_FUNC(1_3)
{
	DEF_VARS_LINE;
	FP_TYPE xf1, xf2;

#ifndef SRC_EXTEND
	mlib_s32 nchan = ws->nchan;
	mlib_s32 chan2 = nchan + nchan;
#endif /* SRC_EXTEND */

#ifndef THREE_CHAN
#ifndef SRC_EXTEND
	mlib_s32 x_mask = ws->x_mask;
#endif /* SRC_EXTEND */
#endif /* THREE_CHAN */

	MLIB_FILTER(y, filterY);
	yf0 = fptr[0];
	MLIB_FILTER(x, filterX);
	xf0 = fptr[0];
	xf1 = fptr[1];
	xf2 = fptr[2];
	CALC_SRC_PTR();

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < size - 1; i++) {
		x += dx;
		y += dy;

		c0 = GET_SRC(0) * xf0 + GET_SRC(1) * xf1 + GET_SRC(2) * xf2;
		buff[i * b_step] += yf0 * c0;

		MLIB_FILTER(y, filterY);
		yf0 = fptr[0];
		MLIB_FILTER(x, filterX);
		xf0 = fptr[0];
		xf1 = fptr[1];
		xf2 = fptr[2];
		CALC_SRC_PTR();
	}

	c0 = GET_SRC(0) * xf0 + GET_SRC(1) * xf1 + GET_SRC(2) * xf2;
	buff[i * b_step] += yf0 * c0;
}

/* *********************************************************** */

static
LINE_FUNC(1_2)
{
	DEF_VARS_LINE;
	FP_TYPE xf1;

#ifndef SRC_EXTEND
	mlib_s32 nchan = ws->nchan;
#endif /* SRC_EXTEND */

#ifndef THREE_CHAN
#ifndef SRC_EXTEND
	mlib_s32 x_mask = ws->x_mask;
#endif /* SRC_EXTEND */
#endif /* THREE_CHAN */

	MLIB_FILTER(y, filterY);
	yf0 = fptr[0];
	MLIB_FILTER(x, filterX);
	xf0 = fptr[0];
	xf1 = fptr[1];
	CALC_SRC_PTR();

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < size - 1; i++) {
		x += dx;
		y += dy;

		c0 = GET_SRC(0) * xf0 + GET_SRC(1) * xf1;
		buff[i * b_step] += yf0 * c0;

		MLIB_FILTER(y, filterY);
		yf0 = fptr[0];
		MLIB_FILTER(x, filterX);
		xf0 = fptr[0];
		xf1 = fptr[1];
		CALC_SRC_PTR();
	}

	c0 = GET_SRC(0) * xf0 + GET_SRC(1) * xf1;
	buff[i * b_step] += yf0 * c0;
}

/* *********************************************************** */

static
LINE_FUNC(1_1)
{
	DEF_VARS_LINE;

#ifndef THREE_CHAN
#ifndef SRC_EXTEND
	mlib_s32 x_mask = ws->x_mask;
#endif /* SRC_EXTEND */
#endif /* THREE_CHAN */

	MLIB_FILTER(y, filterY);
	yf0 = fptr[0];
	MLIB_FILTER(x, filterX);
	xf0 = fptr[0];
	CALC_SRC_PTR();

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < size - 1; i++) {
		x += dx;
		y += dy;

		c0 = GET_SRC(0) * xf0;
		buff[i * b_step] += yf0 * c0;

		MLIB_FILTER(y, filterY);
		yf0 = fptr[0];
		MLIB_FILTER(x, filterX);
		xf0 = fptr[0];
		CALC_SRC_PTR();
	}

	c0 = GET_SRC(0) * xf0;
	buff[i * b_step] += yf0 * c0;
}

/* *********************************************************** */

#ifdef __cplusplus
}
#endif

#endif /* _MLIB_IMAGEAFFINETABLE_FUNC_H */
