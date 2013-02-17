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

#pragma ident	"@(#)mlib_v_ImageNormCrossCorrel.c	9.3	07/11/05 SMI"

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


#include <vis_proto.h>
#include <mlib_image.h>
#include <mlib_ImageCheck.h>
#include <mlib_SysMath.h>
#include <mlib_v_ImageStat.h>
#include <mlib_v_ImageCorrel.h>

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
#define	EPS 1e-9

/* *********************************************************** */
#define	Square_Root(rtn, x, y)                                  \
{                                                               \
	diff = x - y;                                           \
	if (x == 0.0) {                                         \
		rtn = 0.0;                                      \
	}                                                       \
	x = x >= 0 ? x : -x;                                    \
	s = diff / x;                                           \
	if (s < EPS) {                                          \
		rtn = 0.0;                                      \
	}                                                       \
	else                                                    \
		rtn = mlib_sqrt(diff);                          \
}

#define	signof(x)	((x > 0) ? 1 : ((x < 0) ? -1 : 0))

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
		rtn = (numerator / (s1 * s2));                  \
	}                                                       \
}

mlib_status
__mlib_ImageNormCrossCorrel(
    mlib_d64 *correl,
    const mlib_image *img1,
    const mlib_image *img2,
    const mlib_d64   *mean2,
    const mlib_d64   *sdev2)
{
	mlib_s32 num_pixel;
	mlib_d64 rez[20];
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

	mlib_d64 divider, dconst;
	mlib_d64 scale;
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
	dconst = divider * 1073741824.0;
	if ((mean2 != NULL) && (sdev2 != NULL)) {
		scale = mlib_sqrt((mlib_d64)num_pixel);
	}

	for (i = 0; i < 20; i++) {
		rez[i] = 0.0;
	}

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


	if (mlib_ImageGetType(img1) != MLIB_USHORT) {
		switch (mlib_ImageGetType(img1)) {
	/* handle MLIB_BYTE data type of image */
		case MLIB_BYTE:

			if (mlib_ImageGetChannels(img1) == 3) {
				mlib_v_ImageNormCrossCorrel_U8_3(
					img1, img2, mean2, sdev2, rez);
			} else {
				mlib_v_ImageNormCrossCorrel_U8_124(
					img1, img2, mean2, sdev2, rez);
			}
			break;

	/* handle MLIB_SHORT data type of image */
		case MLIB_SHORT:

			if (mlib_ImageGetChannels(img1) == 3) {
				mlib_v_ImageNormCrossCorrel_S16_3(
					img1, img2, mean2, sdev2, rez);
			} else {
				mlib_v_ImageNormCrossCorrel_S16_124(
					img1, img2, mean2, sdev2, rez);
			}
			break;

	/* handle MLIB_INT data type of image */
		case MLIB_INT:

			if (mlib_ImageGetChannels(img1) == 3) {
				mlib_v_ImageNormCrossCorrel_S32_3(
					img1, img2, mean2, sdev2, rez);
			} else {
				mlib_v_ImageNormCrossCorrel_S32_124(
					img1, img2, mean2, sdev2, rez);
			}
			break;
	/* discard any other data types */
		default:
			return (MLIB_FAILURE);
		}

		for (i = 0; i < 4; i++) {
			ccInterMul[i] = rez[i];
			ccSelfMul1[i] = rez[i+4];
			ccSelfMul2[i] = rez[i+8];
			sum1[i] = rez[i+12];
			sum2[i] = rez[i+16];
		}

		switch (mlib_ImageGetChannels(img1)) {
		case 1:
			correl[0] = (ccInterMul[0] + ccInterMul[1] +
					ccInterMul[2] + ccInterMul[3]);
			m1[0] = (sum1[0] + sum1[1] + sum1[2] + sum1[3])
				/ divider;
			x = ccSelfMul1[0] + ccSelfMul1[1] + ccSelfMul1[2]
				+ ccSelfMul1[3];
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
			NormCrossCorrel(correl[0], numerator,
				m1[0], m2[0], s1[0], s2[0]);
			break;

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
			NormCrossCorrel(correl[0], numerator,
				m1[0], m2[0], s1[0], s2[0]);

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
			NormCrossCorrel(correl[1], numerator,
				m1[1], m2[1], s1[1], s2[1]);

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
			NormCrossCorrel(correl[2], numerator,
				m1[2], m2[2], s1[2], s2[2]);
			break;

		case 4:
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
			NormCrossCorrel(correl[0], numerator,
				m1[0], m2[0], s1[0], s2[0]);

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
			NormCrossCorrel(correl[1], numerator,
				m1[1], m2[1], s1[1], s2[1]);

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
			NormCrossCorrel(correl[2], numerator,
				m1[2], m2[2], s1[2], s2[2]);

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
			NormCrossCorrel(correl[3], numerator,
				m1[3], m2[3], s1[3], s2[3]);
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
			NormCrossCorrel(correl[0], numerator,
				m1[0], m2[0], s1[0], s2[0]);

			correl[1] = (ccInterMul[1] + ccInterMul[3]);
			m1[1]  = (sum1[1] + sum1[3]) / divider;
			x = ccSelfMul1[1] + ccSelfMul1[3];
			y = divider * m1[1] * m1[1];
			Square_Root(s1[1], x, y);
			if ((mean2 != NULL) && (sdev2 != NULL)) {
				m2[1] = mean2[1];
				s2[1] = sdev2[1] * scale;
			} else {
				m2[1]  = (sum2[1] + sum2[3]) / divider;
				x = ccSelfMul2[1] + ccSelfMul2[3];
				y = divider * m2[1] * m2[1];
				Square_Root(s2[1], x, y);
			}
			numerator = correl[1] - divider * m1[1] * m2[1];
			NormCrossCorrel(correl[1], numerator,
				m1[1], m2[1], s1[1], s2[1]);
			break;
		}

	} else {
		/* handle MLIB_USHORT data type of image */
		if (mlib_ImageGetChannels(img1) == 3) {
			mlib_v_ImageNormCrossCorrel_U16_3(
				img1, img2, mean2, sdev2, rez);
		} else {
			mlib_v_ImageNormCrossCorrel_U16_124(
				img1, img2, mean2, sdev2, rez);
		}

		for (i = 0; i < 4; i++) {
			ccInterMul[i] = rez[i];
			ccSelfMul1[i] = rez[i+4];
			ccSelfMul2[i] = rez[i+8];
			sum1[i] = rez[i+12];
			sum2[i] = rez[i+16];
		}

		switch (mlib_ImageGetChannels(img1)) {
		case 1:
			correl[0] = (ccInterMul[0] + ccInterMul[1] +
				ccInterMul[2] + ccInterMul[3]);
			m1[0] = (sum1[0] + sum1[1] + sum1[2] + sum1[3]
				+ divider * 32768.0) / divider;
			x = ccSelfMul1[0] + ccSelfMul1[1] + ccSelfMul1[2]
				+ ccSelfMul1[3];
			y = divider * m1[0] * m1[0]
				- divider * m1[0] * 65536 + dconst;
			Square_Root(s1[0], x, y);
			if ((mean2 != NULL) && (sdev2 != NULL)) {
				m2[0] = mean2[0];
				s2[0] = sdev2[0] * scale;
			} else {
				m2[0] = (sum2[0] + sum2[1] + sum2[2] + sum2[3]
					+ divider * 32768.0) / divider;
				x = ccSelfMul2[0] + ccSelfMul2[1]
					+ ccSelfMul2[2] + ccSelfMul2[3];
				y = divider * m2[0] * m2[0]
					- divider * m2[0] * 65536 + dconst;
				Square_Root(s2[0], x, y);
			}
			numerator = correl[0] - divider * m1[0] * m2[0]
				+ divider * (m1[0] + m2[0]) * 32768 - dconst;
			NormCrossCorrel(correl[0], numerator,
				m1[0], m2[0], s1[0], s2[0]);
			break;

		case 3:
			correl[0] = ccInterMul[0];
			m1[0] = (sum1[0] + divider * 32768) / divider;
			x = ccSelfMul1[0];
			y = divider * m1[0] * m1[0]
				- divider * m1[0] * 65536 + dconst;
			Square_Root(s1[0], x, y);
			if ((mean2 != NULL) && (sdev2 != NULL)) {
				m2[0] = mean2[0];
				s2[0] = sdev2[0] * scale;
			} else {
				m2[0] = (sum2[0] + divider * 32768) / divider;
				x = ccSelfMul2[0];
				y = divider * m2[0] * m2[0]
					- divider * m2[0] * 65536 + dconst;
				Square_Root(s2[0], x, y);
			}
			numerator = correl[0] - divider * m1[0] * m2[0]
				+ divider * (m1[0] + m2[0]) * 32768 - dconst;
			NormCrossCorrel(correl[0], numerator,
				m1[0], m2[0], s1[0], s2[0]);

			correl[1] = ccInterMul[1];
			m1[1] = (sum1[1] + divider * 32768) / divider;
			x = ccSelfMul1[1];
			y = divider * m1[1] * m1[1]
				- divider * m1[1] * 65536 + dconst;
			Square_Root(s1[1], x, y);
			if ((mean2 != NULL) && (sdev2 != NULL)) {
				m2[1] = mean2[1];
				s2[1] = sdev2[1] * scale;
			} else {
				m2[1] = (sum2[1] + divider * 32768) / divider;
				x = ccSelfMul2[1];
				y = divider * m2[1] * m2[1]
					- divider * m2[1] * 65536 + dconst;
				Square_Root(s2[1], x, y);
			}
			numerator = correl[1] - divider * m1[1] * m2[1]
				+ divider * (m1[1] + m2[1]) * 32768 - dconst;
			NormCrossCorrel(correl[1], numerator,
				m1[1], m2[1], s1[1], s2[1]);

			correl[2] = ccInterMul[2];
			m1[2] = (sum1[2] + divider * 32768) / divider;
			x = ccSelfMul1[2];
			y = divider * m1[2] * m1[2]
				- divider * m1[2] * 65536 + dconst;
			Square_Root(s1[2], x, y);
			if ((mean2 != NULL) && (sdev2 != NULL)) {
				m2[2] = mean2[2];
				s2[2] = sdev2[2] * scale;
			} else {
				m2[2] = (sum2[2] + divider * 32768) / divider;
				x = ccSelfMul2[2];
				y = divider * m2[2] * m2[2]
					- divider * m2[2] * 65536 + dconst;
				Square_Root(s2[2], x, y);
			}
			numerator = correl[2] - divider * m1[2] * m2[2]
				+ divider * (m1[2] + m2[2]) * 32768 - dconst;
			NormCrossCorrel(correl[2], numerator,
				m1[2], m2[2], s1[2], s2[2]);
			break;

		case 4:
			correl[0] = ccInterMul[0];
			m1[0] = (sum1[0] + divider * 32768) / divider;
			x = ccSelfMul1[0];
			y = divider * m1[0] * m1[0]
				- divider * m1[0] * 65536 + dconst;
			Square_Root(s1[0], x, y);
			if ((mean2 != NULL) && (sdev2 != NULL)) {
				m2[0] = mean2[0];
				s2[0] = sdev2[0] * scale;
			} else {
				m2[0] = (sum2[0] + divider * 32768) / divider;
				x = ccSelfMul2[0];
				y = divider * m2[0] * m2[0]
					- divider * m2[0] * 65536 + dconst;
				Square_Root(s2[0], x, y);
			}
			numerator = correl[0] - divider * m1[0] * m2[0]
				+ divider * (m1[0] + m2[0]) * 32768 - dconst;
			NormCrossCorrel(correl[0], numerator,
				m1[0], m2[0], s1[0], s2[0]);

			correl[1] = ccInterMul[1];
			m1[1] = (sum1[1] + divider * 32768) / divider;
			x = ccSelfMul1[1];
			y = divider * m1[1] * m1[1]
				- divider * m1[1] * 65536 + dconst;
			Square_Root(s1[1], x, y);
			if ((mean2 != NULL) && (sdev2 != NULL)) {
				m2[1] = mean2[1];
				s2[1] = sdev2[1] * scale;
			} else {
				m2[1] = (sum2[1] + divider * 32768) / divider;
				x = ccSelfMul2[1];
				y = divider * m2[1] * m2[1]
					- divider * m2[1] * 65536 + dconst;
				Square_Root(s2[1], x, y);
			}
			numerator = correl[1] - divider * m1[1] * m2[1]
				+ divider * (m1[1] + m2[1]) * 32768 - dconst;
			NormCrossCorrel(correl[1], numerator,
				m1[1], m2[1], s1[1], s2[1]);

			correl[2] = ccInterMul[2];
			m1[2] = (sum1[2] + divider * 32768) / divider;
			x = ccSelfMul1[2];
			y = divider * m1[2] * m1[2]
				- divider * m1[2] * 65536 + dconst;
			Square_Root(s1[2], x, y);
			if ((mean2 != NULL) && (sdev2 != NULL)) {
				m2[2] = mean2[2];
				s2[2] = sdev2[2] * scale;
			} else {
				m2[2] = (sum2[2] + divider * 32768) / divider;
				x = ccSelfMul2[2];
				y = divider * m2[2] * m2[2]
					- divider * m2[2] * 65536 + dconst;
				Square_Root(s2[2], x, y);
			}
			numerator = correl[2] - divider * m1[2] * m2[2]
				+ divider * (m1[2] + m2[2]) * 32768 - dconst;
			NormCrossCorrel(correl[2], numerator,
				m1[2], m2[2], s1[2], s2[2]);

			correl[3] = ccInterMul[3];
			m1[3] = (sum1[3] + divider * 32768) / divider;
			x = ccSelfMul1[3];
			y = divider * m1[3] * m1[3]
				- divider * m1[3] * 65536 + dconst;
			Square_Root(s1[3], x, y);
			if ((mean2 != NULL) && (sdev2 != NULL)) {
				m2[3] = mean2[3];
				s2[3] = sdev2[3] * scale;
			} else {
				m2[3] = (sum2[3] + divider * 32768) / divider;
				x = ccSelfMul2[3];
				y = divider * m2[3] * m2[3]
					- divider * m2[3] * 65536 + dconst;
				Square_Root(s2[3], x, y);
			}
			numerator = correl[3] - divider * m1[3] * m2[3]
				+ divider * (m1[3] + m2[3]) * 32768 - dconst;
			NormCrossCorrel(correl[3], numerator,
				m1[3], m2[3], s1[3], s2[3]);
			break;

		case 2:
			correl[0] = (ccInterMul[0] + ccInterMul[2]);
			m1[0] = (sum1[0] + sum1[2] + divider * 32768.0)
				/ divider;
			x = ccSelfMul1[0] + ccSelfMul1[2];
			y = divider * m1[0] * m1[0]
				- divider * m1[0] * 65536 + dconst;
			Square_Root(s1[0], x, y);
			if ((mean2 != NULL) && (sdev2 != NULL)) {
				m2[0] = mean2[0];
				s2[0] = sdev2[0] * scale;
			} else {
				m2[0] = (sum2[0] + sum2[2] + divider * 32768.0)
					/ divider;
				x = ccSelfMul2[0] + ccSelfMul2[2];
				y = divider * m2[0] * m2[0]
					- divider * m2[0] * 65536 + dconst;
				Square_Root(s2[0], x, y);
			}
			numerator = correl[0] - divider * m1[0] * m2[0]
				+ divider * (m1[0] + m2[0]) * 32768 - dconst;
			NormCrossCorrel(correl[0], numerator,
				m1[0], m2[0], s1[0], s2[0]);

			correl[1] = (ccInterMul[1] + ccInterMul[3]);
			m1[1]  = (sum1[1] + sum1[3] + divider * 32768.0)
				/ divider;
			x = ccSelfMul1[1] + ccSelfMul1[3];
			y = divider * m1[1] * m1[1]
				- divider * m1[1] * 65536 + dconst;
			Square_Root(s1[1], x, y);
			if ((mean2 != NULL) && (sdev2 != NULL)) {
				m2[1] = mean2[1];
				s2[1] = sdev2[1] * scale;
			} else {
				m2[1]  = (sum2[1] + sum2[3] + divider * 32768.0)
					/ divider;
				x = ccSelfMul2[1] + ccSelfMul2[3];
				y = divider * m2[1] * m2[1]
					- divider * m2[1] * 65536 + dconst;
				Square_Root(s2[1], x, y);
			}
			numerator = correl[1] - divider * m1[1] * m2[1]
				+ divider * (m1[1] + m2[1]) * 32768 - dconst;
			NormCrossCorrel(correl[1], numerator,
				m1[1], m2[1], s1[1], s2[1]);
			break;
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#define	INTER_MUL_U8_U32                                                  \
	dxh = vis_fmul8x16al(vis_read_hi(dx), fone);                          \
	dxl = vis_fmul8x16al(vis_read_lo(dx), fone);                          \
	dyh = vis_fpmerge(vis_read_hi(dzero), vis_read_hi(dy));               \
	dyl = vis_fpmerge(vis_read_hi(dzero), vis_read_lo(dy));               \
	ccInterMul1 = vis_fmuld8ulx16(vis_read_hi(dxh), vis_read_hi(dyh));    \
	ccInterMul2 = vis_fmuld8ulx16(vis_read_lo(dxh), vis_read_lo(dyh));    \
	ccInterMul3 = vis_fmuld8ulx16(vis_read_hi(dxl), vis_read_hi(dyl));    \
	ccInterMul4 = vis_fmuld8ulx16(vis_read_lo(dxl), vis_read_lo(dyl));

/* *********************************************************** */


#define	INTER_SUM_U32_124                                           \
	ccInterAcc1 = vis_fpadd32(ccInterMul1, ccInterAcc1);            \
	ccInterAcc2 = vis_fpadd32(ccInterMul2, ccInterAcc2);            \
	ccInterAcc1 = vis_fpadd32(ccInterMul3, ccInterAcc1);            \
	ccInterAcc2 = vis_fpadd32(ccInterMul4, ccInterAcc2);



/* *********************************************************** */

#define	IMG1_SELF_MUL_U8_U32                                            \
	ccSelfMul11 = vis_fmuld8ulx16(vis_read_hi(dxh), vis_read_hi(dxh));  \
	ccSelfMul12 = vis_fmuld8ulx16(vis_read_lo(dxh), vis_read_lo(dxh));  \
	ccSelfMul13 = vis_fmuld8ulx16(vis_read_hi(dxl), vis_read_hi(dxl));  \
	ccSelfMul14 = vis_fmuld8ulx16(vis_read_lo(dxl), vis_read_lo(dxl));

/* *********************************************************** */


#define	IMG1_SELF_SUM_U32_124                                       \
	ccSelfAcc11 = vis_fpadd32(ccSelfMul11, ccSelfAcc11);            \
	ccSelfAcc12 = vis_fpadd32(ccSelfMul12, ccSelfAcc12);            \
	ccSelfAcc11 = vis_fpadd32(ccSelfMul13, ccSelfAcc11);            \
	ccSelfAcc12 = vis_fpadd32(ccSelfMul14, ccSelfAcc12);

/* *********************************************************** */

#define	IMG2_SELF_MUL_U8_U32                                            \
	ccSelfMul21 = vis_fmuld8ulx16(vis_read_hi(dyh), vis_read_hi(dyh));  \
	ccSelfMul22 = vis_fmuld8ulx16(vis_read_lo(dyh), vis_read_lo(dyh));  \
	ccSelfMul23 = vis_fmuld8ulx16(vis_read_hi(dyl), vis_read_hi(dyl));  \
	ccSelfMul24 = vis_fmuld8ulx16(vis_read_lo(dyl), vis_read_lo(dyl));

/* *********************************************************** */


#define	IMG2_SELF_SUM_U32_124                                       \
	ccSelfAcc21 = vis_fpadd32(ccSelfMul21, ccSelfAcc21);            \
	ccSelfAcc22 = vis_fpadd32(ccSelfMul22, ccSelfAcc22);            \
	ccSelfAcc21 = vis_fpadd32(ccSelfMul23, ccSelfAcc21);            \
	ccSelfAcc22 = vis_fpadd32(ccSelfMul24, ccSelfAcc22);

/* *********************************************************** */
#define	IMG1_SUM_U32_124                                               \
	tmpAcc = vis_fpadd16(dxh, dxl);                                    \
	sum11 = vis_fpadd32(sum11, vis_fmuld8ulx16(mult,                   \
		vis_read_hi(tmpAcc)));                                         \
	sum12 = vis_fpadd32(sum12, vis_fmuld8ulx16(mult,                   \
		vis_read_lo(tmpAcc)));                                         \

/* *********************************************************** */

#define	IMG2_SUM_U32_124                                               \
	tmpAcc = vis_fpadd16(dyh, dyl);                                    \
	sum21 = vis_fpadd32(sum21, vis_fmuld8ulx16(mult,                   \
	    vis_read_hi(tmpAcc)));                                         \
	sum22 = vis_fpadd32(sum22, vis_fmuld8ulx16(mult,                   \
	    vis_read_lo(tmpAcc)));                                         \

/* *********************************************************** */

#define	MAX_ELEM	0x20000

/* *********************************************************** */

void
mlib_v_ImageNormCrossCorrel_U8_124(
    const mlib_image *img1,
    const mlib_image *img2,
	const mlib_d64 *m2,
	const mlib_d64 *s2,
    mlib_d64 *rez)
{
/* pointer to the data of first image */
	mlib_u8 *point_src1 = (mlib_u8 *)mlib_ImageGetData(img1);

/* pointer to the data of second image */
	mlib_u8 *point_src2 = (mlib_u8 *)mlib_ImageGetData(img2);

/* height of image */
	mlib_s32 height = mlib_ImageGetHeight(img1);

/* linestride = elements to next row */
	mlib_s32 src1_stride = mlib_ImageGetStride(img1);

/* linestride = elements to next row */
	mlib_s32 src2_stride = mlib_ImageGetStride(img2);
	mlib_s32 nchan = mlib_ImageGetChannels(img1);

/* number of elements in the row */
	mlib_s32 size_row = mlib_ImageGetWidth(img1) << (nchan >> 1);

/* indices */
	mlib_s32 i, j, k;

/* edge masks */
	mlib_s32 emask;

/* offset of address alignment in destination */
	mlib_s32 off;
	mlib_s32 size, num_row;
	mlib_u8 *psrc1, *psrc2;
	mlib_d64 midd[2], *dpsrc1, *dpsrc2;
	mlib_d64 dx, dy,  dy0, dy1, dyh, dyl, dxh, dxl;
	const mlib_f32 fone = vis_to_float(0x100);
	const mlib_d64 dzero = vis_fzero();
	type_union_mlib_d64 dd[10];

	mlib_d64 ccInterMul1, ccInterMul2, ccInterMul3, ccInterMul4;
	mlib_d64 ccInterAcc1, ccInterAcc2;
	mlib_d64 ccSelfMul11, ccSelfMul12, ccSelfMul13, ccSelfMul14;
	mlib_d64 ccSelfAcc11, ccSelfAcc12;
	mlib_d64 ccSelfMul21, ccSelfMul22, ccSelfMul23, ccSelfMul24;
	mlib_d64 ccSelfAcc21, ccSelfAcc22;
	mlib_d64 sum11, sum12;
	mlib_d64 sum21, sum22;

	mlib_d64 tmpAcc;
	mlib_f32 mult = vis_to_float(0x10001);

	if (src1_stride == size_row && src1_stride == src2_stride) {
/* This image is not a sub-image and can be treated as a 1-D vector */
		size_row *= height;
		height = 1;
	}

	if (m2 != NULL && s2 != NULL) {

		for (i = 0; i < height; i++) {
			psrc1 = point_src1;
			psrc2 = point_src2;

			ccInterMul1 = ccInterMul2 = dzero;
			ccInterMul3 = ccInterMul4 = dzero;
			ccInterAcc1 = ccInterAcc2 = dzero;
			ccSelfMul11 = ccSelfMul12 = dzero;
			ccSelfMul13 = ccSelfMul14 = dzero;
			ccSelfAcc11 = ccSelfAcc12 = dzero;
			sum11 = sum12 = dzero;
			tmpAcc = dzero;

/* prepare the source address */
			off = (mlib_addr)psrc1 & 7;
			emask = vis_edge8(psrc1, psrc1 + size_row - 1);
			num_row = size_row + off;
			psrc1 -= off;
			psrc2 -= off;
			dpsrc1 = (mlib_d64 *)psrc1;
			dpsrc2 = (mlib_d64 *)vis_alignaddr(psrc2, 0);

			midd[0] = midd[1] = dzero;
			vis_pst_8(dpsrc1[0], (void *)midd, emask);
			dx = midd[0];

			dy0 = vis_ld_d64_nf(dpsrc2);
			dy1 = vis_ld_d64_nf(dpsrc2 + 1);
			dy = vis_faligndata(dy0, dy1);
			vis_pst_8(dy, (void *)midd, emask);
			dy = midd[0];

			dpsrc1++;
			dpsrc2++;

			midd[0] = midd[1] = dzero;

			while (num_row > 7) {
				size = num_row;

				if (size > MAX_ELEM)
					size = MAX_ELEM;

				if ((((mlib_addr)psrc1 ^
					(mlib_addr)psrc2) & 7) == 0) {
					for (j = 0; j <= (size - 8); j += 8) {
						INTER_MUL_U8_U32
						INTER_SUM_U32_124
						IMG1_SELF_MUL_U8_U32
						IMG1_SELF_SUM_U32_124
						IMG1_SUM_U32_124

						dx = vis_ld_d64_nf(
							(mlib_u8 *)dpsrc1 +
							j);
						dy = vis_ld_d64_nf(
							(mlib_u8 *)dpsrc2 +
							j);
					}
					num_row -= j;
					dpsrc1 = (mlib_d64 *)(
						(mlib_u8 *)dpsrc1 + j);
					dpsrc2 = (mlib_d64 *)(
						(mlib_u8 *)dpsrc2 + j);
					dy = vis_ld_d64_nf(dpsrc2 - 1);

				} else {
					for (j = 0; j <= (size - 8); j += 8) {

						INTER_MUL_U8_U32
						INTER_SUM_U32_124
						IMG1_SELF_MUL_U8_U32
						IMG1_SELF_SUM_U32_124
						IMG1_SUM_U32_124

						dx = vis_ld_d64_nf(
							(mlib_u8 *)dpsrc1 +
							j);
						dy0 = vis_ld_d64_nf(
							(mlib_u8 *)dpsrc2 +
							j);
						dy1 = vis_ld_d64_nf(
							(mlib_u8 *)dpsrc2 +
							j + 8);

						dy = vis_faligndata(dy0, dy1);
					}

					num_row -= j;
					dpsrc1 = (mlib_d64 *)(
						(mlib_u8 *)dpsrc1 + j);
					dpsrc2 = (mlib_d64 *)(
						(mlib_u8 *)dpsrc2 + j);
					dy0 = vis_ld_d64_nf(dpsrc2 - 1);
					dy1 = vis_ld_d64_nf(dpsrc2);
					dy = vis_faligndata(dy0, dy1);
				}


				dd[0].db = ccInterAcc1;
				dd[1].db = ccInterAcc2;
				dd[2].db = ccSelfAcc11;
				dd[3].db = ccSelfAcc12;
				dd[4].db = ccSelfAcc21;
				dd[5].db = ccSelfAcc22;
				dd[6].db = sum11;
				dd[7].db = sum12;
				dd[8].db = sum21;
				dd[9].db = sum22;

				for (k = 0; k < 4; k++) {
					if ((k % 2) == 0) {
						rez[3 & (k - off)] +=
							(mlib_d64)dd[k /
							2].two_int.int0;
						rez[(3 & (k - off)) + 4] +=
							(mlib_d64)dd[k / 2 +
							2].two_int.int0;
						rez[(3 & (k - off)) + 12] +=
							(mlib_d64)dd[k / 2 +
							6].two_int.int0;
					} else {
						rez[3 & (k - off)] +=
							(mlib_d64)dd[k /
							2].two_int.int1;
						rez[(3 & (k - off)) + 4] +=
							(mlib_d64)dd[k / 2 +
							2].two_int.int1;
						rez[(3 & (k - off)) + 12] +=
							(mlib_d64)dd[k / 2 +
							6].two_int.int1;
					}

				}

				ccInterAcc1 = ccInterAcc2 = dzero;
				ccSelfAcc11 = ccSelfAcc12 = dzero;
				sum11 = sum12 = dzero;

			}

			if (num_row > 0) {

				emask =
					vis_edge8(dpsrc1,
					(mlib_u8 *)dpsrc1 + num_row - 1);
				vis_pst_8(dx, (void *)(midd + 1), emask);
				dx = midd[1];

				midd[1] = dzero;
				vis_pst_8(dy, (void *)(midd + 1), emask);
				dy = midd[1];

				INTER_MUL_U8_U32
				INTER_SUM_U32_124
				IMG1_SELF_MUL_U8_U32
				IMG1_SELF_SUM_U32_124
				IMG1_SUM_U32_124

				dd[0].db = ccInterAcc1;
				dd[1].db = ccInterAcc2;
				dd[2].db = ccSelfAcc11;
				dd[3].db = ccSelfAcc12;
				dd[4].db = ccSelfAcc21;
				dd[5].db = ccSelfAcc22;
				dd[6].db = sum11;
				dd[7].db = sum12;
				dd[8].db = sum21;
				dd[9].db = sum22;

				for (k = 0; k < 4; k++) {
					if ((k % 2) == 0) {
						rez[3 & (k - off)] +=
							(mlib_d64)dd[k /
							2].two_int.int0;
						rez[(3 & (k - off)) + 4] +=
							(mlib_d64)dd[k / 2 +
							2].two_int.int0;
						rez[(3 & (k - off)) + 12] +=
							(mlib_d64)dd[k / 2 +
							6].two_int.int0;
					} else {
						rez[3 & (k - off)] +=
							(mlib_d64)dd[k /
							2].two_int.int1;
						rez[(3 & (k - off)) + 4] +=
							(mlib_d64)dd[k / 2 +
							2].two_int.int1;
						rez[(3 & (k - off)) + 12] +=
							(mlib_d64)dd[k / 2 +
							6].two_int.int1;
					}

				}

			}

			point_src1 += src1_stride;
			point_src2 += src2_stride;
		}

	} else {

		for (i = 0; i < height; i++) {
			psrc1 = point_src1;
			psrc2 = point_src2;

			ccInterMul1 = ccInterMul2 = dzero;
			ccInterMul3 = ccInterMul4 = dzero;
			ccInterAcc1 = ccInterAcc2 = dzero;
			ccSelfMul11 = ccSelfMul12 = dzero;
			ccSelfMul13 = ccSelfMul14 = dzero;
			ccSelfAcc11 = ccSelfAcc12 = dzero;
			ccSelfMul21 = ccSelfMul22 = dzero;
			ccSelfMul23 = ccSelfMul24 = dzero;
			ccSelfAcc21 = ccSelfAcc22 = dzero;
			sum11 = sum12 = dzero;
			sum21 = sum22 = dzero;
			tmpAcc = dzero;

/* prepare the source address */
			off = (mlib_addr)psrc1 & 7;
			emask = vis_edge8(psrc1, psrc1 + size_row - 1);
			num_row = size_row + off;
			psrc1 -= off;
			psrc2 -= off;
			dpsrc1 = (mlib_d64 *)psrc1;
			dpsrc2 = (mlib_d64 *)vis_alignaddr(psrc2, 0);

			midd[0] = midd[1] = dzero;
			vis_pst_8(dpsrc1[0], (void *)midd, emask);
			dx = midd[0];

			dy0 = vis_ld_d64_nf(dpsrc2);
			dy1 = vis_ld_d64_nf(dpsrc2 + 1);
			dy = vis_faligndata(dy0, dy1);
			vis_pst_8(dy, (void *)midd, emask);
			dy = midd[0];

			dpsrc1++;
			dpsrc2++;

			midd[0] = midd[1] = dzero;

			while (num_row > 7) {
				size = num_row;

				if (size > MAX_ELEM)
					size = MAX_ELEM;

				if ((((mlib_addr)psrc1 ^
					(mlib_addr)psrc2) & 7) == 0) {
					for (j = 0; j <= (size - 8); j += 8) {
						INTER_MUL_U8_U32
						INTER_SUM_U32_124
						IMG1_SELF_MUL_U8_U32
						IMG1_SELF_SUM_U32_124
						IMG2_SELF_MUL_U8_U32
						IMG2_SELF_SUM_U32_124
						IMG1_SUM_U32_124
						IMG2_SUM_U32_124

						dx = vis_ld_d64_nf(
							(mlib_u8 *)dpsrc1 +
							j);
						dy = vis_ld_d64_nf(
							(mlib_u8 *)dpsrc2 +
							j);
					}
					num_row -= j;
					dpsrc1 = (mlib_d64 *)(
						(mlib_u8 *)dpsrc1 + j);
					dpsrc2 = (mlib_d64 *)(
						(mlib_u8 *)dpsrc2 + j);
					dy = vis_ld_d64_nf(dpsrc2 - 1);

				} else {
					for (j = 0; j <= (size - 8); j += 8) {

						INTER_MUL_U8_U32
						INTER_SUM_U32_124
						IMG1_SELF_MUL_U8_U32
						IMG1_SELF_SUM_U32_124
						IMG2_SELF_MUL_U8_U32
						IMG2_SELF_SUM_U32_124
						IMG1_SUM_U32_124
						IMG2_SUM_U32_124

						dx = vis_ld_d64_nf(
							(mlib_u8 *)dpsrc1 +
							j);
						dy0 = vis_ld_d64_nf(
							(mlib_u8 *)dpsrc2 +
							j);
						dy1 = vis_ld_d64_nf(
							(mlib_u8 *)dpsrc2 +
							j + 8);

						dy = vis_faligndata(dy0, dy1);
					}
					num_row -= j;
					dpsrc1 = (mlib_d64 *)(
						(mlib_u8 *)dpsrc1 + j);
					dpsrc2 = (mlib_d64 *)(
						(mlib_u8 *)dpsrc2 + j);
					dy0 = vis_ld_d64_nf(dpsrc2 - 1);
					dy1 = vis_ld_d64_nf(dpsrc2);
					dy = vis_faligndata(dy0, dy1);
				}

				dd[0].db = ccInterAcc1;
				dd[1].db = ccInterAcc2;
				dd[2].db = ccSelfAcc11;
				dd[3].db = ccSelfAcc12;
				dd[4].db = ccSelfAcc21;
				dd[5].db = ccSelfAcc22;
				dd[6].db = sum11;
				dd[7].db = sum12;
				dd[8].db = sum21;
				dd[9].db = sum22;

				for (k = 0; k < 4; k++) {
					if ((k % 2) == 0) {
						rez[3 & (k - off)] +=
							(mlib_d64)dd[k /
							2].two_int.int0;
						rez[(3 & (k - off)) + 4] +=
							(mlib_d64)dd[k / 2 +
							2].two_int.int0;
						rez[(3 & (k - off)) + 8] +=
							(mlib_d64)dd[k / 2 +
							4].two_int.int0;
						rez[(3 & (k - off)) + 12] +=
							(mlib_d64)dd[k / 2 +
							6].two_int.int0;
						rez[(3 & (k - off)) + 16] +=
							(mlib_d64)dd[k / 2 +
							8].two_int.int0;
					} else {
						rez[3 & (k - off)] +=
							(mlib_d64)dd[k /
							2].two_int.int1;
						rez[(3 & (k - off)) + 4] +=
							(mlib_d64)dd[k / 2 +
							2].two_int.int1;
						rez[(3 & (k - off)) + 8] +=
							(mlib_d64)dd[k / 2 +
							4].two_int.int1;
						rez[(3 & (k - off)) + 12] +=
							(mlib_d64)dd[k / 2 +
							6].two_int.int1;
						rez[(3 & (k - off)) + 16] +=
							(mlib_d64)dd[k / 2 +
							8].two_int.int1;
					}

				}

				ccInterAcc1 = ccInterAcc2 = dzero;
				ccSelfAcc11 = ccSelfAcc12 = dzero;
				ccSelfAcc21 = ccSelfAcc22 = dzero;
				sum11 = sum12 = dzero;
				sum21 = sum22 = dzero;

			}

			if (num_row > 0) {

				emask =
					vis_edge8(dpsrc1,
					(mlib_u8 *)dpsrc1 + num_row - 1);
				vis_pst_8(dx, (void *)(midd + 1), emask);
				dx = midd[1];

				midd[1] = dzero;
				vis_pst_8(dy, (void *)(midd + 1), emask);
				dy = midd[1];

				INTER_MUL_U8_U32
				INTER_SUM_U32_124
				IMG1_SELF_MUL_U8_U32
				IMG1_SELF_SUM_U32_124
				IMG2_SELF_MUL_U8_U32
				IMG2_SELF_SUM_U32_124
				IMG1_SUM_U32_124
				IMG2_SUM_U32_124

				dd[0].db = ccInterAcc1;
				dd[1].db = ccInterAcc2;
				dd[2].db = ccSelfAcc11;
				dd[3].db = ccSelfAcc12;
				dd[4].db = ccSelfAcc21;
				dd[5].db = ccSelfAcc22;
				dd[6].db = sum11;
				dd[7].db = sum12;
				dd[8].db = sum21;
				dd[9].db = sum22;

				for (k = 0; k < 4; k++) {
					if ((k % 2) == 0) {
						rez[3 & (k - off)] +=
							(mlib_d64)dd[k /
							2].two_int.int0;
						rez[(3 & (k - off)) + 4] +=
							(mlib_d64)dd[k / 2 +
							2].two_int.int0;
						rez[(3 & (k - off)) + 8] +=
							(mlib_d64)dd[k / 2 +
							4].two_int.int0;
						rez[(3 & (k - off)) + 12] +=
							(mlib_d64)dd[k / 2 +
							6].two_int.int0;
						rez[(3 & (k - off)) + 16] +=
							(mlib_d64)dd[k / 2 +
							8].two_int.int0;
					} else {
						rez[3 & (k - off)] +=
							(mlib_d64)dd[k /
							2].two_int.int1;
						rez[(3 & (k - off)) + 4] +=
							(mlib_d64)dd[k / 2 +
							2].two_int.int1;
						rez[(3 & (k - off)) + 8] +=
							(mlib_d64)dd[k / 2 +
							4].two_int.int1;
						rez[(3 & (k - off)) + 12] +=
							(mlib_d64)dd[k / 2 +
							6].two_int.int1;
						rez[(3 & (k - off)) + 16] +=
							(mlib_d64)dd[k / 2 +
							8].two_int.int1;
					}

				}

			}

			point_src1 += src1_stride;
			point_src2 += src2_stride;
		}

	}

}

/* *********************************************************** */

#define	EMUL(freg)	vis_fmul8x16al(freg, fone)
#define	EMER(freg)	vis_fpmerge(vis_read_hi(dzero), freg)

/* *********************************************************** */

#define	INTER_SUM_U32_8(a) {                                    \
	switch (a) {                                                \
	case 0:                                                     \
		ccInterAcc1 = vis_fpadd32(ccInterMul1, ccInterAcc1);    \
		ccInterAcc2 = vis_fpadd32(ccInterMul2, ccInterAcc2);    \
		ccInterAcc3 = vis_fpadd32(ccInterMul3, ccInterAcc3);    \
		ccInterAcc1 = vis_fpadd32(ccInterMul4, ccInterAcc1);    \
		break;                                                  \
	case 1:                                                     \
		ccInterAcc2 = vis_fpadd32(ccInterMul1, ccInterAcc2);    \
		ccInterAcc3 = vis_fpadd32(ccInterMul2, ccInterAcc3);    \
		ccInterAcc1 = vis_fpadd32(ccInterMul3, ccInterAcc1);    \
		ccInterAcc2 = vis_fpadd32(ccInterMul4, ccInterAcc2);    \
		break;                                                  \
	case 2:                                                     \
		ccInterAcc3 = vis_fpadd32(ccInterMul1, ccInterAcc3);    \
		ccInterAcc1 = vis_fpadd32(ccInterMul2, ccInterAcc1);    \
		ccInterAcc2 = vis_fpadd32(ccInterMul3, ccInterAcc2);    \
		ccInterAcc3 = vis_fpadd32(ccInterMul4, ccInterAcc3);    \
		break;                                                  \
	}                                                           \
}
/* *********************************************************** */
#define	IMG1_SELF_SUM_U32_8(a) {                                \
	switch (a) {                                                \
	case 0:                                                     \
		ccSelfAcc11 = vis_fpadd32(ccSelfMul11, ccSelfAcc11);    \
		ccSelfAcc12 = vis_fpadd32(ccSelfMul12, ccSelfAcc12);    \
		ccSelfAcc13 = vis_fpadd32(ccSelfMul13, ccSelfAcc13);    \
		ccSelfAcc11 = vis_fpadd32(ccSelfMul14, ccSelfAcc11);    \
		break;                                                  \
	case 1:                                                     \
		ccSelfAcc12 = vis_fpadd32(ccSelfMul11, ccSelfAcc12);    \
		ccSelfAcc13 = vis_fpadd32(ccSelfMul12, ccSelfAcc13);    \
		ccSelfAcc11 = vis_fpadd32(ccSelfMul13, ccSelfAcc11);    \
		ccSelfAcc12 = vis_fpadd32(ccSelfMul14, ccSelfAcc12);    \
		break;                                                  \
	case 2:                                                     \
		ccSelfAcc13 = vis_fpadd32(ccSelfMul11, ccSelfAcc13);    \
		ccSelfAcc11 = vis_fpadd32(ccSelfMul12, ccSelfAcc11);    \
		ccSelfAcc12 = vis_fpadd32(ccSelfMul13, ccSelfAcc12);    \
		ccSelfAcc13 = vis_fpadd32(ccSelfMul14, ccSelfAcc13);    \
		break;                                                  \
	}                                                           \
}


/* *********************************************************** */

#define	IMG2_SELF_SUM_U32_8(a) {                                \
	switch (a) {                                                \
	case 0:                                                     \
		ccSelfAcc21 = vis_fpadd32(ccSelfMul21, ccSelfAcc21);    \
		ccSelfAcc22 = vis_fpadd32(ccSelfMul22, ccSelfAcc22);    \
		ccSelfAcc23 = vis_fpadd32(ccSelfMul23, ccSelfAcc23);    \
		ccSelfAcc21 = vis_fpadd32(ccSelfMul24, ccSelfAcc21);    \
		break;                                                  \
	case 1:                                                     \
		ccSelfAcc22 = vis_fpadd32(ccSelfMul21, ccSelfAcc22);    \
		ccSelfAcc23 = vis_fpadd32(ccSelfMul22, ccSelfAcc23);    \
		ccSelfAcc21 = vis_fpadd32(ccSelfMul23, ccSelfAcc21);    \
		ccSelfAcc22 = vis_fpadd32(ccSelfMul24, ccSelfAcc22);    \
		break;                                                  \
	case 2:                                                     \
		ccSelfAcc23 = vis_fpadd32(ccSelfMul21, ccSelfAcc23);    \
		ccSelfAcc21 = vis_fpadd32(ccSelfMul22, ccSelfAcc21);    \
		ccSelfAcc22 = vis_fpadd32(ccSelfMul23, ccSelfAcc22);    \
		ccSelfAcc23 = vis_fpadd32(ccSelfMul24, ccSelfAcc23);    \
		break;                                                  \
	}                                                           \
}
/* *********************************************************** */

