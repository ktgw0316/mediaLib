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

#pragma ident   "@(#)mlib_s_ImageRotate90.c	9.6	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_ImageRotate90 - rotate image 90 degrees clockwise
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageRotate90(mlib_image *dst, const mlib_image *src)
 *
 * ARGUMENTS
 *      dst  pointer to an dst image
 *      src  pointer to an src image
 *
 * RESTRICTION
 *      src and dst must be the same type and the same number of channels.
 *      They can have 1, 2, 3 or 4 channels. They can be of MLIB_BIT, MLIB_BYTE,
 *      MLIB_SHORT, MLIB_USHORT or MLIB_INT data type.
 */

#include <mlib_image.h>
#include <mlib_ImageCheck.h>

#ifdef  __SUNPRO_C
#include <sunmedia_intrin.h>
#else
#include <emmintrin.h>
#endif

/* *************************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageRotate90 = __mlib_ImageRotate90

#elif defined(__GNUC__)  /* defined(__SUNPRO_C) */

__typeof__(__mlib_ImageRotate90) mlib_ImageRotate90
    __attribute__((weak, alias("__mlib_ImageRotate90")));

#else /* defined(__SUNPRO_C) */

#error "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

    mlib_status mlib_ImageFlipMainDiag_BIT_1(
    mlib_image *dst,
    const mlib_image *src,
    mlib_s32 x_dir,
    mlib_s32 y_dir);

    void mlib_s_ImageRotate90_U8_1(
    mlib_image *dst,
    const mlib_image *src);
    void mlib_c_ImageRotate90_U8_3(
    mlib_image *dst,
    const mlib_image *src);

    void mlib_s_ImageRotate90_S16_1(
    mlib_image *dst,
    const mlib_image *src);
    void mlib_c_ImageRotate90_S16_3(
    mlib_image *dst,
    const mlib_image *src);

    void mlib_s_ImageRotate90_S32_1(
    mlib_image *dst,
    const mlib_image *src);
    void mlib_s_ImageRotate90_S32_3(
    mlib_image *dst,
    const mlib_image *src);

    void mlib_s_ImageRotate90_D64_1(
    mlib_image *dst,
    const mlib_image *src);
    void mlib_s_ImageRotate90_D64_2(
    mlib_image *dst,
    const mlib_image *src);
    void mlib_s_ImageRotate90_D64_3(
    mlib_image *dst,
    const mlib_image *src);
    void mlib_s_ImageRotate90_D64_4(
    mlib_image *dst,
    const mlib_image *src);

/* *********************************************************** */

mlib_status
__mlib_ImageRotate90(
    mlib_image *dst,
    const mlib_image *src)
{

/*  check for obvious errors  */
	MLIB_IMAGE_CHECK(src);
	MLIB_IMAGE_CHECK(dst);
	MLIB_IMAGE_TYPE_EQUAL(src, dst);
	MLIB_IMAGE_CHAN_EQUAL(src, dst);

	switch (mlib_ImageGetType(src)) {

/*  handle MLIB_BIT data type of image  */
	case MLIB_BIT:
		switch (mlib_ImageGetChannels(src)) {
		case 1:
			return (mlib_ImageFlipMainDiag_BIT_1(dst, src, 0, -1));
		default:
			return (MLIB_FAILURE);
		}
/*  handle MLIB_BYTE data type of image  */
	case MLIB_BYTE:
		switch (mlib_ImageGetChannels(src)) {
		case 1:
			mlib_s_ImageRotate90_U8_1(dst, src);
			break;
		case 2:
			mlib_s_ImageRotate90_S16_1(dst, src);
			break;
		case 3:
			mlib_c_ImageRotate90_U8_3(dst, src);
			break;
		case 4:
			mlib_s_ImageRotate90_S32_1(dst, src);
			break;
		default:
			return (MLIB_FAILURE);
		}
		break;

/*  handle MLIB_SHORT data type of image  */
	case MLIB_SHORT:
	case MLIB_USHORT:
		switch (mlib_ImageGetChannels(src)) {
		case 1:
			mlib_s_ImageRotate90_S16_1(dst, src);
			break;
		case 2:
			mlib_s_ImageRotate90_S32_1(dst, src);
			break;
		case 3:
			mlib_c_ImageRotate90_S16_3(dst, src);
			break;
		case 4:
			mlib_s_ImageRotate90_D64_1(dst, src);
			break;
		default:
			return (MLIB_FAILURE);
		}
		break;

/*  handle MLIB_INT data type of image  */
	case MLIB_INT:
		switch (mlib_ImageGetChannels(src)) {
		case 1:
			mlib_s_ImageRotate90_S32_1(dst, src);
			break;
		case 2:
			mlib_s_ImageRotate90_D64_1(dst, src);
			break;
		case 3:
			mlib_s_ImageRotate90_S32_3(dst, src);
			break;
		case 4:
			mlib_s_ImageRotate90_D64_2(dst, src);
			break;
		default:
			return (MLIB_FAILURE);
		}
		break;

/*  discard any other data types  */
	default:
		return (MLIB_FAILURE);
	}

	return (MLIB_SUCCESS);

}

/* *********************************************************** */

#define	dw	sh
#define	dh	sw

#define	PREPAREVARS(data_type, num_ch)                               \
	const mlib_s32 sstride = mlib_ImageGetStride(src);           \
	const mlib_s32 dstride = mlib_ImageGetStride(dst);           \
	const mlib_s32 ws = mlib_ImageGetWidth(src);                 \
	const mlib_s32 hs = mlib_ImageGetHeight(src);                \
	const mlib_s32 wd = mlib_ImageGetWidth(dst);                 \
	const mlib_s32 hd = mlib_ImageGetHeight(dst);                \
	const mlib_s32 sw = (ws < hd) ? ws : hd;                     \
	const mlib_s32 sh = (hs < wd) ? hs : wd;                     \
	const mlib_s32 dx = (int)ws - (int)hd;                       \
	const mlib_s32 dy = (int)hs - (int)wd;                       \
	const mlib_s32 dxs = ((dx > 0) ? (dx + 1) >> 1 : 0);         \
	const mlib_s32 dyd = ((dx > 0) ? 0 : (-dx) >> 1);            \
	const mlib_s32 dys = ((dy > 0) ? (dy + 1) >> 1 : 0);         \
	const mlib_s32 dxd = ((dy > 0) ? 0 : -(dy >> 1));            \
	const mlib_s32 s_head = 0;                                   \
	const mlib_s32 d_head = 0;                                   \
	const data_type *const sdata =                               \
	    (data_type *) ((mlib_u8 *)mlib_ImageGetData(src) +       \
	    sstride * dys) + dxs * num_ch;                           \
	const data_type *const sdata1 =                              \
	    (data_type *) ((mlib_u8 *)sdata + (sh - 1) * sstride);   \
	data_type *const ddata =                                     \
	    (data_type *) ((mlib_u8 *)mlib_ImageGetData(dst) +       \
	    dstride * dyd) + dxd * num_ch;

/* *********************************************************** */

