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

#pragma ident	"@(#)mlib_s_ImageYProj.c	9.4	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_ImageYProj - calculates Y Projections for input image
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageYProj(mlib_d64         *yproj,
 *                                  const mlib_image *src)
 *
 * ARGUMENTS
 *      yproj       Pointer to Y - projection vector
 *      src         Pointer to an image
 *
 * RESTRICTION
 *      The image must be a single channel image.
 *      It can be of MLIB_BYTE, MLIB_SHORT, MLIB_USHORT or MLIB_INT data type.
 *
 * DESCRIPTION
 *                 w-1
 *      yproj[j] = SUM (src[i][j]);
 *                 i = 0
 */

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

#pragma weak mlib_ImageYProj = __mlib_ImageYProj

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_ImageYProj) mlib_ImageYProj
    __attribute__((weak, alias("__mlib_ImageYProj")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

static void mlib_s_ImageYProj_U8(
    const mlib_image *src,
    mlib_d64 *yproj);
static void mlib_s_ImageYProj_U16(
    const mlib_image *src,
    mlib_d64 *yproj);
static void mlib_s_ImageYProj_S16(
    const mlib_image *src,
    mlib_d64 *yproj);
static void mlib_s_ImageYProj_S32(
    const mlib_image *src,
    mlib_d64 *yproj);

/* *********************************************************** */

mlib_status
__mlib_ImageYProj(
    mlib_d64 *yproj,
    const mlib_image *src)
{
/* check for obvious errors */
	MLIB_IMAGE_CHECK(src);
	MLIB_IMAGE_HAVE_CHAN(src, 1);

	if (yproj == NULL)
		return (MLIB_NULLPOINTER);

	switch (mlib_ImageGetType(src)) {
/* handle MLIB_BYTE data type of image */
	case MLIB_BYTE:
		mlib_s_ImageYProj_U8(src, yproj);
		break;

/* handle MLIB_USHORT data type of image */
	case MLIB_USHORT:
		mlib_s_ImageYProj_U16(src, yproj);
		break;

/* handle MLIB_SHORT data type of image */
	case MLIB_SHORT:
		mlib_s_ImageYProj_S16(src, yproj);
		break;

/* handle MLIB_INT data type of image */
	case MLIB_INT:
		mlib_s_ImageYProj_S32(src, yproj);
		break;

/* discard any other data types */
	default:
		return (MLIB_FAILURE);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#define	MLIB_S_YPROJ_U8_MAIN				\
	x_d0_u16 = _mm_unpacklo_epi8(x_s0_u8, x_zero);	\
	x_d1_u16 = _mm_unpackhi_epi8(x_s0_u8, x_zero);	\
	x_sum_u16 = _mm_add_epi16(x_d0_u16, x_d1_u16);	\
	x_sum_u32 = _mm_madd_epi16(x_sum_u16, x_one);	\
	x_dsu_u32 = _mm_add_epi32(x_dsu_u32, x_sum_u32)

#define	MLIB_S_YPROJ_U8_TAIL					\
	x_da0_64 = _mm_unpacklo_epi32(x_dsu_u32, x_zero);	\
	x_da1_64 = _mm_unpackhi_epi32(x_dsu_u32, x_zero);	\
	x_sum_64 = _mm_add_epi64(x_da0_64, x_da1_64);		\
	x_da0_128 = _mm_unpacklo_epi64(x_sum_64, x_zero);	\
	x_da1_128 = _mm_unpackhi_epi64(x_sum_64, x_zero);	\
	x_dsu0_64 = _mm_add_epi64(x_da0_128, x_da1_128);	\
	accum = _mm_cvtsi128_si32(x_dsu0_64)

/* *********************************************************** */

void
mlib_s_ImageYProj_U8(
    const mlib_image *src,
    mlib_d64 *yproj)
{
/* width of source image */
	mlib_s32 Width = mlib_ImageGetWidth(src);

/* height of source image */
	mlib_s32 Height = mlib_ImageGetHeight(src);

/* linestride = bytes to next row */
	mlib_s32 YStride = mlib_ImageGetStride(src);

/* pointers to the data of image */
	mlib_u8 *px = (mlib_u8 *)mlib_ImageGetData(src);
	mlib_u8 *spx;

/* number of pixel to process */
	mlib_s32 size = Width;
	mlib_s32 i, j;
	mlib_s32 accum;

	__m128i x_s0_u8, x_d0_u16, x_d1_u16, x_sum_u16;
	__m128i x_sum_u32, x_dsu_u32, x_da0_64, x_da1_64, x_sum_64;
	__m128i x_da0_128, x_da1_128, x_dsu0_64, x_dsu1_64;
	const __m128i x_zero = _mm_setzero_si128();
	const __m128i x_one = _mm_set1_epi16(0x0001);

	spx = px;

	for (j = 0; j < Height; j++) {
		if (0 == ((mlib_addr)px & 15)) {
			x_dsu_u32 = _mm_setzero_si128();
			accum = 0;
			for (i = 0; i < size - 16; i += 16, px += 16) {
				x_s0_u8 = _mm_load_si128((void *)px);
				MLIB_S_YPROJ_U8_MAIN;
			}

			MLIB_S_YPROJ_U8_TAIL;

			for (; i < size; i++, px++) {
				accum += *px;
			}
			yproj[j] = (mlib_d64)accum;

		} else {

			x_dsu_u32 = _mm_setzero_si128();
			accum = 0;
			for (i = 0; i < size - 16; i += 16, px += 16) {
				x_s0_u8 = _mm_loadu_si128((void *)px);
				MLIB_S_YPROJ_U8_MAIN;
			}

			MLIB_S_YPROJ_U8_TAIL;

			for (; i < size; i++, px++) {
				accum += *px;
			}
			yproj[j] = (mlib_d64)accum;
		}

		px = spx = spx + YStride;
	}
}

/* *********************************************************** */

#define	MLIB_S_YPROJ_U16_MAIN					\
	x_d0_u32 = _mm_unpacklo_epi16(x_s0_u16, x_zero);	\
	x_d1_u32 = _mm_unpackhi_epi16(x_s0_u16, x_zero);	\
	x_sum_u32 = _mm_add_epi32(x_d0_u32, x_d1_u32);		\
	x_dsu_u32 = _mm_add_epi32(x_dsu_u32, x_sum_u32)

#define	MLIB_S_YPROJ_U16_TAIL					\
	x_da0_64 = _mm_unpacklo_epi32(x_dsu_u32, x_zero);	\
	x_da1_64 = _mm_unpackhi_epi32(x_dsu_u32, x_zero);	\
	x_sum_64 = _mm_add_epi64(x_da0_64, x_da1_64);		\
	x_da0_128 = _mm_unpacklo_epi64(x_sum_64, x_zero);	\
	x_da1_128 = _mm_unpackhi_epi64(x_sum_64, x_zero);	\
	x_dsu0_64 = _mm_add_epi64(x_da0_128, x_da1_128);	\
	accum = _mm_cvtsi128_si32(x_dsu0_64)

/* *********************************************************** */

void
mlib_s_ImageYProj_U16(
    const mlib_image *src,
    mlib_d64 *yproj)
{
/* width of source image */
	mlib_s32 Width = mlib_ImageGetWidth(src);

/* height of source image */
	mlib_s32 Height = mlib_ImageGetHeight(src);

/* linestride = bytes to next row */
	mlib_s32 YStride = mlib_ImageGetStride(src);

/* pointers to the data of image */
	mlib_u16 *px = (mlib_u16 *)mlib_ImageGetData(src);
	mlib_u16 *spx;

/* number of pixel to process */
	mlib_s32 size = Width;
	mlib_s32 i, j;
	mlib_s32 accum;

	__m128i x_s0_u16, x_d0_u32, x_d1_u32;
	__m128i x_sum_u32, x_dsu_u32, x_da0_64, x_da1_64, x_sum_64;
	__m128i x_da0_128, x_da1_128, x_dsu0_64, x_dsu1_64;
	const __m128i x_zero = _mm_setzero_si128();
	const __m128i x_one = _mm_set1_epi16(0x0001);

	spx = px;

	for (j = 0; j < Height; j++) {
		if (0 == ((mlib_addr)px & 15)) {
			x_dsu_u32 = _mm_setzero_si128();
			accum = 0;
			for (i = 0; i < size - 8; i += 8, px += 8) {
				x_s0_u16 = _mm_load_si128((void *)px);
				MLIB_S_YPROJ_U16_MAIN;
			}

			MLIB_S_YPROJ_U16_TAIL;

			for (; i < size; i++, px++) {
				accum += *px;
			}
			yproj[j] = (mlib_d64)accum;

		} else {

			x_dsu_u32 = _mm_setzero_si128();
			accum = 0;
			for (i = 0; i < size - 8; i += 8, px += 8) {
				x_s0_u16 = _mm_loadu_si128((void *)px);
				MLIB_S_YPROJ_U16_MAIN;
			}

			MLIB_S_YPROJ_U16_TAIL;

			for (; i < size; i++, px++) {
				accum += *px;
			}
			yproj[j] = (mlib_d64)accum;
		}

		px = spx = (mlib_u16 *)((mlib_u8 *)spx + YStride);
	}
}

/* *********************************************************** */

#define	MLIB_S_YPROJ_S16_MAIN				  \
	x_sum_u32 = _mm_madd_epi16(x_src0_u16, x_one);	  \
	x_dsu_u32 = _mm_add_epi32(x_dsu_u32, x_sum_u32)

#define	MLIB_S_YPROJ_S16_TAIL			     	  \
	x_da0_64 = _mm_unpacklo_epi32(x_dsu_u32, x_zero); \
	x_da1_64 = _mm_unpackhi_epi32(x_dsu_u32, x_zero); \
	x_sum_64 = _mm_add_epi64(x_da0_64, x_da1_64);	  \
	x_da0_128 = _mm_unpacklo_epi64(x_sum_64, x_zero); \
	x_da1_128 = _mm_unpackhi_epi64(x_sum_64, x_zero); \
	x_dsu0_64 = _mm_add_epi64(x_da0_128, x_da1_128);  \
	accum = _mm_cvtsi128_si32(x_dsu0_64)

/* *********************************************************** */

void
mlib_s_ImageYProj_S16(
    const mlib_image *src,
    mlib_d64 *yproj)
{
/* width of source image */
	mlib_s32 Width = mlib_ImageGetWidth(src);

/* height of source image */
	mlib_s32 Height = mlib_ImageGetHeight(src);

/* linestride = bytes to next row */
	mlib_s32 YStride = mlib_ImageGetStride(src);

/* pointers to the data of image */
	mlib_s16 *px = (mlib_s16 *)mlib_ImageGetData(src);
	mlib_s16 *spx;

/* number of pixel to process */
	mlib_s32 size = Width;
	mlib_s32 i, j;
	mlib_s32 accum;

	__m128i x_src0_u16, x_da0_u16, x_da1_u16, x_sum_u16;
	__m128i x_sum_u32, x_dsu_u32, x_da0_64, x_da1_64, x_sum_64;
	__m128i x_da0_128, x_da1_128, x_dsu0_64, x_dsu1_64;
	const __m128i x_zero = _mm_setzero_si128();
	const __m128i x_one = _mm_set1_epi16(0x0001);

	spx = px;

	for (j = 0; j < Height; j++) {
		if (0 == ((mlib_addr)px & 15)) {
			x_dsu_u32 = _mm_setzero_si128();
			accum = 0;
			for (i = 0; i < size - 8; i += 8, px += 8) {
				x_src0_u16 = _mm_load_si128((void *)px);
				MLIB_S_YPROJ_S16_MAIN;
			}

			MLIB_S_YPROJ_S16_TAIL;

			for (; i < size; i++, px++) {
				accum += *px;
			}
			yproj[j] = (mlib_d64)accum;

		} else {

			x_dsu_u32 = _mm_setzero_si128();
			accum = 0;
			for (i = 0; i < size - 8; i += 8, px += 8) {
				x_src0_u16 = _mm_loadu_si128((void *)px);
				MLIB_S_YPROJ_S16_MAIN;
			}

			MLIB_S_YPROJ_S16_TAIL;

			for (; i < size; i++, px++) {
				accum += *px;
			}
			yproj[j] = (mlib_d64)accum;
		}

		px = spx = (mlib_s16 *)((mlib_u8 *)spx + YStride);
	}
}

/* *********************************************************** */

#define	MLIB_S_YPROJ_S32_MAIN				 \
	__m128i x_sign = _mm_srai_epi32(x_s0_s32, 31);   \
	x_s0_s64 = _mm_unpacklo_epi32(x_s0_s32, x_sign); \
	x_s1_s64 = _mm_unpackhi_epi32(x_s0_s32, x_sign); \
	x_ds0_s64 = _mm_add_epi64(x_s0_s64, x_s1_s64);   \
	x_ds1_s64 = _mm_add_epi64(x_ds1_s64, x_ds0_s64)

#define	MLIB_S_YPROJ_S32_TAIL				 \
	_mm_storeu_si128((void *)buffer0, x_ds1_s64);    \
	accum += buffer0[0] + buffer0[1]

/* *********************************************************** */

#define	SF	30
#define	SFNUM	(1 << SF)

/* *********************************************************** */

void
mlib_s_ImageYProj_S32(
    const mlib_image *src,
    mlib_d64 *yproj)
{
/* width of source image */
	mlib_s32 Width = mlib_ImageGetWidth(src);

/* height of source image */
	mlib_s32 Height = mlib_ImageGetHeight(src);

/* linestride = bytes to next row */
	mlib_s32 YStride = mlib_ImageGetStride(src);

/* pointers to the data of image */
	mlib_s32 *px = (mlib_s32 *)mlib_ImageGetData(src);
	mlib_s32 *spx;

/* number of pixel to process */
	mlib_s32 size = Width;
	mlib_s32 i, j, m, k;
	mlib_d64 accum, accum1;

	__m128i x_s0_s32, x_s1_s32;
	__m128i x_s0_s64, x_s1_s64;
	__m128i x_ds0_s64, x_ds1_s64;

	spx = px;

	for (j = 0; j < Height; j++) {
		accum = 0.0;
		mlib_s64 buffer0[2];
		for (m = 0; (m < size) &&
		    ((mlib_addr)px & 15); m++, px++) {
			accum += *px;
		}

		for (i = 0; i < ((size - m - 4) >> SF); i++) {
			for (k = 0; k < SFNUM; k++, px += 4) {
				x_s0_s32 = _mm_load_si128((__m128i *)px);
				MLIB_S_YPROJ_S32_MAIN;
			}

			MLIB_S_YPROJ_S32_TAIL;
		}

		for (i = (i << SF) + m; i < size; i++, px++) {
			accum += (mlib_d64)*px;
		}

		yproj[j] = accum;

		px = spx = (mlib_s32 *)((mlib_u8 *)spx + YStride);
	}
}

/* *********************************************************** */
