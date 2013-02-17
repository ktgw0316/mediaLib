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
 * Copyright 2007 Sun Microsystems, Inc.  All rights reserved.
 * Use is subject to license terms.
 */

#pragma ident	"@(#)mlib_ImageAffineEdge.c	9.4	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_ImageAffineEdgeZero - implementation of MLIB_EDGE_DST_FILL_ZERO
 *                                 edge condition
 *      mlib_ImageAffineEdgeNearest - implementation of MLIB_EDGE_OP_NEAREST
 *                                    edge condition
 *      void mlib_ImageAffineEdgeExtend_BL - implementation of
 *                                           MLIB_EDGE_SRC_EXTEND edge condition
 *                                           for MLIB_BILINEAR filter
 *      void mlib_ImageAffineEdgeExtend_BC - implementation of
 *                                           MLIB_EDGE_SRC_EXTEND edge condition
 *                                           for MLIB_BICUBIC filter
 *      void mlib_ImageAffineEdgeExtend_BC2 - implementation of
 *                                           MLIB_EDGE_SRC_EXTEND edge condition
 *                                           for MLIB_BICUBIC2 filter
 *
 * DESCRIPTION
 *      mlib_ImageAffineEdgeZero:
 *         This function fills the edge pixels (i.e. those which can not
 *         be interpolated with given resampling filter because their prototypes
 *         in the source image lie too close to the border) in the destination
 *         image with zeroes.
 *
 *      mlib_ImageAffineEdgeNearest:
 *         This function fills the edge pixels (i.e. those which can not
 *         be interpolated with given resampling filter because their prototypes
 *         in the source image lie too close to the border) in the destination
 *         image according to the nearest neighbour interpolation.
 *
 *      mlib_ImageAffineEdgeExtend_BL:
 *         This function fills the edge pixels (i.e. those which can not
 *         be interpolated with given resampling filter because their prototypes
 *         in the source image lie too close to the border) in the destination
 *         image according to the bilinear interpolation with border pixels
 *         extend of source image.
 *
 *      mlib_ImageAffineEdgeExtend_BC:
 *         This function fills the edge pixels (i.e. those which can not
 *         be interpolated with given resampling filter because their prototypes
 *         in the source image lie too close to the border) in the destination
 *         image according to the bicubic interpolation with border pixels
 *         extend of source image.
 *
 *      mlib_ImageAffineEdgeExtend_BC2:
 *         This function fills the edge pixels (i.e. those which can not
 *         be interpolated with given resampling filter because their prototypes
 *         in the source image lie too close to the border) in the destination
 *         image according to the bicubic2 interpolation with border pixels
 *         extend of source image.
 */

#ifndef	_NO_LONGLONG

#include <mlib_image.h>
#include <mlib_ImageColormap.h>
#include <mlib_ImageAffine.h>
#include <mlib_GeomBasicIntOp.h>

/* *********************************************************** */

#undef	SHIFT
#define	SHIFT	16
#define	DTOLL(l, d)	BASIC_DTOLL(l, d, SHIFT)

#undef	FLT_SHIFT
#define	FLT_SHIFT	7
#define	FTOLL(l, f)	BASIC_FTOLL(l, f, FLT_SHIFT)

/* *********************************************************** */

#define	FLT_SHIFT_U8	4
#define	FLT_MASK_U8	(((1 << 8) - 1) << 4)

#define	FLT_SHIFT_S16	3

#define	FLT_MASK_S16	(((1 << 9) - 1) << 4)

#define	MLIB_SIGN_SHIFT	31

/* *********************************************************** */

/* #define	D64mlib_u8(X)	mlib_U82D64[X] */
#define	D64mlib_u8(X)	((mlib_s64)(X))

/* #define	D64mlib_s16(X)	((mlib_d64)(X)) */
#define	D64mlib_s16(X)	((mlib_s64)(X))

/* #define	D64mlib_u16(X)	((mlib_d64)(X)) */
#define	D64mlib_u16(X)	((mlib_s64)(X))

/* #define	D64mlib_s32(X)	((mlib_d64)(X)) */
#define	D64mlib_s32(X)	((mlib_s64)(X))

#define	D64mlib_f32(X)	((mlib_d64)(X))

#define	D64mlib_d64(X)	((mlib_d64)(X))

/* *********************************************************** */

#define	SATmlib_u8(DST, val0)                                   \
	val0 -= sat;                                            \
	if (val0 >= MLIB_S32_MAX)                               \
	    val0 = MLIB_S32_MAX;                                \
	if (val0 <= MLIB_S32_MIN)                               \
	    val0 = MLIB_S32_MIN;                                \
	DST = ((mlib_s32)val0 >> 24) ^ 0x80;

/* *********************************************************** */

#define	SAT_int_mlib_u8(DST, val0)                              \
	val0 -= sat_int;                                        \
	if (val0 >= MLIB_S32_MAX)                               \
	    val0 = MLIB_S32_MAX;                                \
	if (val0 <= MLIB_S32_MIN)                               \
	    val0 = MLIB_S32_MIN;                                \
	DST = ((mlib_s32)val0 >> 24) ^ 0x80;

/* *********************************************************** */

#define	SATmlib_s16(DST, val0)                                  \
	if (val0 >= MLIB_S32_MAX)                               \
	    val0 = MLIB_S32_MAX;                                \
	if (val0 <= MLIB_S32_MIN)                               \
	    val0 = MLIB_S32_MIN;                                \
	DST = (mlib_s32)val0 >> 16;

/* *********************************************************** */

#define	SAT_int_mlib_s16(DST, val0)                             \
	if (val0 >= MLIB_S32_MAX)                               \
	    val0 = MLIB_S32_MAX;                                \
	if (val0 <= MLIB_S32_MIN)                               \
	    val0 = MLIB_S32_MIN;                                \
	DST = (mlib_s32)val0 >> 16;

/* *********************************************************** */

#define	SATmlib_u16(DST, val0)                                  \
	val0 -= sat;                                            \
	if (val0 >= MLIB_S32_MAX)                               \
	    val0 = MLIB_S32_MAX;                                \
	if (val0 <= MLIB_S32_MIN)                               \
	    val0 = MLIB_S32_MIN;                                \
	DST = ((mlib_s32)val0 >> 16) ^ 0x8000;

/* *********************************************************** */

#define	SAT_int_mlib_u16(DST, val0)                             \
	val0 -= sat_int;                                        \
	if (val0 >= MLIB_S32_MAX)                               \
	    val0 = MLIB_S32_MAX;                                \
	if (val0 <= MLIB_S32_MIN)                               \
	    val0 = MLIB_S32_MIN;                                \
	DST = ((mlib_s32)val0 >> 16) ^ 0x8000;

/* *********************************************************** */

#define	SATmlib_s32(DST, val0)                                  \
	if (val0 >= MLIB_S32_MAX)                               \
	    val0 = MLIB_S32_MAX;                                \
	if (val0 <= MLIB_S32_MIN)                               \
	    val0 = MLIB_S32_MIN;                                \
	DST = (mlib_s32)val0;

/* *********************************************************** */

#define	SATmlib_f32(DST, val0)                                  \
	DST = (mlib_f32)val0;

/* *********************************************************** */

#define	SATmlib_d64(DST, val0)                                  \
	DST = val0;

/* *********************************************************** */
#define	MLIB_EDGE_ZERO_LINE(TYPE, Left, Right)                  \
	dp = (TYPE *) data + channels * Left;                   \
	dstLineEnd = (TYPE *) data + channels * Right;          \
	for (; dp < dstLineEnd; dp++) {                         \
	    *dp = zero;                                         \
	}

/* *********************************************************** */

#define	MLIB_EDGE_NEAREST_LINE(TYPE, Left, Right)               \
	dp = (TYPE *) data + channels * Left;                   \
	size = Right - Left;                                    \
	for (j = 0; j < size; j++) {                            \
	    ySrc = Y >> MLIB_SHIFT;                             \
	    xSrc = X >> MLIB_SHIFT;                             \
	    sp = (TYPE *) lineAddr[ySrc] + xSrc * channels;     \
	    for (k = 0; k < channels; k++)                      \
		dp[k] = sp[k];                                  \
	    Y += dY;                                            \
	    X += dX;                                            \
	    dp += channels;                                     \
	}

/* *********************************************************** */

#define	MLIB_EDGE_BL(TYPE, Left, Right)                                    \
	dp = (TYPE *) data + channels * Left;                              \
	size = Right - Left;                                               \
	for (j = 0; j < size; j++) {                                       \
	    ySrc = ((Y - 32768) >> MLIB_SHIFT);                            \
	    xSrc = ((X - 32768) >> MLIB_SHIFT);                            \
	    t = ((X - 32768) & MLIB_MASK);                                 \
	    u = ((Y - 32768) & MLIB_MASK);                                 \
	    mlib_s64 t_1 = (1 << MLIB_SHIFT) - t;                          \
	    mlib_s64 u_1 = (1 << MLIB_SHIFT) - u;                          \
	    xDelta = (((xSrc + 1 -                                         \
		    srcWidth)) >> MLIB_SIGN_SHIFT) & channels;             \
	    yDelta = (((ySrc + 1 -                                         \
		    srcHeight)) >> MLIB_SIGN_SHIFT) & srcStride;           \
	    xFlag = (xSrc >> (MLIB_SIGN_SHIFT - MLIB_SHIFT));              \
	    xSrc = xSrc + (1 & xFlag);                                     \
	    xDelta = xDelta & ~xFlag;                                      \
	    yFlag = (ySrc >> (MLIB_SIGN_SHIFT - MLIB_SHIFT));              \
	    ySrc = ySrc + (1 & yFlag);                                     \
	    yDelta = yDelta & ~yFlag;                                      \
	    sp = (TYPE *) lineAddr[ySrc] + xSrc * channels;                \
	    for (k = 0; k < channels; k++) {                               \
		a00 = D64##TYPE(sp[0]);                                    \
		a01 = D64##TYPE(sp[xDelta]);                               \
		a10 = D64##TYPE(sp[yDelta]);                               \
		a11 = D64##TYPE(sp[yDelta + xDelta]);                      \
		pix0 = (a00 * t_1 + a01 * t) * u_1 +                       \
		    (a10 * t_1 + a11 * t) * u;                             \
		dp[k] = (TYPE) (pix0>>(MLIB_SHIFT+MLIB_SHIFT));            \
		sp++;                                                      \
	    }                                                              \
	    X += dX;                                                       \
	    Y += dY;                                                       \
	    dp += channels;                                                \
	}

/* *********************************************************** */

#define	LUT(k, ind)	plut[channels*sp[ind] + k]
#define	LUT_INT(l, k, ind)	DTOLL(l, (plut[channels*sp[ind] + k]))

/* *********************************************************** */