#define	IMG1_SUM_U32_8(a) {                                 \
	switch (a) {                                            \
	case 0:                                                 \
		sum11 = vis_fpadd32(sum11,                          \
			vis_fmuld8ulx16(mult, vis_read_hi(dxh)));       \
		sum12 = vis_fpadd32(sum12,                          \
			vis_fmuld8ulx16(mult, vis_read_lo(dxh)));       \
		sum13 = vis_fpadd32(sum13,                          \
			vis_fmuld8ulx16(mult, vis_read_hi(dxl)));       \
		sum11 = vis_fpadd32(sum11,                          \
			vis_fmuld8ulx16(mult, vis_read_lo(dxl)));       \
		break;                                              \
	case 1:                                                 \
		sum12 = vis_fpadd32(sum12,                          \
			vis_fmuld8ulx16(mult, vis_read_hi(dxh)));       \
		sum13 = vis_fpadd32(sum13,                          \
			vis_fmuld8ulx16(mult, vis_read_lo(dxh)));       \
		sum11 = vis_fpadd32(sum11,                          \
			vis_fmuld8ulx16(mult, vis_read_hi(dxl)));       \
		sum12 = vis_fpadd32(sum12,                          \
			vis_fmuld8ulx16(mult, vis_read_lo(dxl)));       \
		break;                                              \
	case 2:                                                 \
		sum13 = vis_fpadd32(sum13,                          \
			vis_fmuld8ulx16(mult, vis_read_hi(dxh)));       \
		sum11 = vis_fpadd32(sum11,                          \
			vis_fmuld8ulx16(mult, vis_read_lo(dxh)));       \
		sum12 = vis_fpadd32(sum12,                          \
			vis_fmuld8ulx16(mult, vis_read_hi(dxl)));       \
		sum13 = vis_fpadd32(sum13,                          \
			vis_fmuld8ulx16(mult, vis_read_lo(dxl)));       \
		break;                                              \
	}                                                       \
}

/* *********************************************************** */

#define	IMG2_SUM_U32_8(a) {                                 \
	switch (a) {                                            \
	case 0:                                                 \
		sum21 = vis_fpadd32(sum21,                          \
			vis_fmuld8ulx16(mult, vis_read_hi(dyh)));       \
		sum22 = vis_fpadd32(sum22,                          \
			vis_fmuld8ulx16(mult, vis_read_lo(dyh)));       \
		sum23 = vis_fpadd32(sum23,                          \
			vis_fmuld8ulx16(mult, vis_read_hi(dyl)));       \
		sum21 = vis_fpadd32(sum21,                          \
			vis_fmuld8ulx16(mult, vis_read_lo(dyl)));       \
		break;                                              \
	case 1:                                                 \
		sum22 = vis_fpadd32(sum22,                          \
			vis_fmuld8ulx16(mult, vis_read_hi(dyh)));       \
		sum23 = vis_fpadd32(sum23,                          \
			vis_fmuld8ulx16(mult, vis_read_lo(dyh)));       \
		sum21 = vis_fpadd32(sum21,                          \
			vis_fmuld8ulx16(mult, vis_read_hi(dyl)));       \
		sum22 = vis_fpadd32(sum22,                          \
			vis_fmuld8ulx16(mult, vis_read_lo(dyl)));       \
		break;                                              \
	case 2:                                                 \
		sum23 = vis_fpadd32(sum23,                          \
			vis_fmuld8ulx16(mult, vis_read_hi(dyh)));       \
		sum21 = vis_fpadd32(sum21,                          \
			vis_fmuld8ulx16(mult, vis_read_lo(dyh)));       \
		sum22 = vis_fpadd32(sum22,                          \
			vis_fmuld8ulx16(mult, vis_read_hi(dyl)));       \
		sum23 = vis_fpadd32(sum23,                          \
			vis_fmuld8ulx16(mult, vis_read_lo(dyl)));       \
		break;                                              \
	}                                                       \
}

/* *********************************************************** */

