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

#pragma ident	"@(#)mlib_s_ImageColorRGB2Mono.c	9.4	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_ImageColorRGB2Mono - color to mono conversion
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageColorRGB2Mono(mlib_image       *dst,
 *                                          const mlib_image *src,
 *                                          const mlib_d64   *weight)
 *
 *      mlib_status mlib_ImageColorRGB2CIEMono(mlib_image       *dst,
 *                                             const mlib_image *src)
 *
 * ARGUMENT
 *      dst     pointer to output image
 *      src     pointer to input image
 *      weight  array of 3 blending coefficients
 *
 * RESTRICTION
 *      src and dst must be the same type, and the same size.
 *      The input must be a 3-channel image,
 *      and the output image must be a single channel image.
 *      They can be in MLIB_BYTE, MLIB_SHORT, MLIB_USHORT or MLIB_INT data type.
 *
 * DESCRIPTION
 *      mlib_ImageColorRGB2Mono
 *        Z =  weight[0] * R +  weight[1] * G +  weight[2] * B
 *      mlib_ImageColorRGB2CIEMono
 *        Z =  0.2125 * R +  0.7154 * G +  0.0721 * B
 *
 *     It is required that all blending coefficients(weight) be no more
 *     than 1.0
 */

#include <mlib_image.h>
#include <mlib_ImageCheck.h>
#include <mlib_SysMath.h>

#ifdef __SUNPRO_C
#include <sunmedia_intrin.h>
#else
#include <emmintrin.h>
#endif

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageColorRGB2CIEMono = __mlib_ImageColorRGB2CIEMono
#pragma weak mlib_ImageColorRGB2Mono = __mlib_ImageColorRGB2Mono

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_ImageColorRGB2CIEMono) mlib_ImageColorRGB2CIEMono
    __attribute__((weak, alias("__mlib_ImageColorRGB2CIEMono")));
