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

#ifndef _MLIB_V_IMAGEBLENDFUNC_H
#define	_MLIB_V_IMAGEBLENDFUNC_H

#pragma ident	"@(#)mlib_v_ImageBlendFunc.h	9.2	07/11/05 SMI"

#ifdef __cplusplus
extern "C" {
#endif

#include <mlib_types.h>

void mlib_v_ImageBlend_U8_A8D1X8(
    mlib_u8 *src1,
    mlib_u8 *src2,
    mlib_u8 *dst,
    mlib_u8 *aimg,
    mlib_s32 dsize);

void mlib_v_ImageBlend_U8_A8D2(
    mlib_u8 *src1,
    mlib_s32 slb1,
    mlib_u8 *src2,
    mlib_s32 slb2,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_u8 *aimg,
    mlib_s32 alb,
    mlib_s32 xsize,
    mlib_s32 ysize);

void mlib_v_ImageBlend_U8(
    mlib_u8 *src1,
    mlib_s32 slb1,
    mlib_u8 *src2,
    mlib_s32 slb2,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_u8 *aimg,
    mlib_s32 alb,
    mlib_s32 xsize,
    mlib_s32 ysize);

void mlib_v_ImageBlend_S16_A8D1X4(
    mlib_s16 *src1,
    mlib_s16 *src2,
    mlib_s16 *dst,
    mlib_s16 *aimg,
    mlib_s32 dsize);

void mlib_v_ImageBlend_S16_A8D2(
    mlib_s16 *src1,
    mlib_s32 slb1,
    mlib_s16 *src2,
    mlib_s32 slb2,
    mlib_s16 *dst,
    mlib_s32 dlb,
    mlib_s16 *aimg,
    mlib_s32 alb,
    mlib_s32 xsize,
    mlib_s32 ysize);

void mlib_v_ImageBlend_S16(
    mlib_s16 *src1,
    mlib_s32 slb1,
    mlib_s16 *src2,
    mlib_s32 slb2,
    mlib_s16 *dst,
    mlib_s32 dlb,
    mlib_s16 *aimg,
    mlib_s32 alb,
    mlib_s32 xsize,
    mlib_s32 ysize);

void mlib_v_ImageBlend_U16_A8D1X4(
    mlib_u16 *src1,
    mlib_u16 *src2,
    mlib_u16 *dst,
    mlib_u16 *aimg,
    mlib_s32 dsize);

void mlib_v_ImageBlend_U16_A8D2(
    mlib_u16 *src1,
    mlib_s32 slb1,
    mlib_u16 *src2,
    mlib_s32 slb2,
    mlib_u16 *dst,
    mlib_s32 dlb,
    mlib_u16 *aimg,
    mlib_s32 alb,
    mlib_s32 xsize,
    mlib_s32 ysize);

void mlib_v_ImageBlend_U16(
    mlib_u16 *src1,
    mlib_s32 slb1,
    mlib_u16 *src2,
    mlib_s32 slb2,
    mlib_u16 *dst,
    mlib_s32 dlb,
    mlib_u16 *aimg,
    mlib_s32 alb,
    mlib_s32 xsize,
    mlib_s32 ysize);

void mlib_v_ImageBlend_S32(
    mlib_s32 *sa1,
    mlib_s32 slb1,
    mlib_s32 *sa2,
    mlib_s32 slb2,
    mlib_s32 *da,
    mlib_s32 dlb,
    mlib_s32 *aa,
    mlib_s32 alb,
    mlib_s32 xsize,
    mlib_s32 ysize);

void mlib_v_ImageBlendSA_U8_2_A8D1X8(
    mlib_u8 *src1,
    mlib_u8 *src2,
    mlib_u8 *dst,
    mlib_u8 *aimg,
    mlib_s32 dsize);

void mlib_v_ImageBlendSA_U8_2_A8D2(
    mlib_u8 *src1,
    mlib_s32 slb1,
    mlib_u8 *src2,
    mlib_s32 slb2,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_u8 *aimg,
    mlib_s32 alb,
    mlib_s32 xsize,
    mlib_s32 ysize);

void mlib_v_ImageBlendSA_U8_2_DstA8D1(
    mlib_u8 *src1,
    mlib_u8 *src2,
    mlib_u8 *dst,
    mlib_u8 *aimg,
    mlib_s32 xsize);

void mlib_v_ImageBlendSA_U8_2_D1(
    mlib_u8 *src1,
    mlib_u8 *src2,
    mlib_u8 *dst,
    mlib_u8 *aimg,
    mlib_s32 xsize);

void mlib_v_ImageBlendSA_U8_2(
    mlib_u8 *src1,
    mlib_s32 slb1,
    mlib_u8 *src2,
    mlib_s32 slb2,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_u8 *aimg,
    mlib_s32 alb,
    mlib_s32 xsize,
    mlib_s32 ysize);

void mlib_v_ImageBlendSA_U8_3_A8D1(
    mlib_u8 *src1,
    mlib_u8 *src2,
    mlib_u8 *dst,
    mlib_u8 *aimg,
    mlib_s32 dsize);

void mlib_v_ImageBlendSA_U8_3_A8D2(
    mlib_u8 *src1,
    mlib_s32 slb1,
    mlib_u8 *src2,
    mlib_s32 slb2,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_u8 *aimg,
    mlib_s32 alb,
    mlib_s32 xsize,
    mlib_s32 ysize);

void mlib_v_ImageBlendSA_U8_3_DstA8D1(
    mlib_u8 *sa1,
    mlib_u8 *sa2,
    mlib_u8 *dst,
    mlib_u8 *aimg,
    mlib_s32 xsize);

void mlib_v_ImageBlendSA_U8_3(
    mlib_u8 *src1,
    mlib_s32 slb1,
    mlib_u8 *src2,
    mlib_s32 slb2,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_u8 *aimg,
    mlib_s32 alb,
    mlib_s32 xsize,
    mlib_s32 ysize);

void mlib_v_ImageBlendSA_U8_4_A8D1X8(
    mlib_u8 *src1,
    mlib_u8 *src2,
    mlib_u8 *dst,
    mlib_u8 *aimg,
    mlib_s32 dsize);

void mlib_v_ImageBlendSA_U8_4_A8D2(
    mlib_u8 *src1,
    mlib_s32 slb1,
    mlib_u8 *src2,
    mlib_s32 slb2,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_u8 *aimg,
    mlib_s32 alb,
    mlib_s32 xsize,
    mlib_s32 ysize);

void mlib_v_ImageBlendSA_U8_4(
    mlib_u8 *src1,
    mlib_s32 slb1,
    mlib_u8 *src2,
    mlib_s32 slb2,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_u8 *aimg,
    mlib_s32 alb,
    mlib_s32 xsize,
    mlib_s32 ysize);

void mlib_v_ImageBlendSA_S16_2_A8D1X4(
    mlib_s16 *src1,
    mlib_s16 *src2,
    mlib_s16 *dst,
    mlib_s16 *aimg,
    mlib_s32 dsize);

void mlib_v_ImageBlendSA_S16_2_A8D2(
    mlib_s16 *src1,
    mlib_s32 slb1,
    mlib_s16 *src2,
    mlib_s32 slb2,
    mlib_s16 *dst,
    mlib_s32 dlb,
    mlib_s16 *aimg,
    mlib_s32 alb,
    mlib_s32 xsize,
    mlib_s32 ysize);

void mlib_v_ImageBlendSA_S16_2_DstA8D1(
    mlib_s16 *sa1,
    mlib_s16 *sa2,
    mlib_s16 *dst,
    mlib_s16 *aimg,
    mlib_s32 xsize);

void mlib_v_ImageBlendSA_S16_2_D1(
    mlib_s16 *sa1,
    mlib_s16 *sa2,
    mlib_s16 *dst,
    mlib_s16 *aimg,
    mlib_s32 xsize);

void mlib_v_ImageBlendSA_S16_2(
    mlib_s16 *src1,
    mlib_s32 slb1,
    mlib_s16 *src2,
    mlib_s32 slb2,
    mlib_s16 *dst,
    mlib_s32 dlb,
    mlib_s16 *aimg,
    mlib_s32 alb,
    mlib_s32 xsize,
    mlib_s32 ysize);

void mlib_v_ImageBlendSA_S16_3_A8D1(
    mlib_s16 *src1,
    mlib_s16 *src2,
    mlib_s16 *dst,
    mlib_s16 *aimg,
    mlib_s32 dsize);

void mlib_v_ImageBlendSA_S16_3_A8D2(
    mlib_s16 *src1,
    mlib_s32 slb1,
    mlib_s16 *src2,
    mlib_s32 slb2,
    mlib_s16 *dst,
    mlib_s32 dlb,
    mlib_s16 *aimg,
    mlib_s32 alb,
    mlib_s32 xsize,
    mlib_s32 ysize);

void mlib_v_ImageBlendSA_S16_3_DstA8D1(
    mlib_s16 *src1,
    mlib_s16 *src2,
    mlib_s16 *dst,
    mlib_s16 *aimg,
    mlib_s32 xsize);

void mlib_v_ImageBlendSA_S16_3(
    mlib_s16 *src1,
    mlib_s32 slb1,
    mlib_s16 *src2,
    mlib_s32 slb2,
    mlib_s16 *dst,
    mlib_s32 dlb,
    mlib_s16 *aimg,
    mlib_s32 alb,
    mlib_s32 xsize,
    mlib_s32 ysize);

void mlib_v_ImageBlendSA_S16_4_A8D1(
    mlib_s16 *src1,
    mlib_s16 *src2,
    mlib_s16 *dst,
    mlib_s16 *aimg,
    mlib_s32 dsize);

void mlib_v_ImageBlendSA_S16_4_A8D2(
    mlib_s16 *src1,
    mlib_s32 slb1,
    mlib_s16 *src2,
    mlib_s32 slb2,
    mlib_s16 *dst,
    mlib_s32 dlb,
    mlib_s16 *aimg,
    mlib_s32 alb,
    mlib_s32 xsize,
    mlib_s32 ysize);

void mlib_v_ImageBlendSA_S16_4(
    mlib_s16 *src1,
    mlib_s32 slb1,
    mlib_s16 *src2,
    mlib_s32 slb2,
    mlib_s16 *dst,
    mlib_s32 dlb,
    mlib_s16 *aimg,
    mlib_s32 alb,
    mlib_s32 xsize,
    mlib_s32 ysize);

void mlib_v_ImageBlendSA_U16_2_A8D1X4(
    mlib_u16 *src1,
    mlib_u16 *src2,
    mlib_u16 *dst,
    mlib_u16 *aimg,
    mlib_s32 dsize);

void mlib_v_ImageBlendSA_U16_2_A8D2(
    mlib_u16 *src1,
    mlib_s32 slb1,
    mlib_u16 *src2,
    mlib_s32 slb2,
    mlib_u16 *dst,
    mlib_s32 dlb,
    mlib_u16 *aimg,
    mlib_s32 alb,
    mlib_s32 xsize,
    mlib_s32 ysize);

void mlib_v_ImageBlendSA_U16_2_DstA8D1(
    mlib_u16 *sa1,
    mlib_u16 *sa2,
    mlib_u16 *dst,
    mlib_u16 *aimg,
    mlib_s32 xsize);

void mlib_v_ImageBlendSA_U16_2_D1(
    mlib_u16 *sa1,
    mlib_u16 *sa2,
    mlib_u16 *dst,
    mlib_u16 *aimg,
    mlib_s32 xsize);

void mlib_v_ImageBlendSA_U16_2(
    mlib_u16 *src1,
    mlib_s32 slb1,
    mlib_u16 *src2,
    mlib_s32 slb2,
    mlib_u16 *dst,
    mlib_s32 dlb,
    mlib_u16 *aimg,
    mlib_s32 alb,
    mlib_s32 xsize,
    mlib_s32 ysize);

void mlib_v_ImageBlendSA_U16_3_A8D1(
    mlib_u16 *src1,
    mlib_u16 *src2,
    mlib_u16 *dst,
    mlib_u16 *aimg,
    mlib_s32 dsize);

void mlib_v_ImageBlendSA_U16_3_A8D2(
    mlib_u16 *src1,
    mlib_s32 slb1,
    mlib_u16 *src2,
    mlib_s32 slb2,
    mlib_u16 *dst,
    mlib_s32 dlb,
    mlib_u16 *aimg,
    mlib_s32 alb,
    mlib_s32 xsize,
    mlib_s32 ysize);

void mlib_v_ImageBlendSA_U16_3_DstA8D1(
    mlib_u16 *src1,
    mlib_u16 *src2,
    mlib_u16 *dst,
    mlib_u16 *aimg,
    mlib_s32 xsize);

void mlib_v_ImageBlendSA_U16_3(
    mlib_u16 *src1,
    mlib_s32 slb1,
    mlib_u16 *src2,
    mlib_s32 slb2,
    mlib_u16 *dst,
    mlib_s32 dlb,
    mlib_u16 *aimg,
    mlib_s32 alb,
    mlib_s32 xsize,
    mlib_s32 ysize);

void mlib_v_ImageBlendSA_U16_4_A8D1(
    mlib_u16 *src1,
    mlib_u16 *src2,
    mlib_u16 *dst,
    mlib_u16 *aimg,
    mlib_s32 dsize);

void mlib_v_ImageBlendSA_U16_4_A8D2(
    mlib_u16 *src1,
    mlib_s32 slb1,
    mlib_u16 *src2,
    mlib_s32 slb2,
    mlib_u16 *dst,
    mlib_s32 dlb,
    mlib_u16 *aimg,
    mlib_s32 alb,
    mlib_s32 xsize,
    mlib_s32 ysize);

void mlib_v_ImageBlendSA_U16_4(
    mlib_u16 *src1,
    mlib_s32 slb1,
    mlib_u16 *src2,
    mlib_s32 slb2,
    mlib_u16 *dst,
    mlib_s32 dlb,
    mlib_u16 *aimg,
    mlib_s32 alb,
    mlib_s32 xsize,
    mlib_s32 ysize);

void mlib_v_ImageBlendSA_S32(
    mlib_s32 *sa1,
    mlib_s32 slb1,
    mlib_s32 *sa2,
    mlib_s32 slb2,
    mlib_s32 *da,
    mlib_s32 dlb,
    mlib_s32 *aa,
    mlib_s32 alb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 csize);

#ifdef __cplusplus
}
#endif

#endif /* _MLIB_V_IMAGEBLENDFUNC_H */
