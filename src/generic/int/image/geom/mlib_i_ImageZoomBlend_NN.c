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

#pragma ident	"@(#)mlib_i_ImageZoomBlend_NN.c	9.2	07/11/05"

/*
 * FUNCTION
 *      mlib_ImageZoom - image scaling with edge condition
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageZoom(mlib_image       *dst,
 *                                 const mlib_image *src,
 *                                 mlib_f32         zoomx,
 *                                 mlib_f32         zoomy,
 *                                 mlib_filter      filter,
 *                                 mlib_edge        edge)
 *
 * ARGUMENTS
 *      dst       Pointer to destination image
 *      src       Pointer to source image
 *      zoomx     X zoom factor.
 *      zoomy     Y zoom factor.
 *      filter    Type of resampling filter.
 *      edge      Type of edge condition.
 *
 * DESCRIPTION
 *  The center of the source image is mapped to the center of the
 *  destination image.
 *
 *  The upper-left corner pixel of an image is located at (0.5, 0.5).
 *
 *  The resampling filter can be one of the following:
 *    MLIB_NEAREST
 *    MLIB_BILINEAR
 *    MLIB_BICUBIC
 *    MLIB_BICUBIC2
 *
 *  The edge condition can be one of the following:
 *    MLIB_EDGE_DST_NO_WRITE  (default)
 *    MLIB_EDGE_DST_FILL_ZERO
 *    MLIB_EDGE_OP_NEAREST
 *    MLIB_EDGE_SRC_EXTEND
 *    MLIB_EDGE_SRC_PADDED
 */

#ifndef	_NO_LONGLONG

#include <mlib_image.h>

/* *********************************************************** */

#define	_MLIB_C_VER_

#include <mlib_ImageZoom.h>
#include <mlib_GeomBasicIntOp.h>

/* *********************************************************** */

#define	VARIABLE(FORMAT, FL)                                         \
	mlib_s32 j,                                                  \
		dx = GetElemStruct(DX),                              \
		dy = GetElemStruct(DY),                              \
		x = GetElemSubStruct(current, srcX),                 \
		y = GetElemSubStruct(current, srcY),                 \
		src_stride = GetElemStruct(src_stride),              \
		dst_stride = GetElemStruct(dst_stride),              \
		width  = GetElemSubStruct(current, width),           \
		height = GetElemSubStruct(current, height);          \
	ALP_IND_##FL

#define	ALP_IND_YES	mlib_u8  *tsp;                               \
			mlib_s32 alp_ind = param -> alp_ind

#define	ALP_IND_NO	mlib_u8  *tsp

/* *********************************************************** */

#define	F_ONE	((mlib_d64)1)
#define	I2D(x)	mlib_U82D64[x]

/* *********************************************************** */

#define	BLEND(dst, src)                                         \
{                                                               \
	mlib_s64 out, in;                                       \
	in = ((mlib_s32)(alp * ((mlib_s32)(src) -               \
		(mlib_s32)(dst))));                             \
	DIV255(out, in);                                        \
	dst = dst + out;                                        \
}

/* *********************************************************** */

#define	BLEND_ALPHA0(dst, src)                                  \
{                                                               \
	mlib_s64 out, in;                                       \
	in = ((mlib_s32)(alp * ((mlib_s32)(src) -               \
		(mlib_s32)(dst))));                             \
	DIV65025(out, in);                                      \
	dst = dst + out;                                        \
}

/* *********************************************************** */

#define	BLEND_A0(dst, src)                                      \
{                                                               \
	mlib_s64 out, in;                                       \
	in = alp*(mlib_s32)(src);                               \
	DIV255(out, in);                                        \
	dst = (mlib_s32)(out);                                  \
}

/* *********************************************************** */

#define	BLEND2(dst, src)                                        \
	dst = dst + (mlib_s32)(((mlib_s32)(src) -               \
		(mlib_s32)(dst)) * w0 / w)

#define	BLEND2z(dst, src)                                        \
	dst = (mlib_s32)((w0 * (mlib_s32)(src) +                 \
		w1 * (mlib_s32)(dst)) / w)

/* *********************************************************** */

#define	BLEND2_FLOAT(dst, src)                                   \
	dst = dst + (mlib_s32)((I2D(src) - I2D(dst)) * rw)

#define	BLEND2z_FLOAT(dst, src)                                  \
	dst = (mlib_s32)((w0 * I2D(src) + w1 * I2D(dst)) * rw)

/* *********************************************************** */


