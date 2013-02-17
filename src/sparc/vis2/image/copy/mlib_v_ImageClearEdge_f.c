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

#pragma ident	"@(#)mlib_v_ImageClearEdge_f.c	9.2	07/11/05 SMI"

/*
 * DESCRIPTION
 *      Set edge of an image to a specific color.
 *      -- VIS2 version low level functions.
 *
 * NOTE
 *      These functions are separated from mlib_v_ImageClearEdge.c
 *      for structure clarity.
 */

#include <vis_proto.h>
#include <mlib_image.h>
#include <mlib_v_ImageClearEdge_f.h>

/* *********************************************************** */

#define	VERT_EDGES1(type)                                                 \
	type *pimg = (type *) mlib_ImageGetData(img);                     \
	mlib_s32 img_height = mlib_ImageGetHeight(img);                   \
	mlib_s32 img_width = mlib_ImageGetWidth(img);                     \
	mlib_s32 img_stride = mlib_ImageGetStride(img) / sizeof (type);   \
	    mlib_s32                                                      \
		i,                                                        \
		j,                                                        \
		l;                                                        \
	    mlib_s32                                                      \
		emask;                                                    \
	    mlib_d64                                                      \
	    dcolor, *                                                     \
		dpimg;                                                    \
	    type                                                          \
		color_0 = color[0];                                       \
	                                                                  \
	for (i = dy; i < (img_height - dy); i++)                          \
	{                                                                 \
	for (j = 0; j < dx; j++) {                                        \
	    pimg[i * img_stride + j] = color_0;                           \
	    pimg[i * img_stride + (img_width - 1 - j)] = color_0;         \
	}                                                                 \
	}                                                                 \
	                                                                  \
	if (dy == 0)                                                      \
	    return;                                                       \
	if ((img_width) == img_stride) {                                  \
	    img_width *= dy;                                              \
	    img_stride *= (img_height - dy);                              \
	    img_height = 2;                                               \
	    dy = 1;                                                       \
	}

/* *********************************************************** */

#define	VERT_EDGES2(type)                                                 \
	type *pimg = (type *) mlib_ImageGetData(img);                     \
	mlib_s32 img_height = mlib_ImageGetHeight(img);                   \
	mlib_s32 img_width = mlib_ImageGetWidth(img);                     \
	mlib_s32 img_stride = mlib_ImageGetStride(img) / sizeof (type);   \
	    mlib_s32                                                      \
		i,                                                        \
		j,                                                        \
		l;                                                        \
	    mlib_s32                                                      \
		emask;                                                    \
	    mlib_d64                                                      \
	    dcolor, *                                                     \
		dpimg;                                                    \
	    type                                                          \
		color_0 = color[0];                                       \
	    type                                                          \
		color_1 = color[1];                                       \
	                                                                  \
	for (i = dy; i < (img_height - dy); i++)                          \
	{                                                                 \
	for (j = 0; j < dx; j++) {                                        \
	    pimg[i * img_stride + 2 * j] = color_0;                       \
	    pimg[i * img_stride + 2 * j + 1] = color_1;                   \
	    pimg[i * img_stride + 2 * (img_width - 1 - j)] = color_0;     \
	    pimg[i * img_stride + 2 * (img_width - 1 - j) + 1] =          \
		color_1;                                                  \
	}                                                                 \
	}                                                                 \
	                                                                  \
	if (dy == 0)                                                      \
	    return;                                                       \
	if ((img_width * 2) == img_stride) {                              \
	    img_width *= dy;                                              \
	    img_stride *= (img_height - dy);                              \
	    img_height = 2;                                               \
	    dy = 1;                                                       \
	}

/* *********************************************************** */

#define	VERT_EDGES3(type)                                                 \
	type *pimg = (type *) mlib_ImageGetData(img);                     \
	mlib_s32 img_height = mlib_ImageGetHeight(img);                   \
	mlib_s32 img_width = mlib_ImageGetWidth(img);                     \
	mlib_s32 img_stride = mlib_ImageGetStride(img) / sizeof (type);   \
	    mlib_s32                                                      \
		i,                                                        \
		j,                                                        \
		l;                                                        \
	    mlib_s32                                                      \
		emask;                                                    \
	    mlib_d64                                                      \
	    dcolor, *                                                     \
		dpimg;                                                    \
	    type                                                          \
		color_0 = color[0];                                       \
	    type                                                          \
		color_1 = color[1];                                       \
	    type                                                          \
		color_2 = color[2];                                       \
	                                                                  \
	for (i = dy; i < (img_height - dy); i++)                          \
	{                                                                 \
	for (j = 0; j < dx; j++) {                                        \
	    pimg[i * img_stride + 3 * j] = color_0;                       \
	    pimg[i * img_stride + 3 * j + 1] = color_1;                   \
	    pimg[i * img_stride + 3 * j + 2] = color_2;                   \
	    pimg[i * img_stride + 3 * (img_width - 1 - j)] = color_0;     \
	    pimg[i * img_stride + 3 * (img_width - 1 - j) + 1] =          \
		color_1;                                                  \
	    pimg[i * img_stride + 3 * (img_width - 1 - j) + 2] =          \
		color_2;                                                  \
	}                                                                 \
	}                                                                 \
	                                                                  \
	if (dy == 0)                                                      \
	    return;                                                       \
	if ((img_width * 3) == img_stride) {                              \
	    img_width *= dy;                                              \
	    img_stride *= (img_height - dy);                              \
	    img_height = 2;                                               \
	    dy = 1;                                                       \
	}

/* *********************************************************** */

#define	VERT_EDGES4(type)                                                 \
	type *pimg = (type *) mlib_ImageGetData(img);                     \
	mlib_s32 img_height = mlib_ImageGetHeight(img);                   \
	mlib_s32 img_width = mlib_ImageGetWidth(img);                     \
	mlib_s32 img_stride = mlib_ImageGetStride(img) / sizeof (type);   \
	    mlib_s32                                                      \
		i,                                                        \
		j,                                                        \
		l;                                                        \
	    mlib_s32                                                      \
		emask;                                                    \
	    mlib_d64                                                      \
	    dcolor, *                                                     \
		dpimg;                                                    \
	    type                                                          \
		color_0 = color[0];                                       \
	    type                                                          \
		color_1 = color[1];                                       \
	    type                                                          \
		color_2 = color[2];                                       \
	    type                                                          \
		color_3 = color[3];                                       \
	                                                                  \
	for (i = dy; i < (img_height - dy); i++)                          \
	{                                                                 \
	for (j = 0; j < dx; j++) {                                        \
	    pimg[i * img_stride + 4 * j] = color_0;                       \
	    pimg[i * img_stride + 4 * j + 1] = color_1;                   \
	    pimg[i * img_stride + 4 * j + 2] = color_2;                   \
	    pimg[i * img_stride + 4 * j + 3] = color_3;                   \
	    pimg[i * img_stride + 4 * (img_width - 1 - j)] = color_0;     \
	    pimg[i * img_stride + 4 * (img_width - 1 - j) + 1] =          \
		color_1;                                                  \
	    pimg[i * img_stride + 4 * (img_width - 1 - j) + 2] =          \
		color_2;                                                  \
	    pimg[i * img_stride + 4 * (img_width - 1 - j) + 3] =          \
		color_3;                                                  \
	}                                                                 \
	}                                                                 \
	                                                                  \
	if (dy == 0)                                                      \
	    return;                                                       \
	if ((img_width * 4) == img_stride) {                              \
	    img_width *= dy;                                              \
	    img_stride *= (img_height - dy);                              \
	    img_height = 2;                                               \
	    dy = 1;                                                       \
	}

