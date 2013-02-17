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

#pragma ident	"@(#)mlib_ImageCopy_Bit.c	9.3	07/10/09 SMI"

/*
 * FUNCTIONS
 *      mlib_ImageCopy_bit_na     - BIT, non-aligned
 *      mlib_ImageCopy_bit_na_r   - BIT, non-aligned, reverse
 *
 * SYNOPSIS
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
 *
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

#include <mlib_image.h>
#include <mlib_ImageCopy.h>

/* *********************************************************** */

#define	STORE_MASK(res, left, right, shift)                     \
	res >>= (shift);                                        \
	for (k = (right); k >= (left); k--) {                   \
	    ((mlib_u8 *)dp)[k] = res;                           \
	    res >>= 8;                                          \
	}

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
#if defined(_NO_LONGLONG) || defined(_LITTLE_ENDIAN)

/* 4-byte aligned start points in dst */
	mlib_u32 *dp;

/* 4-byte aligned start point in src */
	mlib_u32 *sp;

/* offset of address in dst */
	mlib_s32 j;
	mlib_u32 mask0 = 0xFFFFFFFF;
	mlib_u32 dmask;
	mlib_u32 src, src0, src1 = 0, dst;

	mlib_s32 ls_offset, ld_offset, shift, k;

	if (size <= 0)
		return;

/* prepare the destination addresses */
	dp = (mlib_u32 *)((mlib_addr)da & (~3));
	sp = (mlib_u32 *)((mlib_addr)sa & (~3));
/* bit d_offset to first mlib_s32 */
	ld_offset = (((mlib_addr)da & 3) << 3) + d_offset;
/* bit d_offset to first mlib_s32 */
	ls_offset = (((mlib_addr)sa & 3) << 3) + s_offset;

	if (ld_offset > ls_offset) {
		src0 = sp[0];
		dst = dp[0];

		if (ld_offset + size < 32) {
			dmask = (mask0 << (32 - size)) >> ld_offset;
#ifdef _LITTLE_ENDIAN
			src0 =
			    (src0 << 24) | ((src0 & 0xFF00) << 8) | ((src0 >> 8)
			    & 0xFF00) | (src0 >> 24);
			src = (src0 >> (ld_offset - ls_offset));
			dst =
			    (dst << 24) | ((dst & 0xFF00) << 8) | ((dst >> 8) &
			    0xFF00) | (dst >> 24);
			dst = (dst & (~dmask)) | (src & dmask);
#else /* _LITTLE_ENDIAN */
			src = (src0 >> (ld_offset - ls_offset));
			dst = (dst & (~dmask)) | (src & dmask);
#endif /* _LITTLE_ENDIAN */
			STORE_MASK(dst, (ld_offset >> 3),
			    ((ld_offset + size - 1) >> 3),
			    ((32 - size - ld_offset) & ~7))
			    return;
		}

		dmask = mask0 >> ld_offset;
#ifdef _LITTLE_ENDIAN
		src0 =
		    (src0 << 24) | ((src0 & 0xFF00) << 8) | ((src0 >> 8) &
		    0xFF00) | (src0 >> 24);
		src = (src0 >> (ld_offset - ls_offset));
		dst =
		    (dst << 24) | ((dst & 0xFF00) << 8) | ((dst >> 8) & 0xFF00)
		    | (dst >> 24);
		dst = (dst & ~dmask) | (src & dmask);
#else /* _LITTLE_ENDIAN */
		src = (src0 >> (ld_offset - ls_offset));
		dst = (dst & ~dmask) | (src & dmask);
#endif /* _LITTLE_ENDIAN */
		STORE_MASK(dst, (ld_offset >> 3), 3, 0)
		    j = 32 - ld_offset;
		dp++;
		ls_offset += j;
	} else {

		shift = ls_offset - ld_offset;
		src0 = sp[0];

		if (ls_offset + size > 32)
			src1 = sp[1];
		dst = dp[0];

		if (ld_offset + size < 32) {
			dmask = (mask0 << (32 - size)) >> ld_offset;
#ifdef _LITTLE_ENDIAN
			src0 =
			    (src0 << 24) | ((src0 & 0xFF00) << 8) | ((src0 >> 8)
			    & 0xFF00) | (src0 >> 24);
			src1 =
			    (src1 << 24) | ((src1 & 0xFF00) << 8) | ((src1 >> 8)
			    & 0xFF00) | (src1 >> 24);
			src = (src0 << shift) | (src1 >> (32 - shift));
			dst =
			    (dst << 24) | ((dst & 0xFF00) << 8) | ((dst >> 8) &
			    0xFF00) | (dst >> 24);
			dst = (dst & ~dmask) | (src & dmask);

#else /* _LITTLE_ENDIAN */
			src = (src0 << shift) | (src1 >> (32 - shift));
			dst = (dst & ~dmask) | (src & dmask);
#endif /* _LITTLE_ENDIAN */
			STORE_MASK(dst, (ld_offset >> 3),
			    ((ld_offset + size - 1) >> 3),
			    ((32 - size - ld_offset) & ~7))
			    return;
		}

		dmask = mask0 >> ld_offset;
#ifdef _LITTLE_ENDIAN
		src0 =
		    (src0 << 24) | ((src0 & 0xFF00) << 8) | ((src0 >> 8) &
		    0xFF00) | (src0 >> 24);
		src1 =
		    (src1 << 24) | ((src1 & 0xFF00) << 8) | ((src1 >> 8) &
		    0xFF00) | (src1 >> 24);
		src = (src0 << shift) | (src1 >> (32 - shift));
		dst =
		    (dst << 24) | ((dst & 0xFF00) << 8) | ((dst >> 8) & 0xFF00)
		    | (dst >> 24);
		dst = (dst & ~dmask) | (src & dmask);
#else /* _LITTLE_ENDIAN */
		src = (src0 << shift) | (src1 >> (32 - shift));
		dst = (dst & ~dmask) | (src & dmask);
#endif /* _LITTLE_ENDIAN */
		STORE_MASK(dst, (ld_offset >> 3), 3, 0)
		    j = 32 - ld_offset;
		dp++;
		sp++;
		ls_offset = ls_offset + j - 32;
	}

	if (j < size)
		src1 = sp[0];
