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
 * Copyright 2007 Sun Microsystems, Inc.  All rights reserved.
 * Use is subject to license terms.
 */

#pragma ident	"@(#)mlib_i_SignalEmphasize.c	9.3	07/11/05 SMI"

/*
 * FUNCTIONS
 *      mlib_SignalEmphasizeInit_[S16|S16S]_[S16|S16S] - allocates memory for
 *          internal state structure and converts the filter
 *          coefficients into internal representation
 *
 *      mlib_SignalEmphasize_[S16|S16S]_[S16|S16S]_Sat - apply the preemphasizer
 *          to one packet of signal and update the filter states
 *
 *      mlib_SignalEmphasizeFree_[S16|S16S]_[S16|S16S] - releases the memory
 *          allocated for the internal states structure
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_SignalEmphasizeInit_S16S_S16S(void     **filter,
 *                                                     mlib_f32 alpha);
 *      mlib_status mlib_SignalEmphasizeInit_S16_S16(void     **filter,
 *                                                   mlib_f32 alpha);
 *      mlib_status mlib_SignalEmphasize_S16S_S16S_Sat(mlib_s16       *dst,
 *                                                     const mlib_s16 *src,
 *                                                     void           *filter,
 *                                                     mlib_s32       n);
 *      mlib_status mlib_SignalEmphasize_S16_S16_Sat(mlib_s16       *dst,
 *                                                   const mlib_s16 *src,
 *                                                   void           *filter,
 *                                                   mlib_s32       n);
 *      void mlib_SignalEmphasizeFree_S16S_S16S(void *filter);
 *      void mlib_SignalEmphasizeFree_S16_S16(void *filter);
 *
 * ARGUMENTS
 *      filter   Internal filter structure
 *      alpha    Emphasizing coefficients, 0 < alpha < 1.0
 *      dst      Output signal array
 *      src      Input signal array
 *      n        Number of samples in the input signal array
 *
 * DESCRIPTION
 *     Pre-Emphasizing:     z[i] = x[i] - alpha*x[i-1]
 *
 */

#include <mlib_signal.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_SignalEmphasizeFree_S16S_S16S = \
	__mlib_SignalEmphasizeFree_S16S_S16S
#pragma weak mlib_SignalEmphasizeFree_S16_S16 = \
	__mlib_SignalEmphasizeFree_S16_S16
#pragma weak mlib_SignalEmphasizeInit_S16S_S16S = \
	__mlib_SignalEmphasizeInit_S16S_S16S
#pragma weak mlib_SignalEmphasizeInit_S16_S16 = \
	__mlib_SignalEmphasizeInit_S16_S16
#pragma weak mlib_SignalEmphasize_S16S_S16S_Sat = \
	__mlib_SignalEmphasize_S16S_S16S_Sat
#pragma weak mlib_SignalEmphasize_S16_S16_Sat = \
	__mlib_SignalEmphasize_S16_S16_Sat

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_SignalEmphasizeFree_S16S_S16S)
    mlib_SignalEmphasizeFree_S16S_S16S
    __attribute__((weak, alias("__mlib_SignalEmphasizeFree_S16S_S16S")));
__typeof__(__mlib_SignalEmphasizeFree_S16_S16) mlib_SignalEmphasizeFree_S16_S16
    __attribute__((weak, alias("__mlib_SignalEmphasizeFree_S16_S16")));
__typeof__(__mlib_SignalEmphasizeInit_S16S_S16S)
    mlib_SignalEmphasizeInit_S16S_S16S
    __attribute__((weak, alias("__mlib_SignalEmphasizeInit_S16S_S16S")));
__typeof__(__mlib_SignalEmphasizeInit_S16_S16) mlib_SignalEmphasizeInit_S16_S16
    __attribute__((weak, alias("__mlib_SignalEmphasizeInit_S16_S16")));
__typeof__(__mlib_SignalEmphasize_S16S_S16S_Sat)
    mlib_SignalEmphasize_S16S_S16S_Sat
    __attribute__((weak, alias("__mlib_SignalEmphasize_S16S_S16S_Sat")));
