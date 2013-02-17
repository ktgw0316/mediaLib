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

#pragma ident	"@(#)mlib_ImageSConvKernelConvert.c	9.2	07/10/09 SMI"

/*
 * FUNCTION
 *      mlib_ImageSConvKernelConvert - Convert separable convolution kernel
 *                                     from floating point version to integer
 *                                     version.
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageSConvKernelConvert(mlib_s32       *ihkernel,
 *                                               mlib_s32       *ivkernel,
 *                                               mlib_s32       *iscale,
 *                                               const mlib_d64 *fhkernel,
 *                                               const mlib_d64 *fvkernel,
 *                                               mlib_s32       m,
 *                                               mlib_s32       n,
 *                                               mlib_type      type);
 *
 * ARGUMENTS
 *      ihkernel  integer horizontal kernel
 *      ivkernel  integer vertical kernel
 *      iscale    scaling factor of the integer kernel
 *      fhkernel  floating-point horizontal kernel
 *      fvkernel  floating-point vertical kernel
 *      m         width of the convolution kernel
 *      n         height of the convolution kernel
 *      type      image type
 *
 * DESCRIPTION
 *      Convert a floating point separable convolution kernel to integer
 *      kernel with scaling factor. The result integer kernel and scaling
 *      factor can be used in separable convolution functions directly
 *      without overflow.
 *
 * RESTRICTION
 *      The type can be MLIB_BYTE, MLIB_SHORT, MLIB_USHORT or MLIB_INT.
 */

