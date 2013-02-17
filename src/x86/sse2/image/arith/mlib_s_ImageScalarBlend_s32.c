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

#pragma ident   "@(#)mlib_s_ImageScalarBlend_s32.c	9.3	07/11/05 SMI"

/*
 * FUNCTION
 *	mlib_s_ImageScalarBlend_s32	- blend two images
 *
 * SYNOPSIS
 *	mlib_s_ImageScalarBlend_s32(mlib_s32 *dst,
 *				mlib_s32 dlb,
 *				const mlib_s32 *src1,
 *				mlib_s32 slb1,
 *				const mlib_s32 *src2,
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

#define	MLIB_S_IMAGESCALARBLEND_S32(D_STORE, S1_LOAD, S2_LOAD)          \
	{                                                               \
		__m128i srcs1, srcs2, dsts0, dsts1, dsts;               \
		__m128d part0, part1, part2, part3;                     \
		srcs1  = S1_LOAD((void *)srcPtr1);                      \
		srcs2  = S2_LOAD((void *)srcPtr2);                      \
		part0 = _mm_cvtepi32_pd(srcs1);                         \
		part1 = _mm_cvtepi32_pd(_mm_srli_si128(srcs1, 8));      \
		part2 = _mm_cvtepi32_pd(srcs2);                         \
		part3 = _mm_cvtepi32_pd(_mm_srli_si128(srcs2, 8));      \
		part0 = _mm_mul_pd(alphas0, part0);                     \
		part1 = _mm_mul_pd(alphas1, part1);                     \
		part2 = _mm_mul_pd(betas0, part2);                      \
		part3 = _mm_mul_pd(betas1, part3);                      \
		dsts0 = _mm_cvtpd_epi32(_mm_add_pd(part0, part2));      \
		dsts1 = _mm_cvtpd_epi32(_mm_add_pd(part1, part3));      \
		dsts = _mm_unpacklo_epi64(dsts0, dsts1);                \
		D_STORE((void *)dstPtr, dsts);                          \
		srcPtr1++;                                              \
		srcPtr2++;                                              \
		dstPtr++;                                               \
	}

#define	MLIB_S_IMAGESCALARBLEND3_S32(D_STORE, S1_LOAD, S2_LOAD)         \
	{                                                               \
		__m128i srcs1, srcs2, dsts0, dsts1, dsts;               \
		__m128d part0, part1, part2, part3;                     \
		srcs1  = S1_LOAD((void *)srcPtr1);                      \
		srcs2  = S2_LOAD((void *)srcPtr2);                      \
		part0 = _mm_cvtepi32_pd(srcs1);                         \
		part1 = _mm_cvtepi32_pd(_mm_srli_si128(srcs1, 8));      \
		part2 = _mm_cvtepi32_pd(srcs2);                         \
		part3 = _mm_cvtepi32_pd(_mm_srli_si128(srcs2, 8));      \
		part0 = _mm_mul_pd(alphas0, part0);                     \
		part1 = _mm_mul_pd(alphas1, part1);                     \
		part2 = _mm_mul_pd(betas0, part2);                      \
		part3 = _mm_mul_pd(betas1, part3);                      \
		dsts0 = _mm_cvtpd_epi32(_mm_add_pd(part0, part2));      \
		dsts1 = _mm_cvtpd_epi32(_mm_add_pd(part1, part3));      \
		dsts = _mm_unpacklo_epi64(dsts0, dsts1);                \
		D_STORE((void *)dstPtr, dsts);                          \
		srcs1  = S1_LOAD((void *)(srcPtr1 + 1));                \
		srcs2  = S2_LOAD((void *)(srcPtr2 + 1));                \
		part0 = _mm_cvtepi32_pd(srcs1);                         \
		part1 = _mm_cvtepi32_pd(_mm_srli_si128(srcs1, 8));      \
		part2 = _mm_cvtepi32_pd(srcs2);                         \
		part3 = _mm_cvtepi32_pd(_mm_srli_si128(srcs2, 8));      \
		part0 = _mm_mul_pd(alphau0, part0);                     \
		part1 = _mm_mul_pd(alphau1, part1);                     \
		part2 = _mm_mul_pd(betau0, part2);                      \
		part3 = _mm_mul_pd(betau1, part3);                      \
		dsts0 = _mm_cvtpd_epi32(_mm_add_pd(part0, part2));      \
		dsts1 = _mm_cvtpd_epi32(_mm_add_pd(part1, part3));      \
		dsts = _mm_unpacklo_epi64(dsts0, dsts1);                \
		D_STORE((void *)(dstPtr + 1), dsts);                    \
		srcs1  = S1_LOAD((void *)(srcPtr1 + 2));                \
		srcs2  = S2_LOAD((void *)(srcPtr2 + 2));                \
		part0 = _mm_cvtepi32_pd(srcs1);                         \
		part1 = _mm_cvtepi32_pd(_mm_srli_si128(srcs1, 8));      \
		part2 = _mm_cvtepi32_pd(srcs2);                         \
		part3 = _mm_cvtepi32_pd(_mm_srli_si128(srcs2, 8));      \
		part0 = _mm_mul_pd(alphav0, part0);                     \
		part1 = _mm_mul_pd(alphav1, part1);                     \
		part2 = _mm_mul_pd(betav0, part2);                      \
		part3 = _mm_mul_pd(betav1, part3);                      \
		dsts0 = _mm_cvtpd_epi32(_mm_add_pd(part0, part2));      \
		dsts1 = _mm_cvtpd_epi32(_mm_add_pd(part1, part3));      \
		dsts = _mm_unpacklo_epi64(dsts0, dsts1);                \
		D_STORE((void *)(dstPtr + 2), dsts);                    \
		srcPtr1 += 3;                                           \
		srcPtr2 += 3;                                           \
		dstPtr += 3;                                            \
	}

/* *********************************************************** */

