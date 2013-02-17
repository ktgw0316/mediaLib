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

#pragma ident	"@(#)mlib_s_ImageChannelExtract_1.c	9.5	07/11/05 SMI"

/*
 * FUNCTIONS
 *      mlib_s_ImageChannelExtract_U8_21_A16D1
 *      mlib_s_ImageChannelExtract_U8_21_A16D2
 *      mlib_s_ImageChannelExtract_U8_21_D1
 *      mlib_s_ImageChannelExtract_U8_21
 *      mlib_s_ImageChannelExtract_U8_31_A16D1
 *      mlib_s_ImageChannelExtract_U8_31_A16D2
 *      mlib_s_ImageChannelExtract_U8_31_D1
 *      mlib_s_ImageChannelExtract_U8_31
 *      mlib_s_ImageChannelExtract_U8_41_A16D1
 *      mlib_s_ImageChannelExtract_U8_41_A16D2
 *      mlib_s_ImageChannelExtract_U8_41_D1
 *      mlib_s_ImageChannelExtract_U8_41
 *      mlib_s_ImageChannelExtract_S16_21_A16D1
 *      mlib_s_ImageChannelExtract_S16_21_A16D2
 *      mlib_s_ImageChannelExtract_S16_21_D1
 *      mlib_s_ImageChannelExtract_S16_21
 *      mlib_s_ImageChannelExtract_S16_31_A16D1
 *      mlib_s_ImageChannelExtract_S16_31_A16D2
 *      mlib_s_ImageChannelExtract_S16_31_D1
 *      mlib_s_ImageChannelExtract_S16_31
 *      mlib_s_ImageChannelExtract_S16_41_A16D1
 *      mlib_s_ImageChannelExtract_S16_41_A16D2
 *      mlib_s_ImageChannelExtract_S16_41_D1
 *      mlib_s_ImageChannelExtract_S16_41
 *
 * ARGUMENT
 *      src    pointer to source image data
 *      dst    pointer to destination image data
 *      slb    source image line stride in bytes
 *      dlb   destination image line stride in bytes
 *      dsize  image data size in pixels
 *      xsize  image width in pixels
 *      ysize  image height in lines
 *      cmask channel mask
 *
 * DESCRIPTION
 *      Extract the one selected channel of the source image into the
 *      1-channel destination image.
 *
 * NOTE
 *      These functions are separated from mlib_ImageChannelExtract.c
 *      for loop unrolling and structure clarity.
 */

#include <mlib_image.h>
#include <mlib_s_ImageChannelExtract.h>
#ifdef __SUNPRO_C
#include <sunmedia_intrin.h>
#else
#include <emmintrin.h>
#endif

/* *********************************************************** */

/*
 * extract the left channel of 2 channels
 */

#define	CHANNELEXTRACT_S_U8_21L(ld, st)                         \
	__m128i x_mk = _mm_set1_epi16(0x00FF);                  \
	for (; i < dsize - 15; i += 16) {                       \
		__m128i x_sd0, x_sd1;                           \
		__m128i x_st0, x_st1;                           \
		__m128i x_dd;                                   \
		x_sd0 = _mm_##ld##_si128((__m128i *)sp);        \
		x_sd1 = _mm_##ld##_si128((__m128i *)(sp + 16)); \
		x_st0 = _mm_and_si128(x_sd0, x_mk);             \
		x_st1 = _mm_and_si128(x_sd1, x_mk);             \
		                                                \
		x_dd = _mm_packus_epi16(x_st0, x_st1);          \
		_mm_##st##_si128((__m128i *)dp, x_dd);          \
		dp += 16;                                       \
		sp += 32;                                       \
	}

/* *********************************************************** */

/*
 * extract the right channel of 2 channels
 */

#define	CHANNELEXTRACT_S_U8_21R(ld, st)                         \
	for (; i < dsize - 15; i += 16) {                       \
		__m128i x_sd0, x_sd1;                           \
		__m128i x_st0, x_st1;                           \
		__m128i x_dd;                                   \
		x_sd0 = _mm_##ld##_si128((__m128i *)sp);        \
		x_sd1 = _mm_##ld##_si128((__m128i *)(sp + 16)); \
		x_sd0 = _mm_srli_epi16(x_sd0, 8);               \
		x_sd1 = _mm_srli_epi16(x_sd1, 8);               \
		x_dd = _mm_packus_epi16(x_sd0, x_sd1);          \
		_mm_##st##_si128((__m128i *)dp, x_dd);          \
		dp += 16;                                       \
		sp += 32;                                       \
	}

/* *********************************************************** */

/* Head point handling */

#define	CHANNELEXTRACT_S_U8_21HEAD(tap)                      \
	for (; (i < dsize) && ((mlib_addr)sp & 15); i++) {   \
		mlib_u8 sd0 = sp[tap];                       \
		dp[0] = sd0;	                             \
		sp += 2;	                             \
		dp++;		                             \
	}

/* End point handling */

#define	CHANNELEXTRACT_S_U8_21TAIL(tap)                      \
	for (; i < dsize; i++) {                             \
		mlib_u8 sd0 = sp[tap];                       \
		dp[0] = sd0;                                 \
		sp += 2;                                     \
		dp++;                                        \
	}

/* *********************************************************** */

/*
 * extract one channel from a 2-channel image.
 * both source and destination image data are 16-byte aligned.
 * both stride is equal to width in bytes.
 */

void
mlib_s_ImageChannelExtract_U8_21_A16D1(
    const mlib_u8 *src,
    mlib_u8 *dst,
    mlib_s32 dsize,
    mlib_s32 cmask)
{
	mlib_u8 *sp, *dp;
	mlib_s32 i = 0;

	sp = (mlib_u8 *)src;
	dp = (mlib_u8 *)dst;

	if (cmask == 2) {

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		CHANNELEXTRACT_S_U8_21L(load, store)

		CHANNELEXTRACT_S_U8_21TAIL(0)

	} else {

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		CHANNELEXTRACT_S_U8_21R(load, store)

		CHANNELEXTRACT_S_U8_21TAIL(1)

	}
}

/* *********************************************************** */

/*
 * extract one channel from a 2-channel image.
 * both source and destination image data are 16-byte aligned.
 * both source and destination image strides are multiple of 16.
 */

void
mlib_s_ImageChannelExtract_U8_21_A16D2(
    const mlib_u8 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 cmask)
{
	mlib_u8 *sp, *dp;
	mlib_u8 *sa, *da;
	mlib_s32 i, j;
	mlib_s32 dsize = xsize;

	sp = sa = (mlib_u8 *)src;
	dp = da = (mlib_u8 *)dst;

	if (cmask == 2) {
		for (j = 0; j < ysize; j++) {

			i = 0;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			CHANNELEXTRACT_S_U8_21L(load, store)

			CHANNELEXTRACT_S_U8_21TAIL(0)

			sp = sa += slb;
			dp = da += dlb;
		}
	} else {
		for (j = 0; j < ysize; j++) {

			i = 0;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			CHANNELEXTRACT_S_U8_21R(load, store)

			CHANNELEXTRACT_S_U8_21TAIL(1)

			sp = sa += slb;
			dp = da += dlb;
		}
	}
}

/* *********************************************************** */

/*
 * extract one channel from a 2-channel image.
 * both stride is equal to width in bytes.
 */

void
mlib_s_ImageChannelExtract_U8_21_D1(
    const mlib_u8 *src,
    mlib_u8 *dst,
    mlib_s32 dsize,
    mlib_s32 cmask)
{
	mlib_u8 *sp, *dp;
	mlib_s32 i = 0;

	sp = (mlib_u8 *)src;
	dp = (mlib_u8 *)dst;

	if (cmask == 2) {

		CHANNELEXTRACT_S_U8_21HEAD(0)

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		CHANNELEXTRACT_S_U8_21L(load, storeu)

		CHANNELEXTRACT_S_U8_21TAIL(0)
	} else {

		CHANNELEXTRACT_S_U8_21HEAD(1)

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		CHANNELEXTRACT_S_U8_21R(load, storeu)

		CHANNELEXTRACT_S_U8_21TAIL(1)
	}
}

/* *********************************************************** */

/*
 * extract one channel from a 2-channel image.
 */

void
mlib_s_ImageChannelExtract_U8_21(
    const mlib_u8 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 cmask)
{
	mlib_u8 *sp, *dp;
	mlib_u8 *sa, *da;
	mlib_s32 i, j;
	mlib_s32 dsize = xsize;

	sp = sa = (mlib_u8 *)src;
	dp = da = (mlib_u8 *)dst;

	if (cmask == 2) {
		for (j = 0; j < ysize; j++) {

			i = 0;

			CHANNELEXTRACT_S_U8_21HEAD(0)

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			CHANNELEXTRACT_S_U8_21L(load, storeu)

			CHANNELEXTRACT_S_U8_21TAIL(0)

			sp = sa += slb;
			dp = da += dlb;
		}
	} else {
		for (j = 0; j < ysize; j++) {

			i = 0;

			CHANNELEXTRACT_S_U8_21HEAD(1)

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			CHANNELEXTRACT_S_U8_21R(load, storeu)

			CHANNELEXTRACT_S_U8_21TAIL(1)

			sp = sa += slb;
			dp = da += dlb;
		}
	}
}

