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

#pragma ident   "@(#)mlib_s_ImageLookUp.c	9.5	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_ImageLookUp - table lookup
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageLookUp(mlib_image       *dst,
 *                                   const mlib_image *src,
 *                                   const void       **table)
 *
 * ARGUMENT
 *      dst      Pointer to destination image.
 *      src      Pointer to source image.
 *      table    Lookup table.
 *
 * DESCRIPTION
 *      The mlib_ImageLookUp function performs general table lookup on an
 *      image. The destination image is obtained by passing a source image
 *      through a lookup table.
 *
 *      The source image may be 1-, 2-, 3-, or 4-channeled of data types
 *      MLIB_BIT, MLIB_BYTE, MLIB_SHORT, MLIB_USHORT, or MLIB_INT. The lookup
 *      table may be 1-, 2-, 3-, or 4-channeled of data types MLIB_BYTE,
 *      MLIB_SHORT, MLIB_USHORT, MLIB_INT, MLIB_FLOAT, or MLIB_DOUBLE.
 *      The destination image must have the same
 *      number of channels as either source image or the lookup table,
 *      whichever is greater, and the same data type as the lookup table.
 *
 *      It is the user's responsibility to make sure that the lookup table
 *      supplied is suitable for the source image. Specifically, the table
 *      entries cover the entire range of source data. Otherwise, the result
 *      of this function is undefined.
 *
 *      The pixel values of the destination image are defined as the following:
 *
 *      If the source image is single-channeled and the destination image is
 *      multi-channeled, then the lookup table has the same number of channels
 *      as the destination image:
 *
 *          dst[x][y][c] = table[c][src[x][y][0]]
 *
 *      If the source image is multi-channeled and the destination image is
 *      multi-channeled, with the same number of channels as the source image,
 *      then the lookup table will have the same number of channels as
 *      the source image:
 *
 *          dst[x][y][c] = table[c][src[x][y][c]]
 */

/* *********************************************************** */

#include <mlib_image.h>
#include <mlib_ImageCheck.h>
#ifdef __SUNPRO_C
#include <sunmedia_intrin.h>
#else
#include <mmintrin.h>
#endif

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageLookUp = __mlib_ImageLookUp

#elif defined(__GNUC__)  /* defined(__SUNPRO_C) */

__typeof__(__mlib_ImageLookUp) mlib_ImageLookUp
    __attribute__((weak, alias("__mlib_ImageLookUp")));

#else /* defined(__SUNPRO_C) */

#error "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

    mlib_status mlib_ImageLookUp_Bit_U8_1(
    const mlib_u8 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 csize,
    mlib_s32 bitoff,
    const mlib_u8 **table);

    mlib_status mlib_ImageLookUp_Bit_U8_2(
    const mlib_u8 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 csize,
    mlib_s32 bitoff,
    const mlib_u8 **table);

    mlib_status mlib_ImageLookUp_Bit_U8_3(
    const mlib_u8 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 csize,
    mlib_s32 bitoff,
    const mlib_u8 **table);

    mlib_status mlib_ImageLookUp_Bit_U8_4(
    const mlib_u8 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 csize,
    mlib_s32 bitoff,
    const mlib_u8 **table);

/* *********************************************************** */

    void mlib_s_ImageLookUp_U8_U8_1(
    const mlib_u8 *sa,
    mlib_s32 slb,
    mlib_u8 *da,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_u8 **table);

    void mlib_s_ImageLookUp_U8_U8_2(
    const mlib_u8 *sa,
    mlib_s32 slb,
    mlib_u8 *da,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_u8 **table);

    void mlib_s_ImageLookUp_U8_U8_3(
    const mlib_u8 *sa,
    mlib_s32 slb,
    mlib_u8 *da,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_u8 **table);

    void mlib_s_ImageLookUp_U8_U8_4(
    const mlib_u8 *sa,
    mlib_s32 slb,
    mlib_u8 *da,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_u8 **table);

/* *********************************************************** */

    void mlib_s_ImageLookUpSI_U8_U8_2(
    const mlib_u8 *sa,
    mlib_s32 slb,
    mlib_u8 *da,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_u8 *table);

    void mlib_s_ImageLookUpSI_U8_U8_3(
    const mlib_u8 *sa,
    mlib_s32 slb,
    mlib_u8 *da,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_u8 **table,
    const mlib_u8 *stbl);

    void mlib_s_ImageLookUpSI_U8_U8_4(
    const mlib_u8 *sa,
    mlib_s32 slb,
    mlib_u8 *da,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_u8 *table);

/* *********************************************************** */

    void mlib_s_ImageLookUp_U8_S16_1(
    const mlib_u8 *sa,
    mlib_s32 slb,
    mlib_s16 *da,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_s16 **table);

    void mlib_s_ImageLookUp_U8_S16_2(
    const mlib_u8 *sa,
    mlib_s32 slb,
    mlib_s16 *da,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_s16 **table);

    void mlib_s_ImageLookUp_U8_S16_3(
    const mlib_u8 *sa,
    mlib_s32 slb,
    mlib_s16 *da,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_s16 **table);

    void mlib_s_ImageLookUp_U8_S16_4(
    const mlib_u8 *sa,
    mlib_s32 slb,
    mlib_s16 *da,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_s16 **table);

/* *********************************************************** */

    void mlib_s_ImageLookUpSI_U8_S16_2(
    const mlib_u8 *sa,
    mlib_s32 slb,
    mlib_s16 *da,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_s16 *table);

    void mlib_s_ImageLookUpSI_U8_S16_3(
    const mlib_u8 *sa,
    mlib_s32 slb,
    mlib_s16 *da,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_s16 **table);

    void mlib_s_ImageLookUpSI_U8_S16_4(
    const mlib_u8 *sa,
    mlib_s32 slb,
    mlib_s16 *da,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_s16 *table);

/* *********************************************************** */

    void mlib_s_ImageLookUp_U8_S32_1(
    const mlib_u8 *sa,
    mlib_s32 slb,
    mlib_s32 *da,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_s32 **table);

    void mlib_s_ImageLookUp_U8_S32_2(
    const mlib_u8 *sa,
    mlib_s32 slb,
    mlib_s32 *da,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_s32 **table);

    void mlib_s_ImageLookUp_U8_S32_3(
    const mlib_u8 *sa,
    mlib_s32 slb,
    mlib_s32 *da,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_s32 **table);

    void mlib_s_ImageLookUp_U8_S32_4(
    const mlib_u8 *sa,
    mlib_s32 slb,
    mlib_s32 *da,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_s32 **table);

