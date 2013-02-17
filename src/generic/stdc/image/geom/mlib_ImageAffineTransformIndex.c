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

#pragma ident	"@(#)mlib_ImageAffineTransformIndex.c	9.2	07/10/09 SMI"

/*
 * FUNCTION
 *      mlib_ImageAffineTransformIndex - indexed color image affine
 *                                       transformation with matrix
 *                                       checking and edge condition
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageAffineTransformIndex(mlib_image       *dst,
 *                                                 const mlib_image *src,
 *                                                 const mlib_d64   *mtx,
 *                                                 mlib_filter      filter,
 *                                                 mlib_edge        edge,
 *                                                 const void       *colormap)
 *
 * ARGUMENTS
 *      dst       Pointer to destination image
 *      src       Pointer to source image
 *      mtx       Transformation matrix, where
 *                mtx[0] holds a;  mtx[1] holds b;
 *                mtx[2] holds tx; mtx[3] holds c;
 *                mtx[4] holds d;  mtx[5] holds ty.
 *      filter    Type of resampling filter.
 *      edge      Type of edge conditions.
 *      colormap  Internal data structure for inverse color mapping.
 *
 * DESCRIPTION
 *                      xd = a*xs + b*ys + tx
 *                      yd = c*xs + d*ys + ty
 *
 *  Check the matrix first to identify the simple rotation, scaling,
 *  etc., then call the most efficient mediaLib function.
 *
 *  The upper-left corner pixel of an image is located at (0.5, 0.5).
 *
 *  The resampling filter can be one of the following:
 *    MLIB_NEAREST
 *    MLIB_BILINEAR
 *    MLIB_BICUBIC
 *    MLIB_BICUBIC2
 *
 *  The edge condition can be one of the following:
 *    MLIB_EDGE_DST_NO_WRITE  (default)
 *    MLIB_EDGE_DST_FILL_ZERO
 *    MLIB_EDGE_OP_NEAREST
 *    MLIB_EDGE_SRC_EXTEND
 *    MLIB_EDGE_SRC_PADDED
 *
 * RESTRICTION
 *      src and dst must be the same type and have just one channel.
 *      They can be in MLIB_BYTE or MLIB_SHORT data type.
 *
 *      src image can not have width or height larger than 32767.
 */

