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

#pragma ident	"@(#)mlib_s_ImageMulAlpha_Inp_U8.c	9.5	07/11/05 SMI"

/*
 *  Internal function for mlib_ImageMulAlpha_Inp on U8 data type.
 */

#include <mlib_image.h>
#include <mlib_s_ImageMulAlpha_Inp.h>

#ifdef __SUNPRO_C
#include <sunmedia_intrin.h>
#else
#include <emmintrin.h>
#endif

/* *********************************************************** */

#define	MUL_ALPHA_U8(src0, src1, temp, dst) \
	temp = ((mlib_s32)src0) * src1 >> 8; \
	if (temp > 255) {	\
		dst = 255;	\
	} else {	\
		dst = temp;	\
	}

/* *********************************************************** */

#define	P_TYPE __m128i

/* *********************************************************** */

void
mlib_s_ImageMulAlpha_Inp_U8_2(
    mlib_u8 *sp,
    mlib_s32 s_stride,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s32 alpha)
{
	mlib_u8 *sl, *dl;
	mlib_s32 i, j, k, t, q;
	mlib_s32 temp;

	P_TYPE mzero = _mm_setzero_si128();

	sl = sp;
	dl = sp;

	width *= 2;

	for (i = 0; i < height; i++) {

		mlib_u8 *src, *dst;
		mlib_u8 *ssp, *ddp;
		P_TYPE *spp, *dpp;

		mlib_s32 offsrc_l;
		mlib_s32 m, n, begin;

		src = sl;
		dst = dl;

		offsrc_l = ((mlib_addr)sl) & 0xf;

		if ((offsrc_l / 2 * 2) != offsrc_l) {
			offsrc_l += 1;
		}

		if ((16 - offsrc_l) < width) {
			if (offsrc_l == 0) {
				n = width / 16;
				m = width - n * 16;
				begin = 0;

				spp = (__m128i *)sl;
				dpp = (__m128i *)dl;

				ssp = (mlib_u8 *)(sl + n * 16);
				ddp = (mlib_u8 *)(dl + n * 16);

			} else {

				n = (width - (16 - offsrc_l)) / 16;
				m = width - (16 - offsrc_l) - n * 16;
				begin = 16 - offsrc_l;

				spp = (__m128i *)(sl + 16 - offsrc_l);
				dpp = (__m128i *)(dl + 16 - offsrc_l);

				ssp = (mlib_u8 *)(sl + 16 - offsrc_l + n * 16);
				ddp = (mlib_u8 *)(dl + 16 - offsrc_l + n * 16);
			}

		} else {
			n = 0;
			m = 0;
			begin = width;
		}

		if (alpha == 0) {

			for (k = 0; k < begin/ 2; k ++) {

				mlib_s32 temp;

				dst[0] = src[0];
				MUL_ALPHA_U8(src[0], src[1], temp, dst[1]);

				src += 2;
				dst += 2;
			}

			for (t = 0; t < n; t++) {

				P_TYPE s0, t0, t1, c1,
				d0, d1, e0, e1;

				s0 = _mm_loadu_si128(spp);

				t0 = _mm_srli_epi16(s0, 8);
				t1 = _mm_srli_epi16(_mm_slli_epi16(s0, 8), 8);

				c1 = _mm_srli_epi16(
						_mm_mullo_epi16(t0, t1), 8);

				d0 = _mm_unpacklo_epi8(t1, c1);
				d1 = _mm_unpackhi_epi8(t1, c1);

				e0 = _mm_unpacklo_epi16(d0, d1);
				e1 = _mm_unpackhi_epi16(d0, d1);


				_mm_storeu_si128(dpp, _mm_unpacklo_epi32(
					_mm_unpacklo_epi16(e0,
						_mm_srli_si128(e0, 8)),
					_mm_unpacklo_epi16(e1,
						_mm_srli_si128(e1, 8))));

				spp ++;
				dpp ++;
			}

			for (q = 0; q < m / 2; q++) {

				mlib_s32 temp;

				ddp[0] = ssp[0];

				MUL_ALPHA_U8(ssp[0], ssp[1], temp, ddp[1]);

				ssp += 2;
				ddp += 2;
			}
		} else {

			for (k = 0; k < begin/ 2; k ++) {

				mlib_s32 temp;

				dst[1] = src[1];
				MUL_ALPHA_U8(src[0], src[1], temp, dst[0]);

				src += 2;
				dst += 2;
			}

			for (t = 0; t < n; t++) {

				P_TYPE s0, t0, t1, c1,
				d0, d1, e0, e1;

				s0 = _mm_loadu_si128(spp);

				t0 = _mm_srli_epi16(s0, 8);
				t1 = _mm_srli_epi16(_mm_slli_epi16(s0, 8), 8);

				c1 = _mm_srli_epi16(_mm_mullo_epi16(t0, t1), 8);

				d0 = _mm_unpacklo_epi8(c1, t0);
				d1 = _mm_unpackhi_epi8(c1, t0);

				e0 = _mm_unpacklo_epi16(d0, d1);
				e1 = _mm_unpackhi_epi16(d0, d1);

				_mm_storeu_si128(dpp,
					_mm_unpacklo_epi32(
					_mm_unpacklo_epi16(e0,
					_mm_srli_si128(e0, 8)),
					_mm_unpacklo_epi16(e1,
					_mm_srli_si128(e1, 8))));

				spp ++;
				dpp ++;
			}

			for (q = 0; q < m / 2; q++) {

				mlib_s32 temp;

				ddp[1] = ssp[1];
				MUL_ALPHA_U8(ssp[0], ssp[1], temp, ddp[0]);

				ssp += 2;
				ddp += 2;
			}
		}

		sl += s_stride;
		dl += s_stride;
	}
}