mlib_status
mlib_ImageZoom_U8_33_Nearest(
    mlib_work_image * param,
    mlib_u8 *sp,
    mlib_u8 *dp)
{
	VARIABLE(mlib_u8, NO);
	mlib_s32 i;
	mlib_u8 *tdp;
	mlib_s32 tmp0, tmp1, tmp2;
	mlib_s32 cx, y0 = -1;
	mlib_s32 alp = (param->alpha);

	tsp = sp;
	y = GetElemSubStruct(current, srcY) & MLIB_MASK;

	for (j = 0; j < height; j++) {
		tdp = dp;
		x = GetElemSubStruct(current, srcX) & MLIB_MASK;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < width; i++) {
			cx = (x >> MLIB_SHIFT) * 3;
			x += dx;

			tmp0 = tsp[cx];
			tmp1 = tsp[cx + 1];
			tmp2 = tsp[cx + 2];

			BLEND(tdp[0], tmp0);
			BLEND(tdp[1], tmp1);
			BLEND(tdp[2], tmp2);
			tdp += 3;
		}

		y0 = y;
		y += dy;
		dp = (mlib_u8 *)dp + dst_stride;
		tsp = (mlib_u8 *)sp + ((y >> MLIB_SHIFT) * src_stride);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_ImageZoom_U8_43_Nearest(
    mlib_work_image * param,
    mlib_u8 *sp,
    mlib_u8 *dp)
{
	VARIABLE(mlib_u8, YES);
	mlib_s32 i;
	mlib_u8 *tdp;
	mlib_s32 tmp0, tmp1, tmp2;
	mlib_s32 cx, y0 = -1;
	mlib_s32 alp, alp0 = (param->alpha);

	tsp = sp;
	y = GetElemSubStruct(current, srcY) & MLIB_MASK;

	for (j = 0; j < height; j++) {
		tdp = dp;
		x = GetElemSubStruct(current, srcX) & MLIB_MASK;

		if (alp0 < 0) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < width; i++) {
				cx = (x >> MLIB_SHIFT) * 4;
				x += dx;

				tmp0 = tsp[cx];
				tmp1 = tsp[cx + 1];
				tmp2 = tsp[cx + 2];

				alp = tsp[cx + alp_ind];

				BLEND_A0(tdp[0], tmp0);
				BLEND_A0(tdp[1], tmp1);
				BLEND_A0(tdp[2], tmp2);
				tdp += 3;
			}
		} else {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < width; i++) {
				cx = (x >> MLIB_SHIFT) * 4;
				x += dx;

				tmp0 = tsp[cx];
				tmp1 = tsp[cx + 1];
				tmp2 = tsp[cx + 2];

				alp = alp0 * tsp[cx + alp_ind];

				BLEND_ALPHA0(tdp[0], tmp0);
				BLEND_ALPHA0(tdp[1], tmp1);
				BLEND_ALPHA0(tdp[2], tmp2);
				tdp += 3;
			}
		}

		y0 = y;
		y += dy;
		dp = (mlib_u8 *)dp + dst_stride;
		tsp = (mlib_u8 *)sp + ((y >> MLIB_SHIFT) * src_stride);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_ImageZoom_U8_34_Nearest(
    mlib_work_image * param,
    mlib_u8 *sp,
    mlib_u8 *dp)
{
	VARIABLE(mlib_u8, YES);
	mlib_s32 i;
	mlib_u8 *tdp;
	mlib_s32 cx, y0 = -1;
	mlib_blend blend = param->blend;
	mlib_s32 w0 = (param->alpha);
	mlib_s32 w1s = 255 - w0;
	mlib_s32 w1, w;

	w0 = MUL255(w0);

	tsp = sp;
	y = GetElemSubStruct(current, srcY) & MLIB_MASK;

	for (j = 0; j < height; j++) {
		tdp = dp;
		x = GetElemSubStruct(current, srcX) & MLIB_MASK;

		if (blend == MLIB_BLEND_GTK_SRC) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < width; i++) {
				cx = (x >> MLIB_SHIFT) * 3;
				x += dx;

				tdp[0] = tsp[cx];
				tdp[1] = tsp[cx + 1];
				tdp[2] = tsp[cx + 2];
				tdp[alp_ind] = 255;
				tdp += 4;
			}
		} else if (blend == MLIB_BLEND_GTK_SRC_OVER2) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < width; i++) {
				cx = (x >> MLIB_SHIFT) * 3;
				x += dx;

				w1 = w1s * tdp[alp_ind];
				w = w0 + w1;

				if (w < 255) {
					tdp[alp_ind] = 0;
				} else {
					BLEND2(tdp[0], tsp[cx]);
					BLEND2(tdp[1], tsp[cx + 1]);
					BLEND2(tdp[2], tsp[cx + 2]);
					mlib_s64 out;
					DIV255(out, w);
					tdp[alp_ind] = (mlib_u8)(out);
				}
				tdp += 4;
			}
		} else {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < width; i++) {
				cx = (x >> MLIB_SHIFT) * 3;
				x += dx;

				w1 = w1s * tdp[alp_ind];
				w = w0 + w1;

				if (w < 255) {
					tdp[0] = 0;
					tdp[1] = 0;
					tdp[2] = 0;
					tdp[alp_ind] = 0;
				} else {
					BLEND2z(tdp[0], tsp[cx]);
					BLEND2z(tdp[1], tsp[cx + 1]);
					BLEND2z(tdp[2], tsp[cx + 2]);
					mlib_s64 out;
					DIV255(out, w);
					tdp[alp_ind] = (mlib_u8)(out);
				}
				tdp += 4;
			}
		}

		y0 = y;
		y += dy;
		dp = (mlib_u8 *)dp + dst_stride;
		tsp = (mlib_u8 *)sp + ((y >> MLIB_SHIFT) * src_stride);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_ImageZoom_U8_44_Nearest(
    mlib_work_image * param,
    mlib_u8 *sp,
    mlib_u8 *dp)
{
	VARIABLE(mlib_u8, YES);
	mlib_s32 i;
	mlib_u8 *tdp;
	mlib_s32 cx, y0 = -1;
	mlib_blend blend = param->blend;
	mlib_s64 alp = (param->alpha);
	mlib_s64 w0, w1, w;

	tsp = sp;
	y = GetElemSubStruct(current, srcY) & MLIB_MASK;

	for (j = 0; j < height; j++) {
		tdp = dp;
		x = GetElemSubStruct(current, srcX) & MLIB_MASK;

		if (blend == MLIB_BLEND_GTK_SRC_OVER2) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < width; i++) {
				cx = (x >> MLIB_SHIFT) * 4;
				x += dx;

				w0 = alp * tsp[cx + alp_ind];
				w1 = (65025 - w0) * tdp[alp_ind];
				w0 = MUL255(w0);
				w = w0 + w1;

				if (w < 65025) {
					tdp[alp_ind] = 0;
				} else {
					BLEND2(tdp[0], tsp[cx]);
					BLEND2(tdp[1], tsp[cx + 1]);
					BLEND2(tdp[2], tsp[cx + 2]);
					mlib_s64 out;
					DIV65025(out, w);
					tdp[alp_ind] = (mlib_u8)(out);
				}
				tdp += 4;
			}
		} else {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < width; i++) {
				cx = (x >> MLIB_SHIFT) * 4;
				x += dx;

				w0 = alp * tsp[cx + alp_ind];
				w1 = (65025 - w0) * tdp[alp_ind];
				w0 = MUL255(w0);
				w = w0 + w1;

				if (w < 65025) {
					tdp[0] = 0;
					tdp[1] = 0;
					tdp[2] = 0;
					tdp[alp_ind] = 0;
				} else {
					BLEND2z(tdp[0], tsp[cx]);
					BLEND2z(tdp[1], tsp[cx + 1]);
					BLEND2z(tdp[2], tsp[cx + 2]);
					mlib_s64 out;
					DIV65025(out, w);
					tdp[alp_ind] = (mlib_u8)(out);
				}
				tdp += 4;
			}
		}

		y0 = y;
		y += dy;
		dp = (mlib_u8 *)dp + dst_stride;
		tsp = (mlib_u8 *)sp + ((y >> MLIB_SHIFT) * src_stride);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#else /* _NO_LONGLONG */


