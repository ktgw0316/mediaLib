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

#pragma ident	"@(#)mlib_ImageCreate.c	9.4	07/10/09 SMI"

/*
 * FUNCTION
 *      mlib_ImageCreateStruct   - create image data structure
 *      mlib_ImageCreate         - create image data structure and allocate
 *                                 memory for image data
 *      mlib_ImageDelete         - delete image
 *      mlib_ImageCreateSubimage - create sub-image
 *
 *      mlib_ImageCreateRowTable - create row starts pointer table
 *      mlib_ImageDeleteRowTable - delete row starts pointer table
 *
 *      mlib_ImageSetPaddings    - set paddings for clipping box borders
 *
 *      mlib_ImageSetFormat      - set image format
 *
 * SYNOPSIS
 *        mlib_image *mlib_ImageCreateStruct(mlib_type  type,
 *                                           mlib_s32   channels,
 *                                           mlib_s32   width,
 *                                           mlib_s32   height,
 *                                           mlib_s32   stride,
 *                                           const void *data)
 *
 *        mlib_image *mlib_ImageCreate(mlib_type type,
 *                                     mlib_s32  channels,
 *                                     mlib_s32  width,
 *                                     mlib_s32  height)
 *
 *        void mlib_ImageDelete(mlib_image *img)
 *
 *        mlib_image *mlib_ImageCreateSubimage(mlib_image *img,
 *                                             mlib_s32   x,
 *                                             mlib_s32   y,
 *                                             mlib_s32   w,
 *                                             mlib_s32   h)
 *
 *        void *mlib_ImageCreateRowTable(mlib_image *img)
 *
 *        void mlib_ImageDeleteRowTable(mlib_image *img)
 *
 *        mlib_status mlib_ImageSetPaddings(mlib_image *img,
 *                                          mlib_u8    left,
 *                                          mlib_u8    top,
 *                                          mlib_u8    right,
 *                                          mlib_u8    bottom)
 *
 *        mlib_status mlib_ImageSetFormat(mlib_image  *img,
 *                                        mlib_format format)
 * ARGUMENTS
 *      img       pointer to image data structure
 *      type      image data type, one of MLIB_BIT, MLIB_BYTE, MLIB_SHORT,
 *                MLIB_USHORT, MLIB_INT, MLIB_FLOAT or MLIB_DOUBLE
 *      channels  number of image channels
 *      width     image width in pixels
 *      height    image height in pixels
 *      stride    linebytes( bytes to next row) of the image
 *      data      pointer to image data allocated by user
 *      x         x coordinate of the left border in the source image
 *      y         y coordinate of the top border in the source image
 *      w         width of the sub-image
 *      h         height of the sub-image
 *      left      clipping box left padding
 *      top       clipping box top padding
 *      right     clipping box right padding
 *      bottom    clipping box bottom padding
 *      format    image format
 *
 * DESCRIPTION
 *      mlib_ImageCreateStruct() creates a mediaLib image data structure
 *      using parameter supplied by user.
 *
 *      mlib_ImageCreate() creates a mediaLib image data structure and
 *      allocates memory space for image data.
 *
 *      mlib_ImageDelete() deletes the mediaLib image data structure
 *      and frees the memory space of the image data if it is allocated
 *      through mlib_ImageCreate().
 *
 *      mlib_ImageCreateSubimage() creates a mediaLib image structure
 *      for a sub-image based on a source image.
 *
 *      mlib_ImageCreateRowTable() creates row starts pointer table and
 *      puts it into mlib_image->state field.
 *
 *      mlib_ImageDeleteRowTable() deletes row starts pointer table from
 *      image and puts NULL into mlib_image->state field.
 *
 *      mlib_ImageSetPaddings() sets new values for the clipping box paddings
 *
 *      mlib_ImageSetFormat() sets new value for the image format
 */

