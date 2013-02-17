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

#pragma ident   "@(#)mlib_s_ImageBlendColor_U16.c	9.5	07/11/05 SMI"

/*
 *  Internal function for mlib_ImageBlendColor on U16 data type.
 */

#include <mlib_image.h>
#include <mlib_s_ImageBlendColor.h>

#ifdef __SUNPRO_C
#include <sunmedia_intrin.h>
#else
#include <emmintrin.h>
#endif

/* *********************************************************** */

#define	BLEND_COLOR_U16(dst, color, src0, src1)	\
	dst = (color << 16)	\
		+ src0 * (src1 - color) >> 16;

/* *********************************************************** */

#define	BLEND_COLOR_U16_2(spp, m, n)	\
	P_TYPE s0, src0, src1, src1_0, src1_1,	\
	c, d_0, d_1, e0, e1, f0, f1,	\
	g0, g1, g;	\
\
	mlib_s32* pb_64 = (mlib_s32 *)(&src##n);	\
	mlib_s32* pc_64 = (mlib_s32 *)(&c);	\
\
	mlib_s64 *pstore64_0 = (mlib_s64 *)(&d_0);	\
	mlib_s64 *pstore64_1 = pstore64_0 + 1;	\
	mlib_s64 *pstore64_2 = (mlib_s64 *)(&d_1);	\
	mlib_s64 *pstore64_3 = pstore64_2 + 1;	\
\
	s0 = _mm_loadu_si128(spp);	\
\
	src0 = _mm_and_si128(s0, m255_and);	\
	src1 = _mm_srli_epi32(s0, 16);	\
\
	c = _mm_sub_epi32(src##m, mcolor_##n);	\
\
	*pstore64_0 = (*pb_64) * (*pc_64);	\
	pb_64 ++;	\
	pc_64 ++;	\
\
	*pstore64_1 = (*pb_64) * (*pc_64);	\
	pb_64 ++;	\
	pc_64 ++;	\
\
	*pstore64_2 = (*pb_64) * (*pc_64);	\
	pb_64 ++;	\
	pc_64 ++;	\
\
	*pstore64_3 = (*pb_64) * (*pc_64);	\
	pb_64 ++;	\
	pc_64 ++;	\
\
	e0 = _mm_add_epi64(mcolor_shift16_##n, d_0);	\
	e1 = _mm_add_epi64(mcolor_shift16_##n, d_1);	\
\
	f0 = _mm_srli_epi32(e0, 16);	\
	f1 = _mm_srli_epi32(e1, 16);	\
\
	g0 = _mm_unpacklo_epi16(f0, f1);	\
	g1 = _mm_unpackhi_epi16(f0, f1);	\
	g = _mm_unpacklo_epi16(g0, g1);

/* *********************************************************** */

#define	BLEND_COLOR_U16_4(spp, dpp, m, n)	\
	P_TYPE s0, a0, src0,	\
	src_0, src_1,	\
	d0, d1, e0, e1, e2, e3,	\
	f0, f1, f2, f3,	\
	g0, g1, g2, g3,	\
	h0, h1, h2, h3,	\
	dd0, dd1, dd2, dd;	\
\
	mlib_s32 *pd0_64 = (mlib_s32 *)(&d0);	\
	mlib_s32 *pd1_64 = (mlib_s32 *)(&d1);	\
	mlib_s32 *pb_64_1 = (mlib_s32 *)(&src0);	\
	mlib_s32 *pb_64_2 = (mlib_s32 *)(&src0) + 2;	\
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
	a0 = _mm_slli_epi64(s0, m);	\
	src0 = _mm_srli_epi64(a0, 48);	\
\
	src_0 = _mm_unpacklo_epi16(s0, mzero);	\
	src_1 = _mm_unpackhi_epi16(s0, mzero);	\
\
	d0 = _mm_sub_epi32(src_0, mcolor);	\
	d1 = _mm_sub_epi32(src_1, mcolor);	\
\
	*pstore0_0 = (*pd0_64) * (*pb_64_1);	\
	*pstore1_0 = (*pd1_64) * (*pb_64_2);	\
\
	pd0_64 ++;	\
	pd1_64 ++;	\
\
	*pstore0_1 = (*pd0_64) * (*pb_64_1);	\
	*pstore1_1 = (*pd1_64) * (*pb_64_2);	\
\
	pd0_64 ++;	\
	pd1_64 ++;	\
\
	*pstore0_2 = (*pd0_64) * (*pb_64_1);	\
	*pstore1_2 = (*pd1_64) * (*pb_64_2);	\
\
	pd0_64 ++;	\
	pd1_64 ++;	\
\
	*pstore0_3 = (*pd0_64) * (*pb_64_1);	\
	*pstore1_3 = (*pd1_64) * (*pb_64_2);	\
\
	f0 = _mm_add_epi64(mcolor_shift16_0, e0);	\
	f1 = _mm_add_epi64(mcolor_shift16_1, e1);	\
	f2 = _mm_add_epi64(mcolor_shift16_0, e2);	\
	f3 = _mm_add_epi64(mcolor_shift16_1, e3);	\
\
	g0 = _mm_srli_epi32(f0, 16);	\
	g1 = _mm_srli_epi32(f1, 16);	\
	g2 = _mm_srli_epi32(f2, 16);	\
	g3 = _mm_srli_epi32(f3, 16);	\
\
	h0 = _mm_unpacklo_epi16(g0, g1);	\
	h1 = _mm_unpackhi_epi16(g0, g1);	\
	h2 = _mm_unpacklo_epi16(g2, g3);	\
	h3 = _mm_unpackhi_epi16(g2, g3);	\
\
	dd0 = _mm_unpacklo_epi16(h0, h1);	\
	dd1 = _mm_unpacklo_epi16(h2, h3);	\
	dd2 = _mm_slli_si128(dd1, 8);	\
	dd = _mm_or_si128(dd0, dd2);	\
\
	_mm_storeu_si128(dpp,			\
		_mm_or_si128(mcolor_##n, dd));	\
	spp ++;	\
	dpp ++;

/* *********************************************************** */

#define	P_TYPE __m128i

/* *********************************************************** */

#if 0
/* the SSE2 code doesn't seem to be as fast as the pure C code */

void
mlib_s_ImageBlendColor_U16_2(
    mlib_u16 *sp,
    mlib_u16 *dp,
    mlib_s32 sstride,
    mlib_s32 dstride,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s32 channel,
    mlib_s32 alpha,
    const mlib_s32 *color)
{
	mlib_u16 *sl, *dl;

	mlib_s32 i, j, k, t, q;
	mlib_s32 m, n;

	P_TYPE mzero, mcolor_0, mcolor_1;
	P_TYPE mcolor_shift16_0, mcolor_shift16_1,
	mcolor_shift16_0_0, mcolor_shift16_1_0;
	P_TYPE m255_and, m65535;

	mzero = _mm_setzero_si128();
	mcolor_0 = _mm_set1_epi32(color[1]);
	mcolor_1 = _mm_set1_epi32(color[0]);
	mcolor_shift16_0_0 = _mm_set_epi32(0, color[1], 0, color[1]);
	mcolor_shift16_1_0 = _mm_set_epi32(0, color[0], 0, color[0]);
	mcolor_shift16_0 = _mm_slli_epi64(mcolor_shift16_0_0, 16);
	mcolor_shift16_1 = _mm_slli_epi64(mcolor_shift16_1_0, 16);

	m65535 = _mm_set1_epi16(65535);

	m255_and = _mm_set1_epi32(0xffff);

	sl = sp;
	dl = dp;

	width *= 2;

	for (i = 0; i < height; i++) {

		mlib_u16 *src, *dst;
		mlib_u16 *ssp, *ddp;
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

				ssp = (mlib_u16 *)(sl + n * 8);
				ddp = (mlib_u16 *)(dl + n * 8);
			} else {

				n = (width - (8 - offsrc_l)) / 8;
				m = width - (8 - offsrc_l) - n * 8;
				begin = 8 - offsrc_l;

				spp = (P_TYPE *)(sl + 8 - offsrc_l);
				dpp = (P_TYPE *)(dl + 8 - offsrc_l);

				ssp = (mlib_u16 *)(sl + 8 - offsrc_l + n * 8);
				ddp = (mlib_u16 *)(dl + 8 - offsrc_l + n * 8);
			}
		} else {
			n = 0;
			m = 0;
			begin = width;
		}


		if (alpha == 0) {


			for (k = 0; k < begin / 2; k ++) {

				dst[0] = MLIB_U16_MAX;
				BLEND_COLOR_U16(dst[1],
						color[1], src[0], src[1]);

				src += 2;
				dst += 2;
			}


			for (t = 0; t < n; t++) {

				BLEND_COLOR_U16_2(spp, 1, 0);
				_mm_storeu_si128(dpp,
					_mm_unpacklo_epi16(m65535, g));

				spp ++;
				dpp ++;

			}

			for (q = 0; q < m / 2; q++) {

				ddp[0] = MLIB_U16_MAX;
				BLEND_COLOR_U16(ddp[1],
						color[1], ssp[0], ssp[1]);

				ssp += 2;
				ddp += 2;
			}
		} else {


			for (k = 0; k < begin / 2; k ++) {

				dst[1] = MLIB_U16_MAX;
				BLEND_COLOR_U16(dst[0],
						color[0], src[1], src[0]);

				src += 2;
				dst += 2;
			}

			for (t = 0; t < n; t++) {

				BLEND_COLOR_U16_2(spp, 0, 1);
				_mm_storeu_si128(dpp,
					_mm_unpacklo_epi16(g, m65535));

				spp ++;
				dpp ++;
			}

			for (q = 0; q < m / 2; q++) {

				ddp[1] = MLIB_U16_MAX;
				BLEND_COLOR_U16(ddp[0],
						color[0], ssp[1], ssp[0]);

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
mlib_s_ImageBlendColor_U16_4(
    mlib_u16 *sp,
    mlib_u16 *dp,
    mlib_s32 sstride,
    mlib_s32 dstride,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s32 channel,
    mlib_s32 alpha,
    const mlib_s32 *color)
{

	mlib_u16 *sl, *dl;

	mlib_s32 i, j, k, t, q;
	mlib_s32 m, n;

	P_TYPE mzero, mcolor;
	P_TYPE mcolor_a, mcolor_b, mcolor_shift16_0, mcolor_shift16_1;
	P_TYPE mcolor_0, mcolor_1, mcolor_2, mcolor_3;

	mzero = _mm_setzero_si128();
	mcolor = _mm_set_epi32(color[3], color[2],
			color[1], color[0]);

	mcolor_a = _mm_set_epi32(0, color[1], 0, color[0]);
	mcolor_b = _mm_set_epi32(0, color[3], 0, color[2]);

	mcolor_shift16_0 = _mm_slli_epi32(mcolor_a, 16);
	mcolor_shift16_1 = _mm_slli_epi32(mcolor_b, 16);

	mcolor_0 = _mm_set_epi32(0, 65535, 0, 65535);
	mcolor_1 = _mm_slli_epi32(mcolor_0, 16);
	mcolor_2 = _mm_set_epi32(65535, 0, 65535, 0);
	mcolor_3 = _mm_slli_epi32(mcolor_2, 16);

	sl = sp;
	dl = dp;


	width *= 4;

	for (i = 0; i < height; i++) {

		mlib_u16 *src, *dst;
		mlib_u16 *ssp, *ddp;
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

				ssp = (mlib_u16 *)(sl + n * 8);
				ddp = (mlib_u16 *)(dl + n * 8);
			} else {

				n = (width - (8 - offsrc_l)) / 8;
				m = width - (8 - offsrc_l) - n * 8;
				begin = 8 - offsrc_l;

				spp = (__m128i *)(sl + 8 - offsrc_l);
				dpp = (__m128i *)(dl + 8 - offsrc_l);

				ssp = (mlib_u16 *)(sl + 8 - offsrc_l + n * 8);
				ddp = (mlib_u16 *)(dl + 8 - offsrc_l + n * 8);
			}
		} else {
			n = 0;
			m = 0;
			begin = width;
		}

		if (alpha == 0) {

			for (k = 0; k < begin / 4; k ++) {

				dst[0] = MLIB_U16_MAX;

				BLEND_COLOR_U16(dst[1],
						color[1], src[0], src[1]);
				BLEND_COLOR_U16(dst[2],
						color[2], src[0], src[2]);
				BLEND_COLOR_U16(dst[3],
						color[3], src[0], src[3]);

				src += 4;
				dst += 4;
			}


			for (t = 0; t < n; t++) {

				BLEND_COLOR_U16_4(spp, dpp, 48, 0);
			}

			for (q = 0; q < m / 4; q++) {

				ddp[0] = MLIB_U16_MAX;

				BLEND_COLOR_U16(ddp[1],
						color[1], ssp[0], ssp[1]);
				BLEND_COLOR_U16(ddp[2],
						color[2], ssp[0], ssp[2]);
				BLEND_COLOR_U16(ddp[3],
						color[3], ssp[0], ssp[3]);

				ssp += 4;
				ddp += 4;
			}
		} else if (alpha == 1) {
			for (k = 0; k < begin / 4; k ++) {
				dst[1] = MLIB_U16_MAX;

				BLEND_COLOR_U16(dst[0],
						color[0], src[1], src[0]);
				BLEND_COLOR_U16(dst[2],
						color[2], src[1], src[2]);
				BLEND_COLOR_U16(dst[3],
						color[3], src[1], src[3]);

				src += 4;
				dst += 4;
			}


			for (t = 0; t < n; t++) {

				BLEND_COLOR_U16_4(spp, dpp, 32, 1);
			}

			for (q = 0; q < m / 4; q++) {

				ddp[1] = MLIB_U16_MAX;

				BLEND_COLOR_U16(ddp[0],
						color[0], ssp[1], ssp[0]);
				BLEND_COLOR_U16(ddp[2],
						color[2], ssp[1], ssp[2]);
				BLEND_COLOR_U16(ddp[3],
						color[3], ssp[1], ssp[3]);

				ssp += 4;
				ddp += 4;

			}

		} else if (alpha == 2) {

			for (k = 0; k < begin / 4; k ++) {
				dst[2] = MLIB_U16_MAX;

				BLEND_COLOR_U16(dst[0],
						color[0], src[2], src[0]);
				BLEND_COLOR_U16(dst[1],
						color[1], src[2], src[1]);
				BLEND_COLOR_U16(dst[3],
						color[3], src[2], src[3]);

				src += 4;
				dst += 4;

			}


			for (t = 0; t < n; t++) {

				BLEND_COLOR_U16_4(spp, dpp, 16, 2);
			}

			for (q = 0; q < m / 4; q++) {

				ddp[2] = MLIB_U16_MAX;

				BLEND_COLOR_U16(ddp[0],
						color[0], ssp[2], ssp[0]);
				BLEND_COLOR_U16(ddp[1],
						color[1], ssp[2], ssp[1]);
				BLEND_COLOR_U16(ddp[3],
						color[3], ssp[2], ssp[3]);

				ssp += 4;
				ddp += 4;
			}
		} else {

			for (k = 0; k < begin / 4; k ++) {
				dst[3] = MLIB_U16_MAX;

				BLEND_COLOR_U16(dst[0],
						color[0], src[3], src[0]);
				BLEND_COLOR_U16(dst[1],
						color[1], src[3], src[1]);
				BLEND_COLOR_U16(dst[2],
						color[2], src[3], src[2]);

				src += 4;
				dst += 4;

			}


			for (t = 0; t < n; t++) {

				BLEND_COLOR_U16_4(spp, dpp, 0, 3);
			}

			for (q = 0; q < m / 4; q++) {

				ddp[3] = MLIB_U16_MAX;

				BLEND_COLOR_U16(ddp[0],
						color[0], ssp[3], ssp[0]);
				BLEND_COLOR_U16(ddp[1],
						color[1], ssp[3], ssp[1]);
				BLEND_COLOR_U16(ddp[2],
						color[2], ssp[3], ssp[2]);

				ssp += 4;
				ddp += 4;
			}
		}

		sl += sstride;
		dl += dstride;
	}
}

/* *********************************************************** */

#else

void
mlib_s_ImageBlendColor_U16_2(
    mlib_u16 *sp,
    mlib_u16 *dp,
    mlib_s32 sstride,
    mlib_s32 dstride,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s32 channel,
    mlib_s32 alpha,
    const mlib_s32 *color)
{
	mlib_s32 i, j;
	mlib_u16 *sl, *dl;

	if (alpha == 0) {

		for (i = 0; i < height; i++) {

			sl = sp;
			dl = dp;

			for (j = 0; j < width; j++) {
				dl[0] = MLIB_U16_MAX;

				BLEND_COLOR_U16(dl[1], color[1], sl[0], sl[1]);

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

				dl[1] = MLIB_U16_MAX;

				BLEND_COLOR_U16(dl[0], color[0], sl[1], sl[0]);

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
mlib_s_ImageBlendColor_U16_4(
    mlib_u16 *sp,
    mlib_u16 *dp,
    mlib_s32 sstride,
    mlib_s32 dstride,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s32 channel,
    mlib_s32 alpha,
    const mlib_s32 *color)
{
	mlib_s32 i, j;
	mlib_u16 *sl, *dl;

	if (alpha == 0) {

		for (i = 0; i < height; i++) {

			sl = sp;
			dl = dp;

			for (j = 0; j < width; j++) {
				dl[0] = MLIB_U16_MAX;

				BLEND_COLOR_U16(dl[1], color[1], sl[0], sl[1]);
				BLEND_COLOR_U16(dl[2], color[2], sl[0], sl[2]);
				BLEND_COLOR_U16(dl[3], color[3], sl[0], sl[3]);

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

				dl[1] = MLIB_U16_MAX;

				BLEND_COLOR_U16(dl[0], color[0], sl[1], sl[0]);
				BLEND_COLOR_U16(dl[2], color[2], sl[1], sl[2]);
				BLEND_COLOR_U16(dl[3], color[3], sl[1], sl[3]);

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

				dl[2] = MLIB_U16_MAX;

				BLEND_COLOR_U16(dl[0], color[0], sl[2], sl[0]);
				BLEND_COLOR_U16(dl[1], color[1], sl[2], sl[1]);
				BLEND_COLOR_U16(dl[3], color[3], sl[2], sl[3]);

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

				dl[3] = MLIB_U16_MAX;

				BLEND_COLOR_U16(dl[0], color[0], sl[3], sl[0]);
				BLEND_COLOR_U16(dl[1], color[1], sl[3], sl[1]);
				BLEND_COLOR_U16(dl[2], color[2], sl[3], sl[2]);

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
mlib_s_ImageBlendColor_U16_3(
    mlib_u16 *sp,
    mlib_u16 *dp,
    mlib_s32 sstride,
    mlib_s32 dstride,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s32 channel,
    mlib_s32 alpha,
    const mlib_s32 *color)
{
	mlib_s32 i, j;
	mlib_u16 *sl, *dl;

	if (alpha == 0) {

		for (i = 0; i < height; i++) {

			sl = sp;
			dl = dp;

			for (j = 0; j < width; j++) {
				dl[0] = MLIB_U16_MAX;

				BLEND_COLOR_U16(dl[1], color[1], sl[0], sl[1]);
				BLEND_COLOR_U16(dl[2], color[2], sl[0], sl[2]);

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

				dl[1] = MLIB_U16_MAX;

				BLEND_COLOR_U16(dl[0], color[0], sl[1], sl[0]);
				BLEND_COLOR_U16(dl[2], color[2], sl[1], sl[2]);


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

				dl[2] = MLIB_U16_MAX;

				BLEND_COLOR_U16(dl[0], color[0], sl[2], sl[0]);
				BLEND_COLOR_U16(dl[1], color[1], sl[2], sl[1]);

				sl += 3;
				dl += 3;
			}

			sp += sstride;
			dp += dstride;
		}
	}
}

/* *********************************************************** */
