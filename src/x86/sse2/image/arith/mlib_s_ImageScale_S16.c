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

#pragma ident   "@(#)mlib_s_ImageScale_S16.c	9.2	07/11/05 SMI"

/*
 * FUNCTION
 *	mlib_s_ImageScale_s16_u8	- linear scaling
 *	mlib_s_ImageScale_s16_s16	- linear scaling
 *	mlib_s_ImageScale_s16_u16	- linear scaling
 *	mlib_s_ImageScale_s16_s32	- linear scaling
 *
 * SYNOPSIS
 *	mlib_status mlib_s_ImageScale_s16_u8(
 *				mlib_u8 *dst,
 *				mlib_s32 dlb,
 *				const mlib_s16 *src,
 *				mlib_s32 slb,
 *				mlib_s32 xsize,
 *				mlib_s32 ysize,
 *				mlib_s32 nchan,
 *				const mlib_s32 *alpha,
 *				const mlib_s32 *beta,
 *				const mlib_s32 shift);
 *	mlib_status mlib_s_ImageScale_s16_s16(
 *				mlib_s16 *dst,
 *				mlib_s32 dlb,
 *				const mlib_s16 *src,
 *				mlib_s32 slb,
 *				mlib_s32 xsize,
 *				mlib_s32 ysize,
 *				mlib_s32 nchan,
 *				const mlib_s32 *alpha,
 *				const mlib_s32 *beta,
 *				const mlib_s32 shift);
 *  mlib_status mlib_s_ImageScale_s16_u16(
 *				mlib_u16 *dst,
 *				mlib_s32 dlb,
 *				const mlib_s16 *src,
 *				mlib_s32 slb,
 *				mlib_s32 xsize,
 *				mlib_s32 ysize,
 *				mlib_s32 nchan,
 *				const mlib_s32 *alpha,
 *				const mlib_s32 *beta,
 *				const mlib_s32 shift);
 *  mlib_status mlib_s_ImageScale_s16_s32(
 *				mlib_s32 *dst,
 *				mlib_s32 dlb,
 *				const mlib_s16 *src,
 *				mlib_s32 slb,
 *				mlib_s32 xsize,
 *				mlib_s32 ysize,
 *				mlib_s32 nchan,
 *				const mlib_s32 *alpha,
 *				const mlib_s32 *beta,
 *				const mlib_s32 shift);
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

#include <mlib_image.h>
#include "mlib_s_ImageScale.h"
#ifdef __SUNPRO_C
#include <sunmedia_intrin.h>
#else
#include <emmintrin.h>
#endif

/* *********************************************************** */
#define	CLAMP_CONV_U8(y, x)                                     \
	{                                                       \
	    mlib_d64 xi = (x);                                  \
	                                                        \
	    if (xi >= MLIB_U8_MAX)                              \
			xi = MLIB_U8_MAX;                               \
	    if (xi <= MLIB_U8_MIN)                              \
			xi = MLIB_U8_MIN;                               \
	    y = (mlib_s32)xi;                                   \
	}

/* *********************************************************** */

#define	CLAMP_CONV_S16(y, x)                                    \
	{                                                       \
	    mlib_d64 xi = (x);                                  \
	                                                        \
	    if (xi >= MLIB_S16_MAX)                             \
			xi = MLIB_S16_MAX;                              \
	    if (xi <= MLIB_S16_MIN)                             \
			xi = MLIB_S16_MIN;                              \
	    y = (mlib_s32)xi;                                   \
	}

/* *********************************************************** */

#define	CLAMP_CONV_U16(y, x)                                    \
	{                                                       \
		CLAMP_CONV_S16(y, x);                               \
		y ^= 0x8000;                                        \
	}

/* *********************************************************** */

#define	CLAMP_CONV_S32(y, x)                                    \
	{                                                       \
	    mlib_d64 xi = (x);                                  \
	                                                        \
	    if (xi >= MLIB_S32_MAX)                             \
			xi = MLIB_S32_MAX;                              \
	    if (xi <= MLIB_S32_MIN)                             \
			xi = MLIB_S32_MIN;                              \
	    y = (mlib_s32)xi;                                   \
	}

/* *********************************************************** */

#define	MASK    0xffff

/* *********************************************************** */

#define	MLIB_S_IMAGESCALE_S16_U8(D_STORE, S_LOAD)	        \
	{                                                       \
		__m128i dsts, dsts0, dsts1, dsts2, dsts3;           \
		__m128i srcs0, srcs1;                               \
		__m128i part0, part1, part2, part3;                 \
		srcs0  = S_LOAD((void *)srcPtr);                    \
		srcPtr++;                                           \
		part0 = _mm_mullo_epi16(alphas, srcs0);             \
		part1 = _mm_mulhi_epi16(alphas, srcs0);             \
		dsts0 = _mm_unpacklo_epi16(part0, part1);           \
		dsts1 = _mm_unpackhi_epi16(part0, part1);           \
		dsts0 = _mm_srai_epi32(                             \
			_mm_add_epi32(dsts0, betas), shift);            \
		dsts1 = _mm_srai_epi32(                             \
			_mm_add_epi32(dsts1, betas), shift);            \
		dsts0 = _mm_packs_epi32(dsts0, dsts1);              \
		srcs1  = S_LOAD((void *)srcPtr);                    \
		srcPtr++;                                           \
		part2 = _mm_mullo_epi16(alphas, srcs1);             \
		part3 = _mm_mulhi_epi16(alphas, srcs1);             \
		dsts2 = _mm_unpacklo_epi16(part2, part3);           \
		dsts3 = _mm_unpackhi_epi16(part2, part3);           \
		dsts2 = _mm_srai_epi32(                             \
			_mm_add_epi32(dsts2, betas), shift);            \
		dsts3 = _mm_srai_epi32(                             \
			_mm_add_epi32(dsts3, betas), shift);            \
		dsts2 = _mm_packs_epi32(dsts2, dsts3);              \
		dsts = _mm_packus_epi16(dsts0, dsts2);              \
		D_STORE((void *)dstPtr, dsts);                      \
		dstPtr++;                                           \
	}

/* *********************************************************** */

#define	MLIB_S_IMAGESCALE3_S16_U8(D_STORE, S_LOAD)           \
	{                                                       \
		__m128i dsts, dsts0, dsts1;                         \
		__m128i srcs;                                       \
		__m128i part0, part1;                               \
		srcs  = S_LOAD((void *)srcPtr);                     \
		srcPtr++;                                           \
		part0 = _mm_mullo_epi16(alphas, srcs);              \
		part1 = _mm_mulhi_epi16(alphas, srcs);              \
		dsts0 = _mm_unpacklo_epi16(part0, part1);           \
		dsts1 = _mm_unpackhi_epi16(part0, part1);           \
		dsts0 = _mm_srai_epi32(                             \
			_mm_add_epi32(dsts0, betas), shift);            \
		dsts1 = _mm_srai_epi32(                             \
			_mm_add_epi32(dsts1, betau), shift);            \
		dsts0 = _mm_packs_epi32(dsts0, dsts1);              \
		dsts = _mm_packus_epi16(dsts0, dsts0);              \
		D_STORE((void *)dstPtr, dsts);                      \
		srcs  = S_LOAD((void *)srcPtr);                     \
		srcPtr++;                                           \
		part0 = _mm_mullo_epi16(alphau, srcs);              \
		part1 = _mm_mulhi_epi16(alphau, srcs);              \
		dsts0 = _mm_unpacklo_epi16(part0, part1);           \
		dsts1 = _mm_unpackhi_epi16(part0, part1);           \
		dsts0 = _mm_srai_epi32(                             \
			_mm_add_epi32(dsts0, betav), shift);            \
		dsts1 = _mm_srai_epi32(                             \
			_mm_add_epi32(dsts1, betas), shift);            \
		dsts0 = _mm_packs_epi32(dsts0, dsts1);              \
		dsts = _mm_packus_epi16(dsts0, dsts0);              \
		D_STORE((void *)((mlib_u8 *)dstPtr + 8), dsts);     \
		srcs  = S_LOAD((void *)srcPtr);                     \
		srcPtr++;                                           \
		part0 = _mm_mullo_epi16(alphav, srcs);              \
		part1 = _mm_mulhi_epi16(alphav, srcs);              \
		dsts0 = _mm_unpacklo_epi16(part0, part1);           \
		dsts1 = _mm_unpackhi_epi16(part0, part1);           \
		dsts0 = _mm_srai_epi32(                             \
			_mm_add_epi32(dsts0, betau), shift);            \
		dsts1 = _mm_srai_epi32(                             \
			_mm_add_epi32(dsts1, betav), shift);            \
		dsts0 = _mm_packs_epi32(dsts0, dsts1);              \
		dsts = _mm_packus_epi16(dsts0, dsts0);              \
		D_STORE((void *)((mlib_u8 *)dstPtr + 16), dsts);    \
		dstPtr = (__m128i *)((mlib_u8 *)dstPtr + 24);       \
	}