#define	TRANSPOSE_16X16_1                                            \
	{                                                            \
		__m128i s80, s102, s124, s146, s91, s113, s135, s157;\
		__m128i s12840, s141062, s13951, s151173;            \
		__m128i seven, sodd;                                 \
	                                                             \
	    s80 = _mm_unpacklo_epi8(wi0, wi8);                       \
	    s102 = _mm_unpacklo_epi8(wi2, wi10);                     \
	    s124 = _mm_unpacklo_epi8(wi4, wi12);                     \
	    s146 = _mm_unpacklo_epi8(wi6, wi14);                     \
	    s91 = _mm_unpacklo_epi8(wi1, wi9);                       \
	    s113 = _mm_unpacklo_epi8(wi3, wi11);                     \
	    s135 = _mm_unpacklo_epi8(wi5, wi13);                     \
	    s157 = _mm_unpacklo_epi8(wi7, wi15);                     \
	    s12840 = _mm_unpacklo_epi8(s80, s124);                   \
	    s141062 = _mm_unpacklo_epi8(s102, s146);                 \
	    s13951 = _mm_unpacklo_epi8(s91, s135);                   \
	    s151173 = _mm_unpacklo_epi8(s113, s157);                 \
	    seven = _mm_unpacklo_epi8(s12840, s141062);              \
	    sodd = _mm_unpacklo_epi8(s13951, s151173);               \
	    wo0 = _mm_unpacklo_epi8(seven, sodd);                    \
	    wo1 = _mm_unpackhi_epi8(seven, sodd);                    \
	    seven = _mm_unpackhi_epi8(s12840, s141062);              \
	    sodd = _mm_unpackhi_epi8(s13951, s151173);               \
	    wo2 = _mm_unpacklo_epi8(seven, sodd);                    \
	    wo3 = _mm_unpackhi_epi8(seven, sodd);                    \
	    s12840 = _mm_unpackhi_epi8(s80, s124);                   \
	    s141062 = _mm_unpackhi_epi8(s102, s146);                 \
	    s13951 = _mm_unpackhi_epi8(s91, s135);                   \
	    s151173 = _mm_unpackhi_epi8(s113, s157);                 \
	    seven = _mm_unpacklo_epi8(s12840, s141062);              \
	    sodd = _mm_unpacklo_epi8(s13951, s151173);               \
	    wo4 = _mm_unpacklo_epi8(seven, sodd);                    \
	    wo5 = _mm_unpackhi_epi8(seven, sodd);                    \
	    seven = _mm_unpackhi_epi8(s12840, s141062);              \
	    sodd = _mm_unpackhi_epi8(s13951, s151173);               \
	    wo6 = _mm_unpacklo_epi8(seven, sodd);                    \
	    wo7 = _mm_unpackhi_epi8(seven, sodd);                    \
	    s80 = _mm_unpackhi_epi8(wi0, wi8);                       \
	    s102 = _mm_unpackhi_epi8(wi2, wi10);                     \
	    s124 = _mm_unpackhi_epi8(wi4, wi12);                     \
	    s146 = _mm_unpackhi_epi8(wi6, wi14);                     \
	    s91 = _mm_unpackhi_epi8(wi1, wi9);                       \
	    s113 = _mm_unpackhi_epi8(wi3, wi11);                     \
	    s135 = _mm_unpackhi_epi8(wi5, wi13);                     \
	    s157 = _mm_unpackhi_epi8(wi7, wi15);                     \
	    s12840 = _mm_unpacklo_epi8(s80, s124);                   \
	    s141062 = _mm_unpacklo_epi8(s102, s146);                 \
	    s13951 = _mm_unpacklo_epi8(s91, s135);                   \
	    s151173 = _mm_unpacklo_epi8(s113, s157);                 \
	    seven = _mm_unpacklo_epi8(s12840, s141062);              \
	    sodd = _mm_unpacklo_epi8(s13951, s151173);               \
	    wo8 = _mm_unpacklo_epi8(seven, sodd);                    \
	    wo9 = _mm_unpackhi_epi8(seven, sodd);                    \
	    seven = _mm_unpackhi_epi8(s12840, s141062);              \
	    sodd = _mm_unpackhi_epi8(s13951, s151173);               \
	    wo10 = _mm_unpacklo_epi8(seven, sodd);                   \
	    wo11 = _mm_unpackhi_epi8(seven, sodd);                   \
	    s12840 = _mm_unpackhi_epi8(s80, s124);                   \
	    s141062 = _mm_unpackhi_epi8(s102, s146);                 \
	    s13951 = _mm_unpackhi_epi8(s91, s135);                   \
	    s151173 = _mm_unpackhi_epi8(s113, s157);                 \
	    seven = _mm_unpacklo_epi8(s12840, s141062);              \
	    sodd = _mm_unpacklo_epi8(s13951, s151173);               \
	    wo12 = _mm_unpacklo_epi8(seven, sodd);                   \
	    wo13 = _mm_unpackhi_epi8(seven, sodd);                   \
	    seven = _mm_unpackhi_epi8(s12840, s141062);              \
	    sodd = _mm_unpackhi_epi8(s13951, s151173);               \
	    wo14 = _mm_unpacklo_epi8(seven, sodd);                   \
	    wo15 = _mm_unpackhi_epi8(seven, sodd);                   \
	}

/* *********************************************************** */

#define	TRANSPOSE_8X8_2                                             \
	{                                                           \
		__m128i s04, s26, s15, s37;                         \
		__m128i seven, sodd;                                \
	                                                            \
	    s04 = _mm_unpacklo_epi16(wi0, wi4);                     \
	    s26 = _mm_unpacklo_epi16(wi2, wi6);                     \
	    s15 = _mm_unpacklo_epi16(wi1, wi5);                     \
	    s37 = _mm_unpacklo_epi16(wi3, wi7);                     \
	    seven = _mm_unpacklo_epi16(s04, s26);                   \
	    sodd = _mm_unpacklo_epi16(s15, s37);                    \
	    wo0 = _mm_unpacklo_epi16(seven, sodd);                  \
	    wo1 = _mm_unpackhi_epi16(seven, sodd);                  \
	    seven = _mm_unpackhi_epi16(s04, s26);                   \
	    sodd = _mm_unpackhi_epi16(s15, s37);                    \
	    wo2 = _mm_unpacklo_epi16(seven, sodd);                  \
	    wo3 = _mm_unpackhi_epi16(seven, sodd);                  \
	    s04 = _mm_unpackhi_epi16(wi0, wi4);                     \
	    s26 = _mm_unpackhi_epi16(wi2, wi6);                     \
	    s15 = _mm_unpackhi_epi16(wi1, wi5);                     \
	    s37 = _mm_unpackhi_epi16(wi3, wi7);                     \
	    seven = _mm_unpacklo_epi16(s04, s26);                   \
	    sodd = _mm_unpacklo_epi16(s15, s37);                    \
	    wo4 = _mm_unpacklo_epi16(seven, sodd);                  \
	    wo5 = _mm_unpackhi_epi16(seven, sodd);                  \
	    seven = _mm_unpackhi_epi16(s04, s26);                   \
	    sodd = _mm_unpackhi_epi16(s15, s37);                    \
	    wo6 = _mm_unpacklo_epi16(seven, sodd);                  \
	    wo7 = _mm_unpackhi_epi16(seven, sodd);                  \
	}


/* *********************************************************** */

#define	TRANSPOSE_4X4_1S32                                         \
	{                                                          \
		__m128i seven, sodd;                               \
	                                                           \
	    seven = _mm_unpacklo_epi32(wi0, wi2);                  \
	    sodd = _mm_unpacklo_epi32(wi1, wi3);                   \
	    wo0 = _mm_unpacklo_epi32(seven, sodd);                 \
	    wo1 = _mm_unpackhi_epi32(seven, sodd);                 \
	    seven = _mm_unpackhi_epi32(wi0, wi2);                  \
	    sodd = _mm_unpackhi_epi32(wi1, wi3);                   \
	    wo2 = _mm_unpacklo_epi32(seven, sodd);                 \
	    wo3 = _mm_unpackhi_epi32(seven, sodd);                 \
	}


/* *********************************************************** */

#define	TRANSPOSE_2X2_1D64                                         \
	{                                                          \
	    wo0 = _mm_unpacklo_epi64(wi0, wi1);                    \
	    wo1 = _mm_unpackhi_epi64(wi0, wi1);                    \
	}

/* *********************************************************** */

#define	LOAD_16X16_1U8                                             \
	{                                                          \
	    const mlib_u8 *const psq2 = pssq - 2 * sstride;        \
	    const mlib_u8 *const psq4 = psq2 - 2 * sstride;        \
	    const mlib_u8 *const psq6 = psq4 - 2 * sstride;        \
	    const mlib_u8 *const psq8 = psq6 - 2 * sstride;        \
	    const mlib_u8 *const psq10 = psq8 - 2 * sstride;       \
	    const mlib_u8 *const psq12 = psq10 - 2 * sstride;      \
	    const mlib_u8 *const psq14 = psq12 - 2 * sstride;      \
                                                                   \
	    wi0 = _mm_loadu_si128((__m128i *)(pssq));              \
	    wi1 = _mm_loadu_si128((__m128i *)(pssq - sstride));    \
	    wi2 = _mm_loadu_si128((__m128i *)(psq2));              \
	    wi3 = _mm_loadu_si128((__m128i *)(psq2 - sstride));    \
	    wi4 = _mm_loadu_si128((__m128i *)(psq4));              \
	    wi5 = _mm_loadu_si128((__m128i *)(psq4 - sstride));    \
	    wi6 = _mm_loadu_si128((__m128i *)(psq6));              \
	    wi7 = _mm_loadu_si128((__m128i *)(psq6 - sstride));    \
	    wi8 = _mm_loadu_si128((__m128i *)(psq8));              \
	    wi9 = _mm_loadu_si128((__m128i *)(psq8 - sstride));    \
	    wi10 = _mm_loadu_si128((__m128i *)(psq10));            \
	    wi11 = _mm_loadu_si128((__m128i *)(psq10 - sstride));  \
	    wi12 = _mm_loadu_si128((__m128i *)(psq12));            \
	    wi13 = _mm_loadu_si128((__m128i *)(psq12 - sstride));  \
	    wi14 = _mm_loadu_si128((__m128i *)(psq14));            \
	    wi15 = _mm_loadu_si128((__m128i *)(psq14 - sstride));  \
	    pssq = (mlib_u8 *)((mlib_u8 *)pssq - 16 * sstride);    \
	}

/* *********************************************************** */

#define	LOAD_8X8_1S16                                          \
	{                                                          \
	    const mlib_u16 *const psq2 =                           \
		(mlib_u16 *)((mlib_u8 *)pssq - 2 * sstride);           \
	    const mlib_u16 *const psq4 =                           \
		(mlib_u16 *)((mlib_u8 *)psq2 - 2 * sstride);           \
	    const mlib_u16 *const psq6 =                           \
		(mlib_u16 *)((mlib_u8 *)psq4 - 2 * sstride);           \
                                                               \
	    wi0 = _mm_loadu_si128((__m128i *)(pssq));              \
	    wi1 = _mm_loadu_si128((__m128i *)                      \
				((mlib_u8 *)pssq - sstride));                  \
	    wi2 = _mm_loadu_si128((__m128i *)(psq2));              \
	    wi3 = _mm_loadu_si128((__m128i *)                      \
				((mlib_u8 *)psq2 - sstride));                  \
	    wi4 = _mm_loadu_si128((__m128i *)(psq4));              \
	    wi5 = _mm_loadu_si128((__m128i *)                      \
				((mlib_u8 *)psq4 - sstride));                  \
	    wi6 = _mm_loadu_si128((__m128i *)(psq6));              \
	    wi7 = _mm_loadu_si128((__m128i *)                      \
				((mlib_u8 *)psq6 - sstride));                  \
	    pssq = (mlib_u16 *)((mlib_u8 *)pssq - 8 * sstride);   \
	}

