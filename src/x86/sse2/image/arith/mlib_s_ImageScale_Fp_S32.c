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

#pragma ident "@(#)mlib_s_ImageScale_Fp_S32.c	9.4	07/11/05 SMI"

/*
 * FUNCTION
 *	mlib_s_ImageScale_Fp_s32_f32	- linear scaling
 *	mlib_s_ImageScale_Fp_s32_d64	- linear scaling
 *
 * SYNOPSIS
 *	void mlib_s_ImageScale_Fp_s32_f32(
 *				mlib_s32 *dst,
 *				mlib_s32 dlb,
 *				const mlib_f32 *src,
 *				mlib_s32 slb,
 *				mlib_s32 xsize,
 *				mlib_s32 ysize,
 *				mlib_s32 nchan,
 *				const mlib_d64 *alpha,
 *				const mlib_d64 *beta);
 *	void mlib_s_ImageScale_Fp_s32_d64(
 *				mlib_s32 *dst,
 *				mlib_s32 dlb,
 *				const mlib_d64 *src,
 *				mlib_s32 slb,
 *				mlib_s32 xsize,
 *				mlib_s32 ysize,
 *				mlib_s32 nchan,
 *				const mlib_d64 *alpha,
 *				const mlib_d64 *beta);
 *
 * ARGUMENT
 *	dst     pointer to the output image data
 *	src     pointer to the input image data
 *      xsize	image width in pixels
 *      ysize	image height in pixels
 *      nchan	number of channels in image
 *	alpha   array of scaling factors
 *	beta    array of offsets
 *
 * DESCRIPTION
 *	dst[x][y][c] = src[x][y][c] * alpha[c] + beta[c]
 *	srcdst[x][y][c] = srcdst[x][y][c] * alpha[c] + beta[c]
 *
 *	If the result of the operation underflows/overflows the
 *	minimum/maximum value supported by the destination image, then
 *	it will be clamped to the minimum/maximum value respectively.
 */

#ifdef __SUNPRO_C
#include <sunmedia_intrin.h>
#else
#include <emmintrin.h>
#endif

#include <mlib_image.h>
#include "mlib_s_ImageScale_Fp.h"

/* *********************************************************** */

#define	MLIB_C_IMAGESCALE_S32_F32(d, s, a, b)			\
	{							\
		mlib_f32 t;					\
		t = ((s) * (a) + (b));				\
		if (t >= MLIB_S32_MAX) {			\
			(d) = MLIB_S32_MAX;			\
		} else						\
		if (t <= MLIB_S32_MIN) {			\
			(d) = MLIB_S32_MIN;			\
		} else {					\
			(d) = (mlib_s32)t;			\
		}						\
	}

#define	MLIB_S_IMAGESCALE_S32_F32(D_STORE, S_LOAD)		\
	{							\
		__m128 dsts, srcs;				\
		__m128i s32s;					\
		srcs = S_LOAD((void *)&(src[i]));		\
		dsts = _mm_mul_ps(alphas, srcs);		\
		dsts = _mm_add_ps(betas, dsts);			\
		dsts = _mm_min_ps(dsts, dmax);			\
		dsts = _mm_max_ps(dsts, dmin);			\
		s32s = _mm_cvtps_epi32(dsts);			\
		D_STORE((void *)&(dst[i]), s32s);		\
	}

#define	MLIB_S_IMAGESCALE3_F32_F32(D_STORE, S_LOAD)		\
	{							\
		__m128 dsts, srcs;				\
		__m128i s32s;					\
		srcs = S_LOAD((void *)&(src[i]));		\
		dsts = _mm_mul_ps(alphas, srcs);		\
		dsts = _mm_add_ps(betas, dsts);			\
		dsts = _mm_min_ps(dsts, dmax);			\
		dsts = _mm_max_ps(dsts, dmin);			\
		s32s = _mm_cvtps_epi32(dsts);			\
		D_STORE((void *)&(dst[i]), s32s);		\
		srcs = S_LOAD((void *)&(src[i + 4]));		\
		dsts = _mm_mul_ps(alphau, srcs);		\
		dsts = _mm_add_ps(betau, dsts);			\
		dsts = _mm_min_ps(dsts, dmax);			\
		dsts = _mm_max_ps(dsts, dmin);			\
		s32s = _mm_cvtps_epi32(dsts);			\
		D_STORE((void *)&(dst[i + 4]), s32s);		\
		srcs = S_LOAD((void *)&(src[i + 8]));		\
		dsts = _mm_mul_ps(alphav, srcs);		\
		dsts = _mm_add_ps(betav, dsts);			\
		dsts = _mm_min_ps(dsts, dmax);			\
		dsts = _mm_max_ps(dsts, dmin);			\
		s32s = _mm_cvtps_epi32(dsts);			\
		D_STORE((void *)&(dst[i + 8]), s32s);		\
	}

