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

#pragma ident	"@(#)mlib_ImageZoomIn2X_BL_S32.c	9.3	07/11/05"

/*
 * FUNCTION
 *      mlib_ImageZoomIn2X -  image 2X scaling with edge condition
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageZoomIn2X(mlib_image       *dst,
 *                                     const mlib_image *src,
 *                                     mlib_filter      filter,
 *                                     mlib_edge        edge)
 *
 * ARGUMENTS
 *      dst       Pointer to destination image.
 *      src       Pointer to source image.
 *      filter    Type of resampling filter.
 *      edge      Type of edge condition.
 *
 * DESCRIPTION
 *  The center of the source image is mapped to the center of the
 *  destination image.
 *
 *  The upper-left corner pixel of an image is located at (0.5, 0.5).
 *
 *  The resampling filter can be one of the following:
 *    MLIB_NEAREST
 *    MLIB_BILINEAR
 *    MLIB_BICUBIC
 *
 *  The edge condition can be one of the following:
 *    MLIB_EDGE_DST_NO_WRITE  (default)
 *    MLIB_EDGE_DST_FILL_ZERO
 *    MLIB_EDGE_OP_NEAREST
 *    MLIB_EDGE_SRC_EXTEND
 */

#include <mlib_image.h>
#include <mlib_ImageZoomIn2X.h>

/* *********************************************************** */

#define	MLIB_ZOOMIN2X_UL_FL0	(((TTYPE)9.0*a00 + (TTYPE)3.0*a01 + \
				(TTYPE)3.0*a10 + a11) * (TTYPE)0.0625)
#define	MLIB_ZOOMIN2X_UR_FL0	(((TTYPE)3.0*a00 + (TTYPE)9.0*a01 + \
				a10 + (TTYPE)3.0*a11) * (TTYPE)0.0625)

#define	MLIB_ZOOMIN2X_DL_FL0	(((TTYPE)3.0*a00 + a01 + (TTYPE)9.0*a10 + \
				(TTYPE)3.0*a11) * (TTYPE)0.0625)

#define	MLIB_ZOOMIN2X_DR_FL0	((a00 + (TTYPE)3.0*a01 + (TTYPE)3.0*a10 + \
				(TTYPE)9.0*a11) * (TTYPE)0.0625)

/* *********************************************************** */

#define	MLIB_ZOOMIN2X_S32_UL_FL0	(((TTYPE)9*a00 + (TTYPE)3*a01 + \
				(TTYPE)3*a10 + a11) >> 4)
#define	MLIB_ZOOMIN2X_S32_UR_FL0	(((TTYPE)3*a00 + (TTYPE)9*a01 + \
				a10 + (TTYPE)3*a11) >> 4)

#define	MLIB_ZOOMIN2X_S32_DL_FL0	(((TTYPE)3*a00 + a01 + \
				(TTYPE)9*a10 + (TTYPE)3*a11) >> 4)

#define	MLIB_ZOOMIN2X_S32_DR_FL0	((a00 + (TTYPE)3*a01 + \
				(TTYPE)3*a10 + (TTYPE)9*a11) >> 4)

/* *********************************************************** */

#define	MLIB_ZOOMIN2X_UL_FL1	(((TTYPE)3.0*a00 + a01) * (TTYPE)0.25)
#define	MLIB_ZOOMIN2X_UR_FL1	((a00 + (TTYPE)3.0*a01) * (TTYPE)0.25)

#define	MLIB_ZOOMIN2X_DL_FL1	(((TTYPE)3.0*a00 + a01 + (TTYPE)3.0*a10 + \
				a11) * (TTYPE)0.125)

#define	MLIB_ZOOMIN2X_DR_FL1	((a00 + (TTYPE)3.0*a01 + a10 +\
				(TTYPE)3.0*a11) * (TTYPE)0.125)

/* *********************************************************** */

#define	MLIB_ZOOMIN2X_S32_UL_FL1	(((TTYPE)3*a00 + a01) >> 2)
#define	MLIB_ZOOMIN2X_S32_UR_FL1	((a00 + (TTYPE)3*a01) >> 2)