/*
 * FUNCTION
 *	mlib_ImageSetStruct	- set image data structure
 *
 * SYNOPSIS
 *	mlib_status mlib_ImageSetStruct(mlib_image *img,
 *	                                mlib_type  type,
 *	                                mlib_s32   channels,
 *	                                mlib_s32   width,
 *	                                mlib_s32   height,
 *	                                mlib_s32   stride,
 *	                                const void *datbuf)
 *
 * ARGUMENTS
 *	img       pointer to image data structure
 *	type      image data type, one of MLIB_BIT, MLIB_BYTE, MLIB_SHORT,
 *	          MLIB_USHORT, MLIB_INT, MLIB_FLOAT or MLIB_DOUBLE
 *	channels  number of image channels
 *	width     image width in pixels
 *	height    image height in pixels
 *	stride    image line bytes (number of bytes to next row)
 *	datbuf    pointer to image data buffer
 *
 * RETURN VALUE
 *	MLIB_SUCCESS is returned if the image data structure is set
 *	successfully.  MLIB_FAILURE is returned when the image data
 *	structure can not be set according to the parameters supplied.
 *
 * DESCRIPTION
 *	mlib_ImageSetStruct() sets a mediaLib image data structure
 *	using parameters supplied by user.
 *
 *	mlib_ImageSetStruct() returns MLIB_FAILURE the supplied
 *	parameters do not pass following sanity checks.
 *
 *		- img should not be NULL
 *		- type should be one of MLIB_BIT, MLIB_BYTE, MLIB_SHORT,
 *			MLIB_USHORT, MLIB_INT, MLIB_FLOAT or MLIB_DOUBLE
 *		- channels should be between 1 and 4
 *		- width should be greater than 0
 *		- height should be greater than 0
 *		- stride should be no less than following
 *			width * channels * (size of type in bytes)
 *		  and stride should be a multiple of 2 when type is
 *		  MLIB_SHORT or MLIB_USHORT; 4 when type is MLIB_INT
 *		  or MLIB_FLOAT; 8 when type is MLIB_DOUBLE
 *		- datbuf should not be NULL, and datbuf should be 2-byte
 *		  aligned when type is MLIB_SHORT or MLIB_USHORT; 4-byte
 *		  aligned when type is MLIB_INT or MLIB_FLOAT; 8-byte
 *		  aligned when type is MLIB_DOUBLE
 *
 *	Whenever MLIB_FAILURE is returned, the original image data
 *	structure is not changed.
 *
 *	If the data buffer in the image data structure is not NULL,
 *	it is the user's responsibility to free it if necessary.
 */

/*
 * FUNCTION
 *	mlib_ImageResetStruct	- Reset image data structure
 *
 * SYNOPSIS
 *	mlib_status mlib_ImageResetStruct(mlib_image *img,
 *	                                  mlib_type  type,
 *	                                  mlib_s32   channels,
 *	                                  mlib_s32   width,
 *	                                  mlib_s32   height,
 *	                                  mlib_s32   stride,
 *	                                  const void *datbuf)
 *
 * ARGUMENTS
 *	img       pointer to image data structure
 *	type      image data type, one of MLIB_BIT, MLIB_BYTE, MLIB_SHORT,
 *	          MLIB_USHORT, MLIB_INT, MLIB_FLOAT or MLIB_DOUBLE
 *	channels  number of image channels
 *	width     image width in pixels
 *	height    image height in pixels
 *	stride    image line bytes (number of bytes to next row)
 *	datbuf    pointer to image data buffer
 *
 * RETURN VALUE
 *	MLIB_SUCCESS is returned if the image data structure is reset
 *	successfully.  MLIB_FAILURE is returned when the image data
 *	structure can not be reset according to the parameters supplied.
 *
 * DESCRIPTION
 *	mlib_ImageResetStruct() resets a mediaLib image data structure
 *	using parameters supplied by user.
 *
 *	mlib_ImageResetStruct() returns MLIB_FAILURE the supplied
 *	parameters do not pass following sanity checks.
 *
 *		- img should not be NULL
 *		- type should be one of MLIB_BIT, MLIB_BYTE, MLIB_SHORT,
 *			MLIB_USHORT, MLIB_INT, MLIB_FLOAT or MLIB_DOUBLE
 *		- channels should be between 1 and 4
 *		- width should be greater than 0
 *		- height should be greater than 0
 *		- stride should be no less than following
 *			width * channels * (size of type in bytes)
 *		  and stride should be a multiple of 2 when type is
 *		  MLIB_SHORT or MLIB_USHORT; 4 when type is MLIB_INT
 *		  or MLIB_FLOAT; 8 when type is MLIB_DOUBLE
 *		- if not NULL, datbuf should be 2-byte aligned when type
 *		  is MLIB_SHORT or MLIB_USHORT; 4-byte aligned when type
 *		  is MLIB_INT or MLIB_FLOAT; 8-byte aligned when type is
 *		  MLIB_DOUBLE
 *
 *	Whenever MLIB_FAILURE is returned, the original image data
 *	structure is not changed.
 *
 *	When datbuf is NULL, the original data buffer is reused.  If
 *	mlib_ImageIsUserAllocated(img)==0, such as the case the image
 *	data structure was created by mlib_ImageCreate(), and the data
 *	buffer size required by the parameters supplied is larger than
 *	the original, MLIB_FAILURE is returned.
 *
 *	When datbuf is not NULL, if mlib_ImageIsUserAllocated(img)==0,
 *	the original data buffer is freed, otherwise the original data
 *	buffer is not freed. If datbuf points to the original data
 *	buffer, it is not freed.
 */

