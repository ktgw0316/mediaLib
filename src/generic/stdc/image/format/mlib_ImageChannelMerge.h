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

#ifndef _MLIB_IMAGECHANNELMERGE_H
#define	_MLIB_IMAGECHANNELMERGE_H

#pragma ident	"@(#)mlib_ImageChannelMerge.h	9.2	07/10/09 SMI"

#ifdef __cplusplus
extern "C" {
#endif

#include <mlib_types.h>
#include <mlib_status.h>

mlib_status mlib_ImageChannelMerge2_D64(
    mlib_d64 *dst_d64_0,
    const mlib_d64 *src_d64_0,
    const mlib_d64 *src_d64_1,
    mlib_s32 height,
    mlib_s32 width,
    mlib_s32 dst_stride,
    mlib_s32 src0_stride,
    mlib_s32 src1_stride);

mlib_status mlib_ImageChannelMerge3_D64(
    mlib_d64 *dst_d64_0,
    const mlib_d64 *src_d64_0,
    const mlib_d64 *src_d64_1,
    const mlib_d64 *src_d64_2,
    mlib_s32 height,
    mlib_s32 width,
    mlib_s32 dst_stride,
    mlib_s32 src0_stride,
    mlib_s32 src1_stride,
    mlib_s32 src2_stride);

mlib_status mlib_ImageChannelMerge4_D64(
    mlib_d64 *dst_d64_0,
    const mlib_d64 *src_d64_0,
    const mlib_d64 *src_d64_1,
    const mlib_d64 *src_d64_2,
    const mlib_d64 *src_d64_3,
    mlib_s32 height,
    mlib_s32 width,
    mlib_s32 dst_stride,
    mlib_s32 src0_stride,
    mlib_s32 src1_stride,
    mlib_s32 src2_stride,
    mlib_s32 src3_stride);

mlib_status mlib_ImageChannelMerge2_FS32(
    mlib_s32 *dst_s32_0,
    const mlib_s32 *src_s32_0,
    const mlib_s32 *src_s32_1,
    mlib_s32 height,
    mlib_s32 width,
    mlib_s32 dst_stride,
    mlib_s32 src0_stride,
    mlib_s32 src1_stride);

mlib_status mlib_ImageChannelMerge3_FS32(
    mlib_s32 *dst_s32_0,
    const mlib_s32 *src_s32_0,
    const mlib_s32 *src_s32_1,
    const mlib_s32 *src_s32_2,
    mlib_s32 height,
    mlib_s32 width,
    mlib_s32 dst_stride,
    mlib_s32 src0_stride,
    mlib_s32 src1_stride,
    mlib_s32 src2_stride);

mlib_status mlib_ImageChannelMerge4_FS32(
    mlib_s32 *dst_s32_0,
    const mlib_s32 *src_s32_0,
    const mlib_s32 *src_s32_1,
    const mlib_s32 *src_s32_2,
    const mlib_s32 *src_s32_3,
    mlib_s32 height,
    mlib_s32 width,
    mlib_s32 dst_stride,
    mlib_s32 src0_stride,
    mlib_s32 src1_stride,
    mlib_s32 src2_stride,
    mlib_s32 src3_stride);

mlib_status mlib_ImageChannelMerge2_S16(
    mlib_s16 *dst_s16_0,
    const mlib_s16 *src_s16_0,
    const mlib_s16 *src_s16_1,
    mlib_s32 height,
    mlib_s32 width,
    mlib_s32 dst_stride,
    mlib_s32 src0_stride,
    mlib_s32 src1_stride);

mlib_status mlib_ImageChannelMerge3_S16(
    mlib_s16 *dst_s16_0,
    const mlib_s16 *src_s16_0,
    const mlib_s16 *src_s16_1,
    const mlib_s16 *src_s16_2,
    mlib_s32 height,
    mlib_s32 width,
    mlib_s32 dst_stride,
    mlib_s32 src0_stride,
    mlib_s32 src1_stride,
    mlib_s32 src2_stride);

mlib_status mlib_ImageChannelMerge4_S16(
    mlib_s16 *dst_s16_0,
    const mlib_s16 *src_s16_0,
    const mlib_s16 *src_s16_1,
    const mlib_s16 *src_s16_2,
    const mlib_s16 *src_s16_3,
    mlib_s32 height,
    mlib_s32 width,
    mlib_s32 dst_stride,
    mlib_s32 src0_stride,
    mlib_s32 src1_stride,
    mlib_s32 src2_stride,
    mlib_s32 src3_stride);

mlib_status mlib_ImageChannelMerge2_U8(
    mlib_u8 *dst_u8_0,
    const mlib_u8 *src_u8_0,
    const mlib_u8 *src_u8_1,
    mlib_s32 height,
    mlib_s32 width,
    mlib_s32 dst_stride,
    mlib_s32 src0_stride,
    mlib_s32 src1_stride);

mlib_status mlib_ImageChannelMerge4_U8(
    mlib_u8 *dst_u8_0,
    const mlib_u8 *src_u8_0,
    const mlib_u8 *src_u8_1,
    const mlib_u8 *src_u8_2,
    const mlib_u8 *src_u8_3,
    mlib_s32 height,
    mlib_s32 width,
    mlib_s32 dst_stride,
    mlib_s32 src0_stride,
    mlib_s32 src1_stride,
    mlib_s32 src2_stride,
    mlib_s32 src3_stride);

mlib_status mlib_ImageChannelMerge3_U8(
    mlib_u8 *dst_u8_0,
    const mlib_u8 *src_u8_0,
    const mlib_u8 *src_u8_1,
    const mlib_u8 *src_u8_2,
    mlib_s32 height,
    mlib_s32 width,
    mlib_s32 dst_stride,
    mlib_s32 src0_stride,
    mlib_s32 src1_stride,
    mlib_s32 src2_stride);

#ifdef __cplusplus
}
#endif

#endif /* _MLIB_IMAGECHANNELMERGE_H */
