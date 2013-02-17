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

#pragma ident	"@(#)mlib_ImageNormCrossCorrel_Fp.c	9.3	07/10/09"


/*
 * FUNCTION
 *      mlib_ImageNormCrossCorrel_Fp - calculates normalized cross-correlation
 *                                     coefficient of two input images
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageNormCrossCorrel_Fp(mlib_d64         *correl,
 *                                               const mlib_image *img1,
 *                                               const mlib_image *img2,
 *                                               const mlib_d64   *mean2,
 *                                               const mlib_d64   *sdev2);
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
 *      In some cases, the result of this function could be NaN, Inf, or -Inf.
 *
 * RESTRICTION
 *      The images must have the same type, the same size, and the same number
 *      of channels.
 *      They can have 1, 2, 3 or 4 channels.
 *      They can be of MLIB_FLOAT or MLIB_DOUBLE data type.
 *
 *      If (mean2 == NULL) or (sdev2 == NULL), then m2 and s2 are calculated in
 *      this function according to the formulas shown above.  Otherwise, they
 *      are calculated as follows:
 *
 *      m2[i] = mean2[i];
 *      s2[i] = sdev2[i] * sqrt(w*h);
 *
 *      where mean2 and sdev2 can be the output of mlib_ImageMean_Fp() and
 *      mlib_ImageStdDev_Fp(), respectively.
 */

#include <mlib_image.h>
#include <mlib_ImageCheck.h>
#include <mlib_ImageCorrel.h>
#include <mlib_SysMath.h>

/* *********************************************************** */

