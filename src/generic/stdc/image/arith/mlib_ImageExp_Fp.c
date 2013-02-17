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

#pragma ident	"@(#)mlib_ImageExp_Fp.c	9.2	07/10/09 SMI"

/*
 * FUNCTION
 *      mlib_ImageExp_Fp        - compute the exponential of the pixel values
 *      mlib_ImageExp_Fp_Inp
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageExp_Fp(mlib_image       *dst,
 *                                   const mlib_image *src)
 *      mlib_status mlib_ImageExp_Fp_Inp(mlib_image *srcdst)
 *
 * ARGUMENT
 *      src     pointer to the input image
 *      dst     pointer to the output image
 *      srcdst  pointer to the input and the output image
 *
 * RESTRICTION
 *      The images must have the same type, the same size, and the same number
 *      of channels. The images can have 1, 2, 3, or 4 channels.
 *      The images can be in MLIB_FLOAT and MLIB_DOUBLE data type.
 *
 * DESCRIPTION
 *      For the general version:
 *              dst[x][y][c] = e^src[x][y][c]
 *
 *      For in-place processing version (*_Inp):
 *              srcdst[x][y][c] = e^srcdst[x][y][c]
 *
 *      where, e = 2.718...
 */

#include <mlib_image.h>
#include <mlib_ImageCheck.h>
#include <mlib_ImageExpTbl.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageExp_Fp = __mlib_ImageExp_Fp
#pragma weak mlib_ImageExp_Fp_Inp = __mlib_ImageExp_Fp_Inp

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_ImageExp_Fp) mlib_ImageExp_Fp
    __attribute__((weak, alias("__mlib_ImageExp_Fp")));
__typeof__(__mlib_ImageExp_Fp_Inp) mlib_ImageExp_Fp_Inp
    __attribute__((weak, alias("__mlib_ImageExp_Fp_Inp")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	BUF_SIZE	256

/* *********************************************************** */

#define	K256ONLN2	369.3299304675746271

/* *********************************************************** */

#define	KLN2ON256_hi	2.707606173771637259e-03
#define	KLN2ON256_lo	2.906491058598592519e-13

/* *********************************************************** */

#define	KA5	8.33333478792298551112e-03
#define	KA4	4.16666768487944911020e-02
#define	KA3	1.66666666666665333693e-01
#define	KA2	4.99999999999990669185e-01

/* *********************************************************** */

#define	KF2	3.66556671660783833261e-06
#define	KF1	2.70760782821392980564e-03

/* *********************************************************** */

static void
mlib_Pow1_F32(
    mlib_f32 *dp,
    mlib_f32 *sp,
    mlib_s32 *buf,
    mlib_s32 xsize)
{
	mlib_s32 i;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < xsize; i++) {
		mlib_d64 x, y, z;
		mlib_s32 k, ind;

		x = (mlib_d64)sp[i];
		y = K256ONLN2 * x;

#ifdef MLIB_USE_FTOI_CLAMPING

		buf[i] = (mlib_s32)y;

#else /* MLIB_USE_FTOI_CLAMPING */

#ifdef i386

/* Inf or NaN */
		if ((((mlib_s32 *)&y)[1] & 0x7ff00000) == 0x7ff00000) {
/* MLIB_S32_MAX or MIN */
			buf[i] = (((mlib_s32 *)&y)[1] >> 31) ^ MLIB_S32_MAX;
		} else {
			if (y >= MLIB_S32_MAX)
				buf[i] = MLIB_S32_MAX;
			else if (y <= MLIB_S32_MIN)
				buf[i] = MLIB_S32_MIN;
			else
				buf[i] = (mlib_s32)y;
		}

#else /* i386 */

		if (y >= MLIB_S32_MAX)
			y = MLIB_S32_MAX;

		if (y <= MLIB_S32_MIN)
			y = MLIB_S32_MIN;
		buf[i] = (mlib_s32)y;
#endif /* i386 */
#endif /* MLIB_USE_FTOI_CLAMPING */

		k = buf[i];
		ind = k & 255;
		y = y - (mlib_d64)k;
		z = 1.0 + y * (KF1 + KF2 * y);
		dp[i] = (mlib_f32)(mlib_constexp_hi[ind] * z);
	}
}

/* *********************************************************** */

static void
mlib_Pow1_D64(
    mlib_d64 *dp,
    mlib_d64 *sp,
    mlib_s32 *buf,
    mlib_s32 xsize)
{
	mlib_s32 i;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < xsize; i++) {
		mlib_d64 x, s, y, dbk, hi;
		mlib_s32 k, ind;

		x = sp[i];
		s = K256ONLN2 * x;

#ifdef MLIB_USE_FTOI_CLAMPING

		buf[i] = (mlib_s32)s;

#else /* MLIB_USE_FTOI_CLAMPING */

#ifdef i386

/* Inf or NaN */
		if ((((mlib_s32 *)&s)[1] & 0x7ff00000) == 0x7ff00000) {
/* MLIB_S32_MAX or MIN */
			buf[i] = (((mlib_s32 *)&s)[1] >> 31) ^ MLIB_S32_MAX;
		} else {
			if (s >= MLIB_S32_MAX)
				buf[i] = MLIB_S32_MAX;
			else if (s <= MLIB_S32_MIN)
				buf[i] = MLIB_S32_MIN;
			else
				buf[i] = (mlib_s32)s;
		}

#else /* i386 */

		if (s >= MLIB_S32_MAX)
			s = MLIB_S32_MAX;

		if (s <= MLIB_S32_MIN)
			s = MLIB_S32_MIN;
		buf[i] = (mlib_s32)s;
#endif /* i386 */
#endif /* MLIB_USE_FTOI_CLAMPING */

		k = buf[i];
		dbk = (mlib_d64)buf[i];

/*  exp(x) = exp(y) * 2**(dbk/256) */
		y = (x - dbk * KLN2ON256_hi) - dbk * KLN2ON256_lo;
/*  approximation of (exp(y)-1) */
		y = y + y * y * (KA2 + y * (KA3 + y * (KA4 + y * KA5)));
/*  (ind/256) is fraction part of (dbk/256)  */
		ind = k & 0xFF;
/*  2**(ind/256) value */
		hi = mlib_constexp_hi[ind];
/*  ~= 2**(ind/256) * exp(y) */
		dp[i] = hi + (hi * y + mlib_constexp_lo[ind]);
	}
}

