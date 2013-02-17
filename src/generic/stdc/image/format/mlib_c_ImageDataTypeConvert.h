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

#ifndef _MLIB_C_IMAGEDATATYPECONVERT_H
#define	_MLIB_C_IMAGEDATATYPECONVERT_H

#pragma ident	"@(#)mlib_c_ImageDataTypeConvert.h	9.2	07/10/09 SMI"

#ifdef __cplusplus
extern "C" {
#endif

#include <mlib_types.h>
#include <mlib_image_types.h>
#include <mlib_status.h>

void mlib_c_ImageDataTypeConvert_BIT_U8(
    const mlib_image *src,
    mlib_image *dst);

void mlib_c_ImageDataTypeConvert_BIT_S16(
    const mlib_image *src,
    mlib_image *dst);

/* BIT -> S32 */
#ifdef _NO_LONGLONG

void mlib_c_ImageDataTypeConvert_BIT_S32(
    const mlib_image *src,
    mlib_image *dst);

#else /* _NO_LONGLONG */

void mlib_c_ImageDataTypeConvert_BIT_S32_A8D1X8(
    const mlib_u8 *sp,
    mlib_s32 *dp,
    mlib_s32 dsize);

void mlib_c_ImageDataTypeConvert_BIT_S32_A8D2X8(
    const mlib_u8 *src,
    mlib_s32 strides,
    mlib_s32 *dst,
    mlib_s32 strided,
    mlib_s32 width,
    mlib_s32 height);

void mlib_c_ImageDataTypeConvert_BIT_S32_D1(
    const mlib_u8 *src,
    mlib_s32 *dst,
    mlib_s32 dsize);

void mlib_c_ImageDataTypeConvert_BIT_S32(
    const mlib_u8 *src,
    mlib_s32 slb,
    mlib_s32 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize);
#endif /* _NO_LONGLONG */

/* BIT -> F32 */
void mlib_c_ImageDataTypeConvert_BIT_F32_A8D1X8(
    const mlib_u8 *sp,
    mlib_f32 *dp,
    mlib_s32 dsize);

void mlib_c_ImageDataTypeConvert_BIT_F32_A8D2X8(
    const mlib_u8 *src,
    mlib_s32 strides,
    mlib_f32 *dst,
    mlib_s32 strided,
    mlib_s32 width,
    mlib_s32 height);

void mlib_c_ImageDataTypeConvert_BIT_F32_D1(
    const mlib_u8 *src,
    mlib_f32 *dst,
    mlib_s32 dsize);

void mlib_c_ImageDataTypeConvert_BIT_F32(
    const mlib_u8 *src,
    mlib_s32 slb,
    mlib_f32 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize);

void mlib_c_ImageDataTypeConvert_BIT_D64(
    const mlib_image *src,
    mlib_image *dst);

void mlib_c_ImageDataTypeConvert_U8_BIT(
    const mlib_image *src,
    mlib_image *dst);

void mlib_c_ImageDataTypeConvert_U8_S16(
    const mlib_image *src,
    mlib_image *dst);

void mlib_c_ImageDataTypeConvert_U8_S32(
    const mlib_image *src,
    mlib_image *dst);

void mlib_c_ImageDataTypeConvert_U8_F32(
    const mlib_image *src,
    mlib_image *dst);

void mlib_c_ImageDataTypeConvert_U8_D64(
    const mlib_image *src,
    mlib_image *dst);

void mlib_c_ImageDataTypeConvert_U16_BIT(
    const mlib_image *src,
    mlib_image *dst);

void mlib_c_ImageDataTypeConvert_U16_U8(
    const mlib_image *src,
    mlib_image *dst);

void mlib_c_ImageDataTypeConvert_U16_S16(
    const mlib_image *src,
    mlib_image *dst);

void mlib_c_ImageDataTypeConvert_U16_S32(
    const mlib_image *src,
    mlib_image *dst);

void mlib_c_ImageDataTypeConvert_U16_F32(
    const mlib_image *src,
    mlib_image *dst);

void mlib_c_ImageDataTypeConvert_U16_D64(
    const mlib_image *src,
    mlib_image *dst);

void mlib_c_ImageDataTypeConvert_S16_BIT(
    const mlib_image *src,
    mlib_image *dst);

void mlib_c_ImageDataTypeConvert_S16_U8(
    const mlib_image *src,
    mlib_image *dst);

void mlib_c_ImageDataTypeConvert_S16_U16(
    const mlib_image *src,
    mlib_image *dst);

void mlib_c_ImageDataTypeConvert_S16_S32(
    const mlib_image *src,
    mlib_image *dst);

void mlib_c_ImageDataTypeConvert_S16_F32(
    const mlib_image *src,
    mlib_image *dst);

void mlib_c_ImageDataTypeConvert_S16_D64(
    const mlib_image *src,
    mlib_image *dst);

void mlib_c_ImageDataTypeConvert_S32_BIT(
    const mlib_image *src,
    mlib_image *dst);

void mlib_c_ImageDataTypeConvert_S32_U8(
    const mlib_image *src,
    mlib_image *dst);

void mlib_c_ImageDataTypeConvert_S32_U16(
    const mlib_image *src,
    mlib_image *dst);

void mlib_c_ImageDataTypeConvert_S32_S16(
    const mlib_image *src,
    mlib_image *dst);

void mlib_c_ImageDataTypeConvert_S32_F32(
    const mlib_image *src,
    mlib_image *dst);

void mlib_c_ImageDataTypeConvert_S32_D64(
    const mlib_image *src,
    mlib_image *dst);

void mlib_c_ImageDataTypeConvert_F32_U8(
    const mlib_image *src,
    mlib_image *dst);

void mlib_c_ImageDataTypeConvert_F32_U16(
    const mlib_image *src,
    mlib_image *dst);

void mlib_c_ImageDataTypeConvert_F32_S16(
    const mlib_image *src,
    mlib_image *dst);

void mlib_c_ImageDataTypeConvert_F32_S32(
    const mlib_image *src,
    mlib_image *dst);

void mlib_c_ImageDataTypeConvert_F32_D64(
    const mlib_image *src,
    mlib_image *dst);

void mlib_c_ImageDataTypeConvert_D64_BIT(
    const mlib_image *src,
    mlib_image *dst);

void mlib_c_ImageDataTypeConvert_D64_U8(
    const mlib_image *src,
    mlib_image *dst);

void mlib_c_ImageDataTypeConvert_D64_U16(
    const mlib_image *src,
    mlib_image *dst);

void mlib_c_ImageDataTypeConvert_D64_S16(
    const mlib_image *src,
    mlib_image *dst);

void mlib_c_ImageDataTypeConvert_D64_S32(
    const mlib_image *src,
    mlib_image *dst);

void mlib_c_ImageDataTypeConvert_D64_F32(
    const mlib_image *src,
    mlib_image *dst);

#ifdef __cplusplus
}
#endif

#endif /* _MLIB_C_IMAGEDATATYPECONVERT_H */