/* *********************************************************** */

#define	CHANNELEXTRACT_S_U8_31_1                                             \
	x_sdg = _mm_unpacklo_epi8(x_sdb, _mm_unpackhi_epi64(x_sdc, x_sdc));  \
	x_sde = _mm_unpacklo_epi8(x_sda, _mm_unpackhi_epi64(x_sdb, x_sdb));  \
	x_sdh = _mm_unpackhi_epi8(x_sde, _mm_unpacklo_epi64(x_sdg, x_sdg));  \
	x_sdf = _mm_unpackhi_epi8(x_sda, _mm_unpacklo_epi64(x_sdc, x_sdc));  \
	x_sdi = _mm_unpacklo_epi8(x_sdf, _mm_unpackhi_epi64(x_sdg, x_sdg));  \
	x_dd = _mm_unpacklo_epi8(x_sdh, _mm_unpackhi_epi64(x_sdi, x_sdi))

#define	CHANNELEXTRACT_S_U8_31_2                                             \
	x_sde = _mm_unpacklo_epi8(x_sda, _mm_unpackhi_epi64(x_sdb, x_sdb));  \
	x_sdf = _mm_unpackhi_epi8(x_sda, _mm_unpacklo_epi64(x_sdc, x_sdc));  \
	x_sdg = _mm_unpacklo_epi8(x_sdb, _mm_unpackhi_epi64(x_sdc, x_sdc));  \
	x_sdh = _mm_unpacklo_epi8(x_sde, _mm_unpackhi_epi64(x_sdf, x_sdf));  \
	x_sdi = _mm_unpacklo_epi8(x_sdf, _mm_unpackhi_epi64(x_sdg, x_sdg));  \
	x_dd = _mm_unpackhi_epi8(x_sdh, _mm_unpacklo_epi64(x_sdi, x_sdi))

#define	CHANNELEXTRACT_S_U8_31_4                                             \
	x_sde = _mm_unpacklo_epi8(x_sda, _mm_unpackhi_epi64(x_sdb, x_sdb));  \
	x_sdf = _mm_unpackhi_epi8(x_sda, _mm_unpacklo_epi64(x_sdc, x_sdc));  \
	x_sdg = _mm_unpacklo_epi8(x_sdb, _mm_unpackhi_epi64(x_sdc, x_sdc));  \
	x_sdh = _mm_unpacklo_epi8(x_sde, _mm_unpackhi_epi64(x_sdf, x_sdf));  \
	x_sdi = _mm_unpackhi_epi8(x_sde, _mm_unpacklo_epi64(x_sdg, x_sdg));  \
	x_dd = _mm_unpacklo_epi8(x_sdh, _mm_unpackhi_epi64(x_sdi, x_sdi))

/* *********************************************************** */

/* Body pixels handling */

#define	CHANNELEXTRACT_S_U8_31_BODY(CMK, sd)                   \
	for (; i < dsize - 15; i += 16) {                      \
		__m128i x_sd0, x_sd1, x_sd2;                   \
		__m128i x_sd3, x_sd4, x_sd5;                   \
		__m128i x_sda, x_sdb, x_sdc;                   \
		__m128i x_sde, x_sdf, x_sdg;                   \
		__m128i x_sdh, x_sdi, x_dd;                    \
		x_sd0 = _mm_load_si128((__m128i *)sp);         \
		x_sd1 = _mm_load_si128((__m128i *)(sp + 16));  \
		x_sd2 = _mm_load_si128((__m128i *)(sp + 32));  \
		sp += 48;                                      \
                                                               \
		x_sd3 = _mm_unpackhi_epi64(x_sd1, x_sd1);      \
		x_sd4 = _mm_unpacklo_epi64(x_sd2, x_sd2);      \
		x_sd5 = _mm_unpackhi_epi64(x_sd2, x_sd2);      \
                                                               \
		x_sda = _mm_unpacklo_epi8(x_sd0, x_sd3);       \
		x_sdb = _mm_unpackhi_epi8(x_sd0, x_sd4);       \
		x_sdc = _mm_unpacklo_epi8(x_sd1, x_sd5);       \
                                                               \
		CHANNELEXTRACT_S_U8_31_##CMK;                  \
                                                               \
		_mm_##sd##_si128((__m128i *)dp, x_dd);         \
		dp += 16;                                      \
	}                                                      \
                                                               \
	if (i < dsize - 7) {                                   \
		__m128i x_sd0, x_sd1;                          \
		__m128i x_sda, x_sdb, x_sdc;                   \
		__m128i x_sde, x_sdf, x_sdg;                   \
		__m128i x_sdh, x_sdi, x_dd;                    \
		x_sd0 = _mm_load_si128((__m128i *)sp);         \
		x_sd1 = _mm_loadl_epi64((__m128i *)(sp + 16)); \
		sp += 24;                                      \
                                                               \
		x_sda = _mm_unpacklo_epi8(x_sd0, x_sd0);       \
		x_sdb = _mm_unpackhi_epi8(x_sd0, x_sd0);       \
		x_sdc = _mm_unpacklo_epi8(x_sd1, x_sd1);       \
                                                               \
		CHANNELEXTRACT_S_U8_31_##CMK;                  \
                                                               \
		_mm_storel_epi64((__m128i *)dp, x_dd);         \
		dp += 8;                                       \
		i += 8;                                        \
	}                                                      \

/* *********************************************************** */

/* Head pixels handling */

#define	CHANNELEXTRACT_S_U8_31HEAD(tap)                    \
	for (; (i < dsize) && ((mlib_addr)sp & 15); i++) { \
		dp[0] = sp[tap];                           \
		sp += 3;                                   \
		dp++;                                      \
	}

/* End pixels handling */

#define	CHANNELEXTRACT_S_U8_31TAIL(tap)                    \
	for (; i < dsize; i++) {                           \
		dp[0] = sp[tap];                           \
		sp += 3;                                   \
		dp++;                                      \
	}

/* *********************************************************** */

/*
 * extract one channel from a 3-channel image.
 * both source and destination image data are 16-byte aligned.
 * both stride is equal to width in bytes.
 */

void
mlib_s_ImageChannelExtract_U8_31_A16D1(
    const mlib_u8 *src,
    mlib_u8 *dst,
    mlib_s32 dsize,
    mlib_s32 cmask)
{
	mlib_s32 i = 0;
	mlib_u8 *sp, *dp;

	sp = (mlib_u8 *)src;
	dp = (mlib_u8 *)dst;

	if (cmask == 4) {

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#pragma unroll(3)
#endif /* __SUNPRO_C */
		CHANNELEXTRACT_S_U8_31_BODY(4, store);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		CHANNELEXTRACT_S_U8_31TAIL(0)

	} else if (cmask == 2) {

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#pragma unroll(3)
#endif /* __SUNPRO_C */
		CHANNELEXTRACT_S_U8_31_BODY(2, store);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		CHANNELEXTRACT_S_U8_31TAIL(1)

	} else {

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#pragma unroll(3)
#endif /* __SUNPRO_C */
		CHANNELEXTRACT_S_U8_31_BODY(1, store);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		CHANNELEXTRACT_S_U8_31TAIL(2)
	}
}

/* *********************************************************** */

/*
 * extract one channel from a 3-channel image.
 * both source and destination image data are 16-byte aligned.
 * both source and destination image strides are multiple of 16.
 */

void
mlib_s_ImageChannelExtract_U8_31_A16D2(
    const mlib_u8 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 cmask)
{
	mlib_s32 i, j;
	mlib_u8 *sp, *dp;
	mlib_u8 *sa, *da;

	sa = sp = (mlib_u8 *)src;
	da = dp = (mlib_u8 *)dst;

	mlib_s32 dsize = xsize;

	if (cmask == 4) {
		for (j = 0; j < ysize; j++) {

			i = 0;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#pragma unroll(3)
#endif /* __SUNPRO_C */
			CHANNELEXTRACT_S_U8_31_BODY(4, store);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			CHANNELEXTRACT_S_U8_31TAIL(0)

			sp = sa += slb;
			dp = da += dlb;
		}

	} else if (cmask == 2) {

		for (j = 0; j < ysize; j++) {

			i = 0;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#pragma unroll(3)
#endif /* __SUNPRO_C */
			CHANNELEXTRACT_S_U8_31_BODY(2, store);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			CHANNELEXTRACT_S_U8_31TAIL(1)

			sp = sa += slb;
			dp = da += dlb;
		}

	} else {

		for (j = 0; j < ysize; j++) {

			i = 0;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#pragma unroll(3)
#endif /* __SUNPRO_C */
			CHANNELEXTRACT_S_U8_31_BODY(1, store);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			CHANNELEXTRACT_S_U8_31TAIL(2)

			sp = sa += slb;
			dp = da += dlb;
		}
	}
}

/* *********************************************************** */

/*
 * extract one channel from a 3-channel image.
 * both stride is equal to width in bytes.
 */