/* *********************************************************** */

#define	LOAD_4X4_1S32                                          \
	{                                                          \
	    const mlib_s32 *const psq2 =                           \
		(mlib_s32 *)((mlib_u8 *)pssq - 2 * sstride);           \
                                                               \
	    wi0 = _mm_loadu_si128((__m128i *)(pssq));              \
	    wi1 = _mm_loadu_si128((__m128i *)                      \
				((mlib_u8 *)pssq - sstride));                  \
	    wi2 = _mm_loadu_si128((__m128i *)(psq2));              \
	    wi3 = _mm_loadu_si128((__m128i *)                      \
				((mlib_u8 *)psq2 - sstride));                  \
		pssq = (mlib_u32 *)((mlib_u8 *)pssq - 4 * sstride);   \
	}


/* *********************************************************** */
#define	LOAD_2X2_1D64                                          \
	{                                                          \
	    wi0 = _mm_loadu_si128((__m128i *)(pssq));              \
	    wi1 = _mm_loadu_si128((__m128i *)                      \
				((mlib_u8 *)pssq - sstride));                  \
		pssq = (mlib_d64 *)((mlib_u8 *)pssq - 2 * sstride);    \
	}

/* *********************************************************** */

#define	STORE_16X16_1U8                                        \
	_mm_storeu_si128((__m128i *)pdsq, wo0);                    \
	_mm_storeu_si128((__m128i *)                               \
		((mlib_u8 *)pdsq + dstride), wo1);                     \
	_mm_storeu_si128((__m128i *)                               \
		((mlib_u8 *)pdsq + 2 * dstride), wo2);                 \
	_mm_storeu_si128((__m128i *)                               \
		((mlib_u8 *)pdsq + 3 * dstride), wo3);                 \
	_mm_storeu_si128((__m128i *)                               \
		((mlib_u8 *)pdsq + 4 * dstride), wo4);                 \
	_mm_storeu_si128((__m128i *)                               \
		((mlib_u8 *)pdsq + 5 * dstride), wo5);                 \
	_mm_storeu_si128((__m128i *)                               \
		((mlib_u8 *)pdsq + 6 * dstride), wo6);                 \
	_mm_storeu_si128((__m128i *)                               \
		((mlib_u8 *)pdsq + 7 * dstride), wo7);                 \
	_mm_storeu_si128((__m128i *)                               \
		((mlib_u8 *)pdsq + 8 * dstride), wo8);                 \
	_mm_storeu_si128((__m128i *)                               \
		((mlib_u8 *)pdsq + 9 * dstride), wo9);                 \
	_mm_storeu_si128((__m128i *)                               \
		((mlib_u8 *)pdsq + 10 * dstride), wo10);               \
	_mm_storeu_si128((__m128i *)                               \
		((mlib_u8 *)pdsq + 11 * dstride), wo11);               \
	_mm_storeu_si128((__m128i *)                               \
		((mlib_u8 *)pdsq + 12 * dstride), wo12);               \
	_mm_storeu_si128((__m128i *)                               \
		((mlib_u8 *)pdsq + 13 * dstride), wo13);               \
	_mm_storeu_si128((__m128i *)                               \
		((mlib_u8 *)pdsq + 14 * dstride), wo14);               \
	_mm_storeu_si128((__m128i *)                               \
		((mlib_u8 *)pdsq + 15 * dstride), wo15);               \
	pdsq += 16;                                                \

/* *********************************************************** */

#define	STORE_8X8_1S16                                         \
	_mm_storeu_si128((__m128i *) pdsq, wo0);                   \
	_mm_storeu_si128((__m128i *)                               \
		((mlib_u8 *)pdsq + dstride), wo1);                     \
	_mm_storeu_si128((__m128i *)                               \
		((mlib_u8 *)pdsq + 2 * dstride), wo2);                 \
	_mm_storeu_si128((__m128i *)                               \
		((mlib_u8 *)pdsq + 3 * dstride), wo3);                 \
	_mm_storeu_si128((__m128i *)                               \
		((mlib_u8 *)pdsq + 4 * dstride), wo4);                 \
	_mm_storeu_si128((__m128i *)                               \
		((mlib_u8 *)pdsq + 5 * dstride), wo5);                 \
	_mm_storeu_si128((__m128i *)                               \
		((mlib_u8 *)pdsq + 6 * dstride), wo6);                 \
	_mm_storeu_si128((__m128i *)                               \
		((mlib_u8 *)pdsq + 7 * dstride), wo7);                 \
	pdsq += 8;                                                 \

/* *********************************************************** */

#define	STORE_4X4_1S32                                         \
	_mm_storeu_si128((__m128i *) pdsq, wo0);                   \
	_mm_storeu_si128((__m128i *)                               \
		((mlib_u8 *)pdsq + dstride), wo1);                     \
	_mm_storeu_si128((__m128i *)                               \
		((mlib_u8 *)pdsq + 2 * dstride), wo2);                 \
	_mm_storeu_si128((__m128i *)                               \
		((mlib_u8 *)pdsq + 3 * dstride), wo3);                 \
	pdsq += 4;                                                 \

/* *********************************************************** */

#define	STORE_2X2_1D64                                         \
	_mm_storeu_si128((__m128i *) pdsq, wo0);                   \
	_mm_storeu_si128((__m128i *)                               \
		((mlib_u8 *)pdsq + dstride), wo1);                     \
	pdsq += 2;                                                 \


/* *********************************************************** */

/*  process strip of single channel image  */
#define	STRIP_SSE2_FP_2(pd, ps, w, h)                             \
	{                                                         \
	    int x, y;                                             \
	    const int _w = (w), _h = (h);                         \
	    const mlib_d64 *const _ps = (mlib_d64 *) (ps);        \
	    mlib_d64 *const _pd = (mlib_d64 *) (pd);              \
	    const mlib_d64 *spix = _ps;                           \
	    mlib_d64 *dpix = _pd;                                 \
	                                                          \
	    for (x = 1; x <= _w; ++x) {                           \
		for (y = 0; y < _h; ++y) {                        \
			_mm_storeu_pd(dpix, _mm_loadu_pd(spix));      \
			spix = (void *)((mlib_u8 *)spix - sstride);   \
			dpix += 2;                                    \
		}                                                 \
		spix = _ps + (x << 1);                            \
		dpix = (void *)((mlib_u8 *)_pd + x * dstride);    \
	    }                                                     \
	}

/* *********************************************************** */

/*  process strip of 2 channels image ; w and h in pixels!  */
#define	STRIP_SSE2_FP_4(pd, ps, w, h)                      \
	{                                                      \
	    int x, y;                                          \
	    const int _w = (w), _h = (h);                      \
	    const mlib_d64 *const _ps = (mlib_d64 *) (ps);     \
	    mlib_d64 *const _pd = (mlib_d64 *) (pd);           \
	    const mlib_d64 *spix = _ps;                        \
	    mlib_d64 *dpix = _pd;                              \
	                                                       \
	    for (x = 1; x <= _w; ++x) {                        \
		for (y = 0; y < _h; ++y) {                     \
	                                                       \
			_mm_storeu_pd(dpix,                        \
				_mm_loadu_pd(spix));                   \
			_mm_storeu_pd(dpix + 2,                    \
				_mm_loadu_pd(spix + 2));               \
			spix = (void *)((mlib_u8 *)spix - sstride);\
			dpix += 4;                                 \
		}                                              \
		spix = _ps + (x << 2);                         \
		dpix = (void *)((mlib_u8 *)_pd + x * dstride); \
	    }                                                  \
	}

/* *********************************************************** */

/*  process strip of 3 channels image ; w and h in pixels!  */
#define	STRIP_SSE2_FP_3(pd, ps, w, h)                       \
	{                                                      \
	    int x, y;                                          \
	    const int _w = (w), _h = (h);                      \
	    const mlib_d64 *const _ps = (mlib_d64 *) (ps);     \
	    mlib_d64 *const _pd = (mlib_d64 *) (pd);           \
	    const mlib_d64 *spix = _ps;                        \
	    mlib_d64 *dpix = _pd;                              \
	                                                       \
	    for (x = 1; x <= _w; ++x) {                        \
		for (y = 0; y < _h; ++y) {                     \
	                                                       \
			_mm_storeu_pd(dpix,                        \
				_mm_loadu_pd(spix));                   \
			_mm_storel_pd(dpix + 2,                    \
				_mm_load1_pd(spix + 2));               \
			spix = (void *)((mlib_u8 *)spix - sstride);\
			dpix += 3;                                 \
		}                                              \
		spix = _ps + ((x << 1) + x);                   \
		dpix = (void *)((mlib_u8 *)_pd + x * dstride); \
	    }                                                  \
	}
/* *********************************************************** */

