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

#pragma ident	"@(#)mlib_ImageScale2.c	9.3	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_ImageScale2                - Linear scale an image
 *      mlib_ImageScale2_Inp
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageScale2(mlib_image       *dst,
 *                                   const mlib_image *src,
 *                                   const mlib_d64   *alpha,
 *                                   const mlib_d64   *beta)
 *      mlib_status mlib_ImageScale2_Inp(mlib_image     *srcdst,
 *                                       const mlib_d64 *alpha,
 *                                       const mlib_d64 *beta)
 *
 * ARGUMENT
 *      dst     pointer to the output image
 *      src     pointer to the input image
 *      srcdst  pointer to the input and the output image
 *      alpha   array of scaling factors to be multiplied to each channel
 *              of the pixels
 *      beta    array of offsets to be added to each channel of the pixels
 *
 * RESTRICTION
 *      The images must have the same size, and the same number
 *      of channels.
 *      The images can have 1, 2, 3, or 4 channels.
 *      The input image can be in MLIB_BYTE,
 *      MLIB_SHORT, MLIB_USHORT or MLIB_INT data type.
 *      The output image can be in MLIB_BYTE,
 *      MLIB_SHORT, MLIB_USHORT, MLIB_INT,
 *      MLIB_FLOAT or MLIB_DOUBLE data type.
 *
 * DESCRIPTION
 *      For the general version:
 *              dst[x][y][c] = src[x][y][c] * alpha[c] + beta[c]
 *
 *      For in - place processing version (*_Inp):
 *              srcdst[x][y][c] = srcdst[x][y][c] * alpha[c] + beta[c]
 *
 *      If the result of the operation underflows / overflows the
 *      minimum / maximum value supported by the destination image, then it
 *      will be clamped to the minimum / maximum value respectively.
 */

#ifndef	_NO_LONGLONG

#include <mlib_image.h>
#include <mlib_ImageCheck.h>
#include <mlib_ImageScale2.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageScale2 = __mlib_ImageScale2
#pragma weak mlib_ImageScale2_Inp = __mlib_ImageScale2_Inp

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_ImageScale2) mlib_ImageScale2
    __attribute__((weak, alias("__mlib_ImageScale2")));
