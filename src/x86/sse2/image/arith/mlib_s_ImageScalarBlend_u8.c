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

#pragma ident   "@(#)mlib_s_ImageScalarBlend_u8.c	9.3	07/11/05 SMI"

/*
 * FUNCTION
 *	mlib_s_ImageScalarBlend_u8	- blend two images
 *
 * SYNOPSIS
 *	mlib_s_ImageScalarBlend_u8(mlib_u8 *dst,
 *				mlib_s32 dlb,
 *				const mlib_u8 *src1,
 *				mlib_s32 slb1,
 *				const mlib_u8 *src2,
 *				mlib_s32 slb2,
 *				const mlib_s32 *alpha,
 *				mlib_s32 xsize,
 *				mlib_s32 ysize,
 *				mlib_s32 nchan);
 *
 * ARGUMENT
 *      dst	pointer to output or destination image data
 *      dlb	line stride of destination image data in bytes
 *      src1	pointer to the first source image data
 *      slb1	line stride of the first source image data in bytes
 *      src2	pointer to the second source image data
 *      slb2	line stride of the second source image data in bytes
 *      alpha	pointer to the alpha data
 *      xsize	image width in pixels
 *      ysize	image height in pixels
 *      nchan	number of channels in image
 *
 * DESCRIPTION
 *	dst[x][y][c] = alpha[c] * src1[x][y][c] +
 *	               (1 - alpha[c]) * src2[x][y][c]
 */

#ifdef __SUNPRO_C
#include <sunmedia_intrin.h>
#else
#include <emmintrin.h>
#endif

#include <mlib_image.h>
#include "mlib_s_ImageScalarBlend.h"

/* *********************************************************** */
#define	MASK    0x7fffffff
/* *********************************************************** */

#define	MLIB_S_IMAGESCALARBLEND_U8(D_STORE, S1_LOAD, S2_LOAD)	        \
	{                                                               \
		__m128i dsts, dsts0, dsts1, srcs1, srcs2;               \
		__m128i s0, s1, s2, s3;                                 \
		__m128i part0, part1, part2, part3;                     \
		srcs1  = S1_LOAD((void *)srcPtr1);                      \
		srcs2  = S2_LOAD((void *)srcPtr2);                      \
		s0 = _mm_slli_epi16(                                    \
			_mm_unpacklo_epi8(srcs1, _s_zero), 1);          \
		s1 = _mm_slli_epi16(                                    \
			_mm_unpackhi_epi8(srcs1, _s_zero), 1);          \
		s2 = _mm_slli_epi16(                                    \
			_mm_unpacklo_epi8(srcs2, _s_zero), 1);          \
		s3 = _mm_slli_epi16(                                    \
			_mm_unpackhi_epi8(srcs2, _s_zero), 1);          \
		part0 = _mm_mulhi_epi16(alphas, s0);                    \
		part1 = _mm_mulhi_epi16(alphas, s1);                    \
		part2 = _mm_mulhi_epi16(betas, s2);                     \
		part3 = _mm_mulhi_epi16(betas, s3);                     \
		dsts0 = _mm_add_epi16(part0, part2);                    \
		dsts1 = _mm_add_epi16(part1, part3);                    \
		dsts = _mm_packus_epi16(dsts0, dsts1);                  \
		D_STORE((void *)dstPtr, dsts);		                \
		srcPtr1++;                                              \
		srcPtr2++;                                              \
		dstPtr++;                                               \
	}

