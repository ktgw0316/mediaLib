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

#pragma ident	"@(#)mlib_ImageSubsampleBinaryToGray.c	9.4	07/10/09 SMI"

/*
 * FUNCTION
 *      mlib_ImageSubsampleBinaryToGray    - Subsample a binary image
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageSubsampleBinaryToGray(mlib_image       *dst,
 *                                                  const mlib_image *src,
 *                                                  mlib_d64         xscale,
 *                                                  mlib_d64         yscale,
 *                                                  const mlib_u8    *lutGray)
 *
 * ARGUMENTS
 *      dst       The pointer to the destination image.
 *      src       The pointer to the source image.
 *      xscale    The x scale factor.
 *      yscale    The y scale factor.
 *      lutGray   The pointer to the grayscale lookup-table.
 *
 * DESCRIPTION
 *
 *      This function subsamples a binary (MLIB_BIT) image and produces a
 *      grayscale (MLIB_BYTE) image.
 *
 *      The subsampling algorithm performs the scaling operation by
 *      accumulating all the bits in the source image that correspond to
 *      the destination pixel and, based on the x and y scaling factors,
 *      reserving consecutive indexes in the colormap for the maximum number
 *      of gray levels possible in the destination image. The destination
 *      image pixel values of this function are either gray levels or
 *      indexes (if lutGray == NULL).
 *
 *      For representing the source block of pixels that is used to
 *      determine destination pixel values, the index 0 represents a block
 *      with no 1's (all 0's), the index 1 represents a block with a single
 *      1, and so on. If the scaling factors require a fractional block of
 *      source pixels to determine a destination pixel value, the block
 *      size is rounded up. For example, if a 2.2-by-2.2 block of source
 *      pixels would be required to determine destination pixel values, a
 *      3-by-3 block is used, resulting in 10 possible gray levels and
 *      therefore 10 colormap indexes, whose values are 0 through 9.
 *
 *      The width and height of the source block for a destination pixel are
 *      computed as:
 *
 *              blockX = (mlib_s32)ceil(1.0/xscale);
 *              blockY = (mlib_s32)ceil(1.0/yscale);
 *
 *      If we denote a pixel's location in an image by its column number and
 *      row number (both counted from 0), the destination pixel at (i, j)
 *      is backward mapped to the source block whose upper-left corner pixel
 *      is at (xValues[i], yValues[j]), where
 *
 *              xValues[i] = (mlib_s32)(i/xscale + 0.5);
 *              yValues[j] = (mlib_s32)(j/yscale + 0.5);
 *
 *      The width and height of filled area in destination are restricted by
 *
 *              dstW = (mlib_s32)(srcWidth * xscale)
 *              dstH = (mlib_s32)(srcHeight * yscale)
 *
 *      where srcWidth and srcHeight are width and height of the source image.
 *
 *      Since the block size in source is defined from scale factors with
 *      roundup, some blocks (the most right and the most bottom blocks) may
 *      overrun border of source image by 1 pixel. In this case, such blocks
 *      are moved by 1 pixel to left/up direction in order to be inside of
 *      the source image.
 *
 * RESTRICTION
 *
 *      The src must be of type MLIB_BIT; the dst must be of type MLIB_BYTE.
 *      Both the src and dst must have just one channel.
 *
 *      0.0 < xscale <= 1.0
 *      0.0 < yscale <= 1.0
 *
 *      Special cases with xscale = yscale = 0.5 or xscale = yscale = 0.25
 *      should be handled in optimized algorithms.
 */

#include <mlib_image.h>
#include <mlib_ImageCheck.h>
#include <mlib_ImageCopy.h>
#include <mlib_ImageSubsampleBinaryToGray.h>
/* #include <math.h> */

/* *********************************************************** */

#ifndef MLIB_VIS

#include <mlib_c_ImageLookUp.h>

#else /* MLIB_VIS */

#include <mlib_v_ImageLookUpFunc.h>

