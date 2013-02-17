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

#pragma ident	"@(#)mlib_c_VideoClrCMYK2JFIFYCCK444.c	9.2	07/10/09 SMI"

/*
 * FUNCTION
 *      mlib_VideoColorCMYK2JFIFYCCK444 - color conversion from CMYK to JFIF
 *                                        YCrCbK space
 *
 * SYNOPSIS
 *      mlib_status mlib_VideoColorCMYK2JFIFYCCK444(mlib_u8       *y,
 *                                                  mlib_u8       *cb,
 *                                                  mlib_u8       *cr,
 *                                                  mlib_u8       *k,
 *                                                  const mlib_u8 *cmyk,
 *                                                  mlib_s32      n)
 *
 * ARGUMENTS
 *      y         Pointer to Y component row, 8-byte aligned
 *      cb        Pointer to Cb component row, 8-byte aligned
 *      cr        Pointer to Cr component row, 8-byte aligned
 *      k         Pointer to K component row, 8-byte aligned
 *      cmyk      Pointer to CMYK multicomponent row, 8-byte aligned
 *      n         Length of y, cb, cr, k arrays
 *                Note: length of cmyk array must be 4*n
 *
 * DESCRIPTION
 *      mlib_VideoColorCMYK2JFIFYCCK444
 *        R = (255 - C),  G = (255 - M),  B = (255 - Y)
 *
 *        Y  =  0.29900 * R + 0.58700 * G + 0.11400 * B
 *        Cb = -0.16874 * R - 0.33126 * G + 0.50000 * B  + 128
 *        Cr =  0.50000 * R - 0.41869 * G - 0.08131 * B  + 128
 *
 *        Y = y[i], Cb = cb[i], Cr = cr[i]
 *        C = cmyk[4*i], M = cmyk[4*i+1], Y = cmyk[4*i+2]
 *
 *        K = k[i] = cmyk[4*i+3] - should be passed unchanged
 *
 *        0 <= i < n
 */

#include <mlib_video.h>
#include <mlib_c_VideoColorJFIFYCC.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_VideoColorCMYK2JFIFYCCK444 = \
	__mlib_VideoColorCMYK2JFIFYCCK444

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_VideoColorCMYK2JFIFYCCK444) mlib_VideoColorCMYK2JFIFYCCK444
	__attribute__((weak, alias("__mlib_VideoColorCMYK2JFIFYCCK444")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#ifdef _LITTLE_ENDIAN

#define	SPLIT_S32_3_U8(x, a1, a2, a3)                           \
	a3 = ((x) >> 16) & 0xFF;                                \
	a2 = ((x) >> 8) & 0xFF;                                 \
	a1 = (x) & 0xFF

#define	GET_S32_LAST_U8(x)	((x) >> 24)

#else /* _LITTLE_ENDIAN */

#define	SPLIT_S32_3_U8(x, a1, a2, a3)                           \
	a1 = ((x) >> 24) & 0xFF;                                \
	a2 = ((x) >> 16) & 0xFF;                                \
	a3 = ((x) >> 8) & 0xFF

#define	GET_S32_LAST_U8(x)	(x)

#endif /* _LITTLE_ENDIAN */

/* *********************************************************** */

mlib_status
__mlib_VideoColorCMYK2JFIFYCCK444(
	mlib_u8 *y,
	mlib_u8 *cb,
	mlib_u8 *cr,
	mlib_u8 *k,
	const mlib_u8 *cmyk,
	mlib_s32 n)
{
	mlib_u32 *pcmyk = (mlib_u32 *)cmyk;
	mlib_u32 cmyk0, rgb0, r0, g0, b0, s0;
	mlib_s32 i;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < n; i++) {
		cmyk0 = (*pcmyk++);
		(*k++) = GET_S32_LAST_U8(cmyk0);

		rgb0 = ~cmyk0;
		SPLIT_S32_3_U8(rgb0, r0, g0, b0);

		s0 = mlib_t_r2j[r0] + mlib_t_g2j[g0] + mlib_t_b2j[b0];

		(*y++) = s0;
		(*cb++) = s0 >> 10;
		(*cr++) = s0 >> 21;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
