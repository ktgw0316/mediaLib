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

#pragma ident	"@(#)mlib_i_SignalMulSShiftAdd.c	9.4	07/11/05 SMI"

/*
 * FUNCTIONS
 *      mlib_SignalMulSShiftAdd_[S16|S16S]_Sat,
 *      mlib_SignalMulSShiftAdd_[S16|S16S]_[S16|S16S]_Sat - multiplication of
 *                     16-bit format signal array by a scalar with shifting
 *                     plus addition
 *
 * SYNOPSIS
 *      mlib_status mlib_SignalMulSShiftAdd_S16S_S16S_Sat(mlib_s16       *dst,
 *                                                        const mlib_s16 *src1,
 *                                                        const mlib_s16 *src2,
 *                                                        const mlib_s16 *c,
 *                                                        mlib_s32       shift,
 *                                                        mlib_s32       n);
 *      mlib_status mlib_SignalMulSShiftAdd_S16S_Sat(mlib_s16       *data,
 *                                                   const mlib_s16 *src2,
 *                                                   const mlib_s16 *c,
 *                                                   mlib_s32       shift,
 *                                                   mlib_s32       n);
 *      mlib_status mlib_SignalMulSShiftAdd_S16_S16_Sat(mlib_s16       *dst,
 *                                                      const mlib_s16 *src1,
 *                                                      const mlib_s16 *src2,
 *                                                      const mlib_s16 *c,
 *                                                      mlib_s32       shift,
 *                                                      mlib_s32       n);
 *      mlib_status mlib_SignalMulSShiftAdd_S16_Sat(mlib_s16       *data,
 *                                                  const mlib_s16 *src2,
 *                                                  const mlib_s16 *c,
 *                                                  mlib_s32       shift,
 *                                                  mlib_s32       n);
 *
 * ARGUMENTS
 *      data  Input and output signal array
 *      dst   Output signal array
 *      src   Input signal array
 *      c     Scaling factor. The scaling factor is in Q15 format
 *      shift Left shifting factor, may be negative
 *      n     Number of samples in the input signal arrays.
 *
 * DESCRIPTION
 *      dst = src1 + src2 * c * (2 ** shift)
 */

#include <mlib_signal.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_SignalMulSShiftAdd_S16S_S16S_Sat = \
	__mlib_SignalMulSShiftAdd_S16S_S16S_Sat
#pragma weak mlib_SignalMulSShiftAdd_S16_S16_Sat = \
	__mlib_SignalMulSShiftAdd_S16_S16_Sat

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_SignalMulSShiftAdd_S16S_S16S_Sat)
    mlib_SignalMulSShiftAdd_S16S_S16S_Sat
    __attribute__((weak, alias("__mlib_SignalMulSShiftAdd_S16S_S16S_Sat")));
__typeof__(__mlib_SignalMulSShiftAdd_S16_S16_Sat)
    mlib_SignalMulSShiftAdd_S16_S16_Sat
    __attribute__((weak, alias("__mlib_SignalMulSShiftAdd_S16_S16_Sat")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#ifdef MLIB_USE_FTOI_CLAMPING

#define	FLOAT2INT_CLAMP(X)	((mlib_s32)(X))

#else /* MLIB_USE_FTOI_CLAMPING */

#define	FLOAT2INT_CLAMP(X)                                      \
	(((X) > MLIB_S32_MAX) ? MLIB_S32_MAX : (((X) <          \
	    MLIB_S32_MIN) ? MLIB_S32_MIN : ((mlib_s32)(X))))

#endif /* MLIB_USE_FTOI_CLAMPING */

#define	SATURATE(X)                                              \
	(((X) > 0x7fff) ? MLIB_S16_MAX : ((X) <                  \
	    (mlib_s32)(-0x8000) ? MLIB_S16_MIN : (mlib_s16)(X)))

/* ********************************************************* */

#if ! defined(_NO_LONGLONG)

mlib_status
__mlib_SignalMulSShiftAdd_S16_S16_Sat(
    mlib_s16 *dst,
    const mlib_s16 *src1,
    const mlib_s16 *src2,
    const mlib_s16 *c,
    mlib_s32 shift,
    mlib_s32 n)
{
	mlib_s32 i;
	mlib_s64 val_c;

	if (shift < 0) {
		if (shift < -31)
			shift = -31;
		val_c = (((mlib_u16)(*c) << 16) >> (-shift)) >> 15;
	} else {
		if (shift > 31)
			shift = 31;
		val_c = ((*c) * ((mlib_u64)1 << shift)) << 1;
	}

	for (i = 0; i < (n - 1); i += 2) {
		mlib_s32 a, b;
		mlib_s64 dd1, dd2;
		a = ((mlib_u16 *)src1)[i] << 16;
		b = ((mlib_u16 *)src1)[i + 1] << 16;
		dd1 = (a + src2[i] * val_c) >> 16;
		dd2 = (b + src2[i + 1] * val_c) >> 16;
		dst[i] = SATURATE(dd1);
		dst[i + 1] = SATURATE(dd2);
	}

	if (i < n) {
		mlib_s32 a;
		a = ((mlib_u16 *)src1)[i] << 16;
		dst[i] = SATURATE((a + src2[i] * val_c) >> 16);
	}

	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);
}

/* *********************************************************** */

mlib_status
__mlib_SignalMulSShiftAdd_S16S_S16S_Sat(
    mlib_s16 *dst,
    const mlib_s16 *src1,
    const mlib_s16 *src2,
    const mlib_s16 *c,
    mlib_s32 shift,
    mlib_s32 n)
{
	mlib_s32 i;
	mlib_s64 val_c0, val_c1;

	if (shift < 0) {
		if (shift < -31)
			shift = -31;
		val_c0 = (((mlib_u16)c[0] << 16) >> (-shift)) >> 15;
		val_c1 = (((mlib_u16)c[1] << 16) >> (-shift)) >> 15;
	} else {
		if (shift > 31)
			shift = 31;
		val_c0 = (c[0] * ((mlib_u64)1 << shift)) << 1;
		val_c1 = (c[1] * ((mlib_u64)1 << shift)) << 1;
	}

	for (i = 0; i < n; i++) {
		mlib_s32 a, b;
		mlib_s64 dd1, dd2;
		a = ((mlib_u16 *)src1)[2 * i] << 16;
		b = ((mlib_u16 *)src1)[2 * i + 1] << 16;
		dd1 = (a + src2[2 * i] * val_c0) >> 16;
		dd2 = (b + src2[2 * i + 1] * val_c1) >> 16;
		dst[2 * i] = SATURATE(dd1);
		dst[2 * i + 1] = SATURATE(dd2);
	}

	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);
}

