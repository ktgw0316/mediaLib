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

#ifndef _MLIB_IMAGECONV_H
#define	_MLIB_IMAGECONV_H

#pragma ident	"@(#)mlib_ImageConv.h	9.2	07/10/09 SMI"

#ifdef __cplusplus
extern "C" {
#endif

#include <mlib_types.h>
#include <mlib_image_types.h>
#include <mlib_status.h>

void mlib_ImageXor80_aa(
    mlib_u8 *dl,
    mlib_s32 wid,
    mlib_s32 hgt,
    mlib_s32 str);

void mlib_ImageXor80(
    mlib_u8 *dl,
    mlib_s32 wid,
    mlib_s32 hgt,
    mlib_s32 str,
    mlib_s32 nchan,
    mlib_s32 cmask);

mlib_status mlib_conv2x2ext_d64(
    mlib_image *dst,
    const mlib_image *src,
    mlib_s32 dx_l,
    mlib_s32 dx_r,
    mlib_s32 dy_t,
    mlib_s32 dy_b,
    const mlib_d64 *kern,
    mlib_s32 cmask);

mlib_status mlib_conv2x2ext_f32(
    mlib_image *dst,
    const mlib_image *src,
    mlib_s32 dx_l,
    mlib_s32 dx_r,
    mlib_s32 dy_t,
    mlib_s32 dy_b,
    const mlib_d64 *kern,
    mlib_s32 cmask);

mlib_status mlib_conv2x2ext_s16(
    mlib_image *dst,
    const mlib_image *src,
    mlib_s32 dx_l,
    mlib_s32 dx_r,
    mlib_s32 dy_t,
    mlib_s32 dy_b,
    const mlib_s32 *kern,
    mlib_s32 scale,
    mlib_s32 cmask);

mlib_status mlib_conv2x2ext_s32(
    mlib_image *dst,
    const mlib_image *src,
    mlib_s32 dx_l,
    mlib_s32 dx_r,
    mlib_s32 dy_t,
    mlib_s32 dy_b,
    const mlib_s32 *kern,
    mlib_s32 scale,
    mlib_s32 cmask);

mlib_status mlib_conv2x2ext_u16(
    mlib_image *dst,
    const mlib_image *src,
    mlib_s32 dx_l,
    mlib_s32 dx_r,
    mlib_s32 dy_t,
    mlib_s32 dy_b,
    const mlib_s32 *kern,
    mlib_s32 scale,
    mlib_s32 cmask);

mlib_status mlib_conv2x2ext_u8(
    mlib_image *dst,
    const mlib_image *src,
    mlib_s32 dx_l,
    mlib_s32 dx_r,
    mlib_s32 dy_t,
    mlib_s32 dy_b,
    const mlib_s32 *kern,
    mlib_s32 scale,
    mlib_s32 cmask);

mlib_status mlib_conv2x2nw_d64(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_d64 *kern,
    mlib_s32 cmask);

mlib_status mlib_conv2x2nw_f32(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_d64 *kern,
    mlib_s32 cmask);

mlib_status mlib_conv2x2nw_s16(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_s32 *kern,
    mlib_s32 scale,
    mlib_s32 cmask);

mlib_status mlib_conv2x2nw_s32(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_s32 *kern,
    mlib_s32 scale,
    mlib_s32 cmask);

mlib_status mlib_conv2x2nw_u16(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_s32 *kern,
    mlib_s32 scale,
    mlib_s32 cmask);

mlib_status mlib_conv2x2nw_u8(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_s32 *kern,
    mlib_s32 scale,
    mlib_s32 cmask);

mlib_status mlib_conv3x3ext_bit(
    mlib_image *dst,
    const mlib_image *src,
    mlib_s32 dx_l,
    mlib_s32 dx_r,
    mlib_s32 dy_t,
    mlib_s32 dy_b,
    const mlib_s32 *kern,
    mlib_s32 scale,
    mlib_s32 cmask);

mlib_status mlib_conv3x3ext_d64(
    mlib_image *dst,
    const mlib_image *src,
    mlib_s32 dx_l,
    mlib_s32 dx_r,
    mlib_s32 dy_t,
    mlib_s32 dy_b,
    const mlib_d64 *kern,
    mlib_s32 cmask);

mlib_status mlib_conv3x3ext_f32(
    mlib_image *dst,
    const mlib_image *src,
    mlib_s32 dx_l,
    mlib_s32 dx_r,
    mlib_s32 dy_t,
    mlib_s32 dy_b,
    const mlib_d64 *kern,
    mlib_s32 cmask);

mlib_status mlib_conv3x3ext_s16(
    mlib_image *dst,
    const mlib_image *src,
    mlib_s32 dx_l,
    mlib_s32 dx_r,
    mlib_s32 dy_t,
    mlib_s32 dy_b,
    const mlib_s32 *kern,
    mlib_s32 scale,
    mlib_s32 cmask);

mlib_status mlib_conv3x3ext_s32(
    mlib_image *dst,
    const mlib_image *src,
    mlib_s32 dx_l,
    mlib_s32 dx_r,
    mlib_s32 dy_t,
    mlib_s32 dy_b,
    const mlib_s32 *kern,
    mlib_s32 scale,
    mlib_s32 cmask);

mlib_status mlib_conv3x3ext_u16(
    mlib_image *dst,
    const mlib_image *src,
    mlib_s32 dx_l,
    mlib_s32 dx_r,
    mlib_s32 dy_t,
    mlib_s32 dy_b,
    const mlib_s32 *kern,
    mlib_s32 scale,
    mlib_s32 cmask);

mlib_status mlib_conv3x3ext_u8(
    mlib_image *dst,
    const mlib_image *src,
    mlib_s32 dx_l,
    mlib_s32 dx_r,
    mlib_s32 dy_t,
    mlib_s32 dy_b,
    const mlib_s32 *kern,
    mlib_s32 scale,
    mlib_s32 cmask);

mlib_status mlib_conv3x3nw_bit(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_s32 *kern,
    mlib_s32 scale,
    mlib_s32 cmask);

mlib_status mlib_conv3x3nw_d64(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_d64 *kern,
    mlib_s32 cmask);

mlib_status mlib_conv3x3nw_f32(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_d64 *kern,
    mlib_s32 cmask);

mlib_status mlib_conv3x3nw_s16(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_s32 *kern,
    mlib_s32 scale,
    mlib_s32 cmask);

mlib_status mlib_conv3x3nw_s32(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_s32 *kern,
    mlib_s32 scale,
    mlib_s32 cmask);

mlib_status mlib_conv3x3nw_u16(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_s32 *kern,
    mlib_s32 scale,
    mlib_s32 cmask);

mlib_status mlib_conv3x3nw_u8(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_s32 *kern,
    mlib_s32 scale,
    mlib_s32 cmask);

mlib_status mlib_conv4x4ext_d64(
    mlib_image *dst,
    const mlib_image *src,
    mlib_s32 dx_l,
    mlib_s32 dx_r,
    mlib_s32 dy_t,
    mlib_s32 dy_b,
    const mlib_d64 *kern,
    mlib_s32 cmask);

mlib_status mlib_conv4x4ext_f32(
    mlib_image *dst,
    const mlib_image *src,
    mlib_s32 dx_l,
    mlib_s32 dx_r,
    mlib_s32 dy_t,
    mlib_s32 dy_b,
    const mlib_d64 *kern,
    mlib_s32 cmask);

mlib_status mlib_conv4x4ext_s16(
    mlib_image *dst,
    const mlib_image *src,
    mlib_s32 dx_l,
    mlib_s32 dx_r,
    mlib_s32 dy_t,
    mlib_s32 dy_b,
    const mlib_s32 *kern,
    mlib_s32 scale,
    mlib_s32 cmask);

mlib_status mlib_conv4x4ext_s32(
    mlib_image *dst,
    const mlib_image *src,
    mlib_s32 dx_l,
    mlib_s32 dx_r,
    mlib_s32 dy_t,
    mlib_s32 dy_b,
    const mlib_s32 *kern,
    mlib_s32 scale,
    mlib_s32 cmask);

mlib_status mlib_conv4x4ext_u16(
    mlib_image *dst,
    const mlib_image *src,
    mlib_s32 dx_l,
    mlib_s32 dx_r,
    mlib_s32 dy_t,
    mlib_s32 dy_b,
    const mlib_s32 *kern,
    mlib_s32 scale,
    mlib_s32 cmask);

mlib_status mlib_conv4x4ext_u8(
    mlib_image *dst,
    const mlib_image *src,
    mlib_s32 dx_l,
    mlib_s32 dx_r,
    mlib_s32 dy_t,
    mlib_s32 dy_b,
    const mlib_s32 *kern,
    mlib_s32 scale,
    mlib_s32 cmask);

mlib_status mlib_conv4x4nw_d64(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_d64 *kern,
    mlib_s32 cmask);

mlib_status mlib_conv4x4nw_f32(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_d64 *kern,
    mlib_s32 cmask);

mlib_status mlib_conv4x4nw_s16(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_s32 *kern,
    mlib_s32 scale,
    mlib_s32 cmask);

mlib_status mlib_conv4x4nw_s32(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_s32 *kern,
    mlib_s32 scale,
    mlib_s32 cmask);

mlib_status mlib_conv4x4nw_u16(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_s32 *kern,
    mlib_s32 scale,
    mlib_s32 cmask);

mlib_status mlib_conv4x4nw_u8(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_s32 *kern,
    mlib_s32 scale,
    mlib_s32 cmask);

mlib_status mlib_conv5x5ext_d64(
    mlib_image *dst,
    const mlib_image *src,
    mlib_s32 dx_l,
    mlib_s32 dx_r,
    mlib_s32 dy_t,
    mlib_s32 dy_b,
    const mlib_d64 *kern,
    mlib_s32 cmask);

mlib_status mlib_conv5x5ext_f32(
    mlib_image *dst,
    const mlib_image *src,
    mlib_s32 dx_l,
    mlib_s32 dx_r,
    mlib_s32 dy_t,
    mlib_s32 dy_b,
    const mlib_d64 *kern,
    mlib_s32 cmask);

mlib_status mlib_conv5x5ext_s16(
    mlib_image *dst,
    const mlib_image *src,
    mlib_s32 dx_l,
    mlib_s32 dx_r,
    mlib_s32 dy_t,
    mlib_s32 dy_b,
    const mlib_s32 *kern,
    mlib_s32 scale,
    mlib_s32 cmask);

mlib_status mlib_conv5x5ext_s32(
    mlib_image *dst,
    const mlib_image *src,
    mlib_s32 dx_l,
    mlib_s32 dx_r,
    mlib_s32 dy_t,
    mlib_s32 dy_b,
    const mlib_s32 *kern,
    mlib_s32 scale,
    mlib_s32 cmask);

mlib_status mlib_conv5x5ext_u16(
    mlib_image *dst,
    const mlib_image *src,
    mlib_s32 dx_l,
    mlib_s32 dx_r,
    mlib_s32 dy_t,
    mlib_s32 dy_b,
    const mlib_s32 *kern,
    mlib_s32 scale,
    mlib_s32 cmask);

mlib_status mlib_conv5x5ext_u8(
    mlib_image *dst,
    const mlib_image *src,
    mlib_s32 dx_l,
    mlib_s32 dx_r,
    mlib_s32 dy_t,
    mlib_s32 dy_b,
    const mlib_s32 *kern,
    mlib_s32 scale,
    mlib_s32 cmask);

mlib_status mlib_conv5x5nw_d64(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_d64 *kern,
    mlib_s32 cmask);

mlib_status mlib_conv5x5nw_f32(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_d64 *kern,
    mlib_s32 cmask);

mlib_status mlib_conv5x5nw_s16(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_s32 *kern,
    mlib_s32 scale,
    mlib_s32 cmask);

mlib_status mlib_conv5x5nw_s32(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_s32 *kern,
    mlib_s32 scale,
    mlib_s32 cmask);

mlib_status mlib_conv5x5nw_u16(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_s32 *kern,
    mlib_s32 scale,
    mlib_s32 cmask);

mlib_status mlib_conv5x5nw_u8(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_s32 *kern,
    mlib_s32 scale,
    mlib_s32 cmask);

mlib_status mlib_conv7x7ext_s16(
    mlib_image *dst,
    const mlib_image *src,
    mlib_s32 dx_l,
    mlib_s32 dx_r,
    mlib_s32 dy_t,
    mlib_s32 dy_b,
    const mlib_s32 *kern,
    mlib_s32 scale,
    mlib_s32 cmask);

mlib_status mlib_conv7x7ext_s32(
    mlib_image *dst,
    const mlib_image *src,
    mlib_s32 dx_l,
    mlib_s32 dx_r,
    mlib_s32 dy_t,
    mlib_s32 dy_b,
    const mlib_s32 *kern,
    mlib_s32 scale,
    mlib_s32 cmask);

mlib_status mlib_conv7x7ext_u16(
    mlib_image *dst,
    const mlib_image *src,
    mlib_s32 dx_l,
    mlib_s32 dx_r,
    mlib_s32 dy_t,
    mlib_s32 dy_b,
    const mlib_s32 *kern,
    mlib_s32 scale,
    mlib_s32 cmask);

mlib_status mlib_conv7x7ext_u8(
    mlib_image *dst,
    const mlib_image *src,
    mlib_s32 dx_l,
    mlib_s32 dx_r,
    mlib_s32 dy_t,
    mlib_s32 dy_b,
    const mlib_s32 *kern,
    mlib_s32 scale,
    mlib_s32 cmask);

mlib_status mlib_conv7x7nw_s16(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_s32 *kern,
    mlib_s32 scale,
    mlib_s32 cmask);

mlib_status mlib_conv7x7nw_s32(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_s32 *kern,
    mlib_s32 scale,
    mlib_s32 cmask);

mlib_status mlib_conv7x7nw_u16(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_s32 *kern,
    mlib_s32 scale,
    mlib_s32 cmask);

mlib_status mlib_conv7x7nw_u8(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_s32 *kern,
    mlib_s32 scale,
    mlib_s32 cmask);

mlib_status mlib_convMxNext_s32(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_s32 *kernel,
    mlib_s32 m,
    mlib_s32 n,
    mlib_s32 dx_l,
    mlib_s32 dx_r,
    mlib_s32 dy_t,
    mlib_s32 dy_b,
    mlib_s32 scale,
    mlib_s32 cmask);

mlib_status mlib_convMxNnw_d64(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_d64 *ker,
    mlib_s32 m,
    mlib_s32 n,
    mlib_s32 dm,
    mlib_s32 dn,
    mlib_s32 cmask);

mlib_status mlib_convMxNnw_f32(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_d64 *ker,
    mlib_s32 m,
    mlib_s32 n,
    mlib_s32 dm,
    mlib_s32 dn,
    mlib_s32 cmask);

mlib_status mlib_convMxNnw_s16(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_s32 *kernel,
    mlib_s32 m,
    mlib_s32 n,
    mlib_s32 dm,
    mlib_s32 dn,
    mlib_s32 scale,
    mlib_s32 cmask);

mlib_status mlib_convMxNnw_s32(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_s32 *kernel,
    mlib_s32 m,
    mlib_s32 n,
    mlib_s32 dm,
    mlib_s32 dn,
    mlib_s32 scale,
    mlib_s32 cmask);

mlib_status mlib_convMxNnw_u16(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_s32 *kernel,
    mlib_s32 m,
    mlib_s32 n,
    mlib_s32 dm,
    mlib_s32 dn,
    mlib_s32 scale,
    mlib_s32 cmask);

mlib_s32 mlib_ImageConvVersion(
    mlib_s32 m,
    mlib_s32 n,
    mlib_s32 scale,
    mlib_type type);

mlib_status mlib_ImageConvMxN_f(
    mlib_image *dst,
    const mlib_image *src,
    const void *kernel,
    mlib_s32 m,
    mlib_s32 n,
    mlib_s32 dm,
    mlib_s32 dn,
    mlib_s32 scale,
    mlib_s32 cmask,
    mlib_edge edge);

mlib_status mlib_convMxNnw_u8(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_s32 *kern,
    mlib_s32 m,
    mlib_s32 n,
    mlib_s32 dm,
    mlib_s32 dn,
    mlib_s32 scale,
    mlib_s32 cmask);

mlib_status mlib_convMxNext_u8(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_s32 *kern,
    mlib_s32 m,
    mlib_s32 n,
    mlib_s32 dx_l,
    mlib_s32 dx_r,
    mlib_s32 dy_t,
    mlib_s32 dy_b,
    mlib_s32 scale,
    mlib_s32 cmask);

mlib_status mlib_convMxNext_s16(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_s32 *kernel,
    mlib_s32 m,
    mlib_s32 n,
    mlib_s32 dx_l,
    mlib_s32 dx_r,
    mlib_s32 dy_t,
    mlib_s32 dy_b,
    mlib_s32 scale,
    mlib_s32 cmask);

mlib_status mlib_convMxNext_u16(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_s32 *kernel,
    mlib_s32 m,
    mlib_s32 n,
    mlib_s32 dx_l,
    mlib_s32 dx_r,
    mlib_s32 dy_t,
    mlib_s32 dy_b,
    mlib_s32 scale,
    mlib_s32 cmask);

mlib_status mlib_convMxNext_f32(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_d64 *kernel,
    mlib_s32 m,
    mlib_s32 n,
    mlib_s32 dx_l,
    mlib_s32 dx_r,
    mlib_s32 dy_t,
    mlib_s32 dy_b,
    mlib_s32 cmask);

mlib_status mlib_convMxNext_d64(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_d64 *kernel,
    mlib_s32 m,
    mlib_s32 n,
    mlib_s32 dx_l,
    mlib_s32 dx_r,
    mlib_s32 dy_t,
    mlib_s32 dy_b,
    mlib_s32 cmask);

#ifdef __cplusplus
}
#endif

#endif /* _MLIB_IMAGECONV_H */
