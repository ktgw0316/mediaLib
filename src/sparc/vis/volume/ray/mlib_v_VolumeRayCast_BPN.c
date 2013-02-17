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

#pragma ident	"@(#)mlib_v_VolumeRayCast_BPN.c	9.2	07/11/05 SMI"

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

#include <mlib_volume.h>
#include <mlib_SysMath.h>
#include <vis_proto.h>

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

#define	MLIB_SHIFT_20	20
#define	MLIB_PREC_20	(1 << MLIB_SHIFT_20)
#define	MLIB_MASK_20	~(MLIB_PREC_20 - 1)
#define	MAX_RAY_NUM	64
#define	MLIB_SHIFT	11
#define	MLIB_PREC	(1 << MLIB_SHIFT)

/* *********************************************************** */

#define	LOAD_SAVE_POINT_U8                                      \
	{                                                       \
	    X = OX + cx[cray];                                  \
	    Y = OY + cy[cray];                                  \
	    Z = OZ + cz[cray];                                  \
	    X = X & MLIB_MASK_20;                               \
	    Y = Y & MLIB_MASK_20;                               \
	    Z = Z & MLIB_MASK_20;                               \
	    ZYX = Z;                                            \
	    ZYX = (((ZYX << 22) + Y) << 13) + (X >> 9);         \
	    index = vis_array8(ZYX, n);                         \
	    *dp = *(voxels + index);                            \
	    dp++;                                               \
	}

/* *********************************************************** */

#define	LOAD_COMPRESS_U8_D64                                    \
	{                                                       \
	    X = OX + cx[nray];                                  \
	    Y = OY + cy[nray];                                  \
	    Z = OZ + cz[nray];                                  \
	    X = X & MLIB_MASK_20;                               \
	    Y = Y & MLIB_MASK_20;                               \
	    Z = Z & MLIB_MASK_20;                               \
	    ZYX = Z;                                            \
	    ZYX = (((ZYX << 22) + Y) << 13) + (X >> 9);         \
	    index = vis_array8(ZYX, n);                         \
	    sp = voxels + index;                                \
	    P1 = vis_ld_u8(sp);                                 \
	    P = vis_faligndata(P1, P);                          \
	}

/* *********************************************************** */

#define	LOAD_SAVE_POINT_U16                                     \
	{                                                       \
	    X = OX + cx[cray];                                  \
	    Y = OY + cy[cray];                                  \
	    Z = OZ + cz[cray];                                  \
	    X = X & MLIB_MASK_20;                               \
	    Y = Y & MLIB_MASK_20;                               \
	    Z = Z & MLIB_MASK_20;                               \
	    ZYX = Z;                                            \
	    ZYX = (((ZYX << 22) + Y) << 13) + (X >> 9);         \
	    index = vis_array16(ZYX, n);                        \
	    *dp = *(mlib_u16 *)((mlib_u8 *)voxels + index);     \
	    dp++;                                               \
	}

/* *********************************************************** */

#define	LOAD_COMPRESS_U16_D64                                   \
	{                                                       \
	    X = OX + cx[nray];                                  \
	    Y = OY + cy[nray];                                  \
	    Z = OZ + cz[nray];                                  \
	    X = X & MLIB_MASK_20;                               \
	    Y = Y & MLIB_MASK_20;                               \
	    Z = Z & MLIB_MASK_20;                               \
	    ZYX = Z;                                            \
	    ZYX = (((ZYX << 22) + Y) << 13) + (X >> 9);         \
	    index = vis_array16(ZYX, n);                        \
	    sp = (mlib_u16 *)((mlib_u8 *)voxels + index);       \
	    P1 = vis_ld_u16(sp);                                \
	    P = vis_faligndata(P1, P);                          \
	}

/* *********************************************************** */

static mlib_status
mlib_v_VolumeRayCast_Blocked_Parallel_Nearest_S16_S16_LittleSteps(
	mlib_rays *rays,
	const mlib_blkvolume *blk,
	mlib_s32 minsteps,
	mlib_s32 maxsteps);

static mlib_status
mlib_v_VolumeRayCast_Blocked_Parallel_Nearest_S16_S16_ManySteps(
	mlib_rays *rays,
	const mlib_blkvolume *blk,
	mlib_s32 minsteps,
	mlib_s32 maxsteps);

