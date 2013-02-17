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

#pragma ident	"@(#)mlib_s_ImageNormCrossCorrel_Fp.c	9.5	07/11/05 SMI"


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
#include <mlib_s_ImageCorrel.h>
#include <mlib_SysMath.h>
#ifdef __SUNPRO_C
#include <sunmedia_intrin.h>
#else
#include <emmintrin.h>
#endif

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

#define	Square_Root(rtn, x, y)                                \
{                                                             \
	diff = x - y;                                         \
	if (x == 0) {                                         \
		rtn = 0.0;                                    \
	}                                                     \
	x = (x >= 0) ? x : -x;                                \
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
			mlib_s_ImageNormCrossCorrel_F32_3(img1, img2, rez,
				mean2, sdev2);
		else
			mlib_s_ImageNormCrossCorrel_F32_124(img1, img2, rez,
				mean2, sdev2);
		break;

	case MLIB_DOUBLE:

		if (mlib_ImageGetChannels(img1) == 3)
			mlib_s_ImageNormCrossCorrel_D64_3(img1, img2, rez,
				mean2, sdev2);
		else
			mlib_s_ImageNormCrossCorrel_D64_124(img1, img2, rez,
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
mlib_s_ImageNormCrossCorrel_D64_124(
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
		rez[0] = rez[1] = rez[2] = 0.0;
		rez[4] = rez[5] = rez[6] = 0.0;
		rez[12] = rez[13] = rez[14] = 0.0;
		__m128d xc1, xc3, xm1, xm3, xs1, xs3;
		xc1 = xc3 = xm1 = xm3 = xs1 = xs3 = _mm_setzero_pd();

		for (i = 0; i < height; i++) {
			if ((0 == ((mlib_addr)psrc1 & 15)) &&
			    (0 == ((mlib_addr)psrc2 & 15))) {
				for (j = 0; j <= (size_row - 4); j += 4) {
					__m128d sd0, sd1, sd2, sd3;
					sd0 = _mm_load_pd(psrc1 + j);
					sd1 = _mm_load_pd(psrc2 + j);
					xm1 = _mm_add_pd(xm1, sd0);
					xs1 = _mm_add_pd(xs1,
					    _mm_mul_pd(sd0, sd0));
					xc1 = _mm_add_pd(xc1,
					    _mm_mul_pd(sd0, sd1));

					sd2 = _mm_load_pd(psrc1 + j + 2);
					sd3 = _mm_load_pd(psrc2 + j + 2);
					xm3 = _mm_add_pd(xm3, sd2);
					xs3 = _mm_add_pd(xs3,
					    _mm_mul_pd(sd2, sd2));
					xc3 = _mm_add_pd(xc3,
					    _mm_mul_pd(sd2, sd3));
				}

			} else {

				for (j = 0; j <= (size_row - 4); j += 4) {
					__m128d sd0, sd1, sd2, sd3;
					sd0 = _mm_loadu_pd(psrc1 + j);
					sd1 = _mm_loadu_pd(psrc2 + j);
					xm1 = _mm_add_pd(xm1, sd0);
					xs1 = _mm_add_pd(xs1,
					    _mm_mul_pd(sd0, sd0));
					xc1 = _mm_add_pd(xc1,
					    _mm_mul_pd(sd0, sd1));

					sd2 = _mm_loadu_pd(psrc1 + j + 2);
					sd3 = _mm_loadu_pd(psrc2 + j + 2);
					xm3 = _mm_add_pd(xm3, sd2);
					xs3 = _mm_add_pd(xs3,
					    _mm_mul_pd(sd2, sd2));
					xc3 = _mm_add_pd(xc3,
					    _mm_mul_pd(sd2, sd3));
				}
			}

			for (k = 0; j < size_row; j++, k++) {
				rez[k] += MUL_D64(psrc1[j], psrc2[j]);
				rez[k + 4] += psrc1[j];
				rez[k + 12] += MUL_D64(psrc1[j], psrc1[j]);
			}

			psrc1 += src1_stride;
			psrc2 += src2_stride;
		}

		mlib_d64 c[4], m1[4], s1[4];
		_mm_storeu_pd((c + 0), xc1);
		_mm_storeu_pd((c + 2), xc3);
		_mm_storeu_pd((m1 + 0), xm1);
		_mm_storeu_pd((m1 + 2), xm3);
		_mm_storeu_pd((s1 + 0), xs1);
		_mm_storeu_pd((s1 + 2), xs3);

		rez[0] += c[0];
		rez[1] += c[1];
		rez[2] += c[2];
		rez[3] = c[3];
		rez[4] += m1[0];
		rez[5] += m1[1];
		rez[6] += m1[2];
		rez[7] = m1[3];
		rez[12] += s1[0];
		rez[13] += s1[1];
		rez[14] += s1[2];
		rez[15] = s1[3];

	} else {
		/* calculate m2 and s2 on the spot */
		__m128d xc_12, xc_34, xm1_12, xm1_34, xs1_12, xs1_34;
		__m128d xm2_12, xm2_34, xs2_12, xs2_34;
		xc_12 = xc_34 = _mm_setzero_pd();
		xm1_12 = xm1_34 = xs1_12 = xs1_34 = _mm_setzero_pd();
		xm2_12 = xm2_34 = xs2_12 = xs2_34 = _mm_setzero_pd();

		rez[0] = rez[1] = rez[2] = 0.0;
		rez[4] = rez[5] = rez[6] = 0.0;
		rez[8] = rez[9] = rez[10] = 0.0;
		rez[12] = rez[13] = rez[14] = 0.0;
		rez[16] = rez[17] = rez[18] = 0.0;

		for (i = 0; i < height; i++) {
			if ((0 == ((mlib_addr)psrc1 & 15)) &&
			    (0 == ((mlib_addr)psrc2 & 15))) {
				for (j = 0; j <= (size_row - 4); j += 4) {
					__m128d xs10, xs11, xs20, xs21;
					xs10 = _mm_load_pd((psrc1 + j));
					xs11 = _mm_load_pd((psrc1 + j + 2));
					xs20 = _mm_load_pd((psrc2 + j));
					xs21 = _mm_load_pd((psrc2 + j + 2));

					xm1_12 = _mm_add_pd(xm1_12, xs10);
					xm1_34 = _mm_add_pd(xm1_34, xs11);
					xm2_12 = _mm_add_pd(xm2_12, xs20);
					xm2_34 = _mm_add_pd(xm2_34, xs21);

					xs1_12 = _mm_add_pd(xs1_12,
					    _mm_mul_pd(xs10, xs10));
					xs1_34 = _mm_add_pd(xs1_34,
					    _mm_mul_pd(xs11, xs11));
					xs2_12 = _mm_add_pd(xs2_12,
					    _mm_mul_pd(xs20, xs20));
					xs2_34 = _mm_add_pd(xs2_34,
					    _mm_mul_pd(xs21, xs21));

					xc_12 = _mm_add_pd(xc_12,
					    _mm_mul_pd(xs10, xs20));
					xc_34 = _mm_add_pd(xc_34,
					    _mm_mul_pd(xs11, xs21));
				}

			} else {

				for (j = 0; j <= (size_row - 4); j += 4) {
					__m128d xs10, xs11, xs20, xs21;
					xs10 = _mm_loadu_pd((psrc1 + j));
					xs11 = _mm_loadu_pd((psrc1 + j + 2));
					xs20 = _mm_loadu_pd((psrc2 + j));
					xs21 = _mm_loadu_pd((psrc2 + j + 2));

					xm1_12 = _mm_add_pd(xm1_12, xs10);
					xm1_34 = _mm_add_pd(xm1_34, xs11);
					xm2_12 = _mm_add_pd(xm2_12, xs20);
					xm2_34 = _mm_add_pd(xm2_34, xs21);

					xs1_12 = _mm_add_pd(xs1_12,
					    _mm_mul_pd(xs10, xs10));
					xs1_34 = _mm_add_pd(xs1_34,
					    _mm_mul_pd(xs11, xs11));
					xs2_12 = _mm_add_pd(xs2_12,
					    _mm_mul_pd(xs20, xs20));
					xs2_34 = _mm_add_pd(xs2_34,
					    _mm_mul_pd(xs21, xs21));

					xc_12 = _mm_add_pd(xc_12,
					    _mm_mul_pd(xs10, xs20));
					xc_34 = _mm_add_pd(xc_34,
					    _mm_mul_pd(xs11, xs21));
				}
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

		mlib_d64 c[4], m1[4], s1[4], m2[4], s2[4];
		_mm_storeu_pd((c + 0), xc_12);
		_mm_storeu_pd((c + 2), xc_34);
		_mm_storeu_pd((m1 + 0), xm1_12);
		_mm_storeu_pd((m1 + 2), xm1_34);
		_mm_storeu_pd((s1 + 0), xs1_12);
		_mm_storeu_pd((s1 + 2), xs1_34);
		_mm_storeu_pd((m2 + 0), xm2_12);
		_mm_storeu_pd((m2 + 2), xm2_34);
		_mm_storeu_pd((s2 + 0), xs2_12);
		_mm_storeu_pd((s2 + 2), xs2_34);

		rez[0] += c[0];
		rez[1] += c[1];
		rez[2] += c[2];
		rez[3] = c[3];
		rez[4] += m1[0];
		rez[5] += m1[1];
		rez[6] += m1[2];
		rez[7] = m1[3];
		rez[8] += m2[0];
		rez[9] += m2[1];
		rez[10] += m2[2];
		rez[11] = m2[3];
		rez[12] += s1[0];
		rez[13] += s1[1];
		rez[14] += s1[2];
		rez[15] = s1[3];
		rez[16] += s2[0];
		rez[17] += s2[1];
		rez[18] += s2[2];
		rez[19] = s2[3];
	}
}

/* *********************************************************** */

void
mlib_s_ImageNormCrossCorrel_D64_3(
    const mlib_image *img1,
    const mlib_image *img2,
    mlib_d64 *rez,
    const mlib_d64 *m2,
    const mlib_d64 *s2)
{
	mlib_d64 *psrc1 = (mlib_d64 *)mlib_ImageGetData(img1);
	mlib_d64 *psrc2 = (mlib_d64 *)mlib_ImageGetData(img2);

/* accumulators by channels */
	mlib_d64 c3, m13, m23, s13, s23;

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
		c3 = m13 = s13 = 0.0;

		__m128d xc1, xm1, xs1, xc2, xm2, xs2, xc3, xm3, xs3;
		xc1 = xm1 = xs1 = _mm_setzero_pd();
		xc2 = xm2 = xs2 = _mm_setzero_pd();
		xc3 = xm3 = xs3 = _mm_setzero_pd();

		for (i = 0; i < height; i++) {
			if ((0 == ((mlib_addr)psrc1 & 15)) &&
			    (0 == ((mlib_addr)psrc2 & 15))) {
				for (j = 0; j < size_row - 3; j += 6) {
					__m128d sd0, sd1;
					sd0 = _mm_load_pd(psrc1 + j);
					sd1 = _mm_load_pd(psrc2 + j);

					xm1 = _mm_add_pd(xm1, sd0);
					xs1 = _mm_add_pd(xs1,
					    _mm_mul_pd(sd0, sd0));
					xc1 = _mm_add_pd(xc1,
					    _mm_mul_pd(sd0, sd1));

					sd0 = _mm_load_pd(psrc1 + j + 2);
					sd1 = _mm_load_pd(psrc2 + j + 2);

					xm2 = _mm_add_pd(xm2, sd0);
					xs2 = _mm_add_pd(xs2,
					    _mm_mul_pd(sd0, sd0));
					xc2 = _mm_add_pd(xc2,
					    _mm_mul_pd(sd0, sd1));

					sd0 = _mm_load_pd(psrc1 + j + 4);
					sd1 = _mm_load_pd(psrc2 + j + 4);

					xm3 = _mm_add_pd(xm3, sd0);
					xs3 = _mm_add_pd(xs3,
					    _mm_mul_pd(sd0, sd0));
					xc3 = _mm_add_pd(xc3,
					    _mm_mul_pd(sd0, sd1));
				}

			} else {

				for (j = 0; j < size_row - 3; j += 6) {
					__m128d sd0, sd1;
					sd0 = _mm_loadu_pd(psrc1 + j);
					sd1 = _mm_loadu_pd(psrc2 + j);

					xm1 = _mm_add_pd(xm1, sd0);
					xs1 = _mm_add_pd(xs1,
					    _mm_mul_pd(sd0, sd0));
					xc1 = _mm_add_pd(xc1,
					    _mm_mul_pd(sd0, sd1));

					sd0 = _mm_loadu_pd(psrc1 + j + 2);
					sd1 = _mm_loadu_pd(psrc2 + j + 2);

					xm2 = _mm_add_pd(xm2, sd0);
					xs2 = _mm_add_pd(xs2,
					    _mm_mul_pd(sd0, sd0));
					xc2 = _mm_add_pd(xc2,
					    _mm_mul_pd(sd0, sd1));

					sd0 = _mm_loadu_pd(psrc1 + j + 4);
					sd1 = _mm_loadu_pd(psrc2 + j + 4);

					xm3 = _mm_add_pd(xm3, sd0);
					xs3 = _mm_add_pd(xs3,
					    _mm_mul_pd(sd0, sd0));
					xc3 = _mm_add_pd(xc3,
					    _mm_mul_pd(sd0, sd1));
				}
			}

			if (j < size_row) {
				__m128d sd0, sd1;
				sd0 = _mm_loadu_pd(psrc1 + j);
				sd1 = _mm_loadu_pd(psrc2 + j);

				xm1 = _mm_add_pd(xm1, sd0);
				xs1 = _mm_add_pd(xs1,
				    _mm_mul_pd(sd0, sd0));
				xc1 = _mm_add_pd(xc1,
				    _mm_mul_pd(sd0, sd1));

				c3 += MUL_D64(psrc1[j + 2], psrc2[j + 2]);
				m13 += psrc1[j + 2];
				s13 += MUL_D64(psrc1[j + 2], psrc1[j + 2]);
			}

			psrc1 += src1_stride;
			psrc2 += src2_stride;
		}

		mlib_d64 c[6], m1[6], s1[6];
		_mm_storeu_pd((c + 0), xc1);
		_mm_storeu_pd((m1 + 0), xm1);
		_mm_storeu_pd((s1 + 0), xs1);
		_mm_storeu_pd((c + 2), xc2);
		_mm_storeu_pd((m1 + 2), xm2);
		_mm_storeu_pd((s1 + 2), xs2);
		_mm_storeu_pd((c + 4), xc3);
		_mm_storeu_pd((m1 + 4), xm3);
		_mm_storeu_pd((s1 + 4), xs3);

		rez[0] = c[0] + c[3];
		rez[1] = c[1] + c[4];
		rez[2] = c[2] + c[5] + c3;
		rez[4] = m1[0] + m1[3];
		rez[5] = m1[1] + m1[4];
		rez[6] = m1[2] + m1[5] + m13;
		rez[12] = s1[0] + s1[3];
		rez[13] = s1[1] + s1[4];
		rez[14] = s1[2] + s1[5] + s13;

	} else {

		/* calculate m2 and s2 on the spot */
		c3 = m13 = m23 = s13 = s23 = 0.0;

		__m128d xc_12, xc_34, xc_56;
		__m128d xm1_12, xm1_34, xm1_56, xs1_12, xs1_34, xs1_56;
		__m128d xm2_12, xm2_34, xm2_56, xs2_12, xs2_34, xs2_56;
		xc_12 = xc_34 = xc_56 = _mm_setzero_pd();
		xm1_12 = xm1_34 = xm1_56 = xs1_12 =
		    xs1_34 = xs1_56 = _mm_setzero_pd();
		xm2_12 = xm2_34 = xm2_56 = xs2_12 =
		    xs2_34 = xs2_56 = _mm_setzero_pd();

		for (i = 0; i < height; i++) {
			if ((0 == ((mlib_addr)psrc1 & 15)) &&
			    (0 == ((mlib_addr)psrc2 & 15))) {
				for (j = 0; j <= (size_row - 6); j += 6) {
					__m128d sd0, sd1;
					__m128d xs10, xs11, xs20, xs21;
					xs10 = _mm_load_pd((psrc1 + j));
					xs20 = _mm_load_pd((psrc2 + j));
					xs11 = _mm_load_pd((psrc1 + j + 2));
					xs21 = _mm_load_pd((psrc2 + j + 2));

					xm1_12 = _mm_add_pd(xm1_12, xs10);
					xm1_34 = _mm_add_pd(xm1_34, xs11);
					xm2_12 = _mm_add_pd(xm2_12, xs20);
					xm2_34 = _mm_add_pd(xm2_34, xs21);

					xs1_12 = _mm_add_pd(xs1_12,
					    _mm_mul_pd(xs10, xs10));
					xs1_34 = _mm_add_pd(xs1_34,
					    _mm_mul_pd(xs11, xs11));
					xs2_12 = _mm_add_pd(xs2_12,
					    _mm_mul_pd(xs20, xs20));
					xs2_34 = _mm_add_pd(xs2_34,
					    _mm_mul_pd(xs21, xs21));

					xc_12 = _mm_add_pd(xc_12,
					    _mm_mul_pd(xs10, xs20));
					xc_34 = _mm_add_pd(xc_34,
					    _mm_mul_pd(xs11, xs21));

					xs10 = _mm_load_pd((psrc1 + j + 4));
					xs20 = _mm_load_pd((psrc2 + j + 4));
					xm1_56 = _mm_add_pd(xm1_56, xs10);
					xm2_56 = _mm_add_pd(xm2_56, xs20);
					xs1_56 = _mm_add_pd(xs1_56,
					    _mm_mul_pd(xs10, xs10));
					xs2_56 = _mm_add_pd(xs2_56,
					    _mm_mul_pd(xs20, xs20));
					xc_56 = _mm_add_pd(xc_56,
					    _mm_mul_pd(xs10, xs20));
				}

			} else {

				for (j = 0; j <= (size_row - 6); j += 6) {
					__m128d sd0, sd1;
					__m128d xs10, xs11, xs20, xs21;
					xs10 = _mm_loadu_pd((psrc1 + j));
					xs20 = _mm_loadu_pd((psrc2 + j));
					xs11 = _mm_loadu_pd((psrc1 + j + 2));
					xs21 = _mm_loadu_pd((psrc2 + j + 2));

					xm1_12 = _mm_add_pd(xm1_12, xs10);
					xm1_34 = _mm_add_pd(xm1_34, xs11);
					xm2_12 = _mm_add_pd(xm2_12, xs20);
					xm2_34 = _mm_add_pd(xm2_34, xs21);

					xs1_12 = _mm_add_pd(xs1_12,
					    _mm_mul_pd(xs10, xs10));
					xs1_34 = _mm_add_pd(xs1_34,
					    _mm_mul_pd(xs11, xs11));
					xs2_12 = _mm_add_pd(xs2_12,
					    _mm_mul_pd(xs20, xs20));
					xs2_34 = _mm_add_pd(xs2_34,
					    _mm_mul_pd(xs21, xs21));

					xc_12 = _mm_add_pd(xc_12,
					    _mm_mul_pd(xs10, xs20));
					xc_34 = _mm_add_pd(xc_34,
					    _mm_mul_pd(xs11, xs21));

					xs10 = _mm_loadu_pd((psrc1 + j + 4));
					xs20 = _mm_loadu_pd((psrc2 + j + 4));
					xm1_56 = _mm_add_pd(xm1_56, xs10);
					xm2_56 = _mm_add_pd(xm2_56, xs20);
					xs1_56 = _mm_add_pd(xs1_56,
					    _mm_mul_pd(xs10, xs10));
					xs2_56 = _mm_add_pd(xs2_56,
					    _mm_mul_pd(xs20, xs20));
					xc_56 = _mm_add_pd(xc_56,
					    _mm_mul_pd(xs10, xs20));
				}
			}

			for (; j < size_row; j += 3) {
				__m128d xs10, xs20;
				xs10 = _mm_loadu_pd((psrc1 + j));
				xs20 = _mm_loadu_pd((psrc2 + j));

				xm1_12 = _mm_add_pd(xm1_12, xs10);
				xm2_12 = _mm_add_pd(xm2_12, xs20);

				xs1_12 = _mm_add_pd(xs1_12,
				    _mm_mul_pd(xs10, xs10));
				xs2_12 = _mm_add_pd(xs2_12,
				    _mm_mul_pd(xs20, xs20));

				xc_12 = _mm_add_pd(xc_12,
				    _mm_mul_pd(xs10, xs20));
				c3 += MUL_D64(psrc1[j + 2], psrc2[j + 2]);
				m13 += psrc1[j + 2];
				m23 += psrc2[j + 2];
				s13 += MUL_D64(psrc1[j + 2], psrc1[j + 2]);
				s23 += MUL_D64(psrc2[j + 2], psrc2[j + 2]);
			}

			psrc1 += src1_stride;
			psrc2 += src2_stride;
		}

		mlib_d64 c[6], m1[6], s1[6], m2[6], s2[6];
		_mm_storeu_pd((c + 0), xc_12);
		_mm_storeu_pd((c + 2), xc_34);
		_mm_storeu_pd((c + 4), xc_56);
		_mm_storeu_pd((m1 + 0), xm1_12);
		_mm_storeu_pd((m1 + 2), xm1_34);
		_mm_storeu_pd((m1 + 4), xm1_56);
		_mm_storeu_pd((s1 + 0), xs1_12);
		_mm_storeu_pd((s1 + 2), xs1_34);
		_mm_storeu_pd((s1 + 4), xs1_56);
		_mm_storeu_pd((m2 + 0), xm2_12);
		_mm_storeu_pd((m2 + 2), xm2_34);
		_mm_storeu_pd((m2 + 4), xm2_56);
		_mm_storeu_pd((s2 + 0), xs2_12);
		_mm_storeu_pd((s2 + 2), xs2_34);
		_mm_storeu_pd((s2 + 4), xs2_56);

		rez[0] = c[0] + c[3];
		rez[1] = c[1] + c[4];
		rez[2] = c[2] + c[5] + c3;
		rez[4] = m1[0] + m1[3];
		rez[5] = m1[1] + m1[4];
		rez[6] = m1[2] + m1[5] + m13;
		rez[8] = m2[0] + m2[3];
		rez[9] = m2[1] + m2[4];
		rez[10] = m2[2] + m2[5] + m23;
		rez[12] = s1[0] + s1[3];
		rez[13] = s1[1] + s1[4];
		rez[14] = s1[2] + s1[5] + s13;
		rez[16] = s2[0] + s2[3];
		rez[17] = s2[1] + s2[4];
		rez[18] = s2[2] + s2[5] + s23;
	}
}

/* *********************************************************** */

#define	MUL_F32(x, y)	((x) * (mlib_d64) (y))

/* *********************************************************** */

void
mlib_s_ImageNormCrossCorrel_F32_124(
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
	union
	{
		mlib_d64 d;
		mlib_f32 f[2];
	} dc1, dc2, dc3, dc4;

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

			__m128d xc1, xc3, xm1, xm3, xs1, xs3;
			xc1 = xc3 = xm1 = xm3 = xs1 = xs3 = _mm_setzero_pd();

			for (j = 0; j <= (size_row - 4); j += 4) {
				__m128d xs10, xs11, xs20, xs21;
				xs10 =
				    _mm_cvtps_pd(*((__m128 *)(psrc1 + j)));
				xs11 =
				    _mm_cvtps_pd(*((__m128 *)(psrc1 + j + 2)));
				xs20 =
				    _mm_cvtps_pd(*((__m128 *)(psrc2 + j)));
				xs21 =
				    _mm_cvtps_pd(*((__m128 *)(psrc2 + j + 2)));

				xm1 = _mm_add_pd(xm1, xs10);
				xs1 = _mm_add_pd(xs1,
				    _mm_mul_pd(xs10, xs10));
				xc1 = _mm_add_pd(xc1,
				    _mm_mul_pd(xs10, xs20));

				xm3 = _mm_add_pd(xm3, xs11);
				xs3 = _mm_add_pd(xs3,
				    _mm_mul_pd(xs11, xs11));
				xc3 = _mm_add_pd(xc3,
				    _mm_mul_pd(xs11, xs21));
			}

			for (k = 0; j < size_row; j++, k++) {
				rez[k & 3] += MUL_F32(psrc1[j], psrc2[j]);
				rez[(k & 3) + 4] += psrc1[j];
				rez[(k & 3) + 12] += MUL_F32(psrc1[j],
						psrc1[j]);
			}

			__m128d xsc0, xsc1, xsm10, xsm11, xss10, xss11;
			mlib_d64 *pc0, *pc1, *pm10, *pm11, *ps10, *ps11;
			pc0 = (mlib_d64 *)&xsc0;
			pc1 = (mlib_d64 *)&xsc1;
			pm10 = (mlib_d64 *)&xsm10;
			pm11 = (mlib_d64 *)&xsm11;
			ps10 = (mlib_d64 *)&xss10;
			ps11 = (mlib_d64 *)&xss11;

			_mm_store_pd((pc0), xc1);
			_mm_store_pd((pc1), xc3);
			_mm_store_pd((pm10), xm1);
			_mm_store_pd((pm11), xm3);
			_mm_store_pd((ps10), xs1);
			_mm_store_pd((ps11), xs3);

			rez[0] += pc0[0];
			rez[1] += pc0[1];
			rez[2] += pc1[0];
			rez[3] += pc1[1];
			rez[0 + 4] += pm10[0];
			rez[1 + 4] += pm10[1];
			rez[2 + 4] += pm11[0];
			rez[3 + 4] += pm11[1];
			rez[0 + 12] += ps10[0];
			rez[1 + 12] += ps10[1];
			rez[2 + 12] += ps11[0];
			rez[3 + 12] += ps11[1];

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

			__m128d xc_12, xc_34, xm1_12, xm1_34, xs1_12, xs1_34;
			__m128d xm2_12, xm2_34, xs2_12, xs2_34;
			xc_12 = xc_34 = xm1_12 = xm1_34 =
			    xs1_12 = xs1_34 = xm2_12 = xm2_34 =
			    xs2_12 = xs2_34 = _mm_setzero_pd();

			for (j = 0; j <= (size_row - 4); j += 4) {
				__m128d xs10, xs11, xs20, xs21;
				xs10 =
				    _mm_cvtps_pd(*((__m128 *)(psrc1 + j)));
				xs11 =
				    _mm_cvtps_pd(*((__m128 *)(psrc1 + j + 2)));
				xs20 =
				    _mm_cvtps_pd(*((__m128 *)(psrc2 + j)));
				xs21 =
				    _mm_cvtps_pd(*((__m128 *)(psrc2 + j + 2)));

				xm1_12 = _mm_add_pd(xm1_12, xs10);
				xm1_34 = _mm_add_pd(xm1_34, xs11);
				xm2_12 = _mm_add_pd(xm2_12, xs20);
				xm2_34 = _mm_add_pd(xm2_34, xs21);

				xs1_12 = _mm_add_pd(xs1_12,
				    _mm_mul_pd(xs10, xs10));
				xs1_34 = _mm_add_pd(xs1_34,
				    _mm_mul_pd(xs11, xs11));
				xs2_12 = _mm_add_pd(xs2_12,
				    _mm_mul_pd(xs20, xs20));
				xs2_34 = _mm_add_pd(xs2_34,
				    _mm_mul_pd(xs21, xs21));

				xc_12 = _mm_add_pd(xc_12,
				    _mm_mul_pd(xs10, xs20));
				xc_34 = _mm_add_pd(xc_34,
				    _mm_mul_pd(xs11, xs21));
			}

			for (k = 0; j < size_row; j++, k++) {
				rez[k & 3] += MUL_F32(psrc1[j], psrc2[j]);
				rez[(k & 3) + 4] += psrc1[j];
				rez[(k & 3) + 8] += psrc2[j];
				rez[(k & 3) + 12] += MUL_F32(psrc1[j],
						psrc1[j]);
				rez[(k & 3) + 16] += MUL_F32(psrc2[j],
						psrc2[j]);
			}

			__m128d xc0, xc1, xm10, xm11, xs10, xs11;
			__m128d xm20, xm21, xs20, xs21;
			mlib_d64 *pc0, *pc1, *pm10, *pm11, *ps10, *ps11;
			mlib_d64 *pm20, *pm21, *ps20, *ps21;
			pc0 = (mlib_d64 *)&xc0;
			pc1 = (mlib_d64 *)&xc1;
			pm10 = (mlib_d64 *)&xm10;
			pm11 = (mlib_d64 *)&xm11;
			pm20 = (mlib_d64 *)&xm20;
			pm21 = (mlib_d64 *)&xm21;
			ps10 = (mlib_d64 *)&xs10;
			ps11 = (mlib_d64 *)&xs11;
			ps20 = (mlib_d64 *)&xs20;
			ps21 = (mlib_d64 *)&xs21;

			_mm_store_pd((pc0), xc_12);
			_mm_store_pd((pc1), xc_34);
			_mm_store_pd((pm10), xm1_12);
			_mm_store_pd((pm11), xm1_34);
			_mm_store_pd((ps10), xs1_12);
			_mm_store_pd((ps11), xs1_34);
			_mm_store_pd((pm20), xm2_12);
			_mm_store_pd((pm21), xm2_34);
			_mm_store_pd((ps20), xs2_12);
			_mm_store_pd((ps21), xs2_34);

			rez[0] += pc0[0];
			rez[1] += pc0[1];
			rez[2] += pc1[0];
			rez[3] += pc1[1];
			rez[0 + 4] += pm10[0];
			rez[1 + 4] += pm10[1];
			rez[2 + 4] += pm11[0];
			rez[3 + 4] += pm11[1];
			rez[0 + 8] += pm20[0];
			rez[1 + 8] += pm20[1];
			rez[2 + 8] += pm21[0];
			rez[3 + 8] += pm21[1];
			rez[0 + 12] += ps10[0];
			rez[1 + 12] += ps10[1];
			rez[2 + 12] += ps11[0];
			rez[3 + 12] += ps11[1];
			rez[0 + 16] += ps20[0];
			rez[1 + 16] += ps20[1];
			rez[2 + 16] += ps21[0];
			rez[3 + 16] += ps21[1];

			psrc1 += src1_stride;
			psrc2 += src2_stride;
		}
	}
}

