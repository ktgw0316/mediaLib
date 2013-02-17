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

#ifndef _MLIB_V_IMAGEZOOMIN2XINDEX_H
#define	_MLIB_V_IMAGEZOOMIN2XINDEX_H

#pragma ident	"@(#)mlib_v_ImageZoomIn2XIndex.h	9.2	07/11/05 SMI"

#ifdef __cplusplus
extern "C" {
#endif

#include <mlib_types.h>
#include <mlib_status.h>

/* mlib_v_ImageZoomIn2XIndex_BC.c */
mlib_status mlib_v_ImageZoomIn2XIndex_U8_U8_BC_FL0(
    mlib_u8 *pdst,
    const mlib_u8 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    const void *colormap,
    mlib_s32 chan);

mlib_status mlib_v_ImageZoomIn2XIndex_U8_U8_BC_FL1(
    mlib_u8 *pdst,
    const mlib_u8 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    const void *colormap,
    mlib_s32 chan);

mlib_status mlib_v_ImageZoomIn2XIndex_U8_U8_BC_FL2(
    mlib_u8 *pdst,
    const mlib_u8 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    const void *colormap,
    mlib_s32 chan);

mlib_status mlib_v_ImageZoomIn2XIndex_U8_U8_BC_FL3(
    mlib_u8 *pdst,
    const mlib_u8 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    const void *colormap,
    mlib_s32 chan);

mlib_status mlib_v_ImageZoomIn2XIndex_U8_S16_BC_FL0(
    mlib_s16 *pdst,
    const mlib_s16 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    const void *colormap,
    mlib_s32 chan);

mlib_status mlib_v_imagezoomin2xindex_u8_s16_bc_fl1(
    mlib_s16 *pdst,
    const mlib_s16 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    const void *colormap,
    mlib_s32 chan);

mlib_status mlib_v_ImageZoomIn2XIndex_U8_S16_BC_FL2(
    mlib_s16 *pdst,
    const mlib_s16 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    const void *colormap,
    mlib_s32 chan);

mlib_status mlib_v_ImageZoomIn2XIndex_U8_S16_BC_FL3(
    mlib_s16 *pdst,
    const mlib_s16 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    const void *colormap,
    mlib_s32 chan);

mlib_status mlib_v_ImageZoomIn2XIndex_S16_U8_BC_FL0(
    mlib_u8 *pdst,
    const mlib_u8 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    const void *colormap,
    mlib_s32 chan);

mlib_status mlib_v_ImageZoomIn2XIndex_S16_U8_BC_FL1(
    mlib_u8 *pdst,
    const mlib_u8 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    const void *colormap,
    mlib_s32 chan);

mlib_status mlib_v_ImageZoomIn2XIndex_S16_U8_BC_FL2(
    mlib_u8 *pdst,
    const mlib_u8 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    const void *colormap,
    mlib_s32 chan);

mlib_status mlib_v_ImageZoomIn2XIndex_S16_U8_BC_FL3(
    mlib_u8 *pdst,
    const mlib_u8 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    const void *colormap,
    mlib_s32 chan);

mlib_status mlib_v_ImageZoomIn2XIndex_S16_S16_BC_FL0(
    mlib_s16 *pdst,
    const mlib_s16 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    const void *colormap,
    mlib_s32 chan);

mlib_status mlib_v_ImageZoomIn2XIndex_S16_S16_BC_FL1(
    mlib_s16 *pdst,
    const mlib_s16 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    const void *colormap,
    mlib_s32 chan);

mlib_status mlib_v_ImageZoomIn2XIndex_S16_S16_BC_FL2(
    mlib_s16 *pdst,
    const mlib_s16 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    const void *colormap,
    mlib_s32 chan);

mlib_status mlib_v_ImageZoomIn2XIndex_S16_S16_BC_FL3(
    mlib_s16 *pdst,
    const mlib_s16 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    const void *colormap,
    mlib_s32 chan);

mlib_status mlib_v_ImageZoomIn2XIndex_U8_BC_FL0(
    mlib_u8 *pdst,
    const mlib_u8 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    const void *colormap);

mlib_status mlib_v_ImageZoomIn2XIndex_U8_BC_FL1(
    mlib_u8 *pdst,
    const mlib_u8 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    const void *colormap);

mlib_status mlib_v_ImageZoomIn2XIndex_U8_BC_FL2(
    mlib_u8 *pdst,
    const mlib_u8 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    const void *colormap);

mlib_status mlib_v_ImageZoomIn2XIndex_U8_BC_FL3(
    mlib_u8 *pdst,
    const mlib_u8 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    const void *colormap);

mlib_status mlib_v_ImageZoomIn2XIndex_S16_BC_FL0(
    mlib_s16 *pdst,
    const mlib_s16 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    const void *colormap);

mlib_status mlib_v_ImageZoomIn2XIndex_S16_BC_FL1(
    mlib_s16 *pdst,
    const mlib_s16 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    const void *colormap);

mlib_status mlib_v_ImageZoomIn2XIndex_S16_BC_FL2(
    mlib_s16 *pdst,
    const mlib_s16 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    const void *colormap);

mlib_status mlib_v_ImageZoomIn2XIndex_S16_BC_FL3(
    mlib_s16 *pdst,
    const mlib_s16 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    const void *colormap);

/* mlib_v_ImageZoomIn2XIndex_BC2.c */

mlib_status mlib_v_ImageZoomIn2XIndex_U8_U8_BC2_FL0(
    mlib_u8 *pdst,
    const mlib_u8 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    const void *colormap,
    mlib_s32 chan);

mlib_status mlib_v_ImageZoomIn2XIndex_U8_U8_BC2_FL1(
    mlib_u8 *pdst,
    const mlib_u8 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    const void *colormap,
    mlib_s32 chan);

mlib_status mlib_v_ImageZoomIn2XIndex_U8_U8_BC2_FL2(
    mlib_u8 *pdst,
    const mlib_u8 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    const void *colormap,
    mlib_s32 chan);

mlib_status mlib_v_ImageZoomIn2XIndex_U8_U8_BC2_FL3(
    mlib_u8 *pdst,
    const mlib_u8 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    const void *colormap,
    mlib_s32 chan);

mlib_status mlib_v_ImageZoomIn2XIndex_U8_S16_BC2_FL0(
    mlib_s16 *pdst,
    const mlib_s16 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    const void *colormap,
    mlib_s32 chan);

mlib_status mlib_v_ImageZoomIn2XIndex_U8_S16_BC2_FL1(
    mlib_s16 *pdst,
    const mlib_s16 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    const void *colormap,
    mlib_s32 chan);

mlib_status mlib_v_ImageZoomIn2XIndex_U8_S16_BC2_FL2(
    mlib_s16 *pdst,
    const mlib_s16 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    const void *colormap,
    mlib_s32 chan);

mlib_status mlib_v_ImageZoomIn2XIndex_U8_S16_BC2_FL3(
    mlib_s16 *pdst,
    const mlib_s16 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    const void *colormap,
    mlib_s32 chan);

mlib_status mlib_v_ImageZoomIn2XIndex_S16_U8_BC2_FL0(
    mlib_u8 *pdst,
    const mlib_u8 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    const void *colormap,
    mlib_s32 chan);

mlib_status mlib_v_ImageZoomIn2XIndex_S16_U8_BC2_FL1(
    mlib_u8 *pdst,
    const mlib_u8 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    const void *colormap,
    mlib_s32 chan);

mlib_status mlib_v_ImageZoomIn2XIndex_S16_U8_BC2_FL2(
    mlib_u8 *pdst,
    const mlib_u8 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    const void *colormap,
    mlib_s32 chan);

mlib_status mlib_v_ImageZoomIn2XIndex_S16_U8_BC2_FL3(
    mlib_u8 *pdst,
    const mlib_u8 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    const void *colormap,
    mlib_s32 chan);

mlib_status mlib_v_ImageZoomIn2XIndex_S16_S16_BC2_FL0(
    mlib_s16 *pdst,
    const mlib_s16 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    const void *colormap,
    mlib_s32 chan);

mlib_status mlib_v_ImageZoomIn2XIndex_S16_S16_BC2_FL1(
    mlib_s16 *pdst,
    const mlib_s16 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    const void *colormap,
    mlib_s32 chan);

mlib_status mlib_v_ImageZoomIn2XIndex_S16_S16_BC2_FL2(
    mlib_s16 *pdst,
    const mlib_s16 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    const void *colormap,
    mlib_s32 chan);

mlib_status mlib_v_ImageZoomIn2XIndex_S16_S16_BC2_FL3(
    mlib_s16 *pdst,
    const mlib_s16 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    const void *colormap,
    mlib_s32 chan);

mlib_status mlib_v_ImageZoomIn2XIndex_U8_BC2_FL0(
    mlib_u8 *pdst,
    const mlib_u8 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    const void *colormap);

mlib_status mlib_v_ImageZoomIn2XIndex_U8_BC2_FL1(
    mlib_u8 *pdst,
    const mlib_u8 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    const void *colormap);

mlib_status mlib_v_ImageZoomIn2XIndex_U8_BC2_FL2(
    mlib_u8 *pdst,
    const mlib_u8 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    const void *colormap);

mlib_status mlib_v_ImageZoomIn2XIndex_U8_BC2_FL3(
    mlib_u8 *pdst,
    const mlib_u8 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    const void *colormap);

mlib_status mlib_v_ImageZoomIn2XIndex_S16_BC2_FL0(
    mlib_s16 *pdst,
    const mlib_s16 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    const void *colormap);

mlib_status mlib_v_ImageZoomIn2XIndex_S16_BC2_FL1(
    mlib_s16 *pdst,
    const mlib_s16 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    const void *colormap);

mlib_status mlib_v_ImageZoomIn2XIndex_S16_BC2_FL2(
    mlib_s16 *pdst,
    const mlib_s16 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    const void *colormap);

mlib_status mlib_v_ImageZoomIn2XIndex_S16_BC2_FL3(
    mlib_s16 *pdst,
    const mlib_s16 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    const void *colormap);

/* mlib_v_ImageZoomIn2XIndex_BL.c */

mlib_status mlib_v_ImageZoomIn2XIndex_U8_U8_BL_FL0(
    mlib_u8 *pdst,
    const mlib_u8 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    const void *colormap,
    mlib_s32 chan);

mlib_status mlib_v_ImageZoomIn2XIndex_U8_U8_BL_FL1(
    mlib_u8 *pdst,
    const mlib_u8 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    const void *colormap,
    mlib_s32 chan);

mlib_status mlib_v_ImageZoomIn2XIndex_U8_U8_BL_FL2(
    mlib_u8 *pdst,
    const mlib_u8 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    const void *colormap,
    mlib_s32 chan);

mlib_status mlib_v_ImageZoomIn2XIndex_U8_U8_BL_FL3(
    mlib_u8 *pdst,
    const mlib_u8 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    const void *colormap,
    mlib_s32 chan);

mlib_status mlib_v_ImageZoomIn2XIndex_U8_S16_BL_FL0(
    mlib_s16 *pdst,
    const mlib_s16 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    const void *colormap,
    mlib_s32 chan);

mlib_status mlib_v_ImageZoomIn2XIndex_U8_S16_BL_FL1(
    mlib_s16 *pdst,
    const mlib_s16 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    const void *colormap,
    mlib_s32 chan);

mlib_status mlib_v_ImageZoomIn2XIndex_U8_S16_BL_FL2(
    mlib_s16 *pdst,
    const mlib_s16 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    const void *colormap,
    mlib_s32 chan);

mlib_status mlib_v_ImageZoomIn2XIndex_U8_S16_BL_FL3(
    mlib_s16 *pdst,
    const mlib_s16 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    const void *colormap,
    mlib_s32 chan);

mlib_status mlib_v_ImageZoomIn2XIndex_S16_U8_BL_FL0(
    mlib_u8 *pdst,
    const mlib_u8 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    const void *colormap,
    mlib_s32 chan);

mlib_status mlib_v_ImageZoomIn2XIndex_S16_U8_BL_FL1(
    mlib_u8 *pdst,
    const mlib_u8 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    const void *colormap,
    mlib_s32 chan);

mlib_status mlib_v_ImageZoomIn2XIndex_S16_U8_BL_FL2(
    mlib_u8 *pdst,
    const mlib_u8 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    const void *colormap,
    mlib_s32 chan);

mlib_status mlib_v_ImageZoomIn2XIndex_S16_U8_BL_FL3(
    mlib_u8 *pdst,
    const mlib_u8 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    const void *colormap,
    mlib_s32 chan);

mlib_status mlib_v_ImageZoomIn2XIndex_S16_S16_BL_FL0(
    mlib_s16 *pdst,
    const mlib_s16 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    const void *colormap,
    mlib_s32 chan);

mlib_status mlib_v_ImageZoomIn2XIndex_S16_S16_BL_FL1(
    mlib_s16 *pdst,
    const mlib_s16 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    const void *colormap,
    mlib_s32 chan);

mlib_status mlib_v_ImageZoomIn2XIndex_S16_S16_BL_FL2(
    mlib_s16 *pdst,
    const mlib_s16 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    const void *colormap,
    mlib_s32 chan);

mlib_status mlib_v_ImageZoomIn2XIndex_S16_S16_BL_FL3(
    mlib_s16 *pdst,
    const mlib_s16 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    const void *colormap,
    mlib_s32 chan);

mlib_status mlib_v_ImageZoomIn2XIndex_U8_BL_FL0(
    mlib_u8 *pdst,
    const mlib_u8 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    const void *colormap);

mlib_status mlib_v_ImageZoomIn2XIndex_U8_BL_FL1(
    mlib_u8 *pdst,
    const mlib_u8 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    const void *colormap);

mlib_status mlib_v_ImageZoomIn2XIndex_U8_BL_FL2(
    mlib_u8 *pdst,
    const mlib_u8 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    const void *colormap);

mlib_status mlib_v_ImageZoomIn2XIndex_U8_BL_FL3(
    mlib_u8 *pdst,
    const mlib_u8 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    const void *colormap);

mlib_status mlib_v_ImageZoomIn2XIndex_S16_BL_FL0(
    mlib_s16 *pdst,
    const mlib_s16 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    const void *colormap);

mlib_status mlib_v_ImageZoomIn2XIndex_S16_BL_FL1(
    mlib_s16 *pdst,
    const mlib_s16 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    const void *colormap);

mlib_status mlib_v_ImageZoomIn2XIndex_S16_BL_FL2(
    mlib_s16 *pdst,
    const mlib_s16 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    const void *colormap);

mlib_status mlib_v_ImageZoomIn2XIndex_S16_BL_FL3(
    mlib_s16 *pdst,
    const mlib_s16 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    const void *colormap);

#ifdef __cplusplus
}
#endif

#endif /* _MLIB_V_IMAGEZOOMIN2XINDEX_H */
