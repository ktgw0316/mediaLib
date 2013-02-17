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

#ifndef _MLIB_IMAGEPOLYNOMIALWARPTOOLS_H
#define	_MLIB_IMAGEPOLYNOMIALWARPTOOLS_H

#pragma ident	"@(#)mlib_ImagePolynomialWarpTools.h	9.2	07/10/09 SMI"

#endif /* _MLIB_IMAGEPOLYNOMIALWARPTOOLS_H */

#ifdef __cplusplus
extern "C" {
#endif

/* *********************************************************** */

#if DEGREE < 6
#define	REAL_NAME(X, Y)	mlib_ImagePolynomialWarpClipLine_DG_##X##Y
#if MODE == 0
#define	DECLARE_FUNCTION(X)	REAL_NAME(X, _0)
#elif MODE == 1	   /* MODE == 0 */
#define	DECLARE_FUNCTION(X)	REAL_NAME(X, _1)
#elif MODE == 2	   /* MODE == 0 */
#define	DECLARE_FUNCTION(X)	REAL_NAME(X, _2)
#elif MODE == 10   /* MODE == 0 */
#define	DECLARE_FUNCTION(X)	REAL_NAME(X, _10)
#else		   /* MODE == 0 */
#error ERROR : Incorrect value MODE
#endif		   /* MODE == 0 */

/* *********************************************************** */

#if DEGREE == 1
#define	INIT	INIT_COEFF_1; START_POINT;
#define	DELTA	DELTA_1
#elif DEGREE == 2  /* DEGREE == 1 */
#define	INIT	INIT_COEFF_2; START_POINT;
#define	DELTA	DELTA_2
#elif DEGREE == 3  /* DEGREE == 1 */
#define	INIT	INIT_COEFF_3; START_POINT;
#define	DELTA	DELTA_3
#elif DEGREE == 4  /* DEGREE == 1 */
#define	INIT	INIT_COEFF_4; START_POINT;
#define	DELTA	DELTA_4
#elif DEGREE == 5  /* DEGREE == 1 */
#define	INIT	INIT_COEFF_5; START_POINT;
#define	DELTA	DELTA_5
#else		   /* DEGREE == 1 */
#error ERROR : Incorrect value DEGREE
#endif		   /* DEGREE == 1 */

/* *********************************************************** */

#define	VAR_ALL                                                 \
	VAR_MODE10 mlib_s32 i, count = 0, res;                  \
	mlib_s32 *src_x = pws->src_x;                           \
	mlib_s32 *src_y = pws->src_y;                           \
	mlib_s32 *dst_x = pws->dst_x;                           \
	mlib_d64 *xCoeffs = pws->xCoeffs;                       \
	mlib_d64 *yCoeffs = pws->yCoeffs;                       \
	mlib_d64 SrcStartX = pws->SrcStartX;                    \
	mlib_d64 SrcStartY = pws->SrcStartY;                    \
	mlib_d64 SrcStopX = pws->SrcStopX;                      \
	mlib_d64 SrcStopY = pws->SrcStopY;                      \
	mlib_s32 srcWidth, srcWidthB;                           \
	mlib_s32 srcHeight, srcHeightB;                         \
	mlib_d64 x = 0.5 + preShiftX;                           \
	mlib_d64 a0, a1;                                        \
	mlib_d64 b0, b1;                                        \
	mlib_d64 dx, wx;                                        \
	mlib_d64 dy, wy;                                        \
	mlib_s32 iwx, iwy;
#if MODE == 10 && DEGREE == 1
#define	VAR_MODE10
#else		   /* MODE == 10 && DEGREE == 1 */
#define	VAR_MODE10	mlib_d64 a2, b2, ddx, ddy;
#endif		   /* MODE == 10 && DEGREE == 1 */

/* *********************************************************** */

#if MODE == 0 || MODE == 1
#define	VAR_MODE                                                \
	mlib_s16 *dsrc_x = pws->dsrc_x;                         \
	mlib_s16 *dsrc_y = pws->dsrc_y;
#elif MODE == 2	   /* MODE == 0 || MODE == 1 */
#define	VAR_MODE                                                \
	mlib_d64 *dsrc_x = pws->dsrc_x;                         \
	mlib_d64 *dsrc_y = pws->dsrc_y;
#elif MODE == 10   /* MODE == 0 || MODE == 1 */
#define	VAR_MODE                                                \
	mlib_d64 *pH = pws->tableH;                             \
	mlib_d64 *pV = pws->tableV;                             \
	mlib_d64 scaleH = pws->scaleH;                          \
	mlib_d64 scaleV = pws->scaleV;                          \
	mlib_s32 shiftH = pws->shiftH;                          \
	mlib_s32 shiftV = pws->shiftV;                          \
	mlib_addr *dsrc_x = pws->dsrc_x;                        \
	mlib_addr *dsrc_y = pws->dsrc_y;
#endif		   /* MODE == 0 || MODE == 1 */

/* *********************************************************** */

#if DEGREE > 4
#define	VAR_DEGREE                                              \
	mlib_d64 a3, b3, a4, b4, a5, b5;                        \
	mlib_d64 dddx, dddy, ddddx, ddddy, dddddx, dddddy;
#elif DEGREE > 3   /* DEGREE > 4 */
#define	VAR_DEGREE                                              \
	mlib_d64 a3, b3, a4, b4;                                \
	mlib_d64 dddx, dddy, ddddx, ddddy;
#elif DEGREE > 2   /* DEGREE > 4 */
#define	VAR_DEGREE	mlib_d64 a3, b3; mlib_d64 dddx, dddy;
#endif		   /* DEGREE > 4 */

/* *********************************************************** */

#if MODE == 0 || MODE == 1
#define	CALC_LINE                                               \
	src_y[count] = (mlib_s32)(wy - 0.5);                    \
	src_x[count] = (mlib_s32)(wx - 0.5);                    \
	dsrc_y[count] =                                         \
	    (mlib_s32)(((wy - 0.5) - (mlib_s32)(wy -            \
	    0.5)) * MLIB_PREC) >> (MLIB_SHIFT - 15);            \
	dsrc_x[count] =                                         \
	    (mlib_s32)(((wx - 0.5) - (mlib_s32)(wx -            \
	    0.5)) * MLIB_PREC) >> (MLIB_SHIFT - 15);
#elif MODE == 2	   /* MODE == 0 || MODE == 1 */
#define	CALC_LINE                                               \
	src_y[count] = (mlib_s32)(wy - 0.5);                    \
	src_x[count] = (mlib_s32)(wx - 0.5);                    \
	dsrc_y[count] = (wy - 0.5) - (mlib_s32)(wy - 0.5);      \
	dsrc_x[count] = (wx - 0.5) - (mlib_s32)(wx - 0.5);
#elif MODE == 10   /* MODE == 0 || MODE == 1 */
#define	CALC_LINE                                                        \
	src_y[count] = (mlib_s32)(wy - 0.5);                             \
	src_x[count] = (mlib_s32)(wx - 0.5);                             \
	dsrc_y[count] =                                                  \
	    (mlib_addr)(pV + ((mlib_s32)(((wy - 0.5) - (mlib_s32)(wy -   \
	    0.5)) * scaleV) << shiftV));                                 \
	dsrc_x[count] =                                                  \
	    (mlib_addr)(pH + ((mlib_s32)(((wx - 0.5) - (mlib_s32)(wx -   \
	    0.5)) * scaleH) << shiftH));
#endif		   /* MODE == 0 || MODE == 1 */

/* *********************************************************** */

#define	BODY                                                             \
	iwx = (mlib_s32)(wx + 0.5);                                      \
	iwy = (mlib_s32)(wy + 0.5);                                      \
	CALC_LINE;                                                       \
	dst_x[count] = i;                                                \
	res =                                                            \
	    (mlib_u32)(~(iwx - srcWidthB) & (iwx - srcWidth) & ~(iwy -   \
	    srcHeightB) & (iwy - srcHeight)) >> 31;                      \
	count += res;                                                    \
	DELTA

/* *********************************************************** */

#define	START_POINT                                             \
	srcWidth = (mlib_s32)(SrcStopX + 0.5);                  \
	srcHeight = (mlib_s32)(SrcStopY + 0.5);                 \
	srcWidthB = (mlib_s32)(SrcStartX + 0.5);                \
	srcHeightB = (mlib_s32)(SrcStartY + 0.5)

/* *********************************************************** */

#else		   /* DEGREE < 6 */
#define	REAL_NAME(X)	mlib_ImagePolynomialWarpClipLine_##X
#if MODE == 0
#define	DECLARE_FUNCTION(X)	REAL_NAME(0)
#elif MODE == 1	   /* MODE == 0 */
#define	DECLARE_FUNCTION(X)	REAL_NAME(1)
#elif MODE == 2	   /* MODE == 0 */
#define	DECLARE_FUNCTION(X)	REAL_NAME(2)
#elif MODE == 3	   /* MODE == 0 */
#define	DECLARE_FUNCTION(X)	REAL_NAME(3)
#elif MODE == 10   /* MODE == 0 */
#define	DECLARE_FUNCTION(X)	REAL_NAME(10)
#else		   /* MODE == 0 */
#error ERROR : Incorrect value MODE
#endif		   /* MODE == 0 */

/* *********************************************************** */

#define	VAR_ALL                                                 \
	void *mem = 0;                                          \
	mlib_d64 PowsBuf[2 * MAX_POWS];                         \
	mlib_d64 *xPows = PowsBuf;                              \
	mlib_d64 *yPows = PowsBuf + degree + 1;                 \
	mlib_d64 *xCoeffs = pws->xCoeffs;                       \
	mlib_d64 *yCoeffs = pws->yCoeffs;                       \
	mlib_d64 SrcStartX = pws->SrcStartX;                    \
	mlib_d64 SrcStartY = pws->SrcStartY;                    \
	mlib_d64 SrcStopX = pws->SrcStopX;                      \
	mlib_d64 SrcStopY = pws->SrcStopY;                      \
	mlib_d64 x = 0.5 + preShiftX;                           \
	mlib_d64 wx, wy, tmp_y;                                 \
	mlib_s32 i, j, k, count = 0;                            \
	mlib_s32 *src_x = pws->src_x;                           \
	mlib_s32 *src_y = pws->src_y;                           \
	mlib_s32 *dst_x = pws->dst_x;

/* *********************************************************** */

#if MODE == 1
#define	VAR_MODE                                                \
	mlib_d64 *dsrc_x = pws->dsrc_x;                         \
	mlib_d64 *dsrc_y = pws->dsrc_y;
#elif MODE == 2 || MODE == 3	/* MODE == 1 */
#define	VAR_MODE                                                \
	mlib_s16 *dsrc_x = pws->dsrc_x;                         \
	mlib_s16 *dsrc_y = pws->dsrc_y;
#elif MODE == 10   /* MODE == 1 */
#define	VAR_MODE                                                \
	mlib_d64 *pH = pws->tableH;                             \
	mlib_d64 *pV = pws->tableV;                             \
	mlib_d64 scaleH = pws->scaleH;                          \
	mlib_d64 scaleV = pws->scaleV;                          \
	mlib_s32 shiftH = pws->shiftH;                          \
	mlib_s32 shiftV = pws->shiftV;                          \
	mlib_addr *dsrc_x = pws->dsrc_x;                        \
	mlib_addr *dsrc_y = pws->dsrc_y;
#endif		   /* MODE == 1 */

/* *********************************************************** */

#if MODE == 0
#define	CALC_LINE                                               \
	src_y[count] = (mlib_s32)wy;                            \
	src_x[count] = (mlib_s32)wx;
#elif MODE == 1	   /* MODE == 0 */
#define	CALC_LINE                                               \
	wy -= 0.5;                                              \
	wx -= 0.5;                                              \
	src_y[count] = (mlib_s32)wy;                            \
	src_x[count] = (mlib_s32)wx;                            \
	dsrc_y[count] = wy - (mlib_s32)wy;                      \
	dsrc_x[count] = wx - (mlib_s32)wx;
#elif MODE == 2 || MODE == 3	/* MODE == 0 */
#define	CALC_LINE                                                          \
	wy -= 0.5;                                                         \
	wx -= 0.5;                                                         \
	src_y[count] = (mlib_s32)wy;                                       \
	src_x[count] = (mlib_s32)wx;                                       \
	dsrc_y[count] =                                                    \
	    (mlib_s32)((wy - (mlib_s32)wy) * MLIB_PREC) >> (MLIB_SHIFT -   \
	    15);                                                           \
	dsrc_x[count] =                                                    \
	    (mlib_s32)((wx - (mlib_s32)wx) * MLIB_PREC) >> (MLIB_SHIFT -   \
	    15);
#elif MODE == 10   /* MODE == 0 */
#define	CALC_LINE                                               \
	wy -= 0.5;                                              \
	wx -= 0.5;                                              \
	src_y[count] = (mlib_s32)wy;                            \
	src_x[count] = (mlib_s32)wx;                            \
	dsrc_y[count] =                                         \
	    (mlib_addr)(pV + ((mlib_s32)((wy -                  \
	    (mlib_s32)wy) * scaleV) << shiftV));                \
	dsrc_x[count] =                                         \
	    (mlib_addr)(pH + ((mlib_s32)((wx -                  \
	    (mlib_s32)wx) * scaleH) << shiftH));
#endif		   /* MODE == 0 */

/* *********************************************************** */

#define	INIT	INIT_CLIP

/* *********************************************************** */

#define	BODY                                                        \
	wx = xPows[degree];                                         \
	wy = yPows[degree];                                         \
	for (j = degree - 1; j >= 0; j--) {                         \
	    wx = wx * x + xPows[j];                                 \
	    wy = wy * x + yPows[j];                                 \
	}                                                           \
	                                                            \
	x += 1.0;                                                   \
	if (SrcStartX > wx || wx >= SrcStopX || SrcStartY > wy ||   \
	    wy >= SrcStopY)                                         \
	    continue;                                               \
	CALC_LINE;                                                  \
	(*dst_x++) = i;                                             \
	count++;
#endif		   /* DEGREE < 6 */

/* *********************************************************** */

#if defined(VAR_MODE)
#if defined(VAR_DEGREE)
#define	VAR	VAR_ALL VAR_MODE VAR_DEGREE
#else		   /* defined(VAR_DEGREE) */
#define	VAR	VAR_ALL VAR_MODE
#endif		   /* defined(VAR_DEGREE) */
#else		   /* defined(VAR_MODE) */
#if defined(VAR_DEGREE)
#define	VAR	VAR_ALL VAR_DEGREE
#else		   /* defined(VAR_DEGREE) */
#define	VAR	VAR_ALL
#endif		   /* defined(VAR_DEGREE) */
#endif		   /* defined(VAR_MODE) */

/* *********************************************************** */

mlib_s32 DECLARE_FUNCTION(DEGREE) (
    mlib_PWS *pws,
    mlib_d64 y,
    mlib_d64 preShiftX,
    mlib_s32 dstWidth,
    mlib_s32 degree)
{
	VAR;

	INIT;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#pragma nomemorydepend
#endif /* __SUNPRO_C */
	for (i = 0; i < dstWidth; i++) {
		BODY;
	}

#if (MODE == 0 || MODE == 1) && DEGREE < 6
	src_x[count] = 0;
	src_y[count] = 0;
#endif /* (MODE == 0 || MODE == 1) && DEGREE < 6 */

#if (MODE == 0 && DEGREE < 6) || (MODE == 3 && DEGREE >= 6)
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#pragma nomemorydepend
#endif	/* __SUNPRO_C */
#endif /* (MODE == 0 && DEGREE < 6) || (MODE == 3 && DEGREE >= 6) */

#if DEGREE >= 6
	if (mem)
		__mlib_free(mem);
#endif /* DEGREE >= 6 */

	return (count);
}

/* *********************************************************** */

#undef DECLARE_FUNCTION
#undef REAL_NAME

#undef MODE
#undef INIT
#undef DELTA

#undef VAR
#undef VAR_ALL
#undef VAR_MODE
#undef VAR_DEGREE
#undef VAR_MODE10

#undef BODY
#undef CALC_LINE

/* *********************************************************** */

#ifdef __cplusplus
}
#endif
