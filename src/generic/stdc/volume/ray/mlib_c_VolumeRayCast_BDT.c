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

#pragma ident	"@(#)mlib_c_VolumeRayCast_BDT.c	9.2	07/10/09 SMI"

/*
 *  FUNCTION
 *    mlib_VolumeRayCast_Blocked_Divergent_Trilinear_[U8_U8|S16_S16]
 *      Cast divergent rays through a three-dimensional data set
 *      and return the interpolated samples.
 *
 *  SYNOPSIS
 *    mlib_status mlib_VolumeRayCast_Blocked_Divergent_Trilinear_[U8_U8|S16_S16]
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
 *    Casting rays through a three-dimensional data set, compute and
 *    return the interpolated samples at each step along the rays.
 *    Trilinear interpolation is used.
 *
 */

#include <stdlib.h>
#include <mlib_volume.h>
#include <mlib_c_VolumeRayCast_Blocked.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_VolumeRayCast_Blocked_Divergent_Trilinear_U8_U8 = \
__mlib_VolumeRayCast_Blocked_Divergent_Trilinear_U8_U8
#pragma weak mlib_VolumeRayCast_Blocked_Divergent_Trilinear_S16_S16 = \
	__mlib_VolumeRayCast_Blocked_Divergent_Trilinear_S16_S16
#elif defined(__GNUC__)

__typeof__(__mlib_VolumeRayCast_Blocked_Divergent_Trilinear_U8_U8)
	mlib_VolumeRayCast_Blocked_Divergent_Trilinear_U8_U8
	__attribute__((weak,
	alias("__mlib_VolumeRayCast_Blocked_Divergent_Trilinear_U8_U8")));
__typeof__(__mlib_VolumeRayCast_Blocked_Divergent_Trilinear_S16_S16)
	mlib_VolumeRayCast_Blocked_Divergent_Trilinear_S16_S16
	__attribute__((weak,
	alias("__mlib_VolumeRayCast_Blocked_Divergent_Trilinear_S16_S16")));

#else

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */
#define	TRILINEAR(a, b, c, P0, Px, Py, Pz, Pxy, Pxz, Pyz,                  \
	Pxyz, rsl)                                                         \
	rsl = ((1 - (a)) * (1 - (b)) * (1 - (c)) * (mlib_d64)P0 +          \
		(a) * (1 - (b)) * (1 - (c)) * (mlib_d64)Px + (1 -          \
		(a)) * (b) * (1 - (c)) * (mlib_d64)Py + (1 - (a)) * (1 -   \
		(b)) * (c) * (mlib_d64)Pz + (a) * (b) * (1 -               \
		(c)) * (mlib_d64)Pxy + (a) * (1 -                          \
		(b)) * (c) * (mlib_d64)Pxz + (1 -                          \
		(a)) * (b) * (c) * (mlib_d64)Pyz +                         \
		(a) * (b) * (c) * (mlib_d64)Pxyz)

/* *********************************************************** */
#define	LOAD_8(xsrc, ysrc, zsrc)                                        \
	{                                                               \
	    mlib_u32 mask[3], tempx, tempy, tempz, index;               \
	                                                                \
	    tempx = mlib_indx[xsrc];                                    \
	    tempy = mlib_indy[ysrc & 0x3F] | ((ysrc & (~0x3F)) << n);   \
	    tempz = mlib_indz[zsrc & 0x1F] | ((zsrc & (~0x1F)) << m);   \
	    index = tempx | tempy | tempz;                              \
	    mask[0] = mlib_indx[xsrc + 1] ^ tempx;                      \
	    mask[1] =                                                   \
		    (mlib_indy[(ysrc + 1) & 0x3F] | (((ysrc +           \
		    1) & (~0x3F)) << n)) ^ tempy;                       \
	    mask[2] =                                                   \
		    (mlib_indz[(zsrc + 1) & 0x1F] | (((zsrc +           \
		    1) & (~0x1F)) << m)) ^ tempz;                       \
	    Po = voxels[index];                                         \
	    Px = voxels[index ^ mask[0]];                               \
	    Py = voxels[index ^ mask[1]];                               \
	    Pz = voxels[index ^ mask[2]];                               \
	    Pxy = voxels[index ^ mask[0] ^ mask[1]];                    \
	    Pxz = voxels[index ^ mask[0] ^ mask[2]];                    \
	    Pyz = voxels[index ^ mask[1] ^ mask[2]];                    \
	    Pxyz = voxels[index ^ mask[0] ^ mask[1] ^ mask[2]];         \
	}

/* *********************************************************** */

