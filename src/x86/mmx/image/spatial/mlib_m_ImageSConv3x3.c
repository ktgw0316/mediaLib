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

#pragma ident	"@(#)mlib_m_ImageSConv3x3.c	9.4	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_ImageSConv3x3 - separable convolve of an image with a 3x3 kernel
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageSConv3x3(mlib_image *dst,
 *                                     const mlib_image *src,
 *                                     const mlib_s32   *hkernel,
 *                                     const mlib_s32   *vkernel,
 *                                     mlib_s32   scale,
 *                                     mlib_s32   cmask,
 *                                     mlib_edge  edge)
 *
 * ARGUMENT
 *      dst       Pointer to the destination image data structure
 *      src       Pointer to the source image data structure
 *      hkernel   Pointer to 3 elements of the horizontal kernel
 *      vkernel   Pointer to 3 elements of the vertical kernel
 *      scale     The integer coefficients are assumed to be equal
 *                to some floating point value < 1.0, multiplied by
 *                a scalefactor that is a power of 2. Values are
 *                divided by this scalefactor before being output.
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
 *      For data type MLIB_BYTE - 24 <= scale <= 31
 *      For data type MLIB_SHORT - 26 <= scale <= 33
 *      For data type MLIB_INT - scale >= 0.
 *
 *      The kernel coefficients used in function mlib_ImageSConv3x3
 *      has to be preprocessed in a special way.
 *      It is recommended to preprocess these coefficients
 *      using the mlib_ImageSConvKernelConvert function.
 */

#include <mlib_image.h>
#include <mlib_ImageConv.h>
#include <mlib_c_ImageConv.h>
#include <mlib_ImageClipping.h>
#include <mlib_ImageConvEdge.h>
#include <mlib_ImageCreate.h>
#include <mlib_ImageSConv.h>

/* *************************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageSConv3x3 = __mlib_ImageSConv3x3

#elif defined(__GNUC__)  /* defined(__SUNPRO_C) */

__typeof__(__mlib_ImageSConv3x3) mlib_ImageSConv3x3
    __attribute__((weak, alias("__mlib_ImageSConv3x3")));

#else /* defined(__SUNPRO_C) */

#error "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

    mlib_status mlib_m_sconv3x3_8nw_1(
    mlib_image *dst,
    mlib_image *src,
    mlib_s32 *hkernel,
    mlib_s32 *vkernel,
    mlib_s32 scalef_expon);

    mlib_status mlib_m_sconv3x3_8nw_2(
    mlib_image *dst,
    mlib_image *src,
    mlib_s32 *hkernel,
    mlib_s32 *vkernel,
    mlib_s32 scalef_expon);

    mlib_status mlib_m_sconv3x3_8nw_3(
    mlib_image *dst,
    mlib_image *src,
    mlib_s32 *hkernel,
    mlib_s32 *vkernel,
    mlib_s32 scalef_expon);

    mlib_status mlib_m_sconv3x3_8nw_4(
    mlib_image *dst,
    mlib_image *src,
    mlib_s32 *hkernel,
    mlib_s32 *vkernel,
    mlib_s32 scalef_expon);

/* *********************************************************** */

    mlib_status mlib_m_sconv3x3_16nw_1(
    mlib_image *dst,
    mlib_image *src,
    mlib_s32 *hkernel,
    mlib_s32 *vkernel,
    mlib_s32 scalef_expon);

    mlib_status mlib_m_sconv3x3_16nw_2(
    mlib_image *dst,
    mlib_image *src,
    mlib_s32 *hkernel,
    mlib_s32 *vkernel,
    mlib_s32 scalef_expon);

    mlib_status mlib_m_sconv3x3_16nw_3(
    mlib_image *dst,
    mlib_image *src,
    mlib_s32 *hkernel,
    mlib_s32 *vkernel,
    mlib_s32 scalef_expon);

    mlib_status mlib_m_sconv3x3_16nw_4(
    mlib_image *dst,
    mlib_image *src,
    mlib_s32 *hkernel,
    mlib_s32 *vkernel,
    mlib_s32 scalef_expon);

