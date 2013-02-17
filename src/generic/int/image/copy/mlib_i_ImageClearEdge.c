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

#pragma ident	"@(#)mlib_i_ImageClearEdge.c	9.2	07/11/05 SMI"

/*
 * FUNCTIONS
 *      mlib_ImageClearEdge - Set edge of an image to a specific
 *                            color.
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageClearEdge(mlib_image     *img,
 *                                      mlib_s32       dx,
 *                                      mlib_32        dy,
 *                                      const mlib_s32 *color);
 *
 * ARGUMENT
 *      img   Pointer to an image.
 *      dx    Number of columns on the left and right side of the
 *            image to be cleared.
 *      dy    Number of rows on the top and bottom edge of the
 *            image to be cleared.
 *      color Pointer to the color that the edges are set to.
 *
 * RESTRICTION
 *      img can have 1, 2, 3 or 4 channels of MLIB_BIT, MLIB_BYTE,
 *      MLIB_SHORT, MLIB_USHORT or MLIB_INT data type.
 *
 * DESCRIPTION
 *      Set edge of an image to a specific color.
 */

#include <mlib_image.h>
#include <mlib_ImageCheck.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageClearEdge = __mlib_ImageClearEdge

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_ImageClearEdge) mlib_ImageClearEdge
    __attribute__((weak, alias("__mlib_ImageClearEdge")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

/* do not copy by mlib_s64 data type for _NO_LONGLONG */
#ifdef _NO_LONGLONG

typedef struct
{
	mlib_s32 int0, int1;
} two_int;

#define	TYPE_64BIT	two_int

#else /* _NO_LONGLONG */

#define	TYPE_64BIT	mlib_s64
#endif /* _NO_LONGLONG */

/* *********************************************************** */

#define	DECLARE(NAME)                                           \
	static void                                             \
	NAME(mlib_image *img, mlib_s32 dx, mlib_s32 dy,         \
	    const mlib_s32 *color)

DECLARE(mlib_c_ImageClearEdge_BIT_1);
DECLARE(mlib_c_ImageClearEdge_BIT_2);
DECLARE(mlib_c_ImageClearEdge_BIT_3);
DECLARE(mlib_c_ImageClearEdge_BIT_4);
DECLARE(mlib_c_ImageClearEdge_U8_1);
DECLARE(mlib_c_ImageClearEdge_U8_2);
DECLARE(mlib_c_ImageClearEdge_U8_3);
DECLARE(mlib_c_ImageClearEdge_U8_4);
DECLARE(mlib_c_ImageClearEdge_S16_1);
DECLARE(mlib_c_ImageClearEdge_S16_2);
DECLARE(mlib_c_ImageClearEdge_S16_3);
DECLARE(mlib_c_ImageClearEdge_S16_4);
DECLARE(mlib_c_ImageClearEdge_S32_1);
DECLARE(mlib_c_ImageClearEdge_S32_2);
DECLARE(mlib_c_ImageClearEdge_S32_3);
DECLARE(mlib_c_ImageClearEdge_S32_4);

/* *********************************************************** */

mlib_status
__mlib_ImageClearEdge(
    mlib_image *img,
    mlib_s32 dx,
    mlib_s32 dy,
    const mlib_s32 *color)
{
	MLIB_IMAGE_CHECK(img);

	if (dx < 0 || dy < 0)
		return (MLIB_OUTOFRANGE);
	if (dx == 0 && dy == 0)
		return (MLIB_SUCCESS);
	if ((dx >= (mlib_ImageGetWidth(img) + 1) / 2) ||
	    (dy >= (mlib_ImageGetHeight(img) + 1) / 2)) {
		__mlib_ImageClear(img, color);
		return (MLIB_SUCCESS);
	}

	switch (mlib_ImageGetType(img)) {

	case MLIB_BIT:
		switch (mlib_ImageGetChannels(img)) {

		case 1:
			mlib_c_ImageClearEdge_BIT_1(img, dx, dy, color);
			break;

		case 2:
			mlib_c_ImageClearEdge_BIT_2(img, dx, dy, color);
			break;

		case 3:
			mlib_c_ImageClearEdge_BIT_3(img, dx, dy, color);
			break;

		case 4:
			mlib_c_ImageClearEdge_BIT_4(img, dx, dy, color);
			break;

		default:
			return (MLIB_FAILURE);
		}

		break;

	case MLIB_BYTE:
		switch (mlib_ImageGetChannels(img)) {

		case 1:
			mlib_c_ImageClearEdge_U8_1(img, dx, dy, color);
			break;

		case 2:
			mlib_c_ImageClearEdge_U8_2(img, dx, dy, color);
			break;

		case 3:
			mlib_c_ImageClearEdge_U8_3(img, dx, dy, color);
			break;

		case 4:
			mlib_c_ImageClearEdge_U8_4(img, dx, dy, color);
			break;

		default:
			return (MLIB_FAILURE);
		}

		break;

	case MLIB_SHORT:
	case MLIB_USHORT:
		switch (mlib_ImageGetChannels(img)) {

		case 1:
			mlib_c_ImageClearEdge_S16_1(img, dx, dy, color);
			break;

		case 2:
			mlib_c_ImageClearEdge_S16_2(img, dx, dy, color);
			break;

		case 3:
			mlib_c_ImageClearEdge_S16_3(img, dx, dy, color);
			break;

		case 4:
			mlib_c_ImageClearEdge_S16_4(img, dx, dy, color);
			break;

		default:
			return (MLIB_FAILURE);
		}

		break;

	case MLIB_INT:
		switch (mlib_ImageGetChannels(img)) {

		case 1:
			mlib_c_ImageClearEdge_S32_1(img, dx, dy, color);
			break;

		case 2:
			mlib_c_ImageClearEdge_S32_2(img, dx, dy, color);
			break;

		case 3:
			mlib_c_ImageClearEdge_S32_3(img, dx, dy, color);
			break;

		case 4:
			mlib_c_ImageClearEdge_S32_4(img, dx, dy, color);
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

#define	VERT_EDGES(chan, type)                                            \
	type *pimg = (type *) mlib_ImageGetData(img);                     \
	mlib_s32 img_height = mlib_ImageGetHeight(img);                   \
	mlib_s32 img_width = mlib_ImageGetWidth(img);                     \
	mlib_s32 img_stride = mlib_ImageGetStride(img) / sizeof (type);   \
	    mlib_s32                                                      \
		i,                                                        \
		j,                                                        \
		l;                                                        \
	    TYPE_64BIT                                                    \
		dcolor,                                                   \
		dc[1];                                                    \
	                                                                  \
	for (l = 0; l < chan; l++)                                        \
	{                                                                 \
	type color_i = color[l];                                          \
	                                                                  \
	for (j = 0; j < dx; j++) {                                        \
	    for (i = dy; i < (img_height - dy); i++) {                    \
		pimg[i * img_stride + l + j * chan] = (type) color_i;     \
		pimg[i * img_stride + l + (img_width - 1 - j) * chan] =   \
		    (type) color_i;                                       \
	    }                                                             \
	}                                                                 \
	}                                                                 \
	                                                                  \
	if (dy == 0)                                                      \
	    return;                                                       \
	if ((img_width * chan) == img_stride) {                           \
	    img_width *= dy;                                              \
	    img_stride *= (img_height - dy);                              \
	    img_height = 2;                                               \
	    dy = 1;                                                       \
	}

/* *********************************************************** */

#define	VERT_EDGES_BIT(chan)                                            \
	mlib_u8 *pimg = (mlib_u8 *)mlib_ImageGetData(img);              \
	mlib_s32 img_height = mlib_ImageGetHeight(img);                 \
	mlib_s32 img_width = mlib_ImageGetWidth(img);                   \
	mlib_s32 img_stride = mlib_ImageGetStride(img);                 \
	mlib_s32 img_bitoff = mlib_ImageGetBitOffset(img);              \
	mlib_s32 i, j, l;                                               \
	mlib_u32 ii0, ii1, col0, col1, mask1, mask2;                    \
	mlib_u32 dst;                                                   \
	                                                                \
	for (l = 0; l < chan; l++) {                                    \
	    mlib_s32 color_i = color[l] & 1;                            \
	                                                                \
	    for (i = 0; i < dx; i++) {                                  \
		for (j = dy; j < (img_height - dy); j++) {              \
		    ii0 = (i * chan + l + img_bitoff) >> 3;             \
		    ii1 =                                               \
			((img_width - 1 - i) * chan + l +               \
			img_bitoff) >> 3;                               \
		    col0 =                                              \
			color_i << ((7 - (i * chan + l +                \
			img_bitoff)) & 7);                              \
		    col1 =                                              \
			color_i << ((7 - (img_width - 1 - i) * chan -   \
			l - img_bitoff) & 7);                           \
		    mask1 =                                             \
			1 << ((7 - (i * chan + l + img_bitoff)) & 7);   \
		    mask2 =                                             \
			1 << ((7 - (img_width - 1 - i) * chan - l -     \
			img_bitoff) & 7);                               \
		    dst = pimg[j * img_stride + ii0];                   \
		    pimg[j * img_stride + ii0] =                        \
			(dst & (~mask1)) | col0;                        \
		    dst = pimg[j * img_stride + ii1];                   \
		    pimg[j * img_stride + ii1] =                        \
			(dst & (~mask2)) | col1;                        \
		}                                                       \
	    }                                                           \
	}                                                               \
	                                                                \
	if (((img_width * chan) == img_stride << 3) && (dy != 0)) {     \
	    img_width *= dy;                                            \
	    img_stride *= (img_height - dy);                            \
	    img_height = 2;                                             \
	    dy = 1;                                                     \
	}

/* *********************************************************** */

#define	HORIZ_EDGES(chan, type)                                          \
	{                                                                \
	    for (l = 0; l < chan; l++) {                                 \
		type color_i = (type) color[l];                          \
	                                                                 \
		for (i = 0; i < dy; i++) {                               \
		    for (j = 0; j < img_width; j++) {                    \
			pimg[i * img_stride + l + j * chan] = color_i;   \
			pimg[(img_height - 1 - i) * img_stride + l +     \
			    j * chan] = color_i;                         \
		    }                                                    \
		}                                                        \
	    }                                                            \
	    return;                                                      \
	}

/* *********************************************************** */

void
mlib_c_ImageClearEdge_BIT_1(
    mlib_image *img,
    mlib_s32 dx,
    mlib_s32 dy,
    const mlib_s32 *color)
{
	mlib_u8 bcolor0, bmask, src;
	TYPE_64BIT dcolor, dc[1];
	mlib_u32 color0;
	mlib_u8 *pimg_row;

	VERT_EDGES_BIT(1);

	color0 = ((color[0] & 1) << 31) >> 31;
	bcolor0 = color0 & 0xFF;

	((mlib_u32 *)dc)[0] = ((mlib_u32 *)dc)[1] = color0;
	dcolor = dc[0];

	for (i = 0, j = 0; i < dy; i++) {
		mlib_s32 dalign;

		pimg_row = pimg + i * img_stride;

		if (img_bitoff + img_width <= 8) {
			bmask =
			    (0xFF >> (8 - img_width)) << (8 - img_bitoff -
			    img_width);
			src = pimg_row[0];
			pimg_row[0] = (src & ~bmask) | (color0 & bmask);
			continue;
		} else {
			bmask = 0xFF >> img_bitoff;
			src = pimg_row[0];
			pimg_row[0] = (src & ~bmask) | (color0 & bmask);
			pimg_row++;
			j = 8 - img_bitoff;
		}

		dalign = 8 - ((mlib_addr)pimg_row & 7);
		for (; (j <= img_width - 8); j += 8) {
			if (dalign <= 0)
				break;
			(*pimg_row++) = bcolor0;
			dalign--;
		}

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (; j <= (img_width - 64); j += 64) {
			*((TYPE_64BIT *) pimg_row) = dcolor;
			pimg_row += 8;
		}

		for (; (j <= img_width - 8); j += 8) {
			(*pimg_row++) = bcolor0;
		}

		if (j < img_width) {
			bmask = (0xFF << (8 - (img_width - j))) & 0xFF;
			src = pimg_row[0];
			pimg_row[0] = (src & ~bmask) | (color0 & bmask);
		}
	}

	pimg = pimg + (img_height - dy) * img_stride;

	for (i = 0, j = 0; i < dy; i++) {
		mlib_s32 dalign;

		pimg_row = pimg + i * img_stride;

		if (img_bitoff + img_width <= 8) {
			bmask =
			    (0xFF >> (8 - img_width)) << (8 - img_bitoff -
			    img_width);
			src = pimg_row[0];
			pimg_row[0] = (src & ~bmask) | (color0 & bmask);
			continue;
		} else {
			bmask = 0xFF >> img_bitoff;
			src = pimg_row[0];
			pimg_row[0] = (src & ~bmask) | (color0 & bmask);
			pimg_row++;
			j = 8 - img_bitoff;
		}

		dalign = 8 - ((mlib_addr)pimg_row & 7);
		for (; (j <= img_width - 8); j += 8) {
			if (dalign <= 0)
				break;
			(*pimg_row++) = bcolor0;
			dalign--;
		}

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (; j <= (img_width - 64); j += 64) {
			*((TYPE_64BIT *) pimg_row) = dcolor;
			pimg_row += 8;
		}

		for (; (j <= img_width - 8); j += 8) {
			(*pimg_row++) = bcolor0;
		}

		if (j < img_width) {
			bmask = (0xFF << (8 - (img_width - j))) & 0xFF;
			src = pimg_row[0];
			pimg_row[0] = (src & ~bmask) | (color0 & bmask);
		}
	}
}

/* *********************************************************** */

void
mlib_c_ImageClearEdge_BIT_2(
    mlib_image *img,
    mlib_s32 dx,
    mlib_s32 dy,
    const mlib_s32 *color)
{
	mlib_u8 bcolor0, bmask, src;
	TYPE_64BIT dcolor, dc[1];
	mlib_u8 *pimg_row;
	mlib_s32 dalign;
	mlib_u32 color0 = color[0] & 1, color1 = color[1] & 1;

	VERT_EDGES_BIT(2);

	img_width <<= 1;

	color1 = (color0 << 1) | color1;
	color1 = (color1 << 2) | color1;
	color1 = (color1 << 4) | color1;
	color0 = ((color1 << 1) & 0xFE) | color0;
	bcolor0 = ((img_bitoff & 1) == 0) ? color1 : color0;
	color0 = (bcolor0 << 8) | bcolor0;
	color0 = (color0 << 16) | color0;

	((mlib_u32 *)dc)[0] = ((mlib_u32 *)dc)[1] = color0;
	dcolor = dc[0];

	for (i = 0, j = 0; i < dy; i++) {
		pimg_row = pimg + i * img_stride;

		if (img_bitoff + img_width <= 8) {
			bmask =
			    (0xFF >> (8 - img_width)) << (8 - img_bitoff -
			    img_width);
			src = pimg_row[0];
			pimg_row[0] = (src & ~bmask) | (color0 & bmask);
			continue;
		} else {
			bmask = 0xFF >> img_bitoff;
			src = pimg_row[0];
			pimg_row[0] = (src & ~bmask) | (color0 & bmask);
			pimg_row++;
			j = 8 - img_bitoff;
		}

		dalign = 8 - ((mlib_addr)pimg_row & 7);
		for (; (j <= img_width - 8); j += 8) {
			if (dalign <= 0)
				break;
			(*pimg_row++) = bcolor0;
			dalign--;
		}

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (; j <= (img_width - 64); j += 64) {
			*((TYPE_64BIT *) pimg_row) = dcolor;
			pimg_row += 8;
		}

		for (; (j <= img_width - 8); j += 8) {
			(*pimg_row++) = bcolor0;
		}

		if (j < img_width) {
			bmask = (0xFF << (8 - (img_width - j))) & 0xFF;
			src = pimg_row[0];
			pimg_row[0] = (src & ~bmask) | (color0 & bmask);
		}
	}

	pimg = pimg + (img_height - dy) * img_stride;

	for (i = 0, j = 0; i < dy; i++) {
		pimg_row = pimg + i * img_stride;

		if (img_bitoff + img_width <= 8) {
			bmask =
			    (0xFF >> (8 - img_width)) << (8 - img_bitoff -
			    img_width);
			src = pimg_row[0];
			pimg_row[0] = (src & ~bmask) | (color0 & bmask);
			continue;
		} else {
			bmask = 0xFF >> img_bitoff;
			src = pimg_row[0];
			pimg_row[0] = (src & ~bmask) | (color0 & bmask);
			pimg_row++;
			j = 8 - img_bitoff;
		}

		dalign = 8 - ((mlib_addr)pimg_row & 7);
		for (; (j <= img_width - 8); j += 8) {
			if (dalign <= 0)
				break;
			(*pimg_row++) = bcolor0;
			dalign--;
		}

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (; j <= (img_width - 64); j += 64) {
			*((TYPE_64BIT *) pimg_row) = dcolor;
			pimg_row += 8;
		}

		for (; (j <= img_width - 8); j += 8) {
			(*pimg_row++) = bcolor0;
		}

		if (j < img_width) {
			bmask = (0xFF << (8 - (img_width - j))) & 0xFF;
			src = pimg_row[0];
			pimg_row[0] = (src & ~bmask) | (color0 & bmask);
		}
	}
}

/* *********************************************************** */

#ifdef _LITTLE_ENDIAN

#define	CLEAR_TAIL(dsrc)                                                 \
	pimg_row = (mlib_u8 *)pimg_al;                                   \
	if (tail <= 32) {                                                \
	    src = *((mlib_u32 *)pimg_row);                               \
	    mask = mask0 << (32 - tail);                                 \
	    mask =                                                       \
		(mask << 24) | ((mask & 0xFF00) << 8) | ((mask >> 8) &   \
		0xFF00) | (mask >> 24);                                  \
	    *((mlib_u32 *)pimg_row) =                                    \
		(src & (~mask)) | (*((mlib_u32 *)(&dsrc)) & mask);       \
	} else {                                                         \
	    *((mlib_u32 *)pimg_row) = *((mlib_u32 *)(&dsrc));            \
	    src = *((mlib_u32 *)pimg_row + 1);                           \
	    mask = mask0 << (64 - tail);                                 \
	    mask =                                                       \
		(mask << 24) | ((mask & 0xFF00) << 8) | ((mask >> 8) &   \
		0xFF00) | (mask >> 24);                                  \
	    *((mlib_u32 *)pimg_row + 1) =                                \
		(src & (~mask)) | (*((mlib_u32 *)(&dsrc) + 1) & mask);   \
	}

#else /* _LITTLE_ENDIAN */

#ifdef _NO_LONGLONG

#define	CLEAR_TAIL(dsrc)                                                 \
	pimg_row = (mlib_u8 *)pimg_al;                                   \
	if (tail <= 32) {                                                \
	    src = *((mlib_u32 *)pimg_row);                               \
	    mask = mask0 << (32 - tail);                                 \
	    *((mlib_u32 *)pimg_row) =                                    \
		(src & (~mask)) | (*((mlib_u32 *)(&dsrc)) & mask);       \
	} else {                                                         \
	    *((mlib_u32 *)pimg_row) = *((mlib_u32 *)(&dsrc));            \
	    src = *((mlib_u32 *)pimg_row + 1);                           \
	    mask = mask0 << (64 - tail);                                 \
	    *((mlib_u32 *)pimg_row + 1) =                                \
		(src & (~mask)) | (*((mlib_u32 *)(&dsrc) + 1) & mask);   \
	}

#else /* _NO_LONGLONG */

#define	CLEAR_TAIL(dsrc)                                         \
	lsrc = *((mlib_u64 *)pimg_al);                           \
	lmask = lmask0 << (64 - tail);                           \
	*((mlib_u64 *)pimg_al) =                                 \
	    (lsrc & ~lmask) | (*((mlib_u64 *)(&dsrc)) & lmask)

#endif /* _NO_LONGLONG */

#endif /* _LITTLE_ENDIAN */

/* *********************************************************** */

void
mlib_c_ImageClearEdge_BIT_3(
    mlib_image *img,
    mlib_s32 dx,
    mlib_s32 dy,
    const mlib_s32 *color)
{
	TYPE_64BIT dcolor0, dcolor1, dcolor2, dc[3];
	mlib_u32 color0 = color[0] & 1,
	    color1 = color[1] & 1, color2 = color[2] & 1;
	mlib_u32 mask0 = 0xFFFFFFFFu, col2;
	mlib_s32 tail, dtail;
	mlib_s32 dalign;
	TYPE_64BIT *pimg_al;
	mlib_u8 *pimg_row;

#if defined(_NO_LONGLONG) || defined(_LITTLE_ENDIAN)
	mlib_u32 src, mask;
#else /* _NO_LONGLONG || _LITTLE_ENDIAN */
	mlib_u64 lmask0 = ((mlib_u64)mask0 << 32) | mask0;
	mlib_u64 lsrc, lmask;
#endif /* _NO_LONGLONG || _LITTLE_ENDIAN */

	VERT_EDGES_BIT(3);

	img_width *= 3;

	col0 = (color0 << 3) | (color1 << 2) | (color2 << 1) | color0;
	col1 = (col0 >> 1) | (color2 << 3);
	col2 = (col1 >> 1) | (color1 << 3);
	color0 = (col0 << 4) | col2;
	color1 = (col1 << 4) | col0;
	color2 = (col2 << 4) | col1;

#ifdef _LITTLE_ENDIAN
	col0 = (color0 << 24) | (color2 << 16) | (color1 << 8) | color0;
	*((mlib_u32 *)dc + 0) = *((mlib_u32 *)dc + 3) = col0;
	*((mlib_u32 *)dc + 1) = *((mlib_u32 *)dc + 4) =
	    (col0 << 16) | (color2 << 8) | color1;
	*((mlib_u32 *)dc + 2) = *((mlib_u32 *)dc + 5) = (col0 << 8) | color2;
#else /* _LITTLE_ENDIAN */
	col0 = (color0 << 24) | (color1 << 16) | (color2 << 8) | color0;
	*((mlib_u32 *)dc + 0) = *((mlib_u32 *)dc + 3) = col0;
	*((mlib_u32 *)dc + 1) = *((mlib_u32 *)dc + 4) = (col0 << 8) | color1;
	*((mlib_u32 *)dc + 2) = *((mlib_u32 *)dc + 5) =
	    (col0 << 16) | (color1 << 8) | color2;
#endif /* _LITTLE_ENDIAN */

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0, j = 0; i < dy; i++) {
		pimg_row = pimg + i * img_stride;
		pimg_al =
		    (TYPE_64BIT *) ((mlib_addr)pimg_row & (mlib_addr)(~7));

/* tail in bits till first mlib_s64 */
		dalign =
		    ((((mlib_addr)pimg_row - (mlib_addr)pimg_al)) << 3) +
		    img_bitoff;
		tail = 64 - dalign;
		dcolor0 = dc[tail % 3];
		dcolor1 = dc[(tail + 1) % 3];
		dcolor2 = dc[(tail + 2) % 3];

		if (tail >= img_width) {
#if defined(_NO_LONGLONG) || defined(_LITTLE_ENDIAN)

			if (dalign + img_width <= 32) {
				src = *((mlib_u32 *)pimg_al);
				mask =
				    (mask0 >> (32 - img_width)) << (32 -
				    dalign - img_width);
#ifdef _LITTLE_ENDIAN
				mask =
				    (mask << 24) | ((mask & 0xFF00) << 8) |
				    ((mask >> 8) & 0xFF00) | (mask >> 24);
#endif /* _LITTLE_ENDIAN */
				*((mlib_u32 *)pimg_al) =
				    (src & ~mask) | (*((mlib_u32 *)(&dcolor2)) &
				    mask);
				continue;
			} else {

				if (dalign >= 32) {
					src = *((mlib_u32 *)pimg_al + 1);
					mask =
					    (mask0 >> (32 - img_width)) << (64 -
					    dalign - img_width);
#ifdef _LITTLE_ENDIAN
					mask =
					    (mask << 24) | ((mask & 0xFF00) <<
					    8) | ((mask >> 8) & 0xFF00) | (mask
					    >> 24);
#endif /* _LITTLE_ENDIAN */
					*((mlib_u32 *)pimg_al + 1) =
					    (src & ~mask) | (*((mlib_u32
					    *)(&dcolor2) + 1) & mask);
					continue;
				} else {
					src = *((mlib_u32 *)pimg_al);
					mask = mask0 >> dalign;
#ifdef _LITTLE_ENDIAN
					mask =
					    (mask << 24) | ((mask & 0xFF00) <<
					    8) | ((mask >> 8) & 0xFF00) | (mask
					    >> 24);
#endif /* _LITTLE_ENDIAN */
					*((mlib_u32 *)pimg_al) =
					    (src & ~mask) | (*((mlib_u32
					    *)(&dcolor2)) & mask);
					src = *((mlib_u32 *)pimg_al + 1);
					mask =
					    mask0 << (64 - dalign - img_width);
#ifdef _LITTLE_ENDIAN
					mask =
					    (mask << 24) | ((mask & 0xFF00) <<
					    8) | ((mask >> 8) & 0xFF00) | (mask
					    >> 24);
#endif /* _LITTLE_ENDIAN */
					*((mlib_u32 *)pimg_al + 1) =
					    (src & ~mask) | (*((mlib_u32
					    *)(&dcolor2) + 1) & mask);
					continue;
				}
			}

#else /* _NO_LONGLONG || _LITTLE_ENDIAN */
			lsrc = *((mlib_u64 *)pimg_al);
			lmask =
			    (lmask0 >> (64 - img_width)) << (64 - dalign -
			    img_width);
			*((mlib_u64 *)pimg_al) =
			    (lsrc & ~lmask) | (*((mlib_u64 *)(&dcolor2)) &
			    lmask);
			continue;
#endif /* _NO_LONGLONG || _LITTLE_ENDIAN */
		}
#if defined(_NO_LONGLONG) || defined(_LITTLE_ENDIAN)

		if (dalign >= 32) {
			src = *((mlib_u32 *)pimg_al + 1);
			mask = mask0 >> (dalign - 32);
#ifdef _LITTLE_ENDIAN
			mask =
			    (mask << 24) | ((mask & 0xFF00) << 8) | ((mask >> 8)
			    & 0xFF00) | (mask >> 24);
#endif /* _LITTLE_ENDIAN */
			*((mlib_u32 *)pimg_al + 1) =
			    (src & ~mask) | (*((mlib_u32 *)(&dcolor2) +
			    1) & mask);
			j = 64 - dalign;
		} else {
			src = *((mlib_u32 *)pimg_al);
			mask = mask0 >> dalign;
#ifdef _LITTLE_ENDIAN
			mask =
			    (mask << 24) | ((mask & 0xFF00) << 8) | ((mask >> 8)
			    & 0xFF00) | (mask >> 24);
#endif /* _LITTLE_ENDIAN */
			*((mlib_u32 *)pimg_al) =
			    (src & ~mask) | (*((mlib_u32 *)(&dcolor2)) & mask);
			*((mlib_u32 *)pimg_al + 1) =
			    *((mlib_u32 *)(&dcolor2) + 1);
			j = 64 - dalign;
		}

#else /* _NO_LONGLONG || _LITTLE_ENDIAN */
		lsrc = *(mlib_u64 *)pimg_al;
		lmask = lmask0 >> dalign;
		*((mlib_u64 *)pimg_al) =
		    (lsrc & ~lmask) | (*((mlib_u64 *)(&dcolor2)) & lmask);
		j = 64 - dalign;
#endif /* _NO_LONGLONG || _LITTLE_ENDIAN */

		pimg_al++;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (; j <= (img_width - 192); j += 192) {
			pimg_al[0] = dcolor0;
			pimg_al[1] = dcolor1;
			pimg_al[2] = dcolor2;
			pimg_al += 3;
		}

		tail = img_width - j;
/* in doubles */
		dtail = (tail + 63) / 64;

		if (dtail == 1) {
			CLEAR_TAIL(dcolor0);
		} else {

			if (dtail == 2) {
				(*pimg_al++) = dcolor0;
				tail -= 64;
				CLEAR_TAIL(dcolor1);
			} else {

				if (dtail == 3) {
					pimg_al[0] = dcolor0;
					pimg_al[1] = dcolor1;
					tail -= 128;
					pimg_al += 2;
					CLEAR_TAIL(dcolor2);
				}
			}
		}
	}

	pimg = pimg + (img_height - dy) * img_stride;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0, j = 0; i < dy; i++) {
		pimg_row = pimg + i * img_stride;
		pimg_al =
		    (TYPE_64BIT *) ((mlib_addr)pimg_row & (mlib_addr)(~7));

/* tail in bits till first mlib_s64 */
		dalign =
		    ((((mlib_addr)pimg_row - (mlib_addr)pimg_al)) << 3) +
		    img_bitoff;
		tail = 64 - dalign;
		dcolor0 = dc[tail % 3];
		dcolor1 = dc[(tail + 1) % 3];
		dcolor2 = dc[(tail + 2) % 3];

		if (tail >= img_width) {
#if defined(_NO_LONGLONG) || defined(_LITTLE_ENDIAN)

			if (dalign + img_width <= 32) {
				src = *((mlib_u32 *)pimg_al);
				mask =
				    (mask0 >> (32 - img_width)) << (32 -
				    dalign - img_width);
#ifdef _LITTLE_ENDIAN
				mask =
				    (mask << 24) | ((mask & 0xFF00) << 8) |
				    ((mask >> 8) & 0xFF00) | (mask >> 24);
#endif /* _LITTLE_ENDIAN */
				*((mlib_u32 *)pimg_al) =
				    (src & ~mask) | (*((mlib_u32 *)(&dcolor2)) &
				    mask);
				continue;
			} else {

				if (dalign >= 32) {
					src = *((mlib_u32 *)pimg_al + 1);
					mask =
					    (mask0 >> (32 - img_width)) << (64 -
					    dalign - img_width);
#ifdef _LITTLE_ENDIAN
					mask =
					    (mask << 24) | ((mask & 0xFF00) <<
					    8) | ((mask >> 8) & 0xFF00) | (mask
					    >> 24);
#endif /* _LITTLE_ENDIAN */
					*((mlib_u32 *)pimg_al + 1) =
					    (src & ~mask) | (*((mlib_u32
					    *)(&dcolor2) + 1) & mask);
					continue;
				} else {
					src = *((mlib_u32 *)pimg_al);
					mask = mask0 >> dalign;
#ifdef _LITTLE_ENDIAN
					mask =
					    (mask << 24) | ((mask & 0xFF00) <<
					    8) | ((mask >> 8) & 0xFF00) | (mask
					    >> 24);
#endif /* _LITTLE_ENDIAN */
					*((mlib_u32 *)pimg_al) =
					    (src & ~mask) | (*((mlib_u32
					    *)(&dcolor2)) & mask);
					src = *((mlib_u32 *)pimg_al + 1);
					mask =
					    mask0 << (64 - dalign - img_width);
#ifdef _LITTLE_ENDIAN
					mask =
					    (mask << 24) | ((mask & 0xFF00) <<
					    8) | ((mask >> 8) & 0xFF00) | (mask
					    >> 24);
#endif /* _LITTLE_ENDIAN */
					*((mlib_u32 *)pimg_al + 1) =
					    (src & ~mask) | (*((mlib_u32
					    *)(&dcolor2) + 1) & mask);
					continue;
				}
			}

#else /* _NO_LONGLONG || _LITTLE_ENDIAN */
			lsrc = *((mlib_u64 *)pimg_al);
			lmask =
			    (lmask0 >> (64 - img_width)) << (64 - dalign -
			    img_width);
			*((mlib_u64 *)pimg_al) =
			    (lsrc & ~lmask) | (*((mlib_u64 *)(&dcolor2)) &
			    lmask);
			continue;
#endif /* _NO_LONGLONG || _LITTLE_ENDIAN */
		}
#if defined(_NO_LONGLONG) || defined(_LITTLE_ENDIAN)

		if (dalign >= 32) {
			src = *((mlib_u32 *)pimg_al + 1);
			mask = mask0 >> (dalign - 32);
#ifdef _LITTLE_ENDIAN
			mask =
			    (mask << 24) | ((mask & 0xFF00) << 8) | ((mask >> 8)
			    & 0xFF00) | (mask >> 24);
#endif /* _LITTLE_ENDIAN */
			*((mlib_u32 *)pimg_al + 1) =
			    (src & ~mask) | (*((mlib_u32 *)(&dcolor2) +
			    1) & mask);
			j = 64 - dalign;
		} else {
			src = *((mlib_u32 *)pimg_al);
			mask = mask0 >> dalign;
#ifdef _LITTLE_ENDIAN
			mask =
			    (mask << 24) | ((mask & 0xFF00) << 8) | ((mask >> 8)
			    & 0xFF00) | (mask >> 24);
#endif /* _LITTLE_ENDIAN */
			*((mlib_u32 *)pimg_al) =
			    (src & ~mask) | (*((mlib_u32 *)(&dcolor2)) & mask);
			*((mlib_u32 *)pimg_al + 1) =
			    *((mlib_u32 *)(&dcolor2) + 1);
			j = 64 - dalign;
		}

#else /* _NO_LONGLONG || _LITTLE_ENDIAN */
		lsrc = *(mlib_u64 *)pimg_al;
		lmask = lmask0 >> dalign;
		*((mlib_u64 *)pimg_al) =
		    (lsrc & ~lmask) | (*((mlib_u64 *)(&dcolor2)) & lmask);
		j = 64 - dalign;
#endif /* _NO_LONGLONG || _LITTLE_ENDIAN */

		pimg_al++;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (; j <= (img_width - 192); j += 192) {
			pimg_al[0] = dcolor0;
			pimg_al[1] = dcolor1;
			pimg_al[2] = dcolor2;
			pimg_al += 3;
		}

		tail = img_width - j;
/* in doubles */
		dtail = (tail + 63) / 64;

		if (dtail == 1) {
			CLEAR_TAIL(dcolor0);
		} else {

			if (dtail == 2) {
				(*pimg_al++) = dcolor0;
				tail -= 64;
				CLEAR_TAIL(dcolor1);
			} else {

				if (dtail == 3) {
					pimg_al[0] = dcolor0;
					pimg_al[1] = dcolor1;
					tail -= 128;
					pimg_al += 2;
					CLEAR_TAIL(dcolor2);
				}
			}
		}
	}
}

/* *********************************************************** */

void
mlib_c_ImageClearEdge_BIT_4(
    mlib_image *img,
    mlib_s32 dx,
    mlib_s32 dy,
    const mlib_s32 *color)
{
	mlib_u8 bcolor0, bmask, src;
	TYPE_64BIT dcolor, dc[1];
	mlib_u8 *pimg_row;
	mlib_s32 dalign;
	mlib_u32 color0 = color[0] & 1,
	    color1 = color[1] & 1, color2 = color[2] & 1, color3 = color[3] & 1;

	VERT_EDGES_BIT(4);

	img_width <<= 2;

	color0 = (color0 << 3) | (color1 << 2) | (color2 << 1) | color3;
	color0 = (color0 << 4) | color0;
	color3 = (color0 << 1) | (color0 >> 7);
	color2 = (color0 << 2) | (color0 >> 6);
	color1 = (color0 << 3) | (color0 >> 5);

	bcolor0 = (img_bitoff & 2) ?
	    ((img_bitoff & 1) ? color3 : color2) :
	    ((img_bitoff & 1) ? color1 : color0);
	color0 = (bcolor0 << 24) | (bcolor0 << 16) | (bcolor0 << 8) | bcolor0;

	((mlib_u32 *)dc)[0] = ((mlib_u32 *)dc)[1] = color0;
	dcolor = dc[0];

	for (i = 0, j = 0; i < dy; i++) {
		pimg_row = pimg + i * img_stride;

		if (img_bitoff + img_width <= 8) {
			bmask =
			    (0xFF >> (8 - img_width)) << (8 - img_bitoff -
			    img_width);
			src = pimg_row[0];
			pimg_row[0] = (src & ~bmask) | (color0 & bmask);
			continue;
		} else {
			bmask = 0xFF >> img_bitoff;
			src = pimg_row[0];
			pimg_row[0] = (src & ~bmask) | (color0 & bmask);
			pimg_row++;
			j = 8 - img_bitoff;
		}

		dalign = 8 - ((mlib_addr)pimg_row & 7);
		for (; (j <= img_width - 8); j += 8) {
			if (dalign <= 0)
				break;
			(*pimg_row++) = bcolor0;
			dalign--;
		}

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (; j <= (img_width - 64); j += 64) {
			*((TYPE_64BIT *) pimg_row) = dcolor;
			pimg_row += 8;
		}

		for (; (j <= img_width - 8); j += 8) {
			(*pimg_row++) = bcolor0;
		}

		if (j < img_width) {
			bmask = (0xFF << (8 - (img_width - j))) & 0xFF;
			src = pimg_row[0];
			pimg_row[0] = (src & ~bmask) | (color0 & bmask);
		}
	}

	pimg = pimg + (img_height - dy) * img_stride;

	for (i = 0, j = 0; i < dy; i++) {
		pimg_row = pimg + i * img_stride;

		if (img_bitoff + img_width <= 8) {
			bmask =
			    (0xFF >> (8 - img_width)) << (8 - img_bitoff -
			    img_width);
			src = pimg_row[0];
			pimg_row[0] = (src & ~bmask) | (color0 & bmask);
			continue;
		} else {
			bmask = 0xFF >> img_bitoff;
			src = pimg_row[0];
			pimg_row[0] = (src & ~bmask) | (color0 & bmask);
			pimg_row++;
			j = 8 - img_bitoff;
		}

		dalign = 8 - ((mlib_addr)pimg_row & 7);
		for (; (j <= img_width - 8); j += 8) {
			if (dalign <= 0)
				break;
			(*pimg_row++) = bcolor0;
			dalign--;
		}

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (; j <= (img_width - 64); j += 64) {
			*((TYPE_64BIT *) pimg_row) = dcolor;
			pimg_row += 8;
		}

		for (; (j <= img_width - 8); j += 8) {
			(*pimg_row++) = bcolor0;
		}

		if (j < img_width) {
			bmask = (0xFF << (8 - (img_width - j))) & 0xFF;
			src = pimg_row[0];
			pimg_row[0] = (src & ~bmask) | (color0 & bmask);
		}
	}
}

/* *********************************************************** */

void
mlib_c_ImageClearEdge_U8_1(
    mlib_image *img,
    mlib_s32 dx,
    mlib_s32 dy,
    const mlib_s32 *color)
{
	mlib_u32 color0 = color[0] & 0xFF;

	VERT_EDGES(1, mlib_u8);

	if (img_width < 16)
		HORIZ_EDGES(1, mlib_u8);

	color0 |= (color0 << 8);
	color0 |= (color0 << 16);
	((mlib_s32 *)dc)[0] = ((mlib_s32 *)dc)[1] = color0;
	dcolor = dc[0];
	for (i = 0; i < dy; i++) {
		mlib_u8 *pimg_row = pimg + i * img_stride;
		mlib_s32 align = (8 - (mlib_addr)pimg_row) & 7;

		for (j = 0; j < align; j++)
			pimg_row[j] = color0;
		for (; j <= (img_width - 8); j += 8)
			*((TYPE_64BIT *) (pimg_row + j)) = dcolor;
		for (; j < img_width; j++)
			pimg_row[j] = color0;

		pimg_row = pimg + (img_height - 1 - i) * img_stride;
		align = (8 - (mlib_addr)pimg_row) & 7;
		for (j = 0; j < align; j++)
			pimg_row[j] = color0;
		for (; j <= (img_width - 8); j += 8)
			*((TYPE_64BIT *) (pimg_row + j)) = dcolor;
		for (; j < img_width; j++)
			pimg_row[j] = color0;
	}
}

/* *********************************************************** */

void
mlib_c_ImageClearEdge_U8_2(
    mlib_image *img,
    mlib_s32 dx,
    mlib_s32 dy,
    const mlib_s32 *color)
{
	mlib_u32 color0 = color[0] & 0xFF,
	    color1 = color[1] & 0xFF, col, col0, col1;

	VERT_EDGES(2, mlib_u8);

	if (img_width < 8)
		HORIZ_EDGES(2, mlib_u8);

#ifdef _LITTLE_ENDIAN
	col1 = (color0 << 8) | color1;
	col1 |= (col1 << 16);
	col0 = (col1 << 8) | color0;
#else /* _LITTLE_ENDIAN */
	col0 = (color0 << 8) | color1;
	col0 |= (col0 << 16);
	col1 = (col0 << 8) | color0;
#endif /* _LITTLE_ENDIAN */

	for (i = 0; i < dy; i++) {
		mlib_u8 *pimg_row = pimg + i * img_stride;
		mlib_s32 align = (8 - (mlib_addr)pimg_row) & 7;

		col = (align & 1) ? col1 : col0;
		((mlib_u32 *)dc)[0] = ((mlib_u32 *)dc)[1] = col;
		dcolor = dc[0];
		for (j = 0; j < align; j += 2) {
			pimg_row[j] = color0;
			pimg_row[j + 1] = color1;
		}

		for (j = align; j <= (img_width * 2 - 8); j += 8) {
			*((TYPE_64BIT *) (pimg_row + j)) = dcolor;
		}

		if (align & 1) {
			for (; j < (img_width * 2 - 1); j += 2) {
				pimg_row[j] = color1;
				pimg_row[j + 1] = color0;
			}

			pimg_row[j] = color1;
		} else {
			for (; j < img_width * 2; j += 2) {
				*((mlib_s16 *)(pimg_row + j)) = col0;
			}
		}

		pimg_row = pimg + (img_height - 1 - i) * img_stride;
		align = (8 - (mlib_addr)pimg_row) & 7;
		col = (align & 1) ? col1 : col0;
		((mlib_u32 *)dc)[0] = ((mlib_u32 *)dc)[1] = col;
		dcolor = dc[0];
		for (j = 0; j < align; j += 2) {
			pimg_row[j] = color0;
			pimg_row[j + 1] = color1;
		}

		for (j = align; j <= (img_width * 2 - 8); j += 8) {
			*((TYPE_64BIT *) (pimg_row + j)) = dcolor;
		}

		if (align & 1) {
			for (; j < (img_width * 2 - 1); j += 2) {
				pimg_row[j] = color1;
				pimg_row[j + 1] = color0;
			}

			pimg_row[j] = color1;
		} else {
			for (; j < img_width * 2; j += 2) {
				*((mlib_s16 *)(pimg_row + j)) = col0;
			}
		}
	}
}

/* *********************************************************** */

void
mlib_c_ImageClearEdge_U8_3(
    mlib_image *img,
    mlib_s32 dx,
    mlib_s32 dy,
    const mlib_s32 *color)
{
	mlib_u32 color0 = color[0] & 0xFF,
	    color1 = color[1] & 0xFF,
	    color2 = color[2] & 0xFF, col0, col1, col2;
	TYPE_64BIT dcolor1, dcolor2;

	VERT_EDGES(3, mlib_u8);

	if (img_width < 16)
		HORIZ_EDGES(3, mlib_u8);

#ifdef _LITTLE_ENDIAN
	col2 = (color2 << 16) | (color1 << 8) | color0;
	col2 = (col2 << 8) | color2;
	col1 = (col2 << 8) | color1;
	col0 = (col1 << 8) | color0;
#else /* _LITTLE_ENDIAN */
	col0 = (color0 << 16) | (color1 << 8) | color2;
	col0 = (col0 << 8) | color0;
	col1 = (col0 << 8) | color1;
	col2 = (col1 << 8) | color2;
#endif /* _LITTLE_ENDIAN */

	((mlib_u32 *)dc)[0] = col0;
	((mlib_u32 *)dc)[1] = col1;
	dcolor = dc[0];
	((mlib_u32 *)dc)[0] = col2;
	((mlib_u32 *)dc)[1] = col0;
	dcolor1 = dc[0];
	((mlib_u32 *)dc)[0] = col1;
	((mlib_u32 *)dc)[1] = col2;
	dcolor2 = dc[0];
	for (i = 0; i < dy; i++) {
		mlib_u8 *pimg_row = pimg + i * img_stride;

		for (j = 0; ((mlib_addr)(pimg_row + j) & 7) != 0; j += 3) {
			pimg_row[j] = color0;
			pimg_row[j + 1] = color1;
			pimg_row[j + 2] = color2;
		}

		for (; j <= (img_width * 3 - 24); j += 24) {
			*((TYPE_64BIT *) (pimg_row + j)) = dcolor;
			*((TYPE_64BIT *) (pimg_row + j + 8)) = dcolor1;
			*((TYPE_64BIT *) (pimg_row + j + 16)) = dcolor2;
		}

		for (; j < img_width * 3; j += 3) {
			pimg_row[j] = color0;
			pimg_row[j + 1] = color1;
			pimg_row[j + 2] = color2;
		}

		pimg_row = pimg + (img_height - 1 - i) * img_stride;
		for (j = 0; ((mlib_addr)(pimg_row + j) & 7) != 0; j += 3) {
			pimg_row[j] = color0;
			pimg_row[j + 1] = color1;
			pimg_row[j + 2] = color2;
		}

		for (; j <= (img_width * 3 - 24); j += 24) {
			*((TYPE_64BIT *) (pimg_row + j)) = dcolor;
			*((TYPE_64BIT *) (pimg_row + j + 8)) = dcolor1;
			*((TYPE_64BIT *) (pimg_row + j + 16)) = dcolor2;
		}

		for (; j < img_width * 3; j += 3) {
			pimg_row[j] = color0;
			pimg_row[j + 1] = color1;
			pimg_row[j + 2] = color2;
		}
	}
}

/* *********************************************************** */

void
mlib_c_ImageClearEdge_U8_4(
    mlib_image *img,
    mlib_s32 dx,
    mlib_s32 dy,
    const mlib_s32 *color)
{
	mlib_u32 color0 = color[0] & 0xFF,
	    color1 = color[1] & 0xFF,
	    color2 = color[2] & 0xFF,
	    color3 = color[3] & 0xFF, col, col0, col1, col2, col3;

	VERT_EDGES(4, mlib_u8);

	if (img_width < 4)
		HORIZ_EDGES(4, mlib_u8);

#ifdef _LITTLE_ENDIAN
	col0 = (color3 << 24) | (color2 << 16) | (color1 << 8) | color0;
	col3 = (col0 << 8) | color3;
	col2 = (col3 << 8) | color2;
	col1 = (col2 << 8) | color1;
#else /* _LITTLE_ENDIAN */
	col0 = (color0 << 24) | (color1 << 16) | (color2 << 8) | color3;
	col1 = (col0 << 8) | color0;
	col2 = (col1 << 8) | color1;
	col3 = (col2 << 8) | color2;
#endif /* _LITTLE_ENDIAN */

	for (i = 0; i < dy; i++) {
		mlib_u8 *pimg_row = pimg + i * img_stride;
		mlib_s32 align = (8 - (mlib_addr)pimg_row) & 7;

		col = (align & 2) ?
		    ((align & 1) ? col3 : col2) : ((align & 1) ? col1 : col0);
		((mlib_u32 *)dc)[0] = ((mlib_u32 *)dc)[1] = col;
		dcolor = dc[0];
		for (j = 0; j < align; j += 4) {
			pimg_row[j] = color0;
			pimg_row[j + 1] = color1;
			pimg_row[j + 2] = color2;
			pimg_row[j + 3] = color3;
		}

		for (j = align; j <= (img_width * 4 - 8); j += 8) {
			*((TYPE_64BIT *) (pimg_row + j)) = dcolor;
		}

		if (j <= (img_width * 4 - 4)) {
			*((mlib_s32 *)(pimg_row + j)) = col;
			j += 4;
		}

		if (align & 2) {
			if (!(align & 1))
				pimg_row[j++] = color2;
			pimg_row[j] = color3;
		} else {
			if (align & 1) {
				pimg_row[j] = color1;
				pimg_row[j + 1] = color2;
				pimg_row[j + 2] = color3;
			}
		}

		pimg_row = pimg + (img_height - 1 - i) * img_stride;
		align = (8 - (mlib_addr)pimg_row) & 7;
		col = (align & 2) ?
		    ((align & 1) ? col3 : col2) : ((align & 1) ? col1 : col0);
		((mlib_u32 *)dc)[0] = ((mlib_u32 *)dc)[1] = col;
		dcolor = dc[0];
		for (j = 0; j < align; j += 4) {
			pimg_row[j] = color0;
			pimg_row[j + 1] = color1;
			pimg_row[j + 2] = color2;
			pimg_row[j + 3] = color3;
		}

		for (j = align; j <= (img_width * 4 - 8); j += 8) {
			*((TYPE_64BIT *) (pimg_row + j)) = dcolor;
		}

		if (j <= (img_width * 4 - 4)) {
			*((mlib_s32 *)(pimg_row + j)) = col;
			j += 4;
		}

		if (align & 2) {
			if (!(align & 1))
				pimg_row[j++] = color2;
			pimg_row[j] = color3;
		} else {
			if (align & 1) {
				pimg_row[j] = color1;
				pimg_row[j + 1] = color2;
				pimg_row[j + 2] = color3;
			}
		}
	}
}

/* *********************************************************** */

void
mlib_c_ImageClearEdge_S16_1(
    mlib_image *img,
    mlib_s32 dx,
    mlib_s32 dy,
    const mlib_s32 *color)
{
	mlib_u32 color0 = color[0] & 0xFFFF;

	VERT_EDGES(1, mlib_s16);

	if (img_width < 8)
		HORIZ_EDGES(1, mlib_s16);

	color0 |= (color0 << 16);
	((mlib_s32 *)dc)[0] = ((mlib_s32 *)dc)[1] = color0;
	dcolor = dc[0];
	for (i = 0; i < dy; i++) {
		mlib_s16 *pimg_row = pimg + i * img_stride;
		mlib_s32 align = ((8 - (mlib_addr)pimg_row) & 7) >> 1;

		for (j = 0; j < align; j++)
			pimg_row[j] = color0;
		for (; j <= (img_width - 4); j += 4)
			*((TYPE_64BIT *) (pimg_row + j)) = dcolor;
		for (; j < img_width; j++)
			pimg_row[j] = color0;

		pimg_row = pimg + (img_height - 1 - i) * img_stride;
		align = ((8 - (mlib_addr)pimg_row) & 7) >> 1;
		for (j = 0; j < align; j++)
			pimg_row[j] = color0;
		for (; j <= (img_width - 4); j += 4)
			*((TYPE_64BIT *) (pimg_row + j)) = dcolor;
		for (; j < img_width; j++)
			pimg_row[j] = color0;
	}
}

/* *********************************************************** */

void
mlib_c_ImageClearEdge_S16_2(
    mlib_image *img,
    mlib_s32 dx,
    mlib_s32 dy,
    const mlib_s32 *color)
{
	mlib_u32 color0 = color[0] & 0xFFFF,
	    color1 = color[1] & 0xFFFF, col, col0, col1;

	VERT_EDGES(2, mlib_s16);

	if (img_width < 4)
		HORIZ_EDGES(2, mlib_s16);

#ifdef _LITTLE_ENDIAN
	col1 = (color0 << 16) | color1;
	col0 = (color1 << 16) | color0;
#else /* _LITTLE_ENDIAN */
	col0 = (color0 << 16) | color1;
	col1 = (color1 << 16) | color0;
#endif /* _LITTLE_ENDIAN */

	for (i = 0; i < dy; i++) {
		mlib_s16 *pimg_row = pimg + i * img_stride;
		mlib_s32 align = ((8 - (mlib_addr)pimg_row) & 7) >> 1;

		col = (align & 1) ? col1 : col0;
		((mlib_u32 *)dc)[0] = ((mlib_u32 *)dc)[1] = col;
		dcolor = dc[0];
		for (j = 0; j < align; j += 2) {
			pimg_row[j] = color0;
			pimg_row[j + 1] = color1;
		}

		for (j = align; j <= (img_width * 2 - 4); j += 4) {
			*((TYPE_64BIT *) (pimg_row + j)) = dcolor;
		}

		if (align & 1) {
			if (j < (img_width * 2 - 1)) {
				pimg_row[j] = color1;
				pimg_row[j + 1] = color0;
				j += 2;
			}

			pimg_row[j] = color1;
		} else {
			if (j < img_width * 2)
				*((mlib_s32 *)(pimg_row + j)) = col0;
		}

		pimg_row = pimg + (img_height - 1 - i) * img_stride;
		align = ((8 - (mlib_addr)pimg_row) & 7) >> 1;
		col = (align & 1) ? col1 : col0;
		((mlib_u32 *)dc)[0] = ((mlib_u32 *)dc)[1] = col;
		dcolor = dc[0];
		for (j = 0; j < align; j += 2) {
			pimg_row[j] = color0;
			pimg_row[j + 1] = color1;
		}

		for (j = align; j <= (img_width * 2 - 4); j += 4) {
			*((TYPE_64BIT *) (pimg_row + j)) = dcolor;
		}

		if (align & 1) {
			if (j < (img_width * 2 - 1)) {
				pimg_row[j] = color1;
				pimg_row[j + 1] = color0;
				j += 2;
			}

			pimg_row[j] = color1;
		} else {
			if (j < img_width * 2)
				*((mlib_s32 *)(pimg_row + j)) = col0;
		}
	}
}

/* *********************************************************** */

void
mlib_c_ImageClearEdge_S16_3(
    mlib_image *img,
    mlib_s32 dx,
    mlib_s32 dy,
    const mlib_s32 *color)
{
	mlib_u32 color0 = color[0] & 0xFFFF,
	    color1 = color[1] & 0xFFFF,
	    color2 = color[2] & 0xFFFF, col0, col1, col2;
	TYPE_64BIT dcolor1, dcolor2;

	VERT_EDGES(3, mlib_s16);

	if (img_width < 8)
		HORIZ_EDGES(3, mlib_s16);

#ifdef _LITTLE_ENDIAN
	col2 = (color2 << 16) | color1;
	col1 = (color0 << 16) | color2;
	col0 = (color1 << 16) | color0;
#else /* _LITTLE_ENDIAN */
	col0 = (color0 << 16) | color1;
	col1 = (color2 << 16) | color0;
	col2 = (color1 << 16) | color2;
#endif /* _LITTLE_ENDIAN */

	((mlib_u32 *)dc)[0] = col0;
	((mlib_u32 *)dc)[1] = col1;
	dcolor = dc[0];
	((mlib_u32 *)dc)[0] = col2;
	((mlib_u32 *)dc)[1] = col0;
	dcolor1 = dc[0];
	((mlib_u32 *)dc)[0] = col1;
	((mlib_u32 *)dc)[1] = col2;
	dcolor2 = dc[0];
	for (i = 0; i < dy; i++) {
		mlib_s16 *pimg_row = pimg + i * img_stride;

		for (j = 0; ((mlib_addr)(pimg_row + j) & 7) != 0; j += 3) {
			pimg_row[j] = color0;
			pimg_row[j + 1] = color1;
			pimg_row[j + 2] = color2;
		}

		for (; j <= (img_width * 3 - 12); j += 12) {
			*((TYPE_64BIT *) (pimg_row + j)) = dcolor;
			*((TYPE_64BIT *) (pimg_row + j + 4)) = dcolor1;
			*((TYPE_64BIT *) (pimg_row + j + 8)) = dcolor2;
		}

		for (; j < img_width * 3; j += 3) {
			pimg_row[j] = color0;
			pimg_row[j + 1] = color1;
			pimg_row[j + 2] = color2;
		}

		pimg_row = pimg + (img_height - 1 - i) * img_stride;
		for (j = 0; ((mlib_addr)(pimg_row + j) & 7) != 0; j += 3) {
			pimg_row[j] = color0;
			pimg_row[j + 1] = color1;
			pimg_row[j + 2] = color2;
		}

		for (; j <= (img_width * 3 - 12); j += 12) {
			*((TYPE_64BIT *) (pimg_row + j)) = dcolor;
			*((TYPE_64BIT *) (pimg_row + j + 4)) = dcolor1;
			*((TYPE_64BIT *) (pimg_row + j + 8)) = dcolor2;
		}

		for (; j < img_width * 3; j += 3) {
			pimg_row[j] = color0;
			pimg_row[j + 1] = color1;
			pimg_row[j + 2] = color2;
		}
	}
}

/* *********************************************************** */

void
mlib_c_ImageClearEdge_S16_4(
    mlib_image *img,
    mlib_s32 dx,
    mlib_s32 dy,
    const mlib_s32 *color)
{
	mlib_u32 color0 = color[0] & 0xFFFF,
	    color1 = color[1] & 0xFFFF,
	    color2 = color[2] & 0xFFFF,
	    color3 = color[3] & 0xFFFF, col0, col1, col2, col3, col00, col11;

	VERT_EDGES(4, mlib_s16);

	if (img_width < 4)
		HORIZ_EDGES(4, mlib_s16);

#ifdef _LITTLE_ENDIAN
	col2 = (color3 << 16) | color2;
	col1 = (color2 << 16) | color1;
	col0 = (color1 << 16) | color0;
	col3 = (color0 << 16) | color3;
#else /* _LITTLE_ENDIAN */
	col0 = (color0 << 16) | color1;
	col1 = (color1 << 16) | color2;
	col2 = (color2 << 16) | color3;
	col3 = (color3 << 16) | color0;
#endif /* _LITTLE_ENDIAN */

	for (i = 0; i < dy; i++) {
		mlib_s16 *pimg_row = pimg + i * img_stride;
		mlib_s32 align = ((8 - (mlib_addr)pimg_row) & 7) >> 1;

		if (align & 2) {
			if (align & 1) {
				col00 = col3;
				col11 = col1;
			} else {
				col00 = col2;
				col11 = col0;
			}
		} else {
			if (align & 1) {
				col00 = col1;
				col11 = col3;
			} else {
				col00 = col0;
				col11 = col2;
			}
		}

		((mlib_u32 *)dc)[0] = col00;
		((mlib_u32 *)dc)[1] = col11;
		dcolor = dc[0];
		for (j = 0; j < align; j += 4) {
			pimg_row[j] = color0;
			pimg_row[j + 1] = color1;
			pimg_row[j + 2] = color2;
			pimg_row[j + 3] = color3;
		}

		for (j = align; j <= (img_width * 4 - 4); j += 4) {
			*((TYPE_64BIT *) (pimg_row + j)) = dcolor;
		}

		if (j <= (img_width * 4 - 2)) {
			*((mlib_s32 *)(pimg_row + j)) = col00;
			j += 2;
		}

		if (align & 1)
			pimg_row[j] = color3;

		pimg_row = pimg + (img_height - 1 - i) * img_stride;
		align = ((8 - (mlib_addr)pimg_row) & 7) >> 1;

		if (align & 2) {
			if (align & 1) {
				col00 = col3;
				col11 = col1;
			} else {
				col00 = col2;
				col11 = col0;
			}
		} else {
			if (align & 1) {
				col00 = col1;
				col11 = col3;
			} else {
				col00 = col0;
				col11 = col2;
			}
		}

		((mlib_u32 *)dc)[0] = col00;
		((mlib_u32 *)dc)[1] = col11;
		dcolor = dc[0];
		for (j = 0; j < align; j += 4) {
			pimg_row[j] = color0;
			pimg_row[j + 1] = color1;
			pimg_row[j + 2] = color2;
			pimg_row[j + 3] = color3;
		}

		for (j = align; j <= (img_width * 4 - 4); j += 4) {
			*((TYPE_64BIT *) (pimg_row + j)) = dcolor;
		}

		if (j <= (img_width * 4 - 2)) {
			*((mlib_s32 *)(pimg_row + j)) = col00;
			j += 2;
		}

		if (align & 1)
			pimg_row[j] = color3;
	}
}

/* *********************************************************** */

void
mlib_c_ImageClearEdge_S32_1(
    mlib_image *img,
    mlib_s32 dx,
    mlib_s32 dy,
    const mlib_s32 *color)
{
	mlib_u32 color0 = color[0];

	VERT_EDGES(1, mlib_s32);

	if (img_width < 4)
		HORIZ_EDGES(1, mlib_s32);

	((mlib_s32 *)dc)[0] = ((mlib_s32 *)dc)[1] = color0;
	dcolor = dc[0];
	for (i = 0; i < dy; i++) {
		mlib_s32 *pimg_row = pimg + i * img_stride;
		mlib_s32 align = (mlib_addr)pimg_row & 7;

		j = 0;

		if (align)
			pimg_row[j++] = color0;
		for (; j <= (img_width - 2); j += 2)
			*((TYPE_64BIT *) (pimg_row + j)) = dcolor;
		for (; j < img_width; j++)
			pimg_row[j] = color0;

		pimg_row = pimg + (img_height - 1 - i) * img_stride;
		align = (mlib_addr)pimg_row & 7;
		j = 0;

		if (align)
			pimg_row[j++] = color0;
		for (; j <= (img_width - 2); j += 2)
			*((TYPE_64BIT *) (pimg_row + j)) = dcolor;
		for (; j < img_width; j++)
			pimg_row[j] = color0;
	}
}

/* *********************************************************** */

void
mlib_c_ImageClearEdge_S32_2(
    mlib_image *img,
    mlib_s32 dx,
    mlib_s32 dy,
    const mlib_s32 *color)
{
	mlib_u32 color0 = color[0], color1 = color[1];

	VERT_EDGES(2, mlib_s32);

	if (img_width < 2)
		HORIZ_EDGES(2, mlib_s32);

	for (i = 0; i < dy; i++) {
		mlib_s32 *pimg_row = pimg + i * img_stride;
		mlib_s32 align = (mlib_addr)pimg_row & 7;

		j = 0;

		if (align) {
			pimg_row[j] = color0;
			((mlib_u32 *)dc)[0] = color1;
			((mlib_u32 *)dc)[1] = color0;
			pimg_row[j + img_width * 2 - 1] = color1;
			j++;
		} else {
			((mlib_u32 *)dc)[0] = color0;
			((mlib_u32 *)dc)[1] = color1;
		}

		dcolor = dc[0];
		for (; j <= (img_width * 2 - 2); j += 2) {
			*((TYPE_64BIT *) (pimg_row + j)) = dcolor;
		}

		pimg_row = pimg + (img_height - 1 - i) * img_stride;
		align = (mlib_addr)pimg_row & 7;
		j = 0;

		if (align) {
			pimg_row[j] = color0;
			((mlib_u32 *)dc)[0] = color1;
			((mlib_u32 *)dc)[1] = color0;
			pimg_row[j + img_width * 2 - 1] = color1;
			j++;
		} else {
			((mlib_u32 *)dc)[0] = color0;
			((mlib_u32 *)dc)[1] = color1;
		}

		dcolor = dc[0];
		for (; j <= (img_width * 2 - 2); j += 2) {
			*((TYPE_64BIT *) (pimg_row + j)) = dcolor;
		}
	}
}

/* *********************************************************** */

void
mlib_c_ImageClearEdge_S32_3(
    mlib_image *img,
    mlib_s32 dx,
    mlib_s32 dy,
    const mlib_s32 *color)
{
	mlib_u32 color0 = color[0], color1 = color[1], color2 = color[2];
	TYPE_64BIT dcolor1, dcolor2;

	VERT_EDGES(3, mlib_s32);

	if (img_width < 8)
		HORIZ_EDGES(3, mlib_s32);

	((mlib_u32 *)dc)[0] = color0;
	((mlib_u32 *)dc)[1] = color1;
	dcolor = dc[0];
	((mlib_u32 *)dc)[0] = color2;
	((mlib_u32 *)dc)[1] = color0;
	dcolor1 = dc[0];
	((mlib_u32 *)dc)[0] = color1;
	((mlib_u32 *)dc)[1] = color2;
	dcolor2 = dc[0];
	for (i = 0; i < dy; i++) {
		mlib_s32 *pimg_row = pimg + i * img_stride;
		mlib_s32 align = (mlib_addr)pimg_row & 7;

		j = 0;

		if (align) {
			pimg_row[j] = color0;
			pimg_row[j + 1] = color1;
			pimg_row[j + 2] = color2;
			j += 3;
		}

		for (; j <= (img_width * 3 - 6); j += 6) {
			*((TYPE_64BIT *) (pimg_row + j)) = dcolor;
			*((TYPE_64BIT *) (pimg_row + j + 2)) = dcolor1;
			*((TYPE_64BIT *) (pimg_row + j + 4)) = dcolor2;
		}

		if (j < img_width * 3) {
			pimg_row[j] = color0;
			pimg_row[j + 1] = color1;
			pimg_row[j + 2] = color2;
		}

		pimg_row = pimg + (img_height - 1 - i) * img_stride;
		align = (mlib_addr)pimg_row & 7;
		j = 0;

		if (align) {
			pimg_row[j] = color0;
			pimg_row[j + 1] = color1;
			pimg_row[j + 2] = color2;
			j += 3;
		}

		for (; j <= (img_width * 3 - 6); j += 6) {
			*((TYPE_64BIT *) (pimg_row + j)) = dcolor;
			*((TYPE_64BIT *) (pimg_row + j + 2)) = dcolor1;
			*((TYPE_64BIT *) (pimg_row + j + 4)) = dcolor2;
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
mlib_c_ImageClearEdge_S32_4(
    mlib_image *img,
    mlib_s32 dx,
    mlib_s32 dy,
    const mlib_s32 *color)
{
	mlib_u32 color0 = color[0],
	    color1 = color[1], color2 = color[2], color3 = color[3];
	TYPE_64BIT dcolor1, dc1[1];

	VERT_EDGES(4, mlib_s32);

	if (img_width < 2)
		HORIZ_EDGES(4, mlib_s32);

	for (i = 0; i < dy; i++) {
		mlib_s32 *pimg_row = pimg + i * img_stride;
		mlib_s32 align = (mlib_addr)pimg_row & 7;

		j = 0;

		if (align) {
			pimg_row[j] = color0;
			((mlib_u32 *)dc)[0] = color1;
			((mlib_u32 *)dc)[1] = color2;
			((mlib_u32 *)dc1)[0] = color3;
			((mlib_u32 *)dc1)[1] = color0;
			*((TYPE_64BIT *) (pimg_row + j + img_width * 4 - 3)) =
			    dc[0];
			pimg_row[j + img_width * 4 - 1] = color3;
			j++;
		} else {
			((mlib_u32 *)dc)[0] = color0;
			((mlib_u32 *)dc)[1] = color1;
			((mlib_u32 *)dc1)[0] = color2;
			((mlib_u32 *)dc1)[1] = color3;
		}

		dcolor = dc[0];
		dcolor1 = dc1[0];
		for (; j <= (img_width * 4 - 4); j += 4) {
			*((TYPE_64BIT *) (pimg_row + j)) = dcolor;
			*((TYPE_64BIT *) (pimg_row + j) + 1) = dcolor1;
		}

		pimg_row = pimg + (img_height - 1 - i) * img_stride;
		align = (mlib_addr)pimg_row & 7;
		j = 0;

		if (align) {
			pimg_row[j] = color0;
			((mlib_u32 *)dc)[0] = color1;
			((mlib_u32 *)dc)[1] = color2;
			((mlib_u32 *)dc1)[0] = color3;
			((mlib_u32 *)dc1)[1] = color0;
			*((TYPE_64BIT *) (pimg_row + j + img_width * 4 - 3)) =
			    dc[0];
			pimg_row[j + img_width * 4 - 1] = color3;
			j++;
		} else {
			((mlib_u32 *)dc)[0] = color0;
			((mlib_u32 *)dc)[1] = color1;
			((mlib_u32 *)dc1)[0] = color2;
			((mlib_u32 *)dc1)[1] = color3;
		}

		dcolor = dc[0];
		dcolor1 = dc1[0];
		for (; j <= (img_width * 4 - 4); j += 4) {
			*((TYPE_64BIT *) (pimg_row + j)) = dcolor;
			*((TYPE_64BIT *) (pimg_row + j) + 1) = dcolor1;
		}
	}
}

/* *********************************************************** */
