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

#pragma ident	"@(#)mlib_i_SignalMulHanning.c	9.2	07/11/05 SMI"

/*
 * FUNCTIONS
 *      mlib_SignalMulHanning_S16 - multiply source data on
 *          coefficients of the corresponding window
 *
 *      mlib_SignalMulHanning_S16S - multiply stereo source
 *          data on coefficients of the corresponding window
 *
 *      mlib_SignalMulHanning_S16_S16 - multiply source data on
 *          coefficients of the corresponding window with results
 *          in destination array
 *
 *      mlib_SignalMulHanning_S16S_S16S - multiply stereo source
 *          data on coefficients of the corresponding window with
 *          result in destination array
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_SignalMulHanning_S16(mlib_s16 *data,
 *                                            mlib_s32 n);
 *      mlib_status mlib_SignalMulHanning_S16S(mlib_s16 *data,
 *                                             mlib_s32 n);
 *      mlib_status mlib_SignalMulHanning_S16S_S16S(mlib_s16       *dst,
 *                                                  const mlib_s16 *src,
 *                                                  mlib_s32       n);
 *      mlib_status mlib_SignalMulHanning_S16_S16(mlib_s16       *dst,
 *                                                const mlib_s16 *src,
 *                                                mlib_s32       n);
 * ARGUMENTS
 *      data     Input and output signal array
 *      dst      Output signal array
 *      src      Input signal array
 *      n        Length of window array
 *
 * DESCRIPTION
 *      Mono        :   dst[i] = src[i] * window[i]    i = 0..N-1
 *      Stereo      :   dst[i] = src[i] * window[i/2]  i = 0..(2*N-1)
 *      Hanning     :   window[i] = 0.5*(1 - cos(2 * pi/(N -1)))
 *
 *
 */

#include <mlib_signal.h>
#include <mlib_SysMath.h>
#include <mlib_SignalWindow.h>
#include <mlib_i_SignalWinTab.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_SignalMulHanning_S16 = __mlib_SignalMulHanning_S16
#pragma weak mlib_SignalMulHanning_S16S = __mlib_SignalMulHanning_S16S
#pragma weak mlib_SignalMulHanning_S16S_S16S = \
	__mlib_SignalMulHanning_S16S_S16S
#pragma weak mlib_SignalMulHanning_S16_S16 = __mlib_SignalMulHanning_S16_S16

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_SignalMulHanning_S16) mlib_SignalMulHanning_S16
    __attribute__((weak, alias("__mlib_SignalMulHanning_S16")));
__typeof__(__mlib_SignalMulHanning_S16S) mlib_SignalMulHanning_S16S
    __attribute__((weak, alias("__mlib_SignalMulHanning_S16S")));
__typeof__(__mlib_SignalMulHanning_S16S_S16S) mlib_SignalMulHanning_S16S_S16S
    __attribute__((weak, alias("__mlib_SignalMulHanning_S16S_S16S")));
