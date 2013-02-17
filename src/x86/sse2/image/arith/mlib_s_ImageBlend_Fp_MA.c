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

#pragma ident "@(#)mlib_s_ImageBlend_Fp_MA.c	9.3	07/11/05 SMI"

/*
 * FUNCTION
 *	mlib_s_ImageBlend_Fp_f32	- blend two images
 *	mlib_s_ImageBlend_Fp_d64	- blend two images
 *
 * SYNOPSIS
 *	mlib_s_ImageBlend_Fp_f32(mlib_f32 *dst,
 *				mlib_s32 dlb,
 *				const mlib_f32 *src1,
 *				mlib_s32 slb1,
 *				const mlib_f32 *src2,
 *				mlib_s32 slb2,
 *				const mlib_f32 *alpha,
 *				mlib_s32 alb,
 *				mlib_s32 xsize,
 *				mlib_s32 ysize);
 *	mlib_s_ImageBlend_Fp_d64(mlib_d64 *dst,
 *				mlib_s32 dlb,
 *				const mlib_d64 *src1,
 *				mlib_s32 slb1,
 *				const mlib_d64 *src2,
 *				mlib_s32 slb2,
 *				const mlib_d64 *alpha,
 *				mlib_s32 alb,
 *				mlib_s32 xsize,
 *				mlib_s32 ysize);
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
 *
 * DESCRIPTION
 *	dst[x][y][c] = alpha[x][y] * src1[x][y] +
 *	               (1 - alpha[x][y]) * src2[x][y]
 */

#ifdef __SUNPRO_C
#include <sunmedia_intrin.h>
#else
#include <emmintrin.h>
#endif

#include <mlib_image.h>
#include "mlib_s_ImageBlend_Fp.h"

/* *********************************************************** */

#define	MLIB_S_IMAGEBLEND_F32(D_STORE, S1_LOAD, S2_LOAD, A_LOAD)\
	{							\
		__m128 dsts, srcs1, srcs2, alphas;		\
		__m128 betas, part1, part2;			\
		srcs1  = S1_LOAD((void *)&(src1[i]));		\
		srcs2  = S2_LOAD((void *)&(src2[i]));		\
		alphas = A_LOAD((void *)&(alpha[i]));		\
		betas  = _mm_sub_ps(ones, alphas);		\
		part1  = _mm_mul_ps(alphas, srcs1);		\
		part2  = _mm_mul_ps(betas, srcs2);		\
		dsts   = _mm_add_ps(part1, part2);		\
		D_STORE((void *)&(dst[i]), dsts);		\
	}

#define	MLIB_S_IMAGEBLEND_D64(D_STORE, S1_LOAD, S2_LOAD, A_LOAD)\
	{							\
		__m128d dsts, srcs1, srcs2, alphas;		\
		__m128d betas, part1, part2;			\
		srcs1  = S1_LOAD((void *)&(src1[i]));		\
		srcs2  = S2_LOAD((void *)&(src2[i]));		\
		alphas = A_LOAD((void *)&(alpha[i]));		\
		betas  = _mm_sub_pd(ones, alphas);		\
		part1  = _mm_mul_pd(alphas, srcs1);		\
		part2  = _mm_mul_pd(betas, srcs2);		\
		dsts   = _mm_add_pd(part1, part2);		\
		D_STORE((void *)&(dst[i]), dsts);		\
	}

/* *********************************************************** */

