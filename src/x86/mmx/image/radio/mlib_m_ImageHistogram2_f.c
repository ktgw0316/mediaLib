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

#pragma ident	"@(#)mlib_m_ImageHistogram2_f.c	9.3	07/11/05 SMI"

/*
 * FILENAME: mlib_ImageHistogram2_f.c
 *
 * DESCRIPTION
 *      C version subroutines for mlib_ImageHistogram2
 */

#include <mlib_image.h>

/* *********************************************************** */

/* this value have to >= 1024 */
#define	BUF_SIZE	1024
#define	SIZE	(BUF_SIZE/3)

/* *********************************************************** */

void mlib_ImageHistogram2_U8_124_D1(
    const mlib_u8 *sl,
    mlib_s32 size,
    mlib_s32 offset1,
    mlib_s32 offset2,
    mlib_s32 offset3,
    mlib_s32 offset4,
    mlib_u8 *histo0,
    mlib_u8 *histo1,
    mlib_u8 *histo2,
    mlib_u8 *histo3);

void mlib_ImageHistogram2_U8_124_D1_A2(
    const mlib_u8 *sl,
    mlib_s32 size,
    mlib_s32 offset1,
    mlib_s32 offset2,
    mlib_u8 *histo0,
    mlib_u8 *histo1,
    mlib_u8 *histo2,
    mlib_u8 *histo3);

void mlib_ImageHistogram2_U8_3_D1(
    const mlib_u8 *sl,
    mlib_s32 size,
    mlib_s32 offset3,
    mlib_u8 *histo0,
    mlib_u8 *histo1,
    mlib_u8 *histo2);

void mlib_ImageHistogram2_U8_3_D1_A2(
    const mlib_u8 *sl,
    mlib_s32 size,
    mlib_u8 *histo0,
    mlib_u8 *histo1,
    mlib_u8 *histo2);

void mlib_ImageHistogram2_S16_4_D1(
    const mlib_s32 *sl,
    mlib_s32 *pmask,
    mlib_s32 size,
    mlib_s32 *histo0,
    mlib_s32 *histo1,
    mlib_s32 *histo2,
    mlib_s32 *histo3);

/* *********************************************************** */

