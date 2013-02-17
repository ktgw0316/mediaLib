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

#ifndef _MLIB_IMAGEAFFINETABLE_H
#define	_MLIB_IMAGEAFFINETABLE_H

#pragma ident	"@(#)mlib_ImageAffineTable.h	9.2	07/10/09 SMI"

#ifdef __cplusplus
extern "C" {
#endif

#include <mlib_image.h>

/*
 * Internal structure and types
 */
/* *********************************************************** */
    typedef struct
{
	mlib_s32 type;
	mlib_u8 *srcData;
	mlib_u8 *dstData;
	mlib_s32 *sides;
	mlib_s32 srcWidth;
	mlib_s32 srcHeight;
	mlib_s32 srcStride;
	mlib_s32 dstStride;
	mlib_s32 nchan;
	mlib_edge edge;
	mlib_s32 yStart;
	mlib_s32 yFinish;
	mlib_s32 dx;
	mlib_s32 dy;
	mlib_s32 max_xsize;
	mlib_s32 x_move;
	mlib_s32 y_move;
	mlib_s32 x_shift0;
	mlib_s32 x_shift;
	mlib_s32 y_shift;
	mlib_s32 x_mask;
	mlib_s32 xf_shift;
	mlib_s32 xf_mask;
	mlib_s32 yf_shift;
	mlib_s32 yf_mask;
	mlib_s32 size;
	mlib_s32 x0;
	mlib_s32 y0;
	mlib_s32 k;
	mlib_s32 b_step;
	mlib_s32 affine_mask;
} mlib_affine_workspace;

/* *********************************************************** */
#define	PARAMS_NW                                                          \
	mlib_u8 *dstData, const mlib_u8 **lineAddr, mlib_s32 *leftEdges,   \
	mlib_s32 *rightEdges, mlib_s32 *xStarts, mlib_s32 *yStarts,        \
	mlib_affine_workspace *ws, mlib_interp_table *table

/* *********************************************************** */
#define	PARAMS_EXT                                                     \
	mlib_u8 *dstData, const mlib_u8 **lineAddr, mlib_s32 *x_ind,   \
	mlib_s32 *leftEdges, mlib_s32 *rightEdges,                     \
	mlib_s32 *xStarts, mlib_s32 *yStarts,                          \
	mlib_affine_workspace *ws, mlib_interp_table *table

mlib_status mlib_ImageAffineTable_8nw(PARAMS_NW);
mlib_status mlib_ImageAffineTable_8_3nw(PARAMS_NW);
mlib_status mlib_ImageAffineTable_8ext(PARAMS_EXT);
mlib_status mlib_ImageAffineTable_16nw(PARAMS_NW);
mlib_status mlib_ImageAffineTable_16_3nw(PARAMS_NW);
mlib_status mlib_ImageAffineTable_16ext(PARAMS_EXT);
mlib_status mlib_ImageAffineTable_u16nw(PARAMS_NW);
mlib_status mlib_ImageAffineTable_u16_3nw(PARAMS_NW);
mlib_status mlib_ImageAffineTable_u16ext(PARAMS_EXT);
mlib_status mlib_ImageAffineTable_32nw(PARAMS_NW);
mlib_status mlib_ImageAffineTable_32_3nw(PARAMS_NW);
mlib_status mlib_ImageAffineTable_32ext(PARAMS_EXT);
mlib_status mlib_ImageAffineTable_F32nw(PARAMS_NW);
mlib_status mlib_ImageAffineTable_F32_3nw(PARAMS_NW);
mlib_status mlib_ImageAffineTable_F32ext(PARAMS_EXT);
mlib_status mlib_ImageAffineTable_D64nw(PARAMS_NW);
mlib_status mlib_ImageAffineTable_D64_3nw(PARAMS_NW);
mlib_status mlib_ImageAffineTable_D64ext(PARAMS_EXT);

mlib_status mlib_v_ImageAffineTable_8nw(PARAMS_NW);

typedef mlib_status(*fun_type_nw) (
    PARAMS_NW);
typedef mlib_status(*fun_type_ext) (
    PARAMS_EXT);

/* *********************************************************** */

fun_type_nw mlib_ImageAffine_GetFunc(
    mlib_affine_workspace *ws,
    mlib_interp_table *tbl);

fun_type_nw mlib_ImageAffine_ConvertImage(
    const mlib_image *src,
    mlib_affine_workspace *ws,
    mlib_interp_table *tbl);

/* *********************************************************** */

mlib_status mlib_ImageAffineTable_alltypes(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_d64 *mtx,
    const void *table,
    mlib_edge edge);

/* *********************************************************** */

/* mlib_ImageAffineTable_F32nw_2.c */

void mlib_ImageAffineTableLine_F32nw_c2_2_4(
    mlib_f32 *buff,
    const mlib_f32 *filterX,
    const mlib_f32 *filterY,
    const mlib_u8 **lineAddr,
    mlib_affine_workspace *ws);

void mlib_ImageAffineTableLine_F32nw_c3_2_2(
    mlib_f32 *buff,
    const mlib_f32 *filterX,
    const mlib_f32 *filterY,
    const mlib_u8 **lineAddr,
    mlib_affine_workspace *ws);

void mlib_ImageAffineTableLine_F32nw_c4_2_4(
    mlib_f32 *buff,
    const mlib_f32 *filterX,
    const mlib_f32 *filterY,
    const mlib_u8 **lineAddr,
    mlib_affine_workspace *ws);

/* *********************************************************** */

#ifdef __cplusplus
}
#endif

#endif /* _MLIB_IMAGEAFFINETABLE_H */
