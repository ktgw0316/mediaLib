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
 * Copyright 2007 Sun Microsystems, Inc.  All rights reserved.
 * Use is subject to license terms.
 */

#ifndef _MLIB_S_IMAGEBLENDCOLOR_H
#define	_MLIB_S_IMAGEBLENDCOLOR_H

#pragma ident	"@(#)mlib_s_ImageBlendColor.h	9.4	07/11/05 SMI"

#ifdef __cplusplus
extern "C" {
#endif

#include <mlib_types.h>

void mlib_s_ImageBlendColor_U8_4(
    mlib_u8 *sl,
    mlib_u8 *dl,
    mlib_s32 sstride,
    mlib_s32 dstride,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s32 channel,
    mlib_s32 alpha,
    const mlib_s32 *color);

void mlib_s_ImageBlendColor_U8_3(
    mlib_u8 *sl,
    mlib_u8 *dl,
    mlib_s32 sstride,
    mlib_s32 dstride,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s32 channel,
    mlib_s32 alpha,
    const mlib_s32 *color);

void mlib_s_ImageBlendColor_U8_2(
    mlib_u8 *sl,
    mlib_u8 *dl,
    mlib_s32 sstride,
    mlib_s32 dstride,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s32 channel,
    mlib_s32 alpha,
    const mlib_s32 *color);

void mlib_s_ImageBlendColor_S16_4(
    mlib_s16 *sl,
    mlib_s16 *dl,
    mlib_s32 sstride,
    mlib_s32 dstride,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s32 channel,
    mlib_s32 alpha,
    const mlib_s32 *color);

void mlib_s_ImageBlendColor_S16_3(
    mlib_s16 *sl,
    mlib_s16 *dl,
    mlib_s32 sstride,
    mlib_s32 dstride,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s32 channel,
    mlib_s32 alpha,
    const mlib_s32 *color);

void mlib_s_ImageBlendColor_S16_2(
    mlib_s16 *sl,
    mlib_s16 *dl,
    mlib_s32 sstride,
    mlib_s32 dstride,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s32 channel,
    mlib_s32 alpha,
    const mlib_s32 *color);

void mlib_s_ImageBlendColor_U16_4(
    mlib_u16 *sl,
    mlib_u16 *dl,
    mlib_s32 sstride,
    mlib_s32 dstride,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s32 channel,
    mlib_s32 alpha,
    const mlib_s32 *color);

void mlib_s_ImageBlendColor_U16_3(
    mlib_u16 *sl,
    mlib_u16 *dl,
    mlib_s32 sstride,
    mlib_s32 dstride,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s32 channel,
    mlib_s32 alpha,
    const mlib_s32 *color);

void mlib_s_ImageBlendColor_U16_2(
    mlib_u16 *sl,
    mlib_u16 *dl,
    mlib_s32 sstride,
    mlib_s32 dstride,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s32 channel,
    mlib_s32 alpha,
    const mlib_s32 *color);

void mlib_s_ImageBlendColor_S32_4(
    mlib_s32 *sl,
    mlib_s32 *dl,
    mlib_s32 sstride,
    mlib_s32 dstride,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s32 channel,
    mlib_s32 alpha,
    const mlib_s32 *color);

void mlib_s_ImageBlendColor_S32_3(
    mlib_s32 *sl,
    mlib_s32 *dl,
    mlib_s32 sstride,
    mlib_s32 dstride,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s32 channel,
    mlib_s32 alpha,
    const mlib_s32 *color);

void mlib_s_ImageBlendColor_S32_2(
    mlib_s32 *sl,
    mlib_s32 *dl,
    mlib_s32 sstride,
    mlib_s32 dstride,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s32 channel,
    mlib_s32 alpha,
    const mlib_s32 *color);

#ifdef __cplusplus
}
#endif

#endif /* _MLIB_S_IMAGEBLENDCOLOR_H */
