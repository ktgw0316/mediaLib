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
 * Copyright 2006 Sun Microsystems, Inc.  All rights reserved.
 * Use is subject to license terms.
 */

#pragma ident	"@(#)mlib_c_VideoRGB2JFIFYCC444_S16.c	9.3	07/10/09 SMI"

/*
 * FUNCTION
 *      mlib_VideoColorRGB2JFIFYCC444_S16 - color conversion from RGB 12-bit
 *                                          space to JFIF YCbCr 12-bit space
 *
 * SYNOPSIS
 *      mlib_status mlib_VideoColorRGB2JFIFYCC444_S16(mlib_s16       *y,
 *                                                    mlib_s16       *cb,
 *                                                    mlib_s16       *cr,
 *                                                    const mlib_s16 *rgb,
 *                                                    mlib_s32       n)
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
 *        Y  =  0.29900 * R + 0.58700 * G + 0.11400 * B
 *        Cb = -0.16874 * R - 0.33126 * G + 0.50000 * B  + 2048
 *        Cr =  0.50000 * R - 0.41869 * G - 0.08131 * B  + 2048
 *
 *        R  = rgb[3*i], G = rgb[3*i+1], B = rgb[3*i+2]
 *        y[i] = Y, cr[i] = Cr, cb[i] = Cb, 0 <= i < n
 *
 * RESTRICTION
 *
 *        RGB component values must be in [0, 4095] range.
 *        The resulting YCbCr components are saturating to
 *        the same range.
 */

#include <mlib_video.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_VideoColorRGB2JFIFYCC444_S16 = \
	__mlib_VideoColorRGB2JFIFYCC444_S16

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_VideoColorRGB2JFIFYCC444_S16)
	mlib_VideoColorRGB2JFIFYCC444_S16
	__attribute__((weak, alias("__mlib_VideoColorRGB2JFIFYCC444_S16")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

mlib_status
__mlib_VideoColorRGB2JFIFYCC444_S16(
	mlib_s16 *y,
	mlib_s16 *cb,
	mlib_s16 *cr,
	const mlib_s16 *rgb,
	mlib_s32 n)
{
	mlib_f32 fr, fg, fb, fy, fcb, fcr;
	mlib_s32 i;

	fr = rgb[0];
	fg = rgb[1];
	fb = rgb[2];
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < n - 1; i++) {
		fy = 0.29900f * fr + 0.58700f * fg + 0.11400f * fb;
		fcb = -0.16874f * fr - 0.33126f * fg + 0.50000f * fb + 2048;
		fcr = 0.50000f * fr - 0.41869f * fg - 0.08131f * fb + 2048;

		fr = rgb[3 * i + 3];
		fg = rgb[3 * i + 4];
		fb = rgb[3 * i + 5];
		y[i] = (mlib_s16)fy;
		cb[i] = (mlib_s16)fcb;
		cr[i] = (mlib_s16)fcr;
	}

	fy = 0.29900f * fr + 0.58700f * fg + 0.11400f * fb;
	fcb = -0.16874f * fr - 0.33126f * fg + 0.50000f * fb + 2048;
	fcr = 0.50000f * fr - 0.41869f * fg - 0.08131f * fb + 2048;

	y[i] = (mlib_s16)fy;
	cb[i] = (mlib_s16)fcb;
	cr[i] = (mlib_s16)fcr;

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
