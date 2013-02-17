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

#pragma ident	"@(#)mlib_ImageConvMxNIndex.c	9.2	07/10/09 SMI"

/*
 * FUNCTION
 *      mlib_ImageConvMxNIndex - indexed color image convolution
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageConvMxNIndex(mlib_image       *dst,
 *                                         const mlib_image *src,
 *                                         const mlib_s32   *kernel,
 *                                         mlib_s32         m,
 *                                         mlib_s32         n,
 *                                         mlib_s32         dm,
 *                                         mlib_s32         dn,
 *                                         mlib_s32         scale,
 *                                         mlib_edge        edge,
 *                                         const void       *colormap)
 *
 * ARGUMENTS
 *      dst       Pointer to destination image.
 *      src       Pointer to source image.
 *      m         Kernel width.
 *      n         Kernel height.
 *      dm, dn    Position of key element in convolution kernel.
 *      kernel    Pointer to convolution kernel.
 *      scale     The scaling factor to convert the input integer
 *                coefficients into floating-point coefficients:
 *                floating-point coefficient = integer coefficient * 2^(-scale)
 *      edge      Type of edge conditions.
 *      colormap  Internal data structure for inverse color mapping
 *
 * DESCRIPTION
 *      2-D convolution, MxN kernel.
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
 *      The src and the dst must be the same type and have one channel.
 *      m > 1, n > 1,
 *      0 <= dm < m, 0 <= dn < n.
 *      For lut type MLIB_BYTE - 16 <= scale <= 31 (to be compatible with VIS
 *      version).
 *      For lut type MLIB_SHORT - 17 <= scale <= 32 (to be compatible with VIS
 *      version).
 */

#include <mlib_image.h>
#include <mlib_ImageCheck.h>
#include <mlib_ImageColormap.h>
#include <mlib_ImageConvEdge.h>
#include <mlib_ImageConvIndex.h>
#include <mlib_ImageCreate.h>
#include <mlib_ImageClipping.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageConvMxNIndex = __mlib_ImageConvMxNIndex

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_ImageConvMxNIndex) mlib_ImageConvMxNIndex
    __attribute__((weak, alias("__mlib_ImageConvMxNIndex")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	MIN_KRNL_HGT	2
#define	MIN_KRNL_WID	2

/* *********************************************************** */

mlib_status
__mlib_ImageConvMxNIndex(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_s32 *kernel,
    mlib_s32 m,
    mlib_s32 n,
    mlib_s32 dm,
    mlib_s32 dn,
    mlib_s32 scale,
    mlib_edge edge,
    const void *colormap)
{
	mlib_image dst_i[1], src_i[1], dst_e[1], src_e[1];
	mlib_type  img_type, lut_type;
	mlib_s32   offset, func_index;
	mlib_s32   dx_l, dx_r, dy_t, dy_b;
	mlib_s32   edg_sizes[8];
	mlib_status ret;

	MLIB_IMAGE_CHECK(dst);
	MLIB_IMAGE_CHECK(src);
	MLIB_IMAGE_TYPE_EQUAL(dst, src);
	MLIB_IMAGE_HAVE_CHAN(src, 1);
	MLIB_IMAGE_HAVE_CHAN(dst, 1);

	if (colormap == NULL || kernel == NULL)
		return (MLIB_FAILURE);

	img_type = mlib_ImageGetType(src);
	lut_type = mlib_ImageGetLutType(colormap);
	offset = (mlib_s32)mlib_ImageGetLutOffset(colormap);

	if (mlib_ImageGetWidth(src) < m || mlib_ImageGetHeight(src) < n)
		return (MLIB_FAILURE);

	if (((lut_type == MLIB_BYTE) && (scale < 16 || scale > 31)) ||
	    ((lut_type == MLIB_SHORT) && (scale < 17 || scale > 32)))
		return (MLIB_FAILURE);

	if (n < MIN_KRNL_HGT || m < MIN_KRNL_WID || dm < 0 || dm > m - 1 ||
	    dn < 0 || dn > n - 1)
		return (MLIB_FAILURE);

	ret =
	    mlib_ImageClippingMxN(dst_i, src_i, dst_e, src_e, edg_sizes, dst,
	    src, m, n, dm, dn);

	if (ret != MLIB_SUCCESS)
		return (ret);

	dx_l = edg_sizes[0];
	dx_r = edg_sizes[1];
	dy_t = edg_sizes[2];
	dy_b = edg_sizes[3];

	if (dx_l + dx_r + dy_t + dy_b == 0)
		edge = MLIB_EDGE_DST_NO_WRITE;

	func_index = 0;
	if (img_type == MLIB_SHORT) func_index += 1;
	if (lut_type == MLIB_SHORT) func_index += 2;

	if (edge != MLIB_EDGE_SRC_EXTEND) {
		if (mlib_ImageGetWidth(dst_i) >= m &&
		    mlib_ImageGetHeight(dst_i) >= n) {
			ret = func_convMxNIndex_nw[func_index](src_i, dst_i,
				m, n, dm, dn, kernel, scale, colormap);
		}

		switch (edge) {
		case MLIB_EDGE_DST_FILL_ZERO:
			mlib_ImageConvClearEdge(dst_e,
				    dx_l, dx_r, dy_t, dy_b, &offset, 1);
			break;
		case MLIB_EDGE_DST_COPY_SRC:
			mlib_ImageConvCopyEdge(dst_e, src_e,
				    dx_l, dx_r, dy_t, dy_b, 1);
			break;
		default:
			ret = MLIB_SUCCESS;
			break;
		}
	} else if (mlib_ImageGetWidth(dst_e) > 0 &&
	    mlib_ImageGetHeight(dst_e) > 0) {
		/* adjust src_e image */
		mlib_ImageSetSubimage(src_e, src_e, dx_l - dm, dy_t - dn,
		    mlib_ImageGetWidth(src_e), mlib_ImageGetHeight(src_e));

		ret = func_convMxNIndex_ext[func_index](src_e, dst_e, m, n,
			dx_l, dx_r, dy_t, dy_b, kernel, scale, colormap);
	}

	return (ret);
}

/* *********************************************************** */
