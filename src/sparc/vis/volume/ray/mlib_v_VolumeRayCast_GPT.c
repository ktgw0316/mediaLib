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

#pragma ident	"@(#)mlib_v_VolumeRayCast_GPT.c	9.2	07/11/05 SMI"

/*
 *  FUNCTION
 *    mlib_VolumeRayCast_General_Parallel_Trilinear_[U8_U8|S16_S16]
 *      Cast parallel rays through a three dimensional data set
 *      and return the interpolated samples.
 *
 *  SYNOPSIS
 *    mlib_status mlib_VolumeRayCast_General_Parallel_Trilinear_[U8_U8|S16_S16]
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
 *
 */

#include <stdio.h>
#include <mlib_volume.h>
#include <mlib_SysMath.h>
#include <vis_proto.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_VolumeRayCast_General_Parallel_Trilinear_U8_U8 = \
__mlib_VolumeRayCast_General_Parallel_Trilinear_U8_U8
#pragma weak mlib_VolumeRayCast_General_Parallel_Trilinear_S16_S16 = \
	__mlib_VolumeRayCast_General_Parallel_Trilinear_S16_S16
#elif defined(__GNUC__)

__typeof__(__mlib_VolumeRayCast_General_Parallel_Trilinear_U8_U8)
	mlib_VolumeRayCast_General_Parallel_Trilinear_U8_U8
	__attribute__((weak,
	alias("__mlib_VolumeRayCast_General_Parallel_Trilinear_U8_U8")));
__typeof__(__mlib_VolumeRayCast_General_Parallel_Trilinear_S16_S16)
	mlib_VolumeRayCast_General_Parallel_Trilinear_S16_S16
	__attribute__((weak,
	alias("__mlib_VolumeRayCast_General_Parallel_Trilinear_S16_S16")));

#else

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	MLIB_PREC	65536.0

/* *********************************************************** */

static void mlib_v_VolumeRayCast_General_Parallel_Trilinear_U8_U8_OneRegion_4X(
	mlib_rays *rays,
	mlib_s32 nrays,
	mlib_s32 minsteps,
	mlib_s32 maxsteps,
	const mlib_genvolume *vol);

static void mlib_v_VolumeRayCast_General_Parallel_Trilinear_U8_U8_OneRegion(
	mlib_rays *rays,
	mlib_s32 nrays,
	mlib_s32 minsteps,
	mlib_s32 maxsteps,
	const mlib_genvolume *vol);

static void mlib_v_VolumeRayCast_General_Parallel_Trilinear_U8_U8_MultiRegion(
	mlib_rays *rays,
	mlib_s32 nrays,
	mlib_s32 minsteps,
	mlib_s32 maxsteps,
	const mlib_genvolume *vol);

/* *********************************************************** */

#define	FUN_NAME(REG) \
	mlib_v_VolumeRayCast_General_Parallel_Trilinear_U8_U8_##REG

