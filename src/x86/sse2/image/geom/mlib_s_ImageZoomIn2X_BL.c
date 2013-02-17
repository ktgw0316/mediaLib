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

#pragma ident	"@(#)mlib_s_ImageZoomIn2X_BL.c	9.4	07/11/05 SMI"

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
#include <mlib_s_ImageZoomIn2X.h>
#ifdef __SUNPRO_C
#include <sunmedia_intrin.h>
#else
#include <emmintrin.h>
#endif

/* *********************************************************** */

#define	TTYPE	mlib_s32

/* *********************************************************** */

#define	MLIB_ZOOMIN2X_UL_FL0	(9*a00 + 3*a01 + 3*a10 + a11) >> 4
#define	MLIB_ZOOMIN2X_UR_FL0	(3*a00 + 9*a01 + a10 + 3*a11) >> 4

#define	MLIB_ZOOMIN2X_DL_FL0	(3*a00 + a01 + 9*a10 + 3*a11) >> 4

#define	MLIB_ZOOMIN2X_DR_FL0	(a00 + 3*a01 + 3*a10 + 9*a11) >> 4

/* *********************************************************** */

#define	MLIB_ZOOMIN2X_UL_FL1	(3*a00 + a01) >> 2
#define	MLIB_ZOOMIN2X_UR_FL1	(a00 + 3*a01) >> 2

#define	MLIB_ZOOMIN2X_DL_FL1	(3*a00 + a01 + 3*a10 + a11) >> 3

#define	MLIB_ZOOMIN2X_DR_FL1	(a00 + 3*a01 + a10 + 3*a11) >> 3

/* *********************************************************** */

#define	MLIB_ZOOMIN2X_UL_FL2	(3*a00 + a10) >> 2
#define	MLIB_ZOOMIN2X_UR_FL2	(3*a00 + 3*a01 + a10 + a11) >> 3

#define	MLIB_ZOOMIN2X_DL_FL2	(a00 + 3*a10) >> 2

#define	MLIB_ZOOMIN2X_DR_FL2	(a00 + a01 + 3*a10 + 3*a11) >> 3

/* *********************************************************** */

#define	MLIB_ZOOMIN2X_UL_FL3	a00
#define	MLIB_ZOOMIN2X_UR_FL3	(a00 + a01) >> 1

#define	MLIB_ZOOMIN2X_DL_FL3	(a00 + a10) >> 1

#define	MLIB_ZOOMIN2X_DR_FL3	(a00 + a01 + a10 + a11) >> 2

/* *********************************************************** */

#define	INIT_FL0()                                              \
	TTYPE a0, a1, a13, a31, a39, a93;                       \
	TTYPE b0, b1, b13, b31, b39, b93;                       \
	                                                        \
	a0 = sp[0];                                             \
	a1 = sp[src_stride];                                    \
	a13 = (a0 + a1) + (a1 + a1);                            \
	a31 = (a0 + a1) + (a0 + a0);                            \
	a39 = 3 * a13;                                          \
	a93 = 3 * a31

/* *********************************************************** */

#define	MLIB_S_ZOOM_FL_S16_P3(x_d0, x_d1, x_dt0, x_dt1) {             \
	__m128i x_dp0_0, x_dp0_1, x_dp0_2;                            \
	__m128i x_dp1_0, x_dp1_1, x_dp1_2;                            \
	x_dp0_0 = _mm_and_si128(x_mask31, _mm_slli_si128(x_d1, 6));   \
	x_dp0_1 = _mm_and_si128(x_mask2, _mm_slli_si128(x_d0, 6));    \
	x_dp0_2 = _mm_or_si128(x_dp0_0, x_dp0_1);                     \
	x_dt0 = _mm_or_si128(x_dp0_2, _mm_and_si128(x_d0, x_mask30)); \
	x_dp1_0 = _mm_and_si128(x_mask1, _mm_srli_si128(x_d0, 10));   \
	x_dp1_1 = _mm_srli_si128(x_d1, 4);                            \
	x_dp1_2 = _mm_and_si128(x_dp1_1, x_mask32);                   \
	x_dt1 = _mm_or_si128(x_dp1_0, x_dp1_2);                       \
}

#define	MLIB_S_ZOOM_FL_S16_PACK                                     \
	switch (channels) {                                         \
	case 1: {                                                   \
		x_dd0 = _mm_unpacklo_epi16(x_d0_3, x_d1_3);         \
		x_dd1 = _mm_unpackhi_epi16(x_d0_3, x_d1_3);         \
		x_dd2 = _mm_unpacklo_epi16(x_d0s_3, x_d1s_3);       \
		x_dd3 = _mm_unpackhi_epi16(x_d0s_3, x_d1s_3);       \
		break;                                              \
	}                                                           \
	case 2: {                                                   \
		x_dd0 = _mm_unpacklo_epi32(x_d0_3, x_d1_3);         \
		x_dd1 = _mm_unpackhi_epi32(x_d0_3, x_d1_3);         \
		x_dd2 = _mm_unpacklo_epi32(x_d0s_3, x_d1s_3);       \
		x_dd3 = _mm_unpackhi_epi32(x_d0s_3, x_d1s_3);       \
		break;                                              \
	}                                                           \
	case 4: {                                                   \
		x_dd0 = _mm_unpacklo_epi64(x_d0_3, x_d1_3);         \
		x_dd1 = _mm_unpackhi_epi64(x_d0_3, x_d1_3);         \
		x_dd2 = _mm_unpacklo_epi64(x_d0s_3, x_d1s_3);       \
		x_dd3 = _mm_unpackhi_epi64(x_d0s_3, x_d1s_3);       \
		break;                                              \
	}                                                           \
	case 3: {                                                   \
		__m128i x_mask1 = _mm_set_epi16(                    \
		    0, 0, 0, 0, 0, 0, 0, 0xFFFF);                   \
		__m128i x_mask2 = _mm_set_epi16(                    \
		    0xFFFF, 0xFFFF, 0, 0, 0, 0, 0, 0);              \
		__m128i x_mask30 = _mm_set_epi16(                   \
		    0, 0, 0, 0, 0, 0xFFFF, 0xFFFF, 0xFFFF);         \
		__m128i x_mask31 = _mm_slli_si128(x_mask30, 6);     \
		__m128i x_mask32 = _mm_slli_si128(x_mask30, 2);     \
		MLIB_S_ZOOM_FL_S16_P3(x_d0_3, x_d1_3, x_dd0, x_dd1);    \
		MLIB_S_ZOOM_FL_S16_P3(x_d0s_3, x_d1s_3, x_dd2, x_dd3);  \
		break;                                              \
	}                                                           \
	}                                                           \

#define	MLIB_S_ZOOM_FL_STORE(STR, STR3)                                   \
	if (channels == 3) {                                              \
		_mm_storeu_si128((void *)dp, x_dd0);                      \
		_mm_storel_epi64((void *)(dp + STR), x_dd1);              \
		_mm_storeu_si128((void *)(dp + dst_stride), x_dd2);       \
		_mm_storel_epi64((void *)(dp + STR + dst_stride), x_dd3); \
		sp += STR3;                                               \
		dp += 2 * STR3;                                           \
	} else {                                                          \
		_mm_storeu_si128((void *)dp, x_dd0);                      \
		_mm_storeu_si128((void *)(dp + STR), x_dd1);              \
		_mm_storeu_si128((void *)(dp + dst_stride), x_dd2);       \
		_mm_storeu_si128((void *)(dp + STR + dst_stride), x_dd3); \
		sp += STR;                                                \
		dp += 2 * STR;                                            \
	}