/* *********************************************************** */

    void mlib_s_ImageLookUpSI_U8_S32_2(
    const mlib_u8 *sa,
    mlib_s32 slb,
    mlib_s32 *da,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_s32 *table);

    void mlib_s_ImageLookUpSI_U8_S32_3(
    const mlib_u8 *sa,
    mlib_s32 slb,
    mlib_s32 *da,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_s32 **table);

    void mlib_s_ImageLookUpSI_U8_S32_4(
    const mlib_u8 *sa,
    mlib_s32 slb,
    mlib_s32 *da,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_s32 *table);

/* *********************************************************** */

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

/* *********************************************************** */

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

/* *********************************************************** */

    void mlib_s_ImageLookUp_S16_U8_1(
    const mlib_s16 *sa,
    mlib_s32 slb,
    mlib_u8 *da,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_u8 **table);

    void mlib_s_ImageLookUp_S16_U8_2(
    const mlib_s16 *sa,
    mlib_s32 slb,
    mlib_u8 *da,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_u8 **table);

    void mlib_s_ImageLookUp_S16_U8_3(
    const mlib_s16 *sa,
    mlib_s32 slb,
    mlib_u8 *da,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_u8 **table);

    void mlib_s_ImageLookUp_S16_U8_4(
    const mlib_s16 *sa,
    mlib_s32 slb,
    mlib_u8 *da,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_u8 **table);

/* *********************************************************** */

    void mlib_s_ImageLookUpSI_S16_U8_2(
    const mlib_s16 *sa,
    mlib_s32 slb,
    mlib_u8 *da,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_u8 **table);

    void mlib_s_ImageLookUpSI_S16_U8_3(
    const mlib_s16 *sa,
    mlib_s32 slb,
    mlib_u8 *da,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_u8 **table);

    void mlib_s_ImageLookUpSI_S16_U8_4(
    const mlib_s16 *sa,
    mlib_s32 slb,
    mlib_u8 *da,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_u8 **table);

/* *********************************************************** */

    void mlib_s_ImageLookUp_U16_U8_1(
    const mlib_u16 *sa,
    mlib_s32 slb,
    mlib_u8 *da,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_u8 **table);

    void mlib_s_ImageLookUp_U16_U8_2(
    const mlib_u16 *sa,
    mlib_s32 slb,
    mlib_u8 *da,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_u8 **table);

    void mlib_s_ImageLookUp_U16_U8_3(
    const mlib_u16 *sa,
    mlib_s32 slb,
    mlib_u8 *da,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_u8 **table);

    void mlib_s_ImageLookUp_U16_U8_4(
    const mlib_u16 *sa,
    mlib_s32 slb,
    mlib_u8 *da,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_u8 **table);

/* *********************************************************** */

    void mlib_s_ImageLookUpSI_U16_U8_2(
    const mlib_u16 *sa,
    mlib_s32 slb,
    mlib_u8 *da,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_u8 **table);

    void mlib_s_ImageLookUpSI_U16_U8_3(
    const mlib_u16 *sa,
    mlib_s32 slb,
    mlib_u8 *da,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_u8 **table);

    void mlib_s_ImageLookUpSI_U16_U8_4(
    const mlib_u16 *sa,
    mlib_s32 slb,
    mlib_u8 *da,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_u8 **table);

/* *********************************************************** */

    void mlib_s_ImageLookUp_S16_S16_1(
    const mlib_s16 *sa,
    mlib_s32 slb,
    mlib_s16 *da,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_s16 **table);

    void mlib_s_ImageLookUp_S16_S16_2(
    const mlib_s16 *sa,
    mlib_s32 slb,
    mlib_s16 *da,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_s16 **table);

    void mlib_s_ImageLookUp_S16_S16_3(
    const mlib_s16 *sa,
    mlib_s32 slb,
    mlib_s16 *da,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_s16 **table);

    void mlib_s_ImageLookUp_S16_S16_4(
    const mlib_s16 *sa,
    mlib_s32 slb,
    mlib_s16 *da,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_s16 **table);

/* *********************************************************** */

    void mlib_s_ImageLookUpSI_S16_S16_2(
    const mlib_s16 *sa,
    mlib_s32 slb,
    mlib_s16 *da,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_s16 **table);

    void mlib_s_ImageLookUpSI_S16_S16_3(
    const mlib_s16 *sa,
    mlib_s32 slb,
    mlib_s16 *da,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_s16 **table);

    void mlib_s_ImageLookUpSI_S16_S16_4(
    const mlib_s16 *sa,
    mlib_s32 slb,
    mlib_s16 *da,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_s16 **table);

/* *********************************************************** */

    void mlib_s_ImageLookUp_U16_S16_1(
    const mlib_u16 *sa,
    mlib_s32 slb,
    mlib_s16 *da,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_s16 **table);

    void mlib_s_ImageLookUp_U16_S16_2(
    const mlib_u16 *sa,
    mlib_s32 slb,
    mlib_s16 *da,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_s16 **table);

    void mlib_s_ImageLookUp_U16_S16_3(
    const mlib_u16 *sa,
    mlib_s32 slb,
    mlib_s16 *da,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_s16 **table);

    void mlib_s_ImageLookUp_U16_S16_4(
    const mlib_u16 *sa,
    mlib_s32 slb,
    mlib_s16 *da,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_s16 **table);

/* *********************************************************** */

    void mlib_s_ImageLookUpSI_U16_S16_2(
    const mlib_u16 *sa,
    mlib_s32 slb,
    mlib_s16 *da,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_s16 **table);

    void mlib_s_ImageLookUpSI_U16_S16_3(
    const mlib_u16 *sa,
    mlib_s32 slb,
    mlib_s16 *da,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_s16 **table);

    void mlib_s_ImageLookUpSI_U16_S16_4(
    const mlib_u16 *sa,
    mlib_s32 slb,
    mlib_s16 *da,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_s16 **table);

/* *********************************************************** */

    void mlib_s_ImageLookUp_S16_S32_1(
    const mlib_s16 *sa,
    mlib_s32 slb,
    mlib_s32 *da,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_s32 **table);

    void mlib_s_ImageLookUp_S16_S32_2(
    const mlib_s16 *sa,
    mlib_s32 slb,
    mlib_s32 *da,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_s32 **table);

    void mlib_s_ImageLookUp_S16_S32_3(
    const mlib_s16 *sa,
    mlib_s32 slb,
    mlib_s32 *da,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_s32 **table);

    void mlib_s_ImageLookUp_S16_S32_4(
    const mlib_s16 *sa,
    mlib_s32 slb,
    mlib_s32 *da,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_s32 **table);

