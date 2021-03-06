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

#pragma ident	"@(#)mlib_s_ImageZoomIn2X_BL_Fp.c	9.3	07/11/05 SMI"

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
#ifdef __SUNPRO_C
#include <sunmedia_intrin.h>
#else
#include <emmintrin.h>
#endif

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

#define	MLIB_ZOOMIN2X_UL_FL1	(((TTYPE)3.0*a00 + a01) * (TTYPE)0.25)
#define	MLIB_ZOOMIN2X_UR_FL1	((a00 + (TTYPE)3.0*a01) * (TTYPE)0.25)

#define	MLIB_ZOOMIN2X_DL_FL1	(((TTYPE)3.0*a00 + a01 + (TTYPE)3.0*a10 + \
				a11) * (TTYPE)0.125)

#define	MLIB_ZOOMIN2X_DR_FL1	((a00 + (TTYPE)3.0*a01 + a10 + \
				(TTYPE)3.0*a11) * (TTYPE)0.125)

/* *********************************************************** */

#define	MLIB_ZOOMIN2X_UL_FL2	(((TTYPE)3.0*a00 + a10) * (TTYPE)0.25)
#define	MLIB_ZOOMIN2X_UR_FL2	(((TTYPE)3.0*a00 + (TTYPE)3.0*a01 + a10 + \
				a11) * (TTYPE)0.125)

#define	MLIB_ZOOMIN2X_DL_FL2	((a00 + (TTYPE)3.0*a10) * (TTYPE)0.25)

#define	MLIB_ZOOMIN2X_DR_FL2	((a00 + a01 + (TTYPE)3.0*a10 + \
				(TTYPE)3.0*a11) * (TTYPE)0.125)

/* *********************************************************** */

#define	MLIB_ZOOMIN2X_UL_FL3	a00
#define	MLIB_ZOOMIN2X_UR_FL3	((a00 + a01) * (TTYPE)0.5)

#define	MLIB_ZOOMIN2X_DL_FL3	((a00 + a10) * (TTYPE)0.5)

#define	MLIB_ZOOMIN2X_DR_FL3	((a00 + a01 + a10 + a11) * (TTYPE)0.25)

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

#define	MLIB_S_ZOOM_FL_F32_P3(x_d0, x_d1, x_dt0, x_dt1) {           \
	__m128 x_dp0_0, x_dp0_1;                                    \
	x_dp0_0 = _mm_and_si128(x_mask3, x_d0);                     \
	x_dp0_1 = _mm_and_si128(x_mask1, _mm_slli_si128(x_d1, 12)); \
	x_dt0 = _mm_or_si128(x_dp0_0, x_dp0_1);                     \
	x_dt1 = _mm_and_si128(x_mask2, _mm_srli_si128(x_d1, 4));    \
}

/* *********************************************************** */

#define	MLIB_S_ZOOM_FL_D64_PACK                             \
	switch (channels) {                                 \
	case 1: {                                           \
		x_dd0 = _mm_unpacklo_pd(x_d0_3, x_d1_3);    \
		x_dd1 = _mm_unpackhi_pd(x_d0_3, x_d1_3);    \
		x_dd2 = _mm_unpacklo_pd(x_d0s_3, x_d1s_3);  \
		x_dd3 = _mm_unpackhi_pd(x_d0s_3, x_d1s_3);  \
		break;                                      \
	}                                                   \
	case 2:                                             \
	case 3:                                             \
	case 4: {                                           \
		x_dd0 = x_d0_3;                             \
		x_dd1 = x_d1_3;                             \
		x_dd2 = x_d0s_3;                            \
		x_dd3 = x_d1s_3;                            \
		break;                                      \
	}                                                   \
	}                                                   \

/* *********************************************************** */

#define	MLIB_S_ZOOM_FL_STORE_D64                              \
		_mm_storeu_pd(dp, x_dd0);                     \
		_mm_storeu_pd((dp + 2), x_dd1);               \
		_mm_storeu_pd((dp + dst_stride), x_dd2);      \
		_mm_storeu_pd((dp + 2 + dst_stride), x_dd3);  \
		sp += 2;                                      \
		dp += 4

