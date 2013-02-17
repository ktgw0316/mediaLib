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

#pragma ident	"@(#)mlib_c_VideoRvrsibleClrYUV2RGB.c	9.3	07/10/09 SMI"

/*
 * FUNCTIONS
 *      mlib_VideoReversibleColorYUV2RGB_U8_U8
 *      mlib_VideoReversibleColorYUV2RGB_U8_S16
 *      mlib_VideoReversibleColorYUV2RGB_S16_S16
 *      mlib_VideoReversibleColorYUV2RGB_S16_S32
 *                                    - convert from YUV to RGB
 *
 * SYNOPSIS
 *      mlib_status
 *      mlib_c_VideoReversibleColorYUV2RGB_S16_S16(mlib_s16       *R,
 *                                                 mlib_s16       *G,
 *                                                 mlib_s16       *B,
 *                                                 const mlib_s16 *Y,
 *                                                 const mlib_s16 *U,
 *                                                 const mlib_s16 *V,
 *                                                 mlib_s32       n,
 *                                                 mlib_s32       depth);
 *      mlib_status
 *      mlib_c_VideoReversibleColorYUV2RGB_S16_S32(mlib_s16       *R,
 *                                                 mlib_s16       *G,
 *                                                 mlib_s16       *B,
 *                                                 const mlib_s32 *Y,
 *                                                 const mlib_s32 *U,
 *                                                 const mlib_s32 *V,
 *                                                 mlib_s32       n,
 *                                                 mlib_s32       depth);
 *      mlib_status
 *      mlib_c_VideoReversibleColorYUV2RGB_U8_S16(mlib_u8        *R,
 *                                                mlib_u8        *G,
 *                                                mlib_u8        *B,
 *                                                const mlib_s16 *Y,
 *                                                const mlib_s16 *U,
 *                                                const mlib_s16 *V,
 *                                                mlib_s32       n,
 *                                                mlib_s32       depth);
 *      mlib_status
 *      mlib_c_VideoReversibleColorYUV2RGB_U8_U8(mlib_u8       *R,
 *                                               mlib_u8       *G,
 *                                               mlib_u8       *B,
 *                                               const mlib_u8 *Y,
 *                                               const mlib_u8 *U,
 *                                               const mlib_u8 *V,
 *                                               mlib_s32      n,
 *                                               mlib_s32      depth);
 *
 * ARGUMENT
 *      R     pointer to output R component
 *      G     pointer to output G component
 *      B     pointer to output B component
 *      Y     pointer to input Y component
 *      U     pointer to input U component
 *      V     pointer to input V component
 *      n     length components
 *      depth number of bit_planes required to store the original
 *            R, G, and B components
 *
 * RESTRICTION
 *  All components must have the same sizes and a aligned to 8 bytes.
 *  Possible variants are following:
 *      MLIB_U8    -> MLIB_U8            depth = 0 .. 7
 *      MLIB_S16   -> MLIB_U8            depth = 0 .. 8
 *      MLIB_S16   -> MLIB_S16           depth = 0 .. 15
 *      MLIB_S32   -> MLIB_S16           depth = 0 .. 16
 *
 * DESCRIPTION
 *
 *  G = Y - (U - 2*(2^depth - 1) + V) / 4;
 *  R = U + G - 2^depth + 1;
 *  B = V + G - 2^depth + 1;
 */

#include <mlib_video.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_VideoReversibleColorYUV2RGB_S16_S16 = \
	__mlib_VideoReversibleColorYUV2RGB_S16_S16
#pragma weak mlib_VideoReversibleColorYUV2RGB_S16_S32 = \
	__mlib_VideoReversibleColorYUV2RGB_S16_S32
#pragma weak mlib_VideoReversibleColorYUV2RGB_U8_S16 = \
	__mlib_VideoReversibleColorYUV2RGB_U8_S16
