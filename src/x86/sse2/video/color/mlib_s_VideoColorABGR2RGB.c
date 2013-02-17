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

#pragma ident	"@(#)mlib_s_VideoColorABGR2RGB.c	9.5	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_VideoColorABGR2RGB - ABGR to RGB conversion
 *
 * SYNOPSIS
 *      mlib_status mlib_VideoColorABGR2RGB(mlib_u8       *rgb,
 *                                          const mlib_u8 *abgr,
 *                                          mlib_s32      n)
 *
 * ARGUMENTS
 *      rgb       Pointer to  RGB multicomponent row, 8-byte aligned
 *      abgr      Pointer to ABGR multicomponent row, 8-byte aligned
 *      n         Number of pixels
 *
 * DESCRIPTION
 *
 *      rgb[3*i  ] = argb[4*i+3]
 *      rgb[3*i+1] = argb[4*i+2]
 *      rgb[3*i+2] = argb[4*i+1]
 *
 *      0 <= i < n
 */

#include <mlib_video.h>
#ifdef __SUNPRO_C
#include <sunmedia_intrin.h>
#else
#include <emmintrin.h>
#endif

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_VideoColorABGR2RGB = __mlib_VideoColorABGR2RGB

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_VideoColorABGR2RGB) mlib_VideoColorABGR2RGB
	__attribute__((weak, alias("__mlib_VideoColorABGR2RGB")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */
/* save rgb from sequence: rgb0 rgb0 rgb0 rgb0 */
#define	PACK_RGB1(rgb, pdd)	\
{							\
	mlib_u8 *pout = (mlib_u8*)(&rgb);		\
	*((mlib_s32*)pdd) = *((mlib_s32*)pout);	\
	pdd += 3;					\
	pout += 4;					\
	*((mlib_s32*)pdd) = *((mlib_s32*)pout);	\
	pdd += 3;					\
	pout += 4;					\
	*((mlib_s32*)pdd) = *((mlib_s32*)pout);	\
	pdd += 3;					\
	pout += 4;					\
	*((mlib_s32*)pdd) = *((mlib_s32*)pout);	\
	pdd += 3;					\
}

#define	PACK_RGB2(rgb, pdd)	\
{							\
	mlib_u8 *pout = (mlib_u8*)(&rgb);		\
	*((mlib_s32*)pdd) = *((mlib_s32*)pout);	\
	pdd += 3;					\
	pout += 4;					\
	*((mlib_s32*)pdd) = *((mlib_s32*)pout);	\
	pdd += 3;					\
	pout += 4;					\
	*((mlib_s32*)pdd) = *((mlib_s32*)pout);	\
	pdd += 3;					\
	pout += 4;					\
	*((mlib_s16*)pdd) = *((mlib_s16*)pout);	\
	pdd += 2;					\
	pout += 2;					\
	*((mlib_u8*)pdd) = *((mlib_u8*)pout);		\
	pdd += 1;					\
}

/* half __m128i */
#define	PACK_RGB3(rgb, pdd)	\
{							\
	mlib_u8 *pout = (mlib_u8*)(&rgb);		\
	*((mlib_s32*)pdd) = *((mlib_s32*)pout);	\
	pdd += 3;					\
	pout += 4;					\
	*((mlib_s16*)pdd) = *((mlib_s16*)pout);	\
	pdd += 2;					\
	pout += 2;					\
	*((mlib_u8*)pdd) = *((mlib_u8*)pout);		\
	pdd += 1;					\
}

/* *********************************************************** */
mlib_status
mlib_VideoColorABGR2RGB_aligned(
	mlib_u8 *rgb,
	const mlib_u8 *abgr,
	mlib_s32 n);

mlib_status
mlib_VideoColorABGR2RGB_naligned(
	mlib_u8 *rgb,
	const mlib_u8 *abgr,
	mlib_s32 n);

mlib_status
__mlib_VideoColorABGR2RGB(
	mlib_u8 *rgb,
	const mlib_u8 *abgr,
	mlib_s32 n)
{
	if (rgb == NULL || abgr == NULL)
		return (MLIB_NULLPOINTER);

	if (n <= 0)
		return (MLIB_FAILURE);

	if ((0xf & ((mlib_addr)abgr)) == 0) {
		return mlib_VideoColorABGR2RGB_aligned(
			rgb, abgr, n);
	} else {
		return mlib_VideoColorABGR2RGB_naligned(
			rgb, abgr, n);
	}

}

mlib_status
mlib_VideoColorABGR2RGB_aligned(
	mlib_u8 *rgb,
	const mlib_u8 *abgr,
	mlib_s32 n)
{
	const __m128i x_mask_r = _mm_set1_epi32(0xff);
	const __m128i x_mask_g = _mm_set1_epi32(0xff00);
	const __m128i x_mask_b = _mm_set1_epi32(0xff0000);
	__m128i *px_abgr;
	mlib_s32 i, iabgr, ig, ib, ir, igb;
	mlib_u8 *prgb;

	px_abgr = (__m128i *)abgr;
	prgb = rgb;
	i = 0;

/*
 * code is improved as done for mlib_VideoColorARGB2RGB()
 * but the performance is still not as good as that of STDC version
 * refer to VIS version for SPARC for further tuning, if possible
 */
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	/* 4 pixels */
	for (; i <= (n - 8); i += 4) {
		__m128i x_abgr, x_rgbz, x_r, x_g, x_b, x_temp;
		/* load & extract */
		x_abgr = _mm_load_si128(px_abgr);
		px_abgr++;
		x_temp = _mm_srli_si128(x_abgr, 1);
		x_g = _mm_and_si128(x_mask_g, x_temp);
		x_temp = _mm_slli_si128(x_abgr, 1);
		x_b = _mm_and_si128(x_mask_b, x_temp);
		x_temp = _mm_srli_si128(x_abgr, 3);
		x_r = _mm_and_si128(x_mask_r, x_temp);

		/* construct rgb seq */
		x_rgbz = _mm_or_si128(x_r, x_g);
		x_rgbz = _mm_or_si128(x_rgbz, x_b);

		_mm_storel_epi64((__m128i *)prgb, x_rgbz);
		x_rgbz = _mm_srli_si128(x_rgbz, 4);
		_mm_storel_epi64((__m128i *)(prgb + 3), x_rgbz);
		x_rgbz = _mm_srli_si128(x_rgbz, 4);
		_mm_storel_epi64((__m128i *)(prgb + 6), x_rgbz);
		x_rgbz = _mm_srli_si128(x_rgbz, 4);
		_mm_storel_epi64((__m128i *)(prgb + 9), x_rgbz);
		prgb += 12;
	}

	/* 4 pixels */
	if (i <= (n - 4)) {
		__m128i x_abgr, x_rgbz, x_r, x_g, x_b, x_temp;
		/* load & extract */
		x_abgr = _mm_load_si128(px_abgr);
		px_abgr++;
		x_temp = _mm_srli_si128(x_abgr, 1);
		x_g = _mm_and_si128(x_mask_g, x_temp);
		x_temp = _mm_slli_si128(x_abgr, 1);
		x_b = _mm_and_si128(x_mask_b, x_temp);
		x_temp = _mm_srli_si128(x_abgr, 3);
		x_r = _mm_and_si128(x_mask_r, x_temp);

		/* construct rgb seq */
		x_rgbz = _mm_or_si128(x_r, x_g);
		x_rgbz = _mm_or_si128(x_rgbz, x_b);
		PACK_RGB2(x_rgbz, prgb);

		i += 4;
	}

	/* 2 pixels */
	if (i <= (n - 2)) {
		__m128i x_abgr, x_rgbz, x_r, x_g, x_b, x_temp;
		/* load & extract */
		x_abgr = _mm_loadl_epi64(px_abgr);
		px_abgr = (__m128i *)(((__m64 *)px_abgr) + 1);
		x_temp = _mm_srli_si128(x_abgr, 1);
		x_g = _mm_and_si128(x_mask_g, x_temp);
		x_temp = _mm_slli_si128(x_abgr, 1);
		x_b = _mm_and_si128(x_mask_b, x_temp);
		x_temp = _mm_srli_si128(x_abgr, 3);
		x_r = _mm_and_si128(x_mask_r, x_temp);

		/* construct rgb seq */
		x_rgbz = _mm_or_si128(x_r, x_g);
		x_rgbz = _mm_or_si128(x_rgbz, x_b);
		PACK_RGB3(x_rgbz, prgb);

		i += 2;
	}

	/* 1 pixel */
	if (i <= (n - 1)) {
		/* load & extract */
		iabgr = *((int *)px_abgr);
		ig = (iabgr>>16) & (0xff);
		ib = iabgr & (0xff00);
		ir = (iabgr>>24) & (0xff);
		igb = ig | ib;
		*prgb = ir;
		prgb++;
		*((mlib_s16*)prgb) = (mlib_s16)igb;
		prgb += 2;

		i += 1;
	}

	return (MLIB_SUCCESS);
}

mlib_status
mlib_VideoColorABGR2RGB_naligned(
	mlib_u8 *rgb,
	const mlib_u8 *abgr,
	mlib_s32 n)
{
	const __m128i x_mask_r = _mm_set1_epi32(0xff);
	const __m128i x_mask_g = _mm_set1_epi32(0xff00);
	const __m128i x_mask_b = _mm_set1_epi32(0xff0000);
	__m128i *px_abgr;
	mlib_s32 i, iabgr, ig, ib, ir, igb;
	mlib_u8 *prgb;

	px_abgr = (__m128i *)abgr;
	prgb = rgb;
	i = 0;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	/* 4 pixels */
	for (; i <= (n - 8); i += 4) {
		__m128i x_abgr, x_rgbz, x_r, x_g, x_b, x_temp;
		/* load & extract */
		x_abgr = _mm_loadu_si128(px_abgr);
		px_abgr++;
		x_temp = _mm_srli_si128(x_abgr, 1);
		x_g = _mm_and_si128(x_mask_g, x_temp);
		x_temp = _mm_slli_si128(x_abgr, 1);
		x_b = _mm_and_si128(x_mask_b, x_temp);
		x_temp = _mm_srli_si128(x_abgr, 3);
		x_r = _mm_and_si128(x_mask_r, x_temp);

		/* construct rgb seq */
		x_rgbz = _mm_or_si128(x_r, x_g);
		x_rgbz = _mm_or_si128(x_rgbz, x_b);

		_mm_storel_epi64((__m128i *)prgb, x_rgbz);
		x_rgbz = _mm_srli_si128(x_rgbz, 4);
		_mm_storel_epi64((__m128i *)(prgb + 3), x_rgbz);
		x_rgbz = _mm_srli_si128(x_rgbz, 4);
		_mm_storel_epi64((__m128i *)(prgb + 6), x_rgbz);
		x_rgbz = _mm_srli_si128(x_rgbz, 4);
		_mm_storel_epi64((__m128i *)(prgb + 9), x_rgbz);
		prgb += 12;
	}

	/* 4 pixels */
	if (i <= (n - 4)) {
		__m128i x_abgr, x_rgbz, x_r, x_g, x_b, x_temp;
		/* load & extract */
		x_abgr = _mm_loadu_si128(px_abgr);
		px_abgr++;
		x_temp = _mm_srli_si128(x_abgr, 1);
		x_g = _mm_and_si128(x_mask_g, x_temp);
		x_temp = _mm_slli_si128(x_abgr, 1);
		x_b = _mm_and_si128(x_mask_b, x_temp);
		x_temp = _mm_srli_si128(x_abgr, 3);
		x_r = _mm_and_si128(x_mask_r, x_temp);

		/* construct rgb seq */
		x_rgbz = _mm_or_si128(x_r, x_g);
		x_rgbz = _mm_or_si128(x_rgbz, x_b);
		PACK_RGB2(x_rgbz, prgb);

		i += 4;
	}

	/* 2 pixels */
	if (i <= (n - 2)) {
		__m128i x_abgr, x_rgbz, x_r, x_g, x_b, x_temp;
		/* load & extract */
		x_abgr = _mm_loadl_epi64(px_abgr);
		px_abgr = (__m128i *)(((__m64 *)px_abgr) + 1);
		x_temp = _mm_srli_si128(x_abgr, 1);
		x_g = _mm_and_si128(x_mask_g, x_temp);
		x_temp = _mm_slli_si128(x_abgr, 1);
		x_b = _mm_and_si128(x_mask_b, x_temp);
		x_temp = _mm_srli_si128(x_abgr, 3);
		x_r = _mm_and_si128(x_mask_r, x_temp);

		/* construct rgb seq */
		x_rgbz = _mm_or_si128(x_r, x_g);
		x_rgbz = _mm_or_si128(x_rgbz, x_b);
		PACK_RGB3(x_rgbz, prgb);

		i += 2;
	}

	/* 1 pixel */
	if (i <= (n - 1)) {
		/* load & extract */
		iabgr = *((int *)px_abgr);
		ig = (iabgr>>16) & (0xff);
		ib = iabgr & (0xff00);
		ir = (iabgr>>24) & (0xff);
		igb = ig | ib;
		*prgb = ir;
		prgb++;
		*((mlib_s16*)prgb) = (mlib_s16)igb;
		prgb += 2;

		i += 1;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
