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

#pragma ident	"@(#)mlib_v_VolumeRayCast_BDN.c	9.2	07/11/05 SMI"

/*
 *  FUNCTION
 *    mlib_VolumeRayCast_Blocked_Divergent_Nearest_[U8_U8|S16_S16]
 *      Cast divergent rays through a three dimensional data set
 *      and return the interpolated samples.
 *
 *  SYNOPSIS
 *    mlib_status mlib_VolumeRayCast_Blocked_Divergent_Nearest_[U8_U8|S16_S16]
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

#include <stdlib.h>
#include <mlib_volume.h>
#include <vis_proto.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_VolumeRayCast_Blocked_Divergent_Nearest_U8_U8 = \
__mlib_VolumeRayCast_Blocked_Divergent_Nearest_U8_U8
#pragma weak mlib_VolumeRayCast_Blocked_Divergent_Nearest_S16_S16 = \
	__mlib_VolumeRayCast_Blocked_Divergent_Nearest_S16_S16
#elif defined(__GNUC__)

__typeof__(__mlib_VolumeRayCast_Blocked_Divergent_Nearest_U8_U8)
	mlib_VolumeRayCast_Blocked_Divergent_Nearest_U8_U8
	__attribute__((weak,
	alias("__mlib_VolumeRayCast_Blocked_Divergent_Nearest_U8_U8")));
__typeof__(__mlib_VolumeRayCast_Blocked_Divergent_Nearest_S16_S16)
	mlib_VolumeRayCast_Blocked_Divergent_Nearest_S16_S16
	__attribute__((weak,
	alias("__mlib_VolumeRayCast_Blocked_Divergent_Nearest_S16_S16")));

#else

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */
#define	MLIB_SHIFT_20	20
#define	MLIB_PREC_20	(1 << MLIB_SHIFT_20)
#define	MLIB_MASK_20	~(MLIB_PREC_20 - 1)

#define	MLIB_SHIFT	11
#define	MLIB_PREC	(1 << MLIB_SHIFT)

/* *********************************************************** */

static mlib_status
mlib_v_VolumeRayCast_Blocked_Divergent_Nearest_S16_S16_SmallSteps(
	mlib_rays *rays,
	const mlib_blkvolume *blk);

static mlib_status
mlib_v_VolumeRayCast_Blocked_Divergent_Nearest_S16_S16_ManySteps(
	mlib_rays *rays,
	const mlib_blkvolume *blk);

static mlib_status
mlib_v_VolumeRayCast_Blocked_Divergent_Nearest_U8_U8_SmallSteps(
	mlib_rays *rays,
	const mlib_blkvolume *blk);

static mlib_status
mlib_v_VolumeRayCast_Blocked_Divergent_Nearest_U8_U8_ManySteps(
	mlib_rays *rays,
	const mlib_blkvolume *blk);

/* *********************************************************** */

mlib_status
__mlib_VolumeRayCast_Blocked_Divergent_Nearest_U8_U8(
	mlib_rays *rays,
	const mlib_blkvolume *blk,
	void *buffer)
{
	mlib_s32 nrays, cray;
	mlib_s32 maxsteps;

	if (rays == NULL || blk == NULL)
		return (MLIB_NULLPOINTER);

	nrays = rays->nrays;

	maxsteps = -1;

	for (cray = 0; cray < nrays; cray++) {
		if (rays->nsteps[cray] > maxsteps)
			maxsteps = rays->nsteps[cray];
	}

	if (maxsteps > 2000) {
		return (
		mlib_v_VolumeRayCast_Blocked_Divergent_Nearest_U8_U8_ManySteps
		(rays, blk));
	} else {
		return (
		mlib_v_VolumeRayCast_Blocked_Divergent_Nearest_U8_U8_SmallSteps
		(rays, blk));
	}
}

/* *********************************************************** */

