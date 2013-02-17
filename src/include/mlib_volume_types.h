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

#ifndef _MLIB_VOLUME_TYPES_H
#define	_MLIB_VOLUME_TYPES_H

#pragma ident	"@(#)mlib_volume_types.h	9.3	07/10/09 SMI"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
	void **slices;
	mlib_s32 xsize;
	mlib_s32 ysize;
	mlib_s32 *offsets;
	mlib_s32 nslices;
	mlib_s32 nregions;
	mlib_s32 *zindices;
	mlib_d64 *zcoords;
	mlib_d64 *zsrates;
} mlib_genvolume;

typedef struct
{
	void *voxels;
	mlib_s32 xysize;
	mlib_s32 zsize;
} mlib_blkvolume;

typedef struct
{
	void **results;
	mlib_s32 nrays;
	mlib_d64 **starts;
	mlib_d64 **incs;
	mlib_s32 *nsteps;
	mlib_d64 *image_z;
	mlib_d64 *im_incs;
} mlib_rays;

#ifdef __cplusplus
}
#endif

#endif /* _MLIB_VOLUME_TYPES_H */