/* *********************************************************** */

#define	VERT_EDGES_BIT()                                                   \
	mlib_u8 *pimg = (mlib_u8 *)mlib_ImageGetData(img);                 \
	mlib_s32 img_height = mlib_ImageGetHeight(img);                    \
	mlib_s32 img_width = mlib_ImageGetWidth(img);                      \
	mlib_s32 img_stride = mlib_ImageGetStride(img);                    \
	mlib_s32 img_bitoff = mlib_ImageGetBitOffset(img);                 \
	mlib_s32 i, j;                                                     \
	mlib_s32 emask;                                                    \
	mlib_d64 dcolor, *dpimg;                                           \
	mlib_u32 ii0, ii1, col0, col1, mask0, mask1;                       \
	mlib_u8 dst;                                                       \
	mlib_s32 color_i = color[0] & 1;                                   \
	                                                                   \
	for (i = 0; i < dx; i++) {                                         \
	    for (j = dy; j < (img_height - dy); j++) {                     \
		ii0 = (i + img_bitoff) >> 3;                               \
		ii1 = ((img_width - 1 - i) + img_bitoff) >> 3;             \
		col0 = color_i << ((7 - (i + img_bitoff)) & 7);            \
		col1 =                                                     \
		    color_i << ((7 - ((img_width - 1 - i) +                \
		    img_bitoff)) & 7);                                     \
		mask0 = 1 << ((7 - (i + img_bitoff)) & 7);                 \
		mask1 =                                                    \
		    1 << ((7 - ((img_width - 1 - i) + img_bitoff)) & 7);   \
		dst = pimg[j * img_stride + ii0];                          \
		pimg[j * img_stride + ii0] = (dst & (~mask0)) | col0;      \
		dst = pimg[j * img_stride + ii1];                          \
		pimg[j * img_stride + ii1] = (dst & (~mask1)) | col1;      \
	    }                                                              \
	}                                                                  \
	                                                                   \
	if (((img_width) == img_stride << 3) && (dy != 0)) {               \
	    img_width *= dy;                                               \
	    img_stride *= (img_height - dy);                               \
	    img_height = 2;                                                \
	    dy = 1;                                                        \
	}

/* *********************************************************** */

#define	VERT_EDGES_BIT1(chan)                                             \
	mlib_u8 *pimg = (mlib_u8 *)mlib_ImageGetData(img);                \
	mlib_s32 img_height = mlib_ImageGetHeight(img);                   \
	mlib_s32 img_width = mlib_ImageGetWidth(img);                     \
	mlib_s32 img_stride = mlib_ImageGetStride(img);                   \
	mlib_s32 img_bitoff = mlib_ImageGetBitOffset(img);                \
	mlib_s32 i, j;                                                    \
	mlib_s32 emask;                                                   \
	mlib_d64 *dpimg;                                                  \
	mlib_u32 ii0, ii1, col0, col1, mask0, mask1, mask;                \
	mlib_u8 dst;                                                      \
	mlib_s32 color_0 = 0;                                             \
	                                                                  \
	for (i = 0; i < chan; i++)                                        \
	    color_0 = (color_0 << 1) | (color[i] & 1);                    \
	color_0 <<= (4 - chan);                                           \
	mask = ((1 << chan) - 1) << (4 - chan);                           \
	for (j = dy; j < (img_height - dy); j++) {                        \
	    for (i = 0; i < dx; i++) {                                    \
		ii0 = (i * chan + img_bitoff) >> 3;                       \
		ii1 = ((img_width - 1 - i) * chan + img_bitoff) >> 3;     \
		col0 = color_0 << (12 - ((i * chan + img_bitoff) & 7));   \
		col1 =                                                    \
		    color_0 << (12 - (((img_width - 1 - i) * chan +       \
		    img_bitoff) & 7));                                    \
		mask0 = mask << (12 - ((i * chan + img_bitoff) & 7));     \
		mask1 =                                                   \
		    mask << (12 - (((img_width - 1 - i) * chan +          \
		    img_bitoff) & 7));                                    \
		dst = pimg[j * img_stride + ii0];                         \
		pimg[j * img_stride + ii0] =                              \
		    (dst & (~(mask0 >> 8))) | (col0 >> 8);                \
		dst = pimg[j * img_stride + ii0 + 1];                     \
		pimg[j * img_stride + ii0 + 1] =                          \
		    (dst & (~mask0)) | col0;                              \
		dst = pimg[j * img_stride + ii1];                         \
		pimg[j * img_stride + ii1] =                              \
		    (dst & (~(mask1 >> 8))) | (col1 >> 8);                \
		dst = pimg[j * img_stride + ii1 + 1];                     \
		pimg[j * img_stride + ii1 + 1] =                          \
		    (dst & (~mask1)) | col1;                              \
	    }                                                             \
	}                                                                 \
	                                                                  \
	if (((img_width * chan) == img_stride << 3) && (dy != 0)) {       \
	    img_width *= dy;                                              \
	    img_stride *= (img_height - dy);                              \
	    img_height = 2;                                               \
	    dy = 1;                                                       \
	}

/* *********************************************************** */

#define	HORIZ_EDGES(chan, type)                                          \
	{                                                                \
	    for (l = 0; l < chan; l++) {                                 \
		type color_i = color[l];                                 \
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
mlib_v_ImageClearEdge_BIT_1(
    mlib_image *img,
    mlib_s32 dx,
    mlib_s32 dy,
    const mlib_s32 *color)
{
	mlib_s32 b_j, k;
	mlib_u8 bcolor0, bmask, src;
	mlib_u32 color0;
	mlib_u8 *pimg_row, *pimg_row_end;

	VERT_EDGES_BIT();

	color0 = ((color[0] & 1) << 31) >> 31;
	bcolor0 = color0 & 0xFF;

	dcolor = vis_to_double_dup(color0);
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
			b_j = (img_width - j) / 8;
		}

		if (b_j < 16) {
			int ii;

			for (ii = 0; ii < b_j; ii++)
				pimg_row[ii] = bcolor0;

			pimg_row += ii;
			j += ii << 3;

			if (j < img_width) {
				bmask = (0xFF << (8 - (img_width - j))) & 0xFF;
				src = pimg_row[0];
				pimg_row[0] = (src & ~bmask) | (color0 & bmask);
			}

			continue;
		}

		pimg_row_end = pimg_row + b_j - 1;
		dpimg = (mlib_d64 *)vis_alignaddr(pimg_row, 0);

		emask = vis_edge8(pimg_row, pimg_row_end);
		vis_pst_8(dcolor, dpimg++, emask);
		k = (mlib_addr)dpimg - (mlib_addr)pimg_row;
		for (; k < (b_j - 8); k += 8)
			(*dpimg++) = dcolor;
		emask = vis_edge8(dpimg, pimg_row_end);
		vis_pst_8(dcolor, dpimg, emask);
		j += b_j << 3;

		if (j < img_width) {
			pimg_row = (mlib_u8 *)(pimg_row_end + 1);
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
			b_j = (img_width - j) / 8;
		}

		if (b_j < 16) {
			int ii;

			for (ii = 0; ii < b_j; ii++)
				pimg_row[ii] = bcolor0;

			pimg_row += ii;
			j += ii << 3;

			if (j < img_width) {
				bmask = (0xFF << (8 - (img_width - j))) & 0xFF;
				src = pimg_row[0];
				pimg_row[0] = (src & ~bmask) | (color0 & bmask);
			}

			continue;
		}

		pimg_row_end = pimg_row + b_j - 1;
		dpimg = (mlib_d64 *)vis_alignaddr(pimg_row, 0);

		emask = vis_edge8(pimg_row, pimg_row_end);
		vis_pst_8(dcolor, dpimg++, emask);
		k = (mlib_addr)dpimg - (mlib_addr)pimg_row;
		for (; k < (b_j - 8); k += 8)
			(*dpimg++) = dcolor;
		emask = vis_edge8(dpimg, pimg_row_end);
		vis_pst_8(dcolor, dpimg, emask);
		j += b_j << 3;

		if (j < img_width) {
			pimg_row = (mlib_u8 *)(pimg_row_end + 1);
			bmask = (0xFF << (8 - (img_width - j))) & 0xFF;
			src = pimg_row[0];
			pimg_row[0] = (src & ~bmask) | (color0 & bmask);
		}
	}
}

