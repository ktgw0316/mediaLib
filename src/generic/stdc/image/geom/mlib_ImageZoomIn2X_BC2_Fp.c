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

#pragma ident	"@(#)mlib_ImageZoomIn2X_BC2_Fp.c	9.2	07/10/09 SMI"

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

#define	SUM_A0(a, b, c, d)	((TTYPE)57*b - (TTYPE)9*a + \
				(TTYPE)19*c - (TTYPE)3*d)
#define	SUM_B0(a, b, c, d)	((TTYPE)19*b - (TTYPE)3*a + \
				(TTYPE)57*c - (TTYPE)9*d)

#define	SCL_A0	(TTYPE)0.015625

#define	SCL_B0	(TTYPE)0.015625

/* *********************************************************** */

#define	SUM_A1(a, b, c, d)	((TTYPE)b)
#define	SUM_B1(a, b, c, d)	((TTYPE)5*b + (TTYPE)5*c - a - d)

#define	SCL_A1	(TTYPE)1.0

#define	SCL_B1	(TTYPE)0.125

/* *********************************************************** */

#define	ZOOM_BC_LINE(H, V, STORE_UP, STORE_DN)                      \
	for (k = 0; k < channels; k++) {                            \
	    TTYPE v0, v1, v2, v3, w0, w1, w2, w3;                   \
	                                                            \
	    ps = sl + k;                                            \
	    pd = dl + k;                                            \
	    v0 = SUM_A##V(ps[-src_stride], ps[0], ps[src_stride],   \
		    ps[2 * src_stride]);                            \
	    w0 = SUM_B##V(ps[-src_stride], ps[0], ps[src_stride],   \
		    ps[2 * src_stride]);                            \
	    ps += channels;                                         \
	    v1 = SUM_A##V(ps[-src_stride], ps[0], ps[src_stride],   \
		    ps[2 * src_stride]);                            \
	    w1 = SUM_B##V(ps[-src_stride], ps[0], ps[src_stride],   \
		    ps[2 * src_stride]);                            \
	    ps += channels;                                         \
	    v2 = SUM_A##V(ps[-src_stride], ps[0], ps[src_stride],   \
		    ps[2 * src_stride]);                            \
	    w2 = SUM_B##V(ps[-src_stride], ps[0], ps[src_stride],   \
		    ps[2 * src_stride]);                            \
	    ps += channels;                                         \
	    if (src_w_beg) {                                        \
		v3 = SUM_A##V(ps[-src_stride], ps[0],               \
			ps[src_stride], ps[2 * src_stride]);        \
		w3 = SUM_B##V(ps[-src_stride], ps[0],               \
			ps[src_stride], ps[2 * src_stride]);        \
		ps += channels;                                     \
		r1 = SUM_B##H(v0, v1, v2,                           \
			v3) * (SCL_A##V * SCL_B##H);                \
		r3 = SUM_B##H(w0, w1, w2,                           \
			w3) * (SCL_B##V * SCL_B##H);                \
		STORE_UP(pd[0], r1);                                \
		STORE_DN(pd[dst_stride], r3);                       \
		pd += channels;                                     \
		v0 = v1;                                            \
		v1 = v2;                                            \
		v2 = v3;                                            \
		w0 = w1;                                            \
		w1 = w2;                                            \
		w2 = w3;                                            \
	    }                                                       \
	    for (i = 0; i < (src_width / 2); i++) {                 \
		v3 = SUM_A##V(ps[-src_stride], ps[0],               \
			ps[src_stride], ps[2 * src_stride]);        \
		w3 = SUM_B##V(ps[-src_stride], ps[0],               \
			ps[src_stride], ps[2 * src_stride]);        \
		ps += channels;                                     \
		r0 = SUM_A##H(v0, v1, v2,                           \
			v3) * (SCL_A##V * SCL_A##H);                \
		r1 = SUM_B##H(v0, v1, v2,                           \
			v3) * (SCL_A##V * SCL_B##H);                \
		r2 = SUM_A##H(w0, w1, w2,                           \
			w3) * (SCL_B##V * SCL_A##H);                \
		r3 = SUM_B##H(w0, w1, w2,                           \
			w3) * (SCL_B##V * SCL_B##H);                \
		STORE_UP(pd[0], r0);                                \
		STORE_UP(pd[channels], r1);                         \
		STORE_DN(pd[dst_stride], r2);                       \
		STORE_DN(pd[dst_stride + channels], r3);            \
		pd += 2 * channels;                                 \
		v0 = v1;                                            \
		v1 = v2;                                            \
		v2 = v3;                                            \
		w0 = w1;                                            \
		w1 = w2;                                            \
		w2 = w3;                                            \
	    }                                                       \
	    if (src_width & 1) {                                    \
		v3 = SUM_A##V(ps[-src_stride], ps[0],               \
			ps[src_stride], ps[2 * src_stride]);        \
		w3 = SUM_B##V(ps[-src_stride], ps[0],               \
			ps[src_stride], ps[2 * src_stride]);        \
		r0 = SUM_A##H(v0, v1, v2,                           \
			v3) * (SCL_A##V * SCL_A##H);                \
		r2 = SUM_A##H(w0, w1, w2,                           \
			w3) * (SCL_B##V * SCL_A##H);                \
		STORE_UP(pd[0], r0);                                \
		STORE_DN(pd[dst_stride], r2);                       \
	    }                                                       \
	}                                                           \
	                                                            \
	sl += src_stride;                                           \
	dl += 2 * dst_stride

