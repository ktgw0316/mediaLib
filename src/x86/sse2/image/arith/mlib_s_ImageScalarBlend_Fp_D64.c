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

#pragma ident "@(#)mlib_s_ImageScalarBlend_Fp_D64.c	9.4	07/11/05 SMI"

/*
 * FUNCTION
 *	mlib_s_ImageScalarBlend_Fp_d64	- blend two images
 *
 * SYNOPSIS
 *	mlib_s_ImageScalarBlend_Fp_d64(mlib_d64 *dst,
 *				mlib_s32 dlb,
 *				const mlib_d64 *src1,
 *				mlib_s32 slb1,
 *				const mlib_d64 *src2,
 *				mlib_s32 slb2,
 *				const mlib_d64 *alpha,
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
#include "mlib_s_ImageScalarBlend_Fp.h"

/* *********************************************************** */

#define	MLIB_S_IMAGESCALARBLEND_D64(D_STORE, S1_LOAD, S2_LOAD)  \
	{                                                       \
		__m128d dsts, srcs1, srcs2;                     \
		__m128d part1, part2;                           \
		srcs1  = S1_LOAD((void *)srcPtr1);		\
		srcs2  = S2_LOAD((void *)srcPtr2);		\
		part1  = _mm_mul_pd(alphas, srcs1);		\
		part2  = _mm_mul_pd(betas, srcs2);		\
		dsts   = _mm_add_pd(part1, part2);		\
		D_STORE((void *)dstPtr, dsts);		        \
		srcPtr1 += 2;                                   \
		srcPtr2 += 2;                                   \
		dstPtr += 2;                                    \
	}

#define	MLIB_S_IMAGESCALARBLEND3_D64(D_STORE, S1_LOAD, S2_LOAD) \
	{                                                       \
		__m128d dsts, srcs1, srcs2;                     \
		__m128d part1, part2;                           \
		srcs1  = S1_LOAD((void *)srcPtr1);		\
		srcs2  = S2_LOAD((void *)srcPtr2);		\
		part1  = _mm_mul_pd(alphas, srcs1);		\
		part2  = _mm_mul_pd(betas, srcs2);		\
		dsts   = _mm_add_pd(part1, part2);		\
		D_STORE((void *)dstPtr, dsts);		        \
		srcs1  = S1_LOAD((void *)(srcPtr1 + 2));	\
		srcs2  = S2_LOAD((void *)(srcPtr2 + 2));	\
		part1  = _mm_mul_pd(alphau, srcs1);		\
		part2  = _mm_mul_pd(betau, srcs2);		\
		dsts   = _mm_add_pd(part1, part2);		\
		D_STORE((void *)(dstPtr + 2), dsts);		\
		srcs1  = S1_LOAD((void *)(srcPtr1 + 4));	\
		srcs2  = S2_LOAD((void *)(srcPtr2 + 4));	\
		part1  = _mm_mul_pd(alphav, srcs1);		\
		part2  = _mm_mul_pd(betav, srcs2);		\
		dsts   = _mm_add_pd(part1, part2);		\
		D_STORE((void *)(dstPtr + 4), dsts);		\
		srcPtr1 += 6;                                   \
		srcPtr2 += 6;                                   \
		dstPtr += 6;                                    \
	}

#define	MLIB_S_IMAGESCALARBLEND4_D64(D_STORE, S1_LOAD, S2_LOAD) \
	{                                                       \
		__m128d dsts, srcs1, srcs2;                     \
		__m128d part1, part2;                           \
		srcs1  = S1_LOAD((void *)srcPtr1);		\
		srcs2  = S2_LOAD((void *)srcPtr2);		\
		part1  = _mm_mul_pd(alphas, srcs1);		\
		part2  = _mm_mul_pd(betas, srcs2);		\
		dsts   = _mm_add_pd(part1, part2);		\
		D_STORE((void *)dstPtr, dsts);		        \
		srcs1  = S1_LOAD((void *)(srcPtr1 + 2));	\
		srcs2  = S2_LOAD((void *)(srcPtr2 + 2));	\
		part1  = _mm_mul_pd(alphau, srcs1);		\
		part2  = _mm_mul_pd(betau, srcs2);		\
		dsts   = _mm_add_pd(part1, part2);		\
		D_STORE((void *)(dstPtr + 2), dsts);		\
		srcPtr1 += 4;                                   \
		srcPtr2 += 4;                                   \
		dstPtr += 4;                                    \
	}