void
mlib_v_ImageNormCrossCorrel_U8_3(
    const mlib_image *img1,
    const mlib_image *img2,
	const mlib_d64 *m2,
	const mlib_d64 *s2,
    mlib_d64 *rez)
{
/* pointer to the data of first image */
	mlib_u8 *point_src1 = (mlib_u8 *)mlib_ImageGetData(img1);

/* pointer to the data of second image */
	mlib_u8 *point_src2 = (mlib_u8 *)mlib_ImageGetData(img2);

/* height of image */
	mlib_s32 height = mlib_ImageGetHeight(img1);

/* elements to next row */
	mlib_s32 src1_stride = mlib_ImageGetStride(img1);

/* elements to next row */
	mlib_s32 src2_stride = mlib_ImageGetStride(img2);

/* number of elements in the row */
	mlib_s32 size_row = mlib_ImageGetWidth(img1) * 3;

/* indices */
	mlib_s32 i, j, k;

/* offset of address alignment in destination */
	mlib_s32 off;

/* edge masks */
	mlib_s32 emask;
	mlib_s32 size, num_row;
	mlib_u8 *psrc1, *psrc2;
	mlib_d64 midd[2], *dpsrc1, *dpsrc2;
	mlib_d64 dx, dy, dxh, dxl, dy0, dy1, dyh, dyl;
	const mlib_f32 fone = vis_to_float(0x100);

	const mlib_u8 al2off[12] = { 0, 2, 1, 0, 2, 1, 0, 2, 1, 0, 2, 1 };
	type_union_mlib_d64 dd[15];
	const mlib_d64 dzero = vis_fzero();

	mlib_d64 ccInterMul1, ccInterMul2, ccInterMul3, ccInterMul4;
	mlib_d64 ccInterAcc1, ccInterAcc2, ccInterAcc3;
	mlib_d64 ccSelfMul11, ccSelfMul12, ccSelfMul13, ccSelfMul14;
	mlib_d64 ccSelfAcc11, ccSelfAcc12, ccSelfAcc13;
	mlib_d64 ccSelfMul21, ccSelfMul22, ccSelfMul23, ccSelfMul24;
	mlib_d64 ccSelfAcc21, ccSelfAcc22, ccSelfAcc23;
	mlib_d64 sum11, sum12, sum13;
	mlib_d64 sum21, sum22, sum23;

	mlib_f32 mult = vis_to_float(0x10001);


	if (src1_stride == size_row && src1_stride == src2_stride) {
/* This image is not a sub-image and can be treated as a 1-D vector */
		size_row *= height;
		height = 1;
	}


	/* calculate image cross correlation for 3 channels */
	if (m2 != NULL && s2 != NULL) {
		for (i = 0; i < height; i++) {
			num_row = size_row;
			psrc1 = point_src1;
			psrc2 = point_src2;

			/* prepare the source address */
			off = (mlib_addr)psrc1 & 7;
			emask = vis_edge8(psrc1, psrc1 + size_row - 1);
			num_row = size_row + off;
			psrc1 -= off;
			psrc2 -= off;
			dpsrc1 = (mlib_d64 *)psrc1;
			dpsrc2 = (mlib_d64 *)vis_alignaddr(psrc2, 0);

			midd[0] = midd[1] = dzero;

			ccInterMul1 = ccInterMul2 = dzero;
			ccInterMul3 = ccInterMul4 = dzero;
			ccInterAcc1 = ccInterAcc2 = ccInterAcc3 = dzero;
			ccSelfMul11 = ccSelfMul12 = dzero;
			ccSelfMul13 = ccSelfMul14 = dzero;
			ccSelfAcc11 = ccSelfAcc12 = ccSelfAcc13 = dzero;
			sum11 = sum12 = sum13 = dzero;

			vis_pst_8(dpsrc1[0], (void *)midd, emask);
			dx = midd[0];

			dy0 = vis_ld_d64_nf(dpsrc2);
			dy1 = vis_ld_d64_nf(dpsrc2 + 1);
			midd[0] = dzero;
			dy = vis_faligndata(dy0, dy1);

			vis_pst_8(dy, (void *)midd, emask);
			dy = midd[0];

			size = num_row & ~7;
			while (num_row >= 8) {

				if (size > MAX_ELEM / 4 * 3)
					size = MAX_ELEM / 4 * 3;

				if ((((mlib_addr)psrc1 ^
					(mlib_addr)psrc2) & 7) == 0) {
#pragma pipeloop(0)
					for (j = 0; j <= (size - 8); j += 24) {
						INTER_MUL_U8_U32
						INTER_SUM_U32_8(0)
						IMG1_SELF_MUL_U8_U32
						IMG1_SELF_SUM_U32_8(0)
						IMG1_SUM_U32_8(0)

						dx = vis_ld_d64_nf(
							(mlib_u8 *)dpsrc1 +
							j + 24);
						dy = vis_ld_d64_nf(
							(mlib_u8 *)dpsrc2 +
							j + 24);
					}

					dx = vis_ld_d64_nf(dpsrc1 + 1);
					dy = vis_ld_d64_nf(dpsrc2 + 1);
					for (j = 8; j <= (size - 8); j += 24) {
						INTER_MUL_U8_U32
						INTER_SUM_U32_8(1)
						IMG1_SELF_MUL_U8_U32
						IMG1_SELF_SUM_U32_8(1)
						IMG1_SUM_U32_8(1)

						dx = vis_ld_d64_nf(
							(mlib_u8 *)dpsrc1 +
							j + 24);
						dy = vis_ld_d64_nf(
							(mlib_u8 *)dpsrc2 +
							j + 24);
					}

					dx = vis_ld_d64_nf(dpsrc1 + 2);
					dy = vis_ld_d64_nf(dpsrc2 + 2);
					for (j = 16; j <= (size - 8); j += 24) {
						INTER_MUL_U8_U32
						INTER_SUM_U32_8(2)
						IMG1_SELF_MUL_U8_U32
						IMG1_SELF_SUM_U32_8(2)
						IMG1_SUM_U32_8(2)

						dx = vis_ld_d64_nf(
							(mlib_u8 *)dpsrc1 +
							j + 24);
						dy = vis_ld_d64_nf(
							(mlib_u8 *)dpsrc2 +
							j + 24);
					}
				} else {
#pragma pipeloop(0)
					for (j = 0; j <= (size - 8); j += 24) {
						INTER_MUL_U8_U32
						INTER_SUM_U32_8(0)
						IMG1_SELF_MUL_U8_U32
						IMG1_SELF_SUM_U32_8(0)
						IMG1_SUM_U32_8(0)

						dx = vis_ld_d64_nf(
							(mlib_u8 *)dpsrc1 +
							j + 24);
						dy0 = vis_ld_d64_nf(
							(mlib_u8 *)dpsrc2 +
							j + 24);
						dy1 = vis_ld_d64_nf(
							(mlib_u8 *)dpsrc2 +
							j + 32);
						dy = vis_faligndata(dy0, dy1);
					}

					dx = vis_ld_d64_nf(dpsrc1 + 1);
					dy0 = vis_ld_d64_nf(dpsrc2 + 1);
					dy1 = vis_ld_d64_nf(dpsrc2 + 2);
					for (j = 8; j <= (size - 8); j += 24) {
						dy = vis_faligndata(dy0, dy1);

						INTER_MUL_U8_U32
						INTER_SUM_U32_8(1)
						IMG1_SELF_MUL_U8_U32
						IMG1_SELF_SUM_U32_8(1)
						IMG1_SUM_U32_8(1)

						dx = vis_ld_d64_nf(
							(mlib_u8 *)dpsrc1 +
							j + 24);
						dy0 = vis_ld_d64_nf(
							(mlib_u8 *)dpsrc2 +
							j + 24);
						dy1 = vis_ld_d64_nf(
							(mlib_u8 *)dpsrc2 +
							j + 32);
					}

					dx = vis_ld_d64_nf(dpsrc1 + 2);
					dy0 = vis_ld_d64_nf(dpsrc2 + 2);
					dy1 = vis_ld_d64_nf(dpsrc2 + 3);
					for (j = 16; j <= (size - 8); j += 24) {
						dy = vis_faligndata(dy0, dy1);

						INTER_MUL_U8_U32
						INTER_SUM_U32_8(2)
						IMG1_SELF_MUL_U8_U32
						IMG1_SELF_SUM_U32_8(2)
						IMG1_SUM_U32_8(2)

						dx = vis_ld_d64_nf(
							(mlib_u8 *)dpsrc1 +
							j + 24);
						dy0 = vis_ld_d64_nf(
							(mlib_u8 *)dpsrc2 +
							j + 24);
						dy1 = vis_ld_d64_nf(
							(mlib_u8 *)dpsrc2 +
							j + 32);
					}

				}

				num_row -= size;
				dpsrc1 = (mlib_d64 *)((mlib_u8 *)dpsrc1 + size);
				dpsrc2 = (mlib_d64 *)((mlib_u8 *)dpsrc2 + size);
				dx = vis_ld_d64_nf(dpsrc1);
				dy0 = vis_ld_d64_nf(dpsrc2);
				dy1 = vis_ld_d64_nf(dpsrc2 + 1);
				dy = vis_faligndata(dy0, dy1);

				dd[0].db = ccInterAcc1;
				dd[3].db = ccSelfAcc11;
				dd[9].db = sum11;
				dd[1].db = ccInterAcc2;
				dd[4].db = ccSelfAcc12;
				dd[10].db = sum12;
				dd[2].db = ccInterAcc3;
				dd[5].db = ccSelfAcc13;
				dd[11].db = sum13;

				dd[0].two_float.fl0 =
				    vis_fpadd32s(dd[0].two_float.fl0,
					dd[1].two_float.fl1);
				dd[0].two_float.fl1 =
				    vis_fpadd32s(dd[0].two_float.fl1,
					dd[2].two_float.fl0);
				dd[1].two_float.fl0 =
				    vis_fpadd32s(dd[1].two_float.fl0,
					dd[2].two_float.fl1);
				rez[al2off[off + 0]] += dd[0].two_int.int0;
				rez[al2off[off + 2]] += dd[0].two_int.int1;
				rez[al2off[off + 1]] += dd[1].two_int.int0;

				dd[3].two_float.fl0 =
				    vis_fpadd32s(dd[3].two_float.fl0,
					dd[4].two_float.fl1);
				dd[3].two_float.fl1 =
				    vis_fpadd32s(dd[3].two_float.fl1,
					dd[5].two_float.fl0);
				dd[4].two_float.fl0 =
				    vis_fpadd32s(dd[4].two_float.fl0,
					dd[5].two_float.fl1);
				rez[al2off[off + 0] + 4] += dd[3].two_int.int0;
				rez[al2off[off + 2] + 4] += dd[3].two_int.int1;
				rez[al2off[off + 1] + 4] += dd[4].two_int.int0;

				dd[9].two_float.fl0 =
				    vis_fpadd32s(dd[9].two_float.fl0,
					dd[10].two_float.fl1);
				dd[9].two_float.fl1 =
				    vis_fpadd32s(dd[9].two_float.fl1,
					dd[11].two_float.fl0);
				dd[10].two_float.fl0 =
				    vis_fpadd32s(dd[10].two_float.fl0,
					dd[11].two_float.fl1);
				rez[al2off[off + 0] + 12] +=
					dd[9].two_int.int0;
				rez[al2off[off + 2] + 12] +=
					dd[9].two_int.int1;
				rez[al2off[off + 1] + 12] +=
					dd[10].two_int.int0;

				ccInterAcc1 = ccInterAcc2 = ccInterAcc3 = dzero;
				ccSelfAcc11 = ccSelfAcc12 = ccSelfAcc13 = dzero;
				sum11 = sum12 = sum13 = dzero;

			}

			if (num_row > 0) {

				emask = (mlib_s32)0xff00 >> num_row;
				vis_pst_8(dx, (void *)(midd + 1), emask);
				dx = midd[1];

				midd[1] = dzero;
				vis_pst_8(dy, (void *)(midd + 1), emask);
				dy = midd[1];

				switch ((size >> 3) % 3) {
					case 0:
						INTER_MUL_U8_U32
						INTER_SUM_U32_8(0)
						IMG1_SELF_MUL_U8_U32
						IMG1_SELF_SUM_U32_8(0)
						IMG1_SUM_U32_8(0)
						break;
					case 1:
						INTER_MUL_U8_U32
						INTER_SUM_U32_8(1)
						IMG1_SELF_MUL_U8_U32
						IMG1_SELF_SUM_U32_8(1)
						IMG1_SUM_U32_8(1)
						break;
					case 2:
						INTER_MUL_U8_U32
						INTER_SUM_U32_8(2)
						IMG1_SELF_MUL_U8_U32
						IMG1_SELF_SUM_U32_8(2)
						IMG1_SUM_U32_8(2)
						break;
				}

				dd[0].db = ccInterAcc1;
				dd[3].db = ccSelfAcc11;
				dd[9].db = sum11;
				dd[1].db = ccInterAcc2;
				dd[4].db = ccSelfAcc12;
				dd[10].db = sum12;
				dd[2].db = ccInterAcc3;
				dd[5].db = ccSelfAcc13;
				dd[11].db = sum13;

				dd[0].two_float.fl0 =
				    vis_fpadd32s(dd[0].two_float.fl0,
					dd[1].two_float.fl1);
				dd[0].two_float.fl1 =
				    vis_fpadd32s(dd[0].two_float.fl1,
					dd[2].two_float.fl0);
				dd[1].two_float.fl0 =
				    vis_fpadd32s(dd[1].two_float.fl0,
					dd[2].two_float.fl1);
				rez[al2off[off + 0]] +=
					dd[0].two_int.int0;
				rez[al2off[off + 2]] +=
					dd[0].two_int.int1;
				rez[al2off[off + 1]] +=
					dd[1].two_int.int0;

				dd[3].two_float.fl0 =
				    vis_fpadd32s(dd[3].two_float.fl0,
					dd[4].two_float.fl1);
				dd[3].two_float.fl1 =
				    vis_fpadd32s(dd[3].two_float.fl1,
					dd[5].two_float.fl0);
				dd[4].two_float.fl0 =
				    vis_fpadd32s(dd[4].two_float.fl0,
					dd[5].two_float.fl1);
				rez[al2off[off + 0] + 4] +=
					dd[3].two_int.int0;
				rez[al2off[off + 2] + 4] +=
					dd[3].two_int.int1;
				rez[al2off[off + 1] + 4] +=
					dd[4].two_int.int0;

				dd[9].two_float.fl0 =
				    vis_fpadd32s(dd[9].two_float.fl0,
					dd[10].two_float.fl1);
				dd[9].two_float.fl1 =
				    vis_fpadd32s(dd[9].two_float.fl1,
					dd[11].two_float.fl0);
				dd[10].two_float.fl0 =
				    vis_fpadd32s(dd[10].two_float.fl0,
					dd[11].two_float.fl1);
				rez[al2off[off + 0] + 12] +=
					dd[9].two_int.int0;
				rez[al2off[off + 2] + 12] +=
					dd[9].two_int.int1;
				rez[al2off[off + 1] + 12] +=
					dd[10].two_int.int0;

			}

			point_src1 += src1_stride;
			point_src2 += src2_stride;
		}
	} else {
		for (i = 0; i < height; i++) {
			num_row = size_row;
			psrc1 = point_src1;
			psrc2 = point_src2;

			/* prepare the source address */
			off = (mlib_addr)psrc1 & 7;
			emask = vis_edge8(psrc1, psrc1 + size_row - 1);
			num_row = size_row + off;
			psrc1 -= off;
			psrc2 -= off;
			dpsrc1 = (mlib_d64 *)psrc1;
			dpsrc2 = (mlib_d64 *)vis_alignaddr(psrc2, 0);

			midd[0] = midd[1] = dzero;

			ccInterMul1 = ccInterMul2 = dzero;
			ccInterMul3 = ccInterMul4 = dzero;
			ccInterAcc1 = ccInterAcc2 =  ccInterAcc3 = dzero;
			ccSelfMul11 = ccSelfMul12 = dzero;
			ccSelfMul13 = ccSelfMul14 = dzero;
			ccSelfAcc11 = ccSelfAcc12 = ccSelfAcc13 = dzero;
			ccSelfMul21 = ccSelfMul22 = dzero;
			ccSelfMul23 = ccSelfMul24 = dzero;
			ccSelfAcc21 = ccSelfAcc22 = ccSelfAcc23 = dzero;
			sum11 = sum12 = sum13 = dzero;
			sum21 = sum22 = sum23 = dzero;

			vis_pst_8(dpsrc1[0], (void *)midd, emask);
			dx = midd[0];

			dy0 = vis_ld_d64_nf(dpsrc2);
			dy1 = vis_ld_d64_nf(dpsrc2 + 1);
			midd[0] = dzero;
			dy = vis_faligndata(dy0, dy1);

			vis_pst_8(dy, (void *)midd, emask);
			dy = midd[0];

			size = num_row & ~7;
			while (num_row >= 8) {

				if (size > MAX_ELEM / 4 * 3)
					size = MAX_ELEM / 4 * 3;

				if ((((mlib_addr)psrc1 ^
					(mlib_addr)psrc2) & 7) == 0) {
#pragma pipeloop(0)
					for (j = 0; j <= (size - 8); j += 24) {
						INTER_MUL_U8_U32
						INTER_SUM_U32_8(0)
						IMG1_SELF_MUL_U8_U32
						IMG1_SELF_SUM_U32_8(0)
						IMG2_SELF_MUL_U8_U32
						IMG2_SELF_SUM_U32_8(0)
						IMG1_SUM_U32_8(0)
						IMG2_SUM_U32_8(0)

						dx = vis_ld_d64_nf(
							(mlib_u8 *)dpsrc1 +
							j + 24);
						dy = vis_ld_d64_nf(
							(mlib_u8 *)dpsrc2 +
							j + 24);
					}

					dx = vis_ld_d64_nf(dpsrc1 + 1);
					dy = vis_ld_d64_nf(dpsrc2 + 1);
					for (j = 8; j <= (size - 8); j += 24) {
						INTER_MUL_U8_U32
						INTER_SUM_U32_8(1)
						IMG1_SELF_MUL_U8_U32
						IMG1_SELF_SUM_U32_8(1)
						IMG2_SELF_MUL_U8_U32
						IMG2_SELF_SUM_U32_8(1)
						IMG1_SUM_U32_8(1)
						IMG2_SUM_U32_8(1)

						dx = vis_ld_d64_nf(
							(mlib_u8 *)dpsrc1 +
							j + 24);
						dy = vis_ld_d64_nf(
							(mlib_u8 *)dpsrc2 +
							j + 24);
					}

					dx = vis_ld_d64_nf(dpsrc1 + 2);
					dy = vis_ld_d64_nf(dpsrc2 + 2);
					for (j = 16; j <= (size - 8); j += 24) {
						INTER_MUL_U8_U32
						INTER_SUM_U32_8(2)
						IMG1_SELF_MUL_U8_U32
						IMG1_SELF_SUM_U32_8(2)
						IMG2_SELF_MUL_U8_U32
						IMG2_SELF_SUM_U32_8(2)
						IMG1_SUM_U32_8(2)
						IMG2_SUM_U32_8(2)

						dx = vis_ld_d64_nf(
							(mlib_u8 *)dpsrc1 +
							j + 24);
						dy = vis_ld_d64_nf(
							(mlib_u8 *)dpsrc2 +
							j + 24);
					}
				} else {
#pragma pipeloop(0)
					for (j = 0; j <= (size - 8); j += 24) {
						INTER_MUL_U8_U32
						INTER_SUM_U32_8(0)
						IMG1_SELF_MUL_U8_U32
						IMG1_SELF_SUM_U32_8(0)
						IMG2_SELF_MUL_U8_U32
						IMG2_SELF_SUM_U32_8(0)
						IMG1_SUM_U32_8(0)
						IMG2_SUM_U32_8(0)

						dx = vis_ld_d64_nf(
							(mlib_u8 *)dpsrc1 +
							j + 24);
						dy0 = vis_ld_d64_nf(
							(mlib_u8 *)dpsrc2 +
							j + 24);
						dy1 = vis_ld_d64_nf(
							(mlib_u8 *)dpsrc2 +
							j + 32);
						dy = vis_faligndata(dy0, dy1);
					}

					dx = vis_ld_d64_nf(dpsrc1 + 1);
					dy0 = vis_ld_d64_nf(dpsrc2 + 1);
					dy1 = vis_ld_d64_nf(dpsrc2 + 2);
					for (j = 8; j <= (size - 8); j += 24) {
						dy = vis_faligndata(dy0, dy1);

						INTER_MUL_U8_U32
						INTER_SUM_U32_8(1)
						IMG1_SELF_MUL_U8_U32
						IMG1_SELF_SUM_U32_8(1)
						IMG2_SELF_MUL_U8_U32
						IMG2_SELF_SUM_U32_8(1)
						IMG1_SUM_U32_8(1)
						IMG2_SUM_U32_8(1)

						dx = vis_ld_d64_nf(
							(mlib_u8 *)dpsrc1 +
							j + 24);
						dy0 = vis_ld_d64_nf(
							(mlib_u8 *)dpsrc2 +
							j + 24);
						dy1 = vis_ld_d64_nf(
							(mlib_u8 *)dpsrc2 +
							j + 32);
					}

					dx = vis_ld_d64_nf(dpsrc1 + 2);
					dy0 = vis_ld_d64_nf(dpsrc2 + 2);
					dy1 = vis_ld_d64_nf(dpsrc2 + 3);
					for (j = 16; j <= (size - 8); j += 24) {
						dy = vis_faligndata(dy0, dy1);

						INTER_MUL_U8_U32
						INTER_SUM_U32_8(2)
						IMG1_SELF_MUL_U8_U32
						IMG1_SELF_SUM_U32_8(2)
						IMG2_SELF_MUL_U8_U32
						IMG2_SELF_SUM_U32_8(2)
						IMG1_SUM_U32_8(2)
						IMG2_SUM_U32_8(2)

						dx = vis_ld_d64_nf(
							(mlib_u8 *)dpsrc1 +
							j + 24);
						dy0 = vis_ld_d64_nf(
							(mlib_u8 *)dpsrc2 +
							j + 24);
						dy1 = vis_ld_d64_nf(
							(mlib_u8 *)dpsrc2 +
							j + 32);
					}

				}

				num_row -= size;
				dpsrc1 = (mlib_d64 *)((mlib_u8 *)dpsrc1 + size);
				dpsrc2 = (mlib_d64 *)((mlib_u8 *)dpsrc2 + size);
				dx = vis_ld_d64_nf(dpsrc1);
				dy0 = vis_ld_d64_nf(dpsrc2);
				dy1 = vis_ld_d64_nf(dpsrc2 + 1);
				dy = vis_faligndata(dy0, dy1);

				dd[0].db = ccInterAcc1;
				dd[3].db = ccSelfAcc11;
				dd[6].db = ccSelfAcc21;
				dd[9].db = sum11;
				dd[12].db = sum21;
				dd[1].db = ccInterAcc2;
				dd[4].db = ccSelfAcc12;
				dd[7].db = ccSelfAcc22;
				dd[10].db = sum12;
				dd[13].db = sum22;
				dd[2].db = ccInterAcc3;
				dd[5].db = ccSelfAcc13;
				dd[8].db = ccSelfAcc23;
				dd[11].db = sum13;
				dd[14].db = sum23;

				dd[0].two_float.fl0 =
				    vis_fpadd32s(dd[0].two_float.fl0,
					dd[1].two_float.fl1);
				dd[0].two_float.fl1 =
				    vis_fpadd32s(dd[0].two_float.fl1,
					dd[2].two_float.fl0);
				dd[1].two_float.fl0 =
				    vis_fpadd32s(dd[1].two_float.fl0,
					dd[2].two_float.fl1);
				rez[al2off[off + 0]] +=
					dd[0].two_int.int0;
				rez[al2off[off + 2]] +=
					dd[0].two_int.int1;
				rez[al2off[off + 1]] +=
					dd[1].two_int.int0;

				dd[3].two_float.fl0 =
				    vis_fpadd32s(dd[3].two_float.fl0,
					dd[4].two_float.fl1);
				dd[3].two_float.fl1 =
				    vis_fpadd32s(dd[3].two_float.fl1,
					dd[5].two_float.fl0);
				dd[4].two_float.fl0 =
				    vis_fpadd32s(dd[4].two_float.fl0,
					dd[5].two_float.fl1);
				rez[al2off[off + 0] + 4] +=
					dd[3].two_int.int0;
				rez[al2off[off + 2] + 4] +=
					dd[3].two_int.int1;
				rez[al2off[off + 1] + 4] +=
					dd[4].two_int.int0;

				dd[6].two_float.fl0 =
				    vis_fpadd32s(dd[6].two_float.fl0,
					dd[7].two_float.fl1);
				dd[6].two_float.fl1 =
				    vis_fpadd32s(dd[6].two_float.fl1,
					dd[8].two_float.fl0);
				dd[7].two_float.fl0 =
				    vis_fpadd32s(dd[7].two_float.fl0,
					dd[8].two_float.fl1);
				rez[al2off[off + 0] + 8] +=
					dd[6].two_int.int0;
				rez[al2off[off + 2] + 8] +=
					dd[6].two_int.int1;
				rez[al2off[off + 1] + 8] +=
					dd[7].two_int.int0;

				dd[9].two_float.fl0 =
				    vis_fpadd32s(dd[9].two_float.fl0,
					dd[10].two_float.fl1);
				dd[9].two_float.fl1 =
				    vis_fpadd32s(dd[9].two_float.fl1,
					dd[11].two_float.fl0);
				dd[10].two_float.fl0 =
				    vis_fpadd32s(dd[10].two_float.fl0,
					dd[11].two_float.fl1);
				rez[al2off[off + 0] + 12] +=
					dd[9].two_int.int0;
				rez[al2off[off + 2] + 12] +=
					dd[9].two_int.int1;
				rez[al2off[off + 1] + 12] +=
					dd[10].two_int.int0;

				dd[12].two_float.fl0 =
				    vis_fpadd32s(dd[12].two_float.fl0,
					dd[13].two_float.fl1);
				dd[12].two_float.fl1 =
				    vis_fpadd32s(dd[12].two_float.fl1,
					dd[14].two_float.fl0);
				dd[13].two_float.fl0 =
				    vis_fpadd32s(dd[13].two_float.fl0,
					dd[14].two_float.fl1);
				rez[al2off[off + 0] + 16] +=
					dd[12].two_int.int0;
				rez[al2off[off + 2] + 16] +=
					dd[12].two_int.int1;
				rez[al2off[off + 1] + 16] +=
					dd[13].two_int.int0;

				ccInterAcc1 = ccInterAcc2 = ccInterAcc3 = dzero;
				ccSelfAcc11 = ccSelfAcc12 = ccSelfAcc13 = dzero;
				ccSelfAcc21 = ccSelfAcc22 = ccSelfAcc23 = dzero;
				sum11 = sum12 = sum13 = dzero;
				sum21 = sum22 = sum23 = dzero;

			}

			if (num_row > 0) {

				emask = (mlib_s32)0xff00 >> num_row;
				vis_pst_8(dx, (void *)(midd + 1), emask);
				dx = midd[1];

				midd[1] = dzero;
				vis_pst_8(dy, (void *)(midd + 1), emask);
				dy = midd[1];

				switch ((size >> 3) % 3) {
					case 0:
						INTER_MUL_U8_U32
						INTER_SUM_U32_8(0)
						IMG1_SELF_MUL_U8_U32
						IMG1_SELF_SUM_U32_8(0)
						IMG2_SELF_MUL_U8_U32
						IMG2_SELF_SUM_U32_8(0)
						IMG1_SUM_U32_8(0)
						IMG2_SUM_U32_8(0)
						break;
					case 1:
						INTER_MUL_U8_U32
						INTER_SUM_U32_8(1)
						IMG1_SELF_MUL_U8_U32
						IMG1_SELF_SUM_U32_8(1)
						IMG2_SELF_MUL_U8_U32
						IMG2_SELF_SUM_U32_8(1)
						IMG1_SUM_U32_8(1)
						IMG2_SUM_U32_8(1)
						break;
					case 2:
						INTER_MUL_U8_U32
						INTER_SUM_U32_8(2)
						IMG1_SELF_MUL_U8_U32
						IMG1_SELF_SUM_U32_8(2)
						IMG2_SELF_MUL_U8_U32
						IMG2_SELF_SUM_U32_8(2)
						IMG1_SUM_U32_8(2)
						IMG2_SUM_U32_8(2)
						break;
				}
				dd[0].db = ccInterAcc1;
				dd[3].db = ccSelfAcc11;
				dd[6].db = ccSelfAcc21;
				dd[9].db = sum11;
				dd[12].db = sum21;
				dd[1].db = ccInterAcc2;
				dd[4].db = ccSelfAcc12;
				dd[7].db = ccSelfAcc22;
				dd[10].db = sum12;
				dd[13].db = sum22;
				dd[2].db = ccInterAcc3;
				dd[5].db = ccSelfAcc13;
				dd[8].db = ccSelfAcc23;
				dd[11].db = sum13;
				dd[14].db = sum23;

				dd[0].two_float.fl0 =
				    vis_fpadd32s(dd[0].two_float.fl0,
					dd[1].two_float.fl1);
				dd[0].two_float.fl1 =
				    vis_fpadd32s(dd[0].two_float.fl1,
					dd[2].two_float.fl0);
				dd[1].two_float.fl0 =
				    vis_fpadd32s(dd[1].two_float.fl0,
					dd[2].two_float.fl1);
				rez[al2off[off + 0]] += dd[0].two_int.int0;
				rez[al2off[off + 2]] += dd[0].two_int.int1;
				rez[al2off[off + 1]] += dd[1].two_int.int0;

				dd[3].two_float.fl0 =
				    vis_fpadd32s(dd[3].two_float.fl0,
					dd[4].two_float.fl1);
				dd[3].two_float.fl1 =
				    vis_fpadd32s(dd[3].two_float.fl1,
					dd[5].two_float.fl0);
				dd[4].two_float.fl0 =
				    vis_fpadd32s(dd[4].two_float.fl0,
					dd[5].two_float.fl1);
				rez[al2off[off + 0] + 4] +=
					dd[3].two_int.int0;
				rez[al2off[off + 2] + 4] +=
					dd[3].two_int.int1;
				rez[al2off[off + 1] + 4] +=
					dd[4].two_int.int0;

				dd[6].two_float.fl0 =
				    vis_fpadd32s(dd[6].two_float.fl0,
					dd[7].two_float.fl1);
				dd[6].two_float.fl1 =
				    vis_fpadd32s(dd[6].two_float.fl1,
					dd[8].two_float.fl0);
				dd[7].two_float.fl0 =
				    vis_fpadd32s(dd[7].two_float.fl0,
					dd[8].two_float.fl1);
				rez[al2off[off + 0] + 8] +=
					dd[6].two_int.int0;
				rez[al2off[off + 2] + 8] +=
					dd[6].two_int.int1;
				rez[al2off[off + 1] + 8] +=
					dd[7].two_int.int0;

				dd[9].two_float.fl0 =
				    vis_fpadd32s(dd[9].two_float.fl0,
					dd[10].two_float.fl1);
				dd[9].two_float.fl1 =
				    vis_fpadd32s(dd[9].two_float.fl1,
					dd[11].two_float.fl0);
				dd[10].two_float.fl0 =
				    vis_fpadd32s(dd[10].two_float.fl0,
					dd[11].two_float.fl1);
				rez[al2off[off + 0] + 12] +=
					dd[9].two_int.int0;
				rez[al2off[off + 2] + 12] +=
					dd[9].two_int.int1;
				rez[al2off[off + 1] + 12] +=
					dd[10].two_int.int0;

				dd[12].two_float.fl0 =
				    vis_fpadd32s(dd[12].two_float.fl0,
					dd[13].two_float.fl1);
				dd[12].two_float.fl1 =
				    vis_fpadd32s(dd[12].two_float.fl1,
					dd[14].two_float.fl0);
				dd[13].two_float.fl0 =
				    vis_fpadd32s(dd[13].two_float.fl0,
					dd[14].two_float.fl1);
				rez[al2off[off + 0] + 16] +=
					dd[12].two_int.int0;
				rez[al2off[off + 2] + 16] +=
					dd[12].two_int.int1;
				rez[al2off[off + 1] + 16] +=
					dd[13].two_int.int0;

			}

			point_src1 += src1_stride;
			point_src2 += src2_stride;
		}
	}

}
/* *********************************************************** */
#define	INTER_MUL_U16_S32                                             \
	dr1 = vis_fmuld8sux16(vis_read_hi(dx), vis_read_hi(dy));          \
	dr2 = vis_fmuld8ulx16(vis_read_hi(dx), vis_read_hi(dy));          \
	dr3 = vis_fmuld8sux16(vis_read_lo(dx), vis_read_lo(dy));          \
	dr4 = vis_fmuld8ulx16(vis_read_lo(dx), vis_read_lo(dy));          \
	dr1 = vis_fpadd32(dr1, mone);                                     \
	dr3 = vis_fpadd32(dr3, mone);                                     \
	ccTmpInterAcc1.db = vis_fpadd32(dr1, dr2);                        \
	ccTmpInterAcc2.db = vis_fpadd32(dr3, dr4);                        \
	dr5 = vis_fmuld8sux16(vis_read_hi(dx1), vis_read_hi(dy2));        \
	dr6 = vis_fmuld8ulx16(vis_read_hi(dx1), vis_read_hi(dy2));        \
	dr7 = vis_fmuld8sux16(vis_read_lo(dx1), vis_read_lo(dy2));        \
	dr8 = vis_fmuld8ulx16(vis_read_lo(dx1), vis_read_lo(dy2));        \
	ccTmpInterAcc1.db = vis_fpadd32(ccTmpInterAcc1.db, dr5);          \
	ccTmpInterAcc1.db = vis_fpadd32(ccTmpInterAcc1.db, dr6);          \
	ccTmpInterAcc2.db = vis_fpadd32(ccTmpInterAcc2.db, dr7);          \
	ccTmpInterAcc2.db = vis_fpadd32(ccTmpInterAcc2.db, dr8);          \
	ccInterAcc1 += (mlib_d64)ccTmpInterAcc1.two_int.int0;             \
	ccInterAcc2 += (mlib_d64)ccTmpInterAcc1.two_int.int1;             \
	ccInterAcc3 += (mlib_d64)ccTmpInterAcc2.two_int.int0;             \
	ccInterAcc4 += (mlib_d64)ccTmpInterAcc2.two_int.int1;

