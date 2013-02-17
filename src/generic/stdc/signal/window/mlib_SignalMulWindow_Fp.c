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

#pragma ident	"@(#)mlib_SignalMulWindow_Fp.c	9.2	07/10/09 SMI"

/*
 * FUNCTIONS
 *      mlib_SignalMulWindow_F32 - multiply source data on
 *          coefficients of the corresponding window
 *
 *      mlib_SignalMulWindow_F32S - multiply stereo source
 *          data on coefficients of the corresponding window
 *
 *      mlib_SignalMulWindow_F32_F32 - multiply source data on
 *          coefficients of the corresponding window with results
 *          in destination array
 *
 *      mlib_SignalMulWindow_F32S_F32S - multiply stereo source
 *          data on coefficients of the corresponding window with
 *          result in destination array
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_SignalMulWindow_F32( mlib_f32      *data,
 *                                           const mlib_f32 *window,
 *                                           mlib_s32       n);
 *      mlib_status mlib_SignalMulWindow_F32S( mlib_f32      *dst,
 *                                            const mlib_f32 *window,
 *                                            mlib_s32       n);
 *      mlib_status mlib_SignalMulWindow_F32S_F32S( mlib_f32      *dst,
 *                                                 const mlib_f32 *src,
 *                                                 const mlib_f32 *window,
 *                                                 mlib_s32       n);
 *      mlib_status mlib_SignalMulWindow_F32_F32( mlib_f32      *dst,
 *                                               const mlib_f32 *src,
 *                                               const mlib_f32 *window,
 *                                               mlib_s32       n);
 * ARGUMENTS
 *      data     Input and output signal array
 *      dst      Output signal array
 *      src      Input signal array
 *      window   Window coefficient array
 *      n        Length of window array
 *
 * DESCRIPTION
 *      Mono        :   dst[i] = src[i] * window[i]    i = 0..( N-1)
 *      Stereo      :   dst[i] = src[i] * window[i/2]  i = 0..(2*N-1)
 *
 */

#include <mlib_signal.h>
#include <mlib_SysMath.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_SignalMulWindow_F32 = __mlib_SignalMulWindow_F32
#pragma weak mlib_SignalMulWindow_F32S = __mlib_SignalMulWindow_F32S
#pragma weak mlib_SignalMulWindow_F32S_F32S = \
	__mlib_SignalMulWindow_F32S_F32S
#pragma weak mlib_SignalMulWindow_F32_F32 = __mlib_SignalMulWindow_F32_F32

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_SignalMulWindow_F32) mlib_SignalMulWindow_F32
    __attribute__((weak, alias("__mlib_SignalMulWindow_F32")));
__typeof__(__mlib_SignalMulWindow_F32S) mlib_SignalMulWindow_F32S
    __attribute__((weak, alias("__mlib_SignalMulWindow_F32S")));
__typeof__(__mlib_SignalMulWindow_F32S_F32S) mlib_SignalMulWindow_F32S_F32S
    __attribute__((weak, alias("__mlib_SignalMulWindow_F32S_F32S")));
