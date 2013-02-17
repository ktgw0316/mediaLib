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
 * Copyright 2006 Sun Microsystems, Inc.  All rights reserved.
 * Use is subject to license terms.
 */

#pragma ident "@(#)mlib_s_ImageSqr_Fp.c	9.3	07/11/05 SMI"

/*
 * FUNCTION
 *	mlib_ImageSqr_Fp	- square of an image
 *	mlib_ImageSqr_Fp_Inp	- square of an image
 *
 * SYNOPSIS
 *	mlib_status mlib_ImageSqr_Fp(mlib_image       *dst,
 *	                             const mlib_image *src);
 *	mlib_status mlib_ImageSqr_Fp_Inp(mlib_image *srcdst);
 *
 * ARGUMENT
 *	dst     pointer to output image
 *	src     pointer to input image
 *	srcdst  pointer to input and output image
 *
 * RESTRICTION
 *	src, srcdst, and dst must be the same type, the same size,
 *	and the same number of channels.
 *	They can have 1, 2, 3 or 4 channels.
 *	They can be in MLIB_FLOAT, or MLIB_DOUBLE data type.
 *
 * DESCRIPTION
 *	dst[x][y][c] = src[x][y][c] * src[x][y][c]
 *	srcdst[x][y][c] = srcdst[x][y][c] * srcdst[x][y][c]
 */

#ifdef __SUNPRO_C
#include <sunmedia_intrin.h>
#else
#include <emmintrin.h>
#endif

#include <mlib_image.h>
#include <mlib_ImageCheck.h>
#include <mlib_SysMath.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageSqr_Fp = __mlib_ImageSqr_Fp
#pragma weak mlib_ImageSqr_Fp_Inp = __mlib_ImageSqr_Fp_Inp

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */

__typeof__(__mlib_ImageSqr_Fp) mlib_ImageSqr_Fp
	__attribute__((weak, alias("__mlib_ImageSqr_Fp")));
__typeof__(__mlib_ImageSqr_Fp_Inp) mlib_ImageSqr_Fp_Inp
	__attribute__((weak, alias("__mlib_ImageSqr_Fp_Inp")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	MLIB_S_IMAGESQR_F32(D_STORE, S_LOAD)			\
	{							\
		__m128 dsts, srcs;				\
		srcs = S_LOAD((void *)&(src[i]));		\
		dsts = _mm_mul_ps(srcs, srcs);			\
		D_STORE((void *)&(dst[i]), dsts);		\
	}

#define	MLIB_S_IMAGESQR_D64(D_STORE, S_LOAD)			\
	{							\
		__m128d dsts, srcs;				\
		srcs = S_LOAD((void *)&(src[i]));		\
		dsts = _mm_mul_pd(srcs, srcs);			\
		D_STORE((void *)&(dst[i]), dsts);		\
	}

/* *********************************************************** */

static void
mlib_s_ImageSqr_Fp_f32(
	mlib_f32 *dst,
	mlib_s32 dlb,
	const mlib_f32 *src,
	mlib_s32 slb,
	mlib_s32 xsize,
	mlib_s32 ysize)
{
	mlib_s32 i, j;

	if ((((mlib_addr)dst | dlb |
		(mlib_addr)src | slb) & 0xf) == 0) {
		for (j = 0; j < ysize; j ++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i <= (xsize - 4); i += 4) {
				MLIB_S_IMAGESQR_F32(
					_mm_store_ps,
					_mm_load_ps);
			}
			for (; i < xsize; i ++) {
				dst[i] =  src[i] * src[i];
			}
			src  = (mlib_f32 *)((mlib_u8 *)src + slb);
			dst  = (mlib_f32 *)((mlib_u8 *)dst + dlb);
		}
	} else
	if ((((mlib_addr)src | slb) & 0xf) == 0) {
		for (j = 0; j < ysize; j ++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i <= (xsize - 4); i += 4) {
				MLIB_S_IMAGESQR_F32(
					_mm_storeu_ps,
					_mm_load_ps);
			}
			for (; i < xsize; i ++) {
				dst[i] =  src[i] * src[i];
			}
			src  = (mlib_f32 *)((mlib_u8 *)src + slb);
			dst  = (mlib_f32 *)((mlib_u8 *)dst + dlb);
		}
	} else {
		for (j = 0; j < ysize; j ++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i <= (xsize - 4); i += 4) {
				MLIB_S_IMAGESQR_F32(
					_mm_storeu_ps,
					_mm_loadu_ps);
			}
			for (; i < xsize; i ++) {
				dst[i] =  src[i] * src[i];
			}
			src  = (mlib_f32 *)((mlib_u8 *)src + slb);
			dst  = (mlib_f32 *)((mlib_u8 *)dst + dlb);
		}
	}
}

