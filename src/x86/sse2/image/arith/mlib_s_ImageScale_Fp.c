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
 * Copyright 2006 Sun Microsystems, Inc.  All rights reserved.
 * Use is subject to license terms.
 */

#pragma ident "@(#)mlib_s_ImageScale_Fp.c	9.2	07/11/05 SMI"

/*
 * FUNCTION
 *	mlib_ImageScale_Fp	- linear scaling
 *	mlib_ImageScale_Fp_Inp	- linear scaling
 *
 * SYNOPSIS
 *	mlib_status mlib_ImageScale_Fp(mlib_image       *dst,
 *					const mlib_image *src,
 *					const mlib_d64   *alpha,
 *					const mlib_d64   *beta);
 *	mlib_status mlib_ImageScale_Fp_Inp(mlib_image    *srcdst,
 *					const mlib_d64   *alpha,
 *					const mlib_d64   *beta);
 *
 * ARGUMENT
 *	dst     pointer to the output image
 *	src     pointer to the input image
 *	srcdst  pointer to the input and the output image
 *	alpha   array of scaling factors to be multiplied
 *		to each channel of the pixels
 *	beta    array of offsets to be added to each channel
 *		of the pixels
 *
 * RESTRICTION
 *	src and dst must have the same type, the same size,
 *	and the same number of channels.
 *	The images can have 1, 2, 3, or 4 channels.
 *	The input image can be in MLIB_FLOAT or MLIB_DOUBLE data type.
 *      The output image can be in MLIB_BYTE, MLIB_SHORT, MLIB_USHORT,
 *	MLIB_INT, MLIB_FLOAT or MLIB_DOUBLE data type.
 *
 * DESCRIPTION
 *	dst[x][y][c] = src[x][y][c] * alpha[c] + beta[c]
 *	srcdst[x][y][c] = srcdst[x][y][c] * alpha[c] + beta[c]
 *
 *	If the result of the operation underflows/overflows the
 *	minimum/maximum value supported by the destination image, then
 *	it will be clamped to the minimum/maximum value respectively.
 */

#ifdef __SUNPRO_C
#include <sunmedia_intrin.h>
#else
#include <emmintrin.h>
#endif

#include <mlib_image.h>
#include <mlib_ImageCheck.h>
#include "mlib_s_ImageScale_Fp.h"

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageScale_Fp = __mlib_ImageScale_Fp
#pragma weak mlib_ImageScale_Fp_Inp = __mlib_ImageScale_Fp_Inp

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */

__typeof__(__mlib_ImageScale_Fp) mlib_ImageScale_Fp
	__attribute__((weak, alias("__mlib_ImageScale_Fp")));
