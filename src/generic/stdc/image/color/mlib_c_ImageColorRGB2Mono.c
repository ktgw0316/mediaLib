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

#pragma ident	"@(#)mlib_c_ImageColorRGB2Mono.c	9.2	07/10/09 SMI"

/*
 * FUNCTION
 *      mlib_ImageColorRGB2Mono - color to mono conversion
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageColorRGB2Mono(mlib_image       *dst,
 *                                          const mlib_image *src,
 *                                          const mlib_d64   *weight)
 *
 *      mlib_status mlib_ImageColorRGB2CIEMono(mlib_image       *dst,
 *                                             const mlib_image *src)
 *
 * ARGUMENT
 *      dst     pointer to output image
 *      src     pointer to input image
 *      weight  array of 3 blending coefficients
 *
 * RESTRICTION
 *      src and dst must be the same type, and the same size.
 *      The input must be a 3-channel image,
 *      and the output image must be a single channel image.
 *      They can be in MLIB_BYTE, MLIB_SHORT, MLIB_USHORT or MLIB_INT data type.
 *
 * DESCRIPTION
 *      mlib_ImageColorRGB2Mono
 *        Z =  weight[0] * R +  weight[1] * G +  weight[2] * B
 *      mlib_ImageColorRGB2CIEMono
 *        Z =  0.2125 * R +  0.7154 * G +  0.0721 * B
 *
 *     It is required that all blending coefficients(weight) be no more
 *     than 1.0
 */

#include <mlib_image.h>
#include <mlib_ImageCheck.h>
#include <mlib_SysMath.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageColorRGB2CIEMono = __mlib_ImageColorRGB2CIEMono
#pragma weak mlib_ImageColorRGB2Mono = __mlib_ImageColorRGB2Mono

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_ImageColorRGB2CIEMono) mlib_ImageColorRGB2CIEMono
    __attribute__((weak, alias("__mlib_ImageColorRGB2CIEMono")));