/* *********************************************************** */

#define	IMG1_SELF_MUL_U16_S32                                         \
	dr1 = vis_fmuld8sux16(vis_read_hi(dx), vis_read_hi(dx));          \
	dr2 = vis_fmuld8ulx16(vis_read_hi(dx), vis_read_hi(dx));          \
	dr3 = vis_fmuld8sux16(vis_read_lo(dx), vis_read_lo(dx));          \
	dr4 = vis_fmuld8ulx16(vis_read_lo(dx), vis_read_lo(dx));          \
	dr1 = vis_fpadd32(dr1, mone);                                     \
	dr3 = vis_fpadd32(dr3, mone);                                     \
	ccTmpSelfAcc11.db = vis_fpadd32(dr1, dr2);                        \
	ccTmpSelfAcc12.db = vis_fpadd32(dr3, dr4);                        \
	dr5 = vis_fmuld8sux16(vis_read_hi(dx1), vis_read_hi(dx1));        \
	dr6 = vis_fmuld8ulx16(vis_read_hi(dx1), vis_read_hi(dx1));        \
	dr7 = vis_fmuld8sux16(vis_read_lo(dx1), vis_read_lo(dx1));        \
	dr8 = vis_fmuld8ulx16(vis_read_lo(dx1), vis_read_lo(dx1));        \
	ccTmpSelfAcc11.db = vis_fpadd32(ccTmpSelfAcc11.db, dr5);          \
	ccTmpSelfAcc11.db = vis_fpadd32(ccTmpSelfAcc11.db, dr6);          \
	ccTmpSelfAcc12.db = vis_fpadd32(ccTmpSelfAcc12.db, dr7);          \
	ccTmpSelfAcc12.db = vis_fpadd32(ccTmpSelfAcc12.db, dr8);          \
	ccSelfAcc11 += (mlib_d64)ccTmpSelfAcc11.two_int.int0;             \
	ccSelfAcc12 += (mlib_d64)ccTmpSelfAcc11.two_int.int1;             \
	ccSelfAcc13 += (mlib_d64)ccTmpSelfAcc12.two_int.int0;             \
	ccSelfAcc14 += (mlib_d64)ccTmpSelfAcc12.two_int.int1;

/* *********************************************************** */

#define	IMG2_SELF_MUL_U16_S32                                         \
	dr1 = vis_fmuld8sux16(vis_read_hi(dy), vis_read_hi(dy));          \
	dr2 = vis_fmuld8ulx16(vis_read_hi(dy), vis_read_hi(dy));          \
	dr3 = vis_fmuld8sux16(vis_read_lo(dy), vis_read_lo(dy));          \
	dr4 = vis_fmuld8ulx16(vis_read_lo(dy), vis_read_lo(dy));          \
	dr1 = vis_fpadd32(dr1, mone);                                     \
	dr3 = vis_fpadd32(dr3, mone);                                     \
	ccTmpSelfAcc21.db = vis_fpadd32(dr1, dr2);                        \
	ccTmpSelfAcc22.db = vis_fpadd32(dr3, dr4);                        \
	dr5 = vis_fmuld8sux16(vis_read_hi(dy2), vis_read_hi(dy2));        \
	dr6 = vis_fmuld8ulx16(vis_read_hi(dy2), vis_read_hi(dy2));        \
	dr7 = vis_fmuld8sux16(vis_read_lo(dy2), vis_read_lo(dy2));        \
	dr8 = vis_fmuld8ulx16(vis_read_lo(dy2), vis_read_lo(dy2));        \
	ccTmpSelfAcc21.db = vis_fpadd32(ccTmpSelfAcc21.db, dr5);          \
	ccTmpSelfAcc21.db = vis_fpadd32(ccTmpSelfAcc21.db, dr6);          \
	ccTmpSelfAcc22.db = vis_fpadd32(ccTmpSelfAcc22.db, dr7);          \
	ccTmpSelfAcc22.db = vis_fpadd32(ccTmpSelfAcc22.db, dr8);          \
	ccSelfAcc21 += (mlib_d64)ccTmpSelfAcc21.two_int.int0;             \
	ccSelfAcc22 += (mlib_d64)ccTmpSelfAcc21.two_int.int1;             \
	ccSelfAcc23 += (mlib_d64)ccTmpSelfAcc22.two_int.int0;             \
	ccSelfAcc24 += (mlib_d64)ccTmpSelfAcc22.two_int.int1;

/* *********************************************************** */

#define	IMG1_SUM_U16_D64                                              \
	tmpAcc11 =                                                        \
		vis_fpadd32(tmpAcc11,                                         \
		vis_fmuld8ulx16(scale, vis_read_hi(dx)));                     \
	tmpAcc12 = vis_fpadd32(tmpAcc12,                                  \
		vis_fmuld8ulx16(scale, vis_read_lo(dx)));                     \
	tmpAcc11 = vis_fpadd32(tmpAcc11,                                  \
		vis_fmuld8ulx16(scale, vis_read_hi(dx1)));                    \
	tmpAcc12 = vis_fpadd32(tmpAcc12,                                  \
		vis_fmuld8ulx16(scale, vis_read_lo(dx1)));

/* *********************************************************** */
#define	IMG2_SUM_U16_D64                                              \
	tmpAcc21 = vis_fpadd32(tmpAcc21,                                  \
		vis_fmuld8ulx16(scale, vis_read_hi(dy)));                     \
	tmpAcc22 = vis_fpadd32(tmpAcc22,                                  \
		vis_fmuld8ulx16(scale, vis_read_lo(dy)));                     \
	tmpAcc21 = vis_fpadd32(tmpAcc21,                                  \
		vis_fmuld8ulx16(scale, vis_read_hi(dy2)));                    \
	tmpAcc22 = vis_fpadd32(tmpAcc22,                                  \
		vis_fmuld8ulx16(scale, vis_read_lo(dy2)));

/* *********************************************************** */

void
mlib_v_ImageNormCrossCorrel_U16_124(
    const mlib_image *img1,
    const mlib_image *img2,
	const mlib_d64 *m2,
	const mlib_d64 *s2,
    mlib_d64 *rez)
{
/* pointer to the data of first image */
	mlib_s16 *psrc1 = (mlib_s16 *)mlib_ImageGetData(img1);

/* pointer to the data of second image */
	mlib_s16 *psrc2 = (mlib_s16 *)mlib_ImageGetData(img2);

/* height of image */
	mlib_s32 height = mlib_ImageGetHeight(img1);

/* elements to next row */
	mlib_s32 src1_stride = mlib_ImageGetStride(img1) >> 1;

/* elements to next row */
	mlib_s32 src2_stride = mlib_ImageGetStride(img2) >> 1;
	mlib_s32 nchan = mlib_ImageGetChannels(img1);

/* number of elements in the row */
	mlib_s32 size_row = mlib_ImageGetWidth(img1) << (nchan >> 1);
	mlib_s32 size, num_row;

/* indices */
	mlib_s32 i, j, k;

/* edge masks */
	mlib_s32 emask;

	mlib_d64 ccInterAcc1, ccInterAcc2, ccInterAcc3, ccInterAcc4;
	mlib_d64 ccSelfAcc11, ccSelfAcc12, ccSelfAcc13, ccSelfAcc14;
	mlib_d64 ccSelfAcc21, ccSelfAcc22, ccSelfAcc23, ccSelfAcc24;
	mlib_d64 sum11, sum12, sum13, sum14;
	mlib_d64 sum21, sum22, sum23, sum24;
	mlib_d64 tmpAcc11, tmpAcc12, tmpAcc21, tmpAcc22;

	mlib_f32 scale = vis_to_float(0x10001);
	type_union_mlib_d64 ccTmpInterAcc1, ccTmpInterAcc2,
		ccTmpSelfAcc11, ccTmpSelfAcc12,
		ccTmpSelfAcc21, ccTmpSelfAcc22, accumulators[2];
	mlib_d64 offset = vis_to_double_dup(0x80008000);
	const mlib_d64 dzero = vis_fzero();

/* offset of address alignment src1 */
	mlib_s32 off;
	mlib_d64 midd[2], *dpsrc1, *dpsrc2;
	mlib_d64 dx, dx1, dy, dy0, dy1, dy2, dr1, dr2, dr3, dr4;
	mlib_d64 dr5, dr6, dr7, dr8;
	mlib_d64 mone = vis_fone();
	mlib_s32 compens;

	if (src1_stride == size_row && src1_stride == src2_stride) {
/* This image is not a sub-image and can be treated as a 1-D vector */
		size_row *= height;
		height = 1;
	}

	if (m2 != NULL && s2 != NULL) {
		for (i = 0; i < height; i++) {

			ccInterAcc1 = ccInterAcc2 = dzero;
			ccInterAcc3 = ccInterAcc4 = dzero;
			ccSelfAcc11 = ccSelfAcc12 = dzero;
			ccSelfAcc13 = ccSelfAcc14 = dzero;
			sum11 = sum12 = sum13 = sum14 = dzero;
			tmpAcc11 = tmpAcc12 = dzero;

			ccTmpInterAcc1.db = ccTmpInterAcc2.db = dzero;
			ccTmpSelfAcc11.db = ccTmpSelfAcc12.db = dzero;
			accumulators[0].db = accumulators[1].db = dzero;
			midd[0] = midd[1] = dzero;
	/* prepare the source address */
			off = (mlib_addr)psrc1 & 7;
			off >>= 1;
			emask = vis_edge16(psrc1, psrc1 + size_row - 1);
			num_row = size_row + off;
			dpsrc1 = (mlib_d64 *)(psrc1 - off);
			dpsrc2 = (mlib_d64 *)vis_alignaddr((psrc2 - off), 0);

			dx = vis_fxor((dpsrc1[0]), offset);
			vis_pst_16(dx, (void *)midd, emask);
			dx = midd[0];
			dy0 = vis_ld_d64_nf(dpsrc2);
			dy1 = vis_ld_d64_nf(dpsrc2 + 1);
			dy = vis_faligndata(dy0, dy1);
			dy = vis_fxor((dy), offset);
			midd[0] = dzero;
			vis_pst_16(dy, (void *)midd, emask);
			dy = midd[0];

			dx1 = vis_ld_d64_nf(dpsrc1 + 1);
			dy0 = vis_ld_d64_nf(dpsrc2 + 2);
			dy2 = vis_faligndata(dy1, dy0);
			dx1 = vis_fxor((dx1), offset);
			dy2 = vis_fxor((dy2), offset);

			size = 0;
			compens = (num_row >> 3);

			while (num_row >= 8) {
				size = num_row & ~7;
				size = size > 131072 ? 131072 : size;

				if ((((mlib_addr)psrc1 ^ (mlib_addr)psrc2)
					& 7) == 0) {
#pragma pipeloop(0)
					for (j = 0; j <= (size - 8); j += 8) {
						INTER_MUL_U16_S32
						IMG1_SELF_MUL_U16_S32
						IMG1_SUM_U16_D64

						dpsrc1 += 2;
						dpsrc2 += 2;

						dx = vis_ld_d64_nf(dpsrc1);
						dy = vis_ld_d64_nf(dpsrc2);
						dx1 = dpsrc1[1];
						dy2 = dpsrc2[1];

						dx = vis_fxor((dx), offset);
						dy = vis_fxor((dy), offset);
						dx1 = vis_fxor((dx1), offset);
						dy2 = vis_fxor((dy2), offset);
					}
				} else {
#pragma pipeloop(0)
					for (j = 0; j <= (size - 8); j += 8) {
						INTER_MUL_U16_S32
						IMG1_SELF_MUL_U16_S32
						IMG1_SUM_U16_D64

						dpsrc1 += 2;
						dpsrc2 += 2;

						dx = vis_ld_d64_nf(dpsrc1);
						dy1 = vis_ld_d64_nf(dpsrc2 + 1);
						dy = vis_faligndata(dy0, dy1);

						dx1 = vis_ld_d64_nf(dpsrc1 + 1);
						dy0 = vis_ld_d64_nf(dpsrc2 + 2);
						dy2 = vis_faligndata(dy1, dy0);

						dx = vis_fxor((dx), offset);
						dy = vis_fxor((dy), offset);
						dx1 = vis_fxor((dx1), offset);
						dy2 = vis_fxor((dy2), offset);
					}

				}

				num_row -= size;

				accumulators[0].db = tmpAcc11;
				accumulators[1].db = tmpAcc12;

				sum11 += accumulators[0].two_int.int0;
				sum12 += accumulators[0].two_int.int1;
				sum13 += accumulators[1].two_int.int0;
				sum14 += accumulators[1].two_int.int1;

				rez[3 & (0 - off)] += ccInterAcc1;
				rez[(3 & (0 - off)) + 4] += ccSelfAcc11;
				rez[(3 & (0 - off)) + 12] += sum11;

				rez[3 & (1 - off)] += ccInterAcc2;
				rez[(3 & (1 - off)) + 4] += ccSelfAcc12;
				rez[(3 & (1 - off)) + 12] += sum12;

				rez[3 & (2 - off)] += ccInterAcc3;
				rez[(3 & (2 - off)) + 4] += ccSelfAcc13;
				rez[(3 & (2 - off)) + 12] += sum13;

				rez[3 & (3 - off)] += ccInterAcc4;
				rez[(3 & (3 - off)) + 4] += ccSelfAcc14;
				rez[(3 & (3 - off)) + 12] += sum14;

				ccInterAcc1 = ccInterAcc2 = dzero;
				ccInterAcc3 = ccInterAcc4 = dzero;
				ccSelfAcc11 = ccSelfAcc12 = dzero;
				ccSelfAcc13 = ccSelfAcc14 = dzero;
				sum11 = sum12 = sum13 = sum14 = dzero;
				tmpAcc11 = tmpAcc12 = dzero;
			}

			if (num_row > 0) {
	/* prepare edge mask for the last bytes */
				emask = 0xff00 >> num_row;
				midd[0] = midd[1] = dzero;
				vis_pst_16(dx, (void *)midd, emask >> 4);
				dx = midd[0];
				vis_pst_16(dx1, (void *)(midd + 1), emask);
				dx1 = midd[1];

				midd[0] = midd[1] = dzero;
				vis_pst_16(dy, (void *)midd, emask >> 4);
				dy = midd[0];
				vis_pst_16(dy2, (void *)(midd + 1), emask);
				dy2 = midd[1];

				INTER_MUL_U16_S32
				IMG1_SELF_MUL_U16_S32
				IMG1_SUM_U16_D64

				compens += 1;

				accumulators[0].db = tmpAcc11;
				accumulators[1].db = tmpAcc12;

				sum11 += accumulators[0].two_int.int0;
				sum12 += accumulators[0].two_int.int1;
				sum13 += accumulators[1].two_int.int0;
				sum14 += accumulators[1].two_int.int1;

				rez[3 & (0 - off)] += ccInterAcc1;
				rez[(3 & (0 - off)) + 4] += ccSelfAcc11;
				rez[(3 & (0 - off)) + 12] += sum11;

				rez[3 & (1 - off)] += ccInterAcc2;
				rez[(3 & (1 - off)) + 4] += ccSelfAcc12;
				rez[(3 & (1 - off)) + 12] += sum12;

				rez[3 & (2 - off)] += ccInterAcc3;
				rez[(3 & (2 - off)) + 4] += ccSelfAcc13;
				rez[(3 & (2 - off)) + 12] += sum13;

				rez[3 & (3 - off)] += ccInterAcc4;
				rez[(3 & (3 - off)) + 4] += ccSelfAcc14;
				rez[(3 & (3 - off)) + 12] += sum14;

			}
			rez[3 & (0 - off)] += (mlib_d64)compens;
			rez[(3 & (0 - off)) + 4] += (mlib_d64)compens;

			rez[3 & (1 - off)] += (mlib_d64)compens;
			rez[(3 & (1 - off)) + 4] += (mlib_d64)compens;

			rez[3 & (2 - off)] += (mlib_d64)compens;
			rez[(3 & (2 - off)) + 4] += (mlib_d64)compens;

			rez[3 & (3 - off)] += (mlib_d64)compens;
			rez[(3 & (3 - off)) + 4] += (mlib_d64)compens;

			psrc1 += src1_stride;
			psrc2 += src2_stride;
		}
	} else {
		for (i = 0; i < height; i++) {

			ccInterAcc1 = ccInterAcc2 = dzero;
			ccInterAcc3 = ccInterAcc4 = dzero;
			ccSelfAcc11 = ccSelfAcc12 = dzero;
			ccSelfAcc13 = ccSelfAcc14 = dzero;
			ccSelfAcc21 = ccSelfAcc22 = dzero;
			ccSelfAcc23 = ccSelfAcc24 = dzero;
			sum11 = sum12 = sum13 = sum14 = dzero;
			sum21 = sum22 = sum23 = sum24 = dzero;
			tmpAcc11 = tmpAcc12 = dzero;
			tmpAcc21 = tmpAcc22 = dzero;

			midd[0] = midd[1] = dzero;
			ccTmpInterAcc1.db = ccTmpInterAcc2.db = dzero;
			ccTmpSelfAcc11.db = ccTmpSelfAcc12.db = dzero;
			ccTmpSelfAcc21.db = ccTmpSelfAcc22.db = dzero;
			accumulators[0].db = accumulators[1].db = dzero;

	/* prepare the source address */
			off = (mlib_addr)psrc1 & 7;
			off >>= 1;
			emask = vis_edge16(psrc1, psrc1 + size_row - 1);
			num_row = size_row + off;
			dpsrc1 = (mlib_d64 *)(psrc1 - off);
			dpsrc2 = (mlib_d64 *)vis_alignaddr((psrc2 - off), 0);

			dx = vis_fxor((dpsrc1[0]), offset);
			vis_pst_16(dx, (void *)midd, emask);
			dx = midd[0];
			dy0 = vis_ld_d64_nf(dpsrc2);
			dy1 = vis_ld_d64_nf(dpsrc2 + 1);
			dy = vis_faligndata(dy0, dy1);
			dy = vis_fxor((dy), offset);
			midd[0] = dzero;
			vis_pst_16(dy, (void *)midd, emask);
			dy = midd[0];

			dx1 = vis_ld_d64_nf(dpsrc1 + 1);
			dy0 = vis_ld_d64_nf(dpsrc2 + 2);
			dy2 = vis_faligndata(dy1, dy0);
			dx1 = vis_fxor((dx1), offset);
			dy2 = vis_fxor((dy2), offset);

			compens = (num_row >> 3);

			while (num_row >= 8) {
				size = num_row & ~7;
				size = size > 131072 ? 131072 : size;
				if ((((mlib_addr)psrc1 ^ (mlib_addr)psrc2)
					& 7) == 0) {
#pragma pipeloop(0)
					for (j = 0; j <= (size - 8); j += 8) {
						INTER_MUL_U16_S32
						IMG1_SELF_MUL_U16_S32
						IMG2_SELF_MUL_U16_S32
						IMG1_SUM_U16_D64
						IMG2_SUM_U16_D64

						dpsrc1 += 2;
						dpsrc2 += 2;

						dx = vis_ld_d64_nf(dpsrc1);
						dy = vis_ld_d64_nf(dpsrc2);
						dx1 = dpsrc1[1];
						dy2 = dpsrc2[1];

						dx = vis_fxor((dx), offset);
						dy = vis_fxor((dy), offset);
						dx1 = vis_fxor((dx1), offset);
						dy2 = vis_fxor((dy2), offset);
					}
				} else {
#pragma pipeloop(0)
					for (j = 0; j <= (size - 8); j += 8) {
						INTER_MUL_U16_S32
						IMG1_SELF_MUL_U16_S32
						IMG2_SELF_MUL_U16_S32
						IMG1_SUM_U16_D64
						IMG2_SUM_U16_D64

						dpsrc1 += 2;
						dpsrc2 += 2;
						dx = vis_ld_d64_nf(dpsrc1);
						dy1 = vis_ld_d64_nf(dpsrc2 + 1);
						dy = vis_faligndata(dy0, dy1);

						dx1 = vis_ld_d64_nf(dpsrc1 + 1);
						dy0 = vis_ld_d64_nf(dpsrc2 + 2);
						dy2 = vis_faligndata(dy1, dy0);

						dx = vis_fxor((dx), offset);
						dy = vis_fxor((dy), offset);
						dx1 = vis_fxor((dx1), offset);
						dy2 = vis_fxor((dy2), offset);
					}

				}

				num_row -= size;
				accumulators[0].db = tmpAcc11;
				accumulators[1].db = tmpAcc12;

				sum11 += accumulators[0].two_int.int0;
				sum12 += accumulators[0].two_int.int1;
				sum13 += accumulators[1].two_int.int0;
				sum14 += accumulators[1].two_int.int1;

				accumulators[0].db = tmpAcc21;
				accumulators[1].db = tmpAcc22;

				sum21 += accumulators[0].two_int.int0;
				sum22 += accumulators[0].two_int.int1;
				sum23 += accumulators[1].two_int.int0;
				sum24 += accumulators[1].two_int.int1;

				rez[3 & (0 - off)] += ccInterAcc1;
				rez[(3 & (0 - off)) + 4] += ccSelfAcc11;
				rez[(3 & (0 - off)) + 8] += ccSelfAcc21;
				rez[(3 & (0 - off)) + 12] += sum11;
				rez[(3 & (0 - off)) + 16] += sum21;

				rez[3 & (1 - off)] += ccInterAcc2;
				rez[(3 & (1 - off)) + 4] += ccSelfAcc12;
				rez[(3 & (1 - off)) + 8] += ccSelfAcc22;
				rez[(3 & (1 - off)) + 12] += sum12;
				rez[(3 & (1 - off)) + 16] += sum22;

				rez[3 & (2 - off)] += ccInterAcc3;
				rez[(3 & (2 - off)) + 4] += ccSelfAcc13;
				rez[(3 & (2 - off)) + 8] += ccSelfAcc23;
				rez[(3 & (2 - off)) + 12] += sum13;
				rez[(3 & (2 - off)) + 16] += sum23;

				rez[3 & (3 - off)] += ccInterAcc4;
				rez[(3 & (3 - off)) + 4] += ccSelfAcc14;
				rez[(3 & (3 - off)) + 8] += ccSelfAcc24;
				rez[(3 & (3 - off)) + 12] += sum14;
				rez[(3 & (3 - off)) + 16] += sum24;

				ccInterAcc1 = ccInterAcc2 = dzero;
				ccInterAcc3 = ccInterAcc4 = dzero;
				ccSelfAcc11 = ccSelfAcc12 = dzero;
				ccSelfAcc13 = ccSelfAcc14 = dzero;
				ccSelfAcc21 = ccSelfAcc22 = dzero;
				ccSelfAcc23 = ccSelfAcc24 = dzero;
				sum11 = sum12 = sum13 = sum14 = dzero;
				sum21 = sum22 = sum23 = sum24 = dzero;
				tmpAcc11 = tmpAcc12 = dzero;
				tmpAcc21 = tmpAcc22 = dzero;
			}


			if (num_row > 0) {

				midd[0] = midd[1] = dzero;
	/* prepare edge mask for the last bytes */
				emask = 0xff00 >> num_row;

				midd[0] = midd[1] = dzero;
				vis_pst_16(dx, &midd[0], emask >> 4);
				dx = midd[0];
				vis_pst_16(dx1, &midd[1], emask);
				dx1 = midd[1];

				midd[0] = midd[1] = dzero;
				vis_pst_16(dy, &midd[0], emask >> 4);
				dy = midd[0];
				vis_pst_16(dy2, &midd[1], emask);
				dy2 = midd[1];

				INTER_MUL_U16_S32
				IMG1_SELF_MUL_U16_S32
				IMG2_SELF_MUL_U16_S32
				IMG1_SUM_U16_D64
				IMG2_SUM_U16_D64

				compens += 1;

				accumulators[0].db = tmpAcc11;
				accumulators[1].db = tmpAcc12;

				sum11 += accumulators[0].two_int.int0;
				sum12 += accumulators[0].two_int.int1;
				sum13 += accumulators[1].two_int.int0;
				sum14 += accumulators[1].two_int.int1;

				accumulators[0].db = tmpAcc21;
				accumulators[1].db = tmpAcc22;

				sum21 += accumulators[0].two_int.int0;
				sum22 += accumulators[0].two_int.int1;
				sum23 += accumulators[1].two_int.int0;
				sum24 += accumulators[1].two_int.int1;

				rez[3 & (0 - off)] += ccInterAcc1;
				rez[(3 & (0 - off)) + 4] += ccSelfAcc11;
				rez[(3 & (0 - off)) + 8] += ccSelfAcc21;
				rez[(3 & (0 - off)) + 12] += sum11;
				rez[(3 & (0 - off)) + 16] += sum21;

				rez[3 & (1 - off)] += ccInterAcc2;
				rez[(3 & (1 - off)) + 4] += ccSelfAcc12;
				rez[(3 & (1 - off)) + 8] += ccSelfAcc22;
				rez[(3 & (1 - off)) + 12] += sum12;
				rez[(3 & (1 - off)) + 16] += sum22;

				rez[3 & (2 - off)] += ccInterAcc3;
				rez[(3 & (2 - off)) + 4] += ccSelfAcc13;
				rez[(3 & (2 - off)) + 8] += ccSelfAcc23;
				rez[(3 & (2 - off)) + 12] += sum13;
				rez[(3 & (2 - off)) + 16] += sum23;

				rez[3 & (3 - off)] += ccInterAcc4;
				rez[(3 & (3 - off)) + 4] += ccSelfAcc14;
				rez[(3 & (3 - off)) + 8] += ccSelfAcc24;
				rez[(3 & (3 - off)) + 12] += sum14;
				rez[(3 & (3 - off)) + 16] += sum24;

			}
			rez[3 & (0 - off)] += (mlib_d64)compens;
			rez[(3 & (0 - off)) + 4] += (mlib_d64)compens;
			rez[(3 & (0 - off)) + 8] += (mlib_d64)compens;

			rez[3 & (1 - off)] += (mlib_d64)compens;
			rez[(3 & (1 - off)) + 4] += (mlib_d64)compens;
			rez[(3 & (1 - off)) + 8] += (mlib_d64)compens;

			rez[3 & (2 - off)] += (mlib_d64)compens;
			rez[(3 & (2 - off)) + 4] += (mlib_d64)compens;
			rez[(3 & (2 - off)) + 8] += (mlib_d64)compens;

			rez[3 & (3 - off)] += (mlib_d64)compens;
			rez[(3 & (3 - off)) + 4] += (mlib_d64)compens;
			rez[(3 & (3 - off)) + 8] += (mlib_d64)compens;

			psrc1 += src1_stride;
			psrc2 += src2_stride;
		}
	}


}

