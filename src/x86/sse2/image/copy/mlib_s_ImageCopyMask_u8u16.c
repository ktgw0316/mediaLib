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

#pragma ident "@(#)mlib_s_ImageCopyMask_u8u16.c	9.2	07/11/05 SMI"

/*
 * FUNCTIONS
 *      mlib_s_ImageCopyMask_u16 - Copy one image to another via a mask
 *      mlib_s_ImageCopyMask_u8  - Copy one image to another via a mask
 *
 * SYNOPSIS
 *	void mlib_s_ImageCopyMask_u16(
 *				const mlib_u16 *src,
 *				mlib_s32 slb,
 *				const mlib_u16 *mask,
 *				mlib_s32 mlb,
 *				mlib_u16 *dst,
 *				mlib_s32 dlb,
 *				mlib_s32 xsize,
 *				mlib_s32 ysize,
 *				mlib_s32 nchan,
 *				const mlib_s32 *thresh);
 *	void mlib_s_ImageCopyMask_u8(
 *				const mlib_u8 *src,
 *				mlib_s32 slb,
 *				const mlib_u8 *mask,
 *				mlib_s32 mlb,
 *				mlib_u8 *dst,
 *				mlib_s32 dlb,
 *				mlib_s32 xsize,
 *				mlib_s32 ysize,
 *				mlib_s32 nchan,
 *				const mlib_s32 *thresh);
 *
 * ARGUMENT
 *      src	pointer to input or source image data
 *      slb	line stride of source image data in bytes
 *      dst	pointer to output or destination image data
 *      dlb	line stride of destination image data in bytes
 *      mask	pointer to mask image data
 *      mlb	line stride of mask image data in bytes
 *      xsize	image width in pixels
 *      ysize	image height in pixels
 *      nchan	number of channels in image
 *      thresh  array of thresholds
 *
 * DESCRIPTION
 *	dst(i, j) = src(i, j) if mask(i, j) <  *thresh+1
 *	            dst(i, j) if mask(i, j) >= *thresh+1
 */

#ifdef __SUNPRO_C
#include <sunmedia_intrin.h>
#else
#include <emmintrin.h>
#endif

#include "mlib_s_ImageCopyMask.h"

/* *********************************************************** */

#define	MLIB_S_COPYIMAGEMASK(M_COMP, 				\
		D_STORE, D_LOAD, S_LOAD, M_LOAD)		\
	{							\
		__m128i dsts, srcs, masks;			\
		__m128i bmap, msrcs, mdsts, ndsts;		\
		dsts  = D_LOAD((void *)&(dst[i]));		\
		srcs  = S_LOAD((void *)&(src[i]));		\
		masks = M_LOAD((void *)&(mask[i]));		\
		masks = _mm_xor_si128(masks, xormask);		\
		bmap  = M_COMP(masks, threshs);			\
		msrcs = _mm_and_si128(bmap, srcs);		\
		mdsts = _mm_andnot_si128(bmap, dsts);		\
		ndsts = _mm_or_si128(msrcs, mdsts);		\
		D_STORE((void *)&(dst[i]), ndsts);		\
	}

/* *********************************************************** */

#define	MLIB_S_COPYIMAGEMASK3(OFFSET1, OFFSET2, M_COMP,		\
		D_STORE, D_LOAD, S_LOAD, M_LOAD)		\
	{							\
		__m128i dsts, srcs, masks;			\
		__m128i bmap, msrcs, mdsts, ndsts;		\
		dsts  = D_LOAD((void *)&(dst[i]));		\
		srcs  = S_LOAD((void *)&(src[i]));		\
		masks = M_LOAD((void *)&(mask[i]));		\
		masks = _mm_xor_si128(masks, xormask);		\
		bmap  = M_COMP(masks, threshs);			\
		msrcs = _mm_and_si128(bmap, srcs);		\
		mdsts = _mm_andnot_si128(bmap, dsts);		\
		ndsts = _mm_or_si128(msrcs, mdsts);		\
		D_STORE((void *)&(dst[i]), ndsts);		\
		dsts  = D_LOAD((void *)&(dst[i + OFFSET1]));	\
		srcs  = S_LOAD((void *)&(src[i + OFFSET1]));	\
		masks = M_LOAD((void *)&(mask[i + OFFSET1]));	\
		masks = _mm_xor_si128(masks, xormask);		\
		bmap  = M_COMP(masks, threshu);			\
		msrcs = _mm_and_si128(bmap, srcs);		\
		mdsts = _mm_andnot_si128(bmap, dsts);		\
		ndsts = _mm_or_si128(msrcs, mdsts);		\
		D_STORE((void *)&(dst[i + OFFSET1]), ndsts);	\
		dsts  = D_LOAD((void *)&(dst[i + OFFSET2]));	\
		srcs  = S_LOAD((void *)&(src[i + OFFSET2]));	\
		masks = M_LOAD((void *)&(mask[i + OFFSET2]));	\
		masks = _mm_xor_si128(masks, xormask);		\
		bmap  = M_COMP(masks, threshv);			\
		msrcs = _mm_and_si128(bmap, srcs);		\
		mdsts = _mm_andnot_si128(bmap, dsts);		\
		ndsts = _mm_or_si128(msrcs, mdsts);		\
		D_STORE((void *)&(dst[i + OFFSET2]), ndsts);	\
	}