/* *********************************************************** */

#define	OPER_U8(k) (sl[i + k] * alpha[k] + beta[k]) >> shift
#define	OPER_S16(k) (sl[i + k] * alpha[k] + beta[k]) >> shift
#define	OPER_U16(k) (sl[i + k] * alpha[k] + beta[k]) >> shift
#define	OPER_S32(k) (sl[i + k] * alpha[k] + beta[k]) >> shift

#define	MLIB_C_IMAGESCALE_1(DTYPE)                         \
	xi0 = OPER_##DTYPE(0);                                 \
	CLAMP_CONV_##DTYPE(dl[i], xi0);                        \

#define	MLIB_C_IMAGESCALE_2(DTYPE)                         \
	xi0 = OPER_##DTYPE(0);                                 \
	xi1 = OPER_##DTYPE(1);                                 \
	CLAMP_CONV_##DTYPE(dl[i], xi0);                        \
	CLAMP_CONV_##DTYPE(dl[i + 1], xi1);                    \

#define	MLIB_C_IMAGESCALE_3(DTYPE)                         \
	xi0 = OPER_##DTYPE(0);                                 \
	xi1 = OPER_##DTYPE(1);                                 \
	xi2 = OPER_##DTYPE(2);                                 \
	CLAMP_CONV_##DTYPE(dl[i], xi0);                        \
	CLAMP_CONV_##DTYPE(dl[i + 1], xi1);                    \
	CLAMP_CONV_##DTYPE(dl[i + 2], xi2);                    \

#define	MLIB_C_IMAGESCALE_4(DTYPE)                         \
	xi0 = OPER_##DTYPE(0);                                 \
	xi1 = OPER_##DTYPE(1);                                 \
	xi2 = OPER_##DTYPE(2);                                 \
	xi3 = OPER_##DTYPE(3);                                 \
	CLAMP_CONV_##DTYPE(dl[i], xi0);                        \
	CLAMP_CONV_##DTYPE(dl[i + 1], xi1);                    \
	CLAMP_CONV_##DTYPE(dl[i + 2], xi2);                    \
	CLAMP_CONV_##DTYPE(dl[i + 3], xi3);                    \

/* *********************************************************** */