#ifdef _LITTLE_ENDIAN
	src1 =
	    (src1 << 24) | ((src1 & 0xFF00) << 8) | ((src1 >> 8) & 0xFF00) |
	    (src1 >> 24);
#endif /* _LITTLE_ENDIAN */
	for (; j <= size - 32; j += 32) {
		src0 = src1;
		src1 = sp[1];
#ifdef _LITTLE_ENDIAN
		src1 =
		    (src1 << 24) | ((src1 & 0xFF00) << 8) | ((src1 >> 8) &
		    0xFF00) | (src1 >> 24);
		src = (src0 << ls_offset) | (src1 >> (32 - ls_offset));
		dp[0] =
		    (src << 24) | ((src & 0xFF00) << 8) | ((src >> 8) & 0xFF00)
		    | (src >> 24);
#else /* _LITTLE_ENDIAN */
		dp[0] = (src0 << ls_offset) | (src1 >> (32 - ls_offset));
#endif /* _LITTLE_ENDIAN */
		sp++;
		dp++;
	}

	if (j < size) {
		j = size - j;
		src0 = src1;

		if (ls_offset + j > 32)
			src1 = sp[1];
		dst = dp[0];
		dmask = mask0 << (32 - j);
#ifdef _LITTLE_ENDIAN
		src1 =
		    (src1 << 24) | ((src1 & 0xFF00) << 8) | ((src1 >> 8) &
		    0xFF00) | (src1 >> 24);
		src = (src0 << ls_offset) | (src1 >> (32 - ls_offset));
		dst =
		    (dst << 24) | ((dst & 0xFF00) << 8) | ((dst >> 8) & 0xFF00)
		    | (dst >> 24);
		dst = (dst & ~dmask) | (src & dmask);
#else /* _LITTLE_ENDIAN */
		src = (src0 << ls_offset) | (src1 >> (32 - ls_offset));
		dst = (dst & ~dmask) | (src & dmask);
#endif /* _LITTLE_ENDIAN */
		STORE_MASK(dst, 0, ((j - 1) >> 3), ((32 - j) & ~7))
	}
