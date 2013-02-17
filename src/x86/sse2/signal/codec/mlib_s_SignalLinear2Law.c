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

#pragma ident	"@(#)mlib_s_SignalLinear2Law.c	9.4	07/11/05 SMI"

/*
 * FUNCTIONS
 *
 *      mlib_SignalLinear2ALaw - Functions in this section perform A-law
 *      compression in compliance with the ITU (former CCITT) specification.
 *
 *      mlib_SignalLinear2uLaw - Functions in this section perform u-law
 *      compression in compliance with the ITU (former CCITT) specification.
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_SignalLinear2ALaw(mlib_u8        *acode,
 *                                         const mlib_s16 *pcm,
 *                                         mlib_s32       n);
 *      mlib_status mlib_SignalLinear2uLaw(mlib_u8        *ucode,
 *                                         const mlib_s16 *pcm,
 *                                         mlib_s32       n);
 *
 * ARGUMENTS
 *
 *      pcm      Linear PCM sample array
 *
 *      acode    A-law code array
 *
 *      ucode    u-law code array
 *
 *      n        Number of samples in the input array
 *
 */

#include <mlib_signal.h>
#include <mlib_SignalCodec.h>
#ifdef	__SUNPRO_C
#include <sunmedia_intrin.h>
#else
#include <emmintrin.h>
#endif

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_SignalLinear2ALaw = __mlib_SignalLinear2ALaw
#pragma weak mlib_SignalLinear2uLaw = __mlib_SignalLinear2uLaw

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_SignalLinear2ALaw) mlib_SignalLinear2ALaw
    __attribute__((weak, alias("__mlib_SignalLinear2ALaw")));
__typeof__(__mlib_SignalLinear2uLaw) mlib_SignalLinear2uLaw
    __attribute__((weak, alias("__mlib_SignalLinear2uLaw")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	CALC_u(dx)                                                       \
	((((dx) + 4) - ((dx) >> 15)) >> 3) + ((dx) >> 15)                \
/* *********************************************************** */

#define	LINEAR2LAW_MAIN_LOOP(sel, i0, i1, i2, i3, i4, i5, i6, i7,        \
		i8, i9, i10, i11, i12, i13, i14, i15)                    \
	for (; i < n - 23; i += 16) {                                    \
	    l10 = *(mlib_u32 *)(sp + i + (4 - sel));                      \
	    l11 = *(mlib_u32 *)(sp + i + (6 - sel));                      \
	    l20 = *(mlib_u32 *)(sp + i + (8 - sel));                      \
	    l21 = *(mlib_u32 *)(sp + i + (10 - sel));                      \
	    l30 = *(mlib_u32 *)(sp + i + (12 - sel));                     \
	    l31 = *(mlib_u32 *)(sp + i + (14 - sel));                     \
	    l40 = *(mlib_u32 *)(sp + i + (16 - sel));                     \
	    l41 = *(mlib_u32 *)(sp + i + (18 - sel));                     \
	    accum = _mm_setr_epi8(                                       \
			    mlib_l2A[i0],                                \
			    mlib_l2A[i1],                                \
			    mlib_l2A[i2],                                \
			    mlib_l2A[i3],                                \
			    mlib_l2A[i4],                                \
			    mlib_l2A[i5],                                \
			    mlib_l2A[i6],                                \
			    mlib_l2A[i7],                                \
			    mlib_l2A[i8],                                \
			    mlib_l2A[i9],                                \
			    mlib_l2A[i10],                               \
			    mlib_l2A[i11],                               \
			    mlib_l2A[i12],                               \
			    mlib_l2A[i13],                               \
			    mlib_l2A[i14],                               \
			    mlib_l2A[i15]);                              \
	    _mm_store_si128((__m128i *)(dp + i), accum);                 \
	    l00 = l40;                                                     \
	    l01 = l41;                                                     \
	}

#define	LINEAR2ULAW_MAIN_LOOP(sel, i0, i1, i2, i3, i4, i5, i6, i7,       \
		i8, i9, i10, i11, i12, i13, i14, i15)                    \
	for (; i < n - 23; i += 16) {                                    \
	    l10 = *(mlib_u32 *)(sp + i + (4 - sel));                      \
	    l11 = *(mlib_u32 *)(sp + i + (6 - sel));                      \
	    l20 = *(mlib_u32 *)(sp + i + (8 - sel));                      \
	    l21 = *(mlib_u32 *)(sp + i + (10 - sel));                      \
	    l30 = *(mlib_u32 *)(sp + i + (12 - sel));                     \
	    l31 = *(mlib_u32 *)(sp + i + (14 - sel));                     \
	    l40 = *(mlib_u32 *)(sp + i + (16 - sel));                     \
	    l41 = *(mlib_u32 *)(sp + i + (18 - sel));                     \
	    accum = _mm_setr_epi8(                                       \
			    mlib_l2u[i0],                                \
			    mlib_l2u[i1],                                \
			    mlib_l2u[i2],                                \
			    mlib_l2u[i3],                                \
			    mlib_l2u[i4],                                \
			    mlib_l2u[i5],                                \
			    mlib_l2u[i6],                                \
			    mlib_l2u[i7],                                \
			    mlib_l2u[i8],                                \
			    mlib_l2u[i9],                                \
			    mlib_l2u[i10],                               \
			    mlib_l2u[i11],                               \
			    mlib_l2u[i12],                               \
			    mlib_l2u[i13],                               \
			    mlib_l2u[i14],                               \
			    mlib_l2u[i15]);                              \
	    _mm_store_si128((__m128i *)(dp + i), accum);                 \
	    l00 = l40;                                                     \
	    l01 = l41;                                                     \
	}


