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

#pragma ident	"@(#)mlib_VolumeRayCast_GDT.c	9.2	07/10/09 SMI"

/*
 *  FUNCTION
 *    mlib_VolumeRayCast_General_Divergent_Trilinear_[U8_U8|S16_S16]
 *      Cast divergent rays through a three dimensional data set
 *      and return the interpolated samples.
 *
 *  SYNOPSIS
 *    mlib_status mlib_VolumeRayCast_General_Divergent_Trilinear_[U8_U8|S16_S16]
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
 *      Casting rays through a three-dimensional data set, compute and
 *      return the interpolated samples at each step along the rays.
 *      Trilinear interpolation is used.
 */

#include <stdlib.h>
#include <mlib_volume.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_VolumeRayCast_General_Divergent_Trilinear_S16_S16 = \
__mlib_VolumeRayCast_General_Divergent_Trilinear_S16_S16
#pragma weak mlib_VolumeRayCast_General_Divergent_Trilinear_U8_U8 = \
	__mlib_VolumeRayCast_General_Divergent_Trilinear_U8_U8
#elif defined(__GNUC__)

__typeof__(__mlib_VolumeRayCast_General_Divergent_Trilinear_S16_S16)
	mlib_VolumeRayCast_General_Divergent_Trilinear_S16_S16
	__attribute__((weak,
	alias("__mlib_VolumeRayCast_General_Divergent_Trilinear_S16_S16")));
__typeof__(__mlib_VolumeRayCast_General_Divergent_Trilinear_U8_U8)
	mlib_VolumeRayCast_General_Divergent_Trilinear_U8_U8
	__attribute__((weak,
	alias("__mlib_VolumeRayCast_General_Divergent_Trilinear_U8_U8")));

#else

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	MAX_RAY_NUM	64
#define	STEPS_LIMIT	512
#define	REGION_LIMIT	512
#define	SLICE_LIMIT	512
#define	MLIB_SHIFT	20
#define	MLIB_SHIFT_HALF	(MLIB_SHIFT >> 1)
#define	MLIB_MASK	(MLIB_PREC - 1)
#define	MLIB_PREC	(1 << MLIB_SHIFT)
#define	NSTEP_SHIFT	6
#define	NRAY_MASK	((1 << NSTEP_SHIFT) - 1)

/* *********************************************************** */

#define	ALLOC_REGION_MEM                                        \
	if ((nregions + 1) > REGION_LIMIT) {                    \
	    zcoords =                                           \
		    (mlib_s32 *)__mlib_malloc((nregions +       \
		    1) * sizeof (mlib_s32));                    \
	    if (zcoords == NULL)                                \
		return (MLIB_FAILURE);                          \
	} else                                                  \
	    zcoords = zcoordArr

/* *********************************************************** */

#define	FREE_REGION_MEM                                         \
	if ((nregions + 1) > REGION_LIMIT)                      \
	    __mlib_free(zcoords)

/* *********************************************************** */

#define	ALLOC_SLICE_MEM                                         \
	if (nslices > SLICE_LIMIT) {                            \
	    sliceLst =                                          \
		    (mlib_u32 *)__mlib_malloc(nslices *         \
		    sizeof (mlib_u32));                         \
	    if (sliceLst == NULL) {                             \
		FREE_REGION_MEM;                                \
		return (MLIB_FAILURE);                          \
	    }                                                   \
	} else                                                  \
	    sliceLst = lstArray

/* *********************************************************** */

#define	FREE_SLICE_MEM                                          \
	if (nslices > SLICE_LIMIT)                              \
	    __mlib_free(sliceLst)

/* *********************************************************** */

#define	ALLOC_STEPS_MEM                                                   \
	if (maxsteps > STEPS_LIMIT) {                                     \
	    bufptr = (bufData *) __mlib_malloc(MAX_RAY_NUM * maxsteps *   \
		    BUFSIZE);                                             \
	    if (bufptr == NULL) {                                         \
		FREE_REGION_MEM;                                          \
		FREE_SLICE_MEM;                                           \
		return (MLIB_FAILURE);                                    \
	    }                                                             \
	} else                                                            \
	    bufptr = bufArray

