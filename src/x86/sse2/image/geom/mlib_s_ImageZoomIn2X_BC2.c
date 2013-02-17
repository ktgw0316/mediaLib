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
 * Copyright 2006 Sun Microsystems, Inc.  All rights reserved.
 * Use is subject to license terms.
 */

#pragma ident	"@(#)mlib_s_ImageZoomIn2X_BC2.c	9.2	07/11/05 SMI"

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
 *    MLIB_BICUBIC2
 *
 *  The edge condition can be one of the following:
 *    MLIB_EDGE_DST_NO_WRITE  (default)
 *    MLIB_EDGE_DST_FILL_ZERO
 *    MLIB_EDGE_OP_NEAREST
 *    MLIB_EDGE_SRC_EXTEND
 *    MLIB_EDGE_SRC_PADDED
 */

#include <mlib_image.h>
#include <mlib_ImageZoomIn2X.h>
#include <mlib_ImageZoom2X.h>
#ifdef __SUNPRO_C
#include <sunmedia_intrin.h>
#else
#include <emmintrin.h>
#endif

/* *********************************************************** */

#define	STORE_U8(res, x)	res = mlib_satU8[x]

/* *********************************************************** */

#define	STORE_S16(res, x)                                       \
	{                                                       \
	    mlib_s32 mask =                                     \
		*((mlib_u32 *)((mlib_u8 *)mlib_mask_s16 +       \
		(((x) >> 13) & 0xC)));                          \
	    res = ((x) & mask) | (mask >> 16);                  \
	}

/* *********************************************************** */

#define	STORE_U16(res, x)                                       \
	{                                                       \
	    mlib_s32 mask =                                     \
		*((mlib_u32 *)((mlib_u8 *)mlib_mask_u16 +       \
		(((x) >> 14) & 0xC)));                          \
	    res = ((x) & mask) | (mask >> 16);                  \
	}

/* *********************************************************** */

#define	MLIB_S_MULCONST                               \
	__m128i x_a_l, x_b_l, x_c_l, x_d_l;           \
	__m128i x_a_h, x_b_h, x_c_h, x_d_h;           \
	__m128i x_a_sl, x_b_sl, x_c_sl, x_d_sl;       \
	__m128i x_a_sh, x_b_sh, x_c_sh, x_d_sh;       \
	__m128 x_fa, x_fb, x_fc, x_fd, x_fe;          \
	__m128 x_fma, x_fmb, x_fmc, x_fmd;            \
	__m128 x_fna, x_fnb, x_fnc, x_fnd;            \
	const __m128i x_max_u16 =                     \
	    _mm_set1_epi32(MLIB_U16_MAX);             \
	const __m128i x_zero = _mm_setzero_si128();   \
	const __m128i x_mul1 = _mm_set1_epi16(1);     \
	const __m128i x_mul3 = _mm_set1_epi16(3);     \
	const __m128i x_mul5 = _mm_set1_epi16(5);     \
	const __m128i x_mul9 = _mm_set1_epi16(9);     \
	const __m128i x_mul19 = _mm_set1_epi16(19);   \
	const __m128i x_mul57 = _mm_set1_epi16(57);   \
	const __m128 x_fmul3 = _mm_set1_ps(3.0);      \
	const __m128 x_fmul5 = _mm_set1_ps(5.0);      \
	const __m128 x_fmul9 = _mm_set1_ps(9.0);      \
	const __m128 x_fmul19 = _mm_set1_ps(19.0);    \
	const __m128 x_fmul57 = _mm_set1_ps(57.0)

/* *********************************************************** */

#define	MLIB_S_SUM_L_A0_U16(a, b, c, d, e, f)       \
	x_a_l = _mm_mullo_epi16(a, x_mul9);         \
	x_a_h = _mm_mulhi_epu16(a, x_mul9);         \
	x_b_l = _mm_mullo_epi16(b, x_mul57);        \
	x_b_h = _mm_mulhi_epu16(b, x_mul57);        \
	x_c_l = _mm_mullo_epi16(c, x_mul19);        \
	x_c_h = _mm_mulhi_epu16(c, x_mul19);        \
	x_d_l = _mm_mullo_epi16(d, x_mul3);         \
	x_d_h = _mm_mulhi_epu16(d, x_mul3);         \
	x_a_sl = _mm_unpacklo_epi16(x_a_l, x_a_h);  \
	x_b_sl = _mm_unpacklo_epi16(x_b_l, x_b_h);  \
	x_c_sl = _mm_unpacklo_epi16(x_c_l, x_c_h);  \
	x_d_sl = _mm_unpacklo_epi16(x_d_l, x_d_h);  \
	e = _mm_add_epi32(                          \
	    _mm_sub_epi32(x_b_sl, x_a_sl),          \
	    _mm_sub_epi32(x_c_sl, x_d_sl));         \

#define	MLIB_S_SUM_A0_U16(a, b, c, d, e, f)         \
	x_a_l = _mm_mullo_epi16(a, x_mul9);         \
	x_a_h = _mm_mulhi_epu16(a, x_mul9);         \
	x_b_l = _mm_mullo_epi16(b, x_mul57);        \
	x_b_h = _mm_mulhi_epu16(b, x_mul57);        \
	x_c_l = _mm_mullo_epi16(c, x_mul19);        \
	x_c_h = _mm_mulhi_epu16(c, x_mul19);        \
	x_d_l = _mm_mullo_epi16(d, x_mul3);         \
	x_d_h = _mm_mulhi_epu16(d, x_mul3);         \
	x_a_sl = _mm_unpacklo_epi16(x_a_l, x_a_h);  \
	x_a_sh = _mm_unpackhi_epi16(x_a_l, x_a_h);  \
	x_b_sl = _mm_unpacklo_epi16(x_b_l, x_b_h);  \
	x_b_sh = _mm_unpackhi_epi16(x_b_l, x_b_h);  \
	x_c_sl = _mm_unpacklo_epi16(x_c_l, x_c_h);  \
	x_c_sh = _mm_unpackhi_epi16(x_c_l, x_c_h);  \
	x_d_sl = _mm_unpacklo_epi16(x_d_l, x_d_h);  \
	x_d_sh = _mm_unpackhi_epi16(x_d_l, x_d_h);  \
	e = _mm_add_epi32(                          \
	    _mm_sub_epi32(x_b_sl, x_a_sl),          \
	    _mm_sub_epi32(x_c_sl, x_d_sl));         \
	f = _mm_add_epi32(                          \
	    _mm_sub_epi32(x_b_sh, x_a_sh),          \
	    _mm_sub_epi32(x_c_sh, x_d_sh));         \

#define	MLIB_S_SUM_L_A0_S16(a, b, c, d, e, f)       \
	x_a_l = _mm_mullo_epi16(a, x_mul9);         \
	x_a_h = _mm_mulhi_epi16(a, x_mul9);         \
	x_b_l = _mm_mullo_epi16(b, x_mul57);        \
	x_b_h = _mm_mulhi_epi16(b, x_mul57);        \
	x_c_l = _mm_mullo_epi16(c, x_mul19);        \
	x_c_h = _mm_mulhi_epi16(c, x_mul19);        \
	x_d_l = _mm_mullo_epi16(d, x_mul3);         \
	x_d_h = _mm_mulhi_epi16(d, x_mul3);         \
	x_a_sl = _mm_unpacklo_epi16(x_a_l, x_a_h);  \
	x_b_sl = _mm_unpacklo_epi16(x_b_l, x_b_h);  \
	x_c_sl = _mm_unpacklo_epi16(x_c_l, x_c_h);  \
	x_d_sl = _mm_unpacklo_epi16(x_d_l, x_d_h);  \
	e = _mm_add_epi32(                          \
	    _mm_sub_epi32(x_b_sl, x_a_sl),          \
	    _mm_sub_epi32(x_c_sl, x_d_sl));         \

#define	MLIB_S_SUM_A0_S16(a, b, c, d, e, f)         \
	x_a_l = _mm_mullo_epi16(a, x_mul9);         \
	x_a_h = _mm_mulhi_epi16(a, x_mul9);         \
	x_b_l = _mm_mullo_epi16(b, x_mul57);        \
	x_b_h = _mm_mulhi_epi16(b, x_mul57);        \
	x_c_l = _mm_mullo_epi16(c, x_mul19);        \
	x_c_h = _mm_mulhi_epi16(c, x_mul19);        \
	x_d_l = _mm_mullo_epi16(d, x_mul3);         \
	x_d_h = _mm_mulhi_epi16(d, x_mul3);         \
	x_a_sl = _mm_unpacklo_epi16(x_a_l, x_a_h);  \
	x_a_sh = _mm_unpackhi_epi16(x_a_l, x_a_h);  \
	x_b_sl = _mm_unpacklo_epi16(x_b_l, x_b_h);  \
	x_b_sh = _mm_unpackhi_epi16(x_b_l, x_b_h);  \
	x_c_sl = _mm_unpacklo_epi16(x_c_l, x_c_h);  \
	x_c_sh = _mm_unpackhi_epi16(x_c_l, x_c_h);  \
	x_d_sl = _mm_unpacklo_epi16(x_d_l, x_d_h);  \
	x_d_sh = _mm_unpackhi_epi16(x_d_l, x_d_h);  \
	e = _mm_add_epi32(                          \
	    _mm_sub_epi32(x_b_sl, x_a_sl),          \
	    _mm_sub_epi32(x_c_sl, x_d_sl));         \
	f = _mm_add_epi32(                          \
	    _mm_sub_epi32(x_b_sh, x_a_sh),          \
	    _mm_sub_epi32(x_c_sh, x_d_sh));         \

#define	MLIB_S_SUM_L_B0_U16(a, b, c, d, e, f)	    \
	x_a_l = _mm_mullo_epi16(a, x_mul3);         \
	x_a_h = _mm_mulhi_epu16(a, x_mul3);         \
	x_b_l = _mm_mullo_epi16(b, x_mul19);        \
	x_b_h = _mm_mulhi_epu16(b, x_mul19);        \
	x_c_l = _mm_mullo_epi16(c, x_mul57);        \
	x_c_h = _mm_mulhi_epu16(c, x_mul57);        \
	x_d_l = _mm_mullo_epi16(d, x_mul9);         \
	x_d_h = _mm_mulhi_epu16(d, x_mul9);         \
	x_a_sl = _mm_unpacklo_epi16(x_a_l, x_a_h);  \
	x_b_sl = _mm_unpacklo_epi16(x_b_l, x_b_h);  \
	x_c_sl = _mm_unpacklo_epi16(x_c_l, x_c_h);  \
	x_d_sl = _mm_unpacklo_epi16(x_d_l, x_d_h);  \
	e = _mm_add_epi32(                          \
	    _mm_sub_epi32(x_b_sl, x_a_sl),          \
	    _mm_sub_epi32(x_c_sl, x_d_sl));         \

#define	MLIB_S_SUM_B0_U16(a, b, c, d, e, f)	    \
	x_a_l = _mm_mullo_epi16(a, x_mul3);         \
	x_a_h = _mm_mulhi_epu16(a, x_mul3);         \
	x_b_l = _mm_mullo_epi16(b, x_mul19);        \
	x_b_h = _mm_mulhi_epu16(b, x_mul19);        \
	x_c_l = _mm_mullo_epi16(c, x_mul57);        \
	x_c_h = _mm_mulhi_epu16(c, x_mul57);        \
	x_d_l = _mm_mullo_epi16(d, x_mul9);         \
	x_d_h = _mm_mulhi_epu16(d, x_mul9);         \
	x_a_sl = _mm_unpacklo_epi16(x_a_l, x_a_h);  \
	x_a_sh = _mm_unpackhi_epi16(x_a_l, x_a_h);  \
	x_b_sl = _mm_unpacklo_epi16(x_b_l, x_b_h);  \
	x_b_sh = _mm_unpackhi_epi16(x_b_l, x_b_h);  \
	x_c_sl = _mm_unpacklo_epi16(x_c_l, x_c_h);  \
	x_c_sh = _mm_unpackhi_epi16(x_c_l, x_c_h);  \
	x_d_sl = _mm_unpacklo_epi16(x_d_l, x_d_h);  \
	x_d_sh = _mm_unpackhi_epi16(x_d_l, x_d_h);  \
	e = _mm_add_epi32(                          \
	    _mm_sub_epi32(x_b_sl, x_a_sl),          \
	    _mm_sub_epi32(x_c_sl, x_d_sl));         \
	f = _mm_add_epi32(                          \
	    _mm_sub_epi32(x_b_sh, x_a_sh),          \
	    _mm_sub_epi32(x_c_sh, x_d_sh))

#define	MLIB_S_SUM_L_B0_S16(a, b, c, d, e, f)	    \
	x_a_l = _mm_mullo_epi16(a, x_mul3);         \
	x_a_h = _mm_mulhi_epi16(a, x_mul3);         \
	x_b_l = _mm_mullo_epi16(b, x_mul19);        \
	x_b_h = _mm_mulhi_epi16(b, x_mul19);        \
	x_c_l = _mm_mullo_epi16(c, x_mul57);        \
	x_c_h = _mm_mulhi_epi16(c, x_mul57);        \
	x_d_l = _mm_mullo_epi16(d, x_mul9);         \
	x_d_h = _mm_mulhi_epi16(d, x_mul9);         \
	x_a_sl = _mm_unpacklo_epi16(x_a_l, x_a_h);  \
	x_b_sl = _mm_unpacklo_epi16(x_b_l, x_b_h);  \
	x_c_sl = _mm_unpacklo_epi16(x_c_l, x_c_h);  \
	x_d_sl = _mm_unpacklo_epi16(x_d_l, x_d_h);  \
	e = _mm_add_epi32(                          \
	    _mm_sub_epi32(x_b_sl, x_a_sl),          \
	    _mm_sub_epi32(x_c_sl, x_d_sl));         \