void
mlib_s_ImageChannelExtract_U8_31_D1(
    const mlib_u8 *src,
    mlib_u8 *dst,
    mlib_s32 dsize,
    mlib_s32 cmask)
{
	mlib_s32 i = 0;
	mlib_u8 *sp, *dp;

	sp = (mlib_u8 *)src;
	dp = (mlib_u8 *)dst;

	if (cmask == 4) {

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		CHANNELEXTRACT_S_U8_31HEAD(0)

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		CHANNELEXTRACT_S_U8_31_BODY(4, storeu);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		CHANNELEXTRACT_S_U8_31TAIL(0)

	} else if (cmask == 2) {

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		CHANNELEXTRACT_S_U8_31HEAD(1)

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		CHANNELEXTRACT_S_U8_31_BODY(2, storeu);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		CHANNELEXTRACT_S_U8_31TAIL(1)

	} else {

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		CHANNELEXTRACT_S_U8_31HEAD(2)

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#pragma unroll(3)
#endif /* __SUNPRO_C */
		CHANNELEXTRACT_S_U8_31_BODY(1, storeu);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		CHANNELEXTRACT_S_U8_31TAIL(2)
	}
}

/* *********************************************************** */

/* extract one channel from a 3-channel image. */

void
mlib_s_ImageChannelExtract_U8_31(
    const mlib_u8 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 cmask)
{
	mlib_s32 i, j;
	mlib_u8 *sp, *dp;
	mlib_u8 *sa, *da;

	sa = sp = (mlib_u8 *)src;
	da = dp = (mlib_u8 *)dst;

	mlib_s32 dsize = xsize;

	if (cmask == 4) {
		for (j = 0; j < ysize; j++) {

			i = 0;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			CHANNELEXTRACT_S_U8_31HEAD(0)

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#pragma unroll(3)
#endif /* __SUNPRO_C */
			CHANNELEXTRACT_S_U8_31_BODY(4, storeu);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			CHANNELEXTRACT_S_U8_31TAIL(0)

			sp = sa += slb;
			dp = da += dlb;
		}

	} else if (cmask == 2) {
		for (j = 0; j < ysize; j++) {

			i = 0;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			CHANNELEXTRACT_S_U8_31HEAD(1)

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#pragma unroll(3)
#endif /* __SUNPRO_C */
			CHANNELEXTRACT_S_U8_31_BODY(2, storeu);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			CHANNELEXTRACT_S_U8_31TAIL(1)

			sp = sa += slb;
			dp = da += dlb;
		}

	} else {
		for (j = 0; j < ysize; j++) {

			i = 0;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			CHANNELEXTRACT_S_U8_31HEAD(2)

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#pragma unroll(3)
#endif /* __SUNPRO_C */
			CHANNELEXTRACT_S_U8_31_BODY(1, storeu);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			CHANNELEXTRACT_S_U8_31TAIL(2)

			sp = sa += slb;
			dp = da += dlb;
		}
	}
}

/* *********************************************************** */

/*
 * extract the left channel of 4 channels
 */

#define	CHANNELEXTRACT_S_U8_41L_16(ld, st)                      \
	__m128i x_mk = _mm_set1_epi32(0x000000FF);              \
	for (; i < dsize - 15; i += 16) {                       \
		__m128i x_sd0, x_sd1, x_sd2, x_sd3;             \
		__m128i x_st0, x_st1, x_st2, x_st3;             \
		__m128i x_dd;                                   \
		x_sd0 = _mm_##ld##_si128((__m128i *)sp);        \
		x_sd1 = _mm_##ld##_si128((__m128i *)(sp + 16)); \
		x_sd2 = _mm_##ld##_si128((__m128i *)(sp + 32)); \
		x_sd3 = _mm_##ld##_si128((__m128i *)(sp + 48)); \
		                                                \
		x_st0 = _mm_and_si128(x_sd0, x_mk);             \
		x_st1 = _mm_and_si128(x_sd1, x_mk);             \
		x_st2 = _mm_and_si128(x_sd2, x_mk);             \
		x_st3 = _mm_and_si128(x_sd3, x_mk);             \
		                                                \
		x_st0 = _mm_packus_epi16(x_st0, x_st1);         \
		x_st1 = _mm_packus_epi16(x_st2, x_st3);         \
		                                                \
		x_dd = _mm_packus_epi16(x_st0, x_st1);          \
		_mm_##st##_si128((__m128i *)dp, x_dd);          \
		sp += 64;                                       \
		dp += 16;                                       \
	}

#define	CHANNELEXTRACT_S_U8_41L_8(ld)                           \
	if (i < dsize - 7) {                                    \
		__m128i x_sd0, x_sd1;                           \
		__m128i x_st0, x_st1, x_st2;                    \
		__m128i x_dd;                                   \
		x_sd0 = _mm_##ld##_si128((__m128i *)sp);        \
		x_sd1 = _mm_##ld##_si128((__m128i *)(sp + 16)); \
		x_st0 = _mm_and_si128(x_sd0, x_mk);             \
		x_st1 = _mm_and_si128(x_sd1, x_mk);             \
		                                                \
		x_st2 = _mm_packus_epi16(x_st0, x_st1);         \
		x_dd = _mm_packus_epi16(x_st2, x_st2);          \
		_mm_storel_epi64((__m128i *)dp, x_dd);          \
		sp += 32;                                       \
		dp += 8;                                        \
		i += 8;                                         \
	}

/* *********************************************************** */

/*
 * extract the middle left channel of 4 channels
 */

#define	CHANNELEXTRACT_S_U8_41ML_16(ld, st)                     \
	for (; i < dsize - 15; i += 16) {                       \
		__m128i x_sd0, x_sd1, x_sd2, x_sd3;             \
		__m128i x_st0, x_st1, x_st2, x_st3;             \
		__m128i x_dd;                                   \
		x_sd0 = _mm_##ld##_si128((__m128i *)sp);        \
		x_sd1 = _mm_##ld##_si128((__m128i *)(sp + 16)); \
		x_sd2 = _mm_##ld##_si128((__m128i *)(sp + 32)); \
		x_sd3 = _mm_##ld##_si128((__m128i *)(sp + 48)); \
		                                                \
		x_st0 = _mm_slli_epi32(x_sd0, 16);              \
		x_st1 = _mm_slli_epi32(x_sd1, 16);              \
		x_st2 = _mm_slli_epi32(x_sd2, 16);              \
		x_st3 = _mm_slli_epi32(x_sd3, 16);              \
		                                                \
		x_st0 = _mm_srli_epi32(x_st0, 24);              \
		x_st1 = _mm_srli_epi32(x_st1, 24);              \
		x_st2 = _mm_srli_epi32(x_st2, 24);              \
		x_st3 = _mm_srli_epi32(x_st3, 24);              \
						                \
		x_st0 = _mm_packus_epi16(x_st0, x_st1);         \
		x_st1 = _mm_packus_epi16(x_st2, x_st3);         \
		x_dd = _mm_packus_epi16(x_st0, x_st1);          \
		_mm_##st##_si128((__m128i *)dp, x_dd);          \
		sp += 64;                                       \
		dp += 16;                                       \
	}

#define	CHANNELEXTRACT_S_U8_41ML_8(ld)	                        \
	if (i < dsize - 7) {	                                \
		__m128i x_sd0, x_sd1;                           \
		__m128i x_st0, x_st1, x_st2;                    \
		__m128i x_dd;                                   \
		x_sd0 = _mm_##ld##_si128((__m128i *)sp);        \
		x_sd1 = _mm_##ld##_si128((__m128i *)(sp + 16)); \
		                                                \
		x_st0 = _mm_slli_epi32(x_sd0, 16);              \
		x_st1 = _mm_slli_epi32(x_sd1, 16);              \
		                                                \
		x_st0 = _mm_srli_epi32(x_st0, 24);              \
		x_st1 = _mm_srli_epi32(x_st1, 24);              \
		                                                \
		x_st2 = _mm_packus_epi16(x_st0, x_st1);         \
		x_dd = _mm_packus_epi16(x_st2, x_st2);          \
		_mm_storel_epi64((__m128i *)dp, x_dd);          \
		sp += 32;                                       \
		dp += 8;                                        \
		i += 8;                                         \
	}

/* *********************************************************** */

/*
 * extract the middle right channel of 4 channels
 */

#define	CHANNELEXTRACT_S_U8_41MR_16(ld, st)	                \
	for (; i < dsize - 15; i += 16) {	                \
		__m128i x_sd0, x_sd1, x_sd2, x_sd3;             \
		__m128i x_st0, x_st1, x_st2, x_st3;             \
		__m128i x_dd;                                   \
		x_sd0 = _mm_##ld##_si128((__m128i *)sp);        \
		x_sd1 = _mm_##ld##_si128((__m128i *)(sp + 16)); \
		x_sd2 = _mm_##ld##_si128((__m128i *)(sp + 32)); \
		x_sd3 = _mm_##ld##_si128((__m128i *)(sp + 48)); \
		                                                \
		x_st0 = _mm_slli_epi32(x_sd0, 8);               \
		x_st1 = _mm_slli_epi32(x_sd1, 8);               \
		x_st2 = _mm_slli_epi32(x_sd2, 8);               \
		x_st3 = _mm_slli_epi32(x_sd3, 8);               \
		                                                \
		x_st0 = _mm_srli_epi32(x_st0, 24);              \
		x_st1 = _mm_srli_epi32(x_st1, 24);              \
		x_st2 = _mm_srli_epi32(x_st2, 24);              \
		x_st3 = _mm_srli_epi32(x_st3, 24);              \
		                                                \
		x_st0 = _mm_packus_epi16(x_st0, x_st1);         \
		x_st1 = _mm_packus_epi16(x_st2, x_st3);         \
		x_dd = _mm_packus_epi16(x_st0, x_st1);          \
		_mm_##st##_si128((__m128i *)dp, x_dd);          \
		sp += 64;                                       \
		dp += 16;                                       \
	}