#define	MLIB_EDGE_INDEX(ITYPE, DTYPE, size)                                \
	for (j = 0; j < size; j++) {                                       \
	    ySrc = ((Y - 32768) >> MLIB_SHIFT);                            \
	    xSrc = ((X - 32768) >> MLIB_SHIFT);                            \
	    t = ((X - 32768) & MLIB_MASK);                                 \
	    u = ((Y - 32768) & MLIB_MASK);                                 \
	    mlib_s64 t_1 = (1 << MLIB_SHIFT) - t;                          \
	    mlib_s64 u_1 = (1 << MLIB_SHIFT) - u;                          \
	    xDelta = (((xSrc + 1 - srcWidth)) >> MLIB_SIGN_SHIFT) & 1;     \
	    yDelta = (((ySrc + 1 -                                         \
		    srcHeight)) >> MLIB_SIGN_SHIFT) & srcStride;           \
	    xFlag = (xSrc >> (MLIB_SIGN_SHIFT - MLIB_SHIFT));              \
	    xSrc = xSrc + (1 & xFlag);                                     \
	    xDelta = xDelta & ~xFlag;                                      \
	    yFlag = (ySrc >> (MLIB_SIGN_SHIFT - MLIB_SHIFT));              \
	    ySrc = ySrc + (1 & yFlag);                                     \
	    yDelta = yDelta & ~yFlag;                                      \
	    sp = (ITYPE *) lineAddr[ySrc] + xSrc;                          \
	    for (k = 0; k < channels; k++) {                               \
		LUT_INT(a00, k, 0);                                        \
		LUT_INT(a01, k, xDelta);                                   \
		LUT_INT(a10, k, yDelta);                                   \
		LUT_INT(a11, k, yDelta + xDelta);                          \
		pix0 = (a00 * t_1 + a01 * t) * u_1 +                       \
		    (a10 * t_1 + a11 * t) * u;                             \
		pbuff[k] = (mlib_s32)(pix0>>(SHIFT+MLIB_SHIFT+MLIB_SHIFT));\
	    } pbuff += channels;                                           \
	    X += dX;                                                       \
	    Y += dY;                                                       \
	}

/* *********************************************************** */

#define	MLIB_EDGE_INDEX_u8i(ITYPE, Left, Right)                    \
	{                                                          \
	    mlib_u8 *pbuff = buff;                                 \
	                                                           \
	    size = Right - Left;                                   \
	    MLIB_EDGE_INDEX(ITYPE, mlib_u8,                        \
		    size);                                         \
	                                                           \
	    dp = (ITYPE *) data + Left;                            \
	    if (channels == 3) {                                   \
		if (sizeof (ITYPE) == 1) {                         \
		    mlib_ImageColorTrue2IndexLine_U8_U8_3(buff,    \
			    (void *)dp, size, colormap);           \
		} else {                                           \
		    mlib_ImageColorTrue2IndexLine_U8_S16_3(buff,   \
			    (void *)dp, size, colormap);           \
		}                                                  \
	    } else {                                               \
		if (sizeof (ITYPE) == 1) {                         \
		    mlib_ImageColorTrue2IndexLine_U8_U8_4(buff,    \
			    (void *)dp, size, colormap);           \
		} else {                                           \
		    mlib_ImageColorTrue2IndexLine_U8_S16_4(buff,   \
			    (void *)dp, size, colormap);           \
		}                                                  \
	    }                                                      \
	}

/* *********************************************************** */

#define	MLIB_EDGE_INDEX_u8i_BC(ITYPE, Left, Right)                 \
	{                                                          \
	    mlib_u8 *pbuff = buff;                                 \
	                                                           \
	    size = Right - Left;                                   \
	    MLIB_EDGE_INDEX(ITYPE, mlib_u8,                        \
		    size, 2);                                      \
	                                                           \
	    dp = (ITYPE *) data + Left;                            \
	    if (channels == 3) {                                   \
		if (sizeof (ITYPE) == 1) {                         \
		    mlib_ImageColorTrue2IndexLine_U8_U8_3(buff,    \
			    (void *)dp, size, colormap);           \
		} else {                                           \
		    mlib_ImageColorTrue2IndexLine_U8_S16_3(buff,   \
			    (void *)dp, size, colormap);           \
		}                                                  \
	    } else {                                               \
		if (sizeof (ITYPE) == 1) {                         \
		    mlib_ImageColorTrue2IndexLine_U8_U8_4(buff,    \
			    (void *)dp, size, colormap);           \
		} else {                                           \
		    mlib_ImageColorTrue2IndexLine_U8_S16_4(buff,   \
			    (void *)dp, size, colormap);           \
		}                                                  \
	    }                                                      \
	}

/* *********************************************************** */

#define	MLIB_EDGE_INDEX_s16i(ITYPE, Left, Right)                    \
	{                                                           \
	    mlib_s16 *pbuff = buff;                                 \
	                                                            \
	    size = Right - Left;                                    \
	    MLIB_EDGE_INDEX(ITYPE, mlib_s16,                        \
		    size);                                          \
	                                                            \
	    dp = (ITYPE *) data + Left;                             \
	    if (channels == 3) {                                    \
		if (sizeof (ITYPE) == 1) {                          \
		    mlib_ImageColorTrue2IndexLine_S16_U8_3(buff,    \
			    (void *)dp, size, colormap);            \
		} else {                                            \
		    mlib_ImageColorTrue2IndexLine_S16_S16_3(buff,   \
			    (void *)dp, size, colormap);            \
		}                                                   \
	    } else {                                                \
		if (sizeof (ITYPE) == 1) {                          \
		    mlib_ImageColorTrue2IndexLine_S16_U8_4(buff,    \
			    (void *)dp, size, colormap);            \
		} else {                                            \
		    mlib_ImageColorTrue2IndexLine_S16_S16_4(buff,   \
			    (void *)dp, size, colormap);            \
		}                                                   \
	    }                                                       \
	}

/* *********************************************************** */

#define	MLIB_EDGE_INDEX_s16i_BC(ITYPE, Left, Right)                 \
	{                                                           \
	    mlib_s16 *pbuff = buff;                                 \
	                                                            \
	    size = Right - Left;                                    \
	    MLIB_EDGE_INDEX(ITYPE, mlib_s16,                        \
		    size, 7);                                       \
	                                                            \
	    dp = (ITYPE *) data + Left;                             \
	    if (channels == 3) {                                    \
		if (sizeof (ITYPE) == 1) {                          \
		    mlib_ImageColorTrue2IndexLine_S16_U8_3(buff,    \
			    (void *)dp, size, colormap);            \
		} else {                                            \
		    mlib_ImageColorTrue2IndexLine_S16_S16_3(buff,   \
			    (void *)dp, size, colormap);            \
		}                                                   \
	    } else {                                                \
		if (sizeof (ITYPE) == 1) {                          \
		    mlib_ImageColorTrue2IndexLine_S16_U8_4(buff,    \
			    (void *)dp, size, colormap);            \
		} else {                                            \
		    mlib_ImageColorTrue2IndexLine_S16_S16_4(buff,   \
			    (void *)dp, size, colormap);            \
		}                                                   \
	    }                                                       \
	}

/* *********************************************************** */

#define	GET_FLT_TBL(X, xf0, xf1, xf2, xf3)                      \
	filterpos = ((X - 32768) >> flt_shift) & flt_mask;      \
	fptr = (mlib_f32 *)((mlib_u8 *)flt_tbl + filterpos);    \
	xf0 = fptr[0];                                          \
	xf1 = fptr[1];                                          \
	xf2 = fptr[2];                                          \
	xf3 = fptr[3];

/* *********************************************************** */

#if 0
#define	GET_FLT_TBL_INT(X, xf0, xf1, xf2, xf3)                  \
	filterpos = ((X - 32768) >> flt_shift) & flt_mask;      \
	fptr = (mlib_f32 *)((mlib_u8 *)flt_tbl + filterpos);    \
	xf0 = FTOLL(fptr[0]);                                   \
	xf1 = FTOLL(fptr[1]);                                   \
	xf2 = FTOLL(fptr[2]);                                   \
	xf3 = FTOLL(fptr[3]);
#endif

/* *********************************************************** */

#define	GET_FLT_TBL_INT(X, xf0, xf1, xf2, xf3)                  \
	filterpos = ((X - 32768) >> flt_shift) & flt_mask;      \
	fptr_int = (mlib_s16 *)((mlib_u8 *)flt_tbl_int + (filterpos>>1));    \
	xf0 = (fptr_int[0]);                                    \
	xf1 = (fptr_int[1]);                                    \
	xf2 = (fptr_int[2]);                                    \
	xf3 = (fptr_int[3]);

/* *********************************************************** */

#define	GET_FLT_BC(X, xf0, xf1, xf2, xf3)                       \
	dx = ((X - 32768) & MLIB_MASK) * scale;                 \
	dx_2 = 0.5 * dx;                                        \
	dx2 = dx * dx;                                          \
	dx3_2 = dx_2 * dx2;                                     \
	dx3_3 = 3.0 * dx3_2;                                    \
	xf0 = dx2 - dx3_2 - dx_2;                               \
	xf1 = dx3_3 - 2.5 * dx2 + 1.0;                          \
	xf2 = 2.0 * dx2 - dx3_3 + dx_2;                         \
	xf3 = dx3_2 - 0.5 * dx2;

/* *********************************************************** */

#define	GET_FLT_BC2(X, xf0, xf1, xf2, xf3)                      \
	dx = ((X - 32768) & MLIB_MASK) * scale;                 \
	dx2 = dx * dx;                                          \
	dx3_2 = dx * dx2;                                       \
	dx3_3 = 2.0 * dx2;                                      \
	xf0 = -dx3_2 + dx3_3 - dx;                              \
	xf1 = dx3_2 - dx3_3 + 1.0;                              \
	xf2 = -dx3_2 + dx2 + dx;                                \
	xf3 = dx3_2 - dx2;

/* *********************************************************** */

#define	CALC_SRC_POS(X, Y, channels, srcStride)                            \
	xSrc = ((X - 32768) >> MLIB_SHIFT);                                \
	ySrc = ((Y - 32768) >> MLIB_SHIFT);                                \
	xDelta0 = ((~((xSrc - 1) >> MLIB_SIGN_SHIFT)) & (-channels));      \
	yDelta0 = ((~((ySrc - 1) >> MLIB_SIGN_SHIFT)) & (-srcStride));     \
	xDelta1 =                                                          \
		((xSrc + 1 - srcWidth) >> MLIB_SIGN_SHIFT) & (channels);   \
	yDelta1 =                                                          \
		((ySrc + 1 -                                               \
		srcHeight) >> MLIB_SIGN_SHIFT) & (srcStride);              \
	xDelta2 =                                                          \
		xDelta1 + (((xSrc + 2 -                                    \
		srcWidth) >> MLIB_SIGN_SHIFT) & (channels));               \
	yDelta2 =                                                          \
		yDelta1 + (((ySrc + 2 -                                    \
		srcHeight) >> MLIB_SIGN_SHIFT) & (srcStride));             \
	xFlag = (xSrc >> (MLIB_SIGN_SHIFT - MLIB_SHIFT));                  \
	xSrc = xSrc + (1 & xFlag);                                         \
	xDelta2 -= (xDelta1 & xFlag);                                      \
	xDelta1 = (xDelta1 & ~xFlag);                                      \
	yFlag = (ySrc >> (MLIB_SIGN_SHIFT - MLIB_SHIFT));                  \
	ySrc = ySrc + (1 & yFlag);                                         \
	yDelta2 -= (yDelta1 & yFlag);                                      \
	yDelta1 = yDelta1 & ~yFlag;