/* *********************************************************** */

#define	INTER_MUL_U16_S32_3                                               \
	dr1 = vis_fmuld8sux16(vis_read_hi(dx), vis_read_hi(dy));              \
	dr2 = vis_fmuld8ulx16(vis_read_hi(dx), vis_read_hi(dy));              \
	dr3 = vis_fmuld8sux16(vis_read_lo(dx), vis_read_lo(dy));              \
	dr4 = vis_fmuld8ulx16(vis_read_lo(dx), vis_read_lo(dy));              \
	dr1 = vis_fpadd32(dr1, mone);                                         \
	dr3 = vis_fpadd32(dr3, mone);                                         \
	dc1.db = vis_fpadd32(dr1, dr2);                                       \
	dc2.db = vis_fpadd32(dr3, dr4);                                       \
	dr1 = vis_fmuld8sux16(vis_read_hi(dx2), vis_read_hi(dy1));            \
	dr2 = vis_fmuld8ulx16(vis_read_hi(dx2), vis_read_hi(dy1));            \
	dr3 = vis_fmuld8sux16(vis_read_lo(dx2), vis_read_lo(dy1));            \
	dr4 = vis_fmuld8ulx16(vis_read_lo(dx2), vis_read_lo(dy1));            \
	dr1 = vis_fpadd32(dr1, mone);                                         \
	dc3.db = vis_fpadd32(dr1, dr2);                                       \
	dc1.db = vis_fpadd32(dc1.db, dr3);                                    \
	dc1.db = vis_fpadd32(dc1.db, dr4);                                    \
	ccInterAcc1 += (mlib_d64)dc1.two_int.int0;                            \
	ccInterAcc2 += (mlib_d64)dc1.two_int.int1;                            \
	dr1 = vis_fmuld8sux16(vis_read_hi(dx3), vis_read_hi(dy2));            \
	dr2 = vis_fmuld8ulx16(vis_read_hi(dx3), vis_read_hi(dy2));            \
	dr3 = vis_fmuld8sux16(vis_read_lo(dx3), vis_read_lo(dy2));            \
	dr4 = vis_fmuld8ulx16(vis_read_lo(dx3), vis_read_lo(dy2));            \
	dc2.db = vis_fpadd32(dc2.db, dr1);                                    \
	dc2.db = vis_fpadd32(dc2.db, dr2);                                    \
	dc3.db = vis_fpadd32(dc3.db, dr3);                                    \
	dc3.db = vis_fpadd32(dc3.db, dr4);                                    \
	ccInterAcc3 += (mlib_d64)dc2.two_int.int0;                            \
	ccInterAcc1 += (mlib_d64)dc2.two_int.int1;                            \
	ccInterAcc2 += (mlib_d64)dc3.two_int.int0;                            \
	ccInterAcc3 += (mlib_d64)dc3.two_int.int1;

/* *********************************************************** */

#define	IMG1_SELF_MUL_U16_S32_3                                           \
	dr1 = vis_fmuld8sux16(vis_read_hi(dx), vis_read_hi(dx));              \
	dr2 = vis_fmuld8ulx16(vis_read_hi(dx), vis_read_hi(dx));              \
	dr3 = vis_fmuld8sux16(vis_read_lo(dx), vis_read_lo(dx));              \
	dr4 = vis_fmuld8ulx16(vis_read_lo(dx), vis_read_lo(dx));              \
	dr1 = vis_fpadd32(dr1, mone);                                         \
	dr3 = vis_fpadd32(dr3, mone);                                         \
	dc1.db = vis_fpadd32(dr1, dr2);                                       \
	dc2.db = vis_fpadd32(dr3, dr4);                                       \
	dr1 = vis_fmuld8sux16(vis_read_hi(dx2), vis_read_hi(dx2));            \
	dr2 = vis_fmuld8ulx16(vis_read_hi(dx2), vis_read_hi(dx2));            \
	dr3 = vis_fmuld8sux16(vis_read_lo(dx2), vis_read_lo(dx2));            \
	dr4 = vis_fmuld8ulx16(vis_read_lo(dx2), vis_read_lo(dx2));            \
	dr1 = vis_fpadd32(dr1, mone);                                         \
	dc3.db = vis_fpadd32(dr1, dr2);                                       \
	dc1.db = vis_fpadd32(dc1.db, dr3);                                    \
	dc1.db = vis_fpadd32(dc1.db, dr4);                                    \
	ccSelfAcc11 += (mlib_d64)dc1.two_int.int0;                            \
	ccSelfAcc12 += (mlib_d64)dc1.two_int.int1;                            \
	dr1 = vis_fmuld8sux16(vis_read_hi(dx3), vis_read_hi(dx3));            \
	dr2 = vis_fmuld8ulx16(vis_read_hi(dx3), vis_read_hi(dx3));            \
	dr3 = vis_fmuld8sux16(vis_read_lo(dx3), vis_read_lo(dx3));            \
	dr4 = vis_fmuld8ulx16(vis_read_lo(dx3), vis_read_lo(dx3));            \
	dc2.db = vis_fpadd32(dc2.db, dr1);                                    \
	dc2.db = vis_fpadd32(dc2.db, dr2);                                    \
	dc3.db = vis_fpadd32(dc3.db, dr3);                                    \
	dc3.db = vis_fpadd32(dc3.db, dr4);                                    \
	ccSelfAcc13 += (mlib_d64)dc2.two_int.int0;                            \
	ccSelfAcc11 += (mlib_d64)dc2.two_int.int1;                            \
	ccSelfAcc12 += (mlib_d64)dc3.two_int.int0;                            \
	ccSelfAcc13 += (mlib_d64)dc3.two_int.int1;

/* *********************************************************** */

#define	IMG2_SELF_MUL_U16_S32_3                                           \
	dr1 = vis_fmuld8sux16(vis_read_hi(dy), vis_read_hi(dy));              \
	dr2 = vis_fmuld8ulx16(vis_read_hi(dy), vis_read_hi(dy));              \
	dr3 = vis_fmuld8sux16(vis_read_lo(dy), vis_read_lo(dy));              \
	dr4 = vis_fmuld8ulx16(vis_read_lo(dy), vis_read_lo(dy));              \
	dr1 = vis_fpadd32(dr1, mone);                                         \
	dr3 = vis_fpadd32(dr3, mone);                                         \
	dc1.db = vis_fpadd32(dr1, dr2);                                       \
	dc2.db = vis_fpadd32(dr3, dr4);                                       \
	dr1 = vis_fmuld8sux16(vis_read_hi(dy1), vis_read_hi(dy1));            \
	dr2 = vis_fmuld8ulx16(vis_read_hi(dy1), vis_read_hi(dy1));            \
	dr3 = vis_fmuld8sux16(vis_read_lo(dy1), vis_read_lo(dy1));            \
	dr4 = vis_fmuld8ulx16(vis_read_lo(dy1), vis_read_lo(dy1));            \
	dr1 = vis_fpadd32(dr1, mone);                                         \
	dc3.db = vis_fpadd32(dr1, dr2);                                       \
	dc1.db = vis_fpadd32(dc1.db, dr3);                                    \
	dc1.db = vis_fpadd32(dc1.db, dr4);                                    \
	ccSelfAcc21 += (mlib_d64)dc1.two_int.int0;                            \
	ccSelfAcc22 += (mlib_d64)dc1.two_int.int1;                            \
	dr1 = vis_fmuld8sux16(vis_read_hi(dy2), vis_read_hi(dy2));            \
	dr2 = vis_fmuld8ulx16(vis_read_hi(dy2), vis_read_hi(dy2));            \
	dr3 = vis_fmuld8sux16(vis_read_lo(dy2), vis_read_lo(dy2));            \
	dr4 = vis_fmuld8ulx16(vis_read_lo(dy2), vis_read_lo(dy2));            \
	dc2.db = vis_fpadd32(dc2.db, dr1);                                    \
	dc2.db = vis_fpadd32(dc2.db, dr2);                                    \
	dc3.db = vis_fpadd32(dc3.db, dr3);                                    \
	dc3.db = vis_fpadd32(dc3.db, dr4);                                    \
	ccSelfAcc23 += (mlib_d64)dc2.two_int.int0;                            \
	ccSelfAcc21 += (mlib_d64)dc2.two_int.int1;                            \
	ccSelfAcc22 += (mlib_d64)dc3.two_int.int0;                            \
	ccSelfAcc23 += (mlib_d64)dc3.two_int.int1;

/* *********************************************************** */

#define	IMG1_SUM_S32_U16_D64                                        \
	tmpAcc11 =                                                      \
		vis_fpadd32(tmpAcc11,                                       \
		vis_fmuld8ulx16(scale, vis_read_hi(dx)));                   \
	tmpAcc11 =                                                      \
		vis_fpadd32(tmpAcc11,                                       \
		vis_fmuld8ulx16(scale, vis_read_lo(dx2)));                  \
	tmpAcc12 =                                                      \
		vis_fpadd32(tmpAcc12,                                       \
		vis_fmuld8ulx16(scale, vis_read_lo(dx)));                   \
	tmpAcc12 =                                                      \
		vis_fpadd32(tmpAcc12,                                       \
		vis_fmuld8ulx16(scale, vis_read_hi(dx3)));                  \
	tmpAcc13 =                                                      \
		vis_fpadd32(tmpAcc13,                                       \
		vis_fmuld8ulx16(scale, vis_read_hi(dx2)));                  \
	tmpAcc13 =                                                      \
		vis_fpadd32(tmpAcc13,                                       \
		vis_fmuld8ulx16(scale, vis_read_lo(dx3)));

/* *********************************************************** */

#define	IMG2_SUM_S32_U16_D64                                        \
	tmpAcc21 =                                                      \
		vis_fpadd32(tmpAcc21,                                       \
		vis_fmuld8ulx16(scale, vis_read_hi(dy)));                   \
	tmpAcc21 =                                                      \
		vis_fpadd32(tmpAcc21,                                       \
		vis_fmuld8ulx16(scale, vis_read_lo(dy1)));                  \
	tmpAcc22 =                                                      \
		vis_fpadd32(tmpAcc22,                                       \
		vis_fmuld8ulx16(scale, vis_read_lo(dy)));                   \
	tmpAcc22 =                                                      \
		vis_fpadd32(tmpAcc22,                                       \
		vis_fmuld8ulx16(scale, vis_read_hi(dy2)));                  \
	tmpAcc23 =                                                      \
		vis_fpadd32(tmpAcc23,                                       \
		vis_fmuld8ulx16(scale, vis_read_hi(dy1)));                  \
	tmpAcc23 =                                                      \
		vis_fpadd32(tmpAcc23,                                       \
		vis_fmuld8ulx16(scale, vis_read_lo(dy2)));

/* *********************************************************** */


void
mlib_v_ImageNormCrossCorrel_U16_3(
    const mlib_image *img1,
    const mlib_image *img2,
	const mlib_d64 *m2,
	const mlib_d64 *s2,
    mlib_d64 *rez)
{
/* pointer to the data of first image */
	mlib_s16 *psrc1 = (mlib_s16 *)mlib_ImageGetData(img1);

/* pointer to the data of second image */
	mlib_s16 *psrc2 = (mlib_s16 *)mlib_ImageGetData(img2);

/* height of image */
	mlib_s32 height = mlib_ImageGetHeight(img1);

/* elements to next row */
	mlib_s32 src1_stride = mlib_ImageGetStride(img1) >> 1;

/* elements to next row */
	mlib_s32 src2_stride = mlib_ImageGetStride(img2) >> 1;

/* number of elements in the row */
	mlib_s32 size_row = mlib_ImageGetWidth(img1) * 3;

/* indices */
	mlib_s32 i, j, k;

/* offset of address alignment in destination */
	mlib_s32 off;
	mlib_d64 midd[3], *dpsrc1, *dpsrc2;
	mlib_d64 dx, dy, dx2, dx3, dy1, dy2, dy3, dr1, dr2, dr3, dr4;
	type_union_mlib_d64 dc1, dc2, dc3;
	mlib_s32 num_row, size;

	mlib_d64 ccInterAcc1, ccInterAcc2, ccInterAcc3;
	mlib_d64 ccSelfAcc11, ccSelfAcc12, ccSelfAcc13;
	mlib_d64 ccSelfAcc21, ccSelfAcc22, ccSelfAcc23;
	mlib_d64 sum11, sum12, sum13;
	mlib_d64 sum21, sum22, sum23;
	mlib_d64 tmpAcc11, tmpAcc12, tmpAcc13;
	mlib_d64 tmpAcc21, tmpAcc22, tmpAcc23;

	mlib_f32 scale = vis_to_float(0x10001);
	type_union_mlib_d64 accumulator;
	const mlib_d64 dzero = vis_fzero();
	mlib_d64 offset = vis_to_double_dup(0x80008000);

/* edge masks */
	mlib_s32 emask;

/* compute mean3_off(align, sumn) */
	const mlib_u8 al2off[12] = { 0, 2, 1, 0, 2, 1, 0, 2, 1, 0, 2, 1 };
	mlib_d64 mone = vis_fone();
	mlib_s32 compens;

	if (src1_stride == size_row && src1_stride == src2_stride) {
/* This image is not a sub-image and can be treated as a 1-D vector */
		size_row *= height;
		height = 1;
	}


	if (m2 != NULL && s2 != NULL) {
		for (i = 0; i < height; i++) {
			midd[0] = midd[1] = midd[2] = dzero;

			ccInterAcc1 = ccInterAcc2 = ccInterAcc3 = dzero;
			ccSelfAcc11 = ccSelfAcc12 = ccSelfAcc13 = dzero;
			sum11 = sum12 = sum13 = dzero;
			tmpAcc11 = tmpAcc12 = tmpAcc13 = dzero;

	/* prepare the source address */
			off = (mlib_addr)psrc1 & 7;
			off >>= 1;
			emask = vis_edge16(psrc1, psrc1 + size_row - 1);
			num_row = size_row + off;
			dpsrc1 = (mlib_d64 *)(psrc1 - off);
			dpsrc2 = (mlib_d64 *)vis_alignaddr((psrc2 - off), 0);

			dx = vis_fxor((dpsrc1[0]), offset);
			vis_pst_16(dx, &midd[0], emask);
			dx = midd[0];
			midd[0] = dzero;


			dy = vis_ld_d64_nf(dpsrc2);
			dy1 = vis_ld_d64_nf(dpsrc2 + 1);
			dy = vis_faligndata(dy, dy1);
			dy = vis_fxor((dy), offset);
			vis_pst_16(dy, &midd[0], emask);
			dy = midd[0];

			dx2 = vis_ld_d64_nf(dpsrc1 + 1);
			dy2 = vis_ld_d64_nf(dpsrc2 + 2);
			dy1 = vis_faligndata(dy1, dy2);
			dx3 = vis_ld_d64_nf(dpsrc1 + 2);
			dy3 = vis_ld_d64_nf(dpsrc2 + 3);
			dy2 = vis_faligndata(dy2, dy3);

			dx2 = vis_fxor((dx2), offset);
			dy1 = vis_fxor((dy1), offset);
			dx3 = vis_fxor((dx3), offset);
			dy2 = vis_fxor((dy2), offset);

			compens = 0;

			while (num_row >= 12) {
				size = num_row > 196608 ? 196608 : num_row;
				if ((((mlib_addr)psrc1 ^ (mlib_addr)psrc2)
					& 7) == 0) {
					for (j = 0; j <= (size - 12); j += 12) {

						INTER_MUL_U16_S32_3
						IMG1_SELF_MUL_U16_S32_3
						IMG1_SUM_S32_U16_D64
						dpsrc1 += 3;
						dpsrc2 += 3;
						dx = vis_ld_d64_nf(dpsrc1);
						dy = vis_ld_d64_nf(dpsrc2);

						dx2 = dpsrc1[1];
						dy1 = dpsrc2[1];
						dx3 = dpsrc1[2];
						dy2 = dpsrc2[2];

						dx = vis_fxor((dx), offset);
						dy = vis_fxor((dy), offset);
						dx2 = vis_fxor((dx2), offset);
						dy1 = vis_fxor((dy1), offset);
						dx3 = vis_fxor((dx3), offset);
						dy2 = vis_fxor((dy2), offset);

						compens += 2;
					}
				} else {
					for (j = 0; j <= (size - 12); j += 12) {
						INTER_MUL_U16_S32_3
						IMG1_SELF_MUL_U16_S32_3
						IMG1_SUM_S32_U16_D64

						dpsrc1 += 3;
						dpsrc2 += 3;
						dx = vis_ld_d64_nf(dpsrc1);
						dy1 = vis_ld_d64_nf(dpsrc2 + 1);
						dy = vis_faligndata(dy3, dy1);

						dx2 = vis_ld_d64_nf(dpsrc1 + 1);
						dy2 = vis_ld_d64_nf(dpsrc2 + 2);
						dy1 = vis_faligndata(dy1, dy2);
						dx3 = vis_ld_d64_nf(dpsrc1 + 2);
						dy3 = vis_ld_d64_nf(dpsrc2 + 3);
						dy2 = vis_faligndata(dy2, dy3);

						dx = vis_fxor((dx), offset);
						dy = vis_fxor((dy), offset);
						dx2 = vis_fxor((dx2), offset);
						dy1 = vis_fxor((dy1), offset);
						dx3 = vis_fxor((dx3), offset);
						dy2 = vis_fxor((dy2), offset);

						compens += 2;
					}
				}

				num_row -= j;

				accumulator.db = tmpAcc11;
				sum11 += accumulator.two_int.int0;
				sum12 += accumulator.two_int.int1;
				accumulator.db = tmpAcc12;
				sum13 += accumulator.two_int.int0;
				sum11 += accumulator.two_int.int1;
				accumulator.db = tmpAcc13;
				sum12 += accumulator.two_int.int0;
				sum13 += accumulator.two_int.int1;

				rez[al2off[off + 0]] += ccInterAcc1;
				rez[al2off[off + 0] + 4] += ccSelfAcc11;
				rez[al2off[off + 0] + 12] += sum11;

				rez[al2off[off + 2]] += ccInterAcc2;
				rez[al2off[off + 2] + 4] += ccSelfAcc12;
				rez[al2off[off + 2] + 12] += sum12;

				rez[al2off[off + 1]] += ccInterAcc3;
				rez[al2off[off + 1] + 4] += ccSelfAcc13;
				rez[al2off[off + 1] + 12] += sum13;

				ccInterAcc1 = ccInterAcc2 = ccInterAcc3 = dzero;
				ccSelfAcc11 = ccSelfAcc12 = ccSelfAcc13 = dzero;
				sum11 = sum12 = sum13 = dzero;
				tmpAcc11 = tmpAcc12 = tmpAcc13 = dzero;

			}

			if (num_row > 0) {
				midd[0] = midd[1] = midd[2] = dzero;
				emask = 0xfff000 >> num_row;

				vis_pst_16(dy, &midd[0], emask >> 8);
				dy = midd[0];
				vis_pst_16(dy1, &midd[1], emask >> 4);
				dy1 = midd[1];
				vis_pst_16(dy2, &midd[2], emask);
				dy2 = midd[2];

				midd[0] = midd[1] = midd[2] = dzero;
				vis_pst_16(dx, &midd[0], emask >> 8);
				dx = midd[0];
				vis_pst_16(dx2, &midd[1], emask >> 4);
				dx2 = midd[1];
				vis_pst_16(dx3, &midd[2], emask);
				dx3 = midd[2];

				INTER_MUL_U16_S32_3
				IMG1_SELF_MUL_U16_S32_3
				IMG1_SUM_S32_U16_D64
				compens += 2;

				accumulator.db = tmpAcc11;
				sum11 += accumulator.two_int.int0;
				sum12 += accumulator.two_int.int1;
				accumulator.db = tmpAcc12;
				sum13 += accumulator.two_int.int0;
				sum11 += accumulator.two_int.int1;
				accumulator.db = tmpAcc13;
				sum12 += accumulator.two_int.int0;
				sum13 += accumulator.two_int.int1;

				rez[al2off[off + 0]] += ccInterAcc1;
				rez[al2off[off + 0] + 4] += ccSelfAcc11;
				rez[al2off[off + 0] + 12] += sum11;

				rez[al2off[off + 2]] += ccInterAcc2;
				rez[al2off[off + 2] + 4] += ccSelfAcc12;
				rez[al2off[off + 2] + 12] += sum12;

				rez[al2off[off + 1]] += ccInterAcc3;
				rez[al2off[off + 1] + 4] += ccSelfAcc13;
				rez[al2off[off + 1] + 12] += sum13;

				tmpAcc11 = tmpAcc12 = tmpAcc13 = dzero;

			}

			rez[al2off[off + 0]] += compens;
			rez[al2off[off + 0] + 4] += compens;

			rez[al2off[off + 2]] += compens;
			rez[al2off[off + 2] + 4] += compens;

			rez[al2off[off + 1]] += compens;
			rez[al2off[off + 1] + 4] += compens;

			psrc1 += src1_stride;
			psrc2 += src2_stride;
		}
	} else {
		for (i = 0; i < height; i++) {
			midd[0] = midd[1] = midd[2] = dzero;

			ccInterAcc1 = ccInterAcc2 = ccInterAcc3 = dzero;
			ccSelfAcc11 = ccSelfAcc12 = ccSelfAcc13 = dzero;
			ccSelfAcc21 = ccSelfAcc22 = ccSelfAcc23 = dzero;
			sum11 = sum12 = sum13 = dzero;
			sum21 = sum22 = sum23 = dzero;
			tmpAcc11 = tmpAcc12 = tmpAcc13 = dzero;
			tmpAcc21 = tmpAcc22 = tmpAcc23 = dzero;

	/* prepare the source address */
			off = (mlib_addr)psrc1 & 7;
			off >>= 1;
			emask = vis_edge16(psrc1, psrc1 + size_row - 1);
			num_row = size_row + off;
			dpsrc1 = (mlib_d64 *)(psrc1 - off);
			dpsrc2 = (mlib_d64 *)vis_alignaddr((psrc2 - off), 0);

			dx = vis_fxor((dpsrc1[0]), offset);
			vis_pst_16(dx, &midd[0], emask);
			dx = midd[0];
			midd[0] = dzero;

			dy = vis_ld_d64_nf(dpsrc2);
			dy1 = vis_ld_d64_nf(dpsrc2 + 1);
			dy = vis_faligndata(dy, dy1);
			dy = vis_fxor((dy), offset);
			vis_pst_16(dy, &midd[0], emask);
			dy = midd[0];

			dx2 = vis_ld_d64_nf(dpsrc1 + 1);
			dy2 = vis_ld_d64_nf(dpsrc2 + 2);
			dy1 = vis_faligndata(dy1, dy2);
			dx3 = vis_ld_d64_nf(dpsrc1 + 2);
			dy3 = vis_ld_d64_nf(dpsrc2 + 3);
			dy2 = vis_faligndata(dy2, dy3);

			dx2 = vis_fxor((dx2), offset);
			dy1 = vis_fxor((dy1), offset);
			dx3 = vis_fxor((dx3), offset);
			dy2 = vis_fxor((dy2), offset);

			compens = 0;

			while (num_row >= 12) {
				size = num_row > 196608 ? 196608 : num_row;
				if ((((mlib_addr)psrc1 ^ (mlib_addr)psrc2)
					& 7) == 0) {
					for (j = 0; j <= (size - 12); j += 12) {

						INTER_MUL_U16_S32_3
						IMG1_SELF_MUL_U16_S32_3
						IMG2_SELF_MUL_U16_S32_3
						IMG1_SUM_S32_U16_D64
						IMG2_SUM_S32_U16_D64
						dpsrc1 += 3;
						dpsrc2 += 3;
						dx = vis_ld_d64_nf(dpsrc1);
						dy = vis_ld_d64_nf(dpsrc2);

						dx2 = dpsrc1[1];
						dy1 = dpsrc2[1];
						dx3 = dpsrc1[2];
						dy2 = dpsrc2[2];

						dx = vis_fxor((dx), offset);
						dy = vis_fxor((dy), offset);
						dx2 = vis_fxor((dx2), offset);
						dy1 = vis_fxor((dy1), offset);
						dx3 = vis_fxor((dx3), offset);
						dy2 = vis_fxor((dy2), offset);

						compens += 2;
					}
				} else {
					for (j = 0; j <= (size - 12); j += 12) {

						INTER_MUL_U16_S32_3
						IMG1_SELF_MUL_U16_S32_3
						IMG2_SELF_MUL_U16_S32_3
						IMG1_SUM_S32_U16_D64
						IMG2_SUM_S32_U16_D64

						dpsrc1 += 3;
						dpsrc2 += 3;
						dx = vis_ld_d64_nf(dpsrc1);
						dy1 = vis_ld_d64_nf(dpsrc2 + 1);
						dy = vis_faligndata(dy3, dy1);

						dx2 = vis_ld_d64_nf(dpsrc1 + 1);
						dy2 = vis_ld_d64_nf(dpsrc2 + 2);
						dy1 = vis_faligndata(dy1, dy2);
						dx3 = vis_ld_d64_nf(dpsrc1 + 2);
						dy3 = vis_ld_d64_nf(dpsrc2 + 3);
						dy2 = vis_faligndata(dy2, dy3);

						dx = vis_fxor((dx), offset);
						dy = vis_fxor((dy), offset);
						dx2 = vis_fxor((dx2), offset);
						dy1 = vis_fxor((dy1), offset);
						dx3 = vis_fxor((dx3), offset);
						dy2 = vis_fxor((dy2), offset);

						compens += 2;
					}
				}

				num_row -= j;

				accumulator.db = tmpAcc11;
				sum11 += accumulator.two_int.int0;
				sum12 += accumulator.two_int.int1;
				accumulator.db = tmpAcc12;
				sum13 += accumulator.two_int.int0;
				sum11 += accumulator.two_int.int1;
				accumulator.db = tmpAcc13;
				sum12 += accumulator.two_int.int0;
				sum13 += accumulator.two_int.int1;

				accumulator.db = tmpAcc21;
				sum21 += accumulator.two_int.int0;
				sum22 += accumulator.two_int.int1;
				accumulator.db = tmpAcc22;
				sum23 += accumulator.two_int.int0;
				sum21 += accumulator.two_int.int1;
				accumulator.db = tmpAcc23;
				sum22 += accumulator.two_int.int0;
				sum23 += accumulator.two_int.int1;

				tmpAcc21 = tmpAcc22 = tmpAcc23 = dzero;
				tmpAcc11 = tmpAcc12 = tmpAcc13 = dzero;

				rez[al2off[off + 0]] += ccInterAcc1;
				rez[al2off[off + 0] + 4] += ccSelfAcc11;
				rez[al2off[off + 0] + 8] += ccSelfAcc21;
				rez[al2off[off + 0] + 12] += sum11;
				rez[al2off[off + 0] + 16] += sum21;

				rez[al2off[off + 2]] += ccInterAcc2;
				rez[al2off[off + 2] + 4] += ccSelfAcc12;
				rez[al2off[off + 2] + 8] += ccSelfAcc22;
				rez[al2off[off + 2] + 12] += sum12;
				rez[al2off[off + 2] + 16] += sum22;

				rez[al2off[off + 1]] += ccInterAcc3;
				rez[al2off[off + 1] + 4] += ccSelfAcc13;
				rez[al2off[off + 1] + 8] += ccSelfAcc23;
				rez[al2off[off + 1] + 12] += sum13;
				rez[al2off[off + 1] + 16] += sum23;

				ccInterAcc1 = ccInterAcc2 = ccInterAcc3 = dzero;
				ccSelfAcc11 = ccSelfAcc12 = ccSelfAcc13 = dzero;
				ccSelfAcc21 = ccSelfAcc22 = ccSelfAcc23 = dzero;
				sum11 = sum12 = sum13 = dzero;
				sum21 = sum22 = sum23 = dzero;
				tmpAcc11 = tmpAcc12 = tmpAcc13 = dzero;
				tmpAcc21 = tmpAcc22 = tmpAcc23 = dzero;
			}

			if (num_row > 0) {
				midd[0] = midd[1] = midd[2] = dzero;
				emask = 0xfff000 >> num_row;

				vis_pst_16(dy, &midd[0], emask >> 8);
				dy = midd[0];
				vis_pst_16(dy1, &midd[1], emask >> 4);
				dy1 = midd[1];
				vis_pst_16(dy2, &midd[2], emask);
				dy2 = midd[2];

				midd[0] = midd[1] = midd[2] = dzero;
				vis_pst_16(dx, &midd[0], emask >> 8);
				dx = midd[0];
				vis_pst_16(dx2, &midd[1], emask >> 4);
				dx2 = midd[1];
				vis_pst_16(dx3, &midd[2], emask);
				dx3 = midd[2];

				INTER_MUL_U16_S32_3
				IMG1_SELF_MUL_U16_S32_3
				IMG2_SELF_MUL_U16_S32_3
				IMG1_SUM_S32_U16_D64
				IMG2_SUM_S32_U16_D64
				compens += 2;

				accumulator.db = tmpAcc11;
				sum11 += accumulator.two_int.int0;
				sum12 += accumulator.two_int.int1;
				accumulator.db = tmpAcc12;
				sum13 += accumulator.two_int.int0;
				sum11 += accumulator.two_int.int1;
				accumulator.db = tmpAcc13;
				sum12 += accumulator.two_int.int0;
				sum13 += accumulator.two_int.int1;

				accumulator.db = tmpAcc21;
				sum21 += accumulator.two_int.int0;
				sum22 += accumulator.two_int.int1;
				accumulator.db = tmpAcc22;
				sum23 += accumulator.two_int.int0;
				sum21 += accumulator.two_int.int1;
				accumulator.db = tmpAcc23;
				sum22 += accumulator.two_int.int0;
				sum23 += accumulator.two_int.int1;

				tmpAcc21 = tmpAcc22 = tmpAcc23 = dzero;
				tmpAcc11 = tmpAcc12 = tmpAcc13 = dzero;

				rez[al2off[off + 0]] += ccInterAcc1;
				rez[al2off[off + 0] + 4] += ccSelfAcc11;
				rez[al2off[off + 0] + 8] += ccSelfAcc21;
				rez[al2off[off + 0] + 12] += sum11;
				rez[al2off[off + 0] + 16] += sum21;

				rez[al2off[off + 2]] += ccInterAcc2;
				rez[al2off[off + 2] + 4] += ccSelfAcc12;
				rez[al2off[off + 2] + 8] += ccSelfAcc22;
				rez[al2off[off + 2] + 12] += sum12;
				rez[al2off[off + 2] + 16] += sum22;

				rez[al2off[off + 1]] += ccInterAcc3;
				rez[al2off[off + 1] + 4] += ccSelfAcc13;
				rez[al2off[off + 1] + 8] += ccSelfAcc23;
				rez[al2off[off + 1] + 12] += sum13;
				rez[al2off[off + 1] + 16] += sum23;

			}

			rez[al2off[off + 0]] += compens;
			rez[al2off[off + 0] + 4] += compens;
			rez[al2off[off + 0] + 8] += compens;

			rez[al2off[off + 2]] += compens;
			rez[al2off[off + 2] + 4] += compens;
			rez[al2off[off + 2] + 8] += compens;

			rez[al2off[off + 1]] += compens;
			rez[al2off[off + 1] + 4] += compens;
			rez[al2off[off + 1] + 8] += compens;

			psrc1 += src1_stride;
			psrc2 += src2_stride;
		}
	}

}
/* *********************************************************** */
#define	INTER_MUL_S16_S32                                             \
	dr1 = vis_fmuld8sux16(vis_read_hi(dx), vis_read_hi(dy));          \
	dr2 = vis_fmuld8ulx16(vis_read_hi(dx), vis_read_hi(dy));          \
	dr3 = vis_fmuld8sux16(vis_read_lo(dx), vis_read_lo(dy));          \
	dr4 = vis_fmuld8ulx16(vis_read_lo(dx), vis_read_lo(dy));          \
	dr1 = vis_fpadd32(dr1, mone);                                     \
	dr3 = vis_fpadd32(dr3, mone);                                     \
	ccTmpInterAcc1.db = vis_fpadd32(dr1, dr2);                        \
	ccTmpInterAcc2.db = vis_fpadd32(dr3, dr4);                        \
	dr1 = vis_fmuld8sux16(vis_read_hi(dx1), vis_read_hi(dy2));        \
	dr2 = vis_fmuld8ulx16(vis_read_hi(dx1), vis_read_hi(dy2));        \
	dr3 = vis_fmuld8sux16(vis_read_lo(dx1), vis_read_lo(dy2));        \
	dr4 = vis_fmuld8ulx16(vis_read_lo(dx1), vis_read_lo(dy2));        \
	ccTmpInterAcc1.db = vis_fpadd32(ccTmpInterAcc1.db, dr1);          \
	ccTmpInterAcc1.db = vis_fpadd32(ccTmpInterAcc1.db, dr2);          \
	ccTmpInterAcc2.db = vis_fpadd32(ccTmpInterAcc2.db, dr3);          \
	ccTmpInterAcc2.db = vis_fpadd32(ccTmpInterAcc2.db, dr4);          \
	ccInterAcc1 += (mlib_d64)ccTmpInterAcc1.two_int.int0;             \
	ccInterAcc2 += (mlib_d64)ccTmpInterAcc1.two_int.int1;             \
	ccInterAcc3 += (mlib_d64)ccTmpInterAcc2.two_int.int0;             \
	ccInterAcc4 += (mlib_d64)ccTmpInterAcc2.two_int.int1;