mlib_status
mlib_ImageHistogram2_U8(
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
    mlib_s32 yPeriod)
{
	mlib_u8 *mhisto0, *mhisto1, *mhisto2, *mhisto3;
	mlib_s32 *histo0, *histo1, *histo2, *histo3;
	mlib_s32 ind_tbl[256];
	mlib_s32 xsize;
	mlib_d64 scale, sval;
	mlib_s32 i, j, l;
	mlib_s32 buf[BUF_SIZE + 4];

	for (l = 0; l < nchan; l++) {
		if (lowValue[l] < MLIB_U8_MIN || lowValue[l] > MLIB_U8_MAX ||
		    highValue[l] <= MLIB_U8_MIN ||
		    highValue[l] > (MLIB_U8_MAX + 1))
			return (MLIB_OUTOFRANGE);
	}

	for (i = 0; i < 4 * 256; i++)
		buf[i] = 0;

	mhisto0 = (mlib_u8 *)(buf);
	mhisto1 = (mlib_u8 *)(buf + 256);
	mhisto2 = (mlib_u8 *)(buf + 2 * 256);
	mhisto3 = (mlib_u8 *)(buf + 3 * 256);

	xsize = (width - xStart) * nchan;

	for (j = yStart; j < height; j += yPeriod) {
		const mlib_u8 *psrc = src + j * stride + xStart * nchan;
		mlib_s32 align2 = (mlib_addr)psrc & 1;

		switch (nchan) {
		case 1:
			if (xPeriod == 1 && align2 == 0)
				mlib_ImageHistogram2_U8_124_D1_A2(psrc, xsize,
				    1, 2, mhisto0, mhisto1, mhisto2, mhisto3);
			else
				mlib_ImageHistogram2_U8_124_D1(psrc, xsize,
				    xPeriod, 2 * xPeriod, 3 * xPeriod,
				    4 * xPeriod, mhisto0, mhisto1, mhisto2,
				    mhisto3);
			break;

		case 2:
			if (align2 == 0)
				mlib_ImageHistogram2_U8_124_D1_A2(psrc, xsize,
				    xPeriod, 2 * xPeriod,
				    mhisto0, mhisto1, mhisto2, mhisto3);
			else
				mlib_ImageHistogram2_U8_124_D1(psrc, xsize,
				    1, 2 * xPeriod, 2 * xPeriod + 1,
				    4 * xPeriod, mhisto0, mhisto1, mhisto2,
				    mhisto3);
			break;

		case 3:
			if (xPeriod == 1 && align2 == 0)
				mlib_ImageHistogram2_U8_3_D1_A2(psrc, xsize,
				    mhisto0, mhisto1, mhisto2);
			else
				mlib_ImageHistogram2_U8_3_D1(psrc, xsize,
				    3 * xPeriod, mhisto0, mhisto1, mhisto2);
			break;

		case 4:
			if (align2 == 0)
				mlib_ImageHistogram2_U8_124_D1_A2(psrc, xsize,
				    1, 2 * xPeriod,
				    mhisto0, mhisto1, mhisto2, mhisto3);
			else
				mlib_ImageHistogram2_U8_124_D1(psrc, xsize,
				    1, 2, 3, 4 * xPeriod,
				    mhisto0, mhisto1, mhisto2, mhisto3);
			break;

		default:
			return (MLIB_FAILURE);
		}
	}

	histo0 = buf;
	histo1 = buf + 256;
	histo2 = buf + 2 * 256;
	histo3 = buf + 3 * 256;

	switch (nchan) {
		mlib_s32 *phisto;
		mlib_s32 i, k, low_val, high_val;

	case 1:
		phisto = histo[0];
		low_val = lowValue[0];
		high_val = highValue[0];
		scale = one_per_binWidth[0];
		sval = 0;

		for (i = low_val; i < high_val; i++) {
			ind_tbl[i] = (mlib_s32)sval;
			phisto[ind_tbl[i]] +=
			    (histo0[i] + histo1[i] + histo2[i] + histo3[i]);
			sval += scale;
		}
		return (MLIB_SUCCESS);

	case 2:
		for (k = 0; k < 2; k++) {
			phisto = histo[k];
			low_val = lowValue[k];
			high_val = highValue[k];
			scale = one_per_binWidth[k];
			sval = 0;

			for (i = low_val; i < high_val; i++) {
				ind_tbl[i] = (mlib_s32)sval;
				phisto[ind_tbl[i]] += (histo0[i] + histo2[i]);
				sval += scale;
			}
			histo0 += 256;
			histo2 += 256;
		}
		return (MLIB_SUCCESS);

	case 3:
	case 4:
		for (k = 0; k < nchan; k++) {
			phisto = histo[k];
			low_val = lowValue[k];
			high_val = highValue[k];
			scale = one_per_binWidth[k];
			sval = 0;

			for (i = low_val; i < high_val; i++) {
				ind_tbl[i] = (mlib_s32)sval;
				phisto[ind_tbl[i]] += histo0[i];
				sval += scale;
			}
			histo0 += 256;
		}
		return (MLIB_SUCCESS);

	default:
		return (MLIB_FAILURE);
	}
}

/* *********************************************************** */

