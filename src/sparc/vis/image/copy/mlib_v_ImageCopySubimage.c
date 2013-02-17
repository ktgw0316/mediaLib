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

#pragma ident	"@(#)mlib_v_ImageCopySubimage.c	9.2	07/11/05 SMI"

/*
 * FUNCTIONS
 *      mlib_ImageCopySubimage      - Copy a sub-image of a larger image
 *                                    into a sub-region of another image.
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageCopySubimage(mlib_image       *dst,
 *                                         const mlib_image *src,
 *                                         mlib_s32         xd,
 *                                         mlib_s32         yd,
 *                                         mlib_s32         xs,
 *                                         mlib_s32         ys,
 *                                         mlib_s32         w,
 *                                         mlib_s32         h);
 *
 * ARGUMENT
 *    dst     pointer to output or destination image
 *    src     pointer to input or source image
 *    xd      x coordinate of origin in the destination
 *    yd      y coordinate of origin in the destination
 *    xs      x coordinate of origin in the source
 *    ys      y coordinate of origin in the source
 *    w       width of the sub-image
 *    h       height of the sub-image
 *
 * RESTRICTION
 *    src and dst must have the same type and number of channels.
 *    They can have 1, 2, 3 or 4 channels of MLIB_BIT, MLIB_BYTE,
 *    MLIB_SHORT, MLIB_USHORT, MLIB_INT, MLIB_FLOAT or MLIB_DOUBLE data type.
 *
 * DESCRIPTION
 *    Copy a sub-image of a larger image into a sub-region of another
 *    image.
 */

#include <mlib_image.h>
#include <mlib_ImageCheck.h>
#include <mlib_ImageCopy.h>
#include <mlib_v_ImageCopy_f.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageCopySubimage = __mlib_ImageCopySubimage

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_ImageCopySubimage) mlib_ImageCopySubimage
    __attribute__((weak, alias("__mlib_ImageCopySubimage")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

mlib_status
__mlib_ImageCopySubimage(
    mlib_image *dst,
    const mlib_image *src,
    mlib_s32 xd,
    mlib_s32 yd,
    mlib_s32 xs,
    mlib_s32 ys,
    mlib_s32 w,
    mlib_s32 h)
{
	mlib_s32 src_width, src_height, dst_width, dst_height;

/* start point in source */
	mlib_u8 *sa;

/* start points in destination */
	mlib_u8 *da;

/* number of channels of src and dst */
	mlib_s32 channels;

/* stride in bytes in src */
	mlib_s32 stride;

/* stride in bytes in dst */
	mlib_s32 dstride;

/* bit offset of src */
	mlib_s32 s_offset;

/* bit offset of dst */
	mlib_s32 d_offset;

/* indices for x, y */
	mlib_s32 j;
	mlib_s32 size, offset;

	MLIB_IMAGE_CHECK(src);
	MLIB_IMAGE_CHECK(dst);
	MLIB_IMAGE_TYPE_EQUAL(src, dst);
	MLIB_IMAGE_CHAN_EQUAL(src, dst);

	src_width = mlib_ImageGetWidth(src);
	src_height = mlib_ImageGetHeight(src);
	dst_width = mlib_ImageGetWidth(dst);
	dst_height = mlib_ImageGetHeight(dst);

	if ((w <= 0) || (h <= 0) ||
	    ((xs + w) <= 0) || ((ys + h) <= 0) ||
	    (xs >= src_width) || (ys >= src_height)) {
		return (MLIB_OUTOFRANGE);
	}

	if (((xd + w) <= 0) || ((yd + h) <= 0) ||
	    (xd >= dst_width) || (yd >= dst_height)) {
		return (MLIB_SUCCESS);
	} else {
		if (xs < 0) {
			w += xs;
			xd -= xs;
			xs = 0;
		}

		if (ys < 0) {
			h += ys;
			yd -= ys;
			ys = 0;
		}

		if (xd < 0) {
			w += xd;
			xs -= xd;
			xd = 0;
		}

		if (yd < 0) {
			h += yd;
			ys -= yd;
			yd = 0;
		}

		if ((xs + w) > src_width) {
			w = src_width - xs;

			if (w <= 0)
				return (MLIB_SUCCESS);
		}

		if ((ys + h) > src_height) {
			h = src_height - ys;

			if (h <= 0)
				return (MLIB_SUCCESS);
		}

		if ((xd + w) > dst_width) {
			w = dst_width - xd;

			if (w <= 0)
				return (MLIB_SUCCESS);
		}

		if ((yd + h) > dst_height) {
			h = dst_height - yd;

			if (h <= 0)
				return (MLIB_SUCCESS);
		}
	}

	channels = mlib_ImageGetChannels(dst);

	switch (mlib_ImageGetType(src)) {
	case MLIB_BIT:
/* in byte */
		stride = mlib_ImageGetStride(src);
/* in byte */
		dstride = mlib_ImageGetStride(dst);
/* in bits */
		s_offset = mlib_ImageGetBitOffset(src);
/* in bits */
		d_offset = mlib_ImageGetBitOffset(dst);
		offset = xs * channels + s_offset;
		sa = (mlib_u8 *)mlib_ImageGetData(src) + ys * stride +
		    (offset >> 3);
		s_offset = offset & 7;
		offset = xd * channels + d_offset;
		da = (mlib_u8 *)mlib_ImageGetData(dst) + yd * dstride +
		    (offset >> 3);
		d_offset = offset & 7;

		size = w * channels;
/* row loop */

		if (s_offset == d_offset) {
			for (j = 0; j < h; j++) {
				mlib_ImageCopy_bit_al(sa, da, size, s_offset);
				sa += stride;
				da += dstride;
			}
		} else {
			for (j = 0; j < h; j++) {
				mlib_ImageCopy_bit_na(sa, da, size, s_offset,
				    d_offset);
				sa += stride;
				da += dstride;
			}
		}

		return (MLIB_SUCCESS);
	case MLIB_BYTE:
		break;
	case MLIB_SHORT:
	case MLIB_USHORT:
		channels *= 2;
		break;
	case MLIB_INT:
	case MLIB_FLOAT:
		channels *= 4;
		break;
	case MLIB_DOUBLE:
		channels *= 8;
		break;
	default:
		return (MLIB_FAILURE);
	}

/* in byte */
	stride = mlib_ImageGetStride(src);
/* in byte */
	dstride = mlib_ImageGetStride(dst);
	sa = (mlib_u8 *)mlib_ImageGetData(src) + ys * stride + xs * channels;
	da = (mlib_u8 *)mlib_ImageGetData(dst) + yd * dstride + xd * channels;

/* in byte */
	size = w * channels;

	if ((size & 7 == 0) && !mlib_ImageIsNotStride8X(src) &&
	    !mlib_ImageIsNotStride8X(dst) &&
	    (((mlib_addr)sa & 7) == 0) && (((mlib_addr)da & 7) == 0)) {

		stride >>= 3;
		dstride >>= 3;
		size >>= 3;
		mlib_v_ImageCopy_a2((mlib_d64 *)sa, (mlib_d64 *)da, size, h,
		    stride, dstride);
	} else {

/* row loop */
		for (j = 0; j < h; j++) {
			mlib_ImageCopy_na(sa, da, size);
			sa += stride;
			da += dstride;
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