/* *********************************************************** */

#define	IMG1_SELF_MUL_S16_S32                                         \
	dr1 = vis_fmuld8sux16(vis_read_hi(dx), vis_read_hi(dx));          \
	dr2 = vis_fmuld8ulx16(vis_read_hi(dx), vis_read_hi(dx));          \
	dr3 = vis_fmuld8sux16(vis_read_lo(dx), vis_read_lo(dx));          \
	dr4 = vis_fmuld8ulx16(vis_read_lo(dx), vis_read_lo(dx));          \
	dr1 = vis_fpadd32(dr1, mone);                                     \
	dr3 = vis_fpadd32(dr3, mone);                                     \
	ccTmpSelfAcc11.db = vis_fpadd32(dr1, dr2);                        \
	ccTmpSelfAcc12.db = vis_fpadd32(dr3, dr4);                        \
	dr1 = vis_fmuld8sux16(vis_read_hi(dx1), vis_read_hi(dx1));        \
	dr2 = vis_fmuld8ulx16(vis_read_hi(dx1), vis_read_hi(dx1));        \
	dr3 = vis_fmuld8sux16(vis_read_lo(dx1), vis_read_lo(dx1));        \
	dr4 = vis_fmuld8ulx16(vis_read_lo(dx1), vis_read_lo(dx1));        \
	ccTmpSelfAcc11.db = vis_fpadd32(ccTmpSelfAcc11.db, dr1);          \
	ccTmpSelfAcc11.db = vis_fpadd32(ccTmpSelfAcc11.db, dr2);          \
	ccTmpSelfAcc12.db = vis_fpadd32(ccTmpSelfAcc12.db, dr3);          \
	ccTmpSelfAcc12.db = vis_fpadd32(ccTmpSelfAcc12.db, dr4);          \
	ccSelfAcc11 += (mlib_d64)ccTmpSelfAcc11.two_int.int0;             \
	ccSelfAcc12 += (mlib_d64)ccTmpSelfAcc11.two_int.int1;             \
	ccSelfAcc13 += (mlib_d64)ccTmpSelfAcc12.two_int.int0;             \
	ccSelfAcc14 += (mlib_d64)ccTmpSelfAcc12.two_int.int1;

/* *********************************************************** */

#define	IMG2_SELF_MUL_S16_S32                                         \
	dr1 = vis_fmuld8sux16(vis_read_hi(dy), vis_read_hi(dy));          \
	dr2 = vis_fmuld8ulx16(vis_read_hi(dy), vis_read_hi(dy));          \
	dr3 = vis_fmuld8sux16(vis_read_lo(dy), vis_read_lo(dy));          \
	dr4 = vis_fmuld8ulx16(vis_read_lo(dy), vis_read_lo(dy));          \
	dr1 = vis_fpadd32(dr1, mone);                                     \
	dr3 = vis_fpadd32(dr3, mone);                                     \
	ccTmpSelfAcc21.db = vis_fpadd32(dr1, dr2);                        \
	ccTmpSelfAcc22.db = vis_fpadd32(dr3, dr4);                        \
	dr1 = vis_fmuld8sux16(vis_read_hi(dy2), vis_read_hi(dy2));        \
	dr2 = vis_fmuld8ulx16(vis_read_hi(dy2), vis_read_hi(dy2));        \
	dr3 = vis_fmuld8sux16(vis_read_lo(dy2), vis_read_lo(dy2));        \
	dr4 = vis_fmuld8ulx16(vis_read_lo(dy2), vis_read_lo(dy2));        \
	ccTmpSelfAcc21.db = vis_fpadd32(ccTmpSelfAcc21.db, dr1);          \
	ccTmpSelfAcc21.db = vis_fpadd32(ccTmpSelfAcc21.db, dr2);          \
	ccTmpSelfAcc22.db = vis_fpadd32(ccTmpSelfAcc22.db, dr3);          \
	ccTmpSelfAcc22.db = vis_fpadd32(ccTmpSelfAcc22.db, dr4);          \
	ccSelfAcc21 += (mlib_d64)ccTmpSelfAcc21.two_int.int0;             \
	ccSelfAcc22 += (mlib_d64)ccTmpSelfAcc21.two_int.int1;             \
	ccSelfAcc23 += (mlib_d64)ccTmpSelfAcc22.two_int.int0;             \
	ccSelfAcc24 += (mlib_d64)ccTmpSelfAcc22.two_int.int1;

/* *********************************************************** */

#define	IMG1_SUM_S16_D64                                              \
	tmpAcc11 = vis_fpadd32(tmpAcc11,                                  \
		vis_fmuld8ulx16(scale, vis_read_hi(dx)));                     \
	tmpAcc12 = vis_fpadd32(tmpAcc12,                                  \
		vis_fmuld8ulx16(scale, vis_read_lo(dx)));                     \
	tmpAcc11 = vis_fpadd32(tmpAcc11,                                  \
		vis_fmuld8ulx16(scale, vis_read_hi(dx1)));                    \
	tmpAcc12 = vis_fpadd32(tmpAcc12,                                  \
		vis_fmuld8ulx16(scale, vis_read_lo(dx1)));

/* *********************************************************** */
#define	IMG2_SUM_S16_D64                                              \
	tmpAcc21 = vis_fpadd32(tmpAcc21,                                  \
		vis_fmuld8ulx16(scale, vis_read_hi(dy)));                     \
	tmpAcc22 = vis_fpadd32(tmpAcc22,                                  \
		vis_fmuld8ulx16(scale, vis_read_lo(dy)));                     \
	tmpAcc21 = vis_fpadd32(tmpAcc21,                                  \
		vis_fmuld8ulx16(scale, vis_read_hi(dy2)));                    \
	tmpAcc22 = vis_fpadd32(tmpAcc22,                                  \
		vis_fmuld8ulx16(scale, vis_read_lo(dy2)));
