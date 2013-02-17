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

#pragma ident	"@(#)mlib_c_VideoRvrsibleClrRGB2YUV.c	9.3	07/10/09 SMI"

/*
 * FUNCTIONS
 *      mlib_VideoReversibleColorRGB2YUV_U8_U8
 *      mlib_VideoReversibleColorRGB2YUV_S16_U8
 *      mlib_VideoReversibleColorRGB2YUV_S16_S16
 *      mlib_VideoReversibleColorRGB2YUV_S32_S16
 *                                    - convert from RGB to YUV
 *
 * SYNOPSIS
 *      mlib_status
 *      mlib_VideoReversibleColorRGB2YUV_S16_S16(mlib_s16       *Y,
 *                                               mlib_s16       *U,
 *                                               mlib_s16       *V,
 *                                               const mlib_s16 *R,
 *                                               const mlib_s16 *G,
 *                                               const mlib_s16 *B,
 *                                               mlib_s32       n,
 *                                               mlib_s32       depth);
 *      mlib_status
 *      mlib_VideoReversibleColorRGB2YUV_S16_U8(mlib_s16      *Y,
 *                                              mlib_s16      *U,
 *                                              mlib_s16      *V,
 *                                              const mlib_u8 *R,
 *                                              const mlib_u8 *G,
 *                                              const mlib_u8 *B,
 *                                              mlib_s32      n,
 *                                              mlib_s32      depth);
 *      mlib_status
 *      mlib_VideoReversibleColorRGB2YUV_S32_S16(mlib_s32       *Y,
 *                                               mlib_s32       *U,
 *                                               mlib_s32       *V,
 *                                               const mlib_s16 *R,
 *                                               const mlib_s16 *G,
 *                                               const mlib_s16 *B,
 *                                               mlib_s32       n,
 *                                               mlib_s32       depth);
 *      mlib_status
 *      mlib_VideoReversibleColorRGB2YUV_U8_U8(mlib_u8       *Y,
 *                                             mlib_u8       *U,
 *                                             mlib_u8       *V,
 *                                             const mlib_u8 *R,
 *                                             const mlib_u8 *G,
 *                                             const mlib_u8 *B,
 *                                             mlib_s32      n,
 *                                             mlib_s32      depth);
 *
 * ARGUMENT
 *      Y     pointer to output Y component
 *      U     pointer to output U component
 *      V     pointer to output V component
 *      R     pointer to input R component
 *      G     pointer to input G component
 *      B     pointer to input B component
 *      n     length components
 *      depth number of bit_planes required to store the original
 *            R, G, and B components
 *
 * RESTRICTION
 *  All components must have the same sizes and a aligned to 8 bytes.
 *  Possible variants are following:
 *      MLIB_U8   -> MLIB_U8           depth = 0 .. 7
 *      MLIB_U8   -> MLIB_S16          depth = 0 .. 8
 *      MLIB_S16  -> MLIB_S16          depth = 0 .. 15
 *      MLIB_S16  -> MLIB_S32          depth = 0 .. 16
 *
 * DESCRIPTION
 *
 *      Y = (R + 2 * G + B) / 4;
 *      U = R - G + 2^depth - 1;
 *      V = B - G + 2^depth - 1;
 */

#include <mlib_video.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_VideoReversibleColorRGB2YUV_S16_S16 = \
	__mlib_VideoReversibleColorRGB2YUV_S16_S16
#pragma weak mlib_VideoReversibleColorRGB2YUV_S16_U8 = \
	__mlib_VideoReversibleColorRGB2YUV_S16_U8
#pragma weak mlib_VideoReversibleColorRGB2YUV_S32_S16 = \
	__mlib_VideoReversibleColorRGB2YUV_S32_S16
#pragma weak mlib_VideoReversibleColorRGB2YUV_U8_U8 = \
	__mlib_VideoReversibleColorRGB2YUV_U8_U8

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_VideoReversibleColorRGB2YUV_S16_S16)
	mlib_VideoReversibleColorRGB2YUV_S16_S16
	__attribute__((weak,
	alias("__mlib_VideoReversibleColorRGB2YUV_S16_S16")));
__typeof__(__mlib_VideoReversibleColorRGB2YUV_S16_U8)
	mlib_VideoReversibleColorRGB2YUV_S16_U8
	__attribute__((weak,
	alias("__mlib_VideoReversibleColorRGB2YUV_S16_U8")));
__typeof__(__mlib_VideoReversibleColorRGB2YUV_S32_S16)
	mlib_VideoReversibleColorRGB2YUV_S32_S16
	__attribute__((weak,
	alias("__mlib_VideoReversibleColorRGB2YUV_S32_S16")));
__typeof__(__mlib_VideoReversibleColorRGB2YUV_U8_U8)
	mlib_VideoReversibleColorRGB2YUV_U8_U8
	__attribute__((weak,
	alias("__mlib_VideoReversibleColorRGB2YUV_U8_U8")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

mlib_status
__mlib_VideoReversibleColorRGB2YUV_U8_U8(
	mlib_u8 *Y,
	mlib_u8 *U,
	mlib_u8 *V,
	const mlib_u8 *R,
	const mlib_u8 *G,
	const mlib_u8 *B,
	mlib_s32 n,
	mlib_s32 depth)
{
	mlib_s32 i, shift = (1 << depth) - 1, r, g, b;

	for (i = 0; i < n; i++) {
		r = R[i];
		g = G[i];
		b = B[i];

		Y[i] = (r + 2 * g + b) >> 2;
		U[i] = r - g + shift;
		V[i] = b - g + shift;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VideoReversibleColorRGB2YUV_S16_U8(
	mlib_s16 *Y,
	mlib_s16 *U,
	mlib_s16 *V,
	const mlib_u8 *R,
	const mlib_u8 *G,
	const mlib_u8 *B,
	mlib_s32 n,
	mlib_s32 depth)
{
	mlib_s32 i, shift = (1 << depth) - 1, r, g, b;

	for (i = 0; i < n; i++) {
		r = R[i];
		g = G[i];
		b = B[i];

		Y[i] = (r + 2 * g + b) >> 2;
		U[i] = r - g + shift;
		V[i] = b - g + shift;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VideoReversibleColorRGB2YUV_S16_S16(
	mlib_s16 *Y,
	mlib_s16 *U,
	mlib_s16 *V,
	const mlib_s16 *R,
	const mlib_s16 *G,
	const mlib_s16 *B,
	mlib_s32 n,
	mlib_s32 depth)
{
	mlib_s32 i, shift = (1 << depth) - 1, r, g, b;

	for (i = 0; i < n; i++) {
		r = R[i];
		g = G[i];
		b = B[i];

		Y[i] = (r + 2 * g + b) >> 2;
		U[i] = r - g + shift;
		V[i] = b - g + shift;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VideoReversibleColorRGB2YUV_S32_S16(
	mlib_s32 *Y,
	mlib_s32 *U,
	mlib_s32 *V,
	const mlib_s16 *R,
	const mlib_s16 *G,
	const mlib_s16 *B,
	mlib_s32 n,
	mlib_s32 depth)
{
	mlib_s32 i, shift = (1 << depth) - 1, r, g, b;

	for (i = 0; i < n; i++) {
		r = R[i];
		g = G[i];
		b = B[i];

		Y[i] = (r + 2 * g + b) >> 2;
		U[i] = r - g + shift;
		V[i] = b - g + shift;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
