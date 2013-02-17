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

#pragma ident	"@(#)mlib_s_ImageZoom_BL_Fp.c	9.2	07/11/05 SMI"

/*
 * FUNCTION
 *   Internal functions for mlib_ImageZoom on MLIB_BILINEAR filter
 *   and F32 data type
 */

#include <mlib_s_ImageZoom.h>

/* *********************************************************** */

#define	DTYPE	mlib_f32
#define	FTYPE	mlib_f32

/* *********************************************************** */

#define	VARIABLEFP(FORMAT)                                   \
	FORMAT *sp = GetElemSubStruct(current, sp),          \
		*dp = GetElemSubStruct(current, dp);         \
	FTYPE dx = (FTYPE)param->rzoomx,                     \
		dy = (FTYPE)param->rzoomy,                   \
		yd  = (FTYPE)param->ystart,                  \
		xd;                                          \
	mlib_s32 i, j, x, y,                                 \
		src_stride = GetElemStruct(src_stride),      \
		dst_stride = GetElemStruct(dst_stride),      \
		width  = GetElemSubStruct(current, width),   \
		height = GetElemSubStruct(current, height)

/* *********************************************************** */

#define	MUL_S_F32xF32(S0, S1, D0)            \
	D0 = _mm_mul_ps(S0, S1)

/* *********************************************************** */

#undef	BUFF_SIZE
#define	BUFF_SIZE	2048

/* *********************************************************** */