#define	MLIB_S_IMAGESCALARBLEND3_U8(D_STORE, S1_LOAD, S2_LOAD)	        \
	{                                                               \
		__m128i dsts, srcs1, srcs2, s0, s1;                     \
		__m128i part0, part1;                                   \
		srcs1  = S1_LOAD((void *)srcPtr1);                      \
		srcs2  = S2_LOAD((void *)srcPtr2);                      \
		s0 = _mm_slli_epi16(                                    \
			_mm_unpacklo_epi8(srcs1, _s_zero), 1);          \
		s1 = _mm_slli_epi16(                                    \
			_mm_unpacklo_epi8(srcs2, _s_zero), 1);          \
		part0 = _mm_mulhi_epi16(alphas, s0);                    \
		part1 = _mm_mulhi_epi16(betas, s1);                     \
		dsts   = _mm_add_epi16(part0, part1);	                \
		dsts = _mm_packus_epi16(dsts, _s_zero);                 \
		D_STORE((void *)dstPtr, dsts);		                \
		srcs1  = S1_LOAD((void *)((mlib_u8 *)srcPtr1 + 8));     \
		srcs2  = S2_LOAD((void *)((mlib_u8 *)srcPtr2 + 8));     \
		s0 = _mm_slli_epi16(                                    \
			_mm_unpacklo_epi8(srcs1, _s_zero), 1);          \
		s1 = _mm_slli_epi16(                                    \
			_mm_unpacklo_epi8(srcs2, _s_zero), 1);          \
		part0 = _mm_mulhi_epi16(alphau, s0);                    \
		part1 = _mm_mulhi_epi16(betau, s1);                     \
		dsts   = _mm_add_epi16(part0, part1);	                \
		dsts = _mm_packus_epi16(dsts, _s_zero);                 \
		D_STORE((void *)((mlib_u8 *)dstPtr + 8), dsts);		\
		srcs1  = S1_LOAD((void *)((mlib_u8 *)srcPtr1 + 16));	\
		srcs2  = S2_LOAD((void *)((mlib_u8 *)srcPtr2 + 16));	\
		s0 = _mm_slli_epi16(                                    \
			_mm_unpacklo_epi8(srcs1, _s_zero), 1);          \
		s1 = _mm_slli_epi16(                                    \
			_mm_unpacklo_epi8(srcs2, _s_zero), 1);          \
		part0 = _mm_mulhi_epi16(alphav, s0);                    \
		part1 = _mm_mulhi_epi16(betav, s1);                     \
		dsts   = _mm_add_epi16(part0, part1);	                \
		dsts = _mm_packus_epi16(dsts, _s_zero);                 \
		D_STORE((void *)((mlib_u8 *)dstPtr + 16), dsts);        \
		srcPtr1 = (__m128i *)((mlib_u8 *)srcPtr1 + 24);         \
		srcPtr2 = (__m128i *)((mlib_u8 *)srcPtr2 + 24);         \
		dstPtr = (__m128i *)((mlib_u8 *)dstPtr + 24);           \
	}

/* *********************************************************** */
#define	CHAR3(src)  ((mlib_s32)(((mlib_u32) src) >> 24))
#define	CHAR2(src)  ((mlib_s32)((src >> 16) & 0xff))
#define	CHAR1(src)  ((mlib_s32)((src >> 8) & 0xff))
#define	CHAR0(src)  ((mlib_s32)(src & 0xff))

#define	MLIB_C_IMAGESCALARBLEND_U8_1(sp1, sp2, dp)                 \
	dp[i] = lut0[sp1[i] - sp2[i]] + sp2[i]

#define	MLIB_C_IMAGESCALARBLEND_U8_2(sp1, sp2, dp)                 \
	sdata1 = *((mlib_s32 *)(sp1 + i));                         \
	sdata2 = *((mlib_s32 *)(sp2 + i));                         \
	dp[i] = lut0[CHAR0(sdata1) - CHAR0(sdata2)] +              \
		CHAR0(sdata2);                                     \
	dp[i + 1] = lut1[CHAR1(sdata1) - CHAR1(sdata2)] +          \
		CHAR1(sdata2)

#define	MLIB_C_IMAGESCALARBLEND_U8_3(sp1, sp2, dp)                 \
	sdata1 = *((mlib_s32 *)(sp1 + i));                         \
	sdata2 = *((mlib_s32 *)(sp2 + i));                         \
	dp[i] = lut0[CHAR0(sdata1) - CHAR0(sdata2)] +              \
		CHAR0(sdata2);                                     \
	dp[i + 1] = lut1[CHAR1(sdata1) - CHAR1(sdata2)] +          \
		CHAR1(sdata2);                                     \
	dp[i + 2] = lut2[CHAR2(sdata1) - CHAR2(sdata2)] +          \
		CHAR2(sdata2)

