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

#pragma ident	"@(#)mlib_ImageZoomEdge.c	9.3	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_ImageZoomZeroEdge_[U8 | S16 | S32] -
 *             clearing edge using provided color
 *
 *      mlib_ImageZoomUpNearest_[U8 | S16 | S32] -
 *             uses nearest neighbor filter for computing edge
 *
 *      mlib_ImageZoomExtend_[U8 | S16 | U16 | S32]_
 *                           [Bilinear | Bicubic | Bicubic2] -
 *             pseudo-extended source image and computing
 *
 * SYNOPSIS
 *      void mlib_ImageZoomZeroEdge_U8(mlib_work_image *param)
 *      void mlib_ImageZoomZeroEdge_S16(mlib_work_image *param)
 *      void mlib_ImageZoomZeroEdge_S32(mlib_work_image *param)
 *
 *      void mlib_ImageZoomUpNearest_U8(mlib_work_image *param)
 *      void mlib_ImageZoomUpNearest_S16(mlib_work_image *param)
 *      void mlib_ImageZoomUpNearest_S32(mlib_work_image *param)
 *
 *      void mlib_ImageZoomExtend_U8_Bilinear(mlib_work_image *param)
 *      void mlib_ImageZoomExtend_U16_Bilinear(mlib_work_image *param)
 *      void mlib_ImageZoomExtend_S16_Bilinear(mlib_work_image *param)
 *
 *      void mlib_ImageZoomExtend_U8_Bicubic(mlib_work_image *param)
 *      void mlib_ImageZoomExtend_U16_Bicubic(mlib_work_image *param)
 *      void mlib_ImageZoomExtend_S16_Bicubic(mlib_work_image *param)
 *
 *      void mlib_ImageZoomExtend_U8_Bicubic2(mlib_work_image *param)
 *      void mlib_ImageZoomExtend_U16_Bicubic2(mlib_work_image *param)
 *      void mlib_ImageZoomExtend_S16_Bicubic2(mlib_work_image *param)
 *
 * ARGUMENTS
 *      param     Internal structure
 *
 * DESCRIPTION
 *  The center of the source image is mapped to the center of the
 *  destination image.
 *
 *  Computing border rules.
 *
 */

#ifndef	_NO_LONGLONG

#include <mlib_image.h>
#include <mlib_ImageFilters.h>
#include <mlib_ImageZoom.h>
#include <mlib_ImageColormap.h>
#include <mlib_ImageDivTables.h>
#include <mlib_GeomBasicIntOp.h>

#if 0
#define	DEBUG_PRINT(A)	printf("debug:"); printf(A); printf("\n");
#else
#define	DEBUG_PRINT(A)
#endif

/* *********************************************************** */
/* Clearing */
/* *********************************************************** */

