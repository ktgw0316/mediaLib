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

#pragma ident	"@(#)mlib_s_ImageZoom_BC_U16.c	9.5	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_ImageZoomTranslate(), internal pixels.
 *
 */

#include <mlib_s_ImageZoom.h>

/* *********************************************************** */

#define	XOR(x)	_mm_xor_si128(x, mask_8000)

/* *********************************************************** */

#define	INIT_XLINE()                                            \
	x = GetElemSubStruct(current, srcX) & MLIB_MASK;        \
	ddy = _mm_set1_epi16((y >> 1) & 0x7FFF);                \
	rdy = _mm_sub_epi16(mask_8000, ddy)

/* *********************************************************** */

#define	COEFF_D64_U8(r)                                          \
	fpos = (r >> (MLIB_SHIFT - 9)) & 0x1ff;                  \
	d##r##f0 =                                               \
	_mm_loadl_epi64((void *)((mlib_d64 *)filter1 + fpos));   \
	d##r##f0 =                                               \
	_mm_unpacklo_epi16(d##r##f0, d##r##f0);                  \
	d##r##f2 =                                               \
	_mm_unpackhi_epi32(d##r##f0, d##r##f0);                  \
	d##r##f0 =                                               \
	_mm_unpacklo_epi32(d##r##f0, d##r##f0);                  \
	d##r##f1 =                                               \
	_mm_unpackhi_epi64(d##r##f0, d##r##f0);                  \
	d##r##f0 =                                               \
	_mm_unpacklo_epi64(d##r##f0, d##r##f0);                  \
	d##r##f3 =                                               \
	_mm_unpackhi_epi64(d##r##f2, d##r##f2);                  \
	d##r##f2 =                                               \
	_mm_unpacklo_epi64(d##r##f2, d##r##f2);

/* *********************************************************** */

mlib_status
mlib_s_ImageZoom_BC_U16_1(
    mlib_work_image * param)
{
	VARIABLE(mlib_u16)
	void *filter1 = GetElemStruct(filter1);
	mlib_s32 *prow;
	mlib_s32 fpos, cx;
	__m128i *dfPtr, dyf0, dyf1, dyf2, dyf3;
	__m128i *buff, *buff_arr[8], *buff0, *buff1, *buff2, *buff3, *buffx,
	    *pbuff;
	__m128i mask_8000 = _mm_set1_epi16(0x8000);
	mlib_s32 width4, width8, k, bind = 0, y_step = 4;
	mlib_s32 nchan = param->channels;

	width4 = (width + 3) / 4;
	width8 = (width + 7) / 8;

	buff = __mlib_malloc(width8 * 8 * sizeof (__m128i));
	if (buff == NULL)
		return (MLIB_FAILURE);
	for (k = 0; k < 4; k++)
		buff_arr[k] = buff + k * width8;
	for (k = 0; k < 4; k++)
		buff_arr[4 + k] = buff_arr[k];
	buffx = buff + 4 * width8;

	x = GetElemSubStruct(current, srcX) & MLIB_MASK;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < width4 * 2; i += 2) {
		__m128i a0, a1, a2, a3, b0, b1;

		fpos = (x >> (MLIB_SHIFT - 9)) & 0x1ff;
		x += dx;
		a0 = _mm_loadl_epi64((void *)((mlib_d64 *)filter1 + fpos));
		fpos = (x >> (MLIB_SHIFT - 9)) & 0x1ff;
		x += dx;
		a1 = _mm_loadl_epi64((void *)((mlib_d64 *)filter1 + fpos));
		fpos = (x >> (MLIB_SHIFT - 9)) & 0x1ff;
		x += dx;
		a2 = _mm_loadl_epi64((void *)((mlib_d64 *)filter1 + fpos));
		fpos = (x >> (MLIB_SHIFT - 9)) & 0x1ff;
		x += dx;
		a3 = _mm_loadl_epi64((void *)((mlib_d64 *)filter1 + fpos));

		b0 = _mm_unpacklo_epi16(a0, a2);
		b1 = _mm_unpacklo_epi16(a1, a3);

		buffx[i] = _mm_unpacklo_epi16(b0, b1);
		buffx[i + 1] = _mm_unpackhi_epi16(b0, b1);
	}

	sp -= src_stride + nchan;
	tsp = sp;

	for (j = 0; j < height; j++) {

	    if (y_step >= 4) {
			y_step = 4;
			tsp = sp + (y >> MLIB_SHIFT) * src_stride;
		} else {
			bind += y_step;
			if (bind >= 4)
				bind -= 4;
		}

		for (k = 4 - y_step; k < 4; k++) {
			x = GetElemSubStruct(current, srcX) & MLIB_MASK;
			pbuff = buff_arr[bind + k];

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (int m = 0, i = 0; i < width4; m += 2, i++) {
				__m128i s0, s1, t0, t1, r0, r1, r2;
				mlib_u16 *sp0 = tsp + (x >> MLIB_SHIFT);
				mlib_u16 *sp1 =
				    tsp + ((x + dx) >> MLIB_SHIFT);
				mlib_u16 *sp2 =
				    tsp + ((x + 2 * dx) >> MLIB_SHIFT);
				mlib_u16 *sp3 =
				    tsp + ((x + 3 * dx) >> MLIB_SHIFT);

				t0 = _mm_unpacklo_epi16(_mm_loadl_epi64(
				    (void *)sp0),
				    _mm_loadl_epi64((void *)sp2));
				t1 = _mm_unpacklo_epi16(_mm_loadl_epi64(
				    (void *)sp1),
				    _mm_loadl_epi64((void *)sp3));
				s0 = _mm_unpacklo_epi16(t0, t1);
				s1 = _mm_unpackhi_epi16(t0, t1);
				r0 = _mm_mulhi_epi16(XOR(s0),
				    buffx[m]);
				r1 = _mm_mulhi_epi16(XOR(s1),
				    buffx[m + 1]);
				r0 = _mm_add_epi16(r0, r1);
				r1 = _mm_unpackhi_epi64(r0, r0);
				r2 = _mm_add_epi16(r1, r0);
				_mm_storel_epi64((void *)((mlib_d64 *)
				    pbuff + i), r2);
				x += 4 * dx;
			}

			tsp += src_stride;
		}

		buff0 = buff_arr[bind];
		buff1 = buff_arr[bind + 1];
		buff2 = buff_arr[bind + 2];
		buff3 = buff_arr[bind + 3];

		mlib_u16 *tdp = dp;

		COEFF_D64_U8(y)

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < (width >> 3); i++) {
			__m128i s0, s1, s2, s3, r0, r1, r2, r3, t0, t1, dd;

			s0 = buff0[i];
			r0 = _mm_mulhi_epi16(s0, dyf0);
			s3 = buff3[i];
			r3 = _mm_mulhi_epi16(s3, dyf3);
			t0 = _mm_add_epi16(r0, r3);

			s1 = buff1[i];
			r1 = _mm_mulhi_epi16(s1, dyf1);
			s2 = buff2[i];
			r2 = _mm_mulhi_epi16(s2, dyf2);
			t1 = _mm_add_epi16(r1, r2);

			dd = _mm_add_epi16(t0, t1);
			dd = _mm_add_epi16(dd, dd);
			dd = XOR(_mm_adds_epi16(dd, dd));

			_mm_storeu_si128((void *)tdp, dd);
			tdp += 8;
		}

		if (width & 7) {
			__m128i s0, s1, s2, s3, r0, r1, r2, r3, dd;

			s0 = buff0[i];
			s1 = buff1[i];
			s2 = buff2[i];
			s3 = buff3[i];

			r0 = _mm_mulhi_epi16(s0, dyf0);
			r1 = _mm_mulhi_epi16(s1, dyf1);
			r2 = _mm_mulhi_epi16(s2, dyf2);
			r3 = _mm_mulhi_epi16(s3, dyf3);

			dd = _mm_add_epi16(_mm_add_epi16(r1, r2),
			    _mm_add_epi16(r0, r3));
			dd = _mm_add_epi16(dd, dd);
			dd = XOR(_mm_adds_epi16(dd, dd));

			for (i = 0; i < (width & 7); i++) {
				tdp[i] = ((mlib_u16 *)&dd)[i];
			}
		}

		y_step = ((y + dy) >> MLIB_SHIFT) - (y >> MLIB_SHIFT);
		y += dy;
		dp += dst_stride;
	}

	__mlib_free(buff);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_s_ImageZoom_BC_U16_2(
    mlib_work_image * param)
{
	VARIABLE(mlib_u16)
	void *filter1 = GetElemStruct(filter1);
	mlib_s32 *prow;
	mlib_s32 fpos, cx;
	__m128i *dfPtr, dyf0, dyf1, dyf2, dyf3;
	__m128i *buff, *buff_arr[8], *buff0, *buff1, *buff2, *buff3, *buffx,
	    *pbuff;
	__m128i mask_8000 = _mm_set1_epi16(0x8000);
	mlib_s32 width4, width8, k, bind = 0, y_step = 4;
	mlib_s32 nchan = param->channels;

	width <<= 1;
	width4 = (width + 3) / 4;
	width8 = (width + 7) / 8;

	buff = __mlib_malloc(width8 * 8 * sizeof (__m128i));
	if (buff == NULL)
		return (MLIB_FAILURE);
	for (k = 0; k < 4; k++)
		buff_arr[k] = buff + k * width8;
	for (k = 0; k < 4; k++)
		buff_arr[4 + k] = buff_arr[k];
	buffx = buff + 4 * width8;

	x = GetElemSubStruct(current, srcX) & MLIB_MASK;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < width4 * 2; i += 2) {
		__m128i a0, a1, a2, a3, b0, b1;

		fpos = (x >> (MLIB_SHIFT - 9)) & 0x1ff;
		x += dx;
		a0 = _mm_loadl_epi64((void *)((mlib_d64 *)filter1 + fpos));
		fpos = (x >> (MLIB_SHIFT - 9)) & 0x1ff;
		x += dx;
		a1 = _mm_loadl_epi64((void *)((mlib_d64 *)filter1 + fpos));
		fpos = (x >> (MLIB_SHIFT - 9)) & 0x1ff;
		x += dx;
		a2 = _mm_loadl_epi64((void *)((mlib_d64 *)filter1 + fpos));
		fpos = (x >> (MLIB_SHIFT - 9)) & 0x1ff;
		x += dx;
		a3 = _mm_loadl_epi64((void *)((mlib_d64 *)filter1 + fpos));

		b0 = _mm_unpacklo_epi16(a0, a2);
		b1 = _mm_unpacklo_epi16(a1, a3);

		buffx[i] = _mm_unpacklo_epi16(b0, b1);
		buffx[i + 1] = _mm_unpackhi_epi16(b0, b1);
	}

	sp -= src_stride + nchan;
	tsp = sp;

	for (j = 0; j < height; j++) {
		COEFF_D64_U8(y)

	    if (y_step >= 4) {
			y_step = 4;
			tsp = sp + (y >> MLIB_SHIFT) * src_stride;
		} else {
			bind += y_step;
			if (bind >= 4)
				bind -= 4;
		}

		for (k = 4 - y_step; k < 4; k++) {
			x = GetElemSubStruct(current, srcX) & MLIB_MASK;
			pbuff = buff_arr[bind + k];

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (int m = 0, i = 0; i < width8; m += 2, i++) {
				__m128i s0, s1, s2, s3, t0, t1, t2, t3,
				    f0, f1, f2, f3;
				mlib_u16 *sp0 =
				    tsp + 2 * (x >> MLIB_SHIFT);
				mlib_u16 *sp1 =
				    tsp + 2 * ((x + dx) >> MLIB_SHIFT);
				mlib_u16 *sp2 =
				    tsp + 2 * ((x +
				    2 * dx) >> MLIB_SHIFT);
				mlib_u16 *sp3 =
				    tsp + 2 * ((x +
				    3 * dx) >> MLIB_SHIFT);
#if 0
				t0 = _mm_unpacklo_epi32(_mm_loadl_epi64(
				    (void *)sp0),
				    _mm_loadl_epi64((void *)sp1));
				t1 = _mm_unpacklo_epi32(_mm_loadl_epi64(
				    (void *)sp2),
				    _mm_loadl_epi64((void *)sp3));
				t2 = _mm_unpacklo_epi32(_mm_loadl_epi64(
				    (void *)(sp0 + 4)),
				    _mm_loadl_epi64((void *)(sp1 + 4)));
				t3 = _mm_unpacklo_epi32(_mm_loadl_epi64(
				    (void *)(sp2 + 4)),
				    _mm_loadl_epi64((void *)(sp3 + 4)));
#else
				s0 = _mm_loadu_si128((void *)sp0);
				s1 = _mm_loadu_si128((void *)sp1);
				s2 = _mm_loadu_si128((void *)sp2);
				s3 = _mm_loadu_si128((void *)sp3);
				t0 = _mm_unpacklo_epi32(s0, s1);
				t1 = _mm_unpacklo_epi32(s2, s3);
				t2 = _mm_unpackhi_epi32(s0, s1);
				t3 = _mm_unpackhi_epi32(s2, s3);
#endif
				s0 = _mm_unpacklo_epi64(t0, t1);
				s1 = _mm_unpackhi_epi64(t0, t1);
				s2 = _mm_unpacklo_epi64(t2, t3);
				s3 = _mm_unpackhi_epi64(t2, t3);

				t0 = buffx[m];
				t1 = buffx[m + 1];
				f0 = _mm_unpacklo_epi16(t0, t0);
				f1 = _mm_unpackhi_epi16(t0, t0);
				f2 = _mm_unpacklo_epi16(t1, t1);
				f3 = _mm_unpackhi_epi16(t1, t1);

				s0 = _mm_mulhi_epi16(XOR(s0), f0);
				s1 = _mm_mulhi_epi16(XOR(s1), f1);
				s2 = _mm_mulhi_epi16(XOR(s2), f2);
				s3 = _mm_mulhi_epi16(XOR(s3), f3);

				pbuff[i] =
				    _mm_add_epi16(_mm_add_epi16(s1, s2),
				    _mm_add_epi16(s0, s3));
				x += 4 * dx;
			}

			tsp += src_stride;
		}

		buff0 = buff_arr[bind];
		buff1 = buff_arr[bind + 1];
		buff2 = buff_arr[bind + 2];
		buff3 = buff_arr[bind + 3];

		mlib_u16 *tdp = dp;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < (width >> 3); i++) {
			__m128i s0, s1, s2, s3, r0, r1, r2, r3, dd;

			s0 = buff0[i];
			s1 = buff1[i];
			s2 = buff2[i];
			s3 = buff3[i];

			r0 = _mm_mulhi_epi16(s0, dyf0);
			r1 = _mm_mulhi_epi16(s1, dyf1);
			r2 = _mm_mulhi_epi16(s2, dyf2);
			r3 = _mm_mulhi_epi16(s3, dyf3);

			dd = _mm_add_epi16(_mm_add_epi16(r1, r2),
			    _mm_add_epi16(r0, r3));
			dd = _mm_add_epi16(dd, dd);
			dd = XOR(_mm_adds_epi16(dd, dd));

			_mm_storeu_si128((void *)tdp, dd);
			tdp += 8;
		}

		if (width & 7) {
			__m128i s0, s1, s2, s3, r0, r1, r2, r3, dd;

			s0 = buff0[i];
			s1 = buff1[i];
			s2 = buff2[i];
			s3 = buff3[i];

			r0 = _mm_mulhi_epi16(s0, dyf0);
			r1 = _mm_mulhi_epi16(s1, dyf1);
			r2 = _mm_mulhi_epi16(s2, dyf2);
			r3 = _mm_mulhi_epi16(s3, dyf3);

			dd = _mm_add_epi16(_mm_add_epi16(r1, r2),
			    _mm_add_epi16(r0, r3));
			dd = _mm_add_epi16(dd, dd);
			dd = XOR(_mm_adds_epi16(dd, dd));

			for (i = 0; i < (width & 7); i++) {
				tdp[i] = ((mlib_u16 *)&dd)[i];
			}
		}

		y_step = ((y + dy) >> MLIB_SHIFT) - (y >> MLIB_SHIFT);
		y += dy;
		dp += dst_stride;
	}

	__mlib_free(buff);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_s_ImageZoom_BC_U16_3(
    mlib_work_image * param)
{
	VARIABLE(mlib_u16)
	void *filter1 = GetElemStruct(filter1);
	mlib_s32 *prow;
	mlib_s32 fpos, cx;
	__m128i *dfPtr, dyf0, dyf1, dyf2, dyf3;
	__m128i *buff, *buff_arr[8], *buff0, *buff1, *buff2, *buff3, *buffx,
	    *pbuff;
	__m128i mask_8000 = _mm_set1_epi16(0x8000);
	mlib_s32 width8, k, bind = 0, y_step = 4;
	mlib_s32 nchan = param->channels;

	width8 = (4 * width + 7) / 8;

	buff = __mlib_malloc(width8 * 8 * sizeof (__m128i));
	if (buff == NULL)
		return (MLIB_FAILURE);
	for (k = 0; k < 4; k++)
		buff_arr[k] = buff + k * width8;
	for (k = 0; k < 4; k++)
		buff_arr[4 + k] = buff_arr[k];
	buffx = buff + 4 * width8;

	x = GetElemSubStruct(current, srcX) & MLIB_MASK;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < (width8 << 2); i += 4) {
		__m128i *p_flt0, *p_flt1;
		__m128i a0, a1, b0, b1;

		fpos = (x >> (MLIB_SHIFT - 9)) & 0x1ff;
		x += dx;
		a0 = _mm_loadl_epi64((void *)((mlib_d64 *)filter1 + fpos));
		a0 = _mm_unpacklo_epi16(a0, a0);
		a1 = _mm_unpackhi_epi32(a0, a0);
		a0 = _mm_unpacklo_epi32(a0, a0);

		fpos = (x >> (MLIB_SHIFT - 9)) & 0x1ff;
		x += dx;
		b0 = _mm_loadl_epi64((void *)((mlib_d64 *)filter1 + fpos));
		b0 = _mm_unpacklo_epi16(b0, b0);
		b1 = _mm_unpackhi_epi32(b0, b0);
		b0 = _mm_unpacklo_epi32(b0, b0);

		buffx[i] = _mm_unpacklo_epi64(a0, b0);
		buffx[i + 1] = _mm_unpackhi_epi64(a0, b0);
		buffx[i + 2] = _mm_unpacklo_epi64(a1, b1);
		buffx[i + 3] = _mm_unpackhi_epi64(a1, b1);
	}

	sp -= src_stride + nchan;
	tsp = sp;

	for (j = 0; j < height; j++) {
		COEFF_D64_U8(y)

		mlib_u16 *tdp = dp;
		mlib_u16 tail = dp[width * 3];

	    if (y_step >= 4) {
			y_step = 4;
			tsp = sp + (y >> MLIB_SHIFT) * src_stride;
		} else {
			bind += y_step;
			if (bind >= 4)
				bind -= 4;
		}

		for (k = 4 - y_step; k < 4; k++) {
			x = GetElemSubStruct(current, srcX) & MLIB_MASK;
			pbuff = buff_arr[bind + k];

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (int m = 0, i = 0; i < width8; m += 4, i++) {
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
				    (void *)(sp0 + 6)),
				    _mm_loadl_epi64((void *)(sp1 + 6)));
				s3 = _mm_unpacklo_epi64(_mm_loadl_epi64(
				    (void *)(sp0 + 9)),
				    _mm_loadl_epi64((void *)(sp1 + 9)));

				s0 = _mm_mulhi_epi16(XOR(s0), buffx[m]);
				s1 = _mm_mulhi_epi16(XOR(s1), buffx[m + 1]);
				s2 = _mm_mulhi_epi16(XOR(s2), buffx[m + 2]);
				s3 = _mm_mulhi_epi16(XOR(s3), buffx[m + 3]);

				pbuff[i] =
				    _mm_add_epi16(_mm_add_epi16(s1, s2),
				    _mm_add_epi16(s0, s3));
				x += 2 * dx;
			}

			tsp += src_stride;
		}

		buff0 = buff_arr[bind];
		buff1 = buff_arr[bind + 1];
		buff2 = buff_arr[bind + 2];
		buff3 = buff_arr[bind + 3];

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < (width >> 1); i++) {
			__m128i s0, s1, s2, s3, r0, r1, r2, r3, dd;

			s0 = buff0[i];
			s1 = buff1[i];
			s2 = buff2[i];
			s3 = buff3[i];

			r0 = _mm_mulhi_epi16(s0, dyf0);
			r1 = _mm_mulhi_epi16(s1, dyf1);
			r2 = _mm_mulhi_epi16(s2, dyf2);
			r3 = _mm_mulhi_epi16(s3, dyf3);

			dd = _mm_add_epi16(_mm_add_epi16(r1, r2),
			    _mm_add_epi16(r0, r3));
			dd = _mm_add_epi16(dd, dd);
			dd = _mm_adds_epi16(dd, dd);

			dd = XOR(dd);
			_mm_storel_epi64((void *)tdp, dd);
			tdp += 3;
			_mm_storel_epi64((void *)tdp,
			    _mm_unpackhi_epi64(dd, dd));
			tdp += 3;
		}

		if (i * 2 < width) {
			__m128i s0, s1, s2, s3, r0, r1, r2, r3, dd;

			s0 = buff0[i];
			s1 = buff1[i];
			s2 = buff2[i];
			s3 = buff3[i];

			r0 = _mm_mulhi_epi16(s0, dyf0);
			r1 = _mm_mulhi_epi16(s1, dyf1);
			r2 = _mm_mulhi_epi16(s2, dyf2);
			r3 = _mm_mulhi_epi16(s3, dyf3);

			dd = _mm_add_epi16(_mm_add_epi16(r1, r2),
			    _mm_add_epi16(r0, r3));
			dd = _mm_add_epi16(dd, dd);
			dd = _mm_adds_epi16(dd, dd);

			dd = XOR(dd);
			_mm_storel_epi64((void *)tdp, dd);
		}

		dp[width * 3] = tail;

		y_step = ((y + dy) >> MLIB_SHIFT) - (y >> MLIB_SHIFT);
		y += dy;
		dp += dst_stride;
	}

	__mlib_free(buff);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_s_ImageZoom_BC_U16_4(
    mlib_work_image * param)
{
	VARIABLE(mlib_u16)
	void *filter1 = GetElemStruct(filter1);
	mlib_s32 *prow;
	mlib_s32 fpos, cx;
	__m128i *dfPtr, dyf0, dyf1, dyf2, dyf3;
	__m128i *buff, *buff_arr[8], *buff0, *buff1, *buff2, *buff3, *buffx,
	    *pbuff;
	__m128i mask_8000 = _mm_set1_epi16(0x8000);
	mlib_s32 width4, width8, k, bind = 0, y_step = 4;
	mlib_s32 nchan = param->channels;

	width8 = (width * 4 + 7) / 8;

	buff = __mlib_malloc(width8 * 8 * sizeof (__m128i));
	if (buff == NULL)
		return (MLIB_FAILURE);
	for (k = 0; k < 4; k++)
		buff_arr[k] = buff + k * width8;
	for (k = 0; k < 4; k++)
		buff_arr[4 + k] = buff_arr[k];
	buffx = buff + 4 * width8;

	x = GetElemSubStruct(current, srcX) & MLIB_MASK;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < (width8 << 2); i += 4) {
		__m128i *p_flt0, *p_flt1;
		__m128i a0, a1, b0, b1;

		fpos = (x >> (MLIB_SHIFT - 9)) & 0x1ff;
		x += dx;
		a0 = _mm_loadl_epi64((void *)((mlib_d64 *)filter1 + fpos));
		a0 = _mm_unpacklo_epi16(a0, a0);
		a1 = _mm_unpackhi_epi32(a0, a0);
		a0 = _mm_unpacklo_epi32(a0, a0);

		fpos = (x >> (MLIB_SHIFT - 9)) & 0x1ff;
		x += dx;
		b0 = _mm_loadl_epi64((void *)((mlib_d64 *)filter1 + fpos));
		b0 = _mm_unpacklo_epi16(b0, b0);
		b1 = _mm_unpackhi_epi32(b0, b0);
		b0 = _mm_unpacklo_epi32(b0, b0);

		buffx[i] = _mm_unpacklo_epi64(a0, b0);
		buffx[i + 1] = _mm_unpackhi_epi64(a0, b0);
		buffx[i + 2] = _mm_unpacklo_epi64(a1, b1);
		buffx[i + 3] = _mm_unpackhi_epi64(a1, b1);
	}

	sp -= src_stride + nchan;
	tsp = sp;

	for (j = 0; j < height; j++) {
		COEFF_D64_U8(y)

	    if (y_step >= 4) {
			y_step = 4;
			tsp = sp + (y >> MLIB_SHIFT) * src_stride;
		} else {
			bind += y_step;
			if (bind >= 4)
				bind -= 4;
		}

		for (k = 4 - y_step; k < 4; k++) {
			x = GetElemSubStruct(current, srcX) & MLIB_MASK;
			pbuff = buff_arr[bind + k];

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (int m = 0, i = 0; i < width8; m += 4, i++) {
				__m128i t0, t1, t2, t3;
				__m128i s0, s1, s2, s3;
				mlib_u16 *sp0 =
				    tsp + 4 * (x >> MLIB_SHIFT);
				mlib_u16 *sp1 =
				    tsp + 4 * ((x + dx) >> MLIB_SHIFT);

				t0 = _mm_loadu_si128((void *)sp0);
				t1 = _mm_loadu_si128((void *)sp1);
				t2 = _mm_loadu_si128((void *)(sp0 + 8));
				t3 = _mm_loadu_si128((void *)(sp1 + 8));

				s0 = _mm_unpacklo_epi64(t0, t1);
				s1 = _mm_unpackhi_epi64(t0, t1);
				s2 = _mm_unpacklo_epi64(t2, t3);
				s3 = _mm_unpackhi_epi64(t2, t3);

				s0 = _mm_mulhi_epi16(XOR(s0), buffx[m]);
				s1 = _mm_mulhi_epi16(XOR(s1), buffx[m + 1]);
				s2 = _mm_mulhi_epi16(XOR(s2), buffx[m + 2]);
				s3 = _mm_mulhi_epi16(XOR(s3), buffx[m + 3]);

				pbuff[i] =
				    _mm_add_epi16(_mm_add_epi16(s1, s2),
				    _mm_add_epi16(s0, s3));
				x += 2 * dx;
			}

			tsp += src_stride;
		}

		buff0 = buff_arr[bind];
		buff1 = buff_arr[bind + 1];
		buff2 = buff_arr[bind + 2];
		buff3 = buff_arr[bind + 3];

		mlib_u16 *tdp = dp;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < (width >> 1); i++) {
			__m128i s0, s1, s2, s3, r0, r1, r2, r3, dd;

			s0 = buff0[i];
			s1 = buff1[i];
			s2 = buff2[i];
			s3 = buff3[i];

			r0 = _mm_mulhi_epi16(s0, dyf0);
			r1 = _mm_mulhi_epi16(s1, dyf1);
			r2 = _mm_mulhi_epi16(s2, dyf2);
			r3 = _mm_mulhi_epi16(s3, dyf3);

			dd = _mm_add_epi16(_mm_add_epi16(r1, r2),
			    _mm_add_epi16(r0, r3));
			dd = _mm_add_epi16(dd, dd);
			dd = XOR(_mm_adds_epi16(dd, dd));

			_mm_storeu_si128((void *)tdp, dd);
			tdp += 8;
		}

		if (i * 2 < width) {
			__m128i s0, s1, s2, s3, r0, r1, r2, r3, dd;

			s0 = buff0[i];
			s1 = buff1[i];
			s2 = buff2[i];
			s3 = buff3[i];

			r0 = _mm_mulhi_epi16(s0, dyf0);
			r1 = _mm_mulhi_epi16(s1, dyf1);
			r2 = _mm_mulhi_epi16(s2, dyf2);
			r3 = _mm_mulhi_epi16(s3, dyf3);

			dd = _mm_add_epi16(_mm_add_epi16(r1, r2),
			    _mm_add_epi16(r0, r3));
			dd = _mm_add_epi16(dd, dd);
			dd = XOR(_mm_adds_epi16(dd, dd));

			_mm_storel_epi64((void *)tdp, dd);
		}

		y_step = ((y + dy) >> MLIB_SHIFT) - (y >> MLIB_SHIFT);
		y += dy;
		dp += dst_stride;
	}

	__mlib_free(buff);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
