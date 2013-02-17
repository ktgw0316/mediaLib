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

#pragma ident	"@(#)mlib_c_VideoColorARGB2RGB.c	9.2	07/10/09 SMI"

/*
 * FUNCTION
 *      mlib_VideoColorARGB2RGB - ARGB to RGB conversion
 *
 * SYNOPSIS
 *      mlib_status mlib_VideoColorARGB2RGB(mlib_u8       *rgb,
 *                                          const mlib_u8 *argb,
 *                                          mlib_s32      n)
 *
 * ARGUMENTS
 *      rgb       Pointer to  RGB multicomponent row, 8-byte aligned
 *      argb      Pointer to ARGB multicomponent row, 8-byte aligned
 *      n         Number of pixels
 *
 * DESCRIPTION
 *
 *      rgb[3*i  ] = argb[4*i+1]
 *      rgb[3*i+1] = argb[4*i+2]
 *      rgb[3*i+2] = argb[4*i+3]
 *
 *      0 <= i < n
 */

#include <mlib_video.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_VideoColorARGB2RGB = __mlib_VideoColorARGB2RGB

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_VideoColorARGB2RGB) mlib_VideoColorARGB2RGB
	__attribute__((weak, alias("__mlib_VideoColorARGB2RGB")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#ifdef _LITTLE_ENDIAN

#define	SPLIT_S32(rgbr, gbrg, brgb, argb0, argb1, argb2, argb3)   \
	rgbr = (argb0 >> 8) | ((argb1 & ~0xFF) << 16);            \
	gbrg = (argb1 >> 16) | ((argb2 & ~0xFF) << 8);            \
	brgb = (argb2 >> 24) | (argb3 & ~0xFF)

#define	SPLIT_S32_3_U8(x, a1, a2, a3)                           \
	a3 = (x) >> 24;                                         \
	a2 = (x) >> 16;                                         \
	a1 = (x) >> 8

#else /* _LITTLE_ENDIAN */

#define	SPLIT_S32(rgbr, gbrg, brgb, argb0, argb1, argb2, argb3)   \
	rgbr = (argb0 << 8) | ((argb1 >> 16) & 0xFF);             \
	gbrg = (argb1 << 16) | ((argb2 >> 8) & 0xFFFF);           \
	brgb = (argb2 << 24) | (argb3 & 0xFFFFFF)

#define	SPLIT_S32_3_U8(x, a1, a2, a3)                           \
	a1 = (x) >> 16;                                         \
	a2 = (x) >> 8;                                          \
	a3 = (x)

#endif /* _LITTLE_ENDIAN */

/* *********************************************************** */

mlib_status
__mlib_VideoColorARGB2RGB(
	mlib_u8 *rgb,
	const mlib_u8 *argb,
	mlib_s32 n)
{
	mlib_u32 *sp = (mlib_u32 *)argb, *dp = (mlib_u32 *)rgb;
	mlib_s32 i;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#pragma nomemorydepend
#endif /* __SUNPRO_C */
	for (i = 0; i < n / 4; i++) {
		SPLIT_S32(dp[3 * i], dp[3 * i + 1], dp[3 * i + 2], sp[4 * i],
			sp[4 * i + 1], sp[4 * i + 2], sp[4 * i + 3]);
	}

#ifdef __SUNPRO_C
#pragma unroll(1)
#endif /* __SUNPRO_C */
	for (i *= 4; i < n; i++) {
		SPLIT_S32_3_U8(sp[i], rgb[3 * i], rgb[3 * i + 1],
			rgb[3 * i + 2]);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
