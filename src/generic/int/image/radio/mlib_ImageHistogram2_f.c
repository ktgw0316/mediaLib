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

#pragma ident	"@(#)mlib_ImageHistogram2_f.c	9.3	07/11/05 SMI"

/*
 * FILENAME: mlib_ImageHistogram2_f.c
 *
 * DESCRIPTION
 *      C version subroutines for mlib_ImageHistogram2
 */

#include <mlib_image.h>
#include <mlib_ImageHistogram.h>

/* *********************************************************** */

static void mlib_ImageHistogram2_U8_124_D1_A2(
    const mlib_u8 *sl,
    mlib_s32 size,
    mlib_s32 offset1,
    mlib_s32 offset2,
    mlib_s32 *histo0,
    mlib_s32 *histo1,
    mlib_s32 *histo2,
    mlib_s32 *histo3);

static void mlib_ImageHistogram2_U8_124_D1(
    const mlib_u8 *sl,
    mlib_s32 size,
    mlib_s32 offset1,
    mlib_s32 offset2,
    mlib_s32 offset3,
    mlib_s32 offset4,
    mlib_s32 *histo0,
    mlib_s32 *histo1,
    mlib_s32 *histo2,
    mlib_s32 *histo3);

static void mlib_ImageHistogram2_U8_3_D1_A2(
    const mlib_u8 *sl,
    mlib_s32 size,
    mlib_s32 *histo0,
    mlib_s32 *histo1,
    mlib_s32 *histo2);

static void mlib_ImageHistogram2_U8_3_D1(
    const mlib_u8 *sl,
    mlib_s32 size,
    mlib_s32 offset3,
    mlib_s32 *histo0,
    mlib_s32 *histo1,
    mlib_s32 *histo2);

static void mlib_ImageHistogram2_S32_4_D1(
    const mlib_s32 *sl,
    const mlib_s32 *pmask,
    mlib_s32 size,
    mlib_s32 *histo0,
    mlib_s32 *histo1,
    mlib_s32 *histo2,
    mlib_s32 *histo3);

/* *********************************************************** */

/* this value have to >= 1024 */
#define	BUF_SIZE	1024
#define	SIZE	(BUF_SIZE/3)

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
	mlib_s32 *mhisto0, *mhisto1, *mhisto2, *mhisto3;
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

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < 4 * 256; i++)
		buf[i] = 0;

	mhisto0 = (mlib_s32 *)(buf);
	mhisto1 = (mlib_s32 *)(buf + 256);
	mhisto2 = (mlib_s32 *)(buf + 2 * 256);
	mhisto3 = (mlib_s32 *)(buf + 3 * 256);

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

		if (scale >= 1) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = low_val; i < high_val; i++) {
				ind_tbl[i] = (mlib_s32)sval;
				phisto[ind_tbl[i]] +=
				    (histo0[i] + histo1[i] + histo2[i] +
				    histo3[i]);
				sval += scale;
			}
		} else {
			for (i = low_val; i < high_val; i++) {
				ind_tbl[i] = (mlib_s32)sval;
				phisto[ind_tbl[i]] +=
				    (histo0[i] + histo1[i] + histo2[i] +
				    histo3[i]);
				sval += scale;
			}
		}

		return (MLIB_SUCCESS);

	case 2:
		for (k = 0; k < 2; k++) {
			phisto = histo[k];
			low_val = lowValue[k];
			high_val = highValue[k];
			scale = one_per_binWidth[k];
			sval = 0;

			if (scale >= 1) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = low_val; i < high_val; i++) {
					ind_tbl[i] = (mlib_s32)sval;
					phisto[ind_tbl[i]] +=
					    (histo0[i] + histo2[i]);
					sval += scale;
				}
			} else {
				for (i = low_val; i < high_val; i++) {
					ind_tbl[i] = (mlib_s32)sval;
					phisto[ind_tbl[i]] +=
					    (histo0[i] + histo2[i]);
					sval += scale;
				}
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

			if (scale >= 1) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = low_val; i < high_val; i++) {
					ind_tbl[i] = (mlib_s32)sval;
					phisto[ind_tbl[i]] += histo0[i];
					sval += scale;
				}
			} else {
				for (i = low_val; i < high_val; i++) {
					ind_tbl[i] = (mlib_s32)sval;
					phisto[ind_tbl[i]] += histo0[i];
					sval += scale;
				}
			}

			histo0 += 256;
		}

		return (MLIB_SUCCESS);

	default:
		return (MLIB_FAILURE);
	}
}

