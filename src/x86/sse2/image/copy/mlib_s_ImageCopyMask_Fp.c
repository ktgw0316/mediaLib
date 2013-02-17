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

#pragma ident "@(#)mlib_s_ImageCopyMask_Fp.c	9.2	07/11/05 SMI"

#ifdef __SUNPRO_C
#include <sunmedia_intrin.h>
#else
#include <emmintrin.h>
#endif

/*
 * FUNCTIONS
 *      mlib_ImageCopyMask  - Copy one image to another via a mask
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageCopyMask_Fp(mlib_image       *dst,
 *                                        const mlib_image *src,
 *                                        const mlib_image *mask,
 *                                        const mlib_d64   *thresh);
 *
 * ARGUMENT
 *      dst     pointer to output or destination image
 *      src     pointer to input or source image
 *      mask    pointer to mask image
 *      thresh  array of thresholds
 *
 * RESTRICTION
 *      src, dst and mask images must have the same size, type and
 *      number of channels.
 *      They can have 1, 2, 3 or 4 channels of
 *      MLIB_FLOAT or MLIB_DOUBLE data type.
 *
 * DESCRIPTION
 *      Copy one image to another via a mask.
 *      dst(i, j) = src(i, j) when mask(i, j) <= *thresh, or
 *                 dst(i, j) when mask(i, j) >  *thresh
 */

