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

#pragma ident	"@(#)mlib_s_SignalConvertShift.c	9.2	07/11/05 SMI"

/*
 * FUNCTIONS
 *      mlib_SignalConvertShift_[S32_S16|S32S_S16S]_Sat,
 *      mlib_SignalConvertShift_[S16_S32|S16S_S32S]_Sat,
 *      mlib_SignalConvertShift_[S32_S8|S32S_S8S]_Sat,
 *      mlib_SignalConvertShift_[S8_S32|S8S_S32S]_Sat,
 *      mlib_SignalConvertShift_[S16_S8|S16S_S8S]_Sat,
 *      mlib_SignalConvertShift_[S8_S16|S8S_S16S]_Sat,
 *      mlib_SignalConvertShift_[S32_U8|S32S_U8S]_Sat,
 *      mlib_SignalConvertShift_[U8_S32|U8S_S32S]_Sat,
 *      mlib_SignalConvertShift_[S16_U8|S16S_U8S]_Sat,
 *      mlib_SignalConvertShift_[U8_S16|U8S_S16S]_Sat,
 *      mlib_SignalConvertShift_[S8_U8|S8S_U8S]_Sat,
 *      mlib_SignalConvertShift_[U8_S8|U8S_S8S]_Sat,
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
#ifdef	__SUNPRO_C
#include <sunmedia_intrin.h>
#else
#include <emmintrin.h>
#endif

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_SignalConvertShift_S16S_S32S_Sat = \
	__mlib_SignalConvertShift_S16S_S32S_Sat
#pragma weak mlib_SignalConvertShift_S16S_S8S_Sat = \
	__mlib_SignalConvertShift_S16S_S8S_Sat
#pragma weak mlib_SignalConvertShift_S16S_U8S_Sat = \
	__mlib_SignalConvertShift_S16S_U8S_Sat
#pragma weak mlib_SignalConvertShift_S32S_S16S_Sat = \
	__mlib_SignalConvertShift_S32S_S16S_Sat
#pragma weak mlib_SignalConvertShift_S32S_S8S_Sat = \
	__mlib_SignalConvertShift_S32S_S8S_Sat
#pragma weak mlib_SignalConvertShift_S32S_U8S_Sat = \
	__mlib_SignalConvertShift_S32S_U8S_Sat
#pragma weak mlib_SignalConvertShift_S8S_S16S_Sat = \
	__mlib_SignalConvertShift_S8S_S16S_Sat
#pragma weak mlib_SignalConvertShift_S8S_S32S_Sat = \
	__mlib_SignalConvertShift_S8S_S32S_Sat
#pragma weak mlib_SignalConvertShift_S8S_U8S_Sat = \
	__mlib_SignalConvertShift_S8S_U8S_Sat
#pragma weak mlib_SignalConvertShift_U8S_S16S_Sat = \
	__mlib_SignalConvertShift_U8S_S16S_Sat
#pragma weak mlib_SignalConvertShift_U8S_S32S_Sat = \
	__mlib_SignalConvertShift_U8S_S32S_Sat
#pragma weak mlib_SignalConvertShift_U8S_S8S_Sat = \
	__mlib_SignalConvertShift_U8S_S8S_Sat

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
__typeof__(__mlib_SignalConvertShift_S32S_S16S_Sat)
    mlib_SignalConvertShift_S32S_S16S_Sat
    __attribute__((weak, alias("__mlib_SignalConvertShift_S32S_S16S_Sat")));
__typeof__(__mlib_SignalConvertShift_S32S_S8S_Sat)
    mlib_SignalConvertShift_S32S_S8S_Sat
    __attribute__((weak, alias("__mlib_SignalConvertShift_S32S_S8S_Sat")));
__typeof__(__mlib_SignalConvertShift_S32S_U8S_Sat)
    mlib_SignalConvertShift_S32S_U8S_Sat
    __attribute__((weak, alias("__mlib_SignalConvertShift_S32S_U8S_Sat")));
__typeof__(__mlib_SignalConvertShift_S8S_S16S_Sat)
    mlib_SignalConvertShift_S8S_S16S_Sat
    __attribute__((weak, alias("__mlib_SignalConvertShift_S8S_S16S_Sat")));
__typeof__(__mlib_SignalConvertShift_S8S_S32S_Sat)
    mlib_SignalConvertShift_S8S_S32S_Sat
    __attribute__((weak, alias("__mlib_SignalConvertShift_S8S_S32S_Sat")));
__typeof__(__mlib_SignalConvertShift_S8S_U8S_Sat)
    mlib_SignalConvertShift_S8S_U8S_Sat
    __attribute__((weak, alias("__mlib_SignalConvertShift_S8S_U8S_Sat")));
__typeof__(__mlib_SignalConvertShift_U8S_S16S_Sat)
    mlib_SignalConvertShift_U8S_S16S_Sat
    __attribute__((weak, alias("__mlib_SignalConvertShift_U8S_S16S_Sat")));
__typeof__(__mlib_SignalConvertShift_U8S_S32S_Sat)
    mlib_SignalConvertShift_U8S_S32S_Sat
    __attribute__((weak, alias("__mlib_SignalConvertShift_U8S_S32S_Sat")));
__typeof__(__mlib_SignalConvertShift_U8S_S8S_Sat)
    mlib_SignalConvertShift_U8S_S8S_Sat
    __attribute__((weak, alias("__mlib_SignalConvertShift_U8S_S8S_Sat")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

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

/* *********************************************************** */

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