#define	MLIB_ZOOMIN2X_S32_DL_FL1	(((TTYPE)3*a00 + a01 + (TTYPE)3*a10 + \
				a11) >> 3)

#define	MLIB_ZOOMIN2X_S32_DR_FL1	((a00 + (TTYPE)3*a01 + a10 +\
				(TTYPE)3*a11) >> 3)

/* *********************************************************** */

#define	MLIB_ZOOMIN2X_UL_FL2	(((TTYPE)3.0*a00 + a10) * (TTYPE)0.25)
#define	MLIB_ZOOMIN2X_UR_FL2	(((TTYPE)3.0*a00 + (TTYPE)3.0*a01 + a10 + \
				a11) * (TTYPE)0.125)

#define	MLIB_ZOOMIN2X_DL_FL2	((a00 + (TTYPE)3.0*a10) * (TTYPE)0.25)

#define	MLIB_ZOOMIN2X_DR_FL2	((a00 + a01 + (TTYPE)3.0*a10 + \
				(TTYPE)3.0*a11) * (TTYPE)0.125)

/* *********************************************************** */

#define	MLIB_ZOOMIN2X_S32_UL_FL2	(((TTYPE)3*a00 + a10) >> 2)
#define	MLIB_ZOOMIN2X_S32_UR_FL2	(((TTYPE)3*a00 + (TTYPE)3*a01 + a10 + \
				a11) >> 3)

#define	MLIB_ZOOMIN2X_S32_DL_FL2	((a00 + (TTYPE)3*a10) >> 2)

#define	MLIB_ZOOMIN2X_S32_DR_FL2	((a00 + a01 + (TTYPE)3*a10 + \
				(TTYPE)3*a11) >> 3)

/* *********************************************************** */

#define	MLIB_ZOOMIN2X_UL_FL3	a00
#define	MLIB_ZOOMIN2X_UR_FL3	((a00 + a01) * (TTYPE)0.5)

#define	MLIB_ZOOMIN2X_DL_FL3	((a00 + a10) * (TTYPE)0.5)

#define	MLIB_ZOOMIN2X_DR_FL3	((a00 + a01 + a10 + a11) * (TTYPE)0.25)

/* *********************************************************** */

#define	MLIB_ZOOMIN2X_S32_UL_FL3	a00
#define	MLIB_ZOOMIN2X_S32_UR_FL3	((a00 + a01) >> 1)

#define	MLIB_ZOOMIN2X_S32_DL_FL3	((a00 + a10) >> 1)

#define	MLIB_ZOOMIN2X_S32_DR_FL3	((a00 + a01 + a10 + a11) >> 2)

/* *********************************************************** */

#define	INIT_FL0()                                              \
	TTYPE a0, a1, a13, a31, a39, a93;                       \
	TTYPE b0, b1, b13, b31, b39, b93;                       \
	                                                        \
	a0 = sp[0];                                             \
	a1 = sp[src_stride];                                    \
	a13 = a0 + (TTYPE)                                      \
	    3.0 *a1;                                            \
	                                                        \
	a31 = (TTYPE)                                           \
	    3.0 *a0 + a1;                                       \
	                                                        \
	a39 = (TTYPE)                                           \
	    3.0 *a13;                                           \
	                                                        \
	a93 = (TTYPE)                                           \
	    3.0 *a31

/* *********************************************************** */

#define	INIT_S32_FL0()                                          \
	TTYPE a0, a1, a13, a31, a39, a93;                       \
	TTYPE b0, b1, b13, b31, b39, b93;                       \
	                                                        \
	a0 = sp[0];                                             \
	a1 = sp[src_stride];                                    \
	a13 = a0 + (TTYPE)                                      \
	    3 *a1;                                              \
	                                                        \
	a31 = (TTYPE)                                           \
	    3 *a0 + a1;                                         \
	                                                        \
	a39 = (TTYPE)                                           \
	    3 *a13;                                             \
	                                                        \
	a93 = (TTYPE)                                           \
	    3 *a31

/* *********************************************************** */