/*
 * FUNCTION
 *	mlib_ImageSetSubimageStruct - Set sub-image data structure
 *
 * SYNOPSIS
 *	mlib_status mlib_ImageSetSubimageStruct(mlib_image *subimg,
 *                                          const mlib_image *img,
 *                                          mlib_s32   x,
 *                                          mlib_s32   y,
 *                                          mlib_s32   w,
 *                                          mlib_s32   h)
 *
 * ARGUMENTS
 *	subimg  pointer to the sub-image data structure
 *	img     pointer to the source image data structure
 *	x       x coordinate of the left border in the source image
 *	y       y coordinate of the top border in the source image
 *	w       width of the sub-image
 *	h       height of the sub-image
 *
 * RETURN VALUE
 *	MLIB_SUCCESS is returned if the image data structure is set
 *	successfully.  MLIB_FAILURE is returned when the image data
 *	structure can not be set according to the parameters supplied.
 *
 * DESCRIPTION
 *	mlib_ImageSetSubimageStruct() sets a sub-image's data
 *	structure using parameters supplied by user.
 *
 *	mlib_ImageSetSubimageStruct() returns MLIB_FAILURE if
 *	supplied parameters do not pass following sanity checks.
 *
 *		subimg != NULL
 *		img != NULL
 *		0 < w <= mlib_ImageGetWidth(img)
 *		0 < h <= mlib_ImageGetHeight(img)
 *		0 <= x <= (mlib_ImageGetWidth(img) - w)
 *		0 <= y <= (mlib_ImageGetHeight(img) - h)
 *
 *	Whenever MLIB_FAILURE is returned, the original image data
 *	structure is not changed.
 */

/*
 * FUNCTION
 *	mlib_ImageResetSubimageStruct - Reset sub-image data structure
 *
 * SYNOPSIS
 *	mlib_status mlib_ImageResetSubimageStruct(mlib_image *subimg,
 *                                            const mlib_image *img,
 *                                            mlib_s32   x,
 *                                            mlib_s32   y,
 *                                            mlib_s32   w,
 *                                            mlib_s32   h)
 *
 * ARGUMENTS
 *	subimg  pointer to the sub-image data structure
 *	img     pointer to the source image data structure
 *	x       x coordinate of the left border in the source image
 *	y       y coordinate of the top border in the source image
 *	w       width of the sub-image
 *	h       height of the sub-image
 *
 * RETURN VALUE
 *	MLIB_SUCCESS is returned if the image data structure is reset
 *	successfully.  MLIB_FAILURE is returned when the image data
 *	structure can not be reset according to the parameters supplied.
 *
 * DESCRIPTION
 *	mlib_ImageResetSubimageStruct() resets a sub-image's data
 *	structure using parameters supplied by user.
 *
 *	mlib_ImageResetSubimageStruct() returns MLIB_FAILURE if
 *	supplied parameters do not pass following sanity checks.
 *
 *		subimg != NULL
 *		img != NULL
 *		0 < w <= mlib_ImageGetWidth(img)
 *		0 < h <= mlib_ImageGetHeight(img)
 *		0 <= x <= (mlib_ImageGetWidth(img) - w)
 *		0 <= y <= (mlib_ImageGetHeight(img) - h)
 *
 *	Whenever MLIB_FAILURE is returned, the original image data
 *	structure is not changed.
 *
 *	If mlib_ImageIsUserAllocated(subimg)==0, the original data buffer
 *	is freed, otherwise the original data buffer is not freed.
 */

#include <stdlib.h>
#include <mlib_image.h>
#include <mlib_ImageRowTable.h>
#include <mlib_ImageCreate.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageCreate = __mlib_ImageCreate
#pragma weak mlib_ImageCreateStruct = __mlib_ImageCreateStruct
#pragma weak mlib_ImageCreateSubimage = __mlib_ImageCreateSubimage
#pragma weak mlib_ImageDelete = __mlib_ImageDelete
#pragma weak mlib_ImageSetFormat = __mlib_ImageSetFormat
#pragma weak mlib_ImageSetPaddings = __mlib_ImageSetPaddings
#pragma weak mlib_ImageSetStruct = __mlib_ImageSetStruct
#pragma weak mlib_ImageResetStruct = __mlib_ImageResetStruct
#pragma weak mlib_ImageSetSubimageStruct = __mlib_ImageSetSubimageStruct
#pragma weak mlib_ImageResetSubimageStruct = __mlib_ImageResetSubimageStruct

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_ImageCreate) mlib_ImageCreate
    __attribute__((weak, alias("__mlib_ImageCreate")));
