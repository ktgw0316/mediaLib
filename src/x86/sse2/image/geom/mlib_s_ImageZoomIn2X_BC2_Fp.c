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

#pragma ident	"@(#)mlib_s_ImageZoomIn2X_BC2_Fp.c	9.3	07/11/05 SMI"

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

#define	MLIB_S_MULCONST_F32                        \
	__m128 x_a, x_b, x_c, x_d;                 \
	const __m128 x_mul3 = _mm_set1_ps(3.0);    \
	const __m128 x_mul5 = _mm_set1_ps(5.0);    \
	const __m128 x_mul9 = _mm_set1_ps(9.0);    \
	const __m128 x_mul19 = _mm_set1_ps(19.0);  \
	const __m128 x_mul57 = _mm_set1_ps(57.0)

#define	MLIB_S_MULCONST_D64                         \
	__m128d x_a, x_b, x_c, x_d;                 \
	const __m128d x_mul3 = _mm_set1_pd(3.0);    \
	const __m128d x_mul5 = _mm_set1_pd(5.0);    \
	const __m128d x_mul9 = _mm_set1_pd(9.0);    \
	const __m128d x_mul19 = _mm_set1_pd(19.0);  \
	const __m128d x_mul57 = _mm_set1_pd(57.0)

#define	MLIB_S_SUM_A0_F32(a, b, c, d, e)     \
	x_a = _mm_mul_ps(a, x_mul9);         \
	x_b = _mm_mul_ps(b, x_mul57);        \
	x_c = _mm_mul_ps(c, x_mul19);        \
	x_d = _mm_mul_ps(d, x_mul3);         \
	e = _mm_add_ps(_mm_sub_ps(x_b, x_a), \
	    _mm_sub_ps(x_c, x_d))

#define	MLIB_S_SUM_A0_D64(a, b, c, d, e)     \
	x_a = _mm_mul_pd(a, x_mul9);         \
	x_b = _mm_mul_pd(b, x_mul57);        \
	x_c = _mm_mul_pd(c, x_mul19);        \
	x_d = _mm_mul_pd(d, x_mul3);         \
	e = _mm_add_pd(_mm_sub_pd(x_b, x_a), \
	    _mm_sub_pd(x_c, x_d))

#define	MLIB_S_SUM_B0_F32(a, b, c, d, e)     \
	x_a = _mm_mul_ps(a, x_mul3);         \
	x_b = _mm_mul_ps(b, x_mul19);        \
	x_c = _mm_mul_ps(c, x_mul57);        \
	x_d = _mm_mul_ps(d, x_mul9);         \
	e = _mm_add_ps(_mm_sub_ps(x_b, x_a), \
	    _mm_sub_ps(x_c, x_d))

#define	MLIB_S_SUM_B0_D64(a, b, c, d, e)     \
	x_a = _mm_mul_pd(a, x_mul3);         \
	x_b = _mm_mul_pd(b, x_mul19);        \
	x_c = _mm_mul_pd(c, x_mul57);        \
	x_d = _mm_mul_pd(d, x_mul9);         \
	e = _mm_add_pd(_mm_sub_pd(x_b, x_a), \
	    _mm_sub_pd(x_c, x_d))

#define	SUM_A0(a, b, c, d)	((TTYPE)57*b - (TTYPE)9*a + \
				(TTYPE)19*c - (TTYPE)3*d)
#define	SUM_B0(a, b, c, d)	((TTYPE)19*b - (TTYPE)3*a + \
				(TTYPE)57*c - (TTYPE)9*d)

#define	SCL_A0	(TTYPE)0.015625

#define	SCL_B0	(TTYPE)0.015625

/* *********************************************************** */

#define	MLIB_S_SUM_A1_F32(a, b, c, d, e)	\
	e = b

#define	MLIB_S_SUM_A1_D64(a, b, c, d, e)	\
	e = b

#define	MLIB_S_SUM_B1_F32(a, b, c, d, e)            \
	x_b = _mm_mul_ps(x_mul5, _mm_add_ps(b, c)); \
	e = _mm_sub_ps(x_b, _mm_add_ps(a, d))

#define	MLIB_S_SUM_B1_D64(a, b, c, d, e)            \
	x_b = _mm_mul_pd(x_mul5, _mm_add_pd(b, c)); \
	e = _mm_sub_pd(x_b, _mm_add_pd(a, d))

#define	SUM_A1(a, b, c, d)	((TTYPE)b)
#define	SUM_B1(a, b, c, d)	((TTYPE)5*b + (TTYPE)5*c - a - d)

#define	SCL_A1	(TTYPE)1.0

#define	SCL_B1	(TTYPE)0.125

/* *********************************************************** */

#define	MLIB_S_ZOOM_BC_LINE_LOAD_F32(V, XV, XW)                  \
	x_sd0 = _mm_loadu_ps(ps - src_stride);                   \
	x_sd1 = _mm_loadu_ps(ps);                                \
	x_sd2 = _mm_loadu_ps(ps + src_stride);                   \
	x_sd3 = _mm_loadu_ps(ps + 2 * src_stride);               \
	MLIB_S_SUM_A##V##_F32(x_sd0, x_sd1, x_sd2, x_sd3, XV);   \
	MLIB_S_SUM_B##V##_F32(x_sd0, x_sd1, x_sd2, x_sd3, XW)

/* *********************************************************** */

#define	MLIB_S_ZOOM_BC_LINE_LOAD_D64(V, VL, WL, VH, WH)                    \
	x_sd0_l = _mm_loadu_pd(ps - src_stride);                           \
	x_sd1_l = _mm_loadu_pd(ps);                                        \
	x_sd2_l = _mm_loadu_pd(ps + src_stride);                           \
	x_sd3_l = _mm_loadu_pd(ps + 2 * src_stride);                       \
	ps += 2;                                                           \
	x_sd0_h = _mm_loadu_pd(ps - src_stride);                           \
	x_sd1_h = _mm_loadu_pd(ps);                                        \
	x_sd2_h = _mm_loadu_pd(ps + src_stride);                           \
	x_sd3_h = _mm_loadu_pd(ps + 2 * src_stride);                       \
	MLIB_S_SUM_A##V##_D64(x_sd0_l, x_sd1_l, x_sd2_l, x_sd3_l, VL);     \
	MLIB_S_SUM_B##V##_D64(x_sd0_l, x_sd1_l, x_sd2_l, x_sd3_l, WL);     \
	MLIB_S_SUM_A##V##_D64(x_sd0_h, x_sd1_h, x_sd2_h, x_sd3_h, VH);     \
	MLIB_S_SUM_B##V##_D64(x_sd0_h, x_sd1_h, x_sd2_h, x_sd3_h, WH)

/* *********************************************************** */

#define	MLIB_S_ZOOM_BC_LINE_PRE(V)                           \
slc = sl;                                                    \
dlc = dl;                                                    \
for (k = 0; k < channels; k++) {                             \
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
}                                                            \

/* *********************************************************** */

#define	MLIB_S_ZOOM_BC_LINE_STORE(STORE_UP, STORE_DN)        \
	STORE_UP(pd[0], r0);                                 \
	STORE_UP(pd[channels], r1);                          \
	STORE_DN(pd[dst_stride], r2);                        \
	STORE_DN(pd[dst_stride + channels], r3)

/* *********************************************************** */

