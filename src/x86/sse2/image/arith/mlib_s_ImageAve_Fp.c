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

#pragma ident	"@(#)mlib_s_ImageAve_Fp.c	9.3	07/11/05 SMI"

/*
 * FUNCTION
 *	mlib_ImageAve_Fp	- average of two images
 *	mlib_ImageAve_Fp_Inp	- average of two images
 *
 * SYNOPSIS
 *	mlib_status mlib_ImageAve_Fp(mlib_image       *dst,
 *	                             const mlib_image *src1,
 *                                   const mlib_image *src2);
 *	mlib_status mlib_ImageAve_Fp_Inp(mlib_image       *src1dst,
 *	                                 const mlib_image *src2);
 *
 * ARGUMENT
 *	dst     pointer to output image
 *	src1    pointer to first input image
 *	src2    pointer to second input image
 *	src1dst	pointer to first input and output image
 *
 * RESTRICTION
 *	src1, src2, src1dst, and dst must be the same type,
 *	the same size, and the same number of channels.
 *	They can have 1, 2, 3, or 4 channels.
 *	They can be in MLIB_FLOAT or MLIB_DOUBLE data type.
 *
 * DESCRIPTION
 *	dst[x][y][c] = (src1[x][y][c] + src2[x][y][c]) / 2
 *	src1dst[x][y][c] = (src1dst[x][y][c] + src2[x][y][c]) / 2
 */

#ifdef __SUNPRO_C
#include <sunmedia_intrin.h>
#else
#include <emmintrin.h>
#endif

#include <mlib_image.h>
#include <mlib_ImageCheck.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageAve_Fp = __mlib_ImageAve_Fp
#pragma weak mlib_ImageAve_Fp_Inp = __mlib_ImageAve_Fp_Inp

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */

__typeof__(__mlib_ImageAve_Fp) mlib_ImageAve_Fp
	__attribute__((weak, alias("__mlib_ImageAve_Fp")));
__typeof__(__mlib_ImageAve_Fp_Inp) mlib_ImageAve_Fp_Inp
	__attribute__((weak, alias("__mlib_ImageAve_Fp_Inp")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	MLIB_S_IMAGEAVE_F32(D_STORE, S1_LOAD, S2_LOAD)		\
	{							\
		__m128 dsts, srcs1, srcs2, sums;		\
		srcs1 = S1_LOAD((void *)&(src1[i]));		\
		srcs2 = S2_LOAD((void *)&(src2[i]));		\
		sums = _mm_add_ps(srcs1, srcs2);		\
		dsts = _mm_mul_ps(half, sums);			\
		D_STORE((void *)&(dst[i]), dsts);		\
	}

#define	MLIB_S_IMAGEAVE_D64(D_STORE, S1_LOAD, S2_LOAD)		\
	{							\
		__m128d dsts, srcs1, srcs2, sums;		\
		srcs1 = S1_LOAD((void *)&(src1[i]));		\
		srcs2 = S2_LOAD((void *)&(src2[i]));		\
		sums = _mm_add_pd(srcs1, srcs2);		\
		dsts = _mm_mul_pd(half, sums);			\
		D_STORE((void *)&(dst[i]), dsts);		\
	}

/* *********************************************************** */

#define	MLIB_C_IMAGEAVE(a, b) (0.5 * ((a) + (b)))

/* *********************************************************** */

static void
mlib_s_ImageAve_Fp_f32(
	mlib_f32 *dst,
	mlib_s32 dlb,
	const mlib_f32 *src1,
	mlib_s32 slb1,
	const mlib_f32 *src2,
	mlib_s32 slb2,
	mlib_s32 xsize,
	mlib_s32 ysize)
{
	mlib_s32 i, j;
	__m128 half;

	half = _mm_set1_ps(0.5);
	if ((((mlib_addr)dst | dlb |
		(mlib_addr)src1 | slb1 |
		(mlib_addr)src2 | slb2) & 0xf) == 0) {
		for (j = 0; j < ysize; j ++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i <= (xsize - 4); i += 4) {
				MLIB_S_IMAGEAVE_F32(
					_mm_store_ps,
					_mm_load_ps,
					_mm_load_ps);
			}
			for (; i < xsize; i ++) {
				dst[i] = MLIB_C_IMAGEAVE(src1[i], src2[i]);
			}
			src1 = (mlib_f32 *)((mlib_u8 *)src1 + slb1);
			src2 = (mlib_f32 *)((mlib_u8 *)src2 + slb2);
			dst  = (mlib_f32 *)((mlib_u8 *)dst + dlb);
		}
	} else
	if ((((mlib_addr)src1 | slb1 |
		(mlib_addr)src2 | slb2) & 0xf) == 0) {
		for (j = 0; j < ysize; j ++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i <= (xsize - 4); i += 4) {
				MLIB_S_IMAGEAVE_F32(
					_mm_storeu_ps,
					_mm_load_ps,
					_mm_load_ps);
			}
			for (; i < xsize; i ++) {
				dst[i] = MLIB_C_IMAGEAVE(src1[i], src2[i]);
			}
			src1 = (mlib_f32 *)((mlib_u8 *)src1 + slb1);
			src2 = (mlib_f32 *)((mlib_u8 *)src2 + slb2);
			dst  = (mlib_f32 *)((mlib_u8 *)dst + dlb);
		}
	} else {
		for (j = 0; j < ysize; j ++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i <= (xsize - 4); i += 4) {
				MLIB_S_IMAGEAVE_F32(
					_mm_storeu_ps,
					_mm_loadu_ps,
					_mm_loadu_ps);
			}
			for (; i < xsize; i ++) {
				dst[i] = MLIB_C_IMAGEAVE(src1[i], src2[i]);
			}
			src1 = (mlib_f32 *)((mlib_u8 *)src1 + slb1);
			src2 = (mlib_f32 *)((mlib_u8 *)src2 + slb2);
			dst  = (mlib_f32 *)((mlib_u8 *)dst + dlb);
		}
	}
}

