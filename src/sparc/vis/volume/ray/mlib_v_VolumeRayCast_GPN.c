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

#pragma ident	"@(#)mlib_v_VolumeRayCast_GPN.c	9.3	07/11/05 SMI"

/*
 *  FUNCTION
 *    mlib_VolumeRayCast_General_Parallel_Nearest_[U8_U8|S16_S16]
 *      Cast parallel rays through a three dimensional data set
 *      and return the interpolated samples.
 *
 *  SYNOPSIS
 *    mlib_status mlib_VolumeRayCast_General_Parallel_Nearest_[U8_U8|S16_S16]
 *                                           (const mlib_rays      *rays,
 *                                            const mlib_genvolume *vol,
 *                                            void                 *buffer)
 *
 *  ARGUMENTS
 *    rays      Casting rays.
 *    vol       Volume data that consists of slices.
 *    buffer    Working buffer.
 *
 *  DESCRIPTION
 *    In nearest neighbor operation, the sample value at point P
 *    is replaced with the value of the nearest neighbor voxel.
 */

#include <mlib_volume.h>
#include <mlib_SysMath.h>
#include <vis_proto.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_VolumeRayCast_General_Parallel_Nearest_S16_S16 = \
__mlib_VolumeRayCast_General_Parallel_Nearest_S16_S16
#pragma weak mlib_VolumeRayCast_General_Parallel_Nearest_U8_U8 = \
	__mlib_VolumeRayCast_General_Parallel_Nearest_U8_U8
#elif defined(__GNUC__)

__typeof__(__mlib_VolumeRayCast_General_Parallel_Nearest_S16_S16)
	mlib_VolumeRayCast_General_Parallel_Nearest_S16_S16
	__attribute__((weak,
	alias("__mlib_VolumeRayCast_General_Parallel_Nearest_S16_S16")));
__typeof__(__mlib_VolumeRayCast_General_Parallel_Nearest_U8_U8)
	mlib_VolumeRayCast_General_Parallel_Nearest_U8_U8
	__attribute__((weak,
	alias("__mlib_VolumeRayCast_General_Parallel_Nearest_U8_U8")));

#else

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	MAX_RAY_NUM	64
#define	STEPS_LIMIT	512
#define	REGION_LIMIT	512
#define	SLICE_LIMIT	512
#define	MLIB_SHIFT	21
#define	MLIB_PREC	(1 << MLIB_SHIFT)
#define	MLIB_ROUND	(1 << (MLIB_SHIFT-1))
#define	NSTEP_SHIFT	6
#define	NRAY_MASK	((1 << NSTEP_SHIFT) - 1)

/* *********************************************************** */

#define	ALLOC_MEM                                                         \
	if (maxsteps > STEPS_LIMIT) {                                     \
	    bufptr = (bufData *) __mlib_malloc(MAX_RAY_NUM * maxsteps *   \
		    BUFSIZE);                                             \
	    if (bufptr == NULL)                                           \
		return (MLIB_FAILURE);                                    \
	} else                                                            \
	    bufptr = bufArray

/* *********************************************************** */

#define	FREE_MEM                                                \
	if (maxsteps > STEPS_LIMIT)                             \
	    __mlib_free(bufptr)

/* *********************************************************** */

typedef struct
{
	mlib_u32 nPrev;
	mlib_u16 xSrc;
	mlib_u16 ySrc;
} bufData;

#define	BUFSIZE	sizeof (bufData)

/* *********************************************************** */

static mlib_status
mlib_v_VolumeRayCast_General_Parallel_Nearest_S16_S16_OneRegion(
	mlib_rays *rays,
	mlib_s32 nrays,
	mlib_s32 minsteps,
	mlib_s32 maxsteps,
	const mlib_genvolume *vol);

static mlib_status
mlib_v_VolumeRayCast_General_Parallel_Nearest_S16_S16_MultiRegion(
	mlib_rays *rays,
	mlib_s32 nrays,
	mlib_s32 maxsteps,
	const mlib_genvolume *vol);

static mlib_status
mlib_v_VolumeRayCast_General_Parallel_Nearest_U8_U8_OneRegion(
	mlib_rays *rays,
	mlib_s32 nrays,
	mlib_s32 minsteps,
	mlib_s32 maxsteps,
	const mlib_genvolume *vol);

static mlib_status
mlib_v_VolumeRayCast_General_Parallel_Nearest_U8_U8_MultiRegion(
	mlib_rays *rays,
	mlib_s32 nrays,
	mlib_s32 maxsteps,
	const mlib_genvolume *vol);

/* *********************************************************** */

