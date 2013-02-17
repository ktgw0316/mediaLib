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

#pragma ident	"@(#)mlib_s_SignalMulBartlett.c	9.3	07/11/05 SMI"

/*
 * FUNCTIONS
 *      mlib_SignalMulBartlett_S16 - multiply source data on
 *          coefficients of the corresponding window
 *
 *      mlib_SignalMulBartlett_S16S - multiply stereo source
 *          data on coefficients of the corresponding window
 *
 *      mlib_SignalMulBartlett_S16_S16 - multiply source data on
 *          coefficients of the corresponding window with results
 *          in destination array
 *
 *      mlib_SignalMulBartlett_S16S_S16S - multiply stereo source
 *          data on coefficients of the corresponding window with
 *          result in destination array
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_SignalMulBartlett_S16(mlib_s16 *data,
 *                                             mlib_s32 n);
 *      mlib_status mlib_SignalMulBartlett_S16S(mlib_s16 *data,
 *                                              mlib_s32 n);
 *      mlib_status mlib_SignalMulBartlett_S16S_S16S(mlib_s16       *dst,
 *                                                   const mlib_s16 *src,
 *                                                   mlib_s32       n);
 *      mlib_status mlib_SignalMulBartlett_S16_S16(mlib_s16       *dst,
 *                                                 const mlib_s16 *src,
 *                                                 mlib_s32       n);
 * ARGUMENTS
 *      data     Input and output signal array
 *      dst      Output signal array
 *      src      Input signal array
 *      n        Length of window array
 *
 * DESCRIPTION
 *      Mono        :   dst[i] = src[i] * window[i]    i = 0..(N-1)
 *      Stereo      :   dst[i] = src[i] * window[i/2]  i = 0..(2*N-1)
 *      Bartlett    :   window[i] = ((i <= (N - 1)/2) ? (2*i/(N -1)) :
 *                        (2 - (2 * i)/(N-1)))
 *
 */

#include <mlib_signal.h>
#ifdef __SUNPRO_C
#include <sunmedia_intrin.h>
#else
#include <emmintrin.h>
#endif

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_SignalMulBartlett_S16 = __mlib_SignalMulBartlett_S16
#pragma weak mlib_SignalMulBartlett_S16S = __mlib_SignalMulBartlett_S16S
#pragma weak mlib_SignalMulBartlett_S16S_S16S = \
	__mlib_SignalMulBartlett_S16S_S16S
#pragma weak mlib_SignalMulBartlett_S16_S16 = \
	__mlib_SignalMulBartlett_S16_S16

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_SignalMulBartlett_S16) mlib_SignalMulBartlett_S16
    __attribute__((weak, alias("__mlib_SignalMulBartlett_S16")));
__typeof__(__mlib_SignalMulBartlett_S16S) mlib_SignalMulBartlett_S16S
    __attribute__((weak, alias("__mlib_SignalMulBartlett_S16S")));
__typeof__(__mlib_SignalMulBartlett_S16S_S16S) mlib_SignalMulBartlett_S16S_S16S
    __attribute__((weak, alias("__mlib_SignalMulBartlett_S16S_S16S")));
