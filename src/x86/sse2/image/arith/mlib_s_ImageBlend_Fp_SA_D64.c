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

#pragma ident "@(#)mlib_s_ImageBlend_Fp_SA_D64.c	9.3	07/11/05 SMI"

/*
 * FUNCTION
 *	mlib_s_ImageBlendSA_Fp_d64	- blend two images
 *
 * SYNOPSIS
 *	mlib_s_ImageBlendSA_Fp_d64(mlib_d64 *dst,
 *				mlib_s32 dlb,
 *				const mlib_d64 *src1,
 *				mlib_s32 slb1,
 *				const mlib_d64 *src2,
 *				mlib_s32 slb2,
 *				const mlib_d64 *alpha,
 *				mlib_s32 alb,
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
 *      alpha	pointer to the alpha image data
 *      alb	line stride of the alpha image data in bytes
 *      xsize	image width in pixels
 *      ysize	image height in pixels
 *      nchan	number of channels in image
 *
 * DESCRIPTION
 *	dst[x][y][c] = alpha[x][y] * src1[x][y][c] +
 *	               (1 - alpha[x][y]) * src2[x][y][c]
 */

#ifdef __SUNPRO_C
#include <sunmedia_intrin.h>
#else
#include <emmintrin.h>
#endif

#include <mlib_image.h>
#include "mlib_s_ImageBlend_Fp.h"

/* *********************************************************** */

#define	MLIB_S_IMAGEBLENDSA2_D64(D_STORE, S1_LOAD, S2_LOAD)	\
	{							\
		__m128d dsts, srcs1, srcs2;			\
		__m128d alphas, betas, part1, part2;		\
		alphas = _mm_set1_pd(alpha[i]);			\
		srcs1  = S1_LOAD((void *)&(src1[k]));		\
		srcs2  = S2_LOAD((void *)&(src2[k]));		\
		betas  = _mm_sub_pd(ones, alphas);		\
		part1  = _mm_mul_pd(alphas, srcs1);		\
		part2  = _mm_mul_pd(betas, srcs2);		\
		dsts   = _mm_add_pd(part1, part2);		\
		D_STORE((void *)&(dst[k]), dsts);		\
	}

#define	MLIB_S_IMAGEBLENDSA3_D64(D_STORE, S1_LOAD, S2_LOAD)	\
	{							\
		__m128d dsts, srcs1, srcs2;			\
		__m128d alphas, alphau, alphav;			\
		__m128d betas, part1, part2;			\
		mlib_d64 alpha0, alpha1;			\
		alpha0 = alpha[i];				\
		alpha1 = alpha[i + 1];				\
		alphas = _mm_set_pd(alpha0, alpha0);		\
		alphau = _mm_set_pd(alpha1, alpha0);		\
		alphav = _mm_set_pd(alpha1, alpha1);		\
		srcs1  = S1_LOAD((void *)&(src1[k]));		\
		srcs2  = S2_LOAD((void *)&(src2[k]));		\
		betas  = _mm_sub_pd(ones, alphas);		\
		part1  = _mm_mul_pd(alphas, srcs1);		\
		part2  = _mm_mul_pd(betas, srcs2);		\
		dsts   = _mm_add_pd(part1, part2);		\
		D_STORE((void *)&(dst[k]), dsts);		\
		srcs1  = S1_LOAD((void *)&(src1[k + 2]));	\
		srcs2  = S2_LOAD((void *)&(src2[k + 2]));	\
		betas  = _mm_sub_pd(ones, alphau);		\
		part1  = _mm_mul_pd(alphau, srcs1);		\
		part2  = _mm_mul_pd(betas, srcs2);		\
		dsts   = _mm_add_pd(part1, part2);		\
		D_STORE((void *)&(dst[k + 2]), dsts);		\
		srcs1  = S1_LOAD((void *)&(src1[k + 4]));	\
		srcs2  = S2_LOAD((void *)&(src2[k + 4]));	\
		betas  = _mm_sub_pd(ones, alphav);		\
		part1  = _mm_mul_pd(alphav, srcs1);		\
		part2  = _mm_mul_pd(betas, srcs2);		\
		dsts   = _mm_add_pd(part1, part2);		\
		D_STORE((void *)&(dst[k + 4]), dsts);		\
	}

