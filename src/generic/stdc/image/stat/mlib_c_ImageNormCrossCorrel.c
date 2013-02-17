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

#pragma ident	"@(#)mlib_c_ImageNormCrossCorrel.c	9.3	07/10/09 SMI"

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
#include <mlib_c_ImageCorrel.h>
#include <mlib_ImageDivTables.h>
#include <mlib_SysMath.h>
#include <stdio.h>

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

	switch (mlib_ImageGetType(img1)) {
/* handle MLIB_BYTE data type of image */
	case MLIB_BYTE:

		if (mlib_ImageGetChannels(img1) == 3)
			mlib_c_ImageNormCrossCorrel_U8_3(img1, img2,
					mean2, sdev2, res);
		else
			mlib_c_ImageNormCrossCorrel_U8_124(img1, img2,
					mean2, sdev2, res);
		break;

/* handle MLIB_USHORT data type of image */
	case MLIB_USHORT:

		if (mlib_ImageGetChannels(img1) == 3)
			mlib_c_ImageNormCrossCorrel_U16_3(img1, img2,
					mean2, sdev2, res);
		else
			mlib_c_ImageNormCrossCorrel_U16_124(img1, img2,
					mean2, sdev2, res);
		break;

/* handle MLIB_SHORT data type of image */
	case MLIB_SHORT:

		if (mlib_ImageGetChannels(img1) == 3)
			mlib_c_ImageNormCrossCorrel_S16_3(img1, img2,
					mean2, sdev2, res);
		else
			mlib_c_ImageNormCrossCorrel_S16_124(img1, img2,
					mean2, sdev2, res);
		break;

/* handle MLIB_INT data type of image */
	case MLIB_INT:

		if (mlib_ImageGetChannels(img1) == 3)
			mlib_c_ImageNormCrossCorrel_S32_3(img1, img2,
					mean2, sdev2, res);
		else
			mlib_c_ImageNormCrossCorrel_S32_124(img1, img2,
					mean2, sdev2, res);
		break;

/* discard any other data types */
	default:
		return (MLIB_FAILURE);
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

#define	MUL_U8(x, y)	(mlib_U82D64[(x)] * mlib_U82D64[(y)])

/* *********************************************************** */
void
mlib_c_ImageNormCrossCorrel_U8_124(
    const mlib_image *img1,
    const mlib_image *img2,
    const mlib_d64 *m2,
    const mlib_d64 *s2,
    mlib_d64 *res)
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
	mlib_s32 nchan = mlib_ImageGetChannels(img1);

/* number of elements in the row */
	mlib_s32 size_row = mlib_ImageGetWidth(img1) << (nchan >> 1);

/* indices */
	mlib_s32 i, j;
	mlib_u8 *psrc2;
/* accumulators by channels */
	mlib_d64 ccInterMul1, ccInterMul2, ccInterMul3, ccInterMul4;
	mlib_d64 ccSelfMul11, ccSelfMul12, ccSelfMul13, ccSelfMul14;
	mlib_d64 ccSelfMul21, ccSelfMul22, ccSelfMul23, ccSelfMul24;
	mlib_d64 sum11, sum12, sum13, sum14;
	mlib_d64 sum21, sum22, sum23, sum24;

/* values by channels */
	mlib_d64 d1, d2, d3, d4;
	mlib_d64 d21, d22, d23, d24;

	if (src1_stride == size_row && src1_stride == src2_stride) {
/* This images is not a sub-images and can be treated as a 1-D vectors */
		size_row *= height;
		height = 1;
	}

	for (i = 0; i < 20; i++) {
		res[i] = 0.0;
	}

	ccInterMul1 = ccInterMul2 = 0;
	ccInterMul3 = ccInterMul4 = 0;
	ccSelfMul11 = ccSelfMul12 = 0;
	ccSelfMul13 = ccSelfMul14 = 0;
	ccSelfMul21 = ccSelfMul22 = 0;
	ccSelfMul23 = ccSelfMul24 = 0;
	sum11 = sum12 = sum13 = sum14 = 0;
	sum21 = sum22 = sum23 = sum24 = 0;

	if ((m2 != NULL) && (s2 != NULL)) {
		if (size_row < 4) {
			for (i = 0; i < height; i++) {
#ifdef __SUNPRO_C
#pragma unroll(1)
#endif /* __SUNPRO_C */
				for (j = 0; j < size_row; j++) {
					res[j] += point_src1[j] *
						(mlib_d64)point_src2[j];
					res[j + 4] += point_src1[j]
						* point_src1[j];
					res[j + 12] += point_src1[j];

				}
				point_src1 += src1_stride;
				point_src2 += src2_stride;
			}
			return;
		}

		for (i = 0; i < height; i++) {
			mlib_s32 nalign = (mlib_addr)point_src1 & 3;
			mlib_s32 nbytes = size_row + nalign;
			mlib_s32 *p32 = (mlib_s32 *)(point_src1 - nalign);
			mlib_s32 val = *(p32++);

#ifdef _LITTLE_ENDIAN
			mlib_u32 mask = (mlib_u32)0xffffffff << (nalign << 3);
#else /* _LITTLE_ENDIAN */
			mlib_u32 mask = (mlib_u32)0xffffffff >> (nalign << 3);
#endif /* _LITTLE_ENDIAN */
			val &= mask;
			psrc2 = point_src2 - nalign;

// if (((mlib_addr)psrc2 & 7) != ((mlib_addr)point_src2 & 7)) {
			if (nalign != 0) {
#ifdef _LITTLE_ENDIAN
				d2 = *(mlib_d64 *)((mlib_u8 *)mlib_U82D64 +
				    ((val >> 5) & 0x7f8));
				d3 = *(mlib_d64 *)((mlib_u8 *)mlib_U82D64 +
				    ((val >> 13) & 0x7f8));
				d4 = *(mlib_d64 *)((mlib_u8 *)mlib_U82D64 +
				    ((val >> 21) & 0x7f8));

#else /* _LITTLE_ENDIAN */
				d2 = *(mlib_d64 *)((mlib_u8 *)mlib_U82D64 +
				    ((val >> 13) & 0x7f8));
				d3 = *(mlib_d64 *)((mlib_u8 *)mlib_U82D64 +
				    ((val >> 5) & 0x7f8));
				d4 = *(mlib_d64 *)((mlib_u8 *)mlib_U82D64 +
				    ((val << 3) & 0x7f8));
#endif /* _LITTLE_ENDIAN */
				val = *(p32++);

				d21 = mlib_U82D64[psrc2[0]];
				d22 = mlib_U82D64[psrc2[1]];
				d23 = mlib_U82D64[psrc2[2]];
				d24 = mlib_U82D64[psrc2[3]];

				if (nalign < 3) {
					ccInterMul3 += d3 * d23;
					ccSelfMul13 += d3 * d3;
					sum13 += d3;

					if (nalign < 2) {
						ccInterMul2 += d2 * d22;
						ccSelfMul12 += d2 * d2;
						sum12 += d2;

					}
				}
				ccInterMul4 += d4 * d24;
				ccSelfMul14 += d4 * d4;
				sum14 += d4;

				psrc2 += 4;
				nbytes -= 4;
			}

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j = 0; j <= (nbytes - 8); j += 4) {
#ifdef _LITTLE_ENDIAN
				d1 = *(mlib_d64 *)((mlib_u8 *)mlib_U82D64 +
				    ((val << 3) & 0x7f8));
				d2 = *(mlib_d64 *)((mlib_u8 *)mlib_U82D64 +
				    ((val >> 5) & 0x7f8));
				d3 = *(mlib_d64 *)((mlib_u8 *)mlib_U82D64 +
				    ((val >> 13) & 0x7f8));
				d4 = *(mlib_d64 *)((mlib_u8 *)mlib_U82D64 +
				    ((val >> 21) & 0x7f8));
#else /* _LITTLE_ENDIAN */
				d1 = *(mlib_d64 *)((mlib_u8 *)mlib_U82D64 +
				    ((val >> 21) & 0x7f8));
				d2 = *(mlib_d64 *)((mlib_u8 *)mlib_U82D64 +
				    ((val >> 13) & 0x7f8));
				d3 = *(mlib_d64 *)((mlib_u8 *)mlib_U82D64 +
				    ((val >> 5) & 0x7f8));
				d4 = *(mlib_d64 *)((mlib_u8 *)mlib_U82D64 +
				    ((val << 3) & 0x7f8));
#endif /* _LITTLE_ENDIAN */
				val = *(p32++);

				d21 = mlib_U82D64[psrc2[j]];
				d22 = mlib_U82D64[psrc2[j + 1]];
				d23 = mlib_U82D64[psrc2[j + 2]];
				d24 = mlib_U82D64[psrc2[j + 3]];

				ccInterMul1 += d1 * d21;
				ccSelfMul11 += d1 * d1;
				sum11 += d1;

				ccInterMul2 += d2 * d22;
				ccSelfMul12 += d2 * d2;
				sum12 += d2;

				ccInterMul3 += d3 * d23;
				ccSelfMul13 += d3 * d3;
				sum13 += d3;

				ccInterMul4 += d4 * d24;
				ccSelfMul14 += d4 * d4;
				sum14 += d4;

			}

			if (j <= (nbytes - 4)) {
#ifdef _LITTLE_ENDIAN
				d1 = *(mlib_d64 *)((mlib_u8 *)mlib_U82D64 +
				    ((val << 3) & 0x7f8));
				d2 = *(mlib_d64 *)((mlib_u8 *)mlib_U82D64 +
				    ((val >> 5) & 0x7f8));
				d3 = *(mlib_d64 *)((mlib_u8 *)mlib_U82D64 +
				    ((val >> 13) & 0x7f8));
				d4 = *(mlib_d64 *)((mlib_u8 *)mlib_U82D64 +
				    ((val >> 21) & 0x7f8));
#else /* _LITTLE_ENDIAN */
				d1 = *(mlib_d64 *)((mlib_u8 *)mlib_U82D64 +
				    ((val >> 21) & 0x7f8));
				d2 = *(mlib_d64 *)((mlib_u8 *)mlib_U82D64 +
				    ((val >> 13) & 0x7f8));
				d3 = *(mlib_d64 *)((mlib_u8 *)mlib_U82D64 +
				    ((val >> 5) & 0x7f8));
				d4 = *(mlib_d64 *)((mlib_u8 *)mlib_U82D64 +
				    ((val << 3) & 0x7f8));
#endif /* _LITTLE_ENDIAN */

				d21 = mlib_U82D64[psrc2[j]];
				d22 = mlib_U82D64[psrc2[j + 1]];
				d23 = mlib_U82D64[psrc2[j + 2]];
				d24 = mlib_U82D64[psrc2[j + 3]];

				ccInterMul1 += d1 * d21;
				ccSelfMul11 += d1 * d1;
				sum11 += d1;

				ccInterMul2 += d2 * d22;
				ccSelfMul12 += d2 * d2;
				sum12 += d2;

				ccInterMul3 += d3 * d23;
				ccSelfMul13 += d3 * d3;
				sum13 += d3;

				ccInterMul4 += d4 * d24;
				ccSelfMul14 += d4 * d4;
				sum14 += d4;

				j += 4;
				nbytes &= 3;
				if (nbytes)
					val = *p32;
			}

			if (nbytes) {
#ifdef _LITTLE_ENDIAN
				mask = (mlib_u32)0xffffffff << (nbytes << 3);
				val = val & ~mask;
				d1 = *(mlib_d64 *)((mlib_u8 *)mlib_U82D64 +
				    ((val << 3) & 0x7f8));
				d2 = *(mlib_d64 *)((mlib_u8 *)mlib_U82D64 +
				    ((val >> 5) & 0x7f8));
				d3 = *(mlib_d64 *)((mlib_u8 *)mlib_U82D64 +
				    ((val >> 13) & 0x7f8));
#else /* _LITTLE_ENDIAN */
				mask = (mlib_u32)0xffffffff >> (nbytes << 3);
				val = val & ~mask;
				d1 = *(mlib_d64 *)((mlib_u8 *)mlib_U82D64 +
				    ((val >> 21) & 0x7f8));
				d2 = *(mlib_d64 *)((mlib_u8 *)mlib_U82D64 +
				    ((val >> 13) & 0x7f8));
				d3 = *(mlib_d64 *)((mlib_u8 *)mlib_U82D64 +
				    ((val >> 5) & 0x7f8));
#endif /* _LITTLE_ENDIAN */
				d21 = mlib_U82D64[psrc2[j]];
				d22 = mlib_U82D64[psrc2[j + 1]];
				d23 = mlib_U82D64[psrc2[j + 2]];
				ccInterMul1 += d1 * d21;
				ccSelfMul11 += d1 * d1;
				sum11 += d1;

				if (nbytes > 1) {
					ccInterMul2 += d2 * d22;
					ccSelfMul12 += d2 * d2;
					sum12 += d2;

					if (nbytes > 2) {
						ccInterMul3 += d3 * d23;
						ccSelfMul13 += d3 * d3;
						sum13 += d3;

					}
				}
			}

			res[3 & (0 - nalign)] += ccInterMul1;
			res[(3 & (0 - nalign)) + 4] += ccSelfMul11;
			res[(3 & (0 - nalign)) + 12] += sum11;

			res[3 & (1 - nalign)] += ccInterMul2;
			res[(3 & (1 - nalign)) + 4] += ccSelfMul12;
			res[(3 & (1 - nalign)) + 12] += sum12;

			res[3 & (2 - nalign)] += ccInterMul3;
			res[(3 & (2 - nalign)) + 4] += ccSelfMul13;
			res[(3 & (2 - nalign)) + 12] += sum13;

			res[3 & (3 - nalign)] += ccInterMul4;
			res[(3 & (3 - nalign)) + 4] += ccSelfMul14;
			res[(3 & (3 - nalign)) + 12] += sum14;

			ccInterMul1 = ccInterMul2 = 0;
			ccInterMul3 = ccInterMul4 = 0;
			ccSelfMul11 = ccSelfMul12 = 0;
			ccSelfMul13 = ccSelfMul14 = 0;
			sum11 = sum12 = sum13 = sum14 = 0;

			point_src1 += src1_stride;
			point_src2 += src2_stride;
		}
	} else {
	/* m2 == NULL || s2 == NULL */
		if (size_row < 4) {
			for (i = 0; i < height; i++) {
#ifdef __SUNPRO_C
#pragma unroll(1)
#endif /* __SUNPRO_C */
				for (j = 0; j < size_row; j++) {
					res[j] += point_src1[j] *
						(mlib_d64)point_src2[j];
					res[j + 4] += point_src1[j]
						* point_src1[j];
					res[j + 8] += point_src2[j]
						* point_src2[j];
					res[j + 12] += point_src1[j];
					res[j + 16] += point_src2[j];

				}
				point_src1 += src1_stride;
				point_src2 += src2_stride;
			}
			return;
		}

		for (i = 0; i < height; i++) {
			mlib_s32 nalign = (mlib_addr)point_src1 & 3;
			mlib_s32 nbytes = size_row + nalign;
			mlib_s32 *p32 = (mlib_s32 *)(point_src1 - nalign);
			mlib_s32 val = *(p32++);

#ifdef _LITTLE_ENDIAN
			mlib_u32 mask = (mlib_u32)0xffffffff << (nalign << 3);
#else /* _LITTLE_ENDIAN */
			mlib_u32 mask = (mlib_u32)0xffffffff >> (nalign << 3);
#endif /* _LITTLE_ENDIAN */
			val &= mask;
			psrc2 = point_src2 - nalign;

// if (((mlib_addr)psrc2 & 7) != ((mlib_addr)point_src2 & 7)) {
			if (nalign != 0) {
#ifdef _LITTLE_ENDIAN
				d2 = *(mlib_d64 *)((mlib_u8 *)mlib_U82D64 +
				    ((val >> 5) & 0x7f8));
				d3 = *(mlib_d64 *)((mlib_u8 *)mlib_U82D64 +
				    ((val >> 13) & 0x7f8));
				d4 = *(mlib_d64 *)((mlib_u8 *)mlib_U82D64 +
				    ((val >> 21) & 0x7f8));

#else /* _LITTLE_ENDIAN */
				d2 = *(mlib_d64 *)((mlib_u8 *)mlib_U82D64 +
				    ((val >> 13) & 0x7f8));
				d3 = *(mlib_d64 *)((mlib_u8 *)mlib_U82D64 +
				    ((val >> 5) & 0x7f8));
				d4 = *(mlib_d64 *)((mlib_u8 *)mlib_U82D64 +
				    ((val << 3) & 0x7f8));
#endif /* _LITTLE_ENDIAN */
				val = *(p32++);

				d21 = mlib_U82D64[psrc2[0]];
				d22 = mlib_U82D64[psrc2[1]];
				d23 = mlib_U82D64[psrc2[2]];
				d24 = mlib_U82D64[psrc2[3]];

				if (nalign < 3) {
				    ccInterMul3 += d3 * d23;
					ccSelfMul13 += d3 * d3;
					ccSelfMul23 += d23 * d23;
					sum13 += d3;
					sum23 += d23;

					if (nalign < 2) {
						ccInterMul2 += d2 * d22;
						ccSelfMul12 += d2 * d2;
						ccSelfMul22 += d22 * d22;
						sum12 += d2;
						sum22 += d22;
					}
				}
				ccInterMul4 += d4 * d24;
				ccSelfMul14 += d4 * d4;
				ccSelfMul24 += d24 * d24;
				sum14 += d4;
				sum24 += d24;

				psrc2 += 4;
				nbytes -= 4;
			}

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j = 0; j <= (nbytes - 8); j += 4) {
#ifdef _LITTLE_ENDIAN
				d1 = *(mlib_d64 *)((mlib_u8 *)mlib_U82D64 +
				    ((val << 3) & 0x7f8));
				d2 = *(mlib_d64 *)((mlib_u8 *)mlib_U82D64 +
				    ((val >> 5) & 0x7f8));
				d3 = *(mlib_d64 *)((mlib_u8 *)mlib_U82D64 +
				    ((val >> 13) & 0x7f8));
				d4 = *(mlib_d64 *)((mlib_u8 *)mlib_U82D64 +
				    ((val >> 21) & 0x7f8));
#else /* _LITTLE_ENDIAN */
				d1 = *(mlib_d64 *)((mlib_u8 *)mlib_U82D64 +
				    ((val >> 21) & 0x7f8));
				d2 = *(mlib_d64 *)((mlib_u8 *)mlib_U82D64 +
				    ((val >> 13) & 0x7f8));
				d3 = *(mlib_d64 *)((mlib_u8 *)mlib_U82D64 +
				    ((val >> 5) & 0x7f8));
				d4 = *(mlib_d64 *)((mlib_u8 *)mlib_U82D64 +
				    ((val << 3) & 0x7f8));
#endif /* _LITTLE_ENDIAN */
				val = *(p32++);

				d21 = mlib_U82D64[psrc2[j]];
				d22 = mlib_U82D64[psrc2[j + 1]];
				d23 = mlib_U82D64[psrc2[j + 2]];
				d24 = mlib_U82D64[psrc2[j + 3]];

				ccInterMul1 += d1 * d21;
				ccSelfMul11 += d1 * d1;
				ccSelfMul21 += d21 * d21;
				sum11 += d1;
				sum21 += d21;

				ccInterMul2 += d2 * d22;
				ccSelfMul12 += d2 * d2;
				ccSelfMul22 += d22 * d22;
				sum12 += d2;
				sum22 += d22;

				ccInterMul3 += d3 * d23;
				ccSelfMul13 += d3 * d3;
				ccSelfMul23 += d23 * d23;
				sum13 += d3;
				sum23 += d23;

				ccInterMul4 += d4 * d24;
				ccSelfMul14 += d4 * d4;
				ccSelfMul24 += d24 * d24;
				sum14 += d4;
				sum24 += d24;

			}

			if (j <= (nbytes - 4)) {
#ifdef _LITTLE_ENDIAN
				d1 = *(mlib_d64 *)((mlib_u8 *)mlib_U82D64 +
				    ((val << 3) & 0x7f8));
				d2 = *(mlib_d64 *)((mlib_u8 *)mlib_U82D64 +
				    ((val >> 5) & 0x7f8));
				d3 = *(mlib_d64 *)((mlib_u8 *)mlib_U82D64 +
				    ((val >> 13) & 0x7f8));
				d4 = *(mlib_d64 *)((mlib_u8 *)mlib_U82D64 +
				    ((val >> 21) & 0x7f8));
#else /* _LITTLE_ENDIAN */
				d1 = *(mlib_d64 *)((mlib_u8 *)mlib_U82D64 +
				    ((val >> 21) & 0x7f8));
				d2 = *(mlib_d64 *)((mlib_u8 *)mlib_U82D64 +
				    ((val >> 13) & 0x7f8));
				d3 = *(mlib_d64 *)((mlib_u8 *)mlib_U82D64 +
				    ((val >> 5) & 0x7f8));
				d4 = *(mlib_d64 *)((mlib_u8 *)mlib_U82D64 +
				    ((val << 3) & 0x7f8));
#endif /* _LITTLE_ENDIAN */

				d21 = mlib_U82D64[psrc2[j]];
				d22 = mlib_U82D64[psrc2[j + 1]];
				d23 = mlib_U82D64[psrc2[j + 2]];
				d24 = mlib_U82D64[psrc2[j + 3]];

				ccInterMul1 += d1 * d21;
				ccSelfMul11 += d1 * d1;
				ccSelfMul21 += d21 * d21;
				sum11 += d1;
				sum21 += d21;

				ccInterMul2 += d2 * d22;
				ccSelfMul12 += d2 * d2;
				ccSelfMul22 += d22 * d22;
				sum12 += d2;
				sum22 += d22;

				ccInterMul3 += d3 * d23;
				ccSelfMul13 += d3 * d3;
				ccSelfMul23 += d23 * d23;
				sum13 += d3;
				sum23 += d23;

				ccInterMul4 += d4 * d24;
				ccSelfMul14 += d4 * d4;
				ccSelfMul24 += d24 * d24;
				sum14 += d4;
				sum24 += d24;

				j += 4;
				nbytes &= 3;
				if (nbytes)
					val = *p32;
			}

			if (nbytes) {
#ifdef _LITTLE_ENDIAN
				mask = (mlib_u32)0xffffffff << (nbytes << 3);
				val = val & ~mask;
				d1 = *(mlib_d64 *)((mlib_u8 *)mlib_U82D64 +
				    ((val << 3) & 0x7f8));
				d2 = *(mlib_d64 *)((mlib_u8 *)mlib_U82D64 +
				    ((val >> 5) & 0x7f8));
				d3 = *(mlib_d64 *)((mlib_u8 *)mlib_U82D64 +
				    ((val >> 13) & 0x7f8));
#else /* _LITTLE_ENDIAN */
				mask = (mlib_u32)0xffffffff >> (nbytes << 3);
				val = val & ~mask;
				d1 = *(mlib_d64 *)((mlib_u8 *)mlib_U82D64 +
				    ((val >> 21) & 0x7f8));
				d2 = *(mlib_d64 *)((mlib_u8 *)mlib_U82D64 +
				    ((val >> 13) & 0x7f8));
				d3 = *(mlib_d64 *)((mlib_u8 *)mlib_U82D64 +
				    ((val >> 5) & 0x7f8));
#endif /* _LITTLE_ENDIAN */
				d21 = mlib_U82D64[psrc2[j]];
				d22 = mlib_U82D64[psrc2[j + 1]];
				d23 = mlib_U82D64[psrc2[j + 2]];
				ccInterMul1 += d1 * d21;
				ccSelfMul11 += d1 * d1;
				ccSelfMul21 += d21 * d21;
				sum11 += d1;
				sum21 += d21;

				if (nbytes > 1) {
					ccInterMul2 += d2 * d22;
					ccSelfMul12 += d2 * d2;
					ccSelfMul22 += d22 * d22;
					sum12 += d2;
					sum22 += d22;

					if (nbytes > 2) {
						ccInterMul3 += d3 * d23;
						ccSelfMul13 += d3 * d3;
						ccSelfMul23 += d23 * d23;
						sum13 += d3;
						sum23 += d23;

					}
				}
			}

			res[3 & (0 - nalign)] += ccInterMul1;
			res[(3 & (0 - nalign)) + 4] += ccSelfMul11;
			res[(3 & (0 - nalign)) + 8] += ccSelfMul21;
			res[(3 & (0 - nalign)) + 12] += sum11;
			res[(3 & (0 - nalign)) + 16] += sum21;

			res[3 & (1 - nalign)] += ccInterMul2;
			res[(3 & (1 - nalign)) + 4] += ccSelfMul12;
			res[(3 & (1 - nalign)) + 8] += ccSelfMul22;
			res[(3 & (1 - nalign)) + 12] += sum12;
			res[(3 & (1 - nalign)) + 16] += sum22;

			res[3 & (2 - nalign)] += ccInterMul3;
			res[(3 & (2 - nalign)) + 4] += ccSelfMul13;
			res[(3 & (2 - nalign)) + 8] += ccSelfMul23;
			res[(3 & (2 - nalign)) + 12] += sum13;
			res[(3 & (2 - nalign)) + 16] += sum23;

			res[3 & (3 - nalign)] += ccInterMul4;
			res[(3 & (3 - nalign)) + 4] += ccSelfMul14;
			res[(3 & (3 - nalign)) + 8] += ccSelfMul24;
			res[(3 & (3 - nalign)) + 12] += sum14;
			res[(3 & (3 - nalign)) + 16] += sum24;

			ccInterMul1 = ccInterMul2 = 0;
			ccInterMul3 = ccInterMul4 = 0;
			ccSelfMul11 = ccSelfMul12 = 0;
			ccSelfMul13 = ccSelfMul14 = 0;
			ccSelfMul21 = ccSelfMul22 = 0;
			ccSelfMul23 = ccSelfMul24 = 0;
			sum11 = sum12 = sum13 = sum14 = 0;
			sum21 = sum22 = sum23 = sum24 = 0;

			point_src1 += src1_stride;
			point_src2 += src2_stride;
		}
	}
}

