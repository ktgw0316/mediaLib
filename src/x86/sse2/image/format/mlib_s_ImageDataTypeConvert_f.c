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

#pragma ident	"@(#)mlib_s_ImageDataTypeConvert_f.c	9.4	07/11/05 SMI"

/*
 * FUNCTIONS
 *      mlib_s_ImageDataTypeConvert_BIT_U8         - 2-D, not aligned16
 *      mlib_s_ImageDataTypeConvert_BIT_S16        - 2-D, not aligned16
 *      mlib_s_ImageDataTypeConvert_BIT_S32        - 2-D, not aligned16
 *
 *      mlib_s_ImageDataTypeConvert_BIT_F32_A8D1X8 - 1-D, Aligned16
 *      mlib_s_ImageDataTypeConvert_BIT_F32_A8D2X8 - 2-D, Aligned16
 *      mlib_s_ImageDataTypeConvert_BIT_F32_D1     - 1-D, not aligned16
 *      mlib_s_ImageDataTypeConvert_BIT_F32        - 2-D, not aligned16
 *
 *      mlib_s_ImageDataTypeConvert_BIT_D64        - 2-D, not aligned16
 *
 *      mlib_s_ImageDataTypeConvert_U8_BIT_A8D1X8  - 1-D, Aligned16
 *      mlib_s_ImageDataTypeConvert_U8_BIT_A8D2X8  - 2-D, Aligned16
 *      mlib_s_ImageDataTypeConvert_U8_BIT_D1      - 1-D, not aligned16
 *      mlib_s_ImageDataTypeConvert_U8_BIT         - 2-D, not aligned16
 *
 *      mlib_s_ImageDataTypeConvert_U8_S16_A8D1X8  - 1-D, Aligned16
 *      mlib_s_ImageDataTypeConvert_U8_S16_A8D2X8  - 2-D, Aligned16
 *      mlib_s_ImageDataTypeConvert_U8_S16_D1      - 1-D, not aligned16
 *      mlib_s_ImageDataTypeConvert_U8_S16         - 2-D, not aligned16
 *
 *      mlib_s_ImageDataTypeConvert_U8_S32_A8D1X8  - 1-D, Aligned16
 *      mlib_s_ImageDataTypeConvert_U8_S32_A8D2X8  - 2-D, Aligned16
 *      mlib_s_ImageDataTypeConvert_U8_S32_D1      - 1-D, not aligned16
 *      mlib_s_ImageDataTypeConvert_U8_S32         - 2-D, not aligned16
 *
 *      mlib_s_ImageDataTypeConvert_U8_F32_A8D1X8  - 1-D, Aligned16
 *      mlib_s_ImageDataTypeConvert_U8_F32_A8D2X8  - 2-D, Aligned16
 *      mlib_s_ImageDataTypeConvert_U8_F32_D1      - 1-D, not aligned16
 *      mlib_s_ImageDataTypeConvert_U8_F32         - 2-D, not aligned16
 *
 *      mlib_s_ImageDataTypeConvert_U8_D64_A8D1X8  - 1-D, Aligned16
 *      mlib_s_ImageDataTypeConvert_U8_D64_A8D2X8  - 2-D, Aligned16
 *      mlib_s_ImageDataTypeConvert_U8_D64_D1      - 1-D, not aligned16
 *      mlib_s_ImageDataTypeConvert_U8_D64         - 2-D, not aligned16
 *
 *      mlib_s_ImageDataTypeConvert_U16_BIT_A8D1X8 - 1-D, Aligned16
 *      mlib_s_ImageDataTypeConvert_U16_BIT_A8D2X8 - 2-D, Aligned16
 *      mlib_s_ImageDataTypeConvert_U16_BIT_D1     - 1-D, Aligned16
 *      mlib_s_ImageDataTypeConvert_U16_BIT        - 2-D, not aligned16
 *
 *      mlib_s_ImageDataTypeConvert_U16_U8_A8D1X8  - 1-D, Aligned16
 *      mlib_s_ImageDataTypeConvert_U16_U8_A8D2X8  - 2-D, Aligned16
 *      mlib_s_ImageDataTypeConvert_U16_U8_D1      - 1-D, not aligned16
 *      mlib_s_ImageDataTypeConvert_U16_U8         - 2-D, not aligned16
 *
 *      mlib_s_ImageDataTypeConvert_U16_S16_A8D1X8 - 1-D, Aligned16
 *      mlib_s_ImageDataTypeConvert_U16_S16_A8D2X8 - 2-D, Aligned16
 *      mlib_s_ImageDataTypeConvert_U16_S16_D1     - 1-D, not aligned16
 *      mlib_s_ImageDataTypeConvert_U16_S16        - 2-D, not aligned16
 *
 *      mlib_s_ImageDataTypeConvert_U16_S32_A8D1X8 - 1-D, Aligned16
 *      mlib_s_ImageDataTypeConvert_U16_S32_A8D2X8 - 2-D, Aligned16
 *      mlib_s_ImageDataTypeConvert_U16_S32_D1     - 1-D, not aligned16
 *      mlib_s_ImageDataTypeConvert_U16_S32        - 2-D, not aligned16
 *
 *      mlib_s_ImageDataTypeConvert_U16_F32_A8D1X8 - 1-D, Aligned16
 *      mlib_s_ImageDataTypeConvert_U16_F32_A8D2X8 - 2-D, Aligned16
 *      mlib_s_ImageDataTypeConvert_U16_F32_D1     - 1-D, not aligned16
 *      mlib_s_ImageDataTypeConvert_U16_F32        - 2-D, not aligned16
 *
 *      mlib_s_ImageDataTypeConvert_U16_D64_A8D1X8 - 1-D, Aligned16
 *      mlib_s_ImageDataTypeConvert_U16_D64_A8D2X8 - 2-D, Aligned16
 *      mlib_s_ImageDataTypeConvert_U16_D64_D1     - 1-D, not aligned16
 *      mlib_s_ImageDataTypeConvert_U16_D64        - 2-D, not aligned16
 *
 *      mlib_s_ImageDataTypeConvert_S16_BIT_A8D1X8 - 1-D, Aligned16
 *      mlib_s_ImageDataTypeConvert_S16_BIT_A8D2X8 - 2-D, Aligned16
 *      mlib_s_ImageDataTypeConvert_S16_BIT_D1     - 1-D, Aligned16
 *      mlib_s_ImageDataTypeConvert_S16_BIT        - 2-D, not aligned16
 *
 *      mlib_s_ImageDataTypeConvert_S16_U8_A8D1X8  - 1-D, Aligned16
 *      mlib_s_ImageDataTypeConvert_S16_U8_A8D2X8  - 2-D, Aligned16
 *      mlib_s_ImageDataTypeConvert_S16_U8_D1      - 1-D, not aligned16
 *      mlib_s_ImageDataTypeConvert_S16_U8         - 2-D, not aligned16
 *
 *      mlib_s_ImageDataTypeConvert_S16_U16_A8D1X8  - 1-D, Aligned16
 *      mlib_s_ImageDataTypeConvert_S16_U16_A8D2X8  - 2-D, Aligned16
 *      mlib_s_ImageDataTypeConvert_S16_U16_D1      - 1-D, not aligned16
 *      mlib_s_ImageDataTypeConvert_S16_U16         - 2-D, not aligned16
 *
 *      mlib_s_ImageDataTypeConvert_S16_S32_A8D1X8 - 1-D, Aligned16
 *      mlib_s_ImageDataTypeConvert_S16_S32_A8D2X8 - 2-D, Aligned16
 *      mlib_s_ImageDataTypeConvert_S16_S32_D1     - 1-D, not aligned16
 *      mlib_s_ImageDataTypeConvert_S16_S32        - 2-D, not aligned16
 *
 *      mlib_s_ImageDataTypeConvert_S16_F32_A8D1X8 - 1-D, Aligned16
 *      mlib_s_ImageDataTypeConvert_S16_F32_A8D2X8 - 2-D, Aligned16
 *      mlib_s_ImageDataTypeConvert_S16_F32_D1     - 1-D, not aligned16
 *      mlib_s_ImageDataTypeConvert_S16_F32        - 2-D, not aligned16
 *
 *      mlib_s_ImageDataTypeConvert_S16_D64_A8D1X8 - 1-D, Aligned16
 *      mlib_s_ImageDataTypeConvert_S16_D64_A8D2X8 - 2-D, Aligned16
 *      mlib_s_ImageDataTypeConvert_S16_D64_D1     - 1-D, not aligned16
 *      mlib_s_ImageDataTypeConvert_S16_D64        - 2-D, not aligned16
 *
 *      mlib_s_ImageDataTypeConvert_S32_BIT_A8D1X8 - 1-D, Aligned16
 *      mlib_s_ImageDataTypeConvert_S32_BIT_A8D2X8 - 2-D, Aligned16
 *      mlib_s_ImageDataTypeConvert_S32_BIT_D1     - 2-D, Aligned16
 *      mlib_s_ImageDataTypeConvert_S32_BIT        - 2-D, not aligned16
 *
 *      mlib_s_ImageDataTypeConvert_S32_U8_A8D1X8  - 1-D, Aligned16
 *      mlib_s_ImageDataTypeConvert_S32_U8_A8D2X8  - 2-D, Aligned16
 *      mlib_s_ImageDataTypeConvert_S32_U8_D1      - 1-D, not aligned16
 *      mlib_s_ImageDataTypeConvert_S32_U8         - 2-D, not aligned16
 *
 *      mlib_s_ImageDataTypeConvert_S32_U16_A8D1X8 - 1-D, Aligned16
 *      mlib_s_ImageDataTypeConvert_S32_U16_A8D2X8 - 2-D, Aligned16
 *      mlib_s_ImageDataTypeConvert_S32_U16_D1     - 1-D, not aligned16
 *      mlib_s_ImageDataTypeConvert_S32_U16        - 2-D, not aligned16
 *
 *      mlib_s_ImageDataTypeConvert_S32_S16_A8D1X8 - 1-D, Aligned16
 *      mlib_s_ImageDataTypeConvert_S32_S16_A8D2X8 - 2-D, Aligned16
 *      mlib_s_ImageDataTypeConvert_S32_S16_D1     - 1-D, not aligned16
 *      mlib_s_ImageDataTypeConvert_S32_S16        - 2-D, not aligned16
 *
 *      mlib_s_ImageDataTypeConvert_S32_F32_A8D1X8 - 1-D, Aligned16
 *      mlib_s_ImageDataTypeConvert_S32_F32_A8D2X8 - 2-D, Aligned16
 *      mlib_s_ImageDataTypeConvert_S32_F32_D1     - 1-D, not aligned16
 *      mlib_s_ImageDataTypeConvert_S32_F32        - 2-D, not aligned16
 *
 *      mlib_s_ImageDataTypeConvert_S32_D64_A8D1X8 - 1-D, Aligned16
 *      mlib_s_ImageDataTypeConvert_S32_D64_A8D2X8 - 2-D, Aligned16
 *      mlib_s_ImageDataTypeConvert_S32_D64_D1     - 1-D, not aligned16
 *      mlib_s_ImageDataTypeConvert_S32_D64        - 2-D, not aligned16
 *
 *      mlib_s_ImageDataTypeConvert_F32_BIT_A8D1X8 - 1-D, Aligned16
 *      mlib_s_ImageDataTypeConvert_F32_BIT_A8D2X8 - 2-D, Aligned16
 *      mlib_s_ImageDataTypeConvert_F32_BIT_D1     - 1-D, not aligned16
 *      mlib_s_ImageDataTypeConvert_F32_BIT        - 2-D, not aligned16
 *
 *      mlib_s_ImageDataTypeConvert_F32_U8         - 2-D, not aligned16
 *      mlib_s_ImageDataTypeConvert_F32_U16        - 2-D, not aligned16
 *      mlib_s_ImageDataTypeConvert_F32_S16        - 2-D, not aligned16
 *      mlib_s_ImageDataTypeConvert_F32_S32        - 2-D, not aligned16
 *      mlib_s_ImageDataTypeConvert_F32_D64        - 2-D, not aligned16
 *
 *      mlib_s_ImageDataTypeConvert_D64_BIT        - 2-D, not aligned16
 *      mlib_s_ImageDataTypeConvert_D64_U8         - 2-D, not aligned16
 *      mlib_s_ImageDataTypeConvert_D64_U16        - 2-D, not aligned16
 *      mlib_s_ImageDataTypeConvert_D64_S16        - 2-D, not aligned16
 *      mlib_s_ImageDataTypeConvert_D64_S32        - 2-D, not aligned16
 *      mlib_s_ImageDataTypeConvert_D64_F32        - 2-D, not aligned16
 *
 * SYNOPSIS
 *
 * ARGUMENT
 *      src     pointer to source image data
 *      dst     pointer to destination image data
 *      dsize   data size in pixels
 *      slb     source image line stride in bytes
 *      dlb     destination image line stride in bytes
 *      xsize   image width in pixels
 *      ysize   image height in lines
 *
 * RESTRICTION
 *      dst and src must have the same size and number of channels.
 *      They can have 1, 2, 3 or 4 channels of MLIB_BIT, MLIB_BYTE, MLIB_SHORT,
 *      MLIB_USHORT, MLIB_INT, MLIB_FLOAT or MLIB_DOUBLE data type.
 *
 * DESCRIPTION
 *      Data format conversion.
 *      These functions are separated from mlib_s_ImageDataTypeConvert.c
 *      for loop unrolling and structure clarity.
 */

#include <mlib_image.h>
#include <mlib_ImageDivTables.h>
#include <mlib_s_ImageDataTypeConvert.h>

#ifdef __SUNPRO_C
#include <sunmedia_intrin.h>
#else
#include <emmintrin.h>
#endif

#include <mlib_sse_utils.h>

/* *********************************************************** */

#if defined(__ICL) || defined(__INTEL_COMPILER)
	_declspec(align(8))
#elif __SUNPRO_C
#pragma align 8(mlib_table_F32)
#endif /* __ICL || INTEL_COMPILER */
static const mlib_f32 mlib_table_F32[8] =
{ 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f };

/* *********************************************************** */

#ifdef i386 /* do not perform the copying by mlib_d64 data type for x86 */

typedef struct
{
    mlib_s32 int0, int1;
} two_int;

#define	TYPE_64BIT  two_int

#else /* i386 ( do not perform the copying by mlib_d64 data type for x86 ) */

#define	TYPE_64BIT  mlib_d64
#endif /* i386 ( do not perform the copying by mlib_d64 data type for x86 ) */

/* *********************************************************** */

typedef union
{
	TYPE_64BIT d64;
	struct
	{
		mlib_f32 f0;
		mlib_f32 f1;
	} f32s;
	struct
	{
		mlib_s32 f0;
		mlib_s32 f1;
	} i32s;
} d64_2x32;

/* *********************************************************** */

#define	SIZE	128

/* *********************************************************** */

#define	SAT8(DST, SRC)                                  \
	if (SRC >= MLIB_U8_MAX) {                       \
	    DST = MLIB_U8_MAX;                          \
	} else if (SRC <= MLIB_U8_MIN) {                \
		DST = MLIB_U8_MIN;                      \
	} else {                                        \
		DST = (mlib_u8)SRC;                     \
	}

#define	SAT_U16(DST, SRC)                               \
	{                                               \
	    mlib_d64 s0 = (mlib_d64)(SRC);              \
	                                                \
	    if (s0 > (mlib_d64)MLIB_U16_MAX)            \
			s0 = (mlib_d64)MLIB_U16_MAX;    \
	    if (s0 < (mlib_d64)MLIB_U16_MIN)            \
			s0 = (mlib_d64)MLIB_U16_MIN;    \
	    (DST) = (mlib_s32)s0;                       \
	}

#define	SAT16(DST, SRC)                                 \
	if (SRC >= MLIB_S16_MAX) {                      \
	    DST = MLIB_S16_MAX;                         \
	} else if (SRC <= MLIB_S16_MIN) {               \
		DST = MLIB_S16_MIN;                     \
	} else {                                        \
	    DST = (mlib_s16)SRC;                        \
	}

#define	SAT32(DST, SRC)                                 \
	if (SRC >= MLIB_S32_MAX) {                      \
	    DST = MLIB_S32_MAX;                         \
	} else if (SRC <= MLIB_S32_MIN) {               \
		DST = MLIB_S32_MIN;                     \
	} else {                                        \
		DST = (mlib_s32)SRC;                    \
	}

/* *********************************************************** */

/*
 * source and destination image data are 1-d vectors.
 * source and destination image data are 16-byte aligned.
 */

void
mlib_s_ImageDataTypeConvert_U8_S16_A8D1X8(
    const mlib_u8 *src,
    mlib_s16 *dst,
    mlib_s32 dsize)
{
/* pointer for pixel in source */
	mlib_u8 *sp;

/* pointer for pixel in destination */
	mlib_s16 *dp;

/* pointer for pixel in source */
	mlib_u16 *sp2;

/* pointer for pixel in destination */
	mlib_s32 *dp2;

	__m128i x_sd;
	__m128i x_dd0, x_dd1;
	mlib_u32 val0;
	mlib_s32 i;

	const __m128i x_zero = _mm_setzero_si128();

	sp = (mlib_u8 *)src;
	dp = dst;

/* 16 pixels */
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < (dsize - 15); i += 16) {
		x_sd = _mm_load_si128((__m128i *)sp);
		x_dd0 = _mm_unpacklo_epi8(x_sd, x_zero);
		x_dd1 = _mm_unpackhi_epi8(x_sd, x_zero);
		_mm_store_si128((__m128i *)dp, x_dd0);
		dp += 8;
		_mm_store_si128((__m128i *)dp, x_dd1);
		dp += 8;
		sp += 16;
	}

/* 8 pixels */
	if (i < (dsize - 7)) {
		x_sd = _mm_loadl_epi64((__m128i *)sp);
		x_dd0 = _mm_unpacklo_epi8(x_sd, x_zero);
		_mm_store_si128((__m128i *)dp, x_dd0);
		dp += 8;
		sp += 8;
		i += 8;
	}

/* 2 pixels */
	sp2 = (mlib_u16 *)sp;
	dp2 = (mlib_s32 *)dp;
	for (; i < (dsize - 1); i += 2) {
		val0 = (*sp2++);
		dp2[0] = ((val0 & 0xFF00) << 8) | (val0 & 0xFF);
		dp2++;
	}

/* 1 pixels */
	sp = (mlib_u8 *)sp2;
	dp = (mlib_s16 *)dp2;
	if (i < dsize) {
		(*dp++) = (*sp++);
	}
}

/* *********************************************************** */

/*
 * either source or destination image data are not 1-d vectors.
 * source and destination image data are 16-byte aligned.
 * slb and dlb are multiple of 16.
 */

void
mlib_s_ImageDataTypeConvert_U8_S16_A8D2X8(
    const mlib_u8 *src,
    mlib_s32 slb,
    mlib_s16 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize)
{
	mlib_u8 *sp, *sl;
	mlib_s16 *dp, *dl;
	mlib_s32 j;

	sp = sl = (mlib_u8 *)src;
	dp = dl = (mlib_s16 *)dst;

	for (j = 0; j < ysize; j++) {

		mlib_s_ImageDataTypeConvert_U8_S16_A8D1X8(sp, dp, xsize);

		sp = sl = sl + slb;
		dp = dl = (mlib_s16 *)((mlib_u8 *)dl + dlb);
	}
}

/* *********************************************************** */

/* 1-D non-aligned line */

void
mlib_s_ImageDataTypeConvert_U8_S16_D1(
    const mlib_u8 *src,
    mlib_s16 *dst,
    mlib_s32 dsize)
{
/* pointer for pixel in source */
	mlib_u8 *sp;

/* pointer for pixel in destination */
	mlib_s16 *dp;

/* pointer for pixel in source */
	mlib_u16 *sp2;

/* pointer for pixel in destination */
	mlib_s32 *dp2;

	__m128i x_sd;
	__m128i x_dd0, x_dd1;
	mlib_u32 val0;
	mlib_s32 i;

	const __m128i x_zero = _mm_setzero_si128();

	sp = (mlib_u8 *)src;
	dp = dst;

/* 16 pixels */
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < (dsize - 15); i += 16) {
		x_sd = _mm_loadu_si128((__m128i *)sp);
		x_dd0 = _mm_unpacklo_epi8(x_sd, x_zero);
		x_dd1 = _mm_unpackhi_epi8(x_sd, x_zero);
		_mm_storeu_si128((__m128i *)dp, x_dd0);
		dp += 8;
		_mm_storeu_si128((__m128i *)dp, x_dd1);
		dp += 8;
		sp += 16;
	}

/* 8 pixels */
	if (i < (dsize - 7)) {
		x_sd = _mm_loadl_epi64((__m128i *)sp);
		x_dd0 = _mm_unpacklo_epi8(x_sd, x_zero);
		_mm_storeu_si128((__m128i *)dp, x_dd0);
		dp += 8;
		sp += 8;
		i += 8;
	}

/* 2 pixels */
	sp2 = (mlib_u16 *)sp;
	dp2 = (mlib_s32 *)dp;
	for (; i < (dsize - 1); i += 2) {
		val0 = (*sp2++);
		dp2[0] = ((val0 & 0xFF00) << 8) | (val0 & 0xFF);
		dp2++;
	}

/* 1 pixels */
	sp = (mlib_u8 *)sp2;
	dp = (mlib_s16 *)dp2;
	if (i < dsize) {
		(*dp++) = (*sp++);
	}
}

/* *********************************************************** */

/* 2-D non-aligned image */

void
mlib_s_ImageDataTypeConvert_U8_S16(
    const mlib_u8 *src,
    mlib_s32 slb,
    mlib_s16 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize)
{
	mlib_u8 *sp, *sl;
	mlib_s16 *dp, *dl;
	mlib_s32 j;

	sp = sl = (void *)src;
	dp = dl = dst;

	for (j = 0; j < ysize; j++) {
		mlib_s_ImageDataTypeConvert_U8_S16_D1(sp, dp, xsize);
		sp = sl = sl + slb;
		dp = dl = (mlib_s16 *)((mlib_u8 *)dl + dlb);
	}
}

/* *********************************************************** */

/*
 * Convert a 1-bit image into an 8-bit image.
 */

void
mlib_s_ImageDataTypeConvert_BIT_U8(
    const mlib_image *src,
    mlib_image *dst)
{
/* pointer for pixel in source */
	mlib_u8 *sp;

/* pointer for pixel in destination */
	mlib_u8 *dp;

/* pointer for line in source */
	mlib_u8 *sl;

/* pointer for line in destination */
	mlib_u8 *dl;
	mlib_s32 width, height, channels;

/* src line strides in pixels */
	mlib_s32 strides;

/* dst line strides in bytes */
	mlib_s32 strided;
	mlib_s32 dsize;

/* intermediate value */
	mlib_u8 bitdata;

/* intermediate value */
	mlib_s32 bitdata32;

/* indices for x, y and bit */
	mlib_s32 i, j, k;

	width = mlib_ImageGetWidth(src);
	height = mlib_ImageGetHeight(src);
	channels = mlib_ImageGetChannels(src);

	sp = (mlib_u8 *)mlib_ImageGetData(src);
	dp = (mlib_u8 *)mlib_ImageGetData(dst);

/* in bytes */
	strides = mlib_ImageGetStride(src);
/* in bytes */
	strided = mlib_ImageGetStride(dst);

	dsize = width * channels;

	sl = sp;
	dl = dp;
	for (j = 0; j < height; j++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < dsize / 8; i++) {
			bitdata = (*sp++);
/* get bits 7 */
			dp[0] = (mlib_u8)((bitdata >> 7));
/* get bits 6 */
			dp[1] = (mlib_u8)((bitdata >> 6) & 1);
/* get bits 5 */
			dp[2] = (mlib_u8)((bitdata >> 5) & 1);
/* get bits 4 */
			dp[3] = (mlib_u8)((bitdata >> 4) & 1);
/* get bits 3 */
			dp[4] = (mlib_u8)((bitdata >> 3) & 1);
/* get bits 2 */
			dp[5] = (mlib_u8)((bitdata >> 2) & 1);
/* get bits 1 */
			dp[6] = (mlib_u8)((bitdata >> 1) & 1);
/* get bits 0 */
			dp[7] = (mlib_u8)((bitdata) & 1);
			dp += 8;
		}

		if (dsize & 7) {
			bitdata32 = (mlib_s32)((*sp++));
			for (k = 0; k < (dsize % 8); k++) {
				bitdata32 <<= 1;
				(*dp++) = (mlib_u8)(bitdata32 >> 8);
				bitdata32 &= 0xff;
			}
		}

		sp = sl += strides;
		dp = dl += strided;
	}
}

/* *********************************************************** */

/*
 * Convert a 1-bit image into a 16-bit image.
 */