mlib_status
__mlib_VolumeRayCast_General_Parallel_Trilinear_U8_U8(
	mlib_rays *rays,
	const mlib_genvolume *vol,
	void *buffer)
{
	mlib_s32 cray;
	mlib_s32 nregs = vol->nregions;
	mlib_s32 nrays = rays->nrays;
	mlib_s32 minsteps = INT_MAX;
	mlib_s32 maxsteps = -1;
	mlib_d64 maxdist = 0.0, zdelta;

	for (cray = 0; cray < nrays; cray++) {
		if (rays->nsteps[cray] < minsteps)
			minsteps = rays->nsteps[cray];

		if (rays->nsteps[cray] > maxsteps)
			maxsteps = rays->nsteps[cray];
#ifndef _LP64

		if (mlib_fabs(rays->starts[cray][2] - rays->starts[0][2]) >
			maxdist)
			maxdist =
				mlib_fabs(rays->starts[cray][2] -
				rays->starts[0][2]);
#else /* _LP64 */
		{
			mlib_d64 mod_zd = 0.0;
			mlib_d64 zd =
				(rays->starts[cray][2] - rays->starts[0][2]);

			if (zd < 0)
				mod_zd = 0.0 - zd;
			else
				mod_zd = zd;

			if (mod_zd > maxdist)
				maxdist = mod_zd;
		}

#endif /* _LP64 */
	}

	zdelta = (vol->zcoords[nregs] - vol->zcoords[0]) * 0.50;

	if (nregs == 1 && maxdist < zdelta) {

		if ((nrays % 4 == 0) && !((mlib_addr)rays->results & 3) &&
			(maxsteps % 4 == 0)) {

			FUN_NAME(OneRegion_4X)
				(rays, nrays, minsteps, maxsteps, vol);
		} else {
			FUN_NAME(OneRegion)
				(rays, nrays, minsteps, maxsteps, vol);
		}
	} else {
		FUN_NAME(MultiRegion)
			(rays, nrays, minsteps, maxsteps, vol);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#define	LOAD_8_NEIGHBOR_VOXELS_U8                               \
	currX = Ox1616 + cx[cray];                              \
	currY = Oy1616 + cy[cray];                              \
	currZ = Oz1616 + cz[cray];                              \
	Xint = currX >> 16;                                     \
	Yint = currY >> 16;                                     \
	Zint = currZ >> 16;                                     \
	sp = (mlib_u8 *)((mlib_u8 *)vol->slices[Zint] +         \
		vol->offsets[Yint]) + Xint;                     \
	P1o = vis_ld_u8(sp++);                                  \
	P1x = vis_ld_u8(sp);                                    \
	sp = (mlib_u8 *)((mlib_u8 *)vol->slices[Zint] +         \
		vol->offsets[Yint + 1]) + Xint;                 \
	P1y = vis_ld_u8(sp++);                                  \
	P1xy = vis_ld_u8(sp);                                   \
	sp = (mlib_u8 *)((mlib_u8 *)vol->slices[Zint + 1] +     \
		vol->offsets[Yint]) + Xint;                     \
	P1z = vis_ld_u8(sp++);                                  \
	P1xz = vis_ld_u8(sp);                                   \
	sp = (mlib_u8 *)((mlib_u8 *)vol->slices[Zint + 1] +     \
		vol->offsets[Yint + 1]) + Xint;                 \
	P1yz = vis_ld_u8(sp++);                                 \
	P1xyz = vis_ld_u8(sp);                                  \
	Po = vis_faligndata(P1o, Po);                           \
	Px = vis_faligndata(P1x, Px);                           \
	Py = vis_faligndata(P1y, Py);                           \
	Pxy = vis_faligndata(P1xy, Pxy);                        \
	Pz = vis_faligndata(P1z, Pz);                           \
	Pxz = vis_faligndata(P1xz, Pxz);                        \
	Pyz = vis_faligndata(P1yz, Pyz);                        \
	Pxyz = vis_faligndata(P1xyz, Pxyz)

/* *********************************************************** */

#define	PREPARE_VIS_FRACTIONS_FOR_FOUR_VOXELS_INTERPOLATION             \
	for (i = 0, j = 0; i < nrays / 4; i++, j += 4) {                \
	    cx_frac[i] =                                                \
		    vis_to_double(((cx[j] & 0xfff0) << 12) | ((cx[j +   \
		    1] & 0xfff0) >> 4),                                 \
		    ((cx[j + 2] & 0xfff0) << 12) | ((cx[j +             \
		    3] & 0xfff0) >> 4));                                \
	    cy_frac[i] =                                                \
		    vis_to_double(((cy[j] & 0xfff0) << 12) | ((cy[j +   \
		    1] & 0xfff0) >> 4),                                 \
		    ((cy[j + 2] & 0xfff0) << 12) | ((cy[j +             \
		    3] & 0xfff0) >> 4));                                \
	    cz_frac[i] =                                                \
		    vis_to_double(((cz[j] & 0xfff0) << 12) | ((cz[j +   \
		    1] & 0xfff0) >> 4),                                 \
		    ((cz[j + 2] & 0xfff0) << 12) | ((cz[j +             \
		    3] & 0xfff0) >> 4));                                \
	}

/* *********************************************************** */

#define	PREPARE_VIS_FRACTIONS_FOR_ONE_VOXEL_INTERPOLATION                  \
	for (cray = 0; cray < nrays; cray++) {                             \
	    cx_frac[cray] =                                                \
		    vis_to_double_dup(((cx[cray] & 0xfff0) << 12) |        \
		    ((cx[cray] & 0xfff0) >> 4));                           \
	    cy_frac[cray] = vis_to_double_dup((cy[cray] & 0xfff0) >> 4);   \
	    cz_frac[cray] = vis_to_double_dup((cz[cray] & 0xfff0) >> 4);   \
	}

/* *********************************************************** */

#define	PREPARE_THE_VOXEL_IN_HOME_RAY                                      \
	Oz1616 = (mlib_s32)(((Oz -                                         \
		vol->zcoords[0]) * vol->zsrates[0]) * MLIB_PREC + 0.5);    \
	Ox1616 = (mlib_s32)(Ox * MLIB_PREC + 0.5);                         \
	Oy1616 = (mlib_s32)(Oy * MLIB_PREC + 0.5);                         \
	Ox_frac4 =                                                         \
	vis_to_double_dup(((Ox1616 & 0xfff0) << 12) | ((Ox1616 & 0xfff0)   \
		>> 4));                                                    \
	Oy_frac4 =                                                         \
	vis_to_double_dup(((Oy1616 & 0xfff0) << 12) | ((Oy1616 & 0xfff0)   \
		>> 4));                                                    \
	Oz_frac4 =                                                         \
	vis_to_double_dup(((Oz1616 & 0xfff0) << 12) | ((Oz1616 & 0xfff0)   \
		>> 4))

/* *********************************************************** */

#define	VIS_TRILINEAR_1_VOXEL_INTER_1REG_U8                     \
	sd0 = vis_fpadd16(Ox_frac4, cx_frac[cray]);             \
	sd1 = vis_fand(sd0, mask12bits);                        \
	aaaa = vis_fpack16(sd1);                                \
	sd0 = vis_fpadd16(Oy_frac4, cy_frac[cray]);             \
	sd1 = vis_fand(sd0, mask12bits);                        \
	bbbb = vis_fpack16(sd1);                                \
	sd0 = vis_fpadd16(Oz_frac4, cz_frac[cray]);             \
	sd1 = vis_fand(sd0, mask12bits);                        \
	cccc = vis_fpack16(sd1);                                \
	currX = Ox1616 + cx[cray];                              \
	currY = Oy1616 + cy[cray];                              \
	currZ = Oz1616 + cz[cray];                              \
	Xint = currX >> 16;                                     \
	Yint = currY >> 16;                                     \
	Zint = currZ >> 16;                                     \
	sp = (mlib_u8 *)((mlib_u8 *)vol->slices[Zint] +         \
		vol->offsets[Yint]) + Xint;                     \
	Po = vis_ld_u8(sp++);                                   \
	Px = vis_ld_u8(sp);                                     \
	sp = (mlib_u8 *)((mlib_u8 *)vol->slices[Zint] +         \
		vol->offsets[Yint + 1]) + Xint;                 \
	Py = vis_ld_u8(sp++);                                   \
	Pxy = vis_ld_u8(sp);                                    \
	sp = (mlib_u8 *)((mlib_u8 *)vol->slices[Zint + 1] +     \
		vol->offsets[Yint]) + Xint;                     \
	Pz = vis_ld_u8(sp++);                                   \
	Pxz = vis_ld_u8(sp);                                    \
	sp = (mlib_u8 *)((mlib_u8 *)vol->slices[Zint + 1] +     \
		vol->offsets[Yint + 1]) + Xint;                 \
	Pyz = vis_ld_u8(sp++);                                  \
	Pxyz = vis_ld_u8(sp);                                   \
	sd0 = vis_faligndata(Po, sd0);                          \
	sd0 = vis_faligndata(Py, sd0);                          \
	sd0 = vis_faligndata(Pz, sd0);                          \
	sd0 = vis_faligndata(Pyz, sd0);                         \
	sd0 = vis_fexpand(vis_read_hi(sd0));                    \
	sd1 = vis_faligndata(Px, sd1);                          \
	sd1 = vis_faligndata(Pxy, sd1);                         \
	sd1 = vis_faligndata(Pxz, sd1);                         \
	sd1 = vis_faligndata(Pxyz, sd1);                        \
	sd1 = vis_fexpand(vis_read_hi(sd1));                    \
	sd2 = vis_fpsub16(sd1, sd0);                            \
	sd3 = vis_fmul8x16(aaaa, sd2);                          \
	Px3210 = vis_fpadd16(sd0, sd3);                         \
	sd0 = vis_faligndata(Px3210, Px3210);                   \
	Px0321 = vis_faligndata(sd0, sd0);                      \
	sd0 = vis_fpsub16(Px0321, Px3210);                      \
	sd1 = vis_fmul8x16(bbbb, sd0);                          \
	Py10 = vis_fpadd16(Px3210, sd1);                        \
	sd2 = vis_write_lo(sd2, vis_read_hi(Py10));             \
	sd3 = vis_fpsub16(sd2, Py10);                           \
	sd0 = vis_fmul8x16(cccc, sd3);                          \
	sd1 = vis_fpadd16(Py10, sd0);                           \
	sd2 = vis_fmul8x16(rsh4wrounding, sd1)

/* *********************************************************** */

void
mlib_v_VolumeRayCast_General_Parallel_Trilinear_U8_U8_OneRegion_4X(
	mlib_rays *rays,
	mlib_s32 nrays,
	mlib_s32 minsteps,
	mlib_s32 maxsteps,
	const mlib_genvolume *vol)
{
	mlib_s32 i, j;
	mlib_s32 cray;
	mlib_s32 cstp;
	mlib_d64 Ox, Oy, Oz;
	mlib_d64 dOx, dOy, dOz;
	mlib_s32 Ox1616, Oy1616, Oz1616;
	mlib_s32 currX, currY, currZ;
	mlib_s32 cx[64], cy[64], cz[64];
	mlib_d64 Ox_frac4;
	mlib_d64 Oy_frac4;
	mlib_d64 Oz_frac4;
	mlib_d64 cx_frac[64];
	mlib_d64 cy_frac[64];
	mlib_d64 cz_frac[64];
	mlib_s32 Xint, Yint, Zint;
	mlib_d64 Po, Px, Py, Pz;
	mlib_d64 Pxy, Pxz, Pyz, Pxyz;
	mlib_d64 Px0, Px1, Px2, Px3;
	mlib_d64 Py0, Py1;
	mlib_f32 P;
	mlib_d64 P1o, P1x, P1y, P1z;
	mlib_d64 P1xy, P1xz, P1yz, P1xyz;
	mlib_d64 Px3210, Px0321, Py10;
	mlib_d64 sd0, sd1, sd2, sd3;
	mlib_f32 aaaa, bbbb, cccc;
	mlib_u8 *sp;
	mlib_u8 *dp;
	mlib_f32 *dp_f32;
	mlib_d64 mask12bits = vis_to_double_dup(0x0fff0fff);
	mlib_d64 roundings = vis_to_double_dup(0x00080008);
	mlib_f32 rsh4wrounding = vis_to_float(0x10101010);

	Ox = rays->starts[0][0];
	Oy = rays->starts[0][1];
	Oz = rays->starts[0][2];

	dOx = rays->incs[0][0];
	dOy = rays->incs[0][1];
	dOz = rays->incs[0][2] * vol->zsrates[0];

	for (cray = 0; cray < nrays; cray++) {
		cx[cray] =
			(mlib_s32)((rays->starts[cray][0] - Ox) * MLIB_PREC +
			0.5);
		cy[cray] =
			(mlib_s32)((rays->starts[cray][1] - Oy) * MLIB_PREC +
			0.5);
		cz[cray] =
			(mlib_s32)((rays->starts[cray][2] * vol->zsrates[0] -
			Oz) * MLIB_PREC + 0.5);
	}

	vis_write_gsr((3 << 3) + 7);

	PREPARE_VIS_FRACTIONS_FOR_FOUR_VOXELS_INTERPOLATION;

	for (cstp = 0; cstp < minsteps; cstp++) {

		dp_f32 = (mlib_f32 *)rays->results[cstp];

		PREPARE_THE_VOXEL_IN_HOME_RAY;

#pragma pipeloop(0)
		for (i = 0, cray = 3; i < nrays / 4; i++) {

			sd0 = vis_fpadd16(Ox_frac4, cx_frac[i]);
			sd1 = vis_fand(sd0, mask12bits);
			aaaa = vis_fpack16(sd1);

			sd0 = vis_fpadd16(Oy_frac4, cy_frac[i]);
			sd1 = vis_fand(sd0, mask12bits);
			bbbb = vis_fpack16(sd1);

			sd0 = vis_fpadd16(Oz_frac4, cz_frac[i]);
			sd1 = vis_fand(sd0, mask12bits);
			cccc = vis_fpack16(sd1);

			LOAD_8_NEIGHBOR_VOXELS_U8;
			cray--;

			LOAD_8_NEIGHBOR_VOXELS_U8;
			cray--;

			LOAD_8_NEIGHBOR_VOXELS_U8;
			cray--;

			LOAD_8_NEIGHBOR_VOXELS_U8;
			cray += 7;

			Po = vis_fexpand(vis_read_hi(Po));
			Px = vis_fexpand(vis_read_hi(Px));
			Py = vis_fexpand(vis_read_hi(Py));
			Pxy = vis_fexpand(vis_read_hi(Pxy));
			Pz = vis_fexpand(vis_read_hi(Pz));
			Pxz = vis_fexpand(vis_read_hi(Pxz));
			Pyz = vis_fexpand(vis_read_hi(Pyz));
			Pxyz = vis_fexpand(vis_read_hi(Pxyz));

			sd0 = vis_fpsub16(Px, Po);
			sd1 = vis_fmul8x16(aaaa, sd0);
			Px0 = vis_fpadd16(Po, sd1);

			sd0 = vis_fpsub16(Pxy, Py);
			sd1 = vis_fmul8x16(aaaa, sd0);
			Px1 = vis_fpadd16(Py, sd1);

			sd0 = vis_fpsub16(Pxz, Pz);
			sd1 = vis_fmul8x16(aaaa, sd0);
			Px2 = vis_fpadd16(Pz, sd1);

			sd0 = vis_fpsub16(Pxyz, Pyz);
			sd1 = vis_fmul8x16(aaaa, sd0);
			Px3 = vis_fpadd16(Pyz, sd1);

			sd0 = vis_fpsub16(Px1, Px0);
			sd1 = vis_fmul8x16(bbbb, sd0);
			Py0 = vis_fpadd16(Px0, sd1);

			sd0 = vis_fpsub16(Px3, Px2);
			sd1 = vis_fmul8x16(bbbb, sd0);
			Py1 = vis_fpadd16(Px2, sd1);

			sd0 = vis_fpsub16(Py1, Py0);
			sd1 = vis_fmul8x16(cccc, sd0);
			sd2 = vis_fpadd16(Py0, sd1);

			sd3 = vis_fpadd16(sd2, roundings);
			P = vis_fpack16(sd3);

			(*dp_f32++) = P;
		}

		Ox += dOx;
		Oy += dOy;
		Oz += dOz;
	}

	if (minsteps < maxsteps) {

		PREPARE_VIS_FRACTIONS_FOR_ONE_VOXEL_INTERPOLATION;

		for (cstp = minsteps; cstp < maxsteps; cstp++) {

			dp = (mlib_u8 *)rays->results[cstp];

			PREPARE_THE_VOXEL_IN_HOME_RAY;

#pragma pipeloop(0)
			for (cray = 0; cray < nrays; cray++) {

				if (cstp < rays->nsteps[cray]) {

					VIS_TRILINEAR_1_VOXEL_INTER_1REG_U8;

					vis_st_u8(sd2, dp++);
				} else {
					dp++;
				}
			}

			Ox += dOx;
			Oy += dOy;
			Oz += dOz;
		}
	}
}

/* *********************************************************** */

void
mlib_v_VolumeRayCast_General_Parallel_Trilinear_U8_U8_OneRegion(
	mlib_rays *rays,
	mlib_s32 nrays,
	mlib_s32 minsteps,
	mlib_s32 maxsteps,
	const mlib_genvolume *vol)
{
	mlib_s32 cray;
	mlib_s32 cstp;
	mlib_d64 Ox, Oy, Oz;
	mlib_d64 dOx, dOy, dOz;
	mlib_s32 Ox1616, Oy1616, Oz1616;
	mlib_s32 currX, currY, currZ;
	mlib_s32 cx[64], cy[64], cz[64];
	mlib_d64 Ox_frac4;
	mlib_d64 Oy_frac4;
	mlib_d64 Oz_frac4;
	mlib_d64 cx_frac[64];
	mlib_d64 cy_frac[64];
	mlib_d64 cz_frac[64];
	mlib_s32 Xint, Yint, Zint;
	mlib_d64 Po, Px, Py, Pz;
	mlib_d64 Pxy, Pxz, Pyz, Pxyz;
	mlib_d64 sd0, sd1, sd2, sd3;
	mlib_d64 Px3210, Px0321, Py10;
	mlib_f32 aaaa, bbbb, cccc;
	mlib_u8 *sp;
	mlib_u8 *dp;
	mlib_d64 mask12bits = vis_to_double_dup(0x0fff0fff);
	mlib_f32 rsh4wrounding = vis_to_float(0x10101010);

	Ox = rays->starts[0][0];
	Oy = rays->starts[0][1];
	Oz = rays->starts[0][2];

	dOx = rays->incs[0][0];
	dOy = rays->incs[0][1];
	dOz = rays->incs[0][2] * vol->zsrates[0];

	for (cray = 0; cray < nrays; cray++) {
		cx[cray] =
			(mlib_s32)((rays->starts[cray][0] - Ox) * MLIB_PREC +
			0.5);
		cy[cray] =
			(mlib_s32)((rays->starts[cray][1] - Oy) * MLIB_PREC +
			0.5);
		cz[cray] =
			(mlib_s32)((rays->starts[cray][2] * vol->zsrates[0] -
			Oz) * MLIB_PREC + 0.5);
	}

	vis_write_gsr((3 << 3) + 7);

	PREPARE_VIS_FRACTIONS_FOR_ONE_VOXEL_INTERPOLATION;

	for (cstp = 0; cstp < minsteps; cstp++) {

		dp = (mlib_u8 *)rays->results[cstp];

		PREPARE_THE_VOXEL_IN_HOME_RAY;

#pragma pipeloop(0)
		for (cray = 0; cray < nrays; cray++) {

			VIS_TRILINEAR_1_VOXEL_INTER_1REG_U8;

			vis_st_u8(sd2, dp++);
		}

		Ox += dOx;
		Oy += dOy;
		Oz += dOz;
	}

	for (cstp = minsteps; cstp < maxsteps; cstp++) {

		dp = (mlib_u8 *)rays->results[cstp];

		PREPARE_THE_VOXEL_IN_HOME_RAY;

#pragma pipeloop(0)
		for (cray = 0; cray < nrays; cray++) {
			if (cstp < rays->nsteps[cray]) {

				VIS_TRILINEAR_1_VOXEL_INTER_1REG_U8;

				vis_st_u8(sd2, dp++);
			} else {
				dp++;
			}
		}

		Ox += dOx;
		Oy += dOy;
		Oz += dOz;
	}
}

/* *********************************************************** */

#define	VIS_TRILINEAR_ONE_VOXEL_INTER_MULTIREGION_U8            \
	sd0 = vis_fpadd16(Ox_frac4, cx_frac[cray]);             \
	sd1 = vis_fand(sd0, mask12bits);                        \
	aaaa = vis_fpack16(sd1);                                \
	sd0 = vis_fpadd16(Oy_frac2, cy_frac[cray]);             \
	sd1 = vis_fand(sd0, mask12bits);                        \
	bbbb = vis_fpack16(sd1);                                \
	cccc = vis_read_lo(vis_fpack32(sd0, ssZ_frac[cray]));   \
	Xint = currX >> 16;                                     \
	Yint = currY >> 16;                                     \
	Zint = ssZ[cray] >> 16;                                 \
	sp = (mlib_u8 *)((mlib_u8 *)vol->slices[Zint] +         \
		vol->offsets[Yint]) + Xint;                     \
	Po = vis_ld_u8(sp++);                                   \
	Px = vis_ld_u8(sp);                                     \
	sp = (mlib_u8 *)((mlib_u8 *)vol->slices[Zint] +         \
		vol->offsets[Yint + 1]) + Xint;                 \
	Py = vis_ld_u8(sp++);                                   \
	Pxy = vis_ld_u8(sp);                                    \
	sp = (mlib_u8 *)((mlib_u8 *)vol->slices[Zint + 1] +     \
		vol->offsets[Yint]) + Xint;                     \
	Pz = vis_ld_u8(sp++);                                   \
	Pxz = vis_ld_u8(sp);                                    \
	sp = (mlib_u8 *)((mlib_u8 *)vol->slices[Zint + 1] +     \
		vol->offsets[Yint + 1]) + Xint;                 \
	Pyz = vis_ld_u8(sp++);                                  \
	Pxyz = vis_ld_u8(sp);                                   \
	sd0 = vis_faligndata(Po, sd0);                          \
	sd0 = vis_faligndata(Py, sd0);                          \
	sd0 = vis_faligndata(Pz, sd0);                          \
	sd0 = vis_faligndata(Pyz, sd0);                         \
	sd0 = vis_fexpand(vis_read_hi(sd0));                    \
	sd1 = vis_faligndata(Px, sd1);                          \
	sd1 = vis_faligndata(Pxy, sd1);                         \
	sd1 = vis_faligndata(Pxz, sd1);                         \
	sd1 = vis_faligndata(Pxyz, sd1);                        \
	sd1 = vis_fexpand(vis_read_hi(sd1));                    \
	sd2 = vis_fpsub16(sd1, sd0);                            \
	sd3 = vis_fmul8x16(aaaa, sd2);                          \
	Px3210 = vis_fpadd16(sd0, sd3);                         \
	sd0 = vis_faligndata(Px3210, Px3210);                   \
	Px0321 = vis_faligndata(sd0, sd0);                      \
	sd0 = vis_fpsub16(Px0321, Px3210);                      \
	sd1 = vis_fmul8x16(bbbb, sd0);                          \
	Py10 = vis_fpadd16(Px3210, sd1);                        \
	sd2 = vis_write_lo(sd2, vis_read_hi(Py10));             \
	sd3 = vis_fpsub16(sd2, Py10);                           \
	sd0 = vis_fmul8x16(cccc, sd3);                          \
	sd1 = vis_fpadd16(Py10, sd0);                           \
	sd2 = vis_fmul8x16(rsh4wrounding, sd1)

/* *********************************************************** */

void
mlib_v_VolumeRayCast_General_Parallel_Trilinear_U8_U8_MultiRegion(
	mlib_rays *rays,
	mlib_s32 nrays,
	mlib_s32 minsteps,
	mlib_s32 maxsteps,
	const mlib_genvolume *vol)
{
	mlib_s32 cray;
	mlib_s32 cstp;
	mlib_s32 creg;
	mlib_s32 ssZ[64], ssZinc[64];
	mlib_s32 reg[64];
	mlib_d64 Ox, Oy, Oz;
	mlib_s32 Ox1616, Oy1616;
	mlib_s32 currX, currY;
	mlib_d64 currZ;
	mlib_d64 dOx, dOy, dOz;
	mlib_s32 cx[64], cy[64];
	mlib_d64 cz[64];
	mlib_d64 cx_frac[64];
	mlib_d64 cy_frac[64];
	mlib_d64 Ox_frac4, Oy_frac2;
	mlib_d64 ssZinc_frac[64];
	mlib_d64 ssZ_frac[64];
	mlib_s32 Xint, Yint, Zint;
	mlib_d64 Po, Px, Py, Pz;
	mlib_d64 Pxy, Pxz, Pyz, Pxyz;
	mlib_d64 sd0, sd1, sd2, sd3;
	mlib_d64 Px3210, Px0321, Py10;
	mlib_f32 aaaa, bbbb, cccc;
	mlib_u8 *sp;
	mlib_u8 *dp;
	mlib_d64 mask28bits = vis_to_double_dup(0x0fffffff);
	mlib_d64 mask12bits = vis_to_double_dup(0x0fff0fff);
	mlib_f32 rsh4wrounding = vis_to_float(0x10101010);

	Ox = rays->starts[0][0];
	Oy = rays->starts[0][1];
	Oz = rays->starts[0][2];

	dOx = rays->incs[0][0];
	dOy = rays->incs[0][1];
	dOz = rays->incs[0][2];

	for (cray = 0; cray < nrays; cray++) {
		cx[cray] =
			(mlib_s32)((rays->starts[cray][0] - Ox) * MLIB_PREC +
			0.5);
		cy[cray] =
			(mlib_s32)((rays->starts[cray][1] - Oy) * MLIB_PREC +
			0.5);
		cz[cray] = rays->starts[cray][2] - Oz;

		creg = 0;
		while (rays->starts[cray][2] > vol->zcoords[creg + 1])
			creg++;
		reg[cray] = creg;

		ssZ[cray] =
			(mlib_s32)(((rays->starts[cray][2] -
			vol->zcoords[creg]) * vol->zsrates[creg] +
			vol->zindices[creg]) * MLIB_PREC + 0.5);
		ssZinc[cray] =
			(mlib_s32)(vol->zsrates[creg] * dOz * MLIB_PREC + 0.5);

		cx_frac[cray] =
			vis_to_double_dup(((cx[cray] & 0xfff0) << 12) |
			((cx[cray] & 0xfff0) >> 4));
		cy_frac[cray] = vis_to_double_dup((cy[cray] & 0xfff0) >> 4);

		ssZ_frac[cray] = vis_to_double_dup((ssZ[cray] & 0xffff) << 12);
		ssZinc_frac[cray] =
			vis_to_double_dup((ssZinc[cray] & 0xffff) << 12);
	}

	vis_write_gsr((3 << 3) + 7);

	for (cstp = 0; cstp < minsteps; cstp++) {

		dp = (mlib_u8 *)rays->results[cstp];

		Ox1616 = (mlib_s32)(Ox * MLIB_PREC + 0.5);
		Oy1616 = (mlib_s32)(Oy * MLIB_PREC + 0.5);

		Ox_frac4 =
			vis_to_double_dup(((Ox1616 & 0xfff0) << 12) | ((Ox1616 &
			0xfff0) >> 4));
		Oy_frac2 = vis_to_double_dup((Oy1616 & 0xfff0) >> 4);

#pragma pipeloop(0)
		for (cray = 0; cray < nrays; cray++) {

			currX = Ox1616 + cx[cray];
			currY = Oy1616 + cy[cray];
			currZ = Oz + cz[cray];

			creg = reg[cray];

			if (dOz > 0.0)
				while (currZ > vol->zcoords[creg + 1])
					creg++;
			else if (dOz < 0.0)
				while (currZ < vol->zcoords[creg])
					creg--;

			if (creg != reg[cray]) {
				reg[cray] = creg;

				ssZ[cray] =
					(mlib_s32)(((currZ -
					vol->zcoords[creg]) *
					vol->zsrates[creg] +
					vol->zindices[creg]) * MLIB_PREC + 0.5);
				ssZinc[cray] =
					(mlib_s32)(vol->zsrates[creg] * dOz *
					MLIB_PREC + 0.5);

				ssZ_frac[cray] =
					vis_to_double_dup((ssZ[cray] & 0xffff)
					<< 12);
				ssZinc_frac[cray] =
					vis_to_double_dup((ssZinc[cray] &
					0xffff) << 12);
			}

			VIS_TRILINEAR_ONE_VOXEL_INTER_MULTIREGION_U8;

			vis_st_u8(sd2, dp++);

			ssZ[cray] += ssZinc[cray];
			ssZ_frac[cray] =
				vis_fpadd32(ssZ_frac[cray], ssZinc_frac[cray]);
			ssZ_frac[cray] = vis_fand(ssZ_frac[cray], mask28bits);
		}

		Ox += dOx;
		Oy += dOy;
		Oz += dOz;
	}

	for (cstp = minsteps; cstp < maxsteps; cstp++) {

		dp = (mlib_u8 *)rays->results[cstp];

		Ox1616 = (mlib_s32)(Ox * MLIB_PREC + 0.5);
		Oy1616 = (mlib_s32)(Oy * MLIB_PREC + 0.5);

		Ox_frac4 =
			vis_to_double_dup(((Ox1616 & 0xfff0) << 12) | ((Ox1616 &
			0xfff0) >> 4));
		Oy_frac2 = vis_to_double_dup((Oy1616 & 0xfff0) >> 4);

#pragma pipeloop(0)
		for (cray = 0; cray < nrays; cray++) {
			if (cstp < rays->nsteps[cray]) {

				currX = Ox1616 + cx[cray];
				currY = Oy1616 + cy[cray];
				currZ = Oz + cz[cray];

				creg = reg[cray];

				if (dOz > 0.0)
					while (currZ > vol->zcoords[creg + 1])
						creg++;
				else if (dOz < 0.0)
					while (currZ < vol->zcoords[creg])
						creg--;

				if (creg != reg[cray]) {
					reg[cray] = creg;

					ssZ[cray] =
						(mlib_s32)(((currZ -
						vol->zcoords[creg]) *
						vol->zsrates[creg]
						+
						vol->zindices[creg]) *
						MLIB_PREC + 0.5);
					ssZinc[cray] =
						(mlib_s32)(vol->zsrates[creg] *
						dOz * MLIB_PREC + 0.5);

					ssZ_frac[cray] =
						vis_to_double_dup((ssZ[cray] &
						0xffff) << 12);
					ssZinc_frac[cray] =
						vis_to_double_dup((ssZinc[cray]
						& 0xffff) << 12);
				}

				VIS_TRILINEAR_ONE_VOXEL_INTER_MULTIREGION_U8;

				vis_st_u8(sd2, dp++);

				ssZ[cray] += ssZinc[cray];
				ssZ_frac[cray] =
					vis_fpadd32(ssZ_frac[cray],
					ssZinc_frac[cray]);
				ssZ_frac[cray] =
					vis_fand(ssZ_frac[cray], mask28bits);
			} else {
				dp++;
			}
		}

		Ox += dOx;
		Oy += dOy;
		Oz += dOz;
	}
}

/* *********************************************************** */

static void
mlib_v_VolumeRayCast_General_Parallel_Trilinear_S16_S16_OneRegion_8X(
	mlib_rays *rays,
	mlib_s32 nrays,
	mlib_s32 minsteps,
	mlib_s32 maxsteps,
	const mlib_genvolume *vol);

static void
mlib_v_VolumeRayCast_General_Parallel_Trilinear_S16_S16_OneRegion_4X(
	mlib_rays *rays,
	mlib_s32 nrays,
	mlib_s32 minsteps,
	mlib_s32 maxsteps,
	const mlib_genvolume *vol);

static void mlib_v_VolumeRayCast_General_Parallel_Trilinear_S16_S16_OneRegion(
	mlib_rays *rays,
	mlib_s32 nrays,
	mlib_s32 minsteps,
	mlib_s32 maxsteps,
	const mlib_genvolume *vol);

static void mlib_v_VolumeRayCast_General_Parallel_Trilinear_S16_S16_MultiRegion(
	mlib_rays *rays,
	mlib_s32 nrays,
	mlib_s32 minsteps,
	mlib_s32 maxsteps,
	const mlib_genvolume *vol);

/* *********************************************************** */

#undef	FUN_NAME
#define	FUN_NAME(REG) \
	mlib_v_VolumeRayCast_General_Parallel_Trilinear_S16_S16_##REG

mlib_status
__mlib_VolumeRayCast_General_Parallel_Trilinear_S16_S16(
	mlib_rays *rays,
	const mlib_genvolume *vol,
	void *buffer)
{
	mlib_s32 cray;
	mlib_s32 nregs = vol->nregions;
	mlib_s32 nrays = rays->nrays;
	mlib_s32 minsteps = INT_MAX;
	mlib_s32 maxsteps = -1;
	mlib_d64 maxdist = 0.0, zdelta = 0.0, zdelta4 = 0.0;

	for (cray = 0; cray < nrays; cray++) {
		if (rays->nsteps[cray] < minsteps)
			minsteps = rays->nsteps[cray];

		if (rays->nsteps[cray] > maxsteps)
			maxsteps = rays->nsteps[cray];
#ifndef _LP64

		if (mlib_fabs(rays->starts[cray][2] - rays->starts[0][2]) >
			maxdist)
			maxdist =
				mlib_fabs(rays->starts[cray][2] -
				rays->starts[0][2]);
#else /* _LP64 */
		{
			mlib_d64 mod_zd = 0.0;
			mlib_d64 zd =
				(rays->starts[cray][2] - rays->starts[0][2]);

			if (zd < 0)
				mod_zd = 0.0 - zd;
			else
				mod_zd = zd;

			if (mod_zd > maxdist)
				maxdist = mod_zd;
		}

#endif /* _LP64 */
	}

	zdelta = (vol->zcoords[nregs] - vol->zcoords[0]) * 0.78;
	zdelta4 = (vol->zcoords[nregs] - vol->zcoords[0]) * 1.00;

	if (nregs == 1) {

		if (!((mlib_addr)rays->results & 7) && (maxsteps % 4 == 0)) {

			if (nrays % 8 == 0 && maxdist < zdelta4)

				FUN_NAME(OneRegion_8X)
					(rays, nrays, minsteps, maxsteps, vol);
			else if (nrays % 4 == 0 && maxdist < zdelta4)

				FUN_NAME(OneRegion_4X)
					(rays, nrays, minsteps, maxsteps, vol);
			else if (maxdist < zdelta)

				FUN_NAME(OneRegion)
					(rays, nrays, minsteps, maxsteps, vol);
			else
				FUN_NAME(MultiRegion)
					(rays, nrays, minsteps, maxsteps, vol);
		} else if (maxdist < zdelta)

			FUN_NAME(OneRegion)
				(rays, nrays, minsteps, maxsteps, vol);
		else
			FUN_NAME(MultiRegion)
				(rays, nrays, minsteps, maxsteps, vol);
	} else
		FUN_NAME(MultiRegion)
			(rays, nrays, minsteps, maxsteps, vol);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#define	LOAD_8_NEIGHBOR_VOXELS                                           \
	currX = Ox1616 + cx[cray];                                       \
	currY = Oy1616 + cy[cray];                                       \
	currZ = Oz1616 + cz[cray];                                       \
	Xint = currX >> 16;                                              \
	Yint = currY >> 16;                                              \
	Zint = currZ >> 16;                                              \
	sp = (mlib_s16 *)((mlib_u8 *)slices[Zint++] + offsets[Yint]) +   \
		Xint;                                                    \
	P1o = vis_ld_u16(sp);                                            \
	Po = vis_faligndata(P1o, Po);                                    \
	P1x = vis_ld_u16(sp + 1);                                        \
	Px = vis_faligndata(P1x, Px);                                    \
	sp = (mlib_s16 *)((mlib_u8 *)sp + ystride);                      \
	P1y = vis_ld_u16(sp++);                                          \
	Py = vis_faligndata(P1y, Py);                                    \
	P1xy = vis_ld_u16(sp);                                           \
	Pxy = vis_faligndata(P1xy, Pxy);                                 \
	sp = (mlib_s16 *)((mlib_u8 *)slices[Zint] + offsets[Yint]) +     \
		Xint;                                                    \
	P1z = vis_ld_u16(sp);                                            \
	Pz = vis_faligndata(P1z, Pz);                                    \
	P1xz = vis_ld_u16(sp + 1);                                       \
	Pxz = vis_faligndata(P1xz, Pxz);                                 \
	sp = (mlib_s16 *)((mlib_u8 *)sp + ystride);                      \
	P1yz = vis_ld_u16(sp++);                                         \
	Pyz = vis_faligndata(P1yz, Pyz);                                 \
	P1xyz = vis_ld_u16(sp);                                          \
	Pxyz = vis_faligndata(P1xyz, Pxyz)

/* *********************************************************** */

#define	VIS_TRILINEAR_1_VOXEL_INTER_1REG_S16                    \
	sd0 = vis_fpadd16(Ox_frac4, cx_frac[cray]);             \
	sd1 = vis_fand(sd0, mask12bits);                        \
	aaaa = vis_fpack16(sd1);                                \
	sd0 = vis_fpadd16(Oy_frac4, cy_frac[cray]);             \
	sd1 = vis_fand(sd0, mask12bits);                        \
	bbbb = vis_fpack16(sd1);                                \
	sd2 = vis_fpadd16(Oz_frac4, cz_frac[cray]);             \
	sd3 = vis_fand(sd2, mask12bits);                        \
	cccc = vis_fpack16(sd3);                                \
	currX = Ox1616 + cx[cray];                              \
	currY = Oy1616 + cy[cray];                              \
	currZ = Oz1616 + cz[cray];                              \
	Xint = currX >> 16;                                     \
	Yint = currY >> 16;                                     \
	Zint = currZ >> 16;                                     \
	sp = (mlib_s16 *)((mlib_u8 *)vol->slices[Zint] +        \
		vol->offsets[Yint]) + Xint;                     \
	Po = vis_ld_u16(sp++);                                  \
	Px = vis_ld_u16(sp);                                    \
	sp = (mlib_s16 *)((mlib_u8 *)vol->slices[Zint] +        \
		vol->offsets[Yint + 1]) + Xint;                 \
	Py = vis_ld_u16(sp++);                                  \
	Pxy = vis_ld_u16(sp);                                   \
	sp = (mlib_s16 *)((mlib_u8 *)vol->slices[Zint + 1] +    \
		vol->offsets[Yint]) + Xint;                     \
	Pz = vis_ld_u16(sp++);                                  \
	Pxz = vis_ld_u16(sp);                                   \
	sp = (mlib_s16 *)((mlib_u8 *)vol->slices[Zint + 1] +    \
		vol->offsets[Yint + 1]) + Xint;                 \
	Pyz = vis_ld_u16(sp++);                                 \
	Pxyz = vis_ld_u16(sp);                                  \
	sd0 = vis_faligndata(Po, sd0);                          \
	sd0 = vis_faligndata(Py, sd0);                          \
	sd0 = vis_faligndata(Pz, sd0);                          \
	sd0 = vis_faligndata(Pyz, sd0);                         \
	sd1 = vis_faligndata(Px, sd1);                          \
	sd1 = vis_faligndata(Pxy, sd1);                         \
	sd1 = vis_faligndata(Pxz, sd1);                         \
	sd1 = vis_faligndata(Pxyz, sd1);                        \
	sd2 = vis_fmul8x16(aaaa, sd1);                          \
	sd3 = vis_fmul8x16(aaaa, sd0);                          \
	sd3 = vis_fpsub16(sd2, sd3);                            \
	Px3210 = vis_fpadd16(sd0, sd3);                         \
	Px0321 = vis_faligndata(Px3210, Px3210);                \
	sd0 = vis_fmul8x16(bbbb, Px0321);                       \
	sd1 = vis_fmul8x16(bbbb, Px3210);                       \
	sd1 = vis_fpsub16(sd0, sd1);                            \
	Py10 = vis_fpadd16(Px3210, sd1);                        \
	sd2 = vis_write_lo(sd2, vis_read_hi(Py10));             \
	sd1 = vis_fmul8x16(cccc, sd2);                          \
	sd0 = vis_fmul8x16(cccc, Py10);                         \
	sd0 = vis_fpsub16(sd1, sd0);                            \
	P = vis_fpadd16(Py10, sd0)

/* *********************************************************** */

void
mlib_v_VolumeRayCast_General_Parallel_Trilinear_S16_S16_OneRegion_8X(
	mlib_rays *rays,
	mlib_s32 nrays,
	mlib_s32 minsteps,
	mlib_s32 maxsteps,
	const mlib_genvolume *vol)
{
	mlib_s32 i, j;
	mlib_s32 cray;
	mlib_s32 cstp;
	mlib_d64 Ox, Oy, Oz;
	mlib_d64 dOx, dOy, dOz;
	mlib_s32 Ox1616, Oy1616, Oz1616;
	mlib_s32 currX, currY, currZ;
	mlib_s32 cx[64], cy[64], cz[64];
	mlib_d64 Ox_frac4;
	mlib_d64 Oy_frac4;
	mlib_d64 Oz_frac4;
	mlib_d64 cx_frac[64];
	mlib_d64 cy_frac[64];
	mlib_d64 cz_frac[64];
	mlib_s32 Xint, Yint, Zint;
	mlib_d64 Po, Px, Py, Pz;
	mlib_d64 Pxy, Pxz, Pyz, Pxyz;
	mlib_d64 P;
	mlib_d64 P1o, P1x, P1y, P1z;
	mlib_d64 P1xy, P1xz, P1yz, P1xyz;
	mlib_d64 Px3210, Px0321, Py10;
	mlib_d64 sd0, sd1, sd2, sd3;
	mlib_f32 aaaa, bbbb, cccc;
	mlib_s16 *sp;
	mlib_s16 *dp;
	mlib_d64 *dp_d64;
	mlib_d64 mask12bits = vis_to_double_dup(0x0fff0fff);
	mlib_s32 ystride = vol->offsets[1];
	mlib_s16 **slices;
	mlib_s32 *offsets;

	slices = (mlib_s16 **)vol->slices;
	offsets = (mlib_s32 *)vol->offsets;

	Ox = rays->starts[0][0];
	Oy = rays->starts[0][1];
	Oz = (rays->starts[0][2] - vol->zcoords[0]) * vol->zsrates[0];

	dOx = rays->incs[0][0];
	dOy = rays->incs[0][1];
	dOz = rays->incs[0][2] * vol->zsrates[0];

#pragma pipeloop(0)
	for (cray = 0; cray < nrays; cray++) {
		cx[cray] =
			(mlib_s32)((rays->starts[cray][0] - Ox) * MLIB_PREC +
			0.5);
		cy[cray] =
			(mlib_s32)((rays->starts[cray][1] - Oy) * MLIB_PREC +
			0.5);
		cz[cray] =
			(mlib_s32)((rays->starts[cray][2] * vol->zsrates[0] -
			Oz) * MLIB_PREC + 0.5);
	}

	vis_write_gsr((3 << 3) + 6);

	PREPARE_VIS_FRACTIONS_FOR_FOUR_VOXELS_INTERPOLATION;

	for (cstp = 0; cstp < minsteps; cstp++) {

		dp_d64 = (mlib_d64 *)rays->results[cstp];

		PREPARE_THE_VOXEL_IN_HOME_RAY;
		i = 0;

		cray = 3;

		sd0 = vis_fpadd16(Ox_frac4, cx_frac[i]);
		sd1 = vis_fpadd16(Oy_frac4, cy_frac[i]);
		sd2 = vis_fpadd16(Oz_frac4, cz_frac[i++]);

		sd0 = vis_fand(sd0, mask12bits);
		sd1 = vis_fand(sd1, mask12bits);
		sd2 = vis_fand(sd2, mask12bits);

		aaaa = vis_fpack16(sd0);
		bbbb = vis_fpack16(sd1);
		cccc = vis_fpack16(sd2);

		LOAD_8_NEIGHBOR_VOXELS;
		cray--;

		LOAD_8_NEIGHBOR_VOXELS;
		cray--;

		LOAD_8_NEIGHBOR_VOXELS;
		cray--;

#pragma pipeloop(0)
		for (; i < (nrays >> 2); ) {

			LOAD_8_NEIGHBOR_VOXELS;
			cray += 7;

			Px = vis_fpsub16(vis_fmul8x16(aaaa, Px),
				vis_fmul8x16(aaaa, Po));
			Px = vis_fpadd16(Po, Px);

			Pxy = vis_fpsub16(vis_fmul8x16(aaaa, Pxy),
				vis_fmul8x16(aaaa, Py));
			Pxy = vis_fpadd16(Py, Pxy);

			Pxz = vis_fpsub16(vis_fmul8x16(aaaa, Pxz),
				vis_fmul8x16(aaaa, Pz));
			Pxz = vis_fpadd16(Pz, Pxz);

			Pxyz = vis_fpsub16(vis_fmul8x16(aaaa, Pxyz),
				vis_fmul8x16(aaaa, Pyz));
			Pxyz = vis_fpadd16(Pyz, Pxyz);

			sd0 = vis_fpadd16(Ox_frac4, cx_frac[i]);
			sd1 = vis_fpadd16(Oy_frac4, cy_frac[i]);
			sd2 = vis_fpadd16(Oz_frac4, cz_frac[i++]);

			sd0 = vis_fand(sd0, mask12bits);
			sd1 = vis_fand(sd1, mask12bits);
			sd2 = vis_fand(sd2, mask12bits);

			Pxy = vis_fpsub16(vis_fmul8x16(bbbb, Pxy),
				vis_fmul8x16(bbbb, Px));
			Pxy = vis_fpadd16(Px, Pxy);

			Pxyz = vis_fpsub16(vis_fmul8x16(bbbb, Pxyz),
				vis_fmul8x16(bbbb, Pxz));
			Pxyz = vis_fpadd16(Pxz, Pxyz);

			Pxyz = vis_fpsub16(vis_fmul8x16(cccc, Pxyz),
				vis_fmul8x16(cccc, Pxy));
			P = vis_fpadd16(Pxy, Pxyz);

			aaaa = vis_fpack16(sd0);
			bbbb = vis_fpack16(sd1);
			cccc = vis_fpack16(sd2);

			LOAD_8_NEIGHBOR_VOXELS;
			cray--;

			LOAD_8_NEIGHBOR_VOXELS;
			cray--;

			LOAD_8_NEIGHBOR_VOXELS;
			cray--;

			LOAD_8_NEIGHBOR_VOXELS;
			cray += 7;

			(*dp_d64++) = P;

			Px = vis_fpsub16(vis_fmul8x16(aaaa, Px),
				vis_fmul8x16(aaaa, Po));
			Px = vis_fpadd16(Po, Px);

			Pxy = vis_fpsub16(vis_fmul8x16(aaaa, Pxy),
				vis_fmul8x16(aaaa, Py));
			Pxy = vis_fpadd16(Py, Pxy);

			Pxz = vis_fpsub16(vis_fmul8x16(aaaa, Pxz),
				vis_fmul8x16(aaaa, Pz));
			Pxz = vis_fpadd16(Pz, Pxz);

			Pxyz = vis_fpsub16(vis_fmul8x16(aaaa, Pxyz),
				vis_fmul8x16(aaaa, Pyz));
			Pxyz = vis_fpadd16(Pyz, Pxyz);

			sd0 = vis_fpadd16(Ox_frac4, cx_frac[i]);
			sd1 = vis_fpadd16(Oy_frac4, cy_frac[i]);
			sd2 = vis_fpadd16(Oz_frac4, cz_frac[i++]);

			sd0 = vis_fand(sd0, mask12bits);
			sd1 = vis_fand(sd1, mask12bits);
			sd2 = vis_fand(sd2, mask12bits);

			Pxy = vis_fpsub16(vis_fmul8x16(bbbb, Pxy),
				vis_fmul8x16(bbbb, Px));
			Pxy = vis_fpadd16(Px, Pxy);

			Pxyz = vis_fpsub16(vis_fmul8x16(bbbb, Pxyz),
				vis_fmul8x16(bbbb, Pxz));
			Pxyz = vis_fpadd16(Pxz, Pxyz);

			Pxyz = vis_fpsub16(vis_fmul8x16(cccc, Pxyz),
				vis_fmul8x16(cccc, Pxy));
			P = vis_fpadd16(Pxy, Pxyz);

			if (nrays > cray) {
				aaaa = vis_fpack16(sd0);
				bbbb = vis_fpack16(sd1);
				cccc = vis_fpack16(sd2);

				LOAD_8_NEIGHBOR_VOXELS;
				cray--;

				LOAD_8_NEIGHBOR_VOXELS;
				cray--;

				LOAD_8_NEIGHBOR_VOXELS;
				cray--;
			}

			(*dp_d64++) = P;
		}

		Ox += dOx;
		Oy += dOy;
		Oz += dOz;
	}

	if (minsteps < maxsteps) {

		PREPARE_VIS_FRACTIONS_FOR_ONE_VOXEL_INTERPOLATION;

		for (cstp = minsteps; cstp < maxsteps; cstp++) {

			dp = (mlib_s16 *)rays->results[cstp];

			PREPARE_THE_VOXEL_IN_HOME_RAY;

#pragma pipeloop(0)
			for (cray = 0; cray < nrays; cray++) {

				if (cstp < rays->nsteps[cray]) {

					VIS_TRILINEAR_1_VOXEL_INTER_1REG_S16;

					vis_st_u16(P, dp++);
				} else {
					dp++;
				}
			}

			Ox += dOx;
			Oy += dOy;
			Oz += dOz;
		}
	}
}

/* *********************************************************** */

void
mlib_v_VolumeRayCast_General_Parallel_Trilinear_S16_S16_OneRegion_4X(
	mlib_rays *rays,
	mlib_s32 nrays,
	mlib_s32 minsteps,
	mlib_s32 maxsteps,
	const mlib_genvolume *vol)
{
	mlib_s32 i, j;
	mlib_s32 cray;
	mlib_s32 cstp;
	mlib_d64 Ox, Oy, Oz;
	mlib_d64 dOx, dOy, dOz;
	mlib_s32 Ox1616, Oy1616, Oz1616;
	mlib_s32 currX, currY, currZ;
	mlib_s32 cx[64], cy[64], cz[64];
	mlib_d64 Ox_frac4;
	mlib_d64 Oy_frac4;
	mlib_d64 Oz_frac4;
	mlib_d64 cx_frac[64];
	mlib_d64 cy_frac[64];
	mlib_d64 cz_frac[64];
	mlib_s32 Xint, Yint, Zint;
	mlib_d64 Po, Px, Py, Pz;
	mlib_d64 Pxy, Pxz, Pyz, Pxyz;
	mlib_d64 P;
	mlib_d64 P1o, P1x, P1y, P1z;
	mlib_d64 P1xy, P1xz, P1yz, P1xyz;
	mlib_d64 Px3210, Px0321, Py10;
	mlib_d64 sd0, sd1, sd2, sd3;
	mlib_f32 aaaa, bbbb, cccc;
	mlib_s16 *sp;
	mlib_s16 *dp;
	mlib_d64 *dp_d64;
	mlib_d64 mask12bits = vis_to_double_dup(0x0fff0fff);
	mlib_s32 ystride = vol->offsets[1];
	mlib_s16 **slices;
	mlib_s32 *offsets;

	slices = (mlib_s16 **)vol->slices;
	offsets = (mlib_s32 *)vol->offsets;

	Ox = rays->starts[0][0];
	Oy = rays->starts[0][1];
	Oz = rays->starts[0][2];

	dOx = rays->incs[0][0];
	dOy = rays->incs[0][1];
	dOz = rays->incs[0][2];

	for (cray = 0; cray < nrays; cray++) {
		cx[cray] =
			(mlib_s32)((rays->starts[cray][0] - Ox) * MLIB_PREC +
			0.5);
		cy[cray] =
			(mlib_s32)((rays->starts[cray][1] - Oy) * MLIB_PREC +
			0.5);
		cz[cray] =
			(mlib_s32)((rays->starts[cray][2] * vol->zsrates[0] -
			Oz) * MLIB_PREC + 0.5);
	}

	vis_write_gsr((3 << 3) + 6);

	PREPARE_VIS_FRACTIONS_FOR_FOUR_VOXELS_INTERPOLATION;

	for (cstp = 0; cstp < minsteps; cstp++) {

		dp_d64 = (mlib_d64 *)rays->results[cstp];

		PREPARE_THE_VOXEL_IN_HOME_RAY;
#pragma pipeloop(0)
		for (i = 0, cray = 3; i < (nrays >> 2); i++) {

			sd0 = vis_fpadd16(Ox_frac4, cx_frac[i]);
			sd1 = vis_fpadd16(Oy_frac4, cy_frac[i]);
			sd2 = vis_fpadd16(Oz_frac4, cz_frac[i]);

			sd0 = vis_fand(sd0, mask12bits);
			sd1 = vis_fand(sd1, mask12bits);
			sd2 = vis_fand(sd2, mask12bits);

			aaaa = vis_fpack16(sd0);
			bbbb = vis_fpack16(sd1);
			cccc = vis_fpack16(sd2);

			LOAD_8_NEIGHBOR_VOXELS;
			cray--;

			LOAD_8_NEIGHBOR_VOXELS;
			cray--;

			LOAD_8_NEIGHBOR_VOXELS;
			cray--;

			LOAD_8_NEIGHBOR_VOXELS;
			cray += 7;

			Px = vis_fpsub16(vis_fmul8x16(aaaa, Px),
				vis_fmul8x16(aaaa, Po));
			Px = vis_fpadd16(Po, Px);

			Pxy = vis_fpsub16(vis_fmul8x16(aaaa, Pxy),
				vis_fmul8x16(aaaa, Py));
			Pxy = vis_fpadd16(Py, Pxy);

			Pxz = vis_fpsub16(vis_fmul8x16(aaaa, Pxz),
				vis_fmul8x16(aaaa, Pz));
			Pxz = vis_fpadd16(Pz, Pxz);

			Pxyz = vis_fpsub16(vis_fmul8x16(aaaa, Pxyz),
				vis_fmul8x16(aaaa, Pyz));
			Pxyz = vis_fpadd16(Pyz, Pxyz);

			Pxy = vis_fpsub16(vis_fmul8x16(bbbb, Pxy),
				vis_fmul8x16(bbbb, Px));
			Pxy = vis_fpadd16(Px, Pxy);

			Pxyz = vis_fpsub16(vis_fmul8x16(bbbb, Pxyz),
				vis_fmul8x16(bbbb, Pxz));
			Pxyz = vis_fpadd16(Pxz, Pxyz);

			Pxyz = vis_fpsub16(vis_fmul8x16(cccc, Pxyz),
				vis_fmul8x16(cccc, Pxy));
			(*dp_d64++) = vis_fpadd16(Pxy, Pxyz);
		}

		Ox += dOx;
		Oy += dOy;
		Oz += dOz;
	}

	if (minsteps < maxsteps) {

		PREPARE_VIS_FRACTIONS_FOR_ONE_VOXEL_INTERPOLATION;

		for (cstp = minsteps; cstp < maxsteps; cstp++) {

			dp = (mlib_s16 *)rays->results[cstp];

			PREPARE_THE_VOXEL_IN_HOME_RAY;

#pragma pipeloop(0)
			for (cray = 0; cray < nrays; cray++) {

				if (cstp < rays->nsteps[cray]) {

					VIS_TRILINEAR_1_VOXEL_INTER_1REG_S16;

					vis_st_u16(P, dp++);
				} else {
					dp++;
				}
			}

			Ox += dOx;
			Oy += dOy;
			Oz += dOz;
		}
	}
}

/* *********************************************************** */

void
mlib_v_VolumeRayCast_General_Parallel_Trilinear_S16_S16_OneRegion(
	mlib_rays *rays,
	mlib_s32 nrays,
	mlib_s32 minsteps,
	mlib_s32 maxsteps,
	const mlib_genvolume *vol)
{
	mlib_s32 cray;
	mlib_s32 cstp;
	mlib_d64 Ox, Oy, Oz;
	mlib_d64 dOx, dOy, dOz;
	mlib_s32 Ox1616, Oy1616, Oz1616;
	mlib_s32 currX, currY, currZ;
	mlib_s32 cx[64], cy[64], cz[64];
	mlib_d64 Ox_frac4;
	mlib_d64 Oy_frac4;
	mlib_d64 Oz_frac4;
	mlib_d64 cx_frac[64];
	mlib_d64 cy_frac[64];
	mlib_d64 cz_frac[64];
	mlib_s32 Xint, Yint, Zint;
	mlib_d64 Po, Px, Py, Pz;
	mlib_d64 Pxy, Pxz, Pyz, Pxyz;
	mlib_d64 sd0, sd1, sd2, sd3;
	mlib_d64 Px3210, Px0321, Py10;
	mlib_d64 P;
	mlib_f32 aaaa, bbbb, cccc;
	mlib_s16 *sp;
	mlib_s16 *dp;
	mlib_d64 mask12bits = vis_to_double_dup(0x0fff0fff);

	Ox = rays->starts[0][0];
	Oy = rays->starts[0][1];
	Oz = (rays->starts[0][2] - vol->zcoords[0]) * vol->zsrates[0];

	dOx = rays->incs[0][0];
	dOy = rays->incs[0][1];
	dOz = rays->incs[0][2] * vol->zsrates[0];

	for (cray = 0; cray < nrays; cray++) {
		cx[cray] =
			(mlib_s32)((rays->starts[cray][0] - Ox) * MLIB_PREC +
			0.5);
		cy[cray] =
			(mlib_s32)((rays->starts[cray][1] - Oy) * MLIB_PREC +
			0.5);
		cz[cray] =
			(mlib_s32)((rays->starts[cray][2] * vol->zsrates[0] -
			Oz) * MLIB_PREC + 0.5);
	}

	vis_write_gsr((3 << 3) + 6);

	PREPARE_VIS_FRACTIONS_FOR_ONE_VOXEL_INTERPOLATION;

	for (cstp = 0; cstp < minsteps; cstp++) {

		dp = (mlib_s16 *)rays->results[cstp];

		PREPARE_THE_VOXEL_IN_HOME_RAY;

#pragma pipeloop(0)
		for (cray = 0; cray < nrays; cray++) {

			VIS_TRILINEAR_1_VOXEL_INTER_1REG_S16;
			vis_st_u16(P, dp++);
		}

		Ox += dOx;
		Oy += dOy;
		Oz += dOz;
	}

	for (cstp = minsteps; cstp < maxsteps; cstp++) {

		dp = (mlib_s16 *)rays->results[cstp];

		PREPARE_THE_VOXEL_IN_HOME_RAY;

#pragma pipeloop(0)
		for (cray = 0; cray < nrays; cray++) {
			if (cstp < rays->nsteps[cray]) {

				VIS_TRILINEAR_1_VOXEL_INTER_1REG_S16;

				vis_st_u16(P, dp++);
			} else {
				dp++;
			}
		}

		Ox += dOx;
		Oy += dOy;
		Oz += dOz;
	}
}

/* *********************************************************** */

#define	VIS_TRILINEAR_ONE_VOXEL_INTER_MULTIREGION_S16           \
	sd0 = vis_fpadd16(Ox_frac4, cx_frac[cray]);             \
	sd1 = vis_fand(sd0, mask12bits);                        \
	aaaa = vis_fpack16(sd1);                                \
	sd0 = vis_fpadd16(Oy_frac2, cy_frac[cray]);             \
	sd1 = vis_fand(sd0, mask12bits);                        \
	bbbb = vis_fpack16(sd1);                                \
	cccc = vis_read_lo(vis_fpack32(sd0, ssZ_frac[cray]));   \
	Xint = currX >> 16;                                     \
	Yint = currY >> 16;                                     \
	Zint = ssZ[cray] >> 16;                                 \
	sp = (mlib_s16 *)((mlib_u8 *)vol->slices[Zint] +        \
		vol->offsets[Yint]) + Xint;                     \
	Po = vis_ld_u16(sp++);                                  \
	Px = vis_ld_u16(sp);                                    \
	sp = (mlib_s16 *)((mlib_u8 *)vol->slices[Zint] +        \
		vol->offsets[Yint + 1]) + Xint;                 \
	Py = vis_ld_u16(sp++);                                  \
	Pxy = vis_ld_u16(sp);                                   \
	sp = (mlib_s16 *)((mlib_u8 *)vol->slices[Zint + 1] +    \
		vol->offsets[Yint]) + Xint;                     \
	Pz = vis_ld_u16(sp++);                                  \
	Pxz = vis_ld_u16(sp);                                   \
	sp = (mlib_s16 *)((mlib_u8 *)vol->slices[Zint + 1] +    \
		vol->offsets[Yint + 1]) + Xint;                 \
	Pyz = vis_ld_u16(sp++);                                 \
	Pxyz = vis_ld_u16(sp);                                  \
	sd0 = vis_faligndata(Po, sd0);                          \
	sd0 = vis_faligndata(Py, sd0);                          \
	sd0 = vis_faligndata(Pz, sd0);                          \
	sd0 = vis_faligndata(Pyz, sd0);                         \
	sd1 = vis_faligndata(Px, sd1);                          \
	sd1 = vis_faligndata(Pxy, sd1);                         \
	sd1 = vis_faligndata(Pxz, sd1);                         \
	sd1 = vis_faligndata(Pxyz, sd1);                        \
	sd2 = vis_fmul8x16(aaaa, sd1);                          \
	sd3 = vis_fmul8x16(aaaa, sd0);                          \
	sd3 = vis_fpsub16(sd2, sd3);                            \
	Px3210 = vis_fpadd16(sd0, sd3);                         \
	Px0321 = vis_faligndata(Px3210, Px3210);                \
	sd0 = vis_fmul8x16(bbbb, Px0321);                       \
	sd1 = vis_fmul8x16(bbbb, Px3210);                       \
	sd1 = vis_fpsub16(sd0, sd1);                            \
	Py10 = vis_fpadd16(Px3210, sd1);                        \
	sd2 = vis_write_lo(sd2, vis_read_hi(Py10));             \
	sd1 = vis_fmul8x16(cccc, sd2);                          \
	sd0 = vis_fmul8x16(cccc, Py10);                         \
	sd0 = vis_fpsub16(sd1, sd0);                            \
	P = vis_fpadd16(Py10, sd0)

/* *********************************************************** */

void
mlib_v_VolumeRayCast_General_Parallel_Trilinear_S16_S16_MultiRegion(
	mlib_rays *rays,
	mlib_s32 nrays,
	mlib_s32 minsteps,
	mlib_s32 maxsteps,
	const mlib_genvolume *vol)
{
	mlib_s32 cray;
	mlib_s32 cstp;
	mlib_s32 creg;
	mlib_s32 ssZ[64], ssZinc[64];
	mlib_s32 reg[64];
	mlib_d64 Ox, Oy, Oz;
	mlib_s32 Ox1616, Oy1616;
	mlib_s32 currX, currY;
	mlib_d64 currZ;
	mlib_d64 dOx, dOy, dOz;
	mlib_s32 cx[64], cy[64];
	mlib_d64 cz[64];
	mlib_d64 cx_frac[64];
	mlib_d64 cy_frac[64];
	mlib_d64 Ox_frac4, Oy_frac2;
	mlib_d64 ssZinc_frac[64];
	mlib_d64 ssZ_frac[64];
	mlib_s32 Xint, Yint, Zint;
	mlib_d64 Po, Px, Py, Pz;
	mlib_d64 Pxy, Pxz, Pyz, Pxyz;
	mlib_d64 sd0, sd1, sd2, sd3;
	mlib_d64 Px3210, Px0321, Py10;
	mlib_d64 P;
	mlib_f32 aaaa, bbbb, cccc;
	mlib_s16 *sp;
	mlib_s16 *dp;
	mlib_d64 mask28bits = vis_to_double_dup(0x0fffffff);
	mlib_d64 mask12bits = vis_to_double_dup(0x0fff0fff);

	Ox = rays->starts[0][0];
	Oy = rays->starts[0][1];
	Oz = rays->starts[0][2];

	dOx = rays->incs[0][0];
	dOy = rays->incs[0][1];
	dOz = rays->incs[0][2];

	for (cray = 0; cray < nrays; cray++) {
		cx[cray] =
			(mlib_s32)((rays->starts[cray][0] - Ox) * MLIB_PREC +
			0.5);
		cy[cray] =
			(mlib_s32)((rays->starts[cray][1] - Oy) * MLIB_PREC +
			0.5);
		cz[cray] = rays->starts[cray][2] - Oz;

		creg = 0;
		while (rays->starts[cray][2] > vol->zcoords[creg + 1])
			creg++;
		reg[cray] = creg;

		ssZ[cray] =
			(mlib_s32)(((rays->starts[cray][2] -
			vol->zcoords[creg]) * vol->zsrates[creg] +
			vol->zindices[creg]) * MLIB_PREC + 0.5);
		ssZinc[cray] =
			(mlib_s32)(vol->zsrates[creg] * dOz * MLIB_PREC + 0.5);

		cx_frac[cray] =
			vis_to_double_dup(((cx[cray] & 0xfff0) << 12) |
			((cx[cray] & 0xfff0) >> 4));
		cy_frac[cray] = vis_to_double_dup((cy[cray] & 0xfff0) >> 4);

		ssZ_frac[cray] = vis_to_double_dup((ssZ[cray] & 0xffff) << 12);
		ssZinc_frac[cray] =
			vis_to_double_dup((ssZinc[cray] & 0xffff) << 12);
	}

	vis_write_gsr((3 << 3) + 6);

	for (cstp = 0; cstp < minsteps; cstp++) {

		dp = (mlib_s16 *)rays->results[cstp];

		Ox1616 = (mlib_s32)(Ox * MLIB_PREC + 0.5);
		Oy1616 = (mlib_s32)(Oy * MLIB_PREC + 0.5);

		Ox_frac4 =
			vis_to_double_dup(((Ox1616 & 0xfff0) << 12) | ((Ox1616 &
			0xfff0) >> 4));
		Oy_frac2 = vis_to_double_dup((Oy1616 & 0xfff0) >> 4);

#pragma pipeloop(0)
		for (cray = 0; cray < nrays; cray++) {

			currX = Ox1616 + cx[cray];
			currY = Oy1616 + cy[cray];
			currZ = Oz + cz[cray];

			creg = reg[cray];

			if (dOz > 0.0)
				while (currZ > vol->zcoords[creg + 1])
					creg++;
			else if (dOz < 0.0)
				while (currZ < vol->zcoords[creg])
					creg--;

			if (creg != reg[cray]) {
				reg[cray] = creg;

				ssZ[cray] =
					(mlib_s32)(((currZ -
					vol->zcoords[creg]) *
					vol->zsrates[creg] +
					vol->zindices[creg]) * MLIB_PREC + 0.5);
				ssZinc[cray] =
					(mlib_s32)(vol->zsrates[creg] * dOz *
					MLIB_PREC + 0.5);

				ssZ_frac[cray] =
					vis_to_double_dup((ssZ[cray] & 0xffff)
					<< 12);
				ssZinc_frac[cray] =
					vis_to_double_dup((ssZinc[cray] &
					0xffff) << 12);
			}

			VIS_TRILINEAR_ONE_VOXEL_INTER_MULTIREGION_S16;

			vis_st_u16(P, dp++);

			ssZ[cray] += ssZinc[cray];
			ssZ_frac[cray] =
				vis_fpadd32(ssZ_frac[cray], ssZinc_frac[cray]);
			ssZ_frac[cray] = vis_fand(ssZ_frac[cray], mask28bits);
		}

		Ox += dOx;
		Oy += dOy;
		Oz += dOz;
	}

	for (cstp = minsteps; cstp < maxsteps; cstp++) {

		dp = (mlib_s16 *)rays->results[cstp];

		Ox1616 = (mlib_s32)(Ox * MLIB_PREC + 0.5);
		Oy1616 = (mlib_s32)(Oy * MLIB_PREC + 0.5);

		Ox_frac4 =
			vis_to_double_dup(((Ox1616 & 0xfff0) << 12) | ((Ox1616 &
			0xfff0) >> 4));
		Oy_frac2 = vis_to_double_dup((Oy1616 & 0xfff0) >> 4);

#pragma pipeloop(0)
		for (cray = 0; cray < nrays; cray++) {
			if (cstp < rays->nsteps[cray]) {

				currX = Ox1616 + cx[cray];
				currY = Oy1616 + cy[cray];
				currZ = Oz + cz[cray];

				creg = reg[cray];

				if (dOz > 0.0)
					while (currZ > vol->zcoords[creg + 1])
						creg++;
				else if (dOz < 0.0)
					while (currZ < vol->zcoords[creg])
						creg--;

				if (creg != reg[cray]) {
					reg[cray] = creg;

					ssZ[cray] =
						(mlib_s32)(((currZ -
						vol->zcoords[creg]) *
						vol->zsrates[creg]
						+
						vol->zindices[creg]) *
						MLIB_PREC + 0.5);
					ssZinc[cray] =
						(mlib_s32)(vol->zsrates[creg] *
						dOz * MLIB_PREC + 0.5);

					ssZ_frac[cray] =
						vis_to_double_dup((ssZ[cray] &
						0xffff) << 12);
					ssZinc_frac[cray] =
						vis_to_double_dup((ssZinc[cray]
						& 0xffff) << 12);
				}

				VIS_TRILINEAR_ONE_VOXEL_INTER_MULTIREGION_S16;

				vis_st_u16(P, dp++);

				ssZ[cray] += ssZinc[cray];
				ssZ_frac[cray] =
					vis_fpadd32(ssZ_frac[cray],
					ssZinc_frac[cray]);
				ssZ_frac[cray] =
					vis_fand(ssZ_frac[cray], mask28bits);
			} else {
				dp++;
			}
		}

		Ox += dOx;
		Oy += dOy;
		Oz += dOz;
	}
}

/* *********************************************************** */
