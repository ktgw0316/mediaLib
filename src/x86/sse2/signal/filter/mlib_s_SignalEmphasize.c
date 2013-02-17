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

#pragma ident	"@(#)mlib_s_SignalEmphasize.c	9.5	07/11/05 SMI"

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

/* *********************************************************** */

#define	EX	15
#define	MLIB_ROUND_Q15	(1 << (EX - 1))

/* *********************************************************** */

#define	MLIB_SATURATE_S16(X)					\
	(((X) > 0x7fff) ? MLIB_S16_MAX : ((X) < (-0x8000) ?	\
		MLIB_S16_MIN : (mlib_s16)(X)))

#define	ALPHASUB_S16(ds, sr1, sr2) {				\
	__m128i p = _mm_mulhi_epi16(talpha, sr2);		\
	__m128i q = _mm_mullo_epi16(talpha, sr2);		\
	__m128i t1 = _mm_unpacklo_epi16(q, p);			\
	__m128i t2 = _mm_unpackhi_epi16(q, p);			\
	t1 = _mm_add_epi32(t1, shiftr);				\
	t2 = _mm_add_epi32(t2, shiftr);				\
	t1 = _mm_srai_epi32(t1, EX);				\
	t2 = _mm_srai_epi32(t2, EX);				\
	__m128i t3 = _mm_packs_epi32(t1, t2);			\
	ds = _mm_adds_epi16(sr1, t3);				\
}

/* *********************************************************** */

