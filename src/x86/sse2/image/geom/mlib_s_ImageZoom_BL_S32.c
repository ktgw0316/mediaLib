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

#pragma ident	"@(#)mlib_s_ImageZoom_BL_S32.c	9.2	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_ImageZoomTranslate(), internal pixels.
 *
 */

#include <mlib_s_ImageZoom.h>

/* *********************************************************** */

#define	XOR32(x)	_mm_xor_si128(x, mask_80000000)

/* *********************************************************** */

#define	MUL_S_U32xU32_2X(S0, S1, D0, D1)         \
	D0 = _mm_mul_epu32(S0, S1);              \
	D1 = _mm_mul_epu32(                      \
	    _mm_srli_epi64(S0, 32),              \
	    _mm_srli_epi64(S1, 32))

#define	MUL_S_U32xU32_2Y(S0, S1, D0)             \
	D0 = _mm_mul_epu32(S0, S1)

/* *********************************************************** */

#undef	BUFF_SIZE
#define	BUFF_SIZE	5120

/* *********************************************************** */

mlib_status
mlib_ImageZoomBilinear_S32_1(
    mlib_work_image * param)
{
	VARIABLE(mlib_s32)
	__m128i buff_arr[BUFF_SIZE / 16], *buff0, *buff1, *buffz;
	void *buff = buff_arr;
	mlib_s32 *tdp, *pbuff;
	mlib_s32 k, width4, buffsize, size, y_step, next_step = 2;
	__m128i ddx_start, ddx, ddy, rdy, dx_step;
	__m128i mask_ffffffff, mask_80000000;

	width4 = width / 4;
	buffsize = (width + 3) / 4;

	size = 5 * buffsize * sizeof (__m128i);
	if (size > BUFF_SIZE) {
		buff = __mlib_malloc(size);
		if (buff == NULL)
			return (MLIB_FAILURE);
	}

	buff0 = buff;
	buff1 = buff0 + 2 * buffsize;
	buffz = buff1 + 2 * buffsize;

/* calculate dx for all row */
	x = GetElemSubStruct(current, srcX) & MLIB_MASK;
	for (i = 0; i < 4; i++) {
		((mlib_s32 *)&ddx_start)[i] = (x << 16);
		x += dx;
	}
	mlib_s32 dt = ((4 * dx) << 16);
	dx_step = _mm_set1_epi32(dt);

	mask_ffffffff = _mm_cmpeq_epi32(ddx_start, ddx_start);
	mask_80000000 = _mm_slli_epi32(mask_ffffffff, 31);

	for (j = 0; j < height; j++) {
		tdp = dp;

		y_step = next_step;
		next_step = ((y + dy) >> MLIB_SHIFT) - (y >> MLIB_SHIFT);

		if (y_step == 0) {

			dt = y << 16;
			ddy = _mm_set1_epi32(dt);
			rdy = _mm_sub_epi32(mask_ffffffff, ddy);

			for (i = 0; i < width4; i++) {
				__m128i x0, x1, y0, y1, s0, s1, t0, t1;
				x0 = buff0[2 * i];
				y0 = buff0[2 * i + 1];
				x1 = buff1[2 * i];
				y1 = buff1[2 * i + 1];
				MUL_S_U32xU32_2Y(rdy, x0, s0);
				MUL_S_U32xU32_2Y(ddy, x1, s1);
				MUL_S_U32xU32_2Y(rdy, y0, t0);
				MUL_S_U32xU32_2Y(ddy, y1, t1);
				x0 = _mm_add_epi64(s0, s1);
				x1 = _mm_add_epi64(t0, t1);
				y0 = _mm_unpacklo_epi32(x0, x1);
				y1 = _mm_unpackhi_epi32(x0, x1);
				t0 = _mm_unpackhi_epi64(y0, y1);
				t1 = XOR32(t0);
				_mm_storeu_si128((void *)tdp, t1);
				tdp += 4;
			}

			if (i * 4 < width) {
				__m128i x0, x1, y0, y1, s0, s1, t0, t1;
				x0 = buff0[2 * i];
				y0 = buff0[2 * i + 1];
				x1 = buff1[2 * i];
				y1 = buff1[2 * i + 1];
				MUL_S_U32xU32_2Y(rdy, x0, s0);
				MUL_S_U32xU32_2Y(ddy, x1, s1);
				MUL_S_U32xU32_2Y(rdy, y0, t0);
				MUL_S_U32xU32_2Y(ddy, y1, t1);
				x0 = _mm_add_epi64(s0, s1);
				x1 = _mm_add_epi64(t0, t1);
				y0 = _mm_unpacklo_epi32(x0, x1);
				y1 = _mm_unpackhi_epi32(x0, x1);
				t0 = _mm_unpackhi_epi64(y0, y1);
				t1 = XOR32(t0);
				__m128i maskt[3];
				maskt[0] = _mm_srli_si128(mask_ffffffff, 12);
				maskt[1] = _mm_srli_si128(mask_ffffffff, 8);
				maskt[2] = _mm_srli_si128(mask_ffffffff, 4);
				dt = width - i * 4 - 1;
				s0 = _mm_loadu_si128((void *)tdp);
				t0 = _mm_or_si128(_mm_and_si128(maskt[dt], t1),
				    _mm_andnot_si128(maskt[dt], s0));
				_mm_storeu_si128((void *)tdp, t0);
				tdp += dt;
			}

		} else if (y_step == 1) {
			__m128i *buffT;
			__m128i ddx, rdx;

			ddx = ddx_start;
			dt = y << 16;
			ddy = _mm_set1_epi32(dt);
			rdy = _mm_sub_epi32(mask_ffffffff, ddy);
			x = GetElemSubStruct(current, srcX) & MLIB_MASK;

			buffT = buff0;
			buff0 = buff1;
			buff1 = buffT;

			for (i = 0; i < width4; i++) {
				mlib_s32 *tsp2 = tsp + src_stride;
				mlib_s32 *sp0 = tsp2 + (x >> MLIB_SHIFT);
				mlib_s32 *sp1 =
				    tsp2 + ((x + dx) >> MLIB_SHIFT);
				mlib_s32 *sp2 =
				    tsp2 + ((x + 2 * dx) >> MLIB_SHIFT);
				mlib_s32 *sp3 =
				    tsp2 + ((x + 3 * dx) >> MLIB_SHIFT);

				__m128i v2, v3, w2, w3, y0, y1;
				__m128i x0, x1, x2, x3;
				__m128i s2, s3, t0, t1;

				x0 = _mm_loadl_epi64((void *)sp0);
				x1 = _mm_loadl_epi64((void *)sp1);
				x2 = _mm_loadl_epi64((void *)sp2);
				x3 = _mm_loadl_epi64((void *)sp3);
				t0 = _mm_unpacklo_epi32(x0, x1);
				t1 = _mm_unpacklo_epi32(x2, x3);
				s2 = _mm_unpacklo_epi64(t0, t1);
				s3 = _mm_unpackhi_epi64(t0, t1);

				rdx = _mm_sub_epi32(mask_ffffffff, ddx);
				MUL_S_U32xU32_2X(XOR32(s2), rdx, v2, w2);
				MUL_S_U32xU32_2X(XOR32(s3), ddx, v3, w3);

				x0 = buff0[2 * i];
				y0 = buff0[2 * i + 1];
				t0 = _mm_add_epi64(v2, v3);
				t1 = _mm_add_epi64(w2, w3);
				x1 = _mm_srli_epi64(t0, 32);
				y1 = _mm_srli_epi64(t1, 32);

				MUL_S_U32xU32_2Y(rdy, x0, s2);
				MUL_S_U32xU32_2Y(ddy, x1, s3);
				MUL_S_U32xU32_2Y(rdy, y0, t0);
				MUL_S_U32xU32_2Y(ddy, y1, t1);

				buff1[2 * i] = x1;
				buff1[2 * i + 1] = y1;

				v2 = _mm_add_epi64(s2, s3);
				w2 = _mm_add_epi64(t0, t1);
				v3 = _mm_unpacklo_epi32(v2, w2);
				w3 = _mm_unpackhi_epi32(v2, w2);
				t0 = _mm_unpackhi_epi64(v3, w3);
				t1 = XOR32(t0);
				_mm_storeu_si128((void *)tdp, t1);
				tdp += 4;
				x += 4 * dx;
				ddx = _mm_add_epi32(ddx, dx_step);
			}

			if (i * 4 < width) {
				mlib_s32 *tsp2 = tsp + src_stride;
				mlib_s32 *sp0 = tsp2 + (x >> MLIB_SHIFT);
				mlib_s32 *sp1 =
				    tsp2 + ((x + dx) >> MLIB_SHIFT);
				mlib_s32 *sp2 =
				    tsp2 + ((x + 2 * dx) >> MLIB_SHIFT);
				mlib_s32 *sp3 =
				    tsp2 + ((x + 3 * dx) >> MLIB_SHIFT);

				__m128i v2, v3, w2, w3, y0, y1;
				__m128i x0, x1, x2, x3;
				__m128i s2, s3, t0, t1;

				x0 = _mm_loadl_epi64((void *)sp0);
				x1 = _mm_loadl_epi64((void *)sp1);
				x2 = _mm_loadl_epi64((void *)sp2);
				x3 = _mm_loadl_epi64((void *)sp3);
				t0 = _mm_unpacklo_epi32(x0, x1);
				t1 = _mm_unpacklo_epi32(x2, x3);
				s2 = _mm_unpacklo_epi64(t0, t1);
				s3 = _mm_unpackhi_epi64(t0, t1);

				rdx = _mm_sub_epi32(mask_ffffffff, ddx);
				MUL_S_U32xU32_2X(XOR32(s2), rdx, v2, w2);
				MUL_S_U32xU32_2X(XOR32(s3), ddx, v3, w3);

				x0 = buff0[2 * i];
				y0 = buff0[2 * i + 1];
				t0 = _mm_add_epi64(v2, v3);
				t1 = _mm_add_epi64(w2, w3);
				x1 = _mm_srli_epi64(t0, 32);
				y1 = _mm_srli_epi64(t1, 32);

				MUL_S_U32xU32_2Y(rdy, x0, s2);
				MUL_S_U32xU32_2Y(ddy, x1, s3);
				MUL_S_U32xU32_2Y(rdy, y0, t0);
				MUL_S_U32xU32_2Y(ddy, y1, t1);

				buff1[2 * i] = x1;
				buff1[2 * i + 1] = y1;

				v2 = _mm_add_epi64(s2, s3);
				w2 = _mm_add_epi64(t0, t1);
				v3 = _mm_unpacklo_epi32(v2, w2);
				w3 = _mm_unpackhi_epi32(v2, w2);
				t0 = _mm_unpackhi_epi64(v3, w3);
				t1 = XOR32(t0);
				__m128i maskt[3];
				maskt[0] = _mm_srli_si128(mask_ffffffff, 12);
				maskt[1] = _mm_srli_si128(mask_ffffffff, 8);
				maskt[2] = _mm_srli_si128(mask_ffffffff, 4);
				dt = width - i * 4 - 1;
				s2 = _mm_loadu_si128((void *)tdp);
				t0 = _mm_or_si128(_mm_and_si128(maskt[dt], t1),
				    _mm_andnot_si128(maskt[dt], s2));
				_mm_storeu_si128((void *)tdp, t0);
				tdp += dt;
			}

		} else {
			__m128i ddx, rdx;

			ddx = ddx_start;
			dt = y << 16;
			ddy = _mm_set1_epi32(dt);
			rdy = _mm_sub_epi32(mask_ffffffff, ddy);
			x = GetElemSubStruct(current, srcX) & MLIB_MASK;

			for (i = 0; i < width4; i++) {
				mlib_s32 *sp0 = tsp + (x >> MLIB_SHIFT);
				mlib_s32 *sp1 =
				    tsp + ((x + dx) >> MLIB_SHIFT);
				mlib_s32 *sp2 =
				    tsp + ((x + 2 * dx) >> MLIB_SHIFT);
				mlib_s32 *sp3 =
				    tsp + ((x + 3 * dx) >> MLIB_SHIFT);

				__m128i x0, x1, x2, x3, y0, y1, y2, y3;
				__m128i v0, v1, v2, v3, w0, w1, w2, w3;
				__m128i s0, s1, s2, s3;

				x0 = _mm_loadl_epi64((void *)sp0);
				x1 = _mm_loadl_epi64((void *)sp1);
				x2 = _mm_loadl_epi64((void *)sp2);
				x3 = _mm_loadl_epi64((void *)sp3);
				v0 = _mm_unpacklo_epi32(x0, x1);
				v1 = _mm_unpacklo_epi32(x2, x3);
				s0 = _mm_unpacklo_epi64(v0, v1);
				s1 = _mm_unpackhi_epi64(v0, v1);

				x0 = _mm_loadl_epi64((void *)(sp0 +
				    src_stride));
				x1 = _mm_loadl_epi64((void *)(sp1 +
				    src_stride));
				x2 = _mm_loadl_epi64((void *)(sp2 +
				    src_stride));
				x3 = _mm_loadl_epi64((void *)(sp3 +
				    src_stride));
				w0 = _mm_unpacklo_epi32(x0, x1);
				w1 = _mm_unpacklo_epi32(x2, x3);
				s2 = _mm_unpacklo_epi64(w0, w1);
				s3 = _mm_unpackhi_epi64(w0, w1);

				rdx = _mm_sub_epi32(mask_ffffffff, ddx);
				MUL_S_U32xU32_2X(XOR32(s0), rdx, v0, w0);
				MUL_S_U32xU32_2X(XOR32(s1), ddx, v1, w1);
				MUL_S_U32xU32_2X(XOR32(s2), rdx, v2, w2);
				MUL_S_U32xU32_2X(XOR32(s3), ddx, v3, w3);

				x0 = _mm_add_epi64(v0, v1);
				x1 = _mm_add_epi64(w0, w1);
				x2 = _mm_add_epi64(v2, v3);
				x3 = _mm_add_epi64(w2, w3);

				y0 = _mm_srli_epi64(x0, 32);
				y1 = _mm_srli_epi64(x1, 32);
				y2 = _mm_srli_epi64(x2, 32);
				y3 = _mm_srli_epi64(x3, 32);

				MUL_S_U32xU32_2Y(rdy, y0, s0);
				MUL_S_U32xU32_2Y(ddy, y2, s1);
				MUL_S_U32xU32_2Y(rdy, y1, s2);
				MUL_S_U32xU32_2Y(ddy, y3, s3);

				buff0[2 * i] = y0;
				buff0[2 * i + 1] = y1;
				buff1[2 * i] = y2;
				buff1[2 * i + 1] = y3;

				v2 = _mm_add_epi64(s0, s1);
				w2 = _mm_add_epi64(s2, s3);
				v3 = _mm_unpacklo_epi32(v2, w2);
				w3 = _mm_unpackhi_epi32(v2, w2);
				s0 = _mm_unpackhi_epi64(v3, w3);
				s1 = XOR32(s0);
				_mm_storeu_si128((void *)tdp, s1);
				tdp += 4;
				x += 4 * dx;
				ddx = _mm_add_epi32(ddx, dx_step);
			}

			if (i * 4 < width) {
				mlib_s32 *sp0 = tsp + (x >> MLIB_SHIFT);
				mlib_s32 *sp1 =
				    tsp + ((x + dx) >> MLIB_SHIFT);
				mlib_s32 *sp2 =
				    tsp + ((x + 2 * dx) >> MLIB_SHIFT);
				mlib_s32 *sp3 =
				    tsp + ((x + 3 * dx) >> MLIB_SHIFT);

				__m128i x0, x1, x2, x3, y0, y1, y2, y3;
				__m128i v0, v1, v2, v3, w0, w1, w2, w3;
				__m128i s0, s1, s2, s3;

				x0 = _mm_loadl_epi64((void *)sp0);
				x1 = _mm_loadl_epi64((void *)sp1);
				x2 = _mm_loadl_epi64((void *)sp2);
				x3 = _mm_loadl_epi64((void *)sp3);
				v0 = _mm_unpacklo_epi32(x0, x1);
				v1 = _mm_unpacklo_epi32(x2, x3);
				s0 = _mm_unpacklo_epi64(v0, v1);
				s1 = _mm_unpackhi_epi64(v0, v1);

				x0 = _mm_loadl_epi64((void *)(sp0 +
				    src_stride));
				x1 = _mm_loadl_epi64((void *)(sp1 +
				    src_stride));
				x2 = _mm_loadl_epi64((void *)(sp2 +
				    src_stride));
				x3 = _mm_loadl_epi64((void *)(sp3 +
				    src_stride));
				w0 = _mm_unpacklo_epi32(x0, x1);
				w1 = _mm_unpacklo_epi32(x2, x3);
				s2 = _mm_unpacklo_epi64(w0, w1);
				s3 = _mm_unpackhi_epi64(w0, w1);

				rdx = _mm_sub_epi32(mask_ffffffff, ddx);
				MUL_S_U32xU32_2X(XOR32(s0), rdx, v0, w0);
				MUL_S_U32xU32_2X(XOR32(s1), ddx, v1, w1);
				MUL_S_U32xU32_2X(XOR32(s2), rdx, v2, w2);
				MUL_S_U32xU32_2X(XOR32(s3), ddx, v3, w3);

				x0 = _mm_add_epi64(v0, v1);
				x1 = _mm_add_epi64(w0, w1);
				x2 = _mm_add_epi64(v2, v3);
				x3 = _mm_add_epi64(w2, w3);

				y0 = _mm_srli_epi64(x0, 32);
				y1 = _mm_srli_epi64(x1, 32);
				y2 = _mm_srli_epi64(x2, 32);
				y3 = _mm_srli_epi64(x3, 32);

				MUL_S_U32xU32_2Y(rdy, y0, s0);
				MUL_S_U32xU32_2Y(ddy, y2, s1);
				MUL_S_U32xU32_2Y(rdy, y1, s2);
				MUL_S_U32xU32_2Y(ddy, y3, s3);

				buff0[2 * i] = y0;
				buff0[2 * i + 1] = y1;
				buff1[2 * i] = y2;
				buff1[2 * i + 1] = y3;

				v2 = _mm_add_epi64(s0, s1);
				w2 = _mm_add_epi64(s2, s3);
				v3 = _mm_unpacklo_epi32(v2, w2);
				w3 = _mm_unpackhi_epi32(v2, w2);
				s0 = _mm_unpackhi_epi64(v3, w3);
				s1 = XOR32(s0);

				__m128i maskt[3];
				maskt[0] = _mm_srli_si128(mask_ffffffff, 12);
				maskt[1] = _mm_srli_si128(mask_ffffffff, 8);
				maskt[2] = _mm_srli_si128(mask_ffffffff, 4);
				dt = width - i * 4 - 1;
				s2 = _mm_loadu_si128((void *)tdp);
				s3 = _mm_or_si128(_mm_and_si128(maskt[dt], s1),
				    _mm_andnot_si128(maskt[dt], s2));
				_mm_storeu_si128((void *)tdp, s3);
				tdp += dt;
			}
		}

		dp += dst_stride;
		y += dy;
		tsp = sp + (y >> MLIB_SHIFT) * src_stride;
	}

	if (size > BUFF_SIZE) {
		__mlib_free(buff);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_ImageZoomBilinear_S32_1s(
    mlib_work_image * param)
{
	VARIABLE(mlib_s32)
	mlib_s32 *tdp;
	mlib_s32 k, width4;
	__m128i ddx_start, ddx, ddy, rdy, dx_step;
	__m128i mask_ffffffff, mask_80000000;

	width4 = width / 4;

/* calculate dx for all row */
	x = GetElemSubStruct(current, srcX) & MLIB_MASK;
	for (i = 0; i < 4; i++) {
		((mlib_s32 *)&ddx_start)[i] = (x << 16);
		x += dx;
	}
	mlib_s32 dt = ((4 * dx) << 16);
	dx_step = _mm_set1_epi32(dt);

	mask_ffffffff = _mm_cmpeq_epi32(ddx_start, ddx_start);
	mask_80000000 = _mm_slli_epi32(mask_ffffffff, 31);

	for (j = 0; j < height; j++) {
		__m128i ddx, rdx;
		tdp = dp;

		ddx = ddx_start;
		dt = y << 16;
		ddy = _mm_set1_epi32(dt);
		rdy = _mm_sub_epi32(mask_ffffffff, ddy);
			x = GetElemSubStruct(current, srcX) & MLIB_MASK;

		for (i = 0; i < width4; i++) {
			mlib_s32 *sp0 = tsp + (x >> MLIB_SHIFT);
			mlib_s32 *sp1 =
			    tsp + ((x + dx) >> MLIB_SHIFT);
			mlib_s32 *sp2 =
			    tsp + ((x + 2 * dx) >> MLIB_SHIFT);
			mlib_s32 *sp3 =
			    tsp + ((x + 3 * dx) >> MLIB_SHIFT);

			__m128i x0, x1, x2, x3, y0, y1, y2, y3;
			__m128i v0, v1, v2, v3, w0, w1, w2, w3;
			__m128i s0, s1, s2, s3;

			x0 = _mm_loadl_epi64((void *)sp0);
			x1 = _mm_loadl_epi64((void *)sp1);
			x2 = _mm_loadl_epi64((void *)sp2);
			x3 = _mm_loadl_epi64((void *)sp3);
			v0 = _mm_unpacklo_epi32(x0, x1);
			v1 = _mm_unpacklo_epi32(x2, x3);
			s0 = _mm_unpacklo_epi64(v0, v1);
			s1 = _mm_unpackhi_epi64(v0, v1);

			x0 = _mm_loadl_epi64((void *)(sp0 +
			    src_stride));
			x1 = _mm_loadl_epi64((void *)(sp1 +
			    src_stride));
			x2 = _mm_loadl_epi64((void *)(sp2 +
			    src_stride));
			x3 = _mm_loadl_epi64((void *)(sp3 +
			    src_stride));
			w0 = _mm_unpacklo_epi32(x0, x1);
			w1 = _mm_unpacklo_epi32(x2, x3);
			s2 = _mm_unpacklo_epi64(w0, w1);
			s3 = _mm_unpackhi_epi64(w0, w1);

			rdx = _mm_sub_epi32(mask_ffffffff, ddx);
			MUL_S_U32xU32_2X(XOR32(s0), rdx, v0, w0);
			MUL_S_U32xU32_2X(XOR32(s1), ddx, v1, w1);
			MUL_S_U32xU32_2X(XOR32(s2), rdx, v2, w2);
			MUL_S_U32xU32_2X(XOR32(s3), ddx, v3, w3);

			x0 = _mm_add_epi64(v0, v1);
			x1 = _mm_add_epi64(w0, w1);
			x2 = _mm_add_epi64(v2, v3);
			x3 = _mm_add_epi64(w2, w3);

			y0 = _mm_srli_epi64(x0, 32);
			y1 = _mm_srli_epi64(x1, 32);
			y2 = _mm_srli_epi64(x2, 32);
			y3 = _mm_srli_epi64(x3, 32);

			MUL_S_U32xU32_2Y(rdy, y0, s0);
			MUL_S_U32xU32_2Y(ddy, y2, s1);
			MUL_S_U32xU32_2Y(rdy, y1, s2);
			MUL_S_U32xU32_2Y(ddy, y3, s3);

			v2 = _mm_add_epi64(s0, s1);
			w2 = _mm_add_epi64(s2, s3);
			v3 = _mm_unpacklo_epi32(v2, w2);
			w3 = _mm_unpackhi_epi32(v2, w2);
			s0 = _mm_unpackhi_epi64(v3, w3);
			s1 = XOR32(s0);
			_mm_storeu_si128((void *)tdp, s1);
			tdp += 4;
			x += 4 * dx;
			ddx = _mm_add_epi32(ddx, dx_step);
		}

		if (i * 4 < width) {
			mlib_s32 *sp0 = tsp + (x >> MLIB_SHIFT);
			mlib_s32 *sp1 =
			    tsp + ((x + dx) >> MLIB_SHIFT);
			mlib_s32 *sp2 =
			    tsp + ((x + 2 * dx) >> MLIB_SHIFT);
			mlib_s32 *sp3 =
			    tsp + ((x + 3 * dx) >> MLIB_SHIFT);

			__m128i x0, x1, x2, x3, y0, y1, y2, y3;
			__m128i v0, v1, v2, v3, w0, w1, w2, w3;
			__m128i s0, s1, s2, s3;

			x0 = _mm_loadl_epi64((void *)sp0);
			x1 = _mm_loadl_epi64((void *)sp1);
			x2 = _mm_loadl_epi64((void *)sp2);
			x3 = _mm_loadl_epi64((void *)sp3);
			v0 = _mm_unpacklo_epi32(x0, x1);
			v1 = _mm_unpacklo_epi32(x2, x3);
			s0 = _mm_unpacklo_epi64(v0, v1);
			s1 = _mm_unpackhi_epi64(v0, v1);

			x0 = _mm_loadl_epi64((void *)(sp0 +
			    src_stride));
			x1 = _mm_loadl_epi64((void *)(sp1 +
			    src_stride));
			x2 = _mm_loadl_epi64((void *)(sp2 +
			    src_stride));
			x3 = _mm_loadl_epi64((void *)(sp3 +
			    src_stride));
			w0 = _mm_unpacklo_epi32(x0, x1);
				w1 = _mm_unpacklo_epi32(x2, x3);
			s2 = _mm_unpacklo_epi64(w0, w1);
			s3 = _mm_unpackhi_epi64(w0, w1);

			rdx = _mm_sub_epi32(mask_ffffffff, ddx);
			MUL_S_U32xU32_2X(XOR32(s0), rdx, v0, w0);
			MUL_S_U32xU32_2X(XOR32(s1), ddx, v1, w1);
			MUL_S_U32xU32_2X(XOR32(s2), rdx, v2, w2);
			MUL_S_U32xU32_2X(XOR32(s3), ddx, v3, w3);

			x0 = _mm_add_epi64(v0, v1);
			x1 = _mm_add_epi64(w0, w1);
			x2 = _mm_add_epi64(v2, v3);
			x3 = _mm_add_epi64(w2, w3);

			y0 = _mm_srli_epi64(x0, 32);
			y1 = _mm_srli_epi64(x1, 32);
			y2 = _mm_srli_epi64(x2, 32);
			y3 = _mm_srli_epi64(x3, 32);

			MUL_S_U32xU32_2Y(rdy, y0, s0);
			MUL_S_U32xU32_2Y(ddy, y2, s1);
			MUL_S_U32xU32_2Y(rdy, y1, s2);
			MUL_S_U32xU32_2Y(ddy, y3, s3);

			v2 = _mm_add_epi64(s0, s1);
			w2 = _mm_add_epi64(s2, s3);
			v3 = _mm_unpacklo_epi32(v2, w2);
			w3 = _mm_unpackhi_epi32(v2, w2);
			s0 = _mm_unpackhi_epi64(v3, w3);
			s1 = XOR32(s0);

			__m128i maskt[3];
			maskt[0] = _mm_srli_si128(mask_ffffffff, 12);
			maskt[1] = _mm_srli_si128(mask_ffffffff, 8);
			maskt[2] = _mm_srli_si128(mask_ffffffff, 4);
			dt = width - i * 4 - 1;
			s2 = _mm_loadu_si128((void *)tdp);
			s3 = _mm_or_si128(_mm_and_si128(maskt[dt], s1),
			    _mm_andnot_si128(maskt[dt], s2));
			_mm_storeu_si128((void *)tdp, s3);
			tdp += dt;
		}

		dp += dst_stride;
		y += dy;
		tsp = sp + (y >> MLIB_SHIFT) * src_stride;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#undef	BUFF_SIZE
#define	BUFF_SIZE	10240

/* *********************************************************** */

mlib_status
mlib_ImageZoomBilinear_S32_2(
    mlib_work_image * param)
{
	VARIABLE(mlib_s32)
	__m128i buff_arr[BUFF_SIZE / 16], *buff0, *buff1, *buffz;
	void *buff = buff_arr;
	mlib_s32 *tdp, *pbuff;
	mlib_s32 k, width4, buffsize, size, dt, y_step, next_step = 2;
	__m128i ddx_start, dx_step;
	__m128i mask_ffffffff, mask_80000000;
	width *= 2;
	width4 = width / 4;
	buffsize = (width + 3) / 4;

	size = 5 * buffsize * sizeof (__m128i);
	if (size > BUFF_SIZE) {
		buff = __mlib_malloc(size);
		if (buff == NULL)
			return (MLIB_FAILURE);
	}

	buff0 = buff;
	buff1 = buff0 + 2 * buffsize;
	buffz = buff1 + 2 * buffsize;

/* calculate dx for all row */
	x = GetElemSubStruct(current, srcX) & MLIB_MASK;
	for (i = 0; i < 2; i++) {
		dt = x << 16;
		((mlib_s32 *)&ddx_start)[2 * i] = dt;
		((mlib_s32 *)&ddx_start)[2 * i + 1] = dt;
		x += dx;
	}

	mask_ffffffff = _mm_cmpeq_epi32(ddx_start, ddx_start);
	mask_80000000 = _mm_slli_epi32(mask_ffffffff, 31);

	dt = (2 * dx) << 16;
	dx_step = _mm_set1_epi32(dt);

	for (j = 0; j < height; j++) {
		tdp = dp;

		y_step = next_step;
		next_step = ((y + dy) >> MLIB_SHIFT) - (y >> MLIB_SHIFT);

		if (y_step == 0) {
			__m128i ddx, ddy, rdy;
			dt = y << 16;
			ddy = _mm_set1_epi32(dt);
			rdy = _mm_sub_epi32(mask_ffffffff, ddy);

			for (i = 0; i < width4; i++) {
				__m128i y0, y1, y2, y3;
				__m128i s0, s1, s2, s3, t0, t1, t2, t3;

				y0 = buff0[2 * i];
				y1 = buff0[2 * i + 1];
				y2 = buff1[2 * i];
				y3 = buff1[2 * i + 1];
				MUL_S_U32xU32_2Y(rdy, y0, s0);
				MUL_S_U32xU32_2Y(ddy, y2, s1);
				MUL_S_U32xU32_2Y(rdy, y1, s2);
				MUL_S_U32xU32_2Y(ddy, y3, s3);
				t0 = _mm_add_epi64(s0, s1);
				t1 = _mm_add_epi64(s2, s3);
				t2 = _mm_unpacklo_epi32(t0, t1);
				t3 = _mm_unpackhi_epi32(t0, t1);
				s0 = _mm_unpackhi_epi64(t2, t3);
				s1 = XOR32(s0);
				_mm_storeu_si128((void *)tdp, s1);
				tdp += 4;
			}

			if (i * 4 < width) {
				__m128i y0, y1, y2, y3;
				__m128i s0, s1, s2, s3, t0, t1, t2, t3;

				y0 = buff0[2 * i];
				y1 = buff0[2 * i + 1];
				y2 = buff1[2 * i];
				y3 = buff1[2 * i + 1];
				MUL_S_U32xU32_2Y(rdy, y0, s0);
				MUL_S_U32xU32_2Y(ddy, y2, s1);
				MUL_S_U32xU32_2Y(rdy, y1, s2);
				MUL_S_U32xU32_2Y(ddy, y3, s3);
				t0 = _mm_add_epi64(s0, s1);
				t1 = _mm_add_epi64(s2, s3);
				t2 = _mm_unpacklo_epi32(t0, t1);
				t3 = _mm_unpackhi_epi32(t0, t1);
				s0 = _mm_unpackhi_epi64(t2, t3);
				s1 = XOR32(s0);
				_mm_storel_epi64((void *)tdp, s1);
				tdp += 2;
			}

		} else if (y_step == 1) {
			__m128i *buffT;
			__m128i ddx, rdx;
			__m128i ddy, rdy;

			ddx = ddx_start;
			dt = y << 16;
			ddy = _mm_set1_epi32(dt);
			rdy = _mm_sub_epi32(mask_ffffffff, ddy);
			x = GetElemSubStruct(current, srcX) & MLIB_MASK;

			buffT = buff0;
			buff0 = buff1;
			buff1 = buffT;

			for (i = 0; i < width4; i++) {
				__m128i x0, x1, y0, y1, y2, y3;
				__m128i v2, v3, w2, w3;
				__m128i s0, s1, s2, s3, t0, t1;

				mlib_s32 *tsp2 = tsp + src_stride;
				mlib_s32 *sp0 = tsp2 + 2 * (x >> MLIB_SHIFT);
				mlib_s32 *sp1 =
				    tsp2 + 2 * ((x + dx) >> MLIB_SHIFT);

				x0 = _mm_loadu_si128((void *)sp0);
				x1 = _mm_loadu_si128((void *)sp1);
				t0 = _mm_unpacklo_epi64(x0, x1);
				t1 = _mm_unpackhi_epi64(x0, x1);

				rdx = _mm_sub_epi32(mask_ffffffff, ddx);
				MUL_S_U32xU32_2X(XOR32(t0), rdx, v2, w2);
				MUL_S_U32xU32_2X(XOR32(t1), ddx, v3, w3);

				y0 = buff0[2 * i];
				y1 = buff0[2 * i + 1];
				t0 = _mm_add_epi64(v2, v3);
				t1 = _mm_add_epi64(w2, w3);
				y2 = _mm_srli_epi64(t0, 32);
				y3 = _mm_srli_epi64(t1, 32);

				MUL_S_U32xU32_2Y(rdy, y0, s0);
				MUL_S_U32xU32_2Y(ddy, y2, s1);
				MUL_S_U32xU32_2Y(rdy, y1, s2);
				MUL_S_U32xU32_2Y(ddy, y3, s3);

				buff1[2 * i] = y2;
				buff1[2 * i + 1] = y3;

				v2 = _mm_add_epi64(s0, s1);
				w2 = _mm_add_epi64(s2, s3);
				v3 = _mm_unpacklo_epi32(v2, w2);
				w3 = _mm_unpackhi_epi32(v2, w2);
				s0 = _mm_unpackhi_epi64(v3, w3);
				s1 = XOR32(s0);
				_mm_storeu_si128((void *)tdp, s1);
				tdp += 4;

				x += 2 * dx;
				ddx = _mm_add_epi32(ddx, dx_step);
			}

			if (i * 4 < width) {
				__m128i x0, x1, y0, y1, y2, y3;
				__m128i v2, v3, w2, w3;
				__m128i s0, s1, s2, s3, t0, t1;

				mlib_s32 *tsp2 = tsp + src_stride;
				mlib_s32 *sp0 = tsp2 + 2 * (x >> MLIB_SHIFT);
				mlib_s32 *sp1 =
				    tsp2 + 2 * ((x + dx) >> MLIB_SHIFT);

				x0 = _mm_loadu_si128((void *)sp0);
				x1 = _mm_loadu_si128((void *)sp1);
				t0 = _mm_unpacklo_epi64(x0, x1);
				t1 = _mm_unpackhi_epi64(x0, x1);

				rdx = _mm_sub_epi32(mask_ffffffff, ddx);
				MUL_S_U32xU32_2X(XOR32(t0), rdx, v2, w2);
				MUL_S_U32xU32_2X(XOR32(t1), ddx, v3, w3);

				y0 = buff0[2 * i];
				y1 = buff0[2 * i + 1];
				t0 = _mm_add_epi64(v2, v3);
				t1 = _mm_add_epi64(w2, w3);
				y2 = _mm_srli_epi64(t0, 32);
				y3 = _mm_srli_epi64(t1, 32);

				MUL_S_U32xU32_2Y(rdy, y0, s0);
				MUL_S_U32xU32_2Y(ddy, y2, s1);
				MUL_S_U32xU32_2Y(rdy, y1, s2);
				MUL_S_U32xU32_2Y(ddy, y3, s3);

				buff1[2 * i] = y2;
				buff1[2 * i + 1] = y3;

				v2 = _mm_add_epi64(s0, s1);
				w2 = _mm_add_epi64(s2, s3);
				v3 = _mm_unpacklo_epi32(v2, w2);
				w3 = _mm_unpackhi_epi32(v2, w2);
				s0 = _mm_unpackhi_epi64(v3, w3);
				s1 = XOR32(s0);
				_mm_storel_epi64((void *)tdp, s1);
				tdp += 2;

				x += 2 * dx;
				ddx = _mm_add_epi32(ddx, dx_step);
			}

		} else {
			__m128i ddx, rdx;
			__m128i ddy, rdy;

			ddx = ddx_start;
			dt = y << 16;
			ddy = _mm_set1_epi32(dt);
			rdy = _mm_sub_epi32(mask_ffffffff, ddy);
			x = GetElemSubStruct(current, srcX) & MLIB_MASK;

			for (i = 0; i < width4; i++) {
				__m128i x0, x1, x2, x3, y0, y1, y2, y3;
				__m128i v0, v1, v2, v3, w0, w1, w2, w3;
				__m128i s0, s1, s2, s3, t0, t1, t2, t3;

				mlib_s32 *sp0 = tsp + 2 * (x >> MLIB_SHIFT);
				mlib_s32 *sp1 =
				    tsp + 2 * ((x + dx) >> MLIB_SHIFT);

				x0 = _mm_loadu_si128((void *)sp0);
				x1 = _mm_loadu_si128((void *)sp1);
				x2 = _mm_loadu_si128((void *)(sp0 +
				    src_stride));
				x3 = _mm_loadu_si128((void *)(sp1 +
				    src_stride));
				t0 = _mm_unpacklo_epi64(x0, x1);
				t1 = _mm_unpackhi_epi64(x0, x1);
				t2 = _mm_unpacklo_epi64(x2, x3);
				t3 = _mm_unpackhi_epi64(x2, x3);

				rdx = _mm_sub_epi32(mask_ffffffff, ddx);
				MUL_S_U32xU32_2X(XOR32(t0), rdx, v0, w0);
				MUL_S_U32xU32_2X(XOR32(t1), ddx, v1, w1);
				MUL_S_U32xU32_2X(XOR32(t2), rdx, v2, w2);
				MUL_S_U32xU32_2X(XOR32(t3), ddx, v3, w3);

				s0 = _mm_add_epi64(v0, v1);
				s1 = _mm_add_epi64(w0, w1);
				s2 = _mm_add_epi64(v2, v3);
				s3 = _mm_add_epi64(w2, w3);

				y0 = _mm_srli_epi64(s0, 32);
				y1 = _mm_srli_epi64(s1, 32);
				y2 = _mm_srli_epi64(s2, 32);
				y3 = _mm_srli_epi64(s3, 32);

				MUL_S_U32xU32_2Y(rdy, y0, t0);
				MUL_S_U32xU32_2Y(ddy, y2, t1);
				MUL_S_U32xU32_2Y(rdy, y1, t2);
				MUL_S_U32xU32_2Y(ddy, y3, t3);

				buff0[2 * i] = y0;
				buff0[2 * i + 1] = y1;
				buff1[2 * i] = y2;
				buff1[2 * i + 1] = y3;

				v2 = _mm_add_epi64(t0, t1);
				w2 = _mm_add_epi64(t2, t3);
				v3 = _mm_unpacklo_epi32(v2, w2);
				w3 = _mm_unpackhi_epi32(v2, w2);
				s0 = _mm_unpackhi_epi64(v3, w3);
				s1 = XOR32(s0);
				_mm_storeu_si128((void *)tdp, s1);
				tdp += 4;

				x += 2 * dx;
				ddx = _mm_add_epi32(ddx, dx_step);
			}

			if (i * 4 < width) {
				__m128i x0, x1, x2, x3, y0, y1, y2, y3;
				__m128i v0, v1, v2, v3, w0, w1, w2, w3;
				__m128i s0, s1, s2, s3, t0, t1, t2, t3;

				mlib_s32 *sp0 = tsp + 2 * (x >> MLIB_SHIFT);
				mlib_s32 *sp1 =
				    tsp + 2 * ((x + dx) >> MLIB_SHIFT);

				x0 = _mm_loadu_si128((void *)sp0);
				x1 = _mm_loadu_si128((void *)sp1);
				x2 = _mm_loadu_si128((void *)(sp0 +
				    src_stride));
				x3 = _mm_loadu_si128((void *)(sp1 +
				    src_stride));
				t0 = _mm_unpacklo_epi64(x0, x1);
				t1 = _mm_unpackhi_epi64(x0, x1);
				t2 = _mm_unpacklo_epi64(x2, x3);
				t3 = _mm_unpackhi_epi64(x2, x3);

				rdx = _mm_sub_epi32(mask_ffffffff, ddx);
				MUL_S_U32xU32_2X(XOR32(t0), rdx, v0, w0);
				MUL_S_U32xU32_2X(XOR32(t1), ddx, v1, w1);
				MUL_S_U32xU32_2X(XOR32(t2), rdx, v2, w2);
				MUL_S_U32xU32_2X(XOR32(t3), ddx, v3, w3);

				s0 = _mm_add_epi64(v0, v1);
				s1 = _mm_add_epi64(w0, w1);
				s2 = _mm_add_epi64(v2, v3);
				s3 = _mm_add_epi64(w2, w3);

				y0 = _mm_srli_epi64(s0, 32);
				y1 = _mm_srli_epi64(s1, 32);
				y2 = _mm_srli_epi64(s2, 32);
				y3 = _mm_srli_epi64(s3, 32);

				MUL_S_U32xU32_2Y(rdy, y0, t0);
				MUL_S_U32xU32_2Y(ddy, y2, t1);
				MUL_S_U32xU32_2Y(rdy, y1, t2);
				MUL_S_U32xU32_2Y(ddy, y3, t3);

				buff0[2 * i] = y0;
				buff0[2 * i + 1] = y1;
				buff1[2 * i] = y2;
				buff1[2 * i + 1] = y3;

				v2 = _mm_add_epi64(t0, t1);
				w2 = _mm_add_epi64(t2, t3);
				v3 = _mm_unpacklo_epi32(v2, w2);
				w3 = _mm_unpackhi_epi32(v2, w2);
				s0 = _mm_unpackhi_epi64(v3, w3);
				s1 = XOR32(s0);
				_mm_storel_epi64((void *)tdp, s1);
				tdp += 2;

				x += 2 * dx;
				ddx = _mm_add_epi32(ddx, dx_step);
			}
		}

		dp += dst_stride;
		y += dy;
		tsp = sp + (y >> MLIB_SHIFT) * src_stride;
	}

	if (size > BUFF_SIZE) {
		__mlib_free(buff);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_ImageZoomBilinear_S32_2s(
    mlib_work_image * param)
{
	VARIABLE(mlib_s32)
	mlib_s32 *tdp;
	mlib_s32 k, width4, dt;
	__m128i ddx_start, dx_step;
	__m128i mask_ffffffff, mask_80000000;
	width *= 2;
	width4 = width / 4;

/* calculate dx for all row */
	x = GetElemSubStruct(current, srcX) & MLIB_MASK;
	for (i = 0; i < 2; i++) {
		dt = x << 16;
		((mlib_s32 *)&ddx_start)[2 * i] = dt;
		((mlib_s32 *)&ddx_start)[2 * i + 1] = dt;
		x += dx;
	}

	mask_ffffffff = _mm_cmpeq_epi32(ddx_start, ddx_start);
	mask_80000000 = _mm_slli_epi32(mask_ffffffff, 31);

	dt = (2 * dx) << 16;
	dx_step = _mm_set1_epi32(dt);

	for (j = 0; j < height; j++) {
		tdp = dp;

		__m128i ddx, rdx;
		__m128i ddy, rdy;

		ddx = ddx_start;
		dt = y << 16;
		ddy = _mm_set1_epi32(dt);
		rdy = _mm_sub_epi32(mask_ffffffff, ddy);
		x = GetElemSubStruct(current, srcX) & MLIB_MASK;

		for (i = 0; i < width4; i++) {
			__m128i x0, x1, x2, x3, y0, y1, y2, y3;
			__m128i v0, v1, v2, v3, w0, w1, w2, w3;
			__m128i s0, s1, s2, s3, t0, t1, t2, t3;

			mlib_s32 *sp0 = tsp + 2 * (x >> MLIB_SHIFT);
			mlib_s32 *sp1 =
			    tsp + 2 * ((x + dx) >> MLIB_SHIFT);

			x0 = _mm_loadu_si128((void *)sp0);
			x1 = _mm_loadu_si128((void *)sp1);
			x2 = _mm_loadu_si128((void *)(sp0 +
			    src_stride));
			x3 = _mm_loadu_si128((void *)(sp1 +
			    src_stride));
			t0 = _mm_unpacklo_epi64(x0, x1);
			t1 = _mm_unpackhi_epi64(x0, x1);
			t2 = _mm_unpacklo_epi64(x2, x3);
			t3 = _mm_unpackhi_epi64(x2, x3);

			rdx = _mm_sub_epi32(mask_ffffffff, ddx);
			MUL_S_U32xU32_2X(XOR32(t0), rdx, v0, w0);
			MUL_S_U32xU32_2X(XOR32(t1), ddx, v1, w1);
			MUL_S_U32xU32_2X(XOR32(t2), rdx, v2, w2);
			MUL_S_U32xU32_2X(XOR32(t3), ddx, v3, w3);

			s0 = _mm_add_epi64(v0, v1);
			s1 = _mm_add_epi64(w0, w1);
			s2 = _mm_add_epi64(v2, v3);
			s3 = _mm_add_epi64(w2, w3);

			y0 = _mm_srli_epi64(s0, 32);
			y1 = _mm_srli_epi64(s1, 32);
			y2 = _mm_srli_epi64(s2, 32);
			y3 = _mm_srli_epi64(s3, 32);

			MUL_S_U32xU32_2Y(rdy, y0, t0);
			MUL_S_U32xU32_2Y(ddy, y2, t1);
			MUL_S_U32xU32_2Y(rdy, y1, t2);
			MUL_S_U32xU32_2Y(ddy, y3, t3);

			v2 = _mm_add_epi64(t0, t1);
			w2 = _mm_add_epi64(t2, t3);
			v3 = _mm_unpacklo_epi32(v2, w2);
			w3 = _mm_unpackhi_epi32(v2, w2);
			s0 = _mm_unpackhi_epi64(v3, w3);
			s1 = XOR32(s0);
			_mm_storeu_si128((void *)tdp, s1);
			tdp += 4;

			x += 2 * dx;
			ddx = _mm_add_epi32(ddx, dx_step);
		}

		if (i * 4 < width) {
			__m128i x0, x1, x2, x3, y0, y1, y2, y3;
			__m128i v0, v1, v2, v3, w0, w1, w2, w3;
			__m128i s0, s1, s2, s3, t0, t1, t2, t3;

			mlib_s32 *sp0 = tsp + 2 * (x >> MLIB_SHIFT);
			mlib_s32 *sp1 =
			    tsp + 2 * ((x + dx) >> MLIB_SHIFT);

			x0 = _mm_loadu_si128((void *)sp0);
			x1 = _mm_loadu_si128((void *)sp1);
			x2 = _mm_loadu_si128((void *)(sp0 +
			    src_stride));
			x3 = _mm_loadu_si128((void *)(sp1 +
			    src_stride));
			t0 = _mm_unpacklo_epi64(x0, x1);
			t1 = _mm_unpackhi_epi64(x0, x1);
			t2 = _mm_unpacklo_epi64(x2, x3);
			t3 = _mm_unpackhi_epi64(x2, x3);

			rdx = _mm_sub_epi32(mask_ffffffff, ddx);
			MUL_S_U32xU32_2X(XOR32(t0), rdx, v0, w0);
			MUL_S_U32xU32_2X(XOR32(t1), ddx, v1, w1);
			MUL_S_U32xU32_2X(XOR32(t2), rdx, v2, w2);
			MUL_S_U32xU32_2X(XOR32(t3), ddx, v3, w3);

			s0 = _mm_add_epi64(v0, v1);
			s1 = _mm_add_epi64(w0, w1);
			s2 = _mm_add_epi64(v2, v3);
			s3 = _mm_add_epi64(w2, w3);

			y0 = _mm_srli_epi64(s0, 32);
			y1 = _mm_srli_epi64(s1, 32);
			y2 = _mm_srli_epi64(s2, 32);
			y3 = _mm_srli_epi64(s3, 32);

			MUL_S_U32xU32_2Y(rdy, y0, t0);
			MUL_S_U32xU32_2Y(ddy, y2, t1);
			MUL_S_U32xU32_2Y(rdy, y1, t2);
			MUL_S_U32xU32_2Y(ddy, y3, t3);

			v2 = _mm_add_epi64(t0, t1);
			w2 = _mm_add_epi64(t2, t3);
			v3 = _mm_unpacklo_epi32(v2, w2);
			w3 = _mm_unpackhi_epi32(v2, w2);
			s0 = _mm_unpackhi_epi64(v3, w3);
			s1 = XOR32(s0);
			_mm_storel_epi64((void *)tdp, s1);
			tdp += 2;

			x += 2 * dx;
			ddx = _mm_add_epi32(ddx, dx_step);
		}

		dp += dst_stride;
		y += dy;
		tsp = sp + (y >> MLIB_SHIFT) * src_stride;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#undef	BUFF_SIZE
#define	BUFF_SIZE	16384

/* *********************************************************** */

mlib_status
mlib_ImageZoomBilinear_S32_3(
    mlib_work_image * param)
{
	VARIABLE(mlib_s32)
	__m128i buff_arr[BUFF_SIZE / 16], *buff0, *buff1, *buffz;
	void *buff = buff_arr;
	mlib_s32 *tdp, *pbuff;
	mlib_s32 k, width3, dt, size, buffsize, y_step, next_step = 2;
	mlib_s32 nchan = param->channels;
	__m128i ddx_start, ddx, ddy, rdy;
	__m128i mask_ffffffff, mask_80000000;

	width *= 3;
	width3 = width / 3 - 1;
	buffsize = (width + 2) / 3;

	size = 4 * buffsize * sizeof (__m128i);
	if (size > BUFF_SIZE) {
		buff = __mlib_malloc(size);
		if (buff == NULL)
			return (MLIB_FAILURE);
	}

	buff0 = buff;
	buff1 = buff0 + 2 * buffsize;
	buffz = buff1 + 2 * buffsize;

/* calculate dx for all row */
	x = GetElemSubStruct(current, srcX) & MLIB_MASK;
	for (i = 0; i < 1; i++) {
		dt = x << 16;
		((mlib_s32 *)&ddx_start)[4 * i] = dt;
		((mlib_s32 *)&ddx_start)[4 * i + 1] = dt;
		((mlib_s32 *)&ddx_start)[4 * i + 2] = dt;
		((mlib_s32 *)&ddx_start)[4 * i + 3] = dt;
		x += dx;
	}

	mask_ffffffff = _mm_cmpeq_epi32(ddx_start, ddx_start);
	mask_80000000 = _mm_slli_epi32(mask_ffffffff, 31);

	for (j = 0; j < height; j++) {
		tdp = dp;

		y_step = next_step;
		next_step = ((y + dy) >> MLIB_SHIFT) - (y >> MLIB_SHIFT);

		if (y_step == 0) {

			ddy = _mm_set1_epi32(y << 16);
			rdy = _mm_sub_epi32(mask_ffffffff, ddy);

			for (i = 0; i < width3; i++) {
				__m128i x0, x1, y0, y1, dd0, dd1;
				__m128i s0, s1, s2, s3;
				x0 = buff0[2 * i];
				y0 = buff0[2 * i + 1];
				x1 = buff1[2 * i];
				y1 = buff1[2 * i + 1];
				MUL_S_U32xU32_2Y(rdy, x0, s0);
				MUL_S_U32xU32_2Y(ddy, x1, s1);
				MUL_S_U32xU32_2Y(rdy, y0, s2);
				MUL_S_U32xU32_2Y(ddy, y1, s3);
				dd0 = _mm_add_epi64(s0, s1);
				dd1 = _mm_add_epi64(s2, s3);
				x0 = _mm_unpacklo_epi32(dd0, dd1);
				x1 = _mm_unpackhi_epi32(dd0, dd1);
				y0 = _mm_unpackhi_epi64(x0, x1);
				y1 = XOR32(y0);
				_mm_storeu_si128((void *)tdp, y1);
				tdp += 3;
			}

			if (i * 3 < width) {
				__m128i x0, x1, y0, y1, dd0, dd1;
				__m128i s0, s1, s2, s3;
				x0 = buff0[2 * i];
				y0 = buff0[2 * i + 1];
				x1 = buff1[2 * i];
				y1 = buff1[2 * i + 1];
				MUL_S_U32xU32_2Y(rdy, x0, s0);
				MUL_S_U32xU32_2Y(ddy, x1, s1);
				MUL_S_U32xU32_2Y(rdy, y0, s2);
				MUL_S_U32xU32_2Y(ddy, y1, s3);
				dd0 = _mm_add_epi64(s0, s1);
				dd1 = _mm_add_epi64(s2, s3);
				x0 = _mm_unpacklo_epi32(dd0, dd1);
				x1 = _mm_unpackhi_epi32(dd0, dd1);
				s0 = _mm_unpackhi_epi64(x0, x1);
				s1 = XOR32(s0);

				((mlib_s64 *)tdp)[0] = ((mlib_s64 *)&s1)[0];
				((mlib_s32 *)tdp)[2] = ((mlib_s32 *)&s1)[2];
				tdp += 3;
			}

		} else if (y_step == 1) {
			__m128i *buffT;
			__m128i ddx, rdx;

			ddx = ddx_start;
			ddy = _mm_set1_epi32(y << 16);
			rdy = _mm_sub_epi32(mask_ffffffff, ddy);
			x = GetElemSubStruct(current, srcX) & MLIB_MASK;

			buffT = buff0;
			buff0 = buff1;
			buff1 = buffT;
			for (i = 0; i < width3; i++) {
				mlib_s32 *tsp2 = tsp + src_stride;
				mlib_s32 *sp0 =
				    tsp2 + 3 * (x >> MLIB_SHIFT);
				__m128i x0, x1, y0, y1, y2, y3, dx_step;
				__m128i v2, v3, w2, w3, s0, s1, s2, s3;
				x0 = _mm_loadu_si128((void *)sp0);
				x1 = _mm_loadu_si128((void *)(sp0 + 3));

				rdx = _mm_sub_epi32(mask_ffffffff, ddx);
				MUL_S_U32xU32_2X(XOR32(x0), rdx, v2, w2);
				MUL_S_U32xU32_2X(XOR32(x1), ddx, v3, w3);

				y0 = buff0[2 * i];
				y2 = buff0[2 * i + 1];

				s1 = _mm_add_epi64(v2, v3);
				s3 = _mm_add_epi64(w2, w3);
				y1 = _mm_srli_epi64(s1, 32);
				y3 = _mm_srli_epi64(s3, 32);

				MUL_S_U32xU32_2Y(rdy, y0, v2);
				MUL_S_U32xU32_2Y(ddy, y1, v3);
				MUL_S_U32xU32_2Y(rdy, y2, w2);
				MUL_S_U32xU32_2Y(ddy, y3, w3);

				buff1[2 * i] = y1;
				buff1[2 * i + 1] = y3;

				s0 = _mm_add_epi64(v2, v3);
				s1 = _mm_add_epi64(w2, w3);
				s2 = _mm_unpacklo_epi32(s0, s1);
				s3 = _mm_unpackhi_epi32(s0, s1);
				v2 = _mm_unpackhi_epi64(s2, s3);
				v3 = XOR32(v2);
				_mm_storeu_si128((__m128i *)tdp, v3);
				tdp += 3;
				x += dx;
				dx_step = _mm_set1_epi32(dx << 16);
				ddx = _mm_add_epi32(ddx, dx_step);
			}

			if (i * 3 < width) {
				mlib_s32 *tsp2 = tsp + src_stride;
				mlib_s32 *sp0 =
				    tsp2 + 3 * (x >> MLIB_SHIFT);
				__m128i x0, x1, y0, y1, y2, y3;
				__m128i v2, v3, w2, w3, s0, s1, s2, s3;

				x0 = _mm_loadu_si128((void *)sp0);
				x1 = _mm_loadu_si128((void *)(sp0 + 3));

				rdx = _mm_sub_epi32(mask_ffffffff, ddx);
				MUL_S_U32xU32_2X(XOR32(x0), rdx, v2, w2);
				MUL_S_U32xU32_2X(XOR32(x1), ddx, v3, w3);

				y0 = buff0[2 * i];
				y2 = buff0[2 * i + 1];
				s1 = _mm_add_epi64(v2, v3);
				s3 = _mm_add_epi64(w2, w3);
				y1 = _mm_srli_epi64(s1, 32);
				y3 = _mm_srli_epi64(s3, 32);

				MUL_S_U32xU32_2Y(rdy, y0, v2);
				MUL_S_U32xU32_2Y(ddy, y1, v3);
				MUL_S_U32xU32_2Y(rdy, y2, w2);
				MUL_S_U32xU32_2Y(ddy, y3, w3);

				buff1[2 * i] = y1;
				buff1[2 * i + 1] = y3;

				s0 = _mm_add_epi64(v2, v3);
				s1 = _mm_add_epi64(w2, w3);
				s2 = _mm_unpacklo_epi32(s0, s1);
				s3 = _mm_unpackhi_epi32(s0, s1);
				v2 = _mm_unpackhi_epi64(s2, s3);
				v3 = XOR32(v2);

				((mlib_s64 *)tdp)[0] = ((mlib_s64 *)&v3)[0];
				((mlib_s32 *)tdp)[2] = ((mlib_s32 *)&v3)[2];
				tdp += 3;
			}

		} else {
			__m128i ddx, rdx;
			ddx = ddx_start;
			ddy = _mm_set1_epi32(y << 16);
			rdy = _mm_sub_epi32(mask_ffffffff, ddy);
			x = GetElemSubStruct(current, srcX) & MLIB_MASK;

			for (i = 0; i < width3; i++) {
				__m128i x0, x1, x2, x3, y0, y1, y2, y3;
				__m128i v0, v1, v2, v3, w0, w1, w2, w3;
				__m128i s0, s1, s2, s3, dx_step;
				mlib_s32 *sp0 =
				    tsp + 3 * (x >> MLIB_SHIFT);

				x0 = _mm_loadu_si128((void *)sp0);
				x1 = _mm_loadu_si128((void *)(sp0 + 3));
				x2 = _mm_loadu_si128(
				    (void *)(sp0 + src_stride));
				x3 = _mm_loadu_si128(
				    (void *)(sp0 + src_stride + 3));

				rdx = _mm_sub_epi32(mask_ffffffff, ddx);
				MUL_S_U32xU32_2X(XOR32(x0), rdx, v0, w0);
				MUL_S_U32xU32_2X(XOR32(x1), ddx, v1, w1);
				MUL_S_U32xU32_2X(XOR32(x2), rdx, v2, w2);
				MUL_S_U32xU32_2X(XOR32(x3), ddx, v3, w3);

				y0 = _mm_add_epi64(v0, v1);
				y1 = _mm_add_epi64(v2, v3);
				y2 = _mm_add_epi64(w0, w1);
				y3 = _mm_add_epi64(w2, w3);
				s0 = _mm_srli_epi64(y0, 32);
				s1 = _mm_srli_epi64(y1, 32);
				s2 = _mm_srli_epi64(y2, 32);
				s3 = _mm_srli_epi64(y3, 32);

				MUL_S_U32xU32_2Y(rdy, s0, v0);
				MUL_S_U32xU32_2Y(ddy, s1, v1);
				MUL_S_U32xU32_2Y(rdy, s2, v2);
				MUL_S_U32xU32_2Y(ddy, s3, v3);

				buff0[2 * i] = s0;
				buff0[2 * i + 1] = s2;
				buff1[2 * i] = s1;
				buff1[2 * i + 1] = s3;

				w0 = _mm_add_epi64(v0, v1);
				w1 = _mm_add_epi64(v2, v3);
				w2 = _mm_unpacklo_epi32(w0, w1);
				w3 = _mm_unpackhi_epi32(w0, w1);
				s0 = _mm_unpackhi_epi64(w2, w3);
				s1 = XOR32(s0);
				_mm_storeu_si128((__m128i *)tdp, s1);
				tdp += 3;
				x += dx;
				dx_step = _mm_set1_epi32(dx << 16);
				ddx = _mm_add_epi32(ddx, dx_step);
			}

			if (i * 3 < width) {
				__m128i x0, x1, x2, x3, y0, y1, y2, y3;
				__m128i v0, v1, v2, v3, w0, w1, w2, w3;
				__m128i s0, s1, s2, s3;
				mlib_s32 *sp0 =
				    tsp + 3 * (x >> MLIB_SHIFT);

				x0 = _mm_loadu_si128((void *)sp0);
				x1 = _mm_loadu_si128((void *)(sp0 + 3));
				x2 = _mm_loadu_si128(
				    (void *)(sp0 + src_stride));
				x3 = _mm_loadu_si128(
				    (void *)(sp0 + src_stride + 3));

				rdx = _mm_sub_epi32(mask_ffffffff, ddx);
				MUL_S_U32xU32_2X(XOR32(x0), rdx, v0, w0);
				MUL_S_U32xU32_2X(XOR32(x1), ddx, v1, w1);
				MUL_S_U32xU32_2X(XOR32(x2), rdx, v2, w2);
				MUL_S_U32xU32_2X(XOR32(x3), ddx, v3, w3);

				y0 = _mm_add_epi64(v0, v1);
				y1 = _mm_add_epi64(v2, v3);
				y2 = _mm_add_epi64(w0, w1);
				y3 = _mm_add_epi64(w2, w3);
				s0 = _mm_srli_epi64(y0, 32);
				s1 = _mm_srli_epi64(y1, 32);
				s2 = _mm_srli_epi64(y2, 32);
				s3 = _mm_srli_epi64(y3, 32);

				MUL_S_U32xU32_2Y(rdy, s0, v0);
				MUL_S_U32xU32_2Y(ddy, s1, v1);
				MUL_S_U32xU32_2Y(rdy, s2, v2);
				MUL_S_U32xU32_2Y(ddy, s3, v3);

				buff0[2 * i] = s0;
				buff0[2 * i + 1] = s2;
				buff1[2 * i] = s1;
				buff1[2 * i + 1] = s3;

				w0 = _mm_add_epi64(v0, v1);
				w1 = _mm_add_epi64(v2, v3);
				w2 = _mm_unpacklo_epi32(w0, w1);
				w3 = _mm_unpackhi_epi32(w0, w1);
				s0 = _mm_unpackhi_epi64(w2, w3);
				s1 = XOR32(s0);
				((mlib_s64 *)tdp)[0] = ((mlib_s64 *)&s1)[0];
				((mlib_s32 *)tdp)[2] = ((mlib_s32 *)&s1)[2];
				tdp += 3;
			}
		}

		dp += dst_stride;
		y += dy;
		tsp = sp + (y >> MLIB_SHIFT) * src_stride;
	}

	if (size > BUFF_SIZE) {
		__mlib_free(buff);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_ImageZoomBilinear_S32_3s(
    mlib_work_image * param)
{
	VARIABLE(mlib_s32)
	mlib_s32 *tdp;
	mlib_s32 k, width3, dt;
	mlib_s32 nchan = param->channels;
	__m128i ddx_start, ddx, ddy, rdy;
	__m128i mask_ffffffff, mask_80000000;

	width *= 3;
	width3 = width / 3 - 1;

/* calculate dx for all row */
	x = GetElemSubStruct(current, srcX) & MLIB_MASK;
	for (i = 0; i < 1; i++) {
		dt = x << 16;
		((mlib_s32 *)&ddx_start)[4 * i] = dt;
		((mlib_s32 *)&ddx_start)[4 * i + 1] = dt;
		((mlib_s32 *)&ddx_start)[4 * i + 2] = dt;
		((mlib_s32 *)&ddx_start)[4 * i + 3] = dt;
		x += dx;
	}

	mask_ffffffff = _mm_cmpeq_epi32(ddx_start, ddx_start);
	mask_80000000 = _mm_slli_epi32(mask_ffffffff, 31);

	for (j = 0; j < height; j++) {
		tdp = dp;

			__m128i ddx, rdx;
			ddx = ddx_start;
		ddy = _mm_set1_epi32(y << 16);
		rdy = _mm_sub_epi32(mask_ffffffff, ddy);
		x = GetElemSubStruct(current, srcX) & MLIB_MASK;

		for (i = 0; i < width3; i++) {
			__m128i x0, x1, x2, x3, y0, y1, y2, y3;
			__m128i v0, v1, v2, v3, w0, w1, w2, w3;
			__m128i s0, s1, s2, s3, dx_step;
			mlib_s32 *sp0 =
			    tsp + 3 * (x >> MLIB_SHIFT);

			x0 = _mm_loadu_si128((void *)sp0);
			x1 = _mm_loadu_si128((void *)(sp0 + 3));
			x2 = _mm_loadu_si128((void *)(sp0 + src_stride));
			x3 = _mm_loadu_si128((void *)(sp0 + src_stride + 3));

			rdx = _mm_sub_epi32(mask_ffffffff, ddx);
			MUL_S_U32xU32_2X(XOR32(x0), rdx, v0, w0);
			MUL_S_U32xU32_2X(XOR32(x1), ddx, v1, w1);
			MUL_S_U32xU32_2X(XOR32(x2), rdx, v2, w2);
			MUL_S_U32xU32_2X(XOR32(x3), ddx, v3, w3);

			y0 = _mm_add_epi64(v0, v1);
			y1 = _mm_add_epi64(v2, v3);
			y2 = _mm_add_epi64(w0, w1);
			y3 = _mm_add_epi64(w2, w3);
			s0 = _mm_srli_epi64(y0, 32);
			s1 = _mm_srli_epi64(y1, 32);
			s2 = _mm_srli_epi64(y2, 32);
			s3 = _mm_srli_epi64(y3, 32);

			MUL_S_U32xU32_2Y(rdy, s0, v0);
			MUL_S_U32xU32_2Y(ddy, s1, v1);
			MUL_S_U32xU32_2Y(rdy, s2, v2);
			MUL_S_U32xU32_2Y(ddy, s3, v3);

			w0 = _mm_add_epi64(v0, v1);
			w1 = _mm_add_epi64(v2, v3);
			w2 = _mm_unpacklo_epi32(w0, w1);
			w3 = _mm_unpackhi_epi32(w0, w1);
			s0 = _mm_unpackhi_epi64(w2, w3);
			s1 = XOR32(s0);
			_mm_storeu_si128((__m128i *)tdp, s1);
			tdp += 3;
			x += dx;
			dx_step = _mm_set1_epi32(dx << 16);
			ddx = _mm_add_epi32(ddx, dx_step);
		}

		if (i * 3 < width) {
			__m128i x0, x1, x2, x3, y0, y1, y2, y3;
			__m128i v0, v1, v2, v3, w0, w1, w2, w3;
			__m128i s0, s1, s2, s3;
			mlib_s32 *sp0 =
			    tsp + 3 * (x >> MLIB_SHIFT);

			x0 = _mm_loadu_si128((void *)sp0);
			x1 = _mm_loadu_si128((void *)(sp0 + 3));
			x2 = _mm_loadu_si128((void *)(sp0 + src_stride));
			x3 = _mm_loadu_si128((void *)(sp0 + src_stride + 3));

			rdx = _mm_sub_epi32(mask_ffffffff, ddx);
			MUL_S_U32xU32_2X(XOR32(x0), rdx, v0, w0);
			MUL_S_U32xU32_2X(XOR32(x1), ddx, v1, w1);
			MUL_S_U32xU32_2X(XOR32(x2), rdx, v2, w2);
			MUL_S_U32xU32_2X(XOR32(x3), ddx, v3, w3);

			y0 = _mm_add_epi64(v0, v1);
			y1 = _mm_add_epi64(v2, v3);
			y2 = _mm_add_epi64(w0, w1);
			y3 = _mm_add_epi64(w2, w3);
			s0 = _mm_srli_epi64(y0, 32);
			s1 = _mm_srli_epi64(y1, 32);
			s2 = _mm_srli_epi64(y2, 32);
			s3 = _mm_srli_epi64(y3, 32);

			MUL_S_U32xU32_2Y(rdy, s0, v0);
			MUL_S_U32xU32_2Y(ddy, s1, v1);
			MUL_S_U32xU32_2Y(rdy, s2, v2);
			MUL_S_U32xU32_2Y(ddy, s3, v3);

			w0 = _mm_add_epi64(v0, v1);
			w1 = _mm_add_epi64(v2, v3);
			w2 = _mm_unpacklo_epi32(w0, w1);
			w3 = _mm_unpackhi_epi32(w0, w1);
			s0 = _mm_unpackhi_epi64(w2, w3);
			s1 = XOR32(s0);
			((mlib_s64 *)tdp)[0] = ((mlib_s64 *)&s1)[0];
			((mlib_s32 *)tdp)[2] = ((mlib_s32 *)&s1)[2];
			tdp += 3;
		}

		dp += dst_stride;
		y += dy;
		tsp = sp + (y >> MLIB_SHIFT) * src_stride;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#undef	BUFF_SIZE
#define	BUFF_SIZE	20480

/* *********************************************************** */

mlib_status
mlib_ImageZoomBilinear_S32_4(
    mlib_work_image * param)
{
	VARIABLE(mlib_s32)
	__m128i buff_arr[BUFF_SIZE / 16], *buff0, *buff1, *buffz;
	void *buff = buff_arr;
	mlib_s32 *tdp, *pbuff;
	mlib_s32 k, width4, dt, size, y_step, next_step = 2;
	mlib_s32 nchan = param->channels;
	__m128i ddx_start, ddx, ddy, rdy, dx_step;
	__m128i mask_ffffffff, mask_80000000;

	width *= 4;
	width4 = (width + 3) / 4;

	size = 5 * width4 * sizeof (__m128i);
	if (size > BUFF_SIZE) {
		buff = __mlib_malloc(size);
		if (buff == NULL)
			return (MLIB_FAILURE);
	}

	buff0 = buff;
	buff1 = buff0 + 2 * width4;
	buffz = buff1 + 2 * width4;

/* calculate dx for all row */
	x = GetElemSubStruct(current, srcX) & MLIB_MASK;
	for (i = 0; i < 1; i++) {
		dt = x << 16;
		((mlib_s32 *)&ddx_start)[4 * i] = dt;
		((mlib_s32 *)&ddx_start)[4 * i + 1] = dt;
		((mlib_s32 *)&ddx_start)[4 * i + 2] = dt;
		((mlib_s32 *)&ddx_start)[4 * i + 3] = dt;
		x += dx;
	}

	mask_ffffffff = _mm_cmpeq_epi32(ddx_start, ddx_start);
	mask_80000000 = _mm_slli_epi32(mask_ffffffff, 31);

	dx_step = _mm_set1_epi32(dx << 16);

	for (j = 0; j < height; j++) {
		tdp = dp;

		y_step = next_step;
		next_step = ((y + dy) >> MLIB_SHIFT) - (y >> MLIB_SHIFT);

		if (y_step == 0) {

			ddy = _mm_set1_epi32(y << 16);
			rdy = _mm_sub_epi32(mask_ffffffff, ddy);

			__m128i y0, y1, y2, y3;
			__m128i s0, s1, s2, s3, t0, t1, t2, t3;

			for (i = 0; i < width4; i++) {
				y0 = buff0[2 * i];
				y1 = buff0[2 * i + 1];
				y2 = buff1[2 * i];
				y3 = buff1[2 * i + 1];
				MUL_S_U32xU32_2Y(rdy, y0, s0);
				MUL_S_U32xU32_2Y(ddy, y2, s1);
				MUL_S_U32xU32_2Y(rdy, y1, s2);
				MUL_S_U32xU32_2Y(ddy, y3, s3);
				t0 = _mm_add_epi64(s0, s1);
				t1 = _mm_add_epi64(s2, s3);
				t2 = _mm_unpacklo_epi32(t0, t1);
				t3 = _mm_unpackhi_epi32(t0, t1);
				s0 = _mm_unpackhi_epi64(t2, t3);
				s1 = XOR32(s0);
				_mm_storeu_si128((void *)tdp, s1);
				tdp += 4;
			}

		} else if (y_step == 1) {
			__m128i *buffT;
			__m128i ddx, rdx;

			ddx = ddx_start;
			ddy = _mm_set1_epi32(y << 16);
			rdy = _mm_sub_epi32(mask_ffffffff, ddy);
			x = GetElemSubStruct(current, srcX) & MLIB_MASK;

			buffT = buff0;
			buff0 = buff1;
			buff1 = buffT;

			__m128i x0, x1, t0, t1, y0, y1, y2, y3;
			__m128i v2, v3, w2, w3, s0, s1, s2, s3;

			for (i = 0; i < width4; i++) {
				mlib_s32 *tsp2 = tsp + src_stride;
				mlib_s32 *sp0 =
				    tsp2 + 4 * (x >> MLIB_SHIFT);

				x0 = _mm_loadu_si128((void *)sp0);
				x1 = _mm_loadu_si128((void *)(sp0 + 4));

				rdx = _mm_sub_epi32(mask_ffffffff, ddx);
				MUL_S_U32xU32_2X(XOR32(x0), rdx, v2, w2);
				MUL_S_U32xU32_2X(XOR32(x1), ddx, v3, w3);

				y0 = buff0[2 * i];
				y1 = buff0[2 * i + 1];
				t0 = _mm_add_epi64(v2, v3);
				t1 = _mm_add_epi64(w2, w3);
				y2 = _mm_srli_epi64(t0, 32);
				y3 = _mm_srli_epi64(t1, 32);

				MUL_S_U32xU32_2Y(rdy, y0, s0);
				MUL_S_U32xU32_2Y(ddy, y2, s1);
				MUL_S_U32xU32_2Y(rdy, y1, s2);
				MUL_S_U32xU32_2Y(ddy, y3, s3);

				buff1[2 * i] = y2;
				buff1[2 * i + 1] = y3;

				v2 = _mm_add_epi64(s0, s1);
				w2 = _mm_add_epi64(s2, s3);
				v3 = _mm_unpacklo_epi32(v2, w2);
				w3 = _mm_unpackhi_epi32(v2, w2);
				s0 = _mm_unpackhi_epi64(v3, w3);
				s1 = XOR32(s0);
				_mm_storeu_si128((void *)tdp, s1);
				tdp += 4;

				x += dx;
				ddx = _mm_add_epi32(ddx, dx_step);
			}

		} else {
			__m128i ddx, rdx;

			ddx = ddx_start;
			ddy = _mm_set1_epi32(y << 16);
			rdy = _mm_sub_epi32(mask_ffffffff, ddy);
			x = GetElemSubStruct(current, srcX) & MLIB_MASK;

			__m128i x0, x1, x2, x3, y0, y1, y2, y3;
			__m128i v0, v1, v2, v3, w0, w1, w2, w3;
			__m128i s0, s1, s2, s3, t0, t1, t2, t3;

			for (i = 0; i < width4; i++) {
				mlib_s32 *sp0 =
			    tsp + 4 * (x >> MLIB_SHIFT);

				x0 = _mm_loadu_si128((void *)sp0);
				x1 = _mm_loadu_si128((void *)(sp0 + 4));
				x2 = _mm_loadu_si128(
				    (void *)(sp0 + src_stride));
				x3 = _mm_loadu_si128(
				    (void *)(sp0 + src_stride + 4));

				rdx = _mm_sub_epi32(mask_ffffffff, ddx);
				MUL_S_U32xU32_2X(XOR32(x0), rdx, v0, w0);
				MUL_S_U32xU32_2X(XOR32(x1), ddx, v1, w1);
				MUL_S_U32xU32_2X(XOR32(x2), rdx, v2, w2);
				MUL_S_U32xU32_2X(XOR32(x3), ddx, v3, w3);

				s0 = _mm_add_epi64(v0, v1);
				s1 = _mm_add_epi64(v2, v3);
				s2 = _mm_add_epi64(w0, w1);
				s3 = _mm_add_epi64(w2, w3);
				y0 = _mm_srli_epi64(s0, 32);
				y1 = _mm_srli_epi64(s1, 32);
				y2 = _mm_srli_epi64(s2, 32);
				y3 = _mm_srli_epi64(s3, 32);

				MUL_S_U32xU32_2Y(rdy, y0, t0);
				MUL_S_U32xU32_2Y(ddy, y1, t1);
				MUL_S_U32xU32_2Y(rdy, y2, t2);
				MUL_S_U32xU32_2Y(ddy, y3, t3);

				buff0[2 * i] = y0;
				buff0[2 * i + 1] = y2;
				buff1[2 * i] = y1;
				buff1[2 * i + 1] = y3;

				v2 = _mm_add_epi64(t0, t1);
				w2 = _mm_add_epi64(t2, t3);
				v3 = _mm_unpacklo_epi32(v2, w2);
				w3 = _mm_unpackhi_epi32(v2, w2);
				s0 = _mm_unpackhi_epi64(v3, w3);
				s1 = XOR32(s0);
				_mm_storeu_si128((void *)tdp, s1);
				tdp += 4;

				x += dx;
				ddx = _mm_add_epi32(ddx, dx_step);
			}
		}

		dp += dst_stride;
		y += dy;
		tsp = sp + (y >> MLIB_SHIFT) * src_stride;
	}

	if (size > BUFF_SIZE) {
		__mlib_free(buff);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_ImageZoomBilinear_S32_4s(
    mlib_work_image * param)
{
	VARIABLE(mlib_s32)
	mlib_s32 *tdp;
	mlib_s32 k, width4, dt;
	mlib_s32 nchan = param->channels;
	__m128i ddx_start, ddx, ddy, rdy, dx_step;
	__m128i mask_ffffffff, mask_80000000;

	width *= 4;
	width4 = (width + 3) / 4;

/* calculate dx for all row */
	x = GetElemSubStruct(current, srcX) & MLIB_MASK;
	for (i = 0; i < 1; i++) {
		dt = x << 16;
		((mlib_s32 *)&ddx_start)[4 * i] = dt;
		((mlib_s32 *)&ddx_start)[4 * i + 1] = dt;
		((mlib_s32 *)&ddx_start)[4 * i + 2] = dt;
		((mlib_s32 *)&ddx_start)[4 * i + 3] = dt;
		x += dx;
	}

	mask_ffffffff = _mm_cmpeq_epi32(ddx_start, ddx_start);
	mask_80000000 = _mm_slli_epi32(mask_ffffffff, 31);

	dx_step = _mm_set1_epi32(dx << 16);

	for (j = 0; j < height; j++) {
		tdp = dp;

		__m128i ddx, rdx;

			ddx = ddx_start;
		ddy = _mm_set1_epi32(y << 16);
		rdy = _mm_sub_epi32(mask_ffffffff, ddy);
		x = GetElemSubStruct(current, srcX) & MLIB_MASK;

		__m128i x0, x1, x2, x3, y0, y1, y2, y3;
		__m128i v0, v1, v2, v3, w0, w1, w2, w3;
		__m128i s0, s1, s2, s3, t0, t1, t2, t3;

		for (i = 0; i < width4; i++) {
			mlib_s32 *sp0 =
		    tsp + 4 * (x >> MLIB_SHIFT);

			x0 = _mm_loadu_si128((void *)sp0);
			x1 = _mm_loadu_si128((void *)(sp0 + 4));
			x2 = _mm_loadu_si128((void *)(sp0 + src_stride));
			x3 = _mm_loadu_si128((void *)(sp0 + src_stride + 4));

			rdx = _mm_sub_epi32(mask_ffffffff, ddx);
			MUL_S_U32xU32_2X(XOR32(x0), rdx, v0, w0);
			MUL_S_U32xU32_2X(XOR32(x1), ddx, v1, w1);
			MUL_S_U32xU32_2X(XOR32(x2), rdx, v2, w2);
			MUL_S_U32xU32_2X(XOR32(x3), ddx, v3, w3);

			s0 = _mm_add_epi64(v0, v1);
			s1 = _mm_add_epi64(v2, v3);
			s2 = _mm_add_epi64(w0, w1);
			s3 = _mm_add_epi64(w2, w3);
			y0 = _mm_srli_epi64(s0, 32);
			y1 = _mm_srli_epi64(s1, 32);
			y2 = _mm_srli_epi64(s2, 32);
			y3 = _mm_srli_epi64(s3, 32);

			MUL_S_U32xU32_2Y(rdy, y0, t0);
			MUL_S_U32xU32_2Y(ddy, y1, t1);
			MUL_S_U32xU32_2Y(rdy, y2, t2);
			MUL_S_U32xU32_2Y(ddy, y3, t3);

			v2 = _mm_add_epi64(t0, t1);
			w2 = _mm_add_epi64(t2, t3);
			v3 = _mm_unpacklo_epi32(v2, w2);
			w3 = _mm_unpackhi_epi32(v2, w2);
			s0 = _mm_unpackhi_epi64(v3, w3);
			s1 = XOR32(s0);
			_mm_storeu_si128((void *)tdp, s1);
			tdp += 4;

			x += dx;
			ddx = _mm_add_epi32(ddx, dx_step);
		}

		dp += dst_stride;
		y += dy;
		tsp = sp + (y >> MLIB_SHIFT) * src_stride;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
