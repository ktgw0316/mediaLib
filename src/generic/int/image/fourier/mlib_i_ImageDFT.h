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

#ifndef _MLIB_I_IMAGEDFT_H
#define	_MLIB_I_IMAGEDFT_H

#pragma ident	"@(#)mlib_i_ImageDFT.h	9.2	07/11/05 SMI"

/* ifdef _NO_LONGLONG, use stdc source codes, else use int source codes */
#ifdef _NO_LONGLONG

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

/* ifdef _NO_LONGLONG, use stdc source codes, else use int source codes */
#else

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

void mlib_ImageFFT_2FirstLay_col_i(
    mlib_s64 *pdst,
    mlib_s64 *psrc,
    mlib_s32 dorder,
    mlib_s32 ysize,
    mlib_s32 strided,
    mlib_s64 dsign);

void mlib_ImageFFT_3FirstLay_col(
    mlib_d64 *pdst,
    mlib_d64 *psrc,
    mlib_s32 dorder,
    mlib_s32 ysize,
    mlib_s32 strided,
    mlib_d64 dsign);

void mlib_ImageFFT_3FirstLay_col_i(
    mlib_s64 *pdst,
    mlib_s64 *psrc,
    mlib_s32 dorder,
    mlib_s32 ysize,
    mlib_s32 strided,
    mlib_s64 dsign);

void mlib_Image_first0_blk(
    mlib_d64 *dp,
    mlib_d64 *sp,
    mlib_s32 order,
    mlib_s32 blk_order);

void mlib_Image_first0_blk_i(
    mlib_s64 *dp,
    mlib_s64 *sp,
    mlib_s32 order,
    mlib_s32 blk_order,
    mlib_s64 scale_extra);

void mlib_Image_first0_blk_r(
    mlib_d64 *dp,
    mlib_d64 *sp,
    mlib_s32 order,
    mlib_s32 blk_order);

void mlib_Image_first0_blk_r_i(
    mlib_s64 *dp,
    mlib_s64 *sp,
    mlib_s32 order,
    mlib_s32 blk_order,
    mlib_s64 scale_extra);

void mlib_Image_first1_blk(
    mlib_d64 *dp,
    mlib_d64 *sp,
    mlib_s32 order,
    mlib_s32 blk_order);

void mlib_Image_first1_blk_i(
    mlib_s64 *dp,
    mlib_s64 *sp,
    mlib_s32 order,
    mlib_s32 blk_order,
    mlib_s64 scale_extra);

void mlib_Image_first1_blk_r(
    mlib_d64 *dp,
    mlib_d64 *sp,
    mlib_s32 order,
    mlib_s32 blk_order);

void mlib_Image_first1_blk_r_i(
    mlib_s64 *dp,
    mlib_s64 *sp,
    mlib_s32 order,
    mlib_s32 blk_order,
    mlib_s64 scale_extra);

void mlib_Image_first1_blk_inp(
    mlib_d64 *dp,
    mlib_s32 order,
    mlib_s32 blk_order);

void mlib_Image_first1_blk_inp_i(
    mlib_s64 *dp,
    mlib_s32 order,
    mlib_s32 blk_order);

void mlib_Image_fftKernel_blk(
    void *dat,
    mlib_s32 ord_blk,
    mlib_s32 ord_arr,
    mlib_s32 isign);

void mlib_Image_fftKernel_blk_i(
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

void mlib_ImageDFT_copy_col_i(
    mlib_s64 *pdst,
    mlib_s64 *psrc,
    mlib_s32 dorder,
    mlib_s32 ysize,
    mlib_s32 strided,
    mlib_s64 dsign);

void mlib_ImageConvert_2_1_u8(
    void *pdst,
    mlib_s32 i,
    mlib_d64 *sp,
    mlib_s32 xsize,
    mlib_s32 dorder,
    mlib_s32 strided);

void mlib_ImageConvert_2_1_u8_i(
    void *pdst,
    mlib_s32 i,
    mlib_s64 *sp,
    mlib_s32 xsize,
    mlib_s32 dorder,
    mlib_s32 strided,
    mlib_s32 scale_shift);

void mlib_ImageConvert_2_1_u16(
    void *pdst,
    mlib_s32 i,
    mlib_d64 *sp,
    mlib_s32 xsize,
    mlib_s32 dorder,
    mlib_s32 strided);

void mlib_ImageConvert_2_1_u16_i(
    void *pdst,
    mlib_s32 i,
    mlib_s64 *sp,
    mlib_s32 xsize,
    mlib_s32 dorder,
    mlib_s32 strided,
    mlib_s32 scale_shift);

void mlib_ImageConvert_2_1_s16(
    void *pdst,
    mlib_s32 i,
    mlib_d64 *sp,
    mlib_s32 xsize,
    mlib_s32 dorder,
    mlib_s32 strided);

void mlib_ImageConvert_2_1_s16_i(
    void *pdst,
    mlib_s32 i,
    mlib_s64 *sp,
    mlib_s32 xsize,
    mlib_s32 dorder,
    mlib_s32 strided,
    mlib_s32 scale_shift);

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

void mlib_ImageConvert_2_2_u8_i(
    void *pdst,
    mlib_s32 i,
    mlib_s64 *sp,
    mlib_s32 xsize,
    mlib_s32 dorder,
    mlib_s32 strided,
    mlib_s32 scale_shift);

void mlib_ImageConvert_2_2_u16(
    void *pdst,
    mlib_s32 i,
    mlib_d64 *sp,
    mlib_s32 xsize,
    mlib_s32 dorder,
    mlib_s32 strided);

void mlib_ImageConvert_2_2_u16_i(
    void *pdst,
    mlib_s32 i,
    mlib_s64 *sp,
    mlib_s32 xsize,
    mlib_s32 dorder,
    mlib_s32 strided,
    mlib_s32 scale_shift);

void mlib_ImageConvert_2_2_s16(
    void *pdst,
    mlib_s32 i,
    mlib_d64 *sp,
    mlib_s32 xsize,
    mlib_s32 dorder,
    mlib_s32 strided);

void mlib_ImageConvert_2_2_s16_i(
    void *pdst,
    mlib_s32 i,
    mlib_s64 *sp,
    mlib_s32 xsize,
    mlib_s32 dorder,
    mlib_s32 strided,
    mlib_s32 scale_shift);

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

/* ifdef _NO_LONGLONG, use stdc source codes, else use int source codes */
#endif

#endif /* _MLIB_I_IMAGEDFT_H */