__typeof__(__mlib_ImageCreateStruct) mlib_ImageCreateStruct
    __attribute__((weak, alias("__mlib_ImageCreateStruct")));
__typeof__(__mlib_ImageCreateSubimage) mlib_ImageCreateSubimage
    __attribute__((weak, alias("__mlib_ImageCreateSubimage")));
__typeof__(__mlib_ImageDelete) mlib_ImageDelete
    __attribute__((weak, alias("__mlib_ImageDelete")));
__typeof__(__mlib_ImageSetFormat) mlib_ImageSetFormat
    __attribute__((weak, alias("__mlib_ImageSetFormat")));
__typeof__(__mlib_ImageSetPaddings) mlib_ImageSetPaddings
    __attribute__((weak, alias("__mlib_ImageSetPaddings")));
__typeof__(__mlib_ImageSetStruct) mlib_ImageSetStruct
    __attribute__((weak, alias("__mlib_ImageSetStruct")));
__typeof__(__mlib_ImageResetStruct) mlib_ImageResetStruct
    __attribute__((weak, alias("__mlib_ImageResetStruct")));
__typeof__(__mlib_ImageSetSubimageStruct) mlib_ImageSetSubimageStruct
    __attribute__((weak, alias("__mlib_ImageSetSubimageStruct")));
