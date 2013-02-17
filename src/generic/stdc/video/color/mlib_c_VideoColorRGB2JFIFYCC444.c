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

#pragma ident	"@(#)mlib_c_VideoColorRGB2JFIFYCC444.c	9.2	07/10/09 SMI"

/*
 * FUNCTION
 *      mlib_VideoColorRGB2JFIFYCC444 - color conversion from RGB to JFIF
 *                                      YCrCb space
 *
 * SYNOPSIS
 *      mlib_status mlib_VideoColorRGB2JFIFYCC444(mlib_u8       *y,
 *                                                mlib_u8       *cb,
 *                                                mlib_u8       *cr,
 *                                                const mlib_u8 *rgb,
 *                                                mlib_s32      n)
 *
 * ARGUMENTS
 *      y         Pointer to Y component row, 8-byte aligned
 *      cb        Pointer to Cb component row, 8-byte aligned
 *      cr        Pointer to Cr component row, 8-byte aligned
 *      rgb       Pointer to RGB multicomponent row, 8-byte aligned
 *      n         Length of the y, cb, cr arrays
 *                Note: Length of the rgb array must be 3*n
 *
 * DESCRIPTION
 *
 *      mlib_VideoColorRGB2JFIFYCC444
 *        Y  =  0.29900 * R + 0.58700 * G + 0.11400 * B
 *        Cb = -0.16874 * R - 0.33126 * G + 0.50000 * B  + 128
 *        Cr =  0.50000 * R - 0.41869 * G - 0.08131 * B  + 128
 *
 *        R  = rgb[3*i], G = rgb[3*i+1], B = rgb[3*i+2]
 *        y[i] = Y, cr[i] = Cr, cb[i] = Cb, 0 <= i < n
 */

#include <mlib_video.h>
#include <mlib_c_VideoColorJFIFYCC.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_VideoColorRGB2JFIFYCC444 = __mlib_VideoColorRGB2JFIFYCC444

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_VideoColorRGB2JFIFYCC444) mlib_VideoColorRGB2JFIFYCC444
	__attribute__((weak, alias("__mlib_VideoColorRGB2JFIFYCC444")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

mlib_status
__mlib_VideoColorRGB2JFIFYCC444(
	mlib_u8 *y,
	mlib_u8 *cb,
	mlib_u8 *cr,
	const mlib_u8 *rgb,
	mlib_s32 n)
{
	mlib_u8 *yend = y + n;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (; y <= (yend - 4); y += 4) {
		RGB2JFIFYCC_4(rgb, y[0], cb[0], cr[0],
			y[1], cb[1], cr[1],
			y[2], cb[2], cr[2], y[3], cb[3], cr[3]);
		cb += 4;
		cr += 4;
		rgb += 12;
	}

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (; y < yend; y++) {
		RGB2JFIFYCC_1(rgb, y[0], cb[0], cr[0]);

		cb++;
		cr++;
		rgb += 3;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