#include <mlib_image.h>

/* *********************************************************** */

#define	_MLIB_C_VER_

#include <mlib_ImageZoom.h>

/* *********************************************************** */

#define	VARIABLE(FORMAT, FL)                                         \
	mlib_s32 j,                                                  \
		dx = GetElemStruct(DX),                              \
		dy = GetElemStruct(DY),                              \
		x = GetElemSubStruct(current, srcX),                 \
		y = GetElemSubStruct(current, srcY),                 \
		src_stride = GetElemStruct(src_stride),              \
		dst_stride = GetElemStruct(dst_stride),              \
		width  = GetElemSubStruct(current, width),           \
		height = GetElemSubStruct(current, height);          \
	ALP_IND_##FL

#define	ALP_IND_YES	mlib_u8  *tsp;                               \
			mlib_s32 alp_ind = param -> alp_ind

#define	ALP_IND_NO	mlib_u8  *tsp

/* *********************************************************** */

#define	FP_TYPE	mlib_d64
#define	F_ONE	((FP_TYPE)1)
#define	I2D(x)	mlib_U82D64[x]

/* *********************************************************** */

#define	BLEND(dst, src)                                         \
	dst = dst + (mlib_s32)(alp * (I2D(src) - I2D(dst)))

/* *********************************************************** */

