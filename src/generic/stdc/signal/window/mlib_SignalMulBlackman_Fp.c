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

#pragma ident	"@(#)mlib_SignalMulBlackman_Fp.c	9.2	07/10/09 SMI"

/*
 * FUNCTIONS
 *      mlib_SignalMulBlackman_F32 - multiply source data on
 *          coefficients of the corresponding window
 *
 *      mlib_SignalMulBlackman_F32S - multiply stereo source
 *          data on coefficients of the corresponding window
 *
 *      mlib_SignalMulBlackman_F32_F32 - multiply source data on
 *          coefficients of the corresponding window with results
 *          in destination array
 *
 *      mlib_SignalMulBlackman_F32S_F32S - multiply stereo source
 *          data on coefficients of the corresponding window with
 *          result in destination array
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_SignalMulBlackman_F32( mlib_f32 *data,
 *                                             mlib_f32  alpha,
 *                                             mlib_s32  n);
 *      mlib_status mlib_SignalMulBlackman_F32S( mlib_f32 *data,
 *                                              mlib_f32  alpha,
 *                                              mlib_s32  n);
 *      mlib_status mlib_SignalMulBlackman_F32S_F32S( mlib_f32      *dst,
 *                                                   const mlib_f32 *src,
 *                                                   mlib_f32       alpha,
 *                                                   mlib_s32       n);
 *      mlib_status mlib_SignalMulBlackman_F32_F32(mlib_f32       *dst,
 *                                                 const mlib_f32 *src,
 *                                                 mlib_f32       alpha,
 *                                                 mlib_s32       n);
 * ARGUMENTS
 *      data     Input and output signal array
 *      dst      Output signal array
 *      src      Input signal array         (SRC DATA MUST BE IN Q31 FORMAT !)
 *      alpha    Blackman window parameter
 *      n        Length of window array
 *
 * DESCRIPTION
 *      Mono        :   dst[i] = src[i] * window[i]    i = 0..(N-1)
 *
 *      Stereo      :   dst[i] = src[i] * window[i/2]  i = 0..(2*N-1)
 *
 *      Blackman    :   window[i] = (alpha + 1)/2 - 0.5*cos(2*pi*i/(N - 1)) -
 *                          (alpha/2)*cos(4*pi*i/(N - 1))
 *
 */

#include <mlib_signal.h>
#include <mlib_SysMath.h>
#include <mlib_SignalWindow.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_SignalMulBlackman_F32 = __mlib_SignalMulBlackman_F32
#pragma weak mlib_SignalMulBlackman_F32S = __mlib_SignalMulBlackman_F32S
#pragma weak mlib_SignalMulBlackman_F32S_F32S = \
	__mlib_SignalMulBlackman_F32S_F32S
#pragma weak mlib_SignalMulBlackman_F32_F32 = \
	__mlib_SignalMulBlackman_F32_F32

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_SignalMulBlackman_F32) mlib_SignalMulBlackman_F32
    __attribute__((weak, alias("__mlib_SignalMulBlackman_F32")));
__typeof__(__mlib_SignalMulBlackman_F32S) mlib_SignalMulBlackman_F32S
    __attribute__((weak, alias("__mlib_SignalMulBlackman_F32S")));
__typeof__(__mlib_SignalMulBlackman_F32S_F32S) mlib_SignalMulBlackman_F32S_F32S
    __attribute__((weak, alias("__mlib_SignalMulBlackman_F32S_F32S")));
