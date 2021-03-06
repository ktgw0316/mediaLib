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

#pragma ident	"@(#)mlib_v_VolumeFindMaxCMask.c	9.3	07/11/05 SMI"

/*
 *  FUNCTION
 *    mlib_VolumeFindMaxCMask_[U8|S16]
 *    Finds maximum values on each ray in structure mlib_rays
 *    and return them in array .
 *
 *  SYNOPSIS
 *    mlib_status mlib_VolumeFindMaxCMask_U8(mlib_u8         *max,
 *                                           const mlib_rays *rays,
 *                                           const mlib_u8   *cmask,
 *                                           mlib_s32        thresh)
 *    mlib_status mlib_VolumeFindMaxCMask_S16(mlib_s16        *max,
 *                                            const mlib_rays *rays,
 *                                            const mlib_u8   *cmask,
 *                                            mlib_s32        thresh)
 *
 *  ARGUMENTS
 *    max       Pointer to an array of rays->nrays maximum
 *              values of the samples in each ray.
 *    rays      Pointer to an mlib rays structure.
 *              The data rays->results are organized with ray number,
 *              not ray step, varying fastest, which are the output
 *              of the ray casting functions. The data values beyond
 *              its maximum step on a ray,
 *              e.g. rays->results[rays->nsteps[i]][i] on ray i, might not be 0.
 *    thresh    Threshold.
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
#include <vis_proto.h>

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

#if MLIB_VIS < 0x200

#define	UPDATE_MAX(val, dmax, mask)                             \
	vis_pst_16(val, &dmax, mask)

#else /* MLIB_VIS < 0x200 */

#include <mlib_v_VolumeFindMax.h>

#define	UPDATE_MAX(val, dmax, mask)                             \
	vis_write_bmask(mlib_bmask_cmp16_v[mask], 0);           \
	dmax = vis_bshuffle(val, dmax)