/* *********************************************************** */

#define	MLIB_S_ZOOM_D64_FL0                                 \
	__m128d x_sd0, x_sd0s, x_sd1, x_sd1s;               \
	__m128d x_dd0, x_dd1, x_dd2, x_dd3;                 \
	__m128d x_sd03, x_sd13, x_sd0s3, x_sd1s3;           \
	__m128d x_sd09, x_sd19, x_sd0s9, x_sd1s9;           \
	__m128d x_d0_0, x_d0_1, x_d0_2, x_d0_3;             \
	__m128d x_d1_0, x_d1_1, x_d1_2, x_d1_3;             \
	__m128d x_d0s_0, x_d0s_1, x_d0s_2, x_d0s_3;         \
	__m128d x_d1s_0, x_d1s_1, x_d1s_2, x_d1s_3;         \
	const __m128d x_three = _mm_set1_pd(3.0);           \
	const __m128d x_nine = _mm_set1_pd(9.0);            \
	const __m128d x_div = _mm_set1_pd(0.0625);          \
	x_sd0 = _mm_loadu_pd(sp);                           \
	x_sd0s = _mm_loadu_pd(sp + src_stride);             \
	x_sd1 = _mm_loadu_pd(sp + channels);                \
	x_sd1s = _mm_loadu_pd(sp + channels + src_stride);  \
	                                                    \
	x_sd09 = _mm_mul_pd(x_sd0, x_nine);                 \
	x_sd0s3 = _mm_mul_pd(x_sd0s, x_three);              \
	x_sd13 = _mm_mul_pd(x_sd1, x_three);                \
	x_d0_0 = _mm_add_pd(x_sd09, x_sd0s3);               \
	x_d0_1 = _mm_add_pd(x_sd13, x_sd1s);                \
	x_d0_2 = _mm_add_pd(x_d0_0, x_d0_1);                \
	x_d0_3 = _mm_mul_pd(x_d0_2, x_div);                 \
	                                                    \
	x_sd03 = _mm_mul_pd(x_sd0, x_three);                \
	x_sd19 = _mm_mul_pd(x_sd1, x_nine);                 \
	x_sd1s3 = _mm_mul_pd(x_sd1s, x_three);              \
	x_d1_0 = _mm_add_pd(x_sd03, x_sd0s);                \
	x_d1_1 = _mm_add_pd(x_sd19, x_sd1s3);               \
	x_d1_2 = _mm_add_pd(x_d1_0, x_d1_1);                \
	x_d1_3 = _mm_mul_pd(x_d1_2, x_div);                 \
	                                                    \
	x_sd0s9 = _mm_mul_pd(x_sd0s, x_nine);               \
	x_d0s_0 = _mm_add_pd(x_sd0s9, x_sd03);              \
	x_d0s_1 = _mm_add_pd(x_sd1, x_sd1s3);               \
	x_d0s_2 = _mm_add_pd(x_d0s_0, x_d0s_1);             \
	x_d0s_3 = _mm_mul_pd(x_d0s_2, x_div);               \
	                                                    \
	x_sd1s9 = _mm_mul_pd(x_sd1s, x_nine);               \
	x_d1s_0 = _mm_add_pd(x_sd0, x_sd0s3);               \
	x_d1s_1 = _mm_add_pd(x_sd13, x_sd1s9);              \
	x_d1s_2 = _mm_add_pd(x_d1s_0, x_d1s_1);             \
	x_d1s_3 = _mm_mul_pd(x_d1s_2, x_div);               \
	                                                    \
	MLIB_S_ZOOM_FL_D64_PACK;                            \
	MLIB_S_ZOOM_FL_STORE_D64

/* *********************************************************** */