/* *********************************************************** */

static void
mlib_ScaleExp_F32(
    mlib_s32 *dp,
    mlib_s32 *exp_scale,
    mlib_s32 xsize)
{
	mlib_s32 i;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < xsize; i++) {
		mlib_s32 s_v, off, e, nan_mask, of_mask, uf_mask, mask;

		s_v = dp[i];
		nan_mask = (0x7f800000 - (s_v & 0x7fffffff)) >> 31;
		off = exp_scale[i] >> 8;
		e = (s_v >> 23) + off;
		of_mask = (0xfe - e) >> 31;
		uf_mask = (e - 1) >> 31;
		mask = (of_mask | uf_mask) & ~nan_mask;
		s_v = ((s_v + (off << 23)) & ~mask) | (of_mask & 0x7f800000);
		dp[i] = s_v;
	}
}

/* *********************************************************** */

static void
mlib_ScaleExp_D64(
    mlib_s32 *dp,
    mlib_s32 *exp_scale,
    mlib_s32 xsize)
{
	mlib_s32 i;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < xsize; i++) {
		mlib_s32 s_v, off, e, nan_mask, of_mask, uf_mask, mask;

#ifdef _LITTLE_ENDIAN
		s_v = dp[2 * i + 1];
#else /* _LITTLE_ENDIAN */
		s_v = dp[2 * i];
#endif /* _LITTLE_ENDIAN */
		nan_mask = (0x7ff00000 - (s_v & 0x7fffffff)) >> 31;
		off = exp_scale[i] >> 8;
		e = (s_v >> 20) + off;
		of_mask = (0x7fe - e) >> 31;
		uf_mask = (e - 1) >> 31;
		mask = (of_mask | uf_mask) & ~nan_mask;
		s_v = ((s_v + (off << 20)) & ~mask) | (of_mask & 0x7ff00000);
#ifdef _LITTLE_ENDIAN
		dp[2 * i + 1] = s_v;
		dp[2 * i] = dp[2 * i] & ~mask;
#else /* _LITTLE_ENDIAN */
		dp[2 * i] = s_v;
		dp[2 * i + 1] = dp[2 * i + 1] & ~mask;
#endif /* _LITTLE_ENDIAN */
	}
}

/* *********************************************************** */

mlib_status
__mlib_ImageExp_Fp(
    mlib_image *dst,
    const mlib_image *src)
{
	mlib_type type;
	mlib_s32 nchan, xsize, ysize, dsize, dlb, slb;
	mlib_s32 buf[BUF_SIZE + 2];
	mlib_u8 *pdst, *psrc;
	mlib_s32 j;

	MLIB_IMAGE_CHECK(dst);
	MLIB_IMAGE_CHECK(src);
	MLIB_IMAGE_FULL_EQUAL(dst, src);

	MLIB_IMAGE_GET_ALL_PARAMS(dst, type, nchan, xsize, ysize, dlb, pdst);
	slb = mlib_ImageGetStride(src);
	psrc = mlib_ImageGetData(src);
	xsize *= nchan;

	if (type == MLIB_FLOAT)
		dsize = xsize * 4;
	else if (type == MLIB_DOUBLE)
		dsize = xsize * 8;
	else
		return (MLIB_FAILURE);

	if ((dsize == slb) && (dsize == dlb)) {
		xsize *= ysize;
		ysize = 1;
	}

	if (type == MLIB_FLOAT) {

		for (j = 0; j < ysize; j++) {
			mlib_s32 off, size;

			for (off = 0; off < xsize; off += size) {
				mlib_f32 *sp = (mlib_f32 *)psrc + off;
				mlib_f32 *dp = (mlib_f32 *)pdst + off;

				size = xsize - off;

				if (size > BUF_SIZE)
					size = BUF_SIZE;

				mlib_Pow1_F32(dp, sp, buf, size);
				mlib_ScaleExp_F32((mlib_s32 *)dp, buf, size);
			}

			psrc += slb;
			pdst += dlb;
		}

		return (MLIB_SUCCESS);
	}

	if (type == MLIB_DOUBLE) {

		for (j = 0; j < ysize; j++) {
			mlib_s32 off, size;

			for (off = 0; off < xsize; off += size) {
				mlib_d64 *sp = (mlib_d64 *)psrc + off;
				mlib_d64 *dp = (mlib_d64 *)pdst + off;

				size = xsize - off;

				if (size > BUF_SIZE)
					size = BUF_SIZE;

				mlib_Pow1_D64(dp, sp, buf, size);
				mlib_ScaleExp_D64((mlib_s32 *)dp, buf, size);
			}

			psrc += slb;
			pdst += dlb;
		}

		return (MLIB_SUCCESS);
	}

	return (MLIB_FAILURE);
}

/* *********************************************************** */

mlib_status
__mlib_ImageExp_Fp_Inp(
    mlib_image *srcdst)
{
	return (__mlib_ImageExp_Fp(srcdst, srcdst));
}

/* *********************************************************** */
