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
 * Copyright 2006 Sun Microsystems, Inc.  All rights reserved.
 * Use is subject to license terms.
 */

#pragma ident   "@(#)mlib_s_ImageSConv3x3_Fp.c	9.2	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_ImageSConv3x3_Fp - separable convolve of an image with a 3x3 kernel
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageSConv3x3_Fp(mlib_image       *dst,
 *                                        const mlib_image *src,
 *                                        const mlib_d64   *hkernel,
 *                                        const mlib_d64   *vkernel,
 *                                        mlib_s32         cmask,
 *                                        mlib_edge        edge)
 *
 * ARGUMENT
 *      dst       Pointer to the destination image data structure
 *      src       Pointer to the source image data structure
 *      hkernel   Pointer to 3 elements of the horizontal kernel
 *      vkernel   Pointer to 3 elements of the vertical kernel
 *      cmask     Channel mask to indicate the channels to be convolved.
 *                Each bit of which represents a channel in the image. The
 *                channels corresponded to 1 bits are those to be processed.
 *      edge      Type of edge condition.
 *
 * DESCRIPTION
 *      2-D separable convolution, 3x3 kernel.
 *
 *      The unselected channels are not overwritten. If both src and dst have
 *      just one channel, cmask is ignored.
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
 *      The edge condition can be one of the following:
 *              MLIB_EDGE_DST_NO_WRITE  (default)
 *              MLIB_EDGE_DST_FILL_ZERO
 *              MLIB_EDGE_DST_COPY_SRC
 *              MLIB_EDGE_SRC_EXTEND
 *
 * RESTRICTION
 *      The src and the dst must be the same type and have same number
 *      of channels (1, 2, 3, or 4).
 */

