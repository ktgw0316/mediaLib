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

#ifndef _MLIB_IMAGECONVINDEX_H
#define	_MLIB_IMAGECONVINDEX_H

#pragma ident	"@(#)mlib_ImageConvIndex.h	9.2	07/10/09 SMI"

#ifdef __cplusplus
extern "C" {
#endif

#include <mlib_types.h>
#include <mlib_image_types.h>
#include <mlib_status.h>

/* *********************************************************** */

typedef mlib_status (*ftype_convMxNIndex_nw)(
    const mlib_image *src,
    mlib_image *dst,
    mlib_s32 m,
    mlib_s32 n,
    mlib_s32 dm,
    mlib_s32 dn,
    const mlib_s32 *kern,
    mlib_s32 scalef_expon,
    const void *colormap);

typedef mlib_status (*ftype_convMxNIndex_ext)(
    const mlib_image *src,
    mlib_image *dst,
    mlib_s32 m,
    mlib_s32 n,
    mlib_s32 dx_l,
    mlib_s32 dx_r,
    mlib_s32 dy_t,
    mlib_s32 dy_b,
    const mlib_s32 *kern,
    mlib_s32 scalef_expon,
    const void *colormap);

/* *********************************************************** */

extern ftype_convMxNIndex_nw  func_convMxNIndex_nw[];
extern ftype_convMxNIndex_ext func_convMxNIndex_ext[];

/* *********************************************************** */

mlib_status mlib_conv2x2Index_16_16ext(
    const mlib_image *src,
    mlib_image *dst,
    const mlib_s32 *kern,
    mlib_s32 scalef_expon,
    const void *colormap);

mlib_status mlib_conv3x3Index_16_16ext(
    const mlib_image *src,
    mlib_image *dst,
    const mlib_s32 *kern,
    mlib_s32 scalef_expon,
    const void *colormap);

mlib_status mlib_conv4x4Index_16_16ext(
    const mlib_image *src,
    mlib_image *dst,
    const mlib_s32 *kern,
    mlib_s32 scalef_expon,
    const void *colormap);

mlib_status mlib_conv5x5Index_16_16ext(
    const mlib_image *src,
    mlib_image *dst,
    const mlib_s32 *kern,
    mlib_s32 scalef_expon,
    const void *colormap);

mlib_status mlib_convMxNIndex_16_16ext(
    const mlib_image *src,
    mlib_image *dst,
    mlib_s32 m,
    mlib_s32 n,
    mlib_s32 dx_l,
    mlib_s32 dx_r,
    mlib_s32 dy_t,
    mlib_s32 dy_b,
    const mlib_s32 *kern,
    mlib_s32 scalef_expon,
    const void *colormap);

mlib_status mlib_conv2x2Index_16_16nw(
    const mlib_image *src,
    mlib_image *dst,
    const mlib_s32 *kern,
    mlib_s32 scalef_expon,
    const void *colormap);

mlib_status mlib_conv3x3Index_16_16nw(
    const mlib_image *src,
    mlib_image *dst,
    const mlib_s32 *kern,
    mlib_s32 scalef_expon,
    const void *colormap);

mlib_status mlib_conv4x4Index_16_16nw(
    const mlib_image *src,
    mlib_image *dst,
    const mlib_s32 *kern,
    mlib_s32 scalef_expon,
    const void *colormap);

mlib_status mlib_conv5x5Index_16_16nw(
    const mlib_image *src,
    mlib_image *dst,
    const mlib_s32 *kern,
    mlib_s32 scalef_expon,
    const void *colormap);

mlib_status mlib_convMxNIndex_16_16nw(
    const mlib_image *src,
    mlib_image *dst,
    mlib_s32 m,
    mlib_s32 n,
    mlib_s32 dm,
    mlib_s32 dn,
    const mlib_s32 *kern,
    mlib_s32 scalef_expon,
    const void *colormap);

mlib_status mlib_conv2x2Index_8_16ext(
    const mlib_image *src,
    mlib_image *dst,
    const mlib_s32 *kern,
    mlib_s32 scalef_expon,
    const void *colormap);

mlib_status mlib_conv3x3Index_8_16ext(
    const mlib_image *src,
    mlib_image *dst,
    const mlib_s32 *kern,
    mlib_s32 scalef_expon,
    const void *colormap);

mlib_status mlib_conv4x4Index_8_16ext(
    const mlib_image *src,
    mlib_image *dst,
    const mlib_s32 *kern,
    mlib_s32 scalef_expon,
    const void *colormap);

mlib_status mlib_conv5x5Index_8_16ext(
    const mlib_image *src,
    mlib_image *dst,
    const mlib_s32 *kern,
    mlib_s32 scalef_expon,
    const void *colormap);

mlib_status mlib_convMxNIndex_8_16ext(
    const mlib_image *src,
    mlib_image *dst,
    mlib_s32 m,
    mlib_s32 n,
    mlib_s32 dx_l,
    mlib_s32 dx_r,
    mlib_s32 dy_t,
    mlib_s32 dy_b,
    const mlib_s32 *kern,
    mlib_s32 scalef_expon,
    const void *colormap);

mlib_status mlib_conv2x2Index_8_16nw(
    const mlib_image *src,
    mlib_image *dst,
    const mlib_s32 *kern,
    mlib_s32 scalef_expon,
    const void *colormap);

mlib_status mlib_conv3x3Index_8_16nw(
    const mlib_image *src,
    mlib_image *dst,
    const mlib_s32 *kern,
    mlib_s32 scalef_expon,
    const void *colormap);

mlib_status mlib_conv4x4Index_8_16nw(
    const mlib_image *src,
    mlib_image *dst,
    const mlib_s32 *kern,
    mlib_s32 scalef_expon,
    const void *colormap);

mlib_status mlib_conv5x5Index_8_16nw(
    const mlib_image *src,
    mlib_image *dst,
    const mlib_s32 *kern,
    mlib_s32 scalef_expon,
    const void *colormap);

mlib_status mlib_convMxNIndex_8_16nw(
    const mlib_image *src,
    mlib_image *dst,
    mlib_s32 m,
    mlib_s32 n,
    mlib_s32 dm,
    mlib_s32 dn,
    const mlib_s32 *kern,
    mlib_s32 scalef_expon,
    const void *colormap);

mlib_status mlib_conv2x2Index_16_8ext(
    const mlib_image *src,
    mlib_image *dst,
    const mlib_s32 *kern,
    mlib_s32 scalef_expon,
    const void *colormap);

mlib_status mlib_conv3x3Index_16_8ext(
    const mlib_image *src,
    mlib_image *dst,
    const mlib_s32 *kern,
    mlib_s32 scalef_expon,
    const void *colormap);

mlib_status mlib_conv4x4Index_16_8ext(
    const mlib_image *src,
    mlib_image *dst,
    const mlib_s32 *kern,
    mlib_s32 scalef_expon,
    const void *colormap);

mlib_status mlib_conv5x5Index_16_8ext(
    const mlib_image *src,
    mlib_image *dst,
    const mlib_s32 *kern,
    mlib_s32 scalef_expon,
    const void *colormap);

mlib_status mlib_convMxNIndex_16_8ext(
    const mlib_image *src,
    mlib_image *dst,
    mlib_s32 m,
    mlib_s32 n,
    mlib_s32 dx_l,
    mlib_s32 dx_r,
    mlib_s32 dy_t,
    mlib_s32 dy_b,
    const mlib_s32 *kern,
    mlib_s32 scalef_expon,
    const void *colormap);

mlib_status mlib_conv2x2Index_16_8nw(
    const mlib_image *src,
    mlib_image *dst,
    const mlib_s32 *kern,
    mlib_s32 scalef_expon,
    const void *colormap);

mlib_status mlib_conv3x3Index_16_8nw(
    const mlib_image *src,
    mlib_image *dst,
    const mlib_s32 *kern,
    mlib_s32 scalef_expon,
    const void *colormap);

mlib_status mlib_conv4x4Index_16_8nw(
    const mlib_image *src,
    mlib_image *dst,
    const mlib_s32 *kern,
    mlib_s32 scalef_expon,
    const void *colormap);

mlib_status mlib_conv5x5Index_16_8nw(
    const mlib_image *src,
    mlib_image *dst,
    const mlib_s32 *kern,
    mlib_s32 scalef_expon,
    const void *colormap);

mlib_status mlib_convMxNIndex_16_8nw(
    const mlib_image *src,
    mlib_image *dst,
    mlib_s32 m,
    mlib_s32 n,
    mlib_s32 dm,
    mlib_s32 dn,
    const mlib_s32 *kern,
    mlib_s32 scalef_expon,
    const void *colormap);

mlib_status mlib_conv2x2Index_8_8ext(
    const mlib_image *src,
    mlib_image *dst,
    const mlib_s32 *kern,
    mlib_s32 scalef_expon,
    const void *colormap);

mlib_status mlib_conv3x3Index_8_8ext(
    const mlib_image *src,
    mlib_image *dst,
    const mlib_s32 *kern,
    mlib_s32 scalef_expon,
    const void *colormap);

mlib_status mlib_conv4x4Index_8_8ext(
    const mlib_image *src,
    mlib_image *dst,
    const mlib_s32 *kern,
    mlib_s32 scalef_expon,
    const void *colormap);

mlib_status mlib_conv5x5Index_8_8ext(
    const mlib_image *src,
    mlib_image *dst,
    const mlib_s32 *kern,
    mlib_s32 scalef_expon,
    const void *colormap);

mlib_status mlib_convMxNIndex_8_8ext(
    const mlib_image *src,
    mlib_image *dst,
    mlib_s32 m,
    mlib_s32 n,
    mlib_s32 dx_l,
    mlib_s32 dx_r,
    mlib_s32 dy_t,
    mlib_s32 dy_b,
    const mlib_s32 *kern,
    mlib_s32 scalef_expon,
    const void *colormap);

mlib_status mlib_conv2x2Index_8_8nw(
    const mlib_image *src,
    mlib_image *dst,
    const mlib_s32 *kern,
    mlib_s32 scalef_expon,
    const void *colormap);

mlib_status mlib_conv3x3Index_8_8nw(
    const mlib_image *src,
    mlib_image *dst,
    const mlib_s32 *kern,
    mlib_s32 scalef_expon,
    const void *colormap);

mlib_status mlib_conv4x4Index_8_8nw(
    const mlib_image *src,
    mlib_image *dst,
    const mlib_s32 *kern,
    mlib_s32 scalef_expon,
    const void *colormap);

mlib_status mlib_conv5x5Index_8_8nw(
    const mlib_image *src,
    mlib_image *dst,
    const mlib_s32 *kern,
    mlib_s32 scalef_expon,
    const void *colormap);

mlib_status mlib_convMxNIndex_8_8nw(
    const mlib_image *src,
    mlib_image *dst,
    mlib_s32 m,
    mlib_s32 n,
    mlib_s32 dm,
    mlib_s32 dn,
    const mlib_s32 *kern,
    mlib_s32 scalef_expon,
    const void *colormap);

/* *********************************************************** */

#ifdef __cplusplus
}
#endif

#endif /* _MLIB_IMAGECONVINDEX_H */
