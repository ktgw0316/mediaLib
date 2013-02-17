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
 * Copyright 2006 Sun Microsystems, Inc.  All rights reserved.
 * Use is subject to license terms.
 */

#pragma ident	"@(#)mlib_s_ImageNormCrossCorrel.c	9.2	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_ImageNormCrossCorrel - calculates normalized cross-correlation
 *                                  coefficient of two input images
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageNormCrossCorrel(mlib_d64         *correl,
 *                                            const mlib_image *img1,
 *                                            const mlib_image *img2,
 *                                            const mlib_d64   *mean2,
 *                                            const mlib_d64   *sdev2);
 *
 * ARGUMENTS
 *      correl   Pointer to the cross correlation array.
 *      img1     Pointer to the first input image.
 *      img2     Pointer to the second input image.
 *      mean2    Pointer to the mean array of the second input image.
 *      sdev2    Pointer to the standard deviation array of the second
 *               input image.
 *
 * DESCRIPTION
 *      For each channel i, the normalized cross-correlation coefficient is
 *      defined as follows:
 *
 *              w-1 h-1
 *              SUM SUM {(img1[x][y][i] - m1[i]) * (img2[x][y][i] - m2[i])}
 *              x=0 y=0
 *      c[i] = ------------------------------------------------------------
 *                                   s1[i] * s2[i]
 *
 *                1     w-1 h-1
 *      m1[i] = ----- * SUM SUM img1[x][y][i]
 *               w*h    x=0 y=0
 *
 *                1     w-1 h-1
 *      m2[i] = ----- * SUM SUM img2[x][y][i]
 *               w*h    x=0 y=0
 *
 *                    w-1 h-1
 *      s1[i] = sqrt{ SUM SUM (img1[x][y][i] - m1[i])**2 }
 *                    x=0 y=0
 *
 *                    w-1 h-1
 *      s2[i] = sqrt{ SUM SUM (img2[x][y][i] - m2[i])**2 }
 *                    x=0 y=0
 *
 *      where w and h are the width and height of the images, respectively;
 *      m1 and m2 are the mean arrays of the first and second images,
 *      respectively; s1 and s2 are the un-normalized standard deviation
 *      arrays of the first and second images, respectively.
 *
 *      In usual cases, the normalized cross-correlation coefficient is in
 *      the range of [-1.0, 1.0].  In the case of s1[i] == 0 or s2[i] == 0,
 *      where a constant image is involved, the normalized cross-correlation
 *      coefficient is defined as follows:
 *
 *      #define signof(x) ((x > 0) ? 1 : ((x < 0) ? -1 : 0))
 *
 *      if ((s1[i] == 0.) || (s2[i] == 0.)) {
 *              if ((s1[i] == 0.) && (s2[i] == 0.)) {
 *                      if (signof(m1[i]) == signof(m2[i]) {
 *                              c[i] = 1.0;
 *                      } else {
 *                              c[i] = -1.0;
 *                      }
 *              } else {
 *                      c[i] = -1.0;
 *              }
 *      }
 *
 * RESTRICTION
 *      The images must have the same type, the same size, and the same number
 *      of channels.
 *      They can have 1, 2, 3 or 4 channels.
 *      They can be of MLIB_BYTE, MLIB_SHORT, MLIB_USHORT or MLIB_INT data type.
 *
 *      If (mean2 == NULL) or (sdev2 == NULL), then m2 and s2 are calculated in
 *      this function according to the formulas shown above.  Otherwise, they
 *      are calculated as follows:
 *
 *      m2[i] = mean2[i];
 *      s2[i] = sdev2[i] * sqrt(w*h);
 *
 *      where mean2 and sdev2 can be the output of mlib_ImageMean() and
 *      mlib_ImageStdDev(), respectively.
 */

#include <mlib_image.h>
#include <mlib_ImageCheck.h>
#include <mlib_s_ImageCorrel.h>
#include <mlib_ImageDivTables.h>
#include <mlib_SysMath.h>
#include <stdio.h>

/* *********************************************************** */

#define	EPS 1e-9

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageNormCrossCorrel = __mlib_ImageNormCrossCorrel

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_ImageNormCrossCorrel) mlib_ImageNormCrossCorrel
    __attribute__((weak, alias("__mlib_ImageNormCrossCorrel")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	Square_Root(rtn, x, y)                                \
{                                                             \
	diff = x - y;                                         \
	if (x == 0) {                                         \
		rtn = 0.0;                                    \
	}                                                     \
	x = (x >= 0) ? x : -x;                                \
	diff = (diff >= 0) ? diff : -diff;                    \
	s = diff / x;                                         \
	if (s < EPS) {                                        \
		rtn = 0.0;                                    \
	} else {                                              \
		rtn = mlib_sqrt(diff);                        \
	}                                                     \
}

#define	signof(x)	((x > 0.) ? 1.0 : ((x < 0.) ? -1.0 : 0.))

#define	NormCrossCorrel(rtn, numerator, m1, m2, s1, s2)         \
{                                                               \
	if ((s1 == 0) || (s2 == 0)) {                           \
		/* corner cases */                              \
		if ((s1 == 0) && (s2 == 0)) {                   \
			if (signof(m1) == signof(m2)) {         \
				rtn = 1.0;                      \
			} else {                                \
				rtn = -1.0;                     \
			}                                       \
		} else {                                        \
			rtn = -1.0;                             \
		}                                               \
	} else {                                                \
		/* normal cases */                              \
		if (mlib_fabs(numerator) < EPS) {               \
			rtn = 0.0;                              \
		} else {                                        \
			rtn = (numerator / (s1 * s2));          \
		}                                               \
	}                                                       \
}

/* *********************************************************** */

mlib_status
__mlib_ImageNormCrossCorrel(
	mlib_d64 *correl,
    const mlib_image *img1,
    const mlib_image *img2,
    const mlib_d64   *mean2,
    const mlib_d64   *sdev2)
{
	mlib_s32 num_pixel;
	mlib_d64 res[20];
	mlib_d64 divider;
	mlib_d64 scale;
	mlib_d64 ccInterMul[4];
	mlib_d64 ccSelfMul1[4];
	mlib_d64 ccSelfMul2[4];
	mlib_d64 sum1[4];
	mlib_d64 sum2[4];
	mlib_d64 m1[4];
	mlib_d64 m2[4];
	mlib_d64 s1[4];
	mlib_d64 s2[4];
	mlib_s32 i;
	mlib_d64 x, y, numerator, diff, s;

/* check for obvious errors */
	MLIB_IMAGE_CHECK(img1);
	MLIB_IMAGE_CHECK(img2);
	MLIB_IMAGE_SIZE_EQUAL(img1, img2);
	MLIB_IMAGE_TYPE_EQUAL(img1, img2);
	MLIB_IMAGE_CHAN_EQUAL(img1, img2);

	if (correl == NULL)
		return (MLIB_NULLPOINTER);

/* total number of pixels */
	num_pixel = mlib_ImageGetWidth(img1) * mlib_ImageGetHeight(img1);
	divider = 1.0 * (mlib_d64)num_pixel;
	if ((mean2 != NULL) && (sdev2 != NULL)) {
		scale = mlib_sqrt((mlib_d64)num_pixel);
	}

	for (i = 0; i < 20; i++) {
		res[i] = 0.0;
	}

	mlib_s_ImageNormCrossCorrel(img1, img2,
	    mean2, sdev2, res);

	for (i = 0; i < 4; i++) {
		ccInterMul[i] = 0.0;
		ccSelfMul1[i] = 0.0;
		ccSelfMul2[i] = 0.0;
		sum1[i] = 0.0;
		sum2[i] = 0.0;
		m1[i] = 0.0;
		m2[i] = 0.0;
		s1[i] = 0.0;
		s2[i] = 0.0;
	}
	for (i = 0; i < 4; i++) {
		ccInterMul[i] = res[i];
		ccSelfMul1[i] = res[i+4];
		ccSelfMul2[i] = res[i+8];
		sum1[i] = res[i+12];
		sum2[i] = res[i+16];
	}

	switch (mlib_ImageGetChannels(img1)) {
	case 1:
		correl[0] = (ccInterMul[0] + ccInterMul[1] +
				ccInterMul[2] + ccInterMul[3]);
		m1[0] = (sum1[0] + sum1[1] + sum1[2] + sum1[3]) / divider;
		x = ccSelfMul1[0] + ccSelfMul1[1]
			+ ccSelfMul1[2] + ccSelfMul1[3];
		y = divider * m1[0] * m1[0];
		Square_Root(s1[0], x, y);
		if ((mean2 != NULL) && (sdev2 != NULL)) {
			m2[0] = mean2[0];
			s2[0] = sdev2[0] * scale;
		} else {
			m2[0] = (sum2[0] + sum2[1] + sum2[2] + sum2[3])
				/ divider;
			x = ccSelfMul2[0] + ccSelfMul2[1]
				+ ccSelfMul2[2] + ccSelfMul2[3];
			y = divider * m2[0] * m2[0];
			Square_Root(s2[0], x, y);
		}
		numerator = correl[0] - divider * m1[0] * m2[0];
		NormCrossCorrel(correl[0], numerator, m1[0], m2[0], s1[0],
				s2[0]);
		break;

	case 2:
		correl[0] = (ccInterMul[0] + ccInterMul[2]);

		m1[0] = (sum1[0] + sum1[2]) / divider;
		x = ccSelfMul1[0] + ccSelfMul1[2];
		y = divider * m1[0] * m1[0];
		Square_Root(s1[0], x, y);
		if ((mean2 != NULL) && (sdev2 != NULL)) {
			m2[0] = mean2[0];
			s2[0] = sdev2[0] * scale;
		} else {
			m2[0] = (sum2[0] + sum2[2]) / divider;
			x = ccSelfMul2[0] + ccSelfMul2[2];
			y = divider * m2[0] * m2[0];
			Square_Root(s2[0], x, y);
		}
		numerator = correl[0] - divider * m1[0] * m2[0];
		NormCrossCorrel(correl[0], numerator, m1[0], m2[0], s1[0],
				s2[0]);

		correl[1] = (ccInterMul[1] + ccInterMul[3]);

		m1[1]  = (sum1[1] + sum1[3]) / divider;
		x = ccSelfMul1[1] + ccSelfMul1[3];
		y = divider * m1[1] * m1[1];
		Square_Root(s1[1], x, y);
		if ((mean2 != NULL) && (sdev2 != NULL)) {
			m2[1]  = mean2[1];
			s2[1] = sdev2[1] * scale;
		} else {
			m2[1]  = (sum2[1] + sum2[3]) / divider;
			x = ccSelfMul2[1] + ccSelfMul2[3];
			y = divider * m2[1] * m2[1];
			Square_Root(s2[1], x, y);
		}
		numerator = correl[1] - divider * m1[1] * m2[1];
		NormCrossCorrel(correl[1], numerator, m1[1], m2[1], s1[1],
				s2[1]);

		break;

	case 4:
		correl[3] = ccInterMul[3];
		m1[3] = sum1[3] / divider;
		x = ccSelfMul1[3];
		y = divider * m1[3] * m1[3];
		Square_Root(s1[3], x, y);
		if ((mean2 != NULL) && (sdev2 != NULL)) {
			m2[3] = mean2[3];
			s2[3] = sdev2[3] * scale;
		} else {
			m2[3] = sum2[3] / divider;
			x = ccSelfMul2[3];
			y = divider * m2[3] * m2[3];
			Square_Root(s2[3], x, y);
		}
		numerator = correl[3] - divider * m1[3] * m2[3];
		NormCrossCorrel(correl[3], numerator, m1[3], m2[3], s1[3],
				s2[3]);

	case 3:
		correl[0] = ccInterMul[0];
		m1[0] = sum1[0] / divider;
		x = ccSelfMul1[0];
		y = divider * m1[0] * m1[0];
		Square_Root(s1[0], x, y);
		if ((mean2 != NULL) && (sdev2 != NULL)) {
			m2[0] = mean2[0];
			s2[0] = sdev2[0] * scale;
		} else {
			m2[0] = sum2[0] / divider;
			x = ccSelfMul2[0];
			y = divider * m2[0] * m2[0];
			Square_Root(s2[0], x, y);
		}
		numerator = correl[0] - divider * m1[0] * m2[0];
		NormCrossCorrel(correl[0], numerator, m1[0], m2[0], s1[0],
				s2[0]);

		correl[1] = ccInterMul[1];
		m1[1] = sum1[1] / divider;
		x = ccSelfMul1[1];
		y = divider * m1[1] * m1[1];
		Square_Root(s1[1], x, y);
		if ((mean2 != NULL) && (sdev2 != NULL)) {
			m2[1] = mean2[1];
			s2[1] = sdev2[1] * scale;
		} else {
			m2[1] = sum2[1] / divider;
			x = ccSelfMul2[1];
			y = divider * m2[1] * m2[1];
			Square_Root(s2[1], x, y);
		}
		numerator = correl[1] - divider * m1[1] * m2[1];
		NormCrossCorrel(correl[1], numerator, m1[1], m2[1], s1[1],
				s2[1]);

		correl[2] = ccInterMul[2];
		m1[2] = sum1[2] / divider;
		x = ccSelfMul1[2];
		y = divider * m1[2] * m1[2];
		Square_Root(s1[2], x, y);
		if ((mean2 != NULL) && (sdev2 != NULL)) {
			m2[2] = mean2[2];
			s2[2] = sdev2[2] * scale;
		} else {
			m2[2] = sum2[2] / divider;
			x = ccSelfMul2[2];
			y = divider * m2[2] * m2[2];
			Square_Root(s2[2], x, y);
		}
		numerator = correl[2] - divider * m1[2] * m2[2];
		NormCrossCorrel(correl[2], numerator, m1[2], m2[2],
				s1[2], s2[2]);

	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

void
mlib_s_ImageNormCrossCorrel(
    const mlib_image *img1,
    const mlib_image *img2,
    const mlib_d64 *m2,
    const mlib_d64 *s2,
    mlib_d64 *res)
{
/* total number of pixels */
	mlib_s32 num_pixel = mlib_ImageGetWidth(img1) *
	    mlib_ImageGetHeight(img1);
	mlib_d64 divider = 1.0 * (mlib_d64)num_pixel;

/* accumulators by channels */
	mlib_d64 ccInterMul[4] = {0.0, 0.0, 0.0, 0.0};
	mlib_d64 ccSelfMul1[4] = {0.0, 0.0, 0.0, 0.0};
	mlib_d64 ccSelfMul2[4] = {0.0, 0.0, 0.0, 0.0};
	mlib_d64 sum1[4] = {0.0, 0.0, 0.0, 0.0};
	mlib_d64 sum2[4] = {0.0, 0.0, 0.0, 0.0};
	mlib_s32 k;

	if ((m2 == NULL) && (s2 == NULL)) {

		__mlib_ImageCrossCorrel(ccInterMul, img1, img2);
		__mlib_ImageMoment2(ccSelfMul2, img2);
		__mlib_ImageMean(sum2, img2);
		__mlib_ImageMoment2(ccSelfMul1, img1);
		__mlib_ImageMean(sum1, img1);

		for (k = 0; k < 4; k++) {
			res[k] += ccInterMul[k];
			res[k + 4] += ccSelfMul1[k];
			res[k + 8] += ccSelfMul2[k];
			res[k + 12] += sum1[k];
			res[k + 16] += sum2[k];
		}
	} else {

		__mlib_ImageCrossCorrel(ccInterMul, img1, img2);
		__mlib_ImageMoment2(ccSelfMul1, img1);
		__mlib_ImageMean(sum1, img1);

		for (k = 0; k < 4; k++) {
			res[k] += ccInterMul[k];
			res[k + 4] += ccSelfMul1[k];
			res[k + 12] += sum1[k];
		}
	}

	for (k = 0; k < 4; k++) {
		res[k] *= divider;
		res[k + 4] *= divider;
		res[k + 8] *= divider;
		res[k + 12] *= divider;
		res[k + 16] *= divider;
	}
}

/* *********************************************************** */
