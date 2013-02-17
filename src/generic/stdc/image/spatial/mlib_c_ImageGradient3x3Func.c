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

#pragma ident	"@(#)mlib_c_ImageGradient3x3Func.c	9.2	07/10/09 SMI"

/*
 * FUNCTION
 *      mlib_ImageGradient3x3   - Compute gradient magnitude of an image
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageGradient3x3(mlib_image       *dst,
 *                                        const mlib_image *src,
 *                                        const mlib_d64   *hmask,
 *                                        const mlib_d64   *vmask,
 *                                        mlib_s32         cmask,
 *                                        mlib_edge        edge)
 *
 * ARGUMENTS
 *      dst       Pointer to destination image.
 *      src       Pointer to source image.
 *      hmask     Pointer to horizontal mask in row-major order.
 *      vmask     Pointer to vertical mask in row-major order.
 *      top-left element in x and y directions respectively).
 *      cmask     Channel mask to indicate the channels to be processed.
 *                Each bit of which represents a channel in the image. The
 *                channels corresponded to 1 bits are those to be processed.
 *      edge      Type of edge condition.
 *
 * RESTRICTION
 *
 *  The src and the dst must be images of the same type.
 *  For the integral type function, the src and the dst can be images of
 *  MLIB_BYTE, MLIB_SHORT, MLIB_USHORT or MLIB_INT type.
 *
 *  The operation is applied on a per-channel basis.
 *  The src and dst must have same number of channels (1, 2, 3, or 4).
 *
 *  The unselected channels are not overwritten. If both src and dst have
 *  just one channel, cmask is ignored.
 *
 *  The mapping of source image to destination image is left/top corner to
 *  left/top corner, but with a shift of the destination image if it's
 *  smaller than the source image.
 *  The exact mapping formula is
 *       xs = xd + x_offset
 *       ys = yd + y_offset
 *  where
 *       x_offset = (dst_width >= src_width) ? 0 : (kernel_size - 1)/2
 *       y_offset = (dst_height >= src_height) ? 0 : (kernel_size - 1)/2
 *
 *  The edge condition can be one of the following:
 *    MLIB_EDGE_DST_NO_WRITE  (default)
 *    MLIB_EDGE_DST_FILL_ZERO
 *    MLIB_EDGE_DST_COPY_SRC
 *    MLIB_EDGE_SRC_EXTEND
 *
 * DESCRIPTION
 *
 *  The "Gradient3x3" operation is an edge detector which
 *  computes the magnitude of the image gradient vector in two
 *  orthogonal directions.
 *
 *  The result of the "Gradient3x3" operation may be defined as:
 *
 *      dst[x][y][c] = ((SH(x, y,c))^2 + (SV(x, y,c))^2 )^0.5
 *
 *  where SH(x, y,c) and SV(x, y,c) are the horizontal and vertical
 *  gradient images generated from channel c of the source image by
 *  correlating it with the supplied orthogonal (horizontal and
 *  vertical) gradient masks.
 */

#include <mlib_image.h>
#include <mlib_ImageGradientFunc.h>

/* *********************************************************** */

mlib_status
mlib_ImageGradientKernVIS(
    mlib_d64 *dhmask,
    mlib_d64 *dvmask,
    mlib_s32 *iscale,
    const mlib_d64 *hmask,
    const mlib_d64 *vmask,
    mlib_d64 *offset,
    mlib_type type,
    mlib_s32 m,
    mlib_s32 n)
{
	return (MLIB_FAILURE);
}

/* *********************************************************** */

/* no VIS, empty functions */

void
mlib_ImageGradient3x3_U8_fast(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_d64 *hmask,
    const mlib_d64 *vmask,
    mlib_s32 *scale,
    const mlib_u8 *acmask,
    mlib_d64 *dsa)
{
}

/* *********************************************************** */

void
mlib_ImageGradient3x3_U8_ext_fast(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_d64 *hmask,
    const mlib_d64 *vmask,
    mlib_s32 *scale,
    mlib_s32 dx_l,
    mlib_s32 dx_r,
    mlib_s32 dy_t,
    mlib_s32 dy_b,
    const mlib_u8 *acmask,
    mlib_d64 *dsa)
{
}

/* *********************************************************** */
