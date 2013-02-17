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

#pragma ident	"@(#)mlib_s_ImageMean_Fp.c	9.5	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_ImageMean_Fp - calculates image mean for the input image
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageMean_Fp(mlib_d64         *mean,
 *                                    const mlib_image *src)
 *
 * ARGUMENTS
 *  mean  Pointer to mean array
 *  src   Pointer to input image
 *
 * DESCRIPTION
 *
 *           1    w-1 h-1
 *      m = --- * SUM SUM x(i, j)
 *          w*h   i = 0 j = 0
 *
 * RESTRICTION
 *      img can have 1, 2, 3 or 4 channels. It can be of MLIB_FLOAT or
 *      MLIB_DOUBLE data type.
 */

#include <mlib_image.h>
#include <mlib_ImageCheck.h>
#ifdef __SUNPRO_C
#include <sunmedia_intrin.h>
#else
#include <emmintrin.h>
#endif

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageMean_Fp = __mlib_ImageMean_Fp

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_ImageMean_Fp) mlib_ImageMean_Fp
    __attribute__((weak, alias("__mlib_ImageMean_Fp")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

static void mlib_ImageMeanD64Case(
    const mlib_image *src,
    mlib_d64 *mean);

static void mlib_ImageMeanF32Case(
    const mlib_image *src,
    mlib_d64 *mean);

/* *********************************************************** */

mlib_status
__mlib_ImageMean_Fp(
    mlib_d64 *mean,
    const mlib_image *src)
{
/* check for obvious errors */
	MLIB_IMAGE_CHECK(src);

	if (mean == NULL)
		return (MLIB_NULLPOINTER);

	switch (mlib_ImageGetType(src)) {

/* handle MLIB_FLOAT data type of image */
	case MLIB_FLOAT:
		mlib_ImageMeanF32Case(src, mean);
		return (MLIB_SUCCESS);

/* handle MLIB_DOUBLE data type of image */
	case MLIB_DOUBLE:
		mlib_ImageMeanD64Case(src, mean);
		return (MLIB_SUCCESS);

/* discard any other data types */
	default:
		return (MLIB_FAILURE);
	}
}

/* *********************************************************** */

static void
mlib_ImageMeanD64Case(
    const mlib_image *src,
    mlib_d64 *mean)
{
/* pointer for pixel of source */
	mlib_d64 *srcPixelPtr = (mlib_d64 *)mlib_ImageGetData(src);

/* pointer for line of source */
	mlib_u8 *srcLinePtr = (mlib_u8 *)srcPixelPtr;

/* width of source image */
	mlib_s32 srcWidth = mlib_ImageGetWidth(src);

/* height of source image */
	mlib_s32 srcHeight = mlib_ImageGetHeight(src);

/* linestride = bytes to next row */
	mlib_s32 srcYStride = mlib_ImageGetStride(src);

/* number of channels */
	mlib_s32 numOfChannels = mlib_ImageGetChannels(src);

/* total number of pixels */
	mlib_s32 srcTotalPixNum = srcHeight * srcWidth;
	mlib_d64 divider = 1.0 / (mlib_d64)srcTotalPixNum;

/* indices */
	mlib_s32 i, j, k;

	if (8 * numOfChannels * srcWidth == srcYStride) {
		srcWidth = srcTotalPixNum;
		srcHeight = 1;
	}

	switch (numOfChannels) {
	case 1: {
		mlib_d64 *sl;
		__m128d load0, load1, load2, load3;
		__m128d summa0, summa1, summa2, summa3;
		mlib_d64 buffer0, buffer1, buffer2, buffer3;
		mlib_d64 accum0[2], accum1[2], accum2[2], accum3[2];

		buffer0 = buffer1 = buffer2 = buffer3 = 0.0;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < srcHeight; i++) {

			summa0 = _mm_setzero_pd();
			summa1 = _mm_setzero_pd();
			summa2 = _mm_setzero_pd();
			summa3 = _mm_setzero_pd();
			j = 0;

			if (0 != ((mlib_addr)srcPixelPtr & 15)) {
				buffer0 += srcPixelPtr[j++];
			}

			sl = srcPixelPtr + j;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			/* loop unrolling */
			for (k = 0; k < (srcWidth - j) >> 3; k++) {
				load0 = _mm_load_pd(sl);
				load1 = _mm_load_pd(sl + 2);
				load2 = _mm_load_pd(sl + 4);
				load3 = _mm_load_pd(sl + 6);
				summa0 = _mm_add_pd(summa0, load0);
				summa1 = _mm_add_pd(summa1, load1);
				summa2 = _mm_add_pd(summa2, load2);
				summa3 = _mm_add_pd(summa3, load3);
				sl += 8;
			}

			j += k << 3;

			if (j < srcWidth - 3) {
				load0 = _mm_load_pd(sl);
				load1 = _mm_load_pd(sl + 2);
				summa0 = _mm_add_pd(summa0, load0);
				summa1 = _mm_add_pd(summa1, load1);
				sl += 4;
				j += 4;
			}

			if (j < srcWidth - 1) {
				load0 = _mm_load_pd(sl);
				summa0 = _mm_add_pd(summa0, load0);
				sl += 2;
				j += 2;
			}

			_mm_storeu_pd(accum0, summa0);
			_mm_storeu_pd(accum1, summa1);
			_mm_storeu_pd(accum2, summa2);
			_mm_storeu_pd(accum3, summa3);

			buffer0 += accum0[0] + accum0[1];
			buffer1 += accum1[0] + accum1[1];
			buffer2 += accum2[0] + accum2[1];
			buffer3 += accum3[0] + accum3[1];

			if (j < srcWidth) {
				buffer0 += srcPixelPtr[j];
			}

			srcLinePtr += srcYStride;
			srcPixelPtr = (mlib_d64 *)srcLinePtr;
		}

		buffer0 += buffer1 + buffer2 + buffer3;

		mean[0] = buffer0 * divider;

		break;
	} /* case 1 */

	case 2: {
		mlib_d64 *sl;
		__m128d load0, load1, load2, load3;
		__m128d summa0, summa1, summa2, summa3;
		mlib_d64 buffer0, buffer1, buffer2, buffer3;
		mlib_d64 buffer4, buffer5, buffer6, buffer7;
		mlib_d64 accum0[2], accum1[2], accum2[2], accum3[2];

		buffer0 = buffer1 = buffer2 = buffer3 = 0.0;

/*   ImageWidth * ChannelNumber   */
		srcWidth <<= 1;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < srcHeight; i++) {
			mlib_s32 alignment = 1;
			mlib_s32 j = 0;

			summa0 = _mm_setzero_pd();
			summa1 = _mm_setzero_pd();
			summa2 = _mm_setzero_pd();
			summa3 = _mm_setzero_pd();
			buffer4 = buffer5 = buffer6 = buffer7 = 0.0;

			if (0 != ((mlib_addr)srcPixelPtr & 15)) {
				alignment = 0;
				buffer0 += srcPixelPtr[0];
				j = 1;
			}

			sl = srcPixelPtr + j;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			/* loop unrolling */
			for (k = 0; k < (srcWidth - j) >> 3; k++) {
				load0 = _mm_load_pd(sl);
				load1 = _mm_load_pd(sl + 2);
				load2 = _mm_load_pd(sl + 4);
				load3 = _mm_load_pd(sl + 6);
				summa0 = _mm_add_pd(summa0, load0);
				summa1 = _mm_add_pd(summa1, load1);
				summa2 = _mm_add_pd(summa2, load2);
				summa3 = _mm_add_pd(summa3, load3);
				sl += 8;
			}

			summa0 = _mm_add_pd(summa0, summa2);
			summa1 = _mm_add_pd(summa1, summa3);

			j += k << 3;

			if (j < (srcWidth - 3)) {
				load0 = _mm_load_pd(sl);
				load1 = _mm_load_pd(sl + 2);
				summa0 = _mm_add_pd(summa0, load0);
				summa1 = _mm_add_pd(summa1, load1);
				j += 4;
				sl += 4;
			}

			if (j < srcWidth - 1) {
				load0 = _mm_load_pd(sl);
				summa0 = _mm_add_pd(summa0, load0);
				j += 2;
				sl += 2;
			}

			_mm_storeu_pd(accum0, summa0);
			_mm_storeu_pd(accum1, summa1);

			buffer4 += accum0[0];
			buffer5 += accum0[1];
			buffer6 += accum1[0];
			buffer7 += accum1[1];

			if (alignment) {
				buffer0 += buffer4;
				buffer1 += buffer5;
				buffer2 += buffer6;
				buffer3 += buffer7;

			} else {

				buffer1 += buffer4;
				buffer0 += buffer5;
				buffer3 += buffer6;
				buffer2 += buffer7;

				buffer1 += srcPixelPtr[j];
			}

			srcLinePtr += srcYStride;
			srcPixelPtr = (mlib_d64 *)srcLinePtr;
		}

		buffer0 += buffer2;
		buffer1 += buffer3;

		mean[0] = buffer0 * divider;
		mean[1] = buffer1 * divider;

		break;
	} /* case 2 */

	case 3: {
		mlib_d64 *sl;
		__m128d load0, load1, load2;
		__m128d summa0, summa1, summa2;
		mlib_d64 buffer0, buffer1, buffer2;
		mlib_d64 buffer4, buffer5, buffer6;
		mlib_d64 accum0[2], accum1[2], accum2[2];

		buffer0 = buffer1 = buffer2 = 0.0;

/*   ImageWidth * ChannelNumber   */
		srcWidth *= 3;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < srcHeight; i++) {
			mlib_s32 alignment = 1;
			mlib_s32 j = 0;

			summa0 = _mm_setzero_pd();
			summa1 = _mm_setzero_pd();
			summa2 = _mm_setzero_pd();
			buffer4 = buffer5 = buffer6 = 0.0;

			if (0 != ((mlib_addr)srcPixelPtr & 15)) {
				alignment = 0;
				buffer0 += srcPixelPtr[0];
				j = 1;
			}

			sl = srcPixelPtr + j;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			/* loop unrolling */
			for (k = 0; k < (srcWidth - j - 5); k += 6, sl += 6) {
				load0 = _mm_load_pd(sl);
				load1 = _mm_load_pd(sl + 2);
				load2 = _mm_load_pd(sl + 4);
				summa0 = _mm_add_pd(summa0, load0);
				summa1 = _mm_add_pd(summa1, load1);
				summa2 = _mm_add_pd(summa2, load2);
			}

			j += k;

			_mm_storeu_pd(accum0, summa0);
			_mm_storeu_pd(accum1, summa1);
			_mm_storeu_pd(accum2, summa2);

			buffer4 += accum0[0] + accum1[1];
			buffer5 += accum0[1] + accum2[0];
			buffer6 += accum1[0] + accum2[1];

			if (alignment) {
				buffer0 += buffer4;
				buffer1 += buffer5;
				buffer2 += buffer6;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (; j < srcWidth; j += 3) {
					buffer0 += srcPixelPtr[j + 0];
					buffer1 += srcPixelPtr[j + 1];
					buffer2 += srcPixelPtr[j + 2];
				}

			} else {

				buffer1 += buffer4;
				buffer2 += buffer5;
				buffer0 += buffer6;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (; j < (srcWidth - 2); j += 3) {
					buffer1 += srcPixelPtr[j + 0];
					buffer2 += srcPixelPtr[j + 1];
					buffer0 += srcPixelPtr[j + 2];
				}

				buffer1 += srcPixelPtr[j + 0];
				buffer2 += srcPixelPtr[j + 1];
			}

			srcLinePtr += srcYStride;
			srcPixelPtr = (mlib_d64 *)srcLinePtr;
		}

		mean[0] = buffer0 * divider;
		mean[1] = buffer1 * divider;
		mean[2] = buffer2 * divider;

		break;
	} /* case 3 */

	case 4: {
		mlib_d64 *sl;
		__m128d load0, load1, load2, load3;
		__m128d summa0, summa1, summa2, summa3;
		mlib_d64 buffer0, buffer1, buffer2, buffer3;
		mlib_d64 buffer[4];

		buffer0 = buffer1 = buffer2 = buffer3 = 0.0;

/*   ImageWidth * ChannelNumber   */
		srcWidth <<= 2;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < srcHeight; i++) {
			mlib_s32 alignment = 1;
			mlib_s32 j = 0;

			summa0 = _mm_setzero_pd();
			summa1 = _mm_setzero_pd();
			summa2 = _mm_setzero_pd();
			summa3 = _mm_setzero_pd();

			buffer[0] = buffer[1] = buffer[2] = buffer[3] = 0.0;

			if (0 != ((mlib_addr)srcPixelPtr & 15)) {
				alignment = 0;
				buffer0 += srcPixelPtr[0];
				j = 1;
			}


			sl = srcPixelPtr + j;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			/* loop unrolling */
			for (k = 0; k < (srcWidth - j) >> 3; k++, sl += 8) {
				load0 = _mm_load_pd(sl);
				load1 = _mm_load_pd(sl + 2);
				summa0 = _mm_add_pd(summa0, load0);
				summa1 = _mm_add_pd(summa1, load1);

				load2 = _mm_load_pd(sl + 4);
				load3 = _mm_load_pd(sl + 6);
				summa2 = _mm_add_pd(summa2, load2);
				summa3 = _mm_add_pd(summa3, load3);
			}
			summa0 = _mm_add_pd(summa0, summa2);
			summa1 = _mm_add_pd(summa1, summa3);

			j += k << 3;

			if (j < (srcWidth - 3)) {
				load0 = _mm_load_pd(sl);
				load1 = _mm_load_pd(sl + 2);
				summa0 = _mm_add_pd(summa0, load0);
				summa1 = _mm_add_pd(summa1, load1);
				j += 4;
				sl += 4;
			}

			_mm_storeu_pd((buffer + 0), summa0);
			_mm_storeu_pd((buffer + 2), summa1);

			if (alignment) {
				buffer0 += buffer[0];
				buffer1 += buffer[1];
				buffer2 += buffer[2];
				buffer3 += buffer[3];

			} else {
				buffer1 += buffer[0];
				buffer2 += buffer[1];
				buffer3 += buffer[2];
				buffer0 += buffer[3];

				buffer1 += srcPixelPtr[j++];
				buffer2 += srcPixelPtr[j++];
				buffer3 += srcPixelPtr[j++];
			}

			srcLinePtr += srcYStride;
			srcPixelPtr = (mlib_d64 *)srcLinePtr;
		}

		mean[0] = buffer0 * divider;
		mean[1] = buffer1 * divider;
		mean[2] = buffer2 * divider;
		mean[3] = buffer3 * divider;
		break;
	} /* case 4 */

	} /* switch (numOfChannels) */
}

