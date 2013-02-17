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

#ifndef _MLIB_S_IMAGECONSTLOGIC_H
#define	_MLIB_S_IMAGECONSTLOGIC_H

#pragma ident	"@(#)mlib_s_ImageConstLogic.h	9.5	07/11/05 SMI"

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

#define	VALIDATE()                                                  \
/*                                                                  \
 * pointers for pixel and line of source                            \
 */                                                                 \
	mlib_u8 *sp;                                                \
/*                                                                  \
 * pointers for pixel and line of dst                               \
 */                                                                 \
	mlib_u8 *dp;                                                \
	mlib_s32 width, height, channels, type;			    \
/*                                                                  \
 * for src                                                          \
 */                                                                 \
	mlib_s32 stride1;                                           \
/*                                                                  \
 * for dst                                                          \
 */                                                                 \
	mlib_s32 strided;                                           \
	                                                            \
	MLIB_IMAGE_SIZE_EQUAL(dst, src);                            \
	MLIB_IMAGE_TYPE_EQUAL(dst, src);                            \
	MLIB_IMAGE_CHAN_EQUAL(dst, src);                            \
	dp = (mlib_u8 *)mlib_ImageGetData(dst);                     \
	sp = (mlib_u8 *)mlib_ImageGetData(src);                     \
	height = mlib_ImageGetHeight(dst);                          \
	width = mlib_ImageGetWidth(dst);                            \
	stride1 = mlib_ImageGetStride(src);                         \
	strided = mlib_ImageGetStride(dst);                         \
	channels = mlib_ImageGetChannels(dst);          	    \
	type = mlib_ImageGetType(dst);                              \
	if ((width > stride1) || (width > strided))                 \
	    return MLIB_FAILURE

/* *********************************************************** */

