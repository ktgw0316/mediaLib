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

#pragma ident	"@(#)mlib_c_VolumeRayCast_BPN.c	9.2	07/10/09 SMI"

/*
 *  FUNCTION
 *    mlib_VolumeRayCast_Blocked_Parallel_Nearest_[U8_U8|S16_S16]
 *      Cast parallel rays through a three dimensional data set
 *      and return the interpolated samples.
 *
 *  SYNOPSIS
 *    mlib_status mlib_VolumeRayCast_Blocked_Parallel_Nearest_[U8_U8|S16_S16]
 *                                           (const mlib_rays      *rays,
 *                                            const mlib_blkvolume *blk,
 *                                            void                 *buffer)
 *
 *  ARGUMENTS
 *    rays      Casting rays.
 *    blk       Volume data in blocked data format.
 *    buffer    Working buffer.
 *
 *  DESCRIPTION
 *    In nearest neighbor operation, the sample value at point P
 *    is replaced with the value of the nearest neighbor voxel.
 *
 */

#include <stdio.h>
#include <mlib_volume.h>
#include <mlib_SysMath.h>
#include <mlib_c_VolumeRayCast_Blocked.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_VolumeRayCast_Blocked_Parallel_Nearest_U8_U8 = \
__mlib_VolumeRayCast_Blocked_Parallel_Nearest_U8_U8
#pragma weak mlib_VolumeRayCast_Blocked_Parallel_Nearest_S16_S16 = \
	__mlib_VolumeRayCast_Blocked_Parallel_Nearest_S16_S16
#elif defined(__GNUC__)

__typeof__(__mlib_VolumeRayCast_Blocked_Parallel_Nearest_U8_U8)
	mlib_VolumeRayCast_Blocked_Parallel_Nearest_U8_U8
	__attribute__((weak,
	alias("__mlib_VolumeRayCast_Blocked_Parallel_Nearest_U8_U8")));
__typeof__(__mlib_VolumeRayCast_Blocked_Parallel_Nearest_S16_S16)
	mlib_VolumeRayCast_Blocked_Parallel_Nearest_S16_S16
	__attribute__((weak,
	alias("__mlib_VolumeRayCast_Blocked_Parallel_Nearest_S16_S16")));

#else

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	MAX_RAY_NUM	64

/* *********************************************************** */

