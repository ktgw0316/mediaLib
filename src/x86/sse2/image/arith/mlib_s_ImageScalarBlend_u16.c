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

#pragma ident   "@(#)mlib_s_ImageScalarBlend_u16.c	9.3	07/11/05 SMI"

/*
 * FUNCTION
 *	mlib_s_ImageScalarBlend_u16	- blend two images
 *
 * SYNOPSIS
 *	mlib_s_ImageScalarBlend_u16(mlib_u16 *dst,
 *				mlib_s32 dlb,
 *				const mlib_u16 *src1,
 *				mlib_s32 slb1,
 *				const mlib_u16 *src2,
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

#define	MLIB_S_IMAGESCALARBLEND_U16(D_STORE, S1_LOAD, S2_LOAD)          \
	{                                                               \
		__m128i dsts, dsts0, dsts1, srcs1, srcs2;               \
		__m128i part0, part1;                                   \
		srcs1  = S1_LOAD((void *)srcPtr1);                      \
		srcs2  = S2_LOAD((void *)srcPtr2);                      \
		srcs1 = _mm_xor_si128(srcs1, mask8000);                 \
		srcs2 = _mm_xor_si128(srcs2, mask8000);                 \
		part0 = _mm_mulhi_epi16(alphas, srcs1);                 \
		part1 = _mm_mulhi_epi16(betas, srcs2);                  \
		dsts = _mm_add_epi16(part0, part1);                     \
		dsts = _mm_xor_si128(                                   \
			_mm_add_epi16(dsts, dsts), mask8000);           \
		D_STORE((void *)dstPtr, dsts);                          \
		srcPtr1++;                                              \
		srcPtr2++;                                              \
		dstPtr++;                                               \
	}

#define	MLIB_S_IMAGESCALARBLEND3_U16(D_STORE, S1_LOAD, S2_LOAD)         \
	{                                                               \
		__m128i dsts, srcs1, srcs2;                             \
		__m128i part0, part1;                                   \
		srcs1  = S1_LOAD((void *)srcPtr1);                      \
		srcs2  = S2_LOAD((void *)srcPtr2);                      \
		srcs1 = _mm_xor_si128(srcs1, mask8000);                 \
		srcs2 = _mm_xor_si128(srcs2, mask8000);                 \
		part0 = _mm_mulhi_epi16(alphas, srcs1);                 \
		part1 = _mm_mulhi_epi16(betas, srcs2);                  \
		dsts   = _mm_add_epi16(part0, part1);                   \
		dsts = _mm_xor_si128(                                   \
			_mm_add_epi16(dsts, dsts), mask8000);           \
		D_STORE((void *)dstPtr, dsts);                          \
		srcs1  = S1_LOAD((void *)(srcPtr1 + 1));                \
		srcs2  = S2_LOAD((void *)(srcPtr2 + 1));                \
		srcs1 = _mm_xor_si128(srcs1, mask8000);                 \
		srcs2 = _mm_xor_si128(srcs2, mask8000);                 \
		part0 = _mm_mulhi_epi16(alphau, srcs1);                 \
		part1 = _mm_mulhi_epi16(betau, srcs2);                  \
		dsts   = _mm_add_epi16(part0, part1);                   \
		dsts = _mm_xor_si128(                                   \
			_mm_add_epi16(dsts, dsts), mask8000);           \
		D_STORE((void *)(dstPtr + 1), dsts);                    \
		srcs1  = S1_LOAD((void *)(srcPtr1 + 2));                \
		srcs2  = S2_LOAD((void *)(srcPtr2 + 2));                \
		srcs1 = _mm_xor_si128(srcs1, mask8000);                 \
		srcs2 = _mm_xor_si128(srcs2, mask8000);                 \
		part0 = _mm_mulhi_epi16(alphav, srcs1);                 \
		part1 = _mm_mulhi_epi16(betav, srcs2);                  \
		dsts   = _mm_add_epi16(part0, part1);                   \
		dsts = _mm_xor_si128(                                   \
			_mm_add_epi16(dsts, dsts), mask8000);           \
		D_STORE((void *)(dstPtr + 2), dsts);                    \
		srcPtr1 += 3;                                           \
		srcPtr2 += 3;                                           \
		dstPtr += 3;                                            \
	}

/* *********************************************************** */

