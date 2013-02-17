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

#pragma ident	"@(#)mlib_s_ImageCopy_Bit.c	9.3	07/11/05 SMI"

/*
 * FUNCTIONS
 *      mlib_ImageCopy_bit_na     - BIT, non-aligned
 *      mlib_ImageCopy_bit_na_r   - BIT, non-aligned, reverse
 *
 * SYNOPSIS
 *
 *      void mlib_ImageCopy_bit_na(const mlib_u8 *sa,
 *                                 mlib_u8       *da,
 *                                 mlib_s32      size,
 *                                 mlib_s32      s_offset,
 *                                 mlib_s32      d_offset);
 *      void mlib_ImageCopy_bit_na_r(const mlib_u8 *sa,
 *                                   mlib_u8       *da,
 *                                   mlib_s32      size,
 *                                   mlib_s32      s_offset,
 *                                   mlib_s32      d_offset);
 * ARGUMENT
 *      sp       pointer to source image data
 *      dp       pointer to destination image data
 *      size     size in 8-bytes, bytes, or SHORTs
 *      width    image width in 8-bytes
 *      height   image height in lines
 *      stride   source image line stride in 8-bytes
 *      dstride  destination image line stride in 8-bytes
 *      s_offset source image line bit offset
 *      d_offset destination image line bit offset
 *
 * DESCRIPTION
 *      Direct copy from one image to another -- C version low level
 *      functions.
 */

#ifdef __SUNPRO_C
#include <sunmedia_intrin.h>
#else
#include <emmintrin.h>
#endif

#include <mlib_image.h>
#include <mlib_ImageCopy.h>

/* *********************************************************** */

/*
 * Bit offsets of source and destination are not the same
 */

void
mlib_ImageCopy_bit_na(
    const mlib_u8 *sa,
    mlib_u8 *da,
    mlib_s32 size,
    mlib_s32 s_offset,
    mlib_s32 d_offset)
{
	mlib_u8 *dp = (mlib_u8 *)da;
	mlib_u8 *sp = (mlib_u8 *)sa;

	mlib_s32 j, shift;
	mlib_u8 mask0 = 0xFF;
	mlib_u8 dmask, hmask, tmask;
	mlib_u8 src, src0, src1 = 0, dst;

	if (size <= 0)
		return;

	hmask = mask0 >> d_offset;
	src0 = sp[0];
	dst = dp[0];

	if (s_offset > d_offset) {
		if (s_offset + size > 8)
			src1 = sp[1];
		if (d_offset + size < 8) {
			tmask = mask0 << (8 - size - d_offset);
			dmask = tmask & hmask;
			src = (src0 << (s_offset - d_offset)) |
				(src1 >> (8 - (s_offset - d_offset)));
			*dp = (src & dmask) | (~dmask & dst);
			return;
		}
		dmask = hmask;
		shift = s_offset - d_offset;
		src = ((src0 << shift) | (src1 >> (8 - shift)));
		*dp = (src & dmask) | (~dmask & dst);
		dp++;
		sp++;
		j = 8 - d_offset;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif
		for (; j < size - 8; j += 8) {
			src0 = src1;
			src1 = sp[1];
			src = ((src0 << shift) |
				(src1 >> (8 - shift)));
			*dp = src;
			sp++;
			dp++;
		}

		if (j < size) {
			dmask = mask0 << (8 - (size - j));
			dst = dp[0];
			src0 = src1;
			src1 = sp[1];
			src = ((src0 << shift) | (src1 >> (8 - shift)));
			*dp = (src & dmask) | (~dmask & dst);
		}

	} else {
		if (d_offset + size < 8) {
			tmask = mask0 << (8 - size - d_offset);
			dmask = tmask & hmask;
			src = src0 >> (d_offset - s_offset);
			*dp = (src & dmask) | (~dmask & dst);
			return;
		}

		dmask = hmask;
		src = src0 >> (d_offset - s_offset);
		*dp = (src & dmask) | (~dmask & dst);

		dp++;
		sp++;

		j = 8 - d_offset;
		shift = d_offset - s_offset;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif
		for (; j < size - 8; j += 8) {
			src1 = sp[0];
			src = ((src0 << (8 - shift)) |
				(src1 >>  shift));
			*dp = src;
			src0 = src1;
			sp++;
			dp++;
		}

		if (j < size) {
			dmask = mask0 << (8 - (size - j));
			src1 = sp[0];
			dst = dp[0];
			src = (src0 << (8 - shift) |
				(src1 >>  shift));
			*dp = (src & dmask) | (~dmask & dst);
		}
	}
}
/* *********************************************************** */

/*
 * Bit offsets of source and destination are not the same
 */