__typeof__(__mlib_SignalMulBartlett_S16_S16) mlib_SignalMulBartlett_S16_S16
    __attribute__((weak, alias("__mlib_SignalMulBartlett_S16_S16")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	MLIB_SIGNAL_MASK	0x7FFFFFFF
#define	CACHE_SIZE	12288

/* *********************************************************** */

mlib_status
__mlib_SignalMulBartlett_S16S(
    mlib_s16 *data,
    mlib_s32 n)
{
/*
 *  Seems that following trick doesn't work :-(
 *
 *  return (__mlib_SignalMulBartlett_S16S_S16S(data, data, n));
 */

	mlib_status res0 = MLIB_SUCCESS;
	mlib_status res1 = MLIB_SUCCESS;
/* check for obvious errors */

	if ((data == NULL) || (n <= 1))
		return (MLIB_FAILURE);

	{
		if (CACHE_SIZE > sizeof (mlib_s16) * n) {
			mlib_d64 window[CACHE_SIZE / 8];
			mlib_s16 *pwin = (mlib_s16 *)window;

			res0 = __mlib_SignalGenBartlett_S16(pwin, n);
			res1 = __mlib_SignalMulWindow_S16S_S16S(
					data, data, pwin, n);
		} else {
			mlib_s16 *pwin = __mlib_malloc(sizeof (mlib_s16) * n);
			if (pwin == NULL) {

				mlib_d64 step, base;
				mlib_s32 n2, n1, i, j, j1, e0, e1;
				mlib_d64 src0, src1, src2, src3;

				e1 = (n - 1) << 1;
				e0 = e1 + 1;
				j = 0;
				j1 = 1;
				n1 = (n - 1);
				n2 = (n1) >> 1;
				base = 0;
				step = 2. / n1;

				src0 = 0;
				src1 = 0;
				src2 = 0;
				src3 = 0;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i < n2; i++) {
					data[j] = src0;
					j += 2;
					src0 = data[j];
					data[j1] = base * src1;
					j1 += 2;
					src1 = data[j1];
					data[e1] = base * src2;
					data[e0] = base * src3;
					base += step;
					src0 *= base;
					e1 -= 2;
					src2 = data[e1];
					e0 -= 2;
					src3 = data[e0];
				}

				data[j] = src0;
				data[j1] = base * src1;
				data[e1] = base * src2;
				data[e0] = base * src3;
			} else {
				res0 = __mlib_SignalGenBartlett_S16(pwin, n);
				res1 =
				    __mlib_SignalMulWindow_S16S_S16S(
							data, data, pwin, n);
				__mlib_free(pwin);
			}
		}
	}
	return ((res0 == MLIB_SUCCESS) && (res1 == MLIB_SUCCESS))
	    ? MLIB_SUCCESS : MLIB_FAILURE;

}

/* *********************************************************** */

mlib_status
__mlib_SignalMulBartlett_S16_S16(
    mlib_s16 *dst,
    const mlib_s16 *src,
    mlib_s32 n)
{
	mlib_status res0 = MLIB_SUCCESS;
	mlib_status res1 = MLIB_SUCCESS;
/* check for obvious errors */

	if ((dst == NULL) || (n <= 1) || (src == NULL))
		return (MLIB_FAILURE);

	{
		if (CACHE_SIZE > sizeof (mlib_s16) * n) {
			mlib_d64 window[CACHE_SIZE / 8];
			mlib_s16 *pwin = (mlib_s16 *)window;

			res0 = __mlib_SignalGenBartlett_S16(pwin, n);
			res1 = __mlib_SignalMulWindow_S16_S16(
					dst, src, pwin, n);
		} else {
			mlib_s16 *pwin = __mlib_malloc(sizeof (mlib_s16) * n);
			if (pwin == NULL) {
				mlib_d64 step, base;
				mlib_s32 n2, n1, i;

				n1 = n - 1;
				n2 = (n1 >> 1);
				base = 0;
				step = 2. / n1;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i <= n2; i++) {
					mlib_d64 dat1 = src[i];
					mlib_d64 dat2 = src[n1 - i];

					dst[i] = base * dat1;
					dst[n1 - i] = base * dat2;
					base += step;
				}
			} else {
				res0 = __mlib_SignalGenBartlett_S16(pwin, n);
				res1 =
				    __mlib_SignalMulWindow_S16_S16(
							dst, src, pwin, n);
				__mlib_free(pwin);
			}
		}
	}
	return ((res0 == MLIB_SUCCESS) && (res1 == MLIB_SUCCESS))
	    ? MLIB_SUCCESS : MLIB_FAILURE;
}