/* *********************************************************** */
#if defined(_NO_LONGLONG)
#define	ITYPE mlib_d64
#else /* else if _NO_LONGLONG */
#define	ITYPE mlib_s64
#endif /* end _NO_LONGLONG */
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
	mlib_s32 *buf1, *buf2, *buf3;
	mlib_s32 numBins, rangeValue;
	mlib_s32 val, min_val, max_val, mask, num_out;
	mlib_s32 size, off, src1, src2, src3, elem1;
	mlib_s32 size_row = (width - xStart + xPeriod - 1) / xPeriod;
	mlib_s32 i, j, l;
	mlib_s32 buf[BUF_SIZE + 4];

	buf1 = buf;
	buf2 = buf1 + SIZE;
	buf3 = buf2 + SIZE;

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
			max_val = highValue[l] - 1;

			rangeValue = (max_val - min_val + 1);
			numBins = (mlib_s32)(one_per_binWidth[l] *
					rangeValue + 0.5);

			for (off = 0; off < size_row; off += size) {
				const mlib_s16 *psrc =
				    psl + l + off * (xPeriod * nchan);

				size = (size_row - off);

				if (size > SIZE)
					size = SIZE;

				num_out = 0;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i < size; i++) {
					val = psrc[i * (xPeriod * nchan)];
					buf1[i] = val - min_val;
					buf2[i] = ((ITYPE)buf1[i] * numBins) /
						rangeValue;

					mask =
					    ((max_val - val) | (val -
					    min_val)) >> 31;
/* counter of pixels out of range */
					num_out += mask;
					buf3[i] = mask;
				}

				histogram[0] += num_out;

				src1 = buf2[0] & ~buf3[0];
