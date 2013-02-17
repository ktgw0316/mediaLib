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

#pragma ident   "@(#)mlib_s_ImageMulShift_f.c	9.4	07/11/05 SMI"

/*
 * Internal functions.
 *
 *     void mlib_s_ImageMulShift_U8(mlib_u8 *sp1,
 *                                  mlib_s32 stride1,
 *                                  mlib_u8 *sp2,
 *                                  mlib_s32 stride2,
 *                                  mlib_u8 *dp,
 *                                  mlib_s32 strided,
 *                                  mlib_s32 width,
 *                                  mlib_s32 height,
 *                                  mlib_s32 shift);
 *
 *
 *     void mlib_s_ImageMulShift_S16(mlib_s16 *sp1,
 *                                  mlib_s32 stride1,
 *                                  mlib_s16 *sp2,
 *                                  mlib_s32 stride2,
 *                                  mlib_s16 *dp,
 *                                  mlib_s32 strided,
 *                                  mlib_s32 width,
 *                                  mlib_s32 height,
 *                                  mlib_s32 shift);
 *
 *     void mlib_s_ImageMulShift_U16(mlib_u16 *sp1,
 *                                  mlib_s32 stride1,
 *                                  mlib_u16 *sp2,
 *                                  mlib_s32 stride2,
 *                                  mlib_u16 *dp,
 *                                  mlib_s32 strided,
 *                                  mlib_s32 width,
 *                                  mlib_s32 height,
 *                                  mlib_s32 shift);
 *
 */

#include <mlib_image.h>
#include <mlib_ImageCheck.h>
#include <mlib_sse_utils.h>
#include <mlib_s_ImageMulShift.h>

/* *********************************************************** */

#define	P_TYPE __m128i

/* *********************************************************** */

void
mlib_s_ImageMulShift_U8(
    mlib_u8 *sp1,
    mlib_s32 stride1,
    mlib_u8 *sp2,
    mlib_s32 stride2,
    mlib_u8 *dp,
    mlib_s32 strided,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s32 shift)
{

	mlib_u8 *sl1, *sl2, *dl;
	mlib_s32 offsrc1;
	mlib_s32 j, k, t, q;
	P_TYPE mzero;

	mzero = _mm_setzero_si128();

	sl1 = sp1;
	sl2 = sp2;
	dl = dp;

	offsrc1 = ((mlib_addr)sp1) & 0xf;

	if (width == stride1 && width == stride2 && width == strided) {
		width *= height;
		height = 1;
	}

	for (j = 0; j < height; j++) {

		mlib_u8 *src1, *src2, *dst;
		mlib_u8 *ssp1, *ssp2, *ddp;
		P_TYPE *spp1, *spp2, *dpp;

		mlib_s32 offsrc1_l;
		mlib_s32 m, n, begin;

		src1 = sl1;
		src2 = sl2;
		dst = dl;

		offsrc1_l = ((mlib_addr)sl1) & 0xf;

		if ((16 - offsrc1_l) < width) {

			if (offsrc1_l == 0) {
				n = width / 16;
				m = width - n * 16;
				begin = 0;

				spp1 = (__m128i *)sl1;
				spp2 = (__m128i *)sl2;
				dpp = (__m128i *)dl;

				ssp1 = (mlib_u8 *)(sl1 + n * 16);
				ssp2 = (mlib_u8 *)(sl2 + n * 16);
				ddp = (mlib_u8 *)(dl + n * 16);

			} else {
				n = (width - (16 - offsrc1_l)) / 16;
				m = width - (16 - offsrc1_l) - n * 16;
				begin = 16 - offsrc1_l;

				spp1 = (__m128i *)(sl1 + 16 - offsrc1_l);
				spp2 = (__m128i *)(sl2 + 16 - offsrc1_l);
				dpp = (__m128i *)(dl + 16 - offsrc1_l);

				ssp1 = (mlib_u8 *)(sl1
						+ 16 - offsrc1_l + n * 16);
				ssp2 = (mlib_u8 *)(sl2
						+ 16 - offsrc1_l + n * 16);
				ddp = (mlib_u8 *)(dl
						+ 16 - offsrc1_l + n * 16);
			}

		} else {

			n = 0;
			m = 0;
			begin = width;
		}

		for (k = 0; k < begin; k++) {

			mlib_s32 temp;
			temp = ((mlib_s32)(*src1)) * (*src2) >> shift;

			if (temp > 255) {
				*dst = 255;
			} else {
				*dst = temp;
			}

			src1 ++;
			src2 ++;
			dst ++;
		}

		for (t = 0; t < n; t++) {

			P_TYPE s0, s1, t0, t1;

			s0 = _mm_loadu_si128(spp1);
			s1 = _mm_loadu_si128(spp2);

			t0 = _mm_mullo_epi16(
					_mm_unpacklo_epi8(s0, mzero),
					_mm_unpacklo_epi8(s1, mzero));
			t1 = _mm_mullo_epi16(
					_mm_unpackhi_epi8(s0, mzero),
					_mm_unpackhi_epi8(s1, mzero));

			_mm_storeu_si128(dpp, _mm_packus_epi16(
					_mm_srli_epi16(t0, shift),
					_mm_srli_epi16(t1, shift)));

			spp1 ++;
			spp2 ++;
			dpp ++;
		}

		for (q = 0; q < m; q ++) {
			mlib_s32 temp;
			temp = ((mlib_s32)(* ssp1)) * (* ssp2) >> shift;

			if (temp > 255) {
				*ddp = 255;
			} else {
				*ddp = (mlib_u8)temp;
			}

			ssp1 ++;
			ssp2 ++;
			ddp ++;
		}

		sl1 += stride1;
		sl2 += stride2;
		dl += strided;
	}
}

