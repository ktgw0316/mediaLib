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

#pragma ident	"@(#)mlib_c_ImageZoomIn2X_BC2.c	9.2	07/10/09 SMI"

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

#define	ZOOM_BC2_LINE(H, V, STORE_SAT)                                 \
	for (k = 0; k < channels; k++) {                               \
	    TTYPE v0, v1, v2, v3;                                      \
	    TTYPE r0, r1;                                              \
	                                                               \
	    ps = sl + k;                                               \
	    pd = dl + k;                                               \
	    v0 = SUM_##V(ps[-src_stride], ps[0], ps[src_stride],       \
		ps[2 * src_stride]);                                   \
	    ps += channels;                                            \
	    v1 = SUM_##V(ps[-src_stride], ps[0], ps[src_stride],       \
		ps[2 * src_stride]);                                   \
	    ps += channels;                                            \
	    v2 = SUM_##V(ps[-src_stride], ps[0], ps[src_stride],       \
		ps[2 * src_stride]);                                   \
	    ps += channels;                                            \
	    if (src_w_beg) {                                           \
		v3 = SUM_##V(ps[-src_stride], ps[0], ps[src_stride],   \
		    ps[2 * src_stride]);                               \
		ps += channels;                                        \
		r1 = SUM_B##H(v0, v1, v2,                              \
		    v3) >> (SCL_##V + SCL_B##H);                       \
		STORE_SAT(pd[0], r1);                                  \
		pd += channels;                                        \
		v0 = v1;                                               \
		v1 = v2;                                               \
		v2 = v3;                                               \
	    }                                                          \
	    for (i = 0; i < (src_width / 2); i++) {                    \
		v3 = SUM_##V(ps[-src_stride], ps[0], ps[src_stride],   \
		    ps[2 * src_stride]);                               \
		ps += channels;                                        \
		r0 = SUM_A##H(v0, v1, v2,                              \
		    v3) >> (SCL_##V + SCL_A##H);                       \
		r1 = SUM_B##H(v0, v1, v2,                              \
		    v3) >> (SCL_##V + SCL_B##H);                       \
		STORE_SAT(pd[0], r0);                                  \
		STORE_SAT(pd[channels], r1);                           \
		pd += 2 * channels;                                    \
		v0 = v1;                                               \
		v1 = v2;                                               \
		v2 = v3;                                               \
	    }                                                          \
	    if (src_width & 1) {                                       \
		v3 = SUM_##V(ps[-src_stride], ps[0], ps[src_stride],   \
		    ps[2 * src_stride]);                               \
		r0 = SUM_A##H(v0, v1, v2,                              \
		    v3) >> (SCL_##V + SCL_A##H);                       \
		STORE_SAT(pd[0], r0);                                  \
	    }                                                          \
	}                                                              \
	                                                               \
	dl += dst_stride

/* *********************************************************** */

#define	FUNC_ZOOM_BC2(TT, STYPE, N, H, V)                                  \
	void                                                               \
	mlib_c_ImageZoomIn2X_##TT##_BC2_FL##N(STYPE * pdst,                \
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
		    ZOOM_BC2_LINE(H, A##V, STORE_##TT);                    \
		} else {                                                   \
		    ZOOM_BC2_LINE(H, B##V, STORE_##TT);                    \
		    sl += src_stride;                                      \
		}                                                          \
	    }                                                              \
	}

/* *********************************************************** */

#define	TTYPE	mlib_s32

FUNC_ZOOM_BC2(U8, mlib_u8,
    0,
    0,
    0)
FUNC_ZOOM_BC2(
    U8,
    mlib_u8,
    1,
    0,
    1)
FUNC_ZOOM_BC2(
    U8,
    mlib_u8,
    2,
    1,
    0)
FUNC_ZOOM_BC2(
    U8,
    mlib_u8,
    3,
    1,
    1)

FUNC_ZOOM_BC2(
    S16,
    mlib_s16,
    0,
    0,
    0)
FUNC_ZOOM_BC2(
    S16,
    mlib_s16,
    1,
    0,
    1)
FUNC_ZOOM_BC2(
    S16,
    mlib_s16,
    2,
    1,
    0)
FUNC_ZOOM_BC2(
    S16,
    mlib_s16,
    3,
    1,
    1)

FUNC_ZOOM_BC2(
    U16,
    mlib_u16,
    0,
    0,
    0)
FUNC_ZOOM_BC2(
    U16,
    mlib_u16,
    1,
    0,
    1)
FUNC_ZOOM_BC2(
    U16,
    mlib_u16,
    2,
    1,
    0)
FUNC_ZOOM_BC2(
    U16,
    mlib_u16,
    3,
    1,
    1)

/* *********************************************************** */
