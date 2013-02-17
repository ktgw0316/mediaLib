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

#pragma ident	"@(#)mlib_ImageLog_Fp.c	9.2	07/10/09 SMI"

/*
 * FUNCTION
 *      mlib_ImageLog_Fp    - compute the natural logarithm of the pixel values
 *      mlib_ImageLog_Fp_Inp
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageLog_Fp(mlib_image       *dst,
 *                                   const mlib_image *src)
 *      mlib_status mlib_ImageLog_Fp_Inp(mlib_image *srcdst)
 *
 * ARGUMENT
 *      src     pointer to the input image
 *      dst     pointer to the output image
 *      srcdst  pointer to the input and the output image
 *
 * RESTRICTION
 *      The images must have the same type, the same size, and the same number
 *      of channels. The images can have 1, 2, 3, or 4 channels.
 *      The images can be in in MLIB_FLOAT and MLIB_DOUBLE data type.
 *
 * DESCRIPTION
 *      For the general version:
 *              dst[x][y][c] = log(sub e)(src[x][y][c])
 *
 *      For in-place processing version (*_Inp):
 *              srcdst[x][y][c] = log(sub e)(srcdst[x][y][c])
 *
 *      where, e = 2.718...
 */

#include <mlib_image.h>
#include <mlib_ImageCheck.h>
#include <mlib_ImageLogTbl.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageLog_Fp = __mlib_ImageLog_Fp
#pragma weak mlib_ImageLog_Fp_Inp = __mlib_ImageLog_Fp_Inp

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_ImageLog_Fp) mlib_ImageLog_Fp
    __attribute__((weak, alias("__mlib_ImageLog_Fp")));
__typeof__(__mlib_ImageLog_Fp_Inp) mlib_ImageLog_Fp_Inp
    __attribute__((weak, alias("__mlib_ImageLog_Fp_Inp")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	BUF_SIZE	256

/* *********************************************************** */

#define	K2	4.00006488775094945494e-01
#define	K1	6.66666666629825839191e-01

/* *********************************************************** */

#define	KLN2	0.6931471805599453094
#define	KLN2_hi	0.6931471803691238165
#define	KLN2_lo	1.9082149292705877e-10

#define	TWO_P_52	4.503599627370496e15

/* *********************************************************** */

static void
mlib_SetZeroExp_D64(
    mlib_s32 *dp,
    mlib_s32 *sp,
    mlib_s32 *pexp,
    mlib_s32 xsize)
{
	mlib_s32 i;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < xsize; i++) {
		mlib_s32 s0, s1;

#ifdef _LITTLE_ENDIAN
		s0 = sp[2 * i + 1];
		s1 = sp[2 * i];
		dp[2 * i + 1] = (s0 & 0x000fffff) | 0x3ff00000;
		dp[2 * i] = s1;
#else /* _LITTLE_ENDIAN */
		s0 = sp[2 * i];
		s1 = sp[2 * i + 1];
		dp[2 * i] = (s0 & 0x000fffff) | 0x3ff00000;
		dp[2 * i + 1] = s1;
#endif /* _LITTLE_ENDIAN */
		pexp[i] = (s0 >> 20) - 0x3ff;
	}
}

/* *********************************************************** */

static void
mlib_Log1_D64(
    mlib_d64 *dp,
    mlib_d64 *sp,
    mlib_s32 *pexp,
    mlib_s32 xsize)
{
	mlib_s32 i;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < xsize; i++) {
		mlib_s32 ind;
		mlib_d64 dexp, y, y0, yd, yd2, c0;

		y = sp[i];
		dexp = (mlib_d64)pexp[i];

		ind = (mlib_s32)(y * 128);
		y0 = ind / 128.0;
		ind = ind & 0x7f;
		yd = (y - y0) / (y + y0);
		yd2 = yd * yd;
		c0 = mlib_constln[ind];
		dp[i] = yd + yd + dexp * KLN2_lo + mlib_constln_lo[ind] +
		    yd * yd2 * (K1 + K2 * yd2) + (c0 + dexp * KLN2_hi);
	}
}

/* *********************************************************** */

static void
mlib_LogSpecCases_D64(
    mlib_s32 *dp,
    mlib_s32 *buff,
    mlib_s32 *sp,
    mlib_s32 xsize)
{
	mlib_s32 i;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < xsize; i++) {
		mlib_s32 s0, s1, t0, t1, neg_mask, nan_mask;

		s0 = sp[2 * i];
		s1 = sp[2 * i + 1];
		t0 = buff[2 * i];
		t1 = buff[2 * i + 1];
#ifdef _LITTLE_ENDIAN
		neg_mask = (s1 | (s1 - 0x00100000)) >> 31;
		nan_mask = (0x7fe00000 - (s1 & 0x7ff00000)) >> 31;
		t1 = (t1 & ~neg_mask) | (neg_mask & 0xfff00000);
		t0 = (t0 & ~neg_mask);
		t1 = (t1 & ~nan_mask) | (s1 & nan_mask);
		t0 = (t0 & ~nan_mask) | (s0 & nan_mask);
#else /* _LITTLE_ENDIAN */
		nan_mask = (0x7fefffff - s0) >> 31;
		neg_mask = (s0 | (s0 - 0x00100000)) >> 31;
		t0 = (t0 & ~nan_mask) | (s0 & nan_mask);
		t1 = (t1 & ~nan_mask) | (s1 & nan_mask);
		t0 = (t0 & ~neg_mask) | (neg_mask & 0xfff00000);
		t1 = (t1 & ~neg_mask);
#endif /* _LITTLE_ENDIAN */
		dp[2 * i] = t0;
		dp[2 * i + 1] = t1;
	}
}