mlib_status
mlib_s_ImageScale_s16_u8(
	mlib_u8 *dst,
	mlib_s32 dlb,
	const mlib_s16 *src,
	mlib_s32 slb,
	mlib_s32 xsize,
	mlib_s32 ysize,
	mlib_s32 nchan,
	const mlib_s32 *alpha,
	const mlib_s32 *beta,
	const mlib_s32 shift)
{
	mlib_s32 i, j, nsize;
	mlib_s32 alpha0, alpha1, alpha2, alpha3;
	mlib_s32 xi0, xi1, xi2, xi3;
	__m128i *srcPtr, *dstPtr;
	mlib_u8 *dl = dst;
	mlib_s16 *sl = (mlib_s16 *)src;
	__m128i alphas, alphau, alphav;
	__m128i betas, betau, betav;
	mlib_s32 ialp, ival;
	mlib_s32 sdata;

	nsize = xsize * nchan;

	switch (nchan) {
	case 1:
		alpha0 = (alpha[0] & MASK);
		alphas = _mm_set1_epi16(alpha0);
		betas = _mm_set1_epi32(beta[0]);
		if ((((mlib_addr)dst | dlb |
			(mlib_addr)src | slb) & 0xf) == 0) {
			for (j = 0; j < ysize; j ++) {
				srcPtr = (__m128i *)sl;
				dstPtr = (__m128i *)dl;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i <= (nsize - 16); i += 16) {
					MLIB_S_IMAGESCALE_S16_U8(
						_mm_store_si128,
						_mm_load_si128);
				}
				for (; i < nsize; i++) {
					MLIB_C_IMAGESCALE_1(U8);
				}
				sl = (mlib_s16 *)((mlib_u8 *)sl + slb);
				dl += dlb;
			}
		} else
		if ((((mlib_addr)src | slb) & 0xf) == 0) {
			for (j = 0; j < ysize; j ++) {
				srcPtr = (__m128i *)sl;
				dstPtr = (__m128i *)dl;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i <= (nsize - 16); i += 16) {
					MLIB_S_IMAGESCALE_S16_U8(
						_mm_storeu_si128,
						_mm_load_si128);
				}
				for (; i < nsize; i++) {
					MLIB_C_IMAGESCALE_1(U8);
				}

				sl = (mlib_s16 *)((mlib_u8 *)sl + slb);
				dl += dlb;
			}
		} else {
			for (j = 0; j < ysize; j ++) {
				srcPtr = (__m128i *)sl;
				dstPtr = (__m128i *)dl;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i <= (nsize - 16); i += 16) {
					MLIB_S_IMAGESCALE_S16_U8(
						_mm_storeu_si128,
						_mm_loadu_si128);
				}
				for (; i < nsize; i++) {
					MLIB_C_IMAGESCALE_1(U8);
				}

				sl = (mlib_s16 *)((mlib_u8 *)sl + slb);
				dl += dlb;
			}
		}
		break;
	case 2:
		alpha0 = (alpha[0] & MASK);
		alpha1 = ((alpha[1] & MASK) << 16) | alpha0;
		alphas = _mm_set1_epi32(alpha1);
		betas = _mm_set_epi32(
			beta[1], beta[0], beta[1], beta[0]);

		if ((((mlib_addr)dst | dlb |
			(mlib_addr)src | slb) & 0xf) == 0) {
			for (j = 0; j < ysize; j ++) {
				srcPtr = (__m128i *)sl;
				dstPtr = (__m128i *)dl;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i <= (nsize - 16); i += 16) {
					MLIB_S_IMAGESCALE_S16_U8(
						_mm_store_si128,
						_mm_load_si128);
				}
				for (; i < nsize; i += 2) {
					MLIB_C_IMAGESCALE_2(U8);
				}

				sl = (mlib_s16 *)((mlib_u8 *)sl + slb);
				dl += dlb;
			}
		} else
		if ((((mlib_addr)src | slb) & 0xf) == 0) {
			for (j = 0; j < ysize; j ++) {
				srcPtr = (__m128i *)sl;
				dstPtr = (__m128i *)dl;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i <= (nsize - 16); i += 16) {
					MLIB_S_IMAGESCALE_S16_U8(
						_mm_storeu_si128,
						_mm_load_si128);
				}
				for (; i < nsize; i += 2) {
					MLIB_C_IMAGESCALE_2(U8);
				}

				sl = (mlib_s16 *)((mlib_u8 *)sl + slb);
				dl += dlb;
			}
		} else {
			for (j = 0; j < ysize; j ++) {
				srcPtr = (__m128i *)sl;
				dstPtr = (__m128i *)dl;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i <= (nsize - 16); i += 16) {
					MLIB_S_IMAGESCALE_S16_U8(
						_mm_storeu_si128,
						_mm_loadu_si128);
				}
				for (; i < nsize; i += 2) {
					MLIB_C_IMAGESCALE_2(U8);
				}

				sl = (mlib_s16 *)((mlib_u8 *)sl + slb);
				dl += dlb;
			}
		}
		break;
	case 3:
		alpha0 = (alpha[0] & MASK);
		alpha1 = (alpha[1] & MASK);
		alpha2 = (alpha[2] & MASK);
		alphas = _mm_set_epi16(alpha1, alpha0, alpha2,
			alpha1, alpha0, alpha2, alpha1, alpha0);
		alphau = _mm_set_epi16(alpha0, alpha2, alpha1,
			alpha0, alpha2, alpha1, alpha0, alpha2);
		alphav = _mm_set_epi16(alpha2, alpha1, alpha0,
			alpha2, alpha1, alpha0, alpha2, alpha1);
		betas = _mm_set_epi32(
			beta[0], beta[2], beta[1], beta[0]);
		betau = _mm_set_epi32(
			beta[1], beta[0], beta[2], beta[1]);
		betav = _mm_set_epi32(
			beta[2], beta[1], beta[0], beta[2]);

		if ((((mlib_addr)src | slb) & 0xf) == 0) {
			for (j = 0; j < ysize; j ++) {
				srcPtr = (__m128i *)sl;
				dstPtr = (__m128i *)dl;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i <= (nsize - 24); i += 24) {
					MLIB_S_IMAGESCALE3_S16_U8(
						_mm_storel_epi64,
						_mm_load_si128);
				}
				for (; i < nsize; i += 3) {
					MLIB_C_IMAGESCALE_3(U8);
				}

				sl = (mlib_s16 *)((mlib_u8 *)sl + slb);
				dl += dlb;
			}
		} else {
			for (j = 0; j < ysize; j ++) {
				srcPtr = (__m128i *)sl;
				dstPtr = (__m128i *)dl;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i <= (nsize - 24); i += 24) {
					MLIB_S_IMAGESCALE3_S16_U8(
						_mm_storel_epi64,
						_mm_loadu_si128);
				}
				for (; i < nsize; i += 3) {
					MLIB_C_IMAGESCALE_3(U8);
				}

				sl = (mlib_s16 *)((mlib_u8 *)sl + slb);
				dl += dlb;
			}

		}

		break;
	case 4:
		alpha0 = alpha[0] & MASK;
		alpha1 = ((alpha[1] & MASK) << 16) | alpha0;
		alpha2 = alpha[2] & MASK;
		alpha3 = ((alpha[3] & MASK) << 16) | alpha2;
		alphas = _mm_set_epi32(alpha3, alpha1, alpha3, alpha1);
		betas = _mm_set_epi32(
			beta[3], beta[2], beta[1], beta[0]);

		if ((((mlib_addr)dst | dlb |
			(mlib_addr)src | slb) & 0xf) == 0) {
			for (j = 0; j < ysize; j ++) {
				srcPtr = (__m128i *)sl;
				dstPtr = (__m128i *)dl;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i <= (nsize - 16); i += 16) {
					MLIB_S_IMAGESCALE_S16_U8(
						_mm_store_si128,
						_mm_load_si128);
				}
				for (; i < nsize; i += 4) {
					MLIB_C_IMAGESCALE_4(U8);
				}

				sl = (mlib_s16 *)((mlib_u8 *)sl + slb);
				dl += dlb;
			}
		} else
		if ((((mlib_addr)src | slb) & 0xf) == 0) {
			for (j = 0; j < ysize; j ++) {
				srcPtr = (__m128i *)sl;
				dstPtr = (__m128i *)dl;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i <= (nsize - 16); i += 16) {
					MLIB_S_IMAGESCALE_S16_U8(
						_mm_storeu_si128,
						_mm_load_si128);
				}
				for (; i < nsize; i += 4) {
					MLIB_C_IMAGESCALE_4(U8);
				}

				sl = (mlib_s16 *)((mlib_u8 *)sl + slb);
				dl += dlb;
			}
		} else {
			for (j = 0; j < ysize; j ++) {
				srcPtr = (__m128i *)sl;
				dstPtr = (__m128i *)dl;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i <= (nsize - 16); i += 16) {
					MLIB_S_IMAGESCALE_S16_U8(
						_mm_storeu_si128,
						_mm_loadu_si128);
				}
				for (; i < nsize; i += 4) {
					MLIB_C_IMAGESCALE_4(U8);
				}

				sl = (mlib_s16 *)((mlib_u8 *)sl + slb);
				dl += dlb;
			}
		}
		break;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#define	MLIB_S_IMAGESCALE_S16_S16(D_STORE, S_LOAD)	        \
	{                                                       \
		__m128i dsts, dsts0, dsts1;                         \
		__m128i srcs;                                       \
		__m128i part0, part1;                               \
		srcs  = S_LOAD((void *)srcPtr);                     \
		part0 = _mm_mullo_epi16(alphas, srcs);              \
		part1 = _mm_mulhi_epi16(alphas, srcs);              \
		dsts0 = _mm_unpacklo_epi16(part0, part1);           \
		dsts1 = _mm_unpackhi_epi16(part0, part1);           \
		dsts0 = _mm_srai_epi32(                             \
			_mm_add_epi32(dsts0, betas), shift);            \
		dsts1 = _mm_srai_epi32(                             \
			_mm_add_epi32(dsts1, betas), shift);            \
		dsts = _mm_packs_epi32(dsts0, dsts1);               \
		D_STORE((void *)dstPtr, dsts);                      \
		srcPtr++;                                           \
		dstPtr++;                                           \
	}

/* *********************************************************** */

#define	MLIB_S_IMAGESCALE3_S16_S16(D_STORE, S_LOAD)           \
	{                                                       \
		__m128i dsts, dsts0, dsts1;                         \
		__m128i srcs;                                       \
		__m128i part0, part1;                               \
		srcs  = S_LOAD((void *)srcPtr);                     \
		part0 = _mm_mullo_epi16(alphas, srcs);              \
		part1 = _mm_mulhi_epi16(alphas, srcs);              \
		dsts0 = _mm_unpacklo_epi16(part0, part1);           \
		dsts1 = _mm_unpackhi_epi16(part0, part1);           \
		dsts0 = _mm_srai_epi32(                             \
			_mm_add_epi32(dsts0, betas), shift);            \
		dsts1 = _mm_srai_epi32(                             \
			_mm_add_epi32(dsts1, betau), shift);            \
		dsts = _mm_packs_epi32(dsts0, dsts1);               \
		D_STORE((void *)dstPtr, dsts);                      \
		srcPtr++;                                           \
		dstPtr++;                                           \
		srcs  = S_LOAD((void *)srcPtr);                     \
		part0 = _mm_mullo_epi16(alphau, srcs);              \
		part1 = _mm_mulhi_epi16(alphau, srcs);              \
		dsts0 = _mm_unpacklo_epi16(part0, part1);           \
		dsts1 = _mm_unpackhi_epi16(part0, part1);           \
		dsts0 = _mm_srai_epi32(                             \
			_mm_add_epi32(dsts0, betav), shift);            \
		dsts1 = _mm_srai_epi32(                             \
			_mm_add_epi32(dsts1, betas), shift);            \
		dsts = _mm_packs_epi32(dsts0, dsts1);               \
		D_STORE((void *)dstPtr, dsts);                      \
		srcPtr++;                                           \
		dstPtr++;                                           \
		srcs  = S_LOAD((void *)srcPtr);                     \
		part0 = _mm_mullo_epi16(alphav, srcs);              \
		part1 = _mm_mulhi_epi16(alphav, srcs);              \
		dsts0 = _mm_unpacklo_epi16(part0, part1);           \
		dsts1 = _mm_unpackhi_epi16(part0, part1);           \
		dsts0 = _mm_srai_epi32(                             \
			_mm_add_epi32(dsts0, betau), shift);            \
		dsts1 = _mm_srai_epi32(                             \
			_mm_add_epi32(dsts1, betav), shift);            \
		dsts = _mm_packs_epi32(dsts0, dsts1);               \
		D_STORE((void *)dstPtr, dsts);                      \
		srcPtr++;                                           \
		dstPtr++;                                           \
	}