/* *********************************************************** */

#define	MLIB_EDGE_BC_LINE(TYPE, Left, Right, GET_FILTER)            \
	dp = (TYPE *) data + channels * Left;                       \
	size = Right - Left;                                        \
	for (j = 0; j < size; j++) {                                \
	    GET_FILTER(X, xf0, xf1, xf2, xf3);                      \
	    GET_FILTER(Y, yf0, yf1, yf2, yf3);                      \
	    CALC_SRC_POS(X, Y, channels, srcStride);                \
	    sp = (TYPE *) lineAddr[ySrc] + channels * xSrc;         \
	    for (k = 0; k < channels; k++) {                        \
		c0 = D64##TYPE(sp[yDelta0 + xDelta0]) * xf0 +       \
			D64##TYPE(sp[yDelta0]) * xf1 +              \
			D64##TYPE(sp[yDelta0 + xDelta1]) * xf2 +    \
			D64##TYPE(sp[yDelta0 + xDelta2]) * xf3;     \
		c1 = D64##TYPE(sp[xDelta0]) * xf0 +                 \
			D64##TYPE(sp[0]) * xf1 +                    \
			D64##TYPE(sp[xDelta1]) * xf2 +              \
			D64##TYPE(sp[xDelta2]) * xf3;               \
		c2 = D64##TYPE(sp[yDelta1 + xDelta0]) * xf0 +       \
			D64##TYPE(sp[yDelta1]) * xf1 +              \
			D64##TYPE(sp[yDelta1 + xDelta1]) * xf2 +    \
			D64##TYPE(sp[yDelta1 + xDelta2]) * xf3;     \
		c3 = D64##TYPE(sp[yDelta2 + xDelta0]) * xf0 +       \
			D64##TYPE(sp[yDelta2]) * xf1 +              \
			D64##TYPE(sp[yDelta2 + xDelta1]) * xf2 +    \
			D64##TYPE(sp[yDelta2 + xDelta2]) * xf3;     \
		val0 = c0 * yf0 + c1 * yf1 + c2 * yf2 + c3 * yf3;   \
		SAT##TYPE(dp[k], val0);                             \
		sp++;                                               \
	    }                                                       \
	    X += dX;                                                \
	    Y += dY;                                                \
	    dp += channels;                                         \
	}

/* *********************************************************** */

#define	MLIB_EDGE_BC_LINE_INT_U8(TYPE, Left, Right, GET_FILTER)     \
	dp = (TYPE *) data + channels * Left;                       \
	size = Right - Left;                                        \
	for (j = 0; j < size; j++) {                                \
	    GET_FILTER(X, xf0_int, xf1_int, xf2_int, xf3_int);      \
	    GET_FILTER(Y, yf0_int, yf1_int, yf2_int, yf3_int);      \
	    CALC_SRC_POS(X, Y, channels, srcStride);                \
	    sp = (TYPE *) lineAddr[ySrc] + channels * xSrc;         \
	    for (k = 0; k < channels; k++) {                        \
		c0_int = D64##TYPE(sp[yDelta0 + xDelta0]) * xf0_int +   \
			D64##TYPE(sp[yDelta0]) * xf1_int +              \
			D64##TYPE(sp[yDelta0 + xDelta1]) * xf2_int +    \
			D64##TYPE(sp[yDelta0 + xDelta2]) * xf3_int;     \
		c1_int = D64##TYPE(sp[xDelta0]) * xf0_int +             \
			D64##TYPE(sp[0]) * xf1_int +                    \
			D64##TYPE(sp[xDelta1]) * xf2_int +              \
			D64##TYPE(sp[xDelta2]) * xf3_int;               \
		c2_int = D64##TYPE(sp[yDelta1 + xDelta0]) * xf0_int +   \
			D64##TYPE(sp[yDelta1]) * xf1_int +              \
			D64##TYPE(sp[yDelta1 + xDelta1]) * xf2_int +    \
			D64##TYPE(sp[yDelta1 + xDelta2]) * xf3_int;     \
		c3_int = D64##TYPE(sp[yDelta2 + xDelta0]) * xf0_int +   \
			D64##TYPE(sp[yDelta2]) * xf1_int +              \
			D64##TYPE(sp[yDelta2 + xDelta1]) * xf2_int +    \
			D64##TYPE(sp[yDelta2 + xDelta2]) * xf3_int;     \
		val0_int = c0_int * yf0_int + c1_int * yf1_int +        \
			c2_int * yf2_int + c3_int * yf3_int;            \
		val0_int >>= (2+2);                                     \
		SAT_int_##TYPE(dp[k], val0_int);                        \
		sp++;                                               \
	    }                                                       \
	    X += dX;                                                \
	    Y += dY;                                                \
	    dp += channels;                                         \
	}

/* *********************************************************** */

#define	MLIB_EDGE_BC_LINE_INT_16(TYPE, Left, Right, GET_FILTER)     \
	dp = (TYPE *) data + channels * Left;                       \
	size = Right - Left;                                        \
	for (j = 0; j < size; j++) {                                \
	    GET_FILTER(X, xf0_int, xf1_int, xf2_int, xf3_int);      \
	    GET_FILTER(Y, yf0_int, yf1_int, yf2_int, yf3_int);      \
	    CALC_SRC_POS(X, Y, channels, srcStride);                \
	    sp = (TYPE *) lineAddr[ySrc] + channels * xSrc;         \
	    for (k = 0; k < channels; k++) {                        \
		c0_int = D64##TYPE(sp[yDelta0 + xDelta0]) * xf0_int +   \
			D64##TYPE(sp[yDelta0]) * xf1_int +              \
			D64##TYPE(sp[yDelta0 + xDelta1]) * xf2_int +    \
			D64##TYPE(sp[yDelta0 + xDelta2]) * xf3_int;     \
		c1_int = D64##TYPE(sp[xDelta0]) * xf0_int +             \
			D64##TYPE(sp[0]) * xf1_int +                    \
			D64##TYPE(sp[xDelta1]) * xf2_int +              \
			D64##TYPE(sp[xDelta2]) * xf3_int;               \
		c2_int = D64##TYPE(sp[yDelta1 + xDelta0]) * xf0_int +   \
			D64##TYPE(sp[yDelta1]) * xf1_int +              \
			D64##TYPE(sp[yDelta1 + xDelta1]) * xf2_int +    \
			D64##TYPE(sp[yDelta1 + xDelta2]) * xf3_int;     \
		c3_int = D64##TYPE(sp[yDelta2 + xDelta0]) * xf0_int +   \
			D64##TYPE(sp[yDelta2]) * xf1_int +              \
			D64##TYPE(sp[yDelta2 + xDelta1]) * xf2_int +    \
			D64##TYPE(sp[yDelta2 + xDelta2]) * xf3_int;     \
		val0_int = c0_int * yf0_int + c1_int * yf1_int +        \
			c2_int * yf2_int + c3_int * yf3_int;            \
		val0_int >>= (7+7);                                     \
		SAT_int_##TYPE(dp[k], val0_int);                        \
		sp++;                                               \
	    }                                                       \
	    X += dX;                                                \
	    Y += dY;                                                \
	    dp += channels;                                         \
	}

/* *********************************************************** */

#define	MLIB_EDGE_BC_TBL_U8(TYPE, Left, Right)                     \
	MLIB_EDGE_BC_LINE_INT_U8(TYPE, Left, Right, GET_FLT_TBL_INT);

/* *********************************************************** */

#define	MLIB_EDGE_BC_TBL_16(TYPE, Left, Right)                     \
	MLIB_EDGE_BC_LINE_INT_16(TYPE, Left, Right, GET_FLT_TBL_INT);

/* *********************************************************** */

#define	MLIB_EDGE_BC(TYPE, Left, Right)                         \
	MLIB_EDGE_BC_LINE(TYPE, Left, Right, GET_FLT_BC);

/* *********************************************************** */

#define	MLIB_EDGE_BC2(TYPE, Left, Right)                        \
	MLIB_EDGE_BC_LINE(TYPE, Left, Right, GET_FLT_BC2);

/* *********************************************************** */

#define	MLIB_EDGE_INDEX_BC(ITYPE, DTYPE, size)                          \
	for (j = 0; j < size; j++) {                                    \
	    GET_FLT_TBL(X, xf0, xf1, xf2, xf3);                         \
	    GET_FLT_TBL(Y, yf0, yf1, yf2, yf3);                         \
	    CALC_SRC_POS(X, Y, 1, srcStride);                           \
	    sp = (ITYPE *) lineAddr[ySrc] + xSrc;                       \
	    for (k = 0; k < channels; k++) {                            \
		c0 = LUT(k, yDelta0 + xDelta0) * xf0 + LUT(k,           \
			yDelta0) * xf1 + LUT(k,                         \
			yDelta0 + xDelta1) * xf2 + LUT(k,               \
			yDelta0 + xDelta2) * xf3;                       \
		c1 = LUT(k, xDelta0) * xf0 + LUT(k, 0) * xf1 + LUT(k,   \
			xDelta1) * xf2 + LUT(k, xDelta2) * xf3;         \
		c2 = LUT(k, yDelta1 + xDelta0) * xf0 + LUT(k,           \
			yDelta1) * xf1 + LUT(k,                         \
			yDelta1 + xDelta1) * xf2 + LUT(k,               \
			yDelta1 + xDelta2) * xf3;                       \
		c3 = LUT(k, yDelta2 + xDelta0) * xf0 + LUT(k,           \
			yDelta2) * xf1 + LUT(k,                         \
			yDelta2 + xDelta1) * xf2 + LUT(k,               \
			yDelta2 + xDelta2) * xf3;                       \
		val0 = c0 * yf0 + c1 * yf1 + c2 * yf2 + c3 * yf3;       \
		SAT##DTYPE(pbuff[k], val0);                             \
	    }                                                           \
	    pbuff += channels;                                          \
	    X += dX;                                                    \
	    Y += dY;                                                    \
	}

/* *********************************************************** */

