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

#pragma ident	"@(#)mlib_c_VideoColorARGB2JFIFYCC444.c	9.2	07/10/09 SMI"

/*
 * FUNCTION
 *      mlib_VideoColorARGB2JFIFYCC444 - color conversion from ARGB to JFIF
 *                                       YCrCb space
 *
 * SYNOPSIS
 *      mlib_status mlib_VideoColorARGB2JFIFYCC444(mlib_u8       *y,
 *                                                 mlib_u8       *cb,
 *                                                 mlib_u8       *cr,
 *                                                 const mlib_u8 *argb,
 *                                                 mlib_s32      n)
 *
 * ARGUMENTS
 *      y         Pointer to Y component row, 8-byte aligned
 *      cb        Pointer to Cb component row, 8-byte aligned
 *      cr        Pointer to Cr component row, 8-byte aligned
 *      argb      Pointer to ARGB multicomponent row, 8-byte aligned
 *      n         Length of the y, cb, cr arrays
 *                Note: Length of the argb array must be 4*n
 *
 * DESCRIPTION
 *
 *      mlib_VideoColorARGB2JFIFYCC444
 *        Y  =  0.29900 * R + 0.58700 * G + 0.11400 * B
 *        Cb = -0.16874 * R - 0.33126 * G + 0.50000 * B  + 128
 *        Cr =  0.50000 * R - 0.41869 * G - 0.08131 * B  + 128
 *
 *        R  = argb[4*i+1], G = argb[4*i+2], B = argb[4*i+3]
 *        y[i] = Y, cr[i] = Cr, cb[i] = Cb
 *
 *        0 <= i < n
 */

#include <mlib_video.h>
#include <mlib_c_VideoColorJFIFYCC.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_VideoColorARGB2JFIFYCC444 = \
	__mlib_VideoColorARGB2JFIFYCC444

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_VideoColorARGB2JFIFYCC444) mlib_VideoColorARGB2JFIFYCC444
	__attribute__((weak, alias("__mlib_VideoColorARGB2JFIFYCC444")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#ifdef _LITTLE_ENDIAN

#define	SPLIT_S32_3_U8(x, a1, a2, a3)                           \
	a3 = (x) >> 24;                                         \
	a2 = ((x) >> 16) & 0xFF;                                \
	a1 = ((x) >> 8) & 0xFF

#else /* _LITTLE_ENDIAN */

#define	SPLIT_S32_3_U8(x, a1, a2, a3)                           \
	a1 = ((x) >> 16) & 0xFF;                                \
	a2 = ((x) >> 8) & 0xFF;                                 \
	a3 = (x) & 0xFF

#endif /* _LITTLE_ENDIAN */

/* *********************************************************** */

mlib_status
__mlib_VideoColorARGB2JFIFYCC444(
	mlib_u8 *y,
	mlib_u8 *cb,
	mlib_u8 *cr,
	const mlib_u8 *argb,
	mlib_s32 n)
{
	mlib_u32 *pargb = (mlib_u32 *)argb;
	mlib_u32 argb0, r0, g0, b0, s0;
	mlib_s32 i;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < n; i++) {
		argb0 = (*pargb++);

		SPLIT_S32_3_U8(argb0, r0, g0, b0);

		s0 = mlib_t_r2j[r0] + mlib_t_g2j[g0] + mlib_t_b2j[b0];

		(*y++) = s0;
		(*cb++) = s0 >> 10;
		(*cr++) = s0 >> 21;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