/* *********************************************************** */

    mlib_status mlib_m_sconv3x3_u16nw_1(
    mlib_image *dst,
    mlib_image *src,
    mlib_s32 *hkernel,
    mlib_s32 *vkernel,
    mlib_s32 scalef_expon);

    mlib_status mlib_m_sconv3x3_u16nw_2(
    mlib_image *dst,
    mlib_image *src,
    mlib_s32 *hkernel,
    mlib_s32 *vkernel,
    mlib_s32 scalef_expon);

    mlib_status mlib_m_sconv3x3_u16nw_3(
    mlib_image *dst,
    mlib_image *src,
    mlib_s32 *hkernel,
    mlib_s32 *vkernel,
    mlib_s32 scalef_expon);

    mlib_status mlib_m_sconv3x3_u16nw_4(
    mlib_image *dst,
    mlib_image *src,
    mlib_s32 *hkernel,
    mlib_s32 *vkernel,
    mlib_s32 scalef_expon);

/* *********************************************************** */

mlib_status
mlib_m_sconv3x3nw_u8(
    mlib_image *dst,
    mlib_image *src,
    mlib_s32 *hkernel,
    mlib_s32 *vkernel,
    mlib_s32 scalef_expon,
    mlib_s32 cmask)
{
	mlib_s32 nchan = mlib_ImageGetChannels(src);
	mlib_s32 amask = (1 << nchan) - 1;

	if (nchan == 1)
		cmask = 1;
	if ((cmask & amask) != amask || scalef_expon < 29) {
		return mlib_sconv3x3nw_u8(dst, src, hkernel, vkernel,
		    scalef_expon, cmask);
	}

	switch (nchan) {
	case 1:
		return mlib_m_sconv3x3_8nw_1(dst, src, hkernel, vkernel,
		    scalef_expon);
	case 2:
		return mlib_m_sconv3x3_8nw_2(dst, src, hkernel, vkernel,
		    scalef_expon);
	case 3:
		return mlib_m_sconv3x3_8nw_3(dst, src, hkernel, vkernel,
		    scalef_expon);
	case 4:
		return mlib_m_sconv3x3_8nw_4(dst, src, hkernel, vkernel,
		    scalef_expon);
	}

	return (MLIB_FAILURE);
}

/* *********************************************************** */

mlib_status
mlib_m_sconv3x3nw_s16(
    mlib_image *dst,
    mlib_image *src,
    mlib_s32 *hkernel,
    mlib_s32 *vkernel,
    mlib_s32 scalef_expon,
    mlib_s32 cmask)
{
	mlib_s32 nchan = mlib_ImageGetChannels(src);
	mlib_s32 amask = (1 << nchan) - 1;

	if (nchan == 1)
		cmask = 1;
	if ((cmask & amask) != amask) {
		return mlib_sconv3x3nw_s16(dst, src, hkernel, vkernel,
		    scalef_expon, cmask);
	}

	switch (nchan) {
	case 1:
		return mlib_m_sconv3x3_16nw_1(dst, src, hkernel, vkernel,
		    scalef_expon);
	case 2:
		return mlib_m_sconv3x3_16nw_2(dst, src, hkernel, vkernel,
		    scalef_expon);
	case 3:
		return mlib_m_sconv3x3_16nw_3(dst, src, hkernel, vkernel,
		    scalef_expon);
	case 4:
		return mlib_m_sconv3x3_16nw_4(dst, src, hkernel, vkernel,
		    scalef_expon);
	}

	return (MLIB_FAILURE);
}

/* *********************************************************** */

mlib_status
mlib_m_sconv3x3nw_u16(
    mlib_image *dst,
    mlib_image *src,
    mlib_s32 *hkernel,
    mlib_s32 *vkernel,
    mlib_s32 scalef_expon,
    mlib_s32 cmask)
{
	mlib_s32 nchan = mlib_ImageGetChannels(src);
	mlib_s32 amask = (1 << nchan) - 1;

	if (nchan == 1)
		cmask = 1;
	if ((cmask & amask) != amask) {
		return mlib_sconv3x3nw_u16(dst, src, hkernel, vkernel,
		    scalef_expon, cmask);
	}

	switch (nchan) {
	case 1:
		return mlib_m_sconv3x3_u16nw_1(dst, src, hkernel, vkernel,
		    scalef_expon);
	case 2:
		return mlib_m_sconv3x3_u16nw_2(dst, src, hkernel, vkernel,
		    scalef_expon);
	case 3:
		return mlib_m_sconv3x3_u16nw_3(dst, src, hkernel, vkernel,
		    scalef_expon);
	case 4:
		return mlib_m_sconv3x3_u16nw_4(dst, src, hkernel, vkernel,
		    scalef_expon);
	}

	return (MLIB_FAILURE);
}

