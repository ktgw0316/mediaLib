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

#ifndef _MLIB_S_IMAGELOGIC_H
#define	_MLIB_S_IMAGELOGIC_H

#pragma ident	"@(#)mlib_s_ImageLogic.h	9.4	07/11/05 SMI"

/*
 * Functions for C version image logical functions.
 */

#ifdef __cplusplus
extern "C" {
#endif

#include <mlib_ImageCheck.h>
#include <mlib_ImageLogic_proto.h>

#ifdef __SUNPRO_C
#include <sunmedia_intrin.h>
#else
#include <emmintrin.h>
#endif

/* *********************************************************** */

#define	VALIDATE()                                                         \
	mlib_u8 *sp1;                                                      \
	                                                                   \
/*                                                                         \
 * pointers for pixel and line of source                                   \
 */                                                                        \
	mlib_u8 *sp2;                                                      \
	                                                                   \
/*                                                                         \
 * pointers for pixel and line of source                                   \
 */                                                                        \
	mlib_u8 *dp;                                                       \
	                                                                   \
/*                                                                         \
 * pointers for pixel and line of dst                                      \
 */                                                                        \
	mlib_s32 width, height, channels, type;                            \
									   \
	mlib_s32 stride1;                                                  \
/*                                                                         \
 * for src1                                                                \
 */                                                                        \
	mlib_s32 stride2;                                                  \
/*                                                                         \
 * for src2                                                                \
 */                                                                        \
	mlib_s32 strided;                                                  \
/*                                                                         \
 * for dst                                                                 \
 */                                                                        \
	MLIB_IMAGE_SIZE_EQUAL(dst, src1);                                  \
	MLIB_IMAGE_TYPE_EQUAL(dst, src1);                                  \
	MLIB_IMAGE_CHAN_EQUAL(dst, src1);                                  \
	MLIB_IMAGE_SIZE_EQUAL(dst, src2);                                  \
	MLIB_IMAGE_TYPE_EQUAL(dst, src2);                                  \
	MLIB_IMAGE_CHAN_EQUAL(dst, src2);                                  \
	dp = (mlib_u8 *)mlib_ImageGetData(dst);                            \
	sp1 = (mlib_u8 *)mlib_ImageGetData(src1);                          \
	sp2 = (mlib_u8 *)mlib_ImageGetData(src2);                          \
	height = mlib_ImageGetHeight(dst);                                 \
	width = mlib_ImageGetWidth(dst);                                   \
	stride1 = mlib_ImageGetStride(src1);                               \
	stride2 = mlib_ImageGetStride(src2);                               \
	strided = mlib_ImageGetStride(dst);                                \
	channels = mlib_ImageGetChannels(dst);                             \
	type = mlib_ImageGetType(dst);                                     \
	if ((type == MLIB_SHORT) || (type == MLIB_USHORT))                 \
	    width *= 2;                                                    \
	else if (type == MLIB_INT)                                         \
	    width *= 4;                                                    \
	if ((width * channels) > stride1 ||                                \
	    (width * channels) > stride2 || (width * channels) > strided)  \
	    return MLIB_FAILURE

/* *********************************************************** */

static mlib_status
mlib_s_ImageLogic(
    mlib_image *dst,
    const mlib_image *src1,
    const mlib_image *src2)
{
	VALIDATE();

	if (!mlib_ImageIsNotOneDvector(src1) &&
	    !mlib_ImageIsNotOneDvector(src2) &&
	    !mlib_ImageIsNotOneDvector(dst)) {
		width *= height;
		height = 1;
	}

	mlib_s32 i, j, k;
	mlib_s32 offdst, offsrc1, offsrc2, amount;
	amount = width * channels;
	stride1 -= amount;
	stride2 -= amount;
	strided -= amount;
	if (amount < 32) {	/* too short to use sse2 */
		for (j = 0; j < height; j++) {
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
			sp1 += stride1;
			sp2 += stride2;
			dp += strided;
		}
	} else {	/* sse2 implementation */
		__m128i src1, src2, dst;
		__m128i ones = _mm_set1_epi32(0xffffffff);
		mlib_s32 offsrc1, offsrc2, offdst;

		for (j = 0; j < height; j++) {
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

				/*
				for (i = 0; i < k; i++) {
					src1 = _mm_load_si128((__m128i *)sp1);
					src2 = _mm_load_si128((__m128i *)sp2);
					dst = OPERATION_SSE2(src1, src2);
					_mm_store_si128((__m128i *)dp, dst);
					sp1 += 16;
					sp2 += 16;
					dp += 16;
				}
				*/

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
			} else if (offsrc1 == offdst) {
				k = (amount - offdst) >> 4;
				for (i = 0; i < offdst; i++) {
					*dp = OPERATION(*sp1, *sp2);
					sp1++;
					sp2++;
					dp++;
				}

				/*
				for (i = 0; i < k; i++) {
					src2 = _mm_loadu_si128((__m128i *)sp2);
					src1 = _mm_load_si128((__m128i *)sp1);
					dst = OPERATION_SSE2(src1, src2);
					_mm_store_si128((__m128i *)dp, dst);
					sp1 += 16;
					sp2 += 16;
					dp += 16;
				}
				*/

				int kk = k >> 1;
				__m128i s1, s2, d;

				for (i = 0; i < kk; i++) {
					src2 = _mm_loadu_si128((__m128i *)sp2);
					src1 = _mm_load_si128((__m128i *)sp1);
					s2 = _mm_loadu_si128((__m128i *)(sp2
								+ 16));
					s1 = _mm_load_si128((__m128i *)(sp1
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
					src2 = _mm_loadu_si128((__m128i *)sp2);
					src1 = _mm_load_si128((__m128i *)sp1);
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
			} else if (offsrc2 == offdst) {
				k = (amount - offdst) >> 4;
				for (i = 0; i < offdst; i++) {
					*dp = OPERATION(*sp1, *sp2);
					sp1++;
					sp2++;
					dp++;
				}

				/*
				for (i = 0; i < k; i++) {
					src1 = _mm_loadu_si128((__m128i *)sp1);
					src2 = _mm_load_si128((__m128i *)sp2);
					dst = OPERATION_SSE2(src1, src2);
					_mm_store_si128((__m128i *)dp, dst);
					sp1 += 16;
					sp2 += 16;
					dp += 16;
				}
				*/

				int kk = k >> 1;
				__m128i s1, s2, d;

				for (i = 0; i < kk; i++) {
					src1 = _mm_loadu_si128((__m128i *)sp1);
					src2 = _mm_load_si128((__m128i *)sp2);
					s1 = _mm_loadu_si128((__m128i *)(sp1
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
					src1 = _mm_loadu_si128((__m128i *)sp1);
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
			} else if (offsrc1 == offsrc2) {
				k = (amount - offsrc1) >> 4;
				for (i = 0; i < offsrc1; i++) {
					*dp = OPERATION(*sp1, *sp2);
					sp1++;
					sp2++;
					dp++;
				}

				/*
				for (i = 0; i < k; i++) {
					src1 = _mm_load_si128((__m128i *)sp1);
					src2 = _mm_load_si128((__m128i *)sp2);
					dst = OPERATION_SSE2(src1, src2);
					_mm_storeu_si128((__m128i *)dp, dst);
					sp1 += 16;
					sp2 += 16;
					dp += 16;
				}
				*/

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
					_mm_storeu_si128((__m128i *)dp, dst);
					_mm_storeu_si128((__m128i *)(dp + 16),
								d);
					sp1 += 32;
					sp2 += 32;
					dp += 32;
				}
				if (k & 1) {
					src1 = _mm_load_si128((__m128i *)sp1);
					src2 = _mm_load_si128((__m128i *)sp2);
					dst = OPERATION_SSE2(src1, src2);
					_mm_storeu_si128((__m128i *)dp, dst);
					sp1 += 16;
					sp2 += 16;
					dp += 16;
				}

				for (i = offsrc1 + k * 16; i < amount; i++) {
					*dp = OPERATION(*sp1, *sp2);
					sp1++;
					sp2++;
					dp++;
				}
			} else { /* not aligned */
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
			sp1 += stride1;
			sp2 += stride2;
			dp += strided;
		}
	}

	return (MLIB_SUCCESS);
}

#ifdef __cplusplus
}
#endif

#endif /* _MLIB_S_IMAGELOGIC_H */
