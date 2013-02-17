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

#pragma ident	"@(#)mlib_v_ImageCopyArea.c	9.2	07/11/05 SMI"

/*
 * FUNCTIONS
 *      mlib_ImageCopyArea    - Copy a specified rectangular area from
 *                              one portion of the image to another.
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageCopyArea(mlib_image *img,
 *                                     mlib_s32   x,
 *                                     mlib_s32   y,
 *                                     mlib_s32   w,
 *                                     mlib_s32   h,
 *                                     mlib_s32   dx,
 *                                     mlib_s32   dy);
 *
 * ARGUMENT
 *      img     pointer to an image
 *      x       x coordinate of origin in the source
 *      y       y coordinate of origin in the source
 *      w       width of the area to be copied
 *      h       height of the area to be copied
 *      dx      horizontal displacement
 *      dy      vertical displacement
 *
 * RESTRICTION
 *      The image can have 1, 2, 3 or 4 channels of MLIB_BIT, MLIB_BYTE,
 *      MLIB_SHORT, MLIB_USHORT, MLIB_INT, MLIB_FLOAT or MLIB_DOUBLE data type.
 *
 * DESCRIPTION
 *      Copy a specified rectangular area from one portion of the image
 *      to another (within the same image).
 */

#include <mlib_image.h>
#include <mlib_ImageCheck.h>
#include <mlib_ImageCopy.h>
#include <mlib_v_ImageCopy_f.h>
#include <mlib_v_ImageCopyArea_f.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageCopyArea = __mlib_ImageCopyArea

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_ImageCopyArea) mlib_ImageCopyArea
    __attribute__((weak, alias("__mlib_ImageCopyArea")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

static void mlib_v_ImageCopyArea_f(
    mlib_image *img,
    mlib_s32 x,
    mlib_s32 y,
    mlib_s32 w,
    mlib_s32 h,
    mlib_s32 dx,
    mlib_s32 dy);

static void mlib_v_ImageCopyArea_r(
    mlib_image *img,
    mlib_s32 x,
    mlib_s32 y,
    mlib_s32 w,
    mlib_s32 h,
    mlib_s32 dx,
    mlib_s32 dy);

/* *********************************************************** */

mlib_status
__mlib_ImageCopyArea(
    mlib_image *img,
    mlib_s32 x,
    mlib_s32 y,
    mlib_s32 w,
    mlib_s32 h,
    mlib_s32 dx,
    mlib_s32 dy)
{
	mlib_s32 width, height;

/* start point in source */
	mlib_u8 *sa, *sp;

/* start points in destination */
	mlib_u8 *da;

/* number of channels of src and dst */
	mlib_s32 channels;

/* stride in bytes in src */
	mlib_s32 stride;

/* bit offset of src */
	mlib_s32 s_offset;

/* bit offset of dst */
	mlib_s32 d_offset;

/* indices for x, y */
	mlib_s32 j;
	mlib_s32 size, offset;

	MLIB_IMAGE_CHECK(img);

/* width of the image */
	width = mlib_ImageGetWidth(img);
/* height of the image */
	height = mlib_ImageGetHeight(img);

/* clip the src and dst with respect to the image */

	if ((w == 0) || (h == 0) ||
	    ((x + w) <= 0) || ((y + h) <= 0) ||
	    (x >= width) || (y >= height) ||
	    ((x + dx + w) <= 0) || ((y + dy + h) <= 0) ||
	    ((x + dx) >= width) || ((y + dy) >= height) ||
	    (dx >= width) || (dy >= height) || (dx <= -width) ||
	    (dy <= -height)) {
		return (MLIB_OUTOFRANGE);
	} else {
/* clip WRT src first */
		if (x < 0) {
			w += x;
			x = 0;
		}

		if (y < 0) {
			h += y;
			y = 0;
		}

		if ((x + w) > width) {
			w = width - x;
		}

		if ((y + h) > height) {
			h = height - y;
		}

/* then clip WRT dst */
		if ((x + dx) < 0) {
			w += (x + dx);
			x = -dx;
		}

		if ((y + dy) < 0) {
			h += (y + dy);
			y = -dy;
		}

		if ((x + dx + w) > width) {
			w = width - (x + dx);
		}

		if ((y + dy + h) > height) {
			h = height - (y + dy);
		}
	}

	if ((dx == 0) && (dy == 0)) {
		return (MLIB_SUCCESS);
	} else {
		switch (mlib_ImageGetType(img)) {
		case MLIB_BIT:
			channels = mlib_ImageGetChannels(img);
/* in byte */
			stride = mlib_ImageGetStride(img);
/* in bits */
			s_offset = mlib_ImageGetBitOffset(img);
			size = w * channels;
			sp = (mlib_u8 *)mlib_ImageGetData(img);

			if ((dy > 0) || ((dy == 0) && (dx > 0))) {
				offset = (x + w) * channels + s_offset - 1;
				sa = sp + (y + h - 1) * stride + (offset >> 3);
				s_offset = offset & 7;
				offset += dx * channels;
				da = sp + (y + h + dy - 1) * stride +
				    (offset >> 3);
				d_offset = offset & 7;

				if (s_offset == d_offset) {
					for (j = 0; j < h; j++) {
						mlib_ImageCopyArea_bit_al_r(sa,
						    da, size, s_offset + 1);
						sa -= stride;
						da -= stride;
					}
				} else {
					for (j = 0; j < h; j++) {
						mlib_ImageCopy_bit_na_r(sa, da,
						    size, s_offset + 1,
						    d_offset + 1);
						sa -= stride;
						da -= stride;
					}
				}
			} else {
				offset = x * channels + s_offset;
				sa = sp + y * stride + (offset >> 3);
				s_offset = offset & 7;
				offset += dx * channels;
				da = sp + (y + dy) * stride + (offset >> 3);
				d_offset = offset & 7;

				if (s_offset == d_offset) {
					for (j = 0; j < h; j++) {
						mlib_ImageCopy_bit_al(sa, da,
						    size, s_offset);
						sa += stride;
						da += stride;
					}
				} else {
					for (j = 0; j < h; j++) {
						mlib_ImageCopy_bit_na(sa, da,
						    size, s_offset, d_offset);
						sa += stride;
						da += stride;
					}
				}
			}

			return (MLIB_SUCCESS);
		case MLIB_BYTE:
			break;
		case MLIB_SHORT:
		case MLIB_USHORT:
			w *= 2;
			x *= 2;
			dx *= 2;
			break;
		case MLIB_INT:
		case MLIB_FLOAT:
			w *= 4;
			x *= 4;
			dx *= 4;
			break;
		case MLIB_DOUBLE:
			w *= 8;
			x *= 8;
			dx *= 8;
			break;
		default:
			return (MLIB_FAILURE);
		}

		if ((dy > 0) || ((dy == 0) && (dx > 0))) {
			mlib_v_ImageCopyArea_r(img, x, y, w, h, dx, dy);
		} else {
			mlib_v_ImageCopyArea_f(img, x, y, w, h, dx, dy);
		}

		return (MLIB_SUCCESS);
	}
}

/* *********************************************************** */

/*
 *        data are copied beginning from the start and proceeding
 *        to the end of the area.
 */

void
mlib_v_ImageCopyArea_f(
    mlib_image *img,
    mlib_s32 x,
    mlib_s32 y,
    mlib_s32 w,
    mlib_s32 h,
    mlib_s32 dx,
    mlib_s32 dy)
{
/* start point in source */
	mlib_u8 *sa;

/* start points in destination */
	mlib_u8 *da;

/* number of channels */
	mlib_s32 channels;

/* stride in bytes */
	mlib_s32 stride;

/* indices for x, y */
	mlib_s32 j;
	mlib_s32 size;

	channels = mlib_ImageGetChannels(img);
/* in byte */
	stride = mlib_ImageGetStride(img);
/* in byte */
	size = w * channels;
	sa = (mlib_u8 *)mlib_ImageGetData(img) + y * stride + x * channels;
	da = sa + dy * stride + dx * channels;

	if (((size & 7) == 0) && !mlib_ImageIsNotStride8X(img) &&
	    (((mlib_addr)sa & 7) == 0) && (((mlib_addr)da & 7) == 0)) {

		stride /= 8;
		size /= 8;
		mlib_v_ImageCopy_a2((mlib_d64 *)sa, (mlib_d64 *)da, size, h,
		    stride, stride);
	} else {

/* row loop */
		for (j = 0; j < h; j++) {
			mlib_ImageCopy_na(sa, da, size);
			sa += stride;
			da += stride;
		}
	}
}

/* *********************************************************** */

/*
 *        data are copied beginning from the end and proceeding
 *        to the start of the area.
 */

void
mlib_v_ImageCopyArea_r(
    mlib_image *img,
    mlib_s32 x,
    mlib_s32 y,
    mlib_s32 w,
    mlib_s32 h,
    mlib_s32 dx,
    mlib_s32 dy)
{
/* start point in source */
	mlib_u8 *sa;

/* start points in destination */
	mlib_u8 *da;

/* 8-byte aligned start point in source */
	mlib_d64 *sp;

/* 8-byte aligned start points in dst */
	mlib_d64 *dp;

/* number of channels */
	mlib_s32 channels;

/* stride in bytes */
	mlib_s32 stride;

/* indices for x, y */
	mlib_s32 j;
	mlib_s32 size;

	channels = mlib_ImageGetChannels(img);
/* in byte */
	stride = mlib_ImageGetStride(img);
/* in byte */
	size = w * channels;
	sa = (mlib_u8 *)mlib_ImageGetData(img) +
	    (y + h - 1) * stride + (x + w) * channels - 1;
	da = sa + dy * stride + dx * channels;

	if ((size % 8 == 0) && !mlib_ImageIsNotStride8X(img) &&
	    ((mlib_addr)sa % 8 == 7) && ((mlib_addr)da % 8 == 7)) {

		sp = (mlib_d64 *)((mlib_addr)sa - 7);
		dp = (mlib_d64 *)((mlib_addr)da - 7);
		stride /= 8;
		size /= 8;
		mlib_v_ImageCopyArea_a2_r(sp, dp, size, h, stride);
	} else {

/* row loop */
		for (j = 0; j < h; j++) {
			mlib_ImageCopyArea_na_r(sa, da, size);
			sa -= stride;
			da -= stride;
		}
	}
}

/* *********************************************************** */
