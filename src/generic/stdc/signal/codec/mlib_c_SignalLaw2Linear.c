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

#pragma ident	"@(#)mlib_c_SignalLaw2Linear.c	9.3	07/10/09 SMI"

/*
 * FUNCTIONS
 *
 *      mlib_SignalALaw2Linear - Functions in this section perform A-law
 *      decompression in compliance with the ITU (former CCITT) specification.
 *
 *      mlib_SignaluLaw2Linear - Functions in this section perform u-law
 *      decompression in compliance with the ITU (former CCITT) specification.
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_SignalALaw2Linear(mlib_s16 *pcm,
 *                                         mlib_u8  *acode,
 *                                         mlib_s32 n)
 *
 *      mlib_status mlib_SignaluLaw2Linear(mlib_s16 *pcm,
 *                                         mlib_u8  *ucode,
 *                                         mlib_s32 n)
 *
 * ARGUMENTS
 *
 *      pcm      Linear PCM sample array
 *      acode    A-law code array
 *      ucode    u-law code array
 *      n        Number of samples in the input array
 *
 */

#include <mlib_signal.h>
#include <mlib_SignalCodec.h>

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

#define	TABLE(i)	(*(mlib_u16*)((mlib_u8*)linearTable + (i)))

/* *********************************************************** */

#define	LAW2LINEAR_MAIN_LOOP(sel, i0, i1, i2, i3)               \
	for (; i < n - 3; i += 4) {                             \
	    s1 = *(mlib_u32 *)(sp + i + (4 - sel));             \
	    d0 = (TABLE(i0) << 16) | TABLE(i1);                 \
	    d1 = (TABLE(i2) << 16) | TABLE(i3);                 \
	    *(mlib_u32 *)(dp + i) = d0;                         \
	    *(mlib_u32 *)(dp + i + 2) = d1;                     \
	    s0 = s1;                                            \
	}

/* *********************************************************** */

#define	LAW2LINEAR_MAIN_LOOP_AL(sel, i0, i1, i2, i3)            \
	for (; i < n - 7; i += 4) {                             \
	    s1 = *(mlib_u32 *)(sp + i + (4 - sel));             \
	    d0 = (TABLE(i0) << 16) | TABLE(i1);                 \
	    d1 = (TABLE(i2) << 16) | TABLE(i3);                 \
	    *(mlib_u32 *)(dp + i) = d0;                         \
	    *(mlib_u32 *)(dp + i + 2) = d1;                     \
	    s0 = s1;                                            \
	}                                                       \
	d0 = (TABLE(i0) << 16) | TABLE(i1);                     \
	d1 = (TABLE(i2) << 16) | TABLE(i3);                     \
	*(mlib_u32 *)(dp + i) = d0;                             \
	*(mlib_u32 *)(dp + i + 2) = d1;                         \
	i += 4

/* *********************************************************** */

static void
mlib_c_SignalLaw2Linear(
    mlib_s16 *pcm,
    const mlib_u8 *code,
    mlib_s32 n,
    const mlib_u16 *linearTable)
{
	mlib_u8 *sp = (mlib_u8 *)code;
	mlib_u16 *dp = (mlib_u16 *)pcm;
	mlib_s32 i = 0, sel;
	mlib_u32 s0, s1, d0, d1;

#ifndef _LITTLE_ENDIAN
	for (; (i < n) && ((mlib_addr)(dp + i) & 3); i++) {
		dp[i] = linearTable[sp[i]];
	}

	if (i < n - 3) {
		sel = (mlib_addr)(sp + i) & 3;
		s0 = *(mlib_u32 *)(sp + i - sel);

		if (sel == 0) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			LAW2LINEAR_MAIN_LOOP_AL(0,
			    (s0 >> 23) & 0x1FE,
			    (s0 >> 15) & 0x1FE,
			    (s0 >> 7) & 0x1FE, (s0 << 1) & 0x1FE);
		} else if (sel == 1) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			LAW2LINEAR_MAIN_LOOP(1,
			    (s0 >> 15) & 0x1FE,
			    (s0 >> 7) & 0x1FE,
			    (s0 << 1) & 0x1FE, (s1 >> 23) & 0x1FE);
		} else if (sel == 2) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			LAW2LINEAR_MAIN_LOOP(2,
			    (s0 >> 7) & 0x1FE,
			    (s0 << 1) & 0x1FE,
			    (s1 >> 23) & 0x1FE, (s1 >> 15) & 0x1FE);
		} else {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			LAW2LINEAR_MAIN_LOOP(3,
			    (s0 << 1) & 0x1FE,
			    (s1 >> 23) & 0x1FE,
			    (s1 >> 15) & 0x1FE, (s1 >> 7) & 0x1FE);
		}
	}
#endif /* _LITTLE_ENDIAN */

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

	mlib_c_SignalLaw2Linear(pcm, acode, n, mlib_A2l);

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

	mlib_c_SignalLaw2Linear(pcm, ucode, n, mlib_u2l);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
