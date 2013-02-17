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

#ifndef _MLIB_S_IMAGECONVINDEX_H
#define	_MLIB_S_IMAGECONVINDEX_H

#pragma ident	"@(#)mlib_s_ImageConvIndex.h	9.3	07/11/05 SMI"

#ifdef __cplusplus
extern "C" {
#endif

#include <mlib_types.h>
#include <mlib_image_types.h>
#include <mlib_status.h>

mlib_status mlib_ImageConv2x2Index(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_s32 *kernel,
    mlib_s32 scale,
    mlib_edge edge,
    const void *colormap);

mlib_status mlib_conv2x2_Index3_8_16ext(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_s32 *kern,
    mlib_s32 scale,
    const void *colormap);

mlib_status mlib_conv3x3_Index3_8_16ext(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_s32 *kern,
    mlib_s32 scale,
    const void *colormap);

mlib_status mlib_convMxN_Index3_8_16ext(
    mlib_image *dst,
    const mlib_image *src,
    mlib_s32 m,
    mlib_s32 n,
    mlib_s32 dx_l,
    mlib_s32 dx_r,
    mlib_s32 dy_t,
    mlib_s32 dy_b,
    const mlib_s32 *kern,
    mlib_s32 scale,
    const void *colormap);

mlib_status mlib_conv2x2_Index3_8_16nw(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_s32 *kern,
    mlib_s32 scale,
    const void *colormap);

mlib_status mlib_conv3x3_Index3_8_16nw(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_s32 *kern,
    mlib_s32 scale,
    const void *colormap);

mlib_status mlib_convMxN_Index3_8_16nw(
    mlib_image *dst,
    const mlib_image *src,
    mlib_s32 m,
    mlib_s32 n,
    mlib_s32 dm,
    mlib_s32 dn,
    const mlib_s32 *kern,
    mlib_s32 scale,
    const void *colormap);

mlib_status mlib_conv2x2_Index3_8_8ext(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_s32 *kern,
    mlib_s32 scale,
    const void *colormap);

mlib_status mlib_conv3x3_Index3_8_8ext(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_s32 *kern,
    mlib_s32 scale,
    const void *colormap);

mlib_status mlib_convMxN_Index3_8_8ext(
    mlib_image *dst,
    const mlib_image *src,
    mlib_s32 m,
    mlib_s32 n,
    mlib_s32 dx_l,
    mlib_s32 dx_r,
    mlib_s32 dy_t,
    mlib_s32 dy_b,
    const mlib_s32 *kern,
    mlib_s32 scale,
    const void *colormap);

mlib_status mlib_conv2x2_Index3_8_8nw(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_s32 *kern,
    mlib_s32 scale,
    const void *colormap);

mlib_status mlib_conv3x3_Index3_8_8nw(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_s32 *kern,
    mlib_s32 scale,
    const void *colormap);

mlib_status mlib_convMxN_Index3_8_8nw(
    mlib_image *dst,
    const mlib_image *src,
    mlib_s32 m,
    mlib_s32 n,
    mlib_s32 dm,
    mlib_s32 dn,
    const mlib_s32 *kern,
    mlib_s32 scale,
    const void *colormap);

mlib_status mlib_conv2x2_Index4_8_16ext(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_s32 *kern,
    mlib_s32 scale,
    const void *colormap);

mlib_status mlib_conv3x3_Index4_8_16ext(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_s32 *kern,
    mlib_s32 scale,
    const void *colormap);

mlib_status mlib_convMxN_Index4_8_16ext(
    mlib_image *dst,
    const mlib_image *src,
    mlib_s32 m,
    mlib_s32 n,
    mlib_s32 dx_l,
    mlib_s32 dx_r,
    mlib_s32 dy_t,
    mlib_s32 dy_b,
    const mlib_s32 *kern,
    mlib_s32 scale,
    const void *colormap);

mlib_status mlib_conv2x2_Index4_8_16nw(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_s32 *kern,
    mlib_s32 scale,
    const void *colormap);

mlib_status mlib_conv3x3_Index4_8_16nw(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_s32 *kern,
    mlib_s32 scale,
    const void *colormap);

mlib_status mlib_convMxN_Index4_8_16nw(
    mlib_image *dst,
    const mlib_image *src,
    mlib_s32 m,
    mlib_s32 n,
    mlib_s32 dm,
    mlib_s32 dn,
    const mlib_s32 *kern,
    mlib_s32 scale,
    const void *colormap);

mlib_status mlib_conv2x2_Index4_8_8ext(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_s32 *kern,
    mlib_s32 scale,
    const void *colormap);

mlib_status mlib_conv3x3_Index4_8_8ext(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_s32 *kern,
    mlib_s32 scale,
    const void *colormap);

mlib_status mlib_convMxN_Index4_8_8ext(
    mlib_image *dst,
    const mlib_image *src,
    mlib_s32 m,
    mlib_s32 n,
    mlib_s32 dx_l,
    mlib_s32 dx_r,
    mlib_s32 dy_t,
    mlib_s32 dy_b,
    const mlib_s32 *kern,
    mlib_s32 scale,
    const void *colormap);

mlib_status mlib_conv2x2_Index4_8_8nw(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_s32 *kern,
    mlib_s32 scale,
    const void *colormap);

mlib_status mlib_conv3x3_Index4_8_8nw(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_s32 *kern,
    mlib_s32 scale,
    const void *colormap);

mlib_status mlib_convMxN_Index4_8_8nw(
    mlib_image *dst,
    const mlib_image *src,
    mlib_s32 m,
    mlib_s32 n,
    mlib_s32 dm,
    mlib_s32 dn,
    const mlib_s32 *kern,
    mlib_s32 scale,
    const void *colormap);

#ifdef __cplusplus
}
#endif

#endif /* _MLIB_S_IMAGECONVINDEX_H */
