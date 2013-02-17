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

#ifndef _MLIB_IMAGELOOKUP_H
#define	_MLIB_IMAGELOOKUP_H

#pragma ident	"@(#)mlib_ImageLookUp.h	9.2	07/10/09 SMI"

#ifdef __cplusplus
extern "C" {
#endif

#include <mlib_types.h>
#include <mlib_ImageCopy.h>

#ifdef _MSC_VER
#define	TABLE_SHIFT_S32	(mlib_u32)2147483648
#else		   /* _MSC_VER */
#define	TABLE_SHIFT_S32	2147483648u
#endif		   /* _MSC_VER */
/* mlib_ImageLookUp_64.c */
void mlib_ImageLookUp_U8_D64(
    const mlib_u8 *src,
    mlib_s32 slb,
    mlib_d64 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 csize,
    const mlib_d64 **table);

void mlib_ImageLookUp_S16_D64(
    const mlib_s16 *src,
    mlib_s32 slb,
    mlib_d64 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 csize,
    const mlib_d64 **table);

void mlib_ImageLookUp_U16_D64(
    const mlib_u16 *src,
    mlib_s32 slb,
    mlib_d64 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 csize,
    const mlib_d64 **table);

void mlib_ImageLookUp_S32_D64(
    const mlib_s32 *src,
    mlib_s32 slb,
    mlib_d64 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 csize,
    const mlib_d64 **table);

void mlib_ImageLookUpSI_U8_D64(
    const mlib_u8 *src,
    mlib_s32 slb,
    mlib_d64 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 csize,
    const mlib_d64 **table);

void mlib_ImageLookUpSI_S16_D64(
    const mlib_s16 *src,
    mlib_s32 slb,
    mlib_d64 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 csize,
    const mlib_d64 **table);

void mlib_ImageLookUpSI_U16_D64(
    const mlib_u16 *src,
    mlib_s32 slb,
    mlib_d64 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 csize,
    const mlib_d64 **table);

void mlib_ImageLookUpSI_S32_D64(
    const mlib_s32 *src,
    mlib_s32 slb,
    mlib_d64 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 csize,
    const mlib_d64 **table);

/* mlib_ImageLookUp_Bit.c */

mlib_status mlib_ImageLookUp_Bit_U8_1(
    const mlib_u8 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 nchan,
    mlib_s32 bitoff,
    const mlib_u8 **table);

mlib_status mlib_ImageLookUp_Bit_U8_2(
    const mlib_u8 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 nchan,
    mlib_s32 bitoff,
    const mlib_u8 **table);

mlib_status mlib_ImageLookUp_Bit_U8_3(
    const mlib_u8 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 nchan,
    mlib_s32 bitoff,
    const mlib_u8 **table);

mlib_status mlib_ImageLookUp_Bit_U8_4(
    const mlib_u8 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 nchan,
    mlib_s32 bitoff,
    const mlib_u8 **table);

#ifdef __cplusplus
}
#endif

#endif /* _MLIB_IMAGELOOKUP_H */
