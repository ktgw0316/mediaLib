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

#pragma ident	"@(#)mlib_SignalMul_Fp.c	9.2	07/10/09 SMI"

/*
 * FUNCTIONS
 *      mlib_SignalMul_[F32|F32S],
 *      mlib_SignalMul_[F32|F32S]_[F32|F32S] - multiplication of
 *                     float format signal arrays
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_SignalMul_F32(mlib_f32       *src1dst,
 *                                     const mlib_f32 *src2,
 *                                     mlib_s32       n);
 *      mlib_status mlib_SignalMul_F32S(mlib_f32       *src1dst,
 *                                      const mlib_f32 *src2,
 *                                      mlib_s32       n);
 *      mlib_status mlib_SignalMul_F32S_F32S(mlib_f32       *dst,
 *                                           const mlib_f32 *src1,
 *                                           const mlib_f32 *src2,
 *                                           mlib_s32       n);
 *      mlib_status mlib_SignalMul_F32_F32(mlib_f32       *dst,
 *                                         const mlib_f32 *src1,
 *                                         const mlib_f32 *src2,
 *                                         mlib_s32       n);
 * ARGUMENTS
 *   src1dst   The first input and the output signal array
 *      dst    Output signal array
 *      src1   The first input signal array
 *      src2   The second input signal array
 *      n      Number of samples in the input signal arrays
 *
 * DESCRIPTION
 *      dst = src1 * src2
 */

#include <mlib_signal.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_SignalMul_F32 = __mlib_SignalMul_F32
#pragma weak mlib_SignalMul_F32S = __mlib_SignalMul_F32S
#pragma weak mlib_SignalMul_F32S_F32S = __mlib_SignalMul_F32S_F32S
#pragma weak mlib_SignalMul_F32_F32 = __mlib_SignalMul_F32_F32

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_SignalMul_F32) mlib_SignalMul_F32
    __attribute__((weak, alias("__mlib_SignalMul_F32")));
__typeof__(__mlib_SignalMul_F32S) mlib_SignalMul_F32S
    __attribute__((weak, alias("__mlib_SignalMul_F32S")));
__typeof__(__mlib_SignalMul_F32S_F32S) mlib_SignalMul_F32S_F32S
    __attribute__((weak, alias("__mlib_SignalMul_F32S_F32S")));
__typeof__(__mlib_SignalMul_F32_F32) mlib_SignalMul_F32_F32
    __attribute__((weak, alias("__mlib_SignalMul_F32_F32")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

typedef union
{
	mlib_d64 d64;
	struct
	{
		mlib_f32 i0, i1;
	} f32x2;
} mlib_union64;

/* *********************************************************** */

mlib_status
__mlib_SignalMul_F32_F32(
    mlib_f32 *dst,
    const mlib_f32 *src1,
    const mlib_f32 *src2,
    mlib_s32 n)
{
	mlib_s32 i;

	if (n <= 0)
		return (MLIB_FAILURE);

	if (((mlib_addr)src1 & 7) != 0 && ((mlib_addr)src2 & 7) != 0) {
		dst[0] = src1[0] * src2[0];
		dst++;
		src1++;
		src2++;
		n--;
	}

	if ((mlib_addr)src1 & 7) {
		const mlib_f32 *tmp = src2;

		src2 = src1;
		src1 = tmp;
	}

	if (((mlib_addr)src2 & 7) != 0 && ((mlib_addr)dst & 7) != 0) {
		const mlib_f32 *tmp;

		dst[0] = src1[0] * src2[0];
		dst++;
		src1++;
		src2++;
		n--;
		tmp = src2;
		src2 = src1;
		src1 = tmp;
	}

	if (!((mlib_addr)dst & 7)) {
		if (!((mlib_addr)src2 & 7)) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < n >> 1; i++) {
				mlib_union64 s1, s2, dst0;

				s1.d64 = ((mlib_d64 *)src1)[i];
				s2.d64 = ((mlib_d64 *)src2)[i];
				dst0.f32x2.i0 = s1.f32x2.i0 * s2.f32x2.i0;
				dst0.f32x2.i1 = s1.f32x2.i1 * s2.f32x2.i1;
				((mlib_d64 *)dst)[i] = dst0.d64;
			}
		} else {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < n >> 1; i++) {
				mlib_union64 s1, dst0;
				mlib_f32 s20, s21;

				s1.d64 = ((mlib_d64 *)src1)[i];
				s20 = src2[2 * i];
				s21 = src2[2 * i + 1];
				dst0.f32x2.i0 = s1.f32x2.i0 * s20;
				dst0.f32x2.i1 = s1.f32x2.i1 * s21;
				((mlib_d64 *)dst)[i] = dst0.d64;
			}
		}
	} else {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < n >> 1; i++) {
			mlib_union64 s1, s2;
			mlib_f32 dst00, dst01;

			s1.d64 = ((mlib_d64 *)src1)[i];
			s2.d64 = ((mlib_d64 *)src2)[i];
			dst00 = s1.f32x2.i0 * s2.f32x2.i0;
			dst01 = s1.f32x2.i1 * s2.f32x2.i1;
			dst[2 * i] = dst00;
			dst[2 * i + 1] = dst01;
		}
	}

	for (i <<= 1; i < n; i++)
		dst[i] = src1[i] * src2[i];

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_SignalMul_F32S_F32S(
    mlib_f32 *dst,
    const mlib_f32 *src1,
    const mlib_f32 *src2,
    mlib_s32 n)
{
	return (__mlib_SignalMul_F32_F32(dst, src1, src2, n << 1));
}

/* *********************************************************** */

mlib_status
__mlib_SignalMul_F32(
    mlib_f32 *src1dst,
    const mlib_f32 *src2,
    mlib_s32 n)
{
	return (__mlib_SignalMul_F32_F32(src1dst, src1dst, src2, n));
}

/* *********************************************************** */

mlib_status
__mlib_SignalMul_F32S(
    mlib_f32 *src1dst,
    const mlib_f32 *src2,
    mlib_s32 n)
{
	return (__mlib_SignalMul_F32S_F32S(src1dst, src1dst, src2, n));
}

/* *********************************************************** */
