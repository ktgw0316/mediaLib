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

#ifndef _MLIB_I_IMAGEZOOMTRANSTABLEBLEND_F_H
#define	_MLIB_I_IMAGEZOOMTRANSTABLEBLEND_F_H

#pragma ident	"@(#)mlib_i_ImageZoomTransTableBlend_f.h	9.2	07/11/05 SMI"

#ifdef __cplusplus
extern "C" {
#endif

#include <mlib_types.h>
#include <mlib_image_types.h>
#include <mlib_status.h>
#include <mlib_ImageDivTables.h>

#ifndef _NO_LONGLONG

/* *********************************************************** */
/*
 * FUNCTION
 *      Internal functions for mlib_ImageZoomTranslateTableBlend.
 */

#if IMG_TYPE == 4 || IMG_TYPE == 5

#ifdef _MSC_VER
#pragma optimize("", off)
#endif		   /* _MSC_VER */

#endif		   /* IMG_TYPE == 4 || IMG_TYPE == 5 */

/* *********************************************************** */

/* AMP times magnify */
#define	AMP_DB 23
#define	DB2LL(d) (((d) << 1) == 0 ? 0 :					\
		((((d) & 0x000fffffffffffff | 0x0010000000000000)	\
		>> (52 - AMP_DB - ((((d) >> 52) & 0x7ff) - 1023)))	\
		* (((d) >> 63 << 1) + 1)))

#define	DB2LL_M(d, m) (((d) << 1) == 0 ? 0 :				\
		((((d) & 0x000fffffffffffff | 0x0010000000000000)	\
		>> (52 - (m) - ((((d) >> 52) & 0x7ff) - 1023)))		\
		* (((d) >> 63 << 1) + 1)))

#define	AMP_FT 15
#define	FT2LL(f) (((f) << 1) == 0 ? 0 :					\
		((((f) & 0x007fffff | 0x00800000)			\
		>> (23 - AMP_FT - ((((f) >> 23) & 0xff) - 127)))	\
		* (((f) >> 31 <<1) + 1)))

#define	FT2LL_M(f, m) (((f) << 1) == 0 ? 0 :				\
		((((f) & 0x007fffff | 0x00800000)			\
		>> (23 - (m) - ((((f) >> 23) & 0xff) - 127)))		\
		* (((f) >> 31 <<1) + 1)))

typedef union
{
	mlib_s64 s64;
	struct
	{
		mlib_s32 s0;
		mlib_s32 s1;
	} s32s;
} s64_2_s32;

/* *********************************************************** */

#if FLT_BITS == 2

#define	FILTER_TYPE	mlib_f32
#define	FP_TYPE	mlib_f32
#define	INT_TYPE mlib_s32
#define	AMP_FD AMP_FT
#define	FD2LL FT2LL

#elif FLT_BITS == 3	/* FLT_BITS == 2 */

#define	FILTER_TYPE	mlib_d64
#define	FP_TYPE	mlib_d64
#define	INT_TYPE mlib_s64
#define	AMP_FD AMP_DB
#define	FD2LL DB2LL

#endif		   /* FLT_BITS == 2 */

/* *********************************************************** */

#define	DTYPE	mlib_u8
#define	SAT_OFF	0
#define	SRC2FP(x)	(x)
#define	ZERO_EDGE	(1 << (AMP_FD - 1))	/* 0.5 */

/* *********************************************************** */

#ifdef SRC_EXTEND

#define	PARAMS	PARAMS_EXT

#define	DECL_LINE_ADD

#define	XPARAMS                                                        \
	mlib_s32 x, FILTER_TYPE * filterX, mlib_zoom_workspace * ws,   \
	    INT_TYPE * pbuff, DTYPE * sP0, mlib_s32 *x_ind,             \
	    mlib_s32 *x_tab

#define	CALL_XPARAMS	x, filterX, ws, pbuff, sP0, x_ind, x_tab

#define	X1PARAMS                                                       \
	mlib_s32 x, FILTER_TYPE * filterX, mlib_zoom_workspace * ws,   \
	    INT_TYPE * ybuff, mlib_s32 *x_ind, mlib_s32 *x_tab,         \
	    FILTER_TYPE * fpyy, DTYPE * sP0

#define	CALL_X1PARAMS	x, filterX, ws, ybuff, x_ind, x_tab, fpyy, sP0

#else /* SRC_EXTEND */

#define	PARAMS	PARAMS_NW

#define	DECL_LINE_ADD	mlib_s32 x_mask = ws -> x_mask;

#define	XPARAMS                                                        \
	mlib_s32 x, FILTER_TYPE * filterX, mlib_zoom_workspace * ws,   \
	    INT_TYPE * pbuff, DTYPE * sP0

#define	CALL_XPARAMS	x, filterX, ws, pbuff, sP0

#define	X1PARAMS                                                       \
	mlib_s32 x, FILTER_TYPE * filterX, mlib_zoom_workspace * ws,   \
	    INT_TYPE * ybuff, FILTER_TYPE * fpyy, DTYPE * sP0

#define	CALL_X1PARAMS	x, filterX, ws, ybuff, fpyy, sP0

#endif /* SRC_EXTEND */

/* *********************************************************** */

#define	X2PARAMS	X1PARAMS, DTYPE *sP1
#define	X3PARAMS	X2PARAMS, DTYPE *sP2
#define	X4PARAMS	X3PARAMS, DTYPE *sP3

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

#define	DECL_LINE                                                        \
	mlib_s32 i;                                                      \
	mlib_s32 xSrc;                                                   \
	mlib_s32 nchan = ws->nchan;                                      \
	mlib_s32 off = ws->off;                                          \
	mlib_s32 size = ws->size;                                        \
	mlib_s32 x_shift = ws->x_shift;                                  \
	mlib_s32 xf_shift = ws->xf_shift;                                \
	mlib_s32 xf_mask = ws->xf_mask;                                  \
	mlib_s32 alpha_shift = ((nchan == 3) ? 1 : (ws->alpha_shift));   \
	mlib_s32 filterposx;                                             \
	FILTER_TYPE *fptr;                                               \
	                                                                 \
	DECL_LINE_ADD

#define	DECL_CHAN2

#define	DECL_CHAN3

#define	CALC_SRC_PTRY1()	sP0 = (DTYPE*)lineAddr[ySrc + l]

#define	CALC_SRC_PTRY2()                                        \
	CALC_SRC_PTRY1();                                       \
	sP1 = (DTYPE *) lineAddr[ySrc + l + 1]

#define	CALC_SRC_PTRY3()                                        \
	CALC_SRC_PTRY2();                                       \
	sP2 = (DTYPE *) lineAddr[ySrc + l + 2]

#define	CALC_SRC_PTRY4()                                        \
	CALC_SRC_PTRY3();                                       \
	sP3 = (DTYPE *) lineAddr[ySrc + l + 3]

#define	CALC_XSRC_3	xSrc = (x >> x_shift) + off

#define	CALC_XSRC_4	CALC_XSRC_3

#define	CALC_SRC_PTRX_1(NUM)	CALC_XSRC_##NUM; sP0 = sP0_0

#define	CALC_SRC_PTRX_2(NUM)	CALC_SRC_PTRX_1(NUM); sP1 = sP1_0

#define	CALC_SRC_PTRX_3(NUM)	CALC_SRC_PTRX_2(NUM); sP2 = sP2_0

#define	CALC_SRC_PTRX_4(NUM)	CALC_SRC_PTRX_3(NUM); sP3 = sP3_0

#define	GET_SRC(sP, N)	SRC2FP(sP[x_ind[xSrc + N]])

#define	UPDATE_X	x = x_tab[i+1]

#else /* SRC_EXTEND */

#define	DECL_LINE                                                        \
	mlib_s32 i;                                                      \
	mlib_s32 xSrc;                                                   \
	mlib_s32 nchan = ws->nchan;                                      \
	mlib_s32 off = ws->off;                                          \
	mlib_s32 dx = ws->dx;                                            \
	mlib_s32 size = ws->size;                                        \
	mlib_s32 x_shift = ws->x_shift;                                  \
	mlib_s32 xf_shift = ws->xf_shift;                                \
	mlib_s32 xf_mask = ws->xf_mask;                                  \
	mlib_s32 alpha_shift = ((nchan == 3) ? 1 : (ws->alpha_shift));   \
	mlib_s32 filterposx;                                             \
	FILTER_TYPE *fptr;                                               \
	                                                                 \
	DECL_LINE_ADD

#define	DECL_CHAN2	mlib_s32  chan2 = nchan  + nchan;
#define	DECL_CHAN3	mlib_s32  chan3 = chan2 + nchan;

#define	CALC_SRC_PTRY1()	sP0 = (DTYPE*)lineAddr[ySrc + l] + coff

#define	CALC_SRC_PTRY2()                                        \
	CALC_SRC_PTRY1();                                       \
	sP1 = (DTYPE *) lineAddr[ySrc + l + 1] + coff

#define	CALC_SRC_PTRY3()                                        \
	CALC_SRC_PTRY2();                                       \
	sP2 = (DTYPE *) lineAddr[ySrc + l + 2] + coff

#define	CALC_SRC_PTRY4()                                        \
	CALC_SRC_PTRY3();                                       \
	sP3 = (DTYPE *) lineAddr[ySrc + l + 3] + coff

#define	CALC_XSRC_3	xSrc = (x >> x_shift) * 3

#define	CALC_XSRC_4	xSrc = (x >> x_shift) & x_mask
#define	CALC_SRC_PTRX_1(NUM)	CALC_XSRC_##NUM; sP0 = sP0_0 + xSrc

#define	CALC_SRC_PTRX_2(NUM)	CALC_SRC_PTRX_1(NUM); sP1 = sP1_0 + xSrc

#define	CALC_SRC_PTRX_3(NUM)	CALC_SRC_PTRX_2(NUM); sP2 = sP2_0 + xSrc

#define	CALC_SRC_PTRX_4(NUM)	CALC_SRC_PTRX_3(NUM); sP3 = sP3_0 + xSrc

#define	GET_SRC(sP, N)	SRC2FP(sP[CHAN##N])

#define	UPDATE_X	x += dx

#endif /* SRC_EXTEND */

/* *********************************************************** */

#define	MAX_KER	2
#define	MAX_KERH	4

/* *********************************************************** */
#define	CHAN0	0
#define	CHAN1	nchan
#define	CHAN2	chan2
#define	CHAN3	chan3

#if FLT_BITS == 2

/* *********************************************************** */

#define	DECL_YY	s64_2_s32 yyd0, yyd1;

/* *********************************************************** */

#define	DECL_X_SPECIFIC_1	s64_2_s32 xd0; mlib_s64 alp0

/* *********************************************************** */

#define	DECL_X_SPECIFIC_2	DECL_X_SPECIFIC_1; mlib_s64 alp1

/* *********************************************************** */

#define	DECL_X_SPECIFIC_3                                       \
	DECL_X_SPECIFIC_2;                                      \
	s64_2_s32 xd1;                                          \
	DECL_CHAN2 mlib_s64 alp2

/* *********************************************************** */

#define	DECL_X_SPECIFIC_4	DECL_X_SPECIFIC_3; DECL_CHAN3 mlib_s64 alp3

/* *********************************************************** */

#define	CALC_ALPHA_1	alp0 = 255 * FD2LL(xd0.s32s.s0); c0_0 = alp0

/* *********************************************************** */

#define	CALC_ALPHA_2                                            \
	alp0 = 255 * FD2LL(xd0.s32s.s0);                               \
	alp1 = 255 * FD2LL(xd0.s32s.s1);                               \
	c0_0 = alp0 + alp1
/* *********************************************************** */

#define	CALC_ALPHA_3                                            \
	alp0 = 255 * FD2LL(xd0.s32s.s0);                               \
	alp1 = 255 * FD2LL(xd0.s32s.s1);                               \
	alp2 = 255 * FD2LL(xd1.s32s.s0);                               \
	c0_0 = alp0 + alp1 + alp2

/* *********************************************************** */

#define	CALC_ALPHA_4                                            \
	alp0 = 255 * FD2LL(xd0.s32s.s0);                               \
	alp1 = 255 * FD2LL(xd0.s32s.s1);                               \
	alp2 = 255 * FD2LL(xd1.s32s.s0);                               \
	alp3 = 255 * FD2LL(xd1.s32s.s1);                               \
	c0_0 = alp0 + alp1 + alp2 + alp3

/* *********************************************************** */

#define	GET_ALPHA_4(sum, sP)                                    \
	alp0 = GET_SRC(sP, 0) * FD2LL(xd0.s32s.s0);                    \
	alp1 = GET_SRC(sP, 1) * FD2LL(xd0.s32s.s1);                    \
	alp2 = GET_SRC(sP, 2) * FD2LL(xd1.s32s.s0);                    \
	alp3 = GET_SRC(sP, 3) * FD2LL(xd1.s32s.s1);                    \
	sum = alp0 + alp1 + alp2 + alp3

/* *********************************************************** */

#define	GET_ALPHA_3(sum, sP)                                    \
	alp0 = GET_SRC(sP, 0) * FD2LL(xd0.s32s.s0);                    \
	alp1 = GET_SRC(sP, 1) * FD2LL(xd0.s32s.s1);                    \
	alp2 = GET_SRC(sP, 2) * FD2LL(xd1.s32s.s0);                    \
	sum = alp0 + alp1 + alp2

/* *********************************************************** */

#define	GET_ALPHA_2(sum, sP)                                    \
	alp0 = GET_SRC(sP, 0) * FD2LL(xd0.s32s.s0);                    \
	alp1 = GET_SRC(sP, 1) * FD2LL(xd0.s32s.s1);                    \
	sum = alp0 + alp1

/* *********************************************************** */

#define	GET_ALPHA_1(sum, sP)                                    \
	alp0 = GET_SRC(sP, 0) * FD2LL(xd0.s32s.s0);                    \
	sum = alp0

/* *********************************************************** */

#define	LOAD_YBUFF                                              \
	yyd0.s64 = *(mlib_s64 *)(ybuff + 4 * i);                \
	yyd1.s64 = *(mlib_s64 *)(ybuff + 4 * i + 2)

