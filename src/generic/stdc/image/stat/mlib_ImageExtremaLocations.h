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

#ifndef _MLIB_IMAGEEXTREMALOCATIONS_H
#define	_MLIB_IMAGEEXTREMALOCATIONS_H

#pragma ident	"@(#)mlib_ImageExtremaLocations.h	9.2	07/10/09 SMI"

#ifdef __cplusplus
extern "C" {
#endif

#include <mlib_image.h>

mlib_status mlib_ImageExtremaLocations_u8(
    mlib_s32 *min,
    mlib_s32 *max,
    const mlib_image *img,
    mlib_s32 xStart,
    mlib_s32 yStart,
    mlib_s32 xPeriod,
    mlib_s32 yPeriod,
    mlib_s32 maxRuns,
    mlib_s32 *minCounts,
    mlib_s32 *maxCounts,
    mlib_s32 **minLocations,
    mlib_s32 **maxLocations,
    mlib_s32 *buf_test_min,
    mlib_s32 *buf_test_max,
    mlib_s32 len);

mlib_status mlib_ImageExtremaLocations_s16(
    mlib_s32 *min,
    mlib_s32 *max,
    const mlib_image *img,
    mlib_s32 xStart,
    mlib_s32 yStart,
    mlib_s32 xPeriod,
    mlib_s32 yPeriod,
    mlib_s32 maxRuns,
    mlib_s32 *minCounts,
    mlib_s32 *maxCounts,
    mlib_s32 **minLocations,
    mlib_s32 **maxLocations,
    mlib_s32 *buf_test_min,
    mlib_s32 *buf_test_max,
    mlib_s32 len);

mlib_status mlib_ImageExtremaLocations_u16(
    mlib_s32 *min,
    mlib_s32 *max,
    const mlib_image *img,
    mlib_s32 xStart,
    mlib_s32 yStart,
    mlib_s32 xPeriod,
    mlib_s32 yPeriod,
    mlib_s32 maxRuns,
    mlib_s32 *minCounts,
    mlib_s32 *maxCounts,
    mlib_s32 **minLocations,
    mlib_s32 **maxLocations,
    mlib_s32 *buf_test_min,
    mlib_s32 *buf_test_max,
    mlib_s32 len);

mlib_status mlib_ImageExtremaLocations_s32(
    mlib_s32 *min,
    mlib_s32 *max,
    const mlib_image *img,
    mlib_s32 xStart,
    mlib_s32 yStart,
    mlib_s32 xPeriod,
    mlib_s32 yPeriod,
    mlib_s32 maxRuns,
    mlib_s32 *minCounts,
    mlib_s32 *maxCounts,
    mlib_s32 **minLocations,
    mlib_s32 **maxLocations,
    mlib_s32 *buf_test_min,
    mlib_s32 *buf_test_max,
    mlib_s32 len);

mlib_status mlib_ImageExtremaLocations_f32(
    mlib_d64 *min,
    mlib_d64 *max,
    const mlib_image *img,
    mlib_s32 xStart,
    mlib_s32 yStart,
    mlib_s32 xPeriod,
    mlib_s32 yPeriod,
    mlib_s32 maxRuns,
    mlib_s32 *minCounts,
    mlib_s32 *maxCounts,
    mlib_s32 **minLocations,
    mlib_s32 **maxLocations,
    mlib_s32 *buf_test_min,
    mlib_s32 *buf_test_max,
    mlib_s32 len);

mlib_status mlib_ImageExtremaLocations_d64(
    mlib_d64 *min,
    mlib_d64 *max,
    const mlib_image *img,
    mlib_s32 xStart,
    mlib_s32 yStart,
    mlib_s32 xPeriod,
    mlib_s32 yPeriod,
    mlib_s32 maxRuns,
    mlib_s32 *minCounts,
    mlib_s32 *maxCounts,
    mlib_s32 **minLocations,
    mlib_s32 **maxLocations,
    mlib_s32 *buf_test_min,
    mlib_s32 *buf_test_max,
    mlib_s32 len);

#ifdef __cplusplus
}
#endif

#endif /* _MLIB_IMAGEEXTREMALOCATIONS_H */
