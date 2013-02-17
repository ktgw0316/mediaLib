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

#ifndef _MLIB_V_IMAGECHANNELCOPY_H
#define	_MLIB_V_IMAGECHANNELCOPY_H

#pragma ident	"@(#)mlib_v_ImageChannelCopy.h	9.2	07/11/05 SMI"

#ifdef __cplusplus
extern "C" {
#endif

#include <mlib_types.h>

void mlib_v_ImageChannelCopy_U8_2_A8D1X4(
    const mlib_u8 *src,
    mlib_u8 *dst,
    mlib_s32 dsize,
    mlib_s32 cmask);

void mlib_v_ImageChannelCopy_U8_2_A8D2X4(
    const mlib_u8 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 cmask);

void mlib_v_ImageChannelCopy_U8_2(
    const mlib_u8 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 cmask);

void mlib_v_ImageChannelCopy_U8_3_A8D1(
    const mlib_u8 *src,
    mlib_u8 *dst,
    mlib_s32 dsize,
    mlib_s32 cmask);

void mlib_v_ImageChannelCopy_U8_3_A8D2(
    const mlib_u8 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 cmask);

void mlib_v_ImageChannelCopy_U8_3(
    const mlib_u8 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 cmask);

void mlib_v_ImageChannelCopy_U8_4_A8D1X2(
    const mlib_u8 *src,
    mlib_u8 *dst,
    mlib_s32 dsize,
    mlib_s32 cmask);

void mlib_v_ImageChannelCopy_U8_4_A8D2X2(
    const mlib_u8 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 cmask);

void mlib_v_ImageChannelCopy_U8_4(
    const mlib_u8 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 cmask);

void mlib_v_ImageChannelCopy_S16_2_A8D1X2(
    const mlib_s16 *src,
    mlib_s16 *dst,
    mlib_s32 dsize,
    mlib_s32 cmask);

void mlib_v_ImageChannelCopy_S16_2_A8D2X2(
    const mlib_s16 *src,
    mlib_s32 slb,
    mlib_s16 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 cmask);

void mlib_v_ImageChannelCopy_S16_2(
    const mlib_s16 *src,
    mlib_s32 slb,
    mlib_s16 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 cmask);

void mlib_v_ImageChannelCopy_S16_3_A8D1(
    const mlib_s16 *src,
    mlib_s16 *dst,
    mlib_s32 dsize,
    mlib_s32 cmask);

void mlib_v_ImageChannelCopy_S16_3_A8D2(
    const mlib_s16 *src,
    mlib_s32 slb,
    mlib_s16 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 cmask);

void mlib_v_ImageChannelCopy_S16_3(
    const mlib_s16 *src,
    mlib_s32 slb,
    mlib_s16 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 cmask);

void mlib_v_ImageChannelCopy_S16_4_A8D1(
    const mlib_s16 *src,
    mlib_s16 *dst,
    mlib_s32 dsize,
    mlib_s32 cmask);

void mlib_v_ImageChannelCopy_S16_4_A8D2(
    const mlib_s16 *src,
    mlib_s32 slb,
    mlib_s16 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 cmask);

void mlib_v_ImageChannelCopy_S16_4(
    const mlib_s16 *src,
    mlib_s32 slb,
    mlib_s16 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 cmask);

void mlib_v_ImageChannelCopy_S32_2_A8D1(
    const mlib_s32 *src,
    mlib_s32 *dst,
    mlib_s32 dsize,
    mlib_s32 cmask);

void mlib_v_ImageChannelCopy_S32_2_A8D2(
    const mlib_s32 *src,
    mlib_s32 slb,
    mlib_s32 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 cmask);

void mlib_v_ImageChannelCopy_S32_2(
    const mlib_s32 *src,
    mlib_s32 slb,
    mlib_s32 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 cmask);

void mlib_v_ImageChannelCopy_S32_3_A8D1(
    const mlib_s32 *src,
    mlib_s32 *dst,
    mlib_s32 dsize,
    mlib_s32 cmask);

void mlib_v_ImageChannelCopy_S32_3_A8D2(
    const mlib_s32 *src,
    mlib_s32 slb,
    mlib_s32 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 cmask);

void mlib_v_ImageChannelCopy_S32_3(
    const mlib_s32 *src,
    mlib_s32 slb,
    mlib_s32 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 cmask);

void mlib_v_ImageChannelCopy_S32_4_A8D1(
    const mlib_s32 *src,
    mlib_s32 *dst,
    mlib_s32 dsize,
    mlib_s32 cmask);

void mlib_v_ImageChannelCopy_S32_4_A8D2(
    const mlib_s32 *src,
    mlib_s32 slb,
    mlib_s32 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 cmask);

void mlib_v_ImageChannelCopy_S32_4(
    const mlib_s32 *src,
    mlib_s32 slb,
    mlib_s32 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 cmask);

void mlib_v_ImageChannelCopy_s32(
    const mlib_s32 *src,
    mlib_s32 src_stride,
    mlib_s32 *dst,
    mlib_s32 dst_stride,
    mlib_s32 nchannels,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 cmask);

void mlib_v_ImageChannelCopy_D64(
    const mlib_d64 *src,
    mlib_s32 src_stride,
    mlib_d64 *dst,
    mlib_s32 dst_stride,
    mlib_s32 nchannels,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 cmask);

#ifdef __cplusplus
}
#endif

#endif /* _MLIB_V_IMAGECHANNELCOPY_H */