/* *********************************************************** */

mlib_status
mlib_s_ImageScale_s16_s16(
	mlib_s16 *dst,
	mlib_s32 dlb,
	const mlib_s16 *src,
	mlib_s32 slb,
	mlib_s32 xsize,
	mlib_s32 ysize,
	mlib_s32 nchan,
	const mlib_s32 *alpha,
	const mlib_s32 *beta,
	const mlib_s32 shift)
{
	mlib_s32 i, j, nsize;
	mlib_s32 alpha0, alpha1, alpha2, alpha3;
	mlib_s32 xi0, xi1, xi2, xi3;
	__m128i *srcPtr, *dstPtr;
	mlib_s16 *dl = dst;
	mlib_s16 *sl = (mlib_s16 *)src;
	__m128i alphas, alphau, alphav;
	__m128i betas, betau, betav;
	mlib_s32 ialp, ival;
	mlib_s32 sdata;

	nsize = xsize * nchan;

	switch (nchan) {
	case 1:
		alpha0 = (alpha[0] & MASK);
		alphas = _mm_set1_epi16(alpha0);
		betas = _mm_set1_epi32(beta[0]);
		if ((((mlib_addr)dst | dlb |
			(mlib_addr)src | slb) & 0xf) == 0) {
			for (j = 0; j < ysize; j ++) {
				srcPtr = (__m128i *)sl;
				dstPtr = (__m128i *)dl;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i <= (nsize - 8); i += 8) {
					MLIB_S_IMAGESCALE_S16_S16(
						_mm_store_si128,
						_mm_load_si128);
				}
				for (; i < nsize; i++) {
					MLIB_C_IMAGESCALE_1(S16);
				}
				sl = (mlib_s16 *)((mlib_u8 *)sl + slb);
				dl = (mlib_s16 *)((mlib_u8 *)dl + dlb);
			}
		} else
		if ((((mlib_addr)src | slb) & 0xf) == 0) {
			for (j = 0; j < ysize; j ++) {
				srcPtr = (__m128i *)sl;
				dstPtr = (__m128i *)dl;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i <= (nsize - 8); i += 8) {
					MLIB_S_IMAGESCALE_S16_S16(
						_mm_storeu_si128,
						_mm_load_si128);
				}
				for (; i < nsize; i++) {
					MLIB_C_IMAGESCALE_1(S16);
				}

				sl = (mlib_s16 *)((mlib_u8 *)sl + slb);
				dl = (mlib_s16 *)((mlib_u8 *)dl + dlb);
			}
		} else {
			for (j = 0; j < ysize; j ++) {
				srcPtr = (__m128i *)sl;
				dstPtr = (__m128i *)dl;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i <= (nsize - 8); i += 8) {
					MLIB_S_IMAGESCALE_S16_S16(
						_mm_storeu_si128,
						_mm_loadu_si128);
				}
				for (; i < nsize; i++) {
					MLIB_C_IMAGESCALE_1(S16);
				}

				sl = (mlib_s16 *)((mlib_u8 *)sl + slb);
				dl = (mlib_s16 *)((mlib_u8 *)dl + dlb);
			}
		}
		break;
	case 2:
		alpha0 = (alpha[0] & MASK);
		alpha1 = ((alpha[1] & MASK) << 16) | alpha0;
		alphas = _mm_set1_epi32(alpha1);
		betas = _mm_set_epi32(
			beta[1], beta[0], beta[1], beta[0]);

		if ((((mlib_addr)dst | dlb |
			(mlib_addr)src | slb) & 0xf) == 0) {
			for (j = 0; j < ysize; j ++) {
				srcPtr = (__m128i *)sl;
				dstPtr = (__m128i *)dl;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i <= (nsize - 8); i += 8) {
					MLIB_S_IMAGESCALE_S16_S16(
						_mm_store_si128,
						_mm_load_si128);
				}
				for (; i < nsize; i += 2) {
					MLIB_C_IMAGESCALE_2(S16);
				}

				sl = (mlib_s16 *)((mlib_u8 *)sl + slb);
				dl = (mlib_s16 *)((mlib_u8 *)dl + dlb);
			}
		} else
		if ((((mlib_addr)src | slb) & 0xf) == 0) {
			for (j = 0; j < ysize; j ++) {
				srcPtr = (__m128i *)sl;
				dstPtr = (__m128i *)dl;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i <= (nsize - 8); i += 8) {
					MLIB_S_IMAGESCALE_S16_S16(
						_mm_storeu_si128,
						_mm_load_si128);
				}
				for (; i < nsize; i += 2) {
					MLIB_C_IMAGESCALE_2(S16);
				}

				sl = (mlib_s16 *)((mlib_u8 *)sl + slb);
				dl = (mlib_s16 *)((mlib_u8 *)dl + dlb);
			}
		} else {
			for (j = 0; j < ysize; j ++) {
				srcPtr = (__m128i *)sl;
				dstPtr = (__m128i *)dl;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i <= (nsize - 8); i += 8) {
					MLIB_S_IMAGESCALE_S16_S16(
						_mm_storeu_si128,
						_mm_loadu_si128);
				}
				for (; i < nsize; i += 2) {
					MLIB_C_IMAGESCALE_2(S16);
				}

				sl = (mlib_s16 *)((mlib_u8 *)sl + slb);
				dl = (mlib_s16 *)((mlib_u8 *)dl + dlb);
			}
		}
		break;
	case 3:
		alpha0 = (alpha[0] & MASK);
		alpha1 = (alpha[1] & MASK);
		alpha2 = (alpha[2] & MASK);
		alphas = _mm_set_epi16(alpha1, alpha0, alpha2,
			alpha1, alpha0, alpha2, alpha1, alpha0);
		alphau = _mm_set_epi16(alpha0, alpha2, alpha1,
			alpha0, alpha2, alpha1, alpha0, alpha2);
		alphav = _mm_set_epi16(alpha2, alpha1, alpha0,
			alpha2, alpha1, alpha0, alpha2, alpha1);
		betas = _mm_set_epi32(
			beta[0], beta[2], beta[1], beta[0]);
		betau = _mm_set_epi32(
			beta[1], beta[0], beta[2], beta[1]);
		betav = _mm_set_epi32(
			beta[2], beta[1], beta[0], beta[2]);

		if ((((mlib_addr)dst | dlb |
			(mlib_addr)src | slb) & 0xf) == 0) {
			for (j = 0; j < ysize; j ++) {
				srcPtr = (__m128i *)sl;
				dstPtr = (__m128i *)dl;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i <= (nsize - 24); i += 24) {
					MLIB_S_IMAGESCALE3_S16_S16(
						_mm_store_si128,
						_mm_load_si128);
				}
				for (; i < nsize; i += 3) {
					MLIB_C_IMAGESCALE_3(S16);
				}

				sl = (mlib_s16 *)((mlib_u8 *)sl + slb);
				dl = (mlib_s16 *)((mlib_u8 *)dl + dlb);
			}
		} else
		if ((((mlib_addr)src | slb) & 0xf) == 0) {
			for (j = 0; j < ysize; j ++) {
				srcPtr = (__m128i *)sl;
				dstPtr = (__m128i *)dl;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i <= (nsize - 24); i += 24) {
					MLIB_S_IMAGESCALE3_S16_S16(
						_mm_storeu_si128,
						_mm_load_si128);
				}
				for (; i < nsize; i += 3) {
					MLIB_C_IMAGESCALE_3(S16);
				}

				sl = (mlib_s16 *)((mlib_u8 *)sl + slb);
				dl = (mlib_s16 *)((mlib_u8 *)dl + dlb);
			}
		} else {
			for (j = 0; j < ysize; j ++) {
				srcPtr = (__m128i *)sl;
				dstPtr = (__m128i *)dl;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i <= (nsize - 24); i += 24) {
					MLIB_S_IMAGESCALE3_S16_S16(
						_mm_storeu_si128,
						_mm_loadu_si128);
				}
				for (; i < nsize; i += 3) {
					MLIB_C_IMAGESCALE_3(S16);
				}

				sl = (mlib_s16 *)((mlib_u8 *)sl + slb);
				dl = (mlib_s16 *)((mlib_u8 *)dl + dlb);
			}
		}

		break;
	case 4:
		alpha0 = alpha[0] & MASK;
		alpha1 = ((alpha[1] & MASK) << 16) | alpha0;
		alpha2 = alpha[2] & MASK;
		alpha3 = ((alpha[3] & MASK) << 16) | alpha2;
		alphas = _mm_set_epi32(alpha3, alpha1, alpha3, alpha1);
		betas = _mm_set_epi32(
			beta[3], beta[2], beta[1], beta[0]);

		if ((((mlib_addr)dst | dlb |
			(mlib_addr)src | slb) & 0xf) == 0) {
			for (j = 0; j < ysize; j ++) {
				srcPtr = (__m128i *)sl;
				dstPtr = (__m128i *)dl;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i <= (nsize - 8); i += 8) {
					MLIB_S_IMAGESCALE_S16_S16(
						_mm_store_si128,
						_mm_load_si128);
				}
				for (; i < nsize; i += 4) {
					MLIB_C_IMAGESCALE_4(S16);
				}

				sl = (mlib_s16 *)((mlib_u8 *)sl + slb);
				dl = (mlib_s16 *)((mlib_u8 *)dl + dlb);
			}
		} else
		if ((((mlib_addr)src | slb) & 0xf) == 0) {
			for (j = 0; j < ysize; j ++) {
				srcPtr = (__m128i *)sl;
				dstPtr = (__m128i *)dl;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i <= (nsize - 8); i += 8) {
					MLIB_S_IMAGESCALE_S16_S16(
						_mm_storeu_si128,
						_mm_load_si128);
				}
				for (; i < nsize; i += 4) {
					MLIB_C_IMAGESCALE_4(S16);
				}

				sl = (mlib_s16 *)((mlib_u8 *)sl + slb);
				dl = (mlib_s16 *)((mlib_u8 *)dl + dlb);
			}
		} else {
			for (j = 0; j < ysize; j ++) {
				srcPtr = (__m128i *)sl;
				dstPtr = (__m128i *)dl;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i <= (nsize - 8); i += 8) {
					MLIB_S_IMAGESCALE_S16_S16(
						_mm_storeu_si128,
						_mm_loadu_si128);
				}
				for (; i < nsize; i += 4) {
					MLIB_C_IMAGESCALE_4(S16);
				}

				sl = (mlib_s16 *)((mlib_u8 *)sl + slb);
				dl = (mlib_s16 *)((mlib_u8 *)dl + dlb);
			}
		}
		break;
	}

	return (MLIB_SUCCESS);
}
/* *********************************************************** */