void
mlib_s_ImageDataTypeConvert_BIT_S16(
    const mlib_image *src,
    mlib_image *dst)
{
/* pointer for pixel in source */
	mlib_u8 *sp;

/* pointer for pixel in destination */
	mlib_s16 *dp;

/* pointer for line in source */
	mlib_u8 *sl;

/* pointer for line in destination */
	mlib_s16 *dl;
	mlib_s32 width, height, channels;

/* src line strides in pixels */
	mlib_s32 strides;

/* dst line strides in bytes */
	mlib_s32 strided;
	mlib_s32 dsize;

/* intermediate value */
	mlib_u8 bitdata;

/* intermediate value */
	mlib_s32 bitdata32;

/* indices for x, y and bit */
	mlib_s32 i, j, k;

	width = mlib_ImageGetWidth(src);
	height = mlib_ImageGetHeight(src);
	channels = mlib_ImageGetChannels(src);

	sp = (mlib_u8 *)mlib_ImageGetData(src);
	dp = (mlib_s16 *)mlib_ImageGetData(dst);

/* in bytes */
	strides = mlib_ImageGetStride(src);
/* in bytes */
	strided = mlib_ImageGetStride(dst) / 2;

	dsize = width * channels;

	sl = sp;
	dl = dp;
	for (j = 0; j < height; j++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < dsize / 8; i++) {
			bitdata = (*sp++);
/* get bits 7 */
			dp[0] = (mlib_s16)((bitdata >> 7));
/* get bits 6 */
			dp[1] = (mlib_s16)((bitdata >> 6) & 1);
/* get bits 5 */
			dp[2] = (mlib_s16)((bitdata >> 5) & 1);
/* get bits 4 */
			dp[3] = (mlib_s16)((bitdata >> 4) & 1);
/* get bits 3 */
			dp[4] = (mlib_s16)((bitdata >> 3) & 1);
/* get bits 2 */
			dp[5] = (mlib_s16)((bitdata >> 2) & 1);
/* get bits 1 */
			dp[6] = (mlib_s16)((bitdata >> 1) & 1);
/* get bits 0 */
			dp[7] = (mlib_s16)((bitdata) & 1);
			dp += 8;
		}

		if (dsize & 7) {
			bitdata32 = (mlib_s32)((*sp++));
			for (k = 0; k < (dsize % 8); k++) {
				bitdata32 <<= 1;
				(*dp++) = (mlib_s16)(bitdata32 >> 8);
				bitdata32 &= 0xff;
			}
		}

		sp = sl += strides;
		dp = dl += strided;
	}
}

/* *********************************************************** */

/*
 * Convert a 1-bit image into a 32-bit image.
 */

void
mlib_s_ImageDataTypeConvert_BIT_S32(
    const mlib_image *src,
    mlib_image *dst)
{
/* pointer for pixel in source */
	mlib_u8 *sp;

/* pointer for pixel in destination */
	mlib_s32 *dp;

/* pointer for line in source */
	mlib_u8 *sl;

/* pointer for line in destination */
	mlib_s32 *dl;
	mlib_s32 width, height, channels;

/* src line strides in pixels */
	mlib_s32 strides;

/* dst line strides in bytes */
	mlib_s32 strided;
	mlib_s32 dsize;

/* intermediate value */
	mlib_u8 bitdata;

/* intermediate value */
	mlib_s32 bitdata32;

/* indices for x, y and bit */
	mlib_s32 i, j, k;

	width = mlib_ImageGetWidth(src);
	height = mlib_ImageGetHeight(src);
	channels = mlib_ImageGetChannels(src);

	sp = (mlib_u8 *)mlib_ImageGetData(src);
	dp = (mlib_s32 *)mlib_ImageGetData(dst);

/* in bytes */
	strides = mlib_ImageGetStride(src);
/* in bytes */
	strided = mlib_ImageGetStride(dst) / 4;

	dsize = width * channels;

	sl = sp;
	dl = dp;
	for (j = 0; j < height; j++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < dsize / 8; i++) {
			bitdata = (*sp++);
/* get bits 7 */
			dp[0] = (mlib_s32)((bitdata >> 7));
/* get bits 6 */
			dp[1] = (mlib_s32)((bitdata >> 6) & 1);
/* get bits 5 */
			dp[2] = (mlib_s32)((bitdata >> 5) & 1);
/* get bits 4 */
			dp[3] = (mlib_s32)((bitdata >> 4) & 1);
/* get bits 3 */
			dp[4] = (mlib_s32)((bitdata >> 3) & 1);
/* get bits 2 */
			dp[5] = (mlib_s32)((bitdata >> 2) & 1);
/* get bits 1 */
			dp[6] = (mlib_s32)((bitdata >> 1) & 1);
/* get bits 0 */
			dp[7] = (mlib_s32)((bitdata) & 1);
			dp += 8;
		}

		if (dsize & 7) {
			bitdata32 = (mlib_s32)((*sp++));
			for (k = 0; k < (dsize % 8); k++) {
				bitdata32 <<= 1;
				(*dp++) = (mlib_s32)(bitdata32 >> 8);
				bitdata32 &= 0xff;
			}
		}

		sp = sl += strides;
		dp = dl += strided;
	}
}

/* *********************************************************** */

void
mlib_s_ImageDataTypeConvert_BIT_F32_A8D1X8(
    const mlib_u8 *src,
    mlib_f32 *dst,
    mlib_s32 dsize)
{
/* pointer for pixel in source */
	mlib_u8 *sp;

/* pointer for pixel in destination */
	mlib_d64 *dp;

/* intermediate value */
	mlib_u8 bitdata;

/* indices for x, y and bit */
	mlib_s32 i;
	mlib_d64 *pt;

	sp = (void *)src;
	dp = (mlib_d64 *)dst;
	pt = (mlib_d64 *)mlib_table_F32;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < dsize / 8; i++) {
		bitdata = sp[i];
		dp[0] = pt[(bitdata >> 6)];
		dp[1] = pt[(bitdata >> 4) & 3];
		dp[2] = pt[(bitdata >> 2) & 3];
		dp[3] = pt[(bitdata & 3)];
		dp += 4;
	}
}

/* *********************************************************** */

void
mlib_s_ImageDataTypeConvert_BIT_F32_A8D2X8(
    const mlib_u8 *src,
    mlib_s32 slb,
    mlib_f32 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize)
{
/* pointer for src */
	mlib_u8 *sp, *sl;

/* 8-byte aligned pointer for dst */
	mlib_d64 *dp, *dl;

/* indices for x, y */
	mlib_s32 i, j;

/* intermediate value */
	mlib_u8 bitdata;
	mlib_d64 *pt;

	sp = sl = (void *)src;
	dp = dl = (mlib_d64 *)dst;
	pt = (mlib_d64 *)mlib_table_F32;

	for (j = 0; j < ysize; j++) {

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < xsize / 8; i++) {
			bitdata = sp[i];
			dp[0] = pt[(bitdata >> 6)];
			dp[1] = pt[(bitdata >> 4) & 3];
			dp[2] = pt[(bitdata >> 2) & 3];
			dp[3] = pt[(bitdata & 3)];
			dp += 4;
		}

		sp = sl = sl + slb;
		dp = dl = (mlib_d64 *)((mlib_u8 *)dl + dlb);
	}
}

/* *********************************************************** */

void
mlib_s_ImageDataTypeConvert_BIT_F32_D1(
    const mlib_u8 *src,
    mlib_f32 *dst,
    mlib_s32 dsize)
{
/* pointer in source */
	mlib_u8 *sa;

/* pointer in destination */
	mlib_f32 *da, *dp32;

/* end point in destination */
	mlib_f32 *dend;

/* 8-byte aligned start point in destination */
	mlib_d64 *dp;
	mlib_u8 sd0, sd1;
	mlib_u8 dd1;

/* offset of address alignment in destination */
	mlib_s32 off, n;
	mlib_s32 i, k, num;
	mlib_d64 *pt;

/* intermediate value */
	mlib_s32 bitdata32;

	sa = (void *)src;
	da = dst;
	pt = (mlib_d64 *)mlib_table_F32;

	dp = (mlib_d64 *)((mlib_addr)da & (~7));
	off = ((mlib_u8 *)da - (mlib_u8 *)dp) >> 2;
	dend = da + dsize - 1;
	sd1 = (*sa++);

/* left end handling */

	if ((mlib_f32 *)dp != da) {
		*da = (mlib_f32)((sd1 >> 7));
		dp++;
	} else {
		if (da == dend) {
			*da = (mlib_f32)((sd1 >> 7));
			dp++;
		} else {
			(*dp++) = pt[(sd1 >> 6)];
		}
	}

	n = (((mlib_u8 *)dend - (mlib_u8 *)dp) + 4) / 32;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < n; i++) {
		sd0 = sd1;
		sd1 = (*sa++);
		dd1 = ((sd0 << (2 - off)) | (sd1 >> (6 + off)));
		(*dp++) = pt[(dd1 >> 6)];
		(*dp++) = pt[(dd1 >> 4) & 3];
		(*dp++) = pt[(dd1 >> 2) & 3];
		(*dp++) = pt[(dd1 & 3)];
	}

/* right end handling */

	if ((mlib_addr)dp <= (mlib_addr)dend) {
		dp32 = (mlib_f32 *)dp;
		num = (dend + 1) - dp32;
		sd0 = sd1;
		if (num > (6 + off))
			sd1 = *sa;
		dd1 = ((sd0 << (2 - off)) | (sd1 >> (6 + off)));
		bitdata32 = (mlib_s32)dd1;
		for (k = 0; k < num; k++) {
			bitdata32 <<= 1;
			(*dp32++) = (mlib_f32)(bitdata32 >> 8);
			bitdata32 &= 0xff;
		}
	}
}

/* *********************************************************** */

/* 2-D non-aligned image */

void
mlib_s_ImageDataTypeConvert_BIT_F32(
    const mlib_u8 *src,
    mlib_s32 slb,
    mlib_f32 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize)
{
	mlib_u8 *sa, *sl;
	mlib_f32 *da, *dl;
	mlib_s32 j;

	sa = sl = (void *)src;
	da = dl = dst;
/* in pixel */
	dlb >>= 2;
	for (j = 0; j < ysize; j++) {
		mlib_s_ImageDataTypeConvert_BIT_F32_D1(sa, da, xsize);
		sa = sl = sl + slb;
		da = dl = dl + dlb;
	}
}

/* *********************************************************** */

/*
 * Convert a 1-bit image into a d64-bit image.
 */

void
mlib_s_ImageDataTypeConvert_BIT_D64(
    const mlib_image *src,
    mlib_image *dst)
{
	mlib_s32 buf[SIZE + 8], *sbuf;

/* pointer for pixel in source */
	mlib_u8 *sp;

/* pointer for pixel in destination */
	mlib_d64 *dp;

/* pointer for line in source */
	mlib_u8 *sl;

/* pointer for line in destination */
	mlib_d64 *dl;
	mlib_s32 width, height, channels;

/* src line strides in pixels */
	mlib_s32 strides;

/* dst line strides in bytes */
	mlib_s32 strided;
	mlib_s32 dsize;

/* intermediate value */
	mlib_u8 bitdata;

/* indices for x, y and bit */
	mlib_s32 i, j, num, size;

	width = mlib_ImageGetWidth(src);
	height = mlib_ImageGetHeight(src);
	channels = mlib_ImageGetChannels(src);

	sl = (mlib_u8 *)mlib_ImageGetData(src);
	dl = (mlib_d64 *)mlib_ImageGetData(dst);

/* in bytes */
	strides = mlib_ImageGetStride(src);
/* in bytes */
	strided = mlib_ImageGetStride(dst) >> 3;

	dsize = width * channels;

	for (j = 0; j < height; j++) {

		for (size = 0; size < dsize; size += num) {

			sp = sl + size / 8;
			dp = dl + size;

			num = dsize - size;

			if (num > SIZE)
				num = SIZE;

			sbuf = (mlib_s32 *)buf;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < (num - 1) / 8 + 1; i++) {
				bitdata = (*sp++);
				sbuf[0] = ((bitdata >> 7));
				sbuf[1] = ((bitdata >> 6) & 1);
				sbuf[2] = ((bitdata >> 5) & 1);
				sbuf[3] = ((bitdata >> 4) & 1);
				sbuf[4] = ((bitdata >> 3) & 1);
				sbuf[5] = ((bitdata >> 2) & 1);
				sbuf[6] = ((bitdata >> 1) & 1);
				sbuf[7] = ((bitdata) & 1);
				sbuf += 8;
			}

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < num; i++) {
				dp[i] = (mlib_d64)buf[i];
			}
		}

		sl += strides;
		dl += strided;
	}
}

/* *********************************************************** */

/* U8 -> BIT */
void mlib_s_ImageDataTypeConvert_U8_BIT_A8D1X8(
    const mlib_u8 *src,
    mlib_u8 *dst,
    mlib_s32 dsize)
{
/* pointer for pixel in source */
	mlib_u8 *sp;

/* pointer for pixel in destination */
	mlib_u8 *dp, *dl;

/* pointer for pixel in source */
	mlib_u16 *sp2;

/* pointer for pixel in destination */
	mlib_s32 *dp2;

/* intermediate value */
	mlib_s32 bitdata;

	__m128i x_sd;
	__m128i x_dd0, x_dd1, x_dd2, x_dd3, x_dd4, x_dd5;
	__m128i x_dd6, x_dd7, x_dd8, x_dd9, x_dd10;
	mlib_s32 dd, s0;
	mlib_s32 i;

	const __m128i x_zero = _mm_setzero_si128();

	sp = (mlib_u8 *)src;
	dp = dl = dst;

/* 16 pixels */
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < (dsize - 15); i += 16) {
		x_sd = _mm_load_si128((__m128i *)sp);
		x_dd0 = _mm_cmpeq_epi8(x_sd, x_zero);
		x_dd1 = _mm_cmpeq_epi8(x_dd0, x_zero);

		x_dd2 = _mm_unpacklo_epi8(x_dd1, x_zero);
		x_dd3 = _mm_unpackhi_epi8(x_dd1, x_zero);

		x_dd4 = _mm_shuffle_epi32(x_dd2, 0x1B);
		x_dd5 = _mm_shufflehi_epi16(x_dd4, 0xB1);
		x_dd6 = _mm_shufflelo_epi16(x_dd5, 0xB1);

		x_dd7 = _mm_shuffle_epi32(x_dd3, 0x1B);
		x_dd8 = _mm_shufflehi_epi16(x_dd7, 0xB1);
		x_dd9 = _mm_shufflelo_epi16(x_dd8, 0xB1);

		x_dd10 = _mm_packus_epi16(x_dd6, x_dd9);

		dd = _mm_movemask_epi8(x_dd10);
		*((mlib_u16 *)dp) = (mlib_u16)dd;
		dp += 2;
		sp += 16;
	}

/* 8 pixels */
	if (i < (dsize - 7)) {
		const __m128i x_mask = _mm_set1_epi8(0xFF);
		x_sd = _mm_loadl_epi64((__m128i *)sp);
		x_dd0 = _mm_cmpeq_epi8(x_sd, x_zero);
		x_dd1 = _mm_cmpeq_epi8(x_dd0, x_zero);

		x_dd2 = _mm_unpacklo_epi8(x_dd1, x_zero);

		x_dd3 = _mm_shuffle_epi32(x_dd2, 0x1B);
		x_dd4 = _mm_shufflehi_epi16(x_dd3, 0xB1);
		x_dd5 = _mm_shufflelo_epi16(x_dd4, 0xB1);
		x_dd6 = _mm_packus_epi16(x_dd5, x_dd5);

		dd = _mm_movemask_epi8(x_dd6);
		*(dp++) = (mlib_u8)dd;
		sp += 8;
		i += 8;
	}

/* 1 pixels */
	if (dsize & 7) {
		mlib_s32 mask = 0xFF00 >> (dsize & 7);
		mlib_u32 and = 0x80000000;
		bitdata = 0;

		for (; i < dsize; i++) {
			s0 = -sp[0];
			bitdata |= (s0 & and);
			and >>= 1;
			sp++;
		}
		dl[dsize / 8] = (bitdata >> 24) |
		    (dl[dsize / 8] & ~mask);
	}
}

/* *********************************************************** */

void mlib_s_ImageDataTypeConvert_U8_BIT_A8D2X8(
    const mlib_u8 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize)
{
	mlib_u8 *sp, *sl;
	mlib_u8 *dp, *dl;
	mlib_s32 j;

	sp = sl = (mlib_u8 *)src;
	dp = dl = dst;

	for (j = 0; j < ysize; j++) {

		mlib_s_ImageDataTypeConvert_U8_BIT_A8D1X8(sp, dp, xsize);

		sp = sl = sl + slb;
		dp = dl = dl + dlb;
	}
}

/* *********************************************************** */

void mlib_s_ImageDataTypeConvert_U8_BIT_D1(
    const mlib_u8 *src,
    mlib_u8 *dst,
    mlib_s32 dsize)
{
/* pointer for pixel in source */
	mlib_u8 *sp;

/* pointer for pixel in destination */
	mlib_u8 *dp, *dl;

/* pointer for pixel in source */
	mlib_u16 *sp2;

/* pointer for pixel in destination */
	mlib_s32 *dp2;

/* intermediate value */
	mlib_s32 bitdata;

	__m128i x_sd;
	__m128i x_dd0, x_dd1, x_dd2, x_dd3, x_dd4, x_dd5;
	__m128i x_dd6, x_dd7, x_dd8, x_dd9, x_dd10;
	mlib_s32 dd, s0;
	mlib_s32 i;

	const __m128i x_zero = _mm_setzero_si128();

	sp = (mlib_u8 *)src;
	dp = dl = dst;

/* 16 pixels */
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < (dsize - 15); i += 16) {
		x_sd = _mm_loadu_si128((__m128i *)sp);
		x_dd0 = _mm_cmpeq_epi8(x_sd, x_zero);
		x_dd1 = _mm_cmpeq_epi8(x_dd0, x_zero);

		x_dd2 = _mm_unpacklo_epi8(x_dd1, x_zero);
		x_dd3 = _mm_unpackhi_epi8(x_dd1, x_zero);

		x_dd4 = _mm_shuffle_epi32(x_dd2, 0x1B);
		x_dd5 = _mm_shufflehi_epi16(x_dd4, 0xB1);
		x_dd6 = _mm_shufflelo_epi16(x_dd5, 0xB1);

		x_dd7 = _mm_shuffle_epi32(x_dd3, 0x1B);
		x_dd8 = _mm_shufflehi_epi16(x_dd7, 0xB1);
		x_dd9 = _mm_shufflelo_epi16(x_dd8, 0xB1);

		x_dd10 = _mm_packus_epi16(x_dd6, x_dd9);

		dd = _mm_movemask_epi8(x_dd10);
		*((mlib_u16 *)dp) = (mlib_u16)dd;
		dp += 2;
		sp += 16;
	}

/* 8 pixels */
	if (i < (dsize - 7)) {
		const __m128i x_mask = _mm_set1_epi8(0xFF);
		x_sd = _mm_loadl_epi64((__m128i *)sp);
		x_dd0 = _mm_cmpeq_epi8(x_sd, x_zero);
		x_dd1 = _mm_cmpeq_epi8(x_dd0, x_zero);

		x_dd2 = _mm_unpacklo_epi8(x_dd1, x_zero);

		x_dd3 = _mm_shuffle_epi32(x_dd2, 0x1B);
		x_dd4 = _mm_shufflehi_epi16(x_dd3, 0xB1);
		x_dd5 = _mm_shufflelo_epi16(x_dd4, 0xB1);
		x_dd6 = _mm_packus_epi16(x_dd5, x_dd5);

		dd = _mm_movemask_epi8(x_dd6);
		*(dp++) = (mlib_u8)dd;
		sp += 8;
		i += 8;
	}

/* 1 pixels */
	if (dsize & 7) {
		mlib_s32 mask = 0xFF00 >> (dsize & 7);
		mlib_u32 and = 0x80000000;
		bitdata = 0;

		for (; i < dsize; i++) {
			s0 = -sp[0];
			bitdata |= (s0 & and);
			and >>= 1;
			sp++;
		}
		dl[dsize / 8] = (bitdata >> 24) |
		    (dl[dsize / 8] & ~mask);
	}
}

/* *********************************************************** */

void mlib_s_ImageDataTypeConvert_U8_BIT(
    const mlib_u8 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize)
{
	mlib_u8 *sp, *sl;
	mlib_u8 *dp, *dl;
	mlib_s32 j;

	sp = sl = (mlib_u8 *)src;
	dp = dl = dst;

	for (j = 0; j < ysize; j++) {

		mlib_s_ImageDataTypeConvert_U8_BIT_D1(sp, dp, xsize);

		sp = sl = sl + slb;
		dp = dl = dl + dlb;
	}
}

/* *********************************************************** */

/* U8 -> S32 */
void mlib_s_ImageDataTypeConvert_U8_S32_A8D1X8(
    const mlib_u8 *src,
    mlib_s32 *dst,
    mlib_s32 dsize)
{
/* pointer for pixel in source */
	mlib_u8 *sp;

/* pointer for pixel in destination */
	mlib_s32 *dp;

/* pointer for pixel in source */
	mlib_u16 *sp2;

/* pointer for pixel in destination */
	mlib_s64 *dp2;

	__m128i x_sd;
	__m128i x_dd0, x_dd1, x_dd2, x_dd3, x_dd4, x_dd5;
	mlib_u64 val0;
	mlib_s32 i;

	const __m128i x_zero = _mm_setzero_si128();

	sp = (mlib_u8 *)src;
	dp = dst;

/* 16 pixels */
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < (dsize - 15); i += 16) {
		x_sd = _mm_load_si128((__m128i *)sp);
		x_dd0 = _mm_unpacklo_epi8(x_sd, x_zero);
		x_dd1 = _mm_unpackhi_epi8(x_sd, x_zero);
		x_dd2 = _mm_unpacklo_epi16(x_dd0, x_zero);
		x_dd3 = _mm_unpackhi_epi16(x_dd0, x_zero);
		x_dd4 = _mm_unpacklo_epi16(x_dd1, x_zero);
		x_dd5 = _mm_unpackhi_epi16(x_dd1, x_zero);
		_mm_store_si128((__m128i *)dp, x_dd2);
		dp += 4;
		_mm_store_si128((__m128i *)dp, x_dd3);
		dp += 4;
		_mm_store_si128((__m128i *)dp, x_dd4);
		dp += 4;
		_mm_store_si128((__m128i *)dp, x_dd5);
		dp += 4;
		sp += 16;
	}

/* 8 pixels */
	if (i < (dsize - 7)) {
		x_sd = _mm_loadl_epi64((__m128i *)sp);
		x_dd0 = _mm_unpacklo_epi8(x_sd, x_zero);
		x_dd1 = _mm_unpacklo_epi16(x_dd0, x_zero);
		x_dd2 = _mm_unpackhi_epi16(x_dd0, x_zero);
		_mm_store_si128((__m128i *)dp, x_dd1);
		dp += 4;
		_mm_store_si128((__m128i *)dp, x_dd2);
		dp += 4;
		sp += 8;
		i += 8;
	}

/* 2 pixels */
	sp2 = (mlib_u16 *)sp;
	dp2 = (mlib_s64 *)dp;
	for (; i < (dsize - 1); i += 2) {
		val0 = (*sp2++);
		dp2[0] = ((val0 & 0xFF00) << 24) | (val0 & 0xFF);
		dp2++;
	}

/* 1 pixels */
	sp = (mlib_u8 *)sp2;
	dp = (mlib_s32 *)dp2;
	if (i < dsize) {
		(*dp++) = (*sp++);
	}
}

/* *********************************************************** */

void mlib_s_ImageDataTypeConvert_U8_S32_A8D2X8(
    const mlib_u8 *src,
    mlib_s32 slb,
    mlib_s32 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize)
{
	mlib_u8 *sp, *sl;
	mlib_s32 *dp, *dl;
	mlib_s32 j;

	sp = sl = (mlib_u8 *)src;
	dp = dl = dst;

	for (j = 0; j < ysize; j++) {

		mlib_s_ImageDataTypeConvert_U8_S32_A8D1X8(sp, dp, xsize);

		sp = sl = sl + slb;
		dp = dl = (mlib_s32 *)((mlib_u8 *)dl + dlb);
	}
}

/* *********************************************************** */

void mlib_s_ImageDataTypeConvert_U8_S32_D1(
    const mlib_u8 *src,
    mlib_s32 *dst,
    mlib_s32 dsize)
{
/* pointer for pixel in source */
	mlib_u8 *sp;

/* pointer for pixel in destination */
	mlib_s32 *dp;

/* pointer for pixel in source */
	mlib_u16 *sp2;

/* pointer for pixel in destination */
	mlib_s64 *dp2;

	__m128i x_sd;
	__m128i x_dd0, x_dd1, x_dd2, x_dd3, x_dd4, x_dd5;
	mlib_u64 val0;
	mlib_s32 i;

	const __m128i x_zero = _mm_setzero_si128();

	sp = (mlib_u8 *)src;
	dp = dst;

/* 16 pixels */
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < (dsize - 15); i += 16) {
		x_sd = _mm_loadu_si128((__m128i *)sp);
		x_dd0 = _mm_unpacklo_epi8(x_sd, x_zero);
		x_dd1 = _mm_unpackhi_epi8(x_sd, x_zero);
		x_dd2 = _mm_unpacklo_epi16(x_dd0, x_zero);
		x_dd3 = _mm_unpackhi_epi16(x_dd0, x_zero);
		x_dd4 = _mm_unpacklo_epi16(x_dd1, x_zero);
		x_dd5 = _mm_unpackhi_epi16(x_dd1, x_zero);
		_mm_storeu_si128((__m128i *)dp, x_dd2);
		dp += 4;
		_mm_storeu_si128((__m128i *)dp, x_dd3);
		dp += 4;
		_mm_storeu_si128((__m128i *)dp, x_dd4);
		dp += 4;
		_mm_storeu_si128((__m128i *)dp, x_dd5);
		dp += 4;
		sp += 16;
	}

/* 8 pixels */
	if (i < (dsize - 7)) {
		x_sd = _mm_loadl_epi64((__m128i *)sp);
		x_dd0 = _mm_unpacklo_epi8(x_sd, x_zero);
		x_dd1 = _mm_unpacklo_epi16(x_dd0, x_zero);
		x_dd2 = _mm_unpackhi_epi16(x_dd0, x_zero);
		_mm_storeu_si128((__m128i *)dp, x_dd1);
		dp += 4;
		_mm_storeu_si128((__m128i *)dp, x_dd2);
		dp += 4;
		sp += 8;
		i += 8;
	}

