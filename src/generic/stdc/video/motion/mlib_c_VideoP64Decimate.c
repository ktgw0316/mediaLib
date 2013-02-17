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
 * Copyright 2005 Sun Microsystems, Inc.  All rights reserved.
 * Use is subject to license terms.
 */

#pragma ident	"@(#)mlib_c_VideoP64Decimate.c	9.2	07/10/09 SMI"

/*
 * FUNCTION
 *      mlib_VideoP64Decimate_U8_U8 - Averages the input raster image
 *                                over 2x2 blocks and writes the results
 *                                to output raster image.
 *
 * SYNOPSIS
 *    mlib_status mlib_VideoP64Decimate_U8_U8(mlib_u8       *dst,
 *                                            const mlib_u8 *src,
 *                                            mlib_s32      width,
 *                                            mlib_s32      height,
 *                                            mlib_s32      dst_stride,
 *                                            mlib_s32      src_stride)
 *
 * ARGUMENT
 *      dst         Pointer to output raster image. Note that dst must be
 *                  8-byte aligned.
 *      src         Pointer to input raster image. Note that dst must be
 *                  8-byte aligned.
 *      width       Width of the input image
 *      height      Height of the input image
 *      dst_stride  Stride in bytes between adjacent rows in output
 *                  image. Note that dst_stride must be a multiple of 8.
 *      src_stride  Stride in bytes between adjacent rows in input
 *                  image. Note that src_stride must be a multiple of 8.
 */

#include <mlib_video.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_VideoP64Decimate_U8_U8 = __mlib_VideoP64Decimate_U8_U8

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_VideoP64Decimate_U8_U8) mlib_VideoP64Decimate_U8_U8
	__attribute__((weak, alias("__mlib_VideoP64Decimate_U8_U8")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

mlib_status
__mlib_VideoP64Decimate_U8_U8(
	mlib_u8 *dst,
	const mlib_u8 *src,
	mlib_s32 width,
	mlib_s32 height,
	mlib_s32 dst_stride,
	mlib_s32 src_stride)
{
	mlib_s32 x, y;
	const mlib_u8 *sd1, *sd2;
	mlib_u8 *dd;
	mlib_u32 src_stride2;

	sd1 = src;
	sd2 = src + src_stride;
	src_stride2 = 2 * src_stride;
	dd = dst;

	for (y = 0; y < height; y++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (x = 0; x < width; x++) {
			dd[x] = (sd1[x * 2] + sd1[x * 2 + 1] + sd2[x * 2] +
				sd2[x * 2 + 1] + 2) >> 2;
		}

		sd1 += src_stride2;
		sd2 += src_stride2;
		dd += dst_stride;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
