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

#pragma ident	"@(#)mlib_v_ImageFilteredSubsample.c	9.2	07/11/05 SMI"

/*
 * FUNCTION
 *
 *      mlib_ImageFilteredSubsample - Antialias filter and subsample an image
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_ImageFilteredSubsample(mlib_image       *dst,
 *                                              const mlib_image *src,
 *                                              mlib_s32         scaleX,
 *                                              mlib_s32         scaleY,
 *                                              mlib_s32         transX,
 *                                              mlib_s32         transY,
 *                                              const mlib_d64   *hKernel,
 *                                              const mlib_d64   *vKernel,
 *                                              mlib_s32         hSize,
 *                                              mlib_s32         vSize,
 *                                              mlib_s32         hParity,
 *                                              mlib_s32         vParity,
 *                                              mlib_edge        edge)
 *
 * ARGUMENTS
 *
 *      dst       Pointer to the destination image.
 *      src       Pointer to the source image.
 *      scaleX    The x scale factor of subsampling.
 *      scaleY    The y scale factor of subsampling.
 *      transX    The x translation.
 *      transY    The y translation.
 *      hKernel   Pointer to the compact form of the horizontal kernel.
 *      vKernel   Pointer to the compact form of the vertical kernel.
 *      hSize     Size of the array hKernel.
 *      vSize     Size of the array vKernel.
 *      hParity   Parity of the horizontal kernel (0 => even, 1 => odd).
 *      vParity   Parity of the vertical kernel (0 => even, 1 => odd).
 *      edge      Type of edge condition.
 *
 * DESCRIPTION
 *
 *      An operator to antialias filter and subsample images.  The effect of
 *      this operation on an image is equivalent to performing convolution
 *      (filter) followed by subsample (zoom out) operations.
 *
 *      This operator is similar to the image ZoomTranslate operator.  But
 *      they have different definitions on scale factors and translations,
 *      hence use different coordinate mapping equations.  The scaleX and
 *      scaleY used by FilteredSubsample are the reciprocals of the zoomx
 *      and zoomy, respectively, used by ZoomTranslate.
 *
 *      The FilteredSubsample operator mapping equations are given by
 *
 *          xS = xD*scaleX + transX
 *          yS = yD*scaleY + transY
 *
 *      where, a point (xD, yD) in the destination image is backward mapped
 *      to a point (xS, yS) in the source image.  The arguments transX and
 *      transY are provided to support tiling.
 *
 *      The subsample terms, i.e., the scale factors scaleX and scaleY,
 *      are restricted to positive integral values.  Geometrically, one
 *      destination pixel maps to scaleX by scaleY source pixels.
 *      With odd scale factors, destination pixel centers map directly onto
 *      source pixel centers.  With even scale factors, destination pixels
 *      map squarely between source pixel centers.  Below are examples of
 *      even, odd, and combination cases.
 *
 *          s = source pixel centers
 *          d = destination pixel centers mapped to source
 *
 *          s   s   s   s   s   s           s   s   s   s   s   s
 *            d       d       d
 *          s   s   s   s   s   s           s   d   s   s   d   s
 *
 *          s   s   s   s   s   s           s   s   s   s   s   s
 *            d       d       d
 *          s   s   s   s   s   s           s   s   s   s   s   s
 *
 *          s   s   s   s   s   s           s   d   s   s   d   s
 *            d       d       d
 *          s   s   s   s   s   s           s   s   s   s   s   s
 *
 *          Even scaleX/Y factors            Odd scaleX/Y factors
 *
 *          s   s   s   s   s   s           s   s   s   s   s   s
 *              d           d
 *          s   s   s   s   s   s           s d s   s d s   s d s
 *
 *          s   s   s   s   s   s           s   s   s   s   s   s
 *              d           d
 *          s   s   s   s   s   s           s   s   s   s   s   s
 *
 *          s   s   s   s   s   s           s d s   s d s   s d s
 *              d           d
 *          s   s   s   s   s   s           s   s   s   s   s   s
 *
 *         Odd/even scaleX/Y factors      Even/odd scaleX/Y factors
 *
 *      The applied filter is quadrant symmetric (typically antialias +
 *      resample).  The filter is product-separable, quadrant symmetric,
 *      and is defined by half of its span.  Parity is used to signify
 *      whether the symmetric kernel has a double center (even parity) or
 *      a single center value (odd parity).  For example, if hParity == 0
 *      (even), the horizontal kernel is defined as:
 *
 *      hKernel[hSize-1], ..., hKernel[0], hKernel[0], ..., hKernel[hSize-1]
 *
 *      Otherwise, if hParity == 1 (odd), the horizontal kernel is defined
 *      as:
 *
 *          hKernel[hSize-1], ..., hKernel[0], ..., hKernel[hSize-1]
 *
 *      Horizontal and vertical kernels representing convolved resample
 *      (i.e., the combined separable kernels) can be computed from a
 *      convolution filter (with odd parity), a resample filter, and because
 *      the subsample factors affect resample weights, the subsample scale
 *      factors.  It is the user's responsibility to provide meaningful
 *      combined kernels.
 *
 *      To compute the value of a pixel centered at point (xD, yD) in the
 *      destination image, apply the combined kernel to the source image by
 *      aligning the kernel's geometric center to the backward mapped point
 *      (xS, yS) in the source image.  In the cases that it can not be
 *      exactly on top of point (xS, yS), the kernel's center should be
 *      half-pixel right and/or below that point.  When this is done in a
 *      separable manner, the centers of horizontal and vertical kernels
 *      should align with xS and yS, respectively.
 *
 *      The combination of subsampling and filtering has performance
 *      benefits over sequential operator usage in part due to the symmetry
 *      constraints imposed by only allowing integer parameters for scaling
 *      and only allowing separable symmetric filters.
 *
 * RESTRICTION
 *
 *      The src and the dst must be the same type and have same number
 *      of channels (1, 2, 3, or 4).
 *      The images can be of type MLIB_BYTE, MLIB_SHORT, MLIB_USHORT or
 *      MLIB_INT.
 *
 *      The image pixels are assumed to be centered at .5 coordinate points,
 *      and the upper-left corner pixel of an image is located at (0.5, 0.5).
 *
 *      scaleX > 0
 *      scaleY > 0
 *
 *      The edge condition can be one of the following:
 *              MLIB_EDGE_DST_NO_WRITE  (default)
 */