#define	MLIB_C_IMAGESCALARBLEND_U16_1(sp1, sp2, dp)               \
	dp[i] = a0 * (sp1[i] - sp2[i]) + sp2[i]

#define	MLIB_C_IMAGESCALARBLEND_U16_2(sp1, sp2, dp)               \
	dp[i] = a0 * (sp1[i] - sp2[i]) + sp2[i];                  \
	dp[i + 1] = a1 * (sp1[i + 1] - sp2[i + 1]) + sp2[i + 1]

#define	MLIB_C_IMAGESCALARBLEND_U16_3(sp1, sp2, dp)               \
	dp[i] = a0 * (sp1[i] - sp2[i]) + sp2[i];                  \
	dp[i + 1] = a1 * (sp1[i + 1] - sp2[i + 1]) + sp2[i + 1];  \
	dp[i + 2] = a2 * (sp1[i + 2] - sp2[i + 2]) + sp2[i + 2]

#define	MLIB_C_IMAGESCALARBLEND_U16_4(sp1, sp2, dp)               \
	dp[i] = a0 * (sp1[i] - sp2[i]) + sp2[i];                  \
	dp[i + 1] = a1 * (sp1[i + 1] - sp2[i + 1]) + sp2[i + 1];  \
	dp[i + 2] = a2 * (sp1[i + 2] - sp2[i + 2]) + sp2[i + 2];  \
	dp[i + 3] = a3 * (sp1[i + 3] - sp2[i + 3]) + sp2[i + 3]

/* *********************************************************** */