/* *********************************************************** */

void
mlib_v_ImageClearEdge_BIT_2(
    mlib_image *img,
    mlib_s32 dx,
    mlib_s32 dy,
    const mlib_s32 *color)
{
	mlib_s32 b_j, k;
	mlib_u8 bcolor0, bmask, src;
	mlib_u8 *pimg_row, *pimg_row_end;
	mlib_u32 color0 = color[0] & 1, color1 = color[1] & 1;
	mlib_d64 dcolor;

	VERT_EDGES_BIT1(2);

	img_width <<= 1;

	color1 = (color0 << 1) | color1;
	color1 = (color1 << 2) | color1;
	color1 = (color1 << 4) | color1;
	color0 = ((color1 << 1) & 0xFE) | color0;
	bcolor0 = ((img_bitoff & 1) == 0) ? color1 : color0;
	color0 = (bcolor0 << 8) | bcolor0;
	color0 = (color0 << 16) | color0;

	dcolor = vis_to_double_dup(color0);
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
			b_j = (img_width - j) / 8;
		}

		if (b_j < 16) {
			mlib_s32 ii;

			for (ii = 0; ii < b_j; ii++)
				pimg_row[ii] = bcolor0;

			pimg_row += ii;
			j += ii << 3;

			if (j < img_width) {
				bmask = (0xFF << (8 - (img_width - j))) & 0xFF;
				src = pimg_row[0];
				pimg_row[0] = (src & ~bmask) | (color0 & bmask);
			}

			continue;
		}

		pimg_row_end = pimg_row + b_j - 1;
		dpimg = (mlib_d64 *)vis_alignaddr(pimg_row, 0);

		emask = vis_edge8(pimg_row, pimg_row_end);
		vis_pst_8(dcolor, dpimg++, emask);
		k = (mlib_addr)dpimg - (mlib_addr)pimg_row;
		for (; k < (b_j - 8); k += 8)
			(*dpimg++) = dcolor;
		emask = vis_edge8(dpimg, pimg_row_end);
		vis_pst_8(dcolor, dpimg, emask);
		j += b_j << 3;

		if (j < img_width) {
			pimg_row = (mlib_u8 *)(pimg_row_end + 1);
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
			b_j = (img_width - j) / 8;
		}

		if (b_j < 16) {
			mlib_s32 ii;

			for (ii = 0; ii < b_j; ii++)
				pimg_row[ii] = bcolor0;

			pimg_row += ii;
			j += ii << 3;

			if (j < img_width) {
				bmask = (0xFF << (8 - (img_width - j))) & 0xFF;
				src = pimg_row[0];
				pimg_row[0] = (src & ~bmask) | (color0 & bmask);
			}

			continue;
		}

		pimg_row_end = pimg_row + b_j - 1;
		dpimg = (mlib_d64 *)vis_alignaddr(pimg_row, 0);

		emask = vis_edge8(pimg_row, pimg_row_end);
		vis_pst_8(dcolor, dpimg++, emask);
		k = (mlib_addr)dpimg - (mlib_addr)pimg_row;
		for (; k < (b_j - 8); k += 8)
			(*dpimg++) = dcolor;
		emask = vis_edge8(dpimg, pimg_row_end);
		vis_pst_8(dcolor, dpimg, emask);
		j += b_j << 3;

		if (j < img_width) {
			pimg_row = (mlib_u8 *)(pimg_row_end + 1);
			bmask = (0xFF << (8 - (img_width - j))) & 0xFF;
			src = pimg_row[0];
			pimg_row[0] = (src & ~bmask) | (color0 & bmask);
		}
	}
}

/* *********************************************************** */