/* *********************************************************** */

#define	SAVE_YBUFF                                              \
	* (mlib_s64 *)(ybuff + 4 * i) = yyd0.s64;               \
	*(mlib_s64 *)(ybuff + 4 * i + 2) = yyd1.s64

/* *********************************************************** */

#define	SUM_Y_1                                                 \
	LOAD_YBUFF;                                             \
	yyd0.s32s.s0 += c0_0 * yf0 >> AMP_FD;                   \
	yyd0.s32s.s1 += c0_1 * yf0 >> AMP_FD;                   \
	yyd1.s32s.s0 += c0_2 * yf0 >> AMP_FD;                   \
	yyd1.s32s.s1 += c0_3 * yf0 >> AMP_FD;                   \
	SAVE_YBUFF

/* *********************************************************** */

#define	SUM_Y_2                                                 \
	LOAD_YBUFF;                                             \
	yyd0.s32s.s0 += (c0_0 * yf0 + c1_0 * yf1) >> AMP_FD;    \
	yyd0.s32s.s1 += (c0_1 * yf0 + c1_1 * yf1) >> AMP_FD;    \
	yyd1.s32s.s0 += (c0_2 * yf0 + c1_2 * yf1) >> AMP_FD;    \
	yyd1.s32s.s1 += (c0_3 * yf0 + c1_3 * yf1) >> AMP_FD;    \
	SAVE_YBUFF

/* *********************************************************** */

#define	SUM_Y_3                                                 \
	LOAD_YBUFF;                                             \
	yyd0.s32s.s0 += 						\
		(c0_0 * yf0 + c1_0 * yf1 + c2_0 * yf2) >> AMP_FD;	\
	yyd0.s32s.s1 +=							\
		(c0_1 * yf0 + c1_1 * yf1 + c2_1 * yf2) >> AMP_FD;	\
	yyd1.s32s.s0 +=							\
		(c0_2 * yf0 + c1_2 * yf1 + c2_2 * yf2) >> AMP_FD;	\
	yyd1.s32s.s1 +=							\
		(c0_3 * yf0 + c1_3 * yf1 + c2_3 * yf2) >> AMP_FD;	\
	SAVE_YBUFF

/* *********************************************************** */

#define	SUM_Y_4                                                  \
	LOAD_YBUFF;                                              \
	yyd0.s32s.s0 +=                                          \
	    (c0_0 * yf0 + c1_0 * yf1 + c2_0 * yf2 + c3_0 * yf3) >> AMP_FD;\
	yyd0.s32s.s1 +=                                          \
	    (c0_1 * yf0 + c1_1 * yf1 + c2_1 * yf2 + c3_1 * yf3) >> AMP_FD;\
	yyd1.s32s.s0 +=                                          \
	    (c0_2 * yf0 + c1_2 * yf1 + c2_2 * yf2 + c3_2 * yf3) >> AMP_FD;\
	yyd1.s32s.s1 +=                                          \
	    (c0_3 * yf0 + c1_3 * yf1 + c2_3 * yf2 + c3_3 * yf3) >> AMP_FD;\
	SAVE_YBUFF

/* *********************************************************** */

#else /* FLT_BITS == 2 */

/* *********************************************************** */

#define	DECL_YY

/* *********************************************************** */

#define	DECL_X_SPECIFIC_1	mlib_s64 alp0, xf0

/* *********************************************************** */

#define	DECL_X_SPECIFIC_2	DECL_X_SPECIFIC_1; mlib_s64 alp1, xf1

/* *********************************************************** */

#define	DECL_X_SPECIFIC_3                                       \
	DECL_X_SPECIFIC_2;                                      \
	mlib_s64 alp2, xf2;                                      \
	mlib_s32 chan2 = nchan + nchan

/* *********************************************************** */

#define	DECL_X_SPECIFIC_4                                       \
	DECL_X_SPECIFIC_3;                                      \
	mlib_s64 alp3, xf3;                                      \
	mlib_s32 chan3 = chan2 + nchan

/* *********************************************************** */

#define	CALC_ALPHA_1	alp0 = 255 * xf0; c0_0 = alp0

/* *********************************************************** */

#define	CALC_ALPHA_2                                            \
	alp0 = 255 * xf0;                                       \
	alp1 = 255 * xf1;                                       \
	c0_0 = alp0 + alp1
/* *********************************************************** */

#define	CALC_ALPHA_3                                            \
	alp0 = 255 * xf0;                                       \
	alp1 = 255 * xf1;                                       \
	alp2 = 255 * xf2;                                       \
	c0_0 = alp0 + alp1 + alp2

/* *********************************************************** */

#define	CALC_ALPHA_4                                            \
	alp0 = 255 * xf0;                                       \
	alp1 = 255 * xf1;                                       \
	alp2 = 255 * xf2;                                       \
	alp3 = 255 * xf3;                                       \
	c0_0 = alp0 + alp1 + alp2 + alp3

/* *********************************************************** */

#define	GET_ALPHA_4(sum, sP)                                    \
	alp0 = GET_SRC(sP, 0) * xf0;                            \
	alp1 = GET_SRC(sP, 1) * xf1;                            \
	alp2 = GET_SRC(sP, 2) * xf2;                            \
	alp3 = GET_SRC(sP, 3) * xf3;                            \
	sum = alp0 + alp1 + alp2 + alp3

/* *********************************************************** */

#define	GET_ALPHA_3(sum, sP)                                    \
	alp0 = GET_SRC(sP, 0) * xf0;                            \
	alp1 = GET_SRC(sP, 1) * xf1;                            \
	alp2 = GET_SRC(sP, 2) * xf2;                            \
	sum = alp0 + alp1 + alp2

/* *********************************************************** */

#define	GET_ALPHA_2(sum, sP)                                    \
	alp0 = GET_SRC(sP, 0) * xf0;                            \
	alp1 = GET_SRC(sP, 1) * xf1;                            \
	sum = alp0 + alp1

/* *********************************************************** */

#define	GET_ALPHA_1(sum, sP)	alp0 = GET_SRC(sP, 0) * xf0; sum = alp0

/* *********************************************************** */

#define	SUM_Y_1                                                 \
	ybuff[4 * i + 0] += c0_0 * yf0 >> AMP_FD;                         \
	ybuff[4 * i + 1] += c0_1 * yf0 >> AMP_FD;                         \
	ybuff[4 * i + 2] += c0_2 * yf0 >> AMP_FD;                         \
	ybuff[4 * i + 3] += c0_3 * yf0 >> AMP_FD

/* *********************************************************** */

#define	SUM_Y_2                                                 \
	ybuff[4 * i + 0] += (c0_0 * yf0 + c1_0 * yf1) >> AMP_FD;            \
	ybuff[4 * i + 1] += (c0_1 * yf0 + c1_1 * yf1) >> AMP_FD;            \
	ybuff[4 * i + 2] += (c0_2 * yf0 + c1_2 * yf1) >> AMP_FD;            \
	ybuff[4 * i + 3] += (c0_3 * yf0 + c1_3 * yf1) >> AMP_FD

/* *********************************************************** */

#define	SUM_Y_3                                                     \
	ybuff[4 * i + 0] += (c0_0 * yf0 + c1_0 * yf1 + c2_0 * yf2) >> AMP_FD;\
	ybuff[4 * i + 1] += (c0_1 * yf0 + c1_1 * yf1 + c2_1 * yf2) >> AMP_FD;\
	ybuff[4 * i + 2] += (c0_2 * yf0 + c1_2 * yf1 + c2_2 * yf2) >> AMP_FD;\
	ybuff[4 * i + 3] += (c0_3 * yf0 + c1_3 * yf1 + c2_3 * yf2) >> AMP_FD

/* *********************************************************** */

#define	SUM_Y_4                                                  \
	ybuff[4 * i + 0] +=                                      \
	    (c0_0 * yf0 + c1_0 * yf1 + c2_0 * yf2 + c3_0 * yf3) >> AMP_FD;   \
	ybuff[4 * i + 1] +=                                      \
	    (c0_1 * yf0 + c1_1 * yf1 + c2_1 * yf2 + c3_1 * yf3) >> AMP_FD;   \
	ybuff[4 * i + 2] +=                                      \
	    (c0_2 * yf0 + c1_2 * yf1 + c2_2 * yf2 + c3_2 * yf3) >> AMP_FD;   \
	ybuff[4 * i + 3] +=                                      \
	    (c0_3 * yf0 + c1_3 * yf1 + c2_3 * yf2 + c3_3 * yf3) >> AMP_FD

/* *********************************************************** */

#endif /* FLT_BITS == 2 */

/* *********************************************************** */

#define	MLIB_FILTER_X(x, filter, l)                                   \
	filterposx = (x >> xf_shift) & xf_mask;                       \
	fptr = (FILTER_TYPE *) ((mlib_u8 *)filter + filterposx) + l

/* *********************************************************** */

#define	GET_FILTER_Y()                                            \
	filterposy = (y >> yf_shift) & yf_mask;                   \
	fpy = (FILTER_TYPE *) ((mlib_u8 *)filterY + filterposy)

/* *********************************************************** */

#define	SUM_X_4(sum, sP)                                              \
	sum =                                                         \
	GET_SRC(sP, 0) * alp0 + GET_SRC(sP, 1) * alp1 + GET_SRC(sP,   \
	    2) * alp2 + GET_SRC(sP, 3) * alp3

/* *********************************************************** */

#define	SUM_X_3(sum, sP)                                              \
	sum =                                                         \
	GET_SRC(sP, 0) * alp0 + GET_SRC(sP, 1) * alp1 + GET_SRC(sP,   \
	    2) * alp2

/* *********************************************************** */

#define	SUM_X_2(sum, sP)                                        \
	sum = GET_SRC(sP, 0) * alp0 + GET_SRC(sP, 1) * alp1

/* *********************************************************** */

#define	SUM_X_1(sum, sP)	sum = GET_SRC(sP, 0) * alp0

/* *********************************************************** */

#define	SUM_X_3ch(NUM, LINE)                                    \
	SUM_X_##NUM(c##LINE##_1, sP##LINE);                     \
	sP##LINE++;                                             \
	SUM_X_##NUM(c##LINE##_2, sP##LINE);                     \
	sP##LINE++;                                             \
	SUM_X_##NUM(c##LINE##_3, sP##LINE)

/* *********************************************************** */

#define	SUM_X_4ch(NUM, LINE)                                    \
	GET_ALPHA_##NUM(c##LINE##_0, sP##LINE);                 \
	sP##LINE += alpha_shift;                                \
	SUM_X_3ch(NUM, LINE)

/* *********************************************************** */

#define	DECL_Y_SPECIFIC_1                                       \
	DTYPE *sP0_0 = sP0 + ((alpha_shift == 1) ? 0 : 3);      \
	mlib_s64 c0_0, c0_1, c0_2, c0_3, yf0

/* *********************************************************** */

#define	DECL_Y_SPECIFIC_2                                       \
	DECL_Y_SPECIFIC_1;                                      \
	DTYPE *sP1_0 = sP1 + ((alpha_shift == 1) ? 0 : 3);      \
	mlib_s64 c1_0, c1_1, c1_2, c1_3, yf1

/* *********************************************************** */

#define	DECL_Y_SPECIFIC_3                                       \
	DECL_Y_SPECIFIC_2;                                      \
	DTYPE *sP2_0 = sP2 + ((alpha_shift == 1) ? 0 : 3);      \
	mlib_s64 c2_0, c2_1, c2_2, c2_3, yf2

/* *********************************************************** */

#define	DECL_Y_SPECIFIC_4                                       \
	DECL_Y_SPECIFIC_3;                                      \
	DTYPE *sP3_0 = sP3 + ((alpha_shift == 1) ? 0 : 3);      \
	mlib_s64 c3_0, c3_1, c3_2, c3_3, yf3

/* *********************************************************** */

#define	LOAD_Y_FILTER_1	yf0 = FD2LL(*(INT_TYPE *)&fpyy[0])
#define	LOAD_Y_FILTER_2	LOAD_Y_FILTER_1; yf1 = FD2LL(*(INT_TYPE *)&fpyy[1])
#define	LOAD_Y_FILTER_2	LOAD_Y_FILTER_1; yf1 = FD2LL(*(INT_TYPE *)&fpyy[1])
#define	LOAD_Y_FILTER_3	LOAD_Y_FILTER_2; yf2 = FD2LL(*(INT_TYPE *)&fpyy[2])
#define	LOAD_Y_FILTER_4	LOAD_Y_FILTER_3; yf3 = FD2LL(*(INT_TYPE *)&fpyy[3])

#define	LOAD_X_FILTER_1	xd0.s64 = ((mlib_s64*)fptr)[0]
#define	LOAD_X_FILTER_2	LOAD_X_FILTER_1
#define	LOAD_X_FILTER_3	LOAD_X_FILTER_2; xd1.s64 = ((mlib_s64*)fptr)[1]
#define	LOAD_X_FILTER_4	LOAD_X_FILTER_3

/* *********************************************************** */

#define	EXPAND_ALPHA_1

#define	EXPAND_ALPHA_2	c1_0 = c0_0;
#define	EXPAND_ALPHA_3	c2_0 = c1_0 = c0_0;
#define	EXPAND_ALPHA_4	c3_0 = c2_0 = c1_0 = c0_0;

/* *********************************************************** */

#define	SUM_X_1_3ch(NUM)	SUM_X_3ch(NUM, 0)

/* *********************************************************** */

#define	SUM_X_2_3ch(NUM)	SUM_X_3ch(NUM, 0); SUM_X_3ch(NUM, 1)

/* *********************************************************** */

#define	SUM_X_3_3ch(NUM)                                        \
	SUM_X_3ch(NUM, 0);                                      \
	SUM_X_3ch(NUM, 1);                                      \
	SUM_X_3ch(NUM, 2)

/* *********************************************************** */

#define	SUM_X_4_3ch(NUM)                                        \
	SUM_X_3ch(NUM, 0);                                      \
	SUM_X_3ch(NUM, 1);                                      \
	SUM_X_3ch(NUM, 2);                                      \
	SUM_X_3ch(NUM, 3)

/* *********************************************************** */

