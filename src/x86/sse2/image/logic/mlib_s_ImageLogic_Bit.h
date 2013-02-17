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

#ifndef _MLIB_S_IMAGELOGIC_BIT_H
#define	_MLIB_S_IMAGELOGIC_BIT_H

#pragma ident	"@(#)mlib_s_ImageLogic_Bit.h	9.5	07/11/05 SMI"

#ifdef __cplusplus
extern "C" {
#endif

#include <mlib_image.h>
#include <mlib_ImageCheck.h>
#include <mlib_ImageLogic_proto.h>

#ifdef __SUNPRO_C
#include <sunmedia_intrin.h>
#else
#include <emmintrin.h>
#endif

/* *********************************************************** */

#define	VALIDATE()                                                \
	mlib_u8 *sp1;                                             \
	                                                          \
/*                                                                \
 * pointers for pixel and line of source                          \
 */                                                               \
	mlib_u8 *sp2;                                             \
	                                                          \
/*                                                                \
 * pointers for pixel and line of source                          \
 */                                                               \
	mlib_u8 *dp;                                              \
	                                                          \
/*                                                                \
 * pointers for pixel and line of dst                             \
 */                                                               \
	mlib_s32 width, height, channels;                         \
	mlib_s32 stride1;                                         \
	                                                          \
/*                                                                \
 * for src1                                                       \
 */                                                               \
	mlib_s32 stride2;                                         \
	                                                          \
/*                                                                \
 * for src2                                                       \
 */                                                               \
	mlib_s32 strided;                                         \
	                                                          \
/*                                                                \
 * for dst                                                        \
 */                                                               \
	mlib_s32 bitoffsetd;                                      \
	                                                          \
/*                                                                \
 * for dst                                                        \
 */                                                               \
	mlib_s32 bitoffset1;                                      \
	                                                          \
/*                                                                \
 * for src1                                                       \
 */                                                               \
	mlib_s32 bitoffset2;                                      \
	                                                          \
/*                                                                \
 * for src2                                                       \
 */                                                               \
	MLIB_IMAGE_SIZE_EQUAL(dst, src1);                         \
	MLIB_IMAGE_TYPE_EQUAL(dst, src1);                         \
	MLIB_IMAGE_CHAN_EQUAL(dst, src1);                         \
	MLIB_IMAGE_SIZE_EQUAL(dst, src2);                         \
	MLIB_IMAGE_TYPE_EQUAL(dst, src2);                         \
	MLIB_IMAGE_CHAN_EQUAL(dst, src2);                         \
	dp = (mlib_u8 *)mlib_ImageGetData(dst);                   \
	sp1 = (mlib_u8 *)mlib_ImageGetData(src1);                 \
	sp2 = (mlib_u8 *)mlib_ImageGetData(src2);                 \
	height = mlib_ImageGetHeight(dst);                        \
	width = mlib_ImageGetWidth(dst);                          \
	stride1 = mlib_ImageGetStride(src1);                      \
	stride2 = mlib_ImageGetStride(src2);                      \
	strided = mlib_ImageGetStride(dst);                       \
	channels = mlib_ImageGetChannels(dst);                    \
	bitoffset1 = mlib_ImageGetBitOffset(src1);                \
	bitoffset2 = mlib_ImageGetBitOffset(src2);                \
	bitoffsetd = mlib_ImageGetBitOffset(dst);                 \
	width *= channels;                                        \
	if (width > stride1 * 8 || width > stride2 * 8 ||         \
	    width > strided * 8)                                  \
		return MLIB_FAILURE

/* *********************************************************** */

LOGIC_BITS
{
	VALIDATE();

	mlib_s32 i, j, k;
	mlib_s32 amount, bithead, bittail;
	mlib_u8 masks[8] = {0, 128, 192, 224, 240, 248, 252, 254};
	mlib_u8 buff1, buff2, *dpbeg, *sp1beg, *sp2beg;

	if ((width == stride1 * 8) &&
	    (width == stride2 * 8) &&
	    (width == strided * 8)) {
		amount = (height * width);
		height = 1;
	} else
		amount = width;

	if (bitoffset1 == bitoffsetd && bitoffset2 == bitoffsetd) {
	    amount += bitoffsetd;
	    bithead = bitoffsetd & 7;
	    bittail = amount & 7;
	    amount = (amount >> 3) + ((bittail) ? 1 : 0);
	    stride1 -= amount;
	    stride2 -= amount;
	    strided -= amount;

	    if (amount < 128) { /* too short to use sse2 */
		for (j = 0; j < height; j++) {
			dpbeg = dp;
			buff1 = dpbeg[0];
			buff2 = dpbeg[amount-1];

			for (i = 0; i < (amount>>2); i++) {
				*(mlib_u32 *)dp = OPERATION(*(mlib_u32 *)sp1,
						*(mlib_u32 *)sp2);
				dp += 4;
				sp1 += 4;
				sp2 += 4;
			}
			for (i = 0; i < (amount & 3); i++) {
				*dp = OPERATION(*sp1, *sp2);
				dp++;
				sp1++;
				sp2++;
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

			sp1 += stride1;
			sp2 += stride2;
			dp += strided;
		}
	    } else {	/* sse2 implementation */
		__m128i src1, src2, dst;
		__m128i ones = _mm_set1_epi32(0xffffffff);
		mlib_s32 offsrc1, offsrc2, offdst;

		for (j = 0; j < height; j++) {
			dpbeg = dp;
			buff1 = dpbeg[0];
			buff2 = dpbeg[amount-1];


			offsrc1 = (16 - ((mlib_addr)sp1 & 15)) & 15;
			offsrc2 = (16 - ((mlib_addr)sp2 & 15)) & 15;
			offdst = (16 - ((mlib_addr)dp & 15)) & 15;
			if (offsrc1 == offdst && offsrc2 == offdst) {
				k = (amount - offdst) >> 4;

				for (i = 0; i < offdst; i++) {
					*dp = OPERATION(*sp1, *sp2);
					sp1++;
					sp2++;
					dp++;
				}

				int kk = k >> 1;
				__m128i s1, s2, d;

				for (i = 0; i < kk; i++) {
					src1 = _mm_load_si128((__m128i *)sp1);
					src2 = _mm_load_si128((__m128i *)sp2);
					s1 = _mm_load_si128((__m128i *)(sp1
								+ 16));
					s2 = _mm_load_si128((__m128i *)(sp2
								+ 16));
					dst = OPERATION_SSE2(src1, src2);
					d = OPERATION_SSE2(s1, s2);
					_mm_store_si128((__m128i *)dp, dst);
					_mm_store_si128((__m128i *)(dp + 16),
								d);
					sp1 += 32;
					sp2 += 32;
					dp += 32;
				}
				if (k & 1) {
					src1 = _mm_load_si128((__m128i *)sp1);
					src2 = _mm_load_si128((__m128i *)sp2);
					dst = OPERATION_SSE2(src1, src2);
					_mm_store_si128((__m128i *)dp, dst);
					sp1 += 16;
					sp2 += 16;
					dp += 16;
				}

				for (i = offdst + k * 16; i < amount; i++) {
					*dp = OPERATION(*sp1, *sp2);
					sp1++;
					sp2++;
					dp++;
				}
			} else {
				for (i = 0; i < (amount>>2); i++) {
					*(mlib_u32 *)dp = OPERATION(
							*(mlib_u32 *)sp1,
							*(mlib_u32 *)sp2);
					dp += 4;
					sp1 += 4;
					sp2 += 4;
				}
				for (i = 0; i < (amount & 3); i++) {
					*dp = OPERATION(*sp1, *sp2);
					dp++;
					sp1++;
					sp2++;
				}
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

			sp1 += stride1;
			sp2 += stride2;
			dp += strided;
		}
	    }
	} else {	/* not aligned */

	    bithead = 8 - bitoffsetd;
	    bithead = (bithead < amount) ? bithead : amount;
	    bittail = ((amount + bitoffsetd) & (~7)) - bitoffsetd;
	    bittail = (bittail > 0) ? bittail : amount;
	    mlib_s32 nbytes = ((amount + bitoffsetd) >> 3) - 1;
	    mlib_s32 nsrc1beg = (bithead + bitoffset1) >> 3;
	    mlib_s32 nsrc2beg = (bithead + bitoffset2) >> 3;
	    mlib_s32 left1 = (bithead + bitoffset1) & 7;
	    mlib_s32 right1 = 8 - left1;
	    mlib_s32 left2 = (bithead + bitoffset2) & 7;
	    mlib_s32 right2 = 8 - left2;
	    for (j = 0; j < height; j++) {
		sp1beg = sp1;
		sp2beg = sp2;
		dpbeg = dp;

		mlib_u8 mask, a, tmp1, tmp2;
		for (i = 0; i < bithead; i++) {
			mlib_s32 ii;
			ii = (i+bitoffset1) >> 3;
			tmp1 = sp1[ii] >> (7 - (i+bitoffset1)&7);
			ii = (i+bitoffset2) >> 3;
			tmp2 = sp2[ii] >> (7 - (i+bitoffset2)&7);
			a = OPERATION(tmp1, tmp2);
			ii = (i+bitoffsetd) >> 3;
			mask = 1 << (7 - (i+bitoffsetd)&7);
			a <<= (7 - (i+bitoffsetd)&7);
			dp[ii] = (dp[ii] & (~mask)) | (a & mask);
		}
		for (i = bittail; i < amount; i++) {
			mlib_s32 ii;
			ii = (i+bitoffset1) >> 3;
			tmp1 = sp1[ii] >> (7 - (i+bitoffset1)&7);
			ii = (i+bitoffset2) >> 3;
			tmp2 = sp2[ii] >> (7 - (i+bitoffset2)&7);
			a = OPERATION(tmp1, tmp2);
			ii = (i+bitoffsetd) >> 3;
			mask = 1 << (7 - (i+bitoffsetd)&7);
			a <<= (7 - (i+bitoffsetd)&7);
			dp[ii] = (dp[ii] & (~mask)) | (a & mask);
		}

		sp1 += nsrc1beg;
		sp2 += nsrc2beg;
		mlib_u8 bak11, bak10, bak21, bak20;
		bak10 = *sp1;
		bak20 = *sp2;
		for (i = 0; i < nbytes; i++) {
			sp1++;
			sp2++;
			dp++;
			bak11 = *sp1;
			bak21 = *sp2;
			tmp1 = (bak10 << left1) | (bak11 >> right1);
			tmp2 = (bak20 << left2) | (bak21 >> right2);
			*dp = OPERATION(tmp1, tmp2);
			bak10 = bak11;
			bak20 = bak21;
		}

		sp1 = sp1beg + stride1;
		sp2 = sp2beg + stride2;
		dp = dpbeg + strided;
	    }
	}
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#ifdef __cplusplus
}
#endif

#endif	/* _MLIB_S_IMAGELOGIC_BIT_H */
