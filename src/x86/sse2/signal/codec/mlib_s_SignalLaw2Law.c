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

#pragma ident	"@(#)mlib_s_SignalLaw2Law.c	9.5	07/11/05 SMI"

/*
 * FUNCTIONS
 *
 *      mlib_SignalALaw2uLaw - Functions in this section perform m-law
 *      and A-law compression and decompression in compliance with
 *      the ITU (former CCITT) specification.
 *
 *      mlib_SignaluLaw2ALaw - Functions in this section perform
 *      u-law and A-law compression and decompression in compliance
 *      with the ITU (former CCITT) specification.
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_SignalALaw2uLaw(mlib_u8       *ucode,
 *                                       const mlib_u8 *acode,
 *                                       mlib_s32      n);
 *      mlib_status mlib_SignaluLaw2ALaw(mlib_u8       *acode,
 *                                       const mlib_u8 *ucode,
 *                                       mlib_s32      n);
 * ARGUMENTS
 *
 *      ucode    m-law code array
 *      acode    A-law code array
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

#pragma weak mlib_SignalALaw2uLaw = __mlib_SignalALaw2uLaw
#pragma weak mlib_SignaluLaw2ALaw = __mlib_SignaluLaw2ALaw

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_SignalALaw2uLaw) mlib_SignalALaw2uLaw
    __attribute__((weak, alias("__mlib_SignalALaw2uLaw")));
__typeof__(__mlib_SignaluLaw2ALaw) mlib_SignaluLaw2ALaw
    __attribute__((weak, alias("__mlib_SignaluLaw2ALaw")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/*
 * SSE2 version is substituted by C version
 * since the implementation of _mm_setr_epi8() in Sun compiler (Intel compiler
 * as well) is not the best: data is loaded from memory to xmm registers
 * via GPRs while moving from GPRs to xmm registers is slow
 */

#if 0

#define	LAW2LAW_MAIN_LOOP(sel, i0, i1, i2, i3, i4, i5, i6, i7,           \
		i8, i9, i10, i11, i12, i13, i14, i15)                    \
	for (; i < n - 31; i += 16) {                                    \
	    l20 = *(mlib_u32 *)(src + i + (16 - sel));                   \
	    l21 = *(mlib_u32 *)(src + i + (20 - sel));                   \
	    l30 = *(mlib_u32 *)(src + i + (24 - sel));                   \
	    l31 = *(mlib_u32 *)(src + i + (28 - sel));                   \
	    accum = _mm_setr_epi8(                                       \
			    law_table[i0],                               \
			    law_table[i1],                               \
			    law_table[i2],                               \
			    law_table[i3],                               \
			    law_table[i4],                               \
			    law_table[i5],                               \
			    law_table[i6],                               \
			    law_table[i7],                               \
			    law_table[i8],                               \
			    law_table[i9],                               \
			    law_table[i10],                              \
			    law_table[i11],                              \
			    law_table[i12],                              \
			    law_table[i13],                              \
			    law_table[i14],                              \
			    law_table[i15]);                             \
	    _mm_store_si128((__m128i *)(dst + i), accum);                \
	    l00 = l20;                                                   \
	    l01 = l21;                                                   \
	    l10 = l30;                                                   \
	    l11 = l31;                                                   \
	}

#else

#define	LAW2LAW_MAIN_LOOP(sel, i0, i1, i2, i3, i4, i5, i6, i7,           \
		i8, i9, i10, i11, i12, i13, i14, i15)                    \
	for (; i < n - 31; i += 16) {                                    \
	    l20 = *(mlib_u32 *)(src + i + (16 - sel));                   \
	    l21 = *(mlib_u32 *)(src + i + (20 - sel));                   \
	    l30 = *(mlib_u32 *)(src + i + (24 - sel));                   \
	    l31 = *(mlib_u32 *)(src + i + (28 - sel));                   \
	    dst[i] = law_table[i0];                                      \
	    dst[i + 1] = law_table[i1];                                  \
	    dst[i + 2] = law_table[i2];                                  \
	    dst[i + 3] = law_table[i3];                                  \
	    dst[i + 4] = law_table[i4];                                  \
	    dst[i + 5] = law_table[i5];                                  \
	    dst[i + 6] = law_table[i6];                                  \
	    dst[i + 7] = law_table[i7];                                  \
	    dst[i + 8] = law_table[i8];                                  \
	    dst[i + 9] = law_table[i9];                                  \
	    dst[i + 10] = law_table[i10];                                \
	    dst[i + 11] = law_table[i11];                                \
	    dst[i + 12] = law_table[i12];                                \
	    dst[i + 13] = law_table[i13];                                \
	    dst[i + 14] = law_table[i14];                                \
	    dst[i + 15] = law_table[i15];                                \
	    l00 = l20;                                                   \
	    l01 = l21;                                                   \
	    l10 = l30;                                                   \
	    l11 = l31;                                                   \
	}