/* *********************************************************** */

void
mlib_c_ImageNormCrossCorrel_U8_3(
    const mlib_image *img1,
    const mlib_image *img2,
    const mlib_d64 *m2,
    const mlib_d64 *s2,
    mlib_d64 *res)
{
/* pointer to the data of first image */
	mlib_u8 *point_src1 = (mlib_u8 *)mlib_ImageGetData(img1);

/* pointer to the data of second image */
	mlib_u8 *point_src2 = (mlib_u8 *)mlib_ImageGetData(img2);

/* accumulators by channels */
	mlib_d64 ccInterMul1, ccInterMul2, ccInterMul3;
	mlib_d64 ccSelfMul11, ccSelfMul12, ccSelfMul13;
	mlib_d64 ccSelfMul21, ccSelfMul22, ccSelfMul23;
	mlib_d64 sum11, sum12, sum13;
	mlib_d64 sum21, sum22, sum23;

/* height of image */
	mlib_s32 height = mlib_ImageGetHeight(img1);

/* elements to next row */
	mlib_s32 src1_stride = mlib_ImageGetStride(img1);

/* elements to next row */
	mlib_s32 src2_stride = mlib_ImageGetStride(img2);

/* number of elements in the row */
	mlib_s32 size_row = mlib_ImageGetWidth(img1) * 3;

/* indices */
	mlib_s32 i, j;
	mlib_u8 *psrc1, *psrc2;

	ccInterMul1 = ccInterMul2 = ccInterMul3 = 0;
	ccSelfMul11 = ccSelfMul12 = ccSelfMul13 = 0;
	ccSelfMul21 = ccSelfMul22 = ccSelfMul23 = 0;
	sum11 = sum12 = sum13 = 0;
	sum21 = sum22 = sum23 = 0;

	if (src1_stride == size_row && src1_stride == src2_stride) {
/* This images is not a sub-images and can be treated as a 1-D vectors */
		size_row *= height;
		height = 1;
	}

	if ((m2 != NULL) && (s2 != NULL)) {
		for (i = 0; i < height; i++) {
			psrc1 = point_src1;
			psrc2 = point_src2;
			j = 0;

			if ((mlib_addr)psrc1 & 1) {

				ccInterMul1 += MUL_U8(psrc1[j + 0],
						psrc2[j + 0]);
				ccSelfMul11 += MUL_U8(psrc1[j + 0],
						psrc1[j + 0]);
				sum11 += psrc1[j + 0];

				ccInterMul2 += MUL_U8(psrc1[j + 1],
						psrc2[j + 1]);
				ccSelfMul12 += MUL_U8(psrc1[j + 1],
						psrc1[j + 1]);
				sum12 += psrc1[j + 1];

				ccInterMul3 += MUL_U8(psrc1[j + 2],
						psrc2[j + 2]);
				ccSelfMul13 += MUL_U8(psrc1[j + 2],
						psrc1[j + 2]);
				sum13 += psrc1[j + 2];

				j += 3;
			}
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (; j <= (size_row - 6); j += 6) {
				mlib_u32 u1, u2, u3;
				mlib_u32 u[6] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0};

				u1 = ((mlib_u16 *)(psrc1 + j))[0];
				u2 = ((mlib_u16 *)(psrc1 + j))[1];
				u3 = ((mlib_u16 *)(psrc1 + j))[2];
#ifdef _LITTLE_ENDIAN

				u[0] = (u1 & 0xff);
				u[1] = (u1 >>8);
				u[2] = (u2 & 0xff);
				u[3] = (u2 >> 8);
				u[4] = (u3 & 0xff);
				u[5] = (u3 >> 8);

				ccInterMul1 += MUL_U8(u[0],
					psrc2[j + 0]);
				ccSelfMul11 += MUL_U8(u[0],
					u[0]);
				sum11 += u[0];

				ccInterMul2 += MUL_U8(u[1],
					psrc2[j + 1]);
				ccSelfMul12 += MUL_U8(u[1],
					u[1]);
				sum12 += u[1];

				ccInterMul3 += MUL_U8(u[2],
					psrc2[j + 2]);
				ccSelfMul13 += MUL_U8(u[2],
					u[2]);
				sum13 += u[2];

				ccInterMul1 += MUL_U8(u[3],
					psrc2[j + 3]);
				ccSelfMul11 += MUL_U8(u[3],
					u[3]);
				sum11 += u[3];

				ccInterMul2 += MUL_U8(u[4],
					psrc2[j + 4]);
				ccSelfMul12 += MUL_U8(u[4],
					u[4]);
				sum12 += u[4];

				ccInterMul3 += MUL_U8(u[5],
					psrc2[j + 5]);
				ccSelfMul13 += MUL_U8(u[5],
					u[5]);
				sum13 += u[5];

#else /* _LITTLE_ENDIAN */
				u[0] = (u1 >>8);
				u[1] = (u1 & 0xff);
				u[2] = (u2 >> 8);
				u[3] = (u2 & 0xff);
				u[4] = (u3 >> 8);
				u[5] = (u3 & 0xff);
				ccInterMul1 += MUL_U8(u[0],
					psrc2[j + 0]);
				ccSelfMul11 += MUL_U8(u[0],
					u[0]);
				sum11 += u[0];

				ccInterMul2 += MUL_U8(u[1],
					psrc2[j + 1]);
				ccSelfMul12 += MUL_U8(u[1],
					u[1]);
				sum12 += u[1];

				ccInterMul3 += MUL_U8(u[2],
					psrc2[j + 2]);
				ccSelfMul13 += MUL_U8(u[2],
					u[2]);
				sum13 += u[2];

				ccInterMul1 += MUL_U8(u[3],
					psrc2[j + 3]);
				ccSelfMul11 += MUL_U8(u[3],
					u[3]);
				sum11 += u[3];

				ccInterMul2 += MUL_U8(u[4],
					psrc2[j + 4]);
				ccSelfMul12 += MUL_U8(u[4],
					u[4]);
				sum12 += u[4];

				ccInterMul3 += MUL_U8(u[5],
					psrc2[j + 5]);
				ccSelfMul13 += MUL_U8(u[5],
					u[5]);
				sum13 += u[5];

#endif /* _LITTLE_ENDIAN */
			}

			if (j < size_row) {

				ccInterMul1 += MUL_U8(psrc1[j + 0],
						psrc2[j + 0]);
				ccSelfMul11 += MUL_U8(psrc1[j + 0],
						psrc1[j + 0]);
				sum11 += psrc1[j + 0];

				ccInterMul2 += MUL_U8(psrc1[j + 1],
						psrc2[j + 1]);
				ccSelfMul12 += MUL_U8(psrc1[j + 1],
						psrc1[j + 1]);
				sum12 += psrc1[j + 1];

				ccInterMul3 += MUL_U8(psrc1[j + 2],
						psrc2[j + 2]);
				ccSelfMul13 += MUL_U8(psrc1[j + 2],
						psrc1[j + 2]);
				sum13 += psrc1[j + 2];

			}

			point_src1 += src1_stride;
			point_src2 += src2_stride;
		}

		res[0] += ccInterMul1;
		res[0 + 4] += ccSelfMul11;
		res[0 + 12] += sum11;

		res[1] += ccInterMul2;
		res[1 + 4] += ccSelfMul12;
		res[1 + 12] += sum12;

		res[2] += ccInterMul3;
		res[2 + 4] += ccSelfMul13;
		res[2 + 12] += sum13;

	} else {

	/* m2 == NULL || s2 == NULL */
		for (i = 0; i < height; i++) {
			psrc1 = point_src1;
			psrc2 = point_src2;
			j = 0;

			if ((mlib_addr)psrc1 & 1) {

				ccInterMul1 += MUL_U8(psrc1[j + 0],
						psrc2[j + 0]);
				ccSelfMul11 += MUL_U8(psrc1[j + 0],
						psrc1[j + 0]);
				ccSelfMul21 += MUL_U8(psrc2[j + 0],
						psrc2[j + 0]);
				sum11 += psrc1[j + 0];
				sum21 += psrc2[j + 0];

				ccInterMul2 += MUL_U8(psrc1[j + 1],
						psrc2[j + 1]);
				ccSelfMul12 += MUL_U8(psrc1[j + 1],
						psrc1[j + 1]);
				ccSelfMul22 += MUL_U8(psrc2[j + 1],
						psrc2[j + 1]);
				sum12 += psrc1[j + 1];
				sum22 += psrc2[j + 1];

				ccInterMul3 += MUL_U8(psrc1[j + 2],
						psrc2[j + 2]);
				ccSelfMul13 += MUL_U8(psrc1[j + 2],
						psrc1[j + 2]);
				ccSelfMul23 += MUL_U8(psrc2[j + 2],
						psrc2[j + 2]);
				sum13 += psrc1[j + 2];
				sum23 += psrc2[j + 2];

				j += 3;
			}
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (; j <= (size_row - 6); j += 6) {
				mlib_u32 u1, u2, u3;
				mlib_u32 u[6] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0};

				u1 = ((mlib_u16 *)(psrc1 + j))[0];
				u2 = ((mlib_u16 *)(psrc1 + j))[1];
				u3 = ((mlib_u16 *)(psrc1 + j))[2];
#ifdef _LITTLE_ENDIAN

				u[0] = (u1 & 0xff);
				u[1] = (u1 >>8);
				u[2] = (u2 & 0xff);
				u[3] = (u2 >> 8);
				u[4] = (u3 & 0xff);
				u[5] = (u3 >> 8);

				ccInterMul1 += MUL_U8(u[0],
					psrc2[j + 0]);
				ccSelfMul11 += MUL_U8(u[0],
					u[0]);
				ccSelfMul21 += MUL_U8(
					psrc2[j + 0],
					psrc2[j + 0]);
				sum11 += u[0];
				sum21 += psrc2[j + 0];

				ccInterMul2 += MUL_U8(u[1],
					psrc2[j + 1]);
				ccSelfMul12 += MUL_U8(u[1],
					u[1]);
				ccSelfMul22 += MUL_U8(
					psrc2[j + 1],
					psrc2[j + 1]);
				sum12 += u[1];
				sum22 += psrc2[j + 1];

				ccInterMul3 += MUL_U8(u[2],
					psrc2[j + 2]);
				ccSelfMul13 += MUL_U8(u[2],
					u[2]);
				ccSelfMul23 += MUL_U8(
					psrc2[j + 2],
					psrc2[j + 2]);
				sum13 += u[2];
				sum23 += psrc2[j + 2];

				ccInterMul1 += MUL_U8(u[3],
					psrc2[j + 3]);
				ccSelfMul11 += MUL_U8(u[3],
					u[3]);
				ccSelfMul21 += MUL_U8(
					psrc2[j + 3],
					psrc2[j + 3]);
				sum11 += u[3];
				sum21 += psrc2[j + 3];

				ccInterMul2 += MUL_U8(u[4],
					psrc2[j + 4]);
				ccSelfMul12 += MUL_U8(u[4],
					u[4]);
				ccSelfMul22 += MUL_U8(
					psrc2[j + 4],
					psrc2[j + 4]);
				sum12 += u[4];
				sum22 += psrc2[j + 4];

				ccInterMul3 += MUL_U8(u[5],
					psrc2[j + 5]);
				ccSelfMul13 += MUL_U8(u[5],
					u[5]);
				ccSelfMul23 += MUL_U8(
					psrc2[j + 5],
					psrc2[j + 5]);
				sum13 += u[5];
				sum23 += psrc2[j + 5];

#else /* _LITTLE_ENDIAN */
				u[0] = (u1 >>8);
				u[1] = (u1 & 0xff);
				u[2] = (u2 >> 8);
				u[3] = (u2 & 0xff);
				u[4] = (u3 >> 8);
				u[5] = (u3 & 0xff);

				ccInterMul1 += MUL_U8(u[0],
					psrc2[j + 0]);
				ccSelfMul11 += MUL_U8(u[0],
					u[0]);
				ccSelfMul21 += MUL_U8(
					psrc2[j + 0],
					psrc2[j + 0]);
				sum11 += u[0];
				sum21 += psrc2[j + 0];

				ccInterMul2 += MUL_U8(u[1],
					psrc2[j + 1]);
				ccSelfMul12 += MUL_U8(u[1],
					u[1]);
				ccSelfMul22 += MUL_U8(
					psrc2[j + 1],
					psrc2[j + 1]);
				sum12 += u[1];
				sum22 += psrc2[j + 1];

				ccInterMul3 += MUL_U8(u[2],
					psrc2[j + 2]);
				ccSelfMul13 += MUL_U8(u[2],
					u[2]);
				ccSelfMul23 += MUL_U8(
					psrc2[j + 2],
					psrc2[j + 2]);
				sum13 += u[2];
				sum23 += psrc2[j + 2];

				ccInterMul1 += MUL_U8(u[3],
					psrc2[j + 3]);
				ccSelfMul11 += MUL_U8(u[3],
					u[3]);
				ccSelfMul21 += MUL_U8(
					psrc2[j + 3],
					psrc2[j + 3]);
				sum11 += u[3];
				sum21 += psrc2[j + 3];

				ccInterMul2 += MUL_U8(u[4],
					psrc2[j + 4]);
				ccSelfMul12 += MUL_U8(u[4],
					u[4]);
				ccSelfMul22 += MUL_U8(
					psrc2[j + 4],
					psrc2[j + 4]);
				sum12 += u[4];
				sum22 += psrc2[j + 4];

				ccInterMul3 += MUL_U8(u[5],
					psrc2[j + 5]);
				ccSelfMul13 += MUL_U8(u[5],
					u[5]);
				ccSelfMul23 += MUL_U8(
					psrc2[j + 5],
					psrc2[j + 5]);
				sum13 += u[5];
				sum23 += psrc2[j + 5];

#endif /* _LITTLE_ENDIAN */
			}

			if (j < size_row) {

				ccInterMul1 += MUL_U8(psrc1[j + 0],
						psrc2[j + 0]);
				ccSelfMul11 += MUL_U8(psrc1[j + 0],
						psrc1[j + 0]);
				ccSelfMul21 += MUL_U8(psrc2[j + 0],
						psrc2[j + 0]);
				sum11 += psrc1[j + 0];
				sum21 += psrc2[j + 0];

				ccInterMul2 += MUL_U8(psrc1[j + 1],
						psrc2[j + 1]);
				ccSelfMul12 += MUL_U8(psrc1[j + 1],
						psrc1[j + 1]);
				ccSelfMul22 += MUL_U8(psrc2[j + 1],
						psrc2[j + 1]);
				sum12 += psrc1[j + 1];
				sum22 += psrc2[j + 1];

				ccInterMul3 += MUL_U8(psrc1[j + 2],
						psrc2[j + 2]);
				ccSelfMul13 += MUL_U8(psrc1[j + 2],
						psrc1[j + 2]);
				ccSelfMul23 += MUL_U8(psrc2[j + 2],
						psrc2[j + 2]);
				sum13 += psrc1[j + 2];
				sum23 += psrc2[j + 2];

			}

			point_src1 += src1_stride;
			point_src2 += src2_stride;
		}

		res[0] += ccInterMul1;
		res[0 + 4] += ccSelfMul11;
		res[0 + 8] += ccSelfMul21;
		res[0 + 12] += sum11;
		res[0 + 16] += sum21;

		res[1] += ccInterMul2;
		res[1 + 4] += ccSelfMul12;
		res[1 + 8] += ccSelfMul22;
		res[1 + 12] += sum12;
		res[1 + 16] += sum22;

		res[2] += ccInterMul3;
		res[2 + 4] += ccSelfMul13;
		res[2 + 8] += ccSelfMul23;
		res[2 + 12] += sum13;
		res[2 + 16] += sum23;
	}
}
/* *********************************************************** */

