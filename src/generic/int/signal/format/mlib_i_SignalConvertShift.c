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

#pragma ident	"@(#)mlib_i_SignalConvertShift.c	9.3	07/11/05 SMI"

/*
 * FUNCTIONS
 *      mlib_SignalConvertShift_[S32_S16|S32S_S16S]_Sat
 *      mlib_SignalConvertShift_[S16_S32|S16S_S32S]_Sat
 *      mlib_SignalConvertShift_[S32_S8|S32S_S8S]_Sat
 *      mlib_SignalConvertShift_[S8_S32|S8S_S32S]_Sat
 *      mlib_SignalConvertShift_[S16_S8|S16S_S8S]_Sat
 *      mlib_SignalConvertShift_[S8_S16|S8S_S16S]_Sat
 *      mlib_SignalConvertShift_[S32_U8|S32S_U8S]_Sat
 *      mlib_SignalConvertShift_[U8_S32|U8S_S32S]_Sat
 *      mlib_SignalConvertShift_[S16_U8|S16S_U8S]_Sat
 *      mlib_SignalConvertShift_[U8_S16|U8S_S16S]_Sat
 *      mlib_SignalConvertShift_[S8_U8|S8S_U8S]_Sat
 *      mlib_SignalConvertShift_[U8_S8|U8S_S8S]_Sat
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_SignalConvertShift_S16S_S32S_Sat(mlib_s16       *dst,
 *                                                        const mlib_s32 *src,
 *                                                        mlib_s32       shift,
 *                                                        mlib_s32       n);
 *      mlib_status mlib_SignalConvertShift_S16S_S8S_Sat(mlib_s16      *dst,
 *                                                       const mlib_s8 *src,
 *                                                       mlib_s32      shift,
 *                                                       mlib_s32      n);
 *      mlib_status mlib_SignalConvertShift_S16S_U8S_Sat(mlib_s16      *dst,
 *                                                       const mlib_u8 *src,
 *                                                       mlib_s32      shift,
 *                                                       mlib_s32      n);
 *      mlib_status mlib_SignalConvertShift_S16_S32_Sat(mlib_s16       *dst,
 *                                                      const mlib_s32 *src,
 *                                                      mlib_s32       shift,
 *                                                      mlib_s32       n);
 *      mlib_status mlib_SignalConvertShift_S16_S8_Sat(mlib_s16      *dst,
 *                                                     const mlib_s8 *src,
 *                                                     mlib_s32      shift,
 *                                                     mlib_s32      n);
 *      mlib_status mlib_SignalConvertShift_S16_U8_Sat(mlib_s16      *dst,
 *                                                     const mlib_u8 *src,
 *                                                     mlib_s32      shift,
 *                                                     mlib_s32      n);
 *      mlib_status mlib_SignalConvertShift_S32S_S16S_Sat(mlib_s32       *dst,
 *                                                        const mlib_s16 *src,
 *                                                        mlib_s32       shift,
 *                                                        mlib_s32       n);
 *      mlib_status mlib_SignalConvertShift_S32S_S8S_Sat(mlib_s32      *dst,
 *                                                       const mlib_s8 *src,
 *                                                       mlib_s32      shift,
 *                                                       mlib_s32      n);
 *      mlib_status mlib_SignalConvertShift_S32S_U8S_Sat(mlib_s32      *dst,
 *                                                       const mlib_u8 *src,
 *                                                       mlib_s32      shift,
 *                                                       mlib_s32      n);
 *      mlib_status mlib_SignalConvertShift_S32_S16_Sat(mlib_s32       *dst,
 *                                                      const mlib_s16 *src,
 *                                                      mlib_s32       shift,
 *                                                      mlib_s32       n);
 *      mlib_status mlib_SignalConvertShift_S32_S8_Sat(mlib_s32      *dst,
 *                                                     const mlib_s8 *src,
 *                                                     mlib_s32      shift,
 *                                                     mlib_s32      n);
 *      mlib_status mlib_SignalConvertShift_S32_U8_Sat(mlib_s32      *dst,
 *                                                     const mlib_u8 *src,
 *                                                     mlib_s32      shift,
 *                                                     mlib_s32      n);
 *      mlib_status mlib_SignalConvertShift_S8S_S16S_Sat(mlib_s8        *dst,
 *                                                       const mlib_s16 *src,
 *                                                       mlib_s32       shift,
 *                                                       mlib_s32       n);
 *      mlib_status mlib_SignalConvertShift_S8S_S32S_Sat(mlib_s8        *dst,
 *                                                       const mlib_s32 *src,
 *                                                       mlib_s32       shift,
 *                                                       mlib_s32       n);
 *      mlib_status mlib_SignalConvertShift_S8S_U8S_Sat(mlib_s8       *dst,
 *                                                      const mlib_u8 *src,
 *                                                      mlib_s32      shift,
 *                                                      mlib_s32      n);
 *      mlib_status mlib_SignalConvertShift_S8_S16_Sat(mlib_s8        *dst,
 *                                                     const mlib_s16 *src,
 *                                                     mlib_s32       shift,
 *                                                     mlib_s32       n);
 *      mlib_status mlib_SignalConvertShift_S8_S32_Sat(mlib_s8        *dst,
 *                                                     const mlib_s32 *src,
 *                                                     mlib_s32       shift,
 *                                                     mlib_s32       n);
 *      mlib_status mlib_SignalConvertShift_S8_U8_Sat(mlib_s8       *dst,
 *                                                    const mlib_u8 *src,
 *                                                    mlib_s32      shift,
 *                                                    mlib_s32      n);
 *      mlib_status mlib_SignalConvertShift_U8S_S16S_Sat(mlib_u8        *dst,
 *                                                       const mlib_s16 *src,
 *                                                       mlib_s32       shift,
 *                                                       mlib_s32       n);
 *      mlib_status mlib_SignalConvertShift_U8S_S32S_Sat(mlib_u8        *dst,
 *                                                       const mlib_s32 *src,
 *                                                       mlib_s32       shift,
 *                                                       mlib_s32       n);
 *      mlib_status mlib_SignalConvertShift_U8S_S8S_Sat(mlib_u8       *dst,
 *                                                      const mlib_s8 *src,
 *                                                      mlib_s32      shift,
 *                                                      mlib_s32      n);
 *      mlib_status mlib_SignalConvertShift_U8_S16_Sat(mlib_u8        *dst,
 *                                                     const mlib_s16 *src,
 *                                                     mlib_s32       shift,
 *                                                     mlib_s32       n);
 *      mlib_status mlib_SignalConvertShift_U8_S32_Sat(mlib_u8        *dst,
 *                                                     const mlib_s32 *src,
 *                                                     mlib_s32       shift,
 *                                                     mlib_s32       n);
 *      mlib_status mlib_SignalConvertShift_U8_S8_Sat(mlib_u8       *dst,
 *                                                    const mlib_s8 *src,
 *                                                    mlib_s32      shift,
 *                                                    mlib_s32      n);
 *
 * ARGUMENTS
 *      dst   Output signal array
 *      src   Input signal array
 *      shift Left shifting factor, if negative - shifts right
 *      n     Number of samples in the input signal arrays.
 *
 * DESCRIPTION
 *    - left shift of Q-format signal arrays
 *      dst = src * 2**shift;
 *      dst = saturate( dst );
 */

#include <mlib_signal.h>
#include <mlib_algebra.h>
#include <mlib_SignalConvertShift.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_SignalConvertShift_S16S_S32S_Sat = \
	__mlib_SignalConvertShift_S16S_S32S_Sat
#pragma weak mlib_SignalConvertShift_S16S_S8S_Sat = \
	__mlib_SignalConvertShift_S16S_S8S_Sat
#pragma weak mlib_SignalConvertShift_S16S_U8S_Sat = \
	__mlib_SignalConvertShift_S16S_U8S_Sat
#pragma weak mlib_SignalConvertShift_S16_S32_Sat = \
	__mlib_SignalConvertShift_S16_S32_Sat
#pragma weak mlib_SignalConvertShift_S16_S8_Sat = \
	__mlib_SignalConvertShift_S16_S8_Sat
#pragma weak mlib_SignalConvertShift_S16_U8_Sat = \
	__mlib_SignalConvertShift_S16_U8_Sat
#pragma weak mlib_SignalConvertShift_S32S_S16S_Sat = \
	__mlib_SignalConvertShift_S32S_S16S_Sat
#pragma weak mlib_SignalConvertShift_S32S_S8S_Sat = \
	__mlib_SignalConvertShift_S32S_S8S_Sat