#endif

/* *********************************************************** */

static void
mlib_s_SignalLaw2Law(
    mlib_u8 *dst,
    const mlib_u8 *src,
    mlib_s32 n,
    const mlib_u8 *law_table)
{
	mlib_s32 i, sel;
	__m128i accum;
	mlib_u32 l00, l10, l20, l30;
	mlib_u32 l01, l11, l21, l31;

	for (i = 0; (i < n) && ((mlib_addr)(dst + i) & 15); i++) {
		dst[i] = law_table[src[i]];
	}

	if (i < n - 31) {
		sel = (mlib_addr)(src + i) & 7;
		l00 = *(mlib_u32 *)(src + i - sel);
		l01 = *(mlib_u32 *)(src + 4 + i - sel);
		l10 = *(mlib_u32 *)(src + 8 + i - sel);
		l11 = *(mlib_u32 *)(src + 12 + i - sel);

		switch (sel) {
			case 0:
#ifdef	__SUNPRO_C
#pragma pipeloop(0)
#endif
			LAW2LAW_MAIN_LOOP(0,
			    l00 & 0xFF,
			    (l00 >> 8) & 0xFF,
			    (l00 >> 16) & 0xFF,
			    (l00 >> 24) & 0xFF,
			    l01 & 0xFF,
			    (l01 >> 8) & 0xFF,
			    (l01 >> 16) & 0xFF,
			    (l01 >> 24) & 0xFF,
			    l10 & 0xFF,
			    (l10 >> 8) & 0xFF,
			    (l10 >> 16) & 0xFF,
			    (l10 >> 24) & 0xFF,
			    l11 & 0xFF,
			    (l11 >> 8) & 0xFF,
			    (l11 >> 16) & 0xFF,
			    (l11 >> 24) & 0xFF)
			break;

			case 1:
#ifdef	__SUNPRO_C
#pragma pipeloop(0)
#endif
			LAW2LAW_MAIN_LOOP(1,
			    (l00 >> 8) & 0xFF,
			    (l00 >> 16) & 0xFF,
			    (l00 >> 24) & 0xFF,
			    l01 & 0xFF,
			    (l01 >> 8) & 0xFF,
			    (l01 >> 16) & 0xFF,
			    (l01 >> 24) & 0xFF,
			    l10 & 0xFF,
			    (l10 >> 8) & 0xFF,
			    (l10 >> 16) & 0xFF,
			    (l10 >> 24) & 0xFF,
			    l11 & 0xFF,
			    (l11 >> 8) & 0xFF,
			    (l11 >> 16) & 0xFF,
			    (l11 >> 24) & 0xFF,
			    l20 & 0xFF)
			break;

			case 2:
#ifdef	__SUNPRO_C
#pragma pipeloop(0)
#endif
			LAW2LAW_MAIN_LOOP(2,
			    (l00 >> 16) & 0xFF,
			    (l00 >> 24) & 0xFF,
			    l01 & 0xFF,
			    (l01 >> 8) & 0xFF,
			    (l01 >> 16) & 0xFF,
			    (l01 >> 24) & 0xFF,
			    l10 & 0xFF,
			    (l10 >> 8) & 0xFF,
			    (l10 >> 16) & 0xFF,
			    (l10 >> 24) & 0xFF,
			    l11 & 0xFF,
			    (l11 >> 8) & 0xFF,
			    (l11 >> 16) & 0xFF,
			    (l11 >> 24) & 0xFF,
			    l20 & 0xFF,
			    (l20 >> 8) & 0xFF)
			break;

			case 3:
#ifdef	__SUNPRO_C
#pragma pipeloop(0)
#endif
			LAW2LAW_MAIN_LOOP(3,
			    (l00 >> 24) & 0xFF,
			    l01 & 0xFF,
			    (l01 >> 8) & 0xFF,
			    (l01 >> 16) & 0xFF,
			    (l01 >> 24) & 0xFF,
			    l10 & 0xFF,
			    (l10 >> 8) & 0xFF,
			    (l10 >> 16) & 0xFF,
			    (l10 >> 24) & 0xFF,
			    l11 & 0xFF,
			    (l11 >> 8) & 0xFF,
			    (l11 >> 16) & 0xFF,
			    (l11 >> 24) & 0xFF,
			    l20 & 0xFF,
			    (l20 >> 8) & 0xFF,
			    (l20 >> 16) & 0xFF)
			break;

			case 4:
#ifdef	__SUNPRO_C
#pragma pipeloop(0)
#endif
			LAW2LAW_MAIN_LOOP(4,
			    l01 & 0xFF,
			    (l01 >> 8) & 0xFF,
			    (l01 >> 16) & 0xFF,
			    (l01 >> 24) & 0xFF,
			    l10 & 0xFF,
			    (l10 >> 8) & 0xFF,
			    (l10 >> 16) & 0xFF,
			    (l10 >> 24) & 0xFF,
			    l11 & 0xFF,
			    (l11 >> 8) & 0xFF,
			    (l11 >> 16) & 0xFF,
			    (l11 >> 24) & 0xFF,
			    l20 & 0xFF,
			    (l20 >> 8) & 0xFF,
			    (l20 >> 16) & 0xFF,
			    (l20 >> 24) & 0xFF)
			break;

			case 5:
#ifdef	__SUNPRO_C
#pragma pipeloop(0)
#endif
			LAW2LAW_MAIN_LOOP(5,
			    (l01 >> 8) & 0xFF,
			    (l01 >> 16) & 0xFF,
			    (l01 >> 24) & 0xFF,
			    l10 & 0xFF,
			    (l10 >> 8) & 0xFF,
			    (l10 >> 16) & 0xFF,
			    (l10 >> 24) & 0xFF,
			    l11 & 0xFF,
			    (l11 >> 8) & 0xFF,
			    (l11 >> 16) & 0xFF,
			    (l11 >> 24) & 0xFF,
			    l20 & 0xFF,
			    (l20 >> 8) & 0xFF,
			    (l20 >> 16) & 0xFF,
			    (l20 >> 24) & 0xFF,
			    l21 & 0xFF)
			break;

			case 6:
#ifdef	__SUNPRO_C
#pragma pipeloop(0)
#endif
			LAW2LAW_MAIN_LOOP(6,
			    (l01 >> 16) & 0xFF,
			    (l01 >> 24) & 0xFF,
			    l10 & 0xFF,
			    (l10 >> 8) & 0xFF,
			    (l10 >> 16) & 0xFF,
			    (l10 >> 24) & 0xFF,
			    l11 & 0xFF,
			    (l11 >> 8) & 0xFF,
			    (l11 >> 16) & 0xFF,
			    (l11 >> 24) & 0xFF,
			    l20 & 0xFF,
			    (l20 >> 8) & 0xFF,
			    (l20 >> 16) & 0xFF,
			    (l20 >> 24) & 0xFF,
			    l21 & 0xFF,
			    (l21 >> 8) & 0xFF)
			break;

			default:
#ifdef	__SUNPRO_C
#pragma pipeloop(0)
#endif
			LAW2LAW_MAIN_LOOP(7,
			    (l01 >> 24) & 0xFF,
			    l10 & 0xFF,
			    (l10 >> 8) & 0xFF,
			    (l10 >> 16) & 0xFF,
			    (l10 >> 24) & 0xFF,
			    l11 & 0xFF,
			    (l11 >> 8) & 0xFF,
			    (l11 >> 16) & 0xFF,
			    (l11 >> 24) & 0xFF,
			    l20 & 0xFF,
			    (l20 >> 8) & 0xFF,
			    (l20 >> 16) & 0xFF,
			    (l20 >> 24) & 0xFF,
			    l21 & 0xFF,
			    (l21 >> 8) & 0xFF,
			    (l21 >> 16) & 0xFF)
		}
	}

	for (; i < n; i++) {
		dst[i] = law_table[src[i]];
	}
}

/* *********************************************************** */

mlib_status
__mlib_SignalALaw2uLaw(
    mlib_u8 *ucode,
    const mlib_u8 *acode,
    mlib_s32 n)
{
	if ((ucode == NULL) | (acode == NULL))
		return (MLIB_FAILURE);

	if (n <= 0)
		return (MLIB_FAILURE);

	mlib_s_SignalLaw2Law(ucode, acode, n, mlib_a2u);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_SignaluLaw2ALaw(
    mlib_u8 *acode,
    const mlib_u8 *ucode,
    mlib_s32 n)
{
	if ((ucode == NULL) | (acode == NULL))
		return (MLIB_FAILURE);

	if (n <= 0)
		return (MLIB_FAILURE);

	mlib_s_SignalLaw2Law(acode, ucode, n, mlib_u2a);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