__typeof__(__mlib_SignalMulWindow_F32_F32) mlib_SignalMulWindow_F32_F32
    __attribute__((weak, alias("__mlib_SignalMulWindow_F32_F32")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

typedef union
{
	mlib_d64 value;
	struct
	{
#ifdef _LITTLE_ENDIAN
		mlib_f32 i1, i0;
#else		   /* _LITTLE_ENDIAN */
		mlib_f32 i0, i1;
#endif		   /* _LITTLE_ENDIAN */
	} two_float;
} DOUBLE;

/* *********************************************************** */

mlib_status
__mlib_SignalMulWindow_F32_F32(
    mlib_f32 *dst,
    const mlib_f32 *src,
    const mlib_f32 *window,
    mlib_s32 n)
{
	mlib_s32 i = 0, j, n2 = (n >> 1);
	DOUBLE d, w, s;
	mlib_d64 *dp, *wp, *sp;

	if ((dst == NULL) || (n <= 0) || (src == NULL) || (window == NULL))
		return (MLIB_FAILURE);

	if ((mlib_addr)dst & 7) {
		dst[i] = src[i] * window[i];
		i++;
		n2--;
	}
	dp = (mlib_d64 *)(dst + i);
	sp = (mlib_d64 *)(src + i);
	wp = (mlib_d64 *)(window + i);

	if (((mlib_addr)sp & 7) == 0 && ((mlib_addr)wp & 7) == 0) {

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (j = 0; j < n2; j++, i += 2) {
			s.value = sp[j];
			w.value = wp[j];
			d.two_float.i0 = s.two_float.i0 * w.two_float.i0;
			d.two_float.i1 = s.two_float.i1 * w.two_float.i1;
			dp[j] = d.value;
		}

		if (i < (n - 1)) {
			dst[i] = src[i] * window[i];
			i++;
		}
	} else {
		if (((mlib_addr)sp & 7) == 0) {
			mlib_f32 win;

			wp = (mlib_d64 *)(window + i + 1);

			win = window[i];
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j = 0; j < n2; j++, i += 2) {
				s.value = sp[j];
				w.value = wp[j];
#ifdef _LITTLE_ENDIAN
				d.two_float.i1 = s.two_float.i1 * win;
				d.two_float.i0 =
				    s.two_float.i0 * w.two_float.i1;
				win = w.two_float.i0;
#else /* _LITTLE_ENDIAN */
				d.two_float.i0 = s.two_float.i0 * win;
				d.two_float.i1 =
				    s.two_float.i1 * w.two_float.i0;
				win = w.two_float.i1;
#endif /* _LITTLE_ENDIAN */
				dp[j] = d.value;
			}

			if (i < (n - 1)) {
				dst[i] = src[i] * win;
				i++;
			}
		} else if (((mlib_addr)wp & 7) == 0) {
			mlib_f32 sou;

			sp = (mlib_d64 *)(src + i + 1);

			sou = src[i];
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j = 0; j < (n2); j++, i += 2) {
				s.value = sp[j];
				w.value = wp[j];
#ifdef _LITTLE_ENDIAN
				d.two_float.i1 = sou * w.two_float.i1;
				d.two_float.i0 =
				    s.two_float.i1 * w.two_float.i0;
				sou = s.two_float.i0;
#else /* _LITTLE_ENDIAN */
				d.two_float.i0 = sou * w.two_float.i0;
				d.two_float.i1 =
				    s.two_float.i0 * w.two_float.i1;
				sou = s.two_float.i1;
#endif /* _LITTLE_ENDIAN */
				dp[j] = d.value;
			}

			if (i < (n - 1)) {
				dst[i] = sou * window[i];
				i++;
			}
		} else {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < n; i++)
				dst[i] = src[i] * window[i];
		}
	}
	if (i < n)
		dst[i] = src[i] * window[i];
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_SignalMulWindow_F32S_F32S(
    mlib_f32 *dst,
    const mlib_f32 *src,
    const mlib_f32 *window,
    mlib_s32 n)
{
	mlib_s32 i = 0, j;
	DOUBLE d, s;
	mlib_d64 *dp, *sp;

	if ((dst == NULL) || (n <= 0) || (src == NULL) || (window == NULL))
		return (MLIB_FAILURE);

	if (((mlib_addr)dst & 7) == ((mlib_addr)src & 7)) {
		if (((mlib_addr)dst & 7) == 0) {
			dp = (mlib_d64 *)dst;
			sp = (mlib_d64 *)src;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j = 0; j < n; j++) {
				mlib_f32 win = window[j];

				s.value = sp[j];
				d.two_float.i0 = s.two_float.i0 * win;
				d.two_float.i1 = s.two_float.i1 * win;
				dp[j] = d.value;
			}
		} else {
			mlib_f32 win = window[i];

			dst[i] = src[i] * win;
			i++;
			dp = (mlib_d64 *)(dst + 1);
			sp = (mlib_d64 *)(src + 1);
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j = 0; j < (n - 1); j++, i += 2) {
				s.value = sp[j];
#ifdef _LITTLE_ENDIAN
				d.two_float.i1 = s.two_float.i1 * win;
				win = window[j + 1];
				d.two_float.i0 = s.two_float.i0 * win;
#else /* _LITTLE_ENDIAN */
				d.two_float.i0 = s.two_float.i0 * win;
				win = window[j + 1];
				d.two_float.i1 = s.two_float.i1 * win;
#endif /* _LITTLE_ENDIAN */
				dp[j] = d.value;
			}

			dst[i] = src[i] * win;
		}
	} else {
		if (((mlib_addr)dst & 7) == 0) {
			mlib_f32 win = window[i];
			mlib_f32 temp = src[i];

			dp = (mlib_d64 *)dst;
			sp = (mlib_d64 *)(src + 1);
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j = 0; j < (n); j++, i += 2) {
				win = window[j];
				s.value = sp[j];
#ifdef _LITTLE_ENDIAN
				d.two_float.i1 = temp * win;
				d.two_float.i0 = s.two_float.i1 * win;
				temp = s.two_float.i0;
#else /* _LITTLE_ENDIAN */
				d.two_float.i0 = temp * win;
				d.two_float.i1 = s.two_float.i0 * win;
				temp = s.two_float.i1;
#endif /* _LITTLE_ENDIAN */
				dp[j] = d.value;
			}
		} else {
			mlib_f32 win = window[i];
			mlib_f32 temp = src[1];

			dst[i] = src[i] * win;
			i++;
			n--;
			dp = (mlib_d64 *)(dst + 1);
			sp = (mlib_d64 *)(src + 2);
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j = 0; j < (n); j++, i += 2) {
				s.value = sp[j];
#ifdef _LITTLE_ENDIAN
				d.two_float.i1 = temp * win;
				win = window[j + 1];
				d.two_float.i0 = s.two_float.i1 * win;
				temp = s.two_float.i0;
#else /* _LITTLE_ENDIAN */
				d.two_float.i0 = temp * win;
				win = window[j + 1];
				d.two_float.i1 = s.two_float.i0 * win;
				temp = s.two_float.i1;
#endif /* _LITTLE_ENDIAN */
				dp[j] = d.value;
			}

			dst[i] = temp * win;
		}
	}
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_SignalMulWindow_F32(
    mlib_f32 *data,
    const mlib_f32 *window,
    mlib_s32 n)
{
	mlib_s32 i = 0, j, n2 = (n >> 1);
	DOUBLE d, w;
	mlib_d64 *dp, *wp;

	if ((data == NULL) || (n <= 0) || (window == NULL))
		return (MLIB_FAILURE);

	if (((mlib_addr)data & 7) == ((mlib_addr)window & 7)) {
		if ((mlib_addr)data & 7) {
			data[i] *= window[i];
			i++;
			n2--;
		}

		dp = (mlib_d64 *)(data + i);
		wp = (mlib_d64 *)(window + i);
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (j = 0; j < n2; j++, i += 2) {
			d.value = dp[j];
			w.value = wp[j];
			d.two_float.i0 *= w.two_float.i0;
			d.two_float.i1 *= w.two_float.i1;
			dp[j] = d.value;
		}

		if (i < (n - 1)) {
			data[i] *= window[i];
			i++;
		}

		if (i < n)
			data[i] *= window[i];
	} else {
		mlib_f32 temp;

		if ((mlib_addr)data & 7) {
			data[i] *= window[i];
			i++;
		}

		dp = (mlib_d64 *)(data + i);
		wp = (mlib_d64 *)(window + i + 1);

		temp = window[i];
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (j = 0; j < (n2 - 1); j++, i += 2) {
			d.value = dp[j];
			w.value = wp[j];
#ifdef _LITTLE_ENDIAN
			d.two_float.i1 *= temp;
			d.two_float.i0 *= w.two_float.i1;
			temp = w.two_float.i0;
#else /* _LITTLE_ENDIAN */
			d.two_float.i0 *= temp;
			d.two_float.i1 *= w.two_float.i0;
			temp = w.two_float.i1;
#endif /* _LITTLE_ENDIAN */
			dp[j] = d.value;
		}

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (; i < n; i++)
			data[i] *= window[i];
	}
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_SignalMulWindow_F32S(
    mlib_f32 *dst,
    const mlib_f32 *window,
    mlib_s32 n)
{
	mlib_s32 i = 0, j;
	DOUBLE d;
	mlib_d64 *dp;
	mlib_f32 win;

	if ((dst == NULL) || (n <= 0) || (window == NULL))
		return (MLIB_FAILURE);

	if (((mlib_addr)dst & 7) == 0) {
		dp = (mlib_d64 *)dst;
		win = window[0];
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (j = 0; j < n - 1; j++) {
			d.value = dp[j];
			d.two_float.i0 *= win;
			d.two_float.i1 *= win;
			win = window[j + 1];
			dp[j] = d.value;
		}
		d.value = dp[j];
		d.two_float.i0 *= win;
		d.two_float.i1 *= win;
		dp[j] = d.value;
	} else {
		win = window[0];
		dst[0] *= win;

		dp = (mlib_d64 *)(dst + 1);
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (j = 0, i = 1; j < (n - 1); j++, i += 2) {
			d.value = dp[j];
#ifdef _LITTLE_ENDIAN
			d.two_float.i1 *= win;
			win = window[j + 1];
			d.two_float.i0 *= win;
#else /* _LITTLE_ENDIAN */
			d.two_float.i0 *= win;
			win = window[j + 1];
			d.two_float.i1 *= win;
#endif /* _LITTLE_ENDIAN */
			dp[j] = d.value;
		}

		dst[i] *= win;
	}
	return (MLIB_SUCCESS);
}

/* *********************************************************** */
