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

#pragma ident	"@(#)mlib_s_ImageBlendColor_Inp_U8.c	9.4	07/11/05 SMI"

/*
 *  Internal function for mlib_ImageBlendColor_Inp on U8 data type.
 */

#include <mlib_image.h>
#include <mlib_s_ImageBlendColor_Inp.h>

#ifdef __SUNPRO_C
#include <sunmedia_intrin.h>
#else
#include <emmintrin.h>
#endif


/* *********************************************************** */

#define	P_TYPE __m128i

/* *********************************************************** */

#define	BLEND_COLOR_INP_U8(dst, color, src0, src1)	\
		dst = src0 * src1	\
			+ (256 - src0) * color >> 8;

/* *********************************************************** */

#define	BLEND_COLOR_INP_U8_2(spp, mcolor, TYPE)	\
	\
	P_TYPE s0, a00, a0, a1, c1, b, b0, b1,	\
	d0, d1, e0, e1, f0, f1, g, g0, g1, h;	\
	\
	s0 = _mm_loadu_si128(spp);	\
	\
	a00 = _mm_slli_epi16(s0, 8);	\
	a0 = _mm_srli_epi16(a00, 8);	\
	a1 = _mm_srli_epi16(s0, 8);		\
	\
	b = _mm_mullo_epi16(a0, a1);	\
	b0 = _mm_unpacklo_epi16(b, mzero);	\
	b1 = _mm_unpackhi_epi16(b, mzero);	\
	\
	c1 = _mm_sub_epi32(m256, a##TYPE);	\
	\
	d0 = _mm_mullo_epi16(c1, mcolor);	\
	d1 = _mm_mulhi_epu16(c1, mcolor);	\
	\
	e0 = _mm_unpacklo_epi16(d0, d1);	\
	e1 = _mm_unpackhi_epi16(d0, d1);	\
	\
	g0 = _mm_add_epi32(e0, b0);	\
	g1 = _mm_add_epi32(e1, b1);	\
	\
	f0 = _mm_srli_epi32(g0, 8);	\
	f1 = _mm_srli_epi32(g1, 8);	\
	\
	h = _mm_packs_epi32(f0, f1);


/* *********************************************************** */

#define	BLEND_COLOR_INP_U8_4(spp, number)	\
	\
	P_TYPE s0, a0,	a00, \
	c0, c1, c00, c01, c10, c11,	\
	msrc0, msrc1, msrc2, msrc3,	\
	b0_0, b1_0, b2_0, b3_0,	\
	b0, b1, b2, b3,	\
	t0, t1, t00, t01, t10, t11,	\
	d0, d1, d2, d3,	\
	e0, e1, e2, e3,	\
	g0, g1, g2, g3, f0, f1, f;	\
	\
	s0 = _mm_loadu_si128(spp);	\
	\
	a00 = _mm_slli_epi32(s0, number);	\
	a0 = _mm_srli_epi32(a00, 24);	\
	\
	c0 = _mm_unpacklo_epi32(a0, mzero);	\
	c1 = _mm_unpackhi_epi32(a0, mzero);	\
	\
	c00 = _mm_unpacklo_epi32(c0, c0);	\
	c01 = _mm_unpackhi_epi32(c0, c0);	\
	c10 = _mm_unpacklo_epi32(c1, c1);	\
	c11 = _mm_unpackhi_epi32(c1, c1); 	\
	\
	msrc0 = _mm_unpacklo_epi64(c00, c00);	\
	msrc1 = _mm_unpacklo_epi64(c01, c01);	\
	msrc2 = _mm_unpacklo_epi64(c10, c10);	\
	msrc3 = _mm_unpacklo_epi64(c11, c11);	\
	\
	b0_0 = _mm_sub_epi32(m256, msrc0);	\
	b1_0 = _mm_sub_epi32(m256, msrc1);	\
	b2_0 = _mm_sub_epi32(m256, msrc2);	\
	b3_0 = _mm_sub_epi32(m256, msrc3);	\
	\
	b0 = _mm_mullo_epi16(b0_0, ccolor_0);	\
	b1 = _mm_mullo_epi16(b1_0, ccolor_0); 	\
	b2 = _mm_mullo_epi16(b2_0, ccolor_0);	\
	b3 = _mm_mullo_epi16(b3_0, ccolor_0);	\
	\
	t0 = _mm_unpacklo_epi8(s0, mzero);	\
	t1 = _mm_unpackhi_epi8(s0, mzero);	\
	\
	t00 = _mm_unpacklo_epi16(t0, mzero);	\
	t01 = _mm_unpackhi_epi16(t0, mzero);	\
	t10 = _mm_unpacklo_epi16(t1, mzero);	\
	t11 = _mm_unpackhi_epi16(t1, mzero);	\
	\
	d0 = _mm_mullo_epi16(t00, msrc0);	\
	d1 = _mm_mullo_epi16(t01, msrc1);	\
	d2 = _mm_mullo_epi16(t10, msrc2);	\
	d3 = _mm_mullo_epi16(t11, msrc3);	\
	\
	e0 = _mm_add_epi32(d0, b0);	\
	e1 = _mm_add_epi32(d1, b1);	\
	e2 = _mm_add_epi32(d2, b2);	\
	e3 = _mm_add_epi32(d3, b3);	\
	\
	g0 = _mm_srli_epi32(e0, 8);	\
	g1 = _mm_srli_epi32(e1, 8);	\
	g2 = _mm_srli_epi32(e2, 8);	\
	g3 = _mm_srli_epi32(e3, 8);	\
	\
	f0 = _mm_packs_epi32(g0, g1);	\
	f1 = _mm_packs_epi32(g2, g3);	\
	\
	f = _mm_packus_epi16(f0, f1);



/* *********************************************************** */

void
mlib_s_ImageBlendColor_Inp_U8_2(
    mlib_u8 *sp,
	mlib_u8 *dp,
    mlib_s32 sstride,
    mlib_s32 dstride,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s32 channel,
	mlib_s32 alpha,
	const mlib_s32 *color)
{

	mlib_u8 *sl, *dl;
	mlib_s32 i, j, k, t, q;
	mlib_u8 src0, src1, ssp0, ssp1;

	P_TYPE mzero;
	P_TYPE m256, m255_1, m255_0;
	P_TYPE mcolor_0, mcolor_1;

	mzero = _mm_setzero_si128();

	mcolor_0 = _mm_set1_epi16(color[1]);
	mcolor_1 = _mm_set1_epi16(color[0]);

	m256 = _mm_set1_epi16(256);
	m255_1 = _mm_set_epi8(255, 0, 255, 0,
				255, 0, 255, 0, 255, 0, 255, 0,
				255, 0, 255, 0);

	m255_0 = _mm_set_epi8(0, 255, 0, 255,
			0, 255, 0, 255, 0, 255, 0, 255,
			0, 255, 0, 255);

	sl = sp;
	dl = dp;

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

				src0 = src[0];
				src1 = src[1];

				dst[0] = 255;
				BLEND_COLOR_INP_U8(dst[1], color[1],
						src0, src1);

				src += 2;
				dst += 2;
			}

			for (t = 0; t < n; t++) {

				P_TYPE h1;

				BLEND_COLOR_INP_U8_2(spp, mcolor_0, 0);

				h1 = _mm_slli_epi16(h, 8);
				_mm_storeu_si128(dpp,
					_mm_or_si128(m255_0, h1));

				spp ++;
				dpp ++;

			}

			for (q = 0; q < m / 2; q++) {

				ssp0 = ssp[0];
				ssp1 = ssp[1];

				ddp[0] = 255;
				BLEND_COLOR_INP_U8(ddp[1], color[1],
						ssp0, ssp1);

				ssp += 2;
				ddp += 2;
			}
		} else {

			for (k = 0; k < begin/ 2; k ++) {

				src0 = src[0];
				src1 = src[1];

				dst[1] = 255;
				BLEND_COLOR_INP_U8(dst[0], color[0],
						src1, src0);

				src += 2;
				dst += 2;
			}

			for (t = 0; t < n; t++) {

				BLEND_COLOR_INP_U8_2(spp, mcolor_1, 1);
				_mm_storeu_si128(dpp,
					_mm_or_si128(m255_1, h));

				spp ++;
				dpp ++;

			}

			for (q = 0; q < m / 2; q++) {

				ssp0 = ssp[0];
				ssp1 = ssp[1];

				ddp[1] = 255;
				BLEND_COLOR_INP_U8(ddp[0], color[0],
						ssp1, ssp0);

				ssp += 2;
				ddp += 2;
			}
		}

		sl += sstride;
		dl += dstride;
	}
}