/* 2 pixels */
	sp2 = (mlib_u16 *)sp;
	dp2 = (mlib_s64 *)dp;
	for (; i < (dsize - 1); i += 2) {
		val0 = (*sp2++);
		dp2[0] = ((val0 & 0xFF00) << 24) | (val0 & 0xFF);
		dp2++;
	}

/* 1 pixels */
	sp = (mlib_u8 *)sp2;
	dp = (mlib_s32 *)dp2;
	if (i < dsize) {
		(*dp++) = (*sp++);
	}


}

/* *********************************************************** */

void mlib_s_ImageDataTypeConvert_U8_S32(
    const mlib_u8 *src,
    mlib_s32 slb,
    mlib_s32 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize)
{
	mlib_u8 *sp, *sl;
	mlib_s32 *dp, *dl;
	mlib_s32 j;

	sp = sl = (mlib_u8 *)src;
	dp = dl = dst;

	for (j = 0; j < ysize; j++) {

		mlib_s_ImageDataTypeConvert_U8_S32_D1(sp, dp, xsize);

		sp = sl = sl + slb;
		dp = dl = (mlib_s32 *)((mlib_u8 *)dl + dlb);
	}
}

/* *********************************************************** */

/* U8 -> F32 */
void mlib_s_ImageDataTypeConvert_U8_F32_A8D1X8(
    const mlib_u8 *src,
    mlib_f32 *dst,
    mlib_s32 dsize)
{
/* pointer for pixel in source */
	mlib_u8 *sp;

/* pointer for pixel in destination */
	mlib_f32 *dp;

	__m128i x_sd;
	__m128i x_dd0, x_dd1, x_dd2, x_dd3, x_dd4, x_dd5;
	__m128 x_df0, x_df1, x_df2, x_df3;
	mlib_s32 i;

	const __m128i x_zero = _mm_setzero_si128();

	sp = (mlib_u8 *)src;
	dp = dst;

/* 16 pixels */
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < (dsize - 15); i += 16) {
		x_sd = _mm_load_si128((__m128i *)sp);
		x_dd0 = _mm_unpacklo_epi8(x_sd, x_zero);
		x_dd1 = _mm_unpackhi_epi8(x_sd, x_zero);
		x_dd2 = _mm_unpacklo_epi16(x_dd0, x_zero);
		x_dd3 = _mm_unpackhi_epi16(x_dd0, x_zero);
		x_dd4 = _mm_unpacklo_epi16(x_dd1, x_zero);
		x_dd5 = _mm_unpackhi_epi16(x_dd1, x_zero);

		x_df0 = _mm_cvtepi32_ps(x_dd2);
		x_df1 = _mm_cvtepi32_ps(x_dd3);
		x_df2 = _mm_cvtepi32_ps(x_dd4);
		x_df3 = _mm_cvtepi32_ps(x_dd5);

		_mm_store_ps(dp, x_df0);
		dp += 4;
		_mm_store_ps(dp, x_df1);
		dp += 4;
		_mm_store_ps(dp, x_df2);
		dp += 4;
		_mm_store_ps(dp, x_df3);
		dp += 4;
		sp += 16;
	}

/* 8 pixels */
	if (i < (dsize - 7)) {
		x_sd = _mm_loadl_epi64((__m128i *)sp);
		x_dd0 = _mm_unpacklo_epi8(x_sd, x_zero);
		x_dd1 = _mm_unpacklo_epi16(x_dd0, x_zero);
		x_dd2 = _mm_unpackhi_epi16(x_dd0, x_zero);

		x_df0 = _mm_cvtepi32_ps(x_dd1);
		x_df1 = _mm_cvtepi32_ps(x_dd2);

		_mm_store_ps(dp, x_df0);
		dp += 4;
		_mm_store_ps(dp, x_df1);
		dp += 4;
		sp += 8;
		i += 8;
	}

/* 1 pixels loop */
	for (; i < (dsize); i++) {
		(*dp++) = (*sp++);
	}
}

/* *********************************************************** */

void mlib_s_ImageDataTypeConvert_U8_F32_A8D2X8(
    const mlib_u8 *src,
    mlib_s32 slb,
    mlib_f32 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize)
{
	mlib_u8 *sp, *sl;
	mlib_f32 *dp, *dl;
	mlib_s32 j;

	sp = sl = (mlib_u8 *)src;
	dp = dl = dst;

	for (j = 0; j < ysize; j++) {

		mlib_s_ImageDataTypeConvert_U8_F32_A8D1X8(sp, dp, xsize);

		sp = sl = sl + slb;
		dp = dl = (mlib_f32 *)((mlib_u8 *)dl + dlb);
	}
}

/* *********************************************************** */

void mlib_s_ImageDataTypeConvert_U8_F32_D1(
    const mlib_u8 *src,
    mlib_f32 *dst,
    mlib_s32 dsize)
{
/* pointer for pixel in source */
	mlib_u8 *sp;

/* pointer for pixel in destination */
	mlib_f32 *dp;

	__m128i x_sd;
	__m128i x_dd0, x_dd1, x_dd2, x_dd3, x_dd4, x_dd5;
	__m128 x_df0, x_df1, x_df2, x_df3;
	mlib_s32 i;

	const __m128i x_zero = _mm_setzero_si128();

	sp = (mlib_u8 *)src;
	dp = dst;

/* 16 pixels */
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < (dsize - 15); i += 16) {
		x_sd = _mm_loadu_si128((__m128i *)sp);
		x_dd0 = _mm_unpacklo_epi8(x_sd, x_zero);
		x_dd1 = _mm_unpackhi_epi8(x_sd, x_zero);
		x_dd2 = _mm_unpacklo_epi16(x_dd0, x_zero);
		x_dd3 = _mm_unpackhi_epi16(x_dd0, x_zero);
		x_dd4 = _mm_unpacklo_epi16(x_dd1, x_zero);
		x_dd5 = _mm_unpackhi_epi16(x_dd1, x_zero);

		x_df0 = _mm_cvtepi32_ps(x_dd2);
		x_df1 = _mm_cvtepi32_ps(x_dd3);
		x_df2 = _mm_cvtepi32_ps(x_dd4);
		x_df3 = _mm_cvtepi32_ps(x_dd5);

		_mm_storeu_ps(dp, x_df0);
		dp += 4;
		_mm_storeu_ps(dp, x_df1);
		dp += 4;
		_mm_storeu_ps(dp, x_df2);
		dp += 4;
		_mm_storeu_ps(dp, x_df3);
		dp += 4;
		sp += 16;
	}

/* 8 pixels */
	if (i < (dsize - 7)) {
		x_sd = _mm_loadl_epi64((__m128i *)sp);
		x_dd0 = _mm_unpacklo_epi8(x_sd, x_zero);
		x_dd1 = _mm_unpacklo_epi16(x_dd0, x_zero);
		x_dd2 = _mm_unpackhi_epi16(x_dd0, x_zero);

		x_df0 = _mm_cvtepi32_ps(x_dd1);
		x_df1 = _mm_cvtepi32_ps(x_dd2);

		_mm_storeu_ps(dp, x_df0);
		dp += 4;
		_mm_storeu_ps(dp, x_df1);
		dp += 4;
		sp += 8;
		i += 8;
	}

/* 1 pixels loop */
	for (; i < (dsize); i++) {
		(*dp++) = (*sp++);
	}
}

/* *********************************************************** */

void mlib_s_ImageDataTypeConvert_U8_F32(
    const mlib_u8 *src,
    mlib_s32 slb,
    mlib_f32 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize)
{
	mlib_u8 *sp, *sl;
	mlib_f32 *dp, *dl;
	mlib_s32 j;

	sp = sl = (mlib_u8 *)src;
	dp = dl = dst;

	for (j = 0; j < ysize; j++) {

		mlib_s_ImageDataTypeConvert_U8_F32_D1(sp, dp, xsize);

		sp = sl = sl + slb;
		dp = dl = (mlib_f32 *)((mlib_u8 *)dl + dlb);
	}
}

/* *********************************************************** */

/* U8 -> D64 */
void mlib_s_ImageDataTypeConvert_U8_D64_A8D1X8(
    const mlib_u8 *src,
    mlib_d64 *dst,
    mlib_s32 dsize)
{
/* pointer for pixel in source */
	mlib_u8 *sp;

/* pointer for pixel in destination */
	mlib_d64 *dp;

	__m128i x_sd0, x_sd1, x_sd2, x_sd3, x_sd4, x_sd5, x_sd6, x_sd7;
	__m128i x_sd8, x_sd9, x_sd10, x_sd11, x_sd12, x_sd13, x_sd14;
	__m128d x_dd0, x_dd1, x_dd2, x_dd3, x_dd4, x_dd5, x_dd6, x_dd7;
	mlib_s32 i;
	const __m128i x_zero = _mm_setzero_si128();

	sp = (void *)src;
	dp = dst;

/* 16 pixels */
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < (dsize - 15); i += 16) {
		x_sd0 = _mm_load_si128((__m128i *)sp);
		x_sd1 = _mm_unpacklo_epi8(x_sd0, x_zero);
		x_sd2 = _mm_unpackhi_epi8(x_sd0, x_zero);

		x_sd3 = _mm_unpacklo_epi16(x_sd1, x_zero);
		x_sd4 = _mm_unpackhi_epi16(x_sd1, x_zero);
		x_sd5 = _mm_unpacklo_epi16(x_sd2, x_zero);
		x_sd6 = _mm_unpackhi_epi16(x_sd2, x_zero);

		x_sd7 = _mm_unpacklo_epi64(x_sd3, x_zero);
		x_sd8 = _mm_unpackhi_epi64(x_sd3, x_zero);
		x_dd0 = _mm_cvtepi32_pd(x_sd7);
		x_dd1 = _mm_cvtepi32_pd(x_sd8);
		_mm_store_pd(dp, x_dd0);
		dp += 2;
		_mm_store_pd(dp, x_dd1);
		dp += 2;

		x_sd9 = _mm_unpacklo_epi64(x_sd4, x_zero);
		x_sd10 = _mm_unpackhi_epi64(x_sd4, x_zero);
		x_dd2 = _mm_cvtepi32_pd(x_sd9);
		x_dd3 = _mm_cvtepi32_pd(x_sd10);
		_mm_store_pd(dp, x_dd2);
		dp += 2;
		_mm_store_pd(dp, x_dd3);
		dp += 2;

		x_sd11 = _mm_unpacklo_epi64(x_sd5, x_zero);
		x_sd12 = _mm_unpackhi_epi64(x_sd5, x_zero);
		x_dd4 = _mm_cvtepi32_pd(x_sd11);
		x_dd5 = _mm_cvtepi32_pd(x_sd12);
		_mm_store_pd(dp, x_dd4);
		dp += 2;
		_mm_store_pd(dp, x_dd5);
		dp += 2;

		x_sd13 = _mm_unpacklo_epi64(x_sd6, x_zero);
		x_sd14 = _mm_unpackhi_epi64(x_sd6, x_zero);
		x_dd6 = _mm_cvtepi32_pd(x_sd13);
		x_dd7 = _mm_cvtepi32_pd(x_sd14);
		_mm_store_pd(dp, x_dd6);
		dp += 2;
		_mm_store_pd(dp, x_dd7);
		dp += 2;

		sp += 16;
	}

/* 8 pixels */
	if (i < (dsize - 7)) {
		x_sd0 = _mm_loadl_epi64((__m128i *)sp);
		x_sd1 = _mm_unpacklo_epi8(x_sd0, x_zero);

		x_sd2 = _mm_unpacklo_epi16(x_sd1, x_zero);
		x_sd3 = _mm_unpackhi_epi16(x_sd1, x_zero);

		x_sd4 = _mm_unpacklo_epi64(x_sd2, x_zero);
		x_sd5 = _mm_unpackhi_epi64(x_sd2, x_zero);
		x_dd0 = _mm_cvtepi32_pd(x_sd4);
		x_dd1 = _mm_cvtepi32_pd(x_sd5);
		_mm_store_pd(dp, x_dd0);
		dp += 2;
		_mm_store_pd(dp, x_dd1);
		dp += 2;

		x_sd8 = _mm_unpacklo_epi64(x_sd3, x_zero);
		x_sd9 = _mm_unpackhi_epi64(x_sd3, x_zero);
		x_dd2 = _mm_cvtepi32_pd(x_sd8);
		x_dd3 = _mm_cvtepi32_pd(x_sd9);
		_mm_store_pd(dp, x_dd2);
		dp += 2;
		_mm_store_pd(dp, x_dd3);
		dp += 2;

		sp += 8;
		i += 8;
	}

/* 1 pixels */
	for (; i < dsize; i++) {
		(*dp++) = (mlib_d64)(*sp++);
	}
}

/* *********************************************************** */

void mlib_s_ImageDataTypeConvert_U8_D64_A8D2X8(
    const mlib_u8 *src,
    mlib_s32 slb,
    mlib_d64 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize)
{
	mlib_u8 *sp, *sl;
	mlib_d64 *dp, *dl;
	mlib_s32 j;

	sp = sl = (void *)src;
	dp = dl = dst;

	for (j = 0; j < ysize; j++) {

		mlib_s_ImageDataTypeConvert_U8_D64_A8D1X8(sp, dp, xsize);

		sp = sl = sl + slb;
		dp = dl = (mlib_d64 *)((mlib_u8 *)dl + dlb);
	}
}

/* *********************************************************** */

void mlib_s_ImageDataTypeConvert_U8_D64_D1(
    const mlib_u8 *src,
    mlib_d64 *dst,
    mlib_s32 dsize)
{
/* pointer for pixel in source */
	mlib_u8 *sp;

/* pointer for pixel in destination */
	mlib_d64 *dp;

	__m128i x_sd0, x_sd1, x_sd2, x_sd3, x_sd4, x_sd5, x_sd6, x_sd7;
	__m128i x_sd8, x_sd9, x_sd10, x_sd11, x_sd12, x_sd13, x_sd14;
	__m128d x_dd0, x_dd1, x_dd2, x_dd3, x_dd4, x_dd5, x_dd6, x_dd7;
	mlib_s32 i;
	const __m128i x_zero = _mm_setzero_si128();

	sp = (void *)src;
	dp = dst;

/* 16 pixels */
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < (dsize - 15); i += 16) {
		x_sd0 = _mm_loadu_si128((__m128i *)sp);
		x_sd1 = _mm_unpacklo_epi8(x_sd0, x_zero);
		x_sd2 = _mm_unpackhi_epi8(x_sd0, x_zero);

		x_sd3 = _mm_unpacklo_epi16(x_sd1, x_zero);
		x_sd4 = _mm_unpackhi_epi16(x_sd1, x_zero);
		x_sd5 = _mm_unpacklo_epi16(x_sd2, x_zero);
		x_sd6 = _mm_unpackhi_epi16(x_sd2, x_zero);

		x_sd7 = _mm_unpacklo_epi64(x_sd3, x_zero);
		x_sd8 = _mm_unpackhi_epi64(x_sd3, x_zero);
		x_dd0 = _mm_cvtepi32_pd(x_sd7);
		x_dd1 = _mm_cvtepi32_pd(x_sd8);
		_mm_storeu_pd(dp, x_dd0);
		dp += 2;
		_mm_storeu_pd(dp, x_dd1);
		dp += 2;

		x_sd9 = _mm_unpacklo_epi64(x_sd4, x_zero);
		x_sd10 = _mm_unpackhi_epi64(x_sd4, x_zero);
		x_dd2 = _mm_cvtepi32_pd(x_sd9);
		x_dd3 = _mm_cvtepi32_pd(x_sd10);
		_mm_storeu_pd(dp, x_dd2);
		dp += 2;
		_mm_storeu_pd(dp, x_dd3);
		dp += 2;

		x_sd11 = _mm_unpacklo_epi64(x_sd5, x_zero);
		x_sd12 = _mm_unpackhi_epi64(x_sd5, x_zero);
		x_dd4 = _mm_cvtepi32_pd(x_sd11);
		x_dd5 = _mm_cvtepi32_pd(x_sd12);
		_mm_storeu_pd(dp, x_dd4);
		dp += 2;
		_mm_storeu_pd(dp, x_dd5);
		dp += 2;

		x_sd13 = _mm_unpacklo_epi64(x_sd6, x_zero);
		x_sd14 = _mm_unpackhi_epi64(x_sd6, x_zero);
		x_dd6 = _mm_cvtepi32_pd(x_sd13);
		x_dd7 = _mm_cvtepi32_pd(x_sd14);
		_mm_storeu_pd(dp, x_dd6);
		dp += 2;
		_mm_storeu_pd(dp, x_dd7);
		dp += 2;

		sp += 16;
	}

/* 8 pixels */
	if (i < (dsize - 7)) {
		x_sd0 = _mm_loadl_epi64((__m128i *)sp);
		x_sd1 = _mm_unpacklo_epi8(x_sd0, x_zero);

		x_sd2 = _mm_unpacklo_epi16(x_sd1, x_zero);
		x_sd3 = _mm_unpackhi_epi16(x_sd1, x_zero);

		x_sd4 = _mm_unpacklo_epi64(x_sd2, x_zero);
		x_sd5 = _mm_unpackhi_epi64(x_sd2, x_zero);
		x_dd0 = _mm_cvtepi32_pd(x_sd4);
		x_dd1 = _mm_cvtepi32_pd(x_sd5);
		_mm_storeu_pd(dp, x_dd0);
		dp += 2;
		_mm_storeu_pd(dp, x_dd1);
		dp += 2;

		x_sd8 = _mm_unpacklo_epi64(x_sd3, x_zero);
		x_sd9 = _mm_unpackhi_epi64(x_sd3, x_zero);
		x_dd2 = _mm_cvtepi32_pd(x_sd8);
		x_dd3 = _mm_cvtepi32_pd(x_sd9);
		_mm_storeu_pd(dp, x_dd2);
		dp += 2;
		_mm_storeu_pd(dp, x_dd3);
		dp += 2;

		sp += 8;
		i += 8;
	}

/* 1 pixels */
	for (; i < dsize; i++) {
		(*dp++) = (mlib_d64)(*sp++);
	}
}

/* *********************************************************** */

void mlib_s_ImageDataTypeConvert_U8_D64(
    const mlib_u8 *src,
    mlib_s32 slb,
    mlib_d64 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize)
{
	mlib_u8 *sp, *sl;
	mlib_d64 *dp, *dl;
	mlib_s32 j;

	sp = sl = (void *)src;
	dp = dl = dst;

	for (j = 0; j < ysize; j++) {

		mlib_s_ImageDataTypeConvert_U8_D64_D1(sp, dp, xsize);

		sp = sl = sl + slb;
		dp = dl = (mlib_d64 *)((mlib_u8 *)dl + dlb);
	}
}

/* *********************************************************** */

/* U16 -> BIT */
void mlib_s_ImageDataTypeConvert_U16_BIT_A8D1X8(
    const mlib_u8 *src,
    mlib_u8 *dst,
    mlib_s32 dsize)
{
/* pointer for pixel in source */
	mlib_u16 *sp;

/* pointer for pixel in destination */
	mlib_u8 *dp, *dl;

/* intermediate value */
	mlib_s32 bitdata;

	__m128i x_sd0, x_sd1;
	__m128i x_dd0, x_dd1, x_dd2, x_dd3, x_dd4, x_dd5, x_dd6;
	__m128i x_dd7, x_dd8, x_dd9, x_dd10, x_dd11, x_dd12;
	mlib_s32 dd;
	mlib_u32 s0;
	mlib_s32 i;

	const __m128i x_zero = _mm_setzero_si128();
	const __m128i x_max_s16 = _mm_set1_epi16(0xFF);

	sp = (mlib_u16 *)src;
	dp = dl = dst;

/* 16 pixels */
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < (dsize - 15); i += 16) {
		x_sd0 = _mm_load_si128((__m128i *)sp);
		sp += 8;
		x_dd0 = _mm_cmpeq_epi16(x_sd0, x_zero);
		x_dd1 = _mm_cmpeq_epi16(x_dd0, x_zero);

		x_dd2 = _mm_and_si128(x_dd1, x_max_s16);
		x_dd3 = _mm_shuffle_epi32(x_dd2, 0x1B);
		x_dd4 = _mm_shufflehi_epi16(x_dd3, 0xB1);
		x_dd5 = _mm_shufflelo_epi16(x_dd4, 0xB1);

		x_sd1 = _mm_load_si128((__m128i *)sp);
		x_dd6 = _mm_cmpeq_epi16(x_sd1, x_zero);
		x_dd7 = _mm_cmpeq_epi16(x_dd6, x_zero);

		x_dd8 = _mm_and_si128(x_dd7, x_max_s16);
		x_dd9 = _mm_shuffle_epi32(x_dd8, 0x1B);
		x_dd10 = _mm_shufflehi_epi16(x_dd9, 0xB1);
		x_dd11 = _mm_shufflelo_epi16(x_dd10, 0xB1);

		x_dd12 = _mm_packus_epi16(x_dd5, x_dd11);
		dd = _mm_movemask_epi8(x_dd12);
		*((mlib_u16 *)dp) = (mlib_u16)dd;
		dp += 2;
		sp += 8;
	}

/* 8 pixels */
	if (i < (dsize - 7)) {
		x_sd0 = _mm_load_si128((__m128i *)sp);
		sp += 8;
		x_dd0 = _mm_cmpeq_epi16(x_sd0, x_zero);
		x_dd1 = _mm_cmpeq_epi16(x_dd0, x_zero);

		x_dd2 = _mm_and_si128(x_dd1, x_max_s16);
		x_dd3 = _mm_shuffle_epi32(x_dd2, 0x1B);
		x_dd4 = _mm_shufflehi_epi16(x_dd3, 0xB1);
		x_dd5 = _mm_shufflelo_epi16(x_dd4, 0xB1);

		x_dd6 = _mm_packus_epi16(x_dd5, x_dd5);
		dd = _mm_movemask_epi8(x_dd6);
		*((mlib_u8 *)dp) = (mlib_u8)dd;

		dp += 1;
		i += 8;
	}

	if (i < dsize) {
		mlib_s32 mask = 0xFF00 >> (dsize & 7);
		mlib_u32 and = 0x80000000;
		bitdata = 0;

		for (i = 0; i < (dsize & 7); i++) {
			s0 = -sp[0];
			bitdata |= (s0 & and);
			and >>= 1;
			sp++;
		}
		dl[dsize / 8] = (bitdata >> 24) |
		    (dl[dsize / 8] & ~mask);
	}
}

/* *********************************************************** */

void mlib_s_ImageDataTypeConvert_U16_BIT_A8D2X8(
    const mlib_u8 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize)
{
	mlib_u8 *sp, *sl;
	mlib_u8 *dp, *dl;
	mlib_s32 j;

	sp = sl = (mlib_u8 *)src;
	dp = dl = dst;

	for (j = 0; j < ysize; j++) {

		mlib_s_ImageDataTypeConvert_U16_BIT_A8D1X8(sp, dp, xsize);

		sp = sl = sl + slb;
		dp = dl = dl + dlb;
	}
}

/* *********************************************************** */

void mlib_s_ImageDataTypeConvert_U16_BIT_D1(
    const mlib_u8 *src,
    mlib_u8 *dst,
    mlib_s32 dsize)
{
/* pointer for pixel in source */
	mlib_u16 *sp;

/* pointer for pixel in destination */
	mlib_u8 *dp, *dl;

/* intermediate value */
	mlib_s32 bitdata;

	__m128i x_sd0, x_sd1;
	__m128i x_dd0, x_dd1, x_dd2, x_dd3, x_dd4, x_dd5, x_dd6;
	__m128i x_dd7, x_dd8, x_dd9, x_dd10, x_dd11, x_dd12;
	mlib_s32 dd;
	mlib_u32 s0;
	mlib_s32 i;

	const __m128i x_zero = _mm_setzero_si128();
	const __m128i x_max_s16 = _mm_set1_epi16(0xFF);

	sp = (mlib_u16 *)src;
	dp = dl = dst;

/* 16 pixels */
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < (dsize - 15); i += 16) {
		x_sd0 = _mm_loadu_si128((__m128i *)sp);
		sp += 8;
		x_dd0 = _mm_cmpeq_epi16(x_sd0, x_zero);
		x_dd1 = _mm_cmpeq_epi16(x_dd0, x_zero);

		x_dd2 = _mm_and_si128(x_dd1, x_max_s16);
		x_dd3 = _mm_shuffle_epi32(x_dd2, 0x1B);
		x_dd4 = _mm_shufflehi_epi16(x_dd3, 0xB1);
		x_dd5 = _mm_shufflelo_epi16(x_dd4, 0xB1);

		x_sd1 = _mm_loadu_si128((__m128i *)sp);
		x_dd6 = _mm_cmpeq_epi16(x_sd1, x_zero);
		x_dd7 = _mm_cmpeq_epi16(x_dd6, x_zero);

		x_dd8 = _mm_and_si128(x_dd7, x_max_s16);
		x_dd9 = _mm_shuffle_epi32(x_dd8, 0x1B);
		x_dd10 = _mm_shufflehi_epi16(x_dd9, 0xB1);
		x_dd11 = _mm_shufflelo_epi16(x_dd10, 0xB1);

		x_dd12 = _mm_packus_epi16(x_dd5, x_dd11);
		dd = _mm_movemask_epi8(x_dd12);
		*((mlib_u16 *)dp) = (mlib_u16)dd;
		dp += 2;
		sp += 8;
	}