static mlib_status
mlib_s_ImageConstLogic(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_s32 *c)
{
	VALIDATE();
	mlib_u8 *dpp, *spp, cbuf[48], *ptail;
	mlib_u32 c0, c1, c2;
	mlib_s32 i, j, k;
	mlib_s32 amount, nblock, tail;
	__m128i mx0, mx1, mx2, s0, s1, s2;
	__m128i ones = _mm_set1_epi32(0xffffffff);

	if (type == MLIB_BYTE) {
		width *= channels;
		if (channels == 1) {
			c0 = c[0] & 0xff;
			c0 = c0 | (c0 << 8) | (c0 << 16) | (c0 << 24);
			mx0 = _mm_set1_epi32(c0);
		} else if (channels == 2) {
			c0 = (c[0] & 0xff) | ((c[1] & 0xff) << 8);
			c0 = c0 | (c0 << 16);
			mx0 = _mm_set1_epi32(c0);
		} else if (channels == 3) {
			c0 = (c[0] & 0xff) | ((c[1] & 0xff) << 8)
			    | ((c[2] & 0xff) << 16);
			c1 = (c0 << 16) | (c0 >> 8);
			c2 = (c0 << 8) | (c0 >> 16);
			c0 = (c0 << 24) | c0;
			mx0 = _mm_setr_epi32(c0, c1, c2, c0);
			mx1 = _mm_setr_epi32(c1, c2, c0, c1);
			mx2 = _mm_setr_epi32(c2, c0, c1, c2);
		} else { /* channels == 4 */
			c0 = (c[0] & 0xff) | ((c[1] & 0xff) << 8)
			    | ((c[2] & 0xff) << 16) | ((c[3] & 0xff) << 24);
			mx0 = _mm_set1_epi32(c0);
		}
	} else if (type == MLIB_SHORT || type == MLIB_USHORT) {
		width *= 2 * channels;
		if (channels == 1) {
			c0 = c[0] & 0xffff;
			c0 = c0 | (c0 << 16);
			mx0 = _mm_set1_epi32(c0);
		} else if (channels == 2) {
			c0 = (c[0] & 0xffff) | (c[1] << 16);
			mx0 = _mm_set1_epi32(c0);
		} else if (channels == 3) {
			c0 = (c[0] & 0xffff) | (c[1] << 16);
			c1 = (c[2] & 0xffff) | (c0 << 16);
			c2 = (c0 >> 16) | (c1 << 16);
			mx0 = _mm_setr_epi32(c0, c1, c2, c0);
			mx1 = _mm_setr_epi32(c1, c2, c0, c1);
			mx2 = _mm_setr_epi32(c2, c0, c1, c2);
		} else { /* channels == 4 */
			c0 = (c[0] & 0xffff) | (c[1] << 16);
			c1 = (c[2] & 0xffff) | (c[3] << 16);
			mx0 = _mm_setr_epi32(c0, c1, c0, c1);
		}
	} else { /* MLIB_INT */
		width *= 4 * channels;
		if (channels == 1) {
			mx0 = _mm_set1_epi32(c[0]);
		} else if (channels == 2) {
			c0 = c[0];
			c1 = c[1];
			mx0 = _mm_setr_epi32(c0, c1, c0, c1);
		} else if (channels == 3) {
			c0 = c[0];
			c1 = c[1];
			c2 = c[2];
			mx0 = _mm_setr_epi32(c0, c1, c2, c0);
			mx1 = _mm_setr_epi32(c1, c2, c0, c1);
			mx2 = _mm_setr_epi32(c2, c0, c1, c2);
		} else { /* channels == 4 */
			mx0 = _mm_setr_epi32(c[0], c[1], c[2], c[3]);
		}
	}

#ifdef REV_CONST
	if (channels == 3) {
		mx0 = _mm_andnot_si128(mx0, ones);
		mx1 = _mm_andnot_si128(mx1, ones);
		mx2 = _mm_andnot_si128(mx2, ones);
	} else {
		mx0 = _mm_andnot_si128(mx0, ones);
	}
#endif

	if ((width == stride1) && (width == strided)) {
		amount = (height * width);
		height = 1;
	} else
		amount = width;

	if (channels == 3) {
		_mm_storeu_si128((__m128i *)cbuf, mx0);
		_mm_storeu_si128((__m128i *)(cbuf + 16), mx1);
		_mm_storeu_si128((__m128i *)(cbuf + 32), mx2);

		tail = amount % 48;
		nblock = amount / 48;
		for (j = 0; j < height; j++) {
			spp = sp;
			dpp = dp;
			if (((mlib_addr)sp & 15) || ((mlib_addr)dp & 15)) {
			    for (i = 0; i < nblock; i++) {
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
			} else {
			    for (i = 0; i < nblock; i++) {
				s0 = _mm_load_si128((__m128i *)sp);
				s1 = _mm_load_si128((__m128i *)(sp + 16));
				s2 = _mm_load_si128((__m128i *)(sp + 32));

				s0 = CONSTLOGIC_SSE2(mx0, s0);
				s1 = CONSTLOGIC_SSE2(mx1, s1);
				s2 = CONSTLOGIC_SSE2(mx2, s2);

				_mm_store_si128((__m128i *)dp, s0);
				_mm_store_si128((__m128i *)(dp + 16), s1);
				_mm_store_si128((__m128i *)(dp + 32), s2);
				sp += 48;
				dp += 48;
			    }
			}

			ptail = cbuf;
			if (tail > 16) {
				s0 = _mm_loadu_si128((__m128i *)sp);
				s0 = CONSTLOGIC_SSE2(mx0, s0);
				_mm_storeu_si128((__m128i *)dp, s0);
				sp += 16;
				dp += 16;
				ptail += 16;
			}
			if (tail > 32) {
				s1 = _mm_loadu_si128((__m128i *)sp);
				s1 = CONSTLOGIC_SSE2(mx1, s1);
				_mm_storeu_si128((__m128i *)dp, s1);
				sp += 16;
				dp += 16;
				ptail += 16;
			}
			for (i = 0; i < (tail & 15); i++) {
				*dp = CONSTLOGIC(ptail[i], *sp);
				sp++;
				dp++;
			}
			sp = spp + stride1;
			dp = dpp + strided;
		}
	} else {
		_mm_storeu_si128((__m128i *)cbuf, mx0);
		tail = amount & 15;
		nblock = amount >> 4;
		ptail = cbuf;

		for (j = 0; j < height; j++) {
			spp = sp;
			dpp = dp;
			if (((mlib_addr)sp & 15) || ((mlib_addr)dp & 15)) {
			    for (i = 0; i < nblock; i++) {
				s0 = _mm_loadu_si128((__m128i *)sp);
				s0 = CONSTLOGIC_SSE2(mx0, s0);
				_mm_storeu_si128((__m128i *)dp, s0);
				sp += 16;
				dp += 16;
			    }
			} else {
			    for (i = 0; i < nblock; i++) {
				s0 = _mm_load_si128((__m128i *)sp);
				s0 = CONSTLOGIC_SSE2(mx0, s0);
				_mm_store_si128((__m128i *)dp, s0);
				sp += 16;
				dp += 16;
			    }
			}
			for (i = 0; i < (tail & 15); i++) {
				*dp = CONSTLOGIC(ptail[i], *sp);
				sp++;
				dp++;
			}
			sp = spp + stride1;
			dp = dpp + strided;
		}

	}

	return (MLIB_SUCCESS);
}

#ifdef __cplusplus
}
#endif

#endif /* _MLIB_S_IMAGECONSTLOGIC_H */