mlib_status
mlib_ImageZoomBilinear_F32_1(
    mlib_work_image * param)
{
	VARIABLEFP(mlib_f32);
	__m128 buff_arr[BUFF_SIZE / 16], *buff0, *buff1;
	void *buff = buff_arr;
	DTYPE *tdp;
	mlib_s32 k, width4, buffsize, size, y_step = 2;
	__m128 ddx_start, ddx, ddy, rdy;
	__m128 x_one = _mm_set1_ps(1.0);

	width4 = width / 4;
	buffsize = (width + 3) / 4;

	size = 2 * buffsize * sizeof (__m128);
	if (size > BUFF_SIZE) {
		buff = __mlib_malloc(size);
		if (buff == NULL)
			return (MLIB_FAILURE);
	}

	buff0 = buff;
	buff1 = buff0 + buffsize;

/* calculate dx for all row */
	xd = (FTYPE) GetElemStruct(xstart);

	mlib_f32 dx0, dx1, dx2, dx3;
	dx0 = xd - (mlib_s32)xd;
	dx1 = xd + dx - (mlib_s32)(xd + dx);
	dx2 = xd + 2 * dx - (mlib_s32)(xd + 2 * dx);
	dx3 = xd + 3 * dx - (mlib_s32)(xd + 3 * dx);
	ddx_start = _mm_setr_ps(dx0, dx1, dx2, dx3);

	for (j = 0; j < height; j++) {
		tdp = dp;

		y = (mlib_s32)yd;

		if (y_step == 0) {
			ddy = _mm_set1_ps((yd - y));
			rdy = _mm_sub_ps(x_one, ddy);

			for (i = 0; i < width4; i++) {
				__m128 y0, y1, t0, t1, d0;
				y0 = buff0[i];
				y1 = buff1[i];
				MUL_S_F32xF32(rdy, y0, t0);
				MUL_S_F32xF32(ddy, y1, t1);
				d0 = _mm_add_ps(t0, t1);
				_mm_storeu_ps(tdp, d0);
				tdp += 4;
			}

			if (i * 4 < width) {
				__m128 y0, y1, t0, t1, d0, d1, d2;
				y0 = buff0[i];
				y1 = buff1[i];
				MUL_S_F32xF32(rdy, y0, t0);
				MUL_S_F32xF32(ddy, y1, t1);
				d0 = _mm_add_ps(t0, t1);

				mlib_s32 dt = width - i * 4;
				for (k = 0; k < dt; k++) {
					((DTYPE *)tdp)[k] = ((DTYPE *)&d0)[k];
				}
				tdp += dt;
			}

		} else if (y_step == 1) {
			__m128 *buffT;
			__m128 ddx, rdx;
			DTYPE *tsp = sp + y * src_stride + src_stride;

			ddx = ddx_start;
			ddy = _mm_set1_ps((yd - y));
			rdy = _mm_sub_ps(x_one, ddy);
			xd = GetElemStruct(xstart);

			buffT = buff0;
			buff0 = buff1;
			buff1 = buffT;

			for (i = 0; i < width4; i++) {
				DTYPE *sp0 = tsp + (mlib_s32)xd;
				DTYPE *sp1 =
				    tsp + (mlib_s32)(xd + dx);
				DTYPE *sp2 =
				    tsp + (mlib_s32)(xd + 2 * dx);
				DTYPE *sp3 =
				    tsp + (mlib_s32)(xd + 3 * dx);

				__m128 x0, x1, x2, x3, y0, y1;
				__m128 v0, v1, v2, v3;
				__m128 s0, s1, s2, s3;

				x0 = _mm_loadu_ps(sp0);
				x1 = _mm_loadu_ps(sp1);
				x2 = _mm_loadu_ps(sp2);
				x3 = _mm_loadu_ps(sp3);
				v0 = _mm_unpacklo_ps(x0, x2);
				v1 = _mm_unpacklo_ps(x1, x3);
				s2 = _mm_unpacklo_ps(v0, v1);
				s3 = _mm_unpackhi_ps(v0, v1);

				rdx = _mm_sub_ps(x_one, ddx);

				MUL_S_F32xF32(s2, rdx, v2);
				MUL_S_F32xF32(s3, ddx, v3);

				y1 = _mm_add_ps(v2, v3);
				buff1[i] = y1;
				y0 = buff0[i];

				MUL_S_F32xF32(rdy, y0, s0);
				MUL_S_F32xF32(ddy, y1, s1);

				v0 = _mm_add_ps(s0, s1);
				_mm_storeu_ps(tdp, v0);
				tdp += 4;
				xd += 4.0 * dx;
				mlib_f32 dx0, dx1, dx2, dx3;
				dx0 = xd - (mlib_s32)xd;
				dx1 = xd + dx - (mlib_s32)(xd + dx);
				dx2 = xd + 2 * dx - (mlib_s32)(xd + 2 * dx);
				dx3 = xd + 3 * dx - (mlib_s32)(xd + 3 * dx);
				ddx = _mm_setr_ps(dx0, dx1, dx2, dx3);
			}

			if (i * 4 < width) {
				DTYPE *sp0 = tsp + (mlib_s32)xd;
				DTYPE *sp1 =
				    tsp + (mlib_s32)(xd + dx);
				DTYPE *sp2 =
				    tsp + (mlib_s32)(xd + 2 * dx);
				DTYPE *sp3 =
				    tsp + (mlib_s32)(xd + 3 * dx);

				__m128 x0, x1, x2, x3, y0, y1;
				__m128 v0, v1, v2, v3;
				__m128 s0, s1, s2, s3;

				x0 = _mm_loadu_ps(sp0);
				x1 = _mm_loadu_ps(sp1);
				x2 = _mm_loadu_ps(sp2);
				x3 = _mm_loadu_ps(sp3);
				v0 = _mm_unpacklo_ps(x0, x2);
				v1 = _mm_unpacklo_ps(x1, x3);
				s2 = _mm_unpacklo_ps(v0, v1);
				s3 = _mm_unpackhi_ps(v0, v1);

				rdx = _mm_sub_ps(x_one, ddx);

				MUL_S_F32xF32(s2, rdx, v2);
				MUL_S_F32xF32(s3, ddx, v3);

				y1 = _mm_add_ps(v2, v3);
				buff1[i] = y1;
				y0 = buff0[i];

				MUL_S_F32xF32(rdy, y0, s0);
				MUL_S_F32xF32(ddy, y1, s1);

				v0 = _mm_add_ps(s0, s1);

				mlib_s32 dt = width - i * 4;
				for (k = 0; k < dt; k++) {
					((DTYPE *)tdp)[k] = ((DTYPE *)&v0)[k];
				}
				tdp += dt;
			}

		} else {
			__m128 ddx, rdx;
			DTYPE *tsp = sp + y * src_stride;

			ddx = ddx_start;
			ddy = _mm_set1_ps((yd - y));
			rdy = _mm_sub_ps(x_one, ddy);
			xd = GetElemStruct(xstart);

			for (i = 0; i < width4; i++) {
				DTYPE *sp0 = tsp + (mlib_s32)xd;
				DTYPE *sp1 =
				    tsp + (mlib_s32)(xd + dx);
				DTYPE *sp2 =
				    tsp + (mlib_s32)(xd + 2 * dx);
				DTYPE *sp3 =
				    tsp + (mlib_s32)(xd + 3 * dx);

				__m128 x0, x1, x2, x3, y0, y1;
				__m128 v0, v1, v2, v3;
				__m128 s0, s1, s2, s3;

				x0 = _mm_loadu_ps(sp0);
				x1 = _mm_loadu_ps(sp1);
				x2 = _mm_loadu_ps(sp2);
				x3 = _mm_loadu_ps(sp3);
				v0 = _mm_unpacklo_ps(x0, x2);
				v1 = _mm_unpacklo_ps(x1, x3);
				s0 = _mm_unpacklo_ps(v0, v1);
				s1 = _mm_unpackhi_ps(v0, v1);

				x0 = _mm_loadu_ps((sp0 + src_stride));
				x1 = _mm_loadu_ps((sp1 + src_stride));
				x2 = _mm_loadu_ps((sp2 + src_stride));
				x3 = _mm_loadu_ps((sp3 + src_stride));
				v0 = _mm_unpacklo_ps(x0, x2);
				v1 = _mm_unpacklo_ps(x1, x3);
				s2 = _mm_unpacklo_ps(v0, v1);
				s3 = _mm_unpackhi_ps(v0, v1);

				rdx = _mm_sub_ps(x_one, ddx);

				MUL_S_F32xF32(s0, rdx, v0);
				MUL_S_F32xF32(s1, ddx, v1);
				MUL_S_F32xF32(s2, rdx, v2);
				MUL_S_F32xF32(s3, ddx, v3);

				y0 = _mm_add_ps(v0, v1);
				y1 = _mm_add_ps(v2, v3);

				MUL_S_F32xF32(rdy, y0, s0);
				MUL_S_F32xF32(ddy, y1, s1);

				buff0[i] = y0;
				buff1[i] = y1;

				v2 = _mm_add_ps(s0, s1);
				_mm_storeu_ps(tdp, v2);
				tdp += 4;
				xd += 4.0 * dx;
				mlib_f32 dx0, dx1, dx2, dx3;
				dx0 = xd - (mlib_s32)xd;
				dx1 = xd + dx - (mlib_s32)(xd + dx);
				dx2 = xd + 2 * dx - (mlib_s32)(xd + 2 * dx);
				dx3 = xd + 3 * dx - (mlib_s32)(xd + 3 * dx);
				ddx = _mm_setr_ps(dx0, dx1, dx2, dx3);
			}

			if (i * 4 < width) {
				DTYPE *sp0 = tsp + (mlib_s32)xd;
				DTYPE *sp1 =
				    tsp + (mlib_s32)(xd + dx);
				DTYPE *sp2 =
				    tsp + (mlib_s32)(xd + 2 * dx);
				DTYPE *sp3 =
				    tsp + (mlib_s32)(xd + 3 * dx);

				__m128 x0, x1, x2, x3, y0, y1;
				__m128 v0, v1, v2, v3;
				__m128 s0, s1, s2, s3;

				x0 = _mm_loadu_ps(sp0);
				x1 = _mm_loadu_ps(sp1);
				x2 = _mm_loadu_ps(sp2);
				x3 = _mm_loadu_ps(sp3);
				v0 = _mm_unpacklo_ps(x0, x2);
				v1 = _mm_unpacklo_ps(x1, x3);
				s0 = _mm_unpacklo_ps(v0, v1);
				s1 = _mm_unpackhi_ps(v0, v1);

				x0 = _mm_loadu_ps((sp0 + src_stride));
				x1 = _mm_loadu_ps((sp1 + src_stride));
				x2 = _mm_loadu_ps((sp2 + src_stride));
				x3 = _mm_loadu_ps((sp3 + src_stride));
				v0 = _mm_unpacklo_ps(x0, x2);
				v1 = _mm_unpacklo_ps(x1, x3);
				s2 = _mm_unpacklo_ps(v0, v1);
				s3 = _mm_unpackhi_ps(v0, v1);

				rdx = _mm_sub_ps(x_one, ddx);

				MUL_S_F32xF32(s0, rdx, v0);
				MUL_S_F32xF32(s1, ddx, v1);
				MUL_S_F32xF32(s2, rdx, v2);
				MUL_S_F32xF32(s3, ddx, v3);

				y0 = _mm_add_ps(v0, v1);
				y1 = _mm_add_ps(v2, v3);

				MUL_S_F32xF32(rdy, y0, s0);
				MUL_S_F32xF32(ddy, y1, s1);

				buff0[i] = y0;
				buff1[i] = y1;

				v0 = _mm_add_ps(s0, s1);

				mlib_s32 dt = width - i * 4;
				for (k = 0; k < dt; k++) {
					((DTYPE *)tdp)[k] = ((DTYPE *)&v0)[k];
				}
				tdp += dt;
			}
		}

		y_step = (mlib_s32)(yd + dy) - y;
		dp += dst_stride;
		yd += dy;
	}

	if (size > BUFF_SIZE) {
		__mlib_free(buff);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#undef	BUFF_SIZE
#define	BUFF_SIZE	4096

mlib_status
mlib_ImageZoomBilinear_F32_2(
    mlib_work_image * param)
{
	VARIABLEFP(mlib_f32);
	__m128 buff_arr[BUFF_SIZE / 16], *buff0, *buff1;
	void *buff = buff_arr;
	DTYPE *tdp;
	mlib_s32 k, width4, buffsize, size, y_step = 2;
	__m128 ddx_start, ddx, ddy, rdy;
	__m128 x_one = _mm_set1_ps(1.0);

	width *= 2;
	width4 = width / 4;
	buffsize = (width + 3) / 4;

	size = 2 * buffsize * sizeof (__m128);
	if (size > BUFF_SIZE) {
		buff = __mlib_malloc(size);
		if (buff == NULL)
			return (MLIB_FAILURE);
	}

	buff0 = buff;
	buff1 = buff0 + buffsize;

/* calculate dx for all row */
	xd = (FTYPE) GetElemStruct(xstart);

	mlib_f32 dx0, dx1;
	dx0 = xd - (mlib_s32)xd;
	dx1 = xd + dx - (mlib_s32)(xd + dx);
	ddx_start = _mm_setr_ps(dx0, dx1, dx0, dx1);

	for (j = 0; j < height; j++) {
		tdp = dp;

		y = (mlib_s32)yd;

		if (y_step == 0) {
			ddy = _mm_set1_ps((yd - y));
			rdy = _mm_sub_ps(x_one, ddy);

			for (i = 0; i < width4; i++) {
				__m128 y0, y1, s0, s1, d0, d1;
				y0 = buff0[i];
				y1 = buff1[i];
				MUL_S_F32xF32(rdy, y0, s0);
				MUL_S_F32xF32(ddy, y1, s1);
				d0 = _mm_add_ps(s0, s1);
				/* 0xD8 = _MM_SHUFFLE(11, 01, 10, 00) */
				d1 = _mm_shuffle_ps(d0, d0, 0xD8);
				_mm_storeu_ps(tdp, d1);
				tdp += 4;
			}

			if (i * 4 < width) {
				__m128 y0, y1, s0, s1, d0, d1;
				y0 = buff0[i];
				y1 = buff1[i];
				MUL_S_F32xF32(rdy, y0, s0);
				MUL_S_F32xF32(ddy, y1, s1);
				d0 = _mm_add_ps(s0, s1);
				/* 0xD8 = _MM_SHUFFLE(11, 01, 10, 00) */
				d1 = _mm_shuffle_ps(d0, d0, 0xD8);
				mlib_s32 dt = width - i * 4;
				for (k = 0; k < dt; k++) {
					((DTYPE *)tdp)[k] = ((DTYPE *)&d1)[k];
				}
			}

		} else if (y_step == 1) {
			__m128 *buffT;
			__m128 ddx, rdx;
			DTYPE *tsp = sp + y * src_stride + src_stride;

			ddx = ddx_start;
			ddy = _mm_set1_ps((yd - y));
			rdy = _mm_sub_ps(x_one, ddy);
			xd = GetElemStruct(xstart);

			buffT = buff0;
			buff0 = buff1;
			buff1 = buffT;

			for (i = 0; i < width4; i++) {
				DTYPE *sp0 = tsp + 2 * (mlib_s32)xd;
				DTYPE *sp1 =
				    tsp + 2 * (mlib_s32)(xd + dx);

				__m128 x0, x1, y0, y1;
				__m128 v0, v1, s0, s1;

				x0 = _mm_loadu_ps(sp0);
				x1 = _mm_loadu_ps(sp1);
				s0 = _mm_unpacklo_ps(x0, x1);
				s1 = _mm_unpackhi_ps(x0, x1);

				rdx = _mm_sub_ps(x_one, ddx);

				MUL_S_F32xF32(s0, rdx, v0);
				MUL_S_F32xF32(s1, ddx, v1);

				y1 = _mm_add_ps(v0, v1);
				y0 = buff0[i];

				MUL_S_F32xF32(rdy, y0, s0);
				MUL_S_F32xF32(ddy, y1, s1);

				buff1[i] = y1;

				v0 = _mm_add_ps(s0, s1);
				/* 0xD8 = _MM_SHUFFLE(11, 01, 10, 00) */
				v1 = _mm_shuffle_ps(v0, v0, 0xD8);
				_mm_storeu_ps(tdp, v1);
				tdp += 4;
				xd += 2.0 * dx;
				mlib_f32 dx0, dx1;
				dx0 = xd - (mlib_s32)xd;
				dx1 = xd + dx - (mlib_s32)(xd + dx);
				ddx = _mm_setr_ps(dx0, dx1, dx0, dx1);
			}

			if (i * 4 < width) {
				DTYPE *sp0 = tsp + 2 * (mlib_s32)xd;

				__m128 x0, x1, y0, y1;
				__m128 v0, v1, s0, s1;
				__m128 x_zero = _mm_setzero_ps();

				x0 = _mm_loadu_ps(sp0);
				s0 = _mm_unpacklo_ps(x0, x_zero);
				s1 = _mm_unpackhi_ps(x0, x_zero);

				rdx = _mm_sub_ps(x_one, ddx);

				MUL_S_F32xF32(s0, rdx, v0);
				MUL_S_F32xF32(s1, ddx, v1);

				y1 = _mm_add_ps(v0, v1);
				y0 = buff0[i];

				MUL_S_F32xF32(rdy, y0, s0);
				MUL_S_F32xF32(ddy, y1, s1);

				buff1[i] = y1;

				v0 = _mm_add_ps(s0, s1);
				/* 0xD8 = _MM_SHUFFLE(11, 01, 10, 00) */
				v1 = _mm_shuffle_ps(v0, v0, 0xD8);
				mlib_s32 dt = width - i * 4;
				for (k = 0; k < dt; k++) {
					((DTYPE *)tdp)[k] = ((DTYPE *)&v1)[k];
				}
			}

		} else {
			__m128 ddx, rdx;
			DTYPE *tsp = sp + y * src_stride;

			ddx = ddx_start;
			ddy = _mm_set1_ps((yd - y));
			rdy = _mm_sub_ps(x_one, ddy);
			xd = GetElemStruct(xstart);

			for (i = 0; i < width4; i++) {
				DTYPE *sp0 = tsp + 2 * (mlib_s32)xd;
				DTYPE *sp1 =
				    tsp + 2 * (mlib_s32)(xd + dx);

				__m128 x0, x1, x2, x3, y0, y1;
				__m128 v0, v1, v2, v3;
				__m128 s0, s1, s2, s3;

				x0 = _mm_loadu_ps(sp0);
				x1 = _mm_loadu_ps(sp1);
				s0 = _mm_unpacklo_ps(x0, x1);
				s1 = _mm_unpackhi_ps(x0, x1);

				x2 = _mm_loadu_ps((sp0 + src_stride));
				x3 = _mm_loadu_ps((sp1 + src_stride));
				s2 = _mm_unpacklo_ps(x2, x3);
				s3 = _mm_unpackhi_ps(x2, x3);

				rdx = _mm_sub_ps(x_one, ddx);

				MUL_S_F32xF32(s0, rdx, v0);
				MUL_S_F32xF32(s1, ddx, v1);
				MUL_S_F32xF32(s2, rdx, v2);
				MUL_S_F32xF32(s3, ddx, v3);

				y0 = _mm_add_ps(v0, v1);
				y1 = _mm_add_ps(v2, v3);

				MUL_S_F32xF32(rdy, y0, s0);
				MUL_S_F32xF32(ddy, y1, s1);

				buff0[i] = y0;
				buff1[i] = y1;

				v2 = _mm_add_ps(s0, s1);
				/* 0xD8 = _MM_SHUFFLE(11, 01, 10, 00) */
				v3 = _mm_shuffle_ps(v2, v2, 0xD8);
				_mm_storeu_ps(tdp, v3);
				tdp += 4;
				xd += 2.0 * dx;
				mlib_f32 dx0, dx1;
				dx0 = xd - (mlib_s32)xd;
				dx1 = xd + dx - (mlib_s32)(xd + dx);
				ddx = _mm_setr_ps(dx0, dx1, dx0, dx1);
			}

			if (i * 4 < width) {
				DTYPE *sp0 = tsp + 2 * (mlib_s32)xd;

				__m128 x0, x2, y0, y1;
				__m128 v0, v1, v2, v3;
				__m128 s0, s1, s2, s3;
				__m128 x_zero = _mm_setzero_ps();

				x0 = _mm_loadu_ps(sp0);
				s0 = _mm_unpacklo_ps(x0, x_zero);
				s1 = _mm_unpackhi_ps(x0, x_zero);

				x2 = _mm_loadu_ps((sp0 + src_stride));
				s2 = _mm_unpacklo_ps(x2, x_zero);
				s3 = _mm_unpackhi_ps(x2, x_zero);

				rdx = _mm_sub_ps(x_one, ddx);

				MUL_S_F32xF32(s0, rdx, v0);
				MUL_S_F32xF32(s1, ddx, v1);
				MUL_S_F32xF32(s2, rdx, v2);
				MUL_S_F32xF32(s3, ddx, v3);

				y0 = _mm_add_ps(v0, v1);
				y1 = _mm_add_ps(v2, v3);

				MUL_S_F32xF32(rdy, y0, s0);
				MUL_S_F32xF32(ddy, y1, s1);

				buff0[i] = y0;
				buff1[i] = y1;

				v2 = _mm_add_ps(s0, s1);
				/* 0xD8 = _MM_SHUFFLE(11, 01, 10, 00) */
				v3 = _mm_shuffle_ps(v2, v2, 0xD8);
				mlib_s32 dt = width - i * 4;
				for (k = 0; k < dt; k++) {
					((DTYPE *)tdp)[k] = ((DTYPE *)&v3)[k];
				}
			}
		}

		y_step = (mlib_s32)(yd + dy) - y;
		dp += dst_stride;
		yd += dy;
	}

	if (size > BUFF_SIZE) {
		__mlib_free(buff);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#undef	BUFF_SIZE
#define	BUFF_SIZE	8192

mlib_status
mlib_ImageZoomBilinear_F32_3(
    mlib_work_image * param)
{
	VARIABLEFP(mlib_f32);
	__m128 buff_arr[BUFF_SIZE / 16], *buff0, *buff1;
	void *buff = buff_arr;
	DTYPE *tdp;
	mlib_s32 k, width3, buffsize, size, y_step = 2;
	__m128 ddx_start, ddx, ddy, rdy;
	__m128 x_one = _mm_set1_ps(1.0);

	width *= 3;
	width3 = width / 3 - 1;
	buffsize = (width + 2) / 3;

	size = 2 * buffsize * sizeof (__m128);
	if (size > BUFF_SIZE) {
		buff = __mlib_malloc(size);
		if (buff == NULL)
			return (MLIB_FAILURE);
	}

	buff0 = buff;
	buff1 = buff0 + buffsize;

/* calculate dx for all row */
	xd = (FTYPE) GetElemStruct(xstart);

	x = (mlib_s32)xd;
	ddx_start = _mm_set1_ps((xd - x));

	for (j = 0; j < height; j++) {
		tdp = dp;

		y = (mlib_s32)yd;

		if (y_step == 0) {
			ddy = _mm_set1_ps((yd - y));
			rdy = _mm_sub_ps(x_one, ddy);

			for (i = 0; i < width3; i++) {
				__m128 y0, y1, s0, s1, d0, d1;
				y0 = buff0[i];
				y1 = buff1[i];
				MUL_S_F32xF32(rdy, y0, s0);
				MUL_S_F32xF32(ddy, y1, s1);
				d0 = _mm_add_ps(s0, s1);
				_mm_storeu_ps(tdp, d0);
				tdp += 3;
			}

			if (i * 3 < width) {
				__m128 y0, y1, s0, s1, d0, d1;
				y0 = buff0[i];
				y1 = buff1[i];
				MUL_S_F32xF32(rdy, y0, s0);
				MUL_S_F32xF32(ddy, y1, s1);
				d0 = _mm_add_ps(s0, s1);
				((mlib_d64 *)tdp)[0] = ((mlib_d64 *)&d0)[0];
				((DTYPE *)tdp)[2] = ((DTYPE *)&d0)[2];
			}

		} else if (y_step == 1) {
			__m128 *buffT;
			__m128 ddx, rdx;
			DTYPE *tsp = sp + y * src_stride + src_stride;

			ddx = ddx_start;
			ddy = _mm_set1_ps((yd - y));
			rdy = _mm_sub_ps(x_one, ddy);
			xd = GetElemStruct(xstart);

			buffT = buff0;
			buff0 = buff1;
			buff1 = buffT;

			for (i = 0; i < width3; i++) {
				DTYPE *sp0 = tsp + 3 * (mlib_s32)xd;

				__m128 x0, x1, y0, y1;
				__m128 v0, v1, s0, s1;

				x0 = _mm_loadu_ps(sp0);
				x1 = _mm_loadu_ps(sp0 + 3);

				rdx = _mm_sub_ps(x_one, ddx);

				MUL_S_F32xF32(x0, rdx, v0);
				MUL_S_F32xF32(x1, ddx, v1);

				y1 = _mm_add_ps(v0, v1);
				y0 = buff0[i];

				MUL_S_F32xF32(rdy, y0, s0);
				MUL_S_F32xF32(ddy, y1, s1);

				buff1[i] = y1;

				v0 = _mm_add_ps(s0, s1);
				_mm_storeu_ps(tdp, v0);
				tdp += 3;
				xd += dx;
				ddx = _mm_set1_ps((xd - (mlib_s32)xd));
			}

			if (i * 3 < width) {
				DTYPE *sp0 = tsp + 3 * (mlib_s32)xd;

				__m128 x0, x1, y0, y1;
				__m128 v0, v1, s0, s1;

				x0 = _mm_loadu_ps(sp0);
				x1 = _mm_loadu_ps(sp0 + 3);

				rdx = _mm_sub_ps(x_one, ddx);

				MUL_S_F32xF32(x0, rdx, v0);
				MUL_S_F32xF32(x1, ddx, v1);

				y1 = _mm_add_ps(v0, v1);
				y0 = buff0[i];

				MUL_S_F32xF32(rdy, y0, s0);
				MUL_S_F32xF32(ddy, y1, s1);

				buff1[i] = y1;

				v0 = _mm_add_ps(s0, s1);
				((mlib_d64 *)tdp)[0] = ((mlib_d64 *)&v0)[0];
				((DTYPE *)tdp)[2] = ((DTYPE *)&v0)[2];
			}

		} else {
			__m128 ddx, rdx;
			DTYPE *tsp = sp + y * src_stride;

			ddx = ddx_start;
			ddy = _mm_set1_ps((yd - y));
			rdy = _mm_sub_ps(x_one, ddy);
			xd = GetElemStruct(xstart);

			for (i = 0; i < width3; i++) {
				DTYPE *sp0 = tsp + 3 * (mlib_s32)xd;

				__m128 x0, x1, x2, x3, y0, y1;
				__m128 v0, v1, v2, v3, s0, s1;

				x0 = _mm_loadu_ps(sp0);
				x1 = _mm_loadu_ps(sp0 + 3);
				x2 = _mm_loadu_ps((sp0 + src_stride));
				x3 = _mm_loadu_ps((sp0 + 3 + src_stride));

				rdx = _mm_sub_ps(x_one, ddx);

				MUL_S_F32xF32(x0, rdx, v0);
				MUL_S_F32xF32(x1, ddx, v1);
				MUL_S_F32xF32(x2, rdx, v2);
				MUL_S_F32xF32(x3, ddx, v3);

				y0 = _mm_add_ps(v0, v1);
				y1 = _mm_add_ps(v2, v3);

				MUL_S_F32xF32(rdy, y0, s0);
				MUL_S_F32xF32(ddy, y1, s1);

				buff0[i] = y0;
				buff1[i] = y1;

				v0 = _mm_add_ps(s0, s1);
				_mm_storeu_ps(tdp, v0);
				tdp += 3;
				xd += dx;
				ddx = _mm_set1_ps((xd - (mlib_s32)xd));
			}

			if (i * 3 < width) {
				DTYPE *sp0 = tsp + 3 * (mlib_s32)xd;

				__m128 x0, x1, x2, x3, y0, y1;
				__m128 v0, v1, v2, v3, s0, s1;

				x0 = _mm_loadu_ps(sp0);
				x1 = _mm_loadu_ps(sp0 + 3);
				x2 = _mm_loadu_ps((sp0 + src_stride));
				x3 = _mm_loadu_ps((sp0 + 3 + src_stride));

				rdx = _mm_sub_ps(x_one, ddx);

				MUL_S_F32xF32(x0, rdx, v0);
				MUL_S_F32xF32(x1, ddx, v1);
				MUL_S_F32xF32(x2, rdx, v2);
				MUL_S_F32xF32(x3, ddx, v3);

				y0 = _mm_add_ps(v0, v1);
				y1 = _mm_add_ps(v2, v3);

				MUL_S_F32xF32(rdy, y0, s0);
				MUL_S_F32xF32(ddy, y1, s1);

				buff0[i] = y0;
				buff1[i] = y1;

				v0 = _mm_add_ps(s0, s1);
				((mlib_d64 *)tdp)[0] = ((mlib_d64 *)&v0)[0];
				((DTYPE *)tdp)[2] = ((DTYPE *)&v0)[2];
			}
		}

		y_step = (mlib_s32)(yd + dy) - y;
		dp += dst_stride;
		yd += dy;
	}

	if (size > BUFF_SIZE) {
		__mlib_free(buff);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#undef	BUFF_SIZE
#define	BUFF_SIZE	8192

mlib_status
mlib_ImageZoomBilinear_F32_4(
    mlib_work_image * param)
{
	VARIABLEFP(mlib_f32);
	__m128 buff_arr[BUFF_SIZE / 16], *buff0, *buff1;
	void *buff = buff_arr;
	DTYPE *tdp;
	mlib_s32 k, width4, size, y_step = 2;
	__m128 ddx_start, ddx, ddy, rdy;
	__m128 x_one = _mm_set1_ps(1.0);

	width *= 4;
	width4 = (width + 3) / 4;

	size = 2 * width4 * sizeof (__m128);
	if (size > BUFF_SIZE) {
		buff = __mlib_malloc(size);
		if (buff == NULL)
			return (MLIB_FAILURE);
	}

	buff0 = buff;
	buff1 = buff0 + width4;

/* calculate dx for all row */
	xd = (FTYPE) GetElemStruct(xstart);

	x = (mlib_s32)xd;
	ddx_start = _mm_set1_ps((xd - x));

	for (j = 0; j < height; j++) {
		tdp = dp;

		y = (mlib_s32)yd;

		if (y_step == 0) {
			ddy = _mm_set1_ps((yd - y));
			rdy = _mm_sub_ps(x_one, ddy);

			for (i = 0; i < width4; i++) {
				__m128 y0, y1, s0, s1, d0, d1;
				y0 = buff0[i];
				y1 = buff1[i];
				MUL_S_F32xF32(rdy, y0, s0);
				MUL_S_F32xF32(ddy, y1, s1);
				d0 = _mm_add_ps(s0, s1);
				_mm_storeu_ps(tdp, d0);
				tdp += 4;
			}

		} else if (y_step == 1) {
			__m128 *buffT;
			__m128 ddx, rdx;
			DTYPE *tsp = sp + y * src_stride + src_stride;

			ddx = ddx_start;
			ddy = _mm_set1_ps((yd - y));
			rdy = _mm_sub_ps(x_one, ddy);
			xd = GetElemStruct(xstart);

			buffT = buff0;
			buff0 = buff1;
			buff1 = buffT;

			for (i = 0; i < width4; i++) {
				DTYPE *sp0 = tsp + 4 * (mlib_s32)xd;

				__m128 x0, x1, y0, y1;
				__m128 v0, v1, s0, s1;

				x0 = _mm_loadu_ps(sp0);
				x1 = _mm_loadu_ps(sp0 + 4);

				rdx = _mm_sub_ps(x_one, ddx);

				MUL_S_F32xF32(x0, rdx, v0);
				MUL_S_F32xF32(x1, ddx, v1);

				y1 = _mm_add_ps(v0, v1);
				y0 = buff0[i];

				MUL_S_F32xF32(rdy, y0, s0);
				MUL_S_F32xF32(ddy, y1, s1);

				buff1[i] = y1;

				v0 = _mm_add_ps(s0, s1);
				_mm_storeu_ps(tdp, v0);
				tdp += 4;
				xd += dx;
				ddx = _mm_set1_ps((xd - (mlib_s32)xd));
			}

		} else {
			__m128 ddx, rdx;
			DTYPE *tsp = sp + y * src_stride;

			ddx = ddx_start;
			ddy = _mm_set1_ps((yd - y));
			rdy = _mm_sub_ps(x_one, ddy);
			xd = GetElemStruct(xstart);

			for (i = 0; i < width4; i++) {
				DTYPE *sp0 = tsp + 4 * (mlib_s32)xd;

				__m128 x0, x1, x2, x3, y0, y1;
				__m128 v0, v1, v2, v3, s0, s1;

				x0 = _mm_loadu_ps(sp0);
				x1 = _mm_loadu_ps(sp0 + 4);
				x2 = _mm_loadu_ps((sp0 + src_stride));
				x3 = _mm_loadu_ps((sp0 + 4 + src_stride));

				rdx = _mm_sub_ps(x_one, ddx);

				MUL_S_F32xF32(x0, rdx, v0);
				MUL_S_F32xF32(x1, ddx, v1);
				MUL_S_F32xF32(x2, rdx, v2);
				MUL_S_F32xF32(x3, ddx, v3);

				y0 = _mm_add_ps(v0, v1);
				y1 = _mm_add_ps(v2, v3);

				MUL_S_F32xF32(rdy, y0, s0);
				MUL_S_F32xF32(ddy, y1, s1);

				buff0[i] = y0;
				buff1[i] = y1;

				v0 = _mm_add_ps(s0, s1);
				_mm_storeu_ps(tdp, v0);
				tdp += 4;
				xd += dx;
				ddx = _mm_set1_ps((xd - (mlib_s32)xd));
			}
		}

		y_step = (mlib_s32)(yd + dy) - y;
		dp += dst_stride;
		yd += dy;
	}

	if (size > BUFF_SIZE) {
		__mlib_free(buff);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_ImageZoomBilinear_F32_1s(
    mlib_work_image * param)
{
	VARIABLEFP(mlib_f32);
	DTYPE *tdp;
	mlib_s32 k, width4, y_step = 2;
	__m128 ddx_start, ddx, ddy, rdy;
	__m128 x_one = _mm_set1_ps(1.0);

	width4 = width / 4;

/* calculate dx for all row */
	xd = (FTYPE) GetElemStruct(xstart);

	for (i = 0; i < 4; i++) {
		x = (mlib_s32)xd;
		((DTYPE *)&ddx_start)[i] = xd - x;
		xd += dx;
	}

	for (j = 0; j < height; j++) {
		y = (mlib_s32)yd;

		tdp = dp;
		DTYPE *tsp = sp + y * src_stride;

		__m128 ddx, rdx;
		ddx = ddx_start;
		ddy = _mm_set1_ps((yd - y));
		rdy = _mm_sub_ps(x_one, ddy);
		xd = GetElemStruct(xstart);

		for (i = 0; i < width4; i++) {
			DTYPE *sp0 = tsp + (mlib_s32)xd;
			DTYPE *sp1 =
			    tsp + (mlib_s32)(xd + dx);
			DTYPE *sp2 =
			    tsp + (mlib_s32)(xd + 2 * dx);
			DTYPE *sp3 =
			    tsp + (mlib_s32)(xd + 3 * dx);

			__m128 x0, x1, x2, x3, y0, y1;
			__m128 v0, v1, v2, v3;
			__m128 s0, s1, s2, s3;

			x0 = _mm_loadu_ps(sp0);
			x1 = _mm_loadu_ps(sp1);
			x2 = _mm_loadu_ps(sp2);
			x3 = _mm_loadu_ps(sp3);
			v0 = _mm_unpacklo_ps(x0, x2);
			v1 = _mm_unpacklo_ps(x1, x3);
			s0 = _mm_unpacklo_ps(v0, v1);
			s1 = _mm_unpackhi_ps(v0, v1);

			x0 = _mm_loadu_ps((sp0 + src_stride));
			x1 = _mm_loadu_ps((sp1 + src_stride));
			x2 = _mm_loadu_ps((sp2 + src_stride));
			x3 = _mm_loadu_ps((sp3 + src_stride));
			v0 = _mm_unpacklo_ps(x0, x2);
			v1 = _mm_unpacklo_ps(x1, x3);
			s2 = _mm_unpacklo_ps(v0, v1);
			s3 = _mm_unpackhi_ps(v0, v1);

			rdx = _mm_sub_ps(x_one, ddx);

			MUL_S_F32xF32(s0, rdx, v0);
			MUL_S_F32xF32(s1, ddx, v1);
			MUL_S_F32xF32(s2, rdx, v2);
			MUL_S_F32xF32(s3, ddx, v3);

			y0 = _mm_add_ps(v0, v1);
			y1 = _mm_add_ps(v2, v3);

			MUL_S_F32xF32(rdy, y0, s0);
			MUL_S_F32xF32(ddy, y1, s1);

			v2 = _mm_add_ps(s0, s1);
			_mm_storeu_ps(tdp, v2);
			tdp += 4;
			xd += 4.0 * dx;

			mlib_f32 dx0, dx1, dx2, dx3;
			dx0 = xd - (mlib_s32)xd;
			dx1 = xd + dx - (mlib_s32)(xd + dx);
			dx2 = xd + 2 * dx - (mlib_s32)(xd + 2 * dx);
			dx3 = xd + 3 * dx - (mlib_s32)(xd + 3 * dx);
			ddx = _mm_setr_ps(dx0, dx1, dx2, dx3);
		}

		if (i * 4 < width) {
			DTYPE *sp0 = tsp + (mlib_s32)xd;
			DTYPE *sp1 =
			    tsp + (mlib_s32)(xd + dx);
			DTYPE *sp2 =
			    tsp + (mlib_s32)(xd + 2 * dx);
			DTYPE *sp3 =
			    tsp + (mlib_s32)(xd + 3 * dx);

			__m128 x0, x1, x2, x3, y0, y1;
			__m128 v0, v1, v2, v3;
			__m128 s0, s1, s2, s3;

			x0 = _mm_loadu_ps(sp0);
			x1 = _mm_loadu_ps(sp1);
			x2 = _mm_loadu_ps(sp2);
			x3 = _mm_loadu_ps(sp3);
			v0 = _mm_unpacklo_ps(x0, x2);
			v1 = _mm_unpacklo_ps(x1, x3);
			s0 = _mm_unpacklo_ps(v0, v1);
			s1 = _mm_unpackhi_ps(v0, v1);

			x0 = _mm_loadu_ps((sp0 + src_stride));
			x1 = _mm_loadu_ps((sp1 + src_stride));
			x2 = _mm_loadu_ps((sp2 + src_stride));
			x3 = _mm_loadu_ps((sp3 + src_stride));
			v0 = _mm_unpacklo_ps(x0, x2);
			v1 = _mm_unpacklo_ps(x1, x3);
			s2 = _mm_unpacklo_ps(v0, v1);
			s3 = _mm_unpackhi_ps(v0, v1);

			rdx = _mm_sub_ps(x_one, ddx);

			MUL_S_F32xF32(s0, rdx, v0);
			MUL_S_F32xF32(s1, ddx, v1);
			MUL_S_F32xF32(s2, rdx, v2);
			MUL_S_F32xF32(s3, ddx, v3);

			y0 = _mm_add_ps(v0, v1);
			y1 = _mm_add_ps(v2, v3);

			MUL_S_F32xF32(rdy, y0, s0);
			MUL_S_F32xF32(ddy, y1, s1);

			v0 = _mm_add_ps(s0, s1);

			mlib_s32 dt = width - i * 4;
			for (k = 0; k < dt; k++) {
				((DTYPE *)tdp)[k] = ((DTYPE *)&v0)[k];
			}
		}

		y_step = (mlib_s32)(yd + dy) - y;
		dp += dst_stride;
		yd += dy;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_ImageZoomBilinear_F32_2s(
    mlib_work_image * param)
{
	VARIABLEFP(mlib_f32);
	DTYPE *tdp;
	mlib_s32 k, width4, y_step = 2;
	__m128 ddx_start, ddx, ddy, rdy;
	__m128 x_one = _mm_set1_ps(1.0);

	width *= 2;
	width4 = width / 4;

/* calculate dx for all row */
	xd = (FTYPE) GetElemStruct(xstart);

	for (i = 0; i < 2; i++) {
		((DTYPE *)&ddx_start)[2 * i] = xd - (mlib_s32)xd;
		((DTYPE *)&ddx_start)[2 * i + 1] =
		    xd + dx - (mlib_s32)(xd + dx);
	}

	for (j = 0; j < height; j++) {
		y = (mlib_s32)yd;

		tdp = dp;
		DTYPE *tsp = sp + y * src_stride;

		__m128 ddx, rdx;
		ddx = ddx_start;
		ddy = _mm_set1_ps((yd - y));
		rdy = _mm_sub_ps(x_one, ddy);
		xd = GetElemStruct(xstart);

		for (i = 0; i < width4; i++) {
			DTYPE *sp0 = tsp + 2 * (mlib_s32)xd;
			DTYPE *sp1 =
			    tsp + 2 * (mlib_s32)(xd + dx);

			__m128 x0, x1, x2, x3, y0, y1;
			__m128 v0, v1, v2, v3;
			__m128 s0, s1, s2, s3;

			x0 = _mm_loadu_ps(sp0);
			x1 = _mm_loadu_ps(sp1);
			s0 = _mm_unpacklo_ps(x0, x1);
			s1 = _mm_unpackhi_ps(x0, x1);

			x2 = _mm_loadu_ps((sp0 + src_stride));
			x3 = _mm_loadu_ps((sp1 + src_stride));
			s2 = _mm_unpacklo_ps(x2, x3);
			s3 = _mm_unpackhi_ps(x2, x3);

			rdx = _mm_sub_ps(x_one, ddx);

			MUL_S_F32xF32(s0, rdx, v0);
			MUL_S_F32xF32(s1, ddx, v1);
			MUL_S_F32xF32(s2, rdx, v2);
			MUL_S_F32xF32(s3, ddx, v3);

			y0 = _mm_add_ps(v0, v1);
			y1 = _mm_add_ps(v2, v3);

			MUL_S_F32xF32(rdy, y0, s0);
			MUL_S_F32xF32(ddy, y1, s1);

			v2 = _mm_add_ps(s0, s1);
			/* 0xD8 = _MM_SHUFFLE(11, 01, 10, 00) */
			v3 = _mm_shuffle_ps(v2, v2, 0xD8);
			_mm_storeu_ps(tdp, v3);
			tdp += 4;
			xd += 2.0 * dx;
			mlib_f32 dx0, dx1;
			dx0 = xd - (mlib_s32)xd;
			dx1 = xd + dx - (mlib_s32)(xd + dx);
			ddx = _mm_setr_ps(dx0, dx1, dx0, dx1);
		}

		if (i * 4 < width) {
			DTYPE *sp0 = tsp + 2 * (mlib_s32)xd;

			__m128 x0, x2, y0, y1;
			__m128 v0, v1, v2, v3;
			__m128 s0, s1, s2, s3;
			__m128 x_zero = _mm_setzero_ps();

			x0 = _mm_loadu_ps(sp0);
			s0 = _mm_unpacklo_ps(x0, x_zero);
			s1 = _mm_unpackhi_ps(x0, x_zero);

			x2 = _mm_loadu_ps((sp0 + src_stride));
			s2 = _mm_unpacklo_ps(x2, x_zero);
			s3 = _mm_unpackhi_ps(x2, x_zero);

			rdx = _mm_sub_ps(x_one, ddx);

			MUL_S_F32xF32(s0, rdx, v0);
			MUL_S_F32xF32(s1, ddx, v1);
			MUL_S_F32xF32(s2, rdx, v2);
			MUL_S_F32xF32(s3, ddx, v3);

			y0 = _mm_add_ps(v0, v1);
			y1 = _mm_add_ps(v2, v3);

			MUL_S_F32xF32(rdy, y0, s0);
			MUL_S_F32xF32(ddy, y1, s1);

			v2 = _mm_add_ps(s0, s1);
			/* 0xD8 = _MM_SHUFFLE(11, 01, 10, 00) */
			v3 = _mm_shuffle_ps(v2, v2, 0xD8);
			mlib_s32 dt = width - i * 4;
			for (k = 0; k < dt; k++) {
				((DTYPE *)tdp)[k] = ((DTYPE *)&v3)[k];
			}
		}

		y_step = (mlib_s32)(yd + dy) - y;
		dp += dst_stride;
		yd += dy;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_ImageZoomBilinear_F32_3s(
    mlib_work_image * param)
{
	VARIABLEFP(mlib_f32);
	DTYPE *tdp;
	mlib_s32 k, width3, y_step = 2;
	__m128 ddx_start, ddx, ddy, rdy;
	__m128 x_one = _mm_set1_ps(1.0);

	width *= 3;
	width3 = width / 3 - 1;

/* calculate dx for all row */
	xd = (FTYPE) GetElemStruct(xstart);
	x = (mlib_s32)xd;
	ddx_start = _mm_set1_ps((xd - x));

	for (j = 0; j < height; j++) {
		y = (mlib_s32)yd;

		tdp = dp;
		DTYPE *tsp = sp + y * src_stride;

		__m128 ddx, rdx;
		ddx = ddx_start;
		ddy = _mm_set1_ps((yd - y));
		rdy = _mm_sub_ps(x_one, ddy);
		xd = GetElemStruct(xstart);

		for (i = 0; i < width3; i++) {
			DTYPE *sp0 = tsp + 3 * (mlib_s32)xd;

			__m128 x0, x1, x2, x3, y0, y1;
			__m128 v0, v1, v2, v3, s0, s1;

			x0 = _mm_loadu_ps(sp0);
			x1 = _mm_loadu_ps(sp0 + 3);
			x2 = _mm_loadu_ps((sp0 + src_stride));
			x3 = _mm_loadu_ps((sp0 + 3 + src_stride));

			rdx = _mm_sub_ps(x_one, ddx);

			MUL_S_F32xF32(x0, rdx, v0);
			MUL_S_F32xF32(x1, ddx, v1);
			MUL_S_F32xF32(x2, rdx, v2);
			MUL_S_F32xF32(x3, ddx, v3);

			y0 = _mm_add_ps(v0, v1);
			y1 = _mm_add_ps(v2, v3);

			MUL_S_F32xF32(rdy, y0, s0);
			MUL_S_F32xF32(ddy, y1, s1);

			v0 = _mm_add_ps(s0, s1);
			_mm_storeu_ps(tdp, v0);
			tdp += 3;
			xd += dx;
			ddx = _mm_set1_ps((xd - (mlib_s32)xd));
		}

		if (i * 3 < width) {
			DTYPE *sp0 = tsp + 3 * (mlib_s32)xd;

			__m128 x0, x1, x2, x3, y0, y1;
			__m128 v0, v1, v2, v3, s0, s1;

			x0 = _mm_loadu_ps(sp0);
			x1 = _mm_loadu_ps(sp0 + 3);
			x2 = _mm_loadu_ps((sp0 + src_stride));
			x3 = _mm_loadu_ps((sp0 + 3 + src_stride));

			rdx = _mm_sub_ps(x_one, ddx);

			MUL_S_F32xF32(x0, rdx, v0);
			MUL_S_F32xF32(x1, ddx, v1);
			MUL_S_F32xF32(x2, rdx, v2);
			MUL_S_F32xF32(x3, ddx, v3);

			y0 = _mm_add_ps(v0, v1);
			y1 = _mm_add_ps(v2, v3);

			MUL_S_F32xF32(rdy, y0, s0);
			MUL_S_F32xF32(ddy, y1, s1);

			v0 = _mm_add_ps(s0, s1);
			tdp[0] = ((DTYPE *)&v0)[0];
			tdp[1] = ((DTYPE *)&v0)[1];
			tdp[2] = ((DTYPE *)&v0)[2];
		}

		y_step = (mlib_s32)(yd + dy) - y;
		dp += dst_stride;
		yd += dy;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_ImageZoomBilinear_F32_4s(
    mlib_work_image * param)
{
	VARIABLEFP(mlib_f32);
	DTYPE *tdp;
	mlib_s32 k, width4, size, y_step = 2;
	__m128 ddx_start, ddx, ddy, rdy;
	__m128 x_one = _mm_set1_ps(1.0);

	width *= 4;
	width4 = (width + 3) / 4;

/* calculate dx for all row */
	xd = (FTYPE) GetElemStruct(xstart);

	x = (mlib_s32)xd;
	ddx_start = _mm_set1_ps((xd - x));

	for (j = 0; j < height; j++) {
		y = (mlib_s32)yd;

		tdp = dp;
		DTYPE *tsp = sp + y * src_stride;

		__m128 ddx, rdx;
		ddx = ddx_start;
		ddy = _mm_set1_ps((yd - y));
		rdy = _mm_sub_ps(x_one, ddy);
		xd = GetElemStruct(xstart);

		for (i = 0; i < width4; i++) {
			DTYPE *sp0 = tsp + 4 * (mlib_s32)xd;

			__m128 x0, x1, x2, x3, y0, y1;
			__m128 v0, v1, v2, v3, s0, s1;

			x0 = _mm_loadu_ps(sp0);
			x1 = _mm_loadu_ps(sp0 + 4);
			x2 = _mm_loadu_ps((sp0 + src_stride));
			x3 = _mm_loadu_ps((sp0 + 4 + src_stride));

			rdx = _mm_sub_ps(x_one, ddx);

			MUL_S_F32xF32(x0, rdx, v0);
			MUL_S_F32xF32(x1, ddx, v1);
			MUL_S_F32xF32(x2, rdx, v2);
			MUL_S_F32xF32(x3, ddx, v3);

			y0 = _mm_add_ps(v0, v1);
			y1 = _mm_add_ps(v2, v3);

			MUL_S_F32xF32(rdy, y0, s0);
			MUL_S_F32xF32(ddy, y1, s1);

			v0 = _mm_add_ps(s0, s1);
			_mm_storeu_ps(tdp, v0);
			tdp += 4;
			xd += dx;
			ddx = _mm_set1_ps((xd - (mlib_s32)xd));
		}

		y_step = (mlib_s32)(yd + dy) - y;
		dp += dst_stride;
		yd += dy;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
