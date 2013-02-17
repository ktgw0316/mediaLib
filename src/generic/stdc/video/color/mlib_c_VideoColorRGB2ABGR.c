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

#pragma ident	"@(#)mlib_c_VideoColorRGB2ABGR.c	9.2	07/10/09 SMI"

/*
 * FUNCTION
 *      mlib_VideoColorRGB2ABGR - RGB to ABGR conversion
 *
 * SYNOPSIS
 *      mlib_status mlib_VideoColorRGB2ABGR(mlib_u8       *abgr,
 *                                          const mlib_u8 *rgb,
 *                                          mlib_s32      n)
 *
 * ARGUMENTS
 *      abgr      Pointer to ABGR multicomponent row, 8-byte aligned
 *      rgb       Pointer to  RGB multicomponent row, 8-byte aligned
 *      n         Number of pixels
 *
 *      abgr[4*i] - undefined
 *      abgr[4*i+1] = rgb[3*i+2]
 *      abgr[4*i+2] = rgb[3*i+1]
 *      abgr[4*i+3] = rgb[3*i]
 *
 *      0 <= i < n
 */

#include <mlib_video.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_VideoColorRGB2ABGR = __mlib_VideoColorRGB2ABGR

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_VideoColorRGB2ABGR) mlib_VideoColorRGB2ABGR
	__attribute__((weak, alias("__mlib_VideoColorRGB2ABGR")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#ifdef _LITTLE_ENDIAN

#define	MERGE_S32(abgr0, abgr1, r0, g0, b0r1, g1b1)                  \
	abgr0 = ((g0 | (r0 << 8)) << 16) | ((b0r1 & 0xFF) << 8);     \
	abgr1 = (((b0r1 & ~0xFF) | (g1b1 & 0xFF)) << 16) | (g1b1 &   \
		~0xFF)

#define	MERGE_S32_U8(x, a0, a1, a2)                             \
	x = (a2 << 24) | (a1 << 16) | (a0 << 8)

#else /* _LITTLE_ENDIAN */

#define	MERGE_S32(abgr0, abgr1, r0, g0, b0r1, g1b1)                      \
	abgr0 = r0 | (g0 << 8) | ((b0r1 & ~0xFF) << 8);                  \
	abgr1 = (b0r1 & 0xFF) | (g1b1 & ~0xFF) | ((g1b1 & 0xFF) << 16)

#define	MERGE_S32_U8(x, a0, a1, a2)	x = (a0 << 16) | (a1 << 8) | (a2)

#endif /* _LITTLE_ENDIAN */

/* *********************************************************** */

mlib_status
__mlib_VideoColorRGB2ABGR(
	mlib_u8 *abgr,
	const mlib_u8 *rgb,
	mlib_s32 n)
{
	mlib_s32 i;
	mlib_u32 *dp = (mlib_u32 *)abgr;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#pragma nomemorydepend
#endif /* __SUNPRO_C */
	for (i = 0; i < n / 2; i++) {
		MERGE_S32(dp[2 * i], dp[2 * i + 1], rgb[6 * i], rgb[6 * i + 1],
			((mlib_u16 *)rgb)[3 * i + 1],
			((mlib_u16 *)rgb)[3 * i + 2]);
	}

	if (2 * i < n) {
		MERGE_S32_U8(dp[2 * i], rgb[6 * i + 2], rgb[6 * i + 1],
			rgb[6 * i]);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
