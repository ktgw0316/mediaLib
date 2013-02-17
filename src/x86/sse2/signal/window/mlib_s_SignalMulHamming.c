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

#pragma ident	"@(#)mlib_s_SignalMulHamming.c	9.2	07/11/05 SMI"

/*
 * FUNCTIONS
 *      mlib_SignalMulHamming_S16 - multiply source data on
 *          coefficients of the corresponding window
 *
 *      mlib_SignalMulHamming_S16S - multiply stereo source
 *          data on coefficients of the corresponding window
 *
 *      mlib_SignalMulHamming_S16_S16 - multiply source data on
 *          coefficients of the corresponding window with results
 *          in destination array
 *
 *      mlib_SignalMulHamming_S16S_S16S - multiply stereo source
 *          data on coefficients of the corresponding window with
 *          result in destination array
 *
 * SYNOPSIS
 *      mlib_status mlib_SignalMulHamming_S16(mlib_s16 *data,
 *                                            mlib_s32 n)
 *
 *      mlib_status mlib_SignalMulHamming_S16S(mlib_s16 *data,
 *                                             mlib_s32 n)
 *
 *      mlib_status mlib_SignalMulHamming_S16_S16(mlib_s16       *dst,
 *                                                const mlib_s16 *src,
 *                                                mlib_s32       n)
 *
 *      mlib_status mlib_SignalMulHamming_S16S_S16S(mlib_s16       *dst,
 *                                                  const mlib_s16 *src,
 *                                                  mlib_s32       n)
 *
 * ARGUMENTS
 *      data     Input and output signal array
 *      dst      Output signal array
 *      src      Input signal array
 *      n        Length of window array
 *
 * DESCRIPTION
 *      Mono        :   dst[i] = src[i] * window[i]    i = 0..(N-1)
 *      Stereo      :   dst[i] = src[i] * window[i/2]  i = 0..(2*N-1)
 *      Hamming     :   window[i] = 0.54 - 0.46*cos(2 * pi/(N - 1))
 *
 *
 */

#include <mlib_SysMath.h>
#include <mlib_signal.h>
#include <mlib_SignalWindow.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_SignalMulHamming_S16 = __mlib_SignalMulHamming_S16
#pragma weak mlib_SignalMulHamming_S16S = __mlib_SignalMulHamming_S16S
#pragma weak mlib_SignalMulHamming_S16S_S16S = \
	__mlib_SignalMulHamming_S16S_S16S
#pragma weak mlib_SignalMulHamming_S16_S16 = __mlib_SignalMulHamming_S16_S16

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_SignalMulHamming_S16) mlib_SignalMulHamming_S16
    __attribute__((weak, alias("__mlib_SignalMulHamming_S16")));
__typeof__(__mlib_SignalMulHamming_S16S) mlib_SignalMulHamming_S16S
    __attribute__((weak, alias("__mlib_SignalMulHamming_S16S")));
__typeof__(__mlib_SignalMulHamming_S16S_S16S) mlib_SignalMulHamming_S16S_S16S
    __attribute__((weak, alias("__mlib_SignalMulHamming_S16S_S16S")));
__typeof__(__mlib_SignalMulHamming_S16_S16) mlib_SignalMulHamming_S16_S16
    __attribute__((weak, alias("__mlib_SignalMulHamming_S16_S16")));

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

/* *********************************************************** */

mlib_status
__mlib_SignalMulHamming_S16_S16(
    mlib_s16 *dst,
    const mlib_s16 *src,
    mlib_s32 n)
{
	mlib_status res0 = MLIB_SUCCESS;
	mlib_status res1 = MLIB_SUCCESS;

/* check for obvious errors */

	if ((dst == NULL) || (n <= 1) || (src == NULL))
		return (MLIB_FAILURE);

	if (CACHE_SIZE > sizeof (mlib_s16) * n) {
		mlib_d64 window[CACHE_SIZE / 8];
		mlib_s16 *pwin = (mlib_s16 *)window;

		res0 = __mlib_SignalGenHamming_S16(pwin, n);
		res1 = __mlib_SignalMulWindow_S16_S16(dst, src, pwin, n);
	} else {
		mlib_s16 *pwin = __mlib_malloc(sizeof (mlib_s16) * n);

		if (pwin == NULL) {
			mlib_d64 cbase, sbase, cstep, sstep, temp, temp2,
			    result;
			mlib_s32 n2, n1, i;

			cbase = 0.46;
			sbase = 0.;
			n1 = n - 1;
			n2 = (n1 >> 1);

			if (n1)
				temp = 2. / (n1);
			else
				temp = 0;
			sstep = mlib_sincospi(temp, &cstep);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i <= n2; i++) {
				mlib_d64 src0 = src[i], src1 = src[n1 - i];

				temp = cbase * cstep - sbase * sstep;
				temp2 = sbase * cstep + cbase * sstep;
				result = 0.54 - cbase;
				dst[i] = result * src0;
				dst[n1 - i] = result * src1;
				cbase = temp;
				sbase = temp2;
			}
		} else {
			res0 = __mlib_SignalGenHamming_S16(pwin, n);
			res1 =
			    __mlib_SignalMulWindow_S16_S16(dst, src, pwin, n);
			__mlib_free(pwin);
		}
	}

	return ((res0 == MLIB_SUCCESS) && (res1 == MLIB_SUCCESS))
	    ? MLIB_SUCCESS : MLIB_FAILURE;
}

