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

#pragma ident	"@(#)mlib_ImageSubsamBinToGray4x4.c	9.3	07/10/09 SMI"

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
 *      src       The ointer to the source image.
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
 *              blockX = (int)ceil(1.0/xscale);
 *              blockY = (int)ceil(1.0/yscale);
 *
 *      If we denote a pixel's location in an image by its column number and
 *      row number (both counted from 0), the destination pixel at (i, j)
 *      is backward mapped to the source block whose upper-left corner pixel
 *      is at (xValues[i], yValues[j]), where
 *
 *              xValues[i] = (int)(i/xscale + 0.5);
 *              yValues[j] = (int)(j/yscale + 0.5);
 *
 *      The width and height of filled area in destination are restricted by
 *
 *              dstW = (int)(srcWidth * xscale)
 *              dstH = (int)(srcHeight * yscale)
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
#include <mlib_ImageSubsampleBinaryToGray.h>

/* *********************************************************** */

#ifndef MLIB_VIS

#include <mlib_c_ImageLookUp.h>

#define	LOOKUP(dl, w, lutGray)                                      \
	mlib_c_ImageLookUp_U8_U8(dl, 0, dl, 0, w, 1, 1, &lutGray)

#else /* MLIB_VIS */

#include <mlib_v_ImageLookUpFunc.h>

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
	mlib_u8 *sl, *dl, *sp0, *sp1;                                      \
	mlib_s32 bitoff, j, s0, s1, yy;                                    \
	mlib_d64 dw, dh, fTol, tmp;                                        \
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
	yy = ybegin << 2;                                                  \
	dw = s_width * xscale;                                             \
	dh = s_height * yscale;                                            \
	s_height -= yy;                                                    \
	sl += sll * yy;                                                    \
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

mlib_status
mlib_ImageSubsampleBinaryToGray_4x4(
    mlib_image *dst,
    const mlib_image *src,
    mlib_s32 ybegin,
    const mlib_u8 *lutGray)
{
	mlib_s32 buff_loc[BUFF_SIZE], *buff = buff_loc;
	mlib_d64 xscale = 0.25;
	mlib_d64 yscale = 0.25;
	mlib_s32 ww, bsize, a_flag;
	mlib_u8 *sp2, *sp3, *dp;
	mlib_u8 *dend;
	mlib_s32 s2, s3, r0, r1;

	GET_SIZE();

	ww = (s_width + 1) & ~1;
	bsize = w + 4 * ww;

	if (bsize > (mlib_s32)(BUFF_SIZE * sizeof (mlib_s32))) {
		buff = __mlib_malloc(bsize);

		if (buff == NULL)
			return (MLIB_FAILURE);
	}

	a_flag = (((mlib_s32)sl | sll) & 1) | bitoff;

	for (j = 0; j < h; j++) {

		if ((mlib_addr)dl & 3) {
			dp = (mlib_u8 *)buff;
		} else {
			dp = dl;
		}

		if (a_flag) {
			sp0 = (mlib_u8 *)(buff + w);
			sp1 = sp0 + ww;
			sp2 = sp1 + ww;
			sp3 = sp2 + ww;

			if (bitoff) {
				mlib_ImageCopy_bit_na(sl, sp0, s_width, bitoff,
				    0);
				sl += sll;
				mlib_ImageCopy_bit_na(sl, sp1, s_width, bitoff,
				    0);
				sl += sll;
				mlib_ImageCopy_bit_na(sl, sp2, s_width, bitoff,
				    0);
				sl += sll;
				mlib_ImageCopy_bit_na(sl, sp3, s_width, bitoff,
				    0);
				sl += sll;
			} else {
				mlib_ImageCopy_bit_al(sl, sp0, s_width, bitoff);
				sl += sll;
				mlib_ImageCopy_bit_al(sl, sp1, s_width, bitoff);
				sl += sll;
				mlib_ImageCopy_bit_al(sl, sp2, s_width, bitoff);
				sl += sll;
				mlib_ImageCopy_bit_al(sl, sp3, s_width, bitoff);
				sl += sll;
			}
		} else {
			sp0 = sl;
			sl += sll;
			sp1 = sl;
			sl += sll;
			sp2 = sl;
			sl += sll;
			sp3 = sl;
			sl += sll;
		}

		dend = dp + w;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (; (mlib_u8 *)dp <= (dend - 4); dp += 4) {
			s0 = ((mlib_u16 *)sp0)[0];
			s1 = ((mlib_u16 *)sp1)[0];
			s2 = ((mlib_u16 *)sp2)[0];
			s3 = ((mlib_u16 *)sp3)[0];

			r0 = mlib_nbits4[s0 >> 8] + mlib_nbits4[s1 >> 8] +
			    mlib_nbits4[s2 >> 8] + mlib_nbits4[s3 >> 8];
			r1 = mlib_nbits4[s0 & 255] + mlib_nbits4[s1 & 255] +
			    mlib_nbits4[s2 & 255] + mlib_nbits4[s3 & 255];
			*(mlib_s32 *)dp = (r0 << 16) | r1;
			sp0 += 2;
			sp1 += 2;
			sp2 += 2;
			sp3 += 2;
		}

		if ((mlib_u8 *)dp <= (dend - 2)) {
			r0 = mlib_nbits4[(*sp0++)] + mlib_nbits4[(*sp1++)] +
			    mlib_nbits4[(*sp2++)] + mlib_nbits4[(*sp3++)];
			*(mlib_u16 *)dp = r0;
			dp += 2;
		}

		if (w & 1) {
			r0 = mlib_nbits4[*sp0] + mlib_nbits4[*sp1] +
			    mlib_nbits4[*sp2] + mlib_nbits4[*sp3];
#ifdef _LITTLE_ENDIAN
			dp[0] = r0;
#else /* _LITTLE_ENDIAN */
			dp[0] = r0 >> 8;
#endif /* _LITTLE_ENDIAN */
		}

		if ((mlib_addr)dl & 3) {
			mlib_ImageCopy_na((void *)buff, (void *)dl, w);
		}

		if (lutGray != NULL) {
			LOOKUP(dl, w, lutGray);
		}

		dl += dll;
	}

	if (buff != buff_loc)
		__mlib_free(buff);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
