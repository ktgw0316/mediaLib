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

#pragma ident	"@(#)mlib_c_ImageConv3x3Index.c	9.2	07/10/09 SMI"

/*
 * FUNCTION
 *      mlib_ImageConv3x3Index - indexed color image convolution
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageConv3x3Index(mlib_image       *dst,
 *                                         const mlib_image *src,
 *                                         const mlib_s32   *kernel,
 *                                         mlib_s32         scale,
 *                                         mlib_edge        edge,
 *                                         const void       *colormap)
 *
 * ARGUMENTS
 *      dst       Pointer to destination image.
 *      src       Pointer to source image.
 *      kernel    Pointer to convolution kernel.
 *      scale     The scaling factor to convert the input integer
 *                coefficients into floating-point coefficients:
 *                floating-point coefficient = integer coefficient * 2^(-scale)
 *      edge      Type of edge conditions.
 *      colormap  Internal data structure for inverse color mapping
 *
 * DESCRIPTION
 *      2-D convolution, 3x3 kernel.
 *
 *      The edge condition can be one of the following:
 *              MLIB_EDGE_DST_NO_WRITE  (default)
 *              MLIB_EDGE_DST_FILL_ZERO
 *              MLIB_EDGE_DST_COPY_SRC
 *              MLIB_EDGE_SRC_EXTEND
 *
 * RESTRICTION
 *      The src and the dst must be the same type, same size and have same
 *      number of channels (1).
 *      For lut type MLIB_BYTE - 16 <= scale <= 31 (to be compatible with VIS
 *      version).
 *      For lut type MLIB_SHORT - 17 <= scale <= 32 (to be compatible with VIS
 *      version).
 */