#define	MLIB_C_IMAGESCALARBLEND_U8_4(sp1, sp2, dp)                 \
	sdata1 = *((mlib_s32 *)(sp1 + i));                         \
	sdata2 = *((mlib_s32 *)(sp2 + i));                         \
	dp[i] = lut0[CHAR0(sdata1) - CHAR0(sdata2)] +              \
		CHAR0(sdata2);                                     \
	dp[i + 1] = lut1[CHAR1(sdata1) - CHAR1(sdata2)] +          \
		CHAR1(sdata2);                                     \
	dp[i + 2] = lut2[CHAR2(sdata1) - CHAR2(sdata2)] +          \
		CHAR2(sdata2);                                     \
	dp[i + 3] = lut3[CHAR3(sdata1) - CHAR3(sdata2)] +          \
		CHAR3(sdata2)

/* *********************************************************** */

void
mlib_s_ImageScalarBlend_u8(
	mlib_u8 *dst,
	mlib_s32 dlb,
	const mlib_u8 *src1,
	mlib_s32 slb1,
	const mlib_u8 *src2,
	mlib_s32 slb2,
	const mlib_s32 *alpha,
	mlib_s32 xsize,
	mlib_s32 ysize,
	mlib_s32 nchan)
{
	mlib_s32 i, j, nsize;
	mlib_s32 alpha0, alpha1, alpha2, alpha3;
	__m128i *srcPtr1, *srcPtr2, *dstPtr;
	mlib_u8 *dl = dst;
	mlib_u8 *sl1 = (mlib_u8 *)src1, *sl2 = (mlib_u8 *)src2;
	__m128i alphas, alphau, alphav;
	__m128i betas, betau, betav;
	__m128i ones, _s_zero = _mm_setzero_si128();
	mlib_u8 plut[4 * 511], *lut[4], *lutj;
	mlib_u8 *lut0, *lut1, *lut2, *lut3;
	mlib_s32 res, acc, al, sdata1, sdata2;

	for (j = 0; j < nchan; j++) {
		acc = 1 << 22;
		al = (alpha[j] & MASK) >> 8;
		lut[j] = lutj = plut + j * 511 + 255;
		lutj[0] = 0;
		for (i = 1; i < 256; i++) {
			acc += al;
			res = acc >> 23;
			lutj[i] = res;
			lutj[-i] = -res;
		}
	}

	ones = _mm_set1_epi32(0x7fff7fff);
	nsize = xsize * nchan;
	switch (nchan) {
	case 1:
		lut0 = lut[0];
		alpha0 = (alpha[0] & MASK) >> 16;
		alphas = _mm_set1_epi16(alpha0);
		betas  = _mm_sub_epi16(ones, alphas);
		if ((((mlib_addr)dst | dlb |
			(mlib_addr)src1 | slb1 |
			(mlib_addr)src2 | slb2) & 0xf) == 0) {
			for (j = 0; j < ysize; j ++) {
				srcPtr1 = (__m128i *)sl1;
				srcPtr2 = (__m128i *)sl2;
				dstPtr = (__m128i *)dl;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i <= (nsize - 16); i += 16) {
					MLIB_S_IMAGESCALARBLEND_U8(
						_mm_store_si128,
						_mm_load_si128,
						_mm_load_si128);
				}
				for (; i < nsize; i++) {
					MLIB_C_IMAGESCALARBLEND_U8_1(
						sl1, sl2, dl);
				}
				sl1 += slb1;
				sl2 += slb2;
				dl += dlb;
			}
		} else
		if ((((mlib_addr)src1 | slb1 |
			(mlib_addr)src2 | slb2) & 0xf) == 0) {
			for (j = 0; j < ysize; j ++) {
				srcPtr1 = (__m128i *)sl1;
				srcPtr2 = (__m128i *)sl2;
				dstPtr = (__m128i *)dl;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i <= (nsize - 16); i += 16) {
					MLIB_S_IMAGESCALARBLEND_U8(
						_mm_storeu_si128,
						_mm_load_si128,
						_mm_load_si128);
				}
				for (; i < nsize; i++) {
					MLIB_C_IMAGESCALARBLEND_U8_1(
						sl1, sl2, dl);
				}
				sl1 += slb1;
				sl2 += slb2;
				dl += dlb;
			}
		} else {
			for (j = 0; j < ysize; j ++) {
				srcPtr1 = (__m128i *)sl1;
				srcPtr2 = (__m128i *)sl2;
				dstPtr = (__m128i *)dl;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i <= (nsize - 16); i += 16) {
					MLIB_S_IMAGESCALARBLEND_U8(
						_mm_storeu_si128,
						_mm_loadu_si128,
						_mm_loadu_si128);
				}
				for (; i < nsize; i++) {
					MLIB_C_IMAGESCALARBLEND_U8_1(
						sl1, sl2, dl);
				}
				sl1 += slb1;
				sl2 += slb2;
				dl += dlb;
			}
		}
		break;
	case 2:
		lut0 = lut[0];
		lut1 = lut[1];
		alpha0 = (alpha[0] & MASK) >> 16;
		alpha1 = (((alpha[1] & MASK) >> 16) << 16) | alpha0;
		alphas = _mm_set1_epi32(alpha1);
		betas  = _mm_sub_epi16(ones, alphas);
		if ((((mlib_addr)dst | dlb |
			(mlib_addr)src1 | slb1 |
			(mlib_addr)src2 | slb2) & 0xf) == 0) {
			for (j = 0; j < ysize; j ++) {
				srcPtr1 = (__m128i *)sl1;
				srcPtr2 = (__m128i *)sl2;
				dstPtr = (__m128i *)dl;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i <= (nsize - 16); i += 16) {
					MLIB_S_IMAGESCALARBLEND_U8(
						_mm_store_si128,
						_mm_load_si128,
						_mm_load_si128);
				}
				for (; i < nsize; i += 2) {
					MLIB_C_IMAGESCALARBLEND_U8_2(
						sl1, sl2, dl);
				}
				sl1 += slb1;
				sl2 += slb2;
				dl += dlb;
			}
		} else
		if ((((mlib_addr)src1 | slb1 |
			(mlib_addr)src2 | slb2) & 0xf) == 0) {
			for (j = 0; j < ysize; j ++) {
				srcPtr1 = (__m128i *)sl1;
				srcPtr2 = (__m128i *)sl2;
				dstPtr = (__m128i *)dl;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i <= (nsize - 16); i += 16) {
					MLIB_S_IMAGESCALARBLEND_U8(
						_mm_storeu_si128,
						_mm_load_si128,
						_mm_load_si128);
				}
				for (; i < nsize; i += 2) {
					MLIB_C_IMAGESCALARBLEND_U8_2(
						sl1, sl2, dl);
				}
				sl1 += slb1;
				sl2 += slb2;
				dl += dlb;
			}
		} else {
			for (j = 0; j < ysize; j ++) {
				srcPtr1 = (__m128i *)sl1;
				srcPtr2 = (__m128i *)sl2;
				dstPtr = (__m128i *)dl;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i <= (nsize - 16); i += 16) {
					MLIB_S_IMAGESCALARBLEND_U8(
						_mm_storeu_si128,
						_mm_loadu_si128,
						_mm_loadu_si128);
				}
				for (; i < nsize; i += 2) {
					MLIB_C_IMAGESCALARBLEND_U8_2(
						sl1, sl2, dl);
				}
				sl1 += slb1;
				sl2 += slb2;
				dl += dlb;
			}
		}
		break;
	case 3:
		lut0 = lut[0];
		lut1 = lut[1];
		lut2 = lut[2];
		alpha0 = (alpha[0] & MASK) >> 16;
		alpha1 = (alpha[1] & MASK) >> 16;
		alpha2 = (alpha[2] & MASK) >> 16;
		alphas = _mm_set_epi16(alpha1, alpha0, alpha2,
			alpha1, alpha0, alpha2, alpha1, alpha0);
		alphau = _mm_set_epi16(alpha0, alpha2, alpha1,
			alpha0, alpha2, alpha1, alpha0, alpha2);
		alphav = _mm_set_epi16(alpha2, alpha1, alpha0,
			alpha2, alpha1, alpha0, alpha2, alpha1);
		betas  = _mm_sub_epi16(ones, alphas);
		betau  = _mm_sub_epi16(ones, alphau);
		betav  = _mm_sub_epi16(ones, alphav);

		for (j = 0; j < ysize; j ++) {
			srcPtr1 = (__m128i *)sl1;
			srcPtr2 = (__m128i *)sl2;
			dstPtr = (__m128i *)dl;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i <= (nsize - 24); i += 24) {
				MLIB_S_IMAGESCALARBLEND3_U8(
					_mm_storel_epi64,
					_mm_loadl_epi64,
					_mm_loadl_epi64);
			}
			for (; i < nsize; i += 3) {
				MLIB_C_IMAGESCALARBLEND_U8_3(sl1, sl2, dl);
			}
			sl1 += slb1;
			sl2 += slb2;
			dl += dlb;
		}
		break;
	case 4:
		lut0 = lut[0];
		lut1 = lut[1];
		lut2 = lut[2];
		lut3 = lut[3];
		alpha0 = (alpha[0] & MASK) >> 16;
		alpha1 = (((alpha[1] & MASK) >> 16) << 16) | alpha0;
		alpha2 = (alpha[2] & MASK) >> 16;
		alpha3 = (((alpha[3] & MASK) >> 16) << 16) | alpha2;
		alphas = _mm_set_epi32(alpha3, alpha1, alpha3, alpha1);
		betas  = _mm_sub_epi16(ones, alphas);

		if ((((mlib_addr)dst | dlb |
			(mlib_addr)src1 | slb1 |
			(mlib_addr)src2 | slb2) & 0xf) == 0) {
			for (j = 0; j < ysize; j ++) {
				srcPtr1 = (__m128i *)sl1;
				srcPtr2 = (__m128i *)sl2;
				dstPtr = (__m128i *)dl;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i <= (nsize - 16); i += 16) {
					MLIB_S_IMAGESCALARBLEND_U8(
						_mm_store_si128,
						_mm_load_si128,
						_mm_load_si128);
				}
				for (; i < nsize; i += 4) {
					MLIB_C_IMAGESCALARBLEND_U8_4(
						sl1, sl2, dl);
				}

				sl1 += slb1;
				sl2 += slb2;
				dl += dlb;
			}
		} else
		if ((((mlib_addr)src1 | slb1 |
			(mlib_addr)src2 | slb2) & 0xf) == 0) {
			for (j = 0; j < ysize; j ++) {
				srcPtr1 = (__m128i *)sl1;
				srcPtr2 = (__m128i *)sl2;
				dstPtr = (__m128i *)dl;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i <= (nsize - 16); i += 16) {
					MLIB_S_IMAGESCALARBLEND_U8(
						_mm_storeu_si128,
						_mm_load_si128,
						_mm_load_si128);
				}
				for (; i < nsize; i += 4) {
					MLIB_C_IMAGESCALARBLEND_U8_4(
						sl1, sl2, dl);
				}
				sl1 += slb1;
				sl2 += slb2;
				dl += dlb;
			}
		} else {
			for (j = 0; j < ysize; j ++) {
				srcPtr1 = (__m128i *)sl1;
				srcPtr2 = (__m128i *)sl2;
				dstPtr = (__m128i *)dl;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i <= (nsize - 16); i += 16) {
					MLIB_S_IMAGESCALARBLEND_U8(
						_mm_storeu_si128,
						_mm_loadu_si128,
						_mm_loadu_si128);
				}
				for (; i < nsize; i += 4) {
					MLIB_C_IMAGESCALARBLEND_U8_4(
						sl1, sl2, dl);
				}
				sl1 += slb1;
				sl2 += slb2;
				dl += dlb;
			}
		}
		break;
	}
}

/* *********************************************************** */
