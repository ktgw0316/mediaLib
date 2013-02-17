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

#pragma ident	"@(#)mlib_ImageConvKernelConvert.c	9.2	07/10/09 SMI"

/*
 * FUNCTION
 *      mlib_ImageConvKernelConvert - Convert convolution kernel from
 *                                    floating point version to integer
 *                                    version.
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageConvKernelConvert(mlib_s32       *ikernel,
 *                                              mlib_s32       *iscale,
 *                                              const mlib_d64 *fkernel,
 *                                              mlib_s32       m,
 *                                              mlib_s32       n,
 *                                              mlib_type      type);
 *
 * ARGUMENT
 *      ikernel  integer kernel
 *      iscale   scaling factor of the integer kernel
 *      fkernel  floating-point kernel
 *      m        width of the convolution kernel
 *      n        height of the convolution kernel
 *      type     image type
 *
 * DESCRIPTION
 *      Convert a floating point convolution kernel to integer kernel
 *      with scaling factor. The result integer kernel and scaling factor
 *      can be used in convolution functions directly without overflow.
 *
 * RESTRICTION
 *      The type can be MLIB_BIT, MLIB_BYTE, MLIB_SHORT, MLIB_USHORT or
 *      MLIB_INT.
 */

#include <stdlib.h>
#include <mlib_image.h>
#include <mlib_SysMath.h>
#include <mlib_ImageConv.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageConvKernelConvert = __mlib_ImageConvKernelConvert

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_ImageConvKernelConvert) mlib_ImageConvKernelConvert
    __attribute__((weak, alias("__mlib_ImageConvKernelConvert")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#ifdef __sparc

/* *********************************************************** */

#define	CLAMP_S32(dst, src)	dst = (mlib_s32)(src)

#else /* __sparc */

/* *********************************************************** */

#define	CLAMP_S32(dst, src)                                     \
	{                                                       \
	    mlib_d64 s0 = (mlib_d64)(src);                      \
	                                                        \
	    if (s0 > (mlib_d64)MLIB_S32_MAX)                    \
		s0 = (mlib_d64)MLIB_S32_MAX;                    \
	    if (s0 < (mlib_d64)MLIB_S32_MIN)                    \
		s0 = (mlib_d64)MLIB_S32_MIN;                    \
	    dst = (mlib_s32)s0;                                 \
	}

#endif /* __sparc */

/* *********************************************************** */

mlib_status
__mlib_ImageConvKernelConvert(
    mlib_s32 *ikernel,
    mlib_s32 *iscale,
    const mlib_d64 *fkernel,
    mlib_s32 m,
    mlib_s32 n,
    mlib_type type)
{
	mlib_d64 sum_pos, sum_neg, sum, norm, max, f;
	mlib_s32 isum_pos, isum_neg, isum, test;
	mlib_s32 i, scale, scale1, chk_flag;

	if (ikernel == NULL || iscale == NULL || fkernel == NULL || m < 1 ||
	    n < 1) {
		return (MLIB_FAILURE);
	}

	if ((type == MLIB_BYTE) || (type == MLIB_SHORT) ||
	    (type == MLIB_USHORT)) {

/* MLIB_BYTE, MLIB_USHORT */
		if (type != MLIB_SHORT) {
			sum_pos = 0;
			sum_neg = 0;

			for (i = 0; i < m * n; i++) {
				if (fkernel[i] > 0)
					sum_pos += fkernel[i];
				else
					sum_neg -= fkernel[i];
			}

			sum = (sum_pos > sum_neg) ? sum_pos : sum_neg;
			scale = mlib_ilogb(sum);
			scale++;

			scale = 31 - scale;
		} else {
/* MLIB_SHORT */
			sum = 0;
			max = 0;

			for (i = 0; i < m * n; i++) {
				f = mlib_fabs(fkernel[i]);
				sum += f;
				max = (max > f) ? max : f;
			}

			scale1 = mlib_ilogb(max) + 1;
			scale = mlib_ilogb(sum);
			scale = (scale > scale1) ? scale : scale1;
			scale++;

			scale = 32 - scale;
		}

		if (scale <= 16)
			return (MLIB_FAILURE);

		if (scale > 31)
			scale = 31;

		*iscale = scale;

		chk_flag = mlib_ImageConvVersion(m, n, scale, type);

		if (!chk_flag) {
			norm = (1u << scale);
			for (i = 0; i < m * n; i++) {
				CLAMP_S32(ikernel[i], fkernel[i] * norm);
			}

			return (MLIB_SUCCESS);
		}

/* try to round coefficients */
#ifdef __sparc
/* shift of coefficients is 16 */
		scale1 = 16;
#else /* __sparc */

		if (chk_flag == 3)
/* MMX */
			scale1 = 16;
		else
			scale1 = (type == MLIB_BYTE) ? 8 : 16;
#endif /* __sparc */
		norm = (1u << (scale - scale1));

		for (i = 0; i < m * n; i++) {
			if (fkernel[i] > 0)
				ikernel[i] =
				    (mlib_s32)(fkernel[i] * norm + 0.5);
			else
				ikernel[i] =
				    (mlib_s32)(fkernel[i] * norm - 0.5);
		}

		isum_pos = 0;
		isum_neg = 0;
		test = 0;

		for (i = 0; i < m * n; i++) {
			if (ikernel[i] > 0)
				isum_pos += ikernel[i];
			else
				isum_neg -= ikernel[i];
		}

		if (type == MLIB_BYTE || type == MLIB_USHORT) {
			isum = (isum_pos > isum_neg) ? isum_pos : isum_neg;

			if (isum >= (1 << (31 - scale1)))
				test = 1;
		} else {
			isum = isum_pos + isum_neg;

			if (isum >= (1 << (32 - scale1)))
				test = 1;
			for (i = 0; i < m * n; i++) {
				if (abs(ikernel[i]) >= (1 << (31 - scale1)))
					test = 1;
			}
		}

/* rounding according scale1 cause overflow, truncate instead of round */
		if (test == 1) {
			for (i = 0; i < m * n; i++)
				ikernel[i] =
				    (mlib_s32)(fkernel[i] * norm) << scale1;
		} else {
/* rounding is Ok */
			for (i = 0; i < m * n; i++)
				ikernel[i] = ikernel[i] << scale1;
		}

		return (MLIB_SUCCESS);
	} else if ((type == MLIB_INT) || (type == MLIB_BIT)) {
		max = 0;

		for (i = 0; i < m * n; i++) {
			f = mlib_fabs(fkernel[i]);
			max = (max > f) ? max : f;
		}

		scale = mlib_ilogb(max);

		if (scale > 29)
			return (MLIB_FAILURE);

		if (scale < -100)
			scale = -100;

		*iscale = 29 - scale;
		scale = 29 - scale;

		norm = 1.0;
		while (scale > 30) {
			norm *= (1 << 30);
			scale -= 30;
		}

		norm *= (1 << scale);

		for (i = 0; i < m * n; i++) {
			if (fkernel[i] > 0) {
				CLAMP_S32(ikernel[i], fkernel[i] * norm + 0.5);
			} else {
				CLAMP_S32(ikernel[i], fkernel[i] * norm - 0.5);
			}
		}

		return (MLIB_SUCCESS);
	} else {
		return (MLIB_FAILURE);
	}
}

/* *********************************************************** */