#define	CHANNELEXTRACT_S_U8_41MR_8(ld)	                        \
	if (i < dsize - 7) {	                                \
		__m128i x_sd0, x_sd1;                           \
		__m128i x_st0, x_st1, x_st2;                    \
		__m128i x_dd;                                   \
		x_sd0 = _mm_##ld##_si128((__m128i *)sp);        \
		x_sd1 = _mm_##ld##_si128((__m128i *)(sp + 16)); \
		                                                \
		x_st0 = _mm_slli_epi32(x_sd0, 8);               \
		x_st1 = _mm_slli_epi32(x_sd1, 8);               \
		                                                \
		x_st0 = _mm_srli_epi32(x_st0, 24);              \
		x_st1 = _mm_srli_epi32(x_st1, 24);              \
		                                                \
		x_st2 = _mm_packus_epi16(x_st0, x_st1);         \
		x_dd = _mm_packus_epi16(x_st2, x_st2);          \
		_mm_storel_epi64((__m128i *)dp, x_dd);          \
		sp += 32;                                       \
		dp += 8;                                        \
		i += 8;                                         \
	}

/* *********************************************************** */

/*
 * extract the right channel of 4 channels
 */

#define	CHANNELEXTRACT_S_U8_41R_16(ld, st)	                \
	for (; i < dsize - 15; i += 16) {	                \
		__m128i x_sd0, x_sd1, x_sd2, x_sd3;             \
		__m128i x_st0, x_st1, x_st2, x_st3;             \
		__m128i x_dd;                                   \
		x_sd0 = _mm_##ld##_si128((__m128i *)sp);        \
		x_sd1 = _mm_##ld##_si128((__m128i *)(sp + 16)); \
		x_sd2 = _mm_##ld##_si128((__m128i *)(sp + 32)); \
		x_sd3 = _mm_##ld##_si128((__m128i *)(sp + 48)); \
		                                                \
		x_st0 = _mm_srli_epi32(x_sd0, 24);              \
		x_st1 = _mm_srli_epi32(x_sd1, 24);              \
		x_st2 = _mm_srli_epi32(x_sd2, 24);              \
		x_st3 = _mm_srli_epi32(x_sd3, 24);              \
		                                                \
		x_st0 = _mm_packus_epi16(x_st0, x_st1);         \
		x_st1 = _mm_packus_epi16(x_st2, x_st3);         \
		x_dd = _mm_packus_epi16(x_st0, x_st1);          \
		_mm_##st##_si128((__m128i *)dp, x_dd);          \
		sp += 64;                                       \
		dp += 16;                                       \
	}

#define	CHANNELEXTRACT_S_U8_41R_8(ld)	                        \
	if (i < dsize - 7) {	                                \
		__m128i x_sd0, x_sd1;                           \
		__m128i x_st0, x_st1, x_st2;                    \
		__m128i x_dd;                                   \
		x_sd0 = _mm_##ld##_si128((__m128i *)sp);        \
		x_sd1 = _mm_##ld##_si128((__m128i *)(sp + 16)); \
		                                                \
		x_st0 = _mm_srli_epi32(x_sd0, 24);              \
		x_st1 = _mm_srli_epi32(x_sd1, 24);              \
		x_st2 = _mm_packus_epi16(x_st0, x_st1);         \
		x_dd = _mm_packus_epi16(x_st2, x_st2);          \
		_mm_storel_epi64((__m128i *)dp, x_dd);          \
		sp += 32;                                       \
		dp += 8;                                        \
		i += 8;                                         \
	}

/* *********************************************************** */

/* Head point handling */

#define	CHANNELEXTRACT_S_U8_41HEAD(tap)         \
	for (; (i < dsize) &&                   \
	    ((mlib_addr)sp & 15); i++) {        \
		mlib_u8 sd0 = sp[tap];          \
		dp[0] = sd0;                    \
		sp += 4;                        \
		dp++;                           \
	}

/* End point handling */

#define	CHANNELEXTRACT_S_U8_41TAIL(tap)         \
	for (; i < dsize; i++) {                \
		mlib_u8 sd0 = sp[tap];          \
		dp[0] = sd0;                    \
		sp += 4;                        \
		dp++;                           \
	}

/* *********************************************************** */

/*
 * extract one channel from a 4-channel image.
 * both source and destination image data are 16-byte aligned.
 * both stride is equal to width in bytes.
 */

void
mlib_s_ImageChannelExtract_U8_41_A16D1(
    const mlib_u8 *src,
    mlib_u8 *dst,
    mlib_s32 dsize,
    mlib_s32 cmask)
{
	mlib_u8 *sp, *dp;
	mlib_s32 i = 0;

	sp = (mlib_u8 *)src;
	dp = (mlib_u8 *)dst;

	if (cmask == 8) {

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		CHANNELEXTRACT_S_U8_41L_16(load, store)

		CHANNELEXTRACT_S_U8_41L_8(load)

		CHANNELEXTRACT_S_U8_41TAIL(0)

	} else if (cmask == 4) {

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		CHANNELEXTRACT_S_U8_41ML_16(load, store)

		CHANNELEXTRACT_S_U8_41ML_8(load)

		CHANNELEXTRACT_S_U8_41TAIL(1)

	} else if (cmask == 2) {

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		CHANNELEXTRACT_S_U8_41MR_16(load, store)

		CHANNELEXTRACT_S_U8_41MR_8(load)

		CHANNELEXTRACT_S_U8_41TAIL(2)

	} else {

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		CHANNELEXTRACT_S_U8_41R_16(load, store)

		CHANNELEXTRACT_S_U8_41R_8(load)

		CHANNELEXTRACT_S_U8_41TAIL(3)

	}
}

/* *********************************************************** */

/*
 * extract one channel from a 4-channel image.
 * both source and destination image data are 16-byte aligned.
 * both source and destination image strides are multiple of 16.
 */

void
mlib_s_ImageChannelExtract_U8_41_A16D2(
    const mlib_u8 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 cmask)
{
	mlib_u8 *sp, *dp;
	mlib_u8 *sa, *da;
	mlib_s32 i, j;
	mlib_s32 dsize = xsize;

	sp = sa = (mlib_u8 *)src;
	dp = da = (mlib_u8 *)dst;

	if (cmask == 8) {
		for (j = 0; j < ysize; j++) {

			i = 0;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			CHANNELEXTRACT_S_U8_41L_16(load, store)

			CHANNELEXTRACT_S_U8_41L_8(load)

			CHANNELEXTRACT_S_U8_41TAIL(0)

			sp = sa += slb;
			dp = da += dlb;
		}
	} else if (cmask == 4) {
		for (j = 0; j < ysize; j++) {

			i = 0;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			CHANNELEXTRACT_S_U8_41ML_16(load, store)

			CHANNELEXTRACT_S_U8_41ML_8(load)

			CHANNELEXTRACT_S_U8_41TAIL(1)

			sp = sa += slb;
			dp = da += dlb;
		}
	} else if (cmask == 2) {
		for (j = 0; j < ysize; j++) {

			i = 0;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			CHANNELEXTRACT_S_U8_41MR_16(load, store)

			CHANNELEXTRACT_S_U8_41MR_8(load)

			CHANNELEXTRACT_S_U8_41TAIL(2)

			sp = sa += slb;
			dp = da += dlb;
		}
	} else {
		for (j = 0; j < ysize; j++) {

			i = 0;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			CHANNELEXTRACT_S_U8_41R_16(load, store)

			CHANNELEXTRACT_S_U8_41R_8(load)

			CHANNELEXTRACT_S_U8_41TAIL(3)

			sp = sa += slb;
			dp = da += dlb;
		}
	}
}

/* *********************************************************** */

/*
 * extract one channel from a 4-channel image.
 * both stride is equal to width in bytes.
 */

void
mlib_s_ImageChannelExtract_U8_41_D1(
    const mlib_u8 *src,
    mlib_u8 *dst,
    mlib_s32 dsize,
    mlib_s32 cmask)
{
	mlib_u8 *sp, *dp;
	mlib_s32 i = 0;

	sp = (mlib_u8 *)src;
	dp = (mlib_u8 *)dst;

	if (cmask == 8) {

		CHANNELEXTRACT_S_U8_41HEAD(0)

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		CHANNELEXTRACT_S_U8_41L_16(load, storeu)

		CHANNELEXTRACT_S_U8_41L_8(load)

		CHANNELEXTRACT_S_U8_41TAIL(0)

	} else if (cmask == 4) {

		CHANNELEXTRACT_S_U8_41HEAD(1)

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		CHANNELEXTRACT_S_U8_41ML_16(load, storeu)

		CHANNELEXTRACT_S_U8_41ML_8(load)

		CHANNELEXTRACT_S_U8_41TAIL(1)

	} else if (cmask == 2) {

		CHANNELEXTRACT_S_U8_41HEAD(2)

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		CHANNELEXTRACT_S_U8_41MR_16(load, storeu)

		CHANNELEXTRACT_S_U8_41MR_8(load)

		CHANNELEXTRACT_S_U8_41TAIL(2)

	} else {

		CHANNELEXTRACT_S_U8_41HEAD(3)

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		CHANNELEXTRACT_S_U8_41R_16(load, storeu)

		CHANNELEXTRACT_S_U8_41R_8(load)

		CHANNELEXTRACT_S_U8_41TAIL(3)
	}
}

