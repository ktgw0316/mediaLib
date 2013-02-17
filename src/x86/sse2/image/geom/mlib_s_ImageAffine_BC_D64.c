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

#pragma ident	"@(#)mlib_s_ImageAffine_BC_D64.c	9.2	07/11/05 SMI"

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

#define	IMG_TYPE	5

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

		__m128d p0, p1, p2, p3;
		__m128d p0_, p1_, p2_, p3_;
		__m128d yp, yp0, yp1, yfp0, yfp1, yfp2, yfp3;
		__m128d sum, prod0, prod1, prod2, prod3;
		__m128d sum_, prod0_, prod1_, prod2_, prod3_;
		mlib_d64 *p32 = (mlib_d64 *)&sum;
		mlib_d64 *p32_ = (mlib_d64 *)&sum_;

		CLIP(1);
		dstLineEnd = (DTYPE *) dstData + xRight;

		if (filter == MLIB_BICUBIC) {
			for (; dstPixelPtr <= dstLineEnd; dstPixelPtr++) {
				CREATE_COEF_BICUBIC_SSE2(X, Y);

				xSrc = (X >> MLIB_SHIFT) - 1;
				ySrc = (Y >> MLIB_SHIFT) - 1;

				srcPixelPtr =
					((DTYPE **) lineAddr)[ySrc] + xSrc;
				p0 = _mm_loadu_pd(srcPixelPtr);
				p0_ = _mm_loadu_pd(srcPixelPtr + 2);

				srcPixelPtr =
					(DTYPE *) ((mlib_u8 *)srcPixelPtr +
					srcYStride);
				p1 = _mm_loadu_pd(srcPixelPtr);
				p1_ = _mm_loadu_pd(srcPixelPtr + 2);

				X += dX;
				Y += dY;

				srcPixelPtr =
					(DTYPE *) ((mlib_u8 *)srcPixelPtr +
					srcYStride);
				p2 = _mm_loadu_pd(srcPixelPtr);
				p2_ = _mm_loadu_pd(srcPixelPtr + 2);
				srcPixelPtr =
					(DTYPE *) ((mlib_u8 *)srcPixelPtr +
					srcYStride);
				p3 = _mm_loadu_pd(srcPixelPtr);
				p3_ = _mm_loadu_pd(srcPixelPtr + 2);

				yfp0 = _mm_set1_pd(yf0);
				yfp1 = _mm_set1_pd(yf1);
				yfp2 = _mm_set1_pd(yf2);
				yfp3 = _mm_set1_pd(yf3);

				prod0 = _mm_mul_pd(yfp0, p0);
				prod0_ = _mm_mul_pd(yfp0, p0_);
				prod1 = _mm_mul_pd(yfp1, p1);
				prod1_ = _mm_mul_pd(yfp1, p1_);
				prod2 = _mm_mul_pd(yfp2, p2);
				prod2_ = _mm_mul_pd(yfp2, p2_);
				prod3 = _mm_mul_pd(yfp3, p3);
				prod3_ = _mm_mul_pd(yfp3, p3_);
				prod0 = _mm_add_pd(prod0, prod1);
				prod0_ = _mm_add_pd(prod0_, prod1_);
				prod2 = _mm_add_pd(prod2, prod3);
				prod2_ = _mm_add_pd(prod2_, prod3_);
				sum = _mm_add_pd(prod0, prod2);
				sum_ = _mm_add_pd(prod0_, prod2_);

				c0 = p32[0];
				c1 = p32[1];
				c2 = p32_[0];
				c3 = p32_[1];

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
				p0 = _mm_loadu_pd(srcPixelPtr);
				p0_ = _mm_loadu_pd(srcPixelPtr + 2);

				srcPixelPtr =
					(DTYPE *) ((mlib_u8 *)srcPixelPtr +
					srcYStride);
				p1 = _mm_loadu_pd(srcPixelPtr);
				p1_ = _mm_loadu_pd(srcPixelPtr + 2);

				X += dX;
				Y += dY;

				srcPixelPtr =
					(DTYPE *) ((mlib_u8 *)srcPixelPtr +
					srcYStride);
				p2 = _mm_loadu_pd(srcPixelPtr);
				p2_ = _mm_loadu_pd(srcPixelPtr + 2);
				srcPixelPtr =
					(DTYPE *) ((mlib_u8 *)srcPixelPtr +
					srcYStride);
				p3 = _mm_loadu_pd(srcPixelPtr);
				p3_ = _mm_loadu_pd(srcPixelPtr + 2);

				yfp0 = _mm_set1_pd(yf0);
				yfp1 = _mm_set1_pd(yf1);
				yfp2 = _mm_set1_pd(yf2);
				yfp3 = _mm_set1_pd(yf3);

				prod0 = _mm_mul_pd(yfp0, p0);
				prod0_ = _mm_mul_pd(yfp0, p0_);
				prod1 = _mm_mul_pd(yfp1, p1);
				prod1_ = _mm_mul_pd(yfp1, p1_);
				prod2 = _mm_mul_pd(yfp2, p2);
				prod2_ = _mm_mul_pd(yfp2, p2_);
				prod3 = _mm_mul_pd(yfp3, p3);
				prod3_ = _mm_mul_pd(yfp3, p3_);
				prod0 = _mm_add_pd(prod0, prod1);
				prod0_ = _mm_add_pd(prod0_, prod1_);
				prod2 = _mm_add_pd(prod2, prod3);
				prod2_ = _mm_add_pd(prod2_, prod3_);
				sum = _mm_add_pd(prod0, prod2);
				sum_ = _mm_add_pd(prod0_, prod2_);

				c0 = p32[0];
				c1 = p32[1];
				c2 = p32_[0];
				c3 = p32_[1];

				OPR_SSE2;

				STORE(dstPixelPtr[0], val0);

			}
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
#define	INTERLEAVE(i0, i0_, i1, i1_)                                  \
	{                                                             \
		__m128d tmp0, tmp1, tmp2, tmp3;                       \
		tmp0 = i0;                                            \
		tmp1 = i0_;                                           \
		tmp2 = i1;                                            \
		tmp3 = i1_;                                           \
		i0 = _mm_unpacklo_pd(tmp0, tmp1);                     \
		i1 = _mm_unpackhi_pd(tmp0, tmp1);                     \
		i0_ = _mm_unpacklo_pd(tmp2, tmp3);                    \
		i1_ = _mm_unpackhi_pd(tmp2, tmp3);                    \
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

		__m128d p00, p01, p02, p03;
		__m128d p00_, p01_, p02_, p03_;
		__m128d p10, p11, p12, p13;
		__m128d p10_, p11_, p12_, p13_;
		__m128d yp, yp0, yp1, yfp0, yfp1, yfp2, yfp3;
		__m128d sum0, prod00, prod01, prod02, prod03;
		__m128d sum0_, prod00_, prod01_, prod02_, prod03_;
		__m128d sum1, prod10, prod11, prod12, prod13;
		__m128d sum1_, prod10_, prod11_, prod12_, prod13_;
		mlib_d64 *p32_0 = (mlib_d64 *)&sum0;
		mlib_d64 *p32_0_ = (mlib_d64 *)&sum0_;
		mlib_d64 *p32_1 = (mlib_d64 *)&sum1;
		mlib_d64 *p32_1_ = (mlib_d64 *)&sum1_;

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
				p00 = _mm_setr_pd(
						srcPixelPtr[0],
						srcPixelPtr[2]);
				p00_ = _mm_setr_pd(
						srcPixelPtr[4],
						srcPixelPtr[6]);
				p10 = _mm_setr_pd(
						srcPixelPtr[1],
						srcPixelPtr[3]);
				p10_ = _mm_setr_pd(
						srcPixelPtr[5],
						srcPixelPtr[7]);
#else
				p00 = _mm_loadu_pd(srcPixelPtr);
				p00_ = _mm_loadu_pd(srcPixelPtr + 2);
				p10 = _mm_loadu_pd(srcPixelPtr + 4);
				p10_ = _mm_loadu_pd(srcPixelPtr + 6);
				INTERLEAVE(p00, p00_, p10, p10_)
#endif

				srcPixelPtr =
					(DTYPE *) ((mlib_u8 *)
					srcPixelPtr + srcYStride);
				p01 = _mm_loadu_pd(srcPixelPtr);
				p01_ = _mm_loadu_pd(srcPixelPtr + 2);
				p11 = _mm_loadu_pd(srcPixelPtr + 4);
				p11_ = _mm_loadu_pd(srcPixelPtr + 6);
				INTERLEAVE(p01, p01_, p11, p11_)

				X1 += dX;
				Y1 += dY;

				srcPixelPtr =
					(DTYPE *) ((mlib_u8 *)
					srcPixelPtr + srcYStride);
				p02 = _mm_loadu_pd(srcPixelPtr);
				p02_ = _mm_loadu_pd(srcPixelPtr + 2);
				p12 = _mm_loadu_pd(srcPixelPtr + 4);
				p12_ = _mm_loadu_pd(srcPixelPtr + 6);
				INTERLEAVE(p02, p02_, p12, p12_)

				srcPixelPtr =
					(DTYPE *) ((mlib_u8 *)
					srcPixelPtr + srcYStride);
				p03 = _mm_loadu_pd(srcPixelPtr);
				p03_ = _mm_loadu_pd(srcPixelPtr + 2);
				p13 = _mm_loadu_pd(srcPixelPtr + 4);
				p13_ = _mm_loadu_pd(srcPixelPtr + 6);
				INTERLEAVE(p03, p03_, p13, p13_)

				yfp0 = _mm_set1_pd(yf0);
				yfp1 = _mm_set1_pd(yf1);
				yfp2 = _mm_set1_pd(yf2);
				yfp3 = _mm_set1_pd(yf3);

				prod00 = _mm_mul_pd(yfp0, p00);
				prod00_ = _mm_mul_pd(yfp0, p00_);
				prod01 = _mm_mul_pd(yfp1, p01);
				prod01_ = _mm_mul_pd(yfp1, p01_);
				prod02 = _mm_mul_pd(yfp2, p02);
				prod02_ = _mm_mul_pd(yfp2, p02_);
				prod03 = _mm_mul_pd(yfp3, p03);
				prod03_ = _mm_mul_pd(yfp3, p03_);
				prod00 = _mm_add_pd(prod00, prod01);
				prod00_ = _mm_add_pd(prod00_, prod01_);
				prod02 = _mm_add_pd(prod02, prod03);
				prod02_ = _mm_add_pd(prod02_, prod03_);
				sum0 = _mm_add_pd(prod00, prod02);
				sum0_ = _mm_add_pd(prod00_, prod02_);

				prod10 = _mm_mul_pd(yfp0, p10);
				prod10_ = _mm_mul_pd(yfp0, p10_);
				prod11 = _mm_mul_pd(yfp1, p11);
				prod11_ = _mm_mul_pd(yfp1, p11_);
				prod12 = _mm_mul_pd(yfp2, p12);
				prod12_ = _mm_mul_pd(yfp2, p12_);
				prod13 = _mm_mul_pd(yfp3, p13);
				prod13_ = _mm_mul_pd(yfp3, p13_);
				prod10 = _mm_add_pd(prod10, prod11);
				prod10_ = _mm_add_pd(prod10_, prod11_);
				prod12 = _mm_add_pd(prod12, prod13);
				prod12_ = _mm_add_pd(prod12_, prod13_);
				sum1 = _mm_add_pd(prod10, prod12);
				sum1_ = _mm_add_pd(prod10_, prod12_);

				c0 = p32_0[0];
				c1 = p32_0[1];
				c2 = p32_0_[0];
				c3 = p32_0_[1];
				OPR_SSE2;
				STORE(dPtr[0], val0);

				c0 = p32_1[0];
				c1 = p32_1[1];
				c2 = p32_1_[0];
				c3 = p32_1_[1];
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
				p00 = _mm_loadu_pd(srcPixelPtr);
				p00_ = _mm_loadu_pd(srcPixelPtr + 2);
				p10 = _mm_loadu_pd(srcPixelPtr + 4);
				p10_ = _mm_loadu_pd(srcPixelPtr + 6);
				INTERLEAVE(p00, p00_, p10, p10_)

				srcPixelPtr =
					(DTYPE *) ((mlib_u8 *)
					srcPixelPtr + srcYStride);
				p01 = _mm_loadu_pd(srcPixelPtr);
				p01_ = _mm_loadu_pd(srcPixelPtr + 2);
				p11 = _mm_loadu_pd(srcPixelPtr + 4);
				p11_ = _mm_loadu_pd(srcPixelPtr + 6);
				INTERLEAVE(p01, p01_, p11, p11_)

				X1 += dX;
				Y1 += dY;

				srcPixelPtr =
					(DTYPE *) ((mlib_u8 *)
					srcPixelPtr + srcYStride);
				p02 = _mm_loadu_pd(srcPixelPtr);
				p02_ = _mm_loadu_pd(srcPixelPtr + 2);
				p12 = _mm_loadu_pd(srcPixelPtr + 4);
				p12_ = _mm_loadu_pd(srcPixelPtr + 6);
				INTERLEAVE(p02, p02_, p12, p12_)

				srcPixelPtr =
					(DTYPE *) ((mlib_u8 *)
					srcPixelPtr + srcYStride);
				p03 = _mm_loadu_pd(srcPixelPtr);
				p03_ = _mm_loadu_pd(srcPixelPtr + 2);
				p13 = _mm_loadu_pd(srcPixelPtr + 4);
				p13_ = _mm_loadu_pd(srcPixelPtr + 6);
				INTERLEAVE(p03, p03_, p13, p13_)

				yfp0 = _mm_set1_pd(yf0);
				yfp1 = _mm_set1_pd(yf1);
				yfp2 = _mm_set1_pd(yf2);
				yfp3 = _mm_set1_pd(yf3);

				prod00 = _mm_mul_pd(yfp0, p00);
				prod00_ = _mm_mul_pd(yfp0, p00_);
				prod01 = _mm_mul_pd(yfp1, p01);
				prod01_ = _mm_mul_pd(yfp1, p01_);
				prod02 = _mm_mul_pd(yfp2, p02);
				prod02_ = _mm_mul_pd(yfp2, p02_);
				prod03 = _mm_mul_pd(yfp3, p03);
				prod03_ = _mm_mul_pd(yfp3, p03_);
				prod00 = _mm_add_pd(prod00, prod01);
				prod00_ = _mm_add_pd(prod00_, prod01_);
				prod02 = _mm_add_pd(prod02, prod03);
				prod02_ = _mm_add_pd(prod02_, prod03_);
				sum0 = _mm_add_pd(prod00, prod02);
				sum0_ = _mm_add_pd(prod00_, prod02_);

				prod10 = _mm_mul_pd(yfp0, p10);
				prod10_ = _mm_mul_pd(yfp0, p10_);
				prod11 = _mm_mul_pd(yfp1, p11);
				prod11_ = _mm_mul_pd(yfp1, p11_);
				prod12 = _mm_mul_pd(yfp2, p12);
				prod12_ = _mm_mul_pd(yfp2, p12_);
				prod13 = _mm_mul_pd(yfp3, p13);
				prod13_ = _mm_mul_pd(yfp3, p13_);
				prod10 = _mm_add_pd(prod10, prod11);
				prod10_ = _mm_add_pd(prod10_, prod11_);
				prod12 = _mm_add_pd(prod12, prod13);
				prod12_ = _mm_add_pd(prod12_, prod13_);
				sum1 = _mm_add_pd(prod10, prod12);
				sum1_ = _mm_add_pd(prod10_, prod12_);

				c0 = p32_0[0];
				c1 = p32_0[1];
				c2 = p32_0_[0];
				c3 = p32_0_[1];
				OPR_SSE2;
				STORE(dPtr[0], val0);

				c0 = p32_1[0];
				c1 = p32_1[1];
				c2 = p32_1_[0];
				c3 = p32_1_[1];
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

		__m128d p00, p01, p02, p03;
		__m128d p00_, p01_, p02_, p03_;
		__m128d p10, p11, p12, p13;
		__m128d p10_, p11_, p12_, p13_;
		__m128d p20, p21, p22, p23;
		__m128d p20_, p21_, p22_, p23_;
		__m128d yp, yp0, yp1, yfp0, yfp1, yfp2, yfp3;
		__m128d sum0, prod00, prod01, prod02, prod03;
		__m128d sum0_, prod00_, prod01_, prod02_, prod03_;
		__m128d sum1, prod10, prod11, prod12, prod13;
		__m128d sum1_, prod10_, prod11_, prod12_, prod13_;
		__m128d sum2, prod20, prod21, prod22, prod23;
		__m128d sum2_, prod20_, prod21_, prod22_, prod23_;
		mlib_d64 *p32_0 = (mlib_d64 *)&sum0;
		mlib_d64 *p32_0_ = (mlib_d64 *)&sum0_;
		mlib_d64 *p32_1 = (mlib_d64 *)&sum1;
		mlib_d64 *p32_1_ = (mlib_d64 *)&sum1_;
		mlib_d64 *p32_2 = (mlib_d64 *)&sum2;
		mlib_d64 *p32_2_ = (mlib_d64 *)&sum2_;

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
				p00 = _mm_setr_pd(
						srcPixelPtr[0],
						srcPixelPtr[3]);
				p00_ = _mm_setr_pd(
						srcPixelPtr[6],
						srcPixelPtr[9]);
				p10 = _mm_setr_pd(
						srcPixelPtr[1],
						srcPixelPtr[4]);
				p10_ = _mm_setr_pd(
						srcPixelPtr[7],
						srcPixelPtr[10]);
				p20 = _mm_setr_pd(
						srcPixelPtr[2],
						srcPixelPtr[5]);
				p20_ = _mm_setr_pd(
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
				p01 = _mm_setr_pd(
						srcPixelPtr[0],
						srcPixelPtr[3]);
				p01_ = _mm_setr_pd(
						srcPixelPtr[6],
						srcPixelPtr[9]);
				p11 = _mm_setr_pd(
						srcPixelPtr[1],
						srcPixelPtr[4]);
				p11_ = _mm_setr_pd(
						srcPixelPtr[7],
						srcPixelPtr[10]);
				p21 = _mm_setr_pd(
						srcPixelPtr[2],
						srcPixelPtr[5]);
				p21_ = _mm_setr_pd(
						srcPixelPtr[8],
						srcPixelPtr[11]);

				X1 += dX;
				Y1 += dY;

				srcPixelPtr =
					(DTYPE *) ((mlib_u8 *)
					srcPixelPtr + srcYStride);
				p02 = _mm_setr_pd(
						srcPixelPtr[0],
						srcPixelPtr[3]);
				p02_ = _mm_setr_pd(
						srcPixelPtr[6],
						srcPixelPtr[9]);
				p12 = _mm_setr_pd(
						srcPixelPtr[1],
						srcPixelPtr[4]);
				p12_ = _mm_setr_pd(
						srcPixelPtr[7],
						srcPixelPtr[10]);
				p22 = _mm_setr_pd(
						srcPixelPtr[2],
						srcPixelPtr[5]);
				p22_ = _mm_setr_pd(
						srcPixelPtr[8],
						srcPixelPtr[11]);

				srcPixelPtr =
					(DTYPE *) ((mlib_u8 *)
					srcPixelPtr + srcYStride);
				p03 = _mm_setr_pd(
						srcPixelPtr[0],
						srcPixelPtr[3]);
				p03_ = _mm_setr_pd(
						srcPixelPtr[6],
						srcPixelPtr[9]);
				p13 = _mm_setr_pd(
						srcPixelPtr[1],
						srcPixelPtr[4]);
				p13_ = _mm_setr_pd(
						srcPixelPtr[7],
						srcPixelPtr[10]);
				p23 = _mm_setr_pd(
						srcPixelPtr[2],
						srcPixelPtr[5]);
				p23_ = _mm_setr_pd(
						srcPixelPtr[8],
						srcPixelPtr[11]);

				yfp0 = _mm_set1_pd(yf0);
				yfp1 = _mm_set1_pd(yf1);
				yfp2 = _mm_set1_pd(yf2);
				yfp3 = _mm_set1_pd(yf3);

				prod00 = _mm_mul_pd(yfp0, p00);
				prod00_ = _mm_mul_pd(yfp0, p00_);
				prod01 = _mm_mul_pd(yfp1, p01);
				prod01_ = _mm_mul_pd(yfp1, p01_);
				prod02 = _mm_mul_pd(yfp2, p02);
				prod02_ = _mm_mul_pd(yfp2, p02_);
				prod03 = _mm_mul_pd(yfp3, p03);
				prod03_ = _mm_mul_pd(yfp3, p03_);
				prod00 = _mm_add_pd(prod00, prod01);
				prod00_ = _mm_add_pd(prod00_, prod01_);
				prod02 = _mm_add_pd(prod02, prod03);
				prod02_ = _mm_add_pd(prod02_, prod03_);
				sum0 = _mm_add_pd(prod00, prod02);
				sum0_ = _mm_add_pd(prod00_, prod02_);

				prod10 = _mm_mul_pd(yfp0, p10);
				prod10_ = _mm_mul_pd(yfp0, p10_);
				prod11 = _mm_mul_pd(yfp1, p11);
				prod11_ = _mm_mul_pd(yfp1, p11_);
				prod12 = _mm_mul_pd(yfp2, p12);
				prod12_ = _mm_mul_pd(yfp2, p12_);
				prod13 = _mm_mul_pd(yfp3, p13);
				prod13_ = _mm_mul_pd(yfp3, p13_);
				prod10 = _mm_add_pd(prod10, prod11);
				prod10_ = _mm_add_pd(prod10_, prod11_);
				prod12 = _mm_add_pd(prod12, prod13);
				prod12_ = _mm_add_pd(prod12_, prod13_);
				sum1 = _mm_add_pd(prod10, prod12);
				sum1_ = _mm_add_pd(prod10_, prod12_);

				prod20 = _mm_mul_pd(yfp0, p20);
				prod20_ = _mm_mul_pd(yfp0, p20_);
				prod21 = _mm_mul_pd(yfp1, p21);
				prod21_ = _mm_mul_pd(yfp1, p21_);
				prod22 = _mm_mul_pd(yfp2, p22);
				prod22_ = _mm_mul_pd(yfp2, p22_);
				prod23 = _mm_mul_pd(yfp3, p23);
				prod23_ = _mm_mul_pd(yfp3, p23_);
				prod20 = _mm_add_pd(prod20, prod21);
				prod20_ = _mm_add_pd(prod20_, prod21_);
				prod22 = _mm_add_pd(prod22, prod23);
				prod22_ = _mm_add_pd(prod22_, prod23_);
				sum2 = _mm_add_pd(prod20, prod22);
				sum2_ = _mm_add_pd(prod20_, prod22_);

				c0 = p32_0[0];
				c1 = p32_0[1];
				c2 = p32_0_[0];
				c3 = p32_0_[1];
				OPR_SSE2;
				STORE(dPtr[0], val0);

				c0 = p32_1[0];
				c1 = p32_1[1];
				c2 = p32_1_[0];
				c3 = p32_1_[1];
				OPR_SSE2;
				STORE(dPtr[1], val0);

				c0 = p32_2[0];
				c1 = p32_2[1];
				c2 = p32_2_[0];
				c3 = p32_2_[1];
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
#if 1
				p00 = _mm_setr_pd(
						srcPixelPtr[0],
						srcPixelPtr[3]);
				p00_ = _mm_setr_pd(
						srcPixelPtr[6],
						srcPixelPtr[9]);
				p10 = _mm_setr_pd(
						srcPixelPtr[1],
						srcPixelPtr[4]);
				p10_ = _mm_setr_pd(
						srcPixelPtr[7],
						srcPixelPtr[10]);
				p20 = _mm_setr_pd(
						srcPixelPtr[2],
						srcPixelPtr[5]);
				p20_ = _mm_setr_pd(
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
				p01 = _mm_setr_pd(
						srcPixelPtr[0],
						srcPixelPtr[3]);
				p01_ = _mm_setr_pd(
						srcPixelPtr[6],
						srcPixelPtr[9]);
				p11 = _mm_setr_pd(
						srcPixelPtr[1],
						srcPixelPtr[4]);
				p11_ = _mm_setr_pd(
						srcPixelPtr[7],
						srcPixelPtr[10]);
				p21 = _mm_setr_pd(
						srcPixelPtr[2],
						srcPixelPtr[5]);
				p21_ = _mm_setr_pd(
						srcPixelPtr[8],
						srcPixelPtr[11]);

				X1 += dX;
				Y1 += dY;

				srcPixelPtr =
					(DTYPE *) ((mlib_u8 *)
					srcPixelPtr + srcYStride);
				p02 = _mm_setr_pd(
						srcPixelPtr[0],
						srcPixelPtr[3]);
				p02_ = _mm_setr_pd(
						srcPixelPtr[6],
						srcPixelPtr[9]);
				p12 = _mm_setr_pd(
						srcPixelPtr[1],
						srcPixelPtr[4]);
				p12_ = _mm_setr_pd(
						srcPixelPtr[7],
						srcPixelPtr[10]);
				p22 = _mm_setr_pd(
						srcPixelPtr[2],
						srcPixelPtr[5]);
				p22_ = _mm_setr_pd(
						srcPixelPtr[8],
						srcPixelPtr[11]);

				srcPixelPtr =
					(DTYPE *) ((mlib_u8 *)
					srcPixelPtr + srcYStride);
				p03 = _mm_setr_pd(
						srcPixelPtr[0],
						srcPixelPtr[3]);
				p03_ = _mm_setr_pd(
						srcPixelPtr[6],
						srcPixelPtr[9]);
				p13 = _mm_setr_pd(
						srcPixelPtr[1],
						srcPixelPtr[4]);
				p13_ = _mm_setr_pd(
						srcPixelPtr[7],
						srcPixelPtr[10]);
				p23 = _mm_setr_pd(
						srcPixelPtr[2],
						srcPixelPtr[5]);
				p23_ = _mm_setr_pd(
						srcPixelPtr[8],
						srcPixelPtr[11]);

				yfp0 = _mm_set1_pd(yf0);
				yfp1 = _mm_set1_pd(yf1);
				yfp2 = _mm_set1_pd(yf2);
				yfp3 = _mm_set1_pd(yf3);

				prod00 = _mm_mul_pd(yfp0, p00);
				prod00_ = _mm_mul_pd(yfp0, p00_);
				prod01 = _mm_mul_pd(yfp1, p01);
				prod01_ = _mm_mul_pd(yfp1, p01_);
				prod02 = _mm_mul_pd(yfp2, p02);
				prod02_ = _mm_mul_pd(yfp2, p02_);
				prod03 = _mm_mul_pd(yfp3, p03);
				prod03_ = _mm_mul_pd(yfp3, p03_);
				prod00 = _mm_add_pd(prod00, prod01);
				prod00_ = _mm_add_pd(prod00_, prod01_);
				prod02 = _mm_add_pd(prod02, prod03);
				prod02_ = _mm_add_pd(prod02_, prod03_);
				sum0 = _mm_add_pd(prod00, prod02);
				sum0_ = _mm_add_pd(prod00_, prod02_);

				prod10 = _mm_mul_pd(yfp0, p10);
				prod10_ = _mm_mul_pd(yfp0, p10_);
				prod11 = _mm_mul_pd(yfp1, p11);
				prod11_ = _mm_mul_pd(yfp1, p11_);
				prod12 = _mm_mul_pd(yfp2, p12);
				prod12_ = _mm_mul_pd(yfp2, p12_);
				prod13 = _mm_mul_pd(yfp3, p13);
				prod13_ = _mm_mul_pd(yfp3, p13_);
				prod10 = _mm_add_pd(prod10, prod11);
				prod10_ = _mm_add_pd(prod10_, prod11_);
				prod12 = _mm_add_pd(prod12, prod13);
				prod12_ = _mm_add_pd(prod12_, prod13_);
				sum1 = _mm_add_pd(prod10, prod12);
				sum1_ = _mm_add_pd(prod10_, prod12_);

				prod20 = _mm_mul_pd(yfp0, p20);
				prod20_ = _mm_mul_pd(yfp0, p20_);
				prod21 = _mm_mul_pd(yfp1, p21);
				prod21_ = _mm_mul_pd(yfp1, p21_);
				prod22 = _mm_mul_pd(yfp2, p22);
				prod22_ = _mm_mul_pd(yfp2, p22_);
				prod23 = _mm_mul_pd(yfp3, p23);
				prod23_ = _mm_mul_pd(yfp3, p23_);
				prod20 = _mm_add_pd(prod20, prod21);
				prod20_ = _mm_add_pd(prod20_, prod21_);
				prod22 = _mm_add_pd(prod22, prod23);
				prod22_ = _mm_add_pd(prod22_, prod23_);
				sum2 = _mm_add_pd(prod20, prod22);
				sum2_ = _mm_add_pd(prod20_, prod22_);

				c0 = p32_0[0];
				c1 = p32_0[1];
				c2 = p32_0_[0];
				c3 = p32_0_[1];
				OPR_SSE2;
				STORE(dPtr[0], val0);

				c0 = p32_1[0];
				c1 = p32_1[1];
				c2 = p32_1_[0];
				c3 = p32_1_[1];
				OPR_SSE2;
				STORE(dPtr[1], val0);

				c0 = p32_2[0];
				c1 = p32_2[1];
				c2 = p32_2_[0];
				c3 = p32_2_[1];
				OPR_SSE2;
				STORE(dPtr[2], val0);

			}
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
#define		TRANSPOSE(p0, p0_, p1, p1_, p2, p2_, p3, p3_)       \
	{                                                           \
		__m128d tmp0, tmp1, tmp2, tmp3;                     \
		__m128d tmp4, tmp5, tmp6, tmp7;                     \
                                                                    \
		tmp0 = p0;                                          \
		tmp2 = p1;                                          \
		tmp4 = p2;                                          \
		tmp6 = p3;                                          \
		tmp1 = p0_;                                         \
		tmp3 = p1_;                                         \
		tmp5 = p2_;                                         \
		tmp7 = p3_;                                         \
                                                                    \
		p0 = _mm_unpacklo_pd(tmp0, tmp2);                   \
		p1 = _mm_unpackhi_pd(tmp0, tmp2);                   \
		p2 = _mm_unpacklo_pd(tmp1, tmp3);                   \
		p3 = _mm_unpackhi_pd(tmp1, tmp3);                   \
		p0_ = _mm_unpacklo_pd(tmp4, tmp6);                  \
		p1_ = _mm_unpackhi_pd(tmp4, tmp6);                  \
		p2_ = _mm_unpacklo_pd(tmp5, tmp7);                  \
		p3_ = _mm_unpackhi_pd(tmp5, tmp7);                  \
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

		__m128d yp, yp0, yp1, yfp0, yfp1, yfp2, yfp3;

		__m128d p00, p01, p02, p03;
		__m128d p10, p11, p12, p13;
		__m128d p20, p21, p22, p23;
		__m128d p30, p31, p32, p33;
		__m128d sum0, prod00, prod01, prod02, prod03;
		__m128d sum1, prod10, prod11, prod12, prod13;
		__m128d sum2, prod20, prod21, prod22, prod23;
		__m128d sum3, prod30, prod31, prod32, prod33;
		mlib_d64 *p32_0 = (mlib_d64 *)&sum0;
		mlib_d64 *p32_1 = (mlib_d64 *)&sum1;
		mlib_d64 *p32_2 = (mlib_d64 *)&sum2;
		mlib_d64 *p32_3 = (mlib_d64 *)&sum3;

		__m128d p00_, p01_, p02_, p03_;
		__m128d p10_, p11_, p12_, p13_;
		__m128d p20_, p21_, p22_, p23_;
		__m128d p30_, p31_, p32_, p33_;
		__m128d sum0_, prod00_, prod01_, prod02_, prod03_;
		__m128d sum1_, prod10_, prod11_, prod12_, prod13_;
		__m128d sum2_, prod20_, prod21_, prod22_, prod23_;
		__m128d sum3_, prod30_, prod31_, prod32_, prod33_;
		mlib_d64 *p32_0_ = (mlib_d64 *)&sum0_;
		mlib_d64 *p32_1_ = (mlib_d64 *)&sum1_;
		mlib_d64 *p32_2_ = (mlib_d64 *)&sum2_;
		mlib_d64 *p32_3_ = (mlib_d64 *)&sum3_;

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
				p00 = _mm_loadu_pd(srcPixelPtr);
				p10 = _mm_loadu_pd(srcPixelPtr + 4);
				p20 = _mm_loadu_pd(srcPixelPtr + 8);
				p30 = _mm_loadu_pd(srcPixelPtr + 12);
				p00_ = _mm_loadu_pd(srcPixelPtr + 2);
				p10_ = _mm_loadu_pd(srcPixelPtr + 6);
				p20_ = _mm_loadu_pd(srcPixelPtr + 10);
				p30_ = _mm_loadu_pd(srcPixelPtr + 14);
				TRANSPOSE(p00, p00_, p10, p10_,
						p20, p20_, p30, p30_)
#endif

				srcPixelPtr =
					(DTYPE *) ((mlib_u8 *)
					srcPixelPtr + srcYStride);
				p01 = _mm_loadu_pd(srcPixelPtr);
				p11 = _mm_loadu_pd(srcPixelPtr + 4);
				p21 = _mm_loadu_pd(srcPixelPtr + 8);
				p31 = _mm_loadu_pd(srcPixelPtr + 12);
				p01_ = _mm_loadu_pd(srcPixelPtr + 2);
				p11_ = _mm_loadu_pd(srcPixelPtr + 6);
				p21_ = _mm_loadu_pd(srcPixelPtr + 10);
				p31_ = _mm_loadu_pd(srcPixelPtr + 14);
				TRANSPOSE(p01, p01_, p11, p11_,
						p21, p21_, p31, p31_)

				X1 += dX;
				Y1 += dY;

				srcPixelPtr =
					(DTYPE *) ((mlib_u8 *)
					srcPixelPtr + srcYStride);
				p02 = _mm_loadu_pd(srcPixelPtr);
				p12 = _mm_loadu_pd(srcPixelPtr + 4);
				p22 = _mm_loadu_pd(srcPixelPtr + 8);
				p32 = _mm_loadu_pd(srcPixelPtr + 12);
				p02_ = _mm_loadu_pd(srcPixelPtr + 2);
				p12_ = _mm_loadu_pd(srcPixelPtr + 6);
				p22_ = _mm_loadu_pd(srcPixelPtr + 10);
				p32_ = _mm_loadu_pd(srcPixelPtr + 14);
				TRANSPOSE(p02, p02_, p12, p12_,
						p22, p22_, p32, p32_)

				srcPixelPtr =
					(DTYPE *) ((mlib_u8 *)
					srcPixelPtr + srcYStride);
				p03 = _mm_loadu_pd(srcPixelPtr);
				p13 = _mm_loadu_pd(srcPixelPtr + 4);
				p23 = _mm_loadu_pd(srcPixelPtr + 8);
				p33 = _mm_loadu_pd(srcPixelPtr + 12);
				p03_ = _mm_loadu_pd(srcPixelPtr + 2);
				p13_ = _mm_loadu_pd(srcPixelPtr + 6);
				p23_ = _mm_loadu_pd(srcPixelPtr + 10);
				p33_ = _mm_loadu_pd(srcPixelPtr + 14);
				TRANSPOSE(p03, p03_, p13, p13_,
						p23, p23_, p33, p33_)

				yfp0 = _mm_set1_pd(yf0);
				yfp1 = _mm_set1_pd(yf1);
				yfp2 = _mm_set1_pd(yf2);
				yfp3 = _mm_set1_pd(yf3);

				prod00 = _mm_mul_pd(yfp0, p00);
				prod01 = _mm_mul_pd(yfp1, p01);
				prod02 = _mm_mul_pd(yfp2, p02);
				prod03 = _mm_mul_pd(yfp3, p03);
				prod00 = _mm_add_pd(prod00, prod01);
				prod02 = _mm_add_pd(prod02, prod03);
				sum0 = _mm_add_pd(prod00, prod02);
				prod00_ = _mm_mul_pd(yfp0, p00_);
				prod01_ = _mm_mul_pd(yfp1, p01_);
				prod02_ = _mm_mul_pd(yfp2, p02_);
				prod03_ = _mm_mul_pd(yfp3, p03_);
				prod00_ = _mm_add_pd(prod00_, prod01_);
				prod02_ = _mm_add_pd(prod02_, prod03_);
				sum0_ = _mm_add_pd(prod00_, prod02_);

				prod10 = _mm_mul_pd(yfp0, p10);
				prod11 = _mm_mul_pd(yfp1, p11);
				prod12 = _mm_mul_pd(yfp2, p12);
				prod13 = _mm_mul_pd(yfp3, p13);
				prod10 = _mm_add_pd(prod10, prod11);
				prod12 = _mm_add_pd(prod12, prod13);
				sum1 = _mm_add_pd(prod10, prod12);
				prod10_ = _mm_mul_pd(yfp0, p10_);
				prod11_ = _mm_mul_pd(yfp1, p11_);
				prod12_ = _mm_mul_pd(yfp2, p12_);
				prod13_ = _mm_mul_pd(yfp3, p13_);
				prod10_ = _mm_add_pd(prod10_, prod11_);
				prod12_ = _mm_add_pd(prod12_, prod13_);
				sum1_ = _mm_add_pd(prod10_, prod12_);

				prod20 = _mm_mul_pd(yfp0, p20);
				prod21 = _mm_mul_pd(yfp1, p21);
				prod22 = _mm_mul_pd(yfp2, p22);
				prod23 = _mm_mul_pd(yfp3, p23);
				prod20 = _mm_add_pd(prod20, prod21);
				prod22 = _mm_add_pd(prod22, prod23);
				sum2 = _mm_add_pd(prod20, prod22);
				prod20_ = _mm_mul_pd(yfp0, p20_);
				prod21_ = _mm_mul_pd(yfp1, p21_);
				prod22_ = _mm_mul_pd(yfp2, p22_);
				prod23_ = _mm_mul_pd(yfp3, p23_);
				prod20_ = _mm_add_pd(prod20_, prod21_);
				prod22_ = _mm_add_pd(prod22_, prod23_);
				sum2_ = _mm_add_pd(prod20_, prod22_);

				prod30 = _mm_mul_pd(yfp0, p30);
				prod31 = _mm_mul_pd(yfp1, p31);
				prod32 = _mm_mul_pd(yfp2, p32);
				prod33 = _mm_mul_pd(yfp3, p33);
				prod30 = _mm_add_pd(prod30, prod31);
				prod32 = _mm_add_pd(prod32, prod33);
				sum3 = _mm_add_pd(prod30, prod32);
				prod30_ = _mm_mul_pd(yfp0, p30_);
				prod31_ = _mm_mul_pd(yfp1, p31_);
				prod32_ = _mm_mul_pd(yfp2, p32_);
				prod33_ = _mm_mul_pd(yfp3, p33_);
				prod30_ = _mm_add_pd(prod30_, prod31_);
				prod32_ = _mm_add_pd(prod32_, prod33_);
				sum3_ = _mm_add_pd(prod30_, prod32_);

				c0 = p32_0[0];
				c1 = p32_0[1];
				c2 = p32_0_[0];
				c3 = p32_0_[1];
				OPR_SSE2;
				STORE(dPtr[0], val0);

				c0 = p32_1[0];
				c1 = p32_1[1];
				c2 = p32_1_[0];
				c3 = p32_1_[1];
				OPR_SSE2;
				STORE(dPtr[1], val0);

				c0 = p32_2[0];
				c1 = p32_2[1];
				c2 = p32_2_[0];
				c3 = p32_2_[1];
				OPR_SSE2;
				STORE(dPtr[2], val0);

				c0 = p32_3[0];
				c1 = p32_3[1];
				c2 = p32_3_[0];
				c3 = p32_3_[1];
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
				p00 = _mm_loadu_pd(srcPixelPtr);
				p10 = _mm_loadu_pd(srcPixelPtr + 4);
				p20 = _mm_loadu_pd(srcPixelPtr + 8);
				p30 = _mm_loadu_pd(srcPixelPtr + 12);
				p00_ = _mm_loadu_pd(srcPixelPtr + 2);
				p10_ = _mm_loadu_pd(srcPixelPtr + 6);
				p20_ = _mm_loadu_pd(srcPixelPtr + 10);
				p30_ = _mm_loadu_pd(srcPixelPtr + 14);
				TRANSPOSE(p00, p00_, p10, p10_,
						p20, p20_, p30, p30_)

				srcPixelPtr =
					(DTYPE *) ((mlib_u8 *)
					srcPixelPtr + srcYStride);
				p01 = _mm_loadu_pd(srcPixelPtr);
				p11 = _mm_loadu_pd(srcPixelPtr + 4);
				p21 = _mm_loadu_pd(srcPixelPtr + 8);
				p31 = _mm_loadu_pd(srcPixelPtr + 12);
				p01_ = _mm_loadu_pd(srcPixelPtr + 2);
				p11_ = _mm_loadu_pd(srcPixelPtr + 6);
				p21_ = _mm_loadu_pd(srcPixelPtr + 10);
				p31_ = _mm_loadu_pd(srcPixelPtr + 14);
				TRANSPOSE(p01, p01_, p11, p11_,
						p21, p21_, p31, p31_)

				X1 += dX;
				Y1 += dY;

				srcPixelPtr =
					(DTYPE *) ((mlib_u8 *)
					srcPixelPtr + srcYStride);
				p02 = _mm_loadu_pd(srcPixelPtr);
				p12 = _mm_loadu_pd(srcPixelPtr + 4);
				p22 = _mm_loadu_pd(srcPixelPtr + 8);
				p32 = _mm_loadu_pd(srcPixelPtr + 12);
				p02_ = _mm_loadu_pd(srcPixelPtr + 2);
				p12_ = _mm_loadu_pd(srcPixelPtr + 6);
				p22_ = _mm_loadu_pd(srcPixelPtr + 10);
				p32_ = _mm_loadu_pd(srcPixelPtr + 14);
				TRANSPOSE(p02, p02_, p12, p12_,
						p22, p22_, p32, p32_)

				srcPixelPtr =
					(DTYPE *) ((mlib_u8 *)
					srcPixelPtr + srcYStride);
				p03 = _mm_loadu_pd(srcPixelPtr);
				p13 = _mm_loadu_pd(srcPixelPtr + 4);
				p23 = _mm_loadu_pd(srcPixelPtr + 8);
				p33 = _mm_loadu_pd(srcPixelPtr + 12);
				p03_ = _mm_loadu_pd(srcPixelPtr + 2);
				p13_ = _mm_loadu_pd(srcPixelPtr + 6);
				p23_ = _mm_loadu_pd(srcPixelPtr + 10);
				p33_ = _mm_loadu_pd(srcPixelPtr + 14);
				TRANSPOSE(p03, p03_, p13, p13_,
						p23, p23_, p33, p33_)

				yfp0 = _mm_set1_pd(yf0);
				yfp1 = _mm_set1_pd(yf1);
				yfp2 = _mm_set1_pd(yf2);
				yfp3 = _mm_set1_pd(yf3);

				prod00 = _mm_mul_pd(yfp0, p00);
				prod01 = _mm_mul_pd(yfp1, p01);
				prod02 = _mm_mul_pd(yfp2, p02);
				prod03 = _mm_mul_pd(yfp3, p03);
				prod00 = _mm_add_pd(prod00, prod01);
				prod02 = _mm_add_pd(prod02, prod03);
				sum0 = _mm_add_pd(prod00, prod02);
				prod00_ = _mm_mul_pd(yfp0, p00_);
				prod01_ = _mm_mul_pd(yfp1, p01_);
				prod02_ = _mm_mul_pd(yfp2, p02_);
				prod03_ = _mm_mul_pd(yfp3, p03_);
				prod00_ = _mm_add_pd(prod00_, prod01_);
				prod02_ = _mm_add_pd(prod02_, prod03_);
				sum0_ = _mm_add_pd(prod00_, prod02_);

				prod10 = _mm_mul_pd(yfp0, p10);
				prod11 = _mm_mul_pd(yfp1, p11);
				prod12 = _mm_mul_pd(yfp2, p12);
				prod13 = _mm_mul_pd(yfp3, p13);
				prod10 = _mm_add_pd(prod10, prod11);
				prod12 = _mm_add_pd(prod12, prod13);
				sum1 = _mm_add_pd(prod10, prod12);
				prod10_ = _mm_mul_pd(yfp0, p10_);
				prod11_ = _mm_mul_pd(yfp1, p11_);
				prod12_ = _mm_mul_pd(yfp2, p12_);
				prod13_ = _mm_mul_pd(yfp3, p13_);
				prod10_ = _mm_add_pd(prod10_, prod11_);
				prod12_ = _mm_add_pd(prod12_, prod13_);
				sum1_ = _mm_add_pd(prod10_, prod12_);

				prod20 = _mm_mul_pd(yfp0, p20);
				prod21 = _mm_mul_pd(yfp1, p21);
				prod22 = _mm_mul_pd(yfp2, p22);
				prod23 = _mm_mul_pd(yfp3, p23);
				prod20 = _mm_add_pd(prod20, prod21);
				prod22 = _mm_add_pd(prod22, prod23);
				sum2 = _mm_add_pd(prod20, prod22);
				prod20_ = _mm_mul_pd(yfp0, p20_);
				prod21_ = _mm_mul_pd(yfp1, p21_);
				prod22_ = _mm_mul_pd(yfp2, p22_);
				prod23_ = _mm_mul_pd(yfp3, p23_);
				prod20_ = _mm_add_pd(prod20_, prod21_);
				prod22_ = _mm_add_pd(prod22_, prod23_);
				sum2_ = _mm_add_pd(prod20_, prod22_);

				prod30 = _mm_mul_pd(yfp0, p30);
				prod31 = _mm_mul_pd(yfp1, p31);
				prod32 = _mm_mul_pd(yfp2, p32);
				prod33 = _mm_mul_pd(yfp3, p33);
				prod30 = _mm_add_pd(prod30, prod31);
				prod32 = _mm_add_pd(prod32, prod33);
				sum3 = _mm_add_pd(prod30, prod32);
				prod30_ = _mm_mul_pd(yfp0, p30_);
				prod31_ = _mm_mul_pd(yfp1, p31_);
				prod32_ = _mm_mul_pd(yfp2, p32_);
				prod33_ = _mm_mul_pd(yfp3, p33_);
				prod30_ = _mm_add_pd(prod30_, prod31_);
				prod32_ = _mm_add_pd(prod32_, prod33_);
				sum3_ = _mm_add_pd(prod30_, prod32_);

				c0 = p32_0[0];
				c1 = p32_0[1];
				c2 = p32_0_[0];
				c3 = p32_0_[1];
				OPR_SSE2;
				STORE(dPtr[0], val0);

				c0 = p32_1[0];
				c1 = p32_1[1];
				c2 = p32_1_[0];
				c3 = p32_1_[1];
				OPR_SSE2;
				STORE(dPtr[1], val0);

				c0 = p32_2[0];
				c1 = p32_2[1];
				c2 = p32_2_[0];
				c3 = p32_2_[1];
				OPR_SSE2;
				STORE(dPtr[2], val0);

				c0 = p32_3[0];
				c1 = p32_3[1];
				c2 = p32_3_[0];
				c3 = p32_3_[1];
				OPR_SSE2;
				STORE(dPtr[3], val0);

			}
		}

	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