#pragma weak mlib_SignalConvertShift_S32S_U8S_Sat = \
	__mlib_SignalConvertShift_S32S_U8S_Sat
#pragma weak mlib_SignalConvertShift_S32_S16_Sat = \
	__mlib_SignalConvertShift_S32_S16_Sat
#pragma weak mlib_SignalConvertShift_S32_S8_Sat = \
	__mlib_SignalConvertShift_S32_S8_Sat
#pragma weak mlib_SignalConvertShift_S32_U8_Sat = \
	__mlib_SignalConvertShift_S32_U8_Sat
#pragma weak mlib_SignalConvertShift_S8S_S16S_Sat = \
	__mlib_SignalConvertShift_S8S_S16S_Sat
#pragma weak mlib_SignalConvertShift_S8S_S32S_Sat = \
	__mlib_SignalConvertShift_S8S_S32S_Sat
#pragma weak mlib_SignalConvertShift_S8S_U8S_Sat = \
	__mlib_SignalConvertShift_S8S_U8S_Sat
#pragma weak mlib_SignalConvertShift_S8_S16_Sat = \
	__mlib_SignalConvertShift_S8_S16_Sat
#pragma weak mlib_SignalConvertShift_S8_S32_Sat = \
	__mlib_SignalConvertShift_S8_S32_Sat
#pragma weak mlib_SignalConvertShift_S8_U8_Sat = \
	__mlib_SignalConvertShift_S8_U8_Sat
#pragma weak mlib_SignalConvertShift_U8S_S16S_Sat = \
	__mlib_SignalConvertShift_U8S_S16S_Sat
#pragma weak mlib_SignalConvertShift_U8S_S32S_Sat = \
	__mlib_SignalConvertShift_U8S_S32S_Sat
#pragma weak mlib_SignalConvertShift_U8S_S8S_Sat = \
	__mlib_SignalConvertShift_U8S_S8S_Sat
#pragma weak mlib_SignalConvertShift_U8_S16_Sat = \
	__mlib_SignalConvertShift_U8_S16_Sat
#pragma weak mlib_SignalConvertShift_U8_S32_Sat = \
	__mlib_SignalConvertShift_U8_S32_Sat
#pragma weak mlib_SignalConvertShift_U8_S8_Sat = \
	__mlib_SignalConvertShift_U8_S8_Sat

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_SignalConvertShift_S16S_S32S_Sat)
    mlib_SignalConvertShift_S16S_S32S_Sat
    __attribute__((weak, alias("__mlib_SignalConvertShift_S16S_S32S_Sat")));
__typeof__(__mlib_SignalConvertShift_S16S_S8S_Sat)
    mlib_SignalConvertShift_S16S_S8S_Sat
    __attribute__((weak, alias("__mlib_SignalConvertShift_S16S_S8S_Sat")));
__typeof__(__mlib_SignalConvertShift_S16S_U8S_Sat)
    mlib_SignalConvertShift_S16S_U8S_Sat
    __attribute__((weak, alias("__mlib_SignalConvertShift_S16S_U8S_Sat")));
__typeof__(__mlib_SignalConvertShift_S16_S32_Sat)
    mlib_SignalConvertShift_S16_S32_Sat
    __attribute__((weak, alias("__mlib_SignalConvertShift_S16_S32_Sat")));
__typeof__(__mlib_SignalConvertShift_S16_S8_Sat)
    mlib_SignalConvertShift_S16_S8_Sat
    __attribute__((weak, alias("__mlib_SignalConvertShift_S16_S8_Sat")));
__typeof__(__mlib_SignalConvertShift_S16_U8_Sat)
    mlib_SignalConvertShift_S16_U8_Sat
    __attribute__((weak, alias("__mlib_SignalConvertShift_S16_U8_Sat")));
__typeof__(__mlib_SignalConvertShift_S32S_S16S_Sat)
    mlib_SignalConvertShift_S32S_S16S_Sat
    __attribute__((weak, alias("__mlib_SignalConvertShift_S32S_S16S_Sat")));
__typeof__(__mlib_SignalConvertShift_S32S_S8S_Sat)
    mlib_SignalConvertShift_S32S_S8S_Sat
    __attribute__((weak, alias("__mlib_SignalConvertShift_S32S_S8S_Sat")));
__typeof__(__mlib_SignalConvertShift_S32S_U8S_Sat)
    mlib_SignalConvertShift_S32S_U8S_Sat
    __attribute__((weak, alias("__mlib_SignalConvertShift_S32S_U8S_Sat")));
__typeof__(__mlib_SignalConvertShift_S32_S16_Sat)
    mlib_SignalConvertShift_S32_S16_Sat
    __attribute__((weak, alias("__mlib_SignalConvertShift_S32_S16_Sat")));
__typeof__(__mlib_SignalConvertShift_S32_S8_Sat)
    mlib_SignalConvertShift_S32_S8_Sat
    __attribute__((weak, alias("__mlib_SignalConvertShift_S32_S8_Sat")));
__typeof__(__mlib_SignalConvertShift_S32_U8_Sat)
    mlib_SignalConvertShift_S32_U8_Sat
    __attribute__((weak, alias("__mlib_SignalConvertShift_S32_U8_Sat")));
__typeof__(__mlib_SignalConvertShift_S8S_S16S_Sat)
    mlib_SignalConvertShift_S8S_S16S_Sat
    __attribute__((weak, alias("__mlib_SignalConvertShift_S8S_S16S_Sat")));
__typeof__(__mlib_SignalConvertShift_S8S_S32S_Sat)
    mlib_SignalConvertShift_S8S_S32S_Sat
    __attribute__((weak, alias("__mlib_SignalConvertShift_S8S_S32S_Sat")));
__typeof__(__mlib_SignalConvertShift_S8S_U8S_Sat)
    mlib_SignalConvertShift_S8S_U8S_Sat
    __attribute__((weak, alias("__mlib_SignalConvertShift_S8S_U8S_Sat")));
__typeof__(__mlib_SignalConvertShift_S8_S16_Sat)
    mlib_SignalConvertShift_S8_S16_Sat
    __attribute__((weak, alias("__mlib_SignalConvertShift_S8_S16_Sat")));
__typeof__(__mlib_SignalConvertShift_S8_S32_Sat)
    mlib_SignalConvertShift_S8_S32_Sat
    __attribute__((weak, alias("__mlib_SignalConvertShift_S8_S32_Sat")));
__typeof__(__mlib_SignalConvertShift_S8_U8_Sat)
    mlib_SignalConvertShift_S8_U8_Sat
    __attribute__((weak, alias("__mlib_SignalConvertShift_S8_U8_Sat")));
__typeof__(__mlib_SignalConvertShift_U8S_S16S_Sat)
    mlib_SignalConvertShift_U8S_S16S_Sat
    __attribute__((weak, alias("__mlib_SignalConvertShift_U8S_S16S_Sat")));
__typeof__(__mlib_SignalConvertShift_U8S_S32S_Sat)
    mlib_SignalConvertShift_U8S_S32S_Sat
    __attribute__((weak, alias("__mlib_SignalConvertShift_U8S_S32S_Sat")));
__typeof__(__mlib_SignalConvertShift_U8S_S8S_Sat)
    mlib_SignalConvertShift_U8S_S8S_Sat
    __attribute__((weak, alias("__mlib_SignalConvertShift_U8S_S8S_Sat")));
__typeof__(__mlib_SignalConvertShift_U8_S16_Sat)
    mlib_SignalConvertShift_U8_S16_Sat
    __attribute__((weak, alias("__mlib_SignalConvertShift_U8_S16_Sat")));
__typeof__(__mlib_SignalConvertShift_U8_S32_Sat)
    mlib_SignalConvertShift_U8_S32_Sat
    __attribute__((weak, alias("__mlib_SignalConvertShift_U8_S32_Sat")));
