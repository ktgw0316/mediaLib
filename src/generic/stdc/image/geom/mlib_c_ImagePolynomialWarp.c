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

#pragma ident	"@(#)mlib_c_ImagePolynomialWarp.c	9.2	07/10/09 SMI"

/*
 * FUNCTION
 *
 *      mlib_ImagePolynomialWarp    - polynomial-based image warp
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_ImagePolynomialWarp(mlib_image       *dst,
 *                                           const mlib_image *src,
 *                                           const mlib_d64   *xCoeffs,
 *                                           const mlib_d64   *yCoeffs,
 *                                           mlib_s32         n,
 *                                           mlib_d64         preShiftX,
 *                                           mlib_d64         preShiftY,
 *                                           mlib_d64         postShiftX,
 *                                           mlib_d64         postShiftY,
 *                                           mlib_d64         preScaleX,
 *                                           mlib_d64         preScaleY,
 *                                           mlib_d64         postScaleX,
 *                                           mlib_d64         postScaleY,
 *                                           mlib_filter      filter,
 *                                           mlib_edge        edge)
 *
 * ARGUMENTS
 *
 *      dst         The pointer to destination image.
 *      src         The pointer to source image.
 *      xCoeffs     The destination to source transform coefficients for
 *                      the X coordinate.
 *      yCoeffs     The destination to source transform coefficients for
 *                      the Y coordinate.
 *      n           The degree power of the polynomial.
 *      preShiftX   The displacement to apply to destination X positions.
 *      preShiftY   The displacement to apply to destination Y positions.
 *      postShiftX  The displacement to apply to source X positions.
 *      postShiftY  The displacement to apply to source Y positions.
 *      preScaleX   The scale factor to apply to destination X positions.
 *      preScaleY   The scale factor to apply to destination Y positions.
 *      postScaleX  The scale factor to apply to source X positions.
 *      postScaleY  The scale factor to apply to source Y positions.
 *      filter      The type of resampling filter.
 *      edge        The type of edge condition.
 *
 * RESTRICTION
 *
 *  The images must have the same type, and the same number of channels.
 *  The images can have 1, 2, 3, or 4 channels.
 *  The images can be in MLIB_BYTE, MLIB_SHORT, MLIB_USHORT or MLIB_INT
 *  data type.
 *  The two images might have different sizes.
 *  src image can not have width or height larger than 32767.
 *
 *  The image pixels are assumed to be centered at .5 coordinate points,
 *  e.g., the upper-left corner pixel of an image is located at (0.5, 0.5).
 *
 *  The resampling filter can be one of the following:
 *    MLIB_NEAREST
 *    MLIB_BILINEAR
 *    MLIB_BICUBIC
 *    MLIB_BICUBIC2
 *
 *  The edge condition can be one of the following:
 *    MLIB_EDGE_DST_NO_WRITE  (default)
 *    MLIB_EDGE_SRC_PADDED
 *
 *  The xCoeffs and yCoeffs parameters must contain the same number of
 *  coefficients of the form (n + 1)(n + 2)/2 for some n, where n is the
 *  non-negative degree power of the polynomial. The coefficients,
 *  in order, are associated with the terms:
 *
 *  1, x, y, x^2, x*y, y^2, ..., x^n, x^(n - 1)*y, ..., x*y^(n - 1), y^n
 *
 *  and coefficients of value 0 can not be omitted.
 *
 * DESCRIPTION
 *
 *      A polynomial-based image warp.
 *
 *      For each pixel in the destination image, its center point D is
 *      backward mapped to a point S in the source image. Then the source
 *      pixels with their centers surrounding point S are selected to do
 *      one of the interpolations specified by the `filter' parameter to
 *      generate the pixel value for point D.
 *
 *      The mapping is defined by two bivariate polynomial functions
 *      X(x, y) and Y(x, y) that define the source X and Y positions that
 *      map to a given destination (x, y) pixel coordinate.
 *
 *      The functions X(x, y) and Y(x, y) are:
 *
 *                  n   i
 *      X(x, y) = {SUM{SUM{xCoeffs_ij*((x+preShiftX)*preScaleX)^(i-j)*
 *                 i = 0 j = 0
 *                      ((y+preShiftY)*preScaleY)^j}}}*postScaleX - postShiftX
 *
 *                  n   i
 *      Y(x, y) = {SUM{SUM{yCoeffs_ij*((x+preShiftX)*preScaleX)^(i-j)*
 *                 i = 0 j = 0
 *                      ((y+preShiftY)*preScaleY)^j}}}*postScaleY - postShiftY
 *
 *      The destination (x, y) coordinate is pre-shifted by (preShiftX,
 *      preShiftY) and pre-scaled by the factors preScaleX and preScaleY
 *      prior to the evaluation of the polynomial. The results of the
 *      polynomial evaluations are scaled by postScaleX and postScaleY,
 *      and then shifted by (-postShiftX, -postShiftY) to produce the
 *      source pixel coordinates. This process allows for better precision
 *      of the results and supports tiled images.
 */

