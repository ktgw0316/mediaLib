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

#ifndef _MLIB_V_IMAGEMULALPHA_H
#define	_MLIB_V_IMAGEMULALPHA_H

#pragma ident	"@(#)mlib_v_ImageMulAlpha.h	9.2	07/11/05 SMI"

#ifdef __cplusplus
extern "C" {
#endif

#include <mlib_types.h>
#include <mlib_status.h>

extern const mlib_u64 mlib_amask3_arr[];

mlib_status mlib_ImageMulAlpha_U8(
    mlib_u8 *sl,
    mlib_u8 *dl,
    mlib_s32 sstride,
    mlib_s32 dstride,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s32 channel,
    mlib_s32 alpha);

mlib_status mlib_ImageMulAlpha_U16(
    mlib_u16 *sl,
    mlib_u16 *dl,
    mlib_s32 sstride,
    mlib_s32 dstride,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s32 channel,
    mlib_s32 alpha);

mlib_status mlib_ImageMulAlpha_S16(
    mlib_s16 *sl,
    mlib_s16 *dl,
    mlib_s32 sstride,
    mlib_s32 dstride,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s32 channel,
    mlib_s32 alpha);

mlib_status mlib_ImageMulAlpha_S32(
    mlib_s32 *sl,
    mlib_s32 *dl,
    mlib_s32 sstride,
    mlib_s32 dstride,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s32 channel,
    mlib_s32 alpha);

void mlib_v_ImageMulAlpha_Inp_U8_4_NOOFF_1(
    mlib_d64 *src_dst,
    mlib_s32 size);

void mlib_v_ImageMulAlpha_Inp_U8_4_NOOFF_2(
    mlib_d64 *src_dst,
    mlib_s32 size);

void mlib_v_ImageMulAlpha_Inp_U8_4_NOOFF_3(
    mlib_d64 *src_dst,
    mlib_s32 size);

void mlib_v_ImageMulAlpha_Inp_U8_4_NOOFF_4(
    mlib_d64 *src_dst,
    mlib_s32 size);

void mlib_v_ImageMulAlpha_Inp_U8_4_HALFOFF_1(
    mlib_d64 *src_dst,
    mlib_s32 src_dst_stride,
    mlib_s32 size,
    mlib_s32 height);

void mlib_v_ImageMulAlpha_Inp_U8_4_HALFOFF_2(
    mlib_d64 *src_dst,
    mlib_s32 src_dst_stride,
    mlib_s32 size,
    mlib_s32 height);

void mlib_v_ImageMulAlpha_Inp_U8_4_HALFOFF_3(
    mlib_d64 *src_dst,
    mlib_s32 src_dst_stride,
    mlib_s32 size,
    mlib_s32 height);

void mlib_v_ImageMulAlpha_Inp_U8_4_HALFOFF_4(
    mlib_d64 *src_dst,
    mlib_s32 src_dst_stride,
    mlib_s32 size,
    mlib_s32 height);

mlib_status mlib_v_ImageMulAlpha_Inp_U8_4_OFF_1(
    mlib_u8 *s_data,
    mlib_s32 s_stride,
    mlib_s32 width,
    mlib_s32 height);

mlib_status mlib_v_ImageMulAlpha_Inp_U8_4_OFF_2(
    mlib_u8 *s_data,
    mlib_s32 s_stride,
    mlib_s32 width,
    mlib_s32 height);

mlib_status mlib_v_ImageMulAlpha_Inp_U8_4_OFF_3(
    mlib_u8 *s_data,
    mlib_s32 s_stride,
    mlib_s32 width,
    mlib_s32 height);

mlib_status mlib_v_ImageMulAlpha_Inp_U8_4_OFF_4(
    mlib_u8 *s_data,
    mlib_s32 s_stride,
    mlib_s32 width,
    mlib_s32 height);

void mlib_v_ImageMulAlpha_Inp_S16_4_NOOFF_1(
    mlib_d64 *src_dst,
    mlib_s32 size);

void mlib_v_ImageMulAlpha_Inp_S16_4_NOOFF_2(
    mlib_d64 *src_dst,
    mlib_s32 size);

void mlib_v_ImageMulAlpha_Inp_S16_4_NOOFF_3(
    mlib_d64 *src_dst,
    mlib_s32 size);

void mlib_v_ImageMulAlpha_Inp_S16_4_NOOFF_4(
    mlib_d64 *src_dst,
    mlib_s32 size);

void mlib_v_ImageMulAlpha_Inp_S16_4_HALFOFF_1(
    mlib_d64 *src_dst,
    mlib_s32 src_dst_stride,
    mlib_s32 size,
    mlib_s32 height);

void mlib_v_ImageMulAlpha_Inp_S16_4_HALFOFF_2(
    mlib_d64 *src_dst,
    mlib_s32 src_dst_stride,
    mlib_s32 size,
    mlib_s32 height);

void mlib_v_ImageMulAlpha_Inp_S16_4_HALFOFF_3(
    mlib_d64 *src_dst,
    mlib_s32 src_dst_stride,
    mlib_s32 size,
    mlib_s32 height);

void mlib_v_ImageMulAlpha_Inp_S16_4_HALFOFF_4(
    mlib_d64 *src_dst,
    mlib_s32 src_dst_stride,
    mlib_s32 size,
    mlib_s32 height);

void mlib_v_ImageMulAlpha_Inp_S16_2(
    mlib_s16 *src,
    mlib_s32 stride,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s32 alpha);

void mlib_v_ImageMulAlpha_Inp_S16_3(
    mlib_s16 *src,
    mlib_s32 stride,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s32 alpha);

void mlib_v_ImageMulAlpha_Inp_U8_2_NOOFF_1(
    mlib_d64 *src,
    mlib_s32 size);

void mlib_v_ImageMulAlpha_Inp_U8_2_NOOFF_2(
    mlib_d64 *src,
    mlib_s32 size);

void mlib_v_ImageMulAlpha_Inp_U8_2_HALFOFF_1(
    mlib_d64 *src,
    mlib_s32 stride,
    mlib_s32 size,
    mlib_s32 height);

void mlib_v_ImageMulAlpha_Inp_U8_2_HALFOFF_2(
    mlib_d64 *src,
    mlib_s32 stride,
    mlib_s32 size,
    mlib_s32 height);

mlib_status mlib_v_ImageMulAlpha_Inp_U8_2_OFF_1(
    mlib_u8 *s_data,
    mlib_s32 s_stride,
    mlib_s32 width,
    mlib_s32 height);

mlib_status mlib_v_ImageMulAlpha_Inp_U8_2_OFF_2(
    mlib_u8 *s_data,
    mlib_s32 s_stride,
    mlib_s32 width,
    mlib_s32 height);

void mlib_v_ImageMulAlpha_Inp_S32_2(
    mlib_s32 *src,
    mlib_s32 stride,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s32 alpha);

void mlib_v_ImageMulAlpha_Inp_S32_3(
    mlib_s32 *src,
    mlib_s32 stride,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s32 alpha);

void mlib_v_ImageMulAlpha_Inp_S32_4(
    mlib_s32 *src,
    mlib_s32 stride,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s32 alpha);

void mlib_v_ImageMulAlpha_Inp_U16_4_NOOFF_1(
    mlib_d64 *src,
    mlib_s32 size);

void mlib_v_ImageMulAlpha_Inp_U16_4_NOOFF_2(
    mlib_d64 *src,
    mlib_s32 size);

void mlib_v_ImageMulAlpha_Inp_U16_4_NOOFF_3(
    mlib_d64 *src,
    mlib_s32 size);

void mlib_v_ImageMulAlpha_Inp_U16_4_NOOFF_4(
    mlib_d64 *src,
    mlib_s32 size);

void mlib_v_ImageMulAlpha_Inp_U16_4_HALFOFF_1(
    mlib_d64 *src,
    mlib_s32 src_stride,
    mlib_s32 size,
    mlib_s32 height);

void mlib_v_ImageMulAlpha_Inp_U16_4_HALFOFF_2(
    mlib_d64 *src,
    mlib_s32 src_stride,
    mlib_s32 size,
    mlib_s32 height);

void mlib_v_ImageMulAlpha_Inp_U16_4_HALFOFF_3(
    mlib_d64 *src,
    mlib_s32 src_stride,
    mlib_s32 size,
    mlib_s32 height);

void mlib_v_ImageMulAlpha_Inp_U16_4_HALFOFF_4(
    mlib_d64 *src,
    mlib_s32 src_stride,
    mlib_s32 size,
    mlib_s32 height);

mlib_status mlib_v_ImageMulAlpha_Inp_U16_4_OFF_1(
    mlib_u8 *s_data,
    mlib_s32 s_stride,
    mlib_s32 width,
    mlib_s32 height);

mlib_status mlib_v_ImageMulAlpha_Inp_U16_4_OFF_2(
    mlib_u8 *s_data,
    mlib_s32 s_stride,
    mlib_s32 width,
    mlib_s32 height);

mlib_status mlib_v_ImageMulAlpha_Inp_U16_4_OFF_3(
    mlib_u8 *s_data,
    mlib_s32 s_stride,
    mlib_s32 width,
    mlib_s32 height);

mlib_status mlib_v_ImageMulAlpha_Inp_U16_4_OFF_4(
    mlib_u8 *s_data,
    mlib_s32 s_stride,
    mlib_s32 width,
    mlib_s32 height);

void mlib_v_ImageMulAlpha_Inp_U16_3(
    mlib_u16 *src,
    mlib_s32 s_stride,
    mlib_u16 *dst,
    mlib_s32 d_stride,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s32 alpha);

void mlib_v_ImageMulAlpha_Inp_U16_2(
    mlib_u16 *src,
    mlib_s32 s_stride,
    mlib_u16 *dst,
    mlib_s32 d_stride,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s32 alpha);

#ifdef __cplusplus
}
#endif

#endif /* _MLIB_V_IMAGEMULALPHA_H */
