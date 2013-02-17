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

#ifndef _MLIB_IMAGELOOKUPMASK_H
#define	_MLIB_IMAGELOOKUPMASK_H

#pragma ident	"@(#)mlib_ImageLookUpMask.h	9.2	07/10/09 SMI"

#ifdef __cplusplus
extern "C" {
#endif

#include <mlib_types.h>

void mlib_ImageLookUpMask_D64_U8(
    mlib_u8 *da,
    mlib_s32 dlb,
    const mlib_u8 *sa,
    mlib_s32 slb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_d64 **table,
    const mlib_s32 *dst_i,
    const mlib_s32 *src_i,
    const mlib_s32 *table_i,
    mlib_s32 dchan,
    mlib_s32 schan,
    mlib_s32 chan);

void mlib_ImageLookUpMask_3_4_D64_U8(
    mlib_d64 *dst,
    mlib_s32 dlb,
    const mlib_u8 *src,
    mlib_s32 slb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_d64 **table,
    const mlib_s32 *dst_i,
    const mlib_s32 *table_i,
    mlib_s32 dchan);

void mlib_ImageLookUpMask_4_D64_U8(
    mlib_d64 *dst,
    mlib_s32 dlb,
    const mlib_u8 *src,
    mlib_s32 slb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_d64 **table,
    const mlib_s32 *dst_i,
    const mlib_s32 *table_i);

void mlib_ImageLookUpMask_D64_S16(
    mlib_u8 *da,
    mlib_s32 dlb,
    const mlib_u8 *sa,
    mlib_s32 slb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_d64 **table,
    const mlib_s32 *dst_i,
    const mlib_s32 *src_i,
    const mlib_s32 *table_i,
    mlib_s32 dchan,
    mlib_s32 schan,
    mlib_s32 chan);

void mlib_ImageLookUpMask_D64_U16(
    mlib_u8 *da,
    mlib_s32 dlb,
    const mlib_u8 *sa,
    mlib_s32 slb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_d64 **table,
    const mlib_s32 *dst_i,
    const mlib_s32 *src_i,
    const mlib_s32 *table_i,
    mlib_s32 dchan,
    mlib_s32 schan,
    mlib_s32 chan);

void mlib_ImageLookUpMask_D64_S32(
    mlib_u8 *da,
    mlib_s32 dlb,
    const mlib_u8 *sa,
    mlib_s32 slb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_d64 **table,
    const mlib_s32 *dst_i,
    const mlib_s32 *src_i,
    const mlib_s32 *table_i,
    mlib_s32 dchan,
    mlib_s32 schan,
    mlib_s32 chan);

void mlib_ImageLookUpMask_U8_U8(
    mlib_u8 *da,
    mlib_s32 dlb,
    const mlib_u8 *sa,
    mlib_s32 slb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_u8 **table,
    const mlib_s32 *dst_i,
    const mlib_s32 *src_i,
    const mlib_s32 *table_i,
    mlib_s32 dchan,
    mlib_s32 schan,
    mlib_s32 chan);

void mlib_ImageLookUpMask_3_4_U8_U8(
    mlib_u8 *dst,
    mlib_s32 dlb,
    const mlib_u8 *src,
    mlib_s32 slb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_u8 **table,
    const mlib_s32 *dst_i,
    const mlib_s32 *table_i,
    mlib_s32 dchan);

void mlib_ImageLookUpMask_4_U8_U8(
    mlib_u8 *dst,
    mlib_s32 dlb,
    const mlib_u8 *src,
    mlib_s32 slb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_u8 **table,
    const mlib_s32 *dst_i,
    const mlib_s32 *table_i);

void mlib_ImageLookUpMask_U8_S16(
    mlib_u8 *da,
    mlib_s32 dlb,
    const mlib_u8 *sa,
    mlib_s32 slb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_u8 **table,
    const mlib_s32 *dst_i,
    const mlib_s32 *src_i,
    const mlib_s32 *table_i,
    mlib_s32 dchan,
    mlib_s32 schan,
    mlib_s32 chan);

void mlib_ImageLookUpMask_U8_U16(
    mlib_u8 *da,
    mlib_s32 dlb,
    const mlib_u8 *sa,
    mlib_s32 slb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_u8 **table,
    const mlib_s32 *dst_i,
    const mlib_s32 *src_i,
    const mlib_s32 *table_i,
    mlib_s32 dchan,
    mlib_s32 schan,
    mlib_s32 chan);

void mlib_ImageLookUpMask_S16_U16(
    mlib_u8 *da,
    mlib_s32 dlb,
    const mlib_u8 *sa,
    mlib_s32 slb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_s16 **table,
    const mlib_s32 *dst_i,
    const mlib_s32 *src_i,
    const mlib_s32 *table_i,
    mlib_s32 dchan,
    mlib_s32 schan,
    mlib_s32 chan);

void mlib_ImageLookUpMask_U8_S32(
    mlib_u8 *da,
    mlib_s32 dlb,
    const mlib_u8 *sa,
    mlib_s32 slb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_u8 **table,
    const mlib_s32 *dst_i,
    const mlib_s32 *src_i,
    const mlib_s32 *table_i,
    mlib_s32 dchan,
    mlib_s32 schan,
    mlib_s32 chan);

void mlib_ImageLookUpMask_S16_U8(
    mlib_u8 *da,
    mlib_s32 dlb,
    const mlib_u8 *sa,
    mlib_s32 slb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_s16 **table,
    const mlib_s32 *dst_i,
    const mlib_s32 *src_i,
    const mlib_s32 *table_i,
    mlib_s32 dchan,
    mlib_s32 schan,
    mlib_s32 chan);

void mlib_ImageLookUpMask_3_4_S16_U8(
    mlib_s16 *dst,
    mlib_s32 dlb,
    const mlib_u8 *src,
    mlib_s32 slb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_s16 **table,
    const mlib_s32 *dst_i,
    const mlib_s32 *table_i,
    mlib_s32 dchan);

void mlib_ImageLookUpMask_4_S16_U8(
    mlib_s16 *dst,
    mlib_s32 dlb,
    const mlib_u8 *src,
    mlib_s32 slb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_s16 **table,
    const mlib_s32 *dst_i,
    const mlib_s32 *table_i);

void mlib_ImageLookUpMask_S16_S16(
    mlib_u8 *da,
    mlib_s32 dlb,
    const mlib_u8 *sa,
    mlib_s32 slb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_s16 **table,
    const mlib_s32 *dst_i,
    const mlib_s32 *src_i,
    const mlib_s32 *table_i,
    mlib_s32 dchan,
    mlib_s32 schan,
    mlib_s32 chan);

void mlib_ImageLookUpMask_S16_S32(
    mlib_u8 *da,
    mlib_s32 dlb,
    const mlib_u8 *sa,
    mlib_s32 slb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_s16 **table,
    const mlib_s32 *dst_i,
    const mlib_s32 *src_i,
    const mlib_s32 *table_i,
    mlib_s32 dchan,
    mlib_s32 schan,
    mlib_s32 chan);

void mlib_ImageLookUpMask_S32_U8(
    mlib_u8 *da,
    mlib_s32 dlb,
    const mlib_u8 *sa,
    mlib_s32 slb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_s32 **table,
    const mlib_s32 *dst_i,
    const mlib_s32 *src_i,
    const mlib_s32 *table_i,
    mlib_s32 dchan,
    mlib_s32 schan,
    mlib_s32 chan);

void mlib_ImageLookUpMask_3_4_S32_U8(
    mlib_s32 *dst,
    mlib_s32 dlb,
    const mlib_u8 *src,
    mlib_s32 slb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_s32 **table,
    const mlib_s32 *dst_i,
    const mlib_s32 *table_i,
    mlib_s32 dchan);

void mlib_ImageLookUpMask_4_S32_U8(
    mlib_s32 *dst,
    mlib_s32 dlb,
    const mlib_u8 *src,
    mlib_s32 slb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_s32 **table,
    const mlib_s32 *dst_i,
    const mlib_s32 *table_i);

void mlib_ImageLookUpMask_S32_S16(
    mlib_u8 *da,
    mlib_s32 dlb,
    const mlib_u8 *sa,
    mlib_s32 slb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_s32 **table,
    const mlib_s32 *dst_i,
    const mlib_s32 *src_i,
    const mlib_s32 *table_i,
    mlib_s32 dchan,
    mlib_s32 schan,
    mlib_s32 chan);

void mlib_ImageLookUpMask_S32_U16(
    mlib_u8 *da,
    mlib_s32 dlb,
    const mlib_u8 *sa,
    mlib_s32 slb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_s32 **table,
    const mlib_s32 *dst_i,
    const mlib_s32 *src_i,
    const mlib_s32 *table_i,
    mlib_s32 dchan,
    mlib_s32 schan,
    mlib_s32 chan);

void mlib_ImageLookUpMask_S32_S32(
    mlib_u8 *da,
    mlib_s32 dlb,
    const mlib_u8 *sa,
    mlib_s32 slb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_s32 **table,
    const mlib_s32 *dst_i,
    const mlib_s32 *src_i,
    const mlib_s32 *table_i,
    mlib_s32 dchan,
    mlib_s32 schan,
    mlib_s32 chan);

#ifdef __cplusplus
}
#endif

#endif /* _MLIB_IMAGELOOKUPMASK_H */