#include <mlib_image.h>
#include <mlib_ImageCheck.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageAffineTransformIndex = __mlib_ImageAffineTransformIndex
#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_ImageAffineTransformIndex) mlib_ImageAffineTransformIndex
	__attribute__((weak, alias("__mlib_ImageAffineTransformIndex")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

mlib_status
__mlib_ImageAffineTransformIndex(
	mlib_image *dst,
	const mlib_image *src,
	const mlib_d64 *mtx,
	mlib_filter filter,
	mlib_edge edge,
	const void *colormap)
{
	mlib_s32 srcWidth, dstWidth, srcHeight, dstHeight, cx, cy;
	mlib_d64 a, b, c, d, tx, ty;
	mlib_d64 determ, deltatx, deltaty;

/*
 * check for obvious errors
 */
	MLIB_IMAGE_CHECK(src);
	MLIB_IMAGE_CHECK(dst);

	srcWidth = mlib_ImageGetWidth(src);
	srcHeight = mlib_ImageGetHeight(src);
	dstWidth = mlib_ImageGetWidth(dst);
	dstHeight = mlib_ImageGetHeight(dst);

	if (mtx == NULL)
		return (MLIB_NULLPOINTER);

	a = mtx[0], b = mtx[1], tx = 2 * mtx[2], c = mtx[3], d = mtx[4], ty =
		2 * mtx[5];

	determ = a * d - b * c;

	if (determ == 0)
		return (MLIB_FAILURE);
	determ = 1.0 / determ;

	deltatx =
		(d * (tx - dstWidth) - b * (ty - dstHeight)) * determ +
		srcWidth;
	deltaty =
		(-c * (tx - dstWidth) + a * (ty - dstHeight)) * determ +
		srcHeight;
	cx = ((mlib_s32)((d * dstWidth - b * dstHeight) * determ -
		srcWidth)) & 1;
	cy = ((mlib_s32)((-c * dstWidth + a * dstHeight) * determ -
		srcHeight)) & 1;

	if (filter == MLIB_NEAREST) {
		if (((deltatx > -(cx + 1)) && (deltatx <= (1 - cx))) &&
			((deltaty > -(cy + 1)) && (deltaty <= (1 - cy)))) {

			if ((a == 0) && (d == 0)) {
				if ((b == -1) && (c == 1)) {
					return (__mlib_ImageRotate90(dst, src));
				}

				if ((b == 1) && (c == -1)) {
					return (__mlib_ImageRotate270(
						dst, src));
				}

				if ((b == 1) && (c == 1)) {
					return __mlib_ImageFlipMainDiag(dst,
						src);
				}

				if ((b == -1) && (c == -1)) {
					return __mlib_ImageFlipAntiDiag(dst,
						src);
				}
			}

			if ((b == 0) && (c == 0)) {
				if ((a == -1) && (d == -1)) {
					return (__mlib_ImageRotate180(
						dst, src));
				}

				if ((a == 1) && (d == -1)) {
					return (__mlib_ImageFlipX(dst, src));
				}

				if ((a == -1) && (d == 1)) {
					return (__mlib_ImageFlipY(dst, src));
				}
			}
		}

		if ((b == 0) && (c == 0)) {
			if ((a == 2) && (d == 2) &&
				(((deltatx > (-0.5 * (1 + (dstWidth & 1)))) &&
				(deltatx <= (0.5 * (1 - (dstWidth & 1))))) &&
				((deltaty > (-0.5 * (1 + (dstHeight & 1)))) &&
				(deltaty <= (0.5 * (1 - (dstHeight & 1))))))) {
				return __mlib_ImageZoomIn2X(dst, src, filter,
					edge);
			}

			if ((a == 0.5) && (d == 0.5) &&
				(((deltatx > -(((srcWidth - 1) & 1) + 1)) &&
				(deltatx <= (1 - ((srcWidth - 1) & 1)))) &&
				((deltaty > -(((srcHeight - 1) & 1) + 1)) &&
				(deltaty <= (1 - ((srcHeight - 1) & 1)))))) {
				return __mlib_ImageZoomOut2X(dst, src, filter,
					edge);
			}
		}

		if ((a == 1) && (d == 1) && (b == 0) && (c == 0)) {
			mlib_s32 dx_d;
			mlib_s32 dy_d;

			tx = tx / 2.0 - 0.5;
			dx_d = (mlib_s32)tx;
			ty = ty / 2.0 - 0.5;
			dy_d = (mlib_s32)ty;

			if ((tx >= 0.0) && (dx_d != tx))
				dx_d++;

			if ((ty >= 0.0) && (dy_d != ty))
				dy_d++;

			return __mlib_ImageCopySubimage(dst, src, dx_d, dy_d, 0,
				0, srcWidth, srcHeight);
		}
	}

	if ((b == 0) && (c == 0) && (deltatx == 0) && (deltaty == 0)) {
		if ((a == 2) && (d == 2)) {
			return __mlib_ImageZoomIn2XIndex(dst, src, filter, edge,
				colormap);
		}

		if ((a == 0.5) && (d == 0.5)) {
			return __mlib_ImageZoomOut2XIndex(dst, src, filter,
				edge, colormap);
		}

		if ((a > 0) && (d > 0)) {
			return __mlib_ImageZoomIndex(dst, src, a, d, filter,
				edge, colormap);
		}
	}

	return (__mlib_ImageAffineIndex(dst, src, mtx, filter, edge, colormap));
}

/* *********************************************************** */
