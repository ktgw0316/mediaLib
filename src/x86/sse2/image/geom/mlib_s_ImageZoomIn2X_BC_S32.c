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

#pragma ident	"@(#)mlib_s_ImageZoomIn2X_BC_S32.c	9.3	07/11/05 SMI"

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
#ifdef __SUNPRO_C
#include <sunmedia_intrin.h>
#else
#include <emmintrin.h>
#endif

/* *********************************************************** */

#ifdef MLIB_USE_FTOI_CLAMPING

#define	STORE_S32(res, x)	res = (mlib_s32)(x)

#else /* MLIB_USE_FTOI_CLAMPING */

#define	STORE_S32(res, x)                                       \
	if (x >= MLIB_S32_MAX)                                  \
	    x = MLIB_S32_MAX;                                   \
	if (x <= MLIB_S32_MIN)                                  \
	    x = MLIB_S32_MIN;                                   \
	res = (mlib_s32)(x)

#endif /* MLIB_USE_FTOI_CLAMPING */

#define	STORE_F32(res, x)	res = (x)

#define	STORE_D64(res, x)	res = (x)

#define	SKIP(res, x)

/* *********************************************************** */

#define	MLIB_S_MULCONST                             \
	__m128d x_a, x_b, x_c, x_d;                 \
	const __m128d x_mul3 = _mm_set1_pd(3.0);    \
	const __m128d x_mul9 = _mm_set1_pd(9.0);    \
	const __m128d x_mul29 = _mm_set1_pd(29.0);  \
	const __m128d x_mul111 = _mm_set1_pd(111.0)

#define	MLIB_S_SUM_A0(a, b, c, d, e)         \
	x_a = _mm_mul_pd(a, x_mul9);         \
	x_b = _mm_mul_pd(b, x_mul111);       \
	x_c = _mm_mul_pd(c, x_mul29);        \
	x_d = _mm_mul_pd(d, x_mul3);         \
	e = _mm_add_pd(_mm_sub_pd(x_b, x_a), \
	    _mm_sub_pd(x_c, x_d))

#define	MLIB_S_SUM_B0(a, b, c, d, e)	     \
	x_a = _mm_mul_pd(a, x_mul3);         \
	x_b = _mm_mul_pd(b, x_mul29);        \
	x_c = _mm_mul_pd(c, x_mul111);       \
	x_d = _mm_mul_pd(d, x_mul9);         \
	e = _mm_add_pd(_mm_sub_pd(x_b, x_a), \
	    _mm_sub_pd(x_c, x_d))

#define	SUM_A0(a, b, c, d)	((TTYPE)111*b - (TTYPE)9*a + \
				(TTYPE)29*c - (TTYPE)3*d)
#define	SUM_B0(a, b, c, d)	((TTYPE)29*b - (TTYPE)3*a + \
				(TTYPE)111*c - (TTYPE)9*d)

#define	SCL_A0	(TTYPE)0.0078125

#define	SCL_B0	(TTYPE)0.0078125

/* *********************************************************** */

#define	MLIB_S_SUM_A1(a, b, c, d, e)	\
	e = b

#define	MLIB_S_SUM_B1(a, b, c, d, e)	            \
	x_b = _mm_mul_pd(x_mul9, _mm_add_pd(b, c)); \
	e = _mm_sub_pd(x_b, _mm_add_pd(a, d))

#define	SUM_A1(a, b, c, d)	((TTYPE)b)

#define	SUM_B1(a, b, c, d)	((TTYPE)9*((TTYPE)b + (TTYPE)c) - a - d)

#define	SCL_A1	(TTYPE)1.0

#define	SCL_B1	(TTYPE)0.0625

/* *********************************************************** */

#define	MLIB_S_ZOOM_BC_LINE_LOAD(V, VL, VH, WL, WH)                  \
	x_sd0 = _mm_loadu_si128((__m128i *)(ps - src_stride));       \
	x_sd1 = _mm_loadu_si128((__m128i *)ps);                      \
	x_sd2 = _mm_loadu_si128((__m128i *)(ps + src_stride));       \
	x_sd3 = _mm_loadu_si128((__m128i *)(ps + 2 * src_stride));   \
	x_sd0_l = _mm_cvtepi32_pd(x_sd0);                            \
	x_sd0_h = _mm_cvtepi32_pd(_mm_srli_si128(x_sd0, 8));         \
	x_sd1_l = _mm_cvtepi32_pd(x_sd1);                            \
	x_sd1_h = _mm_cvtepi32_pd(_mm_srli_si128(x_sd1, 8));         \
	x_sd2_l = _mm_cvtepi32_pd(x_sd2);                            \
	x_sd2_h = _mm_cvtepi32_pd(_mm_srli_si128(x_sd2, 8));         \
	x_sd3_l = _mm_cvtepi32_pd(x_sd3);                            \
	x_sd3_h = _mm_cvtepi32_pd(_mm_srli_si128(x_sd3, 8));         \
	MLIB_S_SUM_A##V(x_sd0_l, x_sd1_l, x_sd2_l, x_sd3_l, VL);     \
	MLIB_S_SUM_B##V(x_sd0_l, x_sd1_l, x_sd2_l, x_sd3_l, WL);     \
	MLIB_S_SUM_A##V(x_sd0_h, x_sd1_h, x_sd2_h, x_sd3_h, VH);     \
	MLIB_S_SUM_B##V(x_sd0_h, x_sd1_h, x_sd2_h, x_sd3_h, WH)

/* *********************************************************** */

#define	MLIB_S_ZOOM_BC_LINE_PRE(V)                           \
    slc = sl;                                                \
    dlc = dl;                                                \
    for (k = 0; k < channels; k++) {                         \
    ps = slc + k;                                            \
    v0[k] = SUM_A##V(ps[-src_stride], ps[0], ps[src_stride], \
	    ps[2 * src_stride]);                             \
    w0[k] = SUM_B##V(ps[-src_stride], ps[0], ps[src_stride], \
	    ps[2 * src_stride]);                             \
    ps += channels;                                          \
    v1[k] = SUM_A##V(ps[-src_stride], ps[0], ps[src_stride], \
	    ps[2 * src_stride]);                             \
    w1[k] = SUM_B##V(ps[-src_stride], ps[0], ps[src_stride], \
	    ps[2 * src_stride]);                             \
    ps += channels;                                          \
    v2[k] = SUM_A##V(ps[-src_stride], ps[0], ps[src_stride], \
	    ps[2 * src_stride]);                             \
    w2[k] = SUM_B##V(ps[-src_stride], ps[0], ps[src_stride], \
	    ps[2 * src_stride]);                             \
    ps++;                                                    \
	}                                                    \
    pd = dlc

/* *********************************************************** */

#define	MLIB_S_ZOOM_BC_LINE_STORE(STORE_UP, STORE_DN)        \
	STORE_UP(pd[0], r0);                                 \
	STORE_UP(pd[channels], r1);                          \
	STORE_DN(pd[dst_stride], r2);                        \
	STORE_DN(pd[dst_stride + channels], r3)

/* *********************************************************** */