/* *********************************************************** */
void
mlib_v_ImageNormCrossCorrel_S16_124(
    const mlib_image *img1,
    const mlib_image *img2,
	const mlib_d64 *m2,
	const mlib_d64 *s2,
    mlib_d64 *rez)
{
/* pointer to the data of first image */
	mlib_s16 *psrc1 = (mlib_s16 *)mlib_ImageGetData(img1);

/* pointer to the data of second image */
	mlib_s16 *psrc2 = (mlib_s16 *)mlib_ImageGetData(img2);

/* height of image */
	mlib_s32 height = mlib_ImageGetHeight(img1);

/* elements to next row */
	mlib_s32 src1_stride = mlib_ImageGetStride(img1) >> 1;

/* elements to next row */
	mlib_s32 src2_stride = mlib_ImageGetStride(img2) >> 1;
	mlib_s32 nchan = mlib_ImageGetChannels(img1);

/* number of elements in the row */
	mlib_s32 size_row = mlib_ImageGetWidth(img1) << (nchan >> 1);
	mlib_s32 num_row, size;

/* indices */
	mlib_s32 i, j, k;

/* edge masks */
	mlib_s32 emask;

	mlib_d64 ccInterAcc1, ccInterAcc2, ccInterAcc3, ccInterAcc4;
	mlib_d64 ccSelfAcc11, ccSelfAcc12, ccSelfAcc13, ccSelfAcc14;
	mlib_d64 ccSelfAcc21, ccSelfAcc22, ccSelfAcc23, ccSelfAcc24;
	mlib_d64 sum11, sum12, sum13, sum14;
	mlib_d64 sum21, sum22, sum23, sum24;
	mlib_d64 tmpAcc11, tmpAcc12, tmpAcc21, tmpAcc22;

	mlib_d64 a1, a2;
	mlib_f32 scale = vis_to_float(0x10001);
	type_union_mlib_d64 ccTmpInterAcc1, ccTmpInterAcc2,
		ccTmpSelfAcc11, ccTmpSelfAcc12,
		ccTmpSelfAcc21, ccTmpSelfAcc22, accumulators[2];
	mlib_d64 offset = vis_to_double_dup(0x80008000);
	const mlib_d64 dzero = vis_fzero();

/* offset of address alignment src1 */
	mlib_s32 off;
	mlib_d64 midd[2], *dpsrc1, *dpsrc2;
	mlib_d64 dx, dx1, dy, dy0, dy1, dy2, dr1, dr2, dr3, dr4;
	mlib_d64 mone = vis_fone();
	mlib_s32 compens;

	if (src1_stride == size_row && src1_stride == src2_stride) {
/* This image is not a sub-image and can be treated as a 1-D vector */
		size_row *= height;
		height = 1;
	}

	if (m2 != NULL && s2 != NULL) {
		for (i = 0; i < height; i++) {
			ccInterAcc1 = ccInterAcc2 = dzero;
			ccInterAcc3 = ccInterAcc4 = dzero;
			ccSelfAcc11 = ccSelfAcc12 = dzero;
			ccSelfAcc13 = ccSelfAcc14 = dzero;
			sum11 = sum12 = sum13 = sum14 = dzero;
			tmpAcc11 = tmpAcc12 = dzero;

			ccTmpInterAcc1.db = ccTmpInterAcc2.db = dzero;
			ccTmpSelfAcc11.db = ccTmpSelfAcc12.db = dzero;
			accumulators[0].db = accumulators[1].db = dzero;
			midd[0] = midd[1] = dzero;

	/* prepare the source address */
			off = (mlib_addr)psrc1 & 7;
			off >>= 1;
			emask = vis_edge16(psrc1, psrc1 + size_row - 1);
			num_row = size_row + off;
			dpsrc1 = (mlib_d64 *)(psrc1 - off);
			dpsrc2 = (mlib_d64 *)vis_alignaddr((psrc2 - off), 0);

			dx = dpsrc1[0];
			vis_pst_16(dx, (void *)midd, emask);
			dx = midd[0];
			dy0 = vis_ld_d64_nf(dpsrc2);
			dy1 = vis_ld_d64_nf(dpsrc2 + 1);
			dy = vis_faligndata(dy0, dy1);
			midd[0] = dzero;
			vis_pst_16(dy, (void *)midd, emask);
			dy = midd[0];

			dx1 = vis_ld_d64_nf(dpsrc1 + 1);
			dy0 = vis_ld_d64_nf(dpsrc2 + 2);
			dy2 = vis_faligndata(dy1, dy0);

			size = 0;
			compens = num_row >> 3;

			while (num_row >= 8) {
				size = num_row & ~7;
				size = size > 131072 ? 131072 : size;

				if ((((mlib_addr)psrc1 ^ (mlib_addr)psrc2)
					& 7) == 0) {
					for (j = 0; j <= (size - 8); j += 8) {
						INTER_MUL_S16_S32
						IMG1_SELF_MUL_S16_S32
						IMG1_SUM_S16_D64

						dpsrc1 += 2;
						dpsrc2 += 2;

						dx = vis_ld_d64_nf(dpsrc1);
						dy = vis_ld_d64_nf(dpsrc2);
						dx1 = dpsrc1[1];
						dy2 = dpsrc2[1];

					}
				} else {
					for (j = 0; j <= (size - 8); j += 8) {
						INTER_MUL_S16_S32
						IMG1_SELF_MUL_S16_S32
						IMG1_SUM_S16_D64

						dpsrc1 += 2;
						dpsrc2 += 2;

						dx = vis_ld_d64_nf(dpsrc1);
						dy1 = vis_ld_d64_nf(dpsrc2 + 1);
						dy = vis_faligndata(dy0, dy1);

						dx1 = vis_ld_d64_nf(dpsrc1 + 1);
						dy0 = vis_ld_d64_nf(dpsrc2 + 2);
						dy2 = vis_faligndata(dy1, dy0);

					}
				}

				num_row -= size;

				accumulators[0].db = tmpAcc11;
				accumulators[1].db = tmpAcc12;

				sum11 += accumulators[0].two_int.int0;
				sum12 += accumulators[0].two_int.int1;
				sum13 += accumulators[1].two_int.int0;
				sum14 += accumulators[1].two_int.int1;

				rez[3 & (0 - off)] += ccInterAcc1;
				rez[(3 & (0 - off)) + 4] += ccSelfAcc11;
				rez[(3 & (0 - off)) + 12] += sum11;

				rez[3 & (1 - off)] += ccInterAcc2;
				rez[(3 & (1 - off)) + 4] += ccSelfAcc12;
				rez[(3 & (1 - off)) + 12] += sum12;

				rez[3 & (2 - off)] += ccInterAcc3;
				rez[(3 & (2 - off)) + 4] += ccSelfAcc13;
				rez[(3 & (2 - off)) + 12] += sum13;

				rez[3 & (3 - off)] += ccInterAcc4;
				rez[(3 & (3 - off)) + 4] += ccSelfAcc14;
				rez[(3 & (3 - off)) + 12] += sum14;

				ccInterAcc1 = ccInterAcc2 = dzero;
				ccInterAcc3 = ccInterAcc4 = dzero;
				ccSelfAcc11 = ccSelfAcc12 = dzero;
				ccSelfAcc13 = ccSelfAcc14 = dzero;
				sum11 = sum12 = sum13 = sum14 = dzero;
				tmpAcc11 = tmpAcc12 = dzero;

			}

			if (num_row > 0) {
	/* prepare edge mask for the last bytes */
				emask = 0xff00 >> num_row;
				midd[0] = midd[1] = dzero;
				vis_pst_16(dx, (void *)midd, emask >> 4);
				dx = midd[0];
				vis_pst_16(dx1, (void *)(midd + 1), emask);
				dx1 = midd[1];

				midd[0] = midd[1] = dzero;
				vis_pst_16(dy, (void *)midd, emask >> 4);
				dy = midd[0];
				vis_pst_16(dy2, (void *)(midd + 1), emask);
				dy2 = midd[1];

				INTER_MUL_S16_S32
				IMG1_SELF_MUL_S16_S32
				IMG1_SUM_S16_D64

				compens += 1;

				accumulators[0].db = tmpAcc11;
				accumulators[1].db = tmpAcc12;

				sum11 += accumulators[0].two_int.int0;
				sum12 += accumulators[0].two_int.int1;
				sum13 += accumulators[1].two_int.int0;
				sum14 += accumulators[1].two_int.int1;

				rez[3 & (0 - off)] += ccInterAcc1;
				rez[(3 & (0 - off)) + 4] += ccSelfAcc11;
				rez[(3 & (0 - off)) + 12] += sum11;

				rez[3 & (1 - off)] += ccInterAcc2;
				rez[(3 & (1 - off)) + 4] += ccSelfAcc12;
				rez[(3 & (1 - off)) + 12] += sum12;

				rez[3 & (2 - off)] += ccInterAcc3;
				rez[(3 & (2 - off)) + 4] += ccSelfAcc13;
				rez[(3 & (2 - off)) + 12] += sum13;

				rez[3 & (3 - off)] += ccInterAcc4;
				rez[(3 & (3 - off)) + 4] += ccSelfAcc14;
				rez[(3 & (3 - off)) + 12] += sum14;

			}

			rez[3 & (0 - off)] += (mlib_d64)compens;
			rez[(3 & (0 - off)) + 4] += (mlib_d64)compens;

			rez[3 & (1 - off)] += (mlib_d64)compens;
			rez[(3 & (1 - off)) + 4] += (mlib_d64)compens;

			rez[3 & (2 - off)] += (mlib_d64)compens;
			rez[(3 & (2 - off)) + 4] += (mlib_d64)compens;

			rez[3 & (3 - off)] += (mlib_d64)compens;
			rez[(3 & (3 - off)) + 4] += (mlib_d64)compens;

			psrc1 += src1_stride;
			psrc2 += src2_stride;
		}
	} else {
		for (i = 0; i < height; i++) {
			ccInterAcc1 = ccInterAcc2 = dzero;
			ccInterAcc3 = ccInterAcc4 = dzero;
			ccSelfAcc11 = ccSelfAcc12 = dzero;
			ccSelfAcc13 = ccSelfAcc14 = dzero;
			ccSelfAcc21 = ccSelfAcc22 = dzero;
			ccSelfAcc23 = ccSelfAcc24 = dzero;
			sum11 = sum12 = sum13 = sum14 = dzero;
			sum21 = sum22 = sum23 = sum24 = dzero;
			tmpAcc11 = tmpAcc12 = dzero;
			tmpAcc21 = tmpAcc22 = dzero;

			midd[0] = midd[1] = dzero;
			ccTmpInterAcc1.db = ccTmpInterAcc2.db = dzero;
			ccTmpSelfAcc11.db = ccTmpSelfAcc12.db = dzero;
			ccTmpSelfAcc21.db = ccTmpSelfAcc22.db = dzero;
			accumulators[0].db = accumulators[1].db = dzero;

	/* prepare the source address */
			off = (mlib_addr)psrc1 & 7;
			off >>= 1;
			emask = vis_edge16(psrc1, psrc1 + size_row - 1);
			num_row = size_row + off;
			dpsrc1 = (mlib_d64 *)(psrc1 - off);
			dpsrc2 = (mlib_d64 *)vis_alignaddr((psrc2 - off), 0);

			dx = dpsrc1[0];
			vis_pst_16(dx, (void *)midd, emask);
			dx = midd[0];
			dy0 = vis_ld_d64_nf(dpsrc2);
			dy1 = vis_ld_d64_nf(dpsrc2 + 1);
			dy = vis_faligndata(dy0, dy1);
			midd[0] = dzero;
			vis_pst_16(dy, (void *)midd, emask);
			dy = midd[0];

			dx1 = vis_ld_d64_nf(dpsrc1 + 1);
			dy0 = vis_ld_d64_nf(dpsrc2 + 2);
			dy2 = vis_faligndata(dy1, dy0);

			compens = num_row >> 3;

			while (num_row >= 8) {
				size = num_row & ~7;
				size = size > 131072 ? 131072 : size;

				if ((((mlib_addr)psrc1 ^ (mlib_addr)psrc2)
					& 7) == 0) {
					for (j = 0; j <= (size - 8); j += 8) {
						INTER_MUL_S16_S32
						IMG1_SELF_MUL_S16_S32
						IMG2_SELF_MUL_S16_S32
						IMG1_SUM_S16_D64
						IMG2_SUM_S16_D64

						dpsrc1 += 2;
						dpsrc2 += 2;

						dx = vis_ld_d64_nf(dpsrc1);
						dy = vis_ld_d64_nf(dpsrc2);
						dx1 = dpsrc1[1];
						dy2 = dpsrc2[1];
					}
				} else {
					for (j = 0; j <= (size - 8); j += 8) {
						INTER_MUL_S16_S32
						IMG1_SELF_MUL_S16_S32
						IMG2_SELF_MUL_S16_S32
						IMG1_SUM_S16_D64
						IMG2_SUM_S16_D64

						dpsrc1 += 2;
						dpsrc2 += 2;
						dx = vis_ld_d64_nf(dpsrc1);
						dy1 = vis_ld_d64_nf(dpsrc2 + 1);
						dy = vis_faligndata(dy0, dy1);

						dx1 = vis_ld_d64_nf(dpsrc1 + 1);
						dy0 = vis_ld_d64_nf(dpsrc2 + 2);
						dy2 = vis_faligndata(dy1, dy0);
					}
				}
				num_row -= size;
				accumulators[0].db = tmpAcc11;
				accumulators[1].db = tmpAcc12;

				sum11 += accumulators[0].two_int.int0;
				sum12 += accumulators[0].two_int.int1;
				sum13 += accumulators[1].two_int.int0;
				sum14 += accumulators[1].two_int.int1;

				accumulators[0].db = tmpAcc21;
				accumulators[1].db = tmpAcc22;

				sum21 += accumulators[0].two_int.int0;
				sum22 += accumulators[0].two_int.int1;
				sum23 += accumulators[1].two_int.int0;
				sum24 += accumulators[1].two_int.int1;

				rez[3 & (0 - off)] += ccInterAcc1;
				rez[(3 & (0 - off)) + 4] += ccSelfAcc11;
				rez[(3 & (0 - off)) + 8] += ccSelfAcc21;
				rez[(3 & (0 - off)) + 12] += sum11;
				rez[(3 & (0 - off)) + 16] += sum21;

				rez[3 & (1 - off)] += ccInterAcc2;
				rez[(3 & (1 - off)) + 4] += ccSelfAcc12;
				rez[(3 & (1 - off)) + 8] += ccSelfAcc22;
				rez[(3 & (1 - off)) + 12] += sum12;
				rez[(3 & (1 - off)) + 16] += sum22;

				rez[3 & (2 - off)] += ccInterAcc3;
				rez[(3 & (2 - off)) + 4] += ccSelfAcc13;
				rez[(3 & (2 - off)) + 8] += ccSelfAcc23;
				rez[(3 & (2 - off)) + 12] += sum13;
				rez[(3 & (2 - off)) + 16] += sum23;

				rez[3 & (3 - off)] += ccInterAcc4;
				rez[(3 & (3 - off)) + 4] += ccSelfAcc14;
				rez[(3 & (3 - off)) + 8] += ccSelfAcc24;
				rez[(3 & (3 - off)) + 12] += sum14;
				rez[(3 & (3 - off)) + 16] += sum24;

				ccInterAcc1 = ccInterAcc2 = dzero;
				ccInterAcc3 = ccInterAcc4 = dzero;
				ccSelfAcc11 = ccSelfAcc12 = dzero;
				ccSelfAcc13 = ccSelfAcc14 = dzero;
				ccSelfAcc21 = ccSelfAcc22 = dzero;
				ccSelfAcc23 = ccSelfAcc24 = dzero;
				sum11 = sum12 = sum13 = sum14 = dzero;
				sum21 = sum22 = sum23 = sum24 = dzero;
				tmpAcc11 = tmpAcc12 = dzero;
				tmpAcc21 = tmpAcc22 = dzero;

			}

			if (num_row > 0) {

				midd[0] = midd[1] = dzero;
	/* prepare edge mask for the last bytes */
				emask = 0xff00 >> num_row;

				midd[0] = midd[1] = dzero;
				vis_pst_16(dx, &midd[0], emask >> 4);
				dx = midd[0];
				vis_pst_16(dx1, &midd[1], emask);
				dx1 = midd[1];

				midd[0] = midd[1] = dzero;
				vis_pst_16(dy, &midd[0], emask >> 4);
				dy = midd[0];
				vis_pst_16(dy2, &midd[1], emask);
				dy2 = midd[1];

				INTER_MUL_S16_S32
				IMG1_SELF_MUL_S16_S32
				IMG2_SELF_MUL_S16_S32
				IMG1_SUM_S16_D64
				IMG2_SUM_S16_D64

				compens += 1;

				accumulators[0].db = tmpAcc11;
				accumulators[1].db = tmpAcc12;

				sum11 += accumulators[0].two_int.int0;
				sum12 += accumulators[0].two_int.int1;
				sum13 += accumulators[1].two_int.int0;
				sum14 += accumulators[1].two_int.int1;

				accumulators[0].db = tmpAcc21;
				accumulators[1].db = tmpAcc22;

				sum21 += accumulators[0].two_int.int0;
				sum22 += accumulators[0].two_int.int1;
				sum23 += accumulators[1].two_int.int0;
				sum24 += accumulators[1].two_int.int1;

				rez[3 & (0 - off)] += ccInterAcc1;
				rez[(3 & (0 - off)) + 4] += ccSelfAcc11;
				rez[(3 & (0 - off)) + 8] += ccSelfAcc21;
				rez[(3 & (0 - off)) + 12] += sum11;
				rez[(3 & (0 - off)) + 16] += sum21;

				rez[3 & (1 - off)] += ccInterAcc2;
				rez[(3 & (1 - off)) + 4] += ccSelfAcc12;
				rez[(3 & (1 - off)) + 8] += ccSelfAcc22;
				rez[(3 & (1 - off)) + 12] += sum12;
				rez[(3 & (1 - off)) + 16] += sum22;

				rez[3 & (2 - off)] += ccInterAcc3;
				rez[(3 & (2 - off)) + 4] += ccSelfAcc13;
				rez[(3 & (2 - off)) + 8] += ccSelfAcc23;
				rez[(3 & (2 - off)) + 12] += sum13;
				rez[(3 & (2 - off)) + 16] += sum23;

				rez[3 & (3 - off)] += ccInterAcc4;
				rez[(3 & (3 - off)) + 4] += ccSelfAcc14;
				rez[(3 & (3 - off)) + 8] += ccSelfAcc24;
				rez[(3 & (3 - off)) + 12] += sum14;
				rez[(3 & (3 - off)) + 16] += sum24;

			}

			rez[3 & (0 - off)] += (mlib_d64)compens;
			rez[(3 & (0 - off)) + 4] += (mlib_d64)compens;
			rez[(3 & (0 - off)) + 8] += (mlib_d64)compens;

			rez[3 & (1 - off)] += (mlib_d64)compens;
			rez[(3 & (1 - off)) + 4] += (mlib_d64)compens;
			rez[(3 & (1 - off)) + 8] += (mlib_d64)compens;

			rez[3 & (2 - off)] += (mlib_d64)compens;
			rez[(3 & (2 - off)) + 4] += (mlib_d64)compens;
			rez[(3 & (2 - off)) + 8] += (mlib_d64)compens;

			rez[3 & (3 - off)] += (mlib_d64)compens;
			rez[(3 & (3 - off)) + 4] += (mlib_d64)compens;
			rez[(3 & (3 - off)) + 8] += (mlib_d64)compens;

			psrc1 += src1_stride;
			psrc2 += src2_stride;
		}
	}

}
/* *********************************************************** */

#define	INTER_MUL_S16_S32_3                                               \
	dr1 = vis_fmuld8sux16(vis_read_hi(dx), vis_read_hi(dy));              \
	dr2 = vis_fmuld8ulx16(vis_read_hi(dx), vis_read_hi(dy));              \
	dr3 = vis_fmuld8sux16(vis_read_lo(dx), vis_read_lo(dy));              \
	dr4 = vis_fmuld8ulx16(vis_read_lo(dx), vis_read_lo(dy));              \
	dr1 = vis_fpadd32(dr1, mone);                                         \
	dr3 = vis_fpadd32(dr3, mone);                                         \
	dc1.db = vis_fpadd32(dr1, dr2);                                       \
	dc2.db = vis_fpadd32(dr3, dr4);                                       \
	dr1 = vis_fmuld8sux16(vis_read_hi(dx2), vis_read_hi(dy1));            \
	dr2 = vis_fmuld8ulx16(vis_read_hi(dx2), vis_read_hi(dy1));            \
	dr3 = vis_fmuld8sux16(vis_read_lo(dx2), vis_read_lo(dy1));            \
	dr4 = vis_fmuld8ulx16(vis_read_lo(dx2), vis_read_lo(dy1));            \
	dr1 = vis_fpadd32(dr1, mone);                                         \
	dc3.db = vis_fpadd32(dr1, dr2);                                       \
	dc1.db = vis_fpadd32(dc1.db, dr3);                                    \
	dc1.db = vis_fpadd32(dc1.db, dr4);                                    \
	ccInterAcc1 += (mlib_d64)dc1.two_int.int0;                            \
	ccInterAcc2 += (mlib_d64)dc1.two_int.int1;                            \
	dr1 = vis_fmuld8sux16(vis_read_hi(dx3), vis_read_hi(dy2));            \
	dr2 = vis_fmuld8ulx16(vis_read_hi(dx3), vis_read_hi(dy2));            \
	dr3 = vis_fmuld8sux16(vis_read_lo(dx3), vis_read_lo(dy2));            \
	dr4 = vis_fmuld8ulx16(vis_read_lo(dx3), vis_read_lo(dy2));            \
	dc2.db = vis_fpadd32(dc2.db, dr1);                                    \
	dc2.db = vis_fpadd32(dc2.db, dr2);                                    \
	dc3.db = vis_fpadd32(dc3.db, dr3);                                    \
	dc3.db = vis_fpadd32(dc3.db, dr4);                                    \
	ccInterAcc3 += (mlib_d64)dc2.two_int.int0;                            \
	ccInterAcc1 += (mlib_d64)dc2.two_int.int1;                            \
	ccInterAcc2 += (mlib_d64)dc3.two_int.int0;                            \
	ccInterAcc3 += (mlib_d64)dc3.two_int.int1;

/* *********************************************************** */

#define	IMG1_SELF_MUL_S16_S32_3                                           \
	dr1 = vis_fmuld8sux16(vis_read_hi(dx), vis_read_hi(dx));              \
	dr2 = vis_fmuld8ulx16(vis_read_hi(dx), vis_read_hi(dx));              \
	dr3 = vis_fmuld8sux16(vis_read_lo(dx), vis_read_lo(dx));              \
	dr4 = vis_fmuld8ulx16(vis_read_lo(dx), vis_read_lo(dx));              \
	dr1 = vis_fpadd32(dr1, mone);                                         \
	dr3 = vis_fpadd32(dr3, mone);                                         \
	dc1.db = vis_fpadd32(dr1, dr2);                                       \
	dc2.db = vis_fpadd32(dr3, dr4);                                       \
	dr1 = vis_fmuld8sux16(vis_read_hi(dx2), vis_read_hi(dx2));            \
	dr2 = vis_fmuld8ulx16(vis_read_hi(dx2), vis_read_hi(dx2));            \
	dr3 = vis_fmuld8sux16(vis_read_lo(dx2), vis_read_lo(dx2));            \
	dr4 = vis_fmuld8ulx16(vis_read_lo(dx2), vis_read_lo(dx2));            \
	dr1 = vis_fpadd32(dr1, mone);                                         \
	dc3.db = vis_fpadd32(dr1, dr2);                                       \
	dc1.db = vis_fpadd32(dc1.db, dr3);                                    \
	dc1.db = vis_fpadd32(dc1.db, dr4);                                    \
	ccSelfAcc11 += (mlib_d64)dc1.two_int.int0;                            \
	ccSelfAcc12 += (mlib_d64)dc1.two_int.int1;                            \
	dr1 = vis_fmuld8sux16(vis_read_hi(dx3), vis_read_hi(dx3));            \
	dr2 = vis_fmuld8ulx16(vis_read_hi(dx3), vis_read_hi(dx3));            \
	dr3 = vis_fmuld8sux16(vis_read_lo(dx3), vis_read_lo(dx3));            \
	dr4 = vis_fmuld8ulx16(vis_read_lo(dx3), vis_read_lo(dx3));            \
	dc2.db = vis_fpadd32(dc2.db, dr1);                                    \
	dc2.db = vis_fpadd32(dc2.db, dr2);                                    \
	dc3.db = vis_fpadd32(dc3.db, dr3);                                    \
	dc3.db = vis_fpadd32(dc3.db, dr4);                                    \
	ccSelfAcc13 += (mlib_d64)dc2.two_int.int0;                            \
	ccSelfAcc11 += (mlib_d64)dc2.two_int.int1;                            \
	ccSelfAcc12 += (mlib_d64)dc3.two_int.int0;                            \
	ccSelfAcc13 += (mlib_d64)dc3.two_int.int1;

/* *********************************************************** */

#define	IMG2_SELF_MUL_S16_S32_3                                           \
	dr1 = vis_fmuld8sux16(vis_read_hi(dy), vis_read_hi(dy));              \
	dr2 = vis_fmuld8ulx16(vis_read_hi(dy), vis_read_hi(dy));              \
	dr3 = vis_fmuld8sux16(vis_read_lo(dy), vis_read_lo(dy));              \
	dr4 = vis_fmuld8ulx16(vis_read_lo(dy), vis_read_lo(dy));              \
	dr1 = vis_fpadd32(dr1, mone);                                         \
	dr3 = vis_fpadd32(dr3, mone);                                         \
	dc1.db = vis_fpadd32(dr1, dr2);                                       \
	dc2.db = vis_fpadd32(dr3, dr4);                                       \
	dr1 = vis_fmuld8sux16(vis_read_hi(dy1), vis_read_hi(dy1));            \
	dr2 = vis_fmuld8ulx16(vis_read_hi(dy1), vis_read_hi(dy1));            \
	dr3 = vis_fmuld8sux16(vis_read_lo(dy1), vis_read_lo(dy1));            \
	dr4 = vis_fmuld8ulx16(vis_read_lo(dy1), vis_read_lo(dy1));            \
	dr1 = vis_fpadd32(dr1, mone);                                         \
	dc3.db = vis_fpadd32(dr1, dr2);                                       \
	dc1.db = vis_fpadd32(dc1.db, dr3);                                    \
	dc1.db = vis_fpadd32(dc1.db, dr4);                                    \
	ccSelfAcc21 += (mlib_d64)dc1.two_int.int0;                            \
	ccSelfAcc22 += (mlib_d64)dc1.two_int.int1;                            \
	dr1 = vis_fmuld8sux16(vis_read_hi(dy2), vis_read_hi(dy2));            \
	dr2 = vis_fmuld8ulx16(vis_read_hi(dy2), vis_read_hi(dy2));            \
	dr3 = vis_fmuld8sux16(vis_read_lo(dy2), vis_read_lo(dy2));            \
	dr4 = vis_fmuld8ulx16(vis_read_lo(dy2), vis_read_lo(dy2));            \
	dc2.db = vis_fpadd32(dc2.db, dr1);                                    \
	dc2.db = vis_fpadd32(dc2.db, dr2);                                    \
	dc3.db = vis_fpadd32(dc3.db, dr3);                                    \
	dc3.db = vis_fpadd32(dc3.db, dr4);                                    \
	ccSelfAcc23 += (mlib_d64)dc2.two_int.int0;                            \
	ccSelfAcc21 += (mlib_d64)dc2.two_int.int1;                            \
	ccSelfAcc22 += (mlib_d64)dc3.two_int.int0;                            \
	ccSelfAcc23 += (mlib_d64)dc3.two_int.int1;

/* *********************************************************** */

#define	IMG1_SUM_S32_D64                                            \
	tmpAcc11 =                                                      \
		vis_fpadd32(tmpAcc11,                                       \
		vis_fmuld8ulx16(scale, vis_read_hi(dx)));                   \
	tmpAcc11 =                                                      \
		vis_fpadd32(tmpAcc11,                                       \
		vis_fmuld8ulx16(scale, vis_read_lo(dx2)));                  \
	tmpAcc12 =                                                      \
		vis_fpadd32(tmpAcc12,                                       \
		vis_fmuld8ulx16(scale, vis_read_lo(dx)));                   \
	tmpAcc12 =                                                      \
		vis_fpadd32(tmpAcc12,                                       \
		vis_fmuld8ulx16(scale, vis_read_hi(dx3)));                  \
	tmpAcc13 =                                                      \
		vis_fpadd32(tmpAcc13,                                       \
		vis_fmuld8ulx16(scale, vis_read_hi(dx2)));                  \
	tmpAcc13 =                                                      \
		vis_fpadd32(tmpAcc13,                                       \
		vis_fmuld8ulx16(scale, vis_read_lo(dx3)));

/* *********************************************************** */

#define	IMG2_SUM_S32_D64                                            \
	tmpAcc21 =                                                      \
		vis_fpadd32(tmpAcc21,                                       \
		vis_fmuld8ulx16(scale, vis_read_hi(dy)));                   \
	tmpAcc21 =                                                      \
		vis_fpadd32(tmpAcc21,                                       \
		vis_fmuld8ulx16(scale, vis_read_lo(dy1)));                  \
	tmpAcc22 =                                                      \
		vis_fpadd32(tmpAcc22,                                       \
		vis_fmuld8ulx16(scale, vis_read_lo(dy)));                   \
	tmpAcc22 =                                                      \
		vis_fpadd32(tmpAcc22,                                       \
		vis_fmuld8ulx16(scale, vis_read_hi(dy2)));                  \
	tmpAcc23 =                                                      \
		vis_fpadd32(tmpAcc23,                                       \
		vis_fmuld8ulx16(scale, vis_read_hi(dy1)));                  \
	tmpAcc23 =                                                      \
		vis_fpadd32(tmpAcc23,                                       \
		vis_fmuld8ulx16(scale, vis_read_lo(dy2)));

/* *********************************************************** */