/* *********************************************************** */

void
mlib_s_ImageScale_Fp_s32_f32(
	mlib_s32 *dst,
	mlib_s32 dlb,
	const mlib_f32 *src,
	mlib_s32 slb,
	mlib_s32 xsize,
	mlib_s32 ysize,
	mlib_s32 nchan,
	const mlib_d64 *alpha,
	const mlib_d64 *beta)
{
	mlib_s32 i, j, nsize;
	mlib_f32 alpha0, alpha1, alpha2, alpha3;
	__m128 alphas, alphau, alphav;
	mlib_f32 beta0, beta1, beta2, beta3;
	__m128 betas, betau, betav;
	__m128 dmax, dmin;

	/*
	 * Narrow the range of saturation to compensate the lose
	 * of precision from 23 bits fractional in float format.
	 */
	dmax = _mm_set1_ps((mlib_f32)(MLIB_S32_MAX - 190));
	dmin = _mm_set1_ps((mlib_f32)(MLIB_S32_MIN + 64));

	nsize = xsize * nchan;
	switch (nchan) {
	case 1:
		alpha0 = alpha[0];
		alphas = _mm_set1_ps(alpha0);
		beta0 = beta[0];
		betas = _mm_set1_ps(beta0);
		if ((((mlib_addr)dst | dlb |
			(mlib_addr)src | slb) & 0xf) == 0) {
			for (j = 0; j < ysize; j ++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i <= (nsize - 4); i += 4) {
					MLIB_S_IMAGESCALE_S32_F32(
						_mm_store_si128,
						_mm_load_ps);
				}
				for (; i < nsize; i ++) {
					MLIB_C_IMAGESCALE_S32_F32(
						dst[i], src[i],
						alpha0, beta0);
				}
				src  = (mlib_f32 *)((mlib_u8 *)src + slb);
				dst  = (mlib_s32 *)((mlib_u8 *)dst + dlb);
			}
		} else
		if ((((mlib_addr)src | slb) & 0xf) == 0) {
			for (j = 0; j < ysize; j ++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i <= (nsize - 4); i += 4) {
					MLIB_S_IMAGESCALE_S32_F32(
						_mm_storeu_si128,
						_mm_load_ps);
				}
				for (; i < nsize; i ++) {
					MLIB_C_IMAGESCALE_S32_F32(
						dst[i], src[i],
						alpha0, beta0);
				}
				src  = (mlib_f32 *)((mlib_u8 *)src + slb);
				dst  = (mlib_s32 *)((mlib_u8 *)dst + dlb);
			}
		} else {
			for (j = 0; j < ysize; j ++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i <= (nsize - 4); i += 4) {
					MLIB_S_IMAGESCALE_S32_F32(
						_mm_storeu_si128,
						_mm_loadu_ps);
				}
				for (; i < nsize; i ++) {
					MLIB_C_IMAGESCALE_S32_F32(
						dst[i], src[i],
						alpha0, beta0);
				}
				src  = (mlib_f32 *)((mlib_u8 *)src + slb);
				dst  = (mlib_s32 *)((mlib_u8 *)dst + dlb);
			}
		}
		break;
	case 2:
		alpha0 = alpha[0];
		alpha1 = alpha[1];
		alphas = _mm_set_ps(alpha1, alpha0, alpha1, alpha0);
		beta0 = beta[0];
		beta1 = beta[1];
		betas = _mm_set_ps(beta1, beta0, beta1, beta0);
		if ((((mlib_addr)dst | dlb |
			(mlib_addr)src | slb) & 0xf) == 0) {
			for (j = 0; j < ysize; j ++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i <= (nsize - 4); i += 4) {
					MLIB_S_IMAGESCALE_S32_F32(
						_mm_store_si128,
						_mm_load_ps);
				}
				for (; i < nsize; i += 2) {
					MLIB_C_IMAGESCALE_S32_F32(
						dst[i], src[i],
						alpha0, beta0);
					MLIB_C_IMAGESCALE_S32_F32(
						dst[i + 1], src[i + 1],
						alpha1, beta1);
				}
				src  = (mlib_f32 *)((mlib_u8 *)src + slb);
				dst  = (mlib_s32 *)((mlib_u8 *)dst + dlb);
			}
		} else
		if ((((mlib_addr)src | slb) & 0xf) == 0) {
			for (j = 0; j < ysize; j ++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i <= (nsize - 4); i += 4) {
					MLIB_S_IMAGESCALE_S32_F32(
						_mm_storeu_si128,
						_mm_load_ps);
				}
				for (; i < nsize; i += 2) {
					MLIB_C_IMAGESCALE_S32_F32(
						dst[i], src[i],
						alpha0, beta0);
					MLIB_C_IMAGESCALE_S32_F32(
						dst[i + 1], src[i + 1],
						alpha1, beta1);
				}
				src  = (mlib_f32 *)((mlib_u8 *)src + slb);
				dst  = (mlib_s32 *)((mlib_u8 *)dst + dlb);
			}
		} else {
			for (j = 0; j < ysize; j ++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i <= (nsize - 4); i += 4) {
					MLIB_S_IMAGESCALE_S32_F32(
						_mm_storeu_si128,
						_mm_loadu_ps);
				}
				for (; i < nsize; i += 2) {
					MLIB_C_IMAGESCALE_S32_F32(
						dst[i], src[i],
						alpha0, beta0);
					MLIB_C_IMAGESCALE_S32_F32(
						dst[i + 1], src[i + 1],
						alpha1, beta1);
				}
				src  = (mlib_f32 *)((mlib_u8 *)src + slb);
				dst  = (mlib_s32 *)((mlib_u8 *)dst + dlb);
			}
		}
		break;
	case 3:
		alpha0 = alpha[0];
		alpha1 = alpha[1];
		alpha2 = alpha[2];
		alphas = _mm_set_ps(alpha0, alpha2, alpha1, alpha0);
		alphau = _mm_set_ps(alpha1, alpha0, alpha2, alpha1);
		alphav = _mm_set_ps(alpha2, alpha1, alpha0, alpha2);
		beta0 = beta[0];
		beta1 = beta[1];
		beta2 = beta[2];
		betas = _mm_set_ps(beta0, beta2, beta1, beta0);
		betau = _mm_set_ps(beta1, beta0, beta2, beta1);
		betav = _mm_set_ps(beta2, beta1, beta0, beta2);
		if ((((mlib_addr)dst | dlb |
			(mlib_addr)src | slb) & 0xf) == 0) {
			for (j = 0; j < ysize; j ++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i <= (nsize - 12); i += 12) {
					MLIB_S_IMAGESCALE3_F32_F32(
						_mm_store_si128,
						_mm_load_ps);
				}
				for (; i < nsize; i += 3) {
					MLIB_C_IMAGESCALE_S32_F32(
						dst[i], src[i],
						alpha0, beta0);
					MLIB_C_IMAGESCALE_S32_F32(
						dst[i + 1], src[i + 1],
						alpha1, beta1);
					MLIB_C_IMAGESCALE_S32_F32(
						dst[i + 2], src[i + 2],
						alpha2, beta2);
				}
				src  = (mlib_f32 *)((mlib_u8 *)src + slb);
				dst  = (mlib_s32 *)((mlib_u8 *)dst + dlb);
			}
		} else
		if ((((mlib_addr)src | slb) & 0xf) == 0) {
			for (j = 0; j < ysize; j ++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i <= (nsize - 12); i += 12) {
					MLIB_S_IMAGESCALE3_F32_F32(
						_mm_storeu_si128,
						_mm_load_ps);
				}
				for (; i < nsize; i += 3) {
					MLIB_C_IMAGESCALE_S32_F32(
						dst[i], src[i],
						alpha0, beta0);
					MLIB_C_IMAGESCALE_S32_F32(
						dst[i + 1], src[i + 1],
						alpha1, beta1);
					MLIB_C_IMAGESCALE_S32_F32(
						dst[i + 2], src[i + 2],
						alpha2, beta2);
				}
				src  = (mlib_f32 *)((mlib_u8 *)src + slb);
				dst  = (mlib_s32 *)((mlib_u8 *)dst + dlb);
			}
		} else {
			for (j = 0; j < ysize; j ++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i <= (nsize - 12); i += 12) {
					MLIB_S_IMAGESCALE3_F32_F32(
						_mm_storeu_si128,
						_mm_loadu_ps);
				}
				for (; i < nsize; i += 3) {
					MLIB_C_IMAGESCALE_S32_F32(
						dst[i], src[i],
						alpha0, beta0);
					MLIB_C_IMAGESCALE_S32_F32(
						dst[i + 1], src[i + 1],
						alpha1, beta1);
					MLIB_C_IMAGESCALE_S32_F32(
						dst[i + 2], src[i + 2],
						alpha2, beta2);
				}
				src  = (mlib_f32 *)((mlib_u8 *)src + slb);
				dst  = (mlib_s32 *)((mlib_u8 *)dst + dlb);
			}
		}
		break;
	case 4:
		alpha0 = alpha[0];
		alpha1 = alpha[1];
		alpha2 = alpha[2];
		alpha3 = alpha[3];
		alphas = _mm_set_ps(alpha3, alpha2, alpha1, alpha0);
		beta0 = beta[0];
		beta1 = beta[1];
		beta2 = beta[2];
		beta3 = beta[3];
		betas = _mm_set_ps(beta3, beta2, beta1, beta0);
		if ((((mlib_addr)dst | dlb |
			(mlib_addr)src | slb) & 0xf) == 0) {
			for (j = 0; j < ysize; j ++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i <= (nsize - 4); i += 4) {
					MLIB_S_IMAGESCALE_S32_F32(
						_mm_store_si128,
						_mm_load_ps);
				}
				for (; i < nsize; i += 4) {
					MLIB_C_IMAGESCALE_S32_F32(
						dst[i], src[i],
						alpha0, beta0);
					MLIB_C_IMAGESCALE_S32_F32(
						dst[i + 1], src[i + 1],
						alpha1, beta1);
					MLIB_C_IMAGESCALE_S32_F32(
						dst[i + 2], src[i + 2],
						alpha2, beta2);
					MLIB_C_IMAGESCALE_S32_F32(
						dst[i + 3], src[i + 3],
						alpha3, beta3);
				}
				src  = (mlib_f32 *)((mlib_u8 *)src + slb);
				dst  = (mlib_s32 *)((mlib_u8 *)dst + dlb);
			}
		} else
		if ((((mlib_addr)src | slb) & 0xf) == 0) {
			for (j = 0; j < ysize; j ++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i <= (nsize - 4); i += 4) {
					MLIB_S_IMAGESCALE_S32_F32(
						_mm_storeu_si128,
						_mm_load_ps);
				}
				for (; i < nsize; i += 4) {
					MLIB_C_IMAGESCALE_S32_F32(
						dst[i], src[i],
						alpha0, beta0);
					MLIB_C_IMAGESCALE_S32_F32(
						dst[i + 1], src[i + 1],
						alpha1, beta1);
					MLIB_C_IMAGESCALE_S32_F32(
						dst[i + 2], src[i + 2],
						alpha2, beta2);
					MLIB_C_IMAGESCALE_S32_F32(
						dst[i + 3], src[i + 3],
						alpha3, beta3);
				}
				src  = (mlib_f32 *)((mlib_u8 *)src + slb);
				dst  = (mlib_s32 *)((mlib_u8 *)dst + dlb);
			}
		} else {
			for (j = 0; j < ysize; j ++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i <= (nsize - 4); i += 4) {
					MLIB_S_IMAGESCALE_S32_F32(
						_mm_storeu_si128,
						_mm_loadu_ps);
				}
				for (; i < nsize; i += 4) {
					MLIB_C_IMAGESCALE_S32_F32(
						dst[i], src[i],
						alpha0, beta0);
					MLIB_C_IMAGESCALE_S32_F32(
						dst[i + 1], src[i + 1],
						alpha1, beta1);
					MLIB_C_IMAGESCALE_S32_F32(
						dst[i + 2], src[i + 2],
						alpha2, beta2);
					MLIB_C_IMAGESCALE_S32_F32(
						dst[i + 3], src[i + 3],
						alpha3, beta3);
				}
				src  = (mlib_f32 *)((mlib_u8 *)src + slb);
				dst  = (mlib_s32 *)((mlib_u8 *)dst + dlb);
			}
		}
		break;
	}
}