/* *********************************************************** */

/*
 * extract one channel from a 4-channel image.
 */

void
mlib_s_ImageChannelExtract_U8_41(
    const mlib_u8 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 cmask)
{
	mlib_u8 *sp, *dp;
	mlib_u8 *sa, *da;
	mlib_s32 i, j;
	mlib_s32 dsize = xsize;

	sp = sa = (mlib_u8 *)src;
	dp = da = (mlib_u8 *)dst;

	if (cmask == 8) {
		for (j = 0; j < ysize; j++) {

			i = 0;

			CHANNELEXTRACT_S_U8_41HEAD(0)

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			CHANNELEXTRACT_S_U8_41L_16(load, storeu)

			CHANNELEXTRACT_S_U8_41L_8(load)

			CHANNELEXTRACT_S_U8_41TAIL(0)

			sp = sa += slb;
			dp = da += dlb;
		}
	} else if (cmask == 4) {
		for (j = 0; j < ysize; j++) {

			i = 0;

			CHANNELEXTRACT_S_U8_41HEAD(1)

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			CHANNELEXTRACT_S_U8_41ML_16(load, storeu)

			CHANNELEXTRACT_S_U8_41ML_8(load)

			CHANNELEXTRACT_S_U8_41TAIL(1)

			sp = sa += slb;
			dp = da += dlb;
		}
	} else if (cmask == 2) {
		for (j = 0; j < ysize; j++) {

			i = 0;

			CHANNELEXTRACT_S_U8_41HEAD(2)

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			CHANNELEXTRACT_S_U8_41MR_16(load, storeu)

			CHANNELEXTRACT_S_U8_41MR_8(load)

			CHANNELEXTRACT_S_U8_41TAIL(2)

			sp = sa += slb;
			dp = da += dlb;
		}
	} else {
		for (j = 0; j < ysize; j++) {

			i = 0;

			CHANNELEXTRACT_S_U8_41HEAD(3)

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			CHANNELEXTRACT_S_U8_41R_16(load, storeu)

			CHANNELEXTRACT_S_U8_41R_8(load)

			CHANNELEXTRACT_S_U8_41TAIL(3)

			sp = sa += slb;
			dp = da += dlb;
		}
	}
}

/* *********************************************************** */

/*
 * extract two channel from a 4-channel image.
 */

void
mlib_s_ImageChannelExtract_U8_42(
    const mlib_u8 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 cmask)
{
	mlib_u8 *sa, *da;
	mlib_u8 *sl, *dl;
	mlib_s32 i, j;

	sa = sl = (void *)src;
	da = dl = dst;

	for (j = 0; j < ysize; j++) {
		if (xsize > 1) {
			da[0] = sa[0];
			sa++;
			da++;
			mlib_s_ImageChannelExtract_S16_21_D1(
			    (mlib_s16 *)sa, (mlib_s16 *)da, (xsize - 1), 1);
			sa += (4 * xsize - 2);
			da += (2 * xsize - 2);
			da[0] = sa[0];
		} else {
			da[0] = sa[0];
			da[1] = sa[3];
		}

		sa = sl += slb;
		da = dl += dlb;
	}
}

/* *********************************************************** */

/*
 * extract the left channel of 2 channels
 */

#define	CHANNELEXTRACT_S_S16_21L(ld, st)                \
	for (; i < dsize - 7; i += 8) {	                \
		__m128i x_sd0, x_sd1;                   \
		__m128i x_st0, x_st1, x_dd;             \
		x_sd0 =                                 \
		    _mm_##ld##_si128((void *)sp);       \
		x_sd1 =                                 \
		    _mm_##ld##_si128((void *)(sp + 8)); \
		                                        \
		x_st0 = _mm_slli_epi32(x_sd0, 16);      \
		x_st1 = _mm_slli_epi32(x_sd1, 16);      \
		x_st0 = _mm_srai_epi32(x_st0, 16);      \
		x_st1 = _mm_srai_epi32(x_st1, 16);      \
		                                        \
		x_dd = _mm_packs_epi32(x_st0, x_st1);   \
		_mm_##st##_si128((__m128i *)dp, x_dd);  \
		dp += 8;                                \
		sp += 16;                               \
	}

#define	CHANNELEXTRACT_S_S16_21L_4(ld)                  \
	if (i < dsize - 3) {	                        \
		__m128i x_sd0, x_st0, x_st1, x_dd;      \
		x_sd0 = _mm_##ld##_si128((__m128i *)sp);\
		                                        \
		x_st0 = _mm_slli_epi32(x_sd0, 16);      \
		x_st1 = _mm_srai_epi32(x_st0, 16);      \
		                                        \
		x_dd = _mm_packs_epi32(x_st1, x_st1);   \
		_mm_storel_epi64((__m128i *)dp, x_dd);  \
		dp += 4;                                \
		sp += 8;                                \
		i += 4;                                 \
	}

/* *********************************************************** */

/*
 * extract the right channel of 2 channels
 */

#define	CHANNELEXTRACT_S_S16_21R(ld, st)                \
	for (; i < dsize - 7; i += 8) {	                \
		__m128i x_sd0, x_sd1;                   \
		__m128i x_st0, x_st1, x_dd;             \
		x_sd0 =                                 \
		    _mm_##ld##_si128((void *)sp);       \
		x_sd1 =                                 \
		    _mm_##ld##_si128((void *)(sp + 8)); \
		                                        \
		x_st0 = _mm_srai_epi32(x_sd0, 16);      \
		x_st1 = _mm_srai_epi32(x_sd1, 16);      \
		x_dd = _mm_packs_epi32(x_st0, x_st1);   \
		_mm_##st##_si128((__m128i *)dp, x_dd);  \
		dp += 8;                                \
		sp += 16;                               \
	}

#define	CHANNELEXTRACT_S_S16_21R_4(ld)                  \
	if (i < dsize - 3) {	                        \
		__m128i x_sd0, x_st0, x_dd;             \
		x_sd0 = _mm_##ld##_si128((__m128i *)sp);\
		x_st0 = _mm_srai_epi32(x_sd0, 16);      \
		x_dd = _mm_packs_epi32(x_st0, x_st0);   \
		_mm_storel_epi64((__m128i *)dp, x_dd);  \
		dp += 4;                                \
		sp += 8;                                \
		i += 4;                                 \
	}

/* *********************************************************** */

/* Head point handling */

#define	CHANNELEXTRACT_S_S16_21HEAD(tap)    \
	for (; (i < dsize) &&               \
	    ((mlib_addr)sp & 15); i++) {    \
		mlib_s16 sd0 = sp[tap];     \
		dp[0] = sd0;                \
		sp += 2;                    \
		dp++;                       \
	}

/* End point handling */

#define	CHANNELEXTRACT_S_S16_21TAIL(tap)    \
	for (; i < dsize; i++) {            \
		mlib_s16 sd0 = sp[tap];     \
		dp[0] = sd0;                \
		sp += 2;                    \
		dp++;                       \
	}

/* *********************************************************** */

/*
 * extract one channel from a 2-channel image.
 * both source and destination image data are 16-byte aligned.
 * both stride is equal to width in bytes.
 */

void
mlib_s_ImageChannelExtract_S16_21_A16D1(
    const mlib_s16 *src,
    mlib_s16 *dst,
    mlib_s32 dsize,
    mlib_s32 cmask)
{
	mlib_s16 *sp, *dp;
	mlib_s32 i = 0;

	sp = (mlib_s16 *)src;
	dp = (mlib_s16 *)dst;

	if (cmask == 2) {

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		CHANNELEXTRACT_S_S16_21L(load, store)

		CHANNELEXTRACT_S_S16_21L_4(load)

		CHANNELEXTRACT_S_S16_21TAIL(0)

	} else {

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		CHANNELEXTRACT_S_S16_21R(load, store)

		CHANNELEXTRACT_S_S16_21R_4(load)

		CHANNELEXTRACT_S_S16_21TAIL(1)
	}
}

/* *********************************************************** */

/*
 * extract one channel from a 2-channel image.
 * both source and destination image data are 16-byte aligned.
 * both source and destination image strides are multiple of 16.
 */