/* *********************************************************** */

static void
mlib_s_ImageAve_Fp_d64(
	mlib_d64 *dst,
	mlib_s32 dlb,
	const mlib_d64 *src1,
	mlib_s32 slb1,
	const mlib_d64 *src2,
	mlib_s32 slb2,
	mlib_s32 xsize,
	mlib_s32 ysize)
{
	mlib_s32 i, j;
	__m128d half;

	half = _mm_set1_pd(0.5);
	if ((((mlib_addr)dst | dlb |
		(mlib_addr)src1 | slb1 |
		(mlib_addr)src2 | slb2) & 0xf) == 0) {
		for (j = 0; j < ysize; j ++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i <= (xsize - 2); i += 2) {
				MLIB_S_IMAGEAVE_D64(
					_mm_store_pd,
					_mm_load_pd,
					_mm_load_pd);
			}
			for (; i < xsize; i ++) {
				dst[i] = MLIB_C_IMAGEAVE(src1[i], src2[i]);
			}
			src1 = (mlib_d64 *)((mlib_u8 *)src1 + slb1);
			src2 = (mlib_d64 *)((mlib_u8 *)src2 + slb2);
			dst  = (mlib_d64 *)((mlib_u8 *)dst + dlb);
		}
	} else
	if ((((mlib_addr)src1 | slb1 |
		(mlib_addr)src2 | slb2) & 0xf) == 0) {
		for (j = 0; j < ysize; j ++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i <= (xsize - 2); i += 2) {
				MLIB_S_IMAGEAVE_D64(
					_mm_storeu_pd,
					_mm_load_pd,
					_mm_load_pd);
			}
			for (; i < xsize; i ++) {
				dst[i] = MLIB_C_IMAGEAVE(src1[i], src2[i]);
			}
			src1 = (mlib_d64 *)((mlib_u8 *)src1 + slb1);
			src2 = (mlib_d64 *)((mlib_u8 *)src2 + slb2);
			dst  = (mlib_d64 *)((mlib_u8 *)dst + dlb);
		}
	} else {
		for (j = 0; j < ysize; j ++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i <= (xsize - 2); i += 2) {
				MLIB_S_IMAGEAVE_D64(
					_mm_storeu_pd,
					_mm_loadu_pd,
					_mm_loadu_pd);
			}
			for (; i < xsize; i ++) {
				dst[i] = MLIB_C_IMAGEAVE(src1[i], src2[i]);
			}
			src1 = (mlib_d64 *)((mlib_u8 *)src1 + slb1);
			src2 = (mlib_d64 *)((mlib_u8 *)src2 + slb2);
			dst  = (mlib_d64 *)((mlib_u8 *)dst + dlb);
		}
	}
}

/* *********************************************************** */

mlib_status
__mlib_ImageAve_Fp(
	mlib_image *dst,
	const mlib_image *src1,
	const mlib_image *src2)
{
	mlib_type dtype;
	mlib_s32 slb1, slb2, dlb, xsize, ysize, nchan;
	void *sa1, *sa2, *da;

	MLIB_IMAGE_CHECK(dst);
	MLIB_IMAGE_CHECK(src1);
	MLIB_IMAGE_CHECK(src2);
	MLIB_IMAGE_FULL_EQUAL(dst, src1);
	MLIB_IMAGE_FULL_EQUAL(dst, src2);

	dtype = mlib_ImageGetType(dst);
	nchan = mlib_ImageGetChannels(dst);
	xsize = mlib_ImageGetWidth(dst);
	ysize = mlib_ImageGetHeight(dst);
	slb1 = mlib_ImageGetStride(src1);
	slb2 = mlib_ImageGetStride(src2);
	dlb = mlib_ImageGetStride(dst);
	sa1 = mlib_ImageGetData(src1);
	sa2 = mlib_ImageGetData(src2);
	da = mlib_ImageGetData(dst);

	xsize *= nchan;

	if (dtype == MLIB_FLOAT) {
		mlib_s_ImageAve_Fp_f32(da, dlb, sa1, slb1, sa2, slb2,
					xsize, ysize);
		return (MLIB_SUCCESS);
	} else
	if (dtype == MLIB_DOUBLE) {
		mlib_s_ImageAve_Fp_d64(da, dlb, sa1, slb1, sa2, slb2,
					xsize, ysize);
		return (MLIB_SUCCESS);
	}

	return (MLIB_FAILURE);
}

/* *********************************************************** */

mlib_status
__mlib_ImageAve_Fp_Inp(
	mlib_image *src1dst,
	const mlib_image *src2)
{
	return (__mlib_ImageAve_Fp(src1dst, src1dst, src2));
}

/* *********************************************************** */