#define	MUL_S16(x, y)	((x) * (mlib_d64) (y))

/* *********************************************************** */

void
mlib_c_ImageNormCrossCorrel_U16_124(
    const mlib_image *img1,
    const mlib_image *img2,
    const mlib_d64 *m2,
    const mlib_d64 *s2,
    mlib_d64 *res)
{
/* pointer to the data of first image */
	mlib_u16 *psrc1 = (mlib_u16 *)mlib_ImageGetData(img1);

/* pointer to the data of second image */
	mlib_u16 *psrc2 = (mlib_u16 *)mlib_ImageGetData(img2);

/* accumulators by channels */
	mlib_d64 ccInterMul[4] = {0.0, 0.0, 0.0, 0.0};
	mlib_d64 ccSelfMul1[4] = {0.0, 0.0, 0.0, 0.0};
	mlib_d64 ccSelfMul2[4] = {0.0, 0.0, 0.0, 0.0};
	mlib_d64 sum1[4] = {0.0, 0.0, 0.0, 0.0};
	mlib_d64 sum2[4] = {0.0, 0.0, 0.0, 0.0};

/* height of image */
	mlib_s32 height = mlib_ImageGetHeight(img1);

/* elements to next row */
	mlib_s32 src1_stride = mlib_ImageGetStride(img1) >> 1;

/* elements to next row */
	mlib_s32 src2_stride = mlib_ImageGetStride(img2) >> 1;
	mlib_s32 nchan = mlib_ImageGetChannels(img1);

/* number of elements in the row */
	mlib_s32 size_row = mlib_ImageGetWidth(img1) << (nchan >> 1);

/* indices */
	mlib_s32 i, j;
	mlib_s32 k;

	if ((m2 != NULL) && (s2 != NULL)) {
#ifdef _LITTLE_ENDIAN

		if (src1_stride == size_row && src1_stride == src2_stride) {
/* This images is not a sub-images and can be treated as a 1-D vectors */
			size_row *= height;
			height = 1;
		}

		for (i = 0; i < 20; i++) {
			res[i] = 0.0;
		}

		for (i = 0; i < height; i++) {
			for (j = 0; j <= (size_row - 4); j += 4) {
				for (k = 0; k < 4; k++) {
					ccInterMul[k] += MUL_S16(psrc1[j + k],
							psrc2[j + k]);
					ccSelfMul1[k] += MUL_S16(psrc1[j + k],
							psrc1[j + k]);
					sum1[k] += psrc1[j + k];
				}
			}

			for (k = 0; j < size_row; j++, k++) {
				ccInterMul[k] += MUL_S16(psrc1[j], psrc2[j]);
				ccSelfMul1[k] += MUL_S16(psrc1[j], psrc1[j]);
				sum1[k] += psrc1[j];
			}
			psrc1 += src1_stride;
			psrc2 += src2_stride;
		}
		for (k = 0; k < 4; k++) {
			res[k] += ccInterMul[k];
			res[k + 4] += ccSelfMul1[k];
			res[k + 12] += sum1[k];
		}

#else /* _LITTLE_ENDIAN */

		mlib_s32 a1, a2, a3, a4;
		mlib_s32 align;
		union
		{
			mlib_d64 db;
			mlib_s32 s[2];
			mlib_u16 u[4];
		} b[4],
		    r1;

		b[0].db = b[1].db = b[2].db = b[3].db = 0.0;

		if (src1_stride == size_row && src1_stride == src2_stride) {
/* This images is not a sub-images and can be treated as a 1-D vectors */
			size_row *= height;
			height = 1;
		}

		for (i = 0; i < 20; i++) {
			res[i] = 0.0;
		}


		for (i = 0; i < height; i++) {
			j = 0;

			for (k = 0; k < 4; k++) {
				ccInterMul[k] = 0.0;
				ccSelfMul1[k] = 0.0;
				sum1[k] = 0.0;
			}

			if (2 & (mlib_addr)psrc1) {
				a1 = psrc1[0];
				a2 = psrc2[0];

				ccInterMul[3] += a1 * (mlib_d64)a2;
				ccSelfMul1[3] += a1 * (mlib_d64)a1;
				sum1[3] += (mlib_d64)a1;
				j = 1;
			}

			align = j;

	/* same alignment */
			if (!(2 & (mlib_addr)(psrc2 + j))) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (; j <= (size_row - 4); j += 4) {
					a1 = ((mlib_s32 *)(psrc1 + j))[0];
					a2 = ((mlib_s32 *)(psrc1 + j))[1];
					a3 = ((mlib_s32 *)(psrc2 + j))[0];
					a4 = ((mlib_s32 *)(psrc2 + j))[1];

					b[0].u[1] = (mlib_u32)a1 >> 16;
					b[0].u[3] = (mlib_u32)a3 >> 16;
					b[1].u[1] = a1;
					b[1].u[3] = a3;
					b[2].u[1] = (mlib_u32)a2 >> 16;
					b[2].u[3] = (mlib_u32)a4 >> 16;
					b[3].u[1] = a2;
					b[3].u[3] = a4;

					for (k = 0; k < 4; k++) {
						r1.db = b[k].db;
						ccInterMul[k] +=
							(mlib_d64)r1.s[0]
							* r1.s[1];
						ccSelfMul1[k] +=
							(mlib_d64)r1.s[0]
							* r1.s[0];
						sum1[k] += (mlib_d64)r1.s[0];
					}
				}
			} else {
				a4 = ((mlib_s32 *)(psrc2 + j - 1))[0];
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (; j <= (size_row - 4); j += 4) {
					b[0].u[3] = a4;
					a1 = ((mlib_s32 *)(psrc1 + j))[0];
					a2 = ((mlib_s32 *)(psrc1 + j))[1];
					a3 = ((mlib_s32 *)(psrc2 + j + 1))[0];
					a4 = ((mlib_s32 *)(psrc2 + j + 1))[1];

					b[0].u[1] = (mlib_u32)a1 >> 16;
					b[1].u[1] = a1;
					b[1].u[3] = (mlib_u32)a3 >> 16;
					b[2].u[1] = (mlib_u32)a2 >> 16;
					b[2].u[3] = a3;
					b[3].u[1] = a2;
					b[3].u[3] = (mlib_u32)a4 >> 16;

					for (k = 0; k < 4; k++) {
						r1.db = b[k].db;
						ccInterMul[k] +=
							(mlib_d64)r1.s[0]
							* r1.s[1];
						ccSelfMul1[k] +=
							(mlib_d64)r1.s[0]
							* r1.s[0];
						sum1[k] += (mlib_d64)r1.s[0];
					}
				}
			}

			if (j < size_row) {
				a1 = ((mlib_s32 *)(psrc1 + j))[0];
				b[0].u[1] = (mlib_u32)a1 >> 16;
				b[0].u[3] = psrc2[j];
				r1.db = b[0].db;

				ccInterMul[0] += (mlib_d64)r1.s[0] * r1.s[1];
				ccSelfMul1[0] += (mlib_d64)r1.s[0] * r1.s[0];
				sum1[0] += (mlib_d64)r1.s[0];

				j++;

				if (j < size_row) {
					b[1].u[1] = a1;
					b[1].u[3] = psrc2[j];
					r1.db = b[1].db;

					ccInterMul[1] += (mlib_d64)r1.s[0]
						* r1.s[1];
					ccSelfMul1[1] += (mlib_d64)r1.s[0]
						* r1.s[0];
					sum1[1] += (mlib_d64)r1.s[0];

					j++;

					if (j < size_row) {
						a1 = psrc1[j];
						a2 = psrc2[j];

						ccInterMul[2] += a1 *
							(mlib_d64)a2;
						ccSelfMul1[2] += a1 *
							(mlib_d64)a1;
						sum1[2] += (mlib_d64)a1;
					}
				}
			}

			for (k = 0; k < 4; k++) {
				if (k < 3) {
					res[k + align] += ccInterMul[k];
					res[k + align + 4] += ccSelfMul1[k];
					res[k + align + 12] += sum1[k];
				} else {
					res[3 & (3 + align)] += ccInterMul[k];
					res[(3 & (3 + align)) + 4] +=
						ccSelfMul1[k];
					res[(3 & (3 + align)) + 12] += sum1[k];
				}
			}

			psrc1 += src1_stride;
			psrc2 += src2_stride;
		}

#endif /* _LITTLE_ENDIAN */
	} else {
	/* m2 == NULL || s2 == NULL */
#ifdef _LITTLE_ENDIAN

		if (src1_stride == size_row && src1_stride == src2_stride) {
/* This images is not a sub-images and can be treated as a 1-D vectors */
			size_row *= height;
			height = 1;
		}

		for (i = 0; i < 20; i++) {
			res[i] = 0.0;
		}

		for (i = 0; i < height; i++) {
			for (j = 0; j <= (size_row - 4); j += 4) {
				for (k = 0; k < 4; k++) {
					ccInterMul[k] += MUL_S16(psrc1[j + k],
							psrc2[j + k]);
					ccSelfMul1[k] += MUL_S16(psrc1[j + k],
							psrc1[j + k]);
					ccSelfMul2[k] += MUL_S16(psrc2[j + k],
							psrc2[j + k]);
					sum1[k] += psrc1[j + k];
					sum2[k] += psrc2[j + k];
				}
			}

			for (k = 0; j < size_row; j++, k++) {
				ccInterMul[k] += MUL_S16(psrc1[j], psrc2[j]);
				ccSelfMul1[k] += MUL_S16(psrc1[j], psrc1[j]);
				ccSelfMul2[k] += MUL_S16(psrc2[j], psrc2[j]);
				sum1[k] += psrc1[j];
				sum2[k] += psrc2[j];
			}
			psrc1 += src1_stride;
			psrc2 += src2_stride;
		}
		for (k = 0; k < 4; k++) {
			res[k] += ccInterMul[k];
			res[k + 4] += ccSelfMul1[k];
			res[k + 8] += ccSelfMul2[k];
			res[k + 12] += sum1[k];
			res[k + 16] += sum2[k];
		}

#else /* _LITTLE_ENDIAN */

		mlib_s32 a1, a2, a3, a4;
		mlib_s32 align;
		union
		{
			mlib_d64 db;
			mlib_s32 s[2];
			mlib_u16 u[4];
		} b[4],
		    r1;

		b[0].db = b[1].db = b[2].db = b[3].db = 0.0;

		if (src1_stride == size_row && src1_stride == src2_stride) {
/* This images is not a sub-images and can be treated as a 1-D vectors */
			size_row *= height;
			height = 1;
		}

		for (i = 0; i < 20; i++) {
			res[i] = 0.0;
		}


		for (i = 0; i < height; i++) {
			j = 0;

			for (k = 0; k < 4; k++) {
				ccInterMul[k] = 0.0;
				ccSelfMul1[k] = 0.0;
				ccSelfMul2[k] = 0.0;
				sum1[k] = 0.0;
				sum2[k] = 0.0;
			}

			if (2 & (mlib_addr)psrc1) {
				a1 = psrc1[0];
				a2 = psrc2[0];

				ccInterMul[3] += a1 * (mlib_d64)a2;
				ccSelfMul1[3] += a1 * (mlib_d64)a1;
				ccSelfMul2[3] += a2 * (mlib_d64)a2;
				sum1[3] += (mlib_d64)a1;
				sum2[3] += (mlib_d64)a2;
				j = 1;
			}

			align = j;

	/* same alignment */
			if (!(2 & (mlib_addr)(psrc2 + j))) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (; j <= (size_row - 4); j += 4) {
					a1 = ((mlib_s32 *)(psrc1 + j))[0];
					a2 = ((mlib_s32 *)(psrc1 + j))[1];
					a3 = ((mlib_s32 *)(psrc2 + j))[0];
					a4 = ((mlib_s32 *)(psrc2 + j))[1];

					b[0].u[1] = (mlib_u32)a1 >> 16;
					b[0].u[3] = (mlib_u32)a3 >> 16;
					b[1].u[1] = a1;
					b[1].u[3] = a3;
					b[2].u[1] = (mlib_u32)a2 >> 16;
					b[2].u[3] = (mlib_u32)a4 >> 16;
					b[3].u[1] = a2;
					b[3].u[3] = a4;

					for (k = 0; k < 4; k++) {
						r1.db = b[k].db;
						ccInterMul[k] +=
							(mlib_d64)r1.s[0]
							* r1.s[1];
						ccSelfMul1[k] +=
							(mlib_d64)r1.s[0]
							* r1.s[0];
						ccSelfMul2[k] +=
							(mlib_d64)r1.s[1]
							* r1.s[1];
						sum1[k] += (mlib_d64)r1.s[0];
						sum2[k] += (mlib_d64)r1.s[1];
					}
				}
			} else {
				a4 = ((mlib_s32 *)(psrc2 + j - 1))[0];
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (; j <= (size_row - 4); j += 4) {
					b[0].u[3] = a4;
					a1 = ((mlib_s32 *)(psrc1 + j))[0];
					a2 = ((mlib_s32 *)(psrc1 + j))[1];
					a3 = ((mlib_s32 *)(psrc2 + j + 1))[0];
					a4 = ((mlib_s32 *)(psrc2 + j + 1))[1];

					b[0].u[1] = (mlib_u32)a1 >> 16;
					b[1].u[1] = a1;
					b[1].u[3] = (mlib_u32)a3 >> 16;
					b[2].u[1] = (mlib_u32)a2 >> 16;
					b[2].u[3] = a3;
					b[3].u[1] = a2;
					b[3].u[3] = (mlib_u32)a4 >> 16;

					for (k = 0; k < 4; k++) {
						r1.db = b[k].db;
						ccInterMul[k] +=
							(mlib_d64)r1.s[0]
							* r1.s[1];
						ccSelfMul1[k] +=
							(mlib_d64)r1.s[0]
							* r1.s[0];
						ccSelfMul2[k] +=
							(mlib_d64)r1.s[1]
							* r1.s[1];
						sum1[k] += (mlib_d64)r1.s[0];
						sum2[k] += (mlib_d64)r1.s[1];
					}
				}
			}

			if (j < size_row) {
				a1 = ((mlib_s32 *)(psrc1 + j))[0];
				b[0].u[1] = (mlib_u32)a1 >> 16;
				b[0].u[3] = psrc2[j];
				r1.db = b[0].db;

				ccInterMul[0] += (mlib_d64)r1.s[0] * r1.s[1];
				ccSelfMul1[0] += (mlib_d64)r1.s[0] * r1.s[0];
				ccSelfMul2[0] += (mlib_d64)r1.s[1] * r1.s[1];
				sum1[0] += (mlib_d64)r1.s[0];
				sum2[0] += (mlib_d64)r1.s[1];

				j++;

				if (j < size_row) {
					b[1].u[1] = a1;
					b[1].u[3] = psrc2[j];
					r1.db = b[1].db;

					ccInterMul[1] += (mlib_d64)r1.s[0] *
						r1.s[1];
					ccSelfMul1[1] += (mlib_d64)r1.s[0] *
						r1.s[0];
					ccSelfMul2[1] += (mlib_d64)r1.s[1] *
						r1.s[1];
					sum1[1] += (mlib_d64)r1.s[0];
					sum2[1] += (mlib_d64)r1.s[1];

					j++;

					if (j < size_row) {
						a1 = psrc1[j];
						a2 = psrc2[j];

						ccInterMul[2] += a1 *
							(mlib_d64)a2;
						ccSelfMul1[2] += a1 *
							(mlib_d64)a1;
						ccSelfMul2[2] += a2 *
							(mlib_d64)a2;
						sum1[2] += (mlib_d64)a1;
						sum2[2] += (mlib_d64)a2;
					}
				}
			}

			for (k = 0; k < 4; k++) {
				if (k < 3) {
					res[k + align] += ccInterMul[k];
					res[k + align + 4] += ccSelfMul1[k];
					res[k + align + 8] += ccSelfMul2[k];
					res[k + align + 12] += sum1[k];
					res[k + align + 16] += sum2[k];
				} else {
					res[3 & (3 + align)] += ccInterMul[k];
					res[(3 & (3 + align)) + 4] +=
						ccSelfMul1[k];
					res[(3 & (3 + align)) + 8] +=
						ccSelfMul2[k];
					res[(3 & (3 + align)) + 12] += sum1[k];
					res[(3 & (3 + align)) + 16] += sum2[k];
				}
			}

			psrc1 += src1_stride;
			psrc2 += src2_stride;
		}