void
mlib_s_ImageChannelExtract_S16_21_A16D2(
    const mlib_s16 *src,
    mlib_s32 slb,
    mlib_s16 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 cmask)
{
	mlib_s16 *sp, *dp;
	mlib_s16 *sa, *da;
	mlib_s32 i, j;
	mlib_s32 dsize = xsize;

	sp = sa = (mlib_s16 *)src;
	dp = da = (mlib_s16 *)dst;

	if (cmask == 2) {
		for (j = 0; j < ysize; j++) {

			i = 0;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			CHANNELEXTRACT_S_S16_21L(load, store)

			CHANNELEXTRACT_S_S16_21L_4(load)

			CHANNELEXTRACT_S_S16_21TAIL(0)

			sp = sa = (mlib_s16 *)((mlib_u8 *)sa + slb);
			dp = da = (mlib_s16 *)((mlib_u8 *)da + dlb);
		}
	} else {
		for (j = 0; j < ysize; j++) {

			i = 0;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			CHANNELEXTRACT_S_S16_21R(load, store)

			CHANNELEXTRACT_S_S16_21R_4(load)

			CHANNELEXTRACT_S_S16_21TAIL(1)

			sp = sa = (mlib_s16 *)((mlib_u8 *)sa + slb);
			dp = da = (mlib_s16 *)((mlib_u8 *)da + dlb);
		}
	}
}

/* *********************************************************** */

/*
 * extract one channel from a 2-channel image.
 * not both source and destination image data are 16-byte aligned.
 * both source and destination image strides are equal to widths in bytes.
 */

void
mlib_s_ImageChannelExtract_S16_21_D1(
    const mlib_s16 *src,
    mlib_s16 *dst,
    mlib_s32 dsize,
    mlib_s32 cmask)
{
	mlib_s16 *sp, *dp;
	mlib_s32 i = 0;

	sp = (mlib_s16 *)src;
	dp = (mlib_s16 *)dst;

	if (cmask == 2) {

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		CHANNELEXTRACT_S_S16_21L(loadu, storeu)

		CHANNELEXTRACT_S_S16_21L_4(loadu)

		CHANNELEXTRACT_S_S16_21TAIL(0)

	} else {

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		CHANNELEXTRACT_S_S16_21R(loadu, storeu)

		CHANNELEXTRACT_S_S16_21R_4(loadu)

		CHANNELEXTRACT_S_S16_21TAIL(1)
	}
}

/* *********************************************************** */

/*
 * extract one channel from a 2-channel image.
 */

void
mlib_s_ImageChannelExtract_S16_21(
    const mlib_s16 *src,
    mlib_s32 slb,
    mlib_s16 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 cmask)
{
	mlib_s16 *sp, *dp;
	mlib_s16 *sa, *da;
	mlib_s32 i, j;
	mlib_s32 dsize = xsize;

	sp = sa = (mlib_s16 *)src;
	dp = da = (mlib_s16 *)dst;

	if (cmask == 2) {
		for (j = 0; j < ysize; j++) {

			i = 0;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			CHANNELEXTRACT_S_S16_21L(loadu, storeu)

			CHANNELEXTRACT_S_S16_21L_4(loadu)

			CHANNELEXTRACT_S_S16_21TAIL(0)

			sp = sa = (mlib_s16 *)((mlib_u8 *)sa + slb);
			dp = da = (mlib_s16 *)((mlib_u8 *)da + dlb);
		}
	} else {
		for (j = 0; j < ysize; j++) {

			i = 0;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			CHANNELEXTRACT_S_S16_21R(loadu, storeu)

			CHANNELEXTRACT_S_S16_21R_4(loadu)

			CHANNELEXTRACT_S_S16_21TAIL(1)

			sp = sa = (mlib_s16 *)((mlib_u8 *)sa + slb);
			dp = da = (mlib_s16 *)((mlib_u8 *)da + dlb);
		}
	}
}

/* *********************************************************** */

/* End point handling */

#define	CHANNELEXTRACT_S_S16_31TAIL(tap)        \
	for (; i < dsize; i++) {                \
		mlib_s16 sd0 = sp[tap];         \
		dp[0] = sd0;                    \
		sp += 3;                        \
		dp++;                           \
	}

/* *********************************************************** */

#define	CHANNELEXTRACT_S_S16_31L(ld, st)                                     \
{                                                                            \
	__m128i x_sd0, x_sd1, x_sd2, x_dd;                                   \
	__m128i x_st0, x_st1, x_st2, x_st3, x_st4;                           \
                                                                             \
	x_sd0 = _mm_##ld##_si128((void *)sp);                                \
	x_sd1 = _mm_##ld##_si128((void *)(sp + 8));                          \
	x_sd2 = _mm_##ld##_si128((void *)(sp + 16));                         \
                                                                             \
	x_st0 = _mm_unpacklo_epi16(x_sd0, _mm_unpackhi_epi64(x_sd1, x_sd1)); \
	x_st1 = _mm_unpackhi_epi16(x_sd0, _mm_unpacklo_epi64(x_sd2, x_sd2)); \
	x_st2 = _mm_unpacklo_epi16(x_sd1, _mm_unpackhi_epi64(x_sd2, x_sd2)); \
                                                                             \
	x_st3 = _mm_unpacklo_epi16(x_st0, _mm_unpackhi_epi64(x_st1, x_st1)); \
	x_st4 = _mm_unpackhi_epi16(x_st0, _mm_unpacklo_epi64(x_st2, x_st2)); \
                                                                             \
	x_dd = _mm_unpacklo_epi16(x_st3, _mm_unpackhi_epi64(x_st4, x_st4));  \
	_mm_##st##_si128((void *)dp, x_dd);                                  \
	sp += 24;                                                            \
	dp += 8;                                                             \
}

#define	CHANNELEXTRACT_S_S16_31M(ld, st)                                     \
{                                                                            \
	__m128i x_sd0, x_sd1, x_sd2, x_dd;                                   \
	__m128i x_st0, x_st1, x_st2, x_st3, x_st4;                           \
                                                                             \
	x_sd0 = _mm_##ld##_si128((void *)sp);                                \
	x_sd1 = _mm_##ld##_si128((void *)(sp + 8));                          \
	x_sd2 = _mm_##ld##_si128((void *)(sp + 16));                         \
                                                                             \
	x_st0 = _mm_unpacklo_epi16(x_sd0, _mm_unpackhi_epi64(x_sd1, x_sd1)); \
	x_st1 = _mm_unpackhi_epi16(x_sd0, _mm_unpacklo_epi64(x_sd2, x_sd2)); \
	x_st2 = _mm_unpacklo_epi16(x_sd1, _mm_unpackhi_epi64(x_sd2, x_sd2)); \
                                                                             \
	x_st3 = _mm_unpacklo_epi16(x_st0, _mm_unpackhi_epi64(x_st1, x_st1)); \
	x_st4 = _mm_unpacklo_epi16(x_st1, _mm_unpackhi_epi64(x_st2, x_st2)); \
	x_dd = _mm_unpackhi_epi16(x_st3, _mm_unpacklo_epi64(x_st4, x_st4));  \
	_mm_##st##_si128((void *)dp, x_dd);                                  \
	sp += 24;                                                            \
	dp += 8;                                                             \
}

#define	CHANNELEXTRACT_S_S16_31R(ld, st)                                     \
{                                                                            \
	__m128i x_sd0, x_sd1, x_sd2, x_dd;                                   \
	__m128i x_st0, x_st1, x_st2, x_st3, x_st4;                           \
                                                                             \
	x_sd0 = _mm_##ld##_si128((void *)sp);                                \
	x_sd1 = _mm_##ld##_si128((void *)(sp + 8));                          \
	x_sd2 = _mm_##ld##_si128((void *)(sp + 16));                         \
                                                                             \
	x_st0 = _mm_unpacklo_epi16(x_sd0, _mm_unpackhi_epi64(x_sd1, x_sd1)); \
	x_st1 = _mm_unpackhi_epi16(x_sd0, _mm_unpacklo_epi64(x_sd2, x_sd2)); \
	x_st2 = _mm_unpacklo_epi16(x_sd1, _mm_unpackhi_epi64(x_sd2, x_sd2)); \
                                                                             \
	x_st3 = _mm_unpackhi_epi16(x_st0, _mm_unpacklo_epi64(x_st2, x_st2)); \
	x_st4 = _mm_unpacklo_epi16(x_st1, _mm_unpackhi_epi64(x_st2, x_st2)); \
	x_dd = _mm_unpacklo_epi16(x_st3, _mm_unpackhi_epi64(x_st4, x_st4));  \
	_mm_##st##_si128((void *)dp, x_dd);                                  \
	sp += 24;                                                            \
	dp += 8;                                                             \
}

/* *********************************************************** */

/*
 * extract one channel from a 3-channel image.
 * both source and destination image data are 16-byte aligned.
 * both stride is equal to width in bytes.
 */

void
mlib_s_ImageChannelExtract_S16_31_A16D1(
    const mlib_s16 *src,
    mlib_s16 *dst,
    mlib_s32 dsize,
    mlib_s32 cmask)
{
	mlib_s16 *sp, *dp;
	mlib_s32 i;

	sp = (mlib_s16 *)src;
	dp = dst;

	if (cmask == 4) {

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < dsize - 7; i += 8) {
			CHANNELEXTRACT_S_S16_31L(load, store)
		}

		CHANNELEXTRACT_S_S16_31TAIL(0)

	} else if (cmask == 2) {

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < dsize - 7; i += 8) {
			CHANNELEXTRACT_S_S16_31M(load, store)
		}

		CHANNELEXTRACT_S_S16_31TAIL(1)

	} else {

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < dsize - 7; i += 8) {
			CHANNELEXTRACT_S_S16_31R(load, store)
		}

		CHANNELEXTRACT_S_S16_31TAIL(2)
	}
}