#define	SUM_X_1_4ch(NUM)	SUM_X_4ch(NUM, 0)

/* *********************************************************** */

#define	SUM_X_2_4ch(NUM)	SUM_X_4ch(NUM, 0); SUM_X_4ch(NUM, 1)

/* *********************************************************** */

#define	SUM_X_3_4ch(NUM)                                        \
	SUM_X_4ch(NUM, 0);                                      \
	SUM_X_4ch(NUM, 1);                                      \
	SUM_X_4ch(NUM, 2)

/* *********************************************************** */

#define	SUM_X_4_4ch(NUM)                                        \
	SUM_X_4ch(NUM, 0);                                      \
	SUM_X_4ch(NUM, 1);                                      \
	SUM_X_4ch(NUM, 2);                                      \
	SUM_X_4ch(NUM, 3)

/* *********************************************************** */

#define	FIRST_PART_OF_LOW_FUNCTION(X_POINTS, Y_POINTS)          \
	static void                                             \
	blend_calc_##X_POINTS##_##Y_POINTS                      \
	    (X##Y_POINTS##PARAMS)                               \
	{                                                       \
	    DECL_LINE DECL_YY DECL_X_SPECIFIC_##X_POINTS;       \
	                                                        \
	    DECL_Y_SPECIFIC_##Y_POINTS;                         \
	    LOAD_Y_FILTER_##Y_POINTS;                           \
                                                                \
	    if (nchan == 3) {

/* *********************************************************** */

#define	SECOND_PART_OF_LOW_FUNCTION(X_POINTS, Y_POINTS)         \
		for (i = 0; i < size; i++) {                    \
		    MLIB_FILTER_X(x, filterX, off);             \
		    LOAD_X_FILTER_##X_POINTS;                   \
		    CALC_SRC_PTRX_##Y_POINTS(3);                \
		    CALC_ALPHA_##X_POINTS;                      \
		    EXPAND_ALPHA_##Y_POINTS                     \
		    SUM_X_##Y_POINTS##_3ch(X_POINTS);           \
		    SUM_Y_##Y_POINTS;                           \
		    UPDATE_X;                                   \
		}                                               \
		                                                \
	    } else {

/* *********************************************************** */

#define	THIRD_PART_OF_LOW_FUNCTION(X_POINTS, Y_POINTS)          \
		for (i = 0; i < size; i++) {                    \
		    MLIB_FILTER_X(x, filterX, off);             \
		    LOAD_X_FILTER_##X_POINTS;                   \
		    CALC_SRC_PTRX_##Y_POINTS(4);                \
		    SUM_X_##Y_POINTS##_4ch(X_POINTS);           \
		    SUM_Y_##Y_POINTS;                           \
		    UPDATE_X;                                   \
		}                                               \
	    }                                                   \
	}

/* *********************************************************** */

#define	SAVE_LINE                                               \
	pbuff[4 * i + 0] += c0_0;                               \
	pbuff[4 * i + 1] += c0_1;                               \
	pbuff[4 * i + 2] += c0_2;                               \
	pbuff[4 * i + 3] += c0_3

/* *********************************************************** */

#define	FIRST_PART_OF_LINE_FUNCTION(X_POINTS)                              \
	static void                                                        \
	blend_calc_line_##X_POINTS(XPARAMS)                                \
	{                                                                  \
	    DECL_LINE DTYPE *sP0_0 = sP0 + ((alpha_shift == 1) ? 0 : 3);   \
	    mlib_s64 c0_0, c0_1, c0_2, c0_3;                                \
	                                                                   \
	    DECL_X_SPECIFIC_##X_POINTS;                                    \
                                                                           \
	    if (nchan == 3) {

/* *********************************************************** */

#define	SECOND_PART_OF_LINE_FUNCTION(X_POINTS)                  \
		for (i = 0; i < size; i++) {                    \
		    MLIB_FILTER_X(x, filterX, off);             \
		    LOAD_X_FILTER_##X_POINTS;                   \
		    CALC_SRC_PTRX_1(3);                         \
		    CALC_ALPHA_##X_POINTS;                      \
		    SUM_X_1_3ch(X_POINTS);                      \
		    SAVE_LINE;                                  \
		    UPDATE_X;                                   \
		}                                               \
		                                                \
	    } else {

/* *********************************************************** */

#define	THIRD_PART_OF_LINE_FUNCTION(X_POINTS)                   \
		for (i = 0; i < size; i++) {                    \
		    MLIB_FILTER_X(x, filterX, off);             \
		    LOAD_X_FILTER_##X_POINTS;                   \
		    CALC_SRC_PTRX_1(4);                         \
		    SUM_X_1_4ch(X_POINTS);                      \
		    SAVE_LINE;                                  \
		    UPDATE_X;                                   \
		}                                               \
	    }                                                   \
	}

/* *********************************************************** */

#define	DECLARE_Y_SPECIFIC_1	INT_TYPE *buff0; mlib_s64 yf0; s64_2_s32 yd0

/* *********************************************************** */

#define	DECLARE_Y_SPECIFIC_2                                    \
	DECLARE_Y_SPECIFIC_1;                                   \
	INT_TYPE *buff1;					\
	mlib_s64 yf1;                                    	\
	s64_2_s32 yd1

/* *********************************************************** */

#define	DECLARE_Y_SPECIFIC_3                                    \
	DECLARE_Y_SPECIFIC_2;                                   \
	INT_TYPE *buff2;					\
	mlib_s64 yf2;	                                    	\
	s64_2_s32 yd2

/* *********************************************************** */

#define	DECLARE_Y_SPECIFIC_4                                    \
	DECLARE_Y_SPECIFIC_3;                                   \
	INT_TYPE *buff3;					\
	mlib_s64 yf3;		                                \
	s64_2_s32 yd3

/* *********************************************************** */

#define	LOAD_POINTER_1	buff0 = buff_arr[bnd]
#define	LOAD_POINTER_2	LOAD_POINTER_1; buff1 = buff_arr[bnd + 1]
#define	LOAD_POINTER_3	LOAD_POINTER_2; buff2 = buff_arr[bnd + 2]
#define	LOAD_POINTER_4	LOAD_POINTER_3; buff3 = buff_arr[bnd + 3]

/* *********************************************************** */

#define	LOAD_FROM_BUFFER_1                                      \
	yy.s64 = ((mlib_s64 *)ybuff)[i];                        \
	yd0.s64 = ((mlib_s64 *)buff0)[i]

/* *********************************************************** */

#define	LOAD_FROM_BUFFER_2                                      \
	LOAD_FROM_BUFFER_1;                                     \
	yd1.s64 = ((mlib_s64 *)buff1)[i]

/* *********************************************************** */

#define	LOAD_FROM_BUFFER_3                                      \
	LOAD_FROM_BUFFER_2;                                     \
	yd2.s64 = ((mlib_s64 *)buff2)[i]

/* *********************************************************** */

#define	LOAD_FROM_BUFFER_4                                      \
	LOAD_FROM_BUFFER_3;                                     \
	yd3.s64 = ((mlib_s64 *)buff3)[i]

/* *********************************************************** */

#define	SUM_Y_LINE_1                                            \
	yy.s32s.s0 += (yd0.s32s.s0) * yf0 >> AMP_FD;       \
	yy.s32s.s1 += (yd0.s32s.s1) * yf0 >> AMP_FD

/* *********************************************************** */

#define	SUM_Y_LINE_2                                            \
	yy.s32s.s0 += ((yd0.s32s.s0) * yf0 + (yd1.s32s.s0) * yf1)\
			>> AMP_FD;				\
	yy.s32s.s1 += ((yd0.s32s.s1) * yf0 + (yd1.s32s.s1) * yf1)\
			>> AMP_FD;

/* *********************************************************** */

#define	SUM_Y_LINE_3                                                     \
	yy.s32s.s0 += ((yd0.s32s.s0) * yf0 + (yd1.s32s.s0) * yf1\
		+ (yd2.s32s.s0) * yf2) >> AMP_FD;		\
	yy.s32s.s1 += ((yd0.s32s.s1) * yf0 + (yd1.s32s.s1) * yf1\
		+ (yd2.s32s.s1) * yf2) >> AMP_FD;

/* *********************************************************** */

#define	SUM_Y_LINE_4                                                      \
	yy.s32s.s0 += ((yd0.s32s.s0) * yf0 + (yd1.s32s.s0) * yf1\
		+ (yd2.s32s.s0) * yf2 + (yd3.s32s.s0) * yf3)	\
		>> AMP_FD;					\
	yy.s32s.s1 += ((yd0.s32s.s1) * yf0 + (yd1.s32s.s1) * yf1\
		+ (yd2.s32s.s1) * yf2 + (yd3.s32s.s1) * yf3)	\
		>> AMP_FD;

/* *********************************************************** */

#define	FIRST_PART_OF_Y_SUM_FUNCTION(Y_POINTS)                    \
	static void                                               \
	blend_mlib_calc##Y_POINTS(FILTER_TYPE * fpyy,             \
	    INT_TYPE ** buff_arr, mlib_s32 bnd, INT_TYPE * ybuff,   \
	    mlib_zoom_workspace * ws)                             \
	{                                                         \
	    mlib_s32 i;                                           \
	    mlib_s32 size = ws->size;                             \
	    s64_2_s32 yy;                                         \
	                                                          \
	    DECLARE_Y_SPECIFIC_##Y_POINTS;                        \
	    LOAD_Y_FILTER_##Y_POINTS;                             \
	    LOAD_POINTER_##Y_POINTS;

/* *********************************************************** */

#define	SECOND_PART_OF_Y_SUM_FUNCTION(Y_POINTS)                 \
	    for (i = 0; i < 2 * size; i++) {                    \
		LOAD_FROM_BUFFER_##Y_POINTS;                    \
		SUM_Y_LINE_##Y_POINTS;                          \
		((mlib_s64 *)ybuff)[i] = yy.s64;                \
	    }                                                   \
	}

/* *********************************************************** */

    FIRST_PART_OF_LOW_FUNCTION(1, 1)
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
    SECOND_PART_OF_LOW_FUNCTION(1, 1)
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
    THIRD_PART_OF_LOW_FUNCTION(1, 1)

/* *********************************************************** */

    FIRST_PART_OF_LOW_FUNCTION(2, 1)
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
    SECOND_PART_OF_LOW_FUNCTION(2, 1)
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
    THIRD_PART_OF_LOW_FUNCTION(2, 1)

/* *********************************************************** */

    FIRST_PART_OF_LOW_FUNCTION(1, 2)
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
    SECOND_PART_OF_LOW_FUNCTION(1, 2)
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
    THIRD_PART_OF_LOW_FUNCTION(1, 2)

/* *********************************************************** */

    FIRST_PART_OF_LOW_FUNCTION(2, 2)
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
    SECOND_PART_OF_LOW_FUNCTION(2, 2)
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
    THIRD_PART_OF_LOW_FUNCTION(2, 2)

/* *********************************************************** */

    FIRST_PART_OF_LINE_FUNCTION(1)
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
    SECOND_PART_OF_LINE_FUNCTION(1)
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
    THIRD_PART_OF_LINE_FUNCTION(1)

/* *********************************************************** */

    FIRST_PART_OF_LINE_FUNCTION(2)
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
    SECOND_PART_OF_LINE_FUNCTION(2)
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
    THIRD_PART_OF_LINE_FUNCTION(2)

/* *********************************************************** */

    FIRST_PART_OF_LINE_FUNCTION(3)
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
    SECOND_PART_OF_LINE_FUNCTION(3)
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
    THIRD_PART_OF_LINE_FUNCTION(3)

/* *********************************************************** */

    FIRST_PART_OF_LINE_FUNCTION(4)
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
    SECOND_PART_OF_LINE_FUNCTION(4)
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
    THIRD_PART_OF_LINE_FUNCTION(4)

/* *********************************************************** */

    FIRST_PART_OF_Y_SUM_FUNCTION(1)
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
    SECOND_PART_OF_Y_SUM_FUNCTION(1)

/* *********************************************************** */

    FIRST_PART_OF_Y_SUM_FUNCTION(2)
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
    SECOND_PART_OF_Y_SUM_FUNCTION(2)

/* *********************************************************** */

    FIRST_PART_OF_Y_SUM_FUNCTION(3)
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
    SECOND_PART_OF_Y_SUM_FUNCTION(3)

/* *********************************************************** */

    FIRST_PART_OF_Y_SUM_FUNCTION(4)
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
    SECOND_PART_OF_Y_SUM_FUNCTION(4)

/* *********************************************************** */

#define	SCALE	((1 << AMP_FD) /255)

/* *********************************************************** */

#define	DECLARE_LOAD_SRC                                        \
	mlib_s64 a0 = ybuff[4 * i];                              \
	mlib_s64 sr = ybuff[4 * i + 1];                          \
	mlib_s64 sg = ybuff[4 * i + 2];                          \
	mlib_s64 sb = ybuff[4 * i + 3]

#define	DECLARE_LOAD_4(dst0, dst1, dst2, dst3)                  \
	DECLARE_LOAD_SRC;                                       \
	mlib_s64 dst0 = SRC2FP(dPtr[4 * i]);                     \
	mlib_s64 dst1 = SRC2FP(dPtr[4 * i + 1]);                 \
	mlib_s64 dst2 = SRC2FP(dPtr[4 * i + 2]);                 \
	mlib_s64 dst3 = SRC2FP(dPtr[4 * i + 3]);                 \
	mlib_s64 scale;                                          \
	mlib_s64 ialp

#define	CALC_SCALE_ALPHA_4                                      \
	a1 <<= AMP_FD;						\
	a1 = a1 - (a0 * a1 / 255 >> AMP_FD);			\
	ialp = (a0 + a1 + ZERO_EDGE) >> AMP_FD;	                \
	if (a0 + a1 < ZERO_EDGE)					\
		scale = 0;					\
	else							\
		scale = (1 << AMP_FD * 2) / (a0 + a1)

#define	CALC_SCALE_A0						\
	if (a0 < ZERO_EDGE)						\
		scale = 0;					\
	else							\
		scale = (1 << AMP_FD * 2) / a0;