/* *********************************************************** */

static void
mlib_ImageMeanF32Case(
    const mlib_image *src,
    mlib_d64 *mean)
{
/* pointer for pixel of source */
	mlib_f32 *srcPixelPtr = (mlib_f32 *)mlib_ImageGetData(src);

/* pointer for line of source */
	mlib_u8 *srcLinePtr = (mlib_u8 *)srcPixelPtr;

/* width of source image */
	mlib_s32 srcWidth = mlib_ImageGetWidth(src);

/* height of source image */
	mlib_s32 srcHeight = mlib_ImageGetHeight(src);

/* linestride = bytes to next row */
	mlib_s32 srcYStride = mlib_ImageGetStride(src);

/* number of channels */
	mlib_s32 numOfChannels = mlib_ImageGetChannels(src);

/* total number of pixels */
	mlib_s32 srcTotalPixNum = srcHeight * srcWidth;
	mlib_d64 divider = 1.0 / srcTotalPixNum;

/* indices */
	mlib_s32 i, j, k;
	mlib_d64 firstAccumulator = 0.0;
	mlib_d64 secondAccumulator = 0.0;
	mlib_d64 thirdAccumulator = 0.0;
	mlib_d64 forthAccumulator = 0.0;

	const __m128d _x_dzero = _mm_setzero_pd();

	if (4 * numOfChannels * srcWidth == srcYStride) {
		srcWidth = srcTotalPixNum;
		srcHeight = 1;
	}

	switch (numOfChannels) {

	case 1: {
		mlib_f32 *sl;
		__m128 load0, load1;
		__m128 load2, load3;
		__m128 suma0, suma1;
		__m128 suma2, suma3;
		mlib_d64 buffer0 = 0.0;
		mlib_f32 accum0[4];

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < srcHeight; i++) {

			suma0 = _mm_setzero_ps();
			suma1 = _mm_setzero_ps();
			suma2 = _mm_setzero_ps();
			suma3 = _mm_setzero_ps();

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j = 0; (j < srcWidth) &&
			    ((mlib_addr)((mlib_f32 *)srcPixelPtr + j) & 15);
			    j++) {
				buffer0 += srcPixelPtr[j];
			}

			sl = srcPixelPtr + j;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		    for (k = 0; k < (srcWidth - j) >> 4; k++, sl += 16) {
				load0 = _mm_load_ps(sl);
				load1 = _mm_load_ps(sl + 4);
				load2 = _mm_load_ps(sl + 8);
				load3 = _mm_load_ps(sl + 12);

				suma0 = _mm_add_ps(suma0, load0);
				suma1 = _mm_add_ps(suma1, load1);
				suma2 = _mm_add_ps(suma2, load2);
				suma3 = _mm_add_ps(suma3, load3);
		    }

			j += k << 4;

		    if (j <= (srcWidth - 12)) {
				load0 = _mm_load_ps(sl);
				load1 = _mm_load_ps(sl + 4);
				load2 = _mm_load_ps(sl + 8);
				suma0 = _mm_add_ps(suma0, load0);
				suma1 = _mm_add_ps(suma1, load1);
				suma2 = _mm_add_ps(suma2, load2);

				j += 12;
				sl += 12;
		    }

		    if (j <= (srcWidth - 8)) {
				load0 = _mm_load_ps(sl);
				load1 = _mm_load_ps(sl + 4);
				suma0 = _mm_add_ps(suma0, load0);
				suma1 = _mm_add_ps(suma1, load1);

				j += 8;
				sl += 8;
		    }

		    if (j <= (srcWidth - 4)) {
				load0 = _mm_load_ps(sl);
				load1 = _mm_movehl_ps(load0, load0);
				suma0 = _mm_add_ps(suma0, load0);
				j += 4;
				sl += 4;
		    }

			suma0 = _mm_add_ps(suma0, suma1);
			suma2 = _mm_add_ps(suma2, suma3);
			suma0 = _mm_add_ps(suma0, suma2);

			_mm_storeu_ps(accum0, suma0);
			buffer0 +=
			    accum0[0] + accum0[1] + accum0[2] + accum0[3];

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		    for (; j < srcWidth; j++) {
				buffer0 += srcPixelPtr[j];
			}

		    srcLinePtr += srcYStride;
		    srcPixelPtr = (mlib_f32 *)srcLinePtr;
	    }

	    mean[0] = buffer0 * divider;

		break;
	} /* case 1 */

	case 2: {
		__m128 load0, load1, load2, load3;
		__m128 summa0, summa1, summa2, summa3;
		mlib_f32 *sl;

		mlib_d64 buffer0 = 0.0;
		mlib_d64 buffer1 = 0.0;
		mlib_d64 buffer2 = 0.0;
		mlib_d64 buffer3 = 0.0;
		mlib_f32 accum0[4];

/*   ImageWidth * ChannelNumber   */
		srcWidth <<= 1;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < srcHeight; i++) {
			mlib_s32 alignment = 1;
			mlib_s32 j = 0;
			summa0 = _mm_setzero_ps();
			summa1 = _mm_setzero_ps();
			summa2 = _mm_setzero_ps();
			summa3 = _mm_setzero_ps();

			if ((mlib_addr)srcPixelPtr & 1) {
				alignment = 0;
				buffer0 += srcPixelPtr[0];

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (j = 1;	(j < (srcWidth - 2)) &&
				    ((mlib_addr)(srcPixelPtr + j) & 15);
				    j += 2) {
					buffer1 += srcPixelPtr[j + 0];
					buffer0 += srcPixelPtr[j + 1];
				}

			} else {

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (j = 0;	(j < srcWidth) &&
				    ((mlib_addr)(srcPixelPtr + j) & 15);
				    j += 2) {
					buffer0 += srcPixelPtr[j + 0];
					buffer1 += srcPixelPtr[j + 1];
				}
			}

			sl = srcPixelPtr + j;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		    for (k = 0; k < (srcWidth - j) >> 4; k++, sl += 16) {
				load0 = _mm_load_ps(sl);
				load1 = _mm_load_ps(sl + 4);
				load2 = _mm_load_ps(sl + 8);
				load3 = _mm_load_ps(sl + 12);

				summa0 =
				    _mm_add_ps(summa0, load0);
				summa1 =
				    _mm_add_ps(summa1, load1);
				summa2 =
				    _mm_add_ps(summa2, load2);
				summa3 =
				    _mm_add_ps(summa3, load3);
			}

			j += k << 4;

		    if (j <= (srcWidth - 12)) {
				load0 = _mm_load_ps(sl);
				load1 = _mm_load_ps(sl + 4);
				load2 = _mm_load_ps(sl + 8);

				summa0 =
				    _mm_add_ps(summa0, load0);
				summa1 =
				    _mm_add_ps(summa1, load1);
				summa2 =
				    _mm_add_ps(summa2, load2);
				j += 12;
				sl += 12;
			}

		    if (j <= (srcWidth - 8)) {
				load0 = _mm_load_ps(sl);
				load1 = _mm_load_ps(sl + 4);

				summa0 =
				    _mm_add_ps(summa0, load0);
				summa1 =
				    _mm_add_ps(summa1, load1);
				j += 8;
				sl += 8;
			}

		    if (j <= (srcWidth - 4)) {
				load0 = _mm_load_ps(sl);

				summa0 =
				    _mm_add_ps(summa0, load0);
				j += 4;
				sl += 4;
			}

			summa0 =
			    _mm_add_ps(summa0, summa2);
			summa1 =
			    _mm_add_ps(summa1, summa3);
			summa0 =
			    _mm_add_ps(summa0, summa1);

			_mm_storeu_ps((void *)accum0, summa0);

			buffer2 += accum0[0] + accum0[2];
			buffer3 += accum0[1] + accum0[3];

			if (alignment) {
				buffer0 += buffer2;
				buffer1 += buffer3;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (; j < srcWidth; j += 2) {
					buffer0 += srcPixelPtr[j + 0];
					buffer1 += srcPixelPtr[j + 1];
				}

			} else {
				buffer1 += buffer2;
				buffer0 += buffer3;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (; j < (srcWidth - 1); j += 2) {
					buffer1 += srcPixelPtr[j + 0];
					buffer0 += srcPixelPtr[j + 1];
				}

				buffer1 += srcPixelPtr[j];
			}

			srcLinePtr += srcYStride;
			srcPixelPtr = (mlib_f32 *)srcLinePtr;

			buffer2 = buffer3 = 0.0;
		}

		mean[0] = buffer0 * divider;
		mean[1] = buffer1 * divider;

		break;
	} /* case 2 */

	case 3: {
		__m128 load0, load1, load2, load3, load4, load5;
		__m128 summa0, summa1, summa2;
		mlib_f32 *sl;

		mlib_d64 buffer0 = 0.0;
		mlib_d64 buffer1 = 0.0;
		mlib_d64 buffer2 = 0.0;
		mlib_d64 buffer3 = 0.0;
		mlib_d64 buffer4 = 0.0;
		mlib_d64 buffer5 = 0.0;

		mlib_f32 buff0[4], buff1[4], buff2[4];

		srcWidth *= 3;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < srcHeight; i++) {
			mlib_s32 sign = 0;
			mlib_s32 j = 0;
			mlib_s32 alignment =
			    ((16 - ((mlib_addr)srcPixelPtr & 15)) & 15) >> 2;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j = 0; (j < (srcWidth - 3)) &&
			    (j <= (alignment - 3));	j += 3) {
				buffer0 += srcPixelPtr[j + 0];
				buffer1 += srcPixelPtr[j + 1];
				buffer2 += srcPixelPtr[j + 2];
			}

			if ((alignment - j) == 2) {
				buffer0 += srcPixelPtr[j + 0];
				buffer1 += srcPixelPtr[j + 1];
				j += 2;
				sign = 1;
			} else if ((alignment - j) == 1) {
				buffer0 += srcPixelPtr[j];
				j += 1;
				sign = 2;
			}

			sl = srcPixelPtr + j;

			summa0 = summa1 = summa2 = _mm_setzero_ps();

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		    for (k = 0; k < (srcWidth - j) / 24; k++, sl += 24) {
				load0 = _mm_load_ps(sl);
				load1 = _mm_load_ps(sl + 4);
				load2 = _mm_load_ps(sl + 8);

				summa0 = _mm_add_ps(summa0, load0);
				summa1 = _mm_add_ps(summa1, load1);
				summa2 = _mm_add_ps(summa2, load2);

				load3 = _mm_load_ps(sl + 12);
				load4 = _mm_load_ps(sl + 16);
				load5 = _mm_load_ps(sl + 20);

				summa0 = _mm_add_ps(summa0, load3);
				summa1 = _mm_add_ps(summa1, load4);
				summa2 = _mm_add_ps(summa2, load5);
			}

			j += k * 24;

		    if (j <= (srcWidth - 12)) {
				load0 = _mm_load_ps(sl);
				load1 = _mm_load_ps(sl + 4);
				load2 = _mm_load_ps(sl + 8);

				summa0 = _mm_add_ps(summa0, load0);
				summa1 = _mm_add_ps(summa1, load1);
				summa2 = _mm_add_ps(summa2, load2);
				j += 12;
				sl += 12;
			}

			_mm_storeu_ps((void *)buff0, summa0);
			_mm_storeu_ps((void *)buff1, summa1);
			_mm_storeu_ps((void *)buff2, summa2);

			buffer3 += buff0[0] + buff0[3] + buff1[2] + buff2[1];
			buffer4 += buff0[1] + buff1[0] + buff1[3] + buff2[2];
			buffer5 += buff0[2] + buff1[1] + buff2[0] + buff2[3];

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (; j < (srcWidth - 2); j += 3) {
				buffer3 += srcPixelPtr[j + 0];
				buffer4 += srcPixelPtr[j + 1];
				buffer5 += srcPixelPtr[j + 2];
			}

			if (sign == 2) {
				buffer1 += srcPixelPtr[j + 0];
				buffer2 += srcPixelPtr[j + 1];
				buffer0 += buffer5;
				buffer1 += buffer3;
				buffer2 += buffer4;
			} else if (sign == 1) {
				buffer2 += srcPixelPtr[j];
				buffer0 += buffer4;
				buffer1 += buffer5;
				buffer2 += buffer3;
			} else if (sign == 0) {
				buffer0 += buffer3;
				buffer1 += buffer4;
				buffer2 += buffer5;
			}

			buffer3 = buffer4 = buffer5 = (mlib_d64)0.0;

			srcLinePtr += srcYStride;
			srcPixelPtr = (mlib_f32 *)srcLinePtr;
		}

		mean[0] = buffer0 * divider;
		mean[1] = buffer1 * divider;
		mean[2] = buffer2 * divider;

		break;
	} /* case 3 */

	case 4: {
		__m128 load0, load1, load2, load3;
		__m128 summa0, summa1, summa2, summa3;
		mlib_f32 accum0[4];
		mlib_f32 *sl;

		mlib_d64 buffer0 = 0.0;
		mlib_d64 buffer1 = 0.0;
		mlib_d64 buffer2 = 0.0;
		mlib_d64 buffer3 = 0.0;
		mlib_d64 buffer4 = 0.0;
		mlib_d64 buffer5 = 0.0;
		mlib_d64 buffer6 = 0.0;
		mlib_d64 buffer7 = 0.0;

		srcWidth <<= 2;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < srcHeight; i++) {
			mlib_s32 sign = 0;
			mlib_s32 j = 0, l;
			mlib_s32 alignment =
			    ((16 - ((mlib_addr)srcPixelPtr & 15)) & 15) >> 2;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (; (j < srcWidth - 3) &&
			    (alignment >= 4); j += 4) {
				buffer0 += srcPixelPtr[0];
				buffer1 += srcPixelPtr[1];
				buffer2 += srcPixelPtr[2];
				buffer3 += srcPixelPtr[3];
				alignment -= 4;
			}

			if (j < (srcWidth - 3)) {
				if ((alignment == 3)) {
					buffer0 += srcPixelPtr[j + 0];
					buffer1 += srcPixelPtr[j + 1];
					buffer2 += srcPixelPtr[j + 2];
					j += 3;
					sign = 1;
				} else if ((alignment == 2)) {
					buffer0 += srcPixelPtr[j + 0];
					buffer1 += srcPixelPtr[j + 1];
					j += 2;
					sign = 2;
				} else if ((alignment == 1)) {
					buffer0 += srcPixelPtr[j + 0];
					j += 1;
					sign = 3;
				}

				sl = srcPixelPtr + j;

				summa0 = summa1 = _mm_setzero_ps();
				summa2 = summa3 = _mm_setzero_ps();

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (k = 0; k < (srcWidth - j) >> 4; k++) {
					load0 = _mm_load_ps(sl);
					load1 = _mm_load_ps(sl + 4);
					load2 = _mm_load_ps(sl + 8);
					load3 = _mm_load_ps(sl + 12);

					summa0 = _mm_add_ps(summa0, load0);
					summa1 = _mm_add_ps(summa1, load1);
					summa2 = _mm_add_ps(summa2, load2);
					summa3 = _mm_add_ps(summa3, load3);

					sl += 16;
				}

				j += k << 4;

				if (j <= (srcWidth - 12)) {
					load0 = _mm_load_ps(sl);
					load1 = _mm_load_ps(sl + 4);
					load2 = _mm_load_ps(sl + 8);

					summa0 = _mm_add_ps(summa0, load0);
					summa1 = _mm_add_ps(summa1, load1);
					summa2 = _mm_add_ps(summa2, load2);

					j += 12;
					sl += 12;
				}

				if (j <= (srcWidth - 8)) {
					load0 = _mm_load_ps(sl);
					load1 = _mm_load_ps(sl + 4);
					summa0 = _mm_add_ps(summa0, load0);
					summa1 = _mm_add_ps(summa1, load1);
					j += 8;
					sl += 8;
				}

				if (j <= (srcWidth - 4)) {
					load0 = _mm_load_ps(sl);
					summa0 = _mm_add_ps(summa0, load0);
					j += 4;
					sl += 4;
				}

				summa0 = _mm_add_ps(summa0, summa2);
				summa1 = _mm_add_ps(summa1, summa3);
				summa0 = _mm_add_ps(summa0, summa1);

				_mm_storeu_ps((void *)accum0, summa0);

				buffer4 += accum0[0];
				buffer5 += accum0[1];
				buffer6 += accum0[2];
				buffer7 += accum0[3];
			}

			if (sign == 3) {
				buffer1 += srcPixelPtr[j + 0];
				buffer2 += srcPixelPtr[j + 1];
				buffer3 += srcPixelPtr[j + 2];

				buffer0 += buffer7;
				buffer1 += buffer4;
				buffer2 += buffer5;
				buffer3 += buffer6;
			} else if (sign == 2) {
				buffer2 += srcPixelPtr[j + 0];
				buffer3 += srcPixelPtr[j + 1];

				buffer0 += buffer6;
				buffer1 += buffer7;
				buffer2 += buffer4;
				buffer3 += buffer5;
			} else if (sign == 1) {
				buffer3 += srcPixelPtr[j];

				buffer0 += buffer5;
				buffer1 += buffer6;
				buffer2 += buffer7;
				buffer3 += buffer4;
			} else if (sign == 0) {
				buffer0 += buffer4;
				buffer1 += buffer5;
				buffer2 += buffer6;
				buffer3 += buffer7;
			}

			srcLinePtr += srcYStride;
			srcPixelPtr = (mlib_f32 *)srcLinePtr;
			buffer4 = buffer5 = buffer6 = buffer7 = 0.0;
		}

		mean[0] = buffer0 * divider;
		mean[1] = buffer1 * divider;
		mean[2] = buffer2 * divider;
		mean[3] = buffer3 * divider;
		break;
	} /* case 4 */
	}
}

/* *********************************************************** */