#endif /* MLIB_VIS < 0x200 */

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
	mlib_s32 precount;
	mlib_d64 done = vis_fone();
	mlib_d64 dzero = vis_fzero();
	mlib_f32 fzero = vis_read_hi(dzero);

	if (!max || !rays)
		return (MLIB_NULLPOINTER);

	nrays = rays->nrays;

	if (!nrays)
		return (MLIB_FAILURE);

	vis_write_gsr(7 << 3);
	precount = (nrays < 8) ? nrays : ((4 - (mlib_addr)max) & 3);

	for (i = 0; i < precount; i++) {
		mlib_s32 nsteps, cmax1;

		nsteps = rays->nsteps[i];
		cmax1 = MLIB_U8_MIN;
		for (j = 0; j < nsteps; j++) {
			mlib_s32 val1, del1, mask1;
			mlib_u8 *stepp;
			mlib_s32 maskm = ((mlib_s32)cmask[j] - thresh - 1);

			stepp = (mlib_u8 *)rays->results[j];
			val1 = (stepp)[i];
			del1 = (val1 - cmax1);
			mask1 = (del1 | maskm) >> 31;
			cmax1 += (del1 & ~mask1);
		}

		max[i] = (mlib_u8)cmax1;
	}

	for (; i < nrays - 7; i += 8) {
		mlib_d64 dmax0 = dzero, dmax1 = dzero;
		mlib_d64 *stepp;
		mlib_s32 n, smax;
		mlib_d64 dsteps0, dsteps1, dstepsmax;
		mlib_d64 val0, val1, val;
		mlib_s32 mask0, mask1, maskm;

		vis_alignaddr((void *)0, 6);

		dsteps0 =
			vis_faligndata(vis_ld_u16_i(rays->nsteps + i, 14),
			dsteps0);
		dsteps0 =
			vis_faligndata(vis_ld_u16_i(rays->nsteps + i, 10),
			dsteps0);
		dsteps0 =
			vis_faligndata(vis_ld_u16_i(rays->nsteps + i, 6),
			dsteps0);
		dsteps0 =
			vis_faligndata(vis_ld_u16_i(rays->nsteps + i, 2),
			dsteps0);

		dsteps1 =
			vis_faligndata(vis_ld_u16_i(rays->nsteps + i, 30),
			dsteps1);
		dsteps1 =
			vis_faligndata(vis_ld_u16_i(rays->nsteps + i, 26),
			dsteps1);
		dsteps1 =
			vis_faligndata(vis_ld_u16_i(rays->nsteps + i, 22),
			dsteps1);
		dsteps1 =
			vis_faligndata(vis_ld_u16_i(rays->nsteps + i, 18),
			dsteps1);

		dstepsmax = dsteps0;
		UPDATE_MAX(dsteps1, dstepsmax, vis_fcmpgt16(dsteps1, dsteps0));
		smax = ((mlib_u16 *)&dstepsmax)[0];
		n = ((mlib_u16 *)&dstepsmax)[1];
		smax += (n - smax) & ~((n - smax) >> 31);
		n = ((mlib_u16 *)&dstepsmax)[2];
		smax += (n - smax) & ~((n - smax) >> 31);
		n = ((mlib_u16 *)&dstepsmax)[3];
		smax += (n - smax) & ~((n - smax) >> 31);

		stepp = (mlib_d64 *)(((mlib_u8 *)rays->results[0]) + i);
		maskm = (thresh - (mlib_s32)cmask[0]) >> 31;
		stepp = (mlib_d64 *)vis_alignaddr(stepp, 0);
		val0 = stepp[0];
		val1 = stepp[1];
		val = vis_faligndata(val0, val1);

#if MLIB_VIS >= 0x200
#pragma pipeloop(0)
#endif /* MLIB_VIS >= 0x200 */
		for (j = 1; j < smax; j++) {

			stepp = (mlib_d64 *)(((mlib_u8 *)rays->results[j]) + i);
			stepp = (mlib_d64 *)vis_alignaddr(stepp, 0);

			val0 = vis_fpmerge(fzero, vis_read_hi(val));
			val1 = vis_fpmerge(fzero, vis_read_lo(val));
			mask0 = maskm & vis_fcmpgt16(dsteps0, dzero);
			mask1 = maskm & vis_fcmpgt16(dsteps1, dzero);
			mask0 &= vis_fcmpgt16(val0, dmax0);
			mask1 &= vis_fcmpgt16(val1, dmax1);
			UPDATE_MAX(val0, dmax0, mask0);
			UPDATE_MAX(val1, dmax1, mask1);
			maskm = (thresh - (mlib_s32)cmask[j]) >> 31;
			val0 = stepp[0];
			val1 = stepp[1];
			dsteps0 = vis_fpadd16(dsteps0, done);
			dsteps1 = vis_fpadd16(dsteps1, done);
			val = vis_faligndata(val0, val1);
		}

		val0 = vis_fpmerge(fzero, vis_read_hi(val));
		val1 = vis_fpmerge(fzero, vis_read_lo(val));
		mask0 = maskm & vis_fcmpgt16(dsteps0, dzero);
		mask1 = maskm & vis_fcmpgt16(dsteps1, dzero);
		mask0 &= vis_fcmpgt16(val0, dmax0);
		mask1 &= vis_fcmpgt16(val1, dmax1);
		UPDATE_MAX(val0, dmax0, mask0);
		UPDATE_MAX(val1, dmax1, mask1);

		*(mlib_f32 *)(max + i) = vis_fpack16(dmax0);
		*(mlib_f32 *)(max + i + 4) = vis_fpack16(dmax1);
	}

	for (; i < nrays; i++) {
		mlib_s32 nsteps, cmax1;

		nsteps = rays->nsteps[i];
		cmax1 = MLIB_U8_MIN;

#pragma pipeloop(0)
		for (j = 0; j < nsteps; j++) {
			mlib_s32 val1, del1, mask1;
			mlib_u8 *stepp;
			mlib_s32 maskm = ((mlib_s32)cmask[j] - thresh - 1);

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
	mlib_s32 precount;
	mlib_d64 dstart = vis_to_double_dup(0x80008000);
	mlib_d64 done = vis_fone();
	mlib_d64 dzero = vis_fzero();

	if (!max || !rays)
		return (MLIB_NULLPOINTER);

	nrays = rays->nrays;

	if (!nrays)
		return (MLIB_FAILURE);

	precount = (nrays < 8) ? nrays : (((8 - (mlib_addr)max) & 7) >> 1);

	for (i = 0; i < precount; i++) {
		mlib_s32 nsteps, cmax1;

		nsteps = rays->nsteps[i];
		cmax1 = MLIB_S16_MIN;
		for (j = 0; j < nsteps; j++) {
			mlib_s32 val1, del1, mask1;
			mlib_s16 *stepp;
			mlib_s32 maskm = ((mlib_s32)cmask[j] - thresh - 1);

			stepp = (mlib_s16 *)rays->results[j];
			val1 = (stepp)[i];
			del1 = (val1 - cmax1);
			mask1 = (del1 | maskm) >> 31;
			cmax1 += (del1 & ~mask1);
		}

		max[i] = (mlib_s16)cmax1;
	}

	for (; i < nrays - 7; i += 8) {
		mlib_d64 dmax0 = dstart, dmax1 = dstart;
		mlib_d64 *stepp;
		mlib_s32 n, smax;
		mlib_d64 dsteps0, dsteps1, dstepsmax;
		mlib_d64 val0, val1, sval0, sval1, sval2;
		mlib_s32 mask0, mask1, maskm;

		vis_alignaddr((void *)0, 6);

		dsteps0 =
			vis_faligndata(vis_ld_u16_i(rays->nsteps + i, 14),
			dsteps0);
		dsteps0 =
			vis_faligndata(vis_ld_u16_i(rays->nsteps + i, 10),
			dsteps0);
		dsteps0 =
			vis_faligndata(vis_ld_u16_i(rays->nsteps + i, 6),
			dsteps0);
		dsteps0 =
			vis_faligndata(vis_ld_u16_i(rays->nsteps + i, 2),
			dsteps0);

		dsteps1 =
			vis_faligndata(vis_ld_u16_i(rays->nsteps + i, 30),
			dsteps1);
		dsteps1 =
			vis_faligndata(vis_ld_u16_i(rays->nsteps + i, 26),
			dsteps1);
		dsteps1 =
			vis_faligndata(vis_ld_u16_i(rays->nsteps + i, 22),
			dsteps1);
		dsteps1 =
			vis_faligndata(vis_ld_u16_i(rays->nsteps + i, 18),
			dsteps1);

		dstepsmax = dsteps0;
		UPDATE_MAX(dsteps1, dstepsmax, vis_fcmpgt16(dsteps1, dsteps0));

		smax = ((mlib_u16 *)&dstepsmax)[0];
		n = ((mlib_u16 *)&dstepsmax)[1];
		smax += (n - smax) & ~((n - smax) >> 31);
		n = ((mlib_u16 *)&dstepsmax)[2];
		smax += (n - smax) & ~((n - smax) >> 31);
		n = ((mlib_u16 *)&dstepsmax)[3];
		smax += (n - smax) & ~((n - smax) >> 31);

		stepp = (mlib_d64 *)(((mlib_s16 *)rays->results[0]) + i);

		stepp = (mlib_d64 *)vis_alignaddr(stepp, 0);

		maskm = (thresh - (mlib_s32)cmask[0]) >> 31;
		sval0 = stepp[0];
		sval1 = stepp[1];
		sval2 = stepp[2];

#if MLIB_VIS >= 0x200
#pragma pipeloop(0)
#endif /* MLIB_VIS >= 0x200 */
		for (j = 1; j < smax; j++) {

			val0 = vis_faligndata(sval0, sval1);
			val1 = vis_faligndata(sval1, sval2);

			stepp = (mlib_d64 *)(((mlib_s16 *)rays->results[j]) +
				i);
			stepp = (mlib_d64 *)vis_alignaddr(stepp, 0);

			mask0 = maskm & vis_fcmpgt16(dsteps0, dzero);
			mask1 = maskm & vis_fcmpgt16(dsteps1, dzero);
			mask0 &= vis_fcmpgt16(val0, dmax0);
			mask1 &= vis_fcmpgt16(val1, dmax1);
			sval0 = stepp[0];
			sval1 = stepp[1];
			sval2 = stepp[2];
			maskm = (thresh - (mlib_s32)cmask[j]) >> 31;
			dsteps0 = vis_fpadd16(dsteps0, done);
			dsteps1 = vis_fpadd16(dsteps1, done);
			UPDATE_MAX(val0, dmax0, mask0);
			UPDATE_MAX(val1, dmax1, mask1);
		}

		val0 = vis_faligndata(sval0, sval1);
		val1 = vis_faligndata(sval1, sval2);

		mask0 = maskm & vis_fcmpgt16(dsteps0, dzero);
		mask1 = maskm & vis_fcmpgt16(dsteps1, dzero);
		mask0 &= vis_fcmpgt16(val0, dmax0);
		mask1 &= vis_fcmpgt16(val1, dmax1);
		UPDATE_MAX(val0, dmax0, mask0);
		UPDATE_MAX(val1, dmax1, mask1);

		*(mlib_d64 *)(max + i) = dmax0;
		*(mlib_d64 *)(max + i + 4) = dmax1;
	}

	for (; i < nrays; i++) {
		mlib_s32 nsteps, cmax1;

		nsteps = rays->nsteps[i];
		cmax1 = MLIB_S16_MIN;

#pragma pipeloop(0)
		for (j = 0; j < nsteps; j++) {
			mlib_s32 val1, del1, mask1;
			mlib_s16 *stepp;
			mlib_s32 maskm = ((mlib_s32)cmask[j] - thresh - 1);

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
