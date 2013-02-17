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

#ifndef _MLIB_IMAGEFLIPMAINDIAG_H
#define	_MLIB_IMAGEFLIPMAINDIAG_H

#pragma ident	"@(#)mlib_ImageFlipMainDiag.h	9.2	07/10/09 SMI"

#ifdef __cplusplus
extern "C" {
#endif

#include <mlib_types.h>
#include <mlib_image_types.h>
#include <mlib_status.h>
#include <mlib_ImageDivTables.h>
#include <mlib_ImageCopy.h>

void mlib_ImageFlipMainDiag_U8_1(
    mlib_image *dst,
    const mlib_image *src,
    mlib_s32 x_dir,
    mlib_s32 y_dir);

void mlib_ImageFlipMainDiag_S16_1(
    mlib_image *dst,
    const mlib_image *src,
    mlib_s32 x_dir,
    mlib_s32 y_dir);

void mlib_ImageFlipMainDiag_U8_2(
    mlib_image *dst,
    const mlib_image *src,
    mlib_s32 x_dir,
    mlib_s32 y_dir);

void mlib_ImageFlipMainDiag_U8_3_na(
    mlib_image *dst,
    const mlib_image *src,
    mlib_s32 x_dir,
    mlib_s32 y_dir);

void mlib_ImageFlipMainDiag_U8_4(
    mlib_image *dst,
    const mlib_image *src,
    mlib_s32 x_dir,
    mlib_s32 y_dir);

void mlib_ImageFlipMainDiag_S16_2(
    mlib_image *dst,
    const mlib_image *src,
    mlib_s32 x_dir,
    mlib_s32 y_dir);

void mlib_ImageFlipMainDiag_S16_3_na(
    mlib_image *dst,
    const mlib_image *src,
    mlib_s32 x_dir,
    mlib_s32 y_dir);

void mlib_ImageFlipMainDiag_S16_4(
    mlib_image *dst,
    const mlib_image *src,
    mlib_s32 x_dir,
    mlib_s32 y_dir);

void mlib_ImageFlipMainDiag_S32_1(
    mlib_image *dst,
    const mlib_image *src,
    mlib_s32 x_dir,
    mlib_s32 y_dir);

void mlib_ImageFlipMainDiag_S32_2(
    mlib_image *dst,
    const mlib_image *src,
    mlib_s32 x_dir,
    mlib_s32 y_dir);

void mlib_ImageFlipMainDiag_S32_3(
    mlib_image *dst,
    const mlib_image *src,
    mlib_s32 x_dir,
    mlib_s32 y_dir);

void mlib_ImageFlipMainDiag_S32_4(
    mlib_image *dst,
    const mlib_image *src,
    mlib_s32 x_dir,
    mlib_s32 y_dir);

void mlib_ImageFlipMainDiag_D64_1(
    mlib_image *dst,
    const mlib_image *src,
    mlib_s32 x_dir,
    mlib_s32 y_dir);

void mlib_ImageFlipMainDiag_D64_2(
    mlib_image *dst,
    const mlib_image *src,
    mlib_s32 x_dir,
    mlib_s32 y_dir);

void mlib_ImageFlipMainDiag_D64_3(
    mlib_image *dst,
    const mlib_image *src,
    mlib_s32 x_dir,
    mlib_s32 y_dir);

void mlib_ImageFlipMainDiag_D64_4(
    mlib_image *dst,
    const mlib_image *src,
    mlib_s32 x_dir,
    mlib_s32 y_dir);

/*
 * void mlib_ImageFlipMainDiag_U8_3(mlib_image *dst, const mlib_image *src,
 *					mlib_s32 x_dir, mlib_s32 y_dir);
 * void mlib_ImageFlipMainDiag_S16_3(mlib_image *dst, const mlib_image *src,
 *					mlib_s32 x_dir, mlib_s32 y_dir);
 */

mlib_status mlib_ImageFlipMainDiag_BIT_1(
    mlib_image *dst,
    const mlib_image *src,
    mlib_s32 x_dir,
    mlib_s32 y_dir);

mlib_status mlib_ImageFlipRotate(
    mlib_image *dst,
    const mlib_image *src,
    mlib_s32 x_dir,
    mlib_s32 y_dir);

#ifdef __cplusplus
}
#endif

#endif /* _MLIB_IMAGEFLIPMAINDIAG_H */