#include <stdlib.h>
#include <mlib_image.h>
#include <mlib_SysMath.h>
#include <mlib_ImageVisVersion.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageSConvKernelConvert = __mlib_ImageSConvKernelConvert

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_ImageSConvKernelConvert) mlib_ImageSConvKernelConvert
    __attribute__((weak, alias("__mlib_ImageSConvKernelConvert")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#ifdef MLIB_USE_FTOI_CLAMPING

/* *********************************************************** */

#define	CLAMP_S32(dst, src)	dst = (mlib_s32)(src)

#else /* MLIB_USE_FTOI_CLAMPING */

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

#endif /* MLIB_USE_FTOI_CLAMPING */

/* *********************************************************** */

mlib_status
__mlib_ImageSConvKernelConvert(
    mlib_s32 *ihkernel,
    mlib_s32 *ivkernel,
    mlib_s32 *iscale,
    const mlib_d64 *fhkernel,
    const mlib_d64 *fvkernel,
    mlib_s32 m,
    mlib_s32 n,
    mlib_type type)
{
	mlib_d64 sum_pos, sum_neg, sumh, sumv, normh, normv, max, f;
	mlib_s32 isum;
	mlib_s32 i, scale, scale1, max_scale, min_scale, scaleh, scalev;
	mlib_s32 norm_scale_v, norm_scale_h;
	mlib_s32 isum_pos, isum_neg, test, chk_flag;
	mlib_s32 flag_unsigned = 0;

	if (ihkernel == NULL || ivkernel == NULL || iscale == NULL ||
	    fhkernel == NULL || fvkernel == NULL || m != n) {
		return (MLIB_FAILURE);
	}

/* swap H/V kernels on VIS version */
	if (mlib_ImageVisVersion()) {
		mlib_s32 *iker = ihkernel;
		const mlib_d64 *fker = fhkernel;

		ihkernel = ivkernel;
		fhkernel = fvkernel;
		ivkernel = iker;
		fvkernel = fker;
	}

	if ((type == MLIB_BYTE) || (type == MLIB_SHORT) ||
	    (type == MLIB_USHORT)) {

		sum_pos = 0;
		sum_neg = 0;

		for (i = 0; i < m; i++) {
			if (fhkernel[i] > 0)
				sum_pos += fhkernel[i];
			else
				sum_neg -= fhkernel[i];
		}

		if (type == MLIB_BYTE)
			flag_unsigned = 1;
		if (type == MLIB_USHORT && !mlib_ImageVisVersion())
			flag_unsigned = 1;

		if (flag_unsigned) {
			sumh = (sum_pos > sum_neg) ? sum_pos : sum_neg;

/* all fhkernels = 0 */
			if (sumh == 0) {
				for (i = 0; i < m; i++) {
					ivkernel[i] = ihkernel[i] = 0;
				}

/* some number > 7+16 and < 16+16 */
				*iscale = 30;
				return (MLIB_SUCCESS);
			}

/* scaling sumh to 32767 */
			normh = 32767.0 / (32768.0 * sumh);
			scaleh = mlib_ilogb(sumh * normh);

			if (type == MLIB_BYTE) {
/* -32768 < x*(2**(14 - ilogb(x)) < 32768 */
				norm_scale_h = 14;
/* min_scale and max_scale -  */
				min_scale = 8;
/* from mlib_v_ImageSConv*x*  - 16 */
				max_scale = 15;
			} else {
/* MLIB_USHORT */
				norm_scale_h = 14;
				min_scale = 10;
				max_scale = 17;
			}

		} else {
/* MLIB_SHORT - may be negative */
			sumh = sum_pos + sum_neg;

/* all fhkernels = 0 */
			if (sumh == 0) {
				for (i = 0; i < m; i++) {
					ivkernel[i] = ihkernel[i] = 0;
				}

/* some number > 9+16 and < 18+16 */
				*iscale = 30;
				return (MLIB_SUCCESS);
			}

/* scaling sumh to 32767 */
			normh = 32767.0 / (32768.0 * sumh);
			scaleh = mlib_ilogb(sumh * normh);
			max = 0;
			for (i = 0; i < m; i++) {
				max =
				    (max >
				    mlib_fabs(fhkernel[i])) ? max :
				    mlib_fabs(fhkernel[i]);
			}

			isum = mlib_ilogb(max * normh);

/* all ihkernels must be in the range -32768, 32767 */
			if (scaleh == isum)
				norm_scale_h = 14;
/* but sumv may be in the range -65576, 65575 */
			else
				norm_scale_h = 15;
/* min_scale and max_scale -  */
			min_scale = 10;
/* from mlib_v_ImageSConv*x*  - 16 */
			max_scale = 17;
		}

		sumv = 0;
		normv = 32768.0 * sumh / 32767;
		max = 0;

/* after multiplying by ihkernels */
		for (i = 0; i < m; i++) {
/* all numbers are MLIB_S16 */
			sumv += mlib_fabs(fvkernel[i]);
			max =
			    (max >
			    mlib_fabs(fvkernel[i])) ? max :
			    mlib_fabs(fvkernel[i]);
		}

		isum = mlib_ilogb(max * normv);
		scalev = mlib_ilogb(sumv * normv);

/* all ivkernels must be in the range -32768, 32767 */
		if (scalev == isum)
			norm_scale_v = 14;
/* but sumh may be in the range -65576, 65575 */
		else
			norm_scale_v = 15;

		if ((scaleh + scalev) >
		    (norm_scale_v + norm_scale_h - 2 * min_scale))
/* koeff. are so large */
			return (MLIB_FAILURE);

		scale = norm_scale_v + norm_scale_h - (scaleh + scalev);

/* scale must be even */
		if ((scale & 1) != 0) {
			scalev++;
			scale--;
		}

		scale >>= 1;

		if (scale > max_scale) {
			scaleh += (scale - max_scale);
			scalev += (scale - max_scale);
			scale = max_scale;
		}

		if (scaleh > norm_scale_h)
			normh = normh / (1 << (scaleh - norm_scale_h));
		else
			normh = normh * (1 << (norm_scale_h - scaleh));

		if (scalev > norm_scale_v)
			normv = normv / (1 << (scalev - norm_scale_v));
		else
			normv = normv * (1 << (norm_scale_v - scalev));

		*iscale = scale + 16;

#ifdef __sparc
		chk_flag = mlib_ImageVisVersion();
#else /* __sparc */
/* for integer arithmetic */
		chk_flag = 1;
#endif /* __sparc */

		if (!chk_flag) {
			normh *= (1 << 16);
			for (i = 0; i < m; i++) {
				CLAMP_S32(ihkernel[i], fhkernel[i] * normh);
			}

			normv *= (1 << 16);
			for (i = 0; i < m; i++) {
				CLAMP_S32(ivkernel[i], fvkernel[i] * normv);
			}

			return (MLIB_SUCCESS);
		}

/* try to round coefficients */
/* check H kernel */
#ifdef __sparc
/* shift of coefficients is 16 */
		scale1 = 16;
#else /* __sparc */
		scale1 = (type != MLIB_BYTE) ? 16 : 8;
#endif /* __sparc */

		normh *= (1 << (16 - scale1));

		for (i = 0; i < m; i++) {
			if (fhkernel[i] > 0.0)
				ihkernel[i] =
				    (mlib_s32)(fhkernel[i] * normh + 0.5);
			else
				ihkernel[i] =
				    (mlib_s32)(fhkernel[i] * normh - 0.5);
		}

		isum_pos = 0;
		isum_neg = 0;
		test = 0;

		for (i = 0; i < m; i++) {
			if (ihkernel[i] > 0)
				isum_pos += ihkernel[i];
			else
				isum_neg -= ihkernel[i];
		}

		if (flag_unsigned) {
			isum = (isum_pos > isum_neg) ? isum_pos : isum_neg;

			if (isum >= (1 << (31 - scale1)))
				test = 1;
		} else {
			isum = isum_pos + isum_neg;

			if (isum >= (1 << (32 - scale1)))
				test = 1;
			for (i = 0; i < m; i++) {
				if (abs(ihkernel[i]) >= (1 << (31 - scale1)))
					test = 1;
			}
		}

/* rounding according scale1 cause overflow, truncate instead of round */
		if (test == 1) {
			for (i = 0; i < m; i++)
				ihkernel[i] =
				    ((mlib_s32)(fhkernel[i] * normh)) << scale1;
		} else {
/* rounding is Ok */
			for (i = 0; i < m; i++)
				ihkernel[i] = ihkernel[i] << scale1;
		}

/* check V kernel */
		for (i = 0; i < m; i++) {
			if (fvkernel[i] > 0.0)
				ivkernel[i] =
				    (mlib_s32)(fvkernel[i] * normv + 0.5);
			else
				ivkernel[i] =
				    (mlib_s32)(fvkernel[i] * normv - 0.5);
		}

		isum = 0;
		test = 0;

		for (i = 0; i < m; i++) {
			isum += abs(ivkernel[i]);

			if (abs(ivkernel[i]) >= (1 << 15))
				test = 1;
		}

		if (isum >= (1 << 16))
			test = 1;

/* truncate */
		if (test == 1) {
			for (i = 0; i < m; i++) {
				ivkernel[i] =
				    ((mlib_s32)(fvkernel[i] * normv)) << 16;
			}

		} else {
/* round */
			for (i = 0; i < m; i++) {
				ivkernel[i] = ivkernel[i] << 16;
			}
		}

		return (MLIB_SUCCESS);

	} else if (type == MLIB_INT) {
		max = 0.0;

		for (i = 0; i < m; i++) {
			f = mlib_fabs(fvkernel[i]);
			max = (max > f) ? max : f;
		}

		if (max == 0) {
			for (i = 0; i < m; i++) {
				ihkernel[i] = ivkernel[i] = 0;
			}

			*iscale = 30;
			return (MLIB_SUCCESS);
		}

/* scaling maxv to 1073741823 (2**30 - 1) */
		normv = ((1 << 30) - 1) / ((mlib_d64)(1 << 30) * max);
		normh = ((mlib_d64)(1 << 30) * max) / ((1 << 30) - 1);

		scalev = mlib_ilogb(max * normv);

		max = 0.0;

		for (i = 0; i < m; i++) {
			f = mlib_fabs(fhkernel[i]);
			max = (max > f) ? max : f;
		}

		if (max == 0) {
			for (i = 0; i < m; i++) {
				ihkernel[i] = ivkernel[i] = 0;
			}

			*iscale = 30;
			return (MLIB_SUCCESS);
		}

		scaleh = mlib_ilogb(max * normh);

/* koeff. are so large */
		if ((scaleh + scalev) > 58)
			return (MLIB_FAILURE);

		scale = 58 - (scaleh + scalev);

/* scale must be even */
		if ((scale & 1) != 0) {
			scaleh++;
			scale--;
		}

		scale >>= 1;
		*iscale = scale;
		scaleh -= 29;
		scalev -= 29;

		if (scaleh > 0) {
			while (scaleh > 30) {
				normh /= (1 << 30);
				scaleh -= 30;
			}

			normh = normh / (1 << scaleh);
		} else {
			while (scaleh < -30) {
				normh *= (1 << 30);
				scaleh += 30;
			}

			normh = normh * (1 << (-scaleh));
		}

		for (i = 0; i < m; i++) {
			if (fhkernel[i] > 0.0) {
				CLAMP_S32(ihkernel[i],
				    fhkernel[i] * normh + 0.5);
			} else {
				CLAMP_S32(ihkernel[i],
				    fhkernel[i] * normh - 0.5);
			}
		}

		if (scalev > 0) {
			while (scalev > 30) {
				normv /= (1 << 30);
				scalev -= 30;
			}

			normv = normv / (1 << scalev);
		} else {
			while (scalev < -30) {
				normv *= (1 << 30);
				scalev += 30;
			}

			normv = normv * (1 << (-scalev));
		}

		for (i = 0; i < m; i++) {
			if (fvkernel[i] > 0.0) {
				CLAMP_S32(ivkernel[i],
				    fvkernel[i] * normv + 0.5);
			} else {
				CLAMP_S32(ivkernel[i],
				    fvkernel[i] * normv - 0.5);
			}
		}

		return (MLIB_SUCCESS);
	} else {
		return (MLIB_FAILURE);
	}
}

/* *********************************************************** */
