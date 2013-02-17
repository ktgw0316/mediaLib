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
 * Copyright 2006 Sun Microsystems, Inc.  All rights reserved.
 * Use is subject to license terms.
 */

#ifndef _MLIB_S_IMAGEZOOMIN2X_H
#define	_MLIB_S_IMAGEZOOMIN2X_H

#pragma ident	"@(#)mlib_s_ImageZoomIn2X.h	9.5	07/11/05 SMI"

#ifdef __cplusplus
extern "C" {
#endif

#include <mlib_types.h>
#include <mlib_status.h>
#ifdef __SUNPRO_C
#include <sunmedia_intrin.h>
#else
#include <emmintrin.h>
#endif

mlib_status mlib_ImageZoomIn2X_Edge(
    mlib_image *dst,
    const mlib_image *src,
    mlib_filter filter,
    mlib_edge edge);

mlib_status mlib_ImageZoomIn2X_Edge_Fp(
    mlib_image *dst,
    const mlib_image *src,
    mlib_filter filter,
    mlib_edge edge);

/* mlib_ImageZoomIn2X_BC2_Fp.c */

void mlib_ImageZoomIn2X_D64_BC2_FL0(
    mlib_d64 *pdst,
    const mlib_d64 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    mlib_s32 channels);

void mlib_ImageZoomIn2X_D64_BC2_FL1(
    mlib_d64 *pdst,
    const mlib_d64 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    mlib_s32 channels);

void mlib_ImageZoomIn2X_D64_BC2_FL2(
    mlib_d64 *pdst,
    const mlib_d64 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    mlib_s32 channels);

void mlib_ImageZoomIn2X_D64_BC2_FL3(
    mlib_d64 *pdst,
    const mlib_d64 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    mlib_s32 channels);

void mlib_ImageZoomIn2X_S32_BC2_FL0(
    mlib_s32 *pdst,
    const mlib_s32 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    mlib_s32 channels);

void mlib_ImageZoomIn2X_S32_BC2_FL1(
    mlib_s32 *pdst,
    const mlib_s32 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    mlib_s32 channels);

void mlib_ImageZoomIn2X_S32_BC2_FL2(
    mlib_s32 *pdst,
    const mlib_s32 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    mlib_s32 channels);

void mlib_ImageZoomIn2X_S32_BC2_FL3(
    mlib_s32 *pdst,
    const mlib_s32 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    mlib_s32 channels);

void mlib_ImageZoomIn2X_F32_BC2_FL0(
    mlib_f32 *pdst,
    const mlib_f32 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    mlib_s32 channels);

void mlib_ImageZoomIn2X_F32_BC2_FL1(
    mlib_f32 *pdst,
    const mlib_f32 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    mlib_s32 channels);

void mlib_ImageZoomIn2X_F32_BC2_FL2(
    mlib_f32 *pdst,
    const mlib_f32 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    mlib_s32 channels);

void mlib_ImageZoomIn2X_F32_BC2_FL3(
    mlib_f32 *pdst,
    const mlib_f32 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    mlib_s32 channels);

/* mlib_ImageZoomIn2X_BC_Fp.c */

void mlib_ImageZoomIn2X_D64_BC_FL0(
    mlib_d64 *pdst,
    const mlib_d64 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    mlib_s32 channels);

void mlib_ImageZoomIn2X_D64_BC_FL1(
    mlib_d64 *pdst,
    const mlib_d64 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    mlib_s32 channels);

void mlib_ImageZoomIn2X_D64_BC_FL2(
    mlib_d64 *pdst,
    const mlib_d64 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    mlib_s32 channels);

void mlib_ImageZoomIn2X_D64_BC_FL3(
    mlib_d64 *pdst,
    const mlib_d64 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    mlib_s32 channels);

void mlib_ImageZoomIn2X_S32_BC_FL0(
    mlib_s32 *pdst,
    const mlib_s32 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    mlib_s32 channels);

void mlib_ImageZoomIn2X_S32_BC_FL1(
    mlib_s32 *pdst,
    const mlib_s32 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    mlib_s32 channels);

void mlib_ImageZoomIn2X_S32_BC_FL2(
    mlib_s32 *pdst,
    const mlib_s32 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    mlib_s32 channels);

void mlib_ImageZoomIn2X_S32_BC_FL3(
    mlib_s32 *pdst,
    const mlib_s32 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    mlib_s32 channels);

void mlib_ImageZoomIn2X_F32_BC_FL0(
    mlib_f32 *pdst,
    const mlib_f32 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    mlib_s32 channels);

void mlib_ImageZoomIn2X_F32_BC_FL1(
    mlib_f32 *pdst,
    const mlib_f32 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    mlib_s32 channels);

void mlib_ImageZoomIn2X_F32_BC_FL2(
    mlib_f32 *pdst,
    const mlib_f32 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    mlib_s32 channels);

void mlib_ImageZoomIn2X_F32_BC_FL3(
    mlib_f32 *pdst,
    const mlib_f32 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    mlib_s32 channels);

/* mlib_ImageZoomIn2X_BL.c */

void mlib_s_ImageZoomIn2X_U8_BL_FL0(
    mlib_u8 *pdst,
    const mlib_u8 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    mlib_s32 channels);

void mlib_s_ImageZoomIn2X_U8_BL_FL1(
    mlib_u8 *pdst,
    const mlib_u8 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    mlib_s32 channels);

void mlib_s_ImageZoomIn2X_U8_BL_FL2(
    mlib_u8 *pdst,
    const mlib_u8 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    mlib_s32 channels);

void mlib_s_ImageZoomIn2X_U8_BL_FL3(
    mlib_u8 *pdst,
    const mlib_u8 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    mlib_s32 channels);

void mlib_s_ImageZoomIn2X_S16_BL_FL0(
    mlib_s16 *pdst,
    const mlib_s16 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    mlib_s32 channels);

void mlib_s_ImageZoomIn2X_S16_BL_FL1(
    mlib_s16 *pdst,
    const mlib_s16 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    mlib_s32 channels);

void mlib_s_ImageZoomIn2X_S16_BL_FL2(
    mlib_s16 *pdst,
    const mlib_s16 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    mlib_s32 channels);

void mlib_s_ImageZoomIn2X_S16_BL_FL3(
    mlib_s16 *pdst,
    const mlib_s16 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    mlib_s32 channels);

void mlib_s_ImageZoomIn2X_U16_BL_FL0(
    mlib_u16 *pdst,
    const mlib_u16 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    mlib_s32 channels);

void mlib_s_ImageZoomIn2X_U16_BL_FL1(
    mlib_u16 *pdst,
    const mlib_u16 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    mlib_s32 channels);

void mlib_s_ImageZoomIn2X_U16_BL_FL2(
    mlib_u16 *pdst,
    const mlib_u16 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    mlib_s32 channels);

void mlib_s_ImageZoomIn2X_U16_BL_FL3(
    mlib_u16 *pdst,
    const mlib_u16 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    mlib_s32 channels);

/* mlib_ImageZoomIn2X_BL_Fp.c */

void mlib_ImageZoomIn2X_S32_BL_FL0(
    mlib_s32 *pdst,
    const mlib_s32 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    mlib_s32 channels);

void mlib_ImageZoomIn2X_S32_BL_FL1(
    mlib_s32 *pdst,
    const mlib_s32 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    mlib_s32 channels);

void mlib_ImageZoomIn2X_S32_BL_FL2(
    mlib_s32 *pdst,
    const mlib_s32 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    mlib_s32 channels);

void mlib_ImageZoomIn2X_S32_BL_FL3(
    mlib_s32 *pdst,
    const mlib_s32 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    mlib_s32 channels);

void mlib_ImageZoomIn2X_D64_BL_FL0(
    mlib_d64 *pdst,
    const mlib_d64 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    mlib_s32 channels);

void mlib_ImageZoomIn2X_D64_BL_FL1(
    mlib_d64 *pdst,
    const mlib_d64 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    mlib_s32 channels);

void mlib_ImageZoomIn2X_D64_BL_FL2(
    mlib_d64 *pdst,
    const mlib_d64 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    mlib_s32 channels);

void mlib_ImageZoomIn2X_D64_BL_FL3(
    mlib_d64 *pdst,
    const mlib_d64 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    mlib_s32 channels);

void mlib_ImageZoomIn2X_F32_BL_FL0(
    mlib_f32 *pdst,
    const mlib_f32 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    mlib_s32 channels);

void mlib_ImageZoomIn2X_F32_BL_FL1(
    mlib_f32 *pdst,
    const mlib_f32 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    mlib_s32 channels);

void mlib_ImageZoomIn2X_F32_BL_FL2(
    mlib_f32 *pdst,
    const mlib_f32 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    mlib_s32 channels);

void mlib_ImageZoomIn2X_F32_BL_FL3(
    mlib_f32 *pdst,
    const mlib_f32 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    mlib_s32 channels);

/* mlib_ImageZoomIn2X_Edge.c */

void mlib_ImageZoomIn2X_U8_CE(
    mlib_u8 *pdst,
    const mlib_u8 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    mlib_s32 channels,
    mlib_u8 *pdst_near,
    const mlib_u8 *psrc_near,
    mlib_s32 src_width_near,
    mlib_s32 src_height_near,
    mlib_s32 src_w_beg_near,
    mlib_s32 src_h_beg_near);

void mlib_ImageZoomIn2X_U8_ON(
    mlib_u8 *pdst,
    const mlib_u8 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    mlib_s32 channels,
    mlib_u8 *pdst_near,
    const mlib_u8 *psrc_near,
    mlib_s32 src_width_near,
    mlib_s32 src_height_near,
    mlib_s32 src_w_beg_near,
    mlib_s32 src_h_beg_near);

void mlib_ImageZoomIn2X_U8_SE_BL_FL0(
    mlib_u8 *pdst,
    const mlib_u8 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    mlib_s32 channels,
    mlib_u8 *pdst_near,
    const mlib_u8 *psrc_near,
    mlib_s32 src_width_near,
    mlib_s32 src_height_near,
    mlib_s32 src_w_beg_near,
    mlib_s32 src_h_beg_near);

void mlib_ImageZoomIn2X_U8_SE_BL_FL1(
    mlib_u8 *pdst,
    const mlib_u8 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    mlib_s32 channels,
    mlib_u8 *pdst_near,
    const mlib_u8 *psrc_near,
    mlib_s32 src_width_near,
    mlib_s32 src_height_near,
    mlib_s32 src_w_beg_near,
    mlib_s32 src_h_beg_near);

void mlib_ImageZoomIn2X_U8_SE_BL_FL2(
    mlib_u8 *pdst,
    const mlib_u8 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    mlib_s32 channels,
    mlib_u8 *pdst_near,
    const mlib_u8 *psrc_near,
    mlib_s32 src_width_near,
    mlib_s32 src_height_near,
    mlib_s32 src_w_beg_near,
    mlib_s32 src_h_beg_near);

void mlib_ImageZoomIn2X_U8_SE_BL_FL3(
    mlib_u8 *pdst,
    const mlib_u8 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    mlib_s32 channels,
    mlib_u8 *pdst_near,
    const mlib_u8 *psrc_near,
    mlib_s32 src_width_near,
    mlib_s32 src_height_near,
    mlib_s32 src_w_beg_near,
    mlib_s32 src_h_beg_near);

void mlib_ImageZoomIn2X_U8_SE_BC_FL0(
    mlib_u8 *pdst,
    const mlib_u8 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    mlib_s32 channels,
    mlib_u8 *pdst_near,
    const mlib_u8 *psrc_near,
    mlib_s32 src_width_near,
    mlib_s32 src_height_near,
    mlib_s32 src_w_beg_near,
    mlib_s32 src_h_beg_near,
    const mlib_u8 *psrc_beg,
    mlib_s32 src_width_beg,
    mlib_s32 src_height_beg);

void mlib_ImageZoomIn2X_U8_SE_BC_FL1(
    mlib_u8 *pdst,
    const mlib_u8 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    mlib_s32 channels,
    mlib_u8 *pdst_near,
    const mlib_u8 *psrc_near,
    mlib_s32 src_width_near,
    mlib_s32 src_height_near,
    mlib_s32 src_w_beg_near,
    mlib_s32 src_h_beg_near,
    const mlib_u8 *psrc_beg,
    mlib_s32 src_width_beg,
    mlib_s32 src_height_beg);

void mlib_ImageZoomIn2X_U8_SE_BC_FL2(
    mlib_u8 *pdst,
    const mlib_u8 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    mlib_s32 channels,
    mlib_u8 *pdst_near,
    const mlib_u8 *psrc_near,
    mlib_s32 src_width_near,
    mlib_s32 src_height_near,
    mlib_s32 src_w_beg_near,
    mlib_s32 src_h_beg_near,
    const mlib_u8 *psrc_beg,
    mlib_s32 src_width_beg,
    mlib_s32 src_height_beg);

void mlib_ImageZoomIn2X_U8_SE_BC_FL3(
    mlib_u8 *pdst,
    const mlib_u8 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    mlib_s32 channels,
    mlib_u8 *pdst_near,
    const mlib_u8 *psrc_near,
    mlib_s32 src_width_near,
    mlib_s32 src_height_near,
    mlib_s32 src_w_beg_near,
    mlib_s32 src_h_beg_near,
    const mlib_u8 *psrc_beg,
    mlib_s32 src_width_beg,
    mlib_s32 src_height_beg);

void mlib_ImageZoomIn2X_U8_SE_BC2_FL0(
    mlib_u8 *pdst,
    const mlib_u8 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    mlib_s32 channels,
    mlib_u8 *pdst_near,
    const mlib_u8 *psrc_near,
    mlib_s32 src_width_near,
    mlib_s32 src_height_near,
    mlib_s32 src_w_beg_near,
    mlib_s32 src_h_beg_near,
    const mlib_u8 *psrc_beg,
    mlib_s32 src_width_beg,
    mlib_s32 src_height_beg);

void mlib_ImageZoomIn2X_U8_SE_BC2_FL1(
    mlib_u8 *pdst,
    const mlib_u8 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    mlib_s32 channels,
    mlib_u8 *pdst_near,
    const mlib_u8 *psrc_near,
    mlib_s32 src_width_near,
    mlib_s32 src_height_near,
    mlib_s32 src_w_beg_near,
    mlib_s32 src_h_beg_near,
    const mlib_u8 *psrc_beg,
    mlib_s32 src_width_beg,
    mlib_s32 src_height_beg);

void mlib_ImageZoomIn2X_U8_SE_BC2_FL2(
    mlib_u8 *pdst,
    const mlib_u8 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    mlib_s32 channels,
    mlib_u8 *pdst_near,
    const mlib_u8 *psrc_near,
    mlib_s32 src_width_near,
    mlib_s32 src_height_near,
    mlib_s32 src_w_beg_near,
    mlib_s32 src_h_beg_near,
    const mlib_u8 *psrc_beg,
    mlib_s32 src_width_beg,
    mlib_s32 src_height_beg);

void mlib_ImageZoomIn2X_U8_SE_BC2_FL3(
    mlib_u8 *pdst,
    const mlib_u8 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    mlib_s32 channels,
    mlib_u8 *pdst_near,
    const mlib_u8 *psrc_near,
    mlib_s32 src_width_near,
    mlib_s32 src_height_near,
    mlib_s32 src_w_beg_near,
    mlib_s32 src_h_beg_near,
    const mlib_u8 *psrc_beg,
    mlib_s32 src_width_beg,
    mlib_s32 src_height_beg);

void mlib_ImageZoomIn2X_S16_CE(
    mlib_s16 *pdst,
    const mlib_s16 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    mlib_s32 channels,
    mlib_s16 *pdst_near,
    const mlib_s16 *psrc_near,
    mlib_s32 src_width_near,
    mlib_s32 src_height_near,
    mlib_s32 src_w_beg_near,
    mlib_s32 src_h_beg_near);

void mlib_ImageZoomIn2X_S16_ON(
    mlib_s16 *pdst,
    const mlib_s16 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    mlib_s32 channels,
    mlib_s16 *pdst_near,
    const mlib_s16 *psrc_near,
    mlib_s32 src_width_near,
    mlib_s32 src_height_near,
    mlib_s32 src_w_beg_near,
    mlib_s32 src_h_beg_near);

void mlib_ImageZoomIn2X_S16_SE_BL_FL0(
    mlib_s16 *pdst,
    const mlib_s16 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    mlib_s32 channels,
    mlib_s16 *pdst_near,
    const mlib_s16 *psrc_near,
    mlib_s32 src_width_near,
    mlib_s32 src_height_near,
    mlib_s32 src_w_beg_near,
    mlib_s32 src_h_beg_near);

void mlib_ImageZoomIn2X_S16_SE_BL_FL1(
    mlib_s16 *pdst,
    const mlib_s16 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    mlib_s32 channels,
    mlib_s16 *pdst_near,
    const mlib_s16 *psrc_near,
    mlib_s32 src_width_near,
    mlib_s32 src_height_near,
    mlib_s32 src_w_beg_near,
    mlib_s32 src_h_beg_near);

void mlib_ImageZoomIn2X_S16_SE_BL_FL2(
    mlib_s16 *pdst,
    const mlib_s16 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    mlib_s32 channels,
    mlib_s16 *pdst_near,
    const mlib_s16 *psrc_near,
    mlib_s32 src_width_near,
    mlib_s32 src_height_near,
    mlib_s32 src_w_beg_near,
    mlib_s32 src_h_beg_near);

void mlib_ImageZoomIn2X_S16_SE_BL_FL3(
    mlib_s16 *pdst,
    const mlib_s16 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    mlib_s32 channels,
    mlib_s16 *pdst_near,
    const mlib_s16 *psrc_near,
    mlib_s32 src_width_near,
    mlib_s32 src_height_near,
    mlib_s32 src_w_beg_near,
    mlib_s32 src_h_beg_near);

void mlib_ImageZoomIn2X_S16_SE_BC_FL0(
    mlib_s16 *pdst,
    const mlib_s16 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    mlib_s32 channels,
    mlib_s16 *pdst_near,
    const mlib_s16 *psrc_near,
    mlib_s32 src_width_near,
    mlib_s32 src_height_near,
    mlib_s32 src_w_beg_near,
    mlib_s32 src_h_beg_near,
    const mlib_s16 *psrc_beg,
    mlib_s32 src_width_beg,
    mlib_s32 src_height_beg);

void mlib_ImageZoomIn2X_S16_SE_BC_FL1(
    mlib_s16 *pdst,
    const mlib_s16 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    mlib_s32 channels,
    mlib_s16 *pdst_near,
    const mlib_s16 *psrc_near,
    mlib_s32 src_width_near,
    mlib_s32 src_height_near,
    mlib_s32 src_w_beg_near,
    mlib_s32 src_h_beg_near,
    const mlib_s16 *psrc_beg,
    mlib_s32 src_width_beg,
    mlib_s32 src_height_beg);

void mlib_ImageZoomIn2X_S16_SE_BC_FL2(
    mlib_s16 *pdst,
    const mlib_s16 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    mlib_s32 channels,
    mlib_s16 *pdst_near,
    const mlib_s16 *psrc_near,
    mlib_s32 src_width_near,
    mlib_s32 src_height_near,
    mlib_s32 src_w_beg_near,
    mlib_s32 src_h_beg_near,
    const mlib_s16 *psrc_beg,
    mlib_s32 src_width_beg,
    mlib_s32 src_height_beg);

void mlib_ImageZoomIn2X_S16_SE_BC_FL3(
    mlib_s16 *pdst,
    const mlib_s16 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    mlib_s32 channels,
    mlib_s16 *pdst_near,
    const mlib_s16 *psrc_near,
    mlib_s32 src_width_near,
    mlib_s32 src_height_near,
    mlib_s32 src_w_beg_near,
    mlib_s32 src_h_beg_near,
    const mlib_s16 *psrc_beg,
    mlib_s32 src_width_beg,
    mlib_s32 src_height_beg);

void mlib_ImageZoomIn2X_S16_SE_BC2_FL0(
    mlib_s16 *pdst,
    const mlib_s16 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    mlib_s32 channels,
    mlib_s16 *pdst_near,
    const mlib_s16 *psrc_near,
    mlib_s32 src_width_near,
    mlib_s32 src_height_near,
    mlib_s32 src_w_beg_near,
    mlib_s32 src_h_beg_near,
    const mlib_s16 *psrc_beg,
    mlib_s32 src_width_beg,
    mlib_s32 src_height_beg);

void mlib_ImageZoomIn2X_S16_SE_BC2_FL1(
    mlib_s16 *pdst,
    const mlib_s16 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    mlib_s32 channels,
    mlib_s16 *pdst_near,
    const mlib_s16 *psrc_near,
    mlib_s32 src_width_near,
    mlib_s32 src_height_near,
    mlib_s32 src_w_beg_near,
    mlib_s32 src_h_beg_near,
    const mlib_s16 *psrc_beg,
    mlib_s32 src_width_beg,
    mlib_s32 src_height_beg);

void mlib_ImageZoomIn2X_S16_SE_BC2_FL2(
    mlib_s16 *pdst,
    const mlib_s16 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    mlib_s32 channels,
    mlib_s16 *pdst_near,
    const mlib_s16 *psrc_near,
    mlib_s32 src_width_near,
    mlib_s32 src_height_near,
    mlib_s32 src_w_beg_near,
    mlib_s32 src_h_beg_near,
    const mlib_s16 *psrc_beg,
    mlib_s32 src_width_beg,
    mlib_s32 src_height_beg);

void mlib_ImageZoomIn2X_S16_SE_BC2_FL3(
    mlib_s16 *pdst,
    const mlib_s16 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    mlib_s32 channels,
    mlib_s16 *pdst_near,
    const mlib_s16 *psrc_near,
    mlib_s32 src_width_near,
    mlib_s32 src_height_near,
    mlib_s32 src_w_beg_near,
    mlib_s32 src_h_beg_near,
    const mlib_s16 *psrc_beg,
    mlib_s32 src_width_beg,
    mlib_s32 src_height_beg);

void mlib_ImageZoomIn2X_U16_CE(
    mlib_u16 *pdst,
    const mlib_u16 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    mlib_s32 channels,
    mlib_u16 *pdst_near,
    const mlib_u16 *psrc_near,
    mlib_s32 src_width_near,
    mlib_s32 src_height_near,
    mlib_s32 src_w_beg_near,
    mlib_s32 src_h_beg_near);

void mlib_ImageZoomIn2X_U16_ON(
    mlib_u16 *pdst,
    const mlib_u16 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    mlib_s32 channels,
    mlib_u16 *pdst_near,
    const mlib_u16 *psrc_near,
    mlib_s32 src_width_near,
    mlib_s32 src_height_near,
    mlib_s32 src_w_beg_near,
    mlib_s32 src_h_beg_near);

void mlib_ImageZoomIn2X_U16_SE_BL_FL0(
    mlib_u16 *pdst,
    const mlib_u16 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    mlib_s32 channels,
    mlib_u16 *pdst_near,
    const mlib_u16 *psrc_near,
    mlib_s32 src_width_near,
    mlib_s32 src_height_near,
    mlib_s32 src_w_beg_near,
    mlib_s32 src_h_beg_near);

void mlib_ImageZoomIn2X_U16_SE_BL_FL1(
    mlib_u16 *pdst,
    const mlib_u16 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    mlib_s32 channels,
    mlib_u16 *pdst_near,
    const mlib_u16 *psrc_near,
    mlib_s32 src_width_near,
    mlib_s32 src_height_near,
    mlib_s32 src_w_beg_near,
    mlib_s32 src_h_beg_near);

void mlib_ImageZoomIn2X_U16_SE_BL_FL2(
    mlib_u16 *pdst,
    const mlib_u16 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    mlib_s32 channels,
    mlib_u16 *pdst_near,
    const mlib_u16 *psrc_near,
    mlib_s32 src_width_near,
    mlib_s32 src_height_near,
    mlib_s32 src_w_beg_near,
    mlib_s32 src_h_beg_near);

void mlib_ImageZoomIn2X_U16_SE_BL_FL3(
    mlib_u16 *pdst,
    const mlib_u16 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    mlib_s32 channels,
    mlib_u16 *pdst_near,
    const mlib_u16 *psrc_near,
    mlib_s32 src_width_near,
    mlib_s32 src_height_near,
    mlib_s32 src_w_beg_near,
    mlib_s32 src_h_beg_near);

void mlib_ImageZoomIn2X_U16_SE_BC_FL0(
    mlib_u16 *pdst,
    const mlib_u16 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    mlib_s32 channels,
    mlib_u16 *pdst_near,
    const mlib_u16 *psrc_near,
    mlib_s32 src_width_near,
    mlib_s32 src_height_near,
    mlib_s32 src_w_beg_near,
    mlib_s32 src_h_beg_near,
    const mlib_u16 *psrc_beg,
    mlib_s32 src_width_beg,
    mlib_s32 src_height_beg);

void mlib_ImageZoomIn2X_U16_SE_BC_FL1(
    mlib_u16 *pdst,
    const mlib_u16 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    mlib_s32 channels,
    mlib_u16 *pdst_near,
    const mlib_u16 *psrc_near,
    mlib_s32 src_width_near,
    mlib_s32 src_height_near,
    mlib_s32 src_w_beg_near,
    mlib_s32 src_h_beg_near,
    const mlib_u16 *psrc_beg,
    mlib_s32 src_width_beg,
    mlib_s32 src_height_beg);

void mlib_ImageZoomIn2X_U16_SE_BC_FL2(
    mlib_u16 *pdst,
    const mlib_u16 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    mlib_s32 channels,
    mlib_u16 *pdst_near,
    const mlib_u16 *psrc_near,
    mlib_s32 src_width_near,
    mlib_s32 src_height_near,
    mlib_s32 src_w_beg_near,
    mlib_s32 src_h_beg_near,
    const mlib_u16 *psrc_beg,
    mlib_s32 src_width_beg,
    mlib_s32 src_height_beg);

void mlib_ImageZoomIn2X_U16_SE_BC_FL3(
    mlib_u16 *pdst,
    const mlib_u16 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    mlib_s32 channels,
    mlib_u16 *pdst_near,
    const mlib_u16 *psrc_near,
    mlib_s32 src_width_near,
    mlib_s32 src_height_near,
    mlib_s32 src_w_beg_near,
    mlib_s32 src_h_beg_near,
    const mlib_u16 *psrc_beg,
    mlib_s32 src_width_beg,
    mlib_s32 src_height_beg);

void mlib_ImageZoomIn2X_U16_SE_BC2_FL0(
    mlib_u16 *pdst,
    const mlib_u16 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    mlib_s32 channels,
    mlib_u16 *pdst_near,
    const mlib_u16 *psrc_near,
    mlib_s32 src_width_near,
    mlib_s32 src_height_near,
    mlib_s32 src_w_beg_near,
    mlib_s32 src_h_beg_near,
    const mlib_u16 *psrc_beg,
    mlib_s32 src_width_beg,
    mlib_s32 src_height_beg);

void mlib_ImageZoomIn2X_U16_SE_BC2_FL1(
    mlib_u16 *pdst,
    const mlib_u16 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    mlib_s32 channels,
    mlib_u16 *pdst_near,
    const mlib_u16 *psrc_near,
    mlib_s32 src_width_near,
    mlib_s32 src_height_near,
    mlib_s32 src_w_beg_near,
    mlib_s32 src_h_beg_near,
    const mlib_u16 *psrc_beg,
    mlib_s32 src_width_beg,
    mlib_s32 src_height_beg);

void mlib_ImageZoomIn2X_U16_SE_BC2_FL2(
    mlib_u16 *pdst,
    const mlib_u16 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    mlib_s32 channels,
    mlib_u16 *pdst_near,
    const mlib_u16 *psrc_near,
    mlib_s32 src_width_near,
    mlib_s32 src_height_near,
    mlib_s32 src_w_beg_near,
    mlib_s32 src_h_beg_near,
    const mlib_u16 *psrc_beg,
    mlib_s32 src_width_beg,
    mlib_s32 src_height_beg);

void mlib_ImageZoomIn2X_U16_SE_BC2_FL3(
    mlib_u16 *pdst,
    const mlib_u16 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    mlib_s32 channels,
    mlib_u16 *pdst_near,
    const mlib_u16 *psrc_near,
    mlib_s32 src_width_near,
    mlib_s32 src_height_near,
    mlib_s32 src_w_beg_near,
    mlib_s32 src_h_beg_near,
    const mlib_u16 *psrc_beg,
    mlib_s32 src_width_beg,
    mlib_s32 src_height_beg);

void mlib_ImageZoomIn2X_S32_CE(
    mlib_s32 *pdst,
    const mlib_s32 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    mlib_s32 channels,
    mlib_s32 *pdst_near,
    const mlib_s32 *psrc_near,
    mlib_s32 src_width_near,
    mlib_s32 src_height_near,
    mlib_s32 src_w_beg_near,
    mlib_s32 src_h_beg_near);

void mlib_ImageZoomIn2X_S32_ON(
    mlib_s32 *pdst,
    const mlib_s32 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    mlib_s32 channels,
    mlib_s32 *pdst_near,
    const mlib_s32 *psrc_near,
    mlib_s32 src_width_near,
    mlib_s32 src_height_near,
    mlib_s32 src_w_beg_near,
    mlib_s32 src_h_beg_near);

void mlib_ImageZoomIn2X_S32_SE_BL_FL0(
    mlib_s32 *pdst,
    const mlib_s32 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    mlib_s32 channels,
    mlib_s32 *pdst_near,
    const mlib_s32 *psrc_near,
    mlib_s32 src_width_near,
    mlib_s32 src_height_near,
    mlib_s32 src_w_beg_near,
    mlib_s32 src_h_beg_near);

void mlib_ImageZoomIn2X_S32_SE_BL_FL1(
    mlib_s32 *pdst,
    const mlib_s32 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    mlib_s32 channels,
    mlib_s32 *pdst_near,
    const mlib_s32 *psrc_near,
    mlib_s32 src_width_near,
    mlib_s32 src_height_near,
    mlib_s32 src_w_beg_near,
    mlib_s32 src_h_beg_near);

void mlib_ImageZoomIn2X_S32_SE_BL_FL2(
    mlib_s32 *pdst,
    const mlib_s32 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    mlib_s32 channels,
    mlib_s32 *pdst_near,
    const mlib_s32 *psrc_near,
    mlib_s32 src_width_near,
    mlib_s32 src_height_near,
    mlib_s32 src_w_beg_near,
    mlib_s32 src_h_beg_near);

void mlib_ImageZoomIn2X_S32_SE_BL_FL3(
    mlib_s32 *pdst,
    const mlib_s32 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    mlib_s32 channels,
    mlib_s32 *pdst_near,
    const mlib_s32 *psrc_near,
    mlib_s32 src_width_near,
    mlib_s32 src_height_near,
    mlib_s32 src_w_beg_near,
    mlib_s32 src_h_beg_near);

void mlib_ImageZoomIn2X_S32_SE_BC_FL0(
    mlib_s32 *pdst,
    const mlib_s32 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    mlib_s32 channels,
    mlib_s32 *pdst_near,
    const mlib_s32 *psrc_near,
    mlib_s32 src_width_near,
    mlib_s32 src_height_near,
    mlib_s32 src_w_beg_near,
    mlib_s32 src_h_beg_near,
    const mlib_s32 *psrc_beg,
    mlib_s32 src_width_beg,
    mlib_s32 src_height_beg);

void mlib_ImageZoomIn2X_S32_SE_BC_FL1(
    mlib_s32 *pdst,
    const mlib_s32 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    mlib_s32 channels,
    mlib_s32 *pdst_near,
    const mlib_s32 *psrc_near,
    mlib_s32 src_width_near,
    mlib_s32 src_height_near,
    mlib_s32 src_w_beg_near,
    mlib_s32 src_h_beg_near,
    const mlib_s32 *psrc_beg,
    mlib_s32 src_width_beg,
    mlib_s32 src_height_beg);

void mlib_ImageZoomIn2X_S32_SE_BC_FL2(
    mlib_s32 *pdst,
    const mlib_s32 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    mlib_s32 channels,
    mlib_s32 *pdst_near,
    const mlib_s32 *psrc_near,
    mlib_s32 src_width_near,
    mlib_s32 src_height_near,
    mlib_s32 src_w_beg_near,
    mlib_s32 src_h_beg_near,
    const mlib_s32 *psrc_beg,
    mlib_s32 src_width_beg,
    mlib_s32 src_height_beg);

void mlib_ImageZoomIn2X_S32_SE_BC_FL3(
    mlib_s32 *pdst,
    const mlib_s32 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    mlib_s32 channels,
    mlib_s32 *pdst_near,
    const mlib_s32 *psrc_near,
    mlib_s32 src_width_near,
    mlib_s32 src_height_near,
    mlib_s32 src_w_beg_near,
    mlib_s32 src_h_beg_near,
    const mlib_s32 *psrc_beg,
    mlib_s32 src_width_beg,
    mlib_s32 src_height_beg);

void mlib_ImageZoomIn2X_S32_SE_BC2_FL0(
    mlib_s32 *pdst,
    const mlib_s32 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    mlib_s32 channels,
    mlib_s32 *pdst_near,
    const mlib_s32 *psrc_near,
    mlib_s32 src_width_near,
    mlib_s32 src_height_near,
    mlib_s32 src_w_beg_near,
    mlib_s32 src_h_beg_near,
    const mlib_s32 *psrc_beg,
    mlib_s32 src_width_beg,
    mlib_s32 src_height_beg);

void mlib_ImageZoomIn2X_S32_SE_BC2_FL1(
    mlib_s32 *pdst,
    const mlib_s32 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    mlib_s32 channels,
    mlib_s32 *pdst_near,
    const mlib_s32 *psrc_near,
    mlib_s32 src_width_near,
    mlib_s32 src_height_near,
    mlib_s32 src_w_beg_near,
    mlib_s32 src_h_beg_near,
    const mlib_s32 *psrc_beg,
    mlib_s32 src_width_beg,
    mlib_s32 src_height_beg);

void mlib_ImageZoomIn2X_S32_SE_BC2_FL2(
    mlib_s32 *pdst,
    const mlib_s32 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    mlib_s32 channels,
    mlib_s32 *pdst_near,
    const mlib_s32 *psrc_near,
    mlib_s32 src_width_near,
    mlib_s32 src_height_near,
    mlib_s32 src_w_beg_near,
    mlib_s32 src_h_beg_near,
    const mlib_s32 *psrc_beg,
    mlib_s32 src_width_beg,
    mlib_s32 src_height_beg);

void mlib_ImageZoomIn2X_S32_SE_BC2_FL3(
    mlib_s32 *pdst,
    const mlib_s32 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    mlib_s32 channels,
    mlib_s32 *pdst_near,
    const mlib_s32 *psrc_near,
    mlib_s32 src_width_near,
    mlib_s32 src_height_near,
    mlib_s32 src_w_beg_near,
    mlib_s32 src_h_beg_near,
    const mlib_s32 *psrc_beg,
    mlib_s32 src_width_beg,
    mlib_s32 src_height_beg);

/* mlib_ImageZoomIn2X_Edge_Fp.c */

void mlib_ImageZoomIn2X_F32_SE_BL_FL0(
    mlib_f32 *pdst,
    const mlib_f32 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    mlib_s32 channels,
    mlib_f32 *pdst_near,
    const mlib_f32 *psrc_near,
    mlib_s32 src_width_near,
    mlib_s32 src_height_near,
    mlib_s32 src_w_beg_near,
    mlib_s32 src_h_beg_near);

void mlib_ImageZoomIn2X_F32_SE_BL_FL1(
    mlib_f32 *pdst,
    const mlib_f32 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    mlib_s32 channels,
    mlib_f32 *pdst_near,
    const mlib_f32 *psrc_near,
    mlib_s32 src_width_near,
    mlib_s32 src_height_near,
    mlib_s32 src_w_beg_near,
    mlib_s32 src_h_beg_near);

void mlib_ImageZoomIn2X_F32_SE_BL_FL2(
    mlib_f32 *pdst,
    const mlib_f32 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    mlib_s32 channels,
    mlib_f32 *pdst_near,
    const mlib_f32 *psrc_near,
    mlib_s32 src_width_near,
    mlib_s32 src_height_near,
    mlib_s32 src_w_beg_near,
    mlib_s32 src_h_beg_near);

void mlib_ImageZoomIn2X_F32_SE_BL_FL3(
    mlib_f32 *pdst,
    const mlib_f32 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    mlib_s32 channels,
    mlib_f32 *pdst_near,
    const mlib_f32 *psrc_near,
    mlib_s32 src_width_near,
    mlib_s32 src_height_near,
    mlib_s32 src_w_beg_near,
    mlib_s32 src_h_beg_near);

void mlib_ImageZoomIn2X_F32_SE_BC_FL0(
    mlib_f32 *pdst,
    const mlib_f32 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    mlib_s32 channels,
    mlib_f32 *pdst_near,
    const mlib_f32 *psrc_near,
    mlib_s32 src_width_near,
    mlib_s32 src_height_near,
    mlib_s32 src_w_beg_near,
    mlib_s32 src_h_beg_near,
    const mlib_f32 *psrc_beg,
    mlib_s32 src_width_beg,
    mlib_s32 src_height_beg);

void mlib_ImageZoomIn2X_F32_SE_BC_FL1(
    mlib_f32 *pdst,
    const mlib_f32 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    mlib_s32 channels,
    mlib_f32 *pdst_near,
    const mlib_f32 *psrc_near,
    mlib_s32 src_width_near,
    mlib_s32 src_height_near,
    mlib_s32 src_w_beg_near,
    mlib_s32 src_h_beg_near,
    const mlib_f32 *psrc_beg,
    mlib_s32 src_width_beg,
    mlib_s32 src_height_beg);

void mlib_ImageZoomIn2X_F32_SE_BC_FL2(
    mlib_f32 *pdst,
    const mlib_f32 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    mlib_s32 channels,
    mlib_f32 *pdst_near,
    const mlib_f32 *psrc_near,
    mlib_s32 src_width_near,
    mlib_s32 src_height_near,
    mlib_s32 src_w_beg_near,
    mlib_s32 src_h_beg_near,
    const mlib_f32 *psrc_beg,
    mlib_s32 src_width_beg,
    mlib_s32 src_height_beg);

void mlib_ImageZoomIn2X_F32_SE_BC_FL3(
    mlib_f32 *pdst,
    const mlib_f32 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    mlib_s32 channels,
    mlib_f32 *pdst_near,
    const mlib_f32 *psrc_near,
    mlib_s32 src_width_near,
    mlib_s32 src_height_near,
    mlib_s32 src_w_beg_near,
    mlib_s32 src_h_beg_near,
    const mlib_f32 *psrc_beg,
    mlib_s32 src_width_beg,
    mlib_s32 src_height_beg);

void mlib_ImageZoomIn2X_F32_SE_BC2_FL0(
    mlib_f32 *pdst,
    const mlib_f32 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    mlib_s32 channels,
    mlib_f32 *pdst_near,
    const mlib_f32 *psrc_near,
    mlib_s32 src_width_near,
    mlib_s32 src_height_near,
    mlib_s32 src_w_beg_near,
    mlib_s32 src_h_beg_near,
    const mlib_f32 *psrc_beg,
    mlib_s32 src_width_beg,
    mlib_s32 src_height_beg);

void mlib_ImageZoomIn2X_F32_SE_BC2_FL1(
    mlib_f32 *pdst,
    const mlib_f32 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    mlib_s32 channels,
    mlib_f32 *pdst_near,
    const mlib_f32 *psrc_near,
    mlib_s32 src_width_near,
    mlib_s32 src_height_near,
    mlib_s32 src_w_beg_near,
    mlib_s32 src_h_beg_near,
    const mlib_f32 *psrc_beg,
    mlib_s32 src_width_beg,
    mlib_s32 src_height_beg);

void mlib_ImageZoomIn2X_F32_SE_BC2_FL2(
    mlib_f32 *pdst,
    const mlib_f32 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    mlib_s32 channels,
    mlib_f32 *pdst_near,
    const mlib_f32 *psrc_near,
    mlib_s32 src_width_near,
    mlib_s32 src_height_near,
    mlib_s32 src_w_beg_near,
    mlib_s32 src_h_beg_near,
    const mlib_f32 *psrc_beg,
    mlib_s32 src_width_beg,
    mlib_s32 src_height_beg);

void mlib_ImageZoomIn2X_F32_SE_BC2_FL3(
    mlib_f32 *pdst,
    const mlib_f32 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    mlib_s32 channels,
    mlib_f32 *pdst_near,
    const mlib_f32 *psrc_near,
    mlib_s32 src_width_near,
    mlib_s32 src_height_near,
    mlib_s32 src_w_beg_near,
    mlib_s32 src_h_beg_near,
    const mlib_f32 *psrc_beg,
    mlib_s32 src_width_beg,
    mlib_s32 src_height_beg);

void mlib_ImageZoomIn2X_D64_SE_BL_FL0(
    mlib_d64 *pdst,
    const mlib_d64 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    mlib_s32 channels,
    mlib_d64 *pdst_near,
    const mlib_d64 *psrc_near,
    mlib_s32 src_width_near,
    mlib_s32 src_height_near,
    mlib_s32 src_w_beg_near,
    mlib_s32 src_h_beg_near);

void mlib_ImageZoomIn2X_D64_SE_BL_FL1(
    mlib_d64 *pdst,
    const mlib_d64 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    mlib_s32 channels,
    mlib_d64 *pdst_near,
    const mlib_d64 *psrc_near,
    mlib_s32 src_width_near,
    mlib_s32 src_height_near,
    mlib_s32 src_w_beg_near,
    mlib_s32 src_h_beg_near);

void mlib_ImageZoomIn2X_D64_SE_BL_FL2(
    mlib_d64 *pdst,
    const mlib_d64 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    mlib_s32 channels,
    mlib_d64 *pdst_near,
    const mlib_d64 *psrc_near,
    mlib_s32 src_width_near,
    mlib_s32 src_height_near,
    mlib_s32 src_w_beg_near,
    mlib_s32 src_h_beg_near);

void mlib_ImageZoomIn2X_D64_SE_BL_FL3(
    mlib_d64 *pdst,
    const mlib_d64 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    mlib_s32 channels,
    mlib_d64 *pdst_near,
    const mlib_d64 *psrc_near,
    mlib_s32 src_width_near,
    mlib_s32 src_height_near,
    mlib_s32 src_w_beg_near,
    mlib_s32 src_h_beg_near);

void mlib_ImageZoomIn2X_D64_SE_BC_FL0(
    mlib_d64 *pdst,
    const mlib_d64 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    mlib_s32 channels,
    mlib_d64 *pdst_near,
    const mlib_d64 *psrc_near,
    mlib_s32 src_width_near,
    mlib_s32 src_height_near,
    mlib_s32 src_w_beg_near,
    mlib_s32 src_h_beg_near,
    const mlib_d64 *psrc_beg,
    mlib_s32 src_width_beg,
    mlib_s32 src_height_beg);

void mlib_ImageZoomIn2X_D64_SE_BC_FL1(
    mlib_d64 *pdst,
    const mlib_d64 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    mlib_s32 channels,
    mlib_d64 *pdst_near,
    const mlib_d64 *psrc_near,
    mlib_s32 src_width_near,
    mlib_s32 src_height_near,
    mlib_s32 src_w_beg_near,
    mlib_s32 src_h_beg_near,
    const mlib_d64 *psrc_beg,
    mlib_s32 src_width_beg,
    mlib_s32 src_height_beg);

void mlib_ImageZoomIn2X_D64_SE_BC_FL2(
    mlib_d64 *pdst,
    const mlib_d64 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    mlib_s32 channels,
    mlib_d64 *pdst_near,
    const mlib_d64 *psrc_near,
    mlib_s32 src_width_near,
    mlib_s32 src_height_near,
    mlib_s32 src_w_beg_near,
    mlib_s32 src_h_beg_near,
    const mlib_d64 *psrc_beg,
    mlib_s32 src_width_beg,
    mlib_s32 src_height_beg);

void mlib_ImageZoomIn2X_D64_SE_BC_FL3(
    mlib_d64 *pdst,
    const mlib_d64 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    mlib_s32 channels,
    mlib_d64 *pdst_near,
    const mlib_d64 *psrc_near,
    mlib_s32 src_width_near,
    mlib_s32 src_height_near,
    mlib_s32 src_w_beg_near,
    mlib_s32 src_h_beg_near,
    const mlib_d64 *psrc_beg,
    mlib_s32 src_width_beg,
    mlib_s32 src_height_beg);

void mlib_ImageZoomIn2X_D64_SE_BC2_FL0(
    mlib_d64 *pdst,
    const mlib_d64 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    mlib_s32 channels,
    mlib_d64 *pdst_near,
    const mlib_d64 *psrc_near,
    mlib_s32 src_width_near,
    mlib_s32 src_height_near,
    mlib_s32 src_w_beg_near,
    mlib_s32 src_h_beg_near,
    const mlib_d64 *psrc_beg,
    mlib_s32 src_width_beg,
    mlib_s32 src_height_beg);

void mlib_ImageZoomIn2X_D64_SE_BC2_FL1(
    mlib_d64 *pdst,
    const mlib_d64 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    mlib_s32 channels,
    mlib_d64 *pdst_near,
    const mlib_d64 *psrc_near,
    mlib_s32 src_width_near,
    mlib_s32 src_height_near,
    mlib_s32 src_w_beg_near,
    mlib_s32 src_h_beg_near,
    const mlib_d64 *psrc_beg,
    mlib_s32 src_width_beg,
    mlib_s32 src_height_beg);

void mlib_ImageZoomIn2X_D64_SE_BC2_FL2(
    mlib_d64 *pdst,
    const mlib_d64 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    mlib_s32 channels,
    mlib_d64 *pdst_near,
    const mlib_d64 *psrc_near,
    mlib_s32 src_width_near,
    mlib_s32 src_height_near,
    mlib_s32 src_w_beg_near,
    mlib_s32 src_h_beg_near,
    const mlib_d64 *psrc_beg,
    mlib_s32 src_width_beg,
    mlib_s32 src_height_beg);

void mlib_ImageZoomIn2X_D64_SE_BC2_FL3(
    mlib_d64 *pdst,
    const mlib_d64 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    mlib_s32 channels,
    mlib_d64 *pdst_near,
    const mlib_d64 *psrc_near,
    mlib_s32 src_width_near,
    mlib_s32 src_height_near,
    mlib_s32 src_w_beg_near,
    mlib_s32 src_h_beg_near,
    const mlib_d64 *psrc_beg,
    mlib_s32 src_width_beg,
    mlib_s32 src_height_beg);

void mlib_ImageZoomIn2X_D64_CE(
    mlib_d64 *pdst,
    const mlib_d64 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    mlib_s32 channels,
    mlib_d64 *pdst_near,
    const mlib_d64 *psrc_near,
    mlib_s32 src_width_near,
    mlib_s32 src_height_near,
    mlib_s32 src_w_beg_near,
    mlib_s32 src_h_beg_near);

void mlib_ImageZoomIn2X_D64_ON(
    mlib_d64 *pdst,
    const mlib_d64 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    mlib_s32 channels,
    mlib_d64 *pdst_near,
    const mlib_d64 *psrc_near,
    mlib_s32 src_width_near,
    mlib_s32 src_height_near,
    mlib_s32 src_w_beg_near,
    mlib_s32 src_h_beg_near);

/* mlib_ImageZoomIn2X_Fp.c */

mlib_status mlib_ImageZoomIn2X_Fp(
    mlib_image *dst,
    const mlib_image *src,
    mlib_filter filter,
    mlib_edge edge);

/* mlib_s_ImageZoomIn2X_NN_D64_Fp.c */

static void mlib_ImageZoomIn2X_D64_NN_1(
    mlib_d64 *pdst,
    const mlib_d64 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg);

static void mlib_ImageZoomIn2X_D64_NN_2(
    mlib_d64 *pdst,
    const mlib_d64 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg);

static void mlib_ImageZoomIn2X_D64_NN_3(
    mlib_d64 *pdst,
    const mlib_d64 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg);

static void mlib_ImageZoomIn2X_D64_NN_4(
    mlib_d64 *pdst,
    const mlib_d64 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg);

void mlib_ImageZoomIn2X_D64_NN(
    mlib_d64 *pdst,
    const mlib_d64 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    mlib_s32 nchan);

/* mlib_s_ImageZoomIn2X_NN_S32.c */

static void mlib_ImageZoomIn2X_S32_NN_1(
    mlib_s32 *pdst,
    const mlib_s32 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg);

static void mlib_ImageZoomIn2X_S32_NN_2(
    mlib_s32 *pdst,
    const mlib_s32 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg);

static void mlib_ImageZoomIn2X_S32_NN_3(
    mlib_s32 *pdst,
    const mlib_s32 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg);

static void mlib_ImageZoomIn2X_S32_NN_4(
    mlib_s32 *pdst,
    const mlib_s32 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg);

void mlib_ImageZoomIn2X_S32_NN(
    mlib_s32 *pdst,
    const mlib_s32 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    mlib_s32 nchan);

/* mlib_s_ImageZoomIn2X_BC.c */

void mlib_s_ImageZoomIn2X_U8_BC_FL0(
    mlib_u8 *pdst,
    const mlib_u8 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    mlib_s32 channels);

void mlib_s_ImageZoomIn2X_U8_BC_FL1(
    mlib_u8 *pdst,
    const mlib_u8 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    mlib_s32 channels);

void mlib_s_ImageZoomIn2X_U8_BC_FL2(
    mlib_u8 *pdst,
    const mlib_u8 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    mlib_s32 channels);

void mlib_s_ImageZoomIn2X_U8_BC_FL3(
    mlib_u8 *pdst,
    const mlib_u8 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    mlib_s32 channels);

void mlib_s_ImageZoomIn2X_S16_BC_FL0(
    mlib_s16 *pdst,
    const mlib_s16 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    mlib_s32 channels);

void mlib_s_ImageZoomIn2X_S16_BC_FL1(
    mlib_s16 *pdst,
    const mlib_s16 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    mlib_s32 channels);

void mlib_s_ImageZoomIn2X_S16_BC_FL2(
    mlib_s16 *pdst,
    const mlib_s16 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    mlib_s32 channels);

void mlib_s_ImageZoomIn2X_S16_BC_FL3(
    mlib_s16 *pdst,
    const mlib_s16 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    mlib_s32 channels);

void mlib_s_ImageZoomIn2X_U16_BC_FL0(
    mlib_u16 *pdst,
    const mlib_u16 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    mlib_s32 channels);

void mlib_s_ImageZoomIn2X_U16_BC_FL1(
    mlib_u16 *pdst,
    const mlib_u16 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    mlib_s32 channels);

void mlib_s_ImageZoomIn2X_U16_BC_FL2(
    mlib_u16 *pdst,
    const mlib_u16 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    mlib_s32 channels);

void mlib_s_ImageZoomIn2X_U16_BC_FL3(
    mlib_u16 *pdst,
    const mlib_u16 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    mlib_s32 channels);

/* mlib_s_ImageZoomIn2X_BC2.c */

void mlib_s_ImageZoomIn2X_U8_BC2_FL0(
    mlib_u8 *pdst,
    const mlib_u8 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    mlib_s32 channels);

void mlib_s_ImageZoomIn2X_U8_BC2_FL1(
    mlib_u8 *pdst,
    const mlib_u8 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    mlib_s32 channels);

void mlib_s_ImageZoomIn2X_U8_BC2_FL2(
    mlib_u8 *pdst,
    const mlib_u8 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    mlib_s32 channels);

void mlib_s_ImageZoomIn2X_U8_BC2_FL3(
    mlib_u8 *pdst,
    const mlib_u8 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    mlib_s32 channels);

void mlib_s_ImageZoomIn2X_S16_BC2_FL0(
    mlib_s16 *pdst,
    const mlib_s16 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    mlib_s32 channels);

void mlib_s_ImageZoomIn2X_S16_BC2_FL1(
    mlib_s16 *pdst,
    const mlib_s16 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    mlib_s32 channels);

void mlib_s_ImageZoomIn2X_S16_BC2_FL2(
    mlib_s16 *pdst,
    const mlib_s16 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    mlib_s32 channels);

void mlib_s_ImageZoomIn2X_S16_BC2_FL3(
    mlib_s16 *pdst,
    const mlib_s16 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    mlib_s32 channels);

void mlib_s_ImageZoomIn2X_U16_BC2_FL0(
    mlib_u16 *pdst,
    const mlib_u16 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    mlib_s32 channels);

void mlib_s_ImageZoomIn2X_U16_BC2_FL1(
    mlib_u16 *pdst,
    const mlib_u16 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    mlib_s32 channels);

void mlib_s_ImageZoomIn2X_U16_BC2_FL2(
    mlib_u16 *pdst,
    const mlib_u16 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    mlib_s32 channels);

void mlib_s_ImageZoomIn2X_U16_BC2_FL3(
    mlib_u16 *pdst,
    const mlib_u16 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    mlib_s32 channels);

/* mlib_s_ImageZoomIn2X_NN.c */

void mlib_s_ImageZoomIn2X_U8_NN(
    mlib_u8 *pdst,
    const mlib_u8 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    mlib_s32 channels);

void mlib_s_ImageZoomIn2X_S16_NN(
    mlib_s16 *pdst,
    const mlib_s16 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    mlib_s32 channels);

#ifdef __cplusplus
}
#endif

#endif /* _MLIB_S_IMAGEZOOMIN2X_H */
