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

#pragma ident	"@(#)mlib_s_SignalEmphasize_Fp.c	9.3	07/11/05 SMI"

/*
 * FUNCTIONS
 *      mlib_SignalEmphasizeInit_[F32|F32S]_[F32|F32S] - allocates memory for
 *          internal state structure and converts the filter
 *          coefficients into internal representation
 *
 *      mlib_SignalEmphasize_[F32|F32S]_[F32|F32S] - apply the preemphasizer to
 *          one packet of signal and update the filter states
 *
 *      mlib_SignalEmphasizeFree_[F32|F32S]_[F32|F32S] - releases the memory
 *          allocated for the internal states structure
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_SignalEmphasizeInit_F32S_F32S(void     **filter,
 *                                                     mlib_f32 alpha);
 *      mlib_status mlib_SignalEmphasizeInit_F32_F32(void     **filter,
 *                                                   mlib_f32 alpha);
 *      mlib_status mlib_SignalEmphasize_F32S_F32S(mlib_f32       *dst,
 *                                                 const mlib_f32 *src,
 *                                                 void           *filter,
 *                                                 mlib_s32       n);
 *      mlib_status mlib_SignalEmphasize_F32_F32(mlib_f32       *dst,
 *                                               const mlib_f32 *src,
 *                                               void           *filter,
 *                                               mlib_s32       n);
 *      void mlib_SignalEmphasizeFree_F32S_F32S(void *filter);
 *      void mlib_SignalEmphasizeFree_F32_F32(void *filter);
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
 */

#include <mlib_signal.h>
#include <mlib_sse_utils.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_SignalEmphasizeFree_F32S_F32S = \
	__mlib_SignalEmphasizeFree_F32S_F32S
#pragma weak mlib_SignalEmphasizeFree_F32_F32 = \
	__mlib_SignalEmphasizeFree_F32_F32
#pragma weak mlib_SignalEmphasizeInit_F32S_F32S = \
	__mlib_SignalEmphasizeInit_F32S_F32S
#pragma weak mlib_SignalEmphasizeInit_F32_F32 = \
	__mlib_SignalEmphasizeInit_F32_F32
#pragma weak mlib_SignalEmphasize_F32S_F32S = \
	__mlib_SignalEmphasize_F32S_F32S
#pragma weak mlib_SignalEmphasize_F32_F32 = __mlib_SignalEmphasize_F32_F32

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_SignalEmphasizeFree_F32S_F32S)
    mlib_SignalEmphasizeFree_F32S_F32S
    __attribute__((weak, alias("__mlib_SignalEmphasizeFree_F32S_F32S")));
__typeof__(__mlib_SignalEmphasizeFree_F32_F32) mlib_SignalEmphasizeFree_F32_F32
    __attribute__((weak, alias("__mlib_SignalEmphasizeFree_F32_F32")));
__typeof__(__mlib_SignalEmphasizeInit_F32S_F32S)
    mlib_SignalEmphasizeInit_F32S_F32S
    __attribute__((weak, alias("__mlib_SignalEmphasizeInit_F32S_F32S")));
__typeof__(__mlib_SignalEmphasizeInit_F32_F32) mlib_SignalEmphasizeInit_F32_F32
    __attribute__((weak, alias("__mlib_SignalEmphasizeInit_F32_F32")));
__typeof__(__mlib_SignalEmphasize_F32S_F32S) mlib_SignalEmphasize_F32S_F32S
    __attribute__((weak, alias("__mlib_SignalEmphasize_F32S_F32S")));