/* *********************************************************** */

/*
 * extract one channel from a 3-channel image.
 * both source and destination image data are 16-byte aligned.
 * both source and destination image strides are multiple of 16.
 */

void
mlib_s_ImageChannelExtract_S16_31_A16D2(
    const mlib_s16 *src,
    mlib_s32 slb,
    mlib_s16 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 cmask)
{
	mlib_s16 *sp, *dp;
	mlib_s16 *sa, *da;
	mlib_s32 i, j;
	mlib_s32 dsize = xsize;

	sp = sa = (mlib_s16 *)src;
	dp = da = dst;

	if (cmask == 4) {

		for (j = 0; j < ysize; j++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < dsize - 7; i += 8) {
				CHANNELEXTRACT_S_S16_31L(load, store)
			}

			CHANNELEXTRACT_S_S16_31TAIL(0)

			sp = sa = (mlib_s16 *)((mlib_u8 *)sa + slb);
			dp = da = (mlib_s16 *)((mlib_u8 *)da + dlb);
		}

	} else if (cmask == 2) {

		for (j = 0; j < ysize; j++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < dsize - 7; i += 8) {
				CHANNELEXTRACT_S_S16_31M(load, store)
			}

			CHANNELEXTRACT_S_S16_31TAIL(1)

			sp = sa = (mlib_s16 *)((mlib_u8 *)sa + slb);
			dp = da = (mlib_s16 *)((mlib_u8 *)da + dlb);
		}

	} else {

		for (j = 0; j < ysize; j++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < dsize - 7; i += 8) {
				CHANNELEXTRACT_S_S16_31R(load, store)
			}

			CHANNELEXTRACT_S_S16_31TAIL(2)

			sp = sa = (mlib_s16 *)((mlib_u8 *)sa + slb);
			dp = da = (mlib_s16 *)((mlib_u8 *)da + dlb);
		}
	}
}

/* *********************************************************** */

/*
 * extract one channel from a 3-channel image.
 * both stride is equal to width in bytes.
 */

void
mlib_s_ImageChannelExtract_S16_31_D1(
    const mlib_s16 *src,
    mlib_s16 *dst,
    mlib_s32 dsize,
    mlib_s32 cmask)
{
	mlib_s16 *sp, *dp;
	mlib_s32 i;

	sp = (mlib_s16 *)src;
	dp = dst;

	if (cmask == 4) {

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < dsize - 7; i += 8) {
			CHANNELEXTRACT_S_S16_31L(loadu, storeu)
		}

		CHANNELEXTRACT_S_S16_31TAIL(0)

	} else if (cmask == 2) {

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < dsize - 7; i += 8) {
			CHANNELEXTRACT_S_S16_31M(loadu, storeu)
		}

		CHANNELEXTRACT_S_S16_31TAIL(1)

	} else {

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < dsize - 7; i += 8) {
			CHANNELEXTRACT_S_S16_31R(loadu, storeu)
		}

		CHANNELEXTRACT_S_S16_31TAIL(2)
	}
}

/* *********************************************************** */

/*
 * extract one channel from a 3-channel image.
 */

void
mlib_s_ImageChannelExtract_S16_31(
    const mlib_s16 *src,
    mlib_s32 slb,
    mlib_s16 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 cmask)
{
	mlib_s16 *sp, *dp;
	mlib_s16 *sa, *da;
	mlib_s32 i, j;
	mlib_s32 dsize = xsize;

	sp = sa = (mlib_s16 *)src;
	dp = da = dst;

	if (cmask == 4) {

		for (j = 0; j < ysize; j++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < dsize - 7; i += 8) {
				CHANNELEXTRACT_S_S16_31L(loadu, storeu)
			}

			CHANNELEXTRACT_S_S16_31TAIL(0)

			sp = sa = (mlib_s16 *)((mlib_u8 *)sa + slb);
			dp = da = (mlib_s16 *)((mlib_u8 *)da + dlb);
		}

	} else if (cmask == 2) {

		for (j = 0; j < ysize; j++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < dsize - 7; i += 8) {
				CHANNELEXTRACT_S_S16_31M(loadu, storeu)
			}

			CHANNELEXTRACT_S_S16_31TAIL(1)
			sp = sa = (mlib_s16 *)((mlib_u8 *)sa + slb);
			dp = da = (mlib_s16 *)((mlib_u8 *)da + dlb);
		}

	} else {

		for (j = 0; j < ysize; j++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < dsize - 7; i += 8) {
				CHANNELEXTRACT_S_S16_31R(loadu, storeu)
			}

			CHANNELEXTRACT_S_S16_31TAIL(2)
			sp = sa = (mlib_s16 *)((mlib_u8 *)sa + slb);
			dp = da = (mlib_s16 *)((mlib_u8 *)da + dlb);
		}
	}
}

/* *********************************************************** */

/*
 * extract the left channel of 4 channels
 */

#define	CHANNELEXTRACT_S_S16_41L(ld, st, nsize)              \
	for (; i < dsize - 7; i += 8) {                      \
		__m128i x_sd0, x_sd1, x_sd2, x_sd3;          \
		__m128i x_st0, x_st1, x_st2, x_st3;          \
		__m128i x_st4, x_st5, x_dd;                  \
		x_sd0 = _mm_##ld##_si128((void *)sp);        \
		x_sd1 = _mm_##ld##_si128((void *)(sp + 8));  \
		x_sd2 = _mm_##ld##_si128((void *)(sp + 16)); \
		x_sd3 = _mm_##ld##_si128((void *)(sp + 24)); \
		                                             \
		x_st0 = _mm_unpacklo_epi16(x_sd0, x_sd2);    \
		x_st1 = _mm_unpackhi_epi16(x_sd0, x_sd2);    \
		x_st2 = _mm_unpacklo_epi16(x_sd1, x_sd3);    \
		x_st3 = _mm_unpackhi_epi16(x_sd1, x_sd3);    \
		x_st4 = _mm_unpacklo_epi16(x_st0, x_st2);    \
		x_st5 = _mm_unpacklo_epi16(x_st1, x_st3);    \
		x_dd = _mm_unpacklo_epi16(x_st4, x_st5);     \
		_mm_##st##_si128((void *)dp, x_dd);          \
		dp += 8;                                     \
		sp += 32;                                    \
	}

/* *********************************************************** */

/*
 * extract the middle left channel of 4 channels
 */

#define	CHANNELEXTRACT_S_S16_41ML(ld, st, nsize)                \
	for (; i < dsize - 7; i += 8) {                         \
		__m128i x_sd0, x_sd1, x_sd2, x_sd3;             \
		__m128i x_st0, x_st1, x_st2, x_st3;             \
		__m128i x_st4, x_st5, x_dd;                     \
		x_sd0 = _mm_##ld##_si128((void *)sp);           \
		x_sd1 = _mm_##ld##_si128((void *)(sp + 8));     \
		x_sd2 = _mm_##ld##_si128((void *)(sp + 16));    \
		x_sd3 = _mm_##ld##_si128((void *)(sp + 24));    \
		                                                \
		x_st0 = _mm_unpacklo_epi16(x_sd0, x_sd2);       \
		x_st1 = _mm_unpackhi_epi16(x_sd0, x_sd2);       \
		x_st2 = _mm_unpacklo_epi16(x_sd1, x_sd3);       \
		x_st3 = _mm_unpackhi_epi16(x_sd1, x_sd3);       \
		x_st4 = _mm_unpacklo_epi16(x_st0, x_st2);       \
		x_st5 = _mm_unpacklo_epi16(x_st1, x_st3);       \
		x_dd = _mm_unpackhi_epi16(x_st4, x_st5);        \
		_mm_##st##_si128((void *)dp, x_dd);             \
		dp += 8;                                        \
		sp += 32;                                       \
	}

/* *********************************************************** */

/*
 * extract the middle right channel of 4 channels
 */

#define	CHANNELEXTRACT_S_S16_41MR(ld, st, nsize)                \
	for (; i < dsize - 7; i += 8) {                         \
		__m128i x_sd0, x_sd1, x_sd2, x_sd3;             \
		__m128i x_st0, x_st1, x_st2, x_st3;             \
		__m128i x_st4, x_st5, x_dd;                     \
		x_sd0 = _mm_##ld##_si128((void *)sp);           \
		x_sd1 = _mm_##ld##_si128((void *)(sp + 8));     \
		x_sd2 = _mm_##ld##_si128((void *)(sp + 16));    \
		x_sd3 = _mm_##ld##_si128((void *)(sp + 24));    \
		                                                \
		x_st0 = _mm_unpacklo_epi16(x_sd0, x_sd2);       \
		x_st1 = _mm_unpackhi_epi16(x_sd0, x_sd2);       \
		x_st2 = _mm_unpacklo_epi16(x_sd1, x_sd3);       \
		x_st3 = _mm_unpackhi_epi16(x_sd1, x_sd3);       \
		x_st4 = _mm_unpackhi_epi16(x_st0, x_st2);       \
		x_st5 = _mm_unpackhi_epi16(x_st1, x_st3);       \
		x_dd = _mm_unpacklo_epi16(x_st4, x_st5);        \
		_mm_##st##_si128((void *)dp, x_dd);             \
		dp += 8;                                        \
		sp += 32;                                       \
	}