#else /* _NO_LONGLONG || _LITTLE_ENDIAN */

/* 8-byte aligned start points in dst */
	mlib_u64 *dp;

/* 8-byte aligned start point in src */
	mlib_u64 *sp;

/* offset of address in dst */
	mlib_s32 j;
	mlib_u64 lmask0 = 0xFFFFFFFFFFFFFFFF;
	mlib_u64 dmask;
	mlib_u64 lsrc, lsrc0, lsrc1, ldst;
	mlib_s32 ls_offset, ld_offset, shift, k;

	if (size <= 0)
		return;

/* prepare the destination addresses */
	dp = (mlib_u64 *)((mlib_addr)da & (~7));
	sp = (mlib_u64 *)((mlib_addr)sa & (~7));
/* bit d_offset to first mlib_d64 */
	ld_offset = (((mlib_addr)da & 7) << 3) + d_offset;
/* bit d_offset to first mlib_d64 */
	ls_offset = (((mlib_addr)sa & 7) << 3) + s_offset;

	if (ld_offset > ls_offset) {
		lsrc0 = sp[0];
		ldst = dp[0];
		lsrc = (lsrc0 >> (ld_offset - ls_offset));

		if (ld_offset + size < 64) {
			dmask = (lmask0 << (64 - size)) >> ld_offset;
			ldst = (ldst & (~dmask)) | (lsrc & dmask);
			STORE_MASK(ldst, (ld_offset >> 3),
			    ((ld_offset + size - 1) >> 3),
			    ((64 - size - ld_offset) & ~7))
			    return;
		}

		dmask = lmask0 >> ld_offset;
		ldst = (ldst & ~dmask) | (lsrc & dmask);
		STORE_MASK(ldst, (ld_offset >> 3), 7, 0)
		    j = 64 - ld_offset;
		dp++;
		ls_offset += j;
	} else {

		shift = ls_offset - ld_offset;
		lsrc0 = sp[0];

		if (ls_offset + size > 64)
			lsrc1 = sp[1];
		ldst = dp[0];
		lsrc = (lsrc0 << shift) | (lsrc1 >> (64 - shift));

		if (ld_offset + size < 64) {
			dmask = (lmask0 << (64 - size)) >> ld_offset;
			ldst = (ldst & ~dmask) | (lsrc & dmask);
			STORE_MASK(ldst, (ld_offset >> 3),
			    ((ld_offset + size - 1) >> 3),
			    ((64 - size - ld_offset) & ~7))
			    return;
		}

		dmask = lmask0 >> ld_offset;
		ldst = (ldst & ~dmask) | (lsrc & dmask);
		STORE_MASK(ldst, (ld_offset >> 3), 7, 0)
		    j = 64 - ld_offset;
		dp++;
		sp++;
		ls_offset = ls_offset + j - 64;
	}

	if (j < size)
		lsrc1 = sp[0];
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (; j <= size - 64; j += 64) {
		lsrc0 = lsrc1;
		lsrc1 = sp[1];
		lsrc = (lsrc0 << ls_offset) | (lsrc1 >> (64 - ls_offset));
		dp[0] = lsrc;
		sp++;
		dp++;
	}

	if (j < size) {
		j = size - j;
		lsrc0 = lsrc1;

		if (ls_offset + j > 64)
			lsrc1 = sp[1];
		ldst = dp[0];
		dmask = lmask0 << (64 - j);
		lsrc = (lsrc0 << ls_offset) | (lsrc1 >> (64 - ls_offset));
		ldst = (ldst & ~dmask) | (lsrc & dmask);
		STORE_MASK(ldst, 0, ((j - 1) >> 3), ((64 - j) & ~7))
	}
#endif /* _NO_LONGLONG || _LITTLE_ENDIAN */
}

/* *********************************************************** */

/*
 * Bit offsets of source and destination are not the same
 * This function is both for C and VIS version (LONGLONG case)
 */