/* *********************************************************** */

#define	FUNC_ZOOM_BC(TT, STYPE, N, H, V)                        \
	void                                                    \
	mlib_ImageZoomIn2X_##TT##_BC2_FL##N(STYPE * pdst,       \
		const STYPE * psrc, mlib_s32 src_width,         \
		mlib_s32 src_height, mlib_s32 dst_stride,       \
		mlib_s32 src_stride, mlib_s32 src_w_beg,        \
		mlib_s32 src_h_beg, mlib_s32 channels)          \
	{                                                       \
	    mlib_s32 i, j, k;                                   \
	    STYPE *pd;                                          \
	    const STYPE *ps, *sl = psrc - channels;             \
	    STYPE *dl = pdst;                                   \
	    TTYPE r0, r1, r2, r3;                               \
	                                                        \
	    src_w_beg &= 1;                                     \
	    src_h_beg &= 1;                                     \
	    src_width -= src_w_beg;                             \
	    src_height -= src_h_beg;                            \
	    if (src_h_beg) {                                    \
		dl -= dst_stride;                               \
		ZOOM_BC_LINE(H, V, SKIP, STORE_##TT);           \
	    }                                                   \
	    for (j = 0; j < (src_height / 2); j++) {            \
		ZOOM_BC_LINE(H, V, STORE_##TT, STORE_##TT);     \
	    }                                                   \
	    if (src_height & 1) {                               \
		ZOOM_BC_LINE(H, V, STORE_##TT, SKIP);           \
	    }                                                   \
	}

/* *********************************************************** */

#define	TTYPE	mlib_d64

FUNC_ZOOM_BC(D64, mlib_d64,
	0,
	0,
	0)
FUNC_ZOOM_BC(
	D64,
	mlib_d64,
	1,
	0,
	1)
FUNC_ZOOM_BC(
	D64,
	mlib_d64,
	2,
	1,
	0)
FUNC_ZOOM_BC(
	D64,
	mlib_d64,
	3,
	1,
	1)

/* *********************************************************** */
#undef  TTYPE
#define	TTYPE	mlib_f32
FUNC_ZOOM_BC(
	F32,
	mlib_f32,
	0,
	0,
	0)
FUNC_ZOOM_BC(
	F32,
	mlib_f32,
	1,
	0,
	1)
FUNC_ZOOM_BC(
	F32,
	mlib_f32,
	2,
	1,
	0)
FUNC_ZOOM_BC(
	F32,
	mlib_f32,
	3,
	1,
	1)

/* *********************************************************** */