/* 8 pixels */
	if (i < (dsize - 7)) {
		x_sd0 = _mm_loadu_si128((__m128i *)sp);
		sp += 8;
		x_dd0 = _mm_cmpeq_epi16(x_sd0, x_zero);
		x_dd1 = _mm_cmpeq_epi16(x_dd0, x_zero);

		x_dd2 = _mm_and_si128(x_dd1, x_max_s16);
		x_dd3 = _mm_shuffle_epi32(x_dd2, 0x1B);
		x_dd4 = _mm_shufflehi_epi16(x_dd3, 0xB1);
		x_dd5 = _mm_shufflelo_epi16(x_dd4, 0xB1);

		x_dd6 = _mm_packus_epi16(x_dd5, x_dd5);
		dd = _mm_movemask_epi8(x_dd6);
		*((mlib_u8 *)dp) = (mlib_u8)dd;

		dp += 1;
		i += 8;
	}

	if (i < dsize) {
		mlib_s32 mask = 0xFF00 >> (dsize & 7);
		mlib_u32 and = 0x80000000;
		bitdata = 0;

		for (i = 0; i < (dsize & 7); i++) {
			s0 = -sp[0];
			bitdata |= (s0 & and);
			and >>= 1;
			sp++;
		}
		dl[dsize / 8] = (bitdata >> 24) |
		    (dl[dsize / 8] & ~mask);
	}
}

/* *********************************************************** */

void mlib_s_ImageDataTypeConvert_U16_BIT(
    const mlib_u8 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize)
{
	mlib_u8 *sp, *sl;
	mlib_u8 *dp, *dl;
	mlib_s32 j;

	sp = sl = (mlib_u8 *)src;
	dp = dl = dst;

	for (j = 0; j < ysize; j++) {

		mlib_s_ImageDataTypeConvert_U16_BIT_D1(sp, dp, xsize);

		sp = sl = sl + slb;
		dp = dl = dl + dlb;
	}
}

/* *********************************************************** */

#define	MLIB_CLAMP_U8(dst, src)                         \
{                                                       \
	mlib_s32 v = src, mask = (v - 0xff) >> 31;      \
	dst = (v | ~mask) & ~(v >> 31);                 \
}

/* *********************************************************** */

/* U16 -> U8 */
void mlib_s_ImageDataTypeConvert_U16_U8_A8D1X8(
    const mlib_u16 *src,
    mlib_u8 *dst,
    mlib_s32 dsize)
{
/* pointer for pixel in source */
	mlib_u16 *sp;

/* pointer for pixel in destination */
	mlib_u8 *dp;

	__m128i x_sd0, x_sd1, x_sd2, x_sd3, x_mask;
	__m128i x_dd0, x_dd1, x_dd2;
	mlib_s32 sd, dd;
	mlib_s32 i;

	const __m128i x_cmp = _mm_set1_epi16(0x7FFF);
	const __m128i x_zero = _mm_setzero_si128();

	sp = (void *)src;
	dp = dst;

/* 16 pixels */
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < (dsize - 15); i += 16) {

		x_sd0 = _mm_loadu_si128((__m128i *)sp);
		sp += 8;
		x_sd1 = _mm_loadu_si128((__m128i *)sp);
		sp += 8;
		x_mask = _mm_cmplt_epi16(x_sd0, x_zero);
		x_sd2 = _mm_or_si128(x_sd0, x_mask);
		x_dd0 = _mm_and_si128(x_sd2, x_cmp);

		x_mask = _mm_cmplt_epi16(x_sd1, x_zero);
		x_sd3 = _mm_or_si128(x_sd1, x_mask);
		x_dd1 = _mm_and_si128(x_sd3, x_cmp);

		x_dd2 = _mm_packus_epi16(x_dd0, x_dd1);
		_mm_storeu_si128((__m128i *)dp, x_dd2);
		dp += 16;
	}

/* 8 pixels */
	if (i < (dsize - 7)) {
		x_sd0 = _mm_load_si128((__m128i *)sp);
		sp += 8;
		x_mask = _mm_cmplt_epi16(x_sd0, x_zero);
		x_sd1 = _mm_or_si128(x_sd0, x_mask);
		x_dd0 = _mm_and_si128(x_sd1, x_cmp);
		x_dd1 = _mm_packus_epi16(x_dd0, x_dd0);
		_mm_storel_epi64((__m128i *)dp, x_dd1);
		dp += 8;
		i += 8;
	}

/* 4 pixels */
	if (i < (dsize - 3)) {
		x_sd0 = _mm_loadl_epi64((__m128i *)sp);
		sp += 4;
		x_mask = _mm_cmplt_epi16(x_sd0, x_zero);
		x_sd1 = _mm_or_si128(x_sd0, x_mask);
		x_dd0 = _mm_and_si128(x_sd1, x_cmp);
		x_dd1 = _mm_packus_epi16(x_dd0, x_dd0);
		dd = _mm_cvtsi128_si32(x_dd1);
		*((mlib_s32 *)dp) = dd;
		dp += 4;
		i += 4;
	}

	for (; i < dsize; i++) {
		sd = (*sp++);
		MLIB_CLAMP_U8(*dp, sd);
		dp++;
	}
}

/* *********************************************************** */

void mlib_s_ImageDataTypeConvert_U16_U8_A8D2X8(
    const mlib_u16 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize)
{
	mlib_u16 *sp, *sl;
	mlib_u8 *dp, *dl;
	mlib_s32 j;

	sp = sl = (void *)src;
	dp = dl = dst;

	for (j = 0; j < ysize; j++) {

		mlib_s_ImageDataTypeConvert_U16_U8_A8D1X8(sp, dp, xsize);

		sp = sl = (mlib_u16 *)((mlib_u8 *)sl + slb);
		dp = dl = dl + dlb;
	}
}

/* *********************************************************** */

void mlib_s_ImageDataTypeConvert_U16_U8_D1(
    const mlib_u16 *src,
    mlib_u8 *dst,
    mlib_s32 dsize)
{
/* pointer for pixel in source */
	mlib_u16 *sp;

/* pointer for pixel in destination */
	mlib_u8 *dp;

	__m128i x_sd0, x_sd1, x_sd2, x_sd3, x_mask;
	__m128i x_dd0, x_dd1, x_dd2;
	mlib_s32 sd, dd;
	mlib_s32 i;

	const __m128i x_cmp = _mm_set1_epi16(0x7FFF);
	const __m128i x_zero = _mm_setzero_si128();

	sp = (void *)src;
	dp = dst;

/* 16 pixels */
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < (dsize - 15); i += 16) {
		x_sd0 = _mm_loadu_si128((__m128i *)sp);
		sp += 8;
		x_sd1 = _mm_loadu_si128((__m128i *)sp);
		sp += 8;
		x_mask = _mm_cmplt_epi16(x_sd0, x_zero);
		x_sd2 = _mm_or_si128(x_sd0, x_mask);
		x_dd0 = _mm_and_si128(x_sd2, x_cmp);

		x_mask = _mm_cmplt_epi16(x_sd1, x_zero);
		x_sd3 = _mm_or_si128(x_sd1, x_mask);
		x_dd1 = _mm_and_si128(x_sd3, x_cmp);

		x_dd2 = _mm_packus_epi16(x_dd0, x_dd1);
		_mm_storeu_si128((__m128i *)dp, x_dd2);
		dp += 16;
	}

/* 8 pixels */
	if (i < (dsize - 7)) {
		x_sd0 = _mm_loadu_si128((__m128i *)sp);
		sp += 8;
		x_mask = _mm_cmplt_epi16(x_sd0, x_zero);
		x_sd1 = _mm_or_si128(x_sd0, x_mask);
		x_dd0 = _mm_and_si128(x_sd1, x_cmp);
		x_dd1 = _mm_packus_epi16(x_dd0, x_dd0);
		_mm_storel_epi64((__m128i *)dp, x_dd1);
		dp += 8;
		i += 8;
	}

/* 4 pixels */
	if (i < (dsize - 3)) {
		x_sd0 = _mm_loadl_epi64((__m128i *)sp);
		sp += 4;
		x_mask = _mm_cmplt_epi16(x_sd0, x_zero);
		x_sd1 = _mm_or_si128(x_sd0, x_mask);
		x_dd0 = _mm_and_si128(x_sd1, x_cmp);
		x_dd1 = _mm_packus_epi16(x_dd0, x_dd0);
		dd = _mm_cvtsi128_si32(x_dd1);
		*((mlib_s32 *)dp) = dd;
		dp += 4;
		i += 4;
	}

	for (; i < dsize; i++) {
		sd = (*sp++);
		MLIB_CLAMP_U8(*dp, sd);
		dp++;
	}
}

/* *********************************************************** */

void mlib_s_ImageDataTypeConvert_U16_U8(
    const mlib_u16 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize)
{
	mlib_u16 *sp, *sl;
	mlib_u8 *dp, *dl;
	mlib_s32 j;

	sp = sl = (void *)src;
	dp = dl = dst;

	for (j = 0; j < ysize; j++) {

		mlib_s_ImageDataTypeConvert_U16_U8_D1(sp, dp, xsize);

		sp = sl = (mlib_u16 *)((mlib_u8 *)sl + slb);
		dp = dl = dl + dlb;
	}
}

/* *********************************************************** */

/* U16 -> S16 */
void mlib_s_ImageDataTypeConvert_U16_S16_A8D1X8(
    const mlib_u16 *src,
    mlib_s16 *dst,
    mlib_s32 dsize)
{
/* pointer for pixel in source */
	mlib_u16 *sp;

/* pointer for pixel in destination */
	mlib_s16 *dp;

	__m128i x_sd0, x_sd1, x_mask;
	__m128i x_dd0;
	mlib_s32 sd;
	mlib_s32 i;

	const __m128i x_cmp = _mm_set1_epi16(0x7FFF);
	const __m128i x_zero = _mm_setzero_si128();

	sp = (void *)src;
	dp = dst;

/* 8 pixels */
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < (dsize - 7); i += 8) {
		x_sd0 = _mm_load_si128((__m128i *)sp);
		x_mask = _mm_cmplt_epi16(x_sd0, x_zero);
		x_sd1 = _mm_or_si128(x_sd0, x_mask);
		x_dd0 = _mm_and_si128(x_sd1, x_cmp);
		_mm_store_si128((__m128i *)dp, x_dd0);
		dp += 8;
		sp += 8;
	}

/* 4 pixels */
	if (i < (dsize - 3)) {
		x_sd0 = _mm_loadl_epi64((__m128i *)sp);
		x_mask = _mm_cmplt_epi16(x_sd0, x_zero);
		x_sd1 = _mm_or_si128(x_sd0, x_mask);
		x_dd0 = _mm_and_si128(x_sd1, x_cmp);
		_mm_storel_epi64((__m128i *)dp, x_dd0);
		dp += 4;
		sp += 4;
		i += 4;
	}

/* 1 pixels */
	for (; i < dsize; i++) {
		sd = (*sp++);
		(*dp++) =
		    (((((mlib_s32)(32767 - sd)) >> 31) | sd) & 0x7fff);
	}
}

/* *********************************************************** */

void mlib_s_ImageDataTypeConvert_U16_S16_A8D2X8(
    const mlib_u16 *src,
    mlib_s32 slb,
    mlib_s16 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize)
{
	mlib_u16 *sp, *sl;
	mlib_s16 *dp, *dl;
	mlib_s32 j;

	sp = sl = (void *)src;
	dp = dl = dst;

	for (j = 0; j < ysize; j++) {

		mlib_s_ImageDataTypeConvert_U16_S16_A8D1X8(sp, dp, xsize);

		sp = sl = (mlib_u16 *)((mlib_u8 *)sl + slb);
		dp = dl = (mlib_s16 *)((mlib_u8 *)dl + dlb);
	}
}

/* *********************************************************** */

void mlib_s_ImageDataTypeConvert_U16_S16_D1(
    const mlib_u16 *src,
    mlib_s16 *dst,
    mlib_s32 dsize)
{
/* pointer for pixel in source */
	mlib_u16 *sp;

/* pointer for pixel in destination */
	mlib_s16 *dp;

	__m128i x_sd0, x_sd1, x_mask;
	__m128i x_dd0;
	mlib_s32 sd;
	mlib_s32 i;

	const __m128i x_cmp = _mm_set1_epi16(32767);
	const __m128i x_zero = _mm_setzero_si128();

	sp = (void *)src;
	dp = dst;

/* 8 pixels */
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < (dsize - 7); i += 8) {
		x_sd0 = _mm_loadu_si128((__m128i *)sp);
		x_mask = _mm_cmplt_epi16(x_sd0, x_zero);
		x_sd1 = _mm_or_si128(x_sd0, x_mask);
		x_dd0 = _mm_and_si128(x_sd1, x_cmp);
		_mm_storeu_si128((__m128i *)dp, x_dd0);
		dp += 8;
		sp += 8;
	}

/* 4 pixels */
	if (i < (dsize - 3)) {
		x_sd0 = _mm_loadl_epi64((__m128i *)sp);
		x_mask = _mm_cmplt_epi16(x_sd0, x_zero);
		x_sd1 = _mm_or_si128(x_sd0, x_mask);
		x_dd0 = _mm_and_si128(x_sd1, x_cmp);
		_mm_storel_epi64((__m128i *)dp, x_dd0);
		dp += 4;
		sp += 4;
		i += 4;
	}

/* 1 pixels */
	for (; i < dsize; i++) {
		sd = (*sp++);
		(*dp++) =
		    (((((mlib_s32)(32767 - sd)) >> 31) | sd) & 0x7fff);
	}
}

/* *********************************************************** */

void mlib_s_ImageDataTypeConvert_U16_S16(
    const mlib_u16 *src,
    mlib_s32 slb,
    mlib_s16 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize)
{
	mlib_u16 *sp, *sl;
	mlib_s16 *dp, *dl;
	mlib_s32 j;

	sp = sl = (void *)src;
	dp = dl = dst;

	for (j = 0; j < ysize; j++) {

		mlib_s_ImageDataTypeConvert_U16_S16_D1(sp, dp, xsize);

		sp = sl = (mlib_u16 *)((mlib_u8 *)sl + slb);
		dp = dl = (mlib_s16 *)((mlib_u8 *)dl + dlb);
	}
}

/* *********************************************************** */

/* U16 -> S32 */
void mlib_s_ImageDataTypeConvert_U16_S32_A8D1X8(
    const mlib_u16 *src,
    mlib_s32 *dst,
    mlib_s32 dsize)
{
/* pointer for pixel in source */
	mlib_u16 *sp;

/* pointer for pixel in destination */
	mlib_s32 *dp;

/* pointer for pixel in source */
	mlib_u32 *sp2;

	__m128i x_sd0, x_sd1;
	__m128i x_dd0, x_dd1;
	mlib_u32 val0;
	mlib_s32 i;

	const __m128i x_zero = _mm_setzero_si128();

	sp = (void *)src;
	dp = dst;

/* 8 pixels */
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < (dsize - 7); i += 8) {
		x_sd0 = _mm_load_si128((__m128i *)sp);
		x_dd0 = _mm_unpacklo_epi16(x_sd0, x_zero);
		x_dd1 = _mm_unpackhi_epi16(x_sd0, x_zero);
		_mm_store_si128((__m128i *)dp, x_dd0);
		dp += 4;
		_mm_store_si128((__m128i *)dp, x_dd1);
		dp += 4;
		sp += 8;
	}

/* 4 pixels */
	if (i < (dsize - 3)) {
		x_sd0 = _mm_loadl_epi64((__m128i *)sp);
		x_dd0 = _mm_unpacklo_epi16(x_sd0, x_zero);
		_mm_store_si128((__m128i *)dp, x_dd0);
		dp += 4;
		sp += 4;
		i += 4;
	}

/* 2 pixels */
	sp2 = (mlib_u32 *)sp;
	if (i < (dsize - 1)) {
		val0 = (*sp2++);
		dp[0] = (val0 & 0xFFFF);
		dp[1] = (val0 >> 16);
		dp += 2;
		i += 2;
	}

/* 1 pixels */
	sp = (mlib_u16 *)sp2;
	if (i < dsize) {
		(*dp++) = (*sp++);
	}
}

/* *********************************************************** */

void mlib_s_ImageDataTypeConvert_U16_S32_A8D2X8(
    const mlib_u16 *src,
    mlib_s32 slb,
    mlib_s32 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize)
{
	mlib_u16 *sp, *sl;
	mlib_s32 *dp, *dl;
	mlib_s32 j;

	sp = sl = (void *)src;
	dp = dl = dst;

	for (j = 0; j < ysize; j++) {

		mlib_s_ImageDataTypeConvert_U16_S32_A8D1X8(sp, dp, xsize);

		sp = sl = (mlib_u16 *)((mlib_u8 *)sl + slb);
		dp = dl = (mlib_s32 *)((mlib_u8 *)dl + dlb);
	}
}

/* *********************************************************** */

void mlib_s_ImageDataTypeConvert_U16_S32_D1(
    const mlib_u16 *src,
    mlib_s32 *dst,
    mlib_s32 dsize)
{
/* pointer for pixel in source */
	mlib_u16 *sp;

/* pointer for pixel in destination */
	mlib_s32 *dp;

/* pointer for pixel in source */
	mlib_u32 *sp2;

	__m128i x_sd0, x_sd1;
	__m128i x_dd0, x_dd1;
	mlib_u32 val0;
	mlib_s32 i;

	const __m128i x_zero = _mm_setzero_si128();

	sp = (void *)src;
	dp = dst;

/* 8 pixels */
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < (dsize - 7); i += 8) {
		x_sd0 = _mm_loadu_si128((__m128i *)sp);
		x_dd0 = _mm_unpacklo_epi16(x_sd0, x_zero);
		x_dd1 = _mm_unpackhi_epi16(x_sd0, x_zero);
		_mm_storeu_si128((__m128i *)dp, x_dd0);
		dp += 4;
		_mm_storeu_si128((__m128i *)dp, x_dd1);
		dp += 4;
		sp += 8;
	}

/* 4 pixels */
	if (i < (dsize - 3)) {
		x_sd0 = _mm_loadl_epi64((__m128i *)sp);
		x_dd0 = _mm_unpacklo_epi16(x_sd0, x_zero);
		_mm_storeu_si128((__m128i *)dp, x_dd0);
		dp += 4;
		sp += 4;
		i += 4;
	}

/* 2 pixels */
	sp2 = (mlib_u32 *)sp;
	if (i < (dsize - 1)) {
		val0 = (*sp2++);
		dp[0] = (val0 & 0xFFFF);
		dp[1] = (val0 >> 16);
		dp += 2;
		i += 2;
	}

/* 1 pixels */
	sp = (mlib_u16 *)sp2;
	if (i < dsize) {
		(*dp++) = (*sp++);
	}
}

/* *********************************************************** */

void mlib_s_ImageDataTypeConvert_U16_S32(
    const mlib_u16 *src,
    mlib_s32 slb,
    mlib_s32 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize)
{
	mlib_u16 *sp, *sl;
	mlib_s32 *dp, *dl;
	mlib_s32 j;

	sp = sl = (void *)src;
	dp = dl = dst;

	for (j = 0; j < ysize; j++) {

		mlib_s_ImageDataTypeConvert_U16_S32_D1(sp, dp, xsize);

		sp = sl = (mlib_u16 *)((mlib_u8 *)sl + slb);
		dp = dl = (mlib_s32 *)((mlib_u8 *)dl + dlb);
	}
}

/* *********************************************************** */

/* U16 -> F32 */
void mlib_s_ImageDataTypeConvert_U16_F32_A8D1X8(
    const mlib_u16 *src,
    mlib_f32 *dst,
    mlib_s32 dsize)
{
/* pointer for pixel in source */
	mlib_u16 *sp;

/* pointer for pixel in destination */
	mlib_f32 *dp;

/* pointer for pixel in source */
	mlib_u32 *sp2;

	__m128i x_sd0, x_sd1;
	__m128i x_dd0, x_dd1;
	__m128 x_df0, x_df1;
	mlib_u32 val0;
	mlib_s32 i;

	const __m128i x_zero = _mm_setzero_si128();

	sp = (void *)src;
	dp = dst;

/* 8 pixels */
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < (dsize - 7); i += 8) {
		x_sd0 = _mm_load_si128((__m128i *)sp);
		x_dd0 = _mm_unpacklo_epi16(x_sd0, x_zero);
		x_dd1 = _mm_unpackhi_epi16(x_sd0, x_zero);
		x_df0 = _mm_cvtepi32_ps(x_dd0);
		x_df1 = _mm_cvtepi32_ps(x_dd1);
		_mm_store_ps(dp, x_df0);
		dp += 4;
		_mm_store_ps(dp, x_df1);
		dp += 4;
		sp += 8;
	}

/* 4 pixels */
	if (i < (dsize - 3)) {
		x_sd0 = _mm_loadl_epi64((__m128i *)sp);
		x_dd0 = _mm_unpacklo_epi16(x_sd0, x_zero);
		x_df0 = _mm_cvtepi32_ps(x_dd0);
		_mm_store_ps(dp, x_df0);
		dp += 4;
		sp += 4;
		i += 4;
	}

/* 2 pixels */
	sp2 = (mlib_u32 *)sp;
	if (i < (dsize - 1)) {
		val0 = (*sp2++);
		dp[0] = (val0 & 0xFFFF);
		dp[1] = (val0 >> 16);
		dp += 2;
		i += 2;
	}

/* 1 pixels */
	sp = (mlib_u16 *)sp2;
	if (i < dsize) {
		(*dp++) = (*sp++);
	}
}

/* *********************************************************** */

void mlib_s_ImageDataTypeConvert_U16_F32_A8D2X8(
    const mlib_u16 *src,
    mlib_s32 slb,
    mlib_f32 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize)
{
	mlib_u16 *sp, *sl;
	mlib_f32 *dp, *dl;
	mlib_s32 j;

	sp = sl = (void *)src;
	dp = dl = dst;

	for (j = 0; j < ysize; j++) {

		mlib_s_ImageDataTypeConvert_U16_F32_A8D1X8(sp, dp, xsize);

		sp = sl = (mlib_u16 *)((mlib_u8 *)sl + slb);
		dp = dl = (mlib_f32 *)((mlib_u8 *)dl + dlb);
	}
}

/* *********************************************************** */

void mlib_s_ImageDataTypeConvert_U16_F32_D1(
    const mlib_u16 *src,
    mlib_f32 *dst,
    mlib_s32 dsize)
{
/* pointer for pixel in source */
	mlib_u16 *sp;

/* pointer for pixel in destination */
	mlib_f32 *dp;

/* pointer for pixel in source */
	mlib_u32 *sp2;

	__m128i x_sd0, x_sd1;
	__m128i x_dd0, x_dd1;
	__m128 x_df0, x_df1;
	mlib_u32 val0;
	mlib_s32 i;

	const __m128i x_zero = _mm_setzero_si128();

	sp = (void *)src;
	dp = dst;

/* 8 pixels */
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < (dsize - 7); i += 8) {
		x_sd0 = _mm_loadu_si128((__m128i *)sp);
		x_dd0 = _mm_unpacklo_epi16(x_sd0, x_zero);
		x_dd1 = _mm_unpackhi_epi16(x_sd0, x_zero);
		x_df0 = _mm_cvtepi32_ps(x_dd0);
		x_df1 = _mm_cvtepi32_ps(x_dd1);
		_mm_storeu_ps(dp, x_df0);
		dp += 4;
		_mm_storeu_ps(dp, x_df1);
		dp += 4;
		sp += 8;
	}

/* 4 pixels */
	if (i < (dsize - 3)) {
		x_sd0 = _mm_loadl_epi64((__m128i *)sp);
		x_dd0 = _mm_unpacklo_epi16(x_sd0, x_zero);
		x_df0 = _mm_cvtepi32_ps(x_dd0);
		_mm_storeu_ps(dp, x_df0);
		dp += 4;
		sp += 4;
		i += 4;
	}

/* 2 pixels */
	sp2 = (mlib_u32 *)sp;
	if (i < (dsize - 1)) {
		val0 = (*sp2++);
		dp[0] = (val0 & 0xFFFF);
		dp[1] = (val0 >> 16);
		dp += 2;
		i += 2;
	}

/* 1 pixels */
	sp = (mlib_u16 *)sp2;
	if (i < dsize) {
		(*dp++) = (*sp++);
	}
}

/* *********************************************************** */

void mlib_s_ImageDataTypeConvert_U16_F32(
    const mlib_u16 *src,
    mlib_s32 slb,
    mlib_f32 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize)
{
	mlib_u16 *sp, *sl;
	mlib_f32 *dp, *dl;
	mlib_s32 j;

	sp = sl = (void *)src;
	dp = dl = dst;

	for (j = 0; j < ysize; j++) {

		mlib_s_ImageDataTypeConvert_U16_F32_D1(sp, dp, xsize);

		sp = sl = (mlib_u16 *)((mlib_u8 *)sl + slb);
		dp = dl = (mlib_f32 *)((mlib_u8 *)dl + dlb);
	}
}

/* *********************************************************** */