/*  process strip of single channel image  */
#define	STRIP_1(pd, ps, w, h, data_type)                      \
	{                                                         \
	    int x, y;                                             \
	    const int _w = (w), _h = (h);                         \
	    const data_type *const _ps = (data_type *) (ps);      \
	    data_type *const _pd = (data_type *) (pd);            \
	    const data_type *spix = _ps;                          \
	    data_type *dpix = _pd;                                \
	                                                          \
	    for (x = 1; x <= _w; ++x) {                           \
			for (y = 0; y < _h; ++y) {                        \
				dpix[0] = spix[0];                            \
				spix = (void *)((mlib_u8 *)spix - sstride);   \
				dpix += 1;                                    \
			}                                                 \
			spix = _ps + x;                                   \
			dpix = (void *)((mlib_u8 *)_pd + x * dstride);    \
	    }                                                     \
	}

#define	STRIPH_1(pd, ps, w, h, data_type)                     \
	{                                                         \
	    int x, y;                                             \
	    const int _w = (w), _h = (h);                         \
	    const data_type *const _ps = (data_type *) (ps);      \
	    data_type *const _pd = (data_type *) (pd);            \
	    const data_type *spix = _ps;                          \
	    data_type *dpix = _pd;                                \
	                                                          \
	    for (y = 1; y <= _h; ++y) {                           \
			for (x = 0; x < _w; ++x) {                        \
				dpix[0] = spix[0];                            \
				dpix = (void *)((mlib_u8 *)dpix + dstride);   \
				spix += 1;                                    \
			}                                                 \
			dpix = _pd + y;                                   \
			spix = (void *)((mlib_u8 *)_ps - y * sstride);    \
	    }                                                     \
	}

/* *********************************************************** */

/*  process strip of 2 channels image ; w and h in pixels!  */
#define	STRIP_2(pd, ps, w, h, data_type)                          \
	{                                                         \
	    int x, y;                                             \
	    const int _w = (w), _h = (h);                         \
	    const data_type *const _ps = (data_type *) (ps);      \
	    data_type *const _pd = (data_type *) (pd);            \
	    const data_type *spix = _ps;                          \
	    data_type *dpix = _pd;                                \
	                                                          \
	    for (x = 1; x <= _w; ++x) {                           \
			for (y = 0; y < _h; ++y) {                        \
				data_type c1, c2;                             \
	                                                          \
				c1 = spix[0];                                 \
				c2 = spix[1];                                 \
				dpix[0] = c1;                                 \
				dpix[1] = c2;                                 \
				spix = (void *)((mlib_u8 *)spix - sstride);   \
				dpix += 2;                                    \
			}                                                 \
			spix = _ps + (x << 1);                            \
			dpix = (void *)((mlib_u8 *)_pd + x * dstride);    \
	    }                                                     \
	}

/* *********************************************************** */

#define	STRIPH_2(pd, ps, w, h, data_type)                         \
	{                                                         \
	    int x, y;                                             \
	    const int _w = (w), _h = (h);                         \
	    const data_type *const _ps = (data_type *) (ps);      \
	    data_type *const _pd = (data_type *) (pd);            \
	    const data_type *spix = _ps;                          \
	    data_type *dpix = _pd;                                \
	                                                          \
	    for (y = 1; y <= _h; ++y) {                           \
			for (x = 0; x < _w; ++x) {                        \
				data_type c1, c2;                             \
	                                                          \
				c1 = spix[0];                                 \
				c2 = spix[1];                                 \
				dpix[0] = c1;                                 \
				dpix[1] = c2;                                 \
				dpix = (void *)((mlib_u8 *)dpix + dstride);   \
				spix += 2;                                    \
			}                                                 \
			dpix = _pd + (y << 1);                            \
			spix = (void *)((mlib_u8 *)_ps - y * sstride);    \
	    }                                                     \
	}

/* *********************************************************** */

/*  process strip of 4 channels image ; w and h in pixels!  */
#define	STRIP_4(pd, ps, w, h, data_type)                          \
	{                                                         \
	    int x, y;                                             \
	    const int _w = (w), _h = (h);                         \
	    const data_type *const _ps = (data_type *) (ps);      \
	    data_type *const _pd = (data_type *) (pd);            \
	    const data_type *spix = _ps;                          \
	    data_type *dpix = _pd;                                \
	                                                          \
	    for (x = 1; x <= _w; ++x) {                           \
			for (y = 0; y < _h; ++y) {                        \
				data_type c1, c2, c3, c4;                     \
	                                                          \
				c1 = spix[0];                                 \
				c2 = spix[1];                                 \
				c3 = spix[2];                                 \
				c4 = spix[3];                                 \
				dpix[0] = c1;                                 \
				dpix[1] = c2;                                 \
				dpix[2] = c3;                                 \
				dpix[3] = c4;                                 \
				spix = (void *)((mlib_u8 *)spix - sstride);   \
				dpix += 4;                                    \
			}                                                 \
			spix = _ps + (x << 2);                            \
			dpix = (void *)((mlib_u8 *)_pd + x * dstride);    \
	    }                                                     \
	}

/* *********************************************************** */

#define	STRIPH_4(pd, ps, w, h, data_type)                         \
	{                                                         \
	    int x, y;                                             \
	    const int _w = (w), _h = (h);                         \
	    const data_type *const _ps = (data_type *) (ps);      \
	    data_type *const _pd = (data_type *) (pd);            \
	    const data_type *spix = _ps;                          \
	    data_type *dpix = _pd;                                \
	                                                          \
	    for (y = 1; y <= _h; ++y) {                           \
			for (x = 0; x < _w; ++x) {                        \
				data_type c1, c2, c3, c4;                     \
	                                                          \
				c1 = spix[0];                                 \
				c2 = spix[1];                                 \
				c3 = spix[2];                                 \
				c4 = spix[3];                                 \
				dpix[0] = c1;                                 \
				dpix[1] = c2;                                 \
				dpix[2] = c3;                                 \
				dpix[3] = c4;                                 \
				dpix = (void *)((mlib_u8 *)dpix + dstride);   \
				spix += 4;                                    \
			}                                                 \
			dpix = _pd + (y << 2);                            \
			spix = (void *)((mlib_u8 *)_ps - y * sstride);    \
	    }                                                     \
	}

/* *********************************************************** */

/*  process strip of 3 channels image ; w and h in pixels!  */
#define	STRIP_3(pd, ps, w, h, data_type)                          \
	{                                                         \
	    int x, y;                                             \
	    const int _w = (w), _h = (h);                         \
	    const data_type *const _ps = (data_type *) (ps);      \
	    data_type *const _pd = (data_type *) (pd);            \
	    const data_type *spix = _ps;                          \
	    data_type *dpix = _pd;                                \
	                                                          \
	    for (x = 1; x <= _w; ++x) {                           \
			for (y = 0; y < _h; ++y) {                        \
				data_type c1, c2, c3;                         \
	                                                          \
				c1 = spix[0];                                 \
				c2 = spix[1];                                 \
				c3 = spix[2];                                 \
				dpix[0] = c1;                                 \
				dpix[1] = c2;                                 \
				dpix[2] = c3;                                 \
				spix = (void *)((mlib_u8 *)spix - sstride);   \
				dpix += 3;                                    \
			}                                                 \
			spix = _ps + ((x << 1) + x);                      \
			dpix = (void *)((mlib_u8 *)_pd + x * dstride);    \
	    }                                                     \
	}

/* *********************************************************** */

#define	STRIPH_3(pd, ps, w, h, data_type)                         \
	{                                                         \
	    int x, y;                                             \
	    const int _w = (w), _h = (h);                         \
	    const data_type *const _ps = (data_type *) (ps);      \
	    data_type *const _pd = (data_type *) (pd);            \
	    const data_type *spix = _ps;                          \
	    data_type *dpix = _pd;                                \
	                                                          \
	    for (y = 1; y <= _h; ++y) {                           \
			for (x = 0; x < _w; ++x) {                        \
				data_type c1, c2, c3;                         \
	                                                          \
				c1 = spix[0];                                 \
				c2 = spix[1];                                 \
				c3 = spix[2];                                 \
				dpix[0] = c1;                                 \
				dpix[1] = c2;                                 \
				dpix[2] = c3;                                 \
				dpix = (void *)((mlib_u8 *)dpix + dstride);   \
				spix += 3;                                    \
			}                                                 \
			dpix = _pd + ((y << 1) + y);                      \
			spix = (void *)((mlib_u8 *)_ps - y * sstride);    \
	    }                                                     \
	}


/* *********************************************************** */

#define	BRAK_OPEN(xpix, ypix, data_type, num_ch)                         \
	const int s_tail = (mlib_addr)(sw - s_head) % xpix;              \
	const int d_tail = (mlib_addr)(dw - d_head) % ypix;              \
	const int numhorzsq = (mlib_s32)(sw - s_head - s_tail) / xpix;   \
	const int numvertsq = (mlib_s32)(dw - d_head - d_tail) / ypix;   \
	const data_type *const srcstart =                                \
	    (data_type *) ((mlib_u8 *)sdata + sstride * (sh - 1));       \
	const data_type *const pssq_a =                                  \
	    (data_type *) ((mlib_u8 *)srcstart - d_head * sstride) +     \
	    s_head * num_ch;                                             \
	data_type *const pdsq_a =                                        \
	    (data_type *) ((mlib_u8 *)ddata + s_head * dstride) +        \
	    d_head * num_ch;                                             \
	const data_type *pssq = pssq_a;                                  \
	data_type *pdsq = pdsq_a;                                        \
	int ysq;