/* *********************************************************** */

void
mlib_s_ImageBlendColor_Inp_U8_4(
    mlib_u8 *sp,
	mlib_u8 *dp,
    mlib_s32 sstride,
	mlib_s32 dstride,
    mlib_s32 width,
    mlib_s32 height,
	mlib_s32 channel,
    mlib_s32 alpha,
	const mlib_s32 *color)
{

	mlib_u8 *sl, *dl;
	mlib_s32 i, j, k, t, q;

	mlib_u8 src0, src1, src2, src3,
	ssp0, ssp1, ssp2, ssp3;

	P_TYPE mzero, m255, m256;
	P_TYPE mcolor0, mcolor1, mcolor2, mcolor3;
	P_TYPE m255_1, m255_2, m255_3;

	P_TYPE mmcolor00, mmcolor0,
	mmcolor11, mmcolor111, mmcolor1,
	mmcolor22, mmcolor222, mmcolor2,
	mmcolor33, mmcolor3,
	color0, color1, ccolor_0;

	mzero = _mm_setzero_si128();
	m255 = _mm_set1_epi32(255);
	m256 = _mm_set1_epi32(256);

	m255_1 = _mm_slli_epi32(m255, 8);
	m255_2 = _mm_slli_epi32(m255, 16);
	m255_3 = _mm_slli_epi32(m255, 24);

	mcolor0 = _mm_set1_epi32(color[0]);
	mcolor1 = _mm_set1_epi32(color[1]);
	mcolor2 = _mm_set1_epi32(color[2]);
	mcolor3 = _mm_set1_epi32(color[3]);

	mmcolor00 = _mm_slli_si128(mcolor0, 12);
	mmcolor0 = _mm_srli_si128(mmcolor00, 12);

	mmcolor11 = _mm_slli_si128(mcolor1, 8);
	mmcolor111 = _mm_srli_si128(mmcolor11, 12);
	mmcolor1 = _mm_slli_si128(mmcolor111, 4);

	mmcolor22 = _mm_slli_si128(mcolor2, 4);
	mmcolor222 = _mm_srli_si128(mmcolor22, 12);
	mmcolor2 = _mm_slli_si128(mmcolor222, 8);

	mmcolor33 = _mm_srli_si128(mcolor3, 12);
	mmcolor3 = _mm_slli_si128(mmcolor33, 12);

	color0 = _mm_or_si128(mmcolor0, mmcolor1);
	color1 = _mm_or_si128(mmcolor2, mmcolor3);

	ccolor_0 = _mm_or_si128(color0, color1);

	sl = sp;
	dl = dp;

	width *= 4;

	for (i = 0; i < height; i ++) {

		mlib_u8 *src, *dst;
		mlib_u8 *ssp, *ddp;

		P_TYPE *spp, *dpp;
		P_TYPE *spp0;

		mlib_s32 offdst_l, offsrc_l;

		mlib_s32 m, n, begin;

		src = sl;
		dst = dl;

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

				src0 = src[0];
				src1 = src[1];
				src2 = src[2];
				src3 = src[3];

				dst[0] = MLIB_U8_MAX;
				BLEND_COLOR_INP_U8(dst[1], color[1],
						src0, src1);
				BLEND_COLOR_INP_U8(dst[2], color[2],
						src0, src2);
				BLEND_COLOR_INP_U8(dst[3], color[3],
						src0, src3);

				src += 4;
				dst += 4;
			}

			for (t = 0; t < n; t ++) {

				P_TYPE h0, h1;

				spp0 = spp;
				BLEND_COLOR_INP_U8_4(spp0, 24);

				h0 = _mm_srli_epi32(f, 8);
				h1 = _mm_slli_epi32(h0, 8);

				_mm_storeu_si128(dpp,
					_mm_or_si128(m255, h1));

				spp ++;
				dpp ++;
			}

			for (q = 0; q < m / 4; q++) {

				ssp0 = ssp[0];
				ssp1 = ssp[1];
				ssp2 = ssp[2];
				ssp3 = ssp[3];

				ddp[0] = MLIB_U8_MAX;
				BLEND_COLOR_INP_U8(ddp[1], color[1],
						ssp0, ssp1);
				BLEND_COLOR_INP_U8(ddp[2], color[2],
						ssp0, ssp2);
				BLEND_COLOR_INP_U8(ddp[3], color[3],
						ssp0, ssp3);

				ssp += 4;
				ddp += 4;
			}

		} else if (alpha == 1) {

			for (k = 0; k < begin / 4; k++) {

				src0 = src[0];
				src1 = src[1];
				src2 = src[2];
				src3 = src[3];

				dst[1] = MLIB_U8_MAX;

				BLEND_COLOR_INP_U8(dst[0], color[0],
						src1, src0);
				BLEND_COLOR_INP_U8(dst[2], color[2],
						src1, src2);
				BLEND_COLOR_INP_U8(dst[3], color[3],
						src1, src3);

				src += 4;
				dst += 4;
			}

			for (t = 0; t < n; t ++) {

				P_TYPE h00, h01, h10, h11, h;

				spp0 = spp;
				BLEND_COLOR_INP_U8_4(spp0, 16);

				h00 = _mm_srli_epi32(f, 16);
				h01 = _mm_slli_epi32(h00, 16);

				h10 = _mm_slli_epi32(f, 24);
				h11 = _mm_srli_epi32(h10, 24);

				h = _mm_or_si128(h01, h11);
				_mm_storeu_si128(dpp,
					_mm_or_si128(m255_1, h));

				spp ++;
				dpp ++;

			}

			for (q = 0; q < m / 4; q++) {

				ssp0 = ssp[0];
				ssp1 = ssp[1];
				ssp2 = ssp[2];
				ssp3 = ssp[3];

				ddp[1] = MLIB_U8_MAX;

				BLEND_COLOR_INP_U8(ddp[0], color[0],
						ssp1, ssp0);
				BLEND_COLOR_INP_U8(ddp[2], color[2],
						ssp1, ssp2);
				BLEND_COLOR_INP_U8(ddp[3], color[3],
						ssp1, ssp3);

				ssp += 4;
				ddp += 4;
			}

		} else if (alpha == 2) {

			for (k = 0; k < begin / 4; k++) {

				src0 = src[0];
				src1 = src[1];
				src2 = src[2];
				src3 = src[3];

				dst[2] = MLIB_U8_MAX;

				BLEND_COLOR_INP_U8(dst[0], color[0],
						src2, src0);
				BLEND_COLOR_INP_U8(dst[1], color[1],
						src2, src1);
				BLEND_COLOR_INP_U8(dst[3], color[3],
						src2, src3);

				src += 4;
				dst += 4;
			}

			for (t = 0; t < n; t ++) {

				P_TYPE h00, h01, h10, h11, h;

				spp0 = spp;

				BLEND_COLOR_INP_U8_4(spp0, 8);

				h00 = _mm_srli_epi32(f, 24);
				h01 = _mm_slli_epi32(h00, 24);

				h10 = _mm_slli_epi32(f, 16);
				h11 = _mm_srli_epi32(h10, 16);

				h = _mm_or_si128(h01, h11);

				_mm_storeu_si128(dpp,
					_mm_or_si128(m255_2, h));

				spp ++;
				dpp ++;

			}

			for (q = 0; q < m / 4; q++) {

				ssp0 = ssp[0];
				ssp1 = ssp[1];
				ssp2 = ssp[2];
				ssp3 = ssp[3];

				ddp[2] = MLIB_U8_MAX;

				BLEND_COLOR_INP_U8(ddp[0], color[0],
						ssp2, ssp0);
				BLEND_COLOR_INP_U8(ddp[1], color[1],
						ssp2, ssp1);
				BLEND_COLOR_INP_U8(ddp[3], color[3],
						ssp2, ssp3);

				ssp += 4;
				ddp += 4;
			}

		} else {


			for (k = 0; k < begin / 4; k++) {

				src0 = src[0];
				src1 = src[1];
				src2 = src[2];
				src3 = src[3];

				dst[3] = MLIB_U8_MAX;

				BLEND_COLOR_INP_U8(dst[0], color[0],
						src3, src0);
				BLEND_COLOR_INP_U8(dst[1], color[1],
						src3, src1);
				BLEND_COLOR_INP_U8(dst[2], color[2],
						src3, src2);

				src += 4;
				dst += 4;
			}


			for (t = 0; t < n; t ++) {

				P_TYPE h0, h;

				spp0 = spp;

				BLEND_COLOR_INP_U8_4(spp0, 0);

				h0 = _mm_slli_epi32(f, 8);
				h = _mm_srli_epi32(h0, 8);

				_mm_storeu_si128(dpp,
					_mm_or_si128(m255_3, h));

				spp ++;
				dpp ++;
			}

			for (q = 0; q < m / 4; q++) {

				ssp0 = ssp[0];
				ssp1 = ssp[1];
				ssp2 = ssp[2];
				ssp3 = ssp[3];

				ddp[3] = MLIB_U8_MAX;

				BLEND_COLOR_INP_U8(ddp[0], color[0],
						ssp3, ssp0);
				BLEND_COLOR_INP_U8(ddp[1], color[1],
						ssp3, ssp1);
				BLEND_COLOR_INP_U8(ddp[2], color[2],
						ssp3, ssp2);

				ssp += 4;
				ddp += 4;
			}
		}

		sl += sstride;
		dl += dstride;
	}
}