mlib_status
mlib_SignalMulHamming_S16_S16_i(
    mlib_s16 *dst,
    const mlib_s16 *src,
    mlib_s32 n,
    mlib_s32 i0,
    mlib_s32 i1)
{
	mlib_s32 mm = (i1 - i0) * 4;
	mlib_status res0 = MLIB_SUCCESS;
	mlib_status res1 = MLIB_SUCCESS;

/* check for obvious errors */

	if ((dst == NULL) || (n <= 1) || (src == NULL))
		return (MLIB_FAILURE);

	if (i1 == n / 4) mm += n & 3;

	if (CACHE_SIZE > sizeof (mlib_s16) * mm) {
		mlib_d64 window[CACHE_SIZE / 8];
		mlib_s16 *pwin = (mlib_s16 *)window;
		mlib_s16 *pwin2 = (mlib_s16 *)window + mm - 1;

		res0 = mlib_SignalGenHamming_S16_i(pwin - 2 * i0,
			pwin2 + 2 * i0, n, i0, i1);
		if (i1 == n / 4) {
			res1 = __mlib_SignalMulWindow_S16_S16(dst + i0 * 2,
				src + i0 * 2, pwin, mm);
		} else {
			res1 = __mlib_SignalMulWindow_S16_S16(dst + i0 * 2,
				src + i0 * 2, pwin, (i1 - i0) * 2);
			res1 |= __mlib_SignalMulWindow_S16_S16(dst +
				n - i1 * 2, src + n - i1 * 2,
				pwin2 + 1 - (i1 - i0) * 2, (i1 - i0) * 2);
		}
	} else {
		mlib_s16 *pwin = __mlib_malloc(sizeof (mlib_s16) * mm);
		mlib_s16 *pwin2 = pwin + mm - 1;

		if (pwin == NULL) {
			mlib_d64 cbase0, cbase1, cbase2, cbase3;
			mlib_d64 temp0, temp1, result0, result1, cstep;
			mlib_s32 i;
			mlib_s16 *dst_ = dst + n - 1;
			const mlib_s16 *src_ = src + n - 1;
			mlib_s32 src0, src1, src2, src3;
			mlib_union64 tmp0, tmp1;
			mlib_d64 tmp[2];

			cstep = mlib_cos(2. * M_PI / (n - 1));
			cbase0 = .46 * mlib_cos(2. * M_PI / (n - 1) * (2 * i0));
			cbase1 = .46 *
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

				result0 = .54 - cbase0;
				result1 = .54 - cbase1;
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
					dst[2] = src[2] * (.54 - cbase2);
				case 2:
					dst[1] = src[1] * (.54 - cbase1);
				case 1:
					dst[0] = src[0] * (.54 - cbase0);
				}
			}
		} else {
			res0 = mlib_SignalGenHamming_S16_i(pwin - 2 * i0,
				pwin2 + 2 * i0, n, i0, i1);
			if (i1 == n / 4) {
				res1 = __mlib_SignalMulWindow_S16_S16(dst +
					i0 * 2,	src + i0 * 2, pwin, mm);
			} else {
				res1 = __mlib_SignalMulWindow_S16_S16(dst +
					i0 * 2,	src + i0 * 2, pwin,
					(i1 - i0) * 2);
				res1 |= __mlib_SignalMulWindow_S16_S16(dst +
					n - i1 * 2, src + n - i1 * 2,
					pwin2 + 1 - (i1 - i0) * 2,
					(i1 - i0) * 2);
			}
		}
	}

	return ((res0 == MLIB_SUCCESS) && (res1 == MLIB_SUCCESS))
	    ? MLIB_SUCCESS : MLIB_FAILURE;
}

/* *********************************************************** */