/* *********************************************************** */

#define	FREE_STEPS_MEM                                          \
	if (maxsteps > STEPS_LIMIT)                             \
	    __mlib_free(bufptr)

/* *********************************************************** */

#define	FREE_MEM                                                \
	FREE_REGION_MEM;                                        \
	FREE_SLICE_MEM;                                         \
	FREE_STEPS_MEM

/* *********************************************************** */

typedef struct
{
	mlib_u32 nPrev;
	mlib_s32 xSrc;
	mlib_s32 ySrc;
	mlib_s32 zSrc;
} bufData;

/* *********************************************************** */

#define	BUFSIZE	sizeof (bufData)

/* *********************************************************** */

#define	TRILINEAR_INT(a, b, c, Po, Px, Py, Pxy, Pz, Pxz, Pyz,    \
	Pxyz, rsl)                                               \
	{                                                        \
	    mlib_s32 Px0, Px1, Px2, Px3, Py0, Py1;               \
	                                                         \
	    b >>= MLIB_SHIFT_HALF;                               \
	    c >>= MLIB_SHIFT_HALF;                               \
	    Px0 = (Po << MLIB_SHIFT) + a * (Px - Po);            \
	    Px1 = (Py << MLIB_SHIFT) + a * (Pxy - Py);           \
	    Px2 = (Pz << MLIB_SHIFT) + a * (Pxz - Pz);           \
	    Px3 = (Pyz << MLIB_SHIFT) + a * (Pxyz - Pyz);        \
	    Py0 = Px0 + b * ((Px1 - Px0) >> MLIB_SHIFT_HALF);    \
	    Py1 = Px2 + b * ((Px3 - Px2) >> MLIB_SHIFT_HALF);    \
	    rsl = (Py0 + c * ((Py1 -                             \
		    Py0) >> MLIB_SHIFT_HALF)) >> MLIB_SHIFT;     \
	}

/* *********************************************************** */

#define	TRILINEAR(a, b, c, Po, Px, Py, Pxy, Pz, Pxz, Pyz,       \
	Pxyz, rsl)                                              \
	{                                                       \
	    mlib_d64 Px0, Px1, Px2, Px3, Py0, Py1;              \
	                                                        \
	    Px0 = Po + a * (Px - Po);                           \
	    Px1 = Py + a * (Pxy - Py);                          \
	    Px2 = Pz + a * (Pxz - Pz);                          \
	    Px3 = Pyz + a * (Pxyz - Pyz);                       \
	    Py0 = Px0 + b * (Px1 - Px0);                        \
	    Py1 = Px2 + b * (Px3 - Px2);                        \
	    rsl = Py0 + c * (Py1 - Py0);                        \
	}

/* *********************************************************** */