#define	MLIB_S_ZOOM_BC_LINE_F32_1(H, V, STORE_UP, STORE_DN) {  \
	TTYPE v0, v1, v2, v3s, w0, w1, w2, w3s;            \
	slc = sl;                                          \
	dlc = dl;                                          \
	ps = slc;                                          \
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
	pd = dlc;                                          \
	if (src_w_beg) {                                   \
	v3s = SUM_A##V(ps[-src_stride], ps[0],             \
	    ps[src_stride], ps[2 * src_stride]);           \
	w3s = SUM_B##V(ps[-src_stride], ps[0],             \
	    ps[src_stride], ps[2 * src_stride]);           \
	ps++;                                              \
	r1 = SUM_B##H(v0, v1, v2,                          \
	    v3s) * (SCL_A##V * SCL_B##H);                  \
	r3 = SUM_B##H(w0, w1, w2,                          \
	    w3s) * (SCL_B##V * SCL_B##H);                  \
	STORE_UP(pd[0], r1);                               \
	STORE_DN(pd[dst_stride], r3);                      \
	pd++;                                              \
	v0 = v1;                                           \
	v1 = v2;                                           \
	v2 = v3s;                                          \
	w0 = w1;                                           \
	w1 = w2;                                           \
	w2 = w3s;                                          \
	}                                                  \
	__m128 x_sd0, x_sd1, x_sd2, x_sd3;                 \
	__m128 x_v3, x_w3, x_v4, x_w4;                     \
	TTYPE *v3, *w3, *v4, *w4;                          \
	v3 = (TTYPE *)&x_v3;                               \
	w3 = (TTYPE *)&x_w3;                               \
	v4 = (TTYPE *)&x_v4;                               \
	w4 = (TTYPE *)&x_w4;                               \
	for (i = 0; i < (src_width / 2 - 7); i += 8) {     \
	MLIB_S_MULCONST_F32;                               \
	MLIB_S_ZOOM_BC_LINE_LOAD_F32(V, x_v3, x_w3);       \
	ps += 4;                                           \
	MLIB_S_ZOOM_BC_LINE_LOAD_F32(V, x_v4, x_w4);       \
	ps += 4;                                           \
	r0 = SUM_A##H(v0, v1, v2, v3[0]) *                 \
	    (SCL_A##V * SCL_A##H);                         \
	r1 = SUM_B##H(v0, v1, v2, v3[0]) *                 \
	    (SCL_A##V * SCL_B##H);                         \
	r2 = SUM_A##H(w0, w1, w2, w3[0]) *                 \
	    (SCL_B##V * SCL_A##H);                         \
	r3 = SUM_B##H(w0, w1, w2, w3[0]) *                 \
	    (SCL_B##V * SCL_B##H);                         \
	MLIB_S_ZOOM_BC_LINE_STORE(STORE_UP, STORE_DN);     \
	pd += 2 * channels;                                \
	r0 = SUM_A##H(v1, v2, v3[0], v3[1]) *              \
	    (SCL_A##V * SCL_A##H);                         \
	r1 = SUM_B##H(v1, v2, v3[0], v3[1]) *              \
	    (SCL_A##V * SCL_B##H);                         \
	r2 = SUM_A##H(w1, w2, w3[0], w3[1]) *              \
	    (SCL_B##V * SCL_A##H);                         \
	r3 = SUM_B##H(w1, w2, w3[0], w3[1]) *              \
	    (SCL_B##V * SCL_B##H);                         \
	MLIB_S_ZOOM_BC_LINE_STORE(STORE_UP, STORE_DN);     \
	pd += 2 * channels;                                \
	r0 = SUM_A##H(v2, v3[0], v3[1], v3[2]) *           \
	    (SCL_A##V * SCL_A##H);                         \
	r1 = SUM_B##H(v2, v3[0], v3[1], v3[2]) *           \
	    (SCL_A##V * SCL_B##H);                         \
	r2 = SUM_A##H(w2, w3[0], w3[1], w3[2]) *           \
	    (SCL_B##V * SCL_A##H);                         \
	r3 = SUM_B##H(w2, w3[0], w3[1], w3[2]) *           \
	    (SCL_B##V * SCL_B##H);                         \
	MLIB_S_ZOOM_BC_LINE_STORE(STORE_UP, STORE_DN);     \
	pd += 2 * channels;                                \
	r0 = SUM_A##H(v3[0], v3[1], v3[2],                 \
	    v3[3]) * (SCL_A##V * SCL_A##H);                \
	r1 = SUM_B##H(v3[0], v3[1], v3[2],                 \
	    v3[3]) * (SCL_A##V * SCL_B##H);                \
	r2 = SUM_A##H(w3[0], w3[1], w3[2],                 \
	    w3[3]) * (SCL_B##V * SCL_A##H);                \
	r3 = SUM_B##H(w3[0], w3[1], w3[2],                 \
	    w3[3]) * (SCL_B##V * SCL_B##H);                \
	MLIB_S_ZOOM_BC_LINE_STORE(STORE_UP, STORE_DN);     \
	pd += 2 * channels;                                \
	r0 = SUM_A##H(v3[1], v3[2], v3[3],                 \
	    v4[0]) * (SCL_A##V * SCL_A##H);                \
	r1 = SUM_B##H(v3[1], v3[2], v3[3],                 \
	    v4[0]) * (SCL_A##V * SCL_B##H);                \
	r2 = SUM_A##H(w3[1], w3[2], w3[3],                 \
	    w4[0]) * (SCL_B##V * SCL_A##H);                \
	r3 = SUM_B##H(w3[1], w3[2], w3[3],                 \
	    w4[0]) * (SCL_B##V * SCL_B##H);                \
	MLIB_S_ZOOM_BC_LINE_STORE(STORE_UP, STORE_DN);     \
	pd += 2 * channels;                                \
	r0 = SUM_A##H(v3[2], v3[3], v4[0],                 \
	    v4[1]) * (SCL_A##V * SCL_A##H);                \
	r1 = SUM_B##H(v3[2], v3[3], v4[0],                 \
	    v4[1]) * (SCL_A##V * SCL_B##H);                \
	r2 = SUM_A##H(w3[2], w3[3], w4[0],                 \
	    w4[1]) * (SCL_B##V * SCL_A##H);                \
	r3 = SUM_B##H(w3[2], w3[3], w4[0],                 \
	    w4[1]) * (SCL_B##V * SCL_B##H);                \
	MLIB_S_ZOOM_BC_LINE_STORE(STORE_UP, STORE_DN);     \
	pd += 2 * channels;                                \
	r0 = SUM_A##H(v3[3], v4[0], v4[1],                 \
	    v4[2]) * (SCL_A##V * SCL_A##H);                \
	r1 = SUM_B##H(v3[3], v4[0], v4[1],                 \
	    v4[2]) * (SCL_A##V * SCL_B##H);                \
	r2 = SUM_A##H(w3[3], w4[0], w4[1],                 \
	    w4[2]) * (SCL_B##V * SCL_A##H);                \
	r3 = SUM_B##H(w3[3], w4[0], w4[1],                 \
	    w4[2]) * (SCL_B##V * SCL_B##H);                \
	MLIB_S_ZOOM_BC_LINE_STORE(STORE_UP, STORE_DN);     \
	pd += 2 * channels;                                \
	r0 = SUM_A##H(v4[0], v4[1], v4[2],                 \
	    v4[3]) * (SCL_A##V * SCL_A##H);                \
	r1 = SUM_B##H(v4[0], v4[1], v4[2],                 \
	    v4[3]) * (SCL_A##V * SCL_B##H);                \
	r2 = SUM_A##H(w4[0], w4[1], w4[2],                 \
	    w4[3]) * (SCL_B##V * SCL_A##H);                \
	r3 = SUM_B##H(w4[0], w4[1], w4[2],                 \
	    w4[3]) * (SCL_B##V * SCL_B##H);                \
	MLIB_S_ZOOM_BC_LINE_STORE(STORE_UP, STORE_DN);     \
	pd += 2 * channels;                                \
	v0 = v4[1];                                        \
	v1 = v4[2];                                        \
	v2 = v4[3];                                        \
	w0 = w4[1];                                        \
	w1 = w4[2];                                        \
	w2 = w4[3];                                        \
	}                                                  \
	for (; i < (src_width / 2); i++) {                 \
	v3s = SUM_A##V(ps[-src_stride], ps[0],             \
	    ps[src_stride], ps[2 * src_stride]);           \
	w3s = SUM_B##V(ps[-src_stride], ps[0],             \
	    ps[src_stride], ps[2 * src_stride]);           \
	ps += channels;                                    \
	r0 = SUM_A##H(v0, v1, v2,                          \
	    v3s) * (SCL_A##V * SCL_A##H);                  \
	r1 = SUM_B##H(v0, v1, v2,                          \
	    v3s) * (SCL_A##V * SCL_B##H);                  \
	r2 = SUM_A##H(w0, w1, w2,                          \
	    w3s) * (SCL_B##V * SCL_A##H);                  \
	r3 = SUM_B##H(w0, w1, w2,                          \
	    w3s) * (SCL_B##V * SCL_B##H);                  \
	MLIB_S_ZOOM_BC_LINE_STORE(STORE_UP, STORE_DN);     \
	pd += 2 * channels;                                \
	v0 = v1;                                           \
	v1 = v2;                                           \
	v2 = v3s;                                          \
	w0 = w1;                                           \
	w1 = w2;                                           \
	w2 = w3s;                                          \
	}                                                  \
	if (src_width & 1) {                               \
	v3s = SUM_A##V(ps[-src_stride], ps[0],             \
	    ps[src_stride], ps[2 * src_stride]);           \
	w3s = SUM_B##V(ps[-src_stride], ps[0],             \
	    ps[src_stride], ps[2 * src_stride]);           \
	r0 = SUM_A##H(v0, v1, v2,                          \
	    v3s) * (SCL_A##V * SCL_A##H);                  \
	r2 = SUM_A##H(w0, w1, w2,                          \
	    w3s) * (SCL_B##V * SCL_A##H);                  \
	STORE_UP(pd[0], r0);                               \
	STORE_DN(pd[dst_stride], r2);                      \
	}                                                  \
                                                           \
	sl += src_stride;                                  \
	dl += 2 * dst_stride;                              \
}

/* *********************************************************** */

#define	MLIB_S_ZOOM_BC_LINE_D64_1(H, V, STORE_UP, STORE_DN) {  \
	TTYPE v0, v1, v2, v3, w0, w1, w2, w3;              \
	slc = sl;                                          \
	dlc = dl;                                          \
	ps = slc;                                          \
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
	pd = dlc;                                          \
	if (src_w_beg) {                                   \
	v3 = SUM_A##V(ps[-src_stride], ps[0],              \
	    ps[src_stride], ps[2 * src_stride]);           \
	w3 = SUM_B##V(ps[-src_stride], ps[0],              \
	    ps[src_stride], ps[2 * src_stride]);           \
	r1 = SUM_B##H(v0, v1, v2,                          \
	    v3) * (SCL_A##V * SCL_B##H);                   \
	r3 = SUM_B##H(w0, w1, w2,                          \
	    w3) * (SCL_B##V * SCL_B##H);                   \
	STORE_UP(pd[0], r1);                               \
	STORE_DN(pd[dst_stride], r3);                      \
	ps++;                                              \
	pd++;                                              \
	v0 = v1;                                           \
	v1 = v2;                                           \
	v2 = v3;                                           \
	w0 = w1;                                           \
	w1 = w2;                                           \
	w2 = w3;                                           \
	}                                                  \
	__m128d x_sd0_l, x_sd1_l, x_sd2_l, x_sd3_l;        \
	__m128d x_sd0_h, x_sd1_h, x_sd2_h, x_sd3_h;        \
	__m128d x_v3_l, x_v3_h, x_w3_l, x_w3_h;            \
	__m128d x_v4_l, x_v4_h, x_w4_l, x_w4_h;            \
	TTYPE *tv3l, *tv3h, *tw3l, *tw3h;                  \
	TTYPE *tv4l, *tv4h, *tw4l, *tw4h;                  \
	tv3l = (TTYPE *)&x_v3_l;                           \
	tw3l = (TTYPE *)&x_w3_l;                           \
	tv3h = (TTYPE *)&x_v3_h;                           \
	tw3h = (TTYPE *)&x_w3_h;                           \
	tv4l = (TTYPE *)&x_v4_l;                           \
	tw4l = (TTYPE *)&x_w4_l;                           \
	tv4h = (TTYPE *)&x_v4_h;                           \
	tw4h = (TTYPE *)&x_w4_h;                           \
	for (i = 0; i < (src_width / 2 - 7); i += 8) {     \
	MLIB_S_MULCONST_D64;                               \
	MLIB_S_ZOOM_BC_LINE_LOAD_D64(                      \
	    V, x_v3_l, x_w3_l, x_v3_h, x_w3_h);            \
	ps += 2;                                           \
	MLIB_S_ZOOM_BC_LINE_LOAD_D64(                      \
	    V, x_v4_l, x_w4_l, x_v4_h, x_w4_h);            \
	ps += 2;                                           \
	r0 = SUM_A##H(v0, v1, v2, tv3l[0]) *               \
	    (SCL_A##V * SCL_A##H);                         \
	r1 = SUM_B##H(v0, v1, v2, tv3l[0]) *               \
	    (SCL_A##V * SCL_B##H);                         \
	r2 = SUM_A##H(w0, w1, w2, tw3l[0]) *               \
	    (SCL_B##V * SCL_A##H);                         \
	r3 = SUM_B##H(w0, w1, w2, tw3l[0]) *               \
	    (SCL_B##V * SCL_B##H);                         \
	MLIB_S_ZOOM_BC_LINE_STORE(STORE_UP, STORE_DN);     \
	pd += 2 * channels;                                \
	r0 = SUM_A##H(v1, v2, tv3l[0], tv3l[1]) *          \
	    (SCL_A##V * SCL_A##H);                         \
	r1 = SUM_B##H(v1, v2, tv3l[0], tv3l[1]) *          \
	    (SCL_A##V * SCL_B##H);                         \
	r2 = SUM_A##H(w1, w2, tw3l[0], tw3l[1]) *          \
	    (SCL_B##V * SCL_A##H);                         \
	r3 = SUM_B##H(w1, w2, tw3l[0], tw3l[1]) *          \
	    (SCL_B##V * SCL_B##H);                         \
	MLIB_S_ZOOM_BC_LINE_STORE(STORE_UP, STORE_DN);     \
	pd += 2 * channels;                                \
	r0 = SUM_A##H(v2, tv3l[0], tv3l[1], tv3h[0]) *     \
	    (SCL_A##V * SCL_A##H);                         \
	r1 = SUM_B##H(v2, tv3l[0], tv3l[1], tv3h[0]) *     \
	    (SCL_A##V * SCL_B##H);                         \
	r2 = SUM_A##H(w2, tw3l[0], tw3l[1], tw3h[0]) *     \
	    (SCL_B##V * SCL_A##H);                         \
	r3 = SUM_B##H(w2, tw3l[0], tw3l[1], tw3h[0]) *     \
	    (SCL_B##V * SCL_B##H);                         \
	MLIB_S_ZOOM_BC_LINE_STORE(STORE_UP, STORE_DN);     \
	pd += 2 * channels;                                \
	r0 = SUM_A##H(tv3l[0], tv3l[1], tv3h[0],           \
	    tv3h[1]) * (SCL_A##V * SCL_A##H);              \
	r1 = SUM_B##H(tv3l[0], tv3l[1], tv3h[0],           \
	    tv3h[1]) * (SCL_A##V * SCL_B##H);              \
	r2 = SUM_A##H(tw3l[0], tw3l[1], tw3h[0],           \
	    tw3h[1]) * (SCL_B##V * SCL_A##H);              \
	r3 = SUM_B##H(tw3l[0], tw3l[1], tw3h[0],           \
	    tw3h[1]) * (SCL_B##V * SCL_B##H);              \
	MLIB_S_ZOOM_BC_LINE_STORE(STORE_UP, STORE_DN);     \
	pd += 2 * channels;                                \
	r0 = SUM_A##H(tv3l[1], tv3h[0], tv3h[1],           \
	    tv4l[0]) * (SCL_A##V * SCL_A##H);              \
	r1 = SUM_B##H(tv3l[1], tv3h[0], tv3h[1],           \
	    tv4l[0]) * (SCL_A##V * SCL_B##H);              \
	r2 = SUM_A##H(tw3l[1], tw3h[0], tw3h[1],           \
	    tw4l[0]) * (SCL_B##V * SCL_A##H);              \
	r3 = SUM_B##H(tw3l[1], tw3h[0], tw3h[1],           \
	    tw4l[0]) * (SCL_B##V * SCL_B##H);              \
	MLIB_S_ZOOM_BC_LINE_STORE(STORE_UP, STORE_DN);     \
	pd += 2 * channels;                                \
	r0 = SUM_A##H(tv3h[0], tv3h[1], tv4l[0],           \
	    tv4l[1]) * (SCL_A##V * SCL_A##H);              \
	r1 = SUM_B##H(tv3h[0], tv3h[1], tv4l[0],           \
	    tv4l[1]) * (SCL_A##V * SCL_B##H);              \
	r2 = SUM_A##H(tw3h[0], tw3h[1], tw4l[0],           \
	    tw4l[1]) * (SCL_B##V * SCL_A##H);              \
	r3 = SUM_B##H(tw3h[0], tw3h[1], tw4l[0],           \
	    tw4l[1]) * (SCL_B##V * SCL_B##H);              \
	MLIB_S_ZOOM_BC_LINE_STORE(STORE_UP, STORE_DN);     \
	pd += 2 * channels;                                \
	r0 = SUM_A##H(tv3h[1], tv4l[0], tv4l[1],           \
	    tv4h[0]) * (SCL_A##V * SCL_A##H);              \
	r1 = SUM_B##H(tv3h[1], tv4l[0], tv4l[1],           \
	    tv4h[0]) * (SCL_A##V * SCL_B##H);              \
	r2 = SUM_A##H(tw3h[1], tw4l[0], tw4l[1],           \
	    tw4h[0]) * (SCL_B##V * SCL_A##H);              \
	r3 = SUM_B##H(tw3h[1], tw4l[0], tw4l[1],           \
	    tw4h[0]) * (SCL_B##V * SCL_B##H);              \
	MLIB_S_ZOOM_BC_LINE_STORE(STORE_UP, STORE_DN);     \
	pd += 2 * channels;                                \
	r0 = SUM_A##H(tv4l[0], tv4l[1], tv4h[0],           \
	    tv4h[1]) * (SCL_A##V * SCL_A##H);              \
	r1 = SUM_B##H(tv4l[0], tv4l[1], tv4h[0],           \
	    tv4h[1]) * (SCL_A##V * SCL_B##H);              \
	r2 = SUM_A##H(tw4l[0], tw4l[1], tw4h[0],           \
	    tw4h[1]) * (SCL_B##V * SCL_A##H);              \
	r3 = SUM_B##H(tw4l[0], tw4l[1], tw4h[0],           \
	    tw4h[1]) * (SCL_B##V * SCL_B##H);              \
	MLIB_S_ZOOM_BC_LINE_STORE(STORE_UP, STORE_DN);     \
	pd += 2 * channels;                                \
	v0 = tv4l[1];                                      \
	v1 = tv4h[0];                                      \
	v2 = tv4h[1];                                      \
	w0 = tw4l[1];                                      \
	w1 = tw4h[0];                                      \
	w2 = tw4h[1];                                      \
	}                                                  \
	if (i < (src_width / 2 - 3)) {                     \
	MLIB_S_MULCONST_D64;                               \
	MLIB_S_ZOOM_BC_LINE_LOAD_D64(                      \
	    V, x_v3_l, x_w3_l, x_v3_h, x_w3_h);            \
	r0 = SUM_A##H(v0, v1, v2, tv3l[0]) *               \
	    (SCL_A##V * SCL_A##H);                         \
	r1 = SUM_B##H(v0, v1, v2, tv3l[0]) *               \
	    (SCL_A##V * SCL_B##H);                         \
	r2 = SUM_A##H(w0, w1, w2, tw3l[0]) *               \
	    (SCL_B##V * SCL_A##H);                         \
	r3 = SUM_B##H(w0, w1, w2, tw3l[0]) *               \
	    (SCL_B##V * SCL_B##H);                         \
	MLIB_S_ZOOM_BC_LINE_STORE(STORE_UP, STORE_DN);     \
	pd += 2 * channels;                                \
	r0 = SUM_A##H(v1, v2, tv3l[0], tv3l[1]) *          \
	    (SCL_A##V * SCL_A##H);                         \
	r1 = SUM_B##H(v1, v2, tv3l[0], tv3l[1]) *          \
	    (SCL_A##V * SCL_B##H);                         \
	r2 = SUM_A##H(w1, w2, tw3l[0], tw3l[1]) *          \
	    (SCL_B##V * SCL_A##H);                         \
	r3 = SUM_B##H(w1, w2, tw3l[0], tw3l[1]) *          \
	    (SCL_B##V * SCL_B##H);                         \
	MLIB_S_ZOOM_BC_LINE_STORE(STORE_UP, STORE_DN);     \
	pd += 2 * channels;                                \
	r0 = SUM_A##H(v2, tv3l[0], tv3l[1], tv3h[0]) *     \
	    (SCL_A##V * SCL_A##H);                         \
	r1 = SUM_B##H(v2, tv3l[0], tv3l[1], tv3h[0]) *     \
	    (SCL_A##V * SCL_B##H);                         \
	r2 = SUM_A##H(w2, tw3l[0], tw3l[1], tw3h[0]) *     \
	    (SCL_B##V * SCL_A##H);                         \
	r3 = SUM_B##H(w2, tw3l[0], tw3l[1], tw3h[0]) *     \
	    (SCL_B##V * SCL_B##H);                         \
	MLIB_S_ZOOM_BC_LINE_STORE(STORE_UP, STORE_DN);     \
	pd += 2 * channels;                                \
	r0 = SUM_A##H(tv3l[0], tv3l[1], tv3h[0],           \
	    tv3h[1]) * (SCL_A##V * SCL_A##H);              \
	r1 = SUM_B##H(tv3l[0], tv3l[1], tv3h[0],           \
	    tv3h[1]) * (SCL_A##V * SCL_B##H);              \
	r2 = SUM_A##H(tw3l[0], tw3l[1], tw3h[0],           \
	    tw3h[1]) * (SCL_B##V * SCL_A##H);              \
	r3 = SUM_B##H(tw3l[0], tw3l[1], tw3h[0],           \
	    tw3h[1]) * (SCL_B##V * SCL_B##H);              \
	MLIB_S_ZOOM_BC_LINE_STORE(STORE_UP, STORE_DN);     \
	pd += 2 * channels;                                \
	v0 = tv3l[1];                                      \
	v1 = tv3h[0];                                      \
	v2 = tv3h[1];                                      \
	w0 = tw3l[1];                                      \
	w1 = tw3h[0];                                      \
	w2 = tw3h[1];                                      \
	ps += 2;                                           \
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

#define	MLIB_S_ZOOM_BC_LINE_HEAD(H, V, STORE_UP, STORE_DN)        \
	pd = dlc;                                                 \
	if (src_w_beg) {                                          \
	for (k = 0; k < channels; k++) {                          \
		v3[k] = SUM_A##V(ps[-src_stride], ps[0],          \
		    ps[src_stride], ps[2 * src_stride]);          \
		w3[k] = SUM_B##V(ps[-src_stride], ps[0],          \
		    ps[src_stride], ps[2 * src_stride]);          \
		ps++;                                             \
		r1 = SUM_B##H(v0[k], v1[k], v2[k],                \
		    v3[k]) * (SCL_A##V * SCL_B##H);               \
		r3 = SUM_B##H(w0[k], w1[k], w2[k],                \
		    w3[k]) * (SCL_B##V * SCL_B##H);               \
		STORE_UP(pd[0], r1);                              \
		STORE_DN(pd[dst_stride], r3);                     \
		pd++;                                             \
		v0[k] = v1[k];                                    \
		v1[k] = v2[k];                                    \
		v2[k] = v3[k];                                    \
		w0[k] = w1[k];                                    \
		w1[k] = w2[k];                                    \
		w2[k] = w3[k];                                    \
	}                                                         \
	slc += channels;                                          \
	dlc += channels;                                          \
	}                                                         \

/* *********************************************************** */

#define	MLIB_S_ZOOM_BC_LINE_F32_2(H, V, STORE_UP, STORE_DN) {     \
	TTYPE v0[2], v1[2], v2[2], v3[2];                         \
	TTYPE w0[2], w1[2], w2[2], w3[2];                         \
	TTYPE *v34, *w34, *v56, *w56;                             \
	__m128 x_sd0, x_sd1, x_sd2, x_sd3;                        \
	__m128 x_v3, x_w3, x_v4, x_w4;                            \
	v34 = (TTYPE *)&x_v3;                                     \
	w34 = (TTYPE *)&x_w3;                                     \
	v56 = (TTYPE *)&x_v4;                                     \
	w56 = (TTYPE *)&x_w4;                                     \
	MLIB_S_ZOOM_BC_LINE_PRE(V);                               \
	MLIB_S_ZOOM_BC_LINE_HEAD(H, V, STORE_UP, STORE_DN);       \
	for (i = 0; i < (src_width / 8); i++) {                   \
		MLIB_S_MULCONST_F32;                              \
		MLIB_S_ZOOM_BC_LINE_LOAD_F32(V, x_v3, x_w3);      \
		ps += 4;                                          \
		MLIB_S_ZOOM_BC_LINE_LOAD_F32(V, x_v4, x_w4);      \
		ps += 4;                                          \
		pd = dlc + i * 8 * channels;                      \
		r0 = SUM_A##H(v0[0], v1[0], v2[0],                \
		    v34[0]) * (SCL_A##V * SCL_A##H);              \
		r1 = SUM_B##H(v0[0], v1[0], v2[0],                \
		    v34[0]) * (SCL_A##V * SCL_B##H);              \
		r2 = SUM_A##H(w0[0], w1[0], w2[0],                \
		    w34[0]) * (SCL_B##V * SCL_A##H);              \
		r3 = SUM_B##H(w0[0], w1[0], w2[0],                \
		    w34[0]) * (SCL_B##V * SCL_B##H);              \
		MLIB_S_ZOOM_BC_LINE_STORE(STORE_UP, STORE_DN);    \
		pd += 2 * channels;                               \
		r0 = SUM_A##H(v1[0], v2[0], v34[0], v34[2]) *     \
		    (SCL_A##V * SCL_A##H);                        \
		r1 = SUM_B##H(v1[0], v2[0], v34[0], v34[2]) *     \
		    (SCL_A##V * SCL_B##H);                        \
		r2 = SUM_A##H(w1[0], w2[0], w34[0], w34[2]) *     \
		    (SCL_B##V * SCL_A##H);                        \
		r3 = SUM_B##H(w1[0], w2[0], w34[0], w34[2]) *     \
		    (SCL_B##V * SCL_B##H);                        \
		MLIB_S_ZOOM_BC_LINE_STORE(STORE_UP, STORE_DN);    \
		pd += 2 * channels;                               \
		r0 = SUM_A##H(v2[0], v34[0], v34[2], v56[0]) *    \
		    (SCL_A##V * SCL_A##H);                        \
		r1 = SUM_B##H(v2[0], v34[0], v34[2], v56[0]) *    \
		    (SCL_A##V * SCL_B##H);                        \
		r2 = SUM_A##H(w2[0], w34[0], w34[2], w56[0]) *    \
		    (SCL_B##V * SCL_A##H);                        \
		r3 = SUM_B##H(w2[0], w34[0], w34[2], w56[0]) *    \
		    (SCL_B##V * SCL_B##H);                        \
		MLIB_S_ZOOM_BC_LINE_STORE(STORE_UP, STORE_DN);    \
		pd += 2 * channels;                               \
		r0 = SUM_A##H(v34[0], v34[2], v56[0], v56[2]) *   \
		    (SCL_A##V * SCL_A##H);                        \
		r1 = SUM_B##H(v34[0], v34[2], v56[0], v56[2]) *   \
		    (SCL_A##V * SCL_B##H);                        \
		r2 = SUM_A##H(w34[0], w34[2], w56[0], w56[2]) *   \
		    (SCL_B##V * SCL_A##H);                        \
		r3 = SUM_B##H(w34[0], w34[2], w56[0], w56[2]) *   \
		    (SCL_B##V * SCL_B##H);                        \
		MLIB_S_ZOOM_BC_LINE_STORE(STORE_UP, STORE_DN);    \
		                                                  \
		pd = dlc + i * 8 * channels + 1;                  \
		r0 = SUM_A##H(v0[1], v1[1], v2[1],                \
		    v34[1]) * (SCL_A##V * SCL_A##H);              \
		r1 = SUM_B##H(v0[1], v1[1], v2[1],                \
		    v34[1]) * (SCL_A##V * SCL_B##H);              \
		r2 = SUM_A##H(w0[1], w1[1], w2[1],                \
		    w34[1]) * (SCL_B##V * SCL_A##H);              \
		r3 = SUM_B##H(w0[1], w1[1], w2[1],                \
		    w34[1]) * (SCL_B##V * SCL_B##H);              \
		MLIB_S_ZOOM_BC_LINE_STORE(STORE_UP, STORE_DN);    \
		pd += 2 * channels;                               \
		r0 = SUM_A##H(v1[1], v2[1], v34[1], v34[3]) *     \
		    (SCL_A##V * SCL_A##H);                        \
		r1 = SUM_B##H(v1[1], v2[1], v34[1], v34[3]) *     \
		    (SCL_A##V * SCL_B##H);                        \
		r2 = SUM_A##H(w1[1], w2[1], w34[1], w34[3]) *     \
		    (SCL_B##V * SCL_A##H);                        \
		r3 = SUM_B##H(w1[1], w2[1], w34[1], w34[3]) *     \
		    (SCL_B##V * SCL_B##H);                        \
		MLIB_S_ZOOM_BC_LINE_STORE(STORE_UP, STORE_DN);    \
		pd += 2 * channels;                               \
		r0 = SUM_A##H(v2[1], v34[1], v34[3], v56[1]) *    \
		    (SCL_A##V * SCL_A##H);                        \
		r1 = SUM_B##H(v2[1], v34[1], v34[3], v56[1]) *    \
		    (SCL_A##V * SCL_B##H);                        \
		r2 = SUM_A##H(w2[1], w34[1], w34[3], w56[1]) *    \
		    (SCL_B##V * SCL_A##H);                        \
		r3 = SUM_B##H(w2[1], w34[1], w34[3], w56[1]) *    \
		    (SCL_B##V * SCL_B##H);                        \
		MLIB_S_ZOOM_BC_LINE_STORE(STORE_UP, STORE_DN);    \
		pd += 2 * channels;                               \
		r0 = SUM_A##H(v34[1], v34[3], v56[1], v56[3]) *   \
		    (SCL_A##V * SCL_A##H);                        \
		r1 = SUM_B##H(v34[1], v34[3], v56[1], v56[3]) *   \
		    (SCL_A##V * SCL_B##H);                        \
		r2 = SUM_A##H(w34[1], w34[3], w56[1], w56[3]) *   \
		    (SCL_B##V * SCL_A##H);                        \
		r3 = SUM_B##H(w34[1], w34[3], w56[1], w56[3]) *   \
		    (SCL_B##V * SCL_B##H);                        \
		MLIB_S_ZOOM_BC_LINE_STORE(STORE_UP, STORE_DN);    \
		pd++;                                             \
		v0[0] = v34[2];                                   \
		v0[1] = v34[3];                                   \
		v1[0] = v56[0];                                   \
		v1[1] = v56[1];                                   \
		v2[0] = v56[2];                                   \
		v2[1] = v56[3];                                   \
		w0[0] = w34[2];                                   \
		w0[1] = w34[3];                                   \
		w1[0] = w56[0];                                   \
		w1[1] = w56[1];                                   \
		w2[0] = w56[2];                                   \
		w2[1] = w56[3];                                   \
	}                                                         \
	                                                          \
	for (k = 0; k < channels; k++) {                          \
		ps = slc + i * 4 * channels + k + 6;              \
		pd = dlc + i * 8 * channels + k;                  \
		for (int m = i * 4; m < (src_width / 2); m++) {   \
		v3[k] = SUM_A##V(ps[-src_stride], ps[0],          \
		    ps[src_stride], ps[2 * src_stride]);          \
		w3[k] = SUM_B##V(ps[-src_stride], ps[0],          \
		    ps[src_stride], ps[2 * src_stride]);          \
		ps += channels;                                   \
		r0 = SUM_A##H(v0[k], v1[k], v2[k],                \
		    v3[k]) * (SCL_A##V * SCL_A##H);               \
		r1 = SUM_B##H(v0[k], v1[k], v2[k],                \
		    v3[k]) * (SCL_A##V * SCL_B##H);               \
		r2 = SUM_A##H(w0[k], w1[k], w2[k],                \
		    w3[k]) * (SCL_B##V * SCL_A##H);               \
		r3 = SUM_B##H(w0[k], w1[k], w2[k],                \
		    w3[k]) * (SCL_B##V * SCL_B##H);               \
		MLIB_S_ZOOM_BC_LINE_STORE(STORE_UP, STORE_DN);    \
		pd += 2 * channels;                               \
		v0[k] = v1[k];                                    \
		v1[k] = v2[k];                                    \
		v2[k] = v3[k];                                    \
		w0[k] = w1[k];                                    \
		w1[k] = w2[k];                                    \
		w2[k] = w3[k];                                    \
	    }                                                     \
	    if (src_width & 1) {                                  \
		v3[k] = SUM_A##V(ps[-src_stride], ps[0],          \
		    ps[src_stride], ps[2 * src_stride]);          \
		w3[k] = SUM_B##V(ps[-src_stride], ps[0],          \
		    ps[src_stride], ps[2 * src_stride]);          \
		r0 = SUM_A##H(v0[k], v1[k], v2[k],                \
		    v3[k]) * (SCL_A##V * SCL_A##H);               \
		r2 = SUM_A##H(w0[k], w1[k], w2[k],                \
		    w3[k]) * (SCL_B##V * SCL_A##H);               \
		STORE_UP(pd[0], r0);                              \
		STORE_DN(pd[dst_stride], r2);                     \
	    }                                                     \
	}                                                         \
	                                                          \
	sl += src_stride;                                         \
	dl += 2 * dst_stride;                                     \
}

/* *********************************************************** */

#define	MLIB_S_ZOOM_BC_LINE_D64_2(H, V, STORE_UP, STORE_DN) {      \
	TTYPE v0[2], v1[2], v2[2], v3s[2];                         \
	TTYPE w0[2], w1[2], w2[2], w3s[2];                         \
	__m128d x_sd0_l, x_sd1_l, x_sd2_l, x_sd3_l;                \
	__m128d x_sd0_h, x_sd1_h, x_sd2_h, x_sd3_h;                \
	__m128d x_v3_l, x_v3_h, x_w3_l, x_w3_h;                    \
	__m128d x_v4_l, x_v4_h, x_w4_l, x_w4_h;                    \
	TTYPE *v3, *v4, *w3, *w4;                                  \
	TTYPE *v5, *v6, *w5, *w6;                                  \
	v3 = (TTYPE *)&x_v3_l;                                     \
	w3 = (TTYPE *)&x_w3_l;                                     \
	v4 = (TTYPE *)&x_v3_h;                                     \
	w4 = (TTYPE *)&x_w3_h;                                     \
	v5 = (TTYPE *)&x_v4_l;                                     \
	w5 = (TTYPE *)&x_w4_l;                                     \
	v6 = (TTYPE *)&x_v4_h;                                     \
	w6 = (TTYPE *)&x_w4_h;                                     \
	MLIB_S_ZOOM_BC_LINE_PRE(V);                                \
	MLIB_S_ZOOM_BC_LINE_HEAD(H, V, STORE_UP, STORE_DN);        \
	for (i = 0; i < (src_width / 8); i++) {                    \
		MLIB_S_MULCONST_D64;                               \
		MLIB_S_ZOOM_BC_LINE_LOAD_D64(                      \
		    V, x_v3_l, x_w3_l, x_v3_h, x_w3_h);            \
		ps += 2;                                           \
		MLIB_S_ZOOM_BC_LINE_LOAD_D64(                      \
		    V, x_v4_l, x_w4_l, x_v4_h, x_w4_h);            \
		ps += 2;                                           \
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
		r0 = SUM_A##H(v1[0], v2[0], v3[0], v4[0]) *        \
		    (SCL_A##V * SCL_A##H);                         \
		r1 = SUM_B##H(v1[0], v2[0], v3[0], v4[0]) *        \
		    (SCL_A##V * SCL_B##H);                         \
		r2 = SUM_A##H(w1[0], w2[0], w3[0], w4[0]) *        \
		    (SCL_B##V * SCL_A##H);                         \
		r3 = SUM_B##H(w1[0], w2[0], w3[0], w4[0]) *        \
		    (SCL_B##V * SCL_B##H);                         \
		MLIB_S_ZOOM_BC_LINE_STORE(STORE_UP, STORE_DN);     \
		pd += 2 * channels;                                \
		r0 = SUM_A##H(v2[0], v3[0], v4[0], v5[0]) *        \
		    (SCL_A##V * SCL_A##H);                         \
		r1 = SUM_B##H(v2[0], v3[0], v4[0], v5[0]) *        \
		    (SCL_A##V * SCL_B##H);                         \
		r2 = SUM_A##H(w2[0], w3[0], w4[0], w5[0]) *        \
		    (SCL_B##V * SCL_A##H);                         \
		r3 = SUM_B##H(w2[0], w3[0], w4[0], w5[0]) *        \
		    (SCL_B##V * SCL_B##H);                         \
		MLIB_S_ZOOM_BC_LINE_STORE(STORE_UP, STORE_DN);     \
		pd += 2 * channels;                                \
		r0 = SUM_A##H(v3[0], v4[0], v5[0], v6[0]) *        \
		    (SCL_A##V * SCL_A##H);                         \
		r1 = SUM_B##H(v3[0], v4[0], v5[0], v6[0]) *        \
		    (SCL_A##V * SCL_B##H);                         \
		r2 = SUM_A##H(w3[0], w4[0], w5[0], w6[0]) *        \
		    (SCL_B##V * SCL_A##H);                         \
		r3 = SUM_B##H(w3[0], w4[0], w5[0], w6[0]) *        \
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
		r0 = SUM_A##H(v1[1], v2[1], v3[1], v4[1]) *        \
		    (SCL_A##V * SCL_A##H);                         \
		r1 = SUM_B##H(v1[1], v2[1], v3[1], v4[1]) *        \
		    (SCL_A##V * SCL_B##H);                         \
		r2 = SUM_A##H(w1[1], w2[1], w3[1], w4[1]) *        \
		    (SCL_B##V * SCL_A##H);                         \
		r3 = SUM_B##H(w1[1], w2[1], w3[1], w4[1]) *        \
		    (SCL_B##V * SCL_B##H);                         \
		MLIB_S_ZOOM_BC_LINE_STORE(STORE_UP, STORE_DN);     \
		pd += 2 * channels;                                \
		r0 = SUM_A##H(v2[1], v3[1], v4[1], v5[1]) *        \
		    (SCL_A##V * SCL_A##H);                         \
		r1 = SUM_B##H(v2[1], v3[1], v4[1], v5[1]) *        \
		    (SCL_A##V * SCL_B##H);                         \
		r2 = SUM_A##H(w2[1], w3[1], w4[1], w5[1]) *        \
		    (SCL_B##V * SCL_A##H);                         \
		r3 = SUM_B##H(w2[1], w3[1], w4[1], w5[1]) *        \
		    (SCL_B##V * SCL_B##H);                         \
		MLIB_S_ZOOM_BC_LINE_STORE(STORE_UP, STORE_DN);     \
		pd += 2 * channels;                                \
		r0 = SUM_A##H(v3[1], v4[1], v5[1], v6[1]) *        \
		    (SCL_A##V * SCL_A##H);                         \
		r1 = SUM_B##H(v3[1], v4[1], v5[1], v6[1]) *        \
		    (SCL_A##V * SCL_B##H);                         \
		r2 = SUM_A##H(w3[1], w4[1], w5[1], w6[1]) *        \
		    (SCL_B##V * SCL_A##H);                         \
		r3 = SUM_B##H(w3[1], w4[1], w5[1], w6[1]) *        \
		    (SCL_B##V * SCL_B##H);                         \
		MLIB_S_ZOOM_BC_LINE_STORE(STORE_UP, STORE_DN);     \
		pd++;                                              \
		v0[0] = v4[0];                                     \
		v0[1] = v4[1];                                     \
		v1[0] = v5[0];                                     \
		v1[1] = v5[1];                                     \
		v2[0] = v6[0];                                     \
		v2[1] = v6[1];                                     \
		w0[0] = w4[0];                                     \
		w0[1] = w4[1];                                     \
		w1[0] = w5[0];                                     \
		w1[1] = w5[1];                                     \
		w2[0] = w6[0];                                     \
		w2[1] = w6[1];                                     \
	}                                                          \
	                                                           \
	for (k = 0; k < channels; k++) {                           \
		ps = slc + i * 4 * channels + k + 6;               \
		pd = dlc + i * 8 * channels + k;                   \
		for (int m = i * 4; m < (src_width / 2); m++) {    \
		v3s[k] = SUM_A##V(ps[-src_stride], ps[0],          \
		    ps[src_stride], ps[2 * src_stride]);           \
		w3s[k] = SUM_B##V(ps[-src_stride], ps[0],          \
		    ps[src_stride], ps[2 * src_stride]);           \
		ps += channels;                                    \
		r0 = SUM_A##H(v0[k], v1[k], v2[k],                 \
		    v3s[k]) * (SCL_A##V * SCL_A##H);               \
		r1 = SUM_B##H(v0[k], v1[k], v2[k],                 \
		    v3s[k]) * (SCL_A##V * SCL_B##H);               \
		r2 = SUM_A##H(w0[k], w1[k], w2[k],                 \
		    w3s[k]) * (SCL_B##V * SCL_A##H);               \
		r3 = SUM_B##H(w0[k], w1[k], w2[k],                 \
		    w3s[k]) * (SCL_B##V * SCL_B##H);               \
		MLIB_S_ZOOM_BC_LINE_STORE(STORE_UP, STORE_DN);     \
		pd += 2 * channels;                                \
		v0[k] = v1[k];                                     \
		v1[k] = v2[k];                                     \
		v2[k] = v3s[k];                                    \
		w0[k] = w1[k];                                     \
		w1[k] = w2[k];                                     \
		w2[k] = w3s[k];                                    \
	    }                                                      \
	    if (src_width & 1) {                                   \
		v3s[k] = SUM_A##V(ps[-src_stride], ps[0],          \
		    ps[src_stride], ps[2 * src_stride]);           \
		w3s[k] = SUM_B##V(ps[-src_stride], ps[0],          \
		    ps[src_stride], ps[2 * src_stride]);           \
		r0 = SUM_A##H(v0[k], v1[k], v2[k],                 \
		    v3s[k]) * (SCL_A##V * SCL_A##H);               \
		r2 = SUM_A##H(w0[k], w1[k], w2[k],                 \
		    w3s[k]) * (SCL_B##V * SCL_A##H);               \
		STORE_UP(pd[0], r0);                               \
		STORE_DN(pd[dst_stride], r2);                      \
	    }                                                      \
	}                                                          \
	                                                           \
	sl += src_stride;                                          \
	dl += 2 * dst_stride;                                      \
}

/* *********************************************************** */

#define	MLIB_S_ZOOM_BC_LINE_F32_3(H, V, STORE_UP, STORE_DN) {      \
	TTYPE v0[3], v1[3], v2[3], v3[3];                          \
	TTYPE w0[3], w1[3], w2[3], w3[3];                          \
	TTYPE *v34, *v5, *v6;                                      \
	TTYPE *w34, *w5, *w6;                                      \
	__m128 x_sd0, x_sd1, x_sd2, x_sd3;                         \
	__m128 x_v34, x_w34, x_v5, x_w5, x_v6, x_w6;               \
	v34 = (TTYPE *)&x_v34;                                     \
	w34 = (TTYPE *)&x_w34;                                     \
	v5 = (TTYPE *)&x_v5;                                       \
	w5 = (TTYPE *)&x_w5;                                       \
	v6 = (TTYPE *)&x_v6;                                       \
	w6 = (TTYPE *)&x_w6;                                       \
	MLIB_S_ZOOM_BC_LINE_PRE(V);                                \
	MLIB_S_ZOOM_BC_LINE_HEAD(H, V, STORE_UP, STORE_DN);        \
	for (i = 0; i < (src_width * 3 / (2 * 12)); i++) {         \
		MLIB_S_MULCONST_F32;                               \
		MLIB_S_ZOOM_BC_LINE_LOAD_F32(V, x_v34, x_w34);     \
		ps += 4;                                           \
		MLIB_S_ZOOM_BC_LINE_LOAD_F32(V, x_v5, x_w5);       \
		ps += 4;                                           \
		MLIB_S_ZOOM_BC_LINE_LOAD_F32(V, x_v6, x_w6);       \
		ps += 4;                                           \
			                                           \
		pd = dlc + i * 8 * channels;                       \
		r0 = SUM_A##H(v0[0], v1[0], v2[0],                 \
		    v34[0]) * (SCL_A##V * SCL_A##H);               \
		r1 = SUM_B##H(v0[0], v1[0], v2[0],                 \
		    v34[0]) * (SCL_A##V * SCL_B##H);               \
		r2 = SUM_A##H(w0[0], w1[0], w2[0],                 \
		    w34[0]) * (SCL_B##V * SCL_A##H);               \
		r3 = SUM_B##H(w0[0], w1[0], w2[0],                 \
		    w34[0]) * (SCL_B##V * SCL_B##H);               \
		MLIB_S_ZOOM_BC_LINE_STORE(STORE_UP, STORE_DN);     \
		pd += 2 * channels;                                \
		r0 = SUM_A##H(v1[0], v2[0], v34[0], v34[3]) *      \
		    (SCL_A##V * SCL_A##H);                         \
		r1 = SUM_B##H(v1[0], v2[0], v34[0], v34[3]) *      \
		    (SCL_A##V * SCL_B##H);                         \
		r2 = SUM_A##H(w1[0], w2[0], w34[0], w34[3]) *      \
		    (SCL_B##V * SCL_A##H);                         \
		r3 = SUM_B##H(w1[0], w2[0], w34[0], w34[3]) *      \
		    (SCL_B##V * SCL_B##H);                         \
		MLIB_S_ZOOM_BC_LINE_STORE(STORE_UP, STORE_DN);     \
		pd += 2 * channels;                                \
		r0 = SUM_A##H(v2[0], v34[0], v34[3], v5[2]) *      \
		    (SCL_A##V * SCL_A##H);                         \
		r1 = SUM_B##H(v2[0], v34[0], v34[3], v5[2]) *      \
		    (SCL_A##V * SCL_B##H);                         \
		r2 = SUM_A##H(w2[0], w34[0], w34[3], w5[2]) *      \
		    (SCL_B##V * SCL_A##H);                         \
		r3 = SUM_B##H(w2[0], w34[0], w34[3], w5[2]) *      \
		    (SCL_B##V * SCL_B##H);                         \
		MLIB_S_ZOOM_BC_LINE_STORE(STORE_UP, STORE_DN);     \
		pd += 2 * channels;                                \
		r0 = SUM_A##H(v34[0], v34[3], v5[2], v6[1]) *      \
		    (SCL_A##V * SCL_A##H);                         \
		r1 = SUM_B##H(v34[0], v34[3], v5[2], v6[1]) *      \
		    (SCL_A##V * SCL_B##H);                         \
		r2 = SUM_A##H(w34[0], w34[3], w5[2], w6[1]) *      \
		    (SCL_B##V * SCL_A##H);                         \
		r3 = SUM_B##H(w34[0], w34[3], w5[2], w6[1]) *      \
		    (SCL_B##V * SCL_B##H);                         \
		MLIB_S_ZOOM_BC_LINE_STORE(STORE_UP, STORE_DN);     \
			                                           \
		pd = dlc + i * 8 * channels + 1;                   \
		r0 = SUM_A##H(v0[1], v1[1], v2[1],                 \
		    v34[1]) * (SCL_A##V * SCL_A##H);               \
		r1 = SUM_B##H(v0[1], v1[1], v2[1],                 \
		    v34[1]) * (SCL_A##V * SCL_B##H);               \
		r2 = SUM_A##H(w0[1], w1[1], w2[1],                 \
		    w34[1]) * (SCL_B##V * SCL_A##H);               \
		r3 = SUM_B##H(w0[1], w1[1], w2[1],                 \
		    w34[1]) * (SCL_B##V * SCL_B##H);               \
		MLIB_S_ZOOM_BC_LINE_STORE(STORE_UP, STORE_DN);     \
		pd += 2 * channels;                                \
		r0 = SUM_A##H(v1[1], v2[1], v34[1], v5[0]) *       \
		    (SCL_A##V * SCL_A##H);                         \
		r1 = SUM_B##H(v1[1], v2[1], v34[1], v5[0]) *       \
		    (SCL_A##V * SCL_B##H);                         \
		r2 = SUM_A##H(w1[1], w2[1], w34[1], w5[0]) *       \
		    (SCL_B##V * SCL_A##H);                         \
		r3 = SUM_B##H(w1[1], w2[1], w34[1], w5[0]) *       \
		    (SCL_B##V * SCL_B##H);                         \
		MLIB_S_ZOOM_BC_LINE_STORE(STORE_UP, STORE_DN);     \
		pd += 2 * channels;                                \
		r0 = SUM_A##H(v2[1], v34[1], v5[0], v5[3]) *       \
		    (SCL_A##V * SCL_A##H);                         \
		r1 = SUM_B##H(v2[1], v34[1], v5[0], v5[3]) *       \
		    (SCL_A##V * SCL_B##H);                         \
		r2 = SUM_A##H(w2[1], w34[1], w5[0], w5[3]) *       \
		    (SCL_B##V * SCL_A##H);                         \
		r3 = SUM_B##H(w2[1], w34[1], w5[0], w5[3]) *       \
		    (SCL_B##V * SCL_B##H);                         \
		MLIB_S_ZOOM_BC_LINE_STORE(STORE_UP, STORE_DN);     \
		pd += 2 * channels;                                \
		r0 = SUM_A##H(v34[1], v5[0], v5[3], v6[2]) *       \
		    (SCL_A##V * SCL_A##H);                         \
		r1 = SUM_B##H(v34[1], v5[0], v5[3], v6[2]) *       \
		    (SCL_A##V * SCL_B##H);                         \
		r2 = SUM_A##H(w34[1], w5[0], w5[3], w6[2]) *       \
		    (SCL_B##V * SCL_A##H);                         \
		r3 = SUM_B##H(w34[1], w5[0], w5[3], w6[2]) *       \
		    (SCL_B##V * SCL_B##H);                         \
		MLIB_S_ZOOM_BC_LINE_STORE(STORE_UP, STORE_DN);     \
		pd = dlc + i * 8 * channels + 2;                   \
		r0 = SUM_A##H(v0[2], v1[2], v2[2],                 \
		    v34[2]) * (SCL_A##V * SCL_A##H);               \
		r1 = SUM_B##H(v0[2], v1[2], v2[2],                 \
		    v34[2]) * (SCL_A##V * SCL_B##H);               \
		r2 = SUM_A##H(w0[2], w1[2], w2[2],                 \
		    w34[2]) * (SCL_B##V * SCL_A##H);               \
		r3 = SUM_B##H(w0[2], w1[2], w2[2],                 \
		    w34[2]) * (SCL_B##V * SCL_B##H);               \
		MLIB_S_ZOOM_BC_LINE_STORE(STORE_UP, STORE_DN);     \
		pd += 2 * channels;                                \
		r0 = SUM_A##H(v1[2], v2[2], v34[2], v5[1]) *       \
		    (SCL_A##V * SCL_A##H);                         \
		r1 = SUM_B##H(v1[2], v2[2], v34[2], v5[1]) *       \
		    (SCL_A##V * SCL_B##H);                         \
		r2 = SUM_A##H(w1[2], w2[2], w34[2], w5[1]) *       \
		    (SCL_B##V * SCL_A##H);                         \
		r3 = SUM_B##H(w1[2], w2[2], w34[2], w5[1]) *       \
		    (SCL_B##V * SCL_B##H);                         \
		MLIB_S_ZOOM_BC_LINE_STORE(STORE_UP, STORE_DN);     \
		pd += 2 * channels;                                \
		r0 = SUM_A##H(v2[2], v34[2], v5[1], v6[0]) *       \
		    (SCL_A##V * SCL_A##H);                         \
		r1 = SUM_B##H(v2[2], v34[2], v5[1], v6[0]) *       \
		    (SCL_A##V * SCL_B##H);                         \
		r2 = SUM_A##H(w2[2], w34[2], w5[1], w6[0]) *       \
		    (SCL_B##V * SCL_A##H);                         \
		r3 = SUM_B##H(w2[2], w34[2], w5[1], w6[0]) *       \
		    (SCL_B##V * SCL_B##H);                         \
		MLIB_S_ZOOM_BC_LINE_STORE(STORE_UP, STORE_DN);     \
		pd += 2 * channels;                                \
		r0 = SUM_A##H(v34[2], v5[1], v6[0], v6[3]) *       \
		    (SCL_A##V * SCL_A##H);                         \
		r1 = SUM_B##H(v34[2], v5[1], v6[0], v6[3]) *       \
		    (SCL_A##V * SCL_B##H);                         \
		r2 = SUM_A##H(w34[2], w5[1], w6[0], w6[3]) *       \
		    (SCL_B##V * SCL_A##H);                         \
		r3 = SUM_B##H(w34[2], w5[1], w6[0], w6[3]) *       \
		    (SCL_B##V * SCL_B##H);                         \
		MLIB_S_ZOOM_BC_LINE_STORE(STORE_UP, STORE_DN);     \
		pd++;                                              \
		v0[0] = v34[3];                                    \
		v0[1] = v5[0];                                     \
		v0[2] = v5[1];                                     \
		v1[0] = v5[2];                                     \
		v1[1] = v5[3];                                     \
		v1[2] = v6[0];                                     \
		v2[0] = v6[1];                                     \
		v2[1] = v6[2];                                     \
		v2[2] = v6[3];                                     \
		w0[0] = w34[3];                                    \
		w0[1] = w5[0];                                     \
		w0[2] = w5[1];                                     \
		w1[0] = w5[2];                                     \
		w1[1] = w5[3];                                     \
		w1[2] = w6[0];                                     \
		w2[0] = w6[1];                                     \
		w2[1] = w6[2];                                     \
		w2[2] = w6[3];                                     \
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

#define	MLIB_S_ZOOM_BC_LINE_D64_3(H, V, STORE_UP, STORE_DN) {      \
	TTYPE v0[3], v1[3], v2[3], v3s[3];                         \
	TTYPE w0[3], w1[3], w2[3], w3s[3];                         \
	TTYPE *v3, *v4, *v5, *v6, *v7, *v8;                        \
	TTYPE *w3, *w4, *w5, *w6, *w7, *w8;                        \
	__m128d x_sd0_l, x_sd1_l, x_sd2_l, x_sd3_l;                \
	__m128d x_sd0_h, x_sd1_h, x_sd2_h, x_sd3_h;                \
	__m128d x_v3_l, x_v3_h, x_w3_l, x_w3_h;                    \
	__m128d x_v4_l, x_v4_h, x_w4_l, x_w4_h;                    \
	__m128d x_v5_l, x_v5_h, x_w5_l, x_w5_h;                    \
	v3 = (TTYPE *)&x_v3_l;                                     \
	w3 = (TTYPE *)&x_w3_l;                                     \
	v4 = (TTYPE *)&x_v3_h;                                     \
	w4 = (TTYPE *)&x_w3_h;                                     \
	v5 = (TTYPE *)&x_v4_l;                                     \
	w5 = (TTYPE *)&x_w4_l;                                     \
	v6 = (TTYPE *)&x_v4_h;                                     \
	w6 = (TTYPE *)&x_w4_h;                                     \
	v7 = (TTYPE *)&x_v5_l;                                     \
	w7 = (TTYPE *)&x_w5_l;                                     \
	v8 = (TTYPE *)&x_v5_h;                                     \
	w8 = (TTYPE *)&x_w5_h;                                     \
	MLIB_S_ZOOM_BC_LINE_PRE(V);                                \
	MLIB_S_ZOOM_BC_LINE_HEAD(H, V, STORE_UP, STORE_DN);        \
	for (i = 0; i < (src_width * 3 / (2 * 12)); i++) {         \
		MLIB_S_MULCONST_D64;                               \
		MLIB_S_ZOOM_BC_LINE_LOAD_D64(                      \
		    V, x_v3_l, x_w3_l, x_v3_h, x_w3_h);            \
		ps += 2;                                           \
		MLIB_S_ZOOM_BC_LINE_LOAD_D64(                      \
		    V, x_v4_l, x_w4_l, x_v4_h, x_w4_h);            \
		ps += 2;                                           \
		MLIB_S_ZOOM_BC_LINE_LOAD_D64(                      \
		    V, x_v5_l, x_w5_l, x_v5_h, x_w5_h);            \
		ps += 2;                                           \
			                                           \
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
		v3s[k] = SUM_A##V(ps[-src_stride], ps[0],          \
		    ps[src_stride], ps[2 * src_stride]);           \
		w3s[k] = SUM_B##V(ps[-src_stride], ps[0],          \
		    ps[src_stride], ps[2 * src_stride]);           \
		ps += channels;                                    \
		r0 = SUM_A##H(v0[k], v1[k], v2[k],                 \
		    v3s[k]) * (SCL_A##V * SCL_A##H);               \
		r1 = SUM_B##H(v0[k], v1[k], v2[k],                 \
		    v3s[k]) * (SCL_A##V * SCL_B##H);               \
		r2 = SUM_A##H(w0[k], w1[k], w2[k],                 \
		    w3s[k]) * (SCL_B##V * SCL_A##H);               \
		r3 = SUM_B##H(w0[k], w1[k], w2[k],                 \
		    w3s[k]) * (SCL_B##V * SCL_B##H);               \
		MLIB_S_ZOOM_BC_LINE_STORE(STORE_UP, STORE_DN);     \
		pd += 2 * channels;                                \
		v0[k] = v1[k];                                     \
		v1[k] = v2[k];                                     \
		v2[k] = v3s[k];                                    \
		w0[k] = w1[k];                                     \
		w1[k] = w2[k];                                     \
		w2[k] = w3s[k];                                    \
	    }                                                      \
	    if (src_width & 1) {                                   \
		v3s[k] = SUM_A##V(ps[-src_stride], ps[0],          \
		    ps[src_stride], ps[2 * src_stride]);           \
		w3s[k] = SUM_B##V(ps[-src_stride], ps[0],          \
		    ps[src_stride], ps[2 * src_stride]);           \
		r0 = SUM_A##H(v0[k], v1[k], v2[k],                 \
		    v3s[k]) * (SCL_A##V * SCL_A##H);               \
		r2 = SUM_A##H(w0[k], w1[k], w2[k],                 \
		    w3s[k]) * (SCL_B##V * SCL_A##H);               \
		STORE_UP(pd[0], r0);                               \
		STORE_DN(pd[dst_stride], r2);                      \
	    }                                                      \
	}                                                          \
	                                                           \
	sl += src_stride;                                          \
	dl += 2 * dst_stride;                                      \
}

/* *********************************************************** */

#define	MLIB_S_ZOOM_BC_LINE_F32_4(H, V, STORE_UP, STORE_DN) {      \
	TTYPE v0[4], v1[4], v2[4];                                 \
	TTYPE w0[4], w1[4], w2[4];                                 \
	TTYPE *v3, *w3, *v4, *w4;                                  \
	__m128 x_sd0, x_sd1, x_sd2, x_sd3;                         \
	__m128 x_v3, x_w3, x_v4, x_w4;                             \
	v3 = (TTYPE *)&x_v3;                                       \
	w3 = (TTYPE *)&x_w3;                                       \
	v4 = (TTYPE *)&x_v4;                                       \
	w4 = (TTYPE *)&x_w4;                                       \
	MLIB_S_ZOOM_BC_LINE_PRE(V);                                \
	MLIB_S_ZOOM_BC_LINE_HEAD(H, V, STORE_UP, STORE_DN);        \
	for (i = 0; i < (src_width / 4); i++) {                    \
		MLIB_S_MULCONST_F32;                               \
		MLIB_S_ZOOM_BC_LINE_LOAD_F32(V, x_v3, x_w3);       \
		ps += 4;                                           \
		MLIB_S_ZOOM_BC_LINE_LOAD_F32(V, x_v4, x_w4);       \
		ps += 4;                                           \
			                                           \
		pd = dlc + i * 4 * channels;                       \
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
		r0 = SUM_A##H(v1[0], v2[0], v3[0],                 \
		    v4[0]) * (SCL_A##V * SCL_A##H);                \
		r1 = SUM_B##H(v1[0], v2[0], v3[0],                 \
		    v4[0]) * (SCL_A##V * SCL_B##H);                \
		r2 = SUM_A##H(w1[0], w2[0], w3[0],                 \
		    w4[0]) * (SCL_B##V * SCL_A##H);                \
		r3 = SUM_B##H(w1[0], w2[0], w3[0],                 \
		    w4[0]) * (SCL_B##V * SCL_B##H);                \
		MLIB_S_ZOOM_BC_LINE_STORE(STORE_UP, STORE_DN);     \
		pd = dlc + i * 4 * channels + 1;                   \
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
		r0 = SUM_A##H(v1[1], v2[1], v3[1],                 \
		    v4[1]) * (SCL_A##V * SCL_A##H);                \
		r1 = SUM_B##H(v1[1], v2[1], v3[1],                 \
		    v4[1]) * (SCL_A##V * SCL_B##H);                \
		r2 = SUM_A##H(w1[1], w2[1], w3[1],                 \
		    w4[1]) * (SCL_B##V * SCL_A##H);                \
		r3 = SUM_B##H(w1[1], w2[1], w3[1],                 \
		    w4[1]) * (SCL_B##V * SCL_B##H);                \
		MLIB_S_ZOOM_BC_LINE_STORE(STORE_UP, STORE_DN);     \
		pd = dlc + i * 4 * channels + 2;                   \
		r0 = SUM_A##H(v0[2], v1[2], v2[2],                 \
		    v3[2]) * (SCL_A##V * SCL_A##H);                \
		r1 = SUM_B##H(v0[2], v1[2], v2[2],                 \
		    v3[2]) * (SCL_A##V * SCL_B##H);                \
		r2 = SUM_A##H(w0[2], w1[2], w2[2],                 \
		    w3[2]) * (SCL_B##V * SCL_A##H);                \
		r3 = SUM_B##H(w0[2], w1[2], w2[2],                 \
		    w3[2]) * (SCL_B##V * SCL_B##H);                \
		MLIB_S_ZOOM_BC_LINE_STORE(STORE_UP, STORE_DN);     \
		pd += 2 * channels;                                \
		r0 = SUM_A##H(v1[2], v2[2], v3[2],                 \
		    v4[2]) * (SCL_A##V * SCL_A##H);                \
		r1 = SUM_B##H(v1[2], v2[2], v3[2],                 \
		    v4[2]) * (SCL_A##V * SCL_B##H);                \
		r2 = SUM_A##H(w1[2], w2[2], w3[2],                 \
		    w4[2]) * (SCL_B##V * SCL_A##H);                \
		r3 = SUM_B##H(w1[2], w2[2], w3[2],                 \
		    w4[2]) * (SCL_B##V * SCL_B##H);                \
		MLIB_S_ZOOM_BC_LINE_STORE(STORE_UP, STORE_DN);     \
		pd = dlc + i * 4 * channels + 3;                   \
		r0 = SUM_A##H(v0[3], v1[3], v2[3],                 \
		    v3[3]) * (SCL_A##V * SCL_A##H);                \
		r1 = SUM_B##H(v0[3], v1[3], v2[3],                 \
		    v3[3]) * (SCL_A##V * SCL_B##H);                \
		r2 = SUM_A##H(w0[3], w1[3], w2[3],                 \
		    w3[3]) * (SCL_B##V * SCL_A##H);                \
		r3 = SUM_B##H(w0[3], w1[3], w2[3],                 \
		    w3[3]) * (SCL_B##V * SCL_B##H);                \
		MLIB_S_ZOOM_BC_LINE_STORE(STORE_UP, STORE_DN);     \
		pd += 2 * channels;                                \
		r0 = SUM_A##H(v1[3], v2[3], v3[3],                 \
		    v4[3]) * (SCL_A##V * SCL_A##H);                \
		r1 = SUM_B##H(v1[3], v2[3], v3[3],                 \
		    v4[3]) * (SCL_A##V * SCL_B##H);                \
		r2 = SUM_A##H(w1[3], w2[3], w3[3],                 \
		    w4[3]) * (SCL_B##V * SCL_A##H);                \
		r3 = SUM_B##H(w1[3], w2[3], w3[3],                 \
		    w4[3]) * (SCL_B##V * SCL_B##H);                \
		MLIB_S_ZOOM_BC_LINE_STORE(STORE_UP, STORE_DN);     \
		for (k = 0; k < channels; k++) {                   \
			v0[k] = v2[k];                             \
			v1[k] = v3[k];                             \
			v2[k] = v4[k];                             \
			w0[k] = w2[k];                             \
			w1[k] = w3[k];                             \
			w2[k] = w4[k];                             \
		}                                                  \
	}                                                          \
	int t = i * 2;                                             \
	if (t < (src_width / 2)) {                                 \
		MLIB_S_MULCONST_F32;                               \
		MLIB_S_ZOOM_BC_LINE_LOAD_F32(V, x_v3, x_w3);       \
		ps += 4;                                           \
		pd = dlc + i * 4 * channels;                       \
		r0 = SUM_A##H(v0[0], v1[0], v2[0],                 \
		    v3[0]) * (SCL_A##V * SCL_A##H);                \
		r1 = SUM_B##H(v0[0], v1[0], v2[0],                 \
		    v3[0]) * (SCL_A##V * SCL_B##H);                \
		r2 = SUM_A##H(w0[0], w1[0], w2[0],                 \
		    w3[0]) * (SCL_B##V * SCL_A##H);                \
		r3 = SUM_B##H(w0[0], w1[0], w2[0],                 \
		    w3[0]) * (SCL_B##V * SCL_B##H);                \
		MLIB_S_ZOOM_BC_LINE_STORE(STORE_UP, STORE_DN);     \
		pd = dlc + i * 4 * channels + 1;                   \
		r0 = SUM_A##H(v0[1], v1[1], v2[1],                 \
		    v3[1]) * (SCL_A##V * SCL_A##H);                \
		r1 = SUM_B##H(v0[1], v1[1], v2[1],                 \
		    v3[1]) * (SCL_A##V * SCL_B##H);                \
		r2 = SUM_A##H(w0[1], w1[1], w2[1],                 \
		    w3[1]) * (SCL_B##V * SCL_A##H);                \
		r3 = SUM_B##H(w0[1], w1[1], w2[1],                 \
		    w3[1]) * (SCL_B##V * SCL_B##H);                \
		MLIB_S_ZOOM_BC_LINE_STORE(STORE_UP, STORE_DN);     \
		pd = dlc + i * 4 * channels + 2;                   \
		r0 = SUM_A##H(v0[2], v1[2], v2[2],                 \
		    v3[2]) * (SCL_A##V * SCL_A##H);                \
		r1 = SUM_B##H(v0[2], v1[2], v2[2],                 \
		    v3[2]) * (SCL_A##V * SCL_B##H);                \
		r2 = SUM_A##H(w0[2], w1[2], w2[2],                 \
		    w3[2]) * (SCL_B##V * SCL_A##H);                \
		r3 = SUM_B##H(w0[2], w1[2], w2[2],                 \
		    w3[2]) * (SCL_B##V * SCL_B##H);                \
		MLIB_S_ZOOM_BC_LINE_STORE(STORE_UP, STORE_DN);     \
		pd = dlc + i * 4 * channels + 3;                   \
		r0 = SUM_A##H(v0[3], v1[3], v2[3],                 \
		    v3[3]) * (SCL_A##V * SCL_A##H);                \
		r1 = SUM_B##H(v0[3], v1[3], v2[3],                 \
		    v3[3]) * (SCL_A##V * SCL_B##H);                \
		r2 = SUM_A##H(w0[3], w1[3], w2[3],                 \
		    w3[3]) * (SCL_B##V * SCL_A##H);                \
		r3 = SUM_B##H(w0[3], w1[3], w2[3],                 \
		    w3[3]) * (SCL_B##V * SCL_B##H);                \
		MLIB_S_ZOOM_BC_LINE_STORE(STORE_UP, STORE_DN);     \
		for (k = 0; k < channels; k++) {                   \
			v0[k] = v1[k];                             \
			v1[k] = v2[k];                             \
			v2[k] = v3[k];                             \
			w0[k] = w1[k];                             \
			w1[k] = w2[k];                             \
			w2[k] = w3[k];                             \
		}                                                  \
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

#define	MLIB_S_ZOOM_BC_LINE_D64_4(H, V, STORE_UP, STORE_DN) {      \
	TTYPE v0[4], v1[4], v2[4], v3s[4];                         \
	TTYPE w0[4], w1[4], w2[4], w3s[4];                         \
	TTYPE *v3, *v4, *w3, *w4;                                  \
	TTYPE *v5, *v6, *w5, *w6;                                  \
	__m128d x_sd0_l, x_sd1_l, x_sd2_l, x_sd3_l;                \
	__m128d x_sd0_h, x_sd1_h, x_sd2_h, x_sd3_h;                \
	__m128d x_v3_l, x_v3_h, x_w3_l, x_w3_h;                    \
	__m128d x_v4_l, x_v4_h, x_w4_l, x_w4_h;                    \
	v3 = (TTYPE *)&x_v3_l;                                     \
	w3 = (TTYPE *)&x_w3_l;                                     \
	v4 = (TTYPE *)&x_v3_h;                                     \
	w4 = (TTYPE *)&x_w3_h;                                     \
	v5 = (TTYPE *)&x_v4_l;                                     \
	w5 = (TTYPE *)&x_w4_l;                                     \
	v6 = (TTYPE *)&x_v4_h;                                     \
	w6 = (TTYPE *)&x_w4_h;                                     \
	MLIB_S_ZOOM_BC_LINE_PRE(V);                                \
	MLIB_S_ZOOM_BC_LINE_HEAD(H, V, STORE_UP, STORE_DN);        \
	for (i = 0; i < (src_width / 4); i++) {                    \
		MLIB_S_MULCONST_D64;                               \
		MLIB_S_ZOOM_BC_LINE_LOAD_D64(                      \
		    V, x_v3_l, x_w3_l, x_v3_h, x_w3_h);            \
		ps += 2;                                           \
		MLIB_S_ZOOM_BC_LINE_LOAD_D64(                      \
		    V, x_v4_l, x_w4_l, x_v4_h, x_w4_h);            \
		ps += 2;                                           \
			                                           \
		pd = dlc + i * 4 * channels;                       \
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
		r0 = SUM_A##H(v1[0], v2[0], v3[0],                 \
		    v5[0]) * (SCL_A##V * SCL_A##H);                \
		r1 = SUM_B##H(v1[0], v2[0], v3[0],                 \
		    v5[0]) * (SCL_A##V * SCL_B##H);                \
		r2 = SUM_A##H(w1[0], w2[0], w3[0],                 \
		    w5[0]) * (SCL_B##V * SCL_A##H);                \
		r3 = SUM_B##H(w1[0], w2[0], w3[0],                 \
		    w5[0]) * (SCL_B##V * SCL_B##H);                \
		MLIB_S_ZOOM_BC_LINE_STORE(STORE_UP, STORE_DN);     \
		pd = dlc + i * 4 * channels + 1;                   \
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
		r0 = SUM_A##H(v1[1], v2[1], v3[1],                 \
		    v5[1]) * (SCL_A##V * SCL_A##H);                \
		r1 = SUM_B##H(v1[1], v2[1], v3[1],                 \
		    v5[1]) * (SCL_A##V * SCL_B##H);                \
		r2 = SUM_A##H(w1[1], w2[1], w3[1],                 \
		    w5[1]) * (SCL_B##V * SCL_A##H);                \
		r3 = SUM_B##H(w1[1], w2[1], w3[1],                 \
		    w5[1]) * (SCL_B##V * SCL_B##H);                \
		MLIB_S_ZOOM_BC_LINE_STORE(STORE_UP, STORE_DN);     \
		pd = dlc + i * 4 * channels + 2;                   \
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
		r0 = SUM_A##H(v1[2], v2[2], v4[0],                 \
		    v6[0]) * (SCL_A##V * SCL_A##H);                \
		r1 = SUM_B##H(v1[2], v2[2], v4[0],                 \
		    v6[0]) * (SCL_A##V * SCL_B##H);                \
		r2 = SUM_A##H(w1[2], w2[2], w4[0],                 \
		    w6[0]) * (SCL_B##V * SCL_A##H);                \
		r3 = SUM_B##H(w1[2], w2[2], w4[0],                 \
		    w6[0]) * (SCL_B##V * SCL_B##H);                \
		MLIB_S_ZOOM_BC_LINE_STORE(STORE_UP, STORE_DN);     \
		pd = dlc + i * 4 * channels + 3;                   \
		r0 = SUM_A##H(v0[3], v1[3], v2[3],                 \
		    v4[1]) * (SCL_A##V * SCL_A##H);                \
		r1 = SUM_B##H(v0[3], v1[3], v2[3],                 \
		    v4[1]) * (SCL_A##V * SCL_B##H);                \
		r2 = SUM_A##H(w0[3], w1[3], w2[3],                 \
		    w4[1]) * (SCL_B##V * SCL_A##H);                \
		r3 = SUM_B##H(w0[3], w1[3], w2[3],                 \
		    w4[1]) * (SCL_B##V * SCL_B##H);                \
		MLIB_S_ZOOM_BC_LINE_STORE(STORE_UP, STORE_DN);     \
		pd += 2 * channels;                                \
		r0 = SUM_A##H(v1[3], v2[3], v4[1],                 \
		    v6[1]) * (SCL_A##V * SCL_A##H);                \
		r1 = SUM_B##H(v1[3], v2[3], v4[1],                 \
		    v6[1]) * (SCL_A##V * SCL_B##H);                \
		r2 = SUM_A##H(w1[3], w2[3], w4[1],                 \
		    w6[1]) * (SCL_B##V * SCL_A##H);                \
		r3 = SUM_B##H(w1[3], w2[3], w4[1],                 \
		    w6[1]) * (SCL_B##V * SCL_B##H);                \
		MLIB_S_ZOOM_BC_LINE_STORE(STORE_UP, STORE_DN);     \
		v0[0] = v2[0];                                     \
		v1[0] = v3[0];                                     \
		v2[0] = v5[0];                                     \
		w0[0] = w2[0];                                     \
		w1[0] = w3[0];                                     \
		w2[0] = w5[0];                                     \
		v0[1] = v2[1];                                     \
		v1[1] = v3[1];                                     \
		v2[1] = v5[1];                                     \
		w0[1] = w2[1];                                     \
		w1[1] = w3[1];                                     \
		w2[1] = w5[1];                                     \
		v0[2] = v2[2];                                     \
		v1[2] = v4[0];                                     \
		v2[2] = v6[0];                                     \
		w0[2] = w2[2];                                     \
		w1[2] = w4[0];                                     \
		w2[2] = w6[0];                                     \
		v0[3] = v2[3];                                     \
		v1[3] = v4[1];                                     \
		v2[3] = v6[1];                                     \
		w0[3] = w2[3];                                     \
		w1[3] = w4[1];                                     \
		w2[3] = w6[1];                                     \
	}                                                          \
	int t = i * 2;                                             \
	if (t < (src_width / 2)) {                                 \
		MLIB_S_MULCONST_D64;                               \
		MLIB_S_ZOOM_BC_LINE_LOAD_D64(                      \
		    V, x_v3_l, x_w3_l, x_v3_h, x_w3_h);            \
		ps += 2;                                           \
		pd = dlc + i * 4 * channels;                       \
		r0 = SUM_A##H(v0[0], v1[0], v2[0],                 \
		    v3[0]) * (SCL_A##V * SCL_A##H);                \
		r1 = SUM_B##H(v0[0], v1[0], v2[0],                 \
		    v3[0]) * (SCL_A##V * SCL_B##H);                \
		r2 = SUM_A##H(w0[0], w1[0], w2[0],                 \
		    w3[0]) * (SCL_B##V * SCL_A##H);                \
		r3 = SUM_B##H(w0[0], w1[0], w2[0],                 \
		    w3[0]) * (SCL_B##V * SCL_B##H);                \
		MLIB_S_ZOOM_BC_LINE_STORE(STORE_UP, STORE_DN);     \
		pd = dlc + i * 4 * channels + 1;                   \
		r0 = SUM_A##H(v0[1], v1[1], v2[1],                 \
		    v3[1]) * (SCL_A##V * SCL_A##H);                \
		r1 = SUM_B##H(v0[1], v1[1], v2[1],                 \
		    v3[1]) * (SCL_A##V * SCL_B##H);                \
		r2 = SUM_A##H(w0[1], w1[1], w2[1],                 \
		    w3[1]) * (SCL_B##V * SCL_A##H);                \
		r3 = SUM_B##H(w0[1], w1[1], w2[1],                 \
		    w3[1]) * (SCL_B##V * SCL_B##H);                \
		MLIB_S_ZOOM_BC_LINE_STORE(STORE_UP, STORE_DN);     \
		pd = dlc + i * 4 * channels + 2;                   \
		r0 = SUM_A##H(v0[2], v1[2], v2[2],                 \
		    v4[0]) * (SCL_A##V * SCL_A##H);                \
		r1 = SUM_B##H(v0[2], v1[2], v2[2],                 \
		    v4[0]) * (SCL_A##V * SCL_B##H);                \
		r2 = SUM_A##H(w0[2], w1[2], w2[2],                 \
		    w4[0]) * (SCL_B##V * SCL_A##H);                \
		r3 = SUM_B##H(w0[2], w1[2], w2[2],                 \
		    w4[0]) * (SCL_B##V * SCL_B##H);                \
		MLIB_S_ZOOM_BC_LINE_STORE(STORE_UP, STORE_DN);     \
		pd = dlc + i * 4 * channels + 3;                   \
		r0 = SUM_A##H(v0[3], v1[3], v2[3],                 \
		    v4[1]) * (SCL_A##V * SCL_A##H);                \
		r1 = SUM_B##H(v0[3], v1[3], v2[3],                 \
		    v4[1]) * (SCL_A##V * SCL_B##H);                \
		r2 = SUM_A##H(w0[3], w1[3], w2[3],                 \
		    w4[1]) * (SCL_B##V * SCL_A##H);                \
		r3 = SUM_B##H(w0[3], w1[3], w2[3],                 \
		    w4[1]) * (SCL_B##V * SCL_B##H);                \
		MLIB_S_ZOOM_BC_LINE_STORE(STORE_UP, STORE_DN);     \
		v0[0] = v1[0];                                     \
		v1[0] = v2[0];                                     \
		v2[0] = v3[0];                                     \
		w0[0] = w1[0];                                     \
		w1[0] = w2[0];                                     \
		w2[0] = w3[0];                                     \
		v0[1] = v1[1];                                     \
		v1[1] = v2[1];                                     \
		v2[1] = v3[1];                                     \
		w0[1] = w1[1];                                     \
		w1[1] = w2[1];                                     \
		w2[1] = w3[1];                                     \
		v0[2] = v1[2];                                     \
		v1[2] = v2[2];                                     \
		v2[2] = v4[0];                                     \
		w0[2] = w1[2];                                     \
		w1[2] = w2[2];                                     \
		w2[2] = w4[0];                                     \
		v0[3] = v1[3];                                     \
		v1[3] = v2[3];                                     \
		v2[3] = v4[1];                                     \
		w0[3] = w1[3];                                     \
		w1[3] = w2[3];                                     \
		w2[3] = w4[1];                                     \
		t++;                                               \
	}                                                          \
	for (k = 0; k < channels; k++) {                           \
		ps = slc + t * channels + k + 12;                  \
		pd = dlc + t * 2 * channels + k;                   \
		if (src_width & 1) {                               \
		v3s[k] = SUM_A##V(ps[-src_stride], ps[0],          \
		    ps[src_stride], ps[2 * src_stride]);           \
		w3s[k] = SUM_B##V(ps[-src_stride], ps[0],          \
		    ps[src_stride], ps[2 * src_stride]);           \
		r0 = SUM_A##H(v0[k], v1[k], v2[k],                 \
		    v3s[k]) * (SCL_A##V * SCL_A##H);               \
		r2 = SUM_A##H(w0[k], w1[k], w2[k],                 \
		    w3s[k]) * (SCL_B##V * SCL_A##H);               \
		STORE_UP(pd[0], r0);                               \
		STORE_DN(pd[dst_stride], r2);                      \
	    }                                                      \
	}                                                          \
	                                                           \
	sl += src_stride;                                          \
	dl += 2 * dst_stride;                                      \
}

/* *********************************************************** */

#define	MLIB_S_ZOOM_BC_LINE(TT, H, V, STORE_UP, STORE_DN)               \
	switch (channels) {                                             \
	case 1:                                                         \
		MLIB_S_ZOOM_BC_LINE_##TT##_1(H, V, STORE_UP, STORE_DN); \
		break;                                                  \
	case 2:                                                         \
		MLIB_S_ZOOM_BC_LINE_##TT##_2(H, V, STORE_UP, STORE_DN); \
		break;                                                  \
	case 3:                                                         \
		MLIB_S_ZOOM_BC_LINE_##TT##_3(H, V, STORE_UP, STORE_DN); \
		break;                                                  \
	case 4:                                                         \
		MLIB_S_ZOOM_BC_LINE_##TT##_4(H, V, STORE_UP, STORE_DN); \
		break;                                                  \
	}

/* *********************************************************** */

#define	MLIB_S_FUNC_ZOOM_BC(TT, STYPE, N, H, V)                 \
	void                                                    \
	mlib_ImageZoomIn2X_##TT##_BC2_FL##N(STYPE * pdst,       \
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
		MLIB_S_ZOOM_BC_LINE(TT, H, V,                   \
		    SKIP, STORE_##TT);                          \
	    }                                                   \
	    for (j = 0; j < (src_height / 2); j++) {            \
		MLIB_S_ZOOM_BC_LINE(TT, H, V,                   \
		    STORE_##TT, STORE_##TT);                    \
	    }                                                   \
	    if (src_height & 1) {                               \
		MLIB_S_ZOOM_BC_LINE(TT, H, V,                   \
		    STORE_##TT, SKIP);                          \
	    }                                                   \
	}

/* *********************************************************** */

#define	TTYPE	mlib_d64

MLIB_S_FUNC_ZOOM_BC(D64, mlib_d64,
	0,
	0,
	0)
MLIB_S_FUNC_ZOOM_BC(
	D64,
	mlib_d64,
	1,
	0,
	1)
MLIB_S_FUNC_ZOOM_BC(
	D64,
	mlib_d64,
	2,
	1,
	0)
MLIB_S_FUNC_ZOOM_BC(
	D64,
	mlib_d64,
	3,
	1,
	1)

/* *********************************************************** */
#undef  TTYPE
#define	TTYPE	mlib_f32
MLIB_S_FUNC_ZOOM_BC(
	F32,
	mlib_f32,
	0,
	0,
	0)
MLIB_S_FUNC_ZOOM_BC(
	F32,
	mlib_f32,
	1,
	0,
	1)
MLIB_S_FUNC_ZOOM_BC(
	F32,
	mlib_f32,
	2,
	1,
	0)
MLIB_S_FUNC_ZOOM_BC(
	F32,
	mlib_f32,
	3,
	1,
	1)

/* *********************************************************** */