#include <mlib_image.h>
#include <mlib_ImageCheck.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageCopyMask_Fp = __mlib_ImageCopyMask_Fp

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_ImageCopyMask_Fp) mlib_ImageCopyMask_Fp
    __attribute__((weak, alias("__mlib_ImageCopyMask_Fp")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	MLIB_S_COPYIMAGEMASK_F32(D_STORE, D_LOAD, S_LOAD, M_LOAD)\
	{							\
		__m128 dsts, srcs, masks;			\
		__m128 bmap, msrcs, mdsts, ndsts;		\
		dsts  = D_LOAD((void *)&(dst[i]));		\
		srcs  = S_LOAD((void *)&(src[i]));		\
		masks = M_LOAD((void *)&(mask[i]));		\
		bmap  = _mm_cmpgt_ps(masks, threshs);		\
		msrcs = _mm_andnot_ps(bmap, srcs);		\
		mdsts = _mm_and_ps(bmap, dsts);			\
		ndsts = _mm_or_ps(msrcs, mdsts);		\
		D_STORE((void *)&(dst[i]), ndsts);		\
	}

/* *********************************************************** */

#define	MLIB_S_COPYIMAGEMASK3_F32(D_STORE, D_LOAD, S_LOAD, M_LOAD)\
	{							\
		__m128 dsts, srcs, masks;			\
		__m128 bmap, msrcs, mdsts, ndsts;		\
		dsts  = D_LOAD((void *)&(dst[i]));		\
		srcs  = S_LOAD((void *)&(src[i]));		\
		masks = M_LOAD((void *)&(mask[i]));		\
		bmap  = _mm_cmpgt_ps(masks, threshs);		\
		msrcs = _mm_andnot_ps(bmap, srcs);		\
		mdsts = _mm_and_ps(bmap, dsts);			\
		ndsts = _mm_or_ps(msrcs, mdsts);		\
		D_STORE((void *)&(dst[i]), ndsts);		\
		dsts  = D_LOAD((void *)&(dst[i + 4]));		\
		srcs  = S_LOAD((void *)&(src[i + 4]));		\
		masks = M_LOAD((void *)&(mask[i + 4]));		\
		bmap  = _mm_cmpgt_ps(masks, threshu);		\
		msrcs = _mm_andnot_ps(bmap, srcs);		\
		mdsts = _mm_and_ps(bmap, dsts);			\
		ndsts = _mm_or_ps(msrcs, mdsts);		\
		D_STORE((void *)&(dst[i + 4]), ndsts);		\
		dsts  = D_LOAD((void *)&(dst[i + 8]));		\
		srcs  = S_LOAD((void *)&(src[i + 8]));		\
		masks = M_LOAD((void *)&(mask[i + 8]));		\
		bmap  = _mm_cmpgt_ps(masks, threshv);		\
		msrcs = _mm_andnot_ps(bmap, srcs);		\
		mdsts = _mm_and_ps(bmap, dsts);			\
		ndsts = _mm_or_ps(msrcs, mdsts);		\
		D_STORE((void *)&(dst[i + 8]), ndsts);		\
	}

/* *********************************************************** */

#define	MLIB_S_COPYIMAGEMASK_D64(D_STORE, D_LOAD, S_LOAD, M_LOAD)\
	{							\
		__m128d dsts, srcs, masks;			\
		__m128d bmap, msrcs, mdsts, ndsts;		\
		dsts  = D_LOAD((void *)&(dst[i]));		\
		srcs  = S_LOAD((void *)&(src[i]));		\
		masks = M_LOAD((void *)&(mask[i]));		\
		bmap  = _mm_cmpgt_pd(masks, threshs);		\
		msrcs = _mm_andnot_pd(bmap, srcs);		\
		mdsts = _mm_and_pd(bmap, dsts);			\
		ndsts = _mm_or_pd(msrcs, mdsts);		\
		D_STORE((void *)&(dst[i]), ndsts);		\
	}

/* *********************************************************** */

#define	MLIB_S_COPYIMAGEMASK3_D64(D_STORE, D_LOAD, S_LOAD, M_LOAD)\
	{							\
		__m128d dsts, srcs, masks;			\
		__m128d bmap, msrcs, mdsts, ndsts;		\
		dsts  = D_LOAD((void *)&(dst[i]));		\
		srcs  = S_LOAD((void *)&(src[i]));		\
		masks = M_LOAD((void *)&(mask[i]));		\
		bmap  = _mm_cmpgt_pd(masks, threshs);		\
		msrcs = _mm_andnot_pd(bmap, srcs);		\
		mdsts = _mm_and_pd(bmap, dsts);			\
		ndsts = _mm_or_pd(msrcs, mdsts);		\
		D_STORE((void *)&(dst[i]), ndsts);		\
		dsts  = D_LOAD((void *)&(dst[i + 2]));		\
		srcs  = S_LOAD((void *)&(src[i + 2]));		\
		masks = M_LOAD((void *)&(mask[i + 2]));		\
		bmap  = _mm_cmpgt_pd(masks, threshu);		\
		msrcs = _mm_andnot_pd(bmap, srcs);		\
		mdsts = _mm_and_pd(bmap, dsts);			\
		ndsts = _mm_or_pd(msrcs, mdsts);		\
		D_STORE((void *)&(dst[i + 2]), ndsts);		\
		dsts  = D_LOAD((void *)&(dst[i + 4]));		\
		srcs  = S_LOAD((void *)&(src[i + 4]));		\
		masks = M_LOAD((void *)&(mask[i + 4]));		\
		bmap  = _mm_cmpgt_pd(masks, threshv);		\
		msrcs = _mm_andnot_pd(bmap, srcs);		\
		mdsts = _mm_and_pd(bmap, dsts);			\
		ndsts = _mm_or_pd(msrcs, mdsts);		\
		D_STORE((void *)&(dst[i + 4]), ndsts);		\
	}

/* *********************************************************** */

#define	MLIB_S_COPYIMAGEMASK4_D64(D_STORE, D_LOAD, S_LOAD, M_LOAD)\
	{							\
		__m128d dsts, srcs, masks;			\
		__m128d bmap, msrcs, mdsts, ndsts;		\
		dsts  = D_LOAD((void *)&(dst[i]));		\
		srcs  = S_LOAD((void *)&(src[i]));		\
		masks = M_LOAD((void *)&(mask[i]));		\
		bmap  = _mm_cmpgt_pd(masks, threshs);		\
		msrcs = _mm_andnot_pd(bmap, srcs);		\
		mdsts = _mm_and_pd(bmap, dsts);			\
		ndsts = _mm_or_pd(msrcs, mdsts);		\
		D_STORE((void *)&(dst[i]), ndsts);		\
		dsts  = D_LOAD((void *)&(dst[i + 2]));		\
		srcs  = S_LOAD((void *)&(src[i + 2]));		\
		masks = M_LOAD((void *)&(mask[i + 2]));		\
		bmap  = _mm_cmpgt_pd(masks, threshu);		\
		msrcs = _mm_andnot_pd(bmap, srcs);		\
		mdsts = _mm_and_pd(bmap, dsts);			\
		ndsts = _mm_or_pd(msrcs, mdsts);		\
		D_STORE((void *)&(dst[i + 2]), ndsts);		\
	}

/* *********************************************************** */

static void
mlib_ImageCopyMask_Fp_f32(
    const mlib_f32 *src,
    mlib_s32 slb,
    const mlib_f32 *mask,
    mlib_s32 mlb,
    mlib_f32 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 nchan,
    const mlib_d64 *thresh)
{
	mlib_s32 i, j, nsize;
	mlib_f32 thresh0, thresh1, thresh2, thresh3;
	__m128 threshs, threshu, threshv;

	nsize = xsize * nchan;
	switch (nchan) {
	case 1:
		thresh0 = thresh[0];
		threshs = _mm_set1_ps(thresh0);
		if ((((mlib_addr)dst | dlb |
			(mlib_addr)src | slb |
			(mlib_addr)mask | mlb) & 0xf) == 0) {
			for (j = 0; j < ysize; j ++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i <= (nsize - 4); i += 4) {
					MLIB_S_COPYIMAGEMASK_F32(
						_mm_store_ps,
						_mm_load_ps,
						_mm_load_ps,
						_mm_load_ps);
				}
				for (; i < nsize; i ++) {
					if (mask[i] <= thresh0) {
						dst[i] = src[i];
					}
				}
				src  = (mlib_f32 *)((mlib_u8 *)src + slb);
				mask = (mlib_f32 *)((mlib_u8 *)mask + mlb);
				dst  = (mlib_f32 *)((mlib_u8 *)dst + dlb);
			}
		} else {
			for (j = 0; j < ysize; j ++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i <= (nsize - 4); i += 4) {
					MLIB_S_COPYIMAGEMASK_F32(
						_mm_storeu_ps,
						_mm_loadu_ps,
						_mm_loadu_ps,
						_mm_loadu_ps);
				}
				for (; i < nsize; i ++) {
					if (mask[i] <= thresh0) {
						dst[i] = src[i];
					}
				}
				src  = (mlib_f32 *)((mlib_u8 *)src + slb);
				mask = (mlib_f32 *)((mlib_u8 *)mask + mlb);
				dst  = (mlib_f32 *)((mlib_u8 *)dst + dlb);
			}
		}
		break;
	case 2:
		thresh0 = thresh[0];
		thresh1 = thresh[1];
		threshs = _mm_set_ps(thresh1, thresh0,
					thresh1, thresh0);
		if ((((mlib_addr)dst | dlb |
			(mlib_addr)src | slb |
			(mlib_addr)mask | mlb) & 0xf) == 0) {
			for (j = 0; j < ysize; j ++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i <= (nsize - 4); i += 4) {
					MLIB_S_COPYIMAGEMASK_F32(
						_mm_store_ps,
						_mm_load_ps,
						_mm_load_ps,
						_mm_load_ps);
				}
				for (; i < nsize; i += 2) {
					if (mask[i] <= thresh0) {
						dst[i] = src[i];
					}
					if (mask[i + 1] <= thresh1) {
						dst[i + 1] = src[i + 1];
					}
				}
				src  = (mlib_f32 *)((mlib_u8 *)src + slb);
				mask = (mlib_f32 *)((mlib_u8 *)mask + mlb);
				dst  = (mlib_f32 *)((mlib_u8 *)dst + dlb);
			}
		} else {
			for (j = 0; j < ysize; j ++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i <= (nsize - 4); i += 4) {
					MLIB_S_COPYIMAGEMASK_F32(
						_mm_storeu_ps,
						_mm_loadu_ps,
						_mm_loadu_ps,
						_mm_loadu_ps);
				}
				for (; i < nsize; i += 2) {
					if (mask[i] <= thresh0) {
						dst[i] = src[i];
					}
					if (mask[i + 1] <= thresh1) {
						dst[i + 1] = src[i + 1];
					}
				}
				src  = (mlib_f32 *)((mlib_u8 *)src + slb);
				mask = (mlib_f32 *)((mlib_u8 *)mask + mlb);
				dst  = (mlib_f32 *)((mlib_u8 *)dst + dlb);
			}
		}
		break;
	case 3:
		thresh0 = thresh[0];
		thresh1 = thresh[1];
		thresh2 = thresh[2];
		threshs = _mm_set_ps(thresh0, thresh2,
					thresh1, thresh0);
		threshu = _mm_set_ps(thresh1, thresh0,
					thresh2, thresh1);
		threshv = _mm_set_ps(thresh2, thresh1,
					thresh0, thresh2);
		if ((((mlib_addr)dst | dlb |
			(mlib_addr)src | slb |
			(mlib_addr)mask | mlb) & 0xf) == 0) {
			for (j = 0; j < ysize; j ++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i <= (nsize - 12); i += 12) {
					MLIB_S_COPYIMAGEMASK3_F32(
						_mm_store_ps,
						_mm_load_ps,
						_mm_load_ps,
						_mm_load_ps);
				}
				for (; i < nsize; i += 3) {
					if (mask[i] <= thresh0) {
						dst[i] = src[i];
					}
					if (mask[i + 1] <= thresh1) {
						dst[i + 1] = src[i + 1];
					}
					if (mask[i + 2] <= thresh2) {
						dst[i + 2] = src[i + 2];
					}
				}
				src  = (mlib_f32 *)((mlib_u8 *)src + slb);
				mask = (mlib_f32 *)((mlib_u8 *)mask + mlb);
				dst  = (mlib_f32 *)((mlib_u8 *)dst + dlb);
			}
		} else {
			for (j = 0; j < ysize; j ++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i <= (nsize - 12); i += 12) {
					MLIB_S_COPYIMAGEMASK3_F32(
						_mm_storeu_ps,
						_mm_loadu_ps,
						_mm_loadu_ps,
						_mm_loadu_ps);
				}
				for (; i < nsize; i += 3) {
					if (mask[i] <= thresh0) {
						dst[i] = src[i];
					}
					if (mask[i + 1] <= thresh1) {
						dst[i + 1] = src[i + 1];
					}
					if (mask[i + 2] <= thresh2) {
						dst[i + 2] = src[i + 2];
					}
				}
				src  = (mlib_f32 *)((mlib_u8 *)src + slb);
				mask = (mlib_f32 *)((mlib_u8 *)mask + mlb);
				dst  = (mlib_f32 *)((mlib_u8 *)dst + dlb);
			}
		}
		break;
	case 4:
		thresh0 = thresh[0];
		thresh1 = thresh[1];
		thresh2 = thresh[2];
		thresh3 = thresh[3];
		threshs = _mm_set_ps(thresh3, thresh2,
					thresh1, thresh0);
		if ((((mlib_addr)dst | dlb |
			(mlib_addr)src | slb |
			(mlib_addr)mask | mlb) & 0xf) == 0) {
			for (j = 0; j < ysize; j ++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i <= (nsize - 4); i += 4) {
					MLIB_S_COPYIMAGEMASK_F32(
						_mm_store_ps,
						_mm_load_ps,
						_mm_load_ps,
						_mm_load_ps);
				}
				for (; i < nsize; i += 4) {
					if (mask[i] <= thresh0) {
						dst[i] = src[i];
					}
					if (mask[i + 1] <= thresh1) {
						dst[i + 1] = src[i + 1];
					}
					if (mask[i + 2] <= thresh2) {
						dst[i + 1] = src[i + 2];
					}
					if (mask[i + 3] <= thresh3) {
						dst[i + 1] = src[i + 3];
					}
				}
				src  = (mlib_f32 *)((mlib_u8 *)src + slb);
				mask = (mlib_f32 *)((mlib_u8 *)mask + mlb);
				dst  = (mlib_f32 *)((mlib_u8 *)dst + dlb);
			}
		} else {
			for (j = 0; j < ysize; j ++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i <= (nsize - 4); i += 4) {
					MLIB_S_COPYIMAGEMASK_F32(
						_mm_storeu_ps,
						_mm_loadu_ps,
						_mm_loadu_ps,
						_mm_loadu_ps);
				}
				for (; i < nsize; i += 4) {
					if (mask[i] <= thresh0) {
						dst[i] = src[i];
					}
					if (mask[i + 1] <= thresh1) {
						dst[i + 1] = src[i + 1];
					}
					if (mask[i + 2] <= thresh2) {
						dst[i + 1] = src[i + 2];
					}
					if (mask[i + 3] <= thresh3) {
						dst[i + 1] = src[i + 3];
					}
				}
				src  = (mlib_f32 *)((mlib_u8 *)src + slb);
				mask = (mlib_f32 *)((mlib_u8 *)mask + mlb);
				dst  = (mlib_f32 *)((mlib_u8 *)dst + dlb);
			}
		}
		break;
	}
}

