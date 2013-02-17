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

#ifndef _MLIB_V_IMAGESCALE_H
#define	_MLIB_V_IMAGESCALE_H

#pragma ident	"@(#)mlib_v_ImageScale.h	9.2	07/11/05 SMI"

#ifdef __cplusplus
extern "C" {
#endif

#include <mlib_types.h>
#include <mlib_image_types.h>
#include <mlib_status.h>

mlib_status mlib_ImageScale_VIS(
    mlib_image *dst,
    const mlib_image *src,
    mlib_d64 ad,
    mlib_d64 bd0,
    mlib_d64 bd1,
    mlib_s32 shift,
    mlib_s32 fast_u8_u8);

void mlib_v_ImageScale_U8_S16_124_A8D1X8(
    mlib_u8 *src,
    mlib_s16 *dst,
    mlib_s32 dsize,
    mlib_d64 ad,
    mlib_d64 bdh,
    mlib_d64 bdl);

void mlib_v_ImageScale_U8_S16_124_A8D2(
    mlib_u8 *src,
    mlib_s32 slb,
    mlib_s16 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_d64 ad,
    mlib_d64 bdh,
    mlib_d64 bdl);

void mlib_v_ImageScale_U8_S16_124(
    mlib_u8 *src,
    mlib_s32 slb,
    mlib_s16 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_d64 ad,
    mlib_d64 bdh,
    mlib_d64 bdl);

void mlib_v_ImageScale_U8_S16_3(
    mlib_u8 *src,
    mlib_s32 slb,
    mlib_s16 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_d64 ad,
    mlib_d64 bdh,
    mlib_d64 bdl);

void mlib_v_ImageScale_S16_S32_124(
    mlib_u8 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_f32 *afh,
    mlib_f32 *afl,
    mlib_d64 *bdh,
    mlib_d64 *bdl,
    mlib_s32 shift);

void mlib_v_ImageScale_S16_S32_3(
    mlib_u8 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_f32 *afh,
    mlib_d64 *bdh,
    mlib_s32 shift);

void mlib_v_ImageScale_U8_U8_124_A8D1X8(
    mlib_u8 *src,
    mlib_u8 *dst,
    mlib_s32 dsize,
    mlib_d64 ad,
    mlib_d64 bd);

void mlib_v_ImageScale_U8_U8_124_A8D2(
    mlib_u8 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_d64 ad,
    mlib_d64 bd);

void mlib_v_ImageScale_U8_U8_124(
    mlib_u8 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_d64 ad,
    mlib_d64 bd);

void mlib_v_ImageScale_U8_U8_3_A8D1(
    mlib_u8 *src,
    mlib_u8 *dst,
    mlib_s32 dsize,
    mlib_d64 ad,
    mlib_d64 bd);

void mlib_v_ImageScale_U8_U8_3_A8D2(
    mlib_u8 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_d64 ad,
    mlib_d64 bd);

void mlib_v_ImageScale_U8_U8_3(
    mlib_u8 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_d64 ad,
    mlib_d64 bd);

void mlib_v_ImageScale_S16_S16_124_A8D1X4(
    mlib_s16 *src,
    mlib_s16 *dst,
    mlib_s32 dsize,
    mlib_d64 ad,
    mlib_d64 bdh,
    mlib_d64 bdl);

void mlib_v_ImageScale_S16_S16_124_A8D2(
    mlib_s16 *src,
    mlib_s32 slb,
    mlib_s16 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_d64 ad,
    mlib_d64 bdh,
    mlib_d64 bdl);

void mlib_v_ImageScale_S16_S16_124(
    mlib_s16 *src,
    mlib_s32 slb,
    mlib_s16 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_d64 ad,
    mlib_d64 bdh,
    mlib_d64 bdl);

void mlib_v_ImageScale_S16_S16_3_A8D1(
    mlib_s16 *src,
    mlib_s16 *dst,
    mlib_s32 dsize,
    mlib_d64 ad,
    mlib_d64 bdh,
    mlib_d64 bdl);

void mlib_v_ImageScale_S16_S16_3_A8D2(
    mlib_s16 *src,
    mlib_s32 slb,
    mlib_s16 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_d64 ad,
    mlib_d64 bdh,
    mlib_d64 bdl);

void mlib_v_ImageScale_S16_S16_3(
    mlib_s16 *src,
    mlib_s32 slb,
    mlib_s16 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_d64 ad,
    mlib_d64 bdh,
    mlib_d64 bdl);

void mlib_v_ImageScale_U8_U8_D_124_A8D1X8(
    mlib_u8 *src,
    mlib_u8 *dst,
    mlib_s32 dsize,
    mlib_d64 ad,
    mlib_d64 bdh,
    mlib_d64 bdl);

void mlib_v_ImageScale_U8_U8_D_124_A8D2(
    mlib_u8 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_d64 ad,
    mlib_d64 bdh,
    mlib_d64 bdl);

void mlib_v_ImageScale_U8_U8_D_124(
    mlib_u8 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_d64 ad,
    mlib_d64 bdh,
    mlib_d64 bdl);

void mlib_v_ImageScale_U8_U8_D_3_A8D1(
    mlib_u8 *src,
    mlib_u8 *dst,
    mlib_s32 dsize,
    mlib_d64 ad,
    mlib_d64 bdh,
    mlib_d64 bdl);

void mlib_v_ImageScale_U8_U8_D_3_A8D2(
    mlib_u8 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_d64 ad,
    mlib_d64 bdh,
    mlib_d64 bdl);

void mlib_v_ImageScale_U8_U8_D_3(
    mlib_u8 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_d64 ad,
    mlib_d64 bdh,
    mlib_d64 bdl);

void mlib_v_ImageScale_U8_U16_124(
    mlib_u8 *src,
    mlib_s32 slb,
    mlib_u16 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_d64 ad,
    mlib_d64 bdh,
    mlib_d64 bdl);

void mlib_v_ImageScale_U8_U16_3(
    mlib_u8 *src,
    mlib_s32 slb,
    mlib_u16 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_d64 ad,
    mlib_d64 bdh,
    mlib_d64 bdl);

void mlib_v_ImageScale_S16_U16_124(
    mlib_s16 *src,
    mlib_s32 slb,
    mlib_u16 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_d64 ad,
    mlib_d64 bdh,
    mlib_d64 bdl);

void mlib_v_ImageScale_S16_U16_3(
    mlib_s16 *src,
    mlib_s32 slb,
    mlib_u16 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_d64 ad,
    mlib_d64 bdh,
    mlib_d64 bdl);

void mlib_v_ImageScale_U16_S16_124(
    mlib_u16 *src,
    mlib_s32 slb,
    mlib_s16 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_d64 ad,
    mlib_d64 bdh,
    mlib_d64 bdl);

void mlib_v_ImageScale_U16_S16_3(
    mlib_u16 *src,
    mlib_s32 slb,
    mlib_s16 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_d64 ad,
    mlib_d64 bdh,
    mlib_d64 bdl);

void mlib_v_ImageScale_U16_U16_124(
    mlib_u16 *src,
    mlib_s32 slb,
    mlib_u16 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_d64 ad,
    mlib_d64 bdh,
    mlib_d64 bdl);

void mlib_v_ImageScale_U16_U16_3(
    mlib_u16 *src,
    mlib_s32 slb,
    mlib_u16 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_d64 ad,
    mlib_d64 bdh,
    mlib_d64 bdl);

void mlib_v_ImageScale_U16_S32_124(
    mlib_u8 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_f32 *afh,
    mlib_f32 *afl,
    mlib_d64 *bdh,
    mlib_d64 *bdl,
    mlib_s32 shift);

void mlib_v_ImageScale_U16_S32_3(
    mlib_u8 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_f32 *afh,
    mlib_d64 *bdh,
    mlib_s32 shift);

void mlib_v_ImageScale_S32_S16_124_A8D1(
    mlib_u8 *src,
    mlib_u8 *dst,
    mlib_s32 dsize,
    mlib_d64 ad,
    mlib_d64 bdh,
    mlib_d64 bdl);

void mlib_v_ImageScale_S32_S16_3_A8D1(
    mlib_u8 *src,
    mlib_u8 *dst,
    mlib_s32 dsize,
    mlib_f32 *afh,
    mlib_f32 *afl,
    mlib_d64 *bdh,
    mlib_d64 *bdl);

void mlib_v_ImageScale_S32_S16_124(
    mlib_u8 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_f32 *afh,
    mlib_f32 *afl,
    mlib_d64 *bdh,
    mlib_d64 *bdl);

void mlib_v_ImageScale_S32_S16_3(
    mlib_u8 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_f32 *afh,
    mlib_f32 *afl,
    mlib_d64 *bdh,
    mlib_d64 *bdl);

void mlib_v_ImageScale_S32_U16_124(
    mlib_u8 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_f32 *afh,
    mlib_f32 *afl,
    mlib_d64 *bdh,
    mlib_d64 *bdl);

void mlib_v_ImageScale_S32_U16_3(
    mlib_u8 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_f32 *afh,
    mlib_f32 *afl,
    mlib_d64 *bdh,
    mlib_d64 *bdl);

void mlib_v_ImageScale_S32_U8_124_A8D1(
    mlib_u8 *src,
    mlib_u8 *dst,
    mlib_s32 dsize,
    mlib_d64 ad,
    mlib_d64 bdh,
    mlib_d64 bdl);

void mlib_v_ImageScale_S32_U8_3_A8D1(
    mlib_u8 *src,
    mlib_u8 *dst,
    mlib_s32 dsize,
    mlib_f32 *afh,
    mlib_f32 *afl,
    mlib_d64 *bdh,
    mlib_d64 *bdl);

void mlib_v_ImageScale_S32_U8_124(
    mlib_u8 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_f32 *afh,
    mlib_f32 *afl,
    mlib_d64 *bdh,
    mlib_d64 *bdl);

void mlib_v_ImageScale_S32_U8_3(
    mlib_u8 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_f32 *afh,
    mlib_f32 *afl,
    mlib_d64 *bdh,
    mlib_d64 *bdl);

void mlib_v_ImageScale_S16_U8_124_A8D1X4(
    mlib_s16 *src,
    mlib_u8 *dst,
    mlib_s32 dsize,
    mlib_d64 ad,
    mlib_d64 bdh,
    mlib_d64 bdl);

void mlib_v_ImageScale_S16_U8_124_A8D2(
    mlib_s16 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_d64 ad,
    mlib_d64 bdh,
    mlib_d64 bdl);

void mlib_v_ImageScale_S16_U8_124(
    mlib_s16 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_d64 ad,
    mlib_d64 bdh,
    mlib_d64 bdl);

void mlib_v_ImageScale_S16_U8_3_A8D1(
    mlib_s16 *src,
    mlib_u8 *dst,
    mlib_s32 dsize,
    mlib_d64 ad,
    mlib_d64 bdh,
    mlib_d64 bdl);

void mlib_v_ImageScale_S16_U8_3_A8D2(
    mlib_s16 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_d64 ad,
    mlib_d64 bdh,
    mlib_d64 bdl);

void mlib_v_ImageScale_S16_U8_3(
    mlib_s16 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_d64 ad,
    mlib_d64 bdh,
    mlib_d64 bdl);

void mlib_v_ImageScale_U16_U8_124(
    mlib_u16 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_d64 ad,
    mlib_d64 bdh,
    mlib_d64 bdl);

void mlib_v_ImageScale_U16_U8_3(
    mlib_u16 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_d64 ad,
    mlib_d64 bdh,
    mlib_d64 bdl);

#ifdef __cplusplus
}
#endif

#endif /* _MLIB_V_IMAGESCALE_H */
