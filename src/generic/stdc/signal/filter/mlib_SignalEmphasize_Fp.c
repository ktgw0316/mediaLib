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

#pragma ident	"@(#)mlib_SignalEmphasize_Fp.c	9.3	07/10/09 SMI"

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
 *
 */

#include <mlib_signal.h>

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
	mlib_s32 i;

/* check for obvious errors */

	if ((fist == NULL) || (n <= 0) || (src == 0) || (dst == 0) ||
	    (fist->type != MLIB_EMPH))
		return (MLIB_FAILURE);

	last = fist->last0;
	alpha = fist->alpha;

	if ((mlib_addr)src & 7) {
		dst[0] = src[0] - alpha * last;
		last = src[0];
		src++;
		dst++;
		n--;
	}

	if ((mlib_addr)dst & 7) {
		for (i = 0; i < n >> 2; i++) {
			mlib_union64 src0, src1;

			src0.d64 = ((mlib_d64 *)src)[2 * i];
			src1.d64 = ((mlib_d64 *)src)[2 * i + 1];
			dst[4 * i] = src0.f32x2.i0 - alpha * last;
			dst[4 * i + 1] = src0.f32x2.i1 - alpha * src0.f32x2.i0;
			dst[4 * i + 2] = src1.f32x2.i0 - alpha * src0.f32x2.i1;
			dst[4 * i + 3] = src1.f32x2.i1 - alpha * src1.f32x2.i0;
			last = src1.f32x2.i1;
		}

		i <<= 2;
	} else {
		for (i = 0; i < n >> 3; i++) {
			mlib_union64 src0, dst0;
			mlib_union64 src1, dst1;
			mlib_union64 src2, dst2;
			mlib_union64 src3, dst3;

			src0.d64 = ((mlib_d64 *)src)[4 * i];
			src1.d64 = ((mlib_d64 *)src)[4 * i + 1];
			src2.d64 = ((mlib_d64 *)src)[4 * i + 2];
			src3.d64 = ((mlib_d64 *)src)[4 * i + 3];
			dst0.f32x2.i0 = src0.f32x2.i0 - alpha * last;
			dst0.f32x2.i1 = src0.f32x2.i1 - alpha * src0.f32x2.i0;
			dst1.f32x2.i0 = src1.f32x2.i0 - alpha * src0.f32x2.i1;
			dst1.f32x2.i1 = src1.f32x2.i1 - alpha * src1.f32x2.i0;
			dst2.f32x2.i0 = src2.f32x2.i0 - alpha * src1.f32x2.i1;
			dst2.f32x2.i1 = src2.f32x2.i1 - alpha * src2.f32x2.i0;
			dst3.f32x2.i0 = src3.f32x2.i0 - alpha * src2.f32x2.i1;
			dst3.f32x2.i1 = src3.f32x2.i1 - alpha * src3.f32x2.i0;
			last = src3.f32x2.i1;
			((mlib_d64 *)dst)[4 * i] = dst0.d64;
			((mlib_d64 *)dst)[4 * i + 1] = dst1.d64;
			((mlib_d64 *)dst)[4 * i + 2] = dst2.d64;
			((mlib_d64 *)dst)[4 * i + 3] = dst3.d64;
		}

		i <<= 3;
	}

	for (; i < n; i++) {
		dst[i] = src[i] - alpha * last;
		last = src[i];
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
	mlib_s32 i, n1;

/* check for obvious errors */

	if ((fist == NULL) || (n <= 0) || (src == 0) || (dst == 0) ||
	    (fist->type != MLIB_EMPH))
		return (MLIB_FAILURE);

	alpha = fist->alpha;

	n1 = n;

	if ((mlib_addr)src & 7) {
		last1 = fist->last0;
		dst[0] = src[0] - alpha * last1;
		last1 = src[0];
		src++;
		dst++;
		n--;
		last0 = fist->last1;
	} else {
		last0 = fist->last0;
		last1 = fist->last1;
	}

#if defined(MLIB_VIS) && MLIB_VIS >= 0x200	/* compiler specific */

	if (((mlib_addr)dst & 7) == 0) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < n; i++) {
			mlib_union64 src0, dst0;

			src0.d64 = ((mlib_d64 *)src)[i];

			dst0.f32x2.i0 = src0.f32x2.i0 - alpha * last0;
			dst0.f32x2.i1 = src0.f32x2.i1 - alpha * last1;

			last0 = src0.f32x2.i0;
			last1 = src0.f32x2.i1;
			((mlib_d64 *)dst)[i] = dst0.d64;
		}

	} else
#endif /* MLIB_VIS >= 0x200 ( compiler specific ) */
	{
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < n >> 1; i++) {
			mlib_union64 src0, src1;

			src0.d64 = ((mlib_d64 *)src)[2 * i];
			src1.d64 = ((mlib_d64 *)src)[2 * i + 1];
			dst[4 * i] = src0.f32x2.i0 - alpha * last0;
			dst[4 * i + 1] = src0.f32x2.i1 - alpha * last1;
			dst[4 * i + 2] = src1.f32x2.i0 - alpha * src0.f32x2.i0;
			dst[4 * i + 3] = src1.f32x2.i1 - alpha * src0.f32x2.i1;
			last0 = src1.f32x2.i0;
			last1 = src1.f32x2.i1;
		}

		i <<= 1;
	}

	for (; i < n; i++) {
		dst[2 * i] = src[2 * i] - alpha * last0;
		last0 = src[2 * i];
		dst[2 * i + 1] = src[2 * i + 1] - alpha * last1;
		last1 = src[2 * i + 1];
	}

	if (n != n1) {
		dst[2 * i] = src[2 * i] - alpha * last0;
		last0 = src[2 * i];
		fist->last0 = last1;
		fist->last1 = last0;
	} else {
		fist->last0 = last0;
		fist->last1 = last1;
	}

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
