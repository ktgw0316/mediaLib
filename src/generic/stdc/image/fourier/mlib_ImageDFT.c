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

#pragma ident	"@(#)mlib_ImageDFT.c	9.2	07/10/09 SMI"

/*
 * FUNCTIONS
 *      mlib_ImageFourierTransform  - 2D Discrete Fourier Transform
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageFourierTransform(mlib_image        *dst,
 *                                             const mlib_image  *src,
 *                                             mlib_fourier_mode mode);
 *
 * ARGUMENT
 *      dst     The pointer to the output or destination image
 *      src     The pointer to the input or source image
 *      mode    The mode of the transform
 *
 * RESTRICTION
 *      The src and dst must be the same type and the same size.
 *      The src and dst can be MLIB_BYTE, MLIB_SHORT, MLIB_USHORT, MLIB_INT,
 *                             MLIB_FLOAT or MLIB_DOUBLE data type.
 *
 *      The src and dst height and width must be some powers of 2
 *
 *      The src and dst can have 1 or 2 channels. If the src has just one
 *      channel the imaginary parts are assumed to be zero. If the dst has
 *      just one channel, it is assumed that the imaginary parts of the
 *      output can be discarded. But in case both src and dst are one-channel
 *      images, MLIB_FAILURE should be returned.
 *
 *      The mode can be one of the following:
 *              MLIB_DFT_SCALE_NONE     (default) -
 *                                      fwd transform without scaling
 *              MLIB_DFT_SCALE_MXN                -
 *                                      fwd transform with scaling of 1/(M*N)
 *              MLIB_DFT_SCALE_SQRT               -
 *                                      fwd transform with scaling of
 *                                      1/sqrt(M*N)
 *              MLIB_IDFT_SCALE_NONE              -
 *                                      inv transform without scaling
 *              MLIB_IDFT_SCALE_MXN               -
 *                                      inv transform with scaling of 1/(M*N)
 *              MLIB_IDFT_SCALE_SQRT              -
 *                                      inv transform with scaling of
 *                                      1/sqrt(M*N)
 *
 * DESCRIPTION
 *      The "FourierTransform" function computes the forward or inverse
 *      Discrete Fourier Transform of an image. The source image may be
 *      one- or two-banded depending on whether the data are real or
 *      complex, respectively. In the case of complex data, the real and
 *      imaginary parts of each pixel are respectively stored in the first
 *      and second bands of the image. The destination image should be
 *      two-banded complex data if the source image is real but may be
 *      either one- or two-banded if the source image is complex depending
 *      on the nature of the images.
 *
 *      The scaling mode of the transform can be no-scaling, scaling down
 *      by the product of the image dimensions, or by the square root of
 *      the product of the image dimensions.
 *
 */

