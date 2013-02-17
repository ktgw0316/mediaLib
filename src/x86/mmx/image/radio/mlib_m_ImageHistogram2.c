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

#pragma ident	"@(#)mlib_m_ImageHistogram2.c	9.4	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_ImageHistogram2 - image histogram
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageHistogram2(mlib_s32   **histo,
 *                                       const mlib_image *img,
 *                                       const mlib_s32   *numBins,
 *                                       const mlib_s32   *lowValue,
 *                                       const mlib_s32   *highValue,
 *                                       mlib_s32   xStart,
 *                                       mlib_s32   yStart,
 *                                       mlib_s32   xPeriod,
 *                                       mlib_s32   yPeriod);
 *
 * ARGUMENT
 *      histo      The histogram table. histo[c] is for channel c.
 *      img        The pointer to the input image.
 *      numBins    The number of bins for each channel of the image
 *      lowValue   The lowest pixel value checked for each channel.
 *      highValue  The highest pixel value checked for each channel.
 *                 Note when counting the pixel values, this highValue
 *                 is not included.
 *      xStart     The initial X sample coordinate.
 *      yStart     The initial Y sample coordinate.
 *      xPeriod    The X sampling rate, xPeriod >= 1.
 *      yPeriod    The Y sampling rate, yPeriod >= 1.
 *
 * RESTRICTION
 *      The image can have 1, 2, 3 or 4 channels.
 *      It can be of MLIB_BYTE, MLIB_SHORT, MLIB_USHORT or MLIB_INT data type.
 *      The histogram must have the same number of channels as the image has.
 *      Range from lowValue[k] to (highValue[k] - 1) must be valid subrange of
 *      image type range.
 *
 *      Restrictions to fast MMX version for MLIB_SHORT and
 *      MLIB_USHORT data type:
 *        highValue[k] - lowValue[k] <= 2^13;
 *        numBins[k] < 0.5*(highValue[k] - lowValue[k])
 *        for each channel 'k'
 *      Otherwise C version will be called
 *
 * DESCRIPTION
 *      The mlib_ImageHistogram2 function scans an image, counts number of
 *      pixels within a given range for each channel of the image, and
 *      generates a histogram.
 *
 *      One entry of the histogram, or a bin, is used to accumulate the
 *      number of pixels within a certain sub-range. The legal pixel range
 *      and the number of bins may be controlled separately. If binWidth is
 *      defined as (highValue - lowValue)/numBins, bin i will count pixel
 *      values in the range
 *
 *              lowValue + i*binWidth <= x < lowValue + (i + 1)*binWidth.
 *
 *      The set of pixels scanned may furthermore be reduced by specifying
 *      "xPeriod" and "yPeriod" parameters that specify the sampling rate
 *      along each axis.
 *
 *      The set of pixels to be accumulated may be obtained by intersecting
 *      the grid (xStart + i*xPeriod, yStart + j*yPeriod); i, j >= 0 with
 *      the bounding rectangle of the image.
 *
 *      It is the user's responsibility to clear the histogram table before
 *      this function is called.
 *
 *      It is the user's responsibility to make sure that the histogram
 *      table supplied is suitable for the source image and the parameters.
 *      Otherwise, the result of this function is undefined.
 */

#include <mlib_image.h>
#include <mlib_ImageCheck.h>
#include <mlib_mmx_utils.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageHistogram2 = __mlib_ImageHistogram2

#elif defined(__GNUC__)  /* defined(__SUNPRO_C) */

__typeof__(__mlib_ImageHistogram2) mlib_ImageHistogram2
    __attribute__((weak, alias("__mlib_ImageHistogram2")));

#else /* defined(__SUNPRO_C) */

#error "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

/* buffer size in shorts; this value have to <= 1024 and divided by 12 */
#define	SIZE	1020

/* *********************************************************** */

#define	MMX_GSR_VALUE	0

#define	MLIB_SCALE_INDEX(dd, sd, d_scl, d_min)                  \
	td = _mm_sub_pi16(sd, d_min);                           \
	dl0 = _mm_mullo_pi16(td, d_scl);                        \
	dh0 = _mm_mulhi_pi16(td, d_scl);                        \
	dh = _mm_unpacklo_pi16(dl0, dh0);                       \
	dl = _mm_unpackhi_pi16(dl0, dh0);                       \
	dd = mmx_fpackfix_pair(dh, dl)

