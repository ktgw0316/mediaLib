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

#ifndef _MLIB_S_IMAGEMOMENT2_H
#define	_MLIB_S_IMAGEMOMENT2_H

#pragma ident	"@(#)mlib_s_ImageMoment2.h	9.3	07/11/05 SMI"

#ifdef __cplusplus
extern "C" {
#endif

#include <mlib_types.h>
#include <mlib_image_types.h>
#include <mlib_status.h>

void mlib_s_ImageMoment2_U8_1(
    const mlib_image *img,
    mlib_d64 *rez);

void mlib_s_ImageMoment2_U8_2(
    const mlib_image *img,
    mlib_d64 *rez);

void mlib_s_ImageMoment2_U8_4(
    const mlib_image *img,
    mlib_d64 *rez);

void mlib_s_ImageMoment2_U8_3(
    const mlib_image *img,
    mlib_d64 *rez);

void mlib_s_ImageMoment2_U16_1(
    const mlib_image *img,
    mlib_d64 *rez);

void mlib_s_ImageMoment2_U16_2(
    const mlib_image *img,
    mlib_d64 *rez);

void mlib_s_ImageMoment2_U16_3(
    const mlib_image *img,
    mlib_d64 *rez);

void mlib_s_ImageMoment2_U16_4(
    const mlib_image *img,
    mlib_d64 *rez);

void mlib_s_ImageMoment2_S16_4(
    const mlib_image *img,
    mlib_d64 *rez);

void mlib_s_ImageMoment2_S16_1(
    const mlib_image *img,
    mlib_d64 *rez);

void mlib_s_ImageMoment2_S16_2(
    const mlib_image *img,
    mlib_d64 *rez);

void mlib_s_ImageMoment2_S16_3(
    const mlib_image *img,
    mlib_d64 *rez);

void mlib_s_ImageMoment2_S32_1(
    const mlib_image *img,
    mlib_d64 *rez);

void mlib_s_ImageMoment2_S32_2(
    const mlib_image *img,
    mlib_d64 *rez);

void mlib_s_ImageMoment2_S32_4(
    const mlib_image *img,
    mlib_d64 *rez);

void mlib_s_ImageMoment2_S32_3(
    const mlib_image *img,
    mlib_d64 *rez);

void mlib_s_ImageMoment2_F32_1(
    const mlib_image *img,
    mlib_d64 *res);

void mlib_s_ImageMoment2_F32_2(
    const mlib_image *img,
    mlib_d64 *res);

void mlib_s_ImageMoment2_F32_3(
    const mlib_image *img,
    mlib_d64 *res);

void mlib_s_ImageMoment2_F32_4(
    const mlib_image *img,
    mlib_d64 *res);

void mlib_s_ImageMoment2_D64_1(
    const mlib_image *img,
    mlib_d64 *res);

void mlib_s_ImageMoment2_D64_2(
    const mlib_image *img,
    mlib_d64 *res);

void mlib_s_ImageMoment2_D64_3(
    const mlib_image *img,
    mlib_d64 *res);

void mlib_s_ImageMoment2_D64_4(
    const mlib_image *img,
    mlib_d64 *res);

#ifdef __cplusplus
}
#endif

#endif /* _MLIB_S_IMAGEMOMENT2_H */
