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

#ifndef _MLIB_V_IMAGEHISTOGRAM2_H
#define	_MLIB_V_IMAGEHISTOGRAM2_H

#pragma ident	"@(#)mlib_v_ImageHistogram2.h	9.2	07/11/05 SMI"

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

mlib_status mlib_v_ImageHistogram2_U16_124(
    const void *src,
    mlib_s32 nchan,
    mlib_s32 size_row,
    mlib_s32 height,
    mlib_s32 stride,
    mlib_s32 xStart,
    mlib_s32 yStart,
    mlib_s32 yPeriod,
    mlib_s32 **histogr,
    const mlib_d64 *arr_min,
    const mlib_d64 *arr_max,
    const mlib_d64 *arr_scl);

mlib_status mlib_v_ImageHistogram2_U16_3(
    const void *src,
    mlib_s32 nchan,
    mlib_s32 size_row,
    mlib_s32 height,
    mlib_s32 stride,
    mlib_s32 xStart,
    mlib_s32 yStart,
    mlib_s32 yPeriod,
    mlib_s32 **histogr,
    const mlib_d64 *arr_min,
    const mlib_d64 *arr_max,
    const mlib_d64 *arr_scl);

mlib_status mlib_v_ImageHistogram2_S16_124(
    const void *src,
    mlib_s32 nchan,
    mlib_s32 size_row,
    mlib_s32 height,
    mlib_s32 stride,
    mlib_s32 xStart,
    mlib_s32 yStart,
    mlib_s32 yPeriod,
    mlib_s32 **histogr,
    const mlib_d64 *arr_min,
    const mlib_d64 *arr_max,
    const mlib_d64 *arr_scl);

mlib_status mlib_v_ImageHistogram2_S16_3(
    const void *src,
    mlib_s32 nchan,
    mlib_s32 size_row,
    mlib_s32 height,
    mlib_s32 stride,
    mlib_s32 xStart,
    mlib_s32 yStart,
    mlib_s32 yPeriod,
    mlib_s32 **histogr,
    const mlib_d64 *arr_min,
    const mlib_d64 *arr_max,
    const mlib_d64 *arr_scl);

#ifdef __cplusplus
}
#endif

#endif /* _MLIB_V_IMAGEHISTOGRAM2_H */