#endif /* _LITTLE_ENDIAN */
	}
}

/* *********************************************************** */

void
mlib_c_ImageNormCrossCorrel_U16_3(
    const mlib_image *img1,
    const mlib_image *img2,
    const mlib_d64 *m2,
    const mlib_d64 *s2,
    mlib_d64 *res)
{
/* pointer to the data of first image */
	mlib_u16 *psrc1 = (mlib_u16 *)mlib_ImageGetData(img1);

/* pointer to the data of second image */
	mlib_u16 *psrc2 = (mlib_u16 *)mlib_ImageGetData(img2);

/* accumulators by channels */
	mlib_d64 ccInterMul[4] = {0.0, 0.0, 0.0, 0.0};
	mlib_d64 ccSelfMul1[4] = {0.0, 0.0, 0.0, 0.0};
	mlib_d64 ccSelfMul2[4] = {0.0, 0.0, 0.0, 0.0};
	mlib_d64 sum1[4] = {0.0, 0.0, 0.0, 0.0};
	mlib_d64 sum2[4] = {0.0, 0.0, 0.0, 0.0};

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

	for (k = 0; k < 20; k++) {
		res[k] = 0.0;
	}

	if ((m2 != NULL) && (s2 != NULL)) {
#ifdef _LITTLE_ENDIAN

		if (src1_stride == size_row && src1_stride == src2_stride) {
/* This images is not a sub-images and can be treated as a 1-D vectors */
			size_row *= height;
			height = 1;
		}

		for (i = 0; i < height; i++) {
			for (j = 0; j < size_row; j += 3) {
				for (k = 0; k < 3; k++) {
					ccInterMul[k] += MUL_S16(psrc1[j + k],
							psrc2[j + k]);
					ccSelfMul1[k] += MUL_S16(psrc1[j + k],
							psrc1[j + k]);
					sum1[k] += psrc1[j + k];
				}
			}

			psrc1 += src1_stride;
			psrc2 += src2_stride;
		}

		for (k = 0; k < 4; k++) {
			res[k] += ccInterMul[k];
			res[k + 4] += ccSelfMul1[k];
			res[k + 12] += sum1[k];
		}

#else /* _LITTLE_ENDIAN */
		mlib_s32 a1, a2, a3, a4, a5, a6;
		union
		{
			mlib_d64 db;
			mlib_s32 s[2];
			mlib_u16 u[4];
		} b[3],
		    r1;

		b[0].db = b[1].db = b[2].db = 0.0;

		if (src1_stride == size_row && src1_stride == src2_stride) {
/* This images is not a sub-images and can be treated as a 1-D vectors */
			size_row *= height;
			height = 1;
		}


		for (i = 0; i < height; i++) {
			j = 0;

			if (2 & (mlib_addr)psrc1) {
				a1 = psrc1[0];
				a2 = psrc2[0];
				a3 = psrc1[1];
				a4 = psrc2[1];
				a5 = psrc1[2];
				a6 = psrc2[2];

				ccInterMul[0] += a1 * (mlib_d64)a2;
				ccSelfMul1[0] += a1 * (mlib_d64)a1;
				sum1[0] += (mlib_d64)a1;

				ccInterMul[1] += a3 * (mlib_d64)a4;
				ccSelfMul1[1] += a3 * (mlib_d64)a3;
				sum1[1] += (mlib_d64)a3;

				ccInterMul[2] += a5 * (mlib_d64)a6;
				ccSelfMul1[2] += a5 * (mlib_d64)a5;
				sum1[2] += (mlib_d64)a5;

				j = 3;
				if (j >= size_row) {
					psrc1 += src1_stride;
					psrc2 += src2_stride;
					continue;
				}
			}
	/* same alignment */
			if (!(2 & (mlib_addr)(psrc2 + j))) {
				a1 = ((mlib_s32 *)(psrc1 + j))[0];
				a3 = ((mlib_s32 *)(psrc2 + j))[0];
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (; j <= (size_row - 12); j += 6) {
					a2 = ((mlib_s32 *)(psrc1 + j))[1];
					a4 = ((mlib_s32 *)(psrc2 + j))[1];
					a5 = ((mlib_s32 *)(psrc1 + j))[2];
					a6 = ((mlib_s32 *)(psrc2 + j))[2];

					b[0].u[1] = (mlib_u32)a1 >> 16;
					b[0].u[3] = (mlib_u32)a3 >> 16;
					b[1].u[1] = a1;
					b[1].u[3] = a3;
					b[2].u[1] = (mlib_u32)a2 >> 16;
					b[2].u[3] = (mlib_u32)a4 >> 16;

					for (k = 0; k < 3; k++) {
						r1.db = b[k].db;
						ccInterMul[k] +=
							(mlib_d64)r1.s[0]
							* r1.s[1];
						ccSelfMul1[k] +=
							(mlib_d64)r1.s[0]
							* r1.s[0];
						sum1[k] += (mlib_d64)r1.s[0];
					}

					b[0].u[1] = a2;
					b[0].u[3] = a4;
					b[1].u[1] = (mlib_u32)a5 >> 16;
					b[1].u[3] = (mlib_u32)a6 >> 16;
					b[2].u[1] = a5;
					b[2].u[3] = a6;


					for (k = 0; k < 3; k++) {
						r1.db = b[k].db;
						ccInterMul[k] +=
							(mlib_d64)r1.s[0]
							* r1.s[1];
						ccSelfMul1[k] +=
							(mlib_d64)r1.s[0]
							* r1.s[0];
						sum1[k] += (mlib_d64)r1.s[0];
					}

					a1 = ((mlib_s32 *)(psrc1 + j))[3];
					a3 = ((mlib_s32 *)(psrc2 + j))[3];
				}

				if (j <= (size_row - 6)) {
					a2 = ((mlib_s32 *)(psrc1 + j))[1];
					a4 = ((mlib_s32 *)(psrc2 + j))[1];
					a5 = ((mlib_s32 *)(psrc1 + j))[2];
					a6 = ((mlib_s32 *)(psrc2 + j))[2];

					b[0].u[1] = (mlib_u32)a1 >> 16;
					b[0].u[3] = (mlib_u32)a3 >> 16;
					b[1].u[1] = a1;
					b[1].u[3] = a3;
					b[2].u[1] = (mlib_u32)a2 >> 16;
					b[2].u[3] = (mlib_u32)a4 >> 16;

					for (k = 0; k < 3; k++) {
						r1.db = b[k].db;
						ccInterMul[k] +=
							(mlib_d64)r1.s[0]
							* r1.s[1];
						ccSelfMul1[k] +=
							(mlib_d64)r1.s[0]
							* r1.s[0];
						sum1[k] += (mlib_d64)r1.s[0];
					}

					b[0].u[1] = a2;
					b[0].u[3] = a4;
					b[1].u[1] = (mlib_u32)a5 >> 16;
					b[1].u[3] = (mlib_u32)a6 >> 16;
					b[2].u[1] = a5;
					b[2].u[3] = a6;

					for (k = 0; k < 3; k++) {
						r1.db = b[k].db;
						ccInterMul[k] +=
							(mlib_d64)r1.s[0]
							* r1.s[1];
						ccSelfMul1[k] +=
							(mlib_d64)r1.s[0]
							* r1.s[0];
						sum1[k] += (mlib_d64)r1.s[0];
					}

					j += 6;
				}
			} else {
				a6 = ((mlib_s32 *)(psrc2 + j - 1))[0];
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (; j <= (size_row - 6); j += 6) {
					b[0].u[3] = a6;
					a1 = ((mlib_s32 *)(psrc1 + j))[0];
					a2 = ((mlib_s32 *)(psrc1 + j))[1];
					a3 = ((mlib_s32 *)(psrc2 + j + 1))[0];
					a4 = ((mlib_s32 *)(psrc2 + j + 1))[1];
					a5 = ((mlib_s32 *)(psrc1 + j))[2];
					a6 = ((mlib_s32 *)(psrc2 + j + 1))[2];

					b[0].u[1] = (mlib_u32)a1 >> 16;
					b[1].u[1] = a1;
					b[1].u[3] = (mlib_u32)a3 >> 16;
					b[2].u[1] = (mlib_u32)a2 >> 16;
					b[2].u[3] = a3;

					for (k = 0; k < 3; k++) {
						r1.db = b[k].db;
						ccInterMul[k] +=
							(mlib_d64)r1.s[0]
							* r1.s[1];
						ccSelfMul1[k] +=
							(mlib_d64)r1.s[0]
							* r1.s[0];
						sum1[k] += (mlib_d64)r1.s[0];
					}

					b[0].u[1] = a2;
					b[0].u[3] = (mlib_u32)a4 >> 16;
					b[1].u[1] = (mlib_u32)a5 >> 16;
					b[1].u[3] = a4;
					b[2].u[1] = a5;
					b[2].u[3] = (mlib_u32)a6 >> 16;

					for (k = 0; k < 3; k++) {
						r1.db = b[k].db;
						ccInterMul[k] +=
							(mlib_d64)r1.s[0]
							* r1.s[1];
						ccSelfMul1[k] +=
							(mlib_d64)r1.s[0]
							* r1.s[0];
						sum1[k] += (mlib_d64)r1.s[0];
					}
				}
			}

			if (j < size_row) {
				a1 = psrc1[j];
				a2 = psrc2[j];
				a3 = psrc1[j + 1];
				a4 = psrc2[j + 1];
				a5 = psrc1[j + 2];
				a6 = psrc2[j + 2];

				ccInterMul[0] += a1 * (mlib_d64)a2;
				ccSelfMul1[0] += a1 * (mlib_d64)a1;
				sum1[0] += (mlib_d64)a1;

				ccInterMul[1] += a3 * (mlib_d64)a4;
				ccSelfMul1[1] += a3 * (mlib_d64)a3;
				sum1[1] += (mlib_d64)a3;

				ccInterMul[2] += a5 * (mlib_d64)a6;
				ccSelfMul1[2] += a5 * (mlib_d64)a5;
				sum1[2] += (mlib_d64)a5;
			}

			psrc1 += src1_stride;
			psrc2 += src2_stride;
		}

		for (k = 0; k < 4; k++) {
			res[k] += ccInterMul[k];
			res[k + 4] += ccSelfMul1[k];
			res[k + 12] += sum1[k];
		}

#endif /* _LITTLE_ENDIAN */
	} else {
	/* m2 == NULL || s2 == NULL */
#ifdef _LITTLE_ENDIAN

		if (src1_stride == size_row && src1_stride == src2_stride) {
/* This images is not a sub-images and can be treated as a 1-D vectors */
			size_row *= height;
			height = 1;
		}

		for (i = 0; i < height; i++) {
			for (j = 0; j < size_row; j += 3) {
				for (k = 0; k < 3; k++) {
					ccInterMul[k] += MUL_S16(psrc1[j + k],
							psrc2[j + k]);
					ccSelfMul1[k] += MUL_S16(psrc1[j + k],
							psrc1[j + k]);
					ccSelfMul2[k] += MUL_S16(psrc2[j + k],
							psrc2[j + k]);
					sum1[k] += psrc1[j + k];
					sum2[k] += psrc2[j + k];
				}
			}

			psrc1 += src1_stride;
			psrc2 += src2_stride;
		}

		for (k = 0; k < 4; k++) {
			res[k] += ccInterMul[k];
			res[k + 4] += ccSelfMul1[k];
			res[k + 8] += ccSelfMul2[k];
			res[k + 12] += sum1[k];
			res[k + 16] += sum2[k];
		}

#else /* _LITTLE_ENDIAN */
		mlib_s32 a1, a2, a3, a4, a5, a6;
		union
		{
			mlib_d64 db;
			mlib_s32 s[2];
			mlib_u16 u[4];
		} b[3],
		    r1;

		b[0].db = b[1].db = b[2].db = 0.0;

		if (src1_stride == size_row && src1_stride == src2_stride) {
/* This images is not a sub-images and can be treated as a 1-D vectors */
			size_row *= height;
			height = 1;
		}


		for (i = 0; i < height; i++) {
			j = 0;

			if (2 & (mlib_addr)psrc1) {
				a1 = psrc1[0];
				a2 = psrc2[0];
				a3 = psrc1[1];
				a4 = psrc2[1];
				a5 = psrc1[2];
				a6 = psrc2[2];

				ccInterMul[0] += a1 * (mlib_d64)a2;
				ccSelfMul1[0] += a1 * (mlib_d64)a1;
				ccSelfMul2[0] += a2 * (mlib_d64)a2;
				sum1[0] += (mlib_d64)a1;
				sum2[0] += (mlib_d64)a2;

				ccInterMul[1] += a3 * (mlib_d64)a4;
				ccSelfMul1[1] += a3 * (mlib_d64)a3;
				ccSelfMul2[1] += a4 * (mlib_d64)a4;
				sum1[1] += (mlib_d64)a3;
				sum2[1] += (mlib_d64)a4;

				ccInterMul[2] += a5 * (mlib_d64)a6;
				ccSelfMul1[2] += a5 * (mlib_d64)a5;
				ccSelfMul2[2] += a6 * (mlib_d64)a6;
				sum1[2] += (mlib_d64)a5;
				sum2[2] += (mlib_d64)a6;

				j = 3;
				if (j >= size_row) {
					psrc1 += src1_stride;
					psrc2 += src2_stride;
					continue;
				}
			}
	/* same alignment */
			if (!(2 & (mlib_addr)(psrc2 + j))) {
				a1 = ((mlib_s32 *)(psrc1 + j))[0];
				a3 = ((mlib_s32 *)(psrc2 + j))[0];
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (; j <= (size_row - 12); j += 6) {
					a2 = ((mlib_s32 *)(psrc1 + j))[1];
					a4 = ((mlib_s32 *)(psrc2 + j))[1];
					a5 = ((mlib_s32 *)(psrc1 + j))[2];
					a6 = ((mlib_s32 *)(psrc2 + j))[2];

					b[0].u[1] = (mlib_u32)a1 >> 16;
					b[0].u[3] = (mlib_u32)a3 >> 16;
					b[1].u[1] = a1;
					b[1].u[3] = a3;
					b[2].u[1] = (mlib_u32)a2 >> 16;
					b[2].u[3] = (mlib_u32)a4 >> 16;

					for (k = 0; k < 3; k++) {
						r1.db = b[k].db;
						ccInterMul[k] +=
							(mlib_d64)r1.s[0]
							* r1.s[1];
						ccSelfMul1[k] +=
							(mlib_d64)r1.s[0]
							* r1.s[0];
						ccSelfMul2[k] +=
							(mlib_d64)r1.s[1]
							* r1.s[1];
						sum1[k] += (mlib_d64)r1.s[0];
						sum2[k] += (mlib_d64)r1.s[1];
					}

					b[0].u[1] = a2;
					b[0].u[3] = a4;
					b[1].u[1] = (mlib_u32)a5 >> 16;
					b[1].u[3] = (mlib_u32)a6 >> 16;
					b[2].u[1] = a5;
					b[2].u[3] = a6;


					for (k = 0; k < 3; k++) {
						r1.db = b[k].db;
						ccInterMul[k] +=
							(mlib_d64)r1.s[0]
							* r1.s[1];
						ccSelfMul1[k] +=
							(mlib_d64)r1.s[0]
							* r1.s[0];
						ccSelfMul2[k] +=
							(mlib_d64)r1.s[1]
							* r1.s[1];
						sum1[k] += (mlib_d64)r1.s[0];
						sum2[k] += (mlib_d64)r1.s[1];
					}

					a1 = ((mlib_s32 *)(psrc1 + j))[3];
					a3 = ((mlib_s32 *)(psrc2 + j))[3];
				}

				if (j <= (size_row - 6)) {
					a2 = ((mlib_s32 *)(psrc1 + j))[1];
					a4 = ((mlib_s32 *)(psrc2 + j))[1];
					a5 = ((mlib_s32 *)(psrc1 + j))[2];
					a6 = ((mlib_s32 *)(psrc2 + j))[2];

					b[0].u[1] = (mlib_u32)a1 >> 16;
					b[0].u[3] = (mlib_u32)a3 >> 16;
					b[1].u[1] = a1;
					b[1].u[3] = a3;
					b[2].u[1] = (mlib_u32)a2 >> 16;
					b[2].u[3] = (mlib_u32)a4 >> 16;

					for (k = 0; k < 3; k++) {
						r1.db = b[k].db;
						ccInterMul[k] +=
							(mlib_d64)r1.s[0]
							* r1.s[1];
						ccSelfMul1[k] +=
							(mlib_d64)r1.s[0]
							* r1.s[0];
						ccSelfMul2[k] +=
							(mlib_d64)r1.s[1]
							* r1.s[1];
						sum1[k] += (mlib_d64)r1.s[0];
						sum2[k] += (mlib_d64)r1.s[1];
					}

					b[0].u[1] = a2;
					b[0].u[3] = a4;
					b[1].u[1] = (mlib_u32)a5 >> 16;
					b[1].u[3] = (mlib_u32)a6 >> 16;
					b[2].u[1] = a5;
					b[2].u[3] = a6;

					for (k = 0; k < 3; k++) {
						r1.db = b[k].db;
						ccInterMul[k] +=
							(mlib_d64)r1.s[0]
							* r1.s[1];
						ccSelfMul1[k] +=
							(mlib_d64)r1.s[0]
							* r1.s[0];
						ccSelfMul2[k] +=
							(mlib_d64)r1.s[1]
							* r1.s[1];
						sum1[k] += (mlib_d64)r1.s[0];
						sum2[k] += (mlib_d64)r1.s[1];
					}

					j += 6;
				}
			} else {
				a6 = ((mlib_s32 *)(psrc2 + j - 1))[0];
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (; j <= (size_row - 6); j += 6) {
					b[0].u[3] = a6;
					a1 = ((mlib_s32 *)(psrc1 + j))[0];
					a2 = ((mlib_s32 *)(psrc1 + j))[1];
					a3 = ((mlib_s32 *)(psrc2 + j + 1))[0];
					a4 = ((mlib_s32 *)(psrc2 + j + 1))[1];
					a5 = ((mlib_s32 *)(psrc1 + j))[2];
					a6 = ((mlib_s32 *)(psrc2 + j + 1))[2];

					b[0].u[1] = (mlib_u32)a1 >> 16;
					b[1].u[1] = a1;
					b[1].u[3] = (mlib_u32)a3 >> 16;
					b[2].u[1] = (mlib_u32)a2 >> 16;
					b[2].u[3] = a3;

					for (k = 0; k < 3; k++) {
						r1.db = b[k].db;
						ccInterMul[k] +=
							(mlib_d64)r1.s[0]
							* r1.s[1];
						ccSelfMul1[k] +=
							(mlib_d64)r1.s[0]
							* r1.s[0];
						ccSelfMul2[k] +=
							(mlib_d64)r1.s[1]
							* r1.s[1];
						sum1[k] += (mlib_d64)r1.s[0];
						sum2[k] += (mlib_d64)r1.s[1];
					}

					b[0].u[1] = a2;
					b[0].u[3] = (mlib_u32)a4 >> 16;
					b[1].u[1] = (mlib_u32)a5 >> 16;
					b[1].u[3] = a4;
					b[2].u[1] = a5;
					b[2].u[3] = (mlib_u32)a6 >> 16;

					for (k = 0; k < 3; k++) {
						r1.db = b[k].db;
						ccInterMul[k] +=
							(mlib_d64)r1.s[0]
							* r1.s[1];
						ccSelfMul1[k] +=
							(mlib_d64)r1.s[0]
							* r1.s[0];
						ccSelfMul2[k] +=
							(mlib_d64)r1.s[1]
							* r1.s[1];
						sum1[k] += (mlib_d64)r1.s[0];
						sum2[k] += (mlib_d64)r1.s[1];
					}
				}
			}

			if (j < size_row) {
				a1 = psrc1[j];
				a2 = psrc2[j];
				a3 = psrc1[j + 1];
				a4 = psrc2[j + 1];
				a5 = psrc1[j + 2];
				a6 = psrc2[j + 2];

				ccInterMul[0] += a1 * (mlib_d64)a2;
				ccSelfMul1[0] += a1 * (mlib_d64)a1;
				ccSelfMul2[0] += a2 * (mlib_d64)a2;
				sum1[0] += (mlib_d64)a1;
				sum2[0] += (mlib_d64)a2;

				ccInterMul[1] += a3 * (mlib_d64)a4;
				ccSelfMul1[1] += a3 * (mlib_d64)a3;
				ccSelfMul2[1] += a4 * (mlib_d64)a4;
				sum1[1] += (mlib_d64)a3;
				sum2[1] += (mlib_d64)a4;

				ccInterMul[2] += a5 * (mlib_d64)a6;
				ccSelfMul1[2] += a5 * (mlib_d64)a5;
				ccSelfMul2[2] += a6 * (mlib_d64)a6;
				sum1[2] += (mlib_d64)a5;
				sum2[2] += (mlib_d64)a6;
			}

			psrc1 += src1_stride;
			psrc2 += src2_stride;
		}

		for (k = 0; k < 4; k++) {
			res[k] += ccInterMul[k];
			res[k + 4] += ccSelfMul1[k];
			res[k + 8] += ccSelfMul2[k];
			res[k + 12] += sum1[k];
			res[k + 16] += sum2[k];
		}

#endif /* _LITTLE_ENDIAN */
	}
}