#define	MLIB_EDGE_INDEX_BC_INT(ITYPE, DTYPE, size, TBL_SHIFT)           \
	for (j = 0; j < size; j++) {                                    \
	    GET_FLT_TBL_INT(X, xf0_int, xf1_int, xf2_int, xf3_int);     \
	    GET_FLT_TBL_INT(Y, yf0_int, yf1_int, yf2_int, yf3_int);     \
	    CALC_SRC_POS(X, Y, 1, srcStride);                           \
	    sp = (ITYPE *) lineAddr[ySrc] + xSrc;                       \
	    for (k = 0; k < channels; k++) {                            \
		mlib_s64 c00, c01, c02, c03;                            \
		mlib_s64 c10, c11, c12, c13;                            \
		mlib_s64 c20, c21, c22, c23;                            \
		mlib_s64 c30, c31, c32, c33;                            \
		LUT_INT(c00, k, yDelta0 + xDelta0)                      \
		LUT_INT(c01, k, yDelta0)                                \
		LUT_INT(c02, k, yDelta0 + xDelta1)                      \
		LUT_INT(c03, k, yDelta0 + xDelta2)                      \
		c0_int = c00 * xf0_int + c01 * xf1_int +                \
		    c02 * xf2_int + c03 * xf3_int;                      \
		LUT_INT(c10, k, xDelta0)                                \
		LUT_INT(c11, k, 0)                                      \
		LUT_INT(c12, k, xDelta1)                                \
		LUT_INT(c13, k, xDelta2)                                \
		c1_int = c10 * xf0_int + c11 * xf1_int +                \
		    c12 * xf2_int + c13 * xf3_int;                      \
		LUT_INT(c20, k, yDelta1 + xDelta0)                      \
		LUT_INT(c21, k, yDelta1)                                \
		LUT_INT(c22, k, yDelta1 + xDelta1)                      \
		LUT_INT(c23, k, yDelta1 + xDelta2)                      \
		c2_int = c20 * xf0_int + c21 * xf1_int +                \
		    c22 * xf2_int + c23 * xf3_int;                      \
		LUT_INT(c30, k, yDelta2 + xDelta0)                      \
		LUT_INT(c31, k, yDelta2)                                \
		LUT_INT(c32, k, yDelta2 + xDelta1)                      \
		LUT_INT(c33, k, yDelta2 + xDelta2)                      \
		c3_int = c30 * xf0_int + c31 * xf1_int +                \
		    c32 * xf2_int + c33 * xf3_int;                      \
		val0_int = c0_int * yf0_int + c1_int * yf1_int +        \
			c2_int * yf2_int + c3_int * yf3_int;            \
		val0_int >>= (SHIFT+TBL_SHIFT+TBL_SHIFT);               \
		SAT_int_##DTYPE(pbuff[k], val0_int);                    \
	    }                                                           \
	    pbuff += channels;                                          \
	    X += dX;                                                    \
	    Y += dY;                                                    \
	}

/* *********************************************************** */

#define	MLIB_PROCESS_EDGES_ZERO(TYPE)                           \
	{                                                       \
	    TYPE *dp, *dstLineEnd;                              \
	                                                        \
	    for (i = yStartE; i < yStart; i++) {                \
		xLeftE = leftEdgesE[i];                         \
		xRightE = rightEdgesE[i] + 1;                   \
		data += dstStride;                              \
		MLIB_EDGE_ZERO_LINE(TYPE, xLeftE, xRightE);     \
	    }                                                   \
	    for (; i <= yFinish; i++) {                         \
		xLeftE = leftEdgesE[i];                         \
		xRightE = rightEdgesE[i] + 1;                   \
		xLeft = leftEdges[i];                           \
		xRight = rightEdges[i] + 1;                     \
		data += dstStride;                              \
		if (xLeft < xRight) {                           \
		    MLIB_EDGE_ZERO_LINE(TYPE, xLeftE, xLeft);   \
		} else {                                        \
		    xRight = xLeftE;                            \
		}                                               \
		MLIB_EDGE_ZERO_LINE(TYPE, xRight, xRightE);     \
	    }                                                   \
	    for (; i <= yFinishE; i++) {                        \
		xLeftE = leftEdgesE[i];                         \
		xRightE = rightEdgesE[i] + 1;                   \
		data += dstStride;                              \
		MLIB_EDGE_ZERO_LINE(TYPE, xLeftE, xRightE);     \
	    }                                                   \
	}

/* *********************************************************** */

#define	MLIB_PROCESS_EDGES(PROCESS_LINE, TYPE)                  \
	{                                                       \
	    TYPE *sp, *dp;                                      \
	    mlib_s32 k, size;                                   \
	                                                        \
	    for (i = yStartE; i < yStart; i++) {                \
		xLeftE = leftEdgesE[i];                         \
		xRightE = rightEdgesE[i] + 1;                   \
		X = xStartsE[i];                                \
		Y = yStartsE[i];                                \
		data += dstStride;                              \
		PROCESS_LINE(TYPE, xLeftE, xRightE);            \
	    }                                                   \
	    for (; i <= yFinish; i++) {                         \
		xLeftE = leftEdgesE[i];                         \
		xRightE = rightEdgesE[i] + 1;                   \
		xLeft = leftEdges[i];                           \
		xRight = rightEdges[i] + 1;                     \
		X = xStartsE[i];                                \
		Y = yStartsE[i];                                \
		data += dstStride;                              \
		if (xLeft < xRight) {                           \
		    PROCESS_LINE(TYPE, xLeftE, xLeft);          \
		} else {                                        \
		    xRight = xLeftE;                            \
		}                                               \
		X = xStartsE[i] + dX * (xRight - xLeftE);       \
		Y = yStartsE[i] + dY * (xRight - xLeftE);       \
		PROCESS_LINE(TYPE, xRight, xRightE);            \
	    }                                                   \
	    for (; i <= yFinishE; i++) {                        \
		xLeftE = leftEdgesE[i];                         \
		xRightE = rightEdgesE[i] + 1;                   \
		X = xStartsE[i];                                \
		Y = yStartsE[i];                                \
		data += dstStride;                              \
		PROCESS_LINE(TYPE, xLeftE, xRightE);            \
	    }                                                   \
	}

/* *********************************************************** */

#define	GET_EDGE_PARAMS_ZERO()                                  \
	mlib_image *dst = param->dst;                           \
	mlib_s32 *leftEdges = param->leftEdges;                 \
	mlib_s32 *rightEdges = param->rightEdges;               \
	mlib_s32 *leftEdgesE = param_e->leftEdges;              \
	mlib_s32 *rightEdgesE = param_e->rightEdges;            \
	mlib_type type = mlib_ImageGetType(dst);                \
	mlib_s32 channels = mlib_ImageGetChannels(dst);         \
	mlib_s32 dstStride = mlib_ImageGetStride(dst);          \
	mlib_s32 yStart = param->yStart;                        \
	mlib_s32 yFinish = param->yFinish;                      \
	mlib_s32 yStartE = param_e->yStart;                     \
	mlib_s32 yFinishE = param_e->yFinish;                   \
	mlib_u8 *data = param_e->dstData;                       \
	mlib_s32 xLeft, xRight, xLeftE, xRightE;                \
	mlib_s32 i

/* *********************************************************** */

#define	GET_EDGE_PARAMS_NN()                                    \
	GET_EDGE_PARAMS_ZERO();                                 \
	mlib_s32 *xStartsE = param_e->xStarts;                  \
	mlib_s32 *yStartsE = param_e->yStarts;                  \
	mlib_u8 **lineAddr = param->lineAddr;                   \
	mlib_s32 dX = param_e->dX;                              \
	mlib_s32 dY = param_e->dY;                              \
	mlib_s32 xSrc, ySrc, X, Y;                              \
	mlib_s32 j

/* *********************************************************** */

#define	GET_EDGE_PARAMS()                                       \
	GET_EDGE_PARAMS_NN();                                   \
	mlib_image *src = param->src;                           \
	mlib_s32 srcWidth = mlib_ImageGetWidth(src);            \
	mlib_s32 srcHeight = mlib_ImageGetHeight(src);          \
	mlib_s32 srcStride = mlib_ImageGetStride(src)

/* *********************************************************** */

void
mlib_ImageAffineEdgeZero(
	mlib_affine_param *param,
	mlib_affine_param *param_e,
	const void *colormap)
{
	GET_EDGE_PARAMS_ZERO();
	mlib_s32 zero = 0;

	if (colormap != NULL) {
		zero = mlib_ImageGetLutOffset(colormap);
	}

	switch (type) {
		case MLIB_BYTE:
			MLIB_PROCESS_EDGES_ZERO(mlib_u8);

			break;

		case MLIB_SHORT:
		case MLIB_USHORT:
			MLIB_PROCESS_EDGES_ZERO(mlib_s16);

			break;

		case MLIB_INT:
		case MLIB_FLOAT:
			MLIB_PROCESS_EDGES_ZERO(mlib_s32);

			break;

		case MLIB_DOUBLE: {
				mlib_d64 zero = 0;
				MLIB_PROCESS_EDGES_ZERO(mlib_d64);

				break;
			}
		default:
				break;
	}
}

/* *********************************************************** */

void
mlib_ImageAffineEdgeNearest(
	mlib_affine_param *param,
	mlib_affine_param *param_e)
{
	GET_EDGE_PARAMS_NN();

	switch (type) {
		case MLIB_BYTE:
			MLIB_PROCESS_EDGES(MLIB_EDGE_NEAREST_LINE, mlib_u8);

			break;

		case MLIB_SHORT:
		case MLIB_USHORT:
			MLIB_PROCESS_EDGES(MLIB_EDGE_NEAREST_LINE, mlib_s16);

			break;

		case MLIB_INT:
		case MLIB_FLOAT:
			MLIB_PROCESS_EDGES(MLIB_EDGE_NEAREST_LINE, mlib_s32);

			break;

		case MLIB_DOUBLE:
			MLIB_PROCESS_EDGES(MLIB_EDGE_NEAREST_LINE, mlib_d64);

			break;
		default:
			break;
	}
}

/* *********************************************************** */