mlib_status
mlib_v_VolumeRayCast_Blocked_Divergent_Nearest_U8_U8_ManySteps(
	mlib_rays *rays,
	const mlib_blkvolume *blk)
{
	mlib_s32 xysize;
	mlib_s32 nrays, i;
	mlib_s32 dX, dY, dZ;
	mlib_s32 OX, OY, OZ;
	mlib_u8 *voxels;
	mlib_s32 nsteps, cray, cstp;
	mlib_u32 n, index;
	mlib_s32 X, Y, Z;
	mlib_u8 a;
	mlib_u64 ZYX;

	xysize = blk->xysize, nrays = rays->nrays;
	voxels = (mlib_u8 *)blk->voxels;

	i = xysize >> 6;
	n = 0;
	while (i >>= 1)
		n++;

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

		dX = (mlib_s32)(rays->incs[cray][0] * MLIB_PREC_20);
		dY = (mlib_s32)(rays->incs[cray][1] * MLIB_PREC_20);
		dZ = (mlib_s32)(rays->incs[cray][2] * MLIB_PREC_20);

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
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_v_VolumeRayCast_Blocked_Divergent_Nearest_U8_U8_SmallSteps(
	mlib_rays *rays,
	const mlib_blkvolume *blk)
{
	mlib_s32 xysize;
	mlib_s32 nrays, i;
	mlib_s32 dX, dY, dZ;
	mlib_u8 *voxels;
	mlib_s32 nsteps, cray, cstp;
	mlib_u32 n, index;
	mlib_s32 X, Y, Z;
	mlib_u8 a;
	mlib_u64 dOZYX, ZYX;

	xysize = blk->xysize, nrays = rays->nrays;
	voxels = (mlib_u8 *)blk->voxels;

	i = xysize >> 6;
	n = 0;
	while (i >>= 1)
		n++;

#pragma pipeloop(0)
	for (cray = 0; cray < nrays; cray++) {
		X = (rays->starts[cray][0] + 0.5) * MLIB_PREC;
		Y = (rays->starts[cray][1] + 0.5) * MLIB_PREC;
		Z = (rays->starts[cray][2] + 0.5) * MLIB_PREC;
		ZYX = Z;
		ZYX = (((ZYX << 22) + Y) << 22) + X;

		dX = (mlib_s32)(rays->incs[cray][0] * MLIB_PREC);
		dY = (mlib_s32)(rays->incs[cray][1] * MLIB_PREC);
		dZ = (mlib_s32)(rays->incs[cray][2] * MLIB_PREC);
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

		nsteps = rays->nsteps[cray];

		for (cstp = 0; cstp < nsteps; cstp++) {
			index = vis_array8(ZYX & 0xFF800FFE003FF800, n);

			a = *(voxels + index);

			ZYX += dOZYX;

			((mlib_u8 *)rays->results[cstp])[cray] = a;
		}
	}
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#define	FUN_NAME(STEP) \
	mlib_v_VolumeRayCast_Blocked_Divergent_Nearest_S16_S16_##STEP

mlib_status
__mlib_VolumeRayCast_Blocked_Divergent_Nearest_S16_S16(
	mlib_rays *rays,
	const mlib_blkvolume *blk,
	void *buffer)
{
	mlib_s32 nrays, cray;
	mlib_s32 maxsteps;

	if (rays == NULL || blk == NULL)
		return (MLIB_NULLPOINTER);

	nrays = rays->nrays;

	maxsteps = -1;

	for (cray = 0; cray < nrays; cray++) {
		if (rays->nsteps[cray] > maxsteps)
			maxsteps = rays->nsteps[cray];
	}

	if (maxsteps > 2000) {
		return (
		FUN_NAME(ManySteps)
		(rays, blk));
	} else {
		return (
		FUN_NAME(SmallSteps)
		(rays, blk));
	}
}

/* *********************************************************** */

mlib_status
mlib_v_VolumeRayCast_Blocked_Divergent_Nearest_S16_S16_ManySteps(
	mlib_rays *rays,
	const mlib_blkvolume *blk)
{
	mlib_s32 xysize;
	mlib_s32 nrays, i;
	mlib_s32 dX, dY, dZ;
	mlib_s32 OX, OY, OZ;
	mlib_u16 *voxels;
	mlib_s32 nsteps, cray, cstp;
	mlib_u32 n, index;
	mlib_s32 X, Y, Z;
	mlib_u16 a;
	mlib_u64 ZYX;

	xysize = blk->xysize, nrays = rays->nrays;
	voxels = (mlib_u16 *)blk->voxels;

	i = xysize >> 6;
	n = 0;
	while (i >>= 1)
		n++;

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

		dX = (mlib_s32)(rays->incs[cray][0] * MLIB_PREC_20);
		dY = (mlib_s32)(rays->incs[cray][1] * MLIB_PREC_20);
		dZ = (mlib_s32)(rays->incs[cray][2] * MLIB_PREC_20);

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
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_v_VolumeRayCast_Blocked_Divergent_Nearest_S16_S16_SmallSteps(
	mlib_rays *rays,
	const mlib_blkvolume *blk)
{
	mlib_s32 xysize;
	mlib_s32 nrays, i;
	mlib_s32 dX, dY, dZ;
	mlib_u16 *voxels;
	mlib_s32 nsteps, cray, cstp;
	mlib_u32 n, index;
	mlib_s32 X, Y, Z;
	mlib_u16 a;
	mlib_u64 dOZYX, ZYX;

	xysize = blk->xysize, nrays = rays->nrays;
	voxels = (mlib_u16 *)blk->voxels;

	i = xysize >> 6;
	n = 0;
	while (i >>= 1)
		n++;

#pragma pipeloop(0)
	for (cray = 0; cray < nrays; cray++) {
		X = (rays->starts[cray][0] + 0.5) * MLIB_PREC;
		Y = (rays->starts[cray][1] + 0.5) * MLIB_PREC;
		Z = (rays->starts[cray][2] + 0.5) * MLIB_PREC;
		ZYX = Z;
		ZYX = (((ZYX << 22) + Y) << 22) + X;

		dX = (mlib_s32)(rays->incs[cray][0] * MLIB_PREC);
		dY = (mlib_s32)(rays->incs[cray][1] * MLIB_PREC);
		dZ = (mlib_s32)(rays->incs[cray][2] * MLIB_PREC);
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

		nsteps = rays->nsteps[cray];

		for (cstp = 0; cstp < nsteps; cstp++) {
			index = vis_array16(ZYX & 0xFF800FFE003FF800, n);

			a = *(mlib_u16 *)((mlib_u8 *)voxels + index);

			ZYX += dOZYX;

			((mlib_u16 *)rays->results[cstp])[cray] = a;
		}
	}
	return (MLIB_SUCCESS);
}

/* *********************************************************** */