/* *********************************************************** */

void
mlib_c_ImageNormCrossCorrel_S16_124(
    const mlib_image *img1,
    const mlib_image *img2,
    const mlib_d64 *m2,
    const mlib_d64 *s2,
    mlib_d64 *res)
{
/* pointer to the data of first image */
	mlib_s16 *psrc1 = (mlib_s16 *)mlib_ImageGetData(img1);

/* pointer to the data of second image */
	mlib_s16 *psrc2 = (mlib_s16 *)mlib_ImageGetData(img2);

/* accumulators by channels */
	mlib_d64 ccInterMul[4] = {0.0, 0.0, 0.0, 0.0};
	mlib_d64 ccSelfMul1[4] = {0.0, 0.0, 0.0, 0.0};
	mlib_d64 ccSelfMul2[4] = {0.0, 0.0, 0.0, 0.0};
	mlib_d64 sum1[4] = {0.0, 0.0, 0.0, 0.0};
	mlib_d64 sum2[4] = {0.0, 0.0, 0.0, 0.0};

/* height of image */
	mlib_s32 height = mlib_ImageGetHeight(img1);

/* elements to next row */
	mlib_s32 src1_stride = mlib_ImageGetStride(img1) >> 1;

/* elements to next row */
	mlib_s32 src2_stride = mlib_ImageGetStride(img2) >> 1;
	mlib_s32 nchan = mlib_ImageGetChannels(img1);

/* number of elements in the row */
	mlib_s32 size_row = mlib_ImageGetWidth(img1) << (nchan >> 1);

/* indices */
	mlib_s32 i, j;
	mlib_s32 k;

	if ((m2 != NULL) && (s2 != NULL)) {
#ifdef _LITTLE_ENDIAN

		if (src1_stride == size_row && src1_stride == src2_stride) {
/* This images is not a sub-images and can be treated as a 1-D vectors */
			size_row *= height;
			height = 1;
		}

		res[0] = res[1] = res[2] = 0;
		for (i = 0; i < height; i++) {
			for (j = 0; j <= (size_row - 4); j += 4) {
				for (k = 0; k < 4; k++) {
					ccInterMul[k] += MUL_S16(psrc1[j + k],
							psrc2[j + k]);
					ccSelfMul1[k] += MUL_S16(psrc1[j + k],
							psrc1[j + k]);
					sum1[k] += psrc1[j + k];
				}
			}

			for (k = 0; j < size_row; j++, k++) {
				ccInterMul[k] += MUL_S16(psrc1[j], psrc2[j]);
				ccSelfMul1[k] += MUL_S16(psrc1[j], psrc1[j]);
				sum1[k] += psrc1[j];
			}

			psrc1 += src1_stride;
			psrc2 += src2_stride;
		}

		for (k = 0; k < 4; k++) {
			res[k] += ccInterMul[k];
			res[k + 4] += ccSelfMul1[k];
			res[k + 12] += sum1[k];
		}

#else /* _LITTLE_ENDIAN */
		mlib_s32 a1, a2, a3, a4;
		mlib_s32 align;
		union
		{
			mlib_d64 db;
			mlib_s32 s[2];
			mlib_u16 u[4];
		} b[4],
		    r1;

		b[0].db = b[1].db = b[2].db = b[3].db = 0.0;

		if (src1_stride == size_row && src1_stride == src2_stride) {
/* This images is not a sub-images and can be treated as a 1-D vectors */
			size_row *= height;
			height = 1;
		}

		for (i = 0; i < 20; i++) {
			res[i] = 0.0;
		}


		for (i = 0; i < height; i++) {
			j = 0;

			for (k = 0; k < 4; k++) {
				ccInterMul[k] = 0.0;
				ccSelfMul1[k] = 0.0;
				sum1[k] = 0.0;
			}

			if (2 & (mlib_addr)psrc1) {
				a1 = ((mlib_u16 *)psrc1)[0] << 16;
				a2 = ((mlib_u16 *)psrc2)[0] << 16;
				ccInterMul[3] += a1 * (mlib_d64)a2;
				ccSelfMul1[3] += a1 * (mlib_d64)a1;
				sum1[3] += (mlib_d64)a1;

				j = 1;
			}

			align = j;

	/* same alignment */
			if (!(2 & (mlib_addr)(psrc2 + j))) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (; j <= (size_row - 4); j += 4) {
					a1 = ((mlib_s32 *)(psrc1 + j))[0];
					a2 = ((mlib_s32 *)(psrc1 + j))[1];
					a3 = ((mlib_s32 *)(psrc2 + j))[0];
					a4 = ((mlib_s32 *)(psrc2 + j))[1];

					b[0].u[0] = (mlib_u32)a1 >> 16;
					b[0].u[2] = (mlib_u32)a3 >> 16;
					b[1].u[0] = a1;
					b[1].u[2] = a3;
					b[2].u[0] = (mlib_u32)a2 >> 16;
					b[2].u[2] = (mlib_u32)a4 >> 16;
					b[3].u[0] = a2;
					b[3].u[2] = a4;

					for (k = 0; k < 4; k++) {
						r1.db = b[k].db;
						ccInterMul[k] +=
							(mlib_d64)r1.s[0]
							* r1.s[1];
						ccSelfMul1[k] +=
							(mlib_d64)r1.s[0]
							* r1.s[0];
						sum1[k] += (mlib_d64)r1.s[0];
					}
				}
			} else {
				a4 = ((mlib_s32 *)(psrc2 + j - 1))[0];
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (; j <= (size_row - 4); j += 4) {
					b[0].u[2] = a4;
					a1 = ((mlib_s32 *)(psrc1 + j))[0];
					a2 = ((mlib_s32 *)(psrc1 + j))[1];
					a3 = ((mlib_s32 *)(psrc2 + j + 1))[0];
					a4 = ((mlib_s32 *)(psrc2 + j + 1))[1];

					b[0].u[0] = (mlib_u32)a1 >> 16;
					b[1].u[0] = a1;
					b[1].u[2] = (mlib_u32)a3 >> 16;
					b[2].u[0] = (mlib_u32)a2 >> 16;
					b[2].u[2] = a3;
					b[3].u[0] = a2;
					b[3].u[2] = (mlib_u32)a4 >> 16;

					for (k = 0; k < 4; k++) {
						r1.db = b[k].db;
						ccInterMul[k] +=
							(mlib_d64)r1.s[0]
							* r1.s[1];
						ccSelfMul1[k] +=
							(mlib_d64)r1.s[0]
							* r1.s[0];
						sum1[k] += (mlib_d64)r1.s[0];
					}
				}
			}

			if (j < size_row) {
				a1 = ((mlib_s32 *)(psrc1 + j))[0];
				b[0].u[0] = (mlib_u32)a1 >> 16;
				b[0].u[2] = ((mlib_u16 *)psrc2)[j];
				r1.db = b[0].db;
				ccInterMul[0] += (mlib_d64)r1.s[0] * r1.s[1];
				ccSelfMul1[0] += (mlib_d64)r1.s[0] * r1.s[0];
				sum1[0] += (mlib_d64)r1.s[0];

				j++;

				if (j < size_row) {
					b[1].u[0] = a1;
					b[1].u[2] = ((mlib_u16 *)psrc2)[j];
					r1.db = b[1].db;
					ccInterMul[1] += (mlib_d64)r1.s[0] *
						r1.s[1];
					ccSelfMul1[1] += (mlib_d64)r1.s[0] *
						r1.s[0];
					sum1[1] += (mlib_d64)r1.s[0];

					j++;

					if (j < size_row) {
						a1 = ((mlib_u16 *)psrc1)[j]
							<< 16;
						a2 = ((mlib_u16 *)psrc2)[j]
							<< 16;
						ccInterMul[2] += a1
							* (mlib_d64)a2;
						ccSelfMul1[2] += a1
							* (mlib_d64)a1;
						sum1[2] += (mlib_d64)a1;
					}
				}
			}

			for (k = 0; k < 4; k++) {
				if (k < 3) {
					res[k + align] += ccInterMul[k]
						* (1. / (65536.0 * 65536.0));
					res[k + align + 4] += ccSelfMul1[k]
						* (1. / (65536.0 * 65536.0));
					res[k + align + 12] += sum1[k]
						* (1. / 65536.0);
				} else {
					res[3 & (3 + align)] += ccInterMul[k]
						* (1. / (65536.0 * 65536.0));
					res[(3 & (3 + align)) + 4] +=
						ccSelfMul1[k]
						* (1. / (65536.0 * 65536.0));
					res[(3 & (3 + align)) + 12] += sum1[k]
						* (1. / 65536.0);
				}
			}

			psrc1 += src1_stride;
			psrc2 += src2_stride;
		}

#endif /* _LITTLE_ENDIAN */
	} else {
	/* m2 == NULL || s2 == NULL */
#ifdef _LITTLE_ENDIAN

		if (src1_stride == size_row && src1_stride == src2_stride) {
/* This images is not a sub-images and can be treated as a 1-D vectors */
			size_row *= height;
			height = 1;
		}

		res[0] = res[1] = res[2] = 0;
		for (i = 0; i < height; i++) {
			for (j = 0; j <= (size_row - 4); j += 4) {
				for (k = 0; k < 4; k++) {
					ccInterMul[k] += MUL_S16(psrc1[j + k],
							psrc2[j + k]);
					ccSelfMul1[k] += MUL_S16(psrc1[j + k],
							psrc1[j + k]);
					ccSelfMul2[k] += MUL_S16(psrc2[j + k],
							psrc2[j + k]);
					sum1[k] += psrc1[j + k];
					sum2[k] += psrc2[j + k];
				}
			}

			for (k = 0; j < size_row; j++, k++) {
				ccInterMul[k] += MUL_S16(psrc1[j], psrc2[j]);
				ccSelfMul1[k] += MUL_S16(psrc1[j], psrc1[j]);
				ccSelfMul2[k] += MUL_S16(psrc2[j], psrc2[j]);
				sum1[k] += psrc1[j];
				sum2[k] += psrc2[j];
			}

			psrc1 += src1_stride;
			psrc2 += src2_stride;
		}

		for (k = 0; k < 4; k++) {
			res[k] += ccInterMul[k];
			res[k + 4] += ccSelfMul1[k];
			res[k + 8] += ccSelfMul2[k];
			res[k + 12] += sum1[k];
			res[k + 16] += sum2[k];
		}

#else /* _LITTLE_ENDIAN */
		mlib_s32 a1, a2, a3, a4;
		mlib_s32 align;
		union
		{
			mlib_d64 db;
			mlib_s32 s[2];
			mlib_u16 u[4];
		} b[4],
		    r1;

		b[0].db = b[1].db = b[2].db = b[3].db = 0.0;

		if (src1_stride == size_row && src1_stride == src2_stride) {
/* This images is not a sub-images and can be treated as a 1-D vectors */
			size_row *= height;
			height = 1;
		}

		for (i = 0; i < 20; i++) {
			res[i] = 0.0;
		}


		for (i = 0; i < height; i++) {
			j = 0;

			for (k = 0; k < 4; k++) {
				ccInterMul[k] = 0.0;
				ccSelfMul1[k] = 0.0;
				ccSelfMul2[k] = 0.0;
				sum1[k] = 0.0;
				sum2[k] = 0.0;
			}

			if (2 & (mlib_addr)psrc1) {
				a1 = ((mlib_u16 *)psrc1)[0] << 16;
				a2 = ((mlib_u16 *)psrc2)[0] << 16;
				ccInterMul[3] += a1 * (mlib_d64)a2;
				ccSelfMul1[3] += a1 * (mlib_d64)a1;
				ccSelfMul2[3] += a2 * (mlib_d64)a2;
				sum1[3] += (mlib_d64)a1;
				sum2[3] += (mlib_d64)a2;

				j = 1;
			}

			align = j;

	/* same alignment */
			if (!(2 & (mlib_addr)(psrc2 + j))) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (; j <= (size_row - 4); j += 4) {
					a1 = ((mlib_s32 *)(psrc1 + j))[0];
					a2 = ((mlib_s32 *)(psrc1 + j))[1];
					a3 = ((mlib_s32 *)(psrc2 + j))[0];
					a4 = ((mlib_s32 *)(psrc2 + j))[1];

					b[0].u[0] = (mlib_u32)a1 >> 16;
					b[0].u[2] = (mlib_u32)a3 >> 16;
					b[1].u[0] = a1;
					b[1].u[2] = a3;
					b[2].u[0] = (mlib_u32)a2 >> 16;
					b[2].u[2] = (mlib_u32)a4 >> 16;
					b[3].u[0] = a2;
					b[3].u[2] = a4;

					for (k = 0; k < 4; k++) {
						r1.db = b[k].db;
						ccInterMul[k] +=
							(mlib_d64)r1.s[0]
							* r1.s[1];
						ccSelfMul1[k] +=
							(mlib_d64)r1.s[0]
							* r1.s[0];
						ccSelfMul2[k] +=
							(mlib_d64)r1.s[1]
							* r1.s[1];
						sum1[k] += (mlib_d64)r1.s[0];
						sum2[k] += (mlib_d64)r1.s[1];
					}
				}
			} else {
				a4 = ((mlib_s32 *)(psrc2 + j - 1))[0];
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (; j <= (size_row - 4); j += 4) {
					b[0].u[2] = a4;
					a1 = ((mlib_s32 *)(psrc1 + j))[0];
					a2 = ((mlib_s32 *)(psrc1 + j))[1];
					a3 = ((mlib_s32 *)(psrc2 + j + 1))[0];
					a4 = ((mlib_s32 *)(psrc2 + j + 1))[1];

					b[0].u[0] = (mlib_u32)a1 >> 16;
					b[1].u[0] = a1;
					b[1].u[2] = (mlib_u32)a3 >> 16;
					b[2].u[0] = (mlib_u32)a2 >> 16;
					b[2].u[2] = a3;
					b[3].u[0] = a2;
					b[3].u[2] = (mlib_u32)a4 >> 16;

					for (k = 0; k < 4; k++) {
						r1.db = b[k].db;
						ccInterMul[k] +=
							(mlib_d64)r1.s[0]
							* r1.s[1];
						ccSelfMul1[k] +=
							(mlib_d64)r1.s[0]
							* r1.s[0];
						ccSelfMul2[k] +=
							(mlib_d64)r1.s[1]
							* r1.s[1];
						sum1[k] += (mlib_d64)r1.s[0];
						sum2[k] += (mlib_d64)r1.s[1];
					}
				}
			}

			if (j < size_row) {
				a1 = ((mlib_s32 *)(psrc1 + j))[0];
				b[0].u[0] = (mlib_u32)a1 >> 16;
				b[0].u[2] = ((mlib_u16 *)psrc2)[j];
				r1.db = b[0].db;
				ccInterMul[0] += (mlib_d64)r1.s[0] * r1.s[1];
				ccSelfMul1[0] += (mlib_d64)r1.s[0] * r1.s[0];
				ccSelfMul2[0] += (mlib_d64)r1.s[1] * r1.s[1];
				sum1[0] += (mlib_d64)r1.s[0];
				sum2[0] += (mlib_d64)r1.s[1];

				j++;

				if (j < size_row) {
					b[1].u[0] = a1;
					b[1].u[2] = ((mlib_u16 *)psrc2)[j];
					r1.db = b[1].db;
					ccInterMul[1] += (mlib_d64)r1.s[0]
						* r1.s[1];
					ccSelfMul1[1] += (mlib_d64)r1.s[0]
						* r1.s[0];
					ccSelfMul2[1] += (mlib_d64)r1.s[1]
						* r1.s[1];
					sum1[1] += (mlib_d64)r1.s[0];
					sum2[1] += (mlib_d64)r1.s[1];

					j++;

					if (j < size_row) {
						a1 = ((mlib_u16 *)psrc1)[j]
							<< 16;
						a2 = ((mlib_u16 *)psrc2)[j]
							<< 16;
						ccInterMul[2] += a1
							* (mlib_d64)a2;
						ccSelfMul1[2] += a1
							* (mlib_d64)a1;
						ccSelfMul2[2] += a2
							* (mlib_d64)a2;
						sum1[2] += (mlib_d64)a1;
						sum2[2] += (mlib_d64)a2;
					}
				}
			}

			for (k = 0; k < 4; k++) {
				if (k < 3) {
					res[k + align] += ccInterMul[k]
						* (1. / (65536.0 * 65536.0));
					res[k + align + 4] += ccSelfMul1[k]
						* (1. / (65536.0 * 65536.0));
					res[k + align + 8] += ccSelfMul2[k]
						* (1. / (65536.0 * 65536.0));
					res[k + align + 12] += sum1[k]
						* (1. / 65536.0);
					res[k + align + 16] += sum2[k]
						* (1. / 65536.0);
				} else {
					res[3 & (3 + align)] += ccInterMul[k]
						* (1. / (65536.0 * 65536.0));
					res[(3 & (3 + align)) + 4] +=
						ccSelfMul1[k]
						* (1. / (65536.0 * 65536.0));
					res[(3 & (3 + align)) + 8] +=
						ccSelfMul2[k]
						* (1. / (65536.0 * 65536.0));
					res[(3 & (3 + align)) + 12] += sum1[k]
						* (1. / 65536.0);
					res[(3 & (3 + align)) + 16] += sum2[k]
						* (1. / 65536.0);
				}
			}

			psrc1 += src1_stride;
			psrc2 += src2_stride;
		}

