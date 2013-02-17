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

#ifndef _MLIB_V_IMAGELOOKUPMASKFUNC_H
#define	_MLIB_V_IMAGELOOKUPMASKFUNC_H

#pragma ident	"@(#)mlib_v_ImageLookUpMaskFunc.h	9.2	07/11/05 SMI"

#ifdef __cplusplus
extern "C" {
#endif

#include <mlib_types.h>
#include <mlib_status.h>

void mlib_v_ImageLookUpMaskMN_S16_U8(
    mlib_s16 *dst,
    mlib_s32 dlb,
    const mlib_u8 *src,
    mlib_s32 slb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_s16 **table,
    const mlib_s32 *dst_i,
    const mlib_s32 *table_i,
    mlib_s32 dchan,
    mlib_s32 schan,
    mlib_s32 chan);

void mlib_v_ImageLookUpMaskMN_U8_U8(
    mlib_u8 *dst,
    mlib_s32 dlb,
    const mlib_u8 *src,
    mlib_s32 slb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_u8 **table,
    const mlib_s32 *dst_i,
    const mlib_s32 *table_i,
    mlib_s32 dchan,
    mlib_s32 schan,
    mlib_s32 chan);

void mlib_v_ImageLookUpMaskSI_S16_U8(
    mlib_s16 *dst,
    mlib_s32 dlb,
    const mlib_u8 *src,
    mlib_s32 slb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_s16 **table,
    const mlib_s32 *dst_i,
    const mlib_s32 *table_i,
    mlib_s32 dchan,
    mlib_s32 chan);

void mlib_v_ImageLookUpMaskSI_S32_U8(
    mlib_s32 *dst,
    mlib_s32 dlb,
    const mlib_u8 *src,
    mlib_s32 slb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_s32 **table,
    const mlib_s32 *dst_i,
    const mlib_s32 *table_i,
    mlib_s32 dchan,
    mlib_s32 chan);

void mlib_v_ImageLookUpMaskSI_U8_U8(
    mlib_u8 *dst,
    mlib_s32 dlb,
    const mlib_u8 *src,
    mlib_s32 slb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_u8 **table,
    const mlib_s32 *dst_i,
    const mlib_s32 *table_i,
    mlib_s32 dchan,
    mlib_s32 chan);

#ifdef __cplusplus
}
#endif

#endif /* _MLIB_V_IMAGELOOKUPMASKFUNC_H */