/* *********************************************************** */

mlib_status
__mlib_ImageSConv3x3(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_s32 *hkernel,
    const mlib_s32 *vkernel,
    mlib_s32 scale,
    mlib_s32 cmask,
    mlib_edge edge)
{
	mlib_s32 ksize = 3, ksize2 = ksize / 2;
	mlib_image dst_i[1], src_i[1], dst_e[1], src_e[1];
	mlib_s32 edg_sizes[4];
	mlib_type type;
	mlib_s32 nchan, dx_l, dx_r, dy_t, dy_b;
	mlib_s32 zero[4] = { 0, 0, 0, 0 };
	mlib_status ret;

	ret =
	    mlib_ImageClipping(dst_i, src_i, dst_e, src_e, edg_sizes, dst,
	    (void *)src, ksize);

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
			case MLIB_BYTE:

				if (scale < 24 || scale > 31)
					return (MLIB_FAILURE);
				mlib_m_sconv3x3nw_u8(dst_i, src_i,
				    (void *)hkernel, (void *)vkernel, scale,
				    cmask);
				break;

			case MLIB_SHORT:

				if (scale < 26 || scale > 33)
					return (MLIB_FAILURE);
				mlib_m_sconv3x3nw_s16(dst_i, src_i,
				    (void *)hkernel, (void *)vkernel, scale,
				    cmask);
				break;

			case MLIB_USHORT:

				if (scale < 26 || scale > 33)
					return (MLIB_FAILURE);
				mlib_m_sconv3x3nw_u16(dst_i, src_i,
				    (void *)hkernel, (void *)vkernel, scale,
				    cmask);
				break;

			case MLIB_INT:

				if (scale < 0)
					return (MLIB_FAILURE);
				mlib_sconv3x3nw_s32(dst_i, src_i,
				    (void *)hkernel, (void *)vkernel, scale,
				    cmask);
				break;

			default:
				return (MLIB_FAILURE);
			}

		switch (edge) {
		case MLIB_EDGE_DST_FILL_ZERO:
			mlib_ImageConvClearEdge(dst_e, dx_l, dx_r, dy_t, dy_b,
			    zero, cmask);
			break;
		case MLIB_EDGE_DST_COPY_SRC:
			mlib_ImageConvCopyEdge(dst_e, src_e, dx_l, dx_r, dy_t,
			    dy_b, cmask);
			break;
		}

	} else {
/* MLIB_EDGE_SRC_EXTEND */
/* adjust src_e image */
		mlib_ImageSetSubimage(src_e, src_e, dx_l - ksize2,
		    dy_t - ksize2, mlib_ImageGetWidth(src_e),
		    mlib_ImageGetHeight(src_e));

		switch (type) {
		case MLIB_BYTE:

			if (scale < 24 || scale > 31)
				return (MLIB_FAILURE);
			mlib_sconv3x3ext_u8(dst_e, src_e, dx_l, dx_r, dy_t,
			    dy_b, (void *)hkernel, (void *)vkernel, scale,
			    cmask);
			break;
		case MLIB_SHORT:

			if (scale < 26 || scale > 33)
				return (MLIB_FAILURE);
			mlib_sconv3x3ext_s16(dst_e, src_e, dx_l, dx_r, dy_t,
			    dy_b, (void *)hkernel, (void *)vkernel, scale,
			    cmask);
			break;
		case MLIB_USHORT:

			if (scale < 26 || scale > 33)
				return (MLIB_FAILURE);
			mlib_sconv3x3ext_u16(dst_e, src_e, dx_l, dx_r, dy_t,
			    dy_b, (void *)hkernel, (void *)vkernel, scale,
			    cmask);
			break;
		case MLIB_INT:

			if (scale < 0)
				return (MLIB_FAILURE);
			mlib_sconv3x3ext_s32(dst_e, src_e, dx_l, dx_r, dy_t,
			    dy_b, (void *)hkernel, (void *)vkernel, scale,
			    cmask);
			break;

		default:
			return (MLIB_FAILURE);
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
