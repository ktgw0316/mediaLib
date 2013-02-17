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
 * Copyright 2005 Sun Microsystems, Inc.  All rights reserved.
 * Use is subject to license terms.
 */

#pragma ident	"@(#)mlib_i_VideoIDCT8x8_S16_S16_QM.c	9.2	07/11/05 SMI"

/*
 * FUNCTION
 *
 *    mlib_VideoIDCT8x8_S16_S16_Q1_Mismatch - Computes the inverse IDCT in
 *                                            the inter mode
 *
 * SYNOPSIS
 *
 *    mlib_status mlib_VideoIDCT8x8_S16_S16_Q1_Mismatch(mlib_s16       *block,
 *                                                      const mlib_s16 *coeffs)
 *
 * ARGUMENT
 *      block           Pointer to an 8x8 motion-compensated block which is the
 *                      difference between the reference block and current block
 *      coeffs          Pointer to the input DCT coefficients
 *
 *      It is assumed that block and coeffs are 8-byte aligned
 *      Input DCT coeffs should be in S12 range or it should be obtained
 *      from the correspondent direct DCT.
 *
 * DESCRIPTION
 *   function is similar to mlib_v_VideoIDCT8x8_S16_S16_Q1: it should only
 *   be used when coeffs[u][v] (u, v = 0...7) are non zero
 *   only for u and v less then 4. However, this function
 *   also allows element coeff[7][7] to be non-zero.
 *   The primary benefit of this modification is that it can handle
 *   situations where coeffs[7][7] has been made non-zero
 *   by MPEG mismatch-control, allowing a simplified version of the IDCT
 *   to be undertaken for a much larger number of situations.
 */

#include <mlib_video.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_VideoIDCT8x8_S16_S16_Q1_Mismatch = \
	__mlib_VideoIDCT8x8_S16_S16_Q1_Mismatch

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_VideoIDCT8x8_S16_S16_Q1_Mismatch)
	mlib_VideoIDCT8x8_S16_S16_Q1_Mismatch
	__attribute__((weak,
	alias("__mlib_VideoIDCT8x8_S16_S16_Q1_Mismatch")));

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
	    x7 = x0 + x3;                                       \
	    x8 = x0 - x3;                                       \
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
	}                                                       \
	                                                        \
/*                                                              \
 * 3 columns skip                                               \
 */                                                             \
	inPtr += 3;                                             \
	outPtr += 3 * str;                                      \
/*                                                              \
 * last column                                                  \
 */                                                             \
	if (inPtr[8 * 7] != 1) {                                \
	                                                        \
/*                                                              \
 * first stage                                                  \
 */                                                             \
	    x1 = COS_7_16 * inPtr[8 * 7];                       \
	    x6 = -COS_1_16 * inPtr[8 * 7];                      \
/*                                                              \
 * second stage                                                 \
 */                                                             \
	    x4 = COS_4_16 * x1;                                 \
	    x5 = COS_4_16 * x6;                                 \
/*                                                              \
 * third stage                                                  \
 */                                                             \
	    x2 = x4 + x5;                                       \
	    x4 = x4 - x5;                                       \
/*                                                              \
 * fourth stage                                                 \
 */                                                             \
	    outPtr[0] = x1;                                     \
	    outPtr[1] = x2;                                     \
	    outPtr[2] = x4;                                     \
	    outPtr[3] = x6;                                     \
	    outPtr[4] = -x6;                                    \
	    outPtr[5] = -x4;                                    \
	    outPtr[6] = -x2;                                    \
	    outPtr[7] = -x1;                                    \
	} else {                                                \
	    outPtr[0] = COS_7_16;                               \
	    outPtr[1] = COS_4_16 * (COS_7_16 - COS_1_16);       \
	    outPtr[2] = COS_4_16 * (COS_7_16 + COS_1_16);       \
	    outPtr[3] = -COS_1_16;                              \
	    outPtr[4] = COS_1_16;                               \
	    outPtr[5] = -COS_4_16 * (COS_7_16 + COS_1_16);      \
	    outPtr[6] = -COS_4_16 * (COS_7_16 - COS_1_16);      \
	    outPtr[7] = -COS_7_16;                              \
	}

/* *********************************************************** */