mlib_status
__mlib_VolumeRayCast_Blocked_Divergent_Trilinear_U8_U8(
	mlib_rays *rays,
	const mlib_blkvolume *blk,
	void *buffer)
{
	mlib_s32 xysize;
	mlib_s32 nrays, i;
	mlib_d64 dOx, dOy, dOz;
	mlib_u8 *voxels;
	mlib_s32 nsteps, cray, cstp;
	mlib_u32 n, m;
	mlib_d64 X, Y, Z;
	mlib_d64 a, b, c;
	mlib_u8 Po, Px, Py, Pz, Pxy, Pxz, Pyz, Pxyz;
	mlib_s32 xsrc, ysrc, zsrc;

	if (rays == NULL || blk == NULL)
		return (MLIB_NULLPOINTER);

	xysize = blk->xysize;
	nrays = rays->nrays;
	voxels = (mlib_u8 *)blk->voxels;

	i = xysize >> 6;
	n = 0;
	while (i >>= 1)
		n++;
	m = (n << 1) + 12;
	n += 11;

	for (cray = 0; cray < nrays; cray++) {
		nsteps = rays->nsteps[cray];

		if (nsteps > 0) {

			X = rays->starts[cray][0];
			Y = rays->starts[cray][1];
			Z = rays->starts[cray][2];

			dOx = rays->incs[cray][0];
			dOy = rays->incs[cray][1];
			dOz = rays->incs[cray][2];

			xsrc = (mlib_s32)X;
			a = X - xsrc;
			X += dOx;
			ysrc = (mlib_s32)Y;
			b = Y - ysrc;
			Y += dOy;
			zsrc = (mlib_s32)Z;
			c = Z - zsrc;
			Z += dOz;

			LOAD_8(xsrc, ysrc, zsrc);

			for (cstp = 0; cstp < nsteps - 1; cstp++) {
				TRILINEAR(a, b, c, Po, Px, Py, Pz, Pxy, Pxz,
					Pyz, Pxyz,
					((mlib_u8 *)rays->results[cstp])[cray]);

				xsrc = (mlib_s32)X;
				a = X - xsrc;
				X += dOx;
				ysrc = (mlib_s32)Y;
				b = Y - ysrc;
				Y += dOy;
				zsrc = (mlib_s32)Z;
				c = Z - zsrc;
				Z += dOz;

				LOAD_8(xsrc, ysrc, zsrc);

			}
			TRILINEAR(a, b, c, Po, Px, Py, Pz, Pxy, Pxz, Pyz, Pxyz,
				((mlib_u8 *)rays->results[cstp])[cray]);
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VolumeRayCast_Blocked_Divergent_Trilinear_S16_S16(
	mlib_rays *rays,
	const mlib_blkvolume *blk,
	void *buffer)
{
	mlib_s32 xysize;
	mlib_s32 nrays, i;
	mlib_d64 dOx, dOy, dOz;
	mlib_s16 *voxels;
	mlib_s32 nsteps, cray, cstp;
	mlib_u32 n, m;
	mlib_d64 X, Y, Z;
	mlib_d64 a, b, c;
	mlib_s16 Po, Px, Py, Pz, Pxy, Pxz, Pyz, Pxyz;
	mlib_s32 xsrc, ysrc, zsrc;

	if (rays == NULL || blk == NULL)
		return (MLIB_NULLPOINTER);

	xysize = blk->xysize;
	nrays = rays->nrays;
	voxels = (mlib_s16 *)blk->voxels;

	i = xysize >> 6;
	n = 0;
	while (i >>= 1)
		n++;
	m = (n << 1) + 12;
	n += 11;

	for (cray = 0; cray < nrays; cray++) {
		nsteps = rays->nsteps[cray];

		if (nsteps > 0) {
			X = rays->starts[cray][0];
			Y = rays->starts[cray][1];
			Z = rays->starts[cray][2];

			dOx = rays->incs[cray][0];
			dOy = rays->incs[cray][1];
			dOz = rays->incs[cray][2];

			xsrc = (mlib_s32)X;
			a = X - xsrc;
			X += dOx;
			ysrc = (mlib_s32)Y;
			b = Y - ysrc;
			Y += dOy;
			zsrc = (mlib_s32)Z;
			c = Z - zsrc;
			Z += dOz;

			LOAD_8(xsrc, ysrc, zsrc);

			for (cstp = 0; cstp < nsteps - 1; cstp++) {
				TRILINEAR(a, b, c, Po, Px, Py, Pz, Pxy, Pxz,
					Pyz, Pxyz,
					((mlib_s16 *)rays->
					results[cstp])[cray]);

				xsrc = (mlib_s32)X;
				a = X - xsrc;
				X += dOx;
				ysrc = (mlib_s32)Y;
				b = Y - ysrc;
				Y += dOy;
				zsrc = (mlib_s32)Z;
				c = Z - zsrc;
				Z += dOz;

				LOAD_8(xsrc, ysrc, zsrc);

			}
			TRILINEAR(a, b, c, Po, Px, Py, Pz, Pxy, Pxz, Pyz, Pxyz,
				((mlib_s16 *)rays->results[cstp])[cray]);
		}
	}
	return (MLIB_SUCCESS);
}

/* *********************************************************** */