#define	MLIB_S_IMAGESCALE_S16_U16(D_STORE, S_LOAD)	        \
	{                                                       \
		__m128i dsts, dsts0, dsts1;                         \
		__m128i srcs;                                       \
		__m128i part0, part1;                               \
		srcs  = S_LOAD((void *)srcPtr);                     \
		part0 = _mm_mullo_epi16(alphas, srcs);              \
		part1 = _mm_mulhi_epi16(alphas, srcs);              \
		dsts0 = _mm_unpacklo_epi16(part0, part1);           \
		dsts1 = _mm_unpackhi_epi16(part0, part1);           \
		dsts0 = _mm_srai_epi32(                             \
			_mm_add_epi32(dsts0, betas), shift);            \
		dsts1 = _mm_srai_epi32(                             \
			_mm_add_epi32(dsts1, betas), shift);            \
		dsts = _mm_packs_epi32(dsts0, dsts1);               \
		dsts = _mm_xor_si128(dsts, mask8000);               \
		D_STORE((void *)dstPtr, dsts);                      \
		srcPtr++;                                           \
		dstPtr++;                                           \
	}

/* *********************************************************** */

#define	MLIB_S_IMAGESCALE3_S16_U16(D_STORE, S_LOAD)           \
	{                                                       \
		__m128i dsts, dsts0, dsts1;                         \
		__m128i srcs;                                       \
		__m128i part0, part1;                               \
		srcs  = S_LOAD((void *)srcPtr);                     \
		part0 = _mm_mullo_epi16(alphas, srcs);              \
		part1 = _mm_mulhi_epi16(alphas, srcs);              \
		dsts0 = _mm_unpacklo_epi16(part0, part1);           \
		dsts1 = _mm_unpackhi_epi16(part0, part1);           \
		dsts0 = _mm_srai_epi32(                             \
			_mm_add_epi32(dsts0, betas), shift);            \
		dsts1 = _mm_srai_epi32(                             \
			_mm_add_epi32(dsts1, betau), shift);            \
		dsts = _mm_packs_epi32(dsts0, dsts1);               \
		dsts = _mm_xor_si128(dsts, mask8000);               \
		D_STORE((void *)dstPtr, dsts);                      \
		srcPtr++;                                           \
		dstPtr++;                                           \
		srcs  = S_LOAD((void *)srcPtr);                     \
		part0 = _mm_mullo_epi16(alphau, srcs);              \
		part1 = _mm_mulhi_epi16(alphau, srcs);              \
		dsts0 = _mm_unpacklo_epi16(part0, part1);           \
		dsts1 = _mm_unpackhi_epi16(part0, part1);           \
		dsts0 = _mm_srai_epi32(                             \
			_mm_add_epi32(dsts0, betav), shift);            \
		dsts1 = _mm_srai_epi32(                             \
			_mm_add_epi32(dsts1, betas), shift);            \
		dsts = _mm_packs_epi32(dsts0, dsts1);               \
		dsts = _mm_xor_si128(dsts, mask8000);               \
		D_STORE((void *)dstPtr, dsts);                      \
		srcPtr++;                                           \
		dstPtr++;                                           \
		srcs  = S_LOAD((void *)srcPtr);                     \
		part0 = _mm_mullo_epi16(alphav, srcs);              \
		part1 = _mm_mulhi_epi16(alphav, srcs);              \
		dsts0 = _mm_unpacklo_epi16(part0, part1);           \
		dsts1 = _mm_unpackhi_epi16(part0, part1);           \
		dsts0 = _mm_srai_epi32(                             \
			_mm_add_epi32(dsts0, betau), shift);            \
		dsts1 = _mm_srai_epi32(                             \
			_mm_add_epi32(dsts1, betav), shift);            \
		dsts = _mm_packs_epi32(dsts0, dsts1);               \
		dsts = _mm_xor_si128(dsts, mask8000);               \
		D_STORE((void *)dstPtr, dsts);                      \
		srcPtr++;                                           \
		dstPtr++;                                           \
	}

/* *********************************************************** */

