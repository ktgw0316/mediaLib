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

#ifndef _MLIB_V_IMAGECHANNELEXTRACT_H
#define	_MLIB_V_IMAGECHANNELEXTRACT_H

#pragma ident	"@(#)mlib_v_ImageChannelExtract.h	9.2	07/11/05 SMI"

#ifdef __cplusplus
extern "C" {
#endif

#include <mlib_types.h>

void mlib_v_ImageChannelExtract_U8_21_A8D1X8(
    const mlib_u8 *src,
    mlib_u8 *dst,
    mlib_s32 dsize,
    mlib_s32 cmask);

void mlib_v_ImageChannelExtract_U8_21_A8D2X8(
    const mlib_u8 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 cmask);

void mlib_v_ImageChannelExtract_U8_21_D1(
    const mlib_u8 *src,
    mlib_u8 *dst,
    mlib_s32 dsize,
    mlib_s32 cmask);

void mlib_v_ImageChannelExtract_U8_21(
    const mlib_u8 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 cmask);

void mlib_v_ImageChannelExtract_U8_31_A8D1X8(
    const mlib_u8 *src,
    mlib_u8 *dst,
    mlib_s32 dsize,
    mlib_s32 cmask);

void mlib_v_ImageChannelExtract_U8_31_A8D2X8(
    const mlib_u8 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 cmask);

void mlib_v_ImageChannelExtract_U8_31_D1(
    const mlib_u8 *src,
    mlib_u8 *dst,
    mlib_s32 dsize,
    mlib_s32 cmask);

void mlib_v_ImageChannelExtract_U8_31(
    const mlib_u8 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 cmask);

void mlib_v_ImageChannelExtract_U8_41_A8D1X8(
    const mlib_u8 *src,
    mlib_u8 *dst,
    mlib_s32 dsize,
    mlib_s32 cmask);

void mlib_v_ImageChannelExtract_U8_41_A8D2X8(
    const mlib_u8 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 cmask);

void mlib_v_ImageChannelExtract_U8_41_D1(
    const mlib_u8 *src,
    mlib_u8 *dst,
    mlib_s32 dsize,
    mlib_s32 cmask);

void mlib_v_ImageChannelExtract_U8_41(
    const mlib_u8 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 cmask);

void mlib_v_ImageChannelExtract_S16_21_A8D1X4(
    const mlib_s16 *src,
    mlib_s16 *dst,
    mlib_s32 dsize,
    mlib_s32 cmask);

void mlib_v_ImageChannelExtract_S16_21_A8D2X4(
    const mlib_s16 *src,
    mlib_s32 slb,
    mlib_s16 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 cmask);

void mlib_v_ImageChannelExtract_S16_21_D1(
    const mlib_s16 *src,
    mlib_s16 *dst,
    mlib_s32 dsize,
    mlib_s32 cmask);

void mlib_v_ImageChannelExtract_S16_21(
    const mlib_s16 *src,
    mlib_s32 slb,
    mlib_s16 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 cmask);

void mlib_v_ImageChannelExtract_S16_31_A8D1X4(
    const mlib_s16 *src,
    mlib_s16 *dst,
    mlib_s32 dsize,
    mlib_s32 cmask);

void mlib_v_ImageChannelExtract_S16_31_A8D2X4(
    const mlib_s16 *src,
    mlib_s32 slb,
    mlib_s16 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 cmask);

void mlib_v_ImageChannelExtract_S16_31_D1(
    const mlib_s16 *src,
    mlib_s16 *dst,
    mlib_s32 dsize,
    mlib_s32 cmask);

void mlib_v_ImageChannelExtract_S16_31(
    const mlib_s16 *src,
    mlib_s32 slb,
    mlib_s16 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 cmask);

void mlib_v_ImageChannelExtract_S16_41_A8D1X4(
    const mlib_s16 *src,
    mlib_s16 *dst,
    mlib_s32 dsize,
    mlib_s32 cmask);

void mlib_v_ImageChannelExtract_S16_41_A8D2X4(
    const mlib_s16 *src,
    mlib_s32 slb,
    mlib_s16 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 cmask);

void mlib_v_ImageChannelExtract_S16_41_D1(
    const mlib_s16 *src,
    mlib_s16 *dst,
    mlib_s32 dsize,
    mlib_s32 cmask);

void mlib_v_ImageChannelExtract_S16_41(
    const mlib_s16 *src,
    mlib_s32 slb,
    mlib_s16 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 cmask);

void mlib_v_ImageChannelExtract_U8_43R_A8D1X8(
    const mlib_u8 *src,
    mlib_u8 *dst,
    mlib_s32 dsize);

void mlib_v_ImageChannelExtract_U8_43R_A8D2X8(
    const mlib_u8 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize);

void mlib_v_ImageChannelExtract_U8_43R_D1(
    const mlib_u8 *src,
    mlib_u8 *dst,
    mlib_s32 dsize);

void mlib_v_ImageChannelExtract_U8_43R(
    const mlib_u8 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize);

void mlib_v_ImageChannelExtract_S16_43R_A8D1X4(
    const mlib_s16 *src,
    mlib_s16 *dst,
    mlib_s32 dsize);

void mlib_v_ImageChannelExtract_S16_43R_A8D2X4(
    const mlib_s16 *src,
    mlib_s32 slb,
    mlib_s16 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize);

void mlib_v_ImageChannelExtract_S16_43R_D1(
    const mlib_s16 *src,
    mlib_s16 *dst,
    mlib_s32 dsize);

void mlib_v_ImageChannelExtract_S16_43R(
    const mlib_s16 *src,
    mlib_s32 slb,
    mlib_s16 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize);

void mlib_v_ImageChannelExtract_U8_43L_A8D1X8(
    const mlib_u8 *src,
    mlib_u8 *dst,
    mlib_s32 dsize);

void mlib_v_ImageChannelExtract_U8_43L_A8D2X8(
    const mlib_u8 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize);

void mlib_v_ImageChannelExtract_U8_43L_D1(
    const mlib_u8 *src,
    mlib_u8 *dst,
    mlib_s32 dsize);

void mlib_v_ImageChannelExtract_U8_43L(
    const mlib_u8 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize);

void mlib_v_ImageChannelExtract_S16_43L_A8D1X4(
    const mlib_s16 *src,
    mlib_s16 *dst,
    mlib_s32 dsize);

void mlib_v_ImageChannelExtract_S16_43L_A8D2X4(
    const mlib_s16 *src,
    mlib_s32 slb,
    mlib_s16 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize);

void mlib_v_ImageChannelExtract_S16_43L_D1(
    const mlib_s16 *src,
    mlib_s16 *dst,
    mlib_s32 dsize);

void mlib_v_ImageChannelExtract_S16_43L(
    const mlib_s16 *src,
    mlib_s32 slb,
    mlib_s16 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize);

void mlib_v_ImageChannelExtract_U8_2_1(
    const mlib_u8 *sl,
    mlib_s32 slb,
    mlib_u8 *dl,
    mlib_s32 dlb,
    mlib_s32 width,
    mlib_s32 height);

void mlib_v_ImageChannelExtract_U8_3_2(
    const mlib_u8 *sl,
    mlib_s32 slb,
    mlib_u8 *dl,
    mlib_s32 dlb,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s32 count1);

void mlib_v_ImageChannelExtract_U8_4_2(
    const mlib_u8 *sl,
    mlib_s32 slb,
    mlib_u8 *dl,
    mlib_s32 dlb,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s32 count1);

void mlib_v_ImageChannelExtract_32_2_1(
    const mlib_f32 *sp,
    mlib_s32 slb,
    mlib_f32 *dp,
    mlib_s32 dlb,
    mlib_s32 width,
    mlib_s32 height);

void mlib_v_ImageChannelExtract_32_3_1(
    const mlib_f32 *sl,
    mlib_s32 slb,
    mlib_f32 *dl,
    mlib_s32 dlb,
    mlib_s32 width,
    mlib_s32 height);

void mlib_v_ImageChannelExtract_32_3_2(
    const mlib_f32 *sl,
    mlib_s32 slb,
    mlib_f32 *dl,
    mlib_s32 dlb,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s32 count1);

void mlib_v_ImageChannelExtract_32_4_1(
    const mlib_f32 *sp,
    mlib_s32 slb,
    mlib_f32 *dp,
    mlib_s32 dlb,
    mlib_s32 width,
    mlib_s32 height);

void mlib_v_ImageChannelExtract_32_4_2(
    const mlib_f32 *sl,
    mlib_s32 slb,
    mlib_f32 *dl,
    mlib_s32 dlb,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s32 count1);

void mlib_v_ImageChannelExtract_32_4_3(
    const mlib_f32 *sl,
    mlib_s32 slb,
    mlib_f32 *dl,
    mlib_s32 dlb,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s32 count1);

void mlib_v_ImageChannelExtract_U8(
    const mlib_u8 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 channels,
    mlib_s32 channeld,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s32 cmask);

void mlib_v_ImageChannelExtract_S16(
    const mlib_u16 *src,
    mlib_s32 slb,
    mlib_u16 *dst,
    mlib_s32 dlb,
    mlib_s32 channels,
    mlib_s32 channeld,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s32 cmask);

void mlib_v_ImageChannelExtract_D64(
    const mlib_d64 *src,
    mlib_s32 slb,
    mlib_d64 *dst,
    mlib_s32 dlb,
    mlib_s32 channels,
    mlib_s32 channeld,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s32 cmask);

#ifdef __cplusplus
}
#endif

#endif /* _MLIB_V_IMAGECHANNELEXTRACT_H */
