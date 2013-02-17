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

#pragma ident	"@(#)mlib_ImagePolynomialWarpTable.c	9.2	07/10/09 SMI"

/*
 * FUNCTION
 *
 *      mlib_ImagePolynomialWarpTable    - polynomial-based image warp
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_ImagePolynomialWarpTable(mlib_image       *dst,
 *                                                const mlib_image *src,
 *                                                const mlib_d64   *xCoeffs,
 *                                                const mlib_d64   *yCoeffs,
 *                                                mlib_s32         n,
 *                                                mlib_d64         preShiftX,
 *                                                mlib_d64         preShiftY,
 *                                                mlib_d64         postShiftX,
 *                                                mlib_d64         postShiftY,
 *                                                mlib_d64         preScaleX,
 *                                                mlib_d64         preScaleY,
 *                                                mlib_d64         postScaleX,
 *                                                mlib_d64         postScaleY,
 *                                                const void    *interp_table,
 *                                                mlib_edge        edge)
 *
 * ARGUMENTS
 *
 *      dst             The pointer to destination image.
 *      src             The pointer to source image.
 *      xCoeffs         The destination to source transform coefficients for
 *                          the X coordinate.
 *      yCoeffs         The destination to source transform coefficients for
 *                          the Y coordinate.
 *      n               The degree power of the polynomial.
 *      preShiftX       The displacement to apply to destination X positions.
 *      preShiftY       The displacement to apply to destination Y positions.
 *      postShiftX      The displacement to apply to source X positions.
 *      postShiftY      The displacement to apply to source Y positions.
 *      preScaleX       The scale factor to apply to destination X positions.
 *      preScaleY       The scale factor to apply to destination Y positions.
 *      postScaleX      The scale factor to apply to source X positions.
 *      postScaleY      The scale factor to apply to source Y positions.
 *      interp_table    Pointer to interpolation table structure.
 *      edge            The type of edge condition.
 *
 * RESTRICTION
 *
 *      The images must have the same type, and the same number of channels.
 *      The images can have 1, 2, 3, or 4 channels.
 *      The images can be in MLIB_BYTE, MLIB_SHORT, MLIB_USHORT or MLIB_INT
 *      data type.
 *      The two images might have different sizes.
 *      src image can not have width or height larger than 32767.
 *
 *      The xCoeffs and yCoeffs parameters must contain the same number of
 *      coefficients of the form (n + 1)(n + 2)/2 for some n, where n is the
 *      non-negative degree power of the polynomial. The coefficients,
 *      in order, are associated with the terms:
 *
 *      1, x, y, x^2, x*y, y^2, ..., x^n, x^(n - 1)*y, ..., x*y^(n - 1), y^n
 *
 *      and coefficients of value 0 can not be omitted.
 *
 *      The image pixels are assumed to be centered at .5 coordinate points,
 *      e.g., the upper-left corner pixel of an image is located at (0.5, 0.5).
 *
 *      The edge condition can be one of the following:
 *              MLIB_EDGE_DST_NO_WRITE  (default)
 *              MLIB_EDGE_SRC_PADDED
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
#include <mlib_ImageInterpTable.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImagePolynomialWarpTable = __mlib_ImagePolynomialWarpTable

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_ImagePolynomialWarpTable) mlib_ImagePolynomialWarpTable
    __attribute__((weak, alias("__mlib_ImagePolynomialWarpTable")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

mlib_status
__mlib_ImagePolynomialWarpTable(
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
    const void *interp_table,
    mlib_edge edge)
{
	void *memory;
	mlib_u8 *dstData;
	mlib_u8 *srcData;
	const mlib_u8 **lineAddr;
	mlib_d64 x = 0.5 + preShiftY;
	mlib_d64 *xCoeffs_new;
	mlib_d64 *yCoeffs_new;
	mlib_s32 i, len, pos;
	mlib_s32 k = (n + 1) * (n + 2) / 2;
	mlib_s32 wkernel = mlib_ImageGetInterpWidth(interp_table);
	mlib_s32 hkernel = mlib_ImageGetInterpHeight(interp_table);
	mlib_PWS pws;
	mlib_IPWClipLine func_clip;
	mlib_IPWClipLine array_func_clip[] = {
/* degree = 0 */
		NULL,
/* degree = 1 */
		mlib_ImagePolynomialWarpClipLine_DG_1_10,
/* degree = 2 */
		mlib_ImagePolynomialWarpClipLine_DG_2_10,
/* degree = 3 */
		mlib_ImagePolynomialWarpClipLine_DG_3_10,
/* degree = 4 */
		mlib_ImagePolynomialWarpClipLine_DG_4_10,
/* degree = 5 */
		mlib_ImagePolynomialWarpClipLine_DG_5_10,
		mlib_ImagePolynomialWarpClipLine_10	/* degree > 5 */
	};
	mlib_type itype;
	mlib_s32 type;

	if (n < 0)
		return (MLIB_FAILURE);

