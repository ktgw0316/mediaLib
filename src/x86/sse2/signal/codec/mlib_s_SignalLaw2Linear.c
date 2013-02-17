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

#pragma ident	"@(#)mlib_s_SignalLaw2Linear.c	9.5	07/11/05 SMI"

/*
 * FUNCTIONS
 *      mlib_SignalALaw2Linear - Functions in this section perform A-law
 *      decompression in compliance with the ITU (former CCITT) specification.
 *
 *      mlib_SignaluLaw2Linear - Functions in this section perform u-law
 *      decompression in compliance with the ITU (former CCITT) specification.
 *
 * SYNOPSIS
 *      mlib_status mlib_SignalALaw2Linear(mlib_s16      *pcm,
 *                                         const mlib_u8 *acode,
 *                                         mlib_s32      n)
 *
 *      mlib_status mlib_SignaluLaw2Linear(mlib_s16      *pcm,
 *                                         const mlib_u8 *ucode,
 *                                         mlib_s32      n)
 *
 * ARGUMENTS
 *      pcm      Linear PCM sample array
 *      acode    A-law code array
 *      ucode    u-law code array
 *      n        Number of samples in the input array
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

#pragma weak mlib_SignalALaw2Linear = __mlib_SignalALaw2Linear
#pragma weak mlib_SignaluLaw2Linear = __mlib_SignaluLaw2Linear

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_SignalALaw2Linear) mlib_SignalALaw2Linear
    __attribute__((weak, alias("__mlib_SignalALaw2Linear")));
__typeof__(__mlib_SignaluLaw2Linear) mlib_SignaluLaw2Linear
    __attribute__((weak, alias("__mlib_SignaluLaw2Linear")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	LINEAR_TABLE(i)	*(mlib_s16 *)((mlib_u8 *)linearTable + (i))

/* *********************************************************** */

#define	LAW2LINEAR_MAIN_LOOP(sel, i0, i1, i2, i3, i4, i5, i6, i7)         \
	for (; i < n - 15; i += 8) {                                      \
	    l10 = *(mlib_u32 *)(sp + i + (8 - sel));                      \
	    l11 = *(mlib_u32 *)(sp + i + (12 - sel));                     \
	    accum = _mm_setr_epi16(                                       \
			    LINEAR_TABLE(i0),                             \
			    LINEAR_TABLE(i1),                             \
			    LINEAR_TABLE(i2),                             \
			    LINEAR_TABLE(i3),                             \
			    LINEAR_TABLE(i4),                             \
			    LINEAR_TABLE(i5),                             \
			    LINEAR_TABLE(i6),                             \
			    LINEAR_TABLE(i7));                            \
	    _mm_store_si128((__m128i *)(dp + i), accum);                  \
	    l00 = l10;                                                    \
	    l01 = l11;                                                    \
	}

/* *********************************************************** */

