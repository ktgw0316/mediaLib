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

#pragma ident	"@(#)mlib_c_VideoJFIFYCC2RGB444_S16.c	9.2	07/10/09 SMI"

/*
 * FUNCTION
 *      mlib_VideoColorJFIFYCC2RGB444_S16 - color conversion from JFIF YCbCr
 *                                          12-bit space to RGB 12-bit space
 *
 * SYNOPSIS
 *      mlib_status mlib_VideoColorJFIFYCC2RGB444_S16(mlib_s16       *rgb,
 *                                                    const mlib_s16 *y,
 *                                                    const mlib_s16 *cb,
 *                                                    const mlib_s16 *cr,
 *                                                    mlib_s32       n)
 *
 * ARGUMENTS
 *      rgb       Pointer to RGB multicomponent row, 8-byte aligned
 *      y         Pointer to Y component row, 8-byte aligned
 *      cb        Pointer to Cb component row, 8-byte aligned
 *      cr        Pointer to Cr component row, 8-byte aligned
 *      n         Length of y, cb, cr arrays
 *                Note: length of rgb array must be 3*n
 *
 * DESCRIPTION
 *
 *        R  = Y                         + 1.40200 * (Cr - 2048)
 *        G  = Y - 0.34414 * (Cb - 2048) - 0.71414 * (Cr - 2048)
 *        B  = Y + 1.77200 * (Cb - 2048)
 *
 *        Y  = y[i], Cr = cr[i], Cb = cb[i]
 *        rgb[3*i] = R, rgb[3*i+1] = G, rgb[3*i+2] = B, 0 <= i < n
 *
 * RESTRICTION
 *
 *        YCbCr component values must be in [0, 4095] range.
 */

#include <mlib_video.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_VideoColorJFIFYCC2RGB444_S16 = \
	__mlib_VideoColorJFIFYCC2RGB444_S16

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_VideoColorJFIFYCC2RGB444_S16)
	mlib_VideoColorJFIFYCC2RGB444_S16
	__attribute__((weak, alias("__mlib_VideoColorJFIFYCC2RGB444_S16")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	SCALE	(mlib_f32)0x100000
#define	SAT	(mlib_f32)0x7FF80000
#define	MLIB_U12_MAX	((1 << 12) - 1)
#define	MLIB_U12_MIN	0

#ifdef MLIB_USE_FTOI_CLAMPING

#define	CLAMP_U12(src)	(((mlib_s32)src >> 20) + 0x800)

#else /* MLIB_USE_FTOI_CLAMPING */

#define	CLAMP_U12(src)                                                    \
	(((mlib_d64)src >=                                                \
		MLIB_S32_MAX) ? MLIB_U12_MAX : (((mlib_d64)src <          \
		MLIB_S32_MIN) ? MLIB_U12_MIN : (((mlib_s32)src >> 20) +   \
		0x800)))
/* (mlib_d64) - krakatoa bug */

#endif /* MLIB_USE_FTOI_CLAMPING */

/* *********************************************************** */

mlib_status
__mlib_VideoColorJFIFYCC2RGB444_S16(
	mlib_s16 *rgb,
	const mlib_s16 *y,
	const mlib_s16 *cb,
	const mlib_s16 *cr,
	mlib_s32 n)
{
	mlib_d64 fr, fg, fb, fy, fcb, fcr;
	mlib_s32 i;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < n; i++) {
		fy = y[i] * SCALE - SAT;
		fcb = (mlib_d64)((cb[i] - 2048) << 20);
		fcr = (mlib_d64)((cr[i] - 2048) << 20);
		fr = fy + 1.40200f * fcr;
		fg = fy - 0.34414f * fcb - 0.71414f * fcr;
		fb = fy + 1.77200f * fcb;
		rgb[3 * i] = CLAMP_U12(fr);
		rgb[3 * i + 1] = CLAMP_U12(fg);
		rgb[3 * i + 2] = CLAMP_U12(fb);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
