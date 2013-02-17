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

#pragma ident	"@(#)mlib_VideoWaveletInvTwoTenTrans.c	9.3	07/10/09 SMI"

/*
 * FUNCTIONS
 *      mlib_VideoWaveletInverseTwoTenTrans - two-dimensional
 *             inverse reversible wavelet TT-transform
 *
 * SYNOPSIS
 *      mlib_status
 *      mlib_VideoWaveletInverseTwoTenTrans_S16_S16(mlib_s16       *dst,
 *                                                  const mlib_s16 *src,
 *                                                  mlib_s32       width,
 *                                                  mlib_s32       height,
 *                                                  mlib_s32       *level);
 *      mlib_status
 *      mlib_VideoWaveletInverseTwoTenTrans_S16_S32(mlib_s16       *dst,
 *                                                  const mlib_s32 *src,
 *                                                  mlib_s32       width,
 *                                                  mlib_s32       height,
 *                                                  mlib_s32       *level);
 *      mlib_status
 *      mlib_VideoWaveletInverseTwoTenTrans_S32_S32(mlib_s32       *dst,
 *                                                  const mlib_s32 *src,
 *                                                  mlib_s32       width,
 *                                                  mlib_s32       height,
 *                                                  mlib_s32       *level);
 *      mlib_status
 *      mlib_VideoWaveletInverseTwoTenTrans_U8_S16(mlib_u8        *dst,
 *                                                 const mlib_s16 *src,
 *                                                 mlib_s32       width,
 *                                                 mlib_s32       height,
 *                                                 mlib_s32       *level);
 *
 * ARGUMENT
 *      dst     pointer to output image
 *      src     pointer to TT-transform coefficients
 *      width   width to image
 *      height  height to image
 *      level   pointer to the number of decomposition levels.
 *              Return processed decomposition levels value.
 *
 * RESTRICTION
 *  Possible variants are following:
 *      MLIB_S16  -> MLIB_U8
 *      MLIB_S16  -> MLIB_S16
 *      MLIB_S32  -> MLIB_S16
 *      MLIB_S32  -> MLIB_S32
 *
 * DESCRIPTION
 *
 *  input (src, TT-transform coefficients)
 *
 *  ss_prt -->  *-------------------*  ---
 *              |        SS         |    ^
 *  ds_prt -->  *-------------------*    |
 *              |        DS         |    |
 *  sd_prt -->  *-------------------*    | height
 *              |        SD         |    |
 *  dd_prt -->  *-------------------*    |
 *              |        DD         |    |
 *              *-------------------*  ---
 *
 *              |<----------------->|
 *                      width
 *
 *  output (dst)
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
 *  1st step
 *         | SS : SD |          -> | Smooth |
 *           (ss_prt : sd_prt)       (s_buf)
 *
 *         | DS : DD |          -> | Detail |
 *           (ds_prt : dd_prt)       (d_buf)
 *
 *  2nd step
 *         | Smooth : Detail |  ->  | Original |
 *           (s_buf : d_buf)          (dst)
 *
 *  s_buf, d_buf - local buffers (dynamic memory allocation)
 *
 *  The 1-D inverse of the TwoTen-transform.
 *
 *  TwoTen-transform is given by, ( o : origin, s : smooth, d : detail)
 *
 *  o[y, 2 * x]     = s[x, y] + trunc[ { d[x, y] - p[x, y] + 1 } / 2 ]
 *  o[y, 2 * x + 1] = s[x, y] - trunc[ { d[x, y] - p[x, y] } / 2 ]
 *
 *  where,
 *
 *  p[x, y] = trunc[ { 3 * s[x - 2, y] - 22 * s[x - 1, y] +
 *                        22 * s[x + 1, y]  - 3 * s[x + 2, y] + 32 } / 64 ]
 *
 * The boundaries of every coefficient block are handled by
 * symmetric extension (mirroring).
 *
 *   2  1   1  2  3  4  5  6  7   7  6
 *   *  * | *  *  *  *  *  *  * | *  *
 *
 *  1-D inverse TT-transform is performed 2 times
 *  for each level of decomposition.
 *
 *  For the next level of decomposition, the transform is repeated only
 *  on the SS coefficients.
 */

#include <mlib_video.h>
#include <mlib_algebra.h>
#include <mlib_VideoWaveletTwoTenTrans_f.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_VideoWaveletInverseTwoTenTrans_S16_S16 = \
	__mlib_VideoWaveletInverseTwoTenTrans_S16_S16
#pragma weak mlib_VideoWaveletInverseTwoTenTrans_S16_S32 = \
	__mlib_VideoWaveletInverseTwoTenTrans_S16_S32
#pragma weak mlib_VideoWaveletInverseTwoTenTrans_S32_S32 = \
	__mlib_VideoWaveletInverseTwoTenTrans_S32_S32
#pragma weak mlib_VideoWaveletInverseTwoTenTrans_U8_S16 = \
	__mlib_VideoWaveletInverseTwoTenTrans_U8_S16

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_VideoWaveletInverseTwoTenTrans_S16_S16)
	mlib_VideoWaveletInverseTwoTenTrans_S16_S16
	__attribute__((weak,
	alias("__mlib_VideoWaveletInverseTwoTenTrans_S16_S16")));
__typeof__(__mlib_VideoWaveletInverseTwoTenTrans_S16_S32)
	mlib_VideoWaveletInverseTwoTenTrans_S16_S32
	__attribute__((weak,
	alias("__mlib_VideoWaveletInverseTwoTenTrans_S16_S32")));