#endif /* _LITTLE_ENDIAN */
	}
}

/* *********************************************************** */

void
mlib_c_ImageNormCrossCorrel_S16_3(
    const mlib_image *img1,
    const mlib_image *img2,
    const mlib_d64 *m2,
    const mlib_d64 *s2,
    mlib_d64 *res)
{
/* pointer to the data of first image */
	mlib_s16 *psrc1 = (mlib_s16 *)mlib_ImageGetData(img1);

/* pointer to the data of second image */
	mlib_s16 *psrc2 = (mlib_s16 *)mlib_ImageGetData(img2);

/* accumulators by channels */

	mlib_d64 ccInterMul[4] = {0.0, 0.0, 0.0, 0.0};
	mlib_d64 ccSelfMul1[4] = {0.0, 0.0, 0.0, 0.0};
	mlib_d64 ccSelfMul2[4] = {0.0, 0.0, 0.0, 0.0};
	mlib_d64 sum1[4] = {0.0, 0.0, 0.0, 0.0};
	mlib_d64 sum2[4] = {0.0, 0.0, 0.0, 0.0};


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

	if ((m2 != NULL) && (s2 != NULL)) {
#ifdef _LITTLE_ENDIAN

		if (src1_stride == size_row && src1_stride == src2_stride) {
/* This images is not a sub-images and can be treated as a 1-D vectors */
			size_row *= height;
			height = 1;
		}

		for (i = 0; i < height; i++) {
			for (j = 0; j < size_row; j += 3) {
				for (k = 0; k < 3; k++) {
					ccInterMul[k] += MUL_S16(psrc1[j + k],
							psrc2[j + k]);
					ccSelfMul1[k] += MUL_S16(psrc1[j + k],
							psrc1[j + k]);
					sum1[k] += psrc1[j + k];
				}
			}

			psrc1 += src1_stride;
			psrc2 += src2_stride;
		}

		for (k = 0; k < 4; k++) {
			res[k] += ccInterMul[k];
			res[k + 4] += ccSelfMul1[k];
			res[k + 12] += sum1[k];
		}


#else /* _LITTLE_ENDIAN */

		mlib_s32 a1, a2, a3, a4, a5, a6;
		union
		{
			mlib_d64 db;
			mlib_s32 s[2];
			mlib_u16 u[4];
		} b[3],
		    r1;

		b[0].db = b[1].db = b[2].db = 0.0;

		if (src1_stride == size_row && src1_stride == src2_stride) {
/* This images is not a sub-images and can be treated as a 1-D vectors */
			size_row *= height;
			height = 1;
		}

		for (i = 0; i < height; i++) {
			j = 0;

			if (2 & (mlib_addr)psrc1) {
				a1 = ((mlib_u16 *)psrc1)[0] << 16;
				a2 = ((mlib_u16 *)psrc2)[0] << 16;
				a3 = ((mlib_u16 *)psrc1)[1] << 16;
				a4 = ((mlib_u16 *)psrc2)[1] << 16;
				a5 = ((mlib_u16 *)psrc1)[2] << 16;
				a6 = ((mlib_u16 *)psrc2)[2] << 16;

				ccInterMul[0] += a1 * (mlib_d64)a2;
				ccSelfMul1[0] += a1 * (mlib_d64)a1;
				sum1[0] += (mlib_d64)a1;

				ccInterMul[1] += a3 * (mlib_d64)a4;
				ccSelfMul1[1] += a3 * (mlib_d64)a3;
				sum1[1] += (mlib_d64)a3;

				ccInterMul[2] += a5 * (mlib_d64)a6;
				ccSelfMul1[2] += a5 * (mlib_d64)a5;
				sum1[2] += (mlib_d64)a5;

				j = 3;
				if (j >= size_row) {
					psrc1 += src1_stride;
					psrc2 += src2_stride;
					continue;
				}
			}

	/* same alignment */
			if (!(2 & (mlib_addr)(psrc2 + j))) {
				a1 = ((mlib_s32 *)(psrc1 + j))[0];
				a3 = ((mlib_s32 *)(psrc2 + j))[0];
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (; j <= (size_row - 12); j += 6) {
					a2 = ((mlib_s32 *)(psrc1 + j))[1];
					a4 = ((mlib_s32 *)(psrc2 + j))[1];
					a5 = ((mlib_s32 *)(psrc1 + j))[2];
					a6 = ((mlib_s32 *)(psrc2 + j))[2];

					b[0].u[0] = (mlib_u32)a1 >> 16;
					b[0].u[2] = (mlib_u32)a3 >> 16;
					b[1].u[0] = a1;
					b[1].u[2] = a3;
					b[2].u[0] = (mlib_u32)a2 >> 16;
					b[2].u[2] = (mlib_u32)a4 >> 16;

					for (k = 0; k < 3; k++) {
						r1.db = b[k].db;
						ccInterMul[k] +=
							(mlib_d64)r1.s[0]
							* r1.s[1];
						ccSelfMul1[k] +=
							(mlib_d64)r1.s[0]
							* r1.s[0];
						sum1[k] += (mlib_d64)r1.s[0];
					}

					b[0].u[0] = a2;
					b[0].u[2] = a4;
					b[1].u[0] = (mlib_u32)a5 >> 16;
					b[1].u[2] = (mlib_u32)a6 >> 16;
					b[2].u[0] = a5;
					b[2].u[2] = a6;

					for (k = 0; k < 3; k++) {
						r1.db = b[k].db;
						ccInterMul[k] +=
							(mlib_d64)r1.s[0]
							* r1.s[1];
						ccSelfMul1[k] +=
							(mlib_d64)r1.s[0]
							* r1.s[0];
						sum1[k] += (mlib_d64)r1.s[0];
					}

					a1 = ((mlib_s32 *)(psrc1 + j))[3];
					a3 = ((mlib_s32 *)(psrc2 + j))[3];
				}

				if (j <= (size_row - 6)) {
					a2 = ((mlib_s32 *)(psrc1 + j))[1];
					a4 = ((mlib_s32 *)(psrc2 + j))[1];
					a5 = ((mlib_s32 *)(psrc1 + j))[2];
					a6 = ((mlib_s32 *)(psrc2 + j))[2];

					b[0].u[0] = (mlib_u32)a1 >> 16;
					b[0].u[2] = (mlib_u32)a3 >> 16;
					b[1].u[0] = a1;
					b[1].u[2] = a3;
					b[2].u[0] = (mlib_u32)a2 >> 16;
					b[2].u[2] = (mlib_u32)a4 >> 16;

					for (k = 0; k < 3; k++) {
						r1.db = b[k].db;
						ccInterMul[k] +=
							(mlib_d64)r1.s[0]
							* r1.s[1];
						ccSelfMul1[k] +=
							(mlib_d64)r1.s[0]
							* r1.s[0];
						sum1[k] += (mlib_d64)r1.s[0];
					}

					b[0].u[0] = a2;
					b[0].u[2] = a4;
					b[1].u[0] = (mlib_u32)a5 >> 16;
					b[1].u[2] = (mlib_u32)a6 >> 16;
					b[2].u[0] = a5;
					b[2].u[2] = a6;

					for (k = 0; k < 3; k++) {
						r1.db = b[k].db;
						ccInterMul[k] +=
							(mlib_d64)r1.s[0]
							* r1.s[1];
						ccSelfMul1[k] +=
							(mlib_d64)r1.s[0]
							* r1.s[0];
						sum1[k] += (mlib_d64)r1.s[0];
					}

					j += 6;
				}
			} else {
				a6 = ((mlib_s32 *)(psrc2 + j - 1))[0];
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (; j <= (size_row - 6); j += 6) {
					b[0].u[2] = a6;
					a1 = ((mlib_s32 *)(psrc1 + j))[0];
					a2 = ((mlib_s32 *)(psrc1 + j))[1];
					a3 = ((mlib_s32 *)(psrc2 + j + 1))[0];
					a4 = ((mlib_s32 *)(psrc2 + j + 1))[1];
					a5 = ((mlib_s32 *)(psrc1 + j))[2];
					a6 = ((mlib_s32 *)(psrc2 + j + 1))[2];

					b[0].u[0] = (mlib_u32)a1 >> 16;
					b[1].u[0] = a1;
					b[1].u[2] = (mlib_u32)a3 >> 16;
					b[2].u[0] = (mlib_u32)a2 >> 16;
					b[2].u[2] = a3;

					for (k = 0; k < 3; k++) {
						r1.db = b[k].db;
						ccInterMul[k] +=
							(mlib_d64)r1.s[0]
							* r1.s[1];
						ccSelfMul1[k] +=
							(mlib_d64)r1.s[0]
							* r1.s[0];
						sum1[k] += (mlib_d64)r1.s[0];
					}

					b[0].u[0] = a2;
					b[0].u[2] = (mlib_u32)a4 >> 16;
					b[1].u[0] = (mlib_u32)a5 >> 16;
					b[1].u[2] = a4;
					b[2].u[0] = a5;
					b[2].u[2] = (mlib_u32)a6 >> 16;

					for (k = 0; k < 3; k++) {
						r1.db = b[k].db;
						ccInterMul[k] +=
							(mlib_d64)r1.s[0]
							* r1.s[1];
						ccSelfMul1[k] +=
							(mlib_d64)r1.s[0]
							* r1.s[0];
						sum1[k] += (mlib_d64)r1.s[0];
					}
				}
			}

			if (j < size_row) {
				a1 = ((mlib_u16 *)psrc1)[j] << 16;
				a2 = ((mlib_u16 *)psrc2)[j] << 16;
				a3 = ((mlib_u16 *)psrc1)[j + 1] << 16;
				a4 = ((mlib_u16 *)psrc2)[j + 1] << 16;
				a5 = ((mlib_u16 *)psrc1)[j + 2] << 16;
				a6 = ((mlib_u16 *)psrc2)[j + 2] << 16;

				ccInterMul[0] += a1 * (mlib_d64)a2;
				ccSelfMul1[0] += a1 * (mlib_d64)a1;
				sum1[0] += (mlib_d64)a1;

				ccInterMul[1] += a3 * (mlib_d64)a4;
				ccSelfMul1[1] += a3 * (mlib_d64)a3;
				sum1[1] += (mlib_d64)a3;

				ccInterMul[2] += a5 * (mlib_d64)a6;
				ccSelfMul1[2] += a5 * (mlib_d64)a5;
				sum1[2] += (mlib_d64)a5;
			}

			psrc1 += src1_stride;
			psrc2 += src2_stride;
		}

		for (k = 0; k < 4; k++) {
			res[k] += ccInterMul[k] * (1. / (65536.0 * 65536.0));
			res[k + 4] += ccSelfMul1[k] *
				(1. / (65536.0 * 65536.0));
			res[k + 12] += sum1[k] * (1. / 65536.0);
		}
#endif /* _LITTLE_ENDIAN */
	} else {
	/* m2 == NULL || s2 == NULL */
#ifdef _LITTLE_ENDIAN

		if (src1_stride == size_row && src1_stride == src2_stride) {
/* This images is not a sub-images and can be treated as a 1-D vectors */
			size_row *= height;
			height = 1;
		}

		for (i = 0; i < height; i++) {
			for (j = 0; j < size_row; j += 3) {
				for (k = 0; k < 3; k++) {
					ccInterMul[k] += MUL_S16(psrc1[j + k],
							psrc2[j + k]);
					ccSelfMul1[k] += MUL_S16(psrc1[j + k],
							psrc1[j + k]);
					ccSelfMul2[k] += MUL_S16(psrc2[j + k],
							psrc2[j + k]);
					sum1[k] += psrc1[j + k];
					sum2[k] += psrc2[j + k];
				}
			}

			psrc1 += src1_stride;
			psrc2 += src2_stride;
		}

		for (k = 0; k < 4; k++) {
			res[k] += ccInterMul[k];
			res[k + 4] += ccSelfMul1[k];
			res[k + 8] += ccSelfMul2[k];
			res[k + 12] += sum1[k];
			res[k + 16] += sum2[k];
		}


#else /* _LITTLE_ENDIAN */

		mlib_s32 a1, a2, a3, a4, a5, a6;
		union
		{
			mlib_d64 db;
			mlib_s32 s[2];
			mlib_u16 u[4];
		} b[3],
		    r1;

		b[0].db = b[1].db = b[2].db = 0.0;

		if (src1_stride == size_row && src1_stride == src2_stride) {
/* This images is not a sub-images and can be treated as a 1-D vectors */
			size_row *= height;
			height = 1;
		}

		for (i = 0; i < height; i++) {
			j = 0;

			if (2 & (mlib_addr)psrc1) {
				a1 = ((mlib_u16 *)psrc1)[0] << 16;
				a2 = ((mlib_u16 *)psrc2)[0] << 16;
				a3 = ((mlib_u16 *)psrc1)[1] << 16;
				a4 = ((mlib_u16 *)psrc2)[1] << 16;
				a5 = ((mlib_u16 *)psrc1)[2] << 16;
				a6 = ((mlib_u16 *)psrc2)[2] << 16;

				ccInterMul[0] += a1 * (mlib_d64)a2;
				ccSelfMul1[0] += a1 * (mlib_d64)a1;
				ccSelfMul2[0] += a2 * (mlib_d64)a2;
				sum1[0] += (mlib_d64)a1;
				sum2[0] += (mlib_d64)a2;

				ccInterMul[1] += a3 * (mlib_d64)a4;
				ccSelfMul1[1] += a3 * (mlib_d64)a3;
				ccSelfMul2[1] += a4 * (mlib_d64)a4;
				sum1[1] += (mlib_d64)a3;
				sum2[1] += (mlib_d64)a4;

				ccInterMul[2] += a5 * (mlib_d64)a6;
				ccSelfMul1[2] += a5 * (mlib_d64)a5;
				ccSelfMul2[2] += a6 * (mlib_d64)a6;
				sum1[2] += (mlib_d64)a5;
				sum2[2] += (mlib_d64)a6;

				j = 3;
				if (j >= size_row) {
					psrc1 += src1_stride;
					psrc2 += src2_stride;
					continue;
				}
			}

	/* same alignment */
			if (!(2 & (mlib_addr)(psrc2 + j))) {
				a1 = ((mlib_s32 *)(psrc1 + j))[0];
				a3 = ((mlib_s32 *)(psrc2 + j))[0];
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (; j <= (size_row - 12); j += 6) {
					a2 = ((mlib_s32 *)(psrc1 + j))[1];
					a4 = ((mlib_s32 *)(psrc2 + j))[1];
					a5 = ((mlib_s32 *)(psrc1 + j))[2];
					a6 = ((mlib_s32 *)(psrc2 + j))[2];

					b[0].u[0] = (mlib_u32)a1 >> 16;
					b[0].u[2] = (mlib_u32)a3 >> 16;
					b[1].u[0] = a1;
					b[1].u[2] = a3;
					b[2].u[0] = (mlib_u32)a2 >> 16;
					b[2].u[2] = (mlib_u32)a4 >> 16;

					for (k = 0; k < 3; k++) {
						r1.db = b[k].db;
						ccInterMul[k] +=
							(mlib_d64)r1.s[0]
							* r1.s[1];
						ccSelfMul1[k] +=
							(mlib_d64)r1.s[0]
							* r1.s[0];
						ccSelfMul2[k] +=
							(mlib_d64)r1.s[1]
							* r1.s[1];
						sum1[k] += (mlib_d64)r1.s[0];
						sum2[k] += (mlib_d64)r1.s[1];
					}

					b[0].u[0] = a2;
					b[0].u[2] = a4;
					b[1].u[0] = (mlib_u32)a5 >> 16;
					b[1].u[2] = (mlib_u32)a6 >> 16;
					b[2].u[0] = a5;
					b[2].u[2] = a6;

					for (k = 0; k < 3; k++) {
						r1.db = b[k].db;
						ccInterMul[k] +=
							(mlib_d64)r1.s[0]
							* r1.s[1];
						ccSelfMul1[k] +=
							(mlib_d64)r1.s[0]
							* r1.s[0];
						ccSelfMul2[k] +=
							(mlib_d64)r1.s[1]
							* r1.s[1];
						sum1[k] += (mlib_d64)r1.s[0];
						sum2[k] += (mlib_d64)r1.s[1];
					}

					a1 = ((mlib_s32 *)(psrc1 + j))[3];
					a3 = ((mlib_s32 *)(psrc2 + j))[3];
				}

				if (j <= (size_row - 6)) {
					a2 = ((mlib_s32 *)(psrc1 + j))[1];
					a4 = ((mlib_s32 *)(psrc2 + j))[1];
					a5 = ((mlib_s32 *)(psrc1 + j))[2];
					a6 = ((mlib_s32 *)(psrc2 + j))[2];

					b[0].u[0] = (mlib_u32)a1 >> 16;
					b[0].u[2] = (mlib_u32)a3 >> 16;
					b[1].u[0] = a1;
					b[1].u[2] = a3;
					b[2].u[0] = (mlib_u32)a2 >> 16;
					b[2].u[2] = (mlib_u32)a4 >> 16;

					for (k = 0; k < 3; k++) {
						r1.db = b[k].db;
						ccInterMul[k] +=
							(mlib_d64)r1.s[0]
							* r1.s[1];
						ccSelfMul1[k] +=
							(mlib_d64)r1.s[0]
							* r1.s[0];
						ccSelfMul2[k] +=
							(mlib_d64)r1.s[1]
							* r1.s[1];
						sum1[k] += (mlib_d64)r1.s[0];
						sum2[k] += (mlib_d64)r1.s[1];
					}

					b[0].u[0] = a2;
					b[0].u[2] = a4;
					b[1].u[0] = (mlib_u32)a5 >> 16;
					b[1].u[2] = (mlib_u32)a6 >> 16;
					b[2].u[0] = a5;
					b[2].u[2] = a6;

					for (k = 0; k < 3; k++) {
						r1.db = b[k].db;
						ccInterMul[k] +=
							(mlib_d64)r1.s[0]
							* r1.s[1];
						ccSelfMul1[k] +=
							(mlib_d64)r1.s[0]
							* r1.s[0];
						ccSelfMul2[k] +=
							(mlib_d64)r1.s[1]
							* r1.s[1];
						sum1[k] += (mlib_d64)r1.s[0];
						sum2[k] += (mlib_d64)r1.s[1];
					}

					j += 6;
				}
			} else {
				a6 = ((mlib_s32 *)(psrc2 + j - 1))[0];
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (; j <= (size_row - 6); j += 6) {
					b[0].u[2] = a6;
					a1 = ((mlib_s32 *)(psrc1 + j))[0];
					a2 = ((mlib_s32 *)(psrc1 + j))[1];
					a3 = ((mlib_s32 *)(psrc2 + j + 1))[0];
					a4 = ((mlib_s32 *)(psrc2 + j + 1))[1];
					a5 = ((mlib_s32 *)(psrc1 + j))[2];
					a6 = ((mlib_s32 *)(psrc2 + j + 1))[2];

					b[0].u[0] = (mlib_u32)a1 >> 16;
					b[1].u[0] = a1;
					b[1].u[2] = (mlib_u32)a3 >> 16;
					b[2].u[0] = (mlib_u32)a2 >> 16;
					b[2].u[2] = a3;

					for (k = 0; k < 3; k++) {
						r1.db = b[k].db;
						ccInterMul[k] +=
							(mlib_d64)r1.s[0]
							* r1.s[1];
						ccSelfMul1[k] +=
							(mlib_d64)r1.s[0]
							* r1.s[0];
						ccSelfMul2[k] +=
							(mlib_d64)r1.s[1]
							* r1.s[1];
						sum1[k] += (mlib_d64)r1.s[0];
						sum2[k] += (mlib_d64)r1.s[1];
					}

					b[0].u[0] = a2;
					b[0].u[2] = (mlib_u32)a4 >> 16;
					b[1].u[0] = (mlib_u32)a5 >> 16;
					b[1].u[2] = a4;
					b[2].u[0] = a5;
					b[2].u[2] = (mlib_u32)a6 >> 16;

					for (k = 0; k < 3; k++) {
						r1.db = b[k].db;
						ccInterMul[k] +=
							(mlib_d64)r1.s[0]
							* r1.s[1];
						ccSelfMul1[k] +=
							(mlib_d64)r1.s[0]
							* r1.s[0];
						ccSelfMul2[k] +=
							(mlib_d64)r1.s[1]
							* r1.s[1];
						sum1[k] += (mlib_d64)r1.s[0];
						sum2[k] += (mlib_d64)r1.s[1];
					}
				}
			}

			if (j < size_row) {
				a1 = ((mlib_u16 *)psrc1)[j] << 16;
				a2 = ((mlib_u16 *)psrc2)[j] << 16;
				a3 = ((mlib_u16 *)psrc1)[j + 1] << 16;
				a4 = ((mlib_u16 *)psrc2)[j + 1] << 16;
				a5 = ((mlib_u16 *)psrc1)[j + 2] << 16;
				a6 = ((mlib_u16 *)psrc2)[j + 2] << 16;

				ccInterMul[0] += a1 * (mlib_d64)a2;
				ccSelfMul1[0] += a1 * (mlib_d64)a1;
				ccSelfMul2[0] += a2 * (mlib_d64)a2;
				sum1[0] += (mlib_d64)a1;
				sum2[0] += (mlib_d64)a2;

				ccInterMul[1] += a3 * (mlib_d64)a4;
				ccSelfMul1[1] += a3 * (mlib_d64)a3;
				ccSelfMul2[1] += a4 * (mlib_d64)a4;
				sum1[1] += (mlib_d64)a3;
				sum2[1] += (mlib_d64)a4;

				ccInterMul[2] += a5 * (mlib_d64)a6;
				ccSelfMul1[2] += a5 * (mlib_d64)a5;
				ccSelfMul2[2] += a6 * (mlib_d64)a6;
				sum1[2] += (mlib_d64)a5;
				sum2[2] += (mlib_d64)a6;
			}

			psrc1 += src1_stride;
			psrc2 += src2_stride;
		}

		for (k = 0; k < 4; k++) {
			res[k] += ccInterMul[k] * (1. / (65536.0 * 65536.0));
			res[k + 4] += ccSelfMul1[k] *
				(1. / (65536.0 * 65536.0));
			res[k + 8] += ccSelfMul2[k] *
				(1. / (65536.0 * 65536.0));
			res[k + 12] += sum1[k] * (1. / 65536.0);
			res[k + 16] += sum2[k] * (1. / 65536.0);
		}
#endif /* _LITTLE_ENDIAN */
	}
}