mlib_status
__mlib_VolumeRayCast_General_Divergent_Trilinear_U8_U8(
	mlib_rays *rays,
	const mlib_genvolume *vol,
	void *buffer)
{
	mlib_u8 **slices;
	mlib_s32 *offsets;
	mlib_s32 nslices;
	mlib_s32 nregions;
	mlib_s32 *zcoords;
	mlib_s32 zcoordArr[REGION_LIMIT];
	mlib_s32 *zindices;
	mlib_d64 *zsrates;
	mlib_u8 **results;
	mlib_s32 nrays;
	mlib_s32 *nsteps;
	mlib_s32 maxsteps;
	mlib_u32 *sliceLst;
	mlib_u32 lstArray[SLICE_LIMIT];
	bufData *bufptr;
	bufData bufArray[STEPS_LIMIT * MAX_RAY_NUM];
	mlib_u32 nstep, nray, indx;
	mlib_u8 *slicePtr, *slicePtrN;
	mlib_u8 *resultPtr;
	mlib_s32 nRaySteps;
	mlib_s32 nregion, nslice, zindex;
	mlib_s32 xsrc, ysrc;
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

	ALLOC_REGION_MEM;

	for (i = 0; i <= nregions; i++)
		zcoords[i] = vol->zcoords[i] * MLIB_PREC + 0.5;

	zsrates = vol->zsrates;

	ALLOC_SLICE_MEM;

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

	ALLOC_STEPS_MEM;

	if (nregions == 1) {
		for (i = 0; i < nrays; i++) {
			X = rays->starts[i][0] * MLIB_PREC + 0.5;
			Y = rays->starts[i][1] * MLIB_PREC + 0.5;
			Z = rays->starts[i][2] * MLIB_PREC + 0.5;

			dX = (mlib_s32)(rays->incs[i][0] * MLIB_PREC + 0.5);
			dY = (mlib_s32)(rays->incs[i][1] * MLIB_PREC + 0.5);
			dZ = (mlib_s32)(rays->incs[i][2] * MLIB_PREC + 0.5);

			nRaySteps = nsteps[i];

			nregion = 0;
			zsrate = zsrates[0];
			zcoord = zcoords[0];
			zindex = zindices[0];

			ssZ = (Z - zcoord) * zsrate;
			dssZ = dZ * zsrate;

			for (j = 0; j < nRaySteps; j++) {
				nslice = zindex + (ssZ >> MLIB_SHIFT);

				indx = (j << NSTEP_SHIFT) + i;

				bufptr[indx].nPrev = sliceLst[nslice];
				bufptr[indx].xSrc = X;
				bufptr[indx].ySrc = Y;
				bufptr[indx].zSrc = ssZ;

				sliceLst[nslice] = indx;

				X += dX;
				Y += dY;

				ssZ += dssZ;
			}
		}
	} else {
		for (i = 0; i < nrays; i++) {
			X = rays->starts[i][0] * MLIB_PREC + 0.5;
			Y = rays->starts[i][1] * MLIB_PREC + 0.5;
			Z = rays->starts[i][2] * MLIB_PREC + 0.5;

			dX = (mlib_s32)(rays->incs[i][0] * MLIB_PREC + 0.5);
			dY = (mlib_s32)(rays->incs[i][1] * MLIB_PREC + 0.5);
			dZ = (mlib_s32)(rays->incs[i][2] * MLIB_PREC + 0.5);

			nRaySteps = nsteps[i];

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

					nslice = zindex + (ssZ >> MLIB_SHIFT);

					indx = (j << NSTEP_SHIFT) + i;

					bufptr[indx].nPrev = sliceLst[nslice];
					bufptr[indx].xSrc = X;
					bufptr[indx].ySrc = Y;
					bufptr[indx].zSrc = ssZ;

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

					nslice = zindex + (ssZ >> MLIB_SHIFT);

					indx = (j << NSTEP_SHIFT) + i;

					bufptr[indx].nPrev = sliceLst[nslice];
					bufptr[indx].xSrc = X;
					bufptr[indx].ySrc = Y;
					bufptr[indx].zSrc = ssZ;

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
		slicePtrN = slices[i + 1];
		indx = sliceLst[i];
		nstep = (indx >> 6);
		nray = (indx & 0x3F);

		for (; indx != MLIB_U32_MAX; ) {
			mlib_s32 dx, dy, dz;

			xsrc = bufptr[indx].xSrc >> MLIB_SHIFT;
			ysrc = bufptr[indx].ySrc >> MLIB_SHIFT;

			dx = bufptr[indx].xSrc & MLIB_MASK;
			dy = bufptr[indx].ySrc & MLIB_MASK;
			dz = bufptr[indx].zSrc & MLIB_MASK;

			resultPtr = results[nstep];

			TRILINEAR_INT(dx, dy, dz,
				slicePtr[offsets[ysrc] + xsrc],
				slicePtr[offsets[ysrc] + xsrc + 1],
				slicePtr[offsets[ysrc + 1] + xsrc],
				slicePtr[offsets[ysrc + 1] + xsrc + 1],
				slicePtrN[offsets[ysrc] + xsrc],
				slicePtrN[offsets[ysrc] + xsrc + 1],
				slicePtrN[offsets[ysrc + 1] + xsrc],
				slicePtrN[offsets[ysrc + 1] + xsrc + 1],
				resultPtr[nray]);

			indx = bufptr[indx].nPrev;
			nstep = indx >> NSTEP_SHIFT;
			nray = indx & NRAY_MASK;
		}
	}

	FREE_MEM;

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VolumeRayCast_General_Divergent_Trilinear_S16_S16(
	mlib_rays *rays,
	const mlib_genvolume *vol,
	void *buffer)
{
	mlib_u8 **slices;
	mlib_s32 *offsets;
	mlib_s32 nslices;
	mlib_s32 nregions;
	mlib_s32 *zcoords;
	mlib_s32 zcoordArr[REGION_LIMIT];
	mlib_s32 *zindices;
	mlib_d64 *zsrates;
	mlib_u16 **results;
	mlib_s32 nrays;
	mlib_s32 dX, dY, dZ;
	mlib_s32 *nsteps;
	mlib_s32 maxsteps;
	mlib_u32 *sliceLst;
	mlib_u32 lstArray[SLICE_LIMIT];
	bufData *bufptr;
	bufData bufArray[STEPS_LIMIT * MAX_RAY_NUM];
	mlib_u32 nstep, nray, indx;
	mlib_u8 *slicePtr, *slicePtrN;
	mlib_s16 *rowPtr, *rowPtr1, *rowPtr2, *rowPtr3;
	mlib_u16 *resultPtr;
	mlib_s32 nRaySteps;
	mlib_s32 nregion, nslice, zindex;
	mlib_s32 xsrc, ysrc;
	mlib_d64 zsrate;
	mlib_s32 zcoord, ssZ;
	mlib_s32 X, Y, Z, dssZ;
	mlib_s32 i, j;

	if (rays == NULL || vol == NULL)
		return (MLIB_NULLPOINTER);

	slices = (mlib_u8 **)vol->slices;
	offsets = vol->offsets;
	nslices = vol->nslices;
	nregions = vol->nregions;
	zindices = vol->zindices;

	ALLOC_REGION_MEM;

	for (i = 0; i <= nregions; i++)
		zcoords[i] = vol->zcoords[i] * MLIB_PREC;

	zsrates = vol->zsrates;

	ALLOC_SLICE_MEM;

	for (i = 0; i < nslices; i++)
		sliceLst[i] = MLIB_U32_MAX;

	results = (mlib_u16 **)rays->results;
	nrays = rays->nrays;

	dX = (mlib_s32)(rays->incs[0][0] * MLIB_PREC + 0.5);
	dY = (mlib_s32)(rays->incs[0][1] * MLIB_PREC + 0.5);
	dZ = (mlib_s32)(rays->incs[0][2] * MLIB_PREC + 0.5);

	nsteps = rays->nsteps;

	maxsteps = nsteps[0];

	for (i = 1; i < nrays; i++) {
		if (nsteps[i] > maxsteps)
			maxsteps = nsteps[i];
	}

	ALLOC_STEPS_MEM;

	if (nregions == 1) {
		for (i = 0; i < nrays; i++) {
			X = rays->starts[i][0] * MLIB_PREC + 0.5;
			Y = rays->starts[i][1] * MLIB_PREC + 0.5;
			Z = rays->starts[i][2] * MLIB_PREC + 0.5;

			dX = (mlib_s32)(rays->incs[i][0] * MLIB_PREC + 0.5);
			dY = (mlib_s32)(rays->incs[i][1] * MLIB_PREC + 0.5);
			dZ = (mlib_s32)(rays->incs[i][2] * MLIB_PREC + 0.5);

			nRaySteps = nsteps[i];

			nregion = 0;
			zsrate = zsrates[0];
			zcoord = zcoords[0];
			zindex = zindices[0];

			ssZ = (Z - zcoord) * zsrate;
			dssZ = dZ * zsrate;

			for (j = 0; j < nRaySteps; j++) {
				nslice = zindex + (ssZ >> MLIB_SHIFT);

				indx = (j << NSTEP_SHIFT) + i;

				bufptr[indx].nPrev = sliceLst[nslice];
				bufptr[indx].xSrc = X;
				bufptr[indx].ySrc = Y;
				bufptr[indx].zSrc = ssZ;

				sliceLst[nslice] = indx;

				X += dX;
				Y += dY;

				ssZ += dssZ;
			}
		}
	} else {
		for (i = 0; i < nrays; i++) {
			X = rays->starts[i][0] * MLIB_PREC + 0.5;
			Y = rays->starts[i][1] * MLIB_PREC + 0.5;
			Z = rays->starts[i][2] * MLIB_PREC + 0.5;

			dX = (mlib_s32)(rays->incs[i][0] * MLIB_PREC + 0.5);
			dY = (mlib_s32)(rays->incs[i][1] * MLIB_PREC + 0.5);
			dZ = (mlib_s32)(rays->incs[i][2] * MLIB_PREC + 0.5);

			nRaySteps = nsteps[i];

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

					nslice = zindex + (ssZ >> MLIB_SHIFT);

					indx = (j << NSTEP_SHIFT) + i;

					bufptr[indx].nPrev = sliceLst[nslice];
					bufptr[indx].xSrc = X;
					bufptr[indx].ySrc = Y;
					bufptr[indx].zSrc = ssZ;

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

					nslice = zindex + (ssZ >> MLIB_SHIFT);

					indx = (j << NSTEP_SHIFT) + i;

					bufptr[indx].nPrev = sliceLst[nslice];
					bufptr[indx].xSrc = X;
					bufptr[indx].ySrc = Y;
					bufptr[indx].zSrc = ssZ;

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
		slicePtrN = slices[i + 1];
		indx = sliceLst[i];
		nstep = (indx >> 6);
		nray = (indx & 0x3F);

		for (; indx != MLIB_U32_MAX; ) {
			mlib_d64 dx, dy, dz;

			xsrc = bufptr[indx].xSrc >> MLIB_SHIFT;
			ysrc = bufptr[indx].ySrc >> MLIB_SHIFT;

			dx = (mlib_d64)(bufptr[indx].xSrc & MLIB_MASK) /
				MLIB_PREC;
			dy = (mlib_d64)(bufptr[indx].ySrc & MLIB_MASK) /
				MLIB_PREC;
			dz = (mlib_d64)(bufptr[indx].zSrc & MLIB_MASK) /
				MLIB_PREC;

			resultPtr = results[nstep];

			rowPtr = (mlib_s16 *)(slicePtr + offsets[ysrc]);
			rowPtr1 = (mlib_s16 *)(slicePtr + offsets[ysrc + 1]);
			rowPtr2 = (mlib_s16 *)(slicePtrN + offsets[ysrc]);
			rowPtr3 = (mlib_s16 *)(slicePtrN + offsets[ysrc + 1]);

			TRILINEAR(dx, dy, dz,
				rowPtr[xsrc],
				rowPtr[xsrc + 1],
				rowPtr1[xsrc],
				rowPtr1[xsrc + 1],
				rowPtr2[xsrc],
				rowPtr2[xsrc + 1],
				rowPtr3[xsrc],
				rowPtr3[xsrc + 1], resultPtr[nray]);

			indx = bufptr[indx].nPrev;
			nstep = (indx >> 6);
			nray = (indx & 0x3F);
		}
	}

	FREE_MEM;

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
