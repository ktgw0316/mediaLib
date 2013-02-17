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

#pragma ident	"@(#)mlib_v_SignalMulBlackman.c	9.2	07/11/05 SMI"

/*
 * FUNCTIONS
 *      mlib_SignalMulBlackman_S16 - multiply source data on
 *          coefficients of the corresponding window
 *
 *      mlib_SignalMulBlackman_S16S - multiply stereo source
 *          data on coefficients of the corresponding window
 *
 *      mlib_SignalMulBlackman_S16_S16 - multiply source data on
 *          coefficients of the corresponding window with results
 *          in destination array
 *
 *      mlib_SignalMulBlackman_S16S_S16S - multiply stereo source
 *          data on coefficients of the corresponding window with
 *          result in destination array
 *
 * SYNOPSIS
 *      mlib_status mlib_SignalMulBlackman_S16(mlib_s16 *data,
 *                                             mlib_f32 alpha,
 *                                             mlib_s32 n)
 *
 *      mlib_status mlib_SignalMulBlackman_S16S(mlib_s16 *data,
 *                                              mlib_f32 alpha,
 *                                              mlib_s32 n)
 *
 *      mlib_status mlib_SignalMulBlackman_S16_S16(mlib_s16       *dst,
 *                                                 const mlib_s16 *src,
 *                                                 mlib_f32       alpha,
 *                                                 mlib_s32       n)
 *
 *      mlib_status mlib_SignalMulBlackman_S16S_S16S(mlib_s16      *dst,
 *                                                   cons mlib_s16 *src,
 *                                                   mlib_f32      alpha,
 *                                                   mlib_s32      n)
 *
 * ARGUMENTS
 *      data     Input and output signal array
 *      dst      Output signal array
 *      src      Input signal array
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

#include <mlib_SysMath.h>
#include <mlib_signal.h>
#include <vis_proto.h>
#include <mlib_SignalWindow.h>
#include <mlib_SignalWindowUtil.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_SignalMulBlackman_S16 = __mlib_SignalMulBlackman_S16
#pragma weak mlib_SignalMulBlackman_S16S = __mlib_SignalMulBlackman_S16S
#pragma weak mlib_SignalMulBlackman_S16S_S16S = \
	__mlib_SignalMulBlackman_S16S_S16S
#pragma weak mlib_SignalMulBlackman_S16_S16 = \
	__mlib_SignalMulBlackman_S16_S16

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_SignalMulBlackman_S16) mlib_SignalMulBlackman_S16
    __attribute__((weak, alias("__mlib_SignalMulBlackman_S16")));
__typeof__(__mlib_SignalMulBlackman_S16S) mlib_SignalMulBlackman_S16S
    __attribute__((weak, alias("__mlib_SignalMulBlackman_S16S")));
__typeof__(__mlib_SignalMulBlackman_S16S_S16S) mlib_SignalMulBlackman_S16S_S16S
    __attribute__((weak, alias("__mlib_SignalMulBlackman_S16S_S16S")));
__typeof__(__mlib_SignalMulBlackman_S16_S16) mlib_SignalMulBlackman_S16_S16
    __attribute__((weak, alias("__mlib_SignalMulBlackman_S16_S16")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

/* 3/4 of cache */

#define	CACHE_SIZE	12288

typedef union
{
	mlib_d64 d64;
	struct
	{
		mlib_s32 i0, i1;
	} s32x2;
} mlib_union64;

#define	MLIB_BLACKMAN(X)	((coeff1 - (X)) - (X) * coeff2 * (X))

/* *********************************************************** */

