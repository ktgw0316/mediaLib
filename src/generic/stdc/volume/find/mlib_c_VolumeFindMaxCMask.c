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
 * Copyright 2007 Sun Microsystems, Inc.  All rights reserved.
 * Use is subject to license terms.
 */

#pragma ident	"@(#)mlib_c_VolumeFindMaxCMask.c	9.3	07/10/09 SMI"

/*
 *  FUNCTION
 *    mlib_VolumeFindMaxCMask_[U8|S16]
 *    Finds maximum values on each ray in structure mlib_rays
 *    and return them in array .
 *
 *  SYNOPSIS
 *    mlib_status mlib_VolumeFindMaxCMask_U8
 *                                   (mlib_u8         *max,
 *                                    const mlib_rays *rays,
 *                                    const mlib_u8   *cmask,
 *                                    mlib_s32        thresh)
 *    mlib_status mlib_VolumeFindMaxCMask_S16
 *                                   (mlib_s16        *max,
 *                                    const mlib_rays *rays,
 *                                    const mlib_u8   *cmask,
 *                                    mlib_s32        thresh)
 *
 *  ARGUMENTS
 *    max       Pointer to an array of rays->nrays maximum
 *              values of the samples in each ray.
 *
 *    rays      Pointer to an mlib rays structure.
 *              The data rays->results are organized with ray number,
 *              not ray step, varying fastest, which are the output
 *              of the ray casting functions. The data values beyond
 *              its maximum step on a ray,
 *              e.g. rays->results[rays->nsteps[i]][i] on ray i, might not be 0.
 *
 *    thresh    Threshold.
 *
 *    cmask     Pointer to unsigned 8-bit mask array.
 *              When cmask[j] > thresh, then the data in step j,
 *              rays->results[j], should be considered.
 *
 *  DESCRIPTION
 *    Finds maximum values for each ray in structure mlib_rays rays
 *    and return them in array max . When cmask[j] > thresh,
 *    then the data in step j, rays->results[j], should be considered.
 */

#include <mlib_volume.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_VolumeFindMaxCMask_S16 = __mlib_VolumeFindMaxCMask_S16
#pragma weak mlib_VolumeFindMaxCMask_U8 = __mlib_VolumeFindMaxCMask_U8

#elif defined(__GNUC__)

__typeof__(__mlib_VolumeFindMaxCMask_S16) mlib_VolumeFindMaxCMask_S16
	__attribute__((weak, alias("__mlib_VolumeFindMaxCMask_S16")));
__typeof__(__mlib_VolumeFindMaxCMask_U8) mlib_VolumeFindMaxCMask_U8
	__attribute__((weak, alias("__mlib_VolumeFindMaxCMask_U8")));