void
mlib_ImageCopy_bit_na_r(
    const mlib_u8 *sa,
    mlib_u8 *da,
    mlib_s32 size,
    mlib_s32 s_offset,
    mlib_s32 d_offset)
{

/* 4-byte aligned start points in dst */
	mlib_u32 *dp;

/* 4-byte aligned start point in src */
	mlib_u32 *sp;

/* offset of address in dst */
	mlib_s32 j;
	mlib_u32 lmask0 = 0xFFFFFFFF;
	mlib_u32 dmask;
	mlib_u32 src, src0, src1 = 0, dst;

	mlib_s32 ls_offset, ld_offset, shift;

	if (size <= 0)
		return;

/* prepare the destination addresses */
	dp = (mlib_u32 *)((mlib_addr)da & (~3));
	sp = (mlib_u32 *)((mlib_addr)sa & (~3));
/* bit d_offset to first mlib_s32 */
	ld_offset = (((mlib_addr)da & 3) << 3) + d_offset;
/* bit d_offset to first mlib_s32 */
	ls_offset = (((mlib_addr)sa & 3) << 3) + s_offset;

	if (ld_offset < ls_offset) {
		src0 = sp[0];
		dst = dp[0];

		if (ld_offset >= size) {
			dmask = (lmask0 << (32 - size)) >> (ld_offset - size);
			src0 =
			    (src0 << 24) | ((src0 & 0xFF00) << 8) | ((src0 >> 8)
			    & 0xFF00) | (src0 >> 24);
			src = (src0 << (ls_offset - ld_offset));
			dst =
			    (dst << 24) | ((dst & 0xFF00) << 8) | ((dst >> 8) &
			    0xFF00) | (dst >> 24);
			dst = (dst & (~dmask)) | (src & dmask);
			dp[0] =
			    (dst << 24) | ((dst & 0xFF00) << 8) | ((dst >> 8) &
			    0xFF00) | (dst >> 24);
			    return;
		}

		dmask = lmask0 << (32 - ld_offset);
		src0 =
		    (src0 << 24) | ((src0 & 0xFF00) << 8) | ((src0 >> 8) &
		    0xFF00) | (src0 >> 24);
		src = (src0 << (ls_offset - ld_offset));
		dst =
		    (dst << 24) | ((dst & 0xFF00) << 8) | ((dst >> 8) & 0xFF00)
		    | (dst >> 24);
		dst = (dst & ~dmask) | (src & dmask);
		dp[0] =
		    (dst << 24) | ((dst & 0xFF00) << 8) | ((dst >> 8) & 0xFF00)
		    | (dst >> 24);
		    j = ld_offset;
		dp--;
		ls_offset -= j;
	} else {

		shift = ld_offset - ls_offset;
		src0 = sp[0];

		if (ls_offset < size)
			src1 = sp[-1];
		dst = dp[0];

		if (ld_offset >= size) {
			dmask = (lmask0 << (32 - size)) >> (ld_offset - size);
			src0 =
			    (src0 << 24) | ((src0 & 0xFF00) << 8) | ((src0 >> 8)
			    & 0xFF00) | (src0 >> 24);
			src1 =
			    (src1 << 24) | ((src1 & 0xFF00) << 8) | ((src1 >> 8)
			    & 0xFF00) | (src1 >> 24);
			src = (src0 >> shift) | (src1 << (32 - shift));
			dst =
			    (dst << 24) | ((dst & 0xFF00) << 8) | ((dst >> 8) &
			    0xFF00) | (dst >> 24);
			dst = (dst & ~dmask) | (src & dmask);
			dp[0] =
			    (dst << 24) | ((dst & 0xFF00) << 8) | ((dst >> 8) &
			    0xFF00) | (dst >> 24);
			    return;
		}

		dmask = lmask0 << (32 - ld_offset);
		src0 =
		    (src0 << 24) | ((src0 & 0xFF00) << 8) | ((src0 >> 8) &
		    0xFF00) | (src0 >> 24);
		src1 =
		    (src1 << 24) | ((src1 & 0xFF00) << 8) | ((src1 >> 8) &
		    0xFF00) | (src1 >> 24);
		src = (src0 >> shift) | (src1 << (32 - shift));
		dst =
		    (dst << 24) | ((dst & 0xFF00) << 8) | ((dst >> 8) & 0xFF00)
		    | (dst >> 24);
		dst = (dst & ~dmask) | (src & dmask);
		dp[0] =
		    (dst << 24) | ((dst & 0xFF00) << 8) | ((dst >> 8) & 0xFF00)
		    | (dst >> 24);
		    j = ld_offset;
		dp--;
		sp--;
		ls_offset = ls_offset - j + 32;
	}

	if (j < size)
		src1 = sp[0];
	src1 =
	    (src1 << 24) | ((src1 & 0xFF00) << 8) | ((src1 >> 8) & 0xFF00) |
	    (src1 >> 24);
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (; j <= size - 32; j += 32) {
		src0 = src1;
		src1 = sp[-1];
		src1 =
		    (src1 << 24) | ((src1 & 0xFF00) << 8) | ((src1 >> 8) &
		    0xFF00) | (src1 >> 24);
		src = (src0 >> (32 - ls_offset)) | (src1 << ls_offset);
		dp[0] =
		    (src << 24) | ((src & 0xFF00) << 8) | ((src >> 8) & 0xFF00)
		    | (src >> 24);
		sp--;
		dp--;
	}

	if (j < size) {
		j = size - j;
		src0 = src1;

		if (ls_offset < j)
			src1 = sp[-1];
		dst = dp[0];
		dmask = lmask0 >> (32 - j);
		src1 =
		    (src1 << 24) | ((src1 & 0xFF00) << 8) | ((src1 >> 8) &
		    0xFF00) | (src1 >> 24);
		src = (src0 >> (32 - ls_offset)) | (src1 << ls_offset);
		dst =
		    (dst << 24) | ((dst & 0xFF00) << 8) | ((dst >> 8) & 0xFF00)
		    | (dst >> 24);
		dst = (dst & ~dmask) | (src & dmask);
		dp[0] =
		    (dst << 24) | ((dst & 0xFF00) << 8) | ((dst >> 8) & 0xFF00)
		    | (dst >> 24);
	}
}

/* *********************************************************** */