static void
mlib_s_SignalLaw2Linear(
    mlib_s16 *pcm,
    const mlib_u8 *code,
    mlib_s32 n,
    const mlib_u16 *linearTable)
{
	mlib_u8 *sp = (mlib_u8 *)code;
	mlib_s16 *dp = (mlib_s16 *)pcm;
	mlib_s32 i, sel;
	mlib_u32 l00, l10;
	mlib_u32 l01, l11;
	__m128i accum;

	for (i = 0; (i < n) && ((mlib_addr)(dp + i) & 15); i++) {
		dp[i] = linearTable[sp[i]];
	}

	if (i < n - 7) {
		sel = (mlib_addr)(sp + i) & 7;
		l00 = *(mlib_u32 *)(sp + i - sel);
		l01 = *(mlib_u32 *)(sp + i - sel + 4);

		if (sel == 0) {
#ifdef	__SUNPRO_C
#pragma pipeloop(0)
#endif
			LAW2LINEAR_MAIN_LOOP(0,
			    (l00 << 1) & 0x1FE,
			    (l00 >> 7) & 0x1FE,
			    (l00 >> 15) & 0x1FE,
			    (l00 >> 23) & 0x1FE,
			    (l01 << 1) & 0x1FE,
			    (l01 >> 7) & 0x1FE,
			    (l01 >> 15) & 0x1FE, (l01 >> 23) & 0x1FE)
		} else if (sel == 1) {
#ifdef	__SUNPRO_C
#pragma pipeloop(0)
#endif
			LAW2LINEAR_MAIN_LOOP(1,
			    (l00 >> 7) & 0x1FE,
			    (l00 >> 15) & 0x1FE,
			    (l00 >> 23) & 0x1FE,
			    (l01 << 1) & 0x1FE,
			    (l01 >> 7) & 0x1FE,
			    (l01 >> 15) & 0x1FE,
			    (l01 >> 23) & 0x1FE, (l10 << 1) & 0x1FE)
		} else if (sel == 2) {
#ifdef	__SUNPRO_C
#pragma pipeloop(0)
#endif
			LAW2LINEAR_MAIN_LOOP(2,
			    (l00 >> 15) & 0x1FE,
			    (l00 >> 23) & 0x1FE,
			    (l01 << 1) & 0x1FE,
			    (l01 >> 7) & 0x1FE,
			    (l01 >> 15) & 0x1FE,
			    (l01 >> 23) & 0x1FE,
			    (l10 << 1) & 0x1FE,
			    (l10 >> 7) & 0x1FE)
		} else if (sel == 3) {
#ifdef	__SUNPRO_C
#pragma pipeloop(0)
#endif
			LAW2LINEAR_MAIN_LOOP(3,
			    (l00 >> 23) & 0x1FE,
			    (l01 << 1) & 0x1FE,
			    (l01 >> 7) & 0x1FE,
			    (l01 >> 15) & 0x1FE,
			    (l01 >> 23) & 0x1FE,
			    (l10 << 1) & 0x1FE,
			    (l10 >> 7) & 0x1FE,
			    (l10 >> 15) & 0x1FE)
		} else if (sel == 4) {
#ifdef	__SUNPRO_C
#pragma pipeloop(0)
#endif
			LAW2LINEAR_MAIN_LOOP(4,
			    (l01 << 1) & 0x1FE,
			    (l01 >> 7) & 0x1FE,
			    (l01 >> 15) & 0x1FE,
			    (l01 >> 23) & 0x1FE,
			    (l10 << 1) & 0x1FE,
			    (l10 >> 7) & 0x1FE,
			    (l10 >> 15) & 0x1FE,
			    (l10 >> 23) & 0x1FE)
		} else if (sel == 5) {
#ifdef	__SUNPRO_C
#pragma pipeloop(0)
#endif
			LAW2LINEAR_MAIN_LOOP(5,
			    (l01 >> 7) & 0x1FE,
			    (l01 >> 15) & 0x1FE,
			    (l01 >> 23) & 0x1FE,
			    (l10 << 1) & 0x1FE,
			    (l10 >> 7) & 0x1FE,
			    (l10 >> 15) & 0x1FE,
			    (l10 >> 23) & 0x1FE,
			    (l11 << 1) & 0x1FE)
		} else if (sel == 6) {
#ifdef	__SUNPRO_C
#pragma pipeloop(0)
#endif
			LAW2LINEAR_MAIN_LOOP(6,
			    (l01 >> 15) & 0x1FE,
			    (l01 >> 23) & 0x1FE,
			    (l10 << 1) & 0x1FE,
			    (l10 >> 7) & 0x1FE,
			    (l10 >> 15) & 0x1FE,
			    (l10 >> 23) & 0x1FE,
			    (l11 << 1) & 0x1FE,
			    (l11 >> 7) & 0x1FE)
		} else {
#ifdef	__SUNPRO_C
#pragma pipeloop(0)
#endif
			LAW2LINEAR_MAIN_LOOP(7,
			    (l01 >> 23) & 0x1FE,
			    (l10 << 1) & 0x1FE,
			    (l10 >> 7) & 0x1FE,
			    (l10 >> 15) & 0x1FE,
			    (l10 >> 23) & 0x1FE,
			    (l11 << 1) & 0x1FE,
			    (l11 >> 7) & 0x1FE,
			    (l11 >> 15) & 0x1FE)
		}
	}

	for (; i < n; i++) {
		dp[i] = linearTable[sp[i]];
	}
}

/* *********************************************************** */

mlib_status
__mlib_SignalALaw2Linear(
    mlib_s16 *pcm,
    const mlib_u8 *acode,
    mlib_s32 n)
{
	if ((pcm == NULL) | (acode == NULL))
		return (MLIB_FAILURE);

	if (n <= 0)
		return (MLIB_OUTOFRANGE);

	mlib_s_SignalLaw2Linear(pcm, acode, n, mlib_A2l);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_SignaluLaw2Linear(
    mlib_s16 *pcm,
    const mlib_u8 *ucode,
    mlib_s32 n)
{
	if ((pcm == NULL) | (ucode == NULL))
		return (MLIB_FAILURE);

	if (n <= 0)
		return (MLIB_OUTOFRANGE);

	mlib_s_SignalLaw2Linear(pcm, ucode, n, mlib_u2l);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
