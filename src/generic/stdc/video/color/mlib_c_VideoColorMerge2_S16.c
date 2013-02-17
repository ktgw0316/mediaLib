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

#pragma ident	"@(#)mlib_c_VideoColorMerge2_S16.c	9.2	07/10/09 SMI"

/*
 * FUNCTION
 *      mlib_VideoColorMerge2_S16 - merges two channels colors
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_VideoColorMerge2_S16(mlib_s16       *colors,
 *                                            const mlib_s16 *color1,
 *                                            const mlib_s16 *color2,
 *                                            mlib_s32       n)
 *
 * ARGUMENTS
 *      colors    Pointer to colors multicomponent row, 8-byte aligned
 *      color1    Pointer to first color component row, 8-byte aligned
 *      color2    Pointer to second color component row, 8-byte aligned
 *      n         Length of the color1 array
 *
 * DESCRIPTION
 *
 *      colors[2*i]   = color1[i]
 *      colors[2*i+1] = color2[i]
 *
 *      0 <= i < n
 */

#include <mlib_video.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_VideoColorMerge2_S16 = __mlib_VideoColorMerge2_S16

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_VideoColorMerge2_S16) mlib_VideoColorMerge2_S16
	__attribute__((weak, alias("__mlib_VideoColorMerge2_S16")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

mlib_status
__mlib_VideoColorMerge2_S16(
	mlib_s16 *colors,
	const mlib_s16 *color1,
	const mlib_s16 *color2,
	mlib_s32 n)
{
	mlib_u32 *dp = (mlib_u32 *)colors;
	mlib_u32 *sp0 = (mlib_u32 *)color1;
	mlib_u32 *sp1 = (mlib_u32 *)color2;
	mlib_u32 s0, s1;
	mlib_s32 i;

/* 2-pixels loop */
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i <= (n - 2); i += 2) {
		s0 = (*sp0++);
		s1 = (*sp1++);
#ifdef _LITTLE_ENDIAN
		dp[1] = (s1 & ~0xFFFF) + (s0 >> 16);
		dp[0] = (s1 << 16) + (s0 & 0xFFFF);
#else /* _LITTLE_ENDIAN */
		dp[0] = (s0 & ~0xFFFF) + (s1 >> 16);
		dp[1] = (s0 << 16) + (s1 & 0xFFFF);
#endif /* _LITTLE_ENDIAN */
		dp += 2;
	}

/* last  pixel */

	if (i < n) {
		s0 = *((mlib_u16 *)sp0);
		s1 = *((mlib_u16 *)sp1);
#ifdef _LITTLE_ENDIAN
		dp[0] = (s1 << 16) + s0;
#else /* _LITTLE_ENDIAN */
		dp[0] = (s0 << 16) + s1;
#endif /* _LITTLE_ENDIAN */
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