__typeof__(__mlib_SignalMulHanning_S16_S16) mlib_SignalMulHanning_S16_S16
    __attribute__((weak, alias("__mlib_SignalMulHanning_S16_S16")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	mlib_SignalF	mlib_Signal##F

/* *********************************************************** */

/* do not copy by mlib_d64 data type for x86 */
#ifdef i386
typedef struct
{
	mlib_s32 int0, int1;
} two_int;

#define	TYPE_64BIT	two_int

#else /* i386 */

#define	TYPE_64BIT	mlib_d64

#endif /* i386 */

typedef union
{
	TYPE_64BIT d64;
	struct
	{
		mlib_s32 i0, i1;
	} s32x2;
} mlib_union64;

/* *********************************************************** */

mlib_status
mlib_SignalMulHanning_S16_S16_i(
    mlib_s16 *dst,
    const mlib_s16 *src,
    mlib_s32 n,
    mlib_s32 i0,
    mlib_s32 i1)
{
	if ((src == NULL) || (dst == NULL) || (n <= 1))
		return (MLIB_FAILURE);

	mlib_s16 *dst_ = dst + n - 1;
	const mlib_s16 *src_ = src + n - 1;
	const mlib_s16 * HANNING =
		mlib_i_Hanning_S16 + (n >> 1) - 1;

	if ((n <= 1024) && ((n & (n - 1)) == 0)) {
		MLIB_MULWIN2P_S16(HANNING);
		return (MLIB_SUCCESS);
	}

	mlib_d64 cbase0, cbase1, cbase2, cbase3;
	mlib_d64 temp0, temp1, result0, result1, cstep;
	mlib_s32 i;
	mlib_s32 src0, src1, src2, src3;
	mlib_union64 tmp0, tmp1;
	TYPE_64BIT tmp[2];

	cstep = mlib_cos(2. * M_PI / (n - 1));
	if (i0) {
		cbase0 = .5 * mlib_cos(2. * M_PI / (n - 1) * (2 * i0));
		cbase1 = .5 * mlib_cos(2. * M_PI / (n - 1) * (2 * i0 + 1));
	} else {
		cbase0 = .5;
		cbase1 = .5 * cstep;
	}
	cbase2 = 2. * cstep * cbase1 - cbase0;
	cbase3 = 2. * cstep * cbase2 - cbase1;
	cstep = 4. * cstep * cstep - 2.;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = i0; i < i1; i++) {
		src0 = src[2 * i];
		src1 = src[2 * i + 1];
		src2 = src_[-(2 * i)];
		src3 = src_[-(2 * i + 1)];

		result0 = .5 - cbase0;
		result1 = .5 - cbase1;
		tmp0.s32x2.i0 = result0 * src0;
		tmp0.s32x2.i1 = result1 * src1;
		tmp1.s32x2.i0 = result0 * src2;
		tmp1.s32x2.i1 = result1 * src3;
		tmp[0] = tmp0.d64;
		tmp[1] = tmp1.d64;

		dst[2 * i] = ((mlib_s32 *)tmp)[0];
		dst[2 * i + 1] = ((mlib_s32 *)tmp)[1];
		dst_[-(2 * i)] = ((mlib_s32 *)tmp)[2];
		dst_[-(2 * i + 1)] = ((mlib_s32 *)tmp)[3];
		temp0 = cstep * cbase2 - cbase0;
		temp1 = cstep * cbase3 - cbase1;
		cbase0 = cbase2;
		cbase1 = cbase3;
		cbase2 = temp0;
		cbase3 = temp1;
	}

	if ((n & 3) && i1 == n / 4) {
		dst += 2 * i;
		src += 2 * i;

		switch (n & 3) {
		case 3:
			dst[2] = src[2] * (.5 - cbase2);
		case 2:
			dst[1] = src[1] * (.5 - cbase1);
		case 1:
			dst[0] = src[0] * (.5 - cbase0);
		}
	}

	return (MLIB_SUCCESS);
}

mlib_status
__mlib_SignalMulHanning_S16_S16(
    mlib_s16 *dst,
    const mlib_s16 *src,
    mlib_s32 n)
{
	return mlib_SignalMulHanning_S16_S16_i(dst, src, n,
		0, n >> 2);
}

mlib_status
__mlib_SignalMulHanning_S16(
    mlib_s16 *data,
    mlib_s32 n)
{
	return mlib_SignalMulHanning_S16_S16_i(data, data, n,
		0, n >> 2);
}

/* *********************************************************** */

mlib_status
mlib_SignalMulHanning_S16S_S16S_i(
    mlib_s16 *dst,
    const mlib_s16 *src,
    mlib_s32 n,
    mlib_s32 i0,
    mlib_s32 i1)
{
	if ((src == NULL) || (dst == NULL) || (n <= 1))
		return (MLIB_FAILURE);

	mlib_s16 *dst_ = dst + 2 * n - 1;
	const mlib_s16 *src_ = src + 2 * n - 1;
	const mlib_s16 * HANNING = mlib_i_Hanning_S16 + (n >> 1) - 1;

	if ((n <= 1024) && ((n & (n - 1)) == 0)) {
		MLIB_MULWIN2P_S16S(HANNING);
		return (MLIB_SUCCESS);
	}

	mlib_union64 tmp0, tmp1;
	mlib_d64 cbase0, cbase1;
	mlib_d64 temp0, result0, cstep;
	mlib_s32 i;
	mlib_s32 src0, src1, src2, src3;
	TYPE_64BIT tmp[2];

	cstep = mlib_cos(2. * M_PI / (n - 1));
	if (i0) {
		cbase0 = .5 * mlib_cos(2. * M_PI / (n - 1) * (2 * i0));
		cbase1 = .5 * mlib_cos(2. * M_PI / (n - 1) * (2 * i0 + 1));
	} else {
		cbase0 = .5;
		cbase1 = .5 * cstep;
	}
	cstep = 2. * cstep;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = i0 * 2; i < i1 * 2; i++) {
		src0 = src[2 * i];
		src1 = src[2 * i + 1];
		src2 = src_[-(2 * i)];
		src3 = src_[-(2 * i + 1)];

		result0 = .5 - cbase0;
		tmp0.s32x2.i0 = result0 * src0;
		tmp0.s32x2.i1 = result0 * src1;
		tmp1.s32x2.i0 = result0 * src2;
		tmp1.s32x2.i1 = result0 * src3;
		tmp[0] = tmp0.d64;
		tmp[1] = tmp1.d64;

		dst[2 * i] = ((mlib_s32 *)tmp)[0];
		dst[2 * i + 1] = ((mlib_s32 *)tmp)[1];
		dst_[-(2 * i)] = ((mlib_s32 *)tmp)[2];
		dst_[-(2 * i + 1)] = ((mlib_s32 *)tmp)[3];
		temp0 = cstep * cbase1 - cbase0;
		cbase0 = cbase1;
		cbase1 = temp0;
	}

	if (i1 == n / 4) {
		if (i < n / 2) {
			src0 = src[2 * i];
			src1 = src[2 * i + 1];
			src2 = src_[-(2 * i)];
			src3 = src_[-(2 * i + 1)];

			result0 = .5 - cbase0;
			tmp0.s32x2.i0 = result0 * src0;
			tmp0.s32x2.i1 = result0 * src1;
			tmp1.s32x2.i0 = result0 * src2;
			tmp1.s32x2.i1 = result0 * src3;
			tmp[0] = tmp0.d64;
			tmp[1] = tmp1.d64;

			dst[2 * i] = ((mlib_s32 *)tmp)[0];
			dst[2 * i + 1] = ((mlib_s32 *)tmp)[1];
			dst_[-(2 * i)] = ((mlib_s32 *)tmp)[2];
			dst_[-(2 * i + 1)] = ((mlib_s32 *)tmp)[3];
			temp0 = cstep * cbase1 - cbase0;
			cbase0 = cbase1;
			cbase1 = temp0;
			i++;
		}

		if (n & 1) {
			dst += 2 * i;
			src += 2 * i;

			dst[1] = src[1] * (.5 - cbase0);
			dst[0] = src[0] * (.5 - cbase0);
		}
	}

	return (MLIB_SUCCESS);
}

mlib_status
__mlib_SignalMulHanning_S16S_S16S(
    mlib_s16 *dst,
    const mlib_s16 *src,
    mlib_s32 n)
{
	return mlib_SignalMulHanning_S16S_S16S_i(dst, src, n,
		0, n >> 2);
}

mlib_status
__mlib_SignalMulHanning_S16S(
    mlib_s16 *data,
    mlib_s32 n)
{
	return mlib_SignalMulHanning_S16S_S16S_i(data, data, n,
		0, n >> 2);
}

/* *********************************************************** */