/* *********************************************************** */

static void
mlib_ImageCopyMask_Fp_d64(
    const mlib_d64 *src,
    mlib_s32 slb,
    const mlib_d64 *mask,
    mlib_s32 mlb,
    mlib_d64 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 nchan,
    const mlib_d64 *thresh)
{
	mlib_s32 i, j, nsize;
	mlib_d64 thresh0, thresh1, thresh2, thresh3;
	__m128d threshs, threshu, threshv;

	nsize = xsize * nchan;
	switch (nchan) {
	case 1:
		thresh0 = thresh[0];
		threshs = _mm_set1_pd(thresh0);
		if ((((mlib_addr)dst | dlb |
			(mlib_addr)src | slb |
			(mlib_addr)mask | mlb) & 0xf) == 0) {
			for (j = 0; j < ysize; j ++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i <= (nsize - 2); i += 2) {
					MLIB_S_COPYIMAGEMASK_D64(
						_mm_store_pd,
						_mm_load_pd,
						_mm_load_pd,
						_mm_load_pd);
				}
				for (; i < nsize; i ++) {
					if (mask[i] <= thresh0) {
						dst[i] = src[i];
					}
				}
				src  = (mlib_d64 *)((mlib_u8 *)src + slb);
				mask = (mlib_d64 *)((mlib_u8 *)mask + mlb);
				dst  = (mlib_d64 *)((mlib_u8 *)dst + dlb);
			}
		} else {
			for (j = 0; j < ysize; j ++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i <= (nsize - 2); i += 2) {
					MLIB_S_COPYIMAGEMASK_D64(
						_mm_storeu_pd,
						_mm_loadu_pd,
						_mm_loadu_pd,
						_mm_loadu_pd);
				}
				for (; i < nsize; i ++) {
					if (mask[i] <= thresh0) {
						dst[i] = src[i];
					}
				}
				src  = (mlib_d64 *)((mlib_u8 *)src + slb);
				mask = (mlib_d64 *)((mlib_u8 *)mask + mlb);
				dst  = (mlib_d64 *)((mlib_u8 *)dst + dlb);
			}
		}
		break;
	case 2:
		thresh0 = thresh[0];
		thresh1 = thresh[1];
		threshs = _mm_set_pd(thresh1, thresh0);
		if ((((mlib_addr)dst | dlb |
			(mlib_addr)src | slb |
			(mlib_addr)mask | mlb) & 0xf) == 0) {
			for (j = 0; j < ysize; j ++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i <= (nsize - 2); i += 2) {
					MLIB_S_COPYIMAGEMASK_D64(
						_mm_store_pd,
						_mm_load_pd,
						_mm_load_pd,
						_mm_load_pd);
				}
				for (; i < nsize; i += 2) {
					if (mask[i] <= thresh0) {
						dst[i] = src[i];
					}
					if (mask[i + 1] <= thresh1) {
						dst[i + 1] = src[i + 1];
					}
				}
				src  = (mlib_d64 *)((mlib_u8 *)src + slb);
				mask = (mlib_d64 *)((mlib_u8 *)mask + mlb);
				dst  = (mlib_d64 *)((mlib_u8 *)dst + dlb);
			}
		} else {
			for (j = 0; j < ysize; j ++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i <= (nsize - 2); i += 2) {
					MLIB_S_COPYIMAGEMASK_D64(
						_mm_storeu_pd,
						_mm_loadu_pd,
						_mm_loadu_pd,
						_mm_loadu_pd);
				}
				for (; i < nsize; i += 2) {
					if (mask[i] <= thresh0) {
						dst[i] = src[i];
					}
					if (mask[i + 1] <= thresh1) {
						dst[i + 1] = src[i + 1];
					}
				}
				src  = (mlib_d64 *)((mlib_u8 *)src + slb);
				mask = (mlib_d64 *)((mlib_u8 *)mask + mlb);
				dst  = (mlib_d64 *)((mlib_u8 *)dst + dlb);
			}
		}
		break;
	case 3:
		thresh0 = thresh[0];
		thresh1 = thresh[1];
		thresh2 = thresh[2];
		threshs = _mm_set_pd(thresh1, thresh0);
		threshu = _mm_set_pd(thresh0, thresh2);
		threshv = _mm_set_pd(thresh2, thresh1);
		if ((((mlib_addr)dst | dlb |
			(mlib_addr)src | slb |
			(mlib_addr)mask | mlb) & 0xf) == 0) {
			for (j = 0; j < ysize; j ++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i <= (nsize - 6); i += 6) {
					MLIB_S_COPYIMAGEMASK3_D64(
						_mm_store_pd,
						_mm_load_pd,
						_mm_load_pd,
						_mm_load_pd);
				}
				for (; i < nsize; i += 3) {
					if (mask[i] <= thresh0) {
						dst[i] = src[i];
					}
					if (mask[i + 1] <= thresh1) {
						dst[i + 1] = src[i + 1];
					}
					if (mask[i + 2] <= thresh2) {
						dst[i + 2] = src[i + 2];
					}
				}
				src  = (mlib_d64 *)((mlib_u8 *)src + slb);
				mask = (mlib_d64 *)((mlib_u8 *)mask + mlb);
				dst  = (mlib_d64 *)((mlib_u8 *)dst + dlb);
			}
		} else {
			for (j = 0; j < ysize; j ++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i <= (nsize - 6); i += 6) {
					MLIB_S_COPYIMAGEMASK3_D64(
						_mm_storeu_pd,
						_mm_loadu_pd,
						_mm_loadu_pd,
						_mm_loadu_pd);
				}
				for (; i < nsize; i += 3) {
					if (mask[i] <= thresh0) {
						dst[i] = src[i];
					}
					if (mask[i + 1] <= thresh1) {
						dst[i + 1] = src[i + 1];
					}
					if (mask[i + 2] <= thresh2) {
						dst[i + 2] = src[i + 2];
					}
				}
				src  = (mlib_d64 *)((mlib_u8 *)src + slb);
				mask = (mlib_d64 *)((mlib_u8 *)mask + mlb);
				dst  = (mlib_d64 *)((mlib_u8 *)dst + dlb);
			}
		}
		break;
	case 4:
		thresh0 = thresh[0];
		thresh1 = thresh[1];
		thresh2 = thresh[2];
		thresh3 = thresh[3];
		threshs = _mm_set_pd(thresh1, thresh0);
		threshu = _mm_set_pd(thresh3, thresh2);
		if ((((mlib_addr)dst | dlb |
			(mlib_addr)src | slb |
			(mlib_addr)mask | mlb) & 0xf) == 0) {
			for (j = 0; j < ysize; j ++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i <= (nsize - 4); i += 4) {
					MLIB_S_COPYIMAGEMASK4_D64(
						_mm_store_pd,
						_mm_load_pd,
						_mm_load_pd,
						_mm_load_pd);
				}
				for (; i < nsize; i += 4) {
					if (mask[i] <= thresh0) {
						dst[i] = src[i];
					}
					if (mask[i + 1] <= thresh1) {
						dst[i + 1] = src[i + 1];
					}
					if (mask[i + 2] <= thresh2) {
						dst[i + 1] = src[i + 2];
					}
					if (mask[i + 3] <= thresh3) {
						dst[i + 1] = src[i + 3];
					}
				}
				src  = (mlib_d64 *)((mlib_u8 *)src + slb);
				mask = (mlib_d64 *)((mlib_u8 *)mask + mlb);
				dst  = (mlib_d64 *)((mlib_u8 *)dst + dlb);
			}
		} else {
			for (j = 0; j < ysize; j ++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i <= (nsize - 4); i += 4) {
					MLIB_S_COPYIMAGEMASK4_D64(
						_mm_storeu_pd,
						_mm_loadu_pd,
						_mm_loadu_pd,
						_mm_loadu_pd);
				}
				for (; i < nsize; i += 4) {
					if (mask[i] <= thresh0) {
						dst[i] = src[i];
					}
					if (mask[i + 1] <= thresh1) {
						dst[i + 1] = src[i + 1];
					}
					if (mask[i + 2] <= thresh2) {
						dst[i + 1] = src[i + 2];
					}
					if (mask[i + 3] <= thresh3) {
						dst[i + 1] = src[i + 3];
					}
				}
				src  = (mlib_d64 *)((mlib_u8 *)src + slb);
				mask = (mlib_d64 *)((mlib_u8 *)mask + mlb);
				dst  = (mlib_d64 *)((mlib_u8 *)dst + dlb);
			}
		}
		break;
	}
}

