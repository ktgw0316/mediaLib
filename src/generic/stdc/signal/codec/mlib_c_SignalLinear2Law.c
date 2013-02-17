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

#pragma ident	"@(#)mlib_c_SignalLinear2Law.c	9.2	07/10/09 SMI"

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

mlib_status
__mlib_SignalLinear2ALaw(
    mlib_u8 *acode,
    const mlib_s16 *pcm,
    mlib_s32 n)
{
	mlib_u8 *dp = acode;
	mlib_u16 *sp = (mlib_u16 *)pcm;
	mlib_u32 s0, s1, s2;
	mlib_s32 i = 0;

	if ((sp == NULL) | (dp == NULL))
		return (MLIB_FAILURE);

	if (n <= 0)
		return (MLIB_OUTOFRANGE);

#ifndef _LITTLE_ENDIAN
	for (; (i < n) && ((mlib_addr)(dp + i) & 3); i++) {
		dp[i] = mlib_l2A[(sp[i] >> 4)];
	}

	if (((mlib_addr)(sp + i) & 3) == 0) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (; i < (n - 4); i += 4) {
			s0 = *(mlib_u32 *)(sp + i);
			s1 = *(mlib_u32 *)(sp + i + 2);
			*(mlib_u32 *)(dp + i) =
			    (mlib_l2A[s0 >> 20] << 24) | (mlib_l2A[(s0 >> 4) &
			    0xFFF] << 16) | (mlib_l2A[s1 >> 20] << 8) |
			    mlib_l2A[(s1 >> 4) & 0xFFF];
		}
	} else {
		s0 = *(mlib_u32 *)(sp + i - 1);
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (; i < (n - 4); i += 4) {
			s1 = *(mlib_u32 *)(sp + i + 1);
			s2 = *(mlib_u32 *)(sp + i + 3);
			*(mlib_u32 *)(dp + i) =
			    (mlib_l2A[(s0 >> 4) & 0xFFF] << 24) | (mlib_l2A[s1
			    >> 20] << 16) | (mlib_l2A[(s1 >> 4) & 0xFFF] << 8) |
			    mlib_l2A[s2 >> 20];
			s0 = s2;
		}
	}

#endif /* _LITTLE_ENDIAN */

	for (; i < n; i++) {
		dp[i] = mlib_l2A[(sp[i] >> 4)];
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_SignalLinear2uLaw(
    mlib_u8 *ucode,
    const mlib_s16 *pcm,
    mlib_s32 n)
{
	mlib_u8 *dp = ucode;
	mlib_u16 *sp = (mlib_u16 *)pcm;
	mlib_s32 i, ind, zn;

	if ((sp == NULL) | (dp == NULL))
		return (MLIB_FAILURE);

	if (n <= 0)
		return (MLIB_OUTOFRANGE);

	for (i = 0; i < n; i++) {
		ind = sp[i] + 4;
		zn = sp[i] >> 15;
		ind -= zn;
		ind >>= 3;
		ind += zn;

		dp[i] = mlib_l2u[ind];
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