__typeof__(__mlib_ImageScale2_Inp) mlib_ImageScale2_Inp
    __attribute__((weak, alias("__mlib_ImageScale2_Inp")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#undef	DTOLL
#define	DTOLL(d)	((mlib_s64) ((d) * (1ll << shift)))

#define	HALFONE	(1 << (shift-1))

#define	CAL_SHIFT(d)	((((*(mlib_s64 *)(&(d))) >> 52) & 0x7FF) - 1023)

/* *********************************************************** */

#define	PREPARE_AB_U8                                           \
	for (k = 0; k < nchan; k++) {                           \
	    falpha[k] = alpha[k];                               \
	    fbeta[k] = (beta[k] + 0.5);                         \
	}

#define	PREPARE_AB_U8_INT                                       \
	for (k = 0; k < nchan; k++) {                           \
	    mlib_d64 tmp_beta = beta[k] + 0.5;                  \
	    mlib_s32 tmp_shift;                                 \
	    shift = CAL_SHIFT(alpha[k]);                        \
	    shift = SHIFT - shift;                              \
	    tmp_shift = CAL_SHIFT(tmp_beta);                    \
	    tmp_shift = SHIFT - tmp_shift;                      \
	    shift = shift < tmp_shift ? shift : tmp_shift;      \
	    fshift[k] = shift;                                  \
	    falpha[k] = DTOLL(alpha[k]);                        \
	    fbeta[k] = DTOLL(tmp_beta);                         \
	}

/* *********************************************************** */

#define	PREPARE_AB_S16                                          \
	for (k = 0; k < nchan; k++) {                           \
	    falpha[k] = alpha[k];                               \
	    fbeta[k] = beta[k];                                 \
	}

#define	PREPARE_AB_S16_INT                                      \
	for (k = 0; k < nchan; k++) {                           \
	    mlib_s32 tmp_shift;                                 \
	    shift = CAL_SHIFT(alpha[k]);                        \
	    shift = SHIFT - shift;                              \
	    tmp_shift = CAL_SHIFT(beta[k]);                     \
	    tmp_shift = SHIFT - tmp_shift;                      \
	    shift = shift < tmp_shift ? shift : tmp_shift;      \
	    fshift[k] = shift;                                  \
	    falpha[k] = DTOLL(alpha[k]);                        \
	    fbeta[k] = DTOLL(beta[k]);                          \
	}

/* *********************************************************** */

#define	PREPARE_AB_U16                                          \
	for (k = 0; k < nchan; k++) {                           \
	    falpha[k] = alpha[k];                               \
	    fbeta[k] = beta[k] + 0.5;                           \
	}

#define	PREPARE_AB_U16_INT                                      \
	for (k = 0; k < nchan; k++) {                           \
	    mlib_s32 tmp_shift;                                 \
	    mlib_d64 tmp_beta = beta[k] + 0.5;                  \
	    shift = CAL_SHIFT(alpha[k]);                        \
	    shift = SHIFT - shift;                              \
	    tmp_shift = CAL_SHIFT(tmp_beta);                    \
	    tmp_shift = SHIFT - tmp_shift;                      \
	    shift = shift < tmp_shift ? shift : tmp_shift;      \
	    fshift[k] = shift;                                  \
	    falpha[k] = DTOLL(alpha[k]);                        \
	    fbeta[k] = DTOLL(tmp_beta);                         \
	}

/* *********************************************************** */

#define	CLAMP_CONV_U8(y, x)                                     \
	{                                                       \
	    mlib_s64 xi = (x);                                  \
	                                                        \
	    if (xi >= MLIB_U8_MAX)                              \
		xi = MLIB_U8_MAX;                               \
	    if (xi <= MLIB_U8_MIN)                              \
		xi = MLIB_U8_MIN;                               \
	    y = (mlib_s32)xi;                                   \
	}

#define	CLAMP_CONV_U8_INT(y, x, s)                              \
	{                                                       \
	    mlib_s64 xi = (x) >> s;                             \
	                                                        \
	    if (xi >= MLIB_U8_MAX)                              \
		xi = MLIB_U8_MAX;                               \
	    if (xi <= MLIB_U8_MIN)                              \
		xi = MLIB_U8_MIN;                               \
	    y = (mlib_s32)xi;                                   \
	}

/* *********************************************************** */

#define	CLAMP_CONV_S16(y, x)                                    \
	{                                                       \
	    mlib_s64 xi = (x);                                  \
	                                                        \
	    if (xi >= MLIB_S16_MAX)                             \
		xi = MLIB_S16_MAX;                              \
	    if (xi <= MLIB_S16_MIN)                             \
		xi = MLIB_S16_MIN;                              \
	    y = (mlib_s32)xi;                                   \
	}

#define	CLAMP_CONV_S16_INT(y, x, s)                             \
	{                                                       \
	    mlib_s64 xi;                                        \
	    xi = (x) < 0 ? -(x) : (x);                          \
	    xi >>= s;                                           \
	    if ((x) < 0) xi = -xi;                              \
	                                                        \
	    if (xi >= MLIB_S16_MAX)                             \
		xi = MLIB_S16_MAX;                              \
	    if (xi <= MLIB_S16_MIN)                             \
		xi = MLIB_S16_MIN;                              \
	    y = (mlib_s32)xi;                                   \
	}

/* *********************************************************** */

#define	CLAMP_CONV_U16(y, x)                                    \
	{                                                       \
	    mlib_s64 xi = (x);                                  \
	                                                        \
	    if (xi >= MLIB_U16_MAX)                             \
		xi = MLIB_U16_MAX;                              \
	    if (xi <= MLIB_U16_MIN)                             \
		xi = MLIB_U16_MIN;                              \
	    y = (mlib_s32)xi;                                   \
	}

#define	CLAMP_CONV_U16_INT(y, x, s)                             \
	{                                                       \
	    mlib_s64 xi = (x) >> s;                             \
	                                                        \
	    if (xi >= MLIB_U16_MAX)                             \
		xi = MLIB_U16_MAX;                              \
	    if (xi <= MLIB_U16_MIN)                             \
		xi = MLIB_U16_MIN;                              \
	    y = (mlib_s32)xi;                                   \
	}

/* *********************************************************** */

#define	CLAMP_CONV_S32(y, x)                                    \
	{                                                       \
	    mlib_s64 xi = (x);                                  \
	                                                        \
	    if (xi >= MLIB_S32_MAX)                             \
		xi = MLIB_S32_MAX;                              \
	    if (xi <= MLIB_S32_MIN)                             \
		xi = MLIB_S32_MIN;                              \
	    y = (mlib_s32)xi;                                   \
	}

#define	CLAMP_CONV_S32_INT(y, x, s)                             \
	{                                                       \
	    mlib_s64 xi;                                        \
	    xi = (x) < 0 ? -(x) : (x);                          \
	    xi >>= s;                                           \
	    if ((x) < 0) xi = -xi;                              \
	                                                        \
	    if (xi >= MLIB_S32_MAX)                             \
		xi = MLIB_S32_MAX;                              \
	    if (xi <= MLIB_S32_MIN)                             \
		xi = MLIB_S32_MIN;                              \
	    y = (mlib_s32)xi;                                   \
	}

/* *********************************************************** */

#define	CONSTMUL_FUN(res, x, c)                                 \
	{                                                       \
	    mlib_d64 y = (mlib_d64)(x) * (c);                   \
	                                                        \
	    if (y >= MLIB_S32_MAX)                              \
		y = MLIB_S32_MAX;                               \
	    if (y <= MLIB_S32_MIN)                              \
		y = MLIB_S32_MIN;                               \
	    res = (mlib_s32)y;                                  \
	}

/* *********************************************************** */

typedef union
{
	mlib_s64 d64;
	struct
	{
		mlib_s32 f0, f1;
	} i32s;
} d64_2x32;

/* do not copy the mlib_s16 data as a mlib_s64 if _NO_LONGLONG is defined */
#ifdef _NO_LONGLONG

/* *********************************************************** */

#define	LOAD_DOUBLE(res, psrc)                                  \
	(res).i32s.f0 = ((mlib_s32 *)(psrc))[0];                \
	(res).i32s.f1 = ((mlib_s32 *)(psrc))[1]

/* *********************************************************** */

#define	STORE_DOUBLE(pres, src)                                 \
	((mlib_s32 *)(pres))[0] = (src).i32s.f0;                \
	((mlib_s32 *)(pres))[1] = (src).i32s.f1

#else /* _NO_LONGLONG */

/* *********************************************************** */

#define	LOAD_DOUBLE(res, psrc)	(res).d64 = ((mlib_s64 *)(psrc))[0]

/* *********************************************************** */

#define	STORE_DOUBLE(pres, src)	((mlib_s64 *)(pres))[0] = (src).d64
#endif /* _NO_LONGLONG */

/* *********************************************************** */

#define	CLAMP_CONV_F32(y, x)	y = (mlib_f32) (x);
#define	CLAMP_CONV_D64(y, x)	y = x

/* *********************************************************** */

#define	PROCESS_3CH(TYPE)                                       \
	for (i = 0; i < xsize; i += 3) {                        \
	    v0 = sp[i] * alp0 + bet0;                           \
	    v1 = sp[i + 1] * alp1 + bet1;                       \
	    v2 = sp[i + 2] * alp2 + bet2;                       \
	    CLAMP_CONV_##TYPE(dp[i], v0);                       \
	    CLAMP_CONV_##TYPE(dp[i + 1], v1);                   \
	    CLAMP_CONV_##TYPE(dp[i + 2], v2);                   \
	}

#define	PROCESS_3CH_INT(TYPE)                                   \
	for (i = 0; i < xsize; i += 3) {                        \
	    v0 = sp[i] * alp0 + bet0;                           \
	    v1 = sp[i + 1] * alp1 + bet1;                       \
	    v2 = sp[i + 2] * alp2 + bet2;                       \
	    CLAMP_CONV_##TYPE##_INT(dp[i], v0, sft0);           \
	    CLAMP_CONV_##TYPE##_INT(dp[i + 1], v1, sft1);       \
	    CLAMP_CONV_##TYPE##_INT(dp[i + 2], v2, sft2);       \
	}

/* *********************************************************** */

#define	PROCESS_4CH(TYPE)                                       \
	for (i = 0; i <= xsize - 4; i += 4) {                   \
	    v0 = sp[i] * alp0 + bet0;                           \
	    v1 = sp[i + 1] * alp1 + bet1;                       \
	    v2 = sp[i + 2] * alp2 + bet2;                       \
	    v3 = sp[i + 3] * alp3 + bet3;                       \
	    CLAMP_CONV_##TYPE(dp[i], v0);                       \
	    CLAMP_CONV_##TYPE(dp[i + 1], v1);                   \
	    CLAMP_CONV_##TYPE(dp[i + 2], v2);                   \
	    CLAMP_CONV_##TYPE(dp[i + 3], v3);                   \
	}

#define	PROCESS_4CH_INT(TYPE)                                   \
	for (i = 0; i <= xsize - 4; i += 4) {                   \
	    v0 = sp[i] * alp0 + bet0;                           \
	    v1 = sp[i + 1] * alp1 + bet1;                       \
	    v2 = sp[i + 2] * alp2 + bet2;                       \
	    v3 = sp[i + 3] * alp3 + bet3;                       \
	    CLAMP_CONV_##TYPE##_INT(dp[i], v0, sft0);           \
	    CLAMP_CONV_##TYPE##_INT(dp[i + 1], v1, sft1);       \
	    CLAMP_CONV_##TYPE##_INT(dp[i + 2], v2, sft2);       \
	    CLAMP_CONV_##TYPE##_INT(dp[i + 3], v3, sft3);       \
	}

/* *********************************************************** */

#define	PROCESS_TAIL(TYPE)                                      \
	if (i < xsize) {                                        \
	    CLAMP_CONV_##TYPE(dp[i], sp[i] * alp0 + bet0);      \
	    i++;                                                \
	}                                                       \
	if (i < xsize) {                                        \
	    CLAMP_CONV_##TYPE(dp[i], sp[i] * alp1 + bet1);      \
	    i++;                                                \
	}                                                       \
	if (i < xsize) {                                        \
	    CLAMP_CONV_##TYPE(dp[i], sp[i] * alp2 + bet2);      \
	    i++;                                                \
	}

#define	PROCESS_TAIL_INT(TYPE)                                  \
	if (i < xsize) {                                        \
	    CLAMP_CONV_##TYPE##_INT(dp[i], sp[i] * alp0 + bet0, sft0);      \
	    i++;                                                \
	}                                                       \
	if (i < xsize) {                                        \
	    CLAMP_CONV_##TYPE##_INT(dp[i], sp[i] * alp1 + bet1, sft1);      \
	    i++;                                                \
	}                                                       \
	if (i < xsize) {                                        \
	    CLAMP_CONV_##TYPE##_INT(dp[i], sp[i] * alp2 + bet2, sft2);      \
	    i++;                                                \
	}

/* *********************************************************** */

mlib_status
__mlib_ImageScale2(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_d64 *alpha,
    const mlib_d64 *beta)
{
	mlib_type stype;

	MLIB_IMAGE_CHECK(dst);
	MLIB_IMAGE_CHECK(src);
	MLIB_IMAGE_SIZE_EQUAL(dst, src);
	MLIB_IMAGE_CHAN_EQUAL(dst, src);

	if (alpha == NULL)
		return (MLIB_FAILURE);

	if (beta == NULL)
		return (MLIB_FAILURE);

	stype = mlib_ImageGetType(src);

	if (stype != MLIB_BYTE && stype != MLIB_SHORT && stype != MLIB_USHORT &&
	    stype != MLIB_INT) {
		return (MLIB_FAILURE);
	}

	if (mlib_ImageScale2_fast(dst, src, alpha, beta) == MLIB_SUCCESS) {
		return (MLIB_SUCCESS);
	}

	if (stype == MLIB_BYTE)
		return (mlib_ImageScale2_U8(dst, src, alpha, beta));

	if (stype == MLIB_SHORT)
		return (mlib_ImageScale2_S16(dst, src, alpha, beta));

	if (stype == MLIB_USHORT)
		return (mlib_ImageScale2_U16(dst, src, alpha, beta));

	if (stype == MLIB_INT)
		return (mlib_ImageScale2_S32(dst, src, alpha, beta));

	return (MLIB_FAILURE);
}

/* *********************************************************** */
#undef	SHIFT
#define	SHIFT	52

mlib_status
mlib_ImageScale2_U8(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_d64 *alpha,
    const mlib_d64 *beta)
{
	mlib_d64 dlut[4 * 256];
	void *lut[4];
	mlib_type dtype = mlib_ImageGetType(dst);
	mlib_s32 nchan = mlib_ImageGetChannels(dst);
	mlib_s32 i, k;

	if (dtype == MLIB_BYTE) {
		mlib_u8 *plut = (mlib_u8 *)dlut;
		mlib_s32 fshift[4], shift;
		mlib_s64 falpha[4], fbeta[4];
		mlib_s64 falp, fval;

		PREPARE_AB_U8_INT;

/* lut generation */
		for (k = 0; k < nchan; k++) {
			falp = falpha[k];
			fval = fbeta[k];
			shift = fshift[k];

			for (i = 0; i < 256; i++) {
				CLAMP_CONV_U8_INT(plut[i], fval, shift);
				fval += falp;
			}

			lut[k] = plut;
			plut += 256;
		}
	} else if (dtype == MLIB_SHORT) {
		mlib_s32 fshift[4], shift;
		mlib_s16 *plut = (mlib_s16 *)dlut;
		mlib_s64 falpha[4], fbeta[4];
		mlib_s64 falp, fval;

		PREPARE_AB_S16_INT;

/* lut generation */
		for (k = 0; k < nchan; k++) {
			falp = falpha[k];
			fval = fbeta[k];
			shift = fshift[k];

			for (i = 0; i < 256; i++) {
				CLAMP_CONV_S16_INT(plut[i], fval, shift);
				fval += falp;
			}

			lut[k] = plut;
			plut += 256;
		}
	} else if (dtype == MLIB_USHORT) {
		mlib_u16 *plut = (mlib_u16 *)dlut;
		mlib_s32 fshift[4], shift;
		mlib_s64 falpha[4], fbeta[4];
		mlib_s64 falp, fval;

		PREPARE_AB_U16_INT;

/* lut generation */
		for (k = 0; k < nchan; k++) {
			falp = falpha[k];
			fval = fbeta[k];
			shift = fshift[k];

			for (i = 0; i < 256; i++) {
				CLAMP_CONV_U16_INT(plut[i], fval, shift);
				fval += falp;
			}

			lut[k] = plut;
			plut += 256;
		}
	} else if (dtype == MLIB_INT) {
		mlib_s32 *plut = (mlib_s32 *)dlut;
		mlib_s64 falp, fval;

/* lut generation */
		for (k = 0; k < nchan; k++) {
			mlib_d64 tmp_beta = beta[k] + 0.5;
			mlib_s32 shift, tmp_shift;
			shift = CAL_SHIFT(alpha[k]);
			shift = SHIFT - shift;
			/* tmp_shift = CAL_SHIFT(beta[k]); */
			tmp_shift = CAL_SHIFT(tmp_beta);
			tmp_shift = SHIFT - tmp_shift;
			shift = shift < tmp_shift ? shift : tmp_shift;
			falp = DTOLL(alpha[k]);
			fval = DTOLL(beta[k]);

			for (i = 0; i < 256; i++) {
				/*
				 * mlib_d64 rval =
				 *	fval + ((fval >= 0) ? 0.5 : -0.5);
				 */
				mlib_s64 rval =
				    fval + ((fval >= 0) ? HALFONE : -HALFONE);

				CLAMP_CONV_S32_INT(plut[i], rval, shift);
				fval += falp;
			}

			lut[k] = plut;
			plut += 256;
		}
	} else if (dtype == MLIB_FLOAT) {
		mlib_f32 *plut = (mlib_f32 *)dlut;
		mlib_d64 falp, fval;

/* lut generation */
		for (k = 0; k < nchan; k++) {
			falp = alpha[k];
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

/* lut generation */
		for (k = 0; k < nchan; k++) {
			falp = alpha[k];
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
#undef	SHIFT
#define	SHIFT	47

mlib_status
mlib_ImageScale2_S16(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_d64 *alpha,
    const mlib_d64 *beta)
{
	mlib_type dtype;
	mlib_s32 slb, dlb, xsize, ysize, nchan;
	mlib_u8 *psrc, *pdst;
	mlib_s32 i, j, k;

	MLIB_IMAGE_GET_ALL_PARAMS(dst, dtype, nchan, xsize, ysize, dlb, pdst);
	slb = mlib_ImageGetStride(src);
	psrc = mlib_ImageGetData(src);

	if (dtype == MLIB_BYTE) {
		mlib_s32 fshift[4], shift;
		mlib_s32 sft0, sft1, sft2, sft3;
		mlib_s64 falpha[4], fbeta[4];
		mlib_s64 alp0, alp1, alp2, alp3;
		mlib_s64 bet0, bet1, bet2, bet3;
		mlib_s64 v0, v1, v2, v3;

/* prepare the alpha and beta values */
		PREPARE_AB_U8_INT;
		alp0 = falpha[0];
		bet0 = fbeta[0];
		sft0 = fshift[0];
		alp1 = falpha[1 % nchan];
		bet1 = fbeta[1 % nchan];
		sft1 = fshift[1 % nchan];
		alp2 = falpha[2 % nchan];
		bet2 = fbeta[2 % nchan];
		sft2 = fshift[2 % nchan];
		alp3 = falpha[3 % nchan];
		bet3 = fbeta[3 % nchan];
		sft3 = fshift[3 % nchan];

		xsize *= nchan;

		if (nchan != 3) {
			for (j = 0; j < ysize; j++) {
				mlib_s16 *sp = (mlib_s16 *)psrc;
				mlib_u8 *dp = (mlib_u8 *)pdst;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				PROCESS_4CH_INT(U8);
				PROCESS_TAIL_INT(U8);

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
				PROCESS_3CH_INT(U8);

				psrc += slb;
				pdst += dlb;
			}
		}

		return (MLIB_SUCCESS);
	} else if (dtype == MLIB_SHORT) {
		mlib_s32 fshift[4], shift;
		mlib_s32 sft0, sft1, sft2, sft3;
		mlib_s64 falpha[4], fbeta[4];
		mlib_s64 alp0, alp1, alp2, alp3;
		mlib_s64 bet0, bet1, bet2, bet3;
		mlib_s64 v0, v1, v2, v3;

/* prepare the alpha and beta values */
		PREPARE_AB_S16_INT;
		alp0 = falpha[0];
		bet0 = fbeta[0];
		sft0 = fshift[0];
		alp1 = falpha[1 % nchan];
		bet1 = fbeta[1 % nchan];
		sft1 = fshift[1 % nchan];
		alp2 = falpha[2 % nchan];
		bet2 = fbeta[2 % nchan];
		sft2 = fshift[2 % nchan];
		alp3 = falpha[3 % nchan];
		bet3 = fbeta[3 % nchan];
		sft3 = fshift[3 % nchan];

		xsize *= nchan;

		if (nchan != 3) {
			for (j = 0; j < ysize; j++) {
				mlib_s16 *sp = (mlib_s16 *)psrc;
				mlib_s16 *dp = (mlib_s16 *)pdst;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				PROCESS_4CH_INT(S16);
				PROCESS_TAIL_INT(S16);

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
				PROCESS_3CH_INT(S16);

				psrc += slb;
				pdst += dlb;
			}
		}

		return (MLIB_SUCCESS);
	} else if (dtype == MLIB_USHORT) {
		mlib_s32 fshift[4], shift;
		mlib_s32 sft0, sft1, sft2, sft3;
		mlib_s64 falpha[4], fbeta[4];
		mlib_s64 alp0, alp1, alp2, alp3;
		mlib_s64 bet0, bet1, bet2, bet3;
		mlib_s64 v0, v1, v2, v3;

/* prepare the alpha and beta values */
		PREPARE_AB_U16_INT;
		alp0 = falpha[0];
		bet0 = fbeta[0];
		sft0 = fshift[0];
		alp1 = falpha[1 % nchan];
		bet1 = fbeta[1 % nchan];
		sft1 = fshift[1 % nchan];
		alp2 = falpha[2 % nchan];
		bet2 = fbeta[2 % nchan];
		sft2 = fshift[2 % nchan];
		alp3 = falpha[3 % nchan];
		bet3 = fbeta[3 % nchan];
		sft3 = fshift[3 % nchan];

		xsize *= nchan;

		if (nchan != 3) {
			for (j = 0; j < ysize; j++) {
				mlib_s16 *sp = (mlib_s16 *)psrc;
				mlib_u16 *dp = (mlib_u16 *)pdst;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				PROCESS_4CH_INT(U16);
				PROCESS_TAIL_INT(U16);

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
				PROCESS_3CH_INT(U16);

				psrc += slb;
				pdst += dlb;
			}
		}

		return (MLIB_SUCCESS);
	} else if (dtype == MLIB_INT) {
		mlib_s32 shift, tmp_shift;
		mlib_s32 sft0, sft1, sft2, sft3;
		mlib_s64 alp0, alp1, alp2, alp3;
		mlib_s64 bet0, bet1, bet2, bet3;
		mlib_s64 v0, v1, v2, v3;

/* prepare the alpha and beta values */
		shift = SHIFT - CAL_SHIFT(alpha[0]);
		tmp_shift = SHIFT - CAL_SHIFT(beta[0]);
		sft0 = shift < tmp_shift ? shift : tmp_shift;
		shift = sft0;
		alp0 = DTOLL(alpha[0]);
		bet0 = DTOLL(beta[0]);

		shift = SHIFT - CAL_SHIFT(alpha[1 % nchan]);
		tmp_shift = SHIFT - CAL_SHIFT(beta[1 % nchan]);
		sft1 = shift < tmp_shift ? shift : tmp_shift;
		shift = sft1;
		alp1 = DTOLL(alpha[1 % nchan]);
		bet1 = DTOLL(beta[1 % nchan]);

		shift = SHIFT - CAL_SHIFT(alpha[2 % nchan]);
		tmp_shift = SHIFT - CAL_SHIFT(beta[2 % nchan]);
		sft2 = shift < tmp_shift ? shift : tmp_shift;
		shift = sft2;
		alp2 = DTOLL(alpha[2 % nchan]);
		bet2 = DTOLL(beta[2 % nchan]);

		shift = SHIFT - CAL_SHIFT(alpha[3 % nchan]);
		tmp_shift = SHIFT - CAL_SHIFT(beta[3 % nchan]);
		sft3 = shift < tmp_shift ? shift : tmp_shift;
		shift = sft3;
		alp3 = DTOLL(alpha[3 % nchan]);
		bet3 = DTOLL(beta[3 % nchan]);

		xsize *= nchan;

		if (nchan != 3) {
			for (j = 0; j < ysize; j++) {
				mlib_s16 *sp = (mlib_s16 *)psrc;
				mlib_s32 *dp = (mlib_s32 *)pdst;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				PROCESS_4CH_INT(S32);
				PROCESS_TAIL_INT(S32);

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
				PROCESS_3CH_INT(S32);

				psrc += slb;
				pdst += dlb;
			}
		}

		return (MLIB_SUCCESS);
	} else if (dtype == MLIB_FLOAT) {
		mlib_d64 alp0, alp1, alp2, alp3;
		mlib_d64 bet0, bet1, bet2, bet3;
		mlib_d64 v0, v1, v2, v3;

/* prepare the alpha and beta values */
		alp0 = alpha[0];
		bet0 = beta[0];
		alp1 = alpha[1 % nchan];
		bet1 = beta[1 % nchan];
		alp2 = alpha[2 % nchan];
		bet2 = beta[2 % nchan];
		alp3 = alpha[3 % nchan];
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
				PROCESS_TAIL(F32);

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

/* prepare the alpha and beta values */
		alp0 = alpha[0];
		bet0 = beta[0];
		alp1 = alpha[1 % nchan];
		bet1 = beta[1 % nchan];
		alp2 = alpha[2 % nchan];
		bet2 = beta[2 % nchan];
		alp3 = alpha[3 % nchan];
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
				PROCESS_TAIL(D64);

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
#undef	SHIFT
#define	SHIFT	46

mlib_status
mlib_ImageScale2_U16(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_d64 *alpha,
    const mlib_d64 *beta)
{
	mlib_type dtype;
	mlib_s32 slb, dlb, xsize, ysize, nchan;
	mlib_u8 *psrc, *pdst;
	mlib_s32 i, j, k;

	MLIB_IMAGE_GET_ALL_PARAMS(dst, dtype, nchan, xsize, ysize, dlb, pdst);
	slb = mlib_ImageGetStride(src);
	psrc = mlib_ImageGetData(src);

	if (dtype == MLIB_BYTE) {
		mlib_s32 fshift[4], shift;
		mlib_s32 sft0, sft1, sft2, sft3;
		mlib_s64 falpha[4], fbeta[4];
		mlib_s64 alp0, alp1, alp2, alp3;
		mlib_s64 bet0, bet1, bet2, bet3;
		mlib_s64 v0, v1, v2, v3;

/* prepare the alpha and beta values */
		PREPARE_AB_U8_INT;
		alp0 = falpha[0];
		bet0 = fbeta[0];
		sft0 = fshift[0];
		alp1 = falpha[1 % nchan];
		bet1 = fbeta[1 % nchan];
		sft1 = fshift[1 % nchan];
		alp2 = falpha[2 % nchan];
		bet2 = fbeta[2 % nchan];
		sft2 = fshift[2 % nchan];
		alp3 = falpha[3 % nchan];
		bet3 = fbeta[3 % nchan];
		sft3 = fshift[3 % nchan];

		xsize *= nchan;

		if (nchan != 3) {
			for (j = 0; j < ysize; j++) {
				mlib_u16 *sp = (mlib_u16 *)psrc;
				mlib_u8 *dp = (mlib_u8 *)pdst;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				PROCESS_4CH_INT(U8);
				PROCESS_TAIL_INT(U8);

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
				PROCESS_3CH_INT(U8);

				psrc += slb;
				pdst += dlb;
			}
		}

		return (MLIB_SUCCESS);
	} else if (dtype == MLIB_SHORT) {
		mlib_s32 fshift[4], shift;
		mlib_s32 sft0, sft1, sft2, sft3;
		mlib_s64 falpha[4], fbeta[4];
		mlib_s64 alp0, alp1, alp2, alp3;
		mlib_s64 bet0, bet1, bet2, bet3;
		mlib_s64 v0, v1, v2, v3;

/* prepare the alpha and beta values */
		PREPARE_AB_S16_INT;
		alp0 = falpha[0];
		bet0 = fbeta[0];
		sft0 = fshift[0];
		alp1 = falpha[1 % nchan];
		bet1 = fbeta[1 % nchan];
		sft1 = fshift[1 % nchan];
		alp2 = falpha[2 % nchan];
		bet2 = fbeta[2 % nchan];
		sft2 = fshift[2 % nchan];
		alp3 = falpha[3 % nchan];
		bet3 = fbeta[3 % nchan];
		sft3 = fshift[3 % nchan];

		xsize *= nchan;

		if (nchan != 3) {
			for (j = 0; j < ysize; j++) {
				mlib_u16 *sp = (mlib_u16 *)psrc;
				mlib_s16 *dp = (mlib_s16 *)pdst;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				PROCESS_4CH_INT(S16);
				PROCESS_TAIL_INT(S16);

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
				PROCESS_3CH_INT(S16);

				psrc += slb;
				pdst += dlb;
			}
		}

		return (MLIB_SUCCESS);
	} else if (dtype == MLIB_USHORT) {
		mlib_s32 fshift[4], shift;
		mlib_s32 sft0, sft1, sft2, sft3;
		mlib_s64 falpha[4], fbeta[4];
		mlib_s64 alp0, alp1, alp2, alp3;
		mlib_s64 bet0, bet1, bet2, bet3;
		mlib_s64 v0, v1, v2, v3;

/* prepare the alpha and beta values */
		PREPARE_AB_U16_INT;
		alp0 = falpha[0];
		bet0 = fbeta[0];
		sft0 = fshift[0];
		alp1 = falpha[1 % nchan];
		bet1 = fbeta[1 % nchan];
		sft1 = fshift[1 % nchan];
		alp2 = falpha[2 % nchan];
		bet2 = fbeta[2 % nchan];
		sft2 = fshift[2 % nchan];
		alp3 = falpha[3 % nchan];
		bet3 = fbeta[3 % nchan];
		sft3 = fshift[3 % nchan];

		xsize *= nchan;

		if (nchan != 3) {
			for (j = 0; j < ysize; j++) {
				mlib_u16 *sp = (mlib_u16 *)psrc;
				mlib_u16 *dp = (mlib_u16 *)pdst;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				PROCESS_4CH_INT(U16);
				PROCESS_TAIL_INT(U16);

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
				PROCESS_3CH_INT(U16);

				psrc += slb;
				pdst += dlb;
			}
		}

		return (MLIB_SUCCESS);
	} else if (dtype == MLIB_INT) {
		mlib_d64 alp0, alp1, alp2, alp3;
		mlib_d64 bet0, bet1, bet2, bet3;
		mlib_d64 v0, v1, v2, v3;

/* prepare the alpha and beta values */
		alp0 = alpha[0];
		bet0 = beta[0];
		alp1 = alpha[1 % nchan];
		bet1 = beta[1 % nchan];
		alp2 = alpha[2 % nchan];
		bet2 = beta[2 % nchan];
		alp3 = alpha[3 % nchan];
		bet3 = beta[3 % nchan];

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

/* prepare the alpha and beta values */
		alp0 = alpha[0];
		bet0 = beta[0];
		alp1 = alpha[1 % nchan];
		bet1 = beta[1 % nchan];
		alp2 = alpha[2 % nchan];
		bet2 = beta[2 % nchan];
		alp3 = alpha[3 % nchan];
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
				PROCESS_TAIL(F32);

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

/* prepare the alpha and beta values */
		alp0 = alpha[0];
		bet0 = beta[0];
		alp1 = alpha[1 % nchan];
		bet1 = beta[1 % nchan];
		alp2 = alpha[2 % nchan];
		bet2 = beta[2 % nchan];
		alp3 = alpha[3 % nchan];
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
				PROCESS_TAIL(D64);

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
mlib_ImageDConstMul_S32(
    mlib_s32 *dl,
    mlib_s32 *sl,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 nchan,
    mlib_s32 slb,
    mlib_s32 dlb,
    const mlib_d64 *dconsts)
{
	mlib_s32 *sp, *dp, *pend;
	mlib_d64 *sp2, *dp2;
	mlib_s32 dsize = xsize * nchan;
	mlib_d64 dc[6], *c;
	mlib_s32 j, k;

	for (k = 0; k < nchan; k++) {
		dc[k] = dconsts[k];
	}

	for (k = nchan; k < 6; k++)
		dc[k] = dc[k - nchan];

	sp = sl;
	dp = dl;

	for (j = 0; j < ysize; j++) {
		pend = dp + dsize;

		c = dc;

		if ((mlib_addr)dp & 7) {
			CONSTMUL_FUN(dp[0], (*sp++), (*c++));
			dp++;
		}

		dp2 = (mlib_d64 *)dp;

/* nchan = 3, any aligning for src */
		if (nchan == 3) {
			mlib_d64 c0, c1, c2;

			c0 = c[0];
			c1 = c[1];
			c2 = c[2];
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (; (mlib_s32 *)dp2 <= (pend - 6); dp2 += 3) {
				d64_2x32 d0, d1, d2;

				CONSTMUL_FUN(d0.i32s.f0, sp[0], c0);
				CONSTMUL_FUN(d0.i32s.f1, sp[1], c1);
				STORE_DOUBLE(dp2, d0);

				CONSTMUL_FUN(d1.i32s.f0, sp[2], c2);
				CONSTMUL_FUN(d1.i32s.f1, sp[3], c0);
				STORE_DOUBLE(dp2 + 1, d1);

				CONSTMUL_FUN(d2.i32s.f0, sp[4], c1);
				CONSTMUL_FUN(d2.i32s.f1, sp[5], c2);
				STORE_DOUBLE(dp2 + 2, d2);

				sp += 6;
			}
		} else {

/* nchan != 3, all aligned */
			if (((mlib_addr)sp & 7) == 0) {
				mlib_d64 c0, c1, c2, c3;

				c0 = c[0];
				c1 = c[1];
				c2 = c[2];
				c3 = c[3];

				sp2 = (mlib_d64 *)sp;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (; (mlib_s32 *)dp2 <=
				    (pend - 4); dp2 += 2) {
					d64_2x32 s0, s1, d0, d1;

					LOAD_DOUBLE(s0, sp2);
					CONSTMUL_FUN(d0.i32s.f0, s0.i32s.f0,
					    c0);
					CONSTMUL_FUN(d0.i32s.f1, s0.i32s.f1,
					    c1);
					STORE_DOUBLE(dp2, d0);

					LOAD_DOUBLE(s1, sp2 + 1);
					CONSTMUL_FUN(d1.i32s.f0, s1.i32s.f0,
					    c2);
					CONSTMUL_FUN(d1.i32s.f1, s1.i32s.f1,
					    c3);
					STORE_DOUBLE(dp2 + 1, d1);

					sp2 += 2;
				}

				sp = (mlib_s32 *)sp2;
			} else {
/* nchan != 3, src unaligned */
				mlib_d64 c0, c1, c2, c3;

				c0 = c[0];
				c1 = c[1];
				c2 = c[2];
				c3 = c[3];

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (; (mlib_s32 *)dp2 <=
				    (pend - 4); dp2 += 2) {
					d64_2x32 d0, d1;

					CONSTMUL_FUN(d0.i32s.f0, sp[0], c0);
					CONSTMUL_FUN(d0.i32s.f1, sp[1], c1);
					STORE_DOUBLE(dp2, d0);

					CONSTMUL_FUN(d1.i32s.f0, sp[2], c2);
					CONSTMUL_FUN(d1.i32s.f1, sp[3], c3);
					STORE_DOUBLE(dp2 + 1, d1);

					sp += 4;
				}
			}
		}

		dp = (mlib_s32 *)dp2;
		while (dp < pend) {
			CONSTMUL_FUN(dp[0], (*sp++), (*c++));
			dp++;
		}

		sp = sl += slb;
		dp = dl += dlb;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_ImageScale2_S32(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_d64 *alpha,
    const mlib_d64 *beta)
{
	mlib_type dtype;
	mlib_s32 slb, dlb, xsize, ysize, nchan;
	mlib_u8 *psrc, *pdst;
	mlib_s32 i, j, k;

	MLIB_IMAGE_GET_ALL_PARAMS(dst, dtype, nchan, xsize, ysize, dlb, pdst);
	slb = mlib_ImageGetStride(src);
	psrc = mlib_ImageGetData(src);

	if (dtype == MLIB_INT) {
		mlib_s32 flag = 1;

		for (k = 0; k < nchan; k++) {
			if (beta[k] != 0)
				flag = 0;
		}

		if (flag) {
			return mlib_ImageDConstMul_S32((mlib_s32 *)pdst,
			    (mlib_s32 *)psrc, xsize, ysize, nchan, slb / 4,
			    dlb / 4, alpha);
		}
	}

	if (dtype == MLIB_BYTE) {
		mlib_d64 falpha[4], fbeta[4];
		mlib_d64 alp0, alp1, alp2, alp3;
		mlib_d64 bet0, bet1, bet2, bet3;
		mlib_d64 v0, v1, v2, v3;

/* prepare the alpha and beta values */
		PREPARE_AB_U8;
		alp0 = falpha[0];
		bet0 = fbeta[0];
		alp1 = falpha[1 % nchan];
		bet1 = fbeta[1 % nchan];
		alp2 = falpha[2 % nchan];
		bet2 = fbeta[2 % nchan];
		alp3 = falpha[3 % nchan];
		bet3 = fbeta[3 % nchan];

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
		mlib_d64 falpha[4], fbeta[4];
		mlib_d64 alp0, alp1, alp2, alp3;
		mlib_d64 bet0, bet1, bet2, bet3;
		mlib_d64 v0, v1, v2, v3;

/* prepare the alpha and beta values */
		PREPARE_AB_S16;
		alp0 = falpha[0];
		bet0 = fbeta[0];
		alp1 = falpha[1 % nchan];
		bet1 = fbeta[1 % nchan];
		alp2 = falpha[2 % nchan];
		bet2 = fbeta[2 % nchan];
		alp3 = falpha[3 % nchan];
		bet3 = fbeta[3 % nchan];

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
		mlib_d64 falpha[4], fbeta[4];
		mlib_d64 alp0, alp1, alp2, alp3;
		mlib_d64 bet0, bet1, bet2, bet3;
		mlib_d64 v0, v1, v2, v3;

/* prepare the alpha and beta values */
		PREPARE_AB_U16;
		alp0 = falpha[0];
		bet0 = fbeta[0];
		alp1 = falpha[1 % nchan];
		bet1 = fbeta[1 % nchan];
		alp2 = falpha[2 % nchan];
		bet2 = fbeta[2 % nchan];
		alp3 = falpha[3 % nchan];
		bet3 = fbeta[3 % nchan];

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
		mlib_d64 alp0, alp1, alp2, alp3;
		mlib_d64 bet0, bet1, bet2, bet3;
		mlib_d64 v0, v1, v2, v3;

/* prepare the alpha and beta values */
		alp0 = alpha[0];
		bet0 = beta[0];
		alp1 = alpha[1 % nchan];
		bet1 = beta[1 % nchan];
		alp2 = alpha[2 % nchan];
		bet2 = beta[2 % nchan];
		alp3 = alpha[3 % nchan];
		bet3 = beta[3 % nchan];

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

/* prepare the alpha and beta values */
		alp0 = alpha[0];
		bet0 = beta[0];
		alp1 = alpha[1 % nchan];
		bet1 = beta[1 % nchan];
		alp2 = alpha[2 % nchan];
		bet2 = beta[2 % nchan];
		alp3 = alpha[3 % nchan];
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
				PROCESS_TAIL(F32);

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

/* prepare the alpha and beta values */
		alp0 = alpha[0];
		bet0 = beta[0];
		alp1 = alpha[1 % nchan];
		bet1 = beta[1 % nchan];
		alp2 = alpha[2 % nchan];
		bet2 = beta[2 % nchan];
		alp3 = alpha[3 % nchan];
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
				PROCESS_TAIL(D64);

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

/* *********************************************************** */

mlib_status
__mlib_ImageScale2_Inp(
    mlib_image *srcdst,
    const mlib_d64 *alpha,
    const mlib_d64 *beta)
{
	return (__mlib_ImageScale2(srcdst, srcdst, alpha, beta));
}

/* *********************************************************** */

#else /* _NO_LONGLONG */

#include <mlib_image.h>
#include <mlib_ImageCheck.h>
#include <mlib_ImageScale2.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageScale2 = __mlib_ImageScale2
#pragma weak mlib_ImageScale2_Inp = __mlib_ImageScale2_Inp

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_ImageScale2) mlib_ImageScale2
    __attribute__((weak, alias("__mlib_ImageScale2")));
__typeof__(__mlib_ImageScale2_Inp) mlib_ImageScale2_Inp
    __attribute__((weak, alias("__mlib_ImageScale2_Inp")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#ifdef MLIB_USE_FTOI_CLAMPING

#define	CLAMP_FTOI(DST, SRC)	DST = (mlib_s32)(SRC)

/* *********************************************************** */

#define	PREPARE_AB_U8                                           \
	for (k = 0; k < nchan; k++) {                           \
	    falpha[k] = alpha[k] * (1 << 24);                   \
	    fbeta[k] = (beta[k] - 127.5) * (1 << 24);           \
	}

/* *********************************************************** */

#define	PREPARE_AB_S16                                          \
	for (k = 0; k < nchan; k++) {                           \
	    falpha[k] = alpha[k] * 65536;                       \
	    fbeta[k] = (beta[k] + 0.5) * 65536;                 \
	}

/* *********************************************************** */

#define	PREPARE_AB_U16                                          \
	for (k = 0; k < nchan; k++) {                           \
	    falpha[k] = alpha[k] * 65536;                       \
	    fbeta[k] = (beta[k] - 32767.5) * 65536;             \
	}

/* *********************************************************** */

#define	CLAMP_CONV_U8(y, x)                                     \
	{                                                       \
	    mlib_s32 xi;                                        \
	                                                        \
	    CLAMP_FTOI(xi, x);                                  \
	    y = ((xi >> 24) + 128);                             \
	}

/* *********************************************************** */

#define	CLAMP_CONV_S16(y, x)                                    \
	{                                                       \
	    mlib_s32 xi;                                        \
	                                                        \
	    CLAMP_FTOI(xi, x);                                  \
	    y = (xi >> 16);                                     \
	}

/* *********************************************************** */

#define	CLAMP_CONV_U16(y, x)                                    \
	{                                                       \
	    mlib_s32 xi;                                        \
	                                                        \
	    CLAMP_FTOI(xi, x);                                  \
	    y = (xi >> 16) ^ 0x8000;                            \
	}

/* *********************************************************** */

#define	CLAMP_CONV_S32(y, x)	CLAMP_FTOI(y, x)

/* *********************************************************** */

#define	CONSTMUL_FUN(res, x, c)	res = (mlib_s32)((mlib_d64)(x) * (c))

#else /* MLIB_USE_FTOI_CLAMPING */

/* *********************************************************** */

#define	PREPARE_AB_U8                                           \
	for (k = 0; k < nchan; k++) {                           \
	    falpha[k] = alpha[k];                               \
	    fbeta[k] = (beta[k] + 0.5);                         \
	}

/* *********************************************************** */

#define	PREPARE_AB_S16                                          \
	for (k = 0; k < nchan; k++) {                           \
	    falpha[k] = alpha[k];                               \
	    fbeta[k] = beta[k];                                 \
	}

/* *********************************************************** */

#define	PREPARE_AB_U16                                          \
	for (k = 0; k < nchan; k++) {                           \
	    falpha[k] = alpha[k];                               \
	    fbeta[k] = beta[k] + 0.5;                           \
	}

/* *********************************************************** */

#define	CLAMP_CONV_U8(y, x)                                     \
	{                                                       \
	    mlib_d64 xi = (x);                                  \
	                                                        \
	    if (xi >= MLIB_U8_MAX)                              \
		xi = MLIB_U8_MAX;                               \
	    if (xi <= MLIB_U8_MIN)                              \
		xi = MLIB_U8_MIN;                               \
	    y = (mlib_s32)xi;                                   \
	}

/* *********************************************************** */

#define	CLAMP_CONV_S16(y, x)                                    \
	{                                                       \
	    mlib_d64 xi = (x);                                  \
	                                                        \
	    if (xi >= MLIB_S16_MAX)                             \
		xi = MLIB_S16_MAX;                              \
	    if (xi <= MLIB_S16_MIN)                             \
		xi = MLIB_S16_MIN;                              \
	    y = (mlib_s32)xi;                                   \
	}

/* *********************************************************** */

#define	CLAMP_CONV_U16(y, x)                                    \
	{                                                       \
	    mlib_d64 xi = (x);                                  \
	                                                        \
	    if (xi >= MLIB_U16_MAX)                             \
		xi = MLIB_U16_MAX;                              \
	    if (xi <= MLIB_U16_MIN)                             \
		xi = MLIB_U16_MIN;                              \
	    y = (mlib_s32)xi;                                   \
	}

/* *********************************************************** */

#define	CLAMP_CONV_S32(y, x)                                    \
	{                                                       \
	    mlib_d64 xi = (x);                                  \
	                                                        \
	    if (xi >= MLIB_S32_MAX)                             \
		xi = MLIB_S32_MAX;                              \
	    if (xi <= MLIB_S32_MIN)                             \
		xi = MLIB_S32_MIN;                              \
	    y = (mlib_s32)xi;                                   \
	}

/* *********************************************************** */

#define	CONSTMUL_FUN(res, x, c)                                 \
	{                                                       \
	    mlib_d64 y = (mlib_d64)(x) * (c);                   \
	                                                        \
	    if (y >= MLIB_S32_MAX)                              \
		y = MLIB_S32_MAX;                               \
	    if (y <= MLIB_S32_MIN)                              \
		y = MLIB_S32_MIN;                               \
	    res = (mlib_s32)y;                                  \
	}

#endif /* MLIB_USE_FTOI_CLAMPING */

/* *********************************************************** */

typedef union
{
	mlib_d64 d64;
	struct
	{
		mlib_s32 f0, f1;
	} i32s;
} d64_2x32;

/* do not copy the mlib_s16 data as a mlib_d64 for Intel */
#ifdef i386

/* *********************************************************** */

#define	LOAD_DOUBLE(res, psrc)                                  \
	(res).i32s.f0 = ((mlib_s32 *)(psrc))[0];                \
	(res).i32s.f1 = ((mlib_s32 *)(psrc))[1]

/* *********************************************************** */

#define	STORE_DOUBLE(pres, src)                                 \
	((mlib_s32 *)(pres))[0] = (src).i32s.f0;                \
	((mlib_s32 *)(pres))[1] = (src).i32s.f1

#else /* i386 */

/* *********************************************************** */

#define	LOAD_DOUBLE(res, psrc)	(res).d64 = ((mlib_d64 *)(psrc))[0]

/* *********************************************************** */

#define	STORE_DOUBLE(pres, src)	((mlib_d64 *)(pres))[0] = (src).d64
#endif /* i386 */

/* *********************************************************** */

#define	CLAMP_CONV_F32(y, x)	y = (mlib_f32) (x);
#define	CLAMP_CONV_D64(y, x)	y = x

/* *********************************************************** */

#define	PROCESS_3CH(TYPE)                                       \
	for (i = 0; i < xsize; i += 3) {                        \
	    v0 = sp[i] * alp0 + bet0;                           \
	    v1 = sp[i + 1] * alp1 + bet1;                       \
	    v2 = sp[i + 2] * alp2 + bet2;                       \
	    CLAMP_CONV_##TYPE(dp[i], v0);                       \
	    CLAMP_CONV_##TYPE(dp[i + 1], v1);                   \
	    CLAMP_CONV_##TYPE(dp[i + 2], v2);                   \
	}

/* *********************************************************** */

#define	PROCESS_4CH(TYPE)                                       \
	for (i = 0; i <= xsize - 4; i += 4) {                   \
	    v0 = sp[i] * alp0 + bet0;                           \
	    v1 = sp[i + 1] * alp1 + bet1;                       \
	    v2 = sp[i + 2] * alp2 + bet2;                       \
	    v3 = sp[i + 3] * alp3 + bet3;                       \
	    CLAMP_CONV_##TYPE(dp[i], v0);                       \
	    CLAMP_CONV_##TYPE(dp[i + 1], v1);                   \
	    CLAMP_CONV_##TYPE(dp[i + 2], v2);                   \
	    CLAMP_CONV_##TYPE(dp[i + 3], v3);                   \
	}

/* *********************************************************** */

#define	PROCESS_TAIL(TYPE)                                      \
	if (i < xsize) {                                        \
	    CLAMP_CONV_##TYPE(dp[i], sp[i] * alp0 + bet0);      \
	    i++;                                                \
	}                                                       \
	if (i < xsize) {                                        \
	    CLAMP_CONV_##TYPE(dp[i], sp[i] * alp1 + bet1);      \
	    i++;                                                \
	}                                                       \
	if (i < xsize) {                                        \
	    CLAMP_CONV_##TYPE(dp[i], sp[i] * alp2 + bet2);      \
	    i++;                                                \
	}

/* *********************************************************** */

mlib_status
__mlib_ImageScale2(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_d64 *alpha,
    const mlib_d64 *beta)
{
	mlib_type stype;

	MLIB_IMAGE_CHECK(dst);
	MLIB_IMAGE_CHECK(src);
	MLIB_IMAGE_SIZE_EQUAL(dst, src);
	MLIB_IMAGE_CHAN_EQUAL(dst, src);

	if (alpha == NULL)
		return (MLIB_FAILURE);

	if (beta == NULL)
		return (MLIB_FAILURE);

	stype = mlib_ImageGetType(src);

	if (stype != MLIB_BYTE && stype != MLIB_SHORT && stype != MLIB_USHORT &&
	    stype != MLIB_INT) {
		return (MLIB_FAILURE);
	}

	if (mlib_ImageScale2_fast(dst, src, alpha, beta) == MLIB_SUCCESS) {
		return (MLIB_SUCCESS);
	}

	if (stype == MLIB_BYTE)
		return (mlib_ImageScale2_U8(dst, src, alpha, beta));

	if (stype == MLIB_SHORT)
		return (mlib_ImageScale2_S16(dst, src, alpha, beta));

	if (stype == MLIB_USHORT)
		return (mlib_ImageScale2_U16(dst, src, alpha, beta));

	if (stype == MLIB_INT)
		return (mlib_ImageScale2_S32(dst, src, alpha, beta));

	return (MLIB_FAILURE);
}

/* *********************************************************** */

mlib_status
mlib_ImageScale2_U8(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_d64 *alpha,
    const mlib_d64 *beta)
{
	mlib_d64 dlut[4 * 256];
	void *lut[4];
	mlib_type dtype = mlib_ImageGetType(dst);
	mlib_s32 nchan = mlib_ImageGetChannels(dst);
	mlib_s32 i, k;

	if (dtype == MLIB_BYTE) {
		mlib_u8 *plut = (mlib_u8 *)dlut;
		mlib_d64 falpha[4], fbeta[4];
		mlib_d64 falp, fval;

		PREPARE_AB_U8;

/* lut generation */
		for (k = 0; k < nchan; k++) {
			falp = falpha[k];
			fval = fbeta[k];

			for (i = 0; i < 256; i++) {
				CLAMP_CONV_U8(plut[i], fval);
				fval += falp;
			}

			lut[k] = plut;
			plut += 256;
		}
	} else if (dtype == MLIB_SHORT) {
		mlib_s16 *plut = (mlib_s16 *)dlut;
		mlib_d64 falpha[4], fbeta[4];
		mlib_d64 falp, fval;

		PREPARE_AB_S16;

/* lut generation */
		for (k = 0; k < nchan; k++) {
			falp = falpha[k];
			fval = fbeta[k];

			for (i = 0; i < 256; i++) {
				CLAMP_CONV_S16(plut[i], fval);
				fval += falp;
			}

			lut[k] = plut;
			plut += 256;
		}
	} else if (dtype == MLIB_USHORT) {
		mlib_u16 *plut = (mlib_u16 *)dlut;
		mlib_d64 falpha[4], fbeta[4];
		mlib_d64 falp, fval;

		PREPARE_AB_U16;

/* lut generation */
		for (k = 0; k < nchan; k++) {
			falp = falpha[k];
			fval = fbeta[k];

			for (i = 0; i < 256; i++) {
				CLAMP_CONV_U16(plut[i], fval);
				fval += falp;
			}

			lut[k] = plut;
			plut += 256;
		}
	} else if (dtype == MLIB_INT) {
		mlib_s32 *plut = (mlib_s32 *)dlut;
		mlib_d64 falp, fval;

/* lut generation */
		for (k = 0; k < nchan; k++) {
			falp = alpha[k];
			fval = beta[k];

			for (i = 0; i < 256; i++) {
				mlib_d64 rval =
				    fval + ((fval >= 0) ? 0.5 : -0.5);

				CLAMP_CONV_S32(plut[i], rval);
				fval += falp;
			}

			lut[k] = plut;
			plut += 256;
		}
	} else if (dtype == MLIB_FLOAT) {
		mlib_f32 *plut = (mlib_f32 *)dlut;
		mlib_d64 falp, fval;

/* lut generation */
		for (k = 0; k < nchan; k++) {
			falp = alpha[k];
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

/* lut generation */
		for (k = 0; k < nchan; k++) {
			falp = alpha[k];
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
mlib_ImageScale2_S16(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_d64 *alpha,
    const mlib_d64 *beta)
{
	mlib_type dtype;
	mlib_s32 slb, dlb, xsize, ysize, nchan;
	mlib_u8 *psrc, *pdst;
	mlib_s32 i, j, k;

	MLIB_IMAGE_GET_ALL_PARAMS(dst, dtype, nchan, xsize, ysize, dlb, pdst);
	slb = mlib_ImageGetStride(src);
	psrc = mlib_ImageGetData(src);

	if (dtype == MLIB_BYTE) {
		mlib_d64 falpha[4], fbeta[4];
		mlib_d64 alp0, alp1, alp2, alp3;
		mlib_d64 bet0, bet1, bet2, bet3;
		mlib_d64 v0, v1, v2, v3;

/* prepare the alpha and beta values */
		PREPARE_AB_U8;
		alp0 = falpha[0];
		bet0 = fbeta[0];
		alp1 = falpha[1 % nchan];
		bet1 = fbeta[1 % nchan];
		alp2 = falpha[2 % nchan];
		bet2 = fbeta[2 % nchan];
		alp3 = falpha[3 % nchan];
		bet3 = fbeta[3 % nchan];

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
		mlib_d64 falpha[4], fbeta[4];
		mlib_d64 alp0, alp1, alp2, alp3;
		mlib_d64 bet0, bet1, bet2, bet3;
		mlib_d64 v0, v1, v2, v3;

/* prepare the alpha and beta values */
		PREPARE_AB_S16;
		alp0 = falpha[0];
		bet0 = fbeta[0];
		alp1 = falpha[1 % nchan];
		bet1 = fbeta[1 % nchan];
		alp2 = falpha[2 % nchan];
		bet2 = fbeta[2 % nchan];
		alp3 = falpha[3 % nchan];
		bet3 = fbeta[3 % nchan];

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
		mlib_d64 falpha[4], fbeta[4];
		mlib_d64 alp0, alp1, alp2, alp3;
		mlib_d64 bet0, bet1, bet2, bet3;
		mlib_d64 v0, v1, v2, v3;

/* prepare the alpha and beta values */
		PREPARE_AB_U16;
		alp0 = falpha[0];
		bet0 = fbeta[0];
		alp1 = falpha[1 % nchan];
		bet1 = fbeta[1 % nchan];
		alp2 = falpha[2 % nchan];
		bet2 = fbeta[2 % nchan];
		alp3 = falpha[3 % nchan];
		bet3 = fbeta[3 % nchan];

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
		mlib_d64 alp0, alp1, alp2, alp3;
		mlib_d64 bet0, bet1, bet2, bet3;
		mlib_d64 v0, v1, v2, v3;

/* prepare the alpha and beta values */
		alp0 = alpha[0];
		bet0 = beta[0];
		alp1 = alpha[1 % nchan];
		bet1 = beta[1 % nchan];
		alp2 = alpha[2 % nchan];
		bet2 = beta[2 % nchan];
		alp3 = alpha[3 % nchan];
		bet3 = beta[3 % nchan];

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

/* prepare the alpha and beta values */
		alp0 = alpha[0];
		bet0 = beta[0];
		alp1 = alpha[1 % nchan];
		bet1 = beta[1 % nchan];
		alp2 = alpha[2 % nchan];
		bet2 = beta[2 % nchan];
		alp3 = alpha[3 % nchan];
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
				PROCESS_TAIL(F32);

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

/* prepare the alpha and beta values */
		alp0 = alpha[0];
		bet0 = beta[0];
		alp1 = alpha[1 % nchan];
		bet1 = beta[1 % nchan];
		alp2 = alpha[2 % nchan];
		bet2 = beta[2 % nchan];
		alp3 = alpha[3 % nchan];
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
				PROCESS_TAIL(D64);

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
mlib_ImageScale2_U16(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_d64 *alpha,
    const mlib_d64 *beta)
{
	mlib_type dtype;
	mlib_s32 slb, dlb, xsize, ysize, nchan;
	mlib_u8 *psrc, *pdst;
	mlib_s32 i, j, k;

	MLIB_IMAGE_GET_ALL_PARAMS(dst, dtype, nchan, xsize, ysize, dlb, pdst);
	slb = mlib_ImageGetStride(src);
	psrc = mlib_ImageGetData(src);

	if (dtype == MLIB_BYTE) {
		mlib_d64 falpha[4], fbeta[4];
		mlib_d64 alp0, alp1, alp2, alp3;
		mlib_d64 bet0, bet1, bet2, bet3;
		mlib_d64 v0, v1, v2, v3;

/* prepare the alpha and beta values */
		PREPARE_AB_U8;
		alp0 = falpha[0];
		bet0 = fbeta[0];
		alp1 = falpha[1 % nchan];
		bet1 = fbeta[1 % nchan];
		alp2 = falpha[2 % nchan];
		bet2 = fbeta[2 % nchan];
		alp3 = falpha[3 % nchan];
		bet3 = fbeta[3 % nchan];

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
		mlib_d64 falpha[4], fbeta[4];
		mlib_d64 alp0, alp1, alp2, alp3;
		mlib_d64 bet0, bet1, bet2, bet3;
		mlib_d64 v0, v1, v2, v3;

/* prepare the alpha and beta values */
		PREPARE_AB_S16;
		alp0 = falpha[0];
		bet0 = fbeta[0];
		alp1 = falpha[1 % nchan];
		bet1 = fbeta[1 % nchan];
		alp2 = falpha[2 % nchan];
		bet2 = fbeta[2 % nchan];
		alp3 = falpha[3 % nchan];
		bet3 = fbeta[3 % nchan];

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
		mlib_d64 falpha[4], fbeta[4];
		mlib_d64 alp0, alp1, alp2, alp3;
		mlib_d64 bet0, bet1, bet2, bet3;
		mlib_d64 v0, v1, v2, v3;

/* prepare the alpha and beta values */
		PREPARE_AB_U16;
		alp0 = falpha[0];
		bet0 = fbeta[0];
		alp1 = falpha[1 % nchan];
		bet1 = fbeta[1 % nchan];
		alp2 = falpha[2 % nchan];
		bet2 = fbeta[2 % nchan];
		alp3 = falpha[3 % nchan];
		bet3 = fbeta[3 % nchan];

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
		mlib_d64 alp0, alp1, alp2, alp3;
		mlib_d64 bet0, bet1, bet2, bet3;
		mlib_d64 v0, v1, v2, v3;

/* prepare the alpha and beta values */
		alp0 = alpha[0];
		bet0 = beta[0];
		alp1 = alpha[1 % nchan];
		bet1 = beta[1 % nchan];
		alp2 = alpha[2 % nchan];
		bet2 = beta[2 % nchan];
		alp3 = alpha[3 % nchan];
		bet3 = beta[3 % nchan];

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

/* prepare the alpha and beta values */
		alp0 = alpha[0];
		bet0 = beta[0];
		alp1 = alpha[1 % nchan];
		bet1 = beta[1 % nchan];
		alp2 = alpha[2 % nchan];
		bet2 = beta[2 % nchan];
		alp3 = alpha[3 % nchan];
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
				PROCESS_TAIL(F32);

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

/* prepare the alpha and beta values */
		alp0 = alpha[0];
		bet0 = beta[0];
		alp1 = alpha[1 % nchan];
		bet1 = beta[1 % nchan];
		alp2 = alpha[2 % nchan];
		bet2 = beta[2 % nchan];
		alp3 = alpha[3 % nchan];
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
				PROCESS_TAIL(D64);

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
mlib_ImageDConstMul_S32(
    mlib_s32 *dl,
    mlib_s32 *sl,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 nchan,
    mlib_s32 slb,
    mlib_s32 dlb,
    const mlib_d64 *dconsts)
{
	mlib_s32 *sp, *dp, *pend;
	mlib_d64 *sp2, *dp2;
	mlib_s32 dsize = xsize * nchan;
	mlib_d64 dc[6], *c;
	mlib_s32 j, k;

	for (k = 0; k < nchan; k++) {
		dc[k] = dconsts[k];
	}

	for (k = nchan; k < 6; k++)
		dc[k] = dc[k - nchan];

	sp = sl;
	dp = dl;

	for (j = 0; j < ysize; j++) {
		pend = dp + dsize;

		c = dc;

		if ((mlib_addr)dp & 7) {
			CONSTMUL_FUN(dp[0], (*sp++), (*c++));
			dp++;
		}

		dp2 = (mlib_d64 *)dp;

/* nchan = 3, any aligning for src */
		if (nchan == 3) {
			mlib_d64 c0, c1, c2;

			c0 = c[0];
			c1 = c[1];
			c2 = c[2];
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (; (mlib_s32 *)dp2 <= (pend - 6); dp2 += 3) {
				d64_2x32 d0, d1, d2;

				CONSTMUL_FUN(d0.i32s.f0, sp[0], c0);
				CONSTMUL_FUN(d0.i32s.f1, sp[1], c1);
				STORE_DOUBLE(dp2, d0);

				CONSTMUL_FUN(d1.i32s.f0, sp[2], c2);
				CONSTMUL_FUN(d1.i32s.f1, sp[3], c0);
				STORE_DOUBLE(dp2 + 1, d1);

				CONSTMUL_FUN(d2.i32s.f0, sp[4], c1);
				CONSTMUL_FUN(d2.i32s.f1, sp[5], c2);
				STORE_DOUBLE(dp2 + 2, d2);

				sp += 6;
			}
		} else {

/* nchan != 3, all aligned */
			if (((mlib_addr)sp & 7) == 0) {
				mlib_d64 c0, c1, c2, c3;

				c0 = c[0];
				c1 = c[1];
				c2 = c[2];
				c3 = c[3];

				sp2 = (mlib_d64 *)sp;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (; (mlib_s32 *)dp2 <=
				    (pend - 4); dp2 += 2) {
					d64_2x32 s0, s1, d0, d1;

					LOAD_DOUBLE(s0, sp2);
					CONSTMUL_FUN(d0.i32s.f0, s0.i32s.f0,
					    c0);
					CONSTMUL_FUN(d0.i32s.f1, s0.i32s.f1,
					    c1);
					STORE_DOUBLE(dp2, d0);

					LOAD_DOUBLE(s1, sp2 + 1);
					CONSTMUL_FUN(d1.i32s.f0, s1.i32s.f0,
					    c2);
					CONSTMUL_FUN(d1.i32s.f1, s1.i32s.f1,
					    c3);
					STORE_DOUBLE(dp2 + 1, d1);

					sp2 += 2;
				}

				sp = (mlib_s32 *)sp2;
			} else {
/* nchan != 3, src unaligned */
				mlib_d64 c0, c1, c2, c3;

				c0 = c[0];
				c1 = c[1];
				c2 = c[2];
				c3 = c[3];

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (; (mlib_s32 *)dp2 <=
				    (pend - 4); dp2 += 2) {
					d64_2x32 d0, d1;

					CONSTMUL_FUN(d0.i32s.f0, sp[0], c0);
					CONSTMUL_FUN(d0.i32s.f1, sp[1], c1);
					STORE_DOUBLE(dp2, d0);

					CONSTMUL_FUN(d1.i32s.f0, sp[2], c2);
					CONSTMUL_FUN(d1.i32s.f1, sp[3], c3);
					STORE_DOUBLE(dp2 + 1, d1);

					sp += 4;
				}
			}
		}

		dp = (mlib_s32 *)dp2;
		while (dp < pend) {
			CONSTMUL_FUN(dp[0], (*sp++), (*c++));
			dp++;
		}

		sp = sl += slb;
		dp = dl += dlb;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_ImageScale2_S32(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_d64 *alpha,
    const mlib_d64 *beta)
{
	mlib_type dtype;
	mlib_s32 slb, dlb, xsize, ysize, nchan;
	mlib_u8 *psrc, *pdst;
	mlib_s32 i, j, k;

	MLIB_IMAGE_GET_ALL_PARAMS(dst, dtype, nchan, xsize, ysize, dlb, pdst);
	slb = mlib_ImageGetStride(src);
	psrc = mlib_ImageGetData(src);

	if (dtype == MLIB_INT) {
		mlib_s32 flag = 1;

		for (k = 0; k < nchan; k++) {
			if (beta[k] != 0)
				flag = 0;
		}

		if (flag) {
			return mlib_ImageDConstMul_S32((mlib_s32 *)pdst,
			    (mlib_s32 *)psrc, xsize, ysize, nchan, slb / 4,
			    dlb / 4, alpha);
		}
	}

	if (dtype == MLIB_BYTE) {
		mlib_d64 falpha[4], fbeta[4];
		mlib_d64 alp0, alp1, alp2, alp3;
		mlib_d64 bet0, bet1, bet2, bet3;
		mlib_d64 v0, v1, v2, v3;

/* prepare the alpha and beta values */
		PREPARE_AB_U8;
		alp0 = falpha[0];
		bet0 = fbeta[0];
		alp1 = falpha[1 % nchan];
		bet1 = fbeta[1 % nchan];
		alp2 = falpha[2 % nchan];
		bet2 = fbeta[2 % nchan];
		alp3 = falpha[3 % nchan];
		bet3 = fbeta[3 % nchan];

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
		mlib_d64 falpha[4], fbeta[4];
		mlib_d64 alp0, alp1, alp2, alp3;
		mlib_d64 bet0, bet1, bet2, bet3;
		mlib_d64 v0, v1, v2, v3;

/* prepare the alpha and beta values */
		PREPARE_AB_S16;
		alp0 = falpha[0];
		bet0 = fbeta[0];
		alp1 = falpha[1 % nchan];
		bet1 = fbeta[1 % nchan];
		alp2 = falpha[2 % nchan];
		bet2 = fbeta[2 % nchan];
		alp3 = falpha[3 % nchan];
		bet3 = fbeta[3 % nchan];

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
		mlib_d64 falpha[4], fbeta[4];
		mlib_d64 alp0, alp1, alp2, alp3;
		mlib_d64 bet0, bet1, bet2, bet3;
		mlib_d64 v0, v1, v2, v3;

/* prepare the alpha and beta values */
		PREPARE_AB_U16;
		alp0 = falpha[0];
		bet0 = fbeta[0];
		alp1 = falpha[1 % nchan];
		bet1 = fbeta[1 % nchan];
		alp2 = falpha[2 % nchan];
		bet2 = fbeta[2 % nchan];
		alp3 = falpha[3 % nchan];
		bet3 = fbeta[3 % nchan];

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
		mlib_d64 alp0, alp1, alp2, alp3;
		mlib_d64 bet0, bet1, bet2, bet3;
		mlib_d64 v0, v1, v2, v3;

/* prepare the alpha and beta values */
		alp0 = alpha[0];
		bet0 = beta[0];
		alp1 = alpha[1 % nchan];
		bet1 = beta[1 % nchan];
		alp2 = alpha[2 % nchan];
		bet2 = beta[2 % nchan];
		alp3 = alpha[3 % nchan];
		bet3 = beta[3 % nchan];

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

/* prepare the alpha and beta values */
		alp0 = alpha[0];
		bet0 = beta[0];
		alp1 = alpha[1 % nchan];
		bet1 = beta[1 % nchan];
		alp2 = alpha[2 % nchan];
		bet2 = beta[2 % nchan];
		alp3 = alpha[3 % nchan];
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
				PROCESS_TAIL(F32);

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

/* prepare the alpha and beta values */
		alp0 = alpha[0];
		bet0 = beta[0];
		alp1 = alpha[1 % nchan];
		bet1 = beta[1 % nchan];
		alp2 = alpha[2 % nchan];
		bet2 = beta[2 % nchan];
		alp3 = alpha[3 % nchan];
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
				PROCESS_TAIL(D64);

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

/* *********************************************************** */

mlib_status
__mlib_ImageScale2_Inp(
    mlib_image *srcdst,
    const mlib_d64 *alpha,
    const mlib_d64 *beta)
{
	return (__mlib_ImageScale2(srcdst, srcdst, alpha, beta));
}

/* *********************************************************** */

#endif /* _NO_LONGLONG */
