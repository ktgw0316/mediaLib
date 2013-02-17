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

#ifndef _MLIB_V_IMAGECOLORRGB2MONOFUNC_H
#define	_MLIB_V_IMAGECOLORRGB2MONOFUNC_H

#pragma ident	"@(#)mlib_v_ImageColorRGB2MonoFunc.h	9.2	07/11/05 SMI"

#ifdef __cplusplus
extern "C" {
#endif

#include <mlib_types.h>

void mlib_v_ImageColorRGB2Mono_U8_A8D1X8(
    const mlib_u8 *src,
    mlib_u8 *dst,
    mlib_s32 dsize,
    const mlib_d64 *weight);

void mlib_v_ImageColorRGB2Mono_U8_A8D2X8(
    const mlib_u8 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_d64 *weight);

void mlib_v_ImageColorRGB2Mono_U8_D1(
    const mlib_u8 *src,
    mlib_u8 *dst,
    mlib_s32 dsize,
    const mlib_d64 *weight);

void mlib_v_ImageColorRGB2Mono_U8(
    const mlib_u8 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_d64 *weight);

mlib_status mlib_v_ImageColorRGB2Mono_S32(
    const mlib_s32 *src,
    mlib_s32 slb,
    mlib_s32 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_d64 *weight);

void mlib_v_ImageColorRGB2Mono_S16_A8D1X4(
    const mlib_s16 *src,
    mlib_s16 *dst,
    mlib_s32 dsize,
    const mlib_d64 *weight);

void mlib_v_ImageColorRGB2Mono_S16_A8D2X4(
    const mlib_s16 *src,
    mlib_s32 slb,
    mlib_s16 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_d64 *weight);

void mlib_v_ImageColorRGB2Mono_S16_D1(
    const mlib_s16 *src,
    mlib_s16 *dst,
    mlib_s32 dsize,
    const mlib_d64 *weight);

void mlib_v_ImageColorRGB2Mono_S16(
    const mlib_s16 *src,
    mlib_s32 slb,
    mlib_s16 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_d64 *weight);

void mlib_v_ImageColorRGB2Mono_GEN_S16_A8D1X4(
    const mlib_s16 *src,
    mlib_s16 *dst,
    mlib_s32 dsize,
    const mlib_d64 *weight);

void mlib_v_ImageColorRGB2Mono_GEN_S16_A8D2X4(
    const mlib_s16 *src,
    mlib_s32 slb,
    mlib_s16 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_d64 *weight);

void mlib_v_ImageColorRGB2Mono_GEN_S16_D1(
    const mlib_s16 *src,
    mlib_s16 *dst,
    mlib_s32 dsize,
    const mlib_d64 *weight);

void mlib_v_ImageColorRGB2Mono_GEN_S16(
    const mlib_s16 *src,
    mlib_s32 slb,
    mlib_s16 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_d64 *weight);

void mlib_v_ImageColorRGB2Mono_U16_A8D1X4(
    const mlib_u16 *src,
    mlib_u16 *dst,
    mlib_s32 dsize,
    const mlib_d64 *weight);

void mlib_v_ImageColorRGB2Mono_U16_A8D2X4(
    const mlib_u16 *src,
    mlib_s32 slb,
    mlib_u16 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_d64 *weight);

void mlib_v_ImageColorRGB2Mono_U16_D1(
    const mlib_u16 *src,
    mlib_u16 *dst,
    mlib_s32 dsize,
    const mlib_d64 *weight);

void mlib_v_ImageColorRGB2Mono_U16(
    const mlib_u16 *src,
    mlib_s32 slb,
    mlib_u16 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_d64 *weight);

void mlib_v_ImageColorRGB2Mono_GEN_U16_A8D1X4(
    const mlib_u16 *src,
    mlib_u16 *dst,
    mlib_s32 dsize,
    const mlib_d64 *weight);

void mlib_v_ImageColorRGB2Mono_GEN_U16_A8D2X4(
    const mlib_u16 *src,
    mlib_s32 slb,
    mlib_u16 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_d64 *weight);

void mlib_v_ImageColorRGB2Mono_GEN_U16_D1(
    const mlib_u16 *src,
    mlib_u16 *dst,
    mlib_s32 dsize,
    const mlib_d64 *weight);

void mlib_v_ImageColorRGB2Mono_GEN_U16(
    const mlib_u16 *src,
    mlib_s32 slb,
    mlib_u16 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_d64 *weight);

#ifdef __cplusplus
}
#endif

#endif /* _MLIB_V_IMAGECOLORRGB2MONOFUNC_H */