static mlib_status
mlib_v_VolumeRayCast_Blocked_Parallel_Nearest_U8_U8_LittleSteps(
	mlib_rays *rays,
	const mlib_blkvolume *blk,
	mlib_s32 minsteps,
	mlib_s32 maxsteps);

static mlib_status
mlib_v_VolumeRayCast_Blocked_Parallel_Nearest_U8_U8_ManySteps(
	mlib_rays *rays,
	const mlib_blkvolume *blk,
	mlib_s32 minsteps,
	mlib_s32 maxsteps);

/* *********************************************************** */

mlib_status
__mlib_VolumeRayCast_Blocked_Parallel_Nearest_U8_U8(
	mlib_rays *rays,
	const mlib_blkvolume *blk,
	void *buffer)
{
	mlib_s32 nrays, cray;
	mlib_s32 minsteps, maxsteps;

	if (rays == NULL || blk == NULL)
		return (MLIB_NULLPOINTER);

	nrays = rays->nrays;

	minsteps = MLIB_S32_MAX;
	maxsteps = -1;

	for (cray = 0; cray < nrays; cray++) {
		if (rays->nsteps[cray] < minsteps)
			minsteps = rays->nsteps[cray];

		if (rays->nsteps[cray] > maxsteps)
			maxsteps = rays->nsteps[cray];
	}

	if (maxsteps > 2000) {
		return (
		mlib_v_VolumeRayCast_Blocked_Parallel_Nearest_U8_U8_ManySteps
		(rays, blk, minsteps, maxsteps));
	} else {
		return (
		mlib_v_VolumeRayCast_Blocked_Parallel_Nearest_U8_U8_LittleSteps
		(rays, blk, minsteps, maxsteps));
	}
}

/* *********************************************************** */