#define	MLIB_S_ZOOM_S16_FL0(SFT)                                \
	__m128i x_sd0, x_sd0s, x_sd1, x_sd1s;                   \
	__m128i x_dd0, x_dd1, x_dd2, x_dd3;                     \
	x_sd0 = _mm_loadu_si128((void *)sp);                    \
	x_sd0s = _mm_loadu_si128((void *)(sp + src_stride));    \
	x_sd1 = _mm_loadu_si128((void *)(sp + channels));       \
	x_sd1s = _mm_loadu_si128(                               \
	    (void *)(sp + channels + src_stride));              \
	x_sd0 = _mm_##SFT##_epi16(x_sd0, 1);                    \
	x_sd0s = _mm_##SFT##_epi16(x_sd0s, 1);                  \
	x_sd1 = _mm_##SFT##_epi16(x_sd1, 1);                    \
	x_sd1s = _mm_##SFT##_epi16(x_sd1s, 1);                  \
	__m128i x_sd00s, x_sd11s, x_sd00s2, x_sd11s2;           \
	__m128i x_sd04, x_sd14, x_sd0s4, x_sd1s4, x_sd0s1s;     \
	__m128i x_d0_0, x_d0_1, x_d0_2, x_d0_3, x_sd01;         \
	__m128i x_d1_0, x_d1_1, x_d1_2, x_d1_3;                 \
	__m128i x_d0s_0, x_d0s_1, x_d0s_2, x_d0s_3;             \
	__m128i x_d1s_0, x_d1s_1, x_d1s_2, x_d1s_3;             \
	x_sd00s = _mm_add_epi16(x_sd0, x_sd0s);                 \
	x_sd11s = _mm_add_epi16(x_sd1, x_sd1s);                 \
	x_sd01 = _mm_add_epi16(x_sd0, x_sd1);                   \
	x_sd0s1s = _mm_add_epi16(x_sd0s, x_sd1s);               \
	                                                        \
	x_sd00s = _mm_##SFT##_epi16(x_sd00s, 1);                \
	x_sd11s = _mm_##SFT##_epi16(x_sd11s, 1);                \
	x_d0_0 = _mm_add_epi16(x_sd00s, x_sd11s);               \
	x_sd01 = _mm_##SFT##_epi16(x_sd01, 1);                  \
	x_d0_1 = _mm_add_epi16(x_sd00s, x_sd01);                \
	x_d0_0 = _mm_##SFT##_epi16(x_d0_0, 1);                  \
	x_d0_2 = _mm_add_epi16(x_sd0, x_d0_0);                  \
	x_d0_2 = _mm_##SFT##_epi16(x_d0_2, 1);                  \
	x_d0_1 = _mm_##SFT##_epi16(x_d0_1, 1);                  \
	x_d0_3 = _mm_add_epi16(x_d0_2, x_d0_1);                 \
	                                                        \
	x_d1_1 = _mm_add_epi16(x_sd01, x_sd11s);                \
	x_d1_2 = _mm_add_epi16(x_d0_0, x_sd1);                  \
	x_d1_2 = _mm_##SFT##_epi16(x_d1_2, 1);                  \
	x_d1_1 = _mm_##SFT##_epi16(x_d1_1, 1);                  \
	x_d1_3 = _mm_add_epi16(x_d1_2, x_d1_1);                 \
	                                                        \
	x_sd0s1s = _mm_##SFT##_epi16(x_sd0s1s, 1);              \
	x_d0s_1 = _mm_add_epi16(x_sd0s1s, x_sd00s);             \
	x_d0s_2 = _mm_add_epi16(x_d0_0, x_sd0s);                \
	x_d0s_1 = _mm_##SFT##_epi16(x_d0s_1, 1);                \
	x_d0s_2 = _mm_##SFT##_epi16(x_d0s_2, 1);                \
	x_d0s_3 = _mm_add_epi16(x_d0s_1, x_d0s_2);              \
	                                                        \
	x_d1s_1 = _mm_add_epi16(x_sd0s1s, x_sd11s);             \
	x_d1s_2 = _mm_add_epi16(x_d0_0, x_sd1s);                \
	x_d1s_1 = _mm_##SFT##_epi16(x_d1s_1, 1);                \
	x_d1s_2 = _mm_##SFT##_epi16(x_d1s_2, 1);                \
	x_d1s_3 = _mm_add_epi16(x_d1s_1, x_d1s_2);              \
	MLIB_S_ZOOM_FL_S16_PACK;                                \
	MLIB_S_ZOOM_FL_STORE(8, 6);