/* *********************************************************** */

void
mlib_s_ImageScalarBlend_Fp_d64(
	mlib_d64 *dst,
	mlib_s32 dlb,
	const mlib_d64 *src1,
	mlib_s32 slb1,
	const mlib_d64 *src2,
	mlib_s32 slb2,
	const mlib_d64 *alpha,
	mlib_s32 xsize,
	mlib_s32 ysize,
	mlib_s32 nchan)
{
	mlib_s32 i, j, nsize, odd;
	mlib_d64 alpha0, alpha1, alpha2, alpha3;
	mlib_d64 *srcPtr1, *srcPtr2, *dstPtr;
	mlib_d64 *dl = dst;
	mlib_d64 *sl1 = (mlib_d64 *)src1, *sl2 = (mlib_d64 *)src2;
	__m128d alphas, alphau, alphav;
	__m128d betas, betau, betav;
	__m128d ones;

	odd = (xsize & 1);
	ones = _mm_set1_pd(1.0f);
	nsize = xsize * nchan;
	switch (nchan) {
	case 1:
		alpha0 = alpha[0];
		alphas = _mm_set1_pd(alpha0);
		betas  = _mm_sub_pd(ones, alphas);
		if ((((mlib_addr)dst | dlb |
			(mlib_addr)src1 | slb1 |
			(mlib_addr)src2 | slb2) & 0xf) == 0) {
			for (j = 0; j < ysize; j ++) {
				srcPtr1 = sl1;
				srcPtr2 = sl2;
				dstPtr = dl;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i <= (nsize - 2); i += 2) {
					MLIB_S_IMAGESCALARBLEND_D64(
						_mm_store_pd,
						_mm_load_pd,
						_mm_load_pd);
				}
				if (odd) {
					dl[i] = MLIB_C_IMAGESCALARBLEND(
							sl1[i],
							sl2[i],
							alpha0);
				}
				sl1 = (mlib_d64 *)((mlib_u8 *)sl1 + slb1);
				sl2 = (mlib_d64 *)((mlib_u8 *)sl2 + slb2);
				dl = (mlib_d64 *)((mlib_u8 *)dl + dlb);
			}
		} else
		if ((((mlib_addr)src1 | slb1 |
			(mlib_addr)src2 | slb2) & 0xf) == 0) {
			for (j = 0; j < ysize; j ++) {
				srcPtr1 = sl1;
				srcPtr2 = sl2;
				dstPtr = dl;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i <= (nsize - 2); i += 2) {
					MLIB_S_IMAGESCALARBLEND_D64(
						_mm_storeu_pd,
						_mm_load_pd,
						_mm_load_pd);
				}
				if (odd) {
					dl[i] = MLIB_C_IMAGESCALARBLEND(
							sl1[i],
							sl2[i],
							alpha0);
				}
				sl1 = (mlib_d64 *)((mlib_u8 *)sl1 + slb1);
				sl2 = (mlib_d64 *)((mlib_u8 *)sl2 + slb2);
				dl = (mlib_d64 *)((mlib_u8 *)dl + dlb);
			}
		} else {
			for (j = 0; j < ysize; j ++) {
				srcPtr1 = sl1;
				srcPtr2 = sl2;
				dstPtr = dl;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i <= (nsize - 2); i += 2) {
					MLIB_S_IMAGESCALARBLEND_D64(
						_mm_storeu_pd,
						_mm_loadu_pd,
						_mm_loadu_pd);
				}
				if (odd) {
					dl[i] = MLIB_C_IMAGESCALARBLEND(
							sl1[i],
							sl2[i],
							alpha0);
				}
				sl1 = (mlib_d64 *)((mlib_u8 *)sl1 + slb1);
				sl2 = (mlib_d64 *)((mlib_u8 *)sl2 + slb2);
				dl = (mlib_d64 *)((mlib_u8 *)dl + dlb);
			}
		}
		break;
	case 2:
		alpha0 = alpha[0];
		alpha1 = alpha[1];
		alphas = _mm_set_pd(alpha1, alpha0);
		betas  = _mm_sub_pd(ones, alphas);
		if ((((mlib_addr)dst | dlb |
			(mlib_addr)src1 | slb1 |
			(mlib_addr)src2 | slb2) & 0xf) == 0) {
			for (j = 0; j < ysize; j ++) {
				srcPtr1 = sl1;
				srcPtr2 = sl2;
				dstPtr = dl;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i <= (nsize - 2); i += 2) {
					MLIB_S_IMAGESCALARBLEND_D64(
						_mm_store_pd,
						_mm_load_pd,
						_mm_load_pd);
				}
				sl1 = (mlib_d64 *)((mlib_u8 *)sl1 + slb1);
				sl2 = (mlib_d64 *)((mlib_u8 *)sl2 + slb2);
				dl = (mlib_d64 *)((mlib_u8 *)dl + dlb);
			}
		} else
		if ((((mlib_addr)src1 | slb1 |
			(mlib_addr)src2 | slb2) & 0xf) == 0) {
			for (j = 0; j < ysize; j ++) {
				srcPtr1 = sl1;
				srcPtr2 = sl2;
				dstPtr = dl;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i <= (nsize - 2); i += 2) {
					MLIB_S_IMAGESCALARBLEND_D64(
						_mm_storeu_pd,
						_mm_load_pd,
						_mm_load_pd);
				}
				sl1 = (mlib_d64 *)((mlib_u8 *)sl1 + slb1);
				sl2 = (mlib_d64 *)((mlib_u8 *)sl2 + slb2);
				dl = (mlib_d64 *)((mlib_u8 *)dl + dlb);
			}
		} else {
			for (j = 0; j < ysize; j ++) {
				srcPtr1 = sl1;
				srcPtr2 = sl2;
				dstPtr = dl;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i <= (nsize - 2); i += 2) {
					MLIB_S_IMAGESCALARBLEND_D64(
						_mm_storeu_pd,
						_mm_loadu_pd,
						_mm_loadu_pd);
				}
				sl1 = (mlib_d64 *)((mlib_u8 *)sl1 + slb1);
				sl2 = (mlib_d64 *)((mlib_u8 *)sl2 + slb2);
				dl = (mlib_d64 *)((mlib_u8 *)dl + dlb);
			}
		}
		break;
	case 3:
		alpha0 = alpha[0];
		alpha1 = alpha[1];
		alpha2 = alpha[2];
		alphas = _mm_set_pd(alpha1, alpha0);
		alphau = _mm_set_pd(alpha0, alpha2);
		alphav = _mm_set_pd(alpha2, alpha1);
		betas  = _mm_sub_pd(ones, alphas);
		betau  = _mm_sub_pd(ones, alphau);
		betav  = _mm_sub_pd(ones, alphav);
		if ((((mlib_addr)dst | dlb |
			(mlib_addr)src1 | slb1 |
			(mlib_addr)src2 | slb2) & 0xf) == 0) {
			for (j = 0; j < ysize; j ++) {
				srcPtr1 = sl1;
				srcPtr2 = sl2;
				dstPtr = dl;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i <= (nsize - 6); i += 6) {
					MLIB_S_IMAGESCALARBLEND3_D64(
						_mm_store_pd,
						_mm_load_pd,
						_mm_load_pd);
				}
				if (odd) {
					dl[i] = MLIB_C_IMAGESCALARBLEND(
							sl1[i],
							sl2[i],
							alpha0);
					dl[i + 1] = MLIB_C_IMAGESCALARBLEND(
							sl1[i + 1],
							sl2[i + 1],
							alpha1);
					dl[i + 2] = MLIB_C_IMAGESCALARBLEND(
							sl1[i + 2],
							sl2[i + 2],
							alpha2);
				}
				sl1 = (mlib_d64 *)((mlib_u8 *)sl1 + slb1);
				sl2 = (mlib_d64 *)((mlib_u8 *)sl2 + slb2);
				dl = (mlib_d64 *)((mlib_u8 *)dl + dlb);
			}
		} else
		if ((((mlib_addr)src1 | slb1 |
			(mlib_addr)src2 | slb2) & 0xf) == 0) {
			for (j = 0; j < ysize; j ++) {
				srcPtr1 = sl1;
				srcPtr2 = sl2;
				dstPtr = dl;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i <= (nsize - 6); i += 6) {
					MLIB_S_IMAGESCALARBLEND3_D64(
						_mm_storeu_pd,
						_mm_load_pd,
						_mm_load_pd);
				}
				if (odd) {
					dl[i] = MLIB_C_IMAGESCALARBLEND(
							sl1[i],
							sl2[i],
							alpha0);
					dl[i + 1] = MLIB_C_IMAGESCALARBLEND(
							sl1[i + 1],
							sl2[i + 1],
							alpha1);
					dl[i + 2] = MLIB_C_IMAGESCALARBLEND(
							sl1[i + 2],
							sl2[i + 2],
							alpha2);
				}
				sl1 = (mlib_d64 *)((mlib_u8 *)sl1 + slb1);
				sl2 = (mlib_d64 *)((mlib_u8 *)sl2 + slb2);
				dl = (mlib_d64 *)((mlib_u8 *)dl + dlb);
			}
		} else {
			for (j = 0; j < ysize; j ++) {
				srcPtr1 = sl1;
				srcPtr2 = sl2;
				dstPtr = dl;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i <= (nsize - 6); i += 6) {
					MLIB_S_IMAGESCALARBLEND3_D64(
						_mm_storeu_pd,
						_mm_loadu_pd,
						_mm_loadu_pd);
				}
				if (odd) {
					dl[i] = MLIB_C_IMAGESCALARBLEND(
							sl1[i],
							sl2[i],
							alpha0);
					dl[i + 1] = MLIB_C_IMAGESCALARBLEND(
							sl1[i + 1],
							sl2[i + 1],
							alpha1);
					dl[i + 2] = MLIB_C_IMAGESCALARBLEND(
							sl1[i + 2],
							sl2[i + 2],
							alpha2);
				}
				sl1 = (mlib_d64 *)((mlib_u8 *)sl1 + slb1);
				sl2 = (mlib_d64 *)((mlib_u8 *)sl2 + slb2);
				dl = (mlib_d64 *)((mlib_u8 *)dl + dlb);
			}
		}
		break;
	case 4:
		alpha0 = alpha[0];
		alpha1 = alpha[1];
		alpha2 = alpha[2];
		alpha3 = alpha[3];
		alphas = _mm_set_pd(alpha1, alpha0);
		alphau = _mm_set_pd(alpha3, alpha2);
		betas  = _mm_sub_pd(ones, alphas);
		betau  = _mm_sub_pd(ones, alphau);
		if ((((mlib_addr)dst | dlb |
			(mlib_addr)src1 | slb1 |
			(mlib_addr)src2 | slb2) & 0xf) == 0) {
			for (j = 0; j < ysize; j ++) {
				srcPtr1 = sl1;
				srcPtr2 = sl2;
				dstPtr = dl;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i <= (nsize - 4); i += 4) {
					MLIB_S_IMAGESCALARBLEND4_D64(
						_mm_store_pd,
						_mm_load_pd,
						_mm_load_pd);
				}
				sl1 = (mlib_d64 *)((mlib_u8 *)sl1 + slb1);
				sl2 = (mlib_d64 *)((mlib_u8 *)sl2 + slb2);
				dl = (mlib_d64 *)((mlib_u8 *)dl + dlb);
			}
		} else
		if ((((mlib_addr)src1 | slb1 |
			(mlib_addr)src2 | slb2) & 0xf) == 0) {
			for (j = 0; j < ysize; j ++) {
				srcPtr1 = sl1;
				srcPtr2 = sl2;
				dstPtr = dl;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i <= (nsize - 4); i += 4) {
					MLIB_S_IMAGESCALARBLEND4_D64(
						_mm_storeu_pd,
						_mm_load_pd,
						_mm_load_pd);
				}
				sl1 = (mlib_d64 *)((mlib_u8 *)sl1 + slb1);
				sl2 = (mlib_d64 *)((mlib_u8 *)sl2 + slb2);
				dl = (mlib_d64 *)((mlib_u8 *)dl + dlb);
			}
		} else {
			for (j = 0; j < ysize; j ++) {
				srcPtr1 = sl1;
				srcPtr2 = sl2;
				dstPtr = dl;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i <= (nsize - 4); i += 4) {
					MLIB_S_IMAGESCALARBLEND4_D64(
						_mm_storeu_pd,
						_mm_loadu_pd,
						_mm_loadu_pd);
				}
				sl1 = (mlib_d64 *)((mlib_u8 *)sl1 + slb1);
				sl2 = (mlib_d64 *)((mlib_u8 *)sl2 + slb2);
				dl = (mlib_d64 *)((mlib_u8 *)dl + dlb);
			}
		}
		break;
	}
}

/* *********************************************************** */
