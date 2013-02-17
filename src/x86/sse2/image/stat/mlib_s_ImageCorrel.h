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

#ifndef _MLIB_S_IMAGECORREL_H
#define	_MLIB_S_IMAGECORREL_H

#pragma ident	"@(#)mlib_s_ImageCorrel.h	9.4	07/11/05 SMI"

#ifdef __cplusplus
extern "C" {
#endif

#include <mlib_types.h>
#include <mlib_image_types.h>
#include <mlib_status.h>

void mlib_s_ImageCrossCorrel_U8_3(
    const mlib_image *img1,
    const mlib_image *img2,
    mlib_d64 *res);

void mlib_s_ImageCrossCorrel_U8_1(
    const mlib_image *img1,
    const mlib_image *img2,
    mlib_d64 *res);

void mlib_s_ImageCrossCorrel_U8_2(
    const mlib_image *img1,
    const mlib_image *img2,
    mlib_d64 *res);

void mlib_s_ImageCrossCorrel_U8_4(
    const mlib_image *img1,
    const mlib_image *img2,
    mlib_d64 *res);

void mlib_s_ImageCrossCorrel_U16_1(
    const mlib_image *img1,
    const mlib_image *img2,
    mlib_d64 *res);

void mlib_s_ImageCrossCorrel_U16_2(
    const mlib_image *img1,
    const mlib_image *img2,
    mlib_d64 *res);

void mlib_s_ImageCrossCorrel_U16_3(
    const mlib_image *img1,
    const mlib_image *img2,
    mlib_d64 *res);

void mlib_s_ImageCrossCorrel_U16_4(
    const mlib_image *img1,
    const mlib_image *img2,
    mlib_d64 *res);

void mlib_s_ImageCrossCorrel_S16_4(
    const mlib_image *img1,
    const mlib_image *img2,
    mlib_d64 *res);

void mlib_s_ImageCrossCorrel_S16_1(
    const mlib_image *img1,
    const mlib_image *img2,
    mlib_d64 *res);

void mlib_s_ImageCrossCorrel_S16_2(
    const mlib_image *img1,
    const mlib_image *img2,
    mlib_d64 *res);

void mlib_s_ImageCrossCorrel_S16_3(
    const mlib_image *img1,
    const mlib_image *img2,
    mlib_d64 *res);

void mlib_s_ImageCrossCorrel_S32_1(
    const mlib_image *img1,
    const mlib_image *img2,
    mlib_d64 *res);

void mlib_s_ImageCrossCorrel_S32_2(
    const mlib_image *img1,
    const mlib_image *img2,
    mlib_d64 *res);

void mlib_s_ImageCrossCorrel_S32_4(
    const mlib_image *img1,
    const mlib_image *img2,
    mlib_d64 *res);

void mlib_s_ImageCrossCorrel_S32_3(
    const mlib_image *img1,
    const mlib_image *img2,
    mlib_d64 *res);

void mlib_ImageCrossCorrel_D64_124(
    const mlib_image *img1,
    const mlib_image *img2,
    mlib_d64 *rez);

void mlib_ImageCrossCorrel_D64_1(
    const mlib_image *img1,
    const mlib_image *img2,
    mlib_d64 *rez);

void mlib_ImageCrossCorrel_D64_2(
    const mlib_image *img1,
    const mlib_image *img2,
    mlib_d64 *rez);

void mlib_ImageCrossCorrel_D64_3(
    const mlib_image *img1,
    const mlib_image *img2,
    mlib_d64 *rez);

void mlib_ImageCrossCorrel_D64_4(
    const mlib_image *img1,
    const mlib_image *img2,
    mlib_d64 *rez);

void mlib_ImageCrossCorrel_F32_1(
    const mlib_image *img1,
    const mlib_image *img2,
    mlib_d64 *rez);

void mlib_ImageCrossCorrel_F32_2(
    const mlib_image *img1,
    const mlib_image *img2,
    mlib_d64 *rez);

void mlib_ImageCrossCorrel_F32_3(
    const mlib_image *img1,
    const mlib_image *img2,
    mlib_d64 *rez);

void mlib_ImageCrossCorrel_F32_4(
    const mlib_image *img1,
    const mlib_image *img2,
    mlib_d64 *rez);

void mlib_s_ImageNormCrossCorrel(
    const mlib_image *img1,
    const mlib_image *img2,
    const mlib_d64 *m2,
    const mlib_d64 *s2,
    mlib_d64 *res);

void mlib_s_ImageNormCrossCorrel_D64_124(
    const mlib_image *img1,
    const mlib_image *img2,
    mlib_d64 *rez,
    const mlib_d64 *m2,
    const mlib_d64 *s2);

void mlib_s_ImageNormCrossCorrel_D64_3(
    const mlib_image *img1,
    const mlib_image *img2,
    mlib_d64 *rez,
    const mlib_d64 *m2,
    const mlib_d64 *s2);

void mlib_s_ImageNormCrossCorrel_F32_124(
    const mlib_image *img1,
    const mlib_image *img2,
    mlib_d64 *rez,
    const mlib_d64 *m2,
    const mlib_d64 *s2);

void mlib_s_ImageNormCrossCorrel_F32_3(
    const mlib_image *img1,
    const mlib_image *img2,
    mlib_d64 *rez,
    const mlib_d64 *m2,
    const mlib_d64 *s2);

#ifdef __cplusplus
}
#endif

#endif /* _MLIB_S_IMAGECORREL_H */
