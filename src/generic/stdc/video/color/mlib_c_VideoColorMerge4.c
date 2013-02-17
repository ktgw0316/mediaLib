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

#pragma ident	"@(#)mlib_c_VideoColorMerge4.c	9.2	07/10/09 SMI"

/*
 * FUNCTION
 *      mlib_VideoColorMerge4 - merges four channels colors
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_VideoColorMerge4(mlib_u8       *colors,
 *                                        const mlib_u8 *color1,
 *                                        const mlib_u8 *color2,
 *                                        const mlib_u8 *color3,
 *                                        const mlib_u8 *color4,
 *                                        mlib_s32      n)
 *
 * ARGUMENTS
 *      colors    Pointer to colors multicomponent row, 8-byte aligned
 *      color1    Pointer to first color component row, 8-byte aligned
 *      color2    Pointer to second color component row, 8-byte aligned
 *      color3    Pointer to third color component row, 8-byte aligned
 *      color4    Pointer to forth color component row, 8-byte aligned
 *      n         Length of the color1 array
 *
 * DESCRIPTION
 *
 *      colors[4*i]   = color1[i]
 *      colors[4*i+1] = color2[i]
 *      colors[4*i+2] = color3[i]
 *      colors[4*i+3] = color4[i]
 *
 *      0 <= i < n
 */

#include <mlib_video.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_VideoColorMerge4 = __mlib_VideoColorMerge4

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_VideoColorMerge4) mlib_VideoColorMerge4
	__attribute__((weak, alias("__mlib_VideoColorMerge4")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

mlib_status
__mlib_VideoColorMerge4(
	mlib_u8 *colors,
	const mlib_u8 *color1,
	const mlib_u8 *color2,
	const mlib_u8 *color3,
	const mlib_u8 *color4,
	mlib_s32 n)
{
	mlib_u16 *dp = (mlib_u16 *)colors;
	mlib_u16 *sp0 = (mlib_u16 *)color1;
	mlib_u16 *sp1 = (mlib_u16 *)color2;
	mlib_u16 *sp2 = (mlib_u16 *)color3;
	mlib_u16 *sp3 = (mlib_u16 *)color4;
	mlib_s32 s0, s1, s2, s3;
	mlib_s32 i;

/* 2-pixels loop */
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i <= (n - 2); i += 2) {
		s0 = (*sp0++);
		s1 = (*sp1++);
		s2 = (*sp2++);
		s3 = (*sp3++);
#ifdef _LITTLE_ENDIAN
		dp[2] = (s1 & ~0xFF) + (s0 >> 8);
		dp[3] = (s3 & ~0xFF) + (s2 >> 8);
		dp[0] = (s1 << 8) + (s0 & 0xFF);
		dp[1] = (s3 << 8) + (s2 & 0xFF);
#else /* _LITTLE_ENDIAN */
		dp[0] = (s0 & ~0xFF) + (s1 >> 8);
		dp[1] = (s2 & ~0xFF) + (s3 >> 8);
		dp[2] = (s0 << 8) + (s1 & 0xFF);
		dp[3] = (s2 << 8) + (s3 & 0xFF);
#endif /* _LITTLE_ENDIAN */
		dp += 4;
	}

/* last pixel */

	if (i < n) {
		s0 = *((mlib_u8 *)sp0);
		s1 = *((mlib_u8 *)sp1);
		s2 = *((mlib_u8 *)sp2);
		s3 = *((mlib_u8 *)sp3);
#ifdef _LITTLE_ENDIAN
		dp[0] = (s1 << 8) + s0;
		dp[1] = (s3 << 8) + s2;
#else /* _LITTLE_ENDIAN */
		dp[0] = (s0 << 8) + s1;
		dp[1] = (s2 << 8) + s3;
#endif /* _LITTLE_ENDIAN */
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