mlib_status
__mlib_VolumeRayCast_General_Parallel_Nearest_U8_U8(
	mlib_rays *rays,
	const mlib_genvolume *vol,
	void *buffer)
{
	mlib_s32 cray;
	mlib_s32 nregs, nrays, nsls;
	mlib_s32 minsteps = MLIB_S32_MAX;
	mlib_s32 maxsteps = -1;
	mlib_d64 maxdist = 0.0, zdelta;

	if (rays == NULL || vol == NULL)
		return (MLIB_NULLPOINTER);

	nregs = vol->nregions;
	nsls = vol->nslices;
	nrays = rays->nrays;

	if (nregs > REGION_LIMIT || nsls > SLICE_LIMIT || nrays > MAX_RAY_NUM)
		return (MLIB_OUTOFRANGE);

/*
 * loop to find the minsteps and maxsteps
 */
	for (cray = 0; cray < nrays; cray++) {
		if (rays->nsteps[cray] < minsteps)
			minsteps = rays->nsteps[cray];

		if (rays->nsteps[cray] > maxsteps)
			maxsteps = rays->nsteps[cray];

		if (mlib_fabs(rays->starts[cray][2] - rays->starts[0][2]) >
			maxdist)
			maxdist =
				mlib_fabs(rays->starts[cray][2] -
				rays->starts[0][2]);
	}

	zdelta = (vol->zcoords[nregs] - vol->zcoords[0]) * 0.20;

	if (nregs == 1 && maxdist < zdelta) {
		return (
		mlib_v_VolumeRayCast_General_Parallel_Nearest_U8_U8_OneRegion
		(rays, nrays, minsteps, maxsteps, vol));
	} else {
		return (
		mlib_v_VolumeRayCast_General_Parallel_Nearest_U8_U8_MultiRegion
		(rays, nrays, maxsteps, vol));
	}
}

/* *********************************************************** */

