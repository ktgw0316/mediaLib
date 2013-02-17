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

#pragma ident	"@(#)mlib_c_VideoClrJFIFYCCK2CMYK444.c	9.2	07/10/09 SMI"

/*
 * FUNCTION
 *      mlib_VideoColorJFIFYCCK2CMYK444 - color conversion from JFIF YCbCrK
 *                                        to CMYK space
 *
 * SYNOPSIS
 *      mlib_status mlib_VideoColorJFIFYCCK2CMYK444(mlib_u8       *cmyk,
 *                                                  const mlib_u8 *y,
 *                                                  const mlib_u8 *cb,
 *                                                  const mlib_u8 *cr,
 *                                                  const mlib_u8 *k,
 *                                                  mlib_s32      n)
 *
 * ARGUMENTS
 *      cmyk      Pointer to CMYK multicomponent row, 8-byte aligned
 *      y         Pointer to Y component row, 8-byte aligned
 *      cb        Pointer to Cb component row, 8-byte aligned
 *      cr        Pointer to Cr component row, 8-byte aligned
 *      k         Pointer to K component row, 8-byte aligned
 *      n         Length of y, cb, cr, k arrays
 *                Note: length of cmyk array must be 4*n
 *
 * DESCRIPTION
 *      mlib_VideoColorJFIFYCCK2CMYK444
 *        R  = Y                        + 1.40200 * (Cr - 128)
 *        G  = Y - 0.34414 * (Cb - 128) - 0.71414 * (Cr - 128)
 *        B  = Y + 1.77200 * (Cb - 128)
 *
 *        and CMYK: C = (255 - R),  M = (255 - G),  Y = (255 - B)
 *
 *        Y = y[i], Cb = cb[i], Cr = cr[i]
 *        C = cmyk[4*i], M = cmyk[4*i+1], Y = cmyk[4*i+2]
 *
 *        K = cmyk[4*i+3] = k[i] - should be passed unchanged
 *
 *        0 <= i < n
 */

#include <mlib_video.h>
#include <mlib_c_VideoColorJFIFYCC.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_VideoColorJFIFYCCK2CMYK444 = \
	__mlib_VideoColorJFIFYCCK2CMYK444

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_VideoColorJFIFYCCK2CMYK444) mlib_VideoColorJFIFYCCK2CMYK444
	__attribute__((weak, alias("__mlib_VideoColorJFIFYCCK2CMYK444")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	TCLAMP_U8(val, dst)	dst = pClip[val]

/* *********************************************************** */

#define	SCLAMP_U8(s, dst)                                       \
	{                                                       \
	    mlib_s32 v = s, mask = (v - 0x1fe) >> 31;           \
	                                                        \
	    dst = (((mlib_u32)v >> 1) | ~mask) & ~(v >> 31);    \
	}

/* *********************************************************** */

mlib_status
__mlib_VideoColorJFIFYCCK2CMYK444(
	mlib_u8 *cmyk,
	const mlib_u8 *y,
	const mlib_u8 *cb,
	const mlib_u8 *cr,
	const mlib_u8 *k,
	mlib_s32 n)
{
	mlib_s32 BBi = 454;
	mlib_s32 RRi = 359;
	mlib_u8 *const pClip = (mlib_u8 *const)mlib_tClip + 288 * 2;
	mlib_s32 i;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < n; i++) {
		mlib_s32 y0, cb0, cr0, sr, sg, sb;

		cb0 = (*cb++);
		cr0 = (*cr++);

		y0 = (255 - (*y++)) * 2;

		sr = mlib_tRCr[cr0] - RRi;
		sg = mlib_tGCb[cb0] - mlib_tGCr[cr0];
		sb = mlib_tBCb[cb0] - BBi;

		cmyk[3] = (*k++);
		SCLAMP_U8(y0 - sr, cmyk[0]);
		TCLAMP_U8(y0 - sg, cmyk[1]);
		TCLAMP_U8(y0 - sb, cmyk[2]);

		cmyk += 4;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