/* #pragma pipeloop can't be used here */
				for (i = 0; i <= (size - 2); i += 2) {
					elem1 = histogram[src1];
					src2 = buf2[i + 1] & ~buf3[i + 1];
					histogram[src1] = elem1 + 1;
					elem1 = histogram[src2];
					src3 = buf2[i + 2] & ~buf3[i + 2];
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
	mlib_s32 *buf1, *buf2, *buf3;
	mlib_s32 numBins, rangeValue;
	mlib_s32 val, min_val, max_val, mask, num_out;
	mlib_s32 size, off, src1, src2, src3, elem1;
	mlib_s32 size_row = (width - xStart + xPeriod - 1) / xPeriod;
	mlib_s32 i, j, l, k;
	mlib_s32 buf[BUF_SIZE + 4];

	buf1 = buf;
	buf2 = buf1 + SIZE;
	buf3 = buf2 + SIZE;

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
			max_val = highValue[l] - 1;

			rangeValue = (max_val - min_val + 1);
			numBins = (mlib_s32)(one_per_binWidth[l] *
					rangeValue + 0.5);

			for (off = 0; off < size_row; off += size) {
				const mlib_u16 *psrc =
				    psl + l + off * (xPeriod * nchan);

				size = (size_row - off);

				if (size > SIZE)
					size = SIZE;

				num_out = 0;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i < size; i++) {
					val = psrc[i * (xPeriod * nchan)];
					buf1[i] = val - min_val;
					buf2[i] = ((ITYPE)buf1[i] * numBins) /
						rangeValue;

					mask =
					    ((max_val - val) | (val -
					    min_val)) >> 31;
/* counter of pixels out of range */
					num_out += mask;
					buf3[i] = mask;
				}

				histogram[0] += num_out;

				src1 = buf2[0] & ~buf3[0];
/* #pragma pipeloop can't be used here */
				for (i = 0; i <= (size - 2); i += 2) {
					elem1 = histogram[src1];
					src2 = buf2[i + 1] & ~buf3[i + 1];
					histogram[src1] = elem1 + 1;
					elem1 = histogram[src2];
					src3 = buf2[i + 2] & ~buf3[i + 2];
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

void
mlib_ImageHistogram2_U8_124_D1(
    const mlib_u8 *sl,
    mlib_s32 size,
    mlib_s32 offset1,
    mlib_s32 offset2,
    mlib_s32 offset3,
    mlib_s32 offset4,
    mlib_s32 *histo0,
    mlib_s32 *histo1,
    mlib_s32 *histo2,
    mlib_s32 *histo3)
{
	const mlib_u8 *sp = sl, *send = sl + size;
	mlib_u32 pix0, pix1, pix2, pix3;

#ifdef __SUNPRO_C
#pragma nomemorydepend
#endif /* __SUNPRO_C */
	for (; sp < (send - offset3); sp += offset4) {
		pix0 = sp[0];
		pix1 = sp[offset1];
		pix2 = sp[offset2];
		pix3 = sp[offset3];

		histo0[pix0]++;
		histo1[pix1]++;
		histo2[pix2]++;
		histo3[pix3]++;
	}

	if ((sp) < send)
		histo0[sp[0]]++;

	if ((sp + offset1) < send)
		histo1[sp[offset1]]++;

	if ((sp + offset2) < send)
		histo3[sp[offset2]]++;
}

/* *********************************************************** */

void
mlib_ImageHistogram2_U8_124_D1_A2(
    const mlib_u8 *sl,
    mlib_s32 size,
    mlib_s32 offset1,
    mlib_s32 offset2,
    mlib_s32 *histo0,
    mlib_s32 *histo1,
    mlib_s32 *histo2,
    mlib_s32 *histo3)
{
	const mlib_u8 *sp, *send = sl + size;
	mlib_u16 *sp2 = (mlib_u16 *)sl;
	mlib_u32 pix01, pix23;
	mlib_u32 pix0, pix1, pix2, pix3;

#ifdef __SUNPRO_C
#pragma nomemorydepend
#endif /* __SUNPRO_C */
	for (; (mlib_u8 *)sp2 <= (send - 2 * offset1 - 2); sp2 += offset2) {
		pix01 = sp2[0];
		pix23 = sp2[offset1];

		pix0 = ((pix01 & 0xFF00) >> 8);
		pix1 = (pix01 & 0x00FF);
		pix2 = ((pix23 & 0xFF00) >> 8);
		pix3 = (pix23 & 0x00FF);

#ifdef _LITTLE_ENDIAN
		histo1[pix0]++;
		histo0[pix1]++;
		histo3[pix2]++;
		histo2[pix3]++;
#else /* _LITTLE_ENDIAN */
		histo0[pix0]++;
		histo1[pix1]++;
		histo2[pix2]++;
		histo3[pix3]++;
#endif /* _LITTLE_ENDIAN */
	}

	sp = (mlib_u8 *)sp2;

	if (sp < send)
		histo0[sp[0]]++;
	sp += 1;

	if (sp < send)
		histo1[sp[0]]++;
	sp += (2 * offset1 - 1);

	if (sp < send)
		histo2[sp[0]]++;
}

/* *********************************************************** */

void
mlib_ImageHistogram2_U8_3_D1(
    const mlib_u8 *sl,
    mlib_s32 size,
    mlib_s32 offset3,
    mlib_s32 *histo0,
    mlib_s32 *histo1,
    mlib_s32 *histo2)
{
	const mlib_u8 *sp = sl;
	mlib_u32 pix0, pix1, pix2;
	mlib_s32 i;

#ifdef __SUNPRO_C
#pragma nomemorydepend
#endif /* __SUNPRO_C */
	for (i = 0; i <= (size - 3); i += offset3) {
		pix0 = sp[i];
		pix1 = sp[i + 1];
		pix2 = sp[i + 2];

		histo0[pix0]++;
		histo1[pix1]++;
		histo2[pix2]++;
	}
}

/* *********************************************************** */

void
mlib_ImageHistogram2_U8_3_D1_A2(
    const mlib_u8 *sl,
    mlib_s32 size,
    mlib_s32 *histo0,
    mlib_s32 *histo1,
    mlib_s32 *histo2)
{
	mlib_u32 pix01, pix23, pix45;
	mlib_u32 pix0, pix1, pix2, pix3, pix4, pix5;
	mlib_s32 i;

	for (i = 0; i <= size - 6; i += 6) {
		pix01 = *(mlib_u16 *)(sl + i);
		pix23 = *(mlib_u16 *)(sl + i + 2);
		pix45 = *(mlib_u16 *)(sl + i + 4);

		pix0 = ((pix01 & 0xFF00) >> 8);
		pix1 = (pix01 & 0x00FF);
		pix2 = ((pix23 & 0xFF00) >> 8);
		pix3 = (pix23 & 0x00FF);
		pix4 = ((pix45 & 0xFF00) >> 8);
		pix5 = (pix45 & 0x00FF);

#ifdef _LITTLE_ENDIAN
		histo1[pix0]++;
		histo0[pix1]++;
		histo0[pix2]++;
		histo2[pix3]++;
		histo2[pix4]++;
		histo1[pix5]++;
#else /* _LITTLE_ENDIAN */
		histo0[pix0]++;
		histo1[pix1]++;
		histo2[pix2]++;
		histo0[pix3]++;
		histo1[pix4]++;
		histo2[pix5]++;
#endif /* _LITTLE_ENDIAN */
	}

	if (i <= (size - 3)) {
		histo0[sl[i]]++;
		histo1[sl[i + 1]]++;
		histo2[sl[i + 2]]++;
	}
}

/* *********************************************************** */

void
mlib_ImageHistogram2_S16_4_D1(
    const mlib_s32 *sl,
    const mlib_s32 *pmask,
    mlib_s32 size,
    mlib_s32 *histo0,
    mlib_s32 *histo1,
    mlib_s32 *histo2,
    mlib_s32 *histo3)
{
	const mlib_s32 *sp = sl, *send = sl + size;
	mlib_u32 pix0, pix1, pix2, pix3;

#ifdef __SUNPRO_C
#pragma nomemorydepend
#endif /* __SUNPRO_C */
	for (; sp <= (send - 4); sp += 4) {
		pix0 = sp[0] & ~pmask[0];
		pix1 = sp[1] & ~pmask[1];
		pix2 = sp[2] & ~pmask[2];
		pix3 = sp[3] & ~pmask[3];

		histo0[pix0]++;
		histo1[pix1]++;
		histo2[pix2]++;
		histo3[pix3]++;

		pmask += 4;
	}
}

/* *********************************************************** */

void
mlib_ImageHistogram2_S32_4_D1(
    const mlib_s32 *sl,
    const mlib_s32 *pmask,
    mlib_s32 size,
    mlib_s32 *histo0,
    mlib_s32 *histo1,
    mlib_s32 *histo2,
    mlib_s32 *histo3)
{
	const mlib_s32 *sp = sl, *send = sl + size;
	mlib_u32 pix0, pix1, pix2, pix3;

#ifdef __SUNPRO_C
#pragma nomemorydepend
#endif /* __SUNPRO_C */
	for (; sp <= (send - 4); sp += 4) {
		pix0 = sp[0] & ~pmask[0];
		pix1 = sp[1] & ~pmask[1];
		pix2 = sp[2] & ~pmask[2];
		pix3 = sp[3] & ~pmask[3];

		histo0[pix0]++;
		histo1[pix1]++;
		histo2[pix2]++;
		histo3[pix3]++;

		pmask += 4;
	}
}

/* *********************************************************** */
#if defined(_NO_LONGLONG)
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
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
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
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
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
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
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
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
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

/*
 *      mlib_ImageHistogram2_S32_4_D1(const mlib_s32 *sl,
 *                                    mlib_s32       *pmask,
 *                                    mlib_s32       size,
 *                                    mlib_s32       *histo0,
 *                                    mlib_s32       *histo1,
 *                                    mlib_s32       *histo2,
 *                                    mlib_s32       *histo3);
 */

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
#else
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
	mlib_s64 numBins, rangeValue;
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

			rangeValue = ((mlib_s64)max_val - min_val + 1);
			numBins = (mlib_s64)(one_per_binWidth[l] *
					rangeValue + 0.5);

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
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
					for (i = 0; i < size; i++) {
						buf1[i] = (mlib_s32)(((
							(mlib_s64)psrc[0] -
							min_val) * numBins) /
							rangeValue);

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
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
					for (i = 0; i < size; i++) {
						buf1[i] = (mlib_s32)(((
							(mlib_s64)psrc[0] -
							min_val) * numBins) /
							rangeValue);

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
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
					for (i = 0; i < size; i++) {
						buf1[i] = (mlib_s32)(((
							(mlib_s64)psrc[0] -
							min_val) * numBins) /
							rangeValue);

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
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
					for (i = 0; i < size; i++) {
						buf1[i] = (mlib_s32)(((
							(mlib_s64)psrc[0] -
							min_val) * numBins) /
							rangeValue);


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

/*
 *      mlib_ImageHistogram2_S32_4_D1(const mlib_s32 *sl,
 *                                    mlib_s32       *pmask,
 *                                    mlib_s32       size,
 *                                    mlib_s32       *histo0,
 *                                    mlib_s32       *histo1,
 *                                    mlib_s32       *histo2,
 *                                    mlib_s32       *histo3);
 */

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
#endif
/* *********************************************************** */