/*  body  */

/* *********************************************************** */

#define	BRAK_CLOSE(xpix, ypix, data_type, num_ch)                       \
	if (d_head)                                                     \
	    STRIPH_##num_ch(ddata, srcstart, (sw - s_tail), d_head,     \
		data_type);                                                 \
	if (s_head)                                                     \
	    STRIP_##num_ch(ddata + num_ch * d_head,                     \
		(mlib_u8 *)srcstart - sstride * d_head, s_head,             \
		(dw - d_head - d_tail), data_type);                         \
	if (d_tail)                                                     \
	    STRIPH_##num_ch(ddata + num_ch * (dw - d_tail),             \
		(mlib_u8 *)sdata + sstride * (d_tail - 1), sw, d_tail,      \
		data_type);                                                 \
	if (s_tail)                                                     \
	    STRIP_##num_ch((mlib_u8 *)ddata + dstride * (dh - s_tail),  \
		srcstart + num_ch * (sw - s_tail), s_tail, (dw - d_tail),   \
		data_type);

/* *********************************************************** */

#define	INCREMENT(xpix, ypix, num_ch)                                \
	pssq = pssq_a + ysq * (ypix * num_ch);                       \
	pdsq = (void *)((mlib_u8 *)pdsq_a + ysq * ypix * dstride);   \


/* *********************************************************** */

void
mlib_s_ImageRotate90_U8_1(
    mlib_image *dst,
    const mlib_image *src)
{
	PREPAREVARS(mlib_u8, 1)

	if (sw < 16 || sh < 16) {
		STRIP_1(ddata, sdata1, sw, sh, mlib_u8);
	} else {
		BRAK_OPEN(16, 16, mlib_u8, 1)

		if (numhorzsq < 1) {
			STRIP_1(ddata, srcstart, sw, sh, mlib_u8);
			return;
		}
		for (ysq = 1; ysq <= numhorzsq; ++ysq) {
			int xsq;
			__m128i wi0, wi1, wi2, wi3, wi4, wi5, wi6, wi7;
			__m128i wi8, wi9, wi10, wi11, wi12, wi13, wi14, wi15;
			__m128i wo0, wo1, wo2, wo3, wo4, wo5, wo6, wo7;
			__m128i wo8, wo9, wo10, wo11, wo12, wo13, wo14, wo15;

			LOAD_16X16_1U8;
			for (xsq = 1; xsq < numvertsq; ++xsq) {
				TRANSPOSE_16X16_1;
				STORE_16X16_1U8;
				LOAD_16X16_1U8;
			}
			TRANSPOSE_16X16_1;
			STORE_16X16_1U8;
			INCREMENT(16, 16, 1);
		}

		BRAK_CLOSE(16, 16, mlib_u8, 1)
	}
}

/* *********************************************************** */

void
mlib_s_ImageRotate90_S16_1(
    mlib_image *dst,
    const mlib_image *src)
{
	PREPAREVARS(mlib_u16, 1)
	if (sw < 8 || sh < 8) {
		STRIP_1(ddata, sdata1, sw, sh, mlib_u16);
	} else {
		BRAK_OPEN(8, 8, mlib_u16, 1)

		if (numvertsq < 1 || numhorzsq < 1) {
			STRIP_1(ddata, srcstart, sw, sh, mlib_u16);
			return;
		}
		for (ysq = 1; ysq <= numhorzsq; ++ysq) {
			int xsq;
			__m128i wi0, wi1, wi2, wi3, wi4, wi5, wi6, wi7;
			__m128i wo0, wo1, wo2, wo3, wo4, wo5, wo6, wo7;

			LOAD_8X8_1S16;
			for (xsq = 1; xsq < numvertsq; ++xsq) {
				TRANSPOSE_8X8_2;
				STORE_8X8_1S16;
				LOAD_8X8_1S16;
			}
			TRANSPOSE_8X8_2;
			STORE_8X8_1S16;
			INCREMENT(8, 8, 1);
		}

		BRAK_CLOSE(8, 8, mlib_u16, 1)

	}
}

/* *********************************************************** */

void
mlib_s_ImageRotate90_S32_1(
    mlib_image *dst,
    const mlib_image *src)
{
	PREPAREVARS(mlib_u32, 1)
	if (sw < 4 || sh < 4) {
		STRIP_1(ddata, sdata1, sw, sh, mlib_u32);
	} else {
		BRAK_OPEN(4, 4, mlib_u32, 1)

		if (numvertsq < 1 || numhorzsq < 1) {
			STRIP_1(ddata, sdata1, sw, sh, mlib_u32);
			return;
		}
		for (ysq = 1; ysq <= numhorzsq; ++ysq) {
			int xsq;
			__m128i wi0, wi1, wi2, wi3;
			__m128i wo0, wo1, wo2, wo3;

			LOAD_4X4_1S32;
			for (xsq = 1; xsq < numvertsq; ++xsq) {
				TRANSPOSE_4X4_1S32;
				STORE_4X4_1S32;
				LOAD_4X4_1S32;
			}
			TRANSPOSE_4X4_1S32;
			STORE_4X4_1S32;
			INCREMENT(4, 4, 1);
		}

		BRAK_CLOSE(4, 4, mlib_u32, 1)
	}
}

/* *********************************************************** */

#define	DIRECT_TRANSP_4X4_1D64                                  \
	{                                                       \
	    mlib_d64 a0, a1, a2, a3;                            \
	    mlib_d64 b0, b1, b2, b3;                            \
	    const mlib_d64 *psq2 =                              \
		(mlib_d64 *)((mlib_u8 *)pssq - 2 * sstride);    \
	                                                        \
	    a0 = *pssq;                                         \
	    a1 = *(mlib_d64 *)((mlib_u8 *)pssq - sstride);      \
	    a2 = *psq2;                                         \
	    a3 = *(mlib_d64 *)((mlib_u8 *)psq2 - sstride);      \
	    pssq += 1;                                          \
	    psq2 += 1;                                          \
	    b0 = *pssq;                                         \
	    b1 = *(mlib_d64 *)((mlib_u8 *)pssq - sstride);      \
	    b2 = *psq2;                                         \
	    b3 = *(mlib_d64 *)((mlib_u8 *)psq2 - sstride);      \
	    pssq += 1;                                          \
	    psq2 += 1;                                          \
	    *pdsq = a0;                                         \
	    *(pdsq + 1) = a1;                                   \
	    *(pdsq + 2) = a2;                                   \
	    *(pdsq + 3) = a3;                                   \
	    pdsq = (mlib_d64 *)((mlib_u8 *)pdsq + dstride);     \
	    a0 = *pssq;                                         \
	    a1 = *(mlib_d64 *)((mlib_u8 *)pssq - sstride);      \
	    a2 = *psq2;                                         \
	    a3 = *(mlib_d64 *)((mlib_u8 *)psq2 - sstride);      \
	    pssq += 1;                                          \
	    psq2 += 1;                                          \
	    *pdsq = b0;                                         \
	    *(pdsq + 1) = b1;                                   \
	    *(pdsq + 2) = b2;                                   \
	    *(pdsq + 3) = b3;                                   \
	    pdsq = (mlib_d64 *)((mlib_u8 *)pdsq + dstride);     \
	    b0 = *pssq;                                         \
	    b1 = *(mlib_d64 *)((mlib_u8 *)pssq - sstride);      \
	    b2 = *psq2;                                         \
	    b3 = *(mlib_d64 *)((mlib_u8 *)psq2 - sstride);      \
	    pssq += 1;                                          \
	    *pdsq = a0;                                         \
	    *(pdsq + 1) = a1;                                   \
	    *(pdsq + 2) = a2;                                   \
	    *(pdsq + 3) = a3;                                   \
	    pdsq = (mlib_d64 *)((mlib_u8 *)pdsq + dstride);     \
	    *pdsq = b0;                                         \
	    *(pdsq + 1) = b1;                                   \
	    *(pdsq + 2) = b2;                                   \
	    *(pdsq + 3) = b3;                                   \
	    pdsq = (mlib_d64 *)((mlib_u8 *)pdsq + dstride);     \
	}

/* *********************************************************** */

void
mlib_s_ImageRotate90_D64_1(
    mlib_image *dst,
    const mlib_image *src)
{
	PREPAREVARS(mlib_d64, 1)
	if (sw < 2 || sh < 2) {
		STRIP_1(ddata, sdata1, sw, sh, mlib_d64);
	} else {

		BRAK_OPEN(2, 2, mlib_d64, 1)

		if (numvertsq < 1 || numhorzsq < 1) {
			STRIP_1(ddata, sdata1, sw, sh, mlib_d64);
			return;
		}

		for (ysq = 1; ysq <= numhorzsq; ++ysq) {
			int xsq;
			__m128i wi0, wi1, wo0, wo1;

			LOAD_2X2_1D64;
			for (xsq = 1; xsq < numvertsq; ++xsq) {
				TRANSPOSE_2X2_1D64;
				STORE_2X2_1D64;
				LOAD_2X2_1D64;
			}
			TRANSPOSE_2X2_1D64;
			STORE_2X2_1D64;
			INCREMENT(2, 2, 1);
		}
		BRAK_CLOSE(2, 2, mlib_d64, 1)
	}
}
/* *********************************************************** */