/* check for obvious errors */
	MLIB_IMAGE_CHECK(src);
	MLIB_IMAGE_CHECK(dst);
	MLIB_IMAGE_TYPE_EQUAL(src, dst);
	MLIB_IMAGE_CHAN_EQUAL(src, dst);

	itype = mlib_ImageGetType(src);
	type = (itype == MLIB_BYTE) ? 1 :
	    (itype == MLIB_SHORT || itype == MLIB_USHORT ? 2 : 4);

	if (itype != MLIB_BYTE &&
	    itype != MLIB_SHORT && itype != MLIB_USHORT && itype != MLIB_INT)
		return (MLIB_FAILURE);

	if (mlib_ImageGetWidth(src) >= (1 << 15) ||
	    mlib_ImageGetHeight(src) >= (1 << 15)) {
		return (MLIB_FAILURE);
	}

	dstData = mlib_ImageGetData(dst);
	srcData = mlib_ImageGetData(src);

	if (n == 0) {
		return mlib_ImagePolynomialWarpTable_0(dst, src,
		    xCoeffs, yCoeffs, postShiftX,
		    postShiftY, postScaleX, postScaleY, interp_table, edge);
	}

	len = mlib_ImageGetWidth(dst) + 8;
	len = (len >> 3) + ((len & 7) != 0);
	len <<= 3;
	memory = __mlib_malloc((sizeof (mlib_s32) * 3 +
	    sizeof (mlib_d64) * 2 +
	    sizeof (mlib_d64) * 8) * len +
	    sizeof (void *) * (mlib_ImageGetHeight(src) + 1));

	if (!memory)
		return (MLIB_FAILURE);

	pws.buffer = memory;
	pws.dsrc_x = (mlib_d64 *)pws.buffer + 8 * len;
	pws.dsrc_y = (mlib_d64 *)pws.dsrc_x + len;
	lineAddr = (void *)((mlib_d64 *)pws.dsrc_y + len);
	pws.src_x = (void *)(lineAddr + mlib_ImageGetHeight(src) + 1);
	pws.src_y = (mlib_s32 *)pws.src_x + len;
	pws.dst_x = (mlib_s32 *)pws.src_y + len;
	pws.filter_table = (void *)interp_table;

	pws.SrcStartX = mlib_ImageGetInterpLeftPadding(interp_table) + 0.5;
	pws.SrcStartY = mlib_ImageGetInterpTopPadding(interp_table) + 0.5;
	pws.SrcStopX = mlib_ImageGetWidth(src) - (wkernel - pws.SrcStartX);
	pws.SrcStopY = mlib_ImageGetHeight(src) - (hkernel - pws.SrcStartY);
	pws.channels = mlib_ImageGetChannels(dst);
	pws.scaleH = 1 << mlib_ImageGetInterpSubsampleBitsH(interp_table);
	pws.scaleV = 1 << mlib_ImageGetInterpSubsampleBitsV(interp_table);
	pws.tableH = mlib_ImageGetInterpDoubleDataH(interp_table);
	pws.tableV = mlib_ImageGetInterpDoubleDataV(interp_table);
	pws.shiftH = mlib_ImageGetInterpWidthBits(interp_table);
	pws.shiftV = mlib_ImageGetInterpHeightBits(interp_table);

	srcData -= mlib_ImageGetInterpLeftPadding(interp_table) *
	    mlib_ImageGetChannels(dst) * type;

	pos = mlib_ImageGetHeight(src);
	for (i = 0; i < pos; i++) {
		lineAddr[i] = srcData;
		srcData += mlib_ImageGetStride(src);
	}

	lineAddr -= mlib_ImageGetInterpTopPadding(interp_table);

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

	func_clip = array_func_clip[n < 6 ? n : 6];

	type = (itype == MLIB_BYTE) ? 0 :
	    ((itype == MLIB_SHORT) ? 1 : ((itype == MLIB_USHORT) ? 2 : 3));

	pws.list_call_function = mlib_IPWT_get_call_func(wkernel, hkernel,
	    type, &pws.call_count);

	if (!pws.list_call_function) {
		__mlib_free(memory);
		__mlib_free(xCoeffs_new);
		return (MLIB_FAILURE);
	}

	pos = mlib_ImageGetHeight(dst);
	for (i = 0; i < pos; i++) {

		len = func_clip(&pws, x, preShiftX, mlib_ImageGetWidth(dst), n);

		mlib_ImagePolynomialWarpTable_Line(dstData, lineAddr, &pws,
		    len);
		dstData += mlib_ImageGetStride(dst);

		x += 1.0;
	}

	__mlib_free(memory);
	__mlib_free(xCoeffs_new);
	__mlib_free(pws.list_call_function);
	return (MLIB_SUCCESS);
}

/* *********************************************************** */
