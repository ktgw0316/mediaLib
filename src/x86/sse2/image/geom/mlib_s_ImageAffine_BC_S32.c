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

#pragma ident	"@(#)mlib_s_ImageAffine_BC_S32.c	9.2	07/11/05 SMI"

/*
 * FUNCTION
 *      Image affine transformation with Bicubic filtering
 * SYNOPSIS
 *      mlib_status mlib_ImageAffine_[s32|f32|d64]_?ch_bc
 *                                  (mlib_s32 *leftEdges,
 *                                   mlib_s32 *rightEdges,
 *                                   mlib_s32 *xStarts,
 *                                   mlib_s32 *yStarts,
 *                                   mlib_s32 *sides,
 *                                   mlib_u8  *dstData,
 *                                   mlib_u8  **lineAddr,
 *                                   mlib_s32 dstYStride,
 *                                   mlib_s32 is_affine,
 *                                   mlib_s32 srcYStride,
 *                                   mlib_filter filter)
 *
 * ARGUMENTS
 *      leftEdges  array[dstHeight] of xLeft coordinates
 *      RightEdges array[dstHeight] of xRight coordinates
 *      xStarts    array[dstHeight] of xStart * 65536 coordinates
 *      yStarts    array[dstHeight] of yStart * 65536 coordinates
 *      sides      output array[4]. sides[0] is yStart, sides[1] is yFinish,
 *                 sides[2] is dx * 65536, sides[3] is dy * 65536
 *      dstData    pointer to the first pixel on (yStart - 1) line
 *      lineAddr   array[srcHeight] of pointers to the first pixel on
 *                 the corresponding lines
 *      dstYStride stride of destination image
 *      is_affine  indicator (Affine - GridWarp)
 *      srcYStride stride of source image
 *      filter     type of resampling filter
 *
 * DESCRIPTION
 *      The functions step along the lines from xLeft to xRight and apply
 *      the Bicubic and Bicubic2 filtering.
 *
 */

#include <mlib_ImageAffine.h>
#ifdef	__SUNPRO_C
#include <sunmedia_intrin.h>
#else
#include <emmintrin.h>
#endif

#define	IMG_TYPE	3

/* *********************************************************** */

#if IMG_TYPE == 3

/* *********************************************************** */

#define	DTYPE	mlib_s32
#define	FTYPE	mlib_d64

#define	FUN_NAME(CHAN)	mlib_ImageAffine_s32_##CHAN##_bc

#define	STORE(res, x)	SAT32(res)

#elif IMG_TYPE == 4	/* IMG_TYPE == 3 */

/* *********************************************************** */

#define	DTYPE	mlib_f32
#define	FTYPE	DTYPE

#define	FUN_NAME(CHAN)	mlib_ImageAffine_f32_##CHAN##_bc

#define	STORE(res, x)	res = (x)

#elif IMG_TYPE == 5	/* IMG_TYPE == 3 */

/* *********************************************************** */

#define	DTYPE	mlib_d64
#define	FTYPE	DTYPE

#define	FUN_NAME(CHAN)	mlib_ImageAffine_d64_##CHAN##_bc

#define	STORE(res, x)	res = (x)

#endif /* IMG_TYPE == 3 */

/* *********************************************************** */

#define	DUMMY

#define	OPR	val0 = (c0 * yf0 + c1 * yf1 + c2 * yf2 + c3 * yf3);

#define	CREATE_COEF_BICUBIC(X, Y, OPERATOR)                     \
	dx = (X & MLIB_MASK) * scale;                           \
	dy = (Y & MLIB_MASK) * scale;                           \
	dx_2 = ((FTYPE) 0.5) * dx;                              \
	dy_2 = ((FTYPE) 0.5) * dy;                              \
	dx2 = dx * dx;                                          \
	dy2 = dy * dy;                                          \
	dx3_2 = dx_2 * dx2;                                     \
	dy3_2 = dy_2 * dy2;                                     \
	dx3_3 = ((FTYPE) 3.0) * dx3_2;                          \
	dy3_3 = ((FTYPE) 3.0) * dy3_2;                          \
	xf0 = dx2 - dx3_2 - dx_2;                               \
	xf1 = dx3_3 - ((FTYPE) 2.5) * dx2 + ((FTYPE) 1.0);      \
	xf2 = ((FTYPE) 2.0) * dx2 - dx3_3 + dx_2;               \
	xf3 = dx3_2 - ((FTYPE) 0.5) * dx2;                      \
	OPERATOR                                                \
	yf0 = dy2 - dy3_2 - dy_2;                               \
	yf1 = dy3_3 - ((FTYPE) 2.5) * dy2 + ((FTYPE) 1.0);      \
	yf2 = ((FTYPE) 2.0) * dy2 - dy3_3 + dy_2;               \
	yf3 = dy3_2 - ((FTYPE) 0.5) * dy2

/* *********************************************************** */

#define	CREATE_COEF_BICUBIC_2(X, Y, OPERATOR)                   \
	dx = (X & MLIB_MASK) * scale;                           \
	dy = (Y & MLIB_MASK) * scale;                           \
	dx2 = dx * dx;                                          \
	dy2 = dy * dy;                                          \
	dx3_2 = dx * dx2;                                       \
	dy3_2 = dy * dy2;                                       \
	dx3_3 = ((FTYPE) 2.0) * dx2;                            \
	dy3_3 = ((FTYPE) 2.0) * dy2;                            \
	xf0 = dx3_3 - dx3_2 - dx;                               \
	xf1 = dx3_2 - dx3_3 + ((FTYPE) 1.0);                    \
	xf2 = dx2 - dx3_2 + dx;                                 \
	xf3 = dx3_2 - dx2;                                      \
	OPERATOR                                                \
	yf0 = dy3_3 - dy3_2 - dy;                               \
	yf1 = dy3_2 - dy3_3 + ((FTYPE) 1.0);                    \
	yf2 = dy2 - dy3_2 + dy;                                 \
	yf3 = dy3_2 - dy2

/* *********************************************************** */

#define	OPR_SSE2	val0 = (c0 * xf0 + c1 * xf1 + c2 * xf2 + c3 * xf3);

