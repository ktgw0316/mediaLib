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

#ifndef _MLIB_IMAGESCONV_H
#define	_MLIB_IMAGESCONV_H

#pragma ident	"@(#)mlib_ImageSConv.h	9.2	07/10/09 SMI"

#ifdef __cplusplus
extern "C" {
#endif

#include <mlib_types.h>
#include <mlib_image_types.h>
#include <mlib_status.h>

mlib_status mlib_sconv3x3ext_u8(
    mlib_image *dst,
    const mlib_image *src,
    mlib_s32 dx_l,
    mlib_s32 dx_r,
    mlib_s32 dy_t,
    mlib_s32 dy_b,
    const mlib_s32 *hkernel,
    const mlib_s32 *vkernel,
    mlib_s32 scalef_expon,
    mlib_s32 cmask);

mlib_status mlib_sconv5x5ext_u8(
    mlib_image *dst,
    const mlib_image *src,
    mlib_s32 dx_l,
    mlib_s32 dx_r,
    mlib_s32 dy_t,
    mlib_s32 dy_b,
    const mlib_s32 *hkernel,
    const mlib_s32 *vkernel,
    mlib_s32 scalef_expon,
    mlib_s32 cmask);

mlib_status mlib_sconv7x7ext_u8(
    mlib_image *dst,
    const mlib_image *src,
    mlib_s32 dx_l,
    mlib_s32 dx_r,
    mlib_s32 dy_t,
    mlib_s32 dy_b,
    const mlib_s32 *hkernel,
    const mlib_s32 *vkernel,
    mlib_s32 scalef_expon,
    mlib_s32 cmask);

mlib_status mlib_sconv3x3ext_s16(
    mlib_image *dst,
    const mlib_image *src,
    mlib_s32 dx_l,
    mlib_s32 dx_r,
    mlib_s32 dy_t,
    mlib_s32 dy_b,
    const mlib_s32 *hkernel,
    const mlib_s32 *vkernel,
    mlib_s32 scalef_expon,
    mlib_s32 cmask);

mlib_status mlib_sconv5x5ext_s16(
    mlib_image *dst,
    const mlib_image *src,
    mlib_s32 dx_l,
    mlib_s32 dx_r,
    mlib_s32 dy_t,
    mlib_s32 dy_b,
    const mlib_s32 *hkernel,
    const mlib_s32 *vkernel,
    mlib_s32 scalef_expon,
    mlib_s32 cmask);

mlib_status mlib_sconv7x7ext_s16(
    mlib_image *dst,
    const mlib_image *src,
    mlib_s32 dx_l,
    mlib_s32 dx_r,
    mlib_s32 dy_t,
    mlib_s32 dy_b,
    const mlib_s32 *hkernel,
    const mlib_s32 *vkernel,
    mlib_s32 scalef_expon,
    mlib_s32 cmask);

mlib_status mlib_sconv3x3ext_u16(
    mlib_image *dst,
    const mlib_image *src,
    mlib_s32 dx_l,
    mlib_s32 dx_r,
    mlib_s32 dy_t,
    mlib_s32 dy_b,
    const mlib_s32 *hkernel,
    const mlib_s32 *vkernel,
    mlib_s32 scalef_expon,
    mlib_s32 cmask);

mlib_status mlib_sconv5x5ext_u16(
    mlib_image *dst,
    const mlib_image *src,
    mlib_s32 dx_l,
    mlib_s32 dx_r,
    mlib_s32 dy_t,
    mlib_s32 dy_b,
    const mlib_s32 *hkernel,
    const mlib_s32 *vkernel,
    mlib_s32 scalef_expon,
    mlib_s32 cmask);

mlib_status mlib_sconv7x7ext_u16(
    mlib_image *dst,
    const mlib_image *src,
    mlib_s32 dx_l,
    mlib_s32 dx_r,
    mlib_s32 dy_t,
    mlib_s32 dy_b,
    const mlib_s32 *hkernel,
    const mlib_s32 *vkernel,
    mlib_s32 scalef_expon,
    mlib_s32 cmask);

mlib_status mlib_sconv3x3ext_s32(
    mlib_image *dst,
    const mlib_image *src,
    mlib_s32 dx_l,
    mlib_s32 dx_r,
    mlib_s32 dy_t,
    mlib_s32 dy_b,
    const mlib_s32 *hkernel,
    const mlib_s32 *vkernel,
    mlib_s32 scalef_expon,
    mlib_s32 cmask);

mlib_status mlib_sconv5x5ext_s32(
    mlib_image *dst,
    const mlib_image *src,
    mlib_s32 dx_l,
    mlib_s32 dx_r,
    mlib_s32 dy_t,
    mlib_s32 dy_b,
    const mlib_s32 *hkernel,
    const mlib_s32 *vkernel,
    mlib_s32 scalef_expon,
    mlib_s32 cmask);

mlib_status mlib_sconv7x7ext_s32(
    mlib_image *dst,
    const mlib_image *src,
    mlib_s32 dx_l,
    mlib_s32 dx_r,
    mlib_s32 dy_t,
    mlib_s32 dy_b,
    const mlib_s32 *hkernel,
    const mlib_s32 *vkernel,
    mlib_s32 scalef_expon,
    mlib_s32 cmask);

mlib_status mlib_sconv3x3ext_f32(
    mlib_image *dst,
    const mlib_image *src,
    mlib_s32 dx_l,
    mlib_s32 dx_r,
    mlib_s32 dy_t,
    mlib_s32 dy_b,
    const mlib_d64 *hkernel,
    const mlib_d64 *vkernel,
    mlib_s32 cmask);

mlib_status mlib_sconv5x5ext_f32(
    mlib_image *dst,
    const mlib_image *src,
    mlib_s32 dx_l,
    mlib_s32 dx_r,
    mlib_s32 dy_t,
    mlib_s32 dy_b,
    const mlib_d64 *hkernel,
    const mlib_d64 *vkernel,
    mlib_s32 cmask);

mlib_status mlib_sconv7x7ext_f32(
    mlib_image *dst,
    const mlib_image *src,
    mlib_s32 dx_l,
    mlib_s32 dx_r,
    mlib_s32 dy_t,
    mlib_s32 dy_b,
    const mlib_d64 *hkernel,
    const mlib_d64 *vkernel,
    mlib_s32 cmask);

mlib_status mlib_sconv3x3ext_d64(
    mlib_image *dst,
    const mlib_image *src,
    mlib_s32 dx_l,
    mlib_s32 dx_r,
    mlib_s32 dy_t,
    mlib_s32 dy_b,
    const mlib_d64 *hkernel,
    const mlib_d64 *vkernel,
    mlib_s32 cmask);

mlib_status mlib_sconv5x5ext_d64(
    mlib_image *dst,
    const mlib_image *src,
    mlib_s32 dx_l,
    mlib_s32 dx_r,
    mlib_s32 dy_t,
    mlib_s32 dy_b,
    const mlib_d64 *hkernel,
    const mlib_d64 *vkernel,
    mlib_s32 cmask);

mlib_status mlib_sconv7x7ext_d64(
    mlib_image *dst,
    const mlib_image *src,
    mlib_s32 dx_l,
    mlib_s32 dx_r,
    mlib_s32 dy_t,
    mlib_s32 dy_b,
    const mlib_d64 *hkernel,
    const mlib_d64 *vkernel,
    mlib_s32 cmask);

mlib_status mlib_sconv3x3nw_u8(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_s32 *hkernel,
    const mlib_s32 *vkernel,
    mlib_s32 scalef_expon,
    mlib_s32 cmask);

mlib_status mlib_sconv5x5nw_u8(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_s32 *hkernel,
    const mlib_s32 *vkernel,
    mlib_s32 scalef_expon,
    mlib_s32 cmask);

mlib_status mlib_sconv7x7nw_u8(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_s32 *hkernel,
    const mlib_s32 *vkernel,
    mlib_s32 scalef_expon,
    mlib_s32 cmask);

mlib_status mlib_sconv3x3nw_s16(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_s32 *hkernel,
    const mlib_s32 *vkernel,
    mlib_s32 scalef_expon,
    mlib_s32 cmask);

mlib_status mlib_sconv5x5nw_s16(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_s32 *hkernel,
    const mlib_s32 *vkernel,
    mlib_s32 scalef_expon,
    mlib_s32 cmask);

mlib_status mlib_sconv7x7nw_s16(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_s32 *hkernel,
    const mlib_s32 *vkernel,
    mlib_s32 scalef_expon,
    mlib_s32 cmask);

mlib_status mlib_sconv3x3nw_u16(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_s32 *hkernel,
    const mlib_s32 *vkernel,
    mlib_s32 scalef_expon,
    mlib_s32 cmask);

mlib_status mlib_sconv5x5nw_u16(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_s32 *hkernel,
    const mlib_s32 *vkernel,
    mlib_s32 scalef_expon,
    mlib_s32 cmask);

mlib_status mlib_sconv7x7nw_u16(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_s32 *hkernel,
    const mlib_s32 *vkernel,
    mlib_s32 scalef_expon,
    mlib_s32 cmask);

mlib_status mlib_sconv3x3nw_s32(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_s32 *hkernel,
    const mlib_s32 *vkernel,
    mlib_s32 scalef_expon,
    mlib_s32 cmask);

mlib_status mlib_sconv5x5nw_s32(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_s32 *hkernel,
    const mlib_s32 *vkernel,
    mlib_s32 scalef_expon,
    mlib_s32 cmask);

mlib_status mlib_sconv7x7nw_s32(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_s32 *hkernel,
    const mlib_s32 *vkernel,
    mlib_s32 scalef_expon,
    mlib_s32 cmask);

mlib_status mlib_sconv3x3nw_f32(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_d64 *hkernel,
    const mlib_d64 *vkernel,
    mlib_s32 cmask);

mlib_status mlib_sconv5x5nw_f32(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_d64 *hkernel,
    const mlib_d64 *vkernel,
    mlib_s32 cmask);

mlib_status mlib_sconv7x7nw_f32(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_d64 *hkernel,
    const mlib_d64 *vkernel,
    mlib_s32 cmask);

mlib_status mlib_sconv3x3nw_d64(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_d64 *hkernel,
    const mlib_d64 *vkernel,
    mlib_s32 cmask);

mlib_status mlib_sconv5x5nw_d64(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_d64 *hkernel,
    const mlib_d64 *vkernel,
    mlib_s32 cmask);

mlib_status mlib_sconv7x7nw_d64(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_d64 *hkernel,
    const mlib_d64 *vkernel,
    mlib_s32 cmask);

#ifdef __cplusplus
}
#endif

#endif /* _MLIB_IMAGESCONV_H */
