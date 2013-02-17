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

#pragma ident	"@(#)mlib_SignalSplit_Fp.c	9.2	07/10/09 SMI"

/*
 * FUNCTIONS
 *      mlib_SignalSplit_F32_F32S  - split a stereo signal array into two
 *                                   signal arrays
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_SignalSplit_F32_F32S(mlib_f32       *ch0,
 *                                            mlib_f32       *ch1,
 *                                            const mlib_f32 *src,
 *                                            mlib_s32       n);
 * ARGUMENTS
 *      ch0  Output signal array of channel 0
 *      ch1  Output signal array of channel 1
 *      src  Input stereo signal array. src[2*i] hold the channel 0, and
 *           src[2*i+1] hold channel 1
 *      n    Number of samples in the input signal array
 *
 * DESCRIPTION
 *
 *      src = (l1+r1 l2+r2 ... ln+rn)
 *      ch0 = (l1 l2 ... ln)
 *      ch1 = (r1 r2 ... rn)
 *
 */

#include <mlib_signal.h>
#include <mlib_algebra.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_SignalSplit_F32_F32S = __mlib_SignalSplit_F32_F32S

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_SignalSplit_F32_F32S) mlib_SignalSplit_F32_F32S
    __attribute__((weak, alias("__mlib_SignalSplit_F32_F32S")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

mlib_status
__mlib_SignalSplit_F32_F32S(
    mlib_f32 *ch0,
    mlib_f32 *ch1,
    const mlib_f32 *src,
    mlib_s32 n)
{
	return __mlib_VectorSplit_S32_S32C((mlib_s32 *)ch0, (mlib_s32 *)ch1,
	    (mlib_s32 *)src, n);
}

/* *********************************************************** */
