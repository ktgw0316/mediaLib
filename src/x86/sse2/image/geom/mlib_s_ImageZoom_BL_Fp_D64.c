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

#pragma ident	"@(#)mlib_s_ImageZoom_BL_Fp_D64.c	9.2	07/11/05 SMI"

/*
 * FUNCTION
 *   Internal functions for mlib_ImageZoom on MLIB_BILINEAR filter
 *   and D64 data type
 */

#include <mlib_s_ImageZoom.h>

/* *********************************************************** */

#define	DTYPE	mlib_d64
#define	FTYPE	mlib_d64

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

#define	MUL_S_D64xD64(S0, S1, D0)                \
	D0 = _mm_mul_pd(S0, S1)

/* *********************************************************** */

#undef	BUFF_SIZE
#define	BUFF_SIZE	4096

/* *********************************************************** */

mlib_status
mlib_ImageZoomBilinear_D64_1(
    mlib_work_image * param)
{
	VARIABLEFP(mlib_d64);
	__m128d buff_arr[BUFF_SIZE / 16], *buff0, *buff1;
	void *buff = buff_arr;
	DTYPE *tdp;
	mlib_s32 k, width2, buffsize, size, y_step = 2;
	__m128d ddx_start, ddy, rdy;
	__m128d x_one = _mm_set1_pd(1.0);

	width2 = width / 2;
	buffsize = (width + 1) / 2;

	size = 2 * buffsize * sizeof (__m128d);
	if (size > BUFF_SIZE) {
		buff = __mlib_malloc(size);
		if (buff == NULL)
			return (MLIB_FAILURE);
	}

	buff0 = buff;
	buff1 = buff0 + buffsize;

/* calculate dx for all row */
	xd = (FTYPE) GetElemStruct(xstart);

	mlib_d64 dx0, dx1;
	dx0 = xd - (mlib_s32)xd;
	dx1 = xd + dx - (mlib_s32)(xd + dx);
	ddx_start = _mm_setr_pd(dx0, dx1);

	for (j = 0; j < height; j++) {
		tdp = dp;

		y = (mlib_s32)yd;

		if (y_step == 0) {
			ddy = _mm_set1_pd((yd - y));
			rdy = _mm_sub_pd(x_one, ddy);

			for (i = 0; i < width2; i++) {
				__m128d y0, y1, t0, t1, d0;
				y0 = buff0[i];
				y1 = buff1[i];
				MUL_S_D64xD64(rdy, y0, t0);
				MUL_S_D64xD64(ddy, y1, t1);
				d0 = _mm_add_pd(t0, t1);
				_mm_storeu_pd(tdp, d0);
				tdp += 2;
			}

			if (i * 2 < width) {
				__m128d y0, y1, t0, t1, d0, d1, d2;
				y0 = buff0[i];
				y1 = buff1[i];
				MUL_S_D64xD64(rdy, y0, t0);
				MUL_S_D64xD64(ddy, y1, t1);
				d0 = _mm_add_pd(t0, t1);
				_mm_storel_pd(tdp, d0);
			}

		} else if (y_step == 1) {
			__m128d *buffT;
			__m128d ddx, rdx;
			DTYPE *tsp = sp + y * src_stride + src_stride;

			ddx = ddx_start;
			ddy = _mm_set1_pd((yd - y));
			rdy = _mm_sub_pd(x_one, ddy);
			xd = GetElemStruct(xstart);

			buffT = buff0;
			buff0 = buff1;
			buff1 = buffT;

			for (i = 0; i < width2; i++) {
				DTYPE *sp0 = tsp + (mlib_s32)xd;
				DTYPE *sp1 =
				    tsp + (mlib_s32)(xd + dx);

				__m128d x0, x1, y0, y1;
				__m128d v0, v1, s0, s1;

				x0 = _mm_loadu_pd(sp0);
				x1 = _mm_loadu_pd(sp1);
				s0 = _mm_unpacklo_pd(x0, x1);
				s1 = _mm_unpackhi_pd(x0, x1);

				rdx = _mm_sub_pd(x_one, ddx);

				MUL_S_D64xD64(s0, rdx, v0);
				MUL_S_D64xD64(s1, ddx, v1);

				y1 = _mm_add_pd(v0, v1);
				buff1[i] = y1;
				y0 = buff0[i];

				MUL_S_D64xD64(rdy, y0, s0);
				MUL_S_D64xD64(ddy, y1, s1);

				v0 = _mm_add_pd(s0, s1);
				_mm_storeu_pd(tdp, v0);
				tdp += 2;
				xd += 2.0 * dx;
				mlib_d64 dx0, dx1;
				dx0 = xd - (mlib_s32)xd;
				dx1 = xd + dx - (mlib_s32)(xd + dx);
				ddx = _mm_setr_pd(dx0, dx1);
			}

			if (i * 2 < width) {
				DTYPE *sp0 = tsp + (mlib_s32)xd;
				DTYPE *sp1 =
				    tsp + (mlib_s32)(xd + dx);

				__m128d x0, x1, y0, y1;
				__m128d v0, v1, s0, s1;

				x0 = _mm_loadu_pd(sp0);
				x1 = _mm_loadu_pd(sp1);
				s0 = _mm_unpacklo_pd(x0, x1);
				s1 = _mm_unpackhi_pd(x0, x1);

				rdx = _mm_sub_pd(x_one, ddx);

				MUL_S_D64xD64(s0, rdx, v0);
				MUL_S_D64xD64(s1, ddx, v1);

				y1 = _mm_add_pd(v0, v1);
				buff1[i] = y1;
				y0 = buff0[i];

				MUL_S_D64xD64(rdy, y0, s0);
				MUL_S_D64xD64(ddy, y1, s1);

				v0 = _mm_add_pd(s0, s1);
				_mm_storel_pd(tdp, v0);
			}

		} else {
			__m128d ddx, rdx;
			DTYPE *tsp = sp + y * src_stride;

			ddx = ddx_start;
			ddy = _mm_set1_pd((yd - y));
			rdy = _mm_sub_pd(x_one, ddy);
			xd = GetElemStruct(xstart);

			for (i = 0; i < width2; i++) {
				DTYPE *sp0 = tsp + (mlib_s32)xd;
				DTYPE *sp1 =
				    tsp + (mlib_s32)(xd + dx);

				__m128d x0, x1, x2, x3, y0, y1;
				__m128d v0, v1, v2, v3;
				__m128d s0, s1, s2, s3;

				x0 = _mm_loadu_pd(sp0);
				x1 = _mm_loadu_pd(sp1);
				s0 = _mm_unpacklo_pd(x0, x1);
				s1 = _mm_unpackhi_pd(x0, x1);

				x2 = _mm_loadu_pd((sp0 + src_stride));
				x3 = _mm_loadu_pd((sp1 + src_stride));
				s2 = _mm_unpacklo_pd(x2, x3);
				s3 = _mm_unpackhi_pd(x2, x3);

				rdx = _mm_sub_pd(x_one, ddx);

				MUL_S_D64xD64(s0, rdx, v0);
				MUL_S_D64xD64(s1, ddx, v1);
				MUL_S_D64xD64(s2, rdx, v2);
				MUL_S_D64xD64(s3, ddx, v3);

				y0 = _mm_add_pd(v0, v1);
				y1 = _mm_add_pd(v2, v3);

				MUL_S_D64xD64(rdy, y0, s0);
				MUL_S_D64xD64(ddy, y1, s1);

				buff0[i] = y0;
				buff1[i] = y1;

				v2 = _mm_add_pd(s0, s1);
				_mm_storeu_pd(tdp, v2);
				tdp += 2;
				xd += 2.0 * dx;
				mlib_d64 dx0, dx1;
				dx0 = xd - (mlib_s32)xd;
				dx1 = xd + dx - (mlib_s32)(xd + dx);
				ddx = _mm_setr_pd(dx0, dx1);
			}

			if (i * 2 < width) {
				DTYPE *sp0 = tsp + (mlib_s32)xd;
				DTYPE *sp1 =
				    tsp + (mlib_s32)(xd + dx);

				__m128d x0, x1, x2, x3, y0, y1;
				__m128d v0, v1, v2, v3;
				__m128d s0, s1, s2, s3;

				x0 = _mm_loadu_pd(sp0);
				x1 = _mm_loadu_pd(sp1);
				s0 = _mm_unpacklo_pd(x0, x1);
				s1 = _mm_unpackhi_pd(x0, x1);

				x2 = _mm_loadu_pd((sp0 + src_stride));
				x3 = _mm_loadu_pd((sp1 + src_stride));
				s2 = _mm_unpacklo_pd(x2, x3);
				s3 = _mm_unpackhi_pd(x2, x3);

				rdx = _mm_sub_pd(x_one, ddx);

				MUL_S_D64xD64(s0, rdx, v0);
				MUL_S_D64xD64(s1, ddx, v1);
				MUL_S_D64xD64(s2, rdx, v2);
				MUL_S_D64xD64(s3, ddx, v3);

				y0 = _mm_add_pd(v0, v1);
				y1 = _mm_add_pd(v2, v3);

				MUL_S_D64xD64(rdy, y0, s0);
				MUL_S_D64xD64(ddy, y1, s1);

				buff0[i] = y0;
				buff1[i] = y1;

				v0 = _mm_add_pd(s0, s1);
				_mm_storel_pd(tdp, v0);
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
mlib_ImageZoomBilinear_D64_2(
    mlib_work_image * param)
{
	VARIABLEFP(mlib_d64);
	__m128d buff_arr[BUFF_SIZE / 16], *buff0, *buff1;
	void *buff = buff_arr;
	DTYPE *tdp;
	mlib_s32 k, width2, buffsize, size, y_step = 2;
	__m128d ddx_start, ddy, rdy;
	__m128d x_one = _mm_set1_pd(1.0);

	width2 = width;
	width *= 2;
	buffsize = (width + 1) / 2;

	size = 2 * buffsize * sizeof (__m128d);
	if (size > BUFF_SIZE) {
		buff = __mlib_malloc(size);
		if (buff == NULL)
			return (MLIB_FAILURE);
	}

	buff0 = buff;
	buff1 = buff0 + buffsize;

/* calculate dx for all row */
	xd = (FTYPE) GetElemStruct(xstart);

	mlib_d64 dx0 = xd - (mlib_s32)xd;
	ddx_start = _mm_set1_pd(dx0);

	for (j = 0; j < height; j++) {
		tdp = dp;

		y = (mlib_s32)yd;

		if (y_step == 0) {
			ddy = _mm_set1_pd((yd - y));
			rdy = _mm_sub_pd(x_one, ddy);

			for (i = 0; i < width2; i++) {
				__m128d y0, y1, s0, s1, d0, d1;
				y0 = buff0[i];
				y1 = buff1[i];
				MUL_S_D64xD64(rdy, y0, s0);
				MUL_S_D64xD64(ddy, y1, s1);
				d0 = _mm_add_pd(s0, s1);
				_mm_storeu_pd(tdp, d0);
				tdp += 2;
			}

		} else if (y_step == 1) {
			__m128d *buffT;
			__m128d ddx, rdx;
			DTYPE *tsp = sp + y * src_stride + src_stride;

			ddx = ddx_start;
			ddy = _mm_set1_pd((yd - y));
			rdy = _mm_sub_pd(x_one, ddy);
			xd = GetElemStruct(xstart);

			buffT = buff0;
			buff0 = buff1;
			buff1 = buffT;

			for (i = 0; i < width2; i++) {
				DTYPE *sp0 = tsp + 2 * (mlib_s32)xd;

				__m128d y0, y1;
				__m128d v0, v1, s0, s1;

				s0 = _mm_loadu_pd(sp0);
				s1 = _mm_loadu_pd(sp0 + 2);

				rdx = _mm_sub_pd(x_one, ddx);

				MUL_S_D64xD64(s0, rdx, v0);
				MUL_S_D64xD64(s1, ddx, v1);

				y1 = _mm_add_pd(v0, v1);
				buff1[i] = y1;
				y0 = buff0[i];

				MUL_S_D64xD64(rdy, y0, s0);
				MUL_S_D64xD64(ddy, y1, s1);

				v0 = _mm_add_pd(s0, s1);
				_mm_storeu_pd(tdp, v0);
				tdp += 2;
				xd += dx;
				mlib_d64 dx0 = xd - (mlib_s32)xd;
				ddx = _mm_set1_pd(dx0);
			}

		} else {
			__m128d ddx, rdx;
			DTYPE *tsp = sp + y * src_stride;

			ddx = ddx_start;
			ddy = _mm_set1_pd((yd - y));
			rdy = _mm_sub_pd(x_one, ddy);
			xd = GetElemStruct(xstart);

			for (i = 0; i < width2; i++) {
				DTYPE *sp0 = tsp + 2 * (mlib_s32)xd;

				__m128d y0, y1;
				__m128d v0, v1, v2, v3;
				__m128d s0, s1, s2, s3;

				s0 = _mm_loadu_pd(sp0);
				s1 = _mm_loadu_pd(sp0 + 2);
				s2 = _mm_loadu_pd((sp0 + src_stride));
				s3 = _mm_loadu_pd((sp0 + 2 + src_stride));

				rdx = _mm_sub_pd(x_one, ddx);

				MUL_S_D64xD64(s0, rdx, v0);
				MUL_S_D64xD64(s1, ddx, v1);
				MUL_S_D64xD64(s2, rdx, v2);
				MUL_S_D64xD64(s3, ddx, v3);

				y0 = _mm_add_pd(v0, v1);
				y1 = _mm_add_pd(v2, v3);

				MUL_S_D64xD64(rdy, y0, s0);
				MUL_S_D64xD64(ddy, y1, s1);

				buff0[i] = y0;
				buff1[i] = y1;

				v0 = _mm_add_pd(s0, s1);
				_mm_storeu_pd(tdp, v0);
				tdp += 2;
				xd += dx;
				mlib_d64 dx0 = xd - (mlib_s32)xd;
				ddx = _mm_set1_pd(dx0);
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
#define	BUFF_SIZE	16384

mlib_status
mlib_ImageZoomBilinear_D64_3(
    mlib_work_image * param)
{
	VARIABLEFP(mlib_d64);
	__m128d buff_arr[BUFF_SIZE / 16], *buff0, *buff1;
	void *buff = buff_arr;
	DTYPE *tdp;
	mlib_s32 k, width3, buffsize, size, y_step = 2;
	__m128d ddx_start, ddy, rdy, dx_step;
	__m128d x_one = _mm_set1_pd(1.0);

	width3 = width;
	width *= 3;
	buffsize = (width + 2) / 3;

	size = 4 * buffsize * sizeof (__m128d);
	if (size > BUFF_SIZE) {
		buff = __mlib_malloc(size);
		if (buff == NULL)
			return (MLIB_FAILURE);
	}

	buff0 = buff;
	buff1 = buff0 + 2 * buffsize;

/* calculate dx for all row */
	xd = (FTYPE) GetElemStruct(xstart);

	x = (mlib_s32)xd;
	ddx_start = _mm_set1_pd((xd - x));
	dx_step = _mm_set1_pd(dx);

	for (j = 0; j < height; j++) {
		tdp = dp;

		y = (mlib_s32)yd;

		if (y_step == 0) {
			ddy = _mm_set1_pd((yd - y));
			rdy = _mm_sub_pd(x_one, ddy);

			for (i = 0; i < width3; i++) {
				__m128d ya0, ya1, sa0, sa1;
				__m128d yb0, yb1, sb0, sb1;
				__m128d va0, va1;
				ya0 = buff0[2 * i];
				ya1 = buff0[2 * i + 1];
				yb0 = buff1[2 * i];
				yb1 = buff1[2 * i + 1];

				MUL_S_D64xD64(rdy, ya0, sa0);
				MUL_S_D64xD64(rdy, ya1, sa1);
				MUL_S_D64xD64(ddy, yb0, sb0);
				MUL_S_D64xD64(ddy, yb1, sb1);

				va0 = _mm_add_pd(sa0, sb0);
				va1 = _mm_add_pd(sa1, sb1);
				_mm_storeu_pd(tdp, va0);
				_mm_storel_pd((tdp + 2), va1);
				tdp += 3;
			}

		} else if (y_step == 1) {
			__m128d *buffT;
			__m128d ddx, rdx;
			DTYPE *tsp = sp + y * src_stride + src_stride;

			ddx = ddx_start;
			ddy = _mm_set1_pd((yd - y));
			rdy = _mm_sub_pd(x_one, ddy);
			xd = GetElemStruct(xstart);

			buffT = buff0;
			buff0 = buff1;
			buff1 = buffT;

			for (i = 0; i < width3; i++) {
				DTYPE *sp0 = tsp + 3 * (mlib_s32)xd;

				__m128d ya0, ya1, yb0, yb1;
				__m128d va0, va1, va2, va3;
				__m128d sa0, sa1, sa2, sa3;

				sa0 = _mm_loadu_pd(sp0);
				sa1 = _mm_loadu_pd(sp0 + 2);
				sa2 = _mm_loadu_pd(sp0 + 3);
				sa3 = _mm_loadu_pd(sp0 + 5);

				rdx = _mm_sub_pd(x_one, ddx);

				MUL_S_D64xD64(sa0, rdx, va0);
				MUL_S_D64xD64(sa1, rdx, va1);
				MUL_S_D64xD64(sa2, ddx, va2);
				MUL_S_D64xD64(sa3, ddx, va3);

				yb0 = _mm_add_pd(va0, va2);
				yb1 = _mm_add_pd(va1, va3);
				buff1[2 * i] = yb0;
				buff1[2 * i + 1] = yb1;
				ya0 = buff0[2 * i];
				ya1 = buff0[2 * i + 1];

				MUL_S_D64xD64(rdy, ya0, sa0);
				MUL_S_D64xD64(rdy, ya1, sa1);
				MUL_S_D64xD64(ddy, yb0, sa2);
				MUL_S_D64xD64(ddy, yb1, sa3);

				va0 = _mm_add_pd(sa0, sa2);
				va1 = _mm_add_pd(sa1, sa3);
				_mm_storeu_pd(tdp, va0);
				_mm_storel_pd((tdp + 2), va1);
				tdp += 3;
				xd += dx;
				ddx = _mm_set1_pd((xd - (mlib_s32)xd));
			}

		} else {
			__m128d ddx, rdx;
			DTYPE *tsp = sp + y * src_stride;

			ddx = ddx_start;
			ddy = _mm_set1_pd((yd - y));
			rdy = _mm_sub_pd(x_one, ddy);
			xd = GetElemStruct(xstart);

			for (i = 0; i < width3; i++) {
				DTYPE *sp0 = tsp + 3 * (mlib_s32)xd;

				__m128d ya0, ya1, yb0, yb1;
				__m128d va0, va1, va2, va3;
				__m128d vb0, vb1, vb2, vb3;
				__m128d sa0, sa1, sa2, sa3;
				__m128d sb0, sb1, sb2, sb3;

				sa0 = _mm_loadu_pd(sp0);
				sa1 = _mm_loadu_pd(sp0 + 2);
				sa2 = _mm_loadu_pd(sp0 + 3);
				sa3 = _mm_loadu_pd(sp0 + 5);
				sb0 = _mm_loadu_pd(sp0 + src_stride);
				sb1 = _mm_loadu_pd(sp0 + 2 + src_stride);
				sb2 = _mm_loadu_pd(sp0 + 3 + src_stride);
				sb3 = _mm_loadu_pd(sp0 + 5 + src_stride);

				rdx = _mm_sub_pd(x_one, ddx);

				MUL_S_D64xD64(sa0, rdx, va0);
				MUL_S_D64xD64(sa1, rdx, va1);
				MUL_S_D64xD64(sa2, ddx, va2);
				MUL_S_D64xD64(sa3, ddx, va3);

				MUL_S_D64xD64(sb0, rdx, vb0);
				MUL_S_D64xD64(sb1, rdx, vb1);
				MUL_S_D64xD64(sb2, ddx, vb2);
				MUL_S_D64xD64(sb3, ddx, vb3);

				ya0 = _mm_add_pd(va0, va2);
				ya1 = _mm_add_pd(va1, va3);
				yb0 = _mm_add_pd(vb0, vb2);
				yb1 = _mm_add_pd(vb1, vb3);

				MUL_S_D64xD64(rdy, ya0, sa0);
				MUL_S_D64xD64(rdy, ya1, sa1);
				MUL_S_D64xD64(ddy, yb0, sb0);
				MUL_S_D64xD64(ddy, yb1, sb1);

				buff0[2 * i] = ya0;
				buff0[2 * i + 1] = ya1;
				buff1[2 * i] = yb0;
				buff1[2 * i + 1] = yb1;

				va0 = _mm_add_pd(sa0, sb0);
				va1 = _mm_add_pd(sa1, sb1);
				_mm_storeu_pd(tdp, va0);
				_mm_storel_pd((tdp + 2), va1);
				tdp += 3;
				xd += dx;
				ddx = _mm_set1_pd((xd - (mlib_s32)xd));
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
mlib_ImageZoomBilinear_D64_4(
    mlib_work_image * param)
{
	VARIABLEFP(mlib_d64);
	__m128d buff_arr[BUFF_SIZE / 16], *buff0, *buff1;
	void *buff = buff_arr;
	DTYPE *tdp;
	mlib_s32 k, width2, size, y_step = 2;
	__m128d ddx_start, ddy, rdy, dx_step;
	__m128d x_one = _mm_set1_pd(1.0);

	width *= 4;
	width2 = (width + 3) / 4;

	size = 4 * width2 * sizeof (__m128d);
	if (size > BUFF_SIZE) {
		buff = __mlib_malloc(size);
		if (buff == NULL)
			return (MLIB_FAILURE);
	}

	buff0 = buff;
	buff1 = buff0 + 2 * width2;

/* calculate dx for all row */
	xd = (FTYPE) GetElemStruct(xstart);

	x = (mlib_s32)xd;
	ddx_start = _mm_set1_pd((xd - x));

	for (j = 0; j < height; j++) {
		tdp = dp;

		y = (mlib_s32)yd;

		if (y_step == 0) {
			ddy = _mm_set1_pd((yd - y));
			rdy = _mm_sub_pd(x_one, ddy);

			for (i = 0; i < width2; i++) {
				__m128d ya0, ya1, ya2, ya3;
				__m128d sa0, sa1, sa2, sa3;
				__m128d va0, va1;
				ya0 = buff0[2 * i];
				ya1 = buff0[2 * i + 1];
				ya2 = buff1[2 * i];
				ya3 = buff1[2 * i + 1];

				MUL_S_D64xD64(rdy, ya0, sa0);
				MUL_S_D64xD64(ddy, ya2, sa1);
				MUL_S_D64xD64(rdy, ya1, sa2);
				MUL_S_D64xD64(ddy, ya3, sa3);

				va0 = _mm_add_pd(sa0, sa1);
				va1 = _mm_add_pd(sa2, sa3);
				_mm_storeu_pd(tdp, va0);
				_mm_storeu_pd((tdp + 2), va1);
				tdp += 4;
			}

		} else if (y_step == 1) {
			__m128d *buffT;
			__m128d ddx, rdx;
			DTYPE *tsp = sp + y * src_stride + src_stride;

			ddx = ddx_start;
			ddy = _mm_set1_pd((yd - y));
			rdy = _mm_sub_pd(x_one, ddy);
			xd = GetElemStruct(xstart);

			buffT = buff0;
			buff0 = buff1;
			buff1 = buffT;

			for (i = 0; i < width2; i++) {
				DTYPE *sp0 = tsp + 4 * (mlib_s32)xd;

				__m128d xa0, xa1, xa2, xa3;
				__m128d ya0, ya1, ya2, ya3;
				__m128d va0, va1, va2, va3;
				__m128d sa0, sa1, sa2, sa3;

				xa0 = _mm_loadu_pd(sp0);
				xa1 = _mm_loadu_pd(sp0 + 2);
				xa2 = _mm_loadu_pd(sp0 + 4);
				xa3 = _mm_loadu_pd(sp0 + 6);

				rdx = _mm_sub_pd(x_one, ddx);

				MUL_S_D64xD64(xa0, rdx, va0);
				MUL_S_D64xD64(xa1, rdx, va1);
				MUL_S_D64xD64(xa2, ddx, va2);
				MUL_S_D64xD64(xa3, ddx, va3);

				ya2 = _mm_add_pd(va0, va2);
				ya3 = _mm_add_pd(va1, va3);
				ya0 = buff0[2 * i];
				ya1 = buff0[2 * i + 1];

				MUL_S_D64xD64(rdy, ya0, sa0);
				MUL_S_D64xD64(ddy, ya2, sa1);
				MUL_S_D64xD64(rdy, ya1, sa2);
				MUL_S_D64xD64(ddy, ya3, sa3);

				buff1[2 * i] = ya2;
				buff1[2 * i + 1] = ya3;

				va0 = _mm_add_pd(sa0, sa1);
				va1 = _mm_add_pd(sa2, sa3);
				_mm_storeu_pd(tdp, va0);
				_mm_storeu_pd((tdp + 2), va1);
				tdp += 4;
				xd += dx;
				ddx = _mm_set1_pd((xd - (mlib_s32)xd));
			}

		} else {
			__m128d ddx, rdx;
			DTYPE *tsp = sp + y * src_stride;

			ddx = ddx_start;
			ddy = _mm_set1_pd((yd - y));
			rdy = _mm_sub_pd(x_one, ddy);
			xd = GetElemStruct(xstart);

			for (i = 0; i < width2; i++) {
				DTYPE *sp0 = tsp + 4 * (mlib_s32)xd;

				__m128d xa0, xa1, xa2, xa3;
				__m128d xb0, xb1, xb2, xb3;
				__m128d ya0, ya1, ya2, ya3;
				__m128d va0, va1, va2, va3;
				__m128d vb0, vb1, vb2, vb3;
				__m128d sa0, sa1, sa2, sa3;

				xa0 = _mm_loadu_pd(sp0);
				xa1 = _mm_loadu_pd(sp0 + 2);
				xa2 = _mm_loadu_pd(sp0 + 4);
				xa3 = _mm_loadu_pd(sp0 + 6);
				xb0 = _mm_loadu_pd(sp0 + src_stride);
				xb1 = _mm_loadu_pd(sp0 + 2 + src_stride);
				xb2 = _mm_loadu_pd(sp0 + 4 + src_stride);
				xb3 = _mm_loadu_pd(sp0 + 6 + src_stride);

				rdx = _mm_sub_pd(x_one, ddx);

				MUL_S_D64xD64(xa0, rdx, va0);
				MUL_S_D64xD64(xa1, rdx, va1);
				MUL_S_D64xD64(xa2, ddx, va2);
				MUL_S_D64xD64(xa3, ddx, va3);
				MUL_S_D64xD64(xb0, rdx, vb0);
				MUL_S_D64xD64(xb1, rdx, vb1);
				MUL_S_D64xD64(xb2, ddx, vb2);
				MUL_S_D64xD64(xb3, ddx, vb3);

				ya0 = _mm_add_pd(va0, va2);
				ya1 = _mm_add_pd(va1, va3);
				ya2 = _mm_add_pd(vb0, vb2);
				ya3 = _mm_add_pd(vb1, vb3);

				MUL_S_D64xD64(rdy, ya0, sa0);
				MUL_S_D64xD64(ddy, ya2, sa1);
				MUL_S_D64xD64(rdy, ya1, sa2);
				MUL_S_D64xD64(ddy, ya3, sa3);

				buff0[2 * i] = ya0;
				buff0[2 * i + 1] = ya1;
				buff1[2 * i] = ya2;
				buff1[2 * i + 1] = ya3;

				va0 = _mm_add_pd(sa0, sa1);
				va1 = _mm_add_pd(sa2, sa3);
				_mm_storeu_pd(tdp, va0);
				_mm_storeu_pd((tdp + 2), va1);
				tdp += 4;
				xd += dx;
				ddx = _mm_set1_pd((xd - (mlib_s32)xd));
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
mlib_ImageZoomBilinear_D64_1s(
    mlib_work_image * param)
{
	VARIABLEFP(mlib_d64);
	DTYPE *tdp;
	mlib_s32 k, width2, y_step = 2;
	__m128d ddx_start, ddy, rdy;
	__m128d x_one = _mm_set1_pd(1.0);

	width2 = width / 2;

/* calculate dx for all row */
	xd = (FTYPE) GetElemStruct(xstart);

	mlib_d64 dx0, dx1;
	dx0 = xd - (mlib_s32)xd;
	dx1 = xd + dx - (mlib_s32)(xd + dx);
	ddx_start = _mm_setr_pd(dx0, dx1);

	for (j = 0; j < height; j++) {
		tdp = dp;

		y = (mlib_s32)yd;
		DTYPE *tsp = sp + y * src_stride;

		__m128d ddx, rdx;
		ddx = ddx_start;
		ddy = _mm_set1_pd((yd - y));
		rdy = _mm_sub_pd(x_one, ddy);
		xd = GetElemStruct(xstart);

		for (i = 0; i < width2; i++) {
			DTYPE *sp0 = tsp + (mlib_s32)xd;
			DTYPE *sp1 =
			    tsp + (mlib_s32)(xd + dx);

			__m128d x0, x1, x2, x3, y0, y1;
			__m128d v0, v1, v2, v3;
			__m128d s0, s1, s2, s3;

			x0 = _mm_loadu_pd(sp0);
			x1 = _mm_loadu_pd(sp1);
			s0 = _mm_unpacklo_pd(x0, x1);
			s1 = _mm_unpackhi_pd(x0, x1);

			x2 = _mm_loadu_pd((sp0 + src_stride));
			x3 = _mm_loadu_pd((sp1 + src_stride));
			s2 = _mm_unpacklo_pd(x2, x3);
			s3 = _mm_unpackhi_pd(x2, x3);

			rdx = _mm_sub_pd(x_one, ddx);

			MUL_S_D64xD64(s0, rdx, v0);
			MUL_S_D64xD64(s1, ddx, v1);
			MUL_S_D64xD64(s2, rdx, v2);
			MUL_S_D64xD64(s3, ddx, v3);

			y0 = _mm_add_pd(v0, v1);
			y1 = _mm_add_pd(v2, v3);

			MUL_S_D64xD64(rdy, y0, s0);
			MUL_S_D64xD64(ddy, y1, s1);

			v2 = _mm_add_pd(s0, s1);
			_mm_storeu_pd(tdp, v2);
			tdp += 2;
			xd += 2.0 * dx;
			mlib_d64 dx0, dx1;
			dx0 = xd - (mlib_s32)xd;
			dx1 = xd + dx - (mlib_s32)(xd + dx);
			ddx = _mm_setr_pd(dx0, dx1);
		}

		if (i * 2 < width) {
			DTYPE *sp0 = tsp + (mlib_s32)xd;
			DTYPE *sp1 =
			    tsp + (mlib_s32)(xd + dx);

			__m128d x0, x1, x2, x3, y0, y1;
			__m128d v0, v1, v2, v3;
			__m128d s0, s1, s2, s3;

			x0 = _mm_loadu_pd(sp0);
			x1 = _mm_loadu_pd(sp1);
			s0 = _mm_unpacklo_pd(x0, x1);
			s1 = _mm_unpackhi_pd(x0, x1);

			x2 = _mm_loadu_pd((sp0 + src_stride));
			x3 = _mm_loadu_pd((sp1 + src_stride));
			s2 = _mm_unpacklo_pd(x2, x3);
			s3 = _mm_unpackhi_pd(x2, x3);

			rdx = _mm_sub_pd(x_one, ddx);

			MUL_S_D64xD64(s0, rdx, v0);
			MUL_S_D64xD64(s1, ddx, v1);
			MUL_S_D64xD64(s2, rdx, v2);
			MUL_S_D64xD64(s3, ddx, v3);

			y0 = _mm_add_pd(v0, v1);
			y1 = _mm_add_pd(v2, v3);

			MUL_S_D64xD64(rdy, y0, s0);
			MUL_S_D64xD64(ddy, y1, s1);

			v2 = _mm_add_pd(s0, s1);
			_mm_storel_pd(tdp, v2);
		}

		y_step = (mlib_s32)(yd + dy) - y;
		dp += dst_stride;
		yd += dy;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_ImageZoomBilinear_D64_2s(
    mlib_work_image * param)
{
	VARIABLEFP(mlib_d64);
	DTYPE *tdp;
	mlib_s32 k, width2, y_step = 2;
	__m128d ddx_start, ddy, rdy;
	__m128d x_one = _mm_set1_pd(1.0);

	width2 = width;
	width *= 2;

/* calculate dx for all row */
	xd = (FTYPE) GetElemStruct(xstart);

	mlib_d64 dx0 = xd - (mlib_s32)xd;
	ddx_start = _mm_set1_pd(dx0);

	for (j = 0; j < height; j++) {
		tdp = dp;

		y = (mlib_s32)yd;
		__m128d ddx, rdx;
		DTYPE *tsp = sp + y * src_stride;

		ddx = ddx_start;
		ddy = _mm_set1_pd((yd - y));
		rdy = _mm_sub_pd(x_one, ddy);
		xd = GetElemStruct(xstart);

		for (i = 0; i < width2; i++) {
			DTYPE *sp0 = tsp + 2 * (mlib_s32)xd;

			__m128d y0, y1;
			__m128d v0, v1, v2, v3;
			__m128d s0, s1, s2, s3;

			s0 = _mm_loadu_pd(sp0);
			s1 = _mm_loadu_pd(sp0 + 2);
			s2 = _mm_loadu_pd((sp0 + src_stride));
			s3 = _mm_loadu_pd((sp0 + 2 + src_stride));

			rdx = _mm_sub_pd(x_one, ddx);

			MUL_S_D64xD64(s0, rdx, v0);
			MUL_S_D64xD64(s1, ddx, v1);
			MUL_S_D64xD64(s2, rdx, v2);
			MUL_S_D64xD64(s3, ddx, v3);

			y0 = _mm_add_pd(v0, v1);
			y1 = _mm_add_pd(v2, v3);

			MUL_S_D64xD64(rdy, y0, s0);
			MUL_S_D64xD64(ddy, y1, s1);

			v0 = _mm_add_pd(s0, s1);
			_mm_storeu_pd(tdp, v0);
			tdp += 2;
			xd += dx;
			mlib_d64 dx0 = xd - (mlib_s32)xd;
			ddx = _mm_set1_pd(dx0);
		}

		y_step = (mlib_s32)(yd + dy) - y;
		dp += dst_stride;
		yd += dy;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_ImageZoomBilinear_D64_3s(
    mlib_work_image * param)
{
	VARIABLEFP(mlib_d64);
	DTYPE *tdp;
	mlib_s32 k, width3, y_step = 2;
	__m128d ddx_start, ddy, rdy, dx_step;
	__m128d x_one = _mm_set1_pd(1.0);

	width3 = width;
	width *= 3;

/* calculate dx for all row */
	xd = (FTYPE) GetElemStruct(xstart);

	x = (mlib_s32)xd;
	ddx_start = _mm_set1_pd((xd-x));
	dx_step = _mm_set1_pd(dx);

	for (j = 0; j < height; j++) {
		tdp = dp;

		y = (mlib_s32)yd;
		__m128d ddx, rdx;
		DTYPE *tsp = sp + y * src_stride;

		ddx = ddx_start;
		ddy = _mm_set1_pd((yd - y));
		rdy = _mm_sub_pd(x_one, ddy);
		xd = GetElemStruct(xstart);

		for (i = 0; i < width3; i++) {
			DTYPE *sp0 = tsp + 3 * (mlib_s32)xd;

			__m128d ya0, ya1, yb0, yb1;
			__m128d va0, va1, va2, va3;
			__m128d vb0, vb1, vb2, vb3;
			__m128d sa0, sa1, sa2, sa3;
			__m128d sb0, sb1, sb2, sb3;

			sa0 = _mm_loadu_pd(sp0);
			sa1 = _mm_loadu_pd(sp0 + 2);
			sa2 = _mm_loadu_pd(sp0 + 3);
			sa3 = _mm_loadu_pd(sp0 + 5);
			sb0 = _mm_loadu_pd(sp0 + src_stride);
			sb1 = _mm_loadu_pd(sp0 + 2 + src_stride);
			sb2 = _mm_loadu_pd(sp0 + 3 + src_stride);
			sb3 = _mm_loadu_pd(sp0 + 5 + src_stride);

			rdx = _mm_sub_pd(x_one, ddx);

			MUL_S_D64xD64(sa0, rdx, va0);
			MUL_S_D64xD64(sa1, rdx, va1);
			MUL_S_D64xD64(sa2, ddx, va2);
			MUL_S_D64xD64(sa3, ddx, va3);

			MUL_S_D64xD64(sb0, rdx, vb0);
			MUL_S_D64xD64(sb1, rdx, vb1);
			MUL_S_D64xD64(sb2, ddx, vb2);
			MUL_S_D64xD64(sb3, ddx, vb3);

			ya0 = _mm_add_pd(va0, va2);
			ya1 = _mm_add_pd(va1, va3);
			yb0 = _mm_add_pd(vb0, vb2);
			yb1 = _mm_add_pd(vb1, vb3);

			MUL_S_D64xD64(rdy, ya0, sa0);
			MUL_S_D64xD64(rdy, ya1, sa1);
			MUL_S_D64xD64(ddy, yb0, sb0);
			MUL_S_D64xD64(ddy, yb1, sb1);

			va0 = _mm_add_pd(sa0, sb0);
			va1 = _mm_add_pd(sa1, sb1);
			_mm_storeu_pd(tdp, va0);
			_mm_storel_pd((tdp + 2), va1);
			tdp += 3;
			xd += dx;
			ddx = _mm_set1_pd((xd - (mlib_s32)xd));
		}

		y_step = (mlib_s32)(yd + dy) - y;
		dp += dst_stride;
		yd += dy;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_ImageZoomBilinear_D64_4s(
    mlib_work_image * param)
{
	VARIABLEFP(mlib_d64);
	DTYPE *tdp;
	mlib_s32 k, width2, y_step = 2;
	__m128d ddx_start, ddy, rdy, dx_step;
	__m128d x_one = _mm_set1_pd(1.0);

	width *= 4;
	width2 = (width + 3) / 4;

/* calculate dx for all row */
	xd = (FTYPE) GetElemStruct(xstart);

	x = (mlib_s32)xd;
	ddx_start = _mm_set1_pd((xd - x));

	for (j = 0; j < height; j++) {
		tdp = dp;

		y = (mlib_s32)yd;
		__m128d ddx, rdx;
		DTYPE *tsp = sp + y * src_stride;

		ddx = ddx_start;
		ddy = _mm_set1_pd((yd - y));
		rdy = _mm_sub_pd(x_one, ddy);
		xd = GetElemStruct(xstart);

		for (i = 0; i < width2; i++) {
			DTYPE *sp0 = tsp + 4 * (mlib_s32)xd;

			__m128d xa0, xa1, xa2, xa3;
			__m128d xb0, xb1, xb2, xb3;
			__m128d ya0, ya1, ya2, ya3;
			__m128d va0, va1, va2, va3;
			__m128d vb0, vb1, vb2, vb3;
			__m128d sa0, sa1, sa2, sa3;

			xa0 = _mm_loadu_pd(sp0);
			xa1 = _mm_loadu_pd(sp0 + 2);
			xa2 = _mm_loadu_pd(sp0 + 4);
			xa3 = _mm_loadu_pd(sp0 + 6);
			xb0 = _mm_loadu_pd(sp0 + src_stride);
			xb1 = _mm_loadu_pd(sp0 + 2 + src_stride);
			xb2 = _mm_loadu_pd(sp0 + 4 + src_stride);
			xb3 = _mm_loadu_pd(sp0 + 6 + src_stride);

			rdx = _mm_sub_pd(x_one, ddx);

			MUL_S_D64xD64(xa0, rdx, va0);
			MUL_S_D64xD64(xa1, rdx, va1);
			MUL_S_D64xD64(xa2, ddx, va2);
			MUL_S_D64xD64(xa3, ddx, va3);
			MUL_S_D64xD64(xb0, rdx, vb0);
			MUL_S_D64xD64(xb1, rdx, vb1);
			MUL_S_D64xD64(xb2, ddx, vb2);
			MUL_S_D64xD64(xb3, ddx, vb3);

			ya0 = _mm_add_pd(va0, va2);
			ya1 = _mm_add_pd(va1, va3);
			ya2 = _mm_add_pd(vb0, vb2);
			ya3 = _mm_add_pd(vb1, vb3);

			MUL_S_D64xD64(rdy, ya0, sa0);
			MUL_S_D64xD64(ddy, ya2, sa1);
			MUL_S_D64xD64(rdy, ya1, sa2);
			MUL_S_D64xD64(ddy, ya3, sa3);

			va0 = _mm_add_pd(sa0, sa1);
			va1 = _mm_add_pd(sa2, sa3);
			_mm_storeu_pd(tdp, va0);
			_mm_storeu_pd((tdp + 2), va1);
			tdp += 4;
			xd += dx;
			ddx = _mm_set1_pd((xd - (mlib_s32)xd));
		}

		y_step = (mlib_s32)(yd + dy) - y;
		dp += dst_stride;
		yd += dy;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
