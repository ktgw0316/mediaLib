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

#pragma ident	"@(#)mlib_ImageReformat.c	9.2	07/10/09 SMI"

/*
 * FUNCTION
 *      mlib_ImageReformat    - Reformat image data buffer
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageReformat(void      **dstData,
 *                                     void      **srcData,
 *                                     mlib_s32  numBands,
 *                                     mlib_s32  xSize,
 *                                     mlib_s32  ySize,
 *                                     mlib_type dstDataType,
 *                                     mlib_s32  *dstBandoffsets,
 *                                     mlib_s32  dstScanlinestride,
 *                                     mlib_s32  dstPixelstride,
 *                                     mlib_type srcDataType,
 *                                     mlib_s32  *srcBandoffsets,
 *                                     mlib_s32  srcScanlinestride,
 *                                     mlib_s32  srcPixelstride)
 *
 * ARGUMENTS
 *      dstData            The pointer to the destination image data buffer.
 *      srcData            The pointer to the source image data buffer.
 *      numBands           The number of channels of the image data buffers.
 *      xSize              The width of the image.
 *      ySize              The height of the image.
 *      dstDataType        The data type of the dstData buffer.
 *      dstBandoffsets     The initial pixel's offsets in the dstData buffer
 *                              in terms of destination data buffer elements.
 *      dstScanlinestride  The scanline stride of the dstData buffer in terms
 *                              of destination data buffer elements.
 *      dstPixelstride     The pixel stride of the dstData buffer in terms
 *                              of destination data buffer elements.
 *      srcDataType        The data type of the srcData buffer.
 *      srcBandoffsets     The initial pixel's offsets in the srcData buffer
 *                              in terms of source data buffer elements.
 *      srcScanlinestride  The scanline stride of the srcData buffer in terms
 *                              of source data buffer elements.
 *      srcPixelstride     The pixel stride of the srcData buffer in terms
 *                              of source data buffer elements.
 *
 * RESTRICTION
 *      numBands can be 1, 2, 3, or 4.
 *
 *      The srcDataType and dstDataType can be MLIB_BYTE, MLIB_SHORT,
 *      MLIB_USHORT, MLIB_INT, MLIB_FLOAT or MLIB_DOUBLE.
 *
 * DESCRIPTION
 *
 *      The mlib_ImageReformat function copies, and casts if needed, an image
 *      from one buffer to another. The formats and data types of the two
 *      buffers might be different.
 *
 *              dstPixel[x][y][c] = (dstDataType) srcPixel[x][y][c]
 *
 *      Where the values of a pixel at position (x, y) and in channel c are:
 *
 *      srcPixel[x][y][c] = srcData[c][srcBandoffsets[c] +
 *                                      srcScanlinestride*y + srcPixelstride*x]
 *
 *      dstPixel[x][y][c] = dstData[c][dstBandoffsets[c] +
 *                                      dstScanlinestride*y + dstPixelstride*x]
 *
 *      It is the user's responsibility to make sure that the data buffers
 *      supplied are suitable for this operation.
 */

#include <mlib_image.h>
#include <mlib_ImageReformat.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageReformat = __mlib_ImageReformat

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_ImageReformat) mlib_ImageReformat
    __attribute__((weak, alias("__mlib_ImageReformat")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	REFORMAT_PARAMS2                                            \
	dstData, srcData, numBands, xSize, ySize, dstBandoffsets,   \
	    dstScanlinestride, dstPixelstride, srcBandoffsets,      \
	    srcScanlinestride, srcPixelstride

/* *********************************************************** */

#define	GET_TYPE_SIZE(size, type)                               \
	switch (type) {                                         \
	case MLIB_DOUBLE:                                       \
	    size = 8;                                           \
	    break;                                              \
	case MLIB_FLOAT:                                        \
	    size = 4;                                           \
	    break;                                              \
	case MLIB_INT:                                          \
	    size = 4;                                           \
	    break;                                              \
	case MLIB_SHORT:                                        \
	    size = 2;                                           \
	    break;                                              \
	case MLIB_USHORT:                                       \
	    size = 2;                                           \
	    break;                                              \
	case MLIB_BYTE:                                         \
	    size = 1;                                           \
	    break;                                              \
	default:                                                \
	    return (MLIB_FAILURE);                              \
	}

/* *********************************************************** */

#define	INIT_IMAGE(image, image_type, xsize, ysize, nchan,            \
	lineStride, dataSize)                                         \
	{                                                             \
	    image.type = image_type;                                  \
	    image.channels = nchan;                                   \
	    image.width = xsize;                                      \
	    image.height = ysize;                                     \
	    image.stride = lineStride * dataSize;                     \
	    image.flags = ((xsize & 0xf) << 8);                       \
/*                                                                    \
 * set width field                                                    \
 */                                                                   \
	    image.flags |= ((ysize & 0xf) << 12);                     \
/*                                                                    \
 * set height field                                                   \
 */                                                                   \
	    image.flags |= (((lineStride * dataSize) & 0xf) << 16);   \
/*                                                                    \
 * set stride field                                                   \
 */                                                                   \
	    if (lineStride != xsize) {                                \
	                                                              \
/*                                                                    \
 * not 1-d vector                                                     \
 */                                                                   \
		image.flags |= MLIB_IMAGE_ONEDVECTOR;                 \
	    }                                                         \
	}

/* *********************************************************** */

#define	SET_IMAGE_DATA(image, pdata)                            \
	image.data = pdata;                                     \
	image.flags &= ~0xff;                                   \
	image.flags |= (mlib_addr)pdata & 0xff

/* *********************************************************** */

#define	MLIB_CHECK_MIXED_IMAGE(mdata, imageData,                \
	imageBandoffsets, nchan, dataSize)                      \
	{                                                       \
	    mlib_s8 *ptr;                                       \
	    mlib_s32 i;                                         \
	                                                        \
	    ptr = mdata =                                       \
		(mlib_s8 *)imageData[0] +                       \
		dataSize * imageBandoffsets[0];                 \
	    for (i = 1; i < nchan; i++) {                       \
		ptr += dataSize;                                \
		if ((mlib_s8 *)imageData[i] +                   \
		    dataSize * imageBandoffsets[i] != ptr)      \
		    mdata = NULL;                               \
	    }                                                   \
	}

