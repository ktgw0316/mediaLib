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

#pragma ident	"@(#)mlib_ImageYProj_Fp.c	9.2	07/10/09 SMI"

/*
 * FUNCTION
 *      mlib_ImageYProj_Fp - calculates Y Projections for input image
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageYProj_Fp(mlib_d64         *yproj,
 *                                     const mlib_image *src)
 *
 * ARGUMENTS
 *      yproj       Pointer to Y-projection vector
 *      src         Pointer to an image
 *
 * RESTRICTION
 *      The image must be a single channel image.
 *      It can be of MLIB_FLOAT or MLIB_DOUBLE data type.
 *
 * DESCRIPTION
 *                 w-1
 *      yproj[j] = SUM (src[i][j]);
 *                 i = 0
 */

#include <mlib_image.h>
#include <mlib_ImageCheck.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageYProj_Fp = __mlib_ImageYProj_Fp

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_ImageYProj_Fp) mlib_ImageYProj_Fp
    __attribute__((weak, alias("__mlib_ImageYProj_Fp")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

static void mlib_ImageYProj_F32(
    const mlib_image *src,
    mlib_d64 *yproj);
static void mlib_ImageYProj_D64(
    const mlib_image *src,
    mlib_d64 *yproj);

/* *********************************************************** */

mlib_status
__mlib_ImageYProj_Fp(
    mlib_d64 *yproj,
    const mlib_image *src)
{
/* check for obvious errors */
	MLIB_IMAGE_CHECK(src);
	MLIB_IMAGE_HAVE_CHAN(src, 1);

	if (yproj == NULL)
		return (MLIB_NULLPOINTER);

	switch (mlib_ImageGetType(src)) {
	case MLIB_FLOAT:
		mlib_ImageYProj_F32(src, yproj);
		break;

	case MLIB_DOUBLE:
		mlib_ImageYProj_D64(src, yproj);
		break;

	default:
		return (MLIB_FAILURE);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

void
mlib_ImageYProj_F32(
    const mlib_image *src,
    mlib_d64 *yproj)
{
/* width of source image */
	int Width = mlib_ImageGetWidth(src);

/* height of source image */
	int Height = mlib_ImageGetHeight(src);

/* bytes to next row */
	int YStride = (mlib_ImageGetStride(src)) >> 2;

/* pointers to the data of image */
	mlib_f32 *LinePntr = (mlib_f32 *)mlib_ImageGetData(src), *PixelPntr;
	mlib_d64 Accum1;
	int i, j;

	for (j = 0; j < Height; j++) {
		Accum1 = 0;
		PixelPntr = LinePntr;

		for (i = 0; i < Width; i++)
			Accum1 += (*PixelPntr++);
		yproj[j] = Accum1;

		LinePntr += YStride;
	}
}

/* *********************************************************** */

void
mlib_ImageYProj_D64(
    const mlib_image *src,
    mlib_d64 *yproj)
{
/* width of source image */
	int Width = mlib_ImageGetWidth(src);

/* height of source image */
	int Height = mlib_ImageGetHeight(src);

/* bytes to next row */
	int YStride = (mlib_ImageGetStride(src)) >> 3;

/* pointers to the data of image */
	mlib_d64 *LinePntr = (mlib_d64 *)mlib_ImageGetData(src), *PixelPntr;
	mlib_d64 Accum1;
	int i, j;

	for (j = 0; j < Height; j++) {
		Accum1 = 0;
		PixelPntr = LinePntr;

		for (i = 0; i < Width; i++)
			Accum1 += (*PixelPntr++);
		yproj[j] = Accum1;

		LinePntr += YStride;
	}
}

/* *********************************************************** */