/* U16 -> D64 */
void mlib_s_ImageDataTypeConvert_U16_D64_A8D1X8(
    const mlib_u16 *src,
    mlib_d64 *dst,
    mlib_s32 dsize)
{
/* pointer for pixel in source */
	mlib_u16 *sp;

/* pointer for pixel in destination */
	mlib_d64 *dp;

	__m128i x_sd0, x_sd1, x_sd2, x_sd3;
	__m128i x_sd4, x_sd5, x_sd6, x_sd7;
	__m128d x_dd0, x_dd1, x_dd2, x_dd3;
	__m128d x_dd4, x_dd5, x_dd6, x_dd7;
	mlib_s32 i;
	const __m128i x_zero = _mm_setzero_si128();

	sp = (void *)src;
	dp = dst;

/* 8 pixels */
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < (dsize - 7); i += 8) {
		x_sd0 = _mm_load_si128((__m128i *)sp);
		x_sd2 = _mm_unpacklo_epi16(x_sd0, x_zero);
		x_sd3 = _mm_unpackhi_epi16(x_sd0, x_zero);

		x_sd4 = _mm_unpacklo_epi64(x_sd2, x_zero);
		x_sd5 = _mm_unpackhi_epi64(x_sd2, x_zero);
		x_dd2 = _mm_cvtepi32_pd(x_sd4);
		x_dd3 = _mm_cvtepi32_pd(x_sd5);

		x_sd6 = _mm_unpacklo_epi64(x_sd3, x_zero);
		x_sd7 = _mm_unpackhi_epi64(x_sd3, x_zero);
		x_dd6 = _mm_cvtepi32_pd(x_sd6);
		x_dd7 = _mm_cvtepi32_pd(x_sd7);

		_mm_store_pd(dp, x_dd2);
		dp += 2;
		_mm_store_pd(dp, x_dd3);
		dp += 2;
		_mm_store_pd(dp, x_dd6);
		dp += 2;
		_mm_store_pd(dp, x_dd7);
		dp += 2;
		sp += 8;
	}

/* 4 pixels */
	if (i < (dsize - 3)) {
		x_sd0 = _mm_loadl_epi64((__m128i *)sp);
		x_sd1 = _mm_srai_epi16(x_sd0, 16);
		x_sd2 = _mm_unpacklo_epi16(x_sd0, x_zero);

		x_sd3 = _mm_unpacklo_epi64(x_sd2, x_zero);
		x_sd4 = _mm_unpackhi_epi64(x_sd2, x_zero);
		x_dd2 = _mm_cvtepi32_pd(x_sd3);
		x_dd3 = _mm_cvtepi32_pd(x_sd4);

		_mm_store_pd(dp, x_dd2);
		dp += 2;
		_mm_store_pd(dp, x_dd3);
		dp += 2;
		sp += 4;
		i += 4;
	}

/* 1 pixels */
	for (; i < dsize; i++) {
		(*dp++) = (*sp++);
	}
}

/* *********************************************************** */

void mlib_s_ImageDataTypeConvert_U16_D64_A8D2X8(
    const mlib_u16 *src,
    mlib_s32 slb,
    mlib_d64 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize)
{
	mlib_u16 *sp, *sl;
	mlib_d64 *dp, *dl;
	mlib_s32 j;

	sp = sl = (void *)src;
	dp = dl = dst;

	for (j = 0; j < ysize; j++) {

		mlib_s_ImageDataTypeConvert_U16_D64_A8D1X8(sp, dp, xsize);

		sp = sl = (mlib_u16 *)((mlib_u8 *)sl + slb);
		dp = dl = (mlib_d64 *)((mlib_u8 *)dl + dlb);
	}
}

/* *********************************************************** */

void mlib_s_ImageDataTypeConvert_U16_D64_D1(
    const mlib_u16 *src,
    mlib_d64 *dst,
    mlib_s32 dsize)
{
/* pointer for pixel in source */
	mlib_u16 *sp;

/* pointer for pixel in destination */
	mlib_d64 *dp;

	__m128i x_sd0, x_sd1, x_sd2, x_sd3;
	__m128i x_sd4, x_sd5, x_sd6, x_sd7;
	__m128d x_dd0, x_dd1, x_dd2, x_dd3;
	__m128d x_dd4, x_dd5, x_dd6, x_dd7;
	mlib_s32 i;
	const __m128i x_zero = _mm_setzero_si128();

	sp = (void *)src;
	dp = dst;

/* 8 pixels */
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < (dsize - 7); i += 8) {
		x_sd0 = _mm_loadu_si128((__m128i *)sp);
		x_sd2 = _mm_unpacklo_epi16(x_sd0, x_zero);
		x_sd3 = _mm_unpackhi_epi16(x_sd0, x_zero);

		x_sd4 = _mm_unpacklo_epi64(x_sd2, x_zero);
		x_sd5 = _mm_unpackhi_epi64(x_sd2, x_zero);
		x_dd2 = _mm_cvtepi32_pd(x_sd4);
		x_dd3 = _mm_cvtepi32_pd(x_sd5);

		x_sd6 = _mm_unpacklo_epi64(x_sd3, x_zero);
		x_sd7 = _mm_unpackhi_epi64(x_sd3, x_zero);
		x_dd6 = _mm_cvtepi32_pd(x_sd6);
		x_dd7 = _mm_cvtepi32_pd(x_sd7);

		_mm_storeu_pd(dp, x_dd2);
		dp += 2;
		_mm_storeu_pd(dp, x_dd3);
		dp += 2;
		_mm_storeu_pd(dp, x_dd6);
		dp += 2;
		_mm_storeu_pd(dp, x_dd7);
		dp += 2;
		sp += 8;
	}

/* 4 pixels */
	if (i < (dsize - 3)) {
		x_sd0 = _mm_loadl_epi64((__m128i *)sp);
		x_sd1 = _mm_srai_epi16(x_sd0, 16);
		x_sd2 = _mm_unpacklo_epi16(x_sd0, x_zero);

		x_sd3 = _mm_unpacklo_epi64(x_sd2, x_zero);
		x_sd4 = _mm_unpackhi_epi64(x_sd2, x_zero);
		x_dd2 = _mm_cvtepi32_pd(x_sd3);
		x_dd3 = _mm_cvtepi32_pd(x_sd4);

		_mm_storeu_pd(dp, x_dd2);
		dp += 2;
		_mm_storeu_pd(dp, x_dd3);
		dp += 2;
		sp += 4;
		i += 4;
	}

/* 1 pixels */
	for (; i < dsize; i++) {
		(*dp++) = (*sp++);
	}
}

/* *********************************************************** */

void mlib_s_ImageDataTypeConvert_U16_D64(
    const mlib_u16 *src,
    mlib_s32 slb,
    mlib_d64 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize)
{
	mlib_u16 *sp, *sl;
	mlib_d64 *dp, *dl;
	mlib_s32 j;

	sp = sl = (void *)src;
	dp = dl = dst;

	for (j = 0; j < ysize; j++) {

		mlib_s_ImageDataTypeConvert_U16_D64_D1(sp, dp, xsize);

		sp = sl = (mlib_u16 *)((mlib_u8 *)sl + slb);
		dp = dl = (mlib_d64 *)((mlib_u8 *)dl + dlb);
	}
}

/* *********************************************************** */

/* S16 -> BIT */
void mlib_s_ImageDataTypeConvert_S16_BIT_A8D1X8(
    const mlib_u8 *src,
    mlib_u8 *dst,
    mlib_s32 dsize)
{
/* pointer for pixel in source */
	mlib_s16 *sp;

/* pointer for pixel in destination */
	mlib_u8 *dp, *dl;

/* intermediate value */
	mlib_s32 bitdata;

	__m128i x_sd0, x_sd1;
	__m128i x_dd0, x_dd1, x_dd2, x_dd3, x_dd4, x_dd5;
	__m128i x_dd6, x_dd7, x_dd8, x_dd9, x_dd10;
	mlib_s32 dd, s0;
	mlib_s32 i;

	const __m128i x_zero = _mm_setzero_si128();
	const __m128i x_mask = _mm_set1_epi16(0xFF);

	sp = (mlib_s16 *)src;
	dp = dl = dst;

/* 16 pixels */
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < (dsize - 15); i += 16) {
		x_sd0 = _mm_load_si128((__m128i *)sp);
		sp += 8;
		x_dd0 = _mm_cmpgt_epi16(x_sd0, x_zero);
		x_dd1 = _mm_shuffle_epi32(x_dd0, 0x1B);
		x_dd2 = _mm_shufflehi_epi16(x_dd1, 0xB1);
		x_dd3 = _mm_shufflelo_epi16(x_dd2, 0xB1);
		x_dd4 = _mm_and_si128(x_dd3, x_mask);

		x_sd1 = _mm_load_si128((__m128i *)sp);
		x_dd5 = _mm_cmpgt_epi16(x_sd1, x_zero);
		x_dd6 = _mm_shuffle_epi32(x_dd5, 0x1B);
		x_dd7 = _mm_shufflehi_epi16(x_dd6, 0xB1);
		x_dd8 = _mm_shufflelo_epi16(x_dd7, 0xB1);
		x_dd9 = _mm_and_si128(x_dd8, x_mask);

		x_dd10 = _mm_packus_epi16(x_dd4, x_dd9);
		dd = _mm_movemask_epi8(x_dd10);
		*((mlib_u16 *)dp) = (mlib_u16)dd;
		dp += 2;
		sp += 8;
	}

/* 8 pixels */
	if (i < (dsize - 7)) {
		x_sd0 = _mm_loadu_si128((__m128i *)sp);
		sp += 8;
		x_dd0 = _mm_cmpgt_epi16(x_sd0, x_zero);

		x_dd1 = _mm_shuffle_epi32(x_dd0, 0x1B);
		x_dd2 = _mm_shufflehi_epi16(x_dd1, 0xB1);
		x_dd3 = _mm_shufflelo_epi16(x_dd2, 0xB1);

		x_dd4 = _mm_and_si128(x_dd3, x_mask);
		x_dd5 = _mm_packus_epi16(x_dd4, x_dd4);
		dd = _mm_movemask_epi8(x_dd5);
		*((mlib_u8 *)dp) = (mlib_u8)dd;

		dp += 1;
		i += 8;
	}

	if (i < dsize) {
		mlib_s32 mask = 0xFF00 >> (dsize & 7);
		mlib_u32 and = 0x80000000;
		bitdata = 0;

		for (; i < dsize; i++) {
			s0 = -sp[0];
			bitdata |= (s0 & and);
			and >>= 1;
			sp++;
		}
		dl[dsize / 8] = (bitdata >> 24) |
		    (dl[dsize / 8] & ~mask);
	}
}

/* *********************************************************** */

void mlib_s_ImageDataTypeConvert_S16_BIT_A8D2X8(
    const mlib_u8 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize)
{
	mlib_u8 *sp, *sl;
	mlib_u8 *dp, *dl;
	mlib_s32 j;

	sp = sl = (mlib_u8 *)src;
	dp = dl = dst;

	for (j = 0; j < ysize; j++) {

		mlib_s_ImageDataTypeConvert_S16_BIT_A8D1X8(sp, dp, xsize);

		sp = sl = (mlib_u8 *)((mlib_u8 *)sl + slb);
		dp = dl = dl + dlb;
	}
}

/* *********************************************************** */

void mlib_s_ImageDataTypeConvert_S16_BIT_D1(
    const mlib_u8 *src,
    mlib_u8 *dst,
    mlib_s32 dsize)
{
/* pointer for pixel in source */
	mlib_s16 *sp;

/* pointer for pixel in destination */
	mlib_u8 *dp, *dl;

/* intermediate value */
	mlib_s32 bitdata;

	__m128i x_sd0, x_sd1;
	__m128i x_dd0, x_dd1, x_dd2, x_dd3, x_dd4, x_dd5;
	__m128i x_dd6, x_dd7, x_dd8, x_dd9, x_dd10;
	mlib_s32 dd, s0;
	mlib_s32 i;

	const __m128i x_zero = _mm_setzero_si128();
	const __m128i x_mask = _mm_set1_epi16(0xFF);

	sp = (mlib_s16 *)src;
	dp = dl = dst;

/* 16 pixels */
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < (dsize - 15); i += 16) {
		x_sd0 = _mm_loadu_si128((__m128i *)sp);
		sp += 8;
		x_dd0 = _mm_cmpgt_epi16(x_sd0, x_zero);
		x_dd1 = _mm_shuffle_epi32(x_dd0, 0x1B);
		x_dd2 = _mm_shufflehi_epi16(x_dd1, 0xB1);
		x_dd3 = _mm_shufflelo_epi16(x_dd2, 0xB1);
		x_dd4 = _mm_and_si128(x_dd3, x_mask);

		x_sd1 = _mm_loadu_si128((__m128i *)sp);
		x_dd5 = _mm_cmpgt_epi16(x_sd1, x_zero);
		x_dd6 = _mm_shuffle_epi32(x_dd5, 0x1B);
		x_dd7 = _mm_shufflehi_epi16(x_dd6, 0xB1);
		x_dd8 = _mm_shufflelo_epi16(x_dd7, 0xB1);
		x_dd9 = _mm_and_si128(x_dd8, x_mask);

		x_dd10 = _mm_packus_epi16(x_dd4, x_dd9);
		dd = _mm_movemask_epi8(x_dd10);
		*((mlib_u16 *)dp) = (mlib_u16)dd;
		dp += 2;
		sp += 8;
	}

/* 8 pixels */
	if (i < (dsize - 7)) {
		x_sd0 = _mm_loadu_si128((__m128i *)sp);
		sp += 8;
		x_dd0 = _mm_cmpgt_epi16(x_sd0, x_zero);

		x_dd1 = _mm_shuffle_epi32(x_dd0, 0x1B);
		x_dd2 = _mm_shufflehi_epi16(x_dd1, 0xB1);
		x_dd3 = _mm_shufflelo_epi16(x_dd2, 0xB1);

		x_dd4 = _mm_and_si128(x_dd3, x_mask);
		x_dd5 = _mm_packus_epi16(x_dd4, x_dd4);
		dd = _mm_movemask_epi8(x_dd5);
		*((mlib_u8 *)dp) = (mlib_u8)dd;

		dp += 1;
		i += 8;
	}

	if (i < dsize) {
		mlib_s32 mask = 0xFF00 >> (dsize & 7);
		mlib_u32 and = 0x80000000;
		bitdata = 0;

		for (; i < dsize; i++) {
			s0 = -sp[0];
			bitdata |= (s0 & and);
			and >>= 1;
			sp++;
		}
		dl[dsize / 8] = (bitdata >> 24) |
		    (dl[dsize / 8] & ~mask);
	}
}

/* *********************************************************** */

void mlib_s_ImageDataTypeConvert_S16_BIT(
    const mlib_u8 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize)
{
	mlib_u8 *sp, *sl;
	mlib_u8 *dp, *dl;
	mlib_s32 j;

	sp = sl = (mlib_u8 *)src;
	dp = dl = dst;

	for (j = 0; j < ysize; j++) {

		mlib_s_ImageDataTypeConvert_S16_BIT_D1(sp, dp, xsize);

		sp = sl = (mlib_u8 *)((mlib_u8 *)sl + slb);
		dp = dl = dl + dlb;
	}
}

/* *********************************************************** */

/* S16 -> U8 */
void mlib_s_ImageDataTypeConvert_S16_U8_A8D1X8(
    const mlib_s16 *src,
    mlib_u8 *dst,
    mlib_s32 dsize)
{
/* pointer for pixel in source */
	mlib_s16 *sp;

/* pointer for pixel in destination */
	mlib_u8 *dp;

	__m128i x_sd0, x_sd1;
	__m128i x_dd0;
	mlib_s32 sd, dd;
	mlib_s32 i;

	sp = (void *)src;
	dp = dst;

/* 16 pixels */
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < (dsize - 15); i += 16) {
		x_sd0 = _mm_load_si128((__m128i *)sp);
		sp += 8;
		x_sd1 = _mm_load_si128((__m128i *)sp);
		sp += 8;
		x_dd0 = _mm_packus_epi16(x_sd0, x_sd1);
		_mm_store_si128((__m128i *)dp, x_dd0);
		dp += 16;
	}

/* 8 pixels */
	if (i < (dsize - 7)) {
		x_sd0 = _mm_load_si128((__m128i *)sp);
		sp += 8;
		x_dd0 = _mm_packus_epi16(x_sd0, x_sd0);
		_mm_storel_epi64((__m128i *)dp, x_dd0);
		dp += 8;
		i += 8;
	}

/* 4 pixels */
	if (i < (dsize - 3)) {
		x_sd0 = _mm_loadl_epi64((__m128i *)sp);
		sp += 4;
		x_dd0 = _mm_packus_epi16(x_sd0, x_sd0);
		dd = _mm_cvtsi128_si32(x_dd0);
		*((mlib_s32 *)dp) = dd;
		dp += 4;
		i += 4;
	}

	for (; i < dsize; i++) {
		sd = (*sp++);
		MLIB_CLAMP_U8(*dp, sd);
		dp++;
	}
}

/* *********************************************************** */

void mlib_s_ImageDataTypeConvert_S16_U8_A8D2X8(
    const mlib_s16 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize)
{
	mlib_s16 *sp, *sl;
	mlib_u8 *dp, *dl;
	mlib_s32 j;

	sp = sl = (void *)src;
	dp = dl = dst;

	for (j = 0; j < ysize; j++) {

		mlib_s_ImageDataTypeConvert_S16_U8_A8D1X8(sp, dp, xsize);

		sp = sl = (mlib_s16 *)((mlib_u8 *)sl + slb);
		dp = dl = dl + dlb;
	}
}

/* *********************************************************** */

void mlib_s_ImageDataTypeConvert_S16_U8_D1(
    const mlib_s16 *src,
    mlib_u8 *dst,
    mlib_s32 dsize)
{
/* pointer for pixel in source */
	mlib_s16 *sp;

/* pointer for pixel in destination */
	mlib_u8 *dp;

	__m128i x_sd0, x_sd1;
	__m128i x_dd0;
	mlib_s32 sd, dd;
	mlib_s32 i;

	sp = (void *)src;
	dp = dst;

/* 16 pixels */
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < (dsize - 15); i += 16) {
		x_sd0 = _mm_loadu_si128((__m128i *)sp);
		sp += 8;
		x_sd1 = _mm_loadu_si128((__m128i *)sp);
		sp += 8;
		x_dd0 = _mm_packus_epi16(x_sd0, x_sd1);
		_mm_storeu_si128((__m128i *)dp, x_dd0);
		dp += 16;
	}

/* 8 pixels */
	if (i < (dsize - 7)) {
		x_sd0 = _mm_loadu_si128((__m128i *)sp);
		sp += 8;
		x_dd0 = _mm_packus_epi16(x_sd0, x_sd0);
		_mm_storel_epi64((__m128i *)dp, x_dd0);
		dp += 8;
		i += 8;
	}

/* 4 pixels */
	if (i < (dsize - 3)) {
		x_sd0 = _mm_loadl_epi64((__m128i *)sp);
		sp += 4;
		x_dd0 = _mm_packus_epi16(x_sd0, x_sd0);
		dd = _mm_cvtsi128_si32(x_dd0);
		*((mlib_s32 *)dp) = dd;
		dp += 4;
		i += 4;
	}

	for (; i < dsize; i++) {
		sd = (*sp++);
		MLIB_CLAMP_U8(*dp, sd);
		dp++;
	}
}

/* *********************************************************** */

void mlib_s_ImageDataTypeConvert_S16_U8(
    const mlib_s16 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize)
{
	mlib_s16 *sp, *sl;
	mlib_u8 *dp, *dl;
	mlib_s32 j;

	sp = sl = (void *)src;
	dp = dl = dst;

	for (j = 0; j < ysize; j++) {

		mlib_s_ImageDataTypeConvert_S16_U8_D1(sp, dp, xsize);

		sp = sl = (mlib_s16 *)((mlib_u8 *)sl + slb);
		dp = dl = dl + dlb;
	}
}

/* *********************************************************** */

/* S16 -> U16 */

void mlib_s_ImageDataTypeConvert_S16_U16_A8D1X8(
    const mlib_s16 *src,
    mlib_u16 *dst,
    mlib_s32 dsize)
{
/* pointer for pixel in source */
	mlib_s16 *sp;

/* pointer for pixel in destination */
	mlib_u16 *dp;

	__m128i x_sd0, x_mask;
	__m128i x_dd0;
	mlib_s32 sd;
	mlib_s32 i;

	const __m128i x_zero = _mm_setzero_si128();

	sp = (void *)src;
	dp = dst;

/* 8 pixels */
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < (dsize - 7); i += 8) {
		x_sd0 = _mm_load_si128((__m128i *)sp);
		x_mask = _mm_cmpgt_epi16(x_sd0, x_zero);
		x_dd0 = _mm_and_si128(x_sd0, x_mask);
		_mm_store_si128((__m128i *)dp, x_dd0);
		dp += 8;
		sp += 8;
	}

/* 4 pixels */
	if (i < (dsize - 3)) {
		x_sd0 = _mm_loadl_epi64((__m128i *)sp);
		x_mask = _mm_cmpgt_epi16(x_sd0, x_zero);
		x_dd0 = _mm_and_si128(x_sd0, x_mask);
		_mm_storel_epi64((__m128i *)dp, x_dd0);
		dp += 4;
		sp += 4;
		i += 4;
	}

/* 1 pixels */
	for (; i < dsize; i++) {
		sd = (*sp++);
		(*dp++) = sd & ~(sd >> 31);
	}
}

/* *********************************************************** */

void mlib_s_ImageDataTypeConvert_S16_U16_A8D2X8(
    const mlib_s16 *src,
    mlib_s32 slb,
    mlib_u16 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize)
{
	mlib_s16 *sp, *sl;
	mlib_u16 *dp, *dl;
	mlib_s32 j;

	sp = sl = (void *)src;
	dp = dl = dst;

	for (j = 0; j < ysize; j++) {

		mlib_s_ImageDataTypeConvert_S16_U16_A8D1X8(sp, dp, xsize);

		sp = sl = (mlib_s16 *)((mlib_u8 *)sl + slb);
		dp = dl = (mlib_u16 *)((mlib_u8 *)dl + dlb);
	}
}

/* *********************************************************** */

void mlib_s_ImageDataTypeConvert_S16_U16_D1(
    const mlib_s16 *src,
    mlib_u16 *dst,
    mlib_s32 dsize)
{
/* pointer for pixel in source */
	mlib_s16 *sp;

/* pointer for pixel in destination */
	mlib_u16 *dp;

	__m128i x_sd0, x_mask;
	__m128i x_dd0;
	mlib_s32 sd;
	mlib_s32 i;

	const __m128i x_zero = _mm_setzero_si128();

	sp = (void *)src;
	dp = dst;

/* 8 pixels */
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < (dsize - 7); i += 8) {
		x_sd0 = _mm_loadu_si128((__m128i *)sp);
		x_mask = _mm_cmpgt_epi16(x_sd0, x_zero);
		x_dd0 = _mm_and_si128(x_sd0, x_mask);
		_mm_storeu_si128((__m128i *)dp, x_dd0);
		dp += 8;
		sp += 8;
	}

/* 4 pixels */
	if (i < (dsize - 3)) {
		x_sd0 = _mm_loadl_epi64((__m128i *)sp);
		x_mask = _mm_cmpgt_epi16(x_sd0, x_zero);
		x_dd0 = _mm_and_si128(x_sd0, x_mask);
		_mm_storel_epi64((__m128i *)dp, x_dd0);
		dp += 4;
		sp += 4;
		i += 4;
	}

/* 1 pixels */
	for (; i < dsize; i++) {
		sd = (*sp++);
		(*dp++) = sd & ~(sd >> 31);
	}
}

/* *********************************************************** */

void mlib_s_ImageDataTypeConvert_S16_U16(
    const mlib_s16 *src,
    mlib_s32 slb,
    mlib_u16 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize)
{
	mlib_s16 *sp, *sl;
	mlib_u16 *dp, *dl;
	mlib_s32 j;

	sp = sl = (void *)src;
	dp = dl = dst;

	for (j = 0; j < ysize; j++) {

		mlib_s_ImageDataTypeConvert_S16_U16_D1(sp, dp, xsize);

		sp = sl = (mlib_s16 *)((mlib_u8 *)sl + slb);
		dp = dl = (mlib_u16 *)((mlib_u8 *)dl + dlb);
	}
}

/* *********************************************************** */

/* S16 -> S32 */
void mlib_s_ImageDataTypeConvert_S16_S32_A8D1X8(
    const mlib_s16 *src,
    mlib_s32 *dst,
    mlib_s32 dsize)
{
/* pointer for pixel in source */
	mlib_s16 *sp;

/* pointer for pixel in destination */
	mlib_s32 *dp;

/* pointer for pixel in source */
	mlib_s32 *sp2;

	__m128i x_sd0, x_sd1;
	__m128i x_dd0, x_dd1;
	mlib_s32 val0;
	mlib_s32 i;

	sp = (void *)src;
	dp = dst;

/* 8 pixels */
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < (dsize - 7); i += 8) {
		x_sd0 = _mm_load_si128((__m128i *)sp);
		x_sd1 = _mm_srai_epi16(x_sd0, 16);
		x_dd0 = _mm_unpacklo_epi16(x_sd0, x_sd1);
		x_dd1 = _mm_unpackhi_epi16(x_sd0, x_sd1);
		_mm_store_si128((__m128i *)dp, x_dd0);
		dp += 4;
		_mm_store_si128((__m128i *)dp, x_dd1);
		dp += 4;
		sp += 8;
	}

/* 4 pixels */
	if (i < (dsize - 3)) {
		x_sd0 = _mm_loadl_epi64((__m128i *)sp);
		x_sd1 = _mm_srai_epi16(x_sd0, 16);
		x_dd0 = _mm_unpacklo_epi16(x_sd0, x_sd1);
		_mm_store_si128((__m128i *)dp, x_dd0);
		dp += 4;
		sp += 4;
		i += 4;
	}

