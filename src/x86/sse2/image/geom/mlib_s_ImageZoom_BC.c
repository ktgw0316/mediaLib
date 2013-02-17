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
 * Copyright 2008 Sun Microsystems, Inc.  All rights reserved.
 * Use is subject to license terms.
 */

#pragma ident	"@(#)mlib_s_ImageZoom_BC.c	9.6	08/10/29 SMI"

/*
 * FUNCTION
 *      mlib_ImageZoomTranslate(), internal pixels.
 *
 */

#include <mlib_s_ImageZoom.h>

/* *********************************************************** */

#define	INIT_XLINE()                                            \
	x = GetElemSubStruct(current, srcX) & MLIB_MASK;        \
	ddy = _mm_set1_epi16((y >> 1) & 0x7FFF);                \
	rdy = _mm_sub_epi16(mask_8000, ddy)

/* *********************************************************** */

#define	COEFF_D64_U8(r)                                         \
	fpos = (r >> (MLIB_SHIFT - 8)) & 0xff;                  \
	d##r##f0 =                                              \
	_mm_loadl_epi64((void *)((mlib_d64 *)filter1 + fpos));  \
	d##r##f0 =                                              \
	_mm_unpacklo_epi16(d##r##f0, d##r##f0);                 \
	d##r##f2 =                                              \
	_mm_unpackhi_epi32(d##r##f0, d##r##f0);                 \
	d##r##f0 =                                              \
	_mm_unpacklo_epi32(d##r##f0, d##r##f0);                 \
	d##r##f1 =                                              \
	_mm_unpackhi_epi64(d##r##f0, d##r##f0);                 \
	d##r##f3 =                                              \
	_mm_unpackhi_epi64(d##r##f2, d##r##f2);                 \
	d##r##f0 =                                              \
	_mm_unpacklo_epi64(d##r##f0, d##r##f0);                 \
	d##r##f2 =                                              \
	_mm_unpacklo_epi64(d##r##f2, d##r##f2);

/* *********************************************************** */

#define	MUL_ALPHA(res, bmask)                                   \
	aa = _mm_shufflelo_epi16(res, bmask);                   \
	aa = _mm_shufflehi_epi16(aa, bmask);                    \
	aa = _mm_or_si128(aa, amask);                           \
	aa = _mm_mulhi_epu16(aa, scale255);                     \
	res = _mm_mulhi_epu16(res, aa);

/* *********************************************************** */

#define	COEFF_D64_U8(r)                                         \
	fpos = (r >> (MLIB_SHIFT - 8)) & 0xff;                  \
	d##r##f0 =                                              \
	_mm_loadl_epi64((void *)((mlib_d64 *)filter1 + fpos));  \
	d##r##f0 =                                              \
	_mm_unpacklo_epi16(d##r##f0, d##r##f0);                 \
	d##r##f2 =                                              \
	_mm_unpackhi_epi32(d##r##f0, d##r##f0);                 \
	d##r##f0 =                                              \
	_mm_unpacklo_epi32(d##r##f0, d##r##f0);                 \
	d##r##f1 =                                              \
	_mm_unpackhi_epi64(d##r##f0, d##r##f0);                 \
	d##r##f3 =                                              \
	_mm_unpackhi_epi64(d##r##f2, d##r##f2);                 \
	d##r##f0 =                                              \
	_mm_unpacklo_epi64(d##r##f0, d##r##f0);                 \
	d##r##f2 =                                              \
	_mm_unpacklo_epi64(d##r##f2, d##r##f2);

/* *********************************************************** */

mlib_status
mlib_s_ImageZoomBlend_BC_U8(
    mlib_work_image * param)
{
	VARIABLE(mlib_u8)
	mlib_u8 *filter1 = GetElemStruct(filter1);
	mlib_s32 *prow;
	mlib_s32 fpos, cx;
	__m128i *dfPtr, dyf0, dyf1, dyf2, dyf3, aa, mzero, amask, scale255,
	    const255sh, r1mask;
	__m128i *buff, *buff_arr[8], *buff0, *buff1, *buff2, *buff3, *buffx,
	    *buffz, *buffd, *pbuff;
	mlib_s32 width2, tail, k, bind = 0, y_step = 4;
	mlib_s32 alp_ind = param->alp_ind;
	mlib_s32 chan_d = param->chan_d;
	mlib_s32 chan_s = param->channels;

	width2 = (width + 1) / 2;

	if (alp_ind == -1) {
		if (chan_s == 4)
			sp--;
		tsp = sp;
	}

	mzero = _mm_setzero_si128();
	r1mask = _mm_set1_epi16(0xFF00);
	if (alp_ind == -1) {
		amask = _mm_setr_epi32(0xff00, 0, 0xff00, 0);
	} else {
		amask = _mm_setr_epi32(0, 0xff000000, 0, 0xff000000);
	}
	scale255 = _mm_set1_epi16((mlib_u16)((1 << 15) * 256.0 / 255 + 0.5));
	if (chan_s == 3 || !alp_ind) {
		const255sh = _mm_set1_epi16(255 << 4);
	} else {
		const255sh = _mm_set1_epi16(255 << 3);
	}

	buff = __mlib_malloc(width2 * 10 * sizeof (__m128i));
	if (buff == NULL)
		return (MLIB_FAILURE);
	for (k = 0; k < 4; k++)
		buff_arr[k] = buff + k * width2;
	for (k = 0; k < 4; k++)
		buff_arr[4 + k] = buff_arr[k];
	buffx = buff + 4 * width2;
	buffz = buffx + 4 * width2;
	buffd = buffz + width2;

	x = GetElemSubStruct(current, srcX) & MLIB_MASK;
	for (i = 0; i < width2; i++) {
		__m128i *p_flt0, *p_flt1;
		__m128i a0, a1, b0, b1;

		fpos = (x >> (MLIB_SHIFT - 8)) & 0xff;
		x += dx;
		a0 = _mm_loadl_epi64((void *)((mlib_d64 *)filter1 + fpos));
		a0 = _mm_unpacklo_epi16(a0, a0);
		a1 = _mm_unpackhi_epi32(a0, a0);
		a0 = _mm_unpacklo_epi32(a0, a0);

		fpos = (x >> (MLIB_SHIFT - 8)) & 0xff;
		x += dx;
		b0 = _mm_loadl_epi64((void *)((mlib_d64 *)filter1 + fpos));
		b0 = _mm_unpacklo_epi16(b0, b0);
		b1 = _mm_unpackhi_epi32(b0, b0);
		b0 = _mm_unpacklo_epi32(b0, b0);

		buffx[4 * i] = _mm_sub_epi16(mzero, _mm_unpacklo_epi64(a0, b0));
		buffx[4 * i + 1] = _mm_unpackhi_epi64(a0, b0);
		buffx[4 * i + 2] = _mm_unpacklo_epi64(a1, b1);
		buffx[4 * i + 3] =
		    _mm_sub_epi16(mzero, _mm_unpackhi_epi64(a1, b1));

	}

	sp -= src_stride + chan_s;
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
			__m128i s0, s1, s2, s3, t0, t1;
			__m128i r0, r1, r2, r3, dd;

			x = GetElemSubStruct(current, srcX) & MLIB_MASK;
			pbuff = buff_arr[bind + k];

			prow = (mlib_s32 *)tsp;

			if (chan_s == 3) {
				mlib_s32 cx0, cx1;

				for (i = 0; i < width / 2; i++) {
					cx0 = 3 * (x >> MLIB_SHIFT);
					x += dx;
					cx1 = 3 * (x >> MLIB_SHIFT);
					x += dx;

					LOAD_2s32_to_m128(s0, tsp + cx0,
					    tsp + cx1);
					LOAD_2s32_to_m128(s1, tsp + cx0 + 3,
					    tsp + cx1 + 3);
					LOAD_2s32_to_m128(s2, tsp + cx0 + 6,
					    tsp + cx1 + 6);
					/* -1 and srli for avoiding ABR */
					LOAD_2s32_to_m128(s3, tsp + cx0 + 8,
					    tsp + cx1 + 8);
					s3 = _mm_srli_si128(s3, 2);

					r0 = _mm_mulhi_epu16(s0, buffx[4 * i]);
					r1 = _mm_mulhi_epu16(s1,
					    buffx[4 * i + 1]);
					r2 = _mm_mulhi_epu16(s2,
					    buffx[4 * i + 2]);
					r3 = _mm_mulhi_epu16(s3,
					    buffx[4 * i + 3]);

					dd = _mm_sub_epi16(_mm_add_epi16(r1,
					    r2), _mm_add_epi16(r0, r3));
					pbuff[i] = dd;
				}
				if (width & 1) {
					cx0 = 3 * (x >> MLIB_SHIFT);

					LOAD_s32_to_m128(s0, tsp + cx0);
					LOAD_s32_to_m128(s1, tsp + cx0 + 3);
					LOAD_s32_to_m128(s2, tsp + cx0 + 6);
					LOAD_s32_to_m128(s3, tsp + cx0 + 8);
					s3 = _mm_srli_si128(s3, 2);

					r0 = _mm_mulhi_epu16(s0, buffx[4 * i]);
					r1 = _mm_mulhi_epu16(s1,
					    buffx[4 * i + 1]);
					r2 = _mm_mulhi_epu16(s2,
					    buffx[4 * i + 2]);
					r3 = _mm_mulhi_epu16(s3,
					    buffx[4 * i + 3]);

					dd = _mm_sub_epi16(_mm_add_epi16(r1,
					    r2), _mm_add_epi16(r0, r3));
					pbuff[i] = dd;
				}
			} else if (!alp_ind) {
				for (i = 0; i < width / 2; i++) {
					cx = x >> MLIB_SHIFT;
					x += dx;
					s0 = _mm_loadu_si128((void *)(prow +
					    cx));
					cx = x >> MLIB_SHIFT;
					x += dx;
					s1 = _mm_loadu_si128((void *)(prow +
					    cx));

					t0 = _mm_unpacklo_epi32(s0, s1);
					t1 = _mm_unpackhi_epi32(s0, s1);
					s0 = _mm_unpacklo_epi8(mzero, t0);
					s1 = _mm_unpackhi_epi8(mzero, t0);
					s2 = _mm_unpacklo_epi8(mzero, t1);
					s3 = _mm_unpackhi_epi8(mzero, t1);

					r0 = _mm_mulhi_epu16(s0, buffx[4 * i]);
					r1 = _mm_mulhi_epu16(s1,
					    buffx[4 * i + 1]);
					r2 = _mm_mulhi_epu16(s2,
					    buffx[4 * i + 2]);
					r3 = _mm_mulhi_epu16(s3,
					    buffx[4 * i + 3]);

					dd = _mm_sub_epi16(_mm_add_epi16(r1,
					    r2), _mm_add_epi16(r0, r3));
					pbuff[i] = dd;
				}
				if (width & 1) {
					cx = x >> MLIB_SHIFT;
					s0 = _mm_loadu_si128((void *)(prow +
					    cx));

					t0 = _mm_unpacklo_epi32(s0, mzero);
					t1 = _mm_unpackhi_epi32(s0, mzero);
					s0 = _mm_unpacklo_epi8(mzero, t0);
					s1 = _mm_unpackhi_epi8(mzero, t0);
					s2 = _mm_unpacklo_epi8(mzero, t1);
					s3 = _mm_unpackhi_epi8(mzero, t1);

					r0 = _mm_mulhi_epu16(s0, buffx[4 * i]);
					r1 = _mm_mulhi_epu16(s1,
					    buffx[4 * i + 1]);
					r2 = _mm_mulhi_epu16(s2,
					    buffx[4 * i + 2]);
					r3 = _mm_mulhi_epu16(s3,
					    buffx[4 * i + 3]);

					dd = _mm_sub_epi16(_mm_add_epi16(r1,
					    r2), _mm_add_epi16(r0, r3));
					pbuff[i] = dd;
				}
			} else if (alp_ind == -1) {
				for (i = 0; i < width / 2; i++) {
					cx = x >> MLIB_SHIFT;
					x += dx;
					s0 = _mm_loadu_si128((void *)(prow +
					    cx));
					cx = x >> MLIB_SHIFT;
					x += dx;
					s1 = _mm_loadu_si128((void *)(prow +
					    cx));

					t0 = _mm_unpacklo_epi32(s0, s1);
					t1 = _mm_unpackhi_epi32(s0, s1);
					s0 = _mm_unpacklo_epi8(mzero, t0);
					s1 = _mm_unpackhi_epi8(mzero, t0);
					s2 = _mm_unpacklo_epi8(mzero, t1);
					s3 = _mm_unpackhi_epi8(mzero, t1);

					MUL_ALPHA(s0, 0);
					MUL_ALPHA(s1, 0);
					MUL_ALPHA(s2, 0);
					MUL_ALPHA(s3, 0);

					r0 = _mm_mulhi_epu16(s0, buffx[4 * i]);
					r1 = _mm_mulhi_epu16(s1,
					    buffx[4 * i + 1]);
					r2 = _mm_mulhi_epu16(s2,
					    buffx[4 * i + 2]);
					r3 = _mm_mulhi_epu16(s3,
					    buffx[4 * i + 3]);

					dd = _mm_sub_epi16(_mm_add_epi16(r1,
					    r2), _mm_add_epi16(r0, r3));
					pbuff[i] = dd;
				}
				if (width & 1) {
					cx = x >> MLIB_SHIFT;
					s0 = _mm_loadu_si128((void *)(prow +
					    cx));

					t0 = _mm_unpacklo_epi32(s0, mzero);
					t1 = _mm_unpackhi_epi32(s0, mzero);
					s0 = _mm_unpacklo_epi8(mzero, t0);
					s1 = _mm_unpackhi_epi8(mzero, t0);
					s2 = _mm_unpacklo_epi8(mzero, t1);
					s3 = _mm_unpackhi_epi8(mzero, t1);

					MUL_ALPHA(s0, 0);
					MUL_ALPHA(s1, 0);
					MUL_ALPHA(s2, 0);
					MUL_ALPHA(s3, 0);

					r0 = _mm_mulhi_epu16(s0, buffx[4 * i]);
					r1 = _mm_mulhi_epu16(s1,
					    buffx[4 * i + 1]);
					r2 = _mm_mulhi_epu16(s2,
					    buffx[4 * i + 2]);
					r3 = _mm_mulhi_epu16(s3,
					    buffx[4 * i + 3]);

					dd = _mm_sub_epi16(_mm_add_epi16(r1,
					    r2), _mm_add_epi16(r0, r3));
					pbuff[i] = dd;
				}
			} else {
				for (i = 0; i < width / 2; i++) {
					cx = x >> MLIB_SHIFT;
					x += dx;
					s0 = _mm_loadu_si128((void *)(prow +
					    cx));
					cx = x >> MLIB_SHIFT;
					x += dx;
					s1 = _mm_loadu_si128((void *)(prow +
					    cx));

					t0 = _mm_unpacklo_epi32(s0, s1);
					t1 = _mm_unpackhi_epi32(s0, s1);
					s0 = _mm_unpacklo_epi8(mzero, t0);
					s1 = _mm_unpackhi_epi8(mzero, t0);
					s2 = _mm_unpacklo_epi8(mzero, t1);
					s3 = _mm_unpackhi_epi8(mzero, t1);

					MUL_ALPHA(s0, 0xff);
					MUL_ALPHA(s1, 0xff);
					MUL_ALPHA(s2, 0xff);
					MUL_ALPHA(s3, 0xff);

					r0 = _mm_mulhi_epu16(s0, buffx[4 * i]);
					r1 = _mm_mulhi_epu16(s1,
					    buffx[4 * i + 1]);
					r2 = _mm_mulhi_epu16(s2,
					    buffx[4 * i + 2]);
					r3 = _mm_mulhi_epu16(s3,
					    buffx[4 * i + 3]);

					dd = _mm_sub_epi16(_mm_add_epi16(r1,
					    r2), _mm_add_epi16(r0, r3));
					pbuff[i] = dd;
				}
				if (width & 1) {
					cx = x >> MLIB_SHIFT;
					s0 = _mm_loadu_si128((void *)(prow +
					    cx));

					t0 = _mm_unpacklo_epi32(s0, mzero);
					t1 = _mm_unpackhi_epi32(s0, mzero);
					s0 = _mm_unpacklo_epi8(mzero, t0);
					s1 = _mm_unpackhi_epi8(mzero, t0);
					s2 = _mm_unpacklo_epi8(mzero, t1);
					s3 = _mm_unpackhi_epi8(mzero, t1);

					MUL_ALPHA(s0, 0xff);
					MUL_ALPHA(s1, 0xff);
					MUL_ALPHA(s2, 0xff);
					MUL_ALPHA(s3, 0xff);

					r0 = _mm_mulhi_epu16(s0, buffx[4 * i]);
					r1 = _mm_mulhi_epu16(s1,
					    buffx[4 * i + 1]);
					r2 = _mm_mulhi_epu16(s2,
					    buffx[4 * i + 2]);
					r3 = _mm_mulhi_epu16(s3,
					    buffx[4 * i + 3]);

					dd = _mm_sub_epi16(_mm_add_epi16(r1,
					    r2), _mm_add_epi16(r0, r3));
					pbuff[i] = dd;
				}
			}

			tsp += src_stride;
		}

		buff0 = buff_arr[bind];
		buff1 = buff_arr[bind + 1];
		buff2 = buff_arr[bind + 2];
		buff3 = buff_arr[bind + 3];

		if (chan_s == 3 || !alp_ind) {
			for (i = 0; i < width2; i++) {
				__m128i s0, s1, s2, s3, r0, r1, r2, r3, r4, r5,
				    dd;
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

				r0 = _mm_cmplt_epi16(dd, mzero);
				r1 = _mm_cmpgt_epi16(dd, const255sh);
				dd = _mm_slli_epi16(dd, 4);
				dd = _mm_or_si128(_mm_andnot_si128(r0, dd), r1);
				buffz[i] = dd;
			}
		} else {
			for (i = 0; i < width2; i++) {
				__m128i s0, s1, s2, s3, r0, r1, r2, r3, r4, r5,
				    dd;
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

				r0 = _mm_cmplt_epi16(dd, mzero);
				r1 = _mm_cmpgt_epi16(dd, const255sh);
				r0 = _mm_or_si128(r0, r1);
				r1 = _mm_and_si128(r1, r1mask);
				dd = _mm_slli_epi16(dd, 5);
				dd = _mm_or_si128(_mm_andnot_si128(r0, dd), r1);
				buffz[i] = dd;
			}
		}

		mlib_s_ImageBlendLine(param, dp, buffz, buffd);

		y_step = ((y + dy) >> MLIB_SHIFT) - (y >> MLIB_SHIFT);
		y += dy;
		dp += dst_stride;
	}

	__mlib_free(buff);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#ifdef	__i386

mlib_status
mlib_s_ImageZoom_BC_U8_12(
    mlib_work_image * param)
{
	VARIABLE(mlib_u8)
	mlib_u8 *filter1 = GetElemStruct(filter1);
	mlib_u8 *tdp;
	mlib_s32 *prow;
	__m128i *dfPtr;
	__m128i *buff, *buff_arr[8], *buffx, *pbuff;
	mlib_s32 width4, width8, k, bind = 0, y_step = 4;
	mlib_s32 nchan = param->channels;

	if (nchan == 2)
		width *= 2;
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
	for (i = 0, k = 0; i < width4; i++, k += 2) {
		__m128i a0, a1, a2, a3, b0, b1, b2, b3, b4, b5;
		mlib_s32 fpos;

		fpos = (x >> (MLIB_SHIFT - 8)) & 0xff;
		x += dx;
		a0 = _mm_loadl_epi64((void *)((mlib_d64 *)filter1 + fpos));
		fpos = (x >> (MLIB_SHIFT - 8)) & 0xff;
		x += dx;
		a1 = _mm_loadl_epi64((void *)((mlib_d64 *)filter1 + fpos));
		fpos = (x >> (MLIB_SHIFT - 8)) & 0xff;
		x += dx;
		a2 = _mm_loadl_epi64((void *)((mlib_d64 *)filter1 + fpos));
		fpos = (x >> (MLIB_SHIFT - 8)) & 0xff;
		x += dx;
		a3 = _mm_loadl_epi64((void *)((mlib_d64 *)filter1 + fpos));

		b0 = _mm_unpacklo_epi16(a0, a2);
		b1 = _mm_unpacklo_epi16(a1, a3);

		b2 = _mm_unpacklo_epi16(b0, b1);
		b3 = _mm_unpackhi_epi16(b0, b1);

		b4 = _mm_sub_epi16(_mm_setzero_si128(), b2);
		b4 = _mm_unpacklo_epi64(b4, _mm_unpackhi_epi64(b2, b2));
		_mm_store_si128((void *)(buffx + k), b4);

		b5 = _mm_sub_epi16(_mm_setzero_si128(), b3);
		b5 = _mm_unpacklo_epi64(b3, _mm_unpackhi_epi64(b5, b5));
		_mm_store_si128((void *)(buffx + k + 1), b5);
	}

	sp -= src_stride + nchan;
	tsp = sp;
	mlib_s32 dx4 = 4 * dx;

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
			mlib_s32 t;
			x = GetElemSubStruct(current, srcX) & MLIB_MASK;
			pbuff = buff_arr[bind + k];

			if (nchan == 1) {
				mlib_s32 x1, x2, x3;
				x1 = x + dx;
				x2 = x + 2 * dx;
				x3 = x + 3 * dx;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (t = 0, i = 0; i < width / 4; t += 2, i++) {
					__m128i s0, s1, s2, s3, t0, t1, t2, t3;
					__m128i r0, r1, r2, r3, r4, mzero;
					mlib_u8 *sp0 = tsp + (x >> MLIB_SHIFT);
					mlib_u8 *sp1 =
					    tsp + (x1 >> MLIB_SHIFT);
					mlib_u8 *sp2 =
					    tsp + (x2 >> MLIB_SHIFT);
					mlib_u8 *sp3 =
					    tsp + (x3 >> MLIB_SHIFT);

					mzero = _mm_setzero_si128();
					LOAD_2s32_to_m128(s0, sp0, sp1);
					LOAD_2s32_to_m128(s1, sp2, sp3);
					s2 = _mm_unpacklo_epi16(s0, s1);
					s3 = _mm_unpackhi_epi16(s0, s1);
					t0 = _mm_unpacklo_epi16(s2, s3);
					t1 = _mm_unpackhi_epi16(s2, s3);

					r0 = _mm_mulhi_epu16(t0, buffx[t]);
					r1 = _mm_mulhi_epu16(t1, buffx[t + 1]);
					r2 = _mm_sub_epi16(r0, r1);
					r3 = _mm_unpackhi_epi64(r2, r2);
					r4 = _mm_sub_epi16(r3, r2);
					_mm_storel_epi64((void *)((mlib_d64 *)
					    pbuff + i), r4);
					x += dx4;
					x1 += dx4;
					x2 += dx4;
					x3 += dx4;
				}
				if (width & 3) {
					mlib_s32 leftover = width & 3;
					__m128i s0, s1, s2, s3, t0, t1, t2, t3;
					__m128i r0, r1, r2, r3, r4, mzero;
					mlib_u8 *sp0 = tsp + (x >> MLIB_SHIFT);
					mlib_u8 *sp1 =
					    tsp + (x1 >> MLIB_SHIFT);
					mlib_u8 *sp2 =
					    tsp + (x2 >> MLIB_SHIFT);
					mlib_u8 *sp3 =
					    tsp + (x3 >> MLIB_SHIFT);

					mzero = _mm_setzero_si128();
					switch (leftover) {
					case 1:
						LOAD_s32_to_m128(s0, sp0);
						break;
					case 2:
						LOAD_2s32_to_m128(s0, sp0, sp1);
						break;
					case 3:
						LOAD_2s32_to_m128(s0, sp0, sp1);
						LOAD_s32_to_m128(s1, sp2);
						break;
					default:
						break;
					}

					s2 = _mm_unpacklo_epi16(s0, s1);
					s3 = _mm_unpackhi_epi16(s0, s1);
					t0 = _mm_unpacklo_epi16(s2, s3);
					t1 = _mm_unpackhi_epi16(s2, s3);

					r0 = _mm_mulhi_epu16(t0, buffx[t]);
					r1 = _mm_mulhi_epu16(t1, buffx[t + 1]);
					r2 = _mm_sub_epi16(r0, r1);
					r3 = _mm_unpackhi_epi64(r2, r2);
					r4 = _mm_sub_epi16(r3, r2);
					_mm_storel_epi64((void *)((mlib_d64 *)
					    pbuff + i), r4);
				}
			} else {
				for (t = 0, i = 0; i < width / 8; t += 2, i++) {
					__m128i r0, r1, r2, r3;
					__m128i s0, s1, s2, s3;
					__m128i t0, t1, t2, t3;
					__m128i u0, u1, u2, u3;
					__m128i f0, f1, f2, f3, mzero;
					s0 = _mm_loadl_epi64((void *)(tsp +
					    2 * (x >> MLIB_SHIFT)));
					s1 = _mm_loadl_epi64((void *)(tsp +
					    2 * ((x + dx) >> MLIB_SHIFT)));
					s2 = _mm_loadl_epi64((void *)(tsp +
					    2 * ((x + 2 * dx) >> MLIB_SHIFT)));
					s3 = _mm_loadl_epi64((void *)(tsp +
					    2 * ((x + 3 * dx) >> MLIB_SHIFT)));

					t0 = _mm_unpacklo_epi16(s0, s2);
					t1 = _mm_unpacklo_epi16(s1, s3);
					t2 = _mm_unpacklo_epi16(t0, t1);
					t3 = _mm_unpackhi_epi16(t0, t1);

					mzero = _mm_setzero_si128();
					r0 = _mm_unpacklo_epi8(mzero, t2);
					r1 = _mm_unpackhi_epi8(mzero, t2);
					r2 = _mm_unpacklo_epi8(mzero, t3);
					r3 = _mm_unpackhi_epi8(mzero, t3);

					u0 = buffx[t];
					u1 = buffx[t + 1];
					f0 = _mm_unpacklo_epi16(u0, u0);
					f1 = _mm_unpackhi_epi16(u0, u0);
					f2 = _mm_unpacklo_epi16(u1, u1);
					f3 = _mm_unpackhi_epi16(u1, u1);

					f0 = _mm_mulhi_epu16(r0, f0);
					f1 = _mm_mulhi_epu16(r1, f1);
					f2 = _mm_mulhi_epu16(r2, f2);
					f3 = _mm_mulhi_epu16(r3, f3);

					u2 = _mm_add_epi16(f1, f2);
					u3 = _mm_add_epi16(f0, f3);
					pbuff[i] =
					    _mm_sub_epi16(u2, u3);

					x += dx4;
				}
				if (width & 7) {
					mlib_s32 leftover = width & 7;
					__m128i r0, r1, r2, r3;
					__m128i s0, s1, s2, s3;
					__m128i t0, t1, t2, t3;
					__m128i u0, u1, u2, u3;
					__m128i f0, f1, f2, f3, mzero;
					mlib_u8 *sp0 = tsp +
					    2* (x >> MLIB_SHIFT);
					mlib_u8 *sp1 = tsp +
					    2 * ((x + dx) >> MLIB_SHIFT);
					mlib_u8 *sp2 = tsp +
					    2 * ((x + 2 * dx) >> MLIB_SHIFT);
					mlib_u8 *sp3 = tsp +
					    2 * ((x + 3 * dx) >> MLIB_SHIFT);

					switch (leftover) {
					case 1:
						sse_loadl_epi32(s0, sp0);
						break;
					case 2:
						s0 = _mm_loadl_epi64(
							(void *)sp0);
						break;
					case 3:
						s0 = _mm_loadl_epi64(
							(void *)sp0);
						sse_loadl_epi32(s1, sp1);
						break;
					case 4:
						s0 = _mm_loadl_epi64(
							(void *)sp0);
						s1 = _mm_loadl_epi64(
							(void *)sp1);
						break;
					case 5:
						s0 = _mm_loadl_epi64(
							(void *)sp0);
						s1 = _mm_loadl_epi64(
							(void *)sp1);
						sse_loadl_epi32(s2, sp2);
						break;
					case 6:
						s0 = _mm_loadl_epi64(
							(void *)sp0);
						s1 = _mm_loadl_epi64(
							(void *)sp1);
						s2 = _mm_loadl_epi64(
							(void *)sp2);
						break;
					case 7:
						s0 = _mm_loadl_epi64(
							(void *)sp0);
						s1 = _mm_loadl_epi64(
							(void *)sp1);
						s2 = _mm_loadl_epi64(
							(void *)sp2);
						sse_loadl_epi32(s3, sp3);
						break;
					default:
						break;
					}

					t0 = _mm_unpacklo_epi16(s0, s2);
					t1 = _mm_unpacklo_epi16(s1, s3);
					t2 = _mm_unpacklo_epi16(t0, t1);
					t3 = _mm_unpackhi_epi16(t0, t1);

					mzero = _mm_setzero_si128();
					r0 = _mm_unpacklo_epi8(mzero, t2);
					r1 = _mm_unpackhi_epi8(mzero, t2);
					r2 = _mm_unpacklo_epi8(mzero, t3);
					r3 = _mm_unpackhi_epi8(mzero, t3);

					u0 = buffx[t];
					u1 = buffx[t + 1];
					f0 = _mm_unpacklo_epi16(u0, u0);
					f1 = _mm_unpackhi_epi16(u0, u0);
					f2 = _mm_unpacklo_epi16(u1, u1);
					f3 = _mm_unpackhi_epi16(u1, u1);

					f0 = _mm_mulhi_epu16(r0, f0);
					f1 = _mm_mulhi_epu16(r1, f1);
					f2 = _mm_mulhi_epu16(r2, f2);
					f3 = _mm_mulhi_epu16(r3, f3);

					u2 = _mm_add_epi16(f1, f2);
					u3 = _mm_add_epi16(f0, f3);
					pbuff[i] =
					    _mm_sub_epi16(u2, u3);
				}
			}

			tsp += src_stride;
		}

		__m128i *buff0, *buff1, *buff2, *buff3;

		buff0 = buff_arr[bind];
		buff1 = buff_arr[bind + 1];
		buff2 = buff_arr[bind + 2];
		buff3 = buff_arr[bind + 3];

		tdp = dp;

		__m128i d_rnd = _mm_set1_epi16(8);
		__m128i dyf0, dyf1, dyf2, dyf3;
		mlib_s32 fpos;
		COEFF_D64_U8(y)

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < width / 8; i++) {
			__m128i s0, s1, s2, s3, s4, s5;
			__m128i dd0, dd1, dd2, dd3;

			s0 = buff0[i];
			s0 = _mm_mulhi_epi16(s0, dyf0);
			s1 = buff1[i];
			s1 = _mm_mulhi_epi16(s1, dyf1);
			s2 = buff2[i];
			s2 = _mm_mulhi_epi16(s2, dyf2);
			s3 = buff3[i];
			s3 = _mm_mulhi_epi16(s3, dyf3);

			s4 = _mm_add_epi16(s1, s2);
			s5 = _mm_add_epi16(s0, s3);

			dd0 = _mm_add_epi16(s4, s5);

			dd1 = _mm_add_epi16(dd0, _mm_set1_epi16(8));
			dd2 = _mm_srai_epi16(dd1, 4);
			dd3 = _mm_packus_epi16(dd2, dd2);

			_mm_storel_epi64((void *)tdp, dd3);
			tdp += 8;
		}

		if (width & 7) {
			__m128i s0, s1, s2, s3, s4, s5;
			__m128i dd0, dd1, dd2, dd3;

			s0 = buff0[i];
			s1 = buff1[i];
			s2 = buff2[i];
			s3 = buff3[i];

			s0 = _mm_mulhi_epi16(s0, dyf0);
			s1 = _mm_mulhi_epi16(s1, dyf1);
			s2 = _mm_mulhi_epi16(s2, dyf2);
			s3 = _mm_mulhi_epi16(s3, dyf3);

			s4 = _mm_add_epi16(s1, s2);
			s5 = _mm_add_epi16(s0, s3);

			dd0 = _mm_add_epi16(s4, s5);

			dd1 = _mm_add_epi16(dd0, d_rnd);
			dd2 = _mm_srai_epi16(dd1, 4);
			dd3 = _mm_packus_epi16(dd2, dd2);

			for (i = 0; i < (width & 7); i++) {
				tdp[i] = ((mlib_u8 *)&dd3)[i];
			}
		}

		y_step = ((y + dy) >> MLIB_SHIFT) - (y >> MLIB_SHIFT);
		y += dy;
		dp += dst_stride;
	}

	__mlib_free(buff);

	return (MLIB_SUCCESS);
}

#else

mlib_status
mlib_s_ImageZoom_BC_U8_12(
    mlib_work_image * param)
{
	VARIABLE(mlib_u8)
	mlib_u8 *filter1 = GetElemStruct(filter1);
	mlib_u8 *tdp;
	mlib_s32 *prow;
	mlib_s32 fpos;
	__m128i *dfPtr;
	__m128i *buff, *buff_arr[8], *buffx, *pbuff;
	__m128i dyf0, dyf1, dyf2, dyf3;
	mlib_s32 width4, width8, k, dx4, bind = 0, y_step = 4;
	mlib_s32 nchan = param->channels;

	if (nchan == 2)
		width *= 2;
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
	for (i = 0, k = 0; i < width4; i++, k += 2) {
		__m128i a0, a1, a2, a3, b0, b1, b2, b3, b4, b5;

		fpos = (x >> (MLIB_SHIFT - 8)) & 0xff;
		x += dx;
		a0 = _mm_loadl_epi64((void *)((mlib_d64 *)filter1 + fpos));
		fpos = (x >> (MLIB_SHIFT - 8)) & 0xff;
		x += dx;
		a1 = _mm_loadl_epi64((void *)((mlib_d64 *)filter1 + fpos));
		fpos = (x >> (MLIB_SHIFT - 8)) & 0xff;
		x += dx;
		a2 = _mm_loadl_epi64((void *)((mlib_d64 *)filter1 + fpos));
		fpos = (x >> (MLIB_SHIFT - 8)) & 0xff;
		x += dx;
		a3 = _mm_loadl_epi64((void *)((mlib_d64 *)filter1 + fpos));

		b0 = _mm_unpacklo_epi16(a0, a2);
		b1 = _mm_unpacklo_epi16(a1, a3);

		b2 = _mm_unpacklo_epi16(b0, b1);
		b3 = _mm_unpackhi_epi16(b0, b1);

		b4 = _mm_sub_epi16(_mm_setzero_si128(), b2);
		b4 = _mm_unpacklo_epi64(b4, _mm_unpackhi_epi64(b2, b2));
		_mm_store_si128((void *)(buffx + k), b4);

		b5 = _mm_sub_epi16(_mm_setzero_si128(), b3);
		b5 = _mm_unpacklo_epi64(b3, _mm_unpackhi_epi64(b5, b5));
		_mm_store_si128((void *)(buffx + k + 1), b5);
	}

	sp -= src_stride + nchan;
	tsp = sp;
	dx4 = 4 * dx;

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
			mlib_s32 t;
			x = GetElemSubStruct(current, srcX) & MLIB_MASK;
			pbuff = buff_arr[bind + k];

			if (nchan == 1) {
				mlib_s32 x1, x2, x3;
				x1 = x + dx;
				x2 = x + 2 * dx;
				x3 = x + 3 * dx;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (t = 0, i = 0; i < width / 4; t += 2, i++) {
					__m128i s0, s1, s2, s3, t0, t1, t2, t3;
					__m128i r0, r1, r2, r3, r4, mzero;
					mlib_u8 *sp0 =
					    tsp + (x >> MLIB_SHIFT);
					mlib_u8 *sp1 =
					    tsp + (x1 >> MLIB_SHIFT);
					mlib_u8 *sp2 =
					    tsp + (x2 >> MLIB_SHIFT);
					mlib_u8 *sp3 =
					    tsp + (x3 >> MLIB_SHIFT);

					mzero = _mm_setzero_si128();
					LOAD_2s32_to_m128(s0, sp0, sp1);
					LOAD_2s32_to_m128(s1, sp2, sp3);
					s2 = _mm_unpacklo_epi16(s0, s1);
					s3 = _mm_unpackhi_epi16(s0, s1);
					t0 = _mm_unpacklo_epi16(s2, s3);
					t1 = _mm_unpackhi_epi16(s2, s3);

					r0 = _mm_mulhi_epu16(t0, buffx[t]);
					r1 = _mm_mulhi_epu16(t1, buffx[t + 1]);
					r2 = _mm_sub_epi16(r0, r1);
					r3 = _mm_unpackhi_epi64(r2, r2);
					r4 = _mm_sub_epi16(r3, r2);
					_mm_storel_epi64((void *)((mlib_d64 *)
					    pbuff + i), r4);
					x += dx4;
					x1 += dx4;
					x2 += dx4;
					x3 += dx4;
				}
				if (width & 3) {
					mlib_s32 leftover = width & 3;
					__m128i s0, s1, s2, s3, t0, t1, t2, t3;
					__m128i r0, r1, r2, r3, r4, mzero;
					mlib_u8 *sp0 =
					    tsp + (x >> MLIB_SHIFT);
					mlib_u8 *sp1 =
					    tsp + (x1 >> MLIB_SHIFT);
					mlib_u8 *sp2 =
					    tsp + (x2 >> MLIB_SHIFT);
					mlib_u8 *sp3 =
					    tsp + (x3 >> MLIB_SHIFT);

					mzero = _mm_setzero_si128();
					switch (leftover) {
					case 1:
						LOAD_s32_to_m128(s0, sp0);
						break;
					case 2:
						LOAD_2s32_to_m128(s0, sp0, sp1);
						break;
					case 3:
						LOAD_2s32_to_m128(s0, sp0, sp1);
						LOAD_s32_to_m128(s1, sp2);
						break;
					default:
						break;
					}

					s2 = _mm_unpacklo_epi16(s0, s1);
					s3 = _mm_unpackhi_epi16(s0, s1);
					t0 = _mm_unpacklo_epi16(s2, s3);
					t1 = _mm_unpackhi_epi16(s2, s3);

					r0 = _mm_mulhi_epu16(t0, buffx[t]);
					r1 = _mm_mulhi_epu16(t1, buffx[t + 1]);
					r2 = _mm_sub_epi16(r0, r1);
					r3 = _mm_unpackhi_epi64(r2, r2);
					r4 = _mm_sub_epi16(r3, r2);
					_mm_storel_epi64((void *)((mlib_d64 *)
					    pbuff + i), r4);
				}
			} else {
				for (t = 0, i = 0; i < width / 8; t += 2, i++) {
					__m128i r0, r1, r2, r3;
					__m128i s0, s1, s2, s3;
					__m128i t0, t1, t2, t3;
					__m128i u0, u1, u2, u3;
					__m128i f0, f1, f2, f3, mzero;
					s0 = _mm_loadl_epi64((void *)(tsp +
					    2 * (x >> MLIB_SHIFT)));
					s1 = _mm_loadl_epi64((void *)(tsp +
					    2 * ((x + dx) >> MLIB_SHIFT)));
					s2 = _mm_loadl_epi64((void *)(tsp +
					    2 * ((x + 2 * dx) >> MLIB_SHIFT)));
					s3 = _mm_loadl_epi64((void *)(tsp +
					    2 * ((x + 3 * dx) >> MLIB_SHIFT)));

					t0 = _mm_unpacklo_epi16(s0, s2);
					t1 = _mm_unpacklo_epi16(s1, s3);
					t2 = _mm_unpacklo_epi16(t0, t1);
					t3 = _mm_unpackhi_epi16(t0, t1);

					mzero = _mm_setzero_si128();
					r0 = _mm_unpacklo_epi8(mzero, t2);
					r1 = _mm_unpackhi_epi8(mzero, t2);
					r2 = _mm_unpacklo_epi8(mzero, t3);
					r3 = _mm_unpackhi_epi8(mzero, t3);

					u0 = buffx[t];
					u1 = buffx[t + 1];
					f0 = _mm_unpacklo_epi16(u0, u0);
					f1 = _mm_unpackhi_epi16(u0, u0);
					f2 = _mm_unpacklo_epi16(u1, u1);
					f3 = _mm_unpackhi_epi16(u1, u1);

					f0 = _mm_mulhi_epu16(r0, f0);
					f1 = _mm_mulhi_epu16(r1, f1);
					f2 = _mm_mulhi_epu16(r2, f2);
					f3 = _mm_mulhi_epu16(r3, f3);

					u2 = _mm_add_epi16(f1, f2);
					u3 = _mm_add_epi16(f0, f3);
					pbuff[i] =
					    _mm_sub_epi16(u2, u3);

					x += dx4;
				}
				if (width & 7) {
					mlib_s32 leftover = width & 7;
					__m128i r0, r1, r2, r3;
					__m128i s0, s1, s2, s3;
					__m128i t0, t1, t2, t3;
					__m128i u0, u1, u2, u3;
					__m128i f0, f1, f2, f3, mzero;
					mlib_u8 *sp0 = tsp +
					    2* (x >> MLIB_SHIFT);
					mlib_u8 *sp1 = tsp +
					    2 * ((x + dx) >> MLIB_SHIFT);
					mlib_u8 *sp2 = tsp +
					    2 * ((x + 2 * dx) >> MLIB_SHIFT);
					mlib_u8 *sp3 = tsp +
					    2 * ((x + 3 * dx) >> MLIB_SHIFT);

					switch (leftover) {
					case 1:
						sse_loadl_epi32(s0, sp0);
						break;
					case 2:
						s0 = _mm_loadl_epi64(
							(void *)sp0);
						break;
					case 3:
						s0 = _mm_loadl_epi64(
							(void *)sp0);
						sse_loadl_epi32(s1, sp1);
						break;
					case 4:
						s0 = _mm_loadl_epi64(
							(void *)sp0);
						s1 = _mm_loadl_epi64(
							(void *)sp1);
						break;
					case 5:
						s0 = _mm_loadl_epi64(
							(void *)sp0);
						s1 = _mm_loadl_epi64(
							(void *)sp1);
						sse_loadl_epi32(s2, sp2);
						break;
					case 6:
						s0 = _mm_loadl_epi64(
							(void *)sp0);
						s1 = _mm_loadl_epi64(
							(void *)sp1);
						s2 = _mm_loadl_epi64(
							(void *)sp2);
						break;
					case 7:
						s0 = _mm_loadl_epi64(
							(void *)sp0);
						s1 = _mm_loadl_epi64(
							(void *)sp1);
						s2 = _mm_loadl_epi64(
							(void *)sp2);
						sse_loadl_epi32(s3, sp3);
						break;
					default:
						break;
					}

					t0 = _mm_unpacklo_epi16(s0, s2);
					t1 = _mm_unpacklo_epi16(s1, s3);
					t2 = _mm_unpacklo_epi16(t0, t1);
					t3 = _mm_unpackhi_epi16(t0, t1);

					mzero = _mm_setzero_si128();
					r0 = _mm_unpacklo_epi8(mzero, t2);
					r1 = _mm_unpackhi_epi8(mzero, t2);
					r2 = _mm_unpacklo_epi8(mzero, t3);
					r3 = _mm_unpackhi_epi8(mzero, t3);

					u0 = buffx[t];
					u1 = buffx[t + 1];
					f0 = _mm_unpacklo_epi16(u0, u0);
					f1 = _mm_unpackhi_epi16(u0, u0);
					f2 = _mm_unpacklo_epi16(u1, u1);
					f3 = _mm_unpackhi_epi16(u1, u1);

					f0 = _mm_mulhi_epu16(r0, f0);
					f1 = _mm_mulhi_epu16(r1, f1);
					f2 = _mm_mulhi_epu16(r2, f2);
					f3 = _mm_mulhi_epu16(r3, f3);

					u2 = _mm_add_epi16(f1, f2);
					u3 = _mm_add_epi16(f0, f3);
					pbuff[i] =
					    _mm_sub_epi16(u2, u3);
				}
			}

			tsp += src_stride;
		}

		__m128i *buff0, *buff1, *buff2, *buff3;

		buff0 = buff_arr[bind];
		buff1 = buff_arr[bind + 1];
		buff2 = buff_arr[bind + 2];
		buff3 = buff_arr[bind + 3];

		tdp = dp;

		__m128i d_rnd = _mm_set1_epi16(8);
		COEFF_D64_U8(y)

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < width / 8; i++) {
			__m128i s0, s1, s2, s3, s4, s5;
			__m128i dd0, dd1, dd2, dd3;

			s0 = buff0[i];
			s0 = _mm_mulhi_epi16(s0, dyf0);
			s1 = buff1[i];
			s1 = _mm_mulhi_epi16(s1, dyf1);
			s2 = buff2[i];
			s2 = _mm_mulhi_epi16(s2, dyf2);
			s3 = buff3[i];
			s3 = _mm_mulhi_epi16(s3, dyf3);

			s4 = _mm_add_epi16(s1, s2);
			s5 = _mm_add_epi16(s0, s3);

			dd0 = _mm_add_epi16(s4, s5);

			dd1 = _mm_add_epi16(dd0, d_rnd);
			dd2 = _mm_srai_epi16(dd1, 4);
			dd3 = _mm_packus_epi16(dd2, dd2);

			_mm_storel_epi64((void *)tdp, dd3);
			tdp += 8;
		}

		if (width & 7) {
			__m128i s0, s1, s2, s3, s4, s5;
			__m128i dd0, dd1, dd2, dd3;

			s0 = buff0[i];
			s1 = buff1[i];
			s2 = buff2[i];
			s3 = buff3[i];

			s0 = _mm_mulhi_epi16(s0, dyf0);
			s1 = _mm_mulhi_epi16(s1, dyf1);
			s2 = _mm_mulhi_epi16(s2, dyf2);
			s3 = _mm_mulhi_epi16(s3, dyf3);

			s4 = _mm_add_epi16(s1, s2);
			s5 = _mm_add_epi16(s0, s3);

			dd0 = _mm_add_epi16(s4, s5);

			dd1 = _mm_add_epi16(dd0, d_rnd);
			dd2 = _mm_srai_epi16(dd1, 4);
			dd3 = _mm_packus_epi16(dd2, dd2);

			for (i = 0; i < (width & 7); i++) {
				tdp[i] = ((mlib_u8 *)&dd3)[i];
			}
		}

		y_step = ((y + dy) >> MLIB_SHIFT) - (y >> MLIB_SHIFT);
		y += dy;
		dp += dst_stride;
	}

	__mlib_free(buff);

	return (MLIB_SUCCESS);
}

#endif /* #ifdef	__i386 */

/* *********************************************************** */