/* ************************************************************ */

void
mlib_s_ImageMulShift_S16(
    mlib_s16 *sp1,
    mlib_s32 stride1,
    mlib_s16 *sp2,
    mlib_s32 stride2,
    mlib_s16 *dp,
    mlib_s32 strided,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s32 shift)
{

	mlib_s16 *sl1, *sl2, *dl;
	mlib_s32 offsrc1;
	mlib_s32 j, k, t, q;

	P_TYPE mzero;
	mzero = _mm_setzero_si128();

	sl1 = sp1;
	sl2 = sp2;
	dl = dp;

	offsrc1 = ((mlib_addr)sp1) & 0xf;

	if (width == stride1 && width == stride2 && width == strided) {
		width *= height;
		height = 1;
	}

	for (j = 0; j < height; j++) {

		mlib_s16 *src1, *src2, *dst;
		mlib_s16 *ssp1, *ssp2, *ddp;
		P_TYPE *spp1, *spp2, *dpp;

		mlib_s32 offsrc1_l;
		mlib_s32 m, n, begin;

		src1 = sl1;
		src2 = sl2;
		dst = dl;

		offsrc1_l = ((mlib_addr)sl1) & 0x7;


		if ((8 - offsrc1_l) < width) {

			if (offsrc1_l == 0) {
				n = width / 8;
				m = width - n * 8;
				begin = 0;

				spp1 = (__m128i *)sl1;
				spp2 = (__m128i *)sl2;
				dpp = (__m128i *)dl;

				ssp1 = (mlib_s16 *)(sl1 + n * 8);
				ssp2 = (mlib_s16 *)(sl2 + n * 8);
				ddp = (mlib_s16 *)(dl + n * 8);

			} else {
				n = (width - (8 - offsrc1_l)) / 8;
				m = width - (8 - offsrc1_l) - n * 8;
				begin = 8 - offsrc1_l;

				spp1 = (__m128i *)(sl1 + 8 - offsrc1_l);
				spp2 = (__m128i *)(sl2 + 8 - offsrc1_l);
				dpp = (__m128i *)(dl + 8 - offsrc1_l);

				ssp1 = (mlib_s16 *)(sl1
						+ 8 - offsrc1_l + n * 8);
				ssp2 = (mlib_s16 *)(sl2
						+ 8 - offsrc1_l + n * 8);
				ddp = (mlib_s16 *)(dl
						+ 8 - offsrc1_l + n * 8);
			}

		} else {

			n = 0;
			m = 0;
			begin = width;
		}

		for (k = 0; k < begin; k++) {

			mlib_s32 temp, temp1;
			temp = (*src1) * (*src2);

			if (temp >= 0) {
				temp1 = temp >> shift;
			} else {
				temp1 = -((-temp) >> shift);
			}

			if (temp1 > 32767) {
				*dst = 32767;
			} else if (temp1 < -32768) {
				*dst = -32768;
			} else {
				*dst = temp1;
			}

			src1 ++;
			src2 ++;
			dst ++;
		}

		for (t = 0; t < n; t++) {

			P_TYPE s0, s1, t0, t1, t2, t3, a0, a1, a2, a3,
			c0, c1, c2, c3, d0, d1,
			e0, e1, f0, f1, g0, h0;

			s0 = _mm_loadu_si128(spp1);
			s1 = _mm_loadu_si128(spp2);

			t0 = _mm_unpacklo_epi16(s0, mzero);
			t1 = _mm_unpackhi_epi16(s0, mzero);
			t2 = _mm_unpacklo_epi16(s1, mzero);
			t3 = _mm_unpackhi_epi16(s1, mzero);

			a0 = _mm_mullo_epi16(t0, t2);
			a1 = _mm_mulhi_epi16(t0, t2);
			a2 = _mm_mullo_epi16(t1, t3);
			a3 = _mm_mulhi_epi16(t1, t3);

			c0 = _mm_srai_epi32(_mm_unpacklo_epi16(a0, a1), shift);
			c1 = _mm_srai_epi32(_mm_unpackhi_epi16(a0, a1), shift);
			c2 = _mm_srai_epi32(_mm_unpacklo_epi16(a2, a3), shift);
			c3 = _mm_srai_epi32(_mm_unpackhi_epi16(a2, a3), shift);

			d0 = _mm_packs_epi32(c0, c1);
			d1 = _mm_packs_epi32(c2, c3);

			e0 = _mm_unpacklo_epi16(d0, d1);
			e1 = _mm_unpackhi_epi16(d0, d1);

			g0 = _mm_unpacklo_epi16(
					_mm_unpacklo_epi32(e0, e1),
					_mm_unpackhi_epi32(e0, e1));

			_mm_storeu_si128(dpp, _mm_unpacklo_epi32(g0,
					_mm_unpackhi_epi64(g0, g0)));

			spp1 ++;
			spp2 ++;
			dpp ++;
		}

		for (q = 0; q < m; q ++) {

			mlib_s32 temp, temp1;
			temp = (* ssp1) * (* ssp2);

			if (temp >= 0) {
				temp1 = temp >> shift;
			} else {
				temp1 = -((-temp) >> shift);
			}

			if (temp1 > 32767) {
				*ddp = 32767;
			} else if (temp1 < -32768) {
				*ddp = -32768;
			} else {
				*ddp = (mlib_s16)temp1;
			}

			ssp1 ++;
			ssp2 ++;
			ddp ++;
		}

		sl1 += stride1;
		sl2 += stride2;
		dl += strided;
	}
}

