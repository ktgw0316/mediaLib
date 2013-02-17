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

#pragma ident   "@(#)mlib_i_ImageScale.c	9.2	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_ImageScale - linear scaling
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageScale(mlib_image       *dst,
 *                                  const mlib_image *src,
 *                                  const mlib_s32   *alpha,
 *                                  const mlib_s32   *beta,
 *                                  mlib_s32         shift);
 *
 * ARGUMENT
 *      dst     pointer to the output image
 *      src     pointer to the input image
 *      alpha   array of scaling factors to be multiplied to each channel
 *              of the pixels
 *      beta    array of offsets to be added to each channel of the pixels
 *      shift   right shift factor
 *
 * VALID RANGES
 *   some designations:  maxA  - maximum of absolute values of alpha[i]
 *                       maxB  - maximum of absolute values of beta[i]
 *                       2^x   - two power x
 *                       STYPE - source image type
 *                       DTYPE - destination image type
 *                       SHIFT - shift argument
 *                       RESTRICTIONS - restrictions to
 *                       maxA, maxB and shift for given types and shift
 *
 *  STYPE  DTYPE        SHIFT                 RESTRICTIONS
 *   U8     U8        [ 0, 31]    maxA < 2^(shift+7)
 *                                2^8  * maxA * 2^(-shift) + maxB + 0.5 < 2^23
 *   U8     S16       [ 0,  0]    maxA < 2^(shift+14)
 *                                2^8  * maxA * 2^(-shift) + maxB + 0.5 < 2^30
 *   U8     S16       [ 1,  8]    maxA < 2^15
 *                                2^8  * maxA * 2^(-shift) + maxB + 0.5 <
 *                                2^(31-shift)
 *   U8     S16       [ 9, 31]    maxA < 2^(shift+7)
 *                                2^8  * maxA * 2^(-shift) + maxB + 0.5 < 2^23
 *   U8     U16       [ 0,  0]    maxA < 2^(shift+14)
 *                                2^8  * maxA * 2^(-shift) +
 *                                max(B - 32768) + 0.5 < 2^30
 *   U8     U16       [ 1,  8]    maxA < 2^15
 *                                2^8  * maxA * 2^(-shift) +
 *                                max(B - 32768) + 0.5 < 2^(31-shift)
 *   U8     U16       [ 9, 31]    maxA < 2^(shift+7)
 *                                2^8  * maxA * 2^(-shift) + max(B - 32768) +
 *                                0.5 < 2^23
 *   U8     S32       [ 0, 31]    no restrictions
 *   U8     F32, D64  [ 0, 31]    no restrictions
 *   S16    U8        [ 0,  7]    maxA < 2^(shift+7)
 *                                2^15 * maxA * 2^(-shift) + maxB + 0.5 < 2^23
 *   S16    U8        [ 8, 16]    maxA < 2^15
 *                                2^15 * maxA * 2^(-shift) + maxB + 0.5 <
 *                                2^(31-shift)
 *   S16    U8        [17, 31]    maxA < 2^(shift-1)
 *                                2^15 * maxA * 2^(-shift) + maxB + 0.5 < 2^15
 *   S16    S16       [ 0,  0]    maxA < 2^(shift+14)
 *                                2^15 * maxA * 2^(-shift) + maxB + 0.5 < 2^30
 *   S16    S16       [ 1, 16]    maxA < 2^15
 *                                2^15 * maxA * 2^(-shift) + maxB + 0.5 <
 *                                2^(31-shift)
 *   S16    S16       [17, 31]    maxA < 2^(shift-1)
 *                                2^15 * maxA * 2^(-shift) + maxB + 0.5 < 2^15
 *   S16    U16       [ 0,  0]    maxA < 2^(shift+14)
 *                                2^15 * maxA * 2^(-shift) + max(B - 32768) +
 *                                0.5 < 2^30
 *   S16    U16       [ 1, 16]    maxA < 2^15
 *                                2^15 * maxA * 2^(-shift) + max(B - 32768) +
 *                                0.5 < 2^(31-shift)
 *   S16    U16       [17, 31]    maxA < 2^(shift-1)
 *                                2^15 * maxA * 2^(-shift) + max(B - 32768) +
 *                                0.5 < 2^15
 *   S16    S32       [ 0, 16]    maxA < 2^15
 *                                2^15 * maxA * 2^(-shift) + maxB + 0.5 <
 *                                2^(31-shift)
 *   S16    S32       [17, 31]    maxA < 2^(shift-1)
 *                                2^15 * maxA * 2^(-shift) + maxB + 0.5 < 2^15
 *   S16    F32, D64  [ 0, 31]    no restrictions
 *   U16    U8        [ 0,  7]    maxA < 2^(shift+7)
 *                                2^16 * maxA * 2^(-shift) + maxB + 0.5 < 2^23
 *   U16    U8        [ 8, 16]    maxA < 2^15
 *                                2^16 * maxA * 2^(-shift) + maxB + 0.5 <
 *                                2^(31-shift)
 *   U16    U8        [17, 31]    maxA < 2^(shift-1)
 *                                2^16 * maxA * 2^(-shift) + maxB + 0.5 < 2^15
 *   U16    S16       [ 0,  0]    maxA < 2^(shift+14)
 *                                2^16 * maxA * 2^(-shift) + maxB + 0.5 < 2^30
 *   U16    S16       [ 1, 16]    maxA < 2^15
 *                                2^16 * maxA * 2^(-shift) + maxB + 0.5 <
 *                                2^(31-shift)
 *   U16    S16       [17, 31]    maxA < 2^(shift-1)
 *                                2^16 * maxA * 2^(-shift) + maxB + 0.5 < 2^15
 *   U16    U16       [ 0,  0]    maxA < 2^(shift+14)
 *                                2^16 * maxA * 2^(-shift) + max(B - 32768) +
 *                                0.5 < 2^30
 *   U16    U16       [ 1, 16]    maxA < 2^15
 *                                2^16 * maxA * 2^(-shift) + max(B - 32768) +
 *                                0.5 < 2^(31-shift)
 *   U16    U16       [17, 31]    maxA < 2^(shift-1)
 *                                2^16 * maxA * 2^(-shift) + max(B - 32768) +
 *                                0.5 < 2^15
 *   U16    S32       [ 0, 16]    maxA < 2^15
 *                                2^16 * maxA * 2^(-shift) + maxB + 0.5 <
 *                                2^(31-shift)
 *   U16    S32       [17, 31]    maxA < 2^(shift-1)
 *                                2^16 * maxA * 2^(-shift) + maxB + 0.5 < 2^15
 *   U16    F32, D64  [ 0, 31]    no restrictions
 *   S32    U8, S16   [ 0, 31]    maxA < 2^(shift-17)
 *                                2^31 * maxA * 2^(-shift) + maxB + 0.5 <
 *                                2^(47-shift)
 *   S32    U8, S16   [ 0, 31]    no restrictions, but may be more
 *                                slow than with restrictions
 *   S32    U16       [ 0, 31]    maxA < 2^(shift-17)
 *                                2^31 * maxA * 2^(-shift) + max(B - 32768) +
 *                                0.5 < 2^(47-shift)
 *   S32    U16       [ 0, 31]    no restrictions, but may be more
 *                                slow than with restrictions
 *   S32    S32       [ 0, 31]    no restrictions
 *   S32    F32, D64  [ 0, 31]    no restrictions
 *
 *   If one of RESTRICTIONS is failed
 *   then mlib_ImageScale returns MLIB_OUTOFRANGE
 *
 * RESTRICTION
 *      The images must have the same size, and the same number
 *      of channels.
 *      The images can have 1, 2, 3, or 4 channels.
 *      The input image can be in MLIB_BYTE, MLIB_SHORT,
 *      MLIB_USHORT or MLIB_INT data type.
 *      The output image can be in MLIB_BYTE,
 *      MLIB_SHORT, MLIB_USHORT, MLIB_INT,
 *      MLIB_FLOAT or MLIB_DOUBLE data type.
 *
 * DESCRIPTION
 *      dst[x][y][c] = src[x][y][c] * alpha[c] * 2^(-shift) + beta[c]
 *
 *      If the result of the operation underflows/overflows the
 *      minimum/maximum value supported by the destination image, then it
 *      will be clamped to the minimum/maximum value respectively.
 */