#define	MLIB_S_ZOOM_D64_FL1                                 \
	__m128d x_sd0, x_sd0s, x_sd1, x_sd1s;               \
	__m128d x_dd0, x_dd1, x_dd2, x_dd3;                 \
	__m128d x_d0_0, x_d0_1, x_d0_3;                     \
	__m128d x_d1_0, x_d1_1, x_d1_3;                     \
	__m128d x_d0s_0, x_d0s_1, x_d0s_2, x_d0s_3;         \
	__m128d x_d1s_0, x_d1s_1, x_d1s_2, x_d1s_3;         \
	const __m128d x_three = _mm_set1_pd(3.0);           \
	const __m128d x_div4 = _mm_set1_pd(0.25);           \
	const __m128d x_div8 = _mm_set1_pd(0.125);          \
	x_sd0 = _mm_loadu_pd(sp);                           \
	x_sd0s = _mm_loadu_pd(sp + src_stride);             \
	x_sd1 = _mm_loadu_pd(sp + channels);                \
	x_sd1s = _mm_loadu_pd(sp + channels + src_stride);  \
	                                                    \
	x_d0_0 = _mm_mul_pd(x_sd0, x_three);                \
	x_d0_1 = _mm_add_pd(x_d0_0, x_sd1);                 \
	x_d0_3 = _mm_mul_pd(x_d0_1, x_div4);                \
	                                                    \
	x_d1_0 = _mm_mul_pd(x_sd1, x_three);                \
	x_d1_1 = _mm_add_pd(x_d1_0, x_sd0);                 \
	x_d1_3 = _mm_mul_pd(x_d1_1, x_div4);                \
	                                                    \
	x_d0s_0 = _mm_mul_pd(x_sd0s, x_three);              \
	x_d0s_1 = _mm_add_pd(x_d0s_0, x_sd1s);              \
	x_d0s_2 = _mm_add_pd(x_d0_1, x_d0s_1);              \
	x_d0s_3 = _mm_mul_pd(x_d0s_2, x_div8);              \
	                                                    \
	x_d1s_0 = _mm_mul_pd(x_sd1s, x_three);              \
	x_d1s_1 = _mm_add_pd(x_d1s_0, x_sd0s);              \
	x_d1s_2 = _mm_add_pd(x_d1_1, x_d1s_1);              \
	x_d1s_3 = _mm_mul_pd(x_d1s_2, x_div8);              \
	MLIB_S_ZOOM_FL_D64_PACK;                            \
	MLIB_S_ZOOM_FL_STORE_D64

/* *********************************************************** */

#define	MLIB_S_ZOOM_D64_FL2                                 \
	__m128d x_sd0, x_sd0s, x_sd1, x_sd1s;               \
	__m128d x_dd0, x_dd1, x_dd2, x_dd3;                 \
	__m128d x_sd03, x_sd13, x_sd0s3, x_sd1s3;           \
	__m128d x_sd09, x_sd19, x_sd0s9, x_sd1s9;           \
	__m128d x_d0_0, x_d0_1, x_d0_2, x_d0_3;             \
	__m128d x_d1_0, x_d1_1, x_d1_2, x_d1_3;             \
	__m128d x_d0s_0, x_d0s_1, x_d0s_2, x_d0s_3;         \
	__m128d x_d1s_0, x_d1s_1, x_d1s_2, x_d1s_3;         \
	const __m128d x_three = _mm_set1_pd(3.0);           \
	const __m128d x_nine = _mm_set1_pd(9.0);            \
	const __m128d x_div4 = _mm_set1_pd(0.25);           \
	const __m128d x_div8 = _mm_set1_pd(0.125);          \
	x_sd0 = _mm_loadu_pd(sp);                           \
	x_sd0s = _mm_loadu_pd(sp + src_stride);             \
	x_sd1 = _mm_loadu_pd(sp + channels);                \
	x_sd1s = _mm_loadu_pd(sp + channels + src_stride);  \
	                                                    \
	x_sd03 = _mm_mul_pd(x_sd0, x_three);                \
	x_d0_0 = _mm_add_pd(x_sd03, x_sd0s);                \
	x_d0_3 = _mm_mul_pd(x_d0_0, x_div4);                \
	x_sd13 = _mm_mul_pd(x_sd1, x_three);                \
	x_d1_0 = _mm_add_pd(x_sd13, x_sd1s);                \
	x_d1_1 = _mm_add_pd(x_d1_0, x_d0_0);                \
	x_d1_3 = _mm_mul_pd(x_d1_1, x_div8);                \
	                                                    \
	x_sd0s3 = _mm_mul_pd(x_sd0s, x_three);              \
	x_d0s_0 = _mm_add_pd(x_sd0s3, x_sd0);               \
	x_d0s_3 = _mm_mul_pd(x_d0s_0, x_div4);              \
	                                                    \
	x_sd1s3 = _mm_mul_pd(x_sd1s, x_three);              \
	x_d1s_0 = _mm_add_pd(x_sd1s3, x_sd1);               \
	x_d1s_1 = _mm_add_pd(x_d1s_0, x_d0s_0);             \
	x_d1s_3 = _mm_mul_pd(x_d1s_1, x_div8);              \
	MLIB_S_ZOOM_FL_D64_PACK;                            \
	MLIB_S_ZOOM_FL_STORE_D64

