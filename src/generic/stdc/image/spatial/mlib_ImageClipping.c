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

#pragma ident	"@(#)mlib_ImageClipping.c	9.2	07/10/09 SMI"

/*
 * FUNCTION
 *      mlib_ImageClipping
 *      mlib_ImageClippingMxN
 *
 *      The mapping of source image to destination image is left/top corner to
 *      left/top corner, but with a shift of the destination image if it's
 *      smaller than the source image.
 *      The exact mapping formula is
 *           xs = xd + x_offset
 *           ys = yd + y_offset
 *      where
 *           x_offset = (dst_width >= src_width) ? 0 : (kernel_size - 1)/2
 *           y_offset = (dst_height >= src_height) ? 0 : (kernel_size - 1)/2
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageClipping(mlib_image       *dst_i,
 *                                     mlib_image       *src_i,
 *                                     mlib_image       *dst_e,
 *                                     mlib_image       *src_e,
 *                                     mlib_s32         *edg_sizes,
 *                                     const mlib_image *dst,
 *                                     const mlib_image *src,
 *                                     mlib_s32         ker_size)
 *
 *      mlib_status mlib_ImageClippingMxN(mlib_image       *dst_i,
 *                                        mlib_image       *src_i,
 *                                        mlib_image       *dst_e,
 *                                        mlib_image       *src_e,
 *                                        mlib_s32         *edg_sizes,
 *                                        const mlib_image *dst,
 *                                        const mlib_image *src,
 *                                        mlib_s32         kw,
 *                                        mlib_s32         kh,
 *                                        mlib_s32         kw1,
 *                                        mlib_s32         kh1)
 *
 * OUTPUT ARGUMENTS
 *      dst_i     Pointer to destination image of internal pixels
 *      src_i     Pointer to source image of internal pixels
 *      dst_e     Pointer to destination image for edge processing
 *      src_e     Pointer to source image for edge processing
 *      edg_sizes Array of edge sizes
 *
 * INPUT ARGUMENTS
 *      dst       Pointer to destination image.
 *      src       Pointer to source image.
 *      ksize     Size of kernel
 *
 * RESTRICTION
 *      The src and the dst must be images of the same type.
 *      The src and dst must have same number of channels.
 *
 */

#include <mlib_image.h>
#include <mlib_ImageCheck.h>
#include <mlib_ImageClipping.h>
#include <mlib_ImageCreate.h>

/* *********************************************************** */

mlib_status
mlib_ImageClippingMxN(
    mlib_image *dst_i,
    mlib_image *src_i,
    mlib_image *dst_e,
    mlib_image *src_e,
    mlib_s32 *edg_sizes,
    const mlib_image *dst,
    const mlib_image *src,
    mlib_s32 kw,
    mlib_s32 kh,
    mlib_s32 kw1,
    mlib_s32 kh1)
{
	mlib_s32 kw2 = kw - 1 - kw1;
	mlib_s32 kh2 = kh - 1 - kh1;
	mlib_s32 src_wid, src_hgt, dst_wid, dst_hgt;
	mlib_s32 dx, dy, dxd, dxs, dyd, dys, wid_e, hgt_e;
	mlib_s32 dx_l, dx_r, dy_t, dy_b, wid_i, hgt_i;

	MLIB_IMAGE_CHECK(dst);
	MLIB_IMAGE_CHECK(src);
	MLIB_IMAGE_TYPE_EQUAL(dst, src);
	MLIB_IMAGE_CHAN_EQUAL(dst, src);

	dst_wid = mlib_ImageGetWidth(dst);
	dst_hgt = mlib_ImageGetHeight(dst);
	src_wid = mlib_ImageGetWidth(src);
	src_hgt = mlib_ImageGetHeight(src);

	dxd = dyd = 0;

	if (dst_wid >= src_wid) {
		dx_l = kw1;
		dx_r = kw2;
		dxs = 0;
	} else {
		dx_l = 0;
		dx_r = dst_wid - (src_wid - kw1 - kw2);
		if (dx_r < 0)
			dx_r = 0;
		dxs = kw1;
	}

	if (dst_hgt >= src_hgt) {
		dy_t = kh1;
		dy_b = kh2;
		dys = 0;
	} else {
		dy_t = 0;
		dy_b = dst_hgt - (src_hgt - kh1 - kh2);
		if (dy_b < 0)
			dy_b = 0;
		dys = kh1;
	}

/* image sizes */
	wid_e = (src_wid < dst_wid) ? src_wid : dst_wid;
	hgt_e = (src_hgt < dst_hgt) ? src_hgt : dst_hgt;
	if (dx_r > kw2) {
		wid_e -= (dx_r - kw2);
		if (wid_e < 0) wid_e = 0;
		dx_r = kw2;
	}
	if (dy_b > kh2) {
		hgt_e -= (dy_b - kh2);
		if (hgt_e < 0) hgt_e = 0;
		dy_b = kh2;
	}
	wid_i = wid_e + (kw1 - dx_l) + (kw2 - dx_r);
	hgt_i = hgt_e + (kh1 - dy_t) + (kh2 - dy_b);
	mlib_ImageSetSubimage(dst_i, dst, dxd - (kw1 - dx_l),
	    dyd - (kh1 - dy_t), wid_i, hgt_i);
	mlib_ImageSetSubimage(src_i, src, dxs - (kw1 - dx_l),
	    dys - (kh1 - dy_t), wid_i, hgt_i);

/* images for edge processing */
	if (dst_e != NULL && src_e != NULL) {
		mlib_ImageSetSubimage(dst_e, dst, dxd, dyd, wid_e, hgt_e);
		mlib_ImageSetSubimage(src_e, src, dxs, dys, wid_e, hgt_e);
	}

/* save edges */
	if (edg_sizes != NULL) {
		edg_sizes[0] = dx_l;
		edg_sizes[1] = dx_r;
		edg_sizes[2] = dy_t;
		edg_sizes[3] = dy_b;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_ImageClipping(
    mlib_image *dst_i,
    mlib_image *src_i,
    mlib_image *dst_e,
    mlib_image *src_e,
    mlib_s32 *edg_sizes,
    const mlib_image *dst,
    const mlib_image *src,
    mlib_s32 ker_size)
{
	mlib_s32 kw1 = (ker_size - 1) / 2;

	return mlib_ImageClippingMxN(dst_i, src_i, dst_e, src_e, edg_sizes,
	    dst, src, ker_size, ker_size, kw1, kw1);
}

/* *********************************************************** */