#define	BUF_INI                                                 \
	ybuff[4 * i] = SAT_OFF;                                 \
	ybuff[4 * i + 1] = SAT_OFF;                             \
	ybuff[4 * i + 2] = SAT_OFF;                             \
	ybuff[4 * i + 3] = SAT_OFF

#define	BLEND_SRC_OVER(dst, src0, src1)                         \
	dst = (((src0 + a1 * src1) * scale >> AMP_FD)		\
			+ ZERO_EDGE) >> AMP_FD

#define	BLEND_SRC_OVER2(dst, src0, src1)                        \
	dst = (((src0 - a0 * src1) * scale >> AMP_FD)		\
			+ ZERO_EDGE + (src1 << AMP_FD)) >> AMP_FD

#define	SAT(src, min, max)                                      \
	if (src <= min)                                         \
	    src = min;                                          \
	if (src >= max)                                         \
	    src = max

/* *********************************************************** */

#define	SATURATION                                              \
	SAT(a0, MLIB_U8_MIN, (MLIB_U8_MAX << AMP_FD));                      \
	SAT(sr, MLIB_U8_MIN, (MLIB_U8_MAX * MLIB_U8_MAX << AMP_FD));        \
	SAT(sg, MLIB_U8_MIN, (MLIB_U8_MAX * MLIB_U8_MAX << AMP_FD));        \
	SAT(sb, MLIB_U8_MIN, (MLIB_U8_MAX * MLIB_U8_MAX << AMP_FD))

#define	SATURATION_S                                              \
	SAT(sr, MLIB_U8_MIN, (MLIB_U8_MAX * MLIB_U8_MAX << AMP_FD));        \
	SAT(sg, MLIB_U8_MIN, (MLIB_U8_MAX * MLIB_U8_MAX << AMP_FD));        \
	SAT(sb, MLIB_U8_MIN, (MLIB_U8_MAX * MLIB_U8_MAX << AMP_FD))

/* *********************************************************** */

    mlib_status FUNC_NAME(
    PARAMS)
{
	INT_TYPE *buff;
	mlib_s64 zoomy = DB2LL(*(mlib_s64 *)&ws->zoomy);
	mlib_s32 max_xsize = ws->max_xsize;
	mlib_s32 low;
	mlib_s32 xLeft, xRight = 0, x0, y0, ynew;
	mlib_s32 ySrc;
	mlib_s32 yStart = ws->yStart;
	mlib_s32 yFinish = ws->yFinish;
	mlib_s32 dstYStride = ws->dstStride;
	mlib_s32 nchan = ws->nchan;
	mlib_s32 x_shift = ws->x_shift;
	mlib_s32 y_shift = ws->y_shift;
	mlib_s32 filterposy, sbits, x_mask;
	mlib_s32 size;
	DTYPE *sP0, *dstPixelPtr;
	mlib_s32 xf_shift, xf_mask;
	mlib_s32 m = table->width;
	mlib_s32 n = table->height;
	mlib_s32 xf_bits = table->width_bits;
	mlib_s32 yf_bits = table->height_bits;
	mlib_s32 subsampleBitsH = table->subsampleBitsH;
	mlib_s32 subsampleBitsV = table->subsampleBitsV;
	FILTER_TYPE *filterX, *filterY, *fpy;
	mlib_s32 i, j, k, l, kw, kh, off;

	mlib_s32 yf_shift, yf_mask;
	INT_TYPE **buff_arr = NULL, *pbuff, *ybuff;
	DTYPE *dPtr;
	mlib_s32 bind, y_step, y, x;

#ifndef SRC_EXTEND
	mlib_s32 coff;
#else /* SRC_EXTEND */
	mlib_s32 *x_tab;
#endif /* SRC_EXTEND */

	DTYPE *sP1;
	FILTER_TYPE *fpyy;

	max_xsize = (max_xsize + 1) & ~1;
	ws->max_xsize = max_xsize;

	if ((n * zoomy) > (1 << AMP_DB)) {
		buff =
		    __mlib_malloc((n + 1) * 4 * max_xsize * sizeof (INT_TYPE) +
		    2 * n * sizeof (INT_TYPE *));
		low = 0;
		if (buff == NULL) {
			buff = __mlib_malloc(4 * max_xsize * sizeof (INT_TYPE));
			low = 1;
		}
	} else {
		buff = __mlib_malloc(4 * max_xsize * sizeof (INT_TYPE));
		low = 1;
	}

	if (buff == NULL)
		return (MLIB_FAILURE);

	if (low) {
		ybuff = buff;
	} else {
		ybuff = buff + n * 4 * max_xsize;
		buff_arr = (INT_TYPE **) (ybuff + 4 * max_xsize);

		for (k = 0; k < n; k++)
			buff_arr[k] = buff + k * 4 * max_xsize;
		for (k = 0; k < n; k++)
			buff_arr[n + k] = buff_arr[k];
	}

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < 4 * max_xsize; i++) {
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
		sbits = 0;
		break;
	case 4:
		sbits = 2;
		break;
	default:
		sbits = 0;
	}

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

	y_step = n;
	bind = 0;

#ifndef SRC_EXTEND
	xLeft = leftEdges[yStart];
	xRight = rightEdges[yStart];
	if (xLeft > xRight) {
		__mlib_free(buff);
		return (MLIB_SUCCESS);
	}
	size = xRight - xLeft + 1;
	x0 = xStarts[yStart];

/* move to kernel center */
	x0 -= ws->x_move;
#else /* SRC_EXTEND */
	size = leftEdges[0];
	xLeft = leftEdges[1];
	if (size <= 0) {
		__mlib_free(buff);
		return (MLIB_SUCCESS);
	}
	x_tab = rightEdges;
	x0 = x_tab[0];
#endif /* SRC_EXTEND */

	dstPixelPtr = (DTYPE *) dstData + (ws->dchan) * xLeft;

	ws->size = size;

	ws->x0 = x0;

	for (j = yStart; j <= yFinish; j++) {
		CLIP;

		y = y0 - ws->y_move;
		GET_FILTER_Y();

		ySrc = y >> y_shift;

		ws->y0 = y0;
		if (low) {
			for (l = 0; l < n; l += kh) {

				kh = n - l;

				if (kh > MAX_KER)
					kh = MAX_KER;
				fpyy = fpy + l;

				if (kh == 2) {

/* offset in current kernel line */
					for (off = 0; off < m; off += kw) {
						x = x0;
						ws->off = off;
#ifndef SRC_EXTEND
						coff = CHAN1 * off;
#endif /* SRC_EXTEND */
						CALC_SRC_PTRY2();

						kw = m - off;

						if (kw > MAX_KER)
							kw = MAX_KER;

						if (kw == 2) {
							blend_calc_2_2
							    (CALL_X1PARAMS,
							    sP1);

						} else {    /* if (kw==1) */

							blend_calc_1_2
							    (CALL_X1PARAMS,
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
						coff = CHAN1 * off;
#endif /* SRC_EXTEND */
						CALC_SRC_PTRY1();

						kw = m - off;

						if (kw > MAX_KER)
							kw = MAX_KER;

						if (kw == 2) {
							blend_calc_2_1
							    (CALL_X1PARAMS);

						} else {	/* if (kw==1) */

							blend_calc_1_1
							    (CALL_X1PARAMS);
						}
					}
				}
			}
		} else {

			ynew = (ynew - ws->y_move) >> y_shift;
			if (y_step >= n) {
				y_step = n;
			} else {
				bind += y_step;

				if (bind >= n)
					bind -= n;
			}

			ws->y_step = y_step;
			for (l = n - y_step; l < n; l++) {

				pbuff = buff_arr[bind + l];

				for (i = 0; i < 4 * size; i++) {
					pbuff[i] = SAT_OFF;
				}

/* offset in current kernel line */
				for (off = 0; off < m; off += kw) {
					x = x0;

					ws->off = off;
#ifndef SRC_EXTEND
					coff = CHAN1 * off;
#endif /* SRC_EXTEND */
					CALC_SRC_PTRY1();

					kw = m - off;

					if (kw >= MAX_KERH)
						kw = MAX_KERH;

					if (kw == 4) {
						blend_calc_line_4(CALL_XPARAMS);

					} else if (kw == 3) {
						blend_calc_line_3(CALL_XPARAMS);

					} else if (kw == 2) {
						blend_calc_line_2(CALL_XPARAMS);

					} else {	/* if (kw == 1) */

						blend_calc_line_1(CALL_XPARAMS);
					}
				}
			}

			for (l = 0; l < n; l += kh) {
				kh = n - l;

				if (kh >= MAX_KERH)
					kh = MAX_KERH;

				if (kh == 4) {
					blend_mlib_calc4(fpy + l, buff_arr,
					    bind + l, ybuff, ws);

				} else if (kh == 3) {
					blend_mlib_calc3(fpy + l, buff_arr,
					    bind + l, ybuff, ws);

				} else if (kh == 2) {
					blend_mlib_calc2(fpy + l, buff_arr,
					    bind + l, ybuff, ws);

				} else {
/* kh == 1 */
					blend_mlib_calc1(fpy + l, buff_arr,
					    bind + l, ybuff, ws);
				}
			}

			y_step = ynew - ySrc;
		}
		dPtr = dstPixelPtr;

		if (ws->blend != MLIB_BLEND_GTK_SRC) {

			if (ws->dchan == 3) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i < size; i++) {
					DECLARE_LOAD_SRC;
					mlib_s64 dr = (dPtr[3 * i]);
					mlib_s64 dg = (dPtr[3 * i + 1]);
					mlib_s64 db = (dPtr[3 * i + 2]);
					mlib_s64 a1;
					mlib_s64 scale = SCALE;

					SATURATION;
					a1 = (255 << AMP_FD) - a0;

					BLEND_SRC_OVER(dPtr[3 * i + 0], sr, dr);
					BLEND_SRC_OVER(dPtr[3 * i + 1], sg, dg);
					BLEND_SRC_OVER(dPtr[3 * i + 2], sb, db);

					BUF_INI;
				}
			} else {
				if (ws->blend == MLIB_BLEND_GTK_SRC_OVER) {
					if (ws->alpha_shift == 1) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
						for (i = 0; i < size; i++) {
							DECLARE_LOAD_4(a1, dr,
							    dg, db);

							SATURATION;
							CALC_SCALE_ALPHA_4;

							dPtr[4 * i] = ialp;
							BLEND_SRC_OVER(dPtr[4 *
							    i + 1], sr, dr);
							BLEND_SRC_OVER(dPtr[4 *
							    i + 2], sg, dg);
							BLEND_SRC_OVER(dPtr[4 *
							    i + 3], sb, db);

							BUF_INI;
						}
					} else {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
						for (i = 0; i < size; i++) {
							DECLARE_LOAD_4(dr, dg,
							    db, a1);

							SATURATION;
							CALC_SCALE_ALPHA_4;

							BLEND_SRC_OVER(dPtr[4 *
							    i + 0], sr, dr);
							BLEND_SRC_OVER(dPtr[4 *
							    i + 1], sg, dg);
							BLEND_SRC_OVER(dPtr[4 *
							    i + 2], sb, db);
							dPtr[4 * i + 3] = ialp;

							BUF_INI;
						}
					}
				} else {
					if (ws->alpha_shift == 1) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
						for (i = 0; i < size; i++) {
							DECLARE_LOAD_4(a1, dr,
							    dg, db);

							SATURATION;
							CALC_SCALE_ALPHA_4;

							dPtr[4 * i] = ialp;
							BLEND_SRC_OVER2(dPtr[4 *
							    i + 1], sr, dr);
							BLEND_SRC_OVER2(dPtr[4 *
							    i + 2], sg, dg);
							BLEND_SRC_OVER2(dPtr[4 *
							    i + 3], sb, db);

							BUF_INI;
						}
					} else {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
						for (i = 0; i < size; i++) {
							DECLARE_LOAD_4(dr, dg,
							    db, a1);

							SATURATION;
							CALC_SCALE_ALPHA_4;

							BLEND_SRC_OVER2(dPtr[4 *
							    i + 0], sr, dr);
							BLEND_SRC_OVER2(dPtr[4 *
							    i + 1], sg, dg);
							BLEND_SRC_OVER2(dPtr[4 *
							    i + 2], sb, db);
							dPtr[4 * i + 3] = ialp;

							BUF_INI;
						}
					}
				}
			}

		} else {


			if (ws->dchan == 3) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i < size; i++) {
					mlib_s64 sr = ybuff[4 * i + 1];
					mlib_s64 sg = ybuff[4 * i + 2];
					mlib_s64 sb = ybuff[4 * i + 3];

					SATURATION_S;

					dPtr[3 * i] =
					    (sr / 255 + ZERO_EDGE) >> AMP_FD;
					dPtr[3 * i + 1] =
					    (sg / 255 + ZERO_EDGE) >> AMP_FD;
					dPtr[3 * i + 2] =
					    (sb / 255 + ZERO_EDGE) >> AMP_FD;

					BUF_INI;
				}
			} else {
				if (nchan == 3) {
					if (ws->alpha_shift == 1) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
						for (i = 0; i < size; i++) {
							mlib_s64 sr =
							    ybuff[4 * i + 1];
							mlib_s64 sg =
							    ybuff[4 * i + 2];
							mlib_s64 sb =
							    ybuff[4 * i + 3];

							SATURATION_S;

						    dPtr[4 * i] = 255;
						    dPtr[4 * i + 1] =
							(sr / 255 +
							ZERO_EDGE) >> AMP_FD;
						    dPtr[4 * i + 2] =
							(sg / 255 +
							ZERO_EDGE) >> AMP_FD;
						    dPtr[4 * i + 3] =
							(sb / 255 +
							ZERO_EDGE) >> AMP_FD;

							BUF_INI;
						}
					} else {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
						for (i = 0; i < size; i++) {
							mlib_s64 sr =
							    ybuff[4 * i + 1];
							mlib_s64 sg =
							    ybuff[4 * i + 2];
							mlib_s64 sb =
							    ybuff[4 * i + 3];

							SATURATION_S;

						    dPtr[4 * i] =
							(sr / 255 +
							ZERO_EDGE) >> AMP_FD;
						    dPtr[4 * i + 1] =
							(sg / 255 +
							ZERO_EDGE) >> AMP_FD;
						    dPtr[4 * i + 2] =
							(sb / 255 +
							ZERO_EDGE) >> AMP_FD;
						    dPtr[4 * i + 3] = 255;

							BUF_INI;
						}
					}
				} else {
					if (ws->alpha_shift == 1) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
						for (i = 0; i < size; i++) {
							DECLARE_LOAD_SRC;
							mlib_s64 scale;
							mlib_s64 ialp;

							SATURATION;
							ialp =
							    (a0 + ZERO_EDGE)
							    >> AMP_FD;
							CALC_SCALE_A0;

						    dPtr[4 * i] = ialp;
						    dPtr[4 * i + 1] =
							((sr*scale >> AMP_FD)
							+ ZERO_EDGE)>>AMP_FD;
						    dPtr[4 * i + 2] =
							((sg*scale >> AMP_FD)
							+ ZERO_EDGE)>>AMP_FD;
						    dPtr[4 * i + 3] =
							((sb*scale >> AMP_FD)
							+ ZERO_EDGE)>>AMP_FD;

						    BUF_INI;
						}
					} else {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
						for (i = 0; i < size; i++) {
							DECLARE_LOAD_SRC;
							mlib_s64 scale;
							mlib_s64 ialp;

							SATURATION;
							ialp = (a0 + ZERO_EDGE)
							    >> AMP_FD;
							CALC_SCALE_A0;

						    dPtr[4 * i] =
							((sr*scale >> AMP_FD)
							+ ZERO_EDGE)>>AMP_FD;
						    dPtr[4 * i + 1] =
							((sg*scale >> AMP_FD)
							+ ZERO_EDGE)>>AMP_FD;
						    dPtr[4 * i + 2] =
							((sb*scale >> AMP_FD)
							+ ZERO_EDGE)>>AMP_FD;
						    dPtr[4 * i + 3] = ialp;

						    BUF_INI;
						}
					}
				}
			}

		}
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