/* *********************************************************** */

#define	MUL_S32(x, y)	((x) * (mlib_d64) (y))

/* *********************************************************** */

void
mlib_c_ImageNormCrossCorrel_S32_124(
    const mlib_image *img1,
    const mlib_image *img2,
    const mlib_d64 *m2,
    const mlib_d64 *s2,
    mlib_d64 *res)
{
/* pointer to the data of first image */
	mlib_s32 *psrc1 = (mlib_s32 *)mlib_ImageGetData(img1);

/* pointer to the data of second image */
	mlib_s32 *psrc2 = (mlib_s32 *)mlib_ImageGetData(img2);

/* accumulators by channels */
	mlib_d64 ccInterMul[4] = {0.0, 0.0, 0.0, 0.0};
	mlib_d64 ccSelfMul1[4] = {0.0, 0.0, 0.0, 0.0};
	mlib_d64 ccSelfMul2[4] = {0.0, 0.0, 0.0, 0.0};
	mlib_d64 sum1[4] = {0.0, 0.0, 0.0, 0.0};
	mlib_d64 sum2[4] = {0.0, 0.0, 0.0, 0.0};

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

	if (src1_stride == size_row && src1_stride == src2_stride) {
/* This images is not a sub-images and can be treated as a 1-D vectors */
		size_row *= height;
		height = 1;
	}

	for (i = 0; i < 20; i++) {
		res[i] = 0.0;
	}

	if ((m2 != NULL) && (s2 != NULL)) {
		for (i = 0; i < height; i++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j = 0; j <= (size_row - 4); j += 4) {
				for (k = 0; k < 4; k++) {
					ccInterMul[k] += MUL_S32(psrc1[j + k],
							psrc2[j + k]);
					ccSelfMul1[k] += MUL_S32(psrc1[j + k],
							psrc1[j + k]);
					sum1[k] += psrc1[j + k];
				}
			}

			for (k = 0; j < size_row; j++, k++) {
				ccInterMul[k] += MUL_S32(psrc1[j], psrc2[j]);
				ccSelfMul1[k] += MUL_S32(psrc1[j], psrc1[j]);
				sum1[k] += psrc1[j];
			}
			psrc1 += src1_stride;
			psrc2 += src2_stride;
		}

		for (k = 0; k < 4; k++) {
			res[k] += ccInterMul[k];
			res[k + 4] += ccSelfMul1[k];
			res[k + 12] += sum1[k];
		}
	} else {
	/* m2 == NULL || s2 == NULL */
		for (i = 0; i < height; i++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j = 0; j <= (size_row - 4); j += 4) {
				for (k = 0; k < 4; k++) {
					ccInterMul[k] += MUL_S32(psrc1[j + k],
							psrc2[j + k]);
					ccSelfMul1[k] += MUL_S32(psrc1[j + k],
							psrc1[j + k]);
					ccSelfMul2[k] += MUL_S32(psrc2[j + k],
							psrc2[j + k]);
					sum1[k] += psrc1[j + k];
					sum2[k] += psrc2[j + k];
				}
			}

			for (k = 0; j < size_row; j++, k++) {
				ccInterMul[k] += MUL_S32(psrc1[j], psrc2[j]);
				ccSelfMul1[k] += MUL_S32(psrc1[j], psrc1[j]);
				ccSelfMul2[k] += MUL_S32(psrc2[j], psrc2[j]);
				sum1[k] += psrc1[j];
				sum2[k] += psrc2[j];
			}
			psrc1 += src1_stride;
			psrc2 += src2_stride;
		}

		for (k = 0; k < 4; k++) {
			res[k] += ccInterMul[k];
			res[k + 4] += ccSelfMul1[k];
			res[k + 8] += ccSelfMul2[k];
			res[k + 12] += sum1[k];
			res[k + 16] += sum2[k];
		}
	}
}