#include <mlib_image.h>
#include <mlib_signal.h>
#include <mlib_SysMath.h>
#include <mlib_ImageCheck.h>
#include <mlib_SignalFFTUtil.h>
#include <mlib_ImageDivTables.h>
#include <mlib_ImageDFT.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageFourierTransform = __mlib_ImageFourierTransform

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_ImageFourierTransform) mlib_ImageFourierTransform
    __attribute__((weak, alias("__mlib_ImageFourierTransform")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	BLK_ORDER	CACHE1_ORD

/* *********************************************************** */

#define	MIN(a, b)	(((a) < (b)) ? (a) : (b))
#define	MAX(a, b)	(((a) > (b)) ? (a) : (b))

/* *********************************************************** */

#define	MSIN_PI_BY4	-0.70710678118654752440084436210484903928483593768846

/* *********************************************************** */

#define	DEF_CVAR(x)	mlib_d64 x##_r, x##_i

/* *********************************************************** */

#define	DEF_CVAR4(x)                                            \
	mlib_d64 x##0_r, x##0_i, x##1_r, x##1_i,                \
	    x##2_r, x##2_i, x##3_r, x##3_i

/* *********************************************************** */

#define	DEF_CVAR8(x)                                            \
	mlib_d64 x##0_r, x##0_i, x##1_r, x##1_i,                \
	    x##2_r, x##2_i, x##3_r, x##3_i, x##4_r,             \
	    x##4_i, x##5_r, x##5_i, x##6_r, x##6_i,             \
	    x##7_r, x##7_i

/* *********************************************************** */

#define	CMUL(z, x, y)                                           \
	z##_r = x##_r * y##_r - x##_i * y##_i;                  \
	z##_i = x##_r * y##_i + x##_i * y##_r

/* *********************************************************** */

#define	CROT_M1(y, x)                                           \
	y##_r = SIN_PI_BY4 * (x##_r + x##_i);                   \
	y##_i = SIN_PI_BY4 * (x##_i - x##_r)

/* *********************************************************** */

#define	CROT_P1(y, x)                                           \
	y##_r = SIN_PI_BY4 * (x##_r - x##_i);                   \
	y##_i = SIN_PI_BY4 * (x##_i + x##_r)

/* *********************************************************** */

#define	CROT_S1(y, x)                                           \
	y##_r = SIN_PI_BY4 * (x##_r - dsign * x##_i);           \
	y##_i = SIN_PI_BY4 * (x##_i + dsign * x##_r)

/* *********************************************************** */

#define	CROT_M3(y, x)                                           \
	y##_r = SIN_PI_BY4 * (x##_i - x##_r);                   \
	y##_i = MSIN_PI_BY4 * (x##_i + x##_r)

/* *********************************************************** */

#define	CROT_P3(y, x)                                           \
	y##_r = MSIN_PI_BY4 * (x##_i + x##_r);                  \
	y##_i = SIN_PI_BY4 * (x##_r - x##_i)

/* *********************************************************** */

#define	CROT_S3(y, x)                                           \
	y##_r = MSIN_PI_BY4 * (dsign * x##_i + x##_r);          \
	y##_i = SIN_PI_BY4 * (dsign * x##_r - x##_i)

/* *********************************************************** */

#define	CADD(z, x, y)	z##_r = x##_r + y##_r; z##_i = x##_i + y##_i

/* *********************************************************** */

#define	CSUB(z, x, y)	z##_r = x##_r - y##_r; z##_i = x##_i - y##_i

/* *********************************************************** */

#define	ISUB(z, x, y)	z##_i = x##_r - y##_r; z##_r = y##_i - x##_i

/* *********************************************************** */

#define	FSUB(z, x, y)	z##_i = y##_r - x##_r; z##_r = x##_i - y##_i

/* *********************************************************** */

#define	SSUB(z, x, y)                                           \
	z##_i = dsign * (x##_r - y##_r);                        \
	z##_r = dsign * (y##_i - x##_i)

/* *********************************************************** */

#define	CBUT(a, b, N0, N1)                                      \
	CADD(b##N0, a##N0, a##N1);                              \
	CSUB(b##N1, a##N0, a##N1)

/* *********************************************************** */

#define	IBUT(a, b, N0, N1)                                      \
	CADD(b##N0, a##N0, a##N1);                              \
	ISUB(b##N1, a##N0, a##N1)

/* *********************************************************** */

#define	FBUT(a, b, N0, N1)                                      \
	CADD(b##N0, a##N0, a##N1);                              \
	FSUB(b##N1, a##N0, a##N1)

/* *********************************************************** */

#define	SBUT(a, b, N0, N1)                                      \
	CADD(b##N0, a##N0, a##N1);                              \
	SSUB(b##N1, a##N0, a##N1)

/* *********************************************************** */

void
mlib_ImageFFT_2FirstLay_col(
    mlib_d64 *pdst,
    mlib_d64 *psrc,
    mlib_s32 dorder,
    mlib_s32 ysize,
    mlib_s32 strided,
    mlib_d64 dsign)
{
	mlib_s32 i, j;

	for (i = 0; i < (1 << dorder); i += 4) {
		mlib_d64 *sp0 = psrc + i * ysize * 2;
		mlib_d64 *sp1 = sp0 + ysize * 2;
		mlib_d64 *sp2 = sp1 + ysize * 2;
		mlib_d64 *sp3 = sp2 + ysize * 2;
		mlib_d64 *dp = pdst + 2 * i;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (j = 0; j < ysize; j++) {
			mlib_d64 *pp = dp + j * strided;

			DEF_CVAR4(a);
			DEF_CVAR4(b);
			DEF_CVAR4(c);

			a0_r = sp0[2 * j];
			a0_i = sp0[2 * j + 1];
			a1_r = sp1[2 * j];
			a1_i = sp1[2 * j + 1];
			a2_r = sp2[2 * j];
			a2_i = sp2[2 * j + 1];
			a3_r = sp3[2 * j];
			a3_i = sp3[2 * j + 1];

			CBUT(a, b, 0, 1);
			SBUT(a, b, 2, 3);
			CBUT(b, c, 0, 2);
			CBUT(b, c, 1, 3);

			pp[0] = c0_r;
			pp[1] = c0_i;
			pp[2] = c1_r;
			pp[3] = c1_i;
			pp[4] = c2_r;
			pp[5] = c2_i;
			pp[6] = c3_r;
			pp[7] = c3_i;
		}
	}
}

/* *********************************************************** */

void
mlib_ImageFFT_3FirstLay_col(
    mlib_d64 *pdst,
    mlib_d64 *psrc,
    mlib_s32 dorder,
    mlib_s32 ysize,
    mlib_s32 strided,
    mlib_d64 dsign)
{
	mlib_s32 i, j;

	for (i = 0; i < (1 << dorder); i += 8) {
		mlib_d64 *sp0 = psrc + i * ysize * 2;
		mlib_d64 *sp1 = sp0 + ysize * 2;
		mlib_d64 *sp2 = sp1 + ysize * 2;
		mlib_d64 *sp3 = sp2 + ysize * 2;
		mlib_d64 *sp4 = sp3 + ysize * 2;
		mlib_d64 *sp5 = sp4 + ysize * 2;
		mlib_d64 *sp6 = sp5 + ysize * 2;
		mlib_d64 *sp7 = sp6 + ysize * 2;
		mlib_d64 *dp = pdst + 2 * i;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (j = 0; j < ysize; j++) {
			mlib_d64 *pp = dp + j * strided;

			DEF_CVAR8(a);
			DEF_CVAR8(b);
			DEF_CVAR8(c);
			DEF_CVAR8(d);
			DEF_CVAR(z5);
			DEF_CVAR(z7);

			a0_r = sp0[2 * j];
			a0_i = sp0[2 * j + 1];
			a1_r = sp1[2 * j];
			a1_i = sp1[2 * j + 1];
			a2_r = sp2[2 * j];
			a2_i = sp2[2 * j + 1];
			a3_r = sp3[2 * j];
			a3_i = sp3[2 * j + 1];
			a4_r = sp4[2 * j];
			a4_i = sp4[2 * j + 1];
			a5_r = sp5[2 * j];
			a5_i = sp5[2 * j + 1];
			a6_r = sp6[2 * j];
			a6_i = sp6[2 * j + 1];
			a7_r = sp7[2 * j];
			a7_i = sp7[2 * j + 1];

/* first stage */
			CBUT(a, b, 0, 1);
			SBUT(a, b, 2, 3);
			CBUT(a, b, 4, 5);
			SBUT(a, b, 6, 7);

/* second stage */
			CBUT(b, c, 0, 2);
			CBUT(b, c, 1, 3);
			SBUT(b, c, 4, 6);
			CBUT(b, z, 5, 7);

			CROT_S1(c5, z5);
			CROT_S3(c7, z7);

/* third stage */
			CBUT(c, d, 0, 4);
			CBUT(c, d, 1, 5);
			CBUT(c, d, 2, 6);
			CBUT(c, d, 3, 7);

			pp[0] = d0_r;
			pp[1] = d0_i;
			pp[2] = d1_r;
			pp[3] = d1_i;
			pp[4] = d2_r;
			pp[5] = d2_i;
			pp[6] = d3_r;
			pp[7] = d3_i;
			pp[8] = d4_r;
			pp[9] = d4_i;
			pp[10] = d5_r;
			pp[11] = d5_i;
			pp[12] = d6_r;
			pp[13] = d6_i;
			pp[14] = d7_r;
			pp[15] = d7_i;
		}
	}
}

/* *********************************************************** */

void
mlib_ImageDFT_copy_col(
    mlib_d64 *pdst,
    mlib_d64 *psrc,
    mlib_s32 dorder,
    mlib_s32 ysize,
    mlib_s32 strided,
    mlib_d64 dsign)
{
	mlib_s32 i, j;

	for (i = 0; i < (1 << dorder); i++) {
		mlib_d64 *sp = psrc + i * ysize * 2;
		mlib_d64 *dp = pdst + 2 * i;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (j = 0; j < ysize; j++) {
			dp[j * strided] = sp[2 * j];
			dp[j * strided + 1] = sp[2 * j + 1];
		}
	}
}

/* *********************************************************** */

void
mlib_Image_first0_blk(
    mlib_d64 *dp,
    mlib_d64 *sp,
    mlib_s32 order,
    mlib_s32 blk_order)
{
	mlib_s32 j;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (j = 0; j < (2 << blk_order); j++) {
		dp[j] = sp[j];
	}
}

/* *********************************************************** */

void
mlib_Image_first1_blk(
    mlib_d64 *dp,
    mlib_d64 *sp,
    mlib_s32 order,
    mlib_s32 blk_order)
{
	mlib_s32 j;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (j = 0; j < (2 << blk_order); j += 4) {
		mlib_d64 x_r = sp[j], x_i = sp[j + 1];
		mlib_d64 y_r = sp[j + 2], y_i = sp[j + 3];

		dp[j] = x_r + y_r;
		dp[j + 1] = x_i + y_i;
		dp[j + 2] = x_r - y_r;
		dp[j + 3] = x_i - y_i;
	}
}

/* *********************************************************** */

void
mlib_Image_first0_blk_r(
    mlib_d64 *dp,
    mlib_d64 *sp,
    mlib_s32 order,
    mlib_s32 blk_order)
{
	mlib_s32 j;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (j = 0; j < (1 << blk_order); j++) {
		dp[2 * j] = sp[j];
		dp[2 * j + 1] = 0;
	}
}

/* *********************************************************** */

void
mlib_Image_first1_blk_r(
    mlib_d64 *dp,
    mlib_d64 *sp,
    mlib_s32 order,
    mlib_s32 blk_order)
{
	mlib_s32 j;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (j = 0; j < (1 << blk_order); j += 2) {
		mlib_d64 x_r = sp[j], y_r = sp[j + 1];

		dp[2 * j] = x_r + y_r;
		dp[2 * j + 1] = 0;
		dp[2 * j + 2] = x_r - y_r;
		dp[2 * j + 3] = 0;
	}
}

/* *********************************************************** */

void
mlib_Image_first1_blk_inp(
    mlib_d64 *dp,
    mlib_s32 order,
    mlib_s32 blk_order)
{
	mlib_s32 j;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (j = 0; j < (2 << blk_order); j += 4) {
		mlib_d64 x_r = dp[j], x_i = dp[j + 1];
		mlib_d64 y_r = dp[j + 2], y_i = dp[j + 3];

		dp[j] = x_r + y_r;
		dp[j + 1] = x_i + y_i;
		dp[j + 2] = x_r - y_r;
		dp[j + 3] = x_i - y_i;
	}
}

/* *********************************************************** */

#define	READ_COLUMNS(stype)                                              \
	{                                                                \
	    for (ii = 0; ii < (1 << dorder); ii++) {                     \
		mlib_s32 i = ib + ii;                                    \
		mlib_s32 ir = REV_INDX_16(i, xshift);                    \
		mlib_##stype *sp =                                       \
		    (mlib_##stype *) psrc + (ir << (nchan_s - 1));       \
		mlib_d64 *dp = cbuffs + ii * (ysize << (nchan_s - 1));   \
	                                                                 \
		if (nchan_s == 1) {                                      \
		    for (j = 0; j < ysize; j++) {                        \
			dp[j] = (mlib_d64)sp[j * strides] * scale;       \
		    }                                                    \
		} else {                                                 \
		    for (j = 0; j < ysize; j++) {                        \
			dp[2 * j] = (mlib_d64)sp[j * strides] * scale;   \
			dp[2 * j + 1] =                                  \
			    (mlib_d64)sp[j * strides + 1] * scale;       \
		    }                                                    \
		}                                                        \
	    }                                                            \
	}

/* *********************************************************** */

#ifdef MLIB_USE_FTOI_CLAMPING

#define	CONV_d64_u8(y, x)                                          \
	y = (((mlib_s32)(x - (mlib_d64)(1u << 31))) >> 24) + 128

#define	CONV_d64_u16(y, x)                                             \
	y = ((((mlib_s32)(x - (32767.5 * 65536.0))) >> 16) ^ 0x8000)

#define	CONV_d64_s16(y, x)	y = ((mlib_s32)(x)) >> 16

#define	CONV_d64_s32(y, x)	y = ((mlib_s32)(x))

#else /* MLIB_USE_FTOI_CLAMPING */

#define	CONV_d64_u8(y, x)                                       \
	if (x >= MLIB_U8_MAX)                                   \
	    x = MLIB_U8_MAX;                                    \
	if (x <= MLIB_U8_MIN)                                   \
	    x = MLIB_U8_MIN;                                    \
	y = (mlib_u8)x

#define	CONV_d64_u16(y, x)                                      \
	if (x >= MLIB_U16_MAX)                                  \
	    x = MLIB_U16_MAX;                                   \
	if (x <= MLIB_U16_MIN)                                  \
	    x = MLIB_U16_MIN;                                   \
	y = (mlib_s16)x

#define	CONV_d64_s16(y, x)                                      \
	if (x >= MLIB_S16_MAX)                                  \
	    x = MLIB_S16_MAX;                                   \
	if (x <= MLIB_S16_MIN)                                  \
	    x = MLIB_S16_MIN;                                   \
	y = (mlib_s16)x

#define	CONV_d64_s32(y, x)                                      \
	if (x >= MLIB_S32_MAX)                                  \
	    x = MLIB_S32_MAX;                                   \
	if (x <= MLIB_S32_MIN)                                  \
	    x = MLIB_S32_MIN;                                   \
	y = (mlib_s32)x

#endif /* MLIB_USE_FTOI_CLAMPING */

/* *********************************************************** */

#define	CONV_d64_f32(y, x)	y = (mlib_f32) x

/* *********************************************************** */

#define	CONV_d64_d64(y, x)	y = x

/* *********************************************************** */

#define	FUNC_MLIB_CONVERT_2_1(dtype)                            \
	void                                                    \
	mlib_ImageConvert_2_1_##dtype(void *pdst, mlib_s32 i,   \
	    mlib_d64 *sp, mlib_s32 xsize, mlib_s32 dorder,      \
	    mlib_s32 strided)                                   \
	{                                                       \
	    mlib_##dtype *dp =                                  \
		(mlib_##dtype *) pdst + i * strided;            \
	    mlib_s32 j, k;                                      \
	                                                        \
	    if (strided == xsize) {                             \
		for (j = 0; j < (xsize << dorder); j++) {       \
		    mlib_d64 x = sp[2 * j];                     \
	                                                        \
		    CONV_d64_##dtype(dp[j], x);                 \
		}                                               \
	    } else {                                            \
		for (k = 0; k < (1 << dorder); k++) {           \
		    for (j = 0; j < xsize; j++) {               \
			mlib_d64 x = sp[2 * j];                 \
	                                                        \
			CONV_d64_##dtype(dp[j], x);             \
		    }                                           \
		    sp += 2 * xsize;                            \
		    dp += strided;                              \
		}                                               \
	    }                                                   \
	}

/* *********************************************************** */

#define	FUNC_MLIB_CONVERT_2_2(dtype)                            \
	void                                                    \
	mlib_ImageConvert_2_2_##dtype(void *pdst, mlib_s32 i,   \
	    mlib_d64 *sp, mlib_s32 xsize, mlib_s32 dorder,      \
	    mlib_s32 strided)                                   \
	{                                                       \
	    mlib_##dtype *dp =                                  \
		(mlib_##dtype *) pdst + i * strided;            \
	    mlib_s32 j, k;                                      \
	                                                        \
	    if (strided == xsize) {                             \
		for (j = 0; j < 2 * (xsize << dorder); j++) {   \
		    mlib_d64 x = sp[j];                         \
	                                                        \
		    CONV_d64_##dtype(dp[j], x);                 \
		}                                               \
	    } else {                                            \
		for (k = 0; k < (1 << dorder); k++) {           \
		    for (j = 0; j < 2 * xsize; j++) {           \
			mlib_d64 x = sp[j];                     \
	                                                        \
			CONV_d64_##dtype(dp[j], x);             \
		    }                                           \
		    sp += 2 * xsize;                            \
		    dp += strided;                              \
		}                                               \
	    }                                                   \
	}

/* *********************************************************** */

    FUNC_MLIB_CONVERT_2_1(u8)
    FUNC_MLIB_CONVERT_2_1(u16)
    FUNC_MLIB_CONVERT_2_1(s16)
    FUNC_MLIB_CONVERT_2_1(s32)
    FUNC_MLIB_CONVERT_2_1(f32)
    FUNC_MLIB_CONVERT_2_1(d64)
    FUNC_MLIB_CONVERT_2_2(u8)
    FUNC_MLIB_CONVERT_2_2(u16)
    FUNC_MLIB_CONVERT_2_2(s16)
    FUNC_MLIB_CONVERT_2_2(s32)
    FUNC_MLIB_CONVERT_2_2(f32)

/* *********************************************************** */

void
mlib_Image_fftKernel_blk(
    void *dat,
    mlib_s32 ord_blk,
    mlib_s32 ord_arr,
    mlib_s32 isign)
{
	if (!ord_arr)
		return;

	if (ord_arr == ord_blk) {
		mlib_c_fftKernel(dat, ord_arr, isign);
	} else {
		fft_param *prm =
		    (isign == DIRECT_FFT) ? mlib_fft_func_F : mlib_fft_func_I;
		mlib_c_fftKernel1(dat, NULL, ord_arr, 2 + (ord_blk & 1),
		    ord_blk, prm, 0);
	}
}

/* *********************************************************** */

mlib_status
mlib_USE_FTOI_CLAMPING()
{
#ifdef MLIB_USE_FTOI_CLAMPING
	return (MLIB_SUCCESS);
#else /* MLIB_USE_FTOI_CLAMPING */
	return (MLIB_FAILURE);
#endif /* MLIB_USE_FTOI_CLAMPING */
}

mlib_s32
mlib_ImageFourier_BLK_ORDER()
{
	return (BLK_ORDER);
}

/* *********************************************************** */

mlib_status
__mlib_ImageFourierTransform(
    mlib_image *dst,
    const mlib_image *src,
    mlib_fourier_mode mode)
{
	mlib_type type;
	mlib_d64 *ptmp, *buff, *cbuffs, *cbuffd;
	void *psrc, *pdst;
	mlib_s32 xsize, ysize, bsize, strides, strided, stridet, nchan_s,
	    nchan_d;
	mlib_s32 xorder = 0, yorder = 0, dorder, blk_order, xshift, isign;
	mlib_d64 dsign, scale;
	mlib_s32 i, ib, ii, j, n, first_lays = 0;
	void (
	    *mlib_Signal_first3_blk) (
	    mlib_d64 *,
	    mlib_d64 *,
	    mlib_s32,
	    mlib_s32);
	void (
	    *mlib_Signal_first3_blk_nbr) (
	    mlib_d64 *,
	    mlib_s32,
	    mlib_s32);
	void (
	    *mlib_firstLays_col) (
	    mlib_d64 *,
	    mlib_d64 *,
	    mlib_s32,
	    mlib_s32,
	    mlib_s32,
	    mlib_d64);
	void (
	    *mlib_last_convert) (
	    void *,
	    mlib_s32,
	    mlib_d64 *,
	    mlib_s32,
	    mlib_s32,
	    mlib_s32);

	MLIB_IMAGE_CHECK(dst);
	MLIB_IMAGE_CHECK(src);
	MLIB_IMAGE_SIZE_EQUAL(dst, src);
	MLIB_IMAGE_TYPE_EQUAL(dst, src);

	MLIB_IMAGE_GET_ALL_PARAMS(dst, type, nchan_d, xsize, ysize, strided,
	    pdst);
	strides = mlib_ImageGetStride(src);
	nchan_s = mlib_ImageGetChannels(src);
	psrc = (mlib_d64 *)mlib_ImageGetData(src);

	if (!((nchan_s == 1 && nchan_d == 2) ||
	    (nchan_s == 2 && nchan_d == 1) || (nchan_s == 2 && nchan_d == 2))) {
		return (MLIB_FAILURE);
	}

	switch (type) {
	case MLIB_BYTE:
		break;

	case MLIB_USHORT:
	case MLIB_SHORT:
		strides /= 2;
		strided /= 2;
		break;

	case MLIB_INT:
	case MLIB_FLOAT:
		strides /= 4;
		strided /= 4;
		break;

	case MLIB_DOUBLE:
		strides /= 8;
		strided /= 8;
		break;

	default:
		return (MLIB_FAILURE);
	}

	n = xsize;
	while ((n >>= 1) > 0)
		xorder++;
	n = ysize;
	while ((n >>= 1) > 0)
		yorder++;

	if (xsize != 1 << xorder || ysize != 1 << yorder)
		return (MLIB_FAILURE);
	xshift = 16 - xorder;

	switch (mode) {
	case MLIB_DFT_SCALE_NONE:
		isign = -1;
		scale = 1.0;
		break;
	case MLIB_DFT_SCALE_MXN:
		isign = -1;
		scale = 1.0 / (1 << (xorder + yorder));
		break;
	case MLIB_DFT_SCALE_SQRT:
		isign = -1;
		scale = 1.0 / (1 << ((xorder + yorder) / 2));
		break;
	case MLIB_IDFT_SCALE_NONE:
		isign = 1;
		scale = 1.0;
		break;
	case MLIB_IDFT_SCALE_MXN:
		isign = 1;
		scale = 1.0 / (1 << (xorder + yorder));
		break;
	case MLIB_IDFT_SCALE_SQRT:
		isign = 1;
		scale = 1.0 / (1 << ((xorder + yorder) / 2));
		break;
	default:
		return (MLIB_FAILURE);
	}

	dsign = (mlib_d64)isign;

	if ((mode == MLIB_DFT_SCALE_SQRT || mode == MLIB_IDFT_SCALE_SQRT) &&
	    ((xorder + yorder) & 1)) {
		scale *= SIN_PI_BY4;
	}
#ifdef MLIB_USE_FTOI_CLAMPING

	if (type == MLIB_BYTE) {
		scale *= (mlib_d64)(1 << 24);
	}

	else if (type == MLIB_USHORT) {
		scale *= (mlib_d64)(1 << 16);
	}

	else if (type == MLIB_SHORT) {
		scale *= (mlib_d64)(1 << 16);
	}
#endif /* MLIB_USE_FTOI_CLAMPING */

	mlib_Signal_first3_blk = (nchan_s == 1) ?
	    (isign <
	    0) ? mlib_SignalFFT_first3_blk_r : mlib_SignalIFFT_first3_blk_r
	    : (isign <
	    0) ? mlib_SignalFFT_first3_blk : mlib_SignalIFFT_first3_blk;
	mlib_Signal_first3_blk_nbr =
	    (isign <
	    0) ? mlib_SignalFFT_first3_blk_nbr : mlib_SignalIFFT_first3_blk_nbr;

	if (yorder == 0)
		mlib_Signal_first3_blk =
		    (nchan_s ==
		    1) ? mlib_Image_first0_blk_r : mlib_Image_first0_blk;

	if (yorder == 1)
		mlib_Signal_first3_blk =
		    (nchan_s ==
		    1) ? mlib_Image_first1_blk_r : mlib_Image_first1_blk;

/*
 * allocate memory buffers for 1-D FFT
 */
	bsize = MAX(ysize, (1 << BLK_ORDER));
	buff = (mlib_d64 *)__mlib_malloc(4 * bsize * sizeof (mlib_d64));

	if (buff == NULL)
		return (MLIB_FAILURE);

	cbuffs = buff;
	cbuffd = buff + 2 * bsize;

	if (nchan_d == 2 && type == MLIB_DOUBLE) {
		ptmp = pdst;
		stridet = strided;
	} else {
		ptmp =
		    (mlib_d64 *)__mlib_malloc(2 * xsize * ysize *
		    sizeof (mlib_d64));

		if (ptmp == NULL)
			return (MLIB_FAILURE);
		stridet = 2 * xsize;
	}

/*
 * process columns
 */

	if (yorder >= 3 && yorder < BLK_ORDER) {
		blk_order = MIN(BLK_ORDER, xorder + yorder);
		dorder = blk_order - yorder;

		first_lays = xorder;
		while (first_lays > BLK_ORDER)
			first_lays -= 3;
		first_lays = (first_lays & 1) + 2;

		if (dorder < first_lays)
			first_lays = 0;
	} else {
		blk_order = yorder;
		dorder = 0;
	}

	switch (first_lays) {
	case 0:
		mlib_firstLays_col = mlib_ImageDFT_copy_col;
		break;
	case 2:
		mlib_firstLays_col = mlib_ImageFFT_2FirstLay_col;
		break;
	case 3:
		mlib_firstLays_col = mlib_ImageFFT_3FirstLay_col;
		break;
	default:
		mlib_firstLays_col = NULL;
		break;
	}

	switch (type) {
	case MLIB_DOUBLE:
		for (ib = 0; ib < xsize; ib += (1 << dorder)) {
			for (ii = 0; ii < (1 << dorder); ii++) {
				mlib_s32 i = ib + ii;
				mlib_s32 ir = REV_INDX_16(i, xshift);
				mlib_d64 *sp =
				    (mlib_d64 *)psrc + (ir << (nchan_s - 1));
				mlib_d64 *dp =
				    cbuffs + ii * (ysize << (nchan_s - 1));

				if (nchan_s == 1) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
					for (j = 0; j < ysize; j++) {
						dp[j] =
						    (mlib_d64)sp[j * strides] *
						    scale;
					}

				} else {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
					for (j = 0; j < ysize; j++) {
						dp[2 * j] =
						    (mlib_d64)sp[j * strides] *
						    scale;
						dp[2 * j + 1] =
						    (mlib_d64)sp[j * strides +
						    1] * scale;
					}
				}
			}

			mlib_Signal_first3_blk(cbuffd, cbuffs, yorder,
			    blk_order);
			mlib_Image_fftKernel_blk(cbuffd, yorder, blk_order,
			    isign);
			mlib_firstLays_col(ptmp + 2 * ib, cbuffd, dorder, ysize,
			    stridet, dsign);
		}

		break;

	case MLIB_FLOAT:
		for (ib = 0; ib < xsize; ib += (1 << dorder)) {
			for (ii = 0; ii < (1 << dorder); ii++) {
				mlib_s32 i = ib + ii;
				mlib_s32 ir = REV_INDX_16(i, xshift);
				mlib_f32 *sp =
				    (mlib_f32 *)psrc + (ir << (nchan_s - 1));
				mlib_d64 *dp =
				    cbuffs + ii * (ysize << (nchan_s - 1));

				if (nchan_s == 1) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
					for (j = 0; j < ysize; j++) {
						dp[j] =
						    (mlib_d64)sp[j * strides] *
						    scale;
					}

				} else {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
					for (j = 0; j < ysize; j++) {
						dp[2 * j] =
						    (mlib_d64)sp[j * strides] *
						    scale;
						dp[2 * j + 1] =
						    (mlib_d64)sp[j * strides +
						    1] * scale;
					}
				}
			}

			mlib_Signal_first3_blk(cbuffd, cbuffs, yorder,
			    blk_order);
			mlib_Image_fftKernel_blk(cbuffd, yorder, blk_order,
			    isign);
			mlib_firstLays_col(ptmp + 2 * ib, cbuffd, dorder, ysize,
			    stridet, dsign);
		}

		break;

	case MLIB_INT:
		for (ib = 0; ib < xsize; ib += (1 << dorder)) {
			for (ii = 0; ii < (1 << dorder); ii++) {
				mlib_s32 i = ib + ii;
				mlib_s32 ir = REV_INDX_16(i, xshift);
				mlib_s32 *sp =
				    (mlib_s32 *)psrc + (ir << (nchan_s - 1));
				mlib_d64 *dp =
				    cbuffs + ii * (ysize << (nchan_s - 1));

				if (nchan_s == 1) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
					for (j = 0; j < ysize; j++) {
						dp[j] =
						    (mlib_d64)sp[j * strides] *
						    scale;
					}

				} else {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
					for (j = 0; j < ysize; j++) {
						dp[2 * j] =
						    (mlib_d64)sp[j * strides] *
						    scale;
						dp[2 * j + 1] =
						    (mlib_d64)sp[j * strides +
						    1] * scale;
					}
				}
			}

			mlib_Signal_first3_blk(cbuffd, cbuffs, yorder,
			    blk_order);
			mlib_Image_fftKernel_blk(cbuffd, yorder, blk_order,
			    isign);
			mlib_firstLays_col(ptmp + 2 * ib, cbuffd, dorder, ysize,
			    stridet, dsign);
		}

		break;

	case MLIB_SHORT:
		for (ib = 0; ib < xsize; ib += (1 << dorder)) {
			for (ii = 0; ii < (1 << dorder); ii++) {
				mlib_s32 i = ib + ii;
				mlib_s32 ir = REV_INDX_16(i, xshift);
				mlib_s16 *sp =
				    (mlib_s16 *)psrc + (ir << (nchan_s - 1));
				mlib_d64 *dp =
				    cbuffs + ii * (ysize << (nchan_s - 1));

				if (nchan_s == 1) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
					for (j = 0; j < ysize; j++) {
						dp[j] =
						    (mlib_d64)sp[j * strides] *
						    scale;
					}

				} else {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
					for (j = 0; j < ysize; j++) {
						dp[2 * j] =
						    (mlib_d64)sp[j * strides] *
						    scale;
						dp[2 * j + 1] =
						    (mlib_d64)sp[j * strides +
						    1] * scale;
					}
				}
			}

			mlib_Signal_first3_blk(cbuffd, cbuffs, yorder,
			    blk_order);
			mlib_Image_fftKernel_blk(cbuffd, yorder, blk_order,
			    isign);
			mlib_firstLays_col(ptmp + 2 * ib, cbuffd, dorder, ysize,
			    stridet, dsign);
		}

		break;

	case MLIB_USHORT:
		for (ib = 0; ib < xsize; ib += (1 << dorder)) {
			for (ii = 0; ii < (1 << dorder); ii++) {
				mlib_s32 i = ib + ii;
				mlib_s32 ir = REV_INDX_16(i, xshift);
				mlib_u16 *sp =
				    (mlib_u16 *)psrc + (ir << (nchan_s - 1));
				mlib_d64 *dp =
				    cbuffs + ii * (ysize << (nchan_s - 1));

				if (nchan_s == 1) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
					for (j = 0; j < ysize; j++) {
						dp[j] =
						    (mlib_d64)sp[j * strides] *
						    scale;
					}

				} else {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
					for (j = 0; j < ysize; j++) {
						dp[2 * j] =
						    (mlib_d64)sp[j * strides] *
						    scale;
						dp[2 * j + 1] =
						    (mlib_d64)sp[j * strides +
						    1] * scale;
					}
				}
			}

			mlib_Signal_first3_blk(cbuffd, cbuffs, yorder,
			    blk_order);
			mlib_Image_fftKernel_blk(cbuffd, yorder, blk_order,
			    isign);
			mlib_firstLays_col(ptmp + 2 * ib, cbuffd, dorder, ysize,
			    stridet, dsign);
		}

		break;

	case MLIB_BYTE:
		for (ib = 0; ib < xsize; ib += (1 << dorder)) {
			for (ii = 0; ii < (1 << dorder); ii++) {
				mlib_s32 i = ib + ii;
				mlib_s32 ir = REV_INDX_16(i, xshift);
				mlib_u8 *sp =
				    (mlib_u8 *)psrc + (ir << (nchan_s - 1));
				mlib_d64 *dp =
				    cbuffs + ii * (ysize << (nchan_s - 1));

				if (nchan_s == 1) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
					for (j = 0; j < ysize; j++) {
						dp[j] =
						    mlib_U82D64[sp[j *
						    strides]] * scale;
					}

				} else {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
					for (j = 0; j < ysize; j++) {
						dp[2 * j] =
						    mlib_U82D64[sp[j *
						    strides]] * scale;
						dp[2 * j + 1] =
						    mlib_U82D64[sp[j * strides +
						    1]] * scale;
					}
				}
			}

			mlib_Signal_first3_blk(cbuffd, cbuffs, yorder,
			    blk_order);
			mlib_Image_fftKernel_blk(cbuffd, yorder, blk_order,
			    isign);
			mlib_firstLays_col(ptmp + 2 * ib, cbuffd, dorder, ysize,
			    stridet, dsign);
		}

		break;

	default:
		break;
	}

/*
 * process lines
 */

	if (xorder < BLK_ORDER && stridet == 2 * xsize) {
		blk_order = MIN(BLK_ORDER, xorder + yorder);
	} else {
		blk_order = xorder;
	}

	dorder = blk_order - xorder;

	if (xorder == 0)
		first_lays = -1;
	if (xorder == 1)
		mlib_Signal_first3_blk_nbr = mlib_Image_first1_blk_inp;

	if (nchan_d == 1) {
		switch (type) {
		case MLIB_BYTE:
			mlib_last_convert = mlib_ImageConvert_2_1_u8;
			break;
		case MLIB_USHORT:
			mlib_last_convert = mlib_ImageConvert_2_1_u16;
			break;
		case MLIB_SHORT:
			mlib_last_convert = mlib_ImageConvert_2_1_s16;
			break;
		case MLIB_INT:
			mlib_last_convert = mlib_ImageConvert_2_1_s32;
			break;
		case MLIB_FLOAT:
			mlib_last_convert = mlib_ImageConvert_2_1_f32;
			break;
		case MLIB_DOUBLE:
			mlib_last_convert = mlib_ImageConvert_2_1_d64;
			break;
		default:
			mlib_last_convert = NULL;
			break;
		}

	} else {
		switch (type) {
		case MLIB_BYTE:
			mlib_last_convert = mlib_ImageConvert_2_2_u8;
			break;
		case MLIB_USHORT:
			mlib_last_convert = mlib_ImageConvert_2_2_u16;
			break;
		case MLIB_SHORT:
			mlib_last_convert = mlib_ImageConvert_2_2_s16;
			break;
		case MLIB_INT:
			mlib_last_convert = mlib_ImageConvert_2_2_s32;
			break;
		case MLIB_FLOAT:
			mlib_last_convert = mlib_ImageConvert_2_2_f32;
			break;
		case MLIB_DOUBLE:
			mlib_last_convert = NULL;
			break;
		default:
			mlib_last_convert = NULL;
			break;
		}
	}

	for (i = 0; i < ysize; i += (1 << dorder)) {
		mlib_d64 *sp = ptmp + i * stridet;

		if (first_lays == 0) {
			mlib_Signal_first3_blk_nbr(sp, xorder, blk_order);
		}

		mlib_Image_fftKernel_blk(sp, xorder, blk_order, isign);

		if (mlib_last_convert != NULL) {
			mlib_last_convert(pdst, i, sp, xsize, dorder, strided);
		}
	}

	if ((mlib_u8 *)ptmp != pdst)
		__mlib_free(ptmp);
	__mlib_free(buff);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
