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

#ifndef _MLIB_S_IMAGECLEAR_F_H
#define	_MLIB_S_IMAGECLEAR_F_H

#pragma ident	"@(#)mlib_s_ImageClear_f.h	9.2	07/11/05 SMI"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __SUNPRO_C
#include <sunmedia_intrin.h>
#else
#include <emmintrin.h>
#endif

#include <mlib_types.h>
#include <mlib_image_types.h>

mlib_status mlib_s_ImageClear_BIT(
    mlib_image *img,
    const mlib_s32 *color);

void mlib_s_ImageClear_U8_1(
    mlib_image *img,
    const mlib_s32 *color);

void mlib_s_ImageClear_U8_2(
    mlib_image *img,
    const mlib_s32 *color);

void mlib_s_ImageClear_U8_3(
    mlib_image *img,
    const mlib_s32 *color);

void mlib_s_ImageClear_U8_4(
    mlib_image *img,
    const mlib_s32 *color);

void mlib_s_ImageClear_S16_1(
    mlib_image *img,
    const mlib_s32 *color);

void mlib_s_ImageClear_S16_2(
    mlib_image *img,
    const mlib_s32 *color);

void mlib_s_ImageClear_S16_3(
    mlib_image *img,
    const mlib_s32 *color);

void mlib_s_ImageClear_S16_4(
    mlib_image *img,
    const mlib_s32 *color);

void mlib_s_ImageClear_S32_1(
    mlib_image *img,
    const mlib_s32 *color);

void mlib_s_ImageClear_S32_2(
    mlib_image *img,
    const mlib_s32 *color);

void mlib_s_ImageClear_S32_3(
    mlib_image *img,
    const mlib_s32 *color);

void mlib_s_ImageClear_S32_4(
    mlib_image *img,
    const mlib_s32 *color);

#ifdef __cplusplus
}
#endif

#endif /* _MLIB_S_IMAGECLEAR_F_H */