mlib_status
mlib_SignalMulBartlett_S16_S16_i(
    mlib_s16 *dst,
    const mlib_s16 *src,
    mlib_s32 n,
    mlib_s32 i0,
    mlib_s32 i1)
{
/* check for obvious errors */

	if ((dst == NULL) || (n <= 1) || (src == NULL))
		return (MLIB_FAILURE);

	{
		mlib_d64 step, base;
		mlib_s32 n1, i;

		n1 = n - 1;
		step = 2. / n1;
		base = i0 * step;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = i0; i < i1; i++) {
			mlib_d64 dat1 = src[i], dat2 = src[n1 - i];

			dst[i] = base * dat1;
			dst[n1 - i] = base * dat2;
			base += step;
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_SignalMulBartlett_S16S_S16S(
    mlib_s16 *dst,
    const mlib_s16 *src,
    mlib_s32 n)
{
	mlib_status res0 = MLIB_SUCCESS;
	mlib_status res1 = MLIB_SUCCESS;
/* check for obvious errors */

	if ((dst == NULL) || (n <= 1) || (src == NULL))
		return (MLIB_FAILURE);

	{
		if (CACHE_SIZE > sizeof (mlib_s16) * n) {
			mlib_d64 window[CACHE_SIZE / 8];
			mlib_s16 *pwin = (mlib_s16 *)window;

			res0 = __mlib_SignalGenBartlett_S16(pwin, n);
			res1 = __mlib_SignalMulWindow_S16S_S16S(
					dst, src, pwin, n);
		} else {
			mlib_s16 *pwin = __mlib_malloc(sizeof (mlib_s16) * n);
			if (pwin == NULL) {
				mlib_d64 step, base;
				mlib_s32 n2, n1, i, j, e0, e1;

				e1 = (n - 1) << 1;
				e0 = e1 + 1;
				n1 = (n - 1);
				n2 = (n1) >> 1;
				base = 0;
				step = 2. / n1;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0, j = 0; i <= n2; i++, j += 2) {
					mlib_d64 src0 = src[j];
					mlib_d64 src1 = src[j + 1];
					mlib_d64 src2 = src[e1 - j];
					mlib_d64 src3 = src[e0 - j];

					dst[j] = base * src0;
					dst[j + 1] = base * src1;
					dst[e1 - j] = base * src2;
					dst[e0 - j] = base * src3;
					base += step;
				}
			} else {
				res0 = __mlib_SignalGenBartlett_S16(pwin, n);
				res1 =
				    __mlib_SignalMulWindow_S16S_S16S(
							dst, src, pwin, n);
				__mlib_free(pwin);
			}
		}
	}
	return ((res0 == MLIB_SUCCESS) && (res1 == MLIB_SUCCESS))
	    ? MLIB_SUCCESS : MLIB_FAILURE;
}

mlib_status
mlib_SignalMulBartlett_S16S_S16S_i(
    mlib_s16 *dst,
    const mlib_s16 *src,
    mlib_s32 n,
    mlib_s32 i0,
    mlib_s32 i1)
{
/* check for obvious errors */

	if ((dst == NULL) || (n <= 1) || (src == NULL))
		return (MLIB_FAILURE);

	{
		mlib_d64 step, base;
		mlib_s32 n1, i, j, e0, e1;

		e1 = ((n - 1) << 1);
		e0 = e1 + 1;
		n1 = (n - 1);
		step = 2. / n1;
		base = step * i0;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = i0, j = i0 << 1; i < i1; i++, j += 2) {
			mlib_d64 src0 = src[j];
			mlib_d64 src1 = src[j + 1];
			mlib_d64 src2 = src[e1 - j];
			mlib_d64 src3 = src[e0 - j];

			dst[j] = base * src0;
			dst[j + 1] = base * src1;
			dst[e1 - j] = base * src2;
			dst[e0 - j] = base * src3;
			base += step;
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_SignalMulBartlett_S16(
    mlib_s16 *data,
    mlib_s32 n)
{
	mlib_status res0 = MLIB_SUCCESS;
	mlib_status res1 = MLIB_SUCCESS;
/* check for obvious errors */

	if ((data == NULL) || (n <= 1))
		return (MLIB_FAILURE);

	{
		if (CACHE_SIZE > sizeof (mlib_s16) * n) {
			mlib_d64 window[CACHE_SIZE / 8];
			mlib_s16 *pwin = (mlib_s16 *)window;

			res0 = __mlib_SignalGenBartlett_S16(pwin, n);
			res1 = __mlib_SignalMulWindow_S16_S16(
					data, data, pwin, n);
		} else {
			mlib_s16 *pwin = __mlib_malloc(sizeof (mlib_s16) * n);
			if (pwin == NULL) {
				mlib_d64 step, base;
				mlib_s32 n2, n1, i;

				n1 = n - 1;
				n2 = (n1 >> 1);
				base = 0;
				step = 2. / n1;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (i = 0; i <= n2; i++) {
					mlib_d64 dat1 = data[i];
					mlib_d64 dat2 = data[n1 - i];

					data[i] = base * dat1;
					data[n1 - i] = base * dat2;
					base += step;
				}
			} else {
				res0 = __mlib_SignalGenBartlett_S16(pwin, n);
				res1 =
				    __mlib_SignalMulWindow_S16_S16(
							data, data, pwin, n);
				__mlib_free(pwin);
			}
		}
	}
	return ((res0 == MLIB_SUCCESS) && (res1 == MLIB_SUCCESS))
	    ? MLIB_SUCCESS : MLIB_FAILURE;

}

/* *********************************************************** */
