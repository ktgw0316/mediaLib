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

#pragma ident	"@(#)mlib_m_ImageConv5x5.c	9.4	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_ImageConv5x5 - image convolution with edge condition
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageConv5x5(mlib_image       *dst,
 *                                    const mlib_image *src,
 *                                    const mlib_s32   *kernel,
 *                                    mlib_s32         scale,
 *                                    mlib_s32         cmask,
 *                                    mlib_edge        edge)
 *
 * ARGUMENTS
 *      dst       Pointer to destination image.
 *      src       Pointer to source image.
 *      kernel    Pointer to convolution kernel.
 *      scale     The scaling factor to convert the input integer
 *                coefficients into floating-point coefficients:
 *                floating-point coefficient = integer coefficient * 2^(-scale)
 *      cmask     Channel mask to indicate the channels to be convolved.
 *                Each bit of which represents a channel in the image. The
 *                channels corresponded to 1 bits are those to be processed.
 *      edge      Type of edge condition.
 *
 * DESCRIPTION
 *      2-D convolution, 5x5 kernel.
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
 *      For data type MLIB_BYTE:  16 <= scale <= 31
 *      For data type MLIB_SHORT: 17 <= scale <= 32
 *      For data type MLIB_INT:   scale >= 0.
 *
 *      The kernel coefficients used in VIS version of mlib_ImageConv5x5
 *      has to be preprocessed in a special way.
 *      It is recommended to preprocess these coefficients
 *      using the mlib_ImageConvKernelConvert function.
 */

#include <mlib_image.h>
#include <mlib_ImageConv.h>
#include <mlib_c_ImageConv.h>
#include <mlib_ImageClipping.h>
#include <mlib_ImageConvEdge.h>
#include <mlib_ImageCreate.h>

/* *************************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageConv5x5 = __mlib_ImageConv5x5

#elif defined(__GNUC__)  /* defined(__SUNPRO_C) */

__typeof__(__mlib_ImageConv5x5) mlib_ImageConv5x5
    __attribute__((weak, alias("__mlib_ImageConv5x5")));

#else /* defined(__SUNPRO_C) */

#error "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

    mlib_status mlib_m_conv5x5_8nw_1(
    mlib_image *dst,
    mlib_image *src,
    mlib_s32 *kernel,
    mlib_s32 scalef_expon);

    mlib_status mlib_m_conv5x5_8nw_2(
    mlib_image *dst,
    mlib_image *src,
    mlib_s32 *kernel,
    mlib_s32 scalef_expon);

    mlib_status mlib_m_conv5x5_8nw_3(
    mlib_image *dst,
    mlib_image *src,
    mlib_s32 *kernel,
    mlib_s32 scalef_expon);

    mlib_status mlib_m_conv5x5_8nw_4(
    mlib_image *dst,
    mlib_image *src,
    mlib_s32 *kernel,
    mlib_s32 scalef_expon);

/* *********************************************************** */
    mlib_status mlib_m_conv5x5_16nw_1(
    mlib_image *dst,
    mlib_image *src,
    mlib_s32 *kernel,
    mlib_s32 scalef_expon);

    mlib_status mlib_m_conv5x5_16nw_2(
    mlib_image *dst,
    mlib_image *src,
    mlib_s32 *kernel,
    mlib_s32 scalef_expon);

    mlib_status mlib_m_conv5x5_16nw_3(
    mlib_image *dst,
    mlib_image *src,
    mlib_s32 *kernel,
    mlib_s32 scalef_expon);

    mlib_status mlib_m_conv5x5_16nw_4(
    mlib_image *dst,
    mlib_image *src,
    mlib_s32 *kernel,
    mlib_s32 scalef_expon);

/* *********************************************************** */
    mlib_status mlib_m_conv5x5_u16nw_1(
    mlib_image *dst,
    mlib_image *src,
    mlib_s32 *kernel,
    mlib_s32 scalef_expon);

    mlib_status mlib_m_conv5x5_u16nw_2(
    mlib_image *dst,
    mlib_image *src,
    mlib_s32 *kernel,
    mlib_s32 scalef_expon);

    mlib_status mlib_m_conv5x5_u16nw_3(
    mlib_image *dst,
    mlib_image *src,
    mlib_s32 *kernel,
    mlib_s32 scalef_expon);

    mlib_status mlib_m_conv5x5_u16nw_4(
    mlib_image *dst,
    mlib_image *src,
    mlib_s32 *kernel,
    mlib_s32 scalef_expon);

