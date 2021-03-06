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

#pragma ident	"@(#)mlib_VolumeRayCast_U8_Bit.c	9.3	07/10/09 SMI"

/*
 *  FUNCTION
 *    mlib_VolumeRayCast_General_[Parallel|Divergent]_Nearest_U8_Bit
 *      Cast rays through a three dimensional data set
 *      and return the interpolated samples.
 *
 *  SYNOPSIS
 *    mlib_status mlib_VolumeRayCast_General_[Parallel|Divergent]_Nearest_U8_Bit
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
 *    Type of the slices is an 1-bit integer that eight voxels
 *    are packed into one byte. In nearest neighbor operation,
 *    the sample value at point P is replaced with the value of
 *    the nearest neighbor voxel.
 */

#include <mlib_volume.h>
#include <mlib_SysMath.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_VolumeRayCast_General_Divergent_Nearest_U8_Bit = \
__mlib_VolumeRayCast_General_Divergent_Nearest_U8_Bit
#pragma weak mlib_VolumeRayCast_General_Parallel_Nearest_U8_Bit = \
	__mlib_VolumeRayCast_General_Parallel_Nearest_U8_Bit
#elif defined(__GNUC__)

__typeof__(__mlib_VolumeRayCast_General_Divergent_Nearest_U8_Bit)
	mlib_VolumeRayCast_General_Divergent_Nearest_U8_Bit
	__attribute__((weak,
	alias("__mlib_VolumeRayCast_General_Divergent_Nearest_U8_Bit")));
__typeof__(__mlib_VolumeRayCast_General_Parallel_Nearest_U8_Bit)
	mlib_VolumeRayCast_General_Parallel_Nearest_U8_Bit
	__attribute__((weak,
	alias("__mlib_VolumeRayCast_General_Parallel_Nearest_U8_Bit")));

#else

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	MAX_RAY_NUM	64
#define	SLICE_LIMIT	512
#define	REGION_LIMIT	512
#define	STEPS_LIMIT	512
#define	MLIB_SHIFT	21
#define	MLIB_PREC	(1 << MLIB_SHIFT)
#define	MLIB_ROUND	(1 << (MLIB_SHIFT-1))
#define	NSTEP_SHIFT	6
#define	NRAY_MASK	((1 << NSTEP_SHIFT) - 1)
#define	ONE	0x1

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

static mlib_status mlib_VolumeRayCast_General_Parallel_Nearest_U8_Bit_OneRegion(
	mlib_rays *rays,
	mlib_s32 nrays,
	mlib_s32 minsteps,
	mlib_s32 maxsteps,
	const mlib_genvolume *vol);

static mlib_status
mlib_VolumeRayCast_General_Parallel_Nearest_U8_Bit_MultiRegion(
	mlib_rays *rays,
	mlib_s32 nrays,
	mlib_s32 maxsteps,
	const mlib_genvolume *vol);

/* *********************************************************** */

mlib_status
__mlib_VolumeRayCast_General_Parallel_Nearest_U8_Bit(
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
		mlib_VolumeRayCast_General_Parallel_Nearest_U8_Bit_OneRegion
		(rays, nrays, minsteps, maxsteps, vol));
	} else {
		return (
		mlib_VolumeRayCast_General_Parallel_Nearest_U8_Bit_MultiRegion
		(rays, nrays, maxsteps, vol));
	}
}

/* *********************************************************** */