mlib_status
mlib_s_ImageScale_s16_u16(
	mlib_u16 *dst,
	mlib_s32 dlb,
	const mlib_s16 *src,
	mlib_s32 slb,
	mlib_s32 xsize,
	mlib_s32 ysize,
	mlib_s32 nchan,
	const mlib_s32 *alpha,
	const mlib_s32 *beta,
	const mlib_s32 shift)
{
	mlib_s32 i, j, nsize;
	mlib_s32 alpha0, alpha1, alpha2, alpha3;
	mlib_s32 xi0, xi1, xi2, xi3;
	__m128i *srcPtr, *dstPtr;
	mlib_u16 *dl = dst;
	mlib_s16 *sl = (mlib_s16 *)src;
	__m128i alphas, alphau, alphav;
	__m128i betas, betau, betav;
	__m128i mask8000 = _mm_set1_epi32(0x80008000);
	mlib_s32 ialp, ival;
	mlib_s32 sdata;

	nsize = xsize * nchan;

	switch (nchan) {
	case 1:
		alpha0 = (alpha[0] & MASK);
		alphas = _mm_set1_epi16(alpha0);
		betas = _mm_set1_epi32(beta[0]);
		if ((((mlib_addr)dst | dlb |
			(mlib_addr)src | slb) & 0xf) == 0) {
			for (j = 0; j < ysize; j ++) {
				srcPtr = (__m128i *)sl;
				dstPtr = (__m128i *)dl;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i <= (nsize - 8); i += 8) {
					MLIB_S_IMAGESCALE_S16_U16(
						_mm_store_si128,
						_mm_load_si128);
				}
				for (; i < nsize; i++) {
					MLIB_C_IMAGESCALE_1(U16);
				}
				sl = (mlib_s16 *)((mlib_u8 *)sl + slb);
				dl = (mlib_u16 *)((mlib_u8 *)dl + dlb);
			}
		} else
		if ((((mlib_addr)src | slb) & 0xf) == 0) {
			for (j = 0; j < ysize; j ++) {
				srcPtr = (__m128i *)sl;
				dstPtr = (__m128i *)dl;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i <= (nsize - 8); i += 8) {
					MLIB_S_IMAGESCALE_S16_U16(
						_mm_storeu_si128,
						_mm_load_si128);
				}
				for (; i < nsize; i++) {
					MLIB_C_IMAGESCALE_1(U16);
				}

				sl = (mlib_s16 *)((mlib_u8 *)sl + slb);
				dl = (mlib_u16 *)((mlib_u8 *)dl + dlb);
			}
		} else {
			for (j = 0; j < ysize; j ++) {
				srcPtr = (__m128i *)sl;
				dstPtr = (__m128i *)dl;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i <= (nsize - 8); i += 8) {
					MLIB_S_IMAGESCALE_S16_U16(
						_mm_storeu_si128,
						_mm_loadu_si128);
				}
				for (; i < nsize; i++) {
					MLIB_C_IMAGESCALE_1(U16);
				}

				sl = (mlib_s16 *)((mlib_u8 *)sl + slb);
				dl = (mlib_u16 *)((mlib_u8 *)dl + dlb);
			}
		}
		break;
	case 2:
		alpha0 = (alpha[0] & MASK);
		alpha1 = ((alpha[1] & MASK) << 16) | alpha0;
		alphas = _mm_set1_epi32(alpha1);
		betas = _mm_set_epi32(
			beta[1], beta[0], beta[1], beta[0]);

		if ((((mlib_addr)dst | dlb |
			(mlib_addr)src | slb) & 0xf) == 0) {
			for (j = 0; j < ysize; j ++) {
				srcPtr = (__m128i *)sl;
				dstPtr = (__m128i *)dl;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i <= (nsize - 8); i += 8) {
					MLIB_S_IMAGESCALE_S16_U16(
						_mm_store_si128,
						_mm_load_si128);
				}
				for (; i < nsize; i += 2) {
					MLIB_C_IMAGESCALE_2(U16);
				}

				sl = (mlib_s16 *)((mlib_u8 *)sl + slb);
				dl = (mlib_u16 *)((mlib_u8 *)dl + dlb);
			}
		} else
		if ((((mlib_addr)src | slb) & 0xf) == 0) {
			for (j = 0; j < ysize; j ++) {
				srcPtr = (__m128i *)sl;
				dstPtr = (__m128i *)dl;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i <= (nsize - 8); i += 8) {
					MLIB_S_IMAGESCALE_S16_U16(
						_mm_storeu_si128,
						_mm_load_si128);
				}
				for (; i < nsize; i += 2) {
					MLIB_C_IMAGESCALE_2(U16);
				}

				sl = (mlib_s16 *)((mlib_u8 *)sl + slb);
				dl = (mlib_u16 *)((mlib_u8 *)dl + dlb);
			}
		} else {
			for (j = 0; j < ysize; j ++) {
				srcPtr = (__m128i *)sl;
				dstPtr = (__m128i *)dl;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i <= (nsize - 8); i += 8) {
					MLIB_S_IMAGESCALE_S16_U16(
						_mm_storeu_si128,
						_mm_loadu_si128);
				}
				for (; i < nsize; i += 2) {
					MLIB_C_IMAGESCALE_2(U16);
				}

				sl = (mlib_s16 *)((mlib_u8 *)sl + slb);
				dl = (mlib_u16 *)((mlib_u8 *)dl + dlb);
			}
		}
		break;
	case 3:
		alpha0 = (alpha[0] & MASK);
		alpha1 = (alpha[1] & MASK);
		alpha2 = (alpha[2] & MASK);
		alphas = _mm_set_epi16(alpha1, alpha0, alpha2,
			alpha1, alpha0, alpha2, alpha1, alpha0);
		alphau = _mm_set_epi16(alpha0, alpha2, alpha1,
			alpha0, alpha2, alpha1, alpha0, alpha2);
		alphav = _mm_set_epi16(alpha2, alpha1, alpha0,
			alpha2, alpha1, alpha0, alpha2, alpha1);
		betas = _mm_set_epi32(
			beta[0], beta[2], beta[1], beta[0]);
		betau = _mm_set_epi32(
			beta[1], beta[0], beta[2], beta[1]);
		betav = _mm_set_epi32(
			beta[2], beta[1], beta[0], beta[2]);

		if ((((mlib_addr)dst | dlb |
			(mlib_addr)src | slb) & 0xf) == 0) {
			for (j = 0; j < ysize; j ++) {
				srcPtr = (__m128i *)sl;
				dstPtr = (__m128i *)dl;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i <= (nsize - 24); i += 24) {
					MLIB_S_IMAGESCALE3_S16_U16(
						_mm_store_si128,
						_mm_load_si128);
				}
				for (; i < nsize; i += 3) {
					MLIB_C_IMAGESCALE_3(U16);
				}

				sl = (mlib_s16 *)((mlib_u8 *)sl + slb);
				dl = (mlib_u16 *)((mlib_u8 *)dl + dlb);
			}
		} else
		if ((((mlib_addr)src | slb) & 0xf) == 0) {
			for (j = 0; j < ysize; j ++) {
				srcPtr = (__m128i *)sl;
				dstPtr = (__m128i *)dl;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i <= (nsize - 24); i += 24) {
					MLIB_S_IMAGESCALE3_S16_U16(
						_mm_storeu_si128,
						_mm_load_si128);
				}
				for (; i < nsize; i += 3) {
					MLIB_C_IMAGESCALE_3(U16);
				}

				sl = (mlib_s16 *)((mlib_u8 *)sl + slb);
				dl = (mlib_u16 *)((mlib_u8 *)dl + dlb);
			}
		} else {
			for (j = 0; j < ysize; j ++) {
				srcPtr = (__m128i *)sl;
				dstPtr = (__m128i *)dl;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i <= (nsize - 24); i += 24) {
					MLIB_S_IMAGESCALE3_S16_U16(
						_mm_storeu_si128,
						_mm_loadu_si128);
				}
				for (; i < nsize; i += 3) {
					MLIB_C_IMAGESCALE_3(U16);
				}

				sl = (mlib_s16 *)((mlib_u8 *)sl + slb);
				dl = (mlib_u16 *)((mlib_u8 *)dl + dlb);
			}
		}

		break;
	case 4:
		alpha0 = alpha[0] & MASK;
		alpha1 = ((alpha[1] & MASK) << 16) | alpha0;
		alpha2 = alpha[2] & MASK;
		alpha3 = ((alpha[3] & MASK) << 16) | alpha2;
		alphas = _mm_set_epi32(alpha3, alpha1, alpha3, alpha1);
		betas = _mm_set_epi32(
			beta[3], beta[2], beta[1], beta[0]);

		if ((((mlib_addr)dst | dlb |
			(mlib_addr)src | slb) & 0xf) == 0) {
			for (j = 0; j < ysize; j ++) {
				srcPtr = (__m128i *)sl;
				dstPtr = (__m128i *)dl;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i <= (nsize - 8); i += 8) {
					MLIB_S_IMAGESCALE_S16_U16(
						_mm_store_si128,
						_mm_load_si128);
				}
				for (; i < nsize; i += 4) {
					MLIB_C_IMAGESCALE_4(U16);
				}

				sl = (mlib_s16 *)((mlib_u8 *)sl + slb);
				dl = (mlib_u16 *)((mlib_u8 *)dl + dlb);
			}
		} else
		if ((((mlib_addr)src | slb) & 0xf) == 0) {
			for (j = 0; j < ysize; j ++) {
				srcPtr = (__m128i *)sl;
				dstPtr = (__m128i *)dl;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i <= (nsize - 8); i += 8) {
					MLIB_S_IMAGESCALE_S16_U16(
						_mm_storeu_si128,
						_mm_load_si128);
				}
				for (; i < nsize; i += 4) {
					MLIB_C_IMAGESCALE_4(U16);
				}

				sl = (mlib_s16 *)((mlib_u8 *)sl + slb);
				dl = (mlib_u16 *)((mlib_u8 *)dl + dlb);
			}
		} else {
			for (j = 0; j < ysize; j ++) {
				srcPtr = (__m128i *)sl;
				dstPtr = (__m128i *)dl;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i <= (nsize - 8); i += 8) {
					MLIB_S_IMAGESCALE_S16_U16(
						_mm_storeu_si128,
						_mm_loadu_si128);
				}
				for (; i < nsize; i += 4) {
					MLIB_C_IMAGESCALE_4(U16);
				}

				sl = (mlib_s16 *)((mlib_u8 *)sl + slb);
				dl = (mlib_u16 *)((mlib_u8 *)dl + dlb);
			}
		}
		break;
	}

	return (MLIB_SUCCESS);
}
/* *********************************************************** */

