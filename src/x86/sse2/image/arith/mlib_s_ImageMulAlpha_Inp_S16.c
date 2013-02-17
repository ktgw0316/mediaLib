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

#pragma ident   "@(#)mlib_s_ImageMulAlpha_Inp_S16.c	9.4	07/11/05 SMI"

/*
 *  Internal function for mlib_ImageMulAlpha_Inp on S16 data type.
 */

#include <mlib_image.h>
#include <mlib_s_ImageMulAlpha_Inp.h>

#ifdef __SUNPRO_C
#include <sunmedia_intrin.h>
#else
#include <emmintrin.h>
#endif

/* *********************************************************** */

#define	P_TYPE __m128i

/* *********************************************************** */

#define	MUL_ALPHA_S16(src0, src1, dst, temp, temp1)	\
	temp = src0 * src1;	\
	if (temp >= 0) {	\
		temp1 = temp >> 15;	\
	} else {	\
		temp1 = -((-temp) >> 15);	\
	}	\
	if (temp1 > 32767) {	\
		dst = 32767;	\
	} else if (temp1 < -32768) {	\
		dst = -32768;	\
	} else {	\
		dst = temp1;	\
	}

/* *********************************************************** */

void
mlib_s_ImageMulAlpha_Inp_S16_2(
    mlib_s16 *sp,
    mlib_s32 s_stride,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s32 alpha)
{
	mlib_s16 *sl, *dl;

	mlib_s32 i, j, k, t, q;

	P_TYPE mzero = _mm_setzero_si128();

	sl = sp;
	dl = sp;

	width *= 2;

	for (i = 0; i < height; i++) {

		mlib_s16 *src, *dst;
		mlib_s16 *ssp, *ddp;
		P_TYPE *spp, *dpp;

		mlib_s32 offsrc_l;
		mlib_s32 m, n, begin;

		src = sl;
		dst = dl;

		offsrc_l = ((mlib_addr)sl) & 0x7;

		if ((offsrc_l / 2 * 2) != offsrc_l) {
			offsrc_l += 1;
		}

		if ((8 - offsrc_l) < width) {

			if (offsrc_l == 0) {
				n = width / 8;
				m = width - n * 8;
				begin = 0;

				spp = (P_TYPE *)sl;
				dpp = (P_TYPE *)dl;

				ssp = (mlib_s16 *)(sl + n * 8);
				ddp = (mlib_s16 *)(dl + n * 8);

			} else {

				n = (width - (8 - offsrc_l)) / 8;
				m = width - (8 - offsrc_l) - n * 8;
				begin = 8 - offsrc_l;

				spp = (P_TYPE *)(sl + 8 - offsrc_l);
				dpp = (P_TYPE *)(dl + 8 - offsrc_l);

				ssp = (mlib_s16 *)(sl + 8 - offsrc_l + n * 8);
				ddp = (mlib_s16 *)(dl + 8 - offsrc_l + n * 8);
			}
		} else {
			n = 0;
			m = 0;
			begin = width;
		}

		if (alpha == 0) {

			for (k = 0; k < begin / 2; k ++) {
				mlib_s32 temp, temp1;

				dst[0] = src[0];

				MUL_ALPHA_S16(src[0], src[1], dst[1],
						temp, temp1);

				src += 2;
				dst += 2;
			}


			for (t = 0; t < n; t++) {

				P_TYPE s0, a00, a0, a1,
				b0, b1, c0, c1,
				d0, d1, e0, f0, dd;

				s0 = _mm_loadu_si128(spp);

				a0 = _mm_srli_epi32(
						_mm_slli_epi32(s0, 16), 16);
				a1 = _mm_srli_epi32(s0, 16);

				b0 = _mm_mullo_epi16(a0, a1);
				b1 = _mm_mulhi_epi16(a0, a1);

				d0 = _mm_srai_epi32(
						_mm_unpacklo_epi16(b0, b1), 15);
				d1 = _mm_srai_epi32(
						_mm_unpackhi_epi16(b0, b1), 15);

				f0 = _mm_slli_epi32(
						_mm_packs_epi32(d0, d1), 16);

				_mm_storeu_si128(dpp,
					_mm_or_si128(a0, f0));

				spp ++;
				dpp ++;


			}

			for (q = 0; q < m / 2; q++) {
				mlib_s32 temp, temp1;

				ddp[0] = ssp[0];

				MUL_ALPHA_S16(ssp[0], ssp[1], ddp[1],
						temp, temp1);

				ssp += 2;
				ddp += 2;
			}
		} else {

			for (k = 0; k < begin / 2; k ++) {
				mlib_s32 temp, temp1;

				dst[1] = src[1];

				MUL_ALPHA_S16(src[0], src[1], dst[0],
						temp, temp1);

				src += 2;
				dst += 2;
			}


			for (t = 0; t < n; t++) {

				P_TYPE s0, a00, a0, a1, a11,
				b0, b1, c0, c1,
				d0, d1, e0, f0, dd;

				s0 = _mm_loadu_si128(spp);

				a0 = _mm_srli_epi32(_mm_slli_epi32(s0, 16), 16);
				a1 = _mm_srli_epi32(s0, 16);
				a11 = _mm_slli_epi32(a1, 16);

				b0 = _mm_mullo_epi16(a0, a1);
				b1 = _mm_mulhi_epi16(a0, a1);

				d0 = _mm_srai_epi32(
						_mm_unpacklo_epi16(b0, b1), 15);
				d1 = _mm_srai_epi32(
						_mm_unpackhi_epi16(b0, b1), 15);

				e0 = _mm_packs_epi32(d0, d1);

				_mm_storeu_si128(dpp,
					_mm_or_si128(a11, e0));

				spp ++;
				dpp ++;
			}

			for (q = 0; q < m / 2; q++) {
				mlib_s32 temp, temp1;

				ddp[1] = ssp[1];

				MUL_ALPHA_S16(ssp[0], ssp[1], ddp[0],
						temp, temp1);

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
mlib_s_ImageMulAlpha_Inp_S16_4(
    mlib_s16 *sp,
    mlib_s32 s_stride,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s32 alpha)
{

	mlib_s16 *sl, *dl;

	mlib_s32 i, j, k, t, q;

	P_TYPE mzero = _mm_setzero_si128();

	sl = sp;
	dl = sp;

	width *= 4;

	for (i = 0; i < height; i++) {

		mlib_s16 *src, *dst;
		mlib_s16 *ssp, *ddp;
		P_TYPE *spp, *dpp;

		mlib_s32 offsrc_l;
		mlib_s32 m, n, begin;

		src = sl;
		dst = dl;

		offsrc_l = ((mlib_addr)sl) & 0x7;

		if ((offsrc_l - offsrc_l / 4 * 4) == 3) {
			offsrc_l += 1;
		} else if ((offsrc_l - offsrc_l / 4 * 4) == 2) {
			offsrc_l += 2;
		} else if ((offsrc_l - offsrc_l / 4 * 4) == 1) {
			offsrc_l += 3;
		}

		if ((8 - offsrc_l) < width) {

			if (offsrc_l == 0) {
				n = width / 8;
				m = width - n * 8;
				begin = 0;

				spp = (__m128i *)sl;
				dpp = (__m128i *)dl;

				ssp = (mlib_s16 *)(sl + n * 8);
				ddp = (mlib_s16 *)(dl + n * 8);

			} else {

				n = (width - (8 - offsrc_l)) / 8;
				m = width - (8 - offsrc_l) - n * 8;
				begin = 8 - offsrc_l;

				spp = (__m128i *)(sl + 8 - offsrc_l);
				dpp = (__m128i *)(dl + 8 - offsrc_l);

				ssp = (mlib_s16 *)(sl + 8 - offsrc_l + n * 8);
				ddp = (mlib_s16 *)(dl + 8 - offsrc_l + n * 8);
			}
		} else {
			n = 0;
			m = 0;
			begin = width;
		}

		if (alpha == 0) {

			for (k = 0; k < begin / 4; k ++) {
				mlib_s32 temp1, temp2, temp3,
				temp11, temp22, temp33;

				dst[0] = src[0];

				MUL_ALPHA_S16(src[0], src[1], dst[1],
						temp1, temp11);
				MUL_ALPHA_S16(src[0], src[2], dst[2],
						temp2, temp22);
				MUL_ALPHA_S16(src[0], src[3], dst[3],
						temp3, temp33);

				src += 4;
				dst += 4;
			}


			for (t = 0; t < n; t++) {

				P_TYPE s0, a0, a1, a2, a11, a12,
				b0, b1, c0, c1, c2, c3,
				d0, d1, d2, d3, d,
				e0, e1, e2, e3,
				f0, f1, g0, g1, h0, h1;

				s0 = _mm_loadu_si128(spp);

				a0 = _mm_srli_epi64(_mm_slli_epi64(s0, 48), 48);

				a11 = _mm_unpacklo_epi32(a0, a0);
				a12 = _mm_unpackhi_epi32(a0, a0);
				a1 = _mm_unpacklo_epi32(a11, a11);
				a2 = _mm_unpacklo_epi32(a12, a12);

				b0 = _mm_unpacklo_epi16(s0, mzero);
				b1 = _mm_unpackhi_epi16(s0, mzero);

				c0 = _mm_mullo_epi16(b0, a1);
				c1 = _mm_mulhi_epi16(b0, a1);
				c2 = _mm_mullo_epi16(b1, a2);
				c3 = _mm_mulhi_epi16(b1, a2);

				e0 = _mm_srai_epi32(
						_mm_unpacklo_epi16(c0, c1), 15);
				e1 = _mm_srai_epi32(
						_mm_unpackhi_epi16(c0, c1), 15);
				e2 = _mm_srai_epi32(
						_mm_unpacklo_epi16(c2, c3), 15);
				e3 = _mm_srai_epi32(
						_mm_unpackhi_epi16(c2, c3), 15);

				f0 = _mm_packs_epi32(e0, e1);
				f1 = _mm_packs_epi32(e2, e3);

				g0 = _mm_unpacklo_epi16(f0, f1);
				g1 = _mm_unpackhi_epi16(f0, f1);

				h0 = _mm_unpacklo_epi32(g0, g1);
				h1 = _mm_unpackhi_epi32(g0, g1);

				d0 = _mm_unpacklo_epi16(h0, h1);
				d1 = _mm_srli_si128(d0, 8);

				d2 = _mm_unpacklo_epi32(d0, d1);

				d3 = _mm_srli_epi64(d2, 16);
				d = _mm_slli_epi64(d3, 16);

				_mm_storeu_si128(dpp,
					_mm_or_si128(d, a0));

				spp ++;
				dpp ++;
			}

			for (q = 0; q < m / 4; q++) {

				mlib_s32 temp1, temp2, temp3,
				temp11, temp22, temp33;

				ddp[0] = ssp[0];

				MUL_ALPHA_S16(ssp[0], ssp[1], ddp[1],
						temp1, temp11);
				MUL_ALPHA_S16(ssp[0], ssp[2], ddp[2],
						temp2, temp22);
				MUL_ALPHA_S16(ssp[0], ssp[3], ddp[3],
						temp3, temp33);

				ssp += 4;
				ddp += 4;
			}
		} else if (alpha == 1) {
			for (k = 0; k < begin / 4; k ++) {

				mlib_s32 temp0, temp2, temp3,
				temp00, temp22, temp33;

				dst[1] = src[1];

				MUL_ALPHA_S16(src[1], src[0], dst[0],
						temp0, temp00);
				MUL_ALPHA_S16(src[1], src[2], dst[2],
						temp2, temp22);
				MUL_ALPHA_S16(src[1], src[3], dst[3],
						temp3, temp33);

				src += 4;
				dst += 4;
			}


			for (t = 0; t < n; t++) {

				P_TYPE s0, a01, a11, a0, a1, a2, a12,
				b0, b1, c0, c1, c2, c3,
				d0, d1, d2, d3, d,
				e0, e1, e2, e3,
				f0, f1, g0, g1, h0, h1;

				s0 = _mm_loadu_si128(spp);

				a01 = _mm_srli_epi64(
						_mm_slli_epi64(s0, 32), 48);
				a0 = _mm_slli_epi64(a01, 16);

				a11 = _mm_unpacklo_epi32(a01, a01);
				a12 = _mm_unpackhi_epi32(a01, a01);
				a1 = _mm_unpacklo_epi32(a11, a11);
				a2 = _mm_unpacklo_epi32(a12, a12);

				b0 = _mm_unpacklo_epi16(s0, mzero);
				b1 = _mm_unpackhi_epi16(s0, mzero);

				c0 = _mm_mullo_epi16(b0, a1);
				c1 = _mm_mulhi_epi16(b0, a1);
				c2 = _mm_mullo_epi16(b1, a2);
				c3 = _mm_mulhi_epi16(b1, a2);

				e0 = _mm_srai_epi32(
						_mm_unpacklo_epi16(c0, c1), 15);
				e1 = _mm_srai_epi32(
						_mm_unpackhi_epi16(c0, c1), 15);
				e2 = _mm_srai_epi32(
						_mm_unpacklo_epi16(c2, c3), 15);
				e3 = _mm_srai_epi32(
						_mm_unpackhi_epi16(c2, c3), 15);

				f0 = _mm_packs_epi32(e0, e1);
				f1 = _mm_packs_epi32(e2, e3);

				g0 = _mm_unpacklo_epi16(f0, f1);
				g1 = _mm_unpackhi_epi16(f0, f1);

				h0 = _mm_unpacklo_epi32(g0, g1);
				h1 = _mm_unpackhi_epi32(g0, g1);

				d0 = _mm_unpacklo_epi16(h0, h1);
				d1 = _mm_srli_si128(d0, 8);
				d2 = _mm_srli_epi64(
						_mm_unpacklo_epi32(d0, d1), 32);
				d3 = _mm_slli_epi64(
						_mm_unpacklo_epi32(d0, d1), 48);

				d = _mm_or_si128(_mm_slli_epi64(d2, 32),
						_mm_srli_epi64(d3, 48));

				_mm_storeu_si128(dpp,
					_mm_or_si128(d, a0));

				spp ++;
				dpp ++;

			}

			for (q = 0; q < m / 4; q++) {
				mlib_s32 temp0, temp2, temp3,
				temp00, temp22, temp33;

				ddp[1] = ssp[1];

				MUL_ALPHA_S16(ssp[1], ssp[0], ddp[0],
						temp0, temp00);
				MUL_ALPHA_S16(ssp[1], ssp[2], ddp[2],
						temp2, temp22);
				MUL_ALPHA_S16(ssp[1], ssp[3], ddp[3],
						temp3, temp33);

				ssp += 4;
				ddp += 4;
			}

		} else if (alpha == 2) {

			for (k = 0; k < begin / 4; k ++) {

				mlib_s32 temp0, temp1, temp3,
				temp11, temp00, temp33;

				dst[2] = src[2];

				MUL_ALPHA_S16(src[2], src[0], dst[0],
						temp0, temp00);
				MUL_ALPHA_S16(src[2], src[1], dst[1],
						temp1, temp11);
				MUL_ALPHA_S16(src[2], src[3], dst[3],
						temp3, temp33);

				src += 4;
				dst += 4;
			}


			for (t = 0; t < n; t++) {

				P_TYPE s0, a01, a11, a0, a1, a2, a12,
				b0, b1, c0, c1, c2, c3,
				d0, d1, d2, d3, d4, d,
				e0, e1, e2, e3,
				f0, f1, g0, g1, h0, h1;

				s0 = _mm_loadu_si128(spp);

				a01 = _mm_srli_epi64(
						_mm_slli_epi64(s0, 16), 48);
				a0 = _mm_slli_epi64(a01, 32);

				a11 = _mm_unpacklo_epi32(a01, a01);
				a12 = _mm_unpackhi_epi32(a01, a01);
				a1 = _mm_unpacklo_epi32(a11, a11);
				a2 = _mm_unpacklo_epi32(a12, a12);

				b0 = _mm_unpacklo_epi16(s0, mzero);
				b1 = _mm_unpackhi_epi16(s0, mzero);

				c0 = _mm_mullo_epi16(b0, a1);
				c1 = _mm_mulhi_epi16(b0, a1);
				c2 = _mm_mullo_epi16(b1, a2);
				c3 = _mm_mulhi_epi16(b1, a2);

				e0 = _mm_srai_epi32(
						_mm_unpacklo_epi16(c0, c1), 15);
				e1 = _mm_srai_epi32(
						_mm_unpackhi_epi16(c0, c1), 15);
				e2 = _mm_srai_epi32(
						_mm_unpacklo_epi16(c2, c3), 15);
				e3 = _mm_srai_epi32(
						_mm_unpackhi_epi16(c2, c3), 15);

				f0 = _mm_packs_epi32(e0, e1);
				f1 = _mm_packs_epi32(e2, e3);

				g0 = _mm_unpacklo_epi16(f0, f1);
				g1 = _mm_unpackhi_epi16(f0, f1);

				h0 = _mm_unpacklo_epi32(g0, g1);
				h1 = _mm_unpackhi_epi32(g0, g1);

				d0 = _mm_unpacklo_epi16(h0, h1);
				d1 = _mm_srli_si128(d0, 8);
				d2 = _mm_unpacklo_epi32(d0, d1);
				d3 = _mm_srli_epi64(_mm_slli_epi64(d2, 32), 32);
				d4 = _mm_slli_epi64(_mm_srli_epi64(d2, 48), 48);

				d = _mm_or_si128(d3, d4);

				_mm_storeu_si128(dpp,
					_mm_or_si128(d, a0));

				spp ++;
				dpp ++;

			}

			for (q = 0; q < m / 4; q++) {

				mlib_s32 temp0, temp1, temp3,
				temp11, temp00, temp33;

				ddp[2] = ssp[2];

				MUL_ALPHA_S16(ssp[2], ssp[0], ddp[0],
						temp0, temp00);
				MUL_ALPHA_S16(ssp[2], ssp[1], ddp[1],
						temp1, temp11);
				MUL_ALPHA_S16(ssp[2], ssp[3], ddp[3],
						temp3, temp33);

				ssp += 4;
				ddp += 4;
			}
		} else {


			for (k = 0; k < begin / 4; k ++) {

				mlib_s32 temp1, temp2, temp0,
				temp11, temp22, temp00;

				dst[3] = src[3];

				MUL_ALPHA_S16(src[3], src[0], dst[0],
						temp0, temp00);
				MUL_ALPHA_S16(src[3], src[1], dst[1],
						temp1, temp11);
				MUL_ALPHA_S16(src[3], src[2], dst[2],
						temp2, temp22);

				src += 4;
				dst += 4;
			}


			for (t = 0; t < n; t++) {

				P_TYPE s0, a01, a11, a0, a1, a2, a12,
				b0, b1, c0, c1, c2, c3,
				d0, d1, d2, d3, d,
				e0, e1, e2, e3,
				f0, f1, g0, g1, h0, h1;

				s0 = _mm_loadu_si128(spp);

				a01 = _mm_srli_epi64(s0, 48);
				a0 = _mm_slli_epi64(a01, 48);

				a11 = _mm_unpacklo_epi32(a01, a01);
				a12 = _mm_unpackhi_epi32(a01, a01);
				a1 = _mm_unpacklo_epi32(a11, a11);
				a2 = _mm_unpacklo_epi32(a12, a12);

				b0 = _mm_unpacklo_epi16(s0, mzero);
				b1 = _mm_unpackhi_epi16(s0, mzero);

				c0 = _mm_mullo_epi16(b0, a1);
				c1 = _mm_mulhi_epi16(b0, a1);
				c2 = _mm_mullo_epi16(b1, a2);
				c3 = _mm_mulhi_epi16(b1, a2);

				e0 = _mm_srai_epi32(
						_mm_unpacklo_epi16(c0, c1), 15);
				e1 = _mm_srai_epi32(
						_mm_unpackhi_epi16(c0, c1), 15);
				e2 = _mm_srai_epi32(
						_mm_unpacklo_epi16(c2, c3), 15);
				e3 = _mm_srai_epi32(
						_mm_unpackhi_epi16(c2, c3), 15);

				f0 = _mm_packs_epi32(e0, e1);
				f1 = _mm_packs_epi32(e2, e3);

				g0 = _mm_unpacklo_epi16(f0, f1);
				g1 = _mm_unpackhi_epi16(f0, f1);

				h0 = _mm_unpacklo_epi32(g0, g1);
				h1 = _mm_unpackhi_epi32(g0, g1);

				d0 = _mm_unpacklo_epi16(h0, h1);
				d1 = _mm_srli_si128(d0, 8);
				d2 = _mm_unpacklo_epi32(d0, d1);
				d3 = _mm_slli_epi64(d2, 16);

				d = _mm_srli_epi64(d3, 16);

				_mm_storeu_si128(dpp,
					_mm_or_si128(d, a0));

				spp ++;
				dpp ++;

			}

			for (q = 0; q < m / 4; q++) {
				mlib_s32 temp1, temp2, temp0,
				temp11, temp22, temp00;

				ddp[3] = ssp[3];

				MUL_ALPHA_S16(ssp[3], ssp[0], ddp[0],
						temp0, temp00);
				MUL_ALPHA_S16(ssp[3], ssp[1], ddp[1],
						temp1, temp11);
				MUL_ALPHA_S16(ssp[3], ssp[2], ddp[2],
						temp2, temp22);

				ssp += 4;
				ddp += 4;
			}
		}

		sl += s_stride;
		dl += s_stride;
	}
}