__typeof__(__mlib_SignalEmphasize_S16_S16_Sat) mlib_SignalEmphasize_S16_S16_Sat
    __attribute__((weak, alias("__mlib_SignalEmphasize_S16_S16_Sat")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	MLIB_EMPH	12
#define	MLIB_MUL	32767.
#define	MLIB_DIV	1./32768.

/* *********************************************************** */

typedef struct mlib_emphasize
{
    mlib_s32 type;
    mlib_f32 alpha;
    mlib_f32 last0, last1;
} mlib_emphasize_struct;

#ifdef _NO_LONGLONG
/* *********************************************************** */

mlib_status
__mlib_SignalEmphasize_S16_S16_Sat(
    mlib_s16 *dst,
    const mlib_s16 *src,
    void *filter,
    mlib_s32 n)
{
	mlib_emphasize_struct *fist = filter;

#ifndef MLIB_USE_FTOI_CLAMPING
	mlib_d64 alpha, last, dd1, dd2;
	mlib_s32 i, t = n >> 1, d1, d2;
#else /* MLIB_USE_FTOI_CLAMPING */
	mlib_d64 alpha, last;
	mlib_s32 i, t = n >> 1;
#endif /* MLIB_USE_FTOI_CLAMPING */

/* check for obvious errors */

	if ((fist == NULL) || (n <= 0) || (src == 0) || (dst == 0) ||
		(fist->type != MLIB_EMPH))
		return (MLIB_FAILURE);

	last = fist->last0;
	alpha = fist->alpha;

#ifndef MLIB_USE_FTOI_CLAMPING

	for (i = 0; i < t; i++) {
		dd1 = 65536. * (src[2 * i] - alpha * last);

		if (dd1 > MLIB_S32_MAX)
			d1 = MLIB_S32_MAX;
		else if (dd1 < MLIB_S32_MIN)
			d1 = MLIB_S32_MIN;
		else
			d1 = (mlib_s32)(dd1);
		dst[2 * i] = d1 >> 16;
		last = src[2 * i];

		dd2 = 65536. * (src[2 * i + 1] - alpha * last);

		if (dd2 > MLIB_S32_MAX)
			d2 = MLIB_S32_MAX;
		else if (dd2 < MLIB_S32_MIN)
			d2 = MLIB_S32_MIN;
		else
			d2 = (mlib_s32)(dd2);
		dst[2 * i + 1] = d2 >> 16;
		last = src[2 * i + 1];
	}

	if (n & 1) {
		dd1 = 65536. * (src[n - 1] - alpha * last);

		if (dd1 > MLIB_S32_MAX)
			d1 = MLIB_S32_MAX;
		else if (dd1 < MLIB_S32_MIN)
			d1 = MLIB_S32_MIN;
		else
			d1 = (mlib_s32)(dd1);
		dst[n - 1] = d1 >> 16;
		last = src[n - 1];
	}
#else /* MLIB_USE_FTOI_CLAMPING */

	for (i = 0; i < t; i++) {
		dst[2 * i] =
			(mlib_s32)(65536. * (src[2 * i] - alpha * last)) >> 16;
		last = src[2 * i];
		dst[2 * i + 1] =
		    (mlib_s32)(65536. * (src[2 * i + 1] - alpha * last)) >> 16;
		last = src[2 * i + 1];
	}

	if (n & 1) {
		dst[n - 1] =
			(mlib_s32)(65536. * (src[n - 1] - alpha * last)) >> 16;
		last = src[n - 1];
	}
#endif /* MLIB_USE_FTOI_CLAMPING */
	fist->last0 = (mlib_f32)last;
	return (MLIB_SUCCESS);
}
#else
/* ********************************************************* */

mlib_status
__mlib_SignalEmphasize_S16_S16_Sat(
    mlib_s16 *dst,
    const mlib_s16 *src,
    void *filter,
    mlib_s32 n)
{
	mlib_emphasize_struct *fist = filter;
	mlib_s8 tmp;
	mlib_s8 sshift;
	mlib_s16 last;
	mlib_u32 udata;
	mlib_s32 i, t = n >> 1;
	mlib_s32 s32;
	mlib_s64 s64;

/* check for obvious errors */

	if ((fist == NULL) || (n <= 0) || (src == 0) || (dst == 0) ||
	    (fist->type != MLIB_EMPH))
		return (MLIB_FAILURE);

	s32 = (*((mlib_s32*)(&fist->alpha)));
	tmp = ((s32 & 0x7f800000) >> 23) - 127;
	sshift = (tmp ^ (tmp >> 31)) - (tmp >> 31);
	udata = ((s32 & 0x7fffff) | 0x800000) >> 0;

	last = (mlib_s16)fist->last0;

	for (i = 0; i < t; i++) {
		s64 = (((mlib_s64)src[2 * i] << 23) -
				(((mlib_s64)udata * last) >> sshift)) >> 23;

		if (s64 > MLIB_S16_MAX)
			dst[2 * i] = MLIB_S16_MAX;
		else if (s64 < MLIB_S16_MIN)
			dst[2 * i] = MLIB_S16_MIN;
		else
			dst[2 * i] = s64;
		last = src[2 * i];

		s64 = (((mlib_s64)src[2 * i + 1] << 23) -
				(((mlib_s64)udata * last) >> sshift)) >> 23;
		if (s64 > MLIB_S16_MAX)
			dst[2 * i + 1] = MLIB_S16_MAX;
		else if (s64 < MLIB_S16_MIN)
			dst[2 * i + 1] = MLIB_S16_MIN;
		else
			dst[2 * i + 1] = s64;
		last = src[2 * i + 1];
	}

	if (n & 1) {
		s64 = (((mlib_s64)src[n - 1] << 23) -
				(((mlib_s64)udata * last) >> sshift)) >> 23;

		if (s64 > MLIB_S16_MAX)
			dst[n - 1] = MLIB_S16_MAX;
		else if (s64 < MLIB_S16_MIN)
			dst[n - 1] = MLIB_S16_MIN;
		else
			dst[n - 1] = s64;
		last = src[n - 1];
	}
	fist->last0 = (mlib_f32)last;

	return (MLIB_SUCCESS);
}
#endif

/* *********************************************************** */
mlib_status
__mlib_SignalEmphasizeInit_S16_S16(
	void **filter,
    mlib_f32 alpha)
{
	mlib_emphasize_struct *fist;

/* check for obvious errors */

	if (filter == NULL)
		return (MLIB_FAILURE);

	fist = __mlib_malloc(sizeof (mlib_emphasize_struct));

	if (fist == NULL)
		return (MLIB_FAILURE);
	fist->type = MLIB_EMPH;
	fist->alpha = alpha;
	fist->last0 = 0;
	fist->last1 = 0;
	*filter = fist;

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

void
__mlib_SignalEmphasizeFree_S16_S16(
    void *filter)
{
	mlib_emphasize_struct *fist = filter;

/* check for obvious errors */

	if ((filter == NULL) || (fist->type != MLIB_EMPH))
		return; /* MLIB_FAILURE */

	fist->type = 0;
	__mlib_free(filter);
}
#ifdef _NO_LONGLONG
/* *********************************************************** */
mlib_status
__mlib_SignalEmphasize_S16S_S16S_Sat(
	mlib_s16 *dst,
	const mlib_s16 *src,
	void *filter,
	mlib_s32 n)
{
	mlib_emphasize_struct *fist = filter;

#ifndef MLIB_USE_FTOI_CLAMPING
	mlib_d64 alpha, last0, last1, dd1, dd2;
	mlib_s32 i, d1, d2;
#else /* MLIB_USE_FTOI_CLAMPING */
	mlib_d64 alpha, last0, last1;
	mlib_s32 i;
#endif /* MLIB_USE_FTOI_CLAMPING */
/* check for obvious errors */

	if ((fist == NULL) || (n <= 0) || (src == 0) || (dst == 0) ||
		(fist->type != MLIB_EMPH))
		return (MLIB_FAILURE);

	last0 = fist->last0;
	last1 = fist->last1;
	alpha = fist->alpha;

#ifndef MLIB_USE_FTOI_CLAMPING
	for (i = 0; i < n; i++) {
		dd1 = 65536. * (src[2 * i] - alpha * last0);

		if (dd1 > MLIB_S32_MAX)
			d1 = MLIB_S32_MAX;
		else if (dd1 < MLIB_S32_MIN)
			d1 = MLIB_S32_MIN;
		else
			d1 = (mlib_s32)(dd1);
		dst[2 * i] = d1 >> 16;
		last0 = src[2 * i];

		dd2 = 65536. * (src[2 * i + 1] - alpha * last1);

		if (dd2 > MLIB_S32_MAX)
			d2 = MLIB_S32_MAX;
		else if (dd2 < MLIB_S32_MIN)
			d2 = MLIB_S32_MIN;
		else
			d2 = (mlib_s32)(dd2);
		dst[2 * i + 1] = d2 >> 16;
		last1 = src[2 * i + 1];
	}
#else /* MLIB_USE_FTOI_CLAMPING */
	for (i = 0; i < n; i++) {
		dst[2 * i] =
			(mlib_s32)(65536. * (src[2 * i] - alpha * last0)) >> 16;
		last0 = src[2 * i];
		dst[2 * i + 1] =
		    (mlib_s32)(65536. * (src[2 * i + 1] - alpha * last1)) >> 16;
		last1 = src[2 * i + 1];
	}
#endif /* MLIB_USE_FTOI_CLAMPING */
	fist->last0 = last0;
	fist->last1 = last1;

	return (MLIB_SUCCESS); }
#else
/* *********************************************************** */

mlib_status
__mlib_SignalEmphasize_S16S_S16S_Sat(
	mlib_s16 *dst,
	const mlib_s16 *src,
	void *filter,
	mlib_s32 n)
{
	mlib_emphasize_struct *fist = filter;
	mlib_s8 sshift, sign;
	mlib_s16 last0, last1;
	mlib_u32 udata;
	mlib_s32 s32;
	mlib_s32 tmp;
	mlib_s32 i;
	mlib_s64 s64;
/* check for obvious errors */

	if ((fist == NULL) || (n <= 0) || (src == 0) || (dst == 0) ||
	    (fist->type != MLIB_EMPH))
		return (MLIB_FAILURE);

	s32 = (*((mlib_s32*)(&fist->alpha)));
	tmp = ((s32 & 0x7f800000) >> 23) - 127;
	sshift = (tmp ^ (tmp >> 31)) - (tmp >> 31);
	udata = (s32 & 0x7fffff) | 0x800000;

	last0 = (mlib_s16)fist->last0;
	last1 = (mlib_s16)fist->last1;

	for (i = 0; i < n; i++) {
		s64 = (((mlib_s64)src[2 * i] << 23) -
				(((mlib_s64)udata * last0) >> sshift)) >> 23;
		if (s64 > MLIB_S16_MAX)
			dst[2 * i] = MLIB_S16_MAX;
		else if (s64 < MLIB_S16_MIN)
			dst[2 * i] = MLIB_S16_MIN;
		else
			dst[2 * i] = s64;
		last0 = src[2 * i];

		s64 = (((mlib_s64)src[2 * i + 1] << 23) -
				(((mlib_s64)udata * last1) >> sshift)) >> 23;
		if (s64 > MLIB_S16_MAX)
			dst[2 * i + 1] = MLIB_S16_MAX;
		else if (s64 < MLIB_S16_MIN)
			dst[2 * i + 1] = MLIB_S16_MIN;
		else
			dst[2 * i + 1] = s64;
		last1 = src[2 * i + 1];
	}

	fist->last0 = (mlib_f32)last0;
	fist->last1 = (mlib_f32)last1;

	return (MLIB_SUCCESS);
}
#endif
/* *********************************************************** */

mlib_status
__mlib_SignalEmphasizeInit_S16S_S16S(
    void **filter,
    mlib_f32 alpha)
{
	return (__mlib_SignalEmphasizeInit_S16_S16(filter, alpha));
}

/* *********************************************************** */

void
__mlib_SignalEmphasizeFree_S16S_S16S(
    void *filter)
{
	__mlib_SignalEmphasizeFree_S16_S16(filter);
}

/* *********************************************************** */