#define	ZOOM_FL0(A, B, STYPE)                                   \
	sp += channels;                                         \
	B##0 = sp[0];                                           \
	B##1 = sp[src_stride];                                  \
	B##13 = B##0 + (TTYPE)                                  \
	    3.0 *B##1;                                          \
	                                                        \
	B##31 = (TTYPE)                                         \
	    3.0 *B##0 + B##1;                                   \
	                                                        \
	B##39 = (TTYPE)                                         \
	    3.0 *B##13;                                         \
	                                                        \
	B##93 = (TTYPE)                                         \
	    3.0 *B##31;                                         \
	                                                        \
	dp[0] = (STYPE) ((A##93 + B##31) * (TTYPE) 0.0625);     \
	dp[channels] =                                          \
		(STYPE) ((A##31 + B##93) * (TTYPE) 0.0625);     \
	dp[dst_stride] =                                        \
		(STYPE) ((A##39 + B##13) * (TTYPE) 0.0625);     \
	dp[dst_stride + channels] =                             \
		(STYPE) ((A##13 + B##39) * (TTYPE) 0.0625);     \
	dp += 2 * channels

/* *********************************************************** */

#define	ZOOM_S32_FL0(A, B, STYPE)                               \
	sp += channels;                                         \
	B##0 = sp[0];                                           \
	B##1 = sp[src_stride];                                  \
	B##13 = B##0 + (TTYPE)                                  \
	    3 *B##1;                                            \
	                                                        \
	B##31 = (TTYPE)                                         \
	    3 *B##0 + B##1;                                     \
	                                                        \
	B##39 = (TTYPE)                                         \
	    3 *B##13;                                           \
	                                                        \
	B##93 = (TTYPE)                                         \
	    3 *B##31;                                           \
	                                                        \
	dp[0] = (STYPE) ((A##93 + B##31) >> 4);                 \
	dp[channels] =                                          \
		(STYPE) ((A##31 + B##93) >> 4);                 \
	dp[dst_stride] =                                        \
		(STYPE) ((A##39 + B##13) >> 4);                 \
	dp[dst_stride + channels] =                             \
		(STYPE) ((A##13 + B##39) >> 4);                 \
	dp += 2 * channels

/* *********************************************************** */

#define	INIT_FL1()                                              \
	TTYPE a0, a1, a13, a31;                                 \
	TTYPE b0, b1, b13, b31;                                 \
	                                                        \
	a0 = sp[0];                                             \
	a1 = sp[src_stride]

/* *********************************************************** */

#define	INIT_S32_FL1()                                          \
	TTYPE a0, a1, a13, a31;                                 \
	TTYPE b0, b1, b13, b31;                                 \
	                                                        \
	a0 = sp[0];                                             \
	a1 = sp[src_stride]

/* *********************************************************** */

#define	ZOOM_FL1(A, B, STYPE)                                   \
	sp += channels;                                         \
	B##0 = sp[0];                                           \
	B##1 = sp[src_stride];                                  \
	A##13 = A##0 + (TTYPE)                                  \
	    3.0 *B##0;                                          \
	                                                        \
	A##31 = (TTYPE)                                         \
	    3.0 *A##0 + B##0;                                   \
	                                                        \
	B##13 = A##1 + (TTYPE)                                  \
	    3.0 *B##1;                                          \
	                                                        \
	B##31 = (TTYPE)                                         \
	    3.0 *A##1 + B##1;                                   \
	                                                        \
	dp[0] = (STYPE) (A##31 * (TTYPE) 0.25);                 \
	dp[channels] = (STYPE) (A##13 * (TTYPE) 0.25);          \
	dp[dst_stride] =                                        \
		(STYPE) ((A##31 + B##31) * (TTYPE) 0.125);      \
	dp[dst_stride + channels] =                             \
		(STYPE) ((A##13 + B##13) * (TTYPE) 0.125);      \
	dp += 2 * channels

/* *********************************************************** */

#define	ZOOM_S32_FL1(A, B, STYPE)                               \
	sp += channels;                                         \
	B##0 = sp[0];                                           \
	B##1 = sp[src_stride];                                  \
	A##13 = A##0 + (TTYPE)                                  \
	    3 *B##0;                                            \
	                                                        \
	A##31 = (TTYPE)                                         \
	    3 *A##0 + B##0;                                     \
	                                                        \
	B##13 = A##1 + (TTYPE)                                  \
	    3 *B##1;                                            \
	                                                        \
	B##31 = (TTYPE)                                         \
	    3 *A##1 + B##1;                                     \
	                                                        \
	dp[0] = (STYPE) (A##31 >> 2);                           \
	dp[channels] = (STYPE) (A##13 >> 2);                    \
	dp[dst_stride] =                                        \
		(STYPE) ((A##31 + B##31) >> 3);                 \
	dp[dst_stride + channels] =                             \
		(STYPE) ((A##13 + B##13) >> 3);                 \
	dp += 2 * channels

/* *********************************************************** */

#define	INIT_FL2()                                              \
	TTYPE a0, a1, a13, a31;                                 \
	TTYPE b0, b1, b13, b31;                                 \
	                                                        \
	a0 = sp[0];                                             \
	a1 = sp[src_stride];                                    \
	a13 = a0 + (TTYPE)                                      \
	    3.0 *a1;                                            \
	                                                        \
	a31 = (TTYPE)                                           \
	    3.0 *a0 + a1

/* *********************************************************** */

#define	INIT_S32_FL2()                                          \
	TTYPE a0, a1, a13, a31;                                 \
	TTYPE b0, b1, b13, b31;                                 \
	                                                        \
	a0 = sp[0];                                             \
	a1 = sp[src_stride];                                    \
	a13 = a0 + (TTYPE)                                      \
	    3 *a1;                                              \
	                                                        \
	a31 = (TTYPE)                                           \
	    3 *a0 + a1

/* *********************************************************** */

#define	ZOOM_FL2(A, B, STYPE)                                   \
	sp += channels;                                         \
	B##0 = sp[0];                                           \
	B##1 = sp[src_stride];                                  \
	B##13 = (B##0 + B##1) + (B##1 + B##1);                  \
	B##31 = (B##0 + B##1) + (B##0 + B##0);                  \
	dp[0] = (STYPE) (A##31 * (TTYPE) 0.25);                 \
	dp[channels] =                                          \
		(STYPE) ((A##31 + B##31) * (TTYPE) 0.125);      \
	dp[dst_stride] = (STYPE) (A##13 * (TTYPE) 0.25);        \
	dp[dst_stride + channels] =                             \
		(STYPE) ((A##13 + B##13) * (TTYPE) 0.125);      \
	dp += 2 * channels

/* *********************************************************** */

#define	ZOOM_S32_FL2(A, B, STYPE)                               \
	sp += channels;                                         \
	B##0 = sp[0];                                           \
	B##1 = sp[src_stride];                                  \
	B##13 = (B##0 + B##1) + (B##1 + B##1);                  \
	B##31 = (B##0 + B##1) + (B##0 + B##0);                  \
	dp[0] = (STYPE) (A##31 >> 2);                           \
	dp[channels] =                                          \
		(STYPE) ((A##31 + B##31) >> 3);                 \
	dp[dst_stride] = (STYPE) (A##13 >> 2);                  \
	dp[dst_stride + channels] =                             \
		(STYPE) ((A##13 + B##13) >> 3);                 \
	dp += 2 * channels

/* *********************************************************** */

#define	INIT_FL3()                                              \
	TTYPE a0, a1, a11;                                      \
	TTYPE b0, b1, b11;                                      \
	                                                        \
	a0 = sp[0];                                             \
	a1 = sp[src_stride];                                    \
	a11 = a0 + a1

/* *********************************************************** */

#define	INIT_S32_FL3()                                          \
	TTYPE a0, a1, a11;                                      \
	TTYPE b0, b1, b11;                                      \
	                                                        \
	a0 = sp[0];                                             \
	a1 = sp[src_stride];                                    \
	a11 = a0 + a1

/* *********************************************************** */

#define	ZOOM_FL3(A, B, STYPE)                                   \
	sp += channels;                                         \
	B##0 = sp[0];                                           \
	B##1 = sp[src_stride];                                  \
	B##11 = B##0 + B##1;                                    \
	dp[0] = (STYPE) A##0;                                   \
	dp[channels] = (STYPE) ((A##0 + B##0) * (TTYPE) 0.5);   \
	dp[dst_stride] = (STYPE) (A##11 * (TTYPE) 0.5);         \
	dp[dst_stride + channels] =                             \
		(STYPE) ((A##11 + B##11) * (TTYPE) 0.25);       \
	dp += 2 * channels

/* *********************************************************** */

#define	ZOOM_S32_FL3(A, B, STYPE)                               \
	sp += channels;                                         \
	B##0 = sp[0];                                           \
	B##1 = sp[src_stride];                                  \
	B##11 = B##0 + B##1;                                    \
	dp[0] = (STYPE) A##0;                                   \
	dp[channels] = (STYPE) ((A##0 + B##0) >> 1);            \
	dp[dst_stride] = (STYPE) (A##11 >> 1);                  \
	dp[dst_stride + channels] =                             \
		(STYPE) ((A##11 + B##11) >> 2);                 \
	dp += 2 * channels

/* *********************************************************** */

#define	FUNC_ZOOM_BL(TT, STYPE, N)                                       \
	void                                                             \
	mlib_ImageZoomIn2X_##TT##_BL_FL##N(STYPE * pdst,                 \
		const STYPE * psrc, mlib_s32 src_width,                  \
		mlib_s32 src_height, mlib_s32 dst_stride,                \
		mlib_s32 src_stride, mlib_s32 src_w_beg,                 \
		mlib_s32 src_h_beg, mlib_s32 channels)                   \
	{                                                                \
	    mlib_s32 i, j, k;                                            \
	                                                                 \
/*                                                                       \
 * indices                                                              \
 */                                                                      \
	    TTYPE a00, a01, a10, a11;                                    \
	                                                                 \
	    if (src_h_beg & 1) {                                         \
	                                                                 \
/*                                                                       \
 * upper edge of image                                                   \
 */                                                                      \
		for (k = 0; k < channels; k++) {                         \
		    const STYPE *sp = psrc + k;                          \
		    STYPE *dp = pdst + k;                                \
	                                                                 \
		    a00 = sp[0];                                         \
		    a10 = sp[src_stride];                                \
		    sp += channels;                                      \
		    if (j = src_w_beg & 1) {                             \
			a01 = sp[0];                                     \
			a11 = sp[src_stride];                            \
			sp += channels;                                  \
			dp[0] = (STYPE) MLIB_ZOOMIN2X_DR_FL##N;          \
			dp += channels;                                  \
			a00 = a01;                                       \
			a10 = a11;                                       \
		    }                                                    \
		    for (; j <= src_width - 2; j += 2) {                 \
			a01 = sp[0];                                     \
			a11 = sp[src_stride];                            \
			sp += channels;                                  \
			dp[0] = (STYPE) MLIB_ZOOMIN2X_DL_FL##N;          \
			dp[channels] =                                   \
				(STYPE) MLIB_ZOOMIN2X_DR_FL##N;          \
			dp += 2 * channels;                              \
			a00 = a01;                                       \
			a10 = a11;                                       \
		    }                                                    \
		    if (j < src_width) {                                 \
			a01 = sp[0];                                     \
			a11 = sp[src_stride];                            \
			dp[0] = (STYPE) MLIB_ZOOMIN2X_DL_FL##N;          \
		    }                                                    \
		}                                                        \
		psrc += src_stride;                                      \
		pdst += dst_stride;                                      \
		src_height--;                                            \
	    }                                                            \
	    if (src_w_beg & 1) {                                         \
	                                                                 \
/*                                                                       \
 * left edge of image                                                    \
 */                                                                      \
		for (k = 0; k < channels; k++) {                         \
		    const STYPE *sp = psrc + k;                          \
		    STYPE *dp = pdst + k;                                \
	                                                                 \
		    a00 = sp[0];                                         \
		    a01 = sp[channels];                                  \
		    sp += src_stride;                                    \
		    for (i = 0; i < (src_height / 2); i++) {             \
			a10 = sp[0];                                     \
			a11 = sp[channels];                              \
			sp += src_stride;                                \
			dp[0] = (STYPE) MLIB_ZOOMIN2X_UR_FL##N;          \
			dp[dst_stride] =                                 \
				(STYPE) MLIB_ZOOMIN2X_DR_FL##N;          \
			dp += 2 * dst_stride;                            \
			a00 = a10;                                       \
			a01 = a11;                                       \
		    }                                                    \
		    if (src_height & 1) {                                \
			a10 = sp[0];                                     \
			a11 = sp[channels];                              \
			dp[0] = (STYPE) MLIB_ZOOMIN2X_UR_FL##N;          \
		    }                                                    \
		}                                                        \
		psrc += channels;                                        \
		pdst += channels;                                        \
		src_width--;                                             \
	    }                                                            \
	                                                                 \
/*                                                                       \
 * internal pixels                                                       \
 */                                                                      \
	    for (i = 0; i < (src_height / 2); i++) {                     \
		for (k = 0; k < channels; k++) {                         \
		    const STYPE *sp = psrc + i * src_stride + k;         \
		    STYPE *dp = pdst + 2 * i * dst_stride + k;           \
	                                                                 \
		    INIT_FL##N();                                        \
		    for (j = 0; j < src_width / 4; j++) {                \
			ZOOM_FL##N(a, b, STYPE);                         \
			ZOOM_FL##N(b, a, STYPE);                         \
		    }                                                    \
		    if (src_width & 2) {                                 \
			ZOOM_FL##N(a, b, STYPE);                         \
		    }                                                    \
		}                                                        \
	    }                                                            \
	    if (src_height & 1) {                                        \
	                                                                 \
/*                                                                       \
 * lower edge of image                                                   \
 */                                                                      \
		for (k = 0; k < channels; k++) {                         \
		    const STYPE *sp = psrc + i * src_stride + k;         \
		    STYPE *dp = pdst + 2 * i * dst_stride + k;           \
	                                                                 \
		    a00 = sp[0];                                         \
		    a10 = sp[src_stride];                                \
		    sp += channels;                                      \
		    for (j = 0; j < (src_width / 2); j++) {              \
			a01 = sp[0];                                     \
			a11 = sp[src_stride];                            \
			sp += channels;                                  \
			dp[0] = (STYPE) MLIB_ZOOMIN2X_UL_FL##N;          \
			dp[channels] =                                   \
				(STYPE) MLIB_ZOOMIN2X_UR_FL##N;          \
			dp += 2 * channels;                              \
			a00 = a01;                                       \
			a10 = a11;                                       \
		    }                                                    \
		}                                                        \
	    }                                                            \
	    if (src_width & 1) {                                         \
	                                                                 \
/*                                                                       \
 * right edge of image                                                   \
 */                                                                      \
		for (k = 0; k < channels; k++) {                         \
		    const STYPE *sp =                                    \
			    psrc + (src_width / 2) * channels + k;       \
		    STYPE *dp = pdst + (src_width - 1) * channels + k;   \
	                                                                 \
		    a00 = sp[0];                                         \
		    a01 = sp[channels];                                  \
		    sp += src_stride;                                    \
		    for (i = 0; i < (src_height / 2); i++) {             \
			a10 = sp[0];                                     \
			a11 = sp[channels];                              \
			sp += src_stride;                                \
			dp[0] = (STYPE) MLIB_ZOOMIN2X_UL_FL##N;          \
			dp[dst_stride] =                                 \
				(STYPE) MLIB_ZOOMIN2X_DL_FL##N;          \
			dp += 2 * dst_stride;                            \
			a00 = a10;                                       \
			a01 = a11;                                       \
		    }                                                    \
		    if (src_height & 1) {                                \
			a10 = sp[0];                                     \
			a11 = sp[channels];                              \
			dp[0] = (STYPE) MLIB_ZOOMIN2X_UL_FL##N;          \
		    }                                                    \
		}                                                        \
	    }                                                            \
	}

/* *********************************************************** */

#define	FUNC_ZOOM_BL_S32(TT, STYPE, N)                                   \
	void                                                             \
	mlib_ImageZoomIn2X_##TT##_BL_FL##N(STYPE * pdst,                 \
		const STYPE * psrc, mlib_s32 src_width,                  \
		mlib_s32 src_height, mlib_s32 dst_stride,                \
		mlib_s32 src_stride, mlib_s32 src_w_beg,                 \
		mlib_s32 src_h_beg, mlib_s32 channels)                   \
	{                                                                \
	    mlib_s32 i, j, k;                                            \
	                                                                 \
/*                                                                       \
 * indices                                                              \
 */                                                                      \
	    TTYPE a00, a01, a10, a11;                                    \
	                                                                 \
	    if (src_h_beg & 1) {                                         \
	                                                                 \
/*                                                                       \
 * upper edge of image                                                   \
 */                                                                      \
		for (k = 0; k < channels; k++) {                         \
		    const STYPE *sp = psrc + k;                          \
		    STYPE *dp = pdst + k;                                \
	                                                                 \
		    a00 = sp[0];                                         \
		    a10 = sp[src_stride];                                \
		    sp += channels;                                      \
		    if (j = src_w_beg & 1) {                             \
			a01 = sp[0];                                     \
			a11 = sp[src_stride];                            \
			sp += channels;                                  \
			dp[0] = (STYPE) MLIB_ZOOMIN2X_S32_DR_FL##N;      \
			dp += channels;                                  \
			a00 = a01;                                       \
			a10 = a11;                                       \
		    }                                                    \
		    for (; j <= src_width - 2; j += 2) {                 \
			a01 = sp[0];                                     \
			a11 = sp[src_stride];                            \
			sp += channels;                                  \
			dp[0] = (STYPE) MLIB_ZOOMIN2X_S32_DL_FL##N;      \
			dp[channels] =                                   \
				(STYPE) MLIB_ZOOMIN2X_S32_DR_FL##N;      \
			dp += 2 * channels;                              \
			a00 = a01;                                       \
			a10 = a11;                                       \
		    }                                                    \
		    if (j < src_width) {                                 \
			a01 = sp[0];                                     \
			a11 = sp[src_stride];                            \
			dp[0] = (STYPE) MLIB_ZOOMIN2X_S32_DL_FL##N;      \
		    }                                                    \
		}                                                        \
		psrc += src_stride;                                      \
		pdst += dst_stride;                                      \
		src_height--;                                            \
	    }                                                            \
	    if (src_w_beg & 1) {                                         \
	                                                                 \
/*                                                                       \
 * left edge of image                                                    \
 */                                                                      \
		for (k = 0; k < channels; k++) {                         \
		    const STYPE *sp = psrc + k;                          \
		    STYPE *dp = pdst + k;                                \
	                                                                 \
		    a00 = sp[0];                                         \
		    a01 = sp[channels];                                  \
		    sp += src_stride;                                    \
		    for (i = 0; i < (src_height / 2); i++) {             \
			a10 = sp[0];                                     \
			a11 = sp[channels];                              \
			sp += src_stride;                                \
			dp[0] = (STYPE) MLIB_ZOOMIN2X_S32_UR_FL##N;      \
			dp[dst_stride] =                                 \
				(STYPE) MLIB_ZOOMIN2X_S32_DR_FL##N;      \
			dp += 2 * dst_stride;                            \
			a00 = a10;                                       \
			a01 = a11;                                       \
		    }                                                    \
		    if (src_height & 1) {                                \
			a10 = sp[0];                                     \
			a11 = sp[channels];                              \
			dp[0] = (STYPE) MLIB_ZOOMIN2X_S32_UR_FL##N;      \
		    }                                                    \
		}                                                        \
		psrc += channels;                                        \
		pdst += channels;                                        \
		src_width--;                                             \
	    }                                                            \
	                                                                 \
/*                                                                       \
 * internal pixels                                                       \
 */                                                                      \
	    for (i = 0; i < (src_height / 2); i++) {                     \
		for (k = 0; k < channels; k++) {                         \
		    const STYPE *sp = psrc + i * src_stride + k;         \
		    STYPE *dp = pdst + 2 * i * dst_stride + k;           \
	                                                                 \
		    INIT_S32_FL##N();                                    \
		    for (j = 0; j < src_width / 4; j++) {                \
			ZOOM_S32_FL##N(a, b, STYPE);                     \
			ZOOM_S32_FL##N(b, a, STYPE);                     \
		    }                                                    \
		    if (src_width & 2) {                                 \
			ZOOM_S32_FL##N(a, b, STYPE);                     \
		    }                                                    \
		}                                                        \
	    }                                                            \
	    if (src_height & 1) {                                        \
	                                                                 \
/*                                                                       \
 * lower edge of image                                                   \
 */                                                                      \
		for (k = 0; k < channels; k++) {                         \
		    const STYPE *sp = psrc + i * src_stride + k;         \
		    STYPE *dp = pdst + 2 * i * dst_stride + k;           \
	                                                                 \
		    a00 = sp[0];                                         \
		    a10 = sp[src_stride];                                \
		    sp += channels;                                      \
		    for (j = 0; j < (src_width / 2); j++) {              \
			a01 = sp[0];                                     \
			a11 = sp[src_stride];                            \
			sp += channels;                                  \
			dp[0] = (STYPE) MLIB_ZOOMIN2X_S32_UL_FL##N;      \
			dp[channels] =                                   \
				(STYPE) MLIB_ZOOMIN2X_S32_UR_FL##N;      \
			dp += 2 * channels;                              \
			a00 = a01;                                       \
			a10 = a11;                                       \
		    }                                                    \
		}                                                        \
	    }                                                            \
	    if (src_width & 1) {                                         \
	                                                                 \
/*                                                                       \
 * right edge of image                                                   \
 */                                                                      \
		for (k = 0; k < channels; k++) {                         \
		    const STYPE *sp =                                    \
			    psrc + (src_width / 2) * channels + k;       \
		    STYPE *dp = pdst + (src_width - 1) * channels + k;   \
	                                                                 \
		    a00 = sp[0];                                         \
		    a01 = sp[channels];                                  \
		    sp += src_stride;                                    \
		    for (i = 0; i < (src_height / 2); i++) {             \
			a10 = sp[0];                                     \
			a11 = sp[channels];                              \
			sp += src_stride;                                \
			dp[0] = (STYPE) MLIB_ZOOMIN2X_S32_UL_FL##N;      \
			dp[dst_stride] =                                 \
				(STYPE) MLIB_ZOOMIN2X_S32_DL_FL##N;      \
			dp += 2 * dst_stride;                            \
			a00 = a10;                                       \
			a01 = a11;                                       \
		    }                                                    \
		    if (src_height & 1) {                                \
			a10 = sp[0];                                     \
			a11 = sp[channels];                              \
			dp[0] = (STYPE) MLIB_ZOOMIN2X_S32_UL_FL##N;      \
		    }                                                    \
		}                                                        \
	    }                                                            \
	}

/* *********************************************************** */

#if defined(_NO_LONGLONG)

#define	TTYPE	mlib_d64

FUNC_ZOOM_BL(S32, mlib_s32,
	0)
FUNC_ZOOM_BL(
	S32,
	mlib_s32,
	1)
FUNC_ZOOM_BL(
	S32,
	mlib_s32,
	2)
FUNC_ZOOM_BL(
	S32,
	mlib_s32,
	3)

#else /* _NO_LONGLONG */

#define	TTYPE	mlib_s64

FUNC_ZOOM_BL_S32(S32, mlib_s32,
	0)
FUNC_ZOOM_BL_S32(
	S32,
	mlib_s32,
	1)
FUNC_ZOOM_BL_S32(
	S32,
	mlib_s32,
	2)
FUNC_ZOOM_BL_S32(
	S32,
	mlib_s32,
	3)

#endif /* _NO_LONGLONG */

/* *********************************************************** */
