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

#pragma ident	"@(#)mlib_s_SignalMulWindow.c	9.3	07/11/05 SMI"

/*
 * FUNCTIONS
 *      mlib_SignalMulWindow_S16 - multiply source data on
 *          coefficients of the corresponding window
 *
 *      mlib_SignalMulWindow_S16S - multiply stereo source
 *          data on coefficients of the corresponding window
 *
 *      mlib_SignalMulWindow_S16_S16 - multiply source data on
 *          coefficients of the corresponding window with results
 *          in destination array
 *
 *      mlib_SignalMulWindow_S16S_S16S - multiply stereo source
 *          data on coefficients of the corresponding window with
 *          result in destination array
 *
 * SYNOPSIS
 *      mlib_status mlib_SignalMulWindow_S16(mlib_s16       *data,
 *                                           const mlib_s16 *window,
 *                                           mlib_s32       n)
 *
 *      mlib_status mlib_SignalMulWindow_S16S(mlib_s16       *data,
 *                                            const mlib_s16 *window,
 *                                            mlib_s32       n)
 *
 *      mlib_status mlib_SignalMulWindow_S16_S16(mlib_s16               *dst,
 *                                               const mlib_s16         *src,
 *                                               const mlib_s16 *window
 *                                               mlib_s32               n)
 *
 *      mlib_status mlib_SignalMulWindow_S16S_S16S(mlib_s16       *dst,
 *                                                 const mlib_s16 *src,
 *                                                 const mlib_s16 *window,
 *                                                 mlib_s32       n)
 *
 * ARGUMENTS
 *      data     Input and output signal array
 *      dst      Output signal array
 *      src      Input signal array
 *      window   Window coefficient array
 *      n        Length of window array
 *
 * DESCRIPTION
 *      Mono        :   dst[i] = src[i] * window[i]    i = 0..(N-1)
 *      Stereo      :   dst[i] = src[i] * window[i/2]  i = 0..(2*N-1)
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

#pragma weak mlib_SignalMulWindow_S16 = __mlib_SignalMulWindow_S16
#pragma weak mlib_SignalMulWindow_S16S = __mlib_SignalMulWindow_S16S
#pragma weak mlib_SignalMulWindow_S16S_S16S = \
	__mlib_SignalMulWindow_S16S_S16S
#pragma weak mlib_SignalMulWindow_S16_S16 = __mlib_SignalMulWindow_S16_S16

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_SignalMulWindow_S16) mlib_SignalMulWindow_S16
    __attribute__((weak, alias("__mlib_SignalMulWindow_S16")));
__typeof__(__mlib_SignalMulWindow_S16S) mlib_SignalMulWindow_S16S
    __attribute__((weak, alias("__mlib_SignalMulWindow_S16S")));
__typeof__(__mlib_SignalMulWindow_S16S_S16S) mlib_SignalMulWindow_S16S_S16S
    __attribute__((weak, alias("__mlib_SignalMulWindow_S16S_S16S")));
__typeof__(__mlib_SignalMulWindow_S16_S16) mlib_SignalMulWindow_S16_S16
    __attribute__((weak, alias("__mlib_SignalMulWindow_S16_S16")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	SAT_S16(x)						\
	((x == MLIB_S16_MIN) ? MLIB_S16_MAX : x)

/* *********************************************************** */
#define	LOAD_X_AND_Y  \
{  \
	if (0 == ((mlib_addr)src & 0xf)) {\
		dx = _mm_load_si128(dpx);\
	} else {\
		dx = _mm_loadu_si128(dpx);\
	}\
	if (0 == ((mlib_addr)window & 0xf)) {\
		dy = _mm_load_si128(dpy);\
	} else {\
		dy = _mm_loadu_si128(dpy);\
	}\
}

/* *********************************************************** */
#define	LOAD_X1_AND_Y  \
{  \
	if (0 == ((mlib_addr)src & 0xf)) {\
		dx_lo = _mm_load_si128(dpx);\
	} else {\
		dx_lo = _mm_loadu_si128(dpx);\
	}\
	dpx++; \
	if (0 == ((mlib_addr)src & 0xf)) {\
		dx_hi = _mm_load_si128(dpx);\
	} else {\
		dx_hi = _mm_loadu_si128(dpx);\
	}\
	if (0 == ((mlib_addr)window & 0xf)) {\
		dy = _mm_load_si128(dpy);\
	} else {\
		dy = _mm_loadu_si128(dpy);\
	}\
}
/* *********************************************************** */
#define	STORE_Z     \
		if (0 == ((mlib_addr)dst & 0xf)) { \
			_mm_store_si128(dpz, dz); \
		} else {  \
			_mm_storeu_si128(dpz, dz);  \
		}

/* *********************************************************** */