#define	MLIB_S_IMAGESCALE_S16_S32(D_STORE, S_LOAD)	        \
	{                                                       \
		__m128i dsts0, dsts1;                               \
		__m128i srcs;                                       \
		__m128i part0, part1;                               \
		srcs  = S_LOAD((void *)srcPtr);                     \
		part0 = _mm_mullo_epi16(alphas, srcs);              \
		part1 = _mm_mulhi_epi16(alphas, srcs);              \
		dsts0 = _mm_unpacklo_epi16(part0, part1);           \
		dsts1 = _mm_unpackhi_epi16(part0, part1);           \
		dsts0 = _mm_srai_epi32(                             \
			_mm_add_epi32(dsts0, betas), shift);            \
		dsts1 = _mm_srai_epi32(                             \
			_mm_add_epi32(dsts1, betas), shift);            \
		D_STORE((void *)dstPtr, dsts0);                     \
		dstPtr++;                                           \
		D_STORE((void *)dstPtr, dsts1);                     \
		dstPtr++;                                           \
		srcPtr++;                                           \
	}

/* *********************************************************** */

#define	MLIB_S_IMAGESCALE3_S16_S32(D_STORE, S_LOAD)           \
	{                                                       \
		__m128i dsts0, dsts1;                               \
		__m128i srcs;                                       \
		__m128i part0, part1;                               \
		srcs  = S_LOAD((void *)srcPtr);                     \
		part0 = _mm_mullo_epi16(alphas, srcs);              \
		part1 = _mm_mulhi_epi16(alphas, srcs);              \
		dsts0 = _mm_unpacklo_epi16(part0, part1);           \
		dsts1 = _mm_unpackhi_epi16(part0, part1);           \
		dsts0 = _mm_srai_epi32(                             \
			_mm_add_epi32(dsts0, betas), shift);            \
		dsts1 = _mm_srai_epi32(                             \
			_mm_add_epi32(dsts1, betau), shift);            \
		D_STORE((void *)dstPtr, dsts0);                     \
		dstPtr++;                                           \
		D_STORE((void *)dstPtr, dsts1);                     \
		dstPtr++;                                           \
		srcPtr++;                                           \
		srcs  = S_LOAD((void *)srcPtr);                     \
		part0 = _mm_mullo_epi16(alphau, srcs);              \
		part1 = _mm_mulhi_epi16(alphau, srcs);              \
		dsts0 = _mm_unpacklo_epi16(part0, part1);           \
		dsts1 = _mm_unpackhi_epi16(part0, part1);           \
		dsts0 = _mm_srai_epi32(                             \
			_mm_add_epi32(dsts0, betav), shift);            \
		dsts1 = _mm_srai_epi32(                             \
			_mm_add_epi32(dsts1, betas), shift);            \
		D_STORE((void *)dstPtr, dsts0);                     \
		dstPtr++;                                           \
		D_STORE((void *)dstPtr, dsts1);                     \
		dstPtr++;                                           \
		srcPtr++;                                           \
		srcs  = S_LOAD((void *)srcPtr);                     \
		part0 = _mm_mullo_epi16(alphav, srcs);              \
		part1 = _mm_mulhi_epi16(alphav, srcs);              \
		dsts0 = _mm_unpacklo_epi16(part0, part1);           \
		dsts1 = _mm_unpackhi_epi16(part0, part1);           \
		dsts0 = _mm_srai_epi32(                             \
			_mm_add_epi32(dsts0, betau), shift);            \
		dsts1 = _mm_srai_epi32(                             \
			_mm_add_epi32(dsts1, betav), shift);            \
		D_STORE((void *)dstPtr, dsts0);                     \
		dstPtr++;                                           \
		D_STORE((void *)dstPtr, dsts1);                     \
		dstPtr++;                                           \
		srcPtr++;                                           \
	}

/* *********************************************************** */

