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

#pragma ident	"@(#)mlib_VideoWaveletFwdTwoTenTrans.c	9.3	07/10/09 SMI"

/*
 * FUNCTIONS
 *      mlib_VideoWaveletForwardTwoTenTrans - two-dimensional
 *             forward reversible wavelet TT-transform
 *
 * SYNOPSIS
 *      mlib_status
 *      mlib_VideoWaveletForwardTwoTenTrans_S16_S16(mlib_s16       *dst,
 *                                                  const mlib_s16 *src,
 *                                                  mlib_s32       width,
 *                                                  mlib_s32       height,
 *                                                  mlib_s32       *level);
 *      mlib_status
 *      mlib_VideoWaveletForwardTwoTenTrans_S16_U8(mlib_s16      *dst,
 *                                                 const mlib_u8 *src,
 *                                                 mlib_s32      width,
 *                                                 mlib_s32      height,
 *                                                 mlib_s32      *level);
 *      mlib_status
 *      mlib_VideoWaveletForwardTwoTenTrans_S32_S16(mlib_s32       *dst,
 *                                                  const mlib_s16 *src,
 *                                                  mlib_s32       width,
 *                                                  mlib_s32       height,
 *                                                  mlib_s32       *level);
 *      mlib_status
 *      mlib_VideoWaveletForwardTwoTenTrans_S32_S32(mlib_s32       *dst,
 *                                                  const mlib_s32 *src,
 *                                                  mlib_s32       width,
 *                                                  mlib_s32       height,
 *                                                  mlib_s32       *level);
 *
 * ARGUMENT
 *      dst     pointer to TT-transform coefficients
 *      src     pointer to input image
 *      width   width to image
 *      height  height to image
 *      level   pointer to the number of decomposition levels.
 *              Return processed decomposition levels value.
 *
 * RESTRICTION
 *  Possible variants are following:
 *      MLIB_U8  -> MLIB_S16  input value range [0 ... 255]
 *      MLIB_S16 -> MLIB_S16  input value range [-(2^11) ... (2^11 - 1)]
 *      MLIB_S16 -> MLIB_S32  input value range [-(2^15) ... (2^15 - 1)]
 *      MLIB_S32 -> MLIB_S32  input value range [-(2^27) ... (2^27 - 1)]
 *
 * DESCRIPTION
 *
 *  input (src)
 *
 *          *-------------------*  ---
 *          |                   |    ^
 *          |                   |    |
 *          |                   |    |
 *          |                   |    | height
 *          |                   |    |
 *          |                   |    |
 *          |                   |    |
 *          *-------------------*  ---
 *
 *          |<----------------->|
 *                  width
 *
 *  output (dst, TT-transform coefficients)
 *
 *  ss_prt -->  *-------------------*  ---
 *              |        SS         |    ^
 *  ds_prt -->  *-------------------*    |
 *              |        DS         |    |
 *  sd_prt -->  *-------------------*    | height (h)
 *              |        SD         |    |
 *  dd_prt -->  *-------------------*    |
 *              |        DD         |    |
 *              *-------------------*  ---
 *
 *              |<----------------->|
 *                      width (w)
 *
 *  1st step
 *          | Original | ->   | Smooth : Detail |
 *             (src)            (s_buf : d_buf)
 *             w x h             s_buf size [(w + 1) >> 1, h]
 *                               d_buf size [w >> 1, h]
 *  2nd step
 *          | Smooth |   ->   | SS : SD |
 *            (s_buf)           (ss_prt : sd_prt)
 *
 *          | Detail |   ->   | DS : DD |
 *            (d_buf)           (ds_prt : dd_prt)
 *
 *  s_buf, d_buf - local buffers (dynamic memory allocation)
 *
 *  The 1-D forward of the TwoTen-transform.
 *
 *  TwoTen-transform is given by, (s : smooth, d : detail, o : origin)
 *
 *  s[y, x) = trunc[ { o[2 * x, y] + o[2 * x + 1, y] } / 2 ]
 *  d[y, x) = o[2x, y] - o[2* x + 1, y] + p[x, y]
 *
 *  where,
 *
 *  p[x, y] = trunc[ { 3 * s[x - 2, y] - 22 * s[x - 1, y] +
 *                    22 * s[x + 1, y]  - 3 * s[x + 2, y] + 32 } / 64 ]
 *
 *  When a one-dimensional transform is performed an odd number of components,
 *  there is one more S coefficient than D coefficients. The last S coefficient
 *  is same as the last component, and the D coefficient is zero. The boundaries
 *  of every image tile are handled by symmetric extension (mirroring).
 *
 *   2  1   1  2  3  4  5  6  7   7  6
 *   *  * | *  *  *  *  *  *  * | *  *
 *
 *  1-D forward TT-transform is performed 2 times
 *  for each level of decomposition.
 *
 *  For the next level of decomposition, the transform is repeated only
 *  on the SS coefficients.
 */

#include <mlib_video.h>
#include <mlib_VideoWaveletTwoTenTrans_f.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_VideoWaveletForwardTwoTenTrans_S16_S16 = \
	__mlib_VideoWaveletForwardTwoTenTrans_S16_S16