__typeof__(__mlib_ImageColorRGB2Mono) mlib_ImageColorRGB2Mono
    __attribute__((weak, alias("__mlib_ImageColorRGB2Mono")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	SAT_S32(dst, src)		\
	if (src > MLIB_S32_MAX)		\
		dst = MLIB_S32_MAX;	\
	else if (src < MLIB_S32_MIN)	\
		dst = MLIB_S32_MIN;	\
	else				\
		dst = src;

#ifdef MLIB_USE_FTOI_CLAMPING

#define	SAT32(DST, SRC)	(*DST++) = (mlib_s32) SRC

#else /* MLIB_USE_FTOI_CLAMPING */

#define	SAT32(DST, SRC)                                        \
	if (SRC >= MLIB_S32_MAX)                               \
	    (*DST++) = MLIB_S32_MAX;                           \
	else                                                   \
	    if (SRC <= MLIB_S32_MIN)                           \
		(*DST++) = MLIB_S32_MIN;                       \
	    else                                               \
		(*DST++) = (mlib_s32)SRC

#endif /* MLIB_USE_FTOI_CLAMPING */

/* *********************************************************** */

static mlib_status mlib_s_ImageColorRGB2Mono_U8(
    const mlib_u8 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_d64 *weight);

static mlib_status mlib_s_ImageColorRGB2Mono_S16(
    const mlib_s16 *src,
    mlib_s32 slb,
    mlib_s16 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_d64 *weight);

static mlib_status mlib_s_ImageColorRGB2Mono_U16(
    const mlib_u16 *src,
    mlib_s32 slb,
    mlib_u16 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_d64 *weight);

static mlib_status mlib_s_ImageColorRGB2Mono_S32(
    const mlib_s32 *src,
    mlib_s32 slb,
    mlib_s32 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_d64 *weight);

/* *********************************************************** */

mlib_status
mlib_s_ImageColorRGB2Mono_U8(
    const mlib_u8 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_d64 *weight)
{
	int i, j, nblock, sny, dny;
	mlib_s16 w[3];
	__m128i r, g, b, rl, gl, bl, rh, gh, bh, w0, w1, w2;
	__m128i sat, zero;
	sat = _mm_set1_epi32(128);
	zero = _mm_setzero_si128();
	if (weight[0] > 0)
		w[0] = (mlib_s16)(weight[0] * 256.0 + 0.5);
	else
		w[0] = (mlib_s16)(weight[0] * 256.0 - 0.5);
	if (weight[1] > 0)
		w[1] = (mlib_s16)(weight[1] * 256.0 + 0.5);
	else
		w[1] = (mlib_s16)(weight[1] * 256.0 - 0.5);
	if (weight[2] > 0)
		w[2] = (mlib_s16)(weight[2] * 256.0 + 0.5);
	else
		w[2] = (mlib_s16)(weight[2] * 256.0 - 0.5);
	w0 = _mm_set1_epi16(w[0]);
	w1 = _mm_set1_epi16(w[1]);
	w2 = _mm_set1_epi16(w[2]);
	nblock = xsize >> 3;
	sny = 0;
	dny = 0;
	for (j = 0; j < ysize; j++) {
		int snb = sny;
		int dnb = dny;

		for (i = 0; i < nblock; i++) {

			/* load and shuffle */
			r = _mm_loadl_epi64((__m128i *)(src + snb));
			g = _mm_loadl_epi64((__m128i *)(src + snb + 8));
			b = _mm_loadl_epi64((__m128i *)(src + snb + 16));
			r = _mm_unpacklo_epi8(r, zero);
			g = _mm_unpacklo_epi8(g, zero);
			b = _mm_unpacklo_epi8(b, zero);

			rl = _mm_unpacklo_epi16(r, _mm_srli_si128(g, 8));
			gl = _mm_unpacklo_epi16(_mm_srli_si128(r, 8), b);
			bl = _mm_unpacklo_epi16(g, _mm_srli_si128(b, 8));

			rh = _mm_unpacklo_epi16(rl, _mm_srli_si128(gl, 8));
			gh = _mm_unpacklo_epi16(_mm_srli_si128(rl, 8), bl);
			bh = _mm_unpacklo_epi16(gl, _mm_srli_si128(bl, 8));

			r = _mm_unpacklo_epi16(rh, _mm_srli_si128(gh, 8));
			g = _mm_unpacklo_epi16(_mm_srli_si128(rh, 8), bh);
			b = _mm_unpacklo_epi16(gh, _mm_srli_si128(bh, 8));

			/* 3-channel units are stored in r, g, b */
			/* start to multiply */
			rl = _mm_mullo_epi16(r, w0);
			rh = _mm_mulhi_epi16(r, w0);
			gl = _mm_mullo_epi16(g, w1);
			gh = _mm_mulhi_epi16(g, w1);
			bl = _mm_mullo_epi16(b, w2);
			bh = _mm_mulhi_epi16(b, w2);

			r  = _mm_unpacklo_epi16(rl, rh);
			rh = _mm_unpackhi_epi16(rl, rh);
			g  = _mm_unpacklo_epi16(gl, gh);
			gh = _mm_unpackhi_epi16(gl, gh);
			b  = _mm_unpacklo_epi16(bl, bh);
			bh = _mm_unpackhi_epi16(bl, bh);

			r  = _mm_add_epi32(r, g);
			rh = _mm_add_epi32(rh, gh);
			r  = _mm_add_epi32(r, b);
			rh = _mm_add_epi32(rh, bh);
			r  = _mm_add_epi32(r, sat);
			rh = _mm_add_epi32(rh, sat);

			r  = _mm_srai_epi32(r, 8);
			rh = _mm_srai_epi32(rh, 8);

			r = _mm_packs_epi32(r, rh);
			r = _mm_packus_epi16(r, r);
			_mm_storel_epi64((__m128i *)(dst + dnb), r);

			snb += 24;
			dnb += 8;
		}
		for (i = nblock * 8; i < xsize; i++) {
			int res = (src[snb] * w[0] + src[snb + 1] * w[1]
					+ src[snb + 2] * w[2] + 128) >> 8;
			if (res > 255)
				dst[dnb] = 255;
			else if (res < 0)
				dst[dnb] = 0;
			else
				dst[dnb] = res;

			snb += 3;
			dnb++;
		}
		sny += slb;
		dny += dlb;
	}
	_mm_empty();
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_s_ImageColorRGB2Mono_S16(
    const mlib_s16 *src,
    mlib_s32 slb,
    mlib_s16 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_d64 *weight)
{
	int i, j, nblock, sny, dny;
	mlib_s16 w[3];
	__m128i r, g, b, rl, gl, bl, rh, gh, bh, w0, w1, w2, zero;
	zero  = _mm_setzero_si128();
	if (weight[0] > 0)
		w[0] = (mlib_s16)(weight[0] * 32768.0 + 0.5);
	else
		w[0] = (mlib_s16)(weight[0] * 32768.0 - 0.5);
	if (weight[1] > 0)
		w[1] = (mlib_s16)(weight[1] * 32768.0 + 0.5);
	else
		w[1] = (mlib_s16)(weight[1] * 32768.0 - 0.5);
	if (weight[2] > 0)
		w[2] = (mlib_s16)(weight[2] * 32768.0 + 0.5);
	else
		w[2] = (mlib_s16)(weight[2] * 32768.0 - 0.5);
	w0 = _mm_set1_epi16(w[0]);
	w1 = _mm_set1_epi16(w[1]);
	w2 = _mm_set1_epi16(w[2]);
	nblock = xsize >> 3;
	sny = 0;
	dny = 0;
	for (j = 0; j < ysize; j++) {
		int snb = sny;
		int dnb = dny;

		for (i = 0; i < nblock; i++) {

			/* load and shuffle */
			r = _mm_loadu_si128((__m128i *)(src + snb));
			g = _mm_loadu_si128((__m128i *)(src + snb + 8));
			b = _mm_loadu_si128((__m128i *)(src + snb + 16));

			rl = _mm_unpacklo_epi16(r, _mm_srli_si128(g, 8));
			gl = _mm_unpacklo_epi16(_mm_srli_si128(r, 8), b);
			bl = _mm_unpacklo_epi16(g, _mm_srli_si128(b, 8));

			rh = _mm_unpacklo_epi16(rl, _mm_srli_si128(gl, 8));
			gh = _mm_unpacklo_epi16(_mm_srli_si128(rl, 8), bl);
			bh = _mm_unpacklo_epi16(gl, _mm_srli_si128(bl, 8));

			r = _mm_unpacklo_epi16(rh, _mm_srli_si128(gh, 8));
			g = _mm_unpacklo_epi16(_mm_srli_si128(rh, 8), bh);
			b = _mm_unpacklo_epi16(gh, _mm_srli_si128(bh, 8));

			/* 3-channel units are stored in r, g, b */
			/* start to multiply */
			rl = _mm_mullo_epi16(r, w0);
			rh = _mm_mulhi_epi16(r, w0);
			gl = _mm_mullo_epi16(g, w1);
			gh = _mm_mulhi_epi16(g, w1);
			bl = _mm_mullo_epi16(b, w2);
			bh = _mm_mulhi_epi16(b, w2);

			r  = _mm_unpacklo_epi16(rl, rh);
			rh = _mm_unpackhi_epi16(rl, rh);
			g  = _mm_unpacklo_epi16(gl, gh);
			gh = _mm_unpackhi_epi16(gl, gh);
			b  = _mm_unpacklo_epi16(bl, bh);
			bh = _mm_unpackhi_epi16(bl, bh);

			r  = _mm_add_epi32(r, g);
			r  = _mm_add_epi32(r, b);
			rh = _mm_add_epi32(rh, gh);
			rh = _mm_add_epi32(rh, bh);

			r  = _mm_srai_epi32(r, 15);
			rh = _mm_srai_epi32(rh, 15);

			r = _mm_packs_epi32(r, rh);
			_mm_storeu_si128((__m128i *)(dst + dnb), r);

			snb += 24;
			dnb += 8;
		}
		for (i = nblock * 8; i < xsize; i++) {
			int res = (src[snb] * w[0] + src[snb + 1] * w[1]
					+ src[snb + 2] * w[2]) >> 15;
			if (res > 32767)
				dst[dnb] = 32767;
			else if (res < -32768)
				dst[dnb] = -32768;
			else
				dst[dnb] = res;

			snb += 3;
			dnb++;
		}
		sny += slb;
		dny += dlb;
	}
	_mm_empty();

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_s_ImageColorRGB2Mono_U16(
    const mlib_u16 *src,
    mlib_s32 slb,
    mlib_u16 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_d64 *weight)
{
	int i, j, nblock, sny, dny;
	mlib_u16 w[3];
	__m128i r, g, b, rl, gl, bl, rh, gh, bh, w0, w1, w2;
	__m128i zero, sat1, sat2;
	zero  = _mm_setzero_si128();
	sat1  = _mm_set1_epi32(32768);
	sat2  = _mm_set1_epi16(32768);
	if (weight[0] > 0)
		w[0] = (mlib_s16)(weight[0] * 16384.0 + 0.5);
	else
		w[0] = (mlib_s16)(weight[0] * 16384.0 - 0.5);
	if (weight[1] > 0)
		w[1] = (mlib_s16)(weight[1] * 16384.0 + 0.5);
	else
		w[1] = (mlib_s16)(weight[1] * 16384.0 - 0.5);
	if (weight[2] > 0)
		w[2] = (mlib_s16)(weight[2] * 16384.0 + 0.5);
	else
		w[2] = (mlib_s16)(weight[2] * 16384.0 - 0.5);
	w0 = _mm_set1_epi16(w[0]);
	w1 = _mm_set1_epi16(w[1]);
	w2 = _mm_set1_epi16(w[2]);
	nblock = xsize >> 3;
	sny = 0;
	dny = 0;
	for (j = 0; j < ysize; j++) {
		int snb = sny;
		int dnb = dny;

		for (i = 0; i < nblock; i++) {

			/* load and shuffle */
			r = _mm_loadu_si128((__m128i *)(src + snb));
			g = _mm_loadu_si128((__m128i *)(src + snb + 8));
			b = _mm_loadu_si128((__m128i *)(src + snb + 16));

			rl = _mm_unpacklo_epi16(r, _mm_srli_si128(g, 8));
			gl = _mm_unpacklo_epi16(_mm_srli_si128(r, 8), b);
			bl = _mm_unpacklo_epi16(g, _mm_srli_si128(b, 8));

			rh = _mm_unpacklo_epi16(rl, _mm_srli_si128(gl, 8));
			gh = _mm_unpacklo_epi16(_mm_srli_si128(rl, 8), bl);
			bh = _mm_unpacklo_epi16(gl, _mm_srli_si128(bl, 8));

			r = _mm_unpacklo_epi16(rh, _mm_srli_si128(gh, 8));
			g = _mm_unpacklo_epi16(_mm_srli_si128(rh, 8), bh);
			b = _mm_unpacklo_epi16(gh, _mm_srli_si128(bh, 8));

			/* start to multiply */
			rl = _mm_mullo_epi16(r, w0);
			rh = _mm_mulhi_epu16(r, w0);
			gl = _mm_mullo_epi16(g, w1);
			gh = _mm_mulhi_epu16(g, w1);
			bl = _mm_mullo_epi16(b, w2);
			bh = _mm_mulhi_epu16(b, w2);

			r  = _mm_unpacklo_epi16(rl, rh);
			rh = _mm_unpackhi_epi16(rl, rh);
			g  = _mm_unpacklo_epi16(gl, gh);
			gh = _mm_unpackhi_epi16(gl, gh);
			b  = _mm_unpacklo_epi16(bl, bh);
			bh = _mm_unpackhi_epi16(bl, bh);

			r  = _mm_add_epi32(r, g);
			r  = _mm_add_epi32(r, b);
			rh = _mm_add_epi32(rh, gh);
			rh = _mm_add_epi32(rh, bh);

			r  = _mm_srai_epi32(r, 14);
			rh = _mm_srai_epi32(rh, 14);

			r  = _mm_sub_epi32(r, sat1);
			rh = _mm_sub_epi32(rh, sat1);
			r = _mm_packs_epi32(r, rh);
			r  = _mm_add_epi16(r, sat2);

			_mm_storeu_si128((__m128i *)(dst + dnb), r);

			snb += 24;
			dnb += 8;
		}
		for (i = nblock * 8; i < xsize; i++) {
			int res = ((int)src[snb] * w[0] + src[snb + 1] * w[1]
				+ src[snb + 2] * w[2] + 8192) >> 14;
			if (res > 65535)
				dst[dnb] = 65535;
			else if (res < 0)
				dst[dnb] = 0;
			else
				dst[dnb] = res;

			snb += 3;
			dnb++;
		}
		sny += slb;
		dny += dlb;
	}
	_mm_empty();

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#ifndef _NO_LONGLONG
#define	ZOOM32 (1024.0 * 1024 * 512)
#define	SHIFT32 29

mlib_status
mlib_s_ImageColorRGB2Mono_S32(
    const mlib_s32 *src,
    mlib_s32 slb,
    mlib_s32 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_d64 *weight)
{
	mlib_s64 r, w0, w1, w2, sat;
	int i, j, sny, dny;
	if (weight[0] > 0)
		w0 = (mlib_s64)(weight[0] * ZOOM32 + 0.5);
	else
		w0 = (mlib_s64)(weight[0] * ZOOM32 - 0.5);
	if (weight[1] > 0)
		w1 = (mlib_s64)(weight[1] * ZOOM32 + 0.5);
	else
		w1 = (mlib_s64)(weight[1] * ZOOM32 - 0.5);
	if (weight[2] > 0)
		w2 = (mlib_s64)(weight[2] * ZOOM32 + 0.5);
	else
		w2 = (mlib_s64)(weight[2] * ZOOM32 - 0.5);
	sat = (mlib_s64)ZOOM32 >> 1;
	sny = 0;
	dny = 0;
	for (j = 0; j < ysize; j++) {
		int snb = sny;
		int dnb = dny;

		for (i = 0; i < xsize; i++) {
			r = (src[snb] * w0 + src[snb + 1] * w1
				+ src[snb + 2] * w2 + sat) >> SHIFT32;
			SAT_S32(dst[dnb], r);
			snb += 3;
			dnb += 1;
		}
		sny += slb;
		dny += dlb;
	}

	return (MLIB_SUCCESS);
}

#else	/* _NO_LONGLONG */

mlib_status
mlib_s_ImageColorRGB2Mono_S32(
    const mlib_s32 *src,
    mlib_s32 slb,
    mlib_s32 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_d64 *weight)
{
	mlib_s32 *sa, *sl;
	mlib_s32 *da, *dl;
	mlib_d64 sf0, sf1, sf2, df1, w[3];
	mlib_d64 sf3, sf4, sf5, df2;
	mlib_d64 sf6, sf7, sf8, df3;
	mlib_s32 i, j;
	mlib_s32 num_blocks, leftover_cols;

	w[0] = weight[0];
	w[1] = weight[1];
	w[2] = weight[2];

	sl = sa = (void *)src;
	dl = da = dst;

	num_blocks = xsize / 3;
	leftover_cols = xsize - num_blocks * 3;

	for (j = 0; j < ysize; j++) {
		for (i = 0; i < num_blocks; i++) {
			sf0 = (*sa++);
			sf1 = (*sa++);
			sf2 = (*sa++);
			sf3 = (*sa++);
			sf4 = (*sa++);
			sf5 = (*sa++);
			sf6 = (*sa++);
			sf7 = (*sa++);
			sf8 = (*sa++);

			df1 = w[0] * sf0 + w[1] * sf1 + w[2] * sf2;
			df2 = w[0] * sf3 + w[1] * sf4 + w[2] * sf5;
			df3 = w[0] * sf6 + w[1] * sf7 + w[2] * sf8;

			SAT32(da, df1);
			SAT32(da, df2);
			SAT32(da, df3);
		}

		if (leftover_cols) {
			sf0 = (*sa++);
			sf1 = (*sa++);
			sf2 = (*sa++);

			df1 = w[0] * sf0 + w[1] * sf1 + w[2] * sf2;
			SAT32(da, df1);

			if (leftover_cols > 1) {
				sf0 = (*sa++);
				sf1 = (*sa++);
				sf2 = (*sa++);

				df1 = w[0] * sf0 + w[1] * sf1 + w[2] * sf2;
				SAT32(da, df1);
			}
		}

		sl = sa = sl + slb;
		dl = da = dl + dlb;
	}

	return (MLIB_SUCCESS);
}

#endif	/* _NO_LONGLONG */

/* *********************************************************** */

mlib_status
__mlib_ImageColorRGB2Mono(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_d64 *weight)
{
	mlib_s32 slb, dlb, xsize, ysize;
	mlib_type dtype;
	void *sa, *da;
	mlib_s32 i;

	MLIB_IMAGE_CHECK(src);
	MLIB_IMAGE_CHECK(dst);
	MLIB_IMAGE_TYPE_EQUAL(dst, src);
	MLIB_IMAGE_SIZE_EQUAL(dst, src);
	MLIB_IMAGE_HAVE_CHAN(src, 3);
	MLIB_IMAGE_HAVE_CHAN(dst, 1);

	for (i = 0; i < 3; i++)
		if (mlib_fabs(weight[i]) > 1.0)
			return (MLIB_OUTOFRANGE);

	dtype = mlib_ImageGetType(dst);
	xsize = mlib_ImageGetWidth(dst);
	ysize = mlib_ImageGetHeight(dst);
	slb = mlib_ImageGetStride(src);
	dlb = mlib_ImageGetStride(dst);
	sa = mlib_ImageGetData(src);
	da = mlib_ImageGetData(dst);

	if (dtype == MLIB_BYTE) {

		return mlib_s_ImageColorRGB2Mono_U8(sa, slb, da, dlb, xsize,
		    ysize, weight);
	} else if (dtype == MLIB_SHORT) {

		slb >>= 1;
		dlb >>= 1;
		return mlib_s_ImageColorRGB2Mono_S16(sa, slb, da, dlb, xsize,
		    ysize, weight);
	} else if (dtype == MLIB_USHORT) {

		slb >>= 1;
		dlb >>= 1;
		return mlib_s_ImageColorRGB2Mono_U16(sa, slb, da, dlb, xsize,
		    ysize, weight);
	} else if (dtype == MLIB_INT) {

		slb >>= 2;
		dlb >>= 2;
		return mlib_s_ImageColorRGB2Mono_S32(sa, slb, da, dlb, xsize,
		    ysize, weight);
	} else
		return (MLIB_FAILURE);
}

/* *********************************************************** */

mlib_status
__mlib_ImageColorRGB2CIEMono(
    mlib_image *dst,
    const mlib_image *src)
{
/* blending coefficients for CIE */
	mlib_d64 weight[3] = { 0.2125, 0.7154, 0.0721 };

	return (__mlib_ImageColorRGB2Mono(dst, src, weight));
}

/* *********************************************************** */
