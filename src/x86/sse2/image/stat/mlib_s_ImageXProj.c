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

#pragma ident	"@(#)mlib_s_ImageXProj.c	9.4	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_ImageXProj - calculates X Projections for input image
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageXProj(mlib_d64         *xproj,
 *                                  const mlib_image *src)
 *
 * ARGUMENTS
 *      xproj       Pointer to X - projection vector
 *      src         Pointer to an image
 *
 * RESTRICTION
 *      The image must be a single channel image.
 *      It can be of MLIB_BYTE, MLIB_SHORT, MLIB_USHORT or MLIB_INT data type.
 *
 * DESCRIPTION
 *                 h-1
 *      xproj[i] = SUM (src[i][j]);
 *                 j = 0
 */

#include <stdlib.h>
#include <mlib_image.h>
#include <mlib_ImageCheck.h>

#ifdef __SUNPRO_C
#include <sunmedia_intrin.h>
#else
#include <emmintrin.h>
#endif

#include <mlib_sse_utils.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageXProj = __mlib_ImageXProj

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_ImageXProj) mlib_ImageXProj
    __attribute__((weak, alias("__mlib_ImageXProj")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

static void mlib_s_ImageXProj_U8(
    const mlib_image *src,
    mlib_d64 *xproj);

static void mlib_s_ImageXProj_U16(
    const mlib_image *src,
    mlib_d64 *xproj);

static void mlib_s_ImageXProj_S16(
    const mlib_image *src,
    mlib_d64 *xproj);

static void mlib_s_ImageXProj_S32(
    const mlib_image *src,
    mlib_d64 *xproj);

/* *********************************************************** */

mlib_status
__mlib_ImageXProj(
    mlib_d64 *xproj,
    const mlib_image *src)
{
/* check for obvious errors */
	MLIB_IMAGE_CHECK(src);
	MLIB_IMAGE_HAVE_CHAN(src, 1);

	if (xproj == NULL)
		return (MLIB_NULLPOINTER);

	switch (mlib_ImageGetType(src)) {
/* handle MLIB_BYTE data type of image */
	case MLIB_BYTE:
		mlib_s_ImageXProj_U8(src, xproj);
		break;

/* handle MLIB_USHORT data type of image */
	case MLIB_USHORT:
		mlib_s_ImageXProj_U16(src, xproj);
		break;

/* handle MLIB_SHORT data type of image */
	case MLIB_SHORT:
		mlib_s_ImageXProj_S16(src, xproj);
		break;

/* handle MLIB_INT data type of image */
	case MLIB_INT:
		mlib_s_ImageXProj_S32(src, xproj);
		break;

/* discard any other data types */
	default:
		return (MLIB_FAILURE);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#define	MLIB_S_XPROJ_U8_TO_16				\
	x_src1 = _mm_unpacklo_epi8(x_src0, x_zero);	\
	x_src2 = _mm_unpackhi_epi8(x_src0, x_zero);	\
	x_sum1_u16 = _mm_add_epi16(x_sum1_u16, x_src1);	\
	x_sum2_u16 = _mm_add_epi16(x_sum2_u16, x_src2)

#define	MLIB_S_XPROJ_U8_TO_32					\
	x_sum1_u32 = _mm_unpacklo_epi16(x_sum1_u16, x_zero);	\
	x_sum2_u32 = _mm_unpackhi_epi16(x_sum1_u16, x_zero);	\
	x_sum3_u32 = _mm_unpacklo_epi16(x_sum2_u16, x_zero);	\
	x_sum4_u32 = _mm_unpackhi_epi16(x_sum2_u16, x_zero);	\
	x_d1 = _mm_add_epi32(x_d1, x_sum1_u32);	                \
	x_d2 = _mm_add_epi32(x_d2, x_sum2_u32);	                \
	x_d3 = _mm_add_epi32(x_d3, x_sum3_u32);	                \
	x_d4 = _mm_add_epi32(x_d4, x_sum4_u32)

#define	MLIB_S_XPROJ_U8_TO_64                                       \
	xproj[i] = _mm_cvtsi128_si32(x_d1);                         \
	xproj[i + 1] = _mm_cvtsi128_si32(_mm_srli_si128(x_d1, 4));  \
	xproj[i + 2] = _mm_cvtsi128_si32(_mm_srli_si128(x_d1, 8));  \
	xproj[i + 3] = _mm_cvtsi128_si32(_mm_srli_si128(x_d1, 12)); \
	xproj[i + 4] = _mm_cvtsi128_si32(x_d2);                     \
	xproj[i + 5] = _mm_cvtsi128_si32(_mm_srli_si128(x_d2, 4));  \
	xproj[i + 6] = _mm_cvtsi128_si32(_mm_srli_si128(x_d2, 8));  \
	xproj[i + 7] = _mm_cvtsi128_si32(_mm_srli_si128(x_d2, 12)); \
	xproj[i + 8] = _mm_cvtsi128_si32(x_d3);                     \
	xproj[i + 9] = _mm_cvtsi128_si32(_mm_srli_si128(x_d3, 4));  \
	xproj[i + 10] = _mm_cvtsi128_si32(_mm_srli_si128(x_d3, 8)); \
	xproj[i + 11] = _mm_cvtsi128_si32(_mm_srli_si128(x_d3, 12));\
	xproj[i + 12] = _mm_cvtsi128_si32(x_d4);                    \
	xproj[i + 13] = _mm_cvtsi128_si32(_mm_srli_si128(x_d4, 4)); \
	xproj[i + 14] = _mm_cvtsi128_si32(_mm_srli_si128(x_d4, 8)); \
	xproj[i + 15] = _mm_cvtsi128_si32(_mm_srli_si128(x_d4, 12))

/* *********************************************************** */

void
mlib_s_ImageXProj_U8(
    const mlib_image *src,
    mlib_d64 *xproj)
{
	mlib_u8 *sp = mlib_ImageGetData(src);
	mlib_u8 *spl;

	mlib_s32 width = mlib_ImageGetWidth(src);
	mlib_s32 height = mlib_ImageGetHeight(src);
	mlib_s32 stride = mlib_ImageGetStride(src);
	mlib_s32 accm;
	mlib_s32 i = 0, j, k;

	__m128i x_src0, x_src1, x_src2;
	__m128i x_sum1_u16, x_sum2_u16;
	__m128i x_sum1_u32, x_sum2_u32, x_sum3_u32, x_sum4_u32;
	__m128i x_d1, x_d2, x_d3, x_d4;
	__m128i x_sum1_s64, x_sum2_s64, x_sum3_s64, x_sum4_s64;
	__m128i x_sum5_s64, x_sum6_s64, x_sum7_s64, x_sum8_s64;

	__m128d x_sum1_d64, x_sum2_d64, x_sum3_d64, x_sum4_d64;
	__m128d x_sum5_d64, x_sum6_d64, x_sum7_d64, x_sum8_d64;

	const __m128i x_zero = _mm_setzero_si128();
	const __m128d x_dzero = _mm_setzero_pd();

	if ((0 == ((mlib_addr)sp & 15)) &&
	    (0 == (stride & ~15))) {
		for (; i < width - 16; i += 16) {
			spl = sp + i;
			x_d1 = _mm_setzero_si128();
			x_d2 = _mm_setzero_si128();
			x_d3 = _mm_setzero_si128();
			x_d4 = _mm_setzero_si128();
			x_sum1_u16 = _mm_setzero_si128();
			x_sum2_u16 = _mm_setzero_si128();

			for (j = 0; j < height >> 7; j++) {
				for (k = 0; k < 128; k++, spl += stride) {
					x_src0 = _mm_load_si128((void *)spl);
					MLIB_S_XPROJ_U8_TO_16;
				}

				MLIB_S_XPROJ_U8_TO_32;
				x_sum1_u16 = _mm_setzero_si128();
				x_sum2_u16 = _mm_setzero_si128();
			}

			for (j <<= 7; j < height; j++, spl += stride) {
				x_src0 = _mm_load_si128((void *)spl);
				MLIB_S_XPROJ_U8_TO_16;
			}

			MLIB_S_XPROJ_U8_TO_32;

			MLIB_S_XPROJ_U8_TO_64;

		}

		for (; i < width; i++) {
			spl = sp + i;
			accm = 0;
			for (j = 0; j < height; j++, spl += stride) {
				accm += *spl;
			}

			xproj[i] = (mlib_d64)accm;
		}

	} else {

		for (; i < width - 16; i += 16) {

			spl = sp + i;
			x_d1 = _mm_setzero_si128();
			x_d2 = _mm_setzero_si128();
			x_d3 = _mm_setzero_si128();
			x_d4 = _mm_setzero_si128();
			x_sum1_u16 = _mm_setzero_si128();
			x_sum2_u16 = _mm_setzero_si128();

			for (j = 0; j < height >> 7; j++) {
				for (k = 0; k < 128; k++, spl += stride) {
					x_src0 = _mm_loadu_si128((void *)spl);
					MLIB_S_XPROJ_U8_TO_16;
				}

				MLIB_S_XPROJ_U8_TO_32;

				x_sum1_u16 = _mm_setzero_si128();
				x_sum2_u16 = _mm_setzero_si128();
			}

			for (j <<= 7; j < height; j++, spl += stride) {
				x_src0 = _mm_loadu_si128((void *)spl);
				MLIB_S_XPROJ_U8_TO_16;
			}

			MLIB_S_XPROJ_U8_TO_32;

			MLIB_S_XPROJ_U8_TO_64;

		}

		for (; i < width; i++) {
			spl = sp + i;
			accm = 0;
			for (j = 0; j < height; j++, spl += stride) {
				accm += *spl;
			}

			xproj[i] = (mlib_d64)accm;
		}
	}
}

/* *********************************************************** */

#define	MLIB_S_XPROJ_U16_TO_32				\
	x_src1 = _mm_unpacklo_epi16(x_src0, x_zero);	\
	x_src2 = _mm_unpackhi_epi16(x_src0, x_zero);	\
	x_s1_u32 = _mm_add_epi32(x_s1_u32, x_src1);	\
	x_s2_u32 = _mm_add_epi32(x_s2_u32, x_src2)

#define	MLIB_S_XPROJ_U16_TO_64                                         \
	xproj[i] = _mm_cvtsi128_si32(x_s1_u32);                        \
	xproj[i + 1] = _mm_cvtsi128_si32(_mm_srli_si128(x_s1_u32, 4)); \
	xproj[i + 2] = _mm_cvtsi128_si32(_mm_srli_si128(x_s1_u32, 8)); \
	xproj[i + 3] = _mm_cvtsi128_si32(_mm_srli_si128(x_s1_u32, 12));\
	xproj[i + 4] = _mm_cvtsi128_si32(x_s2_u32);                    \
	xproj[i + 5] = _mm_cvtsi128_si32(_mm_srli_si128(x_s2_u32, 4)); \
	xproj[i + 6] = _mm_cvtsi128_si32(_mm_srli_si128(x_s2_u32, 8)); \
	xproj[i + 7] = _mm_cvtsi128_si32(_mm_srli_si128(x_s2_u32, 12))

/* *********************************************************** */

void
mlib_s_ImageXProj_U16(
    const mlib_image *src,
    mlib_d64 *xproj)
{
	mlib_u16 *sp = mlib_ImageGetData(src);
	mlib_u16 *spl;

	mlib_s32 width = mlib_ImageGetWidth(src);
	mlib_s32 height = mlib_ImageGetHeight(src);
	mlib_s32 stride = mlib_ImageGetStride(src);
	mlib_s32 accm;
	mlib_s32 i = 0, j, k;

	__m128i x_src0, x_src1, x_src2;
	__m128i x_s1_u32, x_s2_u32;
	__m128i x_sum1_s64, x_sum2_s64, x_sum3_s64, x_sum4_s64;
	__m128d x_sum1_d64, x_sum2_d64, x_sum3_d64, x_sum4_d64;

	const __m128i x_zero = _mm_setzero_si128();

	if ((0 == ((mlib_addr)sp & 15)) &&
	    (0 == (stride & ~15))) {
		for (; i < width - 8; i += 8) {
			spl = sp + i;
			x_s1_u32 = _mm_setzero_si128();
			x_s2_u32 = _mm_setzero_si128();

			for (j = 0; j < height; j++) {
				x_src0 = _mm_load_si128((__m128i *)spl);
				MLIB_S_XPROJ_U16_TO_32;

				spl = (mlib_u16 *)((mlib_u8 *)spl + stride);
			}

			MLIB_S_XPROJ_U16_TO_64;

		}

		for (; i < width; i++) {
			spl = sp + i;
			accm = 0;
			for (j = 0; j < height; j++) {
				accm += *spl;
				spl = (mlib_u16 *)((mlib_u8 *)spl + stride);
			}

			xproj[i] = (mlib_d64)accm;
		}

	} else {

		for (; i < width - 8; i += 8) {
			spl = sp + i;
			x_s1_u32 = _mm_setzero_si128();
			x_s2_u32 = _mm_setzero_si128();

			for (j = 0; j < height; j++) {
				x_src0 = _mm_loadu_si128((__m128i *)spl);
				MLIB_S_XPROJ_U16_TO_32;

				spl = (mlib_u16 *)((mlib_u8 *)spl + stride);
			}

			MLIB_S_XPROJ_U16_TO_64;

		}

		for (; i < width; i++) {
			spl = sp + i;
			accm = 0;
			for (j = 0; j < height; j++) {
				accm += *spl;
				spl = (mlib_u16 *)((mlib_u8 *)spl + stride);
			}

			xproj[i] = (mlib_d64)accm;
		}
	}
}

/* *********************************************************** */

#define	MLIB_S_XPROJ_S16_TO_32				\
	x_src1 = _mm_srai_epi16(x_src0, 15);		\
	x_src2 = _mm_unpacklo_epi16(x_src0, x_src1);	\
	x_src3 = _mm_unpackhi_epi16(x_src0, x_src1);	\
	x_s1_s32 = _mm_add_epi32(x_s1_s32, x_src2);	\
	x_s2_s32 = _mm_add_epi32(x_s2_s32, x_src3)

#define	MLIB_S_XPROJ_S16_TO_64                                         \
	xproj[i] = _mm_cvtsi128_si32(x_s1_s32);                        \
	xproj[i + 1] = _mm_cvtsi128_si32(_mm_srli_si128(x_s1_s32, 4)); \
	xproj[i + 2] = _mm_cvtsi128_si32(_mm_srli_si128(x_s1_s32, 8)); \
	xproj[i + 3] = _mm_cvtsi128_si32(_mm_srli_si128(x_s1_s32, 12));\
	xproj[i + 4] = _mm_cvtsi128_si32(x_s2_s32);                    \
	xproj[i + 5] = _mm_cvtsi128_si32(_mm_srli_si128(x_s2_s32, 4)); \
	xproj[i + 6] = _mm_cvtsi128_si32(_mm_srli_si128(x_s2_s32, 8)); \
	xproj[i + 7] = _mm_cvtsi128_si32(_mm_srli_si128(x_s2_s32, 12))

/* *********************************************************** */

void
mlib_s_ImageXProj_S16(
    const mlib_image *src,
    mlib_d64 *xproj)
{
	mlib_s16 *sp = mlib_ImageGetData(src);
	mlib_s16 *spl;

	mlib_s32 width = mlib_ImageGetWidth(src);
	mlib_s32 height = mlib_ImageGetHeight(src);
	mlib_s32 stride = mlib_ImageGetStride(src);
	mlib_s32 accm;
	mlib_s32 i = 0, j, k;

	__m128i x_src0, x_src1, x_src2, x_src3;
	__m128i x_s1_s32, x_s2_s32;
	__m128i x_sum1_s64, x_sum2_s64, x_sum3_s64, x_sum4_s64;
	__m128d x_sum1_d64, x_sum2_d64, x_sum3_d64, x_sum4_d64;

	const __m128i x_zero = _mm_setzero_si128();

	if ((0 == ((mlib_addr)sp & 15)) &&
	    (0 == (stride & ~15))) {
		for (; i < width - 8; i += 8) {
			spl = sp + i;
			x_s1_s32 = _mm_setzero_si128();
			x_s2_s32 = _mm_setzero_si128();

			for (j = 0; j < height; j++) {
				x_src0 = _mm_load_si128((__m128i *)spl);
				MLIB_S_XPROJ_S16_TO_32;

				spl = (mlib_s16 *)((mlib_u8 *)spl + stride);
			}

			MLIB_S_XPROJ_S16_TO_64;

		}

		for (; i < width; i++) {
			spl = sp + i;
			accm = 0;
			for (j = 0; j < height; j++) {
				accm += *spl;
				spl = (mlib_s16 *)((mlib_u8 *)spl + stride);
			}

			xproj[i] = (mlib_d64)accm;
		}

	} else {

		for (; i < width - 8; i += 8) {
			spl = sp + i;
			x_s1_s32 = _mm_setzero_si128();
			x_s2_s32 = _mm_setzero_si128();

			for (j = 0; j < height; j++) {
				x_src0 = _mm_loadu_si128((__m128i *)spl);
				MLIB_S_XPROJ_S16_TO_32;

				spl = (mlib_s16 *)((mlib_u8 *)spl + stride);
			}

			MLIB_S_XPROJ_S16_TO_64;

		}

		for (; i < width; i++) {
			spl = sp + i;
			accm = 0;
			for (j = 0; j < height; j++) {
				accm += *spl;
				spl = (mlib_s16 *)((mlib_u8 *)spl + stride);
			}

			xproj[i] = (mlib_d64)accm;
		}
	}
}

/* *********************************************************** */

#define	MLIB_S_XPROJ_S32_TO_64                            \
	__m128i x_sign = _mm_srai_epi32(x_src0, 31);      \
	x_src1 = _mm_unpacklo_epi32(x_src0, x_sign);      \
	x_src2 = _mm_unpackhi_epi32(x_src0, x_sign);      \
	x_dsum1_s64 = _mm_add_epi64(x_dsum1_s64, x_src1); \
	x_dsum2_s64 = _mm_add_epi64(x_dsum2_s64, x_src2)

/* *********************************************************** */

void
mlib_s_ImageXProj_S32(
    const mlib_image *src,
    mlib_d64 *xproj)
{
	mlib_s32 *sp = mlib_ImageGetData(src);
	mlib_s32 *spl;

	mlib_s32 width = mlib_ImageGetWidth(src);
	mlib_s32 height = mlib_ImageGetHeight(src);
	mlib_s32 stride = mlib_ImageGetStride(src);
	mlib_d64 accm;
	mlib_s32 i = 0, j, k;

	__m128i x_src0, x_src1, x_src2, x_src3;
	__m128i x_dsum1_s64, x_dsum2_s64;
	__m128d x_tsum1_d64, x_tsum2_d64;
	__m128d x_sum1_d64, x_sum2_d64;
	mlib_s64 buffer0[2], buffer1[2];

	const __m128i x_zero = _mm_setzero_si128();

	if ((0 == ((mlib_addr)sp & 15)) &&
	    (0 == (stride & ~15))) {
		for (; i < width - 4; i += 4) {
			spl = sp + i;
			x_dsum1_s64 = _mm_setzero_si128();
			x_dsum2_s64 = _mm_setzero_si128();

			for (j = 0; j < height >> 25; j++) {

				for (k = 0; k < 33554432; k++) {
					x_src0 = _mm_load_si128((__m128i *)spl);
					MLIB_S_XPROJ_S32_TO_64;
					spl = (mlib_s32 *)
					    ((mlib_u8 *)spl + stride);
				}

				_mm_storeu_si128((void *)buffer0, x_dsum1_s64);
				_mm_storeu_si128((void *)buffer1, x_dsum2_s64);
				xproj[i] = buffer0[0];
				xproj[i + 1] = buffer0[1];
				xproj[i + 2] = buffer1[0];
				xproj[i + 3] = buffer1[1];
				x_dsum1_s64 = _mm_setzero_si128();
				x_dsum2_s64 = _mm_setzero_si128();
			}

			for (j <<= 25; j < height; j++) {
				x_src0 = _mm_load_si128((__m128i *)spl);
				MLIB_S_XPROJ_S32_TO_64;
				spl = (mlib_s32 *)((mlib_u8 *)spl + stride);
			}

			_mm_storeu_si128((void *)buffer0, x_dsum1_s64);
			_mm_storeu_si128((void *)buffer1, x_dsum2_s64);
			xproj[i] = buffer0[0];
			xproj[i + 1] = buffer0[1];
			xproj[i + 2] = buffer1[0];
			xproj[i + 3] = buffer1[1];
		}

		for (; i < width; i++) {
			spl = sp + i;
			accm = 0;
			for (j = 0; j < height; j++) {
				accm += *spl;
				spl = (mlib_s32 *)((mlib_u8 *)spl + stride);
			}

			xproj[i] = (mlib_d64)accm;
		}

	} else {

		for (; i < width - 4; i += 4) {
			spl = sp + i;
			x_dsum1_s64 = _mm_setzero_si128();
			x_dsum2_s64 = _mm_setzero_si128();

			for (j = 0; j < height >> 25; j++) {
				for (k = 0; k < 33554432; k++) {

					x_src0 =
					    _mm_loadu_si128((__m128i *)spl);
					MLIB_S_XPROJ_S32_TO_64;
					spl = (mlib_s32 *)
					    ((mlib_u8 *)spl + stride);
				}

				_mm_storeu_si128((void *)buffer0, x_dsum1_s64);
				_mm_storeu_si128((void *)buffer1, x_dsum2_s64);
				xproj[i] = buffer0[0];
				xproj[i + 1] = buffer0[1];
				xproj[i + 2] = buffer1[0];
				xproj[i + 3] = buffer1[1];
				x_dsum1_s64 = _mm_setzero_si128();
				x_dsum2_s64 = _mm_setzero_si128();
			}

			for (j <<= 25; j < height; j++) {
				x_src0 = _mm_loadu_si128((__m128i *)spl);
				MLIB_S_XPROJ_S32_TO_64;
				spl = (mlib_s32 *)((mlib_u8 *)spl + stride);
			}

			_mm_storeu_si128((void *)buffer0, x_dsum1_s64);
			_mm_storeu_si128((void *)buffer1, x_dsum2_s64);
			xproj[i] = buffer0[0];
			xproj[i + 1] = buffer0[1];
			xproj[i + 2] = buffer1[0];
			xproj[i + 3] = buffer1[1];
		}

		for (; i < width; i++) {
			spl = sp + i;
			accm = 0;
			for (j = 0; j < height; j++) {
				accm += *spl;
				spl = (mlib_s32 *)((mlib_u8 *)spl + stride);
			}

			xproj[i] = (mlib_d64)accm;
		}
	}
}

/* *********************************************************** */
