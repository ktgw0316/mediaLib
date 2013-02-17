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

#pragma ident	"@(#)mlib_s_ImageNot.c	9.5	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_ImageNot   - not an image (SSE2 version)
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
 *      They can be in MLIB_BYTE, MLIB_SHORT, MLIB_USHORT, MLIB_INT or
 *      MLIB_BIT data type.
 *
 * DESCRIPTION
 *      Not an image for each channel:
 *          dst = ~src
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

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageNot = __mlib_ImageNot

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_ImageNot) mlib_ImageNot
    __attribute__((weak, alias("__mlib_ImageNot")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

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
	mlib_s32 width, height, channels;                       \
/*                                                              \
 * for src                                                      \
 */                                                             \
	mlib_s32 stride1;                                       \
/*                                                              \
 * for dst                                                      \
 */                                                             \
	mlib_s32 strided;                                       \
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
	channels = mlib_ImageGetChannels(dst);                  \
	if ((type == MLIB_SHORT) || (type == MLIB_USHORT))      \
	    width *= 2;                                         \
	else if (type == MLIB_INT)                              \
	    width *= 4;                                         \
	if ((width * channels) > stride1 ||                     \
	    (width * channels) > strided)                       \
	    return MLIB_FAILURE

/* *********************************************************** */

mlib_status
__mlib_ImageNot(
    mlib_image *dst,
    const mlib_image *src)
{
	mlib_s32 type;

	MLIB_IMAGE_CHECK(src);
	MLIB_IMAGE_CHECK(dst);

	type = mlib_ImageGetType(dst);

	if ((type != MLIB_BYTE) &&
	    (type != MLIB_SHORT) &&
	    (type != MLIB_USHORT) && (type != MLIB_BIT) && (type != MLIB_INT))
		return (MLIB_FAILURE);

	if (type != MLIB_BIT) {

		VALIDATE();

		if (!mlib_ImageIsNotOneDvector(src) &&
		    !mlib_ImageIsNotOneDvector(dst)) {
			width *= height;
			height = 1;
		}

		mlib_s32 i, j, k;
		mlib_s32 offdst, offsrc;
		mlib_s32 amount;
		amount = width * channels;
		stride1 -= amount;
		strided -= amount;

		if (amount < 32) {
			for (j = 0; j < height; j++) {
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
				sp += stride1;
				dp += strided;
			}
		} else {
			__m128i src1, src2, dst1, dst2, ones;
			for (j = 0; j < height; j++) {
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

				    /* workaround for CR 6590907 */
				    ones = _mm_cmpeq_epi16(ones, ones);

				    for (i = 0; i < kk; i++) {
					src1 = _mm_load_si128((__m128i *)sp);
					src2 = _mm_load_si128((__m128i *)(sp
							+ 16));
					dst1 = _mm_andnot_si128(src1, ones);
					dst2 = _mm_andnot_si128(src2, ones);
					_mm_store_si128((__m128i *)dp, dst1);
					_mm_store_si128((__m128i *)(dp + 16),
							dst2);
					sp += 32;
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

				    /* workaround for CR 6590907 */
				    ones = _mm_cmpeq_epi16(ones, ones);

				    for (i = 0; i < kk; i++) {
					src1 = _mm_load_si128((__m128i *)sp);
					src2 = _mm_load_si128((__m128i *)(sp
							+ 16));
					dst1 = _mm_andnot_si128(src1, ones);
					dst2 = _mm_andnot_si128(src2, ones);
					_mm_storeu_si128((__m128i *)dp, dst1);
					_mm_storeu_si128((__m128i *)(dp + 16),
							dst2);
					sp += 32;
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

				sp += stride1;
				dp += strided;
			}
		}
		return (MLIB_SUCCESS);
	} else {
		return (mlib_ImageNot_Bit(dst, src));
	}
}

/* *********************************************************** */
