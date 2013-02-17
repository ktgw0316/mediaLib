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

#pragma ident   "@(#)mlib_s_ImageThresh1BFunc.c	9.3	07/11/05 SMI"

#include <mlib_image.h>
#include <mlib_ImageCheck.h>
#include <mlib_s_ImageThreshFunc.h>
#ifdef  __SUNPRO_C
#include <sunmedia_intrin.h>
#else
#include <emmintrin.h>
#endif

/* *********************************************************** */

static void mlib_s_ImageThresh1B_U8_1_A8D1X8(
    const mlib_u8 *src,
    mlib_u8 *dst,
    mlib_s32 dsize,
    const mlib_s32 *th,
    mlib_s32 hc,
    mlib_s32 lc);

static void mlib_s_ImageThresh1B_U8_1_A8D2X8(
    const mlib_u8 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_s32 *th,
    mlib_s32 hc,
    mlib_s32 lc);

static void mlib_s_ImageThresh1B_U8_3_A8D1(
    const mlib_u8 *src,
    mlib_u8 *dst,
    mlib_s32 dsize,
    const mlib_s32 *th,
    mlib_s32 hc0,
    mlib_s32 lc0);

static void mlib_s_ImageThresh1B_U8_3_A8D2(
    const mlib_u8 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_s32 *th,
    mlib_s32 hc0,
    mlib_s32 lc0);

static void mlib_s_ImageThresh1B_U8_3(
    const mlib_u8 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 dbit_off,
    const mlib_s32 *th,
    mlib_s32 hc0,
    mlib_s32 lc0);

static void mlib_s_ImageThresh1B_S16_1_A8D1X8(
    const mlib_s16 *src,
    mlib_u8 *dst,
    mlib_s32 dsize,
    const mlib_s32 *th,
    mlib_s32 hc,
    mlib_s32 lc);

static void mlib_s_ImageThresh1B_S16_1_A8D2X8(
    const mlib_s16 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_s32 *th,
    mlib_s32 hc,
    mlib_s32 lc);

static void mlib_s_ImageThresh1B_S16_1(
    const mlib_s16 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 dbit_off,
    const mlib_s32 *th,
    mlib_s32 hc,
    mlib_s32 lc);

static void mlib_s_ImageThresh1B_S16_3_A8D1(
    const mlib_s16 *src,
    mlib_u8 *dst,
    mlib_s32 dsize,
    const mlib_s32 *th,
    mlib_s32 hc0,
    mlib_s32 lc0);

static void mlib_s_ImageThresh1B_S16_3_A8D2(
    const mlib_s16 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_s32 *th,
    mlib_s32 hc0,
    mlib_s32 lc0);

static void mlib_s_ImageThresh1B_S16_3(
    const mlib_s16 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 dbit_off,
    const mlib_s32 *th,
    mlib_s32 hc0,
    mlib_s32 lc0);

static void mlib_s_ImageThresh1B_U16_1_A8D1X8(
    const mlib_s16 *src,
    mlib_u8 *dst,
    mlib_s32 dsize,
    const mlib_s32 *th,
    mlib_s32 hc,
    mlib_s32 lc);

static void mlib_s_ImageThresh1B_U16_1_A8D2X8(
    const mlib_s16 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_s32 *th,
    mlib_s32 hc,
    mlib_s32 lc);

static void mlib_s_ImageThresh1B_U16_1(
    const mlib_s16 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 dbit_off,
    const mlib_s32 *th,
    mlib_s32 hc,
    mlib_s32 lc);

static void mlib_s_ImageThresh1B_U16_3_A8D1(
    const mlib_s16 *src,
    mlib_u8 *dst,
    mlib_s32 dsize,
    const mlib_s32 *th,
    mlib_s32 hc0,
    mlib_s32 lc0);

static void mlib_s_ImageThresh1B_U16_3_A8D2(
    const mlib_s16 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_s32 *th,
    mlib_s32 hc0,
    mlib_s32 lc0);

static void mlib_s_ImageThresh1B_U16_3(
    const mlib_s16 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 dbit_off,
    const mlib_s32 *th,
    mlib_s32 hc0,
    mlib_s32 lc0);

static void mlib_s_ImageThresh1B_S32_1_A8D1X8(
    const mlib_s32 *src,
    mlib_u8 *dst,
    mlib_s32 dsize,
    const mlib_s32 *th,
    mlib_s32 hc,
    mlib_s32 lc);

static void mlib_s_ImageThresh1B_S32_1_A8D2X8(
    const mlib_s32 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_s32 *th,
    mlib_s32 hc,
    mlib_s32 lc);

static void mlib_s_ImageThresh1B_S32_1(
    const mlib_s32 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 dbit_off,
    const mlib_s32 *th,
    mlib_s32 hc,
    mlib_s32 lc);

static void mlib_s_ImageThresh1B_S32_3_A8D1(
    const mlib_s32 *src,
    mlib_u8 *dst,
    mlib_s32 dsize,
    const mlib_s32 *th,
    mlib_s32 hc0,
    mlib_s32 lc0);

static void mlib_s_ImageThresh1B_S32_3_A8D2(
    const mlib_s32 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_s32 *th,
    mlib_s32 hc0,
    mlib_s32 lc0);

static void mlib_s_ImageThresh1B_S32_3(
    const mlib_s32 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 dbit_off,
    const mlib_s32 *th,
    mlib_s32 hc0,
    mlib_s32 lc0);

/* *********************************************************** */