#else

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */
mlib_status
__mlib_VolumeFindMaxCMask_U8(
	mlib_u8 *max,
	const mlib_rays *rays,
	const mlib_u8 *cmask,
	mlib_s32 thresh)
{
	mlib_s32 nrays;
	mlib_s32 i, j;

	if (!max || !rays)
		return (MLIB_NULLPOINTER);

	nrays = rays->nrays;
	++thresh;

	if (!nrays)
		return (MLIB_FAILURE);

	for (i = 0; i < (nrays & ~1); i += 2) {
		mlib_s32 nsteps, nstepsmax, imax, mask;
		mlib_s32 cmax1, cmax2;
		mlib_s32 nsteps1, nsteps2, delsteps;

		nsteps1 = rays->nsteps[i];
		nsteps2 = rays->nsteps[i + 1];

		delsteps = (nsteps1 - nsteps2);
		mask = delsteps >> 31;
		nsteps = nsteps2 + (delsteps & mask);
		cmax1 = cmax2 = MLIB_U8_MIN;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (j = 0; j < nsteps; j++) {
			mlib_s32 maskm = ((mlib_s32)cmask[j] - thresh);
			mlib_s32 val1, del1, mask1;
			mlib_s32 val2, del2, mask2;
			mlib_u8 *stepp;

			stepp = (mlib_u8 *)rays->results[j];
			val1 = (stepp)[i];
			val2 = (stepp)[i + 1];
			del1 = (val1 - cmax1);
			del2 = (val2 - cmax2);
			mask1 = (del1 | maskm) >> 31;
			mask2 = (del2 | maskm) >> 31;
			cmax1 += (del1 & ~mask1);
			cmax2 += (del2 & ~mask2);
		}

		max[i] = (mlib_u8)cmax1;
		max[i + 1] = (mlib_u8)cmax2;

		imax = i - mask;
		nstepsmax = nsteps2 + (delsteps & ~mask);
		cmax1 = max[imax];
		for (; j < nstepsmax; j++) {
			mlib_s32 maskm = ((mlib_s32)cmask[j] - thresh);
			mlib_s32 val1, del1, mask1;
			mlib_u8 *stepp;

			stepp = (mlib_u8 *)rays->results[j];
			val1 = (stepp)[imax];
			del1 = (val1 - cmax1);
			mask1 = (del1 | maskm) >> 31;
			cmax1 += (del1 & ~mask1);
		}

		max[imax] = (mlib_u8)cmax1;
	}

	if (nrays & 1) {
		mlib_s32 nsteps, cmax1;

		nsteps = rays->nsteps[i];
		cmax1 = MLIB_U8_MIN;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (j = 0; j < nsteps; j++) {
			mlib_s32 maskm = ((mlib_s32)cmask[j] - thresh);
			mlib_s32 val1, del1, mask1;
			mlib_u8 *stepp;

			stepp = (mlib_u8 *)rays->results[j];
			val1 = (stepp)[i];
			del1 = (val1 - cmax1);
			mask1 = (del1 | maskm) >> 31;
			cmax1 += (del1 & ~mask1);
		}

		max[i] = (mlib_u8)cmax1;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
mlib_status
__mlib_VolumeFindMaxCMask_S16(
	mlib_s16 *max,
	const mlib_rays *rays,
	const mlib_u8 *cmask,
	mlib_s32 thresh)
{
	mlib_s32 nrays;
	mlib_s32 i, j;

	if (!max || !rays)
		return (MLIB_NULLPOINTER);

	nrays = rays->nrays;
	++thresh;

	if (!nrays)
		return (MLIB_FAILURE);

	for (i = 0; i < (nrays & ~1); i += 2) {
		mlib_s32 nsteps, nstepsmax, imax, mask;
		mlib_s32 cmax1, cmax2;
		mlib_s32 nsteps1, nsteps2, delsteps;

		nsteps1 = rays->nsteps[i];
		nsteps2 = rays->nsteps[i + 1];

		delsteps = (nsteps1 - nsteps2);
		mask = delsteps >> 31;
		nsteps = nsteps2 + (delsteps & mask);
		cmax1 = cmax2 = MLIB_S16_MIN;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (j = 0; j < nsteps; j++) {
			mlib_s32 maskm = ((mlib_s32)cmask[j] - thresh);
			mlib_s32 val1, del1, mask1;
			mlib_s32 val2, del2, mask2;
			mlib_s16 *stepp;

			stepp = (mlib_s16 *)rays->results[j];
			val1 = (stepp)[i];
			val2 = (stepp)[i + 1];
			del1 = (val1 - cmax1);
			del2 = (val2 - cmax2);
			mask1 = (del1 | maskm) >> 31;
			mask2 = (del2 | maskm) >> 31;
			cmax1 += (del1 & ~mask1);
			cmax2 += (del2 & ~mask2);
		}

		max[i] = (mlib_s16)cmax1;
		max[i + 1] = (mlib_s16)cmax2;

		imax = i - mask;
		nstepsmax = nsteps2 + (delsteps & ~mask);
		cmax1 = max[imax];
		for (; j < nstepsmax; j++) {
			mlib_s32 maskm = ((mlib_s32)cmask[j] - thresh);
			mlib_s32 val1, del1, mask1;
			mlib_s16 *stepp;

			stepp = (mlib_s16 *)rays->results[j];
			val1 = (stepp)[imax];
			del1 = (val1 - cmax1);
			mask1 = (del1 | maskm) >> 31;
			cmax1 += (del1 & ~mask1);
		}

		max[imax] = (mlib_s16)cmax1;
	}

	if (nrays & 1) {
		mlib_s32 nsteps, cmax1;

		nsteps = rays->nsteps[i];
		cmax1 = MLIB_S16_MIN;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (j = 0; j < nsteps; j++) {
			mlib_s32 maskm = ((mlib_s32)cmask[j] - thresh);
			mlib_s32 val1, del1, mask1;
			mlib_s16 *stepp;

			stepp = (mlib_s16 *)rays->results[j];
			val1 = (stepp)[i];
			del1 = (val1 - cmax1);
			mask1 = (del1 | maskm) >> 31;
			cmax1 += (del1 & ~mask1);
		}

		max[i] = (mlib_s16)cmax1;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