#else /* _NO_LONGLONG */


/* *********************************************************** */

#if FLT_BITS == 2

#define	FILTER_TYPE	mlib_f32
#define	FP_TYPE	mlib_f32

#elif FLT_BITS == 3	/* FLT_BITS == 2 */

#define	FILTER_TYPE	mlib_d64
#define	FP_TYPE	mlib_d64

#endif		   /* FLT_BITS == 2 */

/* *********************************************************** */
/*
 * FUNCTION
 *      Internal functions for mlib_ImageZoomTranslateTableBlend.
 */

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

#define	DTYPE	mlib_u8
#define	SAT_OFF	0.0
#define	SRC2FP(x)	U82FP(x)
#define	ZERO_EDGE	(FP_TYPE)0.5

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

#ifdef SRC_EXTEND

#define	PARAMS	PARAMS_EXT

#define	DECL_LINE_ADD

#define	XPARAMS                                                        \
	mlib_s32 x, FILTER_TYPE * filterX, mlib_zoom_workspace * ws,   \
	    FP_TYPE * pbuff, DTYPE * sP0, mlib_s32 *x_ind,             \
	    mlib_s32 *x_tab

#define	CALL_XPARAMS	x, filterX, ws, pbuff, sP0, x_ind, x_tab

#define	X1PARAMS                                                       \
	mlib_s32 x, FILTER_TYPE * filterX, mlib_zoom_workspace * ws,   \
	    FP_TYPE * ybuff, mlib_s32 *x_ind, mlib_s32 *x_tab,         \
	    FILTER_TYPE * fpyy, DTYPE * sP0

#define	CALL_X1PARAMS	x, filterX, ws, ybuff, x_ind, x_tab, fpyy, sP0

#else /* SRC_EXTEND */

#define	PARAMS	PARAMS_NW

#define	DECL_LINE_ADD	mlib_s32 x_mask = ws -> x_mask;

#define	XPARAMS                                                        \
	mlib_s32 x, FILTER_TYPE * filterX, mlib_zoom_workspace * ws,   \
	    FP_TYPE * pbuff, DTYPE * sP0

#define	CALL_XPARAMS	x, filterX, ws, pbuff, sP0

#define	X1PARAMS                                                       \
	mlib_s32 x, FILTER_TYPE * filterX, mlib_zoom_workspace * ws,   \
	    FP_TYPE * ybuff, FILTER_TYPE * fpyy, DTYPE * sP0

#define	CALL_X1PARAMS	x, filterX, ws, ybuff, fpyy, sP0

#endif /* SRC_EXTEND */

/* *********************************************************** */

#define	X2PARAMS	X1PARAMS, DTYPE *sP1
#define	X3PARAMS	X2PARAMS, DTYPE *sP2
#define	X4PARAMS	X3PARAMS, DTYPE *sP3

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

#define	DECL_LINE                                                        \
	mlib_s32 i;                                                      \
	mlib_s32 xSrc;                                                   \
	mlib_s32 nchan = ws->nchan;                                      \
	mlib_s32 off = ws->off;                                          \
	mlib_s32 size = ws->size;                                        \
	mlib_s32 x_shift = ws->x_shift;                                  \
	mlib_s32 xf_shift = ws->xf_shift;                                \
	mlib_s32 xf_mask = ws->xf_mask;                                  \
	mlib_s32 alpha_shift = ((nchan == 3) ? 1 : (ws->alpha_shift));   \
	mlib_s32 filterposx;                                             \
	FILTER_TYPE *fptr;                                               \
	                                                                 \
	DECL_LINE_ADD

#define	DECL_CHAN2

#define	DECL_CHAN3

#define	CALC_SRC_PTRY1()	sP0 = (DTYPE*)lineAddr[ySrc + l]

#define	CALC_SRC_PTRY2()                                        \
	CALC_SRC_PTRY1();                                       \
	sP1 = (DTYPE *) lineAddr[ySrc + l + 1]

#define	CALC_SRC_PTRY3()                                        \
	CALC_SRC_PTRY2();                                       \
	sP2 = (DTYPE *) lineAddr[ySrc + l + 2]

#define	CALC_SRC_PTRY4()                                        \
	CALC_SRC_PTRY3();                                       \
	sP3 = (DTYPE *) lineAddr[ySrc + l + 3]

#define	CALC_XSRC_3	xSrc = (x >> x_shift) + off

#define	CALC_XSRC_4	CALC_XSRC_3

#define	CALC_SRC_PTRX_1(NUM)	CALC_XSRC_##NUM; sP0 = sP0_0

#define	CALC_SRC_PTRX_2(NUM)	CALC_SRC_PTRX_1(NUM); sP1 = sP1_0

#define	CALC_SRC_PTRX_3(NUM)	CALC_SRC_PTRX_2(NUM); sP2 = sP2_0

#define	CALC_SRC_PTRX_4(NUM)	CALC_SRC_PTRX_3(NUM); sP3 = sP3_0

#define	GET_SRC(sP, N)	SRC2FP(sP[x_ind[xSrc + N]])

#define	UPDATE_X	x = x_tab[i+1]

#else /* SRC_EXTEND */

#define	DECL_LINE                                                        \
	mlib_s32 i;                                                      \
	mlib_s32 xSrc;                                                   \
	mlib_s32 nchan = ws->nchan;                                      \
	mlib_s32 off = ws->off;                                          \
	mlib_s32 dx = ws->dx;                                            \
	mlib_s32 size = ws->size;                                        \
	mlib_s32 x_shift = ws->x_shift;                                  \
	mlib_s32 xf_shift = ws->xf_shift;                                \
	mlib_s32 xf_mask = ws->xf_mask;                                  \
	mlib_s32 alpha_shift = ((nchan == 3) ? 1 : (ws->alpha_shift));   \
	mlib_s32 filterposx;                                             \
	FILTER_TYPE *fptr;                                               \
	                                                                 \
	DECL_LINE_ADD

#define	DECL_CHAN2	mlib_s32  chan2 = nchan  + nchan;
#define	DECL_CHAN3	mlib_s32  chan3 = chan2 + nchan;

#define	CALC_SRC_PTRY1()	sP0 = (DTYPE*)lineAddr[ySrc + l] + coff

#define	CALC_SRC_PTRY2()                                        \
	CALC_SRC_PTRY1();                                       \
	sP1 = (DTYPE *) lineAddr[ySrc + l + 1] + coff

#define	CALC_SRC_PTRY3()                                        \
	CALC_SRC_PTRY2();                                       \
	sP2 = (DTYPE *) lineAddr[ySrc + l + 2] + coff

#define	CALC_SRC_PTRY4()                                        \
	CALC_SRC_PTRY3();                                       \
	sP3 = (DTYPE *) lineAddr[ySrc + l + 3] + coff

#define	CALC_XSRC_3	xSrc = (x >> x_shift) * 3

#define	CALC_XSRC_4	xSrc = (x >> x_shift) & x_mask
#define	CALC_SRC_PTRX_1(NUM)	CALC_XSRC_##NUM; sP0 = sP0_0 + xSrc

#define	CALC_SRC_PTRX_2(NUM)	CALC_SRC_PTRX_1(NUM); sP1 = sP1_0 + xSrc

#define	CALC_SRC_PTRX_3(NUM)	CALC_SRC_PTRX_2(NUM); sP2 = sP2_0 + xSrc

#define	CALC_SRC_PTRX_4(NUM)	CALC_SRC_PTRX_3(NUM); sP3 = sP3_0 + xSrc

#define	GET_SRC(sP, N)	SRC2FP(sP[CHAN##N])

#define	UPDATE_X	x += dx

#endif /* SRC_EXTEND */

/* *********************************************************** */

#define	MAX_KER	2
#define	MAX_KERH	4

/* *********************************************************** */
#define	CHAN0	0
#define	CHAN1	nchan
#define	CHAN2	chan2
#define	CHAN3	chan3

#if FLT_BITS == 2

/* *********************************************************** */

#define	DECL_YY	d64_2_f32 yyd0, yyd1;

/* *********************************************************** */

#define	DECL_X_SPECIFIC_1	d64_2_f32 xd0; FP_TYPE alp0

/* *********************************************************** */

#define	DECL_X_SPECIFIC_2	DECL_X_SPECIFIC_1; FP_TYPE alp1

/* *********************************************************** */

#define	DECL_X_SPECIFIC_3                                       \
	DECL_X_SPECIFIC_2;                                      \
	d64_2_f32 xd1;                                          \
	DECL_CHAN2 FP_TYPE alp2

/* *********************************************************** */

#define	DECL_X_SPECIFIC_4	DECL_X_SPECIFIC_3; DECL_CHAN3 FP_TYPE alp3

/* *********************************************************** */

#define	CALC_ALPHA_1	alp0 = 255 * xd0.f32s.f0; c0_0 = alp0

/* *********************************************************** */

#define	CALC_ALPHA_2                                            \
	alp0 = 255 * xd0.f32s.f0;                               \
	alp1 = 255 * xd0.f32s.f1;                               \
	c0_0 = alp0 + alp1
/* *********************************************************** */

#define	CALC_ALPHA_3                                            \
	alp0 = 255 * xd0.f32s.f0;                               \
	alp1 = 255 * xd0.f32s.f1;                               \
	alp2 = 255 * xd1.f32s.f0;                               \
	c0_0 = alp0 + alp1 + alp2

/* *********************************************************** */

#define	CALC_ALPHA_4                                            \
	alp0 = 255 * xd0.f32s.f0;                               \
	alp1 = 255 * xd0.f32s.f1;                               \
	alp2 = 255 * xd1.f32s.f0;                               \
	alp3 = 255 * xd1.f32s.f1;                               \
	c0_0 = alp0 + alp1 + alp2 + alp3

/* *********************************************************** */

#define	GET_ALPHA_4(sum, sP)                                    \
	alp0 = GET_SRC(sP, 0) * xd0.f32s.f0;                    \
	alp1 = GET_SRC(sP, 1) * xd0.f32s.f1;                    \
	alp2 = GET_SRC(sP, 2) * xd1.f32s.f0;                    \
	alp3 = GET_SRC(sP, 3) * xd1.f32s.f1;                    \
	sum = alp0 + alp1 + alp2 + alp3

/* *********************************************************** */

#define	GET_ALPHA_3(sum, sP)                                    \
	alp0 = GET_SRC(sP, 0) * xd0.f32s.f0;                    \
	alp1 = GET_SRC(sP, 1) * xd0.f32s.f1;                    \
	alp2 = GET_SRC(sP, 2) * xd1.f32s.f0;                    \
	sum = alp0 + alp1 + alp2

/* *********************************************************** */

#define	GET_ALPHA_2(sum, sP)                                    \
	alp0 = GET_SRC(sP, 0) * xd0.f32s.f0;                    \
	alp1 = GET_SRC(sP, 1) * xd0.f32s.f1;                    \
	sum = alp0 + alp1

/* *********************************************************** */

#define	GET_ALPHA_1(sum, sP)                                    \
	alp0 = GET_SRC(sP, 0) * xd0.f32s.f0;                    \
	sum = alp0

/* *********************************************************** */

#define	LOAD_YBUFF                                              \
	yyd0.d64 = *(mlib_d64 *)(ybuff + 4 * i);                \
	yyd1.d64 = *(mlib_d64 *)(ybuff + 4 * i + 2)

/* *********************************************************** */

#define	SAVE_YBUFF                                              \
	* (mlib_d64 *)(ybuff + 4 * i) = yyd0.d64;               \
	*(mlib_d64 *)(ybuff + 4 * i + 2) = yyd1.d64

/* *********************************************************** */

#define	SUM_Y_1                                                 \
	LOAD_YBUFF;                                             \
	yyd0.f32s.f0 += c0_0 * yf0;                             \
	yyd0.f32s.f1 += c0_1 * yf0;                             \
	yyd1.f32s.f0 += c0_2 * yf0;                             \
	yyd1.f32s.f1 += c0_3 * yf0;                             \
	SAVE_YBUFF

/* *********************************************************** */

