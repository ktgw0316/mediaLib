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

#ifndef _MLIB_V_IMAGEZOOMIN2X_PROTO_H
#define	_MLIB_V_IMAGEZOOMIN2X_PROTO_H

#pragma ident	"@(#)mlib_v_ImageZoomIn2X_proto.h	9.2	07/11/05 SMI"

#ifdef __cplusplus
extern "C" {
#endif

#include <mlib_types.h>
#include <mlib_status.h>

/* mlib_v_ImageZoomIn2X_BC.c */
mlib_status mlib_v_ImageZoomIn2X_U8_1BC_FL00(
    void *pdst,
    void *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg);

mlib_status mlib_v_ImageZoomIn2X_U8_1BC_FL01(
    void *pdst,
    void *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg);

mlib_status mlib_v_ImageZoomIn2X_U8_1BC_FL10(
    void *pdst,
    void *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg);

mlib_status mlib_v_ImageZoomIn2X_U8_1BC_FL11(
    void *pdst,
    void *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg);

mlib_status mlib_v_ImageZoomIn2X_U8_2BC_FL00(
    void *pdst,
    void *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg);

mlib_status mlib_v_ImageZoomIn2X_U8_2BC_FL01(
    void *pdst,
    void *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg);

mlib_status mlib_v_ImageZoomIn2X_U8_2BC_FL10(
    void *pdst,
    void *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg);

mlib_status mlib_v_ImageZoomIn2X_U8_2BC_FL11(
    void *pdst,
    void *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg);

mlib_status mlib_v_ImageZoomIn2X_U8_3BC_FL00(
    void *pdst,
    void *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg);

mlib_status mlib_v_ImageZoomIn2X_U8_3BC_FL01(
    void *pdst,
    void *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg);

mlib_status mlib_v_ImageZoomIn2X_U8_3BC_FL10(
    void *pdst,
    void *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg);

mlib_status mlib_v_ImageZoomIn2X_U8_3BC_FL11(
    void *pdst,
    void *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg);

mlib_status mlib_v_ImageZoomIn2X_U8_4BC_FL00(
    void *pdst,
    void *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg);

mlib_status mlib_v_ImageZoomIn2X_U8_4BC_FL01(
    void *pdst,
    void *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg);

mlib_status mlib_v_ImageZoomIn2X_U8_4BC_FL10(
    void *pdst,
    void *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg);

mlib_status mlib_v_ImageZoomIn2X_U8_4BC_FL11(
    void *pdst,
    void *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg);

/* mlib_v_ImageZoomIn2X_BC2.c */

mlib_status mlib_v_ImageZoomIn2X_U8_1BC2_FL00(
    void *pdst,
    void *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg);

mlib_status mlib_v_ImageZoomIn2X_U8_1BC2_FL01(
    void *pdst,
    void *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg);

mlib_status mlib_v_ImageZoomIn2X_U8_1BC2_FL10(
    void *pdst,
    void *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg);

mlib_status mlib_v_ImageZoomIn2X_U8_1BC2_FL11(
    void *pdst,
    void *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg);

mlib_status mlib_v_ImageZoomIn2X_U8_2BC2_FL00(
    void *pdst,
    void *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg);

mlib_status mlib_v_ImageZoomIn2X_U8_2BC2_FL01(
    void *pdst,
    void *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg);

mlib_status mlib_v_ImageZoomIn2X_U8_2BC2_FL10(
    void *pdst,
    void *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg);

mlib_status mlib_v_ImageZoomIn2X_U8_2BC2_FL11(
    void *pdst,
    void *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg);

mlib_status mlib_v_ImageZoomIn2X_U8_3BC2_FL00(
    void *pdst,
    void *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg);

mlib_status mlib_v_ImageZoomIn2X_U8_3BC2_FL01(
    void *pdst,
    void *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg);

mlib_status mlib_v_ImageZoomIn2X_U8_3BC2_FL10(
    void *pdst,
    void *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg);

mlib_status mlib_v_ImageZoomIn2X_U8_3BC2_FL11(
    void *pdst,
    void *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg);

mlib_status mlib_v_ImageZoomIn2X_U8_4BC2_FL00(
    void *pdst,
    void *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg);

mlib_status mlib_v_ImageZoomIn2X_U8_4BC2_FL01(
    void *pdst,
    void *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg);

mlib_status mlib_v_ImageZoomIn2X_U8_4BC2_FL10(
    void *pdst,
    void *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg);

mlib_status mlib_v_ImageZoomIn2X_U8_4BC2_FL11(
    void *pdst,
    void *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg);

/* mlib_v_ImageZoomIn2X_BC2_S16.c */

mlib_status mlib_v_ImageZoomIn2X_S16_1BC2_FL00(
    void *pdst,
    void *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg);

mlib_status mlib_v_ImageZoomIn2X_S16_1BC2_FL01(
    void *pdst,
    void *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg);

mlib_status mlib_v_ImageZoomIn2X_S16_1BC2_FL10(
    void *pdst,
    void *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg);

mlib_status mlib_v_ImageZoomIn2X_S16_1BC2_FL11(
    void *pdst,
    void *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg);

mlib_status mlib_v_ImageZoomIn2X_S16_2BC2_FL00(
    void *pdst,
    void *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg);

mlib_status mlib_v_ImageZoomIn2X_S16_2BC2_FL01(
    void *pdst,
    void *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg);

mlib_status mlib_v_ImageZoomIn2X_S16_2BC2_FL10(
    void *pdst,
    void *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg);

mlib_status mlib_v_ImageZoomIn2X_S16_2BC2_FL11(
    void *pdst,
    void *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg);

mlib_status mlib_v_ImageZoomIn2X_S16_3BC2_FL00(
    void *pdst,
    void *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg);

mlib_status mlib_v_ImageZoomIn2X_S16_3BC2_FL01(
    void *pdst,
    void *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg);

mlib_status mlib_v_ImageZoomIn2X_S16_3BC2_FL10(
    void *pdst,
    void *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg);

mlib_status mlib_v_ImageZoomIn2X_S16_3BC2_FL11(
    void *pdst,
    void *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg);

mlib_status mlib_v_ImageZoomIn2X_S16_4BC2_FL00(
    void *pdst,
    void *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg);

mlib_status mlib_v_ImageZoomIn2X_S16_4BC2_FL01(
    void *pdst,
    void *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg);

mlib_status mlib_v_ImageZoomIn2X_S16_4BC2_FL10(
    void *pdst,
    void *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg);

mlib_status mlib_v_ImageZoomIn2X_S16_4BC2_FL11(
    void *pdst,
    void *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg);

/* mlib_v_ImageZoomIn2X_BC2_U16.c */

mlib_status mlib_v_ImageZoomIn2X_U16_1BC2_FL00(
    void *pdst,
    void *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg);

mlib_status mlib_v_ImageZoomIn2X_U16_1BC2_FL01(
    void *pdst,
    void *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg);

mlib_status mlib_v_ImageZoomIn2X_U16_1BC2_FL10(
    void *pdst,
    void *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg);

mlib_status mlib_v_ImageZoomIn2X_U16_1BC2_FL11(
    void *pdst,
    void *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg);

mlib_status mlib_v_ImageZoomIn2X_U16_2BC2_FL00(
    void *pdst,
    void *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg);

mlib_status mlib_v_ImageZoomIn2X_U16_2BC2_FL01(
    void *pdst,
    void *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg);

mlib_status mlib_v_ImageZoomIn2X_U16_2BC2_FL10(
    void *pdst,
    void *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg);

mlib_status mlib_v_ImageZoomIn2X_U16_2BC2_FL11(
    void *pdst,
    void *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg);

mlib_status mlib_v_ImageZoomIn2X_U16_3BC2_FL00(
    void *pdst,
    void *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg);

mlib_status mlib_v_ImageZoomIn2X_U16_3BC2_FL01(
    void *pdst,
    void *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg);

mlib_status mlib_v_ImageZoomIn2X_U16_3BC2_FL10(
    void *pdst,
    void *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg);

mlib_status mlib_v_ImageZoomIn2X_U16_3BC2_FL11(
    void *pdst,
    void *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg);

mlib_status mlib_v_ImageZoomIn2X_U16_4BC2_FL00(
    void *pdst,
    void *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg);

mlib_status mlib_v_ImageZoomIn2X_U16_4BC2_FL01(
    void *pdst,
    void *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg);

mlib_status mlib_v_ImageZoomIn2X_U16_4BC2_FL10(
    void *pdst,
    void *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg);

mlib_status mlib_v_ImageZoomIn2X_U16_4BC2_FL11(
    void *pdst,
    void *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg);

/* mlib_v_ImageZoomIn2X_BC_S16.c */

mlib_status mlib_v_ImageZoomIn2X_S16_1BC_FL00(
    void *pdst,
    void *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg);

mlib_status mlib_v_ImageZoomIn2X_S16_1BC_FL01(
    void *pdst,
    void *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg);

mlib_status mlib_v_ImageZoomIn2X_S16_1BC_FL10(
    void *pdst,
    void *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg);

mlib_status mlib_v_ImageZoomIn2X_S16_1BC_FL11(
    void *pdst,
    void *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg);

mlib_status mlib_v_ImageZoomIn2X_S16_2BC_FL00(
    void *pdst,
    void *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg);

mlib_status mlib_v_ImageZoomIn2X_S16_2BC_FL01(
    void *pdst,
    void *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg);

mlib_status mlib_v_ImageZoomIn2X_S16_2BC_FL10(
    void *pdst,
    void *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg);

mlib_status mlib_v_ImageZoomIn2X_S16_2BC_FL11(
    void *pdst,
    void *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg);

mlib_status mlib_v_ImageZoomIn2X_S16_3BC_FL00(
    void *pdst,
    void *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg);

mlib_status mlib_v_ImageZoomIn2X_S16_3BC_FL01(
    void *pdst,
    void *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg);

mlib_status mlib_v_ImageZoomIn2X_S16_3BC_FL10(
    void *pdst,
    void *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg);

mlib_status mlib_v_ImageZoomIn2X_S16_3BC_FL11(
    void *pdst,
    void *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg);

mlib_status mlib_v_ImageZoomIn2X_S16_4BC_FL00(
    void *pdst,
    void *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg);

mlib_status mlib_v_ImageZoomIn2X_S16_4BC_FL01(
    void *pdst,
    void *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg);

mlib_status mlib_v_ImageZoomIn2X_S16_4BC_FL10(
    void *pdst,
    void *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg);

mlib_status mlib_v_ImageZoomIn2X_S16_4BC_FL11(
    void *pdst,
    void *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg);

/* mlib_v_ImageZoomIn2X_BC_U16.c */

mlib_status mlib_v_ImageZoomIn2X_U16_1BC_FL00(
    void *pdst,
    void *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg);

mlib_status mlib_v_ImageZoomIn2X_U16_1BC_FL01(
    void *pdst,
    void *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg);

mlib_status mlib_v_ImageZoomIn2X_U16_1BC_FL10(
    void *pdst,
    void *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg);

mlib_status mlib_v_ImageZoomIn2X_U16_1BC_FL11(
    void *pdst,
    void *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg);

mlib_status mlib_v_ImageZoomIn2X_U16_2BC_FL00(
    void *pdst,
    void *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg);

mlib_status mlib_v_ImageZoomIn2X_U16_2BC_FL01(
    void *pdst,
    void *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg);

mlib_status mlib_v_ImageZoomIn2X_U16_2BC_FL10(
    void *pdst,
    void *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg);

mlib_status mlib_v_ImageZoomIn2X_U16_2BC_FL11(
    void *pdst,
    void *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg);

mlib_status mlib_v_ImageZoomIn2X_U16_3BC_FL00(
    void *pdst,
    void *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg);

mlib_status mlib_v_ImageZoomIn2X_U16_3BC_FL01(
    void *pdst,
    void *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg);

mlib_status mlib_v_ImageZoomIn2X_U16_3BC_FL10(
    void *pdst,
    void *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg);

mlib_status mlib_v_ImageZoomIn2X_U16_3BC_FL11(
    void *pdst,
    void *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg);

mlib_status mlib_v_ImageZoomIn2X_U16_4BC_FL00(
    void *pdst,
    void *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg);

mlib_status mlib_v_ImageZoomIn2X_U16_4BC_FL01(
    void *pdst,
    void *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg);

mlib_status mlib_v_ImageZoomIn2X_U16_4BC_FL10(
    void *pdst,
    void *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg);

mlib_status mlib_v_ImageZoomIn2X_U16_4BC_FL11(
    void *pdst,
    void *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg);

/* mlib_v_ImageZoomIn2X_BL.c */

void mlib_v_ImageZoomIn2X_U8_1BL_FL0(
    mlib_u8 *pdst,
    const mlib_u8 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride);

void mlib_v_ImageZoomIn2X_U8_2BL_FL0(
    mlib_u8 *pdst,
    const mlib_u8 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride);

void mlib_v_ImageZoomIn2X_U8_3BL_FL0(
    mlib_u8 *pdst,
    const mlib_u8 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride);

void mlib_v_ImageZoomIn2X_U8_4BL_FL0(
    mlib_u8 *pdst,
    const mlib_u8 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride);

void mlib_v_ImageZoomIn2X_U8_1BL_FL1(
    mlib_u8 *pdst,
    const mlib_u8 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride);

void mlib_v_ImageZoomIn2X_U8_2BL_FL1(
    mlib_u8 *pdst,
    const mlib_u8 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride);

void mlib_v_ImageZoomIn2X_U8_3BL_FL1(
    mlib_u8 *pdst,
    const mlib_u8 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride);

void mlib_v_ImageZoomIn2X_U8_4BL_FL1(
    mlib_u8 *pdst,
    const mlib_u8 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride);

void mlib_v_ImageZoomIn2X_U8_1BL_FL2(
    mlib_u8 *pdst,
    const mlib_u8 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride);

void mlib_v_ImageZoomIn2X_U8_2BL_FL2(
    mlib_u8 *pdst,
    const mlib_u8 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride);

void mlib_v_ImageZoomIn2X_U8_3BL_FL2(
    mlib_u8 *pdst,
    const mlib_u8 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride);

void mlib_v_ImageZoomIn2X_U8_4BL_FL2(
    mlib_u8 *pdst,
    const mlib_u8 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride);

void mlib_v_ImageZoomIn2X_U8_1BL_FL3(
    mlib_u8 *pdst,
    const mlib_u8 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride);

void mlib_v_ImageZoomIn2X_U8_2BL_FL3(
    mlib_u8 *pdst,
    const mlib_u8 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride);

void mlib_v_ImageZoomIn2X_U8_3BL_FL3(
    mlib_u8 *pdst,
    const mlib_u8 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride);

void mlib_v_ImageZoomIn2X_U8_4BL_FL3(
    mlib_u8 *pdst,
    const mlib_u8 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride);

void mlib_v_ImageZoomIn2X_U8_BL_FL0(
    mlib_u8 *pdst,
    const mlib_u8 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    mlib_s32 channels);

void mlib_v_ImageZoomIn2X_U8_BL_FL1(
    mlib_u8 *pdst,
    const mlib_u8 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    mlib_s32 channels);

void mlib_v_ImageZoomIn2X_U8_BL_FL2(
    mlib_u8 *pdst,
    const mlib_u8 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    mlib_s32 channels);

void mlib_v_ImageZoomIn2X_U8_BL_FL3(
    mlib_u8 *pdst,
    const mlib_u8 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    mlib_s32 channels);

mlib_status mlib_v_ImageZoomIn2X_U8_2BL_FL00(
    void *pdst,
    void *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg);

mlib_status mlib_v_ImageZoomIn2X_U8_2BL_FL01(
    void *pdst,
    void *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg);

mlib_status mlib_v_ImageZoomIn2X_U8_2BL_FL10(
    void *pdst,
    void *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg);

mlib_status mlib_v_ImageZoomIn2X_U8_4BL_FL00(
    void *pdst,
    void *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg);

mlib_status mlib_v_ImageZoomIn2X_U8_4BL_FL10(
    void *pdst,
    void *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg);

/* mlib_v_ImageZoomIn2X_BL_S16.c */

void mlib_v_ImageZoomIn2X_S16_1BL_FL0(
    mlib_s16 *pdst,
    const mlib_s16 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride);

void mlib_v_ImageZoomIn2X_S16_2BL_FL0(
    mlib_s16 *pdst,
    const mlib_s16 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride);

void mlib_v_ImageZoomIn2X_S16_3BL_FL0(
    mlib_s16 *pdst,
    const mlib_s16 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride);

void mlib_v_ImageZoomIn2X_S16_4BL_FL0(
    mlib_s16 *pdst,
    const mlib_s16 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride);

void mlib_v_ImageZoomIn2X_S16_1BL_FL1(
    mlib_s16 *pdst,
    const mlib_s16 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride);

void mlib_v_ImageZoomIn2X_S16_2BL_FL1(
    mlib_s16 *pdst,
    const mlib_s16 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride);

void mlib_v_ImageZoomIn2X_S16_4BL_FL1(
    mlib_s16 *pdst,
    const mlib_s16 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride);

void mlib_v_ImageZoomIn2X_S16_1BL_FL2(
    mlib_s16 *pdst,
    const mlib_s16 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride);

void mlib_v_ImageZoomIn2X_S16_2BL_FL2(
    mlib_s16 *pdst,
    const mlib_s16 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride);

void mlib_v_ImageZoomIn2X_S16_3BL_FL2(
    mlib_s16 *pdst,
    const mlib_s16 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride);

void mlib_v_ImageZoomIn2X_S16_4BL_FL2(
    mlib_s16 *pdst,
    const mlib_s16 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride);

void mlib_v_ImageZoomIn2X_S16_1BL_FL3(
    mlib_s16 *pdst,
    const mlib_s16 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride);

void mlib_v_ImageZoomIn2X_S16_2BL_FL3(
    mlib_s16 *pdst,
    const mlib_s16 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride);

void mlib_v_ImageZoomIn2X_S16_3BL_FL3(
    mlib_s16 *pdst,
    const mlib_s16 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride);

void mlib_v_ImageZoomIn2X_S16_4BL_FL3(
    mlib_s16 *pdst,
    const mlib_s16 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride);

void mlib_v_ImageZoomIn2X_S16_BL_FL0(
    mlib_s16 *pdst,
    const mlib_s16 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    mlib_s32 channels);

void mlib_v_ImageZoomIn2X_S16_BL_FL1(
    mlib_s16 *pdst,
    const mlib_s16 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    mlib_s32 channels);

void mlib_v_ImageZoomIn2X_S16_BL_FL2(
    mlib_s16 *pdst,
    const mlib_s16 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    mlib_s32 channels);

void mlib_v_ImageZoomIn2X_S16_BL_FL3(
    mlib_s16 *pdst,
    const mlib_s16 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    mlib_s32 channels);

mlib_status mlib_v_ImageZoomIn2X_S16_2BL_FL00(
    void *pdst,
    void *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg);

mlib_status mlib_v_ImageZoomIn2X_S16_2BL_FL01(
    void *pdst,
    void *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg);

mlib_status mlib_v_ImageZoomIn2X_S16_2BL_FL10(
    void *pdst,
    void *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg);

mlib_status mlib_v_ImageZoomIn2X_S16_4BL_FL00(
    void *pdst,
    void *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg);

mlib_status mlib_v_ImageZoomIn2X_S16_4BL_FL10(
    void *pdst,
    void *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg);

/* mlib_v_ImageZoomIn2X_BL_U16.c */

void mlib_v_ImageZoomIn2X_U16_1BL_FL0(
    mlib_u16 *pdst,
    const mlib_u16 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride);

void mlib_v_ImageZoomIn2X_U16_2BL_FL0(
    mlib_u16 *pdst,
    const mlib_u16 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride);

void mlib_v_ImageZoomIn2X_U16_3BL_FL0(
    mlib_u16 *pdst,
    const mlib_u16 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride);

void mlib_v_ImageZoomIn2X_U16_4BL_FL0(
    mlib_u16 *pdst,
    const mlib_u16 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride);

void mlib_v_ImageZoomIn2X_U16_1BL_FL1(
    mlib_u16 *pdst,
    const mlib_u16 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride);

void mlib_v_ImageZoomIn2X_U16_2BL_FL1(
    mlib_u16 *pdst,
    const mlib_u16 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride);

void mlib_v_ImageZoomIn2X_U16_4BL_FL1(
    mlib_u16 *pdst,
    const mlib_u16 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride);

void mlib_v_ImageZoomIn2X_U16_1BL_FL2(
    mlib_u16 *pdst,
    const mlib_u16 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride);

void mlib_v_ImageZoomIn2X_U16_2BL_FL2(
    mlib_u16 *pdst,
    const mlib_u16 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride);

void mlib_v_ImageZoomIn2X_U16_3BL_FL2(
    mlib_u16 *pdst,
    const mlib_u16 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride);

void mlib_v_ImageZoomIn2X_U16_4BL_FL2(
    mlib_u16 *pdst,
    const mlib_u16 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride);

void mlib_v_ImageZoomIn2X_U16_1BL_FL3(
    mlib_u16 *pdst,
    const mlib_u16 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride);

void mlib_v_ImageZoomIn2X_U16_2BL_FL3(
    mlib_u16 *pdst,
    const mlib_u16 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride);

void mlib_v_ImageZoomIn2X_U16_3BL_FL3(
    mlib_u16 *pdst,
    const mlib_u16 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride);

void mlib_v_ImageZoomIn2X_U16_4BL_FL3(
    mlib_u16 *pdst,
    const mlib_u16 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride);

void mlib_v_ImageZoomIn2X_U16_BL_FL0(
    mlib_u16 *pdst,
    const mlib_u16 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    mlib_s32 channels);

void mlib_v_ImageZoomIn2X_U16_BL_FL1(
    mlib_u16 *pdst,
    const mlib_u16 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    mlib_s32 channels);

void mlib_v_ImageZoomIn2X_U16_BL_FL2(
    mlib_u16 *pdst,
    const mlib_u16 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    mlib_s32 channels);

void mlib_v_ImageZoomIn2X_U16_BL_FL3(
    mlib_u16 *pdst,
    const mlib_u16 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    mlib_s32 channels);

mlib_status mlib_v_ImageZoomIn2X_U16_2BL_FL00(
    void *pdst,
    void *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg);

mlib_status mlib_v_ImageZoomIn2X_U16_2BL_FL01(
    void *pdst,
    void *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg);

mlib_status mlib_v_ImageZoomIn2X_U16_2BL_FL10(
    void *pdst,
    void *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg);

mlib_status mlib_v_ImageZoomIn2X_U16_4BL_FL00(
    void *pdst,
    void *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg);

mlib_status mlib_v_ImageZoomIn2X_U16_4BL_FL10(
    void *pdst,
    void *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg);

/* mlib_v_ImageZoomIn2X_NN.c */

void mlib_v_ImageZoomIn2X_U8_NN(
    mlib_u8 *pdst,
    mlib_u8 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    mlib_s32 channels);

void mlib_v_ImageZoomIn2X_S16_NN(
    mlib_s16 *pdst,
    mlib_s16 *psrc,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 dst_stride,
    mlib_s32 src_stride,
    mlib_s32 src_w_beg,
    mlib_s32 src_h_beg,
    mlib_s32 channels);

/* mlib_v_ImageZoomIn2X.c */

void mlib_ImageMerge3ch_S16(
    const void *src0,
    const void *src1,
    void *dst,
    mlib_s32 n);

void mlib_ImageMerge3ch_U8(
    const void *src0,
    const void *src1,
    void *dst,
    mlib_s32 n);

#ifdef __cplusplus
}
#endif

#endif /* _MLIB_V_IMAGEZOOMIN2X_PROTO_H */