void
mlib_v_ImageClearEdge_BIT_3(
    mlib_image *img,
    mlib_s32 dx,
    mlib_s32 dy,
    const mlib_s32 *color)
{
	mlib_s32 b_j, k, bit_shift;
	mlib_u8 bcolor, bmask, src;
	mlib_u8 *pimg_row, *pimg_row_end;
	mlib_d64 dcolor0, dcolor1, dcolor2;
	mlib_d64 dcolor00, dcolor11, dcolor22;
	mlib_u32 color0 = color[0] & 1,
	    color1 = color[1] & 1, color2 = color[2] & 1;
	mlib_u32 col2;

	VERT_EDGES_BIT1(3);

	img_width *= 3;

	col0 = (color0 << 3) | (color1 << 2) | (color2 << 1) | color0;
	col1 = (col0 >> 1) | (color2 << 3);
	col2 = (col1 >> 1) | (color1 << 3);
	color0 = (col0 << 4) | col2;
	color1 = (col1 << 4) | col0;
	color2 = (col2 << 4) | col1;

	color0 = (color0 << 24) | (color1 << 16) | (color2 << 8) | color0;
	color1 = (color0 << 8) | color1;
	color2 = (color1 << 8) | color2;

	dcolor0 = vis_to_double(color0, color1);
	dcolor1 = vis_to_double(color2, color0);
	dcolor2 = vis_to_double(color1, color2);

	for (i = 0; i < dy; i++) {
		pimg_row = pimg + i * img_stride;

		if (img_bitoff + img_width <= 8) {
			bmask =
			    (0xFF >> (8 - img_width)) << (8 - img_bitoff -
			    img_width);
			src = pimg_row[0];
			bcolor = (color0 >> img_bitoff) & 0xFF;
			pimg_row[0] = (src & ~bmask) | (bcolor & bmask);
			continue;
		} else {
			bmask = 0xFF >> img_bitoff;
			src = pimg_row[0];
			bcolor = (color0 >> img_bitoff) & 0xFF;
			bit_shift =
			    (((mlib_addr)pimg_row & 7) << 3) + img_bitoff;
			pimg_row[0] = (src & ~bmask) | (bcolor & bmask);
			pimg_row++;
			j = 8 - img_bitoff;
			b_j = (img_width - j) / 8;
		}

		pimg_row_end = pimg_row + b_j - 1;

		dpimg = (mlib_d64 *)((mlib_addr)pimg_row & ~7);
		vis_alignaddr((void *)(bit_shift % 3), 0);
		dcolor22 = vis_faligndata(dcolor0, dcolor1);
		dcolor00 = vis_faligndata(dcolor1, dcolor2);
		dcolor11 = vis_faligndata(dcolor2, dcolor0);
		emask = vis_edge8(pimg_row, pimg_row_end);

		if ((mlib_addr)pimg_row & 7)
			vis_pst_8(dcolor22, dpimg++, emask);
		k = (mlib_addr)dpimg - (mlib_addr)pimg_row;
		for (; k <= (b_j - 24); k += 24) {
			dpimg[0] = dcolor00;
			dpimg[1] = dcolor11;
			dpimg[2] = dcolor22;
			dpimg += 3;
		}

		if (k < b_j) {
			if (k < (b_j - 8)) {
				(*dpimg++) = dcolor00;

				if (k < (b_j - 16)) {
					(*dpimg++) = dcolor11;
					dcolor00 = dcolor22;
				} else
					dcolor00 = dcolor11;
			}

			emask = vis_edge8(dpimg, pimg_row_end);
			vis_pst_8(dcolor00, dpimg, emask);
		}

		j = img_width - j - (b_j << 3);

		if (j > 0) {
			pimg_row = (mlib_u8 *)(pimg_row_end + 1);
			bmask = (0xFF << (8 - j)) & 0xFF;
			bcolor = (color0 >> j) & 0xFF;
			src = pimg_row[0];
			pimg_row[0] = (src & ~bmask) | (bcolor & bmask);
		}
	}

	pimg = pimg + (img_height - dy) * img_stride;

	for (i = 0; i < dy; i++) {
		pimg_row = pimg + i * img_stride;

		if (img_bitoff + img_width <= 8) {
			bmask =
			    (0xFF >> (8 - img_width)) << (8 - img_bitoff -
			    img_width);
			src = pimg_row[0];
			bcolor = (color0 >> img_bitoff) & 0xFF;
			pimg_row[0] = (src & ~bmask) | (bcolor & bmask);
			continue;
		} else {
			bmask = 0xFF >> img_bitoff;
			src = pimg_row[0];
			bcolor = (color0 >> img_bitoff) & 0xFF;
			bit_shift =
			    (((mlib_addr)pimg_row & 7) << 3) + img_bitoff;
			pimg_row[0] = (src & ~bmask) | (bcolor & bmask);
			pimg_row++;
			j = 8 - img_bitoff;
			b_j = (img_width - j) / 8;
		}

		pimg_row_end = pimg_row + b_j - 1;

		dpimg = (mlib_d64 *)((mlib_addr)pimg_row & ~7);
		vis_alignaddr((void *)(bit_shift % 3), 0);
		dcolor22 = vis_faligndata(dcolor0, dcolor1);
		dcolor00 = vis_faligndata(dcolor1, dcolor2);
		dcolor11 = vis_faligndata(dcolor2, dcolor0);
		emask = vis_edge8(pimg_row, pimg_row_end);

		if ((mlib_addr)pimg_row & 7)
			vis_pst_8(dcolor22, dpimg++, emask);
		k = (mlib_addr)dpimg - (mlib_addr)pimg_row;
		for (; k <= (b_j - 24); k += 24) {
			dpimg[0] = dcolor00;
			dpimg[1] = dcolor11;
			dpimg[2] = dcolor22;
			dpimg += 3;
		}

		if (k < b_j) {
			if (k < (b_j - 8)) {
				(*dpimg++) = dcolor00;

				if (k < (b_j - 16)) {
					(*dpimg++) = dcolor11;
					dcolor00 = dcolor22;
				} else
					dcolor00 = dcolor11;
			}

			emask = vis_edge8(dpimg, pimg_row_end);
			vis_pst_8(dcolor00, dpimg, emask);
		}

		j = img_width - j - (b_j << 3);

		if (j > 0) {
			pimg_row = (mlib_u8 *)(pimg_row_end + 1);
			bmask = (0xFF << (8 - j)) & 0xFF;
			bcolor = (color0 >> j) & 0xFF;
			src = pimg_row[0];
			pimg_row[0] = (src & ~bmask) | (bcolor & bmask);
		}
	}
}

/* *********************************************************** */

void
mlib_v_ImageClearEdge_BIT_4(
    mlib_image *img,
    mlib_s32 dx,
    mlib_s32 dy,
    const mlib_s32 *color)
{
	mlib_s32 b_j, k;
	mlib_u8 bcolor0, bmask, src;
	mlib_u8 *pimg_row, *pimg_row_end;
	mlib_u32 color0 = color[0] & 1,
	    color1 = color[1] & 1, color2 = color[2] & 1, color3 = color[3] & 1;
	mlib_d64 dcolor;

	VERT_EDGES_BIT1(4);

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

	dcolor = vis_to_double_dup(color0);
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
			b_j = (img_width - j) / 8;
		}

		if (b_j < 16) {
			int ii;

			for (ii = 0; ii < b_j; ii++)
				pimg_row[ii] = bcolor0;

			pimg_row += ii;
			j += ii << 3;

			if (j < img_width) {
				bmask = (0xFF << (8 - (img_width - j))) & 0xFF;
				src = pimg_row[0];
				pimg_row[0] = (src & ~bmask) | (color0 & bmask);
			}

			continue;
		}

		pimg_row_end = pimg_row + b_j - 1;
		dpimg = (mlib_d64 *)vis_alignaddr(pimg_row, 0);

		emask = vis_edge8(pimg_row, pimg_row_end);
		vis_pst_8(dcolor, dpimg++, emask);
		k = (mlib_addr)dpimg - (mlib_addr)pimg_row;
		for (; k < (b_j - 8); k += 8)
			(*dpimg++) = dcolor;
		emask = vis_edge8(dpimg, pimg_row_end);
		vis_pst_8(dcolor, dpimg, emask);
		j += b_j << 3;

		if (j < img_width) {
			pimg_row = (mlib_u8 *)(pimg_row_end + 1);
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
			b_j = (img_width - j) / 8;
		}

		if (b_j < 16) {
			int ii;

			for (ii = 0; ii < b_j; ii++)
				pimg_row[ii] = bcolor0;

			pimg_row += ii;
			j += ii << 3;

			if (j < img_width) {
				bmask = (0xFF << (8 - (img_width - j))) & 0xFF;
				src = pimg_row[0];
				pimg_row[0] = (src & ~bmask) | (color0 & bmask);
			}

			continue;
		}

		pimg_row_end = pimg_row + b_j - 1;
		dpimg = (mlib_d64 *)vis_alignaddr(pimg_row, 0);

		emask = vis_edge8(pimg_row, pimg_row_end);
		vis_pst_8(dcolor, dpimg++, emask);
		k = (mlib_addr)dpimg - (mlib_addr)pimg_row;
		for (; k < (b_j - 8); k += 8)
			(*dpimg++) = dcolor;
		emask = vis_edge8(dpimg, pimg_row_end);
		vis_pst_8(dcolor, dpimg, emask);
		j += b_j << 3;

		if (j < img_width) {
			pimg_row = (mlib_u8 *)(pimg_row_end + 1);
			bmask = (0xFF << (8 - (img_width - j))) & 0xFF;
			src = pimg_row[0];
			pimg_row[0] = (src & ~bmask) | (color0 & bmask);
		}
	}
}

/* *********************************************************** */