mlib_status
__mlib_VolumeRayCast_Blocked_Parallel_Nearest_U8_U8(
	mlib_rays *rays,
	const mlib_blkvolume *blk,
	void *buffer)
{
	mlib_s32 xysize;
	mlib_s32 nrays, i;
	mlib_d64 dOx, dOy, dOz;
	mlib_u8 *voxels, *dp;
	mlib_s32 nsteps, cray, minsteps, maxsteps, cstp;
	mlib_u32 n, m, index;
	mlib_d64 Ox, Oy, Oz;
	mlib_d64 cx[MAX_RAY_NUM], cy[MAX_RAY_NUM], cz[MAX_RAY_NUM];
	mlib_d64 X, Y, Z;
	mlib_u8 a;
	mlib_d64 go_to_rays = 0., go_to_steps = 0., testx, testy, testz;
	mlib_s32 xsrc, ysrc, zsrc;

	if (rays == NULL || blk == NULL)
		return (MLIB_NULLPOINTER);

	xysize = blk->xysize, nrays = rays->nrays;
	voxels = (mlib_u8 *)blk->voxels;

	dOx = rays->incs[0][0];
	dOy = rays->incs[0][1];
	dOz = rays->incs[0][2];

	i = xysize >> 6;
	n = 0;
	while (i >>= 1)
		n++;
	m = (n << 1) + 12;
	n += 11;

	if (nrays <= MAX_RAY_NUM) {
		minsteps = MLIB_S32_MAX;
		maxsteps = -1;

		for (cray = 0; cray < nrays; cray++) {
			if (rays->nsteps[cray] < minsteps)
				minsteps = rays->nsteps[cray];
			if (rays->nsteps[cray] > maxsteps)
				maxsteps = rays->nsteps[cray];
		}

		Ox = rays->starts[0][0];
		Oy = rays->starts[0][1];
		Oz = rays->starts[0][2];

		if (mlib_fabs(rays->incs[0][0]) / 12. > go_to_rays)
			go_to_rays = mlib_fabs(rays->incs[0][0] / 12.);

		if (mlib_fabs(rays->incs[0][1]) / 6. > go_to_rays)
			go_to_rays = mlib_fabs(rays->incs[0][1] / 6.);

		if (mlib_fabs(rays->incs[0][2]) > go_to_rays)
			go_to_rays = mlib_fabs(rays->incs[0][2]);

		cx[0] = 0;
		cy[0] = 0;
		cz[0] = 0;

		testx = 0;
		testy = 0;
		testz = 0;

		for (cray = 1; cray < nrays; cray++) {
			cx[cray] = rays->starts[cray][0] - Ox;
			testx += mlib_fabs(cx[cray] - cx[cray - 1]);
			cy[cray] = rays->starts[cray][1] - Oy;
			testy += mlib_fabs(cy[cray] - cy[cray - 1]);
			cz[cray] = rays->starts[cray][2] - Oz;
			testz += mlib_fabs(cz[cray] - cz[cray - 1]);
		}

		Ox = Ox + 0.5;
		Oy = Oy + 0.5;
		Oz = Oz + 0.5;

		if (nrays != 1) {
			if (testx / (nrays - 1) / 12. > go_to_steps)
				go_to_steps = testx / (nrays - 1) / 12.;

			if (testy / (nrays - 1) / 6. > go_to_steps)
				go_to_steps = testy / (nrays - 1) / 6.;

			if (testz / (nrays - 1) > go_to_steps)
				go_to_steps = testz / (nrays - 1);
		}
	}

	if ((go_to_rays > go_to_steps) && (nrays <= MAX_RAY_NUM)) {

		for (cstp = 0; cstp < minsteps; cstp++) {
			dp = (mlib_u8 *)rays->results[cstp];

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (cray = 0; cray < nrays; cray++) {
				X = Ox + cx[cray];
				xsrc = (mlib_s32)X;
				Y = Oy + cy[cray];
				ysrc = (mlib_s32)Y;
				Z = Oz + cz[cray];
				zsrc = (mlib_s32)Z;
				index = mlib_indx[xsrc] |
					mlib_indy[ysrc & 0x3F] |
					mlib_indz[zsrc & 0x1F] |
					((ysrc & (~0x3F)) << n) |
					((zsrc & (~0x1F)) << m);

				*dp = voxels[index];
				dp++;
			}

			Ox += dOx;
			Oy += dOy;
			Oz += dOz;
		}

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (cstp = minsteps; cstp < maxsteps; cstp++) {
			dp = (mlib_u8 *)rays->results[cstp];

			for (cray = 0; cray < nrays; cray++) {

				if (cstp < rays->nsteps[cray]) {
					X = Ox + cx[cray];
					xsrc = (mlib_s32)X;
					Y = Oy + cy[cray];
					ysrc = (mlib_s32)Y;
					Z = Oz + cz[cray];
					zsrc = (mlib_s32)Z;
					index = mlib_indx[xsrc] |
						mlib_indy[ysrc & 0x3F] |
						mlib_indz[zsrc & 0x1F] |
						((ysrc & (~0x3F)) << n) |
						((zsrc & (~0x1F)) << m);

					*dp = voxels[index];
					dp++;

				} else {
					dp++;
				}

			}
			Ox += dOx;
			Oy += dOy;
			Oz += dOz;
		}
	} else {

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (cray = 0; cray < nrays; cray++) {
			X = rays->starts[cray][0] + 0.5;
			Y = rays->starts[cray][1] + 0.5;
			Z = rays->starts[cray][2] + 0.5;

			xsrc = (mlib_s32)X;
			X += dOx;
			ysrc = (mlib_s32)Y;
			Y += dOy;
			zsrc = (mlib_s32)Z;
			Z += dOz;

			nsteps = rays->nsteps[cray];

			for (cstp = 0; cstp < nsteps; cstp++) {
				index = mlib_indx[xsrc] |
					mlib_indy[ysrc & 0x3F] |
					mlib_indz[zsrc & 0x1F] |
					((ysrc & (~0x3F)) << n) |
					((zsrc & (~0x1F)) << m);

				a = voxels[index];

				xsrc = (mlib_s32)X;
				X += dOx;
				ysrc = (mlib_s32)Y;
				Y += dOy;
				zsrc = (mlib_s32)Z;
				Z += dOz;

				((mlib_u8 *)rays->results[cstp])[cray] = a;
			}
		}
	}
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VolumeRayCast_Blocked_Parallel_Nearest_S16_S16(
	mlib_rays *rays,
	const mlib_blkvolume *blk,
	void *buffer)
{
	mlib_s32 xysize;
	mlib_s32 nrays, i;
	mlib_d64 dOx, dOy, dOz;
	mlib_u16 *voxels, *dp;
	mlib_s32 nsteps, cray, minsteps, maxsteps, cstp;
	mlib_u32 n, m, index;
	mlib_d64 Ox, Oy, Oz;
	mlib_d64 cx[MAX_RAY_NUM], cy[MAX_RAY_NUM], cz[MAX_RAY_NUM];
	mlib_d64 X, Y, Z;
	mlib_u16 a;
	mlib_d64 go_to_rays = 0., go_to_steps = 0., testx, testy, testz;
	mlib_s32 xsrc, ysrc, zsrc;

	if (rays == NULL || blk == NULL)
		return (MLIB_NULLPOINTER);

	xysize = blk->xysize, nrays = rays->nrays;
	voxels = (mlib_u16 *)blk->voxels;

	dOx = rays->incs[0][0];
	dOy = rays->incs[0][1];
	dOz = rays->incs[0][2];

	i = xysize >> 6;
	n = 0;
	while (i >>= 1)
		n++;
	m = (n << 1) + 12;
	n += 11;

	if (nrays <= MAX_RAY_NUM) {
		minsteps = MLIB_S32_MAX;
		maxsteps = -1;

		for (cray = 0; cray < nrays; cray++) {
			if (rays->nsteps[cray] < minsteps)
				minsteps = rays->nsteps[cray];
			if (rays->nsteps[cray] > maxsteps)
				maxsteps = rays->nsteps[cray];
		}

		Ox = rays->starts[0][0];
		Oy = rays->starts[0][1];
		Oz = rays->starts[0][2];

		if (mlib_fabs(rays->incs[0][0]) / 6. > go_to_rays)
			go_to_rays = mlib_fabs(rays->incs[0][0] / 6.);

		if (mlib_fabs(rays->incs[0][1]) / 6. > go_to_rays)
			go_to_rays = mlib_fabs(rays->incs[0][1] / 6.);

		if (mlib_fabs(rays->incs[0][2]) > go_to_rays)
			go_to_rays = mlib_fabs(rays->incs[0][2]);

		cx[0] = 0;
		cy[0] = 0;
		cz[0] = 0;

		testx = 0;
		testy = 0;
		testz = 0;

		for (cray = 1; cray < nrays; cray++) {
			cx[cray] = rays->starts[cray][0] - Ox;
			testx += mlib_fabs(cx[cray] - cx[cray - 1]);
			cy[cray] = rays->starts[cray][1] - Oy;
			testy += mlib_fabs(cy[cray] - cy[cray - 1]);
			cz[cray] = rays->starts[cray][2] - Oz;
			testz += mlib_fabs(cz[cray] - cz[cray - 1]);
		}

		Ox = Ox + 0.5;
		Oy = Oy + 0.5;
		Oz = Oz + 0.5;

		if (nrays != 1) {
			if (testx / (nrays - 1) / 6. > go_to_steps)
				go_to_steps = testx / (nrays - 1) / 6.;

			if (testy / (nrays - 1) / 6. > go_to_steps)
				go_to_steps = testy / (nrays - 1) / 6.;

			if (testz / (nrays - 1) > go_to_steps)
				go_to_steps = testz / (nrays - 1);
		}
	}

	if ((go_to_rays > go_to_steps) && (nrays <= MAX_RAY_NUM)) {

		for (cstp = 0; cstp < minsteps; cstp++) {
			dp = (mlib_u16 *)rays->results[cstp];

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (cray = 0; cray < nrays; cray++) {
				X = Ox + cx[cray];
				xsrc = (mlib_s32)X;
				Y = Oy + cy[cray];
				ysrc = (mlib_s32)Y;
				Z = Oz + cz[cray];
				zsrc = (mlib_s32)Z;
				index = mlib_indx[xsrc] |
					mlib_indy[ysrc & 0x3F] |
					mlib_indz[zsrc & 0x1F] |
					((ysrc & (~0x3F)) << n) |
					((zsrc & (~0x1F)) << m);

				*dp = voxels[index];
				dp++;
			}

			Ox += dOx;
			Oy += dOy;
			Oz += dOz;
		}

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (cstp = minsteps; cstp < maxsteps; cstp++) {
			dp = (mlib_u16 *)rays->results[cstp];

			for (cray = 0; cray < nrays; cray++) {

				if (cstp < rays->nsteps[cray]) {
					X = Ox + cx[cray];
					xsrc = (mlib_s32)X;
					Y = Oy + cy[cray];
					ysrc = (mlib_s32)Y;
					Z = Oz + cz[cray];
					zsrc = (mlib_s32)Z;
					index = mlib_indx[xsrc] |
						mlib_indy[ysrc & 0x3F] |
						mlib_indz[zsrc & 0x1F] |
						((ysrc & (~0x3F)) << n) |
						((zsrc & (~0x1F)) << m);

					*dp = voxels[index];
					dp++;

				} else {
					dp++;
				}

			}
			Ox += dOx;
			Oy += dOy;
			Oz += dOz;
		}
	} else {

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (cray = 0; cray < nrays; cray++) {
			X = rays->starts[cray][0] + 0.5;
			Y = rays->starts[cray][1] + 0.5;
			Z = rays->starts[cray][2] + 0.5;

			xsrc = (mlib_s32)X;
			X += dOx;
			ysrc = (mlib_s32)Y;
			Y += dOy;
			zsrc = (mlib_s32)Z;
			Z += dOz;

			nsteps = rays->nsteps[cray];

			for (cstp = 0; cstp < nsteps; cstp++) {
				index = mlib_indx[xsrc] |
					mlib_indy[ysrc & 0x3F] |
					mlib_indz[zsrc & 0x1F] |
					((ysrc & (~0x3F)) << n) |
					((zsrc & (~0x1F)) << m);

				a = voxels[index];

				xsrc = (mlib_s32)X;
				X += dOx;
				ysrc = (mlib_s32)Y;
				Y += dOy;
				zsrc = (mlib_s32)Z;
				Z += dOz;

				((mlib_u16 *)rays->results[cstp])[cray] = a;
			}
		}
	}
	return (MLIB_SUCCESS);
}

/* *********************************************************** */
