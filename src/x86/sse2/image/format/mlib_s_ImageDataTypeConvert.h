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
 * Copyright 2006 Sun Microsystems, Inc.  All rights reserved.
 * Use is subject to license terms.
 */

#ifndef _MLIB_S_IMAGEDATATYPECONVERT_H
#define	_MLIB_S_IMAGEDATATYPECONVERT_H

#pragma ident	"@(#)mlib_s_ImageDataTypeConvert.h	9.2	07/11/05 SMI"

#ifdef __cplusplus
extern "C" {
#endif

#include <mlib_types.h>

/* BIT -> U8 */
void mlib_s_ImageDataTypeConvert_BIT_U8(
    const mlib_image *src,
    mlib_image *dst);

/* BIT -> S16 */
void mlib_s_ImageDataTypeConvert_BIT_S16(
    const mlib_image *src,
    mlib_image *dst);

/* BIT -> S32 */
void mlib_s_ImageDataTypeConvert_BIT_S32(
    const mlib_image *src,
    mlib_image *dst);

/* BIT -> F32 */
void mlib_s_ImageDataTypeConvert_BIT_F32_A8D1X8(
    const mlib_u8 *sp,
    mlib_f32 *dp,
    mlib_s32 dsize);

void mlib_s_ImageDataTypeConvert_BIT_F32_A8D2X8(
    const mlib_u8 *src,
    mlib_s32 strides,
    mlib_f32 *dst,
    mlib_s32 strided,
    mlib_s32 width,
    mlib_s32 height);

void mlib_s_ImageDataTypeConvert_BIT_F32_D1(
    const mlib_u8 *src,
    mlib_f32 *dst,
    mlib_s32 dsize);

void mlib_s_ImageDataTypeConvert_BIT_F32(
    const mlib_u8 *src,
    mlib_s32 slb,
    mlib_f32 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize);

/* BIT -> D64 */
void mlib_s_ImageDataTypeConvert_BIT_D64(
    const mlib_image *src,
    mlib_image *dst);

/* U8 -> BIT */
void mlib_s_ImageDataTypeConvert_U8_BIT_A8D1X8(
    const mlib_u8 *src,
    mlib_u8 *dst,
    mlib_s32 dsize);

void mlib_s_ImageDataTypeConvert_U8_BIT_A8D2X8(
    const mlib_u8 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize);

void mlib_s_ImageDataTypeConvert_U8_BIT_D1(
    const mlib_u8 *src,
    mlib_u8 *dst,
    mlib_s32 dsize);

void mlib_s_ImageDataTypeConvert_U8_BIT(
    const mlib_u8 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize);

/* U8 -> S16 */
void mlib_s_ImageDataTypeConvert_U8_S16_A8D1X8(
    const mlib_u8 *src,
    mlib_s16 *dst,
    mlib_s32 dsize);

void mlib_s_ImageDataTypeConvert_U8_S16_A8D2X8(
    const mlib_u8 *src,
    mlib_s32 slb,
    mlib_s16 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize);

void mlib_s_ImageDataTypeConvert_U8_S16_D1(
    const mlib_u8 *src,
    mlib_s16 *dst,
    mlib_s32 dsize);

void mlib_s_ImageDataTypeConvert_U8_S16(
    const mlib_u8 *src,
    mlib_s32 slb,
    mlib_s16 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize);

/* U8 -> S32 */
void mlib_s_ImageDataTypeConvert_U8_S32_A8D1X8(
    const mlib_u8 *src,
    mlib_s32 *dst,
    mlib_s32 dsize);

void mlib_s_ImageDataTypeConvert_U8_S32_A8D2X8(
    const mlib_u8 *src,
    mlib_s32 slb,
    mlib_s32 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize);

void mlib_s_ImageDataTypeConvert_U8_S32_D1(
    const mlib_u8 *src,
    mlib_s32 *dst,
    mlib_s32 dsize);

void mlib_s_ImageDataTypeConvert_U8_S32(
    const mlib_u8 *src,
    mlib_s32 slb,
    mlib_s32 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize);

/* U8 -> F32 */
void mlib_s_ImageDataTypeConvert_U8_F32_A8D1X8(
    const mlib_u8 *src,
    mlib_f32 *dst,
    mlib_s32 dsize);

void mlib_s_ImageDataTypeConvert_U8_F32_A8D2X8(
    const mlib_u8 *src,
    mlib_s32 slb,
    mlib_f32 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize);

void mlib_s_ImageDataTypeConvert_U8_F32_D1(
    const mlib_u8 *src,
    mlib_f32 *dst,
    mlib_s32 dsize);

void mlib_s_ImageDataTypeConvert_U8_F32(
    const mlib_u8 *src,
    mlib_s32 slb,
    mlib_f32 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize);

/* U8 -> D64 */
void mlib_s_ImageDataTypeConvert_U8_D64_A8D1X8(
    const mlib_u8 *src,
    mlib_d64 *dst,
    mlib_s32 dsize);

void mlib_s_ImageDataTypeConvert_U8_D64_A8D2X8(
    const mlib_u8 *src,
    mlib_s32 slb,
    mlib_d64 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize);

void mlib_s_ImageDataTypeConvert_U8_D64_D1(
    const mlib_u8 *src,
    mlib_d64 *dst,
    mlib_s32 dsize);

void mlib_s_ImageDataTypeConvert_U8_D64(
    const mlib_u8 *src,
    mlib_s32 slb,
    mlib_d64 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize);

/* U16 -> BIT */
void mlib_s_ImageDataTypeConvert_U16_BIT_A8D1X8(
    const mlib_u8 *src,
    mlib_u8 *dst,
    mlib_s32 dsize);

void mlib_s_ImageDataTypeConvert_U16_BIT_A8D2X8(
    const mlib_u8 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize);

void mlib_s_ImageDataTypeConvert_U16_BIT_D1(
    const mlib_u8 *src,
    mlib_u8 *dst,
    mlib_s32 dsize);

void mlib_s_ImageDataTypeConvert_U16_BIT(
    const mlib_u8 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize);

/* U16 -> U8 */
void mlib_s_ImageDataTypeConvert_U16_U8_A8D1X8(
    const mlib_u16 *src,
    mlib_u8 *dst,
    mlib_s32 dsize);

void mlib_s_ImageDataTypeConvert_U16_U8_A8D2X8(
    const mlib_u16 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize);

void mlib_s_ImageDataTypeConvert_U16_U8_D1(
    const mlib_u16 *src,
    mlib_u8 *dst,
    mlib_s32 dsize);

void mlib_s_ImageDataTypeConvert_U16_U8(
    const mlib_u16 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize);

/* U16 -> S16 */
void mlib_s_ImageDataTypeConvert_U16_S16_A8D1X8(
    const mlib_u16 *src,
    mlib_s16 *dst,
    mlib_s32 dsize);

void mlib_s_ImageDataTypeConvert_U16_S16_A8D2X8(
    const mlib_u16 *src,
    mlib_s32 slb,
    mlib_s16 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize);

void mlib_s_ImageDataTypeConvert_U16_S16_D1(
    const mlib_u16 *src,
    mlib_s16 *dst,
    mlib_s32 dsize);

void mlib_s_ImageDataTypeConvert_U16_S16(
    const mlib_u16 *src,
    mlib_s32 slb,
    mlib_s16 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize);

/* U16 -> S32 */
void mlib_s_ImageDataTypeConvert_U16_S32_A8D1X8(
    const mlib_u16 *src,
    mlib_s32 *dst,
    mlib_s32 dsize);

void mlib_s_ImageDataTypeConvert_U16_S32_A8D2X8(
    const mlib_u16 *src,
    mlib_s32 slb,
    mlib_s32 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize);

void mlib_s_ImageDataTypeConvert_U16_S32_D1(
    const mlib_u16 *src,
    mlib_s32 *dst,
    mlib_s32 dsize);

void mlib_s_ImageDataTypeConvert_U16_S32(
    const mlib_u16 *src,
    mlib_s32 slb,
    mlib_s32 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize);

/* U16 -> F32 */
void mlib_s_ImageDataTypeConvert_U16_F32_A8D1X8(
    const mlib_u16 *src,
    mlib_f32 *dst,
    mlib_s32 dsize);

void mlib_s_ImageDataTypeConvert_U16_F32_A8D2X8(
    const mlib_u16 *src,
    mlib_s32 slb,
    mlib_f32 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize);

void mlib_s_ImageDataTypeConvert_U16_F32_D1(
    const mlib_u16 *src,
    mlib_f32 *dst,
    mlib_s32 dsize);

void mlib_s_ImageDataTypeConvert_U16_F32(
    const mlib_u16 *src,
    mlib_s32 slb,
    mlib_f32 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize);

/* U16 -> D64 */
void mlib_s_ImageDataTypeConvert_U16_D64_A8D1X8(
    const mlib_u16 *src,
    mlib_d64 *dst,
    mlib_s32 dsize);

void mlib_s_ImageDataTypeConvert_U16_D64_A8D2X8(
    const mlib_u16 *src,
    mlib_s32 slb,
    mlib_d64 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize);

void mlib_s_ImageDataTypeConvert_U16_D64_D1(
    const mlib_u16 *src,
    mlib_d64 *dst,
    mlib_s32 dsize);

void mlib_s_ImageDataTypeConvert_U16_D64(
    const mlib_u16 *src,
    mlib_s32 slb,
    mlib_d64 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize);

/* S16 -> BIT */
void mlib_s_ImageDataTypeConvert_S16_BIT_A8D1X8(
    const mlib_u8 *src,
    mlib_u8 *dst,
    mlib_s32 dsize);

void mlib_s_ImageDataTypeConvert_S16_BIT_A8D2X8(
    const mlib_u8 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize);

void mlib_s_ImageDataTypeConvert_S16_BIT_D1(
    const mlib_u8 *src,
    mlib_u8 *dst,
    mlib_s32 dsize);

void mlib_s_ImageDataTypeConvert_S16_BIT(
    const mlib_u8 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize);

/* S16 -> U8 */
void mlib_s_ImageDataTypeConvert_S16_U8_A8D1X8(
    const mlib_s16 *src,
    mlib_u8 *dst,
    mlib_s32 dsize);

void mlib_s_ImageDataTypeConvert_S16_U8_A8D2X8(
    const mlib_s16 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize);

void mlib_s_ImageDataTypeConvert_S16_U8_D1(
    const mlib_s16 *src,
    mlib_u8 *dst,
    mlib_s32 dsize);

void mlib_s_ImageDataTypeConvert_S16_U8(
    const mlib_s16 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize);

/* S16 -> U16 */

void mlib_s_ImageDataTypeConvert_S16_U16_A8D1X8(
    const mlib_s16 *src,
    mlib_u16 *dst,
    mlib_s32 dsize);

void mlib_s_ImageDataTypeConvert_S16_U16_A8D2X8(
    const mlib_s16 *src,
    mlib_s32 slb,
    mlib_u16 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize);

void mlib_s_ImageDataTypeConvert_S16_U16_D1(
    const mlib_s16 *src,
    mlib_u16 *dst,
    mlib_s32 dsize);

void mlib_s_ImageDataTypeConvert_S16_U16(
    const mlib_s16 *src,
    mlib_s32 slb,
    mlib_u16 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize);

/* S16 -> S32 */
void mlib_s_ImageDataTypeConvert_S16_S32_A8D1X8(
    const mlib_s16 *src,
    mlib_s32 *dst,
    mlib_s32 dsize);

void mlib_s_ImageDataTypeConvert_S16_S32_A8D2X8(
    const mlib_s16 *src,
    mlib_s32 slb,
    mlib_s32 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize);

void mlib_s_ImageDataTypeConvert_S16_S32_D1(
    const mlib_s16 *src,
    mlib_s32 *dst,
    mlib_s32 dsize);

void mlib_s_ImageDataTypeConvert_S16_S32(
    const mlib_s16 *src,
    mlib_s32 slb,
    mlib_s32 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize);

/* S16 -> F32 */
void mlib_s_ImageDataTypeConvert_S16_F32_A8D1X8(
    const mlib_s16 *src,
    mlib_f32 *dst,
    mlib_s32 dsize);

void mlib_s_ImageDataTypeConvert_S16_F32_A8D2X8(
    const mlib_s16 *src,
    mlib_s32 slb,
    mlib_f32 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize);

void mlib_s_ImageDataTypeConvert_S16_F32_D1(
    const mlib_s16 *src,
    mlib_f32 *dst,
    mlib_s32 dsize);

void mlib_s_ImageDataTypeConvert_S16_F32(
    const mlib_s16 *src,
    mlib_s32 slb,
    mlib_f32 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize);

/* S16 -> D64 */
void mlib_s_ImageDataTypeConvert_S16_D64_A8D1X8(
    const mlib_s16 *src,
    mlib_d64 *dst,
    mlib_s32 dsize);

void mlib_s_ImageDataTypeConvert_S16_D64_A8D2X8(
    const mlib_s16 *src,
    mlib_s32 slb,
    mlib_d64 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize);

void mlib_s_ImageDataTypeConvert_S16_D64_D1(
    const mlib_s16 *src,
    mlib_d64 *dst,
    mlib_s32 dsize);

void mlib_s_ImageDataTypeConvert_S16_D64(
    const mlib_s16 *src,
    mlib_s32 slb,
    mlib_d64 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize);

/* S32 -> BIT */
void mlib_s_ImageDataTypeConvert_S32_BIT_A8D1X8(
    const mlib_u8 *src,
    mlib_u8 *dst,
    mlib_s32 dsize);

void mlib_s_ImageDataTypeConvert_S32_BIT_A8D2X8(
    const mlib_u8 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize);

void mlib_s_ImageDataTypeConvert_S32_BIT_D1(
    const mlib_u8 *src,
    mlib_u8 *dst,
    mlib_s32 dsize);

void mlib_s_ImageDataTypeConvert_S32_BIT(
    const mlib_u8 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize);

/* S32 -> U8 */
void mlib_s_ImageDataTypeConvert_S32_U8_A8D1X8(
    const mlib_s32 *src,
    mlib_u8 *dst,
    mlib_s32 dsize);

void mlib_s_ImageDataTypeConvert_S32_U8_A8D2X8(
    const mlib_s32 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize);

void mlib_s_ImageDataTypeConvert_S32_U8_D1(
    const mlib_s32 *src,
    mlib_u8 *dst,
    mlib_s32 dsize);

void mlib_s_ImageDataTypeConvert_S32_U81(
    const mlib_s32 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize);

void mlib_s_ImageDataTypeConvert_S32_U8(
    const mlib_image *src,
    mlib_image *dst);

/* S32 -> U16 */
void mlib_s_ImageDataTypeConvert_S32_U16_A8D1X8(
    const mlib_s32 *src,
    mlib_u16 *dst,
    mlib_s32 dsize);

void mlib_s_ImageDataTypeConvert_S32_U16_A8D2X8(
    const mlib_s32 *src,
    mlib_s32 slb,
    mlib_u16 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize);

void mlib_s_ImageDataTypeConvert_S32_U16_D1(
    const mlib_s32 *src,
    mlib_u16 *dst,
    mlib_s32 dsize);

void mlib_s_ImageDataTypeConvert_S32_U16(
    const mlib_s32 *src,
    mlib_s32 slb,
    mlib_u16 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize);

/* S32 -> S16 */
void mlib_s_ImageDataTypeConvert_S32_S16_A8D1X8(
    const mlib_s32 *src,
    mlib_s16 *dst,
    mlib_s32 dsize);

void mlib_s_ImageDataTypeConvert_S32_S16_A8D2X8(
    const mlib_s32 *src,
    mlib_s32 slb,
    mlib_s16 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize);

void mlib_s_ImageDataTypeConvert_S32_S16_D1(
    const mlib_s32 *src,
    mlib_s16 *dst,
    mlib_s32 dsize);

void mlib_s_ImageDataTypeConvert_S32_S16(
    const mlib_s32 *src,
    mlib_s32 slb,
    mlib_s16 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize);

/* S32 -> F32 */
void mlib_s_ImageDataTypeConvert_S32_F32_A8D1X8(
    const mlib_s32 *src,
    mlib_f32 *dst,
    mlib_s32 dsize);

void mlib_s_ImageDataTypeConvert_S32_F32_A8D2X8(
    const mlib_s32 *src,
    mlib_s32 slb,
    mlib_f32 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize);

void mlib_s_ImageDataTypeConvert_S32_F32_D1(
    const mlib_s32 *src,
    mlib_f32 *dst,
    mlib_s32 dsize);

void mlib_s_ImageDataTypeConvert_S32_F32(
    const mlib_s32 *src,
    mlib_s32 slb,
    mlib_f32 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize);

/* S32 -> D64 */
void mlib_s_ImageDataTypeConvert_S32_D64_A8D1X8(
    const mlib_s32 *src,
    mlib_d64 *dst,
    mlib_s32 dsize);

void mlib_s_ImageDataTypeConvert_S32_D64_A8D2X8(
    const mlib_s32 *src,
    mlib_s32 slb,
    mlib_d64 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize);

void mlib_s_ImageDataTypeConvert_S32_D64_D1(
    const mlib_s32 *src,
    mlib_d64 *dst,
    mlib_s32 dsize);

void mlib_s_ImageDataTypeConvert_S32_D64(
    const mlib_s32 *src,
    mlib_s32 slb,
    mlib_d64 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize);

/* F32 -> U8 */
void mlib_s_ImageDataTypeConvert_F32_U8(
    const mlib_image *src,
    mlib_image *dst);

/* F32 -> U16 */
void mlib_s_ImageDataTypeConvert_F32_U16(
    const mlib_image *src,
    mlib_image *dst);

/* F32 -> S16 */
void mlib_s_ImageDataTypeConvert_F32_S16(
    const mlib_image *src,
    mlib_image *dst);

/* F32 -> S32 */
void mlib_s_ImageDataTypeConvert_F32_S32(
    const mlib_image *src,
    mlib_image *dst);

/* F32 -> D64 */
void mlib_s_ImageDataTypeConvert_F32_D64_A8D1X8(
    const mlib_f32 *src,
    mlib_d64 *dst,
    mlib_s32 dsize);

void mlib_s_ImageDataTypeConvert_F32_D64_A8D2X8(
    const mlib_f32 *src,
    mlib_s32 slb,
    mlib_d64 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize);

void mlib_s_ImageDataTypeConvert_F32_D64_D1(
    const mlib_f32 *src,
    mlib_d64 *dst,
    mlib_s32 dsize);

void mlib_s_ImageDataTypeConvert_F32_D64(
    const mlib_f32 *src,
    mlib_s32 slb,
    mlib_d64 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize);

/* D64 -> BIT */
void mlib_s_ImageDataTypeConvert_D64_BIT(
    const mlib_image *src,
    mlib_image *dst);

/* D64 -> U8 */
void mlib_s_ImageDataTypeConvert_D64_U8(
    const mlib_image *src,
    mlib_image *dst);

/* D64 -> U16 */
void mlib_s_ImageDataTypeConvert_D64_U16(
    const mlib_image *src,
    mlib_image *dst);

/* D64 -> S16 */
void mlib_s_ImageDataTypeConvert_D64_S16(
    const mlib_image *src,
    mlib_image *dst);

/* D64 -> S32 */
void mlib_s_ImageDataTypeConvert_D64_S32(
    const mlib_image *src,
    mlib_image *dst);

/* D64 -> F32 */

void mlib_s_ImageDataTypeConvert_D64_F32_D1(
    const mlib_d64 *src,
    mlib_f32 *dst,
    mlib_s32 dsize);

void mlib_s_ImageDataTypeConvert_D64_F32(
    const mlib_d64 *src,
    mlib_s32 slb,
    mlib_f32 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize);

#ifdef __cplusplus
}
#endif

#endif /* _MLIB_S_IMAGEDATATYPECONVERT_H */
/* *********************************************************** */
