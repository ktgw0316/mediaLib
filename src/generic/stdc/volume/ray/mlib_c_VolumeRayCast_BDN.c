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

#pragma ident	"@(#)mlib_c_VolumeRayCast_BDN.c	9.2	07/10/09 SMI"

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
 *    In nearest neighbor operation, the sample value at point P is replaced
 *    with the value of the nearest neighbor voxel.
 *
 */

#include <stdlib.h>
#include <mlib_volume.h>
#include <mlib_c_VolumeRayCast_Blocked.h>

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
#define	MLIB_SHIFT	20
#define	MLIB_PREC	(1 << MLIB_SHIFT)

/* *********************************************************** */

mlib_status
__mlib_VolumeRayCast_Blocked_Divergent_Nearest_U8_U8(
	mlib_rays *rays,
	const mlib_blkvolume *blk,
	void *buffer)
{
	mlib_s32 xysize, zsize;
	mlib_s32 nrays;
	mlib_s32 dX, dY, dZ;
	mlib_u8 *voxels;
	mlib_s32 nsteps;
	mlib_s32 xsrc, ysrc, zsrc;
	mlib_u32 index, n, m;
	mlib_s32 X, Y, Z;
	mlib_s32 i, j;
	mlib_u8 a;

	if (rays == NULL || blk == NULL)
		return (MLIB_NULLPOINTER);

	xysize = blk->xysize, zsize = blk->zsize;
	nrays = rays->nrays;
	voxels = (mlib_u8 *)blk->voxels;

	if (zsize > 512 || xysize > 2048)
		return (MLIB_OUTOFRANGE);

	i = xysize >> 6;
	n = 0;
	while (i >>= 1)
		n++;
	m = (n << 1) + 12;
	n += 11;

	for (i = 0; i < nrays; i++) {
		X = (rays->starts[i][0] + 0.5) * MLIB_PREC;
		Y = (rays->starts[i][1] + 0.5) * MLIB_PREC;
		Z = (rays->starts[i][2] + 0.5) * MLIB_PREC;

		dX = (mlib_s32)(rays->incs[i][0] * MLIB_PREC);
		dY = (mlib_s32)(rays->incs[i][1] * MLIB_PREC);
		dZ = (mlib_s32)(rays->incs[i][2] * MLIB_PREC);

		xsrc = (X >> MLIB_SHIFT);
		X += dX;
		ysrc = (Y >> MLIB_SHIFT);
		Y += dY;
		zsrc = (Z >> MLIB_SHIFT);
		Z += dZ;

		nsteps = rays->nsteps[i];

		for (j = 0; j < nsteps; j++) {
			index = mlib_indx[xsrc] |
				mlib_indy[ysrc & 0x3F] |
				mlib_indz[zsrc & 0x1F] |
				((ysrc & (~0x3F)) << n) |
				((zsrc & (~0x1F)) << m);
			a = voxels[index];

			xsrc = (X >> MLIB_SHIFT);
			X += dX;
			ysrc = (Y >> MLIB_SHIFT);
			Y += dY;
			zsrc = (Z >> MLIB_SHIFT);
			Z += dZ;

			((mlib_u8 *)rays->results[j])[i] = a;
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VolumeRayCast_Blocked_Divergent_Nearest_S16_S16(
	mlib_rays *rays,
	const mlib_blkvolume *blk,
	void *buffer)
{
	mlib_s32 xysize, zsize;
	mlib_s32 nrays;
	mlib_s32 dX, dY, dZ;
	mlib_u16 *voxels;
	mlib_s32 nsteps;
	mlib_s32 xsrc, ysrc, zsrc;
	mlib_u32 index, n, m;
	mlib_s32 X, Y, Z;
	mlib_s32 i, j;
	mlib_u16 a;

	if (rays == NULL || blk == NULL)
		return (MLIB_NULLPOINTER);

	xysize = blk->xysize, zsize = blk->zsize;
	nrays = rays->nrays;
	voxels = (mlib_u16 *)blk->voxels;

	if (zsize > 512 || xysize > 2048)
		return (MLIB_OUTOFRANGE);

	i = xysize >> 6;
	n = 0;
	while (i >>= 1)
		n++;
	m = (n << 1) + 12;
	n += 11;

	for (i = 0; i < nrays; i++) {
		X = (rays->starts[i][0] + 0.5) * MLIB_PREC;
		Y = (rays->starts[i][1] + 0.5) * MLIB_PREC;
		Z = (rays->starts[i][2] + 0.5) * MLIB_PREC;

		dX = (mlib_s32)(rays->incs[i][0] * MLIB_PREC);
		dY = (mlib_s32)(rays->incs[i][1] * MLIB_PREC);
		dZ = (mlib_s32)(rays->incs[i][2] * MLIB_PREC);

		xsrc = (X >> MLIB_SHIFT);
		X += dX;
		ysrc = (Y >> MLIB_SHIFT);
		Y += dY;
		zsrc = (Z >> MLIB_SHIFT);
		Z += dZ;

		nsteps = rays->nsteps[i];

		for (j = 0; j < nsteps; j++) {

			index = mlib_indx[xsrc] |
				mlib_indy[ysrc & 0x3F] |
				mlib_indz[zsrc & 0x1F] |
				((ysrc & (~0x3F)) << n) |
				((zsrc & (~0x1F)) << m);
			a = voxels[index];

			xsrc = (X >> MLIB_SHIFT);
			X += dX;
			ysrc = (Y >> MLIB_SHIFT);
			Y += dY;
			zsrc = (Z >> MLIB_SHIFT);
			Z += dZ;

			((mlib_u16 *)rays->results[j])[i] = a;
		}
	}
	return (MLIB_SUCCESS);
}

/* *********************************************************** */
