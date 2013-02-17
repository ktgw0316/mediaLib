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

#pragma ident	"@(#)mlib_s_ImageNot_Bit.c	9.3	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_ImageNot   - not an bit type image (C version)
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageNot(mlib_image       *dst,
 *                                const mlib_image *src);
 *
 * ARGUMENT
 *      dst     pointer to destination image
 *      src     pointer to source image
 *
 * RESTRICTION
 *      The src and dst must be the same type and the same size.
 *      They can have 1, 2, 3, or 4 channels.
 *      They can be in MLIB_BIT data type.
 *
 * DESCRIPTION
 *      Not an image for each channel:  dst = ~src
 */

#include <mlib_image.h>
#include <mlib_ImageCheck.h>
#include <mlib_ImageLogic_proto.h>

#ifdef __SUNPRO_C
#include <sunmedia_intrin.h>
#else
#include <emmintrin.h>
#endif

/* *********************************************************** */

#define	VALIDATE()                                              \
/*                                                              \
 * pointers for pixel and line of source                        \
 */                                                             \
	mlib_u8 *sp;                                           \
/*                                                              \
 * pointers for pixel and line of dst                           \
 */                                                             \
	mlib_u8 *dp;                                            \
	                                                        \
	mlib_s32 width, height, channels;                       \
/*                                                              \
 * for src                                                      \
 */                                                             \
	mlib_s32 stride1;                                       \
/*                                                              \
 * for dst                                                      \
 */                                                             \
	mlib_s32 strided;                                       \
/*                                                              \
 * for dst                                                      \
 */                                                             \
	mlib_s32 bitoffsetd;                                    \
/*                                                              \
 * for src                                                      \
 */                                                             \
	mlib_s32 bitoffset1;                                     \
	                                                        \
	MLIB_IMAGE_SIZE_EQUAL(dst, src);                        \
	MLIB_IMAGE_TYPE_EQUAL(dst, src);                        \
	MLIB_IMAGE_CHAN_EQUAL(dst, src);                        \
	dp = (mlib_u8 *)mlib_ImageGetData(dst);                 \
	sp = (mlib_u8 *)mlib_ImageGetData(src);                \
	height = mlib_ImageGetHeight(dst);                      \
	width = mlib_ImageGetWidth(dst);                        \
	stride1 = mlib_ImageGetStride(src);                     \
	strided = mlib_ImageGetStride(dst);                     \
	channels = mlib_ImageGetChannels(dst);                  \
	bitoffset1 = mlib_ImageGetBitOffset(src);                \
	bitoffsetd = mlib_ImageGetBitOffset(dst);               \
	width *= channels;                                      \
	if (bitoffset1 > 7 || bitoffsetd > 7)			\
	    return (MLIB_FAILURE);				\
	if ((width + bitoffset1 > stride1 * 8) ||		\
		(width + bitoffsetd > strided * 8))     	\
	    return MLIB_FAILURE

/* *********************************************************** */