#pragma weak mlib_VideoWaveletForwardTwoTenTrans_S16_U8 = \
	__mlib_VideoWaveletForwardTwoTenTrans_S16_U8
#pragma weak mlib_VideoWaveletForwardTwoTenTrans_S32_S16 = \
	__mlib_VideoWaveletForwardTwoTenTrans_S32_S16
#pragma weak mlib_VideoWaveletForwardTwoTenTrans_S32_S32 = \
	__mlib_VideoWaveletForwardTwoTenTrans_S32_S32

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_VideoWaveletForwardTwoTenTrans_S16_S16)
	mlib_VideoWaveletForwardTwoTenTrans_S16_S16
	__attribute__((weak,
	alias("__mlib_VideoWaveletForwardTwoTenTrans_S16_S16")));
__typeof__(__mlib_VideoWaveletForwardTwoTenTrans_S16_U8)
	mlib_VideoWaveletForwardTwoTenTrans_S16_U8
	__attribute__((weak,
	alias("__mlib_VideoWaveletForwardTwoTenTrans_S16_U8")));
__typeof__(__mlib_VideoWaveletForwardTwoTenTrans_S32_S16)
	mlib_VideoWaveletForwardTwoTenTrans_S32_S16
	__attribute__((weak,
	alias("__mlib_VideoWaveletForwardTwoTenTrans_S32_S16")));
__typeof__(__mlib_VideoWaveletForwardTwoTenTrans_S32_S32)
	mlib_VideoWaveletForwardTwoTenTrans_S32_S32
	__attribute__((weak,
	alias("__mlib_VideoWaveletForwardTwoTenTrans_S32_S32")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	CALC_COOR                                               \
	if (x == ((x + 1) >> 1) && y == ((y + 1) >> 1))         \
	    break;                                              \
	x = (x + 1) >> 1;                                       \
	y = (y + 1) >> 1

/* *********************************************************** */

mlib_status
__mlib_VideoWaveletForwardTwoTenTrans_S16_U8(
	mlib_s16 *dst,
	const mlib_u8 *src,
	mlib_s32 width,
	mlib_s32 height,
	mlib_s32 *level)
{
	mlib_status sts;
	mlib_s32 x, y, user_level = *level;

	x = width;
	y = height;

	sts = mlib_1_Level_FTTT_S16_U8(dst, src, x, y);

	if (sts != MLIB_SUCCESS)
		return (sts);

	for (*level = 1; *level < user_level; (*level)++) {

		CALC_COOR;
		sts = mlib_1_Level_FTTT_S16_S16(dst, dst, x, y);

		if (sts != MLIB_SUCCESS)
			break;
	}

	return (sts);
}

/* *********************************************************** */

mlib_status
__mlib_VideoWaveletForwardTwoTenTrans_S16_S16(
	mlib_s16 *dst,
	const mlib_s16 *src,
	mlib_s32 width,
	mlib_s32 height,
	mlib_s32 *level)
{
	mlib_status sts;
	mlib_s32 x, y, user_level = *level;

	x = width;
	y = height;

	sts = mlib_1_Level_FTTT_S16_S16(dst, src, x, y);

	if (sts != MLIB_SUCCESS)
		return (sts);

	for (*level = 1; *level < user_level; (*level)++) {

		CALC_COOR;
		sts = mlib_1_Level_FTTT_S16_S16(dst, dst, x, y);

		if (sts != MLIB_SUCCESS)
			break;
	}

	return (sts);
}

/* *********************************************************** */

mlib_status
__mlib_VideoWaveletForwardTwoTenTrans_S32_S16(
	mlib_s32 *dst,
	const mlib_s16 *src,
	mlib_s32 width,
	mlib_s32 height,
	mlib_s32 *level)
{
	mlib_status sts;
	mlib_s32 x, y, user_level = *level;

	x = width;
	y = height;

	sts = mlib_1_Level_FTTT_S32_S16(dst, src, x, y);

	if (sts != MLIB_SUCCESS)
		return (sts);

	for (*level = 1; *level < user_level; (*level)++) {

		CALC_COOR;
		sts = mlib_1_Level_FTTT_S32_S32(dst, dst, x, y);

		if (sts != MLIB_SUCCESS)
			break;
	}

	return (sts);
}

/* *********************************************************** */

mlib_status
__mlib_VideoWaveletForwardTwoTenTrans_S32_S32(
	mlib_s32 *dst,
	const mlib_s32 *src,
	mlib_s32 width,
	mlib_s32 height,
	mlib_s32 *level)
{
	mlib_status sts;
	mlib_s32 x, y, user_level = *level;

	x = width;
	y = height;

	sts = mlib_1_Level_FTTT_S32_S32(dst, src, x, y);

	if (sts != MLIB_SUCCESS)
		return (sts);

	for (*level = 1; *level < user_level; (*level)++) {

		CALC_COOR;
		sts = mlib_1_Level_FTTT_S32_S32(dst, dst, x, y);

		if (sts != MLIB_SUCCESS)
			break;
	}

	return (sts);
}

/* *********************************************************** */
