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

#ifndef _MLIB_IMAGEZOOMTRANSLATETABLE_H
#define	_MLIB_IMAGEZOOMTRANSLATETABLE_H

#pragma ident	"@(#)mlib_ImageZoomTranslateTable.h	9.4	07/10/09 SMI"

#ifdef __cplusplus
extern "C" {
#endif

#include <mlib_types.h>
#include <mlib_image_types.h>
#include <mlib_status.h>
#include <mlib_ImageInterpTable.h>

/* *********************************************************** */

#undef	FP_DIV

#if MLIB_VIS < 0x200

#define	FP_DIV(rw, x, y)	rw = (y < ZERO_EDGE) ? 0 : x/y

#else

#define	FP_EPS	1e-35f

#define	FP_DIV(rw, x, y)	rw = (y < ZERO_EDGE) ? 0 : x/(y + FP_EPS)

#endif

#define	FP_INV(rw, w)	FP_DIV(rw, 1, w)

/*
 * Internal structure and types
 */
/* *********************************************************** */

    typedef struct
{
	mlib_type type;
	mlib_u8 *srcData;
	mlib_u8 *dstData;
	mlib_s32 *x_ind;
	mlib_s32 srcWidth;
	mlib_s32 srcHeight;
	mlib_s32 srcStride;
	mlib_s32 dstStride;
	mlib_s32 nchan;
	mlib_s32 dchan;
	mlib_s32 alpha_shift;
	mlib_blend blend;
	mlib_edge edge;
	mlib_s32 yStart;
	mlib_s32 yFinish;
	mlib_s32 dx;
	mlib_s32 dy;
	mlib_s32 max_xsize;
	mlib_s32 x_move;
	mlib_s32 y_move;
	mlib_s32 x_shift;
	mlib_s32 y_shift;
	mlib_s32 x_mask;
	mlib_s32 x_mask0;
	mlib_s32 xf_shift;
	mlib_s32 xf_mask;
	mlib_s32 yf_shift;
	mlib_s32 yf_mask;
	mlib_s32 size;
	mlib_s32 size2;
	mlib_s32 x0;
	mlib_s32 y0;
	mlib_s32 off;
	mlib_s32 y_step;
	mlib_d64 zoomx;
	mlib_d64 zoomy;
} mlib_zoom_workspace;

/* *********************************************************** */

#define	PARAMS_NW                                                     \
	mlib_u8 *dstData, mlib_u8 **lineAddr, mlib_s32 *leftEdges,    \
	mlib_s32 *rightEdges, mlib_s32 *xStarts, mlib_s32 *yStarts,   \
	mlib_zoom_workspace * ws, mlib_interp_table * table

/* *********************************************************** */

#define	PARAMS_EXT                                                \
	mlib_u8 *dstData, mlib_u8 **lineAddr, mlib_s32 *x_ind,    \
	mlib_s32 *leftEdges, mlib_s32 *rightEdges,                \
	mlib_s32 *xStarts, mlib_s32 *yStarts,                     \
	mlib_zoom_workspace * ws, mlib_interp_table * table

/* *********************************************************** */

#define	VPARAMS                                                 \
	mlib_d64 *fptr, mlib_d64 **buff_arr, mlib_s32 bnd,      \
	mlib_d64 *ybuff, mlib_s32 size

#define	VPARAMSS	VPARAMS, mlib_d64 dsat, mlib_f32 *dl

/* *********************************************************** */

#define	VPARAMH                                                        \
	mlib_s32 x, FILTER_TYPE * filterX, mlib_zoom_workspace * ws,   \
	FP_TYPE * pbuff, mlib_f32 *sp0

/* *********************************************************** */

#define	VPARAMXY                                                       \
	mlib_s32 x, FILTER_TYPE * filterX, mlib_zoom_workspace * ws,   \
	FP_TYPE * xb

/* *********************************************************** */

mlib_status mlib_ImageZoomTranslateTable_8nw(
    PARAMS_NW);
mlib_status mlib_ImageZoomTranslateTable_8ext(
    PARAMS_EXT);
mlib_status mlib_ImageZoomTranslateTable_16nw(
    PARAMS_NW);
mlib_status mlib_ImageZoomTranslateTable_16ext(
    PARAMS_EXT);
mlib_status mlib_ImageZoomTranslateTable_u16nw(
    PARAMS_NW);
mlib_status mlib_ImageZoomTranslateTable_u16ext(
    PARAMS_EXT);
mlib_status mlib_ImageZoomTranslateTable_32nw(
    PARAMS_NW);
mlib_status mlib_ImageZoomTranslateTable_32ext(
    PARAMS_EXT);
mlib_status mlib_ImageZoomTranslateTable_F32nw(
    PARAMS_NW);
mlib_status mlib_ImageZoomTranslateTable_F32ext(
    PARAMS_EXT);
mlib_status mlib_ImageZoomTranslateTable_D64nw(
    PARAMS_NW);
mlib_status mlib_ImageZoomTranslateTable_D64ext(
    PARAMS_EXT);

mlib_status mlib_ImageZoomTranslateTableBlend_8nw(
    PARAMS_NW);
mlib_status mlib_ImageZoomTranslateTableBlend_8ext(
    PARAMS_EXT);

mlib_status mlib_v_ImageZoomTranslateTable_1_8nw(
    PARAMS_NW);
mlib_status mlib_v_ImageZoomTranslateTable_2_8nw(
    PARAMS_NW);
mlib_status mlib_v_ImageZoomTranslateTable_3_8nw(
    PARAMS_NW);
mlib_status mlib_v_ImageZoomTranslateTable_4_8nw(
    PARAMS_NW);

void mlib_calc_y4(
    VPARAMS);
void mlib_calc_y3(
    VPARAMS);
void mlib_calc_y2(
    VPARAMS);
void mlib_calc_y1(
    VPARAMS);

void mlib_calc_y4c(
    VPARAMSS);
void mlib_calc_y3c(
    VPARAMSS);
void mlib_calc_y2c(
    VPARAMSS);
void mlib_calc_y1c(
    VPARAMSS);

typedef mlib_status (*fun_type_nw) (
    PARAMS_NW);
typedef mlib_status (*fun_type_ext) (
    PARAMS_EXT);

/* *********************************************************** */

mlib_status mlib_ImageZoomTranslateTable_alltypes(
    mlib_image *dst,
    const mlib_image *src,
    mlib_d64 zoomx,
    mlib_d64 zoomy,
    mlib_d64 tx,
    mlib_d64 ty,
    const void *table,
    mlib_edge edge);

fun_type_nw mlib_ImageZoomTranslate_GetFunc();

/* *********************************************************** */

#ifdef __cplusplus
}
#endif

#endif /* _MLIB_IMAGEZOOMTRANSLATETABLE_H */