void
mlib_s_ImageRotate90_D64_2(
    mlib_image *dst,
    const mlib_image *src)
{
	PREPAREVARS(mlib_d64, 2)
	    STRIP_SSE2_FP_2(ddata, sdata1, sw, sh);
}

/* *********************************************************** */
void
mlib_s_ImageRotate90_D64_3(
    mlib_image *dst,
    const mlib_image *src)
{
	PREPAREVARS(mlib_d64, 3)

	BRAK_OPEN(2, 2, mlib_d64, 3)
	if (numhorzsq <= 0 || numvertsq <= 0) {
		STRIP_3(ddata, srcstart, sw, sh, mlib_d64);
		return;
	}
	for (ysq = 1; ysq <= numhorzsq; ++ysq) {
		mlib_s32 xsq;

		for (xsq = 0; xsq < numvertsq; ++xsq) {
			__m128i w0, w1, w2, w3, w4, w5;
			mlib_d64 *psq2 = (mlib_d64 *)
				((mlib_u8 *)pssq - sstride);

			w0 = _mm_loadu_si128((__m128i *)pssq);
			w1 = _mm_loadu_si128((__m128i *)(pssq + 2));
			w2 = _mm_loadu_si128((__m128i *)(pssq + 4));
			w3 = _mm_loadu_si128((__m128i *)psq2);
			w4 = _mm_loadu_si128((__m128i *)(psq2 + 2));
			w5 = _mm_loadu_si128((__m128i *)(psq2 + 4));

			_mm_storeu_si128((__m128i *)pdsq, w0);

			_mm_storeu_si128((__m128i *)(pdsq + 2),
				_mm_unpacklo_epi64(w1, w3));

			_mm_storeu_si128((__m128i *)(pdsq + 4),
				_mm_or_si128(_mm_srli_si128(w3, 8),
				_mm_slli_si128(w4, 8)));

			_mm_storeu_si128((__m128i *)
				((mlib_u8 *)pdsq + dstride),
				_mm_or_si128(_mm_srli_si128(w1, 8),
				_mm_slli_si128(w2, 8)));

			_mm_storeu_si128((__m128i *)
				((mlib_d64 *)((mlib_u8 *)
				pdsq + dstride) + 2),
				_mm_unpackhi_epi64(w2, w4));

			_mm_storeu_si128((__m128i *)
				((mlib_d64 *)((mlib_u8 *)
				pdsq + dstride) + 4), w5);


			pssq = (mlib_d64 *)((mlib_u8 *)pssq - 2 * sstride);
			pdsq += 6;
		}
		INCREMENT(2, 2, 3)
	}
	BRAK_CLOSE(2, 2, mlib_d64, 3)

}

/* *********************************************************** */

void
mlib_s_ImageRotate90_D64_4(
    mlib_image *dst,
    const mlib_image *src)
{
	PREPAREVARS(mlib_d64, 4)
	    STRIP_SSE2_FP_4(ddata, sdata1, sw, sh);
}

/* *********************************************************** */
/*  C version for U8 & S16 3 chan */

#undef PREPAREVARS
#undef STRIP_1
#undef STRIPH_1
#undef STRIP_3
#undef STRIPH_3
#undef BRAK_OPEN
#undef BRAK_CLOSE
#undef INCREMENT

#define	dw	sh
#define	dh	sw
#define	PREPAREVARS(DATA_TYPE, num_ch)                                \
	mlib_s32 numc = num_ch;                                       \
	mlib_s32 sstride =                                            \
	mlib_ImageGetStride(src) / sizeof (DATA_TYPE);                \
	    mlib_s32 dstride =                                        \
		mlib_ImageGetStride(dst) / sizeof (DATA_TYPE);            \
	    mlib_s32 ws = mlib_ImageGetWidth(src);                    \
	    mlib_s32 hs = mlib_ImageGetHeight(src);                   \
	    mlib_s32 wd = mlib_ImageGetWidth(dst);                    \
	    mlib_s32 hd = mlib_ImageGetHeight(dst);                   \
	    mlib_s32 sw = (ws < hd) ? ws : hd;                        \
	    mlib_s32 sh = (hs < wd) ? hs : wd;                        \
	    mlib_s32 dx = (mlib_s32)ws - (mlib_s32)hd;                \
	    mlib_s32 dy = (mlib_s32)hs - (mlib_s32)wd;                \
	    mlib_s32 dxs = ((dx > 0) ? (dx + 1) >> 1 : 0);            \
	    mlib_s32 dyd = ((dx > 0) ? 0 : (-dx) >> 1);               \
	    mlib_s32 dys = ((dy > 0) ? (dy + 1) >> 1 : 0);            \
	    mlib_s32 dxd = ((dy > 0) ? 0 : -(dy >> 1));               \
	    DATA_TYPE *sdata =                                        \
		(DATA_TYPE *) mlib_ImageGetData(src) + dxs * numc +       \
	    sstride * dys;                                            \
	    DATA_TYPE *ddata =                                        \
		(DATA_TYPE *) mlib_ImageGetData(dst) + dxd * numc +       \
	    dstride * dyd;

/* *********************************************************** */

#define	STRIP_1(pd, ps, w, h, DATA_TYPE)                        \
	{                                                       \
	    mlib_s32 x, y;                                      \
	    mlib_s32 _w = (w), _h = (h);                        \
	    DATA_TYPE *_ps = (ps);                              \
	    DATA_TYPE *_pd = (pd);                              \
	    DATA_TYPE *spix = _ps;                              \
	    DATA_TYPE *dpix = _pd;                              \
	                                                        \
	    for (x = 1; x <= _w; ++x) {                         \
			for (y = 0; y < _h; ++y) {                      \
				dpix[0] = spix[0];                          \
				spix -= sstride;                            \
				dpix += 1;                                  \
			}                                               \
			spix = _ps + x;                                 \
			dpix = _pd + x * dstride;                       \
	    }                                                   \
	}

/* *********************************************************** */

#define	STRIPH_1(pd, ps, w, h, DATA_TYPE)                       \
	{                                                       \
	    mlib_s32 x, y;                                      \
	    mlib_s32 _w = (w), _h = (h);                        \
	    DATA_TYPE *_ps = (ps);                              \
	    DATA_TYPE *_pd = (pd);                              \
	    DATA_TYPE *spix = _ps;                              \
	    DATA_TYPE *dpix = _pd;                              \
	                                                        \
	    for (y = 1; y <= _h; ++y) {                         \
			for (x = 0; x < _w; ++x) {                      \
				dpix[0] = spix[0];                          \
				dpix += dstride;                            \
				spix += 1;                                  \
			}                                               \
			dpix = _pd + y;                                 \
			spix = _ps - y * sstride;                       \
	    }                                                   \
	}

/* *********************************************************** */

#define	STRIP_3(pd, ps, w, h, DATA_TYPE)                        \
	{                                                       \
	    mlib_s32 x, y;                                      \
	    mlib_s32 _w = (w), _h = (h);                        \
	    DATA_TYPE *_ps = (ps);                              \
	    DATA_TYPE *_pd = (pd);                              \
	    DATA_TYPE *spix = _ps;                              \
	    DATA_TYPE *dpix = _pd;                              \
	                                                        \
	    for (x = 1; x <= _w; ++x) {                         \
			for (y = 0; y < _h; ++y) {                      \
				DATA_TYPE c1, c2, c3;                       \
	                                                        \
				c1 = spix[0];                               \
				c2 = spix[1];                               \
				c3 = spix[2];                               \
				dpix[0] = c1;                               \
				dpix[1] = c2;                               \
				dpix[2] = c3;                               \
				spix -= sstride;                            \
				dpix += 3;                                  \
			}                                               \
			spix = _ps + ((x << 1) + x);                    \
			dpix = _pd + x * dstride;                       \
	    }                                                   \
	}

/* *********************************************************** */

#define	STRIPH_3(pd, ps, w, h, DATA_TYPE)                       \
	{                                                       \
	    mlib_s32 x, y;                                      \
	    mlib_s32 _w = (w), _h = (h);                        \
	    DATA_TYPE *_ps = (ps);                              \
	    DATA_TYPE *_pd = (pd);                              \
	    DATA_TYPE *spix = _ps;                              \
	    DATA_TYPE *dpix = _pd;                              \
	                                                        \
	    for (y = 1; y <= _h; ++y) {                         \
			for (x = 0; x < _w; ++x) {                      \
				DATA_TYPE c1, c2, c3;                       \
	                                                        \
				c1 = spix[0];                               \
				c2 = spix[1];                               \
				c3 = spix[2];                               \
				dpix[0] = c1;                               \
				dpix[1] = c2;                               \
				dpix[2] = c3;                               \
				dpix += dstride;                            \
				spix += 3;                                  \
			}                                               \
			dpix = _pd + ((y << 1) + y);                    \
			spix = _ps - y * sstride;                       \
	    }                                                   \
	}

/* *********************************************************** */

