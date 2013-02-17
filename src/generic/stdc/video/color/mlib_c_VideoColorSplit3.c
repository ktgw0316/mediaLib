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

#pragma ident	"@(#)mlib_c_VideoColorSplit3.c	9.2	07/10/09 SMI"

/*
 * FUNCTION
 *      mlib_VideoColorSplit3 - split three channels colors
 *
 * SYNOPSIS
 *      mlib_status mlib_VideoColorSplit3(mlib_u8       *color1,
 *                                        mlib_u8       *color2,
 *                                        mlib_u8       *color3,
 *                                        const mlib_u8 *colors,
 *                                        mlib_s32      n)
 *
 * ARGUMENTS
 *      color1    Pointer to first  color component row, 8-byte aligned
 *      color2    Pointer to second color component row, 8-byte aligned
 *      color3    Pointer to third  color component row, 8-byte aligned
 *      colors    Pointer to colors multicomponent row, 8-byte aligned
 *      n         Length of color1, color2, color3 arrays
 *                Note: length of colors array must be 3*n
 *
 * DESCRIPTION
 *
 *        color1[i] = colors[3*i]
 *        color2[i] = colors[3*i+1]
 *        color3[i] = colors[3*i+2]
 *
 *        0 <= i < n
 */

#include <mlib_video.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_VideoColorSplit3 = __mlib_VideoColorSplit3

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_VideoColorSplit3) mlib_VideoColorSplit3
	__attribute__((weak, alias("__mlib_VideoColorSplit3")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

mlib_status
__mlib_VideoColorSplit3(
	mlib_u8 *color1,
	mlib_u8 *color2,
	mlib_u8 *color3,
	const mlib_u8 *colors,
	mlib_s32 n)
{
	mlib_u16 *sp = (mlib_u16 *)colors;
	mlib_u16 *dp0 = (mlib_u16 *)color1;
	mlib_u16 *dp1 = (mlib_u16 *)color2;
	mlib_u16 *dp2 = (mlib_u16 *)color3;
	mlib_s32 s0, s1, s2;
	mlib_s32 i;

/* 2-pixels loop */
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i <= (n - 2); i += 2) {
		s0 = sp[0];
		s1 = sp[1];
		s2 = sp[2];
#ifdef _LITTLE_ENDIAN
		(*dp0++) = (s1 & ~0xFF) + (s0 & 0xFF);
		(*dp1++) = (s2 << 8) + (s0 >> 8);
		(*dp2++) = (s2 & ~0xFF) + (s1 & 0xFF);
#else /* _LITTLE_ENDIAN */
		(*dp0++) = (s0 & ~0xFF) + (s1 & 0xFF);
		(*dp1++) = (s0 << 8) + (s2 >> 8);
		(*dp2++) = (s1 & ~0xFF) + (s2 & 0xFF);
#endif /* _LITTLE_ENDIAN */
		sp += 3;
	}

/* last pixel */

	if (i < n) {
		s0 = sp[0];
#ifdef _LITTLE_ENDIAN
		*((mlib_u8 *)dp0) = s0;
		*((mlib_u8 *)dp1) = s0 >> 8;
#else /* _LITTLE_ENDIAN */
		*((mlib_u8 *)dp0) = s0 >> 8;
		*((mlib_u8 *)dp1) = s0;
#endif /* _LITTLE_ENDIAN */
		*((mlib_u8 *)dp2) = ((mlib_u8 *)sp)[2];
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