void
mlib_s_ImageScalarBlend_u16(
	mlib_u16 *dst,
	mlib_s32 dlb,
	const mlib_u16 *src1,
	mlib_s32 slb1,
	const mlib_u16 *src2,
	mlib_s32 slb2,
	const mlib_s32 *alpha,
	mlib_s32 xsize,
	mlib_s32 ysize,
	mlib_s32 nchan)
{
	mlib_s32 i, j, nsize;
	mlib_s32 alpha0, alpha1, alpha2, alpha3;
	__m128i *srcPtr1, *srcPtr2, *dstPtr;
	mlib_u16 *dl = dst;
	mlib_u16 *sl1 = (mlib_u16 *)src1, *sl2 = (mlib_u16 *)src2;
	__m128i alphas, alphau, alphav;
	__m128i betas, betau, betav;
	__m128i ones, mask8000, _s_zero = _mm_setzero_si128();
	mlib_s32 res, acc, al, sdata1, sdata2;
	mlib_d64 a0, a1, a2, a3;

	ones = _mm_set1_epi32(0x7fff7fff);
	mask8000 = _mm_set1_epi32(0x80008000);
	nsize = xsize * nchan;

	switch (nchan) {
	case 1:
		a0 = -(alpha[0] & MASK) / (mlib_d64)MLIB_S32_MIN;
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
				for (i = 0; i <= (nsize - 8); i += 8) {
					MLIB_S_IMAGESCALARBLEND_U16(
						_mm_store_si128,
						_mm_load_si128,
						_mm_load_si128);
				}
				for (; i < nsize; i++) {
					MLIB_C_IMAGESCALARBLEND_U16_1(
						sl1, sl2, dl);
				}
				sl1 = (mlib_u16 *)((mlib_u8 *)sl1 + slb1);
				sl2 = (mlib_u16 *)((mlib_u8 *)sl2 + slb2);
				dl = (mlib_u16 *)((mlib_u8 *)dl + dlb);
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
				for (i = 0; i <= (nsize - 8); i += 8) {
					MLIB_S_IMAGESCALARBLEND_U16(
						_mm_storeu_si128,
						_mm_load_si128,
						_mm_load_si128);
				}
				for (; i < nsize; i++) {
					MLIB_C_IMAGESCALARBLEND_U16_1(
						sl1, sl2, dl);
				}
				sl1 = (mlib_u16 *)((mlib_u8 *)sl1 + slb1);
				sl2 = (mlib_u16 *)((mlib_u8 *)sl2 + slb2);
				dl = (mlib_u16 *)((mlib_u8 *)dl + dlb);
			}
		} else {
			for (j = 0; j < ysize; j ++) {
				srcPtr1 = (__m128i *)sl1;
				srcPtr2 = (__m128i *)sl2;
				dstPtr = (__m128i *)dl;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i <= (nsize - 8); i += 8) {
					MLIB_S_IMAGESCALARBLEND_U16(
						_mm_storeu_si128,
						_mm_loadu_si128,
						_mm_loadu_si128);
				}
				for (; i < nsize; i++) {
					MLIB_C_IMAGESCALARBLEND_U16_1(
						sl1, sl2, dl);
				}
				sl1 = (mlib_u16 *)((mlib_u8 *)sl1 + slb1);
				sl2 = (mlib_u16 *)((mlib_u8 *)sl2 + slb2);
				dl = (mlib_u16 *)((mlib_u8 *)dl + dlb);
			}
		}
		break;
	case 2:
		a0 = -(alpha[0] & MASK) / (mlib_d64)MLIB_S32_MIN;
		a1 = -(alpha[1] & MASK) / (mlib_d64)MLIB_S32_MIN;
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
				for (i = 0; i <= (nsize - 8); i += 8) {
					MLIB_S_IMAGESCALARBLEND_U16(
						_mm_store_si128,
						_mm_load_si128,
						_mm_load_si128);
				}
				for (; i < nsize; i += 2) {
					MLIB_C_IMAGESCALARBLEND_U16_2(
						sl1, sl2, dl);
				}
				sl1 = (mlib_u16 *)((mlib_u8 *)sl1 + slb1);
				sl2 = (mlib_u16 *)((mlib_u8 *)sl2 + slb2);
				dl = (mlib_u16 *)((mlib_u8 *)dl + dlb);
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
				for (i = 0; i <= (nsize - 8); i += 8) {
					MLIB_S_IMAGESCALARBLEND_U16(
						_mm_storeu_si128,
						_mm_load_si128,
						_mm_load_si128);
				}
				for (; i < nsize; i += 2) {
					MLIB_C_IMAGESCALARBLEND_U16_2(
						sl1, sl2, dl);
				}
				sl1 = (mlib_u16 *)((mlib_u8 *)sl1 + slb1);
				sl2 = (mlib_u16 *)((mlib_u8 *)sl2 + slb2);
				dl = (mlib_u16 *)((mlib_u8 *)dl + dlb);
			}
		} else {
			for (j = 0; j < ysize; j ++) {
				srcPtr1 = (__m128i *)sl1;
				srcPtr2 = (__m128i *)sl2;
				dstPtr = (__m128i *)dl;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i <= (nsize - 8); i += 8) {
					MLIB_S_IMAGESCALARBLEND_U16(
						_mm_storeu_si128,
						_mm_loadu_si128,
						_mm_loadu_si128);
				}
				for (; i < nsize; i += 2) {
					MLIB_C_IMAGESCALARBLEND_U16_2(
						sl1, sl2, dl);
				}
				sl1 = (mlib_u16 *)((mlib_u8 *)sl1 + slb1);
				sl2 = (mlib_u16 *)((mlib_u8 *)sl2 + slb2);
				dl = (mlib_u16 *)((mlib_u8 *)dl + dlb);
			}
		}
		break;
	case 3:
		a0 = -(alpha[0] & MASK) / (mlib_d64)MLIB_S32_MIN;
		a1 = -(alpha[1] & MASK) / (mlib_d64)MLIB_S32_MIN;
		a2 = -(alpha[2] & MASK) / (mlib_d64)MLIB_S32_MIN;
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
				for (i = 0; i <= (nsize - 24); i += 24) {
					MLIB_S_IMAGESCALARBLEND3_U16(
						_mm_store_si128,
						_mm_load_si128,
						_mm_load_si128);
				}
				for (; i < nsize; i += 3) {
					MLIB_C_IMAGESCALARBLEND_U16_3(
						sl1, sl2, dl);
				}
				sl1 = (mlib_u16 *)((mlib_u8 *)sl1 + slb1);
				sl2 = (mlib_u16 *)((mlib_u8 *)sl2 + slb2);
				dl = (mlib_u16 *)((mlib_u8 *)dl + dlb);
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
				for (i = 0; i <= (nsize - 24); i += 24) {
					MLIB_S_IMAGESCALARBLEND3_U16(
						_mm_storeu_si128,
						_mm_load_si128,
						_mm_load_si128);
				}
				for (; i < nsize; i += 3) {
					MLIB_C_IMAGESCALARBLEND_U16_3(
						sl1, sl2, dl);
				}
				sl1 = (mlib_u16 *)((mlib_u8 *)sl1 + slb1);
				sl2 = (mlib_u16 *)((mlib_u8 *)sl2 + slb2);
				dl = (mlib_u16 *)((mlib_u8 *)dl + dlb);
			}
		} else {
			for (j = 0; j < ysize; j ++) {
				srcPtr1 = (__m128i *)sl1;
				srcPtr2 = (__m128i *)sl2;
				dstPtr = (__m128i *)dl;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i <= (nsize - 24); i += 24) {
					MLIB_S_IMAGESCALARBLEND3_U16(
						_mm_storeu_si128,
						_mm_loadu_si128,
						_mm_loadu_si128);
				}
				for (; i < nsize; i += 3) {
					MLIB_C_IMAGESCALARBLEND_U16_3(
						sl1, sl2, dl);
				}
				sl1 = (mlib_u16 *)((mlib_u8 *)sl1 + slb1);
				sl2 = (mlib_u16 *)((mlib_u8 *)sl2 + slb2);
				dl = (mlib_u16 *)((mlib_u8 *)dl + dlb);
			}
		}
		break;

	case 4:
		a0 = -(alpha[0] & MASK) / (mlib_d64)MLIB_S32_MIN;
		a1 = -(alpha[0] & MASK) / (mlib_d64)MLIB_S32_MIN;
		a2 = -(alpha[0] & MASK) / (mlib_d64)MLIB_S32_MIN;
		a3 = -(alpha[0] & MASK) / (mlib_d64)MLIB_S32_MIN;
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
				for (i = 0; i <= (nsize - 8); i += 8) {
					MLIB_S_IMAGESCALARBLEND_U16(
						_mm_store_si128,
						_mm_load_si128,
						_mm_load_si128);
				}
				for (; i < nsize; i += 4) {
					MLIB_C_IMAGESCALARBLEND_U16_4(
						sl1, sl2, dl);
				}
				sl1 = (mlib_u16 *)((mlib_u8 *)sl1 + slb1);
				sl2 = (mlib_u16 *)((mlib_u8 *)sl2 + slb2);
				dl = (mlib_u16 *)((mlib_u8 *)dl + dlb);
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
				for (i = 0; i <= (nsize - 8); i += 8) {
					MLIB_S_IMAGESCALARBLEND_U16(
						_mm_storeu_si128,
						_mm_load_si128,
						_mm_load_si128);
				}
				for (; i < nsize; i += 4) {
					MLIB_C_IMAGESCALARBLEND_U16_4(
						sl1, sl2, dl);
				}
				sl1 = (mlib_u16 *)((mlib_u8 *)sl1 + slb1);
				sl2 = (mlib_u16 *)((mlib_u8 *)sl2 + slb2);
				dl = (mlib_u16 *)((mlib_u8 *)dl + dlb);
			}
		} else {
			for (j = 0; j < ysize; j ++) {
				srcPtr1 = (__m128i *)sl1;
				srcPtr2 = (__m128i *)sl2;
				dstPtr = (__m128i *)dl;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i <= (nsize - 8); i += 8) {
					MLIB_S_IMAGESCALARBLEND_U16(
						_mm_storeu_si128,
						_mm_loadu_si128,
						_mm_loadu_si128);
				}
				for (; i < nsize; i += 4) {
					MLIB_C_IMAGESCALARBLEND_U16_4(
						sl1, sl2, dl);
				}
				sl1 = (mlib_u16 *)((mlib_u8 *)sl1 + slb1);
				sl2 = (mlib_u16 *)((mlib_u8 *)sl2 + slb2);
				dl = (mlib_u16 *)((mlib_u8 *)dl + dlb);
			}
		}
		break;
	}
}

/* *********************************************************** */