#define	BRAK_OPEN(xpix, ypix, DATA_TYPE, num_ch)                          \
	mlib_s32 s_tail = (mlib_u32)(sw - s_head) % xpix;                 \
	mlib_s32 d_tail = (mlib_u32)(dw - d_head) % ypix;                 \
	mlib_s32 numhorzsq = (mlib_s32)(sw - s_head - s_tail) / xpix;     \
	mlib_s32 numvertsq = (mlib_s32)(dw - d_head - d_tail) / ypix;     \
	DATA_TYPE *srcstart = sdata + sstride * (sh - 1);                 \
	DATA_TYPE *pssq_a =                                               \
	    srcstart + s_head * num_ch - d_head * sstride;                \
	DATA_TYPE *pdsq_a = ddata + d_head * num_ch + s_head * dstride;   \
	DATA_TYPE *pssq = pssq_a;                                         \
	DATA_TYPE *pdsq = pdsq_a;                                         \
	mlib_s32 ysq;

/* *********************************************************** */

#define	BRAK_CLOSE(xpix, ypix, DATA_TYPE, num_ch)                      \
	if (d_head)                                                        \
	    STRIPH_##num_ch(ddata, srcstart, (sw - s_tail), d_head,        \
		DATA_TYPE);                                                    \
	if (s_head)                                                        \
	    STRIP_##num_ch(ddata + num_ch * d_head,                        \
		srcstart - sstride * d_head, s_head, (dw - d_head - d_tail),   \
		DATA_TYPE);                                                    \
	if (d_tail)                                                        \
	    STRIPH_##num_ch(ddata + num_ch * (dw - d_tail),                \
		sdata + sstride * (d_tail - 1), sw, d_tail, DATA_TYPE);        \
	if (s_tail)                                                        \
	    STRIP_##num_ch(ddata + dstride * (dh - s_tail),                \
		srcstart + num_ch * (sw - s_tail), s_tail, (dw - d_tail),      \
		DATA_TYPE);

/* *********************************************************** */

#define	INCREMENT(xpix, ypix, num_ch)                           \
	pssq = pssq_a + ysq * (ypix * num_ch);                  \
	pdsq = pdsq_a + ysq * ypix * dstride;                   \

/* *********************************************************** */

void
mlib_c_ImageRotate90_U8_3(
    mlib_image *dst,
    const mlib_image *src)
{
	PREPAREVARS(mlib_u8, 3);

	if (!((sstride | dstride) & 3)) {
		mlib_s32 s_head = (mlib_addr)sdata & 3;
		mlib_s32 d_head = (mlib_addr)ddata & 3;

		BRAK_OPEN(4, 4, mlib_u8, 3)
		if (numhorzsq <= 0 || numvertsq <= 0) {
			STRIP_3(ddata, srcstart, sw, sh, mlib_u8);
			return;
		}
		for (ysq = 1; ysq <= numhorzsq; ++ysq) {
			mlib_s32 xsq;

			for (xsq = 0; xsq < numvertsq; ++xsq) {
				mlib_u32 w0, w1, w2, w3, w4, w5, w6, w7;
				mlib_u8 *psq2 = pssq - 2 * sstride;

				w0 = *(mlib_u32 *)pssq;
				w1 = *(mlib_u32 *)(pssq - sstride);
				w2 = *(mlib_u32 *)(psq2);
				w3 = *(mlib_u32 *)(psq2 - sstride);

				*(mlib_u32 *)pdsq =
				    (w0 & 0xffffff) | (w1 << 24);
				*(mlib_u32 *)(pdsq + 4) =
				    ((w1 & 0xffff00) >> 8) | (w2 << 16);
				*(mlib_u32 *)(pdsq + 8) =
				    ((w2 & 0xff0000) >> 16) | (w3 << 8);

				w4 = *(mlib_u32 *)(pssq + 4);
				w5 = *(mlib_u32 *)(pssq - sstride + 4);
				w6 = *(mlib_u32 *)(psq2 + 4);
				w7 = *(mlib_u32 *)(psq2 - sstride + 4);

				*(mlib_u32 *)(pdsq + dstride) =
				    w0 >> 24 | ((w4 << 8) & 0xffffff) | (w1 &
				    0xff000000);
				*(mlib_u32 *)(pdsq + dstride + 4) =
				    (w5 & 0xffff) | ((w2 & 0xff000000) >> 8) |
				    (w6 << 24);
				*(mlib_u32 *)(pdsq + dstride + 8) =
				    ((w6 >> 8) & 0xff) | ((w3 >> 16) & 0xff00) |
				    (w7 << 16);

				w0 = *(mlib_u32 *)(pssq + 8);
				w1 = *(mlib_u32 *)(pssq - sstride + 8);
				w2 = *(mlib_u32 *)(psq2 + 8);
				w3 = *(mlib_u32 *)(psq2 - sstride + 8);

				*(mlib_u32 *)(pdsq + 2 * dstride) =
				    (w4 >> 16) | ((w0 & 0xff) << 16) | ((w5 &
				    0xff0000) << 8);
				*(mlib_u32 *)(pdsq + 2 * dstride + 4) =
				    (w5 >> 24) | ((w1 & 0xff) << 8) | (w6 &
				    0xffff0000);
				*(mlib_u32 *)(pdsq + 2 * dstride + 8) =
				    (w2 & 0xff) | ((w7 >> 8) & 0xffff00) | (w3
				    << 24);

				*(mlib_u32 *)(pdsq + 3 * dstride) =
				    (w0 >> 8) | ((w1 & 0xff00) << 16);
				*(mlib_u32 *)(pdsq + 3 * dstride + 4) =
				    (w1 >> 16) | ((w2 & 0xffff00) << 8);
				*(mlib_u32 *)(pdsq + 3 * dstride + 8) =
				    (w2 >> 24) | (w3 & 0xffffff00);

				pssq -= 4 * sstride;
				pdsq += 12;
			}
			INCREMENT(4, 4, 3)
		}
		BRAK_CLOSE(4, 4, mlib_u8, 3)
	} else {
/*  if any stride %4 != 0  */
		mlib_s32 s_head = 0;
		mlib_s32 d_head = 0;

		BRAK_OPEN(2, 2, mlib_u8, 3)
		if (numhorzsq <= 0 || numvertsq <= 0) {
			STRIP_3(ddata, srcstart, sw, sh, mlib_u8);
			return;
		}
		for (ysq = 1; ysq <= numhorzsq; ++ysq) {
			mlib_s32 xsq;

			for (xsq = 0; xsq < numvertsq; ++xsq) {
				mlib_u8 a0, a1, a2, a3, a4, a5;

				a0 = pssq[0];
				a1 = pssq[1];
				a2 = pssq[2];
				a3 = pssq[-sstride];
				a4 = pssq[-sstride + 1];
				a5 = pssq[-sstride + 2];
				pdsq[0] = a0;
				pdsq[1] = a1;
				pdsq[2] = a2;
				pdsq[3] = a3;
				pdsq[4] = a4;
				pdsq[5] = a5;

				a0 = pssq[3];
				a1 = pssq[4];
				a2 = pssq[5];
				a3 = pssq[-sstride + 3];
				a4 = pssq[-sstride + 4];
				a5 = pssq[-sstride + 5];
				pdsq[dstride + 0] = a0;
				pdsq[dstride + 1] = a1;
				pdsq[dstride + 2] = a2;
				pdsq[dstride + 3] = a3;
				pdsq[dstride + 4] = a4;
				pdsq[dstride + 5] = a5;

				pssq -= 2 * sstride;
				pdsq += 6;
			}
			INCREMENT(2, 2, 3)
		}
		BRAK_CLOSE(2, 2, mlib_u8, 3)
	}
}

/* *********************************************************** */