mlib_status
mlib_ImageThresh1B(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_s32 *thresh,
    const mlib_s32 *ghigh,
    const mlib_s32 *glow)
{
	mlib_s32 slb, dlb, xsize, ysize, nchan;
	mlib_type stype, dtype;
	void *sa, *da;
	const mlib_s32 X8 = 0x7;
	const mlib_s32 A8D1 = MLIB_IMAGE_ALIGNED8 | MLIB_IMAGE_ONEDVECTOR;
	const mlib_s32 A8D2 = MLIB_IMAGE_ALIGNED8 | MLIB_IMAGE_STRIDE8X;
	const mlib_s32 A8D2X8 =
	    MLIB_IMAGE_ALIGNED8 | MLIB_IMAGE_STRIDE8X | MLIB_IMAGE_WIDTH8X;
	mlib_s32 flags, flagd;
	mlib_s32 dsize, i, k;
	mlib_s32 buff[3 * 8], *th = buff, *gl = buff + 8, *gh = buff + 16;

	MLIB_IMAGE_CHECK(src);
	MLIB_IMAGE_CHECK(dst);
	MLIB_IMAGE_SIZE_EQUAL(src, dst);
	MLIB_IMAGE_CHAN_EQUAL(src, dst);

	stype = mlib_ImageGetType(src);
	dtype = mlib_ImageGetType(dst);
	nchan = mlib_ImageGetChannels(src);
	xsize = mlib_ImageGetWidth(src);
	ysize = mlib_ImageGetHeight(src);
	slb = mlib_ImageGetStride(src);
	dlb = mlib_ImageGetStride(dst);
	sa = mlib_ImageGetData(src);
	da = mlib_ImageGetData(dst);
	flags = mlib_ImageGetFlags(src);
	flagd = mlib_ImageGetFlags(dst);
	dsize = xsize * ysize;

	if (stype == MLIB_BYTE) {

		for (i = 0; i < nchan; i++) {
			gl[i] = glow[i];

			if (thresh[i] > MLIB_U8_MAX) {
				th[i] = MLIB_U8_MAX;
			} else if (thresh[i] < 0) {
				th[i] = 0;
				gl[i] = ghigh[i];
			} else {
				th[i] = thresh[i];
			}
		}
	} else if (stype == MLIB_SHORT) {

		for (i = 0; i < nchan; i++) {
			gl[i] = glow[i];

			if (thresh[i] > MLIB_S16_MAX) {
				th[i] = MLIB_S16_MAX;
			} else if (thresh[i] < MLIB_S16_MIN) {
				th[i] = MLIB_S16_MIN;
				gl[i] = ghigh[i];
			} else {
				th[i] = thresh[i];
			}
		}
	} else if (stype == MLIB_USHORT) {

		for (i = 0; i < nchan; i++) {
			gl[i] = glow[i];

			if (thresh[i] > MLIB_U16_MAX) {
				th[i] = MLIB_U16_MAX;
			} else if (thresh[i] < MLIB_U16_MIN) {
				th[i] = MLIB_U16_MIN;
				gl[i] = ghigh[i];
			} else {
				th[i] = thresh[i];
			}
		}
	} else if (stype == MLIB_INT) {

		for (i = 0; i < nchan; i++) {
			gl[i] = glow[i];
			th[i] = thresh[i];
		}
	} else {

		return (MLIB_FAILURE);
	}

	for (k = 0; k < nchan; k++)
		gh[k] = ghigh[k];
	for (k = nchan; k < 8; k++) {
		th[k] = th[k - nchan];
		gl[k] = gl[k - nchan];
		gh[k] = gh[k - nchan];
	}

	if (nchan == 2) {
		xsize *= 2;
		dsize *= 2;
	} else if (nchan == 4) {
		xsize *= 4;
		dsize *= 4;
	}

	if (dtype != MLIB_BIT) {
		return (MLIB_FAILURE);
	} else {
		mlib_s32 inc, hc, lc, dbit_off = mlib_ImageGetBitOffset(dst);

		if (nchan != 3) {
			hc = (gh[0] > 0) ? 0x8888 : 0;

			if (gh[1] > 0)
				hc |= 0x4444;

			if (gh[2] > 0)
				hc |= 0x2222;

			if (gh[3] > 0)
				hc |= 0x1111;

			lc = (gl[0] > 0) ? 0x8888 : 0;

			if (gl[1] > 0)
				lc |= 0x4444;

			if (gl[2] > 0)
				lc |= 0x2222;

			if (gl[3] > 0)
				lc |= 0x1111;

			inc = (8 - dbit_off) & 3;
		} else {
			hc = (gh[0] > 0) ? 0x492492 : 0;

			if (gh[1] > 0)
				hc |= 0x249249;

			if (gh[2] > 0)
				hc |= 0x924924;

			lc = (gl[0] > 0) ? 0x492492 : 0;

			if (gl[1] > 0)
				lc |= 0x249249;

			if (gl[2] > 0)
				lc |= 0x924924;

			inc = 9 - dbit_off;

			if (inc >= 6)
				inc -= 6;
			else if (inc >= 3)
				inc -= 3;
		}

		th += inc;
		hc = hc >> dbit_off;
		lc = lc >> dbit_off;

		if (stype == MLIB_BYTE) {
			if (nchan != 3) {
				if (((flags & A8D1) == 0) &&
				    ((flagd & MLIB_IMAGE_ONEDVECTOR) == 0) &&
				    ((dsize & X8) == 0)) {

					mlib_s_ImageThresh1B_U8_1_A8D1X8(sa, da,
					    dsize, th, hc, lc);
				} else if ((flags & A8D2X8) == 0 &&
				    dbit_off == 0) {

					mlib_s_ImageThresh1B_U8_1_A8D2X8(sa,
					    slb, da, dlb, xsize, ysize, th, hc,
					    lc);
				} else {

					mlib_s_ImageThresh1B_U8_1(sa, slb, da,
					    dlb, xsize, ysize, dbit_off, th, hc,
					    lc);
				}
			} else {
				if (((flags & A8D1) == 0) &&
				    ((flagd & MLIB_IMAGE_ONEDVECTOR) == 0)) {

					mlib_s_ImageThresh1B_U8_3_A8D1(sa, da,
					    dsize, th, hc, lc);
				} else if ((flags & A8D2) == 0 &&
						dbit_off == 0) {

					mlib_s_ImageThresh1B_U8_3_A8D2(sa, slb,
					    da, dlb, xsize, ysize, th, hc, lc);
				} else {

					mlib_s_ImageThresh1B_U8_3(sa, slb, da,
					    dlb, xsize, ysize, dbit_off, th, hc,
					    lc);
				}
			}
		} else if (stype == MLIB_SHORT) {

			if (nchan != 3) {
				if (((flags & A8D1) == 0) &&
				    ((flagd & MLIB_IMAGE_ONEDVECTOR) == 0) &&
				    ((dsize & X8) == 0)) {

					mlib_s_ImageThresh1B_S16_1_A8D1X8(sa,
					    da, dsize, th, hc, lc);
				} else if ((flags & A8D2X8) == 0 &&
				    dbit_off == 0) {

					mlib_s_ImageThresh1B_S16_1_A8D2X8(sa,
					    slb, da, dlb, xsize, ysize, th, hc,
					    lc);
				} else {

					mlib_s_ImageThresh1B_S16_1(sa, slb, da,
					    dlb, xsize, ysize, dbit_off, th, hc,
					    lc);
				}
			} else {
				if (((flags & A8D1) == 0) &&
				    ((flagd & MLIB_IMAGE_ONEDVECTOR) == 0)) {

					mlib_s_ImageThresh1B_S16_3_A8D1(sa, da,
					    dsize, th, hc, lc);
				} else if ((flags & A8D2) == 0 &&
						dbit_off == 0) {

					mlib_s_ImageThresh1B_S16_3_A8D2(sa, slb,
					    da, dlb, xsize, ysize, th, hc, lc);
				} else {

					mlib_s_ImageThresh1B_S16_3(sa, slb, da,
					    dlb, xsize, ysize, dbit_off, th, hc,
					    lc);
				}
			}
		} else if (stype == MLIB_USHORT) {

			if (nchan != 3) {
				if (((flags & A8D1) == 0) &&
				    ((flagd & MLIB_IMAGE_ONEDVECTOR) == 0) &&
				    ((dsize & X8) == 0)) {

					mlib_s_ImageThresh1B_U16_1_A8D1X8(sa,
					    da, dsize, th, hc, lc);
				} else if ((flags & A8D2X8) == 0 &&
				    dbit_off == 0) {

					mlib_s_ImageThresh1B_U16_1_A8D2X8(sa,
					    slb, da, dlb, xsize, ysize, th, hc,
					    lc);
				} else {

					mlib_s_ImageThresh1B_U16_1(sa, slb, da,
					    dlb, xsize, ysize, dbit_off, th, hc,
					    lc);
				}
			} else {
				if (((flags & A8D1) == 0) &&
				    ((flagd & MLIB_IMAGE_ONEDVECTOR) == 0)) {

					mlib_s_ImageThresh1B_U16_3_A8D1(sa, da,
					    dsize, th, hc, lc);
				} else if ((flags & A8D2) == 0 &&
						dbit_off == 0) {

					mlib_s_ImageThresh1B_U16_3_A8D2(sa, slb,
					    da, dlb, xsize, ysize, th, hc, lc);
				} else {

					mlib_s_ImageThresh1B_U16_3(sa, slb, da,
					    dlb, xsize, ysize, dbit_off, th, hc,
					    lc);
				}
			}
		} else if (stype == MLIB_INT) {

			if (nchan != 3) {
				if (((flags & A8D1) == 0) &&
				    ((flagd & MLIB_IMAGE_ONEDVECTOR) == 0) &&
				    ((dsize & X8) == 0)) {

					mlib_s_ImageThresh1B_S32_1_A8D1X8(sa,
					    da, dsize, th, hc, lc);
				} else if ((flags & A8D2X8) == 0 &&
				    dbit_off == 0) {

					mlib_s_ImageThresh1B_S32_1_A8D2X8(sa,
					    slb, da, dlb, xsize, ysize, th, hc,
					    lc);
				} else {

					mlib_s_ImageThresh1B_S32_1(sa, slb, da,
					    dlb, xsize, ysize, dbit_off, th, hc,
					    lc);
				}
			} else {
				if (((flags & A8D1) == 0) &&
				    ((flagd & MLIB_IMAGE_ONEDVECTOR) == 0)) {

					mlib_s_ImageThresh1B_S32_3_A8D1(sa, da,
					    dsize, th, hc, lc);
				} else if ((flags & A8D2) == 0 &&
						dbit_off == 0) {

					mlib_s_ImageThresh1B_S32_3_A8D2(sa, slb,
					    da, dlb, xsize, ysize, th, hc, lc);
				} else {

					mlib_s_ImageThresh1B_S32_3(sa, slb, da,
					    dlb, xsize, ysize, dbit_off, th, hc,
					    lc);
				}
			}
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#define	MLIB_S_IMAGETHRESH1B_U8(src, dst, th, hs, ls)       \
	sd = _mm_unpacklo_epi8(_mm_xor_si128(                   \
			_mm_loadl_epi64((__m128i *)src), mask80), zero);\
	sd0 = _mm_shufflelo_epi16(sd, _MM_SHUFFLE(0, 1, 2, 3)); \
	sd1 = _mm_shufflehi_epi16(sd, _MM_SHUFFLE(0, 1, 2, 3)); \
	sd = _mm_packus_epi16(_mm_unpacklo_epi64(               \
			_mm_srli_si128(sd1, 8), sd0), zero);            \
	sd = _mm_cmpgt_epi8(sd, th);                            \
	mask = _mm_cvtsi32_si128(_mm_movemask_epi8(sd));        \
	usd = (mlib_u8)_mm_cvtsi128_si32(                       \
			_mm_or_si128(_mm_and_si128(hs, mask),           \
			_mm_andnot_si128(mask, ls)));                   \
	*dst = usd;                                             \
	src++;                                                  \
	dst++

/* *********************************************************** */

#define	MLIB_S_IMAGETHRESH_EDGE_1B_U8(src, th, hs, ls)      \
	sd = _mm_loadu_si128((__m128i *)src);                   \
	sd = _mm_xor_si128(sd, mask80);                         \
	sd = _mm_unpacklo_epi8(sd, zero);                       \
	sd0 = _mm_shufflelo_epi16(sd, _MM_SHUFFLE(0, 1, 2, 3)); \
	sd1 = _mm_shufflehi_epi16(sd, _MM_SHUFFLE(0, 1, 2, 3)); \
	sd = _mm_packus_epi16(_mm_unpacklo_epi64(               \
			_mm_srli_si128(sd1, 8), sd0), zero);            \
	sd = _mm_cmpgt_epi8(sd, th);                            \
	mask = _mm_cvtsi32_si128(_mm_movemask_epi8(sd));        \
	usd = (mlib_u8)_mm_cvtsi128_si32(                       \
			_mm_or_si128(_mm_and_si128(hs, mask),           \
			_mm_andnot_si128(mask, ls)))

/* *********************************************************** */

void
mlib_s_ImageThresh1B_U8_1_A8D1X8(
    const mlib_u8 *src,
    mlib_u8 *dst,
    mlib_s32 dsize,
    const mlib_s32 *th,
    mlib_s32 hc,
    mlib_s32 lc)
{
/* aligned src address */
	__m64 *sp;

/* destination address */
	mlib_u8 *dp;

/* source data */
	__m128i sd, sd0, sd1;

/* destination data */
	mlib_u8 usd;

/* threshold data */
	__m128i thresh;
	__m128i hs, ls;

/* temporaries used in macro */
	__m128i mask80 = _mm_set1_epi32(0x80808080);
	const __m128i zero = _mm_set1_epi8(0x00);
	__m128i mask;

/* loop variables */
	mlib_s32 j;

/* prepare the threshold */
	thresh = _mm_set_epi8(
				th[0], th[1], th[2], th[3],
				th[0], th[1], th[2], th[3],
				th[0], th[1], th[2], th[3],
				th[0], th[1], th[2], th[3]);
	thresh = _mm_xor_si128(thresh, mask80);
	hs = _mm_cvtsi32_si128(hc);
	ls = _mm_cvtsi32_si128(lc);

	sp = (__m64 *)src;
	dp = (mlib_u8 *)dst;

	dsize >>= 3;

/* 8-pixel loop */
#ifdef  __SUNPRO_C
#pragma pipeloop(0)
#endif
	for (j = 0; j < dsize; j++) {
		MLIB_S_IMAGETHRESH1B_U8(sp, dp, thresh, hs, ls);
	}
}

/* *********************************************************** */

void
mlib_s_ImageThresh1B_U8_1_A8D2X8(
    const mlib_u8 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_s32 *th,
    mlib_s32 hc,
    mlib_s32 lc)
{
/* aligned src address */
	__m64 *sp;

/* pointer to begin of a src line */
	mlib_u8 *sl;

/* destination address */
	mlib_u8 *dp, *dl;

/* source data */
	__m128i sd, sd0, sd1;

/* destination data */
	mlib_u8 usd;

/* threshold data */
	__m128i thresh;
	__m128i hs, ls;

/* temporaries used in macro */
	__m128i mask80 = _mm_set1_epi32(0x80808080);
	const __m128i zero = _mm_set1_epi8(0x00);
	__m128i mask;

/* loop variables */
	mlib_s32 i, j;

/* prepare the threshold */
	thresh = _mm_set_epi8(
				th[0], th[1], th[2], th[3],
				th[0], th[1], th[2], th[3],
				th[0], th[1], th[2], th[3],
				th[0], th[1], th[2], th[3]);

	thresh = _mm_xor_si128(thresh, mask80);
	hs = _mm_cvtsi32_si128(hc);
	ls = _mm_cvtsi32_si128(lc);

	sl = (mlib_u8 *)src;
	dl = (mlib_u8 *)dst;

	xsize >>= 3;
/* row loop */
	for (j = 0; j < ysize; j++) {

		sp = (__m64 *)sl;
		dp = (mlib_u8 *)dl;
/* 8-pixel column loop */
#ifdef  __SUNPRO_C
#pragma pipeloop(0)
#endif
		for (i = 0; i < xsize; i++) {
			MLIB_S_IMAGETHRESH1B_U8(sp, dp, thresh, hs, ls);
		}

		sl += slb;
		dl += dlb;
	}
}

/* *********************************************************** */

void
mlib_s_ImageThresh1B_U8_1(
    const mlib_u8 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 dbit_off,
    const mlib_s32 *th,
    mlib_s32 hc,
    mlib_s32 lc)
{
/* aligned src address */
	__m64 *sp;

/* pointer to begin of a src line */
	mlib_u8 *sl;

/* destination address */
	mlib_u8 *dp;

/* pointer to begin of a dst line */
	mlib_u8 *dl;

/* source data */
	__m128i sd, sd0, sd1;

/* destination data */
	mlib_u8 usd;

/* threshold data */
	__m128i thresh;
	__m128i hs, ls;

/* temporaries used in macro */
	__m128i mask80 = _mm_set1_epi32(0x80808080);
	const __m128i zero = _mm_set1_epi8(0x0);
	__m128i mask;

/* edge mask */
	mlib_s32 emask0, emask1, nume;

/* loop variables */
	mlib_s32 i, j;

	thresh = _mm_set_epi8(
				th[0], th[1], th[2], th[3],
				th[0], th[1], th[2], th[3],
				th[0], th[1], th[2], th[3],
				th[0], th[1], th[2], th[3]);
	thresh = _mm_xor_si128(thresh, mask80);
	hs = _mm_cvtsi32_si128(hc);
	ls = _mm_cvtsi32_si128(lc);

	sl = (mlib_u8 *)src;
	dl = (mlib_u8 *)dst;

	nume = (8 - dbit_off);

	if (nume > xsize)
		nume = xsize;
	xsize -= nume;

	emask0 = (((0xFF00 >> nume) & 0xFF) >> dbit_off);
	emask1 = (0xFF << (8 - (xsize & 7))) & 0xFF;

	sl -= dbit_off;
	xsize >>= 3;
/* row loop */
	for (j = 0; j < ysize; j++) {
/* prepare source & destination address */
		sp = (__m64 *)sl;
		dp = (mlib_u8 *)dl;

		MLIB_S_IMAGETHRESH_EDGE_1B_U8(sp, thresh, hs, ls);
		dp[0] = (usd & emask0) | (dp[0] & ~emask0);
		sp++;
		dp++;

/* 8-pixel column loop */
#ifdef  __SUNPRO_C
#pragma pipeloop(0)
#endif
		for (i = 0; i < xsize; i++) {
			MLIB_S_IMAGETHRESH1B_U8(sp, dp, thresh, hs, ls);
		}

/* last 8-pixel column */

		if (emask1) {
			MLIB_S_IMAGETHRESH_EDGE_1B_U8(sp, thresh, hs, ls);
			dp[0] = (usd & emask1) | (dp[0] & ~emask1);
		}

		sl += slb;
		dl += dlb;
	}
}

/* *********************************************************** */

void
mlib_s_ImageThresh1B_U8_3_A8D1(
    const mlib_u8 *src,
    mlib_u8 *dst,
    mlib_s32 dsize,
    const mlib_s32 *th,
    mlib_s32 hc0,
    mlib_s32 lc0)
{
/* aligned src address */
	__m64 *sp;

/* destination address */
	mlib_u8 *dp;

/* source data */
	__m128i sd, sd0, sd1;

/* destination data */
	mlib_u8 usd;

/* threshold data */
	__m128i thresh0, thresh1, thresh2;

/* ghigh data */
	__m128i hs0, hs1, hs2;

/* glow data */
	__m128i ls0, ls1, ls2;

/* temporaries used in macro */
	__m128i mask;
	__m128i mask80 = _mm_set1_epi32(0x80808080);
	const __m128i zero = _mm_set1_epi8(0x00);

/* loop variables */
	mlib_s32 j;

/* prepare the threshold */

	thresh0 = _mm_set_epi8(
				0x0, 0x0, 0x0, 0x0,
				0x0, 0x0, 0x0, 0x0,
				th[0], th[1], th[2], th[0],
				th[1], th[2], th[0], th[1]);
	thresh1 = _mm_set_epi8(
				0x0, 0x0, 0x0, 0x0,
				0x0, 0x0, 0x0, 0x0,
				th[2], th[0], th[1], th[2],
				th[0], th[1], th[2], th[0]);
	thresh2 = _mm_set_epi8(
				0x0, 0x0, 0x0, 0x0,
				0x0, 0x0, 0x0, 0x0,
				th[1], th[2], th[0], th[1],
				th[2], th[0], th[1], th[2]);


	thresh0 = _mm_xor_si128(thresh0, mask80);
	thresh1 = _mm_xor_si128(thresh1, mask80);
	thresh2 = _mm_xor_si128(thresh2, mask80);

	hs0 = _mm_cvtsi32_si128(hc0);
	ls0 = _mm_cvtsi32_si128(lc0);
	hs1 = _mm_cvtsi32_si128(hc0 >> 1);
	ls1 = _mm_cvtsi32_si128(lc0 >> 1);
	hs2 = _mm_cvtsi32_si128(hc0 >> 2);
	ls2 = _mm_cvtsi32_si128(lc0 >> 2);

	sp = (__m64 *)src;
	dp = (mlib_u8 *)dst;

	dsize >>= 3;
/* 8-pixel column loop */
#ifdef  __SUNPRO_C
#pragma pipeloop(0)
#endif
	for (j = 0; j < dsize; j++) {
		MLIB_S_IMAGETHRESH1B_U8(sp, dp, thresh0, hs0, ls0);
		MLIB_S_IMAGETHRESH1B_U8(sp, dp, thresh1, hs1, ls1);
		MLIB_S_IMAGETHRESH1B_U8(sp, dp, thresh2, hs2, ls2);
	}
}

/* *********************************************************** */

void
mlib_s_ImageThresh1B_U8_3_A8D2(
    const mlib_u8 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_s32 *th,
    mlib_s32 hc0,
    mlib_s32 lc0)
{
/* aligned src address */
	__m64 *sp;

/* pointer to a line of src */
	mlib_u8 *sl;

/* pointer to end of src */
	mlib_u8 *send;

/* destination address */
	mlib_u8 *dp;

/* pointer to a line of dst */
	mlib_u8 *dl;

/* source data */
	__m128i sd, sd0, sd1;

/* destination data */
	mlib_u8 usd;

/* ghigh data */
	__m128i hs0, hs1, hs2;

/* glow data */
	__m128i ls0, ls1, ls2;

/* threshold data */
	__m128i thresh0, thresh1, thresh2;

/* temporaries used in macro */
	__m128i mask;
	__m128i mask80 = _mm_set1_epi32(0x80808080);
	const __m128i zero = _mm_set1_epi8(0x00);

/* edge mask */
	mlib_s32 esize;
	mlib_s32 emask;

/* loop variables */
	mlib_s32 i, j;

/* prepare the threshold */
	thresh0 = _mm_set_epi8(
				0x0, 0x0, 0x0, 0x0,
				0x0, 0x0, 0x0, 0x0,
				th[0], th[1], th[2], th[0],
				th[1], th[2], th[0], th[1]);
	thresh1 = _mm_set_epi8(
				0x0, 0x0, 0x0, 0x0,
				0x0, 0x0, 0x0, 0x0,
				th[2], th[0], th[1], th[2],
				th[0], th[1], th[2], th[0]);
	thresh2 = _mm_set_epi8(
				0x0, 0x0, 0x0, 0x0,
				0x0, 0x0, 0x0, 0x0,
				th[1], th[2], th[0], th[1],
				th[2], th[0], th[1], th[2]);

	thresh0 = _mm_xor_si128(thresh0, mask80);
	thresh1 = _mm_xor_si128(thresh1, mask80);
	thresh2 = _mm_xor_si128(thresh2, mask80);

	hs0 = _mm_cvtsi32_si128(hc0);
	ls0 = _mm_cvtsi32_si128(lc0);
	hs1 = _mm_cvtsi32_si128(hc0 >> 1);
	ls1 = _mm_cvtsi32_si128(lc0 >> 1);
	hs2 = _mm_cvtsi32_si128(hc0 >> 2);
	ls2 = _mm_cvtsi32_si128(lc0 >> 2);

	esize = 3 * (xsize & 7);
	emask = (0xFF << (8 - (esize & 7))) & 0xFF;

	sl = (mlib_u8 *)src;
	dl = (mlib_u8 *)dst;

	xsize >>= 3;
/* row loop */
	for (j = 0; j < ysize; j++) {

		sp = (__m64 *)sl;
		dp = (mlib_u8 *)dl;
/* 8-pixel column loop */
#ifdef  __SUNPRO_C
#pragma pipeloop(0)
#endif
		for (i = 0; i < xsize; i++) {
			MLIB_S_IMAGETHRESH1B_U8(sp, dp, thresh0, hs0, ls0);
			MLIB_S_IMAGETHRESH1B_U8(sp, dp, thresh1, hs1, ls1);
			MLIB_S_IMAGETHRESH1B_U8(sp, dp, thresh2, hs2, ls2);
		}

/* last 8-pixel column */
		if (esize) {
			MLIB_S_IMAGETHRESH_EDGE_1B_U8(
				sp, thresh0, hs0, ls0);
			sp++;

			if (esize >= 8) {
				dp[0] = usd;
				dp++;

				MLIB_S_IMAGETHRESH_EDGE_1B_U8(
					sp, thresh1, hs1, ls1);
				sp++;

				if (esize >= 16) {
					dp[0] = usd;
					dp++;

					MLIB_S_IMAGETHRESH_EDGE_1B_U8(
						sp, thresh2, hs2, ls2);
				}
			}

			dp[0] = (usd & emask) | (dp[0] & ~emask);
		}

		sl += slb;
		dl += dlb;
	}
}

/* *********************************************************** */

void
mlib_s_ImageThresh1B_U8_3(
    const mlib_u8 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 dbit_off,
    const mlib_s32 *th,
    mlib_s32 hc0,
    mlib_s32 lc0)
{
/* aligned src address */
	__m64 *sp;

/* pointer to a line of src */
	mlib_u8 *sl;

/* destination address */
	mlib_u8 *dp;

/* pointer to a line of dst */
	mlib_u8 *dl;

/* source data */
	__m128i sd, sd0, sd1;

/* destination data */
	mlib_u8 usd;

/* ghigh data */
	__m128i hs0, hs1, hs2;

/* glow data */
	__m128i ls0, ls1, ls2;

/* threshold data */
	__m128i thresh0, thresh1, thresh2;

/* temporaries used in macro */
	__m128i mask;
	__m128i mask80 = _mm_set1_epi32(0x80808080);
	const __m128i zero = _mm_set1_epi8(0x00);

/* temporaries */
	mlib_s32 esize, dsize = 3 * xsize;

/* edge mask */
	mlib_s32 emask0, emask1, nume;

/* loop variables */
	mlib_s32 i, j;

/* prepare the threshold */
	thresh0 = _mm_set_epi8(
				0x0, 0x0, 0x0, 0x0,
				0x0, 0x0, 0x0, 0x0,
				th[0], th[1], th[2], th[0],
				th[1], th[2], th[0], th[1]);
	thresh1 = _mm_set_epi8(
				0x0, 0x0, 0x0, 0x0,
				0x0, 0x0, 0x0, 0x0,
				th[2], th[0], th[1], th[2],
				th[0], th[1], th[2], th[0]);
	thresh2 = _mm_set_epi8(
				0x0, 0x0, 0x0, 0x0,
				0x0, 0x0, 0x0, 0x0,
				th[1], th[2], th[0], th[1],
				th[2], th[0], th[1], th[2]);

	thresh0 = _mm_xor_si128(thresh0, mask80);
	thresh1 = _mm_xor_si128(thresh1, mask80);
	thresh2 = _mm_xor_si128(thresh2, mask80);

	hs0 = _mm_cvtsi32_si128(hc0);
	ls0 = _mm_cvtsi32_si128(lc0);
	hs1 = _mm_cvtsi32_si128(hc0 >> 1);
	ls1 = _mm_cvtsi32_si128(lc0 >> 1);
	hs2 = _mm_cvtsi32_si128(hc0 >> 2);
	ls2 = _mm_cvtsi32_si128(lc0 >> 2);

	sl = (mlib_u8 *)src;
	dl = (mlib_u8 *)dst;

	nume = (8 - dbit_off);

	if (nume > dsize)
		nume = dsize;
	emask0 = (((0xFF00 >> nume) & 0xFF) >> dbit_off);

	esize = 3 * (xsize & 7) - nume;

	if (esize < 0) {
		xsize -= 8;
		esize += 24;
	}

	emask1 = (0xFF << (8 - (esize & 7))) & 0xFF;

	sl -= dbit_off;
	xsize >>= 3;

/* row loop */
	for (j = 0; j < ysize; j++) {
/* prepare source & destination addresses */
		sp = (__m64 *)sl;
		dp = (mlib_u8 *)dl;
		MLIB_S_IMAGETHRESH_EDGE_1B_U8(sp, thresh0, hs0, ls0);
		dp[0] = (usd & emask0) | (dp[0] & ~emask0);
		dp++;
		sp++;

/* 8-pixel column loop */
#ifdef  __SUNPRO_C
#pragma pipeloop(0)
#endif
		for (i = 0; i < xsize; i++) {
			MLIB_S_IMAGETHRESH1B_U8(sp, dp, thresh1, hs1, ls1);
			MLIB_S_IMAGETHRESH1B_U8(sp, dp, thresh2, hs2, ls2);
			MLIB_S_IMAGETHRESH1B_U8(sp, dp, thresh0, hs0, ls0);
		}

/* last 8-pixel column */
		if (esize) {
			MLIB_S_IMAGETHRESH_EDGE_1B_U8(
				sp, thresh1, hs1, ls1);
			sp++;

			if (esize >= 8) {
				dp[0] = usd;
				dp++;

				MLIB_S_IMAGETHRESH_EDGE_1B_U8(
					sp, thresh2, hs2, ls2);
				sp++;

				if (esize >= 16) {
					dp[0] = usd;
					dp++;

					MLIB_S_IMAGETHRESH_EDGE_1B_U8(
						sp, thresh0, hs0, ls0);
				}
			}

			dp[0] = (usd & emask1) | (dp[0] & ~emask1);
		}

		sl += slb;
		dl += dlb;
	}
}

/* *********************************************************** */

#define	MLIB_S_IMAGETHRESH1B_S16(src, dst, th, hs, ls)       \
	sd = _mm_loadu_si128((__m128i *)src);                   \
	sd0 = _mm_shufflelo_epi16(sd, _MM_SHUFFLE(0, 1, 2, 3)); \
	sd1 = _mm_shufflehi_epi16(sd, _MM_SHUFFLE(0, 1, 2, 3)); \
	sd = _mm_unpacklo_epi64(_mm_srli_si128(sd1, 8), sd0);   \
	sd = _mm_packus_epi16(_mm_srli_epi16(                   \
				_mm_cmpgt_epi16(sd, th), 8), zero);         \
	mask = _mm_cvtsi32_si128(_mm_movemask_epi8(sd));        \
	usd = (mlib_u8)_mm_cvtsi128_si32(                       \
			_mm_or_si128(_mm_and_si128(hs, mask),           \
			_mm_andnot_si128(mask, ls)));                   \
	*dst = usd;                                             \
	src++;                                                  \
	dst++

/* *********************************************************** */

#define	MLIB_S_IMAGETHRESH_EDGE_1B_S16(src, th, hs, ls)      \
	sd = _mm_loadu_si128((__m128i *)src);                   \
	sd0 = _mm_shufflelo_epi16(sd, _MM_SHUFFLE(0, 1, 2, 3)); \
	sd1 = _mm_shufflehi_epi16(sd, _MM_SHUFFLE(0, 1, 2, 3)); \
	sd = _mm_unpacklo_epi64(_mm_srli_si128(sd1, 8), sd0);   \
	sd = _mm_packus_epi16(_mm_srli_epi16(                   \
				_mm_cmpgt_epi16(sd, th), 8), zero);         \
	mask = _mm_cvtsi32_si128(_mm_movemask_epi8(sd));        \
	usd = (mlib_u8)_mm_cvtsi128_si32(                       \
			_mm_or_si128(_mm_and_si128(hs, mask),           \
			_mm_andnot_si128(mask, ls)));                   \

/* *********************************************************** */

void
mlib_s_ImageThresh1B_S16_1_A8D1X8(
    const mlib_s16 *src,
    mlib_u8 *dst,
    mlib_s32 dsize,
    const mlib_s32 *th,
    mlib_s32 hc,
    mlib_s32 lc)
{
/* aligned src address */
	__m128 *sp;

/* destination address */
	mlib_u8 *dp;

/* source data */
	__m128i sd, sd0, sd1;

/* destination data */
	mlib_u8 usd;

/* threshold data */
	__m128i thresh;
	__m128i hs, ls;

/* temporaries used in macro */
	const __m128i zero = _mm_set1_epi8(0x00);
	__m128i mask;

/* loop variables */
	mlib_s32 j;

/* prepare the threshold */
	thresh = _mm_set_epi16(
				th[0], th[1], th[2], th[3],
				th[0], th[1], th[2], th[3]);
	hs = _mm_cvtsi32_si128(hc);
	ls = _mm_cvtsi32_si128(lc);

	sp = (__m128 *)src;
	dp = (mlib_u8 *)dst;

	dsize >>= 3;

/* 8-pixel loop */
#ifdef  __SUNPRO_C
#pragma pipeloop(0)
#endif
	for (j = 0; j < dsize; j++) {
		MLIB_S_IMAGETHRESH1B_S16(sp, dp, thresh, hs, ls);
	}
}

/* *********************************************************** */

void
mlib_s_ImageThresh1B_S16_1_A8D2X8(
    const mlib_s16 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_s32 *th,
    mlib_s32 hc,
    mlib_s32 lc)
{
/* aligned src address */
	__m128i *sp;

/* pointer to begin of a src line */
	mlib_u8 *sl;

/* destination address */
	mlib_u8 *dp, *dl;

/* source data */
	__m128i sd, sd0, sd1;

/* destination data */
	mlib_u8 usd;

/* threshold data */
	__m128i thresh;
	__m128i hs, ls;

/* temporaries used in macro */
	const __m128i zero = _mm_set1_epi8(0x00);
	__m128i mask;

/* loop variables */
	mlib_s32 i, j;

/* prepare the threshold */
	thresh = _mm_set_epi16(
				th[0], th[1], th[2], th[3],
				th[0], th[1], th[2], th[3]);

	hs = _mm_cvtsi32_si128(hc);
	ls = _mm_cvtsi32_si128(lc);

	sl = (mlib_u8 *)src;
	dl = (mlib_u8 *)dst;

	xsize >>= 3;
/* row loop */
	for (j = 0; j < ysize; j++) {

		sp = (__m128i *)sl;
		dp = (mlib_u8 *)dl;
/* 8-pixel column loop */
#ifdef  __SUNPRO_C
#pragma pipeloop(0)
#endif
		for (i = 0; i < xsize; i++) {
			MLIB_S_IMAGETHRESH1B_S16(sp, dp, thresh, hs, ls);
		}

		sl += slb;
		dl += dlb;
	}
}

/* *********************************************************** */

void
mlib_s_ImageThresh1B_S16_1(
    const mlib_s16 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 dbit_off,
    const mlib_s32 *th,
    mlib_s32 hc,
    mlib_s32 lc)

{
/* aligned src address */
	__m128i *sp;

/* pointer to begin of a src line */
	mlib_u8 *sl;

/* destination address */
	mlib_u8 *dp;

/* pointer to begin of a dst line */
	mlib_u8 *dl;

/* source data */
	__m128i sd, sd0, sd1;

/* destination data */
	mlib_u8 usd;

/* threshold data */
	__m128i thresh;
	__m128i hs, ls;

/* temporaries used in macro */
	const __m128i zero = _mm_set1_epi8(0x0);
	__m128i mask;

/* edge mask */
	mlib_s32 emask0, emask1, nume;

/* loop variables */
	mlib_s32 i, j;

	thresh = _mm_set_epi16(
				th[0], th[1], th[2], th[3],
				th[0], th[1], th[2], th[3]);

	hs = _mm_cvtsi32_si128(hc);
	ls = _mm_cvtsi32_si128(lc);

	sl = (mlib_u8 *)src;
	dl = (mlib_u8 *)dst;

	nume = (8 - dbit_off);

	if (nume > xsize)
		nume = xsize;
	xsize -= nume;

	emask0 = (((0xFF00 >> nume) & 0xFF) >> dbit_off);
	emask1 = (0xFF << (8 - (xsize & 7))) & 0xFF;

	sl -= 2 * dbit_off;
	xsize >>= 3;
/* row loop */
	for (j = 0; j < ysize; j++) {
/* prepare source & destination address */
		sp = (__m128i *)sl;
		dp = (mlib_u8 *)dl;

		MLIB_S_IMAGETHRESH_EDGE_1B_S16(sp, thresh, hs, ls);
		dp[0] = (usd & emask0) | (dp[0] & ~emask0);
		sp++;
		dp++;

/* 8-pixel column loop */
#ifdef  __SUNPRO_C
#pragma pipeloop(0)
#endif
		for (i = 0; i < xsize; i++) {
			MLIB_S_IMAGETHRESH1B_S16(sp, dp, thresh, hs, ls);
		}

/* last 8-pixel column */

		if (emask1) {
			MLIB_S_IMAGETHRESH_EDGE_1B_S16(sp, thresh, hs, ls);
			dp[0] = (usd & emask1) | (dp[0] & ~emask1);
		}

		sl += slb;
		dl += dlb;
	}
}

/* *********************************************************** */

void
mlib_s_ImageThresh1B_S16_3_A8D1(
    const mlib_s16 *src,
    mlib_u8 *dst,
    mlib_s32 dsize,
    const mlib_s32 *th,
    mlib_s32 hc0,
    mlib_s32 lc0)
{
/* aligned src address */
	__m128i *sp;

/* destination address */
	mlib_u8 *dp;

/* source data */
	__m128i sd, sd0, sd1;

/* destination data */
	mlib_u8 usd;

/* threshold data */
	__m128i thresh0, thresh1, thresh2;

/* ghigh data */
	__m128i hs0, hs1, hs2;

/* glow data */
	__m128i ls0, ls1, ls2;

/* temporaries used in macro */
	__m128i mask;
	const __m128i zero = _mm_set1_epi8(0x00);

/* loop variables */
	mlib_s32 j;

/* prepare the threshold */

	thresh0 = _mm_set_epi16(
				th[0], th[1], th[2], th[0],
				th[1], th[2], th[0], th[1]);
	thresh1 = _mm_set_epi16(
				th[2], th[0], th[1], th[2],
				th[0], th[1], th[2], th[0]);
	thresh2 = _mm_set_epi16(
				th[1], th[2], th[0], th[1],
				th[2], th[0], th[1], th[2]);

	hs0 = _mm_cvtsi32_si128(hc0);
	ls0 = _mm_cvtsi32_si128(lc0);
	hs1 = _mm_cvtsi32_si128(hc0 >> 1);
	ls1 = _mm_cvtsi32_si128(lc0 >> 1);
	hs2 = _mm_cvtsi32_si128(hc0 >> 2);
	ls2 = _mm_cvtsi32_si128(lc0 >> 2);

	sp = (__m128i *)src;
	dp = (mlib_u8 *)dst;

	dsize >>= 3;
/* 8-pixel column loop */
#ifdef  __SUNPRO_C
#pragma pipeloop(0)
#endif
	for (j = 0; j < dsize; j++) {
		MLIB_S_IMAGETHRESH1B_S16(sp, dp, thresh0, hs0, ls0);
		MLIB_S_IMAGETHRESH1B_S16(sp, dp, thresh1, hs1, ls1);
		MLIB_S_IMAGETHRESH1B_S16(sp, dp, thresh2, hs2, ls2);
	}
}

/* *********************************************************** */

void
mlib_s_ImageThresh1B_S16_3_A8D2(
    const mlib_s16 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_s32 *th,
    mlib_s32 hc0,
    mlib_s32 lc0)
{
/* aligned src address */
	__m128i *sp;

/* pointer to a line of src */
	mlib_u8 *sl;

/* pointer to end of src */
	mlib_u8 *send;

/* destination address */
	mlib_u8 *dp;

/* pointer to a line of dst */
	mlib_u8 *dl;

/* source data */
	__m128i sd, sd0, sd1;

/* destination data */
	mlib_u8 usd;

/* ghigh data */
	__m128i hs0, hs1, hs2;

/* glow data */
	__m128i ls0, ls1, ls2;

/* threshold data */
	__m128i thresh0, thresh1, thresh2;

/* temporaries used in macro */
	__m128i mask;
	const __m128i zero = _mm_set1_epi8(0x00);

/* edge mask */
	mlib_s32 esize;
	mlib_s32 emask;

/* loop variables */
	mlib_s32 i, j;

/* prepare the threshold */
	thresh0 = _mm_set_epi16(
				th[0], th[1], th[2], th[0],
				th[1], th[2], th[0], th[1]);
	thresh1 = _mm_set_epi16(
				th[2], th[0], th[1], th[2],
				th[0], th[1], th[2], th[0]);
	thresh2 = _mm_set_epi16(
				th[1], th[2], th[0], th[1],
				th[2], th[0], th[1], th[2]);

	hs0 = _mm_cvtsi32_si128(hc0);
	ls0 = _mm_cvtsi32_si128(lc0);
	hs1 = _mm_cvtsi32_si128(hc0 >> 1);
	ls1 = _mm_cvtsi32_si128(lc0 >> 1);
	hs2 = _mm_cvtsi32_si128(hc0 >> 2);
	ls2 = _mm_cvtsi32_si128(lc0 >> 2);

	esize = 3 * (xsize & 7);
	emask = (0xFF << (8 - (esize & 7))) & 0xFF;

	sl = (mlib_u8 *)src;
	dl = (mlib_u8 *)dst;

	xsize >>= 3;
/* row loop */
	for (j = 0; j < ysize; j++) {

		sp = (__m128i *)sl;
		dp = (mlib_u8 *)dl;
/* 8-pixel column loop */
#ifdef  __SUNPRO_C
#pragma pipeloop(0)
#endif
		for (i = 0; i < xsize; i++) {
			MLIB_S_IMAGETHRESH1B_S16(sp, dp, thresh0, hs0, ls0);
			MLIB_S_IMAGETHRESH1B_S16(sp, dp, thresh1, hs1, ls1);
			MLIB_S_IMAGETHRESH1B_S16(sp, dp, thresh2, hs2, ls2);
		}

/* last 8-pixel column */
		if (esize) {
			MLIB_S_IMAGETHRESH_EDGE_1B_S16(
				sp, thresh0, hs0, ls0);
			sp++;

			if (esize >= 8) {
				dp[0] = usd;
				dp++;

				MLIB_S_IMAGETHRESH_EDGE_1B_S16(
					sp, thresh1, hs1, ls1);
				sp++;

				if (esize >= 16) {
					dp[0] = usd;
					dp++;

					MLIB_S_IMAGETHRESH_EDGE_1B_S16(
						sp, thresh2, hs2, ls2);
				}
			}

			dp[0] = (usd & emask) | (dp[0] & ~emask);
		}

		sl += slb;
		dl += dlb;
	}
}

/* *********************************************************** */

void
mlib_s_ImageThresh1B_S16_3(
    const mlib_s16 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 dbit_off,
    const mlib_s32 *th,
    mlib_s32 hc0,
    mlib_s32 lc0)
{
/* aligned src address */
	__m128i *sp;

/* pointer to a line of src */
	mlib_u8 *sl;

/* destination address */
	mlib_u8 *dp;

/* pointer to a line of dst */
	mlib_u8 *dl;

/* source data */
	__m128i sd, sd0, sd1;

/* destination data */
	mlib_u8 usd;

/* ghigh data */
	__m128i hs0, hs1, hs2;

/* glow data */
	__m128i ls0, ls1, ls2;

/* threshold data */
	__m128i thresh0, thresh1, thresh2;

/* temporaries used in macro */
	__m128i mask;
	const __m128i zero = _mm_set1_epi8(0x00);

/* temporaries */
	mlib_s32 esize, dsize = 3 * xsize;

/* edge mask */
	mlib_s32 emask0, emask1, nume;

/* loop variables */
	mlib_s32 i, j;

/* prepare the threshold */
	thresh0 = _mm_set_epi16(
				th[0], th[1], th[2], th[0],
				th[1], th[2], th[0], th[1]);
	thresh1 = _mm_set_epi16(
				th[2], th[0], th[1], th[2],
				th[0], th[1], th[2], th[0]);
	thresh2 = _mm_set_epi16(
				th[1], th[2], th[0], th[1],
				th[2], th[0], th[1], th[2]);

	hs0 = _mm_cvtsi32_si128(hc0);
	ls0 = _mm_cvtsi32_si128(lc0);
	hs1 = _mm_cvtsi32_si128(hc0 >> 1);
	ls1 = _mm_cvtsi32_si128(lc0 >> 1);
	hs2 = _mm_cvtsi32_si128(hc0 >> 2);
	ls2 = _mm_cvtsi32_si128(lc0 >> 2);

	sl = (mlib_u8 *)src;
	dl = (mlib_u8 *)dst;

	nume = (8 - dbit_off);

	if (nume > dsize)
		nume = dsize;
	emask0 = (((0xFF00 >> nume) & 0xFF) >> dbit_off);

	esize = 3 * (xsize & 7) - nume;

	if (esize < 0) {
		xsize -= 8;
		esize += 24;
	}

	emask1 = (0xFF << (8 - (esize & 7))) & 0xFF;

	sl -= 2 * dbit_off;
	xsize >>= 3;

/* row loop */
	for (j = 0; j < ysize; j++) {
/* prepare source & destination addresses */
		sp = (__m128i *)sl;
		dp = (mlib_u8 *)dl;
		MLIB_S_IMAGETHRESH_EDGE_1B_S16(sp, thresh0, hs0, ls0);
		dp[0] = (usd & emask0) | (dp[0] & ~emask0);
		dp++;
		sp++;

/* 8-pixel column loop */
#ifdef  __SUNPRO_C
#pragma pipeloop(0)
#endif
		for (i = 0; i < xsize; i++) {
			MLIB_S_IMAGETHRESH1B_S16(sp, dp, thresh1, hs1, ls1);
			MLIB_S_IMAGETHRESH1B_S16(sp, dp, thresh2, hs2, ls2);
			MLIB_S_IMAGETHRESH1B_S16(sp, dp, thresh0, hs0, ls0);
		}

/* last 8-pixel column */
		if (esize) {
			MLIB_S_IMAGETHRESH_EDGE_1B_S16(
				sp, thresh1, hs1, ls1);
			sp++;

			if (esize >= 8) {
				dp[0] = usd;
				dp++;

				MLIB_S_IMAGETHRESH_EDGE_1B_S16(
					sp, thresh2, hs2, ls2);
				sp++;

				if (esize >= 16) {
					dp[0] = usd;
					dp++;

					MLIB_S_IMAGETHRESH_EDGE_1B_S16(
						sp, thresh0, hs0, ls0);
				}
			}

			dp[0] = (usd & emask1) | (dp[0] & ~emask1);
		}

		sl += slb;
		dl += dlb;
	}
}

/* *********************************************************** */

#define	MLIB_S_IMAGETHRESH1B_U16(src, dst, th, hs, ls)       \
	sd = _mm_xor_si128(                                     \
		_mm_loadu_si128((__m128i *)src), mask8000);         \
	sd0 = _mm_shufflelo_epi16(sd, _MM_SHUFFLE(0, 1, 2, 3)); \
	sd1 = _mm_shufflehi_epi16(sd, _MM_SHUFFLE(0, 1, 2, 3)); \
	sd = _mm_unpacklo_epi64(_mm_srli_si128(sd1, 8), sd0);   \
    sd = _mm_packus_epi16(_mm_srli_epi16(                   \
				_mm_cmpgt_epi16(sd, th), 8), zero);         \
    mask = _mm_cvtsi32_si128(_mm_movemask_epi8(sd));        \
    usd = (mlib_u8)_mm_cvtsi128_si32(                       \
			_mm_or_si128(_mm_and_si128(hs, mask),           \
			_mm_andnot_si128(mask, ls)));                   \
	*dst = usd;                                             \
	src++;                                                  \
	dst++

/* *********************************************************** */

#define	MLIB_S_IMAGETHRESH_EDGE_1B_U16(src, th, hs, ls)      \
	sd = _mm_xor_si128(                                     \
		_mm_loadu_si128((__m128i *)src), mask8000);         \
	sd0 = _mm_shufflelo_epi16(sd, _MM_SHUFFLE(0, 1, 2, 3)); \
	sd1 = _mm_shufflehi_epi16(sd, _MM_SHUFFLE(0, 1, 2, 3)); \
	sd = _mm_unpacklo_epi64(_mm_srli_si128(sd1, 8), sd0);   \
    sd = _mm_packus_epi16(_mm_srli_epi16(                   \
				_mm_cmpgt_epi16(sd, th), 8), zero);         \
    mask = _mm_cvtsi32_si128(_mm_movemask_epi8(sd));        \
    usd = (mlib_u8)_mm_cvtsi128_si32(                       \
			_mm_or_si128(_mm_and_si128(hs, mask),           \
			_mm_andnot_si128(mask, ls)));                   \

/* *********************************************************** */

void
mlib_s_ImageThresh1B_U16_1_A8D1X8(
    const mlib_s16 *src,
    mlib_u8 *dst,
    mlib_s32 dsize,
    const mlib_s32 *th,
    mlib_s32 hc,
    mlib_s32 lc)
{
/* aligned src address */
	__m128 *sp;

/* destination address */
	mlib_u8 *dp;

/* source data */
	__m128i sd, sd0, sd1;

/* destination data */
	mlib_u8 usd;

/* threshold data */
	__m128i thresh;
	__m128i hs, ls;

/* temporaries used in macro */
	const __m128i zero = _mm_set1_epi8(0x00);
	__m128i mask8000 = _mm_set1_epi32(0x80008000);
	__m128i mask;

/* loop variables */
	mlib_s32 j;

/* prepare the threshold */
	thresh = _mm_set_epi16(
				th[0], th[1], th[2], th[3],
				th[0], th[1], th[2], th[3]);
	thresh = _mm_xor_si128(thresh, mask8000);

	hs = _mm_cvtsi32_si128(hc);
	ls = _mm_cvtsi32_si128(lc);

	sp = (__m128 *)src;
	dp = (mlib_u8 *)dst;

	dsize >>= 3;

/* 8-pixel loop */
#ifdef  __SUNPRO_C
#pragma pipeloop(0)
#endif
	for (j = 0; j < dsize; j++) {
		MLIB_S_IMAGETHRESH1B_U16(sp, dp, thresh, hs, ls);
	}
}

/* *********************************************************** */

void
mlib_s_ImageThresh1B_U16_1_A8D2X8(
    const mlib_s16 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_s32 *th,
    mlib_s32 hc,
    mlib_s32 lc)
{
/* aligned src address */
	__m128i *sp;

/* pointer to begin of a src line */
	mlib_u8 *sl;

/* destination address */
	mlib_u8 *dp, *dl;

/* source data */
	__m128i sd, sd0, sd1;

/* destination data */
	mlib_u8 usd;

/* threshold data */
	__m128i thresh;
	__m128i hs, ls;

/* temporaries used in macro */
	const __m128i zero = _mm_set1_epi8(0x00);
	__m128i mask8000 = _mm_set1_epi32(0x80008000);
	__m128i mask;

/* loop variables */
	mlib_s32 i, j;

/* prepare the threshold */
	thresh = _mm_set_epi16(
				th[0], th[1], th[2], th[3],
				th[0], th[1], th[2], th[3]);
	thresh = _mm_xor_si128(thresh, mask8000);

	hs = _mm_cvtsi32_si128(hc);
	ls = _mm_cvtsi32_si128(lc);

	sl = (mlib_u8 *)src;
	dl = (mlib_u8 *)dst;

	xsize >>= 3;
/* row loop */
	for (j = 0; j < ysize; j++) {

		sp = (__m128i *)sl;
		dp = (mlib_u8 *)dl;
/* 8-pixel column loop */
#ifdef  __SUNPRO_C
#pragma pipeloop(0)
#endif
		for (i = 0; i < xsize; i++) {
			MLIB_S_IMAGETHRESH1B_U16(sp, dp, thresh, hs, ls);
		}

		sl += slb;
		dl += dlb;
	}
}

/* *********************************************************** */

void
mlib_s_ImageThresh1B_U16_1(
    const mlib_s16 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 dbit_off,
    const mlib_s32 *th,
    mlib_s32 hc,
    mlib_s32 lc)
{
/* aligned src address */
	__m128i *sp;

/* pointer to begin of a src line */
	mlib_u8 *sl;

/* destination address */
	mlib_u8 *dp;

/* pointer to begin of a dst line */
	mlib_u8 *dl;

/* source data */
	__m128i sd, sd0, sd1;

/* destination data */
	mlib_u8 usd;

/* threshold data */
	__m128i thresh;
	__m128i hs, ls;

/* temporaries used in macro */
	const __m128i zero = _mm_set1_epi8(0x0);
	__m128i mask8000 = _mm_set1_epi32(0x80008000);
	__m128i mask;

/* edge mask */
	mlib_s32 emask0, emask1, nume;

/* loop variables */
	mlib_s32 i, j;

	thresh = _mm_set_epi16(
				th[0], th[1], th[2], th[3],
				th[0], th[1], th[2], th[3]);
	thresh = _mm_xor_si128(thresh, mask8000);

	hs = _mm_cvtsi32_si128(hc);
	ls = _mm_cvtsi32_si128(lc);

	sl = (mlib_u8 *)src;
	dl = (mlib_u8 *)dst;

	nume = (8 - dbit_off);

	if (nume > xsize)
		nume = xsize;
	xsize -= nume;

	emask0 = (((0xFF00 >> nume) & 0xFF) >> dbit_off);
	emask1 = (0xFF << (8 - (xsize & 7))) & 0xFF;

	sl -= 2 * dbit_off;
	xsize >>= 3;
/* row loop */
	for (j = 0; j < ysize; j++) {
/* prepare source & destination address */
		sp = (__m128i *)sl;
		dp = (mlib_u8 *)dl;

		MLIB_S_IMAGETHRESH_EDGE_1B_U16(sp, thresh, hs, ls);
		dp[0] = (usd & emask0) | (dp[0] & ~emask0);
		sp++;
		dp++;

/* 8-pixel column loop */
#ifdef  __SUNPRO_C
#pragma pipeloop(0)
#endif
		for (i = 0; i < xsize; i++) {
			MLIB_S_IMAGETHRESH1B_U16(sp, dp, thresh, hs, ls);
		}

/* last 8-pixel column */

		if (emask1) {
			MLIB_S_IMAGETHRESH_EDGE_1B_U16(sp, thresh, hs, ls);
			dp[0] = (usd & emask1) | (dp[0] & ~emask1);
		}

		sl += slb;
		dl += dlb;
	}
}

/* *********************************************************** */

void
mlib_s_ImageThresh1B_U16_3_A8D1(
    const mlib_s16 *src,
    mlib_u8 *dst,
    mlib_s32 dsize,
    const mlib_s32 *th,
    mlib_s32 hc0,
    mlib_s32 lc0)
{
/* aligned src address */
	__m128i *sp;

/* destination address */
	mlib_u8 *dp;

/* source data */
	__m128i sd, sd0, sd1;

/* destination data */
	mlib_u8 usd;

/* threshold data */
	__m128i thresh0, thresh1, thresh2;

/* ghigh data */
	__m128i hs0, hs1, hs2;

/* glow data */
	__m128i ls0, ls1, ls2;

/* temporaries used in macro */
	__m128i mask;
	const __m128i zero = _mm_set1_epi8(0x00);
	__m128i mask8000 = _mm_set1_epi32(0x80008000);

/* loop variables */
	mlib_s32 j;

/* prepare the threshold */

	thresh0 = _mm_set_epi16(
				th[0], th[1], th[2], th[0],
				th[1], th[2], th[0], th[1]);
	thresh1 = _mm_set_epi16(
				th[2], th[0], th[1], th[2],
				th[0], th[1], th[2], th[0]);
	thresh2 = _mm_set_epi16(
				th[1], th[2], th[0], th[1],
				th[2], th[0], th[1], th[2]);

	thresh0 = _mm_xor_si128(thresh0, mask8000);
	thresh1 = _mm_xor_si128(thresh1, mask8000);
	thresh2 = _mm_xor_si128(thresh2, mask8000);

	hs0 = _mm_cvtsi32_si128(hc0);
	ls0 = _mm_cvtsi32_si128(lc0);
	hs1 = _mm_cvtsi32_si128(hc0 >> 1);
	ls1 = _mm_cvtsi32_si128(lc0 >> 1);
	hs2 = _mm_cvtsi32_si128(hc0 >> 2);
	ls2 = _mm_cvtsi32_si128(lc0 >> 2);

	sp = (__m128i *)src;
	dp = (mlib_u8 *)dst;

	dsize >>= 3;
/* 8-pixel column loop */
#ifdef  __SUNPRO_C
#pragma pipeloop(0)
#endif
	for (j = 0; j < dsize; j++) {
		MLIB_S_IMAGETHRESH1B_U16(sp, dp, thresh0, hs0, ls0);
		MLIB_S_IMAGETHRESH1B_U16(sp, dp, thresh1, hs1, ls1);
		MLIB_S_IMAGETHRESH1B_U16(sp, dp, thresh2, hs2, ls2);
	}
}

/* *********************************************************** */

void
mlib_s_ImageThresh1B_U16_3_A8D2(
    const mlib_s16 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_s32 *th,
    mlib_s32 hc0,
    mlib_s32 lc0)
{
/* aligned src address */
	__m128i *sp;

/* pointer to a line of src */
	mlib_u8 *sl;

/* pointer to end of src */
	mlib_u8 *send;

/* destination address */
	mlib_u8 *dp;

/* pointer to a line of dst */
	mlib_u8 *dl;

/* source data */
	__m128i sd, sd0, sd1;

/* destination data */
	mlib_u8 usd;

/* ghigh data */
	__m128i hs0, hs1, hs2;

/* glow data */
	__m128i ls0, ls1, ls2;

/* threshold data */
	__m128i thresh0, thresh1, thresh2;

/* temporaries used in macro */
	__m128i mask;
	const __m128i zero = _mm_set1_epi8(0x00);
	__m128i mask8000 = _mm_set1_epi32(0x80008000);

/* edge mask */
	mlib_s32 esize;
	mlib_s32 emask;

/* loop variables */
	mlib_s32 i, j;

/* prepare the threshold */
	thresh0 = _mm_set_epi16(
				th[0], th[1], th[2], th[0],
				th[1], th[2], th[0], th[1]);
	thresh1 = _mm_set_epi16(
				th[2], th[0], th[1], th[2],
				th[0], th[1], th[2], th[0]);
	thresh2 = _mm_set_epi16(
				th[1], th[2], th[0], th[1],
				th[2], th[0], th[1], th[2]);

	thresh0 = _mm_xor_si128(thresh0, mask8000);
	thresh1 = _mm_xor_si128(thresh1, mask8000);
	thresh2 = _mm_xor_si128(thresh2, mask8000);

	hs0 = _mm_cvtsi32_si128(hc0);
	ls0 = _mm_cvtsi32_si128(lc0);
	hs1 = _mm_cvtsi32_si128(hc0 >> 1);
	ls1 = _mm_cvtsi32_si128(lc0 >> 1);
	hs2 = _mm_cvtsi32_si128(hc0 >> 2);
	ls2 = _mm_cvtsi32_si128(lc0 >> 2);

	esize = 3 * (xsize & 7);
	emask = (0xFF << (8 - (esize & 7))) & 0xFF;

	sl = (mlib_u8 *)src;
	dl = (mlib_u8 *)dst;

	xsize >>= 3;
/* row loop */
	for (j = 0; j < ysize; j++) {

		sp = (__m128i *)sl;
		dp = (mlib_u8 *)dl;
/* 8-pixel column loop */
#ifdef  __SUNPRO_C
#pragma pipeloop(0)
#endif
		for (i = 0; i < xsize; i++) {
			MLIB_S_IMAGETHRESH1B_U16(sp, dp, thresh0, hs0, ls0);
			MLIB_S_IMAGETHRESH1B_U16(sp, dp, thresh1, hs1, ls1);
			MLIB_S_IMAGETHRESH1B_U16(sp, dp, thresh2, hs2, ls2);
		}

/* last 8-pixel column */
		if (esize) {
			MLIB_S_IMAGETHRESH_EDGE_1B_U16(
				sp, thresh0, hs0, ls0);
			sp++;

			if (esize >= 8) {
				dp[0] = usd;
				dp++;

				MLIB_S_IMAGETHRESH_EDGE_1B_U16(
					sp, thresh1, hs1, ls1);
				sp++;

				if (esize >= 16) {
					dp[0] = usd;
					dp++;

					MLIB_S_IMAGETHRESH_EDGE_1B_U16(
						sp, thresh2, hs2, ls2);
				}
			}

			dp[0] = (usd & emask) | (dp[0] & ~emask);
		}

		sl += slb;
		dl += dlb;
	}
}

/* *********************************************************** */

void
mlib_s_ImageThresh1B_U16_3(
    const mlib_s16 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 dbit_off,
    const mlib_s32 *th,
    mlib_s32 hc0,
    mlib_s32 lc0)
{
/* aligned src address */
	__m128i *sp;

/* pointer to a line of src */
	mlib_u8 *sl;

/* destination address */
	mlib_u8 *dp;

/* pointer to a line of dst */
	mlib_u8 *dl;

/* source data */
	__m128i sd, sd0, sd1;

/* destination data */
	mlib_u8 usd;

/* ghigh data */
	__m128i hs0, hs1, hs2;

/* glow data */
	__m128i ls0, ls1, ls2;

/* threshold data */
	__m128i thresh0, thresh1, thresh2;

/* temporaries used in macro */
	__m128i mask;
	const __m128i zero = _mm_set1_epi8(0x00);
	__m128i mask8000 = _mm_set1_epi32(0x80008000);

/* temporaries */
	mlib_s32 esize, dsize = 3 * xsize;

/* edge mask */
	mlib_s32 emask0, emask1, nume;

/* loop variables */
	mlib_s32 i, j;

/* prepare the threshold */
	thresh0 = _mm_set_epi16(
				th[0], th[1], th[2], th[0],
				th[1], th[2], th[0], th[1]);
	thresh1 = _mm_set_epi16(
				th[2], th[0], th[1], th[2],
				th[0], th[1], th[2], th[0]);
	thresh2 = _mm_set_epi16(
				th[1], th[2], th[0], th[1],
				th[2], th[0], th[1], th[2]);

	thresh0 = _mm_xor_si128(thresh0, mask8000);
	thresh1 = _mm_xor_si128(thresh1, mask8000);
	thresh2 = _mm_xor_si128(thresh2, mask8000);

	hs0 = _mm_cvtsi32_si128(hc0);
	ls0 = _mm_cvtsi32_si128(lc0);
	hs1 = _mm_cvtsi32_si128(hc0 >> 1);
	ls1 = _mm_cvtsi32_si128(lc0 >> 1);
	hs2 = _mm_cvtsi32_si128(hc0 >> 2);
	ls2 = _mm_cvtsi32_si128(lc0 >> 2);

	sl = (mlib_u8 *)src;
	dl = (mlib_u8 *)dst;

	nume = (8 - dbit_off);

	if (nume > dsize)
		nume = dsize;
	emask0 = (((0xFF00 >> nume) & 0xFF) >> dbit_off);

	esize = 3 * (xsize & 7) - nume;

	if (esize < 0) {
		xsize -= 8;
		esize += 24;
	}

	emask1 = (0xFF << (8 - (esize & 7))) & 0xFF;

	sl -= 2 * dbit_off;
	xsize >>= 3;

/* row loop */
	for (j = 0; j < ysize; j++) {
/* prepare source & destination addresses */
		sp = (__m128i *)sl;
		dp = (mlib_u8 *)dl;
		MLIB_S_IMAGETHRESH_EDGE_1B_U16(sp, thresh0, hs0, ls0);
		dp[0] = (usd & emask0) | (dp[0] & ~emask0);
		dp++;
		sp++;

/* 8-pixel column loop */
#ifdef  __SUNPRO_C
#pragma pipeloop(0)
#endif
		for (i = 0; i < xsize; i++) {
			MLIB_S_IMAGETHRESH1B_U16(sp, dp, thresh1, hs1, ls1);
			MLIB_S_IMAGETHRESH1B_U16(sp, dp, thresh2, hs2, ls2);
			MLIB_S_IMAGETHRESH1B_U16(sp, dp, thresh0, hs0, ls0);
		}

/* last 8-pixel column */
		if (esize) {
			MLIB_S_IMAGETHRESH_EDGE_1B_U16(
				sp, thresh1, hs1, ls1);
			sp++;

			if (esize >= 8) {
				dp[0] = usd;
				dp++;

				MLIB_S_IMAGETHRESH_EDGE_1B_U16(
					sp, thresh2, hs2, ls2);
				sp++;

				if (esize >= 16) {
					dp[0] = usd;
					dp++;

					MLIB_S_IMAGETHRESH_EDGE_1B_U16(
						sp, thresh0, hs0, ls0);
				}
			}

			dp[0] = (usd & emask1) | (dp[0] & ~emask1);
		}

		sl += slb;
		dl += dlb;
	}
}

/* *********************************************************** */

#define	MLIB_S_IMAGETHRESH1B_S32(src, dst, th0, th1, hs, ls)  \
	sd = _mm_loadu_si128((__m128i *)src);                   \
	sd0 = _mm_shuffle_epi32(sd, _MM_SHUFFLE(0, 1, 2, 3));   \
	sd = _mm_loadu_si128((__m128i *)src + 1);               \
	sd1 = _mm_shuffle_epi32(sd, _MM_SHUFFLE(0, 1, 2, 3));   \
	sd0 = _mm_srli_epi32(_mm_cmpgt_epi32(sd0, th0), 24);    \
	sd1 = _mm_srli_epi32(_mm_cmpgt_epi32(sd1, th1), 24);    \
	sd = _mm_packs_epi32(sd1, sd0);                         \
	sd = _mm_packus_epi16(sd, zero);                        \
	mask = _mm_cvtsi32_si128(_mm_movemask_epi8(sd));        \
	usd = (mlib_u8)_mm_cvtsi128_si32(                       \
			_mm_or_si128(_mm_and_si128(hs, mask),           \
			_mm_andnot_si128(mask, ls)));                   \
	*dst = usd;                                             \
	src += 2;                                               \
	dst++

/* *********************************************************** */

#define	MLIB_S_IMAGETHRESH_EDGE_1B_S32(src, th0, th1, hs, ls)   \
	sd = _mm_loadu_si128((__m128i *)src);                   \
	sd0 = _mm_shuffle_epi32(sd, _MM_SHUFFLE(0, 1, 2, 3));   \
	sd = _mm_loadu_si128((__m128i *)src + 1);               \
	sd1 = _mm_shuffle_epi32(sd, _MM_SHUFFLE(0, 1, 2, 3));   \
	sd0 = _mm_srli_epi32(_mm_cmpgt_epi32(sd0, th0), 24);    \
	sd1 = _mm_srli_epi32(_mm_cmpgt_epi32(sd1, th1), 24);    \
	sd = _mm_packs_epi32(sd1, sd0);                         \
	sd = _mm_packus_epi16(sd, zero);                        \
	mask = _mm_cvtsi32_si128(_mm_movemask_epi8(sd));        \
	usd = (mlib_u8)_mm_cvtsi128_si32(                       \
			_mm_or_si128(_mm_and_si128(hs, mask),           \
			_mm_andnot_si128(mask, ls)));                   \

/* *********************************************************** */


void
mlib_s_ImageThresh1B_S32_1_A8D1X8(
    const mlib_s32 *src,
    mlib_u8 *dst,
    mlib_s32 dsize,
    const mlib_s32 *th,
    mlib_s32 hc,
    mlib_s32 lc)
{
/* aligned src address */
	__m128 *sp;

/* destination address */
	mlib_u8 *dp;

/* source data */
	__m128i sd, sd0, sd1;

/* destination data */
	mlib_u8 usd;

/* threshold data */
	__m128i thresh;
	__m128i hs, ls;

/* temporaries used in macro */
	const __m128i zero = _mm_set1_epi8(0x00);
	__m128i mask;

/* loop variables */
	mlib_s32 j;

/* prepare the threshold */
	thresh = _mm_set_epi32(
				th[0], th[1], th[2], th[3]);
	hs = _mm_cvtsi32_si128(hc);
	ls = _mm_cvtsi32_si128(lc);

	sp = (__m128 *)src;
	dp = (mlib_u8 *)dst;

	dsize >>= 3;

/* 8-pixel loop */
#ifdef  __SUNPRO_C
#pragma pipeloop(0)
#endif
	for (j = 0; j < dsize; j++) {
		MLIB_S_IMAGETHRESH1B_S32(
			sp, dp, thresh, thresh, hs, ls);
	}
}

/* *********************************************************** */

void
mlib_s_ImageThresh1B_S32_1_A8D2X8(
    const mlib_s32 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_s32 *th,
    mlib_s32 hc,
    mlib_s32 lc)
{
/* aligned src address */
	__m128i *sp;

/* pointer to begin of a src line */
	mlib_u8 *sl;

/* destination address */
	mlib_u8 *dp, *dl;

/* source data */
	__m128i sd, sd0, sd1;

/* destination data */
	mlib_u8 usd;

/* threshold data */
	__m128i thresh;
	__m128i hs, ls;

/* temporaries used in macro */
	const __m128i zero = _mm_set1_epi8(0x00);
	__m128i mask;

/* loop variables */
	mlib_s32 i, j;

/* prepare the threshold */
	thresh = _mm_set_epi32(
				th[0], th[1], th[2], th[3]);

	hs = _mm_cvtsi32_si128(hc);
	ls = _mm_cvtsi32_si128(lc);

	sl = (mlib_u8 *)src;
	dl = (mlib_u8 *)dst;

	xsize >>= 3;
/* row loop */
	for (j = 0; j < ysize; j++) {

		sp = (__m128i *)sl;
		dp = (mlib_u8 *)dl;
/* 8-pixel column loop */
#ifdef  __SUNPRO_C
#pragma pipeloop(0)
#endif
		for (i = 0; i < xsize; i++) {
			MLIB_S_IMAGETHRESH1B_S32(
				sp, dp, thresh, thresh, hs, ls);
		}

		sl += slb;
		dl += dlb;
	}
}

/* *********************************************************** */

void
mlib_s_ImageThresh1B_S32_1(
    const mlib_s32 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 dbit_off,
    const mlib_s32 *th,
    mlib_s32 hc,
    mlib_s32 lc)
{
/* aligned src address */
	__m128i *sp;

/* pointer to begin of a src line */
	mlib_u8 *sl;

/* destination address */
	mlib_u8 *dp;

/* pointer to begin of a dst line */
	mlib_u8 *dl;

/* source data */
	__m128i sd, sd0, sd1;

/* destination data */
	mlib_u8 usd;

/* threshold data */
	__m128i thresh;
	__m128i hs, ls;

/* temporaries used in macro */
	const __m128i zero = _mm_set1_epi8(0x0);
	__m128i mask;

/* edge mask */
	mlib_s32 emask0, emask1, nume;

/* loop variables */
	mlib_s32 i, j;

	thresh = _mm_set_epi32(
				th[0], th[1], th[2], th[3]);

	hs = _mm_cvtsi32_si128(hc);
	ls = _mm_cvtsi32_si128(lc);

	sl = (mlib_u8 *)src;
	dl = (mlib_u8 *)dst;

	nume = (8 - dbit_off);

	if (nume > xsize)
		nume = xsize;
	xsize -= nume;

	emask0 = (((0xFF00 >> nume) & 0xFF) >> dbit_off);
	emask1 = (0xFF << (8 - (xsize & 7))) & 0xFF;

	sl -= 4 * dbit_off;
	xsize >>= 3;
/* row loop */
	for (j = 0; j < ysize; j++) {
/* prepare source & destination address */
		sp = (__m128i *)sl;
		dp = (mlib_u8 *)dl;

		MLIB_S_IMAGETHRESH_EDGE_1B_S32(
			sp, thresh, thresh, hs, ls);
		dp[0] = (usd & emask0) | (dp[0] & ~emask0);
		sp += 2;
		dp++;

/* 8-pixel column loop */
#ifdef  __SUNPRO_C
#pragma pipeloop(0)
#endif
		for (i = 0; i < xsize; i++) {
			MLIB_S_IMAGETHRESH1B_S32(
				sp, dp, thresh, thresh, hs, ls);
		}

/* last 8-pixel column */

		if (emask1) {
			MLIB_S_IMAGETHRESH_EDGE_1B_S32(
				sp, thresh, thresh, hs, ls);
			dp[0] = (usd & emask1) | (dp[0] & ~emask1);
		}

		sl += slb;
		dl += dlb;
	}
}

/* *********************************************************** */

void
mlib_s_ImageThresh1B_S32_3_A8D1(
    const mlib_s32 *src,
    mlib_u8 *dst,
    mlib_s32 dsize,
    const mlib_s32 *th,
    mlib_s32 hc0,
    mlib_s32 lc0)
{
/* aligned src address */
	__m128i *sp;

/* destination address */
	mlib_u8 *dp;

/* source data */
	__m128i sd, sd0, sd1;

/* destination data */
	mlib_u8 usd;

/* threshold data */
	__m128i thresh0, thresh1, thresh2;

/* ghigh data */
	__m128i hs0, hs1, hs2;

/* glow data */
	__m128i ls0, ls1, ls2;

/* temporaries used in macro */
	__m128i mask;
	const __m128i zero = _mm_set1_epi8(0x00);

/* loop variables */
	mlib_s32 j;

/* prepare the threshold */

	thresh0 = _mm_set_epi32(
				th[0], th[1], th[2], th[0]);
	thresh1 = _mm_set_epi32(
				th[1], th[2], th[0], th[1]);
	thresh2 = _mm_set_epi32(
				th[2], th[0], th[1], th[2]);

	hs0 = _mm_cvtsi32_si128(hc0);
	ls0 = _mm_cvtsi32_si128(lc0);
	hs1 = _mm_cvtsi32_si128(hc0 >> 1);
	ls1 = _mm_cvtsi32_si128(lc0 >> 1);
	hs2 = _mm_cvtsi32_si128(hc0 >> 2);
	ls2 = _mm_cvtsi32_si128(lc0 >> 2);

	sp = (__m128i *)src;
	dp = (mlib_u8 *)dst;

	dsize >>= 3;
/* 8-pixel column loop */
#ifdef  __SUNPRO_C
#pragma pipeloop(0)
#endif
	for (j = 0; j < dsize; j++) {
		MLIB_S_IMAGETHRESH1B_S32(
			sp, dp, thresh0, thresh1, hs0, ls0);
		MLIB_S_IMAGETHRESH1B_S32(
			sp, dp, thresh2, thresh0, hs1, ls1);
		MLIB_S_IMAGETHRESH1B_S32(
			sp, dp, thresh1, thresh2, hs2, ls2);
	}
}

/* *********************************************************** */

void
mlib_s_ImageThresh1B_S32_3_A8D2(
    const mlib_s32 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_s32 *th,
    mlib_s32 hc0,
    mlib_s32 lc0)
{
/* aligned src address */
	__m128i *sp;

/* pointer to a line of src */
	mlib_u8 *sl;

/* pointer to end of src */
	mlib_u8 *send;

/* destination address */
	mlib_u8 *dp;

/* pointer to a line of dst */
	mlib_u8 *dl;

/* source data */
	__m128i sd, sd0, sd1;

/* destination data */
	mlib_u8 usd;

/* ghigh data */
	__m128i hs0, hs1, hs2;

/* glow data */
	__m128i ls0, ls1, ls2;

/* threshold data */
	__m128i thresh0, thresh1, thresh2;

/* temporaries used in macro */
	__m128i mask;
	const __m128i zero = _mm_set1_epi8(0x00);

/* edge mask */
	mlib_s32 esize;
	mlib_s32 emask;

/* loop variables */
	mlib_s32 i, j;

/* prepare the threshold */
	thresh0 = _mm_set_epi32(
				th[0], th[1], th[2], th[0]);
	thresh1 = _mm_set_epi32(
				th[1], th[2], th[0], th[1]);
	thresh2 = _mm_set_epi32(
				th[2], th[0], th[1], th[2]);

	hs0 = _mm_cvtsi32_si128(hc0);
	ls0 = _mm_cvtsi32_si128(lc0);
	hs1 = _mm_cvtsi32_si128(hc0 >> 1);
	ls1 = _mm_cvtsi32_si128(lc0 >> 1);
	hs2 = _mm_cvtsi32_si128(hc0 >> 2);
	ls2 = _mm_cvtsi32_si128(lc0 >> 2);

	esize = 3 * (xsize & 7);
	emask = (0xFF << (8 - (esize & 7))) & 0xFF;

	sl = (mlib_u8 *)src;
	dl = (mlib_u8 *)dst;

	xsize >>= 3;
/* row loop */
	for (j = 0; j < ysize; j++) {

		sp = (__m128i *)sl;
		dp = (mlib_u8 *)dl;
/* 8-pixel column loop */
#ifdef  __SUNPRO_C
#pragma pipeloop(0)
#endif
		for (i = 0; i < xsize; i++) {
			MLIB_S_IMAGETHRESH1B_S32(
				sp, dp, thresh0, thresh1, hs0, ls0);
			MLIB_S_IMAGETHRESH1B_S32(
				sp, dp, thresh2, thresh0, hs1, ls1);
			MLIB_S_IMAGETHRESH1B_S32(
				sp, dp, thresh1, thresh2, hs2, ls2);
		}

/* last 8-pixel column */
		if (esize) {
			MLIB_S_IMAGETHRESH_EDGE_1B_S32(
				sp, thresh0, thresh1, hs0, ls0);
			sp += 2;

			if (esize >= 8) {
				dp[0] = usd;
				dp++;

				MLIB_S_IMAGETHRESH_EDGE_1B_S32(
					sp, thresh2, thresh0, hs1, ls1);
				sp += 2;

				if (esize >= 16) {
					dp[0] = usd;
					dp++;

					MLIB_S_IMAGETHRESH_EDGE_1B_S32(
						sp, thresh1, thresh2, hs2, ls2);
				}
			}

			dp[0] = (usd & emask) | (dp[0] & ~emask);
		}

		sl += slb;
		dl += dlb;
	}
}

/* *********************************************************** */

void
mlib_s_ImageThresh1B_S32_3(
    const mlib_s32 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 dbit_off,
    const mlib_s32 *th,
    mlib_s32 hc0,
    mlib_s32 lc0)
{
/* aligned src address */
	__m128i *sp;

/* pointer to a line of src */
	mlib_u8 *sl;

/* destination address */
	mlib_u8 *dp;

/* pointer to a line of dst */
	mlib_u8 *dl;

/* source data */
	__m128i sd, sd0, sd1;

/* destination data */
	mlib_u8 usd;

/* ghigh data */
	__m128i hs0, hs1, hs2;

/* glow data */
	__m128i ls0, ls1, ls2;

/* threshold data */
	__m128i thresh0, thresh1, thresh2;

/* temporaries used in macro */
	__m128i mask;
	const __m128i zero = _mm_set1_epi8(0x00);

/* temporaries */
	mlib_s32 esize, dsize = 3 * xsize;

/* edge mask */
	mlib_s32 emask0, emask1, nume;

/* loop variables */
	mlib_s32 i, j;

/* prepare the threshold */
	thresh0 = _mm_set_epi32(
				th[0], th[1], th[2], th[0]);
	thresh1 = _mm_set_epi32(
				th[1], th[2], th[0], th[1]);
	thresh2 = _mm_set_epi32(
				th[2], th[0], th[1], th[2]);

	hs0 = _mm_cvtsi32_si128(hc0);
	ls0 = _mm_cvtsi32_si128(lc0);
	hs1 = _mm_cvtsi32_si128(hc0 >> 1);
	ls1 = _mm_cvtsi32_si128(lc0 >> 1);
	hs2 = _mm_cvtsi32_si128(hc0 >> 2);
	ls2 = _mm_cvtsi32_si128(lc0 >> 2);

	sl = (mlib_u8 *)src;
	dl = (mlib_u8 *)dst;

	nume = (8 - dbit_off);

	if (nume > dsize)
		nume = dsize;
	emask0 = (((0xFF00 >> nume) & 0xFF) >> dbit_off);

	esize = 3 * (xsize & 7) - nume;

	if (esize < 0) {
		xsize -= 8;
		esize += 24;
	}

	emask1 = (0xFF << (8 - (esize & 7))) & 0xFF;

	sl -= 4 * dbit_off;
	xsize >>= 3;

/* row loop */
	for (j = 0; j < ysize; j++) {
/* prepare source & destination addresses */
		sp = (__m128i *)sl;
		dp = (mlib_u8 *)dl;
		MLIB_S_IMAGETHRESH_EDGE_1B_S32(
			sp, thresh0, thresh1, hs0, ls0);
		dp[0] = (usd & emask0) | (dp[0] & ~emask0);
		dp++;
		sp += 2;

/* 8-pixel column loop */
#ifdef  __SUNPRO_C
#pragma pipeloop(0)
#endif
		for (i = 0; i < xsize; i++) {
			MLIB_S_IMAGETHRESH1B_S32(
				sp, dp, thresh2, thresh0, hs1, ls1);
			MLIB_S_IMAGETHRESH1B_S32(
				sp, dp, thresh1, thresh2, hs2, ls2);
			MLIB_S_IMAGETHRESH1B_S32(
				sp, dp, thresh0, thresh1, hs0, ls0);
		}

/* last 8-pixel column */
		if (esize) {
			MLIB_S_IMAGETHRESH_EDGE_1B_S32(
				sp, thresh2, thresh0, hs1, ls1);
			sp += 2;

			if (esize >= 8) {
				dp[0] = usd;
				dp++;

				MLIB_S_IMAGETHRESH_EDGE_1B_S32(
					sp, thresh1, thresh2, hs2, ls2);
				sp += 2;

				if (esize >= 16) {
					dp[0] = usd;
					dp++;

					MLIB_S_IMAGETHRESH_EDGE_1B_S32(
						sp, thresh0, thresh1, hs0, ls0);
				}
			}

			dp[0] = (usd & emask1) | (dp[0] & ~emask1);
		}

		sl += slb;
		dl += dlb;
	}
}

/* *********************************************************** */