/* *********************************************************** */

#define	MLIB_C_IMAGESCALE_S32_D64(d, s, a, b)			\
	{							\
		mlib_d64 t;					\
		t = ((s) * (a) + (b));				\
		if (t >= MLIB_S32_MAX) {			\
			(d) = MLIB_S32_MAX;			\
		} else						\
		if (t <= MLIB_S32_MIN) {			\
			(d) = MLIB_S32_MIN;			\
		} else {					\
			(d) = (mlib_s32)t;			\
		}						\
	}

#define	MLIB_S_IMAGESCALE_S32_D64(D_STORE, S_LOAD)		\
	{							\
		__m128d dsts, srcs;				\
		__m128i s32sh, s32sl, s32s;			\
		srcs = S_LOAD((void *)&(src[i]));		\
		dsts = _mm_mul_pd(alphas, srcs);		\
		dsts = _mm_add_pd(betas, dsts);			\
		dsts = _mm_min_pd(dsts, dmax);			\
		dsts = _mm_max_pd(dsts, dmin);			\
		s32sl = _mm_cvtpd_epi32(dsts);			\
		srcs = S_LOAD((void *)&(src[i + 2]));		\
		dsts = _mm_mul_pd(alphas, srcs);		\
		dsts = _mm_add_pd(betas, dsts);			\
		dsts = _mm_min_pd(dsts, dmax);			\
		dsts = _mm_max_pd(dsts, dmin);			\
		s32sh = _mm_cvtpd_epi32(dsts);			\
		s32sh = _mm_slli_si128(s32sh, 8);		\
		s32s = _mm_or_si128(s32sl, s32sh);		\
		D_STORE((void *)&(dst[i]), s32s);		\
	}