void
mlib_ImageCopy_bit_na_r(
    const mlib_u8 *sa,
    mlib_u8 *da,
    mlib_s32 size,
    mlib_s32 s_offset,
    mlib_s32 d_offset)
{
#if defined(_NO_LONGLONG) || defined(_LITTLE_ENDIAN)

/* 4-byte aligned start points in dst */
	mlib_u32 *dp;

/* 4-byte aligned start point in src */
	mlib_u32 *sp;

/* offset of address in dst */
	mlib_s32 j;
	mlib_u32 lmask0 = 0xFFFFFFFF;
	mlib_u32 dmask;
	mlib_u32 src, src0, src1 = 0, dst;

#ifdef _LITTLE_ENDIAN
	mlib_s32 ls_offset, ld_offset, shift;
#else /* _LITTLE_ENDIAN */
	mlib_s32 ls_offset, ld_offset, shift, k;
#endif /* _LITTLE_ENDIAN */

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
#ifdef _LITTLE_ENDIAN
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
#else /* _LITTLE_ENDIAN */
			src = (src0 << (ls_offset - ld_offset));
			dst = (dst & (~dmask)) | (src & dmask);
			STORE_MASK(dst, ((ld_offset - size) >> 3),
			    ((ld_offset - 1) >> 3), ((32 - ld_offset) & ~7))
#endif /* _LITTLE_ENDIAN */
			    return;
		}

		dmask = lmask0 << (32 - ld_offset);
#ifdef _LITTLE_ENDIAN
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
#else /* _LITTLE_ENDIAN */
		src = (src0 << (ls_offset - ld_offset));
		dst = (dst & ~dmask) | (src & dmask);
		STORE_MASK(dst, 0, ((ld_offset - 1) >> 3),
		    ((32 - ld_offset) & ~7))
#endif /* _LITTLE_ENDIAN */
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
#ifdef _LITTLE_ENDIAN
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
#else /* _LITTLE_ENDIAN */
			src = (src0 >> shift) | (src1 << (32 - shift));
			dst = (dst & ~dmask) | (src & dmask);
			STORE_MASK(dst, ((ld_offset - size) >> 3),
			    ((ld_offset - 1) >> 3), ((32 - ld_offset) & ~7))
#endif /* _LITTLE_ENDIAN */
			    return;
		}

		dmask = lmask0 << (32 - ld_offset);
#ifdef _LITTLE_ENDIAN
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
#else /* _LITTLE_ENDIAN */
		src = (src0 >> shift) | (src1 << (32 - shift));
		dst = (dst & ~dmask) | (src & dmask);
		STORE_MASK(dst, 0, ((ld_offset - 1) >> 3),
		    ((32 - ld_offset) & ~7))
#endif /* _LITTLE_ENDIAN */
		    j = ld_offset;
		dp--;
		sp--;
		ls_offset = ls_offset - j + 32;
	}

	if (j < size)
		src1 = sp[0];
#ifdef _LITTLE_ENDIAN
	src1 =
	    (src1 << 24) | ((src1 & 0xFF00) << 8) | ((src1 >> 8) & 0xFF00) |
	    (src1 >> 24);
#endif /* _LITTLE_ENDIAN */
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (; j <= size - 32; j += 32) {
		src0 = src1;
		src1 = sp[-1];
#ifdef _LITTLE_ENDIAN
		src1 =
		    (src1 << 24) | ((src1 & 0xFF00) << 8) | ((src1 >> 8) &
		    0xFF00) | (src1 >> 24);
		src = (src0 >> (32 - ls_offset)) | (src1 << ls_offset);
		dp[0] =
		    (src << 24) | ((src & 0xFF00) << 8) | ((src >> 8) & 0xFF00)
		    | (src >> 24);
#else /* _LITTLE_ENDIAN */
		dp[0] = (src0 >> (32 - ls_offset)) | (src1 << ls_offset);
#endif /* _LITTLE_ENDIAN */
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
#ifdef _LITTLE_ENDIAN
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
#else /* _LITTLE_ENDIAN */
		src = (src0 >> (32 - ls_offset)) | (src1 << ls_offset);
		dst = (dst & ~dmask) | (src & dmask);
		STORE_MASK(dst, ((32 - j) >> 3), 3, 0)
#endif /* _LITTLE_ENDIAN */
	}