void
mlib_s_ImageBlend_Fp_f32(
	mlib_f32 *dst,
	mlib_s32 dlb,
	const mlib_f32 *src1,
	mlib_s32 slb1,
	const mlib_f32 *src2,
	mlib_s32 slb2,
	const mlib_f32 *alpha,
	mlib_s32 alb,
	mlib_s32 xsize,
	mlib_s32 ysize)
{
	mlib_s32 i, j;
	__m128 ones;

	ones = _mm_set1_ps(1.0f);
	if ((((mlib_addr)dst | dlb |
		(mlib_addr)src1 | slb1 |
		(mlib_addr)src2 | slb2 |
		(mlib_addr)alpha | alb) & 0xf) == 0) {
		for (j = 0; j < ysize; j ++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i <= (xsize - 4); i += 4) {
				MLIB_S_IMAGEBLEND_F32(
					_mm_store_ps,
					_mm_load_ps,
					_mm_load_ps,
					_mm_load_ps);
			}
			for (; i < xsize; i ++) {
				dst[i] =  MLIB_C_IMAGEBLEND(
						src1[i], src2[i], alpha[i]);
			}
			dst   = (mlib_f32 *)((mlib_u8 *)dst + dlb);
			src1  = (mlib_f32 *)((mlib_u8 *)src1 + slb1);
			src2  = (mlib_f32 *)((mlib_u8 *)src2 + slb2);
			alpha = (mlib_f32 *)((mlib_u8 *)alpha + alb);
		}
	} else
	if ((((mlib_addr)src1 | slb1 |
		(mlib_addr)src2 | slb2 |
		(mlib_addr)alpha | alb) & 0xf) == 0) {
		for (j = 0; j < ysize; j ++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i <= (xsize - 4); i += 4) {
				MLIB_S_IMAGEBLEND_F32(
					_mm_storeu_ps,
					_mm_load_ps,
					_mm_load_ps,
					_mm_load_ps);
			}
			for (; i < xsize; i ++) {
				dst[i] =  MLIB_C_IMAGEBLEND(
						src1[i], src2[i], alpha[i]);
			}
			dst   = (mlib_f32 *)((mlib_u8 *)dst + dlb);
			src1  = (mlib_f32 *)((mlib_u8 *)src1 + slb1);
			src2  = (mlib_f32 *)((mlib_u8 *)src2 + slb2);
			alpha = (mlib_f32 *)((mlib_u8 *)alpha + alb);
		}
	} else {
		for (j = 0; j < ysize; j ++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i <= (xsize - 4); i += 4) {
				MLIB_S_IMAGEBLEND_F32(
					_mm_storeu_ps,
					_mm_loadu_ps,
					_mm_loadu_ps,
					_mm_loadu_ps);
			}
			for (; i < xsize; i ++) {
				dst[i] =  MLIB_C_IMAGEBLEND(
						src1[i], src2[i], alpha[i]);
			}
			dst   = (mlib_f32 *)((mlib_u8 *)dst + dlb);
			src1  = (mlib_f32 *)((mlib_u8 *)src1 + slb1);
			src2  = (mlib_f32 *)((mlib_u8 *)src2 + slb2);
			alpha = (mlib_f32 *)((mlib_u8 *)alpha + alb);
		}
	}
}

/* *********************************************************** */

void
mlib_s_ImageBlend_Fp_d64(
	mlib_d64 *dst,
	mlib_s32 dlb,
	const mlib_d64 *src1,
	mlib_s32 slb1,
	const mlib_d64 *src2,
	mlib_s32 slb2,
	const mlib_d64 *alpha,
	mlib_s32 alb,
	mlib_s32 xsize,
	mlib_s32 ysize)
{
	mlib_s32 i, j;
	__m128d ones;

	ones = _mm_set1_pd(1.0);
	if ((((mlib_addr)dst | dlb |
		(mlib_addr)src1 | slb1 |
		(mlib_addr)src2 | slb2 |
		(mlib_addr)alpha | alb) & 0xf) == 0) {
		for (j = 0; j < ysize; j ++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i <= (xsize - 2); i += 2) {
				MLIB_S_IMAGEBLEND_D64(
					_mm_store_pd,
					_mm_load_pd,
					_mm_load_pd,
					_mm_load_pd);
			}
			for (; i < xsize; i ++) {
				dst[i] =  MLIB_C_IMAGEBLEND(
						src1[i], src2[i], alpha[i]);
			}
			dst   = (mlib_d64 *)((mlib_u8 *)dst + dlb);
			src1  = (mlib_d64 *)((mlib_u8 *)src1 + slb1);
			src2  = (mlib_d64 *)((mlib_u8 *)src2 + slb2);
			alpha = (mlib_d64 *)((mlib_u8 *)alpha + alb);
		}
	} else
	if ((((mlib_addr)src1 | slb1 |
		(mlib_addr)src2 | slb2 |
		(mlib_addr)alpha | alb) & 0xf) == 0) {
		for (j = 0; j < ysize; j ++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i <= (xsize - 2); i += 2) {
				MLIB_S_IMAGEBLEND_D64(
					_mm_storeu_pd,
					_mm_load_pd,
					_mm_load_pd,
					_mm_load_pd);
			}
			for (; i < xsize; i ++) {
				dst[i] =  MLIB_C_IMAGEBLEND(
						src1[i], src2[i], alpha[i]);
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
				MLIB_S_IMAGEBLEND_D64(
					_mm_storeu_pd,
					_mm_loadu_pd,
					_mm_loadu_pd,
					_mm_loadu_pd);
			}
			for (; i < xsize; i ++) {
				dst[i] =  MLIB_C_IMAGEBLEND(
						src1[i], src2[i], alpha[i]);
			}
			dst   = (mlib_d64 *)((mlib_u8 *)dst + dlb);
			src1  = (mlib_d64 *)((mlib_u8 *)src1 + slb1);
			src2  = (mlib_d64 *)((mlib_u8 *)src2 + slb2);
			alpha = (mlib_d64 *)((mlib_u8 *)alpha + alb);
		}
	}
}

/* *********************************************************** */
