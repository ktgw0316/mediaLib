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

#pragma ident	"@(#)mlib_c_VideoColorABGR2RGB.c	9.2	07/10/09 SMI"

/*
 * FUNCTION
 *      mlib_VideoColorABGR2RGB - ABGR to RGB conversion
 *
 * SYNOPSIS
 *      mlib_status mlib_VideoColorABGR2RGB(mlib_u8       *rgb,
 *                                          const mlib_u8 *abgr,
 *                                          mlib_s32      n)
 *
 * ARGUMENTS
 *      rgb       Pointer to  RGB multicomponent row, 8-byte aligned
 *      abgr      Pointer to ABGR multicomponent row, 8-byte aligned
 *      n         Number of pixels
 *
 * DESCRIPTION
 *
 *      rgb[3*i  ] = argb[4*i+3]
 *      rgb[3*i+1] = argb[4*i+2]
 *      rgb[3*i+2] = argb[4*i+1]
 *
 *      0 <= i < n
 */

#include <mlib_video.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_VideoColorABGR2RGB = __mlib_VideoColorABGR2RGB

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_VideoColorABGR2RGB) mlib_VideoColorABGR2RGB
	__attribute__((weak, alias("__mlib_VideoColorABGR2RGB")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#ifdef _LITTLE_ENDIAN

#define	SPLIT_S32(abgr0, b1, gr1, r0g0, b0r1, g1b1)             \
	r0g0 = ((abgr0 >> 8) & ~0xFF) | (abgr0 >> 24);          \
	b0r1 = ((abgr0 >> 8) & 0xFF) | (gr1 & ~0xFF);           \
	g1b1 = (gr1 & 0xFF) | (b1 << 8)

#define	SPLIT_S32_3_U8(x, a1, a2, a3)                           \
	a3 = (x) >> 24;                                         \
	a2 = (x) >> 16;                                         \
	a1 = (x) >> 8

#else /* _LITTLE_ENDIAN */

#define	SPLIT_S32(abgr0, b1, gr1, r0g0, b0r1, g1b1)             \
	r0g0 = (abgr0 << 8) | ((abgr0 >> 8) & 0xFF);            \
	b0r1 = ((abgr0 >> 8) & ~0xFF) | (gr1 & 0xFF);           \
	g1b1 = (gr1 & ~0xFF) | b1

#define	SPLIT_S32_3_U8(x, a1, a2, a3)                           \
	a1 = (x) >> 16;                                         \
	a2 = (x) >> 8;                                          \
	a3 = (x)

#endif /* _LITTLE_ENDIAN */

/* *********************************************************** */

mlib_status
__mlib_VideoColorABGR2RGB(
	mlib_u8 *rgb,
	const mlib_u8 *abgr,
	mlib_s32 n)
{
	mlib_u32 *sp = (mlib_u32 *)abgr;
	mlib_u16 *dp = (mlib_u16 *)rgb;
	mlib_s32 i;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < n / 2; i++) {
		SPLIT_S32(sp[2 * i], ((mlib_u8 *)sp)[8 * i + 5],
			((mlib_u16 *)sp)[4 * i + 3], dp[3 * i], dp[3 * i + 1],
			dp[3 * i + 2]);
	}

	if (2 * i < n) {
		SPLIT_S32_3_U8(sp[2 * i], rgb[6 * i + 2], rgb[6 * i + 1],
			rgb[6 * i]);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