#define	MLIB_C_IMAGESCALARBLEND_S32_1(sp1, sp2, dp)             \
	dp[i] = a0 * sp1[i] + (1 - a0) * sp2[i]

#define	MLIB_C_IMAGESCALARBLEND_S32_2(sp1, sp2, dp)             \
	dp[i] = a0 * sp1[i] + (1 - a0) * sp2[i];                \
	dp[i + 1] = a1 * sp1[i + 1] + (1 - a1) * sp2[i + 1]

#define	MLIB_C_IMAGESCALARBLEND_S32_3(sp1, sp2, dp)             \
	dp[i] = a0 * sp1[i] + (1 - a0) * sp2[i];                \
	dp[i + 1] = a1 * sp1[i + 1] + (1 - a1) * sp2[i + 1];    \
	dp[i + 2] = a2 * sp1[i + 2] + (1 - a2) * sp2[i + 2]

#define	MLIB_C_IMAGESCALARBLEND_S32_4(sp1, sp2, dp)             \
	dp[i] = a0 * sp1[i] + (1 - a0) * sp2[i];                \
	dp[i + 1] = a1 * sp1[i + 1] + (1 - a1) * sp2[i + 1];    \
	dp[i + 2] = a2 * sp1[i + 2] + (1 - a2) * sp2[i + 2];    \
	dp[i + 3] = a3 * sp1[i + 3] + (1 - a3) * sp2[i + 3]

/* *********************************************************** */

