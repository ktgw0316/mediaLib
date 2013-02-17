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

#pragma ident	"@(#)mlib_c_VolumeRayCast_BPT.c	9.2	07/10/09 SMI"

/*
 *  FUNCTION
 *    mlib_VolumeRayCast_Blocked_Parallel_Trilinear_[U8_U8|S16_S16]
 *      Cast parallel rays through a three dimensional data set
 *      and return the interpolated samples.
 *
 *  SYNOPSIS
 *    mlib_status mlib_VolumeRayCast_Blocked_Parallel_Trilinear_[U8_U8|S16_S16]
 *                                           (const mlib_rays      *rays,
 *                                            const mlib_blkvolume *blk,
 *                                            void                 *buffer)
 *
 *  ARGUMENTS
 *    rays    Casting rays.
 *    blk     Volume data in blocked data format.
 *    buffer  Working buffer.
 *
 *  DESCRIPTION
 *    Casting rays through a three-dimensional data set, compute and
 *    return the interpolated samples at each step along the rays.
 *    Trilinear interpolation is used.
 *
 */

#include <stdlib.h>
#include <mlib_volume.h>
#include <mlib_SysMath.h>
#include <mlib_c_VolumeRayCast_Blocked.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_VolumeRayCast_Blocked_Parallel_Trilinear_U8_U8 = \
__mlib_VolumeRayCast_Blocked_Parallel_Trilinear_U8_U8
#pragma weak mlib_VolumeRayCast_Blocked_Parallel_Trilinear_S16_S16 = \
	__mlib_VolumeRayCast_Blocked_Parallel_Trilinear_S16_S16
#elif defined(__GNUC__)

__typeof__(__mlib_VolumeRayCast_Blocked_Parallel_Trilinear_U8_U8)
	mlib_VolumeRayCast_Blocked_Parallel_Trilinear_U8_U8
	__attribute__((weak,
	alias("__mlib_VolumeRayCast_Blocked_Parallel_Trilinear_U8_U8")));
__typeof__(__mlib_VolumeRayCast_Blocked_Parallel_Trilinear_S16_S16)
	mlib_VolumeRayCast_Blocked_Parallel_Trilinear_S16_S16
	__attribute__((weak,
	alias("__mlib_VolumeRayCast_Blocked_Parallel_Trilinear_S16_S16")));

#else

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */
#define	MAX_RAY_NUM	64
#define	MLIB_SHIFT	20
#define	MLIB_SHIFT_HALF	(MLIB_SHIFT >> 1)
#define	MLIB_PREC	(1 << MLIB_SHIFT)
#define	MLIB_MASK	(MLIB_PREC - 1)

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
		(a) * (b) * (c) * (mlib_d64)Pxyz);