mlib_status
mlib_v_VolumeRayCast_General_Parallel_Nearest_U8_U8_OneRegion(
	mlib_rays *rays,
	mlib_s32 nrays,
	mlib_s32 minsteps,
	mlib_s32 maxsteps,
	const mlib_genvolume *vol)
{
	mlib_s32 i;
	mlib_s32 cray, nray;
	mlib_s32 cstp;
	mlib_s32 count, align;

/* coordinates of current voxel on home ray */

	mlib_d64 x, y, z;

/* increment triple of home ray */

	mlib_d64 dx, dy, dz;

	mlib_s32 X, Y, Z;
	mlib_s32 currX, currY, currZ;
/*
 * coordinate differences vs
 * the voxel in home ray, for
 * current step
 */
	mlib_s32 cx[64], cy[64], cz[64];

/* integer parts of the coordinates */

	mlib_s32 Xint, Yint, Zint;

/* the nearest neighbor voxel's value */

	mlib_d64 P1;

/* interpolated voxels' values */

	mlib_d64 P;

/* the nearest neighbor voxel's value */

	mlib_u8 sd;

/* pointer to source data */

	mlib_u8 *sp;

/* pointer to destination data */

	mlib_u8 *dp;

/* pointer to destination data */

	mlib_d64 *dp_d64;

	dx = rays->incs[0][0];
	dy = rays->incs[0][1];
	dz = rays->incs[0][2] * vol->zsrates[0];

	for (cray = 0; cray < nrays; cray++) {
		cx[cray] =
			(mlib_s32)((rays->starts[cray][0] -
			rays->starts[0][0]) * MLIB_PREC) + MLIB_ROUND;
		cy[cray] =
			(mlib_s32)((rays->starts[cray][1] -
			rays->starts[0][1]) * MLIB_PREC) + MLIB_ROUND;
		cz[cray] =
			(mlib_s32)(((rays->starts[cray][2] -
			rays->starts[0][2]) * vol->zsrates[0]) * MLIB_PREC) +
			MLIB_ROUND;
	}

	x = rays->starts[0][0];
	y = rays->starts[0][1];
	z = (rays->starts[0][2] - vol->zcoords[0]) * vol->zsrates[0];

/* for combining data with vis_faligndata() */

	vis_write_gsr(7);

	for (cstp = 0; cstp < minsteps; cstp++) {
		dp = (mlib_u8 *)rays->results[cstp];

/*
 * create fixed point versions of x, y, z
 */
		X = (mlib_s32)(x * MLIB_PREC);
		Y = (mlib_s32)(y * MLIB_PREC);
		Z = (mlib_s32)(z * MLIB_PREC);

		align = ((mlib_addr)dp & 0x7);

#pragma pipeloop(0)
		for (cray = 0; (cray < nrays) && (align != 0); cray++) {
/*
 * calculate the coordinates of the current voxel
 */
			currX = X + cx[cray];
			currY = Y + cy[cray];
			currZ = Z + cz[cray];

/*
 * get the coordinates of the nearest neighbor
 */
			Xint = currX >> MLIB_SHIFT;
			Yint = currY >> MLIB_SHIFT;
			Zint = currZ >> MLIB_SHIFT;

/*
 * read the nearest neighbor voxel from the slice dataset
 */
			sp = ((mlib_u8 *)vol->slices[Zint] +
				vol->offsets[Yint]) + Xint;
			sd = *sp;

/*
 * store the interpolated voxel
 */
			(*dp++) = sd;

			align = ((mlib_addr)dp & 0x7);
		}

		dp_d64 = (mlib_d64 *)dp;
		count = (nrays - cray) >> 3;

#pragma pipeloop(0)
		for (i = 0, nray = cray + 7; i < count; i++) {
/*
 * calculate the coordinates of the 8th voxel
 */
			currX = X + cx[nray];
			currY = Y + cy[nray];
			currZ = Z + cz[nray];
			nray--;

/*
 * get the coordinates of the nearest neighbor
 */
			Xint = currX >> MLIB_SHIFT;
			Yint = currY >> MLIB_SHIFT;
			Zint = currZ >> MLIB_SHIFT;

/*
 * read the nearest neighbor voxel from the slice dataset
 */
			sp = ((mlib_u8 *)vol->slices[Zint] +
				vol->offsets[Yint]) + Xint;
			P1 = vis_ld_u8(sp);
			P = vis_faligndata(P1, P);

/*
 * calculate the coordinates of the 7th voxel
 */
			currX = X + cx[nray];
			currY = Y + cy[nray];
			currZ = Z + cz[nray];
			nray--;

/*
 * get the coordinates of the nearest neighbor
 */
			Xint = currX >> MLIB_SHIFT;
			Yint = currY >> MLIB_SHIFT;
			Zint = currZ >> MLIB_SHIFT;

/*
 * read the nearest neighbor voxel from the slice dataset
 */
			sp = ((mlib_u8 *)vol->slices[Zint] +
				vol->offsets[Yint]) + Xint;
			P1 = vis_ld_u8(sp);
			P = vis_faligndata(P1, P);

/*
 * calculate the coordinates of the 6th voxel
 */
			currX = X + cx[nray];
			currY = Y + cy[nray];
			currZ = Z + cz[nray];
			nray--;

/*
 * get the coordinates of the nearest neighbor
 */
			Xint = currX >> MLIB_SHIFT;
			Yint = currY >> MLIB_SHIFT;
			Zint = currZ >> MLIB_SHIFT;

/*
 * read the nearest neighbor voxel from the slice dataset
 */
			sp = ((mlib_u8 *)vol->slices[Zint] +
				vol->offsets[Yint]) + Xint;
			P1 = vis_ld_u8(sp);
			P = vis_faligndata(P1, P);

/*
 * calculate the coordinates of the 5th voxel
 */
			currX = X + cx[nray];
			currY = Y + cy[nray];
			currZ = Z + cz[nray];
			nray--;

/*
 * get the coordinates of the nearest neighbor
 */
			Xint = currX >> MLIB_SHIFT;
			Yint = currY >> MLIB_SHIFT;
			Zint = currZ >> MLIB_SHIFT;

/*
 * read the nearest neighbor voxel from the slice dataset
 */
			sp = ((mlib_u8 *)vol->slices[Zint] +
				vol->offsets[Yint]) + Xint;
			P1 = vis_ld_u8(sp);
			P = vis_faligndata(P1, P);

/*
 * calculate the coordinates of the 4th voxel
 */
			currX = X + cx[nray];
			currY = Y + cy[nray];
			currZ = Z + cz[nray];
			nray--;

/*
 * get the coordinates of the nearest neighbor
 */
			Xint = currX >> MLIB_SHIFT;
			Yint = currY >> MLIB_SHIFT;
			Zint = currZ >> MLIB_SHIFT;

/*
 * read the nearest neighbor voxel from the slice dataset
 */
			sp = ((mlib_u8 *)vol->slices[Zint] +
				vol->offsets[Yint]) + Xint;
			P1 = vis_ld_u8(sp);
			P = vis_faligndata(P1, P);

/*
 * calculate the coordinates of the 3rd voxel
 */
			currX = X + cx[nray];
			currY = Y + cy[nray];
			currZ = Z + cz[nray];
			nray--;

/*
 * get the coordinates of the nearest neighbor
 */
			Xint = currX >> MLIB_SHIFT;
			Yint = currY >> MLIB_SHIFT;
			Zint = currZ >> MLIB_SHIFT;

/*
 * read the nearest neighbor voxel from the slice dataset
 */
			sp = ((mlib_u8 *)vol->slices[Zint] +
				vol->offsets[Yint]) + Xint;
			P1 = vis_ld_u8(sp);
			P = vis_faligndata(P1, P);

/*
 * calculate the coordinates of the 2nd voxel
 */
			currX = X + cx[nray];
			currY = Y + cy[nray];
			currZ = Z + cz[nray];
			nray--;

/*
 * get the coordinates of the nearest neighbor
 */
			Xint = currX >> MLIB_SHIFT;
			Yint = currY >> MLIB_SHIFT;
			Zint = currZ >> MLIB_SHIFT;

/*
 * read the nearest neighbor voxel from the slice dataset
 */
			sp = ((mlib_u8 *)vol->slices[Zint] +
				vol->offsets[Yint]) + Xint;
			P1 = vis_ld_u8(sp);
			P = vis_faligndata(P1, P);

/*
 * calculate the coordinates of the 1st voxel
 */
			currX = X + cx[nray];
			currY = Y + cy[nray];
			currZ = Z + cz[nray];
			nray += 15;

/*
 * get the coordinates of the nearest neighbor
 */
			Xint = currX >> MLIB_SHIFT;
			Yint = currY >> MLIB_SHIFT;
			Zint = currZ >> MLIB_SHIFT;

/*
 * read the nearest neighbor voxel from the slice dataset
 */
			sp = ((mlib_u8 *)vol->slices[Zint] +
				vol->offsets[Yint]) + Xint;
			P1 = vis_ld_u8(sp);
			P = vis_faligndata(P1, P);

/*
 * store the 8 interpolated voxels
 */
			(*dp_d64++) = P;
		}

		dp += (count << 3);
		cray += (count << 3);

#pragma pipeloop(0)
		for (; cray < nrays; cray++) {
/*
 * calculate the coordinates of the current voxel
 */
			currX = X + cx[cray];
			currY = Y + cy[cray];
			currZ = Z + cz[cray];

/*
 * get the coordinates of the nearest neighbor
 */
			Xint = currX >> MLIB_SHIFT;
			Yint = currY >> MLIB_SHIFT;
			Zint = currZ >> MLIB_SHIFT;

/*
 * read the nearest neighbor voxel from the slice dataset
 */
			sp = ((mlib_u8 *)vol->slices[Zint] +
				vol->offsets[Yint]) + Xint;
			sd = *sp;

/*
 * store the interpolated voxel
 */
			(*dp++) = sd;
		}

/*
 * advance along the home ray
 */
		x += dx;
		y += dy;
		z += dz;
	}

/*
 * loop for handling the remained voxels
 */
	for (cstp = minsteps; cstp < maxsteps; cstp++) {
/*
 * prepare the pointer to destination
 */
		dp = (mlib_u8 *)rays->results[cstp];

/*
 * create fixed point versions of x, y, z
 */
		X = (mlib_s32)(x * MLIB_PREC);
		Y = (mlib_s32)(y * MLIB_PREC);
		Z = (mlib_s32)(z * MLIB_PREC);

#pragma pipeloop(0)
		for (cray = 0; cray < nrays; cray++) {
			if (cstp < rays->nsteps[cray]) {
/*
 * calculate the coordinates of the current voxel
 */
				currX = X + cx[cray];
				currY = Y + cy[cray];
				currZ = Z + cz[cray];

/*
 * get the coordinates of the nearest neighbor
 */
				Xint = currX >> MLIB_SHIFT;
				Yint = currY >> MLIB_SHIFT;
				Zint = currZ >> MLIB_SHIFT;

/*
 * read the nearest neighbor voxel from the slice
 * dataset
 */
				sp = (mlib_u8 *)((mlib_u8 *)vol->slices[Zint] +
					vol->offsets[Yint]) + Xint;
				sd = *sp;

/*
 * store the interpolated voxel
 */
				(*dp++) = sd;
			} else {
				dp++;
			}
		}

/*
 * advance along the home ray
 */
		x += dx;
		y += dy;
		z += dz;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_v_VolumeRayCast_General_Parallel_Nearest_U8_U8_MultiRegion(
	mlib_rays *rays,
	mlib_s32 nrays,
	mlib_s32 maxsteps,
	const mlib_genvolume *vol)
{
	mlib_u8 **slices = (mlib_u8 **)vol->slices;
	mlib_s32 *offsets = vol->offsets;
	mlib_s32 nslices = vol->nslices;
	mlib_s32 nregions = vol->nregions;
	mlib_s32 zcoords[REGION_LIMIT];
	mlib_s32 *zindices = vol->zindices;
	mlib_d64 *zsrates = vol->zsrates;
	mlib_u8 **results = (mlib_u8 **)rays->results;
	mlib_s32 *nsteps = rays->nsteps;
	mlib_u32 sliceLst[SLICE_LIMIT];
	bufData *bufptr;
	bufData bufArray[STEPS_LIMIT * MAX_RAY_NUM];
	mlib_u32 nstep, nray, indx;
	mlib_u8 *slicePtr;
	mlib_u8 *resultPtr;
	mlib_s32 nRaySteps;
	mlib_s32 nregion, nslice, zindex;
	mlib_s32 xsrc, ysrc;
	mlib_d64 zsrate;
	mlib_s32 zcoord, ssZ;
	mlib_s32 X, Y, Z;
	mlib_s32 dX = (mlib_s32)(rays->incs[0][0] * MLIB_PREC);
	mlib_s32 dY = (mlib_s32)(rays->incs[0][1] * MLIB_PREC);
	mlib_s32 dZ = (mlib_s32)(rays->incs[0][2] * MLIB_PREC);
	mlib_s32 dssZ;
	mlib_s32 i, j;

	for (i = 0; i <= nregions; i++)
		zcoords[i] = vol->zcoords[i] * MLIB_PREC;

	for (i = 0; i < nslices; i++)
		sliceLst[i] = MLIB_U32_MAX;

	ALLOC_MEM;

	if (nregions == 1) {
		for (i = 0; i < nrays; i++) {
			X = (rays->starts[i][0] + 0.5) * MLIB_PREC;
			Y = (rays->starts[i][1] + 0.5) * MLIB_PREC;
			Z = rays->starts[i][2] * MLIB_PREC;

			nRaySteps = nsteps[i];

			nregion = 0;
			zsrate = zsrates[0];
			zcoord = zcoords[0];
			zindex = zindices[0];

			ssZ = (Z - zcoord) * zsrate;
			dssZ = dZ * zsrate;

			for (j = 0; j < nRaySteps; j++) {
				nslice = zindex + ((ssZ +
					MLIB_ROUND) >> MLIB_SHIFT);

				xsrc = (X >> MLIB_SHIFT);
				ysrc = (Y >> MLIB_SHIFT);

				indx = (j << NSTEP_SHIFT) + i;

				bufptr[indx].nPrev = sliceLst[nslice];
				bufptr[indx].xSrc = (mlib_u16)xsrc;
				bufptr[indx].ySrc = (mlib_u16)ysrc;

				sliceLst[nslice] = indx;

				X += dX;
				Y += dY;

				ssZ += dssZ;
			}
		}
	} else {
		if (dZ > 0) {
			for (i = 0; i < nrays; i++) {
				X = (rays->starts[i][0] + 0.5) * MLIB_PREC;
				Y = (rays->starts[i][1] + 0.5) * MLIB_PREC;
				Z = rays->starts[i][2] * MLIB_PREC;

				nRaySteps = nsteps[i];

				nregion = 0;
				zsrate = zsrates[0];
				zcoord = zcoords[0];
				zindex = zindices[0];

				ssZ = (Z - zcoord) * zsrate;
				dssZ = dZ * zsrate;

				for (j = 0; j < nRaySteps; j++) {
					for (; Z > zcoords[nregion + 1]; ) {
						nregion++;
						zsrate = zsrates[nregion];
						zcoord = zcoords[nregion];
						zindex = zindices[nregion];

						ssZ = (Z - zcoord) * zsrate;
						dssZ = dZ * zsrate;
					}

					nslice = zindex + ((ssZ +
						MLIB_ROUND) >> MLIB_SHIFT);

					xsrc = (X >> MLIB_SHIFT);
					ysrc = (Y >> MLIB_SHIFT);

					indx = (j << NSTEP_SHIFT) + i;

					bufptr[indx].nPrev = sliceLst[nslice];
					bufptr[indx].xSrc = (mlib_u16)xsrc;
					bufptr[indx].ySrc = (mlib_u16)ysrc;

					sliceLst[nslice] = indx;

					X += dX;
					Y += dY;
					Z += dZ;

					ssZ += dssZ;
				}
			}
		} else {
			for (i = 0; i < nrays; i++) {
				X = (rays->starts[i][0] + 0.5) * MLIB_PREC;
				Y = (rays->starts[i][1] + 0.5) * MLIB_PREC;
				Z = rays->starts[i][2] * MLIB_PREC;

				nRaySteps = nsteps[i];
				resultPtr = results[i];

				nregion = nregions - 1;
				zsrate = zsrates[nregions - 1];
				zcoord = zcoords[nregions - 1];
				zindex = zindices[nregions - 1];

				ssZ = (Z - zcoord) * zsrate;
				dssZ = dZ * zsrate;

				for (j = 0; j < nRaySteps; j++) {
					for (; Z < zcoords[nregion]; ) {
						nregion--;
						zsrate = zsrates[nregion];
						zcoord = zcoords[nregion];
						zindex = zindices[nregion];

						ssZ = (Z - zcoord) * zsrate;
						dssZ = dZ * zsrate;
					}

					nslice = zindex + ((ssZ +
						MLIB_ROUND) >> MLIB_SHIFT);

					xsrc = (X >> MLIB_SHIFT);
					ysrc = (Y >> MLIB_SHIFT);

					indx = (j << NSTEP_SHIFT) + i;

					bufptr[indx].nPrev = sliceLst[nslice];
					bufptr[indx].xSrc = (mlib_u16)xsrc;
					bufptr[indx].ySrc = (mlib_u16)ysrc;

					sliceLst[nslice] = indx;

					X += dX;
					Y += dY;
					Z += dZ;

					ssZ += dssZ;
				}
			}
		}
	}

	for (i = 0; i < nslices; i++) {
		slicePtr = slices[i];
		indx = sliceLst[i];
		nstep = (indx >> 6);
		nray = (indx & 0x3F);

		for (; indx != MLIB_U32_MAX; ) {
			xsrc = bufptr[indx].xSrc;
			ysrc = bufptr[indx].ySrc;

			resultPtr = results[nstep];
			resultPtr[nray] = slicePtr[offsets[ysrc] + xsrc];

			indx = bufptr[indx].nPrev;
			nstep = (indx >> NSTEP_SHIFT);
			nray = (indx & NRAY_MASK);
		}
	}

	FREE_MEM;
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#define	FUN_NAME(REG) \
	mlib_v_VolumeRayCast_General_Parallel_Nearest_S16_S16_##REG

mlib_status
__mlib_VolumeRayCast_General_Parallel_Nearest_S16_S16(
	mlib_rays *rays,
	const mlib_genvolume *vol,
	void *buffer)
{
	mlib_s32 cray;
	mlib_s32 nregs, nrays, nsls;
	mlib_s32 minsteps = MLIB_S32_MAX;
	mlib_s32 maxsteps = -1;
	mlib_d64 maxdist = 0.0, zdelta;

	if (rays == NULL || vol == NULL)
		return (MLIB_NULLPOINTER);

	nregs = vol->nregions;
	nsls = vol->nslices;
	nrays = rays->nrays;

	if (nregs > REGION_LIMIT || nsls > SLICE_LIMIT || nrays > MAX_RAY_NUM)
		return (MLIB_OUTOFRANGE);

/*
 * loop to find the minsteps and maxsteps
 */
	for (cray = 0; cray < nrays; cray++) {
		if (rays->nsteps[cray] < minsteps)
			minsteps = rays->nsteps[cray];

		if (rays->nsteps[cray] > maxsteps)
			maxsteps = rays->nsteps[cray];

		if (mlib_fabs(rays->starts[cray][2] - rays->starts[0][2]) >
			maxdist)
			maxdist =
				mlib_fabs(rays->starts[cray][2] -
				rays->starts[0][2]);
	}

	zdelta = (vol->zcoords[nregs] - vol->zcoords[0]) * 0.20;

	if (nregs == 1 && maxdist < zdelta) {
		return (
		FUN_NAME(OneRegion)
		(rays, nrays, minsteps, maxsteps, vol));
	} else {
		return (
		FUN_NAME(MultiRegion)
		(rays, nrays, maxsteps, vol));
	}
}

/* *********************************************************** */

mlib_status
mlib_v_VolumeRayCast_General_Parallel_Nearest_S16_S16_OneRegion(
	mlib_rays *rays,
	mlib_s32 nrays,
	mlib_s32 minsteps,
	mlib_s32 maxsteps,
	const mlib_genvolume *vol)
{
	mlib_s32 i;
	mlib_s32 cray, nray;
	mlib_s32 cstp;
	mlib_s32 count, align;

/* coordinates of current voxel on home ray */

	mlib_d64 x, y, z;

/* increment triple of home ray */

	mlib_d64 dx, dy, dz;

	mlib_s32 X, Y, Z;
	mlib_s32 currX, currY, currZ;
/*
 * coordinate differences vs
 * the voxel in home ray, for
 * current step
 */
	mlib_s32 cx[64], cy[64], cz[64];

/* integer parts of the coordinates */

	mlib_s32 Xint, Yint, Zint;

/* the nearest neighbor voxel's value */

	mlib_d64 P1;

/* interpolated voxels' values */

	mlib_d64 P;

/* the nearest neighbor voxel's value */

	mlib_s16 sd;

/* pointer to source data */

	mlib_s16 *sp;

/* pointer to destination data */

	mlib_s16 *dp;

/* pointer to destination data */

	mlib_d64 *dp_d64;

	dx = rays->incs[0][0];
	dy = rays->incs[0][1];
	dz = rays->incs[0][2] * vol->zsrates[0];

	for (cray = 0; cray < nrays; cray++) {
		cx[cray] =
			(mlib_s32)((rays->starts[cray][0] -
			rays->starts[0][0]) * MLIB_PREC) + MLIB_ROUND;
		cy[cray] =
			(mlib_s32)((rays->starts[cray][1] -
			rays->starts[0][1]) * MLIB_PREC) + MLIB_ROUND;
		cz[cray] =
			(mlib_s32)(((rays->starts[cray][2] -
			rays->starts[0][2]) * vol->zsrates[0]) * MLIB_PREC) +
			MLIB_ROUND;
	}

	x = rays->starts[0][0];
	y = rays->starts[0][1];
	z = (rays->starts[0][2] - vol->zcoords[0]) * vol->zsrates[0];

/* for combining data with vis_faligndata() */

	vis_write_gsr(6);

	for (cstp = 0; cstp < minsteps; cstp++) {
		dp = (mlib_s16 *)rays->results[cstp];

/*
 * create fixed point versions of x, y, z
 */
		X = (mlib_s32)(x * MLIB_PREC);
		Y = (mlib_s32)(y * MLIB_PREC);
		Z = (mlib_s32)(z * MLIB_PREC);

		align = ((mlib_addr)dp & 0x7);

#pragma pipeloop(0)
		for (cray = 0; (cray < nrays) && (align != 0); cray++) {
/*
 * calculate the coordinates of the current voxel
 */
			currX = X + cx[cray];
			currY = Y + cy[cray];
			currZ = Z + cz[cray];

/*
 * get the coordinates of the nearest neighbor
 */
			Xint = currX >> MLIB_SHIFT;
			Yint = currY >> MLIB_SHIFT;
			Zint = currZ >> MLIB_SHIFT;

/*
 * read the nearest neighbor voxel from the slice dataset
 */
			sp = (mlib_s16 *)((mlib_u8 *)vol->slices[Zint] +
				vol->offsets[Yint]) + Xint;
			sd = *sp;

/*
 * store the interpolated voxel
 */
			(*dp++) = sd;

			align = ((mlib_addr)dp & 0x7);
		}

		dp_d64 = (mlib_d64 *)dp;
		count = (nrays - cray) >> 2;

#pragma pipeloop(0)
		for (i = 0, nray = cray + 3; i < count; i++) {
/*
 * calculate the coordinates of the 4th voxel
 */
			currX = X + cx[nray];
			currY = Y + cy[nray];
			currZ = Z + cz[nray];
			nray--;

/*
 * get the coordinates of the nearest neighbor
 */
			Xint = currX >> MLIB_SHIFT;
			Yint = currY >> MLIB_SHIFT;
			Zint = currZ >> MLIB_SHIFT;

/*
 * read the nearest neighbor voxel from the slice dataset
 */
			sp = (mlib_s16 *)((mlib_u8 *)vol->slices[Zint] +
				vol->offsets[Yint]) + Xint;
			P1 = vis_ld_u16(sp);
			P = vis_faligndata(P1, P);

/*
 * calculate the coordinates of the 3rd voxel
 */
			currX = X + cx[nray];
			currY = Y + cy[nray];
			currZ = Z + cz[nray];
			nray--;

/*
 * get the coordinates of the nearest neighbor
 */
			Xint = currX >> MLIB_SHIFT;
			Yint = currY >> MLIB_SHIFT;
			Zint = currZ >> MLIB_SHIFT;

/*
 * read the nearest neighbor voxel from the slice dataset
 */
			sp = (mlib_s16 *)((mlib_u8 *)vol->slices[Zint] +
				vol->offsets[Yint]) + Xint;
			P1 = vis_ld_u16(sp);
			P = vis_faligndata(P1, P);

/*
 * calculate the coordinates of the 2nd voxel
 */
			currX = X + cx[nray];
			currY = Y + cy[nray];
			currZ = Z + cz[nray];
			nray--;

/*
 * get the coordinates of the nearest neighbor
 */
			Xint = currX >> MLIB_SHIFT;
			Yint = currY >> MLIB_SHIFT;
			Zint = currZ >> MLIB_SHIFT;

/*
 * read the nearest neighbor voxel from the slice dataset
 */
			sp = (mlib_s16 *)((mlib_u8 *)vol->slices[Zint] +
				vol->offsets[Yint]) + Xint;
			P1 = vis_ld_u16(sp);
			P = vis_faligndata(P1, P);

/*
 * calculate the coordinates of the 1st voxel
 */
			currX = X + cx[nray];
			currY = Y + cy[nray];
			currZ = Z + cz[nray];
			nray += 7;

/*
 * get the coordinates of the nearest neighbor
 */
			Xint = currX >> MLIB_SHIFT;
			Yint = currY >> MLIB_SHIFT;
			Zint = currZ >> MLIB_SHIFT;

/*
 * read the nearest neighbor voxel from the slice dataset
 */
			sp = (mlib_s16 *)((mlib_u8 *)vol->slices[Zint] +
				vol->offsets[Yint]) + Xint;
			P1 = vis_ld_u16(sp);
			P = vis_faligndata(P1, P);

/*
 * store the 4 interpolated voxels
 */
			(*dp_d64++) = P;
		}

		dp += (count << 2);
		cray += (count << 2);

#pragma pipeloop(0)
		for (; cray < nrays; cray++) {
/*
 * calculate the coordinates of the current voxel
 */
			currX = X + cx[cray];
			currY = Y + cy[cray];
			currZ = Z + cz[cray];

/*
 * get the coordinates of the nearest neighbor
 */
			Xint = currX >> MLIB_SHIFT;
			Yint = currY >> MLIB_SHIFT;
			Zint = currZ >> MLIB_SHIFT;

/*
 * read the nearest neighbor voxel from the slice dataset
 */
			sp = (mlib_s16 *)((mlib_u8 *)vol->slices[Zint] +
				vol->offsets[Yint]) + Xint;
			sd = *sp;

/*
 * store the interpolated voxel
 */
			(*dp++) = sd;
		}

/*
 * advance along the home ray
 */
		x += dx;
		y += dy;
		z += dz;
	}

/*
 * loop for handling the remained voxels
 */
	for (cstp = minsteps; cstp < maxsteps; cstp++) {
/*
 * prepare the pointer to destination
 */
		dp = (mlib_s16 *)rays->results[cstp];

/*
 * create fixed point versions of x, y, z
 */
		X = (mlib_s32)(x * MLIB_PREC);
		Y = (mlib_s32)(y * MLIB_PREC);
		Z = (mlib_s32)(z * MLIB_PREC);

#pragma pipeloop(0)
		for (cray = 0; cray < nrays; cray++) {
			if (cstp < rays->nsteps[cray]) {
/*
 * calculate the coordinates of the current voxel
 */
				currX = X + cx[cray];
				currY = Y + cy[cray];
				currZ = Z + cz[cray];

/*
 * get the coordinates of the nearest neighbor
 */
				Xint = currX >> MLIB_SHIFT;
				Yint = currY >> MLIB_SHIFT;
				Zint = currZ >> MLIB_SHIFT;

/*
 * read the nearest neighbor voxel from the slice
 * dataset
 */
				sp = (mlib_s16 *)((mlib_u8 *)vol->slices[Zint] +
					vol->offsets[Yint]) + Xint;
				sd = *sp;

/*
 * store the interpolated voxel
 */
				(*dp++) = sd;
			} else {
				dp++;
			}
		}

/*
 * advance along the home ray
 */
		x += dx;
		y += dy;
		z += dz;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_v_VolumeRayCast_General_Parallel_Nearest_S16_S16_MultiRegion(
	mlib_rays *rays,
	mlib_s32 nrays,
	mlib_s32 maxsteps,
	const mlib_genvolume *vol)
{
	mlib_u8 **slices = (mlib_u8 **)vol->slices;
	mlib_s32 *offsets = vol->offsets;
	mlib_s32 nslices = vol->nslices;
	mlib_s32 nregions = vol->nregions;
	mlib_s32 zcoords[REGION_LIMIT];
	mlib_s32 *zindices = vol->zindices;
	mlib_d64 *zsrates = vol->zsrates;
	mlib_u16 **results = (mlib_u16 **)rays->results;
	mlib_s32 dX = (mlib_s32)(rays->incs[0][0] * MLIB_PREC);
	mlib_s32 dY = (mlib_s32)(rays->incs[0][1] * MLIB_PREC);
	mlib_s32 dZ = (mlib_s32)(rays->incs[0][2] * MLIB_PREC);
	mlib_s32 *nsteps = rays->nsteps;
	mlib_u32 sliceLst[SLICE_LIMIT];
	bufData *bufptr;
	bufData bufArray[STEPS_LIMIT * MAX_RAY_NUM];
	mlib_u32 nstep, nray, indx;
	mlib_u8 *slicePtr;
	mlib_u16 *rowPtr;
	mlib_u16 *resultPtr;
	mlib_s32 nRaySteps;
	mlib_s32 nregion, nslice, zindex;
	mlib_s32 xsrc, ysrc;
	mlib_d64 zsrate;
	mlib_s32 zcoord, ssZ;
	mlib_s32 X, Y, Z, dssZ;
	mlib_s32 i, j;

	for (i = 0; i <= nregions; i++)
		zcoords[i] = vol->zcoords[i] * MLIB_PREC;

	for (i = 0; i < nslices; i++)
		sliceLst[i] = MLIB_U32_MAX;

	ALLOC_MEM;

	if (nregions == 1) {
		for (i = 0; i < nrays; i++) {
			X = (rays->starts[i][0] + 0.5) * MLIB_PREC;
			Y = (rays->starts[i][1] + 0.5) * MLIB_PREC;
			Z = rays->starts[i][2] * MLIB_PREC;

			nRaySteps = nsteps[i];

			nregion = 0;
			zsrate = zsrates[0];
			zcoord = zcoords[0];
			zindex = zindices[0];

			ssZ = (Z - zcoord) * zsrate;
			dssZ = dZ * zsrate;

			for (j = 0; j < nRaySteps; j++) {
				nslice = zindex + ((ssZ +
					MLIB_ROUND) >> MLIB_SHIFT);

				xsrc = (X >> MLIB_SHIFT);
				ysrc = (Y >> MLIB_SHIFT);

				indx = (j << NSTEP_SHIFT) + i;

				bufptr[indx].nPrev = sliceLst[nslice];
				bufptr[indx].xSrc = (mlib_u16)xsrc;
				bufptr[indx].ySrc = (mlib_u16)ysrc;

				sliceLst[nslice] = indx;

				X += dX;
				Y += dY;

				ssZ += dssZ;
			}
		}
	} else {
		if (dZ > 0) {
			for (i = 0; i < nrays; i++) {
				X = (rays->starts[i][0] + 0.5) * MLIB_PREC;
				Y = (rays->starts[i][1] + 0.5) * MLIB_PREC;
				Z = rays->starts[i][2] * MLIB_PREC;

				nRaySteps = nsteps[i];

				nregion = 0;
				zsrate = zsrates[0];
				zcoord = zcoords[0];
				zindex = zindices[0];

				ssZ = (Z - zcoord) * zsrate;
				dssZ = dZ * zsrate;

				for (j = 0; j < nRaySteps; j++) {
					for (; Z > zcoords[nregion + 1]; ) {
						nregion++;
						zsrate = zsrates[nregion];
						zcoord = zcoords[nregion];
						zindex = zindices[nregion];

						ssZ = (Z - zcoord) * zsrate;
						dssZ = dZ * zsrate;
					}

					nslice = zindex + ((ssZ +
						MLIB_ROUND) >> MLIB_SHIFT);

					xsrc = (X >> MLIB_SHIFT);
					ysrc = (Y >> MLIB_SHIFT);

					indx = (j << NSTEP_SHIFT) + i;

					bufptr[indx].nPrev = sliceLst[nslice];
					bufptr[indx].xSrc = (mlib_u16)xsrc;
					bufptr[indx].ySrc = (mlib_u16)ysrc;

					sliceLst[nslice] = indx;

					X += dX;
					Y += dY;
					Z += dZ;

					ssZ += dssZ;
				}
			}
		} else {
			for (i = 0; i < nrays; i++) {
				X = (rays->starts[i][0] + 0.5) * MLIB_PREC;
				Y = (rays->starts[i][1] + 0.5) * MLIB_PREC;
				Z = rays->starts[i][2] * MLIB_PREC;

				nRaySteps = nsteps[i];
				resultPtr = results[i];

				nregion = nregions - 1;
				zsrate = zsrates[nregions - 1];
				zcoord = zcoords[nregions - 1];
				zindex = zindices[nregions - 1];

				ssZ = (Z - zcoord) * zsrate;
				dssZ = dZ * zsrate;

				for (j = 0; j < nRaySteps; j++) {
					for (; Z < zcoords[nregion]; ) {
						nregion--;
						zsrate = zsrates[nregion];
						zcoord = zcoords[nregion];
						zindex = zindices[nregion];

						ssZ = (Z - zcoord) * zsrate;
						dssZ = dZ * zsrate;
					}

					nslice = zindex + ((ssZ +
						MLIB_ROUND) >> MLIB_SHIFT);

					xsrc = (X >> MLIB_SHIFT);
					ysrc = (Y >> MLIB_SHIFT);

					indx = (j << NSTEP_SHIFT) + i;

					bufptr[indx].nPrev = sliceLst[nslice];
					bufptr[indx].xSrc = (mlib_u16)xsrc;
					bufptr[indx].ySrc = (mlib_u16)ysrc;

					sliceLst[nslice] = indx;

					X += dX;
					Y += dY;
					Z += dZ;

					ssZ += dssZ;
				}
			}
		}
	}

	for (i = 0; i < nslices; i++) {
		slicePtr = slices[i];
		indx = sliceLst[i];
		nstep = (indx >> 6);
		nray = (indx & 0x3F);

		for (; indx != MLIB_U32_MAX; ) {
			xsrc = bufptr[indx].xSrc;
			ysrc = bufptr[indx].ySrc;

			rowPtr = (mlib_u16 *)(slicePtr + offsets[ysrc]);
			resultPtr = results[nstep];
			resultPtr[nray] = rowPtr[xsrc];

			indx = bufptr[indx].nPrev;
			nstep = (indx >> 6);
			nray = (indx & 0x3F);
		}
	}

	FREE_MEM;
	return (MLIB_SUCCESS);
}

/* *********************************************************** */
