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

#ifndef _MLIB_V_IMAGECOPYMASK_F_H
#define	_MLIB_V_IMAGECOPYMASK_F_H

#pragma ident	"@(#)mlib_v_ImageCopyMask_f.h	9.2	07/11/05 SMI"

#ifdef __cplusplus
extern "C" {
#endif

#include <mlib_types.h>

void mlib_v_ImageCopyMask_U8_124_A8D1X8(
    mlib_u8 *src,
    mlib_u8 *mask,
    mlib_u8 *dst,
    mlib_s32 dsize,
    mlib_d64 dthresh);

void mlib_v_ImageCopyMask_U8_124_A8D2(
    mlib_u8 *src,
    mlib_s32 slb,
    mlib_u8 *mask,
    mlib_s32 mlb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_d64 dthresh);

void mlib_v_ImageCopyMask_U8_124(
    mlib_u8 *src,
    mlib_s32 slb,
    mlib_u8 *mask,
    mlib_s32 mlb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_d64 dthresh0);

void mlib_v_ImageCopyMask_U8_3_A8D1(
    mlib_u8 *src,
    mlib_u8 *mask,
    mlib_u8 *dst,
    mlib_s32 dsize,
    mlib_d64 dthresh0);

void mlib_v_ImageCopyMask_U8_3_A8D2(
    mlib_u8 *src,
    mlib_s32 slb,
    mlib_u8 *mask,
    mlib_s32 mlb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_d64 dthresh0);

void mlib_v_ImageCopyMask_U8_3(
    mlib_u8 *src,
    mlib_s32 slb,
    mlib_u8 *mask,
    mlib_s32 mlb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_d64 dthresh0);

void mlib_v_ImageCopyMask_S16_124_A8D1X4(
    mlib_s16 *src,
    mlib_s16 *mask,
    mlib_s16 *dst,
    mlib_s32 dsize,
    mlib_d64 dthresh);

void mlib_v_ImageCopyMask_S16_124_A8D2(
    mlib_s16 *src,
    mlib_s32 slb,
    mlib_s16 *mask,
    mlib_s32 mlb,
    mlib_s16 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_d64 dthresh);

void mlib_v_ImageCopyMask_S16_124(
    mlib_s16 *src,
    mlib_s32 slb,
    mlib_s16 *mask,
    mlib_s32 mlb,
    mlib_s16 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_d64 dthresh0);

void mlib_v_ImageCopyMask_S16_3_A8D1(
    mlib_s16 *src,
    mlib_s16 *mask,
    mlib_s16 *dst,
    mlib_s32 dsize,
    mlib_d64 dthresh0);

void mlib_v_ImageCopyMask_S16_3_A8D2(
    mlib_s16 *src,
    mlib_s32 slb,
    mlib_s16 *mask,
    mlib_s32 mlb,
    mlib_s16 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_d64 dthresh0);

void mlib_v_ImageCopyMask_S16_3(
    mlib_s16 *src,
    mlib_s32 slb,
    mlib_s16 *mask,
    mlib_s32 mlb,
    mlib_s16 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_d64 dthresh0);

void mlib_v_ImageCopyMask_U16_124_A8D1X4(
    mlib_u16 *src,
    mlib_u16 *mask,
    mlib_u16 *dst,
    mlib_s32 dsize,
    mlib_d64 dthresh);

void mlib_v_ImageCopyMask_U16_124_A8D2(
    mlib_u16 *src,
    mlib_s32 slb,
    mlib_u16 *mask,
    mlib_s32 mlb,
    mlib_u16 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_d64 dthresh);

void mlib_v_ImageCopyMask_U16_124(
    mlib_u16 *src,
    mlib_s32 slb,
    mlib_u16 *mask,
    mlib_s32 mlb,
    mlib_u16 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_d64 dthresh0);

void mlib_v_ImageCopyMask_U16_3_A8D1(
    mlib_u16 *src,
    mlib_u16 *mask,
    mlib_u16 *dst,
    mlib_s32 dsize,
    mlib_d64 dthresh0);

void mlib_v_ImageCopyMask_U16_3_A8D2(
    mlib_u16 *src,
    mlib_s32 slb,
    mlib_u16 *mask,
    mlib_s32 mlb,
    mlib_u16 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_d64 dthresh0);

void mlib_v_ImageCopyMask_U16_3(
    mlib_u16 *src,
    mlib_s32 slb,
    mlib_u16 *mask,
    mlib_s32 mlb,
    mlib_u16 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_d64 dthresh0);

void mlib_v_ImageCopyMask_S32_124_A8D1X4(
    mlib_s32 *src,
    mlib_s32 *mask,
    mlib_s32 *dst,
    mlib_s32 dsize,
    mlib_d64 dthresh0,
    mlib_d64 dthresh1);

void mlib_v_ImageCopyMask_S32_124_A8D2(
    mlib_s32 *src,
    mlib_s32 slb,
    mlib_s32 *mask,
    mlib_s32 mlb,
    mlib_s32 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_d64 dthresh0,
    mlib_d64 dthresh1);

void mlib_v_ImageCopyMask_S32_124(
    mlib_s32 *src,
    mlib_s32 slb,
    mlib_s32 *mask,
    mlib_s32 mlb,
    mlib_s32 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_d64 dthresh0,
    mlib_d64 dthresh1);

void mlib_v_ImageCopyMask_S32_3_A8D1(
    mlib_s32 *src,
    mlib_s32 *mask,
    mlib_s32 *dst,
    mlib_s32 dsize,
    mlib_d64 dthresh0,
    mlib_d64 dthresh1);

void mlib_v_ImageCopyMask_S32_3_A8D2(
    mlib_s32 *src,
    mlib_s32 slb,
    mlib_s32 *mask,
    mlib_s32 mlb,
    mlib_s32 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_d64 dthresh0,
    mlib_d64 dthresh1);

void mlib_v_ImageCopyMask_S32_3(
    mlib_s32 *src,
    mlib_s32 slb,
    mlib_s32 *mask,
    mlib_s32 mlb,
    mlib_s32 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_d64 dthresh0_0,
    mlib_d64 dthresh1_0);

#ifdef __cplusplus
}
#endif

#endif /* _MLIB_V_IMAGECOPYMASK_F_H */
