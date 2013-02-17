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

#pragma ident	"@(#)mlib_s_ImageBlendColor_Inp_S16.c	9.5	07/11/05 SMI"

/*
 *  Internal function for mlib_ImageBlendColor_Inp on S16 data type.
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

#define	BLEND_COLOR_INP_S16(dst, color, src0, src1)	\
	dst = (color << 15)	\
			+ (src0 & 0x7fff) * (src1 - color) >> 15;

/* *********************************************************** */

#define	BLEND_COLOR_INP_S16_2(spp, dpp, m, n)	\
	P_TYPE s0, a0,	\
	b, c, c0, c1, d0, d1, d, e0, e1, f0, f1,	\
	src0, src1, src1_0, src1_1,	src0_0, src0_1,\
	d_0, d_1, g0, g1, g2;	\
\
	mlib_s32* pb_64 = (mlib_s32 *)(&b);	\
	mlib_s32* pc_64 = (mlib_s32 *)(&c);\
\
	mlib_s64 *pstore64_1 = (mlib_s64 *)(&d_0);\
	mlib_s64 *pstore64_2 = pstore64_1 + 1;\
	mlib_s64 *pstore64_3 = (mlib_s64 *)(&d_1);\
	mlib_s64 *pstore64_4 = pstore64_3 + 1;\
\
\
	s0 = _mm_loadu_si128(spp);\
	a0 = _mm_slli_epi32(s0, 16);	\
	src0 = _mm_srli_epi32(a0, 16);\
	src1 = _mm_srli_epi32(s0, 16);\
\
	src##n##_0 = _mm_slli_epi32(src##n, 16);	\
	src##n##_1 = _mm_srai_epi32(src##n##_0, 16);\
