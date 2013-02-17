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

#ifndef _MLIB_IMAGECHECK_H
#define	_MLIB_IMAGECHECK_H

#pragma ident	"@(#)mlib_ImageCheck.h	9.2	07/10/09 SMI"

#ifdef __cplusplus
extern "C" {
#endif

#include <mlib_types.h>
#include <mlib_image_types.h>
#include <mlib_status.h>

/* *********************************************************** */

#define	MLIB_IMAGE_CHECK(image)                                 \
	if (image == NULL)                                      \
	    return MLIB_NULLPOINTER

/* *********************************************************** */

#define	MLIB_IMAGE_SIZE_EQUAL(image1, image2)                             \
	if (mlib_ImageGetWidth(image1) != mlib_ImageGetWidth(image2) ||   \
	    mlib_ImageGetHeight(image1) != mlib_ImageGetHeight(image2))   \
	    return MLIB_FAILURE

/* *********************************************************** */

#define	MLIB_IMAGE_TYPE_EQUAL(image1, image2)                         \
	if (mlib_ImageGetType(image1) != mlib_ImageGetType(image2))   \
	    return MLIB_FAILURE

/* *********************************************************** */

#define	MLIB_IMAGE_CHAN_EQUAL(image1, image2)                   \
	if (mlib_ImageGetChannels(image1) !=                    \
	    mlib_ImageGetChannels(image2))                      \
	    return MLIB_FAILURE

/* *********************************************************** */

#define	MLIB_IMAGE_FULL_EQUAL(image1, image2)                   \
	MLIB_IMAGE_SIZE_EQUAL(image1, image2);                  \
	MLIB_IMAGE_TYPE_EQUAL(image1, image2);                  \
	MLIB_IMAGE_CHAN_EQUAL(image1, image2)

/* *********************************************************** */

#define	MLIB_IMAGE_HAVE_TYPE(image, type)                       \
	if (mlib_ImageGetType(image) != type)                   \
	    return MLIB_FAILURE

/* *********************************************************** */

#define	MLIB_IMAGE_HAVE_CHAN(image, channels)                   \
	if (mlib_ImageGetChannels(image) != channels)           \
	    return MLIB_FAILURE

/* *********************************************************** */

#define	MLIB_IMAGE_HAVE_3_OR_4_CHAN(image)                      \
	if (mlib_ImageGetChannels(image) != 3 &&                \
	    mlib_ImageGetChannels(image) != 4)                  \
	    return MLIB_FAILURE

/* *********************************************************** */

#define	MLIB_IMAGE_CHAN_SRC1_OR_EQ(src, dst)                    \
	if (mlib_ImageGetChannels(src) != 1) {                  \
	    if (mlib_ImageGetChannels(src) !=                   \
		mlib_ImageGetChannels(dst))                     \
		return (MLIB_FAILURE);                          \
	}

/* *********************************************************** */

#define	MLIB_IMAGE_TYPE_DSTBIT_OR_EQ(src, dst)                      \
	if ((mlib_ImageGetType(src) != mlib_ImageGetType(dst)) &&   \
	    (mlib_ImageGetType(dst) != MLIB_BIT)) {                 \
	    return (MLIB_FAILURE);                                  \
	}

/* *********************************************************** */

#define	MLIB_IMAGE_AND_COLORMAP_ARE_COMPAT(image, colormap)     \
	if ((mlib_ImageGetChannels(image) !=                    \
	    mlib_ImageGetLutChannels(colormap)) ||              \
	    (mlib_ImageGetLutType(colormap) !=                  \
	    mlib_ImageGetType(image))) {                        \
	    return (MLIB_FAILURE);                              \
	}

/* *********************************************************** */

#define	MLIB_IMAGE_GET_ALL_PARAMS(image, type, nchan, width,    \
	height, stride, pdata)                                  \
	type = mlib_ImageGetType(image);                        \
	nchan = mlib_ImageGetChannels(image);                   \
	width = mlib_ImageGetWidth(image);                      \
	height = mlib_ImageGetHeight(image);                    \
	stride = mlib_ImageGetStride(image);                    \
	pdata = (void *)mlib_ImageGetData(image)

/* *********************************************************** */

#ifdef __cplusplus
}
#endif

#endif /* _MLIB_IMAGECHECK_H */
