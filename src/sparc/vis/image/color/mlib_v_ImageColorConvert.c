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

#pragma ident	"@(#)mlib_v_ImageColorConvert.c	9.2	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_ImageColorConvert - color conversion
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageColorConvert1(mlib_image       *dst,
 *                                          const mlib_image *src,
 *                                          const mlib_d64   *fmat);
 *
 *      mlib_status mlib_ImageColorConvert2(mlib_image       *dst,
 *                                          const mlib_image *src,
 *                                          const mlib_d64   *fmat,
 *                                          const mlib_d64   *offset);
 *
 *      mlib_status mlib_ImageColorRGB2XYZ(mlib_image       *dst,
 *                                         const mlib_image *src);
 *
 *      mlib_status mlib_ImageColorXYZ2RGB(mlib_image       *dst,
 *                                         const mlib_image *src);
 *
 *      mlib_status mlib_ImageColorRGB2YCC(mlib_image       *dst,
 *                                         const mlib_image *src);
 *
 *      mlib_status mlib_ImageColorYCC2RGB(mlib_image       *dst,
 *                                         const mlib_image *src);
 *
 * ARGUMENT
 *      dst     pointer to output image
 *      src     pointer to input image
 *      fmat    9 parameters of color matrix in row major order
 *      offset  3 offset values
 *
 * RESTRICTION
 *      src and dst must be the same type, the same size and the same
 *      number of channels.
 *      They must be 3-channel images.
 *      They can be in MLIB_BYTE, MLIB_SHORT, MLIB_USHORT or MLIB_INT data type.
 *
 * DESCRIPTION
 *      mlib_ImageColorConvert1()
 *        |X|   |fmat[0] fmat[1] fmat[2]|   |R|
 *        |Y| = |fmat[3] fmat[4] fmat[5]| * |G|
 *        |Z|   |fmat[6] fmat[7] fmat[8]|   |B|
 *      mlib_ImageColorConvert2()
 *        |X|   |fmat[0] fmat[1] fmat[2]|   |R|   |offset[0]|
 *        |Y| = |fmat[3] fmat[4] fmat[5]| * |G| + |offset[1]|
 *        |Z|   |fmat[6] fmat[7] fmat[8]|   |B|   |offset[2]|
 *      mlib_ImageColorRGB2XYZ()
 *        convert ITU-R Rec.708 RGB with D64 white point into CIE XYZ
 *      mlib_ImageColorXYZ2RGB()
 *        convert CIE XYZ into ITU-R Rec.708 RGB with D64 white point
 *      mlib_ImageColorRGB2YCC()
 *        convert computer R'G'B' into ITU-R Rec.601 Y'CbCr
 *      mlib_ImageColorYCC2EGB()
 *        convert ITU-R Rec.601 Y'CbCr into computer R'G'B'
 */

#include <vis_proto.h>
#include <mlib_image.h>
#include <mlib_ImageCheck.h>
#include <mlib_SysMath.h>
#include <mlib_v_ImageColorConvertFunc.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageColorConvert1 = __mlib_ImageColorConvert1
#pragma weak mlib_ImageColorConvert2 = __mlib_ImageColorConvert2
#pragma weak mlib_ImageColorRGB2XYZ = __mlib_ImageColorRGB2XYZ
#pragma weak mlib_ImageColorRGB2YCC = __mlib_ImageColorRGB2YCC
#pragma weak mlib_ImageColorXYZ2RGB = __mlib_ImageColorXYZ2RGB
#pragma weak mlib_ImageColorYCC2RGB = __mlib_ImageColorYCC2RGB

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_ImageColorConvert1) mlib_ImageColorConvert1
    __attribute__((weak, alias("__mlib_ImageColorConvert1")));
__typeof__(__mlib_ImageColorConvert2) mlib_ImageColorConvert2
    __attribute__((weak, alias("__mlib_ImageColorConvert2")));
__typeof__(__mlib_ImageColorRGB2XYZ) mlib_ImageColorRGB2XYZ
    __attribute__((weak, alias("__mlib_ImageColorRGB2XYZ")));
__typeof__(__mlib_ImageColorRGB2YCC) mlib_ImageColorRGB2YCC
    __attribute__((weak, alias("__mlib_ImageColorRGB2YCC")));
__typeof__(__mlib_ImageColorXYZ2RGB) mlib_ImageColorXYZ2RGB
    __attribute__((weak, alias("__mlib_ImageColorXYZ2RGB")));
