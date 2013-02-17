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

#ifndef _MLIB_S_IMAGECONSTLOGIC_BIT_H
#define	_MLIB_S_IMAGECONSTLOGIC_BIT_H

#pragma ident	"@(#)mlib_s_ImageConstLogic_Bit.h	9.5	07/11/05 SMI"

/*
 * Macro definitions for bit image logical functions.
 */

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

#define	VALIDATE()                                              \
/*                                                              \
 * pointers for pixel and line of source                        \
 */                                                             \
	mlib_u8 *sp;                                            \
/*                                                              \
 * pointers for pixel and line of dst                           \
 */                                                             \
	mlib_u8 *dp;                                            \
	                                                        \
	mlib_s32 width, height, nchannels;                      \
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
	mlib_s32 bitoffset1;                                    \
	                                                        \
	MLIB_IMAGE_SIZE_EQUAL(dst, src);                        \
	MLIB_IMAGE_TYPE_EQUAL(dst, src);                        \
	MLIB_IMAGE_CHAN_EQUAL(dst, src);                        \
	dp = (mlib_u8 *)mlib_ImageGetData(dst);                 \
	sp = (mlib_u8 *)mlib_ImageGetData(src);                 \
	height = mlib_ImageGetHeight(dst);                      \
	width = mlib_ImageGetWidth(dst);                        \
	stride1 = mlib_ImageGetStride(src);                     \
	strided = mlib_ImageGetStride(dst);                     \
	nchannels = mlib_ImageGetChannels(dst);                 \
	bitoffset1 = mlib_ImageGetBitOffset(src);               \
	bitoffsetd = mlib_ImageGetBitOffset(dst);               \
	if ((width > stride1 * 8) || (width > strided * 8))     \
	    return MLIB_FAILURE

/* *********************************************************** */