void
mlib_v_ImageClearEdge_U8_1(
    mlib_image *img,
    mlib_s32 dx,
    mlib_s32 dy,
    const mlib_s32 *color)
{
	mlib_u32 color0 = color[0] & 0xFF;

	VERT_EDGES1(mlib_u8);

	if (img_width < 16)
		HORIZ_EDGES(1, mlib_u8);

	color0 |= (color0 << 8);
	color0 |= (color0 << 16);
	dcolor = vis_to_double_dup(color0);
	for (i = 0; i < dy; i++) {
		mlib_u8 *pimg_row = pimg + i * img_stride,
		    *pimg_row_end = pimg_row + img_width - 1;
		dpimg = (mlib_d64 *)vis_alignaddr(pimg_row, 0);
		emask = vis_edge8(pimg_row, pimg_row_end);
		vis_pst_8(dcolor, dpimg++, emask);
		j = (mlib_addr)dpimg - (mlib_addr)pimg_row;
		for (; j < (img_width - 8); j += 8)
			(*dpimg++) = dcolor;
		emask = vis_edge8(dpimg, pimg_row_end);
		vis_pst_8(dcolor, dpimg, emask);

		pimg_row = pimg + (img_height - 1 - i) * img_stride;
		pimg_row_end = pimg_row + img_width - 1;
		dpimg = (mlib_d64 *)vis_alignaddr(pimg_row, 0);
		emask = vis_edge8(pimg_row, pimg_row_end);
		vis_pst_8(dcolor, dpimg++, emask);
		j = (mlib_addr)dpimg - (mlib_addr)pimg_row;
		for (; j < (img_width - 8); j += 8)
			(*dpimg++) = dcolor;
		emask = vis_edge8(dpimg, pimg_row_end);
		vis_pst_8(dcolor, dpimg, emask);
	}
}

/* *********************************************************** */

void
mlib_v_ImageClearEdge_U8_2(
    mlib_image *img,
    mlib_s32 dx,
    mlib_s32 dy,
    const mlib_s32 *color)
{
	mlib_u32 color0 = color[0] & 0xFF, color1 = color[1] & 0xFF;
	mlib_d64 dcolor0;

	VERT_EDGES2(mlib_u8);

	if (img_width < 8)
		HORIZ_EDGES(2, mlib_u8);

	color0 = (color0 << 8) | color1;
	color0 |= (color0 << 16);
	dcolor0 = vis_to_double_dup(color0);
	for (i = 0; i < dy; i++) {
		mlib_u8 *pimg_row = pimg + i * img_stride,
		    *pimg_row_end = pimg_row + img_width * 2 - 1;
		dpimg = (mlib_d64 *)vis_alignaddr(pimg_row, 0);
		emask = vis_edge8(pimg_row, pimg_row_end);
		dcolor = vis_faligndata(dcolor0, dcolor0);
		vis_pst_8(dcolor, dpimg++, emask);
		j = (mlib_addr)dpimg - (mlib_addr)pimg_row;
		for (; j < (img_width * 2 - 8); j += 8)
			(*dpimg++) = dcolor;
		emask = vis_edge8(dpimg, pimg_row_end);
		vis_pst_8(dcolor, dpimg, emask);

		pimg_row = pimg + (img_height - 1 - i) * img_stride;
		pimg_row_end = pimg_row + img_width * 2 - 1;
		dpimg = (mlib_d64 *)vis_alignaddr(pimg_row, 0);
		emask = vis_edge8(pimg_row, pimg_row_end);
		dcolor = vis_faligndata(dcolor0, dcolor0);
		vis_pst_8(dcolor, dpimg++, emask);
		j = (mlib_addr)dpimg - (mlib_addr)pimg_row;
		for (; j < (img_width * 2 - 8); j += 8)
			(*dpimg++) = dcolor;
		emask = vis_edge8(dpimg, pimg_row_end);
		vis_pst_8(dcolor, dpimg, emask);
	}
}

/* *********************************************************** */

void
mlib_v_ImageClearEdge_U8_3(
    mlib_image *img,
    mlib_s32 dx,
    mlib_s32 dy,
    const mlib_s32 *color)
{
	mlib_u32 color0 = color[0] & 0xFF,
	    color1 = color[1] & 0xFF, color2 = color[2] & 0xFF, col;
	mlib_d64 dcolor1, dcolor2, dcolor00, dcolor11, dcolor22;

	VERT_EDGES3(mlib_u8);

	if (img_width < 16)
		HORIZ_EDGES(3, mlib_u8);

	col = (color0 << 16) | (color1 << 8) | color2;
	color0 = (col << 8) | color0;
	color1 = (color0 << 8) | color1;
	color2 = (color1 << 8) | color2;
	dcolor = vis_to_double(color0, color1);
	dcolor1 = vis_to_double(color2, color0);
	dcolor2 = vis_to_double(color1, color2);
	for (i = 0; i < dy; i++) {
		mlib_u8 *pimg_row = pimg + i * img_stride,
		    *pimg_row_end = pimg_row + img_width * 3 - 1;
		dpimg = (mlib_d64 *)((mlib_addr)pimg_row & ~7);
		vis_alignaddr((void *)(-(mlib_addr)pimg_row), 8);
		dcolor22 = vis_faligndata(dcolor2, dcolor);
		dcolor00 = vis_faligndata(dcolor, dcolor1);
		dcolor11 = vis_faligndata(dcolor1, dcolor2);
		emask = vis_edge8(pimg_row, pimg_row_end);

		if ((mlib_addr)pimg_row & 7)
			vis_pst_8(dcolor22, dpimg++, emask);
		j = (mlib_addr)dpimg - (mlib_addr)pimg_row;
		for (; j < (img_width * 3 - 24); j += 24) {
			dpimg[0] = dcolor00;
			dpimg[1] = dcolor11;
			dpimg[2] = dcolor22;
			dpimg += 3;
		}

		if (j < (img_width * 3 - 8)) {
			(*dpimg++) = dcolor00;

			if (j < (img_width * 3 - 16)) {
				(*dpimg++) = dcolor11;
				dcolor00 = dcolor22;
			} else
				dcolor00 = dcolor11;
		}

		emask = vis_edge8(dpimg, pimg_row_end);
		vis_pst_8(dcolor00, dpimg, emask);

		pimg_row = pimg + (img_height - 1 - i) * img_stride;
		pimg_row_end = pimg_row + img_width * 3 - 1;
		dpimg = (mlib_d64 *)((mlib_addr)pimg_row & ~7);
		vis_alignaddr((void *)(-(mlib_addr)pimg_row), 8);
		dcolor22 = vis_faligndata(dcolor2, dcolor);
		dcolor00 = vis_faligndata(dcolor, dcolor1);
		dcolor11 = vis_faligndata(dcolor1, dcolor2);
		emask = vis_edge8(pimg_row, pimg_row_end);

		if ((mlib_addr)pimg_row & 7)
			vis_pst_8(dcolor22, dpimg++, emask);
		j = (mlib_addr)dpimg - (mlib_addr)pimg_row;
		for (; j < (img_width * 3 - 24); j += 24) {
			dpimg[0] = dcolor00;
			dpimg[1] = dcolor11;
			dpimg[2] = dcolor22;
			dpimg += 3;
		}

		if (j < (img_width * 3 - 8)) {
			(*dpimg++) = dcolor00;

			if (j < (img_width * 3 - 16)) {
				(*dpimg++) = dcolor11;
				dcolor00 = dcolor22;
			} else
				dcolor00 = dcolor11;
		}

		emask = vis_edge8(dpimg, pimg_row_end);
		vis_pst_8(dcolor00, dpimg, emask);
	}
}

/* *********************************************************** */