mlib_status
__mlib_SignalEmphasize_S16_S16_Sat(
	mlib_s16 *dst,
	const mlib_s16 *src,
	void *filter,
	mlib_s32 n)
{
	mlib_emphasize_struct *fist = filter;

	__m128i sd1, sd2, dd;
	__m128i talpha;
	mlib_u8 *dp, *dl, *dend;
	mlib_u8 *sp1, *sp2, *sl1;
	mlib_s32 nn, emask;
	mlib_s16 last;
	mlib_s16 alpha;

/* check for obvious errors */

	if ((fist == NULL) || (n <= 0) || (src == 0) || (dst == 0) ||
		(fist->type != MLIB_EMPH))
		return (MLIB_FAILURE);

	dp = dl = (mlib_u8 *)dst;
	sp1 = sl1 = (mlib_u8 *)src;
	last = (mlib_s16)(fist->last0);

	alpha = (mlib_s16)((-(fist->alpha)) * 32768.); /* alpha << EX */
	talpha = _mm_set1_epi16(alpha);

	__m128i shiftr = _mm_set1_epi32(MLIB_ROUND_Q15);

	*((mlib_s16 *)(sp1 - 2)) = last;
	sp2 = sp1 - 2;

	if (!(((mlib_addr)dp) & 15)) {

	if (!(((mlib_addr)sp1 | (mlib_addr)sp2) & 15)) {
		for (nn = 0; nn <= (2 * n - 16); nn += 16) {
			sd1 = _mm_load_si128((__m128i *)sp1);
			sd2 = _mm_load_si128((__m128i *)sp2);
			ALPHASUB_S16(dd, sd1, sd2);
			_mm_store_si128((__m128i *)dp, dd);
			last = *(((mlib_s16 *) sp1) + 7);
			dp += 16;
			sp1 += 16;
			sp2 += 16;
		}
	} else {
		for (nn = 0; nn <= (2 * n - 16); nn += 16) {
			sd1 = _mm_loadu_si128((void *)sp1);
			sd2 = _mm_loadu_si128((void *)sp2);

			ALPHASUB_S16(dd, sd1, sd2);
			_mm_store_si128((__m128i *)dp, dd);
			last = *(((mlib_s16 *) sp1) + 7);
			dp += 16;
			sp1 += 16;
			sp2 += 16;
		}
	}

	} else { /* if (!(((mlib_addr)dp) & 15)) */

	if (!(((mlib_addr)sp1 | (mlib_addr)sp2) & 15)) {
		for (nn = 0; nn <= (2 * n - 16); nn += 16) {
			sd1 = _mm_load_si128((__m128i *)sp1);
			sd2 = _mm_load_si128((__m128i *)sp2);
			ALPHASUB_S16(dd, sd1, sd2);
			_mm_storeu_si128((void *)dp, dd);
			last = *(((mlib_s16 *) sp1) + 7);
			dp += 16;
			sp1 += 16;
			sp2 += 16;
		}
	} else {
		for (nn = 0; nn <= (2 * n - 16); nn += 16) {
			sd1 = _mm_loadu_si128((void *)sp1);
			sd2 = _mm_loadu_si128((void *)sp2);

			ALPHASUB_S16(dd, sd1, sd2);
			_mm_storeu_si128((void *)dp, dd);
			last = *(((mlib_s16 *) sp1) + 7);
			dp += 16;
			sp1 += 16;
			sp2 += 16;
		}
	}
	} /* if (!(((mlib_addr)dp) & 15)) */

	for (; nn < 2 * n; nn = nn + 2) {
		mlib_s16 tmp = *((mlib_s16 *)sp1);
		mlib_s32 dtmp = ((((((mlib_s32)tmp) << EX) +
			(mlib_s32)alpha * last)) + MLIB_ROUND_Q15) >> EX;
		*((mlib_s16 *)dp) = MLIB_SATURATE_S16(dtmp);
		last = tmp;
		sp1 = sp1 + 2;
		dp = dp + 2;
	}

	fist->last0 = (mlib_f32)last;

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_SignalEmphasize_S16S_S16S_Sat(
	mlib_s16 *dst,
	const mlib_s16 *src,
	void *filter,
	mlib_s32 n)
{
	mlib_emphasize_struct *fist = filter;

	__m128i sd1, sd2, dd, talpha, shiftr;
	mlib_u8 *dp, *dl, *dend;
	mlib_u8 *sp1, *sp2, *sl1;
	mlib_s32 nn, emask;
	mlib_s16 last0, last1;
	mlib_s16 alpha;

/* check for obvious errors */

	if ((fist == NULL) || (n <= 0) || (src == 0) || (dst == 0) ||
		(fist->type != MLIB_EMPH))
		return (MLIB_FAILURE);

	dp = dl = (mlib_u8 *)dst;
	sp1 = sl1 = (mlib_u8 *)src;
	last0 = (mlib_s16)(fist->last0);
	last1 = (mlib_s16)(fist->last1);
	alpha = (mlib_s16)((-(fist->alpha)) * 32768.); /* alpha << EX */
	talpha = _mm_set1_epi16(alpha);

	shiftr = _mm_set1_epi32(MLIB_ROUND_Q15);

	*((mlib_s16 *)(sp1 - 2)) = last1;
	*((mlib_s16 *)(sp1 - 4)) = last0;
	sp2 = sp1 - 4;

	if (!(((mlib_addr)dp) & 15)) {
	if (!(((mlib_addr)sp1 | (mlib_addr)sp2) & 15)) {
		for (nn = 0; nn <= (4 * n - 16); nn += 16) {
			sd1 = _mm_load_si128((__m128i *)sp1);
			sd2 = _mm_load_si128((__m128i *)sp2);
			ALPHASUB_S16(dd, sd1, sd2);
			_mm_store_si128((__m128i *)dp, dd);

			last0 = *(((mlib_s16 *) sp1) + 6);
			last1 = *(((mlib_s16 *) sp1) + 7);

			dp += 16;
			sp1 += 16;
			sp2 += 16;
		}
	} else if (!((mlib_addr)sp1 & 15)) {
		for (nn = 0; nn <= (4 * n - 16); nn += 16) {
			sd1 = _mm_load_si128((__m128i *)sp1);
			sd2 = _mm_loadu_si128((void *)sp2);

			ALPHASUB_S16(dd, sd1, sd2);
			_mm_store_si128((__m128i *)dp, dd);

			last0 = *(((mlib_s16 *) sp1) + 6);
			last1 = *(((mlib_s16 *) sp1) + 7);

			dp += 16;
			sp1 += 16;
			sp2 += 16;
		}
	} else if (!((mlib_addr)sp2 & 15)) {
		for (nn = 0; nn <= (4 * n - 16); nn += 16) {
			sd1 = _mm_loadu_si128((void *)sp1);
			sd2 = _mm_load_si128((__m128i *)sp2);

			ALPHASUB_S16(dd, sd1, sd2);
			_mm_store_si128((__m128i *)dp, dd);

			last0 = *(((mlib_s16 *) sp1) + 6);
			last1 = *(((mlib_s16 *) sp1) + 7);

			dp += 16;
			sp1 += 16;
			sp2 += 16;
		}
	} else {
		for (nn = 0; nn <= (4 * n - 16); nn += 16) {
			sd1 = _mm_loadu_si128((void *)sp1);
			sd2 = _mm_loadu_si128((void *)sp2);

			ALPHASUB_S16(dd, sd1, sd2);
			_mm_store_si128((__m128i *)dp, dd);

			last0 = *(((mlib_s16 *) sp1) + 6);
			last1 = *(((mlib_s16 *) sp1) + 7);

			dp += 16;
			sp1 += 16;
			sp2 += 16;
		}
	}

	} else { /* if (!(((mlib_addr)dp) & 15)) */

	if (!(((mlib_addr)sp1 | (mlib_addr)sp2) & 15)) {
		for (nn = 0; nn <= (4 * n - 16); nn += 16) {
			sd1 = _mm_load_si128((__m128i *)sp1);
			sd2 = _mm_load_si128((__m128i *)sp2);
			ALPHASUB_S16(dd, sd1, sd2);
			_mm_storeu_si128((void *)dp, dd);

			last0 = *(((mlib_s16 *) sp1) + 6);
			last1 = *(((mlib_s16 *) sp1) + 7);

			dp += 16;
			sp1 += 16;
			sp2 += 16;
		}
	} else if (!((mlib_addr)sp1 & 15)) {
		for (nn = 0; nn <= (4 * n - 16); nn += 16) {
			sd1 = _mm_load_si128((__m128i *)sp1);
			sd2 = _mm_loadu_si128((void *)sp2);

			ALPHASUB_S16(dd, sd1, sd2);
			_mm_storeu_si128((void *)dp, dd);

			last0 = *(((mlib_s16 *) sp1) + 6);
			last1 = *(((mlib_s16 *) sp1) + 7);

			dp += 16;
			sp1 += 16;
			sp2 += 16;
		}
	} else if (!((mlib_addr)sp2 & 15)) {
		for (nn = 0; nn <= (4 * n - 16); nn += 16) {
			sd1 = _mm_loadu_si128((void *)sp1);
			sd2 = _mm_load_si128((__m128i *)sp2);

			ALPHASUB_S16(dd, sd1, sd2);
			_mm_storeu_si128((void *)dp, dd);

			last0 = *(((mlib_s16 *) sp1) + 6);
			last1 = *(((mlib_s16 *) sp1) + 7);

			dp += 16;
			sp1 += 16;
			sp2 += 16;
		}
	} else {
		for (nn = 0; nn <= (4 * n - 16); nn += 16) {
			sd1 = _mm_loadu_si128((void *)sp1);
			sd2 = _mm_loadu_si128((void *)sp2);

			ALPHASUB_S16(dd, sd1, sd2);
			_mm_storeu_si128((void *)dp, dd);

			last0 = *(((mlib_s16 *) sp1) + 6);
			last1 = *(((mlib_s16 *) sp1) + 7);

			dp += 16;
			sp1 += 16;
			sp2 += 16;
		}
	}
	} /* if (!(((mlib_addr)dp) & 15)) */

	for (; nn <= (4 * n - 4); nn = nn + 4) {
		mlib_s16 tmp0 = *((mlib_s16 *)sp1);
		mlib_s16 tmp1 = *((mlib_s16 *)sp1 + 1);

		mlib_s32 dtmp0 = ((((((mlib_s32)tmp0) << EX) +
			(mlib_s32)alpha * last0)) + MLIB_ROUND_Q15) >> EX;
		*((mlib_s16 *)dp) = MLIB_SATURATE_S16(dtmp0);

		mlib_s32 dtmp1 = ((((((mlib_s32)tmp1) << EX) +
			(mlib_s32)alpha * last1)) + MLIB_ROUND_Q15) >> EX;
		*((mlib_s16 *)dp + 1) =	MLIB_SATURATE_S16(dtmp1);

		last0 = tmp0;
		last1 = tmp1;
		sp1 = sp1 + 4;
		dp = dp + 4;
	}

	if ((nn < 4 * n) && (nn > (4 * n - 4))) {
		mlib_s16 tmp0 = *((mlib_s16 *)sp1);
		mlib_s32 dtmp0 = ((((((mlib_s32)tmp0) << EX) +
			(mlib_s32)alpha * last0)) + MLIB_ROUND_Q15) >> EX;
		*((mlib_s16 *)dp) = MLIB_SATURATE_S16(dtmp0);
		last0 = last1;
		last1 = tmp0;
	}

	fist->last0 = (mlib_f32)last0;
	fist->last1 = (mlib_f32)last1;

	return (MLIB_SUCCESS);
}

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