\
	b = _mm_and_si128(src##m, m32767);\
\
	c = _mm_sub_epi32(src##n##_1, mcolor_##m);\
\
	*pstore64_1 = (*pb_64) * (*pc_64);\
	pb_64 ++;\
	pc_64 ++;\
\
\
	*pstore64_2 = (*pb_64) * (*pc_64);\
	pb_64 ++;\
	pc_64 ++;\
\
\
	*pstore64_3 = (*pb_64) * (*pc_64);\
	pb_64 ++;\
	pc_64 ++;\
\
\
	*pstore64_4 = (*pb_64) * (*pc_64);	\
	pb_64 ++;\
	pc_64 ++;\
\
\
	e0 = _mm_add_epi64(mcolor_shift15_##m, d_0);\
	e1 = _mm_add_epi64(mcolor_shift15_##m, d_1);\
\
	f0 = _mm_srai_epi32(e0, 15);\
	f1 = _mm_srai_epi32(e1, 15);\
\
	g0 = _mm_packs_epi32(f0, f1);

/* *********************************************************** */

#define	BLEND_COLOR_INP_S16_4(spp, dpp, m, n)	\
	P_TYPE s0, a0, src0, b, src_0, src_1,	\
	c0_0, c0_1, c1_0, c1_1, d0, d1,	\
	e0, e1, e2, e3,	\
	f0, f1, f2, f3,	\
	g0, g1, g2, g3,	\
	h0, h1, h2, h3,	\
	dd0, dd1, dd, data;	\
\
	mlib_s32 *pd0_64 = (mlib_s32 *)(&d0);	\
	mlib_s32 *pd1_64 = (mlib_s32 *)(&d1);	\
	mlib_s32 *pb_64_1 = (mlib_s32 *)(&b);	\
	mlib_s32 *pb_64_2 = (mlib_s32 *)(&b) + 2;	\
\
	mlib_s64 *pstore0_0 = (mlib_s64 *)(&e0);	\
	mlib_s64 *pstore0_1 = pstore0_0 + 1;	\
	mlib_s64 *pstore0_2 = (mlib_s64 *)(&e1);	\
	mlib_s64 *pstore0_3 = pstore0_2 + 1;	\
\
	mlib_s64 *pstore1_0 = (mlib_s64 *)(&e2);	\
	mlib_s64 *pstore1_1 = pstore1_0 + 1;	\
	mlib_s64 *pstore1_2 = (mlib_s64 *)(&e3);	\
	mlib_s64 *pstore1_3 = pstore1_2 + 1;	\
\
	s0 = _mm_loadu_si128(spp);	\
\
	a0 = _mm_slli_epi64(s0, m);	\
	src0 = _mm_srli_epi64(a0, 48);	\
\
	b = _mm_and_si128(src0,  m32767_in_0);	\
\
	c0_0 = _mm_unpacklo_epi16(s0, mzero);	\
	c0_1 = _mm_unpackhi_epi16(s0, mzero);	\
\
	c1_0 = _mm_slli_epi32(c0_0, 16);	\
	c1_1 = _mm_slli_epi32(c0_1, 16);	\
\
	src_0 = _mm_srai_epi32(c1_0, 16);	\
	src_1 = _mm_srai_epi32(c1_1, 16);	\
\
	d0 = _mm_sub_epi32(src_0, mcolor);	\
	d1 = _mm_sub_epi32(src_1, mcolor);	\
\
	*pstore0_0 = (*pd0_64) * (*pb_64_1);	\
	*pstore1_0 = (*pd1_64) * (*pb_64_2);	\
	pd0_64 ++;	\
	pd1_64 ++;	\
\
	*pstore0_1 = (*pd0_64) * (*pb_64_1);	\
	*pstore1_1 = (*pd1_64) * (*pb_64_2);	\
	pd0_64 ++;	\
	pd1_64 ++;	\
\
	*pstore0_2 = (*pd0_64) * (*pb_64_1);	\
	*pstore1_2 = (*pd1_64) * (*pb_64_2);	\
	pd0_64 ++;	\
	pd1_64 ++;	\
\
	*pstore0_3 = (*pd0_64) * (*pb_64_1);	\
	*pstore1_3 = (*pd1_64) * (*pb_64_2);	\
\
	f0 = _mm_add_epi64(mcolor_shift15_0, e0);	\
	f1 = _mm_add_epi64(mcolor_shift15_1, e1);	\
	f2 = _mm_add_epi64(mcolor_shift15_0, e2);	\
	f3 = _mm_add_epi64(mcolor_shift15_1, e3);	\
\
	g0 = _mm_srai_epi32(f0, 15);	\
	g1 = _mm_srai_epi32(f1, 15);	\
	g2 = _mm_srai_epi32(f2, 15);	\
	g3 = _mm_srai_epi32(f3, 15);	\
\
	h0 = _mm_packs_epi32(g0, g1);	\
	h1 = _mm_packs_epi32(g2, g3);	\
\
	h2 = _mm_slli_epi32(h0, 16);	\
\
	dd0 = _mm_srai_epi32(h2, 16);	\
\
	h3 = _mm_slli_epi32(h1, 16);	\
\
	dd1 = _mm_srai_epi32(h3, 16);	\
	dd = _mm_packs_epi32(dd0, dd1);	\
	data = _mm_and_si128(sign_##n, dd);	\
\
	_mm_storeu_si128(dpp,				\
		_mm_or_si128(m32767_in_##n, data));	\
\
	spp ++;	\
	dpp ++;

/* *********************************************************** */

#if 0
/* the SSE2 code doesn't seem to be as fast as the pure C code */

void
mlib_s_ImageBlendColor_Inp_S16_2(
    mlib_s16 *sp,
    mlib_s16 *dp,
    mlib_s32 sstride,
    mlib_s32 dstride,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s32 channel,
    mlib_s32 alpha,
    const mlib_s32 *color)
{
	mlib_s16 *sl, *dl;
	mlib_s16 src0, src1, ssp0, ssp1;


	mlib_s32 i, j, k, t, q;

	P_TYPE mzero, m32767;
	P_TYPE mcolor_0, mcolor_64, mcolor_shift15_0;
	P_TYPE mcolor_1, mcolor_64_1, mcolor_shift15_1;
	P_TYPE m32767_0, m32767_1;

	mcolor_1 = _mm_set1_epi32(color[0]);
	mcolor_64_1 = _mm_srli_epi64(mcolor_1, 32);
	mcolor_shift15_1 = _mm_slli_epi64(mcolor_64_1, 15);

	mcolor_0 = _mm_set1_epi32(color[1]);
	mcolor_64 = _mm_srli_epi64(mcolor_0, 32);
	mcolor_shift15_0 = _mm_slli_epi64(mcolor_64, 15);

	mzero = _mm_setzero_si128();

	m32767 = _mm_set1_epi32(32767);
	m32767_0 = m32767;
	m32767_1 = _mm_slli_epi32(m32767_0, 16);

	sl = sp;
	dl = dp;

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

				src0 = src[0];
				src1 = src[1];

				dst[0] = MLIB_S16_MAX;
				BLEND_COLOR_INP_S16(dst[1],
						color[1], src0, src1);

				src += 2;
				dst += 2;
			}


			for (t = 0; t < n; t++) {

				BLEND_COLOR_INP_S16_2(spp, dpp, 0, 1);

				g1 = _mm_slli_epi32(g0, 16);

				_mm_storeu_si128(dpp,
					_mm_or_si128(g1, m32767_0));

				spp ++;
				dpp ++;

			}

			for (q = 0; q < m / 2; q++) {

				ssp0 = ssp[0];
				ssp1 = ssp[1];

				ddp[0] = MLIB_S16_MAX;
				BLEND_COLOR_INP_S16(ddp[1],
						color[1], ssp0, ssp1);

				ssp += 2;
				ddp += 2;
			}
		} else {

			for (k = 0; k < begin / 2; k ++) {

				src0 = src[0];
				src1 = src[1];

				dst[1] = MLIB_S16_MAX;
				BLEND_COLOR_INP_S16(dst[0],
						color[0], src1, src0);

				src += 2;
				dst += 2;
			}


			for (t = 0; t < n; t++) {

				BLEND_COLOR_INP_S16_2(spp, dpp, 1, 0);

				g1 = _mm_slli_epi32(g0, 16);
				g2 = _mm_srli_epi32(g1, 16);

				_mm_storeu_si128(dpp,
					_mm_or_si128(g2, m32767_1));

				spp ++;
				dpp ++;

			}

			for (q = 0; q < m / 2; q++) {

				ssp0 = ssp[0];
				ssp1 = ssp[1];

				ddp[1] = MLIB_S16_MAX;
				BLEND_COLOR_INP_S16(ddp[0],
						color[0], ssp1, ssp0);

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
mlib_s_ImageBlendColor_Inp_S16_4(
    mlib_s16 *sp,
    mlib_s16 *dp,
    mlib_s32 sstride,
    mlib_s32 dstride,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s32 channel,
    mlib_s32 alpha,
    const mlib_s32 *color)
{

	mlib_s16 *sl, *dl;
	mlib_s16 src0, src1, src2, src3,
	ssp0, ssp1, ssp2, ssp3;

	mlib_s32 i, j, k, t, q;

	P_TYPE mzero, mcolor, mcolor_a, mcolor_b;
	P_TYPE mcolor_shift15_0, mcolor_shift15_1;
	P_TYPE m32767_in_0, m32767_in_1, m32767_in_2, m32767_in_3;
	P_TYPE sign_0, sign_1, sign_2, sign_3;

	mzero = _mm_setzero_si128();
	mcolor = _mm_set_epi32(color[3], color[2],
			color[1], color[0]);


	mcolor_a = _mm_set_epi32(0, color[1], 0, color[0]);
	mcolor_b = _mm_set_epi32(0, color[3], 0, color[2]);

	mcolor_shift15_0 = _mm_slli_epi32(mcolor_a, 15);
	mcolor_shift15_1 = _mm_slli_epi32(mcolor_b, 15);

	m32767_in_0 = _mm_set_epi16(0, 0, 0, 32767,
			0, 0, 0, 32767);
	m32767_in_1 = _mm_set_epi16(0, 0, 32767, 0,
			0, 0, 32767, 0);

	m32767_in_2 = _mm_set_epi16(0, 32767, 0, 0,
			0, 32767, 0, 0);

	m32767_in_3 = _mm_set_epi16(32767, 0, 0, 0,
			32767, 0, 0, 0);

	sign_0 = _mm_set_epi16(0xffff, 0xffff,
			0xffff, 0, 0xffff, 0xffff, 0xffff, 0);


	sign_1 = _mm_set_epi16(0xffff, 0xffff, 0, 0xffff,
			0xffff, 0xffff, 0, 0xffff);
	sign_2 = _mm_set_epi16(0xffff, 0, 0xffff, 0xffff,
			0xffff, 0, 0xffff, 0xffff);
	sign_3 = _mm_set_epi16(0, 0xffff, 0xffff, 0xffff,
			0, 0xffff, 0xffff, 0xffff);


	sl = sp;
	dl = dp;

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

				src0 = src[0];
				src1 = src[1];
				src2 = src[2];
				src3 = src[3];

				dst[0] = MLIB_S16_MAX;

				BLEND_COLOR_INP_S16(dst[1],
						color[1], src0, src1);
				BLEND_COLOR_INP_S16(dst[2],
						color[2], src0, src2);
				BLEND_COLOR_INP_S16(dst[3],
						color[3], src0, src3);

				src += 4;
				dst += 4;
			}

			for (t = 0; t < n; t++) {

				BLEND_COLOR_INP_S16_4(spp, dpp, 48, 0);
			}

			for (q = 0; q < m / 4; q++) {

				ssp0 = ssp[0];
				ssp1 = ssp[1];
				ssp2 = ssp[2];
				ssp3 = ssp[3];

				ddp[0] = MLIB_S16_MAX;

				BLEND_COLOR_INP_S16(ddp[1],
						color[1], ssp0, ssp1);
				BLEND_COLOR_INP_S16(ddp[2],
						color[2], ssp0, ssp2);
				BLEND_COLOR_INP_S16(ddp[3],
						color[3], ssp0, ssp3);

				ssp += 4;
				ddp += 4;
			}

		} else if (alpha == 1) {

			for (k = 0; k < begin / 4; k ++) {

				src0 = src[0];
				src1 = src[1];
				src2 = src[2];
				src3 = src[3];

				dst[1] = MLIB_S16_MAX;

				BLEND_COLOR_INP_S16(dst[2],
						color[2], src1, src2);
				BLEND_COLOR_INP_S16(dst[3],
						color[3], src1, src3);
				BLEND_COLOR_INP_S16(dst[0],
						color[0], src1, src0);

				src += 4;
				dst += 4;
			}


			for (t = 0; t < n; t++) {

				BLEND_COLOR_INP_S16_4(spp, dpp, 32, 1);
			}

			for (q = 0; q < m / 4; q++) {

				ssp0 = ssp[0];
				ssp1 = ssp[1];
				ssp2 = ssp[2];
				ssp3 = ssp[3];

				ddp[1] = MLIB_S16_MAX;

				BLEND_COLOR_INP_S16(ddp[2],
						color[2], ssp1, ssp2);
				BLEND_COLOR_INP_S16(ddp[3],
						color[3], ssp1, ssp3);
				BLEND_COLOR_INP_S16(ddp[0],
						color[0], ssp1, ssp0);

				ssp += 4;
				ddp += 4;
			}

		} else if (alpha == 2) {

			for (k = 0; k < begin / 4; k ++) {

				src0 = src[0];
				src1 = src[1];
				src2 = src[2];
				src3 = src[3];

				dst[2] = MLIB_S16_MAX;

				BLEND_COLOR_INP_S16(dst[3],
						color[3], src2, src3);
				BLEND_COLOR_INP_S16(dst[0],
						color[0], src2, src0);
				BLEND_COLOR_INP_S16(dst[1],
						color[1], src2, src1);

				src += 4;
				dst += 4;

			}


			for (t = 0; t < n; t++) {

				BLEND_COLOR_INP_S16_4(spp, dpp, 16, 2);
			}

			for (q = 0; q < m / 4; q++) {

				ssp0 = ssp[0];
				ssp1 = ssp[1];
				ssp2 = ssp[2];
				ssp3 = ssp[3];

				ddp[2] = MLIB_S16_MAX;

				BLEND_COLOR_INP_S16(ddp[3],
						color[3], ssp2, ssp3);
				BLEND_COLOR_INP_S16(ddp[0],
						color[0], ssp2, ssp0);
				BLEND_COLOR_INP_S16(ddp[1],
						color[1], ssp2, ssp1);


				src += 4;
				dst += 4;

			}

		} else {

			for (k = 0; k < begin / 4; k ++) {

				src0 = src[0];
				src1 = src[1];
				src2 = src[2];
				src3 = src[3];

				dst[3] = MLIB_S16_MAX;

				BLEND_COLOR_INP_S16(dst[0],
						color[0], src3, src0);
				BLEND_COLOR_INP_S16(dst[1],
						color[1], src3, src1);
				BLEND_COLOR_INP_S16(dst[2],
						color[2], src3, src2);

				src += 4;
				dst += 4;

			}


			for (t = 0; t < n; t++) {

				BLEND_COLOR_INP_S16_4(spp, dpp, 0, 3);
			}

			for (q = 0; q < m / 4; q++) {

				ssp0 = ssp[0];
				ssp1 = ssp[1];
				ssp2 = ssp[2];
				ssp3 = ssp[3];

				ddp[3] = MLIB_S16_MAX;

				BLEND_COLOR_INP_S16(ddp[0],
						color[0], ssp3, ssp0);
				BLEND_COLOR_INP_S16(ddp[1],
						color[1], ssp3, ssp1);
				BLEND_COLOR_INP_S16(ddp[2],
						color[2], ssp3, ssp2);

				ssp += 4;
				ddp += 4;

			}
		}

		sl += sstride;
		dl += dstride;
	}
}

#else

/* *********************************************************** */

void
mlib_s_ImageBlendColor_Inp_S16_2(
    mlib_s16 *sp,
    mlib_s16 *dp,
    mlib_s32 sstride,
    mlib_s32 dstride,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s32 channel,
    mlib_s32 alpha,
    const mlib_s32 *color)
{
	mlib_s16 *sl, *dl;
	mlib_s32 i, j;

	mlib_s16 sl0, sl1, sl2;

	if (alpha == 0) {

		for (i = 0; i < height; i++) {

			sl = sp;
			dl = dp;

			for (j = 0; j < width; j++) {

				sl0 = sl[0];
				sl1 = sl[1];

				dl[0] = MLIB_S16_MAX;

				BLEND_COLOR_INP_S16(dl[1], color[1], sl0, sl1);

				sl += 2;
				dl += 2;
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

				dl[1] = MLIB_S16_MAX;

				BLEND_COLOR_INP_S16(dl[0], color[0], sl1, sl0);

				sl += 2;
				dl += 2;
			}

			sp += sstride;
			dp += dstride;
		}
	}
}

/* *********************************************************** */

void
mlib_s_ImageBlendColor_Inp_S16_4(
    mlib_s16 *sp,
    mlib_s16 *dp,
    mlib_s32 sstride,
    mlib_s32 dstride,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s32 channel,
    mlib_s32 alpha,
    const mlib_s32 *color)
{
	mlib_s16 *sl, *dl;
	mlib_s32 i, j;

	mlib_s16 sl0, sl1, sl2, sl3;

	if (alpha == 0) {

		for (i = 0; i < height; i++) {

			sl = sp;
			dl = dp;

			for (j = 0; j < width; j++) {

				sl0 = sl[0];
				sl1 = sl[1];
				sl2 = sl[2];
				sl3 = sl[3];

				dl[0] = MLIB_S16_MAX;

				BLEND_COLOR_INP_S16(dl[1], color[1], sl0, sl1);
				BLEND_COLOR_INP_S16(dl[2], color[2], sl0, sl2);
				BLEND_COLOR_INP_S16(dl[3], color[3], sl0, sl3);

				sl += 4;
				dl += 4;
			}

			sp += sstride;
			dp += dstride;
		}

	} else if (alpha == 1) {

		for (i = 0; i < height; i++) {

			sl = sp;
			dl = dp;

			for (j = 0; j < width; j++) {

				sl0 = sl[0];
				sl1 = sl[1];
				sl2 = sl[2];
				sl3 = sl[3];

				dl[1] = MLIB_S16_MAX;

				BLEND_COLOR_INP_S16(dl[0], color[0], sl1, sl0);
				BLEND_COLOR_INP_S16(dl[2], color[2], sl1, sl2);
				BLEND_COLOR_INP_S16(dl[3], color[3], sl1, sl3);

				sl += 4;
				dl += 4;
			}

			sp += sstride;
			dp += dstride;
		}

	} else if (alpha == 2) {

		for (i = 0; i < height; i++) {

			sl = sp;
			dl = dp;

			for (j = 0; j < width; j++) {

				sl0 = sl[0];
				sl1 = sl[1];
				sl2 = sl[2];
				sl3 = sl[3];

				dl[2] = MLIB_S16_MAX;

				BLEND_COLOR_INP_S16(dl[0], color[0], sl2, sl0);
				BLEND_COLOR_INP_S16(dl[1], color[1], sl2, sl1);
				BLEND_COLOR_INP_S16(dl[3], color[3], sl2, sl3);

				sl += 4;
				dl += 4;
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
				sl3 = sl[3];

				dl[3] = MLIB_S16_MAX;

				BLEND_COLOR_INP_S16(dl[0], color[0], sl3, sl0);
				BLEND_COLOR_INP_S16(dl[1], color[1], sl3, sl1);
				BLEND_COLOR_INP_S16(dl[2], color[2], sl3, sl2);

				sl += 4;
				dl += 4;
			}

			sp += sstride;
			dp += dstride;
		}
	}
}

#endif

/* *********************************************************** */

void
mlib_s_ImageBlendColor_Inp_S16_3(
    mlib_s16 *sp,
    mlib_s16 *dp,
    mlib_s32 sstride,
    mlib_s32 dstride,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s32 channel,
    mlib_s32 alpha,
    const mlib_s32 *color)
{
	mlib_s16 *sl, *dl;
	mlib_s32 i, j;

	mlib_s16 sl0, sl1, sl2;

	if (alpha == 0) {

		for (i = 0; i < height; i++) {

			sl = sp;
			dl = dp;

			for (j = 0; j < width; j++) {

				sl0 = sl[0];
				sl1 = sl[1];
				sl2 = sl[2];

				dl[0] = MLIB_S16_MAX;

				BLEND_COLOR_INP_S16(dl[1], color[1], sl0, sl1);
				BLEND_COLOR_INP_S16(dl[2], color[2], sl0, sl2);

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

			for (j = 0; j < width; j++) {

				sl0 = sl[0];
				sl1 = sl[1];
				sl2 = sl[2];

				dl[1] = MLIB_S16_MAX;

				BLEND_COLOR_INP_S16(dl[0], color[0], sl1, sl0);
				BLEND_COLOR_INP_S16(dl[2], color[2], sl1, sl2);

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

				dl[2] = MLIB_S16_MAX;

				BLEND_COLOR_INP_S16(dl[0], color[0], sl2, sl0);
				BLEND_COLOR_INP_S16(dl[1], color[1], sl2, sl1);

				sl += 3;
				dl += 3;
			}

			sp += sstride;
			dp += dstride;
		}
	}
}

/* *********************************************************** */