#define	SUM_Y_2                                                 \
	LOAD_YBUFF;                                             \
	yyd0.f32s.f0 += c0_0 * yf0 + c1_0 * yf1;                \
	yyd0.f32s.f1 += c0_1 * yf0 + c1_1 * yf1;                \
	yyd1.f32s.f0 += c0_2 * yf0 + c1_2 * yf1;                \
	yyd1.f32s.f1 += c0_3 * yf0 + c1_3 * yf1;                \
	SAVE_YBUFF

/* *********************************************************** */

#define	SUM_Y_3                                                 \
	LOAD_YBUFF;                                             \
	yyd0.f32s.f0 += c0_0 * yf0 + c1_0 * yf1 + c2_0 * yf2;   \
	yyd0.f32s.f1 += c0_1 * yf0 + c1_1 * yf1 + c2_1 * yf2;   \
	yyd1.f32s.f0 += c0_2 * yf0 + c1_2 * yf1 + c2_2 * yf2;   \
	yyd1.f32s.f1 += c0_3 * yf0 + c1_3 * yf1 + c2_3 * yf2;   \
	SAVE_YBUFF

/* *********************************************************** */

#define	SUM_Y_4                                                  \
	LOAD_YBUFF;                                              \
	yyd0.f32s.f0 +=                                          \
	    c0_0 * yf0 + c1_0 * yf1 + c2_0 * yf2 + c3_0 * yf3;   \
	yyd0.f32s.f1 +=                                          \
	    c0_1 * yf0 + c1_1 * yf1 + c2_1 * yf2 + c3_1 * yf3;   \
	yyd1.f32s.f0 +=                                          \
	    c0_2 * yf0 + c1_2 * yf1 + c2_2 * yf2 + c3_2 * yf3;   \
	yyd1.f32s.f1 +=                                          \
	    c0_3 * yf0 + c1_3 * yf1 + c2_3 * yf2 + c3_3 * yf3;   \
	SAVE_YBUFF

/* *********************************************************** */

#else /* FLT_BITS == 2 */

/* *********************************************************** */

#define	DECL_YY

/* *********************************************************** */

#define	DECL_X_SPECIFIC_1	FP_TYPE alp0, xf0

/* *********************************************************** */

#define	DECL_X_SPECIFIC_2	DECL_X_SPECIFIC_1; FP_TYPE alp1, xf1

/* *********************************************************** */

#define	DECL_X_SPECIFIC_3                                       \
	DECL_X_SPECIFIC_2;                                      \
	FP_TYPE alp2, xf2;                                      \
	mlib_s32 chan2 = nchan + nchan

/* *********************************************************** */

#define	DECL_X_SPECIFIC_4                                       \
	DECL_X_SPECIFIC_3;                                      \
	FP_TYPE alp3, xf3;                                      \
	mlib_s32 chan3 = chan2 + nchan

/* *********************************************************** */

#define	CALC_ALPHA_1	alp0 = 255 * xf0; c0_0 = alp0

/* *********************************************************** */

#define	CALC_ALPHA_2                                            \
	alp0 = 255 * xf0;                                       \
	alp1 = 255 * xf1;                                       \
	c0_0 = alp0 + alp1
/* *********************************************************** */

#define	CALC_ALPHA_3                                            \
	alp0 = 255 * xf0;                                       \
	alp1 = 255 * xf1;                                       \
	alp2 = 255 * xf2;                                       \
	c0_0 = alp0 + alp1 + alp2

/* *********************************************************** */

#define	CALC_ALPHA_4                                            \
	alp0 = 255 * xf0;                                       \
	alp1 = 255 * xf1;                                       \
	alp2 = 255 * xf2;                                       \
	alp3 = 255 * xf3;                                       \
	c0_0 = alp0 + alp1 + alp2 + alp3

/* *********************************************************** */

#define	GET_ALPHA_4(sum, sP)                                    \
	alp0 = GET_SRC(sP, 0) * xf0;                            \
	alp1 = GET_SRC(sP, 1) * xf1;                            \
	alp2 = GET_SRC(sP, 2) * xf2;                            \
	alp3 = GET_SRC(sP, 3) * xf3;                            \
	sum = alp0 + alp1 + alp2 + alp3

/* *********************************************************** */

#define	GET_ALPHA_3(sum, sP)                                    \
	alp0 = GET_SRC(sP, 0) * xf0;                            \
	alp1 = GET_SRC(sP, 1) * xf1;                            \
	alp2 = GET_SRC(sP, 2) * xf2;                            \
	sum = alp0 + alp1 + alp2

/* *********************************************************** */

#define	GET_ALPHA_2(sum, sP)                                    \
	alp0 = GET_SRC(sP, 0) * xf0;                            \
	alp1 = GET_SRC(sP, 1) * xf1;                            \
	sum = alp0 + alp1

/* *********************************************************** */

#define	GET_ALPHA_1(sum, sP)	alp0 = GET_SRC(sP, 0) * xf0; sum = alp0

/* *********************************************************** */

#define	SUM_Y_1                                                 \
	ybuff[4 * i + 0] += c0_0 * yf0;                         \
	ybuff[4 * i + 1] += c0_1 * yf0;                         \
	ybuff[4 * i + 2] += c0_2 * yf0;                         \
	ybuff[4 * i + 3] += c0_3 * yf0

/* *********************************************************** */

#define	SUM_Y_2                                                 \
	ybuff[4 * i + 0] += c0_0 * yf0 + c1_0 * yf1;            \
	ybuff[4 * i + 1] += c0_1 * yf0 + c1_1 * yf1;            \
	ybuff[4 * i + 2] += c0_2 * yf0 + c1_2 * yf1;            \
	ybuff[4 * i + 3] += c0_3 * yf0 + c1_3 * yf1

/* *********************************************************** */

#define	SUM_Y_3                                                     \
	ybuff[4 * i + 0] += c0_0 * yf0 + c1_0 * yf1 + c2_0 * yf2;   \
	ybuff[4 * i + 1] += c0_1 * yf0 + c1_1 * yf1 + c2_1 * yf2;   \
	ybuff[4 * i + 2] += c0_2 * yf0 + c1_2 * yf1 + c2_2 * yf2;   \
	ybuff[4 * i + 3] += c0_3 * yf0 + c1_3 * yf1 + c2_3 * yf2

/* *********************************************************** */

#define	SUM_Y_4                                                  \
	ybuff[4 * i + 0] +=                                      \
	    c0_0 * yf0 + c1_0 * yf1 + c2_0 * yf2 + c3_0 * yf3;   \
	ybuff[4 * i + 1] +=                                      \
	    c0_1 * yf0 + c1_1 * yf1 + c2_1 * yf2 + c3_1 * yf3;   \
	ybuff[4 * i + 2] +=                                      \
	    c0_2 * yf0 + c1_2 * yf1 + c2_2 * yf2 + c3_2 * yf3;   \
	ybuff[4 * i + 3] +=                                      \
	    c0_3 * yf0 + c1_3 * yf1 + c2_3 * yf2 + c3_3 * yf3

/* *********************************************************** */

#endif /* FLT_BITS == 2 */

/* *********************************************************** */

#define	MLIB_FILTER_X(x, filter, l)                                   \
	filterposx = (x >> xf_shift) & xf_mask;                       \
	fptr = (FILTER_TYPE *) ((mlib_u8 *)filter + filterposx) + l

/* *********************************************************** */

#define	GET_FILTER_Y()                                            \
	filterposy = (y >> yf_shift) & yf_mask;                   \
	fpy = (FILTER_TYPE *) ((mlib_u8 *)filterY + filterposy)

/* *********************************************************** */

#define	SUM_X_4(sum, sP)                                              \
	sum =                                                         \
	GET_SRC(sP, 0) * alp0 + GET_SRC(sP, 1) * alp1 + GET_SRC(sP,   \
	    2) * alp2 + GET_SRC(sP, 3) * alp3

/* *********************************************************** */

#define	SUM_X_3(sum, sP)                                              \
	sum =                                                         \
	GET_SRC(sP, 0) * alp0 + GET_SRC(sP, 1) * alp1 + GET_SRC(sP,   \
	    2) * alp2

/* *********************************************************** */

#define	SUM_X_2(sum, sP)                                        \
	sum = GET_SRC(sP, 0) * alp0 + GET_SRC(sP, 1) * alp1

/* *********************************************************** */

#define	SUM_X_1(sum, sP)	sum = GET_SRC(sP, 0) * alp0

/* *********************************************************** */

#define	SUM_X_3ch(NUM, LINE)                                    \
	SUM_X_##NUM(c##LINE##_1, sP##LINE);                     \
	sP##LINE++;                                             \
	SUM_X_##NUM(c##LINE##_2, sP##LINE);                     \
	sP##LINE++;                                             \
	SUM_X_##NUM(c##LINE##_3, sP##LINE)

/* *********************************************************** */

#define	SUM_X_4ch(NUM, LINE)                                    \
	GET_ALPHA_##NUM(c##LINE##_0, sP##LINE);                 \
	sP##LINE += alpha_shift;                                \
	SUM_X_3ch(NUM, LINE)

/* *********************************************************** */

#define	DECL_Y_SPECIFIC_1                                       \
	DTYPE *sP0_0 = sP0 + ((alpha_shift == 1) ? 0 : 3);      \
	FP_TYPE c0_0, c0_1, c0_2, c0_3, yf0

/* *********************************************************** */

#define	DECL_Y_SPECIFIC_2                                       \
	DECL_Y_SPECIFIC_1;                                      \
	DTYPE *sP1_0 = sP1 + ((alpha_shift == 1) ? 0 : 3);      \
	FP_TYPE c1_0, c1_1, c1_2, c1_3, yf1

/* *********************************************************** */

#define	DECL_Y_SPECIFIC_3                                       \
	DECL_Y_SPECIFIC_2;                                      \
	DTYPE *sP2_0 = sP2 + ((alpha_shift == 1) ? 0 : 3);      \
	FP_TYPE c2_0, c2_1, c2_2, c2_3, yf2

/* *********************************************************** */

#define	DECL_Y_SPECIFIC_4                                       \
	DECL_Y_SPECIFIC_3;                                      \
	DTYPE *sP3_0 = sP3 + ((alpha_shift == 1) ? 0 : 3);      \
	FP_TYPE c3_0, c3_1, c3_2, c3_3, yf3

/* *********************************************************** */

#define	LOAD_Y_FILTER_1	yf0 = fpyy[0]
#define	LOAD_Y_FILTER_2	LOAD_Y_FILTER_1; yf1 = fpyy[1]
#define	LOAD_Y_FILTER_3	LOAD_Y_FILTER_2; yf2 = fpyy[2]
#define	LOAD_Y_FILTER_4	LOAD_Y_FILTER_3; yf3 = fpyy[3]

#define	LOAD_X_FILTER_1	xd0.d64 = ((mlib_d64*)fptr)[0]
#define	LOAD_X_FILTER_2	LOAD_X_FILTER_1;
#define	LOAD_X_FILTER_3	LOAD_X_FILTER_2; xd1.d64 = ((mlib_d64*)fptr)[1]
#define	LOAD_X_FILTER_4	LOAD_X_FILTER_3;

/* *********************************************************** */

#define	EXPAND_ALPHA_1

#define	EXPAND_ALPHA_2	c1_0 = c0_0;
#define	EXPAND_ALPHA_3	c2_0 = c1_0 = c0_0;
#define	EXPAND_ALPHA_4	c3_0 = c2_0 = c1_0 = c0_0;

/* *********************************************************** */

#define	SUM_X_1_3ch(NUM)	SUM_X_3ch(NUM, 0)

/* *********************************************************** */

#define	SUM_X_2_3ch(NUM)	SUM_X_3ch(NUM, 0); SUM_X_3ch(NUM, 1)

/* *********************************************************** */

#define	SUM_X_3_3ch(NUM)                                        \
	SUM_X_3ch(NUM, 0);                                      \
	SUM_X_3ch(NUM, 1);                                      \
	SUM_X_3ch(NUM, 2)

/* *********************************************************** */

#define	SUM_X_4_3ch(NUM)                                        \
	SUM_X_3ch(NUM, 0);                                      \
	SUM_X_3ch(NUM, 1);                                      \
	SUM_X_3ch(NUM, 2);                                      \
	SUM_X_3ch(NUM, 3)

/* *********************************************************** */

#define	SUM_X_1_4ch(NUM)	SUM_X_4ch(NUM, 0)

/* *********************************************************** */

#define	SUM_X_2_4ch(NUM)	SUM_X_4ch(NUM, 0); SUM_X_4ch(NUM, 1)

/* *********************************************************** */

#define	SUM_X_3_4ch(NUM)                                        \
	SUM_X_4ch(NUM, 0);                                      \
	SUM_X_4ch(NUM, 1);                                      \
	SUM_X_4ch(NUM, 2)

/* *********************************************************** */

#define	SUM_X_4_4ch(NUM)                                        \
	SUM_X_4ch(NUM, 0);                                      \
	SUM_X_4ch(NUM, 1);                                      \
	SUM_X_4ch(NUM, 2);                                      \
	SUM_X_4ch(NUM, 3)

/* *********************************************************** */