/* *********************************************************** */

#define	SAT_U16(D)	(((D) >= MLIB_U16_MAX) ?		\
				MLIB_U16_MAX :			\
				(((D) < MLIB_U16_MIN) ?		\
					MLIB_U16_MIN :		\
					((D) + 1)))

/* *********************************************************** */

void
mlib_s_ImageCopyMask_u16(
    const mlib_u16 *src,
    mlib_s32 slb,
    const mlib_u16 *mask,
    mlib_s32 mlb,
    mlib_u16 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 nchan,
    const mlib_s32 *thresh)
{
	mlib_s32 i, j, nsize;
	mlib_s32 thresh0, thresh1, thresh2, thresh3;
	__m128i threshs, threshu, threshv, xormask;
	mlib_s32 thresha, threshb;

	xormask = _mm_set1_epi16(0x8000);
	nsize = xsize * nchan;
	switch (nchan) {
	case 1:
		thresh0 = thresh[0];
		threshs = _mm_set1_epi16(SAT_U16(thresh0));
		threshs = _mm_xor_si128(threshs, xormask);
		if ((((mlib_addr)dst | dlb |
			(mlib_addr)src | slb |
			(mlib_addr)mask | mlb) & 0xf) == 0) {
			for (j = 0; j < ysize; j ++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i <= (nsize - 8); i += 8) {
					MLIB_S_COPYIMAGEMASK(
						_mm_cmplt_epi16,
						_mm_store_si128,
						_mm_load_si128,
						_mm_load_si128,
						_mm_load_si128);
				}
				for (; i < nsize; i ++) {
					if (mask[i] <= thresh0) {
						dst[i] = src[i];
					}
				}
				src  = (mlib_u16 *)((mlib_u8 *)src + slb);
				mask = (mlib_u16 *)((mlib_u8 *)mask + mlb);
				dst  = (mlib_u16 *)((mlib_u8 *)dst + dlb);
			}
		} else {
			for (j = 0; j < ysize; j ++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i <= (nsize - 8); i += 8) {
					MLIB_S_COPYIMAGEMASK(
						_mm_cmplt_epi16,
						_mm_storeu_si128,
						_mm_loadu_si128,
						_mm_loadu_si128,
						_mm_loadu_si128);
				}
				for (; i < nsize; i ++) {
					if (mask[i] <= thresh0) {
						dst[i] = src[i];
					}
				}
				src  = (mlib_u16 *)((mlib_u8 *)src + slb);
				mask = (mlib_u16 *)((mlib_u8 *)mask + mlb);
				dst  = (mlib_u16 *)((mlib_u8 *)dst + dlb);
			}
		}
		break;
	case 2:
		thresh0 = thresh[0];
		thresh1 = thresh[1];
		thresha = (SAT_U16(thresh1) << 16) |
				(SAT_U16(thresh0) & 0xffff);
		threshs = _mm_set1_epi32(thresha);
		threshs = _mm_xor_si128(threshs, xormask);
		if ((((mlib_addr)dst | dlb |
			(mlib_addr)src | slb |
			(mlib_addr)mask | mlb) & 0xf) == 0) {
			for (j = 0; j < ysize; j ++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i <= (nsize - 8); i += 8) {
					MLIB_S_COPYIMAGEMASK(
						_mm_cmplt_epi16,
						_mm_store_si128,
						_mm_load_si128,
						_mm_load_si128,
						_mm_load_si128);
				}
				for (; i < nsize; i += 2) {
					if (mask[i] <= thresh0) {
						dst[i] = src[i];
					}
					if (mask[i + 1] <= thresh1) {
						dst[i + 1] = src[i + 1];
					}
				}
				src  = (mlib_u16 *)((mlib_u8 *)src + slb);
				mask = (mlib_u16 *)((mlib_u8 *)mask + mlb);
				dst  = (mlib_u16 *)((mlib_u8 *)dst + dlb);
			}
		} else {
			for (j = 0; j < ysize; j ++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i <= (nsize - 8); i += 8) {
					MLIB_S_COPYIMAGEMASK(
						_mm_cmplt_epi16,
						_mm_storeu_si128,
						_mm_loadu_si128,
						_mm_loadu_si128,
						_mm_loadu_si128);
				}
				for (; i < nsize; i += 2) {
					if (mask[i] <= thresh0) {
						dst[i] = src[i];
					}
					if (mask[i + 1] <= thresh1) {
						dst[i + 1] = src[i + 1];
					}
				}
				src  = (mlib_u16 *)((mlib_u8 *)src + slb);
				mask = (mlib_u16 *)((mlib_u8 *)mask + mlb);
				dst  = (mlib_u16 *)((mlib_u8 *)dst + dlb);
			}
		}
		break;
	case 3:
		thresh0 = thresh[0];
		thresh1 = thresh[1];
		thresh2 = thresh[2];
		threshs = _mm_set_epi16(SAT_U16(thresh1),
					SAT_U16(thresh0),
					SAT_U16(thresh2),
					SAT_U16(thresh1),
					SAT_U16(thresh0),
					SAT_U16(thresh2),
					SAT_U16(thresh1),
					SAT_U16(thresh0));
		threshu = _mm_set_epi16(SAT_U16(thresh0),
					SAT_U16(thresh2),
					SAT_U16(thresh1),
					SAT_U16(thresh0),
					SAT_U16(thresh2),
					SAT_U16(thresh1),
					SAT_U16(thresh0),
					SAT_U16(thresh2));
		threshv = _mm_set_epi16(SAT_U16(thresh2),
					SAT_U16(thresh1),
					SAT_U16(thresh0),
					SAT_U16(thresh2),
					SAT_U16(thresh1),
					SAT_U16(thresh0),
					SAT_U16(thresh2),
					SAT_U16(thresh1));
		threshs = _mm_xor_si128(threshs, xormask);
		threshu = _mm_xor_si128(threshu, xormask);
		threshv = _mm_xor_si128(threshv, xormask);
		if ((((mlib_addr)dst | dlb |
			(mlib_addr)src | slb |
			(mlib_addr)mask | mlb) & 0xf) == 0) {
			for (j = 0; j < ysize; j ++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i <= (nsize - 24); i += 24) {
					MLIB_S_COPYIMAGEMASK3(
						8, 16,
						_mm_cmplt_epi16,
						_mm_store_si128,
						_mm_load_si128,
						_mm_load_si128,
						_mm_load_si128);
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
				src  = (mlib_u16 *)((mlib_u8 *)src + slb);
				mask = (mlib_u16 *)((mlib_u8 *)mask + mlb);
				dst  = (mlib_u16 *)((mlib_u8 *)dst + dlb);
			}
		} else {
			for (j = 0; j < ysize; j ++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i <= (nsize - 24); i += 24) {
					MLIB_S_COPYIMAGEMASK3(
						8, 16,
						_mm_cmplt_epi16,
						_mm_storeu_si128,
						_mm_loadu_si128,
						_mm_loadu_si128,
						_mm_loadu_si128);
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
				src  = (mlib_u16 *)((mlib_u8 *)src + slb);
				mask = (mlib_u16 *)((mlib_u8 *)mask + mlb);
				dst  = (mlib_u16 *)((mlib_u8 *)dst + dlb);
			}
		}
		break;
	case 4:
		thresh0 = thresh[0];
		thresh1 = thresh[1];
		thresh2 = thresh[2];
		thresh3 = thresh[3];
		thresha = (SAT_U16(thresh3) << 16) |
				(SAT_U16(thresh2) & 0xffff);
		threshb = (SAT_U16(thresh1) << 16) |
				(SAT_U16(thresh0) & 0xffff);
		threshs = _mm_set_epi32(thresha, threshb,
					thresha, threshb);
		threshs = _mm_xor_si128(threshs, xormask);
		if ((((mlib_addr)dst | dlb |
			(mlib_addr)src | slb |
			(mlib_addr)mask | mlb) & 0xf) == 0) {
			for (j = 0; j < ysize; j ++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i <= (nsize - 8); i += 8) {
					MLIB_S_COPYIMAGEMASK(
						_mm_cmplt_epi16,
						_mm_store_si128,
						_mm_load_si128,
						_mm_load_si128,
						_mm_load_si128);
				}
				for (; i < nsize; i += 4) {
					if (mask[i] <= thresh0) {
						dst[i] = src[i];
					}
					if (mask[i + 1] <= thresh1) {
						dst[i + 1] = src[i + 1];
					}
					if (mask[i + 2] <= thresh2) {
						dst[i + 2] = src[i + 2];
					}
					if (mask[i + 3] <= thresh3) {
						dst[i + 3] = src[i + 3];
					}
				}
				src  = (mlib_u16 *)((mlib_u8 *)src + slb);
				mask = (mlib_u16 *)((mlib_u8 *)mask + mlb);
				dst  = (mlib_u16 *)((mlib_u8 *)dst + dlb);
			}
		} else {
			for (j = 0; j < ysize; j ++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i <= (nsize - 8); i += 8) {
					MLIB_S_COPYIMAGEMASK(
						_mm_cmplt_epi16,
						_mm_storeu_si128,
						_mm_loadu_si128,
						_mm_loadu_si128,
						_mm_loadu_si128);
				}
				for (; i < nsize; i += 4) {
					if (mask[i] <= thresh0) {
						dst[i] = src[i];
					}
					if (mask[i + 1] <= thresh1) {
						dst[i + 1] = src[i + 1];
					}
					if (mask[i + 2] <= thresh2) {
						dst[i + 2] = src[i + 2];
					}
					if (mask[i + 3] <= thresh3) {
						dst[i + 3] = src[i + 3];
					}
				}
				src  = (mlib_u16 *)((mlib_u8 *)src + slb);
				mask = (mlib_u16 *)((mlib_u8 *)mask + mlb);
				dst  = (mlib_u16 *)((mlib_u8 *)dst + dlb);
			}
		}
		break;
	}
}

