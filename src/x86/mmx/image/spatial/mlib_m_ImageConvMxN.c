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

#pragma ident	"@(#)mlib_m_ImageConvMxN.c	9.4	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_ImageConvMxN - image convolution with edge condition
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageConvMxN(mlib_image       *dst,
 *                                    const mlib_image *src,
 *                                    const mlib_s32   *kernel,
 *                                    mlib_s32         m,
 *                                    mlib_s32         n,
 *                                    mlib_s32         dm,
 *                                    mlib_s32         dn,
 *                                    mlib_s32         scale,
 *                                    mlib_s32         cmask,
 *                                    mlib_edge        edge)
 *
 * ARGUMENTS
 *      dst       Pointer to destination image.
 *      src       Pointer to source image.
 *      m         Kernel width (m must be not less than 1).
 *      n         Kernel height (n must be not less than 1).
 *      dm, dn    Position of key element in convolution kernel.
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
 *      2-D convolution, MxN kernel.
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
 *      of channels (1, 2, 3, or 4). They can be in MLIB_BYTE, MLIB_SHORT,
 *      MLIB_USHORT or MLIB_INT data type.
 *      m >= 1, n >= 1,
 *      0 <= dm < m, 0 <= dn < n.
 *      For data type MLIB_BYTE:   16 <= scale <= 31 (to be compatible
 *      with VIS version)
 *      For data type MLIB_SHORT:  17 <= scale <= 32 (to be compatible
 *      with VIS version)
 *      For data type MLIB_USHORT: 17 <= scale <= 32 (to be compatible
 *      with VIS version)
 *      For data type MLIB_INT:    scale >= 0
 */

#include <mlib_image.h>
#include <mlib_ImageCheck.h>
#include <mlib_SysMath.h>
#include <mlib_ImageConv.h>
#include <mlib_ImageCreate.h>
#include <mlib_c_ImageConv.h>
#include <mlib_ImageClipping.h>
#include <mlib_ImageConvEdge.h>

/* *************************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageConvMxN = __mlib_ImageConvMxN

#elif defined(__GNUC__)  /* defined(__SUNPRO_C) */

__typeof__(__mlib_ImageConvMxN) mlib_ImageConvMxN
    __attribute__((weak, alias("__mlib_ImageConvMxN")));

#else /* defined(__SUNPRO_C) */

#error "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

    mlib_status mlib_m_convMxNnw_u8(
    mlib_image *dst,
    mlib_image *src,
    mlib_s32 m,
    mlib_s32 n,
    mlib_s32 dm,
    mlib_s32 dn,
    mlib_s32 *kern,
    mlib_s32 scale);

    mlib_status mlib_m_convMxNnw_s16(
    mlib_image *dst,
    mlib_image *src,
    mlib_s32 m,
    mlib_s32 n,
    mlib_s32 dm,
    mlib_s32 dn,
    mlib_s32 *kern,
    mlib_s32 scale);

    mlib_status mlib_m_convMxNnw_u16(
    mlib_image *dst,
    mlib_image *src,
    mlib_s32 m,
    mlib_s32 n,
    mlib_s32 dm,
    mlib_s32 dn,
    mlib_s32 *kern,
    mlib_s32 scale);

/* *********************************************************** */