mlib_status
mlib_s_ImageScale_s16_s32(
	mlib_s32 *dst,
	mlib_s32 dlb,
	const mlib_s16 *src,
	mlib_s32 slb,
	mlib_s32 xsize,
	mlib_s32 ysize,
	mlib_s32 nchan,
	const mlib_s32 *alpha,
	const mlib_s32 *beta,
	const mlib_s32 shift)
{
	mlib_s32 i, j, nsize;
	mlib_s32 alpha0, alpha1, alpha2, alpha3;
	mlib_s32 xi0, xi1, xi2, xi3;
	__m128i *srcPtr, *dstPtr;
	mlib_s32 *dl = dst;
	mlib_s16 *sl = (mlib_s16 *)src;
	__m128i alphas, alphau, alphav;
	__m128i betas, betau, betav;
	mlib_s32 ialp, ival;
	mlib_s32 sdata;

	nsize = xsize * nchan;

	switch (nchan) {
	case 1:
		alpha0 = (alpha[0] & MASK);
		alphas = _mm_set1_epi16(alpha0);
		betas = _mm_set1_epi32(beta[0]);
		if ((((mlib_addr)dst | dlb |
			(mlib_addr)src | slb) & 0xf) == 0) {
			for (j = 0; j < ysize; j ++) {
				srcPtr = (__m128i *)sl;
				dstPtr = (__m128i *)dl;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i <= (nsize - 8); i += 8) {
					MLIB_S_IMAGESCALE_S16_S32(
						_mm_store_si128,
						_mm_load_si128);
				}
				for (; i < nsize; i++) {
					MLIB_C_IMAGESCALE_1(S32);
				}
				sl = (mlib_s16 *)((mlib_u8 *)sl + slb);
				dl = (mlib_s32 *)((mlib_u8 *)dl + dlb);
			}
		} else
		if ((((mlib_addr)src | slb) & 0xf) == 0) {
			for (j = 0; j < ysize; j ++) {
				srcPtr = (__m128i *)sl;
				dstPtr = (__m128i *)dl;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i <= (nsize - 8); i += 8) {
					MLIB_S_IMAGESCALE_S16_S32(
						_mm_storeu_si128,
						_mm_load_si128);
				}
				for (; i < nsize; i++) {
					MLIB_C_IMAGESCALE_1(S32);
				}

				sl = (mlib_s16 *)((mlib_u8 *)sl + slb);
				dl = (mlib_s32 *)((mlib_u8 *)dl + dlb);
			}
		} else {
			for (j = 0; j < ysize; j ++) {
				srcPtr = (__m128i *)sl;
				dstPtr = (__m128i *)dl;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i <= (nsize - 8); i += 8) {
					MLIB_S_IMAGESCALE_S16_S32(
						_mm_storeu_si128,
						_mm_loadu_si128);
				}
				for (; i < nsize; i++) {
					MLIB_C_IMAGESCALE_1(S32);
				}

				sl = (mlib_s16 *)((mlib_u8 *)sl + slb);
				dl = (mlib_s32 *)((mlib_u8 *)dl + dlb);
			}
		}
		break;
	case 2:
		alpha0 = (alpha[0] & MASK);
		alpha1 = ((alpha[1] & MASK) << 16) | alpha0;
		alphas = _mm_set1_epi32(alpha1);
		betas = _mm_set_epi32(
			beta[1], beta[0], beta[1], beta[0]);

		if ((((mlib_addr)dst | dlb |
			(mlib_addr)src | slb) & 0xf) == 0) {
			for (j = 0; j < ysize; j ++) {
				srcPtr = (__m128i *)sl;
				dstPtr = (__m128i *)dl;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i <= (nsize - 8); i += 8) {
					MLIB_S_IMAGESCALE_S16_S32(
						_mm_store_si128,
						_mm_load_si128);
				}
				for (; i < nsize; i += 2) {
					MLIB_C_IMAGESCALE_2(S32);
				}

				sl = (mlib_s16 *)((mlib_u8 *)sl + slb);
				dl = (mlib_s32 *)((mlib_u8 *)dl + dlb);
			}
		} else
		if ((((mlib_addr)src | slb) & 0xf) == 0) {
			for (j = 0; j < ysize; j ++) {
				srcPtr = (__m128i *)sl;
				dstPtr = (__m128i *)dl;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i <= (nsize - 8); i += 8) {
					MLIB_S_IMAGESCALE_S16_S32(
						_mm_storeu_si128,
						_mm_load_si128);
				}
				for (; i < nsize; i += 2) {
					MLIB_C_IMAGESCALE_2(S32);
				}

				sl = (mlib_s16 *)((mlib_u8 *)sl + slb);
				dl = (mlib_s32 *)((mlib_u8 *)dl + dlb);
			}
		} else {
			for (j = 0; j < ysize; j ++) {
				srcPtr = (__m128i *)sl;
				dstPtr = (__m128i *)dl;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i <= (nsize - 8); i += 8) {
					MLIB_S_IMAGESCALE_S16_S32(
						_mm_storeu_si128,
						_mm_loadu_si128);
				}
				for (; i < nsize; i += 2) {
					MLIB_C_IMAGESCALE_2(S32);
				}

				sl = (mlib_s16 *)((mlib_u8 *)sl + slb);
				dl = (mlib_s32 *)((mlib_u8 *)dl + dlb);
			}
		}
		break;
	case 3:
		alpha0 = (alpha[0] & MASK);
		alpha1 = (alpha[1] & MASK);
		alpha2 = (alpha[2] & MASK);
		alphas = _mm_set_epi16(alpha1, alpha0, alpha2,
			alpha1, alpha0, alpha2, alpha1, alpha0);
		alphau = _mm_set_epi16(alpha0, alpha2, alpha1,
			alpha0, alpha2, alpha1, alpha0, alpha2);
		alphav = _mm_set_epi16(alpha2, alpha1, alpha0,
			alpha2, alpha1, alpha0, alpha2, alpha1);
		betas = _mm_set_epi32(
			beta[0], beta[2], beta[1], beta[0]);
		betau = _mm_set_epi32(
			beta[1], beta[0], beta[2], beta[1]);
		betav = _mm_set_epi32(
			beta[2], beta[1], beta[0], beta[2]);

		if ((((mlib_addr)dst | dlb |
			(mlib_addr)src | slb) & 0xf) == 0) {
			for (j = 0; j < ysize; j ++) {
				srcPtr = (__m128i *)sl;
				dstPtr = (__m128i *)dl;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i <= (nsize - 24); i += 24) {
					MLIB_S_IMAGESCALE3_S16_S32(
						_mm_store_si128,
						_mm_load_si128);
				}
				for (; i < nsize; i += 3) {
					MLIB_C_IMAGESCALE_3(S32);
				}

				sl = (mlib_s16 *)((mlib_u8 *)sl + slb);
				dl = (mlib_s32 *)((mlib_u8 *)dl + dlb);
			}
		} else
		if ((((mlib_addr)src | slb) & 0xf) == 0) {
			for (j = 0; j < ysize; j ++) {
				srcPtr = (__m128i *)sl;
				dstPtr = (__m128i *)dl;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i <= (nsize - 24); i += 24) {
					MLIB_S_IMAGESCALE3_S16_S32(
						_mm_storeu_si128,
						_mm_load_si128);
				}
				for (; i < nsize; i += 3) {
					MLIB_C_IMAGESCALE_3(S32);
				}

				sl = (mlib_s16 *)((mlib_u8 *)sl + slb);
				dl = (mlib_s32 *)((mlib_u8 *)dl + dlb);
			}
		} else {
			for (j = 0; j < ysize; j ++) {
				srcPtr = (__m128i *)sl;
				dstPtr = (__m128i *)dl;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i <= (nsize - 24); i += 24) {
					MLIB_S_IMAGESCALE3_S16_S32(
						_mm_storeu_si128,
						_mm_loadu_si128);
				}
				for (; i < nsize; i += 3) {
					MLIB_C_IMAGESCALE_3(S32);
				}

				sl = (mlib_s16 *)((mlib_u8 *)sl + slb);
				dl = (mlib_s32 *)((mlib_u8 *)dl + dlb);
			}
		}

		break;
	case 4:
		alpha0 = alpha[0] & MASK;
		alpha1 = ((alpha[1] & MASK) << 16) | alpha0;
		alpha2 = alpha[2] & MASK;
		alpha3 = ((alpha[3] & MASK) << 16) | alpha2;
		alphas = _mm_set_epi32(alpha3, alpha1, alpha3, alpha1);
		betas = _mm_set_epi32(
			beta[3], beta[2], beta[1], beta[0]);

		if ((((mlib_addr)dst | dlb |
			(mlib_addr)src | slb) & 0xf) == 0) {
			for (j = 0; j < ysize; j ++) {
				srcPtr = (__m128i *)sl;
				dstPtr = (__m128i *)dl;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i <= (nsize - 8); i += 8) {
					MLIB_S_IMAGESCALE_S16_S32(
						_mm_store_si128,
						_mm_load_si128);
				}
				for (; i < nsize; i += 4) {
					MLIB_C_IMAGESCALE_4(S32);
				}

				sl = (mlib_s16 *)((mlib_u8 *)sl + slb);
				dl = (mlib_s32 *)((mlib_u8 *)dl + dlb);
			}
		} else
		if ((((mlib_addr)src | slb) & 0xf) == 0) {
			for (j = 0; j < ysize; j ++) {
				srcPtr = (__m128i *)sl;
				dstPtr = (__m128i *)dl;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i <= (nsize - 8); i += 8) {
					MLIB_S_IMAGESCALE_S16_S32(
						_mm_storeu_si128,
						_mm_load_si128);
				}
				for (; i < nsize; i += 4) {
					MLIB_C_IMAGESCALE_4(S32);
				}

				sl = (mlib_s16 *)((mlib_u8 *)sl + slb);
				dl = (mlib_s32 *)((mlib_u8 *)dl + dlb);
			}
		} else {
			for (j = 0; j < ysize; j ++) {
				srcPtr = (__m128i *)sl;
				dstPtr = (__m128i *)dl;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i <= (nsize - 8); i += 8) {
					MLIB_S_IMAGESCALE_S16_S32(
						_mm_storeu_si128,
						_mm_loadu_si128);
				}
				for (; i < nsize; i += 4) {
					MLIB_C_IMAGESCALE_4(S32);
				}

				sl = (mlib_s16 *)((mlib_u8 *)sl + slb);
				dl = (mlib_s32 *)((mlib_u8 *)dl + dlb);
			}
		}
		break;
	}

	return (MLIB_SUCCESS);
}
/* *********************************************************** */