#pragma weak mlib_VideoReversibleColorYUV2RGB_U8_U8 = \
	__mlib_VideoReversibleColorYUV2RGB_U8_U8

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_VideoReversibleColorYUV2RGB_S16_S16)
	mlib_VideoReversibleColorYUV2RGB_S16_S16
	__attribute__((weak,
	alias("__mlib_VideoReversibleColorYUV2RGB_S16_S16")));
__typeof__(__mlib_VideoReversibleColorYUV2RGB_S16_S32)
	mlib_VideoReversibleColorYUV2RGB_S16_S32
	__attribute__((weak,
	alias("__mlib_VideoReversibleColorYUV2RGB_S16_S32")));
__typeof__(__mlib_VideoReversibleColorYUV2RGB_U8_S16)
	mlib_VideoReversibleColorYUV2RGB_U8_S16
	__attribute__((weak,
	alias("__mlib_VideoReversibleColorYUV2RGB_U8_S16")));
__typeof__(__mlib_VideoReversibleColorYUV2RGB_U8_U8)
	mlib_VideoReversibleColorYUV2RGB_U8_U8
	__attribute__((weak,
	alias("__mlib_VideoReversibleColorYUV2RGB_U8_U8")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

mlib_status
__mlib_VideoReversibleColorYUV2RGB_U8_U8(
	mlib_u8 *R,
	mlib_u8 *G,
	mlib_u8 *B,
	const mlib_u8 *Y,
	const mlib_u8 *U,
	const mlib_u8 *V,
	mlib_s32 n,
	mlib_s32 depth)
{
	mlib_s32 i, r, g, b, y, u, v, shift = (1 << depth) - 1;

	for (i = 0; i < n; i++) {
		y = Y[i];
		u = U[i];
		v = V[i];

		g = y - ((u - 2 * shift + v) >> 2);
		r = u + g - shift;
		b = v + g - shift;

		R[i] = r;
		G[i] = g;
		B[i] = b;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VideoReversibleColorYUV2RGB_U8_S16(
	mlib_u8 *R,
	mlib_u8 *G,
	mlib_u8 *B,
	const mlib_s16 *Y,
	const mlib_s16 *U,
	const mlib_s16 *V,
	mlib_s32 n,
	mlib_s32 depth)
{
	mlib_s32 i, r, g, b, y, u, v, shift = (1 << depth) - 1;

	for (i = 0; i < n; i++) {
		y = Y[i];
		u = U[i];
		v = V[i];

		g = y - ((u - 2 * shift + v) >> 2);
		r = u + g - shift;
		b = v + g - shift;

		R[i] = r;
		G[i] = g;
		B[i] = b;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VideoReversibleColorYUV2RGB_S16_S16(
	mlib_s16 *R,
	mlib_s16 *G,
	mlib_s16 *B,
	const mlib_s16 *Y,
	const mlib_s16 *U,
	const mlib_s16 *V,
	mlib_s32 n,
	mlib_s32 depth)
{
	mlib_s32 i, r, g, b, y, u, v, shift = (1 << depth) - 1;

	for (i = 0; i < n; i++) {
		y = Y[i];
		u = U[i];
		v = V[i];

		g = y - ((u - 2 * shift + v) >> 2);
		r = u + g - shift;
		b = v + g - shift;

		R[i] = r;
		G[i] = g;
		B[i] = b;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VideoReversibleColorYUV2RGB_S16_S32(
	mlib_s16 *R,
	mlib_s16 *G,
	mlib_s16 *B,
	const mlib_s32 *Y,
	const mlib_s32 *U,
	const mlib_s32 *V,
	mlib_s32 n,
	mlib_s32 depth)
{
	mlib_s32 i, r, g, b, y, u, v, shift = (1 << depth) - 1;

	for (i = 0; i < n; i++) {
		y = Y[i];
		u = U[i];
		v = V[i];

		g = y - ((u - 2 * shift + v) >> 2);
		r = u + g - shift;
		b = v + g - shift;

		R[i] = r;
		G[i] = g;
		B[i] = b;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