mlib_status
__mlib_SignalMulWindow_S16_S16(
    mlib_s16 *dst,
    const mlib_s16 *src,
    const mlib_s16 *window,
    mlib_s32 n)
{
	mlib_s32 i;
	mlib_s16 dtmp;
	int nlen;
	__m128i dx, dy, dz;
	__m128i *dpx, *dpy, *dpz;
	__m128i dtmp_hi, dtmp_lo;

/* check for obvious errors */
	if ((dst == NULL) || (n <= 0) || (src == NULL) || (window == NULL))
		return (MLIB_FAILURE);

	nlen = n >> 3;
	dpx = (__m128i *) src;
	dpy = (__m128i *) window;
	dpz = (__m128i *) dst;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < nlen; i++) {
		LOAD_X_AND_Y;
		dtmp_hi = _mm_mulhi_epi16(dx, dy);
		dtmp_lo = _mm_mullo_epi16(dx, dy);
		dx = _mm_unpackhi_epi16(dtmp_lo, dtmp_hi);
		dy = _mm_unpacklo_epi16(dtmp_lo, dtmp_hi);
		dx = _mm_srai_epi32(dx, 15);
		dy = _mm_srai_epi32(dy, 15);
		dz = _mm_packs_epi32(dy, dx);
		STORE_Z;
		dpx++;
		dpy++;
		dpz++;
	}

	if ((nlen << 3) != n) {
		for (i = (nlen << 3); i < n; i++) {
			dtmp = (mlib_s32)(src[i] * window[i]) >> 15;
			dst[i] = SAT_S16(dtmp);
		}
	}
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_SignalMulWindow_S16S_S16S(
    mlib_s16 *dst,
    const mlib_s16 *src,
    const mlib_s16 *window,
    mlib_s32 n)
{
	mlib_s32 i, j;
	int nlen;
	__m128i dx_hi, dx_lo, dy, dz;
	__m128i *dpx, *dpy, *dpz;
	__m128i dtmp_hi, dtmp_lo;
	__m128i ddata_hi, ddata_lo;
	__m128i dresult_hi, dresult_lo;
	mlib_s16 dtmp1, dtmp2;

/* check for obvious errors */

	if ((dst == NULL) || (n <= 0) || (src == NULL) || (window == NULL))
		return (MLIB_FAILURE);

	dpx = (__m128i *) src;
	dpy = (__m128i *) window;
	dpz = (__m128i *) dst;
	nlen = 0;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	if (n >= 16) {
		nlen = n >> 3;
		for (i = 0; i < nlen; i++) {
			LOAD_X1_AND_Y;
			ddata_hi = _mm_unpackhi_epi16(dy, dy);
			ddata_lo = _mm_unpacklo_epi16(dy, dy);

			dtmp_hi = _mm_mulhi_epi16(dx_lo, ddata_lo);
			dtmp_lo = _mm_mullo_epi16(dx_lo, ddata_lo);
			dresult_hi = _mm_unpackhi_epi16(dtmp_lo, dtmp_hi);
			dresult_lo = _mm_unpacklo_epi16(dtmp_lo, dtmp_hi);
			dresult_hi = _mm_srai_epi32(dresult_hi, 15);
			dresult_lo = _mm_srai_epi32(dresult_lo, 15);
			dz = _mm_packs_epi32(dresult_lo, dresult_hi);
			STORE_Z;
			dpz++;

			dtmp_hi = _mm_mulhi_epi16(dx_hi, ddata_hi);
			dtmp_lo = _mm_mullo_epi16(dx_hi, ddata_hi);
			dresult_hi = _mm_unpackhi_epi16(dtmp_lo, dtmp_hi);
			dresult_lo = _mm_unpacklo_epi16(dtmp_lo, dtmp_hi);
			dresult_hi = _mm_srai_epi32(dresult_hi, 15);
			dresult_lo = _mm_srai_epi32(dresult_lo, 15);
			dz = _mm_packs_epi32(dresult_lo, dresult_hi);
			STORE_Z;

			dpx++;
			dpy++;
			dpz++;
		}
	}
	if ((nlen << 3) != n) {
		mlib_s32 ncurrent;
		ncurrent = nlen << 4;
		for (i = ncurrent, j = (nlen << 3); j < n; i += 2, j++) {
			dtmp1 = (mlib_s32)(src[i] * window[j]) >> 15;
			dtmp2 = (mlib_s32)(src[i + 1] * window[j]) >> 15;
			dst[i] = SAT_S16(dtmp1);
			dst[i + 1] = SAT_S16(dtmp2);
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_SignalMulWindow_S16(
    mlib_s16 *data,
    const mlib_s16 *window,
    mlib_s32 n)
{
	return (__mlib_SignalMulWindow_S16_S16(data, data, window, n));
}

/* *********************************************************** */

mlib_status
__mlib_SignalMulWindow_S16S(
    mlib_s16 *data,
    const mlib_s16 *window,
    mlib_s32 n)
{
	return (__mlib_SignalMulWindow_S16S_S16S(data, data, window, n));
}

/* *********************************************************** */
