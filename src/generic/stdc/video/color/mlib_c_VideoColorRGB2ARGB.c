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

#pragma ident	"@(#)mlib_c_VideoColorRGB2ARGB.c	9.2	07/10/09 SMI"

/*
 * FUNCTION
 *      mlib_VideoColorRGB2ARGB - RGB to ARGB conversion
 *
 * SYNOPSIS
 *      mlib_status mlib_VideoColorRGB2ARGB(mlib_u8       *argb,
 *                                          const mlib_u8 *rgb,
 *                                          mlib_s32      n)
 *
 * ARGUMENTS
 *      argb      Pointer to ARGB multicomponent row, 8-byte aligned
 *      rgb       Pointer to  RGB multicomponent row, 8-byte aligned
 *      n         Number of pixels
 *
 * DESCRIPTION
 *
 *      argb[4*i] - undefined
 *      argb[4*i+1] = rgb[3*i]
 *      argb[4*i+2] = rgb[3*i+1]
 *      argb[4*i+3] = rgb[3*i+2]
 *
 *      0 <= i < n
 */

#include <mlib_video.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_VideoColorRGB2ARGB = __mlib_VideoColorRGB2ARGB

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_VideoColorRGB2ARGB) mlib_VideoColorRGB2ARGB
	__attribute__((weak, alias("__mlib_VideoColorRGB2ARGB")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#ifdef _LITTLE_ENDIAN

#define	MERGE_S32(argb0, argb1, argb2, argb3, rgbr, gbrg, brgb)   \
	argb0 = rgbr << 8;                                        \
	argb1 = ((rgbr >> 16) & ~0xFF) | (gbrg << 16);            \
	argb2 = ((gbrg >> 8) & ~0xFF) | (brgb << 24);             \
	argb3 = brgb & ~0xFF

#define	MERGE_S32_U8(x, a0, a1, a2)                             \
	x = (a2 << 24) | (a1 << 16) | (a0 << 8)

#else /* _LITTLE_ENDIAN */

#define	MERGE_S32(argb0, argb1, argb2, argb3, rgbr, gbrg, brgb)   \
	argb0 = rgbr >> 8;                                        \
	argb1 = ((rgbr & 0xFF) << 16) | (gbrg >> 16);             \
	argb2 = ((gbrg & 0xFFFF) << 8) | (brgb >> 24);            \
	argb3 = brgb & 0xFFFFFF

#define	MERGE_S32_U8(x, a0, a1, a2)	x = (a0 << 16) | (a1 << 8) | (a2)

#endif /* _LITTLE_ENDIAN */

/* *********************************************************** */

mlib_status
__mlib_VideoColorRGB2ARGB(
	mlib_u8 *argb,
	const mlib_u8 *rgb,
	mlib_s32 n)
{
	mlib_u32 *sp = (mlib_u32 *)rgb, *dp = (mlib_u32 *)argb;
	mlib_s32 i;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#pragma nomemorydepend
#endif /* __SUNPRO_C */
	for (i = 0; i < n / 4; i++) {
		MERGE_S32(dp[4 * i], dp[4 * i + 1], dp[4 * i + 2],
			dp[4 * i + 3], sp[3 * i], sp[3 * i + 1], sp[3 * i + 2]);
	}

#ifdef __SUNPRO_C
#pragma unroll(1)
#endif /* __SUNPRO_C */
	for (i *= 4; i < n; i++) {
		MERGE_S32_U8(dp[i], rgb[3 * i], rgb[3 * i + 1], rgb[3 * i + 2]);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