#define	MLIB_CLEAR(dp, w, h, color)                                  \
	for (j = 0; j < h; j++) {                                    \
	    for (i = 0; i < w; i++) {                                \
		(dp)[j * stride_##dp + i * channels + ch] = color;   \
	    }                                                        \
	}

/* *********************************************************** */

#define	MLIB_CLEAR_F32(dp, w, h, color)                              \
	for (j = 0; j < h; j++) {                                    \
	    for (i = 0; i < w; i++) {                                \
		*((mlib_s32 *)(&((dp)[j * stride_##dp + i * channels + ch])))  \
		    = *((mlib_s32 *)&color);                         \
	    }                                                        \
	}

/* *********************************************************** */

#define	MLIB_CLEAR_D64(dp, w, h, color)                              \
	for (j = 0; j < h; j++) {                                    \
	    for (i = 0; i < w; i++) {                                \
		*((mlib_s64 *)(&((dp)[j * stride_##dp + i * channels + ch])))  \
		    = *((mlib_s64 *)&color);                         \
	    }                                                        \
	}

/* *********************************************************** */

void
mlib_ImageZoomZeroEdge_U8(
    mlib_work_image *param)
{
	VARIABLE_EDGE(mlib_u8);
	mlib_u8 color = (mlib_u8)GetElemStruct(color);

	for (ch = 0; ch < channels; ch++) {

		MLIB_CLEAR(dp0, w1, h1, color);
		MLIB_CLEAR(dp1, w2, h2, color);
		MLIB_CLEAR(dp2, w3, h2, color);
		MLIB_CLEAR(dp3, w1, h3, color);
	}
}

/* *********************************************************** */

void
mlib_ImageZoomZeroEdge_S16(
    mlib_work_image *param)
{
	VARIABLE_EDGE(mlib_s16);
	mlib_s16 color = (mlib_s16)GetElemStruct(color);

	for (ch = 0; ch < channels; ch++) {

		MLIB_CLEAR(dp0, w1, h1, color);
		MLIB_CLEAR(dp1, w2, h2, color);
		MLIB_CLEAR(dp2, w3, h2, color);
		MLIB_CLEAR(dp3, w1, h3, color);
	}
}

/* *********************************************************** */

void
mlib_ImageZoomZeroEdge_S32(
    mlib_work_image *param)
{
	VARIABLE_EDGE(mlib_s32);
	mlib_s32 color = (mlib_s32)GetElemStruct(color);

	for (ch = 0; ch < channels; ch++) {

		MLIB_CLEAR(dp0, w1, h1, color);
		MLIB_CLEAR(dp1, w2, h2, color);
		MLIB_CLEAR(dp2, w3, h2, color);
		MLIB_CLEAR(dp3, w1, h3, color);
	}
}

/* *********************************************************** */

void
mlib_ImageZoomZeroEdge_F32(
    mlib_work_image *param)
{
	VARIABLE_EDGE(mlib_f32);
	mlib_f32 color = 0;
	DEBUG_PRINT("ZeroEdge_F32");

	for (ch = 0; ch < channels; ch++) {

		MLIB_CLEAR_F32(dp0, w1, h1, color);
		MLIB_CLEAR_F32(dp1, w2, h2, color);
		MLIB_CLEAR_F32(dp2, w3, h2, color);
		MLIB_CLEAR_F32(dp3, w1, h3, color);
	}
}

/* *********************************************************** */

void
mlib_ImageZoomZeroEdge_D64(
    mlib_work_image *param)
{
	VARIABLE_EDGE(mlib_d64);
	mlib_d64 color = 0;
	DEBUG_PRINT("ZeroEdge_D64");

	for (ch = 0; ch < channels; ch++) {

		MLIB_CLEAR_D64(dp0, w1, h1, color);
		MLIB_CLEAR_D64(dp1, w2, h2, color);
		MLIB_CLEAR_D64(dp2, w3, h2, color);
		MLIB_CLEAR_D64(dp3, w1, h3, color);
	}
}

/* *********************************************************** */
/* Up Nearest */
/* *********************************************************** */

#define	VARIABLE_UP_NEAREST(FORMAT)                                   \
	mlib_s32 src_stride = GetElemStruct(src_stride),              \
	    dx = GetElemStruct(DX),                                   \
	    dy = GetElemStruct(DY),                                   \
	    x0 = GetElemSubStruct(nearest, srcX),                     \
	    x1 = x0 + (w1 - w3) * dx,                                 \
	    y0 = GetElemSubStruct(nearest, srcY),                     \
	    y1 = y0 + h1 * dy,                                        \
	    y2 = y1 + h2 * dy;                                        \
	FORMAT *sp = (FORMAT *) GetElemStruct(sp)

/* *********************************************************** */

#define	MLIB_UP_NEAREST(dp, xx, yy, w, h)                           \
	{                                                           \
	    mlib_s32 x, y, cx, cy;                                  \
	                                                            \
	    y = yy;                                                 \
	    for (j = 0; j < h; j++) {                               \
		x = xx;                                             \
		cy = y >> MLIB_SHIFT;                               \
		for (i = 0; i < w; i++) {                           \
		    cx = x >> MLIB_SHIFT;                           \
		    (dp)[j * stride_##dp + i * channels + ch] =     \
			sp[cy * src_stride + cx * channels + ch];   \
		    x += dx;                                        \
		}                                                   \
		y += dy;                                            \
	    }                                                       \
	}

/* *********************************************************** */

#define	MLIB_UP_NEAREST_F32(dp, xx, yy, w, h)                       \
	{                                                           \
	    mlib_s32 x, y, cx, cy;                                  \
	                                                            \
	    y = yy;                                                 \
	    for (j = 0; j < h; j++) {                               \
		x = xx;                                             \
		cy = y >> MLIB_SHIFT;                               \
		for (i = 0; i < w; i++) {                           \
		    cx = x >> MLIB_SHIFT;                           \
		    *((mlib_s32 *)                                  \
			(&((dp)[j * stride_##dp + i * channels + ch]))) =     \
			*((mlib_s32 *)                                        \
			(&(sp[cy * src_stride + cx * channels + ch])));       \
		    x += dx;                                        \
		}                                                   \
		y += dy;                                            \
	    }                                                       \
	}

/* *********************************************************** */

#define	MLIB_UP_NEAREST_D64(dp, xx, yy, w, h)                       \
	{                                                           \
	    mlib_s32 x, y, cx, cy;                                  \
	                                                            \
	    y = yy;                                                 \
	    for (j = 0; j < h; j++) {                               \
		x = xx;                                             \
		cy = y >> MLIB_SHIFT;                               \
		for (i = 0; i < w; i++) {                           \
		    cx = x >> MLIB_SHIFT;                           \
		    *((mlib_s64 *)                                  \
			(&((dp)[j * stride_##dp + i * channels + ch]))) =     \
			*((mlib_s64 *)                                        \
			(&(sp[cy * src_stride + cx * channels + ch])));       \
		    x += dx;                                        \
		}                                                   \
		y += dy;                                            \
	    }                                                       \
	}

/* *********************************************************** */

void
mlib_ImageZoomUpNearest_U8(
    mlib_work_image *param)
{
	VARIABLE_EDGE(mlib_u8);
	VARIABLE_UP_NEAREST(mlib_u8);

	for (ch = 0; ch < channels; ch++) {

		MLIB_UP_NEAREST(dp0, x0, y0, w1, h1);
		MLIB_UP_NEAREST(dp1, x0, y1, w2, h2);
		MLIB_UP_NEAREST(dp2, x1, y1, w3, h2);
		MLIB_UP_NEAREST(dp3, x0, y2, w1, h3);
	}
}

/* *********************************************************** */

void
mlib_ImageZoomUpNearest_S16(
    mlib_work_image *param)
{
	VARIABLE_EDGE(mlib_s16);
	VARIABLE_UP_NEAREST(mlib_s16);

	for (ch = 0; ch < channels; ch++) {

		MLIB_UP_NEAREST(dp0, x0, y0, w1, h1);
		MLIB_UP_NEAREST(dp1, x0, y1, w2, h2);
		MLIB_UP_NEAREST(dp2, x1, y1, w3, h2);
		MLIB_UP_NEAREST(dp3, x0, y2, w1, h3);
	}
}

/* *********************************************************** */

void
mlib_ImageZoomUpNearest_S32(
    mlib_work_image *param)
{
	VARIABLE_EDGE(mlib_s32);
	VARIABLE_UP_NEAREST(mlib_s32);

	for (ch = 0; ch < channels; ch++) {

		MLIB_UP_NEAREST(dp0, x0, y0, w1, h1);
		MLIB_UP_NEAREST(dp1, x0, y1, w2, h2);
		MLIB_UP_NEAREST(dp2, x1, y1, w3, h2);
		MLIB_UP_NEAREST(dp3, x0, y2, w1, h3);
	}
}

/* *********************************************************** */

void
mlib_ImageZoomUpNearest_F32(
    mlib_work_image *param)
{
	VARIABLE_EDGE(mlib_f32);
	VARIABLE_UP_NEAREST(mlib_f32);
	DEBUG_PRINT("ZoomUpNearest_F32");

	for (ch = 0; ch < channels; ch++) {

		MLIB_UP_NEAREST_F32(dp0, x0, y0, w1, h1);
		MLIB_UP_NEAREST_F32(dp1, x0, y1, w2, h2);
		MLIB_UP_NEAREST_F32(dp2, x1, y1, w3, h2);
		MLIB_UP_NEAREST_F32(dp3, x0, y2, w1, h3);
	}
}

/* *********************************************************** */

void
mlib_ImageZoomUpNearest_D64(
    mlib_work_image *param)
{
	VARIABLE_EDGE(mlib_d64);
	VARIABLE_UP_NEAREST(mlib_d64);
	DEBUG_PRINT("ZoomUpNearest_D64");

	for (ch = 0; ch < channels; ch++) {

		MLIB_UP_NEAREST_D64(dp0, x0, y0, w1, h1);
		MLIB_UP_NEAREST_D64(dp1, x0, y1, w2, h2);
		MLIB_UP_NEAREST_D64(dp2, x1, y1, w3, h2);
		MLIB_UP_NEAREST_D64(dp3, x0, y2, w1, h3);
	}
}

/* *********************************************************** */
/* Pseudo-extended source image */
/* *********************************************************** */

#define	VARIABLE_SRC_EXTEND(FORMAT)                                   \
	mlib_s32 sw = GetElemStruct(width) - 1,                       \
	    sh = GetElemStruct(height) - 1,                           \
	    max_x = sw,                                               \
	    max_y = sh,                                               \
	    src_stride = GetElemStruct(src_stride),                   \
	    dx = GetElemStruct(DX),                                   \
	    dy = GetElemStruct(DY),                                   \
	    x0 = GetElemSubStruct(current, srcX) - dx * w2,           \
	    x1 = GetElemSubStruct(current, srcX)                      \
		+ GetElemSubStruct(current, width) * dx,              \
	    y0 = GetElemSubStruct(current, srcY) - dy * h1,           \
	    y1 = GetElemSubStruct(current, srcY),                     \
	    y2 = y1 + GetElemSubStruct(current, height) * dy;         \
	    FORMAT *sp = (FORMAT *) GetElemStruct(sp)

/* *********************************************************** */

#define	FLOAT_FORMAT	mlib_f32

/* *********************************************************** */

#define	INT_BOUND(DST, SRC, MAX_VAL)                            \
	{                                                       \
	    mlib_s32 m1, m2;                                    \
	                                                        \
	    m1 = (SRC) >> MLIB_SIGN_SHIFT;                      \
	    m2 = (MAX_VAL - (SRC)) >> MLIB_SIGN_SHIFT;          \
	    DST = (((SRC) & ~(m1 | m2)) | (MAX_VAL & m2));      \
	}

/* *********************************************************** */

#define	READ_SRC(ind)	sp[ind]

#define	READ_SRC_U8(ind)	mlib_U82F32[sp[ind]]

#define	READ_SRC_BLEND(ind)                                     \
	(mlib_U82F32[sp[ind]] * mlib_U82F32[sp[ind + alp_ind]]  \
	    * (1.0f / 255))

#define	READ_SRC_BLEND_INT_S32(ind)                             \
	((mlib_s32)sp[ind] * (mlib_s32)sp[ind + alp_ind] / 255) \

#define	READ_SRC_BLEND_INT_S32_NO_255(ind)                      \
	((mlib_s32)sp[ind] * (mlib_s32)sp[ind + alp_ind]) \

/* *********************************************************** */

#define	NN_COORD(x)                                             \
	c##x = x >> MLIB_SHIFT;                                 \
	INT_BOUND(c##x, c##x, max_##x)

#define	MLIB_SRC_EXTEND_NN_1(dp, xx, yy, w, h, READ_SRC)        \
	{                                                       \
	    mlib_s32 x, y, cx, cy;                              \
	                                                        \
	    y = yy;                                             \
	    for (j = 0; j < h; j++) {                           \
		x = xx;                                         \
		NN_COORD(y);                                    \
		cy *= src_stride;                               \
		for (i = 0; i < w; i++) {                       \
		    NN_COORD(x);                                \
		    cx = channels * cx + ch;                    \
		    dp[j * stride_##dp + i * channels + ch] =   \
			READ_SRC(cy + cx);                      \
		    x += dx;                                    \
		}                                               \
		y += dy;                                        \
	    }                                                   \
	}

/* *********************************************************** */

#define	MLIB_SRC_EXTEND_NN(READ_SRC)                            \
	MLIB_SRC_EXTEND_NN_1(dp0, x0, y0, w1, h1, READ_SRC);    \
	MLIB_SRC_EXTEND_NN_1(dp1, x0, y1, w2, h2, READ_SRC);    \
	MLIB_SRC_EXTEND_NN_1(dp2, x1, y1, w3, h2, READ_SRC);    \
	MLIB_SRC_EXTEND_NN_1(dp3, x0, y2, w1, h3, READ_SRC)

/* *********************************************************** */

#define	MLIB_SRC_EXTEND_NN_1_F32(dp, xx, yy, w, h, READ_SRC)    \
	{                                                       \
	    mlib_s32 x, y, cx, cy;                              \
	                                                        \
	    y = yy;                                             \
	    for (j = 0; j < h; j++) {                           \
		x = xx;                                         \
		NN_COORD(y);                                    \
		cy *= src_stride;                               \
		for (i = 0; i < w; i++) {                       \
		    NN_COORD(x);                                \
		    cx = channels * cx + ch;                    \
		    *((mlib_s32 *)                              \
			(&(dp[j * stride_##dp + i * channels + ch]))) =   \
			*(mlib_s32 *)(&(READ_SRC(cy + cx)));    \
		    x += dx;                                    \
		}                                               \
		y += dy;                                        \
	    }                                                   \
	}

/* *********************************************************** */

#define	MLIB_SRC_EXTEND_NN_F32(READ_SRC)                            \
	MLIB_SRC_EXTEND_NN_1_F32(dp0, x0, y0, w1, h1, READ_SRC);    \
	MLIB_SRC_EXTEND_NN_1_F32(dp1, x0, y1, w2, h2, READ_SRC);    \
	MLIB_SRC_EXTEND_NN_1_F32(dp2, x1, y1, w3, h2, READ_SRC);    \
	MLIB_SRC_EXTEND_NN_1_F32(dp3, x0, y2, w1, h3, READ_SRC)

/* *********************************************************** */

#define	MLIB_SRC_EXTEND_NN_1_D64(dp, xx, yy, w, h, READ_SRC)    \
	{                                                       \
	    mlib_s32 x, y, cx, cy;                              \
	                                                        \
	    y = yy;                                             \
	    for (j = 0; j < h; j++) {                           \
		x = xx;                                         \
		NN_COORD(y);                                    \
		cy *= src_stride;                               \
		for (i = 0; i < w; i++) {                       \
		    NN_COORD(x);                                \
		    cx = channels * cx + ch;                    \
		    *((mlib_s64 *)                              \
			(&(dp[j * stride_##dp + i * channels + ch]))) =   \
			*((mlib_s64 *)(&(READ_SRC(cy + cx))));  \
		    x += dx;                                    \
		}                                               \
		y += dy;                                        \
	    }                                                   \
	}

/* *********************************************************** */

#define	MLIB_SRC_EXTEND_NN_D64(READ_SRC)                            \
	MLIB_SRC_EXTEND_NN_1_D64(dp0, x0, y0, w1, h1, READ_SRC);    \
	MLIB_SRC_EXTEND_NN_1_D64(dp1, x0, y1, w2, h2, READ_SRC);    \
	MLIB_SRC_EXTEND_NN_1_D64(dp2, x1, y1, w3, h2, READ_SRC);    \
	MLIB_SRC_EXTEND_NN_1_D64(dp3, x0, y2, w1, h3, READ_SRC)

/* *********************************************************** */

void
mlib_ImageZoomExtend_U8_Nearest(
    mlib_work_image *param)
{
	VARIABLE_EDGE(mlib_u8);
	VARIABLE_SRC_EXTEND(mlib_u8);

	for (ch = 0; ch < channels; ch++) {
		MLIB_SRC_EXTEND_NN(READ_SRC);
	}
}

/* *********************************************************** */

void
mlib_ImageZoomExtend_U8_Nearest_Blend(
    mlib_work_image *param,
    mlib_s32 alp_ind)
{
	VARIABLE_EDGE(mlib_u8);
	VARIABLE_SRC_EXTEND(mlib_u8);
	DEBUG_PRINT("ZoomExtend_U8_Nearest_Blend");

	for (ch = 0; ch < channels; ch++) {
		if (!alp_ind) {
			MLIB_SRC_EXTEND_NN(READ_SRC);
		} else {
			MLIB_SRC_EXTEND_NN((mlib_u8)READ_SRC_BLEND_INT_S32);
		}
		alp_ind--;
	}
}

/* *********************************************************** */

void
mlib_ImageZoomExtend_S16_Nearest(
    mlib_work_image *param)
{
	VARIABLE_EDGE(mlib_s16);
	VARIABLE_SRC_EXTEND(mlib_s16);

#if 0
	src_stride /= 2;
	stride_dp0 /= 2;
	stride_dp1 /= 2;
	stride_dp2 /= 2;
	stride_dp3 /= 2;
#else
	src_stride >>= 1;
	stride_dp0 >>= 1;
	stride_dp1 >>= 1;
	stride_dp2 >>= 1;
	stride_dp3 >>= 1;
#endif

	for (ch = 0; ch < channels; ch++) {
		MLIB_SRC_EXTEND_NN(READ_SRC);
	}
}

/* *********************************************************** */

void
mlib_ImageZoomExtend_U16_Nearest(
    mlib_work_image *param)
{
	VARIABLE_EDGE(mlib_u16);
	VARIABLE_SRC_EXTEND(mlib_u16);

#if 0
	src_stride /= 2;
	stride_dp0 /= 2;
	stride_dp1 /= 2;
	stride_dp2 /= 2;
	stride_dp3 /= 2;
#else
	src_stride >>= 1;
	stride_dp0 >>= 1;
	stride_dp1 >>= 1;
	stride_dp2 >>= 1;
	stride_dp3 >>= 1;
#endif

	for (ch = 0; ch < channels; ch++) {
		MLIB_SRC_EXTEND_NN(READ_SRC);
	}
}

/* *********************************************************** */

void
mlib_ImageZoomExtend_S32_Nearest(
    mlib_work_image *param)
{
	VARIABLE_EDGE(mlib_s32);
	VARIABLE_SRC_EXTEND(mlib_s32);

#if 0
	src_stride /= 4;
	stride_dp0 /= 4;
	stride_dp1 /= 4;
	stride_dp2 /= 4;
	stride_dp3 /= 4;
#else
	src_stride >>= 2;
	stride_dp0 >>= 2;
	stride_dp1 >>= 2;
	stride_dp2 >>= 2;
	stride_dp3 >>= 2;
#endif

	for (ch = 0; ch < channels; ch++) {
		MLIB_SRC_EXTEND_NN(READ_SRC);
	}
}

/* *********************************************************** */

void
mlib_ImageZoomExtend_F32_Nearest(
    mlib_work_image *param)
{
	VARIABLE_EDGE(mlib_f32);
	VARIABLE_SRC_EXTEND(mlib_f32);

#if 0
	src_stride /= 4;
	stride_dp0 /= 4;
	stride_dp1 /= 4;
	stride_dp2 /= 4;
	stride_dp3 /= 4;
#else
	src_stride >>= 2;
	stride_dp0 >>= 2;
	stride_dp1 >>= 2;
	stride_dp2 >>= 2;
	stride_dp3 >>= 2;
#endif
	DEBUG_PRINT("ZoomExtend_F32_Nearest");

	for (ch = 0; ch < channels; ch++) {
		MLIB_SRC_EXTEND_NN_F32(READ_SRC);
	}
}

/* *********************************************************** */

void
mlib_ImageZoomExtend_D64_Nearest(
    mlib_work_image *param)
{
	VARIABLE_EDGE(mlib_d64);
	VARIABLE_SRC_EXTEND(mlib_d64);
	DEBUG_PRINT("ZoomExtend_D64_Nearest");

#if 0
	src_stride /= 8;
	stride_dp0 /= 8;
	stride_dp1 /= 8;
	stride_dp2 /= 8;
	stride_dp3 /= 8;
#else
	src_stride >>= 3;
	stride_dp0 >>= 3;
	stride_dp1 >>= 3;
	stride_dp2 >>= 3;
	stride_dp3 >>= 3;
#endif

	for (ch = 0; ch < channels; ch++) {
		MLIB_SRC_EXTEND_NN_D64(READ_SRC);
	}
}

/* *********************************************************** */

#define	BL_COORD_int(x)                                         \
	f##x = (x >> 5) & 0x7ff;                                \
	c##x = x >> MLIB_SHIFT;                                 \
	s##x = c##x + 1;                                        \
	c##x = c##x & ~(c##x >> MLIB_SIGN_SHIFT);               \
	s##x = s##x & ~(s##x >> MLIB_SIGN_SHIFT);               \
	if (c##x > max_##x)                                     \
	    c##x = max_##x;                                     \
	if (s##x > max_##x)                                     \
	    s##x = max_##x

#define	MLIB_SRC_EXTEND_BL_1_int(dp, xx, yy, w, h)                     \
	{                                                              \
	    mlib_s32 x, y, cx, cy, sx, sy;                             \
	    mlib_s32 fy, fx, fres, a0, a1, a2, a3;                     \
                                                                       \
	    y = yy;                                                    \
	    for (j = 0; j < h; j++) {                                  \
		x = xx;                                                \
		BL_COORD_int(y);                                       \
		cy *= src_stride;                                      \
		sy *= src_stride;                                      \
		for (i = 0; i < w; i++) {                              \
		    BL_COORD_int(x);                                   \
		    cx = channels * cx + ch;                           \
		    sx = channels * sx + ch;                           \
		    a0 = sp[cy + cx];                                  \
		    a1 = sp[cy + sx];                                  \
		    a3 = sp[sy + cx];                                  \
		    a2 = sp[sy + sx];                                  \
		    fres = ((a0 << 22) + (fy << 11)*(a3 - a0) +        \
			(fx << 11) * (a1 - a0) + fx * fy *             \
			(a0 + a2 - a3 - a1));                          \
		    fres = (fres + (1 << 21)) >> 22;                   \
		    dp[j * stride_##dp + i * channels + ch] = fres;    \
		    x += dx;                                           \
		}                                                      \
		y += dy;                                               \
	    }                                                          \
	}

#define	MLIB_SRC_EXTEND_BL_int()                        \
	MLIB_SRC_EXTEND_BL_1_int(dp0, x0, y0, w1, h1);  \
	MLIB_SRC_EXTEND_BL_1_int(dp1, x0, y1, w2, h2);  \
	MLIB_SRC_EXTEND_BL_1_int(dp2, x1, y1, w3, h2);  \
	MLIB_SRC_EXTEND_BL_1_int(dp3, x0, y2, w1, h3)

/* *********************************************************** */

#define	BL_COORD(x)                                             \
	f##x = (FLOAT_FORMAT) ((x & MLIB_MASK) * MLIB_SCALE);   \
	c##x = x >> MLIB_SHIFT;                                 \
	s##x = c##x + 1;                                        \
	c##x = c##x & ~(c##x >> MLIB_SIGN_SHIFT);               \
	s##x = s##x & ~(s##x >> MLIB_SIGN_SHIFT);               \
	if (c##x > max_##x)                                     \
	    c##x = max_##x;                                     \
	if (s##x > max_##x)                                     \
	    s##x = max_##x

#define	MLIB_SRC_EXTEND_BL_1(dp, xx, yy, w, h, READ_SRC, ROUND, FORMAT)       \
	{                                                                     \
	    mlib_s32 x, y, cx, cy, sx, sy;                                    \
	    FLOAT_FORMAT fy, fx, fres, a0, a1, a2, a3;                        \
	                                                                      \
	    y = yy;                                                           \
	    for (j = 0; j < h; j++) {                                         \
		x = xx;                                                       \
		BL_COORD(y);                                                  \
		cy *= src_stride;                                             \
		sy *= src_stride;                                             \
		for (i = 0; i < w; i++) {                                     \
		    BL_COORD(x);                                              \
		    cx = channels * cx + ch;                                  \
		    sx = channels * sx + ch;                                  \
		    a0 = (FLOAT_FORMAT) READ_SRC(cy + cx);                    \
		    a1 = (FLOAT_FORMAT) READ_SRC(cy + sx);                    \
		    a3 = (FLOAT_FORMAT) READ_SRC(sy + cx);                    \
		    a2 = (FLOAT_FORMAT) READ_SRC(sy + sx);                    \
		    fres =                                                    \
			(a0 + fy * (a3 - a0) + fx * (a1 - a0) +               \
			fx * fy * (a0 + a2 - a3 - a1));                       \
		    ROUND(fres)                                               \
		    dp[j * stride_##dp + i * channels + ch] = (FORMAT) fres;  \
		    x += dx;                                                  \
		}                                                             \
		y += dy;                                                      \
	    }                                                                 \
	}

#define	MLIB_SRC_EXTEND_BL(READ_SRC, ROUND, FORMAT)                            \
	MLIB_SRC_EXTEND_BL_1(dp0, x0, y0, w1, h1, READ_SRC, ROUND_INT, FORMAT);\
	MLIB_SRC_EXTEND_BL_1(dp1, x0, y1, w2, h2, READ_SRC, ROUND_INT, FORMAT);\
	MLIB_SRC_EXTEND_BL_1(dp2, x1, y1, w3, h2, READ_SRC, ROUND_INT, FORMAT);\
	MLIB_SRC_EXTEND_BL_1(dp3, x0, y2, w1, h3, READ_SRC, ROUND_INT, FORMAT)

/* *********************************************************** */

#define	BL_COORD_SRC(x)                                         \
	f##x = (mlib_s64) (x & MLIB_MASK);                      \
	c##x = x >> MLIB_SHIFT;                                 \
	s##x = c##x + 1;                                        \
	c##x = c##x & ~(c##x >> MLIB_SIGN_SHIFT);               \
	s##x = s##x & ~(s##x >> MLIB_SIGN_SHIFT);               \
	if (c##x > max_##x)                                     \
	    c##x = max_##x;                                     \
	if (s##x > max_##x)                                     \
	    s##x = max_##x

#define	MLIB_SRC_EXTEND_BL_1_SRC(dp, xx, yy, w, h, READ_SRC, ROUND, FORMAT)   \
	{                                                                     \
	    mlib_s32 x, y, cx, cy, sx, sy;                                    \
	    mlib_s64 fy, fx, fres, a0, a1, a2, a3;                            \
	                                                                      \
	    y = yy;                                                           \
	    for (j = 0; j < h; j++) {                                         \
		x = xx;                                                       \
		BL_COORD_SRC(y);                                              \
		cy *= src_stride;                                             \
		sy *= src_stride;                                             \
		for (i = 0; i < w; i++) {                                     \
		    BL_COORD_SRC(x);                                          \
		    cx = channels * cx + ch;                                  \
		    sx = channels * sx + ch;                                  \
		    a0 = (mlib_s64) READ_SRC(cy + cx);                        \
		    a1 = (mlib_s64) READ_SRC(cy + sx);                        \
		    a3 = (mlib_s64) READ_SRC(sy + cx);                        \
		    a2 = (mlib_s64) READ_SRC(sy + sx);                        \
		    fres =                                                    \
			((a0<<(2*MLIB_SHIFT)) + (fy<<MLIB_SHIFT) *            \
			(a3 - a0) + (fx<<MLIB_SHIFT) * (a1 - a0) +            \
			fx * fy * (a0 + a2 - a3 - a1));                       \
		    fres += (mlib_s64)1<<(2*MLIB_SHIFT-1);                    \
		    fres >>= (2*MLIB_SHIFT);                                  \
		    dp[j * stride_##dp + i * channels + ch] = (FORMAT) fres;  \
		    x += dx;                                                  \
		}                                                             \
		y += dy;                                                      \
	    }                                                                 \
	}

#define	MLIB_SRC_EXTEND_BL_SRC(READ_SRC, ROUND, FORMAT)                       \
	MLIB_SRC_EXTEND_BL_1_SRC(dp0, x0, y0, w1, h1, READ_SRC,               \
			ROUND_INT, FORMAT);                                   \
	MLIB_SRC_EXTEND_BL_1_SRC(dp1, x0, y1, w2, h2, READ_SRC,               \
			ROUND_INT, FORMAT);                                   \
	MLIB_SRC_EXTEND_BL_1_SRC(dp2, x1, y1, w3, h2, READ_SRC,               \
			ROUND_INT, FORMAT);                                   \
	MLIB_SRC_EXTEND_BL_1_SRC(dp3, x0, y2, w1, h3, READ_SRC,               \
			ROUND_INT, FORMAT)

/* *********************************************************** */

#define	BL_COORD_INT(x)                                         \
	f##x = (mlib_s64) (x & MLIB_MASK);                      \
	c##x = x >> MLIB_SHIFT;                                 \
	s##x = c##x + 1;                                        \
	c##x = c##x & ~(c##x >> MLIB_SIGN_SHIFT);               \
	s##x = s##x & ~(s##x >> MLIB_SIGN_SHIFT);               \
	if (c##x > max_##x)                                     \
	    c##x = max_##x;                                     \
	if (s##x > max_##x)                                     \
	    s##x = max_##x

#define	MLIB_SRC_EXTEND_BL_1_INT(dp, xx, yy, w, h, READ_SRC, ROUND, FORMAT)   \
	{                                                                     \
	    mlib_s32 x, y, cx, cy, sx, sy;                                    \
	    mlib_s64 fy, fx, fres, a0, a1, a2, a3;                            \
	                                                                      \
	    y = yy;                                                           \
	    for (j = 0; j < h; j++) {                                         \
		x = xx;                                                       \
		BL_COORD_INT(y);                                              \
		cy *= src_stride;                                             \
		sy *= src_stride;                                             \
		for (i = 0; i < w; i++) {                                     \
		    BL_COORD_INT(x);                                          \
		    cx = channels * cx + ch;                                  \
		    sx = channels * sx + ch;                                  \
		    a0 = (mlib_s64) READ_SRC(cy + cx);                        \
		    a1 = (mlib_s64) READ_SRC(cy + sx);                        \
		    a3 = (mlib_s64) READ_SRC(sy + cx);                        \
		    a2 = (mlib_s64) READ_SRC(sy + sx);                        \
		    fres =                                                    \
			((a0<<(2*MLIB_SHIFT)) + (fy<<MLIB_SHIFT) *            \
			(a3 - a0) + (fx<<MLIB_SHIFT) * (a1 - a0) +            \
			fx * fy * (a0 + a2 - a3 - a1));                       \
		    fres += ((mlib_s64)255<<(2*MLIB_SHIFT-1));                \
		    DIV255(fres, (fres >> (2*MLIB_SHIFT)));                   \
		    dp[j * stride_##dp + i * channels + ch] = (FORMAT) fres;  \
		    x += dx;                                                  \
		}                                                             \
		y += dy;                                                      \
	    }                                                                 \
	}

#define	MLIB_SRC_EXTEND_BL_INT(READ_SRC, ROUND, FORMAT)                       \
	MLIB_SRC_EXTEND_BL_1_INT(dp0, x0, y0, w1, h1, READ_SRC,               \
			ROUND_INT, FORMAT);                                   \
	MLIB_SRC_EXTEND_BL_1_INT(dp1, x0, y1, w2, h2, READ_SRC,               \
			ROUND_INT, FORMAT);                                   \
	MLIB_SRC_EXTEND_BL_1_INT(dp2, x1, y1, w3, h2, READ_SRC,               \
			ROUND_INT, FORMAT);                                   \
	MLIB_SRC_EXTEND_BL_1_INT(dp3, x0, y2, w1, h3, READ_SRC,               \
			ROUND_INT, FORMAT)

/* *********************************************************** */

#define	ROUND_INT(fres)                                         \
	fres += (FLOAT_FORMAT) 0.5;                             \
	if (fres < 0)                                           \
	    fres -= 1;

/* *********************************************************** */

#define	NO_ROUND(fres)

/* *********************************************************** */

void
mlib_ImageZoomExtend_U8_Bilinear(
    mlib_work_image *param)
{
	VARIABLE_EDGE(mlib_u8);
	VARIABLE_SRC_EXTEND(mlib_u8);
	DEBUG_PRINT("ZoomExtend_U8_Bilinear");

	for (ch = 0; ch < channels; ch++) {
/* #ifndef __sparc */ /* for x86, using integer multiplies is faster */
		MLIB_SRC_EXTEND_BL_int();
/* #else */
/*		MLIB_SRC_EXTEND_BL(READ_SRC_U8, ROUND_INT, mlib_u8); */
/* #endif */
	}
}

/* *********************************************************** */

void
mlib_ImageZoomExtend_U8_Bilinear_Blend(
    mlib_work_image *param,
    mlib_s32 alp_ind)
{
	VARIABLE_EDGE(mlib_u8);
	VARIABLE_SRC_EXTEND(mlib_u8);
	DEBUG_PRINT("ZoomExtend_U8_Bilinear_Blend");

	for (ch = 0; ch < channels; ch++) {
		if (!alp_ind) {
			MLIB_SRC_EXTEND_BL_SRC(READ_SRC, ROUND_INT, mlib_u8);
		} else {
			MLIB_SRC_EXTEND_BL_INT(READ_SRC_BLEND_INT_S32_NO_255,
				ROUND_INT, mlib_u8);
		}
		alp_ind--;
	}
}

/* *********************************************************** */

void
mlib_ImageZoomExtend_S16_Bilinear(
    mlib_work_image *param)
{
	VARIABLE_EDGE(mlib_s16);
	VARIABLE_SRC_EXTEND(mlib_s16);
	DEBUG_PRINT("ZoomExtend_S16_Bilinear");

	for (ch = 0; ch < channels; ch++) {
		MLIB_SRC_EXTEND_BL_SRC(READ_SRC, ROUND_INT, mlib_s16);
	}
}

/* *********************************************************** */

void
mlib_ImageZoomExtend_U16_Bilinear(
    mlib_work_image *param)
{
	VARIABLE_EDGE(mlib_u16);
	VARIABLE_SRC_EXTEND(mlib_u16);
	DEBUG_PRINT("ZoomExtend_U16_Bilinear");

	for (ch = 0; ch < channels; ch++) {
		MLIB_SRC_EXTEND_BL_SRC(READ_SRC, ROUND_INT, mlib_u16);
	}
}

/* *********************************************************** */

void
mlib_ImageZoomExtend_F32_Bilinear(
    mlib_work_image *param)
{
	VARIABLE_EDGE(mlib_f32);
	VARIABLE_SRC_EXTEND(mlib_f32);

	for (ch = 0; ch < channels; ch++) {
		MLIB_SRC_EXTEND_BL(READ_SRC, NO_ROUND, mlib_f32);
	}
}

/* *********************************************************** */

#undef FLOAT_FORMAT
#define	FLOAT_FORMAT	mlib_d64

/* *********************************************************** */

void
mlib_ImageZoomExtend_S32_Bilinear(
    mlib_work_image *param)
{
	VARIABLE_EDGE(mlib_s32);
	VARIABLE_SRC_EXTEND(mlib_s32);

	for (ch = 0; ch < channels; ch++) {
		MLIB_SRC_EXTEND_BL(READ_SRC, ROUND_INT, mlib_s32);
	}
}

/* *********************************************************** */

void
mlib_ImageZoomExtend_D64_Bilinear(
    mlib_work_image *param)
{
	VARIABLE_EDGE(mlib_d64);
	VARIABLE_SRC_EXTEND(mlib_d64);

	for (ch = 0; ch < channels; ch++) {
		MLIB_SRC_EXTEND_BL(READ_SRC, NO_ROUND, mlib_d64);
	}
}

/* *********************************************************** */

#ifdef MLIB_USE_FTOI_CLAMPING

#define	RES_U8(dp)                                                 \
	dp[j * stride_##dp + i * channels + ch] =                  \
	    ((mlib_s32)(fx - (mlib_f32)0x7F800000) >> 24) ^ 0x80

#define	RES_S16(dp)                                             \
	dp[j * stride_##dp + i * channels + ch] =               \
	    (mlib_s32)fx >> MLIB_SHIFT

#define	RES_U16(dp)                                             \
	dp[j * stride_##dp + i * channels + ch] =               \
	    ((mlib_s32)(fx -                                    \
	    (mlib_d64)0x7FFF8000) >> MLIB_SHIFT) ^ 0x8000

#else /* MLIB_USE_FTOI_CLAMPING */

#define	RES_U8(dp)                                              \
	{                                                       \
	    mlib_d64 sat_tmp = fx - (mlib_d64)0x7F800000;       \
	                                                        \
	    if (sat_tmp >= MLIB_S32_MAX)                        \
		sat_tmp = MLIB_S32_MAX;                         \
	    if (sat_tmp <= MLIB_S32_MIN)                        \
		sat_tmp = MLIB_S32_MIN;                         \
	    dp[j * stride_##dp + i * channels + ch] =           \
		((mlib_s32)sat_tmp >> 24) ^ 0x80;               \
	}

#define	RES_S16(dp)                                             \
	{                                                       \
	    mlib_d64 sat_tmp = fx;                              \
	                                                        \
	    if (sat_tmp >= MLIB_S32_MAX)                        \
		sat_tmp = MLIB_S32_MAX;                         \
	    if (sat_tmp <= MLIB_S32_MIN)                        \
		sat_tmp = MLIB_S32_MIN;                         \
	    dp[j * stride_##dp + i * channels + ch] =           \
		(mlib_s32)sat_tmp >> MLIB_SHIFT;                \
	}

#define	RES_U16(dp)                                             \
	{                                                       \
	    mlib_d64 sat_tmp = fx - (mlib_d64)0x7FFF8000;       \
	                                                        \
	    if (sat_tmp >= MLIB_S32_MAX)                        \
		sat_tmp = MLIB_S32_MAX;                         \
	    if (sat_tmp <= MLIB_S32_MIN)                        \
		sat_tmp = MLIB_S32_MIN;                         \
	    dp[j * stride_##dp + i * channels + ch] =           \
		((mlib_s32)sat_tmp >> MLIB_SHIFT) ^ 0x8000;     \
	}

#endif /* MLIB_USE_FTOI_CLAMPING */

/* *********************************************************** */

#define	RES_U8_INT(dp)                                          \
	{                                                       \
	    mlib_s64 sat_tmp = fx - 0x7F800000;                 \
	                                                        \
	    if (sat_tmp >= MLIB_S32_MAX)                        \
		sat_tmp = MLIB_S32_MAX;                         \
	    if (sat_tmp <= MLIB_S32_MIN)                        \
		sat_tmp = MLIB_S32_MIN;                         \
	    dp[j * stride_##dp + i * channels + ch] =           \
		((mlib_s32)sat_tmp >> 24) ^ 0x80;               \
	}

#define	RES_S16_INT(dp)                                         \
	{                                                       \
	    mlib_s64 sat_tmp = fx;                              \
	                                                        \
	    if (sat_tmp >= MLIB_S32_MAX)                        \
		sat_tmp = MLIB_S32_MAX;                         \
	    if (sat_tmp <= MLIB_S32_MIN)                        \
		sat_tmp = MLIB_S32_MIN;                         \
	    dp[j * stride_##dp + i * channels + ch] =           \
		(mlib_s32)sat_tmp >> MLIB_SHIFT;                \
	}

#define	RES_U16_INT(dp)                                         \
	{                                                       \
	    mlib_s64 sat_tmp = fx - (mlib_s64)0x7FFF8000;       \
	                                                        \
	    if (sat_tmp >= MLIB_S32_MAX)                        \
		sat_tmp = MLIB_S32_MAX;                         \
	    if (sat_tmp <= MLIB_S32_MIN)                        \
		sat_tmp = MLIB_S32_MIN;                         \
	    dp[j * stride_##dp + i * channels + ch] =           \
		((mlib_s32)sat_tmp >> MLIB_SHIFT) ^ 0x8000;     \
	}

/* *********************************************************** */

#define	MLIB_SRC_EXTEND_BC_1(dp, xx, yy, w, h, filter, RES, READ_SRC)    \
	{                                                                \
	    mlib_s32 x, y, cx, cy, r0, r1, r2, r3, c0, c1, c2, c3;       \
	    mlib_f32 *fPtr;                                              \
	    mlib_f32 fx, k0, k1, k2, k3, xf0, xf1, xf2, xf3, yf0, yf1,   \
		yf2, yf3;                                                \
	    y = yy;                                                      \
	    for (j = 0; j < h; j++) {                                    \
		x = xx;                                                  \
		cy = y >> MLIB_SHIFT;                                    \
		fPtr =                                                   \
		    (mlib_f32 *)((mlib_u8 *)filter +                     \
		    ((y >> MLIB_XY_SHIFT) & MLIB_XY_MASK));              \
		yf0 = fPtr[0];                                           \
		yf1 = fPtr[1];                                           \
		yf2 = fPtr[2];                                           \
		yf3 = fPtr[3];                                           \
		INT_BOUND(r0, cy - 1, sh);                               \
		INT_BOUND(r1, cy, sh);                                   \
		INT_BOUND(r2, cy + 1, sh);                               \
		INT_BOUND(r3, cy + 2, sh);                               \
		r0 *= src_stride;                                        \
		r1 *= src_stride;                                        \
		r2 *= src_stride;                                        \
		r3 *= src_stride;                                        \
		for (i = 0; i < w; i++) {                                \
		    cx = x >> MLIB_SHIFT;                                \
		    fPtr =                                               \
			(mlib_f32 *)((mlib_u8 *)filter +                 \
			((x >> MLIB_XY_SHIFT) & MLIB_XY_MASK));          \
		    xf0 = fPtr[0];                                       \
		    xf1 = fPtr[1];                                       \
		    xf2 = fPtr[2];                                       \
		    xf3 = fPtr[3];                                       \
		    INT_BOUND(c0, cx - 1, sw);                           \
		    INT_BOUND(c1, cx, sw);                               \
		    INT_BOUND(c2, cx + 1, sw);                           \
		    INT_BOUND(c3, cx + 2, sw);                           \
		    c0 = channels * c0 + ch;                             \
		    c1 = channels * c1 + ch;                             \
		    c2 = channels * c2 + ch;                             \
		    c3 = channels * c3 + ch;                             \
		    k0 = READ_SRC(r0 + c0) * xf0 + READ_SRC(r0 +         \
			c1) * xf1 + READ_SRC(r0 + c2) * xf2 +            \
			READ_SRC(r0 + c3) * xf3;                         \
		    k1 = READ_SRC(r1 + c0) * xf0 + READ_SRC(r1 +         \
			c1) * xf1 + READ_SRC(r1 + c2) * xf2 +            \
			READ_SRC(r1 + c3) * xf3;                         \
		    k2 = READ_SRC(r2 + c0) * xf0 + READ_SRC(r2 +         \
			c1) * xf1 + READ_SRC(r2 + c2) * xf2 +            \
			READ_SRC(r2 + c3) * xf3;                         \
		    k3 = READ_SRC(r3 + c0) * xf0 + READ_SRC(r3 +         \
			c1) * xf1 + READ_SRC(r3 + c2) * xf2 +            \
			READ_SRC(r3 + c3) * xf3;                         \
		    fx = k0 * yf0 + k1 * yf1 + k2 * yf2 + k3 * yf3;      \
		    RES(dp);                                             \
		    x += dx;                                             \
		}                                                        \
		y += dy;                                                 \
	    }                                                            \
	}

/* *********************************************************** */

#define	MLIB_SRC_EXTEND_BC(FLT, RES, READ_SRC)                           \
	MLIB_SRC_EXTEND_BC_1(dp0, x0, y0, w1, h1, FLT, RES, READ_SRC);   \
	MLIB_SRC_EXTEND_BC_1(dp1, x0, y1, w2, h2, FLT, RES, READ_SRC);   \
	MLIB_SRC_EXTEND_BC_1(dp2, x1, y1, w3, h2, FLT, RES, READ_SRC);   \
	MLIB_SRC_EXTEND_BC_1(dp3, x0, y2, w1, h3, FLT, RES, READ_SRC)

/* *********************************************************** */

#define	MLIB_SRC_EXTEND_BC_1_INT(dp, xx, yy, w, h, filter, RES, READ_SRC)    \
	{                                                                \
	    mlib_s32 x, y, cx, cy, r0, r1, r2, r3, c0, c1, c2, c3;       \
	    mlib_s16 *fPtr;                                              \
	    mlib_s64 fx, k0, k1, k2, k3, xf0, xf1, xf2, xf3, yf0, yf1,   \
		yf2, yf3;                                                \
	    y = yy;                                                      \
	    for (j = 0; j < h; j++) {                                    \
		x = xx;                                                  \
		cy = y >> MLIB_SHIFT;                                    \
		fPtr =                                                   \
		    (mlib_s16 *)((mlib_u8 *)filter +                     \
		    (((y >> MLIB_XY_SHIFT) & MLIB_XY_MASK)>>1));         \
		yf0 = fPtr[0];                                           \
		yf1 = fPtr[1];                                           \
		yf2 = fPtr[2];                                           \
		yf3 = fPtr[3];                                           \
		INT_BOUND(r0, cy - 1, sh);                               \
		INT_BOUND(r1, cy, sh);                                   \
		INT_BOUND(r2, cy + 1, sh);                               \
		INT_BOUND(r3, cy + 2, sh);                               \
		r0 *= src_stride;                                        \
		r1 *= src_stride;                                        \
		r2 *= src_stride;                                        \
		r3 *= src_stride;                                        \
		for (i = 0; i < w; i++) {                                \
		    cx = x >> MLIB_SHIFT;                                \
		    fPtr =                                               \
			(mlib_s16 *)((mlib_u8 *)filter +                 \
			(((x >> MLIB_XY_SHIFT) & MLIB_XY_MASK)>>1));     \
		    xf0 = fPtr[0];                                       \
		    xf1 = fPtr[1];                                       \
		    xf2 = fPtr[2];                                       \
		    xf3 = fPtr[3];                                       \
		    INT_BOUND(c0, cx - 1, sw);                           \
		    INT_BOUND(c1, cx, sw);                               \
		    INT_BOUND(c2, cx + 1, sw);                           \
		    INT_BOUND(c3, cx + 2, sw);                           \
		    c0 = channels * c0 + ch;                             \
		    c1 = channels * c1 + ch;                             \
		    c2 = channels * c2 + ch;                             \
		    c3 = channels * c3 + ch;                             \
		    k0 = READ_SRC(r0 + c0) * xf0 + READ_SRC(r0 +         \
			c1) * xf1 + READ_SRC(r0 + c2) * xf2 +            \
			READ_SRC(r0 + c3) * xf3;                         \
		    k1 = READ_SRC(r1 + c0) * xf0 + READ_SRC(r1 +         \
			c1) * xf1 + READ_SRC(r1 + c2) * xf2 +            \
			READ_SRC(r1 + c3) * xf3;                         \
		    k2 = READ_SRC(r2 + c0) * xf0 + READ_SRC(r2 +         \
			c1) * xf1 + READ_SRC(r2 + c2) * xf2 +            \
			READ_SRC(r2 + c3) * xf3;                         \
		    k3 = READ_SRC(r3 + c0) * xf0 + READ_SRC(r3 +         \
			c1) * xf1 + READ_SRC(r3 + c2) * xf2 +            \
			READ_SRC(r3 + c3) * xf3;                         \
		    fx = k0 * yf0 + k1 * yf1 + k2 * yf2 + k3 * yf3;      \
		    fx >>= (2*TABLE_SHIFT);                              \
		    RES(dp);                                             \
		    x += dx;                                             \
		}                                                        \
		y += dy;                                                 \
	    }                                                            \
	}

/* *********************************************************** */

#define	MLIB_SRC_EXTEND_BC_INT(FLT, RES, READ_SRC)                           \
	MLIB_SRC_EXTEND_BC_1_INT(dp0, x0, y0, w1, h1, FLT, RES, READ_SRC);   \
	MLIB_SRC_EXTEND_BC_1_INT(dp1, x0, y1, w2, h2, FLT, RES, READ_SRC);   \
	MLIB_SRC_EXTEND_BC_1_INT(dp2, x1, y1, w3, h2, FLT, RES, READ_SRC);   \
	MLIB_SRC_EXTEND_BC_1_INT(dp3, x0, y2, w1, h3, FLT, RES, READ_SRC)

/* *********************************************************** */

#define	MLIB_SRC_EXTEND_BC_1_INT_BLEND(dp, xx, yy, w, h, filter, RES, READ_SRC)\
	{                                                                \
	    mlib_s32 x, y, cx, cy, r0, r1, r2, r3, c0, c1, c2, c3;       \
	    mlib_s16 *fPtr;                                              \
	    mlib_s64 fx, k0, k1, k2, k3, xf0, xf1, xf2, xf3, yf0, yf1,   \
		yf2, yf3;                                                \
	    y = yy;                                                      \
	    for (j = 0; j < h; j++) {                                    \
		x = xx;                                                  \
		cy = y >> MLIB_SHIFT;                                    \
		fPtr =                                                   \
		    (mlib_s16 *)((mlib_u8 *)filter +                     \
		    (((y >> MLIB_XY_SHIFT) & MLIB_XY_MASK)>>1));         \
		yf0 = fPtr[0];                                           \
		yf1 = fPtr[1];                                           \
		yf2 = fPtr[2];                                           \
		yf3 = fPtr[3];                                           \
		INT_BOUND(r0, cy - 1, sh);                               \
		INT_BOUND(r1, cy, sh);                                   \
		INT_BOUND(r2, cy + 1, sh);                               \
		INT_BOUND(r3, cy + 2, sh);                               \
		r0 *= src_stride;                                        \
		r1 *= src_stride;                                        \
		r2 *= src_stride;                                        \
		r3 *= src_stride;                                        \
		for (i = 0; i < w; i++) {                                \
		    cx = x >> MLIB_SHIFT;                                \
		    fPtr =                                               \
			(mlib_s16 *)((mlib_u8 *)filter +                 \
			(((x >> MLIB_XY_SHIFT) & MLIB_XY_MASK)>>1));     \
		    xf0 = fPtr[0];                                       \
		    xf1 = fPtr[1];                                       \
		    xf2 = fPtr[2];                                       \
		    xf3 = fPtr[3];                                       \
		    INT_BOUND(c0, cx - 1, sw);                           \
		    INT_BOUND(c1, cx, sw);                               \
		    INT_BOUND(c2, cx + 1, sw);                           \
		    INT_BOUND(c3, cx + 2, sw);                           \
		    c0 = channels * c0 + ch;                             \
		    c1 = channels * c1 + ch;                             \
		    c2 = channels * c2 + ch;                             \
		    c3 = channels * c3 + ch;                             \
		    k0 = READ_SRC(r0 + c0) * xf0 + READ_SRC(r0 +         \
			c1) * xf1 + READ_SRC(r0 + c2) * xf2 +            \
			READ_SRC(r0 + c3) * xf3;                         \
		    k1 = READ_SRC(r1 + c0) * xf0 + READ_SRC(r1 +         \
			c1) * xf1 + READ_SRC(r1 + c2) * xf2 +            \
			READ_SRC(r1 + c3) * xf3;                         \
		    k2 = READ_SRC(r2 + c0) * xf0 + READ_SRC(r2 +         \
			c1) * xf1 + READ_SRC(r2 + c2) * xf2 +            \
			READ_SRC(r2 + c3) * xf3;                         \
		    k3 = READ_SRC(r3 + c0) * xf0 + READ_SRC(r3 +         \
			c1) * xf1 + READ_SRC(r3 + c2) * xf2 +            \
			READ_SRC(r3 + c3) * xf3;                         \
		    fx = k0 * yf0 + k1 * yf1 + k2 * yf2 + k3 * yf3;      \
		    DIV255(fx, (fx >> (2*TABLE_SHIFT)));                 \
		    RES(dp);                                             \
		    x += dx;                                             \
		}                                                        \
		y += dy;                                                 \
	    }                                                            \
	}

/* *********************************************************** */

#define	MLIB_SRC_EXTEND_BC_INT_BLEND(FLT, RES, READ_SRC)                 \
	MLIB_SRC_EXTEND_BC_1_INT_BLEND(dp0, x0, y0, w1, h1,              \
			FLT, RES, READ_SRC);                             \
	MLIB_SRC_EXTEND_BC_1_INT_BLEND(dp1, x0, y1, w2, h2,              \
			FLT, RES, READ_SRC);                             \
	MLIB_SRC_EXTEND_BC_1_INT_BLEND(dp2, x1, y1, w3, h2,              \
			FLT, RES, READ_SRC);                             \
	MLIB_SRC_EXTEND_BC_1_INT_BLEND(dp3, x0, y2, w1, h3,              \
			FLT, RES, READ_SRC)

/* *********************************************************** */

#define	MLIB_XY_SHIFT	4
#define	MLIB_XY_MASK	(((1 << 8) - 1) << 4)

/* *********************************************************** */

#undef	TABLE_SHIFT
#define	TABLE_SHIFT	2

void
mlib_ImageZoomExtend_U8_Bicubic(
    mlib_work_image *param)
{
	VARIABLE_EDGE(mlib_u8);
	VARIABLE_SRC_EXTEND(mlib_u8);
	DEBUG_PRINT("ZoomExtend_U8_Bicubic");

	for (ch = 0; ch < channels; ch++) {
		MLIB_SRC_EXTEND_BC_INT(mlib_filters_u8_bc,
			RES_U8_INT, READ_SRC);
	}
}

void
mlib_ImageZoomExtend_U8_Bicubic2(
    mlib_work_image *param)
{
	VARIABLE_EDGE(mlib_u8);
	VARIABLE_SRC_EXTEND(mlib_u8);
	DEBUG_PRINT("ZoomExtend_U8_Bicubic2");

	for (ch = 0; ch < channels; ch++) {
		MLIB_SRC_EXTEND_BC_INT(mlib_filters_u8_bc2,
			RES_U8_INT, READ_SRC);
	}
}

/* *********************************************************** */

void
mlib_ImageZoomExtend_U8_Bicubic_Blend(
    mlib_work_image *param,
    mlib_s32 alp_ind)
{
	VARIABLE_EDGE(mlib_u8);
	VARIABLE_SRC_EXTEND(mlib_u8);
	DEBUG_PRINT("ZoomExtend_U8_Bicubic_Blend");

	for (ch = 0; ch < channels; ch++) {
		if (!alp_ind) {
			MLIB_SRC_EXTEND_BC_INT(mlib_filters_u8_bc, RES_U8_INT,
			    READ_SRC);
		} else {
			MLIB_SRC_EXTEND_BC_INT_BLEND(mlib_filters_u8_bc,
				RES_U8_INT, READ_SRC_BLEND_INT_S32_NO_255);
		}
		alp_ind--;
	}
}

void
mlib_ImageZoomExtend_U8_Bicubic2_Blend(
    mlib_work_image *param,
    mlib_s32 alp_ind)
{
	VARIABLE_EDGE(mlib_u8);
	VARIABLE_SRC_EXTEND(mlib_u8);
	DEBUG_PRINT("ZoomExtend_U8_Bicubic2_Blend");

	for (ch = 0; ch < channels; ch++) {
		if (!alp_ind) {
			MLIB_SRC_EXTEND_BC_INT(mlib_filters_u8_bc2, RES_U8_INT,
			    READ_SRC);
		} else {
			MLIB_SRC_EXTEND_BC_INT_BLEND(mlib_filters_u8_bc2,
				RES_U8_INT, READ_SRC_BLEND_INT_S32_NO_255);
		}
		alp_ind--;
	}
}

/* *********************************************************** */

#undef  READ_SRC
#define	READ_SRC(ind)	sp[ind]

/* *********************************************************** */

#undef  MLIB_XY_SHIFT
#undef  MLIB_XY_MASK

/* *********************************************************** */

#define	MLIB_XY_SHIFT	3
#define	MLIB_XY_MASK	(((1 << 9) - 1) << 4)

/* *********************************************************** */

#undef	TABLE_SHIFT
#define	TABLE_SHIFT	7

void
mlib_ImageZoomExtend_S16_Bicubic(
    mlib_work_image *param)
{
	VARIABLE_EDGE(mlib_s16);
	VARIABLE_SRC_EXTEND(mlib_s16);
	DEBUG_PRINT("ZoomExtend_S16_Bicubic");

	for (ch = 0; ch < channels; ch++) {
		MLIB_SRC_EXTEND_BC_INT(mlib_filters_s16_bc,
			RES_S16_INT, READ_SRC);
	}
}

/* *********************************************************** */

void
mlib_ImageZoomExtend_S16_Bicubic2(
    mlib_work_image *param)
{
	VARIABLE_EDGE(mlib_s16);
	VARIABLE_SRC_EXTEND(mlib_s16);
	DEBUG_PRINT("ZoomExtend_S16_Bicubic2");

	for (ch = 0; ch < channels; ch++) {
		MLIB_SRC_EXTEND_BC_INT(mlib_filters_s16_bc2,
			RES_S16_INT, READ_SRC);
	}
}

/* *********************************************************** */

void
mlib_ImageZoomExtend_U16_Bicubic(
    mlib_work_image *param)
{
	VARIABLE_EDGE(mlib_u16);
	VARIABLE_SRC_EXTEND(mlib_u16);
	DEBUG_PRINT("ZoomExtend_U16_Bicubic");

	for (ch = 0; ch < channels; ch++) {
		MLIB_SRC_EXTEND_BC_INT(mlib_filters_s16_bc,
			RES_U16_INT, READ_SRC);
	}
}

/* *********************************************************** */

void
mlib_ImageZoomExtend_U16_Bicubic2(
    mlib_work_image *param)
{
	VARIABLE_EDGE(mlib_u16);
	VARIABLE_SRC_EXTEND(mlib_u16);
	DEBUG_PRINT("ZoomExtend_U16_Bicubic2");

	for (ch = 0; ch < channels; ch++) {
		MLIB_SRC_EXTEND_BC_INT(mlib_filters_s16_bc2,
			RES_U16_INT, READ_SRC);
	}
}

/* *********************************************************** */

#define	s0(x)	(-0.5 * (x) * (x) * (x) + (x) * (x) - 0.5 * (x))
#define	s1(x)	(1.5 * (x) * (x) * (x) - 2.5 * (x) * (x) + 1.0)
#define	s2(x)	(-1.5 * (x) * (x) * (x) + 2.0 * (x) * (x) + 0.5 * (x))
#define	s3(x)	(0.5 * (x) * (x) * (x) - 0.5 * (x) * (x))

/* *********************************************************** */

#ifdef MLIB_USE_FTOI_CLAMPING

/* *********************************************************** */

#define	RES_S32(dp)	dp[j * stride_##dp + i * channels + ch] = fx

#else /* MLIB_USE_FTOI_CLAMPING */

/* *********************************************************** */

#define	RES_S32(dp)                                              \
	if (fx >= MLIB_S32_MAX)                                  \
	    fx = MLIB_S32_MAX;                                   \
	if (fx <= MLIB_S32_MIN)                                  \
	    fx = MLIB_S32_MIN;                                   \
	dp[j * stride_##dp + i * channels + ch] = (mlib_s32)fx

#endif /* MLIB_USE_FTOI_CLAMPING */

/* *********************************************************** */

#define	RES_STORE_F32(dp)                                                  \
	dp[j * stride_##dp + i * channels + ch] = (mlib_f32)fx

#define	RES_STORE(dp)	dp[j * stride_##dp + i * channels + ch] = fx

/* *********************************************************** */

#define	MLIB_SRC_EXTEND_BC_S32(dp, xx, yy, w, h, RES)                      \
	{                                                                  \
	    mlib_s32 x, y, cx, cy, r0, r1, r2, r3, c0, c1, c2,             \
		c3;                                                        \
	    mlib_d64 fx, ddy, ddx, k0, k1, k2, k3, xf0, xf1, xf2, xf3,     \
		yf0, yf1, yf2, yf3;                                        \
	    y = yy;                                                        \
	    for (j = 0; j < h; j++) {                                      \
		x = xx;                                                    \
		cy = y >> MLIB_SHIFT;                                      \
		ddy = (y & MLIB_MASK) * MLIB_SCALE;                        \
		yf0 = s0(ddy);                                             \
		yf1 = s1(ddy);                                             \
		yf2 = s2(ddy);                                             \
		yf3 = s3(ddy);                                             \
		INT_BOUND(r0, cy - 1, sh);                                 \
		INT_BOUND(r1, cy, sh);                                     \
		INT_BOUND(r2, cy + 1, sh);                                 \
		INT_BOUND(r3, cy + 2, sh);                                 \
		r0 *= src_stride;                                          \
		r1 *= src_stride;                                          \
		r2 *= src_stride;                                          \
		r3 *= src_stride;                                          \
		for (i = 0; i < w; i++) {                                  \
		    cx = x >> MLIB_SHIFT;                                  \
		    ddx = (x & MLIB_MASK) * MLIB_SCALE;                    \
		    xf0 = s0(ddx);                                         \
		    xf1 = s1(ddx);                                         \
		    xf2 = s2(ddx);                                         \
		    xf3 = s3(ddx);                                         \
		    INT_BOUND(c0, cx - 1, sw);                             \
		    INT_BOUND(c1, cx, sw);                                 \
		    INT_BOUND(c2, cx + 1, sw);                             \
		    INT_BOUND(c3, cx + 2, sw);                             \
		    c0 = channels * c0 + ch;                               \
		    c1 = channels * c1 + ch;                               \
		    c2 = channels * c2 + ch;                               \
		    c3 = channels * c3 + ch;                               \
		    k0 = sp[r0 + c0] * xf0 + sp[r0 + c1] * xf1 + sp[r0 +   \
			c2] * xf2 + sp[r0 + c3] * xf3;                     \
		    k1 = sp[r1 + c0] * xf0 + sp[r1 + c1] * xf1 + sp[r1 +   \
			c2] * xf2 + sp[r1 + c3] * xf3;                     \
		    k2 = sp[r2 + c0] * xf0 + sp[r2 + c1] * xf1 + sp[r2 +   \
			c2] * xf2 + sp[r2 + c3] * xf3;                     \
		    k3 = sp[r3 + c0] * xf0 + sp[r3 + c1] * xf1 + sp[r3 +   \
			c2] * xf2 + sp[r3 + c3] * xf3;                     \
		    fx = k0 * yf0 + k1 * yf1 + k2 * yf2 + k3 * yf3;        \
		    RES(dp);                                               \
		    x += dx;                                               \
		}                                                          \
		y += dy;                                                   \
	    }                                                              \
	}

/* *********************************************************** */

void
mlib_ImageZoomExtend_S32_Bicubic(
    mlib_work_image *param)
{
	VARIABLE_EDGE(mlib_s32);
	VARIABLE_SRC_EXTEND(mlib_s32);

	for (ch = 0; ch < channels; ch++) {
		MLIB_SRC_EXTEND_BC_S32(dp0, x0, y0, w1, h1, RES_S32);
		MLIB_SRC_EXTEND_BC_S32(dp1, x0, y1, w2, h2, RES_S32);
		MLIB_SRC_EXTEND_BC_S32(dp2, x1, y1, w3, h2, RES_S32);
		MLIB_SRC_EXTEND_BC_S32(dp3, x0, y2, w1, h3, RES_S32);
	}
}

/* *********************************************************** */

void
mlib_ImageZoomExtend_F32_Bicubic(
    mlib_work_image *param)
{
	VARIABLE_EDGE(mlib_f32);
	VARIABLE_SRC_EXTEND(mlib_f32);

	for (ch = 0; ch < channels; ch++) {
		MLIB_SRC_EXTEND_BC_S32(dp0, x0, y0, w1, h1, RES_STORE_F32);
		MLIB_SRC_EXTEND_BC_S32(dp1, x0, y1, w2, h2, RES_STORE_F32);
		MLIB_SRC_EXTEND_BC_S32(dp2, x1, y1, w3, h2, RES_STORE_F32);
		MLIB_SRC_EXTEND_BC_S32(dp3, x0, y2, w1, h3, RES_STORE_F32);
	}
}

/* *********************************************************** */

void
mlib_ImageZoomExtend_D64_Bicubic(
    mlib_work_image *param)
{
	VARIABLE_EDGE(mlib_d64);
	VARIABLE_SRC_EXTEND(mlib_d64);

	for (ch = 0; ch < channels; ch++) {
		MLIB_SRC_EXTEND_BC_S32(dp0, x0, y0, w1, h1, RES_STORE);
		MLIB_SRC_EXTEND_BC_S32(dp1, x0, y1, w2, h2, RES_STORE);
		MLIB_SRC_EXTEND_BC_S32(dp2, x1, y1, w3, h2, RES_STORE);
		MLIB_SRC_EXTEND_BC_S32(dp3, x0, y2, w1, h3, RES_STORE);
	}
}

/* *********************************************************** */

#undef  s0
#undef  s1
#undef  s2
#undef  s3

/* *********************************************************** */

#define	s0(x)	(- (x) * (x) * (x) + 2.0 * (x) * (x) - (x))
#define	s1(x)	((x) * (x) * (x) - 2.0 * (x) * (x) + 1.0)
#define	s2(x)	(- (x) * (x) * (x) + (x) * (x) + (x))
#define	s3(x)	((x) * (x) * (x) - (x) * (x))

/* *********************************************************** */

void
mlib_ImageZoomExtend_S32_Bicubic2(
    mlib_work_image *param)
{
	VARIABLE_EDGE(mlib_s32);
	VARIABLE_SRC_EXTEND(mlib_s32);

	for (ch = 0; ch < channels; ch++) {
		MLIB_SRC_EXTEND_BC_S32(dp0, x0, y0, w1, h1, RES_S32);
		MLIB_SRC_EXTEND_BC_S32(dp1, x0, y1, w2, h2, RES_S32);
		MLIB_SRC_EXTEND_BC_S32(dp2, x1, y1, w3, h2, RES_S32);
		MLIB_SRC_EXTEND_BC_S32(dp3, x0, y2, w1, h3, RES_S32);
	}
}

/* *********************************************************** */

void
mlib_ImageZoomExtend_F32_Bicubic2(
    mlib_work_image *param)
{
	VARIABLE_EDGE(mlib_f32);
	VARIABLE_SRC_EXTEND(mlib_f32);

	for (ch = 0; ch < channels; ch++) {
		MLIB_SRC_EXTEND_BC_S32(dp0, x0, y0, w1, h1, RES_STORE_F32);
		MLIB_SRC_EXTEND_BC_S32(dp1, x0, y1, w2, h2, RES_STORE_F32);
		MLIB_SRC_EXTEND_BC_S32(dp2, x1, y1, w3, h2, RES_STORE_F32);
		MLIB_SRC_EXTEND_BC_S32(dp3, x0, y2, w1, h3, RES_STORE_F32);
	}
}

/* *********************************************************** */

void
mlib_ImageZoomExtend_D64_Bicubic2(
    mlib_work_image *param)
{
	VARIABLE_EDGE(mlib_d64);
	VARIABLE_SRC_EXTEND(mlib_d64);

	for (ch = 0; ch < channels; ch++) {
		MLIB_SRC_EXTEND_BC_S32(dp0, x0, y0, w1, h1, RES_STORE);
		MLIB_SRC_EXTEND_BC_S32(dp1, x0, y1, w2, h2, RES_STORE);
		MLIB_SRC_EXTEND_BC_S32(dp2, x1, y1, w3, h2, RES_STORE);
		MLIB_SRC_EXTEND_BC_S32(dp3, x0, y2, w1, h3, RES_STORE);
	}
}

#else /* _NO_LONGLONG */

#include <mlib_image.h>
#include <mlib_ImageFilters.h>
#include <mlib_ImageZoom.h>
#include <mlib_ImageColormap.h>
#include <mlib_ImageDivTables.h>

/* *********************************************************** */
/* Clearing */
/* *********************************************************** */

#define	MLIB_CLEAR(dp, w, h, color)                                  \
	for (j = 0; j < h; j++) {                                    \
	    for (i = 0; i < w; i++) {                                \
		(dp)[j * stride_##dp + i * channels + ch] = color;   \
	    }                                                        \
	}

/* *********************************************************** */

void
mlib_ImageZoomZeroEdge_U8(
    mlib_work_image *param)
{
	VARIABLE_EDGE(mlib_u8);
	mlib_u8 color = (mlib_u8)GetElemStruct(color);

	for (ch = 0; ch < channels; ch++) {

		MLIB_CLEAR(dp0, w1, h1, color);
		MLIB_CLEAR(dp1, w2, h2, color);
		MLIB_CLEAR(dp2, w3, h2, color);
		MLIB_CLEAR(dp3, w1, h3, color);
	}
}

/* *********************************************************** */

void
mlib_ImageZoomZeroEdge_S16(
    mlib_work_image *param)
{
	VARIABLE_EDGE(mlib_s16);
	mlib_s16 color = (mlib_s16)GetElemStruct(color);

	for (ch = 0; ch < channels; ch++) {

		MLIB_CLEAR(dp0, w1, h1, color);
		MLIB_CLEAR(dp1, w2, h2, color);
		MLIB_CLEAR(dp2, w3, h2, color);
		MLIB_CLEAR(dp3, w1, h3, color);
	}
}

/* *********************************************************** */

void
mlib_ImageZoomZeroEdge_S32(
    mlib_work_image *param)
{
	VARIABLE_EDGE(mlib_s32);
	mlib_s32 color = (mlib_s32)GetElemStruct(color);

	for (ch = 0; ch < channels; ch++) {

		MLIB_CLEAR(dp0, w1, h1, color);
		MLIB_CLEAR(dp1, w2, h2, color);
		MLIB_CLEAR(dp2, w3, h2, color);
		MLIB_CLEAR(dp3, w1, h3, color);
	}
}

/* *********************************************************** */

void
mlib_ImageZoomZeroEdge_F32(
    mlib_work_image *param)
{
	VARIABLE_EDGE(mlib_f32);
	mlib_f32 color = 0;

	for (ch = 0; ch < channels; ch++) {

		MLIB_CLEAR(dp0, w1, h1, color);
		MLIB_CLEAR(dp1, w2, h2, color);
		MLIB_CLEAR(dp2, w3, h2, color);
		MLIB_CLEAR(dp3, w1, h3, color);
	}
}

/* *********************************************************** */

void
mlib_ImageZoomZeroEdge_D64(
    mlib_work_image *param)
{
	VARIABLE_EDGE(mlib_d64);
	mlib_d64 color = 0;

	for (ch = 0; ch < channels; ch++) {

		MLIB_CLEAR(dp0, w1, h1, color);
		MLIB_CLEAR(dp1, w2, h2, color);
		MLIB_CLEAR(dp2, w3, h2, color);
		MLIB_CLEAR(dp3, w1, h3, color);
	}
}

/* *********************************************************** */
/* Up Nearest */
/* *********************************************************** */

#define	VARIABLE_UP_NEAREST(FORMAT)                                   \
	mlib_s32 src_stride = GetElemStruct(src_stride),              \
	    dx = GetElemStruct(DX),                                   \
	    dy = GetElemStruct(DY),                                   \
	    x0 = GetElemSubStruct(nearest, srcX),                     \
	    x1 = x0 + (w1 - w3) * dx,                                 \
	    y0 = GetElemSubStruct(nearest, srcY),                     \
	    y1 = y0 + h1 * dy,                                        \
	    y2 = y1 + h2 * dy;                                        \
	FORMAT *sp = (FORMAT *) GetElemStruct(sp)

/* *********************************************************** */

#define	MLIB_UP_NEAREST(dp, xx, yy, w, h)                           \
	{                                                           \
	    mlib_s32 x, y, cx, cy;                                  \
	                                                            \
	    y = yy;                                                 \
	    for (j = 0; j < h; j++) {                               \
		x = xx;                                             \
		cy = y >> MLIB_SHIFT;                               \
		for (i = 0; i < w; i++) {                           \
		    cx = x >> MLIB_SHIFT;                           \
		    (dp)[j * stride_##dp + i * channels + ch] =     \
			sp[cy * src_stride + cx * channels + ch];   \
		    x += dx;                                        \
		}                                                   \
		y += dy;                                            \
	    }                                                       \
	}

/* *********************************************************** */

void
mlib_ImageZoomUpNearest_U8(
    mlib_work_image *param)
{
	VARIABLE_EDGE(mlib_u8);
	VARIABLE_UP_NEAREST(mlib_u8);

	for (ch = 0; ch < channels; ch++) {

		MLIB_UP_NEAREST(dp0, x0, y0, w1, h1);
		MLIB_UP_NEAREST(dp1, x0, y1, w2, h2);
		MLIB_UP_NEAREST(dp2, x1, y1, w3, h2);
		MLIB_UP_NEAREST(dp3, x0, y2, w1, h3);
	}
}

/* *********************************************************** */

void
mlib_ImageZoomUpNearest_S16(
    mlib_work_image *param)
{
	VARIABLE_EDGE(mlib_s16);
	VARIABLE_UP_NEAREST(mlib_s16);

	for (ch = 0; ch < channels; ch++) {

		MLIB_UP_NEAREST(dp0, x0, y0, w1, h1);
		MLIB_UP_NEAREST(dp1, x0, y1, w2, h2);
		MLIB_UP_NEAREST(dp2, x1, y1, w3, h2);
		MLIB_UP_NEAREST(dp3, x0, y2, w1, h3);
	}
}

/* *********************************************************** */

void
mlib_ImageZoomUpNearest_S32(
    mlib_work_image *param)
{
	VARIABLE_EDGE(mlib_s32);
	VARIABLE_UP_NEAREST(mlib_s32);

	for (ch = 0; ch < channels; ch++) {

		MLIB_UP_NEAREST(dp0, x0, y0, w1, h1);
		MLIB_UP_NEAREST(dp1, x0, y1, w2, h2);
		MLIB_UP_NEAREST(dp2, x1, y1, w3, h2);
		MLIB_UP_NEAREST(dp3, x0, y2, w1, h3);
	}
}

/* *********************************************************** */

void
mlib_ImageZoomUpNearest_F32(
    mlib_work_image *param)
{
	VARIABLE_EDGE(mlib_f32);
	VARIABLE_UP_NEAREST(mlib_f32);

	for (ch = 0; ch < channels; ch++) {

		MLIB_UP_NEAREST(dp0, x0, y0, w1, h1);
		MLIB_UP_NEAREST(dp1, x0, y1, w2, h2);
		MLIB_UP_NEAREST(dp2, x1, y1, w3, h2);
		MLIB_UP_NEAREST(dp3, x0, y2, w1, h3);
	}
}

/* *********************************************************** */

void
mlib_ImageZoomUpNearest_D64(
    mlib_work_image *param)
{
	VARIABLE_EDGE(mlib_d64);
	VARIABLE_UP_NEAREST(mlib_d64);

	for (ch = 0; ch < channels; ch++) {

		MLIB_UP_NEAREST(dp0, x0, y0, w1, h1);
		MLIB_UP_NEAREST(dp1, x0, y1, w2, h2);
		MLIB_UP_NEAREST(dp2, x1, y1, w3, h2);
		MLIB_UP_NEAREST(dp3, x0, y2, w1, h3);
	}
}

/* *********************************************************** */
/* Pseudo-extended source image */
/* *********************************************************** */

#define	VARIABLE_SRC_EXTEND(FORMAT)                                   \
	mlib_s32 sw = GetElemStruct(width) - 1,                       \
	    sh = GetElemStruct(height) - 1,                           \
	    max_x = sw,                                               \
	    max_y = sh,                                               \
	    src_stride = GetElemStruct(src_stride),                   \
	    dx = GetElemStruct(DX),                                   \
	    dy = GetElemStruct(DY),                                   \
	    x0 = GetElemSubStruct(current, srcX) - dx * w2,           \
	    x1 = GetElemSubStruct(current, srcX)                      \
		+ GetElemSubStruct(current, width) * dx,              \
	    y0 = GetElemSubStruct(current, srcY) - dy * h1,           \
	    y1 = GetElemSubStruct(current, srcY),                     \
	    y2 = y1 + GetElemSubStruct(current, height) * dy;         \
	    FORMAT *sp = (FORMAT *) GetElemStruct(sp)

/* *********************************************************** */

#define	FLOAT_FORMAT	mlib_f32

/* *********************************************************** */

#define	INT_BOUND(DST, SRC, MAX_VAL)                            \
	{                                                       \
	    mlib_s32 m1, m2;                                    \
	                                                        \
	    m1 = (SRC) >> MLIB_SIGN_SHIFT;                      \
	    m2 = (MAX_VAL - (SRC)) >> MLIB_SIGN_SHIFT;          \
	    DST = (((SRC) & ~(m1 | m2)) | (MAX_VAL & m2));      \
	}

/* *********************************************************** */

#define	READ_SRC(ind)	sp[ind]

#define	READ_SRC_U8(ind)	mlib_U82F32[sp[ind]]

#define	READ_SRC_BLEND(ind)                                     \
	(mlib_U82F32[sp[ind]] * mlib_U82F32[sp[ind + alp_ind]]  \
	    * (1.0f / 255))

/* *********************************************************** */

#define	NN_COORD(x)                                             \
	c##x = x >> MLIB_SHIFT;                                 \
	INT_BOUND(c##x, c##x, max_##x)

#define	MLIB_SRC_EXTEND_NN_1(dp, xx, yy, w, h, READ_SRC)        \
	{                                                       \
	    mlib_s32 x, y, cx, cy;                              \
	                                                        \
	    y = yy;                                             \
	    for (j = 0; j < h; j++) {                           \
		x = xx;                                         \
		NN_COORD(y);                                    \
		cy *= src_stride;                               \
		for (i = 0; i < w; i++) {                       \
		    NN_COORD(x);                                \
		    cx = channels * cx + ch;                    \
		    dp[j * stride_##dp + i * channels + ch] =   \
			READ_SRC(cy + cx);                      \
		    x += dx;                                    \
		}                                               \
		y += dy;                                        \
	    }                                                   \
	}

/* *********************************************************** */

#define	MLIB_SRC_EXTEND_NN(READ_SRC)                            \
	MLIB_SRC_EXTEND_NN_1(dp0, x0, y0, w1, h1, READ_SRC);    \
	MLIB_SRC_EXTEND_NN_1(dp1, x0, y1, w2, h2, READ_SRC);    \
	MLIB_SRC_EXTEND_NN_1(dp2, x1, y1, w3, h2, READ_SRC);    \
	MLIB_SRC_EXTEND_NN_1(dp3, x0, y2, w1, h3, READ_SRC)

/* *********************************************************** */

void
mlib_ImageZoomExtend_U8_Nearest(
    mlib_work_image *param)
{
	VARIABLE_EDGE(mlib_u8);
	VARIABLE_SRC_EXTEND(mlib_u8);

	for (ch = 0; ch < channels; ch++) {
		MLIB_SRC_EXTEND_NN(READ_SRC);
	}
}

/* *********************************************************** */

void
mlib_ImageZoomExtend_U8_Nearest_Blend(
    mlib_work_image *param,
    mlib_s32 alp_ind)
{
	VARIABLE_EDGE(mlib_u8);
	VARIABLE_SRC_EXTEND(mlib_u8);

	for (ch = 0; ch < channels; ch++) {
		if (!alp_ind) {
			MLIB_SRC_EXTEND_NN(READ_SRC);
		} else {
			MLIB_SRC_EXTEND_NN((mlib_u8)READ_SRC_BLEND);
		}
		alp_ind--;
	}
}

/* *********************************************************** */

void
mlib_ImageZoomExtend_S16_Nearest(
    mlib_work_image *param)
{
	VARIABLE_EDGE(mlib_s16);
	VARIABLE_SRC_EXTEND(mlib_s16);

	src_stride /= 2;
	stride_dp0 /= 2;
	stride_dp1 /= 2;
	stride_dp2 /= 2;
	stride_dp3 /= 2;

	for (ch = 0; ch < channels; ch++) {
		MLIB_SRC_EXTEND_NN(READ_SRC);
	}
}

/* *********************************************************** */

void
mlib_ImageZoomExtend_U16_Nearest(
    mlib_work_image *param)
{
	VARIABLE_EDGE(mlib_u16);
	VARIABLE_SRC_EXTEND(mlib_u16);

	src_stride /= 2;
	stride_dp0 /= 2;
	stride_dp1 /= 2;
	stride_dp2 /= 2;
	stride_dp3 /= 2;

	for (ch = 0; ch < channels; ch++) {
		MLIB_SRC_EXTEND_NN(READ_SRC);
	}
}

/* *********************************************************** */

void
mlib_ImageZoomExtend_S32_Nearest(
    mlib_work_image *param)
{
	VARIABLE_EDGE(mlib_s32);
	VARIABLE_SRC_EXTEND(mlib_s32);

	src_stride /= 4;
	stride_dp0 /= 4;
	stride_dp1 /= 4;
	stride_dp2 /= 4;
	stride_dp3 /= 4;

	for (ch = 0; ch < channels; ch++) {
		MLIB_SRC_EXTEND_NN(READ_SRC);
	}
}

/* *********************************************************** */

void
mlib_ImageZoomExtend_F32_Nearest(
    mlib_work_image *param)
{
	VARIABLE_EDGE(mlib_f32);
	VARIABLE_SRC_EXTEND(mlib_f32);

	src_stride /= 4;
	stride_dp0 /= 4;
	stride_dp1 /= 4;
	stride_dp2 /= 4;
	stride_dp3 /= 4;

	for (ch = 0; ch < channels; ch++) {
		MLIB_SRC_EXTEND_NN(READ_SRC);
	}
}

/* *********************************************************** */

void
mlib_ImageZoomExtend_D64_Nearest(
    mlib_work_image *param)
{
	VARIABLE_EDGE(mlib_d64);
	VARIABLE_SRC_EXTEND(mlib_d64);

	src_stride /= 8;
	stride_dp0 /= 8;
	stride_dp1 /= 8;
	stride_dp2 /= 8;
	stride_dp3 /= 8;

	for (ch = 0; ch < channels; ch++) {
		MLIB_SRC_EXTEND_NN(READ_SRC);
	}
}

/* *********************************************************** */

#define	BL_COORD_int(x)                                         \
	f##x = (x >> 5) & 0x7ff;                                \
	c##x = x >> MLIB_SHIFT;                                 \
	s##x = c##x + 1;                                        \
	c##x = c##x & ~(c##x >> MLIB_SIGN_SHIFT);               \
	s##x = s##x & ~(s##x >> MLIB_SIGN_SHIFT);               \
	if (c##x > max_##x)                                     \
	    c##x = max_##x;                                     \
	if (s##x > max_##x)                                     \
	    s##x = max_##x

#define	MLIB_SRC_EXTEND_BL_1_int(dp, xx, yy, w, h)                     \
	{                                                              \
	    mlib_s32 x, y, cx, cy, sx, sy;                             \
	    mlib_s32 fy, fx, fres, a0, a1, a2, a3;                     \
                                                                       \
	    y = yy;                                                    \
	    for (j = 0; j < h; j++) {                                  \
		x = xx;                                                \
		BL_COORD_int(y);                                       \
		cy *= src_stride;                                      \
		sy *= src_stride;                                      \
		for (i = 0; i < w; i++) {                              \
		    BL_COORD_int(x);                                   \
		    cx = channels * cx + ch;                           \
		    sx = channels * sx + ch;                           \
		    a0 = sp[cy + cx];                                  \
		    a1 = sp[cy + sx];                                  \
		    a3 = sp[sy + cx];                                  \
		    a2 = sp[sy + sx];                                  \
		    fres = ((a0 << 22) + (fy << 11)*(a3 - a0) +        \
			(fx << 11) * (a1 - a0) + fx * fy *             \
			(a0 + a2 - a3 - a1));                          \
		    fres = (fres + (1 << 21)) >> 22;                   \
		    dp[j * stride_##dp + i * channels + ch] = fres;    \
		    x += dx;                                           \
		}                                                      \
		y += dy;                                               \
	    }                                                          \
	}

#define	MLIB_SRC_EXTEND_BL_int()                        \
	MLIB_SRC_EXTEND_BL_1_int(dp0, x0, y0, w1, h1);  \
	MLIB_SRC_EXTEND_BL_1_int(dp1, x0, y1, w2, h2);  \
	MLIB_SRC_EXTEND_BL_1_int(dp2, x1, y1, w3, h2);  \
	MLIB_SRC_EXTEND_BL_1_int(dp3, x0, y2, w1, h3)

/* *********************************************************** */

#define	BL_COORD(x)                                             \
	f##x = (FLOAT_FORMAT) ((x & MLIB_MASK) * MLIB_SCALE);   \
	c##x = x >> MLIB_SHIFT;                                 \
	s##x = c##x + 1;                                        \
	c##x = c##x & ~(c##x >> MLIB_SIGN_SHIFT);               \
	s##x = s##x & ~(s##x >> MLIB_SIGN_SHIFT);               \
	if (c##x > max_##x)                                     \
	    c##x = max_##x;                                     \
	if (s##x > max_##x)                                     \
	    s##x = max_##x

#define	MLIB_SRC_EXTEND_BL_1(dp, xx, yy, w, h, READ_SRC, ROUND, FORMAT)       \
	{                                                                     \
	    mlib_s32 x, y, cx, cy, sx, sy;                                    \
	    FLOAT_FORMAT fy, fx, fres, a0, a1, a2, a3;                        \
	                                                                      \
	    y = yy;                                                           \
	    for (j = 0; j < h; j++) {                                         \
		x = xx;                                                       \
		BL_COORD(y);                                                  \
		cy *= src_stride;                                             \
		sy *= src_stride;                                             \
		for (i = 0; i < w; i++) {                                     \
		    BL_COORD(x);                                              \
		    cx = channels * cx + ch;                                  \
		    sx = channels * sx + ch;                                  \
		    a0 = (FLOAT_FORMAT) READ_SRC(cy + cx);                    \
		    a1 = (FLOAT_FORMAT) READ_SRC(cy + sx);                    \
		    a3 = (FLOAT_FORMAT) READ_SRC(sy + cx);                    \
		    a2 = (FLOAT_FORMAT) READ_SRC(sy + sx);                    \
		    fres =                                                    \
			(a0 + fy * (a3 - a0) + fx * (a1 - a0) +               \
			fx * fy * (a0 + a2 - a3 - a1));                       \
		    ROUND(fres)                                               \
		    dp[j * stride_##dp + i * channels + ch] = (FORMAT) fres;  \
		    x += dx;                                                  \
		}                                                             \
		y += dy;                                                      \
	    }                                                                 \
	}

#define	MLIB_SRC_EXTEND_BL(READ_SRC, ROUND, FORMAT)                            \
	MLIB_SRC_EXTEND_BL_1(dp0, x0, y0, w1, h1, READ_SRC, ROUND_INT, FORMAT);\
	MLIB_SRC_EXTEND_BL_1(dp1, x0, y1, w2, h2, READ_SRC, ROUND_INT, FORMAT);\
	MLIB_SRC_EXTEND_BL_1(dp2, x1, y1, w3, h2, READ_SRC, ROUND_INT, FORMAT);\
	MLIB_SRC_EXTEND_BL_1(dp3, x0, y2, w1, h3, READ_SRC, ROUND_INT, FORMAT)

/* *********************************************************** */

#define	ROUND_INT(fres)                                         \
	fres += (FLOAT_FORMAT) 0.5;                             \
	if (fres < 0)                                           \
	    fres -= 1;

/* *********************************************************** */

#define	NO_ROUND(fres)

/* *********************************************************** */

void
mlib_ImageZoomExtend_U8_Bilinear(
    mlib_work_image *param)
{
	VARIABLE_EDGE(mlib_u8);
	VARIABLE_SRC_EXTEND(mlib_u8);

	for (ch = 0; ch < channels; ch++) {
#ifndef __sparc	/* for x86, using integer multiplies is faster */
		MLIB_SRC_EXTEND_BL_int();
#else
		MLIB_SRC_EXTEND_BL(READ_SRC_U8, ROUND_INT, mlib_u8);
#endif
	}
}

/* *********************************************************** */

void
mlib_ImageZoomExtend_U8_Bilinear_Blend(
    mlib_work_image *param,
    mlib_s32 alp_ind)
{
	VARIABLE_EDGE(mlib_u8);
	VARIABLE_SRC_EXTEND(mlib_u8);

	for (ch = 0; ch < channels; ch++) {
		if (!alp_ind) {
			MLIB_SRC_EXTEND_BL(READ_SRC, ROUND_INT, mlib_u8);
		} else {
			MLIB_SRC_EXTEND_BL(READ_SRC_BLEND, ROUND_INT, mlib_u8);
		}
		alp_ind--;
	}
}

/* *********************************************************** */

void
mlib_ImageZoomExtend_S16_Bilinear(
    mlib_work_image *param)
{
	VARIABLE_EDGE(mlib_s16);
	VARIABLE_SRC_EXTEND(mlib_s16);

	for (ch = 0; ch < channels; ch++) {
		MLIB_SRC_EXTEND_BL(READ_SRC, ROUND_INT, mlib_s16);
	}
}

/* *********************************************************** */

void
mlib_ImageZoomExtend_U16_Bilinear(
    mlib_work_image *param)
{
	VARIABLE_EDGE(mlib_u16);
	VARIABLE_SRC_EXTEND(mlib_u16);

	for (ch = 0; ch < channels; ch++) {
		MLIB_SRC_EXTEND_BL(READ_SRC, ROUND_INT, mlib_u16);
	}
}

/* *********************************************************** */

void
mlib_ImageZoomExtend_F32_Bilinear(
    mlib_work_image *param)
{
	VARIABLE_EDGE(mlib_f32);
	VARIABLE_SRC_EXTEND(mlib_f32);

	for (ch = 0; ch < channels; ch++) {
		MLIB_SRC_EXTEND_BL(READ_SRC, NO_ROUND, mlib_f32);
	}
}

/* *********************************************************** */

#undef FLOAT_FORMAT
#define	FLOAT_FORMAT	mlib_d64

/* *********************************************************** */

void
mlib_ImageZoomExtend_S32_Bilinear(
    mlib_work_image *param)
{
	VARIABLE_EDGE(mlib_s32);
	VARIABLE_SRC_EXTEND(mlib_s32);

	for (ch = 0; ch < channels; ch++) {
		MLIB_SRC_EXTEND_BL(READ_SRC, ROUND_INT, mlib_s32);
	}
}

/* *********************************************************** */

void
mlib_ImageZoomExtend_D64_Bilinear(
    mlib_work_image *param)
{
	VARIABLE_EDGE(mlib_d64);
	VARIABLE_SRC_EXTEND(mlib_d64);

	for (ch = 0; ch < channels; ch++) {
		MLIB_SRC_EXTEND_BL(READ_SRC, NO_ROUND, mlib_d64);
	}
}

/* *********************************************************** */

#ifdef MLIB_USE_FTOI_CLAMPING

#define	RES_U8(dp)                                                 \
	dp[j * stride_##dp + i * channels + ch] =                  \
	    ((mlib_s32)(fx - (mlib_f32)0x7F800000) >> 24) ^ 0x80

#define	RES_S16(dp)                                             \
	dp[j * stride_##dp + i * channels + ch] =               \
	    (mlib_s32)fx >> MLIB_SHIFT

#define	RES_U16(dp)                                             \
	dp[j * stride_##dp + i * channels + ch] =               \
	    ((mlib_s32)(fx -                                    \
	    (mlib_d64)0x7FFF8000) >> MLIB_SHIFT) ^ 0x8000

#else /* MLIB_USE_FTOI_CLAMPING */

#define	RES_U8(dp)                                              \
	{                                                       \
	    mlib_d64 sat_tmp = fx - (mlib_d64)0x7F800000;       \
	                                                        \
	    if (sat_tmp >= MLIB_S32_MAX)                        \
		sat_tmp = MLIB_S32_MAX;                         \
	    if (sat_tmp <= MLIB_S32_MIN)                        \
		sat_tmp = MLIB_S32_MIN;                         \
	    dp[j * stride_##dp + i * channels + ch] =           \
		((mlib_s32)sat_tmp >> 24) ^ 0x80;               \
	}

#define	RES_S16(dp)                                             \
	{                                                       \
	    mlib_d64 sat_tmp = fx;                              \
	                                                        \
	    if (sat_tmp >= MLIB_S32_MAX)                        \
		sat_tmp = MLIB_S32_MAX;                         \
	    if (sat_tmp <= MLIB_S32_MIN)                        \
		sat_tmp = MLIB_S32_MIN;                         \
	    dp[j * stride_##dp + i * channels + ch] =           \
		(mlib_s32)sat_tmp >> MLIB_SHIFT;                \
	}

#define	RES_U16(dp)                                             \
	{                                                       \
	    mlib_d64 sat_tmp = fx - (mlib_d64)0x7FFF8000;       \
	                                                        \
	    if (sat_tmp >= MLIB_S32_MAX)                        \
		sat_tmp = MLIB_S32_MAX;                         \
	    if (sat_tmp <= MLIB_S32_MIN)                        \
		sat_tmp = MLIB_S32_MIN;                         \
	    dp[j * stride_##dp + i * channels + ch] =           \
		((mlib_s32)sat_tmp >> MLIB_SHIFT) ^ 0x8000;     \
	}

#endif /* MLIB_USE_FTOI_CLAMPING */

/* *********************************************************** */

#define	MLIB_SRC_EXTEND_BC_1(dp, xx, yy, w, h, filter, RES, READ_SRC)    \
	{                                                                \
	    mlib_s32 x, y, cx, cy, r0, r1, r2, r3, c0, c1, c2, c3;       \
	    mlib_f32 *fPtr;                                              \
	    mlib_f32 fx, k0, k1, k2, k3, xf0, xf1, xf2, xf3, yf0, yf1,   \
		yf2, yf3;                                                \
	    y = yy;                                                      \
	    for (j = 0; j < h; j++) {                                    \
		x = xx;                                                  \
		cy = y >> MLIB_SHIFT;                                    \
		fPtr =                                                   \
		    (mlib_f32 *)((mlib_u8 *)filter +                     \
		    ((y >> MLIB_XY_SHIFT) & MLIB_XY_MASK));              \
		yf0 = fPtr[0];                                           \
		yf1 = fPtr[1];                                           \
		yf2 = fPtr[2];                                           \
		yf3 = fPtr[3];                                           \
		INT_BOUND(r0, cy - 1, sh);                               \
		INT_BOUND(r1, cy, sh);                                   \
		INT_BOUND(r2, cy + 1, sh);                               \
		INT_BOUND(r3, cy + 2, sh);                               \
		r0 *= src_stride;                                        \
		r1 *= src_stride;                                        \
		r2 *= src_stride;                                        \
		r3 *= src_stride;                                        \
		for (i = 0; i < w; i++) {                                \
		    cx = x >> MLIB_SHIFT;                                \
		    fPtr =                                               \
			(mlib_f32 *)((mlib_u8 *)filter +                 \
			((x >> MLIB_XY_SHIFT) & MLIB_XY_MASK));          \
		    xf0 = fPtr[0];                                       \
		    xf1 = fPtr[1];                                       \
		    xf2 = fPtr[2];                                       \
		    xf3 = fPtr[3];                                       \
		    INT_BOUND(c0, cx - 1, sw);                           \
		    INT_BOUND(c1, cx, sw);                               \
		    INT_BOUND(c2, cx + 1, sw);                           \
		    INT_BOUND(c3, cx + 2, sw);                           \
		    c0 = channels * c0 + ch;                             \
		    c1 = channels * c1 + ch;                             \
		    c2 = channels * c2 + ch;                             \
		    c3 = channels * c3 + ch;                             \
		    k0 = READ_SRC(r0 + c0) * xf0 + READ_SRC(r0 +         \
			c1) * xf1 + READ_SRC(r0 + c2) * xf2 +            \
			READ_SRC(r0 + c3) * xf3;                         \
		    k1 = READ_SRC(r1 + c0) * xf0 + READ_SRC(r1 +         \
			c1) * xf1 + READ_SRC(r1 + c2) * xf2 +            \
			READ_SRC(r1 + c3) * xf3;                         \
		    k2 = READ_SRC(r2 + c0) * xf0 + READ_SRC(r2 +         \
			c1) * xf1 + READ_SRC(r2 + c2) * xf2 +            \
			READ_SRC(r2 + c3) * xf3;                         \
		    k3 = READ_SRC(r3 + c0) * xf0 + READ_SRC(r3 +         \
			c1) * xf1 + READ_SRC(r3 + c2) * xf2 +            \
			READ_SRC(r3 + c3) * xf3;                         \
		    fx = k0 * yf0 + k1 * yf1 + k2 * yf2 + k3 * yf3;      \
		    RES(dp);                                             \
		    x += dx;                                             \
		}                                                        \
		y += dy;                                                 \
	    }                                                            \
	}

/* *********************************************************** */

#define	MLIB_SRC_EXTEND_BC(FLT, RES, READ_SRC)                           \
	MLIB_SRC_EXTEND_BC_1(dp0, x0, y0, w1, h1, FLT, RES, READ_SRC);   \
	MLIB_SRC_EXTEND_BC_1(dp1, x0, y1, w2, h2, FLT, RES, READ_SRC);   \
	MLIB_SRC_EXTEND_BC_1(dp2, x1, y1, w3, h2, FLT, RES, READ_SRC);   \
	MLIB_SRC_EXTEND_BC_1(dp3, x0, y2, w1, h3, FLT, RES, READ_SRC)

/* *********************************************************** */

#define	MLIB_XY_SHIFT	4
#define	MLIB_XY_MASK	(((1 << 8) - 1) << 4)

/* *********************************************************** */

void
mlib_ImageZoomExtend_U8_Bicubic(
    mlib_work_image *param)
{
	VARIABLE_EDGE(mlib_u8);
	VARIABLE_SRC_EXTEND(mlib_u8);

	for (ch = 0; ch < channels; ch++) {
		MLIB_SRC_EXTEND_BC(mlib_filters_u8f_bc, RES_U8, READ_SRC_U8);
	}
}

void
mlib_ImageZoomExtend_U8_Bicubic2(
    mlib_work_image *param)
{
	VARIABLE_EDGE(mlib_u8);
	VARIABLE_SRC_EXTEND(mlib_u8);

	for (ch = 0; ch < channels; ch++) {
		MLIB_SRC_EXTEND_BC(mlib_filters_u8f_bc2, RES_U8, READ_SRC_U8);
	}
}

/* *********************************************************** */

void
mlib_ImageZoomExtend_U8_Bicubic_Blend(
    mlib_work_image *param,
    mlib_s32 alp_ind)
{
	VARIABLE_EDGE(mlib_u8);
	VARIABLE_SRC_EXTEND(mlib_u8);

	for (ch = 0; ch < channels; ch++) {
		if (!alp_ind) {
			MLIB_SRC_EXTEND_BC(mlib_filters_u8f_bc, RES_U8,
			    READ_SRC_U8);
		} else {
			MLIB_SRC_EXTEND_BC(mlib_filters_u8f_bc, RES_U8,
			    READ_SRC_BLEND);
		}
		alp_ind--;
	}
}

void
mlib_ImageZoomExtend_U8_Bicubic2_Blend(
    mlib_work_image *param,
    mlib_s32 alp_ind)
{
	VARIABLE_EDGE(mlib_u8);
	VARIABLE_SRC_EXTEND(mlib_u8);

	for (ch = 0; ch < channels; ch++) {
		if (!alp_ind) {
			MLIB_SRC_EXTEND_BC(mlib_filters_u8f_bc2, RES_U8,
			    READ_SRC_U8);
		} else {
			MLIB_SRC_EXTEND_BC(mlib_filters_u8f_bc2, RES_U8,
			    READ_SRC_BLEND);
		}
		alp_ind--;
	}
}

/* *********************************************************** */

#undef  READ_SRC
#define	READ_SRC(ind)	sp[ind]

/* *********************************************************** */

#undef  MLIB_XY_SHIFT
#undef  MLIB_XY_MASK

/* *********************************************************** */

#define	MLIB_XY_SHIFT	3
#define	MLIB_XY_MASK	(((1 << 9) - 1) << 4)

/* *********************************************************** */

void
mlib_ImageZoomExtend_S16_Bicubic(
    mlib_work_image *param)
{
	VARIABLE_EDGE(mlib_s16);
	VARIABLE_SRC_EXTEND(mlib_s16);

	for (ch = 0; ch < channels; ch++) {
		MLIB_SRC_EXTEND_BC(mlib_filters_s16f_bc, RES_S16, READ_SRC);
	}
}

/* *********************************************************** */

void
mlib_ImageZoomExtend_S16_Bicubic2(
    mlib_work_image *param)
{
	VARIABLE_EDGE(mlib_s16);
	VARIABLE_SRC_EXTEND(mlib_s16);

	for (ch = 0; ch < channels; ch++) {
		MLIB_SRC_EXTEND_BC(mlib_filters_s16f_bc2, RES_S16, READ_SRC);
	}
}

/* *********************************************************** */

void
mlib_ImageZoomExtend_U16_Bicubic(
    mlib_work_image *param)
{
	VARIABLE_EDGE(mlib_u16);
	VARIABLE_SRC_EXTEND(mlib_u16);

	for (ch = 0; ch < channels; ch++) {
		MLIB_SRC_EXTEND_BC(mlib_filters_s16f_bc, RES_U16, READ_SRC);
	}
}

/* *********************************************************** */

void
mlib_ImageZoomExtend_U16_Bicubic2(
    mlib_work_image *param)
{
	VARIABLE_EDGE(mlib_u16);
	VARIABLE_SRC_EXTEND(mlib_u16);

	for (ch = 0; ch < channels; ch++) {
		MLIB_SRC_EXTEND_BC(mlib_filters_s16f_bc2, RES_U16, READ_SRC);
	}
}

/* *********************************************************** */

#define	s0(x)	(-0.5 * (x) * (x) * (x) + (x) * (x) - 0.5 * (x))
#define	s1(x)	(1.5 * (x) * (x) * (x) - 2.5 * (x) * (x) + 1.0)
#define	s2(x)	(-1.5 * (x) * (x) * (x) + 2.0 * (x) * (x) + 0.5 * (x))
#define	s3(x)	(0.5 * (x) * (x) * (x) - 0.5 * (x) * (x))

/* *********************************************************** */

#ifdef MLIB_USE_FTOI_CLAMPING

/* *********************************************************** */

#define	RES_S32(dp)	dp[j * stride_##dp + i * channels + ch] = fx

#else /* MLIB_USE_FTOI_CLAMPING */

/* *********************************************************** */

#define	RES_S32(dp)                                              \
	if (fx >= MLIB_S32_MAX)                                  \
	    fx = MLIB_S32_MAX;                                   \
	if (fx <= MLIB_S32_MIN)                                  \
	    fx = MLIB_S32_MIN;                                   \
	dp[j * stride_##dp + i * channels + ch] = (mlib_s32)fx

#endif /* MLIB_USE_FTOI_CLAMPING */

/* *********************************************************** */

#define	RES_STORE_F32(dp)                                                  \
	dp[j * stride_##dp + i * channels + ch] = (mlib_f32)fx

#define	RES_STORE(dp)	dp[j * stride_##dp + i * channels + ch] = fx

/* *********************************************************** */

#define	MLIB_SRC_EXTEND_BC_S32(dp, xx, yy, w, h, RES)                      \
	{                                                                  \
	    mlib_s32 x, y, cx, cy, r0, r1, r2, r3, c0, c1, c2,             \
		c3;                                                        \
	    mlib_d64 fx, ddy, ddx, k0, k1, k2, k3, xf0, xf1, xf2, xf3,     \
		yf0, yf1, yf2, yf3;                                        \
	    y = yy;                                                        \
	    for (j = 0; j < h; j++) {                                      \
		x = xx;                                                    \
		cy = y >> MLIB_SHIFT;                                      \
		ddy = (y & MLIB_MASK) * MLIB_SCALE;                        \
		yf0 = s0(ddy);                                             \
		yf1 = s1(ddy);                                             \
		yf2 = s2(ddy);                                             \
		yf3 = s3(ddy);                                             \
		INT_BOUND(r0, cy - 1, sh);                                 \
		INT_BOUND(r1, cy, sh);                                     \
		INT_BOUND(r2, cy + 1, sh);                                 \
		INT_BOUND(r3, cy + 2, sh);                                 \
		r0 *= src_stride;                                          \
		r1 *= src_stride;                                          \
		r2 *= src_stride;                                          \
		r3 *= src_stride;                                          \
		for (i = 0; i < w; i++) {                                  \
		    cx = x >> MLIB_SHIFT;                                  \
		    ddx = (x & MLIB_MASK) * MLIB_SCALE;                    \
		    xf0 = s0(ddx);                                         \
		    xf1 = s1(ddx);                                         \
		    xf2 = s2(ddx);                                         \
		    xf3 = s3(ddx);                                         \
		    INT_BOUND(c0, cx - 1, sw);                             \
		    INT_BOUND(c1, cx, sw);                                 \
		    INT_BOUND(c2, cx + 1, sw);                             \
		    INT_BOUND(c3, cx + 2, sw);                             \
		    c0 = channels * c0 + ch;                               \
		    c1 = channels * c1 + ch;                               \
		    c2 = channels * c2 + ch;                               \
		    c3 = channels * c3 + ch;                               \
		    k0 = sp[r0 + c0] * xf0 + sp[r0 + c1] * xf1 + sp[r0 +   \
			c2] * xf2 + sp[r0 + c3] * xf3;                     \
		    k1 = sp[r1 + c0] * xf0 + sp[r1 + c1] * xf1 + sp[r1 +   \
			c2] * xf2 + sp[r1 + c3] * xf3;                     \
		    k2 = sp[r2 + c0] * xf0 + sp[r2 + c1] * xf1 + sp[r2 +   \
			c2] * xf2 + sp[r2 + c3] * xf3;                     \
		    k3 = sp[r3 + c0] * xf0 + sp[r3 + c1] * xf1 + sp[r3 +   \
			c2] * xf2 + sp[r3 + c3] * xf3;                     \
		    fx = k0 * yf0 + k1 * yf1 + k2 * yf2 + k3 * yf3;        \
		    RES(dp);                                               \
		    x += dx;                                               \
		}                                                          \
		y += dy;                                                   \
	    }                                                              \
	}

/* *********************************************************** */

void
mlib_ImageZoomExtend_S32_Bicubic(
    mlib_work_image *param)
{
	VARIABLE_EDGE(mlib_s32);
	VARIABLE_SRC_EXTEND(mlib_s32);

	for (ch = 0; ch < channels; ch++) {
		MLIB_SRC_EXTEND_BC_S32(dp0, x0, y0, w1, h1, RES_S32);
		MLIB_SRC_EXTEND_BC_S32(dp1, x0, y1, w2, h2, RES_S32);
		MLIB_SRC_EXTEND_BC_S32(dp2, x1, y1, w3, h2, RES_S32);
		MLIB_SRC_EXTEND_BC_S32(dp3, x0, y2, w1, h3, RES_S32);
	}
}

/* *********************************************************** */

void
mlib_ImageZoomExtend_F32_Bicubic(
    mlib_work_image *param)
{
	VARIABLE_EDGE(mlib_f32);
	VARIABLE_SRC_EXTEND(mlib_f32);

	for (ch = 0; ch < channels; ch++) {
		MLIB_SRC_EXTEND_BC_S32(dp0, x0, y0, w1, h1, RES_STORE_F32);
		MLIB_SRC_EXTEND_BC_S32(dp1, x0, y1, w2, h2, RES_STORE_F32);
		MLIB_SRC_EXTEND_BC_S32(dp2, x1, y1, w3, h2, RES_STORE_F32);
		MLIB_SRC_EXTEND_BC_S32(dp3, x0, y2, w1, h3, RES_STORE_F32);
	}
}

/* *********************************************************** */

void
mlib_ImageZoomExtend_D64_Bicubic(
    mlib_work_image *param)
{
	VARIABLE_EDGE(mlib_d64);
	VARIABLE_SRC_EXTEND(mlib_d64);

	for (ch = 0; ch < channels; ch++) {
		MLIB_SRC_EXTEND_BC_S32(dp0, x0, y0, w1, h1, RES_STORE);
		MLIB_SRC_EXTEND_BC_S32(dp1, x0, y1, w2, h2, RES_STORE);
		MLIB_SRC_EXTEND_BC_S32(dp2, x1, y1, w3, h2, RES_STORE);
		MLIB_SRC_EXTEND_BC_S32(dp3, x0, y2, w1, h3, RES_STORE);
	}
}

/* *********************************************************** */

#undef  s0
#undef  s1
#undef  s2
#undef  s3

/* *********************************************************** */

#define	s0(x)	(- (x) * (x) * (x) + 2.0 * (x) * (x) - (x))
#define	s1(x)	((x) * (x) * (x) - 2.0 * (x) * (x) + 1.0)
#define	s2(x)	(- (x) * (x) * (x) + (x) * (x) + (x))
#define	s3(x)	((x) * (x) * (x) - (x) * (x))

/* *********************************************************** */

void
mlib_ImageZoomExtend_S32_Bicubic2(
    mlib_work_image *param)
{
	VARIABLE_EDGE(mlib_s32);
	VARIABLE_SRC_EXTEND(mlib_s32);

	for (ch = 0; ch < channels; ch++) {
		MLIB_SRC_EXTEND_BC_S32(dp0, x0, y0, w1, h1, RES_S32);
		MLIB_SRC_EXTEND_BC_S32(dp1, x0, y1, w2, h2, RES_S32);
		MLIB_SRC_EXTEND_BC_S32(dp2, x1, y1, w3, h2, RES_S32);
		MLIB_SRC_EXTEND_BC_S32(dp3, x0, y2, w1, h3, RES_S32);
	}
}

/* *********************************************************** */

void
mlib_ImageZoomExtend_F32_Bicubic2(
    mlib_work_image *param)
{
	VARIABLE_EDGE(mlib_f32);
	VARIABLE_SRC_EXTEND(mlib_f32);

	for (ch = 0; ch < channels; ch++) {
		MLIB_SRC_EXTEND_BC_S32(dp0, x0, y0, w1, h1, RES_STORE_F32);
		MLIB_SRC_EXTEND_BC_S32(dp1, x0, y1, w2, h2, RES_STORE_F32);
		MLIB_SRC_EXTEND_BC_S32(dp2, x1, y1, w3, h2, RES_STORE_F32);
		MLIB_SRC_EXTEND_BC_S32(dp3, x0, y2, w1, h3, RES_STORE_F32);
	}
}

/* *********************************************************** */

void
mlib_ImageZoomExtend_D64_Bicubic2(
    mlib_work_image *param)
{
	VARIABLE_EDGE(mlib_d64);
	VARIABLE_SRC_EXTEND(mlib_d64);

	for (ch = 0; ch < channels; ch++) {
		MLIB_SRC_EXTEND_BC_S32(dp0, x0, y0, w1, h1, RES_STORE);
		MLIB_SRC_EXTEND_BC_S32(dp1, x0, y1, w2, h2, RES_STORE);
		MLIB_SRC_EXTEND_BC_S32(dp2, x1, y1, w3, h2, RES_STORE);
		MLIB_SRC_EXTEND_BC_S32(dp3, x0, y2, w1, h3, RES_STORE);
	}
}

#endif /* _NO_LONGLONG */

/* *********************************************************** */