/* *********************************************************** */

#define	MLIB_S_ZOOM_D64_FL3                                 \
	__m128d x_sd0, x_sd0s, x_sd1, x_sd1s;               \
	__m128d x_dd0, x_dd1, x_dd2, x_dd3;                 \
	__m128d x_d0_3, x_d1_0, x_d1_3, x_d0s_0;            \
	__m128d x_d0s_3, x_d1s_0, x_d1s_1, x_d1s_3;         \
	const __m128d x_div2 = _mm_set1_pd(0.5);            \
	const __m128d x_div4 = _mm_set1_pd(0.25);           \
	x_sd0 = _mm_loadu_pd(sp);                           \
	x_sd0s = _mm_loadu_pd(sp + src_stride);             \
	x_sd1 = _mm_loadu_pd(sp + channels);                \
	x_sd1s = _mm_loadu_pd(sp + channels + src_stride);  \
	                                                    \
	x_d0_3 = x_sd0;                                     \
	                                                    \
	x_d1_0 = _mm_add_pd(x_sd0, x_sd1);                  \
	x_d1_3 = _mm_mul_pd(x_d1_0, x_div2);                \
	                                                    \
	x_d0s_0 = _mm_add_pd(x_sd0, x_sd0s);                \
	x_d0s_3 = _mm_mul_pd(x_d0s_0, x_div2);              \
	                                                    \
	x_d1s_0 = _mm_add_pd(x_sd1, x_sd1s);                \
	x_d1s_1 = _mm_add_pd(x_d1s_0, x_d0s_0);             \
	x_d1s_3 = _mm_mul_pd(x_d1s_1, x_div4);              \
	MLIB_S_ZOOM_FL_D64_PACK;                            \
	MLIB_S_ZOOM_FL_STORE_D64

/* *********************************************************** */

#define	MLIB_S_ZOOM_FL_F32_PACK                             \
	switch (channels) {                                 \
	case 1: {                                           \
		x_dd0 = _mm_unpacklo_ps(x_d0_3, x_d1_3);    \
		x_dd1 = _mm_unpackhi_ps(x_d0_3, x_d1_3);    \
		x_dd2 = _mm_unpacklo_ps(x_d0s_3, x_d1s_3);  \
		x_dd3 = _mm_unpackhi_ps(x_d0s_3, x_d1s_3);  \
		break;                                      \
	}                                                   \
	case 2: {                                           \
		x_dd0 = _mm_movelh_ps(x_d0_3, x_d1_3);      \
		x_dd1 = _mm_movehl_ps(x_d1_3, x_d0_3);      \
		x_dd2 = _mm_movelh_ps(x_d0s_3, x_d1s_3);    \
		x_dd3 = _mm_movehl_ps(x_d1s_3, x_d0s_3);    \
		break;                                      \
	}                                                   \
	case 3:                                             \
	case 4: {                                           \
		x_dd0 = x_d0_3;                             \
		x_dd1 = x_d1_3;                             \
		x_dd2 = x_d0s_3;                            \
		x_dd3 = x_d1s_3;                            \
		break;                                      \
	}                                                   \
	}                                                   \

/* *********************************************************** */

