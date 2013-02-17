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

#pragma ident	"@(#)mlib_v_ImageZoomIn2X_BC.c	9.2	07/11/05 SMI"

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
#include <mlib_v_ImageZoomIn2X.h>

/* *********************************************************** */

#define	STYPE	mlib_u8

#define	FTYPE	mlib_f32

#define	LOAD_NF(sp)	vis_ld_f32_nf(sp)

/* *********************************************************** */

#define	DEF_CONST_V0()                                                 \
	mlib_d64 q =                                                   \
	vis_to_double((111 << 23) | (9 << 7), (29 << 23) | (3 << 7))

/* *********************************************************** */

#define	DEF_CONST_V1()                                                  \
	mlib_d64 q = vis_to_double((9 << 26) | (1 << 10), (16 << 10))

/* *********************************************************** */

#define	DEF_CONST_H0()                                          \
	mlib_f32 g111 = vis_to_float(0xDEDEDEDE);               \
	mlib_f32 g29 = vis_to_float(0x3A3A3A3A);                \
	mlib_f32 g9 = vis_to_float(0x12121212);                 \
	mlib_f32 g3 = vis_to_float(0x06060606)

/* *********************************************************** */

#define	DEF_CONST_H1()                                          \
	mlib_f32 g9 = vis_to_float(0x90909090);                 \
	mlib_f32 g1 = vis_to_float(0x10101010)

/* *********************************************************** */

#define	SUMV_A0(a, b, c, d)                                          \
	vis_fpadd16(vis_fpsub16(vis_fmul8x16au(b, vis_read_hi(q)),   \
	    vis_fmul8x16al(a, vis_read_hi(q))),                      \
	    vis_fpsub16(vis_fmul8x16au(c, vis_read_lo(q)),           \
	    vis_fmul8x16al(d, vis_read_lo(q))))

/* *********************************************************** */

#define	SUMV_B0(a, b, c, d)                                          \
	vis_fpadd16(vis_fpsub16(vis_fmul8x16au(b, vis_read_lo(q)),   \
	    vis_fmul8x16al(a, vis_read_lo(q))),                      \
	    vis_fpsub16(vis_fmul8x16au(c, vis_read_hi(q)),           \
	    vis_fmul8x16al(d, vis_read_hi(q))))

/* *********************************************************** */

#define	SUMV_A1(a, b, c, d)	vis_fmul8x16al(b, vis_read_lo(q))

/* *********************************************************** */

#define	SUMV_B1(a, b, c, d)                                          \
	vis_fpadd16(vis_fpsub16(vis_fmul8x16au(b, vis_read_hi(q)),   \
	    vis_fmul8x16al(a, vis_read_hi(q))),                      \
	    vis_fpsub16(vis_fmul8x16au(c, vis_read_hi(q)),           \
	    vis_fmul8x16al(d, vis_read_hi(q))))

/* *********************************************************** */

#define	SUMH_A0(a, b, c, d)                                                \
	vis_fpadd16(vis_fpsub16(vis_fmul8x16(g111, b), vis_fmul8x16(g9,    \
	    a)), vis_fpsub16(vis_fmul8x16(g29, c), vis_fmul8x16(g3, d)))

/* *********************************************************** */

#define	SUMH_B0(a, b, c, d)                                              \
	vis_fpadd16(vis_fpsub16(vis_fmul8x16(g29, b), vis_fmul8x16(g3,   \
	    a)), vis_fpsub16(vis_fmul8x16(g111, c), vis_fmul8x16(g9, d)))

#define	SUMH_A1(a, b, c, d)	(b)

/* *********************************************************** */

#define	SUMH_B1(a, b, c, d)                                               \
	vis_fpadd16(vis_fpsub16(vis_fmul8x16(g9, b), vis_fmul8x16(g1,     \
	    a)), vis_fpsub16(vis_fmul8x16(g9, c), vis_fmul8x16(g1, d)))

/* *********************************************************** */

    FUNC_ZOOM_BC(BC, U8, 1, 0, 0)
    FUNC_ZOOM_BC(BC, U8, 1, 0, 1)
    FUNC_ZOOM_BC(BC, U8, 1, 1, 0)
    FUNC_ZOOM_BC(BC, U8, 1, 1, 1)
    FUNC_ZOOM_BC(BC, U8, 2, 0, 0)
    FUNC_ZOOM_BC(BC, U8, 2, 0, 1)
    FUNC_ZOOM_BC(BC, U8, 2, 1, 0)
    FUNC_ZOOM_BC(BC, U8, 2, 1, 1)
    FUNC_ZOOM_BC(BC, U8, 3, 0, 0)
    FUNC_ZOOM_BC(BC, U8, 3, 0, 1)
    FUNC_ZOOM_BC(BC, U8, 3, 1, 0)
    FUNC_ZOOM_BC(BC, U8, 3, 1, 1)
    FUNC_ZOOM_BC(BC, U8, 4, 0, 0)
    FUNC_ZOOM_BC(BC, U8, 4, 0, 1)
    FUNC_ZOOM_BC(BC, U8, 4, 1, 0)
    FUNC_ZOOM_BC(BC, U8, 4, 1, 1)

/* *********************************************************** */
