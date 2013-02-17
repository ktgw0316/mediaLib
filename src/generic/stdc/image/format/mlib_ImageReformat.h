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

#ifndef _MLIB_IMAGEREFORMAT_H
#define	_MLIB_IMAGEREFORMAT_H

#pragma ident	"@(#)mlib_ImageReformat.h	9.2	07/10/09 SMI"

#ifdef __cplusplus
extern "C" {
#endif

#include <mlib_types.h>
#include <mlib_status.h>

/* *********************************************************** */
#define	REFORMAT_PARAMS1                                                  \
	void **dstData, const void **srcData, mlib_s32 numBands,          \
	    mlib_s32 xSize, mlib_s32 ySize,                               \
	    const mlib_s32 *dstBandoffsets, mlib_s32 dstScanlinestride,   \
	    mlib_s32 dstPixelstride, const mlib_s32 *srcBandoffsets,      \
	    mlib_s32 srcScanlinestride, mlib_s32 srcPixelstride
void mlib_ImageReformat_U8_U8(
    REFORMAT_PARAMS1);
void mlib_ImageReformat_U8_S16(
    REFORMAT_PARAMS1);
void mlib_ImageReformat_U8_U16(
    REFORMAT_PARAMS1);
void mlib_ImageReformat_U8_S32(
    REFORMAT_PARAMS1);
void mlib_ImageReformat_S16_U8(
    REFORMAT_PARAMS1);
void mlib_ImageReformat_S16_S16(
    REFORMAT_PARAMS1);
void mlib_ImageReformat_S16_U16(
    REFORMAT_PARAMS1);
void mlib_ImageReformat_S16_S32(
    REFORMAT_PARAMS1);
void mlib_ImageReformat_U16_U8(
    REFORMAT_PARAMS1);
void mlib_ImageReformat_U16_S16(
    REFORMAT_PARAMS1);
void mlib_ImageReformat_U16_U16(
    REFORMAT_PARAMS1);
void mlib_ImageReformat_U16_S32(
    REFORMAT_PARAMS1);
void mlib_ImageReformat_S32_U8(
    REFORMAT_PARAMS1);
void mlib_ImageReformat_S32_S16(
    REFORMAT_PARAMS1);
void mlib_ImageReformat_S32_U16(
    REFORMAT_PARAMS1);
void mlib_ImageReformat_S32_S32(
    REFORMAT_PARAMS1);

void mlib_ImageReformat_U8_F32(
    REFORMAT_PARAMS1);
void mlib_ImageReformat_U8_D64(
    REFORMAT_PARAMS1);
void mlib_ImageReformat_S16_F32(
    REFORMAT_PARAMS1);
void mlib_ImageReformat_S16_D64(
    REFORMAT_PARAMS1);
void mlib_ImageReformat_U16_F32(
    REFORMAT_PARAMS1);
void mlib_ImageReformat_U16_D64(
    REFORMAT_PARAMS1);
void mlib_ImageReformat_S32_F32(
    REFORMAT_PARAMS1);
void mlib_ImageReformat_S32_D64(
    REFORMAT_PARAMS1);

void mlib_ImageReformat_F32_U8(
    REFORMAT_PARAMS1);
void mlib_ImageReformat_F32_U16(
    REFORMAT_PARAMS1);
void mlib_ImageReformat_F32_S16(
    REFORMAT_PARAMS1);
void mlib_ImageReformat_F32_S32(
    REFORMAT_PARAMS1);
void mlib_ImageReformat_F32_F32(
    REFORMAT_PARAMS1);
void mlib_ImageReformat_F32_D64(
    REFORMAT_PARAMS1);
void mlib_ImageReformat_D64_U8(
    REFORMAT_PARAMS1);
void mlib_ImageReformat_D64_U16(
    REFORMAT_PARAMS1);
void mlib_ImageReformat_D64_S16(
    REFORMAT_PARAMS1);
void mlib_ImageReformat_D64_S32(
    REFORMAT_PARAMS1);
void mlib_ImageReformat_D64_F32(
    REFORMAT_PARAMS1);
void mlib_ImageReformat_D64_D64(
    REFORMAT_PARAMS1);

#ifdef __cplusplus
}
#endif

#endif /* _MLIB_IMAGEREFORMAT_H */