#define	BLEND_A0(dst, src)	dst = (mlib_s32)(alp*I2D(src))

/* *********************************************************** */

#define	BLEND2(dst, src)                                        \
	dst = dst + (mlib_s32)((I2D(src) - I2D(dst)) * rw)

#define	BLEND2z(dst, src)                                        \
	dst = (mlib_s32)((w0 * I2D(src) + w1 * I2D(dst)) * rw)

/* *********************************************************** */

mlib_status
mlib_ImageZoom_U8_33_Nearest(
    mlib_work_image * param,
    mlib_u8 *sp,
    mlib_u8 *dp)
{
	VARIABLE(mlib_u8, NO);
	mlib_s32 i;
	mlib_u8 *tdp;
	mlib_s32 tmp0, tmp1, tmp2;
	mlib_s32 cx, y0 = -1;
	FP_TYPE alp = (param->alpha) * (F_ONE / 255);

	tsp = sp;
	y = GetElemSubStruct(current, srcY) & MLIB_MASK;

	for (j = 0; j < height; j++) {
		tdp = dp;
		x = GetElemSubStruct(current, srcX) & MLIB_MASK;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < width; i++) {
			cx = (x >> MLIB_SHIFT) * 3;
			x += dx;

			tmp0 = tsp[cx];
			tmp1 = tsp[cx + 1];
			tmp2 = tsp[cx + 2];

			BLEND(tdp[0], tmp0);
			BLEND(tdp[1], tmp1);
			BLEND(tdp[2], tmp2);
			tdp += 3;
		}

		y0 = y;
		y += dy;
		dp = (mlib_u8 *)dp + dst_stride;
		tsp = (mlib_u8 *)sp + ((y >> MLIB_SHIFT) * src_stride);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_ImageZoom_U8_43_Nearest(
    mlib_work_image * param,
    mlib_u8 *sp,
    mlib_u8 *dp)
{
	VARIABLE(mlib_u8, YES);
	mlib_s32 i;
	mlib_u8 *tdp;
	mlib_s32 tmp0, tmp1, tmp2;
	mlib_s32 cx, y0 = -1;
	FP_TYPE alp, alp0 = (param->alpha) * (F_ONE / (255 * 255));

	tsp = sp;
	y = GetElemSubStruct(current, srcY) & MLIB_MASK;

	for (j = 0; j < height; j++) {
		tdp = dp;
		x = GetElemSubStruct(current, srcX) & MLIB_MASK;

		if (alp0 < 0) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < width; i++) {
				cx = (x >> MLIB_SHIFT) * 4;
				x += dx;

				tmp0 = tsp[cx];
				tmp1 = tsp[cx + 1];
				tmp2 = tsp[cx + 2];

				alp = tsp[cx + alp_ind] * (F_ONE / 255);

				BLEND_A0(tdp[0], tmp0);
				BLEND_A0(tdp[1], tmp1);
				BLEND_A0(tdp[2], tmp2);
				tdp += 3;
			}
		} else {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < width; i++) {
				cx = (x >> MLIB_SHIFT) * 4;
				x += dx;

				tmp0 = tsp[cx];
				tmp1 = tsp[cx + 1];
				tmp2 = tsp[cx + 2];

				alp = alp0 * tsp[cx + alp_ind];

				BLEND(tdp[0], tmp0);
				BLEND(tdp[1], tmp1);
				BLEND(tdp[2], tmp2);
				tdp += 3;
			}
		}

		y0 = y;
		y += dy;
		dp = (mlib_u8 *)dp + dst_stride;
		tsp = (mlib_u8 *)sp + ((y >> MLIB_SHIFT) * src_stride);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_ImageZoom_U8_34_Nearest(
    mlib_work_image * param,
    mlib_u8 *sp,
    mlib_u8 *dp)
{
	VARIABLE(mlib_u8, YES);
	mlib_s32 i;
	mlib_u8 *tdp;
	mlib_s32 cx, y0 = -1;
	mlib_blend blend = param->blend;
	FP_TYPE w0 = param->alpha;
	FP_TYPE w1s = F_ONE - w0 * (F_ONE / 255);
	FP_TYPE w1, w, rw;

	tsp = sp;
	y = GetElemSubStruct(current, srcY) & MLIB_MASK;

	for (j = 0; j < height; j++) {
		tdp = dp;
		x = GetElemSubStruct(current, srcX) & MLIB_MASK;

		if (blend == MLIB_BLEND_GTK_SRC) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < width; i++) {
				cx = (x >> MLIB_SHIFT) * 3;
				x += dx;

				tdp[0] = tsp[cx];
				tdp[1] = tsp[cx + 1];
				tdp[2] = tsp[cx + 2];
				tdp[alp_ind] = 255;
				tdp += 4;
			}
		} else if (blend == MLIB_BLEND_GTK_SRC_OVER2) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < width; i++) {
				cx = (x >> MLIB_SHIFT) * 3;
				x += dx;

				w1 = w1s * tdp[alp_ind];
				w = w0 + w1;
				FP_DIV(rw, w0, w);

				BLEND2(tdp[0], tsp[cx]);
				BLEND2(tdp[1], tsp[cx + 1]);
				BLEND2(tdp[2], tsp[cx + 2]);
				tdp[alp_ind] = (mlib_u8)w;
				tdp += 4;
			}
		} else {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < width; i++) {
				cx = (x >> MLIB_SHIFT) * 3;
				x += dx;

				w1 = w1s * tdp[alp_ind];
				w = w0 + w1;
				FP_INV(rw, w);

				BLEND2z(tdp[0], tsp[cx]);
				BLEND2z(tdp[1], tsp[cx + 1]);
				BLEND2z(tdp[2], tsp[cx + 2]);
				tdp[alp_ind] = (mlib_u8)w;
				tdp += 4;
			}
		}

		y0 = y;
		y += dy;
		dp = (mlib_u8 *)dp + dst_stride;
		tsp = (mlib_u8 *)sp + ((y >> MLIB_SHIFT) * src_stride);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_ImageZoom_U8_44_Nearest(
    mlib_work_image * param,
    mlib_u8 *sp,
    mlib_u8 *dp)
{
	VARIABLE(mlib_u8, YES);
	mlib_s32 i;
	mlib_u8 *tdp;
	mlib_s32 cx, y0 = -1;
	mlib_blend blend = param->blend;
	FP_TYPE alp = (param->alpha) * (F_ONE / 255);
	FP_TYPE w0, w1, w, rw;

	tsp = sp;
	y = GetElemSubStruct(current, srcY) & MLIB_MASK;

	for (j = 0; j < height; j++) {
		tdp = dp;
		x = GetElemSubStruct(current, srcX) & MLIB_MASK;

		if (blend == MLIB_BLEND_GTK_SRC_OVER2) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < width; i++) {
				cx = (x >> MLIB_SHIFT) * 4;
				x += dx;

				w0 = alp * tsp[cx + alp_ind];
				w1 = (F_ONE -
				    w0 * (F_ONE / 255)) * tdp[alp_ind];
				w = w0 + w1;
				FP_DIV(rw, w0, w);

				BLEND2(tdp[0], tsp[cx]);
				BLEND2(tdp[1], tsp[cx + 1]);
				BLEND2(tdp[2], tsp[cx + 2]);
				tdp[alp_ind] = (mlib_u8)w;
				tdp += 4;
			}
		} else {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < width; i++) {
				cx = (x >> MLIB_SHIFT) * 4;
				x += dx;

				w0 = alp * tsp[cx + alp_ind];
				w1 = (F_ONE -
				    w0 * (F_ONE / 255)) * tdp[alp_ind];
				w = w0 + w1;
				FP_INV(rw, w);

				BLEND2z(tdp[0], tsp[cx]);
				BLEND2z(tdp[1], tsp[cx + 1]);
				BLEND2z(tdp[2], tsp[cx + 2]);
				tdp[alp_ind] = (mlib_u8)w;
				tdp += 4;
			}
		}

		y0 = y;
		y += dy;
		dp = (mlib_u8 *)dp + dst_stride;
		tsp = (mlib_u8 *)sp + ((y >> MLIB_SHIFT) * src_stride);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#endif /* _NO_LONGLONG */
