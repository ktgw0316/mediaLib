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

#pragma ident	"@(#)mlib_ImageThresh_Fp_Inp.c	9.2	07/10/09 SMI"

/*
 * FUNCTION
 *      mlib_ImageThresh1_Fp_Inp - thresholding
 *      mlib_ImageThresh2_Fp_Inp - thresholding
 *      mlib_ImageThresh3_Fp_Inp - thresholding
 *      mlib_ImageThresh4_Fp_Inp - thresholding
 *      mlib_ImageThresh5_Fp_Inp - thresholding
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageThresh1_Fp_Inp(mlib_image     *srcdst,
 *                                           const mlib_d64 *thresh,
 *                                           const mlib_d64 *ghigh,
 *                                           const mlib_d64 *glow);
 *
 *      mlib_status mlib_ImageThresh2_Fp_Inp(mlib_image     *srcdst,
 *                                           const mlib_d64 *thresh,
 *                                           const mlib_d64 *glow);
 *
 *      mlib_status mlib_ImageThresh3_Fp_Inp(mlib_image     *srcdst,
 *                                           const mlib_d64 *thresh,
 *                                           const mlib_d64 *ghigh);
 *
 *      mlib_status mlib_ImageThresh4_Fp_Inp(mlib_image     *srcdst,
 *                                           const mlib_d64 *thigh,
 *                                           const mlib_d64 *tlow,
 *                                           const mlib_d64 *ghigh,
 *                                           const mlib_d64 *glow);
 *
 *      mlib_status mlib_ImageThresh5_Fp_Inp(mlib_image     *srcdst,
 *                                           const mlib_d64 *thigh,
 *                                           const mlib_d64 *tlow,
 *                                           const mlib_d64 *gmid);
 *
 * ARGUMENT
 *      srcdst  pointer to input and output image
 *      thresh  array of thresholds
 *      thigh   array of high thresholds
 *      tlow    array of low thresholds
 *      ghigh   array of values above thresholds
 *      glow    array of values below thresholds
 *      gmid    array of values between thresholds
 *
 * RESTRICTION
 *      The image can have 1, 2, 3, or 4 channels.
 *      The image can be in MLIB_FLOAT or MLIB_DOUBLE data type.
 *
 * DESCRIPTION
 *
 *    mlib_ImageThresh1_Fp_Inp:
 *      If the pixel band value is above the threshold for that channel,
 *      set the destination to the ghigh value for that channel.
 *      Otherwise, set the destination to the glow value for that channel.
 *
 *                         +- glow[c]   srcdst[x][y][c] <= thresh[c]
 *      srcdst[x][y][c]  = |
 *                         +- ghigh[c]  srcdst[x][y][c] >  thresh[c]
 *
 *    mlib_ImageThresh2_Fp_Inp:
 *      If the pixel value is below the threshold for that channel,
 *      set the destination to the glow value for that channel.
 *
 *                         +- glow[c]          srcdst[x][y][c] <= thresh[c]
 *      srcdst[x][y][c]  = |
 *                         +- srcdst[x][y][c]  srcdst[x][y][c] >  thresh[c]
 *
 *    mlib_ImageThresh3_Fp_Inp:
 *      If the pixel value is above the threshold for that channel,
 *      set the destination to the ghigh value for that channel.
 *
 *                         +- srcdst[x][y][c]  srcdst[x][y][c] <= thresh[c]
 *      srcdst[x][y][c]  = |
 *                         +- ghigh[c]         srcdst[x][y][c] >  thresh[c]
 *
 *    mlib_ImageThresh4_Fp_Inp:
 *      If the pixel channel value is above the thigh for that channel,
 *      set the destination to the ghigh value for that channel.
 *      If the pixel channel value is below the tlow for that channel,
 *      set the destination to the glow value for that channel.
 *
 *                      +-glow[c]                    srcdst[x][y][c] < tlow[c]
 *                      |
 *    srcdst[x][y][c] = +-srcdst[x][y][c] tlow[c] <= srcdst[x][y][c] <= thigh[c]
 *                      |
 *                      +-ghigh[c]                   srcdst[x][y][c] > thigh[c]
 *
 *    mlib_ImageThresh5_Fp_Inp:
 *      If the pixel channel value is below the thigh for that channel
 *      and the pixel channel value is above the tlow for that channel,
 *      set the destination to the gmid value for that channel.
 *
 *                      +-srcdst[x][y][c]            srcdst[x][y][c] <  tlow[c]
 *                      |
 *    srcdst[x][y][c] = +-gmid[c]         tlow[c] <= srcdst[x][y][c] <= thigh[c]
 *                      |
 *                      +-srcdst[x][y][c]            srcdst[x][y][c] > thigh[c]
 */