void
mlib_c_ImageRotate90_S16_3(
    mlib_image *dst,
    const mlib_image *src)
{
	PREPAREVARS(mlib_u16, 3)

	if (!((sstride | dstride) & 1)) {
		mlib_s32 s_head = ((mlib_addr)sdata & 2) >> 1;
		mlib_s32 d_head = ((mlib_addr)ddata & 2) >> 1;

		BRAK_OPEN(2, 2, mlib_u16, 3)
		if (numhorzsq <= 0 || numvertsq <= 0) {
			STRIP_3(ddata, srcstart, sw, sh, mlib_u16);
			return;
		}
		for (ysq = 1; ysq <= numhorzsq; ++ysq) {
			mlib_s32 xsq;

			for (xsq = 0; xsq < numvertsq; ++xsq) {
				mlib_u32 w0, w1, w2, w3, w4, w5;
				mlib_u16 *psq2 = pssq - sstride;

				w0 = *(mlib_u32 *)pssq;
				w1 = *(mlib_u32 *)(pssq + 2);
				w2 = *(mlib_u32 *)(pssq + 4);
				w3 = *(mlib_u32 *)psq2;
				w4 = *(mlib_u32 *)(psq2 + 2);
				w5 = *(mlib_u32 *)(psq2 + 4);

				*(mlib_u32 *)pdsq = w0;
				*(mlib_u32 *)(pdsq + 2) =
				    (w1 & 0xffff) | (w3 << 16);
				*(mlib_u32 *)(pdsq + 4) =
				    (w3 >> 16) | (w4 << 16);

				*(mlib_u32 *)(pdsq + dstride) =
					(w1 >> 16) | (w2 << 16);
				*(mlib_u32 *)(pdsq + dstride + 2) =
				    (w2 >> 16) | (w4 & 0xffff0000);
				*(mlib_u32 *)(pdsq + dstride + 4) = w5;

				pssq -= 2 * sstride;
				pdsq += 6;
			}
			INCREMENT(2, 2, 3)
		}
		BRAK_CLOSE(2, 2, mlib_u16, 3)
	} else {
/*  if any stride %2 != 0  */
		mlib_s32 s_head = 0;
		mlib_s32 d_head = 0;

		BRAK_OPEN(2, 2, mlib_u16, 3)
		if (numhorzsq <= 0 || numvertsq <= 0) {
			STRIP_3(ddata, srcstart, sw, sh, mlib_u16);
			return;
		}
		for (ysq = 1; ysq <= numhorzsq; ++ysq) {
			mlib_s32 xsq;

			for (xsq = 0; xsq < numvertsq; ++xsq) {
				mlib_u16 a0, a1, a2, a3, a4, a5;
				mlib_u16 *psq2 = pssq - sstride;

				a0 = pssq[0];
				a1 = pssq[1];
				a2 = pssq[2];
				a3 = psq2[0];
				a4 = psq2[1];
				a5 = psq2[2];
				pdsq[0] = a0;
				pdsq[1] = a1;
				pdsq[2] = a2;
				pdsq[3] = a3;
				pdsq[4] = a4;
				pdsq[5] = a5;

				a0 = pssq[3];
				a1 = pssq[4];
				a2 = pssq[5];
				a3 = psq2[3];
				a4 = psq2[4];
				a5 = psq2[5];
				pdsq[dstride + 0] = a0;
				pdsq[dstride + 1] = a1;
				pdsq[dstride + 2] = a2;
				pdsq[dstride + 3] = a3;
				pdsq[dstride + 4] = a4;
				pdsq[dstride + 5] = a5;

				pssq -= 2 * sstride;
				pdsq += 6;
			}
			INCREMENT(2, 2, 3)
		}
		BRAK_CLOSE(2, 2, mlib_u16, 3)
	}
}
/* *********************************************************** */
/*  SSE2 version for S32 & D64 3 chan */

void
mlib_s_ImageRotate90_S32_3(
    mlib_image *dst,
    const mlib_image *src)
{
	PREPAREVARS(mlib_u32, 3);

	if (!((sstride | dstride) & 3)) {
		mlib_s32 s_head = (mlib_addr)sdata & 3;
		mlib_s32 d_head = (mlib_addr)ddata & 3;

		BRAK_OPEN(4, 4, mlib_u32, 3)
		if (numhorzsq <= 0 || numvertsq <= 0) {
			STRIP_3(ddata, srcstart, sw, sh, mlib_u32);
			return;
		}
		for (ysq = 1; ysq <= numhorzsq; ++ysq) {
			mlib_s32 xsq;

			for (xsq = 0; xsq < numvertsq; ++xsq) {
				__m128i w0, w1, w2, w3, w4, w5, w6, w7;
				mlib_u32 *psq2 = pssq - 2 * sstride;

				w0 = _mm_loadu_si128(
						(__m128i *)pssq);
				w1 = _mm_loadu_si128(
						(__m128i *)(pssq -
						sstride));
				w2 = _mm_loadu_si128(
						(__m128i *)(psq2));
				w3 = _mm_loadu_si128(
						(__m128i *)(psq2 -
						sstride));

				_mm_storeu_si128((__m128i *)pdsq,
					_mm_or_si128(_mm_slli_si128(w1, 12),
					_mm_srli_si128(_mm_slli_si128(
					w0, 4), 4)));
				_mm_storeu_si128((__m128i *)(pdsq + 4),
					_mm_or_si128(_mm_slli_si128(w2, 8),
					_mm_srli_si128(_mm_slli_si128(
					w1, 4), 8)));
				_mm_storeu_si128((__m128i *)(pdsq + 8),
					_mm_or_si128(_mm_slli_si128(w3, 4),
					_mm_srli_si128(_mm_slli_si128(
					w2, 4), 12)));

				w4 = _mm_loadu_si128(
						(__m128i *)(pssq + 4));
				w5 = _mm_loadu_si128(
						(__m128i *)(pssq -
						sstride + 4));
				w6 = _mm_loadu_si128(
						(__m128i *)(psq2 + 4));
				w7 = _mm_loadu_si128(
						(__m128i *)(psq2 -
						sstride + 4));

				_mm_storeu_si128(
					(__m128i *)(pdsq + dstride),
					_mm_or_si128(_mm_srli_si128(w0, 12),
					_mm_or_si128(_mm_srli_si128(
					_mm_slli_si128(w4, 8), 4),
					_mm_slli_si128(_mm_srli_si128(
					w1, 12), 12))));

				_mm_storeu_si128(
					(__m128i *)(pdsq + dstride + 4),
					_mm_or_si128(_mm_slli_si128(w6, 12),
					_mm_or_si128(_mm_srli_si128(
					_mm_slli_si128(w5, 8), 8),
					_mm_slli_si128(_mm_srli_si128(
					w2, 12), 8))));

				_mm_storeu_si128(
					(__m128i *)(pdsq + dstride + 8),
					_mm_or_si128(_mm_slli_si128(w7, 8),
					_mm_or_si128(_mm_srli_si128(
					_mm_slli_si128(w6, 8), 12),
					_mm_slli_si128(_mm_srli_si128(
					w3, 12), 4))));

				w0 = _mm_loadu_si128(
						(__m128i *)(pssq + 8));
				w1 = _mm_loadu_si128(
						(__m128i *)(pssq -
						sstride + 8));
				w2 = _mm_loadu_si128(
						(__m128i *)(psq2 + 8));
				w3 = _mm_loadu_si128(
						(__m128i *)(psq2 -
						sstride + 8));

				_mm_storeu_si128(
					(__m128i *)(pdsq + 2 * dstride),
					_mm_or_si128(_mm_srli_si128(w4, 8),
					_mm_or_si128(_mm_srli_si128(
					_mm_slli_si128(w0, 12), 4),
					_mm_slli_si128(_mm_srli_si128(
					w5, 8), 12))));

				_mm_storeu_si128(
					(__m128i *)(pdsq + 2 * dstride + 4),
					_mm_or_si128(_mm_srli_si128(w5, 12),
					_mm_or_si128(_mm_srli_si128(
					_mm_slli_si128(w1, 12), 8),
					_mm_slli_si128(_mm_srli_si128(
					w6, 8), 8))));

				_mm_storeu_si128(
					(__m128i *)(pdsq + 2 * dstride + 8),
					_mm_or_si128(_mm_slli_si128(w3, 12),
					_mm_or_si128(_mm_srli_si128(
					_mm_slli_si128(w2, 12), 12),
					_mm_slli_si128(_mm_srli_si128(
					w7, 8), 4))));


				_mm_storeu_si128(
					(__m128i *)(pdsq + 3 * dstride),
					_mm_or_si128(_mm_srli_si128(w0, 4),
					_mm_slli_si128(_mm_srli_si128(
					w1, 4), 12)));

				_mm_storeu_si128(
					(__m128i *)(pdsq + 3 * dstride + 4),
					_mm_or_si128(_mm_srli_si128(w1, 8),
					_mm_slli_si128(_mm_srli_si128(
					w2, 4), 8)));

				_mm_storeu_si128(
					(__m128i *)(pdsq + 3 * dstride + 8),
					_mm_or_si128(_mm_srli_si128(w2, 12),
					_mm_slli_si128(_mm_srli_si128(
					w3, 4), 4)));

				pssq -= 4 * sstride;
				pdsq += 12;
			}
			INCREMENT(4, 4, 3)
		}
		BRAK_CLOSE(4, 4, mlib_u32, 3)
	} else {
/*  if any stride %4 != 0  */
		mlib_s32 s_head = 0;
		mlib_s32 d_head = 0;

		BRAK_OPEN(2, 2, mlib_u32, 3)
		if (numhorzsq <= 0 || numvertsq <= 0) {
			STRIP_3(ddata, srcstart, sw, sh, mlib_u32);
			return;
		}
		for (ysq = 1; ysq <= numhorzsq; ++ysq) {
			mlib_s32 xsq;

			for (xsq = 0; xsq < numvertsq; ++xsq) {
				mlib_u32 a0, a1, a2, a3, a4, a5;

				a0 = pssq[0];
				a1 = pssq[1];
				a2 = pssq[2];
				a3 = pssq[-sstride];
				a4 = pssq[-sstride + 1];
				a5 = pssq[-sstride + 2];
				pdsq[0] = a0;
				pdsq[1] = a1;
				pdsq[2] = a2;
				pdsq[3] = a3;
				pdsq[4] = a4;
				pdsq[5] = a5;

				a0 = pssq[3];
				a1 = pssq[4];
				a2 = pssq[5];
				a3 = pssq[-sstride + 3];
				a4 = pssq[-sstride + 4];
				a5 = pssq[-sstride + 5];
				pdsq[dstride + 0] = a0;
				pdsq[dstride + 1] = a1;
				pdsq[dstride + 2] = a2;
				pdsq[dstride + 3] = a3;
				pdsq[dstride + 4] = a4;
				pdsq[dstride + 5] = a5;

				pssq -= 2 * sstride;
				pdsq += 6;
			}
			INCREMENT(2, 2, 3)
		}
		BRAK_CLOSE(2, 2, mlib_u32, 3)
	}
}

/* *********************************************************** */