/* *********************************************************** */

mlib_status
mlib_m_conv5x5nw_u8(
    mlib_image *dst,
    mlib_image *src,
    mlib_s32 *kernel,
    mlib_s32 scalef_expon,
    mlib_s32 cmask)
{
	mlib_s32 nchan = mlib_ImageGetChannels(src);
	mlib_s32 amask = (1 << nchan) - 1;
	mlib_s32 chk_flag;

	chk_flag = mlib_ImageConvVersion(5, 5, scalef_expon, MLIB_BYTE);

	if (nchan == 1)
		cmask = 1;
	if ((cmask & amask) != amask || scalef_expon < 25 || chk_flag != 3) {
		if (chk_flag == 0)
			return mlib_c_conv5x5nw_u8(dst, src, kernel,
			    scalef_expon, cmask);
		return mlib_i_conv5x5nw_u8(dst, src, kernel, scalef_expon,
		    cmask);
	}

	switch (nchan) {
	case 1:
		return (mlib_m_conv5x5_8nw_1(dst, src, kernel, scalef_expon));
	case 2:
		return (mlib_m_conv5x5_8nw_2(dst, src, kernel, scalef_expon));
	case 3:
		return (mlib_m_conv5x5_8nw_3(dst, src, kernel, scalef_expon));
	case 4:
		return (mlib_m_conv5x5_8nw_4(dst, src, kernel, scalef_expon));
	}

	return (MLIB_FAILURE);
}

/* *********************************************************** */

mlib_status
mlib_m_conv5x5nw_s16(
    mlib_image *dst,
    mlib_image *src,
    mlib_s32 *kernel,
    mlib_s32 scalef_expon,
    mlib_s32 cmask)
{
	mlib_s32 nchan = mlib_ImageGetChannels(src);
	mlib_s32 amask = (1 << nchan) - 1;
	mlib_s32 chk_flag;

	chk_flag = mlib_ImageConvVersion(5, 5, scalef_expon, MLIB_SHORT);

	if (nchan == 1)
		cmask = 1;
	if ((cmask & amask) != amask || chk_flag != 3) {
		if (chk_flag == 0)
			return mlib_conv5x5nw_s16(dst, src, kernel,
			    scalef_expon, cmask);
		return mlib_i_conv5x5nw_s16(dst, src, kernel, scalef_expon,
		    cmask);
	}

	switch (nchan) {
	case 1:
		return (mlib_m_conv5x5_16nw_1(dst, src, kernel, scalef_expon));
	case 2:
		return (mlib_m_conv5x5_16nw_2(dst, src, kernel, scalef_expon));
	case 3:
		return (mlib_m_conv5x5_16nw_3(dst, src, kernel, scalef_expon));
	case 4:
		return (mlib_m_conv5x5_16nw_4(dst, src, kernel, scalef_expon));
	}

	return (MLIB_FAILURE);
}

/* *********************************************************** */

mlib_status
mlib_m_conv5x5nw_u16(
    mlib_image *dst,
    mlib_image *src,
    mlib_s32 *kernel,
    mlib_s32 scalef_expon,
    mlib_s32 cmask)
{
	mlib_s32 nchan = mlib_ImageGetChannels(src);
	mlib_s32 amask = (1 << nchan) - 1;
	mlib_s32 chk_flag;

	chk_flag = mlib_ImageConvVersion(5, 5, scalef_expon, MLIB_USHORT);

	if (nchan == 1)
		cmask = 1;
	if ((cmask & amask) != amask || chk_flag != 3) {
		if (chk_flag == 0)
			return mlib_conv5x5nw_u16(dst, src, kernel,
			    scalef_expon, cmask);
		return mlib_i_conv5x5nw_u16(dst, src, kernel, scalef_expon,
		    cmask);
	}

	switch (nchan) {
	case 1:
		return (mlib_m_conv5x5_u16nw_1(dst, src, kernel, scalef_expon));
	case 2:
		return (mlib_m_conv5x5_u16nw_2(dst, src, kernel, scalef_expon));
	case 3:
		return (mlib_m_conv5x5_u16nw_3(dst, src, kernel, scalef_expon));
	case 4:
		return (mlib_m_conv5x5_u16nw_4(dst, src, kernel, scalef_expon));
	}

	return (MLIB_FAILURE);
}