mlib_status
__mlib_SignalMulBlackman_S16_S16(
    mlib_s16 *dst,
    const mlib_s16 *src,
    mlib_f32 alpha,
    mlib_s32 n)
{
	mlib_status res0 = MLIB_SUCCESS;
	mlib_status res1 = MLIB_SUCCESS;

/* check for obvious errors */

	if ((dst == NULL) || (n <= 1) || (src == NULL))
		return (MLIB_FAILURE);

	if ((alpha <= -1.0) || (alpha >= 0.0))
		return (MLIB_OUTOFRANGE);

	if (CACHE_SIZE > sizeof (mlib_s16) * n) {
		mlib_d64 window[CACHE_SIZE / 8];
		mlib_s16 *pwin = (mlib_s16 *)window;

		res0 = __mlib_SignalGenBlackman_S16(pwin, alpha, n);
		res1 = __mlib_SignalMulWindow_S16_S16(dst, src, pwin, n);
	} else {
		mlib_s16 *pwin = __mlib_malloc(sizeof (mlib_s16) * n);

		if (pwin == NULL) {
			mlib_d64 cbase, sbase, cstep, sstep, temp, temp2;
			mlib_d64 coef1, coef2, result, t5, t6;
			mlib_s32 n2, n1, i;

			coef1 = alpha * 0.5 + 0.5;
			coef2 = alpha * 0.5;
			cbase = 1.;
			sbase = 0.;
			n1 = n - 1;
			n2 = (n1 >> 1);
			temp = 2. / (n1);
			sstep = mlib_sincospi(temp, &cstep);
			t5 = cbase * cbase;
			t6 = sbase * sbase;

#pragma pipeloop(0)
			for (i = 0; i <= n2; i++) {
				mlib_d64 src0 = src[i], src1 = src[n1 - i];

				temp = cbase * cstep - sbase * sstep;
				temp2 = sbase * cstep + cbase * sstep;
				result =
				    coef1 - 0.5 * cbase - coef2 * (t5 - t6);

				dst[i] = src0 * result;
				dst[n1 - i] = src1 * result;
				cbase = temp;
				sbase = temp2;
				t5 = cbase * cbase;
				t6 = sbase * sbase;
			}
		} else {
			res0 = __mlib_SignalGenBlackman_S16(pwin, alpha, n);
			res1 =
			    __mlib_SignalMulWindow_S16_S16(dst, src, pwin, n);
			__mlib_free(pwin);
		}
	}

	return ((res0 == MLIB_SUCCESS) && (res1 == MLIB_SUCCESS))
	    ? MLIB_SUCCESS : MLIB_FAILURE;
}

mlib_status
mlib_SignalMulBlackman_S16_S16_i(
    mlib_s16 *dst,
    const mlib_s16 *src,
    mlib_f32 alpha,
    mlib_s32 n,
    mlib_s32 i0,
    mlib_s32 i1)
{
	mlib_s32 nn = (i1 - i0) * 4;
	mlib_status res0 = MLIB_SUCCESS;
	mlib_status res1 = MLIB_SUCCESS;

/* check for obvious errors */

	if ((dst == NULL) || (n <= 1) || (src == NULL))
		return (MLIB_FAILURE);

	if ((alpha <= -1.0) || (alpha >= 0.0))
		return (MLIB_OUTOFRANGE);

	if (i1 == n / 4) nn += n & 3;

	if (CACHE_SIZE > sizeof (mlib_s16) * nn) {
		mlib_d64 window[CACHE_SIZE / 8];
		mlib_s16 *pwin = (mlib_s16 *)window;
		mlib_s16 *pwin2 = (mlib_s16 *)window + nn - 1;

		res0 = mlib_SignalGenBlackman_S16_i(pwin - 2 * i0,
			pwin2 + 2 * i0, alpha, n, i0, i1);
		if (i1 == n / 4) {
			res1 = __mlib_SignalMulWindow_S16_S16(dst + i0 * 2,
				src + i0 * 2, pwin, nn);
		} else {
			res1 = __mlib_SignalMulWindow_S16_S16(dst + i0 * 2,
				src + i0 * 2, pwin, (i1 - i0) * 2);
			res1 |= __mlib_SignalMulWindow_S16_S16(dst + n - i1 * 2,
				src + n - i1 * 2, pwin2 + 1 - (i1 - i0)* 2,
				(i1 - i0) * 2);
		}
	} else {
		mlib_s16 *pwin = __mlib_malloc(sizeof (mlib_s16) * nn);
		mlib_s16 *pwin2 = pwin + nn - 1;

		if (pwin == NULL) {
			mlib_d64 cbase0, cbase1, cbase2, cbase3;
			mlib_d64 temp0, temp1, result0, result1, cstep;
			mlib_d64 coeff1, coeff2;
			mlib_s32 i;
			mlib_s16 *dst_ = dst + n - 1;
			const mlib_s16 *src_ = src + n - 1;
			mlib_s32 src0, src1, src2, src3;
			mlib_union64 tmp0, tmp1;
			mlib_d64 tmp[2];

			coeff1 = .5 + alpha;
			coeff2 = 4. * alpha;
			cstep = mlib_cos(2. * M_PI / (n - 1));
			cbase0 = 0.5 *
				mlib_cos(2. * M_PI / (n - 1) * (2 * i0));
			cbase1 = 0.5 *
				mlib_cos(2. * M_PI / (n - 1) * (2 * i0 + 1));
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
					dst[2] = src[2] * MLIB_BLACKMAN(cbase2);
				case 2:
					dst[1] = src[1] * MLIB_BLACKMAN(cbase1);
				case 1:
					dst[0] = src[0] * MLIB_BLACKMAN(cbase0);
				}
			}
		} else {
			res0 = mlib_SignalGenBlackman_S16_i(pwin - 2 * i0,
				pwin2 + 2 * i0, alpha, n, i0, i1);
			if (i1 == n / 4) {
				res1 = __mlib_SignalMulWindow_S16_S16(dst +
					i0 * 2,	src + i0 * 2, pwin, nn);
			} else {
				res1 = __mlib_SignalMulWindow_S16_S16(dst +
					i0 * 2,	src + i0 * 2, pwin,
					(i1 - i0) * 2);
				res1 |= __mlib_SignalMulWindow_S16_S16(dst +
					n - i1 * 2, src + n - i1 * 2,
					pwin2 + 1 - (i1 - i0)* 2,
					(i1 - i0) * 2);
			}
		}
	}

	return ((res0 == MLIB_SUCCESS) && (res1 == MLIB_SUCCESS))
	    ? MLIB_SUCCESS : MLIB_FAILURE;
}