/* *********************************************************** */

static void
mlib_s_ImageSqr_Fp_d64(
	mlib_d64 *dst,
	mlib_s32 dlb,
	const mlib_d64 *src,
	mlib_s32 slb,
	mlib_s32 xsize,
	mlib_s32 ysize)
{
	mlib_s32 i, j;

	if ((((mlib_addr)dst | dlb |
		(mlib_addr)src | slb) & 0xf) == 0) {
		for (j = 0; j < ysize; j ++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i <= (xsize - 2); i += 2) {
				MLIB_S_IMAGESQR_D64(
					_mm_store_pd,
					_mm_load_pd);
			}
			for (; i < xsize; i ++) {
				dst[i] =  src[i] * src[i];
			}
			src  = (mlib_d64 *)((mlib_u8 *)src + slb);
			dst  = (mlib_d64 *)((mlib_u8 *)dst + dlb);
		}
	} else
	if ((((mlib_addr)src | slb) & 0xf) == 0) {
		for (j = 0; j < ysize; j ++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i <= (xsize - 2); i += 2) {
				MLIB_S_IMAGESQR_D64(
					_mm_storeu_pd,
					_mm_load_pd);
			}
			for (; i < xsize; i ++) {
				dst[i] =  src[i] * src[i];
			}
			src  = (mlib_d64 *)((mlib_u8 *)src + slb);
			dst  = (mlib_d64 *)((mlib_u8 *)dst + dlb);
		}
	} else {
		for (j = 0; j < ysize; j ++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i <= (xsize - 2); i += 2) {
				MLIB_S_IMAGESQR_D64(
					_mm_storeu_pd,
					_mm_loadu_pd);
			}
			for (; i < xsize; i ++) {
				dst[i] =  src[i] * src[i];
			}
			src  = (mlib_d64 *)((mlib_u8 *)src + slb);
			dst  = (mlib_d64 *)((mlib_u8 *)dst + dlb);
		}
	}
}

/* *********************************************************** */

mlib_status
__mlib_ImageSqr_Fp(
	mlib_image *dst,
	const mlib_image *src)
{
	mlib_type dtype;
	mlib_s32 slb, dlb, xsize, ysize, nchan;
	void *sa, *da;

	MLIB_IMAGE_CHECK(dst);
	MLIB_IMAGE_CHECK(src);
	MLIB_IMAGE_FULL_EQUAL(dst, src);

	dtype = mlib_ImageGetType(dst);
	nchan = mlib_ImageGetChannels(dst);
	xsize = mlib_ImageGetWidth(dst);
	ysize = mlib_ImageGetHeight(dst);
	slb = mlib_ImageGetStride(src);
	dlb = mlib_ImageGetStride(dst);
	sa = mlib_ImageGetData(src);
	da = mlib_ImageGetData(dst);

	xsize *= nchan;

	if (dtype == MLIB_FLOAT) {
		mlib_s_ImageSqr_Fp_f32(da, dlb, sa, slb, xsize, ysize);
		return (MLIB_SUCCESS);
	} else
	if (dtype == MLIB_DOUBLE) {
		mlib_s_ImageSqr_Fp_d64(da, dlb, sa, slb, xsize, ysize);
		return (MLIB_SUCCESS);
	}

	return (MLIB_FAILURE);
}

/* *********************************************************** */

mlib_status
__mlib_ImageSqr_Fp_Inp(
	mlib_image *srcdst)
{
	return (__mlib_ImageSqr_Fp(srcdst, srcdst));
}

/* *********************************************************** */