void
mlib_v_ImageClearEdge_U8_4(
    mlib_image *img,
    mlib_s32 dx,
    mlib_s32 dy,
    const mlib_s32 *color)
{
	mlib_u32 color0 = color[0] & 0xFF,
	    color1 = color[1] & 0xFF,
	    color2 = color[2] & 0xFF, color3 = color[3] & 0xFF;
	mlib_d64 dcolor0;

	VERT_EDGES4(mlib_u8);

	if (img_width < 4)
		HORIZ_EDGES(4, mlib_u8);

	color0 = (color0 << 24) | (color1 << 16) | (color2 << 8) | color3;
	dcolor0 = vis_to_double_dup(color0);
	for (i = 0; i < dy; i++) {
		mlib_u8 *pimg_row = pimg + i * img_stride,
		    *pimg_row_end = pimg_row + img_width * 4 - 1;
		dpimg = (mlib_d64 *)((mlib_addr)pimg_row & ~7);
		vis_alignaddr((void *)(-(mlib_addr)pimg_row), 8);
		emask = vis_edge8(pimg_row, pimg_row_end);
		dcolor = vis_faligndata(dcolor0, dcolor0);
		vis_pst_8(dcolor, dpimg++, emask);
		j = (mlib_addr)dpimg - (mlib_addr)pimg_row;
		for (; j < (img_width * 4 - 8); j += 8)
			(*dpimg++) = dcolor;
		emask = vis_edge8(dpimg, pimg_row_end);
		vis_pst_8(dcolor, dpimg, emask);

		pimg_row = pimg + (img_height - 1 - i) * img_stride;
		pimg_row_end = pimg_row + img_width * 4 - 1;
		dpimg = (mlib_d64 *)((mlib_addr)pimg_row & ~7);
		vis_alignaddr((void *)(-(mlib_addr)pimg_row), 8);
		emask = vis_edge8(pimg_row, pimg_row_end);
		dcolor = vis_faligndata(dcolor0, dcolor0);
		vis_pst_8(dcolor, dpimg++, emask);
		j = (mlib_addr)dpimg - (mlib_addr)pimg_row;
		for (; j < (img_width * 4 - 8); j += 8)
			(*dpimg++) = dcolor;
		emask = vis_edge8(dpimg, pimg_row_end);
		vis_pst_8(dcolor, dpimg, emask);
	}
}

/* *********************************************************** */

void
mlib_v_ImageClearEdge_S16_1(
    mlib_image *img,
    mlib_s32 dx,
    mlib_s32 dy,
    const mlib_s32 *color)
{
	mlib_u32 color0 = color[0] & 0xFFFF;

	VERT_EDGES1(mlib_s16);

	if (img_width < 8)
		HORIZ_EDGES(1, mlib_s16);

	color0 |= (color0 << 16);
	dcolor = vis_to_double_dup(color0);
	for (i = 0; i < dy; i++) {
		mlib_s16 *pimg_row = pimg + i * img_stride,
		    *pimg_row_end = pimg_row + img_width - 1;
		dpimg = (mlib_d64 *)vis_alignaddr(pimg_row, 0);
		emask = vis_edge16(pimg_row, pimg_row_end);
		vis_pst_16(dcolor, dpimg++, emask);
		j = (mlib_s16 *)dpimg - pimg_row;
		for (; j < (img_width - 4); j += 4)
			(*dpimg++) = dcolor;
		emask = vis_edge16(dpimg, pimg_row_end);
		vis_pst_16(dcolor, dpimg, emask);

		pimg_row = pimg + (img_height - 1 - i) * img_stride;
		pimg_row_end = pimg_row + img_width - 1;
		dpimg = (mlib_d64 *)vis_alignaddr(pimg_row, 0);
		emask = vis_edge16(pimg_row, pimg_row_end);
		vis_pst_16(dcolor, dpimg++, emask);
		j = (mlib_s16 *)dpimg - pimg_row;
		for (; j < (img_width - 4); j += 4)
			(*dpimg++) = dcolor;
		emask = vis_edge16(dpimg, pimg_row_end);
		vis_pst_16(dcolor, dpimg, emask);
	}
}

/* *********************************************************** */

void
mlib_v_ImageClearEdge_S16_2(
    mlib_image *img,
    mlib_s32 dx,
    mlib_s32 dy,
    const mlib_s32 *color)
{
	mlib_u32 color0 = color[0] & 0xFFFF, color1 = color[1] & 0xFFFF;
	mlib_d64 dcolor0;

	VERT_EDGES2(mlib_s16);

	if (img_width < 4)
		HORIZ_EDGES(2, mlib_s16);

	color0 = (color0 << 16) | color1;
	dcolor0 = vis_to_double_dup(color0);
	for (i = 0; i < dy; i++) {
		mlib_s16 *pimg_row = pimg + i * img_stride,
		    *pimg_row_end = pimg_row + img_width * 2 - 1;
		dpimg = (mlib_d64 *)vis_alignaddr(pimg_row, 0);
		emask = vis_edge16(pimg_row, pimg_row_end);
		dcolor = vis_faligndata(dcolor0, dcolor0);
		vis_pst_16(dcolor, dpimg++, emask);
		j = (mlib_s16 *)dpimg - pimg_row;
		for (; j < (img_width * 2 - 4); j += 4)
			(*dpimg++) = dcolor;
		emask = vis_edge16(dpimg, pimg_row_end);
		vis_pst_16(dcolor, dpimg, emask);

		pimg_row = pimg + (img_height - 1 - i) * img_stride;
		pimg_row_end = pimg_row + img_width * 2 - 1;
		dpimg = (mlib_d64 *)vis_alignaddr(pimg_row, 0);
		emask = vis_edge16(pimg_row, pimg_row_end);
		dcolor = vis_faligndata(dcolor0, dcolor0);
		vis_pst_16(dcolor, dpimg++, emask);
		j = (mlib_s16 *)dpimg - pimg_row;
		for (; j < (img_width * 2 - 4); j += 4)
			(*dpimg++) = dcolor;
		emask = vis_edge16(dpimg, pimg_row_end);
		vis_pst_16(dcolor, dpimg, emask);
	}
}

/* *********************************************************** */

