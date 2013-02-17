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

#pragma ident	"@(#)mlib_SignalMerge.c	9.2	07/10/09 SMI"

/*
 * FUNCTIONS
 *  mlib_SignalMerge_S16S_S16  - merge of two signal arrays to form a stereo
 *                               signal array
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_SignalMerge_S16S_S16(mlib_s16       *dst,
 *                                            const mlib_s16 *ch0,
 *                                            const mlib_s16 *ch1,
 *                                            mlib_s32       n);
 * ARGUMENTS
 *      dst   Output stereo signal array. dst[2*i] hold the channel 0, and
 *            dst[2*i+1] hold the channel 1
 *      ch0   Input signal array of channel 0
 *      ch1   Input signal array of channel 1
 *      n     Number of samples in the input signal array
 *
 * DESCRIPTION
 *
 *      dst = ch0 + ch1
 *
 *      ch0 = (l1 l2 ... ln)
 *      ch1 = (r1 r2 ... rn)
 *      dst = (l1+r1 l2+r2 ... ln+rn)
 */

#include <mlib_signal.h>
#include <mlib_algebra.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_SignalMerge_S16S_S16 = __mlib_SignalMerge_S16S_S16

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_SignalMerge_S16S_S16) mlib_SignalMerge_S16S_S16
    __attribute__((weak, alias("__mlib_SignalMerge_S16S_S16")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

mlib_status
__mlib_SignalMerge_S16S_S16(
    mlib_s16 *dst,
    const mlib_s16 *ch0,
    const mlib_s16 *ch1,
    mlib_s32 n)
{
	return (__mlib_VectorMerge_S16C_S16(dst, ch0, ch1, n));
}

/* *********************************************************** */