#define	MLIB_S_SUM_B0_S16(a, b, c, d, e, f)	    \
	x_a_l = _mm_mullo_epi16(a, x_mul3);         \
	x_a_h = _mm_mulhi_epi16(a, x_mul3);         \
	x_b_l = _mm_mullo_epi16(b, x_mul19);        \
	x_b_h = _mm_mulhi_epi16(b, x_mul19);        \
	x_c_l = _mm_mullo_epi16(c, x_mul57);        \
	x_c_h = _mm_mulhi_epi16(c, x_mul57);        \
	x_d_l = _mm_mullo_epi16(d, x_mul9);         \
	x_d_h = _mm_mulhi_epi16(d, x_mul9);         \
	x_a_sl = _mm_unpacklo_epi16(x_a_l, x_a_h);  \
	x_a_sh = _mm_unpackhi_epi16(x_a_l, x_a_h);  \
	x_b_sl = _mm_unpacklo_epi16(x_b_l, x_b_h);  \
	x_b_sh = _mm_unpackhi_epi16(x_b_l, x_b_h);  \
	x_c_sl = _mm_unpacklo_epi16(x_c_l, x_c_h);  \
	x_c_sh = _mm_unpackhi_epi16(x_c_l, x_c_h);  \
	x_d_sl = _mm_unpacklo_epi16(x_d_l, x_d_h);  \
	x_d_sh = _mm_unpackhi_epi16(x_d_l, x_d_h);  \
	e = _mm_add_epi32(                          \
	    _mm_sub_epi32(x_b_sl, x_a_sl),          \
	    _mm_sub_epi32(x_c_sl, x_d_sl));         \
	f = _mm_add_epi32(                          \
	    _mm_sub_epi32(x_b_sh, x_a_sh),          \
	    _mm_sub_epi32(x_c_sh, x_d_sh))

#define	MLIB_S_SUM_AA0(x_fa, x_fb, x_fc, x_fd, e, t)\
	x_fmb = _mm_mul_ps(x_fmul57, x_fb);         \
	x_fma = _mm_mul_ps(x_fmul9, x_fa);          \
	x_fmc = _mm_mul_ps(x_fmul19, x_fc);         \
	x_fmd = _mm_mul_ps(x_fmul3, x_fd);          \
	x_fmb = _mm_sub_ps(x_fmb, x_fma);           \
	x_fmc = _mm_sub_ps(x_fmc, x_fmd);           \
	x_fe = _mm_add_ps(x_fmb, x_fmc);            \
	e = _mm_cvtps_epi32(x_fe);                  \
	e = _mm_srai_epi32(e, t)

#define	MLIB_S_SUM_BB0(x_fa, x_fb, x_fc, x_fd, e, t)\
	x_fmc = _mm_mul_ps(x_fmul57, x_fc);         \
	x_fmd = _mm_mul_ps(x_fmul9, x_fd);          \
	x_fmb = _mm_mul_ps(x_fmul19, x_fb);         \
	x_fma = _mm_mul_ps(x_fmul3, x_fa);          \
	x_fmb = _mm_sub_ps(x_fmb, x_fma);           \
	x_fmc = _mm_sub_ps(x_fmc, x_fmd);           \
	x_fe = _mm_add_ps(x_fmb, x_fmc);            \
	e = _mm_srai_epi32(_mm_cvtps_epi32(x_fe), t)

/* *********************************************************** */

#define	MLIB_S_SUM_L_A1_U16(a, b, c, d, e, f)	\
	x_b_l = _mm_unpacklo_epi16(b, x_zero);  \
	e = x_b_l;                              \

#define	MLIB_S_SUM_A1_U16(a, b, c, d, e, f)	\
	x_b_l = _mm_unpacklo_epi16(b, x_zero);  \
	x_b_h = _mm_unpackhi_epi16(b, x_zero);  \
	e = x_b_l;                              \
	f = x_b_h

#define	MLIB_S_SUM_L_A1_S16(a, b, c, d, e, f)	\
	x_b_l = _mm_unpacklo_epi16(b,           \
	    _mm_srai_epi16(b, 15));             \
	e = x_b_l;                              \

#define	MLIB_S_SUM_A1_S16(a, b, c, d, e, f)	\
	x_b_l = _mm_unpacklo_epi16(b,           \
	    _mm_srai_epi16(b, 15));             \
	x_b_h = _mm_unpackhi_epi16(b,           \
	    _mm_srai_epi16(b, 15));             \
	e = x_b_l;                              \
	f = x_b_h

#define	MLIB_S_SUM_AA1(x_fa, x_fb, x_fc, x_fd, e, t)	\
	e = _mm_srai_epi32(_mm_cvtps_epi32(x_fb), t)

#define	MLIB_S_SUM_L_B1_U16(a, b, c, d, e, f)	    \
	x_b_l = _mm_mullo_epi16(b, x_mul5);         \
	x_b_h = _mm_mulhi_epu16(b, x_mul5);         \
	x_c_l = _mm_mullo_epi16(c, x_mul5);         \
	x_c_h = _mm_mulhi_epu16(c, x_mul5);         \
	x_b_sl = _mm_unpacklo_epi16(x_b_l, x_b_h);  \
	x_c_sl = _mm_unpacklo_epi16(x_c_l, x_c_h);  \
	x_b_sl = _mm_add_epi32(x_b_sl, x_c_sl);     \
	x_a_l = _mm_unpacklo_epi16(a, x_zero);      \
	x_d_l = _mm_unpacklo_epi16(d, x_zero);      \
	x_a_sl = _mm_add_epi32(x_a_l, x_d_l);       \
	e = _mm_sub_epi32(x_b_sl, x_a_sl);          \

#define	MLIB_S_SUM_B1_U16(a, b, c, d, e, f)	    \
	x_b_l = _mm_mullo_epi16(b, x_mul5);         \
	x_b_h = _mm_mulhi_epu16(b, x_mul5);         \
	x_c_l = _mm_mullo_epi16(c, x_mul5);         \
	x_c_h = _mm_mulhi_epu16(c, x_mul5);         \
	x_b_sl = _mm_unpacklo_epi16(x_b_l, x_b_h);  \
	x_b_sh = _mm_unpackhi_epi16(x_b_l, x_b_h);  \
	x_c_sl = _mm_unpacklo_epi16(x_c_l, x_c_h);  \
	x_c_sh = _mm_unpackhi_epi16(x_c_l, x_c_h);  \
	x_b_sl = _mm_add_epi32(x_b_sl, x_c_sl);     \
	x_b_sh = _mm_add_epi32(x_b_sh, x_c_sh);     \
	x_a_l = _mm_unpacklo_epi16(a, x_zero);      \
	x_a_h = _mm_unpackhi_epi16(a, x_zero);      \
	x_d_l = _mm_unpacklo_epi16(d, x_zero);      \
	x_d_h = _mm_unpackhi_epi16(d, x_zero);      \
	x_a_sl = _mm_add_epi32(x_a_l, x_d_l);       \
	x_a_sh = _mm_add_epi32(x_a_h, x_d_h);       \
	e = _mm_sub_epi32(x_b_sl, x_a_sl);          \
	f = _mm_sub_epi32(x_b_sh, x_a_sh)

#define	MLIB_S_SUM_L_B1_S16(a, b, c, d, e, f)	    \
	x_b_l = _mm_mullo_epi16(b, x_mul5);         \
	x_b_h = _mm_mulhi_epi16(b, x_mul5);         \
	x_c_l = _mm_mullo_epi16(c, x_mul5);         \
	x_c_h = _mm_mulhi_epi16(c, x_mul5);         \
	x_b_sl = _mm_unpacklo_epi16(x_b_l, x_b_h);  \
	x_c_sl = _mm_unpacklo_epi16(x_c_l, x_c_h);  \
	x_b_sl = _mm_add_epi32(x_b_sl, x_c_sl);     \
	x_a_l = _mm_unpacklo_epi16(a,               \
	    _mm_srai_epi16(a, 15));                 \
	x_d_l = _mm_unpacklo_epi16(d,               \
	    _mm_srai_epi16(d, 15));                 \
	x_a_sl = _mm_add_epi32(x_a_l, x_d_l);       \
	e = _mm_sub_epi32(x_b_sl, x_a_sl);          \

#define	MLIB_S_SUM_B1_S16(a, b, c, d, e, f)	    \
	x_b_l = _mm_mullo_epi16(b, x_mul5);         \
	x_b_h = _mm_mulhi_epi16(b, x_mul5);         \
	x_c_l = _mm_mullo_epi16(c, x_mul5);         \
	x_c_h = _mm_mulhi_epi16(c, x_mul5);         \
	x_b_sl = _mm_unpacklo_epi16(x_b_l, x_b_h);  \
	x_b_sh = _mm_unpackhi_epi16(x_b_l, x_b_h);  \
	x_c_sl = _mm_unpacklo_epi16(x_c_l, x_c_h);  \
	x_c_sh = _mm_unpackhi_epi16(x_c_l, x_c_h);  \
	x_b_sl = _mm_add_epi32(x_b_sl, x_c_sl);     \
	x_b_sh = _mm_add_epi32(x_b_sh, x_c_sh);     \
	x_a_l = _mm_unpacklo_epi16(a,               \
	    _mm_srai_epi16(a, 15));                 \
	x_a_h = _mm_unpackhi_epi16(a,               \
	    _mm_srai_epi16(a, 15));                 \
	x_d_l = _mm_unpacklo_epi16(d,               \
	    _mm_srai_epi16(d, 15));                 \
	x_d_h = _mm_unpackhi_epi16(d,               \
	    _mm_srai_epi16(d, 15));                 \
	x_a_sl = _mm_add_epi32(x_a_l, x_d_l);       \
	x_a_sh = _mm_add_epi32(x_a_h, x_d_h);       \
	e = _mm_sub_epi32(x_b_sl, x_a_sl);          \
	f = _mm_sub_epi32(x_b_sh, x_a_sh)

#define	MLIB_S_SUM_BB1(x_fa, x_fb, x_fc, x_fd, e, t)         \
	x_fma = _mm_add_ps(x_fa, x_fd);                      \
	x_fmb = _mm_mul_ps(x_fmul5, _mm_add_ps(x_fb, x_fc)); \
	x_fe = _mm_sub_ps(x_fmb, x_fma);                     \
	e = _mm_srai_epi32(_mm_cvtps_epi32(x_fe), t)

/* *********************************************************** */

#define	SUM_A0(a, b, c, d)	(57*b - 9*a + 19*c - 3*d)
#define	SUM_B0(a, b, c, d)	(19*b - 3*a + 57*c - 9*d)

#define	SCL_A0	6

#define	SCL_B0	6

/* *********************************************************** */

#define	SUM_A1(a, b, c, d)	(b)
#define	SUM_B1(a, b, c, d)	(5*(b + c) - a - d)

#define	SCL_A1	0

#define	SCL_B1	3

/* *********************************************************** */

#define	MLIB_S_ZOOM_BC2_LINE_LOAD(x_sd0, x_sd1, x_sd2, x_sd3)        \
	x_sd0 = _mm_loadu_si128((__m128i *)(ps - src_stride));       \
	x_sd1 = _mm_loadu_si128((__m128i *)ps);                      \
	x_sd2 = _mm_loadu_si128((__m128i *)(ps + src_stride));       \
	x_sd3 = _mm_loadu_si128((__m128i *)(ps + 2 * src_stride));   \
	ps += 8

/* *********************************************************** */