/* *********************************************************** */

mlib_status
__mlib_ImageConv5x5(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_s32 *kernel,
    mlib_s32 scale,
    mlib_s32 cmask,
    mlib_edge edge)
{
	mlib_s32 ksize = 5, ksize2 = ksize / 2;
	mlib_image dst_i[1], src_i[1], dst_e[1], src_e[1];
	mlib_s32 edg_sizes[8];
	mlib_type type;
	mlib_s32 nchan, dx_l, dx_r, dy_t, dy_b;
	mlib_s32 zero[4] = { 0, 0, 0, 0 };
	mlib_status ret;

	ret =
	    mlib_ImageClipping(dst_i, src_i, dst_e, src_e, edg_sizes, dst,
	    (void *)src, ksize);

	if (ret != MLIB_SUCCESS)
		return (ret);
	if (kernel == NULL)
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

				if (scale < 16 || scale > 31)
					return (MLIB_FAILURE);
				mlib_m_conv5x5nw_u8(dst_i, src_i,
				    (void *)kernel, scale, cmask);
				break;

			case MLIB_SHORT:

				if (scale < 17 || scale > 32)
					return (MLIB_FAILURE);
				mlib_m_conv5x5nw_s16(dst_i, src_i,
				    (void *)kernel, scale, cmask);
				break;

			case MLIB_USHORT:

				if (scale < 17 || scale > 32)
					return (MLIB_FAILURE);
				mlib_m_conv5x5nw_u16(dst_i, src_i,
				    (void *)kernel, scale, cmask);
				break;

			case MLIB_INT:

				if (scale < 0)
					return (MLIB_FAILURE);
				mlib_conv5x5nw_s32(dst_i, src_i, (void *)kernel,
				    scale, cmask);
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

			if (scale < 16 || scale > 31)
				return (MLIB_FAILURE);
			if (mlib_ImageConvVersion(5, 5, scale, type) == 0)
				mlib_c_conv5x5ext_u8(dst_e, src_e, dx_l, dx_r,
				    dy_t, dy_b, (void *)kernel, scale, cmask);
			else
				mlib_i_conv5x5ext_u8(dst_e, src_e, dx_l, dx_r,
				    dy_t, dy_b, (void *)kernel, scale, cmask);
			break;
		case MLIB_SHORT:

			if (scale < 17 || scale > 32)
				return (MLIB_FAILURE);
			if (mlib_ImageConvVersion(5, 5, scale, type) == 0)
				mlib_conv5x5ext_s16(dst_e, src_e, dx_l, dx_r,
				    dy_t, dy_b, (void *)kernel, scale, cmask);
			else
				mlib_i_conv5x5ext_s16(dst_e, src_e, dx_l, dx_r,
				    dy_t, dy_b, (void *)kernel, scale, cmask);
			break;
		case MLIB_USHORT:

			if (scale < 17 || scale > 32)
				return (MLIB_FAILURE);
			if (mlib_ImageConvVersion(5, 5, scale, type) == 0)
				mlib_conv5x5ext_u16(dst_e, src_e, dx_l, dx_r,
				    dy_t, dy_b, (void *)kernel, scale, cmask);
			else
				mlib_i_conv5x5ext_u16(dst_e, src_e, dx_l, dx_r,
				    dy_t, dy_b, (void *)kernel, scale, cmask);
			break;
		case MLIB_INT:

			if (scale < 0)
				return (MLIB_FAILURE);
			mlib_conv5x5ext_s32(dst_e, src_e, dx_l, dx_r, dy_t,
			    dy_b, (void *)kernel, scale, cmask);
			break;

		default:
			return (MLIB_FAILURE);
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