void
mlib_v_ImageClearEdge_S16_3(
    mlib_image *img,
    mlib_s32 dx,
    mlib_s32 dy,
    const mlib_s32 *color)
{
	mlib_u32 color0 = color[0] & 0xFFFF,
	    color1 = color[1] & 0xFFFF,
	    color2 = color[2] & 0xFFFF, col0, col1, col2;
	mlib_d64 dcolor1, dcolor2, dcolor00, dcolor11, dcolor22;

	VERT_EDGES3(mlib_s16);

	if (img_width < 8)
		HORIZ_EDGES(3, mlib_s16);

	col0 = (color0 << 16) | color1;
	col1 = (color2 << 16) | color0;
	col2 = (color1 << 16) | color2;
	dcolor = vis_to_double(col0, col1);
	dcolor1 = vis_to_double(col2, col0);
	dcolor2 = vis_to_double(col1, col2);
	for (i = 0; i < dy; i++) {
		mlib_s16 *pimg_row = pimg + i * img_stride,
		    *pimg_row_end = pimg_row + img_width * 3 - 1;
		dpimg = (mlib_d64 *)((mlib_addr)pimg_row & ~7);
		vis_alignaddr((void *)(-(mlib_addr)pimg_row), 8);
		dcolor22 = vis_faligndata(dcolor2, dcolor);
		dcolor00 = vis_faligndata(dcolor, dcolor1);
		dcolor11 = vis_faligndata(dcolor1, dcolor2);
		emask = vis_edge16(pimg_row, pimg_row_end);

		if ((mlib_addr)pimg_row & 7)
			vis_pst_16(dcolor22, dpimg++, emask);
		j = (mlib_s16 *)dpimg - pimg_row;
		for (; j < (img_width * 3 - 12); j += 12) {
			dpimg[0] = dcolor00;
			dpimg[1] = dcolor11;
			dpimg[2] = dcolor22;
			dpimg += 3;
		}

		if (j < (img_width * 3 - 4)) {
			(*dpimg++) = dcolor00;

			if (j < (img_width * 3 - 8)) {
				(*dpimg++) = dcolor11;
				dcolor00 = dcolor22;
			} else
				dcolor00 = dcolor11;
		}

		emask = vis_edge16(dpimg, pimg_row_end);
		vis_pst_16(dcolor00, dpimg, emask);

		pimg_row = pimg + (img_height - 1 - i) * img_stride;
		pimg_row_end = pimg_row + img_width * 3 - 1;
		dpimg = (mlib_d64 *)((mlib_addr)pimg_row & ~7);
		vis_alignaddr((void *)(-(mlib_addr)pimg_row), 8);
		dcolor22 = vis_faligndata(dcolor2, dcolor);
		dcolor00 = vis_faligndata(dcolor, dcolor1);
		dcolor11 = vis_faligndata(dcolor1, dcolor2);
		emask = vis_edge16(pimg_row, pimg_row_end);

		if ((mlib_addr)pimg_row & 7)
			vis_pst_16(dcolor22, dpimg++, emask);
		j = (mlib_s16 *)dpimg - pimg_row;
		for (; j < (img_width * 3 - 12); j += 12) {
			dpimg[0] = dcolor00;
			dpimg[1] = dcolor11;
			dpimg[2] = dcolor22;
			dpimg += 3;
		}

		if (j < (img_width * 3 - 4)) {
			(*dpimg++) = dcolor00;

			if (j < (img_width * 3 - 8)) {
				(*dpimg++) = dcolor11;
				dcolor00 = dcolor22;
			} else
				dcolor00 = dcolor11;
		}

		emask = vis_edge16(dpimg, pimg_row_end);
		vis_pst_16(dcolor00, dpimg, emask);
	}
}

/* *********************************************************** */

void
mlib_v_ImageClearEdge_S16_4(
    mlib_image *img,
    mlib_s32 dx,
    mlib_s32 dy,
    const mlib_s32 *color)
{
	mlib_u32 color0 = color[0] & 0xFFFF,
	    color1 = color[1] & 0xFFFF,
	    color2 = color[2] & 0xFFFF, color3 = color[3] & 0xFFFF;
	mlib_d64 dcolor0;

	VERT_EDGES4(mlib_s16);

	if (img_width < 4)
		HORIZ_EDGES(4, mlib_s16);

	color0 = (color0 << 16) | color1;
	color1 = (color2 << 16) | color3;
	dcolor0 = vis_to_double(color0, color1);
	for (i = 0; i < dy; i++) {
		mlib_s16 *pimg_row = pimg + i * img_stride,
		    *pimg_row_end = pimg_row + img_width * 4 - 1;
		dpimg = (mlib_d64 *)((mlib_addr)pimg_row & ~7);
		vis_alignaddr((void *)(-(mlib_addr)pimg_row), 8);
		emask = vis_edge16(pimg_row, pimg_row_end);
		dcolor = vis_faligndata(dcolor0, dcolor0);
		vis_pst_16(dcolor, dpimg++, emask);
		j = (mlib_s16 *)dpimg - pimg_row;
		for (; j < (img_width * 4 - 4); j += 4)
			(*dpimg++) = dcolor;
		emask = vis_edge16(dpimg, pimg_row_end);
		vis_pst_16(dcolor, dpimg, emask);

		pimg_row = pimg + (img_height - 1 - i) * img_stride;
		pimg_row_end = pimg_row + img_width * 4 - 1;
		dpimg = (mlib_d64 *)((mlib_addr)pimg_row & ~7);
		vis_alignaddr((void *)(-(mlib_addr)pimg_row), 8);
		emask = vis_edge16(pimg_row, pimg_row_end);
		dcolor = vis_faligndata(dcolor0, dcolor0);
		vis_pst_16(dcolor, dpimg++, emask);
		j = (mlib_s16 *)dpimg - pimg_row;
		for (; j < (img_width * 4 - 4); j += 4)
			(*dpimg++) = dcolor;
		emask = vis_edge16(dpimg, pimg_row_end);
		vis_pst_16(dcolor, dpimg, emask);
	}
}

/* *********************************************************** */

void
mlib_v_ImageClearEdge_S32_1(
    mlib_image *img,
    mlib_s32 dx,
    mlib_s32 dy,
    const mlib_s32 *color)
{
	mlib_u32 color0 = color[0];

	VERT_EDGES1(mlib_s32);

	if (img_width < 4)
		HORIZ_EDGES(1, mlib_s32);

	dcolor = vis_to_double_dup(color0);
	for (i = 0; i < dy; i++) {
		mlib_s32 *pimg_row = pimg + i * img_stride,
		    *pimg_row_end = pimg_row + img_width - 1;
		dpimg = (mlib_d64 *)vis_alignaddr(pimg_row, 0);
		emask = vis_edge32(pimg_row, pimg_row_end);
		vis_pst_32(dcolor, dpimg++, emask);
		j = (mlib_s32 *)dpimg - pimg_row;
		for (; j < (img_width - 2); j += 2)
			(*dpimg++) = dcolor;
		emask = vis_edge32(dpimg, pimg_row_end);
		vis_pst_32(dcolor, dpimg, emask);

		pimg_row = pimg + (img_height - 1 - i) * img_stride;
		pimg_row_end = pimg_row + img_width - 1;
		dpimg = (mlib_d64 *)vis_alignaddr(pimg_row, 0);
		emask = vis_edge32(pimg_row, pimg_row_end);
		vis_pst_32(dcolor, dpimg++, emask);
		j = (mlib_s32 *)dpimg - pimg_row;
		for (; j < (img_width - 2); j += 2)
			(*dpimg++) = dcolor;
		emask = vis_edge32(dpimg, pimg_row_end);
		vis_pst_32(dcolor, dpimg, emask);
	}
}

/* *********************************************************** */

void
mlib_v_ImageClearEdge_S32_2(
    mlib_image *img,
    mlib_s32 dx,
    mlib_s32 dy,
    const mlib_s32 *color)
{
	mlib_u32 color0 = color[0], color1 = color[1];
	mlib_d64 dcolor0;

	VERT_EDGES2(mlib_s32);

	if (img_width < 2)
		HORIZ_EDGES(2, mlib_s32);

	dcolor0 = vis_to_double(color0, color1);
	for (i = 0; i < dy; i++) {
		mlib_s32 *pimg_row = pimg + i * img_stride,
		    *pimg_row_end = pimg_row + img_width * 2 - 1;
		dpimg = (mlib_d64 *)vis_alignaddr(pimg_row, 0);
		emask = vis_edge32(pimg_row, pimg_row_end);
		dcolor = vis_faligndata(dcolor0, dcolor0);
		vis_pst_32(dcolor, dpimg++, emask);
		j = (mlib_s32 *)dpimg - pimg_row;
		for (; j < (img_width * 2 - 2); j += 2)
			(*dpimg++) = dcolor;
		emask = vis_edge32(dpimg, pimg_row_end);
		vis_pst_32(dcolor, dpimg, emask);

		pimg_row = pimg + (img_height - 1 - i) * img_stride;
		pimg_row_end = pimg_row + img_width * 2 - 1;
		dpimg = (mlib_d64 *)vis_alignaddr(pimg_row, 0);
		emask = vis_edge32(pimg_row, pimg_row_end);
		dcolor = vis_faligndata(dcolor0, dcolor0);
		vis_pst_32(dcolor, dpimg++, emask);
		j = (mlib_s32 *)dpimg - pimg_row;
		for (; j < (img_width * 2 - 2); j += 2)
			(*dpimg++) = dcolor;
		emask = vis_edge32(dpimg, pimg_row_end);
		vis_pst_32(dcolor, dpimg, emask);
	}
}