/* 2 pixels */
	sp2 = (mlib_s32 *)sp;
	if (i < (dsize - 1)) {
		val0 = (*sp2++);
		dp[0] = (val0 << 16) >> 16;
		dp[1] = (val0 >> 16);
		dp += 2;
		i += 2;
	}

/* 1 pixels */
	sp = (mlib_s16 *)sp2;
	if (i < dsize) {
		(*dp++) = (*sp++);
	}
}

/* *********************************************************** */

void mlib_s_ImageDataTypeConvert_S16_S32_A8D2X8(
    const mlib_s16 *src,
    mlib_s32 slb,
    mlib_s32 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize)
{
	mlib_s16 *sp, *sl;
	mlib_s32 *dp, *dl;
	mlib_s32 j;

	sp = sl = (void *)src;
	dp = dl = dst;

	for (j = 0; j < ysize; j++) {

		mlib_s_ImageDataTypeConvert_S16_S32_A8D1X8(sp, dp, xsize);

		sp = sl = (mlib_s16 *)((mlib_u8 *)sl + slb);
		dp = dl = (mlib_s32 *)((mlib_u8 *)dl + dlb);
	}
}

/* *********************************************************** */

void mlib_s_ImageDataTypeConvert_S16_S32_D1(
    const mlib_s16 *src,
    mlib_s32 *dst,
    mlib_s32 dsize)
{
/* pointer for pixel in source */
	mlib_s16 *sp;

/* pointer for pixel in destination */
	mlib_s32 *dp;

/* pointer for pixel in source */
	mlib_s32 *sp2;

	__m128i x_sd0, x_sd1;
	__m128i x_dd0, x_dd1;
	mlib_s32 val0;
	mlib_s32 i;

	sp = (void *)src;
	dp = dst;

/* 8 pixels */
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < (dsize - 7); i += 8) {
		x_sd0 = _mm_loadu_si128((__m128i *)sp);
		x_sd1 = _mm_srai_epi16(x_sd0, 16);
		x_dd0 = _mm_unpacklo_epi16(x_sd0, x_sd1);
		x_dd1 = _mm_unpackhi_epi16(x_sd0, x_sd1);
		_mm_storeu_si128((__m128i *)dp, x_dd0);
		dp += 4;
		_mm_storeu_si128((__m128i *)dp, x_dd1);
		dp += 4;
		sp += 8;
	}

/* 4 pixels */
	if (i < (dsize - 3)) {
		x_sd0 = _mm_loadl_epi64((__m128i *)sp);
		x_sd1 = _mm_srai_epi16(x_sd0, 16);
		x_dd0 = _mm_unpacklo_epi16(x_sd0, x_sd1);
		_mm_storeu_si128((__m128i *)dp, x_dd0);
		dp += 4;
		sp += 4;
		i += 4;
	}

/* 2 pixels */
	sp2 = (mlib_s32 *)sp;
	if (i < (dsize - 1)) {
		val0 = (*sp2++);
		dp[0] = (val0 << 16) >> 16;
		dp[1] = (val0 >> 16);
		dp += 2;
		i += 2;
	}

/* 1 pixels */
	sp = (mlib_s16 *)sp2;
	if (i < dsize) {
		(*dp++) = (*sp++);
	}
}

/* *********************************************************** */

void mlib_s_ImageDataTypeConvert_S16_S32(
    const mlib_s16 *src,
    mlib_s32 slb,
    mlib_s32 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize)
{
	mlib_s16 *sp, *sl;
	mlib_s32 *dp, *dl;
	mlib_s32 j;

	sp = sl = (void *)src;
	dp = dl = dst;

	for (j = 0; j < ysize; j++) {

		mlib_s_ImageDataTypeConvert_S16_S32_D1(sp, dp, xsize);

		sp = sl = (mlib_s16 *)((mlib_u8 *)sl + slb);
		dp = dl = (mlib_s32 *)((mlib_u8 *)dl + dlb);
	}
}

/* *********************************************************** */

/* S16 -> F32 */
void mlib_s_ImageDataTypeConvert_S16_F32_A8D1X8(
    const mlib_s16 *src,
    mlib_f32 *dst,
    mlib_s32 dsize)
{
/* pointer for pixel in source */
	mlib_s16 *sp;

/* pointer for pixel in destination */
	mlib_f32 *dp;

/* pointer for pixel in source */
	mlib_s32 *sp2;

	__m128i x_sd0, x_sd1;
	__m128i x_dd0, x_dd1;
	__m128 x_df0, x_df1;
	mlib_s32 val0;
	mlib_s32 i;

	sp = (void *)src;
	dp = dst;

/* 8 pixels */
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < (dsize - 7); i += 8) {
		x_sd0 = _mm_load_si128((__m128i *)sp);
		x_sd1 = _mm_srai_epi16(x_sd0, 16);
		x_dd0 = _mm_unpacklo_epi16(x_sd0, x_sd1);
		x_dd1 = _mm_unpackhi_epi16(x_sd0, x_sd1);
		x_df0 = _mm_cvtepi32_ps(x_dd0);
		x_df1 = _mm_cvtepi32_ps(x_dd1);
		_mm_store_ps(dp, x_df0);
		dp += 4;
		_mm_store_ps(dp, x_df1);
		dp += 4;
		sp += 8;
	}

/* 4 pixels */
	if (i < (dsize - 3)) {
		x_sd0 = _mm_loadl_epi64((__m128i *)sp);
		x_sd1 = _mm_srai_epi16(x_sd0, 16);
		x_dd0 = _mm_unpacklo_epi16(x_sd0, x_sd1);
		x_df0 = _mm_cvtepi32_ps(x_dd0);
		_mm_store_ps(dp, x_df0);
		dp += 4;
		sp += 4;
		i += 4;
	}

/* 2 pixels */
	sp2 = (mlib_s32 *)sp;
	if (i < (dsize - 1)) {
		val0 = (*sp2++);
		dp[0] = (val0 << 16) >> 16;
		dp[1] = (val0 >> 16);
		dp += 2;
		i += 2;
	}

/* 1 pixels */
	sp = (mlib_s16 *)sp2;
	if (i < dsize) {
		(*dp++) = (*sp++);
	}
}

/* *********************************************************** */

void mlib_s_ImageDataTypeConvert_S16_F32_A8D2X8(
    const mlib_s16 *src,
    mlib_s32 slb,
    mlib_f32 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize)
{
	mlib_s16 *sp, *sl;
	mlib_f32 *dp, *dl;
	mlib_s32 j;

	sp = sl = (void *)src;
	dp = dl = dst;

	for (j = 0; j < ysize; j++) {

		mlib_s_ImageDataTypeConvert_S16_F32_A8D1X8(sp, dp, xsize);

		sp = sl = (mlib_s16 *)((mlib_u8 *)sl + slb);
		dp = dl = (mlib_f32 *)((mlib_u8 *)dl + dlb);
	}
}

/* *********************************************************** */

void mlib_s_ImageDataTypeConvert_S16_F32_D1(
    const mlib_s16 *src,
    mlib_f32 *dst,
    mlib_s32 dsize)
{
/* pointer for pixel in source */
	mlib_s16 *sp;

/* pointer for pixel in destination */
	mlib_f32 *dp;

/* pointer for pixel in source */
	mlib_s32 *sp2;

	__m128i x_sd0, x_sd1;
	__m128i x_dd0, x_dd1;
	__m128 x_df0, x_df1;
	mlib_s32 val0;
	mlib_s32 i;

	sp = (void *)src;
	dp = dst;

/* 8 pixels */
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < (dsize - 7); i += 8) {
		x_sd0 = _mm_loadu_si128((__m128i *)sp);
		x_sd1 = _mm_srai_epi16(x_sd0, 16);
		x_dd0 = _mm_unpacklo_epi16(x_sd0, x_sd1);
		x_dd1 = _mm_unpackhi_epi16(x_sd0, x_sd1);
		x_df0 = _mm_cvtepi32_ps(x_dd0);
		x_df1 = _mm_cvtepi32_ps(x_dd1);
		_mm_storeu_ps(dp, x_df0);
		dp += 4;
		_mm_storeu_ps(dp, x_df1);
		dp += 4;
		sp += 8;
	}

/* 4 pixels */
	if (i < (dsize - 3)) {
		x_sd0 = _mm_loadl_epi64((__m128i *)sp);
		x_sd1 = _mm_srai_epi16(x_sd0, 16);
		x_dd0 = _mm_unpacklo_epi16(x_sd0, x_sd1);
		x_df0 = _mm_cvtepi32_ps(x_dd0);
		_mm_storeu_ps(dp, x_df0);
		dp += 4;
		sp += 4;
		i += 4;
	}

/* 2 pixels */
	sp2 = (mlib_s32 *)sp;
	if (i < (dsize - 1)) {
		val0 = (*sp2++);
		dp[0] = (val0 << 16) >> 16;
		dp[1] = (val0 >> 16);
		dp += 2;
		i += 2;
	}

/* 1 pixels */
	sp = (mlib_s16 *)sp2;
	if (i < dsize) {
		(*dp++) = (*sp++);
	}
}

/* *********************************************************** */

void mlib_s_ImageDataTypeConvert_S16_F32(
    const mlib_s16 *src,
    mlib_s32 slb,
    mlib_f32 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize)
{
	mlib_s16 *sp, *sl;
	mlib_f32 *dp, *dl;
	mlib_s32 j;

	sp = sl = (void *)src;
	dp = dl = dst;

	for (j = 0; j < ysize; j++) {

		mlib_s_ImageDataTypeConvert_S16_F32_D1(sp, dp, xsize);

		sp = sl = (mlib_s16 *)((mlib_u8 *)sl + slb);
		dp = dl = (mlib_f32 *)((mlib_u8 *)dl + dlb);
	}
}

/* *********************************************************** */

/* S16 -> D64 */
void mlib_s_ImageDataTypeConvert_S16_D64_A8D1X8(
    const mlib_s16 *src,
    mlib_d64 *dst,
    mlib_s32 dsize)
{
/* pointer for pixel in source */
	mlib_s16 *sp;

/* pointer for pixel in destination */
	mlib_d64 *dp;

	__m128i x_sd0, x_sd1, x_sd2, x_sd3;
	__m128i x_sd4, x_sd5, x_sd6, x_sd7;
	__m128d x_dd0, x_dd1, x_dd2, x_dd3;
	__m128d x_dd4, x_dd5, x_dd6, x_dd7;
	mlib_s32 i;
	const __m128i x_zero = _mm_setzero_si128();

	sp = (void *)src;
	dp = dst;

/* 8 pixels */
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < (dsize - 7); i += 8) {
		x_sd0 = _mm_load_si128((__m128i *)sp);
		x_sd1 = _mm_srai_epi16(x_sd0, 16);
		x_sd2 = _mm_unpacklo_epi16(x_sd0, x_sd1);
		x_sd3 = _mm_unpackhi_epi16(x_sd0, x_sd1);

		x_sd4 = _mm_unpacklo_epi64(x_sd2, x_zero);
		x_sd5 = _mm_unpackhi_epi64(x_sd2, x_zero);
		x_dd2 = _mm_cvtepi32_pd(x_sd4);
		x_dd3 = _mm_cvtepi32_pd(x_sd5);

		x_sd6 = _mm_unpacklo_epi64(x_sd3, x_zero);
		x_sd7 = _mm_unpackhi_epi64(x_sd3, x_zero);
		x_dd6 = _mm_cvtepi32_pd(x_sd6);
		x_dd7 = _mm_cvtepi32_pd(x_sd7);

		_mm_store_pd(dp, x_dd2);
		dp += 2;
		_mm_store_pd(dp, x_dd3);
		dp += 2;
		_mm_store_pd(dp, x_dd6);
		dp += 2;
		_mm_store_pd(dp, x_dd7);
		dp += 2;
		sp += 8;
	}

/* 4 pixels */
	if (i < (dsize - 3)) {
		x_sd0 = _mm_loadl_epi64((__m128i *)sp);
		x_sd1 = _mm_srai_epi16(x_sd0, 16);
		x_sd2 = _mm_unpacklo_epi16(x_sd0, x_sd1);

		x_sd3 = _mm_unpacklo_epi64(x_sd2, x_zero);
		x_sd4 = _mm_unpackhi_epi64(x_sd2, x_zero);
		x_dd2 = _mm_cvtepi32_pd(x_sd3);
		x_dd3 = _mm_cvtepi32_pd(x_sd4);

		_mm_store_pd(dp, x_dd2);
		dp += 2;
		_mm_store_pd(dp, x_dd3);
		dp += 2;
		sp += 4;
		i += 4;
	}

/* 1 pixels */
	for (; i < dsize; i++) {
		(*dp++) = (*sp++);
	}
}

/* *********************************************************** */

void mlib_s_ImageDataTypeConvert_S16_D64_A8D2X8(
    const mlib_s16 *src,
    mlib_s32 slb,
    mlib_d64 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize)
{
	mlib_s16 *sp, *sl;
	mlib_d64 *dp, *dl;
	mlib_s32 j;

	sp = sl = (void *)src;
	dp = dl = dst;

	for (j = 0; j < ysize; j++) {

		mlib_s_ImageDataTypeConvert_S16_D64_A8D1X8(sp, dp, xsize);

		sp = sl = (mlib_s16 *)((mlib_u8 *)sl + slb);
		dp = dl = (mlib_d64 *)((mlib_u8 *)dl + dlb);
	}
}

/* *********************************************************** */

void mlib_s_ImageDataTypeConvert_S16_D64_D1(
    const mlib_s16 *src,
    mlib_d64 *dst,
    mlib_s32 dsize)
{
/* pointer for pixel in source */
	mlib_s16 *sp;

/* pointer for pixel in destination */
	mlib_d64 *dp;

	__m128i x_sd0, x_sd1, x_sd2, x_sd3;
	__m128i x_sd4, x_sd5, x_sd6, x_sd7;
	__m128d x_dd0, x_dd1, x_dd2, x_dd3;
	__m128d x_dd4, x_dd5, x_dd6, x_dd7;
	mlib_s32 i;
	const __m128i x_zero = _mm_setzero_si128();

	sp = (void *)src;
	dp = dst;

/* 8 pixels */
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < (dsize - 7); i += 8) {
		x_sd0 = _mm_loadu_si128((__m128i *)sp);
		x_sd1 = _mm_srai_epi16(x_sd0, 16);
		x_sd2 = _mm_unpacklo_epi16(x_sd0, x_sd1);
		x_sd3 = _mm_unpackhi_epi16(x_sd0, x_sd1);

		x_sd4 = _mm_unpacklo_epi64(x_sd2, x_zero);
		x_sd5 = _mm_unpackhi_epi64(x_sd2, x_zero);
		x_dd2 = _mm_cvtepi32_pd(x_sd4);
		x_dd3 = _mm_cvtepi32_pd(x_sd5);

		x_sd6 = _mm_unpacklo_epi64(x_sd3, x_zero);
		x_sd7 = _mm_unpackhi_epi64(x_sd3, x_zero);
		x_dd6 = _mm_cvtepi32_pd(x_sd6);
		x_dd7 = _mm_cvtepi32_pd(x_sd7);

		_mm_storeu_pd(dp, x_dd2);
		dp += 2;
		_mm_storeu_pd(dp, x_dd3);
		dp += 2;
		_mm_storeu_pd(dp, x_dd6);
		dp += 2;
		_mm_storeu_pd(dp, x_dd7);
		dp += 2;
		sp += 8;
	}

/* 4 pixels */
	if (i < (dsize - 3)) {
		x_sd0 = _mm_loadl_epi64((__m128i *)sp);
		x_sd1 = _mm_srai_epi16(x_sd0, 16);
		x_sd2 = _mm_unpacklo_epi16(x_sd0, x_sd1);

		x_sd3 = _mm_unpacklo_epi64(x_sd2, x_zero);
		x_sd4 = _mm_unpackhi_epi64(x_sd2, x_zero);
		x_dd2 = _mm_cvtepi32_pd(x_sd3);
		x_dd3 = _mm_cvtepi32_pd(x_sd4);

		_mm_storeu_pd(dp, x_dd2);
		dp += 2;
		_mm_storeu_pd(dp, x_dd3);
		dp += 2;
		sp += 4;
		i += 4;
	}

/* 1 pixels */
	for (; i < dsize; i++) {
		(*dp++) = (*sp++);
	}
}

/* *********************************************************** */

void mlib_s_ImageDataTypeConvert_S16_D64(
    const mlib_s16 *src,
    mlib_s32 slb,
    mlib_d64 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize)
{
	mlib_s16 *sp, *sl;
	mlib_d64 *dp, *dl;
	mlib_s32 j;

	sp = sl = (void *)src;
	dp = dl = dst;

	for (j = 0; j < ysize; j++) {

		mlib_s_ImageDataTypeConvert_S16_D64_D1(sp, dp, xsize);

		sp = sl = (mlib_s16 *)((mlib_u8 *)sl + slb);
		dp = dl = (mlib_d64 *)((mlib_u8 *)dl + dlb);
	}
}

/* *********************************************************** */

/* S32 -> BIT */
void mlib_s_ImageDataTypeConvert_S32_BIT_A8D1X8(
    const mlib_u8 *src,
    mlib_u8 *dst,
    mlib_s32 dsize)
{
/* pointer for pixel in source */
	mlib_s16 *sp;

/* pointer for pixel in destination */
	mlib_u8 *dp, *dl;

/* intermediate value */
	mlib_s32 bitdata;

	__m128i x_sd0, x_sd1, x_sd2, x_sd3, x_sd4, x_sd5;
	__m128i x_dd0, x_dd1, x_dd2, x_dd3;
	__m128i x_dd4, x_dd5, x_dd6, x_dd7;
	mlib_s32 dd, s0;
	mlib_s32 i;

	const __m128i x_zero = _mm_setzero_si128();
	const __m128i x_mask = _mm_set1_epi16(0xFF);

	sp = (mlib_s16 *)src;
	dp = dl = dst;

/* 16 pixels */
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < (dsize - 15); i += 16) {
		x_sd0 = _mm_load_si128((__m128i *)sp);
		sp += 8;
		x_dd0 = _mm_cmpgt_epi32(x_sd0, x_zero);
		x_dd4 = _mm_shuffle_epi32(x_dd0, 0x1B);

		x_sd1 = _mm_load_si128((__m128i *)sp);
		sp += 8;
		x_dd1 = _mm_cmpgt_epi32(x_sd1, x_zero);
		x_dd5 = _mm_shuffle_epi32(x_dd1, 0x1B);

		x_sd2 = _mm_load_si128((__m128i *)sp);
		sp += 8;
		x_dd2 = _mm_cmpgt_epi32(x_sd2, x_zero);
		x_dd6 = _mm_shuffle_epi32(x_dd2, 0x1B);

		x_sd3 = _mm_load_si128((__m128i *)sp);
		sp += 8;
		x_dd3 = _mm_cmpgt_epi32(x_sd3, x_zero);
		x_dd7 = _mm_shuffle_epi32(x_dd3, 0x1B);

		x_sd1 = _mm_packs_epi32(x_dd5, x_dd4);
		x_sd2 = _mm_packs_epi32(x_dd7, x_dd6);

		x_sd3 = _mm_and_si128(x_sd1, x_mask);
		x_sd4 = _mm_and_si128(x_sd2, x_mask);

		x_sd5 = _mm_packus_epi16(x_sd3, x_sd4);
		dd = _mm_movemask_epi8(x_sd5);
		*((mlib_u16 *)dp) = (mlib_u16)dd;
		dp += 2;
	}

	if (i < (dsize - 7)) {
		x_sd0 = _mm_load_si128((__m128i *)sp);
		sp += 8;
		x_dd0 = _mm_cmpgt_epi32(x_sd0, x_zero);

		x_dd2 = _mm_shuffle_epi32(x_dd0, 0x1B);

		x_sd1 = _mm_load_si128((__m128i *)sp);
		sp += 8;
		x_dd1 = _mm_cmpgt_epi32(x_sd1, x_zero);
		x_dd3 = _mm_shuffle_epi32(x_dd1, 0x1B);

		x_dd4 = _mm_packs_epi32(x_dd3, x_dd2);
		x_dd5 = _mm_and_si128(x_dd4, x_mask);
		x_dd6 = _mm_packus_epi16(x_dd5, x_zero);
		dd = _mm_movemask_epi8(x_dd6);
		*((mlib_u8 *)dp) = (mlib_u8)dd;

		dp += 1;
		i += 8;
	}

	if (i < dsize) {
		mlib_s32 mask = 0xFF00 >> (dsize & 7);
		mlib_u32 and = 0x80000000;
		bitdata = 0;

		for (; i < dsize; i++) {
			s0 = -(sp[1] | ((mlib_u16 *)sp)[0]);
			bitdata |= (s0 & and);
			and >>= 1;
			sp += 2;
		}
		dl[dsize / 8] = (bitdata >> 24) |
		    (dl[dsize / 8] & ~mask);
	}
}

/* *********************************************************** */

void mlib_s_ImageDataTypeConvert_S32_BIT_A8D2X8(
    const mlib_u8 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize)
{
	mlib_u8 *sp, *sl;
	mlib_u8 *dp, *dl;
	mlib_s32 j;

	sp = sl = (mlib_u8 *)src;
	dp = dl = dst;

	for (j = 0; j < ysize; j++) {

		mlib_s_ImageDataTypeConvert_S32_BIT_A8D1X8(sp, dp, xsize);

		sp = sl = (mlib_u8 *)((mlib_u8 *)sl + slb);
		dp = dl = dl + dlb;
	}
}

/* *********************************************************** */

void mlib_s_ImageDataTypeConvert_S32_BIT_D1(
    const mlib_u8 *src,
    mlib_u8 *dst,
    mlib_s32 dsize)
{
/* pointer for pixel in source */
	mlib_s16 *sp;

/* pointer for pixel in destination */
	mlib_u8 *dp, *dl;

/* intermediate value */
	mlib_s32 bitdata;

	__m128i x_sd0, x_sd1, x_sd2, x_sd3, x_sd4, x_sd5;
	__m128i x_dd0, x_dd1, x_dd2, x_dd3;
	__m128i x_dd4, x_dd5, x_dd6, x_dd7;
	mlib_s32 dd, s0;
	mlib_s32 i;

	const __m128i x_zero = _mm_setzero_si128();
	const __m128i x_mask = _mm_set1_epi16(0xFF);

	sp = (mlib_s16 *)src;
	dp = dl = dst;

/* 16 pixels */
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < (dsize - 15); i += 16) {
		x_sd0 = _mm_loadu_si128((__m128i *)sp);
		sp += 8;
		x_dd0 = _mm_cmpgt_epi32(x_sd0, x_zero);
		x_dd4 = _mm_shuffle_epi32(x_dd0, 0x1B);

		x_sd1 = _mm_loadu_si128((__m128i *)sp);
		sp += 8;
		x_dd1 = _mm_cmpgt_epi32(x_sd1, x_zero);
		x_dd5 = _mm_shuffle_epi32(x_dd1, 0x1B);

		x_sd2 = _mm_loadu_si128((__m128i *)sp);
		sp += 8;
		x_dd2 = _mm_cmpgt_epi32(x_sd2, x_zero);
		x_dd6 = _mm_shuffle_epi32(x_dd2, 0x1B);

		x_sd3 = _mm_loadu_si128((__m128i *)sp);
		sp += 8;
		x_dd3 = _mm_cmpgt_epi32(x_sd3, x_zero);
		x_dd7 = _mm_shuffle_epi32(x_dd3, 0x1B);

		x_sd1 = _mm_packs_epi32(x_dd5, x_dd4);
		x_sd2 = _mm_packs_epi32(x_dd7, x_dd6);

		x_sd3 = _mm_and_si128(x_sd1, x_mask);
		x_sd4 = _mm_and_si128(x_sd2, x_mask);

		x_sd5 = _mm_packus_epi16(x_sd3, x_sd4);
		dd = _mm_movemask_epi8(x_sd5);
		*((mlib_u16 *)dp) = (mlib_u16)dd;
		dp += 2;
	}

	if (i < (dsize - 7)) {
		x_sd0 = _mm_loadu_si128((__m128i *)sp);
		sp += 8;
		x_dd0 = _mm_cmpgt_epi32(x_sd0, x_zero);

		x_dd2 = _mm_shuffle_epi32(x_dd0, 0x1B);

		x_sd1 = _mm_loadu_si128((__m128i *)sp);
		sp += 8;
		x_dd1 = _mm_cmpgt_epi32(x_sd1, x_zero);
		x_dd3 = _mm_shuffle_epi32(x_dd1, 0x1B);

		x_dd4 = _mm_packs_epi32(x_dd3, x_dd2);
		x_dd5 = _mm_and_si128(x_dd4, x_mask);
		x_dd6 = _mm_packus_epi16(x_dd5, x_zero);
		dd = _mm_movemask_epi8(x_dd6);
		*((mlib_u8 *)dp) = (mlib_u8)dd;

		dp += 1;
		i += 8;
	}

	if (i < dsize) {
		mlib_s32 mask = 0xFF00 >> (dsize & 7);
		mlib_u32 and = 0x80000000;
		bitdata = 0;

		for (; i < dsize; i++) {
			s0 = -(sp[1] | ((mlib_u16 *)sp)[0]);
			bitdata |= (s0 & and);
			and >>= 1;
			sp += 2;
		}
		dl[dsize / 8] = (bitdata >> 24) |
		    (dl[dsize / 8] & ~mask);
	}
}

/* *********************************************************** */