mlib_status
mlib_ImageAffineEdgeExtend_BL(
	mlib_affine_param *param,
	mlib_affine_param *param_e,
	const void *colormap)
{
	GET_EDGE_PARAMS();
	/* mlib_d64 scale = 1.0 / (mlib_d64)MLIB_PREC; */
	mlib_s32 xDelta, yDelta, xFlag, yFlag;
#if 0
	mlib_d64 t, u, pix0;
	mlib_d64 a00, a01, a10, a11;
#else
	mlib_s64 t, u, pix0;
	mlib_s64 a00, a01, a10, a11;
#endif

	if (colormap != NULL) {
		mlib_s32 max_xsize = param_e->max_xsize;
		mlib_type ltype = mlib_ImageGetLutType(colormap);
		mlib_d64 *plut =
			(mlib_d64 *)mlib_ImageGetLutDoubleData(colormap);
		void *buff;

		channels = mlib_ImageGetLutChannels(colormap);
		plut -= channels * mlib_ImageGetLutOffset(colormap);

		if (max_xsize == 0) {
			return (MLIB_SUCCESS);
		}

		if (ltype == MLIB_BYTE) {
			buff = __mlib_malloc(channels * max_xsize);
		} else {
			buff = __mlib_malloc(channels * max_xsize *
				sizeof (mlib_s16));
		}

		if (buff == NULL)
			return (MLIB_FAILURE);

		switch (ltype) {
			case MLIB_BYTE:
				switch (type) {
					case MLIB_BYTE:
						MLIB_PROCESS_EDGES
							(MLIB_EDGE_INDEX_u8i,
							mlib_u8);
						break;

					case MLIB_SHORT:
						srcStride >>= 1;
						MLIB_PROCESS_EDGES
							(MLIB_EDGE_INDEX_u8i,
							mlib_s16);
						break;
					default:
						__mlib_free(buff);
						return (MLIB_FAILURE);
				}

				break;

			case MLIB_SHORT:
				switch (type) {
					case MLIB_BYTE:
						MLIB_PROCESS_EDGES
							(MLIB_EDGE_INDEX_s16i,
							mlib_u8);
						break;

					case MLIB_SHORT:
						srcStride >>= 1;
						MLIB_PROCESS_EDGES
							(MLIB_EDGE_INDEX_s16i,
							mlib_s16);
						break;
					default:
						__mlib_free(buff);
						return (MLIB_FAILURE);
				}

				break;
		default:
				__mlib_free(buff);
				return (MLIB_FAILURE);
		}

		__mlib_free(buff);

		return (MLIB_SUCCESS);
	}

	switch (type) {
		case MLIB_BYTE:
			MLIB_PROCESS_EDGES(MLIB_EDGE_BL, mlib_u8);

			break;

		case MLIB_SHORT:
			srcStride >>= 1;
			MLIB_PROCESS_EDGES(MLIB_EDGE_BL, mlib_s16);

			break;

		case MLIB_USHORT:
			srcStride >>= 1;
			MLIB_PROCESS_EDGES(MLIB_EDGE_BL, mlib_u16);

			break;

		case MLIB_INT:
			srcStride >>= 2;
			MLIB_PROCESS_EDGES(MLIB_EDGE_BL, mlib_s32);

			break;

		case MLIB_FLOAT:
			srcStride >>= 2;
			MLIB_PROCESS_EDGES(MLIB_EDGE_BL, mlib_f32);

			break;

		case MLIB_DOUBLE:
			srcStride >>= 3;
			MLIB_PROCESS_EDGES(MLIB_EDGE_BL, mlib_d64);

			break;
		default:
			return (MLIB_FAILURE);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#undef  MLIB_EDGE_INDEX
/* #define	MLIB_EDGE_INDEX	MLIB_EDGE_INDEX_BC */
#define	MLIB_EDGE_INDEX	MLIB_EDGE_INDEX_BC_INT

mlib_status
mlib_ImageAffineEdgeExtend_BC(
	mlib_affine_param *param,
	mlib_affine_param *param_e,
	const void *colormap)
{
	GET_EDGE_PARAMS();
	mlib_d64 scale = 1.0 / (mlib_d64)MLIB_PREC;
	mlib_s32 xFlag, yFlag;
	mlib_d64 dx, dx_2, dx2, dx3_2, dx3_3;
	mlib_d64 xf0, xf1, xf2, xf3;
	mlib_d64 yf0, yf1, yf2, yf3;
	mlib_d64 c0, c1, c2, c3, val0;
	mlib_s64 dx_int, dx_2_int, dx2_int, dx3_2_int, dx3_3_int;
	mlib_s64 xf0_int, xf1_int, xf2_int, xf3_int;
	mlib_s64 yf0_int, yf1_int, yf2_int, yf3_int;
	mlib_s64 c0_int, c1_int, c2_int, c3_int, val0_int;
	mlib_type ltype;
	mlib_filter filter = param->filter;
	mlib_f32 *fptr;
	mlib_f32 const *flt_tbl;
	mlib_s16 *fptr_int;
	mlib_s16 const *flt_tbl_int;
	mlib_s32 filterpos, flt_shift, flt_mask;
	mlib_s32 xDelta0, xDelta1, xDelta2;
	mlib_s32 yDelta0, yDelta1, yDelta2;
	mlib_d64 sat;
	mlib_s64 sat_int;

	ltype = (colormap != NULL) ? mlib_ImageGetLutType(colormap) : type;

	if (ltype == MLIB_BYTE) {
		flt_shift = FLT_SHIFT_U8;
		flt_mask = FLT_MASK_U8;
		flt_tbl =
			(filter ==
			MLIB_BICUBIC) ? mlib_filters_u8f_bc :
			mlib_filters_u8f_bc2;
		flt_tbl_int =
			(filter ==
			MLIB_BICUBIC) ? mlib_filters_u8_bc :
			mlib_filters_u8_bc2;
/* saturation for U8 */
		sat = (mlib_d64)0x7F800000;
		sat_int = (mlib_s64)0x7F800000;
	} else {
		flt_shift = FLT_SHIFT_S16;
		flt_mask = FLT_MASK_S16;
		flt_tbl =
			(filter ==
			MLIB_BICUBIC) ? mlib_filters_s16f_bc :
			mlib_filters_s16f_bc2;
		flt_tbl_int =
			(filter ==
			MLIB_BICUBIC) ? mlib_filters_s16_bc :
			mlib_filters_s16_bc2;
/* saturation for U16 */
		sat = (mlib_d64)0x7FFF8000;
		sat_int = (mlib_s64)0x7FFF8000;
	}

	if (colormap != NULL) {
		mlib_s32 max_xsize = param_e->max_xsize;
		mlib_d64 *plut =
			(mlib_d64 *)mlib_ImageGetLutDoubleData(colormap);
		void *buff;

		channels = mlib_ImageGetLutChannels(colormap);
		plut -= channels * mlib_ImageGetLutOffset(colormap);

		if (max_xsize == 0) {
			return (MLIB_SUCCESS);
		}

		if (ltype == MLIB_BYTE) {
			buff = __mlib_malloc(channels * max_xsize);
		} else {
			buff = __mlib_malloc(channels * max_xsize *
				sizeof (mlib_s16));
		}

		if (buff == NULL)
			return (MLIB_FAILURE);

		switch (ltype) {
			case MLIB_BYTE:
				switch (type) {
					case MLIB_BYTE:
						MLIB_PROCESS_EDGES
							(MLIB_EDGE_INDEX_u8i_BC,
							mlib_u8);
						break;

					case MLIB_SHORT:
						srcStride >>= 1;
						MLIB_PROCESS_EDGES
							(MLIB_EDGE_INDEX_u8i_BC,
							mlib_s16);
						break;
					default:
						__mlib_free(buff);
						return (MLIB_FAILURE);
				}

				break;

			case MLIB_SHORT:
				switch (type) {
					case MLIB_BYTE:
						MLIB_PROCESS_EDGES
						(MLIB_EDGE_INDEX_s16i_BC,
							mlib_u8);
						break;

					case MLIB_SHORT:
						srcStride >>= 1;
						MLIB_PROCESS_EDGES
						(MLIB_EDGE_INDEX_s16i_BC,
							mlib_s16);
						break;
					default:
						__mlib_free(buff);
						return (MLIB_FAILURE);
				}

				break;
		default:
				__mlib_free(buff);
				return (MLIB_FAILURE);
		}

		__mlib_free(buff);

		return (MLIB_SUCCESS);
	}

	switch (type) {
		case MLIB_BYTE:
			MLIB_PROCESS_EDGES(MLIB_EDGE_BC_TBL_U8, mlib_u8);

			break;

		case MLIB_SHORT:
			srcStride >>= 1;
			MLIB_PROCESS_EDGES(MLIB_EDGE_BC_TBL_16, mlib_s16);

			break;

		case MLIB_USHORT:
			srcStride >>= 1;
			MLIB_PROCESS_EDGES(MLIB_EDGE_BC_TBL_16, mlib_u16);

			break;

		case MLIB_INT:
			srcStride >>= 2;

			if (filter == MLIB_BICUBIC) {
				MLIB_PROCESS_EDGES(MLIB_EDGE_BC, mlib_s32);
			} else {
				MLIB_PROCESS_EDGES(MLIB_EDGE_BC2, mlib_s32);
			}

			break;

		case MLIB_FLOAT:
			srcStride >>= 2;

			if (filter == MLIB_BICUBIC) {
				MLIB_PROCESS_EDGES(MLIB_EDGE_BC, mlib_f32);
			} else {
				MLIB_PROCESS_EDGES(MLIB_EDGE_BC2, mlib_f32);
			}

			break;

		case MLIB_DOUBLE:
			srcStride >>= 3;

			if (filter == MLIB_BICUBIC) {
				MLIB_PROCESS_EDGES(MLIB_EDGE_BC, mlib_d64);
			} else {
				MLIB_PROCESS_EDGES(MLIB_EDGE_BC2, mlib_d64);
			}

			break;
		default:
			return (MLIB_FAILURE);
	}

	return (MLIB_SUCCESS);
}

#else /* _NO_LONGLONG */

#include <mlib_image.h>
#include <mlib_ImageColormap.h>
#include <mlib_ImageAffine.h>

/* *********************************************************** */

#define	FLT_SHIFT_U8	4
#define	FLT_MASK_U8	(((1 << 8) - 1) << 4)

#define	FLT_SHIFT_S16	3

#define	FLT_MASK_S16	(((1 << 9) - 1) << 4)

#define	MLIB_SIGN_SHIFT	31

/* *********************************************************** */

#define	D64mlib_u8(X)	mlib_U82D64[X]
#define	D64mlib_s16(X)	((mlib_d64)(X))

#define	D64mlib_u16(X)	((mlib_d64)(X))

#define	D64mlib_s32(X)	((mlib_d64)(X))

#define	D64mlib_f32(X)	((mlib_d64)(X))

#define	D64mlib_d64(X)	((mlib_d64)(X))

/* *********************************************************** */

#ifdef MLIB_USE_FTOI_CLAMPING

/* *********************************************************** */

#define	SATmlib_u8(DST, val0)                                   \
	DST = ((mlib_s32)(val0 - sat) >> 24) ^ 0x80;

/* *********************************************************** */

#define	SATmlib_s16(DST, val0)                                  \
	DST = ((mlib_s32)val0) >> 16;

/* *********************************************************** */

#define	SATmlib_u16(DST, val0)                                  \
	DST = ((mlib_s32)(val0 - sat) >> 16) ^ 0x8000;

/* *********************************************************** */

#define	SATmlib_s32(DST, val0)                                  \
	DST = val0;

#else /* MLIB_USE_FTOI_CLAMPING */

/* *********************************************************** */

#define	SATmlib_u8(DST, val0)                                   \
	val0 -= sat;                                            \
	if (val0 >= MLIB_S32_MAX)                               \
	    val0 = MLIB_S32_MAX;                                \
	if (val0 <= MLIB_S32_MIN)                               \
	    val0 = MLIB_S32_MIN;                                \
	DST = ((mlib_s32)val0 >> 24) ^ 0x80;

/* *********************************************************** */

#define	SATmlib_s16(DST, val0)                                  \
	if (val0 >= MLIB_S32_MAX)                               \
	    val0 = MLIB_S32_MAX;                                \
	if (val0 <= MLIB_S32_MIN)                               \
	    val0 = MLIB_S32_MIN;                                \
	DST = (mlib_s32)val0 >> 16;

/* *********************************************************** */

#define	SATmlib_u16(DST, val0)                                  \
	val0 -= sat;                                            \
	if (val0 >= MLIB_S32_MAX)                               \
	    val0 = MLIB_S32_MAX;                                \
	if (val0 <= MLIB_S32_MIN)                               \
	    val0 = MLIB_S32_MIN;                                \
	DST = ((mlib_s32)val0 >> 16) ^ 0x8000;

/* *********************************************************** */

#define	SATmlib_s32(DST, val0)                                  \
	if (val0 >= MLIB_S32_MAX)                               \
	    val0 = MLIB_S32_MAX;                                \
	if (val0 <= MLIB_S32_MIN)                               \
	    val0 = MLIB_S32_MIN;                                \
	DST = (mlib_s32)val0;

#endif /* MLIB_USE_FTOI_CLAMPING */

/* *********************************************************** */

#define	SATmlib_f32(DST, val0)                                  \
	DST = (mlib_f32)val0;

/* *********************************************************** */

#define	SATmlib_d64(DST, val0)                                  \
	DST = val0;

/* *********************************************************** */

#define	MLIB_EDGE_ZERO_LINE(TYPE, Left, Right)                  \
	dp = (TYPE *) data + channels * Left;                   \
	dstLineEnd = (TYPE *) data + channels * Right;          \
	for (; dp < dstLineEnd; dp++) {                         \
	    *dp = zero;                                         \
	}

/* *********************************************************** */

#define	MLIB_EDGE_NEAREST_LINE(TYPE, Left, Right)               \
	dp = (TYPE *) data + channels * Left;                   \
	size = Right - Left;                                    \
	for (j = 0; j < size; j++) {                            \
	    ySrc = Y >> MLIB_SHIFT;                             \
	    xSrc = X >> MLIB_SHIFT;                             \
	    sp = (TYPE *) lineAddr[ySrc] + xSrc * channels;     \
	    for (k = 0; k < channels; k++)                      \
		dp[k] = sp[k];                                  \
	    Y += dY;                                            \
	    X += dX;                                            \
	    dp += channels;                                     \
	}

/* *********************************************************** */

#define	MLIB_EDGE_BL(TYPE, Left, Right)                                    \
	dp = (TYPE *) data + channels * Left;                              \
	size = Right - Left;                                               \
	for (j = 0; j < size; j++) {                                       \
	    ySrc = ((Y - 32768) >> MLIB_SHIFT);                            \
	    xSrc = ((X - 32768) >> MLIB_SHIFT);                            \
	    t = ((X - 32768) & MLIB_MASK) * scale;                         \
	    u = ((Y - 32768) & MLIB_MASK) * scale;                         \
	    xDelta = (((xSrc + 1 -                                         \
		    srcWidth)) >> MLIB_SIGN_SHIFT) & channels;             \
	    yDelta = (((ySrc + 1 -                                         \
		    srcHeight)) >> MLIB_SIGN_SHIFT) & srcStride;           \
	    xFlag = (xSrc >> (MLIB_SIGN_SHIFT - MLIB_SHIFT));              \
	    xSrc = xSrc + (1 & xFlag);                                     \
	    xDelta = xDelta & ~xFlag;                                      \
	    yFlag = (ySrc >> (MLIB_SIGN_SHIFT - MLIB_SHIFT));              \
	    ySrc = ySrc + (1 & yFlag);                                     \
	    yDelta = yDelta & ~yFlag;                                      \
	    sp = (TYPE *) lineAddr[ySrc] + xSrc * channels;                \
	    for (k = 0; k < channels; k++) {                               \
		a00 = D64##TYPE(sp[0]);                                    \
		a01 = D64##TYPE(sp[xDelta]);                               \
		a10 = D64##TYPE(sp[yDelta]);                               \
		a11 = D64##TYPE(sp[yDelta + xDelta]);                      \
		pix0 = (a00 * (1 - t) + a01 * t) * (1 - u) + (a10 * (1 -   \
			t) + a11 * t) * u;                                 \
		dp[k] = (TYPE) pix0;                                       \
		sp++;                                                      \
	    }                                                              \
	    X += dX;                                                       \
	    Y += dY;                                                       \
	    dp += channels;                                                \
	}

/* *********************************************************** */

#define	LUT(k, ind)	plut[channels*sp[ind] + k]

/* *********************************************************** */

#define	MLIB_EDGE_INDEX(ITYPE, DTYPE, size)                                \
	for (j = 0; j < size; j++) {                                       \
	    ySrc = ((Y - 32768) >> MLIB_SHIFT);                            \
	    xSrc = ((X - 32768) >> MLIB_SHIFT);                            \
	    t = ((X - 32768) & MLIB_MASK) * scale;                         \
	    u = ((Y - 32768) & MLIB_MASK) * scale;                         \
	    xDelta = (((xSrc + 1 - srcWidth)) >> MLIB_SIGN_SHIFT) & 1;     \
	    yDelta = (((ySrc + 1 -                                         \
		    srcHeight)) >> MLIB_SIGN_SHIFT) & srcStride;           \
	    xFlag = (xSrc >> (MLIB_SIGN_SHIFT - MLIB_SHIFT));              \
	    xSrc = xSrc + (1 & xFlag);                                     \
	    xDelta = xDelta & ~xFlag;                                      \
	    yFlag = (ySrc >> (MLIB_SIGN_SHIFT - MLIB_SHIFT));              \
	    ySrc = ySrc + (1 & yFlag);                                     \
	    yDelta = yDelta & ~yFlag;                                      \
	    sp = (ITYPE *) lineAddr[ySrc] + xSrc;                          \
	    for (k = 0; k < channels; k++) {                               \
		a00 = LUT(k, 0);                                           \
		a01 = LUT(k, xDelta);                                      \
		a10 = LUT(k, yDelta);                                      \
		a11 = LUT(k, yDelta + xDelta);                             \
		pix0 = (a00 * (1 - t) + a01 * t) * (1 - u) + (a10 * (1 -   \
			t) + a11 * t) * u;                                 \
		pbuff[k] = (mlib_s32)pix0;                                 \
	    } pbuff += channels;                                           \
	    X += dX;                                                       \
	    Y += dY;                                                       \
	}

/* *********************************************************** */

#define	MLIB_EDGE_INDEX_u8i(ITYPE, Left, Right)                    \
	{                                                          \
	    mlib_u8 *pbuff = buff;                                 \
	                                                           \
	    size = Right - Left;                                   \
	    MLIB_EDGE_INDEX(ITYPE, mlib_u8,                        \
		    size);                                         \
	                                                           \
	    dp = (ITYPE *) data + Left;                            \
	    if (channels == 3) {                                   \
		if (sizeof (ITYPE) == 1) {                         \
		    mlib_ImageColorTrue2IndexLine_U8_U8_3(buff,    \
			    (void *)dp, size, colormap);           \
		} else {                                           \
		    mlib_ImageColorTrue2IndexLine_U8_S16_3(buff,   \
			    (void *)dp, size, colormap);           \
		}                                                  \
	    } else {                                               \
		if (sizeof (ITYPE) == 1) {                         \
		    mlib_ImageColorTrue2IndexLine_U8_U8_4(buff,    \
			    (void *)dp, size, colormap);           \
		} else {                                           \
		    mlib_ImageColorTrue2IndexLine_U8_S16_4(buff,   \
			    (void *)dp, size, colormap);           \
		}                                                  \
	    }                                                      \
	}

/* *********************************************************** */

#define	MLIB_EDGE_INDEX_s16i(ITYPE, Left, Right)                    \
	{                                                           \
	    mlib_s16 *pbuff = buff;                                 \
	                                                            \
	    size = Right - Left;                                    \
	    MLIB_EDGE_INDEX(ITYPE, mlib_s16,                        \
		    size);                                          \
	                                                            \
	    dp = (ITYPE *) data + Left;                             \
	    if (channels == 3) {                                    \
		if (sizeof (ITYPE) == 1) {                          \
		    mlib_ImageColorTrue2IndexLine_S16_U8_3(buff,    \
			    (void *)dp, size, colormap);            \
		} else {                                            \
		    mlib_ImageColorTrue2IndexLine_S16_S16_3(buff,   \
			    (void *)dp, size, colormap);            \
		}                                                   \
	    } else {                                                \
		if (sizeof (ITYPE) == 1) {                          \
		    mlib_ImageColorTrue2IndexLine_S16_U8_4(buff,    \
			    (void *)dp, size, colormap);            \
		} else {                                            \
		    mlib_ImageColorTrue2IndexLine_S16_S16_4(buff,   \
			    (void *)dp, size, colormap);            \
		}                                                   \
	    }                                                       \
	}

/* *********************************************************** */

#define	GET_FLT_TBL(X, xf0, xf1, xf2, xf3)                      \
	filterpos = ((X - 32768) >> flt_shift) & flt_mask;      \
	fptr = (mlib_f32 *)((mlib_u8 *)flt_tbl + filterpos);    \
	xf0 = fptr[0];                                          \
	xf1 = fptr[1];                                          \
	xf2 = fptr[2];                                          \
	xf3 = fptr[3];

/* *********************************************************** */

#define	GET_FLT_BC(X, xf0, xf1, xf2, xf3)                       \
	dx = ((X - 32768) & MLIB_MASK) * scale;                 \
	dx_2 = 0.5 * dx;                                        \
	dx2 = dx * dx;                                          \
	dx3_2 = dx_2 * dx2;                                     \
	dx3_3 = 3.0 * dx3_2;                                    \
	xf0 = dx2 - dx3_2 - dx_2;                               \
	xf1 = dx3_3 - 2.5 * dx2 + 1.0;                          \
	xf2 = 2.0 * dx2 - dx3_3 + dx_2;                         \
	xf3 = dx3_2 - 0.5 * dx2;

/* *********************************************************** */

#define	GET_FLT_BC2(X, xf0, xf1, xf2, xf3)                      \
	dx = ((X - 32768) & MLIB_MASK) * scale;                 \
	dx2 = dx * dx;                                          \
	dx3_2 = dx * dx2;                                       \
	dx3_3 = 2.0 * dx2;                                      \
	xf0 = -dx3_2 + dx3_3 - dx;                              \
	xf1 = dx3_2 - dx3_3 + 1.0;                              \
	xf2 = -dx3_2 + dx2 + dx;                                \
	xf3 = dx3_2 - dx2;

/* *********************************************************** */

#define	CALC_SRC_POS(X, Y, channels, srcStride)                            \
	xSrc = ((X - 32768) >> MLIB_SHIFT);                                \
	ySrc = ((Y - 32768) >> MLIB_SHIFT);                                \
	xDelta0 = ((~((xSrc - 1) >> MLIB_SIGN_SHIFT)) & (-channels));      \
	yDelta0 = ((~((ySrc - 1) >> MLIB_SIGN_SHIFT)) & (-srcStride));     \
	xDelta1 =                                                          \
		((xSrc + 1 - srcWidth) >> MLIB_SIGN_SHIFT) & (channels);   \
	yDelta1 =                                                          \
		((ySrc + 1 -                                               \
		srcHeight) >> MLIB_SIGN_SHIFT) & (srcStride);              \
	xDelta2 =                                                          \
		xDelta1 + (((xSrc + 2 -                                    \
		srcWidth) >> MLIB_SIGN_SHIFT) & (channels));               \
	yDelta2 =                                                          \
		yDelta1 + (((ySrc + 2 -                                    \
		srcHeight) >> MLIB_SIGN_SHIFT) & (srcStride));             \
	xFlag = (xSrc >> (MLIB_SIGN_SHIFT - MLIB_SHIFT));                  \
	xSrc = xSrc + (1 & xFlag);                                         \
	xDelta2 -= (xDelta1 & xFlag);                                      \
	xDelta1 = (xDelta1 & ~xFlag);                                      \
	yFlag = (ySrc >> (MLIB_SIGN_SHIFT - MLIB_SHIFT));                  \
	ySrc = ySrc + (1 & yFlag);                                         \
	yDelta2 -= (yDelta1 & yFlag);                                      \
	yDelta1 = yDelta1 & ~yFlag;

/* *********************************************************** */

#define	MLIB_EDGE_BC_LINE(TYPE, Left, Right, GET_FILTER)            \
	dp = (TYPE *) data + channels * Left;                       \
	size = Right - Left;                                        \
	for (j = 0; j < size; j++) {                                \
	    GET_FILTER(X, xf0, xf1, xf2, xf3);                      \
	    GET_FILTER(Y, yf0, yf1, yf2, yf3);                      \
	    CALC_SRC_POS(X, Y, channels, srcStride);                \
	    sp = (TYPE *) lineAddr[ySrc] + channels * xSrc;         \
	    for (k = 0; k < channels; k++) {                        \
		c0 = D64##TYPE(sp[yDelta0 + xDelta0]) * xf0 +       \
			D64##TYPE(sp[yDelta0]) * xf1 +              \
			D64##TYPE(sp[yDelta0 + xDelta1]) * xf2 +    \
			D64##TYPE(sp[yDelta0 + xDelta2]) * xf3;     \
		c1 = D64##TYPE(sp[xDelta0]) * xf0 +                 \
			D64##TYPE(sp[0]) * xf1 +                    \
			D64##TYPE(sp[xDelta1]) * xf2 +              \
			D64##TYPE(sp[xDelta2]) * xf3;               \
		c2 = D64##TYPE(sp[yDelta1 + xDelta0]) * xf0 +       \
			D64##TYPE(sp[yDelta1]) * xf1 +              \
			D64##TYPE(sp[yDelta1 + xDelta1]) * xf2 +    \
			D64##TYPE(sp[yDelta1 + xDelta2]) * xf3;     \
		c3 = D64##TYPE(sp[yDelta2 + xDelta0]) * xf0 +       \
			D64##TYPE(sp[yDelta2]) * xf1 +              \
			D64##TYPE(sp[yDelta2 + xDelta1]) * xf2 +    \
			D64##TYPE(sp[yDelta2 + xDelta2]) * xf3;     \
		val0 = c0 * yf0 + c1 * yf1 + c2 * yf2 + c3 * yf3;   \
		SAT##TYPE(dp[k], val0);                             \
		sp++;                                               \
	    }                                                       \
	    X += dX;                                                \
	    Y += dY;                                                \
	    dp += channels;                                         \
	}

/* *********************************************************** */

#define	MLIB_EDGE_BC_TBL(TYPE, Left, Right)                     \
	MLIB_EDGE_BC_LINE(TYPE, Left, Right, GET_FLT_TBL);

/* *********************************************************** */

#define	MLIB_EDGE_BC(TYPE, Left, Right)                         \
	MLIB_EDGE_BC_LINE(TYPE, Left, Right, GET_FLT_BC);

/* *********************************************************** */

#define	MLIB_EDGE_BC2(TYPE, Left, Right)                        \
	MLIB_EDGE_BC_LINE(TYPE, Left, Right, GET_FLT_BC2);

/* *********************************************************** */

#define	MLIB_EDGE_INDEX_BC(ITYPE, DTYPE, size)                          \
	for (j = 0; j < size; j++) {                                    \
	    GET_FLT_TBL(X, xf0, xf1, xf2, xf3);                         \
	    GET_FLT_TBL(Y, yf0, yf1, yf2, yf3);                         \
	    CALC_SRC_POS(X, Y, 1, srcStride);                           \
	    sp = (ITYPE *) lineAddr[ySrc] + xSrc;                       \
	    for (k = 0; k < channels; k++) {                            \
		c0 = LUT(k, yDelta0 + xDelta0) * xf0 + LUT(k,           \
			yDelta0) * xf1 + LUT(k,                         \
			yDelta0 + xDelta1) * xf2 + LUT(k,               \
			yDelta0 + xDelta2) * xf3;                       \
		c1 = LUT(k, xDelta0) * xf0 + LUT(k, 0) * xf1 + LUT(k,   \
			xDelta1) * xf2 + LUT(k, xDelta2) * xf3;         \
		c2 = LUT(k, yDelta1 + xDelta0) * xf0 + LUT(k,           \
			yDelta1) * xf1 + LUT(k,                         \
			yDelta1 + xDelta1) * xf2 + LUT(k,               \
			yDelta1 + xDelta2) * xf3;                       \
		c3 = LUT(k, yDelta2 + xDelta0) * xf0 + LUT(k,           \
			yDelta2) * xf1 + LUT(k,                         \
			yDelta2 + xDelta1) * xf2 + LUT(k,               \
			yDelta2 + xDelta2) * xf3;                       \
		val0 = c0 * yf0 + c1 * yf1 + c2 * yf2 + c3 * yf3;       \
		SAT##DTYPE(pbuff[k], val0);                             \
	    }                                                           \
	    pbuff += channels;                                          \
	    X += dX;                                                    \
	    Y += dY;                                                    \
	}

/* *********************************************************** */

#define	MLIB_PROCESS_EDGES_ZERO(TYPE)                           \
	{                                                       \
	    TYPE *dp, *dstLineEnd;                              \
	                                                        \
	    for (i = yStartE; i < yStart; i++) {                \
		xLeftE = leftEdgesE[i];                         \
		xRightE = rightEdgesE[i] + 1;                   \
		data += dstStride;                              \
		MLIB_EDGE_ZERO_LINE(TYPE, xLeftE, xRightE);     \
	    }                                                   \
	    for (; i <= yFinish; i++) {                         \
		xLeftE = leftEdgesE[i];                         \
		xRightE = rightEdgesE[i] + 1;                   \
		xLeft = leftEdges[i];                           \
		xRight = rightEdges[i] + 1;                     \
		data += dstStride;                              \
		if (xLeft < xRight) {                           \
		    MLIB_EDGE_ZERO_LINE(TYPE, xLeftE, xLeft);   \
		} else {                                        \
		    xRight = xLeftE;                            \
		}                                               \
		MLIB_EDGE_ZERO_LINE(TYPE, xRight, xRightE);     \
	    }                                                   \
	    for (; i <= yFinishE; i++) {                        \
		xLeftE = leftEdgesE[i];                         \
		xRightE = rightEdgesE[i] + 1;                   \
		data += dstStride;                              \
		MLIB_EDGE_ZERO_LINE(TYPE, xLeftE, xRightE);     \
	    }                                                   \
	}

/* *********************************************************** */

#define	MLIB_PROCESS_EDGES(PROCESS_LINE, TYPE)                  \
	{                                                       \
	    TYPE *sp, *dp;                                      \
	    mlib_s32 k, size;                                   \
	                                                        \
	    for (i = yStartE; i < yStart; i++) {                \
		xLeftE = leftEdgesE[i];                         \
		xRightE = rightEdgesE[i] + 1;                   \
		X = xStartsE[i];                                \
		Y = yStartsE[i];                                \
		data += dstStride;                              \
		PROCESS_LINE(TYPE, xLeftE, xRightE);            \
	    }                                                   \
	    for (; i <= yFinish; i++) {                         \
		xLeftE = leftEdgesE[i];                         \
		xRightE = rightEdgesE[i] + 1;                   \
		xLeft = leftEdges[i];                           \
		xRight = rightEdges[i] + 1;                     \
		X = xStartsE[i];                                \
		Y = yStartsE[i];                                \
		data += dstStride;                              \
		if (xLeft < xRight) {                           \
		    PROCESS_LINE(TYPE, xLeftE, xLeft);          \
		} else {                                        \
		    xRight = xLeftE;                            \
		}                                               \
		X = xStartsE[i] + dX * (xRight - xLeftE);       \
		Y = yStartsE[i] + dY * (xRight - xLeftE);       \
		PROCESS_LINE(TYPE, xRight, xRightE);            \
	    }                                                   \
	    for (; i <= yFinishE; i++) {                        \
		xLeftE = leftEdgesE[i];                         \
		xRightE = rightEdgesE[i] + 1;                   \
		X = xStartsE[i];                                \
		Y = yStartsE[i];                                \
		data += dstStride;                              \
		PROCESS_LINE(TYPE, xLeftE, xRightE);            \
	    }                                                   \
	}

/* *********************************************************** */

#define	GET_EDGE_PARAMS_ZERO()                                  \
	mlib_image *dst = param->dst;                           \
	mlib_s32 *leftEdges = param->leftEdges;                 \
	mlib_s32 *rightEdges = param->rightEdges;               \
	mlib_s32 *leftEdgesE = param_e->leftEdges;              \
	mlib_s32 *rightEdgesE = param_e->rightEdges;            \
	mlib_type type = mlib_ImageGetType(dst);                \
	mlib_s32 channels = mlib_ImageGetChannels(dst);         \
	mlib_s32 dstStride = mlib_ImageGetStride(dst);          \
	mlib_s32 yStart = param->yStart;                        \
	mlib_s32 yFinish = param->yFinish;                      \
	mlib_s32 yStartE = param_e->yStart;                     \
	mlib_s32 yFinishE = param_e->yFinish;                   \
	mlib_u8 *data = param_e->dstData;                       \
	mlib_s32 xLeft, xRight, xLeftE, xRightE;                \
	mlib_s32 i

/* *********************************************************** */

#define	GET_EDGE_PARAMS_NN()                                    \
	GET_EDGE_PARAMS_ZERO();                                 \
	mlib_s32 *xStartsE = param_e->xStarts;                  \
	mlib_s32 *yStartsE = param_e->yStarts;                  \
	mlib_u8 **lineAddr = param->lineAddr;                   \
	mlib_s32 dX = param_e->dX;                              \
	mlib_s32 dY = param_e->dY;                              \
	mlib_s32 xSrc, ySrc, X, Y;                              \
	mlib_s32 j

/* *********************************************************** */

#define	GET_EDGE_PARAMS()                                       \
	GET_EDGE_PARAMS_NN();                                   \
	mlib_image *src = param->src;                           \
	mlib_s32 srcWidth = mlib_ImageGetWidth(src);            \
	mlib_s32 srcHeight = mlib_ImageGetHeight(src);          \
	mlib_s32 srcStride = mlib_ImageGetStride(src)

/* *********************************************************** */

void
mlib_ImageAffineEdgeZero(
	mlib_affine_param *param,
	mlib_affine_param *param_e,
	const void *colormap)
{
	GET_EDGE_PARAMS_ZERO();
	mlib_s32 zero = 0;

	if (colormap != NULL) {
		zero = mlib_ImageGetLutOffset(colormap);
	}

	switch (type) {
		case MLIB_BYTE:
			MLIB_PROCESS_EDGES_ZERO(mlib_u8);

			break;

		case MLIB_SHORT:
		case MLIB_USHORT:
			MLIB_PROCESS_EDGES_ZERO(mlib_s16);

			break;

		case MLIB_INT:
		case MLIB_FLOAT:
			MLIB_PROCESS_EDGES_ZERO(mlib_s32);

			break;

		case MLIB_DOUBLE: {
				mlib_d64 zero = 0;
				MLIB_PROCESS_EDGES_ZERO(mlib_d64);

				break;
			}
		default:
				break;
	}
}

/* *********************************************************** */

void
mlib_ImageAffineEdgeNearest(
	mlib_affine_param *param,
	mlib_affine_param *param_e)
{
	GET_EDGE_PARAMS_NN();

	switch (type) {
		case MLIB_BYTE:
			MLIB_PROCESS_EDGES(MLIB_EDGE_NEAREST_LINE, mlib_u8);

			break;

		case MLIB_SHORT:
		case MLIB_USHORT:
			MLIB_PROCESS_EDGES(MLIB_EDGE_NEAREST_LINE, mlib_s16);

			break;

		case MLIB_INT:
		case MLIB_FLOAT:
			MLIB_PROCESS_EDGES(MLIB_EDGE_NEAREST_LINE, mlib_s32);

			break;

		case MLIB_DOUBLE:
			MLIB_PROCESS_EDGES(MLIB_EDGE_NEAREST_LINE, mlib_d64);

			break;
		default:
			break;
	}
}

/* *********************************************************** */

mlib_status
mlib_ImageAffineEdgeExtend_BL(
	mlib_affine_param *param,
	mlib_affine_param *param_e,
	const void *colormap)
{
	GET_EDGE_PARAMS();
	mlib_d64 scale = 1.0 / (mlib_d64)MLIB_PREC;
	mlib_s32 xDelta, yDelta, xFlag, yFlag;
	mlib_d64 t, u, pix0;
	mlib_d64 a00, a01, a10, a11;

	if (colormap != NULL) {
		mlib_s32 max_xsize = param_e->max_xsize;
		mlib_type ltype = mlib_ImageGetLutType(colormap);
		mlib_d64 *plut =
			(mlib_d64 *)mlib_ImageGetLutDoubleData(colormap);
		void *buff;

		channels = mlib_ImageGetLutChannels(colormap);
		plut -= channels * mlib_ImageGetLutOffset(colormap);

		if (max_xsize == 0) {
			return (MLIB_SUCCESS);
		}

		if (ltype == MLIB_BYTE) {
			buff = __mlib_malloc(channels * max_xsize);
		} else {
			buff = __mlib_malloc(channels * max_xsize *
				sizeof (mlib_s16));
		}

		if (buff == NULL)
			return (MLIB_FAILURE);

		switch (ltype) {
			case MLIB_BYTE:
				switch (type) {
					case MLIB_BYTE:
						MLIB_PROCESS_EDGES
							(MLIB_EDGE_INDEX_u8i,
							mlib_u8);
						break;

					case MLIB_SHORT:
						srcStride >>= 1;
						MLIB_PROCESS_EDGES
							(MLIB_EDGE_INDEX_u8i,
							mlib_s16);
						break;
					default:
						__mlib_free(buff);
						return (MLIB_FAILURE);
				}

				break;

			case MLIB_SHORT:
				switch (type) {
					case MLIB_BYTE:
						MLIB_PROCESS_EDGES
							(MLIB_EDGE_INDEX_s16i,
							mlib_u8);
						break;

					case MLIB_SHORT:
						srcStride >>= 1;
						MLIB_PROCESS_EDGES
							(MLIB_EDGE_INDEX_s16i,
							mlib_s16);
						break;
					default:
						__mlib_free(buff);
						return (MLIB_FAILURE);
				}

				break;
		default:
				__mlib_free(buff);
				return (MLIB_FAILURE);
		}

		__mlib_free(buff);

		return (MLIB_SUCCESS);
	}

	switch (type) {
		case MLIB_BYTE:
			MLIB_PROCESS_EDGES(MLIB_EDGE_BL, mlib_u8);

			break;

		case MLIB_SHORT:
			srcStride >>= 1;
			MLIB_PROCESS_EDGES(MLIB_EDGE_BL, mlib_s16);

			break;

		case MLIB_USHORT:
			srcStride >>= 1;
			MLIB_PROCESS_EDGES(MLIB_EDGE_BL, mlib_u16);

			break;

		case MLIB_INT:
			srcStride >>= 2;
			MLIB_PROCESS_EDGES(MLIB_EDGE_BL, mlib_s32);

			break;

		case MLIB_FLOAT:
			srcStride >>= 2;
			MLIB_PROCESS_EDGES(MLIB_EDGE_BL, mlib_f32);

			break;

		case MLIB_DOUBLE:
			srcStride >>= 3;
			MLIB_PROCESS_EDGES(MLIB_EDGE_BL, mlib_d64);

			break;
		default:
			return (MLIB_FAILURE);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#undef  MLIB_EDGE_INDEX
#define	MLIB_EDGE_INDEX	MLIB_EDGE_INDEX_BC

mlib_status
mlib_ImageAffineEdgeExtend_BC(
	mlib_affine_param *param,
	mlib_affine_param *param_e,
	const void *colormap)
{
	GET_EDGE_PARAMS();
	mlib_d64 scale = 1.0 / (mlib_d64)MLIB_PREC;
	mlib_s32 xFlag, yFlag;
	mlib_d64 dx, dx_2, dx2, dx3_2, dx3_3;
	mlib_d64 xf0, xf1, xf2, xf3;
	mlib_d64 yf0, yf1, yf2, yf3;
	mlib_d64 c0, c1, c2, c3, val0;
	mlib_type ltype;
	mlib_filter filter = param->filter;
	mlib_f32 *fptr;
	mlib_f32 const *flt_tbl;
	mlib_s32 filterpos, flt_shift, flt_mask;
	mlib_s32 xDelta0, xDelta1, xDelta2;
	mlib_s32 yDelta0, yDelta1, yDelta2;
	mlib_d64 sat;

	ltype = (colormap != NULL) ? mlib_ImageGetLutType(colormap) : type;

	if (ltype == MLIB_BYTE) {
		flt_shift = FLT_SHIFT_U8;
		flt_mask = FLT_MASK_U8;
		flt_tbl =
			(filter ==
			MLIB_BICUBIC) ? mlib_filters_u8f_bc :
			mlib_filters_u8f_bc2;
/* saturation for U8 */
		sat = (mlib_d64)0x7F800000;
	} else {
		flt_shift = FLT_SHIFT_S16;
		flt_mask = FLT_MASK_S16;
		flt_tbl =
			(filter ==
			MLIB_BICUBIC) ? mlib_filters_s16f_bc :
			mlib_filters_s16f_bc2;
/* saturation for U16 */
		sat = (mlib_d64)0x7FFF8000;
	}

	if (colormap != NULL) {
		mlib_s32 max_xsize = param_e->max_xsize;
		mlib_d64 *plut =
			(mlib_d64 *)mlib_ImageGetLutDoubleData(colormap);
		void *buff;

		channels = mlib_ImageGetLutChannels(colormap);
		plut -= channels * mlib_ImageGetLutOffset(colormap);

		if (max_xsize == 0) {
			return (MLIB_SUCCESS);
		}

		if (ltype == MLIB_BYTE) {
			buff = __mlib_malloc(channels * max_xsize);
		} else {
			buff = __mlib_malloc(channels * max_xsize *
				sizeof (mlib_s16));
		}

		if (buff == NULL)
			return (MLIB_FAILURE);

		switch (ltype) {
			case MLIB_BYTE:
				switch (type) {
					case MLIB_BYTE:
						MLIB_PROCESS_EDGES
							(MLIB_EDGE_INDEX_u8i,
							mlib_u8);
						break;

					case MLIB_SHORT:
						srcStride >>= 1;
						MLIB_PROCESS_EDGES
							(MLIB_EDGE_INDEX_u8i,
							mlib_s16);
						break;
					default:
						__mlib_free(buff);
						return (MLIB_FAILURE);
				}

				break;

			case MLIB_SHORT:
				switch (type) {
					case MLIB_BYTE:
						MLIB_PROCESS_EDGES
							(MLIB_EDGE_INDEX_s16i,
							mlib_u8);
						break;

					case MLIB_SHORT:
						srcStride >>= 1;
						MLIB_PROCESS_EDGES
							(MLIB_EDGE_INDEX_s16i,
							mlib_s16);
						break;
					default:
						__mlib_free(buff);
						return (MLIB_FAILURE);
				}

				break;
		default:
				__mlib_free(buff);
				return (MLIB_FAILURE);
		}

		__mlib_free(buff);

		return (MLIB_SUCCESS);
	}

	switch (type) {
		case MLIB_BYTE:
			MLIB_PROCESS_EDGES(MLIB_EDGE_BC_TBL, mlib_u8);

			break;

		case MLIB_SHORT:
			srcStride >>= 1;
			MLIB_PROCESS_EDGES(MLIB_EDGE_BC_TBL, mlib_s16);

			break;

		case MLIB_USHORT:
			srcStride >>= 1;
			MLIB_PROCESS_EDGES(MLIB_EDGE_BC_TBL, mlib_u16);

			break;

		case MLIB_INT:
			srcStride >>= 2;

			if (filter == MLIB_BICUBIC) {
				MLIB_PROCESS_EDGES(MLIB_EDGE_BC, mlib_s32);
			} else {
				MLIB_PROCESS_EDGES(MLIB_EDGE_BC2, mlib_s32);
			}

			break;

		case MLIB_FLOAT:
			srcStride >>= 2;

			if (filter == MLIB_BICUBIC) {
				MLIB_PROCESS_EDGES(MLIB_EDGE_BC, mlib_f32);
			} else {
				MLIB_PROCESS_EDGES(MLIB_EDGE_BC2, mlib_f32);
			}

			break;

		case MLIB_DOUBLE:
			srcStride >>= 3;

			if (filter == MLIB_BICUBIC) {
				MLIB_PROCESS_EDGES(MLIB_EDGE_BC, mlib_d64);
			} else {
				MLIB_PROCESS_EDGES(MLIB_EDGE_BC2, mlib_d64);
			}

			break;
		default:
			return (MLIB_FAILURE);
	}

	return (MLIB_SUCCESS);
}

#endif /* _NO_LONGLONG */

/* *********************************************************** */
