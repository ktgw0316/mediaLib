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

#pragma ident	"@(#)mlib_c_ImageZoomIndex_BL.c	9.2	07/10/09 SMI"

/*
 * FUNCTION
 *      mlib_ImageZoomIndex - indexed color image scaling
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageZoomIndex(mlib_image       *dst,
 *                                      const mlib_image *src,
 *                                      mlib_f32         zoomx,
 *                                      mlib_f32         zoomy,
 *                                      mlib_filter      filter,
 *                                      mlib_edge        edge,
 *                                      const void       *colormap)
 *
 * ARGUMENTS
 *      dst       Pointer to destination image
 *      src       Pointer to source image
 *      zoomx     X zoom factor.
 *      zoomy     Y zoom factor.
 *      filter    Type of resampling filter.
 *      edge      Type of edge conditions.
 *      colormap  Internal structure.
 *
 * DESCRIPTION
 *  The center of the source image is mapped to the center of the
 *  destination image.
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
#include <mlib_ImageColormap.h>

/* *********************************************************** */

#define	_MLIB_C_VER_

#include <mlib_ImageZoom.h>

/* *********************************************************** */

#define	FTABLE(A)	mlib_c_blend_Q8[(A)]

/* *********************************************************** */

#define	_PAL_U8_

#define	_IDX_U8_

/* *********************************************************** */

#define	_C_BILINEAR_U8_U8_3_

#include <mlib_c_ImageZoomIndex_BL.h>

#undef   _C_BILINEAR_U8_U8_3_

/* *********************************************************** */

#define	_C_BILINEAR_U8_U8_4_

#include <mlib_c_ImageZoomIndex_BL.h>

#undef   _C_BILINEAR_U8_U8_4_

#undef   _PAL_U8_
#undef   _IDX_U8_

/* *********************************************************** */

#define	_C_BILINEAR_S16_S16_3_

#include <mlib_c_ImageZoomIndex_BL.h>

#undef   _C_BILINEAR_S16_S16_3_

/* *********************************************************** */

#define	_C_BILINEAR_S16_S16_4_

#include <mlib_c_ImageZoomIndex_BL.h>

#undef   _C_BILINEAR_S16_S16_4_

#define	_IDX_U8_

/* *********************************************************** */

#define	_C_BILINEAR_U8_S16_3_

#include <mlib_c_ImageZoomIndex_BL.h>

#undef   _C_BILINEAR_U8_S16_3_

/* *********************************************************** */

#define	_C_BILINEAR_U8_S16_4_

#include <mlib_c_ImageZoomIndex_BL.h>

#undef   _C_BILINEAR_U8_S16_4_

/* *********************************************************** */

#undef   _IDX_U8_
#define	_PAL_U8_

/* *********************************************************** */

#define	_C_BILINEAR_S16_U8_3_

#include <mlib_c_ImageZoomIndex_BL.h>

#undef   _C_BILINEAR_S16_U8_3_

/* *********************************************************** */

#define	_C_BILINEAR_S16_U8_4_

#include <mlib_c_ImageZoomIndex_BL.h>

#undef   _C_BILINEAR_S16_U8_4_

/* *********************************************************** */