/* *********************************************************** */

#else /* !defined(_NO_LONGLONG) */

mlib_status
__mlib_SignalMulSShiftAdd_S16_S16_Sat(
    mlib_s16 *dst,
    const mlib_s16 *src1,
    const mlib_s16 *src2,
    const mlib_s16 *c,
    mlib_s32 shift,
    mlib_s32 n)
{
	mlib_s32 i;
	mlib_d64 val_c;
	mlib_d64 dd1, dd2;
	mlib_s32 a, b;

	if (shift < 0) {
		if (shift < -31)
			shift = -31;
		val_c = (((*c) << 16) >> (-shift));
		val_c *= (1. / 32768.);
	} else {
		if (shift > 31)
			shift = 31;
		val_c = (*c) * (mlib_d64)(1u << shift);
		val_c *= 2.;
	}

	for (i = 0; i < (n - 1); i += 2) {
		a = ((mlib_u16 *)src1)[i] << 16;
		b = ((mlib_u16 *)src1)[i + 1] << 16;
		dd1 = a + src2[i] * val_c;
		dd2 = b + src2[i + 1] * val_c;
		dst[i] = FLOAT2INT_CLAMP(dd1) >> 16;
		dst[i + 1] = FLOAT2INT_CLAMP(dd2) >> 16;
	}

	if (i < n) {
		a = ((mlib_u16 *)src1)[i] << 16;
		dst[i] = FLOAT2INT_CLAMP(a + src2[i] * val_c) >> 16;
	}

	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);
}

/* *********************************************************** */

mlib_status
__mlib_SignalMulSShiftAdd_S16S_S16S_Sat(
    mlib_s16 *dst,
    const mlib_s16 *src1,
    const mlib_s16 *src2,
    const mlib_s16 *c,
    mlib_s32 shift,
    mlib_s32 n)
{
	mlib_s32 i;
	mlib_d64 val_c0, val_c1;
	mlib_d64 dd1, dd2;
	mlib_s32 a, b;

	if (shift < 0) {
		if (shift < -31)
			shift = -31;
		val_c0 = ((c[0] << 16) >> (-shift));
		val_c1 = ((c[1] << 16) >> (-shift));
		val_c0 *= (1. / 32768.);
		val_c1 *= (1. / 32768.);
	} else {
		if (shift > 31)
			shift = 31;
		val_c0 = c[0] * (mlib_d64)(1u << shift);
		val_c1 = c[1] * (mlib_d64)(1u << shift);
		val_c0 *= 2.;
		val_c1 *= 2.;
	}

	for (i = 0; i < n; i++) {
		a = ((mlib_u16 *)src1)[2 * i] << 16;
		b = ((mlib_u16 *)src1)[2 * i + 1] << 16;
		dd1 = a + src2[2 * i] * val_c0;
		dd2 = b + src2[2 * i + 1] * val_c1;
		dst[2 * i] = FLOAT2INT_CLAMP(dd1) >> 16;
		dst[2 * i + 1] = FLOAT2INT_CLAMP(dd2) >> 16;
	}

	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);
}

#endif /* !defined(_NO_LONGLONG) */

/* *********************************************************** */