#define	EPS	1e-9

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageNormCrossCorrel_Fp = __mlib_ImageNormCrossCorrel_Fp

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_ImageNormCrossCorrel_Fp) mlib_ImageNormCrossCorrel_Fp
    __attribute__((weak, alias("__mlib_ImageNormCrossCorrel_Fp")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

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
__mlib_ImageNormCrossCorrel_Fp(
    mlib_d64 *correl,
    const mlib_image *img1,
    const mlib_image *img2,
    const mlib_d64   *mean2,
    const mlib_d64   *sdev2)
{
	mlib_s32 num_pixel;
	mlib_d64 rez[20];
	mlib_d64 m1, m2, s1, s2;
	mlib_d64 divider;
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
	divider = 1.0 / (mlib_d64)num_pixel;
	if ((mean2 != NULL) && (sdev2 != NULL)) {
		scale = mlib_sqrt((mlib_d64)num_pixel);
	}

	switch (mlib_ImageGetType(img1)) {
	case MLIB_FLOAT:

		if (mlib_ImageGetChannels(img1) == 3)
			mlib_ImageNormCrossCorrel_F32_3(img1, img2, rez,
				mean2, sdev2);
		else
			mlib_ImageNormCrossCorrel_F32_124(img1, img2, rez,
				mean2, sdev2);
		break;

	case MLIB_DOUBLE:

		if (mlib_ImageGetChannels(img1) == 3)
			mlib_ImageNormCrossCorrel_D64_3(img1, img2, rez,
				mean2, sdev2);
		else
			mlib_ImageNormCrossCorrel_D64_124(img1, img2, rez,
				mean2, sdev2);
		break;

	default:
		return (MLIB_FAILURE);
	}

	switch (mlib_ImageGetChannels(img1)) {
	case 1:
		m1 = rez[4] + rez[5] + rez[6] + rez[7];
		x = rez[12] + rez[13] + rez[14] + rez[15];
		y = m1 * m1 * divider;
		Square_Root(s1, x, y);
		if ((mean2 != NULL) && (sdev2 != NULL)) {
			m2 = mean2[0] * num_pixel;
			s2 = sdev2[0] * scale;
		} else {
			m2 = rez[8] + rez[9] + rez[10] + rez[11];
			x = rez[16] + rez[17] + rez[18] + rez[19];
			y = m2 * m2 * divider;
			Square_Root(s2, x, y);
		}
		numerator = rez[0] + rez[1] + rez[2] + rez[3]
			- m1 * m2 * divider;
		NormCrossCorrel(correl[0], numerator, m1, m2, s1, s2);
		break;

	case 2:
		m1 = rez[4] + rez[6];
		x = rez[12] + rez[14];
		y = m1 * m1 * divider;
		Square_Root(s1, x, y);
		if ((mean2 != NULL) && (sdev2 != NULL)) {
			m2 = mean2[0] * num_pixel;
			s2 = sdev2[0] * scale;
		} else {
			m2 = rez[8] + rez[10];
			x = rez[16] + rez[18];
			y = m2 * m2 * divider;
			Square_Root(s2, x, y);
		}
		numerator = rez[0] + rez[2] - m1 * m2 * divider;
		NormCrossCorrel(correl[0], numerator, m1, m2, s1, s2);

		m1 = rez[5] + rez[7];
		x = rez[13] + rez[15];
		y = m1 * m1 * divider;
		Square_Root(s1, x, y);
		if ((mean2 != NULL) && (sdev2 != NULL)) {
			m2 = mean2[1] * num_pixel;
			s2 = sdev2[1] * scale;
		} else {
			m2 = rez[9] + rez[11];
			x = rez[17] + rez[19];
			y = m2 * m2 * divider;
			Square_Root(s2, x, y);
		}
		numerator = rez[1] + rez[3] - m1 * m2 * divider;
		NormCrossCorrel(correl[1], numerator, m1, m2, s1, s2);
		break;

	case 4:
		m1 = rez[7];
		x = rez[15];
		y = m1 * m1 * divider;
		Square_Root(s1, x, y);
		if ((mean2 != NULL) && (sdev2 != NULL)) {
			m2 = mean2[3] * num_pixel;
			s2 = sdev2[3] * scale;
		} else {
			m2 = rez[11];
			x = rez[19];
			y = m2 * m2 * divider;
			Square_Root(s2, x, y);
		}
		numerator = rez[3] - m1 * m2 * divider;
		NormCrossCorrel(correl[3], numerator, m1, m2, s1, s2);
	case 3:
		m1 = rez[4];
		x = rez[12];
		y = m1 * m1 * divider;
		Square_Root(s1, x, y);
		if ((mean2 != NULL) && (sdev2 != NULL)) {
			m2 = mean2[0] * num_pixel;
			s2 = sdev2[0] * scale;
		} else {
			m2 = rez[8];
			x = rez[16];
			y = m2 * m2 * divider;
			Square_Root(s2, x, y);
		}
		numerator = rez[0] - m1 * m2 * divider;
		NormCrossCorrel(correl[0], numerator, m1, m2, s1, s2);

		m1 = rez[5];
		x = rez[13];
		y = m1 * m1 * divider;
		Square_Root(s1, x, y);
		if ((mean2 != NULL) && (sdev2 != NULL)) {
			m2 = mean2[1] * num_pixel;
			s2 = sdev2[1] * scale;
		} else {
			m2 = rez[9];
			x = rez[17];
			y = m2 * m2 * divider;
			Square_Root(s2, x, y);
		}
		numerator = rez[1] - m1 * m2 * divider;
		NormCrossCorrel(correl[1], numerator, m1, m2, s1, s2);

		m1 = rez[6];
		x = rez[14];
		y = m1 * m1 * divider;
		Square_Root(s1, x, y);
		if ((mean2 != NULL) && (sdev2 != NULL)) {
			m2 = mean2[2] *num_pixel;
			s2 = sdev2[2] * scale;
		} else {
			m2 = rez[10];
			x = rez[18];
			y = m2 * m2 * divider;
			Square_Root(s2, x, y);
		}
		numerator = rez[2] - m1 * m2 * divider;
		NormCrossCorrel(correl[2], numerator, m1, m2, s1, s2);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#define	MUL_D64(x, y)	((x) * (y))

/* *********************************************************** */

void
mlib_ImageNormCrossCorrel_D64_124(
    const mlib_image *img1,
    const mlib_image *img2,
    mlib_d64 *rez,
    const mlib_d64 *m2,
    const mlib_d64 *s2)
{
	mlib_d64 *psrc1 = (mlib_d64 *)mlib_ImageGetData(img1);
	mlib_d64 *psrc2 = (mlib_d64 *)mlib_ImageGetData(img2);

/* accumulators by channels */
	mlib_d64 c1, c2, c3, c4;
	mlib_d64 m11, m12, m13, m14;
	mlib_d64 m21, m22, m23, m24;
	mlib_d64 s11, s12, s13, s14;
	mlib_d64 s21, s22, s23, s24;

/* height of image */
	mlib_s32 height = mlib_ImageGetHeight(img1);

/* elements to next row */
	mlib_s32 src1_stride = mlib_ImageGetStride(img1) >> 3;

/* elements to next row */
	mlib_s32 src2_stride = mlib_ImageGetStride(img2) >> 3;
	mlib_s32 nchan = mlib_ImageGetChannels(img1);

/* number of elements in the row */
	mlib_s32 size_row = mlib_ImageGetWidth(img1) << (nchan >> 1);

/* indices */
	mlib_s32 i, j, k;

	if (src1_stride == size_row && src1_stride == src2_stride) {
/* This images is not a sub-images and can be treated as a 1-D vectors */
		size_row *= height;
		height = 1;
	}

	if ((m2 != NULL) && (s2 != NULL)) {
		/* use the user-provided m2 and s2 */
		c1 = c2 = c3 = c4 = 0.0;
		m11 = m12 = m13 = m14 = 0.0;
		s11 = s12 = s13 = s14 = 0.0;
		rez[0] = rez[1] = rez[2] = 0.0;
		rez[4] = rez[5] = rez[6] = 0.0;
		rez[12] = rez[13] = rez[14] = 0.0;
		for (i = 0; i < height; i++) {
			for (j = 0; j <= (size_row - 4); j += 4) {
				c1 += MUL_D64(psrc1[j], psrc2[j]);
				c2 += MUL_D64(psrc1[j + 1], psrc2[j + 1]);
				c3 += MUL_D64(psrc1[j + 2], psrc2[j + 2]);
				c4 += MUL_D64(psrc1[j + 3], psrc2[j + 3]);
				m11 += psrc1[j];
				m12 += psrc1[j + 1];
				m13 += psrc1[j + 2];
				m14 += psrc1[j + 3];
				s11 += MUL_D64(psrc1[j], psrc1[j]);
				s12 += MUL_D64(psrc1[j + 1], psrc1[j + 1]);
				s13 += MUL_D64(psrc1[j + 2], psrc1[j + 2]);
				s14 += MUL_D64(psrc1[j + 3], psrc1[j + 3]);
			}

			for (k = 0; j < size_row; j++, k++) {
				rez[k] += MUL_D64(psrc1[j], psrc2[j]);
				rez[k + 4] += psrc1[j];
				rez[k + 12] += MUL_D64(psrc1[j], psrc1[j]);
			}
			psrc1 += src1_stride;
			psrc2 += src2_stride;
		}

		rez[0] += c1;
		rez[1] += c2;
		rez[2] += c3;
		rez[3] = c4;
		rez[4] += m11;
		rez[5] += m12;
		rez[6] += m13;
		rez[7] = m14;
		rez[12] += s11;
		rez[13] += s12;
		rez[14] += s13;
		rez[15] = s14;
	} else {
		/* calculate m2 and s2 on the spot */
		c1 = c2 = c3 = c4 = 0.0;
		m11 = m12 = m13 = m14 = 0.0;
		m21 = m22 = m23 = m24 = 0.0;
		s11 = s12 = s13 = s14 = 0.0;
		s21 = s22 = s23 = s24 = 0.0;
		rez[0] = rez[1] = rez[2] = 0.0;
		rez[4] = rez[5] = rez[6] = 0.0;
		rez[8] = rez[9] = rez[10] = 0.0;
		rez[12] = rez[13] = rez[14] = 0.0;
		rez[16] = rez[17] = rez[18] = 0.0;
		for (i = 0; i < height; i++) {
			for (j = 0; j <= (size_row - 4); j += 4) {
				c1 += MUL_D64(psrc1[j], psrc2[j]);
				c2 += MUL_D64(psrc1[j + 1], psrc2[j + 1]);
				c3 += MUL_D64(psrc1[j + 2], psrc2[j + 2]);
				c4 += MUL_D64(psrc1[j + 3], psrc2[j + 3]);
				m11 += psrc1[j];
				m12 += psrc1[j + 1];
				m13 += psrc1[j + 2];
				m14 += psrc1[j + 3];
				m21 += psrc2[j];
				m22 += psrc2[j + 1];
				m23 += psrc2[j + 2];
				m24 += psrc2[j + 3];
				s11 += MUL_D64(psrc1[j], psrc1[j]);
				s12 += MUL_D64(psrc1[j + 1], psrc1[j + 1]);
				s13 += MUL_D64(psrc1[j + 2], psrc1[j + 2]);
				s14 += MUL_D64(psrc1[j + 3], psrc1[j + 3]);
				s21 += MUL_D64(psrc2[j], psrc2[j]);
				s22 += MUL_D64(psrc2[j + 1], psrc2[j + 1]);
				s23 += MUL_D64(psrc2[j + 2], psrc2[j + 2]);
				s24 += MUL_D64(psrc2[j + 3], psrc2[j + 3]);
			}

			for (k = 0; j < size_row; j++, k++) {
				rez[k] += MUL_D64(psrc1[j], psrc2[j]);
				rez[k + 4] += psrc1[j];
				rez[k + 8] += psrc2[j];
				rez[k + 12] += MUL_D64(psrc1[j], psrc1[j]);
				rez[k + 16] += MUL_D64(psrc2[j], psrc2[j]);
			}
			psrc1 += src1_stride;
			psrc2 += src2_stride;
		}

		rez[0] += c1;
		rez[1] += c2;
		rez[2] += c3;
		rez[3] = c4;
		rez[4] += m11;
		rez[5] += m12;
		rez[6] += m13;
		rez[7] = m14;
		rez[8] += m21;
		rez[9] += m22;
		rez[10] += m23;
		rez[11] = m24;
		rez[12] += s11;
		rez[13] += s12;
		rez[14] += s13;
		rez[15] = s14;
		rez[16] += s21;
		rez[17] += s22;
		rez[18] += s23;
		rez[19] = s24;
	}
}

/* *********************************************************** */

void
mlib_ImageNormCrossCorrel_D64_3(
    const mlib_image *img1,
    const mlib_image *img2,
    mlib_d64 *rez,
    const mlib_d64 *m2,
    const mlib_d64 *s2)
{
	mlib_d64 *psrc1 = (mlib_d64 *)mlib_ImageGetData(img1);
	mlib_d64 *psrc2 = (mlib_d64 *)mlib_ImageGetData(img2);

/* accumulators by channels */
	mlib_d64 c1, c2, c3;
	mlib_d64 m11, m12, m13;
	mlib_d64 m21, m22, m23;
	mlib_d64 s11, s12, s13;
	mlib_d64 s21, s22, s23;

/* height of image */
	mlib_s32 height = mlib_ImageGetHeight(img1);

/* elements to next row */
	mlib_s32 src1_stride = mlib_ImageGetStride(img1) >> 3;

/* elements to next row */
	mlib_s32 src2_stride = mlib_ImageGetStride(img2) >> 3;

/* number of elements in the row */
	mlib_s32 size_row = mlib_ImageGetWidth(img1) * 3;

/* indices */
	mlib_s32 i, j;

	if (src1_stride == size_row && src1_stride == src2_stride) {
/* This images is not a sub-images and can be treated as a 1-D vectors */
		size_row *= height;
		height = 1;
	}

	if ((m2 != NULL) && (s2 != NULL)) {
		/* use the user-provided m2 and s2 */
		c1 = c2 = c3 = 0.0;
		m11 = m12 = m13 = 0.0;
		s11 = s12 = s13 = 0.0;
		for (i = 0; i < height; i++) {
			for (j = 0; j < size_row; j += 3) {
				c1 += MUL_D64(psrc1[j], psrc2[j]);
				c2 += MUL_D64(psrc1[j + 1], psrc2[j + 1]);
				c3 += MUL_D64(psrc1[j + 2], psrc2[j + 2]);
				m11 += psrc1[j];
				m12 += psrc1[j + 1];
				m13 += psrc1[j + 2];
				s11 += MUL_D64(psrc1[j], psrc1[j]);
				s12 += MUL_D64(psrc1[j + 1], psrc1[j + 1]);
				s13 += MUL_D64(psrc1[j + 2], psrc1[j + 2]);
			}

			psrc1 += src1_stride;
			psrc2 += src2_stride;
		}

		rez[0] = c1;
		rez[1] = c2;
		rez[2] = c3;
		rez[4] = m11;
		rez[5] = m12;
		rez[6] = m13;
		rez[12] = s11;
		rez[13] = s12;
		rez[14] = s13;
	} else {
		/* calculate m2 and s2 on the spot */
		c1 = c2 = c3 = 0.0;
		m11 = m12 = m13 = 0.0;
		m21 = m22 = m23 = 0.0;
		s11 = s12 = s13 = 0.0;
		s21 = s22 = s23 = 0.0;
		for (i = 0; i < height; i++) {
			for (j = 0; j < size_row; j += 3) {
				c1 += MUL_D64(psrc1[j], psrc2[j]);
				c2 += MUL_D64(psrc1[j + 1], psrc2[j + 1]);
				c3 += MUL_D64(psrc1[j + 2], psrc2[j + 2]);
				m11 += psrc1[j];
				m12 += psrc1[j + 1];
				m13 += psrc1[j + 2];
				m21 += psrc2[j];
				m22 += psrc2[j + 1];
				m23 += psrc2[j + 2];
				s11 += MUL_D64(psrc1[j], psrc1[j]);
				s12 += MUL_D64(psrc1[j + 1], psrc1[j + 1]);
				s13 += MUL_D64(psrc1[j + 2], psrc1[j + 2]);
				s21 += MUL_D64(psrc2[j], psrc2[j]);
				s22 += MUL_D64(psrc2[j + 1], psrc2[j + 1]);
				s23 += MUL_D64(psrc2[j + 2], psrc2[j + 2]);
			}

			psrc1 += src1_stride;
			psrc2 += src2_stride;
		}

		rez[0] = c1;
		rez[1] = c2;
		rez[2] = c3;
		rez[4] = m11;
		rez[5] = m12;
		rez[6] = m13;
		rez[8] = m21;
		rez[9] = m22;
		rez[10] = m23;
		rez[12] = s11;
		rez[13] = s12;
		rez[14] = s13;
		rez[16] = s21;
		rez[17] = s22;
		rez[18] = s23;
	}
}

/* *********************************************************** */

#define	MUL_F32(x, y)	((x) * (mlib_d64) (y))

/* *********************************************************** */

void
mlib_ImageNormCrossCorrel_F32_124(
    const mlib_image *img1,
    const mlib_image *img2,
    mlib_d64 *rez,
    const mlib_d64 *m2,
    const mlib_d64 *s2)
{
	mlib_f32 *psrc1 = (mlib_f32 *)mlib_ImageGetData(img1);
	mlib_f32 *psrc2 = (mlib_f32 *)mlib_ImageGetData(img2);

/* accumulators by channels */
	mlib_d64 c1, c2, c3, c4;
	mlib_d64 m11, m12, m13, m14;
	mlib_d64 m21, m22, m23, m24;
	mlib_d64 s11, s12, s13, s14;
	mlib_d64 s21, s22, s23, s24;

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
	mlib_s32 off;
	union
	{
		mlib_d64 d;
		mlib_f32 f[2];
	} dc1,
	    dc2,
	    dc3,
	    dc4;

	if (src1_stride == size_row && src1_stride == src2_stride) {
/* This images is not a sub-images and can be treated as a 1-D vectors */
		size_row *= height;
		height = 1;
	}

	if ((m2 != NULL) && (s2 != NULL)) {
		/* use the user-provided m2 and s2 */
		rez[0] = rez[1] = rez[2] = rez[3] = 0.0;
		rez[4] = rez[5] = rez[6] = rez[7] = 0.0;
		rez[12] = rez[13] = rez[14] = rez[15] = 0.0;
		for (i = 0; i < height; i++) {
			c1 = c2 = c3 = c4 = 0.0;
			m11 = m12 = m13 = m14 = 0.0;
			s11 = s12 = s13 = s14 = 0.0;
			off = (mlib_addr)psrc1 & 4;
			j = 0;

			if (off) {
				c4 += MUL_F32(psrc1[0], psrc2[0]);
				m14 += psrc1[0];
				s14 += MUL_F32(psrc1[0], psrc1[0]);
				j = 1;
			}

			if ((((mlib_addr)psrc1 ^ (mlib_addr)psrc2) & 7) == 0) {
				for (; j <= (size_row - 4); j += 4) {
					dc1.d = *(mlib_d64 *)&psrc1[j];
					dc2.d = *(mlib_d64 *)&psrc2[j];
					dc3.d = *(mlib_d64 *)&psrc1[j + 2];
					dc4.d = *(mlib_d64 *)&psrc2[j + 2];
					c1 += MUL_F32(dc1.f[0], dc2.f[0]);
					c2 += MUL_F32(dc1.f[1], dc2.f[1]);
					c3 += MUL_F32(dc3.f[0], dc4.f[0]);
					c4 += MUL_F32(dc3.f[1], dc4.f[1]);
					m11 += dc1.f[0];
					m12 += dc1.f[1];
					m13 += dc3.f[0];
					m14 += dc3.f[1];
					s11 += MUL_F32(dc1.f[0], dc1.f[0]);
					s12 += MUL_F32(dc1.f[1], dc1.f[1]);
					s13 += MUL_F32(dc3.f[0], dc3.f[0]);
					s14 += MUL_F32(dc3.f[1], dc3.f[1]);
				}
			} else {
				for (; j <= (size_row - 4); j += 4) {
					dc1.d = *(mlib_d64 *)&psrc1[j];
					dc2.d = *(mlib_d64 *)&psrc2[j + 1];
					dc3.d = *(mlib_d64 *)&psrc1[j + 2];
					c1 += MUL_F32(dc1.f[0], psrc2[j]);
					c2 += MUL_F32(dc1.f[1], dc2.f[0]);
					c3 += MUL_F32(dc3.f[0], dc2.f[1]);
					c4 += MUL_F32(dc3.f[1], psrc2[j + 3]);
					m11 += dc1.f[0];
					m12 += dc1.f[1];
					m13 += dc3.f[0];
					m14 += dc3.f[1];
					s11 += MUL_F32(dc1.f[0], dc1.f[0]);
					s12 += MUL_F32(dc1.f[1], dc1.f[1]);
					s13 += MUL_F32(dc3.f[0], dc3.f[0]);
					s14 += MUL_F32(dc3.f[1], dc3.f[1]);
				}
			}

			off >>= 2;
			for (k = off; j < size_row; j++, k++) {
				rez[k & 3] += MUL_F32(psrc1[j], psrc2[j]);
				rez[(k & 3) + 4] += psrc1[j];
				rez[(k & 3) + 12] += MUL_F32(psrc1[j],
						psrc1[j]);
			}
			rez[0 + off] += c1;
			rez[1 + off] += c2;
			rez[2 + off] += c3;
			rez[3 & (3 + off)] += c4;
			rez[0 + off + 4] += m11;
			rez[1 + off + 4] += m12;
			rez[2 + off + 4] += m13;
			rez[(3 & (3 + off)) + 4] += m14;
			rez[0 + off + 12] += s11;
			rez[1 + off + 12] += s12;
			rez[2 + off + 12] += s13;
			rez[(3 & (3 + off)) + 12] += s14;
			psrc1 += src1_stride;
			psrc2 += src2_stride;
		}
	} else {
		/* calculate m2 and s2 on the spot */
		rez[0] = rez[1] = rez[2] = rez[3] = 0.0;
		rez[4] = rez[5] = rez[6] = rez[7] = 0.0;
		rez[8] = rez[9] = rez[10] = rez[11] = 0.0;
		rez[12] = rez[13] = rez[14] = rez[15] = 0.0;
		rez[16] = rez[17] = rez[18] = rez[19] = 0.0;
		for (i = 0; i < height; i++) {
			c1 = c2 = c3 = c4 = 0.0;
			m11 = m12 = m13 = m14 = 0.0;
			m21 = m22 = m23 = m24 = 0.0;
			s11 = s12 = s13 = s14 = 0.0;
			s21 = s22 = s23 = s24 = 0.0;
			off = (mlib_addr)psrc1 & 4;
			j = 0;

			if (off) {
				c4 += MUL_F32(psrc1[0], psrc2[0]);
				m14 += psrc1[0];
				m24 += psrc2[0];
				s14 += MUL_F32(psrc1[0], psrc1[0]);
				s24 += MUL_F32(psrc2[0], psrc2[0]);
				j = 1;
			}

			if ((((mlib_addr)psrc1 ^ (mlib_addr)psrc2) & 7) == 0) {
				for (; j <= (size_row - 4); j += 4) {
					dc1.d = *(mlib_d64 *)&psrc1[j];
					dc2.d = *(mlib_d64 *)&psrc2[j];
					dc3.d = *(mlib_d64 *)&psrc1[j + 2];
					dc4.d = *(mlib_d64 *)&psrc2[j + 2];
					c1 += MUL_F32(dc1.f[0], dc2.f[0]);
					c2 += MUL_F32(dc1.f[1], dc2.f[1]);
					c3 += MUL_F32(dc3.f[0], dc4.f[0]);
					c4 += MUL_F32(dc3.f[1], dc4.f[1]);
					m11 += dc1.f[0];
					m12 += dc1.f[1];
					m13 += dc3.f[0];
					m14 += dc3.f[1];
					m21 += dc2.f[0];
					m22 += dc2.f[1];
					m23 += dc4.f[0];
					m24 += dc4.f[1];
					s11 += MUL_F32(dc1.f[0], dc1.f[0]);
					s12 += MUL_F32(dc1.f[1], dc1.f[1]);
					s13 += MUL_F32(dc3.f[0], dc3.f[0]);
					s14 += MUL_F32(dc3.f[1], dc3.f[1]);
					s21 += MUL_F32(dc2.f[0], dc2.f[0]);
					s22 += MUL_F32(dc2.f[1], dc2.f[1]);
					s23 += MUL_F32(dc4.f[0], dc4.f[0]);
					s24 += MUL_F32(dc4.f[1], dc4.f[1]);
				}
			} else {
				for (; j <= (size_row - 4); j += 4) {
					dc1.d = *(mlib_d64 *)&psrc1[j];
					dc2.d = *(mlib_d64 *)&psrc2[j + 1];
					dc3.d = *(mlib_d64 *)&psrc1[j + 2];
					c1 += MUL_F32(dc1.f[0], psrc2[j]);
					c2 += MUL_F32(dc1.f[1], dc2.f[0]);
					c3 += MUL_F32(dc3.f[0], dc2.f[1]);
					c4 += MUL_F32(dc3.f[1], psrc2[j + 3]);
					m11 += dc1.f[0];
					m12 += dc1.f[1];
					m13 += dc3.f[0];
					m14 += dc3.f[1];
					m21 += psrc2[j];
					m22 += dc2.f[0];
					m23 += dc2.f[1];
					m24 += psrc2[j + 3];
					s11 += MUL_F32(dc1.f[0], dc1.f[0]);
					s12 += MUL_F32(dc1.f[1], dc1.f[1]);
					s13 += MUL_F32(dc3.f[0], dc3.f[0]);
					s14 += MUL_F32(dc3.f[1], dc3.f[1]);
					s21 += MUL_F32(psrc2[j], psrc2[j]);
					s22 += MUL_F32(dc2.f[0], dc2.f[0]);
					s23 += MUL_F32(dc2.f[1], dc2.f[1]);
					s24 += MUL_F32(psrc2[j + 3],
							psrc2[j + 3]);
				}
			}

			off >>= 2;
			for (k = off; j < size_row; j++, k++) {
				rez[k & 3] += MUL_F32(psrc1[j], psrc2[j]);
				rez[(k & 3) + 4] += psrc1[j];
				rez[(k & 3) + 8] += psrc2[j];
				rez[(k & 3) + 12] += MUL_F32(psrc1[j],
						psrc1[j]);
				rez[(k & 3) + 16] += MUL_F32(psrc2[j],
						psrc2[j]);
			}
			rez[0 + off] += c1;
			rez[1 + off] += c2;
			rez[2 + off] += c3;
			rez[3 & (3 + off)] += c4;
			rez[0 + off + 4] += m11;
			rez[1 + off + 4] += m12;
			rez[2 + off + 4] += m13;
			rez[(3 & (3 + off)) + 4] += m14;
			rez[0 + off + 8] += m21;
			rez[1 + off + 8] += m22;
			rez[2 + off + 8] += m23;
			rez[(3 & (3 + off)) + 8] += m24;
			rez[0 + off + 12] += s11;
			rez[1 + off + 12] += s12;
			rez[2 + off + 12] += s13;
			rez[(3 & (3 + off)) + 12] += s14;
			rez[0 + off + 16] += s21;
			rez[1 + off + 16] += s22;
			rez[2 + off + 16] += s23;
			rez[(3 & (3 + off)) + 16] += s24;
			psrc1 += src1_stride;
			psrc2 += src2_stride;
		}
	}
}

/* *********************************************************** */

void
mlib_ImageNormCrossCorrel_F32_3(
    const mlib_image *img1,
    const mlib_image *img2,
    mlib_d64 *rez,
    const mlib_d64 *m2,
    const mlib_d64 *s2)
{
	mlib_f32 *psrc1 = (mlib_f32 *)mlib_ImageGetData(img1);
	mlib_f32 *psrc2 = (mlib_f32 *)mlib_ImageGetData(img2);

/* accumulators by channels */
	mlib_d64 c1, c2, c3;
	mlib_d64 m11, m12, m13;
	mlib_d64 m21, m22, m23;
	mlib_d64 s11, s12, s13;
	mlib_d64 s21, s22, s23;

/* accumulators by channels */
	mlib_d64 c4, c5, c6;
	mlib_d64 m14, m15, m16;
	mlib_d64 m24, m25, m26;
	mlib_d64 s14, s15, s16;
	mlib_d64 s24, s25, s26;

/* height of image */
	mlib_s32 height = mlib_ImageGetHeight(img1);

/* elements to next row */
	mlib_s32 src1_stride = mlib_ImageGetStride(img1) >> 2;

/* elements to next row */
	mlib_s32 src2_stride = mlib_ImageGetStride(img2) >> 2;

/* number of elements in the row */
	mlib_s32 size_row = mlib_ImageGetWidth(img1) * 3;

/* indices */
	mlib_s32 i, j;
	union
	{
		mlib_d64 d;
		mlib_f32 f[2];
	} dc1,
	    dc2,
	    dc3,
	    dc4,
	    dc5,
	    dc6;

	if (src1_stride == size_row && src1_stride == src2_stride) {
/* This images is not a sub-images and can be treated as a 1-D vectors */
		size_row *= height;
		height = 1;
	}

	if ((m2 != NULL) && (s2 != NULL)) {
		/* use the user-provided m2 and s2 */
		c1 = c2 = c3 = c4 = c5 = c6 = 0.0;
		m11 = m12 = m13 = m14 = m15 = m16 = 0.0;
		s11 = s12 = s13 = s14 = s15 = s16 = 0.0;
		for (i = 0; i < height; i++) {
			j = 0;

			if ((mlib_addr)psrc1 & 4) {
				c1 += MUL_F32(psrc1[j], psrc2[j]);
				c2 += MUL_F32(psrc1[j + 1], psrc2[j + 1]);
				c3 += MUL_F32(psrc1[j + 2], psrc2[j + 2]);
				m11 += psrc1[j];
				m12 += psrc1[j + 1];
				m13 += psrc1[j + 2];
				s11 += MUL_F32(psrc1[j], psrc1[j]);
				s12 += MUL_F32(psrc1[j + 1], psrc1[j + 1]);
				s13 += MUL_F32(psrc1[j + 2], psrc1[j + 2]);
				j = 3;
			}

			if ((((mlib_addr)psrc1 ^ (mlib_addr)psrc2) & 7) == 0) {
				for (; j <= (size_row - 6); j += 6) {
					dc1.d = *(mlib_d64 *)&psrc1[j];
					dc2.d = *(mlib_d64 *)&psrc2[j];
					dc3.d = *(mlib_d64 *)&psrc1[j + 2];
					dc4.d = *(mlib_d64 *)&psrc2[j + 2];
					dc5.d = *(mlib_d64 *)&psrc1[j + 4];
					dc6.d = *(mlib_d64 *)&psrc2[j + 4];
					c1 += MUL_F32(dc1.f[0], dc2.f[0]);
					c2 += MUL_F32(dc1.f[1], dc2.f[1]);
					c3 += MUL_F32(dc3.f[0], dc4.f[0]);
					c4 += MUL_F32(dc3.f[1], dc4.f[1]);
					c5 += MUL_F32(dc5.f[0], dc6.f[0]);
					c6 += MUL_F32(dc5.f[1], dc6.f[1]);
					m11 += dc1.f[0];
					m12 += dc1.f[1];
					m13 += dc3.f[0];
					m14 += dc3.f[1];
					m15 += dc5.f[0];
					m16 += dc5.f[1];
					s11 += MUL_F32(dc1.f[0], dc1.f[0]);
					s12 += MUL_F32(dc1.f[1], dc1.f[1]);
					s13 += MUL_F32(dc3.f[0], dc3.f[0]);
					s14 += MUL_F32(dc3.f[1], dc3.f[1]);
					s15 += MUL_F32(dc5.f[0], dc5.f[0]);
					s16 += MUL_F32(dc5.f[1], dc5.f[1]);
				}
			} else {
				for (; j <= (size_row - 6); j += 6) {
					dc1.d = *(mlib_d64 *)&psrc1[j];
					dc2.d = *(mlib_d64 *)&psrc2[j + 1];
					dc3.d = *(mlib_d64 *)&psrc1[j + 2];
					dc4.d = *(mlib_d64 *)&psrc2[j + 3];
					dc5.d = *(mlib_d64 *)&psrc1[j + 4];
					c1 += MUL_F32(dc1.f[0], psrc2[j]);
					c2 += MUL_F32(dc1.f[1], dc2.f[0]);
					c3 += MUL_F32(dc3.f[0], dc2.f[1]);
					c1 += MUL_F32(dc3.f[1], dc4.f[0]);
					c2 += MUL_F32(dc5.f[0], dc4.f[1]);
					c3 += MUL_F32(dc5.f[1], psrc2[j + 5]);
					m11 += dc1.f[0];
					m12 += dc1.f[1];
					m13 += dc3.f[0];
					m14 += dc3.f[1];
					m15 += dc5.f[0];
					m16 += dc5.f[1];
					s11 += MUL_F32(dc1.f[0], dc1.f[0]);
					s12 += MUL_F32(dc1.f[1], dc1.f[1]);
					s13 += MUL_F32(dc3.f[0], dc3.f[0]);
					s14 += MUL_F32(dc3.f[1], dc3.f[1]);
					s15 += MUL_F32(dc5.f[0], dc5.f[0]);
					s16 += MUL_F32(dc5.f[1], dc5.f[1]);
				}
			}

			if (j < size_row) {
				c1 += MUL_F32(psrc1[j], psrc2[j]);
				c2 += MUL_F32(psrc1[j + 1], psrc2[j + 1]);
				c3 += MUL_F32(psrc1[j + 2], psrc2[j + 2]);
				m11 += psrc1[j];
				m12 += psrc1[j + 1];
				m13 += psrc1[j + 2];
				s11 += MUL_F32(psrc1[j], psrc1[j]);
				s12 += MUL_F32(psrc1[j + 1], psrc1[j + 1]);
				s13 += MUL_F32(psrc1[j + 2], psrc1[j + 2]);
			}

			psrc1 += src1_stride;
			psrc2 += src2_stride;
		}

		rez[0] = c1 + c4;
		rez[1] = c2 + c5;
		rez[2] = c3 + c6;
		rez[4] = m11 + m14;
		rez[5] = m12 + m15;
		rez[6] = m13 + m16;
		rez[12] = s11 + s14;
		rez[13] = s12 + s15;
		rez[14] = s13 + s16;
	} else {
		/* calculate m2 and s2 on the spot */
		c1 = c2 = c3 = c4 = c5 = c6 = 0.0;
		m11 = m12 = m13 = m14 = m15 = m16 = 0.0;
		m21 = m22 = m23 = m24 = m25 = m26 = 0.0;
		s11 = s12 = s13 = s14 = s15 = s16 = 0.0;
		s21 = s22 = s23 = s24 = s25 = s26 = 0.0;
		for (i = 0; i < height; i++) {
			j = 0;

			if ((mlib_addr)psrc1 & 4) {
				c1 += MUL_F32(psrc1[j], psrc2[j]);
				c2 += MUL_F32(psrc1[j + 1], psrc2[j + 1]);
				c3 += MUL_F32(psrc1[j + 2], psrc2[j + 2]);
				m11 += psrc1[j];
				m12 += psrc1[j + 1];
				m13 += psrc1[j + 2];
				m21 += psrc2[j];
				m22 += psrc2[j + 1];
				m23 += psrc2[j + 2];
				s11 += MUL_F32(psrc1[j], psrc1[j]);
				s12 += MUL_F32(psrc1[j + 1], psrc1[j + 1]);
				s13 += MUL_F32(psrc1[j + 2], psrc1[j + 2]);
				s21 += MUL_F32(psrc2[j], psrc2[j]);
				s22 += MUL_F32(psrc2[j + 1], psrc2[j + 1]);
				s23 += MUL_F32(psrc2[j + 2], psrc2[j + 2]);
				j = 3;
			}

			if ((((mlib_addr)psrc1 ^ (mlib_addr)psrc2) & 7) == 0) {
				for (; j <= (size_row - 6); j += 6) {
					dc1.d = *(mlib_d64 *)&psrc1[j];
					dc2.d = *(mlib_d64 *)&psrc2[j];
					dc3.d = *(mlib_d64 *)&psrc1[j + 2];
					dc4.d = *(mlib_d64 *)&psrc2[j + 2];
					dc5.d = *(mlib_d64 *)&psrc1[j + 4];
					dc6.d = *(mlib_d64 *)&psrc2[j + 4];
					c1 += MUL_F32(dc1.f[0], dc2.f[0]);
					c2 += MUL_F32(dc1.f[1], dc2.f[1]);
					c3 += MUL_F32(dc3.f[0], dc4.f[0]);
					c4 += MUL_F32(dc3.f[1], dc4.f[1]);
					c5 += MUL_F32(dc5.f[0], dc6.f[0]);
					c6 += MUL_F32(dc5.f[1], dc6.f[1]);
					m11 += dc1.f[0];
					m12 += dc1.f[1];
					m13 += dc3.f[0];
					m14 += dc3.f[1];
					m15 += dc5.f[0];
					m16 += dc5.f[1];
					m21 += dc2.f[0];
					m22 += dc2.f[1];
					m23 += dc4.f[0];
					m24 += dc4.f[1];
					m25 += dc6.f[0];
					m26 += dc6.f[1];
					s11 += MUL_F32(dc1.f[0], dc1.f[0]);
					s12 += MUL_F32(dc1.f[1], dc1.f[1]);
					s13 += MUL_F32(dc3.f[0], dc3.f[0]);
					s14 += MUL_F32(dc3.f[1], dc3.f[1]);
					s15 += MUL_F32(dc5.f[0], dc5.f[0]);
					s16 += MUL_F32(dc5.f[1], dc5.f[1]);
					s21 += MUL_F32(dc2.f[0], dc2.f[0]);
					s22 += MUL_F32(dc2.f[1], dc2.f[1]);
					s23 += MUL_F32(dc4.f[0], dc4.f[0]);
					s24 += MUL_F32(dc4.f[1], dc4.f[1]);
					s25 += MUL_F32(dc6.f[0], dc6.f[0]);
					s26 += MUL_F32(dc6.f[1], dc6.f[1]);
				}
			} else {
				for (; j <= (size_row - 6); j += 6) {
					dc1.d = *(mlib_d64 *)&psrc1[j];
					dc2.d = *(mlib_d64 *)&psrc2[j + 1];
					dc3.d = *(mlib_d64 *)&psrc1[j + 2];
					dc4.d = *(mlib_d64 *)&psrc2[j + 3];
					dc5.d = *(mlib_d64 *)&psrc1[j + 4];
					c1 += MUL_F32(dc1.f[0], psrc2[j]);
					c2 += MUL_F32(dc1.f[1], dc2.f[0]);
					c3 += MUL_F32(dc3.f[0], dc2.f[1]);
					c1 += MUL_F32(dc3.f[1], dc4.f[0]);
					c2 += MUL_F32(dc5.f[0], dc4.f[1]);
					c3 += MUL_F32(dc5.f[1], psrc2[j + 5]);
					m11 += dc1.f[0];
					m12 += dc1.f[1];
					m13 += dc3.f[0];
					m14 += dc3.f[1];
					m15 += dc5.f[0];
					m16 += dc5.f[1];
					m21 += psrc2[j];
					m22 += dc2.f[0];
					m23 += dc2.f[1];
					m24 += dc4.f[0];
					m25 += dc4.f[1];
					m26 += psrc2[j + 5];
					s11 += MUL_F32(dc1.f[0], dc1.f[0]);
					s12 += MUL_F32(dc1.f[1], dc1.f[1]);
					s13 += MUL_F32(dc3.f[0], dc3.f[0]);
					s14 += MUL_F32(dc3.f[1], dc3.f[1]);
					s15 += MUL_F32(dc5.f[0], dc5.f[0]);
					s16 += MUL_F32(dc5.f[1], dc5.f[1]);
					s21 += MUL_F32(psrc2[j], psrc2[j]);
					s22 += MUL_F32(dc2.f[0], dc2.f[0]);
					s23 += MUL_F32(dc2.f[1], dc2.f[1]);
					s24 += MUL_F32(dc4.f[0], dc4.f[0]);
					s25 += MUL_F32(dc4.f[1], dc4.f[1]);
					s26 += MUL_F32(psrc2[j + 5],
							psrc2[j + 5]);
				}
			}

			if (j < size_row) {
				c1 += MUL_F32(psrc1[j], psrc2[j]);
				c2 += MUL_F32(psrc1[j + 1], psrc2[j + 1]);
				c3 += MUL_F32(psrc1[j + 2], psrc2[j + 2]);
				m11 += psrc1[j];
				m12 += psrc1[j + 1];
				m13 += psrc1[j + 2];
				m21 += psrc2[j];
				m22 += psrc2[j + 1];
				m23 += psrc2[j + 2];
				s11 += MUL_F32(psrc1[j], psrc1[j]);
				s12 += MUL_F32(psrc1[j + 1], psrc1[j + 1]);
				s13 += MUL_F32(psrc1[j + 2], psrc1[j + 2]);
				s21 += MUL_F32(psrc2[j], psrc2[j]);
				s22 += MUL_F32(psrc2[j + 1], psrc2[j + 1]);
				s23 += MUL_F32(psrc2[j + 2], psrc2[j + 2]);
			}

			psrc1 += src1_stride;
			psrc2 += src2_stride;
		}

		rez[0] = c1 + c4;
		rez[1] = c2 + c5;
		rez[2] = c3 + c6;
		rez[4] = m11 + m14;
		rez[5] = m12 + m15;
		rez[6] = m13 + m16;
		rez[8] = m21 + m24;
		rez[9] = m22 + m25;
		rez[10] = m23 + m26;
		rez[12] = s11 + s14;
		rez[13] = s12 + s15;
		rez[14] = s13 + s16;
		rez[16] = s21 + s24;
		rez[17] = s22 + s25;
		rez[18] = s23 + s26;
	}
}

/* *********************************************************** */