#else /* _NO_LONGLONG || _LITTLE_ENDIAN */

/* 8-byte aligned start points in dst */
	mlib_u64 *dp;

/* 8-byte aligned start point in src */
	mlib_u64 *sp;

/* offset of address in dst */
	mlib_s32 j;
	mlib_u64 lmask0 = 0xFFFFFFFFFFFFFFFF;
	mlib_u64 dmask;
	mlib_u64 lsrc, lsrc0, lsrc1, ldst;
	mlib_s32 ls_offset, ld_offset, shift, k;

	if (size <= 0)
		return;

/* prepare the destination addresses */
	dp = (mlib_u64 *)((mlib_addr)da & (~7));
	sp = (mlib_u64 *)((mlib_addr)sa & (~7));
/* bit d_offset to first mlib_d64 */
	ld_offset = (((mlib_addr)da & 7) << 3) + d_offset;
/* bit d_offset to first mlib_d64 */
	ls_offset = (((mlib_addr)sa & 7) << 3) + s_offset;

	if (ld_offset < ls_offset) {
		lsrc0 = sp[0];
		ldst = dp[0];
		lsrc = (lsrc0 << (ls_offset - ld_offset));

		if (ld_offset >= size) {
			dmask = (lmask0 << (64 - size)) >> (ld_offset - size);
			ldst = (ldst & (~dmask)) | (lsrc & dmask);
			STORE_MASK(ldst, ((ld_offset - size) >> 3),
			    ((ld_offset - 1) >> 3), ((64 - ld_offset) & ~7))
			    return;
		}

		dmask = lmask0 << (64 - ld_offset);
		ldst = (ldst & ~dmask) | (lsrc & dmask);
		STORE_MASK(ldst, 0, ((ld_offset - 1) >> 3),
		    ((64 - ld_offset) & ~7))
		    j = ld_offset;
		dp--;
		ls_offset -= j;
	} else {

		shift = ld_offset - ls_offset;
		lsrc0 = sp[0];

		if (ls_offset < size)
			lsrc1 = sp[-1];
		ldst = dp[0];
		lsrc = (lsrc0 >> shift) | (lsrc1 << (64 - shift));

		if (ld_offset >= size) {
			dmask = (lmask0 << (64 - size)) >> (ld_offset - size);
			ldst = (ldst & ~dmask) | (lsrc & dmask);
			STORE_MASK(ldst, ((ld_offset - size) >> 3),
			    ((ld_offset - 1) >> 3), ((64 - ld_offset) & ~7))
			    return;
		}

		dmask = lmask0 << (64 - ld_offset);
		ldst = (ldst & ~dmask) | (lsrc & dmask);
		STORE_MASK(ldst, 0, ((ld_offset - 1) >> 3),
		    ((64 - ld_offset) & ~7))
		    j = ld_offset;
		dp--;
		sp--;
		ls_offset = ls_offset - j + 64;
	}

	if (j < size)
		lsrc1 = sp[0];
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (; j <= size - 64; j += 64) {
		lsrc0 = lsrc1;
		lsrc1 = sp[-1];
		dp[0] = (lsrc0 >> (64 - ls_offset)) | (lsrc1 << ls_offset);
		sp--;
		dp--;
	}

	if (j < size) {
		j = size - j;
		lsrc0 = lsrc1;

		if (ls_offset < j)
			lsrc1 = sp[-1];
		ldst = dp[0];
		dmask = lmask0 >> (64 - j);
		lsrc = (lsrc0 >> (64 - ls_offset)) | (lsrc1 << ls_offset);
		ldst = (ldst & ~dmask) | (lsrc & dmask);
		STORE_MASK(ldst, ((64 - j) >> 3), 7, 0)
	}
#endif /* _NO_LONGLONG || _LITTLE_ENDIAN */
}

/* *********************************************************** */
