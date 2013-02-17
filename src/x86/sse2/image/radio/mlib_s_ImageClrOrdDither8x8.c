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

#pragma ident   "@(#)mlib_s_ImageClrOrdDither8x8.c	9.4	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_ImageColorOrderedDither8x8 - convert a true color image to an
 *                                        indexed color image by ordered
 *                                        dithering
 *
 * SYNOPSIS
 *      mlib_status  mlib_ImageColorOrderedDither8x8(mlib_image       *dst,
 *                                                   const mlib_image *src,
 *                                                   const mlib_s32   *dmask,
 *                                                   mlib_s32         scale,
 *                                                   const void       *colormap)
 *
 * ARGUMENTS
 *      dst       Pointer to destination image.
 *      src       Pointer to source image.
 *      dmask     Pointer to the dither mask, in row major order.
 *      scale     The scaling factor for dmask to convert the input
 *                integer coefficients into floating-point coefficients:
 *                floating-point coefficient = integer coefficient * 2^(-scale)
 *      colormap  Internal data structure for inverse color mapping.
 *
 * DESCRIPTION
 *      Convert a true color image to a pseudo color image with the
 *      method of ordered dithering.
 *
 * RESTRICTIONS
 *      The src can be an MLIB_BYTE or MLIB_SHORT image with 3 or 4 channels.
 *      The dst must be a 1-channel MLIB_BYTE or MLIB_SHORT image.
 *
 *      The colormap must be created by
 *      mlib_ImageColorTrue2IndexInit with colorcube
 *      included (bits != 0) or by mlib_ImageColorDitherInit()
 *      with dimensions != NULL.
 *
 *      0 <= dmask[i][j] < 2^scale;
 *      scale >= 0
 */

