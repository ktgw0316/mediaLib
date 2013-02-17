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

#pragma ident	"@(#)mlib_s_VideoColorARGB2RGB.c	9.5	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_VideoColorARGB2RGB - ARGB to RGB conversion
 *
 * SYNOPSIS
 *      mlib_status mlib_VideoColorARGB2RGB(mlib_u8       *rgb,
 *                                          const mlib_u8 *argb,
 *                                          mlib_s32      n)
 *
 * ARGUMENTS
 *      rgb       Pointer to  RGB multicomponent row, 8-byte aligned
 *      argb      Pointer to ARGB multicomponent row, 8-byte aligned
 *      n         Number of pixels
 *
 * DESCRIPTION
 *
 *      rgb[3*i  ] = argb[4*i+1]
 *      rgb[3*i+1] = argb[4*i+2]
 *      rgb[3*i+2] = argb[4*i+3]
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

#pragma weak mlib_VideoColorARGB2RGB = __mlib_VideoColorARGB2RGB

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_VideoColorARGB2RGB) mlib_VideoColorARGB2RGB
	__attribute__((weak, alias("__mlib_VideoColorARGB2RGB")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

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
mlib_VideoColorARGB2RGB_aligned(
	mlib_u8 *rgb,
	const mlib_u8 *argb,
	mlib_s32 n);

mlib_status
mlib_VideoColorARGB2RGB_naligned(
	mlib_u8 *rgb,
	const mlib_u8 *argb,
	mlib_s32 n);

mlib_status
__mlib_VideoColorARGB2RGB(
	mlib_u8 *rgb,
	const mlib_u8 *argb,
	mlib_s32 n)
{
	if (rgb == NULL || argb == NULL)
		return (MLIB_NULLPOINTER);

	if (n <= 0)
		return (MLIB_FAILURE);

	if (0 == (0xf & (mlib_addr)argb)) {
		return mlib_VideoColorARGB2RGB_aligned(
			rgb, argb, n);
	} else {
		return mlib_VideoColorARGB2RGB_naligned(
			rgb, argb, n);
	}
}

mlib_status
mlib_VideoColorARGB2RGB_aligned(
	mlib_u8 *rgb,
	const mlib_u8 *argb,
	mlib_s32 n)
{
	__m128i *px_argb;
	mlib_s32 i;
	mlib_u8 *prgb;

	px_argb = (__m128i *)argb;
	prgb = rgb;
	i = 0;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	/* 4 pixels */
	for (; i <= (n - 8); i += 4) {
		__m128i x_argb;
		x_argb = _mm_load_si128(px_argb);
		px_argb++;
		x_argb = _mm_srli_si128(x_argb, 1);
		_mm_storel_epi64((__m128i *)prgb, x_argb);
		x_argb = _mm_srli_si128(x_argb, 4);
		_mm_storel_epi64((__m128i *)(prgb+3), x_argb);
		x_argb = _mm_srli_si128(x_argb, 4);
		_mm_storel_epi64((__m128i *)(prgb+6), x_argb);
		x_argb = _mm_srli_si128(x_argb, 4);
		_mm_storel_epi64((__m128i *)(prgb+9), x_argb);
		prgb += 12;
	}

	/* 4 pixels */
	if (i <= (n - 4)) {
		__m128i x_argb;
		x_argb = _mm_load_si128(px_argb);
		px_argb++;
		x_argb = _mm_srli_si128(x_argb, 1);
		PACK_RGB2(x_argb, prgb);

		i += 4;
	}

	/* 2 pixels */
	if (i <= (n - 2)) {
		__m128i x_argb;
		x_argb = _mm_loadl_epi64(px_argb);
		px_argb = (__m128i *)(((__m64 *)px_argb) + 1);
		x_argb = _mm_srli_si128(x_argb, 1);
		PACK_RGB3(x_argb, prgb);

		i += 2;
	}

	/* 1 pixel */
	if (i <= (n - 1)) {
		*prgb = *(((mlib_u8 *)px_argb)+1);
		prgb++;
		*((mlib_s16 *)prgb) = *((mlib_s16 *)(((mlib_u8 *)px_argb)+2));

		i += 1;
	}

	return (MLIB_SUCCESS);
}

mlib_status
mlib_VideoColorARGB2RGB_naligned(
	mlib_u8 *rgb,
	const mlib_u8 *argb,
	mlib_s32 n)
{
	__m128i *px_argb;
	mlib_s32 i;
	mlib_u8 *prgb;

	px_argb = (__m128i *)argb;
	prgb = rgb;
	i = 0;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	/* 4 pixels */
	for (; i <= (n - 8); i += 4) {
		__m128i x_argb;
		x_argb = _mm_loadu_si128(px_argb);
		px_argb++;
		x_argb = _mm_srli_si128(x_argb, 1);
		_mm_storel_epi64((__m128i *)prgb, x_argb);
		x_argb = _mm_srli_si128(x_argb, 4);
		_mm_storel_epi64((__m128i *)(prgb+3), x_argb);
		x_argb = _mm_srli_si128(x_argb, 4);
		_mm_storel_epi64((__m128i *)(prgb+6), x_argb);
		x_argb = _mm_srli_si128(x_argb, 4);
		_mm_storel_epi64((__m128i *)(prgb+9), x_argb);
		prgb += 12;
	}

	/* 4 pixels */
	if (i <= (n - 4)) {
		__m128i x_argb;
		x_argb = _mm_loadu_si128(px_argb);
		px_argb++;
		x_argb = _mm_srli_si128(x_argb, 1);
		PACK_RGB2(x_argb, prgb);

		i += 4;
	}

	/* 2 pixels */
	if (i <= (n - 2)) {
		__m128i x_argb;
		x_argb = _mm_loadl_epi64(px_argb);
		px_argb = (__m128i *)(((__m64 *)px_argb) + 1);
		x_argb = _mm_srli_si128(x_argb, 1);
		PACK_RGB3(x_argb, prgb);

		i += 2;
	}

	/* 1 pixel */
	if (i <= (n - 1)) {
		*prgb = *(((mlib_u8 *)px_argb)+1);
		prgb++;
		*((mlib_s16 *)prgb) = *((mlib_s16 *)(((mlib_u8 *)px_argb)+2));

		i += 1;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