/* *********************************************************** */

    void mlib_s_ImageLookUpSI_S16_S32_2(
    const mlib_s16 *sa,
    mlib_s32 slb,
    mlib_s32 *da,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_s32 **table);

    void mlib_s_ImageLookUpSI_S16_S32_3(
    const mlib_s16 *sa,
    mlib_s32 slb,
    mlib_s32 *da,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_s32 **table);

    void mlib_s_ImageLookUpSI_S16_S32_4(
    const mlib_s16 *sa,
    mlib_s32 slb,
    mlib_s32 *da,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_s32 **table);

/* *********************************************************** */

    void mlib_s_ImageLookUp_U16_S32_1(
    const mlib_u16 *sa,
    mlib_s32 slb,
    mlib_s32 *da,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_s32 **table);

    void mlib_s_ImageLookUp_U16_S32_2(
    const mlib_u16 *sa,
    mlib_s32 slb,
    mlib_s32 *da,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_s32 **table);

    void mlib_s_ImageLookUp_U16_S32_3(
    const mlib_u16 *sa,
    mlib_s32 slb,
    mlib_s32 *da,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_s32 **table);

    void mlib_s_ImageLookUp_U16_S32_4(
    const mlib_u16 *sa,
    mlib_s32 slb,
    mlib_s32 *da,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_s32 **table);

/* *********************************************************** */

    void mlib_s_ImageLookUpSI_U16_S32_2(
    const mlib_u16 *sa,
    mlib_s32 slb,
    mlib_s32 *da,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_s32 **table);

    void mlib_s_ImageLookUpSI_U16_S32_3(
    const mlib_u16 *sa,
    mlib_s32 slb,
    mlib_s32 *da,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_s32 **table);

    void mlib_s_ImageLookUpSI_U16_S32_4(
    const mlib_u16 *sa,
    mlib_s32 slb,
    mlib_s32 *da,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_s32 **table);

/* *********************************************************** */

    void mlib_s_ImageLookUp_S32_U8_1(
    const mlib_s32 *sa,
    mlib_s32 slb,
    mlib_u8 *da,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_u8 **table);

    void mlib_s_ImageLookUp_S32_U8_2(
    const mlib_s32 *sa,
    mlib_s32 slb,
    mlib_u8 *da,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_u8 **table);

    void mlib_s_ImageLookUp_S32_U8_3(
    const mlib_s32 *sa,
    mlib_s32 slb,
    mlib_u8 *da,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_u8 **table);

    void mlib_s_ImageLookUp_S32_U8_4(
    const mlib_s32 *sa,
    mlib_s32 slb,
    mlib_u8 *da,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_u8 **table);

/* *********************************************************** */

    void mlib_s_ImageLookUpSI_S32_U8_2(
    const mlib_s32 *sa,
    mlib_s32 slb,
    mlib_u8 *da,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_u8 **table);

    void mlib_s_ImageLookUpSI_S32_U8_3(
    const mlib_s32 *sa,
    mlib_s32 slb,
    mlib_u8 *da,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_u8 **table);

    void mlib_s_ImageLookUpSI_S32_U8_4(
    const mlib_s32 *sa,
    mlib_s32 slb,
    mlib_u8 *da,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_u8 **table);

/* *********************************************************** */

    void mlib_s_ImageLookUp_S32_S16_1(
    const mlib_s32 *sa,
    mlib_s32 slb,
    mlib_s16 *da,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_s16 **table);

    void mlib_s_ImageLookUp_S32_S16_2(
    const mlib_s32 *sa,
    mlib_s32 slb,
    mlib_s16 *da,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_s16 **table);

    void mlib_s_ImageLookUp_S32_S16_3(
    const mlib_s32 *sa,
    mlib_s32 slb,
    mlib_s16 *da,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_s16 **table);

    void mlib_s_ImageLookUp_S32_S16_4(
    const mlib_s32 *sa,
    mlib_s32 slb,
    mlib_s16 *da,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_s16 **table);

/* *********************************************************** */

    void mlib_s_ImageLookUpSI_S32_S16_2(
    const mlib_s32 *sa,
    mlib_s32 slb,
    mlib_s16 *da,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_s16 **table);

    void mlib_s_ImageLookUpSI_S32_S16_3(
    const mlib_s32 *sa,
    mlib_s32 slb,
    mlib_s16 *da,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_s16 **table);

    void mlib_s_ImageLookUpSI_S32_S16_4(
    const mlib_s32 *sa,
    mlib_s32 slb,
    mlib_s16 *da,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_s16 **table);

/* *********************************************************** */

    void mlib_s_ImageLookUp_S32_S32_1(
    const mlib_s32 *sa,
    mlib_s32 slb,
    mlib_s32 *da,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_s32 **table);

    void mlib_s_ImageLookUp_S32_S32_2(
    const mlib_s32 *sa,
    mlib_s32 slb,
    mlib_s32 *da,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_s32 **table);

    void mlib_s_ImageLookUp_S32_S32_3(
    const mlib_s32 *sa,
    mlib_s32 slb,
    mlib_s32 *da,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_s32 **table);

    void mlib_s_ImageLookUp_S32_S32_4(
    const mlib_s32 *sa,
    mlib_s32 slb,
    mlib_s32 *da,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_s32 **table);

/* *********************************************************** */

    void mlib_s_ImageLookUpSI_S32_S32_2(
    const mlib_s32 *sa,
    mlib_s32 slb,
    mlib_s32 *da,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_s32 **table);

    void mlib_s_ImageLookUpSI_S32_S32_3(
    const mlib_s32 *sa,
    mlib_s32 slb,
    mlib_s32 *da,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_s32 **table);

    void mlib_s_ImageLookUpSI_S32_S32_4(
    const mlib_s32 *sa,
    mlib_s32 slb,
    mlib_s32 *da,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_s32 **table);

/* *********************************************************** */

