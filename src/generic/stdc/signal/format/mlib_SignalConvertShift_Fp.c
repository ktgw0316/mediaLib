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

#pragma ident	"@(#)mlib_SignalConvertShift_Fp.c	9.2	07/10/09 SMI"

/*
 * FUNCTIONS
 *
 *      mlib_SignalConvertShift_[S32_F32|S32S_F32S]_Sat
 *
 *      mlib_SignalConvertShift_[F32_S32|F32S_S32S]
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_SignalConvertShift_F32S_S32S(mlib_f32       *dst,
 *                                                    const mlib_s32 *src,
 *                                                    mlib_s32       shift,
 *                                                    mlib_s32       n);
 *      mlib_status mlib_SignalConvertShift_F32_S32(mlib_f32       *dst,
 *                                                  const mlib_s32 *src,
 *                                                  mlib_s32       shift,
 *                                                  mlib_s32       n);
 *      mlib_status mlib_SignalConvertShift_S32S_F32S_Sat(mlib_s32       *dst,
 *                                                        const mlib_f32 *src,
 *                                                        mlib_s32       shift,
 *                                                        mlib_s32       n);
 *      mlib_status mlib_SignalConvertShift_S32_F32_Sat(mlib_s32       *dst,
 *                                                      const mlib_f32 *src,
 *                                                      mlib_s32       shift,
 *                                                      mlib_s32       n);
 * ARGUMENTS
 *      dst   Output signal array
 *      src   Input signal array
 *      shift Left shifting factor, if negative - shifts right
 *      n     Number of samples in the input signal arrays.
 *
 * DESCRIPTION
 *      left shift of Q-format signal arrays
 *      dst = src * 2**shift;
 *      dst = saturate( dst );
 */

#include <mlib_signal.h>
#include <mlib_SignalConvertShift.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_SignalConvertShift_F32S_S32S = \
	__mlib_SignalConvertShift_F32S_S32S
#pragma weak mlib_SignalConvertShift_F32_S32 = \
	__mlib_SignalConvertShift_F32_S32
#pragma weak mlib_SignalConvertShift_S32S_F32S_Sat = \
	__mlib_SignalConvertShift_S32S_F32S_Sat
#pragma weak mlib_SignalConvertShift_S32_F32_Sat = \
	__mlib_SignalConvertShift_S32_F32_Sat

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_SignalConvertShift_F32S_S32S)
    mlib_SignalConvertShift_F32S_S32S
    __attribute__((weak, alias("__mlib_SignalConvertShift_F32S_S32S")));
__typeof__(__mlib_SignalConvertShift_F32_S32) mlib_SignalConvertShift_F32_S32
    __attribute__((weak, alias("__mlib_SignalConvertShift_F32_S32")));
__typeof__(__mlib_SignalConvertShift_S32S_F32S_Sat)
    mlib_SignalConvertShift_S32S_F32S_Sat
    __attribute__((weak, alias("__mlib_SignalConvertShift_S32S_F32S_Sat")));
__typeof__(__mlib_SignalConvertShift_S32_F32_Sat)
    mlib_SignalConvertShift_S32_F32_Sat
    __attribute__((weak, alias("__mlib_SignalConvertShift_S32_F32_Sat")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

mlib_status
__mlib_SignalConvertShift_S32_F32_Sat(
    mlib_s32 *dst,
    const mlib_f32 *src,
    mlib_s32 shift,
    mlib_s32 n)
{
	mlib_s32 i;
	mlib_f32 fl_c;

	if (n <= 0)
		return (MLIB_FAILURE);

	if (!src || !dst)
		return (MLIB_NULLPOINTER);

	PREPARE_CONST(fl_c, shift + 31);

	if ((mlib_addr)src & 7) {
		SAT32(dst[0], (src[0] * fl_c));
		dst++;
		src++;
		n--;
	}

	if ((mlib_addr)dst & 7) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < n >> 1; i++) {
			mlib_union64 src0;

			src0.d64 = ((TYPE_64BIT *) src)[i];
			SAT32(dst[2 * i], (src0.f32x2.i0 * fl_c));
			SAT32(dst[2 * i + 1], (src0.f32x2.i1 * fl_c));
		}
	} else {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < n >> 1; i++) {
			mlib_union64 src0, dst0;

			src0.d64 = ((TYPE_64BIT *) src)[i];

			SAT32(dst0.s32x2.i0, (src0.f32x2.i0 * fl_c));
			SAT32(dst0.s32x2.i1, (src0.f32x2.i1 * fl_c));
			((TYPE_64BIT *) dst)[i] = dst0.d64;
		}
	}

	for (i <<= 1; i < n; i++) {
		SAT32(dst[i], (src[i] * fl_c));
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_SignalConvertShift_F32_S32(
    mlib_f32 *dst,
    const mlib_s32 *src,
    mlib_s32 shift,
    mlib_s32 n)
{
	mlib_s32 i;
	mlib_f32 fl_c;

	if (n <= 0)
		return (MLIB_FAILURE);

	if (!src || !dst)
		return (MLIB_NULLPOINTER);

	PREPARE_CONST(fl_c, shift - 31);

	if ((mlib_addr)src & 7) {
		dst[0] = src[0] * fl_c;
		dst++;
		src++;
		n--;
	}

	if ((mlib_addr)dst & 7) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < n >> 1; i++) {
			mlib_union64 src0;

			src0.d64 = ((TYPE_64BIT *) src)[i];
			dst[2 * i] = *(mlib_s32 *)&(src0.f32x2.i0) * fl_c;
			dst[2 * i + 1] = *(mlib_s32 *)&(src0.f32x2.i1) * fl_c;
		}
	} else {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < n >> 1; i++) {
			mlib_union64 src0, dst0;

			src0.d64 = ((TYPE_64BIT *) src)[i];
			dst0.f32x2.i0 = *(mlib_s32 *)&(src0.f32x2.i0) * fl_c;
			dst0.f32x2.i1 = *(mlib_s32 *)&(src0.f32x2.i1) * fl_c;
			((TYPE_64BIT *) dst)[i] = dst0.d64;
		}
	}

	for (i <<= 1; i < n; i++) {
		dst[i] = src[i] * fl_c;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_SignalConvertShift_S32S_F32S_Sat(
    mlib_s32 *dst,
    const mlib_f32 *src,
    mlib_s32 shift,
    mlib_s32 n)
{
	return (__mlib_SignalConvertShift_S32_F32_Sat(dst, src, shift, n << 1));
}

/* *********************************************************** */

mlib_status
__mlib_SignalConvertShift_F32S_S32S(
    mlib_f32 *dst,
    const mlib_s32 *src,
    mlib_s32 shift,
    mlib_s32 n)
{
	return (__mlib_SignalConvertShift_F32_S32(dst, src, shift, n << 1));
}

/* *********************************************************** */