/* *********************************************************** */

void
mlib_s_ImageMulAlpha_Inp_U8_4(
    mlib_u8 *sp,
    mlib_s32 s_stride,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s32 alpha)
{

	mlib_u8 *sl, *dl;
	mlib_s32 i, j, k, t, q;

	P_TYPE mzero = _mm_setzero_si128();

	sl = sp;
	dl = sp;

	width *= 4;

	for (i = 0; i < height; i ++) {

		mlib_u8 *src, *dst;
		mlib_u8 *ssp, *ddp;

		P_TYPE *spp, *dpp;

		mlib_s32 offdst_l, offsrc_l;

		mlib_s32 m, n, begin;

		src = sl;
		dst = sl;

		offsrc_l = ((mlib_addr)sl) & 0xf;


		if ((offsrc_l - offsrc_l / 4 * 4) == 3) {
			offsrc_l += 1;
		} else if ((offsrc_l - offsrc_l / 4 * 4) == 2) {
			offsrc_l += 2;
		} else if ((offsrc_l - offsrc_l / 4 * 4) == 1) {
			offsrc_l += 3;
		}

		if ((16 - offsrc_l) < width) {
			if (offsrc_l == 0) {
				n = width / 16;
				m = width - n * 16;
				begin = 0;

				spp = (P_TYPE *)sl;
				dpp = (P_TYPE *)dl;

				ssp = (mlib_u8 *)(sl + n * 16);
				ddp = (mlib_u8 *)(dl + n * 16);
			} else {
				n = (width - (16 - offsrc_l)) / 16;
				m = width - (16 - offsrc_l) - n * 16;
				begin = 16 - offsrc_l;

				spp = (P_TYPE *)(sl + 16 - offsrc_l);
				dpp = (P_TYPE *)(dl + 16 - offsrc_l);

				ssp = (mlib_u8 *)(sl + 16 - offsrc_l + n * 16);
				ddp = (mlib_u8 *)(dl + 16 - offsrc_l + n * 16);
			}
		} else {
			n = 0;
			m = 0;
			begin = width;
		}


		if (alpha == 0) {

			for (k = 0; k < begin / 4; k++) {

				mlib_s32 temp1, temp2, temp3;

				dst[0] = src[0];

				MUL_ALPHA_U8(src[0], src[1], temp1, dst[1]);
				MUL_ALPHA_U8(src[0], src[2], temp2, dst[2]);
				MUL_ALPHA_U8(src[0], src[3], temp3, dst[3]);

				src += 4;
				dst += 4;
			}

			for (t = 0; t < n; t ++) {

				P_TYPE s0, a0, b0, b1,
				b0_0, b0_1, b1_0, b1_1,
				d0, d1, e0, e1, f0, f1,
				g0, h0;

				s0 = _mm_loadu_si128(spp);

				a0 = _mm_srli_epi32(_mm_slli_epi32(s0, 24), 24);

				b0 = _mm_unpacklo_epi16(a0, a0);
				b1 = _mm_unpackhi_epi16(a0, a0);

				b0_0 = _mm_unpacklo_epi32(b0, b0);
				b0_1 = _mm_unpackhi_epi32(b0, b0);

				b1_0 = _mm_unpacklo_epi32(b1, b1);
				b1_1 = _mm_unpackhi_epi32(b1, b1);

				d0 = _mm_unpacklo_epi64(b0_0, b0_1);
				d1 = _mm_unpacklo_epi64(b1_0, b1_1);

				e0 = _mm_unpacklo_epi8(s0, mzero);
				e1 = _mm_unpackhi_epi8(s0, mzero);

				f0 = _mm_srli_epi16(_mm_mullo_epi16(d0, e0), 8);
				f1 = _mm_srli_epi16(_mm_mullo_epi16(d1, e1), 8);

				g0 = _mm_packus_epi16(f0, f1);
				h0 = _mm_slli_epi32(_mm_srli_epi32(g0, 8), 8);

				_mm_storeu_si128(dpp,
					_mm_or_si128(h0, a0));

				spp ++;
				dpp ++;
			}

			for (q = 0; q < m / 4; q++) {
				mlib_s32 temp1, temp2, temp3;

				ddp[0] = ssp[0];

				MUL_ALPHA_U8(ssp[0], ssp[1], temp1, ddp[1]);
				MUL_ALPHA_U8(ssp[0], ssp[2], temp2, ddp[2]);
				MUL_ALPHA_U8(ssp[0], ssp[3], temp3, ddp[3]);

				ssp += 4;
				ddp += 4;
			}

		} else if (alpha == 1) {

			for (k = 0; k < begin / 4; k++) {
				mlib_s32 temp0, temp2, temp3;

				dst[1] = src[1];

				MUL_ALPHA_U8(src[1], src[0], temp0, dst[0]);
				MUL_ALPHA_U8(src[1], src[2], temp2, dst[2]);
				MUL_ALPHA_U8(src[1], src[3], temp3, dst[3]);

				src += 4;
				dst += 4;
			}

			for (t = 0; t < n; t ++) {

				P_TYPE s0, a0, a1, b0, b1,
				b0_0, b0_1, b1_0, b1_1,
				d0, d1, e0, e1, f0, f1,
				g0, h0, h, h1;

				s0 = _mm_loadu_si128(spp);

				a0 = _mm_srli_epi32(_mm_slli_epi32(s0, 16), 24);
				a1 = _mm_slli_epi32(a0, 8);

				b0 = _mm_unpacklo_epi16(a0, a0);
				b1 = _mm_unpackhi_epi16(a0, a0);

				b0_0 = _mm_unpacklo_epi32(b0, b0);
				b0_1 = _mm_unpackhi_epi32(b0, b0);

				b1_0 = _mm_unpacklo_epi32(b1, b1);
				b1_1 = _mm_unpackhi_epi32(b1, b1);

				d0 = _mm_unpacklo_epi64(b0_0, b0_1);
				d1 = _mm_unpacklo_epi64(b1_0, b1_1);

				e0 = _mm_unpacklo_epi8(s0, mzero);
				e1 = _mm_unpackhi_epi8(s0, mzero);

				f0 = _mm_srli_epi16(_mm_mullo_epi16(d0, e0), 8);
				f1 = _mm_srli_epi16(_mm_mullo_epi16(d1, e1), 8);

				g0 = _mm_packus_epi16(f0, f1);

				h0 = _mm_slli_epi32(_mm_srli_epi32(g0, 16), 16);
				h1 = _mm_srli_epi32(_mm_slli_epi32(g0, 24), 24);

				h = _mm_or_si128(h0, h1);

				_mm_storeu_si128(dpp,
					_mm_or_si128(h, a1));

				spp ++;
				dpp ++;
			}

			for (q = 0; q < m / 4; q++) {

				mlib_s32 temp0, temp2, temp3;

				ddp[1] = ssp[1];

				MUL_ALPHA_U8(ssp[1], ssp[0], temp0, ddp[0]);
				MUL_ALPHA_U8(ssp[1], ssp[2], temp2, ddp[2]);
				MUL_ALPHA_U8(ssp[1], ssp[3], temp3, ddp[3]);

				ssp += 4;
				ddp += 4;
			}
		} else if (alpha == 2) {

			for (k = 0; k < begin / 4; k++) {

				mlib_s32 temp0, temp1, temp3;

				dst[2] = src[2];

				MUL_ALPHA_U8(src[2], src[0], temp0, dst[0]);
				MUL_ALPHA_U8(src[2], src[1], temp1, dst[1]);
				MUL_ALPHA_U8(src[2], src[3], temp3, dst[3]);

				src += 4;
				dst += 4;
			}

			for (t = 0; t < n; t ++) {

				P_TYPE s0, a0, a1, b0, b1,
				b0_0, b0_1, b1_0, b1_1,
				d0, d1, e0, e1, f0, f1,
				g0, h0, h1, h;

				s0 = _mm_loadu_si128(spp);

				a0 = _mm_srli_epi32(_mm_slli_epi32(s0, 8), 24);
				a1 = _mm_slli_epi32(a0, 16);

				b0 = _mm_unpacklo_epi16(a0, a0);
				b1 = _mm_unpackhi_epi16(a0, a0);

				b0_0 = _mm_unpacklo_epi32(b0, b0);
				b0_1 = _mm_unpackhi_epi32(b0, b0);

				b1_0 = _mm_unpacklo_epi32(b1, b1);
				b1_1 = _mm_unpackhi_epi32(b1, b1);

				d0 = _mm_unpacklo_epi64(b0_0, b0_1);
				d1 = _mm_unpacklo_epi64(b1_0, b1_1);

				e0 = _mm_unpacklo_epi8(s0, mzero);
				e1 = _mm_unpackhi_epi8(s0, mzero);

				f0 = _mm_srli_epi16(_mm_mullo_epi16(d0, e0), 8);
				f1 = _mm_srli_epi16(_mm_mullo_epi16(d1, e1), 8);

				g0 = _mm_packus_epi16(f0, f1);

				h0 = _mm_srli_epi32(_mm_slli_epi32(g0, 16), 16);
				h1 = _mm_slli_epi32(_mm_srli_epi32(g0, 24), 24);
				h = _mm_or_si128(h0, h1);

				_mm_storeu_si128(dpp,
					_mm_or_si128(h, a1));

				spp ++;
				dpp ++;
			}

			for (q = 0; q < m / 4; q++) {

				mlib_s32 temp0, temp1, temp3;

				ddp[2] = ssp[2];

				MUL_ALPHA_U8(ssp[2], ssp[0], temp0, ddp[0]);
				MUL_ALPHA_U8(ssp[2], ssp[1], temp1, ddp[1]);
				MUL_ALPHA_U8(ssp[2], ssp[3], temp3, ddp[3]);

				ssp += 4;
				ddp += 4;
			}
		} else {

			for (k = 0; k < begin / 4; k++) {


				mlib_s32 temp0, temp1, temp2;

				dst[3] = src[3];

				MUL_ALPHA_U8(src[3], src[0], temp0, dst[0]);
				MUL_ALPHA_U8(src[3], src[1], temp1, dst[1]);
				MUL_ALPHA_U8(src[3], src[2], temp2, dst[2]);

				src += 4;
				dst += 4;
			}


			for (t = 0; t < n; t ++) {

				P_TYPE s0, a0, a1, b0, b1,
				b0_0, b0_1, b1_0, b1_1,
				d0, d1, e0, e1, f0, f1,
				g0, h0, h;

				s0 = _mm_loadu_si128(spp);

				a0 = _mm_srli_epi32(s0, 24);
				a1 = _mm_slli_epi32(a0, 24);

				b0 = _mm_unpacklo_epi16(a0, a0);
				b1 = _mm_unpackhi_epi16(a0, a0);

				b0_0 = _mm_unpacklo_epi32(b0, b0);
				b0_1 = _mm_unpackhi_epi32(b0, b0);

				b1_0 = _mm_unpacklo_epi32(b1, b1);
				b1_1 = _mm_unpackhi_epi32(b1, b1);

				d0 = _mm_unpacklo_epi64(b0_0, b0_1);
				d1 = _mm_unpacklo_epi64(b1_0, b1_1);

				e0 = _mm_unpacklo_epi8(s0, mzero);
				e1 = _mm_unpackhi_epi8(s0, mzero);

				f0 = _mm_srli_epi16(_mm_mullo_epi16(d0, e0), 8);
				f1 = _mm_srli_epi16(_mm_mullo_epi16(d1, e1), 8);

				g0 = _mm_packus_epi16(f0, f1);

				h = _mm_srli_epi32(_mm_slli_epi32(g0, 8), 8);

				_mm_storeu_si128(dpp,
					_mm_or_si128(h, a1));

				spp ++;
				dpp ++;
			}

			for (q = 0; q < m / 4; q++) {

				mlib_s32 temp0, temp1, temp2;

				ddp[3] = ssp[3];

				MUL_ALPHA_U8(ssp[3], ssp[0], temp0, ddp[0]);
				MUL_ALPHA_U8(ssp[3], ssp[1], temp1, ddp[1]);
				MUL_ALPHA_U8(ssp[3], ssp[2], temp2, ddp[2]);

				ssp += 4;
				ddp += 4;
			}
		}

		sl += s_stride;
		dl += s_stride;
	}
}

/* *********************************************************** */