#include <mlib_image.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageThresh1_Fp_Inp = __mlib_ImageThresh1_Fp_Inp
#pragma weak mlib_ImageThresh2_Fp_Inp = __mlib_ImageThresh2_Fp_Inp
#pragma weak mlib_ImageThresh3_Fp_Inp = __mlib_ImageThresh3_Fp_Inp
#pragma weak mlib_ImageThresh4_Fp_Inp = __mlib_ImageThresh4_Fp_Inp
#pragma weak mlib_ImageThresh5_Fp_Inp = __mlib_ImageThresh5_Fp_Inp

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_ImageThresh1_Fp_Inp) mlib_ImageThresh1_Fp_Inp
    __attribute__((weak, alias("__mlib_ImageThresh1_Fp_Inp")));
__typeof__(__mlib_ImageThresh2_Fp_Inp) mlib_ImageThresh2_Fp_Inp
    __attribute__((weak, alias("__mlib_ImageThresh2_Fp_Inp")));
__typeof__(__mlib_ImageThresh3_Fp_Inp) mlib_ImageThresh3_Fp_Inp
    __attribute__((weak, alias("__mlib_ImageThresh3_Fp_Inp")));
__typeof__(__mlib_ImageThresh4_Fp_Inp) mlib_ImageThresh4_Fp_Inp
    __attribute__((weak, alias("__mlib_ImageThresh4_Fp_Inp")));
__typeof__(__mlib_ImageThresh5_Fp_Inp) mlib_ImageThresh5_Fp_Inp
    __attribute__((weak, alias("__mlib_ImageThresh5_Fp_Inp")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

mlib_status
__mlib_ImageThresh1_Fp_Inp(
    mlib_image *srcdst,
    const mlib_d64 *thresh,
    const mlib_d64 *ghigh,
    const mlib_d64 *glow)
{
	return (__mlib_ImageThresh1_Fp(srcdst, srcdst, thresh, ghigh, glow));
}

/* *********************************************************** */

mlib_status
__mlib_ImageThresh2_Fp_Inp(
    mlib_image *srcdst,
    const mlib_d64 *thresh,
    const mlib_d64 *glow)
{
	return (__mlib_ImageThresh2_Fp(srcdst, srcdst, thresh, glow));
}

/* *********************************************************** */

mlib_status
__mlib_ImageThresh3_Fp_Inp(
    mlib_image *srcdst,
    const mlib_d64 *thresh,
    const mlib_d64 *ghigh)
{
	return (__mlib_ImageThresh3_Fp(srcdst, srcdst, thresh, ghigh));
}

/* *********************************************************** */

mlib_status
__mlib_ImageThresh4_Fp_Inp(
    mlib_image *srcdst,
    const mlib_d64 *thigh,
    const mlib_d64 *tlow,
    const mlib_d64 *ghigh,
    const mlib_d64 *glow)
{
	return (__mlib_ImageThresh4_Fp
			(srcdst, srcdst, thigh, tlow, ghigh, glow));
}

/* *********************************************************** */

mlib_status
__mlib_ImageThresh5_Fp_Inp(
    mlib_image *srcdst,
    const mlib_d64 *thigh,
    const mlib_d64 *tlow,
    const mlib_d64 *gmid)
{
	return (__mlib_ImageThresh5_Fp(srcdst, srcdst, thigh, tlow, gmid));
}

/* *********************************************************** */