#define	MLIB_S_ZOOM_BC_LINE_1(H, V, STORE_UP, STORE_DN) {  \
	TTYPE v0, v1, v2, v3, w0, w1, w2, w3;              \
	ps = slc = sl;                                     \
	pd = dlc = dl;                                     \
	v0 = SUM_A##V(ps[-src_stride], ps[0],              \
	    ps[src_stride], ps[2 * src_stride]);           \
	w0 = SUM_B##V(ps[-src_stride], ps[0],              \
	    ps[src_stride], ps[2 * src_stride]);           \
	ps += channels;                                    \
	v1 = SUM_A##V(ps[-src_stride], ps[0],              \
	    ps[src_stride], ps[2 * src_stride]);           \
	w1 = SUM_B##V(ps[-src_stride], ps[0],              \
	    ps[src_stride], ps[2 * src_stride]);           \
	ps += channels;                                    \
	v2 = SUM_A##V(ps[-src_stride], ps[0],              \
	    ps[src_stride], ps[2 * src_stride]);           \
	w2 = SUM_B##V(ps[-src_stride], ps[0],              \
	    ps[src_stride], ps[2 * src_stride]);           \
	ps += channels;                                    \
	if (src_w_beg) {                                   \
	v3 = SUM_A##V(ps[-src_stride], ps[0],              \
	    ps[src_stride], ps[2 * src_stride]);           \
	w3 = SUM_B##V(ps[-src_stride], ps[0],              \
	    ps[src_stride], ps[2 * src_stride]);           \
	ps += channels;                                    \
	r1 = SUM_B##H(v0, v1, v2,                          \
	    v3) * (SCL_A##V * SCL_B##H);                   \
	r3 = SUM_B##H(w0, w1, w2,                          \
	    w3) * (SCL_B##V * SCL_B##H);                   \
	STORE_UP(pd[0], r1);                               \
	STORE_DN(pd[dst_stride], r3);                      \
	pd += channels;                                    \
	v0 = v1;                                           \
	v1 = v2;                                           \
	v2 = v3;                                           \
	w0 = w1;                                           \
	w1 = w2;                                           \
	w2 = w3;                                           \
	}                                                  \
	__m128i x_sd0, x_sd1, x_sd2, x_sd3;                \
	__m128d x_sd0_l, x_sd1_l, x_sd2_l, x_sd3_l;        \
	__m128d x_sd0_h, x_sd1_h, x_sd2_h, x_sd3_h;        \
	__m128d x_v3_l, x_v3_h, x_w3_l, x_w3_h;            \
	__m128d x_v4_l, x_v4_h, x_w4_l, x_w4_h;            \
	TTYPE *v3l, *v4l, *v3h, *v4h;                      \
	TTYPE *w3l, *w4l, *w3h, *w4h;                      \
	v3l = (TTYPE *)&x_v3_l;                            \
	v3h = (TTYPE *)&x_v3_h;                            \
	v4l = (TTYPE *)&x_v4_l;                            \
	v4h = (TTYPE *)&x_v4_h;                            \
	w3l = (TTYPE *)&x_w3_l;                            \
	w3h = (TTYPE *)&x_w3_h;                            \
	w4l = (TTYPE *)&x_w4_l;                            \
	w4h = (TTYPE *)&x_w4_h;                            \
	for (i = 0; i < (src_width / 2 - 7); i += 8) {     \
	MLIB_S_MULCONST;                                   \
	MLIB_S_ZOOM_BC_LINE_LOAD(                          \
	    V, x_v3_l, x_v3_h, x_w3_l, x_w3_h);            \
	ps += 4;                                           \
	MLIB_S_ZOOM_BC_LINE_LOAD(                          \
	    V, x_v4_l, x_v4_h, x_w4_l, x_w4_h);            \
	r0 = SUM_A##H(v0, v1, v2, v3l[0]) *                \
	    (SCL_A##V * SCL_A##H);                         \
	r1 = SUM_B##H(v0, v1, v2, v3l[0]) *                \
	    (SCL_A##V * SCL_B##H);                         \
	r2 = SUM_A##H(w0, w1, w2, w3l[0]) *                \
	    (SCL_B##V * SCL_A##H);                         \
	r3 = SUM_B##H(w0, w1, w2, w3l[0]) *                \
	    (SCL_B##V * SCL_B##H);                         \
	MLIB_S_ZOOM_BC_LINE_STORE(STORE_UP, STORE_DN);     \
	pd += 2 * channels;                                \
	r0 = SUM_A##H(v1, v2, v3l[0], v3l[1]) *            \
	    (SCL_A##V * SCL_A##H);                         \
	r1 = SUM_B##H(v1, v2, v3l[0], v3l[1]) *            \
	    (SCL_A##V * SCL_B##H);                         \
	r2 = SUM_A##H(w1, w2, w3l[0], w3l[1]) *            \
	    (SCL_B##V * SCL_A##H);                         \
	r3 = SUM_B##H(w1, w2, w3l[0], w3l[1]) *            \
	    (SCL_B##V * SCL_B##H);                         \
	MLIB_S_ZOOM_BC_LINE_STORE(STORE_UP, STORE_DN);     \
	pd += 2 * channels;                                \
	r0 = SUM_A##H(v2, v3l[0], v3l[1], v3h[0]) *        \
	    (SCL_A##V * SCL_A##H);                         \
	r1 = SUM_B##H(v2, v3l[0], v3l[1], v3h[0]) *        \
	    (SCL_A##V * SCL_B##H);                         \
	r2 = SUM_A##H(w2, w3l[0], w3l[1], w3h[0]) *        \
	    (SCL_B##V * SCL_A##H);                         \
	r3 = SUM_B##H(w2, w3l[0], w3l[1], w3h[0]) *        \
	    (SCL_B##V * SCL_B##H);                         \
	MLIB_S_ZOOM_BC_LINE_STORE(STORE_UP, STORE_DN);     \
	pd += 2 * channels;                                \
	r0 = SUM_A##H(v3l[0], v3l[1], v3h[0],              \
	    v3h[1]) * (SCL_A##V * SCL_A##H);               \
	r1 = SUM_B##H(v3l[0], v3l[1], v3h[0],              \
	    v3h[1]) * (SCL_A##V * SCL_B##H);               \
	r2 = SUM_A##H(w3l[0], w3l[1], w3h[0],              \
	    w3h[1]) * (SCL_B##V * SCL_A##H);               \
	r3 = SUM_B##H(w3l[0], w3l[1], w3h[0],              \
	    w3h[1]) * (SCL_B##V * SCL_B##H);               \
	MLIB_S_ZOOM_BC_LINE_STORE(STORE_UP, STORE_DN);     \
	pd += 2 * channels;                                \
	r0 = SUM_A##H(v3l[1], v3h[0], v3h[1],              \
	    v4l[0]) * (SCL_A##V * SCL_A##H);               \
	r1 = SUM_B##H(v3l[1], v3h[0], v3h[1],              \
	    v4l[0]) * (SCL_A##V * SCL_B##H);               \
	r2 = SUM_A##H(w3l[1], w3h[0], w3h[1],              \
	    w4l[0]) * (SCL_B##V * SCL_A##H);               \
	r3 = SUM_B##H(w3l[1], w3h[0], w3h[1],              \
	    w4l[0]) * (SCL_B##V * SCL_B##H);               \
	MLIB_S_ZOOM_BC_LINE_STORE(STORE_UP, STORE_DN);     \
	pd += 2 * channels;                                \
	r0 = SUM_A##H(v3h[0], v3h[1], v4l[0],              \
	    v4l[1]) * (SCL_A##V * SCL_A##H);               \
	r1 = SUM_B##H(v3h[0], v3h[1], v4l[0],              \
	    v4l[1]) * (SCL_A##V * SCL_B##H);               \
	r2 = SUM_A##H(w3h[0], w3h[1], w4l[0],              \
	    w4l[1]) * (SCL_B##V * SCL_A##H);               \
	r3 = SUM_B##H(w3h[0], w3h[1], w4l[0],              \
	    w4l[1]) * (SCL_B##V * SCL_B##H);               \
	MLIB_S_ZOOM_BC_LINE_STORE(STORE_UP, STORE_DN);     \
	pd += 2 * channels;                                \
	r0 = SUM_A##H(v3h[1], v4l[0], v4l[1],              \
	    v4h[0]) * (SCL_A##V * SCL_A##H);               \
	r1 = SUM_B##H(v3h[1], v4l[0], v4l[1],              \
	    v4h[0]) * (SCL_A##V * SCL_B##H);               \
	r2 = SUM_A##H(w3h[1], w4l[0], w4l[1],              \
	    w4h[0]) * (SCL_B##V * SCL_A##H);               \
	r3 = SUM_B##H(w3h[1], w4l[0], w4l[1],              \
	    w4h[0]) * (SCL_B##V * SCL_B##H);               \
	MLIB_S_ZOOM_BC_LINE_STORE(STORE_UP, STORE_DN);     \
	pd += 2 * channels;                                \
	r0 = SUM_A##H(v4l[0], v4l[1], v4h[0],              \
	    v4h[1]) * (SCL_A##V * SCL_A##H);               \
	r1 = SUM_B##H(v4l[0], v4l[1], v4h[0],              \
	    v4h[1]) * (SCL_A##V * SCL_B##H);               \
	r2 = SUM_A##H(w4l[0], w4l[1], w4h[0],              \
	    w4h[1]) * (SCL_B##V * SCL_A##H);               \
	r3 = SUM_B##H(w4l[0], w4l[1], w4h[0],              \
	    w4h[1]) * (SCL_B##V * SCL_B##H);               \
	MLIB_S_ZOOM_BC_LINE_STORE(STORE_UP, STORE_DN);     \
	pd += 2 * channels;                                \
	v0 = v4l[1];                                       \
	v1 = v4h[0];                                       \
	v2 = v4h[1];                                       \
	w0 = w4l[1];                                       \
	w1 = w4h[0];                                       \
	w2 = w4h[1];                                       \
	ps += 4 * channels;                                \
	}                                                  \
	if (i < (src_width / 2 - 3)) {                     \
	MLIB_S_MULCONST;                                   \
	MLIB_S_ZOOM_BC_LINE_LOAD(                          \
	    V, x_v3_l, x_v3_h, x_w3_l, x_w3_h);            \
	r0 = SUM_A##H(v0, v1, v2, v3l[0]) *                \
	    (SCL_A##V * SCL_A##H);                         \
	r1 = SUM_B##H(v0, v1, v2, v3l[0]) *                \
	    (SCL_A##V * SCL_B##H);                         \
	r2 = SUM_A##H(w0, w1, w2, w3l[0]) *                \
	    (SCL_B##V * SCL_A##H);                         \
	r3 = SUM_B##H(w0, w1, w2, w3l[0]) *                \
	    (SCL_B##V * SCL_B##H);                         \
	MLIB_S_ZOOM_BC_LINE_STORE(STORE_UP, STORE_DN);     \
	pd += 2 * channels;                                \
	r0 = SUM_A##H(v1, v2, v3l[0], v3l[1]) *            \
	    (SCL_A##V * SCL_A##H);                         \
	r1 = SUM_B##H(v1, v2, v3l[0], v3l[1]) *            \
	    (SCL_A##V * SCL_B##H);                         \
	r2 = SUM_A##H(w1, w2, w3l[0], w3l[1]) *            \
	    (SCL_B##V * SCL_A##H);                         \
	r3 = SUM_B##H(w1, w2, w3l[0], w3l[1]) *            \
	    (SCL_B##V * SCL_B##H);                         \
	MLIB_S_ZOOM_BC_LINE_STORE(STORE_UP, STORE_DN);     \
	pd += 2 * channels;                                \
	r0 = SUM_A##H(v2, v3l[0], v3l[1], v3h[0]) *        \
	    (SCL_A##V * SCL_A##H);                         \
	r1 = SUM_B##H(v2, v3l[0], v3l[1], v3h[0]) *        \
	    (SCL_A##V * SCL_B##H);                         \
	r2 = SUM_A##H(w2, w3l[0], w3l[1], w3h[0]) *        \
	    (SCL_B##V * SCL_A##H);                         \
	r3 = SUM_B##H(w2, w3l[0], w3l[1], w3h[0]) *        \
	    (SCL_B##V * SCL_B##H);                         \
	MLIB_S_ZOOM_BC_LINE_STORE(STORE_UP, STORE_DN);     \
	pd += 2 * channels;                                \
	r0 = SUM_A##H(v3l[0], v3l[1], v3h[0],              \
	    v3h[1]) * (SCL_A##V * SCL_A##H);               \
	r1 = SUM_B##H(v3l[0], v3l[1], v3h[0],              \
	    v3h[1]) * (SCL_A##V * SCL_B##H);               \
	r2 = SUM_A##H(w3l[0], w3l[1], w3h[0],              \
	    w3h[1]) * (SCL_B##V * SCL_A##H);               \
	r3 = SUM_B##H(w3l[0], w3l[1], w3h[0],              \
	    w3h[1]) * (SCL_B##V * SCL_B##H);               \
	MLIB_S_ZOOM_BC_LINE_STORE(STORE_UP, STORE_DN);     \
	pd += 2 * channels;                                \
	v0 = v3l[1];                                       \
	v1 = v3h[0];                                       \
	v2 = v3h[1];                                       \
	w0 = w3l[1];                                       \
	w1 = w3h[0];                                       \
	w2 = w3h[1];                                       \
	ps += 4 * channels;                                \
	i += 4;                                            \
	}                                                  \
	for (; i < (src_width / 2); i++) {                 \
	v3 = SUM_A##V(ps[-src_stride], ps[0],              \
	    ps[src_stride], ps[2 * src_stride]);           \
	w3 = SUM_B##V(ps[-src_stride], ps[0],              \
	    ps[src_stride], ps[2 * src_stride]);           \
	ps += channels;                                    \
	r0 = SUM_A##H(v0, v1, v2,                          \
	    v3) * (SCL_A##V * SCL_A##H);                   \
	r1 = SUM_B##H(v0, v1, v2,                          \
	    v3) * (SCL_A##V * SCL_B##H);                   \
	r2 = SUM_A##H(w0, w1, w2,                          \
	    w3) * (SCL_B##V * SCL_A##H);                   \
	r3 = SUM_B##H(w0, w1, w2,                          \
	    w3) * (SCL_B##V * SCL_B##H);                   \
	MLIB_S_ZOOM_BC_LINE_STORE(STORE_UP, STORE_DN);     \
	pd += 2 * channels;                                \
	v0 = v1;                                           \
	v1 = v2;                                           \
	v2 = v3;                                           \
	w0 = w1;                                           \
	w1 = w2;                                           \
	w2 = w3;                                           \
	}                                                  \
	if (src_width & 1) {                               \
	v3 = SUM_A##V(ps[-src_stride], ps[0],              \
	    ps[src_stride], ps[2 * src_stride]);           \
	w3 = SUM_B##V(ps[-src_stride], ps[0],              \
	    ps[src_stride], ps[2 * src_stride]);           \
	r0 = SUM_A##H(v0, v1, v2,                          \
	    v3) * (SCL_A##V * SCL_A##H);                   \
	r2 = SUM_A##H(w0, w1, w2,                          \
	    w3) * (SCL_B##V * SCL_A##H);                   \
	STORE_UP(pd[0], r0);                               \
	STORE_DN(pd[dst_stride], r2);                      \
	}                                                  \
                                                           \
	sl += src_stride;                                  \
	dl += 2 * dst_stride;                              \
}

/* *********************************************************** */

#define	MLIB_S_ZOOM_BC_LINE_H_S32(H, V,  STORE_UP, STORE_DN)       \
	if (src_w_beg) {                                           \
	for (k = 0; k < channels; k++) {                           \
		ps = slc + 3 * channels + k;                       \
		pd = dlc + k;                                      \
		v3[k] = SUM_A##V(ps[-src_stride], ps[0],           \
		    ps[src_stride], ps[2 * src_stride]);           \
		w3[k] = SUM_B##V(ps[-src_stride], ps[0],           \
		    ps[src_stride], ps[2 * src_stride]);           \
		ps++;                                              \
		r1 = SUM_B##H(v0[k], v1[k], v2[k],                 \
		    v3[k]) * (SCL_A##V * SCL_B##H);                \
		r3 = SUM_B##H(w0[k], w1[k], w2[k],                 \
		    w3[k]) * (SCL_B##V * SCL_B##H);                \
		STORE_UP(pd[0], r1);                               \
		STORE_DN(pd[dst_stride], r3);                      \
		pd++;                                              \
		v0[k] = v1[k];                                     \
		v1[k] = v2[k];                                     \
		v2[k] = v3[k];                                     \
		w0[k] = w1[k];                                     \
		w1[k] = w2[k];                                     \
		w2[k] = w3[k];                                     \
	}                                                          \
	slc += channels;                                           \
	dlc += channels;                                           \
	}                                                          \

/* *********************************************************** */

#define	MLIB_S_ZOOM_BC_LINE_2(H, V, STORE_UP, STORE_DN) {          \
	TTYPE v0[2], v1[2], v2[2], v3[2], v4[2], v5[2], v6[2];     \
	TTYPE w0[2], w1[2], w2[2], w3[2], w4[2], w5[2], w6[2];     \
	__m128i x_sd0, x_sd1, x_sd2, x_sd3;                        \
	__m128d x_sd0_l, x_sd1_l, x_sd2_l, x_sd3_l;                \
	__m128d x_sd0_h, x_sd1_h, x_sd2_h, x_sd3_h;                \
	__m128d x_v3_l, x_v3_h, x_w3_l, x_w3_h;                    \
	__m128d x_v4_l, x_v4_h, x_w4_l, x_w4_h;                    \
	TTYPE *v3l, *v4l, *v3h, *v4h;                              \
	TTYPE *w3l, *w4l, *w3h, *w4h;                              \
	v3l = (TTYPE *)&x_v3_l;                                    \
	v3h = (TTYPE *)&x_v3_h;                                    \
	v4l = (TTYPE *)&x_v4_l;                                    \
	v4h = (TTYPE *)&x_v4_h;                                    \
	w3l = (TTYPE *)&x_w3_l;                                    \
	w3h = (TTYPE *)&x_w3_h;                                    \
	w4l = (TTYPE *)&x_w4_l;                                    \
	w4h = (TTYPE *)&x_w4_h;                                    \
	MLIB_S_ZOOM_BC_LINE_PRE(V);                                \
	MLIB_S_ZOOM_BC_LINE_H_S32(H, V, STORE_UP, STORE_DN);       \
	for (i = 0; i < (src_width / 8); i++) {                    \
		MLIB_S_MULCONST;                                   \
		MLIB_S_ZOOM_BC_LINE_LOAD(                          \
		    V, x_v3_l, x_v3_h, x_w3_l, x_w3_h);            \
		ps += 4;                                           \
		MLIB_S_ZOOM_BC_LINE_LOAD(                          \
		    V, x_v4_l, x_v4_h, x_w4_l, x_w4_h);            \
		pd = dlc + i * 8 * channels;                       \
		r0 = SUM_A##H(v0[0], v1[0], v2[0],                 \
		    v3l[0]) * (SCL_A##V * SCL_A##H);               \
		r1 = SUM_B##H(v0[0], v1[0], v2[0],                 \
		    v3l[0]) * (SCL_A##V * SCL_B##H);               \
		r2 = SUM_A##H(w0[0], w1[0], w2[0],                 \
		    w3l[0]) * (SCL_B##V * SCL_A##H);               \
		r3 = SUM_B##H(w0[0], w1[0], w2[0],                 \
		    w3l[0]) * (SCL_B##V * SCL_B##H);               \
		MLIB_S_ZOOM_BC_LINE_STORE(STORE_UP, STORE_DN);     \
		pd += 2 * channels;                                \
		r0 = SUM_A##H(v1[0], v2[0], v3l[0], v3h[0]) *      \
		    (SCL_A##V * SCL_A##H);                         \
		r1 = SUM_B##H(v1[0], v2[0], v3l[0], v3h[0]) *      \
		    (SCL_A##V * SCL_B##H);                         \
		r2 = SUM_A##H(w1[0], w2[0], w3l[0], w3h[0]) *      \
		    (SCL_B##V * SCL_A##H);                         \
		r3 = SUM_B##H(w1[0], w2[0], w3l[0], w3h[0]) *      \
		    (SCL_B##V * SCL_B##H);                         \
		MLIB_S_ZOOM_BC_LINE_STORE(STORE_UP, STORE_DN);     \
		pd += 2 * channels;                                \
		r0 = SUM_A##H(v2[0], v3l[0], v3h[0], v4l[0]) *     \
		    (SCL_A##V * SCL_A##H);                         \
		r1 = SUM_B##H(v2[0], v3l[0], v3h[0], v4l[0]) *     \
		    (SCL_A##V * SCL_B##H);                         \
		r2 = SUM_A##H(w2[0], w3l[0], w3h[0], w4l[0]) *     \
		    (SCL_B##V * SCL_A##H);                         \
		r3 = SUM_B##H(w2[0], w3l[0], w3h[0], w4l[0]) *     \
		    (SCL_B##V * SCL_B##H);                         \
		MLIB_S_ZOOM_BC_LINE_STORE(STORE_UP, STORE_DN);     \
		pd += 2 * channels;                                \
		r0 = SUM_A##H(v3l[0], v3h[0], v4l[0], v4h[0]) *    \
		    (SCL_A##V * SCL_A##H);                         \
		r1 = SUM_B##H(v3l[0], v3h[0], v4l[0], v4h[0]) *    \
		    (SCL_A##V * SCL_B##H);                         \
		r2 = SUM_A##H(w3l[0], w3h[0], w4l[0], w4h[0]) *    \
		    (SCL_B##V * SCL_A##H);                         \
		r3 = SUM_B##H(w3l[0], w3h[0], w4l[0], w4h[0]) *    \
		    (SCL_B##V * SCL_B##H);                         \
		MLIB_S_ZOOM_BC_LINE_STORE(STORE_UP, STORE_DN);     \
		                                                   \
		pd = dlc + i * 8 * channels + 1;                   \
		r0 = SUM_A##H(v0[1], v1[1], v2[1],                 \
		    v3l[1]) * (SCL_A##V * SCL_A##H);               \
		r1 = SUM_B##H(v0[1], v1[1], v2[1],                 \
		    v3l[1]) * (SCL_A##V * SCL_B##H);               \
		r2 = SUM_A##H(w0[1], w1[1], w2[1],                 \
		    w3l[1]) * (SCL_B##V * SCL_A##H);               \
		r3 = SUM_B##H(w0[1], w1[1], w2[1],                 \
		    w3l[1]) * (SCL_B##V * SCL_B##H);               \
		MLIB_S_ZOOM_BC_LINE_STORE(STORE_UP, STORE_DN);     \
		pd += 2 * channels;                                \
		r0 = SUM_A##H(v1[1], v2[1], v3l[1], v3h[1]) *      \
		    (SCL_A##V * SCL_A##H);                         \
		r1 = SUM_B##H(v1[1], v2[1], v3l[1], v3h[1]) *      \
		    (SCL_A##V * SCL_B##H);                         \
		r2 = SUM_A##H(w1[1], w2[1], w3l[1], w3h[1]) *      \
		    (SCL_B##V * SCL_A##H);                         \
		r3 = SUM_B##H(w1[1], w2[1], w3l[1], w3h[1]) *      \
		    (SCL_B##V * SCL_B##H);                         \
		MLIB_S_ZOOM_BC_LINE_STORE(STORE_UP, STORE_DN);     \
		pd += 2 * channels;                                \
		r0 = SUM_A##H(v2[1], v3l[1], v3h[1], v4l[1]) *     \
		    (SCL_A##V * SCL_A##H);                         \
		r1 = SUM_B##H(v2[1], v3l[1], v3h[1], v4l[1]) *     \
		    (SCL_A##V * SCL_B##H);                         \
		r2 = SUM_A##H(w2[1], w3l[1], w3h[1], w4l[1]) *     \
		    (SCL_B##V * SCL_A##H);                         \
		r3 = SUM_B##H(w2[1], w3l[1], w3h[1], w4l[1]) *     \
		    (SCL_B##V * SCL_B##H);                         \
		MLIB_S_ZOOM_BC_LINE_STORE(STORE_UP, STORE_DN);     \
		pd += 2 * channels;                                \
		r0 = SUM_A##H(v3l[1], v3h[1], v4l[1], v4h[1]) *    \
		    (SCL_A##V * SCL_A##H);                         \
		r1 = SUM_B##H(v3l[1], v3h[1], v4l[1], v4h[1]) *    \
		    (SCL_A##V * SCL_B##H);                         \
		r2 = SUM_A##H(w3l[1], w3h[1], w4l[1], w4h[1]) *    \
		    (SCL_B##V * SCL_A##H);                         \
		r3 = SUM_B##H(w3l[1], w3h[1], w4l[1], w4h[1]) *    \
		    (SCL_B##V * SCL_B##H);                         \
		MLIB_S_ZOOM_BC_LINE_STORE(STORE_UP, STORE_DN);     \
		pd++;                                              \
		v0[0] = v3h[0];                                    \
		v0[1] = v3h[1];                                    \
		v1[0] = v4l[0];                                    \
		v1[1] = v4l[1];                                    \
		v2[0] = v4h[0];                                    \
		v2[1] = v4h[1];                                    \
		w0[0] = w3h[0];                                    \
		w0[1] = w3h[1];                                    \
		w1[0] = w4l[0];                                    \
		w1[1] = w4l[1];                                    \
		w2[0] = w4h[0];                                    \
		w2[1] = w4h[1];                                    \
		ps += 2 * channels;                                \
	}                                                          \
	                                                           \
	for (k = 0; k < channels; k++) {                           \
		ps = slc + i * 4 * channels + k + 6;               \
		pd = dlc + i * 8 * channels + k;                   \
		for (int m = i * 4; m < (src_width / 2); m++) {    \
		v3[k] = SUM_A##V(ps[-src_stride], ps[0],           \
		    ps[src_stride], ps[2 * src_stride]);           \
		w3[k] = SUM_B##V(ps[-src_stride], ps[0],           \
		    ps[src_stride], ps[2 * src_stride]);           \
		ps += channels;                                    \
		r0 = SUM_A##H(v0[k], v1[k], v2[k],                 \
		    v3[k]) * (SCL_A##V * SCL_A##H);                \
		r1 = SUM_B##H(v0[k], v1[k], v2[k],                 \
		    v3[k]) * (SCL_A##V * SCL_B##H);                \
		r2 = SUM_A##H(w0[k], w1[k], w2[k],                 \
		    w3[k]) * (SCL_B##V * SCL_A##H);                \
		r3 = SUM_B##H(w0[k], w1[k], w2[k],                 \
		    w3[k]) * (SCL_B##V * SCL_B##H);                \
		MLIB_S_ZOOM_BC_LINE_STORE(STORE_UP, STORE_DN);     \
		pd += 2 * channels;                                \
		v0[k] = v1[k];                                     \
		v1[k] = v2[k];                                     \
		v2[k] = v3[k];                                     \
		w0[k] = w1[k];                                     \
		w1[k] = w2[k];                                     \
		w2[k] = w3[k];                                     \
	    }                                                      \
	    if (src_width & 1) {                                   \
		v3[k] = SUM_A##V(ps[-src_stride], ps[0],           \
		    ps[src_stride], ps[2 * src_stride]);           \
		w3[k] = SUM_B##V(ps[-src_stride], ps[0],           \
		    ps[src_stride], ps[2 * src_stride]);           \
		r0 = SUM_A##H(v0[k], v1[k], v2[k],                 \
		    v3[k]) * (SCL_A##V * SCL_A##H);                \
		r2 = SUM_A##H(w0[k], w1[k], w2[k],                 \
		    w3[k]) * (SCL_B##V * SCL_A##H);                \
		STORE_UP(pd[0], r0);                               \
		STORE_DN(pd[dst_stride], r2);                      \
	    }                                                      \
	}                                                          \
	                                                           \
	sl += src_stride;                                          \
	dl += 2 * dst_stride;                                      \
}

/* *********************************************************** */

#define	MLIB_S_ZOOM_BC_LINE_3(H, V, STORE_UP, STORE_DN) {          \
	TTYPE v0[3], v1[3], v2[3], v3s[3];                         \
	TTYPE w0[3], w1[3], w2[3], w3s[3];                         \
	__m128i x_sd0, x_sd1, x_sd2, x_sd3;                        \
	__m128d x_sd0_l, x_sd1_l, x_sd2_l, x_sd3_l;                \
	__m128d x_sd0_h, x_sd1_h, x_sd2_h, x_sd3_h;                \
	__m128d x_v3_l, x_v3_h, x_w3_l, x_w3_h;                    \
	__m128d x_v4_l, x_v4_h, x_w4_l, x_w4_h;                    \
	__m128d x_v5_l, x_v5_h, x_w5_l, x_w5_h;                    \
	TTYPE *v3, *v4, *v5, *v6, *v7, *v8;                        \
	TTYPE *w3, *w4, *w5, *w6, *w7, *w8;                        \
	v3 = (TTYPE *)&x_v3_l;                                     \
	v4 = (TTYPE *)&x_v3_h;                                     \
	v5 = (TTYPE *)&x_v4_l;                                     \
	v6 = (TTYPE *)&x_v4_h;                                     \
	v7 = (TTYPE *)&x_v5_l;                                     \
	v8 = (TTYPE *)&x_v5_h;                                     \
	w3 = (TTYPE *)&x_w3_l;                                     \
	w4 = (TTYPE *)&x_w3_h;                                     \
	w5 = (TTYPE *)&x_w4_l;                                     \
	w6 = (TTYPE *)&x_w4_h;                                     \
	w7 = (TTYPE *)&x_w5_l;                                     \
	w8 = (TTYPE *)&x_w5_h;                                     \
	MLIB_S_ZOOM_BC_LINE_PRE(V);                                \
	MLIB_S_ZOOM_BC_LINE_H_S32(H, V, STORE_UP, STORE_DN);       \
	for (i = 0; i < (src_width * 3 / (2 * 12)); i++) {         \
		MLIB_S_MULCONST;                                   \
		MLIB_S_ZOOM_BC_LINE_LOAD(                          \
		    V, x_v3_l, x_v3_h, x_w3_l, x_w3_h);            \
		ps += 4;                                           \
		MLIB_S_ZOOM_BC_LINE_LOAD(                          \
		    V, x_v4_l, x_v4_h, x_w4_l, x_w4_h);            \
		ps += 4;                                           \
		MLIB_S_ZOOM_BC_LINE_LOAD(                          \
		    V, x_v5_l, x_v5_h, x_w5_l, x_w5_h);            \
		ps += 4;                                           \
		pd = dlc + i * 8 * channels;                       \
		r0 = SUM_A##H(v0[0], v1[0], v2[0],                 \
		    v3[0]) * (SCL_A##V * SCL_A##H);                \
		r1 = SUM_B##H(v0[0], v1[0], v2[0],                 \
		    v3[0]) * (SCL_A##V * SCL_B##H);                \
		r2 = SUM_A##H(w0[0], w1[0], w2[0],                 \
		    w3[0]) * (SCL_B##V * SCL_A##H);                \
		r3 = SUM_B##H(w0[0], w1[0], w2[0],                 \
		    w3[0]) * (SCL_B##V * SCL_B##H);                \
		MLIB_S_ZOOM_BC_LINE_STORE(STORE_UP, STORE_DN);     \
		pd += 2 * channels;                                \
		r0 = SUM_A##H(v1[0], v2[0], v3[0], v4[1]) *        \
		    (SCL_A##V * SCL_A##H);                         \
		r1 = SUM_B##H(v1[0], v2[0], v3[0], v4[1]) *        \
		    (SCL_A##V * SCL_B##H);                         \
		r2 = SUM_A##H(w1[0], w2[0], w3[0], w4[1]) *        \
		    (SCL_B##V * SCL_A##H);                         \
		r3 = SUM_B##H(w1[0], w2[0], w3[0], w4[1]) *        \
		    (SCL_B##V * SCL_B##H);                         \
		MLIB_S_ZOOM_BC_LINE_STORE(STORE_UP, STORE_DN);     \
		pd += 2 * channels;                                \
		r0 = SUM_A##H(v2[0], v3[0], v4[1], v6[0]) *        \
		    (SCL_A##V * SCL_A##H);                         \
		r1 = SUM_B##H(v2[0], v3[0], v4[1], v6[0]) *        \
		    (SCL_A##V * SCL_B##H);                         \
		r2 = SUM_A##H(w2[0], w3[0], w4[1], w6[0]) *        \
		    (SCL_B##V * SCL_A##H);                         \
		r3 = SUM_B##H(w2[0], w3[0], w4[1], w6[0]) *        \
		    (SCL_B##V * SCL_B##H);                         \
		MLIB_S_ZOOM_BC_LINE_STORE(STORE_UP, STORE_DN);     \
		pd += 2 * channels;                                \
		r0 = SUM_A##H(v3[0], v4[1], v6[0], v7[1]) *        \
		    (SCL_A##V * SCL_A##H);                         \
		r1 = SUM_B##H(v3[0], v4[1], v6[0], v7[1]) *        \
		    (SCL_A##V * SCL_B##H);                         \
		r2 = SUM_A##H(w3[0], w4[1], w6[0], w7[1]) *        \
		    (SCL_B##V * SCL_A##H);                         \
		r3 = SUM_B##H(w3[0], w4[1], w6[0], w7[1]) *        \
		    (SCL_B##V * SCL_B##H);                         \
		MLIB_S_ZOOM_BC_LINE_STORE(STORE_UP, STORE_DN);     \
			                                           \
		pd = dlc + i * 8 * channels + 1;                   \
		r0 = SUM_A##H(v0[1], v1[1], v2[1],                 \
		    v3[1]) * (SCL_A##V * SCL_A##H);                \
		r1 = SUM_B##H(v0[1], v1[1], v2[1],                 \
		    v3[1]) * (SCL_A##V * SCL_B##H);                \
		r2 = SUM_A##H(w0[1], w1[1], w2[1],                 \
		    w3[1]) * (SCL_B##V * SCL_A##H);                \
		r3 = SUM_B##H(w0[1], w1[1], w2[1],                 \
		    w3[1]) * (SCL_B##V * SCL_B##H);                \
		MLIB_S_ZOOM_BC_LINE_STORE(STORE_UP, STORE_DN);     \
		pd += 2 * channels;                                \
		r0 = SUM_A##H(v1[1], v2[1], v3[1], v5[0]) *        \
		    (SCL_A##V * SCL_A##H);                         \
		r1 = SUM_B##H(v1[1], v2[1], v3[1], v5[0]) *        \
		    (SCL_A##V * SCL_B##H);                         \
		r2 = SUM_A##H(w1[1], w2[1], w3[1], w5[0]) *        \
		    (SCL_B##V * SCL_A##H);                         \
		r3 = SUM_B##H(w1[1], w2[1], w3[1], w5[0]) *        \
		    (SCL_B##V * SCL_B##H);                         \
		MLIB_S_ZOOM_BC_LINE_STORE(STORE_UP, STORE_DN);     \
		pd += 2 * channels;                                \
		r0 = SUM_A##H(v2[1], v3[1], v5[0], v6[1]) *        \
		    (SCL_A##V * SCL_A##H);                         \
		r1 = SUM_B##H(v2[1], v3[1], v5[0], v6[1]) *        \
		    (SCL_A##V * SCL_B##H);                         \
		r2 = SUM_A##H(w2[1], w3[1], w5[0], w6[1]) *        \
		    (SCL_B##V * SCL_A##H);                         \
		r3 = SUM_B##H(w2[1], w3[1], w5[0], w6[1]) *        \
		    (SCL_B##V * SCL_B##H);                         \
		MLIB_S_ZOOM_BC_LINE_STORE(STORE_UP, STORE_DN);     \
		pd += 2 * channels;                                \
		r0 = SUM_A##H(v3[1], v5[0], v6[1], v8[0]) *        \
		    (SCL_A##V * SCL_A##H);                         \
		r1 = SUM_B##H(v3[1], v5[0], v6[1], v8[0]) *        \
		    (SCL_A##V * SCL_B##H);                         \
		r2 = SUM_A##H(w3[1], w5[0], w6[1], w8[0]) *        \
		    (SCL_B##V * SCL_A##H);                         \
		r3 = SUM_B##H(w3[1], w5[0], w6[1], w8[0]) *        \
		    (SCL_B##V * SCL_B##H);                         \
		MLIB_S_ZOOM_BC_LINE_STORE(STORE_UP, STORE_DN);     \
		pd = dlc + i * 8 * channels + 2;                   \
		r0 = SUM_A##H(v0[2], v1[2], v2[2],                 \
		    v4[0]) * (SCL_A##V * SCL_A##H);                \
		r1 = SUM_B##H(v0[2], v1[2], v2[2],                 \
		    v4[0]) * (SCL_A##V * SCL_B##H);                \
		r2 = SUM_A##H(w0[2], w1[2], w2[2],                 \
		    w4[0]) * (SCL_B##V * SCL_A##H);                \
		r3 = SUM_B##H(w0[2], w1[2], w2[2],                 \
		    w4[0]) * (SCL_B##V * SCL_B##H);                \
		MLIB_S_ZOOM_BC_LINE_STORE(STORE_UP, STORE_DN);     \
		pd += 2 * channels;                                \
		r0 = SUM_A##H(v1[2], v2[2], v4[0], v5[1]) *        \
		    (SCL_A##V * SCL_A##H);                         \
		r1 = SUM_B##H(v1[2], v2[2], v4[0], v5[1]) *        \
		    (SCL_A##V * SCL_B##H);                         \
		r2 = SUM_A##H(w1[2], w2[2], w4[0], w5[1]) *        \
		    (SCL_B##V * SCL_A##H);                         \
		r3 = SUM_B##H(w1[2], w2[2], w4[0], w5[1]) *        \
		    (SCL_B##V * SCL_B##H);                         \
		MLIB_S_ZOOM_BC_LINE_STORE(STORE_UP, STORE_DN);     \
		pd += 2 * channels;                                \
		r0 = SUM_A##H(v2[2], v4[0], v5[1], v7[0]) *        \
		    (SCL_A##V * SCL_A##H);                         \
		r1 = SUM_B##H(v2[2], v4[0], v5[1], v7[0]) *        \
		    (SCL_A##V * SCL_B##H);                         \
		r2 = SUM_A##H(w2[2], w4[0], w5[1], w7[0]) *        \
		    (SCL_B##V * SCL_A##H);                         \
		r3 = SUM_B##H(w2[2], w4[0], w5[1], w7[0]) *        \
		    (SCL_B##V * SCL_B##H);                         \
		MLIB_S_ZOOM_BC_LINE_STORE(STORE_UP, STORE_DN);     \
		pd += 2 * channels;                                \
		r0 = SUM_A##H(v4[0], v5[1], v7[0], v8[1]) *        \
		    (SCL_A##V * SCL_A##H);                         \
		r1 = SUM_B##H(v4[0], v5[1], v7[0], v8[1]) *        \
		    (SCL_A##V * SCL_B##H);                         \
		r2 = SUM_A##H(w4[0], w5[1], w7[0], w8[1]) *        \
		    (SCL_B##V * SCL_A##H);                         \
		r3 = SUM_B##H(w4[0], w5[1], w7[0], w8[1]) *        \
		    (SCL_B##V * SCL_B##H);                         \
		MLIB_S_ZOOM_BC_LINE_STORE(STORE_UP, STORE_DN);     \
		pd++;                                              \
		v0[0] = v4[1];                                     \
		v0[1] = v5[0];                                     \
		v0[2] = v5[1];                                     \
		v1[0] = v6[0];                                     \
		v1[1] = v6[1];                                     \
		v1[2] = v7[0];                                     \
		v2[0] = v7[1];                                     \
		v2[1] = v8[0];                                     \
		v2[2] = v8[1];                                     \
		w0[0] = w4[1];                                     \
		w0[1] = w5[0];                                     \
		w0[2] = w5[1];                                     \
		w1[0] = w6[0];                                     \
		w1[1] = w6[1];                                     \
		w1[2] = w7[0];                                     \
		w2[0] = w7[1];                                     \
		w2[1] = w8[0];                                     \
		w2[2] = w8[1];                                     \
	}                                                          \
	for (k = 0; k < channels; k++) {                           \
		ps = slc + i * 4 * channels + k + 9;               \
		pd = dlc + i * 8 * channels + k;                   \
		for (int m = i * 4; m < (src_width / 2); m++) {    \
		v3[k] = SUM_A##V(ps[-src_stride], ps[0],           \
		    ps[src_stride], ps[2 * src_stride]);           \
		w3[k] = SUM_B##V(ps[-src_stride], ps[0],           \
		    ps[src_stride], ps[2 * src_stride]);           \
		ps += channels;                                    \
		r0 = SUM_A##H(v0[k], v1[k], v2[k],                 \
		    v3[k]) * (SCL_A##V * SCL_A##H);                \
		r1 = SUM_B##H(v0[k], v1[k], v2[k],                 \
		    v3[k]) * (SCL_A##V * SCL_B##H);                \
		r2 = SUM_A##H(w0[k], w1[k], w2[k],                 \
		    w3[k]) * (SCL_B##V * SCL_A##H);                \
		r3 = SUM_B##H(w0[k], w1[k], w2[k],                 \
		    w3[k]) * (SCL_B##V * SCL_B##H);                \
		MLIB_S_ZOOM_BC_LINE_STORE(STORE_UP, STORE_DN);     \
		pd += 2 * channels;                                \
		v0[k] = v1[k];                                     \
		v1[k] = v2[k];                                     \
		v2[k] = v3[k];                                     \
		w0[k] = w1[k];                                     \
		w1[k] = w2[k];                                     \
		w2[k] = w3[k];                                     \
	    }                                                      \
	    if (src_width & 1) {                                   \
		v3[k] = SUM_A##V(ps[-src_stride], ps[0],           \
		    ps[src_stride], ps[2 * src_stride]);           \
		w3[k] = SUM_B##V(ps[-src_stride], ps[0],           \
		    ps[src_stride], ps[2 * src_stride]);           \
		r0 = SUM_A##H(v0[k], v1[k], v2[k],                 \
		    v3[k]) * (SCL_A##V * SCL_A##H);                \
		r2 = SUM_A##H(w0[k], w1[k], w2[k],                 \
		    w3[k]) * (SCL_B##V * SCL_A##H);                \
		STORE_UP(pd[0], r0);                               \
		STORE_DN(pd[dst_stride], r2);                      \
	    }                                                      \
	}                                                          \
	                                                           \
	sl += src_stride;                                          \
	dl += 2 * dst_stride;                                      \
}

/* *********************************************************** */

#define	MLIB_S_ZOOM_BC_LINE_4(H, V, STORE_UP, STORE_DN) {          \
	TTYPE v0[4], v1[4], v2[4], v3[4];                          \
	TTYPE w0[4], w1[4], w2[4], w3[4];                          \
	__m128i x_sd0, x_sd1, x_sd2, x_sd3;                        \
	__m128d x_sd0_l, x_sd1_l, x_sd2_l, x_sd3_l;                \
	__m128d x_sd0_h, x_sd1_h, x_sd2_h, x_sd3_h;                \
	__m128d x_v3_l, x_v3_h, x_w3_l, x_w3_h;                    \
	__m128d x_v4_l, x_v4_h, x_w4_l, x_w4_h;                    \
	TTYPE *v3l, *v4l, *v3h, *v4h;                              \
	TTYPE *w3l, *w4l, *w3h, *w4h;                              \
	v3l = (TTYPE *)&x_v3_l;                                    \
	v3h = (TTYPE *)&x_v3_h;                                    \
	v4l = (TTYPE *)&x_v4_l;                                    \
	v4h = (TTYPE *)&x_v4_h;                                    \
	w3l = (TTYPE *)&x_w3_l;                                    \
	w3h = (TTYPE *)&x_w3_h;                                    \
	w4l = (TTYPE *)&x_w4_l;                                    \
	w4h = (TTYPE *)&x_w4_h;                                    \
	MLIB_S_ZOOM_BC_LINE_PRE(V);                                \
	MLIB_S_ZOOM_BC_LINE_H_S32(H, V, STORE_UP, STORE_DN);       \
	for (i = 0; i < (src_width / 4); i++) {                    \
		MLIB_S_MULCONST;                                   \
		MLIB_S_ZOOM_BC_LINE_LOAD(                          \
		    V, x_v3_l, x_v3_h, x_w3_l, x_w3_h);            \
		ps += channels;                                    \
		MLIB_S_ZOOM_BC_LINE_LOAD(                          \
		    V, x_v4_l, x_v4_h, x_w4_l, x_w4_h);            \
		pd = dlc + i * 4 * channels;                       \
		r0 = SUM_A##H(v0[0], v1[0], v2[0],                 \
		    v3l[0]) * (SCL_A##V * SCL_A##H);               \
		r1 = SUM_B##H(v0[0], v1[0], v2[0],                 \
		    v3l[0]) * (SCL_A##V * SCL_B##H);               \
		r2 = SUM_A##H(w0[0], w1[0], w2[0],                 \
		    w3l[0]) * (SCL_B##V * SCL_A##H);               \
		r3 = SUM_B##H(w0[0], w1[0], w2[0],                 \
		    w3l[0]) * (SCL_B##V * SCL_B##H);               \
		MLIB_S_ZOOM_BC_LINE_STORE(STORE_UP, STORE_DN);     \
		pd += 2 * channels;                                \
		r0 = SUM_A##H(v1[0], v2[0], v3l[0],                \
		    v4l[0]) * (SCL_A##V * SCL_A##H);               \
		r1 = SUM_B##H(v1[0], v2[0], v3l[0],                \
		    v4l[0]) * (SCL_A##V * SCL_B##H);               \
		r2 = SUM_A##H(w1[0], w2[0], w3l[0],                \
		    w4l[0]) * (SCL_B##V * SCL_A##H);               \
		r3 = SUM_B##H(w1[0], w2[0], w3l[0],                \
		    w4l[0]) * (SCL_B##V * SCL_B##H);               \
		MLIB_S_ZOOM_BC_LINE_STORE(STORE_UP, STORE_DN);     \
		pd = dlc + i * 4 * channels + 1;                   \
		r0 = SUM_A##H(v0[1], v1[1], v2[1],                 \
		    v3l[1]) * (SCL_A##V * SCL_A##H);               \
		r1 = SUM_B##H(v0[1], v1[1], v2[1],                 \
		    v3l[1]) * (SCL_A##V * SCL_B##H);               \
		r2 = SUM_A##H(w0[1], w1[1], w2[1],                 \
		    w3l[1]) * (SCL_B##V * SCL_A##H);               \
		r3 = SUM_B##H(w0[1], w1[1], w2[1],                 \
		    w3l[1]) * (SCL_B##V * SCL_B##H);               \
		MLIB_S_ZOOM_BC_LINE_STORE(STORE_UP, STORE_DN);     \
		pd += 2 * channels;                                \
		r0 = SUM_A##H(v1[1], v2[1], v3l[1],                \
		    v4l[1]) * (SCL_A##V * SCL_A##H);               \
		r1 = SUM_B##H(v1[1], v2[1], v3l[1],                \
		    v4l[1]) * (SCL_A##V * SCL_B##H);               \
		r2 = SUM_A##H(w1[1], w2[1], w3l[1],                \
		    w4l[1]) * (SCL_B##V * SCL_A##H);               \
		r3 = SUM_B##H(w1[1], w2[1], w3l[1],                \
		    w4l[1]) * (SCL_B##V * SCL_B##H);               \
		MLIB_S_ZOOM_BC_LINE_STORE(STORE_UP, STORE_DN);     \
		pd = dlc + i * 4 * channels + 2;                   \
		r0 = SUM_A##H(v0[2], v1[2], v2[2],                 \
		    v3h[0]) * (SCL_A##V * SCL_A##H);               \
		r1 = SUM_B##H(v0[2], v1[2], v2[2],                 \
		    v3h[0]) * (SCL_A##V * SCL_B##H);               \
		r2 = SUM_A##H(w0[2], w1[2], w2[2],                 \
		    w3h[0]) * (SCL_B##V * SCL_A##H);               \
		r3 = SUM_B##H(w0[2], w1[2], w2[2],                 \
		    w3h[0]) * (SCL_B##V * SCL_B##H);               \
		MLIB_S_ZOOM_BC_LINE_STORE(STORE_UP, STORE_DN);     \
		pd += 2 * channels;                                \
		r0 = SUM_A##H(v1[2], v2[2], v3h[0],                \
		    v4h[0]) * (SCL_A##V * SCL_A##H);               \
		r1 = SUM_B##H(v1[2], v2[2], v3h[0],                \
		    v4h[0]) * (SCL_A##V * SCL_B##H);               \
		r2 = SUM_A##H(w1[2], w2[2], w3h[0],                \
		    w4h[0]) * (SCL_B##V * SCL_A##H);               \
		r3 = SUM_B##H(w1[2], w2[2], w3h[0],                \
		    w4h[0]) * (SCL_B##V * SCL_B##H);               \
		MLIB_S_ZOOM_BC_LINE_STORE(STORE_UP, STORE_DN);     \
		pd = dlc + i * 4 * channels + 3;                   \
		r0 = SUM_A##H(v0[3], v1[3], v2[3],                 \
		    v3h[1]) * (SCL_A##V * SCL_A##H);               \
		r1 = SUM_B##H(v0[3], v1[3], v2[3],                 \
		    v3h[1]) * (SCL_A##V * SCL_B##H);               \
		r2 = SUM_A##H(w0[3], w1[3], w2[3],                 \
		    w3h[1]) * (SCL_B##V * SCL_A##H);               \
		r3 = SUM_B##H(w0[3], w1[3], w2[3],                 \
		    w3h[1]) * (SCL_B##V * SCL_B##H);               \
		MLIB_S_ZOOM_BC_LINE_STORE(STORE_UP, STORE_DN);     \
		pd += 2 * channels;                                \
		r0 = SUM_A##H(v1[3], v2[3], v3h[1],                \
		    v4h[1]) * (SCL_A##V * SCL_A##H);               \
		r1 = SUM_B##H(v1[3], v2[3], v3h[1],                \
		    v4h[1]) * (SCL_A##V * SCL_B##H);               \
		r2 = SUM_A##H(w1[3], w2[3], w3h[1],                \
		    w4h[1]) * (SCL_B##V * SCL_A##H);               \
		r3 = SUM_B##H(w1[3], w2[3], w3h[1],                \
		    w4h[1]) * (SCL_B##V * SCL_B##H);               \
		MLIB_S_ZOOM_BC_LINE_STORE(STORE_UP, STORE_DN);     \
		v0[0] = v2[0];                                     \
		v1[0] = v3l[0];                                    \
		v2[0] = v4l[0];                                    \
		w0[0] = w2[0];                                     \
		w1[0] = w3l[0];                                    \
		w2[0] = w4l[0];                                    \
		v0[1] = v2[1];                                     \
		v1[1] = v3l[1];                                    \
		v2[1] = v4l[1];                                    \
		w0[1] = w2[1];                                     \
		w1[1] = w3l[1];                                    \
		w2[1] = w4l[1];                                    \
		v0[2] = v2[2];                                     \
		v1[2] = v3h[0];                                    \
		v2[2] = v4h[0];                                    \
		w0[2] = w2[2];                                     \
		w1[2] = w3h[0];                                    \
		w2[2] = w4h[0];                                    \
		v0[3] = v2[3];                                     \
		v1[3] = v3h[1];                                    \
		v2[3] = v4h[1];                                    \
		w0[3] = w2[3];                                     \
		w1[3] = w3h[1];                                    \
		w2[3] = w4h[1];                                    \
		ps += channels;                                    \
	}                                                          \
	int t = i * 2;                                             \
	if (t < (src_width / 2)) {                                 \
		MLIB_S_MULCONST;                                   \
		MLIB_S_ZOOM_BC_LINE_LOAD(                          \
		    V, x_v3_l, x_v3_h, x_w3_l, x_w3_h);            \
		pd = dlc + i * 4 * channels;                       \
		r0 = SUM_A##H(v0[0], v1[0], v2[0],                 \
		    v3l[0]) * (SCL_A##V * SCL_A##H);               \
		r1 = SUM_B##H(v0[0], v1[0], v2[0],                 \
		    v3l[0]) * (SCL_A##V * SCL_B##H);               \
		r2 = SUM_A##H(w0[0], w1[0], w2[0],                 \
		    w3l[0]) * (SCL_B##V * SCL_A##H);               \
		r3 = SUM_B##H(w0[0], w1[0], w2[0],                 \
		    w3l[0]) * (SCL_B##V * SCL_B##H);               \
		MLIB_S_ZOOM_BC_LINE_STORE(STORE_UP, STORE_DN);     \
		pd = dlc + i * 4 * channels + 1;                   \
		r0 = SUM_A##H(v0[1], v1[1], v2[1],                 \
		    v3l[1]) * (SCL_A##V * SCL_A##H);               \
		r1 = SUM_B##H(v0[1], v1[1], v2[1],                 \
		    v3l[1]) * (SCL_A##V * SCL_B##H);               \
		r2 = SUM_A##H(w0[1], w1[1], w2[1],                 \
		    w3l[1]) * (SCL_B##V * SCL_A##H);               \
		r3 = SUM_B##H(w0[1], w1[1], w2[1],                 \
		    w3l[1]) * (SCL_B##V * SCL_B##H);               \
		MLIB_S_ZOOM_BC_LINE_STORE(STORE_UP, STORE_DN);     \
		pd = dlc + i * 4 * channels + 2;                   \
		r0 = SUM_A##H(v0[2], v1[2], v2[2],                 \
		    v3h[0]) * (SCL_A##V * SCL_A##H);               \
		r1 = SUM_B##H(v0[2], v1[2], v2[2],                 \
		    v3h[0]) * (SCL_A##V * SCL_B##H);               \
		r2 = SUM_A##H(w0[2], w1[2], w2[2],                 \
		    w3h[0]) * (SCL_B##V * SCL_A##H);               \
		r3 = SUM_B##H(w0[2], w1[2], w2[2],                 \
		    w3h[0]) * (SCL_B##V * SCL_B##H);               \
		MLIB_S_ZOOM_BC_LINE_STORE(STORE_UP, STORE_DN);     \
		pd = dlc + i * 4 * channels + 3;                   \
		r0 = SUM_A##H(v0[3], v1[3], v2[3],                 \
		    v3h[1]) * (SCL_A##V * SCL_A##H);               \
		r1 = SUM_B##H(v0[3], v1[3], v2[3],                 \
		    v3h[1]) * (SCL_A##V * SCL_B##H);               \
		r2 = SUM_A##H(w0[3], w1[3], w2[3],                 \
		    w3h[1]) * (SCL_B##V * SCL_A##H);               \
		r3 = SUM_B##H(w0[3], w1[3], w2[3],                 \
		    w3h[1]) * (SCL_B##V * SCL_B##H);               \
		MLIB_S_ZOOM_BC_LINE_STORE(STORE_UP, STORE_DN);     \
		v0[0] = v1[0];                                     \
		v1[0] = v2[0];                                     \
		v2[0] = v3l[0];                                    \
		w0[0] = w1[0];                                     \
		w1[0] = w2[0];                                     \
		w2[0] = w3l[0];                                    \
		v0[1] = v1[1];                                     \
		v1[1] = v2[1];                                     \
		v2[1] = v3l[1];                                    \
		w0[1] = w1[1];                                     \
		w1[1] = w2[1];                                     \
		w2[1] = w3l[1];                                    \
		v0[2] = v1[2];                                     \
		v1[2] = v2[2];                                     \
		v2[2] = v3h[0];                                    \
		w0[2] = w1[2];                                     \
		w1[2] = w2[2];                                     \
		w2[2] = w3h[0];                                    \
		v0[3] = v1[3];                                     \
		v1[3] = v2[3];                                     \
		v2[3] = v3h[1];                                    \
		w0[3] = w1[3];                                     \
		w1[3] = w2[3];                                     \
		w2[3] = w3h[1];                                    \
		ps += channels;                                    \
		t++;                                               \
	}                                                          \
	for (k = 0; k < channels; k++) {                           \
		ps = slc + t * channels + k + 12;                  \
		pd = dlc + t * 2 * channels + k;                   \
		if (src_width & 1) {                               \
		v3[k] = SUM_A##V(ps[-src_stride], ps[0],           \
		    ps[src_stride], ps[2 * src_stride]);           \
		w3[k] = SUM_B##V(ps[-src_stride], ps[0],           \
		    ps[src_stride], ps[2 * src_stride]);           \
		r0 = SUM_A##H(v0[k], v1[k], v2[k],                 \
		    v3[k]) * (SCL_A##V * SCL_A##H);                \
		r2 = SUM_A##H(w0[k], w1[k], w2[k],                 \
		    w3[k]) * (SCL_B##V * SCL_A##H);                \
		STORE_UP(pd[0], r0);                               \
		STORE_DN(pd[dst_stride], r2);                      \
	    }                                                      \
	}                                                          \
	                                                           \
	sl += src_stride;                                          \
	dl += 2 * dst_stride;                                      \
}

/* *********************************************************** */

#define	MLIB_S_ZOOM_BC_LINE(H, V, STORE_UP, STORE_DN)                \
	switch (channels) {                                          \
	case 1:                                                      \
		MLIB_S_ZOOM_BC_LINE_1(H, V, STORE_UP, STORE_DN);     \
		break;                                               \
	case 2:                                                      \
		MLIB_S_ZOOM_BC_LINE_2(H, V, STORE_UP, STORE_DN);     \
		break;                                               \
	case 3:                                                      \
		MLIB_S_ZOOM_BC_LINE_3(H, V, STORE_UP, STORE_DN);     \
		break;                                               \
	case 4:                                                      \
		MLIB_S_ZOOM_BC_LINE_4(H, V, STORE_UP, STORE_DN);     \
		break;                                               \
	}

/* *********************************************************** */

#define	MLIB_S_FUNC_ZOOM_BC(TT, STYPE, N, H, V)                 \
	void                                                    \
	mlib_ImageZoomIn2X_##TT##_BC_FL##N(STYPE * pdst,        \
		const STYPE * psrc, mlib_s32 src_width,         \
		mlib_s32 src_height, mlib_s32 dst_stride,       \
		mlib_s32 src_stride, mlib_s32 src_w_beg,        \
		mlib_s32 src_h_beg, mlib_s32 channels)          \
	{                                                       \
	    mlib_s32 i, j, k;                                   \
	    STYPE *pd;                                          \
	    const STYPE *ps, *slc, *sl = psrc - channels;       \
	    STYPE *dlc, *dl = pdst;                             \
	    TTYPE r0, r1, r2, r3;                               \
	                                                        \
	    src_w_beg &= 1;                                     \
	    src_h_beg &= 1;                                     \
	    src_width -= src_w_beg;                             \
	    src_height -= src_h_beg;                            \
	    if (src_h_beg) {                                    \
		dl -= dst_stride;                               \
		MLIB_S_ZOOM_BC_LINE(H, V, SKIP, STORE_##TT);    \
	    }                                                   \
	    for (j = 0; j < (src_height / 2); j++) {            \
		MLIB_S_ZOOM_BC_LINE(H, V, STORE_##TT,           \
		    STORE_##TT)                                 \
	    }                                                   \
	    if (src_height & 1) {                               \
		MLIB_S_ZOOM_BC_LINE(H, V, STORE_##TT, SKIP);    \
	    }                                                   \
	}

/* *********************************************************** */

#define	TTYPE	mlib_d64

MLIB_S_FUNC_ZOOM_BC(
	S32,
	mlib_s32,
	0,
	0,
	0)
MLIB_S_FUNC_ZOOM_BC(
	S32,
	mlib_s32,
	1,
	0,
	1)
MLIB_S_FUNC_ZOOM_BC(
	S32,
	mlib_s32,
	2,
	1,
	0)
MLIB_S_FUNC_ZOOM_BC(
	S32,
	mlib_s32,
	3,
	1,
	1)

/* *********************************************************** */