#define	MLIB_S_ZOOM_FL_STORE_F32                            \
	if (channels == 3) {                                \
		TTYPE d0, d0s;                              \
		d0 = dp[6];                                 \
		d0s = dp[dst_stride + 6];                   \
		_mm_storeu_ps(dp, x_dd0);                   \
		_mm_storeu_ps((dp + channels), x_dd1);      \
		dp[6] = d0;                                 \
		_mm_storeu_ps(dp + dst_stride, x_dd2);      \
		_mm_storeu_ps(                              \
		    dp + channels + dst_stride, x_dd3);     \
		dp[6 + dst_stride] = d0s;                   \
		sp += 3;                                    \
		dp += 6;                                    \
	} else {                                            \
		_mm_storeu_ps(dp, x_dd0);                   \
		_mm_storeu_ps((dp + 4), x_dd1);             \
		_mm_storeu_ps((dp + dst_stride), x_dd2);    \
		_mm_storeu_ps(                              \
		    (dp + 4 + dst_stride), x_dd3);          \
		sp += 4;                                    \
		dp += 8;                                    \
	}

/* *********************************************************** */

#define	MLIB_S_ZOOM_F32_FL0                                    \
	__m128 x_sd0, x_sd0s, x_sd1, x_sd1s;                   \
	__m128 x_dd0, x_dd1, x_dd2, x_dd3;                     \
	__m128 x_sd03, x_sd13, x_sd0s3, x_sd1s3;               \
	__m128 x_sd09, x_sd19, x_sd0s9, x_sd1s9;               \
	__m128 x_d0_0, x_d0_1, x_d0_2, x_d0_3;                 \
	__m128 x_d1_0, x_d1_1, x_d1_2, x_d1_3;                 \
	__m128 x_d0s_0, x_d0s_1, x_d0s_2, x_d0s_3;             \
	__m128 x_d1s_0, x_d1s_1, x_d1s_2, x_d1s_3;             \
	const __m128 x_three = _mm_set1_ps(3.0);               \
	const __m128 x_nine = _mm_set1_ps(9.0);                \
	const __m128 x_div = _mm_set1_ps(0.0625);              \
	x_sd0 = _mm_loadu_ps(sp);                              \
	x_sd0s = _mm_loadu_ps(sp + src_stride);                \
	x_sd1 = _mm_loadu_ps(sp + channels);                   \
	x_sd1s = _mm_loadu_ps(sp + channels + src_stride);     \
	                                                       \
	x_sd09 = _mm_mul_ps(x_sd0, x_nine);                    \
	x_sd0s3 = _mm_mul_ps(x_sd0s, x_three);                 \
	x_sd13 = _mm_mul_ps(x_sd1, x_three);                   \
	x_d0_0 = _mm_add_ps(x_sd09, x_sd0s3);                  \
	x_d0_1 = _mm_add_ps(x_sd13, x_sd1s);                   \
	x_d0_2 = _mm_add_ps(x_d0_0, x_d0_1);                   \
	x_d0_3 = _mm_mul_ps(x_d0_2, x_div);                    \
	                                                       \
	x_sd03 = _mm_mul_ps(x_sd0, x_three);                   \
	x_sd19 = _mm_mul_ps(x_sd1, x_nine);                    \
	x_sd1s3 = _mm_mul_ps(x_sd1s, x_three);                 \
	x_d1_0 = _mm_add_ps(x_sd03, x_sd0s);                   \
	x_d1_1 = _mm_add_ps(x_sd19, x_sd1s3);                  \
	x_d1_2 = _mm_add_ps(x_d1_0, x_d1_1);                   \
	x_d1_3 = _mm_mul_ps(x_d1_2, x_div);                    \
	                                                       \
	x_sd0s9 = _mm_mul_ps(x_sd0s, x_nine);                  \
	x_d0s_0 = _mm_add_ps(x_sd0s9, x_sd03);                 \
	x_d0s_1 = _mm_add_ps(x_sd1, x_sd1s3);                  \
	x_d0s_2 = _mm_add_ps(x_d0s_0, x_d0s_1);                \
	x_d0s_3 = _mm_mul_ps(x_d0s_2, x_div);                  \
	                                                       \
	x_sd1s9 = _mm_mul_ps(x_sd1s, x_nine);                  \
	x_d1s_0 = _mm_add_ps(x_sd0, x_sd0s3);                  \
	x_d1s_1 = _mm_add_ps(x_sd13, x_sd1s9);                 \
	x_d1s_2 = _mm_add_ps(x_d1s_0, x_d1s_1);                \
	x_d1s_3 = _mm_mul_ps(x_d1s_2, x_div);                  \
	                                                       \
	MLIB_S_ZOOM_FL_F32_PACK;                               \
	MLIB_S_ZOOM_FL_STORE_F32