/* *********************************************************** */

/*
 * extract the right channel of 4 channels
 */

#define	CHANNELEXTRACT_S_S16_41R(ld, st, nsize)                 \
	for (; i < dsize - 7; i += 8) {                         \
		__m128i x_sd0, x_sd1, x_sd2, x_sd3;             \
		__m128i x_st0, x_st1, x_st2, x_st3;             \
		__m128i x_st4, x_st5, x_dd;                     \
		x_sd0 = _mm_##ld##_si128((void *)sp);           \
		x_sd1 = _mm_##ld##_si128((void *)(sp + 8));     \
		x_sd2 = _mm_##ld##_si128((void *)(sp + 16));    \
		x_sd3 = _mm_##ld##_si128((void *)(sp + 24));    \
		                                                \
		x_st0 = _mm_unpacklo_epi16(x_sd0, x_sd2);       \
		x_st1 = _mm_unpackhi_epi16(x_sd0, x_sd2);       \
		x_st2 = _mm_unpacklo_epi16(x_sd1, x_sd3);       \
		x_st3 = _mm_unpackhi_epi16(x_sd1, x_sd3);       \
		x_st4 = _mm_unpackhi_epi16(x_st0, x_st2);       \
		x_st5 = _mm_unpackhi_epi16(x_st1, x_st3);       \
		x_dd = _mm_unpackhi_epi16(x_st4, x_st5);        \
		_mm_##st##_si128((void *)dp, x_dd);             \
		dp += 8;                                        \
		sp += 32;                                       \
	}

/* *********************************************************** */

/* End point handling */

#define	CHANNELEXTRACT_S_S16_41TAIL(tap)            \
	for (; i < dsize; i++) {                    \
		mlib_s16 sd0 = sp[tap];             \
		dp[0] = sd0;                        \
		sp += 4;                            \
		dp++;                               \
	}

/* *********************************************************** */

/*
 * extract one channel from a 4-channel image.
 * both source and destination image data are 16-byte aligned.
 * both stride is equal to width in bytes.
 */

void
mlib_s_ImageChannelExtract_S16_41_A16D1(
    const mlib_s16 *src,
    mlib_s16 *dst,
    mlib_s32 dsize,
    mlib_s32 cmask)
{
	mlib_s16 *sp, *dp;
	sp = (mlib_s16 *)src;
	dp = (mlib_s16 *)dst;
	mlib_s32 i = 0;

	if (cmask == 8) {

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		CHANNELEXTRACT_S_S16_41L(load, store, dsize);

		CHANNELEXTRACT_S_S16_41TAIL(0)

	} else if (cmask == 4) {

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		CHANNELEXTRACT_S_S16_41ML(load, store, dsize);

		CHANNELEXTRACT_S_S16_41TAIL(1)

	} else if (cmask == 2) {

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		CHANNELEXTRACT_S_S16_41MR(load, store, dsize);

		CHANNELEXTRACT_S_S16_41TAIL(2)

	} else {

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		CHANNELEXTRACT_S_S16_41R(load, store, dsize);

		CHANNELEXTRACT_S_S16_41TAIL(3)
	}
}

/* *********************************************************** */

/*
 * extract one channel from a 4-channel image.
 * both source and destination image data are 16-byte aligned.
 * both source and destination image strides are multiple of 16.
 */

void
mlib_s_ImageChannelExtract_S16_41_A16D2(
    const mlib_s16 *src,
    mlib_s32 slb,
    mlib_s16 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 cmask)
{
	mlib_s32 i, j;
	mlib_s32 dsize = xsize;

	mlib_s16 *sp, *dp;
	mlib_s16 *sa, *da;
	sp = sa = (mlib_s16 *)src;
	dp = da = (mlib_s16 *)dst;

	if (cmask == 8) {
		for (j = 0; j < ysize; j++) {

			i = 0;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			CHANNELEXTRACT_S_S16_41L(load, store, xsize);

			CHANNELEXTRACT_S_S16_41TAIL(0)

			sp = sa = (mlib_s16 *)((mlib_u8 *)sa + slb);
			dp = da = (mlib_s16 *)((mlib_u8 *)da + dlb);
		}
	} else if (cmask == 4) {
		for (j = 0; j < ysize; j++) {

			i = 0;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			CHANNELEXTRACT_S_S16_41ML(load, store, xsize);

			CHANNELEXTRACT_S_S16_41TAIL(1)

			sp = sa = (mlib_s16 *)((mlib_u8 *)sa + slb);
			dp = da = (mlib_s16 *)((mlib_u8 *)da + dlb);
		}
	} else if (cmask == 2) {
		for (j = 0; j < ysize; j++) {

			i = 0;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			CHANNELEXTRACT_S_S16_41MR(load, store, xsize);

			CHANNELEXTRACT_S_S16_41TAIL(2)

			sp = sa = (mlib_s16 *)((mlib_u8 *)sa + slb);
			dp = da = (mlib_s16 *)((mlib_u8 *)da + dlb);
		}
	} else {

		for (j = 0; j < ysize; j++) {

			i = 0;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			CHANNELEXTRACT_S_S16_41R(load, store, xsize);

			CHANNELEXTRACT_S_S16_41TAIL(3)

			sp = sa = (mlib_s16 *)((mlib_u8 *)sa + slb);
			dp = da = (mlib_s16 *)((mlib_u8 *)da + dlb);
		}
	}
}

/* *********************************************************** */

/*
 * extract one channel from a 4-channel image.
 * both stride is equal to width in bytes.
 */

void
mlib_s_ImageChannelExtract_S16_41_D1(
    const mlib_s16 *src,
    mlib_s16 *dst,
    mlib_s32 dsize,
    mlib_s32 cmask)
{
	mlib_s16 *sp, *dp;
	sp = (mlib_s16 *)src;
	dp = (mlib_s16 *)dst;
	mlib_s32 i = 0;

	if (cmask == 8) {

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		CHANNELEXTRACT_S_S16_41L(loadu, storeu, dsize);

		CHANNELEXTRACT_S_S16_41TAIL(0)

	} else if (cmask == 4) {

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		CHANNELEXTRACT_S_S16_41ML(loadu, storeu, dsize);

		CHANNELEXTRACT_S_S16_41TAIL(1)

	} else if (cmask == 2) {

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		CHANNELEXTRACT_S_S16_41MR(loadu, storeu, dsize);

		CHANNELEXTRACT_S_S16_41TAIL(2)

	} else {

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		CHANNELEXTRACT_S_S16_41R(loadu, storeu, dsize);

		CHANNELEXTRACT_S_S16_41TAIL(3)
	}
}

/* *********************************************************** */

/*
 * extract one channel from a 4-channel image.
 */

void
mlib_s_ImageChannelExtract_S16_41(
    const mlib_s16 *src,
    mlib_s32 slb,
    mlib_s16 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 cmask)
{
	mlib_s32 i, j;
	mlib_s32 dsize = xsize;

	mlib_s16 *sp, *dp;
	mlib_s16 *sa, *da;
	sp = sa = (mlib_s16 *)src;
	dp = da = (mlib_s16 *)dst;

	if (cmask == 8) {
		for (j = 0; j < ysize; j++) {

			i = 0;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			CHANNELEXTRACT_S_S16_41L(loadu, storeu, xsize);

			CHANNELEXTRACT_S_S16_41TAIL(0)

			sp = sa = (mlib_s16 *)((mlib_u8 *)sa + slb);
			dp = da = (mlib_s16 *)((mlib_u8 *)da + dlb);
		}
	} else if (cmask == 4) {
		for (j = 0; j < ysize; j++) {

			i = 0;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			CHANNELEXTRACT_S_S16_41ML(loadu, storeu, xsize);

			CHANNELEXTRACT_S_S16_41TAIL(1)

			sp = sa = (mlib_s16 *)((mlib_u8 *)sa + slb);
			dp = da = (mlib_s16 *)((mlib_u8 *)da + dlb);
		}
	} else if (cmask == 2) {
		for (j = 0; j < ysize; j++) {

			i = 0;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			CHANNELEXTRACT_S_S16_41MR(loadu, storeu, xsize);

			CHANNELEXTRACT_S_S16_41TAIL(2)

			sp = sa = (mlib_s16 *)((mlib_u8 *)sa + slb);
			dp = da = (mlib_s16 *)((mlib_u8 *)da + dlb);
		}
	} else {

		for (j = 0; j < ysize; j++) {

			i = 0;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			CHANNELEXTRACT_S_S16_41R(loadu, storeu, xsize);

			CHANNELEXTRACT_S_S16_41TAIL(3)

			sp = sa = (mlib_s16 *)((mlib_u8 *)sa + slb);
			dp = da = (mlib_s16 *)((mlib_u8 *)da + dlb);
		}
	}
}

/* *********************************************************** */
