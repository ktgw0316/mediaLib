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

#pragma ident	"@(#)mlib_v_ImageZoomBlend_NN_f.c	9.2	07/11/05 SMI"

/*
 * FUNCTION
 *      Internal functions for mlib_ImageZoomTranslateBlend().
 *
 */

#include <mlib_image.h>
#include <vis_proto.h>

/* *********************************************************** */

#define	_MLIB_VIS_VER_

#include <mlib_ImageZoom.h>

/* *********************************************************** */

#define	VARIABLE(FORMAT)                                             \
	mlib_s32 j,                                                  \
		dx = GetElemStruct(DX),                              \
		dy = GetElemStruct(DY),                              \
		x = GetElemSubStruct(current, srcX),                 \
		y = GetElemSubStruct(current, srcY),                 \
		src_stride = GetElemStruct(src_stride),              \
		dst_stride = GetElemStruct(dst_stride),              \
		width  = GetElemSubStruct(current, width),           \
		height = GetElemSubStruct(current, height);          \
	mlib_s32 alp_ind = param -> alp_ind;                         \
	mlib_u8  *tsp

/* *********************************************************** */

#define	FP_TYPE	mlib_d64
#define	F_ONE	((FP_TYPE)1)
#define	I2D(x)	mlib_U82D64[x]

/* *********************************************************** */

#define	BLEND2(dst, src)                                        \
	dst = dst + (mlib_s32)((I2D(src) - I2D(dst)) * rw)

#define	BLEND2z(dst, src)                                        \
	dst = (mlib_s32)((w0 * I2D(src) + w1 * I2D(dst)) * rw)

/* *********************************************************** */

mlib_status
mlib_ImageZoom_U8_34_Nearest(
    mlib_work_image *param,
    mlib_u8 *sp,
    mlib_u8 *dp)
{
	VARIABLE(mlib_u8);
	mlib_s32 i;
	mlib_u8 *tdp;
	mlib_s32 tmp0, tmp1, tmp2, t, dd;
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

				w1 = w1s * I2D(tdp[alp_ind]);
				w = w0 + w1;
				FP_DIV(rw, w0, w);

				BLEND2(tdp[0], tsp[cx]);
				BLEND2(tdp[1], tsp[cx + 1]);
				BLEND2(tdp[2], tsp[cx + 2]);
				tdp[alp_ind] = w;
				tdp += 4;
			}
		} else {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < width; i++) {
				cx = (x >> MLIB_SHIFT) * 3;
				x += dx;

				w1 = w1s * I2D(tdp[alp_ind]);
				w = w0 + w1;
				FP_INV(rw, w);

				BLEND2z(tdp[0], tsp[cx]);
				BLEND2z(tdp[1], tsp[cx + 1]);
				BLEND2z(tdp[2], tsp[cx + 2]);
				tdp[alp_ind] = w;
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
    mlib_work_image *param,
    mlib_u8 *sp,
    mlib_u8 *dp)
{
	VARIABLE(mlib_u8);
	mlib_s32 i;
	mlib_u8 *tdp;
	mlib_s32 tmp0, tmp1, tmp2, t, dd;
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

				w0 = alp * I2D(tsp[cx + alp_ind]);
				w1 = (F_ONE -
				    w0 * (F_ONE / 255)) * I2D(tdp[alp_ind]);
				w = w0 + w1;
				FP_DIV(rw, w0, w);

				BLEND2(tdp[0], tsp[cx]);
				BLEND2(tdp[1], tsp[cx + 1]);
				BLEND2(tdp[2], tsp[cx + 2]);
				tdp[alp_ind] = w;
				tdp += 4;
			}
		} else {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < width; i++) {
				cx = (x >> MLIB_SHIFT) * 4;
				x += dx;

				w0 = alp * I2D(tsp[cx + alp_ind]);
				w1 = (F_ONE -
				    w0 * (F_ONE / 255)) * I2D(tdp[alp_ind]);
				w = w0 + w1;
				FP_INV(rw, w);

				BLEND2z(tdp[0], tsp[cx]);
				BLEND2z(tdp[1], tsp[cx + 1]);
				BLEND2z(tdp[2], tsp[cx + 2]);
				tdp[alp_ind] = w;
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
