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

#pragma ident	"@(#)mlib_ImageAffineTransform.c	9.3	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_ImageAffineTransform - image affine transformation with
 *                                  matrix checking and edge condition
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageAffineTransform(mlib_image       *dst,
 *                                            const mlib_image *src,
 *                                            const mlib_d64   *mtx,
 *                                            mlib_filter      filter,
 *                                            mlib_edge        edge)
 *
 * ARGUMENTS
 *      dst      Pointer to destination image
 *      src      Pointer to source image
 *      mtx      Transformation matrix, where
 *               mtx[0] holds a;  mtx[1] holds b;
 *               mtx[2] holds tx; mtx[3] holds c;
 *               mtx[4] holds d;  mtx[5] holds ty.
 *      filter   Type of resampling filter.
 *      edge     Type of edge conditions.
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
 *      src and dst must be the same type and the same number of channels.
 *      They can have 1, 2, 3 or 4 channels. They can be of MLIB_BIT, MLIB_BYTE,
 *      MLIB_SHORT, MLIB_USHORT or MLIB_INT data type.
 *
 *      src image can not have width or height larger than 32767.
 */

#ifndef	_NO_LONGLONG

#include <mlib_image.h>
#include <mlib_ImageCheck.h>
#include <mlib_GeomBasicIntOp.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageAffineTransform = __mlib_ImageAffineTransform

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_ImageAffineTransform) mlib_ImageAffineTransform
	__attribute__((weak, alias("__mlib_ImageAffineTransform")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#undef	SHIFT
#define	SHIFT	16
#define	DTOLL(l, d)	BASIC_DTOLL(l, d, SHIFT)
/* #define	PRINT(A)	printf("DEBUG:");printf(A);printf("\n"); */
#define	PRINT(A)

/* *********************************************************** */

mlib_status
__mlib_ImageAffineTransform(
	mlib_image *dst,
	const mlib_image *src,
	const mlib_d64 *mtx,
	mlib_filter filter,
	mlib_edge edge)
{
	mlib_s32 srcWidth, dstWidth, srcHeight, dstHeight, cx, cy;
	mlib_s64 a, b, c, d, tx, ty;
	mlib_s64 determ, deltatx, deltaty;

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

	DTOLL(a, mtx[0]);
	DTOLL(b, mtx[1]);
	DTOLL(tx, mtx[2]);
	tx <<= 1;
	DTOLL(c, mtx[3]);
	DTOLL(d, mtx[4]);
	DTOLL(ty, mtx[5]);
	ty <<= 1;

	determ = a * d - b * c;

	if (determ == 0)
		return (MLIB_FAILURE);
	/* determ = 1.0 / determ; */

	deltatx =
		(d * (tx - (dstWidth<<SHIFT)) - b * (ty - (dstHeight<<SHIFT))) +
		srcWidth * determ;
	deltaty =
		(-c * (tx - (dstWidth<<SHIFT)) + a * (ty - (dstHeight<<SHIFT)))+
		srcHeight * determ;
	cx = ((mlib_s32)((((d * dstWidth - b * dstHeight)<<SHIFT) - determ *
		srcWidth) / determ)) & 1;
	cy = ((mlib_s32)((((-c * dstWidth + a * dstHeight)<<SHIFT) - determ *
		srcHeight) / determ)) & 1;

	if (filter == MLIB_NEAREST) {
		if (((deltatx > -(cx + 1)*determ) &&
			(deltatx <= (1 - cx)*determ)) &&
			((deltaty > -(cy + 1)*determ) &&
			(deltaty <= (1 - cy)*determ))) {

			if ((a == 0) && (d == 0)) {
				if ((b == -(1<<SHIFT)) && (c == (1<<SHIFT))) {
					PRINT("Rotate90");
					return (__mlib_ImageRotate90(dst, src));
				}

				if ((b == (1<<SHIFT)) && (c == -(1<<SHIFT))) {
					PRINT("Rotate270");
					return (__mlib_ImageRotate270(
						dst, src));
				}

				if ((b == (1<<SHIFT)) && (c == (1<<SHIFT))) {
					PRINT("FlipMainDiag");
					return __mlib_ImageFlipMainDiag(dst,
						src);
				}

				if ((b == -(1<<SHIFT)) && (c == -(1<<SHIFT))) {
					PRINT("FlipAntiDiag");
					return __mlib_ImageFlipAntiDiag(dst,
						src);
				}
			}

			if ((b == 0) && (c == 0)) {
				if ((a == -(1<<SHIFT)) && (d == -(1<<SHIFT))) {
					PRINT("Rotate180");
					return (__mlib_ImageRotate180(
						dst, src));
				}

				if ((a == (1<<SHIFT)) && (d == -(1<<SHIFT))) {
					PRINT("FlipX");
					return (__mlib_ImageFlipX(dst, src));
				}

				if ((a == -(1<<SHIFT)) && (d == (1<<SHIFT))) {
					PRINT("FlipY");
					return (__mlib_ImageFlipY(dst, src));
				}
			}
		}

		if ((b == 0) && (c == 0)) {
			if ((a == (1<<(SHIFT+1))) && (d == (1<<(SHIFT+1))) &&
			((((deltatx<<1) > (-determ * (1 + (dstWidth & 1)))) &&
			((deltatx<<1) <= (determ * (1 - (dstWidth & 1))))) &&
			(((deltaty<<1) > (-determ *(1 + (dstHeight & 1)))) &&
			((deltaty<<1) <= (determ * (1 - (dstHeight & 1))))))) {
				PRINT("ZoomIn2X");
				return __mlib_ImageZoomIn2X(dst, src, filter,
					edge);
			}

			if ((a == (1<<(SHIFT-1))) && (d == (1<<(SHIFT-1))) &&
			(((deltatx > (-determ * (((srcWidth - 1) & 1) + 1))) &&
			(deltatx <= (determ * (1 - ((srcWidth - 1) & 1))))) &&
			((deltaty > (-determ * (((srcHeight - 1) & 1) + 1))) &&
			(deltaty <= (determ * (1 - ((srcHeight - 1) & 1))))))) {
				PRINT("ZoomOut2X");
				return __mlib_ImageZoomOut2X(dst, src, filter,
					edge);
			}
		}

		if ((a == (1<<SHIFT)) && (d == (1<<SHIFT)) &&
			(b == 0) && (c == 0)) {
			mlib_s32 dx_d;
			mlib_s32 dy_d;

#if 0
			tx = tx / 2.0 - 0.5;
			dx_d = (mlib_s32)tx;
			ty = ty / 2.0 - 0.5;
			dy_d = (mlib_s32)ty;
#endif

			tx = (tx >> 1) - (1<<(SHIFT-1));
			if (tx < 0) {
				dx_d = -(mlib_s32)((-tx)>>SHIFT);
			} else {
				dx_d = (mlib_s32)(tx>>SHIFT);
			}
			ty = (ty >> 1) - (1<<(SHIFT-1));
			if (ty < 0) {
				dy_d = -(mlib_s32)((-ty)>>SHIFT);
			} else {
				dy_d = (mlib_s32)(ty>>SHIFT);
			}

			if ((tx >= 0) && ((dx_d<<SHIFT) != tx))
				dx_d++;

			if ((ty >= 0) && ((dy_d<<SHIFT) != ty))
				dy_d++;

			PRINT("CopySubimage");
			return __mlib_ImageCopySubimage(dst, src, dx_d, dy_d, 0,
				0, srcWidth, srcHeight);
		}
	}

	if ((b == 0) && (c == 0) && (deltatx == 0) && (deltaty == 0)) {
		if ((a == (1<<(SHIFT+1))) && (d == (1<<(SHIFT+1)))) {
			PRINT("ZoomIn2X-2");
			return (__mlib_ImageZoomIn2X(dst, src, filter, edge));
		}

		if ((a == (1<<(SHIFT-1))) && (d == (1<<(SHIFT-1)))) {
			PRINT("ZoomOut2X-2");
			return (__mlib_ImageZoomOut2X(dst, src, filter, edge));
		}

		if ((a > 0) && (d > 0)) {
			PRINT("Zoom");
			return (__mlib_ImageZoom(dst, src, mtx[0], mtx[4],
				filter, edge));
		}
	}

	PRINT("Affine");
	return (__mlib_ImageAffine(dst, src, mtx, filter, edge));
}

#else /* _NO_LONGLONG */

#include <mlib_image.h>
#include <mlib_ImageCheck.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageAffineTransform = __mlib_ImageAffineTransform

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_ImageAffineTransform) mlib_ImageAffineTransform
	__attribute__((weak, alias("__mlib_ImageAffineTransform")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

mlib_status
__mlib_ImageAffineTransform(
	mlib_image *dst,
	const mlib_image *src,
	const mlib_d64 *mtx,
	mlib_filter filter,
	mlib_edge edge)
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
			return (__mlib_ImageZoomIn2X(dst, src, filter, edge));
		}

		if ((a == 0.5) && (d == 0.5)) {
			return (__mlib_ImageZoomOut2X(dst, src, filter, edge));
		}

		if ((a > 0) && (d > 0)) {
			return (__mlib_ImageZoom(dst, src, a, d, filter, edge));
		}
	}

	return (__mlib_ImageAffine(dst, src, mtx, filter, edge));
}

#endif /* _NO_LONGLONG */

/* *********************************************************** */