void mlib_s_ImageDataTypeConvert_S32_BIT(
    const mlib_u8 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize)
{
	mlib_u8 *sp, *sl;
	mlib_u8 *dp, *dl;
	mlib_s32 j;

	sp = sl = (mlib_u8 *)src;
	dp = dl = dst;

	for (j = 0; j < ysize; j++) {

		mlib_s_ImageDataTypeConvert_S32_BIT_D1(sp, dp, xsize);

		sp = sl = (mlib_u8 *)((mlib_u8 *)sl + slb);
		dp = dl = dl + dlb;
	}
}

/* *********************************************************** */

/* S32 -> U8 */
void mlib_s_ImageDataTypeConvert_S32_U8_A8D1X8(
    const mlib_s32 *src,
    mlib_u8 *dst,
    mlib_s32 dsize)
{
/* pointer for pixel in source */
	mlib_s32 *sp;

/* pointer for pixel in destination */
	mlib_u8 *dp;

	__m128i x_sd0, x_sd1, x_sd2, x_sd3;
	__m128i x_dd0, x_dd1, x_dd2;
	mlib_s32 sd, dd;
	mlib_s32 i;

	sp = (void *)src;
	dp = dst;

/* 16 pixels */
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < (dsize - 15); i += 16) {
		x_sd0 = _mm_load_si128((__m128i *)sp);
		sp += 4;
		x_sd1 = _mm_load_si128((__m128i *)sp);
		sp += 4;
		x_sd2 = _mm_load_si128((__m128i *)sp);
		sp += 4;
		x_sd3 = _mm_load_si128((__m128i *)sp);
		sp += 4;

		x_dd0 = _mm_packs_epi32(x_sd0, x_sd1);
		x_dd1 = _mm_packs_epi32(x_sd2, x_sd3);
		x_dd2 = _mm_packus_epi16(x_dd0, x_dd1);

		_mm_store_si128((__m128i *)dp, x_dd2);
		dp += 16;
	}

/* 8 pixels */
	if (i < (dsize - 7)) {
		x_sd0 = _mm_load_si128((__m128i *)sp);
		sp += 4;
		x_sd1 = _mm_load_si128((__m128i *)sp);
		sp += 4;

		x_dd0 = _mm_packs_epi32(x_sd0, x_sd1);
		x_dd1 = _mm_packus_epi16(x_dd0, x_dd0);

		_mm_storel_epi64((__m128i *)dp, x_dd1);
		dp += 8;
		i += 8;
	}

/* 4 pixels */
	if (i < (dsize - 3)) {
		x_sd0 = _mm_load_si128((__m128i *)sp);
		sp += 4;

		x_dd0 = _mm_packs_epi32(x_sd0, x_sd0);
		x_dd1 = _mm_packus_epi16(x_dd0, x_dd0);
		dd = _mm_cvtsi128_si32(x_dd1);

		*((mlib_s32 *)dp) = dd;
		dp += 4;
		i += 4;
	}

/* 2 pixels */
	if (i < (dsize - 1)) {
		x_sd0 = _mm_loadl_epi64((__m128i *)sp);
		sp += 2;

		x_dd0 = _mm_packs_epi32(x_sd0, x_sd0);
		x_dd1 = _mm_packus_epi16(x_dd0, x_dd0);
		dd = _mm_cvtsi128_si32(x_dd1);

		*((mlib_u16 *)dp) = (mlib_u16)dd;
		dp += 2;
		i += 2;
	}

/* Pure C Code */
	for (; i < dsize; i++) {
		sd = (*sp++);
		MLIB_CLAMP_U8(*dp, sd);
		dp++;
	}
}

/* *********************************************************** */

void mlib_s_ImageDataTypeConvert_S32_U8_A8D2X8(
    const mlib_s32 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize)
{
	mlib_s32 *sp, *sl;
	mlib_u8 *dp, *dl;
	mlib_s32 j;

	sp = sl = (void *)src;
	dp = dl = dst;

	for (j = 0; j < ysize; j++) {

		mlib_s_ImageDataTypeConvert_S32_U8_A8D1X8(sp, dp, xsize);

		sp = sl = (mlib_s32 *)((mlib_u8 *)sl + slb);
		dp = dl = (mlib_u8 *)((mlib_u8 *)dl + dlb);
	}
}

/* *********************************************************** */

void mlib_s_ImageDataTypeConvert_S32_U8_D1(
    const mlib_s32 *src,
    mlib_u8 *dst,
    mlib_s32 dsize)
{
/* pointer for pixel in source */
	mlib_s32 *sp;

/* pointer for pixel in destination */
	mlib_u8 *dp;

	__m128i x_sd0, x_sd1, x_sd2, x_sd3;
	__m128i x_dd0, x_dd1, x_dd2;
	mlib_s32 sd, dd;
	mlib_s32 i;

	sp = (void *)src;
	dp = dst;

/* 16 pixels */
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < (dsize - 15); i += 16) {
		x_sd0 = _mm_loadu_si128((__m128i *)sp);
		sp += 4;
		x_sd1 = _mm_loadu_si128((__m128i *)sp);
		sp += 4;
		x_sd2 = _mm_loadu_si128((__m128i *)sp);
		sp += 4;
		x_sd3 = _mm_loadu_si128((__m128i *)sp);
		sp += 4;

		x_dd0 = _mm_packs_epi32(x_sd0, x_sd1);
		x_dd1 = _mm_packs_epi32(x_sd2, x_sd3);
		x_dd2 = _mm_packus_epi16(x_dd0, x_dd1);

		_mm_storeu_si128((__m128i *)dp, x_dd2);
		dp += 16;
	}

/* 8 pixels */
	if (i < (dsize - 7)) {
		x_sd0 = _mm_loadu_si128((__m128i *)sp);
		sp += 4;
		x_sd1 = _mm_loadu_si128((__m128i *)sp);
		sp += 4;

		x_dd0 = _mm_packs_epi32(x_sd0, x_sd1);
		x_dd1 = _mm_packus_epi16(x_dd0, x_dd0);

		_mm_storel_epi64((__m128i *)dp, x_dd1);
		dp += 8;
		i += 8;
	}

/* 4 pixels */
	if (i < (dsize - 3)) {
		x_sd0 = _mm_loadu_si128((__m128i *)sp);
		sp += 4;

		x_dd0 = _mm_packs_epi32(x_sd0, x_sd0);
		x_dd1 = _mm_packus_epi16(x_dd0, x_dd0);
		dd = _mm_cvtsi128_si32(x_dd1);

		*((mlib_s32 *)dp) = dd;
		dp += 4;
		i += 4;
	}

/* 2 pixels */
	if (i < (dsize - 1)) {
		x_sd0 = _mm_loadl_epi64((__m128i *)sp);
		sp += 2;

		x_dd0 = _mm_packs_epi32(x_sd0, x_sd0);
		x_dd1 = _mm_packus_epi16(x_dd0, x_dd0);
		dd = _mm_cvtsi128_si32(x_dd1);

		*((mlib_u16 *)dp) = (mlib_u16)dd;
		dp += 2;
		i += 2;
	}

/* Pure C Code */
	for (; i < dsize; i++) {
		sd = (*sp++);
		MLIB_CLAMP_U8(*dp, sd);
		dp++;
	}
}

/* *********************************************************** */

void mlib_s_ImageDataTypeConvert_S32_U81(
    const mlib_s32 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize)
{
	mlib_s32 *sp, *sl;
	mlib_u8 *dp, *dl;
	mlib_s32 j;

	sp = sl = (void *)src;
	dp = dl = dst;

	for (j = 0; j < ysize; j++) {

		mlib_s_ImageDataTypeConvert_S32_U8_D1(sp, dp, xsize);

		sp = sl = (mlib_s32 *)((mlib_u8 *)sl + slb);
		dp = dl = (mlib_u8 *)((mlib_u8 *)dl + dlb);
	}
}

/* *********************************************************** */

/* S32 -> U16 */
void mlib_s_ImageDataTypeConvert_S32_U16_A8D1X8(
    const mlib_s32 *src,
    mlib_u16 *dst,
    mlib_s32 dsize)
{
/* pointer for pixel in source */
	mlib_s32 *sp;

/* pointer for pixel in destination */
	mlib_u16 *dp;

	__m128i x_sd0, x_sd1, x_sd2, x_sd3;
	__m128i x_sd4, x_sd5, x_sd6, x_sd7;
	__m128i x_dd0, x_dd1, x_dd2, x_dd3, x_dd4;
	__m128i x_mask0, x_mask1;
	mlib_s32 sd;
	mlib_s32 i;

	const __m128i x_cmp = _mm_set1_epi32(65535);
	const __m128i x_zero = _mm_setzero_si128();

	sp = (void *)src;
	dp = dst;

/* 8 pixels */
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < (dsize - 7); i += 8) {
		x_sd0 = _mm_load_si128((__m128i *)sp);
		sp += 4;
		x_sd1 = _mm_load_si128((__m128i *)sp);
		sp += 4;

		x_mask0 = _mm_cmpgt_epi32(x_sd0, x_zero);
		x_sd2 = _mm_and_si128(x_sd0, x_mask0);
		x_mask1 = _mm_cmpgt_epi32(x_sd2, x_cmp);
		x_sd3 = _mm_or_si128(x_sd2, x_mask1);
		x_sd4 = _mm_and_si128(x_sd3, x_cmp);

		x_mask0 = _mm_cmpgt_epi32(x_sd1, x_zero);
		x_sd5 = _mm_and_si128(x_sd1, x_mask0);
		x_mask1 = _mm_cmpgt_epi32(x_sd5, x_cmp);
		x_sd6 = _mm_or_si128(x_sd5, x_mask1);
		x_sd7 = _mm_and_si128(x_sd6, x_cmp);

		x_dd0 = _mm_unpacklo_epi16(x_sd4, x_sd7);
		x_dd1 = _mm_unpackhi_epi16(x_sd4, x_sd7);
		x_dd2 = _mm_unpacklo_epi16(x_dd0, x_dd1);
		x_dd3 = _mm_unpackhi_epi16(x_dd0, x_dd1);
		x_dd4 = _mm_unpacklo_epi16(x_dd2, x_dd3);
		_mm_store_si128((__m128i *)dp, x_dd4);
		dp += 8;
	}

	for (; i < dsize; i++) {
		sd = (*sp++);
		*(dp++) = (sd | ((65535 - sd) >> 31)) & ~(sd >> 31);
	}
}

/* *********************************************************** */

void mlib_s_ImageDataTypeConvert_S32_U16_A8D2X8(
    const mlib_s32 *src,
    mlib_s32 slb,
    mlib_u16 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize)
{
	mlib_s32 *sp, *sl;
	mlib_u16 *dp, *dl;
	mlib_s32 j;

	sp = sl = (void *)src;
	dp = dl = dst;

	for (j = 0; j < ysize; j++) {

		mlib_s_ImageDataTypeConvert_S32_U16_A8D1X8(sp, dp, xsize);

		sp = sl = (mlib_s32 *)((mlib_u8 *)sl + slb);
		dp = dl = (mlib_u16 *)((mlib_u8 *)dl + dlb);
	}
}

/* *********************************************************** */

void mlib_s_ImageDataTypeConvert_S32_U16_D1(
    const mlib_s32 *src,
    mlib_u16 *dst,
    mlib_s32 dsize)
{
/* pointer for pixel in source */
	mlib_s32 *sp;

/* pointer for pixel in destination */
	mlib_u16 *dp;

	__m128i x_sd0, x_sd1, x_sd2, x_sd3;
	__m128i x_sd4, x_sd5, x_sd6, x_sd7;
	__m128i x_dd0, x_dd1, x_dd2, x_dd3, x_dd4;
	__m128i x_mask0, x_mask1;
	mlib_s32 sd;
	mlib_s32 i;

	const __m128i x_cmp = _mm_set1_epi32(65535);
	const __m128i x_zero = _mm_setzero_si128();

	sp = (void *)src;
	dp = dst;

/* 8 pixels */
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < (dsize - 7); i += 8) {
		x_sd0 = _mm_loadu_si128((__m128i *)sp);
		sp += 4;
		x_sd1 = _mm_loadu_si128((__m128i *)sp);
		sp += 4;

		x_mask0 = _mm_cmpgt_epi32(x_sd0, x_zero);
		x_sd2 = _mm_and_si128(x_sd0, x_mask0);
		x_mask1 = _mm_cmpgt_epi32(x_sd2, x_cmp);
		x_sd3 = _mm_or_si128(x_sd2, x_mask1);
		x_sd4 = _mm_and_si128(x_sd3, x_cmp);

		x_mask0 = _mm_cmpgt_epi32(x_sd1, x_zero);
		x_sd5 = _mm_and_si128(x_sd1, x_mask0);
		x_mask1 = _mm_cmpgt_epi32(x_sd5, x_cmp);
		x_sd6 = _mm_or_si128(x_sd5, x_mask1);
		x_sd7 = _mm_and_si128(x_sd6, x_cmp);

		x_dd0 = _mm_unpacklo_epi16(x_sd4, x_sd7);
		x_dd1 = _mm_unpackhi_epi16(x_sd4, x_sd7);
		x_dd2 = _mm_unpacklo_epi16(x_dd0, x_dd1);
		x_dd3 = _mm_unpackhi_epi16(x_dd0, x_dd1);
		x_dd4 = _mm_unpacklo_epi16(x_dd2, x_dd3);
		_mm_storeu_si128((__m128i *)dp, x_dd4);
		dp += 8;
	}

	for (; i < dsize; i++) {
		sd = (*sp++);
		*(dp++) = (sd | ((65535 - sd) >> 31)) & ~(sd >> 31);
	}
}

/* *********************************************************** */

void mlib_s_ImageDataTypeConvert_S32_U16(
    const mlib_s32 *src,
    mlib_s32 slb,
    mlib_u16 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize)
{
	mlib_s32 *sp, *sl;
	mlib_u16 *dp, *dl;
	mlib_s32 j;

	sp = sl = (void *)src;
	dp = dl = dst;

	for (j = 0; j < ysize; j++) {

		mlib_s_ImageDataTypeConvert_S32_U16_D1(sp, dp, xsize);

		sp = sl = (mlib_s32 *)((mlib_u8 *)sl + slb);
		dp = dl = (mlib_u16 *)((mlib_u8 *)dl + dlb);
	}
}

/* *********************************************************** */

#define	MLIB_CLAMP_S16(dst, src)                              \
{                                                             \
	mlib_s32 s = src;                                     \
	dst =                                                 \
	    (((s | ((32767 - s) >> 31)) & 0x7FFF) & ~(((s +   \
	    32767) & s) >> 31) | ((s >> 31) & 0x8000));       \
}

/* *********************************************************** */

/* S32 -> S16 */
void mlib_s_ImageDataTypeConvert_S32_S16_A8D1X8(
    const mlib_s32 *src,
    mlib_s16 *dst,
    mlib_s32 dsize)
{
/* pointer for pixel in source */
	mlib_s32 *sp;

/* pointer for pixel in destination */
	mlib_s16 *dp;

	__m128i x_sd0, x_sd1;
	__m128i x_dd0;
	mlib_s32 sd, dd;
	mlib_s32 i;

	sp = (void *)src;
	dp = dst;

/* 8 pixels */
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < (dsize - 7); i += 8) {
		x_sd0 = _mm_load_si128((__m128i *)sp);
		sp += 4;
		x_sd1 = _mm_load_si128((__m128i *)sp);
		sp += 4;
		x_dd0 = _mm_packs_epi32(x_sd0, x_sd1);
		_mm_store_si128((__m128i *)dp, x_dd0);
		dp += 8;
	}

/* 4 pixels */
	if (i < (dsize - 3)) {
		x_sd0 = _mm_load_si128((__m128i *)sp);
		sp += 4;
		x_dd0 = _mm_packs_epi32(x_sd0, x_sd0);
		_mm_storel_epi64((__m128i *)dp, x_dd0);
		dp += 4;
		i += 4;
	}

/* 2 pixels */
	if (i < (dsize - 1)) {
		x_sd0 = _mm_loadl_epi64((__m128i *)sp);
		sp += 2;
		x_dd0 = _mm_packs_epi32(x_sd0, x_sd0);
		dd = _mm_cvtsi128_si32(x_dd0);
		*((mlib_s32 *)dp) = dd;
		dp += 2;
		i += 2;
	}

	if (i < dsize) {
		sd = (*sp++);
		MLIB_CLAMP_S16(*dp, sd);
		dp++;
	}
}

/* *********************************************************** */

void mlib_s_ImageDataTypeConvert_S32_S16_A8D2X8(
    const mlib_s32 *src,
    mlib_s32 slb,
    mlib_s16 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize)
{
	mlib_s32 *sp, *sl;
	mlib_s16 *dp, *dl;
	mlib_s32 j;

	sp = sl = (void *)src;
	dp = dl = dst;

	for (j = 0; j < ysize; j++) {

		mlib_s_ImageDataTypeConvert_S32_S16_A8D1X8(sp, dp, xsize);

		sp = sl = (mlib_s32 *)((mlib_u8 *)sl + slb);
		dp = dl = (mlib_s16 *)((mlib_u8 *)dl + dlb);
	}
}

/* *********************************************************** */

void mlib_s_ImageDataTypeConvert_S32_S16_D1(
    const mlib_s32 *src,
    mlib_s16 *dst,
    mlib_s32 dsize)
{
/* pointer for pixel in source */
	mlib_s32 *sp;

/* pointer for pixel in destination */
	mlib_s16 *dp;

	__m128i x_sd0, x_sd1;
	__m128i x_dd0;
	mlib_s32 sd, dd;
	mlib_s32 i;

	sp = (void *)src;
	dp = dst;

/* 8 pixels */
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < (dsize - 7); i += 8) {
		x_sd0 = _mm_loadu_si128((__m128i *)sp);
		sp += 4;
		x_sd1 = _mm_loadu_si128((__m128i *)sp);
		sp += 4;
		x_dd0 = _mm_packs_epi32(x_sd0, x_sd1);
		_mm_storeu_si128((__m128i *)dp, x_dd0);
		dp += 8;
	}

/* 4 pixels */
	if (i < (dsize - 3)) {
		x_sd0 = _mm_loadu_si128((__m128i *)sp);
		sp += 4;
		x_dd0 = _mm_packs_epi32(x_sd0, x_sd0);
		_mm_storel_epi64((__m128i *)dp, x_dd0);
		dp += 4;
		i += 4;
	}

/* 2 pixels */
	if (i < (dsize - 1)) {
		x_sd0 = _mm_loadl_epi64((__m128i *)sp);
		sp += 2;
		x_dd0 = _mm_packs_epi32(x_sd0, x_sd0);
		dd = _mm_cvtsi128_si32(x_dd0);
		*((mlib_s32 *)dp) = dd;
		dp += 2;
		i += 2;
	}

	if (i < dsize) {
		sd = (*sp++);
		MLIB_CLAMP_S16(*dp, sd);
		dp++;
	}
}

/* *********************************************************** */

void mlib_s_ImageDataTypeConvert_S32_S16(
    const mlib_s32 *src,
    mlib_s32 slb,
    mlib_s16 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize)
{
	mlib_s32 *sp, *sl;
	mlib_s16 *dp, *dl;
	mlib_s32 j;

	sp = sl = (void *)src;
	dp = dl = dst;

	for (j = 0; j < ysize; j++) {

		mlib_s_ImageDataTypeConvert_S32_S16_D1(sp, dp, xsize);

		sp = sl = (mlib_s32 *)((mlib_u8 *)sl + slb);
		dp = dl = (mlib_s16 *)((mlib_u8 *)dl + dlb);
	}
}

/* *********************************************************** */

/* S32 -> F32 */
void mlib_s_ImageDataTypeConvert_S32_F32_A8D1X8(
    const mlib_s32 *src,
    mlib_f32 *dst,
    mlib_s32 dsize)
{
/* pointer for pixel in source */
	mlib_s32 *sp;

/* pointer for pixel in destination */
	mlib_f32 *dp;

	__m128i x_sd0;
	__m128 x_df0;
	mlib_s32 i;

	sp = (void *)src;
	dp = dst;

/* 4 pixels */
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < (dsize - 3); i += 4) {
		x_sd0 = _mm_load_si128((__m128i *)sp);
		x_df0 = _mm_cvtepi32_ps(x_sd0);
		_mm_store_ps(dp, x_df0);
		dp += 4;
		sp += 4;
	}

/* 1 pixels */
	for (; i < dsize; i++) {
		(*dp++) = (*sp++);
	}
}

/* *********************************************************** */

void mlib_s_ImageDataTypeConvert_S32_F32_A8D2X8(
    const mlib_s32 *src,
    mlib_s32 slb,
    mlib_f32 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize)
{
	mlib_s32 *sp, *sl;
	mlib_f32 *dp, *dl;
	mlib_s32 j;

	sp = sl = (void *)src;
	dp = dl = dst;

	for (j = 0; j < ysize; j++) {

		mlib_s_ImageDataTypeConvert_S32_F32_A8D1X8(sp, dp, xsize);

		sp = sl = (mlib_s32 *)((mlib_u8 *)sl + slb);
		dp = dl = (mlib_f32 *)((mlib_u8 *)dl + dlb);
	}
}

/* *********************************************************** */

void mlib_s_ImageDataTypeConvert_S32_F32_D1(
    const mlib_s32 *src,
    mlib_f32 *dst,
    mlib_s32 dsize)
{
/* pointer for pixel in source */
	mlib_s32 *sp;

/* pointer for pixel in destination */
	mlib_f32 *dp;

	__m128i x_sd0;
	__m128 x_df0;
	mlib_s32 i;

	sp = (void *)src;
	dp = dst;

/* 4 pixels */
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < (dsize - 3); i += 4) {
		x_sd0 = _mm_loadu_si128((__m128i *)sp);
		x_df0 = _mm_cvtepi32_ps(x_sd0);
		_mm_storeu_ps(dp, x_df0);
		dp += 4;
		sp += 4;
	}

/* 1 pixels */
	for (; i < dsize; i++) {
		(*dp++) = (*sp++);
	}
}

/* *********************************************************** */

void mlib_s_ImageDataTypeConvert_S32_F32(
    const mlib_s32 *src,
    mlib_s32 slb,
    mlib_f32 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize)
{
	mlib_s32 *sp, *sl;
	mlib_f32 *dp, *dl;
	mlib_s32 j;

	sp = sl = (void *)src;
	dp = dl = dst;

	for (j = 0; j < ysize; j++) {

		mlib_s_ImageDataTypeConvert_S32_F32_D1(sp, dp, xsize);

		sp = sl = (mlib_s32 *)((mlib_u8 *)sl + slb);
		dp = dl = (mlib_f32 *)((mlib_u8 *)dl + dlb);
	}
}

/* *********************************************************** */

/* S32 -> D64 */
void mlib_s_ImageDataTypeConvert_S32_D64_A8D1X8(
    const mlib_s32 *src,
    mlib_d64 *dst,
    mlib_s32 dsize)
{
/* pointer for pixel in source */
	mlib_s32 *sp;

/* pointer for pixel in destination */
	mlib_d64 *dp;

	__m128i x_sd0, x_sd1, x_sd2;
	__m128d x_dd0, x_dd1;
	mlib_s32 i;

	const __m128i x_zero = _mm_setzero_si128();

	sp = (void *)src;
	dp = dst;

/* 4 pixels */
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < (dsize - 3); i += 4) {
		x_sd0 = _mm_load_si128((__m128i *)sp);
		x_sd1 = _mm_unpacklo_epi64(x_sd0, x_zero);
		x_sd2 = _mm_unpackhi_epi64(x_sd0, x_zero);
		x_dd0 = _mm_cvtepi32_pd(x_sd1);
		x_dd1 = _mm_cvtepi32_pd(x_sd2);
		_mm_store_pd(dp, x_dd0);
		dp += 2;
		_mm_store_pd(dp, x_dd1);
		dp += 2;
		sp += 4;
	}

/* 2 pixels */
	if (i < (dsize - 1)) {
		x_sd0 = _mm_loadl_epi64((__m128i *)sp);
		x_sd1 = _mm_unpacklo_epi64(x_sd0, x_zero);
		x_dd0 = _mm_cvtepi32_pd(x_sd1);
		_mm_store_pd(dp, x_dd0);
		dp += 2;
		sp += 2;
		i += 2;
	}

/* 1 pixels */
	if (i < dsize) {
		(*dp++) = (*sp++);
	}
}

/* *********************************************************** */

void mlib_s_ImageDataTypeConvert_S32_D64_A8D2X8(
    const mlib_s32 *src,
    mlib_s32 slb,
    mlib_d64 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize)
{
	mlib_s32 *sp, *sl;
	mlib_d64 *dp, *dl;
	mlib_s32 j;

	sp = sl = (void *)src;
	dp = dl = dst;

	for (j = 0; j < ysize; j++) {

		mlib_s_ImageDataTypeConvert_S32_D64_A8D1X8(sp, dp, xsize);

		sp = sl = (mlib_s32 *)((mlib_u8 *)sl + slb);
		dp = dl = (mlib_d64 *)((mlib_u8 *)dl + dlb);
	}
}

/* *********************************************************** */

void mlib_s_ImageDataTypeConvert_S32_D64_D1(
    const mlib_s32 *src,
    mlib_d64 *dst,
    mlib_s32 dsize)
{
/* pointer for pixel in source */
	mlib_s32 *sp;

/* pointer for pixel in destination */
	mlib_d64 *dp;

	__m128i x_sd0, x_sd1, x_sd2;
	__m128d x_dd0, x_dd1;
	mlib_s32 i;
	const __m128i x_zero = _mm_setzero_si128();

	sp = (void *)src;
	dp = dst;

/* 4 pixels */
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < (dsize - 3); i += 4) {
		x_sd0 = _mm_loadu_si128((__m128i *)sp);
		x_sd1 = _mm_unpacklo_epi64(x_sd0, x_zero);
		x_sd2 = _mm_unpackhi_epi64(x_sd0, x_zero);
		x_dd0 = _mm_cvtepi32_pd(x_sd1);
		x_dd1 = _mm_cvtepi32_pd(x_sd2);
		_mm_storeu_pd(dp, x_dd0);
		dp += 2;
		_mm_storeu_pd(dp, x_dd1);
		dp += 2;
		sp += 4;
	}