__typeof__(__mlib_ImageScale_Fp_Inp) mlib_ImageScale_Fp_Inp
	__attribute__((weak, alias("__mlib_ImageScale_Fp_Inp")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	MLIB_C_IMAGESCALE(DTYPE, STYPE, DMAX, DMIN)		\
{								\
	int x, y, c;						\
								\
	for (y = 0; y < ysize; y ++) {				\
		for (x = 0; x < xsize; x ++) {			\
			for (c = 0; c < nchan; c ++) {		\
				STYPE m;			\
				DTYPE n;			\
				m = ((STYPE *)sa)[nchan*x+c]	\
					* (STYPE)alpha[c]	\
					+ (STYPE)beta[c];	\
				if (m > DMAX) {			\
					n = DMAX;		\
				} else				\
				if (m < DMIN) {			\
					n = DMIN;		\
				} else {			\
					n = (DTYPE)m;		\
				}				\
				((DTYPE *)da)[nchan*x+c] = n;	\
			}					\
		}						\
		sa  = (STYPE *)((mlib_u8 *)sa + slb);		\
		da  = (DTYPE *)((mlib_u8 *)da + dlb);		\
	}							\
}

/* *********************************************************** */

mlib_status
__mlib_ImageScale_Fp(
	mlib_image *dst,
	const mlib_image *src,
	const mlib_d64   *alpha,
	const mlib_d64   *beta)
{
	mlib_type stype, dtype;
	mlib_s32 slb, dlb, xsize, ysize, nchan;
	void *sa, *da;

	MLIB_IMAGE_CHECK(dst);
	MLIB_IMAGE_CHECK(src);
	MLIB_IMAGE_SIZE_EQUAL(dst, src);
	MLIB_IMAGE_CHAN_EQUAL(dst, src);

	stype = mlib_ImageGetType(src);
	dtype = mlib_ImageGetType(dst);
	nchan = mlib_ImageGetChannels(dst);
	xsize = mlib_ImageGetWidth(dst);
	ysize = mlib_ImageGetHeight(dst);
	slb = mlib_ImageGetStride(src);
	dlb = mlib_ImageGetStride(dst);
	sa = mlib_ImageGetData(src);
	da = mlib_ImageGetData(dst);

	if (stype == MLIB_FLOAT) {
		if (dtype == MLIB_BYTE) {
			mlib_s_ImageScale_Fp_u8_f32(da, dlb, sa, slb,
					xsize, ysize, nchan, alpha, beta);
		} else
		if (dtype == MLIB_SHORT) {
			mlib_s_ImageScale_Fp_s16_f32(da, dlb, sa, slb,
					xsize, ysize, nchan, alpha, beta);
		} else
		if (dtype == MLIB_USHORT) {
			mlib_s_ImageScale_Fp_u16_f32(da, dlb, sa, slb,
					xsize, ysize, nchan, alpha, beta);
		} else
		if (dtype == MLIB_INT) {
			mlib_s_ImageScale_Fp_s32_f32(da, dlb, sa, slb,
					xsize, ysize, nchan, alpha, beta);
		} else
		if (dtype == MLIB_FLOAT) {
			mlib_s_ImageScale_Fp_f32_f32(da, dlb, sa, slb,
					xsize, ysize, nchan, alpha, beta);
		} else
		if (dtype == MLIB_DOUBLE) {
			mlib_s_ImageScale_Fp_d64_f32(da, dlb, sa, slb,
					xsize, ysize, nchan, alpha, beta);
		} else {
			return (MLIB_FAILURE);
		}
		return (MLIB_SUCCESS);
	} else
	if (stype == MLIB_DOUBLE) {
		if (dtype == MLIB_BYTE) {
			mlib_s_ImageScale_Fp_u8_d64(da, dlb, sa, slb,
					xsize, ysize, nchan, alpha, beta);
		} else
		if (dtype == MLIB_SHORT) {
			mlib_s_ImageScale_Fp_s16_d64(da, dlb, sa, slb,
					xsize, ysize, nchan, alpha, beta);
		} else
		if (dtype == MLIB_USHORT) {
			mlib_s_ImageScale_Fp_u16_d64(da, dlb, sa, slb,
					xsize, ysize, nchan, alpha, beta);
		} else
		if (dtype == MLIB_INT) {
			mlib_s_ImageScale_Fp_s32_d64(da, dlb, sa, slb,
					xsize, ysize, nchan, alpha, beta);
		} else
		if (dtype == MLIB_FLOAT) {
			mlib_s_ImageScale_Fp_f32_d64(da, dlb, sa, slb,
					xsize, ysize, nchan, alpha, beta);
		} else
		if (dtype == MLIB_DOUBLE) {
			mlib_s_ImageScale_Fp_d64_d64(da, dlb, sa, slb,
					xsize, ysize, nchan, alpha, beta);
		} else {
			return (MLIB_FAILURE);
		}
		return (MLIB_SUCCESS);
	}

	return (MLIB_FAILURE);
}

/* *********************************************************** */

mlib_status
__mlib_ImageScale_Fp_Inp(
	mlib_image *srcdst,
	const mlib_d64 *alpha,
	const mlib_d64 *beta)
{
	return (__mlib_ImageScale_Fp(srcdst, srcdst, alpha, beta));
}

/* *********************************************************** */
