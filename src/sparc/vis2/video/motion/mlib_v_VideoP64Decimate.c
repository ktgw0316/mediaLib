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

#pragma ident	"@(#)mlib_v_VideoP64Decimate.c	9.2	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_VideoP64Decimate_U8_U8 - Averages the input raster image
 *                                over 2x2 blocks and writes the results
 *                                to output raster image.
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_VideoP64Decimate_U8_U8(mlib_u8       *dst,
 *                                              const mlib_u8 *src,
 *                                              mlib_s32      width,
 *                                              mlib_s32      height,
 *                                              mlib_s32      dst_stride,
 *                                              mlib_s32      src_stride);
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

#include <vis_proto.h>
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
	mlib_s32 x, y, x4 = width >> 2;
	mlib_d64 *sl1, *sl2, s1hi, s1lo, s2hi, s2lo, s1, s2;
	mlib_d64 done = vis_to_double_dup(0x1000100);
	mlib_d64 zero = vis_fzero();
	mlib_d64 dmask;
	mlib_f32 *dp;
	mlib_f32 frnd = vis_to_float(0x40404040);
	mlib_s32 src_stride2 = 2 * src_stride;

	dmask = vis_fpadd16(done, vis_fone());
	vis_write_gsr(7 << 3);
	vis_write_bmask(0x80828486, 0);
	sl1 = (mlib_d64 *)src;
	sl2 = (mlib_d64 *)(src + src_stride);
	dp = (mlib_f32 *)dst;

	for (y = 0; y < height; y++) {
#pragma pipeloop(0)
#pragma unroll(4)
		for (x = 0; x < x4; x++) {
			s1 = sl1[x];
			s2 = sl2[x];
			s1lo = vis_fand(s1, dmask);
			s2lo = vis_fand(s2, dmask);
			s1hi = vis_bshuffle(s1, zero);
			s2hi = vis_bshuffle(s2, zero);
			s1hi = vis_fpadd16(s1hi, s2hi);
			s1lo = vis_fpadd16(s1lo, s2lo);
			s1hi = vis_fpadd16(s1hi, s1lo);
			s1hi = vis_fmul8x16(frnd, s1hi);
			dp[x] = vis_fpack16(s1hi);
		}

		dp = (mlib_f32 *)((mlib_u8 *)dp + dst_stride);
		sl1 = (mlib_d64 *)((mlib_u8 *)sl1 + src_stride2);
		sl2 = (mlib_d64 *)((mlib_u8 *)sl2 + src_stride2);
	}

	if (width & 3) {
		const mlib_u8 *sd1, *sd2, *sll1, *sll2;
		mlib_u8 *dl, *dd;

		x = width & ~3;
		sll1 = sd1 = src + 2 * x;
		sll2 = sd2 = sll1 + src_stride;
		dl = dd = dst + x;
		for (; x < width; x++) {
			for (y = 0; y < height; y++) {
				*dd = (*sd1 + *(sd1 + 1) + *(sd2) + *(sd2 + 1) +
					2) >> 2;
				sd1 += src_stride2;
				sd2 += src_stride2;
				dd += dst_stride;
			}

			sll1 = sd1 = sll1 + 2;
			sll2 = sd2 = sll2 + 2;
			dl = dd = dl + 1;
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