void
mlib_v_ImageNormCrossCorrel_S16_3(
    const mlib_image *img1,
    const mlib_image *img2,
	const mlib_d64 *m2,
	const mlib_d64 *s2,
    mlib_d64 *rez)
{
/* pointer to the data of first image */
	mlib_s16 *psrc1 = (mlib_s16 *)mlib_ImageGetData(img1);

/* pointer to the data of second image */
	mlib_s16 *psrc2 = (mlib_s16 *)mlib_ImageGetData(img2);

/* height of image */
	mlib_s32 height = mlib_ImageGetHeight(img1);

/* elements to next row */
	mlib_s32 src1_stride = mlib_ImageGetStride(img1) >> 1;

/* elements to next row */
	mlib_s32 src2_stride = mlib_ImageGetStride(img2) >> 1;

/* number of elements in the row */
	mlib_s32 size_row = mlib_ImageGetWidth(img1) * 3;

/* indices */
	mlib_s32 i, j, k;

/* offset of address alignment in destination */
	mlib_s32 off;
	mlib_d64 midd[3], *dpsrc1, *dpsrc2;
	mlib_d64 dx, dy, dx2, dx3, dy1, dy2, dy3, dr1, dr2, dr3, dr4;
	type_union_mlib_d64 dc1, dc2, dc3;
	mlib_s32 num_row, size;

	mlib_d64 ccInterAcc1, ccInterAcc2, ccInterAcc3;
	mlib_d64 ccSelfAcc11, ccSelfAcc12, ccSelfAcc13;
	mlib_d64 ccSelfAcc21, ccSelfAcc22, ccSelfAcc23;
	mlib_d64 sum11, sum12, sum13;
	mlib_d64 sum21, sum22, sum23;
	mlib_d64 tmpAcc11, tmpAcc12, tmpAcc13;
	mlib_d64 tmpAcc21, tmpAcc22, tmpAcc23;

	mlib_f32 scale = vis_to_float(0x10001);
	type_union_mlib_d64 accumulator;
	const mlib_d64 dzero = vis_fzero();

/* edge masks */
	mlib_s32 emask;

/* compute mean3_off(align, sumn) */
	const mlib_u8 al2off[12] = { 0, 2, 1, 0, 2, 1, 0, 2, 1, 0, 2, 1 };
	mlib_d64 mone = vis_fone();
	mlib_s32 compens;

	if (src1_stride == size_row && src1_stride == src2_stride) {
/* This image is not a sub-image and can be treated as a 1-D vector */
		size_row *= height;
		height = 1;
	}


	if (m2 != NULL && s2 != NULL) {
		for (i = 0; i < height; i++) {
			midd[0] = midd[1] = midd[2] = dzero;

			ccInterAcc1 = ccInterAcc2 = ccInterAcc3 = dzero;
			ccSelfAcc11 = ccSelfAcc12 = ccSelfAcc13 = dzero;
			sum11 = sum12 = sum13 = dzero;
			tmpAcc11 = tmpAcc12 = tmpAcc13 = dzero;

	/* prepare the source address */
			off = (mlib_addr)psrc1 & 7;
			off >>= 1;
			emask = vis_edge16(psrc1, psrc1 + size_row - 1);
			num_row = size_row + off;
			dpsrc1 = (mlib_d64 *)(psrc1 - off);
			dpsrc2 = (mlib_d64 *)vis_alignaddr((psrc2 - off), 0);

			vis_pst_16(dpsrc1[0], &midd[0], emask);
			dx = midd[0];
			midd[0] = dzero;

			dy = vis_ld_d64_nf(dpsrc2);
			dy1 = vis_ld_d64_nf(dpsrc2 + 1);
			dy = vis_faligndata(dy, dy1);
			vis_pst_16(dy, &midd[0], emask);
			dy = midd[0];

			dx2 = vis_ld_d64_nf(dpsrc1 + 1);
			dy2 = vis_ld_d64_nf(dpsrc2 + 2);
			dy1 = vis_faligndata(dy1, dy2);
			dx3 = vis_ld_d64_nf(dpsrc1 + 2);
			dy3 = vis_ld_d64_nf(dpsrc2 + 3);
			dy2 = vis_faligndata(dy2, dy3);

			compens = 0;

			while (num_row >= 12) {
				size = num_row > 196608 ? 196608 : num_row;
				if ((((mlib_addr)psrc1 ^ (mlib_addr)psrc2)
					& 7) == 0) {
					for (j = 0; j <= (size - 12); j += 12) {
						INTER_MUL_S16_S32_3
						IMG1_SELF_MUL_S16_S32_3
						IMG1_SUM_S32_D64
						dpsrc1 += 3;
						dpsrc2 += 3;
						dx = vis_ld_d64_nf(dpsrc1);
						dy = vis_ld_d64_nf(dpsrc2);

						dx2 = dpsrc1[1];
						dy1 = dpsrc2[1];
						dx3 = dpsrc1[2];
						dy2 = dpsrc2[2];

						compens += 2;
					}
				} else {
					for (j = 0; j <= (size - 12); j += 12) {
						INTER_MUL_S16_S32_3
						IMG1_SELF_MUL_S16_S32_3
						IMG1_SUM_S32_D64

						dpsrc1 += 3;
						dpsrc2 += 3;
						dx = vis_ld_d64_nf(dpsrc1);
						dy1 = vis_ld_d64_nf(dpsrc2 + 1);
						dy = vis_faligndata(dy3, dy1);

						dx2 = vis_ld_d64_nf(dpsrc1 + 1);
						dy2 = vis_ld_d64_nf(dpsrc2 + 2);
						dy1 = vis_faligndata(dy1, dy2);
						dx3 = vis_ld_d64_nf(dpsrc1 + 2);
						dy3 = vis_ld_d64_nf(dpsrc2 + 3);
						dy2 = vis_faligndata(dy2, dy3);

						compens += 2;
					}
				}
				num_row -= j;

				accumulator.db = tmpAcc11;
				sum11 += accumulator.two_int.int0;
				sum12 += accumulator.two_int.int1;
				accumulator.db = tmpAcc12;
				sum13 += accumulator.two_int.int0;
				sum11 += accumulator.two_int.int1;
				accumulator.db = tmpAcc13;
				sum12 += accumulator.two_int.int0;
				sum13 += accumulator.two_int.int1;

				rez[al2off[off + 0]] += ccInterAcc1;
				rez[al2off[off + 0] + 4] += ccSelfAcc11;
				rez[al2off[off + 0] + 12] += sum11;

				rez[al2off[off + 2]] += ccInterAcc2;
				rez[al2off[off + 2] + 4] += ccSelfAcc12;
				rez[al2off[off + 2] + 12] += sum12;

				rez[al2off[off + 1]] += ccInterAcc3;
				rez[al2off[off + 1] + 4] += ccSelfAcc13;
				rez[al2off[off + 1] + 12] += sum13;

				ccInterAcc1 = ccInterAcc2 = ccInterAcc3 = dzero;
				ccSelfAcc11 = ccSelfAcc12 = ccSelfAcc13 = dzero;
				sum11 = sum12 = sum13 = dzero;
				tmpAcc11 = tmpAcc12 = tmpAcc13 = dzero;
			}


			if (num_row > 0) {
				midd[0] = midd[1] = midd[2] = dzero;
				emask = 0xfff000 >> num_row;

				vis_pst_16(dy, &midd[0], emask >> 8);
				dy = midd[0];
				vis_pst_16(dy1, &midd[1], emask >> 4);
				dy1 = midd[1];
				vis_pst_16(dy2, &midd[2], emask);
				dy2 = midd[2];

				midd[0] = midd[1] = midd[2] = dzero;
				vis_pst_16(dx, &midd[0], emask >> 8);
				dx = midd[0];
				vis_pst_16(dx2, &midd[1], emask >> 4);
				dx2 = midd[1];
				vis_pst_16(dx3, &midd[2], emask);
				dx3 = midd[2];

				INTER_MUL_S16_S32_3
				IMG1_SELF_MUL_S16_S32_3
				IMG1_SUM_S32_D64
				compens += 2;

				accumulator.db = tmpAcc11;
				sum11 += accumulator.two_int.int0;
				sum12 += accumulator.two_int.int1;
				accumulator.db = tmpAcc12;
				sum13 += accumulator.two_int.int0;
				sum11 += accumulator.two_int.int1;
				accumulator.db = tmpAcc13;
				sum12 += accumulator.two_int.int0;
				sum13 += accumulator.two_int.int1;

				rez[al2off[off + 0]] += ccInterAcc1;
				rez[al2off[off + 0] + 4] += ccSelfAcc11;
				rez[al2off[off + 0] + 12] += sum11;

				rez[al2off[off + 2]] += ccInterAcc2;
				rez[al2off[off + 2] + 4] += ccSelfAcc12;
				rez[al2off[off + 2] + 12] += sum12;

				rez[al2off[off + 1]] += ccInterAcc3;
				rez[al2off[off + 1] + 4] += ccSelfAcc13;
				rez[al2off[off + 1] + 12] += sum13;

				tmpAcc11 = tmpAcc12 = tmpAcc13 = dzero;

			}

			rez[al2off[off + 0]] += compens;
			rez[al2off[off + 0] + 4] += compens;

			rez[al2off[off + 2]] += compens;
			rez[al2off[off + 2] + 4] += compens;

			rez[al2off[off + 1]] += compens;
			rez[al2off[off + 1] + 4] += compens;

			psrc1 += src1_stride;
			psrc2 += src2_stride;
		}
	} else {
		for (i = 0; i < height; i++) {
			midd[0] = midd[1] = midd[2] = dzero;
			ccInterAcc1 = ccInterAcc2 = ccInterAcc3 = dzero;
			ccSelfAcc11 = ccSelfAcc12 = ccSelfAcc13 = dzero;
			ccSelfAcc21 = ccSelfAcc22 = ccSelfAcc23 = dzero;
			sum11 = sum12 = sum13 = dzero;
			sum21 = sum22 = sum23 = dzero;
			tmpAcc11 = tmpAcc12 = tmpAcc13 = dzero;
			tmpAcc21 = tmpAcc22 = tmpAcc23 = dzero;

	/* prepare the source address */
			off = (mlib_addr)psrc1 & 7;
			off >>= 1;
			emask = vis_edge16(psrc1, psrc1 + size_row - 1);
			num_row = size_row + off;
			dpsrc1 = (mlib_d64 *)(psrc1 - off);
			dpsrc2 = (mlib_d64 *)vis_alignaddr((psrc2 - off), 0);

			vis_pst_16(dpsrc1[0], &midd[0], emask);
			dx = midd[0];
			midd[0] = dzero;

			dy = vis_ld_d64_nf(dpsrc2);
			dy1 = vis_ld_d64_nf(dpsrc2 + 1);
			dy = vis_faligndata(dy, dy1);
			vis_pst_16(dy, &midd[0], emask);
			dy = midd[0];

			dx2 = vis_ld_d64_nf(dpsrc1 + 1);
			dy2 = vis_ld_d64_nf(dpsrc2 + 2);
			dy1 = vis_faligndata(dy1, dy2);
			dx3 = vis_ld_d64_nf(dpsrc1 + 2);
			dy3 = vis_ld_d64_nf(dpsrc2 + 3);
			dy2 = vis_faligndata(dy2, dy3);

			compens = 0;

			while (num_row >= 12) {
				size = num_row > 196608 ? 196608 : num_row;
				if ((((mlib_addr)psrc1 ^ (mlib_addr)psrc2)
					& 7) == 0) {
					for (j = 0; j <= (size - 12); j += 12) {
						INTER_MUL_S16_S32_3
						IMG1_SELF_MUL_S16_S32_3
						IMG2_SELF_MUL_S16_S32_3
						IMG1_SUM_S32_D64
						IMG2_SUM_S32_D64
						dpsrc1 += 3;
						dpsrc2 += 3;
						dx = vis_ld_d64_nf(dpsrc1);
						dy = vis_ld_d64_nf(dpsrc2);

						dx2 = dpsrc1[1];
						dy1 = dpsrc2[1];
						dx3 = dpsrc1[2];
						dy2 = dpsrc2[2];

						compens += 2;
					}
				} else {
					for (j = 0; j <= (size - 12); j += 12) {
						INTER_MUL_S16_S32_3
						IMG1_SELF_MUL_S16_S32_3
						IMG2_SELF_MUL_S16_S32_3
						IMG1_SUM_S32_D64
						IMG2_SUM_S32_D64

						dpsrc1 += 3;
						dpsrc2 += 3;
						dx = vis_ld_d64_nf(dpsrc1);
						dy1 = vis_ld_d64_nf(dpsrc2 + 1);
						dy = vis_faligndata(dy3, dy1);

						dx2 = vis_ld_d64_nf(dpsrc1 + 1);
						dy2 = vis_ld_d64_nf(dpsrc2 + 2);
						dy1 = vis_faligndata(dy1, dy2);
						dx3 = vis_ld_d64_nf(dpsrc1 + 2);
						dy3 = vis_ld_d64_nf(dpsrc2 + 3);
						dy2 = vis_faligndata(dy2, dy3);

						compens += 2;
					}
				}
				num_row -= j;

				accumulator.db = tmpAcc11;
				sum11 += accumulator.two_int.int0;
				sum12 += accumulator.two_int.int1;
				accumulator.db = tmpAcc12;
				sum13 += accumulator.two_int.int0;
				sum11 += accumulator.two_int.int1;
				accumulator.db = tmpAcc13;
				sum12 += accumulator.two_int.int0;
				sum13 += accumulator.two_int.int1;

				accumulator.db = tmpAcc21;
				sum21 += accumulator.two_int.int0;
				sum22 += accumulator.two_int.int1;
				accumulator.db = tmpAcc22;
				sum23 += accumulator.two_int.int0;
				sum21 += accumulator.two_int.int1;
				accumulator.db = tmpAcc23;
				sum22 += accumulator.two_int.int0;
				sum23 += accumulator.two_int.int1;

				tmpAcc21 = tmpAcc22 = tmpAcc23 = dzero;
				tmpAcc11 = tmpAcc12 = tmpAcc13 = dzero;

				rez[al2off[off + 0]] += ccInterAcc1;
				rez[al2off[off + 0] + 4] += ccSelfAcc11;
				rez[al2off[off + 0] + 8] += ccSelfAcc21;
				rez[al2off[off + 0] + 12] += sum11;
				rez[al2off[off + 0] + 16] += sum21;

				rez[al2off[off + 2]] += ccInterAcc2;
				rez[al2off[off + 2] + 4] += ccSelfAcc12;
				rez[al2off[off + 2] + 8] += ccSelfAcc22;
				rez[al2off[off + 2] + 12] += sum12;
				rez[al2off[off + 2] + 16] += sum22;

				rez[al2off[off + 1]] += ccInterAcc3;
				rez[al2off[off + 1] + 4] += ccSelfAcc13;
				rez[al2off[off + 1] + 8] += ccSelfAcc23;
				rez[al2off[off + 1] + 12] += sum13;
				rez[al2off[off + 1] + 16] += sum23;

				ccInterAcc1 = ccInterAcc2 = ccInterAcc3 = dzero;
				ccSelfAcc11 = ccSelfAcc12 = ccSelfAcc13 = dzero;
				ccSelfAcc21 = ccSelfAcc22 = ccSelfAcc23 = dzero;
				sum11 = sum12 = sum13 = dzero;
				sum21 = sum22 = sum23 = dzero;
				tmpAcc11 = tmpAcc12 = tmpAcc13 = dzero;
				tmpAcc21 = tmpAcc22 = tmpAcc23 = dzero;

			}

			if (num_row > 0) {
				midd[0] = midd[1] = midd[2] = dzero;
				emask = 0xfff000 >> num_row;

				vis_pst_16(dy, &midd[0], emask >> 8);
				dy = midd[0];
				vis_pst_16(dy1, &midd[1], emask >> 4);
				dy1 = midd[1];
				vis_pst_16(dy2, &midd[2], emask);
				dy2 = midd[2];

				midd[0] = midd[1] = midd[2] = dzero;
				vis_pst_16(dx, &midd[0], emask >> 8);
				dx = midd[0];
				vis_pst_16(dx2, &midd[1], emask >> 4);
				dx2 = midd[1];
				vis_pst_16(dx3, &midd[2], emask);
				dx3 = midd[2];

				INTER_MUL_S16_S32_3
				IMG1_SELF_MUL_S16_S32_3
				IMG2_SELF_MUL_S16_S32_3
				IMG1_SUM_S32_D64
				IMG2_SUM_S32_D64
				compens += 2;

				accumulator.db = tmpAcc11;
				sum11 += accumulator.two_int.int0;
				sum12 += accumulator.two_int.int1;
				accumulator.db = tmpAcc12;
				sum13 += accumulator.two_int.int0;
				sum11 += accumulator.two_int.int1;
				accumulator.db = tmpAcc13;
				sum12 += accumulator.two_int.int0;
				sum13 += accumulator.two_int.int1;

				accumulator.db = tmpAcc21;
				sum21 += accumulator.two_int.int0;
				sum22 += accumulator.two_int.int1;
				accumulator.db = tmpAcc22;
				sum23 += accumulator.two_int.int0;
				sum21 += accumulator.two_int.int1;
				accumulator.db = tmpAcc23;
				sum22 += accumulator.two_int.int0;
				sum23 += accumulator.two_int.int1;

				tmpAcc21 = tmpAcc22 = tmpAcc23 = dzero;
				tmpAcc11 = tmpAcc12 = tmpAcc13 = dzero;

				rez[al2off[off + 0]] += ccInterAcc1;
				rez[al2off[off + 0] + 4] += ccSelfAcc11;
				rez[al2off[off + 0] + 8] += ccSelfAcc21;
				rez[al2off[off + 0] + 12] += sum11;
				rez[al2off[off + 0] + 16] += sum21;

				rez[al2off[off + 2]] += ccInterAcc2;
				rez[al2off[off + 2] + 4] += ccSelfAcc12;
				rez[al2off[off + 2] + 8] += ccSelfAcc22;
				rez[al2off[off + 2] + 12] += sum12;
				rez[al2off[off + 2] + 16] += sum22;

				rez[al2off[off + 1]] += ccInterAcc3;
				rez[al2off[off + 1] + 4] += ccSelfAcc13;
				rez[al2off[off + 1] + 8] += ccSelfAcc23;
				rez[al2off[off + 1] + 12] += sum13;
				rez[al2off[off + 1] + 16] += sum23;

			}

			rez[al2off[off + 0]] += compens;
			rez[al2off[off + 0] + 4] += compens;
			rez[al2off[off + 0] + 8] += compens;

			rez[al2off[off + 2]] += compens;
			rez[al2off[off + 2] + 4] += compens;
			rez[al2off[off + 2] + 8] += compens;

			rez[al2off[off + 1]] += compens;
			rez[al2off[off + 1] + 4] += compens;
			rez[al2off[off + 1] + 8] += compens;

			psrc1 += src1_stride;
			psrc2 += src2_stride;
		}
	}

}

/* *********************************************************** */

#define	MUL_S32(x, y)	((x) * (mlib_d64) (y))

/* *********************************************************** */

void
mlib_v_ImageNormCrossCorrel_S32_124(
    const mlib_image *img1,
    const mlib_image *img2,
	const mlib_d64 *m2,
	const mlib_d64 *s2,
    mlib_d64 *rez)
{
/* pointer to the data of first image */
	mlib_s32 *psrc1 = (mlib_s32 *)mlib_ImageGetData(img1);

/* pointer to the data of second image */
	mlib_s32 *psrc2 = (mlib_s32 *)mlib_ImageGetData(img2);

/* accumulators by channels */
	mlib_d64 ccInterMul1, ccInterMul2, ccInterMul3, ccInterMul4;
	mlib_d64 ccSelfMul11, ccSelfMul12, ccSelfMul13, ccSelfMul14;
	mlib_d64 ccSelfMul21, ccSelfMul22, ccSelfMul23, ccSelfMul24;
	mlib_d64 sum11, sum12, sum13, sum14;
	mlib_d64 sum21, sum22, sum23, sum24;

/* height of image */
	mlib_s32 height = mlib_ImageGetHeight(img1);

/* elements to next row */
	mlib_s32 src1_stride = mlib_ImageGetStride(img1) >> 2;

/* elements to next row */
	mlib_s32 src2_stride = mlib_ImageGetStride(img2) >> 2;
	mlib_s32 nchan = mlib_ImageGetChannels(img1);

/* number of elements in the row */
	mlib_s32 size_row = mlib_ImageGetWidth(img1) << (nchan >> 1);

/* indices */
	mlib_s32 i, j, k;

	const mlib_d64 dzero = vis_fzero();

	if (src1_stride == size_row && src1_stride == src2_stride) {
/* This image is not a sub-image and can be treated as a 1-D vector */
		size_row *= height;
		height = 1;
	}

	ccInterMul1 = ccInterMul2 = ccInterMul3 = ccInterMul4 = dzero;
	ccSelfMul11 = ccSelfMul12 = ccSelfMul13 = ccSelfMul14 = dzero;
	ccSelfMul21 = ccSelfMul22 = ccSelfMul23 = ccSelfMul24 = dzero;
	sum11 = sum12 = sum13 = sum14 = dzero;
	sum21 = sum22 = sum23 = sum24 = dzero;

	if (m2 != NULL && s2 != NULL) {
		for (i = 0; i < height; i++) {

			for (j = 0; j <= (size_row - 4); j += 4) {
				ccInterMul1 +=
					MUL_S32(psrc1[j], psrc2[j]);
				ccSelfMul11 +=
					MUL_S32(psrc1[j], psrc1[j]);
				sum11 += psrc1[j];

				ccInterMul2 +=
					MUL_S32(psrc1[j + 1], psrc2[j + 1]);
				ccSelfMul12 +=
					MUL_S32(psrc1[j + 1], psrc1[j + 1]);
				sum12 += psrc1[j + 1];

				ccInterMul3 +=
					MUL_S32(psrc1[j + 2], psrc2[j + 2]);
				ccSelfMul13 +=
					MUL_S32(psrc1[j + 2], psrc1[j + 2]);
				sum13 += psrc1[j + 2];

				ccInterMul4 +=
					MUL_S32(psrc1[j + 3], psrc2[j + 3]);
				ccSelfMul14 +=
					MUL_S32(psrc1[j + 3], psrc1[j + 3]);
				sum14 += psrc1[j + 3];

			}

			for (k = 0; j < size_row; j++, k++) {
				rez[k] += MUL_S32(psrc1[j], psrc2[j]);
				rez[k + 4] += MUL_S32(psrc1[j], psrc1[j]);
				rez[k + 12] += psrc1[j];

			}

			psrc1 += src1_stride;
			psrc2 += src2_stride;
		}

		rez[0] += ccInterMul1;
		rez[4] += ccSelfMul11;
		rez[12] += sum11;

		rez[1] += ccInterMul2;
		rez[5] += ccSelfMul12;
		rez[13] += sum12;

		rez[2] += ccInterMul3;
		rez[6] += ccSelfMul13;
		rez[14] += sum13;

		rez[3] += ccInterMul4;
		rez[7] += ccSelfMul14;
		rez[15] += sum14;

	} else {
		for (i = 0; i < height; i++) {

			for (j = 0; j <= (size_row - 4); j += 4) {

				ccInterMul1 +=
					MUL_S32(psrc1[j], psrc2[j]);
				ccSelfMul11 +=
					MUL_S32(psrc1[j], psrc1[j]);
				ccSelfMul21 +=
					MUL_S32(psrc2[j], psrc2[j]);
				sum11 += psrc1[j];
				sum21 += psrc2[j];

				ccInterMul2 +=
					MUL_S32(psrc1[j + 1], psrc2[j + 1]);
				ccSelfMul12 +=
					MUL_S32(psrc1[j + 1], psrc1[j + 1]);
				ccSelfMul22 +=
					MUL_S32(psrc2[j + 1], psrc2[j + 1]);
				sum12 += psrc1[j + 1];
				sum22 += psrc2[j + 1];

				ccInterMul3 +=
					MUL_S32(psrc1[j + 2], psrc2[j + 2]);
				ccSelfMul13 +=
					MUL_S32(psrc1[j + 2], psrc1[j + 2]);
				ccSelfMul23 +=
					MUL_S32(psrc2[j + 2], psrc2[j + 2]);
				sum13 += psrc1[j + 2];
				sum23 += psrc2[j + 2];

				ccInterMul4 +=
					MUL_S32(psrc1[j + 3], psrc2[j + 3]);
				ccSelfMul14 +=
					MUL_S32(psrc1[j + 3], psrc1[j + 3]);
				ccSelfMul24 +=
					MUL_S32(psrc2[j + 3], psrc2[j + 3]);
				sum14 += psrc1[j + 3];
				sum24 += psrc2[j + 3];

			}

			for (k = 0; j < size_row; j++, k++) {

				rez[k] += MUL_S32(psrc1[j], psrc2[j]);
				rez[k + 4] += MUL_S32(psrc1[j], psrc1[j]);
				rez[k + 8] += MUL_S32(psrc2[j], psrc2[j]);
				rez[k + 12] += psrc1[j];
				rez[k + 16] += psrc2[j];

			}
			psrc1 += src1_stride;
			psrc2 += src2_stride;
		}

		rez[0] += ccInterMul1;
		rez[4] += ccSelfMul11;
		rez[8] += ccSelfMul21;
		rez[12] += sum11;
		rez[16] += sum21;

		rez[1] += ccInterMul2;
		rez[5] += ccSelfMul12;
		rez[9] += ccSelfMul22;
		rez[13] += sum12;
		rez[17] += sum22;

		rez[2] += ccInterMul3;
		rez[6] += ccSelfMul13;
		rez[10] += ccSelfMul23;
		rez[14] += sum13;
		rez[18] += sum23;

		rez[3] += ccInterMul4;
		rez[7] += ccSelfMul14;
		rez[11] += ccSelfMul24;
		rez[15] += sum14;
		rez[19] += sum24;

	}
}

/* *********************************************************** */

void
mlib_v_ImageNormCrossCorrel_S32_3(
    const mlib_image *img1,
    const mlib_image *img2,
	const mlib_d64 *m2,
	const mlib_d64 *s2,
    mlib_d64 *rez)
{
/* pointer to the data of first image */
	mlib_s32 *psrc1 = (mlib_s32 *)mlib_ImageGetData(img1);

/* pointer to the data of second image */
	mlib_s32 *psrc2 = (mlib_s32 *)mlib_ImageGetData(img2);

/* accumulators by channels */
	mlib_d64 c1, c2, c3;

	mlib_d64 ccInterMul1, ccInterMul2, ccInterMul3;
	mlib_d64 ccSelfMul11, ccSelfMul12, ccSelfMul13;
	mlib_d64 ccSelfMul21, ccSelfMul22, ccSelfMul23;
	mlib_d64 sum11, sum12, sum13;
	mlib_d64 sum21, sum22, sum23;

/* height of image */
	mlib_s32 height = mlib_ImageGetHeight(img1);

/* elements to next row */
	mlib_s32 src1_stride = mlib_ImageGetStride(img1) >> 2;

/* elements to next row */
	mlib_s32 src2_stride = mlib_ImageGetStride(img2) >> 2;

/* number of elements in the row */
	mlib_s32 size_row = mlib_ImageGetWidth(img1) * 3;

/* indices */
	mlib_s32 i, j, k;

	const mlib_d64 dzero = vis_fzero();

	if (src1_stride == size_row && src1_stride == src2_stride) {
/* This image is not a sub-image and can be treated as a 1-D vector */
		size_row *= height;
		height = 1;
	}

	ccInterMul1 = ccInterMul2 = ccInterMul3 = dzero;
	ccSelfMul11 = ccSelfMul12 = ccSelfMul13 = dzero;
	ccSelfMul21 = ccSelfMul22 = ccSelfMul23 = dzero;
	sum11 = sum12 = sum13 = dzero;
	sum21 = sum22 = sum23 = dzero;

	if (m2 != NULL && s2 != NULL) {
		for (i = 0; i < height; i++) {
			for (j = 0; j < size_row; j += 3) {

				ccInterMul1 +=
					MUL_S32(psrc1[j], psrc2[j]);
				ccSelfMul11 +=
					MUL_S32(psrc1[j], psrc1[j]);
				sum11 += psrc1[j];

				ccInterMul2 +=
					MUL_S32(psrc1[j + 1], psrc2[j + 1]);
				ccSelfMul12 +=
					MUL_S32(psrc1[j + 1], psrc1[j + 1]);
				sum12 += psrc1[j + 1];

				ccInterMul3 +=
					MUL_S32(psrc1[j + 2], psrc2[j + 2]);
				ccSelfMul13 +=
					MUL_S32(psrc1[j + 2], psrc1[j + 2]);
				sum13 += psrc1[j + 2];

			}

			psrc1 += src1_stride;
			psrc2 += src2_stride;
		}

		rez[0] += ccInterMul1;
		rez[4] += ccSelfMul11;
		rez[12] += sum11;

		rez[1] += ccInterMul2;
		rez[5] += ccSelfMul12;
		rez[13] += sum12;

		rez[2] += ccInterMul3;
		rez[6] += ccSelfMul13;
		rez[14] += sum13;

	} else {
		for (i = 0; i < height; i++) {
			for (j = 0; j < size_row; j += 3) {

				ccInterMul1 +=
					MUL_S32(psrc1[j], psrc2[j]);
				ccSelfMul11 +=
					MUL_S32(psrc1[j], psrc1[j]);
				ccSelfMul21 +=
					MUL_S32(psrc2[j], psrc2[j]);
				sum11 += psrc1[j];
				sum21 += psrc2[j];

				ccInterMul2 +=
					MUL_S32(psrc1[j + 1], psrc2[j + 1]);
				ccSelfMul12 +=
					MUL_S32(psrc1[j + 1], psrc1[j + 1]);
				ccSelfMul22 +=
					MUL_S32(psrc2[j + 1], psrc2[j + 1]);
				sum12 += psrc1[j + 1];
				sum22 += psrc2[j + 1];

				ccInterMul3 +=
					MUL_S32(psrc1[j + 2], psrc2[j + 2]);
				ccSelfMul13 +=
					MUL_S32(psrc1[j + 2], psrc1[j + 2]);
				ccSelfMul23 +=
					MUL_S32(psrc2[j + 2], psrc2[j + 2]);
				sum13 += psrc1[j + 2];
				sum23 += psrc2[j + 2];

			}

			psrc1 += src1_stride;
			psrc2 += src2_stride;
		}

		rez[0] += ccInterMul1;
		rez[4] += ccSelfMul11;
		rez[8] += ccSelfMul21;
		rez[12] += sum11;
		rez[16] += sum21;

		rez[1] += ccInterMul2;
		rez[5] += ccSelfMul12;
		rez[9] += ccSelfMul22;
		rez[13] += sum12;
		rez[17] += sum22;

		rez[2] += ccInterMul3;
		rez[6] += ccSelfMul13;
		rez[10] += ccSelfMul23;
		rez[14] += sum13;
		rez[18] += sum23;
	}

}

/* *********************************************************** */
