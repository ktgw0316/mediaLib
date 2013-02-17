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

#pragma ident	"@(#)mlib_c_ImageZoomBlend_NN.c	9.3	07/10/09 SMI"

/*
 * Local functions for
 *	mlib_ImageZoomBlend()
 *	mlib_ImageZoomTranslateBlend()
 */

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
	dst = (mlib_s32)(dst + (I2D(src) - I2D(dst)) * rw)

#define	BLEND2z(dst, src)                                       \
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
