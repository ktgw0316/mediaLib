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

#pragma ident	"@(#)mlib_i_VideoIDCT8x8_Q1.c	9.3	07/11/05 SMI"

/*
 * FUNCTIONS
 *
 *   mlib_VideoIDCT8x8_U8_S16_Q1 - computes the inverse DCT.
 *                                 The stride applies to the block which is
 *                                 the part of the current frame
 *                                 being reconstructed,
 *                                 in case of an intra block.
 *   mlib_VideoIDCT8x8_S16_S16_Q1 -computes the inverse DCT in the inter mode.
 *                                 The output of the IDCT routine which
 *                                 is really the difference between
 *                                 the current block and the
 *                                 reference block can occupy 9 bits, and is
 *                                 represented as a 16-bit data.
 *                                 This output needs to be added to
 *                                 the motion-compensated reference
 *                                 block to reconstruct the current block.
 *
 * SYNOPSIS
 *
 *      mlib_status  mlib_VideoIDCT8x8_U8_S16_Q1(mlib_u8        *block,
 *                                               const mlib_s16 *coeffs,
 *                                               mlib_s32       stride)
 *
 *      mlib_status  mlib_VideoIDCT8x8_S16_S16_Q1(mlib_s16       *block,
 *                                                const mlib_s16 *coeffs)
 *
 * ARGUMENTS
 *
 *      block       Pointer to 8x8 block in current frame.
 *      coeffs      Pointer to input DCT coefficients.
 *      stride      Stride in bytes between adjacent rows in a block.
 *
 *      It is assumed that block and coeffs are 8-byte aligned and stride is
 *      a multiple of 8.
 *      Input DCT coeffs should be in S12 range or it should be obtained
 *      from the correspondent direct DCT.
 *
 * DESCRIPTION
 *   function should be used when coeffs[u][v] (u, v = 0...7) are non zero
 *   only for u and v less then 4
 *
 *   F = F(u, v)  u = 0, 1, ... ,7; v = 0, 1, ... ,7
 *   f = f(x, y)  x = 0, 1, ... ,7; y = 0, 1, ... ,7
 *
 *             1   3   3                        pi*(2*x+1)*u      pi*(2*y+1)*v
 *   f(x, y) = ---*SUM SUM C(u)*C(v)*F(u,v)*cos(------------)*cos(------------)
 *             4  u = 0 v = 0                        16                  16
 *
 *   C(u) = 1/sqrt(2) if u == 0, otherwise C(u) = 1
 */

#include <mlib_video.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_VideoIDCT8x8_S16_S16_Q1 = __mlib_VideoIDCT8x8_S16_S16_Q1
#pragma weak mlib_VideoIDCT8x8_U8_S16_Q1 = __mlib_VideoIDCT8x8_U8_S16_Q1

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_VideoIDCT8x8_S16_S16_Q1) mlib_VideoIDCT8x8_S16_S16_Q1
	__attribute__((weak, alias("__mlib_VideoIDCT8x8_S16_S16_Q1")));