#include <mlib_image.h>
#include <mlib_ImageCheck.h>
#include <mlib_ImageColormap.h>
#include <mlib_ImageConvIndex.h>
#include <mlib_ImageConvEdge.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageConv3x3Index = __mlib_ImageConv3x3Index

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_ImageConv3x3Index) mlib_ImageConv3x3Index
    __attribute__((weak, alias("__mlib_ImageConv3x3Index")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

mlib_status
__mlib_ImageConv3x3Index(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_s32 *kernel,
    mlib_s32 scale,
    mlib_edge edge,
    const void *colormap)
{
	mlib_status stat = MLIB_SUCCESS;
	mlib_type src_dtype, lut_type;
	mlib_s32 zero[4];
	mlib_s32 offset;

	if (dst == NULL || src == NULL || colormap == NULL || kernel == NULL)
		return (MLIB_FAILURE);

	MLIB_IMAGE_FULL_EQUAL(dst, src);
	MLIB_IMAGE_HAVE_CHAN(src, 1);

	src_dtype = mlib_ImageGetType(src);
	lut_type = mlib_ImageGetLutType(colormap);
	offset = (mlib_s32)mlib_ImageGetLutOffset(colormap);
	zero[0] = zero[1] = zero[2] = zero[3] = offset;

	if (mlib_ImageGetWidth(src) < 3 || mlib_ImageGetHeight(src) < 3)
		return (MLIB_FAILURE);

	if (((lut_type == MLIB_BYTE) && (scale < 16 || scale > 31)) ||
	    ((lut_type == MLIB_SHORT) && (scale < 17 || scale > 32)))
		return (MLIB_FAILURE);

	switch (src_dtype) {
	case MLIB_BYTE:
		switch (lut_type) {
		case MLIB_BYTE:
			switch (edge) {
			case MLIB_EDGE_DST_FILL_ZERO:
				stat =
				    mlib_conv3x3Index_8_8nw(src, dst, kernel,
				    scale, colormap);

				if (stat != MLIB_SUCCESS)
					break;
				stat =
				    mlib_ImageConvClearEdge(dst, 1, 1, 1, 1,
				    zero, 1);
				break;
			case MLIB_EDGE_DST_COPY_SRC:
				stat =
				    mlib_conv3x3Index_8_8nw(src, dst, kernel,
				    scale, colormap);

				if (stat != MLIB_SUCCESS)
					break;
				stat =
				    mlib_ImageConvCopyEdge(dst, src, 1, 1, 1, 1,
				    1);
				break;
			case MLIB_EDGE_SRC_EXTEND:
				stat =
				    mlib_conv3x3Index_8_8ext(src, dst, kernel,
				    scale, colormap);
				break;
			case MLIB_EDGE_DST_NO_WRITE:
			default:
				stat =
				    mlib_conv3x3Index_8_8nw(src, dst, kernel,
				    scale, colormap);
				break;
			}

			break;
		case MLIB_SHORT:
			switch (edge) {
			case MLIB_EDGE_DST_FILL_ZERO:
				stat =
				    mlib_conv3x3Index_16_8nw(src, dst, kernel,
				    scale, colormap);

				if (stat != MLIB_SUCCESS)
					break;
				stat =
				    mlib_ImageConvClearEdge(dst, 1, 1, 1, 1,
				    zero, 1);
				break;
			case MLIB_EDGE_DST_COPY_SRC:
				stat =
				    mlib_conv3x3Index_16_8nw(src, dst, kernel,
				    scale, colormap);

				if (stat != MLIB_SUCCESS)
					break;
				stat =
				    mlib_ImageConvCopyEdge(dst, src, 1, 1, 1, 1,
				    1);
				break;
			case MLIB_EDGE_SRC_EXTEND:
				stat =
				    mlib_conv3x3Index_16_8ext(src, dst, kernel,
				    scale, colormap);
				break;
			case MLIB_EDGE_DST_NO_WRITE:
			default:
				stat =
				    mlib_conv3x3Index_16_8nw(src, dst, kernel,
				    scale, colormap);
				break;
			}

			break;
		default:
			stat = MLIB_FAILURE;
			break;
		}

		break;
	case MLIB_SHORT:	/* convert wid from pixels to bytes */
		switch (lut_type) {
		case MLIB_SHORT:
			switch (edge) {
			case MLIB_EDGE_DST_FILL_ZERO:
				stat =
				    mlib_conv3x3Index_16_16nw(src, dst, kernel,
				    scale, colormap);

				if (stat != MLIB_SUCCESS)
					break;
				stat =
				    mlib_ImageConvClearEdge(dst, 1, 1, 1, 1,
				    zero, 1);
				break;
			case MLIB_EDGE_DST_COPY_SRC:
				stat =
				    mlib_conv3x3Index_16_16nw(src, dst, kernel,
				    scale, colormap);

				if (stat != MLIB_SUCCESS)
					break;
				stat =
				    mlib_ImageConvCopyEdge(dst, src, 1, 1, 1, 1,
				    1);
				break;
			case MLIB_EDGE_SRC_EXTEND:
				stat =
				    mlib_conv3x3Index_16_16ext(src, dst, kernel,
				    scale, colormap);
				break;
			case MLIB_EDGE_DST_NO_WRITE:
			default:
				stat =
				    mlib_conv3x3Index_16_16nw(src, dst, kernel,
				    scale, colormap);
				break;
			}

			break;
		case MLIB_BYTE:
			switch (edge) {
			case MLIB_EDGE_DST_FILL_ZERO:
				stat =
				    mlib_conv3x3Index_8_16nw(src, dst, kernel,
				    scale, colormap);

				if (stat != MLIB_SUCCESS)
					break;
				stat =
				    mlib_ImageConvClearEdge(dst, 1, 1, 1, 1,
				    zero, 1);
				break;
			case MLIB_EDGE_DST_COPY_SRC:
				stat =
				    mlib_conv3x3Index_8_16nw(src, dst, kernel,
				    scale, colormap);

				if (stat != MLIB_SUCCESS)
					break;
				stat =
				    mlib_ImageConvCopyEdge(dst, src, 1, 1, 1, 1,
				    1);
				break;
			case MLIB_EDGE_SRC_EXTEND:
				stat =
				    mlib_conv3x3Index_8_16ext(src, dst, kernel,
				    scale, colormap);
				break;
			case MLIB_EDGE_DST_NO_WRITE:
			default:
				stat =
				    mlib_conv3x3Index_8_16nw(src, dst, kernel,
				    scale, colormap);
				break;
			}

			break;
		default:
			stat = MLIB_FAILURE;
			break;
		}

		break;
	default:
		stat = MLIB_FAILURE;
		break;
	}

	return (stat);
}

/* *********************************************************** */