#define	IDCT2(inPtr, outPtr, csub)                                    \
	for (i = 0; i < 8; i++) {                                     \
	                                                              \
/*                                                                    \
 * first stage                                                        \
 */                                                                   \
	    x4 = COS_7_16 * inPtr[8 * 7] + COS_1_16 * inPtr[8 * 1];   \
	    x5 = COS_7_16 * inPtr[8 * 1] - COS_1_16 * inPtr[8 * 7];   \
	    x6 = COS_3_16 * inPtr[8 * 3];                             \
	    x7 = COS_5_16 * inPtr[8 * 3];                             \
/*                                                                    \
 * second stage                                                       \
 */                                                                   \
	    x8 = TWOSQRT2 * (inPtr[8 * 0]);                           \
	    x0 = TWOSQRT2 * (inPtr[8 * 0]);                           \
	    x2 = COS_6_16 * inPtr[8 * 2];                             \
	    x3 = COS_2_16 * inPtr[8 * 2];                             \
	    x1 = x4 + x6;                                             \
	    x4 = COS_4_16 * (x4 - x6);                                \
	    x6 = x5 - x7;                                             \
	    x5 = COS_4_16 * (x5 + x7);                                \
/*                                                                    \
 * third stage                                                        \
 */                                                                   \
	    x7 = x8 + x3;                                             \
	    x8 -= x3;                                                 \
	    x3 = x0 + x2;                                             \
	    x0 -= x2;                                                 \
	    x2 = (x4 + x5);                                           \
	    x4 -= x5;                                                 \
/*                                                                    \
 * fourth stage                                                       \
 */                                                                   \
	    outPtr[0] = (mlib_s32)(x7 + x1) - csub;                   \
	    outPtr[1] = (mlib_s32)(x3 + x2) - csub;                   \
	    outPtr[2] = (mlib_s32)(x0 + x4) - csub;                   \
	    outPtr[3] = (mlib_s32)(x8 + x6) - csub;                   \
	    outPtr[4] = (mlib_s32)(x8 - x6) - csub;                   \
	    outPtr[5] = (mlib_s32)(x0 - x4) - csub;                   \
	    outPtr[6] = (mlib_s32)(x3 - x2) - csub;                   \
	    outPtr[7] = (mlib_s32)(x7 - x1) - csub;                   \
	    inPtr++;                                                  \
	    outPtr += str;                                            \
	}

/* *********************************************************** */

