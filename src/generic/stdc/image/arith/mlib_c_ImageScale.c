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

#pragma ident	"@(#)mlib_c_ImageScale.c	9.2	07/10/09 SMI"

/*
 * FUNCTION
 *      mlib_ImageScale - linear scaling
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageScale(mlib_image       *dst,
 *                                  const mlib_image *src,
 *                                  const mlib_s32   *alpha,
 *                                  const mlib_s32   *beta,
 *                                  mlib_s32         shift);
 *
 * ARGUMENT
 *      dst     pointer to the output image
 *      src     pointer to the input image
 *      alpha   array of scaling factors to be multiplied to each channel
 *              of the pixels
 *      beta    array of offsets to be added to each channel of the pixels
 *      shift   right shift factor
 *
 * VALID RANGES
 *   some designations:  maxA  - maximum of absolute values of alpha[i]
 *                       maxB  - maximum of absolute values of beta[i]
 *                       2^x   - two power x
 *                       STYPE - source image type
 *                       DTYPE - destination image type
 *                       SHIFT - shift argument
 *                       RESTRICTIONS - restrictions to
 *                       maxA, maxB and shift for given types and shift
 *
 *  STYPE  DTYPE        SHIFT                 RESTRICTIONS
 *   U8     U8        [ 0, 31]    maxA < 2^(shift+7)
 *                                2^8  * maxA * 2^(-shift) + maxB + 0.5 < 2^23
 *   U8     S16       [ 0,  0]    maxA < 2^(shift+14)
 *                                2^8  * maxA * 2^(-shift) + maxB + 0.5 < 2^30
 *   U8     S16       [ 1,  8]    maxA < 2^15
 *                                2^8  * maxA * 2^(-shift) + maxB + 0.5 <
 *                                2^(31-shift)
 *   U8     S16       [ 9, 31]    maxA < 2^(shift+7)
 *                                2^8  * maxA * 2^(-shift) + maxB + 0.5 < 2^23
 *   U8     U16       [ 0,  0]    maxA < 2^(shift+14)
 *                                2^8  * maxA * 2^(-shift) +
 *                                max(B - 32768) + 0.5 < 2^30
 *   U8     U16       [ 1,  8]    maxA < 2^15
 *                                2^8  * maxA * 2^(-shift) +
 *                                max(B - 32768) + 0.5 < 2^(31-shift)
 *   U8     U16       [ 9, 31]    maxA < 2^(shift+7)
 *                                2^8  * maxA * 2^(-shift) + max(B - 32768) +
 *                                0.5 < 2^23
 *   U8     S32       [ 0, 31]    no restrictions
 *   U8     F32, D64  [ 0, 31]    no restrictions
 *   S16    U8        [ 0,  7]    maxA < 2^(shift+7)
 *                                2^15 * maxA * 2^(-shift) + maxB + 0.5 < 2^23
 *   S16    U8        [ 8, 16]    maxA < 2^15
 *                                2^15 * maxA * 2^(-shift) + maxB + 0.5 <
 *                                2^(31-shift)
 *   S16    U8        [17, 31]    maxA < 2^(shift-1)
 *                                2^15 * maxA * 2^(-shift) + maxB + 0.5 < 2^15
 *   S16    S16       [ 0,  0]    maxA < 2^(shift+14)
 *                                2^15 * maxA * 2^(-shift) + maxB + 0.5 < 2^30
 *   S16    S16       [ 1, 16]    maxA < 2^15
 *                                2^15 * maxA * 2^(-shift) + maxB + 0.5 <
 *                                2^(31-shift)
 *   S16    S16       [17, 31]    maxA < 2^(shift-1)
 *                                2^15 * maxA * 2^(-shift) + maxB + 0.5 < 2^15
 *   S16    U16       [ 0,  0]    maxA < 2^(shift+14)
 *                                2^15 * maxA * 2^(-shift) + max(B - 32768) +
 *                                0.5 < 2^30
 *   S16    U16       [ 1, 16]    maxA < 2^15
 *                                2^15 * maxA * 2^(-shift) + max(B - 32768) +
 *                                0.5 < 2^(31-shift)
 *   S16    U16       [17, 31]    maxA < 2^(shift-1)
 *                                2^15 * maxA * 2^(-shift) + max(B - 32768) +
 *                                0.5 < 2^15
 *   S16    S32       [ 0, 16]    maxA < 2^15
 *                                2^15 * maxA * 2^(-shift) + maxB + 0.5 <
 *                                2^(31-shift)
 *   S16    S32       [17, 31]    maxA < 2^(shift-1)
 *                                2^15 * maxA * 2^(-shift) + maxB + 0.5 < 2^15
 *   S16    F32, D64  [ 0, 31]    no restrictions
 *   U16    U8        [ 0,  7]    maxA < 2^(shift+7)
 *                                2^16 * maxA * 2^(-shift) + maxB + 0.5 < 2^23
 *   U16    U8        [ 8, 16]    maxA < 2^15
 *                                2^16 * maxA * 2^(-shift) + maxB + 0.5 <
 *                                2^(31-shift)
 *   U16    U8        [17, 31]    maxA < 2^(shift-1)
 *                                2^16 * maxA * 2^(-shift) + maxB + 0.5 < 2^15
 *   U16    S16       [ 0,  0]    maxA < 2^(shift+14)
 *                                2^16 * maxA * 2^(-shift) + maxB + 0.5 < 2^30
 *   U16    S16       [ 1, 16]    maxA < 2^15
 *                                2^16 * maxA * 2^(-shift) + maxB + 0.5 <
 *                                2^(31-shift)
 *   U16    S16       [17, 31]    maxA < 2^(shift-1)
 *                                2^16 * maxA * 2^(-shift) + maxB + 0.5 < 2^15
 *   U16    U16       [ 0,  0]    maxA < 2^(shift+14)
 *                                2^16 * maxA * 2^(-shift) + max(B - 32768) +
 *                                0.5 < 2^30
 *   U16    U16       [ 1, 16]    maxA < 2^15
 *                                2^16 * maxA * 2^(-shift) + max(B - 32768) +
 *                                0.5 < 2^(31-shift)
 *   U16    U16       [17, 31]    maxA < 2^(shift-1)
 *                                2^16 * maxA * 2^(-shift) + max(B - 32768) +
 *                                0.5 < 2^15
 *   U16    S32       [ 0, 16]    maxA < 2^15
 *                                2^16 * maxA * 2^(-shift) + maxB + 0.5 <
 *                                2^(31-shift)
 *   U16    S32       [17, 31]    maxA < 2^(shift-1)
 *                                2^16 * maxA * 2^(-shift) + maxB + 0.5 < 2^15
 *   U16    F32, D64  [ 0, 31]    no restrictions
 *   S32    U8, S16   [ 0, 31]    maxA < 2^(shift-17)
 *                                2^31 * maxA * 2^(-shift) + maxB + 0.5 <
 *                                2^(47-shift)
 *   S32    U8, S16   [ 0, 31]    no restrictions, but may be more
 *                                slow than with restrictions
 *   S32    U16       [ 0, 31]    maxA < 2^(shift-17)
 *                                2^31 * maxA * 2^(-shift) + max(B - 32768) +
 *                                0.5 < 2^(47-shift)
 *   S32    U16       [ 0, 31]    no restrictions, but may be more
 *                                slow than with restrictions
 *   S32    S32       [ 0, 31]    no restrictions
 *   S32    F32, D64  [ 0, 31]    no restrictions
 *
 *   If one of RESTRICTIONS is failed
 *   then mlib_ImageScale returns MLIB_OUTOFRANGE
 *
 * RESTRICTION
 *      The images must have the same size, and the same number
 *      of channels.
 *      The images can have 1, 2, 3, or 4 channels.
 *      The input image can be in MLIB_BYTE, MLIB_SHORT,
 *      MLIB_USHORT or MLIB_INT data type.
 *      The output image can be in MLIB_BYTE,
 *      MLIB_SHORT, MLIB_USHORT, MLIB_INT,
 *      MLIB_FLOAT or MLIB_DOUBLE data type.
 *
 * DESCRIPTION
 *      dst[x][y][c] = src[x][y][c] * alpha[c] * 2^(-shift) + beta[c]
 *
 *      If the result of the operation underflows/overflows the
 *      minimum/maximum value supported by the destination image, then it
 *      will be clamped to the minimum/maximum value respectively.
 */