__typeof__(__mlib_SignalEmphasize_F32_F32) mlib_SignalEmphasize_F32_F32
    __attribute__((weak, alias("__mlib_SignalEmphasize_F32_F32")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	MLIB_EMPH	14

typedef struct mlib_emphasize
{
	mlib_s32 type;
	mlib_f32 alpha;
	mlib_f32 last0, last1;
} mlib_emphasize_struct;

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
__mlib_SignalEmphasize_F32_F32(
    mlib_f32 *dst,
    const mlib_f32 *src,
    void *filter,
    mlib_s32 n)
{
	mlib_emphasize_struct *fist = filter;
	mlib_f32 alpha, last;
	mlib_s32 i, t, j;

	__m128 sx0, sx1, sr0, salpha;

/* check for obvious errors */

	if ((fist == NULL) || (n <= 0) || (src == 0) || (dst == 0) ||
	    (fist->type != MLIB_EMPH))
		return (MLIB_FAILURE);

	last = fist->last0;
	alpha = fist->alpha;

	salpha = _mm_set_ps1(alpha);

	for (j = 0; (j < 4) && (j < n); j++) {
		dst[0] = src[0] - alpha * last;
		last = src[0];
		src++;
		dst++;
	}

	for (; ((j < n) && ((mlib_addr)(src) & 15)); j++) {
		dst[0] = src[0] - alpha * last;
		last = src[0];
		src++;
		dst++;
	}

	if (0 == (mlib_addr)dst & 15) {
		for (i = 0; i < ((n - j) >> 2); i++) {
			sx0 = _mm_load_ps(src);
			sx1 = _mm_loadu_ps(src - 1);
			sr0 = _mm_sub_ps(sx0, _mm_mul_ps(salpha, sx1));
			_mm_store_ps(dst, sr0);
			src += 4;
			dst += 4;
			last = *(src - 1);
		}
	} else {
		for (i = 0; i < ((n - j) >> 2); i++) {
			sx0 = _mm_load_ps(src);
			sx1 = _mm_loadu_ps(src - 1);
			sr0 = _mm_sub_ps(sx0, _mm_mul_ps(salpha, sx1));
			_mm_storeu_ps(dst, sr0);
			src += 4;
			dst += 4;
			last = *(src - 1);
		}
	}

	for (i = i * 4; i < (n - j); i++) {
		dst[0] = src[0] - alpha * last;
		last = src[0];
		src++;
		dst++;
	}

	fist->last0 = last;

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_SignalEmphasizeInit_F32_F32(
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
__mlib_SignalEmphasizeFree_F32_F32(
    void *filter)
{
	mlib_emphasize_struct *fist = filter;

/* check for obvious errors */

	if ((filter == NULL) || (fist->type != MLIB_EMPH))
		return; /* MLIB_FAILURE */

	fist->type = 0;
	__mlib_free(filter);
}

/* *********************************************************** */

mlib_status
__mlib_SignalEmphasize_F32S_F32S(
    mlib_f32 *dst,
    const mlib_f32 *src,
    void *filter,
    mlib_s32 n)
{
	mlib_emphasize_struct *fist = filter;
	mlib_f32 alpha, last0, last1;
	mlib_s32 i, j, n1, t;
	__m128 sx0, sx1, sr0, salpha;

/* check for obvious errors */

	if ((fist == NULL) || (n <= 0) || (src == 0) || (dst == 0) ||
	    (fist->type != MLIB_EMPH))
		return (MLIB_FAILURE);

	alpha = fist->alpha;
	last0 = fist->last0;
	last1 = fist->last1;
	n1 = n;
	t = 0;

	salpha = _mm_set_ps1(alpha);

	for (j = 0; (j < 2) && (j < n); j++) {
		dst[0] = src[0] - alpha * last0;
		dst[1] = src[1] - alpha * last1;
		last0 = src[0];
		last1 = src[1];
		src += 2;
		dst += 2;
	}

	if (j == n) {
		fist->last0 = last0;
		fist->last1 = last1;
		return (MLIB_SUCCESS);
	}

	if ((mlib_addr)src & 15) {
		t = 4 - ((mlib_addr)src - ((mlib_addr)src & ~15)) / 4;
		t = (t > (2 * (n - j))) ? 2 * (n - j): t;
		if (t == 1) {
			dst[0] = src[0] - alpha * last0;
			last0 = last1;
			last1 = src[0];
			src++;
			dst++;
			n--;
		} else if (t == 3) {
			dst[0] = src[0] - alpha * last0;
			dst[1] = src[1] - alpha * last1;
			last0 = src[0];
			dst[2] = src[2] - alpha * last0;
			last0 = src[1];
			last1 = src[2];
			src += 3;
			dst += 3;
			n -= 2;
		} else if (t == 2) {
			dst[0] = src[0] - alpha * last0;
			dst[1] = src[1] - alpha * last1;
			last0 = src[0];
			last1 = src[1];
			src += 2;
			dst += 2;
			n--;
		}
	}

	if (0 == (mlib_addr)dst & 15) {
		for (i = 0; i < ((n - j)/ 2); i++) {
			sx0 = _mm_load_ps(src);
			sx1 = _mm_loadu_ps(src - 2);
			sr0 = _mm_sub_ps(sx0, _mm_mul_ps(salpha, sx1));
			_mm_store_ps(dst, sr0);
			src += 4;
			dst += 4;
			last0 = *(src - 2);
			last1 = *(src - 1);
		}
	} else {
		for (i = 0; i < ((n - j)/ 2); i++) {
			sx0 = _mm_load_ps(src);
			sx1 = _mm_loadu_ps(src - 2);
			sr0 = _mm_sub_ps(sx0, _mm_mul_ps(salpha, sx1));
			_mm_storeu_ps(dst, sr0);
			src += 4;
			dst += 4;
			last0 = *(src - 2);
			last1 = *(src - 1);
		}
	}

	t =  2 * n1 - i * 4 - j * 2 - t;
	if (t == 2) {
		dst[0] = src[0] - alpha * last0;
		dst[1] = src[1] - alpha * last1;
		last0 = src[0];
		last1 = src[1];
	} else if (t == 3) {
		dst[0] = src[0] - alpha * (last0);
		dst[1] = src[1] - alpha * (last1);
		last0 = src[0];
		dst[2] = src[2] - alpha * last0;
		last0 = src[1];
		last1 = src[2];
	} else if (t == 1) {
		dst[0] = src[0] - alpha * last0;
		last0 = last1;
		last1 = src[0];
	}

	fist->last0 = last0;
	fist->last1 = last1;

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_SignalEmphasizeInit_F32S_F32S(
    void **filter,
    mlib_f32 alpha)
{
	return (__mlib_SignalEmphasizeInit_F32_F32(filter, alpha));
}

/* *********************************************************** */

void
__mlib_SignalEmphasizeFree_F32S_F32S(
    void *filter)
{
	__mlib_SignalEmphasizeFree_F32_F32(filter);
}

/* *********************************************************** */