#include <mlib_image.h>
#include <mlib_ImageCheck.h>
#include <mlib_ImagePolynomialWarp.h>
#include <mlib_ImageFilters.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImagePolynomialWarp = __mlib_ImagePolynomialWarp

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_ImagePolynomialWarp) mlib_ImagePolynomialWarp
    __attribute__((weak, alias("__mlib_ImagePolynomialWarp")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	FREE_MEMORY	__mlib_free(memory); __mlib_free(xCoeffs_new)

#define	TIN(SFX)	mlib_ImagePolynomialWarp_##SFX

/* *********************************************************** */

mlib_status
__mlib_ImagePolynomialWarp(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_d64 *xCoeffs,
    const mlib_d64 *yCoeffs,
    mlib_s32 n,
    mlib_d64 preShiftX,
    mlib_d64 preShiftY,
    mlib_d64 postShiftX,
    mlib_d64 postShiftY,
    mlib_d64 preScaleX,
    mlib_d64 preScaleY,
    mlib_d64 postScaleX,
    mlib_d64 postScaleY,
    mlib_filter filter,
    mlib_edge edge)
{
	mlib_IPWClipLine array_func_clip[3] = {
		mlib_ImagePolynomialWarpClipLine_0,
		mlib_ImagePolynomialWarpClipLine_1,
		mlib_ImagePolynomialWarpClipLine_2
	};
	mlib_IPWFCall array_func[12][4] = {
/* nearest neighbor */
/* U8  */
		{TIN(NN_U8_1), TIN(NN_U8_2), TIN(NN_U8_3), TIN(NN_U8_4)},
/* S16 */
		{TIN(NN_S16_1), TIN(NN_S16_2), TIN(NN_S16_3), TIN(NN_S16_4)},
/* U16 */
		{TIN(NN_S16_1), TIN(NN_S16_2), TIN(NN_S16_3), TIN(NN_S16_4)},
/* S32 */
		{TIN(NN_S32_1), TIN(NN_S32_2), TIN(NN_S32_3), TIN(NN_S32_4)},

/* bilinear */
/* U8  */
		{TIN(BL_U8_1), TIN(BL_U8_2), TIN(BL_U8_3), TIN(BL_U8_4)},
/* S16 */
		{TIN(BL_S16_1), TIN(BL_S16_2), TIN(BL_S16_3), TIN(BL_S16_4)},
/* U16 */
		{TIN(BL_U16_1), TIN(BL_U16_2), TIN(BL_U16_3), TIN(BL_U16_4)},
/* S32 */
		{TIN(BL_S32_1), TIN(BL_S32_2), TIN(BL_S32_3), TIN(BL_S32_4)},

/* bicubic and bicubic2 */
/* U8  */
		{TIN(BC_U8_1), TIN(BC_U8_2), TIN(BC_U8_3), TIN(BC_U8_4)},
/* S16 */
		{TIN(BC_S16_1), TIN(BC_S16_2), TIN(BC_S16_3), TIN(BC_S16_4)},
/* U16 */
		{TIN(BC_U16_1), TIN(BC_U16_2), TIN(BC_U16_3), TIN(BC_U16_4)},
/* S32 */
		{TIN(BC_S32_1), TIN(BC_S32_2), TIN(BC_S32_3), TIN(BC_S32_4)}
	};

	void *memory = 0;
	mlib_d64 x = 0.5 + preShiftY;
	mlib_d64 border;
	mlib_d64 *xCoeffs_new;
	mlib_d64 *yCoeffs_new;
	mlib_s32 i, len, pos;
	mlib_s32 k = (n + 1) * (n + 2) / 2;
	mlib_s32 channels;
	mlib_u8 *dstData;
	mlib_u8 *srcData;
	const mlib_u8 **lineAddr;
	mlib_type type;
	mlib_PWS pws;
	mlib_IPWClipLine func_clip = NULL;
	mlib_IPWFCall func;

	if (n < 0)
		return (MLIB_FAILURE);

/* check for obvious errors */
	MLIB_IMAGE_CHECK(src);
	MLIB_IMAGE_CHECK(dst);
	MLIB_IMAGE_TYPE_EQUAL(src, dst);
	MLIB_IMAGE_CHAN_EQUAL(src, dst);

	channels = mlib_ImageGetChannels(src);
	dstData = mlib_ImageGetData(dst);
	srcData = mlib_ImageGetData(src);
	type = mlib_ImageGetType(src);

	if (mlib_ImageGetWidth(src) >= (1 << 15) ||
	    mlib_ImageGetHeight(src) >= (1 << 15)) {
		return (MLIB_FAILURE);
	}

	if (n == 0) {
		return mlib_ImagePolynomialWarp_0(dst, src,
		    xCoeffs, yCoeffs,
		    postShiftX, postShiftY, postScaleX, postScaleY,
		    filter, edge);
	}

	if (n == 1) {
		return mlib_ImagePolynomialWarp_1(dst, src,
		    xCoeffs, yCoeffs,
		    preShiftX, preShiftY, postShiftX, postShiftY,
		    preScaleX, preScaleY, postScaleX, postScaleY, filter, edge);
	}

	len = mlib_ImageGetWidth(dst) + 1;
	memory = __mlib_malloc((sizeof (mlib_s32) * 3 +
	    sizeof (mlib_d64) * 2 +
	    sizeof (mlib_d64) * 8) * len +
	    sizeof (void *) * mlib_ImageGetHeight(src));

	if (!memory)
		return (MLIB_FAILURE);

	pws.buffer = memory;

	pws.dsrc_x = (mlib_d64 *)pws.buffer + 8 * mlib_ImageGetWidth(dst);
	pws.dsrc_y = (mlib_d64 *)pws.dsrc_x + mlib_ImageGetWidth(dst);
	lineAddr = (void *)((mlib_d64 *)pws.dsrc_y + mlib_ImageGetWidth(dst));
	pws.src_x = (void *)(lineAddr + mlib_ImageGetHeight(src));
	pws.src_y = (mlib_s32 *)pws.src_x + len;
	pws.dst_x = (mlib_s32 *)pws.src_y + len;

	pws.flags = 0;
	pws.filter_table = (void *)mlib_filters_u8f_bc;

	if (type == MLIB_SHORT || type == MLIB_USHORT)
		pws.filter_table = (void *)mlib_filters_s16f_bc;

	if (filter == MLIB_BICUBIC2) {
		pws.flags = 1;
		filter = MLIB_BICUBIC;
		pws.filter_table = (void *)mlib_filters_u8f_bc2;

		if (type == MLIB_SHORT || type == MLIB_USHORT)
			pws.filter_table = (void *)mlib_filters_s16f_bc2;
	}

	border =
	    (filter == MLIB_BILINEAR) ? 0.5 : ((filter ==
	    MLIB_BICUBIC) ? 1.5 : 0);

	pws.SrcStartX = border;
	pws.SrcStartY = border;
	pws.SrcStopX = mlib_ImageGetWidth(src) - border;
	pws.SrcStopY = mlib_ImageGetHeight(src) - border;

	if (edge == MLIB_EDGE_SRC_PADDED) {
		mlib_u8 *paddings = mlib_ImageGetPaddings(src);
		mlib_s32 width = mlib_ImageGetWidth(src) - paddings[2];
		mlib_s32 height = mlib_ImageGetHeight(src) - paddings[3];

		if (paddings[0] > pws.SrcStartX)
			pws.SrcStartX = paddings[0];
		if (paddings[1] > pws.SrcStartY)
			pws.SrcStartY = paddings[1];
		if (width < pws.SrcStopX)
			pws.SrcStopX = width;
		if (height < pws.SrcStopY)
			pws.SrcStopY = height;
	}

	xCoeffs_new = mlib_ImagePolynimialWarpCoeffsPrepare(xCoeffs, yCoeffs,
	    preScaleX, preScaleY,
	    postScaleX, postScaleY, postShiftX, postShiftY, n);

	if (!xCoeffs_new) {
		__mlib_free(memory);
		return (MLIB_FAILURE);
	}

	yCoeffs_new = xCoeffs_new + k;

	pws.xCoeffs = xCoeffs_new;
	pws.yCoeffs = yCoeffs_new;
	pws.degree = n - 2;

	if (n < 6) {
		mlib_ImagePolynomialWarp_2_5(dst, src,
		    lineAddr, &pws,
		    xCoeffs_new, yCoeffs_new,
		    preShiftX, preShiftY, filter, edge);
		FREE_MEMORY;
		return (MLIB_SUCCESS);
	}

	pos = mlib_ImageGetHeight(src);
	for (i = 0; i < pos; i++) {
		lineAddr[i] = srcData;
		srcData += mlib_ImageGetStride(src);
	}

	if (filter == MLIB_NEAREST) {
		func_clip = array_func_clip[0];
	} else {
		if (filter == MLIB_BILINEAR)
			func_clip = array_func_clip[1];
		else {
			if (filter == MLIB_BICUBIC) {
				if (type == MLIB_INT)
					func_clip = array_func_clip[1];
				else
					func_clip = array_func_clip[2];
			}
		}
	}

	pos =
	    (filter == MLIB_NEAREST) ? 0 : ((filter == MLIB_BILINEAR) ? 4 : 8);
	pos +=
	    (type == MLIB_BYTE) ? 0 : ((type == MLIB_SHORT) ? 1 : ((type ==
	    MLIB_USHORT) ? 2 : 3));

	func = array_func[pos][channels - 1];

	for (i = 0; i < mlib_ImageGetHeight(dst); i++) {

		len = func_clip(&pws, x, preShiftX, mlib_ImageGetWidth(dst), n);

		func(dstData, lineAddr, &pws, len);
		dstData += mlib_ImageGetStride(dst);

		x += 1.0;
	}

	FREE_MEMORY;
	return (MLIB_SUCCESS);
}

/* *********************************************************** */
