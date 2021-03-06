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

#pragma ident	"@(#)mlib_ImageZoomOut2X_NN_Fp.c	9.2	07/10/09 SMI"

/*
 * FUNCTION
 *      mlib_ImageZoomOut2X -  image 0.5X scaling with edge condition
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageZoomOut2X(mlib_image       *dst,
 *                                      const mlib_image *src,
 *                                      mlib_filter      filter,
 *                                      mlib_edge        edge)
 *
 * ARGUMENTS
 *      dst       Pointer to destination image.
 *      src       Pointer to source image.
 *      filter    Type of resampling filter.
 *      edge    Type of edge condition.
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

#include <mlib_ImageZoomOut2X.h>

#define	DTYPE	mlib_d64

#define	FUNC_NAME(SUFF)	mlib_ImageZoomOut2X_D64_##SUFF

/* *********************************************************** */

void FUNC_NAME(
    1_Nearest) (
    NEAREST_PARAMETERS(DTYPE))
{
	mlib_s32 i, j;

	for (j = 0; j < dst_height; j++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < dst_width; i++) {
			p_dst[i] = p_src[2 * i];
		}

		p_src += 2 * src_stride;
		p_dst += dst_stride;
	}
}

/* *********************************************************** */

void FUNC_NAME(
    2_Nearest) (
    NEAREST_PARAMETERS(DTYPE))
{
	mlib_s32 i, j;

	for (j = 0; j < dst_height; j++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < dst_width; i++) {
			p_dst[2 * i] = p_src[4 * i];
			p_dst[2 * i + 1] = p_src[4 * i + 1];
		}

		p_src += 2 * src_stride;
		p_dst += dst_stride;
	}
}

/* *********************************************************** */

void FUNC_NAME(
    3_Nearest) (
    NEAREST_PARAMETERS(DTYPE))
{
	mlib_s32 i, j;

	for (j = 0; j < dst_height; j++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < dst_width; i++) {
			p_dst[3 * i] = p_src[6 * i];
			p_dst[3 * i + 1] = p_src[6 * i + 1];
			p_dst[3 * i + 2] = p_src[6 * i + 2];
		}

		p_src += 2 * src_stride;
		p_dst += dst_stride;
	}
}

/* *********************************************************** */

void FUNC_NAME(
    4_Nearest) (
    NEAREST_PARAMETERS(DTYPE))
{
	mlib_s32 i, j;

	for (j = 0; j < dst_height; j++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < dst_width; i++) {
			p_dst[4 * i] = p_src[8 * i];
			p_dst[4 * i + 1] = p_src[8 * i + 1];
			p_dst[4 * i + 2] = p_src[8 * i + 2];
			p_dst[4 * i + 3] = p_src[8 * i + 3];
		}

		p_src += 2 * src_stride;
		p_dst += dst_stride;
	}
}

/* *********************************************************** */