#include <mlib_image.h>
#include <mlib_ImageCheck.h>
#include <mlib_ImageScale2.h>
#include <mlib_ImageScale.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageScale = __mlib_ImageScale

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_ImageScale) mlib_ImageScale
    __attribute__((weak, alias("__mlib_ImageScale")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	CLAMP_CONV_U8(y, x) \
	if (x >= 0) {	\
		temp = x >> shift; \
	} else {	\
		temp = -((-x) >> shift);	\
	}	\
	if (temp > 255) { \
		y = 255; \
	} else if (temp < 0) { \
		y = 0; \
	} else { \
		y = temp; \
	}
/* *********************************************************** */

#define	CLAMP_CONV_U16(y, x) \
	if (x >= 0) {	\
		temp = x >> shift; \
	} else {	\
		temp = -((-x) >> shift);	\
	}	\
	if (temp > 65535) { \
		y = 65535; \
	} else if (temp < 0) { \
		y = 0; \
	} else { \
		y = temp; \
	}

/* *********************************************************** */

#define	CLAMP_CONV_S16(y, x) \
	if (x >= 0) {	\
		temp = x >> shift; \
	} else {	\
		temp = -((-x) >> shift);	\
	}	\
	if (temp > 32767) { \
		y = 32767; \
	} else if (temp < -32768) { \
		y = -32768; \
	} else { \
		y = temp; \
	}
/* *********************************************************** */

#define	CLAMP_CONV_S32(y, x) \
	if (x >= 0) {	\
		temp = x >> shift; \
	} else {	\
		temp = -((-x) >> shift);	\
	}	\
	if (temp > 2147483647) {  \
		y = 2147483647; \
	} else if (temp < (-2147483647 - 1)) { \
		y = (-2147483647 - 1); \
	} else { \
		y = temp; \
	}

/* *********************************************************** */

#define	CLAMP_CONV_F32(y, x)  y = (mlib_f32) (x);

#define	CLAMP_CONV_D64(y, x)  y = x

/* *********************************************************** */

#define	PROCESS_3CH(TYPE) \
	for (i = 0; i < xsize; i += 3) { \
		v0 = sp[i] * alp0 + bet0; \
		v1 = sp[i + 1] * alp1 + bet1; \
		v2 = sp[i + 2] * alp2 + bet2; \
		CLAMP_CONV_##TYPE(dp[i], v0); \
		CLAMP_CONV_##TYPE(dp[i + 1], v1); \
		CLAMP_CONV_##TYPE(dp[i + 2], v2); \
	}

/* *********************************************************** */

#define	PROCESS_4CH(TYPE) \
	for (i = 0; i <= xsize - 4; i += 4) { \
		v0 = sp[i] * alp0 + bet0; \
		v1 = sp[i + 1] * alp1 + bet1; \
		v2 = sp[i + 2] * alp2 + bet2; \
		v3 = sp[i + 3] * alp3 + bet3; \
		CLAMP_CONV_##TYPE(dp[i], v0); \
		CLAMP_CONV_##TYPE(dp[i + 1], v1); \
		CLAMP_CONV_##TYPE(dp[i + 2], v2); \
		CLAMP_CONV_##TYPE(dp[i + 3], v3); \
	}

/* *********************************************************** */

#define	PROCESS_TAIL(TYPE) \
	if (i < xsize) { \
		CLAMP_CONV_##TYPE(dp[i], (mlib_s64)(sp[i] * alp0 + bet0)); \
		i++; \
	} \
	if (i < xsize) { \
		CLAMP_CONV_##TYPE(dp[i], (mlib_s64)(sp[i] * alp1 + bet1)); \
		i++; \
	} \
	if (i < xsize) { \
		CLAMP_CONV_##TYPE(dp[i], (mlib_s64)(sp[i] * alp1 + bet1)); \
		i++; \
	}

/* *********************************************************** */

#define	PROCESS_F_TAIL(TYPE) \
	if (i < xsize) { \
		CLAMP_CONV_##TYPE(dp[i], sp[i] * alp0 + bet0); \
		i++; \
	} \
	if (i < xsize) { \
		CLAMP_CONV_##TYPE(dp[i], sp[i] * alp1 + bet1); \
		i++; \
	} \
	if (i < xsize) { \
		CLAMP_CONV_##TYPE(dp[i], sp[i] * alp1 + bet1); \
		i++; \
	}
/* *********************************************************** */

#ifndef _NO_LONGLONG

mlib_status
__mlib_ImageScale(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_s32 *alpha,
    const mlib_s32 *beta,
    mlib_s32 shift)
{
	mlib_type stype, dtype;
	mlib_s32 k, nchan;
	MLIB_IMAGE_CHECK(dst);
	MLIB_IMAGE_CHECK(src);
	MLIB_IMAGE_SIZE_EQUAL(dst, src);
	MLIB_IMAGE_CHAN_EQUAL(dst, src);

	if (alpha == NULL)
		return (MLIB_FAILURE);
	if (beta == NULL)
		return (MLIB_FAILURE);
	if (shift < 0 || shift > 31)
		return (MLIB_OUTOFRANGE);

	stype = mlib_ImageGetType(src);
	dtype = mlib_ImageGetType(dst);
	nchan = mlib_ImageGetChannels(dst);

	if (mlib_ImageScale_CheckForVIS(alpha, beta, shift, stype, dtype,
	    nchan) < -1) {
		return (MLIB_OUTOFRANGE);
	}

	if (stype == MLIB_BYTE)
		return (mlib_ImageScale_U8(dst, src, alpha, beta, shift));
	if (stype == MLIB_SHORT)
		return (mlib_ImageScale_S16(dst, src, alpha, beta, shift));
	if (stype == MLIB_USHORT)
		return (mlib_ImageScale_U16(dst, src, alpha, beta, shift));
	if (stype == MLIB_INT)
		return (mlib_ImageScale_S32(dst, src, alpha, beta, shift));

	return (MLIB_FAILURE);
}

/* *********************************************************** */

mlib_status
mlib_ImageScale_U8(
		mlib_image *dst,
		const mlib_image *src,
		const mlib_s32 *alpha,
		const mlib_s32 *beta,
		const mlib_s32 shift)
{
	mlib_d64 dlut[4 * 256];
	mlib_s64 slut[4 * 256];
	void *lut[4];
	mlib_type dtype = mlib_ImageGetType(dst);
	mlib_s32 nchan = mlib_ImageGetChannels(dst);
	mlib_s32 i, k;
	mlib_s64 temp;

	if (dtype == MLIB_BYTE) {
		mlib_u8 *plut = (mlib_u8 *)slut;
		mlib_s64 salp, sval;

/* lut generation */
		for (k = 0; k < nchan; k++) {
			salp = alpha[k];
			sval = ((mlib_s64)beta[k]) << shift;

			for (i = 0; i < 256; i++) {
				CLAMP_CONV_U8(plut[i], sval);
				sval += salp;
			}

			lut[k] = plut;
			plut += 256;
		}
	} else if (dtype == MLIB_SHORT) {
		mlib_s16 *plut = (mlib_s16 *)slut;
		mlib_s64 salp, sval;

/* lut generation */
		for (k = 0; k < nchan; k++) {
			salp = alpha[k];
			sval = ((mlib_s64)beta[k]) << shift;

			for (i = 0; i < 256; i++) {
				CLAMP_CONV_S16(plut[i], sval);
				sval += salp;
			}
			lut[k] = plut;
			plut += 256;
		}
	} else if (dtype == MLIB_USHORT) {
		mlib_u16 *plut = (mlib_u16 *)slut;
		mlib_s64 salp, sval;

/* lut generation */
		for (k = 0; k < nchan; k++) {
			salp = alpha[k];
			sval = ((mlib_s64)beta[k]) << shift;

			for (i = 0; i < 256; i++) {
				CLAMP_CONV_U16(plut[i], sval);
				sval += salp;
			}

			lut[k] = plut;
			plut += 256;
		}

	} else if (dtype == MLIB_INT) {
		mlib_s32 *plut = (mlib_s32 *)slut;
		mlib_s64 salp, sval;

/* lut generation */
		for (k = 0; k < nchan; k++) {
			salp = alpha[k];
			sval = ((mlib_s64)beta[k]) << shift;

			for (i = 0; i < 256; i++) {
				CLAMP_CONV_S32(plut[i], sval);
				sval += salp;
			}
			lut[k] = plut;
			plut += 256;
		}
	} else if (dtype == MLIB_FLOAT) {
		mlib_f32 *plut = (mlib_f32 *)dlut;
		mlib_d64 falp, fval;
		mlib_d64 dshift = 1.0 / (1u << shift);

/* lut generation */
		for (k = 0; k < nchan; k++) {
			falp = alpha[k] * dshift;
			fval = beta[k];

			for (i = 0; i < 256; i++) {
				plut[i] = (mlib_f32)fval;
				fval += falp;
			}

			lut[k] = plut;
			plut += 256;
		}
	} else if (dtype == MLIB_DOUBLE) {
		mlib_d64 *plut = (mlib_d64 *)dlut;
		mlib_d64 falp, fbet, fval;
		mlib_d64 dshift = 1.0 / (1u << shift);

/* lut generation */
		for (k = 0; k < nchan; k++) {
			falp = alpha[k] * dshift;
			fbet = beta[k];

			for (i = 0; i < 256; i++) {
				fval = i * falp + fbet;
				plut[i] = fval;
			}

			lut[k] = plut;
			plut += 256;
		}
	} else {
		return (MLIB_FAILURE);
	}

	return (__mlib_ImageLookUp(dst, src, (const void **)lut));

}


/* *********************************************************** */

mlib_status
mlib_ImageScale_S16(
		mlib_image *dst,
		const mlib_image *src,
		const mlib_s32 *alpha,
		const mlib_s32 *beta,
		const mlib_s32 shift)
{
		mlib_type dtype;
		mlib_s32 slb, dlb, xsize, ysize, nchan;
		mlib_u8 *psrc, *pdst;
		mlib_s32 i, j, k;
		mlib_s64 temp;
		MLIB_IMAGE_GET_ALL_PARAMS(dst, dtype, nchan, xsize, ysize,
				dlb, pdst);

		slb = mlib_ImageGetStride(src);
		psrc = mlib_ImageGetData(src);

		if (dtype == MLIB_BYTE) {
			mlib_s64 alp0, alp1, alp2, alp3;
			mlib_s64 bet0, bet1, bet2, bet3;
			mlib_s64 v0, v1, v2, v3;

/* prepare the alpha and beta values */
			alp0 = alpha[0];
			bet0 = ((mlib_s64)beta[0]) << shift;
			alp1 = alpha[1 % nchan];
			bet1 = ((mlib_s64)beta[1 % nchan]) << shift;
			alp2 = alpha[2 % nchan];
			bet2 = ((mlib_s64)beta[2 % nchan]) << shift;
			alp3 = alpha[3 % nchan];
			bet3 = ((mlib_s64)beta[3 % nchan]) << shift;

			xsize *= nchan;
			if (nchan != 3) {
				for (j = 0; j < ysize; j++) {
					mlib_s16 *sp = (mlib_s16 *)psrc;
					mlib_u8 *dp = (mlib_u8 *)pdst;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
					PROCESS_4CH(U8);
					PROCESS_TAIL(U8);

					psrc += slb;
					pdst += dlb;

				}
			} else {
				for (j = 0; j < ysize; j++) {
					mlib_s16 *sp = (mlib_s16 *)psrc;
					mlib_u8 *dp = (mlib_u8 *)pdst;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
					PROCESS_3CH(U8);

					psrc += slb;
					pdst += dlb;
				}
			}

			return (MLIB_SUCCESS);
		} else if (dtype == MLIB_SHORT) {
			mlib_s64 alp0, alp1, alp2, alp3;
			mlib_s64 bet0, bet1, bet2, bet3;
			mlib_s64 v0, v1, v2, v3;

/* prepare the alpha and beta values */
			alp0 = alpha[0];
			bet0 = ((mlib_s64)beta[0]) << shift;
			alp1 = alpha[1 % nchan];
			bet1 = ((mlib_s64)beta[1 % nchan]) << shift;
			alp2 = alpha[2 % nchan];
			bet2 = ((mlib_s64)beta[2 % nchan]) << shift;
			alp3 = alpha[3 % nchan];
			bet3 = ((mlib_s64)beta[3 % nchan]) << shift;

			xsize *= nchan;

			if (nchan != 3) {
				for (j = 0; j < ysize; j++) {
					mlib_s16 *sp = (mlib_s16 *)psrc;
					mlib_s16 *dp = (mlib_s16 *)pdst;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
					PROCESS_4CH(S16);
					PROCESS_TAIL(S16);

					psrc += slb;
					pdst += dlb;

				}
			} else {
				for (j = 0; j < ysize; j++) {
					mlib_s16 *sp = (mlib_s16 *)psrc;
					mlib_s16 *dp = (mlib_s16 *)pdst;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
					PROCESS_3CH(S16);

					psrc += slb;
					pdst += dlb;
				}
			}

			return (MLIB_SUCCESS);
		} else if (dtype == MLIB_USHORT) {
			mlib_s64 alp0, alp1, alp2, alp3;
			mlib_s64 bet0, bet1, bet2, bet3;
			mlib_s64 v0, v1, v2, v3;

/* prepare the alpha and beta values */
			alp0 = alpha[0];
			bet0 = ((mlib_s64)beta[0]) << shift;
			alp1 = alpha[1 % nchan];
			bet1 = ((mlib_s64)beta[1 % nchan]) << shift;
			alp2 = alpha[2 % nchan];
			bet2 = ((mlib_s64)beta[2 % nchan]) << shift;
			alp3 = alpha[3 % nchan];
			bet3 = ((mlib_s64)beta[3 % nchan]) << shift;

			xsize *= nchan;

			if (nchan != 3) {
				for (j = 0; j < ysize; j++) {
					mlib_s16 *sp = (mlib_s16 *)psrc;
					mlib_u16 *dp = (mlib_u16 *)pdst;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
					PROCESS_4CH(U16);
					PROCESS_TAIL(U16);

					psrc += slb;
					pdst += dlb;

				}
			} else {
				for (j = 0; j < ysize; j++) {
					mlib_s16 *sp = (mlib_s16 *)psrc;
					mlib_u16 *dp = (mlib_u16 *)pdst;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
					PROCESS_3CH(U16);

					psrc += slb;
					pdst += dlb;
				}
			}

			return (MLIB_SUCCESS);
		} else if (dtype == MLIB_INT) {
			mlib_s64 alp0, alp1, alp2, alp3;
			mlib_s64 bet0, bet1, bet2, bet3;
			mlib_s64 v0, v1, v2, v3;

/* prepare the alpha and beta values */
			alp0 = alpha[0];
			bet0 = ((mlib_s64)beta[0]) << shift;
			alp1 = alpha[1 % nchan];
			bet1 = ((mlib_s64)beta[1 % nchan]) << shift;
			alp2 = alpha[2 % nchan];
			bet2 = ((mlib_s64)beta[2 % nchan]) << shift;
			alp3 = alpha[3 % nchan];
			bet3 = ((mlib_s64)beta[3 % nchan]) << shift;

			xsize *= nchan;

			if (nchan != 3) {
				for (j = 0; j < ysize; j++) {
					mlib_s16 *sp = (mlib_s16 *)psrc;
					mlib_s32 *dp = (mlib_s32 *)pdst;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
					PROCESS_4CH(S32);
					PROCESS_TAIL(S32);

					psrc += slb;
					pdst += dlb;

				}
			} else {
				for (j = 0; j < ysize; j++) {
					mlib_s16 *sp = (mlib_s16 *)psrc;
					mlib_s32 *dp = (mlib_s32 *)pdst;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
					PROCESS_3CH(S32);

					psrc += slb;
					pdst += dlb;
				}
			}

			return (MLIB_SUCCESS);
		} else if (dtype == MLIB_FLOAT) {
			mlib_d64 alp0, alp1, alp2, alp3;
			mlib_d64 bet0, bet1, bet2, bet3;
			mlib_d64 v0, v1, v2, v3;
			mlib_d64 dshift = 1.0 / (1u << shift);

/* prepare the alpha and beta values */
			alp0 = alpha[0] * dshift;
			bet0 = beta[0];
			alp1 = alpha[1 % nchan] * dshift;
			bet1 = beta[1 % nchan];
			alp2 = alpha[2 % nchan] * dshift;
			bet2 = beta[2 % nchan];
			alp3 = alpha[3 % nchan] * dshift;
			bet3 = beta[3 % nchan];

			xsize *= nchan;

			if (nchan != 3) {
				for (j = 0; j < ysize; j++) {
					mlib_s16 *sp = (mlib_s16 *)psrc;
					mlib_f32 *dp = (mlib_f32 *)pdst;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
					PROCESS_4CH(F32);
					PROCESS_F_TAIL(F32);

					psrc += slb;
					pdst += dlb;

				}
			} else {
				for (j = 0; j < ysize; j++) {
					mlib_s16 *sp = (mlib_s16 *)psrc;
					mlib_f32 *dp = (mlib_f32 *)pdst;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
					PROCESS_3CH(F32);

					psrc += slb;
					pdst += dlb;
				}
			}

			return (MLIB_SUCCESS);
		} else if (dtype == MLIB_DOUBLE) {
			mlib_d64 alp0, alp1, alp2, alp3;
			mlib_d64 bet0, bet1, bet2, bet3;
			mlib_d64 v0, v1, v2, v3;
			mlib_d64 dshift = 1.0 / (1u << shift);

/* prepare the alpha and beta values */
			alp0 = alpha[0] * dshift;
			bet0 = beta[0];
			alp1 = alpha[1 % nchan] * dshift;
			bet1 = beta[1 % nchan];
			alp2 = alpha[2 % nchan] * dshift;
			bet2 = beta[2 % nchan];
			alp3 = alpha[3 % nchan] * dshift;
			bet3 = beta[3 % nchan];

			xsize *= nchan;

			if (nchan != 3) {
				for (j = 0; j < ysize; j++) {
					mlib_s16 *sp = (mlib_s16 *)psrc;
					mlib_d64 *dp = (mlib_d64 *)pdst;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
					PROCESS_4CH(D64);
					PROCESS_F_TAIL(D64);

					psrc += slb;
					pdst += dlb;

				}
			} else {
				for (j = 0; j < ysize; j++) {
					mlib_s16 *sp = (mlib_s16 *)psrc;
					mlib_d64 *dp = (mlib_d64 *)pdst;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
					PROCESS_3CH(D64);

					psrc += slb;
					pdst += dlb;
				}
			}

			return (MLIB_SUCCESS);
		}

		return (MLIB_FAILURE);
}
/* *********************************************************** */


mlib_status
mlib_ImageScale_U16(
		mlib_image *dst,
		const mlib_image *src,
		const mlib_s32 *alpha,
		const mlib_s32 *beta,
		const mlib_s32 shift)
{
		mlib_type dtype;
		mlib_s32 slb, dlb, xsize, ysize, nchan;
		mlib_u8 *psrc, *pdst;
		mlib_s32 i, j, k;
		mlib_s64 temp;

		MLIB_IMAGE_GET_ALL_PARAMS(dst, dtype, nchan, xsize, ysize,
				dlb, pdst);
		slb = mlib_ImageGetStride(src);
		psrc = mlib_ImageGetData(src);

		if (dtype == MLIB_BYTE) {
			mlib_s64 alp0, alp1, alp2, alp3;
			mlib_s64 bet0, bet1, bet2, bet3;
			mlib_s64 v0, v1, v2, v3;

/* prepare the alpha and beta values */
			alp0 = alpha[0];
			bet0 = ((mlib_s64)beta[0]) << shift;
			alp1 = alpha[1 % nchan];
			bet1 = ((mlib_s64)beta[1 % nchan]) << shift;
			alp2 = alpha[2 % nchan];
			bet2 = ((mlib_s64)beta[2 % nchan]) << shift;
			alp3 = alpha[3 % nchan];
			bet3 = ((mlib_s64)beta[3 % nchan]) << shift;

			xsize *= nchan;

			if (nchan != 3) {
				for (j = 0; j < ysize; j++) {
					mlib_u16 *sp = (mlib_u16 *)psrc;
					mlib_u8 *dp = (mlib_u8 *)pdst;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
					PROCESS_4CH(U8);
					PROCESS_TAIL(U8);

					psrc += slb;
					pdst += dlb;

				}
			} else {
				for (j = 0; j < ysize; j++) {
					mlib_u16 *sp = (mlib_u16 *)psrc;
					mlib_u8 *dp = (mlib_u8 *)pdst;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
					PROCESS_3CH(U8);

					psrc += slb;
					pdst += dlb;
				}
			}

			return (MLIB_SUCCESS);
		} else if (dtype == MLIB_SHORT) {
			mlib_s64 alp0, alp1, alp2, alp3;
			mlib_s64 bet0, bet1, bet2, bet3;
			mlib_s64 v0, v1, v2, v3;

/* prepare the alpha and beta values */
			alp0 = alpha[0];
			bet0 = ((mlib_s64)beta[0]) << shift;
			alp1 = alpha[1 % nchan];
			bet1 = ((mlib_s64)beta[1 % nchan]) << shift;
			alp2 = alpha[2 % nchan];
			bet2 = ((mlib_s64)beta[2 % nchan]) << shift;
			alp3 = alpha[3 % nchan];
			bet3 = ((mlib_s64)beta[3 % nchan]) << shift;

			xsize *= nchan;

			if (nchan != 3) {
				for (j = 0; j < ysize; j++) {
					mlib_u16 *sp = (mlib_u16 *)psrc;
					mlib_s16 *dp = (mlib_s16 *)pdst;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
					PROCESS_4CH(S16);
					PROCESS_TAIL(S16);

					psrc += slb;
					pdst += dlb;

				}
			} else {
				for (j = 0; j < ysize; j++) {
					mlib_u16 *sp = (mlib_u16 *)psrc;
					mlib_s16 *dp = (mlib_s16 *)pdst;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
					PROCESS_3CH(S16);

					psrc += slb;
					pdst += dlb;
				}
			}

			return (MLIB_SUCCESS);
		} else if (dtype == MLIB_USHORT) {
			mlib_s64 alp0, alp1, alp2, alp3;
			mlib_s64 bet0, bet1, bet2, bet3;
			mlib_s64 v0, v1, v2, v3;

/* prepare the alpha and beta values */
			alp0 = alpha[0];
			bet0 = ((mlib_s64)beta[0]) << shift;
			alp1 = alpha[1 % nchan];
			bet1 = ((mlib_s64)beta[1 % nchan]) << shift;
			alp2 = alpha[2 % nchan];
			bet2 = ((mlib_s64)beta[2 % nchan]) << shift;
			alp3 = alpha[3 % nchan];
			bet3 = ((mlib_s64)beta[3 % nchan]) << shift;

			xsize *= nchan;

			if (nchan != 3) {
				for (j = 0; j < ysize; j++) {
					mlib_u16 *sp = (mlib_u16 *)psrc;
					mlib_u16 *dp = (mlib_u16 *)pdst;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
					PROCESS_4CH(U16);
					PROCESS_TAIL(U16);

					psrc += slb;
					pdst += dlb;

				}
			} else {
				for (j = 0; j < ysize; j++) {
					mlib_u16 *sp = (mlib_u16 *)psrc;
					mlib_u16 *dp = (mlib_u16 *)pdst;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
					PROCESS_3CH(U16);

					psrc += slb;
					pdst += dlb;
				}
			}

			return (MLIB_SUCCESS);
		} else if (dtype == MLIB_INT) {
			mlib_s64 alp0, alp1, alp2, alp3;
			mlib_s64 bet0, bet1, bet2, bet3;
			mlib_s64 v0, v1, v2, v3;

/* prepare the alpha and beta values */
			alp0 = alpha[0];
			bet0 = ((mlib_s64)beta[0]) << shift;
			alp1 = alpha[1 % nchan];
			bet1 = ((mlib_s64)beta[1 % nchan]) << shift;
			alp2 = alpha[2 % nchan];
			bet2 = ((mlib_s64)beta[2 % nchan]) << shift;
			alp3 = alpha[3 % nchan];
			bet3 = ((mlib_s64)beta[3 % nchan]) << shift;

			xsize *= nchan;

			if (nchan != 3) {
				for (j = 0; j < ysize; j++) {
					mlib_u16 *sp = (mlib_u16 *)psrc;
					mlib_s32 *dp = (mlib_s32 *)pdst;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
					PROCESS_4CH(S32);
					PROCESS_TAIL(S32);

					psrc += slb;
					pdst += dlb;

				}
			} else {
				for (j = 0; j < ysize; j++) {
					mlib_u16 *sp = (mlib_u16 *)psrc;
					mlib_s32 *dp = (mlib_s32 *)pdst;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
					PROCESS_3CH(S32);

					psrc += slb;
					pdst += dlb;
				}
			}

			return (MLIB_SUCCESS);
		} else if (dtype == MLIB_FLOAT) {
			mlib_d64 alp0, alp1, alp2, alp3;
			mlib_d64 bet0, bet1, bet2, bet3;
			mlib_d64 v0, v1, v2, v3;
			mlib_d64 dshift = 1.0 / (1u << shift);

/* prepare the alpha and beta values */
			alp0 = alpha[0] * dshift;
			bet0 = beta[0];
			alp1 = alpha[1 % nchan] * dshift;
			bet1 = beta[1 % nchan];
			alp2 = alpha[2 % nchan] * dshift;
			bet2 = beta[2 % nchan];
			alp3 = alpha[3 % nchan] * dshift;
			bet3 = beta[3 % nchan];

			xsize *= nchan;

			if (nchan != 3) {
				for (j = 0; j < ysize; j++) {
					mlib_u16 *sp = (mlib_u16 *)psrc;
					mlib_f32 *dp = (mlib_f32 *)pdst;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
					PROCESS_4CH(F32);
					PROCESS_F_TAIL(F32);

					psrc += slb;
					pdst += dlb;

				}
			} else {
				for (j = 0; j < ysize; j++) {
					mlib_u16 *sp = (mlib_u16 *)psrc;
					mlib_f32 *dp = (mlib_f32 *)pdst;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
					PROCESS_3CH(F32);

					psrc += slb;
					pdst += dlb;
				}
			}

			return (MLIB_SUCCESS);
		} else if (dtype == MLIB_DOUBLE) {
			mlib_d64 alp0, alp1, alp2, alp3;
			mlib_d64 bet0, bet1, bet2, bet3;
			mlib_d64 v0, v1, v2, v3;
			mlib_d64 dshift = 1.0 / (1u << shift);

/* prepare the alpha and beta values */
			alp0 = alpha[0] * dshift;
			bet0 = beta[0];
			alp1 = alpha[1 % nchan] * dshift;
			bet1 = beta[1 % nchan];
			alp2 = alpha[2 % nchan] * dshift;
			bet2 = beta[2 % nchan];
			alp3 = alpha[3 % nchan] * dshift;
			bet3 = beta[3 % nchan];

			xsize *= nchan;

			if (nchan != 3) {
				for (j = 0; j < ysize; j++) {
					mlib_u16 *sp = (mlib_u16 *)psrc;
					mlib_d64 *dp = (mlib_d64 *)pdst;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
					PROCESS_4CH(D64);
					PROCESS_F_TAIL(D64);

					psrc += slb;
					pdst += dlb;

				}
			} else {
				for (j = 0; j < ysize; j++) {
					mlib_u16 *sp = (mlib_u16 *)psrc;
					mlib_d64 *dp = (mlib_d64 *)pdst;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
					PROCESS_3CH(D64);

					psrc += slb;
					pdst += dlb;
				}
			}

			return (MLIB_SUCCESS);
		}

		return (MLIB_FAILURE);
}


/* *********************************************************** */


mlib_status
mlib_ImageScale_S32(
		mlib_image *dst,
		const mlib_image *src,
		const mlib_s32 *alpha,
		const mlib_s32 *beta,
		const mlib_s32 shift)
{
		mlib_type dtype;
		mlib_s32 slb, dlb, xsize, ysize, nchan;
		mlib_u8 *psrc, *pdst;
		mlib_s32 i, j, k;
		mlib_s64 temp;
		MLIB_IMAGE_GET_ALL_PARAMS(dst, dtype, nchan, xsize, ysize,
				dlb, pdst);
		slb = mlib_ImageGetStride(src);
		psrc = mlib_ImageGetData(src);

		if (dtype == MLIB_BYTE) {
			mlib_s64 alp0, alp1, alp2, alp3;
			mlib_s64 bet0, bet1, bet2, bet3;
			mlib_s64 v0, v1, v2, v3;

/* prepare the alpha and beta values */
			alp0 = alpha[0];
			bet0 = ((mlib_s64)beta[0]) << shift;
			alp1 = alpha[1 % nchan];
			bet1 = ((mlib_s64)beta[1 % nchan]) << shift;
			alp2 = alpha[2 % nchan];
			bet2 = ((mlib_s64)beta[2 % nchan]) << shift;
			alp3 = alpha[3 % nchan];
			bet3 = ((mlib_s64)beta[3 % nchan]) << shift;

			xsize *= nchan;

			if (nchan != 3) {
				for (j = 0; j < ysize; j++) {
					mlib_s32 *sp = (mlib_s32 *)psrc;
					mlib_u8 *dp = (mlib_u8 *)pdst;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
					PROCESS_4CH(U8);
					PROCESS_TAIL(U8);

					psrc += slb;
					pdst += dlb;

				}
			} else {
				for (j = 0; j < ysize; j++) {
					mlib_s32 *sp = (mlib_s32 *)psrc;
					mlib_u8 *dp = (mlib_u8 *)pdst;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
					PROCESS_3CH(U8);

					psrc += slb;
					pdst += dlb;
				}
			}

			return (MLIB_SUCCESS);
		} else if (dtype == MLIB_SHORT) {
			mlib_s64 alp0, alp1, alp2, alp3;
			mlib_s64 bet0, bet1, bet2, bet3;
			mlib_s64 v0, v1, v2, v3;

/* prepare the alpha and beta values */
			alp0 = alpha[0];
			bet0 = ((mlib_s64)beta[0]) << shift;
			alp1 = alpha[1 % nchan];
			bet1 = ((mlib_s64)beta[1 % nchan]) << shift;
			alp2 = alpha[2 % nchan];
			bet2 = ((mlib_s64)beta[2 % nchan]) << shift;
			alp3 = alpha[3 % nchan];
			bet3 = ((mlib_s64)beta[3 % nchan]) << shift;

			xsize *= nchan;

			if (nchan != 3) {
				for (j = 0; j < ysize; j++) {
					mlib_s32 *sp = (mlib_s32 *)psrc;
					mlib_s16 *dp = (mlib_s16 *)pdst;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
					PROCESS_4CH(S16);
					PROCESS_TAIL(S16);

					psrc += slb;
					pdst += dlb;

				}
			} else {
				for (j = 0; j < ysize; j++) {
					mlib_s32 *sp = (mlib_s32 *)psrc;
					mlib_s16 *dp = (mlib_s16 *)pdst;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
					PROCESS_3CH(S16);

					psrc += slb;
					pdst += dlb;
				}
			}

			return (MLIB_SUCCESS);
		} else if (dtype == MLIB_USHORT) {
			mlib_s64 alp0, alp1, alp2, alp3;
			mlib_s64 bet0, bet1, bet2, bet3;
			mlib_s64 v0, v1, v2, v3;

/* prepare the alpha and beta values */
			alp0 = alpha[0];
			bet0 = ((mlib_s64)beta[0]) << shift;
			alp1 = alpha[1 % nchan];
			bet1 = ((mlib_s64)beta[1 % nchan]) << shift;
			alp2 = alpha[2 % nchan];
			bet2 = ((mlib_s64)beta[2 % nchan]) << shift;
			alp3 = alpha[3 % nchan];
			bet3 = ((mlib_s64)beta[3 % nchan]) << shift;

			xsize *= nchan;

			if (nchan != 3) {
				for (j = 0; j < ysize; j++) {
					mlib_s32 *sp = (mlib_s32 *)psrc;
					mlib_u16 *dp = (mlib_u16 *)pdst;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
					PROCESS_4CH(U16);
					PROCESS_TAIL(U16);

					psrc += slb;
					pdst += dlb;

				}
			} else {
				for (j = 0; j < ysize; j++) {
					mlib_s32 *sp = (mlib_s32 *)psrc;
					mlib_u16 *dp = (mlib_u16 *)pdst;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
					PROCESS_3CH(U16);

					psrc += slb;
					pdst += dlb;
				}
			}

			return (MLIB_SUCCESS);
		} else if (dtype == MLIB_INT) {
			mlib_s64 alp0, alp1, alp2, alp3;
			mlib_s64 bet0, bet1, bet2, bet3;
			mlib_s64 v0, v1, v2, v3;

/* prepare the alpha and beta values */
			alp0 = alpha[0];
			bet0 = ((mlib_s64)beta[0]) << shift;
			alp1 = alpha[1 % nchan];
			bet1 = ((mlib_s64)beta[1 % nchan]) << shift;
			alp2 = alpha[2 % nchan];
			bet2 = ((mlib_s64)beta[2 % nchan]) << shift;
			alp3 = alpha[3 % nchan];
			bet3 = ((mlib_s64)beta[3 % nchan]) << shift;

			xsize *= nchan;

			if (nchan != 3) {
				for (j = 0; j < ysize; j++) {
					mlib_s32 *sp = (mlib_s32 *)psrc;
					mlib_s32 *dp = (mlib_s32 *)pdst;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
					PROCESS_4CH(S32);
					PROCESS_TAIL(S32);

					psrc += slb;
					pdst += dlb;

				}
			} else {
				for (j = 0; j < ysize; j++) {
					mlib_s32 *sp = (mlib_s32 *)psrc;
					mlib_s32 *dp = (mlib_s32 *)pdst;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
					PROCESS_3CH(S32);

					psrc += slb;
					pdst += dlb;
				}
			}

			return (MLIB_SUCCESS);
		} else if (dtype == MLIB_FLOAT) {
			mlib_d64 alp0, alp1, alp2, alp3;
			mlib_d64 bet0, bet1, bet2, bet3;
			mlib_d64 v0, v1, v2, v3;
			mlib_d64 dshift = 1.0 / (1u << shift);

/* prepare the alpha and beta values */
			alp0 = alpha[0] * dshift;
			bet0 = beta[0];
			alp1 = alpha[1 % nchan] * dshift;
			bet1 = beta[1 % nchan];
			alp2 = alpha[2 % nchan] * dshift;
			bet2 = beta[2 % nchan];
			alp3 = alpha[3 % nchan] * dshift;
			bet3 = beta[3 % nchan];

			xsize *= nchan;

			if (nchan != 3) {
				for (j = 0; j < ysize; j++) {
					mlib_s32 *sp = (mlib_s32 *)psrc;
					mlib_f32 *dp = (mlib_f32 *)pdst;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
					PROCESS_4CH(F32);
					PROCESS_F_TAIL(F32);

					psrc += slb;
					pdst += dlb;

				}
			} else {
				for (j = 0; j < ysize; j++) {
					mlib_s32 *sp = (mlib_s32 *)psrc;
					mlib_f32 *dp = (mlib_f32 *)pdst;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
					PROCESS_3CH(F32);

					psrc += slb;
					pdst += dlb;
				}
			}

			return (MLIB_SUCCESS);
		} else if (dtype == MLIB_DOUBLE) {
			mlib_d64 alp0, alp1, alp2, alp3;
			mlib_d64 bet0, bet1, bet2, bet3;
			mlib_d64 v0, v1, v2, v3;
			mlib_d64 dshift = 1.0 /(1u << shift);

/* prepare the alpha and beta values */
			alp0 = alpha[0] * dshift;
			bet0 = beta[0];
			alp1 = alpha[1 % nchan] * dshift;
			bet1 = beta[1 % nchan];
			alp2 = alpha[2 % nchan] * dshift;
			bet2 = beta[2 % nchan];
			alp3 = alpha[3 % nchan] * dshift;
			bet3 = beta[3 % nchan];

			xsize *= nchan;

			if (nchan != 3) {
				for (j = 0; j < ysize; j++) {
					mlib_s32 *sp = (mlib_s32 *)psrc;
					mlib_d64 *dp = (mlib_d64 *)pdst;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
					PROCESS_4CH(D64);
					PROCESS_F_TAIL(D64);

					psrc += slb;
					pdst += dlb;

				}
			} else {
				for (j = 0; j < ysize; j++) {
					mlib_s32 *sp = (mlib_s32 *)psrc;
					mlib_d64 *dp = (mlib_d64 *)pdst;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
					PROCESS_3CH(D64);

					psrc += slb;
					pdst += dlb;
				}
			}

			return (MLIB_SUCCESS);
		}

		return (MLIB_FAILURE);
}


#else


mlib_status
__mlib_ImageScale(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_s32 *alpha,
    const mlib_s32 *beta,
    mlib_s32 shift)
{
	mlib_type stype, dtype;
	mlib_s32 k, nchan;
	mlib_d64 dshift, dalpha[4], dbeta[4];

	MLIB_IMAGE_CHECK(dst);
	MLIB_IMAGE_CHECK(src);
	MLIB_IMAGE_SIZE_EQUAL(dst, src);
	MLIB_IMAGE_CHAN_EQUAL(dst, src);

	if (alpha == NULL)
		return (MLIB_FAILURE);
	if (beta == NULL)
		return (MLIB_FAILURE);
	if (shift < 0 || shift > 31)
		return (MLIB_OUTOFRANGE);

	stype = mlib_ImageGetType(src);
	dtype = mlib_ImageGetType(dst);
	nchan = mlib_ImageGetChannels(dst);

	if (mlib_ImageScale_CheckForVIS(alpha, beta, shift, stype, dtype,
	    nchan) < -1) {
		return (MLIB_OUTOFRANGE);
	}

	dshift = 1.0 / (1u << shift);
	for (k = 0; k < nchan; k++) {
		dalpha[k] = alpha[k] * dshift;
		dbeta[k] = beta[k];
	}
	if (stype == MLIB_BYTE)
		return (mlib_ImageScale2_U8(dst, src, dalpha, dbeta));
	if (stype == MLIB_SHORT)
		return (mlib_ImageScale2_S16(dst, src, dalpha, dbeta));
	if (stype == MLIB_USHORT)
		return (mlib_ImageScale2_U16(dst, src, dalpha, dbeta));
	if (stype == MLIB_INT)
		return (mlib_ImageScale2_S32(dst, src, dalpha, dbeta));

	return (MLIB_FAILURE);
}

/* *********************************************************** */



#endif