__typeof__(__mlib_ImageResetSubimageStruct) mlib_ImageResetSubimageStruct
    __attribute__((weak, alias("__mlib_ImageResetSubimageStruct")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

mlib_image *
mlib_ImageSet(
    mlib_image *image,
    mlib_type type,
    mlib_s32 channels,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s32 stride,
    const void *data)
{
/* width in bytes */
	mlib_s32 wb;

/* mask for check of stride */
	mlib_s32 mask;

	if (image == NULL)
		return (NULL);

/* for some ugly functions calling with incorrect parameters */
	image->type = type;
	image->channels = channels;
	image->width = width;
	image->height = height;
	image->stride = stride;
	image->data = (void *)data;
	image->state = NULL;
	image->format = MLIB_FORMAT_UNKNOWN;

	image->paddings[0] = 0;
	image->paddings[1] = 0;
	image->paddings[2] = 0;
	image->paddings[3] = 0;

	image->bitoffset = 0;

	if (width <= 0 || height <= 0 || channels < 1 || channels > 4) {
		return (NULL);
	}

/*
 * Check if stride == width
 * If it is then image can be treated as a 1-D vector
 */
	switch (type) {
	case MLIB_DOUBLE:
		wb = width * channels * 8;
		mask = 7;
		break;
	case MLIB_FLOAT:
	case MLIB_INT:
		wb = width * channels * 4;
		mask = 3;
		break;
	case MLIB_USHORT:
	case MLIB_SHORT:
		wb = width * channels * 2;
		mask = 1;
		break;
	case MLIB_BYTE:
		wb = width * channels;
		mask = 0;
		break;
	case MLIB_BIT:
		wb = (width * channels + 7) / 8;
		mask = 0;
		break;
	default:
		return (NULL);
	}

	if (stride & mask) {
		return (NULL);
	}

/* set width field */
	image->flags = ((width & 0xf) << 8);
/* set stride field */
	image->flags |= ((stride & 0xf) << 16);
/* set height field */
	image->flags |= ((height & 0xf) << 12);
	image->flags |= (mlib_addr)data & 0xff;
/* user allocated data */
	image->flags |= MLIB_IMAGE_USERALLOCATED;

	if ((stride != wb) ||
	    ((type == MLIB_BIT) && (stride * 8 != width * channels))) {
		image->flags |= MLIB_IMAGE_ONEDVECTOR;
	}

	image->flags &= MLIB_IMAGE_ATTRIBUTESET;

	return (image);
}

/* *********************************************************** */

mlib_image *
__mlib_ImageCreateStruct(
    mlib_type type,
    mlib_s32 channels,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s32 stride,
    const void *data)
{
	mlib_image *image;

	if (stride <= 0) {
		return (NULL);
	}

	image = (mlib_image *)__mlib_malloc(sizeof (mlib_image));

	if (image == NULL) {
		return (NULL);
	}

	if (mlib_ImageSet(image, type, channels, width, height, stride,
	    data) == NULL) {
		__mlib_free(image);
		image = NULL;
	}

	return (image);
}

/* *********************************************************** */

mlib_image *
__mlib_ImageCreate(
    mlib_type type,
    mlib_s32 channels,
    mlib_s32 width,
    mlib_s32 height)
{
	mlib_image *image;

/* width in bytes */
	mlib_s32 wb;
	void *data;

/* sanity check */

	if (width <= 0 || height <= 0 || channels < 1 || channels > 4) {
		return (NULL);
	};

	switch (type) {
	case MLIB_DOUBLE:
		wb = width * channels * 8;
		break;
	case MLIB_FLOAT:
	case MLIB_INT:
		wb = width * channels * 4;
		break;
	case MLIB_USHORT:
	case MLIB_SHORT:
		wb = width * channels * 2;
		break;
	case MLIB_BYTE:
		wb = width * channels;
		break;
	case MLIB_BIT:
		wb = (width * channels + 7) / 8;
		break;
	default:
		return (NULL);
	}

	data = __mlib_malloc(wb * (size_t)height);

	if (data == NULL) {
		return (NULL);
	}

	image = (mlib_image *)__mlib_malloc(sizeof (mlib_image));

	if (image == NULL) {
		__mlib_free(data);
		return (NULL);
	};

	image->type = type;
	image->channels = channels;
	image->width = width;
	image->height = height;
	image->stride = wb;
	image->data = data;
/* set width field */
	image->flags = ((width & 0xf) << 8);
/* set height field */
	image->flags |= ((height & 0xf) << 12);
/* set stride field */
	image->flags |= ((wb & 0xf) << 16);
	image->flags |= (mlib_addr)data & 0xff;
	image->format = MLIB_FORMAT_UNKNOWN;

	image->paddings[0] = 0;
	image->paddings[1] = 0;
	image->paddings[2] = 0;
	image->paddings[3] = 0;

	image->bitoffset = 0;

	if ((type == MLIB_BIT) && (wb * 8 != width * channels)) {
/* not 1-d vector */
		image->flags |= MLIB_IMAGE_ONEDVECTOR;
	}

	image->flags &= MLIB_IMAGE_ATTRIBUTESET;
	image->state = NULL;

	return (image);
}

/* *********************************************************** */

void
__mlib_ImageDelete(
    mlib_image *img)
{
	if (img == NULL)
		return;
	if ((img->flags & MLIB_IMAGE_USERALLOCATED) == 0) {
		__mlib_free(img->data);
	}

	mlib_ImageDeleteRowTable(img);
	__mlib_free(img);
}

/* *********************************************************** */

mlib_image *
__mlib_ImageCreateSubimage(
    mlib_image *img,
    mlib_s32 x,
    mlib_s32 y,
    mlib_s32 w,
    mlib_s32 h)
{
	mlib_image *subimage;
	mlib_type type;
	mlib_s32 channels;

/* for parent image */
	mlib_s32 width;

/* for parent image */
	mlib_s32 height;
	mlib_s32 stride;
	mlib_s32 bitoffset = 0;
	void *data;

/* sanity check */

	if (w <= 0 || h <= 0 || img == NULL)
		return (NULL);

	type = img->type;
	channels = img->channels;
	width = img->width;
	height = img->height;
	stride = img->stride;

/* clip the sub-image with respect to the parent image */

	if (((x + w) <= 0) || ((y + h) <= 0) || (x >= width) || (y >= height)) {
		return (NULL);
	} else {
		if (x < 0) {
/* x is negative */
			w += x;
			x = 0;
		}

		if (y < 0) {
/* y is negative */
			h += y;
			y = 0;
		}

		if ((x + w) > width) {
			w = width - x;
		}

		if ((y + h) > height) {
			h = height - y;
		}
	}

/* compute sub-image origin */
	data = (mlib_u8 *)(img->data) + y * stride;

	switch (type) {
	case MLIB_DOUBLE:
		data = (mlib_u8 *)data + x * channels * 8;
		break;
	case MLIB_FLOAT:
	case MLIB_INT:
		data = (mlib_u8 *)data + x * channels * 4;
		break;
	case MLIB_USHORT:
	case MLIB_SHORT:
		data = (mlib_u8 *)data + x * channels * 2;
		break;
	case MLIB_BYTE:
		data = (mlib_u8 *)data + x * channels;
		break;
	case MLIB_BIT:
		bitoffset = img->bitoffset;
		data = (mlib_u8 *)data + (x * channels + bitoffset) / 8;
		bitoffset = (x * channels + bitoffset) & 7;
		break;
	default:
		return (NULL);
	}

	subimage = __mlib_ImageCreateStruct(type, channels, w, h, stride, data);

	if (subimage != NULL && type == MLIB_BIT)
		subimage->bitoffset = bitoffset;

	return (subimage);
}

/* *********************************************************** */

mlib_image *
mlib_ImageSetSubimage(
    mlib_image *dst,
    const mlib_image *src,
    mlib_s32 x,
    mlib_s32 y,
    mlib_s32 w,
    mlib_s32 h)
{
	mlib_type type = src->type;
	mlib_s32 channels = src->channels;
	mlib_s32 stride = src->stride;
	mlib_u8 *data = src->data;
	mlib_s32 bitoffset = 0;

	data += y * stride;

	switch (type) {
	case MLIB_DOUBLE:
		data += channels * x * 8;
		break;
	case MLIB_FLOAT:
	case MLIB_INT:
		data += channels * x * 4;
		break;
	case MLIB_USHORT:
	case MLIB_SHORT:
		data += channels * x * 2;
		break;
	case MLIB_BYTE:
		data += channels * x;
		break;
	case MLIB_BIT:
		bitoffset = src->bitoffset + channels * x;
/* with rounding toward -Inf */
		data += (bitoffset >= 0) ? bitoffset / 8 : (bitoffset - 7) / 8;
		bitoffset &= 7;
		break;
	default:
		return (NULL);
	}

	if (h > 0) {
		dst = mlib_ImageSet(dst, type, channels, w, h, stride, data);
	} else {
		h = -h;
		dst =
		    mlib_ImageSet(dst, type, channels, w, h, -stride,
		    data + (h - 1) * stride);
	}

	if (dst != NULL && type == MLIB_BIT) {
		dst->bitoffset = bitoffset;
	}

	return (dst);
}

/* *********************************************************** */

void *
mlib_ImageCreateRowTable(
    mlib_image *img)
{
	mlib_u8 **rtable, *tline;
	mlib_s32 i, im_height, im_stride;

	if (img == NULL)
		return (NULL);
	if (img->state)
		return (img->state);

	im_height = mlib_ImageGetHeight(img);
	im_stride = mlib_ImageGetStride(img);
	tline = mlib_ImageGetData(img);
	rtable = __mlib_malloc((3 + im_height) * sizeof (mlib_u8 *));

	if (rtable == NULL || tline == NULL)
		return (NULL);

	rtable[0] = 0;
	rtable[1] = (mlib_u8 *)((void **)rtable + 1);
	rtable[2 + im_height] = (mlib_u8 *)((void **)rtable + 1);
	for (i = 0; i < im_height; i++) {
		rtable[i + 2] = tline;
		tline += im_stride;
	}

	img->state = ((void **)rtable + 2);
	return (img->state);
}

/* *********************************************************** */

void
mlib_ImageDeleteRowTable(
    mlib_image *img)
{
	void **state;

	if (img == NULL)
		return;

	state = img->state;

	if (!state)
		return;

	__mlib_free(state - 2);
	img->state = 0;
}

/* *********************************************************** */

mlib_status
__mlib_ImageSetPaddings(
    mlib_image *img,
    mlib_u8 left,
    mlib_u8 top,
    mlib_u8 right,
    mlib_u8 bottom)
{
	if (img == NULL)
		return (MLIB_FAILURE);

	if ((left + right) >= img->width || (top + bottom) >= img->height)
		return (MLIB_OUTOFRANGE);

	img->paddings[0] = left;
	img->paddings[1] = top;
	img->paddings[2] = right;
	img->paddings[3] = bottom;

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_ImageSetFormat(
    mlib_image *img,
    mlib_format format)
{
	if (img == NULL)
		return (MLIB_FAILURE);

	img->format = format;

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_ImageSetStruct(
    mlib_image *img,
    mlib_type type,
    mlib_s32 channels,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s32 stride,
    const void *datbuf)
{
/* width in bytes */
	mlib_s32 wb;

/* mask for check of stride */
	mlib_s32 mask;

/* Check parameters */
	if ((img == NULL) ||
	    (width <= 0) || (height <= 0) ||
	    (channels < 1) || (channels > 4) ||
	    (datbuf == NULL)) {
		return (MLIB_FAILURE);
	}

	switch (type) {
	case MLIB_DOUBLE:
		wb = width * channels * 8;
		mask = 7;
		break;
	case MLIB_FLOAT:
	case MLIB_INT:
		wb = width * channels * 4;
		mask = 3;
		break;
	case MLIB_USHORT:
	case MLIB_SHORT:
		wb = width * channels * 2;
		mask = 1;
		break;
	case MLIB_BYTE:
		wb = width * channels;
		mask = 0;
		break;
	case MLIB_BIT:
		wb = (width * channels + 7) / 8;
		mask = 0;
		break;
	default:
		return (MLIB_FAILURE);
	}

	if (((mlib_addr)datbuf & mask) ||
	    (stride < wb) || (stride & mask)) {
		return (MLIB_FAILURE);
	}

/* Set image data structure */
	img->type = type;
	img->channels = channels;
	img->width = width;
	img->height = height;
	img->stride = stride;
	img->data = (void *)datbuf;
	img->state = NULL;
	img->format = MLIB_FORMAT_UNKNOWN;

	img->paddings[0] = 0;
	img->paddings[1] = 0;
	img->paddings[2] = 0;
	img->paddings[3] = 0;

	img->bitoffset = 0;

/* set width field */
	img->flags = ((width & 0xf) << 8);
/* set stride field */
	img->flags |= ((stride & 0xf) << 16);
/* set height field */
	img->flags |= ((height & 0xf) << 12);
	img->flags |= (mlib_addr)datbuf & 0xff;
/* user allocated data */
	img->flags |= MLIB_IMAGE_USERALLOCATED;

	if ((stride != wb) ||
	    ((type == MLIB_BIT) &&
	    ((stride * 8) != (width * channels)))) {
		img->flags |= MLIB_IMAGE_ONEDVECTOR;
	}

	img->flags &= MLIB_IMAGE_ATTRIBUTESET;

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_ImageResetStruct(
    mlib_image *img,
    mlib_type type,
    mlib_s32 channels,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s32 stride,
    const void *datbuf)
{
/* original data buffer size */
	mlib_s32 pbufsize;

/* required data buffer size */
	mlib_s32 bufsize;

/* width in bytes */
	mlib_s32 wb;

/* mask for check of stride */
	mlib_s32 mask;

/* Check parameters */
	if ((img == NULL) ||
	    (width <= 0) || (height <= 0) ||
	    (channels < 1) || (channels > 4)) {
		return (MLIB_FAILURE);
	}

	switch (type) {
	case MLIB_DOUBLE:
		wb = width * channels * 8;
		mask = 7;
		break;
	case MLIB_FLOAT:
	case MLIB_INT:
		wb = width * channels * 4;
		mask = 3;
		break;
	case MLIB_USHORT:
	case MLIB_SHORT:
		wb = width * channels * 2;
		mask = 1;
		break;
	case MLIB_BYTE:
		wb = width * channels;
		mask = 0;
		break;
	case MLIB_BIT:
		wb = (width * channels + 7) / 8;
		mask = 0;
		break;
	default:
		return (MLIB_FAILURE);
	}

	if ((stride < wb) || (stride & mask)) {
		return (MLIB_FAILURE);
	}

	if (datbuf == NULL) {		/* original data buffer is reused */
		if (mask & (mlib_addr)img->data) {
			return (MLIB_FAILURE);
		}

		if ((img->flags & MLIB_IMAGE_USERALLOCATED) == 0) {
		/* not user allocated */
			bufsize = stride * height;
			pbufsize = (img->stride) * (img->height);

			if (bufsize > pbufsize) {
				return (MLIB_FAILURE);
			}
		}
	} else {
		if (mask & (mlib_addr)datbuf) {
			return (MLIB_FAILURE);
		}

		if (img->data != datbuf) {
			if ((img->flags & MLIB_IMAGE_USERALLOCATED) == 0) {
			/* free original data buffer */
				__mlib_free(img->data);
			}

			img->data = (void *)datbuf;
		}
	} /* end if(datbuf == NULL) */

/* Reset image data structure */
	img->type = type;
	img->channels = channels;
	img->width = width;
	img->height = height;
	img->stride = stride;
	img->state = NULL;
	img->format = MLIB_FORMAT_UNKNOWN;

	img->paddings[0] = 0;
	img->paddings[1] = 0;
	img->paddings[2] = 0;
	img->paddings[3] = 0;

	img->bitoffset = 0;

	if (datbuf == NULL) {
		img->flags = img->flags & MLIB_IMAGE_USERALLOCATED;
		img->flags |= (mlib_addr)img->data & 0xff;
	} else {
		img->flags = MLIB_IMAGE_USERALLOCATED;
		img->flags |= (mlib_addr)datbuf & 0xff;
	}

/* set width field */
	img->flags |= ((width & 0xf) << 8);
/* set stride field */
	img->flags |= ((stride & 0xf) << 16);
/* set height field */
	img->flags |= ((height & 0xf) << 12);

	if ((stride != wb) ||
	    ((type == MLIB_BIT) &&
	    (stride * 8 != (width * channels)))) {
		img->flags |= MLIB_IMAGE_ONEDVECTOR;
	}

	img->flags &= MLIB_IMAGE_ATTRIBUTESET;

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_ImageSetSubimageStruct(
    mlib_image *subimg,
    const mlib_image *img,
    mlib_s32 x,
    mlib_s32 y,
    mlib_s32 w,
    mlib_s32 h)
{
/* for parent image */
	mlib_s32 width;
	mlib_s32 height;
	mlib_type type;
	mlib_s32 channels;
	mlib_s32 stride;
	mlib_s32 bitoffset = 0;
	void *data;
	mlib_status ret;

/* sanity check */
	if ((subimg == NULL) || (img == NULL) ||
	    (w <= 0) || (h <= 0) ||
	    (x < 0) || (y < 0)) {
		return (MLIB_FAILURE);
	}

	width = img->width;
	height = img->height;

	if ((w > width) || (h > height) ||
	    (x > (width - w)) || (y > (height - h))) {
		return (MLIB_FAILURE);
	}

	type = img->type;
	channels = img->channels;
	stride = img->stride;

/* compute sub-image origin */
	data = (mlib_u8 *)(img->data) + y * stride;

	switch (type) {
	case MLIB_DOUBLE:
		data = (mlib_u8 *)data + x * channels * 8;
		break;
	case MLIB_FLOAT:
	case MLIB_INT:
		data = (mlib_u8 *)data + x * channels * 4;
		break;
	case MLIB_USHORT:
	case MLIB_SHORT:
		data = (mlib_u8 *)data + x * channels * 2;
		break;
	case MLIB_BYTE:
		data = (mlib_u8 *)data + x * channels;
		break;
	case MLIB_BIT:
		bitoffset = img->bitoffset;
		data = (mlib_u8 *)data + (x * channels + bitoffset) / 8;
		bitoffset = (x * channels + bitoffset) & 7;
		break;
	default:
		return (MLIB_FAILURE);
	}

	ret = __mlib_ImageSetStruct(
		subimg, type, channels, w, h, stride, data);

	if (ret == MLIB_SUCCESS && subimg != NULL && type == MLIB_BIT)
		subimg->bitoffset = bitoffset;

	return (ret);
}

/* *********************************************************** */

mlib_status
__mlib_ImageResetSubimageStruct(
    mlib_image *subimg,
    const mlib_image *img,
    mlib_s32 x,
    mlib_s32 y,
    mlib_s32 w,
    mlib_s32 h)
{
/* for parent image */
	mlib_s32 width;
	mlib_s32 height;
	mlib_type type;
	mlib_s32 channels;
	mlib_s32 stride;
	mlib_s32 bitoffset = 0;
	void *data;
	mlib_status ret;

/* sanity check */
	if ((subimg == NULL) || (img == NULL) ||
	    (w <= 0) || (h <= 0) ||
	    (x < 0) || (y < 0)) {
		return (MLIB_FAILURE);
	}

	width = img->width;
	height = img->height;

	if ((w > width) || (h > height) ||
	    (x > (width - w)) || (y > (height - h))) {
		return (MLIB_FAILURE);
	}

	type = img->type;
	channels = img->channels;
	stride = img->stride;

/* compute sub-image origin */
	data = (mlib_u8 *)(img->data) + y * stride;

	switch (type) {
	case MLIB_DOUBLE:
		data = (mlib_u8 *)data + x * channels * 8;
		break;
	case MLIB_FLOAT:
	case MLIB_INT:
		data = (mlib_u8 *)data + x * channels * 4;
		break;
	case MLIB_USHORT:
	case MLIB_SHORT:
		data = (mlib_u8 *)data + x * channels * 2;
		break;
	case MLIB_BYTE:
		data = (mlib_u8 *)data + x * channels;
		break;
	case MLIB_BIT:
		bitoffset = img->bitoffset;
		data = (mlib_u8 *)data + (x * channels + bitoffset) / 8;
		bitoffset = (x * channels + bitoffset) & 7;
		break;
	default:
		return (MLIB_FAILURE);
	}

	ret = __mlib_ImageResetStruct(
		subimg, type, channels, w, h, stride, data);

	if (ret == MLIB_SUCCESS && subimg != NULL && type == MLIB_BIT)
		subimg->bitoffset = bitoffset;

	return (ret);
}

/* *********************************************************** */