/* *********************************************************** */
#define	TRILINEAR_INT(a, b, c, Po, Px, Py, Pz, Pxy, Pxz, Pyz,     \
	Pxyz, rsl)                                                \
	{                                                         \
	    mlib_s32 Px0, Px1, Px2, Px3, Py0, Py1;                \
	                                                          \
	    b >>= MLIB_SHIFT_HALF;                                \
	    c >>= MLIB_SHIFT_HALF;                                \
	    Px0 = (Po << MLIB_SHIFT) + a * (Px - Po);             \
	    Px1 = (Py << MLIB_SHIFT) + a * (Pxy - Py);            \
	    Px2 = (Pz << MLIB_SHIFT) + a * (Pxz - Pz);            \
	    Px3 = (Pyz << MLIB_SHIFT) + a * (Pxyz - Pyz);         \
	    Py0 = Px0 + (b) * ((Px1 - Px0) >> MLIB_SHIFT_HALF);   \
	    Py1 = Px2 + (b) * ((Px3 - Px2) >> MLIB_SHIFT_HALF);   \
	    rsl = (mlib_u8)((Py0 + c * ((Py1 -                    \
		    Py0) >> MLIB_SHIFT_HALF)) >> MLIB_SHIFT);     \
	}

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
__mlib_VolumeRayCast_Blocked_Parallel_Trilinear_U8_U8(
	mlib_rays *rays,
	const mlib_blkvolume *blk,
	void *buffer)
{
	mlib_s32 xysize;
	mlib_s32 nrays;
	mlib_s32 dOx, dOy, dOz;
	mlib_u8 *voxels, *dp;
	mlib_s32 nsteps, cray, minsteps, maxsteps, cstp;
	mlib_u32 n, m;
	mlib_s32 Ox, Oy, Oz;
	mlib_s32 cx[MAX_RAY_NUM], cy[MAX_RAY_NUM], cz[MAX_RAY_NUM];
	mlib_s32 X, Y, Z;
	mlib_s32 a, b, c, i;
	mlib_u8 Po, Px, Py, Pz, Pxy, Pxz, Pyz, Pxyz;
	mlib_d64 go_to_rays = 0., go_to_steps = 0., testx, testy, testz;
	mlib_s32 xsrc, ysrc, zsrc;

	if (rays == NULL || blk == NULL)
		return (MLIB_NULLPOINTER);

	xysize = blk->xysize, nrays = rays->nrays;

	dOx = (mlib_s32)(rays->incs[0][0] * MLIB_PREC);
	dOy = (mlib_s32)(rays->incs[0][1] * MLIB_PREC);
	dOz = (mlib_s32)(rays->incs[0][2] * MLIB_PREC);
	voxels = (mlib_u8 *)blk->voxels;

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

		Ox = (rays->starts[0][0]) * MLIB_PREC;
		Oy = (rays->starts[0][1]) * MLIB_PREC;
		Oz = (rays->starts[0][2]) * MLIB_PREC;

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
			cx[cray] = (rays->starts[cray][0]) * MLIB_PREC - Ox;
			testx += (mlib_d64)(abs(cx[cray] - cx[cray -
				1])) / MLIB_PREC;
			cy[cray] = (rays->starts[cray][1]) * MLIB_PREC - Oy;
			testy += (mlib_d64)(abs(cy[cray] - cy[cray -
				1])) / MLIB_PREC;
			cz[cray] = (rays->starts[cray][2]) * MLIB_PREC - Oz;
			testz += (mlib_d64)(abs(cz[cray] - cz[cray -
				1])) / MLIB_PREC;
		}

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

			for (cray = 0; cray < nrays; cray++) {
				X = Ox + cx[cray];
				Y = Oy + cy[cray];
				Z = Oz + cz[cray];

				a = X & MLIB_MASK;
				b = Y & MLIB_MASK;
				c = Z & MLIB_MASK;

				xsrc = (X >> MLIB_SHIFT);
				ysrc = (Y >> MLIB_SHIFT);
				zsrc = (Z >> MLIB_SHIFT);

				LOAD_8(xsrc, ysrc, zsrc);
				TRILINEAR_INT(a, b, c, Po, Px, Py, Pz, Pxy, Pxz,
					Pyz, Pxyz, dp[0]);

				dp++;
			}

			Ox += dOx;
			Oy += dOy;
			Oz += dOz;
		}

		for (cstp = minsteps; cstp < maxsteps; cstp++) {
			dp = (mlib_u8 *)rays->results[cstp];

			for (cray = 0; cray < nrays; cray++) {

				if (cstp < rays->nsteps[cray]) {
					X = Ox + cx[cray];
					Y = Oy + cy[cray];
					Z = Oz + cz[cray];

					a = X & MLIB_MASK;
					b = Y & MLIB_MASK;
					c = Z & MLIB_MASK;

					xsrc = (X >> MLIB_SHIFT);
					ysrc = (Y >> MLIB_SHIFT);
					zsrc = (Z >> MLIB_SHIFT);

					LOAD_8(xsrc, ysrc, zsrc);
					TRILINEAR_INT(a, b, c, Po, Px, Py, Pz,
						Pxy, Pxz, Pyz, Pxyz, dp[0]);
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

		for (cray = 0; cray < nrays; cray++) {
			nsteps = rays->nsteps[cray];

			if (nsteps > 0) {
				X = (rays->starts[cray][0]) * MLIB_PREC;
				Y = (rays->starts[cray][1]) * MLIB_PREC;
				Z = (rays->starts[cray][2]) * MLIB_PREC;

				a = X & MLIB_MASK;
				b = Y & MLIB_MASK;
				c = Z & MLIB_MASK;

				xsrc = (X >> MLIB_SHIFT);
				ysrc = (Y >> MLIB_SHIFT);
				zsrc = (Z >> MLIB_SHIFT);

				LOAD_8(xsrc, ysrc, zsrc);

				X += dOx;
				Y += dOy;
				Z += dOz;

				for (cstp = 0; cstp < nsteps - 1; cstp++) {
					TRILINEAR_INT(a, b, c, Po, Px, Py, Pz,
						Pxy, Pxz, Pyz, Pxyz,
						((mlib_u8 *)rays->
						results[cstp])[cray]);
					a = X & MLIB_MASK;
					b = Y & MLIB_MASK;
					c = Z & MLIB_MASK;

					xsrc = (X >> MLIB_SHIFT);
					ysrc = (Y >> MLIB_SHIFT);
					zsrc = (Z >> MLIB_SHIFT);

					LOAD_8(xsrc, ysrc, zsrc)

						X += dOx;
					Y += dOy;
					Z += dOz;
				}
				TRILINEAR_INT(a, b, c, Po, Px, Py, Pz, Pxy, Pxz,
					Pyz, Pxyz,
					((mlib_u8 *)rays->results[cstp])[cray]);
			}
		}
	}
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VolumeRayCast_Blocked_Parallel_Trilinear_S16_S16(
	mlib_rays *rays,
	const mlib_blkvolume *blk,
	void *buffer)
{
	mlib_s32 xysize;
	mlib_s32 nrays, i;
	mlib_d64 dOx, dOy, dOz;
	mlib_s16 *voxels, *dp;
	mlib_s32 nsteps, cray, minsteps, maxsteps, cstp;
	mlib_u32 n, m;
	mlib_d64 Ox, Oy, Oz;
	mlib_d64 cx[MAX_RAY_NUM], cy[MAX_RAY_NUM], cz[MAX_RAY_NUM];
	mlib_d64 X, Y, Z;
	mlib_d64 a, b, c;
	mlib_s16 Po, Px, Py, Pz, Pxy, Pxz, Pyz, Pxyz;
	mlib_d64 go_to_rays = 0., go_to_steps = 0., testx, testy, testz;
	mlib_s32 xsrc, ysrc, zsrc;

	if (rays == NULL || blk == NULL)
		return (MLIB_NULLPOINTER);
	xysize = blk->xysize, nrays = rays->nrays;

	dOx = rays->incs[0][0];
	dOy = rays->incs[0][1];
	dOz = rays->incs[0][2];
	voxels = (mlib_s16 *)blk->voxels;

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
			dp = (mlib_s16 *)rays->results[cstp];

			for (cray = 0; cray < nrays; cray++) {
				X = Ox + cx[cray];
				Y = Oy + cy[cray];
				Z = Oz + cz[cray];

				xsrc = (mlib_s32)X;
				ysrc = (mlib_s32)Y;
				zsrc = (mlib_s32)Z;

				a = X - xsrc;
				b = Y - ysrc;
				c = Z - zsrc;

				LOAD_8(xsrc, ysrc, zsrc);
				TRILINEAR(a, b, c, Po, Px, Py, Pz, Pxy, Pxz,
					Pyz, Pxyz, dp[0]);

				dp++;
			}

			Ox += dOx;
			Oy += dOy;
			Oz += dOz;
		}

		for (cstp = minsteps; cstp < maxsteps; cstp++) {
			dp = (mlib_s16 *)rays->results[cstp];

			for (cray = 0; cray < nrays; cray++) {

				if (cstp < rays->nsteps[cray]) {
					X = Ox + cx[cray];
					Y = Oy + cy[cray];
					Z = Oz + cz[cray];

					xsrc = (mlib_s32)X;
					ysrc = (mlib_s32)Y;
					zsrc = (mlib_s32)Z;

					a = X - xsrc;
					b = Y - ysrc;
					c = Z - zsrc;

					LOAD_8(xsrc, ysrc, zsrc);
					TRILINEAR(a, b, c, Po, Px, Py, Pz, Pxy,
						Pxz, Pyz, Pxyz, dp[0]);
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

		for (cray = 0; cray < nrays; cray++) {
			nsteps = rays->nsteps[cray];

			if (nsteps > 0) {
				X = rays->starts[cray][0];
				Y = rays->starts[cray][1];
				Z = rays->starts[cray][2];

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
					TRILINEAR(a, b, c, Po, Px, Py, Pz, Pxy,
						Pxz, Pyz, Pxyz,
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

					LOAD_8(xsrc, ysrc, zsrc)
				}
				TRILINEAR(a, b, c, Po, Px, Py, Pz, Pxy, Pxz,
					Pyz, Pxyz,
					((mlib_s16 *)rays->
					results[cstp])[cray]);
			}
		}
	}
	return (MLIB_SUCCESS);
}

/* *********************************************************** */