#define	MLIB_S_SUM_LINE1_U8(H, V)                              \
	x_a_l = x_v0_l;                                        \
	x_b_l = _mm_or_si128(_mm_srli_si128(x_v0_l, 4),        \
	    _mm_slli_si128(x_v0_h, 12));                       \
	x_c_l = _mm_or_si128(_mm_srli_si128(x_v0_l, 8),        \
	    _mm_slli_si128(x_v0_h, 8));                        \
	x_d_l = _mm_or_si128(_mm_srli_si128(x_v0_l, 12),       \
	    _mm_slli_si128(x_v0_h, 4));                        \
	x_fa = _mm_cvtepi32_ps(x_a_l);                         \
	x_fb = _mm_cvtepi32_ps(x_b_l);                         \
	x_fc = _mm_cvtepi32_ps(x_c_l);                         \
	x_fd = _mm_cvtepi32_ps(x_d_l);                         \
	MLIB_S_SUM_AA##H(x_fa, x_fb, x_fc, x_fd, x_r0_l,       \
	    (SCL_##V + SCL_A##H));                             \
	MLIB_S_SUM_BB##H(x_fa, x_fb, x_fc, x_fd, x_r1_l,       \
	    (SCL_##V + SCL_B##H));                             \
	x_a_h = x_v0_h;                                        \
	x_b_h = _mm_or_si128(_mm_srli_si128(x_v0_h, 4),        \
	    _mm_slli_si128(x_v1_l, 12));                       \
	x_c_h = _mm_or_si128(_mm_srli_si128(x_v0_h, 8),        \
	    _mm_slli_si128(x_v1_l, 8));                        \
	x_d_h = _mm_or_si128(_mm_srli_si128(x_v0_h, 12),       \
	    _mm_slli_si128(x_v1_l, 4));                        \
	x_fa = _mm_cvtepi32_ps(x_a_h);                         \
	x_fb = _mm_cvtepi32_ps(x_b_h);                         \
	x_fc = _mm_cvtepi32_ps(x_c_h);                         \
	x_fd = _mm_cvtepi32_ps(x_d_h);                         \
	MLIB_S_SUM_AA##H(x_fa, x_fb, x_fc, x_fd, x_r0_h,       \
	    (SCL_##V + SCL_A##H));                             \
	MLIB_S_SUM_BB##H(x_fa, x_fb, x_fc, x_fd, x_r1_h,       \
	    (SCL_##V + SCL_B##H));                             \
	x_r0 = _mm_packs_epi32(x_r0_l, x_r0_h);                \
	x_r1 = _mm_packs_epi32(x_r1_l, x_r1_h);                \
	x_a_sl = _mm_cmpgt_epi16(x_r0, x_zero);                \
	x_r0 = _mm_and_si128(x_r0, x_a_sl);                    \
	x_a_sh = _mm_cmpgt_epi16(x_r1, x_zero);                \
	x_r1 = _mm_and_si128(x_r1, x_a_sh);                    \
	x_a_l = x_v1_l;                                        \
	x_b_l = _mm_or_si128(_mm_srli_si128(x_v1_l, 4),        \
	    _mm_slli_si128(x_v1_h, 12));                       \
	x_c_l = _mm_or_si128(_mm_srli_si128(x_v1_l, 8),        \
	    _mm_slli_si128(x_v1_h, 8));                        \
	x_d_l = _mm_or_si128(_mm_srli_si128(x_v1_l, 12),       \
	    _mm_slli_si128(x_v1_h, 4));                        \
	x_fa = _mm_cvtepi32_ps(x_a_l);                         \
	x_fb = _mm_cvtepi32_ps(x_b_l);                         \
	x_fc = _mm_cvtepi32_ps(x_c_l);                         \
	x_fd = _mm_cvtepi32_ps(x_d_l);                         \
	MLIB_S_SUM_AA##H(x_fa, x_fb, x_fc, x_fd, x_r2_l,       \
	    (SCL_##V + SCL_A##H));                             \
	MLIB_S_SUM_BB##H(x_fa, x_fb, x_fc, x_fd, x_r3_l,       \
	    (SCL_##V + SCL_B##H));                             \
	x_a_h = x_v1_h;                                        \
	x_b_h = _mm_or_si128(_mm_srli_si128(x_v1_h, 4),        \
	    _mm_slli_si128(x_v2_l, 12));                       \
	x_c_h = _mm_or_si128(_mm_srli_si128(x_v1_h, 8),        \
	    _mm_slli_si128(x_v2_l, 8));                        \
	x_d_h = _mm_or_si128(_mm_srli_si128(x_v1_h, 12),       \
	    _mm_slli_si128(x_v2_l, 4));                        \
	x_fa = _mm_cvtepi32_ps(x_a_h);                         \
	x_fb = _mm_cvtepi32_ps(x_b_h);                         \
	x_fc = _mm_cvtepi32_ps(x_c_h);                         \
	x_fd = _mm_cvtepi32_ps(x_d_h);                         \
	MLIB_S_SUM_AA##H(x_fa, x_fb, x_fc, x_fd, x_r2_h,       \
	    (SCL_##V + SCL_A##H));                             \
	MLIB_S_SUM_BB##H(x_fa, x_fb, x_fc, x_fd, x_r3_h,       \
	    (SCL_##V + SCL_B##H));                             \
	x_r2 = _mm_packs_epi32(x_r2_l, x_r2_h);                \
	x_r3 = _mm_packs_epi32(x_r3_l, x_r3_h);                \
	x_a_sl = _mm_cmpgt_epi16(x_r2, x_zero);                \
	x_r2 = _mm_and_si128(x_r2, x_a_sl);                    \
	x_a_sh = _mm_cmpgt_epi16(x_r3, x_zero);                \
	x_r3 = _mm_and_si128(x_r3, x_a_sh);                    \
	x_r0 = _mm_packus_epi16(x_r0, x_r2);                   \
	x_r1 = _mm_packus_epi16(x_r1, x_r3);                   \

/* *********************************************************** */

#define	MLIB_S_SUM_LINE2_U8(H, V)                              \
	x_a_l = x_v0_l;                                        \
	x_b_l = _mm_or_si128(_mm_srli_si128(x_v0_l, 8),        \
	    _mm_slli_si128(x_v0_h, 8));                        \
	x_c_l = x_v0_h;                                        \
	x_d_l = _mm_or_si128(_mm_srli_si128(x_v0_h, 8),        \
	    _mm_slli_si128(x_v1_l, 8));                        \
	x_fa = _mm_cvtepi32_ps(x_a_l);                         \
	x_fb = _mm_cvtepi32_ps(x_b_l);                         \
	x_fc = _mm_cvtepi32_ps(x_c_l);                         \
	x_fd = _mm_cvtepi32_ps(x_d_l);                         \
	MLIB_S_SUM_AA##H(x_fa, x_fb, x_fc, x_fd, x_r0_l,       \
	    (SCL_##V + SCL_A##H));                             \
	MLIB_S_SUM_BB##H(x_fa, x_fb, x_fc, x_fd, x_r1_l,       \
	    (SCL_##V + SCL_B##H));                             \
	x_a_h = x_v0_h;                                        \
	x_b_h = _mm_or_si128(_mm_srli_si128(x_v0_h, 8),        \
	    _mm_slli_si128(x_v1_l, 8));                        \
	x_c_h = x_v1_l;                                        \
	x_d_h = _mm_or_si128(_mm_srli_si128(x_v1_l, 8),        \
	    _mm_slli_si128(x_v1_h, 8));                        \
	x_fa = _mm_cvtepi32_ps(x_a_h);                         \
	x_fb = _mm_cvtepi32_ps(x_b_h);                         \
	x_fc = _mm_cvtepi32_ps(x_c_h);                         \
	x_fd = _mm_cvtepi32_ps(x_d_h);                         \
	MLIB_S_SUM_AA##H(x_fa, x_fb, x_fc, x_fd, x_r0_h,       \
	    (SCL_##V + SCL_A##H));                             \
	MLIB_S_SUM_BB##H(x_fa, x_fb, x_fc, x_fd, x_r1_h,       \
	    (SCL_##V + SCL_B##H));                             \
	x_r0 = _mm_packs_epi32(x_r0_l, x_r0_h);                \
	x_r1 = _mm_packs_epi32(x_r1_l, x_r1_h);                \
	x_a_sl = _mm_cmpgt_epi16(x_r0, x_zero);                \
	x_r0 = _mm_and_si128(x_r0, x_a_sl);                    \
	x_a_sh = _mm_cmpgt_epi16(x_r1, x_zero);                \
	x_r1 = _mm_and_si128(x_r1, x_a_sh);                    \
	x_a_l = x_v1_l;                                        \
	x_b_l = _mm_or_si128(_mm_srli_si128(x_v1_l, 8),        \
	    _mm_slli_si128(x_v1_h, 8));                        \
	x_c_l = x_v1_h;                                        \
	x_d_l = _mm_or_si128(_mm_srli_si128(x_v1_h, 8),        \
	    _mm_slli_si128(x_v2_l, 8));                        \
	x_fa = _mm_cvtepi32_ps(x_a_l);                         \
	x_fb = _mm_cvtepi32_ps(x_b_l);                         \
	x_fc = _mm_cvtepi32_ps(x_c_l);                         \
	x_fd = _mm_cvtepi32_ps(x_d_l);                         \
	MLIB_S_SUM_AA##H(x_fa, x_fb, x_fc, x_fd, x_r2_l,       \
	    (SCL_##V + SCL_A##H));                             \
	MLIB_S_SUM_BB##H(x_fa, x_fb, x_fc, x_fd, x_r3_l,       \
	    (SCL_##V + SCL_B##H));                             \
	x_a_h = x_v1_h;                                        \
	x_b_h = _mm_or_si128(_mm_srli_si128(x_v1_h, 8),        \
	    _mm_slli_si128(x_v2_l, 8));                        \
	x_c_h = x_v2_l;                                        \
	x_d_h = _mm_or_si128(_mm_srli_si128(x_v2_l, 8),        \
	    _mm_slli_si128(x_v2_h, 8));                        \
	x_fa = _mm_cvtepi32_ps(x_a_h);                         \
	x_fb = _mm_cvtepi32_ps(x_b_h);                         \
	x_fc = _mm_cvtepi32_ps(x_c_h);                         \
	x_fd = _mm_cvtepi32_ps(x_d_h);                         \
	MLIB_S_SUM_AA##H(x_fa, x_fb, x_fc, x_fd, x_r2_h,       \
	    (SCL_##V + SCL_A##H));                             \
	MLIB_S_SUM_BB##H(x_fa, x_fb, x_fc, x_fd, x_r3_h,       \
	    (SCL_##V + SCL_B##H));                             \
	x_r2 = _mm_packs_epi32(x_r2_l, x_r2_h);                \
	x_r3 = _mm_packs_epi32(x_r3_l, x_r3_h);                \
	x_a_sl = _mm_cmpgt_epi16(x_r2, x_zero);                \
	x_r2 = _mm_and_si128(x_r2, x_a_sl);                    \
	x_a_sh = _mm_cmpgt_epi16(x_r3, x_zero);                \
	x_r3 = _mm_and_si128(x_r3, x_a_sh);                    \
	x_r0 = _mm_packus_epi16(x_r0, x_r2);                   \
	x_r1 = _mm_packus_epi16(x_r1, x_r3);                   \

/* *********************************************************** */

#define	MLIB_S_SUM_LINE3_U8(H, V)                              \
	x_a_l = x_v0_l;                                        \
	x_b_l = _mm_or_si128(_mm_srli_si128(x_v0_l, 12),       \
	    _mm_slli_si128(x_v0_h, 4));                        \
	x_c_l = _mm_or_si128(_mm_srli_si128(x_v0_h, 8),        \
	    _mm_slli_si128(x_v1_l, 8));                        \
	x_d_l = _mm_or_si128(_mm_srli_si128(x_v1_l, 4),        \
	    _mm_slli_si128(x_v1_h, 12));                       \
	x_fa = _mm_cvtepi32_ps(x_a_l);                         \
	x_fb = _mm_cvtepi32_ps(x_b_l);                         \
	x_fc = _mm_cvtepi32_ps(x_c_l);                         \
	x_fd = _mm_cvtepi32_ps(x_d_l);                         \
	MLIB_S_SUM_AA##H(x_fa, x_fb, x_fc, x_fd, x_r0_l,       \
	    (SCL_##V + SCL_A##H));                             \
	MLIB_S_SUM_BB##H(x_fa, x_fb, x_fc, x_fd, x_r1_l,       \
	    (SCL_##V + SCL_B##H));                             \
	x_a_l = x_v0_h;                                        \
	x_b_l = _mm_or_si128(_mm_srli_si128(x_v0_h, 12),       \
	    _mm_slli_si128(x_v1_l, 4));                        \
	x_c_l = _mm_or_si128(_mm_srli_si128(x_v1_l, 8),        \
	    _mm_slli_si128(x_v1_h, 8));                        \
	x_d_l = _mm_or_si128(_mm_srli_si128(x_v1_h, 4),        \
	    _mm_slli_si128(x_v2_l, 12));                       \
	x_fa = _mm_cvtepi32_ps(x_a_l);                         \
	x_fb = _mm_cvtepi32_ps(x_b_l);                         \
	x_fc = _mm_cvtepi32_ps(x_c_l);                         \
	x_fd = _mm_cvtepi32_ps(x_d_l);                         \
	MLIB_S_SUM_AA##H(x_fa, x_fb, x_fc, x_fd, x_r0_h,       \
	    (SCL_##V + SCL_A##H));                             \
	MLIB_S_SUM_BB##H(x_fa, x_fb, x_fc, x_fd, x_r1_h,       \
	    (SCL_##V + SCL_B##H));                             \
	x_r0 = _mm_packs_epi32(x_r0_l, x_r0_h);                \
	x_r1 = _mm_packs_epi32(x_r1_l, x_r1_h);                \
	x_a_sl = _mm_cmpgt_epi16(x_r0, x_zero);                \
	x_r0 = _mm_and_si128(x_r0, x_a_sl);                    \
	x_a_sh = _mm_cmpgt_epi16(x_r1, x_zero);                \
	x_r1 = _mm_and_si128(x_r1, x_a_sh);                    \
	x_a_l = x_v1_l;                                        \
	x_b_l = _mm_or_si128(_mm_srli_si128(x_v1_l, 12),       \
	    _mm_slli_si128(x_v1_h, 4));                        \
	x_c_l = _mm_or_si128(_mm_srli_si128(x_v1_h, 8),        \
	    _mm_slli_si128(x_v2_l, 8));                        \
	x_d_l = _mm_or_si128(_mm_srli_si128(x_v2_l, 4),        \
	    _mm_slli_si128(x_v2_h, 12));                       \
	x_fa = _mm_cvtepi32_ps(x_a_l);                         \
	x_fb = _mm_cvtepi32_ps(x_b_l);                         \
	x_fc = _mm_cvtepi32_ps(x_c_l);                         \
	x_fd = _mm_cvtepi32_ps(x_d_l);                         \
	MLIB_S_SUM_AA##H(x_fa, x_fb, x_fc, x_fd, x_r2_l,       \
	    (SCL_##V + SCL_A##H));                             \
	MLIB_S_SUM_BB##H(x_fa, x_fb, x_fc, x_fd, x_r3_l,       \
	    (SCL_##V + SCL_B##H));                             \
	x_a_l = x_v1_h;                                        \
	x_b_l = _mm_or_si128(_mm_srli_si128(x_v1_h, 12),       \
	    _mm_slli_si128(x_v2_l, 4));                        \
	x_c_l = _mm_or_si128(_mm_srli_si128(x_v2_l, 8),        \
	    _mm_slli_si128(x_v2_h, 8));                        \
	x_d_l = _mm_or_si128(_mm_srli_si128(x_v2_h, 4),        \
	    _mm_slli_si128(x_v3_l, 12));                       \
	x_fa = _mm_cvtepi32_ps(x_a_l);                         \
	x_fb = _mm_cvtepi32_ps(x_b_l);                         \
	x_fc = _mm_cvtepi32_ps(x_c_l);                         \
	x_fd = _mm_cvtepi32_ps(x_d_l);                         \
	MLIB_S_SUM_AA##H(x_fa, x_fb, x_fc, x_fd, x_r2_h,       \
	    (SCL_##V + SCL_A##H));                             \
	MLIB_S_SUM_BB##H(x_fa, x_fb, x_fc, x_fd, x_r3_h,       \
	    (SCL_##V + SCL_B##H));                             \
	x_r2 = _mm_packs_epi32(x_r2_l, x_r2_h);                \
	x_r3 = _mm_packs_epi32(x_r3_l, x_r3_h);                \
	x_a_sl = _mm_cmpgt_epi16(x_r2, x_zero);                \
	x_r2 = _mm_and_si128(x_r2, x_a_sl);                    \
	x_a_sh = _mm_cmpgt_epi16(x_r3, x_zero);                \
	x_r3 = _mm_and_si128(x_r3, x_a_sh);                    \
	x_r0 = _mm_packus_epi16(x_r0, x_r2);                   \
	x_r1 = _mm_packus_epi16(x_r1, x_r3);                   \
	x_a_l = x_v2_l;                                        \
	x_b_l = _mm_or_si128(_mm_srli_si128(x_v2_l, 12),       \
	    _mm_slli_si128(x_v2_h, 4));                        \
	x_c_l = _mm_or_si128(_mm_srli_si128(x_v2_h, 8),        \
	    _mm_slli_si128(x_v3_l, 8));                        \
	x_d_l = _mm_or_si128(_mm_srli_si128(x_v3_l, 4),        \
	    _mm_slli_si128(x_v3_h, 12));                       \
	x_fa = _mm_cvtepi32_ps(x_a_l);                         \
	x_fb = _mm_cvtepi32_ps(x_b_l);                         \
	x_fc = _mm_cvtepi32_ps(x_c_l);                         \
	x_fd = _mm_cvtepi32_ps(x_d_l);                         \
	MLIB_S_SUM_AA##H(x_fa, x_fb, x_fc, x_fd, x_r4_l,       \
	    (SCL_##V + SCL_A##H));                             \
	MLIB_S_SUM_BB##H(x_fa, x_fb, x_fc, x_fd, x_r5_l,       \
	    (SCL_##V + SCL_B##H));                             \
	x_a_l = x_v2_h;                                        \
	x_b_l = _mm_or_si128(_mm_srli_si128(x_v2_h, 12),       \
	    _mm_slli_si128(x_v3_l, 4));                        \
	x_c_l = _mm_or_si128(_mm_srli_si128(x_v3_l, 8),        \
	    _mm_slli_si128(x_v3_h, 8));                        \
	x_d_l = _mm_or_si128(_mm_srli_si128(x_v3_h, 4),        \
	    _mm_slli_si128(x_v4_l, 12));                       \
	x_fa = _mm_cvtepi32_ps(x_a_l);                         \
	x_fb = _mm_cvtepi32_ps(x_b_l);                         \
	x_fc = _mm_cvtepi32_ps(x_c_l);                         \
	x_fd = _mm_cvtepi32_ps(x_d_l);                         \
	MLIB_S_SUM_AA##H(x_fa, x_fb, x_fc, x_fd, x_r4_h,       \
	    (SCL_##V + SCL_A##H));                             \
	MLIB_S_SUM_BB##H(x_fa, x_fb, x_fc, x_fd, x_r5_h,       \
	    (SCL_##V + SCL_B##H));                             \
	x_r4 = _mm_packs_epi32(x_r4_l, x_r4_h);                \
	x_r5 = _mm_packs_epi32(x_r5_l, x_r5_h);                \
	x_a_sl = _mm_cmpgt_epi16(x_r4, x_zero);                \
	x_r4 = _mm_and_si128(x_r4, x_a_sl);                    \
	x_a_sh = _mm_cmpgt_epi16(x_r5, x_zero);                \
	x_r5 = _mm_and_si128(x_r5, x_a_sh);                    \
	x_r4 = _mm_packus_epi16(x_r4, x_r4);                   \
	x_r5 = _mm_packus_epi16(x_r5, x_r5);                   \

/* *********************************************************** */

#define	MLIB_S_SUM_LINE4_U8(H, V)                              \
	x_fa = _mm_cvtepi32_ps(x_v0_l);                        \
	x_fb = _mm_cvtepi32_ps(x_v0_h);                        \
	x_fc = _mm_cvtepi32_ps(x_v1_l);                        \
	x_fd = _mm_cvtepi32_ps(x_v1_h);                        \
	x_fna = _mm_cvtepi32_ps(x_v2_l);                       \
	x_fnb = _mm_cvtepi32_ps(x_v2_h);                       \
	x_fnc = _mm_cvtepi32_ps(x_v3_l);                       \
	x_fnd = _mm_cvtepi32_ps(x_v3_h);                       \
	MLIB_S_SUM_AA##H(x_fa, x_fb, x_fc, x_fd, x_r0_l,       \
	    (SCL_##V + SCL_A##H));                             \
	MLIB_S_SUM_BB##H(x_fa, x_fb, x_fc, x_fd, x_r1_l,       \
	    (SCL_##V + SCL_B##H));                             \
	MLIB_S_SUM_AA##H(x_fb, x_fc, x_fd, x_fna, x_r0_h,      \
	    (SCL_##V + SCL_A##H));                             \
	MLIB_S_SUM_BB##H(x_fb, x_fc, x_fd, x_fna, x_r1_h,      \
	    (SCL_##V + SCL_B##H));                             \
	x_r0 = _mm_packs_epi32(x_r0_l, x_r0_h);                \
	x_r1 = _mm_packs_epi32(x_r1_l, x_r1_h);                \
	x_a_sl = _mm_cmpgt_epi16(x_r0, x_zero);                \
	x_r0 = _mm_and_si128(x_r0, x_a_sl);                    \
	x_a_sh = _mm_cmpgt_epi16(x_r1, x_zero);                \
	x_r1 = _mm_and_si128(x_r1, x_a_sh);                    \
	MLIB_S_SUM_AA##H(x_fc, x_fd, x_fna, x_fnb, x_r2_l,     \
	    (SCL_##V + SCL_A##H));                             \
	MLIB_S_SUM_BB##H(x_fc, x_fd, x_fna, x_fnb, x_r3_l,     \
	    (SCL_##V + SCL_B##H));                             \
	MLIB_S_SUM_AA##H(x_fd, x_fna, x_fnb, x_fnc, x_r2_h,    \
	    (SCL_##V + SCL_A##H));                             \
	MLIB_S_SUM_BB##H(x_fd, x_fna, x_fnb, x_fnc, x_r3_h,    \
	    (SCL_##V + SCL_B##H));                             \
	x_r2 = _mm_packs_epi32(x_r2_l, x_r2_h);                \
	x_r3 = _mm_packs_epi32(x_r3_l, x_r3_h);                \
	x_a_sl = _mm_cmpgt_epi16(x_r2, x_zero);                \
	x_r2 = _mm_and_si128(x_r2, x_a_sl);                    \
	x_a_sh = _mm_cmpgt_epi16(x_r3, x_zero);                \
	x_r3 = _mm_and_si128(x_r3, x_a_sh);                    \
	x_r0 = _mm_packus_epi16(x_r0, x_r2);                   \
	x_r1 = _mm_packus_epi16(x_r1, x_r3);                   \

/* *********************************************************** */

#define	MLIB_S_SUM_LINE1(H, V)                                 \
	x_a_l = x_v0_l;                                        \
	x_b_l = _mm_or_si128(_mm_srli_si128(x_v0_l, 4),        \
	    _mm_slli_si128(x_v0_h, 12));                       \
	x_c_l = _mm_or_si128(_mm_srli_si128(x_v0_l, 8),        \
	    _mm_slli_si128(x_v0_h, 8));                        \
	x_d_l = _mm_or_si128(_mm_srli_si128(x_v0_l, 12),       \
	    _mm_slli_si128(x_v0_h, 4));                        \
	x_fa = _mm_cvtepi32_ps(x_a_l);                         \
	x_fb = _mm_cvtepi32_ps(x_b_l);                         \
	x_fc = _mm_cvtepi32_ps(x_c_l);                         \
	x_fd = _mm_cvtepi32_ps(x_d_l);                         \
	MLIB_S_SUM_AA##H(x_fa, x_fb, x_fc, x_fd, x_r0_l,       \
	    (SCL_##V + SCL_A##H));                             \
	MLIB_S_SUM_BB##H(x_fa, x_fb, x_fc, x_fd, x_r1_l,       \
	    (SCL_##V + SCL_B##H));                             \
	x_a_h = x_v0_h;                                        \
	x_b_h = _mm_or_si128(_mm_srli_si128(x_v0_h, 4),        \
	    _mm_slli_si128(x_v1_l, 12));                       \
	x_c_h = _mm_or_si128(_mm_srli_si128(x_v0_h, 8),        \
	    _mm_slli_si128(x_v1_l, 8));                        \
	x_d_h = _mm_or_si128(_mm_srli_si128(x_v0_h, 12),       \
	    _mm_slli_si128(x_v1_l, 4));                        \
	x_fa = _mm_cvtepi32_ps(x_a_h);                         \
	x_fb = _mm_cvtepi32_ps(x_b_h);                         \
	x_fc = _mm_cvtepi32_ps(x_c_h);                         \
	x_fd = _mm_cvtepi32_ps(x_d_h);                         \
	MLIB_S_SUM_AA##H(x_fa, x_fb, x_fc, x_fd, x_r0_h,       \
	    (SCL_##V + SCL_A##H));                             \
	MLIB_S_SUM_BB##H(x_fa, x_fb, x_fc, x_fd, x_r1_h,       \
	    (SCL_##V + SCL_B##H));                             \
	x_a_l = x_v1_l;                                        \
	x_b_l = _mm_or_si128(_mm_srli_si128(x_v1_l, 4),        \
	    _mm_slli_si128(x_v1_h, 12));                       \
	x_c_l = _mm_or_si128(_mm_srli_si128(x_v1_l, 8),        \
	    _mm_slli_si128(x_v1_h, 8));                        \
	x_d_l = _mm_or_si128(_mm_srli_si128(x_v1_l, 12),       \
	    _mm_slli_si128(x_v1_h, 4));                        \
	x_fa = _mm_cvtepi32_ps(x_a_l);                         \
	x_fb = _mm_cvtepi32_ps(x_b_l);                         \
	x_fc = _mm_cvtepi32_ps(x_c_l);                         \
	x_fd = _mm_cvtepi32_ps(x_d_l);                         \
	MLIB_S_SUM_AA##H(x_fa, x_fb, x_fc, x_fd, x_r2_l,       \
	    (SCL_##V + SCL_A##H));                             \
	MLIB_S_SUM_BB##H(x_fa, x_fb, x_fc, x_fd, x_r3_l,       \
	    (SCL_##V + SCL_B##H));                             \
	x_a_h = x_v1_h;                                        \
	x_b_h = _mm_or_si128(_mm_srli_si128(x_v1_h, 4),        \
	    _mm_slli_si128(x_v2_l, 12));                       \
	x_c_h = _mm_or_si128(_mm_srli_si128(x_v1_h, 8),        \
	    _mm_slli_si128(x_v2_l, 8));                        \
	x_d_h = _mm_or_si128(_mm_srli_si128(x_v1_h, 12),       \
	    _mm_slli_si128(x_v2_l, 4));                        \
	x_fa = _mm_cvtepi32_ps(x_a_h);                         \
	x_fb = _mm_cvtepi32_ps(x_b_h);                         \
	x_fc = _mm_cvtepi32_ps(x_c_h);                         \
	x_fd = _mm_cvtepi32_ps(x_d_h);                         \
	MLIB_S_SUM_AA##H(x_fa, x_fb, x_fc, x_fd, x_r2_h,       \
	    (SCL_##V + SCL_A##H));                             \
	MLIB_S_SUM_BB##H(x_fa, x_fb, x_fc, x_fd, x_r3_h,       \
	    (SCL_##V + SCL_B##H))

/* *********************************************************** */

#define	MLIB_S_SUM_LINE2(H, V)                                 \
	x_a_l = x_v0_l;                                        \
	x_b_l = _mm_or_si128(_mm_srli_si128(x_v0_l, 8),        \
	    _mm_slli_si128(x_v0_h, 8));                        \
	x_c_l = x_v0_h;                                        \
	x_d_l = _mm_or_si128(_mm_srli_si128(x_v0_h, 8),        \
	    _mm_slli_si128(x_v1_l, 8));                        \
	x_a_h = x_v0_h;                                        \
	x_b_h = _mm_or_si128(_mm_srli_si128(x_v0_h, 8),        \
	    _mm_slli_si128(x_v1_l, 8));                        \
	x_c_h = x_v1_l;                                        \
	x_d_h = _mm_or_si128(_mm_srli_si128(x_v1_l, 8),        \
	    _mm_slli_si128(x_v1_h, 8));                        \
	x_fa = _mm_cvtepi32_ps(x_a_l);                         \
	x_fb = _mm_cvtepi32_ps(x_b_l);                         \
	x_fc = _mm_cvtepi32_ps(x_c_l);                         \
	x_fd = _mm_cvtepi32_ps(x_d_l);                         \
	MLIB_S_SUM_AA##H(x_fa, x_fb, x_fc, x_fd, x_r0_l,       \
	    (SCL_##V + SCL_A##H));                             \
	MLIB_S_SUM_BB##H(x_fa, x_fb, x_fc, x_fd, x_r1_l,       \
	    (SCL_##V + SCL_B##H));                             \
	x_fa = _mm_cvtepi32_ps(x_a_h);                         \
	x_fb = _mm_cvtepi32_ps(x_b_h);                         \
	x_fc = _mm_cvtepi32_ps(x_c_h);                         \
	x_fd = _mm_cvtepi32_ps(x_d_h);                         \
	MLIB_S_SUM_AA##H(x_fa, x_fb, x_fc, x_fd, x_r0_h,       \
	    (SCL_##V + SCL_A##H));                             \
	MLIB_S_SUM_BB##H(x_fa, x_fb, x_fc, x_fd, x_r1_h,       \
	    (SCL_##V + SCL_B##H));                             \
	x_a_l = x_v1_l;                                        \
	x_b_l = _mm_or_si128(_mm_srli_si128(x_v1_l, 8),        \
	    _mm_slli_si128(x_v1_h, 8));                        \
	x_c_l = x_v1_h;                                        \
	x_d_l = _mm_or_si128(_mm_srli_si128(x_v1_h, 8),        \
	    _mm_slli_si128(x_v2_l, 8));                        \
	x_a_h = x_v1_h;                                        \
	x_b_h = _mm_or_si128(_mm_srli_si128(x_v1_h, 8),        \
	    _mm_slli_si128(x_v2_l, 8));                        \
	x_c_h = x_v2_l;                                        \
	x_d_h = _mm_or_si128(_mm_srli_si128(x_v2_l, 8),        \
	    _mm_slli_si128(x_v2_h, 8));                        \
	x_fa = _mm_cvtepi32_ps(x_a_l);                         \
	x_fb = _mm_cvtepi32_ps(x_b_l);                         \
	x_fc = _mm_cvtepi32_ps(x_c_l);                         \
	x_fd = _mm_cvtepi32_ps(x_d_l);                         \
	MLIB_S_SUM_AA##H(x_fa, x_fb, x_fc, x_fd, x_r2_l,       \
	    (SCL_##V + SCL_A##H));                             \
	MLIB_S_SUM_BB##H(x_fa, x_fb, x_fc, x_fd, x_r3_l,       \
	    (SCL_##V + SCL_B##H));                             \
	x_fa = _mm_cvtepi32_ps(x_a_h);                         \
	x_fb = _mm_cvtepi32_ps(x_b_h);                         \
	x_fc = _mm_cvtepi32_ps(x_c_h);                         \
	x_fd = _mm_cvtepi32_ps(x_d_h);                         \
	MLIB_S_SUM_AA##H(x_fa, x_fb, x_fc, x_fd, x_r2_h,       \
	    (SCL_##V + SCL_A##H));                             \
	MLIB_S_SUM_BB##H(x_fa, x_fb, x_fc, x_fd, x_r3_h,       \
	    (SCL_##V + SCL_B##H))

/* *********************************************************** */

#define	MLIB_S_SUM_LINE3(H, V)                                 \
	x_a_l = x_v0_l;                                        \
	x_b_l = _mm_or_si128(_mm_srli_si128(x_v0_l, 12),       \
	    _mm_slli_si128(x_v0_h, 4));                        \
	x_c_l = _mm_or_si128(_mm_srli_si128(x_v0_h, 8),        \
	    _mm_slli_si128(x_v1_l, 8));                        \
	x_d_l = _mm_or_si128(_mm_srli_si128(x_v1_l, 4),        \
	    _mm_slli_si128(x_v1_h, 12));                       \
	x_fa = _mm_cvtepi32_ps(x_a_l);                         \
	x_fb = _mm_cvtepi32_ps(x_b_l);                         \
	x_fc = _mm_cvtepi32_ps(x_c_l);                         \
	x_fd = _mm_cvtepi32_ps(x_d_l);                         \
	MLIB_S_SUM_AA##H(x_fa, x_fb, x_fc, x_fd, x_r0_l,       \
	    (SCL_##V + SCL_A##H));                             \
	MLIB_S_SUM_BB##H(x_fa, x_fb, x_fc, x_fd, x_r1_l,       \
	    (SCL_##V + SCL_B##H));                             \
	x_a_l = x_v0_h;                                        \
	x_b_l = _mm_or_si128(_mm_srli_si128(x_v0_h, 12),       \
	    _mm_slli_si128(x_v1_l, 4));                        \
	x_c_l = _mm_or_si128(_mm_srli_si128(x_v1_l, 8),        \
	    _mm_slli_si128(x_v1_h, 8));                        \
	x_d_l = _mm_or_si128(_mm_srli_si128(x_v1_h, 4),        \
	    _mm_slli_si128(x_v2_l, 12));                       \
	x_fa = _mm_cvtepi32_ps(x_a_l);                         \
	x_fb = _mm_cvtepi32_ps(x_b_l);                         \
	x_fc = _mm_cvtepi32_ps(x_c_l);                         \
	x_fd = _mm_cvtepi32_ps(x_d_l);                         \
	MLIB_S_SUM_AA##H(x_fa, x_fb, x_fc, x_fd, x_r0_h,       \
	    (SCL_##V + SCL_A##H));                             \
	MLIB_S_SUM_BB##H(x_fa, x_fb, x_fc, x_fd, x_r1_h,       \
	    (SCL_##V + SCL_B##H));                             \
	x_a_l = x_v1_l;                                        \
	x_b_l = _mm_or_si128(_mm_srli_si128(x_v1_l, 12),       \
	    _mm_slli_si128(x_v1_h, 4));                        \
	x_c_l = _mm_or_si128(_mm_srli_si128(x_v1_h, 8),        \
	    _mm_slli_si128(x_v2_l, 8));                        \
	x_d_l = _mm_or_si128(_mm_srli_si128(x_v2_l, 4),        \
	    _mm_slli_si128(x_v2_h, 12));                       \
	x_fa = _mm_cvtepi32_ps(x_a_l);                         \
	x_fb = _mm_cvtepi32_ps(x_b_l);                         \
	x_fc = _mm_cvtepi32_ps(x_c_l);                         \
	x_fd = _mm_cvtepi32_ps(x_d_l);                         \
	MLIB_S_SUM_AA##H(x_fa, x_fb, x_fc, x_fd, x_r2_l,       \
	    (SCL_##V + SCL_A##H));                             \
	MLIB_S_SUM_BB##H(x_fa, x_fb, x_fc, x_fd, x_r3_l,       \
	    (SCL_##V + SCL_B##H))

/* *********************************************************** */

#define	MLIB_S_SUM_LINE4(H, V)                                 \
	x_fa = _mm_cvtepi32_ps(x_v0_l);                        \
	x_fb = _mm_cvtepi32_ps(x_v0_h);                        \
	x_fc = _mm_cvtepi32_ps(x_v1_l);                        \
	x_fd = _mm_cvtepi32_ps(x_v1_h);                        \
	x_fna = _mm_cvtepi32_ps(x_v2_l);                       \
	x_fnb = _mm_cvtepi32_ps(x_v2_h);                       \
	x_fnc = _mm_cvtepi32_ps(x_v3_l);                       \
	x_fnd = _mm_cvtepi32_ps(x_v3_h);                       \
	MLIB_S_SUM_AA##H(x_fa, x_fb, x_fc, x_fd, x_r0_l,       \
	    (SCL_##V + SCL_A##H));                             \
	MLIB_S_SUM_BB##H(x_fa, x_fb, x_fc, x_fd, x_r1_l,       \
	    (SCL_##V + SCL_B##H));                             \
	MLIB_S_SUM_AA##H(x_fb, x_fc, x_fd, x_fna, x_r0_h,      \
	    (SCL_##V + SCL_A##H));                             \
	MLIB_S_SUM_BB##H(x_fb, x_fc, x_fd, x_fna, x_r1_h,      \
	    (SCL_##V + SCL_B##H));                             \
	MLIB_S_SUM_AA##H(x_fc, x_fd, x_fna, x_fnb, x_r2_l,     \
	    (SCL_##V + SCL_A##H));                             \
	MLIB_S_SUM_BB##H(x_fc, x_fd, x_fna, x_fnb, x_r3_l,     \
	    (SCL_##V + SCL_B##H));                             \
	MLIB_S_SUM_AA##H(x_fd, x_fna, x_fnb, x_fnc, x_r2_h,    \
	    (SCL_##V + SCL_A##H));                             \
	MLIB_S_SUM_BB##H(x_fd, x_fna, x_fnb, x_fnc, x_r3_h,    \
	    (SCL_##V + SCL_B##H));                             \

/* *********************************************************** */

#define	MLIB_S_SAT_U16(al, ah, bl, bh) {                     \
	x_a_l = _mm_cmpgt_epi32(al, x_zero);                 \
	al = _mm_and_si128(al, x_a_l);                       \
	x_b_l = _mm_cmpgt_epi32(ah, x_zero);                 \
	ah = _mm_and_si128(ah, x_b_l);                       \
	x_a_l = _mm_cmpgt_epi32(bl, x_zero);                 \
	bl = _mm_and_si128(bl, x_a_l);                       \
	x_b_l = _mm_cmpgt_epi32(bh, x_zero);                 \
	bh = _mm_and_si128(bh, x_b_l);                       \
	x_a_l = _mm_cmplt_epi32(al, x_max_u16);              \
	al = _mm_and_si128(al, x_a_l);                       \
	al = _mm_or_si128(al,                                \
	    _mm_andnot_si128(x_a_l, x_max_u16));             \
	x_b_l = _mm_cmplt_epi32(ah, x_max_u16);              \
	ah = _mm_and_si128(ah, x_b_l);                       \
	ah = _mm_or_si128(ah,                                \
	    _mm_andnot_si128(x_b_l, x_max_u16));             \
	x_a_l = _mm_cmplt_epi32(bl, x_max_u16);              \
	bl = _mm_and_si128(bl, x_a_l);                       \
	bl = _mm_or_si128(bl,                                \
	    _mm_andnot_si128(x_a_l, x_max_u16));             \
	x_b_l = _mm_cmplt_epi32(bh, x_max_u16);              \
	bh = _mm_and_si128(bh, x_b_l);                       \
	bh = _mm_or_si128(bh,                                \
	    _mm_andnot_si128(x_b_l, x_max_u16));             \
	al = _mm_srli_epi32(al, 2);                          \
	ah = _mm_srli_epi32(ah, 2);                          \
	bl = _mm_srli_epi32(bl, 2);                          \
	bh = _mm_srli_epi32(bh, 2);                          \
}

/* *********************************************************** */

#define	MLIB_S_PACK1_U8                                      \
	x_v0 = _mm_unpacklo_epi8(x_r0, x_r1);                \
	x_v1 = _mm_unpackhi_epi8(x_r0, x_r1);                \

/* *********************************************************** */

#define	MLIB_S_PACK2_U8                                      \
	x_v0 = _mm_unpacklo_epi16(x_r0, x_r1);               \
	x_v1 = _mm_unpackhi_epi16(x_r0, x_r1);               \

/* *********************************************************** */

#define	MLIB_S_PACK4_U8                                      \
	x_v0 = _mm_unpacklo_epi32(x_r0, x_r1);               \
	x_v1 = _mm_unpackhi_epi32(x_r0, x_r1);               \

/* *********************************************************** */

#define	MLIB_S_PACK1_U16                                     \
	MLIB_S_SAT_U16(x_r0_l, x_r0_h, x_r1_l, x_r1_h);      \
	MLIB_S_SAT_U16(x_r2_l, x_r2_h, x_r3_l, x_r3_h);      \
	x_r0 = _mm_packs_epi32(x_r0_l, x_r0_h);              \
	x_r1 = _mm_packs_epi32(x_r1_l, x_r1_h);              \
	x_r2 = _mm_packs_epi32(x_r2_l, x_r2_h);              \
	x_r3 = _mm_packs_epi32(x_r3_l, x_r3_h);              \
	x_a_l = _mm_unpacklo_epi16(x_r0, x_r1);              \
	x_a_h = _mm_unpackhi_epi16(x_r0, x_r1);              \
	x_b_l = _mm_unpacklo_epi16(x_r2, x_r3);              \
	x_b_h = _mm_unpackhi_epi16(x_r2, x_r3);              \
	x_v0 = _mm_slli_epi16(x_a_l, 2);                     \
	x_v1 = _mm_slli_epi16(x_a_h, 2);                     \
	x_v2 = _mm_slli_epi16(x_b_l, 2);                     \
	x_v3 = _mm_slli_epi16(x_b_h, 2);                     \

/* *********************************************************** */

#define	MLIB_S_PACK2_U16                                     \
	MLIB_S_SAT_U16(x_r0_l, x_r0_h, x_r1_l, x_r1_h);      \
	MLIB_S_SAT_U16(x_r2_l, x_r2_h, x_r3_l, x_r3_h);      \
	x_r0 = _mm_packs_epi32(x_r0_l, x_r0_h);              \
	x_r1 = _mm_packs_epi32(x_r1_l, x_r1_h);              \
	x_r2 = _mm_packs_epi32(x_r2_l, x_r2_h);              \
	x_r3 = _mm_packs_epi32(x_r3_l, x_r3_h);              \
	x_a_l = _mm_unpacklo_epi32(x_r0, x_r1);              \
	x_a_h = _mm_unpackhi_epi32(x_r0, x_r1);              \
	x_b_l = _mm_unpacklo_epi32(x_r2, x_r3);              \
	x_b_h = _mm_unpackhi_epi32(x_r2, x_r3);              \
	x_v0 = _mm_slli_epi16(x_a_l, 2);                     \
	x_v1 = _mm_slli_epi16(x_a_h, 2);                     \
	x_v2 = _mm_slli_epi16(x_b_l, 2);                     \
	x_v3 = _mm_slli_epi16(x_b_h, 2);                     \

/* *********************************************************** */

#define	MLIB_S_PACK4_U16                                     \
	MLIB_S_SAT_U16(x_r0_l, x_r0_h, x_r1_l, x_r1_h);      \
	MLIB_S_SAT_U16(x_r2_l, x_r2_h, x_r3_l, x_r3_h);      \
	x_r0 = _mm_packs_epi32(x_r0_l, x_r0_h);              \
	x_r1 = _mm_packs_epi32(x_r1_l, x_r1_h);              \
	x_r2 = _mm_packs_epi32(x_r2_l, x_r2_h);              \
	x_r3 = _mm_packs_epi32(x_r3_l, x_r3_h);              \
	x_a_l = _mm_unpacklo_epi64(x_r0, x_r1);              \
	x_a_h = _mm_unpackhi_epi64(x_r0, x_r1);              \
	x_b_l = _mm_unpacklo_epi64(x_r2, x_r3);              \
	x_b_h = _mm_unpackhi_epi64(x_r2, x_r3);              \
	x_v0 = _mm_slli_epi16(x_a_l, 2);                     \
	x_v1 = _mm_slli_epi16(x_a_h, 2);                     \
	x_v2 = _mm_slli_epi16(x_b_l, 2);                     \
	x_v3 = _mm_slli_epi16(x_b_h, 2);                     \

/* *********************************************************** */

#define	MLIB_S_PACK1_S16                                     \
	x_r0 = _mm_packs_epi32(x_r0_l, x_r0_h);              \
	x_r1 = _mm_packs_epi32(x_r1_l, x_r1_h);              \
	x_r2 = _mm_packs_epi32(x_r2_l, x_r2_h);              \
	x_r3 = _mm_packs_epi32(x_r3_l, x_r3_h);              \
	x_v0 = _mm_unpacklo_epi16(x_r0, x_r1);               \
	x_v1 = _mm_unpackhi_epi16(x_r0, x_r1);               \
	x_v2 = _mm_unpacklo_epi16(x_r2, x_r3);               \
	x_v3 = _mm_unpackhi_epi16(x_r2, x_r3);               \

/* *********************************************************** */

#define	MLIB_S_PACK2_S16                                     \
	x_r0 = _mm_packs_epi32(x_r0_l, x_r0_h);              \
	x_r1 = _mm_packs_epi32(x_r1_l, x_r1_h);              \
	x_r2 = _mm_packs_epi32(x_r2_l, x_r2_h);              \
	x_r3 = _mm_packs_epi32(x_r3_l, x_r3_h);              \
	x_v0 = _mm_unpacklo_epi32(x_r0, x_r1);               \
	x_v1 = _mm_unpackhi_epi32(x_r0, x_r1);               \
	x_v2 = _mm_unpacklo_epi32(x_r2, x_r3);               \
	x_v3 = _mm_unpackhi_epi32(x_r2, x_r3);               \

/* *********************************************************** */

#define	MLIB_S_PACK4_S16                                     \
	x_r0 = _mm_packs_epi32(x_r0_l, x_r0_h);              \
	x_r1 = _mm_packs_epi32(x_r1_l, x_r1_h);              \
	x_r2 = _mm_packs_epi32(x_r2_l, x_r2_h);              \
	x_r3 = _mm_packs_epi32(x_r3_l, x_r3_h);              \
	x_v0 = _mm_unpacklo_epi64(x_r0, x_r1);               \
	x_v1 = _mm_unpackhi_epi64(x_r0, x_r1);               \
	x_v2 = _mm_unpacklo_epi64(x_r2, x_r3);               \
	x_v3 = _mm_unpackhi_epi64(x_r2, x_r3);               \

/* *********************************************************** */

#define	MLIB_S_PACK3_U16                                     \
	MLIB_S_SAT_U16(x_r0_l, x_r0_h, x_r1_l, x_r1_h);      \
	x_a_l = _mm_cmpgt_epi32(x_r2_l, x_zero);             \
	x_r2_l = _mm_and_si128(x_r2_l, x_a_l);               \
	x_a_l = _mm_cmpgt_epi32(x_r3_l, x_zero);             \
	x_r3_l = _mm_and_si128(x_r3_l, x_a_l);               \
	x_a_l = _mm_cmplt_epi32(x_r2_l, x_max_u16);          \
	x_r2_l = _mm_and_si128(x_r2_l, x_a_l);               \
	x_r2_l = _mm_or_si128(x_r2_l,                        \
	    _mm_andnot_si128(x_a_l, x_max_u16));             \
	x_a_l = _mm_cmplt_epi32(x_r3_l, x_max_u16);          \
	x_r3_l = _mm_and_si128(x_r3_l, x_a_l);               \
	x_r3_l = _mm_or_si128(x_r3_l,                        \
	    _mm_andnot_si128(x_a_l, x_max_u16));             \
                                                             \
	x_r2_l = _mm_srli_epi32(x_r2_l, 2);                  \
	x_r3_l = _mm_srli_epi32(x_r3_l, 2);                  \
	x_r0 = _mm_packs_epi32(x_r0_l, x_r0_h);              \
	x_r1 = _mm_packs_epi32(x_r1_l, x_r1_h);              \
	x_r2 = _mm_packs_epi32(x_r2_l, x_r2_l);              \
	x_r3 = _mm_packs_epi32(x_r3_l, x_r3_l);              \
	x_r0 = _mm_slli_epi16(x_r0, 2);                      \
	x_r1 = _mm_slli_epi16(x_r1, 2);                      \
	x_r2 = _mm_slli_epi16(x_r2, 2);                      \
	x_r3 = _mm_slli_epi16(x_r3, 2);                      \

/* *********************************************************** */

#define	MLIB_S_PACK3_S16                                     \
	x_r0 = _mm_packs_epi32(x_r0_l, x_r0_h);              \
	x_r1 = _mm_packs_epi32(x_r1_l, x_r1_h);              \
	x_r2 = _mm_packs_epi32(x_r2_l, x_r2_l);              \
	x_r3 = _mm_packs_epi32(x_r3_l, x_r3_l);              \

/* *********************************************************** */

#define	MLIB_S_ZOOM_BC2_LINE_UNPACK                          \
	x_sd0_l = _mm_unpacklo_epi8(x_sd0, x_zero);          \
	x_sd0_h = _mm_unpackhi_epi8(x_sd0, x_zero);          \
	x_sd1_l = _mm_unpacklo_epi8(x_sd1, x_zero);          \
	x_sd1_h = _mm_unpackhi_epi8(x_sd1, x_zero);          \
	x_sd2_l = _mm_unpacklo_epi8(x_sd2, x_zero);          \
	x_sd2_h = _mm_unpackhi_epi8(x_sd2, x_zero);          \
	x_sd3_l = _mm_unpacklo_epi8(x_sd3, x_zero);          \
	x_sd3_h = _mm_unpackhi_epi8(x_sd3, x_zero);          \

/* *********************************************************** */

#define	MLIB_S_ZOOM_BC2_LINE_HEAD(H, V, STORE_SAT)                     \
	if (src_w_beg) {                                               \
	for (k = 0; k < channels; k++) {                               \
	    ps = sl + k;                                               \
	    pd = dl + k;                                               \
	    v0 = SUM_##V(ps[-src_stride], ps[0], ps[src_stride],       \
			ps[2 * src_stride]);                           \
	    v1 = SUM_##V(ps[channels - src_stride], ps[channels],      \
		    ps[channels + src_stride],                         \
		    ps[channels + 2 * src_stride]);                    \
	    v2 = SUM_##V(ps[2 * channels - src_stride],                \
		    ps[2 * channels], ps[2 * channels + src_stride],   \
		    ps[2 * channels + 2 * src_stride]);                \
		v3 = SUM_##V(ps[3 * channels - src_stride],            \
		    ps[3 * channels], ps[3 * channels + src_stride],   \
		    ps[3 * channels + 2 * src_stride]);                \
		r1 = SUM_B##H(v0, v1, v2,                              \
		    v3) >> (SCL_##V + SCL_B##H);                       \
		STORE_SAT(pd[0], r1);                                  \
	}                                                              \
	ps = sl + channels;                                            \
	pd = dl + channels;                                            \
	}                                                              \

/* *********************************************************** */

#define	MLIB_S_ZOOM_BC2_LINE_TAIL(H, V, STORE_SAT)             \
for (k = 0; k < channels; k++) {                               \
	pd = dl + i * 2 + k;                                   \
	ps = sl + i + k + 3 * channels;                        \
	if (src_w_beg) {                                       \
		pd += channels;                                \
		ps += channels;                                \
	}                                                      \
	v0 = v[k];                                             \
	v1 = v[k + channels];                                  \
	v2 = v[k + 2 * channels];                              \
	for (int m = i / channels; m < (src_width / 2); m++) { \
		v3 = SUM_##V(ps[-src_stride], ps[0],           \
		    ps[src_stride], ps[2 * src_stride]);       \
		r0 = SUM_A##H(v0, v1, v2,                      \
		    v3) >> (SCL_##V + SCL_A##H);               \
		r1 = SUM_B##H(v0, v1, v2,                      \
		    v3) >> (SCL_##V + SCL_B##H);               \
		STORE_SAT(pd[0], r0);                          \
		STORE_SAT(pd[channels], r1);                   \
		ps += channels;                                \
		pd += 2 * channels;                            \
		v0 = v1;                                       \
		v1 = v2;                                       \
		v2 = v3;                                       \
	}                                                      \
	if (src_width & 1) {                                   \
		v3 = SUM_##V(ps[-src_stride], ps[0],           \
		    ps[src_stride], ps[2 * src_stride]);       \
		r0 = SUM_A##H(v0, v1, v2,                      \
		    v3) >> (SCL_##V + SCL_A##H);               \
		STORE_SAT(pd[0], r0);                          \
	}                                                      \
}                                                              \

/* *********************************************************** */

#define	MLIB_S_ZOOM_BC2_LINE_U8_124(CH, TT, H, V, STORE_SAT) { \
	TTYPE v[16];                                           \
	TTYPE v0, v1, v2, v3, r0, r1;                          \
	ps = sl;                                               \
	pd = dl;                                               \
	MLIB_S_ZOOM_BC2_LINE_HEAD(H, V, STORE_SAT);            \
	__m128i x_sd0, x_sd1, x_sd2, x_sd3;                    \
	__m128i x_sd0_l, x_sd1_l, x_sd2_l, x_sd3_l;            \
	__m128i x_sd0_h, x_sd1_h, x_sd2_h, x_sd3_h;            \
	__m128i x_r0, x_r1, x_r2, x_r3, x_v0, x_v1;            \
	__m128i x_v0_l, x_v0_h, x_v1_l, x_v1_h;                \
	__m128i x_v2_l, x_v2_h, x_v3_l, x_v3_h;                \
	__m128i x_r0_l, x_r0_h, x_r1_l, x_r1_h;                \
	__m128i x_r2_l, x_r2_h, x_r3_l, x_r3_h;                \
	MLIB_S_MULCONST;                                       \
	MLIB_S_ZOOM_BC2_LINE_LOAD(x_sd0, x_sd1, x_sd2, x_sd3); \
	ps += 8;                                               \
	MLIB_S_ZOOM_BC2_LINE_UNPACK;                           \
	MLIB_S_SUM_##V##_U16(x_sd0_l, x_sd1_l, x_sd2_l,        \
	    x_sd3_l, x_v0_l, x_v0_h);                          \
	MLIB_S_SUM_##V##_U16(x_sd0_h, x_sd1_h, x_sd2_h,        \
	    x_sd3_h, x_v1_l, x_v1_h);                          \
	for (i = 0; i < (src_width / 2 - 15); i += 16) {       \
	MLIB_S_ZOOM_BC2_LINE_LOAD(x_sd0, x_sd1, x_sd2, x_sd3); \
	ps += 8;                                               \
	MLIB_S_ZOOM_BC2_LINE_UNPACK;                           \
	MLIB_S_SUM_##V##_U16(x_sd0_l, x_sd1_l, x_sd2_l,        \
	    x_sd3_l, x_v2_l, x_v2_h);                          \
	MLIB_S_SUM_##V##_U16(x_sd0_h, x_sd1_h, x_sd2_h,        \
	    x_sd3_h, x_v3_l, x_v3_h);                          \
			                                       \
	MLIB_S_SUM_LINE##CH##_U8(H, V);                        \
	MLIB_S_PACK##CH##_U8;                                  \
	_mm_storeu_si128((__m128i *)pd, x_v0);                 \
	_mm_storeu_si128((__m128i *)(pd + 16), x_v1);          \
	pd += 32;                                              \
	x_v0_l = x_v2_l;                                       \
	x_v0_h = x_v2_h;                                       \
	x_v1_l = x_v3_l;                                       \
	x_v1_h = x_v3_h;                                       \
	}                                                      \
	_mm_storeu_si128((__m128i *)v, x_v0_l);                \
	_mm_storeu_si128((__m128i *)(v + 4), x_v0_h);          \
	_mm_storeu_si128((__m128i *)(v + 8), x_v1_l);          \
	_mm_storeu_si128((__m128i *)(v + 12), x_v1_h);         \
	MLIB_S_ZOOM_BC2_LINE_TAIL(H, V, STORE_SAT);            \
	dl += dst_stride;                                      \
}

/* *********************************************************** */

#define	MLIB_S_MULCONST_U8_3                                       \
	const __m128i x_mask3 = _mm_set_epi8(                      \
	    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,                          \
	    0, 0, 0, 0xFFFF, 0xFFFF, 0xFFFF);                      \
	const __m128i x_mask2 = _mm_set_epi8(                      \
	    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,                          \
	    0, 0, 0, 0, 0xFFFF, 0xFFFF);                           \
	const __m128i x_mask1 = _mm_set_epi8(                      \
	    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0xFFFF);  \
	__m128i x_r00, x_r01, x_r02, x_r03, x_r04, x_r05;          \
	__m128i x_r10, x_r11, x_r12, x_r13, x_r14, x_r15, x_r16;   \
	__m128i x_r20, x_r21, x_r22, x_r23, x_r24, x_r25, x_r26;   \
	__m128i x_mask3l3 = _mm_slli_si128(x_mask3, 3);            \
	__m128i x_mask3l6 = _mm_slli_si128(x_mask3, 6);            \
	__m128i x_mask3l9 = _mm_slli_si128(x_mask3, 9);            \
	__m128i x_mask3l12 = _mm_slli_si128(x_mask3, 12);          \
	__m128i x_mask1l15 = _mm_slli_si128(x_mask1, 15);          \
	__m128i x_mask3l2 = _mm_slli_si128(x_mask3, 2);            \
	__m128i x_mask3l5 = _mm_slli_si128(x_mask3, 5);            \
	__m128i x_mask3l8 = _mm_slli_si128(x_mask3, 8);            \
	__m128i x_mask3l11 = _mm_slli_si128(x_mask3, 11);          \
	__m128i x_mask1l14 = _mm_slli_si128(x_mask1, 14);          \
	__m128i x_mask1l1 = _mm_slli_si128(x_mask1, 1);            \
	__m128i x_mask2l2 = _mm_slli_si128(x_mask2, 2);            \
	__m128i x_mask3l4 = _mm_slli_si128(x_mask3, 4);            \
	__m128i x_mask3l7 = _mm_slli_si128(x_mask3, 7);            \
	__m128i x_mask3l10 = _mm_slli_si128(x_mask3, 10);          \
	__m128i x_mask3l13 = _mm_slli_si128(x_mask3, 13);          \

/* *********************************************************** */

#define	MLIB_S_ZOOM_BC2_LINE_U8_3(TT, H, V, STORE_SAT) {           \
	TTYPE v[12], v0, v1, v2, v3;                               \
	TTYPE r0, r1, r2, r3;                                      \
	ps = sl;                                                   \
	pd = dl;                                                   \
	MLIB_S_ZOOM_BC2_LINE_HEAD(H, V, STORE_SAT);                \
	__m128i x_sd0, x_sd1, x_sd2, x_sd3;                        \
	__m128i x_sd0_l, x_sd1_l, x_sd2_l, x_sd3_l;                \
	__m128i x_sd0_h, x_sd1_h, x_sd2_h, x_sd3_h;                \
	__m128i x_r0, x_r1, x_r2, x_r3, x_r4, x_r5;                \
	__m128i x_v0, x_v1, x_v2;                                  \
	__m128i x_v0_l, x_v0_h;                                    \
	__m128i x_v1_l, x_v1_h;                                    \
	__m128i x_v2_l, x_v2_h;                                    \
	__m128i x_v3_l, x_v3_h;                                    \
	__m128i x_v4_l, x_v4_h;                                    \
	__m128i x_r0_l, x_r0_h, x_r1_l, x_r1_h, x_r2_l, x_r2_h;    \
	__m128i x_r3_l, x_r3_h, x_r4_l, x_r4_h, x_r5_l, x_r5_h;    \
	MLIB_S_MULCONST;                                           \
	MLIB_S_ZOOM_BC2_LINE_LOAD(x_sd0, x_sd1, x_sd2, x_sd3);     \
	MLIB_S_ZOOM_BC2_LINE_UNPACK;                               \
	MLIB_S_SUM_##V##_U16(                                      \
	    x_sd0_l, x_sd1_l, x_sd2_l, x_sd3_l, x_v0_l, x_v0_h);   \
	MLIB_S_SUM_##V##_U16(                                      \
	    x_sd0_h, x_sd1_h, x_sd2_h, x_sd3_h, x_v1_l, x_v1_h);   \
	ps += 8;                                                   \
	MLIB_S_MULCONST_U8_3;                                      \
	for (i = 0; i < ((src_width * 3) / 2 - 23); i += 24) {     \
	MLIB_S_ZOOM_BC2_LINE_LOAD(x_sd0, x_sd1, x_sd2, x_sd3);     \
	MLIB_S_ZOOM_BC2_LINE_UNPACK;                               \
	MLIB_S_SUM_##V##_U16(                                      \
	    x_sd0_l, x_sd1_l, x_sd2_l, x_sd3_l, x_v2_l, x_v2_h);   \
	MLIB_S_SUM_##V##_U16(                                      \
	    x_sd0_h, x_sd1_h, x_sd2_h, x_sd3_h, x_v3_l, x_v3_h);   \
	ps += 8;                                                   \
	MLIB_S_ZOOM_BC2_LINE_LOAD(x_sd0, x_sd1, x_sd2, x_sd3);     \
	MLIB_S_ZOOM_BC2_LINE_UNPACK;                               \
	MLIB_S_SUM_##V##_U16(                                      \
	    x_sd0_l, x_sd1_l, x_sd2_l, x_sd3_l, x_v4_l, x_v4_h);   \
	MLIB_S_SUM_LINE3_U8(H, V);                                 \
	x_r00 = _mm_and_si128(x_r0, x_mask3);                      \
	x_r01 = _mm_and_si128(_mm_slli_si128(x_r1, 3), x_mask3l3); \
	x_r02 = _mm_and_si128(_mm_slli_si128(x_r0, 3), x_mask3l6); \
	x_r03 = _mm_and_si128(_mm_slli_si128(x_r1, 6), x_mask3l9); \
	x_r04 = _mm_and_si128(_mm_slli_si128(x_r0, 6), x_mask3l12);\
	x_r05 = _mm_and_si128(_mm_slli_si128(x_r1, 9), x_mask1l15);\
	x_r00 = _mm_or_si128(x_r00, x_r01);                        \
	x_r02 = _mm_or_si128(x_r02, x_r03);                        \
	x_r04 = _mm_or_si128(x_r04, x_r05);                        \
	x_v0 = _mm_or_si128(x_r00, _mm_or_si128(x_r02, x_r04));    \
			                                           \
	x_r10 = _mm_and_si128(x_mask2, _mm_srli_si128(x_r1, 7));   \
	x_r11 = _mm_and_si128(_mm_srli_si128(x_r0, 7), x_mask3l2); \
	x_r12 = _mm_and_si128(_mm_srli_si128(x_r1, 4), x_mask3l5); \
	x_r13 = _mm_and_si128(_mm_srli_si128(x_r0, 4), x_mask3l8); \
	x_r14 = _mm_and_si128(_mm_srli_si128(x_r1, 1), x_mask3l11);\
	x_r15 = _mm_and_si128(_mm_srli_si128(x_r0, 1), x_mask1l14);\
	x_r16 = _mm_slli_si128(x_r4, 15);                          \
	x_r10 = _mm_or_si128(x_r10, x_r11);                        \
	x_r12 = _mm_or_si128(x_r12, x_r13);                        \
	x_r14 = _mm_or_si128(x_r14, x_r15);                        \
	x_v1 = _mm_or_si128(_mm_or_si128(x_r10, x_r16),            \
	    _mm_or_si128(x_r14, x_r12));                           \
			                                           \
	x_r20 = _mm_and_si128(_mm_srli_si128(x_r4, 1), x_mask1);   \
	x_r21 = _mm_and_si128(_mm_srli_si128(x_r1, 14), x_mask1l1);\
	x_r22 = _mm_and_si128(_mm_slli_si128(x_r5, 2), x_mask2l2); \
	x_r23 = _mm_and_si128(_mm_slli_si128(x_r4, 2), x_mask3l4); \
	x_r24 = _mm_and_si128(_mm_slli_si128(x_r5, 5), x_mask3l7); \
	x_r25 = _mm_and_si128(_mm_slli_si128(x_r4, 5), x_mask3l10);\
	x_r26 = _mm_and_si128(_mm_slli_si128(x_r5, 8), x_mask3l13);\
	x_r20 = _mm_or_si128(x_r20, x_r21);                        \
	x_r22 = _mm_or_si128(x_r22, x_r23);                        \
	x_r24 = _mm_or_si128(x_r24, x_r25);                        \
	x_v2 = _mm_or_si128(_mm_or_si128(x_r20, x_r26),            \
	    _mm_or_si128(x_r22, x_r24));                           \
	_mm_storeu_si128((__m128i *)pd, x_v0);                     \
	_mm_storeu_si128((__m128i *)(pd + 16), x_v1);              \
	_mm_storeu_si128((__m128i *)(pd + 32), x_v2);              \
	pd += 48;                                                  \
	x_v0_l = x_v3_l;                                           \
	x_v0_h = x_v3_h;                                           \
	x_v1_l = x_v4_l;                                           \
	x_v1_h = x_v4_h;                                           \
	}                                                          \
	_mm_storeu_si128((__m128i *)v, x_v0_l);                    \
	_mm_storeu_si128((__m128i *)(v + 4), x_v0_h);              \
	_mm_storeu_si128((__m128i *)(v + 8), x_v1_l);              \
	MLIB_S_ZOOM_BC2_LINE_TAIL(H, V, STORE_SAT);                \
	dl += dst_stride;                                          \
}

/* *********************************************************** */

#define	MLIB_S_MULCONST_3                                       \
	const __m128i x_mask3 = _mm_set_epi16(                  \
	    0, 0, 0, 0, 0, 0xFFFF, 0xFFFF, 0xFFFF);             \
	const __m128i x_mask2 = _mm_set_epi16(                  \
	    0, 0, 0, 0, 0, 0, 0xFFFF, 0xFFFF);                  \
	const __m128i x_mask1 = _mm_set_epi16(                  \
	    0, 0, 0, 0, 0, 0, 0, 0xFFFF);                       \
	__m128i x_r00, x_r01, x_r02;                            \
	__m128i x_r10, x_r11, x_r12, x_r13, x_r14;              \
	__m128i x_r20, x_r21, x_r22, x_r23;                     \
	__m128i x_mask36 = _mm_slli_si128(x_mask3, 6);          \
	__m128i x_mask212 = _mm_slli_si128(x_mask2, 12);        \
	__m128i x_mask32 = _mm_slli_si128(x_mask3, 2);          \
	__m128i x_mask28 = _mm_slli_si128(x_mask2, 8);          \
	__m128i x_mask112 = _mm_slli_si128(x_mask1, 12);        \
	__m128i x_mask114 = _mm_slli_si128(x_mask1, 14);        \
	__m128i x_mask12 = _mm_slli_si128(x_mask1, 2);          \
	__m128i x_mask34 = _mm_slli_si128(x_mask3, 4);          \
	__m128i x_mask310 = _mm_slli_si128(x_mask3, 10);        \

/* *********************************************************** */

#define	MLIB_S_ZOOM_BC2_LINE_3(TT, H, V, STORE_SAT) {           \
	TTYPE v[12], v0, v1, v2, v3;                            \
	TTYPE r0, r1, r2, r3;                                   \
	ps = sl;                                                \
	pd = dl;                                                \
	MLIB_S_ZOOM_BC2_LINE_HEAD(H, V, STORE_SAT);             \
	__m128i x_sd0, x_sd1, x_sd2, x_sd3;                     \
	__m128i x_r0, x_r1, x_r2, x_r3;                         \
	__m128i x_v0, x_v1, x_v2;                               \
	__m128i x_v0_l, x_v0_h;                                 \
	__m128i x_v1_l, x_v1_h;                                 \
	__m128i x_v2_l, x_v2_h;                                 \
	__m128i x_v3_l, x_v3_h;                                 \
	__m128i x_r0_l, x_r0_h, x_r1_l, x_r1_h;                 \
	__m128i x_r2_l, x_r2_h, x_r3_l, x_r3_h;                 \
	MLIB_S_MULCONST;                                        \
	MLIB_S_MULCONST_3;                                      \
	MLIB_S_ZOOM_BC2_LINE_LOAD(x_sd0, x_sd1, x_sd2, x_sd3);  \
	MLIB_S_SUM_##V##_##TT(                                  \
	    x_sd0, x_sd1, x_sd2, x_sd3, x_v0_l, x_v0_h);        \
	MLIB_S_ZOOM_BC2_LINE_LOAD(x_sd0, x_sd1, x_sd2, x_sd3);  \
	MLIB_S_SUM_L_##V##_##TT(                                \
	    x_sd0, x_sd1, x_sd2, x_sd3, x_v1_l, x_v1_h);        \
	ps -= 4;                                                \
	for (i = 0; i < ((src_width * 3) / 2 - 11); i += 12) {  \
	MLIB_S_ZOOM_BC2_LINE_LOAD(x_sd0, x_sd1, x_sd2, x_sd3);  \
	MLIB_S_SUM_##V##_##TT(                                  \
	    x_sd0, x_sd1, x_sd2, x_sd3, x_v1_h, x_v2_l);        \
	MLIB_S_ZOOM_BC2_LINE_LOAD(x_sd0, x_sd1, x_sd2, x_sd3);  \
	ps -= 4;                                                \
	MLIB_S_SUM_L_##V##_##TT(                                \
	    x_sd0, x_sd1, x_sd2, x_sd3, x_v2_h, x_v3_l);        \
	MLIB_S_SUM_LINE3(H, V);                                 \
	MLIB_S_PACK3_##TT;                                      \
	x_r00 = _mm_and_si128(x_r0, x_mask3);                   \
	x_r01 = _mm_and_si128(                                  \
	    _mm_slli_si128(x_r1, 6), x_mask36);                 \
	x_r02 = _mm_and_si128(                                  \
	    _mm_slli_si128(x_r0, 6), x_mask212);                \
	x_v0 = _mm_or_si128(x_r00, _mm_or_si128(x_r01, x_r02)); \
	x_r10 = _mm_and_si128(                                  \
	    x_mask1, _mm_srli_si128(x_r0, 10));                 \
	x_r11 = _mm_and_si128(                                  \
	    _mm_srli_si128(x_r1, 4), x_mask32);                 \
	x_r12 = _mm_and_si128(                                  \
	    _mm_srli_si128(x_r0, 4), x_mask28);                 \
	x_r13 = _mm_and_si128(                                  \
	    _mm_slli_si128(x_r2, 12), x_mask112);               \
	x_r14 = _mm_and_si128(                                  \
	    _mm_slli_si128(x_r1, 2), x_mask114);                \
	x_r10 = _mm_or_si128(x_r10, x_r11);                     \
	x_r12 = _mm_or_si128(x_r12, x_r13);                     \
	x_r14 = _mm_or_si128(x_r14, x_r10);                     \
	x_v1 = _mm_or_si128(x_r14, x_r12);                      \
	x_r20 = _mm_and_si128(                                  \
	    _mm_srli_si128(x_r1, 14), x_mask1);                 \
	x_r21 = _mm_and_si128(                                  \
	    _mm_slli_si128(x_r3, 2), x_mask12);                 \
	x_r22 = _mm_and_si128(                                  \
	    _mm_slli_si128(x_r2, 2), x_mask34);                 \
	x_r23 = _mm_and_si128(                                  \
	    _mm_slli_si128(x_r3, 8), x_mask310);                \
	x_r20 = _mm_or_si128(x_r20, x_r21);                     \
	x_r22 = _mm_or_si128(x_r22, x_r23);                     \
	x_v2 = _mm_or_si128(x_r20, x_r22);                      \
	_mm_storeu_si128((__m128i *)pd, x_v0);                  \
	_mm_storeu_si128((__m128i *)(pd + 8), x_v1);            \
	_mm_storeu_si128((__m128i *)(pd + 16), x_v2);           \
	pd += 24;                                               \
	x_v0_l = x_v1_h;                                        \
	x_v0_h = x_v2_l;                                        \
	x_v1_l = x_v2_h;                                        \
	}                                                       \
	_mm_storeu_si128((__m128i *)v, x_v0_l);                 \
	_mm_storeu_si128((__m128i *)(v + 4), x_v0_h);           \
	_mm_storeu_si128((__m128i *)(v + 8), x_v1_l);           \
	MLIB_S_ZOOM_BC2_LINE_TAIL(H, V, STORE_SAT);             \
	dl += dst_stride;                                       \
}

/* *********************************************************** */

#define	MLIB_S_ZOOM_BC2_LINE_124(CH, TT, H, V, STORE_SAT) {      \
	TTYPE v[16], v0, v1, v2, v3;                             \
	TTYPE r0, r1;                                            \
	ps = sl;                                                 \
	pd = dl;                                                 \
	MLIB_S_ZOOM_BC2_LINE_HEAD(H, V, STORE_SAT);              \
	__m128i x_sd0, x_sd1, x_sd2, x_sd3;                      \
	__m128i x_r0, x_r1, x_r2, x_r3;                          \
	__m128i x_v0, x_v1, x_v2, x_v3;                          \
	__m128i x_v0_l, x_v0_h;                                  \
	__m128i x_v1_l, x_v1_h;                                  \
	__m128i x_v2_l, x_v2_h;                                  \
	__m128i x_v3_l, x_v3_h;                                  \
	__m128i x_r0_l, x_r0_h, x_r1_l, x_r1_h;                  \
	__m128i x_r2_l, x_r2_h, x_r3_l, x_r3_h;                  \
	MLIB_S_MULCONST;                                         \
	MLIB_S_ZOOM_BC2_LINE_LOAD(x_sd0, x_sd1, x_sd2, x_sd3);   \
	MLIB_S_SUM_##V##_##TT(                                   \
	    x_sd0, x_sd1, x_sd2, x_sd3, x_v0_l, x_v0_h);         \
	MLIB_S_ZOOM_BC2_LINE_LOAD(x_sd0, x_sd1, x_sd2, x_sd3);   \
	MLIB_S_SUM_##V##_##TT(                                   \
	    x_sd0, x_sd1, x_sd2, x_sd3, x_v1_l, x_v1_h);         \
	for (i = 0; i < ((src_width * channels) / 2 - 15);       \
	    i += 16) {                                           \
	MLIB_S_ZOOM_BC2_LINE_LOAD(x_sd0, x_sd1, x_sd2, x_sd3);   \
	MLIB_S_SUM_##V##_##TT(                                   \
	    x_sd0, x_sd1, x_sd2, x_sd3, x_v2_l, x_v2_h);         \
	MLIB_S_ZOOM_BC2_LINE_LOAD(x_sd0, x_sd1, x_sd2, x_sd3);   \
	MLIB_S_SUM_##V##_##TT(                                   \
	    x_sd0, x_sd1, x_sd2, x_sd3, x_v3_l, x_v3_h);         \
	MLIB_S_SUM_LINE##CH(H, V);                               \
	MLIB_S_PACK##CH##_##TT;                                  \
	_mm_storeu_si128((__m128i *)pd, x_v0);                   \
	_mm_storeu_si128((__m128i *)(pd + 8), x_v1);             \
	_mm_storeu_si128((__m128i *)(pd + 16), x_v2);            \
	_mm_storeu_si128((__m128i *)(pd + 24), x_v3);            \
	pd += 32;                                                \
	x_v0_l = x_v2_l;                                         \
	x_v0_h = x_v2_h;                                         \
	x_v1_l = x_v3_l;                                         \
	x_v1_h = x_v3_h;                                         \
	}                                                        \
	_mm_storeu_si128((__m128i *)v, x_v0_l);                  \
	_mm_storeu_si128((__m128i *)(v + 4), x_v0_h);            \
	_mm_storeu_si128((__m128i *)(v + 8), x_v1_l);            \
	_mm_storeu_si128((__m128i *)(v + 12), x_v1_h);           \
	MLIB_S_ZOOM_BC2_LINE_TAIL(H, V, STORE_SAT);              \
	dl += dst_stride;                                        \
}

/* *********************************************************** */

#define	MLIB_S_ZOOM_BC2_LINE_U8(TT, H, V, STORE_SAT)                 \
	switch (channels) {                                          \
	case 1:                                                      \
		MLIB_S_ZOOM_BC2_LINE_U8_124(1, TT, H, V, STORE_SAT)  \
		break;                                               \
	case 2:                                                      \
		MLIB_S_ZOOM_BC2_LINE_U8_124(2, TT, H, V, STORE_SAT)  \
		break;                                               \
	case 3:                                                      \
		MLIB_S_ZOOM_BC2_LINE_U8_3(TT, H, V, STORE_SAT)       \
		break;                                               \
	case 4:                                                      \
		MLIB_S_ZOOM_BC2_LINE_U8_124(4, TT, H, V, STORE_SAT)  \
		break;                                               \
	}

/* *********************************************************** */

#define	MLIB_S_ZOOM_BC2_LINE(TT, H, V, STORE_SAT)                    \
	switch (channels) {                                          \
	case 1:                                                      \
		MLIB_S_ZOOM_BC2_LINE_124(1, TT, H, V, STORE_SAT)     \
		break;                                               \
	case 2:                                                      \
		MLIB_S_ZOOM_BC2_LINE_124(2, TT, H, V, STORE_SAT)     \
		break;                                               \
	case 3:                                                      \
		MLIB_S_ZOOM_BC2_LINE_3(TT, H, V, STORE_SAT)          \
		break;                                               \
	case 4:                                                      \
		MLIB_S_ZOOM_BC2_LINE_124(4, TT, H, V, STORE_SAT)     \
		break;                                               \
	}

/* *********************************************************** */

#define	MLIB_S_FUNC_ZOOM_BC2_U8(TT, STYPE, N, H, V)                        \
	void                                                               \
	mlib_s_ImageZoomIn2X_##TT##_BC2_FL##N(STYPE * pdst,                \
	    const STYPE * psrc, mlib_s32 src_width, mlib_s32 src_height,   \
	    mlib_s32 dst_stride, mlib_s32 src_stride,                      \
	    mlib_s32 src_w_beg, mlib_s32 src_h_beg, mlib_s32 channels)     \
	{                                                                  \
	    mlib_s32 i, j, k;                                              \
	    STYPE *pd;                                                     \
	    const STYPE *ps, *sl = psrc - channels;                        \
	    STYPE *dl = pdst;                                              \
	                                                                   \
	    src_w_beg &= 1;                                                \
	    src_h_beg &= 1;                                                \
	    src_width -= src_w_beg;                                        \
	    for (j = 0; j < src_height; j++) {                             \
		if (((src_h_beg + j) & 1) == 0) {                          \
		    MLIB_S_ZOOM_BC2_LINE_U8(TT, H, A##V, STORE_##TT);      \
		} else {                                                   \
		    MLIB_S_ZOOM_BC2_LINE_U8(TT, H, B##V, STORE_##TT);      \
		    sl += src_stride;                                      \
		}                                                          \
	    }                                                              \
	}

/* *********************************************************** */

#define	MLIB_S_FUNC_ZOOM_BC2(TT, STYPE, N, H, V)                           \
	void                                                               \
	mlib_s_ImageZoomIn2X_##TT##_BC2_FL##N(STYPE * pdst,                \
	    const STYPE * psrc, mlib_s32 src_width, mlib_s32 src_height,   \
	    mlib_s32 dst_stride, mlib_s32 src_stride,                      \
	    mlib_s32 src_w_beg, mlib_s32 src_h_beg, mlib_s32 channels)     \
	{                                                                  \
	    mlib_s32 i, j, k;                                              \
	    STYPE *pd;                                                     \
	    const STYPE *ps, *sl = psrc - channels;                        \
	    STYPE *dl = pdst;                                              \
	                                                                   \
	    src_w_beg &= 1;                                                \
	    src_h_beg &= 1;                                                \
	    src_width -= src_w_beg;                                        \
	    for (j = 0; j < src_height; j++) {                             \
		if (((src_h_beg + j) & 1) == 0) {                          \
		    MLIB_S_ZOOM_BC2_LINE(TT, H, A##V, STORE_##TT);         \
		} else {                                                   \
		    MLIB_S_ZOOM_BC2_LINE(TT, H, B##V, STORE_##TT);         \
		    sl += src_stride;                                      \
		}                                                          \
	    }                                                              \
	}

/* *********************************************************** */

#define	TTYPE	mlib_s32

MLIB_S_FUNC_ZOOM_BC2_U8(
    U8,
    mlib_u8,
    0,
    0,
    0)
MLIB_S_FUNC_ZOOM_BC2_U8(
    U8,
    mlib_u8,
    1,
    0,
    1)
MLIB_S_FUNC_ZOOM_BC2_U8(
    U8,
    mlib_u8,
    2,
    1,
    0)
MLIB_S_FUNC_ZOOM_BC2_U8(
    U8,
    mlib_u8,
    3,
    1,
    1)

MLIB_S_FUNC_ZOOM_BC2(
    S16,
    mlib_s16,
    0,
    0,
    0)
MLIB_S_FUNC_ZOOM_BC2(
    S16,
    mlib_s16,
    1,
    0,
    1)
MLIB_S_FUNC_ZOOM_BC2(
    S16,
    mlib_s16,
    2,
    1,
    0)
MLIB_S_FUNC_ZOOM_BC2(
    S16,
    mlib_s16,
    3,
    1,
    1)

MLIB_S_FUNC_ZOOM_BC2(
    U16,
    mlib_u16,
    0,
    0,
    0)
MLIB_S_FUNC_ZOOM_BC2(
    U16,
    mlib_u16,
    1,
    0,
    1)
MLIB_S_FUNC_ZOOM_BC2(
    U16,
    mlib_u16,
    2,
    1,
    0)
MLIB_S_FUNC_ZOOM_BC2(
    U16,
    mlib_u16,
    3,
    1,
    1)

/* *********************************************************** */