mlib_status
mlib_ImageNot_Bit(
    mlib_image *dst,
    const mlib_image *src)
{
	VALIDATE();
	mlib_s32 amount;

	if ((width == stride1 * 8) && (width == strided * 8)) {
		amount = height * width;
		height = 1;
	} else
		amount = width;

	mlib_s32 i, j, k;
	mlib_s32 bithead, bittail;
	mlib_u8 masks[8] = {0, 128, 192, 224, 240, 248, 252, 254};
	mlib_u8 buff1, buff2, *dpbeg, *spbeg;
	if (bitoffset1 == bitoffsetd) {
	    amount += bitoffsetd;
	    bithead = bitoffsetd & 7;
	    bittail = amount & 7;
	    amount = (amount >> 3) + ((bittail) ? 1 : 0);
	    stride1 -= amount;
	    strided -= amount;

	    if (amount < 32) {
		for (j = 0; j < height; j++) {
			dpbeg = dp;
			buff1 = dpbeg[0];
			buff2 = dpbeg[amount-1];

			for (i = 0; i < (amount>>2); i++) {
				*(mlib_u32 *)dp = ~(*(mlib_u32 *)sp);
				dp += 4;
				sp += 4;
			}
			for (i = 0; i < (amount & 3); i++) {
				*dp = ~(*sp);
				dp++;
				sp++;
			}

			if (bithead) {
				mlib_u8 mask = masks[bithead];
				*dpbeg = (*dpbeg & (~mask)) | (buff1 & mask);
			}
			if (bittail) {
				mlib_u8 mask = masks[bittail];
				dpbeg += amount - 1;
				*dpbeg = (*dpbeg & mask) | (buff2 & (~mask));
			}

			sp += stride1;
			dp += strided;
		}
	    } else {
		__m128i src1, src2, dst1, dst2, ones;
		ones = _mm_set1_epi32(0xffffffff);
		for (j = 0; j < height; j++) {
			mlib_s32 offdst, offsrc;
			dpbeg = dp;
			buff1 = dpbeg[0];
			buff2 = dpbeg[amount-1];

			offsrc = (16 - ((mlib_addr)sp & 15)) & 15;
			offdst = (16 - ((mlib_addr)dp & 15)) & 15;
			k = (amount - offsrc) >> 4;
			for (i = 0; i < offsrc; i++) {
				*dp = ~(*sp);
				sp++;
				dp++;
			}

			if (offsrc == offdst) {
			    int kk = k >> 1;

			    for (i = 0; i < kk; i++) {
				src1 = _mm_load_si128((__m128i *)sp);
				src2 = _mm_load_si128((__m128i *)(sp + 16));
				sp += 32;

				dst1 = _mm_andnot_si128(src1, ones);
				dst2 = _mm_andnot_si128(src2, ones);
				_mm_store_si128((__m128i *)dp, dst1);
				_mm_store_si128((__m128i *)(dp + 16), dst2);
				dp  += 32;
			    }
			    if (k & 1) {
				src1 = _mm_load_si128((__m128i *)sp);
				dst1 = _mm_andnot_si128(src1, ones);
				_mm_store_si128((__m128i *)dp, dst1);
				sp += 16;
				dp  += 16;
			    }
			} else {
			    int kk = k >> 1;

			    for (i = 0; i < kk; i++) {
				src1 = _mm_load_si128((__m128i *)sp);
				src2 = _mm_load_si128((__m128i *)(sp + 16));
				sp += 32;

				dst1 = _mm_andnot_si128(src1, ones);
				dst2 = _mm_andnot_si128(src2, ones);
				_mm_storeu_si128((__m128i *)dp, dst1);
				_mm_storeu_si128((__m128i *)(dp + 16), dst2);
				dp  += 32;
			    }
			    if (k & 1) {
				src1 = _mm_load_si128((__m128i *)sp);
				dst1 = _mm_andnot_si128(src1, ones);
				_mm_storeu_si128((__m128i *)dp, dst1);
				sp += 16;
				dp  += 16;
			    }
			}

			for (i = offsrc + k * 16; i < amount; i++) {
				*dp = ~(*sp);
				sp++;
				dp++;
			}

			if (bithead) {
				mlib_u8 mask = masks[bithead];
				*dpbeg = (*dpbeg & (~mask)) | (buff1 & mask);
			}
			if (bittail) {
				mlib_u8 mask = masks[bittail];
				dpbeg += amount - 1;
				*dpbeg = (*dpbeg & mask) | (buff2 & (~mask));
			}

			sp += stride1;
			dp += strided;
		}
	    }
	} else {	/* bitoffset1 != bitoffsetd */
	    bithead = 8 - bitoffsetd;
	    bithead = (bithead < amount) ? bithead : amount;
	    bittail = ((amount + bitoffsetd) & (~7)) - bitoffsetd;
	    bittail = (bittail > 0) ? bittail : amount;
	    mlib_s32 nbytes = ((amount + bitoffsetd) >> 3) - 1;
	    mlib_s32 nsrcbeg = (bithead + bitoffset1) >> 3;
	    mlib_s32 left = (bithead + bitoffset1) & 7;
	    mlib_s32 right = 8 - left;
	    for (j = 0; j < height; j++) {
		spbeg = sp;
		dpbeg = dp;

		for (i = 0; i < bithead; i++) {
			mlib_u8 mask, a, tmp;
			mlib_s32 ii;
			ii = (i+bitoffset1) >> 3;
			tmp = sp[ii] >> (7 - (i+bitoffset1)&7);
			a = ~tmp;
			ii = (i+bitoffsetd) >> 3;
			mask = 1 << (7 - (i+bitoffsetd)&7);
			a <<= (7 - (i+bitoffsetd)&7);
			dp[ii] = (dp[ii] & (~mask)) | (a & mask);
		}
		for (i = bittail; i < amount; i++) {
			mlib_u8 mask, a, tmp;
			mlib_s32 ii;
			ii = (i+bitoffset1) >> 3;
			tmp = sp[ii] >> (7 - (i+bitoffset1)&7);
			a = ~tmp;
			ii = (i+bitoffsetd) >> 3;
			mask = 1 << (7 - (i+bitoffsetd)&7);
			a <<= (7 - (i+bitoffsetd)&7);
			dp[ii] = (dp[ii] & (~mask)) | (a & mask);
		}
		sp += nsrcbeg;
		mlib_u8 bak1, bak0 = ~(*sp);
		for (i = 0; i < nbytes; i++) {
			sp++;
			dp++;
			bak1 = ~(*sp);
			*dp = (bak0 << left) | (bak1 >> right);
			bak0 = bak1;
		}

		sp = spbeg + stride1;
		dp = dpbeg + strided;
	    }
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
