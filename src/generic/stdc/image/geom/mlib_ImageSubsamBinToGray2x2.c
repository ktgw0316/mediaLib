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

#pragma ident	"@(#)mlib_ImageSubsamBinToGray2x2.c	9.3	07/10/09 SMI"

/*
 * FUNCTION
 *      mlib_ImageSubsampleBinaryToGray    - Subsample a binary image
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageSubsampleBinaryToGray(mlib_image *dst,
 *                                                  mlib_image *src,
 *                                                  mlib_d64   xscale,
 *                                                  mlib_d64   yscale,
 *                                                  mlib_u8    *lutGray)
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
#include <mlib_ImageSubsampleBinaryToGray.h>

/* *********************************************************** */

#define	GET_SIZE()                                                         \
	mlib_type s_type, d_type;                                          \
	mlib_s32 s_width, s_height, sll, s_nchan;                          \
	mlib_s32 d_width, d_height, dll, nchan;                            \
	mlib_s32 w, h, w4;                                                 \
	mlib_u8 *sl, *dl, *sp0, *sp1;                                      \
	mlib_s32 bitoff, i, j, s0, s1, sa, sb, rr, tail_mask, lut_flag =   \
	    0, yy;                                                         \
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
	yy = ybegin << 1;                                                  \
	dw = s_width * xscale;                                             \
	dh = s_height * yscale;                                            \
	s_height -= (mlib_s32)yy;                                          \
	sl += sll * (mlib_s32)yy;                                          \
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
mlib_ImageSubsampleBinaryToGray_2x2(
    mlib_image *dst,
    const mlib_image *src,
    mlib_s32 ybegin,
    const mlib_u8 *lutGray)
{
	mlib_d64 xscale = 0.5;
	mlib_d64 yscale = 0.5;
	mlib_u32 *buff, *p_lup = (void *)mlib_nbits2;
	mlib_u32 *dp;

	GET_SIZE();

	w4 = (w / 4) + 2;
	buff = __mlib_malloc(2 * w4 * sizeof (mlib_u32) + 2 * s_width);

	if (buff == NULL)
		return (MLIB_FAILURE);
	sp0 = (mlib_u8 *)(buff + 2 * w4);
	sp1 = sp0 + s_width;

	if (lutGray != NULL) {
		mlib_s32 g1 = lutGray[1];
		p_lup = __mlib_malloc(256 * sizeof (mlib_u32));

		if (p_lup == NULL) {
			__mlib_free(buff);
			return (MLIB_FAILURE);
		}

		if (lutGray[0] == 0 && lutGray[2] == 2 * g1 &&
		    lutGray[3] == 3 * g1 && lutGray[4] == 4 * g1) {
			mlib_s32 gray2[4];

			gray2[0] = 0;
			gray2[1] = g1;
			gray2[2] = g1;
			gray2[3] = 2 * g1;
			for (i = 0; i < 256; i++) {
				mlib_s32 v;

#ifdef _LITTLE_ENDIAN
				v = gray2[i >> 6];
				v |= gray2[(i >> 4) & 3] << (8 * 1);
				v |= gray2[(i >> 2) & 3] << (8 * 2);
				v |= gray2[i & 3] << (8 * 3);
#else /* _LITTLE_ENDIAN */
				v = gray2[i >> 6] << (8 * 3);
				v |= gray2[(i >> 4) & 3] << (8 * 2);
				v |= gray2[(i >> 2) & 3] << (8 * 1);
				v |= gray2[i & 3];
#endif /* _LITTLE_ENDIAN */
				p_lup[i] = v;
			}
		} else {
			mlib_s32 gray4[16];

			gray4[0] = lutGray[0];
			gray4[1] = lutGray[1];
			gray4[2] = lutGray[1];
			gray4[3] = lutGray[2];
			gray4[4] = lutGray[1];
			gray4[5] = lutGray[2];
			gray4[6] = lutGray[2];
			gray4[7] = lutGray[3];
			gray4[8] = lutGray[1];
			gray4[9] = lutGray[2];
			gray4[10] = lutGray[2];
			gray4[11] = lutGray[3];
			gray4[12] = lutGray[2];
			gray4[13] = lutGray[3];
			gray4[14] = lutGray[3];
			gray4[15] = lutGray[4];

			lut_flag = 1;
			for (i = 0; i < 256; i++) {
#ifdef _LITTLE_ENDIAN
				p_lup[i] =
				    gray4[((i & 0xC0) >> 6) | (i & 0x0C)];
				p_lup[i] |=
				    gray4[((i & 0x30) >> 2) | (i & 0x03)] << 8;
#else /* _LITTLE_ENDIAN */
				p_lup[i] =
				    gray4[((i & 0xC0) >> 6) | (i & 0x0C)] << 8;
				p_lup[i] |=
				    gray4[((i & 0x30) >> 2) | (i & 0x03)];
#endif /* _LITTLE_ENDIAN */
			}
		}
	}
#ifdef _LITTLE_ENDIAN
	tail_mask =
	    ((w & 3) == 3) ? 0xFFFFFF : (((w & 3) == 2) ? 0xFFFF : 0xFF);
#else /* _LITTLE_ENDIAN */
	tail_mask = (mlib_s32)MLIB_S32_MIN >> (8 * (w & 3) - 1);
#endif /* _LITTLE_ENDIAN */

	for (j = 0; j < h; j++) {
		mlib_s32 off;

		if (bitoff) {
			mlib_ImageCopy_bit_na(sl, sp0, s_width, bitoff, 0);
			mlib_ImageCopy_bit_na(sl + sll, sp1, s_width, bitoff,
			    0);
		} else {
			sp0 = sl;
			sp1 = sl + sll;
		}

		off = (mlib_s32)dl & 3;

		if (off) {
			dp = buff + w4;
		} else {
			dp = (mlib_u32 *)dl;
		}

		if (!lut_flag) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < w / 4; i++) {
				s0 = sp0[i];
				s1 = sp1[i];
				dp[i] = p_lup[s0] + p_lup[s1];
			}

			if (w & 3) {
				s0 = sp0[i];
				s1 = sp1[i];
				rr = p_lup[s0] + p_lup[s1];
#ifdef _LITTLE_ENDIAN
				{
					mlib_u8 *dl = (mlib_u8 *)(dp + i);
					mlib_u32 last;

					last =
					    (rr & tail_mask) | (dp[i] &
					    ~tail_mask);
					for (i = 0; i < (w & 3);
					    i++, last >>= 8)
						dl[i] = last;
				}
#else /* _LITTLE_ENDIAN */
				{
					mlib_u8 *dl = (mlib_u8 *)(dp + i);
					mlib_u32 last;

					last =
					    (rr & tail_mask) | (dp[i] &
					    ~tail_mask);
					last >>= 8 * (4 - (w & 3));
					for (i = 0; i < (w & 3);
					    i++, last >>= 8)
						dl[(w & 3) - 1 - i] = last;
				}
#endif /* _LITTLE_ENDIAN */
			}

		} else {

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < w / 4; i++) {
				s0 = sp0[i];
				s1 = sp1[i];
#ifdef _LITTLE_ENDIAN
				dp[i] =
				    p_lup[(s0 & 0xF0) | (s1 >> 4)] +
				    (p_lup[((s0 & 0xF) << 4) | (s1 & 0xF)] <<
				    16);
#else /* _LITTLE_ENDIAN */
				dp[i] =
				    (p_lup[(s0 & 0xF0) | (s1 >> 4)] << 16) +
				    p_lup[((s0 & 0xF) << 4) | (s1 & 0xF)];
#endif /* _LITTLE_ENDIAN */
			}

			if (w & 3) {
				s0 = sp0[i];
				s1 = sp1[i];
				sa = (s0 & 0xF0) | (s1 >> 4);
				sb = ((s0 & 0xF) << 4) | (s1 & 0xF);
#ifdef _LITTLE_ENDIAN
				rr = p_lup[sa] + (p_lup[sb] << 16);
				{
					mlib_u8 *dl = (mlib_u8 *)(dp + i);
					mlib_u32 last;

					last =
					    (rr & tail_mask) | (dp[i] &
					    ~tail_mask);
					for (i = 0; i < (w & 3);
					    i++, last >>= 8)
						dl[i] = last;
				}
#else /* _LITTLE_ENDIAN */
				rr = (p_lup[sa] << 16) + p_lup[sb];
				{
					mlib_u8 *dl = (mlib_u8 *)(dp + i);
					mlib_u32 last;

					last =
					    (rr & tail_mask) | (dp[i] &
					    ~tail_mask);
					last >>= 8 * (4 - (w & 3));
					for (i = 0; i < (w & 3);
					    i++, last >>= 8)
						dl[(w & 3) - 1 - i] = last;
				}
#endif /* _LITTLE_ENDIAN */
			}
		}

		if (off) {
			mlib_ImageCopy_na((void *)dp, (void *)dl, w);
		}

		sl += 2 * sll;
		dl += dll;
	}

	__mlib_free(buff);

	if (p_lup != mlib_nbits2)
		__mlib_free(p_lup);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