mlib_status
mlib_ImageConvMxN_f(
    mlib_image *dst,
    const mlib_image *src,
    const void *kernel,
    mlib_s32 m,
    mlib_s32 n,
    mlib_s32 dm,
    mlib_s32 dn,
    mlib_s32 scale,
    mlib_s32 cmask,
    mlib_edge edge)
{
	mlib_image dst_i[1], src_i[1], dst_e[1], src_e[1];
	mlib_type type;
	mlib_s32 nchan, dx_l, dx_r, dy_t, dy_b;
	mlib_s32 edg_sizes[8];
	mlib_s32 chk_flag;
	mlib_status ret;

	if (m < 1 || n < 1 || dm < 0 || dm > m - 1 || dn < 0 || dn > n - 1)
		return (MLIB_FAILURE);
	if (kernel == NULL)
		return (MLIB_NULLPOINTER);

	ret =
	    mlib_ImageClippingMxN(dst_i, src_i, dst_e, src_e, edg_sizes, dst,
	    src, m, n, dm, dn);

	if (ret != MLIB_SUCCESS)
		return (ret);

	nchan = mlib_ImageGetChannels(dst);
	type = mlib_ImageGetType(dst);

	cmask = (nchan == 1) ? 1 : cmask;

	dx_l = edg_sizes[0];
	dx_r = edg_sizes[1];
	dy_t = edg_sizes[2];
	dy_b = edg_sizes[3];

	if (dx_l + dx_r + dy_t + dy_b == 0)
		edge = MLIB_EDGE_DST_NO_WRITE;

	chk_flag = mlib_ImageConvVersion(m, n, scale, type);

	if (edge != MLIB_EDGE_SRC_EXTEND) {
		if (mlib_ImageGetWidth(dst_i) >= m &&
		    mlib_ImageGetHeight(dst_i) >= n) {
			switch (type) {
			case MLIB_BYTE:

				if (cmask == ((1 << nchan) - 1) &&
					scale >= 25 && chk_flag == 3) {
					ret =
					    mlib_m_convMxNnw_u8(dst_i,
					    (void *)src_i, m, n, dm, dn,
					    (void *)kernel, scale);
				} else if (chk_flag != 0) {
					ret =
					    mlib_i_convMxNnw_u8(dst_i, src_i,
					    kernel, m, n, dm, dn, scale, cmask);
				} else {
					ret =
					    mlib_c_convMxNnw_u8(dst_i, src_i,
					    kernel, m, n, dm, dn, scale, cmask);
				}

				break;
			case MLIB_SHORT:

				if ((cmask == ((1 << nchan) - 1)) &&
				    (chk_flag == 3)) {
					ret =
					    mlib_m_convMxNnw_s16(dst_i,
					    (void *)src_i, m, n, dm, dn,
					    (void *)kernel, scale);
				} else if (chk_flag != 0) {
					ret =
					    mlib_i_convMxNnw_s16(dst_i, src_i,
					    kernel, m, n, dm, dn, scale, cmask);
				} else {
					ret =
					    mlib_convMxNnw_s16(dst_i, src_i,
					    kernel, m, n, dm, dn, scale, cmask);
				}

				break;
			case MLIB_USHORT:

				if ((cmask == ((1 << nchan) - 1)) &&
				    (chk_flag == 3)) {
					ret =
					    mlib_m_convMxNnw_u16(dst_i,
					    (void *)src_i, m, n, dm, dn,
					    (void *)kernel, scale);
				} else if (chk_flag != 0) {
					ret =
					    mlib_i_convMxNnw_u16(dst_i, src_i,
					    kernel, m, n, dm, dn, scale, cmask);
				} else {
					ret =
					    mlib_convMxNnw_u16(dst_i, src_i,
					    kernel, m, n, dm, dn, scale, cmask);
				}

				break;
			case MLIB_INT:
				ret =
				    mlib_convMxNnw_s32(dst_i, src_i, kernel, m,
				    n, dm, dn, scale, cmask);
				break;
			case MLIB_FLOAT:
				ret =
				    mlib_convMxNnw_f32(dst_i, src_i, kernel, m,
				    n, dm, dn, cmask);
				break;
			case MLIB_DOUBLE:
				ret =
				    mlib_convMxNnw_d64(dst_i, src_i, kernel, m,
				    n, dm, dn, cmask);
				break;
			}
		}

		switch (edge) {
		case MLIB_EDGE_DST_FILL_ZERO:
			mlib_ImageConvZeroEdge(dst_e, dx_l, dx_r, dy_t, dy_b,
			    cmask);
			break;
		case MLIB_EDGE_DST_COPY_SRC:
			mlib_ImageConvCopyEdge(dst_e, src_e, dx_l, dx_r, dy_t,
			    dy_b, cmask);
			break;
		}

	} else {
/* MLIB_EDGE_SRC_EXTEND */
/* adjust src_e image */
		mlib_ImageSetSubimage(src_e, src_e, dx_l - dm, dy_t - dn,
		    mlib_ImageGetWidth(src_e), mlib_ImageGetHeight(src_e));

		switch (type) {
		case MLIB_BYTE:

			if (chk_flag == 0) {
				ret =
				    mlib_c_convMxNext_u8(dst_e, src_e, kernel,
				    m, n, dx_l, dx_r, dy_t, dy_b, scale, cmask);
			} else {
				ret =
				    mlib_i_convMxNext_u8(dst_e, src_e, kernel,
				    m, n, dx_l, dx_r, dy_t, dy_b, scale, cmask);
			}

			break;
		case MLIB_SHORT:

			if (chk_flag == 0) {
				ret =
				    mlib_convMxNext_s16(dst_e, src_e, kernel, m,
				    n, dx_l, dx_r, dy_t, dy_b, scale, cmask);
			} else {
				ret =
				    mlib_i_convMxNext_s16(dst_e, src_e, kernel,
				    m, n, dx_l, dx_r, dy_t, dy_b, scale, cmask);
			}

			break;
		case MLIB_USHORT:

			if (chk_flag == 0) {
				ret =
				    mlib_convMxNext_u16(dst_e, src_e, kernel, m,
				    n, dx_l, dx_r, dy_t, dy_b, scale, cmask);
			} else {
				ret =
				    mlib_i_convMxNext_u16(dst_e, src_e, kernel,
				    m, n, dx_l, dx_r, dy_t, dy_b, scale, cmask);
			}

			break;
		case MLIB_INT:
			ret =
			    mlib_convMxNext_s32(dst_e, src_e, kernel, m, n,
			    dx_l, dx_r, dy_t, dy_b, scale, cmask);
			break;
		case MLIB_FLOAT:
			mlib_convMxNext_f32(dst_e, src_e, kernel, m, n, dx_l,
			    dx_r, dy_t, dy_b, cmask);
			break;
		case MLIB_DOUBLE:
			mlib_convMxNext_d64(dst_e, src_e, kernel, m, n, dx_l,
			    dx_r, dy_t, dy_b, cmask);
			break;
		}
	}

	return (ret);
}

/* *********************************************************** */

mlib_status
__mlib_ImageConvMxN(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_s32 *kernel,
    mlib_s32 m,
    mlib_s32 n,
    mlib_s32 dm,
    mlib_s32 dn,
    mlib_s32 scale,
    mlib_s32 cmask,
    mlib_edge edge)
{
	MLIB_IMAGE_CHECK(dst);

	switch (mlib_ImageGetType(dst)) {
	case MLIB_BYTE:

		if (scale < 16 || scale > 31)
			return (MLIB_FAILURE);
		break;
	case MLIB_SHORT:
	case MLIB_USHORT:

		if (scale < 17 || scale > 32)
			return (MLIB_FAILURE);
		break;
	case MLIB_INT:

		if (scale < 0)
			return (MLIB_FAILURE);
		break;
	default:
		return (MLIB_FAILURE);
	}

	return mlib_ImageConvMxN_f(dst, (void *)src, (void *)kernel, m, n, dm,
	    dn, scale, cmask, edge);
}

/* *********************************************************** */
