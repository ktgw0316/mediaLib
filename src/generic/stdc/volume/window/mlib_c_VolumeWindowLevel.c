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

#pragma ident	"@(#)mlib_c_VolumeWindowLevel.c	9.2	07/10/09 SMI"

/*
 *  FUNCTION
 *    mlib_VolumeWindowLevel
 *
 *  SYNOPSIS
 *    mlib_status mlib_VolumeWindowLevel(mlib_u8        *dst,
 *                                       const mlib_s16 *src,
 *                                       mlib_s32       window,
 *                                       mlib_s32       level,
 *                                       mlib_s32       gmax,
 *                                       mlib_s32       gmin,
 *                                       mlib_s32       len)
 *
 *  ARGUMENTS
 *    dst       Pointer to output or destination array.
 *    src       Pointer to input or source array.
 *    window    Width of window.
 *    level     Center of window.
 *    gmax      Maximum grayscale in the destination array.
 *    gmin      Minimum grayscale in the destination array.
 *    len       Length of data array.
 *
 *  DESCRIPTION
 */

#include <stdlib.h>
#include <mlib_volume.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_VolumeWindowLevel = __mlib_VolumeWindowLevel

#elif defined(__GNUC__)

__typeof__(__mlib_VolumeWindowLevel) mlib_VolumeWindowLevel
	__attribute__((weak, alias("__mlib_VolumeWindowLevel")));

#else

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

mlib_status
__mlib_VolumeWindowLevel(
	mlib_u8 *dst,
	const mlib_s16 *src,
	mlib_s32 window,
	mlib_s32 level,
	mlib_s32 gmax,
	mlib_s32 gmin,
	mlib_s32 len)
{
	mlib_d64 a = (mlib_d64)(gmax - gmin) / window;
	mlib_d64 b = gmin - a * (level - window * 0.5);
	mlib_s32 i, s;
	mlib_s32 rsl, hmask, lmask;
	mlib_s32 ileft = ((2 * level - window) >> 1) + 1;
	mlib_s32 iright = ((2 * level + window) >> 1) - 1;

	if (dst == NULL || src == NULL)
		return (MLIB_FAILURE);
	if (window <= 0 || gmin > gmax || gmax > MLIB_U8_MAX ||
		gmin < MLIB_U8_MIN)
		return (MLIB_FAILURE);
	if (ileft < MLIB_S16_MIN || iright > MLIB_S16_MAX || ileft > iright)
		return (MLIB_FAILURE);

	for (i = 0; i < len; i++) {
		s = src[i];
		rsl = a * s + b;
		hmask = (iright - s) >> 16;
		lmask = (s - ileft) >> 16;
		dst[i] = (rsl & ~(lmask | hmask)) | (gmin & lmask) | (gmax &
			hmask);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