mlib_status
mlib_VolumeRayCast_General_Parallel_Nearest_U8_Bit_OneRegion(
	mlib_rays *rays,
	mlib_s32 nrays,
	mlib_s32 minsteps,
	mlib_s32 maxsteps,
	const mlib_genvolume *vol)
{
	mlib_s32 cray;
	mlib_s32 cstp;
	mlib_s32 byteOffset, bitOffset;

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

	mlib_u8 sd, bitData;

/* pointer to source data */

	mlib_u8 *sp;

/* pointer to destination data */

	mlib_u8 *dp;

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

	for (cstp = 0; cstp < minsteps; cstp++) {
		dp = (mlib_u8 *)rays->results[cstp];

/*
 * create fixed point versions of x, y, z
 */
		X = (mlib_s32)(x * MLIB_PREC);
		Y = (mlib_s32)(y * MLIB_PREC);
		Z = (mlib_s32)(z * MLIB_PREC);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (cray = 0; cray < nrays; cray++) {
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
			byteOffset = (Xint >> 3);
			sp = ((mlib_u8 *)vol->slices[Zint] +
				vol->offsets[Yint]) + byteOffset;
			sd = *sp;
			bitOffset = Xint - byteOffset;
			bitData = ((sd >> (7 - bitOffset)) & ONE);

/*
 * store the interpolated voxel
 */
			(*dp++) = bitData;
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

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
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
				byteOffset = (Xint >> 3);
				sp = ((mlib_u8 *)vol->slices[Zint] +
					vol->offsets[Yint]) + byteOffset;
				sd = *sp;
				bitOffset = Xint - byteOffset;
				bitData = ((sd >> (7 - bitOffset)) & ONE);

/*
 * store the interpolated voxel
 */
				(*dp++) = bitData;
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
mlib_VolumeRayCast_General_Parallel_Nearest_U8_Bit_MultiRegion(
	mlib_rays *rays,
	mlib_s32 nrays,
	mlib_s32 maxsteps,
	const mlib_genvolume *vol)
{
	mlib_u8 **slices;
	mlib_s32 *offsets;
	mlib_s32 nslices;
	mlib_s32 nregions;
	mlib_s32 zcoords[REGION_LIMIT];
	mlib_s32 *zindices;
	mlib_d64 *zsrates;
	mlib_u8 **results;
	mlib_s32 *nsteps;
	mlib_u32 sliceLst[SLICE_LIMIT];
	bufData *bufptr;
	bufData bufArray[STEPS_LIMIT * MAX_RAY_NUM];
	mlib_u32 nstep, nray, indx;
	mlib_u8 *slicePtr;
	mlib_u8 *resultPtr;
	mlib_s32 nRaySteps;
	mlib_s32 nregion, nslice, zindex;
	mlib_s32 xsrc, ysrc;
	mlib_s32 byteOffset, bitOffset;
	mlib_u8 byte, bitData;
	mlib_d64 zsrate;
	mlib_s32 zcoord, ssZ;
	mlib_s32 X, Y, Z;
	mlib_s32 dX, dY, dZ, dssZ;
	mlib_s32 i, j;

	slices = (mlib_u8 **)vol->slices;
	offsets = vol->offsets;
	nslices = vol->nslices;
	nregions = vol->nregions;
	zindices = vol->zindices;

	for (i = 0; i <= nregions; i++)
		zcoords[i] = vol->zcoords[i] * MLIB_PREC;

	zsrates = vol->zsrates;

	for (i = 0; i < nslices; i++)
		sliceLst[i] = MLIB_U32_MAX;

	results = (mlib_u8 **)rays->results;

	dX = (mlib_s32)(rays->incs[0][0] * MLIB_PREC);
	dY = (mlib_s32)(rays->incs[0][1] * MLIB_PREC);
	dZ = (mlib_s32)(rays->incs[0][2] * MLIB_PREC);

	nsteps = rays->nsteps;

	ALLOC_MEM;

	if (nregions == 1) {
		for (i = 0; i < nrays; i++) {
			X = (rays->starts[i][0] + 0.5) * MLIB_PREC;
			Y = (rays->starts[i][1] + 0.5) * MLIB_PREC;
			Z = rays->starts[i][2] * MLIB_PREC;

			nRaySteps = nsteps[i];

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

			byteOffset = (xsrc >> 3);
			byte = slicePtr[offsets[ysrc] + (xsrc >> 3)];
			bitOffset = xsrc - byteOffset;
			bitData = ((byte >> (7 - bitOffset)) & ONE);
			resultPtr = results[nstep];
			resultPtr[nray] = bitData;

			indx = bufptr[indx].nPrev;
			nstep = (indx >> NSTEP_SHIFT);
			nray = (indx & NRAY_MASK);
		}
	}

	FREE_MEM;

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VolumeRayCast_General_Divergent_Nearest_U8_Bit(
	mlib_rays *rays,
	const mlib_genvolume *vol,
	void *buffer)
{
	mlib_u8 **slices;
	mlib_s32 *offsets;
	mlib_s32 nslices;
	mlib_s32 nregions;
	mlib_s32 zcoords[REGION_LIMIT];
	mlib_s32 *zindices;
	mlib_d64 *zsrates;
	mlib_u8 **results;
	mlib_s32 nrays;
	mlib_s32 *nsteps;
	mlib_s32 maxsteps;
	mlib_u32 sliceLst[SLICE_LIMIT];
	bufData *bufptr;
	bufData bufArray[STEPS_LIMIT * MAX_RAY_NUM];
	mlib_u32 nstep, nray, indx;
	mlib_u8 *slicePtr;
	mlib_u8 *resultPtr;
	mlib_s32 nRaySteps;
	mlib_s32 nregion, nslice, zindex;
	mlib_s32 xsrc, ysrc;
	mlib_s32 byteOffset, bitOffset;
	mlib_u8 byte, bitData;
	mlib_d64 zsrate;
	mlib_s32 zcoord, ssZ;
	mlib_s32 X, Y, Z;
	mlib_s32 dX, dY, dZ, dssZ;
	mlib_s32 i, j;

	if (rays == NULL || vol == NULL)
		return (MLIB_NULLPOINTER);

	slices = (mlib_u8 **)vol->slices;
	offsets = vol->offsets;
	nslices = vol->nslices;
	nregions = vol->nregions;
	zindices = vol->zindices;

	if (nregions > REGION_LIMIT)
		return (MLIB_OUTOFRANGE);

	for (i = 0; i <= nregions; i++)
		zcoords[i] = vol->zcoords[i] * MLIB_PREC;

	zsrates = vol->zsrates;

	if (nslices > SLICE_LIMIT)
		return (MLIB_OUTOFRANGE);

	for (i = 0; i < nslices; i++)
		sliceLst[i] = MLIB_U32_MAX;

	results = (mlib_u8 **)rays->results;
	nrays = rays->nrays;

	nsteps = rays->nsteps;

	maxsteps = nsteps[0];

	for (i = 1; i < nrays; i++) {
		if (nsteps[i] > maxsteps)
			maxsteps = nsteps[i];
	}

	ALLOC_MEM;

	if (nregions == 1) {
		for (i = 0; i < nrays; i++) {
			X = (rays->starts[i][0] + 0.5) * MLIB_PREC;
			Y = (rays->starts[i][1] + 0.5) * MLIB_PREC;
			Z = rays->starts[i][2] * MLIB_PREC;

			dX = (mlib_s32)(rays->incs[i][0] * MLIB_PREC);
			dY = (mlib_s32)(rays->incs[i][1] * MLIB_PREC);
			dZ = (mlib_s32)(rays->incs[i][2] * MLIB_PREC);

			nRaySteps = nsteps[i];

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
		for (i = 0; i < nrays; i++) {
			X = (rays->starts[i][0] + 0.5) * MLIB_PREC;
			Y = (rays->starts[i][1] + 0.5) * MLIB_PREC;
			Z = rays->starts[i][2] * MLIB_PREC;

			dX = (mlib_s32)(rays->incs[i][0] * MLIB_PREC);
			dY = (mlib_s32)(rays->incs[i][1] * MLIB_PREC);
			dZ = (mlib_s32)(rays->incs[i][2] * MLIB_PREC);

			nRaySteps = nsteps[i];
			resultPtr = results[i];

			if (dZ > 0) {
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
			} else {
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

			byteOffset = (xsrc >> 3);
			byte = slicePtr[offsets[ysrc] + (xsrc >> 3)];
			bitOffset = xsrc - byteOffset;
			bitData = ((byte >> (7 - bitOffset)) & ONE);
			resultPtr = results[nstep];
			resultPtr[nray] = bitData;

			indx = bufptr[indx].nPrev;
			nstep = (indx >> NSTEP_SHIFT);
			nray = (indx & NRAY_MASK);
		}
	}

	FREE_MEM;

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