mlib_status
mlib_ImageHistogram2_S16(
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
    mlib_s32 yPeriod)
{
	mlib_s32 val, min_val, max_val, numBins, range;
	mlib_d64 scale, d_off;
	mlib_s32 size_row = (width - xStart + xPeriod - 1) / xPeriod;
	mlib_s32 i, j, l;

	for (l = 0; l < nchan; l++) {
		if (lowValue[l] < MLIB_S16_MIN || lowValue[l] > MLIB_S16_MAX ||
		    highValue[l] <= MLIB_S16_MIN ||
		    highValue[l] > (MLIB_S16_MAX + 1))
			return (MLIB_OUTOFRANGE);
	}

	stride >>= 1;

	for (j = yStart; j < height; j += yPeriod) {
		const mlib_s16 *psl = src + j * stride + xStart * nchan;

		for (l = 0; l < nchan; l++) {
			mlib_s32 *histogram = histo[l];

			min_val = lowValue[l];
			max_val = highValue[l];
			range = max_val - min_val;
			numBins = range * one_per_binWidth[l] + 0.5;
			scale = one_per_binWidth[l];
			d_off = lowValue[l] * scale;

			for (i = 0; i < size_row; i++) {
				mlib_s32 ss = psl[i * (xPeriod * nchan)];
				mlib_s32 ind;

				if (ss >= min_val && ss < max_val) {
/* ind = (ss - min_val)*numBins/range; */
					ind = ss * scale - d_off;
					histogram[ind]++;
				}
			}

			psl++;
		}
	}
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_ImageHistogram2_U16(
    mlib_s32 **histo,
    const mlib_u16 *src,
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
    mlib_s32 yPeriod)
{
	mlib_s32 val, min_val, max_val, numBins, range;
	mlib_d64 scale, d_off;
	mlib_s32 size_row = (width - xStart + xPeriod - 1) / xPeriod;
	mlib_s32 i, j, l;

	for (l = 0; l < nchan; l++) {
		if (lowValue[l] < MLIB_U16_MIN || lowValue[l] > MLIB_U16_MAX ||
		    highValue[l] <= MLIB_U16_MIN ||
		    highValue[l] > (MLIB_U16_MAX + 1))
			return (MLIB_OUTOFRANGE);
	}

	stride >>= 1;

	for (j = yStart; j < height; j += yPeriod) {
		const mlib_u16 *psl = src + j * stride + xStart * nchan;

		for (l = 0; l < nchan; l++) {
			mlib_s32 *histogram = histo[l];

			min_val = lowValue[l];
			max_val = highValue[l];
			range = max_val - min_val;
			numBins = range * one_per_binWidth[l] + 0.5;
			scale = one_per_binWidth[l];
			d_off = lowValue[l] * scale;

			for (i = 0; i < size_row; i++) {
				mlib_s32 ss = psl[i * (xPeriod * nchan)];
				mlib_s32 ind;

				if (ss >= min_val && ss < max_val) {
/* ind = (ss - min_val)*numBins/range; */
					ind = ss * scale - d_off;
					histogram[ind]++;
				}
			}

			psl++;
		}
	}
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

void
mlib_ImageHistogram2_U8_124_D1(
    const mlib_u8 *sl,
    mlib_s32 size,
    mlib_s32 offset1,
    mlib_s32 offset2,
    mlib_s32 offset3,
    mlib_s32 offset4,
    mlib_u8 *histo0,
    mlib_u8 *histo1,
    mlib_u8 *histo2,
    mlib_u8 *histo3)
{
	const mlib_u8 *sp = sl, *send = sl + size;
	mlib_u32 *ptr0, *ptr1, *ptr2, *ptr3;

	for (; sp < (send - offset3); sp += offset4) {
		ptr0 = (mlib_u32 *)(histo0 + 4 * sp[0]);
		ptr1 = (mlib_u32 *)(histo1 + 4 * sp[offset1]);
		ptr2 = (mlib_u32 *)(histo2 + 4 * sp[offset2]);
		ptr3 = (mlib_u32 *)(histo3 + 4 * sp[offset3]);

		ptr0[0]++;
		ptr1[0]++;
		ptr2[0]++;
		ptr3[0]++;
	}

	if ((sp) < send)
		((mlib_u32 *)(histo0 + 4 * sp[0]))[0]++;
	if ((sp + offset1) < send)
		((mlib_u32 *)(histo1 + 4 * sp[offset1]))[0]++;
	if ((sp + offset2) < send)
		((mlib_u32 *)(histo2 + 4 * sp[offset2]))[0]++;
}

/* *********************************************************** */

void
mlib_ImageHistogram2_U8_124_D1_A2(
    const mlib_u8 *sl,
    mlib_s32 size,
    mlib_s32 offset1,
    mlib_s32 offset2,
    mlib_u8 *histo0,
    mlib_u8 *histo1,
    mlib_u8 *histo2,
    mlib_u8 *histo3)
{
	const mlib_u8 *sp, *send = sl + size;
	mlib_u16 *sp2 = (mlib_u16 *)sl;
	mlib_u32 pix01, pix23;
	mlib_u32 *ptr0, *ptr1, *ptr2, *ptr3;

	for (; (mlib_u8 *)sp2 <= (send - 2 * offset1 - 2); sp2 += offset2) {
		pix01 = sp2[0];
		pix23 = sp2[offset1];

		ptr0 = (mlib_u32 *)(histo1 + ((pix01 & 0xFF00) >> 6));
		ptr1 = (mlib_u32 *)(histo0 + ((pix01 & 0x00FF) << 2));
		ptr2 = (mlib_u32 *)(histo3 + ((pix23 & 0xFF00) >> 6));
		ptr3 = (mlib_u32 *)(histo2 + ((pix23 & 0x00FF) << 2));

		ptr0[0]++;
		ptr1[0]++;
		ptr2[0]++;
		ptr3[0]++;
	}

	sp = (mlib_u8 *)sp2;
	if (sp < send)
		((mlib_u32 *)(histo0 + 4 * sp[0]))[0]++;
	sp += 1;
	if (sp < send)
		((mlib_u32 *)(histo1 + 4 * sp[0]))[0]++;
	sp += (2 * offset1 - 1);
	if (sp < send)
		((mlib_u32 *)(histo2 + 4 * sp[0]))[0]++;
}

/* *********************************************************** */

void
mlib_ImageHistogram2_U8_3_D1(
    const mlib_u8 *sl,
    mlib_s32 size,
    mlib_s32 offset3,
    mlib_u8 *histo0,
    mlib_u8 *histo1,
    mlib_u8 *histo2)
{
	const mlib_u8 *sp = sl, *send = sl + size;
	mlib_u32 *ptr0, *ptr1, *ptr2;

	for (; sp <= (send - 3); sp += offset3) {
		ptr0 = (mlib_u32 *)(histo0 + 4 * sp[0]);
		ptr1 = (mlib_u32 *)(histo1 + 4 * sp[1]);
		ptr2 = (mlib_u32 *)(histo2 + 4 * sp[2]);

		ptr0[0]++;
		ptr1[0]++;
		ptr2[0]++;
	}
}

/* *********************************************************** */

void
mlib_ImageHistogram2_U8_3_D1_A2(
    const mlib_u8 *sl,
    mlib_s32 size,
    mlib_u8 *histo0,
    mlib_u8 *histo1,
    mlib_u8 *histo2)
{
	const mlib_u8 *sp, *send = sl + size;
	const mlib_u16 *sp2 = (mlib_u16 *)sl;
	mlib_u32 pix01, pix23, pix45;
	mlib_u32 *ptr0, *ptr1, *ptr2, *ptr3, *ptr4, *ptr5;

	for (; (mlib_u8 *)sp2 <= (send - 6); sp2 += 3) {
		pix01 = sp2[0];
		pix23 = sp2[1];
		pix45 = sp2[2];

		ptr0 = (mlib_u32 *)(histo1 + ((pix01 & 0xFF00) >> 6));
		ptr1 = (mlib_u32 *)(histo0 + ((pix01 & 0x00FF) << 2));
		ptr2 = (mlib_u32 *)(histo0 + ((pix23 & 0xFF00) >> 6));
		ptr3 = (mlib_u32 *)(histo2 + ((pix23 & 0x00FF) << 2));
		ptr4 = (mlib_u32 *)(histo2 + ((pix45 & 0xFF00) >> 6));
		ptr5 = (mlib_u32 *)(histo1 + ((pix45 & 0x00FF) << 2));

		ptr0[0]++;
		ptr1[0]++;
		ptr2[0]++;
		ptr3[0]++;
		ptr4[0]++;
		ptr5[0]++;
	}

	sp = (mlib_u8 *)sp2;
	if (sp <= (send - 3)) {
		((mlib_u32 *)(histo0 + 4 * sp[0]))[0]++;
		((mlib_u32 *)(histo1 + 4 * sp[1]))[0]++;
		((mlib_u32 *)(histo2 + 4 * sp[2]))[0]++;
	}
}

/* *********************************************************** */

mlib_status
mlib_ImageHistogram2_S32(
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
    mlib_s32 yPeriod)
{
	mlib_d64 scale, d_off;
	mlib_s32 *buf1, *buf2;
	mlib_s32 val, signs, min_val, max_val, mask, num_out;
	mlib_s32 size, off, src1, src2, src3, elem1;
	mlib_s32 size_row = (width - xStart + xPeriod - 1) / xPeriod;
	mlib_s32 i, j, l;
	mlib_s32 buf[BUF_SIZE + 4];

	stride >>= 2;

	buf1 = buf;
	buf2 = buf1 + SIZE;

	for (j = yStart; j < height; j += yPeriod) {
		const mlib_s32 *psl = src + j * stride + xStart * nchan;

		for (l = 0; l < nchan; l++) {
			mlib_s32 *histogram = histo[l];

			min_val = lowValue[l];
			max_val = highValue[l] - 1;
			scale = one_per_binWidth[l];
			d_off = lowValue[l] * scale;

			for (off = 0; off < size_row; off += size) {
				const mlib_s32 *psrc =
				    psl + l + off * (xPeriod * nchan);

				size = (size_row - off);
				if (size > SIZE)
					size = SIZE;

				num_out = 0;
				signs = 0;
				if (min_val < 0)
					signs += 2;
				if (max_val < 0)
					signs += 1;
				switch (signs) {
				case 0:
					for (i = 0; i < size; i++) {
						buf1[i] =
						    (mlib_s32)(psrc[0] * scale -
						    d_off);
						val = psrc[0];
						mask =
						    (((val -
						    min_val) | val) | ((max_val
						    - val) & ~val)) >> 31;
/* counter of pixels out of range */
						num_out += mask;
						buf2[i] = mask;
						psrc += (xPeriod * nchan);
					}
					break;
				case 1:
					for (i = 0; i < size; i++) {
						buf1[i] =
						    (mlib_s32)(psrc[0] * scale -
						    d_off);
						val = psrc[0];
						mask =
						    (((val -
						    min_val) | val) | ((max_val
						    - val) | ~val)) >> 31;
/* counter of pixels out of range */
						num_out += mask;
						buf2[i] = mask;
						psrc += (xPeriod * nchan);
					}
					break;
				case 2:
					for (i = 0; i < size; i++) {
						buf1[i] =
						    (mlib_s32)(psrc[0] * scale -
						    d_off);
						val = psrc[0];
						mask =
						    (((val -
						    min_val) & val) | ((max_val
						    - val) & ~val)) >> 31;
/* counter of pixels out of range */
						num_out += mask;
						buf2[i] = mask;
						psrc += (xPeriod * nchan);
					}
					break;
				case 3:
					for (i = 0; i < size; i++) {
						buf1[i] =
						    (mlib_s32)(psrc[0] * scale -
						    d_off);
						val = psrc[0];
						mask =
						    (((val -
						    min_val) & val) | ((max_val
						    - val) | ~val)) >> 31;
/* counter of pixels out of range */
						num_out += mask;
						buf2[i] = mask;
						psrc += (xPeriod * nchan);
					}
					break;
				}
				histogram[0] += num_out;

				src1 = buf1[0] & ~buf2[0];
/* #pragma pipeloop can't be used here */
				for (i = 0; i <= (size - 2); i += 2) {
					elem1 = histogram[src1];
					src2 = buf1[i + 1] & ~buf2[i + 1];
					histogram[src1] = elem1 + 1;
					elem1 = histogram[src2];
					src3 = buf1[i + 2] & ~buf2[i + 2];
					histogram[src2] = elem1 + 1;
					src1 = src3;
				}
				if (i < size)
					histogram[src1]++;
			}
		}
	}
	return (MLIB_SUCCESS);
}

/* *********************************************************** */