__typeof__(__mlib_ImageColorYCC2RGB) mlib_ImageColorYCC2RGB
    __attribute__((weak, alias("__mlib_ImageColorYCC2RGB")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	MAX(x, y)	(((x) > (y)) ? (x) : (y))

/* *********************************************************** */

static void mlib_v_ImageColor1_Mat_U8(
    const mlib_d64 *fmat,
    mlib_s32 *scale);

static void mlib_v_ImageColor1_Mat_S16(
    const mlib_d64 *fmat,
    mlib_s32 *scale);

static void mlib_v_ImageColor2_Mat_U8(
    const mlib_d64 *fmat,
    const mlib_d64 *foff,
    mlib_s32 *scale);

static void mlib_v_ImageColor2_Mat_S16(
    const mlib_d64 *fmat,
    const mlib_d64 *foff,
    mlib_s32 *scale);

/* *********************************************************** */

void
mlib_v_ImageColor1_Mat_U8(
    const mlib_d64 *fmat,
    mlib_s32 *scale)
{
	mlib_s32 shift;
	mlib_d64 max, c0, c1, c2, sum_pos, sum_neg;
	mlib_s32 isum_pos0, isum_pos1, isum_pos2;
	mlib_s32 isum_neg0, isum_neg1, isum_neg2, scalef;

	sum_pos = 0;
	sum_neg = 0;

	if (fmat[0] > 0)
		sum_pos += fmat[0];
	else
		sum_neg += fmat[0];
	if (fmat[1] > 0)
		sum_pos += fmat[1];
	else
		sum_neg += fmat[1];
	if (fmat[2] > 0)
		sum_pos += fmat[2];
	else
		sum_neg += fmat[2];

	c0 = MAX(sum_pos, (-sum_neg));

	sum_pos = 0;
	sum_neg = 0;

	if (fmat[3] > 0)
		sum_pos += fmat[3];
	else
		sum_neg += fmat[3];
	if (fmat[4] > 0)
		sum_pos += fmat[4];
	else
		sum_neg += fmat[4];
	if (fmat[5] > 0)
		sum_pos += fmat[5];
	else
		sum_neg += fmat[5];

	c1 = MAX(sum_pos, (-sum_neg));

	sum_pos = 0;
	sum_neg = 0;

	if (fmat[6] > 0)
		sum_pos += fmat[6];
	else
		sum_neg += fmat[6];
	if (fmat[7] > 0)
		sum_pos += fmat[7];
	else
		sum_neg += fmat[7];
	if (fmat[8] > 0)
		sum_pos += fmat[8];
	else
		sum_neg += fmat[8];

	c2 = MAX(sum_pos, (-sum_neg));

	max = MAX(MAX(c0, c1), c2);

	shift = 0;
	while (max > 1) {
		max *= 0.5;
		shift++;
	}

	scalef = (0x10000 >> (shift + 1));

	isum_pos0 = 0;
	isum_neg0 = 0;
	isum_pos1 = 0;
	isum_neg1 = 0;
	isum_pos2 = 0;
	isum_neg2 = 0;

	if (fmat[0] >= 0)
		isum_pos0 += (mlib_s32)(fmat[0] * scalef + 0.5);
	else
		isum_neg0 += (mlib_s32)(fmat[0] * scalef - 0.5);
	if (fmat[1] >= 0)
		isum_pos0 += (mlib_s32)(fmat[1] * scalef + 0.5);
	else
		isum_neg0 += (mlib_s32)(fmat[1] * scalef - 0.5);
	if (fmat[2] >= 0)
		isum_pos0 += (mlib_s32)(fmat[2] * scalef + 0.5);
	else
		isum_neg0 += (mlib_s32)(fmat[2] * scalef - 0.5);

	if (fmat[3] >= 0)
		isum_pos1 += (mlib_s32)(fmat[3] * scalef + 0.5);
	else
		isum_neg1 += (mlib_s32)(fmat[3] * scalef - 0.5);
	if (fmat[4] >= 0)
		isum_pos1 += (mlib_s32)(fmat[4] * scalef + 0.5);
	else
		isum_neg1 += (mlib_s32)(fmat[4] * scalef - 0.5);
	if (fmat[5] >= 0)
		isum_pos1 += (mlib_s32)(fmat[5] * scalef + 0.5);
	else
		isum_neg1 += (mlib_s32)(fmat[5] * scalef - 0.5);

	if (fmat[6] >= 0)
		isum_pos2 += (mlib_s32)(fmat[6] * scalef + 0.5);
	else
		isum_neg2 += (mlib_s32)(fmat[6] * scalef - 0.5);
	if (fmat[7] >= 0)
		isum_pos2 += (mlib_s32)(fmat[7] * scalef + 0.5);
	else
		isum_neg2 += (mlib_s32)(fmat[7] * scalef - 0.5);
	if (fmat[8] >= 0)
		isum_pos2 += (mlib_s32)(fmat[8] * scalef + 0.5);
	else
		isum_neg2 += (mlib_s32)(fmat[8] * scalef - 0.5);

	if ((isum_pos0 > 32767) || (isum_neg0 < -32768) ||
	    (isum_pos1 > 32767) || (isum_neg1 < -32768) ||
	    (isum_pos2 > 32767) || (isum_neg2 < -32768))
		shift++;

	*scale = shift;
}

/* *********************************************************** */

void
mlib_v_ImageColor1_Mat_S16(
    const mlib_d64 *fmat,
    mlib_s32 *scale)
{
	mlib_s32 shift;
	mlib_d64 max, c0, c1, c2;
	mlib_s32 test, i, sum, smat, scalef;

	c0 = mlib_fabs(fmat[0]) + mlib_fabs(fmat[1]) + mlib_fabs(fmat[2]);
	c1 = mlib_fabs(fmat[3]) + mlib_fabs(fmat[4]) + mlib_fabs(fmat[5]);
	c2 = mlib_fabs(fmat[6]) + mlib_fabs(fmat[7]) + mlib_fabs(fmat[8]);
	max = MAX(MAX(c0, c1), c2);

	shift = 0;
	while (max > 1) {
		max *= 0.5;
		shift++;
	}

	if (shift == 0)
		shift = 1;

	scalef = (0x10000 >> (shift));

	i = 0;
	test = 0;
	sum = 0;

	while ((i < 3) && (test == 0)) {
		smat = (mlib_s32)(mlib_fabs(fmat[i]) * scalef + 0.5);

		if (smat > 32767)
			test = 1;
		else
			sum += smat;
		i++;
	}

	if (test == 0) {
		if (sum > 65535)
			test = 1;
	}

	i = 3;
	sum = 0;

	while ((i < 6) && (test == 0)) {
		smat = (mlib_s32)(mlib_fabs(fmat[i]) * scalef + 0.5);

		if (smat > 32767)
			test = 1;
		else
			sum += smat;
		i++;
	}

	if (test == 0) {
		if (sum > 65535)
			test = 1;
	}

	i = 6;
	sum = 0;

	while ((i < 9) && (test == 0)) {
		smat = (mlib_s32)(mlib_fabs(fmat[i]) * scalef + 0.5);

		if (smat > 32767)
			test = 1;
		else
			sum += smat;
		i++;
	}

	if (test == 0) {
		if (sum > 65535)
			test = 1;
	}

	if (test != 0)
		shift++;

	*scale = shift;
}

/* *********************************************************** */

void
mlib_v_ImageColor2_Mat_U8(
    const mlib_d64 *fmat,
    const mlib_d64 *foff,
    mlib_s32 *scale)
{
	mlib_s32 shift;
	mlib_d64 max, c0, c1, c2;
	mlib_d64 foff0, foff1, foff2, sum_pos, sum_neg;
	mlib_s32 test, smat0, smat1, smat2, i, scalef, smat;

	foff0 = foff[0] / 256;
	foff1 = foff[1] / 256;
	foff2 = foff[2] / 256;

	sum_pos = sum_neg = foff0;

	if (fmat[0] > 0)
		sum_pos += fmat[0];
	else
		sum_neg += fmat[0];
	if (fmat[1] > 0)
		sum_pos += fmat[1];
	else
		sum_neg += fmat[1];
	if (fmat[2] > 0)
		sum_pos += fmat[2];
	else
		sum_neg += fmat[2];

	c0 = MAX(mlib_fabs(sum_pos), mlib_fabs(sum_neg));
	c0 = MAX(c0, mlib_fabs(fmat[0]));
	c0 = MAX(c0, mlib_fabs(fmat[1]));
	c0 = MAX(c0, mlib_fabs(fmat[2]));
	c0 = MAX(c0, mlib_fabs(foff0));

	sum_pos = sum_neg = foff1;

	if (fmat[3] > 0)
		sum_pos += fmat[3];
	else
		sum_neg += fmat[3];
	if (fmat[4] > 0)
		sum_pos += fmat[4];
	else
		sum_neg += fmat[4];
	if (fmat[5] > 0)
		sum_pos += fmat[5];
	else
		sum_neg += fmat[5];

	c1 = MAX(mlib_fabs(sum_pos), mlib_fabs(sum_neg));
	c1 = MAX(c1, mlib_fabs(fmat[3]));
	c1 = MAX(c1, mlib_fabs(fmat[4]));
	c1 = MAX(c1, mlib_fabs(fmat[5]));
	c1 = MAX(c1, mlib_fabs(foff1));

	sum_pos = sum_neg = foff2;

	if (fmat[6] > 0)
		sum_pos += fmat[6];
	else
		sum_neg += fmat[6];
	if (fmat[7] > 0)
		sum_pos += fmat[7];
	else
		sum_neg += fmat[7];
	if (fmat[8] > 0)
		sum_pos += fmat[8];
	else
		sum_neg += fmat[8];

	c2 = MAX(mlib_fabs(sum_pos), mlib_fabs(sum_neg));
	c2 = MAX(c2, mlib_fabs(fmat[6]));
	c2 = MAX(c2, mlib_fabs(fmat[7]));
	c2 = MAX(c2, mlib_fabs(fmat[8]));
	c2 = MAX(c2, mlib_fabs(foff2));

	max = MAX(MAX(c0, c1), c2);

	shift = 0;
	while (max > 1) {
		max *= 0.5;
		shift++;
	}

	scalef = (0x100 >> (shift + 1));
	test = 0;

	if (foff[0] >= 0)
		smat0 = (mlib_s32)(foff[0] * scalef + 0.5);
	else
		smat0 = (mlib_s32)(foff[0] * scalef - 0.5);
	if (foff[1] >= 0)
		smat1 = (mlib_s32)(foff[1] * scalef + 0.5);
	else
		smat1 = (mlib_s32)(foff[1] * scalef - 0.5);
	if (foff[2] >= 0)
		smat2 = (mlib_s32)(foff[2] * scalef + 0.5);
	else
		smat2 = (mlib_s32)(foff[2] * scalef - 0.5);

	if ((smat0 > 32767) || (smat0 < -32768) ||
	    (smat1 > 32767) || (smat1 < -32768) ||
	    (smat2 > 32767) || (smat2 < -32768))
		test = 1;

	scalef = (0x10000 >> (shift + 1));

	i = 0;
	sum_pos = 0;
	sum_neg = 0;

	while ((i < 3) && (test == 0)) {

		if (fmat[i] >= 0) {
			smat = (mlib_s32)(fmat[i] * scalef + 0.5);
			sum_pos += smat;
		} else {
			smat = (mlib_s32)(fmat[i] * scalef - 0.5);
			sum_neg += smat;
		}

		if ((smat > 32767) || (smat < -32768))
			test = 1;
		i++;
	}

	if (test == 0) {
		if (((sum_pos + smat0) > 32767) || ((sum_neg + smat0) < -32768))
			test = 1;
	}

	i = 3;
	sum_pos = 0;
	sum_neg = 0;

	while ((i < 6) && (test == 0)) {

		if (fmat[i] >= 0) {
			smat = (mlib_s32)(fmat[i] * scalef + 0.5);
			sum_pos += smat;
		} else {
			smat = (mlib_s32)(fmat[i] * scalef - 0.5);
			sum_neg += smat;
		}

		if ((smat > 32767) || (smat < -32768))
			test = 1;
		i++;
	}

	if (test == 0) {
		if (((sum_pos + smat1) > 32767) || ((sum_neg + smat1) < -32768))
			test = 1;
	}

	i = 6;
	sum_pos = 0;
	sum_neg = 0;

	while ((i < 9) && (test == 0)) {

		if (fmat[i] >= 0) {
			smat = (mlib_s32)(fmat[i] * scalef + 0.5);
			sum_pos += smat;
		} else {
			smat = (mlib_s32)(fmat[i] * scalef - 0.5);
			sum_neg += smat;
		}

		if ((smat > 32767) || (smat < -32768))
			test = 1;
		i++;
	}

	if (test == 0) {
		if (((sum_pos + smat2) > 32767) || ((sum_neg + smat2) < -32768))
			test = 1;
	}

	if (test != 0)
		shift++;

	*scale = shift;
}

/* *********************************************************** */

void
mlib_v_ImageColor2_Mat_U16(
    const mlib_d64 *fmat,
    const mlib_d64 *foff,
    mlib_s32 *scale)
{
	mlib_s32 shift;
	mlib_d64 max, c0, c1, c2;
	mlib_d64 foff0, foff1, foff2, sum_pos, sum_neg;
	mlib_s32 test, smat0, smat1, smat2, i, scalef, smat;

	foff0 = foff[0] / 65536;
	foff1 = foff[1] / 65536;
	foff2 = foff[2] / 65536;

	sum_pos = sum_neg = foff0 - 0.5;

	if (fmat[0] > 0)
		sum_pos += fmat[0];
	else
		sum_neg += fmat[0];
	if (fmat[1] > 0)
		sum_pos += fmat[1];
	else
		sum_neg += fmat[1];
	if (fmat[2] > 0)
		sum_pos += fmat[2];
	else
		sum_neg += fmat[2];

	c0 = MAX(mlib_fabs(sum_pos), mlib_fabs(sum_neg));
	c0 = MAX(c0, mlib_fabs(fmat[0]));
	c0 = MAX(c0, mlib_fabs(fmat[1]));
	c0 = MAX(c0, mlib_fabs(fmat[2]));
	c0 = MAX(c0, mlib_fabs(foff0));

	sum_pos = sum_neg = foff1 - 0.5;

	if (fmat[3] > 0)
		sum_pos += fmat[3];
	else
		sum_neg += fmat[3];
	if (fmat[4] > 0)
		sum_pos += fmat[4];
	else
		sum_neg += fmat[4];
	if (fmat[5] > 0)
		sum_pos += fmat[5];
	else
		sum_neg += fmat[5];

	c1 = MAX(mlib_fabs(sum_pos), mlib_fabs(sum_neg));
	c1 = MAX(c1, mlib_fabs(fmat[3]));
	c1 = MAX(c1, mlib_fabs(fmat[4]));
	c1 = MAX(c1, mlib_fabs(fmat[5]));
	c1 = MAX(c1, mlib_fabs(foff1));

	sum_pos = sum_neg = foff2 - 0.5;

	if (fmat[6] > 0)
		sum_pos += fmat[6];
	else
		sum_neg += fmat[6];
	if (fmat[7] > 0)
		sum_pos += fmat[7];
	else
		sum_neg += fmat[7];
	if (fmat[8] > 0)
		sum_pos += fmat[8];
	else
		sum_neg += fmat[8];

	c2 = MAX(mlib_fabs(sum_pos), mlib_fabs(sum_neg));
	c2 = MAX(c2, mlib_fabs(fmat[6]));
	c2 = MAX(c2, mlib_fabs(fmat[7]));
	c2 = MAX(c2, mlib_fabs(fmat[8]));
	c2 = MAX(c2, mlib_fabs(foff2));

	max = MAX(MAX(c0, c1), c2);

	shift = 1;
	while (max > 1) {
		max *= 0.5;
		shift++;
	}

	scalef = (0x10000 >> (shift));
	test = 0;

	if (foff0 >= 0)
		smat0 = (mlib_s32)(foff0 * scalef + 0.5);
	else
		smat0 = (mlib_s32)(foff0 * scalef - 0.5);
	if (foff1 >= 0)
		smat1 = (mlib_s32)(foff1 * scalef + 0.5);
	else
		smat1 = (mlib_s32)(foff1 * scalef - 0.5);
	if (foff2 >= 0)
		smat2 = (mlib_s32)(foff2 * scalef + 0.5);
	else
		smat2 = (mlib_s32)(foff2 * scalef - 0.5);

	if ((smat0 > 32767) || (smat0 < -32768) ||
	    (smat1 > 32767) || (smat1 < -32768) ||
	    (smat2 > 32767) || (smat2 < -32768))
		test = 1;

	smat0 -= (0x10000 >> (shift + 1));
	smat1 -= (0x10000 >> (shift + 1));
	smat2 -= (0x10000 >> (shift + 1));

	i = 0;
	sum_pos = 0;
	sum_neg = 0;

	while ((i < 3) && (test == 0)) {

		if (fmat[i] >= 0) {
			smat = (mlib_s32)(fmat[i] * scalef + 0.5);
			sum_pos += smat;
		} else {
			smat = (mlib_s32)(fmat[i] * scalef - 0.5);
			sum_neg += smat;
		}

		if ((smat > 32767) || (smat < -32768))
			test = 1;
		i++;
	}

	if (test == 0) {
		if (((sum_pos + smat0) > 32767) || ((sum_neg + smat0) < -32768))
			test = 1;
	}

	i = 3;
	sum_pos = 0;
	sum_neg = 0;

	while ((i < 6) && (test == 0)) {

		if (fmat[i] >= 0) {
			smat = (mlib_s32)(fmat[i] * scalef + 0.5);
			sum_pos += smat;
		} else {
			smat = (mlib_s32)(fmat[i] * scalef - 0.5);
			sum_neg += smat;
		}

		if ((smat > 32767) || (smat < -32768))
			test = 1;
		i++;
	}

	if (test == 0) {
		if (((sum_pos + smat1) > 32767) || ((sum_neg + smat1) < -32768))
			test = 1;
	}

	i = 6;
	sum_pos = 0;
	sum_neg = 0;

	while ((i < 9) && (test == 0)) {

		if (fmat[i] >= 0) {
			smat = (mlib_s32)(fmat[i] * scalef + 0.5);
			sum_pos += smat;
		} else {
			smat = (mlib_s32)(fmat[i] * scalef - 0.5);
			sum_neg += smat;
		}

		if ((smat > 32767) || (smat < -32768))
			test = 1;
		i++;
	}

	if (test == 0) {
		if (((sum_pos + smat2) > 32767) || ((sum_neg + smat2) < -32768))
			test = 1;
	}

	if (test != 0)
		shift++;

	*scale = shift;
}

/* *********************************************************** */

void
mlib_v_ImageColor2_Mat_S16(
    const mlib_d64 *fmat,
    const mlib_d64 *foff,
    mlib_s32 *scale)
{
	mlib_s32 shift;
	mlib_d64 max, c0, c1, c2;
	mlib_s32 test, i, sum, scalef, smat;

	c0 = mlib_fabs(fmat[0]) + mlib_fabs(fmat[1]) + mlib_fabs(fmat[2]) +
	    mlib_fabs(foff[0] / 32768);
	c1 = mlib_fabs(fmat[3]) + mlib_fabs(fmat[4]) + mlib_fabs(fmat[5]) +
	    mlib_fabs(foff[1] / 32768);
	c2 = mlib_fabs(fmat[6]) + mlib_fabs(fmat[7]) + mlib_fabs(fmat[8]) +
	    mlib_fabs(foff[2] / 32768);
	max = MAX(MAX(c0, c1), c2);

	shift = 0;
	while (max > 1) {
		max *= 0.5;
		shift++;
	}

	if (shift == 0)
		shift = 1;

	scalef = (0x10000 >> (shift));

	i = 0;
	test = 0;
	sum = 0;

	while ((i < 3) && (test == 0)) {
		smat = (mlib_s32)(mlib_fabs(fmat[i]) * scalef + 0.5);

		if (smat > 32767)
			test = 1;
		else
			sum += smat;
		i++;
	}

	if (test == 0) {
		smat = (mlib_s32)(mlib_fabs(foff[0]) * scalef + 0.5);

		if (smat > 32767)
			test = 1;
		if ((sum + smat) > 65535)
			test = 1;
	}

	i = 3;
	sum = 0;

	while ((i < 6) && (test == 0)) {
		smat = (mlib_s32)(mlib_fabs(fmat[i]) * scalef + 0.5);

		if (smat > 32767)
			test = 1;
		else
			sum += smat;
		i++;
	}

	if (test == 0) {
		smat = (mlib_s32)(mlib_fabs(foff[1]) * scalef + 0.5);

		if (smat > 32767)
			test = 1;
		if ((sum + smat) > 65535)
			test = 1;
	}

	i = 6;
	sum = 0;

	while ((i < 9) && (test == 0)) {
		smat = (mlib_s32)(mlib_fabs(fmat[i]) * scalef + 0.5);

		if (smat > 32767)
			test = 1;
		else
			sum += smat;
		i++;
	}

	if (test == 0) {
		smat = (mlib_s32)(mlib_fabs(foff[2]) * scalef + 0.5);

		if (smat > 32767)
			test = 1;
		if ((sum + smat) > 65535)
			test = 1;
	}

	if (test != 0)
		shift++;

	*scale = shift;
}

/* *********************************************************** */

mlib_status
__mlib_ImageColorConvert1(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_d64 *fmat)
{
	mlib_s32 slb, dlb, xsize, ysize;
	mlib_type dtype;
	void *sa, *da;
	const mlib_s32 D1 = MLIB_IMAGE_ONEDVECTOR;
	mlib_d64 offset0[3] = { 0, 0, 0 };
	mlib_s32 flags, flagd;
	mlib_s32 dsize;
	mlib_s32 scale;

	MLIB_IMAGE_CHECK(src);
	MLIB_IMAGE_CHECK(dst);
	MLIB_IMAGE_FULL_EQUAL(dst, src);
	MLIB_IMAGE_HAVE_CHAN(dst, 3);

	if (fmat == NULL)
		return (MLIB_NULLPOINTER);

	dtype = mlib_ImageGetType(dst);
	xsize = mlib_ImageGetWidth(dst);
	ysize = mlib_ImageGetHeight(dst);
	slb = mlib_ImageGetStride(src);
	dlb = mlib_ImageGetStride(dst);
	sa = mlib_ImageGetData(src);
	da = mlib_ImageGetData(dst);
	flags = mlib_ImageGetFlags(src);
	flagd = mlib_ImageGetFlags(dst);

	if (dtype == MLIB_BYTE) {
		mlib_v_ImageColor1_Mat_U8(fmat, &scale);

		if (scale > 7)
			return (MLIB_FAILURE);

		mlib_v_ImageColorConvert2_U8(sa, slb, da, dlb, xsize, ysize,
		    fmat, offset0, scale);

		return (MLIB_SUCCESS);
	} else if (dtype == MLIB_SHORT) {
		mlib_v_ImageColor1_Mat_S16(fmat, &scale);

		if (scale > 15)
			return (MLIB_FAILURE);

/* initialize GSR scale factor and offset */
		vis_write_gsr(((scale & 0xF) << 3) + 6);

		if (((flags & D1) == 0) && ((flagd & D1) == 0)) {
			dsize = xsize * ysize;
			mlib_v_ImageColorConvert1_S16_D1(sa, da, dsize, fmat,
			    scale);
		} else {
			mlib_v_ImageColorConvert1_S16(sa, slb / 2, da, dlb / 2,
			    xsize, ysize, fmat, scale);
		}

		return (MLIB_SUCCESS);
	} else if (dtype == MLIB_USHORT) {

		mlib_v_ImageColor2_Mat_U16(fmat, offset0, &scale);

		if (scale > 15) {
			return (MLIB_FAILURE);
		}

/* initialize GSR scale factor and offset */
		vis_write_gsr(((scale & 0xF) << 3) + 6);

		mlib_v_ImageColorConvert1_U16(sa, slb / 2, da, dlb / 2, xsize,
		    ysize, fmat, scale);
		return (MLIB_SUCCESS);
	} else if (dtype == MLIB_INT) {
		mlib_v_ImageColorConvert1_S32(sa, slb / 4,
		    da, dlb / 4, xsize, ysize, fmat);
		return (MLIB_SUCCESS);
	} else
		return (MLIB_FAILURE);
}

/* *********************************************************** */

mlib_status
__mlib_ImageColorConvert2(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_d64 *fmat,
    const mlib_d64 *offset)
{
	mlib_s32 slb, dlb, xsize, ysize;
	mlib_type dtype;
	void *sa, *da;
	const mlib_s32 D1 = MLIB_IMAGE_ONEDVECTOR;
	mlib_s32 flags, flagd;
	mlib_s32 dsize;
	mlib_s32 scale;

	MLIB_IMAGE_CHECK(src);
	MLIB_IMAGE_CHECK(dst);
	MLIB_IMAGE_FULL_EQUAL(dst, src);
	MLIB_IMAGE_HAVE_CHAN(dst, 3);

	if (fmat == NULL || offset == NULL)
		return (MLIB_NULLPOINTER);

	dtype = mlib_ImageGetType(dst);
	xsize = mlib_ImageGetWidth(dst);
	ysize = mlib_ImageGetHeight(dst);
	slb = mlib_ImageGetStride(src);
	dlb = mlib_ImageGetStride(dst);
	sa = mlib_ImageGetData(src);
	da = mlib_ImageGetData(dst);
	flags = mlib_ImageGetFlags(src);
	flagd = mlib_ImageGetFlags(dst);

	if (dtype == MLIB_BYTE) {

		mlib_v_ImageColor2_Mat_U8(fmat, offset, &scale);

		if (scale > 7)
			return (MLIB_FAILURE);

		mlib_v_ImageColorConvert2_U8(sa, slb, da, dlb, xsize, ysize,
		    fmat, offset, scale);

		return (MLIB_SUCCESS);

	} else if (dtype == MLIB_SHORT) {

		mlib_v_ImageColor2_Mat_S16(fmat, offset, &scale);

		if (scale > 15)
			return (MLIB_FAILURE);

/* initialize GSR scale factor and offset */
		vis_write_gsr(((scale & 0xF) << 3) + 6);

		if (((flags & D1) == 0) && ((flagd & D1) == 0)) {
			dsize = xsize * ysize;
			mlib_v_ImageColorConvert2_S16_D1(sa, da, dsize, fmat,
			    offset, scale);
		} else {
			mlib_v_ImageColorConvert2_S16(sa, slb / 2, da, dlb / 2,
			    xsize, ysize, fmat, offset, scale);
		}

		return (MLIB_SUCCESS);
	} else if (dtype == MLIB_USHORT) {

		mlib_v_ImageColor2_Mat_U16(fmat, offset, &scale);

		if (scale > 15) {
			return (MLIB_FAILURE);
		}

/* initialize GSR scale factor and offset */
		vis_write_gsr(((scale & 0xF) << 3) + 6);

		mlib_v_ImageColorConvert2_U16(sa, slb / 2, da, dlb / 2, xsize,
		    ysize, fmat, offset, scale);
		return (MLIB_SUCCESS);
	} else if (dtype == MLIB_INT) {
		mlib_v_ImageColorConvert2_S32(sa, slb / 4,
		    da, dlb / 4, xsize, ysize, fmat, offset);
		return (MLIB_SUCCESS);
	} else
		return (MLIB_FAILURE);
}

/* *********************************************************** */

mlib_status
__mlib_ImageColorRGB2XYZ(
    mlib_image *dst,
    const mlib_image *src)
{
/* Rec709 RGB with D64 White Point to CIE XYZ */
	mlib_d64 fmat[9] = { 0.412453, 0.357580, 0.180423,
		0.212671, 0.715160, 0.072169,
		0.019334, 0.119193, 0.950227
	};

	return (__mlib_ImageColorConvert1(dst, src, fmat));
}

/* *********************************************************** */

mlib_status
__mlib_ImageColorXYZ2RGB(
    mlib_image *dst,
    const mlib_image *src)
{
/* CIE XYZ to Rec709 RGB with D64 White Point */
	mlib_d64 fmat[9] = { 3.240479, -1.537150, -0.498535,
		-0.969256, 1.875992, 0.041566,
		0.055648, -0.204043, 1.057311
	};

	return (__mlib_ImageColorConvert1(dst, src, fmat));
}

/* *********************************************************** */

mlib_status
__mlib_ImageColorRGB2YCC(
    mlib_image *dst,
    const mlib_image *src)
{
/* Computer R'G'B' to Rec601 Y'CbCr */
	mlib_d64 fmat[9] = { 65.738 / 256, 129.057 / 256, 25.064 / 256,
		-37.945 / 256, -74.494 / 256, 112.439 / 256,
		112.439 / 256, -94.154 / 256, -18.285 / 256
	};
	mlib_d64 offset[3] = { 16, 128, 128 };

	return (__mlib_ImageColorConvert2(dst, src, fmat, offset));
}

/* *********************************************************** */

mlib_status
__mlib_ImageColorYCC2RGB(
    mlib_image *dst,
    const mlib_image *src)
{
/* Rec601 Y'CbCr to Computer R'G'B' */
	mlib_d64 fmat[9] = { 298.082 / 256, 0, 408.583 / 256,
		298.082 / 256, -100.291 / 256, -208.120 / 256,
		298.082 / 256, 516.411 / 256, 0
	};
	mlib_d64 offset[3] = { -222.953, 135.575, -276.836 };

	return (__mlib_ImageColorConvert2(dst, src, fmat, offset));
}

/* *********************************************************** */