mlib_status
__mlib_SignalMulHamming_S16S_S16S(
    mlib_s16 *dst,
    const mlib_s16 *src,
    mlib_s32 n)
{
	mlib_status res0 = MLIB_SUCCESS;
	mlib_status res1 = MLIB_SUCCESS;

/* check for obvious errors */

	if ((dst == NULL) || (n <= 1) || (src == NULL))
		return (MLIB_FAILURE);

	if (CACHE_SIZE > sizeof (mlib_s16) * n) {
		mlib_d64 window[CACHE_SIZE / 8];
		mlib_s16 *pwin = (mlib_s16 *)window;

		res0 = __mlib_SignalGenHamming_S16(pwin, n);
		res1 = __mlib_SignalMulWindow_S16S_S16S(dst, src, pwin, n);
	} else {
		mlib_s16 *pwin = __mlib_malloc(sizeof (mlib_s16) * n);

		if (pwin == NULL) {
			mlib_d64 cbase, sbase, cstep, sstep, temp, temp2,
			    result;
			mlib_s32 n2, n1, i, j, e0, e1;

			e1 = (n - 1) << 1;
			e0 = e1 + 1;
			cbase = 0.46;
			sbase = 0.;
			n1 = n - 1;
			n2 = (n1 >> 1);

			if (n1)
				temp = 2. / (n1);
			else
				temp = 0;
			sstep = mlib_sincospi(temp, &cstep);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0, j = 0; i <= n2; i++, j += 2) {
				mlib_d64 src0 = src[j], src1 =
				    src[j + 1], src2 = src[e0 - j], src3 =
				    src[e1 - j];

				temp = cbase * cstep - sbase * sstep;
				temp2 = sbase * cstep + cbase * sstep;
				result = 0.54 - cbase;
				dst[j] = result * src0;
				dst[j + 1] = result * src1;
				dst[e0 - j] = result * src2;
				dst[e1 - j] = result * src3;
				cbase = temp;
				sbase = temp2;
			}
		} else {
			res0 = __mlib_SignalGenHamming_S16(pwin, n);
			res1 =
			    __mlib_SignalMulWindow_S16S_S16S(dst, src, pwin, n);
			__mlib_free(pwin);
		}
	}

	return ((res0 == MLIB_SUCCESS) && (res1 == MLIB_SUCCESS))
	    ? MLIB_SUCCESS : MLIB_FAILURE;
}

mlib_status
mlib_SignalMulHamming_S16S_S16S_i(
    mlib_s16 *dst,
    const mlib_s16 *src,
    mlib_s32 n,
    mlib_s32 i0,
    mlib_s32 i1)
{
	mlib_s32 mm = (i1 - i0) * 4;
	mlib_status res0 = MLIB_SUCCESS;
	mlib_status res1 = MLIB_SUCCESS;

/* check for obvious errors */

	if ((dst == NULL) || (n <= 1) || (src == NULL))
		return (MLIB_FAILURE);

	if (i1 == n / 4) mm += n & 3;

	if (CACHE_SIZE > sizeof (mlib_s16) * mm) {
		mlib_d64 window[CACHE_SIZE / 8];
		mlib_s16 *pwin = (mlib_s16 *)window;
		mlib_s16 *pwin2 = (mlib_s16 *)window + mm - 1;

		res0 = mlib_SignalGenHamming_S16_i(pwin - 2 * i0,
			pwin2 + 2 * i0, n, i0, i1);
		if (i1 == n / 4) {
			res1 = __mlib_SignalMulWindow_S16S_S16S(dst + i0 * 4,
				src + i0 * 4, pwin, mm);
		} else {
			res1 = __mlib_SignalMulWindow_S16S_S16S(dst + i0 * 4,
				src + i0 * 4, pwin, (i1 - i0) * 2);
			res1 |= __mlib_SignalMulWindow_S16S_S16S(dst + 2 * n -
				i1 * 4,	src + 2 * n - i1 * 4,
				pwin2 + 1 - (i1 - i0) * 2, (i1 - i0) * 2);
		}
	} else {
		mlib_s16 *pwin = __mlib_malloc(sizeof (mlib_s16) * mm);
		mlib_s16 *pwin2 = pwin + mm - 1;

		if (pwin == NULL) {
			mlib_union64 tmp0, tmp1;
			mlib_d64 cbase0, cbase1;
			mlib_d64 temp0, result0, cstep;
			mlib_s32 i;
			mlib_s16 *dst_ = dst + 2 * n - 1;
			const mlib_s16 *src_ = src + 2 * n - 1;
			mlib_s32 src0, src1, src2, src3;
			mlib_d64 tmp[2];

			if ((src == NULL) || (dst == NULL) || (n <= 1))
				return (MLIB_FAILURE);

			cstep = mlib_cos(2. * M_PI / (n - 1));
			cbase0 = .46 * mlib_cos(2. * M_PI / (n - 1) * (2 * i0));
			cbase1 = .46 *
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
					dst_[-(2 * i)] =
						((mlib_s32 *)tmp)[2];
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

					dst[1] = src[1] * (.5 - cbase0);
					dst[0] = src[0] * (.5 - cbase0);
				}
			}

		} else {
			res0 = mlib_SignalGenHamming_S16_i(pwin - 2 * i0,
				pwin2 + 2 * i0, n, i0, i1);
			if (i1 == n / 4) {
				res1 = __mlib_SignalMulWindow_S16S_S16S(dst +
					i0 * 4,	src + i0 * 4, pwin, mm);
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
__mlib_SignalMulHamming_S16(
    mlib_s16 *data,
    mlib_s32 n)
{
	return (__mlib_SignalMulHamming_S16_S16(data, data, n));
}

/* *********************************************************** */

mlib_status
__mlib_SignalMulHamming_S16S(
    mlib_s16 *data,
    mlib_s32 n)
{
	return (__mlib_SignalMulHamming_S16S_S16S(data, data, n));
}

/* *********************************************************** */
