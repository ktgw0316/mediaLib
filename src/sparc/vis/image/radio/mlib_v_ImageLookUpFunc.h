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

#ifndef _MLIB_V_IMAGELOOKUPFUNC_H
#define	_MLIB_V_IMAGELOOKUPFUNC_H

#pragma ident	"@(#)mlib_v_ImageLookUpFunc.h	9.2	07/11/05 SMI"

#ifdef __cplusplus
extern "C" {
#endif

#include <mlib_types.h>
#include <mlib_status.h>

/* mlib_v_ImageLookUpS16S16Func.c */
void mlib_v_ImageLookUp_S16_S16_1(
    const mlib_s16 *src,
    mlib_s32 slb,
    mlib_s16 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_s16 **table);

void mlib_v_ImageLookUp_S16_S16_2(
    const mlib_s16 *src,
    mlib_s32 slb,
    mlib_s16 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_s16 **table);

void mlib_v_ImageLookUp_S16_S16_3(
    const mlib_s16 *src,
    mlib_s32 slb,
    mlib_s16 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_s16 **table);

void mlib_v_ImageLookUp_S16_S16_4(
    const mlib_s16 *src,
    mlib_s32 slb,
    mlib_s16 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_s16 **table);

/* mlib_v_ImageLookUpS16S32Func.c */

void mlib_v_ImageLookUp_S16_S32_1(
    const mlib_s16 *src,
    mlib_s32 slb,
    mlib_s32 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_s32 **table);

void mlib_v_ImageLookUp_S16_S32_2(
    const mlib_s16 *src,
    mlib_s32 slb,
    mlib_s32 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_s32 **table);

void mlib_v_ImageLookUp_S16_S32_3(
    const mlib_s16 *src,
    mlib_s32 slb,
    mlib_s32 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_s32 **table);

void mlib_v_ImageLookUp_S16_S32_4(
    const mlib_s16 *src,
    mlib_s32 slb,
    mlib_s32 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_s32 **table);

/* mlib_v_ImageLookUpS16U8Func.c */

void mlib_v_ImageLookUp_S16_U8_1(
    const mlib_s16 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_u8 **table);

void mlib_v_ImageLookUp_S16_U8_2(
    const mlib_s16 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_u8 **table);

void mlib_v_ImageLookUp_S16_U8_3(
    const mlib_s16 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_u8 **table);

void mlib_v_ImageLookUp_S16_U8_4(
    const mlib_s16 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_u8 **table);

/* mlib_v_ImageLookUpS32S16Func.c */

void mlib_v_ImageLookUp_S32_S16_1(
    const mlib_s32 *src,
    mlib_s32 slb,
    mlib_s16 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_s16 **table);

void mlib_v_ImageLookUp_S32_S16_2(
    const mlib_s32 *src,
    mlib_s32 slb,
    mlib_s16 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_s16 **table);

void mlib_v_ImageLookUp_S32_S16_3(
    const mlib_s32 *src,
    mlib_s32 slb,
    mlib_s16 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_s16 **table);

void mlib_v_ImageLookUp_S32_S16_4(
    const mlib_s32 *src,
    mlib_s32 slb,
    mlib_s16 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_s16 **table);

/* mlib_v_ImageLookUpS32S32Func.c */

void mlib_v_ImageLookUp_S32_S32(
    const mlib_s32 *src,
    mlib_s32 slb,
    mlib_s32 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_s32 **table,
    mlib_s32 csize);

/* mlib_v_ImageLookUpS32U8Func.c */

void mlib_v_ImageLookUp_S32_U8_1(
    const mlib_s32 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_u8 **table);

void mlib_v_ImageLookUp_S32_U8_2(
    const mlib_s32 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_u8 **table);

void mlib_v_ImageLookUp_S32_U8_3(
    const mlib_s32 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_u8 **table);

void mlib_v_ImageLookUp_S32_U8_4(
    const mlib_s32 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_u8 **table);

/* mlib_v_ImageLookUpSIS16S16Func.c */

void mlib_v_ImageLookUpSI_S16_S16_2(
    const mlib_s16 *src,
    mlib_s32 slb,
    mlib_s16 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_s16 **table);

void mlib_v_ImageLookUpSI_S16_S16_3(
    const mlib_s16 *src,
    mlib_s32 slb,
    mlib_s16 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_s16 **table);

void mlib_v_ImageLookUpSI_S16_S16_4(
    const mlib_s16 *src,
    mlib_s32 slb,
    mlib_s16 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_s16 **table);

/* mlib_v_ImageLookUpSIS16S32Func.c */

void mlib_v_ImageLookUpSI_S16_S32_2(
    const mlib_s16 *src,
    mlib_s32 slb,
    mlib_s32 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_s32 **table);

void mlib_v_ImageLookUpSI_S16_S32_3(
    const mlib_s16 *src,
    mlib_s32 slb,
    mlib_s32 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_s32 **table);

void mlib_v_ImageLookUpSI_S16_S32_4(
    const mlib_s16 *src,
    mlib_s32 slb,
    mlib_s32 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_s32 **table);

/* mlib_v_ImageLookUpSIS16U8Func.c */

void mlib_v_ImageLookUpSI_S16_U8_2(
    const mlib_s16 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_u8 **table);

void mlib_v_ImageLookUpSI_S16_U8_3(
    const mlib_s16 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_u8 **table);

void mlib_v_ImageLookUpSI_S16_U8_4(
    const mlib_s16 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_u8 **table);

/* mlib_v_ImageLookUpSIS32S16Func.c */

void mlib_v_ImageLookUpSI_S32_S16_2(
    const mlib_s32 *src,
    mlib_s32 slb,
    mlib_s16 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_s16 **table);

void mlib_v_ImageLookUpSI_S32_S16_3(
    const mlib_s32 *src,
    mlib_s32 slb,
    mlib_s16 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_s16 **table);

void mlib_v_ImageLookUpSI_S32_S16_4(
    const mlib_s32 *src,
    mlib_s32 slb,
    mlib_s16 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_s16 **table);

/* mlib_v_ImageLookUpSIS32S32Func.c */

void mlib_v_ImageLookUpSI_S32_S32(
    const mlib_s32 *src,
    mlib_s32 slb,
    mlib_s32 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_s32 **table,
    mlib_s32 csize);

/* mlib_v_ImageLookUpSIS32U8Func.c */

void mlib_v_ImageLookUpSI_S32_U8_2(
    const mlib_s32 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_u8 **table);

void mlib_v_ImageLookUpSI_S32_U8_3(
    const mlib_s32 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_u8 **table);

void mlib_v_ImageLookUpSI_S32_U8_4(
    const mlib_s32 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_u8 **table);

/* mlib_v_ImageLookUpSIU8S16Func.c */

void mlib_v_ImageLookUpSI_U8_S16_2(
    const mlib_u8 *src,
    mlib_s32 slb,
    mlib_s16 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_s16 **table);

void mlib_v_ImageLookUpSI_U8_S16_3(
    const mlib_u8 *src,
    mlib_s32 slb,
    mlib_s16 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_s16 **table);

void mlib_v_ImageLookUpSI_U8_S16_4(
    const mlib_u8 *src,
    mlib_s32 slb,
    mlib_s16 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_s16 **table);

/* mlib_v_ImageLookUpSIU8S32Func.c */

void mlib_v_ImageLookUpSI_U8_S32_2(
    const mlib_u8 *src,
    mlib_s32 slb,
    mlib_s32 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_s32 **table);

void mlib_v_ImageLookUpSI_U8_S32_3(
    const mlib_u8 *src,
    mlib_s32 slb,
    mlib_s32 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_s32 **table);

void mlib_v_ImageLookUpSI_U8_S32_4(
    const mlib_u8 *src,
    mlib_s32 slb,
    mlib_s32 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_s32 **table);

/* mlib_v_ImageLookUpSIU8U8Func.c */

void mlib_v_ImageLookUpSI_U8_U8_2(
    const mlib_u8 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_u8 **table);

void mlib_v_ImageLookUpSI_U8_U8_3(
    const mlib_u8 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_u8 **table);

void mlib_v_ImageLookUpSI_U8_U8_4(
    const mlib_u8 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_u8 **table);

/* mlib_v_ImageLookUpU8S16Func.c */

void mlib_v_ImageLookUp_U8_S16_1(
    const mlib_u8 *src,
    mlib_s32 slb,
    mlib_s16 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_s16 **table);

void mlib_v_ImageLookUp_U8_S16_2(
    const mlib_u8 *src,
    mlib_s32 slb,
    mlib_s16 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_s16 **table);

void mlib_v_ImageLookUp_U8_S16_3(
    const mlib_u8 *src,
    mlib_s32 slb,
    mlib_s16 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_s16 **table);

void mlib_v_ImageLookUp_U8_S16_4(
    const mlib_u8 *src,
    mlib_s32 slb,
    mlib_s16 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_s16 **table);

/* mlib_v_ImageLookUpU8S32Func.c */

void mlib_v_ImageLookUp_U8_S32_1(
    const mlib_u8 *src,
    mlib_s32 slb,
    mlib_s32 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_s32 **table);

void mlib_v_ImageLookUp_U8_S32_2(
    const mlib_u8 *src,
    mlib_s32 slb,
    mlib_s32 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_s32 **table);

void mlib_v_ImageLookUp_U8_S32_3(
    const mlib_u8 *src,
    mlib_s32 slb,
    mlib_s32 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_s32 **table);

void mlib_v_ImageLookUp_U8_S32_4(
    const mlib_u8 *src,
    mlib_s32 slb,
    mlib_s32 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_s32 **table);

/* mlib_v_ImageLookUpU8U8Func.c */

void mlib_v_ImageLookUp_U8_U8_1(
    const mlib_u8 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_u8 **table);

void mlib_v_ImageLookUp_U8_U8_2(
    const mlib_u8 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_u8 **table);

void mlib_v_ImageLookUp_U8_U8_3(
    const mlib_u8 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_u8 **table);

void mlib_v_ImageLookUp_U8_U8_4(
    const mlib_u8 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_u8 **table);

/* mlib_v_ImageLookUpU16S32Func.c */

void mlib_v_ImageLookUp_U16_S32_1(
    const mlib_u16 *src,
    mlib_s32 slb,
    mlib_s32 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_s32 **table);

void mlib_v_ImageLookUp_U16_S32_2(
    const mlib_u16 *src,
    mlib_s32 slb,
    mlib_s32 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_s32 **table);

void mlib_v_ImageLookUp_U16_S32_3(
    const mlib_u16 *src,
    mlib_s32 slb,
    mlib_s32 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_s32 **table);

void mlib_v_ImageLookUp_U16_S32_4(
    const mlib_u16 *src,
    mlib_s32 slb,
    mlib_s32 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_s32 **table);

/* mlib_v_ImageLookUpSIU16S32Func.c */

void mlib_v_ImageLookUpSI_U16_S32_2(
    const mlib_u16 *src,
    mlib_s32 slb,
    mlib_s32 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_s32 **table);

void mlib_v_ImageLookUpSI_U16_S32_3(
    const mlib_u16 *src,
    mlib_s32 slb,
    mlib_s32 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_s32 **table);

void mlib_v_ImageLookUpSI_U16_S32_4(
    const mlib_u16 *src,
    mlib_s32 slb,
    mlib_s32 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_s32 **table);

/* mlib_v_ImageLookUpU16S16Func.c */

void mlib_v_ImageLookUp_U16_S16_1(
    const mlib_u16 *src,
    mlib_s32 slb,
    mlib_s16 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_s16 **table);

void mlib_v_ImageLookUp_U16_S16_2(
    const mlib_u16 *src,
    mlib_s32 slb,
    mlib_s16 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_s16 **table);

void mlib_v_ImageLookUp_U16_S16_3(
    const mlib_u16 *src,
    mlib_s32 slb,
    mlib_s16 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_s16 **table);

void mlib_v_ImageLookUp_U16_S16_4(
    const mlib_u16 *src,
    mlib_s32 slb,
    mlib_s16 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_s16 **table);

/* mlib_v_ImageLookUpSIU16S16Func.c */

void mlib_v_ImageLookUpSI_U16_S16_2(
    const mlib_u16 *src,
    mlib_s32 slb,
    mlib_s16 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_s16 **table);

void mlib_v_ImageLookUpSI_U16_S16_3(
    const mlib_u16 *src,
    mlib_s32 slb,
    mlib_s16 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_s16 **table);

void mlib_v_ImageLookUpSI_U16_S16_4(
    const mlib_u16 *src,
    mlib_s32 slb,
    mlib_s16 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_s16 **table);

/* mlib_v_ImageLookUpU16U8Func.c */

void mlib_v_ImageLookUp_U16_U8_1(
    const mlib_u16 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_u8 **table);

void mlib_v_ImageLookUp_U16_U8_2(
    const mlib_u16 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_u8 **table);

void mlib_v_ImageLookUp_U16_U8_3(
    const mlib_u16 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_u8 **table);

void mlib_v_ImageLookUp_U16_U8_4(
    const mlib_u16 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_u8 **table);

/* mlib_v_ImageLookUpSIU16U8Func.c */

void mlib_v_ImageLookUpSI_U16_U8_2(
    const mlib_u16 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_u8 **table);

void mlib_v_ImageLookUpSI_U16_U8_3(
    const mlib_u16 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_u8 **table);

void mlib_v_ImageLookUpSI_U16_U8_4(
    const mlib_u16 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_u8 **table);

#ifdef __cplusplus
}
#endif

#endif /* _MLIB_V_IMAGELOOKUPFUNC_H */