/* *********************************************************** */

mlib_status
__mlib_SignalMulBlackman_S16S_S16S(
    mlib_s16 *dst,
    const mlib_s16 *src,
    mlib_f32 alpha,
    mlib_s32 n)
{
	mlib_status res0 = MLIB_SUCCESS;
	mlib_status res1 = MLIB_SUCCESS;

/* check for obvious errors */

	if ((dst == NULL) || (n <= 1) || (src == NULL))
		return (MLIB_FAILURE);

	if ((alpha <= -1.0) || (alpha >= 0.0))
		return (MLIB_OUTOFRANGE);

	if (CACHE_SIZE > sizeof (mlib_s16) * n) {
		mlib_d64 window[CACHE_SIZE / 8];
		mlib_s16 *pwin = (mlib_s16 *)window;

		res0 = __mlib_SignalGenBlackman_S16(pwin, alpha, n);
		res1 = __mlib_SignalMulWindow_S16S_S16S(dst, src, pwin, n);
	} else {
		mlib_s16 *pwin = __mlib_malloc(sizeof (mlib_s16) * n);

		if (pwin == NULL) {
			mlib_d64 cbase, sbase, cstep, sstep, temp, temp2;
			mlib_d64 coef1, coef2, result, t5, t6;
			mlib_s32 n2, n1, i, j, e0, e1;

			coef1 = alpha * 0.5 + 0.5;
			coef2 = alpha * 0.5;
			cbase = 1.;
			sbase = 0.;
			n1 = n - 1;
			n2 = (n1 >> 1);
			e1 = (n1) << 1;
			e0 = e1 + 1;
			temp = 2. / (n1);
			sstep = mlib_sincospi(temp, &cstep);
			t5 = cbase * cbase;
			t6 = sbase * sbase;

#pragma pipeloop(0)
			for (i = 0, j = 0; i <= n2; i++, j += 2) {
				mlib_d64 src0 = src[j], src1 =
				    src[j + 1], src2 = src[e0 - j], src3 =
				    src[e1 - j];

				temp = cbase * cstep - sbase * sstep;
				temp2 = sbase * cstep + cbase * sstep;
				result =
				    coef1 - 0.5 * cbase - coef2 * (t5 - t6);

				dst[j] = result * src0;
				dst[j + 1] = result * src1;
				dst[e0 - j] = result * src2;
				dst[e1 - j] = result * src3;
				cbase = temp;
				sbase = temp2;
				t5 = cbase * cbase;
				t6 = sbase * sbase;
			}
		} else {
			res0 = __mlib_SignalGenBlackman_S16(pwin, alpha, n);
			res1 =
			    __mlib_SignalMulWindow_S16S_S16S(dst, src, pwin, n);
			__mlib_free(pwin);
		}
	}

	return ((res0 == MLIB_SUCCESS) && (res1 == MLIB_SUCCESS))
	    ? MLIB_SUCCESS : MLIB_FAILURE;
}

