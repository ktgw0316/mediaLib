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

#pragma ident	"@(#)mlib_i_ImageXProj.c	9.2	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_ImageXProj - calculates X Projections for input image
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageXProj(mlib_d64         *xproj,
 *                                  const mlib_image *src)
 *
 * ARGUMENTS
 *      xproj       Pointer to X-projection vector
 *      src         Pointer to an image
 *
 * RESTRICTION
 *      The image must be a single channel image.
 *      It can be of MLIB_BYTE, MLIB_SHORT, MLIB_USHORT or MLIB_INT data type.
 *
 * DESCRIPTION
 *                 h-1
 *      xproj[i] = SUM (src[i][j]);
 *                 j = 0
 */

#include <mlib_image.h>
#include <mlib_ImageCheck.h>
/* *********************************************************** */
#ifndef _NO_LONGLONG /* for 64-bit platform */
/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageXProj = __mlib_ImageXProj

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_ImageXProj) mlib_ImageXProj
    __attribute__((weak, alias("__mlib_ImageXProj")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	MASK_00ff00ff	0x00FF00FF
#define	MASK_0000ffff	0x0000FFFF

/* *********************************************************** */

static void mlib_c_ImageXProj_U8(
    const mlib_image *src,
    mlib_d64 *xproj);

static void mlib_c_ImageXProj_U16(
    const mlib_image *src,
    mlib_d64 *xproj);

static void mlib_c_ImageXProj_S16(
    const mlib_image *src,
    mlib_d64 *xproj);

static void mlib_c_ImageXProj_S32(
    const mlib_image *src,
    mlib_d64 *xproj);

/* *********************************************************** */

mlib_status
__mlib_ImageXProj(
    mlib_d64 *xproj,
    const mlib_image *src)
{
/* check for obvious errors */
	MLIB_IMAGE_CHECK(src);
	MLIB_IMAGE_HAVE_CHAN(src, 1);

	if (xproj == NULL)
		return (MLIB_NULLPOINTER);

	switch (mlib_ImageGetType(src)) {
/* handle MLIB_BYTE data type of image */
	case MLIB_BYTE:
		mlib_c_ImageXProj_U8(src, xproj);
		break;

/* handle MLIB_USHORT data type of image */
	case MLIB_USHORT:
		mlib_c_ImageXProj_U16(src, xproj);
		break;

/* handle MLIB_SHORT data type of image */
	case MLIB_SHORT:
		mlib_c_ImageXProj_S16(src, xproj);
		break;

/* handle MLIB_INT data type of image */
	case MLIB_INT:
		mlib_c_ImageXProj_S32(src, xproj);
		break;

/* discard any other data types */
	default:
		return (MLIB_FAILURE);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

void
mlib_c_ImageXProj_U8(
    const mlib_image *src,
    mlib_d64 *xproj)
{
	mlib_u8 *sp = mlib_ImageGetData(src), *spl;
	mlib_s32 width = mlib_ImageGetWidth(src),
	    height = mlib_ImageGetHeight(src),
	    stride = mlib_ImageGetStride(src),
	    i = 0, j, k, accm, accm0, accm1, offset = (4 - (mlib_addr)sp) & 3;
	mlib_s64 *lxproj = __mlib_malloc(width * sizeof (mlib_s64));
	if (lxproj == NULL) {
		__mlib_free(lxproj);
		lxproj = NULL;
		return;
	}

	if (!(stride & 3) && width > 3) {

		for (; i < offset; i++) {
			spl = sp + i;
			accm = 0;

			for (k = 0; k < height; k++, spl += stride) {
				accm += *spl;
			}

			xproj[i] = (mlib_d64)accm;
		}

		for (; i <= width - 4; i += 4) {
			accm0 = 0;
			accm1 = 0;
			spl = sp + i;

			lxproj[i] = 0;
			lxproj[i + 1] = 0;
			lxproj[i + 2] = 0;
			lxproj[i + 3] = 0;

			for (j = 0; j < height >> 8; j++) {

				for (k = 0; k < 256; k++, spl += stride) {
					accm0 +=
					    *(mlib_u32 *)spl & MASK_00ff00ff;
					accm1 +=
					    (*(mlib_u32 *)spl >> 8) &
					    MASK_00ff00ff;
				}

#ifdef _LITTLE_ENDIAN
			lxproj[i + 3] += ((accm1 >> 16) & MASK_0000ffff);
			lxproj[i + 2] += ((accm0 >> 16) & MASK_0000ffff);
			lxproj[i + 1] += (accm1 & MASK_0000ffff);
			lxproj[i] += (accm0 & MASK_0000ffff);

#else /* _LITTLE_ENDIAN */
			lxproj[i] += ((accm1 >> 16) & MASK_0000ffff);
			lxproj[i + 1] += ((accm0 >> 16) & MASK_0000ffff);
			lxproj[i + 2] += (accm1 & MASK_0000ffff);
			lxproj[i + 3] += (accm0 & MASK_0000ffff);
#endif /* _LITTLE_ENDIAN */
				accm0 = 0;
				accm1 = 0;
			}

			for (j <<= 8; j < height; j++, spl += stride) {
				accm0 += *(mlib_u32 *)spl & MASK_00ff00ff;
				accm1 +=
				    (*(mlib_u32 *)spl >> 8) & MASK_00ff00ff;
			}

#ifdef _LITTLE_ENDIAN
			lxproj[i + 3] += ((accm1 >> 16) & MASK_0000ffff);
			lxproj[i + 2] += ((accm0 >> 16) & MASK_0000ffff);
			lxproj[i + 1] += (accm1 & MASK_0000ffff);
			lxproj[i] += (accm0 & MASK_0000ffff);
#else /* _LITTLE_ENDIAN */
			lxproj[i] += ((accm1 >> 16) & MASK_0000ffff);
			lxproj[i + 1] += ((accm0 >> 16) & MASK_0000ffff);
			lxproj[i + 2] += (accm1 & MASK_0000ffff);
			lxproj[i + 3] += (accm0 & MASK_0000ffff);
#endif /* _LITTLE_ENDIAN */
			xproj[i] = lxproj[i];
			xproj[i + 1] = lxproj[i + 1];
			xproj[i + 2] = lxproj[i + 2];
			xproj[i + 3] = lxproj[i + 3];
		}
	}

	for (; i < width; i++) {
		accm = 0;
		for (j = 0; j < height; j++)
			accm += sp[j * stride + i];
		xproj[i] = (mlib_d64)accm;
	}
	if (lxproj != NULL) {
		__mlib_free(lxproj);
		lxproj = NULL;
	}
}

/* *********************************************************** */

void
mlib_c_ImageXProj_U16(
    const mlib_image *src,
    mlib_d64 *xproj)
{
/* width of source image */
	mlib_s32 Width = mlib_ImageGetWidth(src),
/* height of source image */
	    Height = mlib_ImageGetHeight(src),
/* bytes to next row */
	    YStride = mlib_ImageGetStride(src) >> 1;

/* pointers to the data of image */
	mlib_u16 *LinePntr = (mlib_u16 *)mlib_ImageGetData(src), *PixelPntr;
	mlib_s32 Accum0, Accum1, Accum2, Accum3, i, j;

	for (i = 0; i <= Width - 4; i += 4) {
		Accum0 = Accum1 = Accum2 = Accum3 = 0;
		PixelPntr = LinePntr + i;

		for (j = 0; j < Height; j++) {
			Accum0 += *PixelPntr;
			Accum1 += *(PixelPntr + 1);
			Accum2 += *(PixelPntr + 2);
			Accum3 += *(PixelPntr + 3);
			PixelPntr += YStride;
		}

		xproj[i] = (mlib_d64)Accum0;
		xproj[i + 1] = (mlib_d64)Accum1;
		xproj[i + 2] = (mlib_d64)Accum2;
		xproj[i + 3] = (mlib_d64)Accum3;
	}

	for (; i < Width; i++) {
		Accum0 = 0;
		PixelPntr = LinePntr + i;

		for (j = 0; j < Height; j++) {
			Accum0 += *(PixelPntr);
			PixelPntr += YStride;
		}

		xproj[i] = (mlib_d64)Accum0;
	}
}

/* *********************************************************** */

void
mlib_c_ImageXProj_S16(
    const mlib_image *src,
    mlib_d64 *xproj)
{
/* width of source image */
	mlib_s32 Width = mlib_ImageGetWidth(src),
/* height of source image */
	    Height = mlib_ImageGetHeight(src),
/* bytes to next row */
	    YStride = mlib_ImageGetStride(src) >> 1;

/* pointers to the data of image */
	mlib_s16 *LinePntr = (mlib_s16 *)mlib_ImageGetData(src), *PixelPntr;
	mlib_s32 Accum0, Accum1, Accum2, Accum3, i, j;

	for (i = 0; i <= Width - 4; i += 4) {
		Accum0 = Accum1 = Accum2 = Accum3 = 0;
		PixelPntr = LinePntr + i;

		for (j = 0; j < Height; j++) {
			Accum0 += *PixelPntr;
			Accum1 += *(PixelPntr + 1);
			Accum2 += *(PixelPntr + 2);
			Accum3 += *(PixelPntr + 3);
			PixelPntr += YStride;
		}

		xproj[i] = (mlib_d64)Accum0;
		xproj[i + 1] = (mlib_d64)Accum1;
		xproj[i + 2] = (mlib_d64)Accum2;
		xproj[i + 3] = (mlib_d64)Accum3;
	}

	for (; i < Width; i++) {
		Accum0 = 0;
		PixelPntr = LinePntr + i;

		for (j = 0; j < Height; j++) {
			Accum0 += *(PixelPntr);
			PixelPntr += YStride;
		}

		xproj[i] = (mlib_d64)Accum0;
	}
}

/* *********************************************************** */

void
mlib_c_ImageXProj_S32(
    const mlib_image *src,
    mlib_d64 *xproj)
{
/* width of source image */
	mlib_s32 Width = mlib_ImageGetWidth(src),
/* height of source image */
	    Height = mlib_ImageGetHeight(src),
/* bytes to next row */
	    YStride = mlib_ImageGetStride(src) >> 2;

/* pointers to the data of image */
	mlib_s32 *LinePntr = (mlib_s32 *)mlib_ImageGetData(src), *PixelPntr;
	mlib_s32 i, j;
	mlib_s64 Accum0, Accum1, Accum2, Accum3;

	for (i = 0; i <= Width - 4; i += 4) {
		Accum0 = Accum1 = Accum2 = Accum3 = 0;
		PixelPntr = LinePntr + i;

		for (j = 0; j < Height; j++) {
			Accum0 += *PixelPntr;
			Accum1 += *(PixelPntr + 1);
			Accum2 += *(PixelPntr + 2);
			Accum3 += *(PixelPntr + 3);
			PixelPntr += YStride;
		}

		xproj[i] = Accum0;
		xproj[i + 1] = Accum1;
		xproj[i + 2] = Accum2;
		xproj[i + 3] = Accum3;
	}

	for (; i < Width; i++) {
		Accum0 = 0;
		PixelPntr = LinePntr + i;

		for (j = 0; j < Height; j++) {
			Accum0 += *(PixelPntr);
			PixelPntr += YStride;
		}

		xproj[i] = Accum0;
	}
}

/* *********************************************************** */
#else /* _NO_LONGLONG */
/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageXProj = __mlib_ImageXProj

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_ImageXProj) mlib_ImageXProj
    __attribute__((weak, alias("__mlib_ImageXProj")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	MASK_00ff00ff	0x00FF00FF
#define	MASK_0000ffff	0x0000FFFF

/* *********************************************************** */

static void mlib_c_ImageXProj_U8(
    const mlib_image *src,
    mlib_d64 *xproj);

static void mlib_c_ImageXProj_U16(
    const mlib_image *src,
    mlib_d64 *xproj);

static void mlib_c_ImageXProj_S16(
    const mlib_image *src,
    mlib_d64 *xproj);

static void mlib_c_ImageXProj_S32(
    const mlib_image *src,
    mlib_d64 *xproj);

/* *********************************************************** */

mlib_status
__mlib_ImageXProj(
    mlib_d64 *xproj,
    const mlib_image *src)
{
/* check for obvious errors */
	MLIB_IMAGE_CHECK(src);
	MLIB_IMAGE_HAVE_CHAN(src, 1);

	if (xproj == NULL)
		return (MLIB_NULLPOINTER);

	switch (mlib_ImageGetType(src)) {
/* handle MLIB_BYTE data type of image */
	case MLIB_BYTE:
		mlib_c_ImageXProj_U8(src, xproj);
		break;

/* handle MLIB_USHORT data type of image */
	case MLIB_USHORT:
		mlib_c_ImageXProj_U16(src, xproj);
		break;

/* handle MLIB_SHORT data type of image */
	case MLIB_SHORT:
		mlib_c_ImageXProj_S16(src, xproj);
		break;

/* handle MLIB_INT data type of image */
	case MLIB_INT:
		mlib_c_ImageXProj_S32(src, xproj);
		break;

/* discard any other data types */
	default:
		return (MLIB_FAILURE);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

void
mlib_c_ImageXProj_U8(
    const mlib_image *src,
    mlib_d64 *xproj)
{
	mlib_u8 *sp = mlib_ImageGetData(src), *spl;
	mlib_s32 width = mlib_ImageGetWidth(src),
	    height = mlib_ImageGetHeight(src),
	    stride = mlib_ImageGetStride(src),
	    i = 0, j, k, accm, accm0, accm1, offset = (4 - (mlib_addr)sp) & 3;

	if (!(stride & 3) && width > 3) {

		for (; i < offset; i++) {
			spl = sp + i;
			accm = 0;

			for (k = 0; k < height; k++, spl += stride) {
				accm += *spl;
			}

			xproj[i] = (mlib_d64)accm;
		}

		for (; i <= width - 4; i += 4) {
			accm0 = 0;
			accm1 = 0;
			spl = sp + i;

			xproj[i] = 0;
			xproj[i + 1] = 0;
			xproj[i + 2] = 0;
			xproj[i + 3] = 0;

			for (j = 0; j < height >> 8; j++) {

				for (k = 0; k < 256; k++, spl += stride) {
					accm0 +=
					    *(mlib_u32 *)spl & MASK_00ff00ff;
					accm1 +=
					    (*(mlib_u32 *)spl >> 8) &
					    MASK_00ff00ff;
				}

#ifdef _LITTLE_ENDIAN
				xproj[i + 3] +=
				    (mlib_d64)((accm1 >> 16) & MASK_0000ffff);
				xproj[i + 2] +=
				    (mlib_d64)((accm0 >> 16) & MASK_0000ffff);
				xproj[i + 1] +=
				    (mlib_d64)(accm1 & MASK_0000ffff);
				xproj[i] += (mlib_d64)(accm0 & MASK_0000ffff);
#else /* _LITTLE_ENDIAN */
				xproj[i] +=
				    (mlib_d64)((accm1 >> 16) & MASK_0000ffff);
				xproj[i + 1] +=
				    (mlib_d64)((accm0 >> 16) & MASK_0000ffff);
				xproj[i + 2] +=
				    (mlib_d64)(accm1 & MASK_0000ffff);
				xproj[i + 3] +=
				    (mlib_d64)(accm0 & MASK_0000ffff);
#endif /* _LITTLE_ENDIAN */
				accm0 = 0;
				accm1 = 0;
			}

			for (j <<= 8; j < height; j++, spl += stride) {
				accm0 += *(mlib_u32 *)spl & MASK_00ff00ff;
				accm1 +=
				    (*(mlib_u32 *)spl >> 8) & MASK_00ff00ff;
			}

#ifdef _LITTLE_ENDIAN
			xproj[i + 3] +=
			    (mlib_d64)((accm1 >> 16) & MASK_0000ffff);
			xproj[i + 2] +=
			    (mlib_d64)((accm0 >> 16) & MASK_0000ffff);
			xproj[i + 1] += (mlib_d64)(accm1 & MASK_0000ffff);
			xproj[i] += (mlib_d64)(accm0 & MASK_0000ffff);
#else /* _LITTLE_ENDIAN */
			xproj[i] += (mlib_d64)((accm1 >> 16) & MASK_0000ffff);
			xproj[i + 1] +=
			    (mlib_d64)((accm0 >> 16) & MASK_0000ffff);
			xproj[i + 2] += (mlib_d64)(accm1 & MASK_0000ffff);
			xproj[i + 3] += (mlib_d64)(accm0 & MASK_0000ffff);
#endif /* _LITTLE_ENDIAN */
		}
	}

	for (; i < width; i++) {
		accm = 0;
		for (j = 0; j < height; j++)
			accm += sp[j * stride + i];
		xproj[i] = (mlib_d64)accm;
	}
}

/* *********************************************************** */

void
mlib_c_ImageXProj_U16(
    const mlib_image *src,
    mlib_d64 *xproj)
{
/* width of source image */
	mlib_s32 Width = mlib_ImageGetWidth(src),
/* height of source image */
	    Height = mlib_ImageGetHeight(src),
/* bytes to next row */
	    YStride = mlib_ImageGetStride(src) >> 1;

/* pointers to the data of image */
	mlib_u16 *LinePntr = (mlib_u16 *)mlib_ImageGetData(src), *PixelPntr;
	mlib_s32 Accum0, Accum1, Accum2, Accum3, i, j;

	for (i = 0; i <= Width - 4; i += 4) {
		Accum0 = Accum1 = Accum2 = Accum3 = 0;
		PixelPntr = LinePntr + i;

		for (j = 0; j < Height; j++) {
			Accum0 += *PixelPntr;
			Accum1 += *(PixelPntr + 1);
			Accum2 += *(PixelPntr + 2);
			Accum3 += *(PixelPntr + 3);
			PixelPntr += YStride;
		}

		xproj[i] = (mlib_d64)Accum0;
		xproj[i + 1] = (mlib_d64)Accum1;
		xproj[i + 2] = (mlib_d64)Accum2;
		xproj[i + 3] = (mlib_d64)Accum3;
	}

	for (; i < Width; i++) {
		Accum0 = 0;
		PixelPntr = LinePntr + i;

		for (j = 0; j < Height; j++) {
			Accum0 += *(PixelPntr);
			PixelPntr += YStride;
		}

		xproj[i] = (mlib_d64)Accum0;
	}
}

/* *********************************************************** */

void
mlib_c_ImageXProj_S16(
    const mlib_image *src,
    mlib_d64 *xproj)
{
/* width of source image */
	mlib_s32 Width = mlib_ImageGetWidth(src),
/* height of source image */
	    Height = mlib_ImageGetHeight(src),
/* bytes to next row */
	    YStride = mlib_ImageGetStride(src) >> 1;

/* pointers to the data of image */
	mlib_s16 *LinePntr = (mlib_s16 *)mlib_ImageGetData(src), *PixelPntr;
	mlib_s32 Accum0, Accum1, Accum2, Accum3, i, j;

	for (i = 0; i <= Width - 4; i += 4) {
		Accum0 = Accum1 = Accum2 = Accum3 = 0;
		PixelPntr = LinePntr + i;

		for (j = 0; j < Height; j++) {
			Accum0 += *PixelPntr;
			Accum1 += *(PixelPntr + 1);
			Accum2 += *(PixelPntr + 2);
			Accum3 += *(PixelPntr + 3);
			PixelPntr += YStride;
		}

		xproj[i] = (mlib_d64)Accum0;
		xproj[i + 1] = (mlib_d64)Accum1;
		xproj[i + 2] = (mlib_d64)Accum2;
		xproj[i + 3] = (mlib_d64)Accum3;
	}

	for (; i < Width; i++) {
		Accum0 = 0;
		PixelPntr = LinePntr + i;

		for (j = 0; j < Height; j++) {
			Accum0 += *(PixelPntr);
			PixelPntr += YStride;
		}

		xproj[i] = (mlib_d64)Accum0;
	}
}

/* *********************************************************** */

void
mlib_c_ImageXProj_S32(
    const mlib_image *src,
    mlib_d64 *xproj)
{
/* width of source image */
	mlib_s32 Width = mlib_ImageGetWidth(src),
/* height of source image */
	    Height = mlib_ImageGetHeight(src),
/* bytes to next row */
	    YStride = mlib_ImageGetStride(src) >> 2;

/* pointers to the data of image */
	mlib_s32 *LinePntr = (mlib_s32 *)mlib_ImageGetData(src), *PixelPntr;
	mlib_s32 i, j;
	mlib_d64 Accum0, Accum1, Accum2, Accum3;

	for (i = 0; i <= Width - 4; i += 4) {
		Accum0 = Accum1 = Accum2 = Accum3 = 0;
		PixelPntr = LinePntr + i;

		for (j = 0; j < Height; j++) {
			Accum0 += *PixelPntr;
			Accum1 += *(PixelPntr + 1);
			Accum2 += *(PixelPntr + 2);
			Accum3 += *(PixelPntr + 3);
			PixelPntr += YStride;
		}

		xproj[i] = Accum0;
		xproj[i + 1] = Accum1;
		xproj[i + 2] = Accum2;
		xproj[i + 3] = Accum3;
	}

	for (; i < Width; i++) {
		Accum0 = 0;
		PixelPntr = LinePntr + i;

		for (j = 0; j < Height; j++) {
			Accum0 += *(PixelPntr);
			PixelPntr += YStride;
		}

		xproj[i] = Accum0;
	}
}
/* *********************************************************** */
#endif /* end of ifndef _NO_LONGLONG */
/* *********************************************************** */