#include <mlib_image.h>
#include <mlib_SysMath.h>
#include <mlib_ImageCheck.h>
#include <mlib_ImageFilteredSubsample.h>
#include <mlib_v_ImageFilteredSubsample.h>
#include <mlib_v_ImageFilteredSubsample_f.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageFilteredSubsample = __mlib_ImageFilteredSubsample

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_ImageFilteredSubsample) mlib_ImageFilteredSubsample
    __attribute__((weak, alias("__mlib_ImageFilteredSubsample")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	PARAMS                                                        \
	dst, src, scaleX, scaleY, transX, transY, hKernel, vKernel,   \
	hSize, vSize, hParity, vParity, edge

/* *********************************************************** */

#define	PARAMS_VIS                                                    \
	dst, src, scaleX, scaleY, transX, transY, hKernel, vKernel,   \
	iscale, hSize, vSize, hParity, vParity, edge

/* *********************************************************** */

mlib_status
__mlib_ImageFilteredSubsample(
    mlib_image *dst,
    const mlib_image *src,
    mlib_s32 scaleX,
    mlib_s32 scaleY,
    mlib_s32 transX,
    mlib_s32 transY,
    const mlib_d64 *hKernel,
    const mlib_d64 *vKernel,
    mlib_s32 hSize,
    mlib_s32 vSize,
    mlib_s32 hParity,
    mlib_s32 vParity,
    mlib_edge edge)
{
	mlib_type type;
	mlib_d64 fhkernel[16], fvkernel[16];
	mlib_s32 kw = 2 * hSize - hParity;
	mlib_s32 kh = 2 * vSize - vParity;
	mlib_s32 i, j, nchan, iscale;
	mlib_d64 sum_p, sum_n, sum;

	MLIB_IMAGE_CHECK(dst);
	MLIB_IMAGE_CHECK(src);
	MLIB_IMAGE_TYPE_EQUAL(dst, src);
	MLIB_IMAGE_CHAN_EQUAL(dst, src);

	if (edge != MLIB_EDGE_DST_NO_WRITE)
		return (MLIB_FAILURE);

	type = mlib_ImageGetType(dst);
	nchan = mlib_ImageGetChannels(dst);

/* VIS version */
	if (type == MLIB_BYTE && (kh < 3 * scaleY || nchan == 4)) {
		for (i = 0; i < hSize; i++) {
			fhkernel[i] = fhkernel[kw - 1 - i] =
			    hKernel[hSize - 1 - i];
		}

		for (i = 0; i < vSize; i++) {
			fvkernel[i] = fvkernel[kh - 1 - i] =
			    vKernel[vSize - 1 - i];
		}

		sum_p = 0;
		sum_n = 0;
		for (j = 0; j < kh; j++) {
			for (i = 0; i < kw; i++) {
				mlib_d64 kk = fvkernel[j] * fhkernel[i];

				if (kk > 0)
					sum_p += kk;
				else
					sum_n -= kk;
			}
		}

		sum = (sum_p > sum_n) ? sum_p : sum_n;
		iscale = 1 + mlib_ilogb((128.0 / 127.) * sum);

		if (iscale < 0)
			iscale = 0;
		if (iscale < 7)
		    return (mlib_v_ImageFilteredSubsample_8nw_1(PARAMS_VIS));
	}

/* C version */

	if (kh > 2 * scaleY || (kh == 2 * scaleY && scaleY > 1)) {
		switch (type) {
		case MLIB_BYTE:
			return (mlib_ImageFilteredSubsample_8nw_2(PARAMS));
		case MLIB_SHORT:
			return (mlib_v_ImageFilteredSubsample_16nw_2(PARAMS));
		case MLIB_USHORT:
			return (mlib_v_ImageFilteredSubsample_u16nw_2(PARAMS));
		case MLIB_INT:
			return (mlib_ImageFilteredSubsample_32nw_2(PARAMS));
		default:
			return (MLIB_FAILURE);
		}

	} else {
		switch (type) {
		case MLIB_BYTE:
			return (mlib_ImageFilteredSubsample_8nw_1(PARAMS));
		case MLIB_SHORT:
			return (mlib_v_ImageFilteredSubsample_16nw_1(PARAMS));
		case MLIB_USHORT:
			return (mlib_v_ImageFilteredSubsample_u16nw_1(PARAMS));
		case MLIB_INT:
			return (mlib_ImageFilteredSubsample_32nw_1(PARAMS));
		default:
			return (MLIB_FAILURE);
		}
	}
}

/* *********************************************************** */
