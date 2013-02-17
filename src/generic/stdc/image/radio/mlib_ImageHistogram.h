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

#ifndef _MLIB_IMAGEHISTOGRAM_H
#define	_MLIB_IMAGEHISTOGRAM_H

#pragma ident	"@(#)mlib_ImageHistogram.h	9.2	07/10/09 SMI"

#ifdef __cplusplus
extern "C" {
#endif

#include <mlib_types.h>
#include <mlib_status.h>

mlib_status mlib_ImageHistogram2_U8(
    mlib_s32 **histo,
    const mlib_u8 *src,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s32 nchan,
    mlib_s32 stride,
    const mlib_s32 *lowValue,
    const mlib_s32 *highValue,
    const mlib_d64 *one_per_binWidth,
    mlib_s32 xStart,
    mlib_s32 yStart,
    mlib_s32 xPeriod,
    mlib_s32 yPeriod);

mlib_status mlib_ImageHistogram2_S16(
    mlib_s32 **histo,
    const mlib_s16 *src,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s32 nchan,
    mlib_s32 stride,
    const mlib_s32 *lowValue,
    const mlib_s32 *highValue,
    const mlib_d64 *one_per_binWidth,
    mlib_s32 xStart,
    mlib_s32 yStart,
    mlib_s32 xPeriod,
    mlib_s32 yPeriod);

mlib_status mlib_ImageHistogram2_U16(
    mlib_s32 **histo,
    const mlib_u16 *src,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s32 nchan,
    mlib_s32 stride,
    const mlib_s32 *lowValue,
    const mlib_s32 *highValue,
    const mlib_d64 *one_per_binWidth,
    mlib_s32 xStart,
    mlib_s32 yStart,
    mlib_s32 xPeriod,
    mlib_s32 yPeriod);

mlib_status mlib_ImageHistogram2_S32(
    mlib_s32 **histo,
    const mlib_s32 *src,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s32 nchan,
    mlib_s32 stride,
    const mlib_s32 *lowValue,
    const mlib_s32 *highValue,
    const mlib_d64 *one_per_binWidth,
    mlib_s32 xStart,
    mlib_s32 yStart,
    mlib_s32 xPeriod,
    mlib_s32 yPeriod);

void mlib_ImageHistogram2_S16_4_D1(
    const mlib_s32 *sl,
    const mlib_s32 *pmask,
    mlib_s32 size,
    mlib_s32 *histo0,
    mlib_s32 *histo1,
    mlib_s32 *histo2,
    mlib_s32 *histo3);

#ifdef __cplusplus
}
#endif

#endif /* _MLIB_IMAGEHISTOGRAM_H */
