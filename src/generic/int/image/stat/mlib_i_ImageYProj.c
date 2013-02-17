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

#pragma ident	"@(#)mlib_i_ImageYProj.c	9.3	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_ImageYProj - calculates Y Projections for input image
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageYProj(mlib_d64         *yproj,
 *                                  const mlib_image *src)
 *
 * ARGUMENTS
 *      yproj       Pointer to Y-projection vector
 *      src         Pointer to an image
 *
 * RESTRICTION
 *      The image must be a single channel image.
 *      It can be of MLIB_BYTE, MLIB_SHORT, MLIB_USHORT or MLIB_INT data type.
 *
 * DESCRIPTION
 *                 w-1
 *      yproj[j] = SUM (src[i][j])
 *                 i = 0
 */

#include <mlib_image.h>
#include <mlib_ImageCheck.h>

/* *********************************************************** */
#ifndef _NO_LONGLONG /* for 64-bit platform */
/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageYProj = __mlib_ImageYProj

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_ImageYProj) mlib_ImageYProj
    __attribute__((weak, alias("__mlib_ImageYProj")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	MASK_000f	0x000000FF
#define	MASK_0f0f	0x00FF00FF

#define	MASK_00ff	0x0000FFFF

#define	MASK_0003	0x00000003

/* *********************************************************** */

static void mlib_c_ImageYProj_U8(
    const mlib_image *src,
    mlib_d64 *yproj);

static void mlib_c_ImageYProj_U16(
    const mlib_image *src,
    mlib_d64 *yproj);

static void mlib_c_ImageYProj_S16(
    const mlib_image *src,
    mlib_d64 *yproj);

static void mlib_c_ImageYProj_S32(
    const mlib_image *src,
    mlib_d64 *yproj);

/* *********************************************************** */

mlib_status
__mlib_ImageYProj(
    mlib_d64 *yproj,
    const mlib_image *src)
{
/* check for obvious errors */
	MLIB_IMAGE_CHECK(src);
	MLIB_IMAGE_HAVE_CHAN(src, 1);

	if (yproj == NULL)
		return (MLIB_NULLPOINTER);

	switch (mlib_ImageGetType(src)) {
/* handle MLIB_BYTE data type of image */
	case MLIB_BYTE:
		mlib_c_ImageYProj_U8(src, yproj);
		break;

/* handle MLIB_USHORT data type of image */
	case MLIB_USHORT:
		mlib_c_ImageYProj_U16(src, yproj);
		break;

/* handle MLIB_SHORT data type of image */
	case MLIB_SHORT:
		mlib_c_ImageYProj_S16(src, yproj);
		break;

/* handle MLIB_INT data type of image */
	case MLIB_INT:
		mlib_c_ImageYProj_S32(src, yproj);
		break;

/* discard any other data types */
	default:
		return (MLIB_FAILURE);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

void
mlib_c_ImageYProj_U8(
    const mlib_image *src,
    mlib_d64 *yproj)
{
/* width of source image */
	mlib_s32 Width = mlib_ImageGetWidth(src);

/* height of source image */
	mlib_s32 Height = mlib_ImageGetHeight(src);

/* bytes to next row */
	mlib_s32 YStride = mlib_ImageGetStride(src);

/* pointers to the data of image */
	mlib_u8 *LinePntr = (mlib_u8 *)mlib_ImageGetData(src),
	    *PixelPntr = LinePntr;
	mlib_u32 *LoadPntr;
	mlib_u32 Load;
	mlib_u32 Accum1, Accum2;
	mlib_s32 alignSteps;

/* number of pixel to process */
	mlib_s32 PixelNumber;

/* number of 32-bit loads */
	mlib_s32 loadsNumber;

/* number of cycles 256 * 32-bit loads */
	mlib_s32 cycleNumber;
	mlib_s32 i, j, k;
	mlib_s64 *lyproj = __mlib_malloc(Height * sizeof (mlib_s64));
	if (lyproj == NULL) {
		__mlib_free(lyproj);
		lyproj = NULL;
		return;
	}

	for (j = 0; j < Height; j++) {

		PixelNumber = Width;
	//	yproj[j] = 0;
		Accum1 = 0;
		Accum2 = 0;

		alignSteps = (4 - (mlib_addr)PixelPntr) & 3;

		for (i = 0; i < alignSteps; i++) {
			Accum1 += (*PixelPntr++);
			PixelNumber--;

			if (PixelNumber == 0)
				break;
		}

		lyproj[j] = Accum1;

		Accum1 = 0;

		LoadPntr = (mlib_u32 *)PixelPntr;

		loadsNumber = PixelNumber >> 2;
		cycleNumber = loadsNumber >> 8;

		for (i = 0; i < cycleNumber; i++) {
			for (k = 0; k < 256; k++) {
				Load = (*LoadPntr++);

				Accum1 += (Load & MASK_0f0f);
				Accum2 += ((Load & ~MASK_0f0f) >> 8);
			}

			lyproj[j] += ((Accum1 & MASK_00ff)
			    + ((Accum1 & ~MASK_00ff) >> 16)
			    + (Accum2 & MASK_00ff)
			    + ((Accum2 & ~MASK_00ff) >> 16));

			Accum1 = 0;
			Accum2 = 0;
		}

		loadsNumber &= MASK_000f;

		for (i = 0; i < loadsNumber; i++) {
			Load = (*LoadPntr++);

			Accum1 += (Load & MASK_0f0f);
			Accum2 += ((Load & ~MASK_0f0f) >> 8);
		}

		lyproj[j] += ((Accum1 & MASK_00ff)
		    + ((Accum1 & ~MASK_00ff) >> 16)
		    + (Accum2 & MASK_00ff)
		    + ((Accum2 & ~MASK_00ff) >> 16));

		Accum1 = 0;
		Accum2 = 0;

		PixelNumber &= MASK_0003;

		PixelPntr = (mlib_u8 *)LoadPntr;
		for (i = 0; i < PixelNumber; i++)
			Accum1 += (*PixelPntr++);

		lyproj[j] += Accum1;
		yproj[j] = lyproj[j];

		LinePntr += YStride;
		PixelPntr = LinePntr;
	}
	if (lyproj != NULL) {
		__mlib_free(lyproj);
		lyproj = NULL;
	}
}

/* *********************************************************** */

void
mlib_c_ImageYProj_U16(
    const mlib_image *src,
    mlib_d64 *yproj)
{
/* width of source image */
	mlib_s32 Width = mlib_ImageGetWidth(src);

/* height of source image */
	mlib_s32 Height = mlib_ImageGetHeight(src);

/* bytes to next row */
	mlib_s32 YStride = (mlib_ImageGetStride(src)) / 2;

/* pointers to the data of image */
	mlib_u16 *LinePntr = (mlib_u16 *)mlib_ImageGetData(src);
	mlib_u16 *PixelPntr = (mlib_u16 *)mlib_ImageGetData(src);
	mlib_s32 Accum1;
	mlib_s32 i, j;

	for (j = 0; j < Height; j++) {
		Accum1 = 0;

		for (i = 0; i < Width; i++)
			Accum1 += (*PixelPntr++);
		yproj[j] = (mlib_d64)Accum1;

		LinePntr += YStride;
		PixelPntr = LinePntr;
	}
}

/* *********************************************************** */

void
mlib_c_ImageYProj_S16(
    const mlib_image *src,
    mlib_d64 *yproj)
{
/* width of source image */
	mlib_s32 Width = mlib_ImageGetWidth(src);

/* height of source image */
	mlib_s32 Height = mlib_ImageGetHeight(src);

/* bytes to next row */
	mlib_s32 YStride = (mlib_ImageGetStride(src)) / 2;

/* pointers to the data of image */
	mlib_s16 *LinePntr = (mlib_s16 *)mlib_ImageGetData(src);
	mlib_s16 *PixelPntr = (mlib_s16 *)mlib_ImageGetData(src);
	mlib_s32 Accum1;
	mlib_s32 i, j;

	for (j = 0; j < Height; j++) {
		Accum1 = 0;

		for (i = 0; i < Width; i++)
			Accum1 += (*PixelPntr++);
		yproj[j] = (mlib_d64)Accum1;

		LinePntr += YStride;
		PixelPntr = LinePntr;
	}
}

/* *********************************************************** */

void
mlib_c_ImageYProj_S32(
    const mlib_image *src,
    mlib_d64 *yproj)
{
/* width of source image */
	mlib_s32 Width = mlib_ImageGetWidth(src);

/* height of source image */
	mlib_s32 Height = mlib_ImageGetHeight(src);

/* bytes to next row */
	mlib_s32 YStride = (mlib_ImageGetStride(src)) / 4;

/* pointers to the data of image */
	mlib_s32 *LinePntr = (mlib_s32 *)mlib_ImageGetData(src);
	mlib_s32 *PixelPntr = (mlib_s32 *)mlib_ImageGetData(src);
	mlib_s64 Accum1;
	mlib_s32 i, j;

	for (j = 0; j < Height; j++) {
		Accum1 = 0;

		for (i = 0; i < Width; i++)
			Accum1 += (*PixelPntr++);
		yproj[j] = (mlib_d64)Accum1;

		LinePntr += YStride;
		PixelPntr = LinePntr;
	}
}

/* *********************************************************** */
#else /* _NO_LONGLONG */
/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageYProj = __mlib_ImageYProj

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_ImageYProj) mlib_ImageYProj
    __attribute__((weak, alias("__mlib_ImageYProj")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	MASK_000f	0x000000FF
#define	MASK_0f0f	0x00FF00FF

#define	MASK_00ff	0x0000FFFF

#define	MASK_0003	0x00000003

/* *********************************************************** */

static void mlib_c_ImageYProj_U8(
    const mlib_image *src,
    mlib_d64 *yproj);

static void mlib_c_ImageYProj_U16(
    const mlib_image *src,
    mlib_d64 *yproj);

static void mlib_c_ImageYProj_S16(
    const mlib_image *src,
    mlib_d64 *yproj);

static void mlib_c_ImageYProj_S32(
    const mlib_image *src,
    mlib_d64 *yproj);

/* *********************************************************** */

mlib_status
__mlib_ImageYProj(
    mlib_d64 *yproj,
    const mlib_image *src)
{
/* check for obvious errors */
	MLIB_IMAGE_CHECK(src);
	MLIB_IMAGE_HAVE_CHAN(src, 1);

	if (yproj == NULL)
		return (MLIB_NULLPOINTER);

	switch (mlib_ImageGetType(src)) {
/* handle MLIB_BYTE data type of image */
	case MLIB_BYTE:
		mlib_c_ImageYProj_U8(src, yproj);
		break;

/* handle MLIB_USHORT data type of image */
	case MLIB_USHORT:
		mlib_c_ImageYProj_U16(src, yproj);
		break;

/* handle MLIB_SHORT data type of image */
	case MLIB_SHORT:
		mlib_c_ImageYProj_S16(src, yproj);
		break;

/* handle MLIB_INT data type of image */
	case MLIB_INT:
		mlib_c_ImageYProj_S32(src, yproj);
		break;

/* discard any other data types */
	default:
		return (MLIB_FAILURE);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

void
mlib_c_ImageYProj_U8(
    const mlib_image *src,
    mlib_d64 *yproj)
{
/* width of source image */
	mlib_s32 Width = mlib_ImageGetWidth(src);

/* height of source image */
	mlib_s32 Height = mlib_ImageGetHeight(src);

/* bytes to next row */
	mlib_s32 YStride = mlib_ImageGetStride(src);

/* pointers to the data of image */
	mlib_u8 *LinePntr = (mlib_u8 *)mlib_ImageGetData(src),
	    *PixelPntr = LinePntr;
	mlib_u32 *LoadPntr;
	mlib_u32 Load;
	mlib_u32 Accum1, Accum2;
	mlib_s32 alignSteps;

/* number of pixel to process */
	mlib_s32 PixelNumber;

/* number of 32-bit loads */
	mlib_s32 loadsNumber;

/* number of cycles 256 * 32-bit loads */
	mlib_s32 cycleNumber;
	mlib_s32 i, j, k;

	for (j = 0; j < Height; j++) {

		PixelNumber = Width;
		yproj[j] = 0;
		Accum1 = 0;
		Accum2 = 0;

		alignSteps = (4 - (mlib_addr)PixelPntr) & 3;

		for (i = 0; i < alignSteps; i++) {
			Accum1 += (*PixelPntr++);
			PixelNumber--;

			if (PixelNumber == 0)
				break;
		}

		yproj[j] += (mlib_d64)Accum1;

		Accum1 = 0;

		LoadPntr = (mlib_u32 *)PixelPntr;

		loadsNumber = PixelNumber >> 2;
		cycleNumber = loadsNumber >> 8;

		for (i = 0; i < cycleNumber; i++) {
			for (k = 0; k < 256; k++) {
				Load = (*LoadPntr++);

				Accum1 += (Load & MASK_0f0f);
				Accum2 += ((Load & ~MASK_0f0f) >> 8);
			}

			yproj[j] += (mlib_d64)((Accum1 & MASK_00ff)
			    + ((Accum1 & ~MASK_00ff) >> 16)
			    + (Accum2 & MASK_00ff)
			    + ((Accum2 & ~MASK_00ff) >> 16));
			Accum1 = 0;
			Accum2 = 0;
		}

		loadsNumber &= MASK_000f;

		for (i = 0; i < loadsNumber; i++) {
			Load = (*LoadPntr++);

			Accum1 += (Load & MASK_0f0f);
			Accum2 += ((Load & ~MASK_0f0f) >> 8);
		}

		yproj[j] += (mlib_d64)((Accum1 & MASK_00ff)
		    + ((Accum1 & ~MASK_00ff) >> 16)
		    + (Accum2 & MASK_00ff)
		    + ((Accum2 & ~MASK_00ff) >> 16));

		Accum1 = 0;
		Accum2 = 0;

		PixelNumber &= MASK_0003;

		PixelPntr = (mlib_u8 *)LoadPntr;
		for (i = 0; i < PixelNumber; i++)
			Accum1 += (*PixelPntr++);

		yproj[j] += (mlib_d64)Accum1;

		LinePntr += YStride;
		PixelPntr = LinePntr;
	}
}

/* *********************************************************** */

void
mlib_c_ImageYProj_U16(
    const mlib_image *src,
    mlib_d64 *yproj)
{
/* width of source image */
	mlib_s32 Width = mlib_ImageGetWidth(src);

/* height of source image */
	mlib_s32 Height = mlib_ImageGetHeight(src);

/* bytes to next row */
	mlib_s32 YStride = (mlib_ImageGetStride(src)) / 2;

/* pointers to the data of image */
	mlib_u16 *LinePntr = (mlib_u16 *)mlib_ImageGetData(src);
	mlib_u16 *PixelPntr = (mlib_u16 *)mlib_ImageGetData(src);
	mlib_s32 Accum1;
	mlib_s32 i, j;

	for (j = 0; j < Height; j++) {
		Accum1 = 0;

		for (i = 0; i < Width; i++)
			Accum1 += (*PixelPntr++);
		yproj[j] = (mlib_d64)Accum1;

		LinePntr += YStride;
		PixelPntr = LinePntr;
	}
}

/* *********************************************************** */

void
mlib_c_ImageYProj_S16(
    const mlib_image *src,
    mlib_d64 *yproj)
{
/* width of source image */
	mlib_s32 Width = mlib_ImageGetWidth(src);

/* height of source image */
	mlib_s32 Height = mlib_ImageGetHeight(src);

/* bytes to next row */
	mlib_s32 YStride = (mlib_ImageGetStride(src)) / 2;

/* pointers to the data of image */
	mlib_s16 *LinePntr = (mlib_s16 *)mlib_ImageGetData(src);
	mlib_s16 *PixelPntr = (mlib_s16 *)mlib_ImageGetData(src);
	mlib_s32 Accum1;
	mlib_s32 i, j;

	for (j = 0; j < Height; j++) {
		Accum1 = 0;

		for (i = 0; i < Width; i++)
			Accum1 += (*PixelPntr++);
		yproj[j] = (mlib_d64)Accum1;

		LinePntr += YStride;
		PixelPntr = LinePntr;
	}
}

/* *********************************************************** */

void
mlib_c_ImageYProj_S32(
    const mlib_image *src,
    mlib_d64 *yproj)
{
/* width of source image */
	mlib_s32 Width = mlib_ImageGetWidth(src);

/* height of source image */
	mlib_s32 Height = mlib_ImageGetHeight(src);

/* bytes to next row */
	mlib_s32 YStride = (mlib_ImageGetStride(src)) / 4;

/* pointers to the data of image */
	mlib_s32 *LinePntr = (mlib_s32 *)mlib_ImageGetData(src);
	mlib_s32 *PixelPntr = (mlib_s32 *)mlib_ImageGetData(src);
	mlib_d64 Accum1;
	mlib_s32 i, j;

	for (j = 0; j < Height; j++) {
		Accum1 = 0;

		for (i = 0; i < Width; i++)
			Accum1 += (*PixelPntr++);
		yproj[j] = (mlib_d64)Accum1;

		LinePntr += YStride;
		PixelPntr = LinePntr;
	}
}

/* *********************************************************** */
#endif /* end of ifndef _NO_LONGLONG */
/* *********************************************************** */