void
mlib_s_ImageScalarBlend_s32(
	mlib_s32 *dst,
	mlib_s32 dlb,
	const mlib_s32 *src1,
	mlib_s32 slb1,
	const mlib_s32 *src2,
	mlib_s32 slb2,
	const mlib_s32 *alpha,
	mlib_s32 xsize,
	mlib_s32 ysize,
	mlib_s32 nchan)
{
	mlib_s32 i, j, nsize;
	__m128i *srcPtr1, *srcPtr2, *dstPtr;
	mlib_s32 *dl = dst;
	mlib_s32 *sl1 = (mlib_s32 *)src1, *sl2 = (mlib_s32 *)src2;
	__m128d alphas0, alphas1, alphau0, alphau1, alphav0, alphav1;
	__m128d betas0, betas1, betau0, betau1, betav0, betav1;
	__m128d ones = _mm_set1_pd(1.0f);
	mlib_s32 res, sdata1, sdata2;
	mlib_d64 a0, a1, a2, a3;



	nsize = xsize * nchan;

	switch (nchan) {
	case 1:
		a0 = -(alpha[0] & MASK) / (mlib_d64)MLIB_S32_MIN;
		alphas0 = _mm_set1_pd(a0);
		alphas1 = _mm_set1_pd(a0);
		betas0  = _mm_sub_pd(ones, alphas0);
		betas1  = _mm_sub_pd(ones, alphas1);
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
				for (i = 0; i <= (nsize - 4); i += 4) {
					MLIB_S_IMAGESCALARBLEND_S32(
						_mm_store_si128,
						_mm_load_si128,
						_mm_load_si128);
				}
				for (; i < nsize; i++) {
					MLIB_C_IMAGESCALARBLEND_S32_1(
						sl1, sl2, dl);
				}
				sl1 = (mlib_s32 *)((mlib_u8 *)sl1 + slb1);
				sl2 = (mlib_s32 *)((mlib_u8 *)sl2 + slb2);
				dl = (mlib_s32 *)((mlib_u8 *)dl + dlb);
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
				for (i = 0; i <= (nsize - 4); i += 4) {
					MLIB_S_IMAGESCALARBLEND_S32(
						_mm_storeu_si128,
						_mm_load_si128,
						_mm_load_si128);
				}
				for (; i < nsize; i++) {
					MLIB_C_IMAGESCALARBLEND_S32_1(
						sl1, sl2, dl);
				}
				sl1 = (mlib_s32 *)((mlib_u8 *)sl1 + slb1);
				sl2 = (mlib_s32 *)((mlib_u8 *)sl2 + slb2);
				dl = (mlib_s32 *)((mlib_u8 *)dl + dlb);
			}
		} else {
			for (j = 0; j < ysize; j ++) {
				srcPtr1 = (__m128i *)sl1;
				srcPtr2 = (__m128i *)sl2;
				dstPtr = (__m128i *)dl;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i <= (nsize - 4); i += 4) {
					MLIB_S_IMAGESCALARBLEND_S32(
						_mm_storeu_si128,
						_mm_loadu_si128,
						_mm_loadu_si128);
				}
				for (; i < nsize; i++) {
					MLIB_C_IMAGESCALARBLEND_S32_1(
						sl1, sl2, dl);
				}
				sl1 = (mlib_s32 *)((mlib_u8 *)sl1 + slb1);
				sl2 = (mlib_s32 *)((mlib_u8 *)sl2 + slb2);
				dl = (mlib_s32 *)((mlib_u8 *)dl + dlb);
			}
		}
		break;
	case 2:
		a0 = -(alpha[0] & MASK) / (mlib_d64)MLIB_S32_MIN;
		a1 = -(alpha[1] & MASK) / (mlib_d64)MLIB_S32_MIN;
		alphas0 = _mm_set_pd(a1, a0);
		alphas1 = _mm_set_pd(a1, a0);
		betas0  = _mm_sub_pd(ones, alphas0);
		betas1  = _mm_sub_pd(ones, alphas1);

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
				for (i = 0; i <= (nsize - 4); i += 4) {
					MLIB_S_IMAGESCALARBLEND_S32(
						_mm_store_si128,
						_mm_load_si128,
						_mm_load_si128);
				}
				for (; i < nsize; i += 2) {
					MLIB_C_IMAGESCALARBLEND_S32_2(
						sl1, sl2, dl);
				}
				sl1 = (mlib_s32 *)((mlib_u8 *)sl1 + slb1);
				sl2 = (mlib_s32 *)((mlib_u8 *)sl2 + slb2);
				dl = (mlib_s32 *)((mlib_u8 *)dl + dlb);
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
				for (i = 0; i <= (nsize - 4); i += 4) {
					MLIB_S_IMAGESCALARBLEND_S32(
						_mm_storeu_si128,
						_mm_load_si128,
						_mm_load_si128);
				}
				for (; i < nsize; i += 2) {
					MLIB_C_IMAGESCALARBLEND_S32_2(
						sl1, sl2, dl);
				}
				sl1 = (mlib_s32 *)((mlib_u8 *)sl1 + slb1);
				sl2 = (mlib_s32 *)((mlib_u8 *)sl2 + slb2);
				dl = (mlib_s32 *)((mlib_u8 *)dl + dlb);
			}
		} else {
			for (j = 0; j < ysize; j ++) {
				srcPtr1 = (__m128i *)sl1;
				srcPtr2 = (__m128i *)sl2;
				dstPtr = (__m128i *)dl;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i <= (nsize - 4); i += 4) {
					MLIB_S_IMAGESCALARBLEND_S32(
						_mm_storeu_si128,
						_mm_loadu_si128,
						_mm_loadu_si128);
				}
				for (; i < nsize; i += 2) {
					MLIB_C_IMAGESCALARBLEND_S32_2(
						sl1, sl2, dl);
				}
				sl1 = (mlib_s32 *)((mlib_u8 *)sl1 + slb1);
				sl2 = (mlib_s32 *)((mlib_u8 *)sl2 + slb2);
				dl = (mlib_s32 *)((mlib_u8 *)dl + dlb);
			}
		}
		break;
	case 3:
		a0 = -(alpha[0] & MASK) / (mlib_d64)MLIB_S32_MIN;
		a1 = -(alpha[1] & MASK) / (mlib_d64)MLIB_S32_MIN;
		a2 = -(alpha[2] & MASK) / (mlib_d64)MLIB_S32_MIN;
		alphas0 = _mm_set_pd(a1, a0);
		alphas1 = _mm_set_pd(a0, a2);
		alphau0 = _mm_set_pd(a2, a1);
		alphau1 = _mm_set_pd(a1, a0);
		alphav0 = _mm_set_pd(a0, a2);
		alphav1 = _mm_set_pd(a2, a1);
		betas0  = _mm_sub_pd(ones, alphas0);
		betas1  = _mm_sub_pd(ones, alphas1);
		betau0  = _mm_sub_pd(ones, alphau0);
		betau1  = _mm_sub_pd(ones, alphau1);
		betav0  = _mm_sub_pd(ones, alphav0);
		betav1  = _mm_sub_pd(ones, alphav1);

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
				for (i = 0; i <= (nsize - 12); i += 12) {
					MLIB_S_IMAGESCALARBLEND3_S32(
						_mm_store_si128,
						_mm_load_si128,
						_mm_load_si128);
				}
				for (; i < nsize; i += 3) {
					MLIB_C_IMAGESCALARBLEND_S32_3(
						sl1, sl2, dl);
				}
				sl1 = (mlib_s32 *)((mlib_u8 *)sl1 + slb1);
				sl2 = (mlib_s32 *)((mlib_u8 *)sl2 + slb2);
				dl = (mlib_s32 *)((mlib_u8 *)dl + dlb);
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
				for (i = 0; i <= (nsize - 12); i += 12) {
					MLIB_S_IMAGESCALARBLEND3_S32(
						_mm_storeu_si128,
						_mm_load_si128,
						_mm_load_si128);
				}
				for (; i < nsize; i += 3) {
					MLIB_C_IMAGESCALARBLEND_S32_3(
						sl1, sl2, dl);
				}
				sl1 = (mlib_s32 *)((mlib_u8 *)sl1 + slb1);
				sl2 = (mlib_s32 *)((mlib_u8 *)sl2 + slb2);
				dl = (mlib_s32 *)((mlib_u8 *)dl + dlb);
			}
		} else {
			for (j = 0; j < ysize; j ++) {
				srcPtr1 = (__m128i *)sl1;
				srcPtr2 = (__m128i *)sl2;
				dstPtr = (__m128i *)dl;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i <= (nsize - 12); i += 12) {
					MLIB_S_IMAGESCALARBLEND3_S32(
						_mm_storeu_si128,
						_mm_loadu_si128,
						_mm_loadu_si128);
				}
				for (; i < nsize; i += 3) {
					MLIB_C_IMAGESCALARBLEND_S32_3(
						sl1, sl2, dl);
				}
				sl1 = (mlib_s32 *)((mlib_u8 *)sl1 + slb1);
				sl2 = (mlib_s32 *)((mlib_u8 *)sl2 + slb2);
				dl = (mlib_s32 *)((mlib_u8 *)dl + dlb);
			}
		}
		break;

	case 4:
		a0 = -(alpha[0] & MASK) / (mlib_d64)MLIB_S32_MIN;
		a1 = -(alpha[1] & MASK) / (mlib_d64)MLIB_S32_MIN;
		a2 = -(alpha[2] & MASK) / (mlib_d64)MLIB_S32_MIN;
		a3 = -(alpha[3] & MASK) / (mlib_d64)MLIB_S32_MIN;
		alphas0 = _mm_set_pd(a1, a0);
		alphas1 = _mm_set_pd(a3, a2);
		betas0  = _mm_sub_pd(ones, alphas0);
		betas1  = _mm_sub_pd(ones, alphas1);

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
				for (i = 0; i <= (nsize - 4); i += 4) {
					MLIB_S_IMAGESCALARBLEND_S32(
						_mm_store_si128,
						_mm_load_si128,
						_mm_load_si128);
				}
				sl1 = (mlib_s32 *)((mlib_u8 *)sl1 + slb1);
				sl2 = (mlib_s32 *)((mlib_u8 *)sl2 + slb2);
				dl = (mlib_s32 *)((mlib_u8 *)dl + dlb);
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
				for (i = 0; i <= (nsize - 4); i += 4) {
					MLIB_S_IMAGESCALARBLEND_S32(
						_mm_storeu_si128,
						_mm_load_si128,
						_mm_load_si128);
				}
				sl1 = (mlib_s32 *)((mlib_u8 *)sl1 + slb1);
				sl2 = (mlib_s32 *)((mlib_u8 *)sl2 + slb2);
				dl = (mlib_s32 *)((mlib_u8 *)dl + dlb);
			}
		} else {
			for (j = 0; j < ysize; j ++) {
				srcPtr1 = (__m128i *)sl1;
				srcPtr2 = (__m128i *)sl2;
				dstPtr = (__m128i *)dl;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i <= (nsize - 4); i += 4) {
					MLIB_S_IMAGESCALARBLEND_S32(
						_mm_storeu_si128,
						_mm_loadu_si128,
						_mm_loadu_si128);
				}
				sl1 = (mlib_s32 *)((mlib_u8 *)sl1 + slb1);
				sl2 = (mlib_s32 *)((mlib_u8 *)sl2 + slb2);
				dl = (mlib_s32 *)((mlib_u8 *)dl + dlb);
			}
		}
		break;
	}
}

/* *********************************************************** */