#endif /* MLIB_VIS */

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageSubsampleBinaryToGray = \
	__mlib_ImageSubsampleBinaryToGray

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_ImageSubsampleBinaryToGray) mlib_ImageSubsampleBinaryToGray
    __attribute__((weak, alias("__mlib_ImageSubsampleBinaryToGray")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

static mlib_status mlib_ImageSubsampleBinaryToGray_y(
    mlib_image *dst,
    const mlib_image *src,
    mlib_d64 xscale,
    mlib_d64 yscale,
    mlib_s32 kw,
    mlib_s32 ybegin,
    const mlib_u8 *lutGray);

static mlib_status mlib_ImageSubsampleBinaryToGray_xy_large(
    mlib_image *dst,
    const mlib_image *src,
    mlib_d64 xscale,
    mlib_d64 yscale,
    mlib_s32 kw,
    mlib_s32 kh,
    mlib_s32 ybegin,
    const mlib_u8 *lutGray);

static mlib_status mlib_ImageSubsampleBinaryToGray_xy(
    mlib_image *dst,
    const mlib_image *src,
    mlib_d64 xscale,
    mlib_d64 yscale,
    mlib_s32 ybegin,
    const mlib_u8 *lutGray);

/* *********************************************************** */

#ifndef MLIB_VIS

#define	LOOKUP(dl, w, lutGray)                                      \
	mlib_c_ImageLookUp_U8_U8(dl, 0, dl, 0, w, 1, 1, &lutGray)

#else /* MLIB_VIS */

#define	LOOKUP(dl, w, lutGray)                                     \
	mlib_v_ImageLookUp_U8_U8_1(dl, 0, dl, 0, w, 1, &lutGray)

#endif /* MLIB_VIS */

/* *********************************************************** */

#define	BUFF_SIZE	1024

/* *********************************************************** */

#define	GET_SIZE()                                                         \
	mlib_type s_type, d_type;                                          \
	mlib_s32 s_width, s_height, sll, s_nchan;                          \
	mlib_s32 d_width, d_height, dll, nchan;                            \
	mlib_s32 w, h;                                                     \
	mlib_u8 *sl, *dl;                                                  \
	mlib_s32 bitoff, j;                                                \
	mlib_f32 dw, dh, fTol, tmp, yy;                                    \
	                                                                   \
	MLIB_IMAGE_CHECK(dst);                                             \
	MLIB_IMAGE_CHECK(src);                                             \
	MLIB_IMAGE_GET_ALL_PARAMS(dst, d_type, nchan, d_width, d_height,   \
	    dll, dl);                                                      \
	MLIB_IMAGE_GET_ALL_PARAMS(src, s_type, s_nchan, s_width,           \
	    s_height, sll, sl);                                            \
	bitoff = mlib_ImageGetBitOffset(src);                              \
	if (s_type != MLIB_BIT || d_type != MLIB_BYTE ||                   \
	    nchan != s_nchan || nchan != 1) {                              \
	    return (MLIB_FAILURE);                                         \
	}                                                                  \
	yy = 0.5 + ybegin * invScaleY;                                     \
	y = (mlib_s32)yy;                                                  \
	sl += sll * y;                                                     \
	dw = s_width * xscale;                                             \
	dh = s_height * yscale;                                            \
	s_height -= y;                                                     \
	fTol = xscale / (dw + 1.0);                                        \
	tmp = yscale / (dh + 1.0);                                         \
	if (tmp < fTol)                                                    \
	    fTol = tmp;                                                    \
	w = (mlib_s32)(dw + fTol);                                         \
	h = (mlib_s32)(dh + fTol) - ybegin;                                \
	if (w > d_width)                                                   \
	    w = d_width;                                                   \
	if (h > d_height)                                                  \
	    h = d_height

/* *********************************************************** */

#ifdef _LITTLE_ENDIAN

const mlib_u32 mlib_edgemask4[4] = {
	0x00000000, 0x000000FF, 0x0000FFFF, 0x00FFFFFF
};

const mlib_u32 mlib_nbits1[64] = {
	0x00000000, 0x01000000, 0x00010000, 0x01010000,
	0x00000100, 0x01000100, 0x00010100, 0x01010100,
	0x00000001, 0x01000001, 0x00010001, 0x01010001,
	0x00000101, 0x01000101, 0x00010101, 0x01010101
};

const mlib_u32 mlib_nbits2[256] = {
	0x00000000, 0x01000000, 0x01000000, 0x02000000,
	0x00010000, 0x01010000, 0x01010000, 0x02010000,
	0x00010000, 0x01010000, 0x01010000, 0x02010000,
	0x00020000, 0x01020000, 0x01020000, 0x02020000,
	0x00000100, 0x01000100, 0x01000100, 0x02000100,
	0x00010100, 0x01010100, 0x01010100, 0x02010100,
	0x00010100, 0x01010100, 0x01010100, 0x02010100,
	0x00020100, 0x01020100, 0x01020100, 0x02020100,
	0x00000100, 0x01000100, 0x01000100, 0x02000100,
	0x00010100, 0x01010100, 0x01010100, 0x02010100,
	0x00010100, 0x01010100, 0x01010100, 0x02010100,
	0x00020100, 0x01020100, 0x01020100, 0x02020100,
	0x00000200, 0x01000200, 0x01000200, 0x02000200,
	0x00010200, 0x01010200, 0x01010200, 0x02010200,
	0x00010200, 0x01010200, 0x01010200, 0x02010200,
	0x00020200, 0x01020200, 0x01020200, 0x02020200,
	0x00000001, 0x01000001, 0x01000001, 0x02000001,
	0x00010001, 0x01010001, 0x01010001, 0x02010001,
	0x00010001, 0x01010001, 0x01010001, 0x02010001,
	0x00020001, 0x01020001, 0x01020001, 0x02020001,
	0x00000101, 0x01000101, 0x01000101, 0x02000101,
	0x00010101, 0x01010101, 0x01010101, 0x02010101,
	0x00010101, 0x01010101, 0x01010101, 0x02010101,
	0x00020101, 0x01020101, 0x01020101, 0x02020101,
	0x00000101, 0x01000101, 0x01000101, 0x02000101,
	0x00010101, 0x01010101, 0x01010101, 0x02010101,
	0x00010101, 0x01010101, 0x01010101, 0x02010101,
	0x00020101, 0x01020101, 0x01020101, 0x02020101,
	0x00000201, 0x01000201, 0x01000201, 0x02000201,
	0x00010201, 0x01010201, 0x01010201, 0x02010201,
	0x00010201, 0x01010201, 0x01010201, 0x02010201,
	0x00020201, 0x01020201, 0x01020201, 0x02020201,
	0x00000001, 0x01000001, 0x01000001, 0x02000001,
	0x00010001, 0x01010001, 0x01010001, 0x02010001,
	0x00010001, 0x01010001, 0x01010001, 0x02010001,
	0x00020001, 0x01020001, 0x01020001, 0x02020001,
	0x00000101, 0x01000101, 0x01000101, 0x02000101,
	0x00010101, 0x01010101, 0x01010101, 0x02010101,
	0x00010101, 0x01010101, 0x01010101, 0x02010101,
	0x00020101, 0x01020101, 0x01020101, 0x02020101,
	0x00000101, 0x01000101, 0x01000101, 0x02000101,
	0x00010101, 0x01010101, 0x01010101, 0x02010101,
	0x00010101, 0x01010101, 0x01010101, 0x02010101,
	0x00020101, 0x01020101, 0x01020101, 0x02020101,
	0x00000201, 0x01000201, 0x01000201, 0x02000201,
	0x00010201, 0x01010201, 0x01010201, 0x02010201,
	0x00010201, 0x01010201, 0x01010201, 0x02010201,
	0x00020201, 0x01020201, 0x01020201, 0x02020201,
	0x00000002, 0x01000002, 0x01000002, 0x02000002,
	0x00010002, 0x01010002, 0x01010002, 0x02010002,
	0x00010002, 0x01010002, 0x01010002, 0x02010002,
	0x00020002, 0x01020002, 0x01020002, 0x02020002,
	0x00000102, 0x01000102, 0x01000102, 0x02000102,
	0x00010102, 0x01010102, 0x01010102, 0x02010102,
	0x00010102, 0x01010102, 0x01010102, 0x02010102,
	0x00020102, 0x01020102, 0x01020102, 0x02020102,
	0x00000102, 0x01000102, 0x01000102, 0x02000102,
	0x00010102, 0x01010102, 0x01010102, 0x02010102,
	0x00010102, 0x01010102, 0x01010102, 0x02010102,
	0x00020102, 0x01020102, 0x01020102, 0x02020102,
	0x00000202, 0x01000202, 0x01000202, 0x02000202,
	0x00010202, 0x01010202, 0x01010202, 0x02010202,
	0x00010202, 0x01010202, 0x01010202, 0x02010202,
	0x00020202, 0x01020202, 0x01020202, 0x02020202
};

const mlib_u16 mlib_nbits3[64] = {
	0x0000, 0x0100, 0x0100, 0x0200,
	0x0100, 0x0200, 0x0200, 0x0300,
	0x0001, 0x0101, 0x0101, 0x0201,
	0x0101, 0x0201, 0x0201, 0x0301,
	0x0001, 0x0101, 0x0101, 0x0201,
	0x0101, 0x0201, 0x0201, 0x0301,
	0x0002, 0x0102, 0x0102, 0x0202,
	0x0102, 0x0202, 0x0202, 0x0302,
	0x0001, 0x0101, 0x0101, 0x0201,
	0x0101, 0x0201, 0x0201, 0x0301,
	0x0002, 0x0102, 0x0102, 0x0202,
	0x0102, 0x0202, 0x0202, 0x0302,
	0x0002, 0x0102, 0x0102, 0x0202,
	0x0102, 0x0202, 0x0202, 0x0302,
	0x0003, 0x0103, 0x0103, 0x0203,
	0x0103, 0x0203, 0x0203, 0x0303
};

const mlib_u16 mlib_nbits4[256] = {
	0x0000, 0x0100, 0x0100, 0x0200,
	0x0100, 0x0200, 0x0200, 0x0300,
	0x0100, 0x0200, 0x0200, 0x0300,
	0x0200, 0x0300, 0x0300, 0x0400,
	0x0001, 0x0101, 0x0101, 0x0201,
	0x0101, 0x0201, 0x0201, 0x0301,
	0x0101, 0x0201, 0x0201, 0x0301,
	0x0201, 0x0301, 0x0301, 0x0401,
	0x0001, 0x0101, 0x0101, 0x0201,
	0x0101, 0x0201, 0x0201, 0x0301,
	0x0101, 0x0201, 0x0201, 0x0301,
	0x0201, 0x0301, 0x0301, 0x0401,
	0x0002, 0x0102, 0x0102, 0x0202,
	0x0102, 0x0202, 0x0202, 0x0302,
	0x0102, 0x0202, 0x0202, 0x0302,
	0x0202, 0x0302, 0x0302, 0x0402,
	0x0001, 0x0101, 0x0101, 0x0201,
	0x0101, 0x0201, 0x0201, 0x0301,
	0x0101, 0x0201, 0x0201, 0x0301,
	0x0201, 0x0301, 0x0301, 0x0401,
	0x0002, 0x0102, 0x0102, 0x0202,
	0x0102, 0x0202, 0x0202, 0x0302,
	0x0102, 0x0202, 0x0202, 0x0302,
	0x0202, 0x0302, 0x0302, 0x0402,
	0x0002, 0x0102, 0x0102, 0x0202,
	0x0102, 0x0202, 0x0202, 0x0302,
	0x0102, 0x0202, 0x0202, 0x0302,
	0x0202, 0x0302, 0x0302, 0x0402,
	0x0003, 0x0103, 0x0103, 0x0203,
	0x0103, 0x0203, 0x0203, 0x0303,
	0x0103, 0x0203, 0x0203, 0x0303,
	0x0203, 0x0303, 0x0303, 0x0403,
	0x0001, 0x0101, 0x0101, 0x0201,
	0x0101, 0x0201, 0x0201, 0x0301,
	0x0101, 0x0201, 0x0201, 0x0301,
	0x0201, 0x0301, 0x0301, 0x0401,
	0x0002, 0x0102, 0x0102, 0x0202,
	0x0102, 0x0202, 0x0202, 0x0302,
	0x0102, 0x0202, 0x0202, 0x0302,
	0x0202, 0x0302, 0x0302, 0x0402,
	0x0002, 0x0102, 0x0102, 0x0202,
	0x0102, 0x0202, 0x0202, 0x0302,
	0x0102, 0x0202, 0x0202, 0x0302,
	0x0202, 0x0302, 0x0302, 0x0402,
	0x0003, 0x0103, 0x0103, 0x0203,
	0x0103, 0x0203, 0x0203, 0x0303,
	0x0103, 0x0203, 0x0203, 0x0303,
	0x0203, 0x0303, 0x0303, 0x0403,
	0x0002, 0x0102, 0x0102, 0x0202,
	0x0102, 0x0202, 0x0202, 0x0302,
	0x0102, 0x0202, 0x0202, 0x0302,
	0x0202, 0x0302, 0x0302, 0x0402,
	0x0003, 0x0103, 0x0103, 0x0203,
	0x0103, 0x0203, 0x0203, 0x0303,
	0x0103, 0x0203, 0x0203, 0x0303,
	0x0203, 0x0303, 0x0303, 0x0403,
	0x0003, 0x0103, 0x0103, 0x0203,
	0x0103, 0x0203, 0x0203, 0x0303,
	0x0103, 0x0203, 0x0203, 0x0303,
	0x0203, 0x0303, 0x0303, 0x0403,
	0x0004, 0x0104, 0x0104, 0x0204,
	0x0104, 0x0204, 0x0204, 0x0304,
	0x0104, 0x0204, 0x0204, 0x0304,
	0x0204, 0x0304, 0x0304, 0x0404
};

#else /* _LITTLE_ENDIAN */

const mlib_u32 mlib_edgemask4[4] = {
	0x00000000, 0xFF000000, 0xFFFF0000, 0xFFFFFF00
};

const mlib_u32 mlib_nbits1[64] = {
	0x00000000, 0x00000001, 0x00000100, 0x00000101,
	0x00010000, 0x00010001, 0x00010100, 0x00010101,
	0x01000000, 0x01000001, 0x01000100, 0x01000101,
	0x01010000, 0x01010001, 0x01010100, 0x01010101
};

const mlib_u32 mlib_nbits2[256] = {
	0x00000000, 0x00000001, 0x00000001, 0x00000002,
	0x00000100, 0x00000101, 0x00000101, 0x00000102,
	0x00000100, 0x00000101, 0x00000101, 0x00000102,
	0x00000200, 0x00000201, 0x00000201, 0x00000202,
	0x00010000, 0x00010001, 0x00010001, 0x00010002,
	0x00010100, 0x00010101, 0x00010101, 0x00010102,
	0x00010100, 0x00010101, 0x00010101, 0x00010102,
	0x00010200, 0x00010201, 0x00010201, 0x00010202,
	0x00010000, 0x00010001, 0x00010001, 0x00010002,
	0x00010100, 0x00010101, 0x00010101, 0x00010102,
	0x00010100, 0x00010101, 0x00010101, 0x00010102,
	0x00010200, 0x00010201, 0x00010201, 0x00010202,
	0x00020000, 0x00020001, 0x00020001, 0x00020002,
	0x00020100, 0x00020101, 0x00020101, 0x00020102,
	0x00020100, 0x00020101, 0x00020101, 0x00020102,
	0x00020200, 0x00020201, 0x00020201, 0x00020202,
	0x01000000, 0x01000001, 0x01000001, 0x01000002,
	0x01000100, 0x01000101, 0x01000101, 0x01000102,
	0x01000100, 0x01000101, 0x01000101, 0x01000102,
	0x01000200, 0x01000201, 0x01000201, 0x01000202,
	0x01010000, 0x01010001, 0x01010001, 0x01010002,
	0x01010100, 0x01010101, 0x01010101, 0x01010102,
	0x01010100, 0x01010101, 0x01010101, 0x01010102,
	0x01010200, 0x01010201, 0x01010201, 0x01010202,
	0x01010000, 0x01010001, 0x01010001, 0x01010002,
	0x01010100, 0x01010101, 0x01010101, 0x01010102,
	0x01010100, 0x01010101, 0x01010101, 0x01010102,
	0x01010200, 0x01010201, 0x01010201, 0x01010202,
	0x01020000, 0x01020001, 0x01020001, 0x01020002,
	0x01020100, 0x01020101, 0x01020101, 0x01020102,
	0x01020100, 0x01020101, 0x01020101, 0x01020102,
	0x01020200, 0x01020201, 0x01020201, 0x01020202,
	0x01000000, 0x01000001, 0x01000001, 0x01000002,
	0x01000100, 0x01000101, 0x01000101, 0x01000102,
	0x01000100, 0x01000101, 0x01000101, 0x01000102,
	0x01000200, 0x01000201, 0x01000201, 0x01000202,
	0x01010000, 0x01010001, 0x01010001, 0x01010002,
	0x01010100, 0x01010101, 0x01010101, 0x01010102,
	0x01010100, 0x01010101, 0x01010101, 0x01010102,
	0x01010200, 0x01010201, 0x01010201, 0x01010202,
	0x01010000, 0x01010001, 0x01010001, 0x01010002,
	0x01010100, 0x01010101, 0x01010101, 0x01010102,
	0x01010100, 0x01010101, 0x01010101, 0x01010102,
	0x01010200, 0x01010201, 0x01010201, 0x01010202,
	0x01020000, 0x01020001, 0x01020001, 0x01020002,
	0x01020100, 0x01020101, 0x01020101, 0x01020102,
	0x01020100, 0x01020101, 0x01020101, 0x01020102,
	0x01020200, 0x01020201, 0x01020201, 0x01020202,
	0x02000000, 0x02000001, 0x02000001, 0x02000002,
	0x02000100, 0x02000101, 0x02000101, 0x02000102,
	0x02000100, 0x02000101, 0x02000101, 0x02000102,
	0x02000200, 0x02000201, 0x02000201, 0x02000202,
	0x02010000, 0x02010001, 0x02010001, 0x02010002,
	0x02010100, 0x02010101, 0x02010101, 0x02010102,
	0x02010100, 0x02010101, 0x02010101, 0x02010102,
	0x02010200, 0x02010201, 0x02010201, 0x02010202,
	0x02010000, 0x02010001, 0x02010001, 0x02010002,
	0x02010100, 0x02010101, 0x02010101, 0x02010102,
	0x02010100, 0x02010101, 0x02010101, 0x02010102,
	0x02010200, 0x02010201, 0x02010201, 0x02010202,
	0x02020000, 0x02020001, 0x02020001, 0x02020002,
	0x02020100, 0x02020101, 0x02020101, 0x02020102,
	0x02020100, 0x02020101, 0x02020101, 0x02020102,
	0x02020200, 0x02020201, 0x02020201, 0x02020202,
};

const mlib_u16 mlib_nbits3[64] = {
	0x0000, 0x0001, 0x0001, 0x0002,
	0x0001, 0x0002, 0x0002, 0x0003,
	0x0100, 0x0101, 0x0101, 0x0102,
	0x0101, 0x0102, 0x0102, 0x0103,
	0x0100, 0x0101, 0x0101, 0x0102,
	0x0101, 0x0102, 0x0102, 0x0103,
	0x0200, 0x0201, 0x0201, 0x0202,
	0x0201, 0x0202, 0x0202, 0x0203,
	0x0100, 0x0101, 0x0101, 0x0102,
	0x0101, 0x0102, 0x0102, 0x0103,
	0x0200, 0x0201, 0x0201, 0x0202,
	0x0201, 0x0202, 0x0202, 0x0203,
	0x0200, 0x0201, 0x0201, 0x0202,
	0x0201, 0x0202, 0x0202, 0x0203,
	0x0300, 0x0301, 0x0301, 0x0302,
	0x0301, 0x0302, 0x0302, 0x0303,
};

const mlib_u16 mlib_nbits4[256] = {
	0x0000, 0x0001, 0x0001, 0x0002,
	0x0001, 0x0002, 0x0002, 0x0003,
	0x0001, 0x0002, 0x0002, 0x0003,
	0x0002, 0x0003, 0x0003, 0x0004,
	0x0100, 0x0101, 0x0101, 0x0102,
	0x0101, 0x0102, 0x0102, 0x0103,
	0x0101, 0x0102, 0x0102, 0x0103,
	0x0102, 0x0103, 0x0103, 0x0104,
	0x0100, 0x0101, 0x0101, 0x0102,
	0x0101, 0x0102, 0x0102, 0x0103,
	0x0101, 0x0102, 0x0102, 0x0103,
	0x0102, 0x0103, 0x0103, 0x0104,
	0x0200, 0x0201, 0x0201, 0x0202,
	0x0201, 0x0202, 0x0202, 0x0203,
	0x0201, 0x0202, 0x0202, 0x0203,
	0x0202, 0x0203, 0x0203, 0x0204,
	0x0100, 0x0101, 0x0101, 0x0102,
	0x0101, 0x0102, 0x0102, 0x0103,
	0x0101, 0x0102, 0x0102, 0x0103,
	0x0102, 0x0103, 0x0103, 0x0104,
	0x0200, 0x0201, 0x0201, 0x0202,
	0x0201, 0x0202, 0x0202, 0x0203,
	0x0201, 0x0202, 0x0202, 0x0203,
	0x0202, 0x0203, 0x0203, 0x0204,
	0x0200, 0x0201, 0x0201, 0x0202,
	0x0201, 0x0202, 0x0202, 0x0203,
	0x0201, 0x0202, 0x0202, 0x0203,
	0x0202, 0x0203, 0x0203, 0x0204,
	0x0300, 0x0301, 0x0301, 0x0302,
	0x0301, 0x0302, 0x0302, 0x0303,
	0x0301, 0x0302, 0x0302, 0x0303,
	0x0302, 0x0303, 0x0303, 0x0304,
	0x0100, 0x0101, 0x0101, 0x0102,
	0x0101, 0x0102, 0x0102, 0x0103,
	0x0101, 0x0102, 0x0102, 0x0103,
	0x0102, 0x0103, 0x0103, 0x0104,
	0x0200, 0x0201, 0x0201, 0x0202,
	0x0201, 0x0202, 0x0202, 0x0203,
	0x0201, 0x0202, 0x0202, 0x0203,
	0x0202, 0x0203, 0x0203, 0x0204,
	0x0200, 0x0201, 0x0201, 0x0202,
	0x0201, 0x0202, 0x0202, 0x0203,
	0x0201, 0x0202, 0x0202, 0x0203,
	0x0202, 0x0203, 0x0203, 0x0204,
	0x0300, 0x0301, 0x0301, 0x0302,
	0x0301, 0x0302, 0x0302, 0x0303,
	0x0301, 0x0302, 0x0302, 0x0303,
	0x0302, 0x0303, 0x0303, 0x0304,
	0x0200, 0x0201, 0x0201, 0x0202,
	0x0201, 0x0202, 0x0202, 0x0203,
	0x0201, 0x0202, 0x0202, 0x0203,
	0x0202, 0x0203, 0x0203, 0x0204,
	0x0300, 0x0301, 0x0301, 0x0302,
	0x0301, 0x0302, 0x0302, 0x0303,
	0x0301, 0x0302, 0x0302, 0x0303,
	0x0302, 0x0303, 0x0303, 0x0304,
	0x0300, 0x0301, 0x0301, 0x0302,
	0x0301, 0x0302, 0x0302, 0x0303,
	0x0301, 0x0302, 0x0302, 0x0303,
	0x0302, 0x0303, 0x0303, 0x0304,
	0x0400, 0x0401, 0x0401, 0x0402,
	0x0401, 0x0402, 0x0402, 0x0403,
	0x0401, 0x0402, 0x0402, 0x0403,
	0x0402, 0x0403, 0x0403, 0x0404,
};

#endif /* _LITTLE_ENDIAN */

/* *********************************************************** */

static void
mlib_addLine_kw1(
    mlib_u8 *dl,
    const mlib_u8 *sl,
    const mlib_u8 *src_end,
    mlib_s32 w)
{
	const mlib_u8 *sp = sl;
	mlib_u32 *dp = (mlib_u32 *)dl;
	mlib_u8 *dend = dl + w;
	mlib_s32 s0, tail_mask;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (; (mlib_u8 *)dp <= (dend - 8); dp += 2) {
		s0 = (*sp++);
		dp[0] += mlib_nbits1[s0 >> 4];
		dp[1] += mlib_nbits1[s0 & 0xF];
	}

	if (src_end == NULL || sp < src_end) s0 = *sp; else s0 = 0;

	if ((mlib_u8 *)dp <= (dend - 4)) {
		(*dp++) += mlib_nbits1[s0 >> 4];
		s0 &= 0xF;
	} else {
		s0 >>= 4;
	}

#ifdef _LITTLE_ENDIAN
	dl = (mlib_u8 *)dp;

	if ((mlib_u8 *)dl < dend) {
		mlib_s32 i;
		mlib_u32 last;

		tail_mask = mlib_edgemask4[dend - (mlib_u8 *)dp];
		last = dp[0] + (mlib_nbits1[s0] & tail_mask);
		for (i = 0; i < (dend - dl); i++, last >>= 8)
			dl[i] = last;
	}

#else /* _LITTLE_ENDIAN */
	dl = (mlib_u8 *)dp;

	if ((mlib_u8 *)dl < dend) {
		mlib_s32 i;
		mlib_u32 last;

		tail_mask = mlib_edgemask4[dend - (mlib_u8 *)dp];
		last = dp[0] + (mlib_nbits1[s0] & tail_mask);
		last >>= 8 * (4 - (dend - dl));
		for (i = 0; i < (dend - dl); i++, last >>= 8)
			dl[(dend - dl) - 1 - i] = last;
	}
#endif /* _LITTLE_ENDIAN */
}

/* *********************************************************** */

static void
mlib_addLine_kw2(
    mlib_u8 *dl,
    const mlib_u8 *sl,
    const mlib_u8 *src_end,
    mlib_s32 w)
{
	mlib_u32 *dp = (mlib_u32 *)dl;
	mlib_s32 i, s0, tail;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < w / 4; i++) {
		s0 = sl[i];
		dp[i] += mlib_nbits2[s0];
	}

	if (src_end == NULL || (sl + i) < src_end) s0 = sl[i]; else s0 = 0;
	tail = w & 3;

#ifdef _LITTLE_ENDIAN
	dl = (mlib_u8 *)(dp + i);
	if (tail) {
		mlib_u32 last;

		last = dp[i] + (mlib_nbits2[s0] & mlib_edgemask4[tail]);
		for (i = 0; i < tail; i++, last >>= 8)
			dl[i] = last;
	}
#else /* _LITTLE_ENDIAN */
	dl = (mlib_u8 *)(dp + i);
	if (tail) {
		mlib_u32 last;

		last = dp[i] + (mlib_nbits2[s0] & mlib_edgemask4[tail]);
		last >>= 8 * (4 - tail);
		for (i = 0; i < tail; i++, last >>= 8)
			dl[tail - 1 - i] = last;
	}
#endif /* _LITTLE_ENDIAN */
}

/* *********************************************************** */

static void
mlib_addLine_kw3(
    mlib_u8 *dp,
    const mlib_u8 *sp,
    const mlib_u8 *src_end,
    mlib_s32 w)
{
	mlib_u8 *dend = dp + w;
	mlib_s32 s0, s1, s2;
	mlib_s32 r0, r1, r2, r3;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (; (mlib_u8 *)dp <= (dend - 8); dp += 8) {
		s0 = (*sp++);
		s1 = (*sp++);
		s2 = (*sp++);
		r0 = mlib_nbits3[s0 >> 2];
		r1 = mlib_nbits3[((s0 & 3) << 4) | (s1 >> 4)];
		r2 = mlib_nbits3[((s1 & 15) << 2) | (s2 >> 6)];
		r3 = mlib_nbits3[s2 & 0x3F];
#ifdef _LITTLE_ENDIAN
		((mlib_s32 *)dp)[0] += (r1 << 16) | r0;
		((mlib_s32 *)dp)[1] += (r3 << 16) | r2;
#else /* _LITTLE_ENDIAN */
		((mlib_s32 *)dp)[0] += (r0 << 16) | r1;
		((mlib_s32 *)dp)[1] += (r2 << 16) | r3;
#endif /* _LITTLE_ENDIAN */
	}

	if (src_end == NULL || sp < src_end) s0 = (*sp++); else s0 = 0;
	if (src_end == NULL || sp < src_end) s1 = (*sp++); else s1 = 0;
	if (src_end == NULL || sp < src_end) s2 = (*sp++); else s2 = 0;
	r0 = mlib_nbits3[s0 >> 2];
	r1 = mlib_nbits3[((s0 & 3) << 4) | (s1 >> 4)];
#ifdef _LITTLE_ENDIAN
	r0 = (r1 << 16) | r0;
#else /* _LITTLE_ENDIAN */
	r0 = (r0 << 16) | r1;
#endif /* _LITTLE_ENDIAN */

	if ((mlib_u8 *)dp <= (dend - 4)) {
		((mlib_s32 *)dp)[0] += r0;
		dp += 4;
		r2 = mlib_nbits3[((s1 & 15) << 2) | (s2 >> 6)];
		r3 = mlib_nbits3[s2 & 0x3F];
#ifdef _LITTLE_ENDIAN
		r0 = (r3 << 16) | r2;
#else /* _LITTLE_ENDIAN */
		r0 = (r2 << 16) | r3;
#endif /* _LITTLE_ENDIAN */
	}
#ifdef _LITTLE_ENDIAN
		if ((mlib_u8 *)dp < dend) {
		mlib_s32 last, i;

		last =
		    ((mlib_s32 *)dp)[0] + (r0 & mlib_edgemask4[dend -
		    (mlib_u8 *)dp]);
		for (i = 0; i < (dend - dp); i++, last >>= 8)
			dp[i] = (mlib_u8)last;
	}
#else /* _LITTLE_ENDIAN */
	if ((mlib_u8 *)dp < dend) {
		mlib_s32 last, i;

		last =
		    ((mlib_s32 *)dp)[0] + (r0 & mlib_edgemask4[dend -
		    (mlib_u8 *)dp]);
		last >>= 8 * (4 - (dend - dp));
		for (i = 0; i < (dend - dp); i++, last >>= 8)
			dp[(dend - dp) - 1 - i] = (mlib_u32)last;
	}
#endif /* _LITTLE_ENDIAN */
}

/* *********************************************************** */

static void
mlib_addLine_kw4(
    mlib_u8 *dp,
    const mlib_u8 *sp,
    const mlib_u8 *src_end,
    mlib_s32 w)
{
	mlib_u8 *dend = dp + w;
	mlib_s32 s0, s1, rr;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (; (mlib_u8 *)dp <= (dend - 4); dp += 4) {
/* s0 = ((mlib_u16*)sp)[0]; */
		s0 = sp[0];
		s1 = sp[1];
#ifdef _LITTLE_ENDIAN
		rr = (mlib_nbits4[s1] << 16) + mlib_nbits4[s0];
#else /* _LITTLE_ENDIAN */
		rr = (mlib_nbits4[s0] << 16) + mlib_nbits4[s1];
#endif /* _LITTLE_ENDIAN */
		*(mlib_s32 *)dp += rr;
		sp += 2;
	}

	if ((mlib_u8 *)dp <= (dend - 2)) {
		s0 = (*sp++);
		*(mlib_u16 *)dp += mlib_nbits4[s0];
		dp += 2;
	}

	if (w & 1) {
		s0 = sp[0];
#ifdef _LITTLE_ENDIAN
		dp[0] += mlib_nbits4[s0];
#else /* _LITTLE_ENDIAN */
		dp[0] += mlib_nbits4[s0] >> 8;
#endif /* _LITTLE_ENDIAN */
	}
}

/* *********************************************************** */

typedef void (*line_fun_type) (
	mlib_u8 *dl,
	const mlib_u8 *sl,
	const mlib_u8 *src_end,
	mlib_s32 w);

static const line_fun_type mlib_addLine_arr[] = {
	mlib_addLine_kw1,
	mlib_addLine_kw2,
	mlib_addLine_kw3,
	mlib_addLine_kw4
};

/* *********************************************************** */

mlib_status
mlib_ImageSubsampleBinaryToGray_y(
    mlib_image *dst,
    const mlib_image *src,
    mlib_d64 xscale,
    mlib_d64 yscale,
    mlib_s32 kw,
    mlib_s32 ybegin,
    const mlib_u8 *lutGray)
{
	mlib_s32 buff_loc[BUFF_SIZE], *buff = buff_loc;
	mlib_f32 invScaleY = 1.0F / yscale;	/* to be consistent with JAI */
	mlib_s32 kh, l, mask, bsize, y, y_start, y_next, y_off;
	mlib_u8 *sp, *dp, *src_end;
	line_fun_type line_fun;

	GET_SIZE();
	y_start = y;

	src_end = (mlib_u8*)mlib_ImageGetData(src) +
			(mlib_ImageGetHeight(src) - 1)*sll +
			((mlib_ImageGetWidth(src) + 7 + bitoff)/8);

	kh = (mlib_s32)invScaleY;

	if (kh < invScaleY)
		kh++;
	if (kh > s_height)
		kh = s_height;

	line_fun = mlib_addLine_arr[kw - 1];
	mask = (kw == 2) ? 7 : 3;

	bsize = w + s_width + 8;

	if (bsize > (mlib_s32)(BUFF_SIZE * sizeof (mlib_s32))) {
		buff = __mlib_malloc(bsize);

		if (buff == NULL)
			return (MLIB_FAILURE);
	}

	for (j = 0; j < h; j++) {
		mlib_u8 *p0, *p1;

		if ((mlib_s32)dl & mask) {
			dp = (mlib_u8 *)buff;
		} else {
			dp = dl;
		}

/* clear dp */
		p0 = dp;
		p1 = dp + w;

		while (((mlib_s32)p0 & 3) && p0 < p1) {
			(*p0++) = 0;
		}

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (; p0 <= (p1 - 4); p0 += 4) {
			*(mlib_s32 *)p0 = 0;
		}

		while (p0 < p1) {
			(*p0++) = 0;
		}

		for (l = 0; l < kh; l++) {
			if (bitoff) {
				sp = (mlib_u8 *)(buff + w);
				mlib_ImageCopy_bit_na(sl, sp, s_width, bitoff,
				    0);
				line_fun(dp, sp, NULL, w);
			} else {
				line_fun(dp, sl, src_end, w);
			}

			sl += sll;
		}

		if (dp != dl) {
			mlib_ImageCopy_na((void *)dp, (void *)dl, w);
		}

		if (lutGray != NULL) {
			LOOKUP(dl, w, lutGray);
		}

		y_next = (mlib_s32)(invScaleY * (j + ybegin + 1) + 0.5);

		if (y_next + kh > s_height + y_start)
			y_next--;
		y_off = y_next - (y + kh);

		if (y_off)
			sl += y_off * sll;
		y = y_next;

		dl += dll;
	}

	if (buff != buff_loc)
		__mlib_free(buff);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
/* for the case of (blockX * blockY >= 256) */

mlib_status
mlib_ImageSubsampleBinaryToGray_xy_large(
    mlib_image *dst,
    const mlib_image *src,
    mlib_d64 xscale,
    mlib_d64 yscale,
    mlib_s32 kw,
    mlib_s32 kh,
    mlib_s32 ybegin,
    const mlib_u8 *lutGray)
{
	mlib_s32 buff_loc[BUFF_SIZE], *buff = buff_loc;
	mlib_s32 *yValues, *xValues, *oValues;
	mlib_f32 invScaleX = 1.0F / xscale;	/* use mlib_f32	*/
	mlib_f32 invScaleY = 1.0F / yscale;	/* to be consistent with JAI */
	mlib_s32 bsize, kw1, kw_mask;
	mlib_s32 i, l, x, o, y, y_off, bb, s0;
	mlib_u8 *p_lup;
	mlib_s32 *sAreaBitsOn = NULL;

	GET_SIZE();

	kw1 = (kw <= 8) ? kw : 8;
	bsize = h + 2 * w + (1 << kw1);

	if (bsize > BUFF_SIZE) {
		buff = (mlib_s32 *)__mlib_malloc(bsize * sizeof (mlib_s32));
		if (buff == NULL) {
			return (MLIB_FAILURE);
		}
	}

	if (w > 0) {
		sAreaBitsOn = (mlib_s32 *)__mlib_malloc(w * sizeof (mlib_s32));
		if (sAreaBitsOn == NULL) {
			if (buff != buff_loc) {
				__mlib_free(buff);
			}
			return (MLIB_FAILURE);
		}
	}

	yValues = buff;
	xValues = yValues + h;
	oValues = xValues + w;
	p_lup = (mlib_u8 *)(oValues + w);

	for (i = 0; i < h; i++) {
		yValues[i] = (mlib_s32)(invScaleY * (i + ybegin) + 0.5);
	}

	if (h > 0) {
		if (yValues[h - 1] + kh > s_height + y)
			yValues[h - 1]--;
	}

	for (i = 0; i < w; i++) {
		xValues[i] = (mlib_s32)(invScaleX * i + 0.5);
	}

	if (w > 0) {
		if (xValues[w - 1] + kw > s_width)
			xValues[w - 1]--;
	}

	for (i = 0; i < w; i++) {
		mlib_s32 x = xValues[i] + bitoff;

		xValues[i] = x / 8;
		oValues[i] = x & 7;
	}

	p_lup[0] = 0;
	for (l = 0; l < kw1; l++) {
		mlib_s32 ll = (1 << l);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < ll; i++) {
			p_lup[ll + i] = p_lup[i] + 1;
		}
	}

	for (j = 0; j < h; j++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < w; i++) {
			sAreaBitsOn[i] = 0;
		}

		for (l = 0; l < kh; l++) {
			for (bb = 0; bb < kw; bb += 8) {
				mlib_u8 *sp = sl + (bb / 8);

				kw1 = kw - bb;

				if (kw1 > 8)
					kw1 = 8;
				kw_mask = (1 << kw1) - 1;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i < w - 1; i++) {
					x = xValues[i];
					o = oValues[i];
					s0 = (sp[x] << o) | (sp[x + 1] >> (8 -
					    o));
					sAreaBitsOn[i] +=
					    p_lup[(s0 >> (8 - kw1)) & kw_mask];
				}
				if (w) {
					x = xValues[i];
					o = oValues[i];
					s0 = (sp[x] << o);
					if (8 - o < kw1)
						s0 |= (sp[x + 1] >> (8 - o));
					sAreaBitsOn[i] +=
					    p_lup[(s0 >> (8 - kw1)) & kw_mask];
				}
			}

			sl += sll;
		}

		if (lutGray != NULL) {
			for (i = 0; i < w; i++) {
				dl[i] = lutGray[sAreaBitsOn[i]];
			}
		} else {
			for (i = 0; i < w; i++) {
				if (sAreaBitsOn[i] > 255) {
					dl[i] = 255;
				} else {
					dl[i] = (mlib_u8)(sAreaBitsOn[i]);
				}
			}
		}

		dl += dll;

		if (j + 1 == h)
			break;
		y_off = yValues[j + 1] - (yValues[j] + kh);

		if (y_off) {
			sl += y_off * sll;
		}
	}

	if (buff != buff_loc) {
		__mlib_free(buff);
	}
	if (sAreaBitsOn != NULL) {
		__mlib_free(sAreaBitsOn);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_ImageSubsampleBinaryToGray_xy(
    mlib_image *dst,
    const mlib_image *src,
    mlib_d64 xscale,
    mlib_d64 yscale,
    mlib_s32 ybegin,
    const mlib_u8 *lutGray)
{
	mlib_s32 buff_loc[BUFF_SIZE], *buff = buff_loc;
	mlib_s32 *yValues, *xValues, *oValues;
	mlib_f32 invScaleX = 1.0F / xscale;	/* use mlib_f32 */
	mlib_f32 invScaleY = 1.0F / yscale;	/* to be consistent with JAI */
	mlib_s32 kw, kh, bsize, kw1, kw_mask;
	mlib_s32 i, l, x, o, y, y_off, bb, s0;
	mlib_u8 *p_lup;

	GET_SIZE();

#if 0
	kw = (mlib_s32)ceil(invScaleX);
	kh = (mlib_s32)ceil(invScaleY);
#else
	kw = (mlib_s32)invScaleX;
	kh = (mlib_s32)invScaleY;

	if (kw < invScaleX)
		kw++;
	if (kh < invScaleY)
		kh++;
#endif
	if (kw > s_width)
		kw = s_width;
	if (kh > s_height)
		kh = s_height;

	if (kw * kh >= 256) {
		return mlib_ImageSubsampleBinaryToGray_xy_large(dst, src,
			    xscale, yscale, kw, kh, ybegin, lutGray);
	}

/* step of x position always equal to kw */
	if ((mlib_s32)(invScaleX * w + 0.5) == kw * w) {
		if (kw <= 4) {
			return mlib_ImageSubsampleBinaryToGray_y(dst, src,
			    xscale, yscale, kw, ybegin, lutGray);
		}
	}

	kw1 = (kw <= 8) ? kw : 8;
	bsize = h + 2 * w + (1 << kw1);

	if (bsize > BUFF_SIZE) {
		buff = __mlib_malloc(bsize * sizeof (mlib_s32));

		if (buff == NULL)
			return (MLIB_FAILURE);
	}

	yValues = buff;
	xValues = yValues + h;
	oValues = xValues + w;
	p_lup = (mlib_u8 *)(oValues + w);

	for (i = 0; i < h; i++) {
		yValues[i] = (mlib_s32)(invScaleY * (i + ybegin) + 0.5);
	}

	if (h > 0) {
		if (yValues[h - 1] + kh > s_height + y)
			yValues[h - 1]--;
	}

	for (i = 0; i < w; i++) {
		xValues[i] = (mlib_s32)(invScaleX * i + 0.5);
	}

	if (w > 0) {
		if (xValues[w - 1] + kw > s_width)
			xValues[w - 1]--;
	}

	for (i = 0; i < w; i++) {
		mlib_s32 x = xValues[i] + bitoff;

		xValues[i] = x / 8;
		oValues[i] = x & 7;
	}

	p_lup[0] = 0;
	for (l = 0; l < kw1; l++) {
		mlib_s32 ll = (1 << l);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < ll; i++) {
			p_lup[ll + i] = p_lup[i] + 1;
		}
	}

	for (j = 0; j < h; j++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < w; i++) {
			dl[i] = 0;
		}

		for (l = 0; l < kh; l++) {
			for (bb = 0; bb < kw; bb += 8) {
				mlib_u8 *sp = sl + (bb / 8);

				kw1 = kw - bb;

				if (kw1 > 8)
					kw1 = 8;
				kw_mask = (1 << kw1) - 1;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i < w - 1; i++) {
					x = xValues[i];
					o = oValues[i];
					s0 = (sp[x] << o) | (sp[x + 1] >> (8 -
					    o));
					dl[i] +=
					    p_lup[(s0 >> (8 - kw1)) & kw_mask];
				}
				if (w) {
					x = xValues[i];
					o = oValues[i];
					s0 = (sp[x] << o);
					if (8 - o < kw1)
						s0 |= (sp[x + 1] >> (8 - o));
					dl[i] +=
					    p_lup[(s0 >> (8 - kw1)) & kw_mask];
				}
			}

			sl += sll;
		}

		if (lutGray != NULL) {
			LOOKUP(dl, w, lutGray);
		}

		dl += dll;

		if (j + 1 == h)
			break;
		y_off = yValues[j + 1] - (yValues[j] + kh);

		if (y_off) {
			sl += y_off * sll;
		}
	}

	if (buff != buff_loc)
		__mlib_free(buff);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_ImageSubsampleBinaryToGray(
    mlib_image *dst,
    const mlib_image *src,
    mlib_d64 xscale,
    mlib_d64 yscale,
    const mlib_u8 *lutGray)
{
	if (!(0.0 < xscale && xscale <= 1.0 && 0.0 < yscale && yscale <= 1.0))
		return (MLIB_FAILURE);

	if (xscale == 0.5 && yscale == 0.5) {
		return mlib_ImageSubsampleBinaryToGray_2x2(dst, src, 0,
		    lutGray);
	}

	if (xscale == 0.25 && yscale == 0.25) {
		return mlib_ImageSubsampleBinaryToGray_4x4(dst, src, 0,
		    lutGray);
	}

	return mlib_ImageSubsampleBinaryToGray_xy(dst, src, xscale, yscale, 0,
	    lutGray);
}

/* *********************************************************** */

mlib_status
__mlib_ImageSubsampleBinaryToGray_ty(
    mlib_image *dst,
    const mlib_image *src,
    mlib_d64 xscale,
    mlib_d64 yscale,
    mlib_s32 ybegin,
    const mlib_u8 *lutGray)
{
	if (!(0.0 < xscale && xscale <= 1.0 && 0.0 < yscale && yscale <= 1.0))
		return (MLIB_FAILURE);

	if (xscale == 0.5 && yscale == 0.5) {
		return mlib_ImageSubsampleBinaryToGray_2x2(dst, src, ybegin,
		    lutGray);
	}

	if (xscale == 0.25 && yscale == 0.25) {
		return mlib_ImageSubsampleBinaryToGray_4x4(dst, src, ybegin,
		    lutGray);
	}

	return mlib_ImageSubsampleBinaryToGray_xy(dst, src, xscale, yscale,
	    ybegin, lutGray);
}

/* *********************************************************** */