#define	MLIB_S_IMAGESCALE3_F32_D64(D_STORE, S_LOAD)		\
	{							\
		__m128d dsts, srcs;				\
		__m128i s32sh, s32sl, s32s;			\
		srcs = S_LOAD((void *)&(src[i]));		\
		dsts = _mm_mul_pd(alphas, srcs);		\
		dsts = _mm_add_pd(betas, dsts);			\
		dsts = _mm_min_pd(dsts, dmax);			\
		dsts = _mm_max_pd(dsts, dmin);			\
		s32sl = _mm_cvtpd_epi32(dsts);			\
		srcs = S_LOAD((void *)&(src[i + 2]));		\
		dsts = _mm_mul_pd(alphau, srcs);		\
		dsts = _mm_add_pd(betau, dsts);			\
		dsts = _mm_min_pd(dsts, dmax);			\
		dsts = _mm_max_pd(dsts, dmin);			\
		s32sh = _mm_cvtpd_epi32(dsts);			\
		s32sh = _mm_slli_si128(s32sh, 8);		\
		s32s = _mm_or_si128(s32sl, s32sh);		\
		D_STORE((void *)&(dst[i]), s32s);		\
		srcs = S_LOAD((void *)&(src[i + 4]));		\
		dsts = _mm_mul_pd(alphav, srcs);		\
		dsts = _mm_add_pd(betav, dsts);			\
		dsts = _mm_min_pd(dsts, dmax);			\
		dsts = _mm_max_pd(dsts, dmin);			\
		s32sl = _mm_cvtpd_epi32(dsts);			\
		srcs = S_LOAD((void *)&(src[i + 6]));		\
		dsts = _mm_mul_pd(alphas, srcs);		\
		dsts = _mm_add_pd(betas, dsts);			\
		dsts = _mm_min_pd(dsts, dmax);			\
		dsts = _mm_max_pd(dsts, dmin);			\
		s32sh = _mm_cvtpd_epi32(dsts);			\
		s32sh = _mm_slli_si128(s32sh, 8);		\
		s32s = _mm_or_si128(s32sl, s32sh);		\
		D_STORE((void *)&(dst[i + 4]), s32s);		\
		srcs = S_LOAD((void *)&(src[i + 8]));		\
		dsts = _mm_mul_pd(alphau, srcs);		\
		dsts = _mm_add_pd(betau, dsts);			\
		dsts = _mm_min_pd(dsts, dmax);			\
		dsts = _mm_max_pd(dsts, dmin);			\
		s32sl = _mm_cvtpd_epi32(dsts);			\
		srcs = S_LOAD((void *)&(src[i + 10]));		\
		dsts = _mm_mul_pd(alphav, srcs);		\
		dsts = _mm_add_pd(betav, dsts);			\
		dsts = _mm_min_pd(dsts, dmax);			\
		dsts = _mm_max_pd(dsts, dmin);			\
		s32sh = _mm_cvtpd_epi32(dsts);			\
		s32sh = _mm_slli_si128(s32sh, 8);		\
		s32s = _mm_or_si128(s32sl, s32sh);		\
		D_STORE((void *)&(dst[i + 8]), s32s);		\
	}