mlib_status
__mlib_VideoIDCT8x8_S16_S16_Q1_Mismatch(
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

#define	TWOSQRT2	(370728L)
#define	COS_1_16	(514214L)
#define	COS_2_16	(484379L)
#define	COS_3_16	(435930L)
#define	COS_4_16	(741455L)
#define	COS_5_16	(291279L)
#define	COS_6_16	(200636L)
#define	COS_7_16	(102284L)
#define	Q2_COS_1_16	(363604L)
#define	Q2_COS_2_16	(342508L)
#define	Q2_COS_3_16	(308249L)
#define	Q2_COS_5_16	(205965L)
#define	Q2_COS_6_16	(141871L)
#define	Q2_COS_7_16	(72325L)


#define	ROUND(a)	((a + 0x8000000000) >> 40)

/* *********************************************************** */

#define	IDCT1(inPtr, outPtr, cadd)                              \
	x0 = inPtr[8 * 0] + 4 + cadd;                           \
	for (i = 0; i < 4; i++) {                               \
	                                                        \
/*                                                              \
 * first stage                                                  \
 */                                                             \
	    x4 = COS_1_16 * inPtr[8 * 1];                       \
	    x4a = Q2_COS_1_16 * inPtr[8 * 1];                   \
	    x5 = COS_7_16 * inPtr[8 * 1];                       \
	    x5a = Q2_COS_7_16 * inPtr[8 * 1];                   \
	    x6 = COS_3_16 * inPtr[8 * 3];                       \
	    x6a = Q2_COS_3_16 * inPtr[8 * 3];                   \
	    x7 = COS_5_16 * inPtr[8 * 3];                       \
	    x7a = Q2_COS_5_16 * inPtr[8 * 3];                   \
/*                                                              \
 * second stage                                                 \
 */                                                             \
	    x0 = TWOSQRT2 * (x0);                               \
	    x2 = COS_6_16 * inPtr[8 * 2];                       \
	    x3 = COS_2_16 * inPtr[8 * 2];                       \
	    x1 = x4 + x6;                                       \
	    x4 = x4a - x6a;                                     \
	    x6 = x5 - x7;                                       \
	    x5 = x5a + x7a;                                     \
/*                                                              \
 * third stage                                                  \
 */                                                             \
	    x7 = x0 + x3;                                       \
	    x8 = x0 - x3;                                       \
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
	}                                                       \
	                                                        \
/*                                                              \
 * 3 columns skip                                               \
 */                                                             \
	inPtr += 3;                                             \
	outPtr += 3 * str;                                      \
/*                                                              \
 * last column                                                  \
 */                                                             \
	if (inPtr[8 * 7] != 1) {                                \
	                                                        \
/*                                                              \
 * first stage                                                  \
 */                                                             \
	    x1 = COS_7_16 * inPtr[8 * 7];                       \
	    x6 = -COS_1_16 * inPtr[8 * 7];                      \
/*                                                              \
 * second stage                                                 \
 */                                                             \
	    x4 = Q2_COS_7_16 * inPtr[8 * 7];                    \
	    x5 = -Q2_COS_1_16 * inPtr[8 * 7];                   \
/*                                                              \
 * third stage                                                  \
 */                                                             \
	    x2 = x4 + x5;                                       \
	    x4 = x4 - x5;                                       \
/*                                                              \
 * fourth stage                                                 \
 */                                                             \
	    outPtr[0] = x1;                                     \
	    outPtr[1] = x2;                                     \
	    outPtr[2] = x4;                                     \
	    outPtr[3] = x6;                                     \
	    outPtr[4] = -x6;                                    \
	    outPtr[5] = -x4;                                    \
	    outPtr[6] = -x2;                                    \
	    outPtr[7] = -x1;                                    \
	} else {                                                \
	    outPtr[0] = COS_7_16;                               \
	    outPtr[1] = Q2_COS_7_16 - Q2_COS_1_16;              \
	    outPtr[2] = (Q2_COS_7_16 + Q2_COS_1_16);            \
	    outPtr[3] = -COS_1_16;                              \
	    outPtr[4] = COS_1_16;                               \
	    outPtr[5] = -Q2_COS_7_16 - Q2_COS_1_16;             \
	    outPtr[6] = -Q2_COS_7_16 + Q2_COS_1_16;             \
	    outPtr[7] = -COS_7_16;                              \
	}

/* *********************************************************** */

#define	IDCT2(inPtr, outPtr, csub)                                    \
	for (i = 0; i < 8; i++) {                                     \
	                                                              \
/*                                                                    \
 * first stage                                                        \
 */                                                                   \
	    x4 = COS_7_16 * inPtr[8 * 7] + COS_1_16 * inPtr[8 * 1];   \
	    x4a = Q2_COS_7_16 * inPtr[8 * 7]                          \
		+ Q2_COS_1_16 * inPtr[8 * 1];                         \
	    x5 = COS_7_16 * inPtr[8 * 1] - COS_1_16 * inPtr[8 * 7];   \
	    x5a = Q2_COS_7_16 * inPtr[8 * 1]                          \
		- Q2_COS_1_16 * inPtr[8 * 7];                         \
	    x6 = COS_3_16 * inPtr[8 * 3];                             \
	    x6a = Q2_COS_3_16 * inPtr[8 * 3];                         \
	    x7 = COS_5_16 * inPtr[8 * 3];                             \
	    x7a = Q2_COS_5_16 * inPtr[8 * 3];                         \
/*                                                                    \
 * second stage                                                       \
 */                                                                   \
	    x8 = TWOSQRT2 * (inPtr[8 * 0]);                           \
	    x0 = TWOSQRT2 * (inPtr[8 * 0]);                           \
	    x2 = COS_6_16 * inPtr[8 * 2];                             \
	    x3 = COS_2_16 * inPtr[8 * 2];                             \
	    x1 = x4 + x6;                                             \
	    x4 = (x4a - x6a);                                         \
	    x6 = x5 - x7;                                             \
	    x5 = (x5a + x7a);                                         \
/*                                                                    \
 * third stage                                                        \
 */                                                                   \
	    x7 = x8 + x3;                                             \
	    x8 -= x3;                                                 \
	    x3 = x0 + x2;                                             \
	    x0 -= x2;                                                 \
	    x2 = (x4 + x5);                                           \
	    x4 -= x5;                                                 \
/*                                                                    \
 * fourth stage                                                       \
 */                                                                   \
	    outPtr[0] = (mlib_s32)(ROUND(x7 + x1)) - csub;            \
	    outPtr[1] = (mlib_s32)(ROUND(x3 + x2)) - csub;            \
	    outPtr[2] = (mlib_s32)(ROUND(x0 + x4)) - csub;            \
	    outPtr[3] = (mlib_s32)(ROUND(x8 + x6)) - csub;            \
	    outPtr[4] = (mlib_s32)(ROUND(x8 - x6)) - csub;            \
	    outPtr[5] = (mlib_s32)(ROUND(x0 - x4)) - csub;            \
	    outPtr[6] = (mlib_s32)(ROUND(x3 - x2)) - csub;            \
	    outPtr[7] = (mlib_s32)(ROUND(x7 - x1)) - csub;            \
	    inPtr++;                                                  \
	    outPtr += str;                                            \
	}

/* *********************************************************** */

mlib_status
__mlib_VideoIDCT8x8_S16_S16_Q1_Mismatch(
	mlib_s16 *block,
	const mlib_s16 *coeffs)
{
	const mlib_s16 *coeffPtr = coeffs;
	mlib_s16 *blockPtr = block;
	mlib_s64 workspace[64];
	mlib_s64 *workPtr = workspace;
	mlib_s64 x0, x1, x2, x3, x4, x5, x6, x7, x8, x4a, x5a, x6a, x7a;
	mlib_s32 str = 8;
	mlib_s32 i;

	IDCT1(coeffPtr, workPtr, 16384);

	workPtr = workspace;

	IDCT2(workPtr, blockPtr, 2048);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
#endif
