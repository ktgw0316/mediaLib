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

#ifndef _MLIB_IMAGEDFT_H
#define	_MLIB_IMAGEDFT_H

#pragma ident	"@(#)mlib_ImageDFT.h	9.2	07/10/09 SMI"

#ifdef __cplusplus
extern "C" {
#endif

void mlib_ImageFFT_2FirstLay_col(
    mlib_d64 *pdst,
    mlib_d64 *psrc,
    mlib_s32 dorder,
    mlib_s32 ysize,
    mlib_s32 strided,
    mlib_d64 dsign);

void mlib_ImageFFT_3FirstLay_col(
    mlib_d64 *pdst,
    mlib_d64 *psrc,
    mlib_s32 dorder,
    mlib_s32 ysize,
    mlib_s32 strided,
    mlib_d64 dsign);

void mlib_Image_first0_blk(
    mlib_d64 *dp,
    mlib_d64 *sp,
    mlib_s32 order,
    mlib_s32 blk_order);

void mlib_Image_first0_blk_r(
    mlib_d64 *dp,
    mlib_d64 *sp,
    mlib_s32 order,
    mlib_s32 blk_order);

void mlib_Image_first1_blk(
    mlib_d64 *dp,
    mlib_d64 *sp,
    mlib_s32 order,
    mlib_s32 blk_order);

void mlib_Image_first1_blk_r(
    mlib_d64 *dp,
    mlib_d64 *sp,
    mlib_s32 order,
    mlib_s32 blk_order);

void mlib_Image_first1_blk_inp(
    mlib_d64 *dp,
    mlib_s32 order,
    mlib_s32 blk_order);

void mlib_Image_fftKernel_blk(
    void *dat,
    mlib_s32 ord_blk,
    mlib_s32 ord_arr,
    mlib_s32 isign);

void mlib_ImageDFT_copy_col(
    mlib_d64 *pdst,
    mlib_d64 *psrc,
    mlib_s32 dorder,
    mlib_s32 ysize,
    mlib_s32 strided,
    mlib_d64 dsign);

void mlib_ImageConvert_2_1_u8(
    void *pdst,
    mlib_s32 i,
    mlib_d64 *sp,
    mlib_s32 xsize,
    mlib_s32 dorder,
    mlib_s32 strided);

void mlib_ImageConvert_2_1_u16(
    void *pdst,
    mlib_s32 i,
    mlib_d64 *sp,
    mlib_s32 xsize,
    mlib_s32 dorder,
    mlib_s32 strided);

void mlib_ImageConvert_2_1_s16(
    void *pdst,
    mlib_s32 i,
    mlib_d64 *sp,
    mlib_s32 xsize,
    mlib_s32 dorder,
    mlib_s32 strided);

void mlib_ImageConvert_2_1_s32(
    void *pdst,
    mlib_s32 i,
    mlib_d64 *sp,
    mlib_s32 xsize,
    mlib_s32 dorder,
    mlib_s32 strided);

void mlib_ImageConvert_2_1_f32(
    void *pdst,
    mlib_s32 i,
    mlib_d64 *sp,
    mlib_s32 xsize,
    mlib_s32 dorder,
    mlib_s32 strided);

void mlib_ImageConvert_2_1_d64(
    void *pdst,
    mlib_s32 i,
    mlib_d64 *sp,
    mlib_s32 xsize,
    mlib_s32 dorder,
    mlib_s32 strided);

void mlib_ImageConvert_2_2_u8(
    void *pdst,
    mlib_s32 i,
    mlib_d64 *sp,
    mlib_s32 xsize,
    mlib_s32 dorder,
    mlib_s32 strided);

void mlib_ImageConvert_2_2_u16(
    void *pdst,
    mlib_s32 i,
    mlib_d64 *sp,
    mlib_s32 xsize,
    mlib_s32 dorder,
    mlib_s32 strided);

void mlib_ImageConvert_2_2_s16(
    void *pdst,
    mlib_s32 i,
    mlib_d64 *sp,
    mlib_s32 xsize,
    mlib_s32 dorder,
    mlib_s32 strided);

void mlib_ImageConvert_2_2_s32(
    void *pdst,
    mlib_s32 i,
    mlib_d64 *sp,
    mlib_s32 xsize,
    mlib_s32 dorder,
    mlib_s32 strided);

void mlib_ImageConvert_2_2_f32(
    void *pdst,
    mlib_s32 i,
    mlib_d64 *sp,
    mlib_s32 xsize,
    mlib_s32 dorder,
    mlib_s32 strided);

mlib_status mlib_USE_FTOI_CLAMPING();

mlib_s32
mlib_ImageFourier_BLK_ORDER();

#ifdef __cplusplus
}
#endif

#endif /* _MLIB_IMAGEDFT_H */
