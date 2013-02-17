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

#pragma ident	"@(#)mlib_ImageClear_Fp.c	9.2	07/10/09 SMI"

/*
 * FUNCTIONS
 *      mlib_ImageClear_Fp - Clear an image to a specific color.
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageClear_Fp(mlib_image     *img,
 *                                     const mlib_d64 *color);
 *
 * ARGUMENT
 *      img     Pointer to an image.
 *      color   Pointer to the color that the image is set to.
 *
 * RESTRICTION
 *      img can have 1, 2, 3 or 4 channels of
 *      MLIB_FLOAT or MLIB_DOUBLE data type.
 *
 * DESCRIPTION
 *      Clear an image to a specific color.
 */

#include <mlib_image.h>
#include <mlib_ImageCheck.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageClear_Fp = __mlib_ImageClear_Fp

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_ImageClear_Fp) mlib_ImageClear_Fp
    __attribute__((weak, alias("__mlib_ImageClear_Fp")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

static void mlib_ImageClear_Fp_F32_1(
    mlib_image *img,
    const mlib_d64 *color);
static void mlib_ImageClear_Fp_F32_2(
    mlib_image *img,
    const mlib_d64 *color);
static void mlib_ImageClear_Fp_F32_3(
    mlib_image *img,
    const mlib_d64 *color);
static void mlib_ImageClear_Fp_F32_4(
    mlib_image *img,
    const mlib_d64 *color);

static void mlib_ImageClear_Fp_D64_1(
    mlib_image *img,
    const mlib_d64 *color);
static void mlib_ImageClear_Fp_D64_2(
    mlib_image *img,
    const mlib_d64 *color);
static void mlib_ImageClear_Fp_D64_3(
    mlib_image *img,
    const mlib_d64 *color);
static void mlib_ImageClear_Fp_D64_4(
    mlib_image *img,
    const mlib_d64 *color);

/* *********************************************************** */

mlib_status
__mlib_ImageClear_Fp(
    mlib_image *img,
    const mlib_d64 *color)
{
	MLIB_IMAGE_CHECK(img);

	switch (mlib_ImageGetType(img)) {

	case MLIB_FLOAT:

		switch (mlib_ImageGetChannels(img)) {

		case 1:
			mlib_ImageClear_Fp_F32_1(img, color);
			break;

		case 2:
			mlib_ImageClear_Fp_F32_2(img, color);
			break;

		case 3:
			mlib_ImageClear_Fp_F32_3(img, color);
			break;

		case 4:
			mlib_ImageClear_Fp_F32_4(img, color);
			break;

		default:
			return (MLIB_FAILURE);
		}

		break;

	case MLIB_DOUBLE:

		switch (mlib_ImageGetChannels(img)) {

		case 1:
			mlib_ImageClear_Fp_D64_1(img, color);
			break;

		case 2:
			mlib_ImageClear_Fp_D64_2(img, color);
			break;

		case 3:
			mlib_ImageClear_Fp_D64_3(img, color);
			break;

		case 4:
			mlib_ImageClear_Fp_D64_4(img, color);
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

#define	PREPAREVARS(type, chan)                                           \
	type *pimg = (type *) mlib_ImageGetData(img);                     \
	mlib_s32 img_height = mlib_ImageGetHeight(img);                   \
	mlib_s32 img_width = mlib_ImageGetWidth(img);                     \
	mlib_s32 img_stride = mlib_ImageGetStride(img) / sizeof (type);   \
	    mlib_s32                                                      \
		i,                                                        \
		j,                                                        \
		l;                                                        \
	    mlib_d64                                                      \
		dcolor,                                                   \
		dc[1];                                                    \
	                                                                  \
	if ((img_width * chan) == img_stride)                             \
	{                                                                 \
	img_width *= img_height;                                          \
	img_height = 1;                                                   \
	}

/* *********************************************************** */

#define	STRIP(pd, color, w, h, chan, data_type)                    \
	for (l = 0; l < chan; l++) {                               \
	    data_type color_i = (data_type) color[l];              \
	                                                           \
	    for (i = 0; i < h; i++) {                              \
		for (j = 0; j < w; j++)                            \
		    pd[i * img_stride + l + j * chan] = color_i;   \
	    }                                                      \
	}

/* *********************************************************** */

void
mlib_ImageClear_Fp_F32_1(
    mlib_image *img,
    const mlib_d64 *color)
{
	mlib_f32 color0 = (mlib_f32)color[0];

	PREPAREVARS(mlib_f32,
	    1);

	if (img_width < 4) {
		STRIP(pimg, color, img_width, img_height, 1, mlib_f32);

		return;
	}

	((mlib_f32 *)dc)[0] = ((mlib_f32 *)dc)[1] = color0;
	dcolor = dc[0];
	for (i = 0; i < img_height; i++) {
		mlib_f32 *pimg_row = pimg + i * img_stride;
		mlib_s32 align = (mlib_addr)pimg_row & 7;

		j = 0;

		if (align)
			pimg_row[j++] = color0;
		for (; j <= (img_width - 2); j += 2)
			*((mlib_d64 *)(pimg_row + j)) = dcolor;
		for (; j < img_width; j++)
			pimg_row[j] = color0;
	}
}

/* *********************************************************** */

void
mlib_ImageClear_Fp_F32_2(
    mlib_image *img,
    const mlib_d64 *color)
{
	mlib_f32 color0 = (mlib_f32)color[0], color1 = (mlib_f32)color[1];

	PREPAREVARS(mlib_f32,
	    2);

	if (img_width < 2) {
		STRIP(pimg, color, img_width, img_height, 2, mlib_f32);

		return;
	}

	for (i = 0; i < img_height; i++) {
		mlib_f32 *pimg_row = pimg + i * img_stride;
		mlib_s32 align = (mlib_addr)pimg_row & 7;

		j = 0;

		if (align) {
			pimg_row[j] = color0;
			((mlib_f32 *)dc)[0] = color1;
			((mlib_f32 *)dc)[1] = color0;
			pimg_row[j + img_width * 2 - 1] = color1;
			j++;
		} else {
			((mlib_f32 *)dc)[0] = color0;
			((mlib_f32 *)dc)[1] = color1;
		}

		dcolor = dc[0];
		for (; j <= (img_width * 2 - 2); j += 2) {
			*((mlib_d64 *)(pimg_row + j)) = dcolor;
		}
	}
}

/* *********************************************************** */

void
mlib_ImageClear_Fp_F32_3(
    mlib_image *img,
    const mlib_d64 *color)
{
	mlib_f32 color0 = (mlib_f32)color[0];
	mlib_f32 color1 = (mlib_f32)color[1];
	mlib_f32 color2 = (mlib_f32)color[2];
	mlib_d64 dcolor1, dcolor2;

	PREPAREVARS(mlib_f32,
	    3);

	if (img_width < 2) {
		STRIP(pimg, color, img_width, img_height, 3, mlib_f32);

		return;
	}

	((mlib_f32 *)dc)[0] = color0;
	((mlib_f32 *)dc)[1] = color1;
	dcolor = dc[0];
	((mlib_f32 *)dc)[0] = color2;
	((mlib_f32 *)dc)[1] = color0;
	dcolor1 = dc[0];
	((mlib_f32 *)dc)[0] = color1;
	((mlib_f32 *)dc)[1] = color2;
	dcolor2 = dc[0];
	for (i = 0; i < img_height; i++) {
		mlib_f32 *pimg_row = pimg + i * img_stride;
		mlib_s32 align = (mlib_addr)pimg_row & 7;

		j = 0;

		if (align) {
			pimg_row[j] = color0;
			pimg_row[j + 1] = color1;
			pimg_row[j + 2] = color2;
			j += 3;
		}

		for (; j <= (img_width * 3 - 6); j += 6) {
			*((mlib_d64 *)(pimg_row + j)) = dcolor;
			*((mlib_d64 *)(pimg_row + j + 2)) = dcolor1;
			*((mlib_d64 *)(pimg_row + j + 4)) = dcolor2;
		}

		if (j < img_width * 3) {
			pimg_row[j] = color0;
			pimg_row[j + 1] = color1;
			pimg_row[j + 2] = color2;
		}
	}
}

/* *********************************************************** */

void
mlib_ImageClear_Fp_F32_4(
    mlib_image *img,
    const mlib_d64 *color)
{
	mlib_f32 color0 = (mlib_f32)color[0], color1 = (mlib_f32)color[1];
	mlib_f32 color2 = (mlib_f32)color[2], color3 = (mlib_f32)color[3];
	mlib_d64 dcolor1, dc1[1];

	PREPAREVARS(mlib_f32,
	    4);

	if (img_width < 2) {
		STRIP(pimg, color, img_width, img_height, 4, mlib_f32);

		return;
	}

	for (i = 0; i < img_height; i++) {
		mlib_f32 *pimg_row = pimg + i * img_stride;
		mlib_s32 align = (mlib_addr)pimg_row & 7;

		j = 0;

		if (align) {
			pimg_row[j] = color0;
			((mlib_f32 *)dc)[0] = color1;
			((mlib_f32 *)dc)[1] = color2;
			((mlib_f32 *)dc1)[0] = color3;
			((mlib_f32 *)dc1)[1] = color0;
			*((mlib_d64 *)(pimg_row + j + img_width * 4 - 3)) =
			    dc[0];
			pimg_row[j + img_width * 4 - 1] = color3;
			j++;
		} else {
			((mlib_f32 *)dc)[0] = color0;
			((mlib_f32 *)dc)[1] = color1;
			((mlib_f32 *)dc1)[0] = color2;
			((mlib_f32 *)dc1)[1] = color3;
		}

		dcolor = dc[0];
		dcolor1 = dc1[0];
		for (; j <= (img_width * 4 - 4); j += 4) {
			*((mlib_d64 *)(pimg_row + j)) = dcolor;
			*((mlib_d64 *)(pimg_row + j) + 1) = dcolor1;
		}
	}
}

/* *********************************************************** */

void
mlib_ImageClear_Fp_D64_1(
    mlib_image *img,
    const mlib_d64 *color)
{
	mlib_d64 *pimg = (mlib_d64 *)mlib_ImageGetData(img);
	mlib_d64 color0 = color[0];
	mlib_s32 img_height = mlib_ImageGetHeight(img);
	mlib_s32 img_width = mlib_ImageGetWidth(img);
	mlib_s32 img_stride = mlib_ImageGetStride(img) >> 3;
	mlib_s32 i, j;

	for (i = 0; i < img_height; i++) {
		mlib_d64 *pimg_row = pimg + i * img_stride;

		for (j = 0; j < img_width; j++) {
			pimg_row[j] = color0;
		}
	}
}

/* *********************************************************** */

void
mlib_ImageClear_Fp_D64_2(
    mlib_image *img,
    const mlib_d64 *color)
{
	mlib_d64 *pimg = (mlib_d64 *)mlib_ImageGetData(img);
	mlib_d64 color0 = color[0];
	mlib_d64 color1 = color[1];
	mlib_s32 img_height = mlib_ImageGetHeight(img);
	mlib_s32 img_width = mlib_ImageGetWidth(img);
	mlib_s32 img_stride = mlib_ImageGetStride(img) >> 3;
	mlib_s32 i, j;

	for (i = 0; i < img_height; i++) {
		mlib_d64 *pimg_row = pimg + i * img_stride;

		for (j = 0; j < img_width * 2; j += 2) {
			pimg_row[j] = color0;
			pimg_row[j + 1] = color1;
		}
	}
}

/* *********************************************************** */

void
mlib_ImageClear_Fp_D64_3(
    mlib_image *img,
    const mlib_d64 *color)
{
	mlib_d64 *pimg = (mlib_d64 *)mlib_ImageGetData(img);
	mlib_d64 color0 = color[0];
	mlib_d64 color1 = color[1];
	mlib_d64 color2 = color[2];
	mlib_s32 img_height = mlib_ImageGetHeight(img);
	mlib_s32 img_width = mlib_ImageGetWidth(img);
	mlib_s32 img_stride = mlib_ImageGetStride(img) >> 3;
	mlib_s32 i, j;

	for (i = 0; i < img_height; i++) {
		mlib_d64 *pimg_row = pimg + i * img_stride;

		for (j = 0; j < img_width * 3; j += 3) {
			pimg_row[j] = color0;
			pimg_row[j + 1] = color1;
			pimg_row[j + 2] = color2;
		}
	}
}

/* *********************************************************** */

void
mlib_ImageClear_Fp_D64_4(
    mlib_image *img,
    const mlib_d64 *color)
{
	mlib_d64 *pimg = (mlib_d64 *)mlib_ImageGetData(img);
	mlib_d64 color0 = color[0];
	mlib_d64 color1 = color[1];
	mlib_d64 color2 = color[2];
	mlib_d64 color3 = color[3];
	mlib_s32 img_height = mlib_ImageGetHeight(img);
	mlib_s32 img_width = mlib_ImageGetWidth(img);
	mlib_s32 img_stride = mlib_ImageGetStride(img) >> 3;
	mlib_s32 i, j;

	for (i = 0; i < img_height; i++) {
		mlib_d64 *pimg_row = pimg + i * img_stride;

		for (j = 0; j < img_width * 4; j += 4) {
			pimg_row[j] = color0;
			pimg_row[j + 1] = color1;
			pimg_row[j + 2] = color2;
			pimg_row[j + 3] = color3;
		}
	}
}

/* *********************************************************** */