mlib_status
__mlib_ImageLookUp(
    mlib_image *dst,
    const mlib_image *src,
    const void **table)
{
	mlib_s32 slb, dlb, xsize, ysize, nchan, ichan, bitoff_src;
	mlib_type stype, dtype;
	void *sa, *da;

	MLIB_IMAGE_CHECK(src);
	MLIB_IMAGE_CHECK(dst);
	MLIB_IMAGE_SIZE_EQUAL(src, dst);
	MLIB_IMAGE_CHAN_SRC1_OR_EQ(src, dst);

	stype = mlib_ImageGetType(src);
	dtype = mlib_ImageGetType(dst);
	ichan = mlib_ImageGetChannels(src);
	nchan = mlib_ImageGetChannels(dst);
	xsize = mlib_ImageGetWidth(src);
	ysize = mlib_ImageGetHeight(src);
	slb = mlib_ImageGetStride(src);
	dlb = mlib_ImageGetStride(dst);
	sa = mlib_ImageGetData(src);
	da = mlib_ImageGetData(dst);

	if (ichan == nchan) {
		if (dtype == MLIB_BYTE) {
			if (stype == MLIB_BYTE) {
				switch (nchan) {
				case 1:
					mlib_s_ImageLookUp_U8_U8_1((const
					    mlib_u8 *)sa, slb, (mlib_u8 *)da,
					    dlb, xsize, ysize,
					    (const mlib_u8 **)table);
					break;
				case 2:
					mlib_s_ImageLookUp_U8_U8_2((const
					    mlib_u8 *)sa, slb, (mlib_u8 *)da,
					    dlb, xsize, ysize,
					    (const mlib_u8 **)table);
					break;
				case 3:
					mlib_s_ImageLookUp_U8_U8_3((const
					    mlib_u8 *)sa, slb, (mlib_u8 *)da,
					    dlb, xsize, ysize,
					    (const mlib_u8 **)table);
					break;
				case 4:
					mlib_s_ImageLookUp_U8_U8_4((const
					    mlib_u8 *)sa, slb, (mlib_u8 *)da,
					    dlb, xsize, ysize,
					    (const mlib_u8 **)table);
					break;
				default:
					return (MLIB_FAILURE);
				}
				return (MLIB_SUCCESS);

			} else if (stype == MLIB_SHORT) {
				switch (nchan) {
				case 1:
					mlib_s_ImageLookUp_S16_U8_1((const
					    mlib_s16 *)sa, slb, (mlib_u8 *)da,
					    dlb, xsize, ysize,
					    (const mlib_u8 **)table);
					break;
				case 2:
					mlib_s_ImageLookUp_S16_U8_2((const
					    mlib_s16 *)sa, slb, (mlib_u8 *)da,
					    dlb, xsize, ysize,
					    (const mlib_u8 **)table);
					break;
				case 3:
					mlib_s_ImageLookUp_S16_U8_3((const
					    mlib_s16 *)sa, slb, (mlib_u8 *)da,
					    dlb, xsize, ysize,
					    (const mlib_u8 **)table);
					break;
				case 4:
					mlib_s_ImageLookUp_S16_U8_4((const
					    mlib_s16 *)sa, slb, (mlib_u8 *)da,
					    dlb, xsize, ysize,
					    (const mlib_u8 **)table);
					break;
				default:
					return (MLIB_FAILURE);
				}
				return (MLIB_SUCCESS);
			} else if (stype == MLIB_INT) {
				switch (nchan) {
				case 1:
					mlib_s_ImageLookUp_S32_U8_1((const
					    mlib_s32 *)sa, slb, (mlib_u8 *)da,
					    dlb, xsize, ysize,
					    (const mlib_u8 **)table);
					break;
				case 2:
					mlib_s_ImageLookUp_S32_U8_2((const
					    mlib_s32 *)sa, slb, (mlib_u8 *)da,
					    dlb, xsize, ysize,
					    (const mlib_u8 **)table);
					break;
				case 3:
					mlib_s_ImageLookUp_S32_U8_3((const
					    mlib_s32 *)sa, slb, (mlib_u8 *)da,
					    dlb, xsize, ysize,
					    (const mlib_u8 **)table);
					break;
				case 4:
					mlib_s_ImageLookUp_S32_U8_4((const
					    mlib_s32 *)sa, slb, (mlib_u8 *)da,
					    dlb, xsize, ysize,
					    (const mlib_u8 **)table);
					break;
				default:
					return (MLIB_FAILURE);
				}
				return (MLIB_SUCCESS);

			} else if (stype == MLIB_BIT) {
				if (nchan != 1)
					return (MLIB_FAILURE);
/* bits to first byte */
				bitoff_src = mlib_ImageGetBitOffset(src);
				return mlib_ImageLookUp_Bit_U8_1((const mlib_u8
				    *)sa, slb, (mlib_u8 *)da, dlb, xsize, ysize,
				    nchan, bitoff_src, (const mlib_u8 **)table);
			} else if (stype == MLIB_USHORT) {
				switch (nchan) {
				case 1:
					mlib_s_ImageLookUp_U16_U8_1((const
					    mlib_u16 *)sa, slb, (mlib_u8 *)da,
					    dlb, xsize, ysize,
					    (const mlib_u8 **)table);
					break;
				case 2:
					mlib_s_ImageLookUp_U16_U8_2((const
					    mlib_u16 *)sa, slb, (mlib_u8 *)da,
					    dlb, xsize, ysize,
					    (const mlib_u8 **)table);
					break;
				case 3:
					mlib_s_ImageLookUp_U16_U8_3((const
					    mlib_u16 *)sa, slb, (mlib_u8 *)da,
					    dlb, xsize, ysize,
					    (const mlib_u8 **)table);
					break;
				case 4:
					mlib_s_ImageLookUp_U16_U8_4((const
					    mlib_u16 *)sa, slb, (mlib_u8 *)da,
					    dlb, xsize, ysize,
					    (const mlib_u8 **)table);
					break;
				default:
					return (MLIB_FAILURE);
				}
				return (MLIB_SUCCESS);
			}
		} else if (dtype == MLIB_SHORT || dtype == MLIB_USHORT) {
			if (stype == MLIB_BYTE) {
				switch (nchan) {
				case 1:
					mlib_s_ImageLookUp_U8_S16_1((const
					    mlib_u8 *)sa, slb, (mlib_s16 *)da,
					    dlb, xsize, ysize,
					    (const mlib_s16 **)table);
					break;
				case 2:
					mlib_s_ImageLookUp_U8_S16_2((const
					    mlib_u8 *)sa, slb, (mlib_s16 *)da,
					    dlb, xsize, ysize,
					    (const mlib_s16 **)table);
					break;
				case 3:
					mlib_s_ImageLookUp_U8_S16_3((const
					    mlib_u8 *)sa, slb, (mlib_s16 *)da,
					    dlb, xsize, ysize,
					    (const mlib_s16 **)table);
					break;
				case 4:
					mlib_s_ImageLookUp_U8_S16_4((const
					    mlib_u8 *)sa, slb, (mlib_s16 *)da,
					    dlb, xsize, ysize,
					    (const mlib_s16 **)table);
					break;
				default:
					return (MLIB_FAILURE);
				}
				return (MLIB_SUCCESS);
			} else if (stype == MLIB_SHORT) {
				switch (nchan) {
				case 1:
					mlib_s_ImageLookUp_S16_S16_1((const
					    mlib_s16 *)sa, slb, (mlib_s16 *)da,
					    dlb, xsize, ysize,
					    (const mlib_s16 **)table);
					break;
				case 2:
					mlib_s_ImageLookUp_S16_S16_2((const
					    mlib_s16 *)sa, slb, (mlib_s16 *)da,
					    dlb, xsize, ysize,
					    (const mlib_s16 **)table);
					break;
				case 3:
					mlib_s_ImageLookUp_S16_S16_3((const
					    mlib_s16 *)sa, slb, (mlib_s16 *)da,
					    dlb, xsize, ysize,
					    (const mlib_s16 **)table);
					break;
				case 4:
					mlib_s_ImageLookUp_S16_S16_4((const
					    mlib_s16 *)sa, slb, (mlib_s16 *)da,
					    dlb, xsize, ysize,
					    (const mlib_s16 **)table);
					break;
				default:
					return (MLIB_FAILURE);
				}
				return (MLIB_SUCCESS);
			} else if (stype == MLIB_USHORT) {
				switch (nchan) {
				case 1:
					mlib_s_ImageLookUp_U16_S16_1((const
					    mlib_u16 *)sa, slb, (mlib_s16 *)da,
					    dlb, xsize, ysize,
					    (const mlib_s16 **)table);
					break;
				case 2:
					mlib_s_ImageLookUp_U16_S16_2((const
					    mlib_u16 *)sa, slb, (mlib_s16 *)da,
					    dlb, xsize, ysize,
					    (const mlib_s16 **)table);
					break;
				case 3:
					mlib_s_ImageLookUp_U16_S16_3((const
					    mlib_u16 *)sa, slb, (mlib_s16 *)da,
					    dlb, xsize, ysize,
					    (const mlib_s16 **)table);
					break;
				case 4:
					mlib_s_ImageLookUp_U16_S16_4((const
					    mlib_u16 *)sa, slb, (mlib_s16 *)da,
					    dlb, xsize, ysize,
					    (const mlib_s16 **)table);
					break;
				default:
					return (MLIB_FAILURE);
				}
				return (MLIB_SUCCESS);
			} else if (stype == MLIB_INT) {
				switch (nchan) {
				case 1:
					mlib_s_ImageLookUp_S32_S16_1((const
					    mlib_s32 *)sa, slb, (mlib_s16 *)da,
					    dlb, xsize, ysize,
					    (const mlib_s16 **)table);
					break;
				case 2:
					mlib_s_ImageLookUp_S32_S16_2((const
					    mlib_s32 *)sa, slb, (mlib_s16 *)da,
					    dlb, xsize, ysize,
					    (const mlib_s16 **)table);
					break;
				case 3:
					mlib_s_ImageLookUp_S32_S16_3((const
					    mlib_s32 *)sa, slb, (mlib_s16 *)da,
					    dlb, xsize, ysize,
					    (const mlib_s16 **)table);
					break;
				case 4:
					mlib_s_ImageLookUp_S32_S16_4((const
					    mlib_s32 *)sa, slb, (mlib_s16 *)da,
					    dlb, xsize, ysize,
					    (const mlib_s16 **)table);
					break;
				default:
					return (MLIB_FAILURE);
				}
				return (MLIB_SUCCESS);
			}
		} else if (dtype == MLIB_INT || dtype == MLIB_FLOAT) {
			if (stype == MLIB_BYTE) {
				switch (nchan) {
				case 1:
					mlib_s_ImageLookUp_U8_S32_1((const
					    mlib_u8 *)sa, slb, (mlib_s32 *)da,
					    dlb, xsize, ysize,
					    (const mlib_s32 **)table);
					break;
				case 2:
					mlib_s_ImageLookUp_U8_S32_2((const
					    mlib_u8 *)sa, slb, (mlib_s32 *)da,
					    dlb, xsize, ysize,
					    (const mlib_s32 **)table);
					break;
				case 3:
					mlib_s_ImageLookUp_U8_S32_3((const
					    mlib_u8 *)sa, slb, (mlib_s32 *)da,
					    dlb, xsize, ysize,
					    (const mlib_s32 **)table);
					break;
				case 4:
					mlib_s_ImageLookUp_U8_S32_4((const
					    mlib_u8 *)sa, slb, (mlib_s32 *)da,
					    dlb, xsize, ysize,
					    (const mlib_s32 **)table);
					break;
				default:
					return (MLIB_FAILURE);
				}
				return (MLIB_SUCCESS);
			} else if (stype == MLIB_SHORT) {
				switch (nchan) {
				case 1:
					mlib_s_ImageLookUp_S16_S32_1((const
					    mlib_s16 *)sa, slb, (mlib_s32 *)da,
					    dlb, xsize, ysize,
					    (const mlib_s32 **)table);
					break;
				case 2:
					mlib_s_ImageLookUp_S16_S32_2((const
					    mlib_s16 *)sa, slb, (mlib_s32 *)da,
					    dlb, xsize, ysize,
					    (const mlib_s32 **)table);
					break;
				case 3:
					mlib_s_ImageLookUp_S16_S32_3((const
					    mlib_s16 *)sa, slb, (mlib_s32 *)da,
					    dlb, xsize, ysize,
					    (const mlib_s32 **)table);
					break;
				case 4:
					mlib_s_ImageLookUp_S16_S32_4((const
					    mlib_s16 *)sa, slb, (mlib_s32 *)da,
					    dlb, xsize, ysize,
					    (const mlib_s32 **)table);
					break;
				default:
					return (MLIB_FAILURE);
				}
				return (MLIB_SUCCESS);
			} else if (stype == MLIB_USHORT) {
				switch (nchan) {
				case 1:
					mlib_s_ImageLookUp_U16_S32_1((const
					    mlib_u16 *)sa, slb, (mlib_s32 *)da,
					    dlb, xsize, ysize,
					    (const mlib_s32 **)table);
					break;
				case 2:
					mlib_s_ImageLookUp_U16_S32_2((const
					    mlib_u16 *)sa, slb, (mlib_s32 *)da,
					    dlb, xsize, ysize,
					    (const mlib_s32 **)table);
					break;
				case 3:
					mlib_s_ImageLookUp_U16_S32_3((const
					    mlib_u16 *)sa, slb, (mlib_s32 *)da,
					    dlb, xsize, ysize,
					    (const mlib_s32 **)table);
					break;
				case 4:
					mlib_s_ImageLookUp_U16_S32_4((const
					    mlib_u16 *)sa, slb, (mlib_s32 *)da,
					    dlb, xsize, ysize,
					    (const mlib_s32 **)table);
					break;
				default:
					return (MLIB_FAILURE);
				}
				return (MLIB_SUCCESS);
			} else if (stype == MLIB_INT) {
				switch (nchan) {
				case 1:
					mlib_s_ImageLookUp_S32_S32_1((const
					    mlib_s32 *)sa, slb, (mlib_s32 *)da,
					    dlb, xsize, ysize,
					    (const mlib_s32 **)table);
					break;
				case 2:
					mlib_s_ImageLookUp_S32_S32_2((const
					    mlib_s32 *)sa, slb, (mlib_s32 *)da,
					    dlb, xsize, ysize,
					    (const mlib_s32 **)table);
					break;
				case 3:
					mlib_s_ImageLookUp_S32_S32_3((const
					    mlib_s32 *)sa, slb, (mlib_s32 *)da,
					    dlb, xsize, ysize,
					    (const mlib_s32 **)table);
					break;
				case 4:
					mlib_s_ImageLookUp_S32_S32_4((const
					    mlib_s32 *)sa, slb, (mlib_s32 *)da,
					    dlb, xsize, ysize,
					    (const mlib_s32 **)table);
					break;
				default:
					return (MLIB_FAILURE);
				}
				return (MLIB_SUCCESS);
			}
		} else if (dtype == MLIB_DOUBLE) {
			if (stype == MLIB_BYTE) {
				mlib_ImageLookUp_U8_D64((const mlib_u8 *)sa,
				    slb, (mlib_d64 *)da, dlb / 8, xsize, ysize,
				    nchan, (const mlib_d64 **)table);
				return (MLIB_SUCCESS);
			} else if (stype == MLIB_SHORT) {
				mlib_ImageLookUp_S16_D64((const mlib_s16 *)sa,
				    slb / 2, (mlib_d64 *)da, dlb / 8, xsize,
				    ysize, nchan, (const mlib_d64 **)table);
				return (MLIB_SUCCESS);
			} else if (stype == MLIB_USHORT) {
				mlib_ImageLookUp_U16_D64((const mlib_u16 *)sa,
				    slb / 2, (mlib_d64 *)da, dlb / 8, xsize,
				    ysize, nchan, (const mlib_d64 **)table);
				return (MLIB_SUCCESS);
			} else if (stype == MLIB_INT) {
				mlib_ImageLookUp_S32_D64((const mlib_s32 *)sa,
				    slb / 4, (mlib_d64 *)da, dlb / 8, xsize,
				    ysize, nchan, (const mlib_d64 **)table);
				return (MLIB_SUCCESS);
			}
		}
	} else if (ichan == 1) {
		if (dtype == MLIB_BYTE) {

			if (stype == MLIB_BYTE) {
				mlib_s32 i;
				mlib_u8 *tab0, *tab1, *tab2, *tab3;
				mlib_u8 *stbl =
				    (mlib_u8 *)(__mlib_malloc(256 * 4 *
				    sizeof (mlib_u8)));
				switch (nchan) {
				case 2:
					tab0 = (mlib_u8 *)table[0];
					tab1 = (mlib_u8 *)table[1];
					for (i = 0; i < 256; i++) {
						stbl[2 * i + 0] = tab0[i];
						stbl[2 * i + 1] = tab1[i];
						((mlib_u16 *)stbl)[i] =
						    tab0[i] |
						    ((mlib_u16)tab1[i] << 8);
					}
					mlib_s_ImageLookUpSI_U8_U8_2((const
					    mlib_u8 *)sa, slb, (mlib_u8 *)da,
					    dlb, xsize, ysize,
					    (const mlib_u8 *)stbl);
					break;
				case 3:
					tab0 = (mlib_u8 *)table[0];
					tab1 = (mlib_u8 *)table[1];
					tab2 = (mlib_u8 *)table[2];
					for (i = 0; i < 256; i++) {
						((mlib_u32 *)stbl)[i] =
						    tab0[i] |
						    (tab1[i] << 8) |
						    ((mlib_u32)tab2[i] << 16);
					}
					mlib_s_ImageLookUpSI_U8_U8_3((const
					    mlib_u8 *)sa, slb, (mlib_u8 *)da,
					    dlb, xsize, ysize,
					    (const mlib_u8 **)table,
					    (const mlib_u8 *)stbl);
					break;
				case 4:
					tab0 = (mlib_u8 *)table[0];
					tab1 = (mlib_u8 *)table[1];
					tab2 = (mlib_u8 *)table[2];
					tab3 = (mlib_u8 *)table[3];
					for (i = 0; i < 256; i++) {
						((mlib_u32 *)stbl)[i] =
						    tab0[i] | (tab1[i] << 8) |
						    (tab2[i] << 16) |
						    ((mlib_u32)tab3[i] << 24);
					}
					mlib_s_ImageLookUpSI_U8_U8_4((const
					    mlib_u8 *)sa, slb, (mlib_u8 *)da,
					    dlb, xsize, ysize,
					    (const mlib_u8 *)stbl);
					break;
				default:
					__mlib_free(stbl);
					return (MLIB_FAILURE);
				}
				__mlib_free(stbl);
				return (MLIB_SUCCESS);
			} else if (stype == MLIB_SHORT) {
				switch (nchan) {
				case 2:
					mlib_s_ImageLookUpSI_S16_U8_2((const
					    mlib_s16 *)sa, slb, (mlib_u8 *)da,
					    dlb, xsize, ysize,
					    (const mlib_u8 **)table);
					break;
				case 3:
					mlib_s_ImageLookUpSI_S16_U8_3((const
					    mlib_s16 *)sa, slb, (mlib_u8 *)da,
					    dlb, xsize, ysize,
					    (const mlib_u8 **)table);
					break;
				case 4:
					mlib_s_ImageLookUpSI_S16_U8_4((const
					    mlib_s16 *)sa, slb, (mlib_u8 *)da,
					    dlb, xsize, ysize,
					    (const mlib_u8 **)table);
					break;
				default:
					return (MLIB_FAILURE);
				}
				return (MLIB_SUCCESS);
			} else if (stype == MLIB_USHORT) {
				switch (nchan) {
				case 2:
					mlib_s_ImageLookUpSI_U16_U8_2((const
					    mlib_u16 *)sa, slb, (mlib_u8 *)da,
					    dlb, xsize, ysize,
					    (const mlib_u8 **)table);
					break;
				case 3:
					mlib_s_ImageLookUpSI_U16_U8_3((const
					    mlib_u16 *)sa, slb, (mlib_u8 *)da,
					    dlb, xsize, ysize,
					    (const mlib_u8 **)table);
					break;
				case 4:
					mlib_s_ImageLookUpSI_U16_U8_4((const
					    mlib_u16 *)sa, slb, (mlib_u8 *)da,
					    dlb, xsize, ysize,
					    (const mlib_u8 **)table);
					break;
				default:
					return (MLIB_FAILURE);
				}
				return (MLIB_SUCCESS);
			} else if (stype == MLIB_INT) {
				switch (nchan) {
				case 2:
					mlib_s_ImageLookUpSI_S32_U8_2((const
					    mlib_s32 *)sa, slb, (mlib_u8 *)da,
					    dlb, xsize, ysize,
					    (const mlib_u8 **)table);
					break;
				case 3:
					mlib_s_ImageLookUpSI_S32_U8_3((const
					    mlib_s32 *)sa, slb, (mlib_u8 *)da,
					    dlb, xsize, ysize,
					    (const mlib_u8 **)table);
					break;
				case 4:
					mlib_s_ImageLookUpSI_S32_U8_4((const
					    mlib_s32 *)sa, slb, (mlib_u8 *)da,
					    dlb, xsize, ysize,
					    (const mlib_u8 **)table);
					break;
				default:
					return (MLIB_FAILURE);
				}
				return (MLIB_SUCCESS);
			} else if (stype == MLIB_BIT) {
/* bits to first byte */
				bitoff_src = mlib_ImageGetBitOffset(src);

				if (nchan == 2) {
					return mlib_ImageLookUp_Bit_U8_2((const
					    mlib_u8 *)sa, slb, (mlib_u8 *)da,
					    dlb, xsize, ysize, nchan,
					    bitoff_src,
					    (const mlib_u8 **)table);
				} else if (nchan == 3) {
					return mlib_ImageLookUp_Bit_U8_3((const
					    mlib_u8 *)sa, slb, (mlib_u8 *)da,
					    dlb, xsize, ysize, nchan,
					    bitoff_src,
					    (const mlib_u8 **)table);
				} else {
/* (nchan == 4) */
					return mlib_ImageLookUp_Bit_U8_4((const
					    mlib_u8 *)sa, slb, (mlib_u8 *)da,
					    dlb, xsize, ysize, nchan,
					    bitoff_src,
					    (const mlib_u8 **)table);
				}
			}
		} else if (dtype == MLIB_SHORT || dtype == MLIB_USHORT) {
			if (stype == MLIB_BYTE) {
				mlib_s32 i;
				mlib_s16 *stbl =
				    (mlib_s16 *)(__mlib_malloc(256 * 4 *
				    sizeof (mlib_s16)));
				mlib_s16 *tab0, *tab1, *tab2, *tab3;

				switch (nchan) {
				case 2:
					tab0 = (mlib_s16 *)table[0];
					tab1 = (mlib_s16 *)table[1];
					for (i = 0; i < 256; i++) {
						stbl[2 * i + 0] = tab0[i];
						stbl[2 * i + 1] = tab1[i];
					}
					mlib_s_ImageLookUpSI_U8_S16_2((const
					    mlib_u8 *)sa, slb, (mlib_s16 *)da,
					    dlb, xsize, ysize,
					    (const mlib_s16 *)stbl);
					break;
				case 3:
					tab0 = (mlib_s16 *)table[0];
					tab1 = (mlib_s16 *)table[1];
					tab2 = (mlib_s16 *)table[2];
					for (i = 0; i < 256; i++) {
						stbl[4 * i + 0] = tab0[i];
						stbl[4 * i + 1] = tab1[i];
						stbl[4 * i + 2] = tab2[i];
						stbl[4 * i + 3] = 0;
					}
					mlib_s_ImageLookUpSI_U8_S16_3((const
					    mlib_u8 *)sa, slb, (mlib_s16 *)da,
					    dlb, xsize, ysize,
					    (const mlib_s16 **)table);
					break;
				case 4:
					tab0 = (mlib_s16 *)table[0];
					tab1 = (mlib_s16 *)table[1];
					tab2 = (mlib_s16 *)table[2];
					tab3 = (mlib_s16 *)table[3];
					for (i = 0; i < 256; i++) {
						stbl[4 * i + 0] = tab0[i];
						stbl[4 * i + 1] = tab1[i];
						stbl[4 * i + 2] = tab2[i];
						stbl[4 * i + 3] = tab3[i];
					}
					mlib_s_ImageLookUpSI_U8_S16_4((const
					    mlib_u8 *)sa, slb, (mlib_s16 *)da,
					    dlb, xsize, ysize,
					    (const mlib_s16 *)stbl);
					break;
				default:
					__mlib_free(stbl);
					return (MLIB_FAILURE);
				}
				__mlib_free(stbl);
				return (MLIB_SUCCESS);
			} else if (stype == MLIB_SHORT) {
				switch (nchan) {
				case 2:
					mlib_s_ImageLookUpSI_S16_S16_2((const
					    mlib_s16 *)sa, slb, (mlib_s16 *)da,
					    dlb, xsize, ysize,
					    (const mlib_s16 **)table);
					break;
				case 3:
					mlib_s_ImageLookUpSI_S16_S16_3((const
					    mlib_s16 *)sa, slb, (mlib_s16 *)da,
					    dlb, xsize, ysize,
					    (const mlib_s16 **)table);
					break;
				case 4:
					mlib_s_ImageLookUpSI_S16_S16_4((const
					    mlib_s16 *)sa, slb, (mlib_s16 *)da,
					    dlb, xsize, ysize,
					    (const mlib_s16 **)table);
					break;
				default:
					return (MLIB_FAILURE);
				}
				return (MLIB_SUCCESS);
			} else if (stype == MLIB_USHORT) {
				switch (nchan) {
				case 2:
					mlib_s_ImageLookUpSI_U16_S16_2((const
					    mlib_u16 *)sa, slb, (mlib_s16 *)da,
					    dlb, xsize, ysize,
					    (const mlib_s16 **)table);
					break;
				case 3:
					mlib_s_ImageLookUpSI_U16_S16_3((const
					    mlib_u16 *)sa, slb, (mlib_s16 *)da,
					    dlb, xsize, ysize,
					    (const mlib_s16 **)table);
					break;
				case 4:
					mlib_s_ImageLookUpSI_U16_S16_4((const
					    mlib_u16 *)sa, slb, (mlib_s16 *)da,
					    dlb, xsize, ysize,
					    (const mlib_s16 **)table);
					break;
				default:
					return (MLIB_FAILURE);
				}
				return (MLIB_SUCCESS);
			} else if (stype == MLIB_INT) {
				switch (nchan) {
				case 2:
					mlib_s_ImageLookUpSI_S32_S16_2((const
					    mlib_s32 *)sa, slb, (mlib_s16 *)da,
					    dlb, xsize, ysize,
					    (const mlib_s16 **)table);
					break;
				case 3:
					mlib_s_ImageLookUpSI_S32_S16_3((const
					    mlib_s32 *)sa, slb, (mlib_s16 *)da,
					    dlb, xsize, ysize,
					    (const mlib_s16 **)table);
					break;
				case 4:
					mlib_s_ImageLookUpSI_S32_S16_4((const
					    mlib_s32 *)sa, slb, (mlib_s16 *)da,
					    dlb, xsize, ysize,
					    (const mlib_s16 **)table);
					break;
				default:
					return (MLIB_FAILURE);
				}
				return (MLIB_SUCCESS);
			}
		} else if (dtype == MLIB_INT || dtype == MLIB_FLOAT) {

			if (stype == MLIB_BYTE) {
				mlib_s32 i;
				mlib_s32 *tab0, *tab1, *tab2, *tab3;
				mlib_s32 *stbl =
				    (mlib_s32 *)(__mlib_malloc(256 * 4 *
				    sizeof (mlib_s32)));
				switch (nchan) {
				case 2:
					tab0 = (mlib_s32 *)table[0];
					tab1 = (mlib_s32 *)table[1];
					for (i = 0; i < 256; i++) {
						stbl[2 * i + 0] = tab0[i];
						stbl[2 * i + 1] = tab1[i];
					}
					mlib_s_ImageLookUpSI_U8_S32_2((const
					    mlib_u8 *)sa, slb, (mlib_s32 *)da,
					    dlb, xsize, ysize,
					    (const mlib_s32 *)stbl);
					break;
				case 3:
					tab0 = (mlib_s32 *)table[0];
					tab1 = (mlib_s32 *)table[1];
					tab2 = (mlib_s32 *)table[2];
					for (i = 0; i < 256; i++) {
						stbl[4 * i + 0] = tab0[i];
						stbl[4 * i + 1] = tab1[i];
						stbl[4 * i + 2] = tab2[i];
						stbl[4 * i + 3] = 0;
					}
					mlib_s_ImageLookUpSI_U8_S32_3((const
					    mlib_u8 *)sa, slb, (mlib_s32 *)da,
					    dlb, xsize, ysize,
					    (const mlib_s32 **)table);
					break;
				case 4:
					tab0 = (mlib_s32 *)table[0];
					tab1 = (mlib_s32 *)table[1];
					tab2 = (mlib_s32 *)table[2];
					tab3 = (mlib_s32 *)table[3];
					for (i = 0; i < 256; i++) {
						stbl[4 * i + 0] = tab0[i];
						stbl[4 * i + 1] = tab1[i];
						stbl[4 * i + 2] = tab2[i];
						stbl[4 * i + 3] = tab3[i];
					}
					mlib_s_ImageLookUpSI_U8_S32_4((const
					    mlib_u8 *)sa, slb, (mlib_s32 *)da,
					    dlb, xsize, ysize,
					    (const mlib_s32 *)stbl);
					break;
				default:
					__mlib_free(stbl);
					return (MLIB_FAILURE);
				}
				__mlib_free(stbl);
				return (MLIB_SUCCESS);
			} else if (stype == MLIB_SHORT) {
				switch (nchan) {
				case 2:
					mlib_s_ImageLookUpSI_S16_S32_2((const
					    mlib_s16 *)sa, slb, (mlib_s32 *)da,
					    dlb, xsize, ysize,
					    (const mlib_s32 **)table);
					break;
				case 3:
					mlib_s_ImageLookUpSI_S16_S32_3((const
					    mlib_s16 *)sa, slb, (mlib_s32 *)da,
					    dlb, xsize, ysize,
					    (const mlib_s32 **)table);
					break;
				case 4:
					mlib_s_ImageLookUpSI_S16_S32_4((const
					    mlib_s16 *)sa, slb, (mlib_s32 *)da,
					    dlb, xsize, ysize,
					    (const mlib_s32 **)table);
					break;
				default:
					return (MLIB_FAILURE);
				}
				return (MLIB_SUCCESS);
			} else if (stype == MLIB_USHORT) {
				switch (nchan) {
				case 2:
					mlib_s_ImageLookUpSI_U16_S32_2((const
					    mlib_u16 *)sa, slb, (mlib_s32 *)da,
					    dlb, xsize, ysize,
					    (const mlib_s32 **)table);
					break;
				case 3:
					mlib_s_ImageLookUpSI_U16_S32_3((const
					    mlib_u16 *)sa, slb, (mlib_s32 *)da,
					    dlb, xsize, ysize,
					    (const mlib_s32 **)table);
					break;
				case 4:
					mlib_s_ImageLookUpSI_U16_S32_4((const
					    mlib_u16 *)sa, slb, (mlib_s32 *)da,
					    dlb, xsize, ysize,
					    (const mlib_s32 **)table);
					break;
				default:
					return (MLIB_FAILURE);
				}
				return (MLIB_SUCCESS);
			} else if (stype == MLIB_INT) {
				switch (nchan) {
				case 2:
					mlib_s_ImageLookUpSI_S32_S32_2((const
					    mlib_s32 *)sa, slb, (mlib_s32 *)da,
					    dlb, xsize, ysize,
					    (const mlib_s32 **)table);
					break;
				case 3:
					mlib_s_ImageLookUpSI_S32_S32_3((const
					    mlib_s32 *)sa, slb, (mlib_s32 *)da,
					    dlb, xsize, ysize,
					    (const mlib_s32 **)table);
					break;
				case 4:
					mlib_s_ImageLookUpSI_S32_S32_4((const
					    mlib_s32 *)sa, slb, (mlib_s32 *)da,
					    dlb, xsize, ysize,
					    (const mlib_s32 **)table);
					break;
				default:
					return (MLIB_FAILURE);
				}
				return (MLIB_SUCCESS);
			}
		} else if (dtype == MLIB_DOUBLE) {
			if (stype == MLIB_BYTE) {
				mlib_ImageLookUpSI_U8_D64((const mlib_u8 *)sa,
				    slb, (mlib_d64 *)da, dlb / 8, xsize, ysize,
				    nchan, (const mlib_d64 **)table);
				return (MLIB_SUCCESS);
			} else if (stype == MLIB_SHORT) {
				mlib_ImageLookUpSI_S16_D64((const mlib_s16 *)sa,
				    slb / 2, (mlib_d64 *)da, dlb / 8, xsize,
				    ysize, nchan, (const mlib_d64 **)table);
				return (MLIB_SUCCESS);
			} else if (stype == MLIB_USHORT) {
				mlib_ImageLookUpSI_U16_D64((const mlib_u16 *)sa,
				    slb / 2, (mlib_d64 *)da, dlb / 8, xsize,
				    ysize, nchan, (const mlib_d64 **)table);
				return (MLIB_SUCCESS);
			} else if (stype == MLIB_INT) {
				mlib_ImageLookUpSI_S32_D64((const mlib_s32 *)sa,
				    slb / 4, (mlib_d64 *)da, dlb / 8, xsize,
				    ysize, nchan, (const mlib_d64 **)table);
				return (MLIB_SUCCESS);
			}
		}
	}
	return (MLIB_FAILURE);
}

/* *********************************************************** */