__typeof__(__mlib_VideoIDCT8x8_U8_S16_Q1) mlib_VideoIDCT8x8_U8_S16_Q1
	__attribute__((weak, alias("__mlib_VideoIDCT8x8_U8_S16_Q1")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

#ifdef _NO_LONGLONG
/* *********************************************************** */

#define	TWOSQRT2	0.35355339
#define	COS_1_16	0.9807852804032304305792422383*0.5
#define	COS_2_16	0.9238795325112867384831361051*0.5
#define	COS_3_16	0.8314696123025452356714026791*0.5
#define	COS_4_16	0.7071067811865475727373109294
#define	COS_5_16	0.5555702330196022886710238708*0.5
#define	COS_6_16	0.3826834323650898372903839117*0.5
#define	COS_7_16	0.1950903220161283313505151682*0.5

/* *********************************************************** */

#define	RTWOSQRT2	0.35355339*16777216.0
#define	RCOS_1_16	0.9807852804032304305792422383*8388608.0
#define	RCOS_2_16	0.9238795325112867384831361051*8388608.0
#define	RCOS_3_16	0.8314696123025452356714026791*8388608.0
#define	RCOS_5_16	0.5555702330196022886710238708*8388608.0
#define	RCOS_6_16	0.3826834323650898372903839117*8388608.0
#define	RCOS_7_16	0.1950903220161283313505151682*8388608.0

/* *********************************************************** */

#ifdef MLIB_USE_FTOI_CLAMPING

#define	SATURATE(src, dst)	dst = (((mlib_s32)((src))) >> 24) ^ 128

#else /* MLIB_USE_FTOI_CLAMPING */

#define	SATURATE(src, dst)                                       \
	if ((src) > MLIB_S32_MAX)                                \
	    dst = MLIB_U8_MAX;                                   \
	else                                                     \
	    if ((src) <= MLIB_S32_MIN)                           \
		dst = MLIB_U8_MIN;                               \
	    else                                                 \
		dst = (((mlib_u32)(mlib_s32)((src))) >> 24) ^ 128

#endif /* MLIB_USE_FTOI_CLAMPING */

/* *********************************************************** */

#define	IDCT1(inPtr, outPtr, cadd)                              \
	x0 = inPtr[8 * 0] + 4 + cadd;                           \
	for (i = 0; i < 4; i++) {                               \
	                                                        \
/*                                                              \
 * first stage                                                  \
 */                                                             \
	    x4 = COS_1_16 * inPtr[8 * 1];                       \
	    x5 = COS_7_16 * inPtr[8 * 1];                       \
	    x6 = COS_3_16 * inPtr[8 * 3];                       \
	    x7 = COS_5_16 * inPtr[8 * 3];                       \
/*                                                              \
 * second stage                                                 \
 */                                                             \
	    x8 = TWOSQRT2 * (x0);                               \
	    x0 = TWOSQRT2 * (x0);                               \
	    x2 = COS_6_16 * inPtr[8 * 2];                       \
	    x3 = COS_2_16 * inPtr[8 * 2];                       \
	    x1 = x4 + x6;                                       \
	    x4 = COS_4_16 * (x4 - x6);                          \
	    x6 = x5 - x7;                                       \
	    x5 = COS_4_16 * (x5 + x7);                          \
/*                                                              \
 * third stage                                                  \
 */                                                             \
	    x7 = x8 + x3;                                       \
	    x8 -= x3;                                           \
	    x3 = x0 + x2;                                       \
	    x0 -= x2;                                           \
	    x2 = (x4 + x5);                                     \
	    x4 -= x5;                                           \
/*                                                              \
 * fourth stage                                                 \
 */                                                             \
	    outPtr[0] = x7 + x1;                                \
	    outPtr[1] = x3 + x2;                                \
	    outPtr[2] = x0 + x4;                                \
	    outPtr[3] = x8 + x6;                                \
	    outPtr[4] = x8 - x6;                                \
	    outPtr[5] = x0 - x4;                                \
	    outPtr[6] = x3 - x2;                                \
	    outPtr[7] = x7 - x1;                                \
	    inPtr++;                                            \
	    outPtr += str;                                      \
	    x0 = inPtr[8 * 0];                                  \
	}

/* *********************************************************** */

#define	IDCT2(inPtr, outPtr, csub)                              \
	for (i = 0; i < 8; i++) {                               \
	                                                        \
/*                                                              \
 * first stage                                                  \
 */                                                             \
	    x4 = COS_1_16 * inPtr[8 * 1];                       \
	    x5 = COS_7_16 * inPtr[8 * 1];                       \
	    x6 = COS_3_16 * inPtr[8 * 3];                       \
	    x7 = COS_5_16 * inPtr[8 * 3];                       \
/*                                                              \
 * second stage                                                 \
 */                                                             \
	    x8 = TWOSQRT2 * (inPtr[8 * 0]);                     \
	    x0 = TWOSQRT2 * (inPtr[8 * 0]);                     \
	    x2 = COS_6_16 * inPtr[8 * 2];                       \
	    x3 = COS_2_16 * inPtr[8 * 2];                       \
	    x1 = x4 + x6;                                       \
	    x4 = COS_4_16 * (x4 - x6);                          \
	    x6 = x5 - x7;                                       \
	    x5 = COS_4_16 * (x5 + x7);                          \
/*                                                              \
 * third stage                                                  \
 */                                                             \
	    x7 = x8 + x3;                                       \
	    x8 -= x3;                                           \
	    x3 = x0 + x2;                                       \
	    x0 -= x2;                                           \
	    x2 = (x4 + x5);                                     \
	    x4 -= x5;                                           \
/*                                                              \
 * fourth stage                                                 \
 */                                                             \
	    outPtr[0] = (mlib_s32)(x7 + x1) - csub;             \
	    outPtr[1] = (mlib_s32)(x3 + x2) - csub;             \
	    outPtr[2] = (mlib_s32)(x0 + x4) - csub;             \
	    outPtr[3] = (mlib_s32)(x8 + x6) - csub;             \
	    outPtr[4] = (mlib_s32)(x8 - x6) - csub;             \
	    outPtr[5] = (mlib_s32)(x0 - x4) - csub;             \
	    outPtr[6] = (mlib_s32)(x3 - x2) - csub;             \
	    outPtr[7] = (mlib_s32)(x7 - x1) - csub;             \
	    inPtr++;                                            \
	    outPtr += str;                                      \
	}

/* *********************************************************** */

mlib_status
__mlib_VideoIDCT8x8_U8_S16_Q1(
	mlib_u8 *block,
	const mlib_s16 *coeffs,
	mlib_s32 stride)
{
	const mlib_s16 *coeffPtr = coeffs;
	mlib_u8 *blockPtr = block;
	mlib_d64 workspace[64];
	mlib_d64 *workPtr = workspace;
	mlib_d64 x0, x1, x2, x3, x4, x5, x6, x7, x8;
	mlib_s32 i;
	mlib_s32 str = 8;

	mlib_d64 *inPtr;

	IDCT1(coeffPtr, workPtr, (-128 * 8));

	inPtr = workspace;

	for (i = 0; i < 8; i++) {
/* first stage */
		x4 = RCOS_1_16 * inPtr[8 * 1];
		x5 = RCOS_7_16 * inPtr[8 * 1];

		x6 = RCOS_3_16 * inPtr[8 * 3];
		x7 = RCOS_5_16 * inPtr[8 * 3];

/* second stage */
		x8 = RTWOSQRT2 * (inPtr[8 * 0]);
		x0 = RTWOSQRT2 * (inPtr[8 * 0]);

		x2 = RCOS_6_16 * inPtr[8 * 2];
		x3 = RCOS_2_16 * inPtr[8 * 2];

		x1 = x4 + x6;
		x4 = COS_4_16 * (x4 - x6);
		x6 = x5 - x7;
		x5 = COS_4_16 * (x5 + x7);

/* third stage */
		x7 = x8 + x3;
		x8 -= x3;
		x3 = x0 + x2;
		x0 -= x2;
		x2 = (x4 + x5);
		x4 -= x5;

/* fourth stage */
		SATURATE(x7 + x1, blockPtr[0]);
		SATURATE(x3 + x2, blockPtr[1]);
		SATURATE(x0 + x4, blockPtr[2]);
		SATURATE(x8 + x6, blockPtr[3]);
		SATURATE(x8 - x6, blockPtr[4]);
		SATURATE(x0 - x4, blockPtr[5]);
		SATURATE(x3 - x2, blockPtr[6]);
		SATURATE(x7 - x1, blockPtr[7]);

		inPtr++;
		blockPtr += stride;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VideoIDCT8x8_S16_S16_Q1(
	mlib_s16 *block,
	const mlib_s16 *coeffs)
{
	const mlib_s16 *coeffPtr = coeffs;
	mlib_s16 *blockPtr = block;
	mlib_d64 workspace[64];
	mlib_d64 *workPtr = workspace;
	mlib_d64 x0, x1, x2, x3, x4, x5, x6, x7, x8;
	mlib_s32 str = 8;
	mlib_s32 i;

	IDCT1(coeffPtr, workPtr, 16384);

	workPtr = workspace;

	IDCT2(workPtr, blockPtr, 2048);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
#else
/* *********************************************************** */
/*
 * All coffes are multiplied with (1<<20)
 */
#define	TWOSQRT2	370728L
#define	COS_1_16	514214L
#define	COS_2_16	484379L
#define	COS_3_16	435930L
#define	COS_4_16	741455L
#define	COS_5_16	291279L
#define	COS_6_16	200636L
#define	COS_7_16	102284L

/* *********************************************************** */

#define	RTWOSQRT2	5931642L
#define	RCOS_1_16	8227423L
#define	RCOS_2_16	7750063L
#define	RCOS_3_16	6974873L
#define	RCOS_5_16	4660461L
#define	RCOS_6_16	3210181L
#define	RCOS_7_16	1636536L

/* *********************************************************** */

#define	ROUND(a)	((a + (1<<19)) >> 20)

#define	SATURATE(src, dst)                                       \
	if ((src) > MLIB_S32_MAX)                                \
	    dst = MLIB_U8_MAX;                                   \
	else                                                     \
	    if ((src) <= MLIB_S32_MIN)                           \
		dst = MLIB_U8_MIN;                               \
	    else                                                 \
		dst = (((mlib_u32)(mlib_s32)((src))) >> 24) ^ 128


/* *********************************************************** */

#define	IDCT1(inPtr, outPtr, cadd)                              \
	x0 = inPtr[8 * 0] + 4 + cadd;                           \
	for (i = 0; i < 4; i++) {                               \
	                                                        \
/*                                                              \
 * first stage                                                  \
 */                                                             \
	    x4 = ROUND(COS_1_16 * inPtr[8 * 1]);                \
	    x5 = ROUND(COS_7_16 * inPtr[8 * 1]);                \
	    x6 = ROUND(COS_3_16 * inPtr[8 * 3]);                \
	    x7 = ROUND(COS_5_16 * inPtr[8 * 3]);                \
/*                                                              \
 * second stage                                                 \
 */                                                             \
	    x8 = ROUND(TWOSQRT2 * (x0));                        \
	    x0 = ROUND(TWOSQRT2 * (x0));                        \
	    x2 = ROUND(COS_6_16 * inPtr[8 * 2]);                \
	    x3 = ROUND(COS_2_16 * inPtr[8 * 2]);                \
	    x1 = x4 + x6;                                       \
	    x4 = ROUND(COS_4_16 * (x4 - x6));                   \
	    x6 = x5 - x7;                                       \
	    x5 = ROUND(COS_4_16 * (x5 + x7));                   \
/*                                                              \
 * third stage                                                  \
 */                                                             \
	    x7 = x8 + x3;                                       \
	    x8 -= x3;                                           \
	    x3 = x0 + x2;                                       \
	    x0 -= x2;                                           \
	    x2 = (x4 + x5);                                     \
	    x4 -= x5;                                           \
/*                                                              \
 * fourth stage                                                 \
 */                                                             \
	    outPtr[0] = x7 + x1;                                \
	    outPtr[1] = x3 + x2;                                \
	    outPtr[2] = x0 + x4;                                \
	    outPtr[3] = x8 + x6;                                \
	    outPtr[4] = x8 - x6;                                \
	    outPtr[5] = x0 - x4;                                \
	    outPtr[6] = x3 - x2;                                \
	    outPtr[7] = x7 - x1;                                \
	    inPtr++;                                            \
	    outPtr += str;                                      \
	    x0 = inPtr[8 * 0];                                  \
	}

/* *********************************************************** */

#define	IDCT2(inPtr, outPtr, csub)                              \
	for (i = 0; i < 8; i++) {                               \
	                                                        \
/*                                                              \
 * first stage                                                  \
 */                                                             \
	    x4 = ROUND(COS_1_16 * inPtr[8 * 1]);                \
	    x5 = ROUND(COS_7_16 * inPtr[8 * 1]);                \
	    x6 = ROUND(COS_3_16 * inPtr[8 * 3]);                \
	    x7 = ROUND(COS_5_16 * inPtr[8 * 3]);                \
/*                                                              \
 * second stage                                                 \
 */                                                             \
	    x8 = ROUND(TWOSQRT2 * (inPtr[8 * 0]));              \
	    x0 = ROUND(TWOSQRT2 * (inPtr[8 * 0]));              \
	    x2 = ROUND(COS_6_16 * inPtr[8 * 2]);                \
	    x3 = ROUND(COS_2_16 * inPtr[8 * 2]);                \
	    x1 = x4 + x6;                                       \
	    x4 = ROUND(COS_4_16 * (x4 - x6));                   \
	    x6 = x5 - x7;                                       \
	    x5 = ROUND(COS_4_16 * (x5 + x7));                   \
/*                                                              \
 * third stage                                                  \
 */                                                             \
	    x7 = x8 + x3;                                       \
	    x8 -= x3;                                           \
	    x3 = x0 + x2;                                       \
	    x0 -= x2;                                           \
	    x2 = (x4 + x5);                                     \
	    x4 -= x5;                                           \
/*                                                              \
 * fourth stage                                                 \
 */                                                             \
	    outPtr[0] = (mlib_s32)(x7 + x1) - csub;             \
	    outPtr[1] = (mlib_s32)(x3 + x2) - csub;             \
	    outPtr[2] = (mlib_s32)(x0 + x4) - csub;             \
	    outPtr[3] = (mlib_s32)(x8 + x6) - csub;             \
	    outPtr[4] = (mlib_s32)(x8 - x6) - csub;             \
	    outPtr[5] = (mlib_s32)(x0 - x4) - csub;             \
	    outPtr[6] = (mlib_s32)(x3 - x2) - csub;             \
	    outPtr[7] = (mlib_s32)(x7 - x1) - csub;             \
	    inPtr++;                                            \
	    outPtr += str;                                      \
	}

/* *********************************************************** */

mlib_status
__mlib_VideoIDCT8x8_U8_S16_Q1(
	mlib_u8 *block,
	const mlib_s16 *coeffs,
	mlib_s32 stride)
{
	const mlib_s16 *coeffPtr = coeffs;
	mlib_u8 *blockPtr = block;
	mlib_s64 workspace[64];
	mlib_s64 *workPtr = workspace;
	mlib_s64 x0, x1, x2, x3, x4, x5, x6, x7, x8;
	mlib_s32 i;
	mlib_s32 str = 8;

	mlib_s64 *inPtr;

	IDCT1(coeffPtr, workPtr, (-128 * 8));

	inPtr = workspace;

	for (i = 0; i < 8; i++) {
/* first stage */
		x4 = RCOS_1_16 * inPtr[8 * 1];
		x5 = RCOS_7_16 * inPtr[8 * 1];

		x6 = RCOS_3_16 * inPtr[8 * 3];
		x7 = RCOS_5_16 * inPtr[8 * 3];

/* second stage */
		x8 = RTWOSQRT2 * (inPtr[8 * 0]);
		x0 = RTWOSQRT2 * (inPtr[8 * 0]);

		x2 = RCOS_6_16 * inPtr[8 * 2];
		x3 = RCOS_2_16 * inPtr[8 * 2];

		x1 = x4 + x6;
		x4 = ROUND(COS_4_16 * (x4 - x6));
		x6 = x5 - x7;
		x5 = ROUND(COS_4_16 * (x5 + x7));

/* third stage */
		x7 = x8 + x3;
		x8 -= x3;
		x3 = x0 + x2;
		x0 -= x2;
		x2 = (x4 + x5);
		x4 -= x5;

/* fourth stage */
		SATURATE(x7 + x1, blockPtr[0]);
		SATURATE(x3 + x2, blockPtr[1]);
		SATURATE(x0 + x4, blockPtr[2]);
		SATURATE(x8 + x6, blockPtr[3]);
		SATURATE(x8 - x6, blockPtr[4]);
		SATURATE(x0 - x4, blockPtr[5]);
		SATURATE(x3 - x2, blockPtr[6]);
		SATURATE(x7 - x1, blockPtr[7]);

		inPtr++;
		blockPtr += stride;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VideoIDCT8x8_S16_S16_Q1(
	mlib_s16 *block,
	const mlib_s16 *coeffs)
{
	const mlib_s16 *coeffPtr = coeffs;
	mlib_s16 *blockPtr = block;
	mlib_s64 workspace[64];
	mlib_s64 *workPtr = workspace;
	mlib_s64 x0, x1, x2, x3, x4, x5, x6, x7, x8;
	mlib_s32 str = 8;
	mlib_s32 i;

	IDCT1(coeffPtr, workPtr, 16384);

	workPtr = workspace;

	IDCT2(workPtr, blockPtr, 2048);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
#endif