/* *********************************************************** */

void
mlib_s_ImageNormCrossCorrel_F32_3(
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

		__m128d xc_12, xc_34, xc_56;
		__m128d xm1_12, xm1_34, xm1_56, xs1_12, xs1_34, xs1_56;
		xc_12 = xc_34 = xc_56 = xm1_12 = xm1_34 = xm1_56 = xs1_12 =
		    xs1_34 = xs1_56 = _mm_setzero_pd();

		for (i = 0; i < height; i++) {
			for (j = 0; j <= (size_row - 6); j += 6) {
				__m128d xs10, xs11, xs20, xs21;
				xs10 =
				    _mm_cvtps_pd(*((__m128 *)(psrc1 + j)));
				xs11 =
				    _mm_cvtps_pd(*((__m128 *)(psrc1 + j + 2)));
				xs20 =
				    _mm_cvtps_pd(*((__m128 *)(psrc2 + j)));
				xs21 =
				    _mm_cvtps_pd(*((__m128 *)(psrc2 + j + 2)));

				xm1_12 = _mm_add_pd(xm1_12, xs10);
				xm1_34 = _mm_add_pd(xm1_34, xs11);

				xs1_12 = _mm_add_pd(xs1_12,
				    _mm_mul_pd(xs10, xs10));
				xs1_34 = _mm_add_pd(xs1_34,
				    _mm_mul_pd(xs11, xs11));

				xc_12 = _mm_add_pd(xc_12,
				    _mm_mul_pd(xs10, xs20));
				xc_34 = _mm_add_pd(xc_34,
				    _mm_mul_pd(xs11, xs21));

				xs10 =
				    _mm_cvtps_pd(*((__m128 *)(psrc1 + j + 4)));
				xs20 =
				    _mm_cvtps_pd(*((__m128 *)(psrc2 + j + 4)));
				xm1_56 = _mm_add_pd(xm1_56, xs10);
				xs1_56 = _mm_add_pd(xs1_56,
				    _mm_mul_pd(xs10, xs10));
				xc_56 = _mm_add_pd(xc_56,
				    _mm_mul_pd(xs10, xs20));
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

		__m128d xca, xm1a, xs1a;
		__m128d xcb, xm1b, xs1b;
		__m128d xcc, xm1c, xs1c;
		mlib_d64 *pca, *pm1a, *ps1a;
		mlib_d64 *pcb, *pm1b, *ps1b;
		mlib_d64 *pcc, *pm1c, *ps1c;

		pca = (mlib_d64 *)&xca;
		pm1a = (mlib_d64 *)&xm1a;
		ps1a = (mlib_d64 *)&xs1a;

		pcb = (mlib_d64 *)&xcb;
		pm1b = (mlib_d64 *)&xm1b;
		ps1b = (mlib_d64 *)&xs1b;

		pcc = (mlib_d64 *)&xcc;
		pm1c = (mlib_d64 *)&xm1c;
		ps1c = (mlib_d64 *)&xs1c;

		_mm_store_pd((pca), xc_12);
		_mm_store_pd((pcb), xc_34);
		_mm_store_pd((pcc), xc_56);
		_mm_store_pd((pm1a), xm1_12);
		_mm_store_pd((pm1b), xm1_34);
		_mm_store_pd((pm1c), xm1_56);
		_mm_store_pd((ps1a), xs1_12);
		_mm_store_pd((ps1b), xs1_34);
		_mm_store_pd((ps1c), xs1_56);

		rez[0] = pca[0] + pcb[1] + c1;
		rez[1] = pca[1] + pcc[0] + c2;
		rez[2] = pcb[0] + pcc[1] + c3;
		rez[4] = pm1a[0] + pm1b[1] + m11;
		rez[5] = pm1a[1] + pm1c[0] + m12;
		rez[6] = pm1b[0] + pm1c[1] + m13;
		rez[12] = ps1a[0] + ps1b[1] + s11;
		rez[13] = ps1a[1] + ps1c[0] + s12;
		rez[14] = ps1b[0] + ps1c[1] + s13;

	} else {
		/* calculate m2 and s2 on the spot */
		c1 = c2 = c3 = 0.0;
		m11 = m12 = m13 = 0.0;
		m21 = m22 = m23 = 0.0;
		s11 = s12 = s13 = 0.0;
		s21 = s22 = s23 = 0.0;

		__m128d xc_12, xc_34, xc_56;
		__m128d xm1_12, xm1_34, xm1_56, xs1_12, xs1_34, xs1_56;
		__m128d xm2_12, xm2_34, xm2_56, xs2_12, xs2_34, xs2_56;

		xc_12 = xc_34 = xc_56 = xm1_12 = xm1_34 = xm1_56 = xs1_12 =
		    xs1_34 = xs1_56 = xm2_12 = xm2_34 = xm2_56 = xs2_12 =
		    xs2_34 = xs2_56 = _mm_setzero_pd();

		for (i = 0; i < height; i++) {
			for (j = 0; j <= (size_row - 6); j += 6) {
				__m128d xs10, xs11, xs20, xs21;
				xs10 =
				    _mm_cvtps_pd(*((__m128 *)(psrc1 + j)));
				xs11 =
				    _mm_cvtps_pd(*((__m128 *)(psrc1 + j + 2)));
				xs20 =
				    _mm_cvtps_pd(*((__m128 *)(psrc2 + j)));
				xs21 =
				    _mm_cvtps_pd(*((__m128 *)(psrc2 + j + 2)));

				xm1_12 = _mm_add_pd(xm1_12, xs10);
				xm1_34 = _mm_add_pd(xm1_34, xs11);
				xm2_12 = _mm_add_pd(xm2_12, xs20);
				xm2_34 = _mm_add_pd(xm2_34, xs21);

				xs1_12 = _mm_add_pd(xs1_12,
				    _mm_mul_pd(xs10, xs10));
				xs1_34 = _mm_add_pd(xs1_34,
				    _mm_mul_pd(xs11, xs11));
				xs2_12 = _mm_add_pd(xs2_12,
				    _mm_mul_pd(xs20, xs20));
				xs2_34 = _mm_add_pd(xs2_34,
				    _mm_mul_pd(xs21, xs21));

				xc_12 = _mm_add_pd(xc_12,
				    _mm_mul_pd(xs10, xs20));
				xc_34 = _mm_add_pd(xc_34,
				    _mm_mul_pd(xs11, xs21));

				xs10 =
				    _mm_cvtps_pd(*((__m128 *)(psrc1 + j + 4)));
				xs20 =
				    _mm_cvtps_pd(*((__m128 *)(psrc2 + j + 4)));
				xm1_56 = _mm_add_pd(xm1_56, xs10);
				xm2_56 = _mm_add_pd(xm2_56, xs20);
				xs1_56 = _mm_add_pd(xs1_56,
				    _mm_mul_pd(xs10, xs10));
				xs2_56 = _mm_add_pd(xs2_56,
				    _mm_mul_pd(xs20, xs20));
				xc_56 = _mm_add_pd(xc_56,
				    _mm_mul_pd(xs10, xs20));
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

		__m128d xca, xm1a, xs1a, xm2a, xs2a;
		__m128d xcb, xm1b, xs1b, xm2b, xs2b;
		__m128d xcc, xm1c, xs1c, xm2c, xs2c;
		mlib_d64 *pca, *pm1a, *ps1a, *pm2a, *ps2a;
		mlib_d64 *pcb, *pm1b, *ps1b, *pm2b, *ps2b;
		mlib_d64 *pcc, *pm1c, *ps1c, *pm2c, *ps2c;

		pca = (mlib_d64 *)&xca;
		pm1a = (mlib_d64 *)&xm1a;
		ps1a = (mlib_d64 *)&xs1a;
		pm2a = (mlib_d64 *)&xm2a;
		ps2a = (mlib_d64 *)&xs2a;

		pcb = (mlib_d64 *)&xcb;
		pm1b = (mlib_d64 *)&xm1b;
		ps1b = (mlib_d64 *)&xs1b;
		pm2b = (mlib_d64 *)&xm2b;
		ps2b = (mlib_d64 *)&xs2b;

		pcc = (mlib_d64 *)&xcc;
		pm1c = (mlib_d64 *)&xm1c;
		ps1c = (mlib_d64 *)&xs1c;
		pm2c = (mlib_d64 *)&xm2c;
		ps2c = (mlib_d64 *)&xs2c;

		_mm_store_pd((pca), xc_12);
		_mm_store_pd((pcb), xc_34);
		_mm_store_pd((pcc), xc_56);
		_mm_store_pd((pm1a), xm1_12);
		_mm_store_pd((pm1b), xm1_34);
		_mm_store_pd((pm1c), xm1_56);
		_mm_store_pd((ps1a), xs1_12);
		_mm_store_pd((ps1b), xs1_34);
		_mm_store_pd((ps1c), xs1_56);
		_mm_store_pd((pm2a), xm2_12);
		_mm_store_pd((pm2b), xm2_34);
		_mm_store_pd((pm2c), xm2_56);
		_mm_store_pd((ps2a), xs2_12);
		_mm_store_pd((ps2b), xs2_34);
		_mm_store_pd((ps2c), xs2_56);

		rez[0] = pca[0] + pcb[1] + c1;
		rez[1] = pca[1] + pcc[0] + c2;
		rez[2] = pcb[0] + pcc[1] + c3;
		rez[4] = pm1a[0] + pm1b[1] + m11;
		rez[5] = pm1a[1] + pm1c[0] + m12;
		rez[6] = pm1b[0] + pm1c[1] + m13;
		rez[8] = pm2a[0] + pm2b[1] + m21;
		rez[9] = pm2a[1] + pm2c[0] + m22;
		rez[10] = pm2b[0] + pm2c[1] + m23;
		rez[12] = ps1a[0] + ps1b[1] + s11;
		rez[13] = ps1a[1] + ps1c[0] + s12;
		rez[14] = ps1b[0] + ps1c[1] + s13;
		rez[16] = ps2a[0] + ps2b[1] + s21;
		rez[17] = ps2a[1] + ps2c[0] + s22;
		rez[18] = ps2b[0] + ps2c[1] + s23;
	}
}

/* *********************************************************** */
