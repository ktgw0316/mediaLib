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

#pragma ident	"@(#)mlib_s_ImageZoom_BL_U16.c	9.5	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_ImageZoomTranslate(), internal pixels.
 *
 */

#include <mlib_s_ImageZoom.h>

/* *********************************************************** */

#define	XOR(x)	(x)

#define	RND_VAL	3

/* *********************************************************** */

mlib_status
mlib_s_ImageZoom_BL_U16_1(
    mlib_work_image * param)
{
	VARIABLE(mlib_u16)
	__m128i buff_arr[BUFF_SIZE / 16], *buff0, *buff1, *buffz;
	void *buff = buff_arr;
	mlib_u16 *tdp, *pbuff;
	mlib_s32 k, width8, size, y_step, next_step = 2;
	__m128i ddx_start, ddx, ddy, rdy, dx_step;
	__m128i mzero, mask_ffff, mask_8000;
	__m128i d_rnd = _mm_set1_epi16(RND_VAL);

	width8 = (width + 7) / 8;

	size = 3 * width8 * sizeof (__m128i);
	if (size > BUFF_SIZE) {
		buff = __mlib_malloc(size);
		if (buff == NULL)
			return (MLIB_FAILURE);
	}

	buff0 = buff;
	buff1 = buff0 + width8;
	buffz = buff1 + width8;

	mzero = _mm_setzero_si128();
	mask_ffff = _mm_set1_epi16(0xFFFF);
	mask_8000 = _mm_set1_epi16(0x8000);

/* calculate dx for all row */
	x = GetElemSubStruct(current, srcX) & MLIB_MASK;
	for (i = 0; i < 8; i++) {
		((mlib_s16 *)&ddx_start)[i] = x;
		x += dx;
	}
	dx_step = _mm_set1_epi16(8 * dx);

	for (j = 0; j < height; j++) {
		tdp = dp;

		y_step = next_step;
		next_step = ((y + dy) >> MLIB_SHIFT) - (y >> MLIB_SHIFT);

		if (y_step == 0) {
			__m128i y0, y1, dd;

			ddy = _mm_set1_epi16(y);
			rdy = _mm_sub_epi16(mask_ffff, ddy);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < width8; i++) {
				y0 = buff0[i];
				y1 = buff1[i];
				y0 = _mm_mulhi_epu16(rdy, y0);
				y1 = _mm_mulhi_epu16(ddy, y1);
				dd = _mm_add_epi16(y0, y1);
				dd = _mm_adds_epu16(dd, d_rnd);
				buffz[i] = XOR(dd);
			}
		} else if (y_step == 1) {
			__m128i *buffT;
			__m128i ddx, rdx;

			ddx = ddx_start;
			ddy = _mm_set1_epi16(y);
			rdy = _mm_sub_epi16(mask_ffff, ddy);
			x = GetElemSubStruct(current, srcX) & MLIB_MASK;

			buffT = buff0;
			buff0 = buff1;
			buff1 = buffT;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < width8; i++) {
				mlib_u16 *tsp2 = tsp + src_stride;
				mlib_u16 *sp0 = tsp2 + (x >> MLIB_SHIFT);
				mlib_u16 *sp1 = tsp2 + ((x + dx) >> MLIB_SHIFT);
				mlib_u16 *sp2 =
				    tsp2 + ((x + 2 * dx) >> MLIB_SHIFT);
				mlib_u16 *sp3 =
				    tsp2 + ((x + 3 * dx) >> MLIB_SHIFT);
				mlib_u16 *sp4 =
				    tsp2 + ((x + 4 * dx) >> MLIB_SHIFT);
				mlib_u16 *sp5 =
				    tsp2 + ((x + 5 * dx) >> MLIB_SHIFT);
				mlib_u16 *sp6 =
				    tsp2 + ((x + 6 * dx) >> MLIB_SHIFT);
				mlib_u16 *sp7 =
				    tsp2 + ((x + 7 * dx) >> MLIB_SHIFT);
				__m128i v2, v3, y0, y1, z0, z1, dd;
				__m128i x0, x1, x2, x3;
				__m128i s2, s3, t0, t1;

				LOAD_4s16_to_m128(x0, sp0, sp1);
				LOAD_4s16_to_m128(x1, sp2, sp3);
				LOAD_4s16_to_m128(x2, sp4, sp5);
				LOAD_4s16_to_m128(x3, sp6, sp7);
				t0 = _mm_unpacklo_epi32(x0, x1);
				t1 = _mm_unpacklo_epi32(x2, x3);
				s2 = _mm_unpacklo_epi64(t0, t1);
				s3 = _mm_unpackhi_epi64(t0, t1);

				rdx = _mm_sub_epi16(mask_ffff, ddx);

				v2 = _mm_mulhi_epu16(XOR(s2), rdx);
				v3 = _mm_mulhi_epu16(XOR(s3), ddx);

				y0 = buff0[i];
				y1 = _mm_add_epi16(v2, v3);

				z0 = _mm_mulhi_epu16(rdy, y0);
				z1 = _mm_mulhi_epu16(ddy, y1);
				dd = _mm_add_epi16(z0, z1);
				dd = _mm_adds_epu16(dd, d_rnd);
				buffz[i] = XOR(dd);

				buff1[i] = y1;
				x += 8 * dx;
				ddx = _mm_add_epi16(ddx, dx_step);
			}

		} else {
			__m128i ddx, rdx;

			ddx = ddx_start;
			ddy = _mm_set1_epi16(y);
			rdy = _mm_sub_epi16(mask_ffff, ddy);
			x = GetElemSubStruct(current, srcX) & MLIB_MASK;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < width8; i++) {
				__m128i v0, v1, v2, v3, y0, y1, z0, z1, dd;
				__m128i x0, x1, x2, x3;
				__m128i s0, s1, s2, s3;
				__m128i t0, t1;
				mlib_u16 *sp0 = tsp + (x >> MLIB_SHIFT);
				mlib_u16 *sp1 = tsp + ((x + dx) >> MLIB_SHIFT);
				mlib_u16 *sp2 =
				    tsp + ((x + 2 * dx) >> MLIB_SHIFT);
				mlib_u16 *sp3 =
				    tsp + ((x + 3 * dx) >> MLIB_SHIFT);
				mlib_u16 *sp4 =
				    tsp + ((x + 4 * dx) >> MLIB_SHIFT);
				mlib_u16 *sp5 =
				    tsp + ((x + 5 * dx) >> MLIB_SHIFT);
				mlib_u16 *sp6 =
				    tsp + ((x + 6 * dx) >> MLIB_SHIFT);
				mlib_u16 *sp7 =
				    tsp + ((x + 7 * dx) >> MLIB_SHIFT);

				LOAD_4s16_to_m128(x0, sp0, sp1);
				LOAD_4s16_to_m128(x1, sp2, sp3);
				LOAD_4s16_to_m128(x2, sp4, sp5);
				LOAD_4s16_to_m128(x3, sp6, sp7);
				t0 = _mm_unpacklo_epi32(x0, x1);
				t1 = _mm_unpacklo_epi32(x2, x3);
				s0 = _mm_unpacklo_epi64(t0, t1);
				s1 = _mm_unpackhi_epi64(t0, t1);

				LOAD_4s16_to_m128(x0, sp0 + src_stride,
				    sp1 + src_stride);
				LOAD_4s16_to_m128(x1, sp2 + src_stride,
				    sp3 + src_stride);
				LOAD_4s16_to_m128(x2, sp4 + src_stride,
				    sp5 + src_stride);
				LOAD_4s16_to_m128(x3, sp6 + src_stride,
				    sp7 + src_stride);
				t0 = _mm_unpacklo_epi32(x0, x1);
				t1 = _mm_unpacklo_epi32(x2, x3);
				s2 = _mm_unpacklo_epi64(t0, t1);
				s3 = _mm_unpackhi_epi64(t0, t1);

				rdx = _mm_sub_epi16(mask_ffff, ddx);

				v0 = _mm_mulhi_epu16(XOR(s0), rdx);
				v1 = _mm_mulhi_epu16(XOR(s1), ddx);
				v2 = _mm_mulhi_epu16(XOR(s2), rdx);
				v3 = _mm_mulhi_epu16(XOR(s3), ddx);

				y0 = _mm_add_epi16(v0, v1);
				y1 = _mm_add_epi16(v2, v3);

				z0 = _mm_mulhi_epu16(rdy, y0);
				z1 = _mm_mulhi_epu16(ddy, y1);
				dd = _mm_add_epi16(z0, z1);
				dd = _mm_adds_epu16(dd, d_rnd);
				buffz[i] = XOR(dd);

				buff0[i] = y0;
				buff1[i] = y1;
				x += 8 * dx;
				ddx = _mm_add_epi16(ddx, dx_step);
			}
		}

		mlib_ImageCopy_na((void *)buffz, (void *)dp,
		    width * sizeof (mlib_u16));

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
mlib_s_ImageZoom_BL_U16_2(
    mlib_work_image * param)
{
	VARIABLE(mlib_u16)
	__m128i buff_arr[BUFF_SIZE / 16], *buff0, *buff1, *buffz;
	void *buff = buff_arr;
	mlib_u16 *tdp, *pbuff;
	mlib_s32 k, width8, size, y_step, next_step = 2;
	__m128i ddx_start, ddx, ddy, rdy, dx_step;
	__m128i mzero, mask_ffff, mask_8000;
	__m128i d_rnd = _mm_set1_epi16(RND_VAL);

	width *= 2;
	width8 = (width + 7) / 8;

	size = 3 * width8 * sizeof (__m128i);
	if (size > BUFF_SIZE) {
		buff = __mlib_malloc(size);
		if (buff == NULL)
			return (MLIB_FAILURE);
	}

	buff0 = buff;
	buff1 = buff0 + width8;
	buffz = buff1 + width8;

	mzero = _mm_setzero_si128();
	mask_ffff = _mm_set1_epi16(0xFFFF);
	mask_8000 = _mm_set1_epi16(0x8000);

/* calculate dx for all row */
	x = GetElemSubStruct(current, srcX) & MLIB_MASK;
	for (i = 0; i < 4; i++) {
		((mlib_s16 *)&ddx_start)[2 * i] = x;
		((mlib_s16 *)&ddx_start)[2 * i + 1] = x;
		x += dx;
	}
	dx_step = _mm_set1_epi16(4 * dx);

	for (j = 0; j < height; j++) {
		tdp = dp;

		y_step = next_step;
		next_step = ((y + dy) >> MLIB_SHIFT) - (y >> MLIB_SHIFT);

		if (y_step == 0) {
			__m128i y0, y1, dd;

			ddy = _mm_set1_epi16(y);
			rdy = _mm_sub_epi16(mask_ffff, ddy);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < width8; i++) {
				y0 = buff0[i];
				y1 = buff1[i];
				y0 = _mm_mulhi_epu16(rdy, y0);
				y1 = _mm_mulhi_epu16(ddy, y1);
				dd = _mm_add_epi16(y0, y1);
				dd = _mm_adds_epu16(dd, d_rnd);
				buffz[i] = XOR(dd);
			}
		} else if (y_step == 1) {
			__m128i *buffT;
			__m128i ddx, rdx;

			ddx = ddx_start;
			ddy = _mm_set1_epi16(y);
			rdy = _mm_sub_epi16(mask_ffff, ddy);
			x = GetElemSubStruct(current, srcX) & MLIB_MASK;

			buffT = buff0;
			buff0 = buff1;
			buff1 = buffT;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < width8; i++) {
				mlib_u16 *tsp2 = tsp + src_stride;
				mlib_u16 *sp0 = tsp2 + 2 * (x >> MLIB_SHIFT);
				mlib_u16 *sp1 =
				    tsp2 + 2 * ((x + dx) >> MLIB_SHIFT);
				mlib_u16 *sp2 =
				    tsp2 + 2 * ((x + 2 * dx) >> MLIB_SHIFT);
				mlib_u16 *sp3 =
				    tsp2 + 2 * ((x + 3 * dx) >> MLIB_SHIFT);
				__m128i v2, v3, y0, y1, z0, z1, dd;
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

				rdx = _mm_sub_epi16(mask_ffff, ddx);

				v2 = _mm_mulhi_epu16(XOR(s2), rdx);
				v3 = _mm_mulhi_epu16(XOR(s3), ddx);

				y0 = buff0[i];
				y1 = _mm_add_epi16(v2, v3);

				z0 = _mm_mulhi_epu16(rdy, y0);
				z1 = _mm_mulhi_epu16(ddy, y1);
				dd = _mm_add_epi16(z0, z1);
				dd = _mm_adds_epu16(dd, d_rnd);
				buffz[i] = XOR(dd);

				buff1[i] = y1;
				x += 4 * dx;
				ddx = _mm_add_epi16(ddx, dx_step);
			}

		} else {
			__m128i ddx, rdx;

			ddx = ddx_start;
			ddy = _mm_set1_epi16(y);
			rdy = _mm_sub_epi16(mask_ffff, ddy);
			x = GetElemSubStruct(current, srcX) & MLIB_MASK;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < width8; i++) {
				__m128i v0, v1, v2, v3, y0, y1, z0, z1, dd;
				__m128i x0, x1, x2, x3;
				__m128i s0, s1, s2, s3;
				__m128i t0, t1;
				mlib_u16 *sp0 = tsp + 2 * (x >> MLIB_SHIFT);
				mlib_u16 *sp1 =
				    tsp + 2 * ((x + dx) >> MLIB_SHIFT);
				mlib_u16 *sp2 =
				    tsp + 2 * ((x + 2 * dx) >> MLIB_SHIFT);
				mlib_u16 *sp3 =
				    tsp + 2 * ((x + 3 * dx) >> MLIB_SHIFT);

				x0 = _mm_loadl_epi64((void *)sp0);
				x1 = _mm_loadl_epi64((void *)sp1);
				x2 = _mm_loadl_epi64((void *)sp2);
				x3 = _mm_loadl_epi64((void *)sp3);
				t0 = _mm_unpacklo_epi32(x0, x1);
				t1 = _mm_unpacklo_epi32(x2, x3);
				s0 = _mm_unpacklo_epi64(t0, t1);
				s1 = _mm_unpackhi_epi64(t0, t1);

				x0 = _mm_loadl_epi64((void *)(sp0 +
				    src_stride));
				x1 = _mm_loadl_epi64((void *)(sp1 +
				    src_stride));
				x2 = _mm_loadl_epi64((void *)(sp2 +
				    src_stride));
				x3 = _mm_loadl_epi64((void *)(sp3 +
				    src_stride));
				t0 = _mm_unpacklo_epi32(x0, x1);
				t1 = _mm_unpacklo_epi32(x2, x3);
				s2 = _mm_unpacklo_epi64(t0, t1);
				s3 = _mm_unpackhi_epi64(t0, t1);

				rdx = _mm_sub_epi16(mask_ffff, ddx);

				v0 = _mm_mulhi_epu16(XOR(s0), rdx);
				v1 = _mm_mulhi_epu16(XOR(s1), ddx);
				v2 = _mm_mulhi_epu16(XOR(s2), rdx);
				v3 = _mm_mulhi_epu16(XOR(s3), ddx);

				y0 = _mm_add_epi16(v0, v1);
				y1 = _mm_add_epi16(v2, v3);

				z0 = _mm_mulhi_epu16(rdy, y0);
				z1 = _mm_mulhi_epu16(ddy, y1);
				dd = _mm_add_epi16(z0, z1);
				dd = _mm_adds_epu16(dd, d_rnd);
				buffz[i] = XOR(dd);

				buff0[i] = y0;
				buff1[i] = y1;
				x += 4 * dx;
				ddx = _mm_add_epi16(ddx, dx_step);
			}
		}

		mlib_ImageCopy_na((void *)buffz, (void *)dp,
		    width * sizeof (mlib_u16));

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
mlib_s_ImageZoom_BL_U16_3(
    mlib_work_image * param)
{
	VARIABLE(mlib_u16)
	__m128i buff_arr[BUFF_SIZE / 16], *buff0, *buff1;
	void *buff = buff_arr;
	mlib_u16 *tdp, *pbuff;
	mlib_s32 k, width8, widthbuff, size, y_step, next_step = 2;
	__m128i ddx_start, ddx, ddy, rdy, dx_step;
	__m128i mzero, mask_ffff, mask_8000;
	__m128i d_rnd = _mm_set1_epi16(RND_VAL);

	width8 = width >> 1;
	widthbuff = (width * 4 + 7) / 8;

	size = 2 * widthbuff * sizeof (__m128i);
	if (size > BUFF_SIZE) {
		buff = __mlib_malloc(size);
		if (buff == NULL)
			return (MLIB_FAILURE);
	}

	buff0 = buff;
	buff1 = buff0 + widthbuff;

	mzero = _mm_setzero_si128();
	mask_ffff = _mm_set1_epi16(0xFFFF);
	mask_8000 = _mm_set1_epi16(0x8000);

/* calculate dx for all row */
	x = GetElemSubStruct(current, srcX) & MLIB_MASK;
	for (i = 0; i < 2; i++) {
		((mlib_s16 *)&ddx_start)[4 * i] = x;
		((mlib_s16 *)&ddx_start)[4 * i + 1] = x;
		((mlib_s16 *)&ddx_start)[4 * i + 2] = x;
		((mlib_s16 *)&ddx_start)[4 * i + 3] = x;
		x += dx;
	}
	dx_step = _mm_set1_epi16(2 * dx);

	for (j = 0; j < height; j++) {
		tdp = dp;

		y_step = next_step;
		next_step = ((y + dy) >> MLIB_SHIFT) - (y >> MLIB_SHIFT);

		mlib_u16 tail = dp[width * 3];

		if (y_step == 0) {
			__m128i y0, y1, dd;

			ddy = _mm_set1_epi16(y);
			rdy = _mm_sub_epi16(mask_ffff, ddy);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < width8; i++) {
				y0 = buff0[i];
				y1 = buff1[i];
				y0 = _mm_mulhi_epu16(rdy, y0);
				y1 = _mm_mulhi_epu16(ddy, y1);
				dd = _mm_add_epi16(y0, y1);
				dd = _mm_adds_epu16(dd, d_rnd);
				dd = XOR(dd);
				_mm_storel_epi64((void *)tdp, dd);
				tdp += 3;
				_mm_storel_epi64((void *)tdp,
				    _mm_unpackhi_epi64(dd, dd));
				tdp += 3;
			}

			if (i * 2 < width) {
				y0 = buff0[i];
				y1 = buff1[i];
				y0 = _mm_mulhi_epu16(rdy, y0);
				y1 = _mm_mulhi_epu16(ddy, y1);
				dd = _mm_add_epi16(y0, y1);
				dd = _mm_adds_epu16(dd, d_rnd);
				dd = XOR(dd);
				_mm_storel_epi64((void *)tdp, dd);
			}

		} else if (y_step == 1) {
			__m128i *buffT;
			__m128i ddx, rdx;

			ddx = ddx_start;
			ddy = _mm_set1_epi16(y);
			rdy = _mm_sub_epi16(mask_ffff, ddy);
			x = GetElemSubStruct(current, srcX) & MLIB_MASK;

			buffT = buff0;
			buff0 = buff1;
			buff1 = buffT;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < width8; i++) {
				mlib_u16 *tsp2 = tsp + src_stride;
				mlib_u16 *sp0 =
				    tsp2 + 3 * (x >> MLIB_SHIFT);
				mlib_u16 *sp1 =
				    tsp2 + 3 * ((x + dx) >> MLIB_SHIFT);
				__m128i v2, v3, y0, y1, z0, z1, dd;
				__m128i s2, s3;

				s2 = _mm_unpacklo_epi64(_mm_loadl_epi64(
				    (void *)sp0),
				    _mm_loadl_epi64((void *)sp1));
				s3 = _mm_unpacklo_epi64(_mm_loadl_epi64(
				    (void *)(sp0 + 3)),
				    _mm_loadl_epi64((void *)(sp1 + 3)));

				rdx = _mm_sub_epi16(mask_ffff, ddx);

				v2 = _mm_mulhi_epu16(XOR(s2), rdx);
				v3 = _mm_mulhi_epu16(XOR(s3), ddx);

				y0 = buff0[i];
				y1 = _mm_add_epi16(v2, v3);
				buff1[i] = y1;

				z0 = _mm_mulhi_epu16(rdy, y0);
				z1 = _mm_mulhi_epu16(ddy, y1);
				dd = _mm_add_epi16(z0, z1);
				dd = _mm_adds_epu16(dd, d_rnd);
				dd = XOR(dd);
				_mm_storel_epi64((void *)tdp, dd);
				tdp += 3;
				_mm_storel_epi64((void *)tdp,
				    _mm_unpackhi_epi64(dd, dd));
				tdp += 3;

				x += 2 * dx;
				ddx = _mm_add_epi16(ddx, dx_step);
			}

			if (i * 2 < width) {
				mlib_u16 *tsp2 = tsp + src_stride;
				mlib_u16 *sp0 =
				    tsp2 + 3 * (x >> MLIB_SHIFT);
				mlib_u16 *sp1 =
				    tsp2 + 3 * ((x + dx) >> MLIB_SHIFT);
				__m128i v2, v3, y0, y1, z0, z1, dd;
				__m128i s2, s3;

				s2 = _mm_unpacklo_epi64(_mm_loadl_epi64(
				    (void *)sp0),
				    _mm_loadl_epi64((void *)sp1));
				s3 = _mm_unpacklo_epi64(_mm_loadl_epi64(
				    (void *)(sp0 + 3)),
				    _mm_loadl_epi64((void *)(sp1 + 3)));

				rdx = _mm_sub_epi16(mask_ffff, ddx);

				v2 = _mm_mulhi_epu16(XOR(s2), rdx);
				v3 = _mm_mulhi_epu16(XOR(s3), ddx);

				y0 = buff0[i];
				y1 = _mm_add_epi16(v2, v3);
				buff1[i] = y1;

				z0 = _mm_mulhi_epu16(rdy, y0);
				z1 = _mm_mulhi_epu16(ddy, y1);
				dd = _mm_add_epi16(z0, z1);
				dd = _mm_adds_epu16(dd, d_rnd);
				dd = XOR(dd);
				_mm_storel_epi64((void *)tdp, dd);
			}

		} else {
			__m128i ddx, rdx;

			ddx = ddx_start;
			ddy = _mm_set1_epi16(y);
			rdy = _mm_sub_epi16(mask_ffff, ddy);
			x = GetElemSubStruct(current, srcX) & MLIB_MASK;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < width8; i++) {
				__m128i v0, v1, v2, v3, y0, y1, z0, z1,
				    dd;
				__m128i s0, s1, s2, s3;
				mlib_u16 *sp0 =
				    tsp + 3 * (x >> MLIB_SHIFT);
				mlib_u16 *sp1 =
				    tsp + 3 * ((x + dx) >> MLIB_SHIFT);

				s0 = _mm_unpacklo_epi64(_mm_loadl_epi64(
				    (void *)sp0),
				    _mm_loadl_epi64((void *)sp1));
				s1 = _mm_unpacklo_epi64(_mm_loadl_epi64(
				    (void *)(sp0 + 3)),
				    _mm_loadl_epi64((void *)(sp1 + 3)));
				s2 = _mm_unpacklo_epi64(_mm_loadl_epi64(
				    (void *)(sp0 + src_stride)),
				    _mm_loadl_epi64((void *)(sp1 +
				    src_stride)));
				s3 = _mm_unpacklo_epi64(_mm_loadl_epi64(
				    (void *)(sp0 + src_stride + 3)),
				    _mm_loadl_epi64((void *)(sp1 +
				    src_stride + 3)));

				rdx = _mm_sub_epi16(mask_ffff, ddx);

				v0 = _mm_mulhi_epu16(XOR(s0), rdx);
				v1 = _mm_mulhi_epu16(XOR(s1), ddx);
				v2 = _mm_mulhi_epu16(XOR(s2), rdx);
				v3 = _mm_mulhi_epu16(XOR(s3), ddx);

				y0 = _mm_add_epi16(v0, v1);
				y1 = _mm_add_epi16(v2, v3);
				buff0[i] = y0;
				buff1[i] = y1;

				z0 = _mm_mulhi_epu16(rdy, y0);
				z1 = _mm_mulhi_epu16(ddy, y1);
				dd = _mm_add_epi16(z0, z1);
				dd = _mm_adds_epu16(dd, d_rnd);
				dd = XOR(dd);
				_mm_storel_epi64((void *)tdp, dd);
				tdp += 3;
				_mm_storel_epi64((void *)tdp,
				    _mm_unpackhi_epi64(dd, dd));
				tdp += 3;

				x += 2 * dx;
				ddx = _mm_add_epi16(ddx, dx_step);
			}

			if (i * 2 < width) {
				__m128i v0, v1, v2, v3, y0, y1, z0, z1,
				    dd;
				__m128i s0, s1, s2, s3;
				mlib_u16 *sp0 =
				    tsp + 3 * (x >> MLIB_SHIFT);
				mlib_u16 *sp1 =
				    tsp + 3 * ((x + dx) >> MLIB_SHIFT);

				s0 = _mm_unpacklo_epi64(_mm_loadl_epi64(
				    (void *)sp0),
				    _mm_loadl_epi64((void *)sp1));
				s1 = _mm_unpacklo_epi64(_mm_loadl_epi64(
				    (void *)(sp0 + 3)),
				    _mm_loadl_epi64((void *)(sp1 + 3)));
				s2 = _mm_unpacklo_epi64(_mm_loadl_epi64(
				    (void *)(sp0 + src_stride)),
				    _mm_loadl_epi64((void *)(sp1 +
				    src_stride)));
				s3 = _mm_unpacklo_epi64(_mm_loadl_epi64(
				    (void *)(sp0 + src_stride + 3)),
				    _mm_loadl_epi64((void *)(sp1 +
				    src_stride + 3)));

				rdx = _mm_sub_epi16(mask_ffff, ddx);

				v0 = _mm_mulhi_epu16(XOR(s0), rdx);
				v1 = _mm_mulhi_epu16(XOR(s1), ddx);
				v2 = _mm_mulhi_epu16(XOR(s2), rdx);
				v3 = _mm_mulhi_epu16(XOR(s3), ddx);

				y0 = _mm_add_epi16(v0, v1);
				y1 = _mm_add_epi16(v2, v3);
				buff0[i] = y0;
				buff1[i] = y1;

				z0 = _mm_mulhi_epu16(rdy, y0);
				z1 = _mm_mulhi_epu16(ddy, y1);
				dd = _mm_add_epi16(z0, z1);
				dd = _mm_adds_epu16(dd, d_rnd);
				dd = XOR(dd);
				_mm_storel_epi64((void *)tdp, dd);
			}
		}

		dp[width * 3] = tail;

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
mlib_s_ImageZoom_BL_U16_4(
    mlib_work_image * param)
{
	VARIABLE(mlib_u16)
	__m128i buff_arr[BUFF_SIZE / 16], *buff0, *buff1;
	void *buff = buff_arr;
	mlib_u16 *tdp, *pbuff;
	mlib_s32 k, width8, widthbuff, size, y_step, next_step = 2;
	__m128i ddx_start, ddx, ddy, rdy, dx_step;
	__m128i mzero, mask_ffff, mask_8000;
	__m128i d_rnd = _mm_set1_epi16(RND_VAL);

	width8 = width >> 1;
	widthbuff = (width * 4 + 7) / 8;

	size = 2 * widthbuff * sizeof (__m128i);
	if (size > BUFF_SIZE) {
		buff = __mlib_malloc(size);
		if (buff == NULL)
			return (MLIB_FAILURE);
	}

	buff0 = buff;
	buff1 = buff0 + widthbuff;

	mzero = _mm_setzero_si128();
	mask_ffff = _mm_set1_epi16(0xFFFF);
	mask_8000 = _mm_set1_epi16(0x8000);

/* calculate dx for all row */
	x = GetElemSubStruct(current, srcX) & MLIB_MASK;
	for (i = 0; i < 2; i++) {
		((mlib_s16 *)&ddx_start)[4 * i] = x;
		((mlib_s16 *)&ddx_start)[4 * i + 1] = x;
		((mlib_s16 *)&ddx_start)[4 * i + 2] = x;
		((mlib_s16 *)&ddx_start)[4 * i + 3] = x;
		x += dx;
	}
	dx_step = _mm_set1_epi16(2 * dx);

	for (j = 0; j < height; j++) {
		tdp = dp;

		y_step = next_step;
		next_step = ((y + dy) >> MLIB_SHIFT) - (y >> MLIB_SHIFT);

		if (y_step == 0) {
			__m128i y0, y1, dd;

			ddy = _mm_set1_epi16(y);
			rdy = _mm_sub_epi16(mask_ffff, ddy);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < width8; i++) {
				y0 = buff0[i];
				y1 = buff1[i];
				y0 = _mm_mulhi_epu16(rdy, y0);
				y1 = _mm_mulhi_epu16(ddy, y1);
				dd = _mm_add_epi16(y0, y1);
				dd = _mm_adds_epu16(dd, d_rnd);
				_mm_storeu_si128((void *)tdp, XOR(dd));
				tdp += 8;
			}

			if (i * 2 < width) {
				y0 = buff0[i];
				y1 = buff1[i];
				y0 = _mm_mulhi_epu16(rdy, y0);
				y1 = _mm_mulhi_epu16(ddy, y1);
				dd = _mm_add_epi16(y0, y1);
				dd = _mm_adds_epu16(dd, d_rnd);
				_mm_storel_epi64((void *)tdp, XOR(dd));
			}

		} else if (y_step == 1) {
			__m128i *buffT;
			__m128i ddx, rdx;

			ddx = ddx_start;
			ddy = _mm_set1_epi16(y);
			rdy = _mm_sub_epi16(mask_ffff, ddy);
			x = GetElemSubStruct(current, srcX) & MLIB_MASK;

			buffT = buff0;
			buff0 = buff1;
			buff1 = buffT;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < width8; i++) {
				mlib_u16 *tsp2 = tsp + src_stride;
				mlib_u16 *sp0 =
				    tsp2 + 4 * (x >> MLIB_SHIFT);
				mlib_u16 *sp1 =
				    tsp2 + 4 * ((x + dx) >> MLIB_SHIFT);
				__m128i v2, v3, y0, y1, z0, z1, dd;
				__m128i s2, s3;

				s2 = _mm_unpacklo_epi64(_mm_loadl_epi64(
				    (void *)sp0),
				    _mm_loadl_epi64((void *)sp1));
				s3 = _mm_unpacklo_epi64(_mm_loadl_epi64(
				    (void *)(sp0 + 4)),
				    _mm_loadl_epi64((void *)(sp1 + 4)));

				rdx = _mm_sub_epi16(mask_ffff, ddx);

				v2 = _mm_mulhi_epu16(XOR(s2), rdx);
				v3 = _mm_mulhi_epu16(XOR(s3), ddx);

				y0 = buff0[i];
				y1 = _mm_add_epi16(v2, v3);
				buff1[i] = y1;

				z0 = _mm_mulhi_epu16(rdy, y0);
				z1 = _mm_mulhi_epu16(ddy, y1);
				dd = _mm_add_epi16(z0, z1);
				dd = _mm_adds_epu16(dd, d_rnd);
				_mm_storeu_si128((void *)tdp, XOR(dd));
				tdp += 8;

				x += 2 * dx;
				ddx = _mm_add_epi16(ddx, dx_step);
			}

			if (i * 2 < width) {
				mlib_u16 *tsp2 = tsp + src_stride;
				mlib_u16 *sp0 =
				    tsp2 + 4 * (x >> MLIB_SHIFT);
				mlib_u16 *sp1 =
				    tsp2 + 4 * ((x + dx) >> MLIB_SHIFT);
				__m128i v2, v3, y0, y1, z0, z1, dd;
				__m128i s2, s3;

				s2 = _mm_unpacklo_epi64(_mm_loadl_epi64(
				    (void *)sp0),
				    _mm_loadl_epi64((void *)sp1));
				s3 = _mm_unpacklo_epi64(_mm_loadl_epi64(
				    (void *)(sp0 + 4)),
				    _mm_loadl_epi64((void *)(sp1 + 4)));

				rdx = _mm_sub_epi16(mask_ffff, ddx);

				v2 = _mm_mulhi_epu16(XOR(s2), rdx);
				v3 = _mm_mulhi_epu16(XOR(s3), ddx);

				y0 = buff0[i];
				y1 = _mm_add_epi16(v2, v3);
				buff1[i] = y1;

				z0 = _mm_mulhi_epu16(rdy, y0);
				z1 = _mm_mulhi_epu16(ddy, y1);
				dd = _mm_add_epi16(z0, z1);
				dd = _mm_adds_epu16(dd, d_rnd);
				_mm_storel_epi64((void *)tdp, XOR(dd));
			}

		} else {
			__m128i ddx, rdx;

			ddx = ddx_start;
			ddy = _mm_set1_epi16(y);
			rdy = _mm_sub_epi16(mask_ffff, ddy);
			x = GetElemSubStruct(current, srcX) & MLIB_MASK;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < width8; i++) {
				__m128i v0, v1, v2, v3, y0, y1, z0, z1,
				    dd;
				__m128i s0, s1, s2, s3;
				mlib_u16 *sp0 =
				    tsp + 4 * (x >> MLIB_SHIFT);
				mlib_u16 *sp1 =
				    tsp + 4 * ((x + dx) >> MLIB_SHIFT);

				s0 = _mm_unpacklo_epi64(_mm_loadl_epi64(
				    (void *)sp0),
				    _mm_loadl_epi64((void *)sp1));
				s1 = _mm_unpacklo_epi64(_mm_loadl_epi64(
				    (void *)(sp0 + 4)),
				    _mm_loadl_epi64((void *)(sp1 + 4)));
				s2 = _mm_unpacklo_epi64(_mm_loadl_epi64(
				    (void *)(sp0 + src_stride)),
				    _mm_loadl_epi64((void *)(sp1 +
				    src_stride)));
				s3 = _mm_unpacklo_epi64(_mm_loadl_epi64(
				    (void *)(sp0 + src_stride + 4)),
				    _mm_loadl_epi64((void *)(sp1 +
				    src_stride + 4)));

				rdx = _mm_sub_epi16(mask_ffff, ddx);

				v0 = _mm_mulhi_epu16(XOR(s0), rdx);
				v1 = _mm_mulhi_epu16(XOR(s1), ddx);
				v2 = _mm_mulhi_epu16(XOR(s2), rdx);
				v3 = _mm_mulhi_epu16(XOR(s3), ddx);

				y0 = _mm_add_epi16(v0, v1);
				y1 = _mm_add_epi16(v2, v3);
				buff0[i] = y0;
				buff1[i] = y1;

				z0 = _mm_mulhi_epu16(rdy, y0);
				z1 = _mm_mulhi_epu16(ddy, y1);
				dd = _mm_add_epi16(z0, z1);
				dd = _mm_adds_epu16(dd, d_rnd);
				_mm_storeu_si128((void *)tdp, XOR(dd));
				tdp += 8;

				x += 2 * dx;
				ddx = _mm_add_epi16(ddx, dx_step);
			}

			if (i * 2 < width) {
				__m128i v0, v1, v2, v3, y0, y1, z0, z1,
				    dd;
				__m128i s0, s1, s2, s3;
				mlib_u16 *sp0 =
				    tsp + 4 * (x >> MLIB_SHIFT);
				mlib_u16 *sp1 =
				    tsp + 4 * ((x + dx) >> MLIB_SHIFT);

				s0 = _mm_unpacklo_epi64(_mm_loadl_epi64(
				    (void *)sp0),
				    _mm_loadl_epi64((void *)sp1));
				s1 = _mm_unpacklo_epi64(_mm_loadl_epi64(
				    (void *)(sp0 + 4)),
				    _mm_loadl_epi64((void *)(sp1 + 4)));
				s2 = _mm_unpacklo_epi64(_mm_loadl_epi64(
				    (void *)(sp0 + src_stride)),
				    _mm_loadl_epi64((void *)(sp1 +
				    src_stride)));
				s3 = _mm_unpacklo_epi64(_mm_loadl_epi64(
				    (void *)(sp0 + src_stride + 4)),
				    _mm_loadl_epi64((void *)(sp1 +
				    src_stride + 4)));

				rdx = _mm_sub_epi16(mask_ffff, ddx);

				v0 = _mm_mulhi_epu16(XOR(s0), rdx);
				v1 = _mm_mulhi_epu16(XOR(s1), ddx);
				v2 = _mm_mulhi_epu16(XOR(s2), rdx);
				v3 = _mm_mulhi_epu16(XOR(s3), ddx);

				y0 = _mm_add_epi16(v0, v1);
				y1 = _mm_add_epi16(v2, v3);
				buff0[i] = y0;
				buff1[i] = y1;

				z0 = _mm_mulhi_epu16(rdy, y0);
				z1 = _mm_mulhi_epu16(ddy, y1);
				dd = _mm_add_epi16(z0, z1);
				dd = _mm_adds_epu16(dd, d_rnd);
				_mm_storel_epi64((void *)tdp, XOR(dd));
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