#define	CREATE_COEF_BICUBIC_SSE2(X, Y)                          \
	dx = (X & MLIB_MASK) * scale;                           \
	dy = (Y & MLIB_MASK) * scale;                           \
	dx_2 = ((FTYPE) 0.5) * dx;                              \
	dy_2 = ((FTYPE) 0.5) * dy;                              \
	dx2 = dx * dx;                                          \
	dy2 = dy * dy;                                          \
	dx3_2 = dx_2 * dx2;                                     \
	dy3_2 = dy_2 * dy2;                                     \
	dx3_3 = ((FTYPE) 3.0) * dx3_2;                          \
	dy3_3 = ((FTYPE) 3.0) * dy3_2;                          \
	xf0 = dx2 - dx3_2 - dx_2;                               \
	xf1 = dx3_3 - ((FTYPE) 2.5) * dx2 + ((FTYPE) 1.0);      \
	xf2 = ((FTYPE) 2.0) * dx2 - dx3_3 + dx_2;               \
	xf3 = dx3_2 - ((FTYPE) 0.5) * dx2;                      \
	yf0 = dy2 - dy3_2 - dy_2;                               \
	yf1 = dy3_3 - ((FTYPE) 2.5) * dy2 + ((FTYPE) 1.0);      \
	yf2 = ((FTYPE) 2.0) * dy2 - dy3_3 + dy_2;               \
	yf3 = dy3_2 - ((FTYPE) 0.5) * dy2

/* *********************************************************** */

#define	CREATE_COEF_BICUBIC_2_SSE2(X, Y)                        \
	dx = (X & MLIB_MASK) * scale;                           \
	dy = (Y & MLIB_MASK) * scale;                           \
	dx2 = dx * dx;                                          \
	dy2 = dy * dy;                                          \
	dx3_2 = dx * dx2;                                       \
	dy3_2 = dy * dy2;                                       \
	dx3_3 = ((FTYPE) 2.0) * dx2;                            \
	dy3_3 = ((FTYPE) 2.0) * dy2;                            \
	xf0 = dx3_3 - dx3_2 - dx;                               \
	xf1 = dx3_2 - dx3_3 + ((FTYPE) 1.0);                    \
	xf2 = dx2 - dx3_2 + dx;                                 \
	xf3 = dx3_2 - dx2;                                      \
	yf0 = dy3_3 - dy3_2 - dy;                               \
	yf1 = dy3_2 - dy3_3 + ((FTYPE) 1.0);                    \
	yf2 = dy2 - dy3_2 + dy;                                 \
	yf3 = dy3_2 - dy2

/* *********************************************************** */