#define	MLIB_S_IMAGESCALE4_F32_D64(D_STORE, S_LOAD)		\
	{							\
		__m128d dsts, srcs;				\
		__m128i s32sh, s32sl, s32s;			\
		srcs = S_LOAD((void *)&(src[i]));		\
		dsts = _mm_mul_pd(alphas, srcs);		\
		dsts = _mm_add_pd(betas, dsts);			\
		dsts = _mm_min_pd(dsts, dmax);			\
		dsts = _mm_max_pd(dsts, dmin);			\
		s32sl = _mm_cvtpd_epi32(dsts);			\
		srcs = S_LOAD((void *)&(src[i + 2]));		\
		dsts = _mm_mul_pd(alphau, srcs);		\
		dsts = _mm_add_pd(betau, dsts);			\
		dsts = _mm_min_pd(dsts, dmax);			\
		dsts = _mm_max_pd(dsts, dmin);			\
		s32sh = _mm_cvtpd_epi32(dsts);			\
		s32sh = _mm_slli_si128(s32sh, 8);		\
		s32s = _mm_or_si128(s32sl, s32sh);		\
		D_STORE((void *)&(dst[i]), s32s);		\
	}

/* *********************************************************** */

void
mlib_s_ImageScale_Fp_s32_d64(
	mlib_s32 *dst,
	mlib_s32 dlb,
	const mlib_d64 *src,
	mlib_s32 slb,
	mlib_s32 xsize,
	mlib_s32 ysize,
	mlib_s32 nchan,
	const mlib_d64 *alpha,
	const mlib_d64 *beta)
{
	mlib_s32 i, j, nsize;
	mlib_d64 alpha0, alpha1, alpha2, alpha3;
	__m128d alphas, alphau, alphav;
	mlib_d64 beta0, beta1, beta2, beta3;
	__m128d betas, betau, betav;
	__m128d dmax, dmin;

	dmax = _mm_set1_pd((mlib_d64)MLIB_S32_MAX);
	dmin = _mm_set1_pd((mlib_d64)MLIB_S32_MIN);

	nsize = xsize * nchan;
	switch (nchan) {
	case 1:
		alpha0 = alpha[0];
		alphas = _mm_set1_pd(alpha0);
		beta0 = beta[0];
		betas = _mm_set1_pd(beta0);
		if ((((mlib_addr)dst | dlb |
			(mlib_addr)src | slb) & 0xf) == 0) {
			for (j = 0; j < ysize; j ++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i <= (nsize - 4); i += 4) {
					MLIB_S_IMAGESCALE_S32_D64(
						_mm_store_si128,
						_mm_load_pd);
				}
				for (; i < nsize; i ++) {
					MLIB_C_IMAGESCALE_S32_D64(
						dst[i], src[i],
						alpha0, beta0);
				}
				src  = (mlib_d64 *)((mlib_u8 *)src + slb);
				dst  = (mlib_s32 *)((mlib_u8 *)dst + dlb);
			}
		} else
		if ((((mlib_addr)src | slb) & 0xf) == 0) {
			for (j = 0; j < ysize; j ++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i <= (nsize - 4); i += 4) {
					MLIB_S_IMAGESCALE_S32_D64(
						_mm_storeu_si128,
						_mm_load_pd);
				}
				for (; i < nsize; i ++) {
					MLIB_C_IMAGESCALE_S32_D64(
						dst[i], src[i],
						alpha0, beta0);
				}
				src  = (mlib_d64 *)((mlib_u8 *)src + slb);
				dst  = (mlib_s32 *)((mlib_u8 *)dst + dlb);
			}
		} else {
			for (j = 0; j < ysize; j ++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i <= (nsize - 4); i += 4) {
					MLIB_S_IMAGESCALE_S32_D64(
						_mm_storeu_si128,
						_mm_loadu_pd);
				}
				for (; i < nsize; i ++) {
					MLIB_C_IMAGESCALE_S32_D64(
						dst[i], src[i],
						alpha0, beta0);
				}
				src  = (mlib_d64 *)((mlib_u8 *)src + slb);
				dst  = (mlib_s32 *)((mlib_u8 *)dst + dlb);
			}
		}
		break;
	case 2:
		alpha0 = alpha[0];
		alpha1 = alpha[1];
		alphas = _mm_set_pd(alpha1, alpha0);
		beta0 = beta[0];
		beta1 = beta[1];
		betas = _mm_set_pd(beta1, beta0);
		if ((((mlib_addr)dst | dlb |
			(mlib_addr)src | slb) & 0xf) == 0) {
			for (j = 0; j < ysize; j ++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i <= (nsize - 4); i += 4) {
					MLIB_S_IMAGESCALE_S32_D64(
						_mm_store_si128,
						_mm_load_pd);
				}
				for (; i < nsize; i += 2) {
					MLIB_C_IMAGESCALE_S32_D64(
						dst[i], src[i],
						alpha0, beta0);
					MLIB_C_IMAGESCALE_S32_D64(
						dst[i + 1], src[i + 1],
						alpha1, beta1);
				}
				src  = (mlib_d64 *)((mlib_u8 *)src + slb);
				dst  = (mlib_s32 *)((mlib_u8 *)dst + dlb);
			}
		} else
		if ((((mlib_addr)src | slb) & 0xf) == 0) {
			for (j = 0; j < ysize; j ++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i <= (nsize - 4); i += 4) {
					MLIB_S_IMAGESCALE_S32_D64(
						_mm_storeu_si128,
						_mm_load_pd);
				}
				for (; i < nsize; i += 2) {
					MLIB_C_IMAGESCALE_S32_D64(
						dst[i], src[i],
						alpha0, beta0);
					MLIB_C_IMAGESCALE_S32_D64(
						dst[i + 1], src[i + 1],
						alpha1, beta1);
				}
				src  = (mlib_d64 *)((mlib_u8 *)src + slb);
				dst  = (mlib_s32 *)((mlib_u8 *)dst + dlb);
			}
		} else {
			for (j = 0; j < ysize; j ++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i <= (nsize - 4); i += 4) {
					MLIB_S_IMAGESCALE_S32_D64(
						_mm_storeu_si128,
						_mm_loadu_pd);
				}
				for (; i < nsize; i += 2) {
					MLIB_C_IMAGESCALE_S32_D64(
						dst[i], src[i],
						alpha0, beta0);
					MLIB_C_IMAGESCALE_S32_D64(
						dst[i + 1], src[i + 1],
						alpha1, beta1);
				}
				src  = (mlib_d64 *)((mlib_u8 *)src + slb);
				dst  = (mlib_s32 *)((mlib_u8 *)dst + dlb);
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
		beta0 = beta[0];
		beta1 = beta[1];
		beta2 = beta[2];
		betas = _mm_set_pd(beta1, beta0);
		betau = _mm_set_pd(beta0, beta2);
		betav = _mm_set_pd(beta2, beta1);
		if ((((mlib_addr)dst | dlb |
			(mlib_addr)src | slb) & 0xf) == 0) {
			for (j = 0; j < ysize; j ++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i <= (nsize - 12); i += 12) {
					MLIB_S_IMAGESCALE3_F32_D64(
						_mm_store_si128,
						_mm_load_pd);
				}
				for (; i < nsize; i += 3) {
					MLIB_C_IMAGESCALE_S32_D64(
						dst[i], src[i],
						alpha0, beta0);
					MLIB_C_IMAGESCALE_S32_D64(
						dst[i + 1], src[i + 1],
						alpha1, beta1);
					MLIB_C_IMAGESCALE_S32_D64(
						dst[i + 2], src[i + 2],
						alpha2, beta2);
				}
				src  = (mlib_d64 *)((mlib_u8 *)src + slb);
				dst  = (mlib_s32 *)((mlib_u8 *)dst + dlb);
			}
		} else
		if ((((mlib_addr)src | slb) & 0xf) == 0) {
			for (j = 0; j < ysize; j ++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i <= (nsize - 12); i += 12) {
					MLIB_S_IMAGESCALE3_F32_D64(
						_mm_storeu_si128,
						_mm_load_pd);
				}
				for (; i < nsize; i += 3) {
					MLIB_C_IMAGESCALE_S32_D64(
						dst[i], src[i],
						alpha0, beta0);
					MLIB_C_IMAGESCALE_S32_D64(
						dst[i + 1], src[i + 1],
						alpha1, beta1);
					MLIB_C_IMAGESCALE_S32_D64(
						dst[i + 2], src[i + 2],
						alpha2, beta2);
				}
				src  = (mlib_d64 *)((mlib_u8 *)src + slb);
				dst  = (mlib_s32 *)((mlib_u8 *)dst + dlb);
			}
		} else {
			for (j = 0; j < ysize; j ++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i <= (nsize - 12); i += 12) {
					MLIB_S_IMAGESCALE3_F32_D64(
						_mm_storeu_si128,
						_mm_loadu_pd);
				}
				for (; i < nsize; i += 3) {
					MLIB_C_IMAGESCALE_S32_D64(
						dst[i], src[i],
						alpha0, beta0);
					MLIB_C_IMAGESCALE_S32_D64(
						dst[i + 1], src[i + 1],
						alpha1, beta1);
					MLIB_C_IMAGESCALE_S32_D64(
						dst[i + 2], src[i + 2],
						alpha2, beta2);
				}
				src  = (mlib_d64 *)((mlib_u8 *)src + slb);
				dst  = (mlib_s32 *)((mlib_u8 *)dst + dlb);
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
		beta0 = beta[0];
		beta1 = beta[1];
		beta2 = beta[2];
		beta3 = beta[3];
		betas = _mm_set_pd(beta1, beta0);
		betau = _mm_set_pd(beta3, beta2);
		if ((((mlib_addr)dst | dlb |
			(mlib_addr)src | slb) & 0xf) == 0) {
			for (j = 0; j < ysize; j ++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i <= (nsize - 4); i += 4) {
					MLIB_S_IMAGESCALE4_F32_D64(
						_mm_store_si128,
						_mm_load_pd);
				}
				for (; i < nsize; i += 4) {
					MLIB_C_IMAGESCALE_S32_D64(
						dst[i], src[i],
						alpha0, beta0);
					MLIB_C_IMAGESCALE_S32_D64(
						dst[i + 1], src[i + 1],
						alpha1, beta1);
					MLIB_C_IMAGESCALE_S32_D64(
						dst[i + 2], src[i + 2],
						alpha2, beta2);
					MLIB_C_IMAGESCALE_S32_D64(
						dst[i + 3], src[i + 3],
						alpha3, beta3);
				}
				src  = (mlib_d64 *)((mlib_u8 *)src + slb);
				dst  = (mlib_s32 *)((mlib_u8 *)dst + dlb);
			}
		} else
		if ((((mlib_addr)src | slb) & 0xf) == 0) {
			for (j = 0; j < ysize; j ++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i <= (nsize - 4); i += 4) {
					MLIB_S_IMAGESCALE4_F32_D64(
						_mm_storeu_si128,
						_mm_load_pd);
				}
				for (; i < nsize; i += 4) {
					MLIB_C_IMAGESCALE_S32_D64(
						dst[i], src[i],
						alpha0, beta0);
					MLIB_C_IMAGESCALE_S32_D64(
						dst[i + 1], src[i + 1],
						alpha1, beta1);
					MLIB_C_IMAGESCALE_S32_D64(
						dst[i + 2], src[i + 2],
						alpha2, beta2);
					MLIB_C_IMAGESCALE_S32_D64(
						dst[i + 3], src[i + 3],
						alpha3, beta3);
				}
				src  = (mlib_d64 *)((mlib_u8 *)src + slb);
				dst  = (mlib_s32 *)((mlib_u8 *)dst + dlb);
			}
		} else {
			for (j = 0; j < ysize; j ++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i <= (nsize - 4); i += 4) {
					MLIB_S_IMAGESCALE4_F32_D64(
						_mm_storeu_si128,
						_mm_loadu_pd);
				}
				for (; i < nsize; i += 4) {
					MLIB_C_IMAGESCALE_S32_D64(
						dst[i], src[i],
						alpha0, beta0);
					MLIB_C_IMAGESCALE_S32_D64(
						dst[i + 1], src[i + 1],
						alpha1, beta1);
					MLIB_C_IMAGESCALE_S32_D64(
						dst[i + 2], src[i + 2],
						alpha2, beta2);
					MLIB_C_IMAGESCALE_S32_D64(
						dst[i + 3], src[i + 3],
						alpha3, beta3);
				}
				src  = (mlib_d64 *)((mlib_u8 *)src + slb);
				dst  = (mlib_s32 *)((mlib_u8 *)dst + dlb);
			}
		}
		break;
	}
}

/* *********************************************************** */