__typeof__(__mlib_SignalConvertShift_U8_S8_Sat)
    mlib_SignalConvertShift_U8_S8_Sat
    __attribute__((weak, alias("__mlib_SignalConvertShift_U8_S8_Sat")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	SIZE	1024

#ifdef _NO_LONGLONG
#define	FLOAT2INT_CLAMP1_i FLOAT2INT_CLAMP1
#else
#define	FLOAT2INT_CLAMP1_i(X)                                     \
	(((X) >= MLIB_S32_MAX) ? MLIB_S32_MAX :                   \
	(((X) < MLIB_S32_MIN) ? MLIB_S32_MIN : ((mlib_s32)(X))))
#endif

/* *********************************************************** */

mlib_status
__mlib_SignalConvertShift_S32_S16_Sat(
    mlib_s32 *dst,
    const mlib_s16 *src,
    mlib_s32 shift,
    mlib_s32 n)
{
	mlib_s32 i;
#ifdef _NO_LONGLONG
	mlib_d64 d_val;
#else
	mlib_s64 d_val;
#endif
	mlib_s16 *psrc = (mlib_s16 *)src;
	mlib_s32 *pdst = (mlib_s32 *)dst;

/* ashift - means arith shift (not Q) */
	mlib_s32 ashift = shift + 16;

	if (n <= 0)
		return (MLIB_FAILURE);

	if (!src || !dst)
		return (MLIB_NULLPOINTER);

	if (shift <= 0) {
		if (shift < -31) {
			shift = -31;
			ashift = -15;
		}

		if ((mlib_addr)psrc & 2) {
			pdst[0] =
			    (ashift >=
			    0) ? (psrc[0] << ashift) : (psrc[0] >> (-ashift));
			--n;
			psrc++;
			pdst++;
		}

		if (ashift >= 0)
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < (n >> 1); i++) {
				mlib_s32 a = ((mlib_s32 *)psrc)[i];

#ifdef _LITTLE_ENDIAN
				pdst[2 * i] = (a << 16) >> (-shift);
				pdst[2 * i + 1] = (a >> 16) << ashift;
#else /* _LITTLE_ENDIAN */
				pdst[2 * i] = (a >> 16) << ashift;
				pdst[2 * i + 1] = (a << 16) >> (-shift);
#endif /* _LITTLE_ENDIAN */
		} else	/* if ( ashift < 0) */
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < (n >> 1); i++) {
				mlib_s32 a = ((mlib_s32 *)psrc)[i];

#ifdef _LITTLE_ENDIAN
				pdst[2 * i] = (a << 16) >> (-shift);
				pdst[2 * i + 1] = a >> (-shift);
#else /* _LITTLE_ENDIAN */
				pdst[2 * i] = a >> (-shift);
				pdst[2 * i + 1] = (a << 16) >> (-shift);
#endif /* _LITTLE_ENDIAN */
			}

		i <<= 1;

		if (i < n)
			pdst[i] =
			    (ashift >=
			    0) ? (psrc[i] << ashift) : (psrc[i] >> (-ashift));
	} else {
		if (shift > 15)
			ashift = 31;
		d_val = ((mlib_u32)1) << ashift;

		if ((mlib_addr)psrc & 2) {
			pdst[0] = FLOAT2INT_CLAMP1_i(d_val * psrc[0]);
			--n;
			psrc++;
			pdst++;
		}
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < (n >> 1); i++) {
			mlib_s32 a = ((mlib_s32 *)psrc)[i];

#ifdef _LITTLE_ENDIAN
			pdst[2 * i] =
			    FLOAT2INT_CLAMP1_i(d_val * ((a << 16) >> 16));
			pdst[2 * i + 1] = FLOAT2INT_CLAMP1_i(d_val * (a >> 16));

#else /* _LITTLE_ENDIAN */
			pdst[2 * i] = FLOAT2INT_CLAMP1_i(d_val * (a >> 16));
			pdst[2 * i + 1] =
			    FLOAT2INT_CLAMP1_i(d_val * ((a << 16) >> 16));
#endif /* _LITTLE_ENDIAN */
		}

		if (2 * i < n)
			pdst[2 * i] = FLOAT2INT_CLAMP1_i(d_val * psrc[2 * i]);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_SignalConvertShift_S16_S32_Sat(
    mlib_s16 *dst,
    const mlib_s32 *src,
    mlib_s32 shift,
    mlib_s32 n)
{
	mlib_s32 i, tmp1, tmp2, tmp3, tmp4;
#ifdef _NO_LONGLONG
	mlib_d64 d_val;
#else
	mlib_s64 d_val;
#endif
	mlib_s32 *psrc = (mlib_s32 *)src;
	mlib_s16 *pdst = (mlib_s16 *)dst;

/* ashift - means right arith shift (not Q) */
	mlib_s32 ashift = 16 - shift;

	if (n <= 0)
		return (MLIB_FAILURE);

	if (!src || !dst)
		return (MLIB_NULLPOINTER);

	if (shift <= 0) {
		if (shift < -15) {
			shift = -15;
			ashift = 31;
		}

		if ((mlib_addr)pdst & 2) {
			pdst[0] = psrc[0] >> ashift;
			--n;
			psrc++;
			pdst++;
		}
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < (n >> 1); i++) {
#ifdef _LITTLE_ENDIAN
			((mlib_s32 *)pdst)[i] =
			    ((psrc[2 * i +
			    1] >> ashift) << 16) | ((psrc[2 *
			    i] >> ashift) & 0xffff);

#else /* _LITTLE_ENDIAN */

			((mlib_s32 *)pdst)[i] =
			    ((psrc[2 * i] >> ashift) << 16) | ((psrc[2 * i +
			    1] >> ashift) & 0xffff);
#endif /* _LITTLE_ENDIAN */
		}

		i <<= 1;

		if (i < n)
			pdst[i] = psrc[i] >> ashift;
	} else {
		if (shift > 31)
			shift = 31;

		d_val = ((mlib_u32)1) << shift;

		if ((mlib_addr)pdst & 2) {
			pdst[0] = FLOAT2INT_CLAMP1_i(d_val * psrc[0]) >> 16;
			--n;
			psrc++;
			pdst++;
		}
		i = 0;
		if (n > 3) {
#ifdef _LITTLE_ENDIAN
		    tmp1 =
			(FLOAT2INT_CLAMP1_i(d_val * psrc[1]) & 0xffff0000) |
			((FLOAT2INT_CLAMP1_i(d_val * psrc[0]) >> 16) & 0xffff);
		    tmp2 =
			(FLOAT2INT_CLAMP1_i(d_val * psrc[3]) & 0xffff0000) |
			((FLOAT2INT_CLAMP1_i(d_val * psrc[2]) >> 16) & 0xffff);
#else /* _LITTLE_ENDIAN */
		    tmp1 =
			(FLOAT2INT_CLAMP1_i(d_val * psrc[0]) & 0xffff0000) |
			((FLOAT2INT_CLAMP1_i(d_val * psrc[1]) >> 16) & 0xffff);
		    tmp2 =
			(FLOAT2INT_CLAMP1_i(d_val * psrc[2]) & 0xffff0000) |
			((FLOAT2INT_CLAMP1_i(d_val * psrc[3]) >> 16) & 0xffff);
#endif /* _LITTLE_ENDIAN */
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (; i < (n >> 2) - 1; i++) {
			    tmp3 = tmp1;
			    tmp4 = tmp2;
#ifdef _LITTLE_ENDIAN
			    tmp1 =
				(FLOAT2INT_CLAMP1_i(d_val * psrc[4 * i +
				5]) & 0xffff0000) | ((FLOAT2INT_CLAMP1_i(d_val *
				psrc[4 * i + 4]) >> 16) & 0xffff);
			    tmp2 =
				(FLOAT2INT_CLAMP1_i(d_val * psrc[4 * i +
				7]) & 0xffff0000) | ((FLOAT2INT_CLAMP1_i(d_val *
				psrc[4 * i + 6]) >> 16) & 0xffff);
#else /* _LITTLE_ENDIAN */
			    tmp1 =
				(FLOAT2INT_CLAMP1_i(d_val * psrc[4 * i +
				4]) & 0xffff0000) | ((FLOAT2INT_CLAMP1_i(d_val *
				psrc[4 * i + 5]) >> 16) & 0xffff);
			    tmp2 =
				(FLOAT2INT_CLAMP1_i(d_val * psrc[4 * i +
				6]) & 0xffff0000) | ((FLOAT2INT_CLAMP1_i(d_val *
				psrc[4 * i + 7]) >> 16) & 0xffff);
#endif /* _LITTLE_ENDIAN */
			    ((mlib_s32 *)pdst)[2 * i] = tmp3;
			    ((mlib_s32 *)pdst)[2 * i + 1] = tmp4;
			}
			((mlib_s32 *)pdst)[2 * i] = tmp1;
			((mlib_s32 *)pdst)[2 * i + 1] = tmp2;
			i++;
			i <<= 2;
		}

		for (; (i < n); i++)
			pdst[i] = FLOAT2INT_CLAMP1_i(d_val * psrc[i]) >> 16;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_SignalConvertShift_S8_S16_Sat(
    mlib_s8 *dst,
    const mlib_s16 *src,
    mlib_s32 shift,
    mlib_s32 n)
{
	mlib_s32 i;
#ifdef _NO_LONGLONG
	mlib_d64 d_val;
#else
	mlib_s64 d_val;
#endif
	mlib_s16 *psrc = (mlib_s16 *)src;
	mlib_s8 *pdst = (mlib_s8 *)dst;

/* ashift - means arith right shift (not Q) */
	mlib_s32 ashift = 8 - shift;
	mlib_s32 num, size;
	mlib_s32 buf[SIZE];

	if (n <= 0)
		return (MLIB_FAILURE);

	if (!src || !dst)
		return (MLIB_NULLPOINTER);

	if (shift <= 0) {
		if (shift < -7) {
			shift = -7;
			ashift = 15;
		}

		while (n > 0 && (mlib_addr)pdst & 3) {
			pdst[0] = psrc[0] >> ashift;
			--n;
			psrc++;
			pdst++;
		}

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < (n >> 2); i++) {
			mlib_s32 a0, a1;

#ifdef _LITTLE_ENDIAN
			a0 = ((psrc[4 * i + 3] >> (ashift - 8)) & 0xff00) |
			    ((psrc[4 * i + 2] >> ashift) & 0xff);
			a1 = ((psrc[4 * i + 1] >> (ashift - 8)) & 0xff00) |
			    ((psrc[4 * i] >> ashift) & 0xff);
#else /* _LITTLE_ENDIAN */
			a0 = ((psrc[4 * i] >> (ashift - 8)) & 0xff00) |
			    ((psrc[4 * i + 1] >> ashift) & 0xff);
			a1 = ((psrc[4 * i + 2] >> (ashift - 8)) & 0xff00) |
			    ((psrc[4 * i + 3] >> ashift) & 0xff);
#endif /* _LITTLE_ENDIAN */
			((mlib_s32 *)pdst)[i] = (a0 << 16) | a1;
		}

		i <<= 2;
		while (i < n) {
			pdst[i] = psrc[i] >> ashift;
			++i;
		}

		return (MLIB_SUCCESS);
	}

	if (shift > 15) {
		shift = 15;
	}

	d_val = ((mlib_u32)1) << (shift + 16);

	while (n > 0 && (mlib_addr)pdst & 3) {
		pdst[0] = FLOAT2INT_CLAMP1_i(d_val * psrc[0]) >> 24;
		--n;
		psrc++;
		pdst++;
	}

	for (size = 0; size < n; size += num) {
		num = n - size;

		if (num > SIZE)
			num = SIZE;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < num >> 2; i++) {
			buf[4 * i] = FLOAT2INT_CLAMP1_i(d_val * psrc[4 * i]);
			buf[4 * i + 1] =
			    FLOAT2INT_CLAMP1_i(d_val * psrc[4 * i + 1]);
			buf[4 * i + 2] =
			    FLOAT2INT_CLAMP1_i(d_val * psrc[4 * i + 2]);
			buf[4 * i + 3] =
			    FLOAT2INT_CLAMP1_i(d_val * psrc[4 * i + 3]);
		}

		i <<= 2;
		for (; i < num; i++)
			buf[i] = FLOAT2INT_CLAMP1_i(d_val * psrc[i]);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < num >> 2; i++) {
#ifdef _LITTLE_ENDIAN
			((mlib_s32 *)pdst)[i] =
			    (buf[4 * i + 3] & 0xff000000) | ((buf[4 * i +
			    2] >> 8) & 0xff0000) | ((buf[4 * i +
			    1] >> 16) & 0xff00) | ((mlib_u32)buf[4 * i] >> 24);
#else /* _LITTLE_ENDIAN */
			((mlib_s32 *)pdst)[i] =
			    (buf[4 * i] & 0xff000000) | ((buf[4 * i +
			    1] >> 8) & 0xff0000) | ((buf[4 * i +
			    2] >> 16) & 0xff00) | ((mlib_u32)buf[4 * i +
			    3] >> 24);
#endif /* _LITTLE_ENDIAN */
		}

		i <<= 2;
		for (; i < num; i++)
			pdst[i] = (buf[i] >> 24);
		psrc += num;
		pdst += num;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_SignalConvertShift_S16_S8_Sat(
    mlib_s16 *dst,
    const mlib_s8 *src,
    mlib_s32 shift,
    mlib_s32 n)
{
	mlib_s32 i;
	mlib_s8 *psrc = (mlib_s8 *)src;
	mlib_s16 *pdst = (mlib_s16 *)dst;

/* ashift - means arith shift (not Q) */
	mlib_s32 ashift = shift + 8;

	if (n <= 0)
		return (MLIB_FAILURE);

	if (!src || !dst)
		return (MLIB_NULLPOINTER);

	if (shift <= 0) {
		if (shift < -15) {
			shift = -15;
			ashift = -7;
		}

		while (n > 0 && (mlib_addr)psrc & 3) {
			pdst[0] =
			    ((ashift >=
			    0) ? (psrc[0] << ashift) : (psrc[0] >> (-ashift)));
			--n;
			psrc++;
			pdst++;
		}

		if (ashift > 0)
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < (n >> 2); i++) {
				mlib_s32 a = ((mlib_s32 *)psrc)[i];

#ifdef _LITTLE_ENDIAN
				pdst[4 * i] =
				    (mlib_s32)((a << 24) & 0xff000000) >> (24 -
				    ashift);
				pdst[4 * i + 1] =
				    (mlib_s32)((a << 16) & 0xff000000) >> (24 -
				    ashift);
				pdst[4 * i + 2] =
				    (mlib_s32)((a << 8) & 0xff000000) >> (24 -
				    ashift);
				pdst[4 * i + 3] =
				    (mlib_s32)(a & 0xff000000) >> (24 - ashift);
#else /* _LITTLE_ENDIAN */
				pdst[4 * i] =
				    (mlib_s32)(a & 0xff000000) >> (24 - ashift);
				pdst[4 * i + 1] =
				    (mlib_s32)((a << 8) & 0xff000000) >> (24 -
				    ashift);
				pdst[4 * i + 2] =
				    (mlib_s32)((a << 16) & 0xff000000) >> (24 -
				    ashift);
				pdst[4 * i + 3] =
				    (mlib_s32)((a << 24) & 0xff000000) >> (24 -
				    ashift);
#endif /* _LITTLE_ENDIAN */
		} else
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < (n >> 2); i++) {
				mlib_s32 a = ((mlib_s32 *)psrc)[i];

#ifdef _LITTLE_ENDIAN
				pdst[4 * i] = (a << 24) >> (24 - ashift);
				pdst[4 * i + 1] = (a << 16) >> (24 - ashift);
				pdst[4 * i + 2] = (a << 8) >> (24 - ashift);
				pdst[4 * i + 3] = a >> (24 - ashift);
#else /* _LITTLE_ENDIAN */
				pdst[4 * i] = a >> (24 - ashift);
				pdst[4 * i + 1] = (a << 8) >> (24 - ashift);
				pdst[4 * i + 2] = (a << 16) >> (24 - ashift);
				pdst[4 * i + 3] = (a << 24) >> (24 - ashift);
#endif /* _LITTLE_ENDIAN */
			}

		i <<= 2;
		while (i < n) {
			pdst[i] =
			    ((ashift >=
			    0) ? (psrc[i] << ashift) : (psrc[i] >> (-ashift)));
			++i;
		}

		return (MLIB_SUCCESS);
	}

	if (shift > 7) {
		shift = 7;
		ashift = 15;
	}
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < n >> 1; i++) {
		mlib_s32 x0, x1, y0, y1;
		mlib_s32 mask_sat0, mask_sat1;
		mlib_s32 val_sat0, val_sat1;

		x0 = psrc[2 * i];
		x1 = psrc[2 * i + 1];
		y0 = (x0 << ashift);
		y1 = (x1 << ashift);
		mask_sat0 = (0x7fff - (x0 ^ y0)) >> 31;
		mask_sat1 = (0x7fff - (x1 ^ y1)) >> 31;
		val_sat0 = 0x7fff ^ (x0 >> 31);
		val_sat1 = 0x7fff ^ (x1 >> 31);
		pdst[2 * i] = (val_sat0 & mask_sat0) | (y0 & ~mask_sat0);
		pdst[2 * i + 1] = (val_sat1 & mask_sat1) | (y1 & ~mask_sat1);
	}

	i <<= 1;
	for (; i < n; i++) {
		mlib_s32 x0, y0;
		mlib_s32 mask_sat0, val_sat0;

		x0 = psrc[i];
		y0 = (x0 << ashift);
		mask_sat0 = (0x7fff - (x0 ^ y0)) >> 31;
		val_sat0 = 0x7fff ^ (x0 >> 31);
		pdst[i] = (val_sat0 & mask_sat0) | (y0 & ~mask_sat0);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_SignalConvertShift_S8_S32_Sat(
    mlib_s8 *dst,
    const mlib_s32 *src,
    mlib_s32 shift,
    mlib_s32 n)
{
	mlib_s32 i;
#ifdef _NO_LONGLONG
	mlib_d64 d_val;
#else
	mlib_s64 d_val;
#endif
	mlib_s32 *psrc = (mlib_s32 *)src;
	mlib_s8 *pdst = (mlib_s8 *)dst;

/* ashift - means arith shift (not Q) */
	mlib_s32 ashift = 24 - shift;
	mlib_s32 num, size;
	mlib_s32 buf[SIZE];

	if (n <= 0)
		return (MLIB_FAILURE);

	if (!src || !dst)
		return (MLIB_NULLPOINTER);

	if (shift <= 0) {
		if (shift < -7) {
			shift = -7;
			ashift = 31;
		}
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < n; i++) {
			pdst[i] = psrc[i] >> ashift;
		}
	} else {
		mlib_s32 count = ((n < 4) ? n : (3 & (4 - (mlib_addr)dst)));

		if (shift > 31)
			shift = 31;
		d_val = ((mlib_u32)1) << shift;

		for (i = 0; i < count; ++i) {
			pdst[i] = FLOAT2INT_CLAMP1_i(d_val * psrc[i]) >> 24;
		}

		n -= count;
		psrc += count;
		pdst += count;

		for (size = 0; size < n; size += num) {
			num = n - size;

			if (num > SIZE)
				num = SIZE;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < num >> 2; i++) {
				buf[4 * i] =
				    FLOAT2INT_CLAMP1_i(d_val * psrc[4 * i]);
				buf[4 * i + 1] =
				    FLOAT2INT_CLAMP1_i(d_val * psrc[4 * i + 1]);
				buf[4 * i + 2] =
				    FLOAT2INT_CLAMP1_i(d_val * psrc[4 * i + 2]);
				buf[4 * i + 3] =
				    FLOAT2INT_CLAMP1_i(d_val * psrc[4 * i + 3]);
			}

			i <<= 2;
			for (; i < num; i++)
				buf[i] = FLOAT2INT_CLAMP1_i(d_val * psrc[i]);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < (num >> 2); i++) {

#ifdef _LITTLE_ENDIAN
				((mlib_s32 *)pdst)[i] =
				    (buf[4 * i +
				    3] & 0xff000000) | ((buf[4 * i +
				    2] >> 8) & 0xff0000) | ((buf[4 * i +
				    1] >> 16) & 0xff00) | ((mlib_u32)buf[4 *
				    i] >> 24);

#else /* _LITTLE_ENDIAN */
				((mlib_s32 *)pdst)[i] =
				    (buf[4 * i] & 0xff000000) | ((buf[4 * i +
				    1] >> 8) & 0xff0000) | ((buf[4 * i +
				    2] >> 16) & 0xff00) | ((mlib_u32)buf[4 * i +
				    3] >> 24);
#endif /* _LITTLE_ENDIAN */
			}

			i <<= 2;
			while (i < num) {
				pdst[i] = (buf[i] >> 24);
				++i;
			}

			psrc += num;
			pdst += num;
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_SignalConvertShift_S32_S8_Sat(
    mlib_s32 *dst,
    const mlib_s8 *src,
    mlib_s32 shift,
    mlib_s32 n)
{
	mlib_s32 i;
#ifdef _NO_LONGLONG
	mlib_d64 d_val;
#else
	mlib_s64 d_val;
#endif
	mlib_s8 *psrc = (mlib_s8 *)src;
	mlib_s32 *pdst = (mlib_s32 *)dst;

/* ashift - means arith shift (not Q) */
	mlib_s32 ashift = shift + 24;

	if (n <= 0)
		return (MLIB_FAILURE);

	if (!src || !dst)
		return (MLIB_NULLPOINTER);

	if (shift <= 0) {
		if (shift < -31) {
			shift = -31;
			ashift = -7;
		}

		while (n > 0 && (mlib_addr)psrc & 3) {
			pdst[0] =
			    ((ashift >=
			    0) ? (psrc[0] << ashift) : (psrc[0] >> (-ashift)));
			--n;
			psrc++;
			pdst++;
		}

		if (ashift >= 0)
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < (n >> 2); i++) {
				mlib_s32 a = ((mlib_s32 *)psrc)[i];

#ifdef _LITTLE_ENDIAN
				pdst[4 * i] = (a << 24) >> (-shift);
				pdst[4 * i + 1] =
				    (mlib_s32)((a & 0xff00) << 16) >> (-shift);
				pdst[4 * i + 2] =
				    (mlib_s32)((a & 0xff0000) << 8) >> (-shift);
				pdst[4 * i + 3] =
				    (mlib_s32)(a & 0xff000000) >> (-shift);
#else /* _LITTLE_ENDIAN */
				pdst[4 * i] =
				    (mlib_s32)(a & 0xff000000) >> (-shift);
				pdst[4 * i + 1] =
				    (mlib_s32)((a & 0xff0000) << 8) >> (-shift);
				pdst[4 * i + 2] =
				    (mlib_s32)((a & 0xff00) << 16) >> (-shift);
				pdst[4 * i + 3] = (a << 24) >> (-shift);
#endif /* _LITTLE_ENDIAN */
		} else
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < (n >> 2); i++) {
				mlib_s32 a = ((mlib_s32 *)psrc)[i];

#ifdef _LITTLE_ENDIAN
				pdst[4 * i] = (a << 24) >> (-shift);
				pdst[4 * i + 1] = (a << 16) >> (-shift);
				pdst[4 * i + 2] = (a << 8) >> (-shift);
				pdst[4 * i + 3] = a >> (-shift);

#else /* _LITTLE_ENDIAN */
				pdst[4 * i] = a >> (-shift);
				pdst[4 * i + 1] = (a << 8) >> (-shift);
				pdst[4 * i + 2] = (a << 16) >> (-shift);
				pdst[4 * i + 3] = (a << 24) >> (-shift);
#endif /* _LITTLE_ENDIAN */
			}

		i <<= 2;
		while (i < n) {
			pdst[i] =
			    ((ashift >=
			    0) ? (psrc[i] << ashift) : (psrc[i] >> (-ashift)));
			++i;
		}

		return (MLIB_SUCCESS);
	}

	if (shift > 7) {
		shift = 7;
		ashift = 31;
	}

	d_val = ((mlib_u32)1) << ashift;

	while (n > 0 && (mlib_addr)psrc & 3) {
		pdst[0] = FLOAT2INT_CLAMP1_i(d_val * psrc[0]);
		--n;
		psrc++;
		pdst++;
	}

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < (n >> 2); i++) {
		mlib_s32 a = ((mlib_s32 *)psrc)[i];

#ifdef _LITTLE_ENDIAN
		pdst[4 * i] = FLOAT2INT_CLAMP1_i(d_val * ((a << 24) >> 24));
		pdst[4 * i + 1] = FLOAT2INT_CLAMP1_i(d_val * ((a << 16) >> 24));
		pdst[4 * i + 2] = FLOAT2INT_CLAMP1_i(d_val * ((a << 8) >> 24));
		pdst[4 * i + 3] = FLOAT2INT_CLAMP1_i(d_val * (a >> 24));
#else /* _LITTLE_ENDIAN */
		pdst[4 * i] = FLOAT2INT_CLAMP1_i(d_val * (a >> 24));
		pdst[4 * i + 1] = FLOAT2INT_CLAMP1_i(d_val * ((a << 8) >> 24));
		pdst[4 * i + 2] = FLOAT2INT_CLAMP1_i(d_val * ((a << 16) >> 24));
		pdst[4 * i + 3] = FLOAT2INT_CLAMP1_i(d_val * ((a << 24) >> 24));

#endif /* _LITTLE_ENDIAN */
	}

	i <<= 2;
	while (i < n) {
		pdst[i] = FLOAT2INT_CLAMP1_i(d_val * psrc[i]);
		++i;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

/* u8 */

mlib_status
__mlib_SignalConvertShift_U8_S8_Sat(
    mlib_u8 *dst,
    const mlib_s8 *src,
    mlib_s32 shift,
    mlib_s32 n)
{
	mlib_s32 i;
	mlib_s8 *psrc = (mlib_s8 *)src;
	mlib_u8 *pdst = (mlib_u8 *)dst;

/* ashift - means arith right shift (not Q) */
	mlib_s32 ashift = -(shift + 1);

	if (n <= 0)
		return (MLIB_FAILURE);

	if (!src || !dst)
		return (MLIB_NULLPOINTER);

	if (shift < 0) {
		if (shift < -8) {
			shift = -8;
			ashift = 7;
		}

		while (n > 0 && (mlib_addr)psrc & 3) {
			mlib_s32 t = psrc[0];

			pdst[0] = (t >> ashift) & ~(t >> 7);
			--n;
			psrc++;
			pdst++;
		}

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < (n >> 2); i++) {
			mlib_s32 t = ((mlib_s32 *)psrc)[i];

#ifdef _LITTLE_ENDIAN
			pdst[4 * i] =
			    ((t << 24) >> (24 + ashift)) & ~((t << 24) >> 31);
			pdst[4 * i + 1] =
			    ((t << 16) >> (24 + ashift)) & ~((t << 16) >> 31);
			pdst[4 * i + 2] =
			    ((t << 8) >> (24 + ashift)) & ~((t << 8) >> 31);
			pdst[4 * i + 3] = (t >> (24 + ashift)) & ~(t >> 31);

#else /* _LITTLE_ENDIAN */
			pdst[4 * i] = (t >> (24 + ashift)) & ~(t >> 31);
			pdst[4 * i + 1] =
			    ((t << 8) >> (24 + ashift)) & ~((t << 8) >> 31);
			pdst[4 * i + 2] =
			    ((t << 16) >> (24 + ashift)) & ~((t << 16) >> 31);
			pdst[4 * i + 3] =
			    ((t << 24) >> (24 + ashift)) & ~((t << 24) >> 31);
#endif /* _LITTLE_ENDIAN */
		}

		i <<= 2;
		while (i < n) {
			mlib_s32 t = psrc[i];

			pdst[i] = (t >> ashift) & ~(t >> 7);
			++i;
		}

		return (MLIB_SUCCESS);
	}

	if (shift > 7) {
		shift = 7;
	}

	ashift = shift + 1;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < n >> 1; i++) {
		mlib_s32 mask0, mask1;
		mlib_u32 t0, t1, t0_, t1_;

		t0 = ((mlib_u8 *)psrc)[2 * i];
		t1 = ((mlib_u8 *)psrc)[2 * i + 1];
		t0_ = t0 << ashift;
		t1_ = t1 << ashift;
		mask0 = (0xff - (mlib_s32)t0_) >> 31;
		mask1 = (0xff - (mlib_s32)t1_) >> 31;
		pdst[2 * i] = (t0_ | mask0) & ~((mlib_s32)(t0 << 24) >> 31);
		pdst[2 * i + 1] = (t1_ | mask1) & ~((mlib_s32)(t1 << 24) >> 31);
	}

	i <<= 1;
	for (; i < n; i++)