__typeof__(__mlib_ImageColorRGB2Mono) mlib_ImageColorRGB2Mono
    __attribute__((weak, alias("__mlib_ImageColorRGB2Mono")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#ifdef MLIB_USE_FTOI_CLAMPING

#define	SAT32(DST, SRC)	(*DST++) = (mlib_s32) SRC

#else /* MLIB_USE_FTOI_CLAMPING */

#define	SAT32(DST, SRC)                                        \
	if (SRC >= MLIB_S32_MAX)                               \
	    (*DST++) = MLIB_S32_MAX;                           \
	else                                                   \
	    if (SRC <= MLIB_S32_MIN)                           \
		(*DST++) = MLIB_S32_MIN;                       \
	    else                                               \
		(*DST++) = (mlib_s32)SRC

#endif /* MLIB_USE_FTOI_CLAMPING */

/* *********************************************************** */

static mlib_status mlib_c_ImageColorRGB2Mono_U8(
    const mlib_u8 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_d64 *weight);

static mlib_status mlib_c_ImageColorRGB2Mono_S16(
    const mlib_s16 *src,
    mlib_s32 slb,
    mlib_s16 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_d64 *weight);

static mlib_status mlib_c_ImageColorRGB2Mono_U16(
    const mlib_u16 *src,
    mlib_s32 slb,
    mlib_u16 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_d64 *weight);

static mlib_status mlib_c_ImageColorRGB2Mono_S32(
    const mlib_s32 *src,
    mlib_s32 slb,
    mlib_s32 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_d64 *weight);

/* *********************************************************** */

mlib_status
mlib_c_ImageColorRGB2Mono_U8(
    const mlib_u8 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_d64 *weight)
{
	mlib_u8 *sa, *sl;
	mlib_u8 *da, *dl;
	mlib_d64 sf0, sf1, sf2, df1, w[3];
	mlib_d64 sf3, sf4, sf5, df2;
	mlib_d64 sf6, sf7, sf8, df3;
	mlib_d64 sat;
	mlib_s32 i, j, isum;
	mlib_s32 tmp = 0x7FFFFFFF;
	mlib_s32 *outbuf, *pout;
	mlib_s32 num_blocks, leftover_cols;

	w[0] = weight[0] * 16777216.;
	w[1] = weight[1] * 16777216.;
	w[2] = weight[2] * 16777216.;

	sl = sa = (void *)src;
	dl = da = dst;
	sat = (mlib_d64)tmp + 1.;
	outbuf = (mlib_s32 *)__mlib_malloc(xsize * sizeof (mlib_s32));

	if (outbuf == NULL)
		return (MLIB_FAILURE);
	pout = outbuf;

	num_blocks = xsize / 3;
	leftover_cols = xsize - num_blocks * 3;

	for (j = 0; j < ysize; j++) {
		for (i = 0; i < num_blocks; i++) {
			sf0 = (*sa++);
			sf1 = (*sa++);
			sf2 = (*sa++);
			sf3 = (*sa++);
			sf4 = (*sa++);
			sf5 = (*sa++);
			sf6 = (*sa++);
			sf7 = (*sa++);
			sf8 = (*sa++);

			df1 = w[0] * sf0 + w[1] * sf1 + w[2] * sf2;
			df2 = w[0] * sf3 + w[1] * sf4 + w[2] * sf5;
			df3 = w[0] * sf6 + w[1] * sf7 + w[2] * sf8;

			df1 -= sat;
			SAT32(pout, df1);
			df2 -= sat;
			SAT32(pout, df2);
			df3 -= sat;
			SAT32(pout, df3);
		}

		if (leftover_cols) {
			sf0 = (*sa++);
			sf1 = (*sa++);
			sf2 = (*sa++);
			df1 = w[0] * sf0 + w[1] * sf1 + w[2] * sf2;
			df1 -= sat;
			SAT32(pout, df1);

			if (leftover_cols > 1) {
				sf0 = (*sa++);
				sf1 = (*sa++);
				sf2 = (*sa++);
				df1 = w[0] * sf0 + w[1] * sf1 + w[2] * sf2;
				df1 -= sat;
				SAT32(pout, df1);
			}
		}

		pout = outbuf;
		for (i = 0; i < xsize; i++) {
			isum = (*pout++);
			(*da++) = ((isum >> 24) ^ 0x80);
		}

		sl = sa = sl + slb;
		dl = da = dl + dlb;
		pout = outbuf;
	}

	__mlib_free(outbuf);
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_c_ImageColorRGB2Mono_S16(
    const mlib_s16 *src,
    mlib_s32 slb,
    mlib_s16 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_d64 *weight)
{
	mlib_s16 *sa, *sl;
	mlib_s16 *da, *dl;
	mlib_d64 sf0, sf1, sf2, df1, w[3];
	mlib_d64 sf3, sf4, sf5, df2;
	mlib_d64 sf6, sf7, sf8, df3;
	mlib_s32 i, j, isum;
	mlib_s32 *outbuf, *pout;
	mlib_s32 num_blocks, leftover_cols;

	w[0] = weight[0] * 65536.;
	w[1] = weight[1] * 65536.;
	w[2] = weight[2] * 65536.;

	sl = sa = (void *)src;
	dl = da = dst;
	outbuf = (mlib_s32 *)__mlib_malloc(xsize * sizeof (mlib_s32));

	if (outbuf == NULL)
		return (MLIB_FAILURE);
	pout = outbuf;

	num_blocks = xsize / 3;
	leftover_cols = xsize - num_blocks * 3;

	for (j = 0; j < ysize; j++) {
		for (i = 0; i < num_blocks; i++) {
			sf0 = (*sa++);
			sf1 = (*sa++);
			sf2 = (*sa++);
			sf3 = (*sa++);
			sf4 = (*sa++);
			sf5 = (*sa++);
			sf6 = (*sa++);
			sf7 = (*sa++);
			sf8 = (*sa++);

			df1 = w[0] * sf0 + w[1] * sf1 + w[2] * sf2;
			df2 = w[0] * sf3 + w[1] * sf4 + w[2] * sf5;
			df3 = w[0] * sf6 + w[1] * sf7 + w[2] * sf8;

			SAT32(pout, df1);
			SAT32(pout, df2);
			SAT32(pout, df3);
		}

		if (leftover_cols) {
			sf0 = (*sa++);
			sf1 = (*sa++);
			sf2 = (*sa++);
			df1 = w[0] * sf0 + w[1] * sf1 + w[2] * sf2;
			SAT32(pout, df1);

			if (leftover_cols > 1) {
				sf0 = (*sa++);
				sf1 = (*sa++);
				sf2 = (*sa++);
				df1 = w[0] * sf0 + w[1] * sf1 + w[2] * sf2;
				SAT32(pout, df1);
			}
		}

		pout = outbuf;
		for (i = 0; i < xsize; i++) {
			isum = (*pout++);
			(*da++) = (isum >> 16);
		}

		sl = sa = sl + slb;
		dl = da = dl + dlb;
		pout = outbuf;
	}

	__mlib_free(outbuf);
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_c_ImageColorRGB2Mono_U16(
    const mlib_u16 *src,
    mlib_s32 slb,
    mlib_u16 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_d64 *weight)
{
	mlib_u16 *sa, *sl;
	mlib_u16 *da, *dl;
	mlib_d64 sf0, sf1, sf2, df1, w[3];
	mlib_d64 sf3, sf4, sf5, df2;
	mlib_d64 sf6, sf7, sf8, df3;
	mlib_d64 sat;
	mlib_s32 i, j, isum;
	mlib_s32 tmp = 0x7FFFFFFF;
	mlib_s32 *outbuf, *pout;
	mlib_s32 num_blocks, leftover_cols;

	w[0] = weight[0] * 65536.;
	w[1] = weight[1] * 65536.;
	w[2] = weight[2] * 65536.;

	sl = sa = (void *)src;
	dl = da = dst;
	sat = (mlib_d64)tmp + 1.;
	outbuf = (mlib_s32 *)__mlib_malloc(xsize * sizeof (mlib_s32));

	if (outbuf == NULL)
		return (MLIB_FAILURE);
	pout = outbuf;

	num_blocks = xsize / 3;
	leftover_cols = xsize - num_blocks * 3;

	for (j = 0; j < ysize; j++) {
		for (i = 0; i < num_blocks; i++) {
			sf0 = (*sa++);
			sf1 = (*sa++);
			sf2 = (*sa++);
			sf3 = (*sa++);
			sf4 = (*sa++);
			sf5 = (*sa++);
			sf6 = (*sa++);
			sf7 = (*sa++);
			sf8 = (*sa++);

			df1 = w[0] * sf0 + w[1] * sf1 + w[2] * sf2;
			df2 = w[0] * sf3 + w[1] * sf4 + w[2] * sf5;
			df3 = w[0] * sf6 + w[1] * sf7 + w[2] * sf8;

			df1 -= sat;
			SAT32(pout, df1);
			df2 -= sat;
			SAT32(pout, df2);
			df3 -= sat;
			SAT32(pout, df3);
		}

		if (leftover_cols) {
			sf0 = (*sa++);
			sf1 = (*sa++);
			sf2 = (*sa++);
			df1 = w[0] * sf0 + w[1] * sf1 + w[2] * sf2;
			df1 -= sat;
			SAT32(pout, df1);

			if (leftover_cols > 1) {
				sf0 = (*sa++);
				sf1 = (*sa++);
				sf2 = (*sa++);
				df1 = w[0] * sf0 + w[1] * sf1 + w[2] * sf2;
				df1 -= sat;
				SAT32(pout, df1);
			}
		}

		pout = outbuf;
		for (i = 0; i < xsize; i++) {
			isum = (*pout++);
			(*da++) = ((isum >> 16) ^ 0x8000);
		}

		sl = sa = sl + slb;
		dl = da = dl + dlb;
		pout = outbuf;
	}

	__mlib_free(outbuf);
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_c_ImageColorRGB2Mono_S32(
    const mlib_s32 *src,
    mlib_s32 slb,
    mlib_s32 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_d64 *weight)
{
	mlib_s32 *sa, *sl;
	mlib_s32 *da, *dl;
	mlib_d64 sf0, sf1, sf2, df1, w[3];
	mlib_d64 sf3, sf4, sf5, df2;
	mlib_d64 sf6, sf7, sf8, df3;
	mlib_s32 i, j;
	mlib_s32 num_blocks, leftover_cols;

	w[0] = weight[0];
	w[1] = weight[1];
	w[2] = weight[2];

	sl = sa = (void *)src;
	dl = da = dst;

	num_blocks = xsize / 3;
	leftover_cols = xsize - num_blocks * 3;

	for (j = 0; j < ysize; j++) {
		for (i = 0; i < num_blocks; i++) {
			sf0 = (*sa++);
			sf1 = (*sa++);
			sf2 = (*sa++);
			sf3 = (*sa++);
			sf4 = (*sa++);
			sf5 = (*sa++);
			sf6 = (*sa++);
			sf7 = (*sa++);
			sf8 = (*sa++);

			df1 = w[0] * sf0 + w[1] * sf1 + w[2] * sf2;
			df2 = w[0] * sf3 + w[1] * sf4 + w[2] * sf5;
			df3 = w[0] * sf6 + w[1] * sf7 + w[2] * sf8;

			SAT32(da, df1);
			SAT32(da, df2);
			SAT32(da, df3);
		}

		if (leftover_cols) {
			sf0 = (*sa++);
			sf1 = (*sa++);
			sf2 = (*sa++);

			df1 = w[0] * sf0 + w[1] * sf1 + w[2] * sf2;
			SAT32(da, df1);

			if (leftover_cols > 1) {
				sf0 = (*sa++);
				sf1 = (*sa++);
				sf2 = (*sa++);

				df1 = w[0] * sf0 + w[1] * sf1 + w[2] * sf2;
				SAT32(da, df1);
			}
		}

		sl = sa = sl + slb;
		dl = da = dl + dlb;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_ImageColorRGB2Mono(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_d64 *weight)
{
	mlib_s32 slb, dlb, xsize, ysize;
	mlib_type dtype;
	void *sa, *da;
	mlib_s32 i;

	MLIB_IMAGE_CHECK(src);
	MLIB_IMAGE_CHECK(dst);
	MLIB_IMAGE_TYPE_EQUAL(dst, src);
	MLIB_IMAGE_SIZE_EQUAL(dst, src);
	MLIB_IMAGE_HAVE_CHAN(src, 3);
	MLIB_IMAGE_HAVE_CHAN(dst, 1);

	for (i = 0; i < 3; i++)
		if (mlib_fabs(weight[i]) > 1.0)
			return (MLIB_OUTOFRANGE);

	dtype = mlib_ImageGetType(dst);
	xsize = mlib_ImageGetWidth(dst);
	ysize = mlib_ImageGetHeight(dst);
	slb = mlib_ImageGetStride(src);
	dlb = mlib_ImageGetStride(dst);
	sa = mlib_ImageGetData(src);
	da = mlib_ImageGetData(dst);

	if (dtype == MLIB_BYTE) {

		return mlib_c_ImageColorRGB2Mono_U8(sa, slb, da, dlb, xsize,
		    ysize, weight);
	} else if (dtype == MLIB_SHORT) {

		slb >>= 1;
		dlb >>= 1;
		return mlib_c_ImageColorRGB2Mono_S16(sa, slb, da, dlb, xsize,
		    ysize, weight);
	} else if (dtype == MLIB_USHORT) {

		slb >>= 1;
		dlb >>= 1;
		return mlib_c_ImageColorRGB2Mono_U16(sa, slb, da, dlb, xsize,
		    ysize, weight);
	} else if (dtype == MLIB_INT) {

		slb >>= 2;
		dlb >>= 2;
		return mlib_c_ImageColorRGB2Mono_S32(sa, slb, da, dlb, xsize,
		    ysize, weight);
	} else
		return (MLIB_FAILURE);
}

/* *********************************************************** */

mlib_status
__mlib_ImageColorRGB2CIEMono(
    mlib_image *dst,
    const mlib_image *src)
{
/* blending coefficients for CIE */
	mlib_d64 weight[3] = { 0.2125, 0.7154, 0.0721 };

	return (__mlib_ImageColorRGB2Mono(dst, src, weight));
}

/* *********************************************************** */