/* 2 pixels */
	if (i < (dsize - 1)) {
		x_sd0 = _mm_loadl_epi64((__m128i *)sp);
		x_sd1 = _mm_unpacklo_epi64(x_sd0, x_zero);
		x_dd0 = _mm_cvtepi32_pd(x_sd1);
		_mm_storeu_pd(dp, x_dd0);
		dp += 2;
		sp += 2;
		i += 2;
	}

/* 1 pixels */
	if (i < dsize) {
		(*dp++) = (*sp++);
	}
}

/* *********************************************************** */

void mlib_s_ImageDataTypeConvert_S32_D64(
    const mlib_s32 *src,
    mlib_s32 slb,
    mlib_d64 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize)
{
	mlib_s32 *sp, *sl;
	mlib_d64 *dp, *dl;
	mlib_s32 j;

	sp = sl = (void *)src;
	dp = dl = dst;

	for (j = 0; j < ysize; j++) {

		mlib_s_ImageDataTypeConvert_S32_D64_D1(sp, dp, xsize);

		sp = sl = (mlib_s32 *)((mlib_u8 *)sl + slb);
		dp = dl = (mlib_d64 *)((mlib_u8 *)dl + dlb);
	}
}

/* *********************************************************** */

/*
 * Convert a f32-bit image into an 8-bit image.
 */

void
mlib_s_ImageDataTypeConvert_F32_U8(
    const mlib_image *src,
    mlib_image *dst)
{
/* pointer for pixel in source */
	mlib_f32 *sp;

/* pointer for pixel in destination */
	mlib_u8 *dp;

/* pointer for line in source */
	mlib_f32 *sl;

/* pointer for line in destination */
	mlib_u8 *dl;
	mlib_s32 width;
	mlib_s32 height;
	mlib_s32 channels;

/* src line stride in pixels */
	mlib_s32 strides;

/* dst line stride in pixels */
	mlib_s32 strided;
	mlib_s32 size, num;

/* indices for x, y */
	mlib_s32 i, j;

	width = mlib_ImageGetWidth(src);
	height = mlib_ImageGetHeight(src);
	channels = mlib_ImageGetChannels(src);

	if (((mlib_ImageGetFlags(src) & MLIB_IMAGE_ONEDVECTOR) == 0) &&
	    ((mlib_ImageGetFlags(dst) & MLIB_IMAGE_ONEDVECTOR) == 0)) {
		width *= height;
		height = 1;
	}

	sl = (mlib_f32 *)mlib_ImageGetData(src);
	dl = (mlib_u8 *)mlib_ImageGetData(dst);

/* in pixels */
	strides = mlib_ImageGetStride(src) >> 2;
/* in pixels */
	strided = mlib_ImageGetStride(dst);

	for (j = 0; j < height; j++) {

		for (size = 0; size < width * channels; size += num) {

			num = width * channels - size;

			if (num > SIZE)
				num = SIZE;

			sp = sl + size;
			dp = dl + size;
			for (i = 0; i < num; i++) {
				SAT8(dp[i], sp[i])
			}
		}

		sl += strides;
		dl += strided;
	}
}

/* *********************************************************** */

/*
 * Convert a f32-bit image into an 16-bit image.
 */

void
mlib_s_ImageDataTypeConvert_F32_U16(
    const mlib_image *src,
    mlib_image *dst)
{
/* pointer for pixel in source */
	mlib_f32 *sp;

/* pointer for pixel in destination */
	mlib_u16 *dp;

/* pointer for line in source */
	mlib_f32 *sl;

/* pointer for line in destination */
	mlib_u16 *dl;
	mlib_s32 width;
	mlib_s32 height;
	mlib_s32 channels;

/* src line stride in pixels */
	mlib_s32 strides;

/* dst line stride in pixels */
	mlib_s32 strided;

/* indices for x, y */
	mlib_s32 i, j;

	width = mlib_ImageGetWidth(src);
	height = mlib_ImageGetHeight(src);
	channels = mlib_ImageGetChannels(src);

	if (((mlib_ImageGetFlags(src) & MLIB_IMAGE_ONEDVECTOR) == 0) &&
	    ((mlib_ImageGetFlags(dst) & MLIB_IMAGE_ONEDVECTOR) == 0)) {
		width *= height;
		height = 1;
	}

	sl = (mlib_f32 *)mlib_ImageGetData(src);
	dl = (mlib_u16 *)mlib_ImageGetData(dst);

/* in pixels */
	strides = mlib_ImageGetStride(src) >> 2;
/* in pixels */
	strided = mlib_ImageGetStride(dst) >> 1;

	for (j = 0; j < height; j++) {
		sp = sl;
		dp = dl;
		for (i = 0; i < width * channels; i++) {
			SAT_U16((*dp++), *sp);
			sp++;
		}

		sl += strides;
		dl += strided;
	}
}

/* *********************************************************** */

/*
 * Convert a f32-bit image into an 16-bit image.
 */

void
mlib_s_ImageDataTypeConvert_F32_S16(
    const mlib_image *src,
    mlib_image *dst)
{
/* pointer for pixel in source */
	mlib_f32 *sp;

/* pointer for pixel in destination */
	mlib_s16 *dp;

/* pointer for line in source */
	mlib_f32 *sl;

/* pointer for line in destination */
	mlib_s16 *dl;
	mlib_s32 width;
	mlib_s32 height;
	mlib_s32 channels;

/* src line stride in pixels */
	mlib_s32 strides;

/* dst line stride in pixels */
	mlib_s32 strided;

/* indices for x, y */
	mlib_s32 i, j;

	width = mlib_ImageGetWidth(src);
	height = mlib_ImageGetHeight(src);
	channels = mlib_ImageGetChannels(src);

	if (((mlib_ImageGetFlags(src) & MLIB_IMAGE_ONEDVECTOR) == 0) &&
	    ((mlib_ImageGetFlags(dst) & MLIB_IMAGE_ONEDVECTOR) == 0)) {
		width *= height;
		height = 1;
	}

	sl = (mlib_f32 *)mlib_ImageGetData(src);
	dl = (mlib_s16 *)mlib_ImageGetData(dst);

/* in pixels */
	strides = mlib_ImageGetStride(src) >> 2;
/* in pixels */
	strided = mlib_ImageGetStride(dst) >> 1;

	for (j = 0; j < height; j++) {
		sp = sl;
		dp = dl;
		for (i = 0; i < width * channels; i++) {
			SAT16((*dp++), *sp);
			sp++;
		}

		sl += strides;
		dl += strided;
	}
}

/* *********************************************************** */

#ifdef _MSC_VER
#pragma optimize("", off)
#endif /* _MSC_VER */

/* *********************************************************** */

/*
 * Convert a f32-bit image into an 32-bit image.
 */

void
mlib_s_ImageDataTypeConvert_F32_S32(
    const mlib_image *src,
    mlib_image *dst)
{
/* pointer for pixel in source */
	mlib_f32 *sp;

/* pointer for pixel in destination */
	mlib_s32 *dp;

/* pointer for line in source */
	mlib_f32 *sl;

/* pointer for line in destination */
	mlib_s32 *dl;
	TYPE_64BIT *sp2, *dp2;
	mlib_s32 *dend;
	mlib_s32 width;
	mlib_s32 height;
	mlib_s32 channels;

/* src line stride in pixels */
	mlib_s32 strides;

/* dst line stride in pixels */
	mlib_s32 strided;

/* indices for x, y */
	mlib_s32 j;

	width = mlib_ImageGetWidth(src);
	height = mlib_ImageGetHeight(src);
	channels = mlib_ImageGetChannels(src);

	if (((mlib_ImageGetFlags(src) & MLIB_IMAGE_ONEDVECTOR) == 0) &&
	    ((mlib_ImageGetFlags(dst) & MLIB_IMAGE_ONEDVECTOR) == 0)) {
		width *= height;
		height = 1;
	}

	sp = sl = (mlib_f32 *)mlib_ImageGetData(src);
	dp = dl = (mlib_s32 *)mlib_ImageGetData(dst);

/* in pixels */
	strides = mlib_ImageGetStride(src) >> 2;
/* in pixels */
	strided = mlib_ImageGetStride(dst) >> 2;

	for (j = 0; j < height; j++) {
		dend = dp + width * channels;

		if ((mlib_addr)dp & 7) {
			SAT32((*dp++), *sp);
			sp++;
		}

		dp2 = (TYPE_64BIT *) dp;

		if (((mlib_addr)sp & 7) == 0) {
			sp2 = (TYPE_64BIT *) sp;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (; (mlib_s32 *)dp2 <= (dend - 2); dp2++) {
				d64_2x32 dd;

				dd.d64 = (*sp2++);
				SAT32(dd.i32s.f0, dd.f32s.f0);
				SAT32(dd.i32s.f1, dd.f32s.f1);
				dp2[0] = dd.d64;
			}

			sp = (mlib_f32 *)sp2;
		} else {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (; (mlib_s32 *)dp2 <= (dend - 2); dp2++) {
				d64_2x32 dd;

				SAT32(dd.i32s.f0, *sp);
				sp++;
				SAT32(dd.i32s.f1, *sp);
				sp++;
				dp2[0] = dd.d64;
			}
		}

		dp = (mlib_s32 *)dp2;

		if (dp < dend) {
			SAT32(*dp, *sp);
		}

		sp = sl += strides;
		dp = dl += strided;
	}
}

/* *********************************************************** */

#ifdef _MSC_VER
#pragma optimize("", on)
#endif /* _MSC_VER */

/* *********************************************************** */

/* F32 -> D64 */
void mlib_s_ImageDataTypeConvert_F32_D64_A8D1X8(
    const mlib_f32 *src,
    mlib_d64 *dst,
    mlib_s32 dsize)
{
/* pointer for pixel in source */
	mlib_f32 *sp;

/* pointer for pixel in destination */
	mlib_d64 *dp;

	__m128 x_sd0;
	__m128d x_dd0;
	mlib_s32 i;

	sp = (void *)src;
	dp = dst;

/* 2 pixels */
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < (dsize - 1); i += 2) {
		x_sd0 = _mm_loadu_ps(sp);
		x_dd0 = _mm_cvtps_pd(x_sd0);
		_mm_store_pd(dp, x_dd0);
		dp += 2;
		sp += 2;
	}

/* 1 pixels */
	if (i < dsize) {
		(*dp++) = (*sp++);
	}
}

/* *********************************************************** */

void mlib_s_ImageDataTypeConvert_F32_D64_A8D2X8(
    const mlib_f32 *src,
    mlib_s32 slb,
    mlib_d64 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize)
{
	mlib_f32 *sp, *sl;
	mlib_d64 *dp, *dl;
	mlib_s32 j;

	sp = sl = (void *)src;
	dp = dl = dst;

	for (j = 0; j < ysize; j++) {

		mlib_s_ImageDataTypeConvert_F32_D64_A8D1X8(sp, dp, xsize);

		sp = sl = (mlib_f32 *)((mlib_u8 *)sl + slb);
		dp = dl = (mlib_d64 *)((mlib_u8 *)dl + dlb);
	}
}

/* *********************************************************** */

void mlib_s_ImageDataTypeConvert_F32_D64_D1(
    const mlib_f32 *src,
    mlib_d64 *dst,
    mlib_s32 dsize)
{
/* pointer for pixel in source */
	mlib_f32 *sp;

/* pointer for pixel in destination */
	mlib_d64 *dp;

	__m128 x_sd0;
	__m128d x_dd0;
	mlib_s32 i;

	sp = (void *)src;
	dp = dst;

/* 2 pixels */
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < (dsize - 1); i += 2) {
		x_sd0 = _mm_loadu_ps(sp);
		x_dd0 = _mm_cvtps_pd(x_sd0);
		_mm_storeu_pd(dp, x_dd0);
		dp += 2;
		sp += 2;
	}

/* 1 pixels */
	if (i < dsize) {
		(*dp++) = (*sp++);
	}
}

/* *********************************************************** */

void mlib_s_ImageDataTypeConvert_F32_D64(
    const mlib_f32 *src,
    mlib_s32 slb,
    mlib_d64 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize)
{
	mlib_f32 *sp, *sl;
	mlib_d64 *dp, *dl;
	mlib_s32 j;

	sp = sl = (void *)src;
	dp = dl = dst;

	for (j = 0; j < ysize; j++) {

		mlib_s_ImageDataTypeConvert_F32_D64_D1(sp, dp, xsize);

		sp = sl = (mlib_f32 *)((mlib_u8 *)sl + slb);
		dp = dl = (mlib_d64 *)((mlib_u8 *)dl + dlb);
	}
}

/* *********************************************************** */

/*
 * Convert an d64-bit image into a 1-bit image.
 */

void
mlib_s_ImageDataTypeConvert_D64_BIT(
    const mlib_image *src,
    mlib_image *dst)
{
/* pointer for pixel in source */
	mlib_u16 *sp;

/* pointer for pixel in destination */
	mlib_u8 *dp;

/* pointer for line in source */
	mlib_u16 *sl;

/* pointer for line in destination */
	mlib_u8 *dl;
	mlib_s32 width, height, channels;

/* src line strides in pixels */
	mlib_s32 strides;

/* dst line strides in bytes */
	mlib_s32 strided;
	mlib_s32 dsize;

/* intermediate value */
	mlib_s32 bitdata;

/* indices for x, y and bit */
	mlib_s32 i, j;

	width = mlib_ImageGetWidth(src);
	height = mlib_ImageGetHeight(src);
	channels = mlib_ImageGetChannels(src);

	sp = sl = mlib_ImageGetData(src);
	dp = dl = mlib_ImageGetData(dst);

/* in u16's */
	strides = mlib_ImageGetStride(src) >> 1;
/* in bytes */
	strided = mlib_ImageGetStride(dst);

	dsize = width * channels;

	for (j = 0; j < height; j++) {
		mlib_s32 s0, s1, s2, s3, s4, s5, s6, s7;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < dsize / 8; i++) {
#ifdef _LITTLE_ENDIAN
			s0 = -(((mlib_s16 *)sp)[3] | sp[0] | sp[1] | sp[2]);
			s1 = -(((mlib_s16 *)sp)[7] | sp[4] | sp[5] | sp[6]);
			s2 = -(((mlib_s16 *)sp)[11] | sp[8] | sp[9] | sp[10]);
			s3 = -(((mlib_s16 *)sp)[15] | sp[12] | sp[13] | sp[14]);
			s4 = -(((mlib_s16 *)sp)[19] | sp[16] | sp[17] | sp[18]);
			s5 = -(((mlib_s16 *)sp)[23] | sp[20] | sp[21] | sp[22]);
			s6 = -(((mlib_s16 *)sp)[27] | sp[24] | sp[25] | sp[26]);
			s7 = -(((mlib_s16 *)sp)[31] | sp[28] | sp[29] | sp[30]);
#else /* _LITTLE_ENDIAN */
			s0 = -(((mlib_s16 *)sp)[0] | sp[1] | sp[2] | sp[3]);
			s1 = -(((mlib_s16 *)sp)[4] | sp[5] | sp[6] | sp[7]);
			s2 = -(((mlib_s16 *)sp)[8] | sp[9] | sp[10] | sp[11]);
			s3 = -(((mlib_s16 *)sp)[12] | sp[13] | sp[14] | sp[15]);
			s4 = -(((mlib_s16 *)sp)[16] | sp[17] | sp[18] | sp[19]);
			s5 = -(((mlib_s16 *)sp)[20] | sp[21] | sp[22] | sp[23]);
			s6 = -(((mlib_s16 *)sp)[24] | sp[25] | sp[26] | sp[27]);
			s7 = -(((mlib_s16 *)sp)[28] | sp[29] | sp[30] | sp[31]);
#endif /* _LITTLE_ENDIAN */
			bitdata = (s0 & 0x80000000) |
			    (s1 & 0x40000000) |
			    (s2 & 0x20000000) |
			    (s3 & 0x10000000) |
			    (s4 & 0x08000000) |
			    (s5 & 0x04000000) | (s6 & 0x02000000) | (s7 &
			    0x01000000);

			(*dp++) = bitdata >> 24;
			sp += 32;
		}

		if (dsize & 7) {
			mlib_s32 mask = 0xFF00 >> (dsize & 7);
			mlib_u32 and = 0x80000000;
			bitdata = 0;

			for (i = 0; i < (dsize & 7); i++) {
#ifdef _LITTLE_ENDIAN
				s0 = -(((mlib_s16 *)sp)[3] | sp[0] | sp[1] |
					sp[2]);
#else /* _LITTLE_ENDIAN */
				s0 = -(((mlib_s16 *)sp)[0] | sp[1] | sp[2] |
					sp[3]);
#endif /* _LITTLE_ENDIAN */
				bitdata |= (s0 & and);
				and >>= 1;
				sp += 4;

			}
			dl[dsize / 8] = (bitdata >> 24) |
				(dl[dsize / 8] & ~mask);
		}

		sp = sl += strides;
		dp = dl += strided;
	}
}

/* *********************************************************** */

/*
 * Convert a d64-bit image into an 8-bit image.
 */

void
mlib_s_ImageDataTypeConvert_D64_U8(
    const mlib_image *src,
    mlib_image *dst)
{
/* pointer for pixel in source */
	mlib_d64 *sp;

/* pointer for pixel in destination */
	mlib_u8 *dp;

/* pointer for line in source */
	mlib_d64 *sl;

/* pointer for line in destination */
	mlib_u8 *dl;
	mlib_s32 width;
	mlib_s32 height;
	mlib_s32 channels;

/* src line stride in pixels */
	mlib_s32 strides;

/* dst line stride in pixels */
	mlib_s32 strided;
	mlib_s32 size, num;

/* indices for x, y */
	mlib_s32 i, j;

	width = mlib_ImageGetWidth(src);
	height = mlib_ImageGetHeight(src);
	channels = mlib_ImageGetChannels(src);

	if (((mlib_ImageGetFlags(src) & MLIB_IMAGE_ONEDVECTOR) == 0) &&
	    ((mlib_ImageGetFlags(dst) & MLIB_IMAGE_ONEDVECTOR) == 0)) {
		width *= height;
		height = 1;
	}

	sl = (mlib_d64 *)mlib_ImageGetData(src);
	dl = (mlib_u8 *)mlib_ImageGetData(dst);

/* in pixels */
	strides = mlib_ImageGetStride(src) >> 3;
/* in pixels */
	strided = mlib_ImageGetStride(dst);

	for (j = 0; j < height; j++) {

		for (size = 0; size < width * channels; size += num) {

			num = width * channels - size;

			if (num > SIZE)
				num = SIZE;

			sp = sl + size;
			dp = dl + size;
			for (i = 0; i < num; i++) {
				SAT8(dp[i], sp[i]);
			}
		}

		sl += strides;
		dl += strided;
	}
}

/* *********************************************************** */

/*
 * Convert a d64-bit image into an 16-bit image.
 */

void
mlib_s_ImageDataTypeConvert_D64_U16(
    const mlib_image *src,
    mlib_image *dst)
{
/* pointer for pixel in source */
	mlib_d64 *sp;

/* pointer for pixel in destination */
	mlib_u16 *dp;

/* pointer for line in source */
	mlib_d64 *sl;

/* pointer for line in destination */
	mlib_u16 *dl;
	mlib_s32 width;
	mlib_s32 height;
	mlib_s32 channels;

/* src line stride in pixels */
	mlib_s32 strides;

/* dst line stride in pixels */
	mlib_s32 strided;

/* indices for x, y */
	mlib_s32 i, j;

	width = mlib_ImageGetWidth(src);
	height = mlib_ImageGetHeight(src);
	channels = mlib_ImageGetChannels(src);

	if (((mlib_ImageGetFlags(src) & MLIB_IMAGE_ONEDVECTOR) == 0) &&
	    ((mlib_ImageGetFlags(dst) & MLIB_IMAGE_ONEDVECTOR) == 0)) {
		width *= height;
		height = 1;
	}

	sl = (mlib_d64 *)mlib_ImageGetData(src);
	dl = (mlib_u16 *)mlib_ImageGetData(dst);

/* in pixels */
	strides = mlib_ImageGetStride(src) >> 3;
/* in pixels */
	strided = mlib_ImageGetStride(dst) >> 1;

	for (j = 0; j < height; j++) {
		sp = sl;
		dp = dl;
		for (i = 0; i < width * channels; i++) {
			SAT_U16((*dp++), *sp);
			sp++;
		}

		sl += strides;
		dl += strided;
	}
}

/* *********************************************************** */

/*
 * Convert a d64-bit image into an 16-bit image.
 */

void
mlib_s_ImageDataTypeConvert_D64_S16(
    const mlib_image *src,
    mlib_image *dst)
{
/* pointer for pixel in source */
	mlib_d64 *sp;

/* pointer for pixel in destination */
	mlib_s16 *dp;

/* pointer for line in source */
	mlib_d64 *sl;

/* pointer for line in destination */
	mlib_s16 *dl;
	mlib_s32 width;
	mlib_s32 height;
	mlib_s32 channels;

/* src line stride in pixels */
	mlib_s32 strides;

/* dst line stride in pixels */
	mlib_s32 strided;

/* indices for x, y */
	mlib_s32 i, j;

	width = mlib_ImageGetWidth(src);
	height = mlib_ImageGetHeight(src);
	channels = mlib_ImageGetChannels(src);

	if (((mlib_ImageGetFlags(src) & MLIB_IMAGE_ONEDVECTOR) == 0) &&
	    ((mlib_ImageGetFlags(dst) & MLIB_IMAGE_ONEDVECTOR) == 0)) {
		width *= height;
		height = 1;
	}

	sl = (mlib_d64 *)mlib_ImageGetData(src);
	dl = (mlib_s16 *)mlib_ImageGetData(dst);

/* in pixels */
	strides = mlib_ImageGetStride(src) >> 3;
/* in pixels */
	strided = mlib_ImageGetStride(dst) >> 1;

	for (j = 0; j < height; j++) {
		sp = sl;
		dp = dl;
		for (i = 0; i < width * channels; i++) {
			SAT16((*dp++), *sp);
			sp++;
		}

		sl += strides;
		dl += strided;
	}
}

/* *********************************************************** */

/*
 * Convert a d64-bit image into an 32-bit image.
 */

void
mlib_s_ImageDataTypeConvert_D64_S32(
    const mlib_image *src,
    mlib_image *dst)
{
/* pointer for pixel in source */
	mlib_d64 *sp;

/* pointer for pixel in destination */
	mlib_s32 *dp;

/* pointer for line in source */
	mlib_d64 *sl;

/* pointer for line in destination */
	mlib_s32 *dl;
	mlib_s32 width;
	mlib_s32 height;
	mlib_s32 channels;

/* src line stride in pixels */
	mlib_s32 strides;

/* dst line stride in pixels */
	mlib_s32 strided;
	mlib_s32 sd;

/* indices for x, y */
	mlib_s32 i, j;

	width = mlib_ImageGetWidth(src);
	height = mlib_ImageGetHeight(src);
	channels = mlib_ImageGetChannels(src);

	if (((mlib_ImageGetFlags(src) & MLIB_IMAGE_ONEDVECTOR) == 0) &&
	    ((mlib_ImageGetFlags(dst) & MLIB_IMAGE_ONEDVECTOR) == 0)) {
		width *= height;
		height = 1;
	}

	sp = sl = (mlib_d64 *)mlib_ImageGetData(src);
	dp = dl = (mlib_s32 *)mlib_ImageGetData(dst);

/* in pixels */
	strides = mlib_ImageGetStride(src) >> 3;
/* in pixels */
	strided = mlib_ImageGetStride(dst) >> 2;

	for (j = 0; j < height; j++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < width * channels; i++) {
			SAT32(sd, *sp);
			sp++;
			(*dp++) = sd;
		}

		sp = sl += strides;
		dp = dl += strided;
	}
}

/* *********************************************************** */

/* D64 -> F32 */
void mlib_s_ImageDataTypeConvert_D64_F32_D1(
    const mlib_d64 *src,
    mlib_f32 *dst,
    mlib_s32 dsize)
{
/* pointer for pixel in source */
	mlib_d64 *sp;

/* pointer for pixel in destination */
	mlib_f32 *dp;

	__m128d x_sd0;
	__m128 x_df0;
	mlib_s32 i;

	sp = (void *)src;
	dp = dst;

/* 1 pixels */
	for (i = 0; i < dsize; i++) {
		(*dp++) = (mlib_f32)(*sp++);
	}
}

/* *********************************************************** */

void mlib_s_ImageDataTypeConvert_D64_F32(
    const mlib_d64 *src,
    mlib_s32 slb,
    mlib_f32 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize)
{
	mlib_d64 *sp, *sl;
	mlib_f32 *dp, *dl;
	mlib_s32 j;

	sp = sl = (void *)src;
	dp = dl = dst;

	for (j = 0; j < ysize; j++) {

		mlib_s_ImageDataTypeConvert_D64_F32_D1(sp, dp, xsize);

		sp = sl = (mlib_d64 *)((mlib_u8 *)sl + slb);
		dp = dl = (mlib_f32 *)((mlib_u8 *)dl + dlb);
	}
}

/* *********************************************************** */