/* *********************************************************** */

mlib_status
__mlib_ImageReformat(
    void **dstData,
    const void **srcData,
    mlib_s32 numBands,
    mlib_s32 xSize,
    mlib_s32 ySize,
    mlib_type dstDataType,
    const mlib_s32 *dstBandoffsets,
    mlib_s32 dstScanlinestride,
    mlib_s32 dstPixelstride,
    mlib_type srcDataType,
    const mlib_s32 *srcBandoffsets,
    mlib_s32 srcScanlinestride,
    mlib_s32 srcPixelstride)
{
	mlib_image src_image, dst_image;
	mlib_status res;
	mlib_s32 srcDataSize, dstDataSize;
	mlib_s8 *src_mdata, *dst_mdata;
	mlib_s32 k;

	if (dstData == NULL || srcData == NULL) {
		return (MLIB_NULLPOINTER);
	}

	if ((numBands < 1) || (numBands > 4) || (xSize <= 0) || (ySize <= 0)) {
		return (MLIB_FAILURE);
	}

	GET_TYPE_SIZE(srcDataSize, srcDataType);
	GET_TYPE_SIZE(dstDataSize, dstDataType);

	if (dstPixelstride == 1 && srcPixelstride == 1) {

		INIT_IMAGE(src_image, srcDataType, xSize, ySize, 1,
		    srcScanlinestride, srcDataSize);
		INIT_IMAGE(dst_image, dstDataType, xSize, ySize, 1,
		    dstScanlinestride, dstDataSize);

		for (k = 0; k < numBands; k++) {
			SET_IMAGE_DATA(src_image,
			    (mlib_u8 *)srcData[k] +
			    srcBandoffsets[k] * srcDataSize);
			SET_IMAGE_DATA(dst_image,
			    (mlib_u8 *)dstData[k] +
			    dstBandoffsets[k] * dstDataSize);

			if (dstDataType != srcDataType) {
				res =
				    __mlib_ImageDataTypeConvert(&dst_image,
				    &src_image);
			} else {
				res = __mlib_ImageCopy(&dst_image, &src_image);
			}

			if (res != MLIB_SUCCESS)
				return (res);
		}

		return (MLIB_SUCCESS);
	}

	MLIB_CHECK_MIXED_IMAGE(dst_mdata, dstData, dstBandoffsets, numBands,
	    dstDataSize);
	MLIB_CHECK_MIXED_IMAGE(src_mdata, srcData, srcBandoffsets, numBands,
	    srcDataSize);

	if (dstPixelstride == numBands && dst_mdata != NULL &&
	    srcPixelstride == numBands && src_mdata != NULL) {

		INIT_IMAGE(src_image, srcDataType, xSize, ySize, numBands,
		    srcScanlinestride, srcDataSize);
		INIT_IMAGE(dst_image, dstDataType, xSize, ySize, numBands,
		    dstScanlinestride, dstDataSize);

		SET_IMAGE_DATA(src_image, src_mdata);
		SET_IMAGE_DATA(dst_image, dst_mdata);

		if (dstDataType != srcDataType) {
			return __mlib_ImageDataTypeConvert(&dst_image,
			    &src_image);
		} else {
			return (__mlib_ImageCopy(&dst_image, &src_image));
		}
	}

	switch (dstDataType) {
	case MLIB_BYTE:
		switch (srcDataType) {
		case MLIB_BYTE:
			mlib_ImageReformat_U8_U8(REFORMAT_PARAMS2);
			break;
		case MLIB_USHORT:
			mlib_ImageReformat_U8_U16(REFORMAT_PARAMS2);
			break;
		case MLIB_SHORT:
			mlib_ImageReformat_U8_S16(REFORMAT_PARAMS2);
			break;
		case MLIB_INT:
			mlib_ImageReformat_U8_S32(REFORMAT_PARAMS2);
			break;
		case MLIB_FLOAT:
			mlib_ImageReformat_U8_F32(REFORMAT_PARAMS2);
			break;
		case MLIB_DOUBLE:
			mlib_ImageReformat_U8_D64(REFORMAT_PARAMS2);
			break;
		default:
			return (MLIB_FAILURE);
		}

		break;

	case MLIB_SHORT:
		switch (srcDataType) {
		case MLIB_BYTE:
			mlib_ImageReformat_S16_U8(REFORMAT_PARAMS2);
			break;
		case MLIB_USHORT:
			mlib_ImageReformat_S16_U16(REFORMAT_PARAMS2);
			break;
		case MLIB_SHORT:
			mlib_ImageReformat_S16_S16(REFORMAT_PARAMS2);
			break;
		case MLIB_INT:
			mlib_ImageReformat_S16_S32(REFORMAT_PARAMS2);
			break;
		case MLIB_FLOAT:
			mlib_ImageReformat_S16_F32(REFORMAT_PARAMS2);
			break;
		case MLIB_DOUBLE:
			mlib_ImageReformat_S16_D64(REFORMAT_PARAMS2);
			break;
		default:
			return (MLIB_FAILURE);
		}

		break;

	case MLIB_USHORT:
		switch (srcDataType) {
		case MLIB_BYTE:
			mlib_ImageReformat_U16_U8(REFORMAT_PARAMS2);
			break;
		case MLIB_USHORT:
			mlib_ImageReformat_U16_U16(REFORMAT_PARAMS2);
			break;
		case MLIB_SHORT:
			mlib_ImageReformat_U16_S16(REFORMAT_PARAMS2);
			break;
		case MLIB_INT:
			mlib_ImageReformat_U16_S32(REFORMAT_PARAMS2);
			break;
		case MLIB_FLOAT:
			mlib_ImageReformat_U16_F32(REFORMAT_PARAMS2);
			break;
		case MLIB_DOUBLE:
			mlib_ImageReformat_U16_D64(REFORMAT_PARAMS2);
			break;
		default:
			return (MLIB_FAILURE);
		}

		break;

	case MLIB_INT:
		switch (srcDataType) {
		case MLIB_BYTE:
			mlib_ImageReformat_S32_U8(REFORMAT_PARAMS2);
			break;
		case MLIB_USHORT:
			mlib_ImageReformat_S32_U16(REFORMAT_PARAMS2);
			break;
		case MLIB_SHORT:
			mlib_ImageReformat_S32_S16(REFORMAT_PARAMS2);
			break;
		case MLIB_INT:
			mlib_ImageReformat_S32_S32(REFORMAT_PARAMS2);
			break;
		case MLIB_FLOAT:
			mlib_ImageReformat_S32_F32(REFORMAT_PARAMS2);
			break;
		case MLIB_DOUBLE:
			mlib_ImageReformat_S32_D64(REFORMAT_PARAMS2);
			break;
		default:
			return (MLIB_FAILURE);
		}

		break;

	case MLIB_FLOAT:
		switch (srcDataType) {
		case MLIB_BYTE:
			mlib_ImageReformat_F32_U8(REFORMAT_PARAMS2);
			break;
		case MLIB_USHORT:
			mlib_ImageReformat_F32_U16(REFORMAT_PARAMS2);
			break;
		case MLIB_SHORT:
			mlib_ImageReformat_F32_S16(REFORMAT_PARAMS2);
			break;
		case MLIB_INT:
			mlib_ImageReformat_F32_S32(REFORMAT_PARAMS2);
			break;
		case MLIB_FLOAT:
			mlib_ImageReformat_F32_F32(REFORMAT_PARAMS2);
			break;
		case MLIB_DOUBLE:
			mlib_ImageReformat_F32_D64(REFORMAT_PARAMS2);
			break;
		default:
			return (MLIB_FAILURE);
		}

		break;

	case MLIB_DOUBLE:
		switch (srcDataType) {
		case MLIB_BYTE:
			mlib_ImageReformat_D64_U8(REFORMAT_PARAMS2);
			break;
		case MLIB_USHORT:
			mlib_ImageReformat_D64_U16(REFORMAT_PARAMS2);
			break;
		case MLIB_SHORT:
			mlib_ImageReformat_D64_S16(REFORMAT_PARAMS2);
			break;
		case MLIB_INT:
			mlib_ImageReformat_D64_S32(REFORMAT_PARAMS2);
			break;
		case MLIB_FLOAT:
			mlib_ImageReformat_D64_F32(REFORMAT_PARAMS2);
			break;
		case MLIB_DOUBLE:
			mlib_ImageReformat_D64_D64(REFORMAT_PARAMS2);
			break;
		default:
			return (MLIB_FAILURE);
		}

		break;

	default:
		return (MLIB_FAILURE);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