/* *********************************************************** */

#define	MLIB_S_ZOOM_F32_FL1                                    \
	__m128 x_sd0, x_sd0s, x_sd1, x_sd1s;                   \
	__m128 x_dd0, x_dd1, x_dd2, x_dd3;                     \
	__m128 x_d0_0, x_d0_1, x_d0_3;                         \
	__m128 x_d1_0, x_d1_1, x_d1_3;                         \
	__m128 x_d0s_0, x_d0s_1, x_d0s_2, x_d0s_3;             \
	__m128 x_d1s_0, x_d1s_1, x_d1s_2, x_d1s_3;             \
	const __m128 x_three = _mm_set1_ps(3.0);               \
	const __m128 x_div4 = _mm_set1_ps(0.25);               \
	const __m128 x_div8 = _mm_set1_ps(0.125);              \
	x_sd0 = _mm_loadu_ps(sp);                              \
	x_sd0s = _mm_loadu_ps(sp + src_stride);                \
	x_sd1 = _mm_loadu_ps(sp + channels);                   \
	x_sd1s = _mm_loadu_ps(sp + channels + src_stride);     \
	                                                       \
	x_d0_0 = _mm_mul_ps(x_sd0, x_three);                   \
	x_d0_1 = _mm_add_ps(x_d0_0, x_sd1);                    \
	x_d0_3 = _mm_mul_ps(x_d0_1, x_div4);                   \
	                                                       \
	x_d1_0 = _mm_mul_ps(x_sd1, x_three);                   \
	x_d1_1 = _mm_add_ps(x_d1_0, x_sd0);                    \
	x_d1_3 = _mm_mul_ps(x_d1_1, x_div4);                   \
	                                                       \
	x_d0s_0 = _mm_mul_ps(x_sd0s, x_three);                 \
	x_d0s_1 = _mm_add_ps(x_d0s_0, x_sd1s);                 \
	x_d0s_2 = _mm_add_ps(x_d0_1, x_d0s_1);                 \
	x_d0s_3 = _mm_mul_ps(x_d0s_2, x_div8);                 \
	                                                       \
	x_d1s_0 = _mm_mul_ps(x_sd1s, x_three);                 \
	x_d1s_1 = _mm_add_ps(x_d1s_0, x_sd0s);                 \
	x_d1s_2 = _mm_add_ps(x_d1_1, x_d1s_1);                 \
	x_d1s_3 = _mm_mul_ps(x_d1s_2, x_div8);                 \
	MLIB_S_ZOOM_FL_F32_PACK;                               \
	MLIB_S_ZOOM_FL_STORE_F32

/* *********************************************************** */

