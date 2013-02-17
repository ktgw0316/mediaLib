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

#pragma ident	"@(#)mlib_VolumeRayCast_GDN.c	9.2	07/10/09 SMI"

/*
 *  FUNCTION
 *    mlib_VolumeRayCast_General_Divergent_Nearest_[U8_U8|S16_S16]
 *      Cast divergent rays through a three dimensional data set
 *      and return the interpolated samples.
 *
 *  SYNOPSIS
 *    mlib_status mlib_VolumeRayCast_General_Divergent_Nearest_[U8_U8|S16_S16]
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

#include <stdlib.h>
#include <mlib_volume.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_VolumeRayCast_General_Divergent_Nearest_S16_S16 = \
__mlib_VolumeRayCast_General_Divergent_Nearest_S16_S16
#pragma weak mlib_VolumeRayCast_General_Divergent_Nearest_U8_U8 = \
	__mlib_VolumeRayCast_General_Divergent_Nearest_U8_U8
#elif defined(__GNUC__)

__typeof__(__mlib_VolumeRayCast_General_Divergent_Nearest_S16_S16)
	mlib_VolumeRayCast_General_Divergent_Nearest_S16_S16
	__attribute__((weak,
	alias("__mlib_VolumeRayCast_General_Divergent_Nearest_S16_S16")));
__typeof__(__mlib_VolumeRayCast_General_Divergent_Nearest_U8_U8)
	mlib_VolumeRayCast_General_Divergent_Nearest_U8_U8
	__attribute__((weak,
	alias("__mlib_VolumeRayCast_General_Divergent_Nearest_U8_U8")));

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
	mlib_u16 xSrc;
	mlib_u16 ySrc;
} bufData;

#define	BUFSIZE	sizeof (bufData)

/* *********************************************************** */

mlib_status
__mlib_VolumeRayCast_General_Divergent_Nearest_U8_U8(
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
	mlib_u8 *slicePtr;
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
		zcoords[i] = vol->zcoords[i] * MLIB_PREC;

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
			X = (rays->starts[i][0] + 0.5) * MLIB_PREC;
			Y = (rays->starts[i][1] + 0.5) * MLIB_PREC;
			Z = rays->starts[i][2] * MLIB_PREC;

			dX = (mlib_s32)(rays->incs[i][0] * MLIB_PREC + 0.5);
			dY = (mlib_s32)(rays->incs[i][1] * MLIB_PREC + 0.5);
			dZ = (mlib_s32)(rays->incs[i][2] * MLIB_PREC);

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

mlib_status
__mlib_VolumeRayCast_General_Divergent_Nearest_S16_S16(
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
	dZ = (mlib_s32)(rays->incs[0][2] * MLIB_PREC);

	nsteps = rays->nsteps;

	maxsteps = nsteps[0];

	for (i = 1; i < nrays; i++) {
		if (nsteps[i] > maxsteps)
			maxsteps = nsteps[i];
	}

	ALLOC_STEPS_MEM;

	if (nregions == 1) {
		for (i = 0; i < nrays; i++) {
			X = (rays->starts[i][0] + 0.5) * MLIB_PREC;
			Y = (rays->starts[i][1] + 0.5) * MLIB_PREC;
			Z = rays->starts[i][2] * MLIB_PREC;

			dX = (mlib_s32)(rays->incs[i][0] * MLIB_PREC + 0.5);
			dY = (mlib_s32)(rays->incs[i][1] * MLIB_PREC + 0.5);
			dZ = (mlib_s32)(rays->incs[i][2] * MLIB_PREC);

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