#define	FIRST_PART_OF_LOW_FUNCTION(X_POINTS, Y_POINTS)          \
	static void                                             \
	blend_calc_##X_POINTS##_##Y_POINTS                      \
	    (X##Y_POINTS##PARAMS)                               \
	{                                                       \
	    DECL_LINE DECL_YY DECL_X_SPECIFIC_##X_POINTS;       \
	                                                        \
	    DECL_Y_SPECIFIC_##Y_POINTS;                         \
	    LOAD_Y_FILTER_##Y_POINTS;                           \
                                                                \
	    if (nchan == 3) {

/* *********************************************************** */

#define	SECOND_PART_OF_LOW_FUNCTION(X_POINTS, Y_POINTS)         \
		for (i = 0; i < size; i++) {                    \
		    MLIB_FILTER_X(x, filterX, off);             \
		    LOAD_X_FILTER_##X_POINTS;                   \
		    CALC_SRC_PTRX_##Y_POINTS(3);                \
		    CALC_ALPHA_##X_POINTS;                      \
		    EXPAND_ALPHA_##Y_POINTS                     \
		    SUM_X_##Y_POINTS##_3ch(X_POINTS);           \
		    SUM_Y_##Y_POINTS;                           \
		    UPDATE_X;                                   \
		}                                               \
		                                                \
	    } else {

/* *********************************************************** */

#define	THIRD_PART_OF_LOW_FUNCTION(X_POINTS, Y_POINTS)          \
		for (i = 0; i < size; i++) {                    \
		    MLIB_FILTER_X(x, filterX, off);             \
		    LOAD_X_FILTER_##X_POINTS;                   \
		    CALC_SRC_PTRX_##Y_POINTS(4);                \
		    SUM_X_##Y_POINTS##_4ch(X_POINTS);           \
		    SUM_Y_##Y_POINTS;                           \
		    UPDATE_X;                                   \
		}                                               \
	    }                                                   \
	}

/* *********************************************************** */

#define	SAVE_LINE                                               \
	pbuff[4 * i + 0] += c0_0;                               \
	pbuff[4 * i + 1] += c0_1;                               \
	pbuff[4 * i + 2] += c0_2;                               \
	pbuff[4 * i + 3] += c0_3

/* *********************************************************** */

#define	FIRST_PART_OF_LINE_FUNCTION(X_POINTS)                              \
	static void                                                        \
	blend_calc_line_##X_POINTS(XPARAMS)                                \
	{                                                                  \
	    DECL_LINE DTYPE *sP0_0 = sP0 + ((alpha_shift == 1) ? 0 : 3);   \
	    FP_TYPE c0_0, c0_1, c0_2, c0_3;                                \
	                                                                   \
	    DECL_X_SPECIFIC_##X_POINTS;                                    \
                                                                           \
	    if (nchan == 3) {

/* *********************************************************** */

#define	SECOND_PART_OF_LINE_FUNCTION(X_POINTS)                  \
		for (i = 0; i < size; i++) {                    \
		    MLIB_FILTER_X(x, filterX, off);             \
		    LOAD_X_FILTER_##X_POINTS;                   \
		    CALC_SRC_PTRX_1(3);                         \
		    CALC_ALPHA_##X_POINTS;                      \
		    SUM_X_1_3ch(X_POINTS);                      \
		    SAVE_LINE;                                  \
		    UPDATE_X;                                   \
		}                                               \
		                                                \
	    } else {

/* *********************************************************** */

#define	THIRD_PART_OF_LINE_FUNCTION(X_POINTS)                   \
		for (i = 0; i < size; i++) {                    \
		    MLIB_FILTER_X(x, filterX, off);             \
		    LOAD_X_FILTER_##X_POINTS;                   \
		    CALC_SRC_PTRX_1(4);                         \
		    SUM_X_1_4ch(X_POINTS);                      \
		    SAVE_LINE;                                  \
		    UPDATE_X;                                   \
		}                                               \
	    }                                                   \
	}

/* *********************************************************** */

#define	DECLARE_Y_SPECIFIC_1	FP_TYPE *buff0, yf0; d64_2_f32 yd0

/* *********************************************************** */

#define	DECLARE_Y_SPECIFIC_2                                    \
	DECLARE_Y_SPECIFIC_1;                                   \
	FP_TYPE *buff1, yf1;                                    \
	d64_2_f32 yd1

/* *********************************************************** */

#define	DECLARE_Y_SPECIFIC_3                                    \
	DECLARE_Y_SPECIFIC_2;                                   \
	FP_TYPE *buff2, yf2;                                    \
	d64_2_f32 yd2

/* *********************************************************** */

#define	DECLARE_Y_SPECIFIC_4                                    \
	DECLARE_Y_SPECIFIC_3;                                   \
	FP_TYPE *buff3, yf3;                                    \
	d64_2_f32 yd3

/* *********************************************************** */

#define	LOAD_POINTER_1	buff0 = buff_arr[bnd]
#define	LOAD_POINTER_2	LOAD_POINTER_1; buff1 = buff_arr[bnd + 1]
#define	LOAD_POINTER_3	LOAD_POINTER_2; buff2 = buff_arr[bnd + 2]
#define	LOAD_POINTER_4	LOAD_POINTER_3; buff3 = buff_arr[bnd + 3]

/* *********************************************************** */

#define	LOAD_FROM_BUFFER_1                                      \
	yy.d64 = ((mlib_d64 *)ybuff)[i];                        \
	yd0.d64 = ((mlib_d64 *)buff0)[i]

/* *********************************************************** */

#define	LOAD_FROM_BUFFER_2                                      \
	LOAD_FROM_BUFFER_1;                                     \
	yd1.d64 = ((mlib_d64 *)buff1)[i]

/* *********************************************************** */

#define	LOAD_FROM_BUFFER_3                                      \
	LOAD_FROM_BUFFER_2;                                     \
	yd2.d64 = ((mlib_d64 *)buff2)[i]

/* *********************************************************** */

#define	LOAD_FROM_BUFFER_4                                      \
	LOAD_FROM_BUFFER_3;                                     \
	yd3.d64 = ((mlib_d64 *)buff3)[i]

/* *********************************************************** */

#define	SUM_Y_LINE_1                                            \
	yy.f32s.f0 += yd0.f32s.f0 * yf0;                        \
	yy.f32s.f1 += yd0.f32s.f1 * yf0

/* *********************************************************** */

#define	SUM_Y_LINE_2                                            \
	yy.f32s.f0 += yd0.f32s.f0 * yf0 + yd1.f32s.f0 * yf1;    \
	yy.f32s.f1 += yd0.f32s.f1 * yf0 + yd1.f32s.f1 * yf1

/* *********************************************************** */

#define	SUM_Y_LINE_3                                                     \
	yy.f32s.f0 +=                                                    \
	    yd0.f32s.f0 * yf0 + yd1.f32s.f0 * yf1 + yd2.f32s.f0 * yf2;   \
	yy.f32s.f1 +=                                                    \
	    yd0.f32s.f1 * yf0 + yd1.f32s.f1 * yf1 + yd2.f32s.f1 * yf2

/* *********************************************************** */

#define	SUM_Y_LINE_4                                                      \
	yy.f32s.f0 +=                                                     \
	    yd0.f32s.f0 * yf0 + yd1.f32s.f0 * yf1 + yd2.f32s.f0 * yf2 +   \
	    yd3.f32s.f0 * yf3;                                            \
	yy.f32s.f1 +=                                                     \
	    yd0.f32s.f1 * yf0 + yd1.f32s.f1 * yf1 + yd2.f32s.f1 * yf2 +   \
	    yd3.f32s.f1 * yf3

/* *********************************************************** */

#define	FIRST_PART_OF_Y_SUM_FUNCTION(Y_POINTS)                    \
	static void                                               \
	blend_mlib_calc##Y_POINTS(FILTER_TYPE * fpyy,             \
	    FP_TYPE ** buff_arr, mlib_s32 bnd, FP_TYPE * ybuff,   \
	    mlib_zoom_workspace * ws)                             \
	{                                                         \
	    mlib_s32 i;                                           \
	    mlib_s32 size = ws->size;                             \
	    d64_2_f32 yy;                                         \
	                                                          \
	    DECLARE_Y_SPECIFIC_##Y_POINTS;                        \
	    LOAD_Y_FILTER_##Y_POINTS;                             \
	    LOAD_POINTER_##Y_POINTS;

/* *********************************************************** */

#define	SECOND_PART_OF_Y_SUM_FUNCTION(Y_POINTS)                 \
	    for (i = 0; i < 2 * size; i++) {                    \
		LOAD_FROM_BUFFER_##Y_POINTS;                    \
		SUM_Y_LINE_##Y_POINTS;                          \
		((mlib_d64 *)ybuff)[i] = yy.d64;                \
	    }                                                   \
	}

/* *********************************************************** */

    FIRST_PART_OF_LOW_FUNCTION(1, 1)
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
    SECOND_PART_OF_LOW_FUNCTION(1, 1)
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
    THIRD_PART_OF_LOW_FUNCTION(1, 1)

/* *********************************************************** */

    FIRST_PART_OF_LOW_FUNCTION(2, 1)
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
    SECOND_PART_OF_LOW_FUNCTION(2, 1)
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
    THIRD_PART_OF_LOW_FUNCTION(2, 1)

/* *********************************************************** */

    FIRST_PART_OF_LOW_FUNCTION(1, 2)
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
    SECOND_PART_OF_LOW_FUNCTION(1, 2)
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
    THIRD_PART_OF_LOW_FUNCTION(1, 2)

/* *********************************************************** */

    FIRST_PART_OF_LOW_FUNCTION(2, 2)
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
    SECOND_PART_OF_LOW_FUNCTION(2, 2)
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
    THIRD_PART_OF_LOW_FUNCTION(2, 2)

/* *********************************************************** */

    FIRST_PART_OF_LINE_FUNCTION(1)
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
    SECOND_PART_OF_LINE_FUNCTION(1)
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
    THIRD_PART_OF_LINE_FUNCTION(1)

/* *********************************************************** */

    FIRST_PART_OF_LINE_FUNCTION(2)
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
    SECOND_PART_OF_LINE_FUNCTION(2)
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
    THIRD_PART_OF_LINE_FUNCTION(2)

/* *********************************************************** */

    FIRST_PART_OF_LINE_FUNCTION(3)
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
    SECOND_PART_OF_LINE_FUNCTION(3)
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
    THIRD_PART_OF_LINE_FUNCTION(3)

/* *********************************************************** */

    FIRST_PART_OF_LINE_FUNCTION(4)
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
    SECOND_PART_OF_LINE_FUNCTION(4)
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
    THIRD_PART_OF_LINE_FUNCTION(4)

/* *********************************************************** */

    FIRST_PART_OF_Y_SUM_FUNCTION(1)
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
    SECOND_PART_OF_Y_SUM_FUNCTION(1)

/* *********************************************************** */

    FIRST_PART_OF_Y_SUM_FUNCTION(2)
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
    SECOND_PART_OF_Y_SUM_FUNCTION(2)

/* *********************************************************** */

    FIRST_PART_OF_Y_SUM_FUNCTION(3)
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
    SECOND_PART_OF_Y_SUM_FUNCTION(3)

/* *********************************************************** */

    FIRST_PART_OF_Y_SUM_FUNCTION(4)
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
    SECOND_PART_OF_Y_SUM_FUNCTION(4)

/* *********************************************************** */

#define	SCALE	(((FP_TYPE)1)/((FP_TYPE)255))

/* *********************************************************** */

#define	DECLARE_LOAD_SRC                                        \
	FP_TYPE a0 = ybuff[4 * i];                              \
	FP_TYPE sr = ybuff[4 * i + 1];                          \
	FP_TYPE sg = ybuff[4 * i + 2];                          \
	FP_TYPE sb = ybuff[4 * i + 3]

#define	DECLARE_LOAD_4(dst0, dst1, dst2, dst3)                  \
	DECLARE_LOAD_SRC;                                       \
	FP_TYPE dst0 = SRC2FP(dPtr[4 * i]);                     \
	FP_TYPE dst1 = SRC2FP(dPtr[4 * i + 1]);                 \
	FP_TYPE dst2 = SRC2FP(dPtr[4 * i + 2]);                 \
	FP_TYPE dst3 = SRC2FP(dPtr[4 * i + 3]);                 \
	FP_TYPE scale;                                          \
	mlib_s32 ialp

#define	CALC_SCALE_ALPHA_4                                      \
	a1 = (255 - a0) * a1 * SCALE;                           \
	ialp = (mlib_s32)(a0 + a1 + (FP_TYPE) 0.5);             \
	FP_INV(scale, (a0 + a1))

#define	BUF_INI                                                 \
	ybuff[4 * i] = SAT_OFF;                                 \
	ybuff[4 * i + 1] = SAT_OFF;                             \
	ybuff[4 * i + 2] = SAT_OFF;                             \
	ybuff[4 * i + 3] = SAT_OFF

#define	BLEND_SRC_OVER(dst, src0, src1)                              \
	dst = (DTYPE) ((src0 + a1 * src1) * scale + (FP_TYPE) 0.5)

#define	BLEND_SRC_OVER2(dst, src0, src1)                                   \
	dst =                                                              \
	    (DTYPE) ((src0 - a0 * src1) * scale + (FP_TYPE) 0.5 + src1);

#define	SAT(src, min, max)                                      \
	if (src <= min)                                         \
	    src = min;                                          \
	if (src >= max)                                         \
	    src = max

/* *********************************************************** */

#define	SATURATION                                              \
	SAT(a0, MLIB_U8_MIN, MLIB_U8_MAX);                      \
	SAT(sr, MLIB_U8_MIN, MLIB_U8_MAX * MLIB_U8_MAX);        \
	SAT(sg, MLIB_U8_MIN, MLIB_U8_MAX * MLIB_U8_MAX);        \
	SAT(sb, MLIB_U8_MIN, MLIB_U8_MAX * MLIB_U8_MAX);