#define	MLIB_S_ZOOM_F32_FL2                                    \
	__m128 x_sd0, x_sd0s, x_sd1, x_sd1s;                   \
	__m128 x_dd0, x_dd1, x_dd2, x_dd3;                     \
	__m128 x_sd03, x_sd13, x_sd0s3, x_sd1s3;               \
	__m128 x_sd09, x_sd19, x_sd0s9, x_sd1s9;               \
	__m128 x_d0_0, x_d0_1, x_d0_2, x_d0_3;                 \
	__m128 x_d1_0, x_d1_1, x_d1_2, x_d1_3;                 \
	__m128 x_d0s_0, x_d0s_1, x_d0s_2, x_d0s_3;             \
	__m128 x_d1s_0, x_d1s_1, x_d1s_2, x_d1s_3;             \
	const __m128 x_three = _mm_set1_ps(3.0);               \
	const __m128 x_nine = _mm_set1_ps(9.0);                \
	const __m128 x_div4 = _mm_set1_ps(0.25);               \
	const __m128 x_div8 = _mm_set1_ps(0.125);              \
	x_sd0 = _mm_loadu_ps(sp);                              \
	x_sd0s = _mm_loadu_ps(sp + src_stride);                \
	x_sd1 = _mm_loadu_ps(sp + channels);                   \
	x_sd1s = _mm_loadu_ps(sp + channels + src_stride);     \
	                                                       \
	x_sd03 = _mm_mul_ps(x_sd0, x_three);                   \
	x_d0_0 = _mm_add_ps(x_sd03, x_sd0s);                   \
	x_d0_3 = _mm_mul_ps(x_d0_0, x_div4);                   \
	x_sd13 = _mm_mul_ps(x_sd1, x_three);                   \
	x_d1_0 = _mm_add_ps(x_sd13, x_sd1s);                   \
	x_d1_1 = _mm_add_ps(x_d1_0, x_d0_0);                   \
	x_d1_3 = _mm_mul_ps(x_d1_1, x_div8);                   \
	                                                       \
	x_sd0s3 = _mm_mul_ps(x_sd0s, x_three);                 \
	x_d0s_0 = _mm_add_ps(x_sd0s3, x_sd0);                  \
	x_d0s_3 = _mm_mul_ps(x_d0s_0, x_div4);                 \
	                                                       \
	x_sd1s3 = _mm_mul_ps(x_sd1s, x_three);                 \
	x_d1s_0 = _mm_add_ps(x_sd1s3, x_sd1);                  \
	x_d1s_1 = _mm_add_ps(x_d1s_0, x_d0s_0);                \
	x_d1s_3 = _mm_mul_ps(x_d1s_1, x_div8);                 \
	MLIB_S_ZOOM_FL_F32_PACK;                               \
	MLIB_S_ZOOM_FL_STORE_F32

/* *********************************************************** */

#define	MLIB_S_ZOOM_F32_FL3                                    \
	__m128 x_sd0, x_sd0s, x_sd1, x_sd1s;                   \
	__m128 x_dd0, x_dd1, x_dd2, x_dd3;                     \
	__m128 x_d0_3, x_d1_0, x_d1_3, x_d0s_0;                \
	__m128 x_d0s_3, x_d1s_0, x_d1s_1, x_d1s_3;             \
	const __m128 x_div2 = _mm_set1_ps(0.5);                \
	const __m128 x_div4 = _mm_set1_ps(0.25);               \
	x_sd0 = _mm_loadu_ps(sp);                              \
	x_sd0s = _mm_loadu_ps(sp + src_stride);                \
	x_sd1 = _mm_loadu_ps(sp + channels);                   \
	x_sd1s = _mm_loadu_ps(sp + channels + src_stride);     \
	                                                       \
	x_d0_3 = x_sd0;                                        \
	                                                       \
	x_d1_0 = _mm_add_ps(x_sd0, x_sd1);                     \
	x_d1_3 = _mm_mul_ps(x_d1_0, x_div2);                   \
	                                                       \
	x_d0s_0 = _mm_add_ps(x_sd0, x_sd0s);                   \
	x_d0s_3 = _mm_mul_ps(x_d0s_0, x_div2);                 \
	                                                       \
	x_d1s_0 = _mm_add_ps(x_sd1, x_sd1s);                   \
	x_d1s_1 = _mm_add_ps(x_d1s_0, x_d0s_0);                \
	x_d1s_3 = _mm_mul_ps(x_d1s_1, x_div4);                 \
	MLIB_S_ZOOM_FL_F32_PACK;                               \
	MLIB_S_ZOOM_FL_STORE_F32

/* *********************************************************** */

#define	INIT_FL1()                                              \
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