__typeof__(__mlib_SignalMulBlackman_F32_F32) mlib_SignalMulBlackman_F32_F32
    __attribute__((weak, alias("__mlib_SignalMulBlackman_F32_F32")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#ifdef i386

/* do not copy by mlib_d64 data type for x86 */
typedef struct
{
	mlib_s32 int0, int1;
} two_int;

#define	TYPE_64BIT	two_int

#else /* i386 */

#define	TYPE_64BIT	mlib_d64

#endif /* i386 */

/* *********************************************************** */

typedef union
{
	TYPE_64BIT d64;
#ifdef _LITTLE_ENDIAN
	struct
	{
		mlib_f32 i1, i0;
	} f32x2;
#else		   /* _LITTLE_ENDIAN */
	struct
	{
		mlib_f32 i0, i1;
	} f32x2;
#endif		   /* _LITTLE_ENDIAN */
} mlib_union64;

/* *********************************************************** */

#define	MLIB_BLACKMAN(X)	((coeff1 - (X)) - (X) * coeff2 * (X))

/* *********************************************************** */

mlib_status
mlib_SignalMulBlackman_F32_F32_i(
    mlib_f32 *dst,
    const mlib_f32 *src,
    mlib_f32 alpha,
    mlib_s32 n,
    mlib_s32 i0,
    mlib_s32 i1)
{
	mlib_d64 cbase0, cbase1, cbase2, cbase3;
	mlib_d64 temp0, temp1, result0, result1, cstep;
	mlib_d64 coeff1, coeff2;
	mlib_s32 i;
	mlib_f32 *dst_ = dst + n - 1;
	const mlib_f32 *src_ = src + n - 1;
	mlib_f32 src0, src1, src2, src3;

	if ((src == NULL) || (dst == NULL) || (n <= 1))
		return (MLIB_FAILURE);

	if ((alpha <= -1.0) || (alpha >= 0.0))
		return (MLIB_OUTOFRANGE);

	coeff1 = .5 + alpha;
	coeff2 = 4. * alpha;
	cstep = mlib_cos(2. * M_PI / (n - 1));
	if (i0) {
		cbase0 = 0.5 * mlib_cos(2. * M_PI / (n - 1) * (2 * i0));
		cbase1 = 0.5 * mlib_cos(2. * M_PI / (n - 1) * (2 * i0 + 1));
	} else {
		cbase0 = 0.5;
		cbase1 = 0.5 * cstep;
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

		result0 = MLIB_BLACKMAN(cbase0);
		result1 = MLIB_BLACKMAN(cbase1);
		dst[2 * i] = result0 * src0;
		dst[2 * i + 1] = result1 * src1;
		dst_[-(2 * i)] = result0 * src2;
		dst_[-(2 * i + 1)] = result1 * src3;
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
			dst[2] = src[2] * MLIB_BLACKMAN(cbase2);
		case 2:
			dst[1] = src[1] * MLIB_BLACKMAN(cbase1);
		case 1:
			dst[0] = src[0] * MLIB_BLACKMAN(cbase0);
		}
	}

	return (MLIB_SUCCESS);
}

mlib_status
__mlib_SignalMulBlackman_F32_F32(
    mlib_f32 *dst,
    const mlib_f32 *src,
    mlib_f32 alpha,
    mlib_s32 n)
{
	return mlib_SignalMulBlackman_F32_F32_i(dst, src, alpha, n,
		0, n / 4);
}

mlib_status
__mlib_SignalMulBlackman_F32(
    mlib_f32 *data,
    mlib_f32 alpha,
    mlib_s32 n)
{
	return mlib_SignalMulBlackman_F32_F32_i(data, data, alpha, n,
		0, n / 4);
}

/* *********************************************************** */

mlib_status
mlib_SignalMulBlackman_F32S_F32S_i(
    mlib_f32 *dst,
    const mlib_f32 *src,
    mlib_f32 alpha,
    mlib_s32 n,
    mlib_s32 i0,
    mlib_s32 i1)
{
	mlib_d64 cbase0, cbase1, cbase2, cbase3;
	mlib_d64 temp0, temp1, result0, result1, cstep;
	mlib_d64 coeff1, coeff2;
	mlib_s32 i;
	mlib_f32 *dst_ = dst + 2 * n - 1;
	const mlib_f32 *src_ = src + 2 * n - 1;
	mlib_f32 src0, src1, src2, src3;
	mlib_f32 src4, src5, src6, src7;

	if ((src == NULL) || (dst == NULL) || (n <= 1))
		return (MLIB_FAILURE);

	if ((alpha <= -1.0) || (alpha >= 0.0))
		return (MLIB_OUTOFRANGE);

	coeff1 = .5 + alpha;
	coeff2 = 4. * alpha;
	cstep = mlib_cos(2. * M_PI / (n - 1));
	if (i0) {
		cbase0 = 0.5 * mlib_cos(2. * M_PI / (n - 1) * (2 * i0));
		cbase1 = 0.5 * mlib_cos(2. * M_PI / (n - 1) * (2 * i0 + 1));
	} else {
		cbase0 = 0.5;
		cbase1 = 0.5 * cstep;
	}
	cbase2 = 2. * cstep * cbase1 - cbase0;
	cbase3 = 2. * cstep * cbase2 - cbase1;
	cstep = 4. * cstep * cstep - 2.;

	if ((((mlib_addr)src | (mlib_addr)dst | (n * 4)) & 7) == 0) {
		mlib_union64 tmp0, tmp1, tmp2, tmp3;
		mlib_d64 *src__ = (mlib_d64 *)((mlib_addr)src_ & ~7);
		mlib_d64 *dst__ = (mlib_d64 *)((mlib_addr)dst_ & ~7);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = i0; i < i1; i++) {
			tmp0.d64 = ((TYPE_64BIT *) src)[2 * i];
			tmp1.d64 = ((TYPE_64BIT *) src)[2 * i + 1];
			tmp2.d64 = ((TYPE_64BIT *) src__)[-(2 * i)];
			tmp3.d64 = ((TYPE_64BIT *) src__)[-(2 * i + 1)];

			result0 = MLIB_BLACKMAN(cbase0);
			result1 = MLIB_BLACKMAN(cbase1);

			tmp0.f32x2.i0 = result0 * tmp0.f32x2.i0;
			tmp0.f32x2.i1 = result0 * tmp0.f32x2.i1;
			tmp1.f32x2.i0 = result1 * tmp1.f32x2.i0;
			tmp1.f32x2.i1 = result1 * tmp1.f32x2.i1;
			tmp2.f32x2.i0 = result0 * tmp2.f32x2.i0;
			tmp2.f32x2.i1 = result0 * tmp2.f32x2.i1;
			tmp3.f32x2.i0 = result1 * tmp3.f32x2.i0;
			tmp3.f32x2.i1 = result1 * tmp3.f32x2.i1;
			((TYPE_64BIT *) dst)[2 * i] = tmp0.d64;
			((TYPE_64BIT *) dst)[2 * i + 1] = tmp1.d64;
			((TYPE_64BIT *) dst__)[-(2 * i)] = tmp2.d64;
			((TYPE_64BIT *) dst__)[-(2 * i + 1)] = tmp3.d64;
			temp0 = cstep * cbase2 - cbase0;
			temp1 = cstep * cbase3 - cbase1;
			cbase0 = cbase2;
			cbase1 = cbase3;
			cbase2 = temp0;
			cbase3 = temp1;
		}
	} else {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = i0; i < i1; i++) {
			src0 = src[4 * i];
			src1 = src[4 * i + 1];
			src2 = src[4 * i + 2];
			src3 = src[4 * i + 3];
			src4 = src_[-(4 * i)];
			src5 = src_[-(4 * i + 1)];
			src6 = src_[-(4 * i + 2)];
			src7 = src_[-(4 * i + 3)];

			result0 = MLIB_BLACKMAN(cbase0);
			result1 = MLIB_BLACKMAN(cbase1);
			dst[4 * i] = result0 * src0;
			dst[4 * i + 1] = result0 * src1;
			dst[4 * i + 2] = result1 * src2;
			dst[4 * i + 3] = result1 * src3;
			dst_[-(4 * i)] = result0 * src4;
			dst_[-(4 * i + 1)] = result0 * src5;
			dst_[-(4 * i + 2)] = result1 * src6;
			dst_[-(4 * i + 3)] = result1 * src7;
			temp0 = cstep * cbase2 - cbase0;
			temp1 = cstep * cbase3 - cbase1;
			cbase0 = cbase2;
			cbase1 = cbase3;
			cbase2 = temp0;
			cbase3 = temp1;
		}
	}

	if ((n & 3) && i1 == n / 4) {
		dst += 4 * i;
		src += 4 * i;

		switch (n & 3) {
		case 3:
			dst[5] = src[5] * MLIB_BLACKMAN(cbase2);
			dst[4] = src[4] * MLIB_BLACKMAN(cbase2);
		case 2:
			dst[3] = src[3] * MLIB_BLACKMAN(cbase1);
			dst[2] = src[2] * MLIB_BLACKMAN(cbase1);
		case 1:
			dst[1] = src[1] * MLIB_BLACKMAN(cbase0);
			dst[0] = src[0] * MLIB_BLACKMAN(cbase0);
		}
	}

	return (MLIB_SUCCESS);
}

mlib_status
__mlib_SignalMulBlackman_F32S_F32S(
    mlib_f32 *dst,
    const mlib_f32 *src,
    mlib_f32 alpha,
    mlib_s32 n)
{
	return mlib_SignalMulBlackman_F32S_F32S_i(dst, src, alpha, n,
		0, n / 4);
}

mlib_status
__mlib_SignalMulBlackman_F32S(
    mlib_f32 *data,
    mlib_f32 alpha,
    mlib_s32 n)
{
	return mlib_SignalMulBlackman_F32S_F32S_i(data, data, alpha, n,
		0, n / 4);
}

/* *********************************************************** */