#define	ZOOM_FL0(A, B)                                          \
	sp += channels;                                         \
	B##0 = sp[0];                                           \
	B##1 = sp[src_stride];                                  \
	B##13 = (B##0 + B##1) + (B##1 + B##1);                  \
	B##31 = (B##0 + B##1) + (B##0 + B##0);                  \
	B##39 = 3 * B##13;                                      \
	B##93 = 3 * B##31;                                      \
	dp[0] = (A##93 + B##31) >> 4;                           \
	dp[channels] = (A##31 + B##93) >> 4;                    \
	dp[dst_stride] = (A##39 + B##13) >> 4;                  \
	dp[dst_stride + channels] = (A##13 + B##39) >> 4;       \
	dp += 2 * channels

/* *********************************************************** */

#define	INIT_FL1()                                              \
	TTYPE a0, a1, a13, a31;                                 \
	TTYPE b0, b1, b13, b31;                                 \
	                                                        \
	a0 = sp[0];                                             \
	a1 = sp[src_stride]

/* *********************************************************** */

#define	ZOOM_FL1(A, B)                                          \
	sp += channels;                                         \
	B##0 = sp[0];                                           \
	B##1 = sp[src_stride];                                  \
	A##13 = (A##0 + B##0) + (B##0 + B##0);                  \
	A##31 = (A##0 + B##0) + (A##0 + A##0);                  \
	B##13 = (A##1 + B##1) + (B##1 + B##1);                  \
	B##31 = (A##1 + B##1) + (A##1 + A##1);                  \
	dp[0] = A##31 >> 2;                                     \
	dp[channels] = A##13 >> 2;                              \
	dp[dst_stride] = (A##31 + B##31) >> 3;                  \
	dp[dst_stride + channels] = (A##13 + B##13) >> 3;       \
	dp += 2 * channels

#define	MLIB_S_ZOOM_S16_FL1(SFT)                                \
	__m128i x_sd0, x_sd0s, x_sd1, x_sd1s;                   \
	__m128i x_sd01, x_sd0s1s, x_d0_3, x_d1_3;               \
	__m128i x_d0s_0, x_d0s_1, x_d0s_2, x_d0s_3;             \
	__m128i x_d1s_0, x_d1s_1, x_d1s_2, x_d1s_3;             \
	__m128i x_dd0, x_dd1, x_dd2, x_dd3;                     \
	x_sd0 = _mm_loadu_si128((void *)sp);                    \
	x_sd0s = _mm_loadu_si128((void *)(sp + src_stride));    \
	x_sd1 = _mm_loadu_si128((void *)(sp + channels));       \
	x_sd1s = _mm_loadu_si128(                               \
	    (void *)(sp + channels + src_stride));              \
	x_sd0 = _mm_##SFT##_epi16(x_sd0, 1);                    \
	x_sd0s = _mm_##SFT##_epi16(x_sd0s, 1);                  \
	x_sd1 = _mm_##SFT##_epi16(x_sd1, 1);                    \
	x_sd1s = _mm_##SFT##_epi16(x_sd1s, 1);                  \
	x_sd01 = _mm_add_epi16(x_sd0, x_sd1);                   \
	x_sd01 = _mm_##SFT##_epi16(x_sd01, 1);                  \
	x_d0_3 = _mm_add_epi16(x_sd01, x_sd0);                  \
	x_d1_3 = _mm_add_epi16(x_sd01, x_sd1);                  \
	x_sd0s1s = _mm_add_epi16(x_sd0s, x_sd1s);               \
	x_sd0s1s = _mm_##SFT##_epi16(x_sd0s1s, 1);              \
	x_d0s_0 = _mm_add_epi16(x_sd0s1s, x_sd0s);              \
	x_d0s_1 = _mm_##SFT##_epi16(x_d0s_0, 1);                \
	x_d0s_2 = _mm_##SFT##_epi16(x_d0_3, 1);                 \
	x_d0s_3 = _mm_add_epi16(x_d0s_1, x_d0s_2);              \
	x_d1s_0 = _mm_add_epi16(x_sd0s1s, x_sd1s);              \
	x_d1s_1 = _mm_##SFT##_epi16(x_d1s_0, 1);                \
	x_d1s_2 = _mm_##SFT##_epi16(x_d1_3, 1);                 \
	x_d1s_3 = _mm_add_epi16(x_d1s_1, x_d1s_2);              \
	MLIB_S_ZOOM_FL_S16_PACK;                                \
	MLIB_S_ZOOM_FL_STORE(8, 6);

/* *********************************************************** */

#define	MLIB_S_ZOOM_S16_FL2(SFT)                                \
	__m128i x_sd0, x_sd0s, x_sd1, x_sd1s;                   \
	__m128i x_dd0, x_dd1, x_dd2, x_dd3;                     \
	__m128i x_sd00s, x_sd11s, x_d0_3, x_d1_3;               \
	__m128i x_d0s_0, x_d0s_1, x_d0s_3;                      \
	__m128i x_d1s_0, x_d1s_1, x_d1s_3;                      \
	x_sd0 = _mm_loadu_si128((void *)sp);                    \
	x_sd0s = _mm_loadu_si128((void *)(sp + src_stride));    \
	x_sd1 = _mm_loadu_si128((void *)(sp + channels));       \
	x_sd1s = _mm_loadu_si128(                               \
	    (void *)(sp + channels + src_stride));              \
	x_sd0 = _mm_##SFT##_epi16(x_sd0, 1);                    \
	x_sd0s = _mm_##SFT##_epi16(x_sd0s, 1);                  \
	x_sd1 = _mm_##SFT##_epi16(x_sd1, 1);                    \
	x_sd1s = _mm_##SFT##_epi16(x_sd1s, 1);                  \
	x_sd00s = _mm_add_epi16(x_sd0, x_sd0s);                 \
	x_sd11s = _mm_add_epi16(x_sd1, x_sd1s);                 \
	x_sd00s = _mm_##SFT##_epi16(x_sd00s, 1);                \
	x_sd11s = _mm_##SFT##_epi16(x_sd11s, 1);                \
	x_d0_3 = _mm_add_epi16(x_sd00s, x_sd0);                 \
	x_d0s_3 = _mm_add_epi16(x_sd00s, x_sd0s);               \
	x_d0s_0 = _mm_add_epi16(x_sd00s, x_sd11s);              \
	x_d0s_1 = _mm_add_epi16(x_sd0, x_sd1);                  \
	x_d0s_0 = _mm_##SFT##_epi16(x_d0s_0, 1);                \
	x_d0s_1 = _mm_##SFT##_epi16(x_d0s_1, 1);                \
	x_d1_3 = _mm_add_epi16(x_d0s_0, x_d0s_1);               \
	x_d1s_0 = _mm_add_epi16(x_sd0s, x_sd1s);                \
	x_d1s_1 = _mm_##SFT##_epi16(x_d1s_0, 1);                \
	x_d1s_3 = _mm_add_epi16(x_d0s_0, x_d1s_1);              \
	MLIB_S_ZOOM_FL_S16_PACK;                                \
	MLIB_S_ZOOM_FL_STORE(8, 6);

#define	INIT_FL2()                                              \
	TTYPE a0, a1, a13, a31;                                 \
	TTYPE b0, b1, b13, b31;                                 \
	                                                        \
	a0 = sp[0];                                             \
	a1 = sp[src_stride];                                    \
	a13 = (a0 + a1) + (a1 + a1);                            \
	a31 = (a0 + a1) + (a0 + a0)

/* *********************************************************** */

#define	ZOOM_FL2(A, B)                                          \
	sp += channels;                                         \
	B##0 = sp[0];                                           \
	B##1 = sp[src_stride];                                  \
	B##13 = (B##0 + B##1) + (B##1 + B##1);                  \
	B##31 = (B##0 + B##1) + (B##0 + B##0);                  \
	dp[0] = A##31 >> 2;                                     \
	dp[channels] = (A##31 + B##31) >> 3;                    \
	dp[dst_stride] = A##13 >> 2;                            \
	dp[dst_stride + channels] = (A##13 + B##13) >> 3;       \
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

#define	ZOOM_FL3(A, B)                                          \
	sp += channels;                                         \
	B##0 = sp[0];                                           \
	B##1 = sp[src_stride];                                  \
	B##11 = B##0 + B##1;                                    \
	dp[0] = A##0;                                           \
	dp[channels] = (A##0 + B##0) >> 1;                      \
	dp[dst_stride] = A##11 >> 1;                            \
	dp[dst_stride + channels] = (A##11 + B##11) >> 2;       \
	dp += 2 * channels

#define	MLIB_S_ZOOM_S16_FL3(SFT)                                \
	__m128i x_sd0, x_sd0s, x_sd1, x_sd1s;                   \
	__m128i x_dd0, x_dd1, x_dd2, x_dd3;                     \
	__m128i x_d0_3, x_d1_3, x_d0s_3;                        \
	__m128i x_d1s_0, x_d1s_1, x_d1s_2, x_d1s_3;             \
	x_sd0 = _mm_loadu_si128((void *)sp);                    \
	x_sd0s = _mm_loadu_si128((void *)(sp + src_stride));    \
	x_sd1 = _mm_loadu_si128((void *)(sp + channels));       \
	x_sd1s = _mm_loadu_si128(                               \
	    (void *)(sp + channels + src_stride));              \
	x_d0_3 = x_sd0;                                         \
	x_sd0 = _mm_##SFT##_epi16(x_sd0, 1);                    \
	x_sd0s = _mm_##SFT##_epi16(x_sd0s, 1);                  \
	x_sd1 = _mm_##SFT##_epi16(x_sd1, 1);                    \
	x_sd1s = _mm_##SFT##_epi16(x_sd1s, 1);                  \
	x_d1_3 = _mm_add_epi16(x_sd0, x_sd1);                   \
	x_d0s_3 = _mm_add_epi16(x_sd0, x_sd0s);                 \
	x_d1s_0 = _mm_add_epi16(x_sd1, x_sd1s);                 \
	x_d1s_1 = _mm_##SFT##_epi16(x_d1s_0, 1);                \
	x_d1s_2 = _mm_##SFT##_epi16(x_d0s_3, 1);                \
	x_d1s_3 = _mm_add_epi16(x_d1s_1, x_d1s_2);              \
	MLIB_S_ZOOM_FL_S16_PACK;                                \
	MLIB_S_ZOOM_FL_STORE(8, 6);

/* *********************************************************** */

#define	MLIB_S_ZOOM_FL_U8_P3(x_d0, x_d1, x_dt0, x_dt1) {                 \
	__m128i x_dp0_0, x_dp0_1, x_dp0_2;                            \
	__m128i x_dp0_3, x_dp0_4, x_dp0_5, x_dp0_6;                  \
	x_dp0_0 = _mm_and_si128(x_d0, x_mask30);  \
	x_dp0_1 = _mm_and_si128(x_d1, x_mask30);  \
	x_dp0_2 = _mm_and_si128(x_d0, x_mask31);  \
	x_dp0_3 = _mm_and_si128(x_d1, x_mask31);  \
	x_dp0_4 = _mm_and_si128(x_d0, x_mask32);  \
	x_dp0_5 = _mm_or_si128(x_dp0_1, x_dp0_2); \
	x_dp0_6 = _mm_or_si128(x_dp0_3, x_dp0_4); \
	x_dp0_1 = _mm_slli_si128(x_dp0_5, 3);     \
	x_dp0_2 = _mm_slli_si128(x_dp0_6, 6);     \
	x_dp0_3 = _mm_or_si128(x_dp0_1, x_dp0_2); \
	x_dp0_4 = _mm_or_si128(x_dp0_3, x_dp0_0); \
	x_dp0_5 = _mm_and_si128(x_mask1, _mm_slli_si128(x_d1, 9)); \
	x_dt0 = _mm_or_si128(x_dp0_5, x_dp0_4);   \
	x_dp0_0 = _mm_and_si128(x_mask33, _mm_srli_si128(x_d0, 7)); \
	x_dp0_1 = _mm_and_si128(x_mask34, _mm_srli_si128(x_d1, 4)); \
	x_dp0_2 = _mm_and_si128(x_mask2, _mm_srli_si128(x_d1, 7));  \
	x_dp0_4 = _mm_or_si128(x_dp0_0, x_dp0_1);                   \
	x_dt1 = _mm_or_si128(x_dp0_4, x_dp0_2);  \
	}

#define	MLIB_S_ZOOM_FL_U8_PACK                                         \
	x_d0_3 = _mm_packus_epi16(x_d0_3_l, x_d0_3_h);     \
	x_d1_3 = _mm_packus_epi16(x_d1_3_l, x_d1_3_h);     \
	x_d0s_3 = _mm_packus_epi16(x_d0s_3_l, x_d0s_3_h);     \
	x_d1s_3 = _mm_packus_epi16(x_d1s_3_l, x_d1s_3_h);     \
	switch (channels) {                                         \
	case 1: {                                                   \
		x_dd0 = _mm_unpacklo_epi8(x_d0_3, x_d1_3);         \
		x_dd1 = _mm_unpackhi_epi8(x_d0_3, x_d1_3);         \
		x_dd2 = _mm_unpacklo_epi8(x_d0s_3, x_d1s_3);       \
		x_dd3 = _mm_unpackhi_epi8(x_d0s_3, x_d1s_3);       \
		break;                                              \
	}                                                           \
	case 2: {                                                   \
		x_dd0 = _mm_unpacklo_epi16(x_d0_3, x_d1_3);         \
		x_dd1 = _mm_unpackhi_epi16(x_d0_3, x_d1_3);         \
		x_dd2 = _mm_unpacklo_epi16(x_d0s_3, x_d1s_3);       \
		x_dd3 = _mm_unpackhi_epi16(x_d0s_3, x_d1s_3);       \
		break;                                              \
	}                                                           \
	case 4: {                                                   \
		x_dd0 = _mm_unpacklo_epi32(x_d0_3, x_d1_3);         \
		x_dd1 = _mm_unpackhi_epi32(x_d0_3, x_d1_3);         \
		x_dd2 = _mm_unpacklo_epi32(x_d0s_3, x_d1s_3);       \
		x_dd3 = _mm_unpackhi_epi32(x_d0s_3, x_d1s_3);       \
		break;                                              \
	}                                                           \
	case 3: {                                                   \
		__m128i x_mask1 = _mm_set_epi8(0xFF, 0, 0, 0,		\
						0, 0, 0, 0,		\
						0, 0, 0, 0,		\
						0, 0, 0, 0);		\
		__m128i x_mask2 = _mm_set_epi8(0, 0, 0, 0,		\
						0, 0, 0, 0,		\
						0, 0, 0, 0,		\
						0, 0, 0xFF, 0xFF);	\
		__m128i x_mask30 = _mm_set_epi8(0, 0, 0, 0,		\
						0, 0, 0, 0,		\
						0, 0, 0, 0,		\
						0, 0xFF, 0xFF, 0xFF);	\
		__m128i x_mask31 = _mm_slli_si128(x_mask30, 3); \
		__m128i x_mask32 = _mm_slli_si128(x_mask30, 6); \
		__m128i x_mask33 = _mm_slli_si128(x_mask30, 2); \
		__m128i x_mask34 = _mm_slli_si128(x_mask30, 5); \
		MLIB_S_ZOOM_FL_U8_P3(x_d0_3, x_d1_3, x_dd0, x_dd1)      \
		MLIB_S_ZOOM_FL_U8_P3(x_d0s_3, x_d1s_3, x_dd2, x_dd3)    \
		break;                                              \
	}                                                           \
	}                                                           \

#define	MLIB_S_ZOOM_U8_FL0                               \
	__m128i x_sd0, x_sd0s, x_sd1, x_sd1s;                   \
	__m128i x_dd0, x_dd1, x_dd2, x_dd3;                     \
	__m128i x_sd0_l, x_sd0s_l, x_sd1_l, x_sd1s_l;                   \
	__m128i x_sd0_h, x_sd0s_h, x_sd1_h, x_sd1s_h;                   \
	__m128i x_sd00s_l, x_sd11s_l, x_sd01_l, x_sd0s1s_l;             \
	__m128i x_sd00s_h, x_sd11s_h, x_sd01_h, x_sd0s1s_h;             \
	__m128i x_d0_0_l, x_d0_1_l, x_d0_2_l, x_d0_3_l, x_d0_3;         \
	__m128i x_d0_0_h, x_d0_1_h, x_d0_2_h, x_d0_3_h;                 \
	__m128i x_d1_0_l, x_d1_1_l, x_d1_2_l, x_d1_3_l, x_d1_3;         \
	__m128i x_d1_0_h, x_d1_1_h, x_d1_2_h, x_d1_3_h;                 \
	__m128i x_d0s_0_l, x_d0s_1_l, x_d0s_2_l, x_d0s_3_l, x_d0s_3;    \
	__m128i x_d0s_0_h, x_d0s_1_h, x_d0s_2_h, x_d0s_3_h;             \
	__m128i x_d1s_0_l, x_d1s_1_l, x_d1s_2_l, x_d1s_3_l, x_d1s_3;    \
	__m128i x_d1s_0_h, x_d1s_1_h, x_d1s_2_h, x_d1s_3_h;             \
	const __m128i x_zero = _mm_setzero_si128();             \
	x_sd0 = _mm_loadu_si128((void *)sp);                    \
	x_sd0s = _mm_loadu_si128((void *)(sp + src_stride));    \
	x_sd1 = _mm_loadu_si128((void *)(sp + channels));       \
	x_sd1s = _mm_loadu_si128(                               \
	    (void *)(sp + channels + src_stride));              \
	x_sd0_l = _mm_unpacklo_epi8(x_sd0, x_zero);             \
	x_sd0_h = _mm_unpackhi_epi8(x_sd0, x_zero);             \
	x_sd0s_l = _mm_unpacklo_epi8(x_sd0s, x_zero);             \
	x_sd0s_h = _mm_unpackhi_epi8(x_sd0s, x_zero);             \
	x_sd1_l = _mm_unpacklo_epi8(x_sd1, x_zero);             \
	x_sd1_h = _mm_unpackhi_epi8(x_sd1, x_zero);             \
	x_sd1s_l = _mm_unpacklo_epi8(x_sd1s, x_zero);             \
	x_sd1s_h = _mm_unpackhi_epi8(x_sd1s, x_zero);             \
	\
	x_sd0_l = _mm_srli_epi16(x_sd0_l, 1);                    \
	x_sd0s_l = _mm_srli_epi16(x_sd0s_l, 1);                  \
	x_sd1_l = _mm_srli_epi16(x_sd1_l, 1);                    \
	x_sd1s_l = _mm_srli_epi16(x_sd1s_l, 1);                  \
	x_sd0_h = _mm_srli_epi16(x_sd0_h, 1);                    \
	x_sd0s_h = _mm_srli_epi16(x_sd0s_h, 1);                  \
	x_sd1_h = _mm_srli_epi16(x_sd1_h, 1);                    \
	x_sd1s_h = _mm_srli_epi16(x_sd1s_h, 1);                  \
	\
	x_sd00s_l = _mm_add_epi16(x_sd0_l, x_sd0s_l);                  \
	x_sd00s_h = _mm_add_epi16(x_sd0_h, x_sd0s_h);                  \
	x_sd01_l = _mm_add_epi16(x_sd0_l, x_sd1_l);                    \
	x_sd01_h = _mm_add_epi16(x_sd0_h, x_sd1_h);                    \
	x_sd11s_l = _mm_add_epi16(x_sd1_l, x_sd1s_l);                  \
	x_sd11s_h = _mm_add_epi16(x_sd1_h, x_sd1s_h);                  \
	x_sd0s1s_l = _mm_add_epi16(x_sd0s_l, x_sd1s_l);                \
	x_sd0s1s_h = _mm_add_epi16(x_sd0s_h, x_sd1s_h);                \
	\
	x_sd00s_l = _mm_srli_epi16(x_sd00s_l, 1);                  \
	x_sd00s_h = _mm_srli_epi16(x_sd00s_h, 1);                  \
	x_sd11s_l = _mm_srli_epi16(x_sd11s_l, 1);                  \
	x_sd11s_h = _mm_srli_epi16(x_sd11s_h, 1);                  \
	x_sd01_l = _mm_srli_epi16(x_sd01_l, 1);                  \
	x_sd01_h = _mm_srli_epi16(x_sd01_h, 1);                  \
	x_sd0s1s_l = _mm_srli_epi16(x_sd0s1s_l, 1);                  \
	x_sd0s1s_h = _mm_srli_epi16(x_sd0s1s_h, 1);                  \
\
	x_d0_0_l = _mm_add_epi16(x_sd00s_l, x_sd11s_l);                 \
	x_d0_0_h = _mm_add_epi16(x_sd00s_h, x_sd11s_h);                 \
	x_d0_0_l = _mm_srli_epi16(x_d0_0_l, 1);                 \
	x_d0_0_h = _mm_srli_epi16(x_d0_0_h, 1);                 \
	x_d0_1_l = _mm_add_epi16(x_d0_0_l, x_sd0_l);                \
	x_d0_1_h = _mm_add_epi16(x_d0_0_h, x_sd0_h);                \
	x_d0_2_l = _mm_add_epi16(x_sd00s_l, x_sd01_l);                   \
	x_d0_2_h = _mm_add_epi16(x_sd00s_h, x_sd01_h);                   \
	x_d0_1_l = _mm_srli_epi16(x_d0_1_l, 1);                 \
	x_d0_1_h = _mm_srli_epi16(x_d0_1_h, 1);                 \
	x_d0_2_l = _mm_srli_epi16(x_d0_2_l, 1);                 \
	x_d0_2_h = _mm_srli_epi16(x_d0_2_h, 1);                 \
	x_d0_3_l = _mm_add_epi16(x_d0_1_l, x_d0_2_l); \
	x_d0_3_h = _mm_add_epi16(x_d0_1_h, x_d0_2_h); \
	\
	x_d1_1_l = _mm_add_epi16(x_d0_0_l, x_sd1_l);                \
	x_d1_1_h = _mm_add_epi16(x_d0_0_h, x_sd1_h);                \
	x_d1_2_l = _mm_add_epi16(x_sd11s_l, x_sd01_l);                   \
	x_d1_2_h = _mm_add_epi16(x_sd11s_h, x_sd01_h);                   \
	x_d1_1_l = _mm_srli_epi16(x_d1_1_l, 1);                 \
	x_d1_1_h = _mm_srli_epi16(x_d1_1_h, 1);                 \
	x_d1_2_l = _mm_srli_epi16(x_d1_2_l, 1);                 \
	x_d1_2_h = _mm_srli_epi16(x_d1_2_h, 1);                 \
	x_d1_3_l = _mm_add_epi16(x_d1_1_l, x_d1_2_l); \
	x_d1_3_h = _mm_add_epi16(x_d1_1_h, x_d1_2_h); \
	\
	x_d0s_1_l = _mm_add_epi16(x_d0_0_l, x_sd0s_l);                \
	x_d0s_1_h = _mm_add_epi16(x_d0_0_h, x_sd0s_h);                \
	x_d0s_2_l = _mm_add_epi16(x_sd00s_l, x_sd0s1s_l);                   \
	x_d0s_2_h = _mm_add_epi16(x_sd00s_h, x_sd0s1s_h);                   \
	x_d0s_1_l = _mm_srli_epi16(x_d0s_1_l, 1);                 \
	x_d0s_1_h = _mm_srli_epi16(x_d0s_1_h, 1);                 \
	x_d0s_2_l = _mm_srli_epi16(x_d0s_2_l, 1);                 \
	x_d0s_2_h = _mm_srli_epi16(x_d0s_2_h, 1);                 \
	x_d0s_3_l = _mm_add_epi16(x_d0s_1_l, x_d0s_2_l); \
	x_d0s_3_h = _mm_add_epi16(x_d0s_1_h, x_d0s_2_h); \
	\
	x_d1s_1_l = _mm_add_epi16(x_d0_0_l, x_sd1s_l);                \
	x_d1s_1_h = _mm_add_epi16(x_d0_0_h, x_sd1s_h);                \
	x_d1s_2_l = _mm_add_epi16(x_sd11s_l, x_sd0s1s_l);                   \
	x_d1s_2_h = _mm_add_epi16(x_sd11s_h, x_sd0s1s_h);                   \
	x_d1s_1_l = _mm_srli_epi16(x_d1s_1_l, 1);                 \
	x_d1s_1_h = _mm_srli_epi16(x_d1s_1_h, 1);                 \
	x_d1s_2_l = _mm_srli_epi16(x_d1s_2_l, 1);                 \
	x_d1s_2_h = _mm_srli_epi16(x_d1s_2_h, 1);                 \
	x_d1s_3_l = _mm_add_epi16(x_d1s_1_l, x_d1s_2_l); \
	x_d1s_3_h = _mm_add_epi16(x_d1s_1_h, x_d1s_2_h); \
	\
	MLIB_S_ZOOM_FL_U8_PACK;                                 \
	MLIB_S_ZOOM_FL_STORE(16, 12); \

#define	MLIB_S_ZOOM_U8_FL1                               \
	__m128i x_sd0, x_sd0s, x_sd1, x_sd1s;                   \
	__m128i x_dd0, x_dd1, x_dd2, x_dd3;                     \
	__m128i x_sd0_l, x_sd0s_l, x_sd1_l, x_sd1s_l;                   \
	__m128i x_sd0_h, x_sd0s_h, x_sd1_h, x_sd1s_h;                   \
	__m128i x_sd00s_l, x_sd11s_l, x_sd01_l, x_sd0s1s_l;             \
	__m128i x_sd00s_h, x_sd11s_h, x_sd01_h, x_sd0s1s_h;             \
	__m128i x_d0_3, x_d0_3_l, x_d0_3_h, x_d1_3, x_d1_3_l, x_d1_3_h; \
	__m128i x_d0s_3, x_d0s_0_l, x_d0s_0_h, x_d0s_3_l, x_d0s_3_h; \
	__m128i x_d1s_3, x_d1s_3_l, x_d1s_3_h;                     \
	const __m128i x_zero = _mm_setzero_si128();             \
	x_sd0 = _mm_loadu_si128((void *)sp);                    \
	x_sd0s = _mm_loadu_si128((void *)(sp + src_stride));    \
	x_sd1 = _mm_loadu_si128((void *)(sp + channels));       \
	x_sd1s = _mm_loadu_si128(                               \
	    (void *)(sp + channels + src_stride));              \
	x_sd0_l = _mm_unpacklo_epi8(x_sd0, x_zero);             \
	x_sd0_h = _mm_unpackhi_epi8(x_sd0, x_zero);             \
	x_sd0s_l = _mm_unpacklo_epi8(x_sd0s, x_zero);             \
	x_sd0s_h = _mm_unpackhi_epi8(x_sd0s, x_zero);             \
	x_sd1_l = _mm_unpacklo_epi8(x_sd1, x_zero);             \
	x_sd1_h = _mm_unpackhi_epi8(x_sd1, x_zero);             \
	x_sd1s_l = _mm_unpacklo_epi8(x_sd1s, x_zero);             \
	x_sd1s_h = _mm_unpackhi_epi8(x_sd1s, x_zero);             \
	\
	x_sd0_l = _mm_srli_epi16(x_sd0_l, 1);                    \
	x_sd0s_l = _mm_srli_epi16(x_sd0s_l, 1);                  \
	x_sd1_l = _mm_srli_epi16(x_sd1_l, 1);                    \
	x_sd1s_l = _mm_srli_epi16(x_sd1s_l, 1);                  \
	x_sd0_h = _mm_srli_epi16(x_sd0_h, 1);                    \
	x_sd0s_h = _mm_srli_epi16(x_sd0s_h, 1);                  \
	x_sd1_h = _mm_srli_epi16(x_sd1_h, 1);                    \
	x_sd1s_h = _mm_srli_epi16(x_sd1s_h, 1);                  \
	\
	x_sd00s_l = _mm_add_epi16(x_sd0_l, x_sd0s_l);                  \
	x_sd00s_h = _mm_add_epi16(x_sd0_h, x_sd0s_h);                  \
	x_sd01_l = _mm_add_epi16(x_sd0_l, x_sd1_l);                    \
	x_sd01_h = _mm_add_epi16(x_sd0_h, x_sd1_h);                    \
	x_sd11s_l = _mm_add_epi16(x_sd1_l, x_sd1s_l);                  \
	x_sd11s_h = _mm_add_epi16(x_sd1_h, x_sd1s_h);                  \
	x_sd0s1s_l = _mm_add_epi16(x_sd0s_l, x_sd1s_l);                \
	x_sd0s1s_h = _mm_add_epi16(x_sd0s_h, x_sd1s_h);                \
	\
	x_sd00s_l = _mm_srli_epi16(x_sd00s_l, 1);                  \
	x_sd00s_h = _mm_srli_epi16(x_sd00s_h, 1);                  \
	x_sd11s_l = _mm_srli_epi16(x_sd11s_l, 1);                  \
	x_sd11s_h = _mm_srli_epi16(x_sd11s_h, 1);                  \
	x_sd01_l = _mm_srli_epi16(x_sd01_l, 1);                  \
	x_sd01_h = _mm_srli_epi16(x_sd01_h, 1);                  \
	x_sd0s1s_l = _mm_srli_epi16(x_sd0s1s_l, 1);                  \
	x_sd0s1s_h = _mm_srli_epi16(x_sd0s1s_h, 1);                  \
\
	x_d0_3_l = _mm_add_epi16(x_sd01_l, x_sd0_l);                 \
	x_d0_3_h = _mm_add_epi16(x_sd01_h, x_sd0_h);                 \
	\
	x_d1_3_l = _mm_add_epi16(x_sd01_l, x_sd1_l);                \
	x_d1_3_h = _mm_add_epi16(x_sd01_h, x_sd1_h);                \
	\
	x_d0s_0_l = _mm_add_epi16(x_sd01_l, x_sd0s1s_l);                   \
	x_d0s_0_h = _mm_add_epi16(x_sd01_h, x_sd0s1s_h);                   \
	x_d0s_0_l = _mm_srli_epi16(x_d0s_0_l, 1);                   \
	x_d0s_0_h = _mm_srli_epi16(x_d0s_0_h, 1);                   \
	x_d0s_3_l = _mm_add_epi16(x_d0s_0_l, x_sd00s_l); \
	x_d0s_3_h = _mm_add_epi16(x_d0s_0_h, x_sd00s_h); \
	\
	x_d1s_3_l = _mm_add_epi16(x_d0s_0_l, x_sd11s_l); \
	x_d1s_3_h = _mm_add_epi16(x_d0s_0_h, x_sd11s_h); \
	\
	MLIB_S_ZOOM_FL_U8_PACK;                                 \
	MLIB_S_ZOOM_FL_STORE(16, 12)

#define	MLIB_S_ZOOM_U8_FL2                               \
	__m128i x_sd0, x_sd0s, x_sd1, x_sd1s;                   \
	__m128i x_dd0, x_dd1, x_dd2, x_dd3;                     \
	__m128i x_sd0_l, x_sd0s_l, x_sd1_l, x_sd1s_l;                   \
	__m128i x_sd0_h, x_sd0s_h, x_sd1_h, x_sd1s_h;                   \
	__m128i x_sd00s_l, x_sd11s_l, x_sd01_l, x_sd0s1s_l;             \
	__m128i x_sd00s_h, x_sd11s_h, x_sd01_h, x_sd0s1s_h;             \
	__m128i x_d0_0_l, x_d0_1_l, x_d0_2_l, x_d0_3_l, x_d0_3;         \
	__m128i x_d0_0_h, x_d0_1_h, x_d0_2_h, x_d0_3_h;                 \
	__m128i x_d1_0_l, x_d1_1_l, x_d1_2_l, x_d1_3_l, x_d1_3;         \
	__m128i x_d1_0_h, x_d1_1_h, x_d1_2_h, x_d1_3_h;                 \
	__m128i x_d0s_0_l, x_d0s_1_l, x_d0s_2_l, x_d0s_3_l, x_d0s_3;    \
	__m128i x_d0s_0_h, x_d0s_1_h, x_d0s_2_h, x_d0s_3_h;             \
	__m128i x_d1s_0_l, x_d1s_1_l, x_d1s_2_l, x_d1s_3_l, x_d1s_3;    \
	__m128i x_d1s_0_h, x_d1s_1_h, x_d1s_2_h, x_d1s_3_h;             \
	const __m128i x_zero = _mm_setzero_si128();             \
	x_sd0 = _mm_loadu_si128((void *)sp);                    \
	x_sd0s = _mm_loadu_si128((void *)(sp + src_stride));    \
	x_sd1 = _mm_loadu_si128((void *)(sp + channels));       \
	x_sd1s = _mm_loadu_si128(                               \
	    (void *)(sp + channels + src_stride));              \
	x_sd0_l = _mm_unpacklo_epi8(x_sd0, x_zero);             \
	x_sd0_h = _mm_unpackhi_epi8(x_sd0, x_zero);             \
	x_sd0s_l = _mm_unpacklo_epi8(x_sd0s, x_zero);             \
	x_sd0s_h = _mm_unpackhi_epi8(x_sd0s, x_zero);             \
	x_sd1_l = _mm_unpacklo_epi8(x_sd1, x_zero);             \
	x_sd1_h = _mm_unpackhi_epi8(x_sd1, x_zero);             \
	x_sd1s_l = _mm_unpacklo_epi8(x_sd1s, x_zero);             \
	x_sd1s_h = _mm_unpackhi_epi8(x_sd1s, x_zero);             \
	\
	x_sd0_l = _mm_srli_epi16(x_sd0_l, 1);                    \
	x_sd0s_l = _mm_srli_epi16(x_sd0s_l, 1);                  \
	x_sd1_l = _mm_srli_epi16(x_sd1_l, 1);                    \
	x_sd1s_l = _mm_srli_epi16(x_sd1s_l, 1);                  \
	x_sd0_h = _mm_srli_epi16(x_sd0_h, 1);                    \
	x_sd0s_h = _mm_srli_epi16(x_sd0s_h, 1);                  \
	x_sd1_h = _mm_srli_epi16(x_sd1_h, 1);                    \
	x_sd1s_h = _mm_srli_epi16(x_sd1s_h, 1);                  \
	\
	x_sd00s_l = _mm_add_epi16(x_sd0_l, x_sd0s_l);                  \
	x_sd00s_h = _mm_add_epi16(x_sd0_h, x_sd0s_h);                  \
	x_sd01_l = _mm_add_epi16(x_sd0_l, x_sd1_l);                    \
	x_sd01_h = _mm_add_epi16(x_sd0_h, x_sd1_h);                    \
	x_sd11s_l = _mm_add_epi16(x_sd1_l, x_sd1s_l);                  \
	x_sd11s_h = _mm_add_epi16(x_sd1_h, x_sd1s_h);                  \
	x_sd0s1s_l = _mm_add_epi16(x_sd0s_l, x_sd1s_l);                \
	x_sd0s1s_h = _mm_add_epi16(x_sd0s_h, x_sd1s_h);                \
	\
	x_sd00s_l = _mm_srli_epi16(x_sd00s_l, 1);                  \
	x_sd00s_h = _mm_srli_epi16(x_sd00s_h, 1);                  \
	x_sd11s_l = _mm_srli_epi16(x_sd11s_l, 1);                  \
	x_sd11s_h = _mm_srli_epi16(x_sd11s_h, 1);                  \
	x_sd01_l = _mm_srli_epi16(x_sd01_l, 1);                  \
	x_sd01_h = _mm_srli_epi16(x_sd01_h, 1);                  \
	x_sd0s1s_l = _mm_srli_epi16(x_sd0s1s_l, 1);                  \
	x_sd0s1s_h = _mm_srli_epi16(x_sd0s1s_h, 1);                  \
	\
	x_d0_3_l = _mm_add_epi16(x_sd00s_l, x_sd0_l); \
	x_d0_3_h = _mm_add_epi16(x_sd00s_h, x_sd0_h); \
	\
	x_d1_0_l = _mm_add_epi16(x_sd00s_l, x_sd11s_l); \
	x_d1_0_h = _mm_add_epi16(x_sd00s_h, x_sd11s_h); \
	x_d1_0_l = _mm_srli_epi16(x_d1_0_l, 1); \
	x_d1_0_h = _mm_srli_epi16(x_d1_0_h, 1); \
	x_d1_3_l = _mm_add_epi16(x_d1_0_l, x_sd01_l); \
	x_d1_3_h = _mm_add_epi16(x_d1_0_h, x_sd01_h); \
	\
	x_d0s_3_l = _mm_add_epi16(x_sd00s_l, x_sd0s_l); \
	x_d0s_3_h = _mm_add_epi16(x_sd00s_h, x_sd0s_h); \
	\
	x_d1s_3_l = _mm_add_epi16(x_d1_0_l, x_sd0s1s_l); \
	x_d1s_3_h = _mm_add_epi16(x_d1_0_h, x_sd0s1s_h); \
	MLIB_S_ZOOM_FL_U8_PACK;                                 \
	MLIB_S_ZOOM_FL_STORE(16, 12)

#define	MLIB_S_ZOOM_U8_FL3                               \
	__m128i x_sd0, x_sd0s, x_sd1, x_sd1s;                   \
	__m128i x_dd0, x_dd1, x_dd2, x_dd3;                     \
	__m128i x_sd0_l, x_sd0s_l, x_sd1_l, x_sd1s_l;                   \
	__m128i x_sd0_h, x_sd0s_h, x_sd1_h, x_sd1s_h;                   \
	__m128i x_sd00s_l, x_sd11s_l, x_sd01_l, x_sd0s1s_l;             \
	__m128i x_sd00s_h, x_sd11s_h, x_sd01_h, x_sd0s1s_h;             \
	__m128i x_d0_0_l, x_d0_1_l, x_d0_2_l, x_d0_3_l, x_d0_3;         \
	__m128i x_d0_0_h, x_d0_1_h, x_d0_2_h, x_d0_3_h;                 \
	__m128i x_d1_0_l, x_d1_1_l, x_d1_2_l, x_d1_3_l, x_d1_3;         \
	__m128i x_d1_0_h, x_d1_1_h, x_d1_2_h, x_d1_3_h;                 \
	__m128i x_d0s_0_l, x_d0s_1_l, x_d0s_2_l, x_d0s_3_l, x_d0s_3;    \
	__m128i x_d0s_0_h, x_d0s_1_h, x_d0s_2_h, x_d0s_3_h;             \
	__m128i x_d1s_0_l, x_d1s_1_l, x_d1s_2_l, x_d1s_3_l, x_d1s_3;    \
	__m128i x_d1s_0_h, x_d1s_1_h, x_d1s_2_h, x_d1s_3_h;             \
	const __m128i x_zero = _mm_setzero_si128();             \
	x_sd0 = _mm_loadu_si128((void *)sp);                    \
	x_sd0s = _mm_loadu_si128((void *)(sp + src_stride));    \
	x_sd1 = _mm_loadu_si128((void *)(sp + channels));       \
	x_sd1s = _mm_loadu_si128(                               \
	    (void *)(sp + channels + src_stride));              \
	x_sd0_l = _mm_unpacklo_epi8(x_sd0, x_zero);             \
	x_sd0_h = _mm_unpackhi_epi8(x_sd0, x_zero);             \
	x_sd0s_l = _mm_unpacklo_epi8(x_sd0s, x_zero);             \
	x_sd0s_h = _mm_unpackhi_epi8(x_sd0s, x_zero);             \
	x_sd1_l = _mm_unpacklo_epi8(x_sd1, x_zero);             \
	x_sd1_h = _mm_unpackhi_epi8(x_sd1, x_zero);             \
	x_sd1s_l = _mm_unpacklo_epi8(x_sd1s, x_zero);             \
	x_sd1s_h = _mm_unpackhi_epi8(x_sd1s, x_zero);             \
	\
	x_d0_3_l = x_sd0_l; \
	x_d0_3_h = x_sd0_h; \
	\
	x_sd0_l = _mm_srli_epi16(x_sd0_l, 1);                    \
	x_sd0s_l = _mm_srli_epi16(x_sd0s_l, 1);                  \
	x_sd1_l = _mm_srli_epi16(x_sd1_l, 1);                    \
	x_sd1s_l = _mm_srli_epi16(x_sd1s_l, 1);                  \
	x_sd0_h = _mm_srli_epi16(x_sd0_h, 1);                    \
	x_sd0s_h = _mm_srli_epi16(x_sd0s_h, 1);                  \
	x_sd1_h = _mm_srli_epi16(x_sd1_h, 1);                    \
	x_sd1s_h = _mm_srli_epi16(x_sd1s_h, 1);                  \
	\
	x_sd00s_l = _mm_add_epi16(x_sd0_l, x_sd0s_l);                  \
	x_sd00s_h = _mm_add_epi16(x_sd0_h, x_sd0s_h);                  \
	x_sd01_l = _mm_add_epi16(x_sd0_l, x_sd1_l);                    \
	x_sd01_h = _mm_add_epi16(x_sd0_h, x_sd1_h);                    \
	x_sd11s_l = _mm_add_epi16(x_sd1_l, x_sd1s_l);                  \
	x_sd11s_h = _mm_add_epi16(x_sd1_h, x_sd1s_h);                  \
	x_d1_3_l = x_sd01_l; \
	x_d1_3_h = x_sd01_h; \
	x_d0s_3_l = x_sd00s_l; \
	x_d0s_3_h = x_sd00s_h; \
	\
	x_sd00s_l = _mm_srli_epi16(x_sd00s_l, 1);                  \
	x_sd00s_h = _mm_srli_epi16(x_sd00s_h, 1);                  \
	x_sd11s_l = _mm_srli_epi16(x_sd11s_l, 1);                  \
	x_sd11s_h = _mm_srli_epi16(x_sd11s_h, 1);                  \
\
	x_d1s_3_l = _mm_add_epi16(x_sd00s_l, x_sd11s_l); \
	x_d1s_3_h = _mm_add_epi16(x_sd00s_h, x_sd11s_h); \
	\
	MLIB_S_ZOOM_FL_U8_PACK;                                 \
	MLIB_S_ZOOM_FL_STORE(16, 12)

#define	MLIB_S_ZOOM_BL(TT, STYPE, N, SFT)                                \
	void                                                             \
	mlib_s_ImageZoomIn2X_##TT##_BL_FL##N(STYPE * pdst,               \
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
			dp[0] = MLIB_ZOOMIN2X_DR_FL##N;                  \
			dp += channels;                                  \
			a00 = a01;                                       \
			a10 = a11;                                       \
		    }                                                    \
		    for (; j <= src_width - 2; j += 2) {                 \
			a01 = sp[0];                                     \
			a11 = sp[src_stride];                            \
			sp += channels;                                  \
			dp[0] = MLIB_ZOOMIN2X_DL_FL##N;                  \
			dp[channels] = MLIB_ZOOMIN2X_DR_FL##N;           \
			dp += 2 * channels;                              \
			a00 = a01;                                       \
			a10 = a11;                                       \
		    }                                                    \
		    if (j < src_width) {                                 \
			a01 = sp[0];                                     \
			a11 = sp[src_stride];                            \
			dp[0] = MLIB_ZOOMIN2X_DL_FL##N;                  \
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
			dp[0] = MLIB_ZOOMIN2X_UR_FL##N;                  \
			dp[dst_stride] = MLIB_ZOOMIN2X_DR_FL##N;         \
			dp += 2 * dst_stride;                            \
			a00 = a10;                                       \
			a01 = a11;                                       \
		    }                                                    \
		    if (src_height & 1) {                                \
			a10 = sp[0];                                     \
			a11 = sp[channels];                              \
			dp[0] = MLIB_ZOOMIN2X_UR_FL##N;                  \
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
	    if (channels == 3) {                                         \
		for (i = 0; i < (src_height / 2); i++) {                 \
		    const STYPE *sp = psrc + i * src_stride;             \
		    STYPE *dp = pdst + 2 * i * dst_stride;               \
		    for (j = 0; j < (src_width / 4); j++) {              \
			MLIB_S_ZOOM_S16_FL##N(SFT);                      \
		    }                                                    \
		    if (src_width & 2) {                                 \
			for (k = 0; k < channels; k++) {                 \
			    sp = psrc + i * src_stride + j * 6 + k;      \
			    dp = pdst + 2 * i * dst_stride + j * 12 + k; \
			    INIT_FL##N();                                \
			    ZOOM_FL##N(a, b);                            \
			}                                                \
		    }                                                    \
		}                                                        \
	    } else {                                                     \
		for (i = 0; i < (src_height / 2); i++) {                 \
		    const STYPE *sp = psrc + i * src_stride;             \
		    STYPE *dp = pdst + 2 * i * dst_stride;               \
		    for (j = 0; j < ((src_width * channels) / 16); j++) {\
			MLIB_S_ZOOM_S16_FL##N(SFT);                      \
		    }                                                    \
		    for (k = 0; k < channels; k++) {                     \
			sp = psrc + i * src_stride + j * 8 + k;          \
			dp = pdst + 2 * i * dst_stride + j * 16 + k;     \
			INIT_FL##N();                                    \
			for (int m = j * (4 / channels);                 \
			    m < src_width / 4; m++) {                    \
			    ZOOM_FL##N(a, b);                            \
			    ZOOM_FL##N(b, a);                            \
			}                                                \
			if (src_width & 2) {                             \
			    ZOOM_FL##N(a, b);                            \
			}                                                \
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
			dp[0] = MLIB_ZOOMIN2X_UL_FL##N;                  \
			dp[channels] = MLIB_ZOOMIN2X_UR_FL##N;           \
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
			    psrc + channels * (src_width / 2) + k;       \
		    STYPE *dp = pdst + channels * (src_width - 1) + k;   \
	                                                                 \
		    a00 = sp[0];                                         \
		    a01 = sp[channels];                                  \
		    sp += src_stride;                                    \
		    for (i = 0; i < (src_height / 2); i++) {             \
			a10 = sp[0];                                     \
			a11 = sp[channels];                              \
			sp += src_stride;                                \
			dp[0] = MLIB_ZOOMIN2X_UL_FL##N;                  \
			dp[dst_stride] = MLIB_ZOOMIN2X_DL_FL##N;         \
			dp += 2 * dst_stride;                            \
			a00 = a10;                                       \
			a01 = a11;                                       \
		    }                                                    \
		    if (src_height & 1) {                                \
			a10 = sp[0];                                     \
			a11 = sp[channels];                              \
			dp[0] = MLIB_ZOOMIN2X_UL_FL##N;                  \
		    }                                                    \
		}                                                        \
	    }                                                            \
	}

#define	FUNC_ZOOM_BL(TT, STYPE, N)                                       \
	void                                                             \
	mlib_s_ImageZoomIn2X_##TT##_BL_FL##N(STYPE * pdst,               \
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
			dp[0] = MLIB_ZOOMIN2X_DR_FL##N;                  \
			dp += channels;                                  \
			a00 = a01;                                       \
			a10 = a11;                                       \
		    }                                                    \
		    for (; j <= src_width - 2; j += 2) {                 \
			a01 = sp[0];                                     \
			a11 = sp[src_stride];                            \
			sp += channels;                                  \
			dp[0] = MLIB_ZOOMIN2X_DL_FL##N;                  \
			dp[channels] = MLIB_ZOOMIN2X_DR_FL##N;           \
			dp += 2 * channels;                              \
			a00 = a01;                                       \
			a10 = a11;                                       \
		    }                                                    \
		    if (j < src_width) {                                 \
			a01 = sp[0];                                     \
			a11 = sp[src_stride];                            \
			dp[0] = MLIB_ZOOMIN2X_DL_FL##N;                  \
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
			dp[0] = MLIB_ZOOMIN2X_UR_FL##N;                  \
			dp[dst_stride] = MLIB_ZOOMIN2X_DR_FL##N;         \
			dp += 2 * dst_stride;                            \
			a00 = a10;                                       \
			a01 = a11;                                       \
		    }                                                    \
		    if (src_height & 1) {                                \
			a10 = sp[0];                                     \
			a11 = sp[channels];                              \
			dp[0] = MLIB_ZOOMIN2X_UR_FL##N;                  \
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
	    if ((channels == 3)) {                                       \
		for (i = 0; i < (src_height / 2); i++) {                 \
		    const STYPE *sp = psrc + i * src_stride;             \
		    STYPE *dp = pdst + 2 * i * dst_stride;               \
		    for (j = 0; j < (src_width / 8); j++) {              \
			    MLIB_S_ZOOM_U8_FL##N;                        \
		    }                                                    \
		    for (k = 0; k < channels; k++) {                     \
			sp = psrc + i * src_stride + j * 12 + k;         \
			dp = pdst + 2 * i * dst_stride + j * 24 + k;     \
			INIT_FL##N();                                    \
			for (int m = j * 2;                              \
			    m < src_width / 4; m++) {                    \
			    ZOOM_FL##N(a, b);                            \
			    ZOOM_FL##N(b, a);                            \
			}                                                \
			if (src_width & 2) {                             \
			    ZOOM_FL##N(a, b);                            \
			}                                                \
		    }                                                    \
		}                                                        \
		} else {                                                 \
		for (i = 0; i < (src_height / 2); i++) {                 \
		    const STYPE *sp = psrc + i * src_stride;             \
		    STYPE *dp = pdst + 2 * i * dst_stride;               \
		    for (j = 0; j < ((src_width * channels) / 32); j++) {\
			MLIB_S_ZOOM_U8_FL##N;                            \
		    }                                                    \
		    for (k = 0; k < channels; k++) {                     \
			sp = psrc + i * src_stride + j * 16 + k;         \
			dp = pdst + 2 * i * dst_stride + j * 32 + k;     \
			INIT_FL##N();                                    \
			for (int m = j * (8 / channels);                 \
			    m < src_width / 4; m++) {                    \
			    ZOOM_FL##N(a, b);                            \
			    ZOOM_FL##N(b, a);                            \
			}                                                \
			if (src_width & 2) {                             \
			    ZOOM_FL##N(a, b);                            \
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
			dp[0] = MLIB_ZOOMIN2X_UL_FL##N;                  \
			dp[channels] = MLIB_ZOOMIN2X_UR_FL##N;           \
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
			    psrc + channels * (src_width / 2) + k;       \
		    STYPE *dp = pdst + channels * (src_width - 1) + k;   \
	                                                                 \
		    a00 = sp[0];                                         \
		    a01 = sp[channels];                                  \
		    sp += src_stride;                                    \
		    for (i = 0; i < (src_height / 2); i++) {             \
			a10 = sp[0];                                     \
			a11 = sp[channels];                              \
			sp += src_stride;                                \
			dp[0] = MLIB_ZOOMIN2X_UL_FL##N;                  \
			dp[dst_stride] = MLIB_ZOOMIN2X_DL_FL##N;         \
			dp += 2 * dst_stride;                            \
			a00 = a10;                                       \
			a01 = a11;                                       \
		    }                                                    \
		    if (src_height & 1) {                                \
			a10 = sp[0];                                     \
			a11 = sp[channels];                              \
			dp[0] = MLIB_ZOOMIN2X_UL_FL##N;                  \
		    }                                                    \
		}                                                        \
	    }                                                            \
	}

/* *********************************************************** */

FUNC_ZOOM_BL(U8, mlib_u8,
	0)
FUNC_ZOOM_BL(
	U8,
	mlib_u8,
	1)
FUNC_ZOOM_BL(
	U8,
	mlib_u8,
	2)
FUNC_ZOOM_BL(
	U8,
	mlib_u8,
	3)

MLIB_S_ZOOM_BL(
	S16,
	mlib_s16,
	0,
	srai)
MLIB_S_ZOOM_BL(
	S16,
	mlib_s16,
	1,
	srai)
MLIB_S_ZOOM_BL(
	S16,
	mlib_s16,
	2,
	srai)
MLIB_S_ZOOM_BL(
	S16,
	mlib_s16,
	3,
	srai)

MLIB_S_ZOOM_BL(
	U16,
	mlib_u16,
	0,
	srli)
MLIB_S_ZOOM_BL(
	U16,
	mlib_u16,
	1,
	srli)
MLIB_S_ZOOM_BL(
	U16,
	mlib_u16,
	2,
	srli)
MLIB_S_ZOOM_BL(
	U16,
	mlib_u16,
	3,
	srli)

/* *********************************************************** */