#define	INIT_FL3()                                              \
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
 * indices                                                               \
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
	    if (channels == 1) {                                         \
		for (i = 0; i < (src_height / 2); i++) {                 \
		    const STYPE *sp = psrc + i * src_stride;             \
		    STYPE *dp = pdst + 2 * i * dst_stride;               \
		    for (j = 0; j < ((src_width * channels) / 4); j++) { \
			MLIB_S_ZOOM_D64_FL##N;                           \
		    }                                                    \
			int m;                                           \
		    for (k = 0; k < channels; k++) {                     \
			sp = psrc + i * src_stride + j * 2 + k;          \
			dp = pdst + 2 * i * dst_stride + j * 4 + k;      \
			INIT_FL##N();                                    \
			m = j / channels;                                \
			if (m < src_width / 4) {                         \
			    ZOOM_FL##N(a, b, STYPE);                     \
			    ZOOM_FL##N(b, a, STYPE);                     \
			    m++;                                         \
			}                                                \
			m = m * 2;                                       \
			if (m < (src_width / 2)) {                       \
			    ZOOM_FL##N(a, b, STYPE);                     \
			}                                                \
		    }                                                    \
		}                                                        \
	    } else if (channels == 2) {                                  \
		for (i = 0; i < (src_height / 2); i++) {                 \
		    const STYPE *sp = psrc + i * src_stride;             \
		    STYPE *dp = pdst + 2 * i * dst_stride;               \
		    for (j = 0; j < ((src_width * channels) / 4); j++) { \
			MLIB_S_ZOOM_D64_FL##N;                           \
		    }                                                    \
		}                                                        \
	    } else {                                                     \
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
		}  \
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

#define	MLIB_S_FUNC_ZOOM_BL_F32(TT, STYPE, N)                            \
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
 * indices                                                               \
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
	    if ((channels == 3) || (channels == 4)) {                    \
		for (i = 0; i < (src_height / 2); i++) {                 \
		    const STYPE *sp = psrc + i * src_stride;             \
		    STYPE *dp = pdst + 2 * i * dst_stride;               \
		    for (j = 0; j < (src_width / 2); j++) {              \
			MLIB_S_ZOOM_F32_FL##N;                           \
		    }                                                    \
		}                                                        \
	    } else {                                                     \
		for (i = 0; i < (src_height / 2); i++) {                 \
		    const STYPE *sp = psrc + i * src_stride;             \
		    STYPE *dp = pdst + 2 * i * dst_stride;               \
		    for (j = 0; j < ((src_width * channels) / 8); j++) { \
			MLIB_S_ZOOM_F32_FL##N;                           \
		    }                                                    \
			int m;                                           \
		    for (k = 0; k < channels; k++) {                     \
			sp = psrc + i * src_stride + j * 4 + k;          \
			dp = pdst + 2 * i * dst_stride + j * 8 + k;      \
			INIT_FL##N();                                    \
			m = j * 2 / channels;                            \
			if (m < src_width / 4) {                         \
			    ZOOM_FL##N(a, b, STYPE);                     \
			    ZOOM_FL##N(b, a, STYPE);                     \
			    m++;                                         \
			}                                                \
			m = m * 2;                                       \
			if (m < (src_width / 2)) {                       \
			    ZOOM_FL##N(a, b, STYPE);                     \
			}                                                \
		    }                                                    \
		}                                                        \
	    }                                                            \
		if (src_height & 1) {                                    \
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

#define	TTYPE	mlib_d64

FUNC_ZOOM_BL(
	D64,
	mlib_d64,
	0)
FUNC_ZOOM_BL(
	D64,
	mlib_d64,
	1)
FUNC_ZOOM_BL(
	D64,
	mlib_d64,
	2)
FUNC_ZOOM_BL(
	D64,
	mlib_d64,
	3)

/* *********************************************************** */

#undef  TTYPE
#define	TTYPE	mlib_f32
MLIB_S_FUNC_ZOOM_BL_F32(
	F32,
	mlib_f32,
	0)
MLIB_S_FUNC_ZOOM_BL_F32(
	F32,
	mlib_f32,
	1)
MLIB_S_FUNC_ZOOM_BL_F32(
	F32,
	mlib_f32,
	2)
MLIB_S_FUNC_ZOOM_BL_F32(
	F32,
	mlib_f32,
	3)

/* *********************************************************** */