/* *********************************************************** */

mlib_status
__mlib_ImageCopyMask_Fp(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_image *mask,
    const mlib_d64 *thresh)
{
	mlib_type dtype;
	mlib_s32 slb, mlb, dlb, xsize, ysize, nchan;
	void *sa, *ma, *da;

	MLIB_IMAGE_CHECK(src);
	MLIB_IMAGE_CHECK(dst);
	MLIB_IMAGE_CHECK(mask);

	MLIB_IMAGE_TYPE_EQUAL(src, dst);
	MLIB_IMAGE_TYPE_EQUAL(mask, dst);

	MLIB_IMAGE_CHAN_EQUAL(src, dst);
	MLIB_IMAGE_CHAN_EQUAL(mask, dst);

	MLIB_IMAGE_SIZE_EQUAL(src, dst);
	MLIB_IMAGE_SIZE_EQUAL(mask, dst);

	dtype = mlib_ImageGetType(dst);
	nchan = mlib_ImageGetChannels(dst);
	xsize = mlib_ImageGetWidth(dst);
	ysize = mlib_ImageGetHeight(dst);
	slb = mlib_ImageGetStride(src);
	mlb = mlib_ImageGetStride(mask);
	dlb = mlib_ImageGetStride(dst);
	sa = mlib_ImageGetData(src);
	ma = mlib_ImageGetData(mask);
	da = mlib_ImageGetData(dst);

	if (dtype == MLIB_FLOAT) {
		mlib_ImageCopyMask_Fp_f32(sa, slb, ma, mlb, da, dlb, xsize,
		    ysize, nchan, thresh);
		return (MLIB_SUCCESS);
	} else if (dtype == MLIB_DOUBLE) {
		mlib_ImageCopyMask_Fp_d64(sa, slb, ma, mlb, da, dlb, xsize,
		    ysize, nchan, thresh);
		return (MLIB_SUCCESS);
	} else
		return (MLIB_FAILURE);
}

/* *********************************************************** */