CONST_LOGIC_BITS
{
	VALIDATE();

	mlib_u8 *dpp, *spp;
	mlib_u8 masks[8] = {0, 128, 192, 224, 240, 248, 252, 254};
	mlib_u8 c0, c1, c2, buff1, buff2;
	mlib_s32 i, j, k;
	mlib_s32 offdst, offsrc;
	mlib_s32 bithead, bittail;
	mlib_s32 amount, nblock;
	__m128i mx0, mx1, mx2, s0, s1, s2;
	__m128i ones = _mm_set1_epi32(0xffffffff);

	width = width * nchannels;
	if (nchannels == 1) {
	    c0 = (((mlib_s32)(c[0] & 1)) << 31) >> 31;
	} else if (nchannels == 2) {
	    c0 = ((c[0] & 1) << 1) | (c[1] & 1);
	    c0 |= (c0 << 2);
	    c0 |= (c0 << 4);
	} else if (nchannels == 3) {
	    c0 = ((c[0] & 1) << 2) | ((c[1] & 1) << 1) | (c[2] & 1);
	    c0 |= (c0 << 3);
	    c1 = (c0 << 4) | (c0 >> 2);
	    c2 = (c0 << 3) | (c0 >> 3);
	    c0 = (c0 << 2) | (c0 >> 4);
	} else {
	    c0 = ((c[0] & 1) << 3) | ((c[1] & 1) << 2) | ((c[2] & 1) << 1)
		| (c[3] & 1);
	    c0 |= (c0 << 4);
	}

#ifdef REV_CONST
	c0 = ~c0;
	c1 = ~c1;
	c2 = ~c2;
#endif

	if ((width == stride1 * 8) &&
	    (width == strided * 8)) {
		amount = (height * width);
		height = 1;
	} else
		amount = width;

	if (bitoffset1 == bitoffsetd) {
	    amount += bitoffsetd;
	    bithead = bitoffsetd & 7;
	    bittail = amount & 7;
	    amount = (amount >> 3) + ((bittail) ? 1 : 0);

	    if (nchannels == 3) {
		mlib_u8 ctmp0 = c0, ctmp1 = c1, ctmp2 = c2;
		c0 = (ctmp2 << (8 - bithead)) | (c0 >> bithead);
		c1 = (ctmp0 << (8 - bithead)) | (c1 >> bithead);
		c2 = (ctmp1 << (8 - bithead)) | (c2 >> bithead);

		if (amount < 32) {
		    nblock = amount / 3;
		    for (j = 0; j < height; j++) {
			dpp = dp;
			spp = sp;
			buff1 = dpp[0];
			buff2 = dpp[amount-1];

			for (i = 0; i < nblock; i++) {
				*dp = CONSTLOGIC(c0, *sp);
				*(dp + 1) = CONSTLOGIC(c1, *(sp + 1));
				*(dp + 2) = CONSTLOGIC(c2, *(sp + 2));
				dp += 3;
				sp += 3;
			}
			if (amount - nblock * 3 > 0) {
				*dp = CONSTLOGIC(c0, *sp);
				dp++;
				sp++;
			}
			if (amount - nblock * 3 > 1) {
				*dp = CONSTLOGIC(c1, *sp);
				dp++;
				sp++;
			}

			if (bithead) {
				mlib_u8 mask = masks[bithead];
				*dpp = (*dpp & (~mask)) | (buff1 & mask);
			}
			if (bittail) {
				mlib_u8 mask = masks[bittail];
				dp--;
				*dp = (*dp & mask) | (buff2 & (~mask));
			}

			sp = spp + stride1;
			dp = dpp + strided;
		    }
		} else {
		    mlib_u32 cc0, cc1, cc2;
		    cc0 = c0 | (c1 << 8) | (c2 << 16) | (c0 << 24);
		    cc1 = c1 | (c2 << 8) | (c0 << 16) | (c1 << 24);
		    cc2 = c2 | (c0 << 8) | (c1 << 16) | (c2 << 24);
		    mx0 = _mm_setr_epi32(cc0, cc1, cc2, cc0);
		    mx1 = _mm_setr_epi32(cc1, cc2, cc0, cc1);
		    mx2 = _mm_setr_epi32(cc2, cc0, cc1, cc2);

		    nblock = amount / 3;
		    k = amount / 48;
		    for (j = 0; j < height; j++) {
			dpp = dp;
			spp = sp;
			buff1 = dpp[0];
			buff2 = dpp[amount-1];

			for (i = 0; i < k; i++) {
				s0 = _mm_loadu_si128((__m128i *)sp);
				s1 = _mm_loadu_si128((__m128i *)(sp + 16));
				s2 = _mm_loadu_si128((__m128i *)(sp + 32));
				s0 = CONSTLOGIC_SSE2(mx0, s0);
				s1 = CONSTLOGIC_SSE2(mx1, s1);
				s2 = CONSTLOGIC_SSE2(mx2, s2);
				_mm_storeu_si128((__m128i *)dp, s0);
				_mm_storeu_si128((__m128i *)(dp + 16), s1);
				_mm_storeu_si128((__m128i *)(dp + 32), s2);
				sp += 48;
				dp += 48;
			}
			for (i = k * 16; i < nblock; i++) {
				*dp = CONSTLOGIC(c0, *sp);
				*(dp + 1) = CONSTLOGIC(c1, *(sp + 1));
				*(dp + 2) = CONSTLOGIC(c2, *(sp + 2));
				dp += 3;
				sp += 3;
			}
			if (amount - nblock * 3 > 0) {
				*dp = CONSTLOGIC(c0, *sp);
				dp++;
				sp++;
			}
			if (amount - nblock * 3 > 1) {
				*dp = CONSTLOGIC(c1, *sp);
				dp++;
				sp++;
			}

			if (bithead) {
				mlib_u8 mask = masks[bithead];
				*dpp = (*dpp & (~mask)) | (buff1 & mask);
			}
			if (bittail) {
				mlib_u8 mask = masks[bittail];
				dp--;
				*dp = (*dp & mask) | (buff2 & (~mask));
			}

			sp = spp + stride1;
			dp = dpp + strided;
		    }

		}
	    } else { /* nchannels = 1, 2, 4 */
		c0 = (c0 << (8 - bithead)) | (c0 >> bithead);
		if (amount < 32) {
		    for (j = 0; j < height; j++) {
			dpp = dp;
			spp = sp;
			buff1 = dpp[0];
			buff2 = dpp[amount-1];

			for (i = 0; i < amount; i++) {
				*dp = CONSTLOGIC(c0, *sp);
				dp++;
				sp++;
			}
			if (bithead) {
				mlib_u8 mask = masks[bithead];
				*dpp = (*dpp & (~mask)) | (buff1 & mask);
			}
			if (bittail) {
				mlib_u8 mask = masks[bittail];
				dp--;
				*dp = (*dp & mask) | (buff2 & (~mask));
			}

			sp = spp + stride1;
			dp = dpp + strided;
		    }
		} else {
		    mx0 = _mm_set1_epi8(c0);
		    for (j = 0; j < height; j++) {
			dpp = dp;
			spp = sp;
			buff1 = dpp[0];
			buff2 = dpp[amount-1];

			offsrc = (16 - ((mlib_addr)sp & 15)) & 15;
			offdst = (16 - ((mlib_addr)dp & 15)) & 15;
			k = (amount - offsrc) >> 4;
			for (i = 0; i < offsrc; i++) {
				*dp = CONSTLOGIC(c0, *sp);
				sp++;
				dp++;
			}
			if (offsrc == offdst) {
			    for (i = 0; i < k; i++) {
				s0 = _mm_load_si128((__m128i *)sp);
				s0 = CONSTLOGIC_SSE2(mx0, s0);
				_mm_store_si128((__m128i *)dp, s0);
				sp += 16;
				dp += 16;
			    }
			} else {
			    for (i = 0; i < k; i++) {
				s0 = _mm_load_si128((__m128i *)sp);
				s0 = CONSTLOGIC_SSE2(mx0, s0);
				_mm_storeu_si128((__m128i *)dp, s0);
				sp += 16;
				dp += 16;
			    }
			}
			for (i = offsrc + k * 16; i < amount; i++) {
				*dp = CONSTLOGIC(c0, *sp);
				sp++;
				dp++;
			}
			if (bithead) {
				mlib_u8 mask = masks[bithead];
				*dpp = (*dpp & (~mask)) | (buff1 & mask);
			}
			if (bittail) {
				mlib_u8 mask = masks[bittail];
				dp--;
				*dp = (*dp & mask) | (buff2 & (~mask));
			}

			sp = spp + stride1;
			dp = dpp + strided;
		    }

		}

	    }
	} else { /* bitoffset1 != bitoffsetd */
	    bithead = 8 - bitoffsetd;
	    bithead = (bithead < amount) ? bithead : amount;
	    bittail = ((amount + bitoffsetd) & (~7)) - bitoffsetd;
	    bittail = (bittail > 0) ? bittail : amount;
	    mlib_s32 nbytes = ((amount + bitoffsetd) >> 3) - 1;
	    mlib_s32 nsrcbeg = (bithead + bitoffset1) >> 3;
	    mlib_s32 left = (bithead + bitoffset1) & 7;
	    mlib_s32 right = 8 - left;

	    if (nchannels == 3) {
		mlib_u8 ctmp0 = c0, ctmp1 = c1, ctmp2 = c2;
		c0 = (ctmp2 << (8 - bitoffsetd)) | (c0 >> bitoffsetd);
		c1 = (ctmp0 << (8 - bitoffsetd)) | (c1 >> bitoffsetd);
		c2 = (ctmp1 << (8 - bitoffsetd)) | (c2 >> bitoffsetd);
		nblock = nbytes / 3;
		for (j = 0; j < height; j++) {
		    spp = sp;
		    dpp = dp;
		    for (i = 0; i < bithead; i++) {
			mlib_u8 mask, a, tmp;
			mlib_s32 ii;
			ii = (i + bitoffset1) >> 3;
			tmp = sp[ii] >> (7 - (i + bitoffset1)&7);
			a = CONSTLOGIC_bit(c[i % nchannels], tmp);
			ii = (i + bitoffsetd) >> 3;
			mask = 1 << (7 - (i + bitoffsetd)&7);
			a <<= (7 - (i + bitoffsetd)&7);
			dp[ii] = (dp[ii] & (~mask)) | (a & mask);
		    }
		    for (i = bittail; i < amount; i++) {
			mlib_u8 mask, a, tmp;
			mlib_s32 ii;
			ii = (i+bitoffset1) >> 3;
			tmp = sp[ii] >> (7 - (i + bitoffset1)&7);
			a = CONSTLOGIC_bit(c[i % nchannels], tmp);
			ii = (i+bitoffsetd) >> 3;
			mask = 1 << (7 - (i + bitoffsetd)&7);
			a <<= (7 - (i + bitoffsetd)&7);
			dp[ii] = (dp[ii] & (~mask)) | (a & mask);
		    }
		    sp += nsrcbeg;
		    mlib_u8 bak1, bak0;
		    bak0 = *sp;
		    for (i = 0; i < nblock; i++) {
			sp++;
			dp++;
			bak1 = *sp;
			*dp = CONSTLOGIC(c1, (bak0 << left) | (bak1 >> right));
			bak0 = bak1;

			sp++;
			dp++;
			bak1 = *sp;
			*dp = CONSTLOGIC(c2, (bak0 << left) | (bak1 >> right));
			bak0 = bak1;

			sp++;
			dp++;
			bak1 = *sp;
			*dp = CONSTLOGIC(c0, (bak0 << left) | (bak1 >> right));
			bak0 = bak1;
		    }
		    if (nbytes - nblock * 3 > 0) {
			sp++;
			dp++;
			bak1 = *sp;
			*dp = CONSTLOGIC(c1, (bak0 << left) | (bak1 >> right));
			bak0 = bak1;
		    }
		    if (nbytes - nblock * 3 > 1) {
			sp++;
			dp++;
			bak1 = *sp;
			*dp = CONSTLOGIC(c2, (bak0 << left) | (bak1 >> right));
			bak0 = bak1;
		    }

		    sp = spp + stride1;
		    dp = dpp + strided;
		}

	    } else { /* nchannels == 1, 2, 4 */
		c0 = (c0 << (8 - bitoffsetd)) | (c0 >> bitoffsetd);
		for (j = 0; j < height; j++) {
		    spp = sp;
		    dpp = dp;

		    for (i = 0; i < bithead; i++) {
			mlib_u8 mask, a, tmp;
			mlib_s32 ii;
			ii = (i + bitoffset1) >> 3;
			tmp = sp[ii] >> (7 - (i + bitoffset1)&7);
			a = CONSTLOGIC_bit(c[i % nchannels], tmp);
			ii = (i + bitoffsetd) >> 3;
			mask = 1 << (7 - (i + bitoffsetd)&7);
			a <<= (7 - (i + bitoffsetd)&7);
			dp[ii] = (dp[ii] & (~mask)) | (a & mask);
		    }
		    for (i = bittail; i < amount; i++) {
			mlib_u8 mask, a, tmp;
			mlib_s32 ii;
			ii = (i+bitoffset1) >> 3;
			tmp = sp[ii] >> (7 - (i + bitoffset1)&7);
			a = CONSTLOGIC_bit(c[i % nchannels], tmp);
			ii = (i+bitoffsetd) >> 3;
			mask = 1 << (7 - (i + bitoffsetd)&7);
			a <<= (7 - (i + bitoffsetd)&7);
			dp[ii] = (dp[ii] & (~mask)) | (a & mask);
		    }
		    sp += nsrcbeg;
		    mlib_u8 bak1, bak0;
		    bak0 = *sp;
		    for (i = 0; i < nbytes; i++) {
			sp++;
			dp++;
			bak1 = *sp;
			*dp = CONSTLOGIC(c0, (bak0 << left) | (bak1 >> right));
			bak0 = bak1;
		    }

		    sp = spp + stride1;
		    dp = dpp + strided;
		}
	    }
	}
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#ifdef __cplusplus
}
#endif

#endif /* _MLIB_S_IMAGECONSTLOGIC_BIT_H */