#include <mlib_image.h>
#include <mlib_ImageCheck.h>
#include <mlib_ImageColormap.h>
#ifdef  __SUNPRO_C
#include <sunmedia_intrin.h>
#else
#include <emmintrin.h>
#endif

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageColorOrderedDither8x8 = \
	__mlib_ImageColorOrderedDither8x8

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_ImageColorOrderedDither8x8) mlib_ImageColorOrderedDither8x8
    __attribute__((weak, alias("__mlib_ImageColorOrderedDither8x8")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

static mlib_status mlib_s_OrderedDither8x8_3_8_8(
    const mlib_image *src,
    mlib_image *dst,
    const mlib_s32 *dmask,
    mlib_s32 scalef_expon,
    const void *colormap);

static mlib_status mlib_s_OrderedDither8x8_4_8_8(
    const mlib_image *src,
    mlib_image *dst,
    const mlib_s32 *dmask,
    mlib_s32 scalef_expon,
    const void *colormap);

static mlib_status mlib_s_OrderedDither8x8_3_16_8(
    const mlib_image *src,
    mlib_image *dst,
    const mlib_s32 *dmask,
    mlib_s32 scalef_expon,
    const void *colormap);

static mlib_status mlib_s_OrderedDither8x8_4_16_8(
    const mlib_image *src,
    mlib_image *dst,
    const mlib_s32 *dmask,
    mlib_s32 scalef_expon,
    const void *colormap);

static mlib_status mlib_s_OrderedDither8x8_3_8_16(
    const mlib_image *src,
    mlib_image *dst,
    const mlib_s32 *dmask,
    mlib_s32 scalef_expon,
    const void *colormap);

static mlib_status mlib_s_OrderedDither8x8_4_8_16(
    const mlib_image *src,
    mlib_image *dst,
    const mlib_s32 *dmask,
    mlib_s32 scalef_expon,
    const void *colormap);

static mlib_status mlib_s_OrderedDither8x8_3_16_16(
    const mlib_image *src,
    mlib_image *dst,
    const mlib_s32 *dmask,
    mlib_s32 scalef_expon,
    const void *colormap);

static mlib_status mlib_s_OrderedDither8x8_4_16_16(
    const mlib_image *src,
    mlib_image *dst,
    const mlib_s32 *dmask,
    mlib_s32 scalef_expon,
    const void *colormap);

/* *********************************************************** */

mlib_status
__mlib_ImageColorOrderedDither8x8(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_s32 *dmask,
    mlib_s32 scale,
    const void *colormap)
{
	mlib_status stat;
	mlib_s32 src_nch;
	mlib_type src_dtype, dst_dtype;
	mlib_s32 method;

	MLIB_IMAGE_CHECK(src);
	MLIB_IMAGE_CHECK(dst);
	MLIB_IMAGE_SIZE_EQUAL(src, dst);
	MLIB_IMAGE_HAVE_CHAN(dst, 1);
	MLIB_IMAGE_AND_COLORMAP_ARE_COMPAT(src, colormap);

	if (dmask == NULL || colormap == NULL)
		return (MLIB_NULLPOINTER);

	src_dtype = mlib_ImageGetType(src);
	dst_dtype = mlib_ImageGetType(dst);
	src_nch = mlib_ImageGetChannels(src);

	if (scale < 0) {
		stat = MLIB_OUTOFRANGE;
	} else {
		method = mlib_ImageGetMethod(colormap);

		if (method != LUT_COLOR_CUBE_SEARCH) {
			if (method == LUT_COLOR_DIMENSIONS) {
				mlib_s32 dmask1[64], dmask2[64], dmask3[64],
				    *pmask[4];
				mlib_s32 i, j;

				for (j = 0; j < 8; j++) {
					for (i = 0; i < 8; i++) {
						dmask3[8 * (7 - j) + (7 - i)] =
						    dmask2[8 * (7 - j) + i] =
						    dmask1[8 * j + (7 - i)] =
						    dmask[8 * j + i];
					}
				}

				pmask[0] = (void *)dmask;
				pmask[1] = dmask1;
				pmask[2] = dmask2;
				pmask[3] = dmask3;
				return __mlib_ImageColorOrderedDitherMxN(dst,
				    src, (const mlib_s32 **)pmask, 8, 8, scale,
				    colormap);
			}

			return (MLIB_FAILURE);
		} else {
			switch (src_dtype) {
			case MLIB_BYTE:
				switch (dst_dtype) {
				case MLIB_BYTE:
					switch (src_nch) {
					case 3:
					    stat =
						mlib_s_OrderedDither8x8_3_8_8
						(src, dst, dmask, scale,
						colormap);
					    break;
					case 4:
					    stat =
						mlib_s_OrderedDither8x8_4_8_8
						(src, dst, dmask, scale,
						colormap);
					    break;
					default:
					    stat = MLIB_FAILURE;
					    break;
					}

					break;
				case MLIB_SHORT:
/*
 * convert wid from pixels to bytes
 */
					switch (src_nch) {
					case 3:
					    stat =
						mlib_s_OrderedDither8x8_3_8_16
						(src, dst, dmask, scale,
						colormap);
					    break;
					case 4:
					    stat =
						mlib_s_OrderedDither8x8_4_8_16
						(src, dst, dmask, scale,
						colormap);
					    break;
					default:
					    stat = MLIB_FAILURE;
					    break;
					}

					break;
				case MLIB_BIT:
				default:
					stat = MLIB_FAILURE;
					break;
				}

				break;
			case MLIB_SHORT:
/*
 * convert wid from pixels to bytes
 */
				switch (dst_dtype) {
				case MLIB_BYTE:
					switch (src_nch) {
					case 3:
					    stat =
						mlib_s_OrderedDither8x8_3_16_8
						(src, dst, dmask, scale,
						colormap);
					    break;
					case 4:
					    stat =
						mlib_s_OrderedDither8x8_4_16_8
						(src, dst, dmask, scale,
						colormap);
					    break;
					default:
					    stat = MLIB_FAILURE;
					    break;
					}

					break;
				case MLIB_SHORT:
/*
 * convert wid from pixels to bytes
 */
					switch (src_nch) {
					case 3:
					    stat =
						mlib_s_OrderedDither8x8_3_16_16
						(src, dst, dmask, scale,
						colormap);
					    break;
					case 4:
					    stat =
						mlib_s_OrderedDither8x8_4_16_16
						(src, dst, dmask, scale,
						colormap);
					    break;
					default:
					    stat = MLIB_FAILURE;
					    break;
					}

					break;
				case MLIB_BIT:
				default:
					stat = MLIB_FAILURE;
					break;
				}

				break;
			case MLIB_BIT:
			default:
				stat = MLIB_FAILURE;
				break;
			}
		}
	}

	return (stat);
}

/* *********************************************************** */

#define	CALCULATE_STORE_INTO_INTERM_BUF_U8(s, koeff)              \
	tmp = _mm_unpacklo_epi8(s, _s_zero);                          \
/*                                                                \
 * U8 -> S16                                                      \
 */                                                               \
	tmp = _mm_add_epi16(tmp, koeff);                              \
	tmp = _mm_packus_epi16(tmp, _s_zero);                         \
	_mm_storel_epi64((__m128i *)intrm, tmp);                      \
	intrm++

/* *********************************************************** */

#define	CALCULATE_STORE_INTO_INTERM_BUF_S16(s, koeff_0, koeff_1)  \
	tmp0 = _mm_srai_epi32(_mm_unpacklo_epi16(_s_zero, s), 16);    \
	tmp1 = _mm_srai_epi32(_mm_unpackhi_epi16(_s_zero, s), 16);    \
/*                                                                \
 * S16 -> S32                                                     \
 */                                                               \
	tmp0 = _mm_add_epi32(tmp0, koeff_0);                          \
	tmp1 = _mm_add_epi32(tmp1, koeff_1);                          \
	tmp1 = _mm_packs_epi32(tmp0, tmp1);                           \
/*                                                                \
 * S16 -> S32                                                     \
 */                                                               \
	_mm_storeu_si128(intrm, tmp1);                                \
	intrm++

/* *********************************************************** */

mlib_status
mlib_s_OrderedDither8x8_3_8_8(
    const mlib_image *src,
    mlib_image *dst,
    const mlib_s32 *dmask,
    mlib_s32 scalef_expon,
    const void *colormap)
{
/* pointers for pixel and line of source */
	mlib_u8 *adr_src;

/* pointers for pixel and line of destination */
	mlib_u8 *da, *adr_dst;
	__m64 *interm_buf;
	__m64 *intrm;

/* pointer to row in dither array */
	__m128i *d_d;
	__m64 *sa;

/* line length (in pixels) of source image */
	mlib_s32 sll,
/* line length (in pixels) of destination image */
	    dll,
/* src width in pixels */
	    sw,
/* src height in pixels */
	    sh;

/* indices */
	mlib_s32 row, i;

/* dither koeff */
	__m128i dith_0, dith_1, dith_2, dith_3, dith_4, dith_5;
	__m128i d1, d2, d3, d4, d5;
	__m128i tmp;
	__m128i _s_zero = _mm_xor_si128(_s_zero, _s_zero);
	mlib_d64 mult;
	mlib_s32 bits, shift;

/* temp. dither array */
	mlib_s16 dither[8][8];

/* __m128i dither array */

	__m128i dither_si128[8][3];

	sh = mlib_ImageGetHeight(src);
	sw = mlib_ImageGetWidth(src);
	sll = mlib_ImageGetStride(src);
	dll = mlib_ImageGetStride(dst);
	adr_src = (mlib_u8 *)mlib_ImageGetData(src);
	adr_dst = (mlib_u8 *)mlib_ImageGetData(dst);
	bits = mlib_ImageGetBits(colormap);

	shift = (1 << (8 - bits));
	mult = (mlib_d64)shift;
	while (scalef_expon > 30) {
		mult /= (1 << 30);
		scalef_expon -= 30;
	}

	mult /= (1 << scalef_expon);

	for (row = 0; row < 8; row++) {
		for (i = 0; i < 8; i++) {
			dither[row][i] =
			    (shift >> 1) - 1 - (mlib_s32)(dmask[8 * row +
			    i] * mult);
		}
	}

/* load dither koeff. into __m128i  1, 2, 3, 1, 2, 3, 1, 2   etc. */
	for (row = 0; row < 8; row++) {
		dither_si128[row][0] =
			_mm_setr_epi32(
			((dither[row][7] << 16) |
			(dither[row][0] & 0xFFFF)),
			((dither[row][1] << 16) |
			(dither[7 - row][0] & 0xFFFF)),
			((dither[7 - row][1] << 16) |
			(dither[row][6] & 0xFFFF)),
			((dither[row][5] << 16) |
			(dither[row][2] & 0xFFFF)));

		dither_si128[row][1] =
			_mm_setr_epi32(
			((dither[row][3] << 16) |
			(dither[7 - row][2] & 0xFFFF)),
			((dither[7 - row][3] << 16) |
			(dither[row][4] & 0xFFFF)),
			((dither[row][3] << 16) |
			(dither[row][4] & 0xFFFF)),
			((dither[row][5] << 16) |
			(dither[7 - row][4] & 0xFFFF)));

		dither_si128[row][2] =
			_mm_setr_epi32(
			((dither[7 - row][5] << 16) |
			(dither[row][2] & 0xFFFF)),
			((dither[row][1] << 16) |
			(dither[row][6] & 0xFFFF)),
			((dither[row][7] << 16) |
			(dither[7 - row][6] & 0xFFFF)),
			((dither[7 - row][7] << 16) |
			(dither[row][0] & 0xFFFF)));
	}

	interm_buf =
	    (__m64 *)__mlib_malloc((((sw * 3) >> 3) +
	    2) * sizeof (__m64));

	if (interm_buf == NULL)
		return (MLIB_FAILURE);

#ifdef  __SUNPRO_C
#pragma pipeloop(0)
#endif
	for (row = 0; row < sh; row++) {
		intrm = interm_buf;
		sa = (__m64 *)adr_src;
		da = adr_dst;
		d_d = dither_si128[row & 7];
		dith_0 = d_d[0];
		dith_1 = d_d[1];
		dith_2 = d_d[2];
#ifdef  __SUNPRO_C
#pragma pipeloop(0)
#endif
		for (i = 0; i <= sw - 6; i += 8) {
			d1 = _mm_loadl_epi64((__m128i *)sa);
			sa++;
			d2 = _mm_loadl_epi64((__m128i *)sa);
			sa++;
			d3 = _mm_loadl_epi64((__m128i *)sa);
			sa++;
			CALCULATE_STORE_INTO_INTERM_BUF_U8(d1, dith_0);
			CALCULATE_STORE_INTO_INTERM_BUF_U8(d2, dith_1);
			CALCULATE_STORE_INTO_INTERM_BUF_U8(d3, dith_2);
		}

		if (i <= sw - 3) {
			d1 = _mm_loadl_epi64((__m128i *)sa);
			sa++;
			d2 = _mm_loadl_epi64((__m128i *)sa);
			CALCULATE_STORE_INTO_INTERM_BUF_U8(d1, dith_0);
			CALCULATE_STORE_INTO_INTERM_BUF_U8(d2, dith_1);
		} else if (i < sw) {
			d1 = _mm_loadl_epi64((__m128i *)sa);
			CALCULATE_STORE_INTO_INTERM_BUF_U8(d1, dith_0);
		}

		mlib_ImageColorTrue2IndexLine_U8_U8_3(
			(mlib_u8 *)interm_buf, da, sw, colormap);
		adr_src += sll;
		adr_dst += dll;
	}

	__mlib_free(interm_buf);
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_s_OrderedDither8x8_4_8_8(
    const mlib_image *src,
    mlib_image *dst,
    const mlib_s32 *dmask,
    mlib_s32 scalef_expon,
    const void *colormap)
{
/* pointers for pixel and line of source */
	mlib_u8 *adr_src;

/* pointers for pixel and line of destination */
	mlib_u8 *da, *adr_dst;
	__m64 *interm_buf, *intrm;

/* pointer to row in dither array */
	__m128i *d_d;
	__m64 *sa;

/* line length (in pixels) of source image */
	mlib_s32 sll,
/* line length (in pixels) of destination image */
	    dll,
/* src width in pixels */
	    sw,
/* src height in pixels */
	    sh;

/* indices */
	mlib_s32 row, i;
/* dither koeff */
	__m128i dith_0, dith_1, dith_2, dith_3;
	__m128i d1, d2, d3, d4;
	__m128i tmp;
	__m128i _s_zero = _mm_xor_si128(_s_zero, _s_zero);
	mlib_d64 mult;
	mlib_s32 bits, shift;

/* temp. dither array */
	mlib_s16 dither[8][8];

/* __m128i dither array */
	__m128i dither_si128[8][4];

	sh = mlib_ImageGetHeight(src);
	sw = mlib_ImageGetWidth(src);
	sll = mlib_ImageGetStride(src);
	dll = mlib_ImageGetStride(dst);
	adr_src = (mlib_u8 *)mlib_ImageGetData(src);
	adr_dst = (mlib_u8 *)mlib_ImageGetData(dst);
	bits = mlib_ImageGetBits(colormap);

	shift = (1 << (8 - bits));
	mult = (mlib_d64)shift;
	while (scalef_expon > 30) {
		mult /= (1 << 30);
		scalef_expon -= 30;
	}

	mult /= (1 << scalef_expon);

	for (row = 0; row < 8; row++) {
		for (i = 0; i < 8; i++) {
			dither[row][i] =
				(shift >> 1) - 1 - (mlib_s32)(dmask[8 * row +
			    i] * mult);
		}
	}

/* load dither koeff. into __m128i  1, 2, 3, 4 */
	for (row = 0; row < 8; row++) {
		for (i = 0; i < 8; i += 2) {
			dither_si128[row][i >> 1] =
				_mm_setr_epi32(
				((dither[row][7 - i] << 16) |
				(dither[row][i] & 0xFFFF)),
				((dither[7 - row][7 - i] << 16) |
				(dither[7 - row][i] & 0xFFFF)),
				((dither[row][6 - i] << 16) |
				(dither[row][i + 1] & 0xFFFF)),
				((dither[7 - row][6 - i] << 16) |
				(dither[7 - row][i + 1] & 0xFFFF)));
		}
	}

	interm_buf =
	    (__m64 *)__mlib_malloc(((sw + 1) << 1) * sizeof (__m64));

	if (interm_buf == NULL)
		return (MLIB_FAILURE);

#ifdef  __SUNPRO_C
#pragma pipeloop(0)
#endif
	for (row = 0; row < sh; row++) {
		intrm = interm_buf;
		sa = (__m64 *)adr_src;
		da = adr_dst;
		d_d = dither_si128[row & 7];
		dith_0 = d_d[0];
		dith_1 = d_d[1];
		dith_2 = d_d[2];
		dith_3 = d_d[3];

#ifdef  __SUNPRO_C
#pragma pipeloop(0)
#endif
		for (i = 0; i <= sw - 7; i += 8) {
			d1 = _mm_loadl_epi64((__m128i *)sa);
			sa++;
			d2 = _mm_loadl_epi64((__m128i *)sa);
			sa++;
			d3 = _mm_loadl_epi64((__m128i *)sa);
			sa++;
			d4 = _mm_loadl_epi64((__m128i *)sa);
			sa++;
			CALCULATE_STORE_INTO_INTERM_BUF_U8(d1, dith_0);
			CALCULATE_STORE_INTO_INTERM_BUF_U8(d2, dith_1);
			CALCULATE_STORE_INTO_INTERM_BUF_U8(d3, dith_2);
			CALCULATE_STORE_INTO_INTERM_BUF_U8(d4, dith_3);
		}

		if (i <= sw - 3) {
			d1 = _mm_loadl_epi64((__m128i *)sa);
			sa++;
			CALCULATE_STORE_INTO_INTERM_BUF_U8(d1, dith_0);
			d2 = _mm_loadl_epi64((__m128i *)sa);
			sa++;
			CALCULATE_STORE_INTO_INTERM_BUF_U8(d2, dith_1);
			i += 4;
			dith_0 = dith_2;
		}

		if (i < sw) {
			d3 = _mm_loadl_epi64((__m128i *)sa);
			CALCULATE_STORE_INTO_INTERM_BUF_U8(d3, dith_0);
		}

		mlib_ImageColorTrue2IndexLine_U8_U8_4(
			(mlib_u8 *)interm_buf, da, sw, colormap);
		adr_src += sll;
		adr_dst += dll;
	}

	__mlib_free(interm_buf);
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_s_OrderedDither8x8_3_16_8(
    const mlib_image *src,
    mlib_image *dst,
    const mlib_s32 *dmask,
    mlib_s32 scalef_expon,
    const void *colormap)
{
/* pointers for pixel and line of source */
	mlib_s16 *adr_src;

/* pointers for pixel and line of destination */
	mlib_u8 *da, *adr_dst;
	__m128i *interm_buf, *intrm;

/* pointer to row in dither array */
	__m128i *d_d, *sa;

/* line length (in pixels) of source image */
	mlib_s32 sll,
/* line length (in pixels) of destination image */
	    dll,
/* src width in pixels */
	    sw,
/* src height in pixels */
	    sh;

/* indices */
	mlib_s32 row, i;
	__m128i dith_0, dith_1, dith_2, dith_3, dith_4, dith_5;
/* dither koeff */

/* dither koeff */
	__m128i d1, d2, d3;
	__m128i tmp0, tmp1;
	__m128i _s_zero = _mm_xor_si128(_s_zero, _s_zero);
	mlib_d64 mult;
	mlib_s32 bits, shift;

/* temp. dither array */
	mlib_s32 dither[8][8];

/* __m128i dither array */
	__m128i dither_si128[8][6];

	sh = mlib_ImageGetHeight(src);
	sw = mlib_ImageGetWidth(src);
	sll = mlib_ImageGetStride(src) >> 1;
	dll = mlib_ImageGetStride(dst);
	adr_src = (mlib_s16 *)mlib_ImageGetData(src);
	adr_dst = (mlib_u8 *)mlib_ImageGetData(dst);
	bits = mlib_ImageGetBits(colormap);

	shift = (1 << (16 - bits));
	mult = (mlib_d64)shift;
	while (scalef_expon > 30) {
		mult /= (1 << 30);
		scalef_expon -= 30;
	}

	mult /= (1 << scalef_expon);

	for (row = 0; row < 8; row++) {
		for (i = 0; i < 8; i++) {
			dither[row][i] =
				(shift >> 1) - 1 - (mlib_s32)(dmask[8 * row +
				i] * mult);
		}
	}

/* load dither koeff. into __m128i  1, 2, 3 */
	for (row = 0; row < 8; row++) {
		dither_si128[row][0] =
			_mm_setr_epi32(
			dither[row][0], dither[row][7],
			dither[7 - row][0], dither[row][1]);

		dither_si128[row][1] =
			_mm_setr_epi32(
			dither[row][6], dither[7 - row][1],
			dither[row][2], dither[row][5]);

		dither_si128[row][2] =
			_mm_setr_epi32(
			dither[7 - row][2], dither[row][3],
			dither[row][4], dither[7 - row][3]);

		dither_si128[row][3] =
			_mm_setr_epi32(
			dither[row][4], dither[row][3],
			dither[7 - row][4], dither[row][5]);

		dither_si128[row][4] =
			_mm_setr_epi32(
			dither[row][2], dither[7 - row][5],
			dither[row][6], dither[row][1]);

		dither_si128[row][5] =
			_mm_setr_epi32(
			dither[7 - row][6], dither[row][7],
			dither[row][0], dither[7 - row][7]);
	}

	interm_buf =
	    (__m128i *)__mlib_malloc((((sw * 3) << 2) +
	    2) * sizeof (__m128i));

	if (interm_buf == NULL)
		return (MLIB_FAILURE);

#ifdef  __SUNPRO_C
#pragma pipeloop(0)
#endif
	for (row = 0; row < sh; row++) {
		intrm = interm_buf;
		sa = (__m128i *)adr_src;
		da = adr_dst;
		d_d = dither_si128[row & 7];
		dith_0 = d_d[0];
		dith_1 = d_d[1];
		dith_2 = d_d[2];
		dith_3 = d_d[3];
		dith_4 = d_d[4];
		dith_5 = d_d[5];

#ifdef  __SUNPRO_C
#pragma pipeloop(0)
#endif
		for (i = 0; i <= sw - 6; i += 8) {
			d1 = _mm_loadu_si128(sa);
			sa++;
			d2 = _mm_loadu_si128(sa);
			sa++;
			d3 = _mm_loadu_si128(sa);
			sa++;
			CALCULATE_STORE_INTO_INTERM_BUF_S16(d1, dith_0, dith_1);
			CALCULATE_STORE_INTO_INTERM_BUF_S16(d2, dith_2, dith_3);
			CALCULATE_STORE_INTO_INTERM_BUF_S16(d3, dith_4, dith_5);
		}

		if (i <= sw - 3) {
			d1 = _mm_loadu_si128((__m128i *)sa);
			sa++;
			d2 = _mm_loadu_si128((__m128i *)sa);
			CALCULATE_STORE_INTO_INTERM_BUF_S16(d1, dith_0, dith_1);
			CALCULATE_STORE_INTO_INTERM_BUF_S16(d2, dith_2, dith_3);
		} else if (i < sw) {
			d1 = _mm_loadu_si128((__m128i *)sa);
			CALCULATE_STORE_INTO_INTERM_BUF_S16(d1, dith_0, dith_1);
		}

		mlib_ImageColorTrue2IndexLine_S16_U8_3((mlib_s16 *)interm_buf,
		    da, sw, colormap);
		adr_src += sll;
		adr_dst += dll;
	}

	__mlib_free(interm_buf);
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_s_OrderedDither8x8_4_16_8(
    const mlib_image *src,
    mlib_image *dst,
    const mlib_s32 *dmask,
    mlib_s32 scalef_expon,
    const void *colormap)
{
/* pointers for pixel and line of source */
	mlib_s16 *adr_src;

/* pointers for pixel and line of destination */
	mlib_u8 *da, *adr_dst;
	__m128i *interm_buf, *intrm;

/* pointer to row in dither array */
	__m128i *d_d, *sa;

/* line length (in pixels) of source image */
	mlib_s32 sll,
/* line length (in pixels) of destination image */
	    dll,
/* src width in pixels */
	    sw,
/* src height in pixels */
	    sh;

/* indices */
	mlib_s32 row, i;
	__m128i dith_0, dith_1, dith_2, dith_3,
			dith_4, dith_5, dith_6, dith_7;
/* dither koeff */
	__m128i d1, d2, d3, d4;
	__m128i tmp0, tmp1;
	__m128i _s_zero = _mm_xor_si128(_s_zero, _s_zero);
	mlib_d64 mult;
	mlib_s32 bits, shift;

/* temp. dither array */
	mlib_s32 dither[8][8];

/* __m128i dither array */
	__m128i dither_si128[8][8];

	sh = mlib_ImageGetHeight(src);
	sw = mlib_ImageGetWidth(src);
	sll = mlib_ImageGetStride(src) >> 1;
	dll = mlib_ImageGetStride(dst);
	adr_src = (mlib_s16 *)mlib_ImageGetData(src);
	adr_dst = (mlib_u8 *)mlib_ImageGetData(dst);
	bits = mlib_ImageGetBits(colormap);

	shift = (1 << (16 - bits));
	mult = (mlib_d64)shift;
	while (scalef_expon > 30) {
		mult /= (1 << 30);
		scalef_expon -= 30;
	}

	mult /= (1 << scalef_expon);

	for (row = 0; row < 8; row++) {
		for (i = 0; i < 8; i++) {
			dither[row][i] =
			    (shift >> 1) - 1 - (mlib_s32)(dmask[8 * row +
			    i] * mult);
		}
	}

/* load dither koeff. into __m128i  1, 2, 3, 4 */
	for (row = 0; row < 8; row++) {
		for (i = 0; i < 8; i++) {
			dither_si128[row][i] =
			    _mm_setr_epi32(
				dither[row][i], dither[row][7 - i],
				dither[7 - row][i], dither[7 - row][7 - i]);
		}
	}

	interm_buf = (__m128i *)__mlib_malloc(sw * sizeof (__m128i));

	if (interm_buf == NULL)
		return (MLIB_FAILURE);

#ifdef  __SUNPRO_C
#pragma pipeloop(0)
#endif
	for (row = 0; row < sh; row++) {
		intrm = interm_buf;
		sa = (__m128i *)adr_src;
		da = adr_dst;
		d_d = dither_si128[row & 7];
		dith_0 = d_d[0];
		dith_1 = d_d[1];
		dith_2 = d_d[2];
		dith_3 = d_d[3];
		dith_4 = d_d[4];
		dith_5 = d_d[5];
		dith_6 = d_d[6];
		dith_7 = d_d[7];
#ifdef  __SUNPRO_C
#pragma pipeloop(0)
#endif
		for (i = 0; i <= sw - 7; i += 8) {
			d1 = _mm_loadu_si128(sa);
			sa++;
			d2 = _mm_loadu_si128(sa);
			sa++;
			d3 = _mm_loadu_si128(sa);
			sa++;
			d4 = _mm_loadu_si128(sa);
			sa++;
			CALCULATE_STORE_INTO_INTERM_BUF_S16(d1, dith_0, dith_1);
			CALCULATE_STORE_INTO_INTERM_BUF_S16(d2, dith_2, dith_3);
			CALCULATE_STORE_INTO_INTERM_BUF_S16(d3, dith_4, dith_5);
			CALCULATE_STORE_INTO_INTERM_BUF_S16(d4, dith_6, dith_7);
		}

		if (i <= sw - 3) {
			d1 = _mm_loadu_si128(sa);
			sa++;
			CALCULATE_STORE_INTO_INTERM_BUF_S16(d1, dith_0, dith_1);
			d2 = _mm_loadu_si128(sa);
			sa++;
			CALCULATE_STORE_INTO_INTERM_BUF_S16(d2, dith_2, dith_3);
			i += 4;
			dith_0 = dith_4;
			dith_1 = dith_5;
		}

		if (i < sw) {
			d3 = _mm_loadu_si128(sa);
			CALCULATE_STORE_INTO_INTERM_BUF_S16(d3, dith_0, dith_1);
		}

		mlib_ImageColorTrue2IndexLine_S16_U8_4((mlib_s16 *)interm_buf,
		    da, sw, colormap);
		adr_src += sll;
		adr_dst += dll;
	}

	__mlib_free(interm_buf);
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_s_OrderedDither8x8_3_8_16(
    const mlib_image *src,
    mlib_image *dst,
    const mlib_s32 *dmask,
    mlib_s32 scalef_expon,
    const void *colormap)
{
/* pointers for pixel and line of source */
	mlib_u8 *adr_src;

/* pointers for pixel and line of destination */
	mlib_s16 *da, *adr_dst;
	__m64 *interm_buf;
	__m64 *intrm;

/* pointer to row in dither array */
	__m128i *d_d;
	__m64 *sa;

/* line length (in pixels) of source image */
	mlib_s32 sll,
/* line length (in pixels) of destination image */
	    dll,
/* src width in pixels */
	    sw,
/* src height in pixels */
	    sh;

/* indices */
	mlib_s32 row, i;

/* dither koeff */
	__m128i dith_0, dith_1, dith_2, dith_3, dith_4, dith_5;
	__m128i d1, d2, d3, d4, d5;
	__m128i tmp;
	__m128i _s_zero = _mm_xor_si128(_s_zero, _s_zero);
	mlib_d64 mult;
	mlib_s32 bits, shift;

/* temp. dither array */
	mlib_s16 dither[8][8];

/* __m128i dither array */

	__m128i dither_si128[8][3];

	sh = mlib_ImageGetHeight(src);
	sw = mlib_ImageGetWidth(src);
	sll = mlib_ImageGetStride(src);
	dll = mlib_ImageGetStride(dst) >> 1;
	adr_src = (mlib_u8 *)mlib_ImageGetData(src);
	adr_dst = (mlib_s16 *)mlib_ImageGetData(dst);
	bits = mlib_ImageGetBits(colormap);

	shift = (1 << (8 - bits));
	mult = (mlib_d64)shift;
	while (scalef_expon > 30) {
		mult /= (1 << 30);
		scalef_expon -= 30;
	}

	mult /= (1 << scalef_expon);

	for (row = 0; row < 8; row++) {
		for (i = 0; i < 8; i++) {
			dither[row][i] =
			    (shift >> 1) - 1 - (mlib_s32)(dmask[8 * row +
			    i] * mult);
		}
	}

/* load dither koeff. into __m128i  1, 2, 3, 1, 2, 3, 1, 2   etc. */
	for (row = 0; row < 8; row++) {
		dither_si128[row][0] =
			_mm_setr_epi32(
			((dither[row][7] << 16) |
			(dither[row][0] & 0xFFFF)),
			((dither[row][1] << 16) |
			(dither[7 - row][0] & 0xFFFF)),
			((dither[7 - row][1] << 16) |
			(dither[row][6] & 0xFFFF)),
			((dither[row][5] << 16) |
			(dither[row][2] & 0xFFFF)));

		dither_si128[row][1] =
			_mm_setr_epi32(
			((dither[row][3] << 16) |
			(dither[7 - row][2] & 0xFFFF)),
			((dither[7 - row][3] << 16) |
			(dither[row][4] & 0xFFFF)),
			((dither[row][3] << 16) |
			(dither[row][4] & 0xFFFF)),
			((dither[row][5] << 16) |
			(dither[7 - row][4] & 0xFFFF)));

		dither_si128[row][2] =
			_mm_setr_epi32(
			((dither[7 - row][5] << 16) |
			(dither[row][2] & 0xFFFF)),
			((dither[row][1] << 16) |
			(dither[row][6] & 0xFFFF)),
			((dither[row][7] << 16) |
			(dither[7 - row][6] & 0xFFFF)),
			((dither[7 - row][7] << 16) |
			(dither[row][0] & 0xFFFF)));
	}

	interm_buf =
	    (__m64 *)__mlib_malloc((((sw * 3) >> 3) +
	    2) * sizeof (__m64));

	if (interm_buf == NULL)
		return (MLIB_FAILURE);

#ifdef  __SUNPRO_C
#pragma pipeloop(0)
#endif
	for (row = 0; row < sh; row++) {
		intrm = interm_buf;
		sa = (__m64 *)adr_src;
		da = adr_dst;
		d_d = dither_si128[row & 7];
		dith_0 = d_d[0];
		dith_1 = d_d[1];
		dith_2 = d_d[2];
#ifdef  __SUNPRO_C
#pragma pipeloop(0)
#endif
		for (i = 0; i <= sw - 6; i += 8) {
			d1 = _mm_loadl_epi64((__m128i *)sa);
			sa++;
			d2 = _mm_loadl_epi64((__m128i *)sa);
			sa++;
			d3 = _mm_loadl_epi64((__m128i *)sa);
			sa++;
			CALCULATE_STORE_INTO_INTERM_BUF_U8(d1, dith_0);
			CALCULATE_STORE_INTO_INTERM_BUF_U8(d2, dith_1);
			CALCULATE_STORE_INTO_INTERM_BUF_U8(d3, dith_2);
		}

		if (i <= sw - 3) {
			d1 = _mm_loadl_epi64((__m128i *)sa);
			sa++;
			d2 = _mm_loadl_epi64((__m128i *)sa);
			CALCULATE_STORE_INTO_INTERM_BUF_U8(d1, dith_0);
			CALCULATE_STORE_INTO_INTERM_BUF_U8(d2, dith_1);
		} else if (i < sw) {
			d1 = _mm_loadl_epi64((__m128i *)sa);
			CALCULATE_STORE_INTO_INTERM_BUF_U8(d1, dith_0);
		}

		mlib_ImageColorTrue2IndexLine_U8_S16_3(
			(mlib_u8 *)interm_buf, da, sw, colormap);

		adr_src += sll;
		adr_dst += dll;
	}

	__mlib_free(interm_buf);
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_s_OrderedDither8x8_4_8_16(
    const mlib_image *src,
    mlib_image *dst,
    const mlib_s32 *dmask,
    mlib_s32 scalef_expon,
    const void *colormap)
{
/* pointers for pixel and line of source */
	mlib_u8 *adr_src;

/* pointers for pixel and line of destination */
	mlib_s16 *da, *adr_dst;
	__m64 *interm_buf, *intrm;

/* pointer to row in dither array */
	__m128i *d_d;
	__m64 *sa;

/* line length (in pixels) of source image */
	mlib_s32 sll,
/* line length (in pixels) of destination image */
	    dll,
/* src width in pixels */
	    sw,
/* src height in pixels */
	    sh;

/* indices */
	mlib_s32 row, i;
/* dither koeff */
	__m128i dith_0, dith_1, dith_2, dith_3;
	__m128i d1, d2, d3, d4;
	__m128i tmp;
	__m128i _s_zero = _mm_xor_si128(_s_zero, _s_zero);
	mlib_d64 mult;
	mlib_s32 bits, shift;

/* temp. dither array */
	mlib_s16 dither[8][8];

/* __m128i dither array */
	__m128i dither_si128[8][4];

	sh = mlib_ImageGetHeight(src);
	sw = mlib_ImageGetWidth(src);
	sll = mlib_ImageGetStride(src);
	dll = mlib_ImageGetStride(dst) >> 1;
	adr_src = (mlib_u8 *)mlib_ImageGetData(src);
	adr_dst = (mlib_s16 *)mlib_ImageGetData(dst);
	bits = mlib_ImageGetBits(colormap);

	shift = (1 << (8 - bits));
	mult = (mlib_d64)shift;
	while (scalef_expon > 30) {
		mult /= (1 << 30);
		scalef_expon -= 30;
	}

	mult /= (1 << scalef_expon);

	for (row = 0; row < 8; row++) {
		for (i = 0; i < 8; i++) {
			dither[row][i] =
				(shift >> 1) - 1 - (mlib_s32)(dmask[8 * row +
			    i] * mult);
		}
	}

/* load dither koeff. into __m128i  1, 2, 3, 4 */
	for (row = 0; row < 8; row++) {
		for (i = 0; i < 8; i += 2) {
			dither_si128[row][i >> 1] =
				_mm_setr_epi32(
				((dither[row][7 - i] << 16) |
				(dither[row][i] & 0xFFFF)),
				((dither[7 - row][7 - i] << 16) |
				(dither[7 - row][i] & 0xFFFF)),
				((dither[row][6 - i] << 16) |
				(dither[row][i + 1] & 0xFFFF)),
				((dither[7 - row][6 - i] << 16) |
				(dither[7 - row][i + 1] & 0xFFFF)));
		}
	}

	interm_buf =
	    (__m64 *)__mlib_malloc(((sw + 1) << 1) * sizeof (__m64));

	if (interm_buf == NULL)
		return (MLIB_FAILURE);

#ifdef  __SUNPRO_C
#pragma pipeloop(0)
#endif
	for (row = 0; row < sh; row++) {
		intrm = interm_buf;
		sa = (__m64 *)adr_src;
		da = adr_dst;
		d_d = dither_si128[row & 7];
		dith_0 = d_d[0];
		dith_1 = d_d[1];
		dith_2 = d_d[2];
		dith_3 = d_d[3];

#ifdef  __SUNPRO_C
#pragma pipeloop(0)
#endif
		for (i = 0; i <= sw - 7; i += 8) {
			d1 = _mm_loadl_epi64((__m128i *)sa);
			sa++;
			d2 = _mm_loadl_epi64((__m128i *)sa);
			sa++;
			d3 = _mm_loadl_epi64((__m128i *)sa);
			sa++;
			d4 = _mm_loadl_epi64((__m128i *)sa);
			sa++;
			CALCULATE_STORE_INTO_INTERM_BUF_U8(d1, dith_0);
			CALCULATE_STORE_INTO_INTERM_BUF_U8(d2, dith_1);
			CALCULATE_STORE_INTO_INTERM_BUF_U8(d3, dith_2);
			CALCULATE_STORE_INTO_INTERM_BUF_U8(d4, dith_3);
		}

		if (i <= sw - 3) {
			d1 = _mm_loadl_epi64((__m128i *)sa);
			sa++;
			CALCULATE_STORE_INTO_INTERM_BUF_U8(d1, dith_0);
			d2 = _mm_loadl_epi64((__m128i *)sa);
			sa++;
			CALCULATE_STORE_INTO_INTERM_BUF_U8(d2, dith_1);
			i += 4;
			dith_0 = dith_2;
		}

		if (i < sw) {
			d3 = _mm_loadl_epi64((__m128i *)sa);
			CALCULATE_STORE_INTO_INTERM_BUF_U8(d3, dith_0);
		}

		mlib_ImageColorTrue2IndexLine_U8_S16_4(
			(mlib_u8 *)interm_buf, da, sw, colormap);
		adr_src += sll;
		adr_dst += dll;
	}

	__mlib_free(interm_buf);
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_s_OrderedDither8x8_3_16_16(
    const mlib_image *src,
    mlib_image *dst,
    const mlib_s32 *dmask,
    mlib_s32 scalef_expon,
    const void *colormap)

{
/* pointers for pixel and line of source */
	mlib_s16 *adr_src;

/* pointers for pixel and line of destination */
	mlib_s16 *da, *adr_dst;
	__m128i *interm_buf, *intrm;

/* pointer to row in dither array */
	__m128i *d_d, *sa;

/* line length (in pixels) of source image */
	mlib_s32 sll,
/* line length (in pixels) of destination image */
	    dll,
/* src width in pixels */
	    sw,
/* src height in pixels */
	    sh;

/* indices */
	mlib_s32 row, i;
	__m128i dith_0, dith_1, dith_2, dith_3, dith_4, dith_5;
/* dither koeff */

/* dither koeff */
	__m128i d1, d2, d3;
	__m128i tmp0, tmp1;
	__m128i _s_zero = _mm_xor_si128(_s_zero, _s_zero);
	mlib_d64 mult;
	mlib_s32 bits, shift;

/* temp. dither array */
	mlib_s32 dither[8][8];

/* __m128i dither array */
	__m128i dither_si128[8][6];

	sh = mlib_ImageGetHeight(src);
	sw = mlib_ImageGetWidth(src);
	sll = mlib_ImageGetStride(src) >> 1;
	dll = mlib_ImageGetStride(dst) >> 1;
	adr_src = (mlib_s16 *)mlib_ImageGetData(src);
	adr_dst = (mlib_s16 *)mlib_ImageGetData(dst);
	bits = mlib_ImageGetBits(colormap);

	shift = (1 << (16 - bits));
	mult = (mlib_d64)shift;
	while (scalef_expon > 30) {
		mult /= (1 << 30);
		scalef_expon -= 30;
	}

	mult /= (1 << scalef_expon);

	for (row = 0; row < 8; row++) {
		for (i = 0; i < 8; i++) {
			dither[row][i] =
				(shift >> 1) - 1 - (mlib_s32)(dmask[8 * row +
				i] * mult);
		}
	}

/* load dither koeff. into __m128i  1, 2, 3 */
	for (row = 0; row < 8; row++) {
		dither_si128[row][0] =
			_mm_setr_epi32(
			dither[row][0], dither[row][7],
			dither[7 - row][0], dither[row][1]);

		dither_si128[row][1] =
			_mm_setr_epi32(
			dither[row][6], dither[7 - row][1],
			dither[row][2], dither[row][5]);

		dither_si128[row][2] =
			_mm_setr_epi32(
			dither[7 - row][2], dither[row][3],
			dither[row][4], dither[7 - row][3]);

		dither_si128[row][3] =
			_mm_setr_epi32(
			dither[row][4], dither[row][3],
			dither[7 - row][4], dither[row][5]);

		dither_si128[row][4] =
			_mm_setr_epi32(
			dither[row][2], dither[7 - row][5],
			dither[row][6], dither[row][1]);

		dither_si128[row][5] =
			_mm_setr_epi32(
			dither[7 - row][6], dither[row][7],
			dither[row][0], dither[7 - row][7]);
	}

	interm_buf =
	    (__m128i *)__mlib_malloc((((sw * 3) << 2) +
	    2) * sizeof (__m128i));

	if (interm_buf == NULL)
		return (MLIB_FAILURE);

#ifdef  __SUNPRO_C
#pragma pipeloop(0)
#endif
	for (row = 0; row < sh; row++) {
		intrm = interm_buf;
		sa = (__m128i *)adr_src;
		da = adr_dst;
		d_d = dither_si128[row & 7];
		dith_0 = d_d[0];
		dith_1 = d_d[1];
		dith_2 = d_d[2];
		dith_3 = d_d[3];
		dith_4 = d_d[4];
		dith_5 = d_d[5];

#ifdef  __SUNPRO_C
#pragma pipeloop(0)
#endif
		for (i = 0; i <= sw - 6; i += 8) {
			d1 = _mm_loadu_si128(sa);
			sa++;
			d2 = _mm_loadu_si128(sa);
			sa++;
			d3 = _mm_loadu_si128(sa);
			sa++;
			CALCULATE_STORE_INTO_INTERM_BUF_S16(d1, dith_0, dith_1);
			CALCULATE_STORE_INTO_INTERM_BUF_S16(d2, dith_2, dith_3);
			CALCULATE_STORE_INTO_INTERM_BUF_S16(d3, dith_4, dith_5);
		}

		if (i <= sw - 3) {
			d1 = _mm_loadu_si128((__m128i *)sa);
			sa++;
			d2 = _mm_loadu_si128((__m128i *)sa);
			CALCULATE_STORE_INTO_INTERM_BUF_S16(d1, dith_0, dith_1);
			CALCULATE_STORE_INTO_INTERM_BUF_S16(d2, dith_2, dith_3);
		} else if (i < sw) {
			d1 = _mm_loadu_si128((__m128i *)sa);
			CALCULATE_STORE_INTO_INTERM_BUF_S16(d1, dith_0, dith_1);
		}

		mlib_ImageColorTrue2IndexLine_S16_S16_3((mlib_s16 *)interm_buf,
		    da, sw, colormap);
		adr_src += sll;
		adr_dst += dll;
	}
	__mlib_free(interm_buf);
	return (MLIB_SUCCESS);
}
/* *********************************************************** */

mlib_status
mlib_s_OrderedDither8x8_4_16_16(
    const mlib_image *src,
    mlib_image *dst,
    const mlib_s32 *dmask,
    mlib_s32 scalef_expon,
    const void *colormap)
{
/* pointers for pixel and line of source */
	mlib_s16 *adr_src;

/* pointers for pixel and line of destination */
	mlib_s16 *da, *adr_dst;
	__m128i *interm_buf, *intrm;

/* pointer to row in dither array */
	__m128i *d_d, *sa;

/* line length (in pixels) of source image */
	mlib_s32 sll,
/* line length (in pixels) of destination image */
	    dll,
/* src width in pixels */
	    sw,
/* src height in pixels */
	    sh;

/* indices */
	mlib_s32 row, i;
	__m128i dith_0, dith_1, dith_2, dith_3,
			dith_4, dith_5, dith_6, dith_7;
/* dither koeff */
	__m128i d1, d2, d3, d4;
	__m128i tmp0, tmp1;
	__m128i _s_zero = _mm_xor_si128(_s_zero, _s_zero);
	mlib_d64 mult;
	mlib_s32 bits, shift;

/* temp. dither array */
	mlib_s32 dither[8][8];

/* __m128i dither array */
	__m128i dither_si128[8][8];

	sh = mlib_ImageGetHeight(src);
	sw = mlib_ImageGetWidth(src);
	sll = mlib_ImageGetStride(src) >> 1;
	dll = mlib_ImageGetStride(dst) >> 1;
	adr_src = (mlib_s16 *)mlib_ImageGetData(src);
	adr_dst = (mlib_s16 *)mlib_ImageGetData(dst);
	bits = mlib_ImageGetBits(colormap);

	shift = (1 << (16 - bits));
	mult = (mlib_d64)shift;
	while (scalef_expon > 30) {
		mult /= (1 << 30);
		scalef_expon -= 30;
	}

	mult /= (1 << scalef_expon);

	for (row = 0; row < 8; row++) {
		for (i = 0; i < 8; i++) {
			dither[row][i] =
			    (shift >> 1) - 1 - (mlib_s32)(dmask[8 * row +
			    i] * mult);
		}
	}

/* load dither koeff. into __m128i  1, 2, 3, 4 */
	for (row = 0; row < 8; row++) {
		for (i = 0; i < 8; i++) {
			dither_si128[row][i] =
			    _mm_setr_epi32(
				dither[row][i], dither[row][7 - i],
				dither[7 - row][i], dither[7 - row][7 - i]);
		}
	}

	interm_buf = (__m128i *)__mlib_malloc(sw * sizeof (__m128i));

	if (interm_buf == NULL)
		return (MLIB_FAILURE);

#ifdef  __SUNPRO_C
#pragma pipeloop(0)
#endif
	for (row = 0; row < sh; row++) {
		intrm = interm_buf;
		sa = (__m128i *)adr_src;
		da = adr_dst;
		d_d = dither_si128[row & 7];
		dith_0 = d_d[0];
		dith_1 = d_d[1];
		dith_2 = d_d[2];
		dith_3 = d_d[3];
		dith_4 = d_d[4];
		dith_5 = d_d[5];
		dith_6 = d_d[6];
		dith_7 = d_d[7];
#ifdef  __SUNPRO_C
#pragma pipeloop(0)
#endif
		for (i = 0; i <= sw - 7; i += 8) {
			d1 = _mm_loadu_si128(sa);
			sa++;
			d2 = _mm_loadu_si128(sa);
			sa++;
			d3 = _mm_loadu_si128(sa);
			sa++;
			d4 = _mm_loadu_si128(sa);
			sa++;
			CALCULATE_STORE_INTO_INTERM_BUF_S16(d1, dith_0, dith_1);
			CALCULATE_STORE_INTO_INTERM_BUF_S16(d2, dith_2, dith_3);
			CALCULATE_STORE_INTO_INTERM_BUF_S16(d3, dith_4, dith_5);
			CALCULATE_STORE_INTO_INTERM_BUF_S16(d4, dith_6, dith_7);
		}

		if (i <= sw - 3) {
			d1 = _mm_loadu_si128(sa);
			sa++;
			CALCULATE_STORE_INTO_INTERM_BUF_S16(d1, dith_0, dith_1);
			d2 = _mm_loadu_si128(sa);
			sa++;
			CALCULATE_STORE_INTO_INTERM_BUF_S16(d2, dith_2, dith_3);
			i += 4;
			dith_0 = dith_4;
			dith_1 = dith_5;
		}

		if (i < sw) {
			d3 = _mm_loadu_si128(sa);
			CALCULATE_STORE_INTO_INTERM_BUF_S16(d3, dith_0, dith_1);
		}

		mlib_ImageColorTrue2IndexLine_S16_S16_4((mlib_s16 *)interm_buf,
		    da, sw, colormap);
		adr_src += sll;
		adr_dst += dll;
	}

	__mlib_free(interm_buf);
	return (MLIB_SUCCESS);
}

/* *********************************************************** */
