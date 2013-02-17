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

#ifndef _MLIB_V_IMAGEDIVALPHA_H
#define	_MLIB_V_IMAGEDIVALPHA_H

#pragma ident	"@(#)mlib_v_ImageDivAlpha.h	9.2	07/11/05 SMI"

#ifdef __cplusplus
extern "C" {
#endif

#include <mlib_types.h>

#if defined(__INIT_TABLE)
#pragma align 8(mlib_v_tab1)
#pragma align 8(mlib_v_tab2)
#pragma align 8(mlib_v_tab3)
#pragma align 8(mlib_v_tab4)
#endif		   /* defined(__INIT_TABLE) */
extern const mlib_u64 mlib_DivAlpha_tbl[];
extern const mlib_u64 mlib_DivAlpha_tbl4[];
extern const mlib_f32 mlib_v_blend_u8_sat[];
extern const mlib_f32 mlib_v_blend_u8_sat[];
extern const mlib_f32 mlib_div_tab_Q8[];
extern const mlib_u16 mlib_v_tab1[];
extern const mlib_u16 mlib_v_tab2[];
extern const mlib_u16 mlib_v_tab3[];
extern const mlib_u16 mlib_v_tab4[];

void mlib_v_ImageDivAlpha_Inp_U8_4_NOOFF_1(
    mlib_d64 *src_dst,
    mlib_s32 size);

void mlib_v_ImageDivAlpha_Inp_U8_4_NOOFF_2(
    mlib_d64 *src_dst,
    mlib_s32 size);

void mlib_v_ImageDivAlpha_Inp_U8_4_NOOFF_3(
    mlib_d64 *src_dst,
    mlib_s32 size);

void mlib_v_ImageDivAlpha_Inp_U8_4_NOOFF_4(
    mlib_d64 *src_dst,
    mlib_s32 size);

void mlib_v_ImageDivAlpha_Inp_U8_4_HALFOFF_1(
    mlib_d64 *src_dst,
    mlib_s32 src_dst_stride,
    mlib_s32 size,
    mlib_s32 height);

void mlib_v_ImageDivAlpha_Inp_U8_4_HALFOFF_2(
    mlib_d64 *src_dst,
    mlib_s32 src_dst_stride,
    mlib_s32 size,
    mlib_s32 height);

void mlib_v_ImageDivAlpha_Inp_U8_4_HALFOFF_3(
    mlib_d64 *src_dst,
    mlib_s32 src_dst_stride,
    mlib_s32 size,
    mlib_s32 height);

void mlib_v_ImageDivAlpha_Inp_U8_4_HALFOFF_4(
    mlib_d64 *src_dst,
    mlib_s32 src_dst_stride,
    mlib_s32 size,
    mlib_s32 height);

void mlib_v_ImageDivAlpha_Inp_U8_2_NOOFF_1(
    mlib_d64 *src_dst,
    mlib_s32 size);

void mlib_v_ImageDivAlpha_Inp_U8_2_NOOFF_2(
    mlib_d64 *src_dst,
    mlib_s32 size);

void mlib_v_ImageDivAlpha_Inp_U8_2_HALFOFF_1(
    mlib_d64 *src_dst,
    mlib_s32 stride,
    mlib_s32 size,
    mlib_s32 height);

void mlib_v_ImageDivAlpha_Inp_U8_2_HALFOFF_2(
    mlib_d64 *src_dst,
    mlib_s32 stride,
    mlib_s32 size,
    mlib_s32 height);

void mlib_v_ImageDivAlpha_Inp_U8_2_OFF_1(
    mlib_u8 *s_data,
    mlib_s32 stride,
    mlib_s32 width,
    mlib_s32 height);

void mlib_v_ImageDivAlpha_Inp_U8_2_OFF_2(
    mlib_u8 *s_data,
    mlib_s32 stride,
    mlib_s32 width,
    mlib_s32 height);

void mlib_v_ImageDivAlpha_Inp_S16_2(
    mlib_s16 *src,
    mlib_s32 s_stride,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s32 alpha);

void mlib_v_ImageDivAlpha_Inp_S32_2(
    mlib_s32 *src,
    mlib_s32 s_stride,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s32 alpha);

void mlib_v_ImageDivAlpha_Inp_U8_3(
    mlib_u8 *src,
    mlib_s32 s_stride,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s32 alpha);

void mlib_v_ImageDivAlpha_Inp_S16_3(
    mlib_s16 *src,
    mlib_s32 s_stride,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s32 alpha);

void mlib_v_ImageDivAlpha_Inp_S32_3(
    mlib_s32 *src,
    mlib_s32 s_stride,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s32 alpha);

void mlib_v_ImageDivAlpha_Inp_S16_4(
    mlib_s16 *src,
    mlib_s32 stride,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s32 alpha);

void mlib_v_ImageDivAlpha_Inp_S32_4(
    mlib_s32 *src,
    mlib_s32 stride,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s32 alpha);

void mlib_v_ImageDivAlpha_Inp_U16_4(
    mlib_u16 *src,
    mlib_s32 stride,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s32 alpha);

void mlib_v_ImageDivAlpha_Inp_U16_3(
    mlib_u16 *src,
    mlib_s32 s_stride,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s32 alpha);

void mlib_v_ImageDivAlpha_Inp_U16_2(
    mlib_u16 *src,
    mlib_s32 s_stride,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s32 alpha);

#ifdef __cplusplus
}
#endif

#endif /* _MLIB_V_IMAGEDIVALPHA_H */