/* *********************************************************** */

void
mlib_v_ImageClearEdge_S32_3(
    mlib_image *img,
    mlib_s32 dx,
    mlib_s32 dy,
    const mlib_s32 *color)
{
	mlib_u32 color0 = color[0], color1 = color[1], color2 = color[2];
	mlib_d64 dcolor1, dcolor2, dcolor00, dcolor11, dcolor22;

	VERT_EDGES3(mlib_s32);

	if (img_width < 2)
		HORIZ_EDGES(3, mlib_s32);

	dcolor = vis_to_double(color0, color1);
	dcolor1 = vis_to_double(color2, color0);
	dcolor2 = vis_to_double(color1, color2);
	for (i = 0; i < dy; i++) {
		mlib_s32 *pimg_row = pimg + i * img_stride,
		    *pimg_row_end = pimg_row + img_width * 3 - 1;
		dpimg = (mlib_d64 *)((mlib_addr)pimg_row & ~7);
		vis_alignaddr((void *)(-(mlib_addr)pimg_row), 8);
		dcolor22 = vis_faligndata(dcolor2, dcolor);
		dcolor00 = vis_faligndata(dcolor, dcolor1);
		dcolor11 = vis_faligndata(dcolor1, dcolor2);
		emask = vis_edge32(pimg_row, pimg_row_end);

		if ((mlib_addr)pimg_row & 7)
			vis_pst_32(dcolor22, dpimg++, emask);
		j = (mlib_s32 *)dpimg - pimg_row;
		for (; j < (img_width * 3 - 6); j += 6) {
			dpimg[0] = dcolor00;
			dpimg[1] = dcolor11;
			dpimg[2] = dcolor22;
			dpimg += 3;
		}

		if (j < (img_width * 3 - 2)) {
			(*dpimg++) = dcolor00;

			if (j < (img_width * 3 - 4)) {
				(*dpimg++) = dcolor11;
				dcolor00 = dcolor22;
			} else
				dcolor00 = dcolor11;
		}

		emask = vis_edge32(dpimg, pimg_row_end);
		vis_pst_32(dcolor00, dpimg, emask);

		pimg_row = pimg + (img_height - 1 - i) * img_stride;
		pimg_row_end = pimg_row + img_width * 3 - 1;
		dpimg = (mlib_d64 *)((mlib_addr)pimg_row & ~7);
		vis_alignaddr((void *)(-(mlib_addr)pimg_row), 8);
		dcolor22 = vis_faligndata(dcolor2, dcolor);
		dcolor00 = vis_faligndata(dcolor, dcolor1);
		dcolor11 = vis_faligndata(dcolor1, dcolor2);
		emask = vis_edge32(pimg_row, pimg_row_end);

		if ((mlib_addr)pimg_row & 7)
			vis_pst_32(dcolor22, dpimg++, emask);
		j = (mlib_s32 *)dpimg - pimg_row;
		for (; j < (img_width * 3 - 6); j += 6) {
			dpimg[0] = dcolor00;
			dpimg[1] = dcolor11;
			dpimg[2] = dcolor22;
			dpimg += 3;
		}

		if (j < (img_width * 3 - 2)) {
			(*dpimg++) = dcolor00;

			if (j < (img_width * 3 - 4)) {
				(*dpimg++) = dcolor11;
				dcolor00 = dcolor22;
			} else
				dcolor00 = dcolor11;
		}

		emask = vis_edge32(dpimg, pimg_row_end);
		vis_pst_32(dcolor00, dpimg, emask);
	}
}

/* *********************************************************** */

void
mlib_v_ImageClearEdge_S32_4(
    mlib_image *img,
    mlib_s32 dx,
    mlib_s32 dy,
    const mlib_s32 *color)
{
	mlib_u32 color0 = color[0],
	    color1 = color[1], color2 = color[2], color3 = color[3];
	mlib_d64 dcolor0, dcolor00, dcolor0_, dcolor00_, dcolor1;

	VERT_EDGES4(mlib_s32);

	if (img_width < 2)
		HORIZ_EDGES(4, mlib_s32);

	dcolor0 = vis_to_double(color2, color3);
	dcolor00 = vis_to_double(color0, color1);
	vis_alignaddr((void *)0, 4);
	dcolor0_ = vis_faligndata(dcolor0, dcolor00);
	dcolor00_ = vis_faligndata(dcolor00, dcolor0);
	for (i = 0; i < dy; i++) {
		mlib_s32 *pimg_row = pimg + i * img_stride,
		    *pimg_row_end = pimg_row + img_width * 4 - 1;
		dpimg = (mlib_d64 *)((mlib_addr)pimg_row & ~7);
		vis_alignaddr((void *)(-(mlib_addr)pimg_row), 4);
		emask = vis_edge32(pimg_row, pimg_row_end);
		dcolor = vis_faligndata(dcolor0_, dcolor00_);
		dcolor1 = vis_faligndata(dcolor00_, dcolor0_);
		vis_pst_32(dcolor, dpimg++, emask);
		(*dpimg++) = dcolor1;
		j = (mlib_s32 *)dpimg - pimg_row;
		for (; j <= (img_width * 4 - 4); j += 4) {
			dpimg[0] = dcolor;
			dpimg[1] = dcolor1;
			dpimg += 2;
		}

		if (j < (img_width * 4)) {
			emask = vis_edge32(dpimg, pimg_row_end);
			vis_pst_32(dcolor, dpimg, emask);
		}

		pimg_row = pimg + (img_height - 1 - i) * img_stride;
		pimg_row_end = pimg_row + img_width * 4 - 1;
		dpimg = (mlib_d64 *)((mlib_addr)pimg_row & ~7);
		vis_alignaddr((void *)(-(mlib_addr)pimg_row), 4);
		emask = vis_edge32(pimg_row, pimg_row_end);
		dcolor = vis_faligndata(dcolor0_, dcolor00_);
		dcolor1 = vis_faligndata(dcolor00_, dcolor0_);
		vis_pst_32(dcolor, dpimg++, emask);
		(*dpimg++) = dcolor1;
		j = (mlib_s32 *)dpimg - pimg_row;
		for (; j <= (img_width * 4 - 4); j += 4) {
			dpimg[0] = dcolor;
			dpimg[1] = dcolor1;
			dpimg += 2;
		}

		if (j < (img_width * 4)) {
			emask = vis_edge32(dpimg, pimg_row_end);
			vis_pst_32(dcolor, dpimg, emask);
		}
	}
}

/* *********************************************************** */