/* *********************************************************** */

void
mlib_c_ImageNormCrossCorrel_S32_3(
    const mlib_image *img1,
    const mlib_image *img2,
    const mlib_d64 *m2,
    const mlib_d64 *s2,
    mlib_d64 *res)
{
/* pointer to the data of first image */
	mlib_s32 *psrc1 = (mlib_s32 *)mlib_ImageGetData(img1);

/* pointer to the data of second image */
	mlib_s32 *psrc2 = (mlib_s32 *)mlib_ImageGetData(img2);

/* accumulators by channels */
	mlib_d64 ccInterMul[4] = {0.0, 0.0, 0.0, 0.0};
	mlib_d64 ccSelfMul1[4] = {0.0, 0.0, 0.0, 0.0};
	mlib_d64 ccSelfMul2[4] = {0.0, 0.0, 0.0, 0.0};
	mlib_d64 sum1[4] = {0.0, 0.0, 0.0, 0.0};
	mlib_d64 sum2[4] = {0.0, 0.0, 0.0, 0.0};

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

	for (k = 0; k < 20; k++) {
		res[k] = 0.0;
	}

	if (src1_stride == size_row && src1_stride == src2_stride) {
/* This images is not a sub-images and can be treated as a 1-D vectors */
		size_row *= height;
		height = 1;
	}

	if ((m2 != NULL) && (s2 != NULL)) {
		for (i = 0; i < height; i++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j = 0; j < size_row; j += 3) {
				for (k = 0; k < 3; k++) {
					ccInterMul[k] += MUL_S32(psrc1[j + k],
							psrc2[j + k]);
					ccSelfMul1[k] += MUL_S32(psrc1[j + k],
							psrc1[j + k]);
					sum1[k] += psrc1[j + k];
				}
			}

			psrc1 += src1_stride;
			psrc2 += src2_stride;
		}

		for (k = 0; k < 4; k++) {
			res[k] += ccInterMul[k];
			res[k + 4] += ccSelfMul1[k];
			res[k + 12] += sum1[k];
		}
	} else {
	/* m2 == NULL || s2 == NULL */
		for (i = 0; i < height; i++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j = 0; j < size_row; j += 3) {
				for (k = 0; k < 3; k++) {
					ccInterMul[k] += MUL_S32(psrc1[j + k],
							psrc2[j + k]);
					ccSelfMul1[k] += MUL_S32(psrc1[j + k],
							psrc1[j + k]);
					ccSelfMul2[k] += MUL_S32(psrc2[j + k],
							psrc2[j + k]);
					sum1[k] += psrc1[j + k];
					sum2[k] += psrc2[j + k];
				}
			}

			psrc1 += src1_stride;
			psrc2 += src2_stride;
		}

		for (k = 0; k < 4; k++) {
			res[k] += ccInterMul[k];
			res[k + 4] += ccSelfMul1[k];
			res[k + 8] += ccSelfMul2[k];
			res[k + 12] += sum1[k];
			res[k + 16] += sum2[k];
		}
	}
}

/* *********************************************************** */