/* *********************************************************** */

mlib_status
__mlib_SignalLinear2ALaw(
    mlib_u8 *acode,
    const mlib_s16 *pcm,
    mlib_s32 n)
{
	mlib_u8 *dp = acode;
	mlib_u16 *sp = (mlib_u16 *)pcm;
	mlib_s32 i, ind;
	__m128i accum;
	mlib_u32 l00, l10, l20, l30, l40;
	mlib_u32 l01, l11, l21, l31, l41;
	mlib_s32 sel;

	if ((sp == NULL) | (dp == NULL))
		return (MLIB_FAILURE);

	if (n <= 0)
		return (MLIB_OUTOFRANGE);

	for (i = 0; (i < n) && ((mlib_addr)(dp + i) & 15); i++) {
		ind = sp[i];
		ind >>= 4;

		dp[i] = mlib_l2A[ind];
	}

	if (i < n - 23) {
		sel = ((mlib_addr)(sp + i) & 7) >> 1;
		l00 = *(mlib_u32 *)(sp + i - sel);
		l01 = *(mlib_u32 *)(sp + i - sel + 2);
		if (sel == 0) {
#ifdef	__SUNPRO_C
#pragma pipeloop(0)
#endif
			LINEAR2LAW_MAIN_LOOP(0,
			    (l00 >> 4) & 0xFFF,
			    (l00 >> 20) & 0xFFF,
			    (l01 >> 4) & 0xFFF,
			    (l01 >> 20) & 0xFFF,
			    (l10 >> 4)& 0xFFF,
			    (l10 >> 20) & 0xFFF,
			    (l11 >> 4) & 0xFFF,
			    (l11 >> 20) & 0xFFF,
			    (l20 >> 4) & 0xFFF,
			    (l20 >> 20) & 0xFFF,
			    (l21 >> 4) & 0xFFF,
			    (l21 >> 20) & 0xFFF,
			    (l30 >> 4)& 0xFFF,
			    (l30 >> 20) & 0xFFF,
			    (l31 >> 4) & 0xFFF,
			    (l31 >> 20) & 0xFFF)
		} else if (sel == 1) {
#ifdef	__SUNPRO_C
#pragma pipeloop(0)
#endif
			LINEAR2LAW_MAIN_LOOP(1,
			    (l00 >> 20) & 0xFFF,
			    (l01 >> 4) & 0xFFF,
			    (l01 >> 20) & 0xFFF,
			    (l10 >> 4)& 0xFFF,
			    (l10 >> 20) & 0xFFF,
			    (l11 >> 4) & 0xFFF,
			    (l11 >> 20) & 0xFFF,
			    (l20 >> 4) & 0xFFF,
			    (l20 >> 20) & 0xFFF,
			    (l21 >> 4) & 0xFFF,
			    (l21 >> 20) & 0xFFF,
			    (l30 >> 4)& 0xFFF,
			    (l30 >> 20) & 0xFFF,
			    (l31 >> 4) & 0xFFF,
			    (l31 >> 20) & 0xFFF,
			    (l40 >> 4) & 0xFFF)
		} else if (sel == 2) {
#ifdef	__SUNPRO_C
#pragma pipeloop(0)
#endif
			LINEAR2LAW_MAIN_LOOP(2,
			    (l01 >> 4) & 0xFFF,
			    (l01 >> 20) & 0xFFF,
			    (l10 >> 4)& 0xFFF,
			    (l10 >> 20) & 0xFFF,
			    (l11 >> 4) & 0xFFF,
			    (l11 >> 20) & 0xFFF,
			    (l20 >> 4) & 0xFFF,
			    (l20 >> 20) & 0xFFF,
			    (l21 >> 4) & 0xFFF,
			    (l21 >> 20) & 0xFFF,
			    (l30 >> 4)& 0xFFF,
			    (l30 >> 20) & 0xFFF,
			    (l31 >> 4) & 0xFFF,
			    (l31 >> 20) & 0xFFF,
			    (l40 >> 4) & 0xFFF,
			    (l40 >> 20) & 0xFFF)
		} else {
#ifdef	__SUNPRO_C
#pragma pipeloop(0)
#endif
			LINEAR2LAW_MAIN_LOOP(3,
			    (l01 >> 20) & 0xFFF,
			    (l10 >> 4)& 0xFFF,
			    (l10 >> 20) & 0xFFF,
			    (l11 >> 4) & 0xFFF,
			    (l11 >> 20) & 0xFFF,
			    (l20 >> 4) & 0xFFF,
			    (l20 >> 20) & 0xFFF,
			    (l21 >> 4) & 0xFFF,
			    (l21 >> 20) & 0xFFF,
			    (l30 >> 4)& 0xFFF,
			    (l30 >> 20) & 0xFFF,
			    (l31 >> 4) & 0xFFF,
			    (l31 >> 20) & 0xFFF,
			    (l40 >> 4) & 0xFFF,
			    (l40 >> 20) & 0xFFF,
			    (l41 >> 4) & 0xFFF)
		}
	}

	for (; i < n; i++) {
		ind = sp[i];
		ind >>= 4;

		dp[i] = mlib_l2A[ind];
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_SignalLinear2uLaw(
    mlib_u8 *acode,
    const mlib_s16 *pcm,
    mlib_s32 n)
{
	mlib_u8 *dp = acode;
	mlib_u16 *sp = (mlib_u16 *)pcm;
	mlib_s32 i, ind, zn;
	__m128i accum;
	mlib_u32 l00, l10, l20, l30, l40;
	mlib_u32 l01, l11, l21, l31, l41;
	mlib_s32 sel;
	mlib_u8 d0, d1, d2, d3, d4, d5, d6, d7;

	if ((sp == NULL) | (dp == NULL))
		return (MLIB_FAILURE);

	if (n <= 0)
		return (MLIB_OUTOFRANGE);

	for (i = 0; (i < n) && ((mlib_addr)(dp + i) & 15); i++) {
		ind = sp[i] + 4;
		zn = sp[i] >> 15;
		ind -= zn;
		ind >>= 3;
		ind += zn;

		dp[i] = mlib_l2u[ind];
	}

	if (i < n - 23) {
		sel = ((mlib_addr)(sp + i) & 7) >> 1;
		l00 = *(mlib_u32 *)(sp + i - sel);
		l01 = *(mlib_u32 *)(sp + i - sel + 2);
		if (sel == 0) {
#ifdef	__SUNPRO_C
#pragma pipeloop(0)
#endif
			LINEAR2ULAW_MAIN_LOOP(0,
			    CALC_u((l00) & 0xFFFF),
			    CALC_u((l00 >> 16) & 0xFFFF),
			    CALC_u((l01) & 0xFFFF),
			    CALC_u((l01 >> 16) & 0xFFFF),
			    CALC_u((l10) & 0xFFFF),
			    CALC_u((l10 >> 16) & 0xFFFF),
			    CALC_u((l11) & 0xFFFF),
			    CALC_u((l11 >> 16) & 0xFFFF),
			    CALC_u((l20) & 0xFFFF),
			    CALC_u((l20 >> 16) & 0xFFFF),
			    CALC_u((l21) & 0xFFFF),
			    CALC_u((l21 >> 16) & 0xFFFF),
			    CALC_u((l30) & 0xFFFF),
			    CALC_u((l30 >> 16) & 0xFFFF),
			    CALC_u((l31) & 0xFFFF),
			    CALC_u((l31 >> 16) & 0xFFFF))
		} else if (sel == 1) {
#ifdef	__SUNPRO_C
#pragma pipeloop(0)
#endif
			LINEAR2ULAW_MAIN_LOOP(1,
			    CALC_u((l00 >> 16) & 0xFFFF),
			    CALC_u((l01) & 0xFFFF),
			    CALC_u((l01 >> 16) & 0xFFFF),
			    CALC_u((l10) & 0xFFFF),
			    CALC_u((l10 >> 16) & 0xFFFF),
			    CALC_u((l11) & 0xFFFF),
			    CALC_u((l11 >> 16) & 0xFFFF),
			    CALC_u((l20) & 0xFFFF),
			    CALC_u((l20 >> 16) & 0xFFFF),
			    CALC_u((l21) & 0xFFFF),
			    CALC_u((l21 >> 16) & 0xFFFF),
			    CALC_u((l30) & 0xFFFF),
			    CALC_u((l30 >> 16) & 0xFFFF),
			    CALC_u((l31) & 0xFFFF),
			    CALC_u((l31 >> 16) & 0xFFFF),
			    CALC_u((l40) & 0xFFFF))
		} else if (sel == 2) {
#ifdef	__SUNPRO_C
#pragma pipeloop(0)
#endif
			LINEAR2ULAW_MAIN_LOOP(2,
			    CALC_u((l01) & 0xFFFF),
			    CALC_u((l01 >> 16) & 0xFFFF),
			    CALC_u((l10) & 0xFFFF),
			    CALC_u((l10 >> 16) & 0xFFFF),
			    CALC_u((l11) & 0xFFFF),
			    CALC_u((l11 >> 16) & 0xFFFF),
			    CALC_u((l20) & 0xFFFF),
			    CALC_u((l20 >> 16) & 0xFFFF),
			    CALC_u((l21) & 0xFFFF),
			    CALC_u((l21 >> 16) & 0xFFFF),
			    CALC_u((l30) & 0xFFFF),
			    CALC_u((l30 >> 16) & 0xFFFF),
			    CALC_u((l31) & 0xFFFF),
			    CALC_u((l31 >> 16) & 0xFFFF),
			    CALC_u((l40) & 0xFFFF),
			    CALC_u((l40 >> 16) & 0xFFFF))
		} else {
#ifdef	__SUNPRO_C
#pragma pipeloop(0)
#endif
			LINEAR2ULAW_MAIN_LOOP(3,
			    CALC_u((l01 >> 16) & 0xFFFF),
			    CALC_u((l10) & 0xFFFF),
			    CALC_u((l10 >> 16) & 0xFFFF),
			    CALC_u((l11) & 0xFFFF),
			    CALC_u((l11 >> 16) & 0xFFFF),
			    CALC_u((l20) & 0xFFFF),
			    CALC_u((l20 >> 16) & 0xFFFF),
			    CALC_u((l21) & 0xFFFF),
			    CALC_u((l21 >> 16) & 0xFFFF),
			    CALC_u((l30) & 0xFFFF),
			    CALC_u((l30 >> 16) & 0xFFFF),
			    CALC_u((l31) & 0xFFFF),
			    CALC_u((l31 >> 16) & 0xFFFF),
			    CALC_u((l40) & 0xFFFF),
			    CALC_u((l40 >> 16) & 0xFFFF),
			    CALC_u((l41) & 0xFFFF))
		}
	}

	for (; i < n; i++) {
		ind = sp[i] + 4;
		zn = sp[i] >> 15;
		ind -= zn;
		ind >>= 3;
		ind += zn;

		dp[i] = mlib_l2u[ind];
	}

	return (MLIB_SUCCESS);
}