#include <mlib_image.h>
#include <mlib_ImageCheck.h>
#include <mlib_ImageScale2.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageScale = __mlib_ImageScale

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_ImageScale) mlib_ImageScale
    __attribute__((weak, alias("__mlib_ImageScale")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

mlib_status
__mlib_ImageScale(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_s32 *alpha,
    const mlib_s32 *beta,
    mlib_s32 shift)
{
	mlib_type stype, dtype;
	mlib_s32 k, nchan;
	mlib_d64 dshift, dalpha[4], dbeta[4];

	MLIB_IMAGE_CHECK(dst);
	MLIB_IMAGE_CHECK(src);
	MLIB_IMAGE_SIZE_EQUAL(dst, src);
	MLIB_IMAGE_CHAN_EQUAL(dst, src);

	if (alpha == NULL)
		return (MLIB_FAILURE);
	if (beta == NULL)
		return (MLIB_FAILURE);
	if (shift < 0 || shift > 31)
		return (MLIB_OUTOFRANGE);

	stype = mlib_ImageGetType(src);
	dtype = mlib_ImageGetType(dst);
	nchan = mlib_ImageGetChannels(dst);

	if (mlib_ImageScale_CheckForVIS(alpha, beta, shift, stype, dtype,
	    nchan) < -1) {
		return (MLIB_OUTOFRANGE);
	}

	dshift = 1.0 / (1u << shift);
	for (k = 0; k < nchan; k++) {
		dalpha[k] = alpha[k] * dshift;
		dbeta[k] = beta[k];
	}

	if (stype == MLIB_BYTE)
		return (mlib_ImageScale2_U8(dst, src, dalpha, dbeta));
	if (stype == MLIB_SHORT)
		return (mlib_ImageScale2_S16(dst, src, dalpha, dbeta));
	if (stype == MLIB_USHORT)
		return (mlib_ImageScale2_U16(dst, src, dalpha, dbeta));
	if (stype == MLIB_INT)
		return (mlib_ImageScale2_S32(dst, src, dalpha, dbeta));

	return (MLIB_FAILURE);
}

/* *********************************************************** */