/* *********************************************************** */

#define	SAT_U8(D)	(((D) >= MLIB_U8_MAX) ?			\
				MLIB_U8_MAX :			\
				(((D) < MLIB_U8_MIN) ?		\
					MLIB_U8_MIN :		\
					((D) + 1)))

/* *********************************************************** */

void
mlib_s_ImageCopyMask_u8(
    mlib_u8 *src,
    mlib_s32 slb,
    const mlib_u8 *mask,
    mlib_s32 mlb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 nchan,
    const mlib_s32 *thresh)
{
	mlib_s32 i, j, nsize;
	mlib_s32 thresh0, thresh1, thresh2, thresh3;
	__m128i threshs, threshu, threshv, xormask;

	xormask = _mm_set1_epi8(0x80);
	nsize = xsize * nchan;
	switch (nchan) {
	case 1:
		thresh0 = thresh[0];
		threshs = _mm_set1_epi8(SAT_U8(thresh0));
		threshs = _mm_xor_si128(threshs, xormask);
		if ((((mlib_addr)dst | dlb |
			(mlib_addr)src | slb |
			(mlib_addr)mask | mlb) & 0xf) == 0) {
			for (j = 0; j < ysize; j ++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i <= (nsize - 16); i += 16) {
					MLIB_S_COPYIMAGEMASK(
						_mm_cmplt_epi8,
						_mm_store_si128,
						_mm_load_si128,
						_mm_load_si128,
						_mm_load_si128);
				}
				for (; i < nsize; i ++) {
					if (mask[i] <= thresh0) {
						dst[i] = src[i];
					}
				}
				src  = (mlib_u8 *)((mlib_u8 *)src + slb);
				mask = (mlib_u8 *)((mlib_u8 *)mask + mlb);
				dst  = (mlib_u8 *)((mlib_u8 *)dst + dlb);
			}
		} else {
			for (j = 0; j < ysize; j ++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i <= (nsize - 16); i += 16) {
					MLIB_S_COPYIMAGEMASK(
						_mm_cmplt_epi8,
						_mm_storeu_si128,
						_mm_loadu_si128,
						_mm_loadu_si128,
						_mm_loadu_si128);
				}
				for (; i < nsize; i ++) {
					if (mask[i] <= thresh0) {
						dst[i] = src[i];
					}
				}
				src  = (mlib_u8 *)((mlib_u8 *)src + slb);
				mask = (mlib_u8 *)((mlib_u8 *)mask + mlb);
				dst  = (mlib_u8 *)((mlib_u8 *)dst + dlb);
			}
		}
		break;
	case 2:
		thresh0 = thresh[0];
		thresh1 = thresh[1];
		threshs = _mm_set1_epi16((SAT_U8(thresh1) << 8) |
					(SAT_U8(thresh0) & 0xff));
		threshs = _mm_xor_si128(threshs, xormask);
		if ((((mlib_addr)dst | dlb |
			(mlib_addr)src | slb |
			(mlib_addr)mask | mlb) & 0xf) == 0) {
			for (j = 0; j < ysize; j ++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i <= (nsize - 16); i += 16) {
					MLIB_S_COPYIMAGEMASK(
						_mm_cmplt_epi8,
						_mm_store_si128,
						_mm_load_si128,
						_mm_load_si128,
						_mm_load_si128);
				}
				for (; i < nsize; i += 2) {
					if (mask[i] <= thresh0) {
						dst[i] = src[i];
					}
					if (mask[i + 1] <= thresh1) {
						dst[i + 1] = src[i + 1];
					}
				}
				src  = (mlib_u8 *)((mlib_u8 *)src + slb);
				mask = (mlib_u8 *)((mlib_u8 *)mask + mlb);
				dst  = (mlib_u8 *)((mlib_u8 *)dst + dlb);
			}
		} else {
			for (j = 0; j < ysize; j ++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i <= (nsize - 16); i += 16) {
					MLIB_S_COPYIMAGEMASK(
						_mm_cmplt_epi8,
						_mm_storeu_si128,
						_mm_loadu_si128,
						_mm_loadu_si128,
						_mm_loadu_si128);
				}
				for (; i < nsize; i += 2) {
					if (mask[i] <= thresh0) {
						dst[i] = src[i];
					}
					if (mask[i + 1] <= thresh1) {
						dst[i + 1] = src[i + 1];
					}
				}
				src  = (mlib_u8 *)((mlib_u8 *)src + slb);
				mask = (mlib_u8 *)((mlib_u8 *)mask + mlb);
				dst  = (mlib_u8 *)((mlib_u8 *)dst + dlb);
			}
		}
		break;
	case 3:
		thresh0 = thresh[0];
		thresh1 = thresh[1];
		thresh2 = thresh[2];
		threshs = _mm_set_epi8(SAT_U8(thresh0),
					SAT_U8(thresh2),
					SAT_U8(thresh1),
					SAT_U8(thresh0),
					SAT_U8(thresh2),
					SAT_U8(thresh1),
					SAT_U8(thresh0),
					SAT_U8(thresh2),
					SAT_U8(thresh1),
					SAT_U8(thresh0),
					SAT_U8(thresh2),
					SAT_U8(thresh1),
					SAT_U8(thresh0),
					SAT_U8(thresh2),
					SAT_U8(thresh1),
					SAT_U8(thresh0));
		threshu = _mm_set_epi8(SAT_U8(thresh1),
					SAT_U8(thresh0),
					SAT_U8(thresh2),
					SAT_U8(thresh1),
					SAT_U8(thresh0),
					SAT_U8(thresh2),
					SAT_U8(thresh1),
					SAT_U8(thresh0),
					SAT_U8(thresh2),
					SAT_U8(thresh1),
					SAT_U8(thresh0),
					SAT_U8(thresh2),
					SAT_U8(thresh1),
					SAT_U8(thresh0),
					SAT_U8(thresh2),
					SAT_U8(thresh1));
		threshv = _mm_set_epi8(SAT_U8(thresh2),
					SAT_U8(thresh1),
					SAT_U8(thresh0),
					SAT_U8(thresh2),
					SAT_U8(thresh1),
					SAT_U8(thresh0),
					SAT_U8(thresh2),
					SAT_U8(thresh1),
					SAT_U8(thresh0),
					SAT_U8(thresh2),
					SAT_U8(thresh1),
					SAT_U8(thresh0),
					SAT_U8(thresh2),
					SAT_U8(thresh1),
					SAT_U8(thresh0),
					SAT_U8(thresh2));
		threshs = _mm_xor_si128(threshs, xormask);
		threshu = _mm_xor_si128(threshu, xormask);
		threshv = _mm_xor_si128(threshv, xormask);
		if ((((mlib_addr)dst | dlb |
			(mlib_addr)src | slb |
			(mlib_addr)mask | mlb) & 0xf) == 0) {
			for (j = 0; j < ysize; j ++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i <= (nsize - 48); i += 48) {
					MLIB_S_COPYIMAGEMASK3(
						16, 32,
						_mm_cmplt_epi8,
						_mm_store_si128,
						_mm_load_si128,
						_mm_load_si128,
						_mm_load_si128);
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
				src  = (mlib_u8 *)((mlib_u8 *)src + slb);
				mask = (mlib_u8 *)((mlib_u8 *)mask + mlb);
				dst  = (mlib_u8 *)((mlib_u8 *)dst + dlb);
			}
		} else {
			for (j = 0; j < ysize; j ++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i <= (nsize - 48); i += 48) {
					MLIB_S_COPYIMAGEMASK3(
						16, 32,
						_mm_cmplt_epi8,
						_mm_storeu_si128,
						_mm_loadu_si128,
						_mm_loadu_si128,
						_mm_loadu_si128);
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
				src  = (mlib_u8 *)((mlib_u8 *)src + slb);
				mask = (mlib_u8 *)((mlib_u8 *)mask + mlb);
				dst  = (mlib_u8 *)((mlib_u8 *)dst + dlb);
			}
		}
		break;
	case 4:
		thresh0 = thresh[0];
		thresh1 = thresh[1];
		thresh2 = thresh[2];
		thresh3 = thresh[3];
		threshs = _mm_set1_epi32(((SAT_U8(thresh3) & 0xff) << 24) |
					((SAT_U8(thresh2) & 0xff) << 16) |
					((SAT_U8(thresh1) & 0xff) << 8) |
					(SAT_U8(thresh0) & 0xff));
		threshs = _mm_xor_si128(threshs, xormask);
		if ((((mlib_addr)dst | dlb |
			(mlib_addr)src | slb |
			(mlib_addr)mask | mlb) & 0xf) == 0) {
			for (j = 0; j < ysize; j ++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i <= (nsize - 16); i += 16) {
					MLIB_S_COPYIMAGEMASK(
						_mm_cmplt_epi8,
						_mm_store_si128,
						_mm_load_si128,
						_mm_load_si128,
						_mm_load_si128);
				}
				for (; i < nsize; i += 4) {
					if (mask[i] <= thresh0) {
						dst[i] = src[i];
					}
					if (mask[i + 1] <= thresh1) {
						dst[i + 1] = src[i + 1];
					}
					if (mask[i + 2] <= thresh2) {
						dst[i + 2] = src[i + 2];
					}
					if (mask[i + 3] <= thresh3) {
						dst[i + 3] = src[i + 3];
					}
				}
				src  = (mlib_u8 *)((mlib_u8 *)src + slb);
				mask = (mlib_u8 *)((mlib_u8 *)mask + mlb);
				dst  = (mlib_u8 *)((mlib_u8 *)dst + dlb);
			}
		} else {
			for (j = 0; j < ysize; j ++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i <= (nsize - 16); i += 16) {
					MLIB_S_COPYIMAGEMASK(
						_mm_cmplt_epi8,
						_mm_storeu_si128,
						_mm_loadu_si128,
						_mm_loadu_si128,
						_mm_loadu_si128);
				}
				for (; i < nsize; i += 4) {
					if (mask[i] <= thresh0) {
						dst[i] = src[i];
					}
					if (mask[i + 1] <= thresh1) {
						dst[i + 1] = src[i + 1];
					}
					if (mask[i + 2] <= thresh2) {
						dst[i + 2] = src[i + 2];
					}
					if (mask[i + 3] <= thresh3) {
						dst[i + 3] = src[i + 3];
					}
				}
				src  = (mlib_u8 *)((mlib_u8 *)src + slb);
				mask = (mlib_u8 *)((mlib_u8 *)mask + mlb);
				dst  = (mlib_u8 *)((mlib_u8 *)dst + dlb);
			}
		}
		break;
	}
}

/* *********************************************************** */