/* *********************************************************** */

void
mlib_s_ImageMulShift_U16(
    mlib_u16 *sp1,
    mlib_s32 stride1,
    mlib_u16 *sp2,
    mlib_s32 stride2,
    mlib_u16 *dp,
    mlib_s32 strided,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s32 shift)
{

	mlib_u16 *sl1, *sl2, *dl;
	mlib_s32 offdst;
	mlib_s32 j, k, t, q;

	P_TYPE mzero, saturate_U16;

	mzero = _mm_setzero_si128();
	saturate_U16 = _mm_set1_epi32(65535);

	sl1 = sp1;
	sl2 = sp2;
	dl = dp;

	offdst = ((mlib_addr)dp) & 0xf;

	if (width == stride1 && width == stride2 && width == strided) {
		width *= height;
		height = 1;
	}

	for (j = 0; j < height; j++) {

		mlib_u16 *src1, *src2, *dst;
		mlib_u16 *ssp1, *ssp2, *ddp;
		P_TYPE *spp1, *spp2, *dpp;

		mlib_s32 offsrc1_l;
		mlib_s32 m, n, begin;

		src1 = sl1;
		src2 = sl2;
		dst = dl;

		offsrc1_l = ((mlib_addr)sl1) & 0x7;


		if ((8 - offsrc1_l) < width) {

			if (offsrc1_l == 0) {
				n = width / 8;
				m = width - n * 8;
				begin = 0;

				spp1 = (P_TYPE *)sl1;
				spp2 = (P_TYPE *)sl2;
				dpp = (P_TYPE *)dl;

				ssp1 = (mlib_u16 *)(sl1 + n * 8);
				ssp2 = (mlib_u16 *)(sl2 + n * 8);
				ddp = (mlib_u16 *)(dl + n * 8);

			} else {

				n = (width - (8 - offsrc1_l)) / 8;
				m = width - (8 - offsrc1_l) - n * 8;
				begin = 8 - offsrc1_l;

				spp1 = (P_TYPE *)(sl1 + 8 - offsrc1_l);
				spp2 = (P_TYPE *)(sl2 + 8 - offsrc1_l);
				dpp = (P_TYPE *)(dl + 8 - offsrc1_l);

				ssp1 = (mlib_u16 *)(sl1
						+ 8 - offsrc1_l + n * 8);
				ssp2 = (mlib_u16 *)(sl2
						+ 8 - offsrc1_l + n * 8);
				ddp = (mlib_u16 *)(dl
						+ 8 - offsrc1_l + n * 8);
			}

		} else {

			n = 0;
			m = 0;
			begin = width;
		}

		for (k = 0; k < begin; k++) {

#if !defined(_NO_LONGLONG)
			mlib_s64 temp;
			temp = ((mlib_s64)(*src1)) * (*src2) >> shift;
#else
			mlib_u32 temp;
			temp = ((mlib_u32)(*src1)) * (*src2) >> shift;
#endif /* !defined(_NO_LONGLONG) */

			if (temp > 65535) {
				*dst = 65535;
			} else {
				*dst = temp;
			}

			src1 ++;
			src2 ++;
			dst ++;
		}

		for (t = 0; t < n; t++) {

			P_TYPE s0, s1, t0, t1, t2, t3,
			a0, a1, a2, a3, b0, b1, b2, b3,
			mask0, mask1, mask2, mask3,
			n0, n1, n2, n3,
			c0, c1, c2, c3;

			s0 = _mm_loadu_si128(spp1);
			s1 = _mm_loadu_si128(spp2);

			t0 = _mm_unpacklo_epi16(s0, mzero);
			t1 = _mm_unpackhi_epi16(s0, mzero);
			t2 = _mm_unpacklo_epi16(s1, mzero);
			t3 = _mm_unpackhi_epi16(s1, mzero);

			a0 = _mm_mullo_epi16(t0, t2);
			a1 = _mm_mulhi_epu16(t0, t2);
			a2 = _mm_mullo_epi16(t1, t3);
			a3 = _mm_mulhi_epu16(t1, t3);

			b0 = _mm_srli_epi32(_mm_unpacklo_epi16(a0, a1), shift);
			b1 = _mm_srli_epi32(_mm_unpackhi_epi16(a0, a1), shift);
			b2 = _mm_srli_epi32(_mm_unpacklo_epi16(a2, a3), shift);
			b3 = _mm_srli_epi32(_mm_unpackhi_epi16(a2, a3), shift);

			mask0 = _mm_cmpeq_epi32(_mm_srli_epi32(b0, 16), mzero);
			mask1 = _mm_cmpeq_epi32(_mm_srli_epi32(b1, 16), mzero);
			mask2 = _mm_cmpeq_epi32(_mm_srli_epi32(b2, 16), mzero);
			mask3 = _mm_cmpeq_epi32(_mm_srli_epi32(b3, 16), mzero);

			n0 = _mm_or_si128(_mm_andnot_si128(mask0, saturate_U16),
					_mm_and_si128(mask0, b0));
			n1 = _mm_or_si128(_mm_andnot_si128(mask1, saturate_U16),
					_mm_and_si128(mask1, b1));
			n2 = _mm_or_si128(_mm_andnot_si128(mask2, saturate_U16),
					_mm_and_si128(mask2, b2));
			n3 = _mm_or_si128(_mm_andnot_si128(mask3, saturate_U16),
					_mm_and_si128(mask3, b3));

			c0 = _mm_unpacklo_epi16(n0, n1);
			c1 = _mm_unpackhi_epi16(n0, n1);
			c2 = _mm_unpacklo_epi16(n2, n3);
			c3 = _mm_unpackhi_epi16(n2, n3);

			_mm_storeu_si128(dpp, _mm_unpacklo_epi64(
					_mm_unpacklo_epi16(c0, c1),
					_mm_unpacklo_epi16(c2, c3)));

			spp1 ++;
			spp2 ++;
			dpp ++;
		}

		for (q = 0; q < m; q ++) {

#if !defined(_NO_LONGLONG)
			mlib_s64 temp;
			temp = ((mlib_s64)(* ssp1)) * (* ssp2) >> shift;
#else
			mlib_u32 temp;
			temp = ((mlib_u32)(* ssp1)) * (* ssp2) >> shift;
#endif /* !defined(_NO_LONGLONG) */

			if (temp > 65535) {
				*ddp = 65535;
			} else {
				*ddp = temp;
			}


			ssp1 ++;
			ssp2 ++;
			ddp ++;
		}

		sl1 += stride1;
		sl2 += stride2;
		dl += strided;
	}
}

/* *********************************************************** */