/* *********************************************************** */

    mlib_status mlib_ImageHistogram2_U8(
    mlib_s32 **histo,
    const mlib_u8 *src,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s32 nchan,
    mlib_s32 stride,
    const mlib_s32 *lowValue,
    const mlib_s32 *highValue,
    const mlib_d64 *one_per_binWidth,
    mlib_s32 xStart,
    mlib_s32 yStart,
    mlib_s32 xPeriod,
    mlib_s32 yPeriod);

    mlib_status mlib_ImageHistogram2_S16(
    mlib_s32 **histo,
    const mlib_s16 *src,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s32 nchan,
    mlib_s32 stride,
    const mlib_s32 *lowValue,
    const mlib_s32 *highValue,
    const mlib_d64 *one_per_binWidth,
    mlib_s32 xStart,
    mlib_s32 yStart,
    mlib_s32 xPeriod,
    mlib_s32 yPeriod);

    mlib_status mlib_ImageHistogram2_U16(
    mlib_s32 **histo,
    const mlib_s16 *src,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s32 nchan,
    mlib_s32 stride,
    const mlib_s32 *lowValue,
    const mlib_s32 *highValue,
    const mlib_d64 *one_per_binWidth,
    mlib_s32 xStart,
    mlib_s32 yStart,
    mlib_s32 xPeriod,
    mlib_s32 yPeriod);

    mlib_status mlib_ImageHistogram2_S32(
    mlib_s32 **histo,
    const mlib_s32 *src,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s32 nchan,
    mlib_s32 stride,
    const mlib_s32 *lowValue,
    const mlib_s32 *highValue,
    const mlib_d64 *one_per_binWidth,
    mlib_s32 xStart,
    mlib_s32 yStart,
    mlib_s32 xPeriod,
    mlib_s32 yPeriod);

/* *********************************************************** */

    mlib_status mlib_m_ImageHistogram2_S16_124(
    void *src,
    mlib_s32 nchan,
    mlib_s32 size_row,
    mlib_s32 height,
    mlib_s32 stride,
    mlib_s32 xStart,
    mlib_s32 yStart,
    mlib_s32 yPeriod,
    mlib_s32 **histogr,
    __m64 * arr_min,
    __m64 * arr_max,
    __m64 * arr_scl);

    mlib_status mlib_m_ImageHistogram2_S16_3(
    void *src,
    mlib_s32 nchan,
    mlib_s32 size_row,
    mlib_s32 height,
    mlib_s32 stride,
    mlib_s32 xStart,
    mlib_s32 yStart,
    mlib_s32 yPeriod,
    mlib_s32 **histogr,
    __m64 * arr_min,
    __m64 * arr_max,
    __m64 * arr_scl);

    mlib_status mlib_m_ImageHistogram2_U16_124(
    void *src,
    mlib_s32 nchan,
    mlib_s32 size_row,
    mlib_s32 height,
    mlib_s32 stride,
    mlib_s32 xStart,
    mlib_s32 yStart,
    mlib_s32 yPeriod,
    mlib_s32 **histogr,
    __m64 * arr_min,
    __m64 * arr_max,
    __m64 * arr_scl);

    mlib_status mlib_m_ImageHistogram2_U16_3(
    void *src,
    mlib_s32 nchan,
    mlib_s32 size_row,
    mlib_s32 height,
    mlib_s32 stride,
    mlib_s32 xStart,
    mlib_s32 yStart,
    mlib_s32 yPeriod,
    mlib_s32 **histogr,
    __m64 * arr_min,
    __m64 * arr_max,
    __m64 * arr_scl);

/* *********************************************************** */

    void mlib_m_ImageHistogram2_S16_2_D1(
    void *sl,
    mlib_s32 size,
    mlib_s32 *histo0,
    mlib_s32 *histo1);

    void mlib_m_ImageHistogram2_S16_4_D1(
    void *sl,
    mlib_s32 size,
    mlib_s32 *histo0,
    mlib_s32 *histo1,
    mlib_s32 *histo2,
    mlib_s32 *histo3);

    void mlib_m_ImageHistogram2_S16_43_D1(
    void *sl,
    mlib_s32 size,
    mlib_s32 *histo0,
    mlib_s32 *histo1,
    mlib_s32 *histo2);

/* *********************************************************** */