#ifdef _NO_LONGLONG
		pdst[i] =
		    (FLOAT2INT_CLAMP1(((mlib_d64)(1u << ashift) * psrc[i] -
		    128.) * 16777216.)
		    >> 24) + 128;
#else
		pdst[i] =
		    (FLOAT2INT_CLAMP1_i(((mlib_s64)(1u << ashift) * psrc[i] -
		    128) << 24) >> 24) + 128;
#endif
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_SignalConvertShift_S8_U8_Sat(
    mlib_s8 *dst,
    const mlib_u8 *src,
    mlib_s32 shift,
    mlib_s32 n)
{
	mlib_s32 i;
	mlib_u8 *psrc = (mlib_u8 *)src;
	mlib_s8 *pdst = (mlib_s8 *)dst;

/* ashift - means arith shift (not Q) */
	mlib_s32 ashift = shift - 1;

	if (n <= 0)
		return (MLIB_FAILURE);

	if (!src || !dst)
		return (MLIB_NULLPOINTER);

	if (shift <= 0) {
		if (shift <= -7) {
			return (__mlib_VectorZero_S8(pdst, n));
		}

		while (n > 0 && (mlib_addr)psrc & 3) {
			pdst[0] = psrc[0] >> (-ashift);
			--n;
			psrc++;
			pdst++;
		}

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < (n >> 2); i++) {
			mlib_u32 a = ((mlib_u32 *)psrc)[i];

#ifdef _LITTLE_ENDIAN
			pdst[4 * i] = (a << 24) >> (24 - ashift);
			pdst[4 * i + 1] = (a << 16) >> (24 - ashift);
			pdst[4 * i + 2] = (a << 8) >> (24 - ashift);
			pdst[4 * i + 3] = a >> (24 - ashift);
#else /* _LITTLE_ENDIAN */
			pdst[4 * i] = a >> (24 - ashift);
			pdst[4 * i + 1] = (a << 8) >> (24 - ashift);
			pdst[4 * i + 2] = (a << 16) >> (24 - ashift);
			pdst[4 * i + 3] = (a << 24) >> (24 - ashift);
#endif /* _LITTLE_ENDIAN */
		}

		i <<= 2;
		while (i < n) {
			pdst[i] = psrc[i] >> (-ashift);
			++i;
		}

		return (MLIB_SUCCESS);
	}

	if (shift > 8) {
		shift = 8;
		ashift = 7;
	}
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < n >> 2; i++) {
		mlib_u32 t0, t1, t2, t3;
		mlib_s32 mask0, mask1, mask2, mask3;

		t0 = psrc[4 * i] << ashift;
		t1 = psrc[4 * i + 1] << ashift;
		t2 = psrc[4 * i + 2] << ashift;
		t3 = psrc[4 * i + 3] << ashift;
		mask0 = (0x7f - ((mlib_s32)t0)) >> 31;
		mask1 = (0x7f - ((mlib_s32)t1)) >> 31;
		mask2 = (0x7f - ((mlib_s32)t2)) >> 31;
		mask3 = (0x7f - ((mlib_s32)t3)) >> 31;
		pdst[4 * i] = (t0 | mask0) & 0x7f;
		pdst[4 * i + 1] = (t1 | mask1) & 0x7f;
		pdst[4 * i + 2] = (t2 | mask2) & 0x7f;
		pdst[4 * i + 3] = (t3 | mask3) & 0x7f;
	}

	i <<= 2;
	for (; i < n; i++) {
		mlib_u32 t;
		mlib_s32 mask;

		t = psrc[i] << ashift;
		mask = (0x7f - ((mlib_s32)t)) >> 31;
		pdst[i] = (t | mask) & 0x7f;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_SignalConvertShift_U8_S16_Sat(
    mlib_u8 *dst,
    const mlib_s16 *src,
    mlib_s32 shift,
    mlib_s32 n)
{
	mlib_s32 i;
	mlib_s16 *psrc = (mlib_s16 *)src;
	mlib_u8 *pdst = (mlib_u8 *)dst;

/* ashift - means arith right shift (not Q) */
	mlib_s32 ashift = 7 - shift;

	if (n <= 0)
		return (MLIB_FAILURE);

	if (!src || !dst)
		return (MLIB_NULLPOINTER);

	if (shift <= 0) {
		if (shift < -8) {
			shift = -8;
			ashift = 15;
		}

		if ((mlib_addr)psrc & 3) {
			mlib_s32 t = psrc[0];

			pdst[0] = (t >> ashift) & ~(t >> 15);
			--n;
			psrc++;
			pdst++;
		}
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < (n >> 1); i++) {
			mlib_s32 t = ((mlib_s32 *)psrc)[i];

#ifdef _LITTLE_ENDIAN
			pdst[2 * i] =
			    ((t << 16) >> (16 + ashift)) & ~((t << 16) >> 31);
			pdst[2 * i + 1] = (t >> (16 + ashift)) & ~(t >> 31);
#else /* _LITTLE_ENDIAN */
			pdst[2 * i] = (t >> (16 + ashift)) & ~(t >> 31);
			pdst[2 * i + 1] =
			    ((t << 16) >> (16 + ashift)) & ~((t << 16) >> 31);
#endif /* _LITTLE_ENDIAN */
		}

		i <<= 1;

		if (i < n) {
			mlib_s32 t = psrc[i];

			pdst[i] = (t >> ashift) & ~(t >> 15);
			++i;
		}

		return (MLIB_SUCCESS);
	}

	if (shift > 15) {
		shift = 15;
	}

	ashift = shift - 7;

	if (ashift < 0) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < n >> 1; i++) {
			mlib_s32 mask0, mask1;
			mlib_u32 t0, t1, t0_, t1_;

			t0 = ((mlib_u16 *)psrc)[2 * i];
			t1 = ((mlib_u16 *)psrc)[2 * i + 1];
			t0_ = t0 >> -ashift;
			t1_ = t1 >> -ashift;
			mask0 = (0xff - (mlib_s32)t0_) >> 31;
			mask1 = (0xff - (mlib_s32)t1_) >> 31;
			pdst[2 * i] =
			    (t0_ | mask0) & ~((mlib_s32)(t0 << 16) >> 31);
			pdst[2 * i + 1] =
			    (t1_ | mask1) & ~((mlib_s32)(t1 << 16) >> 31);
		}

		i <<= 1;
		for (; i < n; i++) {
			mlib_s32 mask;
			mlib_u32 t0, t;

			t0 = ((mlib_u16 *)psrc)[i];
			t = t0 >> -ashift;
			mask = (0xff - (mlib_s32)t) >> 31;
			pdst[i] = (t | mask) & ~((mlib_s32)(t0 << 16) >> 31);
		}
	} else {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < n >> 1; i++) {
			mlib_u32 t0, t1, t0_, t1_;
			mlib_s32 mask0, mask1;

			t0 = ((mlib_u16 *)psrc)[2 * i];
			t1 = ((mlib_u16 *)psrc)[2 * i + 1];
			t0_ = t0 << ashift;
			t1_ = t1 << ashift;
			mask0 = (0xff - (mlib_s32)t0_) >> 31;
			mask1 = (0xff - (mlib_s32)t1_) >> 31;
			pdst[2 * i] =
			    (t0_ | mask0) & ~((mlib_s32)(t0 << 16) >> 31);
			pdst[2 * i + 1] =
			    (t1_ | mask1) & ~((mlib_s32)(t1 << 16) >> 31);
		}

		i <<= 1;
		for (; i < n; i++) {
			mlib_s32 mask;
			mlib_u32 t0, t;

			t0 = ((mlib_u16 *)psrc)[i];
			t = t0 << ashift;
			mask = (0xff - (mlib_s32)t) >> 31;
			pdst[i] = (t | mask) & ~((mlib_s32)(t0 << 16) >> 31);
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_SignalConvertShift_S16_U8_Sat(
    mlib_s16 *dst,
    const mlib_u8 *src,
    mlib_s32 shift,
    mlib_s32 n)
{
	mlib_s32 i;
	mlib_u8 *psrc = (mlib_u8 *)src;
	mlib_s16 *pdst = (mlib_s16 *)dst;

/* ashift - means arith shift (not Q) */
	mlib_s32 ashift = shift + 7;

	if (n <= 0)
		return (MLIB_FAILURE);

	if (!src || !dst)
		return (MLIB_NULLPOINTER);

	if (shift <= 0) {
		if (shift <= -15) {
/* set output to zero */
			return (__mlib_VectorZero_S16(pdst, n));
		}

		while (n > 0 && (mlib_addr)psrc & 3) {
			pdst[0] =
			    ((ashift >=
			    0) ? (psrc[0] << ashift) : (psrc[0] >> (-ashift)));
			--n;
			psrc++;
			pdst++;
		}

		if (ashift > 0)
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < (n >> 2); i++) {
				mlib_u32 a = ((mlib_u32 *)psrc)[i];

#ifdef _LITTLE_ENDIAN
				pdst[4 * i] = (a & 0xff) << ashift;
				pdst[4 * i + 1] = ((a & 0xff00) >> 8) << ashift;
				pdst[4 * i + 2] =
				    ((a & 0xff0000) >> 16) << ashift;
				pdst[4 * i + 3] = (a >> 24) << ashift;
#else /* _LITTLE_ENDIAN */
				pdst[4 * i] = (a >> 24) << ashift;
				pdst[4 * i + 1] =
				    ((a & 0xff0000) >> 16) << ashift;
				pdst[4 * i + 2] = ((a & 0xff00) >> 8) << ashift;
				pdst[4 * i + 3] = (a & 0xff) << ashift;
#endif /* _LITTLE_ENDIAN */
		} else
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < (n >> 2); i++) {
				mlib_u32 a = ((mlib_u32 *)psrc)[i];

#ifdef _LITTLE_ENDIAN
				pdst[4 * i] = (a << 24) >> (24 - ashift);
				pdst[4 * i + 1] = (a << 16) >> (24 - ashift);
				pdst[4 * i + 2] = (a << 8) >> (24 - ashift);
				pdst[4 * i + 3] = a >> (24 - ashift);
#else /* _LITTLE_ENDIAN */
				pdst[4 * i] = a >> (24 - ashift);
				pdst[4 * i + 1] = (a << 8) >> (24 - ashift);
				pdst[4 * i + 2] = (a << 16) >> (24 - ashift);
				pdst[4 * i + 3] = (a << 24) >> (24 - ashift);
#endif /* _LITTLE_ENDIAN */
			}

		i <<= 2;
		while (i < n) {
			pdst[i] =
			    ((ashift >=
			    0) ? (psrc[i] << ashift) : (psrc[i] >> (-ashift)));
			++i;
		}

		return (MLIB_SUCCESS);
	}

	if (shift > 8) {
		shift = 8;
		ashift = 15;
	}
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < n >> 2; i++) {
		mlib_u32 t0, t1, t2, t3;
		mlib_s32 mask0, mask1, mask2, mask3;

		t0 = psrc[4 * i] << ashift;
		t1 = psrc[4 * i + 1] << ashift;
		t2 = psrc[4 * i + 2] << ashift;
		t3 = psrc[4 * i + 3] << ashift;
		mask0 = (0x7fff - ((mlib_s32)t0)) >> 31;
		mask1 = (0x7fff - ((mlib_s32)t1)) >> 31;
		mask2 = (0x7fff - ((mlib_s32)t2)) >> 31;
		mask3 = (0x7fff - ((mlib_s32)t3)) >> 31;
		pdst[4 * i] = (t0 | mask0) & 0x7fff;
		pdst[4 * i + 1] = (t1 | mask1) & 0x7fff;
		pdst[4 * i + 2] = (t2 | mask2) & 0x7fff;
		pdst[4 * i + 3] = (t3 | mask3) & 0x7fff;
	}

	i <<= 2;
	for (; i < n; i++) {
		mlib_s32 mask;
		mlib_u32 t;

		t = psrc[i] << ashift;
		mask = (0x7fff - (mlib_s32)t) >> 31;
		pdst[i] = (t | mask) & 0x7fff;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_SignalConvertShift_U8_S32_Sat(
    mlib_u8 *dst,
    const mlib_s32 *src,
    mlib_s32 shift,
    mlib_s32 n)
{
	mlib_s32 i;
#ifdef _NO_LONGLONG
	mlib_d64 d_val;
#else
	mlib_s64 d_val;
#endif
	mlib_s32 *psrc = (mlib_s32 *)src;
	mlib_u8 *pdst = (mlib_u8 *)dst;

/* ashift - means arith shift (not Q) */
	mlib_s32 ashift = 23 - shift;

	if (n <= 0)
		return (MLIB_FAILURE);

	if (!src || !dst)
		return (MLIB_NULLPOINTER);

	if (shift <= 0) {
		if (shift < -8) {
			shift = -8;
			ashift = 31;
		}
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < (n >> 1); i++) {
			pdst[2 * i] =
			    (psrc[2 * i] >> ashift) & ~(psrc[2 * i] >> 31);
			pdst[2 * i + 1] =
			    (psrc[2 * i + 1] >> ashift) & ~(psrc[2 * i +
			    1] >> 31);
		}

		i <<= 1;

		if (i < n) {
			mlib_s32 t = psrc[i];

			pdst[i] = (t >> ashift) & ~(t >> 31);
			++i;
		}
	} else {
		if (shift > 31)
			shift = 31;
		d_val = ((mlib_u32)1) << (shift);

		if ((mlib_addr)pdst & 1) {
			pdst[0] =
			    (FLOAT2INT_CLAMP1_i(d_val *
			    psrc[0]) >> 23) & ~(psrc[0] >> 31);
			--n;
			++psrc;
			++pdst;
		}
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < (n >> 1); i++) {
#ifdef _LITTLE_ENDIAN
			((mlib_u16 *)pdst)[i] =
			    ((FLOAT2INT_CLAMP1_i(d_val * psrc[2*i + 1]) >> 15) &
			    ~((psrc[2 * i + 1] >> 31) | 0xffff00ff)) |
			    ((FLOAT2INT_CLAMP1_i(d_val * psrc[2 *
			    i]) >> 23) & ~(psrc[2 * i] >> 31));
#else /* _LITTLE_ENDIAN */
			((mlib_u16 *)pdst)[i] =
			    ((FLOAT2INT_CLAMP1_i(d_val * psrc[2 * i]) >> 15) &
			    ~((psrc[2 * i] >> 31) | 0xffff00ff)) |
			    ((FLOAT2INT_CLAMP1_i(d_val * psrc[2*i + 1]) >> 23) &
			    ~(psrc[2 * i + 1] >> 31));
#endif /* _LITTLE_ENDIAN */
		}

		i <<= 1;

		if (i < n) {
			pdst[i] =
			    (FLOAT2INT_CLAMP1_i(d_val *
			    psrc[i]) >> 23) & ~(psrc[i] >> 31);
			++i;
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_SignalConvertShift_S32_U8_Sat(
    mlib_s32 *dst,
    const mlib_u8 *src,
    mlib_s32 shift,
    mlib_s32 n)
{
	mlib_s32 i;
#ifdef _NO_LONGLONG
	mlib_d64 d_val;
#else
	mlib_s64 d_val;
#endif
	mlib_u8 *psrc = (mlib_u8 *)src;
	mlib_s32 *pdst = (mlib_s32 *)dst;

/* ashift - means arith shift (not Q) */
	mlib_s32 ashift = shift + 23;

	if (n <= 0)
		return (MLIB_FAILURE);

	if (!src || !dst)
		return (MLIB_NULLPOINTER);

	if (shift <= 0) {
		if (shift <= -31) {
/* set output to zero */
			return (__mlib_VectorZero_S32(pdst, n));
		}

		while (n > 0 && (mlib_addr)psrc & 3) {
			pdst[0] =
			    ((ashift >=
			    0) ? (psrc[0] << ashift) : (psrc[0] >> (-ashift)));
			--n;
			psrc++;
			pdst++;
		}

		if (ashift >= 0)
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < (n >> 2); i++) {
				mlib_u32 a = ((mlib_u32 *)psrc)[i];

#ifdef _LITTLE_ENDIAN
				pdst[4 * i] = (a & 0xff) << ashift;
				pdst[4 * i + 1] =
				    ((a & 0xff00) << 16) >> (24 - ashift);
				pdst[4 * i + 2] =
				    ((a & 0xff0000) << 8) >> (24 - ashift);
				pdst[4 * i + 3] =
				    (a & 0xff000000) >> (24 - ashift);
#else /* _LITTLE_ENDIAN */
				pdst[4 * i] = (a & 0xff000000) >> (24 - ashift);
				pdst[4 * i + 1] =
				    ((a & 0xff0000) << 8) >> (24 - ashift);
				pdst[4 * i + 2] =
				    ((a & 0xff00) << 16) >> (24 - ashift);
				pdst[4 * i + 3] = (a & 0xff) << ashift;
#endif /* _LITTLE_ENDIAN */
		} else
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < (n >> 2); i++) {
				mlib_u32 a = ((mlib_u32 *)psrc)[i];

#ifdef _LITTLE_ENDIAN
				pdst[4 * i] = (a << 24) >> (1 - shift);
				pdst[4 * i + 1] = (a << 16) >> (1 - shift);
				pdst[4 * i + 2] = (a << 8) >> (1 - shift);
				pdst[4 * i + 3] = a >> (1 - shift);
#else /* _LITTLE_ENDIAN */
				pdst[4 * i] = a >> (1 - shift);
				pdst[4 * i + 1] = (a << 8) >> (1 - shift);
				pdst[4 * i + 2] = (a << 16) >> (1 - shift);
				pdst[4 * i + 3] = (a & 0xff) >> (-ashift);
#endif /* _LITTLE_ENDIAN */
			}

		i <<= 2;
		while (i < n) {
			pdst[i] =
			    ((ashift >=
			    0) ? (psrc[i] << ashift) : (psrc[i] >> (-ashift)));
			++i;
		}

		return (MLIB_SUCCESS);
	}

	if (shift > 8) {
		shift = 8;
		ashift = 31;
	}

	d_val = ((mlib_u32)1) << ashift;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < (n >> 1); i++) {
		pdst[2 * i + 0] = FLOAT2INT_CLAMP1_i(d_val * psrc[2 * i]);
		pdst[2 * i + 1] = FLOAT2INT_CLAMP1_i(d_val * psrc[2 * i + 1]);
	}

	i <<= 1;

	if (i < n) {
		pdst[i] = FLOAT2INT_CLAMP1_i(d_val * psrc[i]);
		++i;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

/* STEREO */

/* *********************************************************** */

mlib_status
__mlib_SignalConvertShift_S8S_S16S_Sat(
    mlib_s8 *dst,
    const mlib_s16 *src,
    mlib_s32 shift,
    mlib_s32 n)
{
	return (__mlib_SignalConvertShift_S8_S16_Sat(dst, src, shift, n << 1));
}

/* *********************************************************** */

mlib_status
__mlib_SignalConvertShift_S16S_S8S_Sat(
    mlib_s16 *dst,
    const mlib_s8 *src,
    mlib_s32 shift,
    mlib_s32 n)
{
	return (__mlib_SignalConvertShift_S16_S8_Sat(dst, src, shift, n << 1));
}

/* *********************************************************** */

mlib_status
__mlib_SignalConvertShift_S8S_S32S_Sat(
    mlib_s8 *dst,
    const mlib_s32 *src,
    mlib_s32 shift,
    mlib_s32 n)
{
	return (__mlib_SignalConvertShift_S8_S32_Sat(dst, src, shift, n << 1));
}

/* *********************************************************** */

mlib_status
__mlib_SignalConvertShift_S32S_S8S_Sat(
    mlib_s32 *dst,
    const mlib_s8 *src,
    mlib_s32 shift,
    mlib_s32 n)
{
	return (__mlib_SignalConvertShift_S32_S8_Sat(dst, src, shift, n << 1));
}

/* *********************************************************** */

mlib_status
__mlib_SignalConvertShift_S32S_S16S_Sat(
    mlib_s32 *dst,
    const mlib_s16 *src,
    mlib_s32 shift,
    mlib_s32 n)
{
	return (__mlib_SignalConvertShift_S32_S16_Sat(dst, src, shift, n << 1));
}

/* *********************************************************** */

mlib_status
__mlib_SignalConvertShift_S16S_S32S_Sat(
    mlib_s16 *dst,
    const mlib_s32 *src,
    mlib_s32 shift,
    mlib_s32 n)
{
	return (__mlib_SignalConvertShift_S16_S32_Sat(dst, src, shift, n << 1));
}

/* u8 */

mlib_status
__mlib_SignalConvertShift_U8S_S16S_Sat(
    mlib_u8 *dst,
    const mlib_s16 *src,
    mlib_s32 shift,
    mlib_s32 n)
{
	return (__mlib_SignalConvertShift_U8_S16_Sat(dst, src, shift, n << 1));
}

/* *********************************************************** */

mlib_status
__mlib_SignalConvertShift_S16S_U8S_Sat(
    mlib_s16 *dst,
    const mlib_u8 *src,
    mlib_s32 shift,
    mlib_s32 n)
{
	return (__mlib_SignalConvertShift_S16_U8_Sat(dst, src, shift, n << 1));
}

/* *********************************************************** */

mlib_status
__mlib_SignalConvertShift_U8S_S32S_Sat(
    mlib_u8 *dst,
    const mlib_s32 *src,
    mlib_s32 shift,
    mlib_s32 n)
{
	return (__mlib_SignalConvertShift_U8_S32_Sat(dst, src, shift, n << 1));
}

/* *********************************************************** */

mlib_status
__mlib_SignalConvertShift_S32S_U8S_Sat(
    mlib_s32 *dst,
    const mlib_u8 *src,
    mlib_s32 shift,
    mlib_s32 n)
{
	return (__mlib_SignalConvertShift_S32_U8_Sat(dst, src, shift, n << 1));
}

/* *********************************************************** */

mlib_status
__mlib_SignalConvertShift_S8S_U8S_Sat(
    mlib_s8 *dst,
    const mlib_u8 *src,
    mlib_s32 shift,
    mlib_s32 n)
{
	return (__mlib_SignalConvertShift_S8_U8_Sat(dst, src, shift, n << 1));
}

/* *********************************************************** */

mlib_status
__mlib_SignalConvertShift_U8S_S8S_Sat(
    mlib_u8 *dst,
    const mlib_s8 *src,
    mlib_s32 shift,
    mlib_s32 n)
{
	return (__mlib_SignalConvertShift_U8_S8_Sat(dst, src, shift, n << 1));
}

/* *********************************************************** */