#define	MLIB_S_IMAGEBLENDSA4_D64(D_STORE, S1_LOAD, S2_LOAD)	\
	{							\
		__m128d dsts, srcs1, srcs2, alphas;		\
		__m128d betas, part1, part2;			\
		alphas = _mm_set1_pd(alpha[i]);			\
		srcs1  = S1_LOAD((void *)&(src1[k]));		\
		srcs2  = S2_LOAD((void *)&(src2[k]));		\
		betas  = _mm_sub_pd(ones, alphas);		\
		part1  = _mm_mul_pd(alphas, srcs1);		\
		part2  = _mm_mul_pd(betas, srcs2);		\
		dsts   = _mm_add_pd(part1, part2);		\
		D_STORE((void *)&(dst[k]), dsts);		\
		srcs1  = S1_LOAD((void *)&(src1[k + 2]));	\
		srcs2  = S2_LOAD((void *)&(src2[k + 2]));	\
		betas  = _mm_sub_pd(ones, alphas);		\
		part1  = _mm_mul_pd(alphas, srcs1);		\
		part2  = _mm_mul_pd(betas, srcs2);		\
		dsts   = _mm_add_pd(part1, part2);		\
		D_STORE((void *)&(dst[k + 2]), dsts);		\
	}

/* *********************************************************** */