mlib_status
__mlib_ImageHistogram2(
    mlib_s32 **histo,
    const mlib_image *img,
    const mlib_s32 *numBins,
    const mlib_s32 *lowValue,
    const mlib_s32 *highValue,
    mlib_s32 xStart,
    mlib_s32 yStart,
    mlib_s32 xPeriod,
    mlib_s32 yPeriod)
{
	mlib_type type;
	void *src;
	mlib_s32 width, height, nchan, stride;
	mlib_d64 one_per_binWidth[4];
	mlib_u16 s_min[8], s_max[8], s_scl[8];
	mlib_s32 *histogr[8];
	__m64 arr_min[4], arr_max[4], arr_scl[4];
	mlib_s32 i, k, size_row;

	MLIB_IMAGE_CHECK(img);

	if (histo == NULL || numBins == NULL ||
	    lowValue == NULL || highValue == NULL) {
		return (MLIB_NULLPOINTER);
	}

	if (xPeriod < 1 || yPeriod < 1)
		return (MLIB_OUTOFRANGE);

	while (xStart < 0)
		xStart += xPeriod;
	while (yStart < 0)
		yStart += yPeriod;

	type = mlib_ImageGetType(img);
	nchan = mlib_ImageGetChannels(img);
	width = mlib_ImageGetWidth(img);
	height = mlib_ImageGetHeight(img);
	stride = mlib_ImageGetStride(img);
	src = mlib_ImageGetData(img);

	for (i = 0; i < nchan; i++) {
		if (highValue[i] <= lowValue[i] || numBins[i] <= 0)
			return (MLIB_OUTOFRANGE);
		one_per_binWidth[i] =
		    numBins[i] / (highValue[i] - (mlib_d64)lowValue[i]);
	}

	if (type == MLIB_BYTE) {
		return mlib_ImageHistogram2_U8(histo, src, width, height, nchan,
		    stride, lowValue, highValue, one_per_binWidth, xStart,
		    yStart, xPeriod, yPeriod);
	} else if (type == MLIB_INT) {
		return mlib_ImageHistogram2_S32(histo, src, width, height,
		    nchan, stride, lowValue, highValue, one_per_binWidth,
		    xStart, yStart, xPeriod, yPeriod);
	} else if (type == MLIB_USHORT) {
		return mlib_ImageHistogram2_U16(histo, src, width, height,
		    nchan, stride, lowValue, highValue, one_per_binWidth,
		    xStart, yStart, xPeriod, yPeriod);
	} else if (type != MLIB_SHORT && type != MLIB_USHORT) {
		return (MLIB_FAILURE);
	}

	if (xPeriod != 1) {
		if (type == MLIB_SHORT) {
			return mlib_ImageHistogram2_S16(histo, src, width,
			    height, nchan, stride, lowValue, highValue,
			    one_per_binWidth, xStart, yStart, xPeriod, yPeriod);
		} else {
			return mlib_ImageHistogram2_U16(histo, src, width,
			    height, nchan, stride, lowValue, highValue,
			    one_per_binWidth, xStart, yStart, xPeriod, yPeriod);
		}
	}

	for (k = 0; k < nchan; k++) {
		if (lowValue[k] < MLIB_S16_MIN || lowValue[k] > MLIB_S16_MAX ||
		    highValue[k] <= MLIB_S16_MIN ||
		    highValue[k] > (MLIB_S16_MAX + 1))
			return (MLIB_OUTOFRANGE);

		if (highValue[k] - lowValue[k] > (1 << 13) ||
		    one_per_binWidth[k] >= 0.5) {
			if (type == MLIB_SHORT) {
				return mlib_ImageHistogram2_S16(histo, src,
				    width, height, nchan, stride, lowValue,
				    highValue, one_per_binWidth, xStart, yStart,
				    xPeriod, yPeriod);
			} else {
				return mlib_ImageHistogram2_U16(histo, src,
				    width, height, nchan, stride, lowValue,
				    highValue, one_per_binWidth, xStart, yStart,
				    xPeriod, yPeriod);
			}
		}
	}

	stride >>= 1;

	for (k = 0; k < nchan; k++) {
		s_min[k] = lowValue[k];
		s_max[k] = highValue[k] - 1;
		s_scl[k] = one_per_binWidth[k] * (1 << 16);
		histogr[k] = histo[k];
	}
	for (k = nchan; k < 8; k++) {
		s_min[k] = s_min[k - nchan];
		s_max[k] = s_max[k - nchan];
		s_scl[k] = s_scl[k - nchan];
		histogr[k] = histogr[k - nchan];
	}
	for (k = 0; k < 4; k++) {
		((mlib_s16 *)(arr_min + k))[0] = s_min[k];
		((mlib_s16 *)(arr_min + k))[1] = s_min[k + 1];
		((mlib_s16 *)(arr_min + k))[2] = s_min[k + 2];
		((mlib_s16 *)(arr_min + k))[3] = s_min[k + 3];
		((mlib_s16 *)(arr_max + k))[0] = s_max[k];
		((mlib_s16 *)(arr_max + k))[1] = s_max[k + 1];
		((mlib_s16 *)(arr_max + k))[2] = s_max[k + 2];
		((mlib_s16 *)(arr_max + k))[3] = s_max[k + 3];
		((mlib_s16 *)(arr_scl + k))[0] = s_scl[k];
		((mlib_s16 *)(arr_scl + k))[1] = s_scl[k + 1];
		((mlib_s16 *)(arr_scl + k))[2] = s_scl[k + 2];
		((mlib_s16 *)(arr_scl + k))[3] = s_scl[k + 3];
	}

	size_row = (width - xStart + xPeriod - 1) / xPeriod;

/* 1-D vector */
	if (yPeriod == 1 && xStart == 0 && stride == nchan * width) {
		size_row *= (height - yStart);
		height = yStart + 1;
	}

	if (type == MLIB_SHORT) {
		if (nchan != 3) {
			return mlib_m_ImageHistogram2_S16_124(src, nchan,
			    size_row, height, stride, xStart, yStart, yPeriod,
			    histogr, arr_min, arr_max, arr_scl);
		} else {
			return mlib_m_ImageHistogram2_S16_3(src, nchan,
			    size_row, height, stride, xStart, yStart, yPeriod,
			    histogr, arr_min, arr_max, arr_scl);
		}
	} else {
		if (nchan != 3) {
			return mlib_m_ImageHistogram2_U16_124(src, nchan,
			    size_row, height, stride, xStart, yStart, yPeriod,
			    histogr, arr_min, arr_max, arr_scl);
		} else {
			return mlib_m_ImageHistogram2_U16_3(src, nchan,
			    size_row, height, stride, xStart, yStart, yPeriod,
			    histogr, arr_min, arr_max, arr_scl);
		}
	}
}