/* *********************************************************** */

void
mlib_s_ImageBlendColor_Inp_U8_3(
    mlib_u8 *sp,
	mlib_u8 *dp,
    mlib_s32 sstride,
	mlib_s32 dstride,
    mlib_s32 width,
    mlib_s32 height,
	mlib_s32 channel,
    mlib_s32 alpha,
	const mlib_s32 *color)
{

	mlib_s32 i, j;
	mlib_u8 *sl, *dl;
	mlib_u8 sl1, sl2, sl0;

	if (alpha == 0) {

		for (i = 0; i < height; i ++) {

			sl = sp;
			dl = dp;

			for (j = 0; j < width; j++) {
				sl0 = sl[0];
				sl1 = sl[1];
				sl2 = sl[2];

				dl[0] = MLIB_U8_MAX;
				BLEND_COLOR_INP_U8(dl[1], color[1],
						sl0, sl1);
				BLEND_COLOR_INP_U8(dl[2], color[2],
						sl0, sl2);

				sl += 3;
				dl += 3;
			}

			sp += sstride;
			dp += dstride;
		}
	} else if (alpha == 1) {

		for (i = 0; i < height; i++) {
			sl = sp;
			dl = dp;

			for (j = 0; j < width; j ++) {


				sl0 = sl[0];
				sl1 = sl[1];
				sl2 = sl[2];

				dl[1] = MLIB_U8_MAX;
				BLEND_COLOR_INP_U8(dl[0], color[0],
						sl1, sl0);
				BLEND_COLOR_INP_U8(dl[2], color[2],
						sl1, sl2);

				sl += 3;
				dl += 3;
			}

			sp += sstride;
			dp += dstride;
		}

	} else {

		for (i = 0; i < height; i++) {

			sl = sp;
			dl = dp;

			for (j = 0; j < width; j++) {

				sl0 = sl[0];
				sl1 = sl[1];
				sl2 = sl[2];

				dl[2] = MLIB_U8_MAX;

				BLEND_COLOR_INP_U8(dl[0], color[0],
						sl2, sl0);
				BLEND_COLOR_INP_U8(dl[1], color[1],
						sl2, sl1);


				sl += 3;
				dl += 3;
			}

			sp += sstride;
			dp += dstride;

		}
	}
}