#include <mlib_image.h>
#include <mlib_ImageCreate.h>
#include <mlib_ImageCheck.h>
#include <mlib_ImageClipping.h>
#include <mlib_ImageConvEdge.h>
#include <mlib_ImageSConv.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageSConv3x3_Fp = __mlib_ImageSConv3x3_Fp

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_ImageSConv3x3_Fp) mlib_ImageSConv3x3_Fp
    __attribute__((weak, alias("__mlib_ImageSConv3x3_Fp")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */
    mlib_status mlib_s_sconv3x3_f32nw_1(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_d64 *hkernel,
	const mlib_d64 *vkernel);

    mlib_status mlib_s_sconv3x3_f32nw_2(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_d64 *hkernel,
	const mlib_d64 *vkernel);

    mlib_status mlib_s_sconv3x3_f32nw_3(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_d64 *hkernel,
	const mlib_d64 *vkernel);

    mlib_status mlib_s_sconv3x3_f32nw_4(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_d64 *hkernel,
	const mlib_d64 *vkernel);

/* *********************************************************** */
    mlib_status mlib_s_sconv3x3_d64nw_1(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_d64 *hkernel,
	const mlib_d64 *vkernel);

    mlib_status mlib_s_sconv3x3_d64nw_2(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_d64 *hkernel,
	const mlib_d64 *vkernel);

    mlib_status mlib_s_sconv3x3_d64nw_3(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_d64 *hkernel,
	const mlib_d64 *vkernel);

    mlib_status mlib_s_sconv3x3_d64nw_4(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_d64 *hkernel,
	const mlib_d64 *vkernel);

/* *********************************************************** */
    mlib_status mlib_s_sconv3x3_f32ext_1(
    mlib_image *dst,
    const mlib_image *src,
	mlib_s32 dx_l,
	mlib_s32 dx_r,
	mlib_s32 dy_t,
	mlib_s32 dy_b,
    const mlib_d64 *hkernel,
	const mlib_d64 *vkernel);

    mlib_status mlib_s_sconv3x3_f32ext_2(
    mlib_image *dst,
    const mlib_image *src,
	mlib_s32 dx_l,
	mlib_s32 dx_r,
	mlib_s32 dy_t,
	mlib_s32 dy_b,
    const mlib_d64 *hkernel,
	const mlib_d64 *vkernel);

    mlib_status mlib_s_sconv3x3_f32ext_3(
    mlib_image *dst,
    const mlib_image *src,
	mlib_s32 dx_l,
	mlib_s32 dx_r,
	mlib_s32 dy_t,
	mlib_s32 dy_b,
    const mlib_d64 *hkernel,
	const mlib_d64 *vkernel);

    mlib_status mlib_s_sconv3x3_f32ext_4(
    mlib_image *dst,
    const mlib_image *src,
	mlib_s32 dx_l,
	mlib_s32 dx_r,
	mlib_s32 dy_t,
	mlib_s32 dy_b,
    const mlib_d64 *hkernel,
	const mlib_d64 *vkernel);

/* *********************************************************** */
    mlib_status mlib_s_sconv3x3_d64ext_1(
    mlib_image *dst,
    const mlib_image *src,
	mlib_s32 dx_l,
	mlib_s32 dx_r,
	mlib_s32 dy_t,
	mlib_s32 dy_b,
    const mlib_d64 *hkernel,
	const mlib_d64 *vkernel);

    mlib_status mlib_s_sconv3x3_d64ext_2(
    mlib_image *dst,
    const mlib_image *src,
	mlib_s32 dx_l,
	mlib_s32 dx_r,
	mlib_s32 dy_t,
	mlib_s32 dy_b,
    const mlib_d64 *hkernel,
	const mlib_d64 *vkernel);

    mlib_status mlib_s_sconv3x3_d64ext_3(
    mlib_image *dst,
    const mlib_image *src,
	mlib_s32 dx_l,
	mlib_s32 dx_r,
	mlib_s32 dy_t,
	mlib_s32 dy_b,
    const mlib_d64 *hkernel,
	const mlib_d64 *vkernel);

    mlib_status mlib_s_sconv3x3_d64ext_4(
    mlib_image *dst,
    const mlib_image *src,
	mlib_s32 dx_l,
	mlib_s32 dx_r,
	mlib_s32 dy_t,
	mlib_s32 dy_b,
    const mlib_d64 *hkernel,
	const mlib_d64 *vkernel);


/* *********************************************************** */

    mlib_status mlib_s_sconv3x3nw_f32(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_d64 *hkernel,
	const mlib_d64 *vkernel,
    mlib_s32 cmask)
{
	mlib_s32 nchan = mlib_ImageGetChannels(src);
	mlib_s32 amask = (1 << nchan) - 1;

	if (nchan == 1)
		cmask = 1;
	if ((cmask & amask) != amask) {
		return (mlib_sconv3x3nw_f32(dst, src, hkernel, vkernel, cmask));
	}

	switch (nchan) {
	case 1:
		return (mlib_s_sconv3x3_f32nw_1(dst, src, hkernel, vkernel));
	case 2:
		return (mlib_s_sconv3x3_f32nw_2(dst, src, hkernel, vkernel));
	case 3:
		return (mlib_s_sconv3x3_f32nw_3(dst, src, hkernel, vkernel));
	case 4:
		return (mlib_s_sconv3x3_f32nw_4(dst, src, hkernel, vkernel));
	}

	return (MLIB_FAILURE);
}
/* *********************************************************** */

    mlib_status mlib_s_sconv3x3ext_f32(
	mlib_image *dst,
	const mlib_image *src,
	mlib_s32 dx_l,
	mlib_s32 dx_r,
	mlib_s32 dy_t,
	mlib_s32 dy_b,
	const mlib_d64 *hkernel,
	const mlib_d64 *vkernel,
	mlib_s32 cmask)
{
	mlib_s32 nchan = mlib_ImageGetChannels(src);
	mlib_s32 amask = (1 << nchan) - 1;

	if (nchan == 1)
		cmask = 1;
	if ((cmask & amask) != amask) {
		return (mlib_sconv3x3ext_f32(dst, src, dx_l, dx_r,
			dy_t, dy_b, hkernel, vkernel, cmask));
	}


	switch (nchan) {
	case 1:
		return (mlib_s_sconv3x3_f32ext_1(dst, src,
			dx_l, dx_r, dy_t, dy_b, (void *)hkernel, vkernel));
	case 2:
		return (mlib_s_sconv3x3_f32ext_2(dst, src,
			dx_l, dx_r, dy_t, dy_b, (void *)hkernel, vkernel));
	case 3:
		return (mlib_s_sconv3x3_f32ext_3(dst, src,
			dx_l, dx_r, dy_t, dy_b, (void *)hkernel, vkernel));
	case 4:
		return (mlib_s_sconv3x3_f32ext_4(dst, src,
			dx_l, dx_r, dy_t, dy_b, (void *)hkernel, vkernel));
	}

	return (MLIB_FAILURE);
}

/* *********************************************************** */

    mlib_status mlib_s_sconv3x3nw_d64(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_d64 *hkernel,
	const mlib_d64 *vkernel,
    mlib_s32 cmask)
{
	mlib_s32 nchan = mlib_ImageGetChannels(src);
	mlib_s32 amask = (1 << nchan) - 1;

	if (nchan == 1)
		cmask = 1;
	if ((cmask & amask) != amask) {
		return (mlib_sconv3x3nw_d64(dst, src, hkernel, vkernel, cmask));
	}

	switch (nchan) {
	case 1:
		return (mlib_s_sconv3x3_d64nw_1(dst, src, hkernel, vkernel));
	case 2:
		return (mlib_s_sconv3x3_d64nw_2(dst, src, hkernel, vkernel));
	case 3:
		return (mlib_s_sconv3x3_d64nw_3(dst, src, hkernel, vkernel));
	case 4:
		return (mlib_s_sconv3x3_d64nw_4(dst, src, hkernel, vkernel));
	}

	return (MLIB_FAILURE);
}
/* *********************************************************** */

    mlib_status mlib_s_sconv3x3ext_d64(
	mlib_image *dst,
	const mlib_image *src,
	mlib_s32 dx_l,
	mlib_s32 dx_r,
	mlib_s32 dy_t,
	mlib_s32 dy_b,
	const mlib_d64 *hkernel,
	const mlib_d64 *vkernel,
	mlib_s32 cmask)
{
	mlib_s32 nchan = mlib_ImageGetChannels(src);
	mlib_s32 amask = (1 << nchan) - 1;

	if (nchan == 1)
		cmask = 1;
	if ((cmask & amask) != amask) {
		return (mlib_sconv3x3ext_d64(dst, src, dx_l, dx_r,
			dy_t, dy_b, hkernel, vkernel, cmask));
	}

	switch (nchan) {
	case 1:
		return (mlib_s_sconv3x3_d64ext_1(dst, src,
			dx_l, dx_r, dy_t, dy_b, (void *)hkernel, vkernel));
	case 2:
		return (mlib_s_sconv3x3_d64ext_2(dst, src,
			dx_l, dx_r, dy_t, dy_b, (void *)hkernel, vkernel));
	case 3:
		return (mlib_s_sconv3x3_d64ext_3(dst, src,
			dx_l, dx_r, dy_t, dy_b, (void *)hkernel, vkernel));
	case 4:
		return (mlib_s_sconv3x3_d64ext_4(dst, src,
			dx_l, dx_r, dy_t, dy_b, (void *)hkernel, vkernel));
	}

	return (MLIB_FAILURE);
}


/* *********************************************************** */

mlib_status
__mlib_ImageSConv3x3_Fp(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_d64 *hkernel,
    const mlib_d64 *vkernel,
    mlib_s32 cmask,
    mlib_edge edge)
{
	mlib_s32 ksize = 3, ksize2 = ksize / 2;
	mlib_image dst_i[1], src_i[1], dst_e[1], src_e[1];
	mlib_s32 edg_sizes[4];
	mlib_type type;
	mlib_s32 nchan, dx_l, dx_r, dy_t, dy_b;
	mlib_d64 zero[4] = { 0, 0, 0, 0 };
	mlib_status ret;

	ret =
	    mlib_ImageClipping(dst_i, src_i, dst_e, src_e, edg_sizes, dst, src,
	    ksize);

	if (ret != MLIB_SUCCESS)
		return (ret);

	if (hkernel == NULL || vkernel == NULL)
		return (MLIB_FAILURE);

	type = mlib_ImageGetType(dst);
	nchan = mlib_ImageGetChannels(dst);

	if (nchan == 1)
		cmask = 1;

	if ((cmask & ((1 << nchan) - 1)) == 0)
		return (MLIB_SUCCESS);

	dx_l = edg_sizes[0];
	dx_r = edg_sizes[1];
	dy_t = edg_sizes[2];
	dy_b = edg_sizes[3];

	if (dx_l + dx_r + dy_t + dy_b == 0)
		edge = MLIB_EDGE_DST_NO_WRITE;

	if (edge != MLIB_EDGE_SRC_EXTEND) {
		if (mlib_ImageGetWidth(src_i) >= ksize &&
		    mlib_ImageGetHeight(src_i) >= ksize)
			switch (type) {
			case MLIB_FLOAT:
				mlib_s_sconv3x3nw_f32(dst_i, src_i, hkernel,
				    vkernel, cmask);
				break;

			case MLIB_DOUBLE:
				mlib_s_sconv3x3nw_d64(dst_i, src_i, hkernel,
				    vkernel, cmask);
				break;

			default:
				return (MLIB_FAILURE);
			}

		switch (edge) {
		case MLIB_EDGE_DST_FILL_ZERO:
			mlib_ImageConvClearEdge_Fp(dst_e, dx_l, dx_r, dy_t,
			    dy_b, zero, cmask);
			break;
		case MLIB_EDGE_DST_COPY_SRC:
			mlib_ImageConvCopyEdge_Fp(dst_e, src_e, dx_l, dx_r,
			    dy_t, dy_b, cmask);
			break;
		default:
			return (MLIB_SUCCESS);
		}
	} else {
/* MLIB_EDGE_SRC_EXTEND */
/* adjust src_e image */
		mlib_ImageSetSubimage(src_e, src_e, dx_l - ksize2,
		    dy_t - ksize2, mlib_ImageGetWidth(src_e),
		    mlib_ImageGetHeight(src_e));

		switch (type) {
		case MLIB_FLOAT:
			mlib_s_sconv3x3ext_f32(dst_e, src_e, dx_l, dx_r, dy_t,
			    dy_b, hkernel, vkernel, cmask);
			break;
		case MLIB_DOUBLE:
			mlib_s_sconv3x3ext_d64(dst_e, src_e, dx_l, dx_r, dy_t,
			    dy_b, hkernel, vkernel, cmask);
			break;

		default:
			return (MLIB_FAILURE);
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