mlib_status
mlib_SignalMulBlackman_S16S_S16S_i(
    mlib_s16 *dst,
    const mlib_s16 *src,
    mlib_f32 alpha,
    mlib_s32 n,
    mlib_s32 i0,
    mlib_s32 i1)
{
	mlib_s32 nn = (i1 - i0) * 4;
	mlib_status res0 = MLIB_SUCCESS;
	mlib_status res1 = MLIB_SUCCESS;

/* check for obvious errors */

	if ((dst == NULL) || (n <= 1) || (src == NULL))
		return (MLIB_FAILURE);

	if ((alpha <= -1.0) || (alpha >= 0.0))
		return (MLIB_OUTOFRANGE);

	if (i1 == n / 4) nn += n & 3;

	if (CACHE_SIZE > sizeof (mlib_s16) * nn) {
		mlib_d64 window[CACHE_SIZE / 8];
		mlib_s16 *pwin = (mlib_s16 *)window;
		mlib_s16 *pwin2 = (mlib_s16 *)window + nn - 1;

		res0 = mlib_SignalGenBlackman_S16_i(pwin - 2 * i0,
			pwin2 + 2 * i0, alpha, n, i0, i1);
		if (i1 == n / 4) {
			res1 = __mlib_SignalMulWindow_S16S_S16S(dst + i0 * 4,
				src + i0 * 4, pwin, nn);
		} else {
			res1 = __mlib_SignalMulWindow_S16S_S16S(dst + i0 * 4,
				src + i0 * 4, pwin, (i1 - i0) * 2);
			res1 |= __mlib_SignalMulWindow_S16S_S16S(dst + 2 * n -
				i1 * 4,	src + 2 * n - i1 * 4,
				pwin2 + 1 - (i1 - i0) * 2, (i1 - i0) * 2);
		}
	} else {
		mlib_s16 *pwin = __mlib_malloc(sizeof (mlib_s16) * nn);
		mlib_s16 *pwin2 = pwin + nn - 1;

		if (pwin == NULL) {
			mlib_union64 tmp0, tmp1;
			mlib_d64 cbase0, cbase1;
			mlib_d64 temp0, result0, cstep;
			mlib_d64 coeff1, coeff2;
			mlib_s32 i;
			mlib_s16 *dst_ = dst + 2 * n - 1;
			const mlib_s16 *src_ = src + 2 * n - 1;
			mlib_s32 src0, src1, src2, src3;
			mlib_d64 tmp[2];

			coeff1 = .5 + alpha;
			coeff2 = 4. * alpha;
			cstep = mlib_cos(2. * M_PI / (n - 1));
			cbase0 = 0.5 * mlib_cos(2. * M_PI / (n - 1) * (2 * i0));
			cbase1 = 0.5 *
				mlib_cos(2. * M_PI / (n - 1) * (2 * i0 + 1));
			cstep = 2. * cstep;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = i0 * 2; i < i1 * 2; i++) {
				src0 = src[2 * i];
				src1 = src[2 * i + 1];
				src2 = src_[-(2 * i)];
				src3 = src_[-(2 * i + 1)];

				result0 = MLIB_BLACKMAN(cbase0);
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

					result0 = MLIB_BLACKMAN(cbase0);
					tmp0.s32x2.i0 = result0 * src0;
					tmp0.s32x2.i1 = result0 * src1;
					tmp1.s32x2.i0 = result0 * src2;
					tmp1.s32x2.i1 = result0 * src3;
					tmp[0] = tmp0.d64;
					tmp[1] = tmp1.d64;

					dst[2 * i] = ((mlib_s32 *)tmp)[0];
					dst[2 * i + 1] = ((mlib_s32 *)tmp)[1];
					dst_[-(2 * i)] = ((mlib_s32 *)tmp)[2];
					dst_[-(2 * i + 1)] =
						((mlib_s32 *)tmp)[3];
					temp0 = cstep * cbase1 - cbase0;
					cbase0 = cbase1;
					cbase1 = temp0;
					i++;
				}

				if (n & 1) {
					dst += 2 * i;
					src += 2 * i;

					dst[1] = src[1] * MLIB_BLACKMAN(cbase0);
					dst[0] = src[0] * MLIB_BLACKMAN(cbase0);
				}
			}
		} else {
			res0 = mlib_SignalGenBlackman_S16_i(pwin - 2 * i0,
				pwin2 + 2 * i0, alpha, n, i0, i1);
			if (i1 == n / 4) {
				res1 = __mlib_SignalMulWindow_S16S_S16S(dst +
					i0 * 4,	src + i0 * 4, pwin, nn);
			} else {
				res1 = __mlib_SignalMulWindow_S16S_S16S(dst +
					i0 * 4, src + i0 * 4, pwin,
					(i1 - i0) * 2);
				res1 |= __mlib_SignalMulWindow_S16S_S16S(dst +
					2 * n - i1 * 4, src + 2 * n - i1 * 4,
					pwin2 + 1 - (i1 - i0)* 2,
					(i1 - i0) * 2);
			}
		}
	}

	return ((res0 == MLIB_SUCCESS) && (res1 == MLIB_SUCCESS))
	    ? MLIB_SUCCESS : MLIB_FAILURE;
}

/* *********************************************************** */

mlib_status
__mlib_SignalMulBlackman_S16(
    mlib_s16 *data,
    mlib_f32 alpha,
    mlib_s32 n)
{
	return (__mlib_SignalMulBlackman_S16_S16(data, data, alpha, n));
}

/* *********************************************************** */

mlib_status
__mlib_SignalMulBlackman_S16S(
    mlib_s16 *data,
    mlib_f32 alpha,
    mlib_s32 n)
{
	return (__mlib_SignalMulBlackman_S16S_S16S(data, data, alpha, n));
}

/* *********************************************************** */
