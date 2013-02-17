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

#ifndef _MLIB_IMAGEPOLYNOMIALWARP_H
#define	_MLIB_IMAGEPOLYNOMIALWARP_H

#pragma ident	"@(#)mlib_ImagePolynomialWarp.h	9.3	07/10/09 SMI"

#ifdef __cplusplus
extern "C" {
#endif

#include <mlib_types.h>
#include <mlib_image_types.h>
#include <mlib_status.h>
#include <mlib_ImageDivTables.h>

/* *********************************************************** */

#define	MLIB_SHIFT	16
#define	MLIB_PREC	(1 << MLIB_SHIFT)
#define	MLIB_MASK	(MLIB_PREC - 1)

/* *********************************************************** */
    typedef struct
{
	mlib_d64 SrcStartX, SrcStartY;
	mlib_d64 SrcStopX, SrcStopY;
	void *xCoeffs, *yCoeffs;
	void *dsrc_x, *dsrc_y;
	void *src_x, *src_y;
	void *dst_x, *buffer;
	void *filter_table;
	void *filter_table_vis;
	mlib_s32 flags;
/* PolynomialWarpTable */
	mlib_d64 scaleH;
	mlib_d64 scaleV;
	void *tableH;
	void *tableV;
	void *list_call_function;
	mlib_s32 degree;
	mlib_s32 shiftH;
	mlib_s32 shiftV;
	mlib_s32 channels;
	mlib_s32 call_count;
} mlib_PWS;

/* *********************************************************** */

typedef void (*mlib_IPWFSat) (
    void *,
    mlib_PWS *,
    mlib_s32);

typedef void (*mlib_IPWFCall) (
    mlib_u8 *,
    const mlib_u8 **,
    mlib_PWS *,
    mlib_s32);

typedef mlib_s32 (*mlib_IPWClipLine) (
    mlib_PWS *,
    mlib_d64,
    mlib_d64,
    mlib_s32,
    mlib_s32);

typedef void (*mlib_IPWFCall_NN) (
    mlib_u8 *,
    const mlib_u8 **,
    const mlib_d64 *,
    const mlib_d64 *,
    mlib_d64,
    mlib_d64,
    mlib_s32,
    mlib_s32,
    mlib_s32,
    mlib_s32,
    mlib_s32);

typedef void (*mlib_IPWFTCall) (
    void *,
    const mlib_u8 **,
    mlib_PWS *,
    mlib_s32,
    mlib_s32,
    mlib_s32);

/* *********************************************************** */

typedef struct
{
	mlib_IPWFTCall func;
	mlib_s32 offH, offV;
} mlib_IPWT_CF;

/* *********************************************************** */

void *mlib_IPWT_get_call_func(
    mlib_s32 w,
    mlib_s32 h,
    mlib_s32 type,
    mlib_s32 *count);

/* *********************************************************** */

void *mlib_ImagePolynimialWarpCoeffsPrepare(
    const mlib_d64 *xCoeffs,
    const mlib_d64 *yCoeffs,
    mlib_d64 preScaleX,
    mlib_d64 preScaleY,
    mlib_d64 postScaleX,
    mlib_d64 postScaleY,
    mlib_d64 postShiftX,
    mlib_d64 postShiftY,
    mlib_s32 degree);

/* *********************************************************** */

#define	DEFINE_CLIP_FUNC(NAME)                                  \
	mlib_s32                                                \
	NAME(mlib_PWS * pws, mlib_d64 y, mlib_d64 preShiftX,    \
	    mlib_s32 dstWidth, mlib_s32 degree)

DEFINE_CLIP_FUNC(mlib_ImagePolynomialWarpClipLine_0);
DEFINE_CLIP_FUNC(mlib_ImagePolynomialWarpClipLine_1);
DEFINE_CLIP_FUNC(mlib_ImagePolynomialWarpClipLine_2);
DEFINE_CLIP_FUNC(mlib_ImagePolynomialWarpClipLine_3);
DEFINE_CLIP_FUNC(mlib_ImagePolynomialWarpClipLine_10);

DEFINE_CLIP_FUNC(mlib_ImagePolynomialWarpClipLine_DG_1_10);

DEFINE_CLIP_FUNC(mlib_ImagePolynomialWarpClipLine_DG_2_0);
DEFINE_CLIP_FUNC(mlib_ImagePolynomialWarpClipLine_DG_2_1);
DEFINE_CLIP_FUNC(mlib_ImagePolynomialWarpClipLine_DG_2_2);
DEFINE_CLIP_FUNC(mlib_ImagePolynomialWarpClipLine_DG_2_10);

DEFINE_CLIP_FUNC(mlib_ImagePolynomialWarpClipLine_DG_3_0);
DEFINE_CLIP_FUNC(mlib_ImagePolynomialWarpClipLine_DG_3_1);
DEFINE_CLIP_FUNC(mlib_ImagePolynomialWarpClipLine_DG_3_2);
DEFINE_CLIP_FUNC(mlib_ImagePolynomialWarpClipLine_DG_3_10);

DEFINE_CLIP_FUNC(mlib_ImagePolynomialWarpClipLine_DG_4_0);
DEFINE_CLIP_FUNC(mlib_ImagePolynomialWarpClipLine_DG_4_1);
DEFINE_CLIP_FUNC(mlib_ImagePolynomialWarpClipLine_DG_4_2);
DEFINE_CLIP_FUNC(mlib_ImagePolynomialWarpClipLine_DG_4_10);

DEFINE_CLIP_FUNC(mlib_ImagePolynomialWarpClipLine_DG_5_0);
DEFINE_CLIP_FUNC(mlib_ImagePolynomialWarpClipLine_DG_5_1);
DEFINE_CLIP_FUNC(mlib_ImagePolynomialWarpClipLine_DG_5_2);
DEFINE_CLIP_FUNC(mlib_ImagePolynomialWarpClipLine_DG_5_10);

/* *********************************************************** */

#define	DEFINE_FUNC_0(NAME)                                     \
	mlib_status                                             \
	NAME(mlib_image *dst, const mlib_image *src,            \
	    const mlib_d64 *xCoeffs, const mlib_d64 *yCoeffs,   \
	    mlib_d64 postShiftX, mlib_d64 postShiftY,           \
	    mlib_d64 postScaleX, mlib_d64 postScaleY,           \
	    mlib_filter filter, mlib_edge edge)

DEFINE_FUNC_0(mlib_ImagePolynomialWarp_0);
DEFINE_FUNC_0(mlib_ImagePolynomialWarp_0_Fp);

/* *********************************************************** */

#define	DEFINE_FUNC_1(NAME)                                                \
	mlib_status                                                        \
	NAME(mlib_image *dst, const mlib_image *src,                       \
	    const mlib_d64 *xCoeffs, const mlib_d64 *yCoeffs,              \
	    mlib_d64 preShiftX, mlib_d64 preShiftY, mlib_d64 postShiftX,   \
	    mlib_d64 postShiftY, mlib_d64 preScaleX, mlib_d64 preScaleY,   \
	    mlib_d64 postScaleX, mlib_d64 postScaleY,                      \
	    mlib_filter filter, mlib_edge edge)

DEFINE_FUNC_1(mlib_ImagePolynomialWarp_1);

/* *********************************************************** */

#define	DEFINE_FUNC_NN(NAME)                                              \
	void                                                              \
	NAME(mlib_image *dst, const mlib_image *src,                      \
	    const mlib_u8 **lineAddr, mlib_PWS * pws,                     \
	    const mlib_d64 *xCoeffs, const mlib_d64 *yCoeffs,             \
	    mlib_d64 preShiftX, mlib_d64 preShiftY, mlib_filter filter,   \
	    mlib_edge edge)

DEFINE_FUNC_NN(mlib_ImagePolynomialWarp_2_5);
DEFINE_FUNC_NN(mlib_ImagePolynomialWarp_2_5_Fp);

/* *********************************************************** */

#define	DEF_FUNC_2(NAME)                                                 \
	void                                                             \
	NAME(mlib_u8 *dstData, const mlib_u8 **lineAddr,                 \
	    const mlib_d64 *xCoeffs, const mlib_d64 *yCoeffs,            \
	    mlib_d64 preShiftX, mlib_d64 preShiftY, mlib_s32 srcWidth,   \
	    mlib_s32 srcHeight, mlib_s32 dstWidth, mlib_s32 dstHeight,   \
	    mlib_s32 dstStride)

DEF_FUNC_2(mlib_ImagePolynomialWarp_2_NN_U8_1);
DEF_FUNC_2(mlib_ImagePolynomialWarp_2_NN_U8_2);
DEF_FUNC_2(mlib_ImagePolynomialWarp_2_NN_U8_3);
DEF_FUNC_2(mlib_ImagePolynomialWarp_2_NN_U8_4);
DEF_FUNC_2(mlib_ImagePolynomialWarp_2_NN_S16_1);
DEF_FUNC_2(mlib_ImagePolynomialWarp_2_NN_S16_2);
DEF_FUNC_2(mlib_ImagePolynomialWarp_2_NN_S16_3);
DEF_FUNC_2(mlib_ImagePolynomialWarp_2_NN_S16_4);
DEF_FUNC_2(mlib_ImagePolynomialWarp_2_NN_S32_1);
DEF_FUNC_2(mlib_ImagePolynomialWarp_2_NN_S32_2);
DEF_FUNC_2(mlib_ImagePolynomialWarp_2_NN_S32_3);
DEF_FUNC_2(mlib_ImagePolynomialWarp_2_NN_S32_4);
DEF_FUNC_2(mlib_ImagePolynomialWarp_2_NN_D64_1);
DEF_FUNC_2(mlib_ImagePolynomialWarp_2_NN_D64_2);
DEF_FUNC_2(mlib_ImagePolynomialWarp_2_NN_D64_3);
DEF_FUNC_2(mlib_ImagePolynomialWarp_2_NN_D64_4);

DEF_FUNC_2(mlib_ImagePolynomialWarp_3_NN_U8_1);
DEF_FUNC_2(mlib_ImagePolynomialWarp_3_NN_U8_2);
DEF_FUNC_2(mlib_ImagePolynomialWarp_3_NN_U8_3);
DEF_FUNC_2(mlib_ImagePolynomialWarp_3_NN_U8_4);
DEF_FUNC_2(mlib_ImagePolynomialWarp_3_NN_S16_1);
DEF_FUNC_2(mlib_ImagePolynomialWarp_3_NN_S16_2);
DEF_FUNC_2(mlib_ImagePolynomialWarp_3_NN_S16_3);
DEF_FUNC_2(mlib_ImagePolynomialWarp_3_NN_S16_4);
DEF_FUNC_2(mlib_ImagePolynomialWarp_3_NN_S32_1);
DEF_FUNC_2(mlib_ImagePolynomialWarp_3_NN_S32_2);
DEF_FUNC_2(mlib_ImagePolynomialWarp_3_NN_S32_3);
DEF_FUNC_2(mlib_ImagePolynomialWarp_3_NN_S32_4);
DEF_FUNC_2(mlib_ImagePolynomialWarp_3_NN_D64_1);
DEF_FUNC_2(mlib_ImagePolynomialWarp_3_NN_D64_2);
DEF_FUNC_2(mlib_ImagePolynomialWarp_3_NN_D64_3);
DEF_FUNC_2(mlib_ImagePolynomialWarp_3_NN_D64_4);

DEF_FUNC_2(mlib_ImagePolynomialWarp_4_NN_U8_1);
DEF_FUNC_2(mlib_ImagePolynomialWarp_4_NN_U8_2);
DEF_FUNC_2(mlib_ImagePolynomialWarp_4_NN_U8_3);
DEF_FUNC_2(mlib_ImagePolynomialWarp_4_NN_U8_4);
DEF_FUNC_2(mlib_ImagePolynomialWarp_4_NN_S16_1);
DEF_FUNC_2(mlib_ImagePolynomialWarp_4_NN_S16_2);
DEF_FUNC_2(mlib_ImagePolynomialWarp_4_NN_S16_3);
DEF_FUNC_2(mlib_ImagePolynomialWarp_4_NN_S16_4);
DEF_FUNC_2(mlib_ImagePolynomialWarp_4_NN_S32_1);
DEF_FUNC_2(mlib_ImagePolynomialWarp_4_NN_S32_2);
DEF_FUNC_2(mlib_ImagePolynomialWarp_4_NN_S32_3);
DEF_FUNC_2(mlib_ImagePolynomialWarp_4_NN_S32_4);
DEF_FUNC_2(mlib_ImagePolynomialWarp_4_NN_D64_1);
DEF_FUNC_2(mlib_ImagePolynomialWarp_4_NN_D64_2);
DEF_FUNC_2(mlib_ImagePolynomialWarp_4_NN_D64_3);
DEF_FUNC_2(mlib_ImagePolynomialWarp_4_NN_D64_4);

DEF_FUNC_2(mlib_ImagePolynomialWarp_5_NN_U8_1);
DEF_FUNC_2(mlib_ImagePolynomialWarp_5_NN_U8_2);
DEF_FUNC_2(mlib_ImagePolynomialWarp_5_NN_U8_3);
DEF_FUNC_2(mlib_ImagePolynomialWarp_5_NN_U8_4);
DEF_FUNC_2(mlib_ImagePolynomialWarp_5_NN_S16_1);
DEF_FUNC_2(mlib_ImagePolynomialWarp_5_NN_S16_2);
DEF_FUNC_2(mlib_ImagePolynomialWarp_5_NN_S16_3);
DEF_FUNC_2(mlib_ImagePolynomialWarp_5_NN_S16_4);
DEF_FUNC_2(mlib_ImagePolynomialWarp_5_NN_S32_1);
DEF_FUNC_2(mlib_ImagePolynomialWarp_5_NN_S32_2);
DEF_FUNC_2(mlib_ImagePolynomialWarp_5_NN_S32_3);
DEF_FUNC_2(mlib_ImagePolynomialWarp_5_NN_S32_4);
DEF_FUNC_2(mlib_ImagePolynomialWarp_5_NN_D64_1);
DEF_FUNC_2(mlib_ImagePolynomialWarp_5_NN_D64_2);
DEF_FUNC_2(mlib_ImagePolynomialWarp_5_NN_D64_3);
DEF_FUNC_2(mlib_ImagePolynomialWarp_5_NN_D64_4);

/* *********************************************************** */

#define	DEFINE_FUNC(NAME)                                                  \
	void                                                               \
	NAME(mlib_u8 *dstData, const mlib_u8 **lineAddr, mlib_PWS * pws,   \
	    mlib_s32 n)
/*
 * Nearest neighbor mode
 */
DEFINE_FUNC(mlib_ImagePolynomialWarp_NN_U8_1);
DEFINE_FUNC(mlib_ImagePolynomialWarp_NN_U8_2);
DEFINE_FUNC(mlib_ImagePolynomialWarp_NN_U8_3);
DEFINE_FUNC(mlib_ImagePolynomialWarp_NN_U8_4);
DEFINE_FUNC(mlib_ImagePolynomialWarp_NN_S16_1);
DEFINE_FUNC(mlib_ImagePolynomialWarp_NN_S16_2);
DEFINE_FUNC(mlib_ImagePolynomialWarp_NN_S16_3);
DEFINE_FUNC(mlib_ImagePolynomialWarp_NN_S16_4);
DEFINE_FUNC(mlib_ImagePolynomialWarp_NN_S32_1);
DEFINE_FUNC(mlib_ImagePolynomialWarp_NN_S32_2);
DEFINE_FUNC(mlib_ImagePolynomialWarp_NN_S32_3);
DEFINE_FUNC(mlib_ImagePolynomialWarp_NN_S32_4);
DEFINE_FUNC(mlib_ImagePolynomialWarp_NN_D64_1);
DEFINE_FUNC(mlib_ImagePolynomialWarp_NN_D64_2);
DEFINE_FUNC(mlib_ImagePolynomialWarp_NN_D64_3);
DEFINE_FUNC(mlib_ImagePolynomialWarp_NN_D64_4);

/*
 * Bilinear mode
 */
DEFINE_FUNC(mlib_ImagePolynomialWarp_BL_U8_1);
DEFINE_FUNC(mlib_ImagePolynomialWarp_BL_U8_2);
DEFINE_FUNC(mlib_ImagePolynomialWarp_BL_U8_3);
DEFINE_FUNC(mlib_ImagePolynomialWarp_BL_U8_4);
DEFINE_FUNC(mlib_ImagePolynomialWarp_BL_S16_1);
DEFINE_FUNC(mlib_ImagePolynomialWarp_BL_S16_2);
DEFINE_FUNC(mlib_ImagePolynomialWarp_BL_S16_3);
DEFINE_FUNC(mlib_ImagePolynomialWarp_BL_S16_4);
DEFINE_FUNC(mlib_ImagePolynomialWarp_BL_U16_1);
DEFINE_FUNC(mlib_ImagePolynomialWarp_BL_U16_2);
DEFINE_FUNC(mlib_ImagePolynomialWarp_BL_U16_3);
DEFINE_FUNC(mlib_ImagePolynomialWarp_BL_U16_4);
DEFINE_FUNC(mlib_ImagePolynomialWarp_BL_S32_1);
DEFINE_FUNC(mlib_ImagePolynomialWarp_BL_S32_2);
DEFINE_FUNC(mlib_ImagePolynomialWarp_BL_S32_3);
DEFINE_FUNC(mlib_ImagePolynomialWarp_BL_S32_4);
DEFINE_FUNC(mlib_ImagePolynomialWarp_BL_F32_1);
DEFINE_FUNC(mlib_ImagePolynomialWarp_BL_F32_2);
DEFINE_FUNC(mlib_ImagePolynomialWarp_BL_F32_3);
DEFINE_FUNC(mlib_ImagePolynomialWarp_BL_F32_4);
DEFINE_FUNC(mlib_ImagePolynomialWarp_BL_D64_1);
DEFINE_FUNC(mlib_ImagePolynomialWarp_BL_D64_2);
DEFINE_FUNC(mlib_ImagePolynomialWarp_BL_D64_3);
DEFINE_FUNC(mlib_ImagePolynomialWarp_BL_D64_4);

/*
 * Bicubic mode
 */
DEFINE_FUNC(mlib_ImagePolynomialWarp_BC_U8_1);
DEFINE_FUNC(mlib_ImagePolynomialWarp_BC_U8_2);
DEFINE_FUNC(mlib_ImagePolynomialWarp_BC_U8_3);
DEFINE_FUNC(mlib_ImagePolynomialWarp_BC_U8_4);
DEFINE_FUNC(mlib_ImagePolynomialWarp_BC_S16_1);
DEFINE_FUNC(mlib_ImagePolynomialWarp_BC_S16_2);
DEFINE_FUNC(mlib_ImagePolynomialWarp_BC_S16_3);
DEFINE_FUNC(mlib_ImagePolynomialWarp_BC_S16_4);
DEFINE_FUNC(mlib_ImagePolynomialWarp_BC_U16_1);
DEFINE_FUNC(mlib_ImagePolynomialWarp_BC_U16_2);
DEFINE_FUNC(mlib_ImagePolynomialWarp_BC_U16_3);
DEFINE_FUNC(mlib_ImagePolynomialWarp_BC_U16_4);
DEFINE_FUNC(mlib_ImagePolynomialWarp_BC_S32_1);
DEFINE_FUNC(mlib_ImagePolynomialWarp_BC_S32_2);
DEFINE_FUNC(mlib_ImagePolynomialWarp_BC_S32_3);
DEFINE_FUNC(mlib_ImagePolynomialWarp_BC_S32_4);
DEFINE_FUNC(mlib_ImagePolynomialWarp_BC_F32_1);
DEFINE_FUNC(mlib_ImagePolynomialWarp_BC_F32_2);
DEFINE_FUNC(mlib_ImagePolynomialWarp_BC_F32_3);
DEFINE_FUNC(mlib_ImagePolynomialWarp_BC_F32_4);
DEFINE_FUNC(mlib_ImagePolynomialWarp_BC_D64_1);
DEFINE_FUNC(mlib_ImagePolynomialWarp_BC_D64_2);
DEFINE_FUNC(mlib_ImagePolynomialWarp_BC_D64_3);
DEFINE_FUNC(mlib_ImagePolynomialWarp_BC_D64_4);

/*
 * Define function for Table mode
 */
DEFINE_FUNC(mlib_ImagePolynomialWarpTable_Line);
void mlib_IPWT_U8_Sat(
    void *,
    mlib_PWS *,
    mlib_s32);
void mlib_IPWT_S16_Sat(
    void *,
    mlib_PWS *,
    mlib_s32);
void mlib_IPWT_U16_Sat(
    void *,
    mlib_PWS *,
    mlib_s32);
void mlib_IPWT_S32_Sat(
    void *,
    mlib_PWS *,
    mlib_s32);
void mlib_IPWT_F32_Sat(
    void *,
    mlib_PWS *,
    mlib_s32);
void mlib_IPWT_D64_Sat(
    void *,
    mlib_PWS *,
    mlib_s32);

/* *********************************************************** */

#define	DEFINE_FUNC_KERNEL(NAME)                                        \
	void                                                            \
	NAME(void *dstData, const mlib_u8 **lineAddr, mlib_PWS * pws,   \
	    mlib_s32 n, mlib_s32 offx, mlib_s32 offy)

DEFINE_FUNC_KERNEL(mlib_IPWT_U8_4x1);
DEFINE_FUNC_KERNEL(mlib_IPWT_U8_4x2);
DEFINE_FUNC_KERNEL(mlib_IPWT_U8_4x3);
DEFINE_FUNC_KERNEL(mlib_IPWT_U8_4x4);
DEFINE_FUNC_KERNEL(mlib_IPWT_U8_3x1);
DEFINE_FUNC_KERNEL(mlib_IPWT_U8_3x2);
DEFINE_FUNC_KERNEL(mlib_IPWT_U8_3x3);
DEFINE_FUNC_KERNEL(mlib_IPWT_U8_3x4);
DEFINE_FUNC_KERNEL(mlib_IPWT_U8_2x1);
DEFINE_FUNC_KERNEL(mlib_IPWT_U8_2x2);
DEFINE_FUNC_KERNEL(mlib_IPWT_U8_2x3);
DEFINE_FUNC_KERNEL(mlib_IPWT_U8_2x4);

DEFINE_FUNC_KERNEL(mlib_IPWT_S16_4x1);
DEFINE_FUNC_KERNEL(mlib_IPWT_S16_4x2);
DEFINE_FUNC_KERNEL(mlib_IPWT_S16_4x3);
DEFINE_FUNC_KERNEL(mlib_IPWT_S16_4x4);
DEFINE_FUNC_KERNEL(mlib_IPWT_S16_3x1);
DEFINE_FUNC_KERNEL(mlib_IPWT_S16_3x2);
DEFINE_FUNC_KERNEL(mlib_IPWT_S16_3x3);
DEFINE_FUNC_KERNEL(mlib_IPWT_S16_3x4);
DEFINE_FUNC_KERNEL(mlib_IPWT_S16_2x1);
DEFINE_FUNC_KERNEL(mlib_IPWT_S16_2x2);
DEFINE_FUNC_KERNEL(mlib_IPWT_S16_2x3);
DEFINE_FUNC_KERNEL(mlib_IPWT_S16_2x4);

DEFINE_FUNC_KERNEL(mlib_IPWT_U16_4x1);
DEFINE_FUNC_KERNEL(mlib_IPWT_U16_4x2);
DEFINE_FUNC_KERNEL(mlib_IPWT_U16_4x3);
DEFINE_FUNC_KERNEL(mlib_IPWT_U16_4x4);
DEFINE_FUNC_KERNEL(mlib_IPWT_U16_3x1);
DEFINE_FUNC_KERNEL(mlib_IPWT_U16_3x2);
DEFINE_FUNC_KERNEL(mlib_IPWT_U16_3x3);
DEFINE_FUNC_KERNEL(mlib_IPWT_U16_3x4);
DEFINE_FUNC_KERNEL(mlib_IPWT_U16_2x1);
DEFINE_FUNC_KERNEL(mlib_IPWT_U16_2x2);
DEFINE_FUNC_KERNEL(mlib_IPWT_U16_2x3);
DEFINE_FUNC_KERNEL(mlib_IPWT_U16_2x4);

DEFINE_FUNC_KERNEL(mlib_IPWT_S32_4x1);
DEFINE_FUNC_KERNEL(mlib_IPWT_S32_4x2);
DEFINE_FUNC_KERNEL(mlib_IPWT_S32_4x3);
DEFINE_FUNC_KERNEL(mlib_IPWT_S32_4x4);
DEFINE_FUNC_KERNEL(mlib_IPWT_S32_3x1);
DEFINE_FUNC_KERNEL(mlib_IPWT_S32_3x2);
DEFINE_FUNC_KERNEL(mlib_IPWT_S32_3x3);
DEFINE_FUNC_KERNEL(mlib_IPWT_S32_3x4);
DEFINE_FUNC_KERNEL(mlib_IPWT_S32_2x1);
DEFINE_FUNC_KERNEL(mlib_IPWT_S32_2x2);
DEFINE_FUNC_KERNEL(mlib_IPWT_S32_2x3);
DEFINE_FUNC_KERNEL(mlib_IPWT_S32_2x4);

DEFINE_FUNC_KERNEL(mlib_IPWT_F32_4x1);
DEFINE_FUNC_KERNEL(mlib_IPWT_F32_4x2);
DEFINE_FUNC_KERNEL(mlib_IPWT_F32_4x3);
DEFINE_FUNC_KERNEL(mlib_IPWT_F32_4x4);
DEFINE_FUNC_KERNEL(mlib_IPWT_F32_3x1);
DEFINE_FUNC_KERNEL(mlib_IPWT_F32_3x2);
DEFINE_FUNC_KERNEL(mlib_IPWT_F32_3x3);
DEFINE_FUNC_KERNEL(mlib_IPWT_F32_3x4);
DEFINE_FUNC_KERNEL(mlib_IPWT_F32_2x1);
DEFINE_FUNC_KERNEL(mlib_IPWT_F32_2x2);
DEFINE_FUNC_KERNEL(mlib_IPWT_F32_2x3);
DEFINE_FUNC_KERNEL(mlib_IPWT_F32_2x4);

DEFINE_FUNC_KERNEL(mlib_IPWT_D64_4x1);
DEFINE_FUNC_KERNEL(mlib_IPWT_D64_4x2);
DEFINE_FUNC_KERNEL(mlib_IPWT_D64_4x3);
DEFINE_FUNC_KERNEL(mlib_IPWT_D64_4x4);
DEFINE_FUNC_KERNEL(mlib_IPWT_D64_3x1);
DEFINE_FUNC_KERNEL(mlib_IPWT_D64_3x2);
DEFINE_FUNC_KERNEL(mlib_IPWT_D64_3x3);
DEFINE_FUNC_KERNEL(mlib_IPWT_D64_3x4);
DEFINE_FUNC_KERNEL(mlib_IPWT_D64_2x1);
DEFINE_FUNC_KERNEL(mlib_IPWT_D64_2x2);
DEFINE_FUNC_KERNEL(mlib_IPWT_D64_2x3);
DEFINE_FUNC_KERNEL(mlib_IPWT_D64_2x4);

/* *********************************************************** */

#define	DEFINE_FUNC_TABLE_0(NAME)                               \
	mlib_status                                             \
	NAME(mlib_image *dst, const mlib_image *src,            \
	    const mlib_d64 *xCoeffs, const mlib_d64 *yCoeffs,   \
	    mlib_d64 postShiftX, mlib_d64 postShiftY,           \
	    mlib_d64 postScaleX, mlib_d64 postScaleY,           \
	    const void *interp_table, mlib_edge edge)

DEFINE_FUNC_TABLE_0(mlib_ImagePolynomialWarpTable_0);
DEFINE_FUNC_TABLE_0(mlib_ImagePolynomialWarpTable_0_Fp);

/* *********************************************************** */

#ifdef __cplusplus
}
#endif

#endif /* _MLIB_IMAGEPOLYNOMIALWARP_H */