void
mlib_s_ImageBlendSA_Fp_d64(
	mlib_d64 *dst,
	mlib_s32 dlb,
	const mlib_d64 *src1,
	mlib_s32 slb1,
	const mlib_d64 *src2,
	mlib_s32 slb2,
	const mlib_d64 *alpha,
	mlib_s32 alb,
	mlib_s32 xsize,
	mlib_s32 ysize,
	mlib_s32 nchan)
{
	mlib_s32 i, j;
	__m128d ones;

	ones = _mm_set1_pd(1.0);
	switch (nchan) {
	case 2:
		if ((((mlib_addr)dst | dlb |
			(mlib_addr)src1 | slb1 |
			(mlib_addr)src2 | slb2) & 0xf) == 0) {
			for (j = 0; j < ysize; j ++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i < xsize; i ++) {
					int k = 2 * i;
					MLIB_S_IMAGEBLENDSA2_D64(
						_mm_store_pd,
						_mm_load_pd,
						_mm_load_pd);
				}
				dst   = (mlib_d64 *)((mlib_u8 *)dst + dlb);
				src1  = (mlib_d64 *)((mlib_u8 *)src1 + slb1);
				src2  = (mlib_d64 *)((mlib_u8 *)src2 + slb2);
				alpha = (mlib_d64 *)((mlib_u8 *)alpha + alb);
			}
		} else
		if ((((mlib_addr)src1 | slb1 |
			(mlib_addr)src2 | slb2) & 0xf) == 0) {
			for (j = 0; j < ysize; j ++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i < xsize; i ++) {
					int k = 2 * i;
					MLIB_S_IMAGEBLENDSA2_D64(
						_mm_storeu_pd,
						_mm_load_pd,
						_mm_load_pd);
				}
				dst   = (mlib_d64 *)((mlib_u8 *)dst + dlb);
				src1  = (mlib_d64 *)((mlib_u8 *)src1 + slb1);
				src2  = (mlib_d64 *)((mlib_u8 *)src2 + slb2);
				alpha = (mlib_d64 *)((mlib_u8 *)alpha + alb);
			}
		} else {
			for (j = 0; j < ysize; j ++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i < xsize; i ++) {
					int k = 2 * i;
					MLIB_S_IMAGEBLENDSA2_D64(
						_mm_storeu_pd,
						_mm_loadu_pd,
						_mm_loadu_pd);
				}
				dst   = (mlib_d64 *)((mlib_u8 *)dst + dlb);
				src1  = (mlib_d64 *)((mlib_u8 *)src1 + slb1);
				src2  = (mlib_d64 *)((mlib_u8 *)src2 + slb2);
				alpha = (mlib_d64 *)((mlib_u8 *)alpha + alb);
			}
		}
		break;
	case 3:
		if ((((mlib_addr)dst | dlb |
			(mlib_addr)src1 | slb1 |
			(mlib_addr)src2 | slb2) & 0xf) == 0) {
			for (j = 0; j < ysize; j ++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i <= (xsize - 2); i += 2) {
					int k = 3 * i;
					MLIB_S_IMAGEBLENDSA3_D64(
						_mm_store_pd,
						_mm_load_pd,
						_mm_load_pd);
				}
				for (; i < xsize; i ++) {
					int k = 3 * i;
					dst[k] =  MLIB_C_IMAGEBLEND(
							src1[k],
							src2[k],
							alpha[i]);
					dst[k + 1] =  MLIB_C_IMAGEBLEND(
							src1[k + 1],
							src2[k + 1],
							alpha[i]);
					dst[k + 2] =  MLIB_C_IMAGEBLEND(
							src1[k + 2],
							src2[k + 2],
							alpha[i]);
				}
				dst   = (mlib_d64 *)((mlib_u8 *)dst + dlb);
				src1  = (mlib_d64 *)((mlib_u8 *)src1 + slb1);
				src2  = (mlib_d64 *)((mlib_u8 *)src2 + slb2);
				alpha = (mlib_d64 *)((mlib_u8 *)alpha + alb);
			}
		} else
		if ((((mlib_addr)src1 | slb1 |
			(mlib_addr)src2 | slb2) & 0xf) == 0) {
			for (j = 0; j < ysize; j ++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i <= (xsize - 2); i += 2) {
					int k = 3 * i;
					MLIB_S_IMAGEBLENDSA3_D64(
						_mm_storeu_pd,
						_mm_load_pd,
						_mm_load_pd);
				}
				for (; i < xsize; i ++) {
					int k = 3 * i;
					dst[k] =  MLIB_C_IMAGEBLEND(
							src1[k],
							src2[k],
							alpha[i]);
					dst[k + 1] =  MLIB_C_IMAGEBLEND(
							src1[k + 1],
							src2[k + 1],
							alpha[i]);
					dst[k + 2] =  MLIB_C_IMAGEBLEND(
							src1[k + 2],
							src2[k + 2],
							alpha[i]);
				}
				dst   = (mlib_d64 *)((mlib_u8 *)dst + dlb);
				src1  = (mlib_d64 *)((mlib_u8 *)src1 + slb1);
				src2  = (mlib_d64 *)((mlib_u8 *)src2 + slb2);
				alpha = (mlib_d64 *)((mlib_u8 *)alpha + alb);
			}
		} else {
			for (j = 0; j < ysize; j ++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i <= (xsize - 2); i += 2) {
					int k = 3 * i;
					MLIB_S_IMAGEBLENDSA3_D64(
						_mm_storeu_pd,
						_mm_loadu_pd,
						_mm_loadu_pd);
				}
				for (; i < xsize; i ++) {
					int k = 3 * i;
					dst[k] =  MLIB_C_IMAGEBLEND(
							src1[k],
							src2[k],
							alpha[i]);
					dst[k + 1] =  MLIB_C_IMAGEBLEND(
							src1[k + 1],
							src2[k + 1],
							alpha[i]);
					dst[k + 2] =  MLIB_C_IMAGEBLEND(
							src1[k + 2],
							src2[k + 2],
							alpha[i]);
				}
				dst   = (mlib_d64 *)((mlib_u8 *)dst + dlb);
				src1  = (mlib_d64 *)((mlib_u8 *)src1 + slb1);
				src2  = (mlib_d64 *)((mlib_u8 *)src2 + slb2);
				alpha = (mlib_d64 *)((mlib_u8 *)alpha + alb);
			}
		}
		break;
	case 4:
		if ((((mlib_addr)dst | dlb |
			(mlib_addr)src1 | slb1 |
			(mlib_addr)src2 | slb2) & 0xf) == 0) {
			for (j = 0; j < ysize; j ++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i < xsize; i ++) {
					int k = 4 * i;
					MLIB_S_IMAGEBLENDSA4_D64(
						_mm_store_pd,
						_mm_load_pd,
						_mm_load_pd);
				}
				dst   = (mlib_d64 *)((mlib_u8 *)dst + dlb);
				src1  = (mlib_d64 *)((mlib_u8 *)src1 + slb1);
				src2  = (mlib_d64 *)((mlib_u8 *)src2 + slb2);
				alpha = (mlib_d64 *)((mlib_u8 *)alpha + alb);
			}
		} else
		if ((((mlib_addr)src1 | slb1 |
			(mlib_addr)src2 | slb2) & 0xf) == 0) {
			for (j = 0; j < ysize; j ++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i < xsize; i ++) {
					int k = 4 * i;
					MLIB_S_IMAGEBLENDSA4_D64(
						_mm_storeu_pd,
						_mm_load_pd,
						_mm_load_pd);
				}
				dst   = (mlib_d64 *)((mlib_u8 *)dst + dlb);
				src1  = (mlib_d64 *)((mlib_u8 *)src1 + slb1);
				src2  = (mlib_d64 *)((mlib_u8 *)src2 + slb2);
				alpha = (mlib_d64 *)((mlib_u8 *)alpha + alb);
			}
		} else {
			for (j = 0; j < ysize; j ++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i < xsize; i ++) {
					int k = 4 * i;
					MLIB_S_IMAGEBLENDSA4_D64(
						_mm_storeu_pd,
						_mm_loadu_pd,
						_mm_loadu_pd);
				}
				dst   = (mlib_d64 *)((mlib_u8 *)dst + dlb);
				src1  = (mlib_d64 *)((mlib_u8 *)src1 + slb1);
				src2  = (mlib_d64 *)((mlib_u8 *)src2 + slb2);
				alpha = (mlib_d64 *)((mlib_u8 *)alpha + alb);
			}
		}
		break;
	}
}

/* *********************************************************** */