/* *********************************************************** */

mlib_status
mlib_m_ImageHistogram2_S16_124(
    void *src,
    mlib_s32 nchan,
    mlib_s32 size_row,
    mlib_s32 height,
    mlib_s32 stride,
    mlib_s32 xStart,
    mlib_s32 yStart,
    mlib_s32 yPeriod,
    mlib_s32 **histogr,
    __m64 * arr_min,
    __m64 * arr_max,
    __m64 * arr_scl)
{
	__m64 bufd[(SIZE / 4) + 4];
	int i, j, off, size;

	for (j = yStart; j < height; j += yPeriod) {
		mlib_s16 *sl = (mlib_s16 *)src + j * stride + xStart * nchan;
		__m64 d_min, d_max, d_scl;
		int k = 0, n = nchan * size_row;

		while (((mlib_addr)sl & 7) && (k < n)) {
			mlib_s32 ind =
			    ((sl[0] -
			    ((mlib_s16 *)arr_min)[k]) *
			    ((mlib_s16 *)arr_scl)[k]) >> 16;
			if (sl[0] >= ((mlib_s16 *)arr_min)[k] &&
			    sl[0] <= ((mlib_s16 *)arr_max)[k]) {
				histogr[k][ind]++;
			}
			sl++;
			k++;
		}
		n -= k;

		d_min = arr_min[k];
		d_max = arr_max[k];
		d_scl = arr_scl[k];

/* loop on buffer size */
		for (off = 0; off < n; off += size) {
			__m64 *sp = (__m64 *) (sl + off);
			__m64 low_mask, hgh_mask, out_mask = _m_zero, out_sum =
			    _m_zero;

			size = (n - off);
			if (size > SIZE)
				size = SIZE;

/* find histogram indexes */
			for (i = 0; i < (size + 3) / 4; i++) {
				__m64 sd, td, dd, dh, dl, dh0, dl0;

				out_sum = _mm_add_pi16(out_sum, out_mask);

				sd = sp[i];

				low_mask = _mm_cmpgt_pi16(d_min, sd);
				hgh_mask = _mm_cmpgt_pi16(sd, d_max);
				out_mask = _mm_or_si64(low_mask, hgh_mask);

				MLIB_SCALE_INDEX(dd, sd, d_scl, d_min);

				bufd[i + 1] = _mm_andnot_si64(out_mask, dd);
			}

			if ((size & 3) == 0)
				out_sum = _mm_add_pi16(out_sum, out_mask);
			if ((size & 3) >= 1)
				((mlib_s16 *)&out_sum)[0] +=
				    ((mlib_s16 *)&out_mask)[0];
			if ((size & 3) >= 2)
				((mlib_s16 *)&out_sum)[1] +=
				    ((mlib_s16 *)&out_mask)[1];
			if ((size & 3) >= 3)
				((mlib_s16 *)&out_sum)[2] +=
				    ((mlib_s16 *)&out_mask)[2];

/* correct histo[k][0] */
			histogr[k][0] += ((mlib_s16 *)&out_sum)[0];
			histogr[k + 1][0] += ((mlib_s16 *)&out_sum)[1];
			histogr[k + 2][0] += ((mlib_s16 *)&out_sum)[2];
			histogr[k + 3][0] += ((mlib_s16 *)&out_sum)[3];

			if (nchan == 1) {
				mlib_s32 *histo0 = histogr[0];
				mlib_u32 *sp = (mlib_u32 *)(bufd + 1);
				mlib_u32 src12, src1, src2, elem1;
				int i;

				src12 = (*sp++);
				src1 = src12 >> 16;
				src2 = src12 & 0xFFFF;
/* #pragma pipeloop can't be used here */
				for (i = 0; i < size / 2; i++) {
					elem1 = histo0[src1];
					src12 = sp[i];
					histo0[src1] = elem1 + 1;
					elem1 = histo0[src2];
					src1 = src12 >> 16;
					histo0[src2] = elem1 + 1;
					src2 = src12 & 0xFFFF;
				}
				if (size & 1)
					histo0[src2]++;
			} else if (nchan == 2) {
				mlib_m_ImageHistogram2_S16_2_D1(bufd + 1, size,
				    histogr[k], histogr[k + 1]);
			} else if (nchan == 4) {
				mlib_m_ImageHistogram2_S16_4_D1(bufd + 1, size,
				    histogr[k], histogr[k + 1], histogr[k + 2],
				    histogr[k + 3]);
			}
		}
	}

	_mm_empty();

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_m_ImageHistogram2_S16_3(
    void *src,
    mlib_s32 nchan,
    mlib_s32 size_row,
    mlib_s32 height,
    mlib_s32 stride,
    mlib_s32 xStart,
    mlib_s32 yStart,
    mlib_s32 yPeriod,
    mlib_s32 **histogr,
    __m64 * arr_min,
    __m64 * arr_max,
    __m64 * arr_scl)
{
	__m64 bufd[(SIZE / 4) + 4];
	int i, j, off, size;

	for (j = yStart; j < height; j += yPeriod) {
		mlib_s16 *sl = (mlib_s16 *)src + j * stride + xStart * nchan;
		int k = 0, l, n = nchan * size_row;

		while (((mlib_addr)sl & 7) && (k < n)) {
			mlib_s32 ind =
			    ((sl[0] -
			    ((mlib_s16 *)arr_min)[k]) *
			    ((mlib_s16 *)arr_scl)[k]) >> 16;
			if (sl[0] >= ((mlib_s16 *)arr_min)[k] &&
			    sl[0] <= ((mlib_s16 *)arr_max)[k]) {
				histogr[k][ind]++;
			}
			sl++;
			k++;
		}
		n -= k;

/* 'l' is double offset from sl */
		for (l = 0; l < 3; l++) {
			__m64 d_min, d_max, d_scl;

			d_min = arr_min[k];
			d_max = arr_max[k];
			d_scl = arr_scl[k];

/* loop on buffer size */
			for (off = 0; off < n; off += size) {
				__m64 *sp = (__m64 *) (sl + 4 * l + off);
				__m64 low_mask, hgh_mask, out_mask =
				    _m_zero, out_sum = _m_zero;
				int num, pnum;

				size = (n - off);
				if (size > SIZE)
					size = SIZE;

/* find histogram indexes */
				for (i = 0; i < (size + 11) / 12; i++) {
					__m64 sd, td, dd, dh, dl, dh0, dl0;

					out_sum =
					    _mm_add_pi16(out_sum, out_mask);

					sd = sp[3 * i];

					low_mask = _mm_cmpgt_pi16(d_min, sd);
					hgh_mask = _mm_cmpgt_pi16(sd, d_max);
					out_mask =
					    _mm_or_si64(low_mask, hgh_mask);

					MLIB_SCALE_INDEX(dd, sd, d_scl, d_min);

					bufd[i + 1] =
					    _mm_andnot_si64(out_mask, dd);
				}

				num = i - 1;
				pnum = size - 12 * num;
				if (pnum > 4)
					pnum = 4;

				if (pnum >= 1)
					((mlib_s16 *)&out_sum)[0] +=
					    ((mlib_s16 *)&out_mask)[0];
				if (pnum >= 2)
					((mlib_s16 *)&out_sum)[1] +=
					    ((mlib_s16 *)&out_mask)[1];
				if (pnum >= 3)
					((mlib_s16 *)&out_sum)[2] +=
					    ((mlib_s16 *)&out_mask)[2];
				if (pnum >= 4)
					((mlib_s16 *)&out_sum)[3] +=
					    ((mlib_s16 *)&out_mask)[3];

/* correct histo[k][0] */
				histogr[k][0] += ((mlib_s16 *)&out_sum)[0];
				histogr[k + 1][0] += ((mlib_s16 *)&out_sum)[1];
				histogr[k + 2][0] += ((mlib_s16 *)&out_sum)[2];
				histogr[k + 3][0] += ((mlib_s16 *)&out_sum)[3];

				mlib_m_ImageHistogram2_S16_43_D1(bufd + 1,
				    4 * num + pnum, histogr[k], histogr[k + 1],
				    histogr[k + 2]);
			}

/* correct 'k' and 'n' for next offset */
			k++;
			if (k >= 3)
				k -= 3;
			n -= 4;
		}
	}

	_mm_empty();

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

void
mlib_m_ImageHistogram2_S16_2_D1(
    void *sl,
    mlib_s32 size,
    mlib_s32 *histo0,
    mlib_s32 *histo1)
{
	mlib_u32 *sp = sl;
	mlib_u32 pix01, pix0, pix1;
	mlib_s32 elem0, elem1;
	int i;

	pix01 = (*sp++);
	pix0 = pix01 >> 16;
	pix1 = pix01 & 0xFFFF;

	for (i = 0; i < size / 2; i++) {
		elem0 = histo0[pix1] + 1;
		elem1 = histo1[pix0] + 1;
		histo0[pix1] = elem0;
		histo1[pix0] = elem1;
		pix01 = sp[i];
		pix0 = pix01 >> 16;
		pix1 = pix01 & 0xFFFF;
	}
	if (size & 1)
		histo0[pix1]++;
}

/* *********************************************************** */

void
mlib_m_ImageHistogram2_S16_4_D1(
    void *sl,
    mlib_s32 size,
    mlib_s32 *histo0,
    mlib_s32 *histo1,
    mlib_s32 *histo2,
    mlib_s32 *histo3)
{
	mlib_u32 *sp = sl, *send = sp + size / 2;
	mlib_u16 *ep;

	for (; sp <= (send - 2); sp += 2) {
		mlib_u32 pix01, pix23, pix0, pix1, pix2, pix3;

		pix01 = sp[0];
		pix23 = sp[1];

		pix0 = pix01 & 0xFFFF;
		pix1 = pix01 >> 16;
		pix2 = pix23 & 0xFFFF;
		pix3 = pix23 >> 16;

		histo0[pix0]++;
		histo1[pix1]++;
		histo2[pix2]++;
		histo3[pix3]++;
	}

	ep = (mlib_u16 *)sp;
	if ((size & 3))
		histo0[ep[0]]++;
	if ((size & 3) > 1)
		histo1[ep[1]]++;
	if ((size & 3) > 2)
		histo2[ep[2]]++;
}

/* *********************************************************** */

void
mlib_m_ImageHistogram2_S16_43_D1(
    void *sl,
    mlib_s32 size,
    mlib_s32 *histo0,
    mlib_s32 *histo1,
    mlib_s32 *histo2)
{
	mlib_u32 *sp = sl, *send = sp + size / 2;
	mlib_u16 *ep;

	for (; sp <= (send - 2); sp += 2) {
		mlib_u32 pix01, pix23, pix0, pix1, pix2, pix3;

		pix01 = sp[0];
		pix23 = sp[1];

		pix0 = pix01 & 0xFFFF;
		pix1 = pix01 >> 16;
		pix2 = pix23 & 0xFFFF;
		pix3 = pix23 >> 16;

		histo0[pix0]++;
		histo1[pix1]++;
		histo2[pix2]++;
		histo0[pix3]++;
	}

	ep = (mlib_u16 *)sp;
	if ((size & 3))
		histo0[ep[0]]++;
	if ((size & 3) > 1)
		histo1[ep[1]]++;
	if ((size & 3) > 2)
		histo2[ep[2]]++;
}

/* *********************************************************** */

mlib_status
mlib_m_ImageHistogram2_U16_124(
    void *src,
    mlib_s32 nchan,
    mlib_s32 size_row,
    mlib_s32 height,
    mlib_s32 stride,
    mlib_s32 xStart,
    mlib_s32 yStart,
    mlib_s32 yPeriod,
    mlib_s32 **histogr,
    __m64 * arr_min,
    __m64 * arr_max,
    __m64 * arr_scl)
{
	__m64 bufd[(SIZE / 4) + 4];
	__m64 mask8000 = mmx_to_double_dup(0x80008000);
	int i, j, off, size;

	for (j = yStart; j < height; j += yPeriod) {
		mlib_u16 *sl = (mlib_u16 *)src + j * stride + xStart * nchan;
		__m64 d_off, d_min, d_max, d_scl;
		int k = 0, n = nchan * size_row;

		while (((mlib_addr)sl & 7) && (k < n)) {
			mlib_s32 ind =
			    ((sl[0] -
			    ((mlib_u16 *)arr_min)[k]) *
			    ((mlib_u16 *)arr_scl)[k]) >> 16;
			if (sl[0] >= ((mlib_u16 *)arr_min)[k] &&
			    sl[0] <= ((mlib_u16 *)arr_max)[k]) {
				histogr[k][ind]++;
			}
			sl++;
			k++;
		}
		n -= k;

		d_off = arr_min[k];
		d_min = _mm_or_si64(arr_min[k], mask8000);
		d_max = _mm_or_si64(arr_max[k], mask8000);
		d_scl = arr_scl[k];

/* loop on buffer size */
		for (off = 0; off < n; off += size) {
			__m64 *sp = (__m64 *) (sl + off);
			__m64 low_mask, hgh_mask, out_mask = _m_zero, out_sum =
			    _m_zero;

			size = (n - off);
			if (size > SIZE)
				size = SIZE;

/* find histogram indexes */
			for (i = 0; i < (size + 3) / 4; i++) {
				__m64 sd, sdx, td, dd, dh, dl, dh0, dl0;

				out_sum = _mm_add_pi16(out_sum, out_mask);

				sd = sp[i];

				sdx = _mm_or_si64(sd, mask8000);
				low_mask = _mm_cmpgt_pi16(d_min, sdx);
				hgh_mask = _mm_cmpgt_pi16(sdx, d_max);
				out_mask = _mm_or_si64(low_mask, hgh_mask);

				MLIB_SCALE_INDEX(dd, sd, d_scl, d_off);

				bufd[i + 1] = _mm_andnot_si64(out_mask, dd);
			}

			if ((size & 3) == 0)
				out_sum = _mm_add_pi16(out_sum, out_mask);
			if ((size & 3) >= 1)
				((mlib_s16 *)&out_sum)[0] +=
				    ((mlib_s16 *)&out_mask)[0];
			if ((size & 3) >= 2)
				((mlib_s16 *)&out_sum)[1] +=
				    ((mlib_s16 *)&out_mask)[1];
			if ((size & 3) >= 3)
				((mlib_s16 *)&out_sum)[2] +=
				    ((mlib_s16 *)&out_mask)[2];

/* correct histo[k][0] */
			histogr[k][0] += ((mlib_s16 *)&out_sum)[0];
			histogr[k + 1][0] += ((mlib_s16 *)&out_sum)[1];
			histogr[k + 2][0] += ((mlib_s16 *)&out_sum)[2];
			histogr[k + 3][0] += ((mlib_s16 *)&out_sum)[3];

			if (nchan == 1) {
				mlib_s32 *histo0 = histogr[0];
				mlib_u32 *sp = (mlib_u32 *)(bufd + 1);
				mlib_u32 src12, src1, src2, elem1;
				int i;

				src12 = (*sp++);
				src1 = src12 >> 16;
				src2 = src12 & 0xFFFF;
/* #pragma pipeloop can't be used here */
				for (i = 0; i < size / 2; i++) {
					elem1 = histo0[src1];
					src12 = sp[i];
					histo0[src1] = elem1 + 1;
					elem1 = histo0[src2];
					src1 = src12 >> 16;
					histo0[src2] = elem1 + 1;
					src2 = src12 & 0xFFFF;
				}
				if (size & 1)
					histo0[src2]++;
			} else if (nchan == 2) {
				mlib_m_ImageHistogram2_S16_2_D1(bufd + 1, size,
				    histogr[k], histogr[k + 1]);
			} else if (nchan == 4) {
				mlib_m_ImageHistogram2_S16_4_D1(bufd + 1, size,
				    histogr[k], histogr[k + 1], histogr[k + 2],
				    histogr[k + 3]);
			}
		}
	}

	_mm_empty();

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_m_ImageHistogram2_U16_3(
    void *src,
    mlib_s32 nchan,
    mlib_s32 size_row,
    mlib_s32 height,
    mlib_s32 stride,
    mlib_s32 xStart,
    mlib_s32 yStart,
    mlib_s32 yPeriod,
    mlib_s32 **histogr,
    __m64 * arr_min,
    __m64 * arr_max,
    __m64 * arr_scl)
{
	__m64 bufd[(SIZE / 4) + 4];
	__m64 mask8000 = mmx_to_double_dup(0x80008000);
	int i, j, off, size;

	for (j = yStart; j < height; j += yPeriod) {
		mlib_u16 *sl = (mlib_u16 *)src + j * stride + xStart * nchan;
		int k = 0, l, n = nchan * size_row;

		while (((mlib_addr)sl & 7) && (k < n)) {
			mlib_s32 ind =
			    ((sl[0] -
			    ((mlib_u16 *)arr_min)[k]) *
			    ((mlib_u16 *)arr_scl)[k]) >> 16;
			if (sl[0] >= ((mlib_u16 *)arr_min)[k] &&
			    sl[0] <= ((mlib_u16 *)arr_max)[k]) {
				histogr[k][ind]++;
			}
			sl++;
			k++;
		}
		n -= k;

/* 'l' is double offset from sl */
		for (l = 0; l < 3; l++) {
			__m64 d_off, d_min, d_max, d_scl;

			d_off = arr_min[k];
			d_min = _mm_or_si64(arr_min[k], mask8000);
			d_max = _mm_or_si64(arr_max[k], mask8000);
			d_scl = arr_scl[k];

/* loop on buffer size */
			for (off = 0; off < n; off += size) {
				__m64 *sp = (__m64 *) (sl + 4 * l + off);
				__m64 low_mask, hgh_mask, out_mask =
				    _m_zero, out_sum = _m_zero;
				int num, pnum;

				size = (n - off);
				if (size > SIZE)
					size = SIZE;

/* find histogram indexes */
				for (i = 0; i < (size + 11) / 12; i++) {
					__m64 sd, sdx, td, dd, dh, dl, dh0, dl0;

					out_sum =
					    _mm_add_pi16(out_sum, out_mask);

					sd = sp[3 * i];

					sdx = _mm_or_si64(sd, mask8000);
					low_mask = _mm_cmpgt_pi16(d_min, sdx);
					hgh_mask = _mm_cmpgt_pi16(sdx, d_max);
					out_mask =
					    _mm_or_si64(low_mask, hgh_mask);

					MLIB_SCALE_INDEX(dd, sd, d_scl, d_off);

					bufd[i + 1] =
					    _mm_andnot_si64(out_mask, dd);
				}

				num = i - 1;
				pnum = size - 12 * num;
				if (pnum > 4)
					pnum = 4;

				if (pnum >= 1)
					((mlib_s16 *)&out_sum)[0] +=
					    ((mlib_s16 *)&out_mask)[0];
				if (pnum >= 2)
					((mlib_s16 *)&out_sum)[1] +=
					    ((mlib_s16 *)&out_mask)[1];
				if (pnum >= 3)
					((mlib_s16 *)&out_sum)[2] +=
					    ((mlib_s16 *)&out_mask)[2];
				if (pnum >= 4)
					((mlib_s16 *)&out_sum)[3] +=
					    ((mlib_s16 *)&out_mask)[3];

/* correct histo[k][0] */
				histogr[k][0] += ((mlib_s16 *)&out_sum)[0];
				histogr[k + 1][0] += ((mlib_s16 *)&out_sum)[1];
				histogr[k + 2][0] += ((mlib_s16 *)&out_sum)[2];
				histogr[k + 3][0] += ((mlib_s16 *)&out_sum)[3];

				mlib_m_ImageHistogram2_S16_43_D1(bufd + 1,
				    4 * num + pnum, histogr[k], histogr[k + 1],
				    histogr[k + 2]);
			}

/* correct 'k' and 'n' for next offset */
			k++;
			if (k >= 3)
				k -= 3;
			n -= 4;
		}
	}

	_mm_empty();

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