mlib_status
mlib_v_VolumeRayCast_Blocked_Parallel_Nearest_U8_U8_ManySteps(
	mlib_rays *rays,
	const mlib_blkvolume *blk,
	mlib_s32 minsteps,
	mlib_s32 maxsteps)
{
	mlib_s32 xysize;
	mlib_s32 nrays, i, nray, count;
	mlib_s32 dX, dY, dZ;
	mlib_s32 OX, OY, OZ;
	mlib_u8 *voxels, *dp, *sp;
	mlib_s32 nsteps, cray, cstp;
	mlib_u32 n, index, align;
	mlib_d64 Ox, Oy, Oz;
	mlib_s32 X, Y, Z, cx[MAX_RAY_NUM], cy[MAX_RAY_NUM], cz[MAX_RAY_NUM];
	mlib_u8 a;
	mlib_d64 go_to_rays, go_to_steps, testx, testy, testz;
	mlib_d64 P, P1;
	mlib_d64 *dp_d64;
	mlib_u64 ZYX;

	xysize = blk->xysize, nrays = rays->nrays;

	dX = (mlib_s32)(rays->incs[0][0] * MLIB_PREC_20);
	dY = (mlib_s32)(rays->incs[0][1] * MLIB_PREC_20);
	dZ = (mlib_s32)(rays->incs[0][2] * MLIB_PREC_20);

	voxels = (mlib_u8 *)blk->voxels;

	i = xysize >> 6;
	n = 0;
	while (i >>= 1)
		n++;

	if (nrays <= MAX_RAY_NUM) {

		Ox = rays->starts[0][0];
		Oy = rays->starts[0][1];
		Oz = rays->starts[0][2];

		go_to_rays = 0;

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
			cx[cray] =
				(mlib_s32)((rays->starts[cray][0] -
				Ox) * MLIB_PREC_20);
			cy[cray] =
				(mlib_s32)((rays->starts[cray][1] -
				Oy) * MLIB_PREC_20);
			cz[cray] =
				(mlib_s32)((rays->starts[cray][2] -
				Oz) * MLIB_PREC_20);

			testx += mlib_fabs(rays->starts[cray][0] -
				rays->starts[cray - 1][0]);
			testy += mlib_fabs(rays->starts[cray][1] -
				rays->starts[cray - 1][1]);
			testz += mlib_fabs(rays->starts[cray][2] -
				rays->starts[cray - 1][2]);
		}

		OX = (mlib_s32)((Ox + 0.5) * MLIB_PREC_20);
		OY = (mlib_s32)((Oy + 0.5) * MLIB_PREC_20);
		OZ = (mlib_s32)((Oz + 0.5) * MLIB_PREC_20);

		go_to_steps = 0;

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

		vis_write_gsr(7);

		for (cstp = 0; cstp < minsteps; cstp++) {
			dp = (mlib_u8 *)rays->results[cstp];

			align = ((mlib_addr)dp & 0x7);

#pragma pipeloop(0)
			for (cray = 0; (cray < nrays) && (align != 0); cray++) {

				LOAD_SAVE_POINT_U8;
				align = ((mlib_addr)dp & 0x7);
			}

			dp_d64 = (mlib_d64 *)dp;
			count = (nrays - cray) >> 3;

#pragma pipeloop(0)
			for (i = 0, nray = cray + 7; i < count; i++) {
				LOAD_COMPRESS_U8_D64;
				nray--;

				LOAD_COMPRESS_U8_D64;
				nray--;

				LOAD_COMPRESS_U8_D64;
				nray--;

				LOAD_COMPRESS_U8_D64;
				nray--;

				LOAD_COMPRESS_U8_D64;
				nray--;

				LOAD_COMPRESS_U8_D64;
				nray--;

				LOAD_COMPRESS_U8_D64;
				nray--;

				LOAD_COMPRESS_U8_D64;
				nray += 15;

				(*dp_d64++) = P;
			}

			dp += (count << 3);
			cray += (count << 3);

#pragma pipeloop(0)
			for (; cray < nrays; cray++) {

				LOAD_SAVE_POINT_U8;
			}

			OX += dX;
			OY += dY;
			OZ += dZ;
		}

#pragma pipeloop(0)
		for (cstp = minsteps; cstp < maxsteps; cstp++) {
			dp = (mlib_u8 *)rays->results[cstp];

			for (cray = 0; cray < nrays; cray++) {

				if (cstp < rays->nsteps[cray]) {

					LOAD_SAVE_POINT_U8;
				} else {
					dp++;
				}
			}

			OX += dX;
			OY += dY;
			OZ += dZ;
		}
	} else {

#pragma pipeloop(0)
		for (cray = 0; cray < nrays; cray++) {
			OX = (rays->starts[cray][0] + 0.5) * MLIB_PREC_20;
			OY = (rays->starts[cray][1] + 0.5) * MLIB_PREC_20;
			OZ = (rays->starts[cray][2] + 0.5) * MLIB_PREC_20;
			X = OX & MLIB_MASK_20;
			Y = OY & MLIB_MASK_20;
			Z = OZ & MLIB_MASK_20;
			ZYX = Z;
			ZYX = (((ZYX << 22) + Y) << 13) + (X >> 9);

			nsteps = rays->nsteps[cray];

			for (cstp = 0; cstp < nsteps; cstp++) {
				index = vis_array8(ZYX, n);

				a = *(voxels + index);

				OX += dX;
				OY += dY;
				OZ += dZ;

				X = OX & MLIB_MASK_20;
				Y = OY & MLIB_MASK_20;
				Z = OZ & MLIB_MASK_20;
				ZYX = Z;
				ZYX = (((ZYX << 22) + Y) << 13) + (X >> 9);

				((mlib_u8 *)rays->results[cstp])[cray] = a;
			}
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_v_VolumeRayCast_Blocked_Parallel_Nearest_U8_U8_LittleSteps(
	mlib_rays *rays,
	const mlib_blkvolume *blk,
	mlib_s32 minsteps,
	mlib_s32 maxsteps)
{
	mlib_s32 xysize;
	mlib_s32 nrays, i, nray, count;
	mlib_s32 dX, dY, dZ;
	mlib_u8 *voxels, *dp, *sp;
	mlib_s32 nsteps, cray, cstp;
	mlib_u32 n, index, align;
	mlib_d64 Ox, Oy, Oz;
	mlib_s32 X, Y, Z, cx, cy, cz;
	mlib_u8 a;
	mlib_d64 go_to_rays, go_to_steps, testx, testy, testz;
	mlib_d64 P, P1;
	mlib_d64 *dp_d64;
	mlib_u64 dOZYX, OZYX, ZYX, cZYX[MAX_RAY_NUM];

	xysize = blk->xysize, nrays = rays->nrays;

	dX = (mlib_s32)(rays->incs[0][0] * MLIB_PREC);
	dY = (mlib_s32)(rays->incs[0][1] * MLIB_PREC);
	dZ = (mlib_s32)(rays->incs[0][2] * MLIB_PREC);

	if (dX < 0) {
		dX = dX & 0x3FFFFF;
		dY = dY - 1;
	}

	if (dY < 0) {
		dY = dY & 0x3FFFFF;
		dZ = dZ - 1;
	}

	dOZYX = dZ;
	dOZYX = (((dOZYX << 22) + dY) << 22) + dX;

	voxels = (mlib_u8 *)blk->voxels;

	i = xysize >> 6;
	n = 0;
	while (i >>= 1)
		n++;

	if (nrays <= MAX_RAY_NUM) {

		Ox = rays->starts[0][0];
		Oy = rays->starts[0][1];
		Oz = rays->starts[0][2];

		go_to_rays = 0;

		if (mlib_fabs(rays->incs[0][0]) / 12. > go_to_rays)
			go_to_rays = mlib_fabs(rays->incs[0][0] / 12.);

		if (mlib_fabs(rays->incs[0][1]) / 6. > go_to_rays)
			go_to_rays = mlib_fabs(rays->incs[0][1] / 6.);

		if (mlib_fabs(rays->incs[0][2]) > go_to_rays)
			go_to_rays = mlib_fabs(rays->incs[0][2]);

		cZYX[0] = 0;

		testx = 0;
		testy = 0;
		testz = 0;

		for (cray = 1; cray < nrays; cray++) {
			cx = (mlib_s32)((rays->starts[cray][0] -
				Ox) * MLIB_PREC);
			cy = (mlib_s32)((rays->starts[cray][1] -
				Oy) * MLIB_PREC);
			cz = (mlib_s32)((rays->starts[cray][2] -
				Oz) * MLIB_PREC);

			if (cx < 0) {
				cx = cx & 0x3FFFFF;
				cy = cy - 1;
			}

			if (cy < 0) {
				cy = cy & 0x3FFFFF;
				cz = cz - 1;
			}

			cZYX[cray] = cz;
			cZYX[cray] = (((cZYX[cray] << 22) + cy) << 22) + cx;

			testx += mlib_fabs(rays->starts[cray][0] -
				rays->starts[cray - 1][0]);
			testy += mlib_fabs(rays->starts[cray][1] -
				rays->starts[cray - 1][1]);
			testz += mlib_fabs(rays->starts[cray][2] -
				rays->starts[cray - 1][2]);
		}

		X = (mlib_s32)((Ox + 0.5) * MLIB_PREC);
		Y = (mlib_s32)((Oy + 0.5) * MLIB_PREC);
		Z = (mlib_s32)((Oz + 0.5) * MLIB_PREC);
		OZYX = Z;
		OZYX = (((OZYX << 22) + Y) << 22) + X;

		go_to_steps = 0;

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

		vis_write_gsr(7);

		for (cstp = 0; cstp < minsteps; cstp++) {
			dp = (mlib_u8 *)rays->results[cstp];

			align = ((mlib_addr)dp & 0x7);

#pragma pipeloop(0)
			for (cray = 0; (cray < nrays) && (align != 0); cray++) {
				ZYX = OZYX + cZYX[cray];
				index = vis_array8(ZYX & 0xFF800FFE003FF800, n);

				*dp = *(voxels + index);
				dp++;

				align = ((mlib_addr)dp & 0x7);
			}

			dp_d64 = (mlib_d64 *)dp;
			count = (nrays - cray) >> 3;

#pragma pipeloop(0)
			for (i = 0, nray = cray + 7; i < count; i++) {
				ZYX = OZYX + cZYX[nray];
				index = vis_array8(ZYX & 0xFF800FFE003FF800, n);
				nray--;
				sp = voxels + index;
				P1 = vis_ld_u8(sp);
				P = vis_faligndata(P1, P);

				ZYX = OZYX + cZYX[nray];
				index = vis_array8(ZYX & 0xFF800FFE003FF800, n);
				nray--;
				sp = voxels + index;
				P1 = vis_ld_u8(sp);
				P = vis_faligndata(P1, P);

				ZYX = OZYX + cZYX[nray];
				index = vis_array8(ZYX & 0xFF800FFE003FF800, n);
				nray--;
				sp = voxels + index;
				P1 = vis_ld_u8(sp);
				P = vis_faligndata(P1, P);

				ZYX = OZYX + cZYX[nray];
				index = vis_array8(ZYX & 0xFF800FFE003FF800, n);
				nray--;
				sp = voxels + index;
				P1 = vis_ld_u8(sp);
				P = vis_faligndata(P1, P);

				ZYX = OZYX + cZYX[nray];
				index = vis_array8(ZYX & 0xFF800FFE003FF800, n);
				nray--;
				sp = voxels + index;
				P1 = vis_ld_u8(sp);
				P = vis_faligndata(P1, P);

				ZYX = OZYX + cZYX[nray];
				index = vis_array8(ZYX & 0xFF800FFE003FF800, n);
				nray--;
				sp = voxels + index;
				P1 = vis_ld_u8(sp);
				P = vis_faligndata(P1, P);

				ZYX = OZYX + cZYX[nray];
				index = vis_array8(ZYX & 0xFF800FFE003FF800, n);
				nray--;
				sp = voxels + index;
				P1 = vis_ld_u8(sp);
				P = vis_faligndata(P1, P);

				ZYX = OZYX + cZYX[nray];
				index = vis_array8(ZYX & 0xFF800FFE003FF800, n);
				nray += 15;
				sp = voxels + index;
				P1 = vis_ld_u8(sp);
				P = vis_faligndata(P1, P);

				(*dp_d64++) = P;
			}

			dp += (count << 3);
			cray += (count << 3);

#pragma pipeloop(0)
			for (; cray < nrays; cray++) {
				ZYX = OZYX + cZYX[cray];
				index = vis_array8(ZYX & 0xFF800FFE003FF800, n);

				*dp = *(voxels + index);
				dp++;
			}

			OZYX += dOZYX;
		}

#pragma pipeloop(0)
		for (cstp = minsteps; cstp < maxsteps; cstp++) {
			dp = (mlib_u8 *)rays->results[cstp];

			for (cray = 0; cray < nrays; cray++) {

				if (cstp < rays->nsteps[cray]) {
					ZYX = OZYX + cZYX[cray];
					index = vis_array8(ZYX &
						0xFF800FFE003FF800, n);

					*dp = *(voxels + index);
					dp++;
				} else {
					dp++;
				}
			}

			OZYX += dOZYX;
		}
	} else {

#pragma pipeloop(0)
		for (cray = 0; cray < nrays; cray++) {
			X = (rays->starts[cray][0] + 0.5) * MLIB_PREC;
			Y = (rays->starts[cray][1] + 0.5) * MLIB_PREC;
			Z = (rays->starts[cray][2] + 0.5) * MLIB_PREC;
			ZYX = Z;
			ZYX = (((ZYX << 22) + Y) << 22) + X;

			nsteps = rays->nsteps[cray];

			for (cstp = 0; cstp < nsteps; cstp++) {
				index = vis_array8(ZYX & 0xFF800FFE003FF800, n);

				a = *(voxels + index);

				ZYX += dOZYX;

				((mlib_u8 *)rays->results[cstp])[cray] = a;
			}
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#define	FUN_NAME(STEP) \
	mlib_v_VolumeRayCast_Blocked_Parallel_Nearest_S16_S16_##STEP

mlib_status
__mlib_VolumeRayCast_Blocked_Parallel_Nearest_S16_S16(
	mlib_rays *rays,
	const mlib_blkvolume *blk,
	void *buffer)
{
	mlib_s32 nrays, cray;
	mlib_s32 minsteps, maxsteps;

	if (rays == NULL || blk == NULL)
		return (MLIB_NULLPOINTER);

	nrays = rays->nrays;

	minsteps = MLIB_S32_MAX;
	maxsteps = -1;

	for (cray = 0; cray < nrays; cray++) {
		if (rays->nsteps[cray] < minsteps)
			minsteps = rays->nsteps[cray];

		if (rays->nsteps[cray] > maxsteps)
			maxsteps = rays->nsteps[cray];
	}

	if (maxsteps > 2000) {
		return (
		FUN_NAME(ManySteps)
		(rays, blk, minsteps, maxsteps));
	} else {
		return (
		FUN_NAME(LittleSteps)
		(rays, blk, minsteps, maxsteps));
	}
}

/* *********************************************************** */

mlib_status
mlib_v_VolumeRayCast_Blocked_Parallel_Nearest_S16_S16_ManySteps(
	mlib_rays *rays,
	const mlib_blkvolume *blk,
	mlib_s32 minsteps,
	mlib_s32 maxsteps)
{
	mlib_s32 xysize;
	mlib_s32 nrays, i, nray, count;
	mlib_s32 dX, dY, dZ;
	mlib_s32 OX, OY, OZ;
	mlib_u16 *voxels, *dp, *sp;
	mlib_s32 nsteps, cray, cstp;
	mlib_u32 n, index, align;
	mlib_d64 Ox, Oy, Oz;
	mlib_s32 X, Y, Z, cx[MAX_RAY_NUM], cy[MAX_RAY_NUM], cz[MAX_RAY_NUM];
	mlib_u16 a;
	mlib_d64 go_to_rays, go_to_steps, testx, testy, testz;
	mlib_d64 P, P1;
	mlib_d64 *dp_d64;
	mlib_u64 ZYX;

	xysize = blk->xysize, nrays = rays->nrays;

	dX = (mlib_s32)(rays->incs[0][0] * MLIB_PREC_20);
	dY = (mlib_s32)(rays->incs[0][1] * MLIB_PREC_20);
	dZ = (mlib_s32)(rays->incs[0][2] * MLIB_PREC_20);

	voxels = (mlib_u16 *)blk->voxels;

	i = xysize >> 6;
	n = 0;
	while (i >>= 1)
		n++;

	if (nrays <= MAX_RAY_NUM) {

		Ox = rays->starts[0][0];
		Oy = rays->starts[0][1];
		Oz = rays->starts[0][2];

		go_to_rays = 0;

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
			cx[cray] =
				(mlib_s32)((rays->starts[cray][0] -
				Ox) * MLIB_PREC_20);
			cy[cray] =
				(mlib_s32)((rays->starts[cray][1] -
				Oy) * MLIB_PREC_20);
			cz[cray] =
				(mlib_s32)((rays->starts[cray][2] -
				Oz) * MLIB_PREC_20);

			testx += mlib_fabs(rays->starts[cray][0] -
				rays->starts[cray - 1][0]);
			testy += mlib_fabs(rays->starts[cray][1] -
				rays->starts[cray - 1][1]);
			testz += mlib_fabs(rays->starts[cray][2] -
				rays->starts[cray - 1][2]);
		}

		OX = (mlib_s32)((Ox + 0.5) * MLIB_PREC_20);
		OY = (mlib_s32)((Oy + 0.5) * MLIB_PREC_20);
		OZ = (mlib_s32)((Oz + 0.5) * MLIB_PREC_20);

		go_to_steps = 0;

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

		vis_write_gsr(6);

		for (cstp = 0; cstp < minsteps; cstp++) {
			dp = (mlib_u16 *)rays->results[cstp];

			align = ((mlib_addr)dp & 0x7);

#pragma pipeloop(0)
			for (cray = 0; (cray < nrays) && (align != 0); cray++) {

				LOAD_SAVE_POINT_U16;
				align = ((mlib_addr)dp & 0x7);
			}

			dp_d64 = (mlib_d64 *)dp;
			count = (nrays - cray) >> 2;

#pragma pipeloop(0)
			for (i = 0, nray = cray + 3; i < count; i++) {
				LOAD_COMPRESS_U16_D64;
				nray--;

				LOAD_COMPRESS_U16_D64;
				nray--;

				LOAD_COMPRESS_U16_D64;
				nray--;

				LOAD_COMPRESS_U16_D64;
				nray += 7;

				(*dp_d64++) = P;
			}

			dp += (count << 2);
			cray += (count << 2);

#pragma pipeloop(0)
			for (; cray < nrays; cray++) {

				LOAD_SAVE_POINT_U16;
			}

			OX += dX;
			OY += dY;
			OZ += dZ;
		}

#pragma pipeloop(0)
		for (cstp = minsteps; cstp < maxsteps; cstp++) {
			dp = (mlib_u16 *)rays->results[cstp];

			for (cray = 0; cray < nrays; cray++) {

				if (cstp < rays->nsteps[cray]) {

					LOAD_SAVE_POINT_U16;
				} else {
					dp++;
				}
			}

			OX += dX;
			OY += dY;
			OZ += dZ;
		}
	} else {

#pragma pipeloop(0)
		for (cray = 0; cray < nrays; cray++) {
			OX = (rays->starts[cray][0] + 0.5) * MLIB_PREC_20;
			OY = (rays->starts[cray][1] + 0.5) * MLIB_PREC_20;
			OZ = (rays->starts[cray][2] + 0.5) * MLIB_PREC_20;
			X = OX & MLIB_MASK_20;
			Y = OY & MLIB_MASK_20;
			Z = OZ & MLIB_MASK_20;
			ZYX = Z;
			ZYX = (((ZYX << 22) + Y) << 13) + (X >> 9);

			nsteps = rays->nsteps[cray];

			for (cstp = 0; cstp < nsteps; cstp++) {
				index = vis_array16(ZYX, n);

				a = *(mlib_u16 *)((mlib_u8 *)voxels + index);

				OX += dX;
				OY += dY;
				OZ += dZ;

				X = OX & MLIB_MASK_20;
				Y = OY & MLIB_MASK_20;
				Z = OZ & MLIB_MASK_20;
				ZYX = Z;
				ZYX = (((ZYX << 22) + Y) << 13) + (X >> 9);

				((mlib_u16 *)rays->results[cstp])[cray] = a;
			}
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_v_VolumeRayCast_Blocked_Parallel_Nearest_S16_S16_LittleSteps(
	mlib_rays *rays,
	const mlib_blkvolume *blk,
	mlib_s32 minsteps,
	mlib_s32 maxsteps)
{
	mlib_s32 xysize;
	mlib_s32 nrays, i, nray, count;
	mlib_s32 dX, dY, dZ;
	mlib_u16 *voxels, *dp, *sp;
	mlib_s32 nsteps, cray, cstp;
	mlib_u32 n, index, align;
	mlib_d64 Ox, Oy, Oz;
	mlib_s32 X, Y, Z, cx, cy, cz;
	mlib_u16 a;
	mlib_d64 go_to_rays, go_to_steps, testx, testy, testz;
	mlib_d64 P, P1;
	mlib_d64 *dp_d64;
	mlib_u64 dOZYX, OZYX, ZYX, cZYX[MAX_RAY_NUM];

	xysize = blk->xysize, nrays = rays->nrays;

	dX = (mlib_s32)(rays->incs[0][0] * MLIB_PREC);
	dY = (mlib_s32)(rays->incs[0][1] * MLIB_PREC);
	dZ = (mlib_s32)(rays->incs[0][2] * MLIB_PREC);

	if (dX < 0) {
		dX = dX & 0x3FFFFF;
		dY = dY - 1;
	}

	if (dY < 0) {
		dY = dY & 0x3FFFFF;
		dZ = dZ - 1;
	}

	dOZYX = dZ;
	dOZYX = (((dOZYX << 22) + dY) << 22) + dX;

	voxels = (mlib_u16 *)blk->voxels;

	i = xysize >> 6;
	n = 0;
	while (i >>= 1)
		n++;

	if (nrays <= MAX_RAY_NUM) {

		Ox = rays->starts[0][0];
		Oy = rays->starts[0][1];
		Oz = rays->starts[0][2];

		go_to_rays = 0;

		if (mlib_fabs(rays->incs[0][0]) / 6. > go_to_rays)
			go_to_rays = mlib_fabs(rays->incs[0][0] / 6.);

		if (mlib_fabs(rays->incs[0][1]) / 6. > go_to_rays)
			go_to_rays = mlib_fabs(rays->incs[0][1] / 6.);

		if (mlib_fabs(rays->incs[0][2]) > go_to_rays)
			go_to_rays = mlib_fabs(rays->incs[0][2]);

		cZYX[0] = 0;

		testx = 0;
		testy = 0;
		testz = 0;

		for (cray = 1; cray < nrays; cray++) {
			cx = (mlib_s32)((rays->starts[cray][0] -
				Ox) * MLIB_PREC);
			cy = (mlib_s32)((rays->starts[cray][1] -
				Oy) * MLIB_PREC);
			cz = (mlib_s32)((rays->starts[cray][2] -
				Oz) * MLIB_PREC);

			if (cx < 0) {
				cx = cx & 0x3FFFFF;
				cy = cy - 1;
			}

			if (cy < 0) {
				cy = cy & 0x3FFFFF;
				cz = cz - 1;
			}

			cZYX[cray] = cz;
			cZYX[cray] = (((cZYX[cray] << 22) + cy) << 22) + cx;

			testx += mlib_fabs(rays->starts[cray][0] -
				rays->starts[cray - 1][0]);
			testy += mlib_fabs(rays->starts[cray][1] -
				rays->starts[cray - 1][1]);
			testz += mlib_fabs(rays->starts[cray][2] -
				rays->starts[cray - 1][2]);
		}

		X = (mlib_s32)((Ox + 0.5) * MLIB_PREC);
		Y = (mlib_s32)((Oy + 0.5) * MLIB_PREC);
		Z = (mlib_s32)((Oz + 0.5) * MLIB_PREC);
		OZYX = Z;
		OZYX = (((OZYX << 22) + Y) << 22) + X;

		go_to_steps = 0;

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

		vis_write_gsr(6);

		for (cstp = 0; cstp < minsteps; cstp++) {
			dp = (mlib_u16 *)rays->results[cstp];

			align = ((mlib_addr)dp & 0x7);

#pragma pipeloop(0)
			for (cray = 0; (cray < nrays) && (align != 0); cray++) {
				ZYX = OZYX + cZYX[cray];
				index = vis_array16(ZYX & 0xFF800FFE003FF800,
					n);

				*dp = *(mlib_u16 *)((mlib_u8 *)voxels + index);
				dp++;

				align = ((mlib_addr)dp & 0x7);
			}

			dp_d64 = (mlib_d64 *)dp;
			count = (nrays - cray) >> 2;

#pragma pipeloop(0)
			for (i = 0, nray = cray + 3; i < count; i++) {
				ZYX = OZYX + cZYX[nray];
				index = vis_array16(ZYX & 0xFF800FFE003FF800,
					n);
				nray--;
				sp = (mlib_u16 *)((mlib_u8 *)voxels + index);
				P1 = vis_ld_u16(sp);
				P = vis_faligndata(P1, P);

				ZYX = OZYX + cZYX[nray];
				index = vis_array16(ZYX & 0xFF800FFE003FF800,
					n);
				nray--;
				sp = (mlib_u16 *)((mlib_u8 *)voxels + index);
				P1 = vis_ld_u16(sp);
				P = vis_faligndata(P1, P);

				ZYX = OZYX + cZYX[nray];
				index = vis_array16(ZYX & 0xFF800FFE003FF800,
					n);
				nray--;
				sp = (mlib_u16 *)((mlib_u8 *)voxels + index);
				P1 = vis_ld_u16(sp);
				P = vis_faligndata(P1, P);

				ZYX = OZYX + cZYX[nray];
				index = vis_array16(ZYX & 0xFF800FFE003FF800,
					n);
				nray += 7;
				sp = (mlib_u16 *)((mlib_u8 *)voxels + index);
				P1 = vis_ld_u16(sp);
				P = vis_faligndata(P1, P);

				(*dp_d64++) = P;
			}

			dp += (count << 2);
			cray += (count << 2);

#pragma pipeloop(0)
			for (; cray < nrays; cray++) {
				ZYX = OZYX + cZYX[cray];
				index = vis_array16(ZYX & 0xFF800FFE003FF800,
					n);

				*dp = *(mlib_u16 *)((mlib_u8 *)voxels + index);
				dp++;
			}

			OZYX += dOZYX;
		}

#pragma pipeloop(0)
		for (cstp = minsteps; cstp < maxsteps; cstp++) {
			dp = (mlib_u16 *)rays->results[cstp];

			for (cray = 0; cray < nrays; cray++) {

				if (cstp < rays->nsteps[cray]) {
					ZYX = OZYX + cZYX[cray];
					index = vis_array16(ZYX &
						0xFF800FFE003FF800, n);

					*dp = *(mlib_u16 *)((mlib_u8 *)voxels +
						index);
					dp++;
				} else {
					dp++;
				}
			}

			OZYX += dOZYX;
		}
	} else {

#pragma pipeloop(0)
		for (cray = 0; cray < nrays; cray++) {
			X = (rays->starts[cray][0] + 0.5) * MLIB_PREC;
			Y = (rays->starts[cray][1] + 0.5) * MLIB_PREC;
			Z = (rays->starts[cray][2] + 0.5) * MLIB_PREC;
			ZYX = Z;
			ZYX = (((ZYX << 22) + Y) << 22) + X;

			nsteps = rays->nsteps[cray];

			for (cstp = 0; cstp < nsteps; cstp++) {
				index = vis_array16(ZYX & 0xFF800FFE003FF800,
					n);

				a = *(mlib_u16 *)((mlib_u8 *)voxels + index);

				ZYX += dOZYX;

				((mlib_u16 *)rays->results[cstp])[cray] = a;
			}
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