/* *********************************************************** */

static void
mlib_SetZeroExp_F32(
    mlib_d64 *dp,
    mlib_f32 *sp,
    mlib_s32 *pexp,
    mlib_s32 xsize)
{
	mlib_s32 i;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < xsize; i++) {
		mlib_s32 s0;

		dp[i] = (mlib_d64)sp[i];
#ifdef _LITTLE_ENDIAN
		s0 = ((mlib_s32 *)dp)[2 * i + 1];
		((mlib_s32 *)dp)[2 * i + 1] = (s0 & 0x000fffff) | 0x3ff00000;
#else /* _LITTLE_ENDIAN */
		s0 = ((mlib_s32 *)dp)[2 * i];
		((mlib_s32 *)dp)[2 * i] = (s0 & 0x000fffff) | 0x3ff00000;
#endif /* _LITTLE_ENDIAN */
		pexp[i] = ((s0 & 0x7ff00000) >> 20) - 0x3ff;
	}
}

/* *********************************************************** */

static void
mlib_Log1_F32(
    mlib_f32 *dp,
    mlib_d64 *sp,
    mlib_s32 *pexp,
    mlib_s32 xsize)
{
	mlib_s32 i;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < xsize; i++) {
		mlib_s32 ind;
		mlib_d64 y, y0, dexp, yd, yd2, c0;

		y = sp[i];
		dexp = pexp[i];

		ind = (mlib_s32)(y * 128);
		y0 = ind / 128.0;
		ind = ind & 0x7f;
		yd = (y - y0) / (y + y0);
		yd2 = yd * yd;
		c0 = mlib_constln[ind];

		dp[i] = (mlib_f32)(yd + yd + mlib_constln_lo[ind]
		    + yd * yd2 * (K1 + K2 * yd2) + (c0 + dexp * KLN2));
	}
}

/* *********************************************************** */

static void
mlib_LogSpecCases_F32(
    mlib_s32 *dp,
    mlib_s32 *buff,
    mlib_s32 *sp,
    mlib_s32 xsize)
{
	mlib_s32 i;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < xsize; i++) {
		mlib_s32 s, t, neg_mask, nan_mask;

		s = sp[i];
		t = buff[i];
		neg_mask = (s | (s - 0x00800000)) >> 31;
#if i386
		nan_mask = (0x7f000000 - (s & 0x7f800000)) >> 31;
		t = (t & ~neg_mask) | (neg_mask & 0xff800000);
		t = (t & ~nan_mask) | (s & nan_mask);
#else /* i386 */
		nan_mask = ((0x7f800000 - 1) - s) >> 31;
		t = (t & ~nan_mask) | (s & nan_mask);
		t = (t & ~neg_mask) | (neg_mask & 0xff800000);
#endif /* i386 */
		dp[i] = t;
	}
}

/* *********************************************************** */

mlib_status
__mlib_ImageLog_Fp(
    mlib_image *dst,
    const mlib_image *src)
{
	mlib_type type;
	mlib_s32 nchan, xsize, ysize, dsize, dlb, slb;
	mlib_s32 buf[BUF_SIZE + 2];
	mlib_d64 dbuf1[BUF_SIZE + 2], dbuf2[BUF_SIZE + 2];
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

				mlib_SetZeroExp_F32(dbuf1, sp, buf, size);
				mlib_Log1_F32((mlib_f32 *)dbuf2, dbuf1, buf,
				    size);
				mlib_LogSpecCases_F32((mlib_s32 *)dp,
				    (mlib_s32 *)dbuf2, (mlib_s32 *)sp, size);
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

				mlib_SetZeroExp_D64((mlib_s32 *)dbuf1,
				    (mlib_s32 *)sp, buf, size);
				mlib_Log1_D64(dbuf2, dbuf1, buf, size);
				mlib_LogSpecCases_D64((mlib_s32 *)dp,
				    (mlib_s32 *)dbuf2, (mlib_s32 *)sp, size);
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
__mlib_ImageLog_Fp_Inp(
    mlib_image *srcdst)
{
	return (__mlib_ImageLog_Fp(srcdst, srcdst));
}

/* *********************************************************** */
