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

#ifndef _MLIB_V_IMAGECHANNELEXTRACT_2_H
#define	_MLIB_V_IMAGECHANNELEXTRACT_2_H

#pragma ident	"@(#)mlib_v_ImageChannelExtract_2.h	9.2	07/11/05 SMI"

#ifdef __cplusplus
extern "C" {
#endif

#include <mlib_types.h>

void mlib_v_ImageChannelExtract_U8_32_A8D1X8(
    const mlib_u8 *src,
    mlib_u8 *dst,
    mlib_s32 dsize,
    mlib_s32 cmask);

void mlib_v_ImageChannelExtract_U8_32_A8D2X8(
    const mlib_u8 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 cmask);

void mlib_v_ImageChannelExtract_U8_32_D1(
    const mlib_u8 *src,
    mlib_u8 *dst,
    mlib_s32 dsize,
    mlib_s32 cmask);

void mlib_v_ImageChannelExtract_U8_32(
    const mlib_u8 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 cmask);

void mlib_v_ImageChannelExtract_U8_42_A8D1X8(
    const mlib_u8 *src,
    mlib_u8 *dst,
    mlib_s32 dsize,
    mlib_s32 cmask);

void mlib_v_ImageChannelExtract_U8_42_A8D2X8(
    const mlib_u8 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 cmask);

void mlib_v_ImageChannelExtract_U8_42_D1(
    const mlib_u8 *src,
    mlib_u8 *dst,
    mlib_s32 dsize,
    mlib_s32 cmask);

void mlib_v_ImageChannelExtract_U8_42(
    const mlib_u8 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 cmask);

void mlib_v_ImageChannelExtract_S16_32_A8D1X4(
    const mlib_s16 *src,
    mlib_s16 *dst,
    mlib_s32 dsize,
    mlib_s32 cmask);

void mlib_v_ImageChannelExtract_S16_32_A8D2X4(
    const mlib_s16 *src,
    mlib_s32 slb,
    mlib_s16 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 cmask);

void mlib_v_ImageChannelExtract_S16_32_D1(
    const mlib_s16 *src,
    mlib_s16 *dst,
    mlib_s32 dsize,
    mlib_s32 cmask);

void mlib_v_ImageChannelExtract_S16_32(
    const mlib_s16 *src,
    mlib_s32 slb,
    mlib_s16 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 cmask);

void mlib_v_ImageChannelExtract_S16_42_A8D1X4(
    const mlib_s16 *src,
    mlib_s16 *dst,
    mlib_s32 dsize,
    mlib_s32 cmask);

void mlib_v_ImageChannelExtract_S16_42_A8D2X4(
    const mlib_s16 *src,
    mlib_s32 slb,
    mlib_s16 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 cmask);

void mlib_v_ImageChannelExtract_S16_42_D1(
    const mlib_s16 *src,
    mlib_s16 *dst,
    mlib_s32 dsize,
    mlib_s32 cmask);

void mlib_v_ImageChannelExtract_S16_42(
    const mlib_s16 *src,
    mlib_s32 slb,
    mlib_s16 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 cmask);

void mlib_v_ImageChannelExtract_43_A8D2X8(
    const mlib_u8 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_s32 *bmask);

void mlib_v_ImageChannelExtract_43(
    const mlib_u8 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_s32 *bmask);

void mlib_v_ImageChannelExtract_32_21(
    const mlib_f32 *src,
    mlib_s32 slb,
    mlib_f32 *dst,
    mlib_s32 dlb,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s32 cmask);

void mlib_v_ImageChannelExtract_32_31(
    const mlib_f32 *src,
    mlib_s32 slb,
    mlib_f32 *dst,
    mlib_s32 dlb,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s32 cmask);

void mlib_v_ImageChannelExtract_32_41(
    const mlib_f32 *src,
    mlib_s32 slb,
    mlib_f32 *dst,
    mlib_s32 dlb,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s32 cmask);

void mlib_v_ImageChannelExtract_32_32(
    const mlib_f32 *src,
    mlib_s32 slb,
    mlib_f32 *dst,
    mlib_s32 dlb,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s32 cmask);

void mlib_v_ImageChannelExtract_32_42(
    const mlib_f32 *src,
    mlib_s32 slb,
    mlib_f32 *dst,
    mlib_s32 dlb,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s32 cmask);

void mlib_v_ImageChannelExtract_32_43(
    const mlib_f32 *src,
    mlib_s32 slb,
    mlib_f32 *dst,
    mlib_s32 dlb,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s32 cmask);

void mlib_v_ImageChannelExtract_D64_21(
    const mlib_d64 *src,
    mlib_s32 slb,
    mlib_d64 *dst,
    mlib_s32 dlb,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s32 cmask);

void mlib_v_ImageChannelExtract_D64_31(
    const mlib_d64 *src,
    mlib_s32 slb,
    mlib_d64 *dst,
    mlib_s32 dlb,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s32 cmask);

void mlib_v_ImageChannelExtract_D64_41(
    const mlib_d64 *src,
    mlib_s32 slb,
    mlib_d64 *dst,
    mlib_s32 dlb,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s32 cmask);

void mlib_v_ImageChannelExtract_D64_32(
    const mlib_d64 *src,
    mlib_s32 slb,
    mlib_d64 *dst,
    mlib_s32 dlb,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s32 cmask);

void mlib_v_ImageChannelExtract_D64_42(
    const mlib_d64 *src,
    mlib_s32 slb,
    mlib_d64 *dst,
    mlib_s32 dlb,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s32 cmask);

void mlib_v_ImageChannelExtract_D64_43(
    const mlib_d64 *src,
    mlib_s32 slb,
    mlib_d64 *dst,
    mlib_s32 dlb,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s32 cmask);

void mlib_v_ImageChannelExtract_U8_43_D1(
    const mlib_u8 *src,
    mlib_u8 *dst,
    mlib_s32 dsize,
    mlib_s32 ncmask);

#ifdef __cplusplus
}
#endif

#endif /* _MLIB_V_IMAGECHANNELEXTRACT_2_H */