/* *********************************************************** */

    mlib_status FUNC_NAME(
    PARAMS)
{
	FP_TYPE *buff;
	mlib_d64 zoomy = ws->zoomy;
	mlib_s32 max_xsize = ws->max_xsize;
	mlib_s32 low;
	mlib_s32 xLeft, xRight = 0, x0, y0, ynew;
	mlib_s32 ySrc;
	mlib_s32 yStart = ws->yStart;
	mlib_s32 yFinish = ws->yFinish;
	mlib_s32 dstYStride = ws->dstStride;
	mlib_s32 nchan = ws->nchan;
	mlib_s32 x_shift = ws->x_shift;
	mlib_s32 y_shift = ws->y_shift;
	mlib_s32 filterposy, sbits, x_mask;
	mlib_s32 size;
	DTYPE *sP0, *dstPixelPtr;
	mlib_s32 xf_shift, xf_mask;
	mlib_s32 m = table->width;
	mlib_s32 n = table->height;
	mlib_s32 xf_bits = table->width_bits;
	mlib_s32 yf_bits = table->height_bits;
	mlib_s32 subsampleBitsH = table->subsampleBitsH;
	mlib_s32 subsampleBitsV = table->subsampleBitsV;
	FILTER_TYPE *filterX, *filterY, *fpy;
	mlib_s32 i, j, k, l, kw, kh, off;

	mlib_s32 yf_shift, yf_mask;
	FP_TYPE **buff_arr = NULL, *pbuff, *ybuff;
	DTYPE *dPtr;
	mlib_s32 bind, y_step, y, x;

#ifndef SRC_EXTEND
	mlib_s32 coff;
#else /* SRC_EXTEND */
	mlib_s32 *x_tab;
#endif /* SRC_EXTEND */

	DTYPE *sP1;
	FILTER_TYPE *fpyy;

	max_xsize = (max_xsize + 1) & ~1;
	ws->max_xsize = max_xsize;

	if (n * zoomy > 1) {
		buff =
		    __mlib_malloc((n + 1) * 4 * max_xsize * sizeof (FP_TYPE) +
		    2 * n * sizeof (FP_TYPE *));
		low = 0;
		if (buff == NULL) {
			buff = __mlib_malloc(4 * max_xsize * sizeof (FP_TYPE));
			low = 1;
		}
	} else {
		buff = __mlib_malloc(4 * max_xsize * sizeof (FP_TYPE));
		low = 1;
	}

	if (buff == NULL)
		return (MLIB_FAILURE);

	if (low) {
		ybuff = buff;
	} else {
		ybuff = buff + n * 4 * max_xsize;
		buff_arr = (FP_TYPE **) (ybuff + 4 * max_xsize);

		for (k = 0; k < n; k++)
			buff_arr[k] = buff + k * 4 * max_xsize;
		for (k = 0; k < n; k++)
			buff_arr[n + k] = buff_arr[k];
	}

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < 4 * max_xsize; i++) {
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
		sbits = 0;
		break;
	case 4:
		sbits = 2;
		break;
	default:
		sbits = 0;
	}

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

	y_step = n;
	bind = 0;

#ifndef SRC_EXTEND
	xLeft = leftEdges[yStart];
	xRight = rightEdges[yStart];
	if (xLeft > xRight) {
		__mlib_free(buff);
		return (MLIB_SUCCESS);
	}
	size = xRight - xLeft + 1;
	x0 = xStarts[yStart];

/* move to kernel center */
	x0 -= ws->x_move;
#else /* SRC_EXTEND */
	size = leftEdges[0];
	xLeft = leftEdges[1];
	if (size <= 0) {
		__mlib_free(buff);
		return (MLIB_SUCCESS);
	}
	x_tab = rightEdges;
	x0 = x_tab[0];
#endif /* SRC_EXTEND */

	dstPixelPtr = (DTYPE *) dstData + (ws->dchan) * xLeft;

	ws->size = size;

	ws->x0 = x0;

	for (j = yStart; j <= yFinish; j++) {
		CLIP;

		y = y0 - ws->y_move;
		GET_FILTER_Y();

		ySrc = y >> y_shift;

		ws->y0 = y0;

		if (low) {
			for (l = 0; l < n; l += kh) {

				kh = n - l;

				if (kh > MAX_KER)
					kh = MAX_KER;
				fpyy = fpy + l;

				if (kh == 2) {

/* offset in current kernel line */
					for (off = 0; off < m; off += kw) {
						x = x0;
						ws->off = off;
#ifndef SRC_EXTEND
						coff = CHAN1 * off;
#endif /* SRC_EXTEND */
						CALC_SRC_PTRY2();

						kw = m - off;

						if (kw > MAX_KER)
							kw = MAX_KER;

						if (kw == 2) {
							blend_calc_2_2
							    (CALL_X1PARAMS,
							    sP1);

						} else {    /* if (kw==1) */

							blend_calc_1_2
							    (CALL_X1PARAMS,
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
						coff = CHAN1 * off;
#endif /* SRC_EXTEND */
						CALC_SRC_PTRY1();

						kw = m - off;

						if (kw > MAX_KER)
							kw = MAX_KER;

						if (kw == 2) {
							blend_calc_2_1
							    (CALL_X1PARAMS);

						} else {	/* if (kw==1) */

							blend_calc_1_1
							    (CALL_X1PARAMS);
						}
					}
				}
			}
		} else {

			ynew = (ynew - ws->y_move) >> y_shift;
			if (y_step >= n) {
				y_step = n;
			} else {
				bind += y_step;

				if (bind >= n)
					bind -= n;
			}

			ws->y_step = y_step;
			for (l = n - y_step; l < n; l++) {

				pbuff = buff_arr[bind + l];

				for (i = 0; i < 4 * size; i++) {
					pbuff[i] = SAT_OFF;
				}

/* offset in current kernel line */
				for (off = 0; off < m; off += kw) {
					x = x0;

					ws->off = off;
#ifndef SRC_EXTEND
					coff = CHAN1 * off;
#endif /* SRC_EXTEND */
					CALC_SRC_PTRY1();

					kw = m - off;

					if (kw >= MAX_KERH)
						kw = MAX_KERH;

					if (kw == 4) {
						blend_calc_line_4(CALL_XPARAMS);

					} else if (kw == 3) {
						blend_calc_line_3(CALL_XPARAMS);

					} else if (kw == 2) {
						blend_calc_line_2(CALL_XPARAMS);

					} else {	/* if (kw == 1) */

						blend_calc_line_1(CALL_XPARAMS);
					}
				}
			}

			for (l = 0; l < n; l += kh) {
				kh = n - l;

				if (kh >= MAX_KERH)
					kh = MAX_KERH;

				if (kh == 4) {
					blend_mlib_calc4(fpy + l, buff_arr,
					    bind + l, ybuff, ws);

				} else if (kh == 3) {
					blend_mlib_calc3(fpy + l, buff_arr,
					    bind + l, ybuff, ws);

				} else if (kh == 2) {
					blend_mlib_calc2(fpy + l, buff_arr,
					    bind + l, ybuff, ws);

				} else {
/* kh == 1 */
					blend_mlib_calc1(fpy + l, buff_arr,
					    bind + l, ybuff, ws);
				}
			}

			y_step = ynew - ySrc;
		}

		dPtr = dstPixelPtr;

		if (ws->blend != MLIB_BLEND_GTK_SRC) {
			if (ws->dchan == 3) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i < size; i++) {
					DECLARE_LOAD_SRC;
					FP_TYPE dr = SRC2FP(dPtr[3 * i]);
					FP_TYPE dg = SRC2FP(dPtr[3 * i + 1]);
					FP_TYPE db = SRC2FP(dPtr[3 * i + 2]);
					FP_TYPE a1;
					FP_TYPE scale = SCALE;

					SATURATION a1 = 255 - a0;

					BLEND_SRC_OVER(dPtr[3 * i + 0], sr, dr);
					BLEND_SRC_OVER(dPtr[3 * i + 1], sg, dg);
					BLEND_SRC_OVER(dPtr[3 * i + 2], sb, db);

					BUF_INI;
				}
			} else {
				if (ws->blend == MLIB_BLEND_GTK_SRC_OVER) {
					if (ws->alpha_shift == 1) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
						for (i = 0; i < size; i++) {
							DECLARE_LOAD_4(a1, dr,
							    dg, db);

							SATURATION
							    CALC_SCALE_ALPHA_4;

							dPtr[4 * i] = ialp;
							BLEND_SRC_OVER(dPtr[4 *
							    i + 1], sr, dr);
							BLEND_SRC_OVER(dPtr[4 *
							    i + 2], sg, dg);
							BLEND_SRC_OVER(dPtr[4 *
							    i + 3], sb, db);

							BUF_INI;
						}
					} else {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
						for (i = 0; i < size; i++) {
							DECLARE_LOAD_4(dr, dg,
							    db, a1);

							SATURATION
							    CALC_SCALE_ALPHA_4;

							BLEND_SRC_OVER(dPtr[4 *
							    i + 0], sr, dr);
							BLEND_SRC_OVER(dPtr[4 *
							    i + 1], sg, dg);
							BLEND_SRC_OVER(dPtr[4 *
							    i + 2], sb, db);
							dPtr[4 * i + 3] = ialp;

							BUF_INI;
						}
					}
				} else {
					if (ws->alpha_shift == 1) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
						for (i = 0; i < size; i++) {
							DECLARE_LOAD_4(a1, dr,
							    dg, db);

							SATURATION
							    CALC_SCALE_ALPHA_4;

							dPtr[4 * i] = ialp;
							BLEND_SRC_OVER2(dPtr[4 *
							    i + 1], sr, dr);
							BLEND_SRC_OVER2(dPtr[4 *
							    i + 2], sg, dg);
							BLEND_SRC_OVER2(dPtr[4 *
							    i + 3], sb, db);

							BUF_INI;
						}
					} else {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
						for (i = 0; i < size; i++) {
							DECLARE_LOAD_4(dr, dg,
							    db, a1);

							SATURATION
							    CALC_SCALE_ALPHA_4;

							BLEND_SRC_OVER2(dPtr[4 *
							    i + 0], sr, dr);
							BLEND_SRC_OVER2(dPtr[4 *
							    i + 1], sg, dg);
							BLEND_SRC_OVER2(dPtr[4 *
							    i + 2], sb, db);
							dPtr[4 * i + 3] = ialp;

							BUF_INI;
						}
					}
				}
			}
		} else {
			if (ws->dchan == 3) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i < size; i++) {
					FP_TYPE sr = ybuff[4 * i + 1];
					FP_TYPE sg = ybuff[4 * i + 2];
					FP_TYPE sb = ybuff[4 * i + 3];

					SAT(sr, MLIB_U8_MIN,
					    MLIB_U8_MAX * MLIB_U8_MAX);
					SAT(sg, MLIB_U8_MIN,
					    MLIB_U8_MAX * MLIB_U8_MAX);
					SAT(sb, MLIB_U8_MIN,
					    MLIB_U8_MAX * MLIB_U8_MAX);

					dPtr[3 * i] =
					    (DTYPE) (sr * SCALE +
					    (FP_TYPE) 0.5);
					dPtr[3 * i + 1] =
					    (DTYPE) (sg * SCALE +
					    (FP_TYPE) 0.5);
					dPtr[3 * i + 2] =
					    (DTYPE) (sb * SCALE +
					    (FP_TYPE) 0.5);

					BUF_INI;
				}
			} else {
				if (nchan == 3) {
					if (ws->alpha_shift == 1) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
						for (i = 0; i < size; i++) {
							FP_TYPE sr =
							    ybuff[4 * i + 1];
							FP_TYPE sg =
							    ybuff[4 * i + 2];
							FP_TYPE sb =
							    ybuff[4 * i + 3];

							SAT(sr, MLIB_U8_MIN,
							    MLIB_U8_MAX *
							    MLIB_U8_MAX);
							SAT(sg, MLIB_U8_MIN,
							    MLIB_U8_MAX *
							    MLIB_U8_MAX);
							SAT(sb, MLIB_U8_MIN,
							    MLIB_U8_MAX *
							    MLIB_U8_MAX);

							dPtr[4 * i] = 255;
							dPtr[4 * i + 1] =
							    (DTYPE) (sr *
							    SCALE +
							    (FP_TYPE) 0.5);
							dPtr[4 * i + 2] =
							    (DTYPE) (sg *
							    SCALE +
							    (FP_TYPE) 0.5);
							dPtr[4 * i + 3] =
							    (DTYPE) (sb *
							    SCALE +
							    (FP_TYPE) 0.5);

							BUF_INI;
						}
					} else {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
						for (i = 0; i < size; i++) {
							FP_TYPE sr =
							    ybuff[4 * i + 1];
							FP_TYPE sg =
							    ybuff[4 * i + 2];
							FP_TYPE sb =
							    ybuff[4 * i + 3];

							SAT(sr, MLIB_U8_MIN,
							    MLIB_U8_MAX *
							    MLIB_U8_MAX);
							SAT(sg, MLIB_U8_MIN,
							    MLIB_U8_MAX *
							    MLIB_U8_MAX);
							SAT(sb, MLIB_U8_MIN,
							    MLIB_U8_MAX *
							    MLIB_U8_MAX);

							dPtr[4 * i] =
							    (DTYPE) (sr *
							    SCALE +
							    (FP_TYPE) 0.5);
							dPtr[4 * i + 1] =
							    (DTYPE) (sg *
							    SCALE +
							    (FP_TYPE) 0.5);
							dPtr[4 * i + 2] =
							    (DTYPE) (sb *
							    SCALE +
							    (FP_TYPE) 0.5);
							dPtr[4 * i + 3] = 255;

							BUF_INI;
						}
					}
				} else {
					if (ws->alpha_shift == 1) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
						for (i = 0; i < size; i++) {
							DECLARE_LOAD_SRC;
							FP_TYPE scale;
							mlib_s32 ialp;

							SATURATION
							    ialp =
							    (mlib_s32)(a0 +
							    (FP_TYPE) 0.5);
							FP_INV(scale, a0);

							dPtr[4 * i] = ialp;
							dPtr[4 * i + 1] =
							    (DTYPE) (sr *
							    scale +
							    (FP_TYPE) 0.5);
							dPtr[4 * i + 2] =
							    (DTYPE) (sg *
							    scale +
							    (FP_TYPE) 0.5);
							dPtr[4 * i + 3] =
							    (DTYPE) (sb *
							    scale +
							    (FP_TYPE) 0.5);

							BUF_INI;
						}
					} else {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
						for (i = 0; i < size; i++) {
							DECLARE_LOAD_SRC;
							FP_TYPE scale;
							mlib_s32 ialp;

							SATURATION
							    ialp =
							    (mlib_s32)(a0 +
							    (FP_TYPE) 0.5);
							FP_INV(scale, a0);

							dPtr[4 * i] =
							    (DTYPE) (sr *
							    scale +
							    (FP_TYPE) 0.5);
							dPtr[4 * i + 1] =
							    (DTYPE) (sg *
							    scale +
							    (FP_TYPE) 0.5);
							dPtr[4 * i + 2] =
							    (DTYPE) (sb *
							    scale +
							    (FP_TYPE) 0.5);
							dPtr[4 * i + 3] = ialp;

							BUF_INI;
						}
					}
				}
			}
		}
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

#endif /* _NO_LONGLONG */

#ifdef __cplusplus
}
#endif

#endif /* _MLIB_I_IMAGEZOOMTRANSTABLEBLEND_F_H */