__typeof__(__mlib_VideoWaveletInverseTwoTenTrans_S32_S32)
	mlib_VideoWaveletInverseTwoTenTrans_S32_S32
	__attribute__((weak,
	alias("__mlib_VideoWaveletInverseTwoTenTrans_S32_S32")));
__typeof__(__mlib_VideoWaveletInverseTwoTenTrans_U8_S16)
	mlib_VideoWaveletInverseTwoTenTrans_U8_S16
	__attribute__((weak,
	alias("__mlib_VideoWaveletInverseTwoTenTrans_U8_S16")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	CALC_COORD                                              \
	x = width;                                              \
	y = height;                                             \
	coord[0] = x;                                           \
	coord[1] = y;                                           \
	for (i = 1; (i < user_level); i++) {                    \
	    if (x == ((x + 1) >> 1) && y == ((y + 1) >> 1))     \
		break;                                          \
	    x = (x + 1) >> 1;                                   \
	    y = (y + 1) >> 1;                                   \
	    coord[2 * i] = x;                                   \
	    coord[2 * i + 1] = y;                               \
	}                                                       \
	                                                        \
	*level = i

/* *********************************************************** */

mlib_status
__mlib_VideoWaveletInverseTwoTenTrans_U8_S16(
	mlib_u8 *dst,
	const mlib_s16 *src,
	mlib_s32 width,
	mlib_s32 height,
	mlib_s32 *level)
{
	mlib_s16 *buf;
	mlib_s32 *coord, x, y, i, size, user_level = *level;
	mlib_status sts;

	size = (user_level * 2 + width * height +
		((mlib_addr)src & 7)) * sizeof (mlib_s32);
	coord = (void *)__mlib_malloc(size);

	if (coord == NULL)
		return (MLIB_FAILURE);
	buf = (void *)((mlib_u8 *)(coord + user_level * 2) +
		((mlib_addr)src & 7));

	__mlib_VectorCopy_S16(buf, src, width * height);

	CALC_COORD;
	for (i = *level - 1; i > 0; i--) {
		sts = mlib_1_Level_ITTT_S16_S16(buf, buf, coord[2 * i],
			coord[2 * i + 1]);

		if (sts != MLIB_SUCCESS)
			return (sts);
	}

	sts = mlib_1_Level_ITTT_U8_S16(dst, buf, coord[0], coord[1]);

	__mlib_free(coord);
	return (sts);
}

/* *********************************************************** */

mlib_status
__mlib_VideoWaveletInverseTwoTenTrans_S16_S16(
	mlib_s16 *dst,
	const mlib_s16 *src,
	mlib_s32 width,
	mlib_s32 height,
	mlib_s32 *level)
{
	mlib_s32 *coord, x, y, i, user_level = *level;
	mlib_status sts = MLIB_SUCCESS;

	coord = __mlib_malloc(sizeof (mlib_s32) * user_level * 2);

	if (coord == NULL)
		return (MLIB_FAILURE);

	__mlib_VectorCopy_S16(dst, src, width * height);

	CALC_COORD;
	for (i = *level - 1; i >= 0; i--) {
		sts = mlib_1_Level_ITTT_S16_S16(dst, dst, coord[2 * i],
			coord[2 * i + 1]);

		if (sts != MLIB_SUCCESS)
			return (sts);
	}

	__mlib_free(coord);
	return (sts);
}

/* *********************************************************** */

mlib_status
__mlib_VideoWaveletInverseTwoTenTrans_S16_S32(
	mlib_s16 *dst,
	const mlib_s32 *src,
	mlib_s32 width,
	mlib_s32 height,
	mlib_s32 *level)
{
	mlib_s32 *buf;
	mlib_s32 *coord, x, y, i, user_level = *level;
	mlib_status sts;

	coord = __mlib_malloc(sizeof (mlib_s32) * (user_level * 2 +
		width * height));

	if (coord == NULL)
		return (MLIB_FAILURE);
	buf = coord + user_level * 2;

	__mlib_VectorCopy_S32(buf, src, width * height);

	CALC_COORD;
	for (i = *level - 1; i > 0; i--) {
		sts = mlib_1_Level_ITTT_S32_S32(buf, buf, coord[2 * i],
			coord[2 * i + 1]);

		if (sts != MLIB_SUCCESS)
			return (sts);
	}

	sts = mlib_1_Level_ITTT_S16_S32(dst, buf, coord[2 * i],
		coord[2 * i + 1]);

	__mlib_free(coord);
	return (sts);
}

/* *********************************************************** */

mlib_status
__mlib_VideoWaveletInverseTwoTenTrans_S32_S32(
	mlib_s32 *dst,
	const mlib_s32 *src,
	mlib_s32 width,
	mlib_s32 height,
	mlib_s32 *level)
{
	mlib_s32 *coord, x, y, i, user_level = *level;
	mlib_status sts = MLIB_SUCCESS;

	coord = __mlib_malloc(sizeof (mlib_s32) * user_level * 2);

	if (coord == NULL)
		return (MLIB_FAILURE);

	__mlib_VectorCopy_S32(dst, src, width * height);

	CALC_COORD;
	for (i = *level - 1; i >= 0; i--) {
		sts = mlib_1_Level_ITTT_S32_S32(dst, dst, coord[2 * i],
			coord[2 * i + 1]);

		if (sts != MLIB_SUCCESS)
			return (sts);
	}

	__mlib_free(coord);
	return (sts);
}

/* *********************************************************** */
