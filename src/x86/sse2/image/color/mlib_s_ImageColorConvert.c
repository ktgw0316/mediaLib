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

#pragma ident	"@(#)mlib_s_ImageColorConvert.c	9.7	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_ImageColorConvert - color conversion
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageColorConvert1(mlib_image *dst,
 *                                          const mlib_image *src,
 *                                          const mlib_d64 *fmat);
 *      mlib_status mlib_ImageColorConvert2(mlib_image *dst,
 *                                          const mlib_image *src,
 *                                          const mlib_d64 *fmat,
 *                                          const mlib_d64 *offset);
 *      mlib_status mlib_ImageColorRGB2XYZ(mlib_image *dst,
 *                                         const mlib_image *src);
 *      mlib_status mlib_ImageColorXYZ2RGB(mlib_image *dst,
 *                                         const mlib_image *src);
 *      mlib_status mlib_ImageColorRGB2YCC(mlib_image *dst,
 *                                         const mlib_image *src);
 *      mlib_status mlib_ImageColorYCC2RGB(mlib_image *dst,
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
 *      mlib_ImageColorYCC2RGB()
 *        convert ITU-R Rec.601 Y'CbCr into computer R'G'B'
 */

#include <mlib_image.h>
#include <mlib_SysMath.h>
#include <mlib_ImageCheck.h>
#include <mlib_sse_utils.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageColorConvert1 = __mlib_ImageColorConvert1
#pragma weak mlib_ImageColorConvert2 = __mlib_ImageColorConvert2

#elif defined(__GNUC__)  /* defined(__SUNPRO_C) */

__typeof__(__mlib_ImageColorConvert1) mlib_ImageColorConvert1
    __attribute__((weak, alias("__mlib_ImageColorConvert1")));
__typeof__(__mlib_ImageColorConvert2) mlib_ImageColorConvert2
    __attribute__((weak, alias("__mlib_ImageColorConvert2")));

#else /* defined(__SUNPRO_C) */

#error "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	SAT_S32(dst, src)		\
	if (src > MLIB_S32_MAX)		\
		dst = MLIB_S32_MAX;	\
	else if (src < MLIB_S32_MIN)	\
		dst = MLIB_S32_MIN;	\
	else				\
		dst = src;

/* *********************************************************** */

    mlib_status mlib_s_ImageColorConvert1_U8(
    const mlib_u8 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_d64 *fmat,
    mlib_s32 scale);

    mlib_status mlib_s_ImageColorConvert1_S16(
    const mlib_s16 *src,
    mlib_s32 slb,
    mlib_s16 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_d64 *fmat,
    mlib_s32 scale);

    mlib_status mlib_s_ImageColorConvert2_U8(
    const mlib_u8 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_d64 *fmat,
    const mlib_d64 *offset,
    mlib_s32 scale);

    mlib_status mlib_s_ImageColorConvert2_S16(
    const mlib_s16 *src,
    mlib_s32 slb,
    mlib_s16 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_d64 *fmat,
    const mlib_d64 *offset,
    mlib_s32 scale);

    mlib_status mlib_s_ImageColorConvert2_U16(
    const mlib_u16 *src,
    mlib_s32 slb,
    mlib_u16 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_d64 *fmat,
    const mlib_d64 *offset,
    mlib_s32 scale);

/* *********************************************************** */

#define	MAX(x, y)	(((x) > (y)) ? (x) : (y))

/* *********************************************************** */

#define	SAT8(DST, SRC)                                          \
	SRC -= sat;                                             \
	if (SRC >= MLIB_S32_MAX)                                \
	    (*DST++) = MLIB_U8_MAX;                             \
	else if (SRC <= MLIB_S32_MIN)                           \
	    (*DST++) = MLIB_U8_MIN;                             \
	else                                                    \
	    (*DST++) = ((mlib_s32)SRC >> 24) ^ 0x80;

#define	SAT32(DST, SRC)                                         \
	if (SRC >= MLIB_S32_MAX)                                \
	    (*DST++) = MLIB_S32_MAX;                            \
	else if (SRC <= MLIB_S32_MIN)                           \
	    (*DST++) = MLIB_S32_MIN;                            \
	else                                                    \
	    (*DST++) = (mlib_s32)SRC;

/* *********************************************************** */

void
mlib_s_ImageColor1_Mat_U8(
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
mlib_s_ImageColor1_Mat_S16(
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

#if defined(_LP64) && !defined(_NO_LONGLONG)
#define	ZOOM32 (1024.0 * 1024 * 256)
#define	SHIFT32 28

mlib_status
mlib_s_ImageColorConvert1_S32(
    const mlib_s32 *src,
    mlib_s32 slb,
    mlib_s32 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_d64 *fmat)
{
	mlib_s64 r, w0, w1, w2, w3, w4, w5, w6, w7, w8;
	int i, j, sny, dny;
	int s0, s1, s2;

	w0 = fmat[0] * ZOOM32;
	w1 = fmat[1] * ZOOM32;
	w2 = fmat[2] * ZOOM32;
	w3 = fmat[3] * ZOOM32;
	w4 = fmat[4] * ZOOM32;
	w5 = fmat[5] * ZOOM32;
	w6 = fmat[6] * ZOOM32;
	w7 = fmat[7] * ZOOM32;
	w8 = fmat[8] * ZOOM32;
	sny = 0;
	dny = 0;
	for (j = 0; j < ysize; j++) {
		int snb = sny;
		int dnb = dny;

		for (i = 0; i < xsize; i++) {
			s0 = src[snb];
			s1 = src[snb + 1];
			s2 = src[snb + 2];

			r = (s0 * w0 + s1 * w1 + s2 * w2) >> SHIFT32;
			SAT_S32(dst[dnb], r);

			r = (s0 * w3 + s1 * w4 + s2 * w5) >> SHIFT32;
			SAT_S32(dst[dnb + 1], r);

			r = (s0 * w6 + s1 * w7 + s2 * w8) >> SHIFT32;
			SAT_S32(dst[dnb + 2], r);

			snb += 3;
			dnb += 3;
		}

		sny += slb;
		dny += dlb;
	}

	return (MLIB_SUCCESS);
}

#else	/* defined(_LP64) && !defined(_NO_LONGLONG) */

mlib_status
mlib_s_ImageColorConvert1_S32(
    const mlib_s32 *src,
    mlib_s32 slb,
    mlib_s32 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_d64 *fmat)
{
/* pointers for pixel and line of source */
	const mlib_s32 *sa, *sl;

/* pointers for pixel and line of destination */
	mlib_s32 *da, *dl;

/* indices */
	mlib_s32 i, j;

/* accumulators */
	mlib_d64 sum1, sum2, sum3;

/* coeff's */
	mlib_d64 k0, k1, k2, k3, k4, k5, k6, k7, k8;
	mlib_d64 p0, p1, p2;

/* keep kernel in regs */
	k0 = fmat[0];
	k1 = fmat[1];
	k2 = fmat[2];
	k3 = fmat[3];
	k4 = fmat[4];
	k5 = fmat[5];
	k6 = fmat[6];
	k7 = fmat[7];
	k8 = fmat[8];

	sa = sl = src;
	da = dl = dst;

	for (j = 0; j < ysize; j++) {

		for (i = 0; i < 3 * xsize; i += 3) {
			p0 = sa[i];
			p1 = sa[i + 1];
			p2 = sa[i + 2];
			sum1 = p0 * k0 + p1 * k1 + p2 * k2;
			sum2 = p0 * k3 + p1 * k4 + p2 * k5;
			sum3 = p0 * k6 + p1 * k7 + p2 * k8;

			SAT32(da, sum1)
			    SAT32(da, sum2)
			    SAT32(da, sum3)
		}
/* set src ptrs to next row */
		sl = sa = sl + slb;
/* set dst ptrs to next row */
		dl = da = dl + dlb;
	}

	return (MLIB_SUCCESS);
}
#endif 	/* defined(_LP64) && !defined(_NO_LONGLONG) */

/* *********************************************************** */

void
mlib_s_ImageColor2_Mat_U8(
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
mlib_s_ImageColor2_Mat_S16(
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

#if defined(_LP64) && !defined(_NO_LONGLONG)

mlib_status
mlib_s_ImageColorConvert2_S32(
    const mlib_s32 *src,
    mlib_s32 slb,
    mlib_s32 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_d64 *fmat,
    const mlib_d64 *offset)
{
	mlib_s64 r, w[9], off0, off1, off2;
	int i, j, nblock, sny, dny;
	int s0, s1, s2;
	for (i = 0; i < 9; i++)
		w[i] = fmat[i] * ZOOM32;
	off0 = offset[0] * ZOOM32;
	off1 = offset[1] * ZOOM32;
	off2 = offset[2] * ZOOM32;

	nblock = xsize >> 1;
	sny = 0;
	dny = 0;
	for (j = 0; j < ysize; j++) {
		int snb = sny;
		int dnb = dny;

		for (i = 0; i < xsize; i++) {
			s0 = src[snb];
			s1 = src[snb + 1];
			s2 = src[snb + 2];

			r = (s0 * w[0] + s1 * w[1] + s2 * w[2] + off0)
				>> SHIFT32;
			SAT_S32(dst[dnb], r);

			r = (s0 * w[3] + s1 * w[4] + s2 * w[5] + off1)
				>> SHIFT32;
			SAT_S32(dst[dnb + 1], r);

			r = (s0 * w[6] + s1 * w[7] + s2 * w[8] + off2)
				>> SHIFT32;
			SAT_S32(dst[dnb + 2], r);

			snb += 3;
			dnb += 3;
		}

		sny += slb;
		dny += dlb;
	}

	return (MLIB_SUCCESS);
}

#else	/* defined(_LP64) && !defined(_NO_LONGLONG) */

mlib_status
mlib_s_ImageColorConvert2_S32(
    const mlib_s32 *src,
    mlib_s32 slb,
    mlib_s32 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_d64 *fmat,
    const mlib_d64 *offset)
{
/* pointers for pixel and line of source */
	const mlib_s32 *sa, *sl;

/* pointers for pixel and line of destination */
	mlib_s32 *da, *dl;

/* indices */
	mlib_s32 i, j;

/* accumulators */
	mlib_d64 sum1, sum2, sum3;

/* coeff's */
	mlib_d64 k0, k1, k2, k3, k4, k5, k6, k7, k8;
	mlib_d64 p0, p1, p2;
	mlib_d64 off0, off1, off2;

/* keep kernel in regs */
	k0 = fmat[0];
	k1 = fmat[1];
	k2 = fmat[2];
	k3 = fmat[3];
	k4 = fmat[4];
	k5 = fmat[5];
	k6 = fmat[6];
	k7 = fmat[7];
	k8 = fmat[8];

	off0 = offset[0];
	off1 = offset[1];
	off2 = offset[2];

	sa = sl = src;
	da = dl = dst;

	for (j = 0; j < ysize; j++) {

		for (i = 0; i < 3 * xsize; i += 3) {
			p0 = sa[i];
			p1 = sa[i + 1];
			p2 = sa[i + 2];
			sum1 = p0 * k0 + p1 * k1 + p2 * k2 + off0;
			sum2 = p0 * k3 + p1 * k4 + p2 * k5 + off1;
			sum3 = p0 * k6 + p1 * k7 + p2 * k8 + off2;

			SAT32(da, sum1)
			    SAT32(da, sum2)
			    SAT32(da, sum3)
		}

/* set src ptrs to next row */
		sl = sa = sl + slb;
/* set dst ptrs to next row */
		dl = da = dl + dlb;
	}

	return (MLIB_SUCCESS);
}

#endif	/* defined(_LP64) && !defined(_NO_LONGLONG) */

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
	mlib_s32 scale;
	mlib_d64 offset0[3] = { 0, 0, 0 };

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

	if (dtype == MLIB_BYTE) {
		mlib_s_ImageColor1_Mat_U8(fmat, &scale);

		if (scale > 7)
			return (MLIB_FAILURE);

		return mlib_s_ImageColorConvert1_U8(sa, slb,
		    da, dlb, xsize, ysize, fmat, scale);
	} else if (dtype == MLIB_SHORT) {
		mlib_s_ImageColor1_Mat_S16(fmat, &scale);

		if (scale > 15)
			return (MLIB_FAILURE);

		return mlib_s_ImageColorConvert1_S16(sa, slb / 2,
		    da, dlb / 2, xsize, ysize, fmat, scale);
	} else if (dtype == MLIB_USHORT) {

		offset0[0] =
		    32768. * (fmat[0] + fmat[1] + fmat[2]) - 32768.;
		offset0[1] =
		    32768. * (fmat[3] + fmat[4] + fmat[5]) - 32768.;
		offset0[2] =
		    32768. * (fmat[6] + fmat[7] + fmat[8]) - 32768.;

		mlib_s_ImageColor2_Mat_S16(fmat, offset0, &scale);

		if (scale > 15)
			return (MLIB_FAILURE);

		return mlib_s_ImageColorConvert2_U16(sa, slb / 2,
		    da, dlb / 2, xsize, ysize, fmat, offset0, scale);
	} else if (dtype == MLIB_INT) {
		return mlib_s_ImageColorConvert1_S32(sa, slb / 4,
		    da, dlb / 4, xsize, ysize, fmat);
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
	mlib_s32 scale;
	mlib_d64 offset0[3] = { 0, 0, 0 };

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

	if (dtype == MLIB_BYTE) {
		mlib_s_ImageColor2_Mat_U8(fmat, offset, &scale);

		if (scale > 7)
			return (MLIB_FAILURE);

		return mlib_s_ImageColorConvert2_U8(sa, slb,
		    da, dlb, xsize, ysize, fmat, offset, scale);
	} else if (dtype == MLIB_SHORT) {
		mlib_s_ImageColor2_Mat_S16(fmat, offset, &scale);

		if (scale > 15)
			return (MLIB_FAILURE);

		return mlib_s_ImageColorConvert2_S16(sa, slb / 2,
		    da, dlb / 2, xsize, ysize, fmat, offset, scale);
	} else if (dtype == MLIB_USHORT) {

		offset0[0] =
		    offset[0] + 32768. * (fmat[0] + fmat[1] + fmat[2]) - 32768.;
		offset0[1] =
		    offset[1] + 32768. * (fmat[3] + fmat[4] + fmat[5]) - 32768.;
		offset0[2] =
		    offset[2] + 32768. * (fmat[6] + fmat[7] + fmat[8]) - 32768.;

		mlib_s_ImageColor2_Mat_S16(fmat, offset0, &scale);

		if (scale > 15)
			return (MLIB_FAILURE);

		return mlib_s_ImageColorConvert2_U16(sa, slb / 2,
		    da, dlb / 2, xsize, ysize, fmat, offset0, scale);
	} else if (dtype == MLIB_INT) {
		return mlib_s_ImageColorConvert2_S32(sa, slb / 4,
		    da, dlb / 4, xsize, ysize, fmat, offset);
	} else
		return (MLIB_FAILURE);
}

/* *********************************************************** */