mlib_status FUN_NAME(
	1ch) (
	mlib_affine_param *param)
{
	DECLAREVAR_BC();
	DTYPE *dstLineEnd;

	for (j = yStart; j <= yFinish; j++) {
		FTYPE xf0, xf1, xf2, xf3;
		FTYPE yf0, yf1, yf2, yf3;
		FTYPE dx, dx_2, dx2, dx3_2, dx3_3;
		FTYPE dy, dy_2, dy2, dy3_2, dy3_3;
		FTYPE c0, c1, c2, c3, val0;
		FTYPE scale = 1 / 65536.f;

		__m128i i0, i1, i2, i3;
		__m128 p0, p1, p2, p3;
		__m128 yp, yp0, yp1, yfp0, yfp1, yfp2, yfp3;
		__m128 sum, prod0, prod1, prod2, prod3;
		mlib_f32 *p32 = (mlib_f32 *)&sum;

		CLIP(1);
		dstLineEnd = (DTYPE *) dstData + xRight;

		if (filter == MLIB_BICUBIC) {
			for (; dstPixelPtr <= dstLineEnd; dstPixelPtr++) {
				CREATE_COEF_BICUBIC_SSE2(X, Y);

				xSrc = (X >> MLIB_SHIFT) - 1;
				ySrc = (Y >> MLIB_SHIFT) - 1;

				srcPixelPtr =
					((DTYPE **) lineAddr)[ySrc] + xSrc;
				i0 = _mm_loadu_si128((__m128i *)srcPixelPtr);

				srcPixelPtr =
					(DTYPE *) ((mlib_u8 *)srcPixelPtr +
					srcYStride);
				i1 = _mm_loadu_si128((__m128i *)srcPixelPtr);

				X += dX;
				Y += dY;

				srcPixelPtr =
					(DTYPE *) ((mlib_u8 *)srcPixelPtr +
					srcYStride);
				i2 = _mm_loadu_si128((__m128i *)srcPixelPtr);
				srcPixelPtr =
					(DTYPE *) ((mlib_u8 *)srcPixelPtr +
					srcYStride);
				i3 = _mm_loadu_si128((__m128i *)srcPixelPtr);

				p0 = _mm_cvtepi32_ps(i0);
				p1 = _mm_cvtepi32_ps(i1);
				p2 = _mm_cvtepi32_ps(i2);
				p3 = _mm_cvtepi32_ps(i3);

				yfp0 = _mm_set1_ps(yf0);
				yfp1 = _mm_set1_ps(yf1);
				yfp2 = _mm_set1_ps(yf2);
				yfp3 = _mm_set1_ps(yf3);

				prod0 = _mm_mul_ps(yfp0, p0);
				prod1 = _mm_mul_ps(yfp1, p1);
				prod2 = _mm_mul_ps(yfp2, p2);
				prod3 = _mm_mul_ps(yfp3, p3);
				prod0 = _mm_add_ps(prod0, prod1);
				prod2 = _mm_add_ps(prod2, prod3);
				sum = _mm_add_ps(prod0, prod2);

				c0 = p32[0];
				c1 = p32[1];
				c2 = p32[2];
				c3 = p32[3];

				OPR_SSE2;

				STORE(dstPixelPtr[0], val0);

			}
		} else {
			for (; dstPixelPtr <= dstLineEnd; dstPixelPtr++) {
				CREATE_COEF_BICUBIC_2_SSE2(X, Y);

				xSrc = (X >> MLIB_SHIFT) - 1;
				ySrc = (Y >> MLIB_SHIFT) - 1;

				srcPixelPtr =
					((DTYPE **) lineAddr)[ySrc] + xSrc;
				i0 = _mm_loadu_si128((__m128i *)srcPixelPtr);

				srcPixelPtr =
					(DTYPE *) ((mlib_u8 *)srcPixelPtr +
					srcYStride);
				i1 = _mm_loadu_si128((__m128i *)srcPixelPtr);

				X += dX;
				Y += dY;

				srcPixelPtr =
					(DTYPE *) ((mlib_u8 *)srcPixelPtr +
					srcYStride);
				i2 = _mm_loadu_si128((__m128i *)srcPixelPtr);
				srcPixelPtr =
					(DTYPE *) ((mlib_u8 *)srcPixelPtr +
					srcYStride);
				i3 = _mm_loadu_si128((__m128i *)srcPixelPtr);

				p0 = _mm_cvtepi32_ps(i0);
				p1 = _mm_cvtepi32_ps(i1);
				p2 = _mm_cvtepi32_ps(i2);
				p3 = _mm_cvtepi32_ps(i3);

				yfp0 = _mm_set1_ps(yf0);
				yfp1 = _mm_set1_ps(yf1);
				yfp2 = _mm_set1_ps(yf2);
				yfp3 = _mm_set1_ps(yf3);

				prod0 = _mm_mul_ps(yfp0, p0);
				prod1 = _mm_mul_ps(yfp1, p1);
				prod2 = _mm_mul_ps(yfp2, p2);
				prod3 = _mm_mul_ps(yfp3, p3);
				prod0 = _mm_add_ps(prod0, prod1);
				prod2 = _mm_add_ps(prod2, prod3);
				sum = _mm_add_ps(prod0, prod2);

				c0 = p32[0];
				c1 = p32[1];
				c2 = p32[2];
				c3 = p32[3];

				OPR_SSE2;

				STORE(dstPixelPtr[0], val0);

			}
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
#define	INTERLEAVE(i0, i1)                                            \
	{                                                             \
		__m128i tmp0, tmp1;                                   \
		tmp0 = _mm_unpacklo_epi32(i0, i1);                    \
		tmp1 = _mm_unpackhi_epi32(i0, i1);                    \
		i0 = _mm_unpacklo_epi32(tmp0, tmp1);                  \
		i1 = _mm_unpackhi_epi32(tmp0, tmp1);                  \
	}

mlib_status FUN_NAME(
	2ch) (
	mlib_affine_param *param)
{
	DECLAREVAR_BC();
	DTYPE *dstLineEnd;

	for (j = yStart; j <= yFinish; j++) {
		FTYPE xf0, xf1, xf2, xf3;
		FTYPE yf0, yf1, yf2, yf3;
		FTYPE dx, dx_2, dx2, dx3_2, dx3_3;
		FTYPE dy, dy_2, dy2, dy3_2, dy3_3;
		FTYPE c0, c1, c2, c3, val0;
		FTYPE scale = 1 / 65536.f;

		__m128i i00, i01, i02, i03;
		__m128 p00, p01, p02, p03;
		__m128i i10, i11, i12, i13;
		__m128 p10, p11, p12, p13;
		__m128 yp, yp0, yp1, yfp0, yfp1, yfp2, yfp3;
		__m128 sum0, prod00, prod01, prod02, prod03;
		__m128 sum1, prod10, prod11, prod12, prod13;
		mlib_f32 *p32_0 = (mlib_f32 *)&sum0;
		mlib_f32 *p32_1 = (mlib_f32 *)&sum1;

		CLIP(2);
		dstLineEnd = (DTYPE *) dstData + 2 * xRight;

		mlib_s32 X1 = X;
		mlib_s32 Y1 = Y;
		DTYPE *dPtr = dstPixelPtr;

		if (filter == MLIB_BICUBIC) {
			for (; dPtr <= (dstLineEnd + 1); dPtr += 2) {
				CREATE_COEF_BICUBIC_SSE2(X1, Y1);
				xSrc = (X1 >> MLIB_SHIFT) - 1;
				ySrc = (Y1 >> MLIB_SHIFT) - 1;

				srcPixelPtr =
					((DTYPE **) lineAddr)[ySrc] +
					2 * xSrc;
#if 0
				i00 = _mm_setr_epi32(
						srcPixelPtr[0],
						srcPixelPtr[2],
						srcPixelPtr[4],
						srcPixelPtr[6]);
				i10 = _mm_setr_epi32(
						srcPixelPtr[1],
						srcPixelPtr[3],
						srcPixelPtr[5],
						srcPixelPtr[7]);
#else
				i00 = _mm_loadu_si128((__m128i *)srcPixelPtr);
				i10 = _mm_loadu_si128((__m128i *)srcPixelPtr
						+ 1);
				INTERLEAVE(i00, i10)
#endif

				srcPixelPtr =
					(DTYPE *) ((mlib_u8 *)
					srcPixelPtr + srcYStride);
				i01 = _mm_loadu_si128((__m128i *)srcPixelPtr);
				i11 = _mm_loadu_si128((__m128i *)srcPixelPtr
						+ 1);
				INTERLEAVE(i01, i11)

				X1 += dX;
				Y1 += dY;

				srcPixelPtr =
					(DTYPE *) ((mlib_u8 *)
					srcPixelPtr + srcYStride);
				i02 = _mm_loadu_si128((__m128i *)srcPixelPtr);
				i12 = _mm_loadu_si128((__m128i *)srcPixelPtr
						+ 1);
				INTERLEAVE(i02, i12)

				srcPixelPtr =
					(DTYPE *) ((mlib_u8 *)
					srcPixelPtr + srcYStride);
				i03 = _mm_loadu_si128((__m128i *)srcPixelPtr);
				i13 = _mm_loadu_si128((__m128i *)srcPixelPtr
						+ 1);
				INTERLEAVE(i03, i13)

				p00 = _mm_cvtepi32_ps(i00);
				p01 = _mm_cvtepi32_ps(i01);
				p02 = _mm_cvtepi32_ps(i02);
				p03 = _mm_cvtepi32_ps(i03);
				p10 = _mm_cvtepi32_ps(i10);
				p11 = _mm_cvtepi32_ps(i11);
				p12 = _mm_cvtepi32_ps(i12);
				p13 = _mm_cvtepi32_ps(i13);

				yfp0 = _mm_set1_ps(yf0);
				yfp1 = _mm_set1_ps(yf1);
				yfp2 = _mm_set1_ps(yf2);
				yfp3 = _mm_set1_ps(yf3);

				prod00 = _mm_mul_ps(yfp0, p00);
				prod01 = _mm_mul_ps(yfp1, p01);
				prod02 = _mm_mul_ps(yfp2, p02);
				prod03 = _mm_mul_ps(yfp3, p03);
				prod00 = _mm_add_ps(prod00, prod01);
				prod02 = _mm_add_ps(prod02, prod03);
				sum0 = _mm_add_ps(prod00, prod02);

				prod10 = _mm_mul_ps(yfp0, p10);
				prod11 = _mm_mul_ps(yfp1, p11);
				prod12 = _mm_mul_ps(yfp2, p12);
				prod13 = _mm_mul_ps(yfp3, p13);
				prod10 = _mm_add_ps(prod10, prod11);
				prod12 = _mm_add_ps(prod12, prod13);
				sum1 = _mm_add_ps(prod10, prod12);

				c0 = p32_0[0];
				c1 = p32_0[1];
				c2 = p32_0[2];
				c3 = p32_0[3];
				OPR_SSE2;
				STORE(dPtr[0], val0);

				c0 = p32_1[0];
				c1 = p32_1[1];
				c2 = p32_1[2];
				c3 = p32_1[3];
				OPR_SSE2;
				STORE(dPtr[1], val0);
			}
		} else {
			for (; dPtr <= (dstLineEnd + 1); dPtr += 2) {
				CREATE_COEF_BICUBIC_2_SSE2(X1, Y1);
				xSrc = (X1 >> MLIB_SHIFT) - 1;
				ySrc = (Y1 >> MLIB_SHIFT) - 1;

				srcPixelPtr =
					((DTYPE **) lineAddr)[ySrc] +
					2 * xSrc;
				i00 = _mm_loadu_si128((__m128i *)srcPixelPtr);
				i10 = _mm_loadu_si128((__m128i *)srcPixelPtr
						+ 1);
				INTERLEAVE(i00, i10)

				srcPixelPtr =
					(DTYPE *) ((mlib_u8 *)
					srcPixelPtr + srcYStride);
				i01 = _mm_loadu_si128((__m128i *)srcPixelPtr);
				i11 = _mm_loadu_si128((__m128i *)srcPixelPtr
						+ 1);
				INTERLEAVE(i01, i11)

				X1 += dX;
				Y1 += dY;

				srcPixelPtr =
					(DTYPE *) ((mlib_u8 *)
					srcPixelPtr + srcYStride);
				i02 = _mm_loadu_si128((__m128i *)srcPixelPtr);
				i12 = _mm_loadu_si128((__m128i *)srcPixelPtr
						+ 1);
				INTERLEAVE(i02, i12)

				srcPixelPtr =
					(DTYPE *) ((mlib_u8 *)
					srcPixelPtr + srcYStride);
				i03 = _mm_loadu_si128((__m128i *)srcPixelPtr);
				i13 = _mm_loadu_si128((__m128i *)srcPixelPtr
						+ 1);
				INTERLEAVE(i03, i13)

				p00 = _mm_cvtepi32_ps(i00);
				p01 = _mm_cvtepi32_ps(i01);
				p02 = _mm_cvtepi32_ps(i02);
				p03 = _mm_cvtepi32_ps(i03);
				p10 = _mm_cvtepi32_ps(i10);
				p11 = _mm_cvtepi32_ps(i11);
				p12 = _mm_cvtepi32_ps(i12);
				p13 = _mm_cvtepi32_ps(i13);

				yfp0 = _mm_set1_ps(yf0);
				yfp1 = _mm_set1_ps(yf1);
				yfp2 = _mm_set1_ps(yf2);
				yfp3 = _mm_set1_ps(yf3);

				prod00 = _mm_mul_ps(yfp0, p00);
				prod01 = _mm_mul_ps(yfp1, p01);
				prod02 = _mm_mul_ps(yfp2, p02);
				prod03 = _mm_mul_ps(yfp3, p03);
				prod00 = _mm_add_ps(prod00, prod01);
				prod02 = _mm_add_ps(prod02, prod03);
				sum0 = _mm_add_ps(prod00, prod02);

				prod10 = _mm_mul_ps(yfp0, p10);
				prod11 = _mm_mul_ps(yfp1, p11);
				prod12 = _mm_mul_ps(yfp2, p12);
				prod13 = _mm_mul_ps(yfp3, p13);
				prod10 = _mm_add_ps(prod10, prod11);
				prod12 = _mm_add_ps(prod12, prod13);
				sum1 = _mm_add_ps(prod10, prod12);

				c0 = p32_0[0];
				c1 = p32_0[1];
				c2 = p32_0[2];
				c3 = p32_0[3];
				OPR_SSE2;
				STORE(dPtr[0], val0);

				c0 = p32_1[0];
				c1 = p32_1[1];
				c2 = p32_1[2];
				c3 = p32_1[3];
				OPR_SSE2;
				STORE(dPtr[1], val0);
			}
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status FUN_NAME(
	3ch) (
	mlib_affine_param *param)
{
	DECLAREVAR_BC();
	DTYPE *dstLineEnd;

	for (j = yStart; j <= yFinish; j++) {
		FTYPE xf0, xf1, xf2, xf3;
		FTYPE yf0, yf1, yf2, yf3;
		FTYPE dx, dx_2, dx2, dx3_2, dx3_3;
		FTYPE dy, dy_2, dy2, dy3_2, dy3_3;
		FTYPE c0, c1, c2, c3, val0;
		FTYPE scale = 1 / 65536.f;
		FTYPE s0, s1, s2, s3;
		FTYPE s4, s5, s6, s7;

		__m128i i00, i01, i02, i03;
		__m128 p00, p01, p02, p03;
		__m128i i10, i11, i12, i13;
		__m128 p10, p11, p12, p13;
		__m128i i20, i21, i22, i23;
		__m128 p20, p21, p22, p23;
		__m128 yp, yp0, yp1, yfp0, yfp1, yfp2, yfp3;
		__m128 sum0, prod00, prod01, prod02, prod03;
		__m128 sum1, prod10, prod11, prod12, prod13;
		__m128 sum2, prod20, prod21, prod22, prod23;
		mlib_f32 *p32_0 = (mlib_f32 *)&sum0;
		mlib_f32 *p32_1 = (mlib_f32 *)&sum1;
		mlib_f32 *p32_2 = (mlib_f32 *)&sum2;

		CLIP(3);
		dstLineEnd = (DTYPE *) dstData + 3 * xRight;

		mlib_s32 X1 = X;
		mlib_s32 Y1 = Y;
		DTYPE *dPtr = dstPixelPtr;

		if (filter == MLIB_BICUBIC) {
			for (; dPtr <= dstLineEnd; dPtr += 3) {
				CREATE_COEF_BICUBIC_SSE2(X1, Y1);
				xSrc = (X1 >> MLIB_SHIFT) - 1;
				ySrc = (Y1 >> MLIB_SHIFT) - 1;

				srcPixelPtr =
					((DTYPE **) lineAddr)[ySrc] +
					3 * xSrc;
#if 1
				i00 = _mm_setr_epi32(
						srcPixelPtr[0],
						srcPixelPtr[3],
						srcPixelPtr[6],
						srcPixelPtr[9]);
				i10 = _mm_setr_epi32(
						srcPixelPtr[1],
						srcPixelPtr[4],
						srcPixelPtr[7],
						srcPixelPtr[10]);
				i20 = _mm_setr_epi32(
						srcPixelPtr[2],
						srcPixelPtr[5],
						srcPixelPtr[8],
						srcPixelPtr[11]);
#else
				i00 = _mm_loadu_si128((__m128i *)srcPixelPtr);
				i10 = _mm_loadu_si128((__m128i *)srcPixelPtr
						+ 1);
				INTERLEAVE(i00, i10)
#endif

				srcPixelPtr =
					(DTYPE *) ((mlib_u8 *)
					srcPixelPtr + srcYStride);
				i01 = _mm_setr_epi32(
						srcPixelPtr[0],
						srcPixelPtr[3],
						srcPixelPtr[6],
						srcPixelPtr[9]);
				i11 = _mm_setr_epi32(
						srcPixelPtr[1],
						srcPixelPtr[4],
						srcPixelPtr[7],
						srcPixelPtr[10]);
				i21 = _mm_setr_epi32(
						srcPixelPtr[2],
						srcPixelPtr[5],
						srcPixelPtr[8],
						srcPixelPtr[11]);

				X1 += dX;
				Y1 += dY;

				srcPixelPtr =
					(DTYPE *) ((mlib_u8 *)
					srcPixelPtr + srcYStride);
				i02 = _mm_setr_epi32(
						srcPixelPtr[0],
						srcPixelPtr[3],
						srcPixelPtr[6],
						srcPixelPtr[9]);
				i12 = _mm_setr_epi32(
						srcPixelPtr[1],
						srcPixelPtr[4],
						srcPixelPtr[7],
						srcPixelPtr[10]);
				i22 = _mm_setr_epi32(
						srcPixelPtr[2],
						srcPixelPtr[5],
						srcPixelPtr[8],
						srcPixelPtr[11]);

				srcPixelPtr =
					(DTYPE *) ((mlib_u8 *)
					srcPixelPtr + srcYStride);
				i03 = _mm_setr_epi32(
						srcPixelPtr[0],
						srcPixelPtr[3],
						srcPixelPtr[6],
						srcPixelPtr[9]);
				i13 = _mm_setr_epi32(
						srcPixelPtr[1],
						srcPixelPtr[4],
						srcPixelPtr[7],
						srcPixelPtr[10]);
				i23 = _mm_setr_epi32(
						srcPixelPtr[2],
						srcPixelPtr[5],
						srcPixelPtr[8],
						srcPixelPtr[11]);

				p00 = _mm_cvtepi32_ps(i00);
				p01 = _mm_cvtepi32_ps(i01);
				p02 = _mm_cvtepi32_ps(i02);
				p03 = _mm_cvtepi32_ps(i03);
				p10 = _mm_cvtepi32_ps(i10);
				p11 = _mm_cvtepi32_ps(i11);
				p12 = _mm_cvtepi32_ps(i12);
				p13 = _mm_cvtepi32_ps(i13);
				p20 = _mm_cvtepi32_ps(i20);
				p21 = _mm_cvtepi32_ps(i21);
				p22 = _mm_cvtepi32_ps(i22);
				p23 = _mm_cvtepi32_ps(i23);

				yfp0 = _mm_set1_ps(yf0);
				yfp1 = _mm_set1_ps(yf1);
				yfp2 = _mm_set1_ps(yf2);
				yfp3 = _mm_set1_ps(yf3);

				prod00 = _mm_mul_ps(yfp0, p00);
				prod01 = _mm_mul_ps(yfp1, p01);
				prod02 = _mm_mul_ps(yfp2, p02);
				prod03 = _mm_mul_ps(yfp3, p03);
				prod00 = _mm_add_ps(prod00, prod01);
				prod02 = _mm_add_ps(prod02, prod03);
				sum0 = _mm_add_ps(prod00, prod02);

				prod10 = _mm_mul_ps(yfp0, p10);
				prod11 = _mm_mul_ps(yfp1, p11);
				prod12 = _mm_mul_ps(yfp2, p12);
				prod13 = _mm_mul_ps(yfp3, p13);
				prod10 = _mm_add_ps(prod10, prod11);
				prod12 = _mm_add_ps(prod12, prod13);
				sum1 = _mm_add_ps(prod10, prod12);

				prod20 = _mm_mul_ps(yfp0, p20);
				prod21 = _mm_mul_ps(yfp1, p21);
				prod22 = _mm_mul_ps(yfp2, p22);
				prod23 = _mm_mul_ps(yfp3, p23);
				prod20 = _mm_add_ps(prod20, prod21);
				prod22 = _mm_add_ps(prod22, prod23);
				sum2 = _mm_add_ps(prod20, prod22);

				c0 = p32_0[0];
				c1 = p32_0[1];
				c2 = p32_0[2];
				c3 = p32_0[3];
				OPR_SSE2;
				STORE(dPtr[0], val0);

				c0 = p32_1[0];
				c1 = p32_1[1];
				c2 = p32_1[2];
				c3 = p32_1[3];
				OPR_SSE2;
				STORE(dPtr[1], val0);

				c0 = p32_2[0];
				c1 = p32_2[1];
				c2 = p32_2[2];
				c3 = p32_2[3];
				OPR_SSE2;
				STORE(dPtr[2], val0);
			}
		} else {
			for (; dPtr <= dstLineEnd; dPtr += 3) {
				CREATE_COEF_BICUBIC_2_SSE2(X1, Y1);
				xSrc = (X1 >> MLIB_SHIFT) - 1;
				ySrc = (Y1 >> MLIB_SHIFT) - 1;

				srcPixelPtr =
					((DTYPE **) lineAddr)[ySrc] +
					3 * xSrc;
				i00 = _mm_setr_epi32(
						srcPixelPtr[0],
						srcPixelPtr[3],
						srcPixelPtr[6],
						srcPixelPtr[9]);
				i10 = _mm_setr_epi32(
						srcPixelPtr[1],
						srcPixelPtr[4],
						srcPixelPtr[7],
						srcPixelPtr[10]);
				i20 = _mm_setr_epi32(
						srcPixelPtr[2],
						srcPixelPtr[5],
						srcPixelPtr[8],
						srcPixelPtr[11]);

				srcPixelPtr =
					(DTYPE *) ((mlib_u8 *)
					srcPixelPtr + srcYStride);
				i01 = _mm_setr_epi32(
						srcPixelPtr[0],
						srcPixelPtr[3],
						srcPixelPtr[6],
						srcPixelPtr[9]);
				i11 = _mm_setr_epi32(
						srcPixelPtr[1],
						srcPixelPtr[4],
						srcPixelPtr[7],
						srcPixelPtr[10]);
				i21 = _mm_setr_epi32(
						srcPixelPtr[2],
						srcPixelPtr[5],
						srcPixelPtr[8],
						srcPixelPtr[11]);

				X1 += dX;
				Y1 += dY;

				srcPixelPtr =
					(DTYPE *) ((mlib_u8 *)
					srcPixelPtr + srcYStride);
				i02 = _mm_setr_epi32(
						srcPixelPtr[0],
						srcPixelPtr[3],
						srcPixelPtr[6],
						srcPixelPtr[9]);
				i12 = _mm_setr_epi32(
						srcPixelPtr[1],
						srcPixelPtr[4],
						srcPixelPtr[7],
						srcPixelPtr[10]);
				i22 = _mm_setr_epi32(
						srcPixelPtr[2],
						srcPixelPtr[5],
						srcPixelPtr[8],
						srcPixelPtr[11]);

				srcPixelPtr =
					(DTYPE *) ((mlib_u8 *)
					srcPixelPtr + srcYStride);
				i03 = _mm_setr_epi32(
						srcPixelPtr[0],
						srcPixelPtr[3],
						srcPixelPtr[6],
						srcPixelPtr[9]);
				i13 = _mm_setr_epi32(
						srcPixelPtr[1],
						srcPixelPtr[4],
						srcPixelPtr[7],
						srcPixelPtr[10]);
				i23 = _mm_setr_epi32(
						srcPixelPtr[2],
						srcPixelPtr[5],
						srcPixelPtr[8],
						srcPixelPtr[11]);

				p00 = _mm_cvtepi32_ps(i00);
				p01 = _mm_cvtepi32_ps(i01);
				p02 = _mm_cvtepi32_ps(i02);
				p03 = _mm_cvtepi32_ps(i03);
				p10 = _mm_cvtepi32_ps(i10);
				p11 = _mm_cvtepi32_ps(i11);
				p12 = _mm_cvtepi32_ps(i12);
				p13 = _mm_cvtepi32_ps(i13);
				p20 = _mm_cvtepi32_ps(i20);
				p21 = _mm_cvtepi32_ps(i21);
				p22 = _mm_cvtepi32_ps(i22);
				p23 = _mm_cvtepi32_ps(i23);

				yfp0 = _mm_set1_ps(yf0);
				yfp1 = _mm_set1_ps(yf1);
				yfp2 = _mm_set1_ps(yf2);
				yfp3 = _mm_set1_ps(yf3);

				prod00 = _mm_mul_ps(yfp0, p00);
				prod01 = _mm_mul_ps(yfp1, p01);
				prod02 = _mm_mul_ps(yfp2, p02);
				prod03 = _mm_mul_ps(yfp3, p03);
				prod00 = _mm_add_ps(prod00, prod01);
				prod02 = _mm_add_ps(prod02, prod03);
				sum0 = _mm_add_ps(prod00, prod02);

				prod10 = _mm_mul_ps(yfp0, p10);
				prod11 = _mm_mul_ps(yfp1, p11);
				prod12 = _mm_mul_ps(yfp2, p12);
				prod13 = _mm_mul_ps(yfp3, p13);
				prod10 = _mm_add_ps(prod10, prod11);
				prod12 = _mm_add_ps(prod12, prod13);
				sum1 = _mm_add_ps(prod10, prod12);

				prod20 = _mm_mul_ps(yfp0, p20);
				prod21 = _mm_mul_ps(yfp1, p21);
				prod22 = _mm_mul_ps(yfp2, p22);
				prod23 = _mm_mul_ps(yfp3, p23);
				prod20 = _mm_add_ps(prod20, prod21);
				prod22 = _mm_add_ps(prod22, prod23);
				sum2 = _mm_add_ps(prod20, prod22);

				c0 = p32_0[0];
				c1 = p32_0[1];
				c2 = p32_0[2];
				c3 = p32_0[3];
				OPR_SSE2;
				STORE(dPtr[0], val0);

				c0 = p32_1[0];
				c1 = p32_1[1];
				c2 = p32_1[2];
				c3 = p32_1[3];
				OPR_SSE2;
				STORE(dPtr[1], val0);

				c0 = p32_2[0];
				c1 = p32_2[1];
				c2 = p32_2[2];
				c3 = p32_2[3];
				OPR_SSE2;
				STORE(dPtr[2], val0);

			}
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
#define		TRANSPOSE(i0, i1, i2, i3)                           \
	{                                                           \
		__m128i tmp0, tmp1, tmp2, tmp3;                     \
		tmp0 = _mm_unpacklo_epi32(i0, i2);                  \
		tmp1 = _mm_unpackhi_epi32(i0, i2);                  \
		tmp2 = _mm_unpacklo_epi32(i1, i3);                  \
		tmp3 = _mm_unpackhi_epi32(i1, i3);                  \
		i0 = _mm_unpacklo_epi32(tmp0, tmp2);                \
		i1 = _mm_unpackhi_epi32(tmp0, tmp2);                \
		i2 = _mm_unpacklo_epi32(tmp1, tmp3);                \
		i3 = _mm_unpackhi_epi32(tmp1, tmp3);                \
	}

mlib_status FUN_NAME(
	4ch) (
	mlib_affine_param *param)
{
	DECLAREVAR_BC();
	DTYPE *dstLineEnd;

	for (j = yStart; j <= yFinish; j++) {
		FTYPE xf0, xf1, xf2, xf3;
		FTYPE yf0, yf1, yf2, yf3;
		FTYPE dx, dx_2, dx2, dx3_2, dx3_3;
		FTYPE dy, dy_2, dy2, dy3_2, dy3_3;
		FTYPE c0, c1, c2, c3, val0;
		FTYPE scale = 1 / 65536.f;
		FTYPE s0, s1, s2, s3;
		FTYPE s4, s5, s6, s7;

		__m128i i00, i01, i02, i03;
		__m128 p00, p01, p02, p03;
		__m128i i10, i11, i12, i13;
		__m128 p10, p11, p12, p13;
		__m128i i20, i21, i22, i23;
		__m128 p20, p21, p22, p23;
		__m128i i30, i31, i32, i33;
		__m128 p30, p31, p32, p33;
		__m128 yp, yp0, yp1, yfp0, yfp1, yfp2, yfp3;
		__m128 sum0, prod00, prod01, prod02, prod03;
		__m128 sum1, prod10, prod11, prod12, prod13;
		__m128 sum2, prod20, prod21, prod22, prod23;
		__m128 sum3, prod30, prod31, prod32, prod33;
		mlib_f32 *p32_0 = (mlib_f32 *)&sum0;
		mlib_f32 *p32_1 = (mlib_f32 *)&sum1;
		mlib_f32 *p32_2 = (mlib_f32 *)&sum2;
		mlib_f32 *p32_3 = (mlib_f32 *)&sum3;

		CLIP(4);
		dstLineEnd = (DTYPE *) dstData + 4 * xRight;

		mlib_s32 X1 = X;
		mlib_s32 Y1 = Y;
		DTYPE *dPtr = dstPixelPtr;

		if (filter == MLIB_BICUBIC) {
			for (; dPtr <= dstLineEnd; dPtr += 4) {
				CREATE_COEF_BICUBIC_SSE2(X1, Y1);
				xSrc = (X1 >> MLIB_SHIFT) - 1;
				ySrc = (Y1 >> MLIB_SHIFT) - 1;

				srcPixelPtr =
					((DTYPE **) lineAddr)[ySrc] +
					4 * xSrc;
#if 0
				i00 = _mm_setr_epi32(
						srcPixelPtr[0],
						srcPixelPtr[4],
						srcPixelPtr[8],
						srcPixelPtr[12]);
				i10 = _mm_setr_epi32(
						srcPixelPtr[1],
						srcPixelPtr[5],
						srcPixelPtr[9],
						srcPixelPtr[13]);
				i20 = _mm_setr_epi32(
						srcPixelPtr[2],
						srcPixelPtr[6],
						srcPixelPtr[10],
						srcPixelPtr[14]);
				i30 = _mm_setr_epi32(
						srcPixelPtr[3],
						srcPixelPtr[7],
						srcPixelPtr[11],
						srcPixelPtr[15]);
#else
				i00 = _mm_loadu_si128((__m128i *)srcPixelPtr);
				i10 = _mm_loadu_si128((__m128i *)srcPixelPtr
						+ 1);
				i20 = _mm_loadu_si128((__m128i *)srcPixelPtr
						+ 2);
				i30 = _mm_loadu_si128((__m128i *)srcPixelPtr
						+ 3);
				TRANSPOSE(i00, i10, i20, i30)
#endif

				srcPixelPtr =
					(DTYPE *) ((mlib_u8 *)
					srcPixelPtr + srcYStride);
				i01 = _mm_loadu_si128((__m128i *)srcPixelPtr);
				i11 = _mm_loadu_si128((__m128i *)srcPixelPtr
						+ 1);
				i21 = _mm_loadu_si128((__m128i *)srcPixelPtr
						+ 2);
				i31 = _mm_loadu_si128((__m128i *)srcPixelPtr
						+ 3);
				TRANSPOSE(i01, i11, i21, i31)

				X1 += dX;
				Y1 += dY;

				srcPixelPtr =
					(DTYPE *) ((mlib_u8 *)
					srcPixelPtr + srcYStride);
				i02 = _mm_loadu_si128((__m128i *)srcPixelPtr);
				i12 = _mm_loadu_si128((__m128i *)srcPixelPtr
						+ 1);
				i22 = _mm_loadu_si128((__m128i *)srcPixelPtr
						+ 2);
				i32 = _mm_loadu_si128((__m128i *)srcPixelPtr
						+ 3);
				TRANSPOSE(i02, i12, i22, i32)

				srcPixelPtr =
					(DTYPE *) ((mlib_u8 *)
					srcPixelPtr + srcYStride);
				i03 = _mm_loadu_si128((__m128i *)srcPixelPtr);
				i13 = _mm_loadu_si128((__m128i *)srcPixelPtr
						+ 1);
				i23 = _mm_loadu_si128((__m128i *)srcPixelPtr
						+ 2);
				i33 = _mm_loadu_si128((__m128i *)srcPixelPtr
						+ 3);
				TRANSPOSE(i03, i13, i23, i33)

				p00 = _mm_cvtepi32_ps(i00);
				p01 = _mm_cvtepi32_ps(i01);
				p02 = _mm_cvtepi32_ps(i02);
				p03 = _mm_cvtepi32_ps(i03);
				p10 = _mm_cvtepi32_ps(i10);
				p11 = _mm_cvtepi32_ps(i11);
				p12 = _mm_cvtepi32_ps(i12);
				p13 = _mm_cvtepi32_ps(i13);
				p20 = _mm_cvtepi32_ps(i20);
				p21 = _mm_cvtepi32_ps(i21);
				p22 = _mm_cvtepi32_ps(i22);
				p23 = _mm_cvtepi32_ps(i23);
				p30 = _mm_cvtepi32_ps(i30);
				p31 = _mm_cvtepi32_ps(i31);
				p32 = _mm_cvtepi32_ps(i32);
				p33 = _mm_cvtepi32_ps(i33);

				yfp0 = _mm_set1_ps(yf0);
				yfp1 = _mm_set1_ps(yf1);
				yfp2 = _mm_set1_ps(yf2);
				yfp3 = _mm_set1_ps(yf3);

				prod00 = _mm_mul_ps(yfp0, p00);
				prod01 = _mm_mul_ps(yfp1, p01);
				prod02 = _mm_mul_ps(yfp2, p02);
				prod03 = _mm_mul_ps(yfp3, p03);
				prod00 = _mm_add_ps(prod00, prod01);
				prod02 = _mm_add_ps(prod02, prod03);
				sum0 = _mm_add_ps(prod00, prod02);

				prod10 = _mm_mul_ps(yfp0, p10);
				prod11 = _mm_mul_ps(yfp1, p11);
				prod12 = _mm_mul_ps(yfp2, p12);
				prod13 = _mm_mul_ps(yfp3, p13);
				prod10 = _mm_add_ps(prod10, prod11);
				prod12 = _mm_add_ps(prod12, prod13);
				sum1 = _mm_add_ps(prod10, prod12);

				prod20 = _mm_mul_ps(yfp0, p20);
				prod21 = _mm_mul_ps(yfp1, p21);
				prod22 = _mm_mul_ps(yfp2, p22);
				prod23 = _mm_mul_ps(yfp3, p23);
				prod20 = _mm_add_ps(prod20, prod21);
				prod22 = _mm_add_ps(prod22, prod23);
				sum2 = _mm_add_ps(prod20, prod22);

				prod30 = _mm_mul_ps(yfp0, p30);
				prod31 = _mm_mul_ps(yfp1, p31);
				prod32 = _mm_mul_ps(yfp2, p32);
				prod33 = _mm_mul_ps(yfp3, p33);
				prod30 = _mm_add_ps(prod30, prod31);
				prod32 = _mm_add_ps(prod32, prod33);
				sum3 = _mm_add_ps(prod30, prod32);

				c0 = p32_0[0];
				c1 = p32_0[1];
				c2 = p32_0[2];
				c3 = p32_0[3];
				OPR_SSE2;
				STORE(dPtr[0], val0);

				c0 = p32_1[0];
				c1 = p32_1[1];
				c2 = p32_1[2];
				c3 = p32_1[3];
				OPR_SSE2;
				STORE(dPtr[1], val0);

				c0 = p32_2[0];
				c1 = p32_2[1];
				c2 = p32_2[2];
				c3 = p32_2[3];
				OPR_SSE2;
				STORE(dPtr[2], val0);

				c0 = p32_3[0];
				c1 = p32_3[1];
				c2 = p32_3[2];
				c3 = p32_3[3];
				OPR_SSE2;
				STORE(dPtr[3], val0);
			}
		} else {
			for (; dPtr <= dstLineEnd; dPtr += 4) {
				CREATE_COEF_BICUBIC_2_SSE2(X1, Y1);
				xSrc = (X1 >> MLIB_SHIFT) - 1;
				ySrc = (Y1 >> MLIB_SHIFT) - 1;

				srcPixelPtr =
					((DTYPE **) lineAddr)[ySrc] +
					4 * xSrc;
				i00 = _mm_loadu_si128((__m128i *)srcPixelPtr);
				i10 = _mm_loadu_si128((__m128i *)srcPixelPtr
						+ 1);
				i20 = _mm_loadu_si128((__m128i *)srcPixelPtr
						+ 2);
				i30 = _mm_loadu_si128((__m128i *)srcPixelPtr
						+ 3);
				TRANSPOSE(i00, i10, i20, i30)

				srcPixelPtr =
					(DTYPE *) ((mlib_u8 *)
					srcPixelPtr + srcYStride);
				i01 = _mm_loadu_si128((__m128i *)srcPixelPtr);
				i11 = _mm_loadu_si128((__m128i *)srcPixelPtr
						+ 1);
				i21 = _mm_loadu_si128((__m128i *)srcPixelPtr
						+ 2);
				i31 = _mm_loadu_si128((__m128i *)srcPixelPtr
						+ 3);
				TRANSPOSE(i01, i11, i21, i31)

				X1 += dX;
				Y1 += dY;

				srcPixelPtr =
					(DTYPE *) ((mlib_u8 *)
					srcPixelPtr + srcYStride);
				i02 = _mm_loadu_si128((__m128i *)srcPixelPtr);
				i12 = _mm_loadu_si128((__m128i *)srcPixelPtr
						+ 1);
				i22 = _mm_loadu_si128((__m128i *)srcPixelPtr
						+ 2);
				i32 = _mm_loadu_si128((__m128i *)srcPixelPtr
						+ 3);
				TRANSPOSE(i02, i12, i22, i32)

				srcPixelPtr =
					(DTYPE *) ((mlib_u8 *)
					srcPixelPtr + srcYStride);
				i03 = _mm_loadu_si128((__m128i *)srcPixelPtr);
				i13 = _mm_loadu_si128((__m128i *)srcPixelPtr
						+ 1);
				i23 = _mm_loadu_si128((__m128i *)srcPixelPtr
						+ 2);
				i33 = _mm_loadu_si128((__m128i *)srcPixelPtr
						+ 3);
				TRANSPOSE(i03, i13, i23, i33)

				p00 = _mm_cvtepi32_ps(i00);
				p01 = _mm_cvtepi32_ps(i01);
				p02 = _mm_cvtepi32_ps(i02);
				p03 = _mm_cvtepi32_ps(i03);
				p10 = _mm_cvtepi32_ps(i10);
				p11 = _mm_cvtepi32_ps(i11);
				p12 = _mm_cvtepi32_ps(i12);
				p13 = _mm_cvtepi32_ps(i13);
				p20 = _mm_cvtepi32_ps(i20);
				p21 = _mm_cvtepi32_ps(i21);
				p22 = _mm_cvtepi32_ps(i22);
				p23 = _mm_cvtepi32_ps(i23);
				p30 = _mm_cvtepi32_ps(i30);
				p31 = _mm_cvtepi32_ps(i31);
				p32 = _mm_cvtepi32_ps(i32);
				p33 = _mm_cvtepi32_ps(i33);

				yfp0 = _mm_set1_ps(yf0);
				yfp1 = _mm_set1_ps(yf1);
				yfp2 = _mm_set1_ps(yf2);
				yfp3 = _mm_set1_ps(yf3);

				prod00 = _mm_mul_ps(yfp0, p00);
				prod01 = _mm_mul_ps(yfp1, p01);
				prod02 = _mm_mul_ps(yfp2, p02);
				prod03 = _mm_mul_ps(yfp3, p03);
				prod00 = _mm_add_ps(prod00, prod01);
				prod02 = _mm_add_ps(prod02, prod03);
				sum0 = _mm_add_ps(prod00, prod02);

				prod10 = _mm_mul_ps(yfp0, p10);
				prod11 = _mm_mul_ps(yfp1, p11);
				prod12 = _mm_mul_ps(yfp2, p12);
				prod13 = _mm_mul_ps(yfp3, p13);
				prod10 = _mm_add_ps(prod10, prod11);
				prod12 = _mm_add_ps(prod12, prod13);
				sum1 = _mm_add_ps(prod10, prod12);

				prod20 = _mm_mul_ps(yfp0, p20);
				prod21 = _mm_mul_ps(yfp1, p21);
				prod22 = _mm_mul_ps(yfp2, p22);
				prod23 = _mm_mul_ps(yfp3, p23);
				prod20 = _mm_add_ps(prod20, prod21);
				prod22 = _mm_add_ps(prod22, prod23);
				sum2 = _mm_add_ps(prod20, prod22);

				prod30 = _mm_mul_ps(yfp0, p30);
				prod31 = _mm_mul_ps(yfp1, p31);
				prod32 = _mm_mul_ps(yfp2, p32);
				prod33 = _mm_mul_ps(yfp3, p33);
				prod30 = _mm_add_ps(prod30, prod31);
				prod32 = _mm_add_ps(prod32, prod33);
				sum3 = _mm_add_ps(prod30, prod32);

				c0 = p32_0[0];
				c1 = p32_0[1];
				c2 = p32_0[2];
				c3 = p32_0[3];
				OPR_SSE2;
				STORE(dPtr[0], val0);

				c0 = p32_1[0];
				c1 = p32_1[1];
				c2 = p32_1[2];
				c3 = p32_1[3];
				OPR_SSE2;
				STORE(dPtr[1], val0);

				c0 = p32_2[0];
				c1 = p32_2[1];
				c2 = p32_2[2];
				c3 = p32_2[3];
				OPR_SSE2;
				STORE(dPtr[2], val0);

				c0 = p32_3[0];
				c1 = p32_3[1];
				c2 = p32_3[2];
				c3 = p32_3[3];
				OPR_SSE2;
				STORE(dPtr[3], val0);

			}
		}

	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
